/* We need this macro to access core_apply_sparse_checkout */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "git-compat-util.h"
#شامل "config.h"
#شامل "parse-options.h"
#شامل "repository.h"
#شامل "commit.h"
#شامل "dir.h"
#شامل "environment.h"
#شامل "hex.h"
#شامل "tree.h"
#شامل "tree-walk.h"
#شامل "object.h"
#شامل "odb.h"
#شامل "oid-array.h"
#شامل "oidset.h"
#شامل "promisor-remote.h"
#شامل "strmap.h"
#شامل "string-list.h"
#شامل "revision.h"
#شامل "trace2.h"
#شامل "progress.h"
#شامل "packfile.h"
#شامل "path-walk.h"

ساکن ثابت حرف * ثابت builtin_backfill_usage[] = {
	N_("git backfill [--min-batch-size=<n>] [--[no-]sparse] [--[no-]include-edges] [<revision-range>]"),
	تہی
};

ساخت backfill_context {
	ساخت repository *repo;
	ساخت oid_array current_batch;
	سائز_t min_batch_سائز;
	صحیح sparse;
	صحیح include_کنارے;
	ساخت rev_info revs;
};

ساکن خالی backfill_context_clear(ساخت backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

ساکن خالی download_batch(ساخت backfill_context *ctx)
{
	promisor_remote_get_براہ_راست(ctx->repo,
				   ctx->current_batch.oid,
				   ctx->current_batch.nr);
	oid_array_clear(&ctx->current_batch);

	/*
	 * We likely have a new packfile. Add it to the packed list to
	 * avoid possible duplicate downloads of the same objects.
	 */
	odb_reprepare(ctx->repo->objects);
}

ساکن صحیح fill_missing_blobs(ثابت حرف *راستہ غیر_مستعمل,
			      ساخت oid_array *list,
			      شمار object_type type,
			      خالی *ڈیٹا)
{
	ساخت backfill_context *ctx = ڈیٹا;

	اگر (type != OBJ_BLOB)
		واپس 0;

	برائے (سائز_t i = 0; i < list->nr; i++) {
		اگر (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	اگر (ctx->current_batch.nr >= ctx->min_batch_سائز)
		download_batch(ctx);

	واپس 0;
}

ساکن خالی reject_unsupported_rev_list_options(ساخت rev_info *revs)
{
	اگر (revs->diffopt.pickaxe)
		die(_("'%s' cannot be used with 'git backfill'"),
		    (revs->diffopt.pickaxe_opts & DIFF_PICKAXE_REGEX) ? "-G" : "-S");
	اگر (revs->diffopt.filter || revs->diffopt.filter_not)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-filter");
	اگر (revs->diffopt.flags.follow_renames)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--follow");
	اگر (revs->سطر_level_traverse)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "-L");
	اگر (revs->explicit_diff_merges)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-merges");
	اگر (!راستہ_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	اگر (revs->filter.blob_حد_بندی_قدر)
		die(_("cannot backfill with blob size limits"));
}

ساکن صحیح do_backfill(ساخت backfill_context *ctx)
{
	ساخت راستہ_walk_info info = راستہ_WALK_INFO_INIT;
	صحیح ret;

	اگر (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		اگر (get_sparse_checkout_patterns(info.pl)) {
			راستہ_walk_info_clear(&info);
			واپس error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	اگر (!ctx->revs.pending.nr)
		جوڑو_سر_مقصد_pending(&ctx->revs);
	اگر (ctx->include_کنارے)
		ctx->revs.کنارہ_hint = 1;

	info.blobs = 1;
	info.لیبل_فہرست = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.راستہ_fn = fill_missing_blobs;
	info.راستہ_fn_ڈیٹا = ctx;

	ret = walk_objects_by_راستہ(&info);

	/* Download the objects that did not fill a batch. */
	اگر (!ret)
		download_batch(ctx);

	راستہ_walk_info_clear(&info);
	واپس ret;
}

صحیح cmd_backfill(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ, ساخت repository *repo)
{
	صحیح نتیجہ;
	ساخت backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_سائز = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_کنارے = 1,
	};
	ساخت option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_سائز,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_کنارے,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_انتہا(),
	};
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, سابقہ, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, سابقہ);
	argc = تیاری_revisions(argc, argv, &ctx.revs, تہی);

	اگر (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, تہی);

	اگر (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	نتیجہ = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	واپس نتیجہ;
}
