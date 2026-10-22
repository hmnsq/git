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

সংরচনা last_modified_entry {
	সংরচনা hashmap_entry hashent;
	সংরচনা object_id oid;
	সংরচনা bloom_চাবি চাবি;
	size_t diff_idx;
	ধ্রুব বর্ণ path[FLEX_ARRAY];
};

স্থির পূর্ণাঙ্ক last_modified_entry_hashcmp(ধ্রুব শূন্য *unused UNUSED,
				       ধ্রুব সংরচনা hashmap_entry *hent1,
				       ধ্রুব সংরচনা hashmap_entry *hent2,
				       ধ্রুব শূন্য *path)
{
	ধ্রুব সংরচনা last_modified_entry *ent1 =
		container_of(hent1, ধ্রুব সংরচনা last_modified_entry, hashent);
	ধ্রুব সংরচনা last_modified_entry *ent2 =
		container_of(hent2, ধ্রুব সংরচনা last_modified_entry, hashent);
	প্রত্যাগম strcmp(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, সংরচনা bitmap *);

সংরচনা last_modified {
	সংরচনা hashmap paths;
	সংরচনা rev_info rev;
	bool show_trees;
	bool nul_termination;
	পূর্ণাঙ্ক max_depth;

	ধ্রুব বর্ণ **all_paths;
	size_t all_paths_nr;
	সংরচনা active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	সংরচনা bitmap *scratch;
};

স্থির সংরচনা bitmap *active_paths_for(সংরচনা last_modified *lm, সংরচনা commit *c)
{
	সংরচনা bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	যদি (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	প্রত্যাগম *bitmap;
}

স্থির শূন্য active_paths_free(সংরচনা last_modified *lm, সংরচনা commit *c)
{
	সংরচনা bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	যদি (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = রিক্ত;
	}
}

স্থির শূন্য last_modified_release(সংরচনা last_modified *lm)
{
	সংরচনা hashmap_iter iter;
	সংরচনা last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_চাবি_clear(&ent->চাবি);

	hashmap_clear_and_free(&lm->paths, সংরচনা last_modified_entry, hashent);
	release_revisions(&lm->rev);

	মুক্ত(lm->all_paths);
}

সংরচনা last_modified_callback_data {
	সংরচনা last_modified *lm;
	সংরচনা commit *commit;
};

স্থির শূন্য add_path_from_diff(সংরচনা diff_queue_struct *q,
			       সংরচনা diff_options *opt UNUSED, শূন্য *data)
{
	সংরচনা last_modified *lm = data;

	প্রতি (পূর্ণাঙ্ক i = 0; i < q->nr; i++) {
		সংরচনা diff_filepair *p = q->queue[i];
		সংরচনা last_modified_entry *ent;
		ধ্রুব বর্ণ *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		যদি (lm->rev.bloom_filter_settings)
			bloom_চাবি_fill(&ent->চাবি, path, strlen(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

স্থির পূর্ণাঙ্ক populate_paths_from_revs(সংরচনা last_modified *lm)
{
	পূর্ণাঙ্ক অঙ্ক_interesting = 0, ret = 0;
	সংরচনা diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	memcpy(&diffopt, &lm->rev.diffopt, পরিমাণ(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_data = lm;

	প্রতি (size_t i = 0; i < lm->rev.pending.nr; i++) {
		সংরচনা object_array_entry *obj = lm->rev.pending.objects + i;

		যদি (obj->item->flags & UNINTERESTING)
			অনুবর্ত;

		যদি (অঙ্ক_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			গচ্ছ out;
		}

		যদি (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			গচ্ছ out;
		}

		diff_বৃক্ষ_oid(lm->rev.repo->hash_algo->empty_বৃক্ষ,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	প্রত্যাগম ret;
}

স্থির শূন্য last_modified_emit(সংরচনা last_modified *lm,
			       ধ্রুব বর্ণ *path, ধ্রুব সংরচনা commit *commit)

{
	যদি (commit->object.flags & BOUNDARY)
		putchar('^');
	printf("%s\t", oid_to_hex(&commit->object.oid));

	যদি (lm->nul_termination)
		printf("%s%c", path, '\0');
	অন্যথা
		write_name_quoted(path, stdout, '\n');
}

স্থির শূন্য mark_path(ধ্রুব বর্ণ *path, ধ্রুব সংরচনা object_id *oid,
		      সংরচনা last_modified_callback_data *data)
{
	সংরচনা last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&data->lm->paths, strhash(path), path,
					  সংরচনা last_modified_entry, hashent);
	যদি (!ent)
		প্রত্যাগম;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	যদি (oid && !oideq(oid, &ent->oid))
		প্রত্যাগম;

	last_modified_emit(data->lm, path, data->commit);

	hashmap_remove(&data->lm->paths, &ent->hashent, path);
	bloom_চাবি_clear(&ent->চাবি);
	মুক্ত(ent);
}

স্থির শূন্য last_modified_diff(সংরচনা diff_queue_struct *q,
			       সংরচনা diff_options *opt UNUSED, শূন্য *cbdata)
{
	সংরচনা last_modified_callback_data *data = cbdata;

	প্রতি (পূর্ণাঙ্ক i = 0; i < q->nr; i++) {
		সংরচনা diff_filepair *p = q->queue[i];
		বিকল্প (p->status) {
		পক্ষ DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			ভঙ্গ;

		স্বভাব:
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
			ভঙ্গ;
		}
	}
}

স্থির শূন্য pass_to_parent(সংরচনা bitmap *c,
			   সংরচনা bitmap *p,
			   size_t pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

স্থির bool maybe_changed_path(সংরচনা last_modified *lm,
			       সংরচনা commit *origin,
			       সংরচনা bitmap *active)
{
	সংরচনা bloom_filter *filter;
	সংরচনা last_modified_entry *ent;
	সংরচনা hashmap_iter iter;

	যদি (!lm->rev.bloom_filter_settings)
		প্রত্যাগম true;

	যদি (commit_graph_generation(origin) == GENERATION_সংখ্যা_INFINITY)
		প্রত্যাগম true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	যদি (!filter)
		প্রত্যাগম true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		যদি (active && !bitmap_get(active, ent->diff_idx))
			অনুবর্ত;

		যদি (bloom_filter_contains(filter, &ent->চাবি,
					  lm->rev.bloom_filter_settings))
			প্রত্যাগম true;
	}
	প্রত্যাগম false;
}

স্থির শূন্য process_parent(সংরচনা last_modified *lm,
			   সংরচনা prio_queue *queue,
			   সংরচনা commit *c, সংরচনা bitmap *active_c,
			   সংরচনা commit *parent, পূর্ণাঙ্ক parent_i)
{
	সংরচনা bitmap *active_p;

	যদি (repo_parse_commit(lm->rev.repo, parent))
		প্রত্যাগম;
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
	প্রতি (পূর্ণাঙ্ক i = 0; i < diff_queued_diff.nr; i++) {
		সংরচনা diff_filepair *fp = diff_queued_diff.queue[i];
		ধ্রুব বর্ণ *path = fp->two->path;
		সংরচনা last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    সংরচনা last_modified_entry, hashent);
		যদি (ent) {
			size_t k = ent->diff_idx;
			যদি (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	প্রতি (size_t i = 0; i < lm->all_paths_nr; i++) {
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

স্থির পূর্ণাঙ্ক last_modified_run(সংরচনা last_modified *lm)
{
	পূর্ণাঙ্ক max_গুনতি, queue_popped = 0;
	সংরচনা commit *c, *n;
	সংরচনা prio_queue queue = { compare_commits_by_gen_then_commit_date };
	সংরচনা prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	সংরচনা commit_list *list;
	সংরচনা last_modified_callback_data data = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_data = &data;
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
	প্রতি (list = lm->rev.commits; list; list = list->next) {
		সংরচনা commit *c = list->item;

		যদি (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} অন্যথা যদি (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			সংরচনা bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			প্রতি (size_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	যাবত্ ((c = prio_queue_get(&queue))) {
		পূর্ণাঙ্ক parent_i;
		সংরচনা commit_list *p;
		সংরচনা bitmap *active_c = active_paths_for(lm, c);

		যদি ((0 <= max_গুনতি && max_গুনতি < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			data.commit = c;
			diff_বৃক্ষ_oid(lm->rev.repo->hash_algo->empty_বৃক্ষ,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			গচ্ছ cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		যদি (repo_parse_commit(lm->rev.repo, c))
			গচ্ছ cleanup;

		যাবত্ ((n = prio_queue_get(&not_queue))) {
			সংরচনা commit_list *np;

			যদি (repo_parse_commit(lm->rev.repo, n))
				অনুবর্ত;

			প্রতি (np = n->parents; np; np = np->next) {
				যদি (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			যদি (commit_graph_generation(n) < commit_graph_generation(c))
				ভঙ্গ;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		প্রতি (p = c->parents, parent_i = 0; p; p = p->next, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			যদি (bitmap_is_empty(active_c))
				ভঙ্গ;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		যদি (!bitmap_is_empty(active_c))  {
			data.commit = c;
			প্রতি (size_t i = 0; i < lm->all_paths_nr; i++) {
				যদি (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], রিক্ত, &data);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	যদি (hashmap_get_size(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক last_modified_init(সংরচনা last_modified *lm, সংরচনা repository *r,
			      ধ্রুব বর্ণ *prefix, পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv)
{
	সংরচনা hashmap_iter iter;
	সংরচনা last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, রিক্ত, 0);

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

	argc = setup_revisions(argc, argv, &lm->rev, রিক্ত);
	যদি (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		প্রত্যাগম argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	যদি (populate_paths_from_revs(lm) < 0)
		প্রত্যাগম -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_size(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	প্রত্যাগম 0;
}

পূর্ণাঙ্ক cmd_last_modified(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		      সংরচনা repository *repo)
{
	পূর্ণাঙ্ক ret;
	সংরচনা last_modified lm = { 0 };

	ধ্রুব বর্ণ * ধ্রুব last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		রিক্ত
	};

	সংরচনা option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', রিক্ত, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, রিক্ত);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	যদি (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	যদি (ret)
		গচ্ছ out;

	ret = last_modified_run(&lm);
	যদি (ret)
		গচ্ছ out;

out:
	last_modified_release(&lm);

	প্রত্যাগম ret;
}
