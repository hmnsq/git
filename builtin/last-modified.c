#ਅਨ੍ਤਰ੍ਭਾਵ "git-compat-util.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bloom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-graph.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-slab.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diffcore.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ewah/ewok.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hashmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "prio-queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repository.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"

/* Remember to update object flag allocation in object.h */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PARENT1 (1u<<16) /* used instead of SEEN */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ {
	ਬਣਤਰ hashmap_ਇੰਦਰਾਜ hashent;
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ bloom_ਕੁੰਜੀ ਕੁੰਜੀ;
	ਮਾਪ_t diff_idx;
	ਅਚਲ ਅੱਖਰ ਰਸਤਾ[FLEX_ARRAY];
};

ਸਥਿਰ ਪੂਰਨ ਆਖਰੀ_modified_ਇੰਦਰਾਜ_hashcmp(ਅਚਲ ਖਾਲੀ *ਅਣਵਰਤਿਆ_2 ਅਣਵਰਤਿਆ,
				       ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *hent1,
				       ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *hent2,
				       ਅਚਲ ਖਾਲੀ *ਰਸਤਾ)
{
	ਅਚਲ ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ *ent1 =
		container_ਦਾ(hent1, ਅਚਲ ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ, hashent);
	ਅਚਲ ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ *ent2 =
		container_ਦਾ(hent2, ਅਚਲ ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ, hashent);
	ਵਾਪਸ strcmp(ent1->ਰਸਤਾ, ਰਸਤਾ ? ਰਸਤਾ : ent2->ਰਸਤਾ);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(ਸਰਗਰਮ_paths_for_commit, ਬਣਤਰ bitmap *);

ਬਣਤਰ ਆਖਰੀ_modified {
	ਬਣਤਰ hashmap paths;
	ਬਣਤਰ rev_info rev;
	bool show_trees;
	bool nul_termination;
	ਪੂਰਨ max_ਡੂੰਘਾਈ;

	ਅਚਲ ਅੱਖਰ **all_paths;
	ਮਾਪ_t all_paths_nr;
	ਬਣਤਰ ਸਰਗਰਮ_paths_for_commit ਸਰਗਰਮ_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	ਬਣਤਰ bitmap *scratch;
};

ਸਥਿਰ ਬਣਤਰ bitmap *ਸਰਗਰਮ_paths_for(ਬਣਤਰ ਆਖਰੀ_modified *lm, ਬਣਤਰ commit *c)
{
	ਬਣਤਰ bitmap **bitmap = ਸਰਗਰਮ_paths_for_commit_at(&lm->ਸਰਗਰਮ_paths, c);
	ਜੇ (!*bitmap)
		*bitmap = bitmap_ਸ਼ਬਦ_alloc(lm->all_paths_nr / ਬਿੱਟ_ਵਿੱਚ_EWORD + 1);

	ਵਾਪਸ *bitmap;
}

ਸਥਿਰ ਖਾਲੀ ਸਰਗਰਮ_paths_free(ਬਣਤਰ ਆਖਰੀ_modified *lm, ਬਣਤਰ commit *c)
{
	ਬਣਤਰ bitmap **bitmap = ਸਰਗਰਮ_paths_for_commit_at(&lm->ਸਰਗਰਮ_paths, c);
	ਜੇ (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = ਨਿਰਾ;
	}
}

ਸਥਿਰ ਖਾਲੀ ਆਖਰੀ_modified_release(ਬਣਤਰ ਆਖਰੀ_modified *lm)
{
	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ *ent;

	hashmap_for_each_ਇੰਦਰਾਜ(&lm->paths, &iter, ent, hashent)
		bloom_ਕੁੰਜੀ_clear(&ent->ਕੁੰਜੀ);

	hashmap_clear_and_free(&lm->paths, ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ, hashent);
	release_revisions(&lm->rev);

	ਮੁਕਤ(lm->all_paths);
}

ਬਣਤਰ ਆਖਰੀ_modified_callback_ਅੰਕੜੇ {
	ਬਣਤਰ ਆਖਰੀ_modified *lm;
	ਬਣਤਰ commit *commit;
};

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਰਸਤਾ_from_diff(ਬਣਤਰ diff_ਕਤਾਰ_struct *q,
			       ਬਣਤਰ diff_options *opt ਅਣਵਰਤਿਆ, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ ਆਖਰੀ_modified *lm = ਅੰਕੜੇ;

	ਲਈ (ਪੂਰਨ i = 0; i < q->nr; i++) {
		ਬਣਤਰ diff_filepair *p = q->ਕਤਾਰ[i];
		ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ *ent;
		ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = p->two->ਰਸਤਾ;

		FLEX_ALLOC_STR(ent, ਰਸਤਾ, ਰਸਤਾ);
		oidcpy(&ent->oid, &p->two->oid);
		ਜੇ (lm->rev.bloom_filter_settings)
			bloom_ਕੁੰਜੀ_fill(&ent->ਕੁੰਜੀ, ਰਸਤਾ, ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ),
				       lm->rev.bloom_filter_settings);
		hashmap_ਇੰਦਰਾਜ_init(&ent->hashent, strhash(ent->ਰਸਤਾ));
		hashmap_ਜੋੜੋ(&lm->paths, &ent->hashent);
	}
}

