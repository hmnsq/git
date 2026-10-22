#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "run-command.h"
#شامل "server-info.h"
#شامل "string-list.h"
#شامل "midx.h"
#شامل "packfile.h"
#شامل "prune-packed.h"
#شامل "promisor-remote.h"
#شامل "repack.h"
#شامل "shallow.h"
#شامل "list-objects-filter-options.h"
#شامل "oidset.h"
#شامل "hex.h"
#شامل "wt-status.h"
#شامل "read-cache-ll.h"

#تعریف_کرو ALL_INTO_ONE 1
#تعریف_کرو LOOSEN_UNREACHABLE 2
#تعریف_کرو PACK_CRUFT 4

#تعریف_کرو DELETE_PACK 1
#تعریف_کرو RETAIN_PACK 2

ساکن صحیح pack_everything;
ساکن صحیح write_bitmaps = -1;
ساکن صحیح use_delta_islands;
ساکن صحیح تسلسل_update_server_info = 1;
ساکن حرف *packdir, *packtmp_name, *packtmp;
ساکن صحیح midx_must_contain_cruft = 1;
ساکن صحیح drop_filtered;
ساکن صحیح dry_تسلسل;
ساکن صحیح write_bitmaps_given;

ساکن ثابت حرف *ثابت git_repack_usage[] = {
	N_("git repack [-a] [-A] [-d] [-f] [-F] [-l] [-n] [-q] [-b] [-m]\n"
	   "[--window=<n>] [--depth=<n>] [--threads=<n>] [--keep-pack=<pack-name>]\n"
	   "[--write-midx[=<mode>]] [--name-hash-version=<n>] [--path-walk]\n"
	   "[--filter=<filter-spec>] [--drop-filtered [--dry-run]]"),
	تہی
};

ساکن ثابت حرف incremental_bitmap_conflict_error[] = N_(
"Incremental repacks are incompatible with bitmap indexes.  Use\n"
"--no-write-bitmap-index or disable the pack.writeBitmaps configuration."
);

#تعریف_کرو DEFAULT_MIDX_بانٹو_ضربی_عدد 2
#تعریف_کرو DEFAULT_MIDX_NEW_LAYER_حد 8

ساخت repack_config_ctx {
	ساخت pack_objects_دلائل *po_دلائل;
	ساخت pack_objects_دلائل *cruft_po_دلائل;
	صحیح midx_بانٹو_ضربی_عدد;
	صحیح midx_new_layer_حد;
};

ساکن صحیح repack_config(ثابت حرف *var, ثابت حرف *قدر,
			 ثابت ساخت config_context *ctx, خالی *cb)
{
	ساخت repack_config_ctx *repack_ctx = cb;
	ساخت pack_objects_دلائل *po_دلائل = repack_ctx->po_دلائل;
	ساخت pack_objects_دلائل *cruft_po_دلائل = repack_ctx->cruft_po_دلائل;
	اگر (!strcmp(var, "repack.usedeltabaseoffset")) {
		po_دلائل->delta_بنیاد_انحراف = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "repack.packkeptobjects")) {
		po_دلائل->pack_kept_objects = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "repack.writebitmaps") ||
	    !strcmp(var, "pack.writebitmaps")) {
		write_bitmaps = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "repack.usedeltaislands")) {
		use_delta_islands = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (strcmp(var, "repack.updateserverinfo") == 0) {
		تسلسل_update_server_info = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "repack.cruftwindow")) {
		آزاد(cruft_po_دلائل->کھڑکی);
		واپس git_config_string(&cruft_po_دلائل->کھڑکی, var, قدر);
	}
	اگر (!strcmp(var, "repack.cruftwindowmemory")) {
		آزاد(cruft_po_دلائل->کھڑکی_حافظہ);
		واپس git_config_string(&cruft_po_دلائل->کھڑکی_حافظہ, var, قدر);
	}
	اگر (!strcmp(var, "repack.cruftdepth")) {
		آزاد(cruft_po_دلائل->گہرائی);
		واپس git_config_string(&cruft_po_دلائل->گہرائی, var, قدر);
	}
	اگر (!strcmp(var, "repack.cruftthreads")) {
		آزاد(cruft_po_دلائل->دھاگے);
		واپس git_config_string(&cruft_po_دلائل->دھاگے, var, قدر);
	}
	اگر (!strcmp(var, "repack.midxmustcontaincruft")) {
		midx_must_contain_cruft = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "repack.midxsplitfactor")) {
		repack_ctx->midx_بانٹو_ضربی_عدد = git_config_int(var, قدر,
							       ctx->kvi);
		واپس 0;
	}
	اگر (!strcmp(var, "repack.midxnewlayerthreshold")) {
		repack_ctx->midx_new_layer_حد = git_config_int(var, قدر,
								      ctx->kvi);
		واپس 0;
	}
	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن صحیح option_parse_write_bitmaps(ثابت ساخت option *opt, ثابت حرف *arg,
				      صحیح unset)
{
	صحیح *قدر = opt->قدر;

	BUG_ON_OPT_ARG(arg);
	اگر (unset)
		*قدر = 0;
	ورنہ
		*قدر = 1;

	write_bitmaps_given = 1;
	واپس 0;
}

