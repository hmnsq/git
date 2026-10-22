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
#परिभाषय PARENT1 (1u<<16) /* used instead of SEEN */
#परिभाषय PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

संरचना चरमः_modified_प्रविष्टिः {
	संरचना hashmap_प्रविष्टिः hashent;
	संरचना object_id oid;
	संरचना bloom_कुञ्चिका कुञ्चिका;
	आकार_t diff_idx;
	ध्रुव वर्ण मार्गः[FLEX_ARRAY];
};

स्थिर पूर्णाङ्क चरमः_modified_प्रविष्टिः_hashcmp(ध्रुव शून्य *अप्रयुक्तम्_2 अप्रयुक्तम्,
				       ध्रुव संरचना hashmap_प्रविष्टिः *hent1,
				       ध्रुव संरचना hashmap_प्रविष्टिः *hent2,
				       ध्रुव शून्य *मार्गः)
{
	ध्रुव संरचना चरमः_modified_प्रविष्टिः *ent1 =
		container_सम्बन्धि(hent1, ध्रुव संरचना चरमः_modified_प्रविष्टिः, hashent);
	ध्रुव संरचना चरमः_modified_प्रविष्टिः *ent2 =
		container_सम्बन्धि(hent2, ध्रुव संरचना चरमः_modified_प्रविष्टिः, hashent);
	प्रत्यागम वाक्यं_तुलय(ent1->मार्गः, मार्गः ? मार्गः : ent2->मार्गः);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(सक्रिय_paths_for_commit, संरचना bitmap *);

संरचना चरमः_modified {
	संरचना hashmap paths;
	संरचना rev_info rev;
	bool show_trees;
	bool nul_termination;
	पूर्णाङ्क max_गभीरता;

	ध्रुव वर्ण **all_paths;
	आकार_t all_paths_nr;
	संरचना सक्रिय_paths_for_commit सक्रिय_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	संरचना bitmap *scratch;
};

स्थिर संरचना bitmap *सक्रिय_paths_for(संरचना चरमः_modified *lm, संरचना commit *c)
{
	संरचना bitmap **bitmap = सक्रिय_paths_for_commit_at(&lm->सक्रिय_paths, c);
	यदि (!*bitmap)
		*bitmap = bitmap_word_alloc(lm->all_paths_nr / द्वयङ्काः_अन्तर्_EWORD + 1);

	प्रत्यागम *bitmap;
}

स्थिर शून्य सक्रिय_paths_free(संरचना चरमः_modified *lm, संरचना commit *c)
{
	संरचना bitmap **bitmap = सक्रिय_paths_for_commit_at(&lm->सक्रिय_paths, c);
	यदि (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = रिक्त;
	}
}

स्थिर शून्य चरमः_modified_release(संरचना चरमः_modified *lm)
{
	संरचना hashmap_iter iter;
	संरचना चरमः_modified_प्रविष्टिः *ent;

	hashmap_for_each_प्रविष्टिः(&lm->paths, &iter, ent, hashent)
		bloom_कुञ्चिका_clear(&ent->कुञ्चिका);

	hashmap_clear_and_free(&lm->paths, संरचना चरमः_modified_प्रविष्टिः, hashent);
	release_revisions(&lm->rev);

	मुक्त(lm->all_paths);
}

संरचना चरमः_modified_callback_दत्त {
	संरचना चरमः_modified *lm;
	संरचना commit *commit;
};

स्थिर शून्य सङ्कलय_मार्गः_from_diff(संरचना diff_queue_struct *q,
			       संरचना diff_options *opt अप्रयुक्तम्, शून्य *दत्त)
{
	संरचना चरमः_modified *lm = दत्त;

	प्रति (पूर्णाङ्क i = 0; i < q->nr; i++) {
		संरचना diff_filepair *p = q->queue[i];
		संरचना चरमः_modified_प्रविष्टिः *ent;
		ध्रुव वर्ण *मार्गः = p->two->मार्गः;

		FLEX_ALLOC_STR(ent, मार्गः, मार्गः);
		oidcpy(&ent->oid, &p->two->oid);
		यदि (lm->rev.bloom_filter_settings)
			bloom_कुञ्चिका_fill(&ent->कुञ्चिका, मार्गः, वाक्यदैर्घ्यम्(मार्गः),
				       lm->rev.bloom_filter_settings);
		hashmap_प्रविष्टिः_init(&ent->hashent, strhash(ent->मार्गः));
		hashmap_सङ्कलय(&lm->paths, &ent->hashent);
	}
}

स्थिर पूर्णाङ्क populate_paths_from_revs(संरचना चरमः_modified *lm)
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
	स्मृतिं_प्रतिलिख(&diffopt, &lm->rev.diffopt, परिमाण(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = सङ्कलय_मार्गः_from_diff;
	diffopt.format_callback_दत्त = lm;

	प्रति (आकार_t i = 0; i < lm->rev.pending.nr; i++) {
		संरचना object_array_प्रविष्टिः *obj = lm->rev.pending.objects + i;

		यदि (obj->item->flags & UNINTERESTING)
			अनुवर्त;

		यदि (अङ्कः_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			गच्छ बहिर्गमनम्;
		}

		यदि (!repo_peel_गन्तव्यम्_type(lm->rev.repo, obj->मार्गः, 0, obj->item, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->item->type));
			गच्छ बहिर्गमनम्;
		}

		diff_वृक्षः_oid(lm->rev.repo->hash_algo->रिक्तानि_वृक्षः,
			      &obj->item->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

बहिर्गमनम्:
	clear_pathspec(&diffopt.pathspec);

	प्रत्यागम ret;
}

स्थिर शून्य चरमः_modified_emit(संरचना चरमः_modified *lm,
			       ध्रुव वर्ण *मार्गः, ध्रुव संरचना commit *commit)

{
	यदि (commit->object.flags & BOUNDARY)
		अक्षरं_लिख('^');
	प्रारूप_लिख("%s\t", oid_गन्तव्यम्_hex(&commit->object.oid));

	यदि (lm->nul_termination)
		प्रारूप_लिख("%s%c", मार्गः, '\0');
	अन्यथा
		write_name_quoted(मार्गः, stdout, '\n');
}

स्थिर शून्य mark_मार्गः(ध्रुव वर्ण *मार्गः, ध्रुव संरचना object_id *oid,
		      संरचना चरमः_modified_callback_दत्त *दत्त)
{
	संरचना चरमः_modified_प्रविष्टिः *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_प्रविष्टिः_from_hash(&दत्त->lm->paths, strhash(मार्गः), मार्गः,
					  संरचना चरमः_modified_प्रविष्टिः, hashent);
	यदि (!ent)
		प्रत्यागम;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	यदि (oid && !oideq(oid, &ent->oid))
		प्रत्यागम;

	चरमः_modified_emit(दत्त->lm, मार्गः, दत्त->commit);

	hashmap_remove(&दत्त->lm->paths, &ent->hashent, मार्गः);
	bloom_कुञ्चिका_clear(&ent->कुञ्चिका);
	मुक्त(ent);
}

स्थिर शून्य चरमः_modified_diff(संरचना diff_queue_struct *q,
			       संरचना diff_options *opt अप्रयुक्तम्, शून्य *cbdata)
{
	संरचना चरमः_modified_callback_दत्त *दत्त = cbdata;

	प्रति (पूर्णाङ्क i = 0; i < q->nr; i++) {
		संरचना diff_filepair *p = q->queue[i];
		विकल्प (p->स्थितिः) {
		पक्ष DIFF_स्थितिः_DELETED:
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
			mark_मार्गः(p->two->मार्गः, &p->two->oid, दत्त);
			भङ्ग;
		}
	}
}

स्थिर शून्य pass_गन्तव्यम्_parent(संरचना bitmap *c,
			   संरचना bitmap *p,
			   आकार_t pos)
{
	bitmap_unset(c, pos);
	bitmap_समुच्चयः(p, pos);
}

स्थिर bool maybe_परिवर्तितम्_मार्गः(संरचना चरमः_modified *lm,
			       संरचना commit *origin,
			       संरचना bitmap *सक्रिय)
{
	संरचना bloom_filter *filter;
	संरचना चरमः_modified_प्रविष्टिः *ent;
	संरचना hashmap_iter iter;

	यदि (!lm->rev.bloom_filter_settings)
		प्रत्यागम true;

	यदि (commit_graph_generation(origin) == GENERATION_सङ्ख्या_INFINITY)
		प्रत्यागम true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	यदि (!filter)
		प्रत्यागम true;

	hashmap_for_each_प्रविष्टिः(&lm->paths, &iter, ent, hashent) {
		यदि (सक्रिय && !bitmap_get(सक्रिय, ent->diff_idx))
			अनुवर्त;

		यदि (bloom_filter_अन्तर्भवति(filter, &ent->कुञ्चिका,
					  lm->rev.bloom_filter_settings))
			प्रत्यागम true;
	}
	प्रत्यागम false;
}

स्थिर शून्य process_parent(संरचना चरमः_modified *lm,
			   संरचना prio_queue *queue,
			   संरचना commit *c, संरचना bitmap *सक्रिय_c,
			   संरचना commit *parent, पूर्णाङ्क parent_i)
{
	संरचना bitmap *सक्रिय_p;

	यदि (repo_parse_commit(lm->rev.repo, parent))
		प्रत्यागम;
	सक्रिय_p = सक्रिय_paths_for(lm, parent);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	यदि (parent_i || maybe_परिवर्तितम्_मार्गः(lm, c, सक्रिय_c)) {
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
		ध्रुव वर्ण *मार्गः = fp->two->मार्गः;
		संरचना चरमः_modified_प्रविष्टिः *ent =
			hashmap_get_प्रविष्टिः_from_hash(&lm->paths, strhash(मार्गः), मार्गः,
						    संरचना चरमः_modified_प्रविष्टिः, hashent);
		यदि (ent) {
			आकार_t k = ent->diff_idx;
			यदि (bitmap_get(सक्रिय_c, k))
				bitmap_समुच्चयः(lm->scratch, k);
		}
	}
	प्रति (आकार_t i = 0; i < lm->all_paths_nr; i++) {
		यदि (bitmap_get(सक्रिय_c, i) && !bitmap_get(lm->scratch, i))
			pass_गन्तव्यम्_parent(सक्रिय_c, सक्रिय_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	यदि (!bitmap_is_रिक्तानि(सक्रिय_p) && !(parent->object.flags & PARENT1)) {
		parent->object.flags |= PARENT1;
		prio_queue_put(queue, parent);
	}
	यदि (!(parent->object.flags & PARENT1))
		सक्रिय_paths_free(lm, parent);

	MEMZERO_ARRAY(lm->scratch->शब्दाः, lm->scratch->word_alloc);
	diff_queue_clear(&diff_queued_diff);
}

स्थिर पूर्णाङ्क चरमः_modified_सततम्(संरचना चरमः_modified *lm)
{
	पूर्णाङ्क max_गणनम्, queue_popped = 0;
	संरचना commit *c, *n;
	संरचना prio_queue queue = { compare_commits_by_gen_then_commit_date };
	संरचना prio_queue not_queue = { compare_commits_by_gen_then_commit_date };
	संरचना commit_list *list;
	संरचना चरमः_modified_callback_दत्त दत्त = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = चरमः_modified_diff;
	lm->rev.diffopt.format_callback_दत्त = &दत्त;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_गणनम् = lm->rev.max_गणनम्;

	init_सक्रिय_paths_for_commit(&lm->सक्रिय_paths);
	lm->scratch = bitmap_word_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	प्रति (list = lm->rev.commits; list; list = list->अग्र) {
		संरचना commit *c = list->item;

		यदि (c->object.flags & BOTTOM) {
			prio_queue_put(&not_queue, c);
			c->object.flags |= PARENT2;
		} अन्यथा यदि (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			संरचना bitmap *सक्रिय;

			prio_queue_put(&queue, c);
			c->object.flags |= PARENT1;

			सक्रिय = सक्रिय_paths_for(lm, c);
			प्रति (आकार_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_समुच्चयः(सक्रिय, i);
		}
	}

	यावत् ((c = prio_queue_get(&queue))) {
		पूर्णाङ्क parent_i;
		संरचना commit_list *p;
		संरचना bitmap *सक्रिय_c = सक्रिय_paths_for(lm, c);

		यदि ((0 <= max_गणनम् && max_गणनम् < ++queue_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			दत्त.commit = c;
			diff_वृक्षः_oid(lm->rev.repo->hash_algo->रिक्तानि_वृक्षः,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			गच्छ cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		यदि (repo_parse_commit(lm->rev.repo, c))
			गच्छ cleanup;

		यावत् ((n = prio_queue_get(&not_queue))) {
			संरचना commit_list *np;

			यदि (repo_parse_commit(lm->rev.repo, n))
				अनुवर्त;

			प्रति (np = n->parents; np; np = np->अग्र) {
				यदि (!(np->item->object.flags & PARENT2)) {
					prio_queue_put(&not_queue, np->item);
					np->item->object.flags |= PARENT2;
				}
			}

			यदि (commit_graph_generation(n) < commit_graph_generation(c))
				भङ्ग;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		प्रति (p = c->parents, parent_i = 0; p; p = p->अग्र, parent_i++) {
			process_parent(lm, &queue,
				       c, सक्रिय_c,
				       p->item, parent_i);

			यदि (bitmap_is_रिक्तानि(सक्रिय_c))
				भङ्ग;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		यदि (!bitmap_is_रिक्तानि(सक्रिय_c))  {
			दत्त.commit = c;
			प्रति (आकार_t i = 0; i < lm->all_paths_nr; i++) {
				यदि (bitmap_get(सक्रिय_c, i))
					mark_मार्गः(lm->all_paths[i], रिक्त, &दत्त);
			}
		}

cleanup:
		सक्रिय_paths_free(lm, c);
	}

	यदि (hashmap_get_आकार(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_queue(&not_queue);
	clear_prio_queue(&queue);
	clear_सक्रिय_paths_for_commit(&lm->सक्रिय_paths);
	bitmap_free(lm->scratch);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क चरमः_modified_init(संरचना चरमः_modified *lm, संरचना repository *r,
			      ध्रुव वर्ण *उपसर्गः, पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	संरचना hashmap_iter iter;
	संरचना चरमः_modified_प्रविष्टिः *ent;

	hashmap_init(&lm->paths, चरमः_modified_प्रविष्टिः_hashcmp, रिक्त, 0);

	repo_init_revisions(r, &lm->rev, उपसर्गः);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_मूलम्_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_वृक्षः_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.वृक्षः_अन्तर्_recursive = lm->show_trees;
	lm->rev.diffopt.max_गभीरता = lm->max_गभीरता;
	lm->rev.diffopt.max_गभीरता_valid = lm->max_गभीरता >= 0;

	argc = सज्जता_revisions(argc, argv, &lm->rev, रिक्त);
	यदि (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		प्रत्यागम argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	यदि (populate_paths_from_revs(lm) < 0)
		प्रत्यागम -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_आकार(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_प्रविष्टिः(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->मार्गः;
	}

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_चरमः_modified(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		      संरचना repository *repo)
{
	पूर्णाङ्क ret;
	संरचना चरमः_modified lm = { 0 };

	ध्रुव वर्ण * ध्रुव चरमः_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		रिक्त
	};

	संरचना option चरमः_modified_options[] = {
		OPT_समुच्चयः_INT('r', "recursive", &lm.max_गभीरता,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_गभीरता,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', रिक्त, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, चरमः_modified_options,
			     चरमः_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, रिक्त);

	ret = चरमः_modified_init(&lm, repo, उपसर्गः, argc, argv);
	यदि (ret > 0)
		usage_with_options(चरमः_modified_usage,
				   चरमः_modified_options);
	यदि (ret)
		गच्छ बहिर्गमनम्;

	ret = चरमः_modified_सततम्(&lm);
	यदि (ret)
		गच्छ बहिर्गमनम्;

बहिर्गमनम्:
	चरमः_modified_release(&lm);

	प्रत्यागम ret;
}
