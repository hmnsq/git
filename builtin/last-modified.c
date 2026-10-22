#شامل "git-compat-util.h"
#شامل "bloom.h"
#شامل "builtin.h"
#شامل "commit-graph.h"
#شامل "commit-slab.h"
#شامل "commit.h"
#شامل "config.h"
#شامل "diff.h"
#شامل "diffcore.h"
#شامل "environment.h"
#شامل "ewah/ewok.h"
#شامل "hashmap.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "object.h"
#شامل "parse-options.h"
#شامل "prio-queue.h"
#شامل "quote.h"
#شامل "repository.h"
#شامل "revision.h"

/* Remember to update object flag allocation in object.h */
#تعریف_کرو PARENT1 (1u<<16) /* used instead of SEEN */
#تعریف_کرو PARENT2 (1u<<17) /* used instead of BOTTOM, BOUNDARY */

ساخت آخری_modified_اندراج {
	ساخت hashmap_اندراج hashent;
	ساخت object_id oid;
	ساخت bloom_کلید کلید;
	سائز_t diff_idx;
	ثابت حرف راستہ[FLEX_ARRAY];
};

ساکن صحیح آخری_modified_اندراج_hashcmp(ثابت خالی *غیر_مستعمل_2 غیر_مستعمل,
				       ثابت ساخت hashmap_اندراج *hent1,
				       ثابت ساخت hashmap_اندراج *hent2,
				       ثابت خالی *راستہ)
{
	ثابت ساخت آخری_modified_اندراج *ent1 =
		container_کا(hent1, ثابت ساخت آخری_modified_اندراج, hashent);
	ثابت ساخت آخری_modified_اندراج *ent2 =
		container_کا(hent2, ثابت ساخت آخری_modified_اندراج, hashent);
	واپس strcmp(ent1->راستہ, راستہ ? راستہ : ent2->راستہ);
}

/*
 * Hold a bitmap for each commit we're working with. In the bitmap, each bit
 * represents a path in `lm->all_paths`. An active bit indicates the path still
 * needs to be associated to a commit.
 */
define_commit_slab(فعال_paths_for_commit, ساخت bitmap *);

ساخت آخری_modified {
	ساخت hashmap paths;
	ساخت rev_info rev;
	bool show_trees;
	bool nul_termination;
	صحیح max_گہرائی;

	ثابت حرف **all_paths;
	سائز_t all_paths_nr;
	ساخت فعال_paths_for_commit فعال_paths;

	/* 'scratch' to avoid allocating a bitmap every process_parent() */
	ساخت bitmap *scratch;
};

ساکن ساخت bitmap *فعال_paths_for(ساخت آخری_modified *lm, ساخت commit *c)
{
	ساخت bitmap **bitmap = فعال_paths_for_commit_at(&lm->فعال_paths, c);
	اگر (!*bitmap)
		*bitmap = bitmap_لفظ_alloc(lm->all_paths_nr / بٹ_اندر_EWORD + 1);

	واپس *bitmap;
}

ساکن خالی فعال_paths_free(ساخت آخری_modified *lm, ساخت commit *c)
{
	ساخت bitmap **bitmap = فعال_paths_for_commit_at(&lm->فعال_paths, c);
	اگر (*bitmap) {
		bitmap_free(*bitmap);
		*bitmap = تہی;
	}
}

ساکن خالی آخری_modified_release(ساخت آخری_modified *lm)
{
	ساخت hashmap_iter iter;
	ساخت آخری_modified_اندراج *ent;

	hashmap_for_each_اندراج(&lm->paths, &iter, ent, hashent)
		bloom_کلید_clear(&ent->کلید);

	hashmap_clear_and_free(&lm->paths, ساخت آخری_modified_اندراج, hashent);
	release_revisions(&lm->rev);

	آزاد(lm->all_paths);
}

ساخت آخری_modified_callback_ڈیٹا {
	ساخت آخری_modified *lm;
	ساخت commit *commit;
};

