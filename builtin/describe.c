#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "lockfile.h"
#include "commit.h"
#include "tag.h"
#include "refs.h"
#include "object-name.h"
#include "parse-options.h"
#include "read-cache-ll.h"
#include "revision.h"
#include "diff.h"
#include "hashmap.h"
#include "setup.h"
#include "strvec.h"
#include "run-command.h"
#include "odb.h"
#include "list-objects.h"
#include "commit-slab.h"
#include "wildmatch.h"
#include "prio-queue.h"
#include "oidset.h"

#define MAX_TAGS	(FLAG_BITS - 1)
#define DEFAULT_CANDIDATES 10

define_commit_slab(commit_names, struct commit_name *);

static const char * const describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	NULL
};

static số_nguyên debug;	/* Display lots of verbose info */
static số_nguyên all;	/* Any valid ref can be used */
static số_nguyên tags;	/* Allow lightweight tags */
static số_nguyên longformat;
static số_nguyên first_parent;
static số_nguyên abbrev = -1; /* unspecified */
static số_nguyên max_candidates = DEFAULT_CANDIDATES;
static struct hashmap names;
static số_nguyên have_util;
static struct string_list patterns = STRING_LIST_INIT_NODUP;
static struct string_list exclude_patterns = STRING_LIST_INIT_NODUP;
static số_nguyên always;
static const char *suffix, *dirty, *broken;
static struct commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
static const char *diff_index_args[] = {
	"diff-index", "--quiet", "HEAD", "--", NULL
};

static const char *update_index_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", NULL
};

struct commit_name {
	struct hashmap_entry entry;
	struct object_id peeled;
	struct tag *tag;
	unsigned prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	unsigned name_checked:1;
	unsigned misnamed:1;
	struct object_id oid;
	char *path;
};

static const char *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

static số_nguyên commit_name_neq(const void *cmp_data UNUSED,
			   const struct hashmap_entry *eptr,
			   const struct hashmap_entry *entry_or_key,
			   const void *peeled)
{
	const struct commit_name *cn1, *cn2;

	cn1 = container_of(eptr, const struct commit_name, entry);
	cn2 = container_of(entry_or_key, const struct commit_name, entry);

	trả_về !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

static inline struct commit_name *find_commit_name(const struct object_id *peeled)
{
	trả_về hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						struct commit_name, entry);
}

static số_nguyên replace_name(struct commit_name *e,
			       số_nguyên prio,
			       const struct object_id *oid,
			       struct tag **tag)
{
	nếu (!e || e->prio < prio)
		trả_về 1;

