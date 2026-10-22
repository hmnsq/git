/* We need this macro to access core_apply_sparse_checkout */
#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "git-compat-util.h"
#समावेश "config.h"
#समावेश "parse-options.h"
#समावेश "repository.h"
#समावेश "commit.h"
#समावेश "dir.h"
#समावेश "environment.h"
#समावेश "hex.h"
#समावेश "tree.h"
#समावेश "tree-walk.h"
#समावेश "object.h"
#समावेश "odb.h"
#समावेश "oid-array.h"
#समावेश "oidset.h"
#समावेश "promisor-remote.h"
#समावेश "strmap.h"
#समावेश "string-list.h"
#समावेश "revision.h"
#समावेश "trace2.h"
#समावेश "progress.h"
#समावेश "packfile.h"
#समावेश "path-walk.h"

अटल स्थिर अक्षर * स्थिर builtin_backfill_usage[] = {
	N_("git backfill [--min-batch-size=<n>] [--[no-]sparse] [--[no-]include-edges] [<revision-range>]"),
	शून्य
};

काष्ठा backfill_context {
	काष्ठा repository *repo;
	काष्ठा oid_array current_batch;
	माप_प्रकार min_batch_आकार;
	पूर्णांक sparse;
	पूर्णांक include_edges;
	काष्ठा rev_info revs;
};

अटल व्योम backfill_context_clear(काष्ठा backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

अटल व्योम download_batch(काष्ठा backfill_context *ctx)
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

अटल पूर्णांक fill_missing_blobs(स्थिर अक्षर *path UNUSED,
			      काष्ठा oid_array *list,
			      क्रमागत object_type type,
			      व्योम *तथ्य)
{
	काष्ठा backfill_context *ctx = तथ्य;

	अगर (type != OBJ_BLOB)
		वापस 0;

	क्रम (माप_प्रकार i = 0; i < list->nr; i++) {
		अगर (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	अगर (ctx->current_batch.nr >= ctx->min_batch_आकार)
		download_batch(ctx);

	वापस 0;
}

अटल व्योम reject_unsupported_rev_list_options(काष्ठा rev_info *revs)
{
	अगर (revs->diffopt.pickaxe)
		die(_("'%s' cannot be used with 'git backfill'"),
		    (revs->diffopt.pickaxe_opts & DIFF_PICKAXE_REGEX) ? "-G" : "-S");
	अगर (revs->diffopt.filter || revs->diffopt.filter_not)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-filter");
	अगर (revs->diffopt.flags.follow_renames)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--follow");
	अगर (revs->पंक्ति_level_traverse)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "-L");
	अगर (revs->explicit_diff_merges)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-merges");
	अगर (!path_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	अगर (revs->filter.blob_limit_मूल्य)
		die(_("cannot backfill with blob size limits"));
}

अटल पूर्णांक do_backfill(काष्ठा backfill_context *ctx)
{
	काष्ठा path_walk_info info = PATH_WALK_INFO_INIT;
	पूर्णांक ret;

	अगर (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		अगर (get_sparse_checkout_patterns(info.pl)) {
			path_walk_info_clear(&info);
			वापस error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	अगर (!ctx->revs.pending.nr)
		add_शीर्ष_to_pending(&ctx->revs);
	अगर (ctx->include_edges)
		ctx->revs.edge_hint = 1;

	info.blobs = 1;
	info.tags = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.path_fn = fill_missing_blobs;
	info.path_fn_तथ्य = ctx;

	ret = walk_objects_by_path(&info);

	/* Download the objects that did not fill a batch. */
	अगर (!ret)
		download_batch(ctx);

	path_walk_info_clear(&info);
	वापस ret;
}

पूर्णांक cmd_backfill(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix, काष्ठा repository *repo)
{
	पूर्णांक परिणाम;
	काष्ठा backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_आकार = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_edges = 1,
	};
	काष्ठा option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_आकार,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_edges,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_END(),
	};
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, prefix, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, prefix);
	argc = setup_revisions(argc, argv, &ctx.revs, शून्य);

	अगर (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, शून्य);

	अगर (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	परिणाम = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	वापस परिणाम;
}
