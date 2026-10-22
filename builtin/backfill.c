/* We need this macro to access core_apply_sparse_checkout */
#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "git-compat-util.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "repository.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "tree-walk.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "oidset.h"
#അന്തര്ഭാവ "promisor-remote.h"
#അന്തര്ഭാവ "strmap.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "trace2.h"
#അന്തര്ഭാവ "progress.h"
#അന്തര്ഭാവ "packfile.h"
#അന്തര്ഭാവ "path-walk.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_backfill_usage[] = {
	N_("git backfill [--min-batch-size=<n>] [--[no-]sparse] [--[no-]include-edges] [<revision-range>]"),
	ശൂന്യ
};

ഘടന backfill_context {
	ഘടന repository *repo;
	ഘടന oid_array current_batch;
	size_t min_batch_size;
	പൂർണ്ണസംഖ്യ sparse;
	പൂർണ്ണസംഖ്യ include_edges;
	ഘടന rev_info revs;
};

സ്ഥിതം ശൂന്യം backfill_context_clear(ഘടന backfill_context *ctx)
{
	oid_array_clear(&ctx->current_batch);
}

സ്ഥിതം ശൂന്യം download_batch(ഘടന backfill_context *ctx)
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

സ്ഥിതം പൂർണ്ണസംഖ്യ fill_missing_blobs(സ്ഥിരം അക്ഷരം *path UNUSED,
			      ഘടന oid_array *list,
			      എണ്ണൽ object_type type,
			      ശൂന്യം *data)
{
	ഘടന backfill_context *ctx = data;

	എങ്കിൽ (type != OBJ_BLOB)
		തിരികെ 0;

	വേണ്ടി (size_t i = 0; i < list->nr; i++) {
		എങ്കിൽ (!odb_has_object(ctx->repo->objects, &list->oid[i], 0))
			oid_array_append(&ctx->current_batch, &list->oid[i]);
	}

	എങ്കിൽ (ctx->current_batch.nr >= ctx->min_batch_size)
		download_batch(ctx);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം reject_unsupported_rev_list_options(ഘടന rev_info *revs)
{
	എങ്കിൽ (revs->diffopt.pickaxe)
		die(_("'%s' cannot be used with 'git backfill'"),
		    (revs->diffopt.pickaxe_opts & DIFF_PICKAXE_REGEX) ? "-G" : "-S");
	എങ്കിൽ (revs->diffopt.filter || revs->diffopt.filter_not)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-filter");
	എങ്കിൽ (revs->diffopt.flags.follow_renames)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--follow");
	എങ്കിൽ (revs->വരി_level_traverse)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "-L");
	എങ്കിൽ (revs->explicit_diff_merges)
		die(_("'%s' cannot be used with 'git backfill'"),
		    "--diff-merges");
	എങ്കിൽ (!path_walk_filter_compatible(&revs->filter))
		die(_("cannot backfill with these filter options"));
	എങ്കിൽ (revs->filter.blob_limit_മൂല്യം)
		die(_("cannot backfill with blob size limits"));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_backfill(ഘടന backfill_context *ctx)
{
	ഘടന path_walk_info info = PATH_WALK_INFO_INIT;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (ctx->sparse) {
		CALLOC_ARRAY(info.pl, 1);
		info.pl_sparse_trees = 1;
		എങ്കിൽ (get_sparse_checkout_patterns(info.pl)) {
			path_walk_info_clear(&info);
			തിരികെ error(_("problem loading sparse-checkout"));
		}
	}

	/* Walk from HEAD if otherwise unspecified. */
	എങ്കിൽ (!ctx->revs.pending.nr)
		add_head_to_pending(&ctx->revs);
	എങ്കിൽ (ctx->include_edges)
		ctx->revs.edge_hint = 1;

	info.blobs = 1;
	info.tags = info.commits = info.trees = 0;

	info.revs = &ctx->revs;
	info.path_fn = fill_missing_blobs;
	info.path_fn_data = ctx;

	ret = walk_objects_by_path(&info);

	/* Download the objects that did not fill a batch. */
	എങ്കിൽ (!ret)
		download_batch(ctx);

	path_walk_info_clear(&info);
	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ cmd_backfill(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix, ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ result;
	ഘടന backfill_context ctx = {
		.repo = repo,
		.current_batch = OID_ARRAY_INIT,
		.min_batch_size = 50000,
		.sparse = -1,
		.revs = REV_INFO_INIT,
		.include_edges = 1,
	};
	ഘടന option options[] = {
		OPT_UNSIGNED(0, "min-batch-size", &ctx.min_batch_size,
			     N_("Minimum number of objects to request at a time")),
		OPT_BOOL(0, "sparse", &ctx.sparse,
			 N_("Restrict the missing objects to the current sparse-checkout")),
		OPT_BOOL(0, "include-edges", &ctx.include_edges,
			 N_("Include blobs from boundary commits in the backfill")),
		OPT_END(),
	};
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	show_usage_with_options_if_asked(argc, argv,
					 builtin_backfill_usage, options);

	argc = parse_options(argc, argv, prefix, options, builtin_backfill_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_DASHDASH);

	repo_init_revisions(repo, &ctx.revs, prefix);
	argc = setup_revisions(argc, argv, &ctx.revs, ശൂന്യ);

	എങ്കിൽ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);
	reject_unsupported_rev_list_options(&ctx.revs);

	repo_config(repo, git_default_config, ശൂന്യ);

	എങ്കിൽ (ctx.sparse < 0)
		ctx.sparse = cfg->apply_sparse_checkout;

	result = do_backfill(&ctx);
	backfill_context_clear(&ctx);
	release_revisions(&ctx.revs);
	തിരികെ result;
}
