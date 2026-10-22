#அந்தர்பாவ "git-compat-util.h"
#அந்தர்பாவ "bloom.h"
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "commit-graph.h"
#அந்தர்பாவ "commit-slab.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "diffcore.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "ewah/ewok.h"
#அந்தர்பாவ "hashmap.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "prio-queue.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "repository.h"
#அந்தர்பாவ "revision.h"

/* Remember to update object flag allocation in object.h */
#define PARENT1 (1u<<16) /* used instead of SEEN */
#define PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

கட்டமைப்பு last_modified_entry {
	கட்டமைப்பு hashmap_entry hashent;
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு bloom_சாவி சாவி;
	size_t diff_idx;
	மாறிலி எழுத்து path[FLEX_ARRAY];
};

நிலையான முழு last_modified_entry_hashcmp(மாறிலி வெறுமை *unused UNUSED,
				       மாறிலி கட்டமைப்பு hashmap_entry *hent1,
				       மாறிலி கட்டமைப்பு hashmap_entry *hent2,
				       மாறிலி வெறுமை *path)
{
	மாறிலி கட்டமைப்பு last_modified_entry *ent1 =
		container_of(hent1, மாறிலி கட்டமைப்பு last_modified_entry, hashent);
	மாறிலி கட்டமைப்பு last_modified_entry *ent2 =
		container_of(hent2, மாறிலி கட்டமைப்பு last_modified_entry, hashent);
	திரும்பு strcmp(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, கட்டமைப்பு bitmap *);

கட்டமைப்பு last_modified {
	கட்டமைப்பு hashmap paths;
	கட்டமைப்பு rev_info rev;
	bool show_trees;
	bool nul_termination;
	முழு max_depth;

	மாறிலி எழுத்து **all_paths;
	size_t all_paths_nr;
	கட்டமைப்பு active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	கட்டமைப்பு bitmap *scratch;
};

நிலையான கட்டமைப்பு bitmap *active_paths_for(கட்டமைப்பு last_modified *lm, கட்டமைப்பு commit *c)
{
	கட்டமைப்பு bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	எனில் (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	திரும்பு *bitmap;
}

நிலையான வெறுமை active_paths_free(கட்டமைப்பு last_modified *lm, கட்டமைப்பு commit *c)
{
	கட்டமைப்பு bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	எனில் (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = வெற்று;
	}
}

நிலையான வெறுமை last_modified_release(கட்டமைப்பு last_modified *lm)
{
	கட்டமைப்பு hashmap_iter iter;
	கட்டமைப்பு last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_சாவி_clear(&ent->சாவி);

	hashmap_clear_and_free(&lm->paths, கட்டமைப்பு last_modified_entry, hashent);
	release_revisions(&lm->rev);

	விடுவி(lm->all_paths);
}

கட்டமைப்பு last_modified_callback_data {
	கட்டமைப்பு last_modified *lm;
	கட்டமைப்பு commit *commit;
};

நிலையான வெறுமை add_path_from_diff(கட்டமைப்பு diff_queue_struct *q,
			       கட்டமைப்பு diff_options *opt UNUSED, வெறுமை *data)
{
	கட்டமைப்பு last_modified *lm = data;

	க்கு (முழு i = 0; i < q->nr; i++) {
		கட்டமைப்பு diff_filepair *p = q->queue[i];
		கட்டமைப்பு last_modified_entry *ent;
		மாறிலி எழுத்து *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		எனில் (lm->rev.bloom_filter_settings)
			bloom_சாவி_fill(&ent->சாவி, path, strlen(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

நிலையான முழு populate_paths_from_revs(கட்டமைப்பு last_modified *lm)
{
	முழு இலக்கம்_interesting = 0, ret = 0;
	கட்டமைப்பு diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	memcpy(&diffopt, &lm->rev.diffopt, அளவு(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_data = lm;

	க்கு (size_t i = 0; i < lm->rev.pending.nr; i++) {
		கட்டமைப்பு object_array_entry *obj = lm->rev.pending.objects + i;

		எனில் (obj->item->flags & UNINTERESTING)
			தொடர்;

		எனில் (இலக்கம்_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			செல் out;
		}

		எனில் (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			செல் out;
		}

		diff_மரம்_oid(lm->rev.repo->hash_algo->empty_மரம்,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	திரும்பு ret;
}

நிலையான வெறுமை last_modified_emit(கட்டமைப்பு last_modified *lm,
			       மாறிலி எழுத்து *path, மாறிலி கட்டமைப்பு commit *commit)

{
	எனில் (commit->object.flags & BOUNDARY)
		putchar('^');
	printf("%s\t", oid_to_hex(&commit->object.oid));

	எனில் (lm->nul_termination)
		printf("%s%c", path, '\0');
	இல்லையெனில்
		write_name_quoted(path, stdout, '\n');
}

நிலையான வெறுமை mark_path(மாறிலி எழுத்து *path, மாறிலி கட்டமைப்பு object_id *oid,
		      கட்டமைப்பு last_modified_callback_data *data)
{
	கட்டமைப்பு last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&data->lm->paths, strhash(path), path,
					  கட்டமைப்பு last_modified_entry, hashent);
	எனில் (!ent)
		திரும்பு;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	எனில் (oid && !oideq(oid, &ent->oid))
		திரும்பு;

	last_modified_emit(data->lm, path, data->commit);

	hashmap_remove(&data->lm->paths, &ent->hashent, path);
	bloom_சாவி_clear(&ent->சாவி);
	விடுவி(ent);
}

நிலையான வெறுமை last_modified_diff(கட்டமைப்பு diff_queue_struct *q,
			       கட்டமைப்பு diff_options *opt UNUSED, வெறுமை *cbdata)
{
	கட்டமைப்பு last_modified_callback_data *data = cbdata;

	க்கு (முழு i = 0; i < q->nr; i++) {
		கட்டமைப்பு diff_filepair *p = q->queue[i];
		தேர்வு (p->status) {
		நிலை DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			நிறுத்து;

		இயல்பு:
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
			நிறுத்து;
		}
	}
}

நிலையான வெறுமை pass_to_parent(கட்டமைப்பு bitmap *c,
			   கட்டமைப்பு bitmap *p,
			   size_t pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

நிலையான bool maybe_changed_path(கட்டமைப்பு last_modified *lm,
			       கட்டமைப்பு commit *origin,
			       கட்டமைப்பு bitmap *active)
{
	கட்டமைப்பு bloom_filter *filter;
	கட்டமைப்பு last_modified_entry *ent;
	கட்டமைப்பு hashmap_iter iter;

	எனில் (!lm->rev.bloom_filter_settings)
		திரும்பு true;

	எனில் (commit_graph_generation(origin) == GENERATION_எண்_INFINITY)
		திரும்பு true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	எனில் (!filter)
		திரும்பு true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		எனில் (active && !bitmap_get(active, ent->diff_idx))
			தொடர்;

		எனில் (bloom_filter_contains(filter, &ent->சாவி,
					  lm->rev.bloom_filter_settings))
			திரும்பு true;
	}
	திரும்பு false;
}

நிலையான வெறுமை process_parent(கட்டமைப்பு last_modified *lm,
			   கட்டமைப்பு prio_queue *queue,
			   கட்டமைப்பு commit *c, கட்டமைப்பு bitmap *active_c,
			   கட்டமைப்பு commit *parent, முழு parent_i)
{
	கட்டமைப்பு bitmap *active_p;

	எனில் (repo_parse_commit(lm->rev.repo, parent))
		திரும்பு;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	எனில் (parent_i || maybe_changed_path(lm, c, active_c)) {
		diff_மரம்_oid(&parent->object.oid,
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
	க்கு (முழு i = 0; i < diff_queued_diff.nr; i++) {
		கட்டமைப்பு diff_filepair *fp = diff_queued_diff.queue[i];
		மாறிலி எழுத்து *path = fp->two->path;
		கட்டமைப்பு last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    கட்டமைப்பு last_modified_entry, hashent);
		எனில் (ent) {
			size_t k = ent->diff_idx;
			எனில் (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	க்கு (size_t i = 0; i < lm->all_paths_nr; i++) {
		எனில் (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	எனில் (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	எனில் (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->சொற்கள், lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

நிலையான முழு last_modified_run(கட்டமைப்பு last_modified *lm)
{
	முழு max_மொத்தம், queue_popped = 0;
	கட்டமைப்பு commit *c, *n;
	கட்டமைப்பு prio_queue queue = { compare_commits_by_gen_then_commit_date };
	கட்டமைப்பு prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	கட்டமைப்பு commit_list *list;
	கட்டமைப்பு last_modified_callback_data data = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_data = &data;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_மொத்தம் = lm->rev.max_மொத்தம்;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	க்கு (list = lm->rev.commits; list; list = list->next) {
		கட்டமைப்பு commit *c = list->item;

		எனில் (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} இல்லையெனில் எனில் (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			கட்டமைப்பு bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			க்கு (size_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	போது ((c = prio_queue_get(&queue))) {
		முழு parent_i;
		கட்டமைப்பு commit_list *p;
		கட்டமைப்பு bitmap *active_c = active_paths_for(lm, c);

		எனில் ((0 <= max_மொத்தம் && max_மொத்தம் < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			data.commit = c;
			diff_மரம்_oid(lm->rev.repo->hash_algo->empty_மரம்,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			செல் cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		எனில் (repo_parse_commit(lm->rev.repo, c))
			செல் cleanup;

		போது ((n = prio_queue_get(&not_queue))) {
			கட்டமைப்பு commit_list *np;

			எனில் (repo_parse_commit(lm->rev.repo, n))
				தொடர்;

			க்கு (np = n->parents; np; np = np->next) {
				எனில் (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			எனில் (commit_graph_generation(n) < commit_graph_generation(c))
				நிறுத்து;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		க்கு (p = c->parents, parent_i = 0; p; p = p->next, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			எனில் (bitmap_is_empty(active_c))
				நிறுத்து;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		எனில் (!bitmap_is_empty(active_c))  {
			data.commit = c;
			க்கு (size_t i = 0; i < lm->all_paths_nr; i++) {
				எனில் (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], வெற்று, &data);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	எனில் (hashmap_get_size(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	திரும்பு 0;
}

நிலையான முழு last_modified_init(கட்டமைப்பு last_modified *lm, கட்டமைப்பு repository *r,
			      மாறிலி எழுத்து *prefix, முழு argc, மாறிலி எழுத்து **argv)
{
	கட்டமைப்பு hashmap_iter iter;
	கட்டமைப்பு last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, வெற்று, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_வேர்_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_மரம்_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.மரம்_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, வெற்று);
	எனில் (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		திரும்பு argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	எனில் (populate_paths_from_revs(lm) < 0)
		திரும்பு -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_size(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	திரும்பு 0;
}

முழு cmd_last_modified(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		      கட்டமைப்பு repository *repo)
{
	முழு ret;
	கட்டமைப்பு last_modified lm = { 0 };

	மாறிலி எழுத்து * மாறிலி last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		வெற்று
	};

	கட்டமைப்பு option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', வெற்று, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, வெற்று);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	எனில் (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	எனில் (ret)
		செல் out;

	ret = last_modified_run(&lm);
	எனில் (ret)
		செல் out;

out:
	last_modified_release(&lm);

	திரும்பு ret;
}
