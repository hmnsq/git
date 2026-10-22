/* We need this macro to access core_apply_sparse_checkout */
#تعریف_کن USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "git-compat-util.h"
#ضمن "config.h"
#ضمن "parse-options.h"
#ضمن "repository.h"
#ضمن "commit.h"
#ضمن "dir.h"
#ضمن "environment.h"
#ضمن "hex.h"
#ضمن "tree.h"
#ضمن "tree-walk.h"
#ضمن "object.h"
#ضمن "odb.h"
#ضمن "oid-array.h"
#ضمن "oidset.h"
#ضمن "promisor-remote.h"
#ضمن "strmap.h"
#ضمن "string-list.h"
#ضمن "revision.h"
#ضمن "trace2.h"
#ضمن "progress.h"
#ضمن "packfile.h"
#ضمن "path-walk.h"

ساكن ثابت حرف * ثابت builtin_backfill_usage[] = {
	N_("git backfill [--min-batch-size=<n>] [--[no-]sparse] [--[no-]include-edges] [<revision-range>]"),
	عدم
};

بنية backfill_context {
	بنية repository *repo;
	بنية oid_array current_batch;
	اندازه_t min_batch_اندازه;
	صحيح sparse;
	صحيح include_یالها;
	بنية rev_info revs;
};

ساكن فراغ backfill_context_clear(بنية backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

ساكن فراغ download_batch(بنية backfill_context *ctx)
{
	promisor_remote_get_مستقیم(ctx->repo,
				   ctx->current_batch.oid,
				   ctx->current_batch.nr);
	oid_array_clear(&ctx->current_batch);

	/*
	 * We likely have a new packfile. Add it to the packed list to
	 * avoid possible duplicate downloads of the same objects.
	 */
	odb_reprepare(ctx->repo->objects);
}

ساكن صحيح fill_missing_blobs(ثابت حرف *مسیر استفادهنشده,
			      بنية oid_array *list,
			      تعداد object_type type,
			      فراغ *داده)
{
	بنية backfill_context *ctx = داده;

	إذا (type != OBJ_BLOB)
		أرجع 0;

	لكل (اندازه_t i = 0; i < list->nr; i++) {
		إذا (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	إذا (ctx->current_batch.nr >= ctx->min_batch_اندازه)
		download_batch(ctx);

	أرجع 0;
}

ساكن فراغ reject_unsupported_rev_list_options(بنية rev_info *revs)
{
	إذا (revs->diffopt.pickaxe)
		die(_("'%s' cannot be used with 'git backfill'"),
		    (revs->diffopt.pickaxe_opts & DIFF_PICKAXE_REGEX) ? "-G" : "-S");
	إذا (revs->diffopt.filter || revs->diffopt.filter_not)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-filter");
	إذا (revs->diffopt.flags.follow_renames)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--follow");
	إذا (revs->سطر_level_traverse)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "-L");
	إذا (revs->explicit_diff_merges)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-merges");
	إذا (!مسیر_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	إذا (revs->filter.blob_حد_مقدار)
		die(_("cannot backfill with blob size limits"));
}

ساكن صحيح do_backfill(بنية backfill_context *ctx)
{
	بنية مسیر_walk_info info = مسیر_WALK_INFO_INIT;
	صحيح ret;

	إذا (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		إذا (get_sparse_checkout_patterns(info.pl)) {
			مسیر_walk_info_clear(&info);
			أرجع error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	إذا (!ctx->revs.pending.nr)
		جمع_کن_سر_مقصد_pending(&ctx->revs);
	إذا (ctx->include_یالها)
		ctx->revs.یال_hint = 1;

	info.blobs = 1;
	info.برچسبها = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.مسیر_fn = fill_missing_blobs;
	info.مسیر_fn_داده = ctx;

	ret = walk_objects_by_مسیر(&info);

	/* Download the objects that did not fill a batch. */
	إذا (!ret)
		download_batch(ctx);

	مسیر_walk_info_clear(&info);
	أرجع ret;
}

صحيح cmd_backfill(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند, بنية repository *repo)
{
	صحيح نتیجه;
	بنية backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_اندازه = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_یالها = 1,
	};
	بنية option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_اندازه,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_یالها,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_انتها(),
	};
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, پیشوند, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, پیشوند);
	argc = راهاندازی_revisions(argc, argv, &ctx.revs, عدم);

	إذا (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, عدم);

	إذا (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	نتیجه = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	أرجع نتیجه;
}
