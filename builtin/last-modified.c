#अन्तर्भाव "git-compat-util.h"
#अन्तर्भाव "bloom.h"
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "commit-graph.h"
#अन्तर्भाव "commit-slab.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "diffcore.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "ewah/ewok.h"
#अन्तर्भाव "hashmap.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "prio-queue.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "repository.h"
#अन्तर्भाव "revision.h"

/* Remember to update object flag allocation in object.h */
#define PARENT1 (1u<<16) /* used instead of SEEN */
#define PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

संरचना last_modified_entry {
	संरचना hashmap_entry hashent;
	संरचना object_id oid;
	संरचना bloom_किल्ली किल्ली;
	माप_प्रकार diff_idx;
	स्थिरांक अक्षर path[FLEX_ARRAY];
};

स्थिर पूर्णांक last_modified_entry_hashcmp(स्थिरांक शून्य *unused UNUSED,
				       स्थिरांक संरचना hashmap_entry *hent1,
				       स्थिरांक संरचना hashmap_entry *hent2,
				       स्थिरांक शून्य *path)
{
	स्थिरांक संरचना last_modified_entry *ent1 =
		container_of(hent1, स्थिरांक संरचना last_modified_entry, hashent);
	स्थिरांक संरचना last_modified_entry *ent2 =
		container_of(hent2, स्थिरांक संरचना last_modified_entry, hashent);
	परत म_भेद(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, संरचना bitmap *);

संरचना last_modified {
	संरचना hashmap paths;
	संरचना rev_info rev;
	bool show_trees;
	bool nul_termination;
	पूर्णांक max_depth;

	स्थिरांक अक्षर **all_paths;
	माप_प्रकार all_paths_nr;
	संरचना active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	संरचना bitmap *scratch;
};

स्थिर संरचना bitmap *active_paths_for(संरचना last_modified *lm, संरचना commit *c)
{
	संरचना bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	जर (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	परत *bitmap;
}

स्थिर शून्य active_paths_free(संरचना last_modified *lm, संरचना commit *c)
{
	संरचना bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	जर (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = रिक्त;
	}
}

स्थिर शून्य last_modified_release(संरचना last_modified *lm)
{
	संरचना hashmap_iter iter;
	संरचना last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_किल्ली_clear(&ent->किल्ली);

	hashmap_clear_and_free(&lm->paths, संरचना last_modified_entry, hashent);
	release_revisions(&lm->rev);

	मुक्त(lm->all_paths);
}

संरचना last_modified_callback_data {
	संरचना last_modified *lm;
	संरचना commit *commit;
};

स्थिर शून्य add_path_from_diff(संरचना diff_queue_struct *q,
			       संरचना diff_options *opt UNUSED, शून्य *data)
{
	संरचना last_modified *lm = data;

	साठी (पूर्णांक i = 0; i < q->nr; i++) {
		संरचना diff_filepair *p = q->queue[i];
		संरचना last_modified_entry *ent;
		स्थिरांक अक्षर *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		जर (lm->rev.bloom_filter_settings)
			bloom_किल्ली_fill(&ent->किल्ली, path, म_माप(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

स्थिर पूर्णांक populate_paths_from_revs(संरचना last_modified *lm)
{
	पूर्णांक अंक_interesting = 0, ret = 0;
	संरचना diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	स_नकल(&diffopt, &lm->rev.diffopt, आकार(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_data = lm;

	साठी (माप_प्रकार i = 0; i < lm->rev.pending.nr; i++) {
		संरचना object_array_entry *obj = lm->rev.pending.objects + i;

		जर (obj->item->flags & UNINTERESTING)
			सुरूठेवा;

		जर (अंक_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			जा out;
		}

		जर (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			जा out;
		}

		diff_वृक्ष_oid(lm->rev.repo->hash_algo->empty_वृक्ष,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	परत ret;
}

स्थिर शून्य last_modified_emit(संरचना last_modified *lm,
			       स्थिरांक अक्षर *path, स्थिरांक संरचना commit *commit)

{
	जर (commit->object.flags & BOUNDARY)
		अक्षर_दो('^');
	म_लिखो("%s\t", oid_to_hex(&commit->object.oid));

	जर (lm->nul_termination)
		म_लिखो("%s%c", path, '\0');
	अन्यथा
		write_name_quoted(path, मानक_निकास, '\n');
}

स्थिर शून्य mark_path(स्थिरांक अक्षर *path, स्थिरांक संरचना object_id *oid,
		      संरचना last_modified_callback_data *data)
{
	संरचना last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&data->lm->paths, strhash(path), path,
					  संरचना last_modified_entry, hashent);
	जर (!ent)
		परत;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	जर (oid && !oideq(oid, &ent->oid))
		परत;

	last_modified_emit(data->lm, path, data->commit);

	hashmap_remove(&data->lm->paths, &ent->hashent, path);
	bloom_किल्ली_clear(&ent->किल्ली);
	मुक्त(ent);
}

स्थिर शून्य last_modified_diff(संरचना diff_queue_struct *q,
			       संरचना diff_options *opt UNUSED, शून्य *cbdata)
{
	संरचना last_modified_callback_data *data = cbdata;

	साठी (पूर्णांक i = 0; i < q->nr; i++) {
		संरचना diff_filepair *p = q->queue[i];
		निवड (p->status) {
		स्थिती DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			थांबा;

		पूर्वनिर्धारित:
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
			थांबा;
		}
	}
}

स्थिर शून्य pass_to_parent(संरचना bitmap *c,
			   संरचना bitmap *p,
			   माप_प्रकार pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

स्थिर bool maybe_changed_path(संरचना last_modified *lm,
			       संरचना commit *origin,
			       संरचना bitmap *active)
{
	संरचना bloom_filter *filter;
	संरचना last_modified_entry *ent;
	संरचना hashmap_iter iter;

	जर (!lm->rev.bloom_filter_settings)
		परत true;

	जर (commit_graph_generation(origin) == GENERATION_संख्या_INFINITY)
		परत true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	जर (!filter)
		परत true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		जर (active && !bitmap_get(active, ent->diff_idx))
			सुरूठेवा;

		जर (bloom_filter_contains(filter, &ent->किल्ली,
					  lm->rev.bloom_filter_settings))
			परत true;
	}
	परत false;
}

स्थिर शून्य process_parent(संरचना last_modified *lm,
			   संरचना prio_queue *queue,
			   संरचना commit *c, संरचना bitmap *active_c,
			   संरचना commit *parent, पूर्णांक parent_i)
{
	संरचना bitmap *active_p;

	जर (repo_parse_commit(lm->rev.repo, parent))
		परत;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	जर (parent_i || maybe_changed_path(lm, c, active_c)) {
		diff_वृक्ष_oid(&parent->object.oid,
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
	साठी (पूर्णांक i = 0; i < diff_queued_diff.nr; i++) {
		संरचना diff_filepair *fp = diff_queued_diff.queue[i];
		स्थिरांक अक्षर *path = fp->two->path;
		संरचना last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    संरचना last_modified_entry, hashent);
		जर (ent) {
			माप_प्रकार k = ent->diff_idx;
			जर (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	साठी (माप_प्रकार i = 0; i < lm->all_paths_nr; i++) {
		जर (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	जर (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	जर (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->शब्द, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

स्थिर पूर्णांक last_modified_run(संरचना last_modified *lm)
{
	पूर्णांक max_मोजणी, queue_popped = 0;
	संरचना commit *c, *n;
	संरचना prio_queue queue = { compare_commits_by_gen_then_commit_date };
	संरचना prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	संरचना commit_list *list;
	संरचना last_modified_callback_data data = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_data = &data;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_मोजणी = lm->rev.max_मोजणी;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	साठी (list = lm->rev.commits; list; list = list->next) {
		संरचना commit *c = list->item;

		जर (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} अन्यथा जर (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			संरचना bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			साठी (माप_प्रकार i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	पर्यंत ((c = prio_queue_get(&queue))) {
		पूर्णांक parent_i;
		संरचना commit_list *p;
		संरचना bitmap *active_c = active_paths_for(lm, c);

		जर ((0 <= max_मोजणी && max_मोजणी < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			data.commit = c;
			diff_वृक्ष_oid(lm->rev.repo->hash_algo->empty_वृक्ष,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			जा cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		जर (repo_parse_commit(lm->rev.repo, c))
			जा cleanup;

		पर्यंत ((n = prio_queue_get(&not_queue))) {
			संरचना commit_list *np;

			जर (repo_parse_commit(lm->rev.repo, n))
				सुरूठेवा;

			साठी (np = n->parents; np; np = np->next) {
				जर (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			जर (commit_graph_generation(n) < commit_graph_generation(c))
				थांबा;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		साठी (p = c->parents, parent_i = 0; p; p = p->next, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			जर (bitmap_is_empty(active_c))
				थांबा;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		जर (!bitmap_is_empty(active_c))  {
			data.commit = c;
			साठी (माप_प्रकार i = 0; i < lm->all_paths_nr; i++) {
				जर (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], रिक्त, &data);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	जर (hashmap_get_size(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	परत 0;
}

स्थिर पूर्णांक last_modified_init(संरचना last_modified *lm, संरचना repository *r,
			      स्थिरांक अक्षर *prefix, पूर्णांक argc, स्थिरांक अक्षर **argv)
{
	संरचना hashmap_iter iter;
	संरचना last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, रिक्त, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_मूळ_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_वृक्ष_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.वृक्ष_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, रिक्त);
	जर (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		परत argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	जर (populate_paths_from_revs(lm) < 0)
		परत -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_size(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	परत 0;
}

पूर्णांक cmd_last_modified(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		      संरचना repository *repo)
{
	पूर्णांक ret;
	संरचना last_modified lm = { 0 };

	स्थिरांक अक्षर * स्थिरांक last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		रिक्त
	};

	संरचना option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', रिक्त, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, रिक्त);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	जर (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	जर (ret)
		जा out;

	ret = last_modified_run(&lm);
	जर (ret)
		जा out;

out:
	last_modified_release(&lm);

	परत ret;
}
