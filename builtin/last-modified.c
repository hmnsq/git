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
	संरचना bloom_कुञ्चिका कुञ्चिका;
	size_t diff_idx;
	ध्रुव वर्ण path[FLEX_ARRAY];
};

स्थिर पूर्णाङ्क last_modified_entry_hashcmp(ध्रुव शून्य *unused UNUSED,
				       ध्रुव संरचना hashmap_entry *hent1,
				       ध्रुव संरचना hashmap_entry *hent2,
				       ध्रुव शून्य *path)
{
	ध्रुव संरचना last_modified_entry *ent1 =
		container_of(hent1, ध्रुव संरचना last_modified_entry, hashent);
	ध्रुव संरचना last_modified_entry *ent2 =
		container_of(hent2, ध्रुव संरचना last_modified_entry, hashent);
	प्रत्यागम strcmp(ent1->path, path ? path : ent2->path);
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
	पूर्णाङ्क max_depth;

	ध्रुव वर्ण **all_paths;
	size_t all_paths_nr;
	संरचना active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	संरचना bitmap *scratch;
};

स्थिर संरचना bitmap *active_paths_for(संरचना last_modified *lm, संरचना commit *c)
{
	संरचना bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	जइ (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	प्रत्यागम *bitmap;
}

स्थिर शून्य active_paths_free(संरचना last_modified *lm, संरचना commit *c)
{
	संरचना bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	जइ (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = रिक्त;
	}
}

स्थिर शून्य last_modified_release(संरचना last_modified *lm)
{
	संरचना hashmap_iter iter;
	संरचना last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_कुञ्चिका_clear(&ent->कुञ्चिका);

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

	प्रति (पूर्णाङ्क i = 0; i < q->nr; i++) {
		संरचना diff_filepair *p = q->queue[i];
		संरचना last_modified_entry *ent;
		ध्रुव वर्ण *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		जइ (lm->rev.bloom_filter_settings)
			bloom_कुञ्चिका_fill(&ent->कुञ्चिका, path, strlen(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

स्थिर पूर्णाङ्क populate_paths_from_revs(संरचना last_modified *lm)
{
	पूर्णाङ्क अङ्कः_interesting = 0, ret = 0;
	संरचना diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	memcpy(&diffopt, &lm->rev.diffopt, परिमाण(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_data = lm;

	प्रति (size_t i = 0; i < lm->rev.pending.nr; i++) {
		संरचना object_array_entry *obj = lm->rev.pending.objects + i;

		जइ (obj->item->flags & UNINTERESTING)
			अनुवर्त;

		जइ (अङ्कः_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			गच्छ out;
		}

		जइ (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			गच्छ out;
		}

		diff_वृक्षः_oid(lm->rev.repo->hash_algo->empty_वृक्षः,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	प्रत्यागम ret;
}

स्थिर शून्य last_modified_emit(संरचना last_modified *lm,
			       ध्रुव वर्ण *path, ध्रुव संरचना commit *commit)

{
	जइ (commit->object.flags & BOUNDARY)
		putchar('^');
	printf("%s\t", oid_to_hex(&commit->object.oid));

	जइ (lm->nul_termination)
		printf("%s%c", path, '\0');
	अण्णहा
		write_name_quoted(path, stdout, '\n');
}

स्थिर शून्य mark_path(ध्रुव वर्ण *path, ध्रुव संरचना object_id *oid,
		      संरचना last_modified_callback_data *data)
{
	संरचना last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&data->lm->paths, strhash(path), path,
					  संरचना last_modified_entry, hashent);
	जइ (!ent)
		प्रत्यागम;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	जइ (oid && !oideq(oid, &ent->oid))
		प्रत्यागम;

	last_modified_emit(data->lm, path, data->commit);

	hashmap_remove(&data->lm->paths, &ent->hashent, path);
	bloom_कुञ्चिका_clear(&ent->कुञ्चिका);
	मुक्त(ent);
}

स्थिर शून्य last_modified_diff(संरचना diff_queue_struct *q,
			       संरचना diff_options *opt UNUSED, शून्य *cbdata)
{
	संरचना last_modified_callback_data *data = cbdata;

	प्रति (पूर्णाङ्क i = 0; i < q->nr; i++) {
		संरचना diff_filepair *p = q->queue[i];
		विकल्प (p->status) {
		पक्ष DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			भङ्ग;

		स्वभाव:
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
			भङ्ग;
		}
	}
}

स्थिर शून्य pass_to_parent(संरचना bitmap *c,
			   संरचना bitmap *p,
			   size_t pos)
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

	जइ (!lm->rev.bloom_filter_settings)
		प्रत्यागम true;

	जइ (commit_graph_generation(origin) == GENERATION_सङ्ख्या_INFINITY)
		प्रत्यागम true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	जइ (!filter)
		प्रत्यागम true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		जइ (active && !bitmap_get(active, ent->diff_idx))
			अनुवर्त;

		जइ (bloom_filter_contains(filter, &ent->कुञ्चिका,
					  lm->rev.bloom_filter_settings))
			प्रत्यागम true;
	}
	प्रत्यागम false;
}

स्थिर शून्य process_parent(संरचना last_modified *lm,
			   संरचना prio_queue *queue,
			   संरचना commit *c, संरचना bitmap *active_c,
			   संरचना commit *parent, पूर्णाङ्क parent_i)
{
	संरचना bitmap *active_p;

	जइ (repo_parse_commit(lm->rev.repo, parent))
		प्रत्यागम;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	जइ (parent_i || maybe_changed_path(lm, c, active_c)) {
		diff_वृक्षः_oid(&parent->object.oid,
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
	प्रति (पूर्णाङ्क i = 0; i < diff_queued_diff.nr; i++) {
		संरचना diff_filepair *fp = diff_queued_diff.queue[i];
		ध्रुव वर्ण *path = fp->two->path;
		संरचना last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    संरचना last_modified_entry, hashent);
		जइ (ent) {
			size_t k = ent->diff_idx;
			जइ (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	प्रति (size_t i = 0; i < lm->all_paths_nr; i++) {
		जइ (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	जइ (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	जइ (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->शब्दाः, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

स्थिर पूर्णाङ्क last_modified_run(संरचना last_modified *lm)
{
	पूर्णाङ्क max_गणनम्, queue_popped = 0;
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

	max_गणनम् = lm->rev.max_गणनम्;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	प्रति (list = lm->rev.commits; list; list = list->next) {
		संरचना commit *c = list->item;

		जइ (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} अण्णहा जइ (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			संरचना bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			प्रति (size_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	जाव ((c = prio_queue_get(&queue))) {
		पूर्णाङ्क parent_i;
		संरचना commit_list *p;
		संरचना bitmap *active_c = active_paths_for(lm, c);

		जइ ((0 <= max_गणनम् && max_गणनम् < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			data.commit = c;
			diff_वृक्षः_oid(lm->rev.repo->hash_algo->empty_वृक्षः,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			गच्छ cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		जइ (repo_parse_commit(lm->rev.repo, c))
			गच्छ cleanup;

		जाव ((n = prio_queue_get(&not_queue))) {
			संरचना commit_list *np;

			जइ (repo_parse_commit(lm->rev.repo, n))
				अनुवर्त;

			प्रति (np = n->parents; np; np = np->next) {
				जइ (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			जइ (commit_graph_generation(n) < commit_graph_generation(c))
				भङ्ग;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		प्रति (p = c->parents, parent_i = 0; p; p = p->next, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			जइ (bitmap_is_empty(active_c))
				भङ्ग;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		जइ (!bitmap_is_empty(active_c))  {
			data.commit = c;
			प्रति (size_t i = 0; i < lm->all_paths_nr; i++) {
				जइ (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], रिक्त, &data);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	जइ (hashmap_get_size(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क last_modified_init(संरचना last_modified *lm, संरचना repository *r,
			      ध्रुव वर्ण *prefix, पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	संरचना hashmap_iter iter;
	संरचना last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, रिक्त, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_मूलम्_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_वृक्षः_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.वृक्षः_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, रिक्त);
	जइ (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		प्रत्यागम argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	जइ (populate_paths_from_revs(lm) < 0)
		प्रत्यागम -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_size(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_last_modified(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		      संरचना repository *repo)
{
	पूर्णाङ्क ret;
	संरचना last_modified lm = { 0 };

	ध्रुव वर्ण * ध्रुव last_modified_usage[] = {
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
	जइ (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	जइ (ret)
		गच्छ out;

	ret = last_modified_run(&lm);
	जइ (ret)
		गच्छ out;

out:
	last_modified_release(&lm);

	प्रत्यागम ret;
}
