#شامل "git-compat-util.h"
#شامل "bloom.h"
#شامل "builtin.h"
#شامل "commit-graph.h"
#شامل "commit-slab.h"
#شامل "commit.h"
#شامل "config.h"
#شامل "diff.h"
#شامل "diffcore.h"
#شامل "environment.h"
#شامل "ewah/ewok.h"
#شامل "hashmap.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "object.h"
#شامل "parse-options.h"
#شامل "prio-queue.h"
#شامل "quote.h"
#شامل "repository.h"
#شامل "revision.h"

/* Remember to update object flag allocation in object.h */
#define PARENT1 (1u<<16) /* used instead of SEEN */
#define PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

ساختار last_modified_entry {
	ساختار hashmap_entry hashent;
	ساختار object_id oid;
	ساختار bloom_کلید کلید;
	size_t diff_idx;
	ثابت نویسه path[FLEX_ARRAY];
};

ایستا صحیح last_modified_entry_hashcmp(ثابت تهی *unused UNUSED,
				       ثابت ساختار hashmap_entry *hent1,
				       ثابت ساختار hashmap_entry *hent2,
				       ثابت تهی *path)
{
	ثابت ساختار last_modified_entry *ent1 =
		container_of(hent1, ثابت ساختار last_modified_entry, hashent);
	ثابت ساختار last_modified_entry *ent2 =
		container_of(hent2, ثابت ساختار last_modified_entry, hashent);
	بازگشت strcmp(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, ساختار bitmap *);

ساختار last_modified {
	ساختار hashmap paths;
	ساختار rev_info rev;
	bool show_trees;
	bool nul_termination;
	صحیح max_depth;

	ثابت نویسه **all_paths;
	size_t all_paths_nr;
	ساختار active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	ساختار bitmap *scratch;
};

ایستا ساختار bitmap *active_paths_for(ساختار last_modified *lm, ساختار commit *c)
{
	ساختار bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	اگر (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	بازگشت *bitmap;
}

ایستا تهی active_paths_free(ساختار last_modified *lm, ساختار commit *c)
{
	ساختار bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	اگر (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = پوچ;
	}
}

ایستا تهی last_modified_release(ساختار last_modified *lm)
{
	ساختار hashmap_iter iter;
	ساختار last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_کلید_clear(&ent->کلید);

	hashmap_clear_and_free(&lm->paths, ساختار last_modified_entry, hashent);
	release_revisions(&lm->rev);

	آزاد(lm->all_paths);
}

ساختار last_modified_callback_data {
	ساختار last_modified *lm;
	ساختار commit *commit;
};

ایستا تهی add_path_from_diff(ساختار diff_queue_struct *q,
			       ساختار diff_options *opt UNUSED, تهی *data)
{
	ساختار last_modified *lm = data;

	برای (صحیح i = 0; i < q->nr; i++) {
		ساختار diff_filepair *p = q->queue[i];
		ساختار last_modified_entry *ent;
		ثابت نویسه *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		اگر (lm->rev.bloom_filter_settings)
			bloom_کلید_fill(&ent->کلید, path, strlen(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

ایستا صحیح populate_paths_from_revs(ساختار last_modified *lm)
{
	صحیح رقم_interesting = 0, ret = 0;
	ساختار diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	memcpy(&diffopt, &lm->rev.diffopt, اندازه(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_data = lm;

	برای (size_t i = 0; i < lm->rev.pending.nr; i++) {
		ساختار object_array_entry *obj = lm->rev.pending.objects + i;

		اگر (obj->item->flags & UNINTERESTING)
			ادامه;

		اگر (رقم_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			برو out;
		}

		اگر (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			برو out;
		}

		diff_درخت_oid(lm->rev.repo->hash_algo->empty_درخت,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	بازگشت ret;
}

ایستا تهی last_modified_emit(ساختار last_modified *lm,
			       ثابت نویسه *path, ثابت ساختار commit *commit)

{
	اگر (commit->object.flags & BOUNDARY)
		putchar('^');
	printf("%s\t", oid_to_hex(&commit->object.oid));

	اگر (lm->nul_termination)
		printf("%s%c", path, '\0');
	وگرنه
		write_name_quoted(path, stdout, '\n');
}

ایستا تهی mark_path(ثابت نویسه *path, ثابت ساختار object_id *oid,
		      ساختار last_modified_callback_data *data)
{
	ساختار last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&data->lm->paths, strhash(path), path,
					  ساختار last_modified_entry, hashent);
	اگر (!ent)
		بازگشت;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	اگر (oid && !oideq(oid, &ent->oid))
		بازگشت;

	last_modified_emit(data->lm, path, data->commit);

	hashmap_remove(&data->lm->paths, &ent->hashent, path);
	bloom_کلید_clear(&ent->کلید);
	آزاد(ent);
}

ایستا تهی last_modified_diff(ساختار diff_queue_struct *q,
			       ساختار diff_options *opt UNUSED, تهی *cbdata)
{
	ساختار last_modified_callback_data *data = cbdata;

	برای (صحیح i = 0; i < q->nr; i++) {
		ساختار diff_filepair *p = q->queue[i];
		انتخاب (p->status) {
		حالت DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			بشکن;

		پیشفرض:
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
			بشکن;
		}
	}
}

ایستا تهی pass_to_parent(ساختار bitmap *c,
			   ساختار bitmap *p,
			   size_t pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

ایستا bool maybe_changed_path(ساختار last_modified *lm,
			       ساختار commit *origin,
			       ساختار bitmap *active)
{
	ساختار bloom_filter *filter;
	ساختار last_modified_entry *ent;
	ساختار hashmap_iter iter;

	اگر (!lm->rev.bloom_filter_settings)
		بازگشت true;

	اگر (commit_graph_generation(origin) == GENERATION_عدد_INFINITY)
		بازگشت true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	اگر (!filter)
		بازگشت true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		اگر (active && !bitmap_get(active, ent->diff_idx))
			ادامه;

		اگر (bloom_filter_contains(filter, &ent->کلید,
					  lm->rev.bloom_filter_settings))
			بازگشت true;
	}
	بازگشت false;
}

ایستا تهی process_parent(ساختار last_modified *lm,
			   ساختار prio_queue *queue,
			   ساختار commit *c, ساختار bitmap *active_c,
			   ساختار commit *parent, صحیح parent_i)
{
	ساختار bitmap *active_p;

	اگر (repo_parse_commit(lm->rev.repo, parent))
		بازگشت;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	اگر (parent_i || maybe_changed_path(lm, c, active_c)) {
		diff_درخت_oid(&parent->object.oid,
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
	برای (صحیح i = 0; i < diff_queued_diff.nr; i++) {
		ساختار diff_filepair *fp = diff_queued_diff.queue[i];
		ثابت نویسه *path = fp->two->path;
		ساختار last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    ساختار last_modified_entry, hashent);
		اگر (ent) {
			size_t k = ent->diff_idx;
			اگر (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	برای (size_t i = 0; i < lm->all_paths_nr; i++) {
		اگر (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	اگر (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	اگر (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->واژگان, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

ایستا صحیح last_modified_run(ساختار last_modified *lm)
{
	صحیح max_شمار, queue_popped = 0;
	ساختار commit *c, *n;
	ساختار prio_queue queue = { compare_commits_by_gen_then_commit_date };
	ساختار prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	ساختار commit_list *list;
	ساختار last_modified_callback_data data = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_data = &data;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_شمار = lm->rev.max_شمار;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	برای (list = lm->rev.commits; list; list = list->next) {
		ساختار commit *c = list->item;

		اگر (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} وگرنه اگر (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			ساختار bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			برای (size_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	وقتی ((c = prio_queue_get(&queue))) {
		صحیح parent_i;
		ساختار commit_list *p;
		ساختار bitmap *active_c = active_paths_for(lm, c);

		اگر ((0 <= max_شمار && max_شمار < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			data.commit = c;
			diff_درخت_oid(lm->rev.repo->hash_algo->empty_درخت,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			برو cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		اگر (repo_parse_commit(lm->rev.repo, c))
			برو cleanup;

		وقتی ((n = prio_queue_get(&not_queue))) {
			ساختار commit_list *np;

			اگر (repo_parse_commit(lm->rev.repo, n))
				ادامه;

			برای (np = n->parents; np; np = np->next) {
				اگر (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			اگر (commit_graph_generation(n) < commit_graph_generation(c))
				بشکن;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		برای (p = c->parents, parent_i = 0; p; p = p->next, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			اگر (bitmap_is_empty(active_c))
				بشکن;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		اگر (!bitmap_is_empty(active_c))  {
			data.commit = c;
			برای (size_t i = 0; i < lm->all_paths_nr; i++) {
				اگر (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], پوچ, &data);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	اگر (hashmap_get_size(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	بازگشت 0;
}

ایستا صحیح last_modified_init(ساختار last_modified *lm, ساختار repository *r,
			      ثابت نویسه *prefix, صحیح argc, ثابت نویسه **argv)
{
	ساختار hashmap_iter iter;
	ساختار last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, پوچ, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_ریشه_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_درخت_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.درخت_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, پوچ);
	اگر (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		بازگشت argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	اگر (populate_paths_from_revs(lm) < 0)
		بازگشت -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_size(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	بازگشت 0;
}

صحیح cmd_last_modified(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		      ساختار repository *repo)
{
	صحیح ret;
	ساختار last_modified lm = { 0 };

	ثابت نویسه * ثابت last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		پوچ
	};

	ساختار option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', پوچ, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, پوچ);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	اگر (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	اگر (ret)
		برو out;

	ret = last_modified_run(&lm);
	اگر (ret)
		برو out;

out:
	last_modified_release(&lm);

	بازگشت ret;
}
