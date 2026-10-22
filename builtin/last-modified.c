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
#تعریف_کن PARENT1 (1u<<16) /* used instead of SEEN */
#تعریف_کن PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

بنية آخرین_modified_مدخل {
	بنية hashmap_مدخل hashent;
	بنية object_id oid;
	بنية bloom_کلید کلید;
	اندازه_t diff_idx;
	ثابت حرف مسیر[FLEX_ARRAY];
};

ساكن صحيح آخرین_modified_مدخل_hashcmp(ثابت فراغ *استفادهنشده_2 استفادهنشده,
				       ثابت بنية hashmap_مدخل *hent1,
				       ثابت بنية hashmap_مدخل *hent2,
				       ثابت فراغ *مسیر)
{
	ثابت بنية آخرین_modified_مدخل *ent1 =
		container_از(hent1, ثابت بنية آخرین_modified_مدخل, hashent);
	ثابت بنية آخرین_modified_مدخل *ent2 =
		container_از(hent2, ثابت بنية آخرین_modified_مدخل, hashent);
	أرجع strcmp(ent1->مسیر, مسیر ? مسیر : ent2->مسیر);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(فعال_paths_for_commit, بنية bitmap *);

بنية آخرین_modified {
	بنية hashmap paths;
	بنية rev_info rev;
	bool show_trees;
	bool nul_termination;
	صحيح max_عمق;

	ثابت حرف **all_paths;
	اندازه_t all_paths_nr;
	بنية فعال_paths_for_commit فعال_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	بنية bitmap *scratch;
};

ساكن بنية bitmap *فعال_paths_for(بنية آخرین_modified *lm, بنية commit *c)
{
	بنية bitmap **bitmap = فعال_paths_for_commit_at(&lm->فعال_paths, c);
	إذا (!*bitmap)
		*bitmap = bitmap_کلمه_alloc(lm->all_paths_nr / بیتها_درون_EWORD + 1);

	أرجع *bitmap;
}

ساكن فراغ فعال_paths_free(بنية آخرین_modified *lm, بنية commit *c)
{
	بنية bitmap **bitmap = فعال_paths_for_commit_at(&lm->فعال_paths, c);
	إذا (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = عدم;
	}
}

ساكن فراغ آخرین_modified_release(بنية آخرین_modified *lm)
{
	بنية hashmap_iter iter;
	بنية آخرین_modified_مدخل *ent;

	hashmap_for_each_مدخل(&lm->paths, &iter, ent, hashent)
		bloom_کلید_clear(&ent->کلید);

	hashmap_clear_and_free(&lm->paths, بنية آخرین_modified_مدخل, hashent);
	release_revisions(&lm->rev);

	حرر(lm->all_paths);
}

بنية آخرین_modified_callback_داده {
	بنية آخرین_modified *lm;
	بنية commit *commit;
};

ساكن فراغ جمع_کن_مسیر_from_diff(بنية diff_صف_struct *q,
			       بنية diff_options *opt استفادهنشده, فراغ *داده)
{
	بنية آخرین_modified *lm = داده;

	لكل (صحيح i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->صف[i];
		بنية آخرین_modified_مدخل *ent;
		ثابت حرف *مسیر = p->two->مسیر;

		FLEX_ALLOC_STR(ent, مسیر, مسیر);
		oidcpy(&ent->oid, &p->two->oid);
		إذا (lm->rev.bloom_filter_settings)
			bloom_کلید_fill(&ent->کلید, مسیر, طول_رشته(مسیر),
				       lm->rev.bloom_filter_settings);
		hashmap_مدخل_init(&ent->hashent, strhash(ent->مسیر));
		hashmap_جمع_کن(&lm->paths, &ent->hashent);
	}
}

ساكن صحيح populate_paths_from_revs(بنية آخرین_modified *lm)
{
	صحيح رقم_interesting = 0, ret = 0;
	بنية diff_options diffopt;

	/*
	 * Create a copy of `struct diff_options`. In this copy a callback is
	 * set that when called adds entries to `paths` in `struct last_modified`.
	 * This copy is used to diff the tree of the target revision against an
	 * empty tree. This results in all paths in the target revision being
	 * listed. After `paths` is populated, we don't need this copy no more.
	 */
	کپی_حافظه(&diffopt, &lm->rev.diffopt, حجم(diffopt));
	copy_pathspec(&diffopt.pathspec, &lm->rev.diffopt.pathspec);
	diffopt.output_format = DIFF_FORMAT_CALLBACK;
	diffopt.format_callback = جمع_کن_مسیر_from_diff;
	diffopt.format_callback_داده = lm;

	لكل (اندازه_t i = 0; i < lm->rev.pending.nr; i++) {
		بنية object_array_مدخل *obj = lm->rev.pending.objects + i;

		إذا (obj->قلم->flags & UNINTERESTING)
			استمر;

		إذا (رقم_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			اذهب بیرون;
		}

		إذا (!repo_peel_مقصد_type(lm->rev.repo, obj->مسیر, 0, obj->قلم, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->قلم->type));
			اذهب بیرون;
		}

		diff_درخت_oid(lm->rev.repo->hash_algo->خالی_درخت,
			      &obj->قلم->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

بیرون:
	clear_pathspec(&diffopt.pathspec);

	أرجع ret;
}

ساكن فراغ آخرین_modified_emit(بنية آخرین_modified *lm,
			       ثابت حرف *مسیر, ثابت بنية commit *commit)

{
	إذا (commit->object.flags & BOUNDARY)
		حرف_چاپ_کن('^');
	چاپ_کن("%s\t", oid_مقصد_hex(&commit->object.oid));

	إذا (lm->nul_termination)
		چاپ_کن("%s%c", مسیر, '\0');
	وإلا
		write_name_quoted(مسیر, stdout, '\n');
}

ساكن فراغ mark_مسیر(ثابت حرف *مسیر, ثابت بنية object_id *oid,
		      بنية آخرین_modified_callback_داده *داده)
{
	بنية آخرین_modified_مدخل *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_مدخل_from_hash(&داده->lm->paths, strhash(مسیر), مسیر,
					  بنية آخرین_modified_مدخل, hashent);
	إذا (!ent)
		أرجع;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	إذا (oid && !oideq(oid, &ent->oid))
		أرجع;

	آخرین_modified_emit(داده->lm, مسیر, داده->commit);

	hashmap_remove(&داده->lm->paths, &ent->hashent, مسیر);
	bloom_کلید_clear(&ent->کلید);
	حرر(ent);
}

ساكن فراغ آخرین_modified_diff(بنية diff_صف_struct *q,
			       بنية diff_options *opt استفادهنشده, فراغ *cbdata)
{
	بنية آخرین_modified_callback_داده *داده = cbdata;

	لكل (صحيح i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->صف[i];
		بدل (p->وضعیت) {
		حالة DIFF_وضعیت_DELETED:
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
			mark_مسیر(p->two->مسیر, &p->two->oid, داده);
			اكسر;
		}
	}
}

ساكن فراغ pass_مقصد_والد(بنية bitmap *c,
			   بنية bitmap *p,
			   اندازه_t pos)
{
	bitmap_unset(c, pos);
	bitmap_مجموعه(p, pos);
}

ساكن bool maybe_تغییریافته_مسیر(بنية آخرین_modified *lm,
			       بنية commit *origin,
			       بنية bitmap *فعال)
{
	بنية bloom_filter *filter;
	بنية آخرین_modified_مدخل *ent;
	بنية hashmap_iter iter;

	إذا (!lm->rev.bloom_filter_settings)
		أرجع true;

	إذا (commit_گراف_generation(origin) == GENERATION_عدد_INFINITY)
		أرجع true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	إذا (!filter)
		أرجع true;

	hashmap_for_each_مدخل(&lm->paths, &iter, ent, hashent) {
		إذا (فعال && !bitmap_get(فعال, ent->diff_idx))
			استمر;

		إذا (bloom_filter_شامل_است(filter, &ent->کلید,
					  lm->rev.bloom_filter_settings))
			أرجع true;
	}
	أرجع false;
}

ساكن فراغ process_والد(بنية آخرین_modified *lm,
			   بنية prio_صف *صف,
			   بنية commit *c, بنية bitmap *فعال_c,
			   بنية commit *والد, صحيح والد_i)
{
	بنية bitmap *فعال_p;

	إذا (repo_parse_commit(lm->rev.repo, والد))
		أرجع;
	فعال_p = فعال_paths_for(lm, والد);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	إذا (والد_i || maybe_تغییریافته_مسیر(lm, c, فعال_c)) {
		diff_درخت_oid(&والد->object.oid,
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
		بنية diff_filepair *fp = diff_queued_diff.صف[i];
		ثابت حرف *مسیر = fp->two->مسیر;
		بنية آخرین_modified_مدخل *ent =
			hashmap_get_مدخل_from_hash(&lm->paths, strhash(مسیر), مسیر,
						    بنية آخرین_modified_مدخل, hashent);
		إذا (ent) {
			اندازه_t k = ent->diff_idx;
			إذا (bitmap_get(فعال_c, k))
				bitmap_مجموعه(lm->scratch, k);
		}
	}
	لكل (اندازه_t i = 0; i < lm->all_paths_nr; i++) {
		إذا (bitmap_get(فعال_c, i) && !bitmap_get(lm->scratch, i))
			pass_مقصد_والد(فعال_c, فعال_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	إذا (!bitmap_is_خالی(فعال_p) && !(والد->object.flags & PARENT1)) {
		والد->object.flags |= PARENT1;
		prio_صف_put(صف, والد);
	}
	إذا (!(والد->object.flags & PARENT1))
		فعال_paths_free(lm, والد);

	MEMZERO_ARRAY(lm->scratch->واژگان, lm->scratch->کلمه_alloc);
	diff_صف_clear(&diff_queued_diff);
}

ساكن صحيح آخرین_modified_پیاپی(بنية آخرین_modified *lm)
{
	صحيح max_شمار, صف_popped = 0;
	بنية commit *c, *n;
	بنية prio_صف صف = { compare_commits_by_gen_then_commit_date };
	بنية prio_صف not_صف = { compare_commits_by_gen_then_commit_date };
	بنية commit_list *list;
	بنية آخرین_modified_callback_داده داده = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = آخرین_modified_diff;
	lm->rev.diffopt.format_callback_داده = &داده;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_شمار = lm->rev.max_شمار;

	init_فعال_paths_for_commit(&lm->فعال_paths);
	lm->scratch = bitmap_کلمه_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	لكل (list = lm->rev.commits; list; list = list->بعدی) {
		بنية commit *c = list->قلم;

		إذا (c->object.flags & BOTTOM) {
			prio_صف_put(&not_صف, c);
			c->object.flags |= PARENT2;
		} وإلا إذا (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			بنية bitmap *فعال;

			prio_صف_put(&صف, c);
			c->object.flags |= PARENT1;

			فعال = فعال_paths_for(lm, c);
			لكل (اندازه_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_مجموعه(فعال, i);
		}
	}

	طالما ((c = prio_صف_get(&صف))) {
		صحيح والد_i;
		بنية commit_list *p;
		بنية bitmap *فعال_c = فعال_paths_for(lm, c);

		إذا ((0 <= max_شمار && max_شمار < ++صف_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			داده.commit = c;
			diff_درخت_oid(lm->rev.repo->hash_algo->خالی_درخت,
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

		طالما ((n = prio_صف_get(&not_صف))) {
			بنية commit_list *np;

			إذا (repo_parse_commit(lm->rev.repo, n))
				استمر;

			لكل (np = n->parents; np; np = np->بعدی) {
				إذا (!(np->قلم->object.flags & PARENT2)) {
					prio_صف_put(&not_صف, np->قلم);
					np->قلم->object.flags |= PARENT2;
				}
			}

			إذا (commit_گراف_generation(n) < commit_گراف_generation(c))
				اكسر;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		لكل (p = c->parents, والد_i = 0; p; p = p->بعدی, والد_i++) {
			process_والد(lm, &صف,
				       c, فعال_c,
				       p->قلم, والد_i);

			إذا (bitmap_is_خالی(فعال_c))
				اكسر;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		إذا (!bitmap_is_خالی(فعال_c))  {
			داده.commit = c;
			لكل (اندازه_t i = 0; i < lm->all_paths_nr; i++) {
				إذا (bitmap_get(فعال_c, i))
					mark_مسیر(lm->all_paths[i], عدم, &داده);
			}
		}

cleanup:
		فعال_paths_free(lm, c);
	}

	إذا (hashmap_get_اندازه(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_صف(&not_صف);
	clear_prio_صف(&صف);
	clear_فعال_paths_for_commit(&lm->فعال_paths);
	bitmap_free(lm->scratch);

	أرجع 0;
}

ساكن صحيح آخرین_modified_init(بنية آخرین_modified *lm, بنية repository *r,
			      ثابت حرف *پیشوند, صحيح argc, ثابت حرف **argv)
{
	بنية hashmap_iter iter;
	بنية آخرین_modified_مدخل *ent;

	hashmap_init(&lm->paths, آخرین_modified_مدخل_hashcmp, عدم, 0);

	repo_init_revisions(r, &lm->rev, پیشوند);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_ریشه_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_درخت_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.درخت_درون_recursive = lm->show_trees;
	lm->rev.diffopt.max_عمق = lm->max_عمق;
	lm->rev.diffopt.max_عمق_valid = lm->max_عمق >= 0;

	argc = راهاندازی_revisions(argc, argv, &lm->rev, عدم);
	إذا (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		أرجع argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	إذا (populate_paths_from_revs(lm) < 0)
		أرجع -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_اندازه(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_مدخل(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->مسیر;
	}

	أرجع 0;
}

صحيح cmd_آخرین_modified(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		      بنية repository *repo)
{
	صحيح ret;
	بنية آخرین_modified lm = { 0 };

	ثابت حرف * ثابت آخرین_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		عدم
	};

	بنية option آخرین_modified_options[] = {
		OPT_مجموعه_INT('r', "recursive", &lm.max_عمق,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_عمق,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', عدم, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, آخرین_modified_options,
			     آخرین_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, عدم);

	ret = آخرین_modified_init(&lm, repo, پیشوند, argc, argv);
	إذا (ret > 0)
		usage_with_options(آخرین_modified_usage,
				   آخرین_modified_options);
	إذا (ret)
		اذهب بیرون;

	ret = آخرین_modified_پیاپی(&lm);
	إذا (ret)
		اذهب بیرون;

بیرون:
	آخرین_modified_release(&lm);

	أرجع ret;
}
