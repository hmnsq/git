#ಅನ್ತರ್ಭಾವ "git-compat-util.h"
#ಅನ್ತರ್ಭಾವ "bloom.h"
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "commit-graph.h"
#ಅನ್ತರ್ಭಾವ "commit-slab.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "diffcore.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "ewah/ewok.h"
#ಅನ್ತರ್ಭಾವ "hashmap.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "prio-queue.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "repository.h"
#ಅನ್ತರ್ಭಾವ "revision.h"

/* Remember to update object flag allocation in object.h */
#ವ್ಯಾಖ್ಯಾನಿಸು PARENT1 (1u<<16) /* used instead of SEEN */
#ವ್ಯಾಖ್ಯಾನಿಸು PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು {
	ರಚನೆ hashmap_ನಮೂದು hashent;
	ರಚನೆ object_id oid;
	ರಚನೆ bloom_ಕೀಲಿ ಕೀಲಿ;
	ಗಾತ್ರ_t diff_idx;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಮಾರ್ಗ[FLEX_ARRAY];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_modified_ನಮೂದು_hashcmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಬಳಸದ_2 ಬಳಸದ,
				       ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *hent1,
				       ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *hent2,
				       ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಮಾರ್ಗ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು *ent1 =
		container_ದ(hent1, ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು, hashent);
	ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು *ent2 =
		container_ದ(hent2, ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು, hashent);
	ಹಿಂತಿರುಗಿ strcmp(ent1->ಮಾರ್ಗ, ಮಾರ್ಗ ? ಮಾರ್ಗ : ent2->ಮಾರ್ಗ);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(ಸಕ್ರಿಯ_paths_for_commit, ರಚನೆ bitmap *);

ರಚನೆ ಕೊನೆಯ_modified {
	ರಚನೆ hashmap paths;
	ರಚನೆ rev_info rev;
	bool show_trees;
	bool nul_termination;
	ಪೂರ್ಣಾಂಕ max_ಆಳ;

	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **all_paths;
	ಗಾತ್ರ_t all_paths_nr;
	ರಚನೆ ಸಕ್ರಿಯ_paths_for_commit ಸಕ್ರಿಯ_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	ರಚನೆ bitmap *scratch;
};

