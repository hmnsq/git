#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "hashmap.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "list-objects.h"
#ಅನ್ತರ್ಭಾವ "commit-slab.h"
#ಅನ್ತರ್ಭಾವ "wildmatch.h"
#ಅನ್ತರ್ಭಾವ "prio-queue.h"
#ಅನ್ತರ್ಭಾವ "oidset.h"

#define MAX_TAGS	(FLAG_BITS - 1)
#define DEFAULT_CANDIDATES 10

define_commit_slab(commit_names, ಸಂರಚನಾ commit_name *);

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ debug;	/* Display lots of verbose info */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ all;	/* Any valid ref can be used */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tags;	/* Allow lightweight tags */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ longformat;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ first_parent;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ abbrev = -1; /* unspecified */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ max_candidates = DEFAULT_CANDIDATES;
ಸ್ಥಿರ ಸಂರಚನಾ hashmap names;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ have_util;
ಸ್ಥಿರ ಸಂರಚನಾ string_list patterns = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಸಂರಚನಾ string_list exclude_patterns = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ always;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *suffix, *dirty, *broken;
ಸ್ಥಿರ ಸಂರಚನಾ commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *diff_index_args[] = {
	"diff-index", "--quiet", "HEAD", "--", ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *update_index_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", ರಿಕ್ತ
};