ساکن صحیح option_parse_write_midx(ثابت ساخت option *opt, ثابت حرف *arg,
				   صحیح unset)
{
	شمار repack_write_midx_منوال *cfg = opt->قدر;

	اگر (unset) {
		*cfg = REPACK_WRITE_MIDX_NONE;
		واپس 0;
	}

	اگر (!arg || !*arg)
		*cfg = REPACK_WRITE_MIDX_DEFAULT;
	ورنہ اگر (!strcmp(arg, "incremental"))
		*cfg = REPACK_WRITE_MIDX_INCREMENTAL;
	ورنہ
		واپس error(_("unknown value for %s: %s"), opt->long_name, arg);

	واپس 0;
}

صحیح cmd_repack(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;
	ساخت string_list_شے *شے;
	ساخت string_list نام = STRING_LIST_INIT_DUP;
	ساخت existing_packs existing = EXISTING_PACKS_INIT;
	ساخت oidset drop_oids = OIDSET_INIT;
	ساخت pack_geometry geometry = { 0 };
	ساخت tempfile *refs_snapshot = تہی;
	صحیح i, ret;
	صحیح show_پیش_رفت;

	/* variables to be filled by option parsing */
	ساخت repack_config_ctx config_ctx;
	صحیح delete_redundant = 0;
	ثابت حرف *unpack_unreachable = تہی;
	صحیح keep_unreachable = 0;
	ساخت string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	ساخت pack_objects_دلائل po_دلائل = PACK_OBJECTS_دلائل_INIT;
	ساخت pack_objects_دلائل cruft_po_دلائل = PACK_OBJECTS_دلائل_INIT;
	شمار repack_write_midx_منوال write_midx = REPACK_WRITE_MIDX_NONE;
	ثابت حرف *cruft_expiration = تہی;
	ثابت حرف *expire_مقصد = تہی;
	ثابت حرف *filter_مقصد = تہی;
	ثابت حرف *opt_کھڑکی = تہی;
	ثابت حرف *opt_کھڑکی_حافظہ = تہی;
	ثابت حرف *opt_گہرائی = تہی;
	ثابت حرف *opt_دھاگے = تہی;
	غیرعلامتی طویل combine_cruft_نیچے_سائز = 0ul;

	ساخت option builtin_repack_options[] = {
		OPT_ثنائی_ہندسہ('a', تہی, &pack_everything,
				N_("pack everything in a single pack"), ALL_INTO_ONE),
		OPT_ثنائی_ہندسہ('A', تہی, &pack_everything,
				N_("same as -a, and turn unreachable objects loose"),
				   LOOSEN_UNREACHABLE | ALL_INTO_ONE),
		OPT_ثنائی_ہندسہ(0, "cruft", &pack_everything,
				N_("same as -a, pack unreachable cruft objects separately"),
				   PACK_CRUFT),
		OPT_STRING(0, "cruft-expiration", &cruft_expiration, N_("approxidate"),
				N_("with --cruft, expire objects older than this")),
		OPT_UNSIGNED(0, "combine-cruft-below-size",
			     &combine_cruft_نیچے_سائز,
			     N_("with --cruft, only repack cruft packs smaller than this")),
		OPT_UNSIGNED(0, "max-cruft-size", &cruft_po_دلائل.max_pack_سائز,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL('d', تہی, &delete_redundant,
				N_("remove redundant packs, and run git-prune-packed")),
		OPT_BOOL('f', تہی, &po_دلائل.no_reuse_delta,
				N_("pass --no-reuse-delta to git-pack-objects")),
		OPT_BOOL('F', تہی, &po_دلائل.no_reuse_object,
				N_("pass --no-reuse-object to git-pack-objects")),
		OPT_INTEGER(0, "name-hash-version", &po_دلائل.name_hash_نسخہ,
				N_("specify the name hash version to use for grouping similar objects by path")),
		OPT_BOOL(0, "path-walk", &po_دلائل.راستہ_walk,
				N_("pass --path-walk to git-pack-objects")),
		OPT_NEGBIT('n', تہی, &تسلسل_update_server_info,
				N_("do not run git-update-server-info"), 1),
		OPT__QUIET(&po_دلائل.quiet, N_("be quiet")),
		OPT_BOOL('l', "local", &po_دلائل.local,
				N_("pass --local to git-pack-objects")),
		OPT_CALLBACK_F('b', "write-bitmap-index", &write_bitmaps, تہی,
				N_("write bitmap index"),
				PARSE_OPT_NOARG, option_parse_write_bitmaps),
		OPT_BOOL('i', "delta-islands", &use_delta_islands,
				N_("pass --delta-islands to git-pack-objects")),
		OPT_STRING(0, "unpack-unreachable", &unpack_unreachable, N_("approxidate"),
				N_("with -A, do not loosen objects older than this")),
		OPT_BOOL('k', "keep-unreachable", &keep_unreachable,
				N_("with -a, repack unreachable objects")),
		OPT_STRING(0, "window", &opt_کھڑکی, N_("n"),
				N_("size of the window used for delta compression")),
		OPT_STRING(0, "window-memory", &opt_کھڑکی_حافظہ, N_("bytes"),
				N_("same as the above, but limit memory size instead of entries count")),
		OPT_STRING(0, "depth", &opt_گہرائی, N_("n"),
				N_("limits the maximum delta depth")),
		OPT_STRING(0, "threads", &opt_دھاگے, N_("n"),
				N_("limits the maximum number of threads")),
		OPT_UNSIGNED(0, "max-pack-size", &po_دلائل.max_pack_سائز,
			     N_("maximum size of each packfile")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&po_دلائل.filter_options),
		OPT_BOOL(0, "pack-kept-objects", &po_دلائل.pack_kept_objects,
				N_("repack objects in packs marked with .keep")),
		OPT_STRING_LIST(0, "keep-pack", &keep_pack_list, N_("name"),
				N_("do not repack this pack")),
		OPT_INTEGER('g', "geometric", &geometry.بانٹو_ضربی_عدد,
			    N_("find a geometric progression with factor <N>")),
		OPT_CALLBACK_F(0, "write-midx", &write_midx,
			   N_("mode"),
			   N_("write a multi-pack index of the resulting packs"),
			   PARSE_OPT_OPTARG, option_parse_write_midx),
		OPT_سیٹ_INT_F('m', تہی, &write_midx,
			   N_("write a multi-pack index of the resulting packs"),
			   REPACK_WRITE_MIDX_DEFAULT,
			   PARSE_OPT_HIDDEN),
		OPT_STRING(0, "expire-to", &expire_مقصد, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_STRING(0, "filter-to", &filter_مقصد, N_("dir"),
			   N_("pack prefix to store a pack containing filtered out objects")),
		OPT_BOOL(0, "drop-filtered", &drop_filtered,
				N_("delete filtered out objects (requires --filter)")),
		OPT_BOOL(0, "dry-run", &dry_تسلسل,
				N_("only show which objects would be dropped")),
		OPT_انتہا()
	};

	list_objects_filter_init(&po_دلائل.filter_options);

	memset(&config_ctx, 0, حجم(config_ctx));
	config_ctx.po_دلائل = &po_دلائل;
	config_ctx.cruft_po_دلائل = &cruft_po_دلائل;
	config_ctx.midx_بانٹو_ضربی_عدد = DEFAULT_MIDX_بانٹو_ضربی_عدد;
	config_ctx.midx_new_layer_حد = DEFAULT_MIDX_NEW_LAYER_حد;

	repo_config(repo, repack_config, &config_ctx);

	argc = parse_options(argc, argv, سابقہ, builtin_repack_options,
				git_repack_usage, 0);

	po_دلائل.کھڑکی = xstrdup_or_null(opt_کھڑکی);
	po_دلائل.کھڑکی_حافظہ = xstrdup_or_null(opt_کھڑکی_حافظہ);
	po_دلائل.گہرائی = xstrdup_or_null(opt_گہرائی);
	po_دلائل.دھاگے = xstrdup_or_null(opt_دھاگے);

	die_for_incompatible_opt2(drop_filtered, "--drop-filtered",
		!!filter_مقصد, "--filter-to");

	اگر (dry_تسلسل && !drop_filtered)
		die(_("--dry-run only takes effect with --drop-filtered"));

	اگر (drop_filtered) {
		اگر (!po_دلائل.filter_options.choice)
			die(_("--drop-filtered requires --filter"));

		اگر (!(pack_everything & ALL_INTO_ONE))
			die(_("--drop-filtered requires -a"));

		/*
		 * Only blob:limit=<n> is supported for now. Reject other
		 * filter choices early, before walking the object database.
		 */
		اگر (po_دلائل.filter_options.choice != LOFC_BLOB_حد_بندی)
			die(_("--drop-filtered only supports --filter=blob:limit=<n> for now"));

		/*
		 * An explicit -b on the command line is a conflict we have to
		 * report; a bitmap setting from config is silently overridden
		 * for the duration of the command.
		 */
		اگر (write_bitmaps_given && write_bitmaps > 0)
			die(_("options '%s' and '%s' cannot be used together"),
				"--drop-filtered", "--write-bitmap-index");

		/*
		 * Without a promisor remote there is nowhere to re-fetch the
		 * dropped objects from, so dropping them would be permanent
		 * data loss.
		 */
		اگر (!repo_has_promisor_remote(repo))
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
		اگر (!is_bare_repository(repo)) {
			ساخت wt_حالت_state state = { 0 };

			wt_حالت_get_state(repo, &state, 0);
			اگر (state.ملاؤ_اندر_پیش_رفت || state.revert_اندر_پیش_رفت ||
			    state.rebase_اندر_پیش_رفت || state.bisect_اندر_پیش_رفت ||
			    state.cherry_pick_اندر_پیش_رفت || state.am_اندر_پیش_رفت ||
			    state.rebase_interactive_اندر_پیش_رفت) {
				wt_حالت_state_free_buffers(&state);
				die(_("--drop-filtered cannot be used while "
				      "another operation (merge, rebase, am, "
				      "cherry-pick, revert, or bisect) is in "
				      "progress"));
			}
			wt_حالت_state_free_buffers(&state);
		}

		write_bitmaps = 0;

		/*
		 * Dropping objects means rebuilding the promisor packs
		 * without them and then removing the old packs, so the
		 * redundant packs must be deleted. Imply -d on a real run.
		 */
		اگر (!dry_تسلسل)
			delete_redundant = 1;

		ret = enumerate_promisor_blobs(repo, &po_دلائل.filter_options, &drop_oids);

		اگر (ret)
			جاؤ cleanup;

		/*
		 * Refuse to drop blobs that the current index references.
		 * Such a blob would only be lazily re-fetched by the next
		 * command that touches the worktree, so dropping it reclaims
		 * nothing. This guard just avoids that churn. Bare
		 * repositories have no index, so the check is skipped there.
		 */
		اگر (!is_bare_repository(repo) && oidset_سائز(&drop_oids)) {
			ساخت اشاریہ_state *istate = repo->اشاریہ;
			غیرعلامتی صحیح i;

			اگر (repo_read_اشاریہ(repo) < 0)
				die(_("could not read the index"));

			برائے (i = 0; i < istate->cache_nr; i++) {
				ثابت ساخت cache_اندراج *ce = istate->cache[i];

				اگر (oidset_شامل_ہے(&drop_oids, &ce->oid))
					die(_("cannot drop '%s' (%s): it is referenced by the current index"),
						ce->name, oid_مقصد_hex(&ce->oid));
			}
		}

		اگر (dry_تسلسل) {
			ساخت oidset_iter iter;
			ثابت ساخت object_id *oid;

			oidset_iter_init(&drop_oids, &iter);
			جب ((oid = oidset_iter_اگلا(&iter)))
				لکھو("%s\n", oid_مقصد_hex(oid));
		}
	}

	اگر (delete_redundant && repo->repository_format_precious_objects)
		die(_("cannot delete packs in a precious-objects repo"));

	die_for_incompatible_opt3(unpack_unreachable || (pack_everything & LOOSEN_UNREACHABLE), "-A",
				  keep_unreachable, "-k/--keep-unreachable",
				  pack_everything & PACK_CRUFT, "--cruft");

	اگر (pack_everything & PACK_CRUFT)
		pack_everything |= ALL_INTO_ONE;

	اگر (write_bitmaps < 0) {
		اگر (write_midx == REPACK_WRITE_MIDX_NONE &&
		    (!(pack_everything & ALL_INTO_ONE) || !is_bare_repository(repo)))
			write_bitmaps = 0;
	}
	اگر (po_دلائل.pack_kept_objects < 0)
		po_دلائل.pack_kept_objects = write_bitmaps > 0 &&
			write_midx == REPACK_WRITE_MIDX_NONE;

	اگر (write_bitmaps && !(pack_everything & ALL_INTO_ONE) &&
	    write_midx == REPACK_WRITE_MIDX_NONE)
		die(_(incremental_bitmap_conflict_error));

	اگر (write_bitmaps && po_دلائل.local &&
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

	اگر (config_ctx.midx_بانٹو_ضربی_عدد < 2)
		die(_("invalid value for %s: %d"), "--midx-split-factor",
		    config_ctx.midx_بانٹو_ضربی_عدد);
	اگر (config_ctx.midx_new_layer_حد < 1)
		die(_("invalid value for %s: %d"), "--midx-new-layer-threshold",
		    config_ctx.midx_new_layer_حد);

	اگر (write_midx != REPACK_WRITE_MIDX_NONE && write_bitmaps) {
		ساخت strbuf راستہ = STRBUF_INIT;

		strbuf_addf(&راستہ, "%s/%s_XXXXXX",
			    repo_get_object_directory(repo),
			    "bitmap-ref-tips");

		refs_snapshot = xmks_tempfile(راستہ.buf);
		midx_snapshot_refs(repo, refs_snapshot);

		strbuf_release(&راستہ);
	}

	packdir = mkpathdup("%s/pack", repo_get_object_directory(repo));
	packtmp_name = xstrfmt(".tmp-%d-pack", (صحیح)getpid());
	packtmp = mkpathdup("%s/%s", packdir, packtmp_name);

	existing.repo = repo;
	existing_packs_collect(&existing, &keep_pack_list);

	اگر (geometry.بانٹو_ضربی_عدد) {
		اگر (pack_everything)
			die(_("options '%s' and '%s' cannot be used together"), "--geometric", "-A/-a");
		اگر (write_midx == REPACK_WRITE_MIDX_INCREMENTAL) {
			geometry.midx_layer_حد = config_ctx.midx_new_layer_حد;
			geometry.midx_layer_حد_سیٹ = true;
		}
		pack_geometry_init(&geometry, &existing, &po_دلائل);
		pack_geometry_بانٹو(&geometry);
	}

	prepare_pack_objects(&cmd, &po_دلائل, packtmp);

	show_پیش_رفت = !po_دلائل.quiet && isatty(2);

	strvec_ڈالو(&cmd.دلائل, "--keep-true-parents");
	برائے (i = 0; i < keep_pack_list.nr; i++)
		strvec_pushf(&cmd.دلائل, "--keep-pack=%s",
			     keep_pack_list.اشیاء[i].string);
	strvec_ڈالو(&cmd.دلائل, "--non-empty");
	اگر (!geometry.بانٹو_ضربی_عدد) {
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
		strvec_ڈالو(&cmd.دلائل, "--all");
		strvec_ڈالو(&cmd.دلائل, "--reflog");
		strvec_ڈالو(&cmd.دلائل, "--indexed-objects");
	}
	اگر (repo_has_promisor_remote(repo))
		strvec_ڈالو(&cmd.دلائل, "--exclude-promisor-objects");
	اگر (write_midx == REPACK_WRITE_MIDX_NONE) {
		اگر (write_bitmaps > 0)
			strvec_ڈالو(&cmd.دلائل, "--write-bitmap-index");
		ورنہ اگر (write_bitmaps < 0)
			strvec_ڈالو(&cmd.دلائل, "--write-bitmap-index-quiet");
	}
	اگر (use_delta_islands)
		strvec_ڈالو(&cmd.دلائل, "--delta-islands");

	اگر (pack_everything & ALL_INTO_ONE) {
		repack_promisor_objects(repo, &po_دلائل, &نام, packtmp,
			(drop_filtered && !dry_تسلسل) ? &drop_oids : تہی);

		اگر (existing_packs_has_non_kept(&existing) &&
		    delete_redundant &&
		    !(pack_everything & PACK_CRUFT)) {
			for_each_string_list_شے(شے, &نام) {
				strvec_pushf(&cmd.دلائل, "--keep-pack=%s-%s.pack",
					     packtmp_name, شے->string);
			}
			اگر (unpack_unreachable) {
				strvec_pushf(&cmd.دلائل,
					     "--unpack-unreachable=%s",
					     unpack_unreachable);
			} ورنہ اگر (pack_everything & LOOSEN_UNREACHABLE) {
				strvec_ڈالو(&cmd.دلائل,
					    "--unpack-unreachable");
			} ورنہ اگر (keep_unreachable) {
				strvec_ڈالو(&cmd.دلائل, "--keep-unreachable");
			}
		}

		اگر (keep_unreachable && delete_redundant &&
		    !(pack_everything & PACK_CRUFT))
			strvec_ڈالو(&cmd.دلائل, "--pack-loose-unreachable");
	} ورنہ اگر (geometry.بانٹو_ضربی_عدد) {
		pack_geometry_repack_promisors(repo, &po_دلائل, &geometry,
					       &نام, packtmp);

		اگر (midx_must_contain_cruft)
			strvec_ڈالو(&cmd.دلائل, "--stdin-packs");
		ورنہ
			strvec_ڈالو(&cmd.دلائل, "--stdin-packs=follow");
		strvec_ڈالو(&cmd.دلائل, "--unpacked");
	} ورنہ {
		strvec_ڈالو(&cmd.دلائل, "--unpacked");
		strvec_ڈالو(&cmd.دلائل, "--incremental");
	}

	اگر (po_دلائل.filter_options.choice)
		strvec_pushf(&cmd.دلائل, "--filter=%s",
			     expand_list_objects_filter_spec(&po_دلائل.filter_options));
	ورنہ اگر (filter_مقصد)
		die(_("option '%s' can only be used along with '%s'"), "--filter-to", "--filter");

	اگر (geometry.بانٹو_ضربی_عدد)
		cmd.اندر = -1;
	ورنہ
		cmd.no_stdin = 1;

	ret = آغاز_command(&cmd);
	اگر (ret)
		جاؤ cleanup;

	اگر (geometry.بانٹو_ضربی_عدد) {
		FILE *اندر = xfdopen(cmd.اندر, "w");
		/*
		 * The resulting pack should contain all objects in packs that
		 * are going to be rolled up, but exclude objects in packs which
		 * are being left alone.
		 */
		برائے (i = 0; i < geometry.بانٹو; i++)
			fprintf(اندر, "%s\n", pack_basename(geometry.pack[i]));
		برائے (i = geometry.بانٹو; i < geometry.pack_nr; i++) {
			ثابت حرف *basename = pack_basename(geometry.pack[i]);
			حرف marker = '^';

			اگر (!midx_must_contain_cruft &&
			    !string_list_has_string(&existing.midx_packs,
						    basename)) {
				/*
				 * Assume non-MIDX'd packs are not
				 * necessarily closed under
				 * reachability.
				 */
				marker = '!';
			}

			fprintf(اندر, "%c%s\n", marker, basename);
		}
		fclose(اندر);
	}

	{
		ساخت write_pack_opts opts = {
			.packdir = packdir,
			.destination = packdir,
			.packtmp = packtmp,
		};
		ret = اختتام_pack_objects_cmd(repo->hash_algo, &opts, &cmd,
					      &نام);
		اگر (ret)
			جاؤ cleanup;
	}

	اگر (!نام.nr) {
		ساخت odb_ماخذ_files *files = odb_ماخذ_files_downcast(existing.ماخذ);

		اگر (!po_دلائل.quiet)
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
		اگر (!get_multi_pack_اشاریہ(files->dirs->packed))
			midx_must_contain_cruft = 1;
	}

	اگر (pack_everything & PACK_CRUFT) {
		ساخت write_pack_opts opts = {
			.po_دلائل = &cruft_po_دلائل,
			.destination = packtmp,
			.packtmp = packtmp,
			.packdir = packdir,
		};

		اگر (!cruft_po_دلائل.کھڑکی)
			cruft_po_دلائل.کھڑکی = xstrdup_or_null(po_دلائل.کھڑکی);
		اگر (!cruft_po_دلائل.کھڑکی_حافظہ)
			cruft_po_دلائل.کھڑکی_حافظہ = xstrdup_or_null(po_دلائل.کھڑکی_حافظہ);
		اگر (!cruft_po_دلائل.گہرائی)
			cruft_po_دلائل.گہرائی = xstrdup_or_null(po_دلائل.گہرائی);
		اگر (!cruft_po_دلائل.دھاگے)
			cruft_po_دلائل.دھاگے = xstrdup_or_null(po_دلائل.دھاگے);
		اگر (!cruft_po_دلائل.max_pack_سائز)
			cruft_po_دلائل.max_pack_سائز = po_دلائل.max_pack_سائز;

		cruft_po_دلائل.local = po_دلائل.local;
		cruft_po_دلائل.quiet = po_دلائل.quiet;
		cruft_po_دلائل.delta_بنیاد_انحراف = po_دلائل.delta_بنیاد_انحراف;
		cruft_po_دلائل.pack_kept_objects = 0;

		ret = write_cruft_pack(&opts, cruft_expiration,
				       combine_cruft_نیچے_سائز, &نام,
				       &existing);
		اگر (ret)
			جاؤ cleanup;

		اگر (delete_redundant && expire_مقصد) {
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
			opts.destination = expire_مقصد;
			ret = write_cruft_pack(&opts, تہی, 0ul, &نام,
					       &existing);
			اگر (ret)
				جاؤ cleanup;
		}
	}

	اگر (po_دلائل.filter_options.choice && !drop_filtered) {
		ساخت write_pack_opts opts = {
			.po_دلائل = &po_دلائل,
			.destination = filter_مقصد,
			.packdir = packdir,
			.packtmp = packtmp,
		};

		اگر (!opts.destination)
			opts.destination = packtmp;

		ret = write_filtered_pack(&opts, &existing, &نام);
		اگر (ret)
			جاؤ cleanup;
	}

	string_list_ترتیب_دو(&نام);

	odb_close(repo->objects);

	/*
	 * Ok we have prepared all new packfiles.
	 */
	for_each_string_list_شے(شے, &نام)
		generated_pack_install(شے->util, شے->string, packdir,
				       packtmp);
	/* End of pack replacement. */

	اگر (delete_redundant && pack_everything & ALL_INTO_ONE) {
		اگر (write_midx == REPACK_WRITE_MIDX_INCREMENTAL)
			existing_packs_retain_midx_packs(&existing);
		existing_packs_mark_for_deletion(&existing, &نام);
	}

	اگر (write_midx != REPACK_WRITE_MIDX_NONE) {
		ساخت repack_write_midx_opts opts = {
			.existing = &existing,
			.geometry = &geometry,
			.نام = &نام,
			.refs_snapshot = refs_snapshot ? get_tempfile_راستہ(refs_snapshot) : تہی,
			.packdir = packdir,
			.show_پیش_رفت = show_پیش_رفت,
			.write_bitmaps = write_bitmaps > 0,
			.midx_must_contain_cruft = midx_must_contain_cruft,
			.midx_بانٹو_ضربی_عدد = config_ctx.midx_بانٹو_ضربی_عدد,
			.midx_new_layer_حد = config_ctx.midx_new_layer_حد,
			.منوال = write_midx,
		};

		ret = repack_write_midx(&opts);
		اگر (ret)
			جاؤ cleanup;
	}

	odb_reprepare(repo->objects);

	اگر (delete_redundant) {
		صحیح opts = 0;
		bool wrote_incremental_midx = write_midx == REPACK_WRITE_MIDX_INCREMENTAL;

		existing_packs_remove_redundant(&existing, packdir,
						wrote_incremental_midx);

		اگر (geometry.بانٹو_ضربی_عدد)
			pack_geometry_remove_redundant(&geometry, &نام,
						       &existing, packdir,
						       wrote_incremental_midx);
		اگر (show_پیش_رفت)
			opts |= PRUNE_PACKED_VERBOSE;
		prune_packed_objects(opts);

		اگر (!keep_unreachable &&
		    (!(pack_everything & LOOSEN_UNREACHABLE) ||
		     unpack_unreachable) &&
		    is_repository_shallow(repo))
			prune_shallow(PRUNE_تیز);
	}

	اگر (تسلسل_update_server_info)
		update_server_info(repo, 0);

	اگر (git_env_bool(GIT_TEST_MULTI_PACK_اشاریہ, 0)) {
		ساخت odb_ماخذ_files *files = odb_ماخذ_files_downcast(existing.ماخذ);
		غیرعلامتی flags = 0;

		اگر (git_env_bool(GIT_TEST_MULTI_PACK_اشاریہ_WRITE_INCREMENTAL, 0))
			flags |= MIDX_WRITE_INCREMENTAL;
		write_midx_file(files->dirs->packed, تہی, تہی, flags);
	}

cleanup:
	string_list_clear(&keep_pack_list, 0);
	string_list_clear(&نام, 1);
	oidset_clear(&drop_oids);
	existing_packs_release(&existing);
	pack_geometry_release(&geometry);
	pack_objects_دلائل_release(&po_دلائل);
	pack_objects_دلائل_release(&cruft_po_دلائل);

	واپس ret;
}
