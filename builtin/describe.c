#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "hashmap.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "list-objects.h"
#ଅନ୍ତର୍ଭାଵ "commit-slab.h"
#ଅନ୍ତର୍ଭାଵ "wildmatch.h"
#ଅନ୍ତର୍ଭାଵ "prio-queue.h"
#ଅନ୍ତର୍ଭାଵ "oidset.h"

#define MAX_TAGS	(FLAG_BITS - 1)
#define DEFAULT_CANDIDATES 10

define_commit_slab(commit_names, ସଂରଚନା commit_name *);

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା debug;	/* Display lots of verbose info */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା all;	/* Any valid ref can be used */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tags;	/* Allow lightweight tags */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା longformat;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_parent;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା abbrev = -1; /* unspecified */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_candidates = DEFAULT_CANDIDATES;
ସ୍ଥିର ସଂରଚନା hashmap names;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା have_util;
ସ୍ଥିର ସଂରଚନା string_list patterns = STRING_LIST_INIT_NODUP;
ସ୍ଥିର ସଂରଚନା string_list exclude_patterns = STRING_LIST_INIT_NODUP;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା always;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *suffix, *dirty, *broken;
ସ୍ଥିର ସଂରଚନା commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *diff_index_args[] = {
	"diff-index", "--quiet", "HEAD", "--", ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *update_index_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", ଖାଲି
};

