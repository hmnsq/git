/* We need this macro to access core_apply_sparse_checkout */
#define USE_THE_REPOSITORY_VARIABLE

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
	size_t min_batch_size;
	صحيح sparse;
	صحيح include_edges;
	بنية rev_info revs;
};

ساكن فراغ backfill_context_clear(بنية backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

ساكن فراغ download_batch(بنية backfill_context *ctx)
{
	promisor_remote_get_direct(ctx->repo,
				   ctx->current_batch.oid,
				   ctx->current_batch.nr);
	oid_array_clear(&ctx->current_batch);

	/*
	 * We likely have a new packfile. Add it to the packed list to
	 * avoid possible duplicate downloads of the same objects.
	 */
	odb_reprepare(ctx->repo->objects);
}

ساكن صحيح fill_missing_blobs(ثابت حرف *path UNUSED,
			      بنية oid_array *list,
			      تعداد object_type type,
			      فراغ *data)
{
	بنية backfill_context *ctx = data;

	إذا (type != OBJ_BLOB)
		أرجع 0;

	لكل (size_t i = 0; i < list->nr; i++) {
		إذا (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	إذا (ctx->current_batch.nr >= ctx->min_batch_size)
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
	إذا (revs->لي_level_traverse)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "-L");
	إذا (revs->explicit_diff_merges)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-merges");
	إذا (!path_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	إذا (revs->filter.blob_limit_درج)
		die(_("cannot backfill with blob size limits"));
}

ساكن صحيح do_backfill(بنية backfill_context *ctx)
{
	بنية path_walk_info info = PATH_WALK_INFO_INIT;
	صحيح ret;

	إذا (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		إذا (get_sparse_checkout_patterns(info.pl)) {
			path_walk_info_clear(&info);
			أرجع error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	إذا (!ctx->revs.pending.nr)
		add_head_to_pending(&ctx->revs);
	إذا (ctx->include_edges)
		ctx->revs.edge_hint = 1;

	info.blobs = 1;
	info.tags = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.path_fn = fill_missing_blobs;
	info.path_fn_data = ctx;

	ret = walk_objects_by_path(&info);

	/* Download the objects that did not fill a batch. */
	إذا (!ret)
		download_batch(ctx);

	path_walk_info_clear(&info);
	أرجع ret;
}

صحيح cmd_backfill(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix, بنية repository *repo)
{
	صحيح result;
	بنية backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_size = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_edges = 1,
	};
	بنية option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_size,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_edges,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_END(),
	};
	بنية repo_config_درجوجي *cfg = repo_config_درجوجي(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, prefix, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, prefix);
	argc = setup_revisions(argc, argv, &ctx.revs, عدم);

	إذا (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, عدم);

	إذا (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	result = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	أرجع result;
}