ಸ್ಥಿರ ರಚನೆ bitmap *ಸಕ್ರಿಯ_paths_for(ರಚನೆ ಕೊನೆಯ_modified *lm, ರಚನೆ commit *c)
{
	ರಚನೆ bitmap **bitmap = ಸಕ್ರಿಯ_paths_for_commit_at(&lm->ಸಕ್ರಿಯ_paths, c);
	ಆದರೆ (!*bitmap)
		*bitmap = bitmap_ಶಬ್ದ_alloc(lm->all_paths_nr / ಬಿಟ್ಟುಗಳು_ಒಳ_EWORD + 1);

	ಹಿಂತಿರುಗಿ *bitmap;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸಕ್ರಿಯ_paths_free(ರಚನೆ ಕೊನೆಯ_modified *lm, ರಚನೆ commit *c)
{
	ರಚನೆ bitmap **bitmap = ಸಕ್ರಿಯ_paths_for_commit_at(&lm->ಸಕ್ರಿಯ_paths, c);
	ಆದರೆ (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = ಖಾಲಿ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೊನೆಯ_modified_release(ರಚನೆ ಕೊನೆಯ_modified *lm)
{
	ರಚನೆ hashmap_iter iter;
	ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು *ent;

	hashmap_for_each_ನಮೂದು(&lm->paths, &iter, ent, hashent)
		bloom_ಕೀಲಿ_clear(&ent->ಕೀಲಿ);

	hashmap_clear_and_free(&lm->paths, ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು, hashent);
	release_revisions(&lm->rev);

	ಬಿಡುಗಡೆ(lm->all_paths);
}

ರಚನೆ ಕೊನೆಯ_modified_callback_ದತ್ತಾಂಶ {
	ರಚನೆ ಕೊನೆಯ_modified *lm;
	ರಚನೆ commit *commit;
};

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ಮಾರ್ಗ_from_diff(ರಚನೆ diff_ಸರತಿ_struct *q,
			       ರಚನೆ diff_options *opt ಬಳಸದ, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ ಕೊನೆಯ_modified *lm = ದತ್ತಾಂಶ;

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < q->nr; i++) {
		ರಚನೆ diff_filepair *p = q->ಸರತಿ[i];
		ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು *ent;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = p->two->ಮಾರ್ಗ;

		FLEX_ALLOC_STR(ent, ಮಾರ್ಗ, ಮಾರ್ಗ);
		oidcpy(&ent->oid, &p->two->oid);
		ಆದರೆ (lm->rev.bloom_filter_settings)
			bloom_ಕೀಲಿ_fill(&ent->ಕೀಲಿ, ಮಾರ್ಗ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ),
				       lm->rev.bloom_filter_settings);
		hashmap_ನಮೂದು_init(&ent->hashent, strhash(ent->ಮಾರ್ಗ));
		hashmap_ಕೂಡಿಸು(&lm->paths, &ent->hashent);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ populate_paths_from_revs(ರಚನೆ ಕೊನೆಯ_modified *lm)
{
	ಪೂರ್ಣಾಂಕ ಅಂಕೆ_interesting = 0, ret = 0;
	ರಚನೆ diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	ಸ್ಮರಣೆ_ನಕಲು(&diffopt, &lm->rev.diffopt, ಗಾತ್ರ(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = ಕೂಡಿಸು_ಮಾರ್ಗ_from_diff;
	diffopt.format_callback_ದತ್ತಾಂಶ = lm;

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < lm->rev.pending.nr; i++) {
		ರಚನೆ object_array_ನಮೂದು *obj = lm->rev.pending.objects + i;

		ಆದರೆ (obj->ಅಂಶ->flags & UNINTERESTING)
			ಮುಂದುವರಿ;

		ಆದರೆ (ಅಂಕೆ_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			ಹೋಗು ಹೊರಹೋಗು;
		}

		ಆದರೆ (!repo_peel_ಗಮ್ಯ_type(lm->rev.repo, obj->ಮಾರ್ಗ, 0, obj->ಅಂಶ, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->ಅಂಶ->type));
			ಹೋಗು ಹೊರಹೋಗು;
		}

		diff_ವೃಕ್ಷ_oid(lm->rev.repo->hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ,
			      &obj->ಅಂಶ->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

ಹೊರಹೋಗು:
	clear_pathspec(&diffopt.pathspec);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೊನೆಯ_modified_emit(ರಚನೆ ಕೊನೆಯ_modified *lm,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ರಚನೆ commit *commit)

{
	ಆದರೆ (commit->object.flags & BOUNDARY)
		ಅಕ್ಷರ_ಬರೆ('^');
	ಬರೆ("%s\t", oid_ಗಮ್ಯ_hex(&commit->object.oid));

	ಆದರೆ (lm->nul_termination)
		ಬರೆ("%s%c", ಮಾರ್ಗ, '\0');
	ಇಲ್ಲದಿದ್ದರೆ
		write_name_quoted(ಮಾರ್ಗ, stdout, '\n');
}

ಸ್ಥಿರ ಶೂನ್ಯ mark_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
		      ರಚನೆ ಕೊನೆಯ_modified_callback_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_ನಮೂದು_from_hash(&ದತ್ತಾಂಶ->lm->paths, strhash(ಮಾರ್ಗ), ಮಾರ್ಗ,
					  ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು, hashent);
	ಆದರೆ (!ent)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	ಆದರೆ (oid && !oideq(oid, &ent->oid))
		ಹಿಂತಿರುಗಿ;

	ಕೊನೆಯ_modified_emit(ದತ್ತಾಂಶ->lm, ಮಾರ್ಗ, ದತ್ತಾಂಶ->commit);

	hashmap_remove(&ದತ್ತಾಂಶ->lm->paths, &ent->hashent, ಮಾರ್ಗ);
	bloom_ಕೀಲಿ_clear(&ent->ಕೀಲಿ);
	ಬಿಡುಗಡೆ(ent);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೊನೆಯ_modified_diff(ರಚನೆ diff_ಸರತಿ_struct *q,
			       ರಚನೆ diff_options *opt ಬಳಸದ, ಶೂನ್ಯ *cbdata)
{
	ರಚನೆ ಕೊನೆಯ_modified_callback_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cbdata;

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < q->nr; i++) {
		ರಚನೆ diff_filepair *p = q->ಸರತಿ[i];
		ಆಯ್ಕೆ (p->ಸ್ಥಿತಿ_2) {
		ಸ್ಥಿತಿ DIFF_ಸ್ಥಿತಿ_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			ನಿಲ್ಲಿಸು;

		ಪೂರ್ವನಿಯೋಜಿತ:
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
			mark_ಮಾರ್ಗ(p->two->ಮಾರ್ಗ, &p->two->oid, ದತ್ತಾಂಶ);
			ನಿಲ್ಲಿಸು;
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ pass_ಗಮ್ಯ_ಪೋಷಕ(ರಚನೆ bitmap *c,
			   ರಚನೆ bitmap *p,
			   ಗಾತ್ರ_t pos)
{
	bitmap_unset(c, pos);
	bitmap_ಗಣ(p, pos);
}

ಸ್ಥಿರ bool maybe_ಬದಲಾಯಿತು_ಮಾರ್ಗ(ರಚನೆ ಕೊನೆಯ_modified *lm,
			       ರಚನೆ commit *origin,
			       ರಚನೆ bitmap *ಸಕ್ರಿಯ)
{
	ರಚನೆ bloom_filter *filter;
	ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು *ent;
	ರಚನೆ hashmap_iter iter;

	ಆದರೆ (!lm->rev.bloom_filter_settings)
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (commit_ನಕ್ಷೆ_generation(origin) == GENERATION_ಸಂಖ್ಯೆ_INFINITY)
		ಹಿಂತಿರುಗಿ true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	ಆದರೆ (!filter)
		ಹಿಂತಿರುಗಿ true;

	hashmap_for_each_ನಮೂದು(&lm->paths, &iter, ent, hashent) {
		ಆದರೆ (ಸಕ್ರಿಯ && !bitmap_get(ಸಕ್ರಿಯ, ent->diff_idx))
			ಮುಂದುವರಿ;

		ಆದರೆ (bloom_filter_ಒಳಗೊಂಡಿದೆ(filter, &ent->ಕೀಲಿ,
					  lm->rev.bloom_filter_settings))
			ಹಿಂತಿರುಗಿ true;
	}
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ process_ಪೋಷಕ(ರಚನೆ ಕೊನೆಯ_modified *lm,
			   ರಚನೆ prio_ಸರತಿ *ಸರತಿ,
			   ರಚನೆ commit *c, ರಚನೆ bitmap *ಸಕ್ರಿಯ_c,
			   ರಚನೆ commit *ಪೋಷಕ, ಪೂರ್ಣಾಂಕ ಪೋಷಕ_i)
{
	ರಚನೆ bitmap *ಸಕ್ರಿಯ_p;

	ಆದರೆ (repo_parse_commit(lm->rev.repo, ಪೋಷಕ))
		ಹಿಂತಿರುಗಿ;
	ಸಕ್ರಿಯ_p = ಸಕ್ರಿಯ_paths_for(lm, ಪೋಷಕ);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	ಆದರೆ (ಪೋಷಕ_i || maybe_ಬದಲಾಯಿತು_ಮಾರ್ಗ(lm, c, ಸಕ್ರಿಯ_c)) {
		diff_ವೃಕ್ಷ_oid(&ಪೋಷಕ->object.oid,
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
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < diff_queued_diff.nr; i++) {
		ರಚನೆ diff_filepair *fp = diff_queued_diff.ಸರತಿ[i];
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = fp->two->ಮಾರ್ಗ;
		ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು *ent =
			hashmap_get_ನಮೂದು_from_hash(&lm->paths, strhash(ಮಾರ್ಗ), ಮಾರ್ಗ,
						    ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು, hashent);
		ಆದರೆ (ent) {
			ಗಾತ್ರ_t k = ent->diff_idx;
			ಆದರೆ (bitmap_get(ಸಕ್ರಿಯ_c, k))
				bitmap_ಗಣ(lm->scratch, k);
		}
	}
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < lm->all_paths_nr; i++) {
		ಆದರೆ (bitmap_get(ಸಕ್ರಿಯ_c, i) && !bitmap_get(lm->scratch, i))
			pass_ಗಮ್ಯ_ಪೋಷಕ(ಸಕ್ರಿಯ_c, ಸಕ್ರಿಯ_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	ಆದರೆ (!bitmap_is_ಖಾಲಿಗಳು(ಸಕ್ರಿಯ_p) && !(ಪೋಷಕ->object.flags & PARENT1)) {
		ಪೋಷಕ->object.flags |= PARENT1;
		prio_ಸರತಿ_put(ಸರತಿ, ಪೋಷಕ);
	}
	ಆದರೆ (!(ಪೋಷಕ->object.flags & PARENT1))
		ಸಕ್ರಿಯ_paths_free(lm, ಪೋಷಕ);

	MEMZERO_ARRAY(lm->scratch->ಪದಗಳು, lm->scratch->ಶಬ್ದ_alloc);
	diff_ಸರತಿ_clear(&diff_queued_diff);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_modified_ಸತತ(ರಚನೆ ಕೊನೆಯ_modified *lm)
{
	ಪೂರ್ಣಾಂಕ max_ಎಣಿಕೆ, ಸರತಿ_popped = 0;
	ರಚನೆ commit *c, *n;
	ರಚನೆ prio_ಸರತಿ ಸರತಿ = { compare_commits_by_gen_then_commit_date };
	ರಚನೆ prio_ಸರತಿ not_ಸರತಿ = { compare_commits_by_gen_then_commit_date };
	ರಚನೆ commit_list *list;
	ರಚನೆ ಕೊನೆಯ_modified_callback_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = ಕೊನೆಯ_modified_diff;
	lm->rev.diffopt.format_callback_ದತ್ತಾಂಶ = &ದತ್ತಾಂಶ;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_ಎಣಿಕೆ = lm->rev.max_ಎಣಿಕೆ;

	init_ಸಕ್ರಿಯ_paths_for_commit(&lm->ಸಕ್ರಿಯ_paths);
	lm->scratch = bitmap_ಶಬ್ದ_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	ಗಾಗಿ (list = lm->rev.commits; list; list = list->ಮುಂದಿನ) {
		ರಚನೆ commit *c = list->ಅಂಶ;

		ಆದರೆ (c->object.flags & BOTTOM) {
			prio_ಸರತಿ_put(&not_ಸರತಿ, c);
			c->object.flags |= PARENT2;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			ರಚನೆ bitmap *ಸಕ್ರಿಯ;

			prio_ಸರತಿ_put(&ಸರತಿ, c);
			c->object.flags |= PARENT1;

			ಸಕ್ರಿಯ = ಸಕ್ರಿಯ_paths_for(lm, c);
			ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_ಗಣ(ಸಕ್ರಿಯ, i);
		}
	}

	ಆಗ ((c = prio_ಸರತಿ_get(&ಸರತಿ))) {
		ಪೂರ್ಣಾಂಕ ಪೋಷಕ_i;
		ರಚನೆ commit_list *p;
		ರಚನೆ bitmap *ಸಕ್ರಿಯ_c = ಸಕ್ರಿಯ_paths_for(lm, c);

		ಆದರೆ ((0 <= max_ಎಣಿಕೆ && max_ಎಣಿಕೆ < ++ಸರತಿ_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			ದತ್ತಾಂಶ.commit = c;
			diff_ವೃಕ್ಷ_oid(lm->rev.repo->hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			ಹೋಗು cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		ಆದರೆ (repo_parse_commit(lm->rev.repo, c))
			ಹೋಗು cleanup;

		ಆಗ ((n = prio_ಸರತಿ_get(&not_ಸರತಿ))) {
			ರಚನೆ commit_list *np;

			ಆದರೆ (repo_parse_commit(lm->rev.repo, n))
				ಮುಂದುವರಿ;

			ಗಾಗಿ (np = n->parents; np; np = np->ಮುಂದಿನ) {
				ಆದರೆ (!(np->ಅಂಶ->object.flags & PARENT2)) {
					prio_ಸರತಿ_put(&not_ಸರತಿ, np->ಅಂಶ);
					np->ಅಂಶ->object.flags |= PARENT2;
				}
			}

			ಆದರೆ (commit_ನಕ್ಷೆ_generation(n) < commit_ನಕ್ಷೆ_generation(c))
				ನಿಲ್ಲಿಸು;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		ಗಾಗಿ (p = c->parents, ಪೋಷಕ_i = 0; p; p = p->ಮುಂದಿನ, ಪೋಷಕ_i++) {
			process_ಪೋಷಕ(lm, &ಸರತಿ,
				       c, ಸಕ್ರಿಯ_c,
				       p->ಅಂಶ, ಪೋಷಕ_i);

			ಆದರೆ (bitmap_is_ಖಾಲಿಗಳು(ಸಕ್ರಿಯ_c))
				ನಿಲ್ಲಿಸು;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		ಆದರೆ (!bitmap_is_ಖಾಲಿಗಳು(ಸಕ್ರಿಯ_c))  {
			ದತ್ತಾಂಶ.commit = c;
			ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < lm->all_paths_nr; i++) {
				ಆದರೆ (bitmap_get(ಸಕ್ರಿಯ_c, i))
					mark_ಮಾರ್ಗ(lm->all_paths[i], ಖಾಲಿ, &ದತ್ತಾಂಶ);
			}
		}

cleanup:
		ಸಕ್ರಿಯ_paths_free(lm, c);
	}

	ಆದರೆ (hashmap_get_ಗಾತ್ರ(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_ಸರತಿ(&not_ಸರತಿ);
	clear_prio_ಸರತಿ(&ಸರತಿ);
	clear_ಸಕ್ರಿಯ_paths_for_commit(&lm->ಸಕ್ರಿಯ_paths);
	bitmap_free(lm->scratch);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_modified_init(ರಚನೆ ಕೊನೆಯ_modified *lm, ರಚನೆ repository *r,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ರಚನೆ hashmap_iter iter;
	ರಚನೆ ಕೊನೆಯ_modified_ನಮೂದು *ent;

	hashmap_init(&lm->paths, ಕೊನೆಯ_modified_ನಮೂದು_hashcmp, ಖಾಲಿ, 0);

	repo_init_revisions(r, &lm->rev, ಪೂರ್ವಪ್ರತ್ಯಯ);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_ಬೇರು_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_ವೃಕ್ಷ_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.ವೃಕ್ಷ_ಒಳ_recursive = lm->show_trees;
	lm->rev.diffopt.max_ಆಳ = lm->max_ಆಳ;
	lm->rev.diffopt.max_ಆಳ_valid = lm->max_ಆಳ >= 0;

	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, &lm->rev, ಖಾಲಿ);
	ಆದರೆ (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		ಹಿಂತಿರುಗಿ argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	ಆದರೆ (populate_paths_from_revs(lm) < 0)
		ಹಿಂತಿರುಗಿ -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_ಗಾತ್ರ(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_ನಮೂದು(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->ಮಾರ್ಗ;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_ಕೊನೆಯ_modified(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		      ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ ಕೊನೆಯ_modified lm = { 0 };

	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ಕೊನೆಯ_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		ಖಾಲಿ
	};

	ರಚನೆ option ಕೊನೆಯ_modified_options[] = {
		OPT_ಗಣ_INT('r', "recursive", &lm.max_ಆಳ,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_ಆಳ,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', ಖಾಲಿ, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, ಕೊನೆಯ_modified_options,
			     ಕೊನೆಯ_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, ಖಾಲಿ);

	ret = ಕೊನೆಯ_modified_init(&lm, repo, ಪೂರ್ವಪ್ರತ್ಯಯ, argc, argv);
	ಆದರೆ (ret > 0)
		usage_with_options(ಕೊನೆಯ_modified_usage,
				   ಕೊನೆಯ_modified_options);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;

	ret = ಕೊನೆಯ_modified_ಸತತ(&lm);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;

ಹೊರಹೋಗು:
	ಕೊನೆಯ_modified_release(&lm);

	ಹಿಂತಿರುಗಿ ret;
}
