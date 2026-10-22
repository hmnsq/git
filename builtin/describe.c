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

static puur_naa_nka debug;	/* Display lots of verbose info */
static puur_naa_nka all;	/* Any valid ref can be used */
static puur_naa_nka tags;	/* Allow lightweight tags */
static puur_naa_nka longformat;
static puur_naa_nka first_parent;
static puur_naa_nka abbrev = -1; /* unspecified */
static puur_naa_nka max_candidates = DEFAULT_CANDIDATES;
static sa_nracanaa hashmap names;
static puur_naa_nka have_util;
static sa_nracanaa string_list patterns = STRING_LIST_INIT_NODUP;
static sa_nracanaa string_list exclude_patterns = STRING_LIST_INIT_NODUP;
static puur_naa_nka always;
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

static puur_naa_nka commit_name_neq(const void *cmp_data UNUSED,
			   const sa_nracanaa hashmap_entry *eptr,
			   const sa_nracanaa hashmap_entry *entry_or_key,
			   const void *peeled)
{
	const sa_nracanaa commit_name *cn1, *cn2;

	cn1 = container_of(eptr, const sa_nracanaa commit_name, entry);
	cn2 = container_of(entry_or_key, const sa_nracanaa commit_name, entry);

	ᱨᱩᱣᱟᱹᱲ !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

static anta_hstha sa_nracanaa commit_name *find_commit_name(const sa_nracanaa object_id *peeled)
{
	ᱨᱩᱣᱟᱹᱲ hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						sa_nracanaa commit_name, entry);
}

