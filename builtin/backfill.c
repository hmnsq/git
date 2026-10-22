/* We need this macro to access core_apply_sparse_checkout */
#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "git-compat-util.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "repository.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "oidset.h"
#அந்தர்பாவ "promisor-remote.h"
#அந்தர்பாவ "strmap.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "trace2.h"
#அந்தர்பாவ "progress.h"
#அந்தர்பாவ "packfile.h"
#அந்தர்பாவ "path-walk.h"

நிலையான மாறிலி எழுத்து * மாறிலி builtin_backfill_usage[] = {
	N_("git backfill [--min-batch-size=<n>] [--[no-]sparse] [--[no-]include-edges] [<revision-range>]"),
	வெற்று
};

கட்டமைப்பு backfill_context {
	கட்டமைப்பு repository *repo;
	கட்டமைப்பு oid_array current_batch;
	size_t min_batch_size;
	முழு sparse;
	முழு include_edges;
	கட்டமைப்பு rev_info revs;
};

நிலையான வெறுமை backfill_context_clear(கட்டமைப்பு backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

நிலையான வெறுமை download_batch(கட்டமைப்பு backfill_context *ctx)
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

நிலையான முழு fill_missing_blobs(மாறிலி எழுத்து *path UNUSED,
			      கட்டமைப்பு oid_array *list,
			      எண்ணிக்கை object_type type,
			      வெறுமை *data)
{
	கட்டமைப்பு backfill_context *ctx = data;

	எனில் (type != OBJ_BLOB)
		திரும்பு 0;

	க்கு (size_t i = 0; i < list->nr; i++) {
		எனில் (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	எனில் (ctx->current_batch.nr >= ctx->min_batch_size)
		download_batch(ctx);

	திரும்பு 0;
}

நிலையான வெறுமை reject_unsupported_rev_list_options(கட்டமைப்பு rev_info *revs)
{
	எனில் (revs->diffopt.pickaxe)
		die(_("'%s' cannot be used with 'git backfill'"),
		    (revs->diffopt.pickaxe_opts & DIFF_PICKAXE_REGEX) ? "-G" : "-S");
	எனில் (revs->diffopt.filter || revs->diffopt.filter_not)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-filter");
	எனில் (revs->diffopt.flags.follow_renames)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--follow");
	எனில் (revs->வரி_level_traverse)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "-L");
	எனில் (revs->explicit_diff_merges)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-merges");
	எனில் (!path_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	எனில் (revs->filter.blob_limit_மதிப்பு)
		die(_("cannot backfill with blob size limits"));
}

நிலையான முழு do_backfill(கட்டமைப்பு backfill_context *ctx)
{
	கட்டமைப்பு path_walk_info info = PATH_WALK_INFO_INIT;
	முழு ret;

	எனில் (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		எனில் (get_sparse_checkout_patterns(info.pl)) {
			path_walk_info_clear(&info);
			திரும்பு error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	எனில் (!ctx->revs.pending.nr)
		add_head_to_pending(&ctx->revs);
	எனில் (ctx->include_edges)
		ctx->revs.edge_hint = 1;

	info.blobs = 1;
	info.tags = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.path_fn = fill_missing_blobs;
	info.path_fn_data = ctx;

	ret = walk_objects_by_path(&info);

	/* Download the objects that did not fill a batch. */
	எனில் (!ret)
		download_batch(ctx);

	path_walk_info_clear(&info);
	திரும்பு ret;
}

முழு cmd_backfill(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix, கட்டமைப்பு repository *repo)
{
	முழு result;
	கட்டமைப்பு backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_size = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_edges = 1,
	};
	கட்டமைப்பு option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_size,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_edges,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_END(),
	};
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, prefix, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, prefix);
	argc = setup_revisions(argc, argv, &ctx.revs, வெற்று);

	எனில் (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, வெற்று);

	எனில் (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	result = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	திரும்பு result;
}