ସଂରଚନା commit_name {
	ସଂରଚନା hashmap_entry entry;
	ସଂରଚନା object_id peeled;
	ସଂରଚନା tag *tag;
	unsigned prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	unsigned name_checked:1;
	unsigned misnamed:1;
	ସଂରଚନା object_id oid;
	ଅକ୍ଷର *path;
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା commit_name_neq(ଅଚଳ ଶୂନ୍ୟ *cmp_data UNUSED,
			   ଅଚଳ ସଂରଚନା hashmap_entry *eptr,
			   ଅଚଳ ସଂରଚନା hashmap_entry *entry_or_ଚାବି,
			   ଅଚଳ ଶୂନ୍ୟ *peeled)
{
	ଅଚଳ ସଂରଚନା commit_name *cn1, *cn2;

	cn1 = container_of(eptr, ଅଚଳ ସଂରଚନା commit_name, entry);
	cn2 = container_of(entry_or_ଚାବି, ଅଚଳ ସଂରଚନା commit_name, entry);

	ଫେରନ୍ତୁ !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା commit_name *find_commit_name(ଅଚଳ ସଂରଚନା object_id *peeled)
{
	ଫେରନ୍ତୁ hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						ସଂରଚନା commit_name, entry);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା replace_name(ସଂରଚନା commit_name *e,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio,
			       ଅଚଳ ସଂରଚନା object_id *oid,
			       ସଂରଚନା tag **tag)
{
	ଯଦି (!e || e->prio < prio)
		ଫେରନ୍ତୁ 1;

	ଯଦି (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		ସଂରଚନା tag *t;

		ଯଦି (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			ଯଦି (!t || parse_tag(the_repository, t))
				ଫେରନ୍ତୁ 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		ଯଦି (!t || parse_tag(the_repository, t))
			ଫେରନ୍ତୁ 0;
		*tag = t;

		ଯଦି (e->tag->date < t->date)
			ଫେରନ୍ତୁ 1;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ add_to_known_names(ଅଚଳ ଅକ୍ଷର *path,
			       ଅଚଳ ସଂରଚନା object_id *peeled,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio,
			       ଅଚଳ ସଂରଚନା object_id *oid)
{
	ସଂରଚନା commit_name *e = find_commit_name(peeled);
	ସଂରଚନା tag *tag = ଖାଲି;
	ଯଦି (replace_name(e, prio, oid, &tag)) {
		ଯଦି (!e) {
			e = xmalloc(ଆକାର(ସଂରଚନା commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_entry_init(&e->entry, oidhash(peeled));
			hashmap_add(&names, &e->entry);
			e->path = ଖାଲି;
		}
		e->tag = tag;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		ମୁକ୍ତ(e->path);
		e->path = xstrdup(path);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_name(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_tag = 0;
	ସଂରଚନା object_id peeled;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_annotated, prio;
	ଅଚଳ ଅକ୍ଷର *path_to_match = ଖାଲି;

	ଯଦି (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} ଅନ୍ୟଥା ଯଦି (all) {
		ଯଦି ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			ଫେରନ୍ତୁ 0;
		}
	} ଅନ୍ୟଥା {
		/* Reject anything outside refs/tags/ unless --all */
		ଫେରନ୍ତୁ 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	ଯଦି (exclude_patterns.nr) {
		ସଂରଚନା string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			ଯଦି (!wildmatch(item->string, path_to_match, 0))
				ଫେରନ୍ତୁ 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	ଯଦି (patterns.nr) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା found = 0;
		ସଂରଚନା string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			ଯଦି (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				ଭାଙ୍ଗ;
			}
		}

		ଯଦି (!found)
			ଫେରନ୍ତୁ 0;
	}

	/* Is it annotated? */
	ଯଦି (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} ଅନ୍ୟଥା {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	ଯଦି (is_annotated)
		prio = 2;
	ଅନ୍ୟଥା ଯଦି (is_tag)
		prio = 1;
	ଅନ୍ୟଥା
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା possible_tag {
	ସଂରଚନା commit_name *name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା found_order;
	unsigned flag_within;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compare_pt(ଅଚଳ ଶୂନ୍ୟ *a_, ଅଚଳ ଶୂନ୍ୟ *b_)
{
	ସଂରଚନା possible_tag *a = (ସଂରଚନା possible_tag *)a_;
	ସଂରଚନା possible_tag *b = (ସଂରଚନା possible_tag *)b_;
	ଯଦି (a->depth != b->depth)
		ଫେରନ୍ତୁ a->depth - b->depth;
	ଯଦି (a->found_order != b->found_order)
		ଫେରନ୍ତୁ a->found_order - b->found_order;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର unsigned long finish_depth_computation(ସଂରଚନା prio_queue *queue,
					      ସଂରଚନା possible_tag *best)
{
	unsigned long seen_commits = 0;
	ସଂରଚନା oidset unflagged = OIDSET_INIT;
	ସଂରଚନା commit *c;

	prio_queue_for_each(queue, c) {
		ଯଦି (!(c->object.flags & best->flag_within))
			oidset_ଭର୍ତ୍ତି(&unflagged, &c->object.oid);
	}

	ଯେତେବେଳେ ((c = prio_queue_get(queue))) {
		ସଂରଚନା commit_list *parents = c->parents;
		seen_commits++;
		ଯଦି (c->object.flags & best->flag_within) {
			ଯଦି (!oidset_size(&unflagged))
				ଭାଙ୍ଗ;
		} ଅନ୍ୟଥା {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		ଯେତେବେଳେ (parents) {
			unsigned seen, flag_before, flag_after;
			ସଂରଚନା commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			ଯଦି (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			ଯଦି (!seen && !flag_after)
				oidset_ଭର୍ତ୍ତି(&unflagged, &p->object.oid);
			ଯଦି (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->next;
		}
	}
	oidset_clear(&unflagged);
	ଫେରନ୍ତୁ seen_commits;
}

ସ୍ଥିର ଶୂନ୍ୟ append_name(ସଂରଚନା commit_name *n, ସଂରଚନା strbuf *dst)
{
	ଯଦି (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		ଯଦି (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	ଯଦି (n->tag && !n->name_checked) {
		ଯଦି (strcmp(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	ଯଦି (n->tag) {
		ଯଦି (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} ଅନ୍ୟଥା {
		strbuf_addstr(dst, n->path);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ append_suffix(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

ସ୍ଥିର ଶୂନ୍ୟ describe_commit(ସଂରଚନା commit *cmit, ସଂରଚନା strbuf *dst)
{
	ସଂରଚନା commit *c, *gave_up_on = ଖାଲି;
	ସଂରଚନା prio_queue queue = { compare_commits_by_commit_date };
	ସଂରଚନା commit_name *n;
	ସଂରଚନା possible_tag all_matches[MAX_TAGS];
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା match_cnt = 0, annotated_cnt = 0, cur_match;
	unsigned long seen_commits = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	ଯଦି (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		ଯଦି (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		ଯଦି (suffix)
			strbuf_addstr(dst, suffix);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	ଯଦି (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	ଯଦି (!have_util) {
		ସଂରଚନା hashmap_iter iter;
		ସଂରଚନା commit *c;
		ସଂରଚନା commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			ଯଦି (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	ଯେତେବେଳେ ((c = prio_queue_get(&queue))) {
		ସଂରଚନା commit_list *parents = c->parents;
		ସଂରଚନା commit_name **slot;

		seen_commits++;

		ଯଦି (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_size(&names)) {
			gave_up_on = c;
			ଭାଙ୍ଗ;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : ଖାଲି;
		ଯଦି (n) {
			ଯଦି (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} ଅନ୍ୟଥା ଯଦି (match_cnt < max_candidates) {
				ସଂରଚନା possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				ଯଦି (n->prio == 2)
					annotated_cnt++;
			}
		}
		ପାଇଁ (cur_match = 0; cur_match < match_cnt; cur_match++) {
			ସଂରଚନା possible_tag *t = &all_matches[cur_match];
			ଯଦି (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		ଯଦି (annotated_cnt && !prio_queue_size(&queue)) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_depth = INT_MAX;
			unsigned best_within = 0;
			ପାଇଁ (cur_match = 0; cur_match < match_cnt; cur_match++) {
				ସଂରଚନା possible_tag *t = &all_matches[cur_match];
				ଯଦି (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} ଅନ୍ୟଥା ଯଦି (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			ଯଦି ((c->object.flags & best_within) == best_within) {
				ଯଦି (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				ଭାଙ୍ଗ;
			}
		}
		ଯେତେବେଳେ (parents) {
			ସଂରଚନା commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			ଯଦି (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->next;

			ଯଦି (first_parent)
				ଭାଙ୍ଗ;
		}
	}

	ଯଦି (!match_cnt) {
		ସଂରଚନା object_id *cmit_oid = &cmit->object.oid;
		ଯଦି (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			ଯଦି (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			ଫେରନ୍ତୁ;
		}
		ଯଦି (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		ଅନ୍ୟଥା
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	ଯଦି (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	ଯଦି (debug) {
		ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା label_width = -1;
		ଯଦି (label_width < 0) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, w;
			ପାଇଁ (i = 0; i < ARRAY_SIZE(prio_names); i++) {
				w = strlen(_(prio_names[i]));
				ଯଦି (label_width < w)
					label_width = w;
			}
		}
		ପାଇଁ (cur_match = 0; cur_match < match_cnt; cur_match++) {
			ସଂରଚନା possible_tag *t = &all_matches[cur_match];
			fprintf(stderr, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		fprintf(stderr, _("traversed %lu commits\n"), seen_commits);
		ଯଦି (gave_up_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	ଯଦି (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	ଯଦି (suffix)
		strbuf_addstr(dst, suffix);
}

ସଂରଚନା process_commit_data {
	ସଂରଚନା commit *current_commit;
	ଅଚଳ ସଂରଚନା object_id *looking_for;
	ସଂରଚନା strbuf *dst;
	ସଂରଚନା rev_info *revs;
};

ସ୍ଥିର ଶୂନ୍ୟ process_commit(ସଂରଚନା commit *commit, ଶୂନ୍ୟ *data)
{
	ସଂରଚନା process_commit_data *pcd = data;
	pcd->current_commit = commit;
}

ସ୍ଥିର ଶୂନ୍ୟ process_object(ସଂରଚନା object *obj, ଅଚଳ ଅକ୍ଷର *path, ଶୂନ୍ୟ *data)
{
	ସଂରଚନା process_commit_data *pcd = data;

	ଯଦି (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		ଯଦି (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = ଖାଲି;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ describe_blob(ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା strbuf *dst)
{
	ସଂରଚନା rev_info revs;
	ସଂରଚନା strvec args = STRVEC_INIT;
	ସଂରଚନା object_id head_oid;
	ସଂରଚନା process_commit_data pcd = { ଖାଲି, oid, dst, &revs};

	ଯଦି (repo_get_oid(the_repository, "HEAD", &head_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&head_oid),
		     ଖାଲି);

	repo_init_revisions(the_repository, &revs, ଖାଲି);
	setup_revisions_from_strvec(&args, &revs, ଖାଲି);
	ଯଦି (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	ଯଦି (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	ଯଦି (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

ସ୍ଥିର ଶୂନ୍ୟ describe(ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_one)
{
	ସଂରଚନା object_id oid;
	ସଂରଚନା commit *cmit;
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ଯଦି (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	ଯଦି (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	ଯଦି (cmit)
		describe_commit(cmit, &sb);
	ଅନ୍ୟଥା ଯଦି (odb_read_object_info(the_repository->objects,
				      &oid, ଖାଲି) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	ଅନ୍ୟଥା
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	ଯଦି (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_exact_match(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *val = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_describe(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		 ଅଚଳ ଅକ୍ଷର **argv,
		 ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo UNUSED )
{
	ସଂରଚନା refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା contains = 0;
	ସଂରଚନା option options[] = {
		OPT_BOOL(0, "contains",   &contains, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &tags, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, ଖାଲି,
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
			.ମୂଲ୍ୟ = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.ମୂଲ୍ୟ = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ଖାଲି);
	argc = parse_options(argc, argv, prefix, options, describe_usage, 0);
	ଯଦି (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	ଯଦି (max_candidates < 0)
		max_candidates = 0;
	ଅନ୍ୟଥା ଯଦି (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_buffer = 0;

	ଯଦି (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	ଯଦି (contains) {
		ସଂରଚନା string_list_item *item;
		ସଂରଚନା strvec args;
		ଅଚଳ ଅକ୍ଷର **argv_copy;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     ଖାଲି);
		ଯଦି (always)
			strvec_ଠେଲ(&args, "--always");
		ଯଦି (!all)
			strvec_ଠେଲ(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		ଯଦି (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		ଯଦି (argc)
			strvec_pushv(&args, argv);
		ଅନ୍ୟଥା
			strvec_ଠେଲ(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		ପାଇଁ (size_t i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = ଖାଲି;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		ମୁକ୍ତ(argv_copy);
		ଫେରନ୍ତୁ ret;
	}

	ଯଦି (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, ଖାଲି, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, ଖାଲି, &for_each_ref_opts);
	ଯଦି (!hashmap_get_size(&names) && !always)
		die(_("No names found, cannot describe anything."));

	ଯଦି (argc == 0) {
		ଯଦି (broken) {
			ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

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

			ଯଦି (!dirty)
				dirty = "-dirty";

			ବାଛ (run_command(&cp)) {
			ସ୍ଥିତି 0:
				suffix = ଖାଲି;
				ଭାଙ୍ଗ;
			ସ୍ଥିତି 1:
				suffix = dirty;
				ଭାଙ୍ଗ;
			ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} ଅନ୍ୟଥା ଯଦି (dirty) {
			ସଂରଚନା lock_file index_lock = LOCK_INIT;
			ସଂରଚନା rev_info revs;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

			setup_work_ବୃକ୍ଷ(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			repo_read_index(the_repository);
			refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED,
				      ଖାଲି, ଖାଲି, ଖାଲି);
			fd = repo_hold_locked_index(the_repository,
						    &index_lock, 0);
			ଯଦି (0 <= fd)
				repo_update_index_if_able(the_repository, &index_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			ଯଦି (setup_revisions(ARRAY_SIZE(diff_index_args) - 1,
					    diff_index_args, &revs, ଖାଲି) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_index(&revs, 0);

			ଯଦି (!diff_result_code(&revs))
				suffix = ଖାଲି;
			ଅନ୍ୟଥା
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} ଅନ୍ୟଥା ଯଦି (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} ଅନ୍ୟଥା ଯଦି (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} ଅନ୍ୟଥା {
		ଯେତେବେଳେ (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	ଫେରନ୍ତୁ 0;
}