	nếu (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		struct tag *t;

		nếu (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			nếu (!t || parse_tag(the_repository, t))
				trả_về 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		nếu (!t || parse_tag(the_repository, t))
			trả_về 0;
		*tag = t;

		nếu (e->tag->date < t->date)
			trả_về 1;
	}

	trả_về 0;
}

static void add_to_known_names(const char *path,
			       const struct object_id *peeled,
			       số_nguyên prio,
			       const struct object_id *oid)
{
	struct commit_name *e = find_commit_name(peeled);
	struct tag *tag = NULL;
	nếu (replace_name(e, prio, oid, &tag)) {
		nếu (!e) {
			e = xmalloc(sizeof(struct commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_entry_init(&e->entry, oidhash(peeled));
			hashmap_add(&names, &e->entry);
			e->path = NULL;
		}
		e->tag = tag;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		free(e->path);
		e->path = xstrdup(path);
	}
}

static số_nguyên get_name(const struct reference *ref, void *cb_data UNUSED)
{
	số_nguyên is_tag = 0;
	struct object_id peeled;
	số_nguyên is_annotated, prio;
	const char *path_to_match = NULL;

	nếu (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} không_thì nếu (all) {
		nếu ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			trả_về 0;
		}
	} không_thì {
		/* Reject anything outside refs/tags/ unless --all */
		trả_về 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	nếu (exclude_patterns.nr) {
		struct string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			nếu (!wildmatch(item->string, path_to_match, 0))
				trả_về 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	nếu (patterns.nr) {
		số_nguyên found = 0;
		struct string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			nếu (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				break;
			}
		}

		nếu (!found)
			trả_về 0;
	}

	/* Is it annotated? */
	nếu (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} không_thì {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	nếu (is_annotated)
		prio = 2;
	không_thì nếu (is_tag)
		prio = 1;
	không_thì
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	trả_về 0;
}

struct possible_tag {
	struct commit_name *name;
	số_nguyên depth;
	số_nguyên found_order;
	unsigned flag_within;
};

static số_nguyên compare_pt(const void *a_, const void *b_)
{
	struct possible_tag *a = (struct possible_tag *)a_;
	struct possible_tag *b = (struct possible_tag *)b_;
	nếu (a->depth != b->depth)
		trả_về a->depth - b->depth;
	nếu (a->found_order != b->found_order)
		trả_về a->found_order - b->found_order;
	trả_về 0;
}

static unsigned long finish_depth_computation(struct prio_queue *queue,
					      struct possible_tag *best)
{
	unsigned long seen_commits = 0;
	struct oidset unflagged = OIDSET_INIT;
	struct commit *c;

	prio_queue_for_each(queue, c) {
		nếu (!(c->object.flags & best->flag_within))
			oidset_insert(&unflagged, &c->object.oid);
	}

	trong_khi ((c = prio_queue_get(queue))) {
		struct commit_list *parents = c->parents;
		seen_commits++;
		nếu (c->object.flags & best->flag_within) {
			nếu (!oidset_size(&unflagged))
				break;
		} không_thì {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		trong_khi (parents) {
			unsigned seen, flag_before, flag_after;
			struct commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			nếu (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			nếu (!seen && !flag_after)
				oidset_insert(&unflagged, &p->object.oid);
			nếu (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->next;
		}
	}
	oidset_clear(&unflagged);
	trả_về seen_commits;
}

static void append_name(struct commit_name *n, struct strbuf *dst)
{
	nếu (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		nếu (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	nếu (n->tag && !n->name_checked) {
		nếu (strcmp(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	nếu (n->tag) {
		nếu (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} không_thì {
		strbuf_addstr(dst, n->path);
	}
}

static void append_suffix(số_nguyên depth, const struct object_id *oid, struct strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

static void describe_commit(struct commit *cmit, struct strbuf *dst)
{
	struct commit *c, *gave_up_on = NULL;
	struct prio_queue queue = { compare_commits_by_commit_date };
	struct commit_name *n;
	struct possible_tag all_matches[MAX_TAGS];
	unsigned số_nguyên match_cnt = 0, annotated_cnt = 0, cur_match;
	unsigned long seen_commits = 0;
	unsigned số_nguyên unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	nếu (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		nếu (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		nếu (suffix)
			strbuf_addstr(dst, suffix);
		trả_về;
	}

	nếu (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	nếu (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	nếu (!have_util) {
		struct hashmap_iter iter;
		struct commit *c;
		struct commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			nếu (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	trong_khi ((c = prio_queue_get(&queue))) {
		struct commit_list *parents = c->parents;
		struct commit_name **slot;

		seen_commits++;

		nếu (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_size(&names)) {
			gave_up_on = c;
			break;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : NULL;
		nếu (n) {
			nếu (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} không_thì nếu (match_cnt < max_candidates) {
				struct possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				nếu (n->prio == 2)
					annotated_cnt++;
			}
		}
		cho (cur_match = 0; cur_match < match_cnt; cur_match++) {
			struct possible_tag *t = &all_matches[cur_match];
			nếu (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		nếu (annotated_cnt && !prio_queue_size(&queue)) {
			số_nguyên best_depth = INT_MAX;
			unsigned best_within = 0;
			cho (cur_match = 0; cur_match < match_cnt; cur_match++) {
				struct possible_tag *t = &all_matches[cur_match];
				nếu (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} không_thì nếu (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			nếu ((c->object.flags & best_within) == best_within) {
				nếu (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				break;
			}
		}
		trong_khi (parents) {
			struct commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			nếu (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->next;

			nếu (first_parent)
				break;
		}
	}

	nếu (!match_cnt) {
		struct object_id *cmit_oid = &cmit->object.oid;
		nếu (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			nếu (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			trả_về;
		}
		nếu (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		không_thì
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	nếu (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	nếu (debug) {
		static số_nguyên label_width = -1;
		nếu (label_width < 0) {
			số_nguyên i, w;
			cho (i = 0; i < ARRAY_SIZE(prio_names); i++) {
				w = strlen(_(prio_names[i]));
				nếu (label_width < w)
					label_width = w;
			}
		}
		cho (cur_match = 0; cur_match < match_cnt; cur_match++) {
			struct possible_tag *t = &all_matches[cur_match];
			fprintf(stderr, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		fprintf(stderr, _("traversed %lu commits\n"), seen_commits);
		nếu (gave_up_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	nếu (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	nếu (suffix)
		strbuf_addstr(dst, suffix);
}

struct process_commit_data {
	struct commit *current_commit;
	const struct object_id *looking_for;
	struct strbuf *dst;
	struct rev_info *revs;
};

static void process_commit(struct commit *commit, void *data)
{
	struct process_commit_data *pcd = data;
	pcd->current_commit = commit;
}

static void process_object(struct object *obj, const char *path, void *data)
{
	struct process_commit_data *pcd = data;

	nếu (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		nếu (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = NULL;
	}
}

static void describe_blob(const struct object_id *oid, struct strbuf *dst)
{
	struct rev_info revs;
	struct strvec args = STRVEC_INIT;
	struct object_id head_oid;
	struct process_commit_data pcd = { NULL, oid, dst, &revs};

	nếu (repo_get_oid(the_repository, "HEAD", &head_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&head_oid),
		     NULL);

	repo_init_revisions(the_repository, &revs, NULL);
	setup_revisions_from_strvec(&args, &revs, NULL);
	nếu (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	nếu (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	nếu (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

static void describe(const char *arg, số_nguyên last_one)
{
	struct object_id oid;
	struct commit *cmit;
	struct strbuf sb = STRBUF_INIT;

	nếu (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	nếu (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	nếu (cmit)
		describe_commit(cmit, &sb);
	không_thì nếu (odb_read_object_info(the_repository->objects,
				      &oid, NULL) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	không_thì
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	nếu (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

static số_nguyên option_parse_exact_match(const struct option *opt, const char *arg,
				    số_nguyên unset)
{
	số_nguyên *val = opt->value;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	trả_về 0;
}

số_nguyên cmd_describe(số_nguyên argc,
		 const char **argv,
		 const char *prefix,
		 struct repository *repo UNUSED )
{
	struct refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	số_nguyên contains = 0;
	struct option options[] = {
		OPT_BOOL(0, "contains",   &contains, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &tags, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, NULL,
			       N_("only output exact matches"),
			       PARSE_OPT_NOARG, option_parse_exact_match),
		OPT_INTEGER(0, "candidates", &max_candidates,
			    N_("consider <n> most recent tags (default: 10)")),
		OPT_STRING_LIST(0, "match", &patterns, N_("pattern"),
			   N_("only consider tags matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &exclude_patterns, N_("pattern"),
			   N_("do not consider tags matching <pattern>")),
		OPT_BOOL(0, "always",        &always,
			N_("show abbreviated commit object as fallback")),
		{
			.type = OPTION_STRING,
			.long_name = "dirty",
			.value = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.value = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, NULL);
	argc = parse_options(argc, argv, prefix, options, describe_usage, 0);
	nếu (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	nếu (max_candidates < 0)
		max_candidates = 0;
	không_thì nếu (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_buffer = 0;

	nếu (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	nếu (contains) {
		struct string_list_item *item;
		struct strvec args;
		const char **argv_copy;
		số_nguyên ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     NULL);
		nếu (always)
			strvec_push(&args, "--always");
		nếu (!all)
			strvec_push(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		nếu (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		nếu (argc)
			strvec_pushv(&args, argv);
		không_thì
			strvec_push(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		cho (size_t i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = NULL;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		free(argv_copy);
		trả_về ret;
	}

	nếu (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, NULL, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, NULL, &for_each_ref_opts);
	nếu (!hashmap_get_size(&names) && !always)
		die(_("No names found, cannot describe anything."));

	nếu (argc == 0) {
		nếu (broken) {
			struct child_process cp = CHILD_PROCESS_INIT;

			strvec_pushv(&cp.args, update_index_args);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			run_command(&cp);

			child_process_init(&cp);
			strvec_pushv(&cp.args, diff_index_args);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			nếu (!dirty)
				dirty = "-dirty";

			switch (run_command(&cp)) {
			case 0:
				suffix = NULL;
				break;
			case 1:
				suffix = dirty;
				break;
			default:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} không_thì nếu (dirty) {
			struct lock_file index_lock = LOCK_INIT;
			struct rev_info revs;
			số_nguyên fd;

			setup_work_tree(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			repo_read_index(the_repository);
			refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED,
				      NULL, NULL, NULL);
			fd = repo_hold_locked_index(the_repository,
						    &index_lock, 0);
			nếu (0 <= fd)
				repo_update_index_if_able(the_repository, &index_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			nếu (setup_revisions(ARRAY_SIZE(diff_index_args) - 1,
					    diff_index_args, &revs, NULL) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_index(&revs, 0);

			nếu (!diff_result_code(&revs))
				suffix = NULL;
			không_thì
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} không_thì nếu (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} không_thì nếu (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} không_thì {
		trong_khi (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	trả_về 0;
}
