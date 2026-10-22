#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "server-info.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "midx.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "prune-packed.h"
#ಅನ್ತರ್ಭಾವ "promisor-remote.h"
#ಅನ್ತರ್ಭಾವ "repack.h"
#ಅನ್ತರ್ಭಾವ "shallow.h"
#ಅನ್ತರ್ಭಾವ "list-objects-filter-options.h"
#ಅನ್ತರ್ಭಾವ "oidset.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "wt-status.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"

#define ALL_INTO_ONE 1
#define LOOSEN_UNREACHABLE 2
#define PACK_CRUFT 4

#define DELETE_PACK 1
#define RETAIN_PACK 2

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pack_everything;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_bitmaps = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_delta_islands;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_update_server_info = 1;
ಸ್ಥಿರ ವರ್ಣ *packdir, *packtmp_name, *packtmp;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ midx_must_contain_cruft = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ drop_filtered;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ dry_run;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_bitmaps_given;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ git_repack_usage[] = {
	N_("git repack [-a] [-A] [-d] [-f] [-F] [-l] [-n] [-q] [-b] [-m]\n"
	   "[--window=<n>] [--depth=<n>] [--threads=<n>] [--keep-pack=<pack-name>]\n"
	   "[--write-midx[=<mode>]] [--name-hash-version=<n>] [--path-walk]\n"
	   "[--filter=<filter-spec>] [--drop-filtered [--dry-run]]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ incremental_bitmap_conflict_error[] = N_(
"Incremental repacks are incompatible with bitmap indexes.  Use\n"
"--no-write-bitmap-index or disable the pack.writeBitmaps configuration."
);

#define DEFAULT_MIDX_SPLIT_FACTOR 2
#define DEFAULT_MIDX_NEW_LAYER_THRESHOLD 8

ಸಂರಚನಾ repack_config_ctx {
	ಸಂರಚನಾ pack_objects_args *po_args;
	ಸಂರಚನಾ pack_objects_args *cruft_po_args;
	ಪೂರ್ಣಾಙ್ಕ midx_split_factor;
	ಪೂರ್ಣಾಙ್ಕ midx_new_layer_threshold;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ repack_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			 ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ repack_config_ctx *repack_ctx = cb;
	ಸಂರಚನಾ pack_objects_args *po_args = repack_ctx->po_args;
	ಸಂರಚನಾ pack_objects_args *cruft_po_args = repack_ctx->cruft_po_args;
	ಯದಿ (!strcmp(var, "repack.usedeltabaseoffset")) {
		po_args->delta_base_offset = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "repack.packkeptobjects")) {
		po_args->pack_kept_objects = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "repack.writebitmaps") ||
	    !strcmp(var, "pack.writebitmaps")) {
		write_bitmaps = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "repack.usedeltaislands")) {
		use_delta_islands = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (strcmp(var, "repack.updateserverinfo") == 0) {
		run_update_server_info = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "repack.cruftwindow")) {
		ಮುಕ್ತ(cruft_po_args->window);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cruft_po_args->window, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "repack.cruftwindowmemory")) {
		ಮುಕ್ತ(cruft_po_args->window_memory);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cruft_po_args->window_memory, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "repack.cruftdepth")) {
		ಮುಕ್ತ(cruft_po_args->depth);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cruft_po_args->depth, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "repack.cruftthreads")) {
		ಮುಕ್ತ(cruft_po_args->threads);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cruft_po_args->threads, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "repack.midxmustcontaincruft")) {
		midx_must_contain_cruft = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "repack.midxsplitfactor")) {
		repack_ctx->midx_split_factor = git_config_int(var, ಮೌಲ್ಯ,
							       ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "repack.midxnewlayerthreshold")) {
		repack_ctx->midx_new_layer_threshold = git_config_int(var, ಮೌಲ್ಯ,
								      ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_write_bitmaps(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
				      ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_ARG(arg);
	ಯದಿ (unset)
		*ಮೌಲ್ಯ = 0;
	ಅನ್ಯಥಾ
		*ಮೌಲ್ಯ = 1;

	write_bitmaps_given = 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_write_midx(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
				   ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಗಣನಾ repack_write_midx_mode *cfg = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset) {
		*cfg = REPACK_WRITE_MIDX_NONE;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!arg || !*arg)
		*cfg = REPACK_WRITE_MIDX_DEFAULT;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "incremental"))
		*cfg = REPACK_WRITE_MIDX_INCREMENTAL;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_("unknown value for %s: %s"), opt->long_name, arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_repack(ಪೂರ್ಣಾಙ್ಕ argc,
	       ಧ್ರುವ ವರ್ಣ **argv,
	       ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ string_list names = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ existing_packs existing = EXISTING_PACKS_INIT;
	ಸಂರಚನಾ oidset drop_oids = OIDSET_INIT;
	ಸಂರಚನಾ pack_geometry geometry = { 0 };
	ಸಂರಚನಾ tempfile *refs_snapshot = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ i, ret;
	ಪೂರ್ಣಾಙ್ಕ show_progress;

	/* variables to be filled by option parsing */
	ಸಂರಚನಾ repack_config_ctx config_ctx;
	ಪೂರ್ಣಾಙ್ಕ delete_redundant = 0;
	ಧ್ರುವ ವರ್ಣ *unpack_unreachable = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ keep_unreachable = 0;
	ಸಂರಚನಾ string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ pack_objects_args po_args = PACK_OBJECTS_ARGS_INIT;
	ಸಂರಚನಾ pack_objects_args cruft_po_args = PACK_OBJECTS_ARGS_INIT;
	ಗಣನಾ repack_write_midx_mode write_midx = REPACK_WRITE_MIDX_NONE;
	ಧ್ರುವ ವರ್ಣ *cruft_expiration = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *expire_to = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *filter_to = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *opt_window = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *opt_window_memory = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *opt_depth = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *opt_threads = ರಿಕ್ತ;
	unsigned long combine_cruft_below_size = 0ul;

	ಸಂರಚನಾ option builtin_repack_options[] = {
		OPT_BIT('a', ರಿಕ್ತ, &pack_everything,
				N_("pack everything in a single pack"), ALL_INTO_ONE),
		OPT_BIT('A', ರಿಕ್ತ, &pack_everything,
				N_("same as -a, and turn unreachable objects loose"),
				   LOOSEN_UNREACHABLE | ALL_INTO_ONE),
		OPT_BIT(0, "cruft", &pack_everything,
				N_("same as -a, pack unreachable cruft objects separately"),
				   PACK_CRUFT),
		OPT_STRING(0, "cruft-expiration", &cruft_expiration, N_("approxidate"),
				N_("with --cruft, expire objects older than this")),
		OPT_UNSIGNED(0, "combine-cruft-below-size",
			     &combine_cruft_below_size,
			     N_("with --cruft, only repack cruft packs smaller than this")),
		OPT_UNSIGNED(0, "max-cruft-size", &cruft_po_args.max_pack_size,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL('d', ರಿಕ್ತ, &delete_redundant,
				N_("remove redundant packs, and run git-prune-packed")),
		OPT_BOOL('f', ರಿಕ್ತ, &po_args.no_reuse_delta,
				N_("pass --no-reuse-delta to git-pack-objects")),
		OPT_BOOL('F', ರಿಕ್ತ, &po_args.no_reuse_object,
				N_("pass --no-reuse-object to git-pack-objects")),
		OPT_INTEGER(0, "name-hash-version", &po_args.name_hash_version,
				N_("specify the name hash version to use for grouping similar objects by path")),
		OPT_BOOL(0, "path-walk", &po_args.path_walk,
				N_("pass --path-walk to git-pack-objects")),
		OPT_NEGBIT('n', ರಿಕ್ತ, &run_update_server_info,
				N_("do not run git-update-server-info"), 1),
		OPT__QUIET(&po_args.quiet, N_("be quiet")),
		OPT_BOOL('l', "local", &po_args.local,
				N_("pass --local to git-pack-objects")),
		OPT_CALLBACK_F('b', "write-bitmap-index", &write_bitmaps, ರಿಕ್ತ,
				N_("write bitmap index"),
				PARSE_OPT_NOARG, option_parse_write_bitmaps),
		OPT_BOOL('i', "delta-islands", &use_delta_islands,
				N_("pass --delta-islands to git-pack-objects")),
		OPT_STRING(0, "unpack-unreachable", &unpack_unreachable, N_("approxidate"),
				N_("with -A, do not loosen objects older than this")),
		OPT_BOOL('k', "keep-unreachable", &keep_unreachable,
				N_("with -a, repack unreachable objects")),
		OPT_STRING(0, "window", &opt_window, N_("n"),
				N_("size of the window used for delta compression")),
		OPT_STRING(0, "window-memory", &opt_window_memory, N_("bytes"),
				N_("same as the above, but limit memory size instead of entries count")),
		OPT_STRING(0, "depth", &opt_depth, N_("n"),
				N_("limits the maximum delta depth")),
		OPT_STRING(0, "threads", &opt_threads, N_("n"),
				N_("limits the maximum number of threads")),
		OPT_UNSIGNED(0, "max-pack-size", &po_args.max_pack_size,
			     N_("maximum size of each packfile")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&po_args.filter_options),
		OPT_BOOL(0, "pack-kept-objects", &po_args.pack_kept_objects,
				N_("repack objects in packs marked with .keep")),
		OPT_STRING_LIST(0, "keep-pack", &keep_pack_list, N_("name"),
				N_("do not repack this pack")),
		OPT_INTEGER('g', "geometric", &geometry.split_factor,
			    N_("find a geometric progression with factor <N>")),
		OPT_CALLBACK_F(0, "write-midx", &write_midx,
			   N_("mode"),
			   N_("write a multi-pack index of the resulting packs"),
			   PARSE_OPT_OPTARG, option_parse_write_midx),
		OPT_SET_INT_F('m', ರಿಕ್ತ, &write_midx,
			   N_("write a multi-pack index of the resulting packs"),
			   REPACK_WRITE_MIDX_DEFAULT,
			   PARSE_OPT_HIDDEN),
		OPT_STRING(0, "expire-to", &expire_to, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_STRING(0, "filter-to", &filter_to, N_("dir"),
			   N_("pack prefix to store a pack containing filtered out objects")),
		OPT_BOOL(0, "drop-filtered", &drop_filtered,
				N_("delete filtered out objects (requires --filter)")),
		OPT_BOOL(0, "dry-run", &dry_run,
				N_("only show which objects would be dropped")),
		OPT_END()
	};

	list_objects_filter_init(&po_args.filter_options);

	memset(&config_ctx, 0, ಪರಿಮಾಣ(config_ctx));
	config_ctx.po_args = &po_args;
	config_ctx.cruft_po_args = &cruft_po_args;
	config_ctx.midx_split_factor = DEFAULT_MIDX_SPLIT_FACTOR;
	config_ctx.midx_new_layer_threshold = DEFAULT_MIDX_NEW_LAYER_THRESHOLD;

	repo_config(repo, repack_config, &config_ctx);

	argc = parse_options(argc, argv, prefix, builtin_repack_options,
				git_repack_usage, 0);

	po_args.window = xstrdup_or_null(opt_window);
	po_args.window_memory = xstrdup_or_null(opt_window_memory);
	po_args.depth = xstrdup_or_null(opt_depth);
	po_args.threads = xstrdup_or_null(opt_threads);

	die_for_incompatible_opt2(drop_filtered, "--drop-filtered",
		!!filter_to, "--filter-to");

	ಯದಿ (dry_run && !drop_filtered)
		die(_("--dry-run only takes effect with --drop-filtered"));

	ಯದಿ (drop_filtered) {
		ಯದಿ (!po_args.filter_options.choice)
			die(_("--drop-filtered requires --filter"));

		ಯದಿ (!(pack_everything & ALL_INTO_ONE))
			die(_("--drop-filtered requires -a"));

		/*
		 * Only blob:limit=<n> is supported for now. Reject other
		 * filter choices early, before walking the object database.
		 */
		ಯದಿ (po_args.filter_options.choice != LOFC_BLOB_LIMIT)
			die(_("--drop-filtered only supports --filter=blob:limit=<n> for now"));

		/*
		 * An explicit -b on the command line is a conflict we have to
		 * report; a bitmap setting from config is silently overridden
		 * for the duration of the command.
		 */
		ಯದಿ (write_bitmaps_given && write_bitmaps > 0)
			die(_("options '%s' and '%s' cannot be used together"),
				"--drop-filtered", "--write-bitmap-index");

		/*
		 * Without a promisor remote there is nowhere to re-fetch the
		 * dropped objects from, so dropping them would be permanent
		 * data loss.
		 */
		ಯದಿ (!repo_has_promisor_remote(repo))
			die(_("--drop-filtered requires a promisor remote"));

		/*
		 * Refuse to run while another operation is in progress. A
		 * dropped object would just be lazily re-fetched when the
		 * operation resumes, but triggering a network fetch in the
		 * middle of a half-finished
		 * merge/rebase/cherry-pick/revert/bisect is a poor
		 * experience, so this is a UX convenience rather than a
		 * safety measure. Bare repositories have no such state, so
		 * the check is skipped there.
		 */
		ಯದಿ (!is_bare_repository(repo)) {
			ಸಂರಚನಾ wt_status_state state = { 0 };

			wt_status_get_state(repo, &state, 0);
			ಯದಿ (state.merge_in_progress || state.revert_in_progress ||
			    state.rebase_in_progress || state.bisect_in_progress ||
			    state.cherry_pick_in_progress || state.am_in_progress ||
			    state.rebase_interactive_in_progress) {
				wt_status_state_free_buffers(&state);
				die(_("--drop-filtered cannot be used while "
				      "another operation (merge, rebase, am, "
				      "cherry-pick, revert, or bisect) is in "
				      "progress"));
			}
			wt_status_state_free_buffers(&state);
		}

		write_bitmaps = 0;

		/*
		 * Dropping objects means rebuilding the promisor packs
		 * without them and then removing the old packs, so the
		 * redundant packs must be deleted. Imply -d on a real run.
		 */
		ಯದಿ (!dry_run)
			delete_redundant = 1;

		ret = enumerate_promisor_blobs(repo, &po_args.filter_options, &drop_oids);

		ಯದಿ (ret)
			ಗಚ್ಛ cleanup;

		/*
		 * Refuse to drop blobs that the current index references.
		 * Such a blob would only be lazily re-fetched by the next
		 * command that touches the worktree, so dropping it reclaims
		 * nothing. This guard just avoids that churn. Bare
		 * repositories have no index, so the check is skipped there.
		 */
		ಯದಿ (!is_bare_repository(repo) && oidset_size(&drop_oids)) {
			ಸಂರಚನಾ index_state *istate = repo->index;
			unsigned ಪೂರ್ಣಾಙ್ಕ i;

			ಯದಿ (repo_read_index(repo) < 0)
				die(_("could not read the index"));

			ಪ್ರತಿ (i = 0; i < istate->cache_nr; i++) {
				ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = istate->cache[i];

				ಯದಿ (oidset_contains(&drop_oids, &ce->oid))
					die(_("cannot drop '%s' (%s): it is referenced by the current index"),
						ce->name, oid_to_hex(&ce->oid));
			}
		}

		ಯದಿ (dry_run) {
			ಸಂರಚನಾ oidset_iter iter;
			ಧ್ರುವ ಸಂರಚನಾ object_id *oid;

			oidset_iter_init(&drop_oids, &iter);
			ಯಾವತ್ ((oid = oidset_iter_next(&iter)))
				printf("%s\n", oid_to_hex(oid));
		}
	}

	ಯದಿ (delete_redundant && repo->repository_format_precious_objects)
		die(_("cannot delete packs in a precious-objects repo"));

	die_for_incompatible_opt3(unpack_unreachable || (pack_everything & LOOSEN_UNREACHABLE), "-A",
				  keep_unreachable, "-k/--keep-unreachable",
				  pack_everything & PACK_CRUFT, "--cruft");

	ಯದಿ (pack_everything & PACK_CRUFT)
		pack_everything |= ALL_INTO_ONE;

	ಯದಿ (write_bitmaps < 0) {
		ಯದಿ (write_midx == REPACK_WRITE_MIDX_NONE &&
		    (!(pack_everything & ALL_INTO_ONE) || !is_bare_repository(repo)))
			write_bitmaps = 0;
	}
	ಯದಿ (po_args.pack_kept_objects < 0)
		po_args.pack_kept_objects = write_bitmaps > 0 &&
			write_midx == REPACK_WRITE_MIDX_NONE;

	ಯದಿ (write_bitmaps && !(pack_everything & ALL_INTO_ONE) &&
	    write_midx == REPACK_WRITE_MIDX_NONE)
		die(_(incremental_bitmap_conflict_error));

	ಯದಿ (write_bitmaps && po_args.local &&
	    odb_has_alternates(repo->objects)) {
		/*
		 * When asked to do a local repack, but we have
		 * packfiles that are inherited from an alternate, then
		 * we cannot guarantee that the multi-pack-index would
		 * have full coverage of all objects. We thus disable
		 * writing bitmaps in that case.
		 */
		warning(_("disabling bitmap writing, as some objects are not being packed"));
		write_bitmaps = 0;
	}

	ಯದಿ (config_ctx.midx_split_factor < 2)
		die(_("invalid value for %s: %d"), "--midx-split-factor",
		    config_ctx.midx_split_factor);
	ಯದಿ (config_ctx.midx_new_layer_threshold < 1)
		die(_("invalid value for %s: %d"), "--midx-new-layer-threshold",
		    config_ctx.midx_new_layer_threshold);

	ಯದಿ (write_midx != REPACK_WRITE_MIDX_NONE && write_bitmaps) {
		ಸಂರಚನಾ strbuf path = STRBUF_INIT;

		strbuf_addf(&path, "%s/%s_XXXXXX",
			    repo_get_object_directory(repo),
			    "bitmap-ref-tips");

		refs_snapshot = xmks_tempfile(path.buf);
		midx_snapshot_refs(repo, refs_snapshot);

		strbuf_release(&path);
	}

	packdir = mkpathdup("%s/pack", repo_get_object_directory(repo));
	packtmp_name = xstrfmt(".tmp-%d-pack", (ಪೂರ್ಣಾಙ್ಕ)getpid());
	packtmp = mkpathdup("%s/%s", packdir, packtmp_name);

	existing.repo = repo;
	existing_packs_collect(&existing, &keep_pack_list);

	ಯದಿ (geometry.split_factor) {
		ಯದಿ (pack_everything)
			die(_("options '%s' and '%s' cannot be used together"), "--geometric", "-A/-a");
		ಯದಿ (write_midx == REPACK_WRITE_MIDX_INCREMENTAL) {
			geometry.midx_layer_threshold = config_ctx.midx_new_layer_threshold;
			geometry.midx_layer_threshold_set = true;
		}
		pack_geometry_init(&geometry, &existing, &po_args);
		pack_geometry_split(&geometry);
	}

	prepare_pack_objects(&cmd, &po_args, packtmp);

	show_progress = !po_args.quiet && isatty(2);

	strvec_ತಳ್ಳು(&cmd.args, "--keep-true-parents");
	ಪ್ರತಿ (i = 0; i < keep_pack_list.nr; i++)
		strvec_pushf(&cmd.args, "--keep-pack=%s",
			     keep_pack_list.ವಸ್ತುಗಳು[i].string);
	strvec_ತಳ್ಳು(&cmd.args, "--non-empty");
	ಯದಿ (!geometry.split_factor) {
		/*
		 * We need to grab all reachable objects, including those that
		 * are reachable from reflogs and the index.
		 *
		 * When repacking into a geometric progression of packs,
		 * however, we ask 'git pack-objects --stdin-packs', and it is
		 * not about packing objects based on reachability but about
		 * repacking all the objects in specified packs and loose ones
		 * (indeed, --stdin-packs is incompatible with these options).
		 */
		strvec_ತಳ್ಳು(&cmd.args, "--all");
		strvec_ತಳ್ಳು(&cmd.args, "--reflog");
		strvec_ತಳ್ಳು(&cmd.args, "--indexed-objects");
	}
	ಯದಿ (repo_has_promisor_remote(repo))
		strvec_ತಳ್ಳು(&cmd.args, "--exclude-promisor-objects");
	ಯದಿ (write_midx == REPACK_WRITE_MIDX_NONE) {
		ಯದಿ (write_bitmaps > 0)
			strvec_ತಳ್ಳು(&cmd.args, "--write-bitmap-index");
		ಅನ್ಯಥಾ ಯದಿ (write_bitmaps < 0)
			strvec_ತಳ್ಳು(&cmd.args, "--write-bitmap-index-quiet");
	}
	ಯದಿ (use_delta_islands)
		strvec_ತಳ್ಳು(&cmd.args, "--delta-islands");

	ಯದಿ (pack_everything & ALL_INTO_ONE) {
		repack_promisor_objects(repo, &po_args, &names, packtmp,
			(drop_filtered && !dry_run) ? &drop_oids : ರಿಕ್ತ);

		ಯದಿ (existing_packs_has_non_kept(&existing) &&
		    delete_redundant &&
		    !(pack_everything & PACK_CRUFT)) {
			for_each_string_list_item(item, &names) {
				strvec_pushf(&cmd.args, "--keep-pack=%s-%s.pack",
					     packtmp_name, item->string);
			}
			ಯದಿ (unpack_unreachable) {
				strvec_pushf(&cmd.args,
					     "--unpack-unreachable=%s",
					     unpack_unreachable);
			} ಅನ್ಯಥಾ ಯದಿ (pack_everything & LOOSEN_UNREACHABLE) {
				strvec_ತಳ್ಳು(&cmd.args,
					    "--unpack-unreachable");
			} ಅನ್ಯಥಾ ಯದಿ (keep_unreachable) {
				strvec_ತಳ್ಳು(&cmd.args, "--keep-unreachable");
			}
		}

		ಯದಿ (keep_unreachable && delete_redundant &&
		    !(pack_everything & PACK_CRUFT))
			strvec_ತಳ್ಳು(&cmd.args, "--pack-loose-unreachable");
	} ಅನ್ಯಥಾ ಯದಿ (geometry.split_factor) {
		pack_geometry_repack_promisors(repo, &po_args, &geometry,
					       &names, packtmp);

		ಯದಿ (midx_must_contain_cruft)
			strvec_ತಳ್ಳು(&cmd.args, "--stdin-packs");
		ಅನ್ಯಥಾ
			strvec_ತಳ್ಳು(&cmd.args, "--stdin-packs=follow");
		strvec_ತಳ್ಳು(&cmd.args, "--unpacked");
	} ಅನ್ಯಥಾ {
		strvec_ತಳ್ಳು(&cmd.args, "--unpacked");
		strvec_ತಳ್ಳು(&cmd.args, "--incremental");
	}

	ಯದಿ (po_args.filter_options.choice)
		strvec_pushf(&cmd.args, "--filter=%s",
			     expand_list_objects_filter_spec(&po_args.filter_options));
	ಅನ್ಯಥಾ ಯದಿ (filter_to)
		die(_("option '%s' can only be used along with '%s'"), "--filter-to", "--filter");

	ಯದಿ (geometry.split_factor)
		cmd.in = -1;
	ಅನ್ಯಥಾ
		cmd.no_stdin = 1;

	ret = start_command(&cmd);
	ಯದಿ (ret)
		ಗಚ್ಛ cleanup;

	ಯದಿ (geometry.split_factor) {
		FILE *in = xfdopen(cmd.in, "w");
		/*
		 * The resulting pack should contain all objects in packs that
		 * are going to be rolled up, but exclude objects in packs which
		 * are being left alone.
		 */
		ಪ್ರತಿ (i = 0; i < geometry.split; i++)
			fprintf(in, "%s\n", pack_basename(geometry.pack[i]));
		ಪ್ರತಿ (i = geometry.split; i < geometry.pack_nr; i++) {
			ಧ್ರುವ ವರ್ಣ *basename = pack_basename(geometry.pack[i]);
			ವರ್ಣ marker = '^';

			ಯದಿ (!midx_must_contain_cruft &&
			    !string_list_has_string(&existing.midx_packs,
						    basename)) {
				/*
				 * Assume non-MIDX'd packs are not
				 * necessarily closed under
				 * reachability.
				 */
				marker = '!';
			}

			fprintf(in, "%c%s\n", marker, basename);
		}
		fclose(in);
	}

	{
		ಸಂರಚನಾ write_pack_opts opts = {
			.packdir = packdir,
			.destination = packdir,
			.packtmp = packtmp,
		};
		ret = finish_pack_objects_cmd(repo->hash_algo, &opts, &cmd,
					      &names);
		ಯದಿ (ret)
			ಗಚ್ಛ cleanup;
	}

	ಯದಿ (!names.nr) {
		ಸಂರಚನಾ odb_source_files *files = odb_source_files_downcast(existing.source);

		ಯದಿ (!po_args.quiet)
			printf_ln(_("Nothing new to pack."));
		/*
		 * If we didn't write any new packs, the non-cruft packs
		 * may refer to once-unreachable objects in the cruft
		 * pack(s).
		 *
		 * If there isn't already a MIDX, the one we write
		 * must include the cruft pack(s), in case the
		 * non-cruft pack(s) refer to once-cruft objects.
		 *
		 * If there is already a MIDX, we can punt here, since
		 * midx_has_unknown_packs() will make the decision for
		 * us.
		 */
		ಯದಿ (!get_multi_pack_index(files->dirs->packed))
			midx_must_contain_cruft = 1;
	}

	ಯದಿ (pack_everything & PACK_CRUFT) {
		ಸಂರಚನಾ write_pack_opts opts = {
			.po_args = &cruft_po_args,
			.destination = packtmp,
			.packtmp = packtmp,
			.packdir = packdir,
		};

		ಯದಿ (!cruft_po_args.window)
			cruft_po_args.window = xstrdup_or_null(po_args.window);
		ಯದಿ (!cruft_po_args.window_memory)
			cruft_po_args.window_memory = xstrdup_or_null(po_args.window_memory);
		ಯದಿ (!cruft_po_args.depth)
			cruft_po_args.depth = xstrdup_or_null(po_args.depth);
		ಯದಿ (!cruft_po_args.threads)
			cruft_po_args.threads = xstrdup_or_null(po_args.threads);
		ಯದಿ (!cruft_po_args.max_pack_size)
			cruft_po_args.max_pack_size = po_args.max_pack_size;

		cruft_po_args.local = po_args.local;
		cruft_po_args.quiet = po_args.quiet;
		cruft_po_args.delta_base_offset = po_args.delta_base_offset;
		cruft_po_args.pack_kept_objects = 0;

		ret = write_cruft_pack(&opts, cruft_expiration,
				       combine_cruft_below_size, &names,
				       &existing);
		ಯದಿ (ret)
			ಗಚ್ಛ cleanup;

		ಯದಿ (delete_redundant && expire_to) {
			/*
			 * If `--expire-to` is given with `-d`, it's possible
			 * that we're about to prune some objects. With cruft
			 * packs, pruning is implicit: any objects from existing
			 * packs that weren't picked up by new packs are removed
			 * when their packs are deleted.
			 *
			 * Generate an additional cruft pack, with one twist:
			 * `names` now includes the name of the cruft pack
			 * written in the previous step. So the contents of
			 * _this_ cruft pack exclude everything contained in the
			 * existing cruft pack (that is, all of the unreachable
			 * objects which are no older than
			 * `--cruft-expiration`).
			 *
			 * To make this work, cruft_expiration must become NULL
			 * so that this cruft pack doesn't actually prune any
			 * objects. If it were non-NULL, this call would always
			 * generate an empty pack (since every object not in the
			 * cruft pack generated above will have an mtime older
			 * than the expiration).
			 *
			 * Pretend we don't have a `--combine-cruft-below-size`
			 * argument, since we're not selectively combining
			 * anything based on size to generate the limbo cruft
			 * pack, but rather removing all cruft packs from the
			 * main repository regardless of size.
			 */
			opts.destination = expire_to;
			ret = write_cruft_pack(&opts, ರಿಕ್ತ, 0ul, &names,
					       &existing);
			ಯದಿ (ret)
				ಗಚ್ಛ cleanup;
		}
	}

	ಯದಿ (po_args.filter_options.choice && !drop_filtered) {
		ಸಂರಚನಾ write_pack_opts opts = {
			.po_args = &po_args,
			.destination = filter_to,
			.packdir = packdir,
			.packtmp = packtmp,
		};

		ಯದಿ (!opts.destination)
			opts.destination = packtmp;

		ret = write_filtered_pack(&opts, &existing, &names);
		ಯದಿ (ret)
			ಗಚ್ಛ cleanup;
	}

	string_list_sort(&names);

	odb_close(repo->objects);

	/*
	 * Ok we have prepared all new packfiles.
	 */
	for_each_string_list_item(item, &names)
		generated_pack_install(item->util, item->string, packdir,
				       packtmp);
	/* End of pack replacement. */

	ಯದಿ (delete_redundant && pack_everything & ALL_INTO_ONE) {
		ಯದಿ (write_midx == REPACK_WRITE_MIDX_INCREMENTAL)
			existing_packs_retain_midx_packs(&existing);
		existing_packs_mark_for_deletion(&existing, &names);
	}

	ಯದಿ (write_midx != REPACK_WRITE_MIDX_NONE) {
		ಸಂರಚನಾ repack_write_midx_opts opts = {
			.existing = &existing,
			.geometry = &geometry,
			.names = &names,
			.refs_snapshot = refs_snapshot ? get_tempfile_path(refs_snapshot) : ರಿಕ್ತ,
			.packdir = packdir,
			.show_progress = show_progress,
			.write_bitmaps = write_bitmaps > 0,
			.midx_must_contain_cruft = midx_must_contain_cruft,
			.midx_split_factor = config_ctx.midx_split_factor,
			.midx_new_layer_threshold = config_ctx.midx_new_layer_threshold,
			.mode = write_midx,
		};

		ret = repack_write_midx(&opts);
		ಯದಿ (ret)
			ಗಚ್ಛ cleanup;
	}

	odb_reprepare(repo->objects);

	ಯದಿ (delete_redundant) {
		ಪೂರ್ಣಾಙ್ಕ opts = 0;
		bool wrote_incremental_midx = write_midx == REPACK_WRITE_MIDX_INCREMENTAL;

		existing_packs_remove_redundant(&existing, packdir,
						wrote_incremental_midx);

		ಯದಿ (geometry.split_factor)
			pack_geometry_remove_redundant(&geometry, &names,
						       &existing, packdir,
						       wrote_incremental_midx);
		ಯದಿ (show_progress)
			opts |= PRUNE_PACKED_VERBOSE;
		prune_packed_objects(opts);

		ಯದಿ (!keep_unreachable &&
		    (!(pack_everything & LOOSEN_UNREACHABLE) ||
		     unpack_unreachable) &&
		    is_repository_shallow(repo))
			prune_shallow(PRUNE_QUICK);
	}

	ಯದಿ (run_update_server_info)
		update_server_info(repo, 0);

	ಯದಿ (git_env_bool(GIT_TEST_MULTI_PACK_INDEX, 0)) {
		ಸಂರಚನಾ odb_source_files *files = odb_source_files_downcast(existing.source);
		unsigned flags = 0;

		ಯದಿ (git_env_bool(GIT_TEST_MULTI_PACK_INDEX_WRITE_INCREMENTAL, 0))
			flags |= MIDX_WRITE_INCREMENTAL;
		write_midx_file(files->dirs->packed, ರಿಕ್ತ, ರಿಕ್ತ, flags);
	}

cleanup:
	string_list_clear(&keep_pack_list, 0);
	string_list_clear(&names, 1);
	oidset_clear(&drop_oids);
	existing_packs_release(&existing);
	pack_geometry_release(&geometry);
	pack_objects_args_release(&po_args);
	pack_objects_args_release(&cruft_po_args);

	ಪ್ರತ್ಯಾಗಮ ret;
}