ساکن خالی جوڑو_راستہ_from_diff(ساخت diff_قطار_struct *q,
			       ساخت diff_options *opt غیر_مستعمل, خالی *ڈیٹا)
{
	ساخت آخری_modified *lm = ڈیٹا;

	برائے (صحیح i = 0; i < q->nr; i++) {
		ساخت diff_filepair *p = q->قطار[i];
		ساخت آخری_modified_اندراج *ent;
		ثابت حرف *راستہ = p->two->راستہ;

		FLEX_ALLOC_STR(ent, راستہ, راستہ);
		oidcpy(&ent->oid, &p->two->oid);
		اگر (lm->rev.bloom_filter_settings)
			bloom_کلید_fill(&ent->کلید, راستہ, لڑی_لمبائی(راستہ),
				       lm->rev.bloom_filter_settings);
		hashmap_اندراج_init(&ent->hashent, strhash(ent->راستہ));
		hashmap_جوڑو(&lm->paths, &ent->hashent);
	}
}

ساکن صحیح populate_paths_from_revs(ساخت آخری_modified *lm)
{
	صحیح عدد_interesting = 0, ret = 0;
	ساخت diff_options diffopt;

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
	diffopt.format_callback = جوڑو_راستہ_from_diff;
	diffopt.format_callback_ڈیٹا = lm;

	برائے (سائز_t i = 0; i < lm->rev.pending.nr; i++) {
		ساخت object_array_اندراج *obj = lm->rev.pending.objects + i;

		اگر (obj->شے->flags & UNINTERESTING)
			جاری;

		اگر (عدد_interesting++) {
			ret = error(_("last-modified can only operate on one commit at a time"));
			جاؤ برآمد;
		}

		اگر (!repo_peel_مقصد_type(lm->rev.repo, obj->راستہ, 0, obj->شے, OBJ_COMMIT)) {
			ret = error(_("revision argument '%s' is a %s, not a commit-ish"), obj->name, type_name(obj->شے->type));
			جاؤ برآمد;
		}

		diff_درخت_oid(lm->rev.repo->hash_algo->خالی_خانے_درخت,
			      &obj->شے->oid, "", &diffopt);
		diff_flush(&diffopt);
	}

برآمد:
	clear_pathspec(&diffopt.pathspec);

	واپس ret;
}

ساکن خالی آخری_modified_emit(ساخت آخری_modified *lm,
			       ثابت حرف *راستہ, ثابت ساخت commit *commit)

{
	اگر (commit->object.flags & BOUNDARY)
		حرف_لکھو('^');
	لکھو("%s\t", oid_مقصد_hex(&commit->object.oid));

	اگر (lm->nul_termination)
		لکھو("%s%c", راستہ, '\0');
	ورنہ
		write_name_quoted(راستہ, stdout, '\n');
}

ساکن خالی mark_راستہ(ثابت حرف *راستہ, ثابت ساخت object_id *oid,
		      ساخت آخری_modified_callback_ڈیٹا *ڈیٹا)
{
	ساخت آخری_modified_اندراج *ent;

	/* Is it even a path that we are interested in? */
	ent = hashmap_get_اندراج_from_hash(&ڈیٹا->lm->paths, strhash(راستہ), راستہ,
					  ساخت آخری_modified_اندراج, hashent);
	اگر (!ent)
		واپس;

	/*
	 * Is it arriving at a version of interest, or is it from a side branch
	 * which did not contribute to the final state?
	 */
	اگر (oid && !oideq(oid, &ent->oid))
		واپس;

	آخری_modified_emit(ڈیٹا->lm, راستہ, ڈیٹا->commit);

	hashmap_remove(&ڈیٹا->lm->paths, &ent->hashent, راستہ);
	bloom_کلید_clear(&ent->کلید);
	آزاد(ent);
}

