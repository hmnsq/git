#ضمن "git-compat-util.h"
#ضمن "bloom.h"
#ضمن "builtin.h"
#ضمن "commit-graph.h"
#ضمن "commit-slab.h"
#ضمن "commit.h"
#ضمن "config.h"
#ضمن "diff.h"
#ضمن "diffcore.h"
#ضمن "environment.h"
#ضمن "ewah/ewok.h"
#ضمن "hashmap.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "object.h"
#ضمن "parse-options.h"
#ضمن "prio-queue.h"
#ضمن "quote.h"
#ضمن "repository.h"
#ضمن "revision.h"

/* Remember to update object flag allocation in object.h */
#وصف_ڪريو PARENT1 (1u<<16) /* used instead of SEEN */
#وصف_ڪريو PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

بنية آخري_modified_داخلا {
	بنية hashmap_داخلا hashent;
	بنية object_id oid;
	بنية bloom_ڪنجي ڪنجي;
	سائيز_t diff_idx;
	ثابت حرف رستو[FLEX_ARRAY];
};

ساكن صحيح آخري_modified_داخلا_hashcmp(ثابت فراغ *اڻ_استعمال_ٿيل_2 اڻ_استعمال_ٿيل,
				       ثابت بنية hashmap_داخلا *hent1,
				       ثابت بنية hashmap_داخلا *hent2,
				       ثابت فراغ *رستو)
{
	ثابت بنية آخري_modified_داخلا *ent1 =
		container_جو(hent1, ثابت بنية آخري_modified_داخلا, hashent);
	ثابت بنية آخري_modified_داخلا *ent2 =
		container_جو(hent2, ثابت بنية آخري_modified_داخلا, hashent);
	أرجع strcmp(ent1->رستو, رستو ? رستو : ent2->رستو);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(سرگرم_paths_for_commit, بنية bitmap *);

بنية آخري_modified {
	بنية hashmap paths;
	بنية rev_info rev;
	bool show_trees;
	bool nul_termination;
	صحيح max_اونهائي;

	ثابت حرف **all_paths;
	سائيز_t all_paths_nr;
	بنية سرگرم_paths_for_commit سرگرم_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	بنية bitmap *scratch;
};

ساكن بنية bitmap *سرگرم_paths_for(بنية آخري_modified *lm, بنية commit *c)
{
	بنية bitmap **bitmap = سرگرم_paths_for_commit_at(&lm->سرگرم_paths, c);
	إذا (!*bitmap)
		*bitmap = bitmap_لفظ_alloc(lm->all_paths_nr / بٽ_اندريون_EWORD + 1);

	أرجع *bitmap;
}

ساكن فراغ سرگرم_paths_free(بنية آخري_modified *lm, بنية commit *c)
{
	بنية bitmap **bitmap = سرگرم_paths_for_commit_at(&lm->سرگرم_paths, c);
	إذا (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = عدم;
	}
}

ساكن فراغ آخري_modified_release(بنية آخري_modified *lm)
{
	بنية hashmap_iter iter;
	بنية آخري_modified_داخلا *ent;

	hashmap_for_each_داخلا(&lm->paths, &iter, ent, hashent)
		bloom_ڪنجي_clear(&ent->ڪنجي);

	hashmap_clear_and_free(&lm->paths, بنية آخري_modified_داخلا, hashent);
	release_revisions(&lm->rev);

	حرر(lm->all_paths);
}

بنية آخري_modified_callback_معلومات {
	بنية آخري_modified *lm;
	بنية commit *commit;
};

ساكن فراغ جوڙيو_رستو_from_diff(بنية diff_قطار_struct *q,
			       بنية diff_options *opt اڻ_استعمال_ٿيل, فراغ *معلومات)
{
	بنية آخري_modified *lm = معلومات;

	لكل (صحيح i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->قطار[i];
		بنية آخري_modified_داخلا *ent;
		ثابت حرف *رستو = p->two->رستو;

		FLEX_ALLOC_STR(ent, رستو, رستو);
		oidcpy(&ent->oid, &p->two->oid);
		إذا (lm->rev.bloom_filter_settings)
			bloom_ڪنجي_fill(&ent->ڪنجي, رستو, لڙي_ڊيگهه(رستو),
				       lm->rev.bloom_filter_settings);
		hashmap_داخلا_init(&ent->hashent, strhash(ent->رستو));
		hashmap_جوڙيو(&lm->paths, &ent->hashent);
	}
}

ساكن صحيح populate_paths_from_revs(بنية آخري_modified *lm)
{
	صحيح انگ_interesting = 0, ret = 0;
	بنية diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	يادگيري_نقل(&diffopt, &lm->rev.diffopt, حجم(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = جوڙيو_رستو_from_diff;
	diffopt.format_callback_معلومات = lm;

	لكل (سائيز_t i = 0; i < lm->rev.pending.nr; i++) {
		بنية object_array_داخلا *obj = lm->rev.pending.objects + i;

		إذا (obj->شي->flags & UNINTERESTING)
			استمر;

		إذا (انگ_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			اذهب ٻاهريون;
		}

		إذا (!repo_peel_منزل_type(lm->rev.repo, obj->رستو, 0, obj->شي, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->شي->type));
			اذهب ٻاهريون;
		}

		diff_وڻ_oid(lm->rev.repo->hash_algo->خالي_جايون_وڻ,
			      &obj->شي->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

ٻاهريون:
	clear_pathspec(&diffopt.pathspec);

	أرجع ret;
}

ساكن فراغ آخري_modified_emit(بنية آخري_modified *lm,
			       ثابت حرف *رستو, ثابت بنية commit *commit)

{
	إذا (commit->object.flags & BOUNDARY)
		اکر_لکو('^');
	لکو("%s\t", oid_منزل_hex(&commit->object.oid));

	إذا (lm->nul_termination)
		لکو("%s%c", رستو, '\0');
	وإلا
		write_name_quoted(رستو, stdout, '\n');
}

ساكن فراغ mark_رستو(ثابت حرف *رستو, ثابت بنية object_id *oid,
		      بنية آخري_modified_callback_معلومات *معلومات)
{
	بنية آخري_modified_داخلا *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_داخلا_from_hash(&معلومات->lm->paths, strhash(رستو), رستو,
					  بنية آخري_modified_داخلا, hashent);
	إذا (!ent)
		أرجع;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	إذا (oid && !oideq(oid, &ent->oid))
		أرجع;

	آخري_modified_emit(معلومات->lm, رستو, معلومات->commit);

	hashmap_remove(&معلومات->lm->paths, &ent->hashent, رستو);
	bloom_ڪنجي_clear(&ent->ڪنجي);
	حرر(ent);
}

ساكن فراغ آخري_modified_diff(بنية diff_قطار_struct *q,
			       بنية diff_options *opt اڻ_استعمال_ٿيل, فراغ *cbdata)
{
	بنية آخري_modified_callback_معلومات *معلومات = cbdata;

	لكل (صحيح i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->قطار[i];
		بدل (p->حالت) {
		حالة DIFF_حالت_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			اكسر;

		افتراضي:
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
			mark_رستو(p->two->رستو, &p->two->oid, معلومات);
			اكسر;
		}
	}
}

ساكن فراغ pass_منزل_والد(بنية bitmap *c,
			   بنية bitmap *p,
			   سائيز_t pos)
{
	bitmap_unset(c, pos);
	bitmap_سيٽ(p, pos);
}

ساكن bool maybe_بدليو_رستو(بنية آخري_modified *lm,
			       بنية commit *origin,
			       بنية bitmap *سرگرم)
{
	بنية bloom_filter *filter;
	بنية آخري_modified_داخلا *ent;
	بنية hashmap_iter iter;

	إذا (!lm->rev.bloom_filter_settings)
		أرجع true;

	إذا (commit_گراف_generation(origin) == GENERATION_عدد_INFINITY)
		أرجع true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	إذا (!filter)
		أرجع true;

	hashmap_for_each_داخلا(&lm->paths, &iter, ent, hashent) {
		إذا (سرگرم && !bitmap_get(سرگرم, ent->diff_idx))
			استمر;

		إذا (bloom_filter_شامل_آهي(filter, &ent->ڪنجي,
					  lm->rev.bloom_filter_settings))
			أرجع true;
	}
	أرجع false;
}

ساكن فراغ process_والد(بنية آخري_modified *lm,
			   بنية prio_قطار *قطار,
			   بنية commit *c, بنية bitmap *سرگرم_c,
			   بنية commit *والد, صحيح والد_i)
{
	بنية bitmap *سرگرم_p;

	إذا (repo_parse_commit(lm->rev.repo, والد))
		أرجع;
	سرگرم_p = سرگرم_paths_for(lm, والد);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	إذا (والد_i || maybe_بدليو_رستو(lm, c, سرگرم_c)) {
		diff_وڻ_oid(&والد->object.oid,
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
	لكل (صحيح i = 0; i < diff_queued_diff.nr; i++) {
		بنية diff_filepair *fp = diff_queued_diff.قطار[i];
		ثابت حرف *رستو = fp->two->رستو;
		بنية آخري_modified_داخلا *ent =
			hashmap_get_داخلا_from_hash(&lm->paths, strhash(رستو), رستو,
						    بنية آخري_modified_داخلا, hashent);
		إذا (ent) {
			سائيز_t k = ent->diff_idx;
			إذا (bitmap_get(سرگرم_c, k))
				bitmap_سيٽ(lm->scratch, k);
		}
	}
	لكل (سائيز_t i = 0; i < lm->all_paths_nr; i++) {
		إذا (bitmap_get(سرگرم_c, i) && !bitmap_get(lm->scratch, i))
			pass_منزل_والد(سرگرم_c, سرگرم_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	إذا (!bitmap_is_خالي_جايون(سرگرم_p) && !(والد->object.flags & PARENT1)) {
		والد->object.flags |= PARENT1;
		prio_قطار_put(قطار, والد);
	}
	إذا (!(والد->object.flags & PARENT1))
		سرگرم_paths_free(lm, والد);

	MEMZERO_ARRAY(lm->scratch->لفظ, lm->scratch->لفظ_alloc);
	diff_قطار_clear(&diff_queued_diff);
}

ساكن صحيح آخري_modified_لاڳيتو(بنية آخري_modified *lm)
{
	صحيح max_ڳڻپ, قطار_popped = 0;
	بنية commit *c, *n;
	بنية prio_قطار قطار = { compare_commits_by_gen_then_commit_date };
	بنية prio_قطار not_قطار = { compare_commits_by_gen_then_commit_date };
	بنية commit_list *list;
	بنية آخري_modified_callback_معلومات معلومات = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = آخري_modified_diff;
	lm->rev.diffopt.format_callback_معلومات = &معلومات;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_ڳڻپ = lm->rev.max_ڳڻپ;

	init_سرگرم_paths_for_commit(&lm->سرگرم_paths);
	lm->scratch = bitmap_لفظ_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	لكل (list = lm->rev.commits; list; list = list->ايندڙ) {
		بنية commit *c = list->شي;

		إذا (c->object.flags & BOTTOM) {
			prio_قطار_put(&not_قطار, c);
			c->object.flags |= PARENT2;
		} وإلا إذا (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			بنية bitmap *سرگرم;

			prio_قطار_put(&قطار, c);
			c->object.flags |= PARENT1;

			سرگرم = سرگرم_paths_for(lm, c);
			لكل (سائيز_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_سيٽ(سرگرم, i);
		}
	}

	طالما ((c = prio_قطار_get(&قطار))) {
		صحيح والد_i;
		بنية commit_list *p;
		بنية bitmap *سرگرم_c = سرگرم_paths_for(lm, c);

		إذا ((0 <= max_ڳڻپ && max_ڳڻپ < ++قطار_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			معلومات.commit = c;
			diff_وڻ_oid(lm->rev.repo->hash_algo->خالي_جايون_وڻ,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			اذهب cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		إذا (repo_parse_commit(lm->rev.repo, c))
			اذهب cleanup;

		طالما ((n = prio_قطار_get(&not_قطار))) {
			بنية commit_list *np;

			إذا (repo_parse_commit(lm->rev.repo, n))
				استمر;

			لكل (np = n->parents; np; np = np->ايندڙ) {
				إذا (!(np->شي->object.flags & PARENT2)) {
					prio_قطار_put(&not_قطار, np->شي);
					np->شي->object.flags |= PARENT2;
				}
			}

			إذا (commit_گراف_generation(n) < commit_گراف_generation(c))
				اكسر;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		لكل (p = c->parents, والد_i = 0; p; p = p->ايندڙ, والد_i++) {
			process_والد(lm, &قطار,
				       c, سرگرم_c,
				       p->شي, والد_i);

			إذا (bitmap_is_خالي_جايون(سرگرم_c))
				اكسر;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		إذا (!bitmap_is_خالي_جايون(سرگرم_c))  {
			معلومات.commit = c;
			لكل (سائيز_t i = 0; i < lm->all_paths_nr; i++) {
				إذا (bitmap_get(سرگرم_c, i))
					mark_رستو(lm->all_paths[i], عدم, &معلومات);
			}
		}

cleanup:
		سرگرم_paths_free(lm, c);
	}

	إذا (hashmap_get_سائيز(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_قطار(&not_قطار);
	clear_prio_قطار(&قطار);
	clear_سرگرم_paths_for_commit(&lm->سرگرم_paths);
	bitmap_free(lm->scratch);

	أرجع 0;
}

ساكن صحيح آخري_modified_init(بنية آخري_modified *lm, بنية repository *r,
			      ثابت حرف *اڳياڙي, صحيح argc, ثابت حرف **argv)
{
	بنية hashmap_iter iter;
	بنية آخري_modified_داخلا *ent;

	hashmap_init(&lm->paths, آخري_modified_داخلا_hashcmp, عدم, 0);

	repo_init_revisions(r, &lm->rev, اڳياڙي);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_پاڙ_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_وڻ_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.وڻ_اندريون_recursive = lm->show_trees;
	lm->rev.diffopt.max_اونهائي = lm->max_اونهائي;
	lm->rev.diffopt.max_اونهائي_valid = lm->max_اونهائي >= 0;

	argc = تياري_revisions(argc, argv, &lm->rev, عدم);
	إذا (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		أرجع argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	إذا (populate_paths_from_revs(lm) < 0)
		أرجع -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_سائيز(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_داخلا(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->رستو;
	}

	أرجع 0;
}

صحيح cmd_آخري_modified(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		      بنية repository *repo)
{
	صحيح ret;
	بنية آخري_modified lm = { 0 };

	ثابت حرف * ثابت آخري_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		عدم
	};

	بنية option آخري_modified_options[] = {
		OPT_سيٽ_INT('r', "recursive", &lm.max_اونهائي,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_اونهائي,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', عدم, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, آخري_modified_options,
			     آخري_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, عدم);

	ret = آخري_modified_init(&lm, repo, اڳياڙي, argc, argv);
	إذا (ret > 0)
		usage_with_options(آخري_modified_usage,
				   آخري_modified_options);
	إذا (ret)
		اذهب ٻاهريون;

	ret = آخري_modified_لاڳيتو(&lm);
	إذا (ret)
		اذهب ٻاهريون;

ٻاهريون:
	آخري_modified_release(&lm);

	أرجع ret;
}
