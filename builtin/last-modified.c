#समावेश "git-compat-util.h"
#समावेश "bloom.h"
#समावेश "builtin.h"
#समावेश "commit-graph.h"
#समावेश "commit-slab.h"
#समावेश "commit.h"
#समावेश "config.h"
#समावेश "diff.h"
#समावेश "diffcore.h"
#समावेश "environment.h"
#समावेश "ewah/ewok.h"
#समावेश "hashmap.h"
#समावेश "hex.h"
#समावेश "object-name.h"
#समावेश "object.h"
#समावेश "parse-options.h"
#समावेश "prio-queue.h"
#समावेश "quote.h"
#समावेश "repository.h"
#समावेश "revision.h"

/* Remember to update object flag allocation in object.h */
#घोषणा PARENT1 (1u<<16) /* used instead of SEEN */
#घोषणा PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

काष्ठा last_modified_entry {
	काष्ठा hashmap_entry hashent;
	काष्ठा object_id oid;
	काष्ठा bloom_कुंजी कुंजी;
	माप_प्रकार diff_idx;
	स्थिर अक्षर path[FLEX_ARRAY];
};

अटल पूर्णांक last_modified_entry_hashcmp(स्थिर व्योम *unused UNUSED,
				       स्थिर काष्ठा hashmap_entry *hent1,
				       स्थिर काष्ठा hashmap_entry *hent2,
				       स्थिर व्योम *path)
{
	स्थिर काष्ठा last_modified_entry *ent1 =
		container_of(hent1, स्थिर काष्ठा last_modified_entry, hashent);
	स्थिर काष्ठा last_modified_entry *ent2 =
		container_of(hent2, स्थिर काष्ठा last_modified_entry, hashent);
	वापस म_भेद(ent1->path, path ? path : ent2->path);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(active_paths_for_commit, काष्ठा bitmap *);

काष्ठा last_modified {
	काष्ठा hashmap paths;
	काष्ठा rev_info rev;
	bool show_trees;
	bool nul_termination;
	पूर्णांक max_depth;

	स्थिर अक्षर **all_paths;
	माप_प्रकार all_paths_nr;
	काष्ठा active_paths_for_commit active_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	काष्ठा bitmap *scratch;
};

अटल काष्ठा bitmap *active_paths_for(काष्ठा last_modified *lm, काष्ठा commit *c)
{
	काष्ठा bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	अगर (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / BITS_IN_EWORD + 1);

	वापस *bitmap;
}

अटल व्योम active_paths_free(काष्ठा last_modified *lm, काष्ठा commit *c)
{
	काष्ठा bitmap **bitmap = active_paths_for_commit_at(&lm->active_paths, c);
	अगर (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = शून्य;
	}
}

अटल व्योम last_modified_release(काष्ठा last_modified *lm)
{
	काष्ठा hashmap_iter iter;
	काष्ठा last_modified_entry *ent;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent)
		bloom_कुंजी_clear(&ent->कुंजी);

	hashmap_clear_and_free(&lm->paths, काष्ठा last_modified_entry, hashent);
	release_revisions(&lm->rev);

	मुक्त(lm->all_paths);
}

काष्ठा last_modified_callback_तथ्य {
	काष्ठा last_modified *lm;
	काष्ठा commit *commit;
};

अटल व्योम add_path_from_diff(काष्ठा diff_queue_struct *q,
			       काष्ठा diff_options *opt UNUSED, व्योम *तथ्य)
{
	काष्ठा last_modified *lm = तथ्य;

	क्रम (पूर्णांक i = 0; i < q->nr; i++) {
		काष्ठा diff_filepair *p = q->queue[i];
		काष्ठा last_modified_entry *ent;
		स्थिर अक्षर *path = p->two->path;

		FLEX_ALLOC_STR(ent, path, path);
		oidcpy(&ent->oid, &p->two->oid);
		अगर (lm->rev.bloom_filter_settings)
			bloom_कुंजी_fill(&ent->कुंजी, path, म_माप(path),
				       lm->rev.bloom_filter_settings);
		hashmap_entry_init(&ent->hashent, strhash(ent->path));
		hashmap_add(&lm->paths, &ent->hashent);
	}
}

