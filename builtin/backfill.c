/* We need this macro to access core_apply_sparse_checkout */
#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "git-compat-util.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "repository.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "oidset.h"
#ಅನ್ತರ್ಭಾವ "promisor-remote.h"
#ಅನ್ತರ್ಭಾವ "strmap.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"
#ಅನ್ತರ್ಭಾವ "progress.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "path-walk.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_backfill_usage[] = {
	N_("git backfill [--min-batch-size=<n>] [--[no-]sparse] [--[no-]include-edges] [<revision-range>]"),
	ಖಾಲಿ
};

ರಚನೆ backfill_context {
	ರಚನೆ repository *repo;
	ರಚನೆ oid_array current_batch;
	ಗಾತ್ರ_t min_batch_ಗಾತ್ರ;
	ಪೂರ್ಣಾಂಕ sparse;
	ಪೂರ್ಣಾಂಕ include_ಅಂಚುಗಳು;
	ರಚನೆ rev_info revs;
};

ಸ್ಥಿರ ಶೂನ್ಯ backfill_context_clear(ರಚನೆ backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

ಸ್ಥಿರ ಶೂನ್ಯ download_batch(ರಚನೆ backfill_context *ctx)
{
	promisor_remote_get_ನೇರ(ctx->repo,
				   ctx->current_batch.oid,
				   ctx->current_batch.nr);
	oid_array_clear(&ctx->current_batch);

	/*
	 * We likely have a new packfile. Add it to the packed list to
	 * avoid possible duplicate downloads of the same objects.
	 */
	odb_reprepare(ctx->repo->objects);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fill_missing_blobs(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ ಬಳಸದ,
			      ರಚನೆ oid_array *list,
			      ಎಣಿಕೆ object_type type,
			      ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ backfill_context *ctx = ದತ್ತಾಂಶ;

	ಆದರೆ (type != OBJ_BLOB)
		ಹಿಂತಿರುಗಿ 0;

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < list->nr; i++) {
		ಆದರೆ (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	ಆದರೆ (ctx->current_batch.nr >= ctx->min_batch_ಗಾತ್ರ)
		download_batch(ctx);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ reject_unsupported_rev_list_options(ರಚನೆ rev_info *revs)
{
	ಆದರೆ (revs->diffopt.pickaxe)
		die(_("'%s' cannot be used with 'git backfill'"),
		    (revs->diffopt.pickaxe_opts & DIFF_PICKAXE_REGEX) ? "-G" : "-S");
	ಆದರೆ (revs->diffopt.filter || revs->diffopt.filter_not)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-filter");
	ಆದರೆ (revs->diffopt.flags.follow_renames)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--follow");
	ಆದರೆ (revs->ಸಾಲು_level_traverse)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "-L");
	ಆದರೆ (revs->explicit_diff_merges)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-merges");
	ಆದರೆ (!ಮಾರ್ಗ_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	ಆದರೆ (revs->filter.blob_ಎಲ್ಲೆ_ಮೌಲ್ಯ)
		die(_("cannot backfill with blob size limits"));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_backfill(ರಚನೆ backfill_context *ctx)
{
	ರಚನೆ ಮಾರ್ಗ_walk_info info = ಮಾರ್ಗ_WALK_INFO_INIT;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		ಆದರೆ (get_sparse_checkout_patterns(info.pl)) {
			ಮಾರ್ಗ_walk_info_clear(&info);
			ಹಿಂತಿರುಗಿ error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	ಆದರೆ (!ctx->revs.pending.nr)
		ಕೂಡಿಸು_ತಲೆ_ಗಮ್ಯ_pending(&ctx->revs);
	ಆದರೆ (ctx->include_ಅಂಚುಗಳು)
		ctx->revs.ಅಂಚು_hint = 1;

	info.blobs = 1;
	info.ಟ್ಯಾಗುಗಳು = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.ಮಾರ್ಗ_fn = fill_missing_blobs;
	info.ಮಾರ್ಗ_fn_ದತ್ತಾಂಶ = ctx;

	ret = walk_objects_by_ಮಾರ್ಗ(&info);

	/* Download the objects that did not fill a batch. */
	ಆದರೆ (!ret)
		download_batch(ctx);

	ಮಾರ್ಗ_walk_info_clear(&info);
	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ cmd_backfill(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
	ರಚನೆ backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_ಗಾತ್ರ = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_ಅಂಚುಗಳು = 1,
	};
	ರಚನೆ option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_ಗಾತ್ರ,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_ಅಂಚುಗಳು,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, ಪೂರ್ವಪ್ರತ್ಯಯ);
	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, &ctx.revs, ಖಾಲಿ);

	ಆದರೆ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, ಖಾಲಿ);

	ಆದರೆ (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	ಫಲಿತಾಂಶ = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
