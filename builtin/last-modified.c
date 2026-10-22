#അന്തര്ഭാവ "git-compat-util.h"
#അന്തര്ഭാവ "bloom.h"
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "commit-graph.h"
#അന്തര്ഭാവ "commit-slab.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "diffcore.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "ewah/ewok.h"
#അന്തര്ഭാവ "hashmap.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "prio-queue.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "repository.h"
#അന്തര്ഭാവ "revision.h"

/* Remember to update object flag allocation in object.h */
#define PARENT1 (1u<<16) /* used instead of SEEN */
#define PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

ഘടന last_modified_entry {
	ഘടന hashmap_entry hashent;
	ഘടന object_id oid;
	ഘടന bloom_താക്കോൽ താക്കോൽ;
	size_t diff_idx;
	സ്ഥിരം അക്ഷരം path[FLEX_ARRAY];
};

സ്ഥിതം പൂർണ്ണസംഖ്യ last_modified_entry_hashcmp(സ്ഥിരം ശൂന്യം *unused UNUSED,
				       സ്ഥിരം ഘടന hashmap_entry *hent1,
				       സ്ഥിരം ഘടന hashmap_entry *hent2,
				       സ്ഥിരം ശൂന്യം *path)
{
	സ്ഥിരം ഘടന last_modified_entry *ent1 =
		container_of(hent1, സ്ഥിരം ഘടന last_modified_entry, hashent);
	സ്ഥിരം ഘടന last_modified_entry *ent2 =
		container_of(hent2, സ്ഥിരം ഘടന last_modified_entry, hashent);
	തിരികെ strcmp(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, ഘടന bitmap *);

ഘടന last_modified {
	ഘടന hashmap paths;
	ഘടന rev_info rev;
	bool show_trees;
	bool nul_termination;
	പൂർണ്ണസംഖ്യ max_depth;

	സ്ഥിരം അക്ഷരം **all_paths;
	size_t all_paths_nr;
	ഘടന active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	ഘടന bitmap *scratch;
};

സ്ഥിതം ഘടന bitmap *active_paths_for(ഘടന last_modified *lm, ഘടന commit *c)
{
	ഘടന bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	എങ്കിൽ (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	തിരികെ *bitmap;
}

സ്ഥിതം ശൂന്യം active_paths_free(ഘടന last_modified *lm, ഘടന commit *c)
{
	ഘടന bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	എങ്കിൽ (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = ശൂന്യ;
	}
}

സ്ഥിതം ശൂന്യം last_modified_release(ഘടന last_modified *lm)
{
	ഘടന hashmap_iter iter;
	ഘടന last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_താക്കോൽ_clear(&ent->താക്കോൽ);

	hashmap_clear_and_free(&lm->paths, ഘടന last_modified_entry, hashent);
	release_revisions(&lm->rev);

	സ്വതന്ത്രം(lm->all_paths);
}

ഘടന last_modified_callback_data {
	ഘടന last_modified *lm;
	ഘടന commit *commit;
};

സ്ഥിതം ശൂന്യം add_path_from_diff(ഘടന diff_queue_struct *q,
			       ഘടന diff_options *opt UNUSED, ശൂന്യം *data)
{
	ഘടന last_modified *lm = data;

	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < q->nr; i++) {
		ഘടന diff_filepair *p = q->queue[i];
		ഘടന last_modified_entry *ent;
		സ്ഥിരം അക്ഷരം *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		എങ്കിൽ (lm->rev.bloom_filter_settings)
			bloom_താക്കോൽ_fill(&ent->താക്കോൽ, path, strlen(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ populate_paths_from_revs(ഘടന last_modified *lm)
{
	പൂർണ്ണസംഖ്യ അക്കം_interesting = 0, ret = 0;
	ഘടന diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	memcpy(&diffopt, &lm->rev.diffopt, വലുപ്പം(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_data = lm;

	വേണ്ടി (size_t i = 0; i < lm->rev.pending.nr; i++) {
		ഘടന object_array_entry *obj = lm->rev.pending.objects + i;

		എങ്കിൽ (obj->item->flags & UNINTERESTING)
			തുടരുക;

		എങ്കിൽ (അക്കം_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			പോകുക out;
		}

		എങ്കിൽ (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			പോകുക out;
		}

		diff_വൃക്ഷം_oid(lm->rev.repo->hash_algo->empty_വൃക്ഷം,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം last_modified_emit(ഘടന last_modified *lm,
			       സ്ഥിരം അക്ഷരം *path, സ്ഥിരം ഘടന commit *commit)

{
	എങ്കിൽ (commit->object.flags & BOUNDARY)
		putchar('^');
	printf("%s\t", oid_to_hex(&commit->object.oid));

	എങ്കിൽ (lm->nul_termination)
		printf("%s%c", path, '\0');
	അല്ലെങ്കിൽ
		write_name_quoted(path, stdout, '\n');
}

സ്ഥിതം ശൂന്യം mark_path(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം ഘടന object_id *oid,
		      ഘടന last_modified_callback_data *data)
{
	ഘടന last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&data->lm->paths, strhash(path), path,
					  ഘടന last_modified_entry, hashent);
	എങ്കിൽ (!ent)
		തിരികെ;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	എങ്കിൽ (oid && !oideq(oid, &ent->oid))
		തിരികെ;

	last_modified_emit(data->lm, path, data->commit);

	hashmap_remove(&data->lm->paths, &ent->hashent, path);
	bloom_താക്കോൽ_clear(&ent->താക്കോൽ);
	സ്വതന്ത്രം(ent);
}

സ്ഥിതം ശൂന്യം last_modified_diff(ഘടന diff_queue_struct *q,
			       ഘടന diff_options *opt UNUSED, ശൂന്യം *cbdata)
{
	ഘടന last_modified_callback_data *data = cbdata;

	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < q->nr; i++) {
		ഘടന diff_filepair *p = q->queue[i];
		തിരഞ്ഞെടുപ്പ് (p->status) {
		അവസ്ഥ DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			നിർത്തുക;

		സ്ഥിരസ്ഥിതി:
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
			നിർത്തുക;
		}
	}
}

സ്ഥിതം ശൂന്യം pass_to_parent(ഘടന bitmap *c,
			   ഘടന bitmap *p,
			   size_t pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

സ്ഥിതം bool maybe_changed_path(ഘടന last_modified *lm,
			       ഘടന commit *origin,
			       ഘടന bitmap *active)
{
	ഘടന bloom_filter *filter;
	ഘടന last_modified_entry *ent;
	ഘടന hashmap_iter iter;

	എങ്കിൽ (!lm->rev.bloom_filter_settings)
		തിരികെ true;

	എങ്കിൽ (commit_graph_generation(origin) == GENERATION_സംഖ്യ_INFINITY)
		തിരികെ true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	എങ്കിൽ (!filter)
		തിരികെ true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		എങ്കിൽ (active && !bitmap_get(active, ent->diff_idx))
			തുടരുക;

		എങ്കിൽ (bloom_filter_contains(filter, &ent->താക്കോൽ,
					  lm->rev.bloom_filter_settings))
			തിരികെ true;
	}
	തിരികെ false;
}

സ്ഥിതം ശൂന്യം process_parent(ഘടന last_modified *lm,
			   ഘടന prio_queue *queue,
			   ഘടന commit *c, ഘടന bitmap *active_c,
			   ഘടന commit *parent, പൂർണ്ണസംഖ്യ parent_i)
{
	ഘടന bitmap *active_p;

	എങ്കിൽ (repo_parse_commit(lm->rev.repo, parent))
		തിരികെ;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	എങ്കിൽ (parent_i || maybe_changed_path(lm, c, active_c)) {
		diff_വൃക്ഷം_oid(&parent->object.oid,
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
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < diff_queued_diff.nr; i++) {
		ഘടന diff_filepair *fp = diff_queued_diff.queue[i];
		സ്ഥിരം അക്ഷരം *path = fp->two->path;
		ഘടന last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    ഘടന last_modified_entry, hashent);
		എങ്കിൽ (ent) {
			size_t k = ent->diff_idx;
			എങ്കിൽ (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	വേണ്ടി (size_t i = 0; i < lm->all_paths_nr; i++) {
		എങ്കിൽ (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	എങ്കിൽ (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	എങ്കിൽ (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->വാക്കുകൾ, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ last_modified_run(ഘടന last_modified *lm)
{
	പൂർണ്ണസംഖ്യ max_എണ്ണം, queue_popped = 0;
	ഘടന commit *c, *n;
	ഘടന prio_queue queue = { compare_commits_by_gen_then_commit_date };
	ഘടന prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	ഘടന commit_list *list;
	ഘടന last_modified_callback_data data = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_data = &data;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_എണ്ണം = lm->rev.max_എണ്ണം;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	വേണ്ടി (list = lm->rev.commits; list; list = list->next) {
		ഘടന commit *c = list->item;

		എങ്കിൽ (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} അല്ലെങ്കിൽ എങ്കിൽ (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			ഘടന bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			വേണ്ടി (size_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	ആകുമ്പോൾ ((c = prio_queue_get(&queue))) {
		പൂർണ്ണസംഖ്യ parent_i;
		ഘടന commit_list *p;
		ഘടന bitmap *active_c = active_paths_for(lm, c);

		എങ്കിൽ ((0 <= max_എണ്ണം && max_എണ്ണം < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			data.commit = c;
			diff_വൃക്ഷം_oid(lm->rev.repo->hash_algo->empty_വൃക്ഷം,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			പോകുക cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		എങ്കിൽ (repo_parse_commit(lm->rev.repo, c))
			പോകുക cleanup;

		ആകുമ്പോൾ ((n = prio_queue_get(&not_queue))) {
			ഘടന commit_list *np;

			എങ്കിൽ (repo_parse_commit(lm->rev.repo, n))
				തുടരുക;

			വേണ്ടി (np = n->parents; np; np = np->next) {
				എങ്കിൽ (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			എങ്കിൽ (commit_graph_generation(n) < commit_graph_generation(c))
				നിർത്തുക;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		വേണ്ടി (p = c->parents, parent_i = 0; p; p = p->next, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			എങ്കിൽ (bitmap_is_empty(active_c))
				നിർത്തുക;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		എങ്കിൽ (!bitmap_is_empty(active_c))  {
			data.commit = c;
			വേണ്ടി (size_t i = 0; i < lm->all_paths_nr; i++) {
				എങ്കിൽ (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], ശൂന്യ, &data);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	എങ്കിൽ (hashmap_get_size(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ last_modified_init(ഘടന last_modified *lm, ഘടന repository *r,
			      സ്ഥിരം അക്ഷരം *prefix, പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	ഘടന hashmap_iter iter;
	ഘടന last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, ശൂന്യ, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_വേര്_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_വൃക്ഷം_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.വൃക്ഷം_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, ശൂന്യ);
	എങ്കിൽ (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		തിരികെ argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	എങ്കിൽ (populate_paths_from_revs(lm) < 0)
		തിരികെ -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_size(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_last_modified(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		      ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ ret;
	ഘടന last_modified lm = { 0 };

	സ്ഥിരം അക്ഷരം * സ്ഥിരം last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		ശൂന്യ
	};

	ഘടന option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', ശൂന്യ, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, ശൂന്യ);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	എങ്കിൽ (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	എങ്കിൽ (ret)
		പോകുക out;

	ret = last_modified_run(&lm);
	എങ്കിൽ (ret)
		പോകുക out;

out:
	last_modified_release(&lm);

	തിരികെ ret;
}
