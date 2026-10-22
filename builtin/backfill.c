/* We need this macro to access core_apply_sparse_checkout */
#define USE_THE_REPOSITORY_VARIABLE

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

ایستا ثابت نویسه * ثابت builtin_backfill_usage[] = {
	N_("git backfill [--min-batch-size=<n>] [--[no-]sparse] [--[no-]include-edges] [<revision-range>]"),
	پوچ
};

ساختار backfill_context {
	ساختار repository *repo;
	ساختار oid_array current_batch;
	size_t min_batch_size;
	صحیح sparse;
	صحیح include_edges;
	ساختار rev_info revs;
};

ایستا تهی backfill_context_clear(ساختار backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

ایستا تهی download_batch(ساختار backfill_context *ctx)
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

ایستا صحیح fill_missing_blobs(ثابت نویسه *path UNUSED,
			      ساختار oid_array *list,
			      شمارش object_type type,
			      تهی *data)
{
	ساختار backfill_context *ctx = data;

	اگر (type != OBJ_BLOB)
		بازگشت 0;

	برای (size_t i = 0; i < list->nr; i++) {
		اگر (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	اگر (ctx->current_batch.nr >= ctx->min_batch_size)
		download_batch(ctx);

	بازگشت 0;
}

ایستا تهی reject_unsupported_rev_list_options(ساختار rev_info *revs)
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
	اگر (!path_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	اگر (revs->filter.blob_limit_مقدار)
		die(_("cannot backfill with blob size limits"));
}

ایستا صحیح do_backfill(ساختار backfill_context *ctx)
{
	ساختار path_walk_info info = PATH_WALK_INFO_INIT;
	صحیح ret;

	اگر (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		اگر (get_sparse_checkout_patterns(info.pl)) {
			path_walk_info_clear(&info);
			بازگشت error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	اگر (!ctx->revs.pending.nr)
		add_head_to_pending(&ctx->revs);
	اگر (ctx->include_edges)
		ctx->revs.edge_hint = 1;

	info.blobs = 1;
	info.tags = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.path_fn = fill_missing_blobs;
	info.path_fn_data = ctx;

	ret = walk_objects_by_path(&info);

	/* Download the objects that did not fill a batch. */
	اگر (!ret)
		download_batch(ctx);

	path_walk_info_clear(&info);
	بازگشت ret;
}

صحیح cmd_backfill(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix, ساختار repository *repo)
{
	صحیح result;
	ساختار backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_size = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_edges = 1,
	};
	ساختار option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_size,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_edges,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_END(),
	};
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, prefix, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, prefix);
	argc = setup_revisions(argc, argv, &ctx.revs, پوچ);

	اگر (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, پوچ);

	اگر (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	result = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	بازگشت result;
}