ساکن خالی آخری_modified_diff(ساخت diff_قطار_struct *q,
			       ساخت diff_options *opt غیر_مستعمل, خالی *cbdata)
{
	ساخت آخری_modified_callback_ڈیٹا *ڈیٹا = cbdata;

	برائے (صحیح i = 0; i < q->nr; i++) {
		ساخت diff_filepair *p = q->قطار[i];
		انتخاب (p->حالت_2) {
		حالت DIFF_حالت_DELETED:
			/*
			 * There's no point in feeding a deletion, as it could
			 * not have resulted in our current state, which
			 * actually has the file.
			 */
			توڑو;

		طےشدہ:
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
			mark_راستہ(p->two->راستہ, &p->two->oid, ڈیٹا);
			توڑو;
		}
	}
}

ساکن خالی pass_مقصد_والد(ساخت bitmap *c,
			   ساخت bitmap *p,
			   سائز_t pos)
{
	bitmap_unset(c, pos);
	bitmap_سیٹ(p, pos);
}

ساکن bool maybe_بدلا_راستہ(ساخت آخری_modified *lm,
			       ساخت commit *origin,
			       ساخت bitmap *فعال)
{
	ساخت bloom_filter *filter;
	ساخت آخری_modified_اندراج *ent;
	ساخت hashmap_iter iter;

	اگر (!lm->rev.bloom_filter_settings)
		واپس true;

	اگر (commit_گراف_generation(origin) == GENERATION_نمبر_INFINITY)
		واپس true;

	filter = get_bloom_filter(lm->rev.repo, origin);
	اگر (!filter)
		واپس true;

	hashmap_for_each_اندراج(&lm->paths, &iter, ent, hashent) {
		اگر (فعال && !bitmap_get(فعال, ent->diff_idx))
			جاری;

		اگر (bloom_filter_شامل_ہے(filter, &ent->کلید,
					  lm->rev.bloom_filter_settings))
			واپس true;
	}
	واپس false;
}

