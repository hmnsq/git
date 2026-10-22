#అన్తర్భావ "git-compat-util.h"
#అన్తర్భావ "bloom.h"
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "commit-graph.h"
#అన్తర్భావ "commit-slab.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "diffcore.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "ewah/ewok.h"
#అన్తర్భావ "hashmap.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "prio-queue.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "repository.h"
#అన్తర్భావ "revision.h"

/* Remember to update object flag allocation in object.h */
#నిర్వచించు PARENT1 (1u<<16) /* used instead of SEEN */
#నిర్వచించు PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

నిర్మాణం చివరి_modified_నమోదు {
	నిర్మాణం hashmap_నమోదు hashent;
	నిర్మాణం object_id oid;
	నిర్మాణం bloom_కీలకం కీలకం;
	కొలత_t diff_idx;
	స్థిరరాశి అక్షరం మార్గం[FLEX_ARRAY];
};

స్థిర పూర్ణాంకం చివరి_modified_నమోదు_hashcmp(స్థిరరాశి శూన్య *వాడని_2 వాడని,
				       స్థిరరాశి నిర్మాణం hashmap_నమోదు *hent1,
				       స్థిరరాశి నిర్మాణం hashmap_నమోదు *hent2,
				       స్థిరరాశి శూన్య *మార్గం)
{
	స్థిరరాశి నిర్మాణం చివరి_modified_నమోదు *ent1 =
		container_యొక్క(hent1, స్థిరరాశి నిర్మాణం చివరి_modified_నమోదు, hashent);
	స్థిరరాశి నిర్మాణం చివరి_modified_నమోదు *ent2 =
		container_యొక్క(hent2, స్థిరరాశి నిర్మాణం చివరి_modified_నమోదు, hashent);
	తిరిగి strcmp(ent1->మార్గం, మార్గం ? మార్గం : ent2->మార్గం);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(చురుకైన_paths_for_commit, నిర్మాణం bitmap *);

నిర్మాణం చివరి_modified {
	నిర్మాణం hashmap paths;
	నిర్మాణం rev_info rev;
	bool show_trees;
	bool nul_termination;
	పూర్ణాంకం max_లోతు;

	స్థిరరాశి అక్షరం **all_paths;
	కొలత_t all_paths_nr;
	నిర్మాణం చురుకైన_paths_for_commit చురుకైన_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	నిర్మాణం bitmap *scratch;
};

స్థిర నిర్మాణం bitmap *చురుకైన_paths_for(నిర్మాణం చివరి_modified *lm, నిర్మాణం commit *c)
{
	నిర్మాణం bitmap **bitmap = చురుకైన_paths_for_commit_at(&lm->చురుకైన_paths, c);
	ఒకవేళ (!*bitmap)
		*bitmap = bitmap_మాట_alloc(lm->all_paths_nr / బిట్లు_లోపల_EWORD + 1);

	తిరిగి *bitmap;
}

స్థిర శూన్య చురుకైన_paths_free(నిర్మాణం చివరి_modified *lm, నిర్మాణం commit *c)
{
	నిర్మాణం bitmap **bitmap = చురుకైన_paths_for_commit_at(&lm->చురుకైన_paths, c);
	ఒకవేళ (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = ఖాళీ;
	}
}

స్థిర శూన్య చివరి_modified_release(నిర్మాణం చివరి_modified *lm)
{
	నిర్మాణం hashmap_iter iter;
	నిర్మాణం చివరి_modified_నమోదు *ent;

	hashmap_for_each_నమోదు(&lm->paths, &iter, ent, hashent)
		bloom_కీలకం_clear(&ent->కీలకం);

	hashmap_clear_and_free(&lm->paths, నిర్మాణం చివరి_modified_నమోదు, hashent);
	release_revisions(&lm->rev);

	విడుదల(lm->all_paths);
}

నిర్మాణం చివరి_modified_callback_దత్తాంశం {
	నిర్మాణం చివరి_modified *lm;
	నిర్మాణం commit *commit;
};

స్థిర శూన్య కూడు_మార్గం_from_diff(నిర్మాణం diff_వరుస_struct *q,
			       నిర్మాణం diff_options *opt వాడని, శూన్య *దత్తాంశం)
{
	నిర్మాణం చివరి_modified *lm = దత్తాంశం;

	కొరకు (పూర్ణాంకం i = 0; i < q->nr; i++) {
		నిర్మాణం diff_filepair *p = q->వరుస[i];
		నిర్మాణం చివరి_modified_నమోదు *ent;
		స్థిరరాశి అక్షరం *మార్గం = p->two->మార్గం;

		FLEX_ALLOC_STR(ent, మార్గం, మార్గం);
		oidcpy(&ent->oid, &p->two->oid);
		ఒకవేళ (lm->rev.bloom_filter_settings)
			bloom_కీలకం_fill(&ent->కీలకం, మార్గం, అక్షరశ్రేణి_పొడవు(మార్గం),
				       lm->rev.bloom_filter_settings);
		hashmap_నమోదు_init(&ent->hashent, strhash(ent->మార్గం));
		hashmap_కూడు(&lm->paths, &ent->hashent);
	}
}

స్థిర పూర్ణాంకం populate_paths_from_revs(నిర్మాణం చివరి_modified *lm)
{
	పూర్ణాంకం అంకె_interesting = 0, ret = 0;
	నిర్మాణం diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	జ్ఞాపకం_నకలు(&diffopt, &lm->rev.diffopt, పరిమాణం(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = కూడు_మార్గం_from_diff;
	diffopt.format_callback_దత్తాంశం = lm;

	కొరకు (కొలత_t i = 0; i < lm->rev.pending.nr; i++) {
		నిర్మాణం object_array_నమోదు *obj = lm->rev.pending.objects + i;

		ఒకవేళ (obj->అంశం->flags & UNINTERESTING)
			కొనసాగు;

		ఒకవేళ (అంకె_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			వెళ్ళు బహిర్గతం;
		}

		ఒకవేళ (!repo_peel_గమ్యం_type(lm->rev.repo, obj->మార్గం, 0, obj->అంశం, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->అంశం->type));
			వెళ్ళు బహిర్గతం;
		}

		diff_వృక్షం_oid(lm->rev.repo->hash_algo->ఖాళీలు_వృక్షం,
			      &obj->అంశం->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

బహిర్గతం:
	clear_pathspec(&diffopt.pathspec);

	తిరిగి ret;
}

స్థిర శూన్య చివరి_modified_emit(నిర్మాణం చివరి_modified *lm,
			       స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి నిర్మాణం commit *commit)

{
	ఒకవేళ (commit->object.flags & BOUNDARY)
		అక్షరం_రాయి('^');
	రాయి("%s\t", oid_గమ్యం_hex(&commit->object.oid));

	ఒకవేళ (lm->nul_termination)
		రాయి("%s%c", మార్గం, '\0');
	లేకపోతే
		write_name_quoted(మార్గం, stdout, '\n');
}

స్థిర శూన్య mark_మార్గం(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి నిర్మాణం object_id *oid,
		      నిర్మాణం చివరి_modified_callback_దత్తాంశం *దత్తాంశం)
{
	నిర్మాణం చివరి_modified_నమోదు *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_నమోదు_from_hash(&దత్తాంశం->lm->paths, strhash(మార్గం), మార్గం,
					  నిర్మాణం చివరి_modified_నమోదు, hashent);
	ఒకవేళ (!ent)
		తిరిగి;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	ఒకవేళ (oid && !oideq(oid, &ent->oid))
		తిరిగి;

	చివరి_modified_emit(దత్తాంశం->lm, మార్గం, దత్తాంశం->commit);

	hashmap_remove(&దత్తాంశం->lm->paths, &ent->hashent, మార్గం);
	bloom_కీలకం_clear(&ent->కీలకం);
	విడుదల(ent);
}

స్థిర శూన్య చివరి_modified_diff(నిర్మాణం diff_వరుస_struct *q,
			       నిర్మాణం diff_options *opt వాడని, శూన్య *cbdata)
{
	నిర్మాణం చివరి_modified_callback_దత్తాంశం *దత్తాంశం = cbdata;

	కొరకు (పూర్ణాంకం i = 0; i < q->nr; i++) {
		నిర్మాణం diff_filepair *p = q->వరుస[i];
		ఎంపిక (p->స్థితి_2) {
		స్థితి DIFF_స్థితి_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			ఆపు;

		అప్రమేయ:
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
			mark_మార్గం(p->two->మార్గం, &p->two->oid, దత్తాంశం);
			ఆపు;
		}
	}
}

స్థిర శూన్య pass_గమ్యం_మాతృ(నిర్మాణం bitmap *c,
			   నిర్మాణం bitmap *p,
			   కొలత_t pos)
{
	bitmap_unset(c, pos);
	bitmap_సమితి(p, pos);
}

స్థిర bool maybe_మారింది_మార్గం(నిర్మాణం చివరి_modified *lm,
			       నిర్మాణం commit *origin,
			       నిర్మాణం bitmap *చురుకైన)
{
	నిర్మాణం bloom_filter *filter;
	నిర్మాణం చివరి_modified_నమోదు *ent;
	నిర్మాణం hashmap_iter iter;

	ఒకవేళ (!lm->rev.bloom_filter_settings)
		తిరిగి true;

	ఒకవేళ (commit_లేఖాచిత్రం_generation(origin) == GENERATION_సంఖ్య_INFINITY)
		తిరిగి true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	ఒకవేళ (!filter)
		తిరిగి true;

	hashmap_for_each_నమోదు(&lm->paths, &iter, ent, hashent) {
		ఒకవేళ (చురుకైన && !bitmap_get(చురుకైన, ent->diff_idx))
			కొనసాగు;

		ఒకవేళ (bloom_filter_కలిగియుంది(filter, &ent->కీలకం,
					  lm->rev.bloom_filter_settings))
			తిరిగి true;
	}
	తిరిగి false;
}

స్థిర శూన్య process_మాతృ(నిర్మాణం చివరి_modified *lm,
			   నిర్మాణం prio_వరుస *వరుస,
			   నిర్మాణం commit *c, నిర్మాణం bitmap *చురుకైన_c,
			   నిర్మాణం commit *మాతృ, పూర్ణాంకం మాతృ_i)
{
	నిర్మాణం bitmap *చురుకైన_p;

	ఒకవేళ (repo_parse_commit(lm->rev.repo, మాతృ))
		తిరిగి;
	చురుకైన_p = చురుకైన_paths_for(lm, మాతృ);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	ఒకవేళ (మాతృ_i || maybe_మారింది_మార్గం(lm, c, చురుకైన_c)) {
		diff_వృక్షం_oid(&మాతృ->object.oid,
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
	కొరకు (పూర్ణాంకం i = 0; i < diff_queued_diff.nr; i++) {
		నిర్మాణం diff_filepair *fp = diff_queued_diff.వరుస[i];
		స్థిరరాశి అక్షరం *మార్గం = fp->two->మార్గం;
		నిర్మాణం చివరి_modified_నమోదు *ent =
			hashmap_get_నమోదు_from_hash(&lm->paths, strhash(మార్గం), మార్గం,
						    నిర్మాణం చివరి_modified_నమోదు, hashent);
		ఒకవేళ (ent) {
			కొలత_t k = ent->diff_idx;
			ఒకవేళ (bitmap_get(చురుకైన_c, k))
				bitmap_సమితి(lm->scratch, k);
		}
	}
	కొరకు (కొలత_t i = 0; i < lm->all_paths_nr; i++) {
		ఒకవేళ (bitmap_get(చురుకైన_c, i) && !bitmap_get(lm->scratch, i))
			pass_గమ్యం_మాతృ(చురుకైన_c, చురుకైన_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	ఒకవేళ (!bitmap_is_ఖాళీలు(చురుకైన_p) && !(మాతృ->object.flags & PARENT1)) {
		మాతృ->object.flags |= PARENT1;
		prio_వరుస_put(వరుస, మాతృ);
	}
	ఒకవేళ (!(మాతృ->object.flags & PARENT1))
		చురుకైన_paths_free(lm, మాతృ);

	MEMZERO_ARRAY(lm->scratch->పదాలు, lm->scratch->మాట_alloc);
	diff_వరుస_clear(&diff_queued_diff);
}

స్థిర పూర్ణాంకం చివరి_modified_నిరంతరం(నిర్మాణం చివరి_modified *lm)
{
	పూర్ణాంకం max_లెక్క, వరుస_popped = 0;
	నిర్మాణం commit *c, *n;
	నిర్మాణం prio_వరుస వరుస = { compare_commits_by_gen_then_commit_date };
	నిర్మాణం prio_వరుస not_వరుస = { compare_commits_by_gen_then_commit_date };
	నిర్మాణం commit_list *list;
	నిర్మాణం చివరి_modified_callback_దత్తాంశం దత్తాంశం = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = చివరి_modified_diff;
	lm->rev.diffopt.format_callback_దత్తాంశం = &దత్తాంశం;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_లెక్క = lm->rev.max_లెక్క;

	init_చురుకైన_paths_for_commit(&lm->చురుకైన_paths);
	lm->scratch = bitmap_మాట_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	కొరకు (list = lm->rev.commits; list; list = list->తదుపరి) {
		నిర్మాణం commit *c = list->అంశం;

		ఒకవేళ (c->object.flags & BOTTOM) {
			prio_వరుస_put(&not_వరుస, c);
			c->object.flags |= PARENT2;
		} లేకపోతే ఒకవేళ (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			నిర్మాణం bitmap *చురుకైన;

			prio_వరుస_put(&వరుస, c);
			c->object.flags |= PARENT1;

			చురుకైన = చురుకైన_paths_for(lm, c);
			కొరకు (కొలత_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_సమితి(చురుకైన, i);
		}
	}

	అయితే ((c = prio_వరుస_get(&వరుస))) {
		పూర్ణాంకం మాతృ_i;
		నిర్మాణం commit_list *p;
		నిర్మాణం bitmap *చురుకైన_c = చురుకైన_paths_for(lm, c);

		ఒకవేళ ((0 <= max_లెక్క && max_లెక్క < ++వరుస_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			దత్తాంశం.commit = c;
			diff_వృక్షం_oid(lm->rev.repo->hash_algo->ఖాళీలు_వృక్షం,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			వెళ్ళు cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		ఒకవేళ (repo_parse_commit(lm->rev.repo, c))
			వెళ్ళు cleanup;

		అయితే ((n = prio_వరుస_get(&not_వరుస))) {
			నిర్మాణం commit_list *np;

			ఒకవేళ (repo_parse_commit(lm->rev.repo, n))
				కొనసాగు;

			కొరకు (np = n->parents; np; np = np->తదుపరి) {
				ఒకవేళ (!(np->అంశం->object.flags & PARENT2)) {
					prio_వరుస_put(&not_వరుస, np->అంశం);
					np->అంశం->object.flags |= PARENT2;
				}
			}

			ఒకవేళ (commit_లేఖాచిత్రం_generation(n) < commit_లేఖాచిత్రం_generation(c))
				ఆపు;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		కొరకు (p = c->parents, మాతృ_i = 0; p; p = p->తదుపరి, మాతృ_i++) {
			process_మాతృ(lm, &వరుస,
				       c, చురుకైన_c,
				       p->అంశం, మాతృ_i);

			ఒకవేళ (bitmap_is_ఖాళీలు(చురుకైన_c))
				ఆపు;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		ఒకవేళ (!bitmap_is_ఖాళీలు(చురుకైన_c))  {
			దత్తాంశం.commit = c;
			కొరకు (కొలత_t i = 0; i < lm->all_paths_nr; i++) {
				ఒకవేళ (bitmap_get(చురుకైన_c, i))
					mark_మార్గం(lm->all_paths[i], ఖాళీ, &దత్తాంశం);
			}
		}

cleanup:
		చురుకైన_paths_free(lm, c);
	}

	ఒకవేళ (hashmap_get_కొలత(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_వరుస(&not_వరుస);
	clear_prio_వరుస(&వరుస);
	clear_చురుకైన_paths_for_commit(&lm->చురుకైన_paths);
	bitmap_free(lm->scratch);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం చివరి_modified_init(నిర్మాణం చివరి_modified *lm, నిర్మాణం repository *r,
			      స్థిరరాశి అక్షరం *ఉపసర్గ, పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	నిర్మాణం hashmap_iter iter;
	నిర్మాణం చివరి_modified_నమోదు *ent;

	hashmap_init(&lm->paths, చివరి_modified_నమోదు_hashcmp, ఖాళీ, 0);

	repo_init_revisions(r, &lm->rev, ఉపసర్గ);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_మూలం_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_వృక్షం_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.వృక్షం_లోపల_recursive = lm->show_trees;
	lm->rev.diffopt.max_లోతు = lm->max_లోతు;
	lm->rev.diffopt.max_లోతు_valid = lm->max_లోతు >= 0;

	argc = అమరిక_revisions(argc, argv, &lm->rev, ఖాళీ);
	ఒకవేళ (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		తిరిగి argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	ఒకవేళ (populate_paths_from_revs(lm) < 0)
		తిరిగి -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_కొలత(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_నమోదు(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->మార్గం;
	}

	తిరిగి 0;
}

పూర్ణాంకం cmd_చివరి_modified(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		      నిర్మాణం repository *repo)
{
	పూర్ణాంకం ret;
	నిర్మాణం చివరి_modified lm = { 0 };

	స్థిరరాశి అక్షరం * స్థిరరాశి చివరి_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		ఖాళీ
	};

	నిర్మాణం option చివరి_modified_options[] = {
		OPT_సమితి_INT('r', "recursive", &lm.max_లోతు,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_లోతు,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', ఖాళీ, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, చివరి_modified_options,
			     చివరి_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, ఖాళీ);

	ret = చివరి_modified_init(&lm, repo, ఉపసర్గ, argc, argv);
	ఒకవేళ (ret > 0)
		usage_with_options(చివరి_modified_usage,
				   చివరి_modified_options);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;

	ret = చివరి_modified_నిరంతరం(&lm);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;

బహిర్గతం:
	చివరి_modified_release(&lm);

	తిరిగి ret;
}
