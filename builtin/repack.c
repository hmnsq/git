#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "server-info.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "midx.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "prune-packed.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "promisor-remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "shallow.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list-objects-filter-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oidset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wt-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALL_INTO_ONE 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LOOSEN_UNREACHABLE 2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PACK_CRUFT 4

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DELETE_PACK 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RETAIN_PACK 2

ਸਥਿਰ ਪੂਰਨ pack_everything;
ਸਥਿਰ ਪੂਰਨ write_bitmaps = -1;
ਸਥਿਰ ਪੂਰਨ use_delta_islands;
ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_update_server_info = 1;
ਸਥਿਰ ਅੱਖਰ *packdir, *packtmp_name, *packtmp;
ਸਥਿਰ ਪੂਰਨ midx_must_contain_cruft = 1;
ਸਥਿਰ ਪੂਰਨ drop_filtered;
ਸਥਿਰ ਪੂਰਨ dry_ਲੜੀਵਾਰ;
ਸਥਿਰ ਪੂਰਨ write_bitmaps_given;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ git_repack_usage[] = {
	N_("git repack [-a] [-A] [-d] [-f] [-F] [-l] [-n] [-q] [-b] [-m]\n"
	   "[--window=<n>] [--depth=<n>] [--threads=<n>] [--keep-pack=<pack-name>]\n"
	   "[--write-midx[=<mode>]] [--name-hash-version=<n>] [--path-walk]\n"
	   "[--filter=<filter-spec>] [--drop-filtered [--dry-run]]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ incremental_bitmap_conflict_error[] = N_(
"Incremental repacks are incompatible with bitmap indexes.  Use\n"
"--no-write-bitmap-index or disable the pack.writeBitmaps configuration."
);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_MIDX_ਵੰਡੋ_ਗੁਣਕ 2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_MIDX_NEW_LAYER_ਹੱਦ 8

ਬਣਤਰ repack_config_ctx {
	ਬਣਤਰ pack_objects_ਦਲੀਲਾਂ *po_ਦਲੀਲਾਂ;
	ਬਣਤਰ pack_objects_ਦਲੀਲਾਂ *cruft_po_ਦਲੀਲਾਂ;
	ਪੂਰਨ midx_ਵੰਡੋ_ਗੁਣਕ;
	ਪੂਰਨ midx_new_layer_ਹੱਦ;
};

ਸਥਿਰ ਪੂਰਨ repack_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			 ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਬਣਤਰ repack_config_ctx *repack_ctx = cb;
	ਬਣਤਰ pack_objects_ਦਲੀਲਾਂ *po_ਦਲੀਲਾਂ = repack_ctx->po_ਦਲੀਲਾਂ;
	ਬਣਤਰ pack_objects_ਦਲੀਲਾਂ *cruft_po_ਦਲੀਲਾਂ = repack_ctx->cruft_po_ਦਲੀਲਾਂ;
	ਜੇ (!strcmp(var, "repack.usedeltabaseoffset")) {
		po_ਦਲੀਲਾਂ->delta_ਅਧਾਰ_ਵਿੱਥ = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "repack.packkeptobjects")) {
		po_ਦਲੀਲਾਂ->pack_kept_objects = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "repack.writebitmaps") ||
	    !strcmp(var, "pack.writebitmaps")) {
		write_bitmaps = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "repack.usedeltaislands")) {
		use_delta_islands = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (strcmp(var, "repack.updateserverinfo") == 0) {
		ਲੜੀਵਾਰ_update_server_info = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "repack.cruftwindow")) {
		ਮੁਕਤ(cruft_po_ਦਲੀਲਾਂ->ਖਿੜਕੀ);
		ਵਾਪਸ git_config_string(&cruft_po_ਦਲੀਲਾਂ->ਖਿੜਕੀ, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "repack.cruftwindowmemory")) {
		ਮੁਕਤ(cruft_po_ਦਲੀਲਾਂ->ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ);
		ਵਾਪਸ git_config_string(&cruft_po_ਦਲੀਲਾਂ->ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "repack.cruftdepth")) {
		ਮੁਕਤ(cruft_po_ਦਲੀਲਾਂ->ਡੂੰਘਾਈ);
		ਵਾਪਸ git_config_string(&cruft_po_ਦਲੀਲਾਂ->ਡੂੰਘਾਈ, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "repack.cruftthreads")) {
		ਮੁਕਤ(cruft_po_ਦਲੀਲਾਂ->ਧਾਗੇ);
		ਵਾਪਸ git_config_string(&cruft_po_ਦਲੀਲਾਂ->ਧਾਗੇ, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "repack.midxmustcontaincruft")) {
		midx_must_contain_cruft = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "repack.midxsplitfactor")) {
		repack_ctx->midx_ਵੰਡੋ_ਗੁਣਕ = git_config_int(var, ਮੁੱਲ,
							       ctx->kvi);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "repack.midxnewlayerthreshold")) {
		repack_ctx->midx_new_layer_ਹੱਦ = git_config_int(var, ਮੁੱਲ,
								      ctx->kvi);
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਪੂਰਨ option_parse_write_bitmaps(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				      ਪੂਰਨ unset)
{
	ਪੂਰਨ *ਮੁੱਲ = opt->ਮੁੱਲ;

	BUG_ON_OPT_ARG(arg);
	ਜੇ (unset)
		*ਮੁੱਲ = 0;
	ਨਹੀਂਤਾਂ
		*ਮੁੱਲ = 1;

	write_bitmaps_given = 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_write_midx(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				   ਪੂਰਨ unset)
{
	ਗਿਣਤੀ repack_write_midx_ਬਹੁਲਕ *cfg = opt->ਮੁੱਲ;

	ਜੇ (unset) {
		*cfg = REPACK_WRITE_MIDX_NONE;
		ਵਾਪਸ 0;
	}

	ਜੇ (!arg || !*arg)
		*cfg = REPACK_WRITE_MIDX_DEFAULT;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "incremental"))
		*cfg = REPACK_WRITE_MIDX_INCREMENTAL;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("unknown value for %s: %s"), opt->long_name, arg);

	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_repack(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ string_list ਨਾਂ = STRING_LIST_INIT_DUP;
	ਬਣਤਰ existing_packs existing = EXISTING_PACKS_INIT;
	ਬਣਤਰ oidset drop_oids = OIDSET_INIT;
	ਬਣਤਰ pack_geometry geometry = { 0 };
	ਬਣਤਰ tempfile *refs_snapshot = ਨਿਰਾ;
	ਪੂਰਨ i, ret;
	ਪੂਰਨ show_ਤਰੱਕੀ;

	/* variables to be filled by option parsing */
	ਬਣਤਰ repack_config_ctx config_ctx;
	ਪੂਰਨ delete_redundant = 0;
	ਅਚਲ ਅੱਖਰ *unpack_unreachable = ਨਿਰਾ;
	ਪੂਰਨ keep_unreachable = 0;
	ਬਣਤਰ string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ pack_objects_ਦਲੀਲਾਂ po_ਦਲੀਲਾਂ = PACK_OBJECTS_ਦਲੀਲਾਂ_INIT;
	ਬਣਤਰ pack_objects_ਦਲੀਲਾਂ cruft_po_ਦਲੀਲਾਂ = PACK_OBJECTS_ਦਲੀਲਾਂ_INIT;
	ਗਿਣਤੀ repack_write_midx_ਬਹੁਲਕ write_midx = REPACK_WRITE_MIDX_NONE;
	ਅਚਲ ਅੱਖਰ *cruft_expiration = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *expire_ਮੰਜ਼ਿਲ = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *filter_ਮੰਜ਼ਿਲ = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *opt_ਖਿੜਕੀ = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *opt_ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *opt_ਡੂੰਘਾਈ = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *opt_ਧਾਗੇ = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ combine_cruft_ਹੇਠਾਂ_ਮਾਪ = 0ul;

	ਬਣਤਰ option builtin_repack_options[] = {
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('a', ਨਿਰਾ, &pack_everything,
				N_("pack everything in a single pack"), ALL_INTO_ONE),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('A', ਨਿਰਾ, &pack_everything,
				N_("same as -a, and turn unreachable objects loose"),
				   LOOSEN_UNREACHABLE | ALL_INTO_ONE),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "cruft", &pack_everything,
				N_("same as -a, pack unreachable cruft objects separately"),
				   PACK_CRUFT),
		OPT_STRING(0, "cruft-expiration", &cruft_expiration, N_("approxidate"),
				N_("with --cruft, expire objects older than this")),
		OPT_UNSIGNED(0, "combine-cruft-below-size",
			     &combine_cruft_ਹੇਠਾਂ_ਮਾਪ,
			     N_("with --cruft, only repack cruft packs smaller than this")),
		OPT_UNSIGNED(0, "max-cruft-size", &cruft_po_ਦਲੀਲਾਂ.max_pack_ਮਾਪ,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL('d', ਨਿਰਾ, &delete_redundant,
				N_("remove redundant packs, and run git-prune-packed")),
		OPT_BOOL('f', ਨਿਰਾ, &po_ਦਲੀਲਾਂ.no_reuse_delta,
				N_("pass --no-reuse-delta to git-pack-objects")),
		OPT_BOOL('F', ਨਿਰਾ, &po_ਦਲੀਲਾਂ.no_reuse_object,
				N_("pass --no-reuse-object to git-pack-objects")),
		OPT_INTEGER(0, "name-hash-version", &po_ਦਲੀਲਾਂ.name_hash_ਸੰਸਕਰਣ,
				N_("specify the name hash version to use for grouping similar objects by path")),
		OPT_BOOL(0, "path-walk", &po_ਦਲੀਲਾਂ.ਰਸਤਾ_walk,
				N_("pass --path-walk to git-pack-objects")),
		OPT_NEGBIT('n', ਨਿਰਾ, &ਲੜੀਵਾਰ_update_server_info,
				N_("do not run git-update-server-info"), 1),
		OPT__QUIET(&po_ਦਲੀਲਾਂ.quiet, N_("be quiet")),
		OPT_BOOL('l', "local", &po_ਦਲੀਲਾਂ.local,
				N_("pass --local to git-pack-objects")),
		OPT_CALLBACK_F('b', "write-bitmap-index", &write_bitmaps, ਨਿਰਾ,
				N_("write bitmap index"),
				PARSE_OPT_NOARG, option_parse_write_bitmaps),
		OPT_BOOL('i', "delta-islands", &use_delta_islands,
				N_("pass --delta-islands to git-pack-objects")),
		OPT_STRING(0, "unpack-unreachable", &unpack_unreachable, N_("approxidate"),
				N_("with -A, do not loosen objects older than this")),
		OPT_BOOL('k', "keep-unreachable", &keep_unreachable,
				N_("with -a, repack unreachable objects")),
		OPT_STRING(0, "window", &opt_ਖਿੜਕੀ, N_("n"),
				N_("size of the window used for delta compression")),
		OPT_STRING(0, "window-memory", &opt_ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ, N_("bytes"),
				N_("same as the above, but limit memory size instead of entries count")),
		OPT_STRING(0, "depth", &opt_ਡੂੰਘਾਈ, N_("n"),
				N_("limits the maximum delta depth")),
		OPT_STRING(0, "threads", &opt_ਧਾਗੇ, N_("n"),
				N_("limits the maximum number of threads")),
		OPT_UNSIGNED(0, "max-pack-size", &po_ਦਲੀਲਾਂ.max_pack_ਮਾਪ,
			     N_("maximum size of each packfile")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&po_ਦਲੀਲਾਂ.filter_options),
		OPT_BOOL(0, "pack-kept-objects", &po_ਦਲੀਲਾਂ.pack_kept_objects,
				N_("repack objects in packs marked with .keep")),
		OPT_STRING_LIST(0, "keep-pack", &keep_pack_list, N_("name"),
				N_("do not repack this pack")),
		OPT_INTEGER('g', "geometric", &geometry.ਵੰਡੋ_ਗੁਣਕ,
			    N_("find a geometric progression with factor <N>")),
		OPT_CALLBACK_F(0, "write-midx", &write_midx,
			   N_("mode"),
			   N_("write a multi-pack index of the resulting packs"),
			   PARSE_OPT_OPTARG, option_parse_write_midx),
		OPT_ਸੈੱਟ_INT_F('m', ਨਿਰਾ, &write_midx,
			   N_("write a multi-pack index of the resulting packs"),
			   REPACK_WRITE_MIDX_DEFAULT,
			   PARSE_OPT_HIDDEN),
		OPT_STRING(0, "expire-to", &expire_ਮੰਜ਼ਿਲ, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_STRING(0, "filter-to", &filter_ਮੰਜ਼ਿਲ, N_("dir"),
			   N_("pack prefix to store a pack containing filtered out objects")),
		OPT_BOOL(0, "drop-filtered", &drop_filtered,
				N_("delete filtered out objects (requires --filter)")),
		OPT_BOOL(0, "dry-run", &dry_ਲੜੀਵਾਰ,
				N_("only show which objects would be dropped")),
		OPT_ਸਿਰਾ()
	};

	list_objects_filter_init(&po_ਦਲੀਲਾਂ.filter_options);

	memset(&config_ctx, 0, ਆਕਾਰ(config_ctx));
	config_ctx.po_ਦਲੀਲਾਂ = &po_ਦਲੀਲਾਂ;
	config_ctx.cruft_po_ਦਲੀਲਾਂ = &cruft_po_ਦਲੀਲਾਂ;
	config_ctx.midx_ਵੰਡੋ_ਗੁਣਕ = DEFAULT_MIDX_ਵੰਡੋ_ਗੁਣਕ;
	config_ctx.midx_new_layer_ਹੱਦ = DEFAULT_MIDX_NEW_LAYER_ਹੱਦ;

	repo_config(repo, repack_config, &config_ctx);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_repack_options,
				git_repack_usage, 0);

	po_ਦਲੀਲਾਂ.ਖਿੜਕੀ = xstrdup_or_null(opt_ਖਿੜਕੀ);
	po_ਦਲੀਲਾਂ.ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ = xstrdup_or_null(opt_ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ);
	po_ਦਲੀਲਾਂ.ਡੂੰਘਾਈ = xstrdup_or_null(opt_ਡੂੰਘਾਈ);
	po_ਦਲੀਲਾਂ.ਧਾਗੇ = xstrdup_or_null(opt_ਧਾਗੇ);

	die_for_incompatible_opt2(drop_filtered, "--drop-filtered",
		!!filter_ਮੰਜ਼ਿਲ, "--filter-to");

	ਜੇ (dry_ਲੜੀਵਾਰ && !drop_filtered)
		die(_("--dry-run only takes effect with --drop-filtered"));

	ਜੇ (drop_filtered) {
		ਜੇ (!po_ਦਲੀਲਾਂ.filter_options.choice)
			die(_("--drop-filtered requires --filter"));

		ਜੇ (!(pack_everything & ALL_INTO_ONE))
			die(_("--drop-filtered requires -a"));

		/*
		 * Only blob:limit=<n> is supported for now. Reject other
		 * filter choices early, before walking the object database.
		 */
		ਜੇ (po_ਦਲੀਲਾਂ.filter_options.choice != LOFC_BLOB_ਹੱਦਬੰਦੀ)
			die(_("--drop-filtered only supports --filter=blob:limit=<n> for now"));

		/*
		 * An explicit -b on the command line is a conflict we have to
		 * report; a bitmap setting from config is silently overridden
		 * for the duration of the command.
		 */
		ਜੇ (write_bitmaps_given && write_bitmaps > 0)
			die(_("options '%s' and '%s' cannot be used together"),
				"--drop-filtered", "--write-bitmap-index");

		/*
		 * Without a promisor remote there is nowhere to re-fetch the
		 * dropped objects from, so dropping them would be permanent
		 * data loss.
		 */
		ਜੇ (!repo_has_promisor_remote(repo))
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
		ਜੇ (!is_bare_repository(repo)) {
			ਬਣਤਰ wt_ਸਥਿਤੀ_state state = { 0 };

			wt_ਸਥਿਤੀ_get_state(repo, &state, 0);
			ਜੇ (state.ਮਿਲਾਓ_ਵਿੱਚ_ਤਰੱਕੀ || state.revert_ਵਿੱਚ_ਤਰੱਕੀ ||
			    state.rebase_ਵਿੱਚ_ਤਰੱਕੀ || state.bisect_ਵਿੱਚ_ਤਰੱਕੀ ||
			    state.cherry_pick_ਵਿੱਚ_ਤਰੱਕੀ || state.am_ਵਿੱਚ_ਤਰੱਕੀ ||
			    state.rebase_interactive_ਵਿੱਚ_ਤਰੱਕੀ) {
				wt_ਸਥਿਤੀ_state_free_buffers(&state);
				die(_("--drop-filtered cannot be used while "
				      "another operation (merge, rebase, am, "
				      "cherry-pick, revert, or bisect) is in "
				      "progress"));
			}
			wt_ਸਥਿਤੀ_state_free_buffers(&state);
		}

		write_bitmaps = 0;

		/*
		 * Dropping objects means rebuilding the promisor packs
		 * without them and then removing the old packs, so the
		 * redundant packs must be deleted. Imply -d on a real run.
		 */
		ਜੇ (!dry_ਲੜੀਵਾਰ)
			delete_redundant = 1;

		ret = enumerate_promisor_blobs(repo, &po_ਦਲੀਲਾਂ.filter_options, &drop_oids);

		ਜੇ (ret)
			ਜਾਓ cleanup;

		/*
		 * Refuse to drop blobs that the current index references.
		 * Such a blob would only be lazily re-fetched by the next
		 * command that touches the worktree, so dropping it reclaims
		 * nothing. This guard just avoids that churn. Bare
		 * repositories have no index, so the check is skipped there.
		 */
		ਜੇ (!is_bare_repository(repo) && oidset_ਮਾਪ(&drop_oids)) {
			ਬਣਤਰ ਸੂਚਕਾਂਕ_state *istate = repo->ਸੂਚਕਾਂਕ;
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

			ਜੇ (repo_read_ਸੂਚਕਾਂਕ(repo) < 0)
				die(_("could not read the index"));

			ਲਈ (i = 0; i < istate->cache_nr; i++) {
				ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = istate->cache[i];

				ਜੇ (oidset_ਸ਼ਾਮਲ_ਹੈ(&drop_oids, &ce->oid))
					die(_("cannot drop '%s' (%s): it is referenced by the current index"),
						ce->name, oid_ਮੰਜ਼ਿਲ_hex(&ce->oid));
			}
		}

		ਜੇ (dry_ਲੜੀਵਾਰ) {
			ਬਣਤਰ oidset_iter iter;
			ਅਚਲ ਬਣਤਰ object_id *oid;

			oidset_iter_init(&drop_oids, &iter);
			ਜਦੋਂ ((oid = oidset_iter_ਅਗਲਾ(&iter)))
				ਲਿਖੋ("%s\n", oid_ਮੰਜ਼ਿਲ_hex(oid));
		}
	}

	ਜੇ (delete_redundant && repo->repository_format_precious_objects)
		die(_("cannot delete packs in a precious-objects repo"));

	die_for_incompatible_opt3(unpack_unreachable || (pack_everything & LOOSEN_UNREACHABLE), "-A",
				  keep_unreachable, "-k/--keep-unreachable",
				  pack_everything & PACK_CRUFT, "--cruft");

	ਜੇ (pack_everything & PACK_CRUFT)
		pack_everything |= ALL_INTO_ONE;

	ਜੇ (write_bitmaps < 0) {
		ਜੇ (write_midx == REPACK_WRITE_MIDX_NONE &&
		    (!(pack_everything & ALL_INTO_ONE) || !is_bare_repository(repo)))
			write_bitmaps = 0;
	}
	ਜੇ (po_ਦਲੀਲਾਂ.pack_kept_objects < 0)
		po_ਦਲੀਲਾਂ.pack_kept_objects = write_bitmaps > 0 &&
			write_midx == REPACK_WRITE_MIDX_NONE;

	ਜੇ (write_bitmaps && !(pack_everything & ALL_INTO_ONE) &&
	    write_midx == REPACK_WRITE_MIDX_NONE)
		die(_(incremental_bitmap_conflict_error));

	ਜੇ (write_bitmaps && po_ਦਲੀਲਾਂ.local &&
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

	ਜੇ (config_ctx.midx_ਵੰਡੋ_ਗੁਣਕ < 2)
		die(_("invalid value for %s: %d"), "--midx-split-factor",
		    config_ctx.midx_ਵੰਡੋ_ਗੁਣਕ);
	ਜੇ (config_ctx.midx_new_layer_ਹੱਦ < 1)
		die(_("invalid value for %s: %d"), "--midx-new-layer-threshold",
		    config_ctx.midx_new_layer_ਹੱਦ);

	ਜੇ (write_midx != REPACK_WRITE_MIDX_NONE && write_bitmaps) {
		ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;

		strbuf_addf(&ਰਸਤਾ, "%s/%s_XXXXXX",
			    repo_get_object_directory(repo),
			    "bitmap-ref-tips");

		refs_snapshot = xmks_tempfile(ਰਸਤਾ.buf);
		midx_snapshot_refs(repo, refs_snapshot);

		strbuf_release(&ਰਸਤਾ);
	}

	packdir = mkpathdup("%s/pack", repo_get_object_directory(repo));
	packtmp_name = xstrfmt(".tmp-%d-pack", (ਪੂਰਨ)getpid());
	packtmp = mkpathdup("%s/%s", packdir, packtmp_name);

	existing.repo = repo;
	existing_packs_collect(&existing, &keep_pack_list);

	ਜੇ (geometry.ਵੰਡੋ_ਗੁਣਕ) {
		ਜੇ (pack_everything)
			die(_("options '%s' and '%s' cannot be used together"), "--geometric", "-A/-a");
		ਜੇ (write_midx == REPACK_WRITE_MIDX_INCREMENTAL) {
			geometry.midx_layer_ਹੱਦ = config_ctx.midx_new_layer_ਹੱਦ;
			geometry.midx_layer_ਹੱਦ_ਸੈੱਟ = true;
		}
		pack_geometry_init(&geometry, &existing, &po_ਦਲੀਲਾਂ);
		pack_geometry_ਵੰਡੋ(&geometry);
	}

	prepare_pack_objects(&cmd, &po_ਦਲੀਲਾਂ, packtmp);

	show_ਤਰੱਕੀ = !po_ਦਲੀਲਾਂ.quiet && isatty(2);

	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--keep-true-parents");
	ਲਈ (i = 0; i < keep_pack_list.nr; i++)
		strvec_pushf(&cmd.ਦਲੀਲਾਂ, "--keep-pack=%s",
			     keep_pack_list.ਚੀਜ਼ਾਂ[i].string);
	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--non-empty");
	ਜੇ (!geometry.ਵੰਡੋ_ਗੁਣਕ) {
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
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--all");
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--reflog");
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--indexed-objects");
	}
	ਜੇ (repo_has_promisor_remote(repo))
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--exclude-promisor-objects");
	ਜੇ (write_midx == REPACK_WRITE_MIDX_NONE) {
		ਜੇ (write_bitmaps > 0)
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--write-bitmap-index");
		ਨਹੀਂਤਾਂ ਜੇ (write_bitmaps < 0)
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--write-bitmap-index-quiet");
	}
	ਜੇ (use_delta_islands)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--delta-islands");

	ਜੇ (pack_everything & ALL_INTO_ONE) {
		repack_promisor_objects(repo, &po_ਦਲੀਲਾਂ, &ਨਾਂ, packtmp,
			(drop_filtered && !dry_ਲੜੀਵਾਰ) ? &drop_oids : ਨਿਰਾ);

		ਜੇ (existing_packs_has_non_kept(&existing) &&
		    delete_redundant &&
		    !(pack_everything & PACK_CRUFT)) {
			for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਨਾਂ) {
				strvec_pushf(&cmd.ਦਲੀਲਾਂ, "--keep-pack=%s-%s.pack",
					     packtmp_name, ਵਸਤੂ->string);
			}
			ਜੇ (unpack_unreachable) {
				strvec_pushf(&cmd.ਦਲੀਲਾਂ,
					     "--unpack-unreachable=%s",
					     unpack_unreachable);
			} ਨਹੀਂਤਾਂ ਜੇ (pack_everything & LOOSEN_UNREACHABLE) {
				strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ,
					    "--unpack-unreachable");
			} ਨਹੀਂਤਾਂ ਜੇ (keep_unreachable) {
				strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--keep-unreachable");
			}
		}

		ਜੇ (keep_unreachable && delete_redundant &&
		    !(pack_everything & PACK_CRUFT))
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--pack-loose-unreachable");
	} ਨਹੀਂਤਾਂ ਜੇ (geometry.ਵੰਡੋ_ਗੁਣਕ) {
		pack_geometry_repack_promisors(repo, &po_ਦਲੀਲਾਂ, &geometry,
					       &ਨਾਂ, packtmp);

		ਜੇ (midx_must_contain_cruft)
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--stdin-packs");
		ਨਹੀਂਤਾਂ
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--stdin-packs=follow");
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--unpacked");
	} ਨਹੀਂਤਾਂ {
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--unpacked");
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--incremental");
	}

	ਜੇ (po_ਦਲੀਲਾਂ.filter_options.choice)
		strvec_pushf(&cmd.ਦਲੀਲਾਂ, "--filter=%s",
			     expand_list_objects_filter_spec(&po_ਦਲੀਲਾਂ.filter_options));
	ਨਹੀਂਤਾਂ ਜੇ (filter_ਮੰਜ਼ਿਲ)
		die(_("option '%s' can only be used along with '%s'"), "--filter-to", "--filter");

	ਜੇ (geometry.ਵੰਡੋ_ਗੁਣਕ)
		cmd.ਵਿੱਚ = -1;
	ਨਹੀਂਤਾਂ
		cmd.no_stdin = 1;

	ret = ਸ਼ੁਰੂ_command(&cmd);
	ਜੇ (ret)
		ਜਾਓ cleanup;

	ਜੇ (geometry.ਵੰਡੋ_ਗੁਣਕ) {
		FILE *ਵਿੱਚ = xfdopen(cmd.ਵਿੱਚ, "w");
		/*
		 * The resulting pack should contain all objects in packs that
		 * are going to be rolled up, but exclude objects in packs which
		 * are being left alone.
		 */
		ਲਈ (i = 0; i < geometry.ਵੰਡੋ; i++)
			fprintf(ਵਿੱਚ, "%s\n", pack_basename(geometry.pack[i]));
		ਲਈ (i = geometry.ਵੰਡੋ; i < geometry.pack_nr; i++) {
			ਅਚਲ ਅੱਖਰ *basename = pack_basename(geometry.pack[i]);
			ਅੱਖਰ marker = '^';

			ਜੇ (!midx_must_contain_cruft &&
			    !string_list_has_string(&existing.midx_packs,
						    basename)) {
				/*
				 * Assume non-MIDX'd packs are not
				 * necessarily closed under
				 * reachability.
				 */
				marker = '!';
			}

			fprintf(ਵਿੱਚ, "%c%s\n", marker, basename);
		}
		fclose(ਵਿੱਚ);
	}

	{
		ਬਣਤਰ write_pack_opts opts = {
			.packdir = packdir,
			.destination = packdir,
			.packtmp = packtmp,
		};
		ret = ਅੰਤ_pack_objects_cmd(repo->hash_algo, &opts, &cmd,
					      &ਨਾਂ);
		ਜੇ (ret)
			ਜਾਓ cleanup;
	}

	ਜੇ (!ਨਾਂ.nr) {
		ਬਣਤਰ odb_ਸਰੋਤ_files *files = odb_ਸਰੋਤ_files_downcast(existing.ਸਰੋਤ);

		ਜੇ (!po_ਦਲੀਲਾਂ.quiet)
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
		ਜੇ (!get_multi_pack_ਸੂਚਕਾਂਕ(files->dirs->packed))
			midx_must_contain_cruft = 1;
	}

	ਜੇ (pack_everything & PACK_CRUFT) {
		ਬਣਤਰ write_pack_opts opts = {
			.po_ਦਲੀਲਾਂ = &cruft_po_ਦਲੀਲਾਂ,
			.destination = packtmp,
			.packtmp = packtmp,
			.packdir = packdir,
		};

		ਜੇ (!cruft_po_ਦਲੀਲਾਂ.ਖਿੜਕੀ)
			cruft_po_ਦਲੀਲਾਂ.ਖਿੜਕੀ = xstrdup_or_null(po_ਦਲੀਲਾਂ.ਖਿੜਕੀ);
		ਜੇ (!cruft_po_ਦਲੀਲਾਂ.ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ)
			cruft_po_ਦਲੀਲਾਂ.ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ = xstrdup_or_null(po_ਦਲੀਲਾਂ.ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ);
		ਜੇ (!cruft_po_ਦਲੀਲਾਂ.ਡੂੰਘਾਈ)
			cruft_po_ਦਲੀਲਾਂ.ਡੂੰਘਾਈ = xstrdup_or_null(po_ਦਲੀਲਾਂ.ਡੂੰਘਾਈ);
		ਜੇ (!cruft_po_ਦਲੀਲਾਂ.ਧਾਗੇ)
			cruft_po_ਦਲੀਲਾਂ.ਧਾਗੇ = xstrdup_or_null(po_ਦਲੀਲਾਂ.ਧਾਗੇ);
		ਜੇ (!cruft_po_ਦਲੀਲਾਂ.max_pack_ਮਾਪ)
			cruft_po_ਦਲੀਲਾਂ.max_pack_ਮਾਪ = po_ਦਲੀਲਾਂ.max_pack_ਮਾਪ;

		cruft_po_ਦਲੀਲਾਂ.local = po_ਦਲੀਲਾਂ.local;
		cruft_po_ਦਲੀਲਾਂ.quiet = po_ਦਲੀਲਾਂ.quiet;
		cruft_po_ਦਲੀਲਾਂ.delta_ਅਧਾਰ_ਵਿੱਥ = po_ਦਲੀਲਾਂ.delta_ਅਧਾਰ_ਵਿੱਥ;
		cruft_po_ਦਲੀਲਾਂ.pack_kept_objects = 0;

		ret = write_cruft_pack(&opts, cruft_expiration,
				       combine_cruft_ਹੇਠਾਂ_ਮਾਪ, &ਨਾਂ,
				       &existing);
		ਜੇ (ret)
			ਜਾਓ cleanup;

		ਜੇ (delete_redundant && expire_ਮੰਜ਼ਿਲ) {
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
			opts.destination = expire_ਮੰਜ਼ਿਲ;
			ret = write_cruft_pack(&opts, ਨਿਰਾ, 0ul, &ਨਾਂ,
					       &existing);
			ਜੇ (ret)
				ਜਾਓ cleanup;
		}
	}

	ਜੇ (po_ਦਲੀਲਾਂ.filter_options.choice && !drop_filtered) {
		ਬਣਤਰ write_pack_opts opts = {
			.po_ਦਲੀਲਾਂ = &po_ਦਲੀਲਾਂ,
			.destination = filter_ਮੰਜ਼ਿਲ,
			.packdir = packdir,
			.packtmp = packtmp,
		};

		ਜੇ (!opts.destination)
			opts.destination = packtmp;

		ret = write_filtered_pack(&opts, &existing, &ਨਾਂ);
		ਜੇ (ret)
			ਜਾਓ cleanup;
	}

	string_list_ਛਾਂਟੋ(&ਨਾਂ);

	odb_close(repo->objects);

	/*
	 * Ok we have prepared all new packfiles.
	 */
	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਨਾਂ)
		generated_pack_install(ਵਸਤੂ->util, ਵਸਤੂ->string, packdir,
				       packtmp);
	/* End of pack replacement. */

	ਜੇ (delete_redundant && pack_everything & ALL_INTO_ONE) {
		ਜੇ (write_midx == REPACK_WRITE_MIDX_INCREMENTAL)
			existing_packs_retain_midx_packs(&existing);
		existing_packs_mark_for_deletion(&existing, &ਨਾਂ);
	}

	ਜੇ (write_midx != REPACK_WRITE_MIDX_NONE) {
		ਬਣਤਰ repack_write_midx_opts opts = {
			.existing = &existing,
			.geometry = &geometry,
			.ਨਾਂ = &ਨਾਂ,
			.refs_snapshot = refs_snapshot ? get_tempfile_ਰਸਤਾ(refs_snapshot) : ਨਿਰਾ,
			.packdir = packdir,
			.show_ਤਰੱਕੀ = show_ਤਰੱਕੀ,
			.write_bitmaps = write_bitmaps > 0,
			.midx_must_contain_cruft = midx_must_contain_cruft,
			.midx_ਵੰਡੋ_ਗੁਣਕ = config_ctx.midx_ਵੰਡੋ_ਗੁਣਕ,
			.midx_new_layer_ਹੱਦ = config_ctx.midx_new_layer_ਹੱਦ,
			.ਬਹੁਲਕ = write_midx,
		};

		ret = repack_write_midx(&opts);
		ਜੇ (ret)
			ਜਾਓ cleanup;
	}

	odb_reprepare(repo->objects);

	ਜੇ (delete_redundant) {
		ਪੂਰਨ opts = 0;
		bool wrote_incremental_midx = write_midx == REPACK_WRITE_MIDX_INCREMENTAL;

		existing_packs_remove_redundant(&existing, packdir,
						wrote_incremental_midx);

		ਜੇ (geometry.ਵੰਡੋ_ਗੁਣਕ)
			pack_geometry_remove_redundant(&geometry, &ਨਾਂ,
						       &existing, packdir,
						       wrote_incremental_midx);
		ਜੇ (show_ਤਰੱਕੀ)
			opts |= PRUNE_PACKED_VERBOSE;
		prune_packed_objects(opts);

		ਜੇ (!keep_unreachable &&
		    (!(pack_everything & LOOSEN_UNREACHABLE) ||
		     unpack_unreachable) &&
		    is_repository_shallow(repo))
			prune_shallow(PRUNE_ਤੇਜ਼);
	}

	ਜੇ (ਲੜੀਵਾਰ_update_server_info)
		update_server_info(repo, 0);

	ਜੇ (git_env_bool(GIT_TEST_MULTI_PACK_ਸੂਚਕਾਂਕ, 0)) {
		ਬਣਤਰ odb_ਸਰੋਤ_files *files = odb_ਸਰੋਤ_files_downcast(existing.ਸਰੋਤ);
		ਬਿਨਾਨਿਸ਼ਾਨੀ flags = 0;

		ਜੇ (git_env_bool(GIT_TEST_MULTI_PACK_ਸੂਚਕਾਂਕ_WRITE_INCREMENTAL, 0))
			flags |= MIDX_WRITE_INCREMENTAL;
		write_midx_file(files->dirs->packed, ਨਿਰਾ, ਨਿਰਾ, flags);
	}

cleanup:
	string_list_clear(&keep_pack_list, 0);
	string_list_clear(&ਨਾਂ, 1);
	oidset_clear(&drop_oids);
	existing_packs_release(&existing);
	pack_geometry_release(&geometry);
	pack_objects_ਦਲੀਲਾਂ_release(&po_ਦਲੀਲਾਂ);
	pack_objects_ਦਲੀਲਾਂ_release(&cruft_po_ਦਲੀਲਾਂ);

	ਵਾਪਸ ret;
}