static puur_naa_nka replace_name(sa_nracanaa commit_name *e,
			       puur_naa_nka prio,
			       const sa_nracanaa object_id *oid,
			       sa_nracanaa tag **tag)
{
	ᱡᱩᱫᱤ (!e || e->prio < prio)
		ᱨᱩᱣᱟᱹᱲ 1;

	ᱡᱩᱫᱤ (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		sa_nracanaa tag *t;

		ᱡᱩᱫᱤ (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			ᱡᱩᱫᱤ (!t || parse_tag(the_repository, t))
				ᱨᱩᱣᱟᱹᱲ 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		ᱡᱩᱫᱤ (!t || parse_tag(the_repository, t))
			ᱨᱩᱣᱟᱹᱲ 0;
		*tag = t;

		ᱡᱩᱫᱤ (e->tag->date < t->date)
			ᱨᱩᱣᱟᱹᱲ 1;
	}

	ᱨᱩᱣᱟᱹᱲ 0;
}

static void add_to_known_names(const var_na *path,
			       const sa_nracanaa object_id *peeled,
			       puur_naa_nka prio,
			       const sa_nracanaa object_id *oid)
{
	sa_nracanaa commit_name *e = find_commit_name(peeled);
	sa_nracanaa tag *tag = NULL;
	ᱡᱩᱫᱤ (replace_name(e, prio, oid, &tag)) {
		ᱡᱩᱫᱤ (!e) {
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

static puur_naa_nka get_name(const sa_nracanaa reference *ref, void *cb_data UNUSED)
{
	puur_naa_nka is_tag = 0;
	sa_nracanaa object_id peeled;
	puur_naa_nka is_annotated, prio;
	const var_na *path_to_match = NULL;

	ᱡᱩᱫᱤ (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} ᱵᱟᱝ ᱡᱩᱫᱤ (all) {
		ᱡᱩᱫᱤ ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			ᱨᱩᱣᱟᱹᱲ 0;
		}
	} ᱵᱟᱝ {
		/* Reject anything outside refs/tags/ unless --all */
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	ᱡᱩᱫᱤ (exclude_patterns.nr) {
		sa_nracanaa string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			ᱡᱩᱫᱤ (!wildmatch(item->string, path_to_match, 0))
				ᱨᱩᱣᱟᱹᱲ 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	ᱡᱩᱫᱤ (patterns.nr) {
		puur_naa_nka found = 0;
		sa_nracanaa string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			ᱡᱩᱫᱤ (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				bha_nga;
			}
		}

		ᱡᱩᱫᱤ (!found)
			ᱨᱩᱣᱟᱹᱲ 0;
	}

	/* Is it annotated? */
	ᱡᱩᱫᱤ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} ᱵᱟᱝ {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	ᱡᱩᱫᱤ (is_annotated)
		prio = 2;
	ᱵᱟᱝ ᱡᱩᱫᱤ (is_tag)
		prio = 1;
	ᱵᱟᱝ
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	ᱨᱩᱣᱟᱹᱲ 0;
}

sa_nracanaa possible_tag {
	sa_nracanaa commit_name *name;
	puur_naa_nka depth;
	puur_naa_nka found_order;
	unsigned flag_within;
};

static puur_naa_nka compare_pt(const void *a_, const void *b_)
{
	sa_nracanaa possible_tag *a = (sa_nracanaa possible_tag *)a_;
	sa_nracanaa possible_tag *b = (sa_nracanaa possible_tag *)b_;
	ᱡᱩᱫᱤ (a->depth != b->depth)
		ᱨᱩᱣᱟᱹᱲ a->depth - b->depth;
	ᱡᱩᱫᱤ (a->found_order != b->found_order)
		ᱨᱩᱣᱟᱹᱲ a->found_order - b->found_order;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static unsigned long finish_depth_computation(sa_nracanaa prio_queue *queue,
					      sa_nracanaa possible_tag *best)
{
	unsigned long seen_commits = 0;
	sa_nracanaa oidset unflagged = OIDSET_INIT;
	sa_nracanaa commit *c;

	prio_queue_for_each(queue, c) {
		ᱡᱩᱫᱤ (!(c->object.flags & best->flag_within))
			oidset_insert(&unflagged, &c->object.oid);
	}

	ᱡᱚᱠᱷᱚᱱ ((c = prio_queue_get(queue))) {
		sa_nracanaa commit_list *parents = c->parents;
		seen_commits++;
		ᱡᱩᱫᱤ (c->object.flags & best->flag_within) {
			ᱡᱩᱫᱤ (!oidset_size(&unflagged))
				bha_nga;
		} ᱵᱟᱝ {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		ᱡᱚᱠᱷᱚᱱ (parents) {
			unsigned seen, flag_before, flag_after;
			sa_nracanaa commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			ᱡᱩᱫᱤ (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			ᱡᱩᱫᱤ (!seen && !flag_after)
				oidset_insert(&unflagged, &p->object.oid);
			ᱡᱩᱫᱤ (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->next;
		}
	}
	oidset_clear(&unflagged);
	ᱨᱩᱣᱟᱹᱲ seen_commits;
}

static void append_name(sa_nracanaa commit_name *n, sa_nracanaa strbuf *dst)
{
	ᱡᱩᱫᱤ (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		ᱡᱩᱫᱤ (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	ᱡᱩᱫᱤ (n->tag && !n->name_checked) {
		ᱡᱩᱫᱤ (strcmp(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	ᱡᱩᱫᱤ (n->tag) {
		ᱡᱩᱫᱤ (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} ᱵᱟᱝ {
		strbuf_addstr(dst, n->path);
	}
}

static void append_suffix(puur_naa_nka depth, const sa_nracanaa object_id *oid, sa_nracanaa strbuf *dst)
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
	unsigned puur_naa_nka match_cnt = 0, annotated_cnt = 0, cur_match;
	unsigned long seen_commits = 0;
	unsigned puur_naa_nka unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	ᱡᱩᱫᱤ (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		ᱡᱩᱫᱤ (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		ᱡᱩᱫᱤ (suffix)
			strbuf_addstr(dst, suffix);
		ᱨᱩᱣᱟᱹᱲ;
	}

	ᱡᱩᱫᱤ (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	ᱡᱩᱫᱤ (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	ᱡᱩᱫᱤ (!have_util) {
		sa_nracanaa hashmap_iter iter;
		sa_nracanaa commit *c;
		sa_nracanaa commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			ᱡᱩᱫᱤ (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	ᱡᱚᱠᱷᱚᱱ ((c = prio_queue_get(&queue))) {
		sa_nracanaa commit_list *parents = c->parents;
		sa_nracanaa commit_name **slot;

		seen_commits++;

		ᱡᱩᱫᱤ (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_size(&names)) {
			gave_up_on = c;
			bha_nga;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : NULL;
		ᱡᱩᱫᱤ (n) {
			ᱡᱩᱫᱤ (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} ᱵᱟᱝ ᱡᱩᱫᱤ (match_cnt < max_candidates) {
				sa_nracanaa possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				ᱡᱩᱫᱤ (n->prio == 2)
					annotated_cnt++;
			}
		}
		for (cur_match = 0; cur_match < match_cnt; cur_match++) {
			sa_nracanaa possible_tag *t = &all_matches[cur_match];
			ᱡᱩᱫᱤ (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		ᱡᱩᱫᱤ (annotated_cnt && !prio_queue_size(&queue)) {
			puur_naa_nka best_depth = INT_MAX;
			unsigned best_within = 0;
			for (cur_match = 0; cur_match < match_cnt; cur_match++) {
				sa_nracanaa possible_tag *t = &all_matches[cur_match];
				ᱡᱩᱫᱤ (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} ᱵᱟᱝ ᱡᱩᱫᱤ (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			ᱡᱩᱫᱤ ((c->object.flags & best_within) == best_within) {
				ᱡᱩᱫᱤ (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				bha_nga;
			}
		}
		ᱡᱚᱠᱷᱚᱱ (parents) {
			sa_nracanaa commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			ᱡᱩᱫᱤ (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->next;

			ᱡᱩᱫᱤ (first_parent)
				bha_nga;
		}
	}

	ᱡᱩᱫᱤ (!match_cnt) {
		sa_nracanaa object_id *cmit_oid = &cmit->object.oid;
		ᱡᱩᱫᱤ (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			ᱡᱩᱫᱤ (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			ᱨᱩᱣᱟᱹᱲ;
		}
		ᱡᱩᱫᱤ (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		ᱵᱟᱝ
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	ᱡᱩᱫᱤ (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	ᱡᱩᱫᱤ (debug) {
		static puur_naa_nka label_width = -1;
		ᱡᱩᱫᱤ (label_width < 0) {
			puur_naa_nka i, w;
			for (i = 0; i < ARRAY_SIZE(prio_names); i++) {
				w = strlen(_(prio_names[i]));
				ᱡᱩᱫᱤ (label_width < w)
					label_width = w;
			}
		}
		for (cur_match = 0; cur_match < match_cnt; cur_match++) {
			sa_nracanaa possible_tag *t = &all_matches[cur_match];
			fprintf(stderr, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		fprintf(stderr, _("traversed %lu commits\n"), seen_commits);
		ᱡᱩᱫᱤ (gave_up_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	ᱡᱩᱫᱤ (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	ᱡᱩᱫᱤ (suffix)
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

	ᱡᱩᱫᱤ (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		ᱡᱩᱫᱤ (pcd->current_commit) {
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

	ᱡᱩᱫᱤ (repo_get_oid(the_repository, "HEAD", &head_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&head_oid),
		     NULL);

	repo_init_revisions(the_repository, &revs, NULL);
	setup_revisions_from_strvec(&args, &revs, NULL);
	ᱡᱩᱫᱤ (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	ᱡᱩᱫᱤ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	ᱡᱩᱫᱤ (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

static void describe(const var_na *arg, puur_naa_nka last_one)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit *cmit;
	sa_nracanaa strbuf sb = STRBUF_INIT;

	ᱡᱩᱫᱤ (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	ᱡᱩᱫᱤ (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	ᱡᱩᱫᱤ (cmit)
		describe_commit(cmit, &sb);
	ᱵᱟᱝ ᱡᱩᱫᱤ (odb_read_object_info(the_repository->objects,
				      &oid, NULL) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	ᱵᱟᱝ
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	ᱡᱩᱫᱤ (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

static puur_naa_nka option_parse_exact_match(const sa_nracanaa option *opt, const var_na *arg,
				    puur_naa_nka unset)
{
	puur_naa_nka *val = opt->value;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka cmd_describe(puur_naa_nka argc,
		 const var_na **argv,
		 const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED )
{
	sa_nracanaa refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	puur_naa_nka contains = 0;
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
	ᱡᱩᱫᱤ (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	ᱡᱩᱫᱤ (max_candidates < 0)
		max_candidates = 0;
	ᱵᱟᱝ ᱡᱩᱫᱤ (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_buffer = 0;

	ᱡᱩᱫᱤ (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	ᱡᱩᱫᱤ (contains) {
		sa_nracanaa string_list_item *item;
		sa_nracanaa strvec args;
		const var_na **argv_copy;
		puur_naa_nka ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     NULL);
		ᱡᱩᱫᱤ (always)
			strvec_push(&args, "--always");
		ᱡᱩᱫᱤ (!all)
			strvec_push(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		ᱡᱩᱫᱤ (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		ᱡᱩᱫᱤ (argc)
			strvec_pushv(&args, argv);
		ᱵᱟᱝ
			strvec_push(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		for (size_t i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = NULL;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		free(argv_copy);
		ᱨᱩᱣᱟᱹᱲ ret;
	}

	ᱡᱩᱫᱤ (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, NULL, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, NULL, &for_each_ref_opts);
	ᱡᱩᱫᱤ (!hashmap_get_size(&names) && !always)
		die(_("No names found, cannot describe anything."));

	ᱡᱩᱫᱤ (argc == 0) {
		ᱡᱩᱫᱤ (broken) {
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

			ᱡᱩᱫᱤ (!dirty)
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
		} ᱵᱟᱝ ᱡᱩᱫᱤ (dirty) {
			sa_nracanaa lock_file index_lock = LOCK_INIT;
			sa_nracanaa rev_info revs;
			puur_naa_nka fd;

			setup_work_tree(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			repo_read_index(the_repository);
			refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED,
				      NULL, NULL, NULL);
			fd = repo_hold_locked_index(the_repository,
						    &index_lock, 0);
			ᱡᱩᱫᱤ (0 <= fd)
				repo_update_index_if_able(the_repository, &index_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			ᱡᱩᱫᱤ (setup_revisions(ARRAY_SIZE(diff_index_args) - 1,
					    diff_index_args, &revs, NULL) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_index(&revs, 0);

			ᱡᱩᱫᱤ (!diff_result_code(&revs))
				suffix = NULL;
			ᱵᱟᱝ
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} ᱵᱟᱝ ᱡᱩᱫᱤ (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} ᱵᱟᱝ ᱡᱩᱫᱤ (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} ᱵᱟᱝ {
		ᱡᱚᱠᱷᱚᱱ (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	ᱨᱩᱣᱟᱹᱲ 0;
}
