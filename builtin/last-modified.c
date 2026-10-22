#ಅನ್ತರ್ಭಾವ "git-compat-util.h"
#ಅನ್ತರ್ಭಾವ "bloom.h"
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "commit-graph.h"
#ಅನ್ತರ್ಭಾವ "commit-slab.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "diffcore.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "ewah/ewok.h"
#ಅನ್ತರ್ಭಾವ "hashmap.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "prio-queue.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "repository.h"
#ಅನ್ತರ್ಭಾವ "revision.h"

/* Remember to update object flag allocation in object.h */
#define PARENT1 (1u<<16) /* used instead of SEEN */
#define PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

ಸಂರಚನಾ last_modified_entry {
	ಸಂರಚನಾ hashmap_entry hashent;
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ bloom_ಕೀಲಿ ಕೀಲಿ;
	size_t diff_idx;
	ಧ್ರುವ ವರ್ಣ path[FLEX_ARRAY];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ last_modified_entry_hashcmp(ಧ್ರುವ ಶೂನ್ಯ *unused UNUSED,
				       ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *hent1,
				       ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *hent2,
				       ಧ್ರುವ ಶೂನ್ಯ *path)
{
	ಧ್ರುವ ಸಂರಚನಾ last_modified_entry *ent1 =
		container_of(hent1, ಧ್ರುವ ಸಂರಚನಾ last_modified_entry, hashent);
	ಧ್ರುವ ಸಂರಚನಾ last_modified_entry *ent2 =
		container_of(hent2, ಧ್ರುವ ಸಂರಚನಾ last_modified_entry, hashent);
	ಪ್ರತ್ಯಾಗಮ strcmp(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, ಸಂರಚನಾ bitmap *);

ಸಂರಚನಾ last_modified {
	ಸಂರಚನಾ hashmap paths;
	ಸಂರಚನಾ rev_info rev;
	bool show_trees;
	bool nul_termination;
	ಪೂರ್ಣಾಙ್ಕ max_depth;

	ಧ್ರುವ ವರ್ಣ **all_paths;
	size_t all_paths_nr;
	ಸಂರಚನಾ active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	ಸಂರಚನಾ bitmap *scratch;
};

ಸ್ಥಿರ ಸಂರಚನಾ bitmap *active_paths_for(ಸಂರಚನಾ last_modified *lm, ಸಂರಚನಾ commit *c)
{
	ಸಂರಚನಾ bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	ಯದಿ (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	ಪ್ರತ್ಯಾಗಮ *bitmap;
}

ಸ್ಥಿರ ಶೂನ್ಯ active_paths_free(ಸಂರಚನಾ last_modified *lm, ಸಂರಚನಾ commit *c)
{
	ಸಂರಚನಾ bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	ಯದಿ (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = ರಿಕ್ತ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ last_modified_release(ಸಂರಚನಾ last_modified *lm)
{
	ಸಂರಚನಾ hashmap_iter iter;
	ಸಂರಚನಾ last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_ಕೀಲಿ_clear(&ent->ಕೀಲಿ);

	hashmap_clear_and_free(&lm->paths, ಸಂರಚನಾ last_modified_entry, hashent);
	release_revisions(&lm->rev);

	ಮುಕ್ತ(lm->all_paths);
}

ಸಂರಚನಾ last_modified_callback_data {
	ಸಂರಚನಾ last_modified *lm;
	ಸಂರಚನಾ commit *commit;
};

ಸ್ಥಿರ ಶೂನ್ಯ add_path_from_diff(ಸಂರಚನಾ diff_queue_struct *q,
			       ಸಂರಚನಾ diff_options *opt UNUSED, ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ last_modified *lm = data;

	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < q->nr; i++) {
		ಸಂರಚನಾ diff_filepair *p = q->queue[i];
		ಸಂರಚನಾ last_modified_entry *ent;
		ಧ್ರುವ ವರ್ಣ *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		ಯದಿ (lm->rev.bloom_filter_settings)
			bloom_ಕೀಲಿ_fill(&ent->ಕೀಲಿ, path, strlen(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ populate_paths_from_revs(ಸಂರಚನಾ last_modified *lm)
{
	ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_interesting = 0, ret = 0;
	ಸಂರಚನಾ diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	memcpy(&diffopt, &lm->rev.diffopt, ಪರಿಮಾಣ(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_data = lm;

	ಪ್ರತಿ (size_t i = 0; i < lm->rev.pending.nr; i++) {
		ಸಂರಚನಾ object_array_entry *obj = lm->rev.pending.objects + i;

		ಯದಿ (obj->item->flags & UNINTERESTING)
			ಅನುವರ್ತ;

		ಯದಿ (ಅಂಕೆ_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			ಗಚ್ಛ out;
		}

		ಯದಿ (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			ಗಚ್ಛ out;
		}

		diff_ವೃಕ್ಷ_oid(lm->rev.repo->hash_algo->empty_ವೃಕ್ಷ,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ last_modified_emit(ಸಂರಚನಾ last_modified *lm,
			       ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ಸಂರಚನಾ commit *commit)

{
	ಯದಿ (commit->object.flags & BOUNDARY)
		putchar('^');
	printf("%s\t", oid_to_hex(&commit->object.oid));

	ಯದಿ (lm->nul_termination)
		printf("%s%c", path, '\0');
	ಅನ್ಯಥಾ
		write_name_quoted(path, stdout, '\n');
}

ಸ್ಥಿರ ಶೂನ್ಯ mark_path(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
		      ಸಂರಚನಾ last_modified_callback_data *data)
{
	ಸಂರಚನಾ last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&data->lm->paths, strhash(path), path,
					  ಸಂರಚನಾ last_modified_entry, hashent);
	ಯದಿ (!ent)
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	ಯದಿ (oid && !oideq(oid, &ent->oid))
		ಪ್ರತ್ಯಾಗಮ;

	last_modified_emit(data->lm, path, data->commit);

	hashmap_remove(&data->lm->paths, &ent->hashent, path);
	bloom_ಕೀಲಿ_clear(&ent->ಕೀಲಿ);
	ಮುಕ್ತ(ent);
}

ಸ್ಥಿರ ಶೂನ್ಯ last_modified_diff(ಸಂರಚನಾ diff_queue_struct *q,
			       ಸಂರಚನಾ diff_options *opt UNUSED, ಶೂನ್ಯ *cbdata)
{
	ಸಂರಚನಾ last_modified_callback_data *data = cbdata;

	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < q->nr; i++) {
		ಸಂರಚನಾ diff_filepair *p = q->queue[i];
		ವಿಕಲ್ಪ (p->status) {
		ಪಕ್ಷ DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			ಭಙ್ಗ;

		ಸ್ವಭಾವ:
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
			mark_path(p->two->path, &p->two->oid, data);
			ಭಙ್ಗ;
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ pass_to_parent(ಸಂರಚನಾ bitmap *c,
			   ಸಂರಚನಾ bitmap *p,
			   size_t pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

ಸ್ಥಿರ bool maybe_changed_path(ಸಂರಚನಾ last_modified *lm,
			       ಸಂರಚನಾ commit *origin,
			       ಸಂರಚನಾ bitmap *active)
{
	ಸಂರಚನಾ bloom_filter *filter;
	ಸಂರಚನಾ last_modified_entry *ent;
	ಸಂರಚನಾ hashmap_iter iter;

	ಯದಿ (!lm->rev.bloom_filter_settings)
		ಪ್ರತ್ಯಾಗಮ true;

	ಯದಿ (commit_graph_generation(origin) == GENERATION_ಸಂಖ್ಯೆ_INFINITY)
		ಪ್ರತ್ಯಾಗಮ true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	ಯದಿ (!filter)
		ಪ್ರತ್ಯಾಗಮ true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ಯದಿ (active && !bitmap_get(active, ent->diff_idx))
			ಅನುವರ್ತ;

		ಯದಿ (bloom_filter_contains(filter, &ent->ಕೀಲಿ,
					  lm->rev.bloom_filter_settings))
			ಪ್ರತ್ಯಾಗಮ true;
	}
	ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ process_parent(ಸಂರಚನಾ last_modified *lm,
			   ಸಂರಚನಾ prio_queue *queue,
			   ಸಂರಚನಾ commit *c, ಸಂರಚನಾ bitmap *active_c,
			   ಸಂರಚನಾ commit *parent, ಪೂರ್ಣಾಙ್ಕ parent_i)
{
	ಸಂರಚನಾ bitmap *active_p;

	ಯದಿ (repo_parse_commit(lm->rev.repo, parent))
		ಪ್ರತ್ಯಾಗಮ;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	ಯದಿ (parent_i || maybe_changed_path(lm, c, active_c)) {
		diff_ವೃಕ್ಷ_oid(&parent->object.oid,
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
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < diff_queued_diff.nr; i++) {
		ಸಂರಚನಾ diff_filepair *fp = diff_queued_diff.queue[i];
		ಧ್ರುವ ವರ್ಣ *path = fp->two->path;
		ಸಂರಚನಾ last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    ಸಂರಚನಾ last_modified_entry, hashent);
		ಯದಿ (ent) {
			size_t k = ent->diff_idx;
			ಯದಿ (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	ಪ್ರತಿ (size_t i = 0; i < lm->all_paths_nr; i++) {
		ಯದಿ (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	ಯದಿ (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	ಯದಿ (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->ಪದಗಳು, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ last_modified_run(ಸಂರಚನಾ last_modified *lm)
{
	ಪೂರ್ಣಾಙ್ಕ max_ಎಣಿಕೆ, queue_popped = 0;
	ಸಂರಚನಾ commit *c, *n;
	ಸಂರಚನಾ prio_queue queue = { compare_commits_by_gen_then_commit_date };
	ಸಂರಚನಾ prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	ಸಂರಚನಾ commit_list *list;
	ಸಂರಚನಾ last_modified_callback_data data = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_data = &data;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_ಎಣಿಕೆ = lm->rev.max_ಎಣಿಕೆ;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	ಪ್ರತಿ (list = lm->rev.commits; list; list = list->next) {
		ಸಂರಚನಾ commit *c = list->item;

		ಯದಿ (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} ಅನ್ಯಥಾ ಯದಿ (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			ಸಂರಚನಾ bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			ಪ್ರತಿ (size_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	ಯಾವತ್ ((c = prio_queue_get(&queue))) {
		ಪೂರ್ಣಾಙ್ಕ parent_i;
		ಸಂರಚನಾ commit_list *p;
		ಸಂರಚನಾ bitmap *active_c = active_paths_for(lm, c);

		ಯದಿ ((0 <= max_ಎಣಿಕೆ && max_ಎಣಿಕೆ < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			data.commit = c;
			diff_ವೃಕ್ಷ_oid(lm->rev.repo->hash_algo->empty_ವೃಕ್ಷ,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			ಗಚ್ಛ cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		ಯದಿ (repo_parse_commit(lm->rev.repo, c))
			ಗಚ್ಛ cleanup;

		ಯಾವತ್ ((n = prio_queue_get(&not_queue))) {
			ಸಂರಚನಾ commit_list *np;

			ಯದಿ (repo_parse_commit(lm->rev.repo, n))
				ಅನುವರ್ತ;

			ಪ್ರತಿ (np = n->parents; np; np = np->next) {
				ಯದಿ (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			ಯದಿ (commit_graph_generation(n) < commit_graph_generation(c))
				ಭಙ್ಗ;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		ಪ್ರತಿ (p = c->parents, parent_i = 0; p; p = p->next, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			ಯದಿ (bitmap_is_empty(active_c))
				ಭಙ್ಗ;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		ಯದಿ (!bitmap_is_empty(active_c))  {
			data.commit = c;
			ಪ್ರತಿ (size_t i = 0; i < lm->all_paths_nr; i++) {
				ಯದಿ (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], ರಿಕ್ತ, &data);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	ಯದಿ (hashmap_get_size(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ last_modified_init(ಸಂರಚನಾ last_modified *lm, ಸಂರಚನಾ repository *r,
			      ಧ್ರುವ ವರ್ಣ *prefix, ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	ಸಂರಚನಾ hashmap_iter iter;
	ಸಂರಚನಾ last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, ರಿಕ್ತ, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_ಬೇರು_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_ವೃಕ್ಷ_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.ವೃಕ್ಷ_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, ರಿಕ್ತ);
	ಯದಿ (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		ಪ್ರತ್ಯಾಗಮ argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	ಯದಿ (populate_paths_from_revs(lm) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_size(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_last_modified(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		      ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ last_modified lm = { 0 };

	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		ರಿಕ್ತ
	};

	ಸಂರಚನಾ option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', ರಿಕ್ತ, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, ರಿಕ್ತ);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	ಯದಿ (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	ಯದಿ (ret)
		ಗಚ್ಛ out;

	ret = last_modified_run(&lm);
	ಯದಿ (ret)
		ಗಚ್ಛ out;

out:
	last_modified_release(&lm);

	ಪ್ರತ್ಯಾಗಮ ret;
}
