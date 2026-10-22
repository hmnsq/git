#ଅନ୍ତର୍ଭାଵ "git-compat-util.h"
#ଅନ୍ତର୍ଭାଵ "bloom.h"
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "commit-graph.h"
#ଅନ୍ତର୍ଭାଵ "commit-slab.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "diffcore.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "ewah/ewok.h"
#ଅନ୍ତର୍ଭାଵ "hashmap.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "prio-queue.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "repository.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"

/* Remember to update object flag allocation in object.h */
#define PARENT1 (1u<<16) /* used instead of SEEN */
#define PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

ସଂରଚନା last_modified_entry {
	ସଂରଚନା hashmap_entry hashent;
	ସଂରଚନା object_id oid;
	ସଂରଚନା bloom_ଚାବି ଚାବି;
	size_t diff_idx;
	ଅଚଳ ଅକ୍ଷର path[FLEX_ARRAY];
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_modified_entry_hashcmp(ଅଚଳ ଶୂନ୍ୟ *unused UNUSED,
				       ଅଚଳ ସଂରଚନା hashmap_entry *hent1,
				       ଅଚଳ ସଂରଚନା hashmap_entry *hent2,
				       ଅଚଳ ଶୂନ୍ୟ *path)
{
	ଅଚଳ ସଂରଚନା last_modified_entry *ent1 =
		container_of(hent1, ଅଚଳ ସଂରଚନା last_modified_entry, hashent);
	ଅଚଳ ସଂରଚନା last_modified_entry *ent2 =
		container_of(hent2, ଅଚଳ ସଂରଚନା last_modified_entry, hashent);
	ଫେରନ୍ତୁ strcmp(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, ସଂରଚନା bitmap *);

ସଂରଚନା last_modified {
	ସଂରଚନା hashmap paths;
	ସଂରଚନା rev_info rev;
	bool show_trees;
	bool nul_termination;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_depth;

	ଅଚଳ ଅକ୍ଷର **all_paths;
	size_t all_paths_nr;
	ସଂରଚନା active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	ସଂରଚନା bitmap *scratch;
};

ସ୍ଥିର ସଂରଚନା bitmap *active_paths_for(ସଂରଚନା last_modified *lm, ସଂରଚନା commit *c)
{
	ସଂରଚନା bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	ଯଦି (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	ଫେରନ୍ତୁ *bitmap;
}

ସ୍ଥିର ଶୂନ୍ୟ active_paths_free(ସଂରଚନା last_modified *lm, ସଂରଚନା commit *c)
{
	ସଂରଚନା bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	ଯଦି (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = ଖାଲି;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ last_modified_release(ସଂରଚନା last_modified *lm)
{
	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_ଚାବି_clear(&ent->ଚାବି);

	hashmap_clear_and_free(&lm->paths, ସଂରଚନା last_modified_entry, hashent);
	release_revisions(&lm->rev);

	ମୁକ୍ତ(lm->all_paths);
}

ସଂରଚନା last_modified_callback_data {
	ସଂରଚନା last_modified *lm;
	ସଂରଚନା commit *commit;
};

ସ୍ଥିର ଶୂନ୍ୟ add_path_from_diff(ସଂରଚନା diff_queue_struct *q,
			       ସଂରଚନା diff_options *opt UNUSED, ଶୂନ୍ୟ *data)
{
	ସଂରଚନା last_modified *lm = data;

	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < q->nr; i++) {
		ସଂରଚନା diff_filepair *p = q->queue[i];
		ସଂରଚନା last_modified_entry *ent;
		ଅଚଳ ଅକ୍ଷର *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		ଯଦି (lm->rev.bloom_filter_settings)
			bloom_ଚାବି_fill(&ent->ଚାବି, path, strlen(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା populate_paths_from_revs(ସଂରଚନା last_modified *lm)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_interesting = 0, ret = 0;
	ସଂରଚନା diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	memcpy(&diffopt, &lm->rev.diffopt, ଆକାର(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_data = lm;

	ପାଇଁ (size_t i = 0; i < lm->rev.pending.nr; i++) {
		ସଂରଚନା object_array_entry *obj = lm->rev.pending.objects + i;

		ଯଦି (obj->item->flags & UNINTERESTING)
			ଜାରି;

		ଯଦି (ଅଙ୍କ_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			ଯାଅ out;
		}

		ଯଦି (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			ଯାଅ out;
		}

		diff_ବୃକ୍ଷ_oid(lm->rev.repo->hash_algo->empty_ବୃକ୍ଷ,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ last_modified_emit(ସଂରଚନା last_modified *lm,
			       ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ସଂରଚନା commit *commit)

{
	ଯଦି (commit->object.flags & BOUNDARY)
		putchar('^');
	printf("%s\t", oid_to_hex(&commit->object.oid));

	ଯଦି (lm->nul_termination)
		printf("%s%c", path, '\0');
	ଅନ୍ୟଥା
		write_name_quoted(path, stdout, '\n');
}

ସ୍ଥିର ଶୂନ୍ୟ mark_path(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ସଂରଚନା object_id *oid,
		      ସଂରଚନା last_modified_callback_data *data)
{
	ସଂରଚନା last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&data->lm->paths, strhash(path), path,
					  ସଂରଚନା last_modified_entry, hashent);
	ଯଦି (!ent)
		ଫେରନ୍ତୁ;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	ଯଦି (oid && !oideq(oid, &ent->oid))
		ଫେରନ୍ତୁ;

	last_modified_emit(data->lm, path, data->commit);

	hashmap_remove(&data->lm->paths, &ent->hashent, path);
	bloom_ଚାବି_clear(&ent->ଚାବି);
	ମୁକ୍ତ(ent);
}

ସ୍ଥିର ଶୂନ୍ୟ last_modified_diff(ସଂରଚନା diff_queue_struct *q,
			       ସଂରଚନା diff_options *opt UNUSED, ଶୂନ୍ୟ *cbdata)
{
	ସଂରଚନା last_modified_callback_data *data = cbdata;

	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < q->nr; i++) {
		ସଂରଚନା diff_filepair *p = q->queue[i];
		ବାଛ (p->status) {
		ସ୍ଥିତି DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			ଭାଙ୍ଗ;

		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
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
			ଭାଙ୍ଗ;
		}
	}
}

ସ୍ଥିର ଶୂନ୍ୟ pass_to_parent(ସଂରଚନା bitmap *c,
			   ସଂରଚନା bitmap *p,
			   size_t pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

ସ୍ଥିର bool maybe_changed_path(ସଂରଚନା last_modified *lm,
			       ସଂରଚନା commit *origin,
			       ସଂରଚନା bitmap *active)
{
	ସଂରଚନା bloom_filter *filter;
	ସଂରଚନା last_modified_entry *ent;
	ସଂରଚନା hashmap_iter iter;

	ଯଦି (!lm->rev.bloom_filter_settings)
		ଫେରନ୍ତୁ true;

	ଯଦି (commit_graph_generation(origin) == GENERATION_ସଂଖ୍ୟା_INFINITY)
		ଫେରନ୍ତୁ true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	ଯଦି (!filter)
		ଫେରନ୍ତୁ true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ଯଦି (active && !bitmap_get(active, ent->diff_idx))
			ଜାରି;

		ଯଦି (bloom_filter_contains(filter, &ent->ଚାବି,
					  lm->rev.bloom_filter_settings))
			ଫେରନ୍ତୁ true;
	}
	ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ process_parent(ସଂରଚନା last_modified *lm,
			   ସଂରଚନା prio_queue *queue,
			   ସଂରଚନା commit *c, ସଂରଚନା bitmap *active_c,
			   ସଂରଚନା commit *parent, ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent_i)
{
	ସଂରଚନା bitmap *active_p;

	ଯଦି (repo_parse_commit(lm->rev.repo, parent))
		ଫେରନ୍ତୁ;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	ଯଦି (parent_i || maybe_changed_path(lm, c, active_c)) {
		diff_ବୃକ୍ଷ_oid(&parent->object.oid,
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
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < diff_queued_diff.nr; i++) {
		ସଂରଚନା diff_filepair *fp = diff_queued_diff.queue[i];
		ଅଚଳ ଅକ୍ଷର *path = fp->two->path;
		ସଂରଚନା last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    ସଂରଚନା last_modified_entry, hashent);
		ଯଦି (ent) {
			size_t k = ent->diff_idx;
			ଯଦି (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	ପାଇଁ (size_t i = 0; i < lm->all_paths_nr; i++) {
		ଯଦି (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	ଯଦି (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	ଯଦି (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->ଶବ୍ଦଗୁଡ଼ିକ, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_modified_run(ସଂରଚନା last_modified *lm)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_ଗଣନା, queue_popped = 0;
	ସଂରଚନା commit *c, *n;
	ସଂରଚନା prio_queue queue = { compare_commits_by_gen_then_commit_date };
	ସଂରଚନା prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	ସଂରଚନା commit_list *list;
	ସଂରଚନା last_modified_callback_data data = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_data = &data;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_ଗଣନା = lm->rev.max_ଗଣନା;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	ପାଇଁ (list = lm->rev.commits; list; list = list->next) {
		ସଂରଚନା commit *c = list->item;

		ଯଦି (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} ଅନ୍ୟଥା ଯଦି (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			ସଂରଚନା bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			ପାଇଁ (size_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	ଯେତେବେଳେ ((c = prio_queue_get(&queue))) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent_i;
		ସଂରଚନା commit_list *p;
		ସଂରଚନା bitmap *active_c = active_paths_for(lm, c);

		ଯଦି ((0 <= max_ଗଣନା && max_ଗଣନା < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			data.commit = c;
			diff_ବୃକ୍ଷ_oid(lm->rev.repo->hash_algo->empty_ବୃକ୍ଷ,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			ଯାଅ cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		ଯଦି (repo_parse_commit(lm->rev.repo, c))
			ଯାଅ cleanup;

		ଯେତେବେଳେ ((n = prio_queue_get(&not_queue))) {
			ସଂରଚନା commit_list *np;

			ଯଦି (repo_parse_commit(lm->rev.repo, n))
				ଜାରି;

			ପାଇଁ (np = n->parents; np; np = np->next) {
				ଯଦି (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			ଯଦି (commit_graph_generation(n) < commit_graph_generation(c))
				ଭାଙ୍ଗ;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		ପାଇଁ (p = c->parents, parent_i = 0; p; p = p->next, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			ଯଦି (bitmap_is_empty(active_c))
				ଭାଙ୍ଗ;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		ଯଦି (!bitmap_is_empty(active_c))  {
			data.commit = c;
			ପାଇଁ (size_t i = 0; i < lm->all_paths_nr; i++) {
				ଯଦି (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], ଖାଲି, &data);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	ଯଦି (hashmap_get_size(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_modified_init(ସଂରଚନା last_modified *lm, ସଂରଚନା repository *r,
			      ଅଚଳ ଅକ୍ଷର *prefix, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, ଖାଲି, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_ମୂଳ_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_ବୃକ୍ଷ_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.ବୃକ୍ଷ_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, ଖାଲି);
	ଯଦି (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		ଫେରନ୍ତୁ argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	ଯଦି (populate_paths_from_revs(lm) < 0)
		ଫେରନ୍ତୁ -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_size(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_last_modified(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		      ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା last_modified lm = { 0 };

	ଅଚଳ ଅକ୍ଷର * ଅଚଳ last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		ଖାଲି
	};

	ସଂରଚନା option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', ଖାଲି, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, ଖାଲି);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	ଯଦି (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	ଯଦି (ret)
		ଯାଅ out;

	ret = last_modified_run(&lm);
	ଯଦି (ret)
		ଯାଅ out;

out:
	last_modified_release(&lm);

	ଫେରନ୍ତୁ ret;
}
