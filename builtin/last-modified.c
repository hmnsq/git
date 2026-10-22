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
#تعریف_کرو PARENT1 (1u<<16) /* used instead of SEEN */
#تعریف_کرو PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

بنية آخری_modified_اندراج {
	بنية hashmap_اندراج hashent;
	بنية object_id oid;
	بنية bloom_کلید کلید;
	ناپ_t diff_idx;
	ثابت حرف رستہ[FLEX_ARRAY];
};

ساكن صحيح آخری_modified_اندراج_hashcmp(ثابت فراغ *بن_ورتیا_2 بن_ورتیا,
				       ثابت بنية hashmap_اندراج *hent1,
				       ثابت بنية hashmap_اندراج *hent2,
				       ثابت فراغ *رستہ)
{
	ثابت بنية آخری_modified_اندراج *ent1 =
		container_دا(hent1, ثابت بنية آخری_modified_اندراج, hashent);
	ثابت بنية آخری_modified_اندراج *ent2 =
		container_دا(hent2, ثابت بنية آخری_modified_اندراج, hashent);
	أرجع strcmp(ent1->رستہ, رستہ ? رستہ : ent2->رستہ);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(سرگرم_paths_for_commit, بنية bitmap *);

بنية آخری_modified {
	بنية hashmap paths;
	بنية rev_info rev;
	bool show_trees;
	bool nul_termination;
	صحيح max_ڈونگھائی;

	ثابت حرف **all_paths;
	ناپ_t all_paths_nr;
	بنية سرگرم_paths_for_commit سرگرم_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	بنية bitmap *scratch;
};

ساكن بنية bitmap *سرگرم_paths_for(بنية آخری_modified *lm, بنية commit *c)
{
	بنية bitmap **bitmap = سرگرم_paths_for_commit_at(&lm->سرگرم_paths, c);
	إذا (!*bitmap)
		*bitmap = bitmap_لفظ_alloc(lm->all_paths_nr / بٹ_وچ_EWORD + 1);

	أرجع *bitmap;
}

ساكن فراغ سرگرم_paths_free(بنية آخری_modified *lm, بنية commit *c)
{
	بنية bitmap **bitmap = سرگرم_paths_for_commit_at(&lm->سرگرم_paths, c);
	إذا (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = عدم;
	}
}

ساكن فراغ آخری_modified_release(بنية آخری_modified *lm)
{
	بنية hashmap_iter iter;
	بنية آخری_modified_اندراج *ent;

	hashmap_for_each_اندراج(&lm->paths, &iter, ent, hashent)
		bloom_کلید_clear(&ent->کلید);

	hashmap_clear_and_free(&lm->paths, بنية آخری_modified_اندراج, hashent);
	release_revisions(&lm->rev);

	حرر(lm->all_paths);
}

بنية آخری_modified_callback_انکڑے {
	بنية آخری_modified *lm;
	بنية commit *commit;
};

ساكن فراغ جوڑو_رستہ_from_diff(بنية diff_قطار_struct *q,
			       بنية diff_options *opt بن_ورتیا, فراغ *انکڑے)
{
	بنية آخری_modified *lm = انکڑے;

	لكل (صحيح i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->قطار[i];
		بنية آخری_modified_اندراج *ent;
		ثابت حرف *رستہ = p->two->رستہ;

		FLEX_ALLOC_STR(ent, رستہ, رستہ);
		oidcpy(&ent->oid, &p->two->oid);
		إذا (lm->rev.bloom_filter_settings)
			bloom_کلید_fill(&ent->کلید, رستہ, لڑی_لمبائی(رستہ),
				       lm->rev.bloom_filter_settings);
		hashmap_اندراج_init(&ent->hashent, strhash(ent->رستہ));
		hashmap_جوڑو(&lm->paths, &ent->hashent);
	}
}

ساكن صحيح populate_paths_from_revs(بنية آخری_modified *lm)
{
	صحيح عدد_interesting = 0, ret = 0;
	بنية diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	حافظہ_نقل(&diffopt, &lm->rev.diffopt, حجم(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = جوڑو_رستہ_from_diff;
	diffopt.format_callback_انکڑے = lm;

	لكل (ناپ_t i = 0; i < lm->rev.pending.nr; i++) {
		بنية object_array_اندراج *obj = lm->rev.pending.objects + i;

		إذا (obj->شے->flags & UNINTERESTING)
			استمر;

		إذا (عدد_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			اذهب باہرلا;
		}

		إذا (!repo_peel_منزل_type(lm->rev.repo, obj->رستہ, 0, obj->شے, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->شے->type));
			اذهب باہرلا;
		}

		diff_درخت_oid(lm->rev.repo->hash_algo->خالی_تھاواں_درخت,
			      &obj->شے->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

باہرلا:
	clear_pathspec(&diffopt.pathspec);

	أرجع ret;
}

ساكن فراغ آخری_modified_emit(بنية آخری_modified *lm,
			       ثابت حرف *رستہ, ثابت بنية commit *commit)

{
	إذا (commit->object.flags & BOUNDARY)
		حرف_لکھو('^');
	لکھو("%s\t", oid_منزل_hex(&commit->object.oid));

	إذا (lm->nul_termination)
		لکھو("%s%c", رستہ, '\0');
	وإلا
		write_name_quoted(رستہ, stdout, '\n');
}

ساكن فراغ mark_رستہ(ثابت حرف *رستہ, ثابت بنية object_id *oid,
		      بنية آخری_modified_callback_انکڑے *انکڑے)
{
	بنية آخری_modified_اندراج *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_اندراج_from_hash(&انکڑے->lm->paths, strhash(رستہ), رستہ,
					  بنية آخری_modified_اندراج, hashent);
	إذا (!ent)
		أرجع;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	إذا (oid && !oideq(oid, &ent->oid))
		أرجع;

	آخری_modified_emit(انکڑے->lm, رستہ, انکڑے->commit);

	hashmap_remove(&انکڑے->lm->paths, &ent->hashent, رستہ);
	bloom_کلید_clear(&ent->کلید);
	حرر(ent);
}

ساكن فراغ آخری_modified_diff(بنية diff_قطار_struct *q,
			       بنية diff_options *opt بن_ورتیا, فراغ *cbdata)
{
	بنية آخری_modified_callback_انکڑے *انکڑے = cbdata;

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
			mark_رستہ(p->two->رستہ, &p->two->oid, انکڑے);
			اكسر;
		}
	}
}

ساكن فراغ pass_منزل_ماپا(بنية bitmap *c,
			   بنية bitmap *p,
			   ناپ_t pos)
{
	bitmap_unset(c, pos);
	bitmap_سیٹ(p, pos);
}

ساكن bool maybe_بدلیا_رستہ(بنية آخری_modified *lm,
			       بنية commit *origin,
			       بنية bitmap *سرگرم)
{
	بنية bloom_filter *filter;
	بنية آخری_modified_اندراج *ent;
	بنية hashmap_iter iter;

	إذا (!lm->rev.bloom_filter_settings)
		أرجع true;

	إذا (commit_گراف_generation(origin) == GENERATION_نمبر_INFINITY)
		أرجع true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	إذا (!filter)
		أرجع true;

	hashmap_for_each_اندراج(&lm->paths, &iter, ent, hashent) {
		إذا (سرگرم && !bitmap_get(سرگرم, ent->diff_idx))
			استمر;

		إذا (bloom_filter_شامل_اے(filter, &ent->کلید,
					  lm->rev.bloom_filter_settings))
			أرجع true;
	}
	أرجع false;
}

ساكن فراغ process_ماپا(بنية آخری_modified *lm,
			   بنية prio_قطار *قطار,
			   بنية commit *c, بنية bitmap *سرگرم_c,
			   بنية commit *ماپا, صحيح ماپا_i)
{
	بنية bitmap *سرگرم_p;

	إذا (repo_parse_commit(lm->rev.repo, ماپا))
		أرجع;
	سرگرم_p = سرگرم_paths_for(lm, ماپا);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	إذا (ماپا_i || maybe_بدلیا_رستہ(lm, c, سرگرم_c)) {
		diff_درخت_oid(&ماپا->object.oid,
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
		ثابت حرف *رستہ = fp->two->رستہ;
		بنية آخری_modified_اندراج *ent =
			hashmap_get_اندراج_from_hash(&lm->paths, strhash(رستہ), رستہ,
						    بنية آخری_modified_اندراج, hashent);
		إذا (ent) {
			ناپ_t k = ent->diff_idx;
			إذا (bitmap_get(سرگرم_c, k))
				bitmap_سیٹ(lm->scratch, k);
		}
	}
	لكل (ناپ_t i = 0; i < lm->all_paths_nr; i++) {
		إذا (bitmap_get(سرگرم_c, i) && !bitmap_get(lm->scratch, i))
			pass_منزل_ماپا(سرگرم_c, سرگرم_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	إذا (!bitmap_is_خالی_تھاواں(سرگرم_p) && !(ماپا->object.flags & PARENT1)) {
		ماپا->object.flags |= PARENT1;
		prio_قطار_put(قطار, ماپا);
	}
	إذا (!(ماپا->object.flags & PARENT1))
		سرگرم_paths_free(lm, ماپا);

	MEMZERO_ARRAY(lm->scratch->الفاظ, lm->scratch->لفظ_alloc);
	diff_قطار_clear(&diff_queued_diff);
}

ساكن صحيح آخری_modified_لگاتار(بنية آخری_modified *lm)
{
	صحيح max_گنتی, قطار_popped = 0;
	بنية commit *c, *n;
	بنية prio_قطار قطار = { compare_commits_by_gen_then_commit_date };
	بنية prio_قطار not_قطار = { compare_commits_by_gen_then_commit_date };
	بنية commit_list *list;
	بنية آخری_modified_callback_انکڑے انکڑے = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = آخری_modified_diff;
	lm->rev.diffopt.format_callback_انکڑے = &انکڑے;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_گنتی = lm->rev.max_گنتی;

	init_سرگرم_paths_for_commit(&lm->سرگرم_paths);
	lm->scratch = bitmap_لفظ_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	لكل (list = lm->rev.commits; list; list = list->اگلا) {
		بنية commit *c = list->شے;

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
			لكل (ناپ_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_سیٹ(سرگرم, i);
		}
	}

	طالما ((c = prio_قطار_get(&قطار))) {
		صحيح ماپا_i;
		بنية commit_list *p;
		بنية bitmap *سرگرم_c = سرگرم_paths_for(lm, c);

		إذا ((0 <= max_گنتی && max_گنتی < ++قطار_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			انکڑے.commit = c;
			diff_درخت_oid(lm->rev.repo->hash_algo->خالی_تھاواں_درخت,
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

			لكل (np = n->parents; np; np = np->اگلا) {
				إذا (!(np->شے->object.flags & PARENT2)) {
					prio_قطار_put(&not_قطار, np->شے);
					np->شے->object.flags |= PARENT2;
				}
			}

			إذا (commit_گراف_generation(n) < commit_گراف_generation(c))
				اكسر;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		لكل (p = c->parents, ماپا_i = 0; p; p = p->اگلا, ماپا_i++) {
			process_ماپا(lm, &قطار,
				       c, سرگرم_c,
				       p->شے, ماپا_i);

			إذا (bitmap_is_خالی_تھاواں(سرگرم_c))
				اكسر;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		إذا (!bitmap_is_خالی_تھاواں(سرگرم_c))  {
			انکڑے.commit = c;
			لكل (ناپ_t i = 0; i < lm->all_paths_nr; i++) {
				إذا (bitmap_get(سرگرم_c, i))
					mark_رستہ(lm->all_paths[i], عدم, &انکڑے);
			}
		}

cleanup:
		سرگرم_paths_free(lm, c);
	}

	إذا (hashmap_get_ناپ(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_قطار(&not_قطار);
	clear_prio_قطار(&قطار);
	clear_سرگرم_paths_for_commit(&lm->سرگرم_paths);
	bitmap_free(lm->scratch);

	أرجع 0;
}

ساكن صحيح آخری_modified_init(بنية آخری_modified *lm, بنية repository *r,
			      ثابت حرف *اگیتر, صحيح argc, ثابت حرف **argv)
{
	بنية hashmap_iter iter;
	بنية آخری_modified_اندراج *ent;

	hashmap_init(&lm->paths, آخری_modified_اندراج_hashcmp, عدم, 0);

	repo_init_revisions(r, &lm->rev, اگیتر);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_جڑ_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_درخت_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.درخت_وچ_recursive = lm->show_trees;
	lm->rev.diffopt.max_ڈونگھائی = lm->max_ڈونگھائی;
	lm->rev.diffopt.max_ڈونگھائی_valid = lm->max_ڈونگھائی >= 0;

	argc = تیاری_revisions(argc, argv, &lm->rev, عدم);
	إذا (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		أرجع argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	إذا (populate_paths_from_revs(lm) < 0)
		أرجع -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_ناپ(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_اندراج(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->رستہ;
	}

	أرجع 0;
}

صحيح cmd_آخری_modified(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		      بنية repository *repo)
{
	صحيح ret;
	بنية آخری_modified lm = { 0 };

	ثابت حرف * ثابت آخری_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		عدم
	};

	بنية option آخری_modified_options[] = {
		OPT_سیٹ_INT('r', "recursive", &lm.max_ڈونگھائی,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_ڈونگھائی,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', عدم, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, آخری_modified_options,
			     آخری_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, عدم);

	ret = آخری_modified_init(&lm, repo, اگیتر, argc, argv);
	إذا (ret > 0)
		usage_with_options(آخری_modified_usage,
				   آخری_modified_options);
	إذا (ret)
		اذهب باہرلا;

	ret = آخری_modified_لگاتار(&lm);
	إذا (ret)
		اذهب باہرلا;

باہرلا:
	آخری_modified_release(&lm);

	أرجع ret;
}