ಸಂರಚನಾ commit_name {
	ಸಂರಚನಾ hashmap_entry entry;
	ಸಂರಚನಾ object_id peeled;
	ಸಂರಚನಾ tag *tag;
	unsigned prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	unsigned name_checked:1;
	unsigned misnamed:1;
	ಸಂರಚನಾ object_id oid;
	ವರ್ಣ *path;
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ commit_name_neq(ಧ್ರುವ ಶೂನ್ಯ *cmp_data UNUSED,
			   ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *eptr,
			   ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *entry_or_ಕೀಲಿ,
			   ಧ್ರುವ ಶೂನ್ಯ *peeled)
{
	ಧ್ರುವ ಸಂರಚನಾ commit_name *cn1, *cn2;

	cn1 = container_of(eptr, ಧ್ರುವ ಸಂರಚನಾ commit_name, entry);
	cn2 = container_of(entry_or_ಕೀಲಿ, ಧ್ರುವ ಸಂರಚನಾ commit_name, entry);

	ಪ್ರತ್ಯಾಗಮ !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ commit_name *find_commit_name(ಧ್ರುವ ಸಂರಚನಾ object_id *peeled)
{
	ಪ್ರತ್ಯಾಗಮ hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						ಸಂರಚನಾ commit_name, entry);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ replace_name(ಸಂರಚನಾ commit_name *e,
			       ಪೂರ್ಣಾಙ್ಕ prio,
			       ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
			       ಸಂರಚನಾ tag **tag)
{
	ಯದಿ (!e || e->prio < prio)
		ಪ್ರತ್ಯಾಗಮ 1;

	ಯದಿ (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		ಸಂರಚನಾ tag *t;

		ಯದಿ (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			ಯದಿ (!t || parse_tag(the_repository, t))
				ಪ್ರತ್ಯಾಗಮ 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		ಯದಿ (!t || parse_tag(the_repository, t))
			ಪ್ರತ್ಯಾಗಮ 0;
		*tag = t;

		ಯದಿ (e->tag->date < t->date)
			ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ add_to_known_names(ಧ್ರುವ ವರ್ಣ *path,
			       ಧ್ರುವ ಸಂರಚನಾ object_id *peeled,
			       ಪೂರ್ಣಾಙ್ಕ prio,
			       ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ commit_name *e = find_commit_name(peeled);
	ಸಂರಚನಾ tag *tag = ರಿಕ್ತ;
	ಯದಿ (replace_name(e, prio, oid, &tag)) {
		ಯದಿ (!e) {
			e = xmalloc(ಪರಿಮಾಣ(ಸಂರಚನಾ commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_entry_init(&e->entry, oidhash(peeled));
			hashmap_add(&names, &e->entry);
			e->path = ರಿಕ್ತ;
		}
		e->tag = tag;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		ಮುಕ್ತ(e->path);
		e->path = xstrdup(path);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_name(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ is_tag = 0;
	ಸಂರಚನಾ object_id peeled;
	ಪೂರ್ಣಾಙ್ಕ is_annotated, prio;
	ಧ್ರುವ ವರ್ಣ *path_to_match = ರಿಕ್ತ;

	ಯದಿ (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} ಅನ್ಯಥಾ ಯದಿ (all) {
		ಯದಿ ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	} ಅನ್ಯಥಾ {
		/* Reject anything outside refs/tags/ unless --all */
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	ಯದಿ (exclude_patterns.nr) {
		ಸಂರಚನಾ string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			ಯದಿ (!wildmatch(item->string, path_to_match, 0))
				ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	ಯದಿ (patterns.nr) {
		ಪೂರ್ಣಾಙ್ಕ found = 0;
		ಸಂರಚನಾ string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			ಯದಿ (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				ಭಙ್ಗ;
			}
		}

		ಯದಿ (!found)
			ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* Is it annotated? */
	ಯದಿ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} ಅನ್ಯಥಾ {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	ಯದಿ (is_annotated)
		prio = 2;
	ಅನ್ಯಥಾ ಯದಿ (is_tag)
		prio = 1;
	ಅನ್ಯಥಾ
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ possible_tag {
	ಸಂರಚನಾ commit_name *name;
	ಪೂರ್ಣಾಙ್ಕ depth;
	ಪೂರ್ಣಾಙ್ಕ found_order;
	unsigned flag_within;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ compare_pt(ಧ್ರುವ ಶೂನ್ಯ *a_, ಧ್ರುವ ಶೂನ್ಯ *b_)
{
	ಸಂರಚನಾ possible_tag *a = (ಸಂರಚನಾ possible_tag *)a_;
	ಸಂರಚನಾ possible_tag *b = (ಸಂರಚನಾ possible_tag *)b_;
	ಯದಿ (a->depth != b->depth)
		ಪ್ರತ್ಯಾಗಮ a->depth - b->depth;
	ಯದಿ (a->found_order != b->found_order)
		ಪ್ರತ್ಯಾಗಮ a->found_order - b->found_order;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ unsigned long finish_depth_computation(ಸಂರಚನಾ prio_queue *queue,
					      ಸಂರಚನಾ possible_tag *best)
{
	unsigned long seen_commits = 0;
	ಸಂರಚನಾ oidset unflagged = OIDSET_INIT;
	ಸಂರಚನಾ commit *c;

	prio_queue_for_each(queue, c) {
		ಯದಿ (!(c->object.flags & best->flag_within))
			oidset_ಸೇರಿಸು(&unflagged, &c->object.oid);
	}

	ಯಾವತ್ ((c = prio_queue_get(queue))) {
		ಸಂರಚನಾ commit_list *parents = c->parents;
		seen_commits++;
		ಯದಿ (c->object.flags & best->flag_within) {
			ಯದಿ (!oidset_size(&unflagged))
				ಭಙ್ಗ;
		} ಅನ್ಯಥಾ {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		ಯಾವತ್ (parents) {
			unsigned seen, flag_before, flag_after;
			ಸಂರಚನಾ commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			ಯದಿ (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			ಯದಿ (!seen && !flag_after)
				oidset_ಸೇರಿಸು(&unflagged, &p->object.oid);
			ಯದಿ (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->next;
		}
	}
	oidset_clear(&unflagged);
	ಪ್ರತ್ಯಾಗಮ seen_commits;
}

ಸ್ಥಿರ ಶೂನ್ಯ append_name(ಸಂರಚನಾ commit_name *n, ಸಂರಚನಾ strbuf *dst)
{
	ಯದಿ (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		ಯದಿ (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	ಯದಿ (n->tag && !n->name_checked) {
		ಯದಿ (strcmp(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	ಯದಿ (n->tag) {
		ಯದಿ (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} ಅನ್ಯಥಾ {
		strbuf_addstr(dst, n->path);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ append_suffix(ಪೂರ್ಣಾಙ್ಕ depth, ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

ಸ್ಥಿರ ಶೂನ್ಯ describe_commit(ಸಂರಚನಾ commit *cmit, ಸಂರಚನಾ strbuf *dst)
{
	ಸಂರಚನಾ commit *c, *gave_up_on = ರಿಕ್ತ;
	ಸಂರಚನಾ prio_queue queue = { compare_commits_by_commit_date };
	ಸಂರಚನಾ commit_name *n;
	ಸಂರಚನಾ possible_tag all_matches[MAX_TAGS];
	unsigned ಪೂರ್ಣಾಙ್ಕ match_cnt = 0, annotated_cnt = 0, cur_match;
	unsigned long seen_commits = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	ಯದಿ (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		ಯದಿ (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		ಯದಿ (suffix)
			strbuf_addstr(dst, suffix);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	ಯದಿ (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	ಯದಿ (!have_util) {
		ಸಂರಚನಾ hashmap_iter iter;
		ಸಂರಚನಾ commit *c;
		ಸಂರಚನಾ commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			ಯದಿ (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	ಯಾವತ್ ((c = prio_queue_get(&queue))) {
		ಸಂರಚನಾ commit_list *parents = c->parents;
		ಸಂರಚನಾ commit_name **slot;

		seen_commits++;

		ಯದಿ (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_size(&names)) {
			gave_up_on = c;
			ಭಙ್ಗ;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : ರಿಕ್ತ;
		ಯದಿ (n) {
			ಯದಿ (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} ಅನ್ಯಥಾ ಯದಿ (match_cnt < max_candidates) {
				ಸಂರಚನಾ possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				ಯದಿ (n->prio == 2)
					annotated_cnt++;
			}
		}
		ಪ್ರತಿ (cur_match = 0; cur_match < match_cnt; cur_match++) {
			ಸಂರಚನಾ possible_tag *t = &all_matches[cur_match];
			ಯದಿ (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		ಯದಿ (annotated_cnt && !prio_queue_size(&queue)) {
			ಪೂರ್ಣಾಙ್ಕ best_depth = INT_MAX;
			unsigned best_within = 0;
			ಪ್ರತಿ (cur_match = 0; cur_match < match_cnt; cur_match++) {
				ಸಂರಚನಾ possible_tag *t = &all_matches[cur_match];
				ಯದಿ (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} ಅನ್ಯಥಾ ಯದಿ (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			ಯದಿ ((c->object.flags & best_within) == best_within) {
				ಯದಿ (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				ಭಙ್ಗ;
			}
		}
		ಯಾವತ್ (parents) {
			ಸಂರಚನಾ commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			ಯದಿ (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->next;

			ಯದಿ (first_parent)
				ಭಙ್ಗ;
		}
	}

	ಯದಿ (!match_cnt) {
		ಸಂರಚನಾ object_id *cmit_oid = &cmit->object.oid;
		ಯದಿ (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			ಯದಿ (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			ಪ್ರತ್ಯಾಗಮ;
		}
		ಯದಿ (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		ಅನ್ಯಥಾ
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	ಯದಿ (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	ಯದಿ (debug) {
		ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ label_width = -1;
		ಯದಿ (label_width < 0) {
			ಪೂರ್ಣಾಙ್ಕ i, w;
			ಪ್ರತಿ (i = 0; i < ARRAY_SIZE(prio_names); i++) {
				w = strlen(_(prio_names[i]));
				ಯದಿ (label_width < w)
					label_width = w;
			}
		}
		ಪ್ರತಿ (cur_match = 0; cur_match < match_cnt; cur_match++) {
			ಸಂರಚನಾ possible_tag *t = &all_matches[cur_match];
			fprintf(stderr, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		fprintf(stderr, _("traversed %lu commits\n"), seen_commits);
		ಯದಿ (gave_up_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	ಯದಿ (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	ಯದಿ (suffix)
		strbuf_addstr(dst, suffix);
}

ಸಂರಚನಾ process_commit_data {
	ಸಂರಚನಾ commit *current_commit;
	ಧ್ರುವ ಸಂರಚನಾ object_id *looking_for;
	ಸಂರಚನಾ strbuf *dst;
	ಸಂರಚನಾ rev_info *revs;
};

ಸ್ಥಿರ ಶೂನ್ಯ process_commit(ಸಂರಚನಾ commit *commit, ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ process_commit_data *pcd = data;
	pcd->current_commit = commit;
}

ಸ್ಥಿರ ಶೂನ್ಯ process_object(ಸಂರಚನಾ object *obj, ಧ್ರುವ ವರ್ಣ *path, ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ process_commit_data *pcd = data;

	ಯದಿ (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		ಯದಿ (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = ರಿಕ್ತ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ describe_blob(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ strbuf *dst)
{
	ಸಂರಚನಾ rev_info revs;
	ಸಂರಚನಾ strvec args = STRVEC_INIT;
	ಸಂರಚನಾ object_id head_oid;
	ಸಂರಚನಾ process_commit_data pcd = { ರಿಕ್ತ, oid, dst, &revs};

	ಯದಿ (repo_get_oid(the_repository, "HEAD", &head_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&head_oid),
		     ರಿಕ್ತ);

	repo_init_revisions(the_repository, &revs, ರಿಕ್ತ);
	setup_revisions_from_strvec(&args, &revs, ರಿಕ್ತ);
	ಯದಿ (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	ಯದಿ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	ಯದಿ (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

ಸ್ಥಿರ ಶೂನ್ಯ describe(ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ last_one)
{
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ commit *cmit;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	ಯದಿ (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	ಯದಿ (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	ಯದಿ (cmit)
		describe_commit(cmit, &sb);
	ಅನ್ಯಥಾ ಯದಿ (odb_read_object_info(the_repository->objects,
				      &oid, ರಿಕ್ತ) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	ಅನ್ಯಥಾ
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	ಯದಿ (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_exact_match(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
				    ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *val = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_describe(ಪೂರ್ಣಾಙ್ಕ argc,
		 ಧ್ರುವ ವರ್ಣ **argv,
		 ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo UNUSED )
{
	ಸಂರಚನಾ refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ಪೂರ್ಣಾಙ್ಕ contains = 0;
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL(0, "contains",   &contains, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &tags, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, ರಿಕ್ತ,
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
			.ಮೌಲ್ಯ = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.ಮೌಲ್ಯ = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
	argc = parse_options(argc, argv, prefix, options, describe_usage, 0);
	ಯದಿ (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	ಯದಿ (max_candidates < 0)
		max_candidates = 0;
	ಅನ್ಯಥಾ ಯದಿ (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_buffer = 0;

	ಯದಿ (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	ಯದಿ (contains) {
		ಸಂರಚನಾ string_list_item *item;
		ಸಂರಚನಾ strvec args;
		ಧ್ರುವ ವರ್ಣ **argv_copy;
		ಪೂರ್ಣಾಙ್ಕ ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     ರಿಕ್ತ);
		ಯದಿ (always)
			strvec_ತಳ್ಳು(&args, "--always");
		ಯದಿ (!all)
			strvec_ತಳ್ಳು(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		ಯದಿ (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		ಯದಿ (argc)
			strvec_pushv(&args, argv);
		ಅನ್ಯಥಾ
			strvec_ತಳ್ಳು(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		ಪ್ರತಿ (size_t i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = ರಿಕ್ತ;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		ಮುಕ್ತ(argv_copy);
		ಪ್ರತ್ಯಾಗಮ ret;
	}

	ಯದಿ (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, ರಿಕ್ತ, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, ರಿಕ್ತ, &for_each_ref_opts);
	ಯದಿ (!hashmap_get_size(&names) && !always)
		die(_("No names found, cannot describe anything."));

	ಯದಿ (argc == 0) {
		ಯದಿ (broken) {
			ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

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

			ಯದಿ (!dirty)
				dirty = "-dirty";

			ವಿಕಲ್ಪ (run_command(&cp)) {
			ಪಕ್ಷ 0:
				suffix = ರಿಕ್ತ;
				ಭಙ್ಗ;
			ಪಕ್ಷ 1:
				suffix = dirty;
				ಭಙ್ಗ;
			ಸ್ವಭಾವ:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} ಅನ್ಯಥಾ ಯದಿ (dirty) {
			ಸಂರಚನಾ lock_file index_lock = LOCK_INIT;
			ಸಂರಚನಾ rev_info revs;
			ಪೂರ್ಣಾಙ್ಕ fd;

			setup_work_ವೃಕ್ಷ(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			repo_read_index(the_repository);
			refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED,
				      ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
			fd = repo_hold_locked_index(the_repository,
						    &index_lock, 0);
			ಯದಿ (0 <= fd)
				repo_update_index_if_able(the_repository, &index_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			ಯದಿ (setup_revisions(ARRAY_SIZE(diff_index_args) - 1,
					    diff_index_args, &revs, ರಿಕ್ತ) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_index(&revs, 0);

			ಯದಿ (!diff_result_code(&revs))
				suffix = ರಿಕ್ತ;
			ಅನ್ಯಥಾ
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} ಅನ್ಯಥಾ ಯದಿ (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} ಅನ್ಯಥಾ ಯದಿ (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} ಅನ್ಯಥಾ {
		ಯಾವತ್ (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}
