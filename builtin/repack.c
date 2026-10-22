#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

#ವ್ಯಾಖ್ಯಾನಿಸು ALL_INTO_ONE 1
#ವ್ಯಾಖ್ಯಾನಿಸು LOOSEN_UNREACHABLE 2
#ವ್ಯಾಖ್ಯಾನಿಸು PACK_CRUFT 4

#ವ್ಯಾಖ್ಯಾನಿಸು DELETE_PACK 1
#ವ್ಯಾಖ್ಯಾನಿಸು RETAIN_PACK 2

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pack_everything;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_bitmaps = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_delta_islands;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_update_server_info = 1;
ಸ್ಥಿರ ಅಕ್ಷರ *packdir, *packtmp_name, *packtmp;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ midx_must_contain_cruft = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ drop_filtered;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dry_ಸತತ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_bitmaps_given;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ git_repack_usage[] = {
	N_("git repack [-a] [-A] [-d] [-f] [-F] [-l] [-n] [-q] [-b] [-m]\n"
	   "[--window=<n>] [--depth=<n>] [--threads=<n>] [--keep-pack=<pack-name>]\n"
	   "[--write-midx[=<mode>]] [--name-hash-version=<n>] [--path-walk]\n"
	   "[--filter=<filter-spec>] [--drop-filtered [--dry-run]]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ incremental_bitmap_conflict_error[] = N_(
"Incremental repacks are incompatible with bitmap indexes.  Use\n"
"--no-write-bitmap-index or disable the pack.writeBitmaps configuration."
);

#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_MIDX_ವಿಭಜಿಸು_ಗುಣಕ 2
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_MIDX_NEW_LAYER_ಮಿತಿ 8

ರಚನೆ repack_config_ctx {
	ರಚನೆ pack_objects_ವಾದಗಳು *po_ವಾದಗಳು;
	ರಚನೆ pack_objects_ವಾದಗಳು *cruft_po_ವಾದಗಳು;
	ಪೂರ್ಣಾಂಕ midx_ವಿಭಜಿಸು_ಗುಣಕ;
	ಪೂರ್ಣಾಂಕ midx_new_layer_ಮಿತಿ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ repack_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ರಚನೆ repack_config_ctx *repack_ctx = cb;
	ರಚನೆ pack_objects_ವಾದಗಳು *po_ವಾದಗಳು = repack_ctx->po_ವಾದಗಳು;
	ರಚನೆ pack_objects_ವಾದಗಳು *cruft_po_ವಾದಗಳು = repack_ctx->cruft_po_ವಾದಗಳು;
	ಆದರೆ (!strcmp(var, "repack.usedeltabaseoffset")) {
		po_ವಾದಗಳು->delta_ತಳ_ಸ್ಥಾನಾಂತರ = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "repack.packkeptobjects")) {
		po_ವಾದಗಳು->pack_kept_objects = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "repack.writebitmaps") ||
	    !strcmp(var, "pack.writebitmaps")) {
		write_bitmaps = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "repack.usedeltaislands")) {
		use_delta_islands = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (strcmp(var, "repack.updateserverinfo") == 0) {
		ಸತತ_update_server_info = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "repack.cruftwindow")) {
		ಬಿಡುಗಡೆ(cruft_po_ವಾದಗಳು->ಕಿಟಕಿ);
		ಹಿಂತಿರುಗಿ git_config_string(&cruft_po_ವಾದಗಳು->ಕಿಟಕಿ, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "repack.cruftwindowmemory")) {
		ಬಿಡುಗಡೆ(cruft_po_ವಾದಗಳು->ಕಿಟಕಿ_ಸ್ಮರಣೆ);
		ಹಿಂತಿರುಗಿ git_config_string(&cruft_po_ವಾದಗಳು->ಕಿಟಕಿ_ಸ್ಮರಣೆ, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "repack.cruftdepth")) {
		ಬಿಡುಗಡೆ(cruft_po_ವಾದಗಳು->ಆಳ);
		ಹಿಂತಿರುಗಿ git_config_string(&cruft_po_ವಾದಗಳು->ಆಳ, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "repack.cruftthreads")) {
		ಬಿಡುಗಡೆ(cruft_po_ವಾದಗಳು->ಎಳೆಗಳು);
		ಹಿಂತಿರುಗಿ git_config_string(&cruft_po_ವಾದಗಳು->ಎಳೆಗಳು, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "repack.midxmustcontaincruft")) {
		midx_must_contain_cruft = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "repack.midxsplitfactor")) {
		repack_ctx->midx_ವಿಭಜಿಸು_ಗುಣಕ = git_config_int(var, ಮೌಲ್ಯ,
							       ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "repack.midxnewlayerthreshold")) {
		repack_ctx->midx_new_layer_ಮಿತಿ = git_config_int(var, ಮೌಲ್ಯ,
								      ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_write_bitmaps(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				      ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_ARG(arg);
	ಆದರೆ (unset)
		*ಮೌಲ್ಯ = 0;
	ಇಲ್ಲದಿದ್ದರೆ
		*ಮೌಲ್ಯ = 1;

	write_bitmaps_given = 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_write_midx(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				   ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ repack_write_midx_ಬಹುಲಕ *cfg = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset) {
		*cfg = REPACK_WRITE_MIDX_NONE;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!arg || !*arg)
		*cfg = REPACK_WRITE_MIDX_DEFAULT;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "incremental"))
		*cfg = REPACK_WRITE_MIDX_INCREMENTAL;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("unknown value for %s: %s"), opt->long_name, arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_repack(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ string_list ಹೆಸರುಗಳು = STRING_LIST_INIT_DUP;
	ರಚನೆ existing_packs existing = EXISTING_PACKS_INIT;
	ರಚನೆ oidset drop_oids = OIDSET_INIT;
	ರಚನೆ pack_geometry geometry = { 0 };
	ರಚನೆ tempfile *refs_snapshot = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ i, ret;
	ಪೂರ್ಣಾಂಕ show_ಪ್ರಗತಿ;

	/* variables to be filled by option parsing */
	ರಚನೆ repack_config_ctx config_ctx;
	ಪೂರ್ಣಾಂಕ delete_redundant = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *unpack_unreachable = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ keep_unreachable = 0;
	ರಚನೆ string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	ರಚನೆ pack_objects_ವಾದಗಳು po_ವಾದಗಳು = PACK_OBJECTS_ವಾದಗಳು_INIT;
	ರಚನೆ pack_objects_ವಾದಗಳು cruft_po_ವಾದಗಳು = PACK_OBJECTS_ವಾದಗಳು_INIT;
	ಎಣಿಕೆ repack_write_midx_ಬಹುಲಕ write_midx = REPACK_WRITE_MIDX_NONE;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cruft_expiration = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *expire_ಗಮ್ಯ = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filter_ಗಮ್ಯ = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_ಕಿಟಕಿ = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_ಕಿಟಕಿ_ಸ್ಮರಣೆ = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_ಆಳ = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_ಎಳೆಗಳು = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ combine_cruft_ಕೆಳಗೆ_ಗಾತ್ರ = 0ul;

	ರಚನೆ option builtin_repack_options[] = {
		OPT_ಬಿಟ್('a', ಖಾಲಿ, &pack_everything,
				N_("pack everything in a single pack"), ALL_INTO_ONE),
		OPT_ಬಿಟ್('A', ಖಾಲಿ, &pack_everything,
				N_("same as -a, and turn unreachable objects loose"),
				   LOOSEN_UNREACHABLE | ALL_INTO_ONE),
		OPT_ಬಿಟ್(0, "cruft", &pack_everything,
				N_("same as -a, pack unreachable cruft objects separately"),
				   PACK_CRUFT),
		OPT_STRING(0, "cruft-expiration", &cruft_expiration, N_("approxidate"),
				N_("with --cruft, expire objects older than this")),
		OPT_UNSIGNED(0, "combine-cruft-below-size",
			     &combine_cruft_ಕೆಳಗೆ_ಗಾತ್ರ,
			     N_("with --cruft, only repack cruft packs smaller than this")),
		OPT_UNSIGNED(0, "max-cruft-size", &cruft_po_ವಾದಗಳು.max_pack_ಗಾತ್ರ,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL('d', ಖಾಲಿ, &delete_redundant,
				N_("remove redundant packs, and run git-prune-packed")),
		OPT_BOOL('f', ಖಾಲಿ, &po_ವಾದಗಳು.no_reuse_delta,
				N_("pass --no-reuse-delta to git-pack-objects")),
		OPT_BOOL('F', ಖಾಲಿ, &po_ವಾದಗಳು.no_reuse_object,
				N_("pass --no-reuse-object to git-pack-objects")),
		OPT_INTEGER(0, "name-hash-version", &po_ವಾದಗಳು.name_hash_ಆವೃತ್ತಿ,
				N_("specify the name hash version to use for grouping similar objects by path")),
		OPT_BOOL(0, "path-walk", &po_ವಾದಗಳು.ಮಾರ್ಗ_walk,
				N_("pass --path-walk to git-pack-objects")),
		OPT_NEGBIT('n', ಖಾಲಿ, &ಸತತ_update_server_info,
				N_("do not run git-update-server-info"), 1),
		OPT__QUIET(&po_ವಾದಗಳು.quiet, N_("be quiet")),
		OPT_BOOL('l', "local", &po_ವಾದಗಳು.local,
				N_("pass --local to git-pack-objects")),
		OPT_CALLBACK_F('b', "write-bitmap-index", &write_bitmaps, ಖಾಲಿ,
				N_("write bitmap index"),
				PARSE_OPT_NOARG, option_parse_write_bitmaps),
		OPT_BOOL('i', "delta-islands", &use_delta_islands,
				N_("pass --delta-islands to git-pack-objects")),
		OPT_STRING(0, "unpack-unreachable", &unpack_unreachable, N_("approxidate"),
				N_("with -A, do not loosen objects older than this")),
		OPT_BOOL('k', "keep-unreachable", &keep_unreachable,
				N_("with -a, repack unreachable objects")),
		OPT_STRING(0, "window", &opt_ಕಿಟಕಿ, N_("n"),
				N_("size of the window used for delta compression")),
		OPT_STRING(0, "window-memory", &opt_ಕಿಟಕಿ_ಸ್ಮರಣೆ, N_("bytes"),
				N_("same as the above, but limit memory size instead of entries count")),
		OPT_STRING(0, "depth", &opt_ಆಳ, N_("n"),
				N_("limits the maximum delta depth")),
		OPT_STRING(0, "threads", &opt_ಎಳೆಗಳು, N_("n"),
				N_("limits the maximum number of threads")),
		OPT_UNSIGNED(0, "max-pack-size", &po_ವಾದಗಳು.max_pack_ಗಾತ್ರ,
			     N_("maximum size of each packfile")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&po_ವಾದಗಳು.filter_options),
		OPT_BOOL(0, "pack-kept-objects", &po_ವಾದಗಳು.pack_kept_objects,
				N_("repack objects in packs marked with .keep")),
		OPT_STRING_LIST(0, "keep-pack", &keep_pack_list, N_("name"),
				N_("do not repack this pack")),
		OPT_INTEGER('g', "geometric", &geometry.ವಿಭಜಿಸು_ಗುಣಕ,
			    N_("find a geometric progression with factor <N>")),
		OPT_CALLBACK_F(0, "write-midx", &write_midx,
			   N_("mode"),
			   N_("write a multi-pack index of the resulting packs"),
			   PARSE_OPT_OPTARG, option_parse_write_midx),
		OPT_ಗಣ_INT_F('m', ಖಾಲಿ, &write_midx,
			   N_("write a multi-pack index of the resulting packs"),
			   REPACK_WRITE_MIDX_DEFAULT,
			   PARSE_OPT_HIDDEN),
		OPT_STRING(0, "expire-to", &expire_ಗಮ್ಯ, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_STRING(0, "filter-to", &filter_ಗಮ್ಯ, N_("dir"),
			   N_("pack prefix to store a pack containing filtered out objects")),
		OPT_BOOL(0, "drop-filtered", &drop_filtered,
				N_("delete filtered out objects (requires --filter)")),
		OPT_BOOL(0, "dry-run", &dry_ಸತತ,
				N_("only show which objects would be dropped")),
		OPT_ಕೊನೆ()
	};

	list_objects_filter_init(&po_ವಾದಗಳು.filter_options);

	memset(&config_ctx, 0, ಗಾತ್ರ(config_ctx));
	config_ctx.po_ವಾದಗಳು = &po_ವಾದಗಳು;
	config_ctx.cruft_po_ವಾದಗಳು = &cruft_po_ವಾದಗಳು;
	config_ctx.midx_ವಿಭಜಿಸು_ಗುಣಕ = DEFAULT_MIDX_ವಿಭಜಿಸು_ಗುಣಕ;
	config_ctx.midx_new_layer_ಮಿತಿ = DEFAULT_MIDX_NEW_LAYER_ಮಿತಿ;

	repo_config(repo, repack_config, &config_ctx);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_repack_options,
				git_repack_usage, 0);

	po_ವಾದಗಳು.ಕಿಟಕಿ = xstrdup_or_null(opt_ಕಿಟಕಿ);
	po_ವಾದಗಳು.ಕಿಟಕಿ_ಸ್ಮರಣೆ = xstrdup_or_null(opt_ಕಿಟಕಿ_ಸ್ಮರಣೆ);
	po_ವಾದಗಳು.ಆಳ = xstrdup_or_null(opt_ಆಳ);
	po_ವಾದಗಳು.ಎಳೆಗಳು = xstrdup_or_null(opt_ಎಳೆಗಳು);

	die_for_incompatible_opt2(drop_filtered, "--drop-filtered",
		!!filter_ಗಮ್ಯ, "--filter-to");

	ಆದರೆ (dry_ಸತತ && !drop_filtered)
		die(_("--dry-run only takes effect with --drop-filtered"));

	ಆದರೆ (drop_filtered) {
		ಆದರೆ (!po_ವಾದಗಳು.filter_options.choice)
			die(_("--drop-filtered requires --filter"));

		ಆದರೆ (!(pack_everything & ALL_INTO_ONE))
			die(_("--drop-filtered requires -a"));

		/*
		 * Only blob:limit=<n> is supported for now. Reject other
		 * filter choices early, before walking the object database.
		 */
		ಆದರೆ (po_ವಾದಗಳು.filter_options.choice != LOFC_BLOB_ಎಲ್ಲೆ)
			die(_("--drop-filtered only supports --filter=blob:limit=<n> for now"));

		/*
		 * An explicit -b on the command line is a conflict we have to
		 * report; a bitmap setting from config is silently overridden
		 * for the duration of the command.
		 */
		ಆದರೆ (write_bitmaps_given && write_bitmaps > 0)
			die(_("options '%s' and '%s' cannot be used together"),
				"--drop-filtered", "--write-bitmap-index");

		/*
		 * Without a promisor remote there is nowhere to re-fetch the
		 * dropped objects from, so dropping them would be permanent
		 * data loss.
		 */
		ಆದರೆ (!repo_has_promisor_remote(repo))
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
		ಆದರೆ (!is_bare_repository(repo)) {
			ರಚನೆ wt_ಸ್ಥಿತಿ_state state = { 0 };

			wt_ಸ್ಥಿತಿ_get_state(repo, &state, 0);
			ಆದರೆ (state.ವಿಲೀನ_ಒಳ_ಪ್ರಗತಿ || state.revert_ಒಳ_ಪ್ರಗತಿ ||
			    state.rebase_ಒಳ_ಪ್ರಗತಿ || state.bisect_ಒಳ_ಪ್ರಗತಿ ||
			    state.cherry_pick_ಒಳ_ಪ್ರಗತಿ || state.am_ಒಳ_ಪ್ರಗತಿ ||
			    state.rebase_interactive_ಒಳ_ಪ್ರಗತಿ) {
				wt_ಸ್ಥಿತಿ_state_free_buffers(&state);
				die(_("--drop-filtered cannot be used while "
				      "another operation (merge, rebase, am, "
				      "cherry-pick, revert, or bisect) is in "
				      "progress"));
			}
			wt_ಸ್ಥಿತಿ_state_free_buffers(&state);
		}

		write_bitmaps = 0;

		/*
		 * Dropping objects means rebuilding the promisor packs
		 * without them and then removing the old packs, so the
		 * redundant packs must be deleted. Imply -d on a real run.
		 */
		ಆದರೆ (!dry_ಸತತ)
			delete_redundant = 1;

		ret = enumerate_promisor_blobs(repo, &po_ವಾದಗಳು.filter_options, &drop_oids);

		ಆದರೆ (ret)
			ಹೋಗು cleanup;

		/*
		 * Refuse to drop blobs that the current index references.
		 * Such a blob would only be lazily re-fetched by the next
		 * command that touches the worktree, so dropping it reclaims
		 * nothing. This guard just avoids that churn. Bare
		 * repositories have no index, so the check is skipped there.
		 */
		ಆದರೆ (!is_bare_repository(repo) && oidset_ಗಾತ್ರ(&drop_oids)) {
			ರಚನೆ ಸೂಚ್ಯಂಕ_state *istate = repo->ಸೂಚ್ಯಂಕ;
			ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;

			ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(repo) < 0)
				die(_("could not read the index"));

			ಗಾಗಿ (i = 0; i < istate->cache_nr; i++) {
				ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = istate->cache[i];

				ಆದರೆ (oidset_ಒಳಗೊಂಡಿದೆ(&drop_oids, &ce->oid))
					die(_("cannot drop '%s' (%s): it is referenced by the current index"),
						ce->name, oid_ಗಮ್ಯ_hex(&ce->oid));
			}
		}

		ಆದರೆ (dry_ಸತತ) {
			ರಚನೆ oidset_iter iter;
			ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid;

			oidset_iter_init(&drop_oids, &iter);
			ಆಗ ((oid = oidset_iter_ಮುಂದಿನ(&iter)))
				ಬರೆ("%s\n", oid_ಗಮ್ಯ_hex(oid));
		}
	}

	ಆದರೆ (delete_redundant && repo->repository_format_precious_objects)
		die(_("cannot delete packs in a precious-objects repo"));

	die_for_incompatible_opt3(unpack_unreachable || (pack_everything & LOOSEN_UNREACHABLE), "-A",
				  keep_unreachable, "-k/--keep-unreachable",
				  pack_everything & PACK_CRUFT, "--cruft");

	ಆದರೆ (pack_everything & PACK_CRUFT)
		pack_everything |= ALL_INTO_ONE;

	ಆದರೆ (write_bitmaps < 0) {
		ಆದರೆ (write_midx == REPACK_WRITE_MIDX_NONE &&
		    (!(pack_everything & ALL_INTO_ONE) || !is_bare_repository(repo)))
			write_bitmaps = 0;
	}
	ಆದರೆ (po_ವಾದಗಳು.pack_kept_objects < 0)
		po_ವಾದಗಳು.pack_kept_objects = write_bitmaps > 0 &&
			write_midx == REPACK_WRITE_MIDX_NONE;

	ಆದರೆ (write_bitmaps && !(pack_everything & ALL_INTO_ONE) &&
	    write_midx == REPACK_WRITE_MIDX_NONE)
		die(_(incremental_bitmap_conflict_error));

	ಆದರೆ (write_bitmaps && po_ವಾದಗಳು.local &&
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

	ಆದರೆ (config_ctx.midx_ವಿಭಜಿಸು_ಗುಣಕ < 2)
		die(_("invalid value for %s: %d"), "--midx-split-factor",
		    config_ctx.midx_ವಿಭಜಿಸು_ಗುಣಕ);
	ಆದರೆ (config_ctx.midx_new_layer_ಮಿತಿ < 1)
		die(_("invalid value for %s: %d"), "--midx-new-layer-threshold",
		    config_ctx.midx_new_layer_ಮಿತಿ);

	ಆದರೆ (write_midx != REPACK_WRITE_MIDX_NONE && write_bitmaps) {
		ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;

		strbuf_addf(&ಮಾರ್ಗ, "%s/%s_XXXXXX",
			    repo_get_object_directory(repo),
			    "bitmap-ref-tips");

		refs_snapshot = xmks_tempfile(ಮಾರ್ಗ.buf);
		midx_snapshot_refs(repo, refs_snapshot);

		strbuf_release(&ಮಾರ್ಗ);
	}

	packdir = mkpathdup("%s/pack", repo_get_object_directory(repo));
	packtmp_name = xstrfmt(".tmp-%d-pack", (ಪೂರ್ಣಾಂಕ)getpid());
	packtmp = mkpathdup("%s/%s", packdir, packtmp_name);

	existing.repo = repo;
	existing_packs_collect(&existing, &keep_pack_list);

	ಆದರೆ (geometry.ವಿಭಜಿಸು_ಗುಣಕ) {
		ಆದರೆ (pack_everything)
			die(_("options '%s' and '%s' cannot be used together"), "--geometric", "-A/-a");
		ಆದರೆ (write_midx == REPACK_WRITE_MIDX_INCREMENTAL) {
			geometry.midx_layer_ಮಿತಿ = config_ctx.midx_new_layer_ಮಿತಿ;
			geometry.midx_layer_ಮಿತಿ_ಗಣ = true;
		}
		pack_geometry_init(&geometry, &existing, &po_ವಾದಗಳು);
		pack_geometry_ವಿಭಜಿಸು(&geometry);
	}

	prepare_pack_objects(&cmd, &po_ವಾದಗಳು, packtmp);

	show_ಪ್ರಗತಿ = !po_ವಾದಗಳು.quiet && isatty(2);

	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--keep-true-parents");
	ಗಾಗಿ (i = 0; i < keep_pack_list.nr; i++)
		strvec_pushf(&cmd.ವಾದಗಳು, "--keep-pack=%s",
			     keep_pack_list.ವಸ್ತುಗಳು[i].string);
	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--non-empty");
	ಆದರೆ (!geometry.ವಿಭಜಿಸು_ಗುಣಕ) {
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
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--all");
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--reflog");
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--indexed-objects");
	}
	ಆದರೆ (repo_has_promisor_remote(repo))
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--exclude-promisor-objects");
	ಆದರೆ (write_midx == REPACK_WRITE_MIDX_NONE) {
		ಆದರೆ (write_bitmaps > 0)
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--write-bitmap-index");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (write_bitmaps < 0)
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--write-bitmap-index-quiet");
	}
	ಆದರೆ (use_delta_islands)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--delta-islands");

	ಆದರೆ (pack_everything & ALL_INTO_ONE) {
		repack_promisor_objects(repo, &po_ವಾದಗಳು, &ಹೆಸರುಗಳು, packtmp,
			(drop_filtered && !dry_ಸತತ) ? &drop_oids : ಖಾಲಿ);

		ಆದರೆ (existing_packs_has_non_kept(&existing) &&
		    delete_redundant &&
		    !(pack_everything & PACK_CRUFT)) {
			for_each_string_list_ಅಂಶ(ಅಂಶ, &ಹೆಸರುಗಳು) {
				strvec_pushf(&cmd.ವಾದಗಳು, "--keep-pack=%s-%s.pack",
					     packtmp_name, ಅಂಶ->string);
			}
			ಆದರೆ (unpack_unreachable) {
				strvec_pushf(&cmd.ವಾದಗಳು,
					     "--unpack-unreachable=%s",
					     unpack_unreachable);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pack_everything & LOOSEN_UNREACHABLE) {
				strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು,
					    "--unpack-unreachable");
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (keep_unreachable) {
				strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--keep-unreachable");
			}
		}

		ಆದರೆ (keep_unreachable && delete_redundant &&
		    !(pack_everything & PACK_CRUFT))
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--pack-loose-unreachable");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (geometry.ವಿಭಜಿಸು_ಗುಣಕ) {
		pack_geometry_repack_promisors(repo, &po_ವಾದಗಳು, &geometry,
					       &ಹೆಸರುಗಳು, packtmp);

		ಆದರೆ (midx_must_contain_cruft)
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--stdin-packs");
		ಇಲ್ಲದಿದ್ದರೆ
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--stdin-packs=follow");
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--unpacked");
	} ಇಲ್ಲದಿದ್ದರೆ {
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--unpacked");
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--incremental");
	}

	ಆದರೆ (po_ವಾದಗಳು.filter_options.choice)
		strvec_pushf(&cmd.ವಾದಗಳು, "--filter=%s",
			     expand_list_objects_filter_spec(&po_ವಾದಗಳು.filter_options));
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (filter_ಗಮ್ಯ)
		die(_("option '%s' can only be used along with '%s'"), "--filter-to", "--filter");

	ಆದರೆ (geometry.ವಿಭಜಿಸು_ಗುಣಕ)
		cmd.ಒಳ = -1;
	ಇಲ್ಲದಿದ್ದರೆ
		cmd.no_stdin = 1;

	ret = ಆರಂಭ_command(&cmd);
	ಆದರೆ (ret)
		ಹೋಗು cleanup;

	ಆದರೆ (geometry.ವಿಭಜಿಸು_ಗುಣಕ) {
		FILE *ಒಳ = xfdopen(cmd.ಒಳ, "w");
		/*
		 * The resulting pack should contain all objects in packs that
		 * are going to be rolled up, but exclude objects in packs which
		 * are being left alone.
		 */
		ಗಾಗಿ (i = 0; i < geometry.ವಿಭಜಿಸು; i++)
			fprintf(ಒಳ, "%s\n", pack_basename(geometry.pack[i]));
		ಗಾಗಿ (i = geometry.ವಿಭಜಿಸು; i < geometry.pack_nr; i++) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *basename = pack_basename(geometry.pack[i]);
			ಅಕ್ಷರ marker = '^';

			ಆದರೆ (!midx_must_contain_cruft &&
			    !string_list_has_string(&existing.midx_packs,
						    basename)) {
				/*
				 * Assume non-MIDX'd packs are not
				 * necessarily closed under
				 * reachability.
				 */
				marker = '!';
			}

			fprintf(ಒಳ, "%c%s\n", marker, basename);
		}
		fclose(ಒಳ);
	}

	{
		ರಚನೆ write_pack_opts opts = {
			.packdir = packdir,
			.destination = packdir,
			.packtmp = packtmp,
		};
		ret = ಮುಕ್ತಾಯ_pack_objects_cmd(repo->hash_algo, &opts, &cmd,
					      &ಹೆಸರುಗಳು);
		ಆದರೆ (ret)
			ಹೋಗು cleanup;
	}

	ಆದರೆ (!ಹೆಸರುಗಳು.nr) {
		ರಚನೆ odb_ಮೂಲ_files *files = odb_ಮೂಲ_files_downcast(existing.ಮೂಲ);

		ಆದರೆ (!po_ವಾದಗಳು.quiet)
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
		ಆದರೆ (!get_multi_pack_ಸೂಚ್ಯಂಕ(files->dirs->packed))
			midx_must_contain_cruft = 1;
	}

	ಆದರೆ (pack_everything & PACK_CRUFT) {
		ರಚನೆ write_pack_opts opts = {
			.po_ವಾದಗಳು = &cruft_po_ವಾದಗಳು,
			.destination = packtmp,
			.packtmp = packtmp,
			.packdir = packdir,
		};

		ಆದರೆ (!cruft_po_ವಾದಗಳು.ಕಿಟಕಿ)
			cruft_po_ವಾದಗಳು.ಕಿಟಕಿ = xstrdup_or_null(po_ವಾದಗಳು.ಕಿಟಕಿ);
		ಆದರೆ (!cruft_po_ವಾದಗಳು.ಕಿಟಕಿ_ಸ್ಮರಣೆ)
			cruft_po_ವಾದಗಳು.ಕಿಟಕಿ_ಸ್ಮರಣೆ = xstrdup_or_null(po_ವಾದಗಳು.ಕಿಟಕಿ_ಸ್ಮರಣೆ);
		ಆದರೆ (!cruft_po_ವಾದಗಳು.ಆಳ)
			cruft_po_ವಾದಗಳು.ಆಳ = xstrdup_or_null(po_ವಾದಗಳು.ಆಳ);
		ಆದರೆ (!cruft_po_ವಾದಗಳು.ಎಳೆಗಳು)
			cruft_po_ವಾದಗಳು.ಎಳೆಗಳು = xstrdup_or_null(po_ವಾದಗಳು.ಎಳೆಗಳು);
		ಆದರೆ (!cruft_po_ವಾದಗಳು.max_pack_ಗಾತ್ರ)
			cruft_po_ವಾದಗಳು.max_pack_ಗಾತ್ರ = po_ವಾದಗಳು.max_pack_ಗಾತ್ರ;

		cruft_po_ವಾದಗಳು.local = po_ವಾದಗಳು.local;
		cruft_po_ವಾದಗಳು.quiet = po_ವಾದಗಳು.quiet;
		cruft_po_ವಾದಗಳು.delta_ತಳ_ಸ್ಥಾನಾಂತರ = po_ವಾದಗಳು.delta_ತಳ_ಸ್ಥಾನಾಂತರ;
		cruft_po_ವಾದಗಳು.pack_kept_objects = 0;

		ret = write_cruft_pack(&opts, cruft_expiration,
				       combine_cruft_ಕೆಳಗೆ_ಗಾತ್ರ, &ಹೆಸರುಗಳು,
				       &existing);
		ಆದರೆ (ret)
			ಹೋಗು cleanup;

		ಆದರೆ (delete_redundant && expire_ಗಮ್ಯ) {
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
			opts.destination = expire_ಗಮ್ಯ;
			ret = write_cruft_pack(&opts, ಖಾಲಿ, 0ul, &ಹೆಸರುಗಳು,
					       &existing);
			ಆದರೆ (ret)
				ಹೋಗು cleanup;
		}
	}

	ಆದರೆ (po_ವಾದಗಳು.filter_options.choice && !drop_filtered) {
		ರಚನೆ write_pack_opts opts = {
			.po_ವಾದಗಳು = &po_ವಾದಗಳು,
			.destination = filter_ಗಮ್ಯ,
			.packdir = packdir,
			.packtmp = packtmp,
		};

		ಆದರೆ (!opts.destination)
			opts.destination = packtmp;

		ret = write_filtered_pack(&opts, &existing, &ಹೆಸರುಗಳು);
		ಆದರೆ (ret)
			ಹೋಗು cleanup;
	}

	string_list_ವಿಂಗಡಿಸು(&ಹೆಸರುಗಳು);

	odb_close(repo->objects);

	/*
	 * Ok we have prepared all new packfiles.
	 */
	for_each_string_list_ಅಂಶ(ಅಂಶ, &ಹೆಸರುಗಳು)
		generated_pack_install(ಅಂಶ->util, ಅಂಶ->string, packdir,
				       packtmp);
	/* End of pack replacement. */

	ಆದರೆ (delete_redundant && pack_everything & ALL_INTO_ONE) {
		ಆದರೆ (write_midx == REPACK_WRITE_MIDX_INCREMENTAL)
			existing_packs_retain_midx_packs(&existing);
		existing_packs_mark_for_deletion(&existing, &ಹೆಸರುಗಳು);
	}

	ಆದರೆ (write_midx != REPACK_WRITE_MIDX_NONE) {
		ರಚನೆ repack_write_midx_opts opts = {
			.existing = &existing,
			.geometry = &geometry,
			.ಹೆಸರುಗಳು = &ಹೆಸರುಗಳು,
			.refs_snapshot = refs_snapshot ? get_tempfile_ಮಾರ್ಗ(refs_snapshot) : ಖಾಲಿ,
			.packdir = packdir,
			.show_ಪ್ರಗತಿ = show_ಪ್ರಗತಿ,
			.write_bitmaps = write_bitmaps > 0,
			.midx_must_contain_cruft = midx_must_contain_cruft,
			.midx_ವಿಭಜಿಸು_ಗುಣಕ = config_ctx.midx_ವಿಭಜಿಸು_ಗುಣಕ,
			.midx_new_layer_ಮಿತಿ = config_ctx.midx_new_layer_ಮಿತಿ,
			.ಬಹುಲಕ = write_midx,
		};

		ret = repack_write_midx(&opts);
		ಆದರೆ (ret)
			ಹೋಗು cleanup;
	}

	odb_reprepare(repo->objects);

	ಆದರೆ (delete_redundant) {
		ಪೂರ್ಣಾಂಕ opts = 0;
		bool wrote_incremental_midx = write_midx == REPACK_WRITE_MIDX_INCREMENTAL;

		existing_packs_remove_redundant(&existing, packdir,
						wrote_incremental_midx);

		ಆದರೆ (geometry.ವಿಭಜಿಸು_ಗುಣಕ)
			pack_geometry_remove_redundant(&geometry, &ಹೆಸರುಗಳು,
						       &existing, packdir,
						       wrote_incremental_midx);
		ಆದರೆ (show_ಪ್ರಗತಿ)
			opts |= PRUNE_PACKED_VERBOSE;
		prune_packed_objects(opts);

		ಆದರೆ (!keep_unreachable &&
		    (!(pack_everything & LOOSEN_UNREACHABLE) ||
		     unpack_unreachable) &&
		    is_repository_shallow(repo))
			prune_shallow(PRUNE_ಶೀಘ್ರ);
	}

	ಆದರೆ (ಸತತ_update_server_info)
		update_server_info(repo, 0);

	ಆದರೆ (git_env_bool(GIT_TEST_MULTI_PACK_ಸೂಚ್ಯಂಕ, 0)) {
		ರಚನೆ odb_ಮೂಲ_files *files = odb_ಮೂಲ_files_downcast(existing.ಮೂಲ);
		ಚಿಹ್ನೆರಹಿತ flags = 0;

		ಆದರೆ (git_env_bool(GIT_TEST_MULTI_PACK_ಸೂಚ್ಯಂಕ_WRITE_INCREMENTAL, 0))
			flags |= MIDX_WRITE_INCREMENTAL;
		write_midx_file(files->dirs->packed, ಖಾಲಿ, ಖಾಲಿ, flags);
	}

cleanup:
	string_list_clear(&keep_pack_list, 0);
	string_list_clear(&ಹೆಸರುಗಳು, 1);
	oidset_clear(&drop_oids);
	existing_packs_release(&existing);
	pack_geometry_release(&geometry);
	pack_objects_ವಾದಗಳು_release(&po_ವಾದಗಳು);
	pack_objects_ವಾದಗಳು_release(&cruft_po_ವಾದಗಳು);

	ಹಿಂತಿರುಗಿ ret;
}
