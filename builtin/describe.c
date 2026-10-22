#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "hashmap.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "list-objects.h"
#অন্তর্ভাব "commit-slab.h"
#অন্তর্ভাব "wildmatch.h"
#অন্তর্ভাব "prio-queue.h"
#অন্তর্ভাব "oidset.h"

#define MAX_TAGS	(FLAG_BITS - 1)
#define DEFAULT_CANDIDATES 10

define_commit_slab(commit_names, কাঠামো commit_name *);

স্থির ধ্রুবক অক্ষর * ধ্রুবক describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	ফাঁকা
};

স্থির পূর্ণসংখ্যা debug;	/* Display lots of verbose info */
স্থির পূর্ণসংখ্যা all;	/* Any valid ref can be used */
স্থির পূর্ণসংখ্যা tags;	/* Allow lightweight tags */
স্থির পূর্ণসংখ্যা longformat;
স্থির পূর্ণসংখ্যা first_parent;
স্থির পূর্ণসংখ্যা abbrev = -1; /* unspecified */
স্থির পূর্ণসংখ্যা max_candidates = DEFAULT_CANDIDATES;
স্থির কাঠামো hashmap names;
স্থির পূর্ণসংখ্যা have_util;
স্থির কাঠামো string_list patterns = STRING_LIST_INIT_NODUP;
স্থির কাঠামো string_list exclude_patterns = STRING_LIST_INIT_NODUP;
স্থির পূর্ণসংখ্যা always;
স্থির ধ্রুবক অক্ষর *suffix, *dirty, *broken;
স্থির কাঠামো commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
স্থির ধ্রুবক অক্ষর *diff_সূচক_args[] = {
	"diff-index", "--quiet", "HEAD", "--", ফাঁকা
};

স্থির ধ্রুবক অক্ষর *update_সূচক_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", ফাঁকা
};

কাঠামো commit_name {
	কাঠামো hashmap_entry entry;
	কাঠামো object_id peeled;
	কাঠামো tag *tag;
	unsigned prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	unsigned name_checked:1;
	unsigned misnamed:1;
	কাঠামো object_id oid;
	অক্ষর *path;
};

স্থির ধ্রুবক অক্ষর *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

স্থির পূর্ণসংখ্যা commit_name_neq(ধ্রুবক শূন্য *cmp_তথ্য UNUSED,
			   ধ্রুবক কাঠামো hashmap_entry *eptr,
			   ধ্রুবক কাঠামো hashmap_entry *entry_or_চাবি,
			   ধ্রুবক শূন্য *peeled)
{
	ধ্রুবক কাঠামো commit_name *cn1, *cn2;

	cn1 = container_of(eptr, ধ্রুবক কাঠামো commit_name, entry);
	cn2 = container_of(entry_or_চাবি, ধ্রুবক কাঠামো commit_name, entry);

	ফেরত !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

স্থির অন্তঃরেখ কাঠামো commit_name *find_commit_name(ধ্রুবক কাঠামো object_id *peeled)
{
	ফেরত hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						কাঠামো commit_name, entry);
}