ساکن خالی process_والد(ساخت آخری_modified *lm,
			   ساخت prio_قطار *قطار,
			   ساخت commit *c, ساخت bitmap *فعال_c,
			   ساخت commit *والد, صحیح والد_i)
{
	ساخت bitmap *فعال_p;

	اگر (repo_parse_commit(lm->rev.repo, والد))
		واپس;
	فعال_p = فعال_paths_for(lm, والد);

	/*
	 * The first time entering this function for this commit (i.e. first parent)
	 * see if Bloom filters will tell us it's worth to do the diff.
	 */
	اگر (والد_i || maybe_بدلا_راستہ(lm, c, فعال_c)) {
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
	برائے (صحیح i = 0; i < diff_queued_diff.nr; i++) {
		ساخت diff_filepair *fp = diff_queued_diff.قطار[i];
		ثابت حرف *راستہ = fp->two->راستہ;
		ساخت آخری_modified_اندراج *ent =
			hashmap_get_اندراج_from_hash(&lm->paths, strhash(راستہ), راستہ,
						    ساخت آخری_modified_اندراج, hashent);
		اگر (ent) {
			سائز_t k = ent->diff_idx;
			اگر (bitmap_get(فعال_c, k))
				bitmap_سیٹ(lm->scratch, k);
		}
	}
	برائے (سائز_t i = 0; i < lm->all_paths_nr; i++) {
		اگر (bitmap_get(فعال_c, i) && !bitmap_get(lm->scratch, i))
			pass_مقصد_والد(فعال_c, فعال_p, i);
	}

	/*
	 * If parent has any active paths, put it on the queue (if not already).
	 */
	اگر (!bitmap_is_خالی_خانے(فعال_p) && !(والد->object.flags & PARENT1)) {
		والد->object.flags |= PARENT1;
		prio_قطار_put(قطار, والد);
	}
	اگر (!(والد->object.flags & PARENT1))
		فعال_paths_free(lm, والد);

	MEMZERO_ARRAY(lm->scratch->الفاظ, lm->scratch->لفظ_alloc);
	diff_قطار_clear(&diff_queued_diff);
}

ساکن صحیح آخری_modified_تسلسل(ساخت آخری_modified *lm)
{
	صحیح max_گنتی, قطار_popped = 0;
	ساخت commit *c, *n;
	ساخت prio_قطار قطار = { compare_commits_by_gen_then_commit_date };
	ساخت prio_قطار not_قطار = { compare_commits_by_gen_then_commit_date };
	ساخت commit_list *list;
	ساخت آخری_modified_callback_ڈیٹا ڈیٹا = { .lm = lm };

	lm->rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	lm->rev.diffopt.format_callback = آخری_modified_diff;
	lm->rev.diffopt.format_callback_ڈیٹا = &ڈیٹا;
	lm->rev.no_walk = 1;

	prepare_revision_walk(&lm->rev);

	max_گنتی = lm->rev.max_گنتی;

	init_فعال_paths_for_commit(&lm->فعال_paths);
	lm->scratch = bitmap_لفظ_alloc(lm->all_paths_nr);

	/*
	 * lm->rev.commits holds the set of boundary commits for our walk.
	 *
	 * Loop through each such commit, and place it in the appropriate queue.
	 */
	برائے (list = lm->rev.commits; list; list = list->اگلا) {
		ساخت commit *c = list->شے;

		اگر (c->object.flags & BOTTOM) {
			prio_قطار_put(&not_قطار, c);
			c->object.flags |= PARENT2;
		} ورنہ اگر (!(c->object.flags & PARENT1)) {
			/*
			 * If the commit is a starting point (and hasn't been
			 * seen yet), then initialize the set of interesting
			 * paths, too.
			 */
			ساخت bitmap *فعال;

			prio_قطار_put(&قطار, c);
			c->object.flags |= PARENT1;

			فعال = فعال_paths_for(lm, c);
			برائے (سائز_t i = 0; i < lm->all_paths_nr; i++)
				bitmap_سیٹ(فعال, i);
		}
	}

	جب ((c = prio_قطار_get(&قطار))) {
		صحیح والد_i;
		ساخت commit_list *p;
		ساخت bitmap *فعال_c = فعال_paths_for(lm, c);

		اگر ((0 <= max_گنتی && max_گنتی < ++قطار_popped) ||
		    (c->object.flags & PARENT2)) {
			/*
			 * Either a boundary commit, or we have already seen too
			 * many others. Either way, stop here.
			 */
			c->object.flags |= PARENT2 | BOUNDARY;
			ڈیٹا.commit = c;
			diff_درخت_oid(lm->rev.repo->hash_algo->خالی_خانے_درخت,
				      &c->object.oid,
				      "", &lm->rev.diffopt);
			diff_flush(&lm->rev.diffopt);
			جاؤ cleanup;
		}

		/*
		 * Otherwise, make sure that 'c' isn't reachable from anything
		 * in the '--not' queue.
		 */
		اگر (repo_parse_commit(lm->rev.repo, c))
			جاؤ cleanup;

		جب ((n = prio_قطار_get(&not_قطار))) {
			ساخت commit_list *np;

			اگر (repo_parse_commit(lm->rev.repo, n))
				جاری;

			برائے (np = n->parents; np; np = np->اگلا) {
				اگر (!(np->شے->object.flags & PARENT2)) {
					prio_قطار_put(&not_قطار, np->شے);
					np->شے->object.flags |= PARENT2;
				}
			}

			اگر (commit_گراف_generation(n) < commit_گراف_generation(c))
				توڑو;
		}

		/*
		 * Look at each parent and pass on each path that's TREESAME
		 * with that parent. Stop early when no active paths remain.
		 */
		برائے (p = c->parents, والد_i = 0; p; p = p->اگلا, والد_i++) {
			process_والد(lm, &قطار,
				       c, فعال_c,
				       p->شے, والد_i);

			اگر (bitmap_is_خالی_خانے(فعال_c))
				توڑو;
		}

		/*
		 * Paths that remain active, or not TREESAME with any parent,
		 * were changed by 'c'.
		 */
		اگر (!bitmap_is_خالی_خانے(فعال_c))  {
			ڈیٹا.commit = c;
			برائے (سائز_t i = 0; i < lm->all_paths_nr; i++) {
				اگر (bitmap_get(فعال_c, i))
					mark_راستہ(lm->all_paths[i], تہی, &ڈیٹا);
			}
		}

cleanup:
		فعال_paths_free(lm, c);
	}

	اگر (hashmap_get_سائز(&lm->paths))
		BUG("paths remaining beyond boundary in last-modified");

	clear_prio_قطار(&not_قطار);
	clear_prio_قطار(&قطار);
	clear_فعال_paths_for_commit(&lm->فعال_paths);
	bitmap_free(lm->scratch);

	واپس 0;
}

ساکن صحیح آخری_modified_init(ساخت آخری_modified *lm, ساخت repository *r,
			      ثابت حرف *سابقہ, صحیح argc, ثابت حرف **argv)
{
	ساخت hashmap_iter iter;
	ساخت آخری_modified_اندراج *ent;

	hashmap_init(&lm->paths, آخری_modified_اندراج_hashcmp, تہی, 0);

	repo_init_revisions(r, &lm->rev, سابقہ);
	lm->rev.def = "HEAD";
	lm->rev.combine_merges = 1;
	lm->rev.show_جڑ_diff = 1;
	lm->rev.boundary = 1;
	lm->rev.no_commit_id = 1;
	lm->rev.diff = 1;
	lm->rev.diffopt.flags.no_recursive_diff_درخت_combined = 1;
	lm->rev.diffopt.flags.recursive = 1;
	lm->rev.diffopt.flags.درخت_اندر_recursive = lm->show_trees;
	lm->rev.diffopt.max_گہرائی = lm->max_گہرائی;
	lm->rev.diffopt.max_گہرائی_valid = lm->max_گہرائی >= 0;

	argc = تیاری_revisions(argc, argv, &lm->rev, تہی);
	اگر (argc > 1) {
		error(_("unknown last-modified argument: %s"), argv[1]);
		واپس argc;
	}

	lm->rev.bloom_filter_settings = get_bloom_filter_settings(lm->rev.repo);

	اگر (populate_paths_from_revs(lm) < 0)
		واپس -1;

	CALLOC_ARRAY(lm->all_paths, hashmap_get_سائز(&lm->paths));
	lm->all_paths_nr = 0;
	hashmap_for_each_اندراج(&lm->paths, &iter, ent, hashent) {
		ent->diff_idx = lm->all_paths_nr++;
		lm->all_paths[ent->diff_idx] = ent->راستہ;
	}

	واپس 0;
}

صحیح cmd_آخری_modified(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		      ساخت repository *repo)
{
	صحیح ret;
	ساخت آخری_modified lm = { 0 };

	ثابت حرف * ثابت آخری_modified_usage[] = {
		N_("git last-modified [--recursive] [--show-trees] [--max-depth=<depth>] [-z]\n"
		   "                  [<revision-range>] [[--] <pathspec>...]"),
		تہی
	};

	ساخت option آخری_modified_options[] = {
		OPT_سیٹ_INT('r', "recursive", &lm.max_گہرائی,
			    N_("recurse into subtrees"), -1),
		OPT_BOOL('t', "show-trees", &lm.show_trees,
			 N_("show tree entries when recursing into subtrees")),
		OPT_INTEGER_F(0, "max-depth", &lm.max_گہرائی,
			      N_("maximum tree depth to recurse"), PARSE_OPT_NONEG),
		OPT_BOOL('z', تہی, &lm.nul_termination,
			 N_("lines are separated with NUL character")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, آخری_modified_options,
			     آخری_modified_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_config(repo, git_default_config, تہی);

	ret = آخری_modified_init(&lm, repo, سابقہ, argc, argv);
	اگر (ret > 0)
		usage_with_options(آخری_modified_usage,
				   آخری_modified_options);
	اگر (ret)
		جاؤ برآمد;

	ret = آخری_modified_تسلسل(&lm);
	اگر (ret)
		جاؤ برآمد;

برآمد:
	آخری_modified_release(&lm);

	واپس ret;
}
