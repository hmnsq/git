#অন্তর্ভাব "git-compat-util.h"
#অন্তর্ভাব "bloom.h"
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "commit-graph.h"
#অন্তর্ভাব "commit-slab.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "diffcore.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "ewah/ewok.h"
#অন্তর্ভাব "hashmap.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "object.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "prio-queue.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "repository.h"
#অন্তর্ভাব "revision.h"

/* Remember to update object flag allocation in object.h */
#define PARENT1 (1u<<16) /* used instead of SEEN */
#define PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

কাঠামো last_modified_entry {
	কাঠামো hashmap_entry hashent;
	কাঠামো object_id oid;
	কাঠামো bloom_চাবি চাবি;
	আকার_t diff_idx;
	ধ্রুবক অক্ষর path[FLEX_ARRAY];
};

স্থির পূর্ণসংখ্যা last_modified_entry_hashcmp(ধ্রুবক শূন্য *unused UNUSED,
				       ধ্রুবক কাঠামো hashmap_entry *hent1,
				       ধ্রুবক কাঠামো hashmap_entry *hent2,
				       ধ্রুবক শূন্য *path)
{
	ধ্রুবক কাঠামো last_modified_entry *ent1 =
		container_of(hent1, ধ্রুবক কাঠামো last_modified_entry, hashent);
	ধ্রুবক কাঠামো last_modified_entry *ent2 =
		container_of(hent2, ধ্রুবক কাঠামো last_modified_entry, hashent);
	ফেরত strcmp(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, কাঠামো bitmap *);

কাঠামো last_modified {
	কাঠামো hashmap paths;
	কাঠামো rev_info rev;
	bool show_trees;
	bool nul_termination;
	পূর্ণসংখ্যা max_depth;

	ধ্রুবক অক্ষর **all_paths;
	আকার_t all_paths_nr;
	কাঠামো active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	কাঠামো bitmap *scratch;
};

স্থির কাঠামো bitmap *active_paths_for(কাঠামো last_modified *lm, কাঠামো commit *c)
{
	কাঠামো bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	যদি (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	ফেরত *bitmap;
}

স্থির শূন্য active_paths_free(কাঠামো last_modified *lm, কাঠামো commit *c)
{
	কাঠামো bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	যদি (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = ফাঁকা;
	}
}

স্থির শূন্য last_modified_release(কাঠামো last_modified *lm)
{
	কাঠামো hashmap_iter iter;
	কাঠামো last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_চাবি_clear(&ent->চাবি);

	hashmap_clear_and_free(&lm->paths, কাঠামো last_modified_entry, hashent);
	release_revisions(&lm->rev);

	মুক্ত(lm->all_paths);
}

কাঠামো last_modified_callback_তথ্য {
	কাঠামো last_modified *lm;
	কাঠামো commit *commit;
};

স্থির শূন্য add_path_from_diff(কাঠামো diff_queue_struct *q,
			       কাঠামো diff_options *opt UNUSED, শূন্য *তথ্য)
{
	কাঠামো last_modified *lm = তথ্য;

	জন্য (পূর্ণসংখ্যা i = 0; i < q->nr; i++) {
		কাঠামো diff_filepair *p = q->queue[i];
		কাঠামো last_modified_entry *ent;
		ধ্রুবক অক্ষর *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		যদি (lm->rev.bloom_filter_settings)
			bloom_চাবি_fill(&ent->চাবি, path, strlen(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

স্থির পূর্ণসংখ্যা populate_paths_from_revs(কাঠামো last_modified *lm)
{
	পূর্ণসংখ্যা অঙ্ক_interesting = 0, ret = 0;
	কাঠামো diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	memcpy(&diffopt, &lm->rev.diffopt, আকার(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_তথ্য = lm;

	জন্য (আকার_t i = 0; i < lm->rev.pending.nr; i++) {
		কাঠামো object_array_entry *obj = lm->rev.pending.objects + i;

		যদি (obj->item->flags & UNINTERESTING)
			চালু;

		যদি (অঙ্ক_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			যাও out;
		}

		যদি (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			যাও out;
		}

		diff_বৃক্ষ_oid(lm->rev.repo->hash_algo->empty_বৃক্ষ,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	ফেরত ret;
}

স্থির শূন্য last_modified_emit(কাঠামো last_modified *lm,
			       ধ্রুবক অক্ষর *path, ধ্রুবক কাঠামো commit *commit)

{
	যদি (commit->object.flags & BOUNDARY)
		putchar('^');
	printf("%s\t", oid_to_hex(&commit->object.oid));

	যদি (lm->nul_termination)
		printf("%s%c", path, '\0');
	অন্যথা
		write_name_quoted(path, stdout, '\n');
}

স্থির শূন্য mark_path(ধ্রুবক অক্ষর *path, ধ্রুবক কাঠামো object_id *oid,
		      কাঠামো last_modified_callback_তথ্য *তথ্য)
{
	কাঠামো last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&তথ্য->lm->paths, strhash(path), path,
					  কাঠামো last_modified_entry, hashent);
	যদি (!ent)
		ফেরত;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	যদি (oid && !oideq(oid, &ent->oid))
		ফেরত;

	last_modified_emit(তথ্য->lm, path, তথ্য->commit);

	hashmap_remove(&তথ্য->lm->paths, &ent->hashent, path);
	bloom_চাবি_clear(&ent->চাবি);
	মুক্ত(ent);
}

স্থির শূন্য last_modified_diff(কাঠামো diff_queue_struct *q,
			       কাঠামো diff_options *opt UNUSED, শূন্য *cbdata)
{
	কাঠামো last_modified_callback_তথ্য *তথ্য = cbdata;

	জন্য (পূর্ণসংখ্যা i = 0; i < q->nr; i++) {
		কাঠামো diff_filepair *p = q->queue[i];
		নির্বাচন (p->status) {
		ক্ষেত্র DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			বিরতি;

		পূর্বনির্ধারিত:
			/*
			 * Otherwise, we care only that we somehow arrived at
			 * a final oid state. Note that this covers some
			 * potentially controversial areas, including:
			 *
			 *  1. A rename or copy will be found, as it is the
			 *     first time the content has arrived at the given
			 *     path.
			 *
			 *  2. Even a non-content modification like a mode or
			 *     type change will trigger it.
			 *
			 * We take the inclusive approach for now, and find
			 * anything which impacts the path. Options to tweak
			 * the behavior (e.g., to "--follow" the content across
			 * renames) can come later.
			 */
			mark_path(p->two->path, &p->two->oid, তথ্য);
			বিরতি;
		}
	}
}

স্থির শূন্য pass_to_parent(কাঠামো bitmap *c,
			   কাঠামো bitmap *p,
			   আকার_t pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

স্থির bool maybe_changed_path(কাঠামো last_modified *lm,
			       কাঠামো commit *origin,
			       কাঠামো bitmap *active)
{
	কাঠামো bloom_filter *filter;
	কাঠামো last_modified_entry *ent;
	কাঠামো hashmap_iter iter;

	যদি (!lm->rev.bloom_filter_settings)
		ফেরত true;

	যদি (commit_graph_generation(origin) == GENERATION_সংখ্যা_INFINITY)
		ফেরত true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	যদি (!filter)
		ফেরত true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		যদি (active && !bitmap_get(active, ent->diff_idx))
			চালু;

		যদি (bloom_filter_contains(filter, &ent->চাবি,
					  lm->rev.bloom_filter_settings))
			ফেরত true;
	}
	ফেরত false;
}

স্থির শূন্য process_parent(কাঠামো last_modified *lm,
			   কাঠামো prio_queue *queue,
			   কাঠামো commit *c, কাঠামো bitmap *active_c,
			   কাঠামো commit *parent, পূর্ণসংখ্যা parent_i)
{
	কাঠামো bitmap *active_p;

	যদি (repo_parse_commit(lm->rev.repo, parent))
		ফেরত;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	যদি (parent_i || maybe_changed_path(lm, c, active_c)) {
		diff_বৃক্ষ_oid(&parent->object.oid,
			      &c->object.oid, "", &lm->rev.diffopt);
		diffcore_std(&lm->rev.diffopt);
	}

	/*
	 * Test each path for TREESAME-ness against the parent. If a path is
	 * TREESAME, pass it on to this parent.
	 *
	 * First, collect all paths that are *not* TREESAME in 'scratch'.
	 * Then, pass paths that *are* TREESAME and active to the parent.
	 */
	জন্য (পূর্ণসংখ্যা i = 0; i < diff_queued_diff.nr; i++) {
		কাঠামো diff_filepair *fp = diff_queued_diff.queue[i];
		ধ্রুবক অক্ষর *path = fp->two->path;
		কাঠামো last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    কাঠামো last_modified_entry, hashent);
		যদি (ent) {
			আকার_t k = ent->diff_idx;
			যদি (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	জন্য (আকার_t i = 0; i < lm->all_paths_nr; i++) {
		যদি (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	যদি (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	যদি (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->শব্দগুলি, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

স্থির পূর্ণসংখ্যা last_modified_run(কাঠামো last_modified *lm)
{
	পূর্ণসংখ্যা max_গুনতি, queue_popped = 0;
	কাঠামো commit *c, *n;
	কাঠামো prio_queue queue = { compare_commits_by_gen_then_commit_date };
	কাঠামো prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	কাঠামো commit_list *list;
	কাঠামো last_modified_callback_তথ্য তথ্য = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_তথ্য = &তথ্য;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_গুনতি = lm->rev.max_গুনতি;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	জন্য (list = lm->rev.commits; list; list = list->পরবর্তী) {
		কাঠামো commit *c = list->item;

		যদি (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} অন্যথা যদি (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			কাঠামো bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			জন্য (আকার_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	যখন ((c = prio_queue_get(&queue))) {
		পূর্ণসংখ্যা parent_i;
		কাঠামো commit_list *p;
		কাঠামো bitmap *active_c = active_paths_for(lm, c);

		যদি ((0 <= max_গুনতি && max_গুনতি < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			তথ্য.commit = c;
			diff_বৃক্ষ_oid(lm->rev.repo->hash_algo->empty_বৃক্ষ,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			যাও cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		যদি (repo_parse_commit(lm->rev.repo, c))
			যাও cleanup;

		যখন ((n = prio_queue_get(&not_queue))) {
			কাঠামো commit_list *np;

			যদি (repo_parse_commit(lm->rev.repo, n))
				চালু;

			জন্য (np = n->parents; np; np = np->পরবর্তী) {
				যদি (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			যদি (commit_graph_generation(n) < commit_graph_generation(c))
				বিরতি;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		জন্য (p = c->parents, parent_i = 0; p; p = p->পরবর্তী, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			যদি (bitmap_is_empty(active_c))
				বিরতি;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		যদি (!bitmap_is_empty(active_c))  {
			তথ্য.commit = c;
			জন্য (আকার_t i = 0; i < lm->all_paths_nr; i++) {
				যদি (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], ফাঁকা, &তথ্য);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	যদি (hashmap_get_আকার(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা last_modified_init(কাঠামো last_modified *lm, কাঠামো repository *r,
			      ধ্রুবক অক্ষর *prefix, পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	কাঠামো hashmap_iter iter;
	কাঠামো last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, ফাঁকা, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_মূল_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_বৃক্ষ_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.বৃক্ষ_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, ফাঁকা);
	যদি (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		ফেরত argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	যদি (populate_paths_from_revs(lm) < 0)
		ফেরত -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_আকার(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	ফেরত 0;
}

পূর্ণসংখ্যা cmd_last_modified(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		      কাঠামো repository *repo)
{
	পূর্ণসংখ্যা ret;
	কাঠামো last_modified lm = { 0 };

	ধ্রুবক অক্ষর * ধ্রুবক last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		ফাঁকা
	};

	কাঠামো option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', ফাঁকা, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, ফাঁকা);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	যদি (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	যদি (ret)
		যাও out;

	ret = last_modified_run(&lm);
	যদি (ret)
		যাও out;

out:
	last_modified_release(&lm);

	ফেরত ret;
}
