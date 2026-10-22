/* We need this macro to access core_apply_sparse_checkout */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "git-compat-util.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repository.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oidset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "promisor-remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "progress.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path-walk.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_backfill_usage[] = {
	N_("git backfill [--min-batch-size=<n>] [--[no-]sparse] [--[no-]include-edges] [<revision-range>]"),
	ਨਿਰਾ
};

ਬਣਤਰ backfill_context {
	ਬਣਤਰ repository *repo;
	ਬਣਤਰ oid_array current_batch;
	ਮਾਪ_t min_batch_ਮਾਪ;
	ਪੂਰਨ sparse;
	ਪੂਰਨ include_ਕਿਨਾਰੇ;
	ਬਣਤਰ rev_info revs;
};

ਸਥਿਰ ਖਾਲੀ backfill_context_clear(ਬਣਤਰ backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

ਸਥਿਰ ਖਾਲੀ download_batch(ਬਣਤਰ backfill_context *ctx)
{
	promisor_remote_get_ਸਿੱਧਾ(ctx->repo,
				   ctx->current_batch.oid,
				   ctx->current_batch.nr);
	oid_array_clear(&ctx->current_batch);

	/*
	 * We likely have a new packfile. Add it to the packed list to
	 * avoid possible duplicate downloads of the same objects.
	 */
	odb_reprepare(ctx->repo->objects);
}

ਸਥਿਰ ਪੂਰਨ fill_missing_blobs(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ ਅਣਵਰਤਿਆ,
			      ਬਣਤਰ oid_array *list,
			      ਗਿਣਤੀ object_type type,
			      ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ backfill_context *ctx = ਅੰਕੜੇ;

	ਜੇ (type != OBJ_BLOB)
		ਵਾਪਸ 0;

	ਲਈ (ਮਾਪ_t i = 0; i < list->nr; i++) {
		ਜੇ (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	ਜੇ (ctx->current_batch.nr >= ctx->min_batch_ਮਾਪ)
		download_batch(ctx);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ reject_unsupported_rev_list_options(ਬਣਤਰ rev_info *revs)
{
	ਜੇ (revs->diffopt.pickaxe)
		die(_("'%s' cannot be used with 'git backfill'"),
		    (revs->diffopt.pickaxe_opts & DIFF_PICKAXE_REGEX) ? "-G" : "-S");
	ਜੇ (revs->diffopt.filter || revs->diffopt.filter_not)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-filter");
	ਜੇ (revs->diffopt.flags.follow_renames)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--follow");
	ਜੇ (revs->ਸਤਰ_level_traverse)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "-L");
	ਜੇ (revs->explicit_diff_merges)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-merges");
	ਜੇ (!ਰਸਤਾ_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	ਜੇ (revs->filter.blob_ਹੱਦਬੰਦੀ_ਮੁੱਲ)
		die(_("cannot backfill with blob size limits"));
}

ਸਥਿਰ ਪੂਰਨ do_backfill(ਬਣਤਰ backfill_context *ctx)
{
	ਬਣਤਰ ਰਸਤਾ_walk_info info = ਰਸਤਾ_WALK_INFO_INIT;
	ਪੂਰਨ ret;

	ਜੇ (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		ਜੇ (get_sparse_checkout_patterns(info.pl)) {
			ਰਸਤਾ_walk_info_clear(&info);
			ਵਾਪਸ error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	ਜੇ (!ctx->revs.pending.nr)
		ਜੋੜੋ_ਸਿਰ_ਮੰਜ਼ਿਲ_pending(&ctx->revs);
	ਜੇ (ctx->include_ਕਿਨਾਰੇ)
		ctx->revs.ਕਿਨਾਰਾ_hint = 1;

	info.blobs = 1;
	info.ਟੈਗ_ਸੂਚੀ = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.ਰਸਤਾ_fn = fill_missing_blobs;
	info.ਰਸਤਾ_fn_ਅੰਕੜੇ = ctx;

	ret = walk_objects_by_ਰਸਤਾ(&info);

	/* Download the objects that did not fill a batch. */
	ਜੇ (!ret)
		download_batch(ctx);

	ਰਸਤਾ_walk_info_clear(&info);
	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_backfill(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ ਨਤੀਜਾ;
	ਬਣਤਰ backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_ਮਾਪ = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_ਕਿਨਾਰੇ = 1,
	};
	ਬਣਤਰ option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_ਮਾਪ,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_ਕਿਨਾਰੇ,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, ਅਗੇਤਰ);
	argc = ਤਿਆਰੀ_revisions(argc, argv, &ctx.revs, ਨਿਰਾ);

	ਜੇ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, ਨਿਰਾ);

	ਜੇ (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	ਨਤੀਜਾ = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	ਵਾਪਸ ਨਤੀਜਾ;
}