স্থির পূর্ণসংখ্যা replace_name(কাঠামো commit_name *e,
			       পূর্ণসংখ্যা prio,
			       ধ্রুবক কাঠামো object_id *oid,
			       কাঠামো tag **tag)
{
	যদি (!e || e->prio < prio)
		ফেরত 1;

	যদি (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		কাঠামো tag *t;

		যদি (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			যদি (!t || parse_tag(the_repository, t))
				ফেরত 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		যদি (!t || parse_tag(the_repository, t))
			ফেরত 0;
		*tag = t;

		যদি (e->tag->date < t->date)
			ফেরত 1;
	}

	ফেরত 0;
}

স্থির শূন্য add_to_known_names(ধ্রুবক অক্ষর *path,
			       ধ্রুবক কাঠামো object_id *peeled,
			       পূর্ণসংখ্যা prio,
			       ধ্রুবক কাঠামো object_id *oid)
{
	কাঠামো commit_name *e = find_commit_name(peeled);
	কাঠামো tag *tag = ফাঁকা;
	যদি (replace_name(e, prio, oid, &tag)) {
		যদি (!e) {
			e = xmalloc(আকার(কাঠামো commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_entry_init(&e->entry, oidhash(peeled));
			hashmap_add(&names, &e->entry);
			e->path = ফাঁকা;
		}
		e->tag = tag;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		মুক্ত(e->path);
		e->path = xstrdup(path);
	}
}

স্থির পূর্ণসংখ্যা get_name(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য UNUSED)
{
	পূর্ণসংখ্যা is_tag = 0;
	কাঠামো object_id peeled;
	পূর্ণসংখ্যা is_annotated, prio;
	ধ্রুবক অক্ষর *path_to_match = ফাঁকা;

	যদি (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} অন্যথা যদি (all) {
		যদি ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			ফেরত 0;
		}
	} অন্যথা {
		/* Reject anything outside refs/tags/ unless --all */
		ফেরত 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	যদি (exclude_patterns.nr) {
		কাঠামো string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			যদি (!wildmatch(item->string, path_to_match, 0))
				ফেরত 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	যদি (patterns.nr) {
		পূর্ণসংখ্যা found = 0;
		কাঠামো string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			যদি (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				বিরতি;
			}
		}

		যদি (!found)
			ফেরত 0;
	}

	/* Is it annotated? */
	যদি (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} অন্যথা {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	যদি (is_annotated)
		prio = 2;
	অন্যথা যদি (is_tag)
		prio = 1;
	অন্যথা
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	ফেরত 0;
}

কাঠামো possible_tag {
	কাঠামো commit_name *name;
	পূর্ণসংখ্যা depth;
	পূর্ণসংখ্যা found_order;
	unsigned flag_within;
};

স্থির পূর্ণসংখ্যা compare_pt(ধ্রুবক শূন্য *a_, ধ্রুবক শূন্য *b_)
{
	কাঠামো possible_tag *a = (কাঠামো possible_tag *)a_;
	কাঠামো possible_tag *b = (কাঠামো possible_tag *)b_;
	যদি (a->depth != b->depth)
		ফেরত a->depth - b->depth;
	যদি (a->found_order != b->found_order)
		ফেরত a->found_order - b->found_order;
	ফেরত 0;
}

স্থির unsigned long finish_depth_computation(কাঠামো prio_queue *queue,
					      কাঠামো possible_tag *best)
{
	unsigned long seen_commits = 0;
	কাঠামো oidset unflagged = OIDSET_INIT;
	কাঠামো commit *c;

	prio_queue_for_each(queue, c) {
		যদি (!(c->object.flags & best->flag_within))
			oidset_ঢোকাও(&unflagged, &c->object.oid);
	}

	যখন ((c = prio_queue_get(queue))) {
		কাঠামো commit_list *parents = c->parents;
		seen_commits++;
		যদি (c->object.flags & best->flag_within) {
			যদি (!oidset_আকার(&unflagged))
				বিরতি;
		} অন্যথা {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		যখন (parents) {
			unsigned seen, flag_before, flag_after;
			কাঠামো commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			যদি (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			যদি (!seen && !flag_after)
				oidset_ঢোকাও(&unflagged, &p->object.oid);
			যদি (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->পরবর্তী;
		}
	}
	oidset_clear(&unflagged);
	ফেরত seen_commits;
}

স্থির শূন্য append_name(কাঠামো commit_name *n, কাঠামো strbuf *dst)
{
	যদি (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		যদি (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	যদি (n->tag && !n->name_checked) {
		যদি (strcmp(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	যদি (n->tag) {
		যদি (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} অন্যথা {
		strbuf_addstr(dst, n->path);
	}
}

স্থির শূন্য append_suffix(পূর্ণসংখ্যা depth, ধ্রুবক কাঠামো object_id *oid, কাঠামো strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

স্থির শূন্য describe_commit(কাঠামো commit *cmit, কাঠামো strbuf *dst)
{
	কাঠামো commit *c, *gave_up_on = ফাঁকা;
	কাঠামো prio_queue queue = { compare_commits_by_commit_date };
	কাঠামো commit_name *n;
	কাঠামো possible_tag all_matches[MAX_TAGS];
	unsigned পূর্ণসংখ্যা match_cnt = 0, annotated_cnt = 0, cur_match;
	unsigned long seen_commits = 0;
	unsigned পূর্ণসংখ্যা unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	যদি (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		যদি (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		যদি (suffix)
			strbuf_addstr(dst, suffix);
		ফেরত;
	}

	যদি (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	যদি (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	যদি (!have_util) {
		কাঠামো hashmap_iter iter;
		কাঠামো commit *c;
		কাঠামো commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			যদি (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	যখন ((c = prio_queue_get(&queue))) {
		কাঠামো commit_list *parents = c->parents;
		কাঠামো commit_name **slot;

		seen_commits++;

		যদি (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_আকার(&names)) {
			gave_up_on = c;
			বিরতি;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : ফাঁকা;
		যদি (n) {
			যদি (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} অন্যথা যদি (match_cnt < max_candidates) {
				কাঠামো possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				যদি (n->prio == 2)
					annotated_cnt++;
			}
		}
		জন্য (cur_match = 0; cur_match < match_cnt; cur_match++) {
			কাঠামো possible_tag *t = &all_matches[cur_match];
			যদি (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		যদি (annotated_cnt && !prio_queue_আকার(&queue)) {
			পূর্ণসংখ্যা best_depth = INT_MAX;
			unsigned best_within = 0;
			জন্য (cur_match = 0; cur_match < match_cnt; cur_match++) {
				কাঠামো possible_tag *t = &all_matches[cur_match];
				যদি (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} অন্যথা যদি (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			যদি ((c->object.flags & best_within) == best_within) {
				যদি (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				বিরতি;
			}
		}
		যখন (parents) {
			কাঠামো commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			যদি (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->পরবর্তী;

			যদি (first_parent)
				বিরতি;
		}
	}

	যদি (!match_cnt) {
		কাঠামো object_id *cmit_oid = &cmit->object.oid;
		যদি (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			যদি (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			ফেরত;
		}
		যদি (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		অন্যথা
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	যদি (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	যদি (debug) {
		স্থির পূর্ণসংখ্যা label_width = -1;
		যদি (label_width < 0) {
			পূর্ণসংখ্যা i, w;
			জন্য (i = 0; i < ARRAY_আকার(prio_names); i++) {
				w = strlen(_(prio_names[i]));
				যদি (label_width < w)
					label_width = w;
			}
		}
		জন্য (cur_match = 0; cur_match < match_cnt; cur_match++) {
			কাঠামো possible_tag *t = &all_matches[cur_match];
			fprintf(stderr, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		fprintf(stderr, _("traversed %lu commits\n"), seen_commits);
		যদি (gave_up_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	যদি (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	যদি (suffix)
		strbuf_addstr(dst, suffix);
}

কাঠামো process_commit_তথ্য {
	কাঠামো commit *current_commit;
	ধ্রুবক কাঠামো object_id *looking_for;
	কাঠামো strbuf *dst;
	কাঠামো rev_info *revs;
};

স্থির শূন্য process_commit(কাঠামো commit *commit, শূন্য *তথ্য)
{
	কাঠামো process_commit_তথ্য *pcd = তথ্য;
	pcd->current_commit = commit;
}

স্থির শূন্য process_object(কাঠামো object *obj, ধ্রুবক অক্ষর *path, শূন্য *তথ্য)
{
	কাঠামো process_commit_তথ্য *pcd = তথ্য;

	যদি (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		যদি (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = ফাঁকা;
	}
}

স্থির শূন্য describe_blob(ধ্রুবক কাঠামো object_id *oid, কাঠামো strbuf *dst)
{
	কাঠামো rev_info revs;
	কাঠামো strvec args = STRVEC_INIT;
	কাঠামো object_id শীর্ষ_oid;
	কাঠামো process_commit_তথ্য pcd = { ফাঁকা, oid, dst, &revs};

	যদি (repo_get_oid(the_repository, "HEAD", &শীর্ষ_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&শীর্ষ_oid),
		     ফাঁকা);

	repo_init_revisions(the_repository, &revs, ফাঁকা);
	setup_revisions_from_strvec(&args, &revs, ফাঁকা);
	যদি (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	যদি (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	যদি (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

স্থির শূন্য describe(ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা last_one)
{
	কাঠামো object_id oid;
	কাঠামো commit *cmit;
	কাঠামো strbuf sb = STRBUF_INIT;

	যদি (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	যদি (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	যদি (cmit)
		describe_commit(cmit, &sb);
	অন্যথা যদি (odb_read_object_info(the_repository->objects,
				      &oid, ফাঁকা) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	অন্যথা
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	যদি (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

স্থির পূর্ণসংখ্যা option_parse_exact_match(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
				    পূর্ণসংখ্যা unset)
{
	পূর্ণসংখ্যা *val = opt->মান;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	ফেরত 0;
}

পূর্ণসংখ্যা cmd_describe(পূর্ণসংখ্যা argc,
		 ধ্রুবক অক্ষর **argv,
		 ধ্রুবক অক্ষর *prefix,
		 কাঠামো repository *repo UNUSED )
{
	কাঠামো refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	পূর্ণসংখ্যা contains = 0;
	কাঠামো option options[] = {
		OPT_BOOL(0, "contains",   &contains, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &tags, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, ফাঁকা,
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
			.মান = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.মান = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ফাঁকা);
	argc = parse_options(argc, argv, prefix, options, describe_usage, 0);
	যদি (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	যদি (max_candidates < 0)
		max_candidates = 0;
	অন্যথা যদি (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_বাফার = 0;

	যদি (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	যদি (contains) {
		কাঠামো string_list_item *item;
		কাঠামো strvec args;
		ধ্রুবক অক্ষর **argv_copy;
		পূর্ণসংখ্যা ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     ফাঁকা);
		যদি (always)
			strvec_ঠেলো(&args, "--always");
		যদি (!all)
			strvec_ঠেলো(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		যদি (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		যদি (argc)
			strvec_pushv(&args, argv);
		অন্যথা
			strvec_ঠেলো(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		জন্য (আকার_t i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = ফাঁকা;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		মুক্ত(argv_copy);
		ফেরত ret;
	}

	যদি (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, ফাঁকা, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, ফাঁকা, &for_each_ref_opts);
	যদি (!hashmap_get_আকার(&names) && !always)
		die(_("No names found, cannot describe anything."));

	যদি (argc == 0) {
		যদি (broken) {
			কাঠামো child_process cp = CHILD_PROCESS_INIT;

			strvec_pushv(&cp.args, update_সূচক_args);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			run_command(&cp);

			child_process_init(&cp);
			strvec_pushv(&cp.args, diff_সূচক_args);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			যদি (!dirty)
				dirty = "-dirty";

			নির্বাচন (run_command(&cp)) {
			ক্ষেত্র 0:
				suffix = ফাঁকা;
				বিরতি;
			ক্ষেত্র 1:
				suffix = dirty;
				বিরতি;
			পূর্বনির্ধারিত:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} অন্যথা যদি (dirty) {
			কাঠামো lock_file সূচক_lock = LOCK_INIT;
			কাঠামো rev_info revs;
			পূর্ণসংখ্যা fd;

			setup_work_বৃক্ষ(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_সূচক = 0;
			repo_read_সূচক(the_repository);
			refresh_সূচক(the_repository->সূচক, REFRESH_QUIET|REFRESH_UNMERGED,
				      ফাঁকা, ফাঁকা, ফাঁকা);
			fd = repo_hold_locked_সূচক(the_repository,
						    &সূচক_lock, 0);
			যদি (0 <= fd)
				repo_update_সূচক_if_able(the_repository, &সূচক_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			যদি (setup_revisions(ARRAY_আকার(diff_সূচক_args) - 1,
					    diff_সূচক_args, &revs, ফাঁকা) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_সূচক(&revs, 0);

			যদি (!diff_ফলাফল_code(&revs))
				suffix = ফাঁকা;
			অন্যথা
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} অন্যথা যদি (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} অন্যথা যদি (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} অন্যথা {
		যখন (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	ফেরত 0;
}
