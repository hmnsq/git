/* We need this macro to access core_apply_sparse_checkout */
#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "git-compat-util.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "repository.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "oidset.h"
#అన్తర్భావ "promisor-remote.h"
#అన్తర్భావ "strmap.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "trace2.h"
#అన్తర్భావ "progress.h"
#అన్తర్భావ "packfile.h"
#అన్తర్భావ "path-walk.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_backfill_usage[] = {
	N_("git backfill [--min-batch-size=<n>] [--[no-]sparse] [--[no-]include-edges] [<revision-range>]"),
	ఖాళీ
};

నిర్మాణం backfill_context {
	నిర్మాణం repository *repo;
	నిర్మాణం oid_array current_batch;
	కొలత_t min_batch_కొలత;
	పూర్ణాంకం sparse;
	పూర్ణాంకం include_అంచులు;
	నిర్మాణం rev_info revs;
};

స్థిర శూన్య backfill_context_clear(నిర్మాణం backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

స్థిర శూన్య download_batch(నిర్మాణం backfill_context *ctx)
{
	promisor_remote_get_ప్రత్యక్ష(ctx->repo,
				   ctx->current_batch.oid,
				   ctx->current_batch.nr);
	oid_array_clear(&ctx->current_batch);

	/*
	 * We likely have a new packfile. Add it to the packed list to
	 * avoid possible duplicate downloads of the same objects.
	 */
	odb_reprepare(ctx->repo->objects);
}

స్థిర పూర్ణాంకం fill_missing_blobs(స్థిరరాశి అక్షరం *మార్గం వాడని,
			      నిర్మాణం oid_array *list,
			      గణన object_type type,
			      శూన్య *దత్తాంశం)
{
	నిర్మాణం backfill_context *ctx = దత్తాంశం;

	ఒకవేళ (type != OBJ_BLOB)
		తిరిగి 0;

	కొరకు (కొలత_t i = 0; i < list->nr; i++) {
		ఒకవేళ (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	ఒకవేళ (ctx->current_batch.nr >= ctx->min_batch_కొలత)
		download_batch(ctx);

	తిరిగి 0;
}

స్థిర శూన్య reject_unsupported_rev_list_options(నిర్మాణం rev_info *revs)
{
	ఒకవేళ (revs->diffopt.pickaxe)
		die(_("'%s' cannot be used with 'git backfill'"),
		    (revs->diffopt.pickaxe_opts & DIFF_PICKAXE_REGEX) ? "-G" : "-S");
	ఒకవేళ (revs->diffopt.filter || revs->diffopt.filter_not)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-filter");
	ఒకవేళ (revs->diffopt.flags.follow_renames)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--follow");
	ఒకవేళ (revs->పంక్తి_level_traverse)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "-L");
	ఒకవేళ (revs->explicit_diff_merges)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-merges");
	ఒకవేళ (!మార్గం_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	ఒకవేళ (revs->filter.blob_హద్దు_విలువ)
		die(_("cannot backfill with blob size limits"));
}

స్థిర పూర్ణాంకం do_backfill(నిర్మాణం backfill_context *ctx)
{
	నిర్మాణం మార్గం_walk_info info = మార్గం_WALK_INFO_INIT;
	పూర్ణాంకం ret;

	ఒకవేళ (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		ఒకవేళ (get_sparse_checkout_patterns(info.pl)) {
			మార్గం_walk_info_clear(&info);
			తిరిగి error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	ఒకవేళ (!ctx->revs.pending.nr)
		కూడు_తల_గమ్యం_pending(&ctx->revs);
	ఒకవేళ (ctx->include_అంచులు)
		ctx->revs.అంచు_hint = 1;

	info.blobs = 1;
	info.ట్యాగులు = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.మార్గం_fn = fill_missing_blobs;
	info.మార్గం_fn_దత్తాంశం = ctx;

	ret = walk_objects_by_మార్గం(&info);

	/* Download the objects that did not fill a batch. */
	ఒకవేళ (!ret)
		download_batch(ctx);

	మార్గం_walk_info_clear(&info);
	తిరిగి ret;
}

పూర్ణాంకం cmd_backfill(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ, నిర్మాణం repository *repo)
{
	పూర్ణాంకం ఫలితం;
	నిర్మాణం backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_కొలత = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_అంచులు = 1,
	};
	నిర్మాణం option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_కొలత,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_అంచులు,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_చివర(),
	};
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, ఉపసర్గ, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, ఉపసర్గ);
	argc = అమరిక_revisions(argc, argv, &ctx.revs, ఖాళీ);

	ఒకవేళ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, ఖాళీ);

	ఒకవేళ (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	ఫలితం = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	తిరిగి ఫలితం;
}
