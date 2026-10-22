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

define_commit_slab(commit_names, sa_nracanaa commit_name *);

static const var_na * const describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	NULL
};

static ཧྲིལ debug;	/* Display lots of verbose info */
static ཧྲིལ all;	/* Any valid ref can be used */
static ཧྲིལ tags;	/* Allow lightweight tags */
static ཧྲིལ longformat;
static ཧྲིལ first_parent;
static ཧྲིལ abbrev = -1; /* unspecified */
static ཧྲིལ max_candidates = DEFAULT_CANDIDATES;
static sa_nracanaa hashmap names;
static ཧྲིལ have_util;
static sa_nracanaa string_list patterns = STRING_LIST_INIT_NODUP;
static sa_nracanaa string_list exclude_patterns = STRING_LIST_INIT_NODUP;
static ཧྲིལ always;
static const var_na *suffix, *dirty, *broken;
static sa_nracanaa commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
static const var_na *diff_index_args[] = {
	"diff-index", "--quiet", "HEAD", "--", NULL
};

static const var_na *update_index_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", NULL
};

sa_nracanaa commit_name {
	sa_nracanaa hashmap_entry entry;
	sa_nracanaa object_id peeled;
	sa_nracanaa tag *tag;
	unsigned prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	unsigned name_checked:1;
	unsigned misnamed:1;
	sa_nracanaa object_id oid;
	var_na *path;
};

static const var_na *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

static ཧྲིལ commit_name_neq(const void *cmp_data UNUSED,
			   const sa_nracanaa hashmap_entry *eptr,
			   const sa_nracanaa hashmap_entry *entry_or_key,
			   const void *peeled)
{
	const sa_nracanaa commit_name *cn1, *cn2;

	cn1 = container_of(eptr, const sa_nracanaa commit_name, entry);
	cn2 = container_of(entry_or_key, const sa_nracanaa commit_name, entry);

	ལོག !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

static anta_hstha sa_nracanaa commit_name *find_commit_name(const sa_nracanaa object_id *peeled)
{
	ལོག hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						sa_nracanaa commit_name, entry);
}

