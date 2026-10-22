/* We need this macro to access core_apply_sparse_checkout */
#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "git-compat-util.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "repository.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "tree.h"
#অন্তর্ভাব "tree-walk.h"
#অন্তর্ভাব "object.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "oidset.h"
#অন্তর্ভাব "promisor-remote.h"
#অন্তর্ভাব "strmap.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "trace2.h"
#অন্তর্ভাব "progress.h"
#অন্তর্ভাব "packfile.h"
#অন্তর্ভাব "path-walk.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_backfill_usage[] = {
	N_("git backfill [--min-batch-size=<n>] [--[no-]sparse] [--[no-]include-edges] [<revision-range>]"),
	ফাঁকা
};

কাঠামো backfill_context {
	কাঠামো repository *repo;
	কাঠামো oid_array current_batch;
	আকার_t min_batch_আকার;
	পূর্ণসংখ্যা sparse;
	পূর্ণসংখ্যা include_edges;
	কাঠামো rev_info revs;
};

স্থির শূন্য backfill_context_clear(কাঠামো backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

স্থির শূন্য download_batch(কাঠামো backfill_context *ctx)
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

স্থির পূর্ণসংখ্যা fill_missing_blobs(ধ্রুবক অক্ষর *path UNUSED,
			      কাঠামো oid_array *list,
			      গণনা object_type type,
			      শূন্য *তথ্য)
{
	কাঠামো backfill_context *ctx = তথ্য;

	যদি (type != OBJ_BLOB)
		ফেরত 0;

	জন্য (আকার_t i = 0; i < list->nr; i++) {
		যদি (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	যদি (ctx->current_batch.nr >= ctx->min_batch_আকার)
		download_batch(ctx);

	ফেরত 0;
}

স্থির শূন্য reject_unsupported_rev_list_options(কাঠামো rev_info *revs)
{
	যদি (revs->diffopt.pickaxe)
		die(_("'%s' cannot be used with 'git backfill'"),
		    (revs->diffopt.pickaxe_opts & DIFF_PICKAXE_REGEX) ? "-G" : "-S");
	যদি (revs->diffopt.filter || revs->diffopt.filter_not)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-filter");
	যদি (revs->diffopt.flags.follow_renames)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--follow");
	যদি (revs->সারি_level_traverse)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "-L");
	যদি (revs->explicit_diff_merges)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-merges");
	যদি (!path_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	যদি (revs->filter.blob_limit_মান)
		die(_("cannot backfill with blob size limits"));
}

স্থির পূর্ণসংখ্যা do_backfill(কাঠামো backfill_context *ctx)
{
	কাঠামো path_walk_info info = PATH_WALK_INFO_INIT;
	পূর্ণসংখ্যা ret;

	যদি (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		যদি (get_sparse_checkout_patterns(info.pl)) {
			path_walk_info_clear(&info);
			ফেরত error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	যদি (!ctx->revs.pending.nr)
		add_শীর্ষ_to_pending(&ctx->revs);
	যদি (ctx->include_edges)
		ctx->revs.edge_hint = 1;

	info.blobs = 1;
	info.tags = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.path_fn = fill_missing_blobs;
	info.path_fn_তথ্য = ctx;

	ret = walk_objects_by_path(&info);

	/* Download the objects that did not fill a batch. */
	যদি (!ret)
		download_batch(ctx);

	path_walk_info_clear(&info);
	ফেরত ret;
}

পূর্ণসংখ্যা cmd_backfill(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix, কাঠামো repository *repo)
{
	পূর্ণসংখ্যা ফলাফল;
	কাঠামো backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_আকার = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_edges = 1,
	};
	কাঠামো option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_আকার,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_edges,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_END(),
	};
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, prefix, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, prefix);
	argc = setup_revisions(argc, argv, &ctx.revs, ফাঁকা);

	যদি (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, ফাঁকা);

	যদি (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	ফলাফল = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	ফেরত ফলাফল;
}
