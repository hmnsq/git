/* We need this macro to access core_apply_sparse_checkout */
#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "git-compat-util.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "repository.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "tree-walk.h"
#અન્તર્ભાવ "object.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "oidset.h"
#અન્તર્ભાવ "promisor-remote.h"
#અન્તર્ભાવ "strmap.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "trace2.h"
#અન્તર્ભાવ "progress.h"
#અન્તર્ભાવ "packfile.h"
#અન્તર્ભાવ "path-walk.h"

સ્થિર અચલ અક્ષર * અચલ builtin_backfill_usage[] = {
	N_("git backfill [--min-batch-size=<n>] [--[no-]sparse] [--[no-]include-edges] [<revision-range>]"),
	ખાલી
};

સંરચના backfill_context {
	સંરચના repository *repo;
	સંરચના oid_array current_batch;
	size_t min_batch_size;
	પૂર્ણાંક sparse;
	પૂર્ણાંક include_edges;
	સંરચના rev_info revs;
};

સ્થિર શૂન્ય backfill_context_clear(સંરચના backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

સ્થિર શૂન્ય download_batch(સંરચના backfill_context *ctx)
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

સ્થિર પૂર્ણાંક fill_missing_blobs(અચલ અક્ષર *path UNUSED,
			      સંરચના oid_array *list,
			      ગણના object_type type,
			      શૂન્ય *data)
{
	સંરચના backfill_context *ctx = data;

	જો (type != OBJ_BLOB)
		પાછા 0;

	માટે (size_t i = 0; i < list->nr; i++) {
		જો (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	જો (ctx->current_batch.nr >= ctx->min_batch_size)
		download_batch(ctx);

	પાછા 0;
}

સ્થિર શૂન્ય reject_unsupported_rev_list_options(સંરચના rev_info *revs)
{
	જો (revs->diffopt.pickaxe)
		die(_("'%s' cannot be used with 'git backfill'"),
		    (revs->diffopt.pickaxe_opts & DIFF_PICKAXE_REGEX) ? "-G" : "-S");
	જો (revs->diffopt.filter || revs->diffopt.filter_not)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-filter");
	જો (revs->diffopt.flags.follow_renames)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--follow");
	જો (revs->લીટી_level_traverse)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "-L");
	જો (revs->explicit_diff_merges)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-merges");
	જો (!path_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	જો (revs->filter.blob_limit_કિંમત)
		die(_("cannot backfill with blob size limits"));
}

સ્થિર પૂર્ણાંક do_backfill(સંરચના backfill_context *ctx)
{
	સંરચના path_walk_info info = PATH_WALK_INFO_INIT;
	પૂર્ણાંક ret;

	જો (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		જો (get_sparse_checkout_patterns(info.pl)) {
			path_walk_info_clear(&info);
			પાછા error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	જો (!ctx->revs.pending.nr)
		add_head_to_pending(&ctx->revs);
	જો (ctx->include_edges)
		ctx->revs.edge_hint = 1;

	info.blobs = 1;
	info.tags = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.path_fn = fill_missing_blobs;
	info.path_fn_data = ctx;

	ret = walk_objects_by_path(&info);

	/* Download the objects that did not fill a batch. */
	જો (!ret)
		download_batch(ctx);

	path_walk_info_clear(&info);
	પાછા ret;
}

પૂર્ણાંક cmd_backfill(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix, સંરચના repository *repo)
{
	પૂર્ણાંક result;
	સંરચના backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_size = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_edges = 1,
	};
	સંરચના option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_size,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_edges,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_END(),
	};
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, prefix, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, prefix);
	argc = setup_revisions(argc, argv, &ctx.revs, ખાલી);

	જો (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, ખાલી);

	જો (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	result = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	પાછા result;
}