अटल पूर्णांक populate_paths_from_revs(काष्ठा last_modified *lm)
{
	पूर्णांक अंक_interesting = 0, ret = 0;
	काष्ठा diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	स_नकल(&diffopt, &lm->rev.diffopt, माप(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = add_path_from_diff;
	diffopt.format_callback_तथ्य = lm;

	क्रम (माप_प्रकार i = 0; i < lm->rev.pending.nr; i++) {
		काष्ठा object_array_entry *obj = lm->rev.pending.objects + i;

		अगर (obj->item->flags & UNINTERESTING)
			जारी;

		अगर (अंक_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			जाओ out;
		}

		अगर (!repo_peel_to_type(lm->rev.repo, obj->path, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			जाओ out;
		}

		diff_वृक्ष_oid(lm->rev.repo->hash_algo->empty_वृक्ष,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

out:
	clear_pathspec(&diffopt.pathspec);

	वापस ret;
}

अटल व्योम last_modified_emit(काष्ठा last_modified *lm,
			       स्थिर अक्षर *path, स्थिर काष्ठा commit *commit)

{
	अगर (commit->object.flags & BOUNDARY)
		अक्षर_दो('^');
	म_लिखो("%s\t", oid_to_hex(&commit->object.oid));

	अगर (lm->nul_termination)
		म_लिखो("%s%c", path, '\0');
	अन्यथा
		write_name_quoted(path, मानक_निकास, '\n');
}

अटल व्योम mark_path(स्थिर अक्षर *path, स्थिर काष्ठा object_id *oid,
		      काष्ठा last_modified_callback_तथ्य *तथ्य)
{
	काष्ठा last_modified_entry *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_entry_from_hash(&तथ्य->lm->paths, strhash(path), path,
					  काष्ठा last_modified_entry, hashent);
	अगर (!ent)
		वापस;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	अगर (oid && !oideq(oid, &ent->oid))
		वापस;

	last_modified_emit(तथ्य->lm, path, तथ्य->commit);

	hashmap_remove(&तथ्य->lm->paths, &ent->hashent, path);
	bloom_कुंजी_clear(&ent->कुंजी);
	मुक्त(ent);
}

अटल व्योम last_modified_diff(काष्ठा diff_queue_struct *q,
			       काष्ठा diff_options *opt UNUSED, व्योम *cbdata)
{
	काष्ठा last_modified_callback_तथ्य *तथ्य = cbdata;

	क्रम (पूर्णांक i = 0; i < q->nr; i++) {
		काष्ठा diff_filepair *p = q->queue[i];
		चयन (p->status) {
		हाल DIFF_STATUS_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			अवरोध;

		शेष:
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
			mark_path(p->two->path, &p->two->oid, तथ्य);
			अवरोध;
		}
	}
}

अटल व्योम pass_to_parent(काष्ठा bitmap *c,
			   काष्ठा bitmap *p,
			   माप_प्रकार pos)
{
	bitmap_unset(c, pos);
	bitmap_set(p, pos);
}

अटल bool maybe_changed_path(काष्ठा last_modified *lm,
			       काष्ठा commit *origin,
			       काष्ठा bitmap *active)
{
	काष्ठा bloom_filter *filter;
	काष्ठा last_modified_entry *ent;
	काष्ठा hashmap_iter iter;

	अगर (!lm->rev.bloom_filter_settings)
		वापस true;

	अगर (commit_graph_generation(origin) == GENERATION_संख्या_INFINITY)
		वापस true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	अगर (!filter)
		वापस true;

	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		अगर (active && !bitmap_get(active, ent->diff_idx))
			जारी;

		अगर (bloom_filter_contains(filter, &ent->कुंजी,
					  lm->rev.bloom_filter_settings))
			वापस true;
	}
	वापस false;
}

अटल व्योम process_parent(काष्ठा last_modified *lm,
			   काष्ठा prio_queue *queue,
			   काष्ठा commit *c, काष्ठा bitmap *active_c,
			   काष्ठा commit *parent, पूर्णांक parent_i)
{
	काष्ठा bitmap *active_p;

	अगर (repo_parse_commit(lm->rev.repo, parent))
		वापस;
	active_p = active_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	अगर (parent_i || maybe_changed_path(lm, c, active_c)) {
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
	क्रम (पूर्णांक i = 0; i < diff_queued_diff.nr; i++) {
		काष्ठा diff_filepair *fp = diff_queued_diff.queue[i];
		स्थिर अक्षर *path = fp->two->path;
		काष्ठा last_modified_entry *ent =
			hashmap_get_entry_from_hash(&lm->paths, strhash(path), path,
						    काष्ठा last_modified_entry, hashent);
		अगर (ent) {
			माप_प्रकार k = ent->diff_idx;
			अगर (bitmap_get(active_c, k))
				bitmap_set(lm->scratch, k);
		}
	}
	क्रम (माप_प्रकार i = 0; i < lm->all_paths_nr; i++) {
		अगर (bitmap_get(active_c, i) && !bitmap_get(lm->scratch, i))
			pass_to_parent(active_c, active_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	अगर (!bitmap_is_empty(active_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	अगर (!(parent->object.flags & PARENT1))
		active_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->शब्दगण, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

अटल पूर्णांक last_modified_run(काष्ठा last_modified *lm)
{
	पूर्णांक max_गिनती, queue_popped = 0;
	काष्ठा commit *c, *n;
	काष्ठा prio_queue queue = { compare_commits_by_gen_then_commit_date };
	काष्ठा prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	काष्ठा commit_list *list;
	काष्ठा last_modified_callback_तथ्य तथ्य = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = last_modified_diff;
	lm->rev.diffopt.format_callback_तथ्य = &तथ्य;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_गिनती = lm->rev.max_गिनती;

	init_active_paths_for_commit(&lm->active_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	क्रम (list = lm->rev.commits; list; list = list->अग्र) {
		काष्ठा commit *c = list->item;

		अगर (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} अन्यथा अगर (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			काष्ठा bitmap *active;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			active = active_paths_for(lm, c);
			क्रम (माप_प्रकार i = 0; i < lm->all_paths_nr; i++)
				bitmap_set(active, i);
		}
	}

	जबतक ((c = prio_queue_get(&queue))) {
		पूर्णांक parent_i;
		काष्ठा commit_list *p;
		काष्ठा bitmap *active_c = active_paths_for(lm, c);

		अगर ((0 <= max_गिनती && max_गिनती < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			तथ्य.commit = c;
			diff_वृक्ष_oid(lm->rev.repo->hash_algo->empty_वृक्ष,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			जाओ cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		अगर (repo_parse_commit(lm->rev.repo, c))
			जाओ cleanup;

		जबतक ((n = prio_queue_get(&not_queue))) {
			काष्ठा commit_list *np;

			अगर (repo_parse_commit(lm->rev.repo, n))
				जारी;

			क्रम (np = n->parents; np; np = np->अग्र) {
				अगर (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			अगर (commit_graph_generation(n) < commit_graph_generation(c))
				अवरोध;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		क्रम (p = c->parents, parent_i = 0; p; p = p->अग्र, parent_i++) {
			process_parent(lm, &queue,
				       c, active_c,
				       p->item, parent_i);

			अगर (bitmap_is_empty(active_c))
				अवरोध;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		अगर (!bitmap_is_empty(active_c))  {
			तथ्य.commit = c;
			क्रम (माप_प्रकार i = 0; i < lm->all_paths_nr; i++) {
				अगर (bitmap_get(active_c, i))
					mark_path(lm->all_paths[i], शून्य, &तथ्य);
			}
		}

cleanup:
		active_paths_free(lm, c);
	}

	अगर (hashmap_get_आकार(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_active_paths_for_commit(&lm->active_paths);
	bitmap_free(lm->scratch);

	वापस 0;
}

अटल पूर्णांक last_modified_init(काष्ठा last_modified *lm, काष्ठा repository *r,
			      स्थिर अक्षर *prefix, पूर्णांक argc, स्थिर अक्षर **argv)
{
	काष्ठा hashmap_iter iter;
	काष्ठा last_modified_entry *ent;

	hashmap_init(&lm->paths, last_modified_entry_hashcmp, शून्य, 0);

	repo_init_revisions(r, &lm->rev, prefix);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_मूल_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_वृक्ष_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.वृक्ष_in_recursive = lm->show_trees;
	lm->rev.diffopt.max_depth = lm->max_depth;
	lm->rev.diffopt.max_depth_valid = lm->max_depth >= 0;

	argc = setup_revisions(argc, argv, &lm->rev, शून्य);
	अगर (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		वापस argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	अगर (populate_paths_from_revs(lm) < 0)
		वापस -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_आकार(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_entry(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->path;
	}

	वापस 0;
}

पूर्णांक cmd_last_modified(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		      काष्ठा repository *repo)
{
	पूर्णांक ret;
	काष्ठा last_modified lm = { 0 };

	स्थिर अक्षर * स्थिर last_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		शून्य
	};

	काष्ठा option last_modified_options[] = {
		OPT_SET_INT('r', "recursive", &lm.max_depth,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_depth,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', शून्य, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, last_modified_options,
			     last_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, शून्य);

	ret = last_modified_init(&lm, repo, prefix, argc, argv);
	अगर (ret > 0)
		usage_with_options(last_modified_usage,
				   last_modified_options);
	अगर (ret)
		जाओ out;

	ret = last_modified_run(&lm);
	अगर (ret)
		जाओ out;

out:
	last_modified_release(&lm);

	वापस ret;
}
