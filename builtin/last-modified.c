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
#تعریف_کړه PARENT1 (1u<<16) /* used instead of SEEN */
#تعریف_کړه PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

بنية وروستی_modified_ثبت {
	بنية hashmap_ثبت hashent;
	بنية object_id oid;
	بنية bloom_کلی کلی;
	اندازه_t diff_idx;
	ثابت حرف لار[FLEX_ARRAY];
};

ساكن صحيح وروستی_modified_ثبت_hashcmp(ثابت فراغ *ناکارول_شوی_2 ناکارول_شوی,
				       ثابت بنية hashmap_ثبت *hent1,
				       ثابت بنية hashmap_ثبت *hent2,
				       ثابت فراغ *لار)
{
	ثابت بنية وروستی_modified_ثبت *ent1 =
		container_د(hent1, ثابت بنية وروستی_modified_ثبت, hashent);
	ثابت بنية وروستی_modified_ثبت *ent2 =
		container_د(hent2, ثابت بنية وروستی_modified_ثبت, hashent);
	أرجع strcmp(ent1->لار, لار ? لار : ent2->لار);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(فعال_paths_for_commit, بنية bitmap *);

بنية وروستی_modified {
	بنية hashmap paths;
	بنية rev_info rev;
	bool show_trees;
	bool nul_termination;
	صحيح max_ژوروالی;

	ثابت حرف **all_paths;
	اندازه_t all_paths_nr;
	بنية فعال_paths_for_commit فعال_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	بنية bitmap *scratch;
};

ساكن بنية bitmap *فعال_paths_for(بنية وروستی_modified *lm, بنية commit *c)
{
	بنية bitmap **bitmap = فعال_paths_for_commit_at(&lm->فعال_paths, c);
	إذا (!*bitmap)
		*bitmap = bitmap_کلمه_alloc(lm->all_paths_nr / بټونه_دننه_EWORD + 1);

	أرجع *bitmap;
}

ساكن فراغ فعال_paths_free(بنية وروستی_modified *lm, بنية commit *c)
{
	بنية bitmap **bitmap = فعال_paths_for_commit_at(&lm->فعال_paths, c);
	إذا (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = عدم;
	}
}

ساكن فراغ وروستی_modified_release(بنية وروستی_modified *lm)
{
	بنية hashmap_iter iter;
	بنية وروستی_modified_ثبت *ent;

	hashmap_for_each_ثبت(&lm->paths, &iter, ent, hashent)
		bloom_کلی_clear(&ent->کلی);

	hashmap_clear_and_free(&lm->paths, بنية وروستی_modified_ثبت, hashent);
	release_revisions(&lm->rev);

	حرر(lm->all_paths);
}

بنية وروستی_modified_callback_معلومات {
	بنية وروستی_modified *lm;
	بنية commit *commit;
};

ساكن فراغ جمع_کړه_لار_from_diff(بنية diff_کتار_struct *q,
			       بنية diff_options *opt ناکارول_شوی, فراغ *معلومات)
{
	بنية وروستی_modified *lm = معلومات;

	لكل (صحيح i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->کتار[i];
		بنية وروستی_modified_ثبت *ent;
		ثابت حرف *لار = p->two->لار;

		FLEX_ALLOC_STR(ent, لار, لار);
		oidcpy(&ent->oid, &p->two->oid);
		إذا (lm->rev.bloom_filter_settings)
			bloom_کلی_fill(&ent->کلی, لار, مزي_اوږدوالی(لار),
				       lm->rev.bloom_filter_settings);
		hashmap_ثبت_init(&ent->hashent, strhash(ent->لار));
		hashmap_جمع_کړه(&lm->paths, &ent->hashent);
	}
}

ساكن صحيح populate_paths_from_revs(بنية وروستی_modified *lm)
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
	حافظه_نقل(&diffopt, &lm->rev.diffopt, حجم(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = جمع_کړه_لار_from_diff;
	diffopt.format_callback_معلومات = lm;

	لكل (اندازه_t i = 0; i < lm->rev.pending.nr; i++) {
		بنية object_array_ثبت *obj = lm->rev.pending.objects + i;

		إذا (obj->توکی->flags & UNINTERESTING)
			استمر;

		إذا (عدد_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			اذهب بهر;
		}

		إذا (!repo_peel_منزل_type(lm->rev.repo, obj->لار, 0, obj->توکی, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->توکی->type));
			اذهب بهر;
		}

		diff_ونه_oid(lm->rev.repo->hash_algo->تشه_ونه,
			      &obj->توکی->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

بهر:
	clear_pathspec(&diffopt.pathspec);

	أرجع ret;
}

ساكن فراغ وروستی_modified_emit(بنية وروستی_modified *lm,
			       ثابت حرف *لار, ثابت بنية commit *commit)

{
	إذا (commit->object.flags & BOUNDARY)
		توری_چاپ_کړه('^');
	چاپ_کړه("%s\t", oid_منزل_hex(&commit->object.oid));

	إذا (lm->nul_termination)
		چاپ_کړه("%s%c", لار, '\0');
	وإلا
		write_name_quoted(لار, stdout, '\n');
}

ساكن فراغ mark_لار(ثابت حرف *لار, ثابت بنية object_id *oid,
		      بنية وروستی_modified_callback_معلومات *معلومات)
{
	بنية وروستی_modified_ثبت *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_ثبت_from_hash(&معلومات->lm->paths, strhash(لار), لار,
					  بنية وروستی_modified_ثبت, hashent);
	إذا (!ent)
		أرجع;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	إذا (oid && !oideq(oid, &ent->oid))
		أرجع;

	وروستی_modified_emit(معلومات->lm, لار, معلومات->commit);

	hashmap_remove(&معلومات->lm->paths, &ent->hashent, لار);
	bloom_کلی_clear(&ent->کلی);
	حرر(ent);
}

ساكن فراغ وروستی_modified_diff(بنية diff_کتار_struct *q,
			       بنية diff_options *opt ناکارول_شوی, فراغ *cbdata)
{
	بنية وروستی_modified_callback_معلومات *معلومات = cbdata;

	لكل (صحيح i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->کتار[i];
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
			mark_لار(p->two->لار, &p->two->oid, معلومات);
			اكسر;
		}
	}
}

ساكن فراغ pass_منزل_پلار(بنية bitmap *c,
			   بنية bitmap *p,
			   اندازه_t pos)
{
	bitmap_unset(c, pos);
	bitmap_سېټ(p, pos);
}

ساكن bool maybe_بدل_شوی_لار(بنية وروستی_modified *lm,
			       بنية commit *origin,
			       بنية bitmap *فعال)
{
	بنية bloom_filter *filter;
	بنية وروستی_modified_ثبت *ent;
	بنية hashmap_iter iter;

	إذا (!lm->rev.bloom_filter_settings)
		أرجع true;

	إذا (commit_ګراف_generation(origin) == GENERATION_شمېره_INFINITY)
		أرجع true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	إذا (!filter)
		أرجع true;

	hashmap_for_each_ثبت(&lm->paths, &iter, ent, hashent) {
		إذا (فعال && !bitmap_get(فعال, ent->diff_idx))
			استمر;

		إذا (bloom_filter_لري(filter, &ent->کلی,
					  lm->rev.bloom_filter_settings))
			أرجع true;
	}
	أرجع false;
}

ساكن فراغ process_پلار(بنية وروستی_modified *lm,
			   بنية prio_کتار *کتار,
			   بنية commit *c, بنية bitmap *فعال_c,
			   بنية commit *پلار, صحيح پلار_i)
{
	بنية bitmap *فعال_p;

	إذا (repo_parse_commit(lm->rev.repo, پلار))
		أرجع;
	فعال_p = فعال_paths_for(lm, پلار);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	إذا (پلار_i || maybe_بدل_شوی_لار(lm, c, فعال_c)) {
		diff_ونه_oid(&پلار->object.oid,
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
		بنية diff_filepair *fp = diff_queued_diff.کتار[i];
		ثابت حرف *لار = fp->two->لار;
		بنية وروستی_modified_ثبت *ent =
			hashmap_get_ثبت_from_hash(&lm->paths, strhash(لار), لار,
						    بنية وروستی_modified_ثبت, hashent);
		إذا (ent) {
			اندازه_t k = ent->diff_idx;
			إذا (bitmap_get(فعال_c, k))
				bitmap_سېټ(lm->scratch, k);
		}
	}
	لكل (اندازه_t i = 0; i < lm->all_paths_nr; i++) {
		إذا (bitmap_get(فعال_c, i) && !bitmap_get(lm->scratch, i))
			pass_منزل_پلار(فعال_c, فعال_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	إذا (!bitmap_is_تشه(فعال_p) && !(پلار->object.flags & PARENT1)) {
		پلار->object.flags |= PARENT1;
		prio_کتار_put(کتار, پلار);
	}
	إذا (!(پلار->object.flags & PARENT1))
		فعال_paths_free(lm, پلار);

	MEMZERO_ARRAY(lm->scratch->ټکي, lm->scratch->کلمه_alloc);
	diff_کتار_clear(&diff_queued_diff);
}

ساكن صحيح وروستی_modified_پرلپسې(بنية وروستی_modified *lm)
{
	صحيح max_شمېر, کتار_popped = 0;
	بنية commit *c, *n;
	بنية prio_کتار کتار = { compare_commits_by_gen_then_commit_date };
	بنية prio_کتار not_کتار = { compare_commits_by_gen_then_commit_date };
	بنية commit_list *list;
	بنية وروستی_modified_callback_معلومات معلومات = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = وروستی_modified_diff;
	lm->rev.diffopt.format_callback_معلومات = &معلومات;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_شمېر = lm->rev.max_شمېر;

	init_فعال_paths_for_commit(&lm->فعال_paths);
	lm->scratch = bitmap_کلمه_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	لكل (list = lm->rev.commits; list; list = list->بل) {
		بنية commit *c = list->توکی;

		إذا (c->object.flags & BOTTOM) {
			prio_کتار_put(&not_کتار, c);
			c->object.flags |= PARENT2;
		} وإلا إذا (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			بنية bitmap *فعال;

			prio_کتار_put(&کتار, c);
			c->object.flags |= PARENT1;

			فعال = فعال_paths_for(lm, c);
			لكل (اندازه_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_سېټ(فعال, i);
		}
	}

	طالما ((c = prio_کتار_get(&کتار))) {
		صحيح پلار_i;
		بنية commit_list *p;
		بنية bitmap *فعال_c = فعال_paths_for(lm, c);

		إذا ((0 <= max_شمېر && max_شمېر < ++کتار_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			معلومات.commit = c;
			diff_ونه_oid(lm->rev.repo->hash_algo->تشه_ونه,
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

		طالما ((n = prio_کتار_get(&not_کتار))) {
			بنية commit_list *np;

			إذا (repo_parse_commit(lm->rev.repo, n))
				استمر;

			لكل (np = n->parents; np; np = np->بل) {
				إذا (!(np->توکی->object.flags & PARENT2)) {
					prio_کتار_put(&not_کتار, np->توکی);
					np->توکی->object.flags |= PARENT2;
				}
			}

			إذا (commit_ګراف_generation(n) < commit_ګراف_generation(c))
				اكسر;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		لكل (p = c->parents, پلار_i = 0; p; p = p->بل, پلار_i++) {
			process_پلار(lm, &کتار,
				       c, فعال_c,
				       p->توکی, پلار_i);

			إذا (bitmap_is_تشه(فعال_c))
				اكسر;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		إذا (!bitmap_is_تشه(فعال_c))  {
			معلومات.commit = c;
			لكل (اندازه_t i = 0; i < lm->all_paths_nr; i++) {
				إذا (bitmap_get(فعال_c, i))
					mark_لار(lm->all_paths[i], عدم, &معلومات);
			}
		}

cleanup:
		فعال_paths_free(lm, c);
	}

	إذا (hashmap_get_اندازه(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_کتار(&not_کتار);
	clear_prio_کتار(&کتار);
	clear_فعال_paths_for_commit(&lm->فعال_paths);
	bitmap_free(lm->scratch);

	أرجع 0;
}

ساكن صحيح وروستی_modified_init(بنية وروستی_modified *lm, بنية repository *r,
			      ثابت حرف *مختاړی, صحيح argc, ثابت حرف **argv)
{
	بنية hashmap_iter iter;
	بنية وروستی_modified_ثبت *ent;

	hashmap_init(&lm->paths, وروستی_modified_ثبت_hashcmp, عدم, 0);

	repo_init_revisions(r, &lm->rev, مختاړی);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_ریښه_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_ونه_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.ونه_دننه_recursive = lm->show_trees;
	lm->rev.diffopt.max_ژوروالی = lm->max_ژوروالی;
	lm->rev.diffopt.max_ژوروالی_valid = lm->max_ژوروالی >= 0;

	argc = تیاری_revisions(argc, argv, &lm->rev, عدم);
	إذا (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		أرجع argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	إذا (populate_paths_from_revs(lm) < 0)
		أرجع -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_اندازه(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_ثبت(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->لار;
	}

	أرجع 0;
}

صحيح cmd_وروستی_modified(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		      بنية repository *repo)
{
	صحيح ret;
	بنية وروستی_modified lm = { 0 };

	ثابت حرف * ثابت وروستی_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		عدم
	};

	بنية option وروستی_modified_options[] = {
		OPT_سېټ_INT('r', "recursive", &lm.max_ژوروالی,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_ژوروالی,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', عدم, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, وروستی_modified_options,
			     وروستی_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, عدم);

	ret = وروستی_modified_init(&lm, repo, مختاړی, argc, argv);
	إذا (ret > 0)
		usage_with_options(وروستی_modified_usage,
				   وروستی_modified_options);
	إذا (ret)
		اذهب بهر;

	ret = وروستی_modified_پرلپسې(&lm);
	إذا (ret)
		اذهب بهر;

بهر:
	وروستی_modified_release(&lm);

	أرجع ret;
}