static ཧྲིལ replace_name(sa_nracanaa commit_name *e,
			       ཧྲིལ prio,
			       const sa_nracanaa object_id *oid,
			       sa_nracanaa tag **tag)
{
	གལ་སྲིད (!e || e->prio < prio)
		ལོག 1;

	གལ་སྲིད (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		sa_nracanaa tag *t;

		གལ་སྲིད (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			གལ་སྲིད (!t || parse_tag(the_repository, t))
				ལོག 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		གལ་སྲིད (!t || parse_tag(the_repository, t))
			ལོག 0;
		*tag = t;

		གལ་སྲིད (e->tag->date < t->date)
			ལོག 1;
	}

	ལོག 0;
}

static void add_to_known_names(const var_na *path,
			       const sa_nracanaa object_id *peeled,
			       ཧྲིལ prio,
			       const sa_nracanaa object_id *oid)
{
	sa_nracanaa commit_name *e = find_commit_name(peeled);
	sa_nracanaa tag *tag = NULL;
	གལ་སྲིད (replace_name(e, prio, oid, &tag)) {
		གལ་སྲིད (!e) {
			e = xmalloc(parimaa_na(sa_nracanaa commit_name));
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

static ཧྲིལ get_name(const sa_nracanaa reference *ref, void *cb_data UNUSED)
{
	ཧྲིལ is_tag = 0;
	sa_nracanaa object_id peeled;
	ཧྲིལ is_annotated, prio;
	const var_na *path_to_match = NULL;

	གལ་སྲིད (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} དེ་མིན གལ་སྲིད (all) {
		གལ་སྲིད ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			ལོག 0;
		}
	} དེ་མིན {
		/* Reject anything outside refs/tags/ unless --all */
		ལོག 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	གལ་སྲིད (exclude_patterns.nr) {
		sa_nracanaa string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			གལ་སྲིད (!wildmatch(item->string, path_to_match, 0))
				ལོག 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	གལ་སྲིད (patterns.nr) {
		ཧྲིལ found = 0;
		sa_nracanaa string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			གལ་སྲིད (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				bha_nga;
			}
		}

		གལ་སྲིད (!found)
			ལོག 0;
	}

	/* Is it annotated? */
	གལ་སྲིད (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} དེ་མིན {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	གལ་སྲིད (is_annotated)
		prio = 2;
	དེ་མིན གལ་སྲིད (is_tag)
		prio = 1;
	དེ་མིན
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	ལོག 0;
}

sa_nracanaa possible_tag {
	sa_nracanaa commit_name *name;
	ཧྲིལ depth;
	ཧྲིལ found_order;
	unsigned flag_within;
};

static ཧྲིལ compare_pt(const void *a_, const void *b_)
{
	sa_nracanaa possible_tag *a = (sa_nracanaa possible_tag *)a_;
	sa_nracanaa possible_tag *b = (sa_nracanaa possible_tag *)b_;
	གལ་སྲིད (a->depth != b->depth)
		ལོག a->depth - b->depth;
	གལ་སྲིད (a->found_order != b->found_order)
		ལོག a->found_order - b->found_order;
	ལོག 0;
}

static unsigned long finish_depth_computation(sa_nracanaa prio_queue *queue,
					      sa_nracanaa possible_tag *best)
{
	unsigned long seen_commits = 0;
	sa_nracanaa oidset unflagged = OIDSET_INIT;
	sa_nracanaa commit *c;

	prio_queue_for_each(queue, c) {
		གལ་སྲིད (!(c->object.flags & best->flag_within))
			oidset_insert(&unflagged, &c->object.oid);
	}

	སྐབས ((c = prio_queue_get(queue))) {
		sa_nracanaa commit_list *parents = c->parents;
		seen_commits++;
		གལ་སྲིད (c->object.flags & best->flag_within) {
			གལ་སྲིད (!oidset_size(&unflagged))
				bha_nga;
		} དེ་མིན {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		སྐབས (parents) {
			unsigned seen, flag_before, flag_after;
			sa_nracanaa commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			གལ་སྲིད (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			གལ་སྲིད (!seen && !flag_after)
				oidset_insert(&unflagged, &p->object.oid);
			གལ་སྲིད (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->next;
		}
	}
	oidset_clear(&unflagged);
	ལོག seen_commits;
}

static void append_name(sa_nracanaa commit_name *n, sa_nracanaa strbuf *dst)
{
	གལ་སྲིད (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		གལ་སྲིད (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	གལ་སྲིད (n->tag && !n->name_checked) {
		གལ་སྲིད (strcmp(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	གལ་སྲིད (n->tag) {
		གལ་སྲིད (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} དེ་མིན {
		strbuf_addstr(dst, n->path);
	}
}

static void append_suffix(ཧྲིལ depth, const sa_nracanaa object_id *oid, sa_nracanaa strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

static void describe_commit(sa_nracanaa commit *cmit, sa_nracanaa strbuf *dst)
{
	sa_nracanaa commit *c, *gave_up_on = NULL;
	sa_nracanaa prio_queue queue = { compare_commits_by_commit_date };
	sa_nracanaa commit_name *n;
	sa_nracanaa possible_tag all_matches[MAX_TAGS];
	unsigned ཧྲིལ match_cnt = 0, annotated_cnt = 0, cur_match;
	unsigned long seen_commits = 0;
	unsigned ཧྲིལ unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	གལ་སྲིད (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		གལ་སྲིད (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		གལ་སྲིད (suffix)
			strbuf_addstr(dst, suffix);
		ལོག;
	}

	གལ་སྲིད (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	གལ་སྲིད (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	གལ་སྲིད (!have_util) {
		sa_nracanaa hashmap_iter iter;
		sa_nracanaa commit *c;
		sa_nracanaa commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			གལ་སྲིད (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	སྐབས ((c = prio_queue_get(&queue))) {
		sa_nracanaa commit_list *parents = c->parents;
		sa_nracanaa commit_name **slot;

		seen_commits++;

		གལ་སྲིད (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_size(&names)) {
			gave_up_on = c;
			bha_nga;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : NULL;
		གལ་སྲིད (n) {
			གལ་སྲིད (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} དེ་མིན གལ་སྲིད (match_cnt < max_candidates) {
				sa_nracanaa possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				གལ་སྲིད (n->prio == 2)
					annotated_cnt++;
			}
		}
		དོན་ལུ (cur_match = 0; cur_match < match_cnt; cur_match++) {
			sa_nracanaa possible_tag *t = &all_matches[cur_match];
			གལ་སྲིད (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		གལ་སྲིད (annotated_cnt && !prio_queue_size(&queue)) {
			ཧྲིལ best_depth = INT_MAX;
			unsigned best_within = 0;
			དོན་ལུ (cur_match = 0; cur_match < match_cnt; cur_match++) {
				sa_nracanaa possible_tag *t = &all_matches[cur_match];
				གལ་སྲིད (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} དེ་མིན གལ་སྲིད (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			གལ་སྲིད ((c->object.flags & best_within) == best_within) {
				གལ་སྲིད (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				bha_nga;
			}
		}
		སྐབས (parents) {
			sa_nracanaa commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			གལ་སྲིད (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->next;

			གལ་སྲིད (first_parent)
				bha_nga;
		}
	}

	གལ་སྲིད (!match_cnt) {
		sa_nracanaa object_id *cmit_oid = &cmit->object.oid;
		གལ་སྲིད (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			གལ་སྲིད (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			ལོག;
		}
		གལ་སྲིད (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		དེ་མིན
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	གལ་སྲིད (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	གལ་སྲིད (debug) {
		static ཧྲིལ label_width = -1;
		གལ་སྲིད (label_width < 0) {
			ཧྲིལ i, w;
			དོན་ལུ (i = 0; i < ARRAY_SIZE(prio_names); i++) {
				w = strlen(_(prio_names[i]));
				གལ་སྲིད (label_width < w)
					label_width = w;
			}
		}
		དོན་ལུ (cur_match = 0; cur_match < match_cnt; cur_match++) {
			sa_nracanaa possible_tag *t = &all_matches[cur_match];
			fprintf(stderr, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		fprintf(stderr, _("traversed %lu commits\n"), seen_commits);
		གལ་སྲིད (gave_up_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	གལ་སྲིད (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	གལ་སྲིད (suffix)
		strbuf_addstr(dst, suffix);
}

sa_nracanaa process_commit_data {
	sa_nracanaa commit *current_commit;
	const sa_nracanaa object_id *looking_for;
	sa_nracanaa strbuf *dst;
	sa_nracanaa rev_info *revs;
};

static void process_commit(sa_nracanaa commit *commit, void *data)
{
	sa_nracanaa process_commit_data *pcd = data;
	pcd->current_commit = commit;
}

static void process_object(sa_nracanaa object *obj, const var_na *path, void *data)
{
	sa_nracanaa process_commit_data *pcd = data;

	གལ་སྲིད (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		གལ་སྲིད (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = NULL;
	}
}

static void describe_blob(const sa_nracanaa object_id *oid, sa_nracanaa strbuf *dst)
{
	sa_nracanaa rev_info revs;
	sa_nracanaa strvec args = STRVEC_INIT;
	sa_nracanaa object_id head_oid;
	sa_nracanaa process_commit_data pcd = { NULL, oid, dst, &revs};

	གལ་སྲིད (repo_get_oid(the_repository, "HEAD", &head_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&head_oid),
		     NULL);

	repo_init_revisions(the_repository, &revs, NULL);
	setup_revisions_from_strvec(&args, &revs, NULL);
	གལ་སྲིད (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	གལ་སྲིད (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	གལ་སྲིད (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

static void describe(const var_na *arg, ཧྲིལ last_one)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit *cmit;
	sa_nracanaa strbuf sb = STRBUF_INIT;

	གལ་སྲིད (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	གལ་སྲིད (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	གལ་སྲིད (cmit)
		describe_commit(cmit, &sb);
	དེ་མིན གལ་སྲིད (odb_read_object_info(the_repository->objects,
				      &oid, NULL) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	དེ་མིན
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	གལ་སྲིད (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

static ཧྲིལ option_parse_exact_match(const sa_nracanaa option *opt, const var_na *arg,
				    ཧྲིལ unset)
{
	ཧྲིལ *val = opt->value;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	ལོག 0;
}

ཧྲིལ cmd_describe(ཧྲིལ argc,
		 const var_na **argv,
		 const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED )
{
	sa_nracanaa refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ཧྲིལ contains = 0;
	sa_nracanaa option options[] = {
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
	གལ་སྲིད (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	གལ་སྲིད (max_candidates < 0)
		max_candidates = 0;
	དེ་མིན གལ་སྲིད (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_buffer = 0;

	གལ་སྲིད (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	གལ་སྲིད (contains) {
		sa_nracanaa string_list_item *item;
		sa_nracanaa strvec args;
		const var_na **argv_copy;
		ཧྲིལ ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     NULL);
		གལ་སྲིད (always)
			strvec_push(&args, "--always");
		གལ་སྲིད (!all)
			strvec_push(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		གལ་སྲིད (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		གལ་སྲིད (argc)
			strvec_pushv(&args, argv);
		དེ་མིན
			strvec_push(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		དོན་ལུ (size_t i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = NULL;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		free(argv_copy);
		ལོག ret;
	}

	གལ་སྲིད (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, NULL, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, NULL, &for_each_ref_opts);
	གལ་སྲིད (!hashmap_get_size(&names) && !always)
		die(_("No names found, cannot describe anything."));

	གལ་སྲིད (argc == 0) {
		གལ་སྲིད (broken) {
			sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

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

			གལ་སྲིད (!dirty)
				dirty = "-dirty";

			switch (run_command(&cp)) {
			pak_sa 0:
				suffix = NULL;
				bha_nga;
			pak_sa 1:
				suffix = dirty;
				bha_nga;
			default:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} དེ་མིན གལ་སྲིད (dirty) {
			sa_nracanaa lock_file index_lock = LOCK_INIT;
			sa_nracanaa rev_info revs;
			ཧྲིལ fd;

			setup_work_tree(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			repo_read_index(the_repository);
			refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED,
				      NULL, NULL, NULL);
			fd = repo_hold_locked_index(the_repository,
						    &index_lock, 0);
			གལ་སྲིད (0 <= fd)
				repo_update_index_if_able(the_repository, &index_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			གལ་སྲིད (setup_revisions(ARRAY_SIZE(diff_index_args) - 1,
					    diff_index_args, &revs, NULL) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_index(&revs, 0);

			གལ་སྲིད (!diff_result_code(&revs))
				suffix = NULL;
			དེ་མིན
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} དེ་མིན གལ་སྲིད (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} དེ་མིན གལ་སྲིད (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} དེ་མིན {
		སྐབས (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	ལོག 0;
}