ਸਥਿਰ ਪੂਰਨ populate_paths_from_revs(ਬਣਤਰ ਆਖਰੀ_modified *lm)
{
	ਪੂਰਨ ਅੰਕ_interesting = 0, ret = 0;
	ਬਣਤਰ diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&diffopt, &lm->rev.diffopt, ਆਕਾਰ(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = ਜੋੜੋ_ਰਸਤਾ_from_diff;
	diffopt.format_callback_ਅੰਕੜੇ = lm;

	ਲਈ (ਮਾਪ_t i = 0; i < lm->rev.pending.nr; i++) {
		ਬਣਤਰ object_array_ਇੰਦਰਾਜ *obj = lm->rev.pending.objects + i;

		ਜੇ (obj->ਵਸਤੂ->flags & UNINTERESTING)
			ਜਾਰੀ;

		ਜੇ (ਅੰਕ_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਜੇ (!repo_peel_ਮੰਜ਼ਿਲ_type(lm->rev.repo, obj->ਰਸਤਾ, 0, obj->ਵਸਤੂ, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->ਵਸਤੂ->type));
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		diff_ਰੁੱਖ_oid(lm->rev.repo->hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ,
			      &obj->ਵਸਤੂ->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

ਬਾਹਰਲਾ:
	clear_pathspec(&diffopt.pathspec);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ ਆਖਰੀ_modified_emit(ਬਣਤਰ ਆਖਰੀ_modified *lm,
			       ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਬਣਤਰ commit *commit)

{
	ਜੇ (commit->object.flags & BOUNDARY)
		ਅੱਖਰ_ਲਿਖੋ('^');
	ਲਿਖੋ("%s\t", oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));

	ਜੇ (lm->nul_termination)
		ਲਿਖੋ("%s%c", ਰਸਤਾ, '\0');
	ਨਹੀਂਤਾਂ
		write_name_quoted(ਰਸਤਾ, stdout, '\n');
}

ਸਥਿਰ ਖਾਲੀ mark_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਬਣਤਰ object_id *oid,
		      ਬਣਤਰ ਆਖਰੀ_modified_callback_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_ਇੰਦਰਾਜ_from_hash(&ਅੰਕੜੇ->lm->paths, strhash(ਰਸਤਾ), ਰਸਤਾ,
					  ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ, hashent);
	ਜੇ (!ent)
		ਵਾਪਸ;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	ਜੇ (oid && !oideq(oid, &ent->oid))
		ਵਾਪਸ;

	ਆਖਰੀ_modified_emit(ਅੰਕੜੇ->lm, ਰਸਤਾ, ਅੰਕੜੇ->commit);

	hashmap_remove(&ਅੰਕੜੇ->lm->paths, &ent->hashent, ਰਸਤਾ);
	bloom_ਕੁੰਜੀ_clear(&ent->ਕੁੰਜੀ);
	ਮੁਕਤ(ent);
}

ਸਥਿਰ ਖਾਲੀ ਆਖਰੀ_modified_diff(ਬਣਤਰ diff_ਕਤਾਰ_struct *q,
			       ਬਣਤਰ diff_options *opt ਅਣਵਰਤਿਆ, ਖਾਲੀ *cbdata)
{
	ਬਣਤਰ ਆਖਰੀ_modified_callback_ਅੰਕੜੇ *ਅੰਕੜੇ = cbdata;

	ਲਈ (ਪੂਰਨ i = 0; i < q->nr; i++) {
		ਬਣਤਰ diff_filepair *p = q->ਕਤਾਰ[i];
		ਚੋਣ (p->ਸਥਿਤੀ) {
		ਹਾਲਤ DIFF_ਸਥਿਤੀ_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			ਰੋਕੋ;

		ਮੂਲ:
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
			mark_ਰਸਤਾ(p->two->ਰਸਤਾ, &p->two->oid, ਅੰਕੜੇ);
			ਰੋਕੋ;
		}
	}
}

ਸਥਿਰ ਖਾਲੀ pass_ਮੰਜ਼ਿਲ_ਮਾਪਾ(ਬਣਤਰ bitmap *c,
			   ਬਣਤਰ bitmap *p,
			   ਮਾਪ_t pos)
{
	bitmap_unset(c, pos);
	bitmap_ਸੈੱਟ(p, pos);
}

ਸਥਿਰ bool maybe_ਬਦਲਿਆ_ਰਸਤਾ(ਬਣਤਰ ਆਖਰੀ_modified *lm,
			       ਬਣਤਰ commit *origin,
			       ਬਣਤਰ bitmap *ਸਰਗਰਮ)
{
	ਬਣਤਰ bloom_filter *filter;
	ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ *ent;
	ਬਣਤਰ hashmap_iter iter;

	ਜੇ (!lm->rev.bloom_filter_settings)
		ਵਾਪਸ true;

	ਜੇ (commit_ਗ੍ਰਾਫ_generation(origin) == GENERATION_ਸੰਖਿਆ_INFINITY)
		ਵਾਪਸ true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	ਜੇ (!filter)
		ਵਾਪਸ true;

	hashmap_for_each_ਇੰਦਰਾਜ(&lm->paths, &iter, ent, hashent) {
		ਜੇ (ਸਰਗਰਮ && !bitmap_get(ਸਰਗਰਮ, ent->diff_idx))
			ਜਾਰੀ;

		ਜੇ (bloom_filter_ਸ਼ਾਮਲ_ਹੈ(filter, &ent->ਕੁੰਜੀ,
					  lm->rev.bloom_filter_settings))
			ਵਾਪਸ true;
	}
	ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ process_ਮਾਪਾ(ਬਣਤਰ ਆਖਰੀ_modified *lm,
			   ਬਣਤਰ prio_ਕਤਾਰ *ਕਤਾਰ,
			   ਬਣਤਰ commit *c, ਬਣਤਰ bitmap *ਸਰਗਰਮ_c,
			   ਬਣਤਰ commit *ਮਾਪਾ, ਪੂਰਨ ਮਾਪਾ_i)
{
	ਬਣਤਰ bitmap *ਸਰਗਰਮ_p;

	ਜੇ (repo_parse_commit(lm->rev.repo, ਮਾਪਾ))
		ਵਾਪਸ;
	ਸਰਗਰਮ_p = ਸਰਗਰਮ_paths_for(lm, ਮਾਪਾ);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	ਜੇ (ਮਾਪਾ_i || maybe_ਬਦਲਿਆ_ਰਸਤਾ(lm, c, ਸਰਗਰਮ_c)) {
		diff_ਰੁੱਖ_oid(&ਮਾਪਾ->object.oid,
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
	ਲਈ (ਪੂਰਨ i = 0; i < diff_queued_diff.nr; i++) {
		ਬਣਤਰ diff_filepair *fp = diff_queued_diff.ਕਤਾਰ[i];
		ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = fp->two->ਰਸਤਾ;
		ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ *ent =
			hashmap_get_ਇੰਦਰਾਜ_from_hash(&lm->paths, strhash(ਰਸਤਾ), ਰਸਤਾ,
						    ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ, hashent);
		ਜੇ (ent) {
			ਮਾਪ_t k = ent->diff_idx;
			ਜੇ (bitmap_get(ਸਰਗਰਮ_c, k))
				bitmap_ਸੈੱਟ(lm->scratch, k);
		}
	}
	ਲਈ (ਮਾਪ_t i = 0; i < lm->all_paths_nr; i++) {
		ਜੇ (bitmap_get(ਸਰਗਰਮ_c, i) && !bitmap_get(lm->scratch, i))
			pass_ਮੰਜ਼ਿਲ_ਮਾਪਾ(ਸਰਗਰਮ_c, ਸਰਗਰਮ_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	ਜੇ (!bitmap_is_ਖਾਲੀ_ਥਾਵਾਂ(ਸਰਗਰਮ_p) && !(ਮਾਪਾ->object.flags & PARENT1)) {
		ਮਾਪਾ->object.flags |= PARENT1;
		prio_ਕਤਾਰ_put(ਕਤਾਰ, ਮਾਪਾ);
	}
	ਜੇ (!(ਮਾਪਾ->object.flags & PARENT1))
		ਸਰਗਰਮ_paths_free(lm, ਮਾਪਾ);

	MEMZERO_ARRAY(lm->scratch->ਸ਼ਬਦ, lm->scratch->ਸ਼ਬਦ_alloc);
	diff_ਕਤਾਰ_clear(&diff_queued_diff);
}

ਸਥਿਰ ਪੂਰਨ ਆਖਰੀ_modified_ਲੜੀਵਾਰ(ਬਣਤਰ ਆਖਰੀ_modified *lm)
{
	ਪੂਰਨ max_ਗਿਣਤੀ, ਕਤਾਰ_popped = 0;
	ਬਣਤਰ commit *c, *n;
	ਬਣਤਰ prio_ਕਤਾਰ ਕਤਾਰ = { compare_commits_by_gen_then_commit_date };
	ਬਣਤਰ prio_ਕਤਾਰ not_ਕਤਾਰ = { compare_commits_by_gen_then_commit_date };
	ਬਣਤਰ commit_list *list;
	ਬਣਤਰ ਆਖਰੀ_modified_callback_ਅੰਕੜੇ ਅੰਕੜੇ = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = ਆਖਰੀ_modified_diff;
	lm->rev.diffopt.format_callback_ਅੰਕੜੇ = &ਅੰਕੜੇ;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_ਗਿਣਤੀ = lm->rev.max_ਗਿਣਤੀ;

	init_ਸਰਗਰਮ_paths_for_commit(&lm->ਸਰਗਰਮ_paths);
	lm->scratch = bitmap_ਸ਼ਬਦ_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	ਲਈ (list = lm->rev.commits; list; list = list->ਅਗਲਾ) {
		ਬਣਤਰ commit *c = list->ਵਸਤੂ;

		ਜੇ (c->object.flags & BOTTOM) {
			prio_ਕਤਾਰ_put(&not_ਕਤਾਰ, c);
			c->object.flags |= PARENT2;
		} ਨਹੀਂਤਾਂ ਜੇ (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			ਬਣਤਰ bitmap *ਸਰਗਰਮ;

			prio_ਕਤਾਰ_put(&ਕਤਾਰ, c);
			c->object.flags |= PARENT1;

			ਸਰਗਰਮ = ਸਰਗਰਮ_paths_for(lm, c);
			ਲਈ (ਮਾਪ_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_ਸੈੱਟ(ਸਰਗਰਮ, i);
		}
	}

	ਜਦੋਂ ((c = prio_ਕਤਾਰ_get(&ਕਤਾਰ))) {
		ਪੂਰਨ ਮਾਪਾ_i;
		ਬਣਤਰ commit_list *p;
		ਬਣਤਰ bitmap *ਸਰਗਰਮ_c = ਸਰਗਰਮ_paths_for(lm, c);

		ਜੇ ((0 <= max_ਗਿਣਤੀ && max_ਗਿਣਤੀ < ++ਕਤਾਰ_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			ਅੰਕੜੇ.commit = c;
			diff_ਰੁੱਖ_oid(lm->rev.repo->hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			ਜਾਓ cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		ਜੇ (repo_parse_commit(lm->rev.repo, c))
			ਜਾਓ cleanup;

		ਜਦੋਂ ((n = prio_ਕਤਾਰ_get(&not_ਕਤਾਰ))) {
			ਬਣਤਰ commit_list *np;

			ਜੇ (repo_parse_commit(lm->rev.repo, n))
				ਜਾਰੀ;

			ਲਈ (np = n->parents; np; np = np->ਅਗਲਾ) {
				ਜੇ (!(np->ਵਸਤੂ->object.flags & PARENT2)) {
					prio_ਕਤਾਰ_put(&not_ਕਤਾਰ, np->ਵਸਤੂ);
					np->ਵਸਤੂ->object.flags |= PARENT2;
				}
			}

			ਜੇ (commit_ਗ੍ਰਾਫ_generation(n) < commit_ਗ੍ਰਾਫ_generation(c))
				ਰੋਕੋ;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		ਲਈ (p = c->parents, ਮਾਪਾ_i = 0; p; p = p->ਅਗਲਾ, ਮਾਪਾ_i++) {
			process_ਮਾਪਾ(lm, &ਕਤਾਰ,
				       c, ਸਰਗਰਮ_c,
				       p->ਵਸਤੂ, ਮਾਪਾ_i);

			ਜੇ (bitmap_is_ਖਾਲੀ_ਥਾਵਾਂ(ਸਰਗਰਮ_c))
				ਰੋਕੋ;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		ਜੇ (!bitmap_is_ਖਾਲੀ_ਥਾਵਾਂ(ਸਰਗਰਮ_c))  {
			ਅੰਕੜੇ.commit = c;
			ਲਈ (ਮਾਪ_t i = 0; i < lm->all_paths_nr; i++) {
				ਜੇ (bitmap_get(ਸਰਗਰਮ_c, i))
					mark_ਰਸਤਾ(lm->all_paths[i], ਨਿਰਾ, &ਅੰਕੜੇ);
			}
		}

cleanup:
		ਸਰਗਰਮ_paths_free(lm, c);
	}

	ਜੇ (hashmap_get_ਮਾਪ(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_ਕਤਾਰ(&not_ਕਤਾਰ);
	clear_prio_ਕਤਾਰ(&ਕਤਾਰ);
	clear_ਸਰਗਰਮ_paths_for_commit(&lm->ਸਰਗਰਮ_paths);
	bitmap_free(lm->scratch);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਆਖਰੀ_modified_init(ਬਣਤਰ ਆਖਰੀ_modified *lm, ਬਣਤਰ repository *r,
			      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ ਆਖਰੀ_modified_ਇੰਦਰਾਜ *ent;

	hashmap_init(&lm->paths, ਆਖਰੀ_modified_ਇੰਦਰਾਜ_hashcmp, ਨਿਰਾ, 0);

	repo_init_revisions(r, &lm->rev, ਅਗੇਤਰ);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_ਜੜ੍ਹ_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_ਰੁੱਖ_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.ਰੁੱਖ_ਵਿੱਚ_recursive = lm->show_trees;
	lm->rev.diffopt.max_ਡੂੰਘਾਈ = lm->max_ਡੂੰਘਾਈ;
	lm->rev.diffopt.max_ਡੂੰਘਾਈ_valid = lm->max_ਡੂੰਘਾਈ >= 0;

	argc = ਤਿਆਰੀ_revisions(argc, argv, &lm->rev, ਨਿਰਾ);
	ਜੇ (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		ਵਾਪਸ argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	ਜੇ (populate_paths_from_revs(lm) < 0)
		ਵਾਪਸ -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_ਮਾਪ(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_ਇੰਦਰਾਜ(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->ਰਸਤਾ;
	}

	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_ਆਖਰੀ_modified(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ ret;
	ਬਣਤਰ ਆਖਰੀ_modified lm = { 0 };

	ਅਚਲ ਅੱਖਰ * ਅਚਲ ਆਖਰੀ_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		ਨਿਰਾ
	};

	ਬਣਤਰ option ਆਖਰੀ_modified_options[] = {
		OPT_ਸੈੱਟ_INT('r', "recursive", &lm.max_ਡੂੰਘਾਈ,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_ਡੂੰਘਾਈ,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', ਨਿਰਾ, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, ਆਖਰੀ_modified_options,
			     ਆਖਰੀ_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, ਨਿਰਾ);

	ret = ਆਖਰੀ_modified_init(&lm, repo, ਅਗੇਤਰ, argc, argv);
	ਜੇ (ret > 0)
		usage_with_options(ਆਖਰੀ_modified_usage,
				   ਆਖਰੀ_modified_options);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;

	ret = ਆਖਰੀ_modified_ਲੜੀਵਾਰ(&lm);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;

ਬਾਹਰਲਾ:
	ਆਖਰੀ_modified_release(&lm);

	ਵਾਪਸ ret;
}
