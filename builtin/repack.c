#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "run-command.h"
#ضمن "server-info.h"
#ضمن "string-list.h"
#ضمن "midx.h"
#ضمن "packfile.h"
#ضمن "prune-packed.h"
#ضمن "promisor-remote.h"
#ضمن "repack.h"
#ضمن "shallow.h"
#ضمن "list-objects-filter-options.h"
#ضمن "oidset.h"
#ضمن "hex.h"
#ضمن "wt-status.h"
#ضمن "read-cache-ll.h"

#تعریف_کن ALL_INTO_ONE 1
#تعریف_کن LOOSEN_UNREACHABLE 2
#تعریف_کن PACK_CRUFT 4

#تعریف_کن DELETE_PACK 1
#تعریف_کن RETAIN_PACK 2

ساكن صحيح pack_everything;
ساكن صحيح write_bitmaps = -1;
ساكن صحيح use_delta_islands;
ساكن صحيح پیاپی_update_server_info = 1;
ساكن حرف *packdir, *packtmp_name, *packtmp;
ساكن صحيح midx_must_contain_cruft = 1;
ساكن صحيح drop_filtered;
ساكن صحيح dry_پیاپی;
ساكن صحيح write_bitmaps_given;

ساكن ثابت حرف *ثابت git_repack_usage[] = {
	N_("git repack [-a] [-A] [-d] [-f] [-F] [-l] [-n] [-q] [-b] [-m]\n"
	   "[--window=<n>] [--depth=<n>] [--threads=<n>] [--keep-pack=<pack-name>]\n"
	   "[--write-midx[=<mode>]] [--name-hash-version=<n>] [--path-walk]\n"
	   "[--filter=<filter-spec>] [--drop-filtered [--dry-run]]"),
	عدم
};

ساكن ثابت حرف incremental_bitmap_conflict_error[] = N_(
"Incremental repacks are incompatible with bitmap indexes.  Use\n"
"--no-write-bitmap-index or disable the pack.writeBitmaps configuration."
);

#تعریف_کن DEFAULT_MIDX_تقسیم_ضریب 2
#تعریف_کن DEFAULT_MIDX_NEW_LAYER_آستانه 8

بنية repack_config_ctx {
	بنية pack_objects_آرگومانها *po_آرگومانها;
	بنية pack_objects_آرگومانها *cruft_po_آرگومانها;
	صحيح midx_تقسیم_ضریب;
	صحيح midx_new_layer_آستانه;
};

ساكن صحيح repack_config(ثابت حرف *var, ثابت حرف *مقدار,
			 ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية repack_config_ctx *repack_ctx = cb;
	بنية pack_objects_آرگومانها *po_آرگومانها = repack_ctx->po_آرگومانها;
	بنية pack_objects_آرگومانها *cruft_po_آرگومانها = repack_ctx->cruft_po_آرگومانها;
	إذا (!strcmp(var, "repack.usedeltabaseoffset")) {
		po_آرگومانها->delta_پایه_جابه_جایی = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "repack.packkeptobjects")) {
		po_آرگومانها->pack_kept_objects = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "repack.writebitmaps") ||
	    !strcmp(var, "pack.writebitmaps")) {
		write_bitmaps = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "repack.usedeltaislands")) {
		use_delta_islands = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (strcmp(var, "repack.updateserverinfo") == 0) {
		پیاپی_update_server_info = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "repack.cruftwindow")) {
		حرر(cruft_po_آرگومانها->پنجره);
		أرجع git_config_string(&cruft_po_آرگومانها->پنجره, var, مقدار);
	}
	إذا (!strcmp(var, "repack.cruftwindowmemory")) {
		حرر(cruft_po_آرگومانها->پنجره_حافظه);
		أرجع git_config_string(&cruft_po_آرگومانها->پنجره_حافظه, var, مقدار);
	}
	إذا (!strcmp(var, "repack.cruftdepth")) {
		حرر(cruft_po_آرگومانها->عمق);
		أرجع git_config_string(&cruft_po_آرگومانها->عمق, var, مقدار);
	}
	إذا (!strcmp(var, "repack.cruftthreads")) {
		حرر(cruft_po_آرگومانها->نخها);
		أرجع git_config_string(&cruft_po_آرگومانها->نخها, var, مقدار);
	}
	إذا (!strcmp(var, "repack.midxmustcontaincruft")) {
		midx_must_contain_cruft = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "repack.midxsplitfactor")) {
		repack_ctx->midx_تقسیم_ضریب = git_config_int(var, مقدار,
							       ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(var, "repack.midxnewlayerthreshold")) {
		repack_ctx->midx_new_layer_آستانه = git_config_int(var, مقدار,
								      ctx->kvi);
		أرجع 0;
	}
	أرجع git_default_config(var, مقدار, ctx, cb);
}

ساكن صحيح option_parse_write_bitmaps(ثابت بنية option *opt, ثابت حرف *arg,
				      صحيح unset)
{
	صحيح *مقدار = opt->مقدار;

	BUG_ON_OPT_ARG(arg);
	إذا (unset)
		*مقدار = 0;
	وإلا
		*مقدار = 1;

	write_bitmaps_given = 1;
	أرجع 0;
}

ساكن صحيح option_parse_write_midx(ثابت بنية option *opt, ثابت حرف *arg,
				   صحيح unset)
{
	تعداد repack_write_midx_نما *cfg = opt->مقدار;

	إذا (unset) {
		*cfg = REPACK_WRITE_MIDX_NONE;
		أرجع 0;
	}

	إذا (!arg || !*arg)
		*cfg = REPACK_WRITE_MIDX_DEFAULT;
	وإلا إذا (!strcmp(arg, "incremental"))
		*cfg = REPACK_WRITE_MIDX_INCREMENTAL;
	وإلا
		أرجع error(_("unknown value for %s: %s"), opt->long_name, arg);

	أرجع 0;
}

صحيح cmd_repack(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *پیشوند,
	       بنية repository *repo)
{
	بنية فرزند_process cmd = فرزند_PROCESS_INIT;
	بنية string_list_قلم *قلم;
	بنية string_list نامها = STRING_LIST_INIT_DUP;
	بنية existing_packs existing = EXISTING_PACKS_INIT;
	بنية oidset drop_oids = OIDSET_INIT;
	بنية pack_geometry geometry = { 0 };
	بنية tempfile *refs_snapshot = عدم;
	صحيح i, ret;
	صحيح show_پیشرفت;

	/* variables to be filled by option parsing */
	بنية repack_config_ctx config_ctx;
	صحيح delete_redundant = 0;
	ثابت حرف *unpack_unreachable = عدم;
	صحيح keep_unreachable = 0;
	بنية string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	بنية pack_objects_آرگومانها po_آرگومانها = PACK_OBJECTS_آرگومانها_INIT;
	بنية pack_objects_آرگومانها cruft_po_آرگومانها = PACK_OBJECTS_آرگومانها_INIT;
	تعداد repack_write_midx_نما write_midx = REPACK_WRITE_MIDX_NONE;
	ثابت حرف *cruft_expiration = عدم;
	ثابت حرف *expire_مقصد = عدم;
	ثابت حرف *filter_مقصد = عدم;
	ثابت حرف *opt_پنجره = عدم;
	ثابت حرف *opt_پنجره_حافظه = عدم;
	ثابت حرف *opt_عمق = عدم;
	ثابت حرف *opt_نخها = عدم;
	بیعلامت دراز combine_cruft_زیر_اندازه = 0ul;

	بنية option builtin_repack_options[] = {
		OPT_بیت('a', عدم, &pack_everything,
				N_("pack everything in a single pack"), ALL_INTO_ONE),
		OPT_بیت('A', عدم, &pack_everything,
				N_("same as -a, and turn unreachable objects loose"),
				   LOOSEN_UNREACHABLE | ALL_INTO_ONE),
		OPT_بیت(0, "cruft", &pack_everything,
				N_("same as -a, pack unreachable cruft objects separately"),
				   PACK_CRUFT),
		OPT_STRING(0, "cruft-expiration", &cruft_expiration, N_("approxidate"),
				N_("with --cruft, expire objects older than this")),
		OPT_UNSIGNED(0, "combine-cruft-below-size",
			     &combine_cruft_زیر_اندازه,
			     N_("with --cruft, only repack cruft packs smaller than this")),
		OPT_UNSIGNED(0, "max-cruft-size", &cruft_po_آرگومانها.max_pack_اندازه,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL('d', عدم, &delete_redundant,
				N_("remove redundant packs, and run git-prune-packed")),
		OPT_BOOL('f', عدم, &po_آرگومانها.no_reuse_delta,
				N_("pass --no-reuse-delta to git-pack-objects")),
		OPT_BOOL('F', عدم, &po_آرگومانها.no_reuse_object,
				N_("pass --no-reuse-object to git-pack-objects")),
		OPT_INTEGER(0, "name-hash-version", &po_آرگومانها.name_hash_نسخه,
				N_("specify the name hash version to use for grouping similar objects by path")),
		OPT_BOOL(0, "path-walk", &po_آرگومانها.مسیر_walk,
				N_("pass --path-walk to git-pack-objects")),
		OPT_NEGBIT('n', عدم, &پیاپی_update_server_info,
				N_("do not run git-update-server-info"), 1),
		OPT__QUIET(&po_آرگومانها.quiet, N_("be quiet")),
		OPT_BOOL('l', "local", &po_آرگومانها.local,
				N_("pass --local to git-pack-objects")),
		OPT_CALLBACK_F('b', "write-bitmap-index", &write_bitmaps, عدم,
				N_("write bitmap index"),
				PARSE_OPT_NOARG, option_parse_write_bitmaps),
		OPT_BOOL('i', "delta-islands", &use_delta_islands,
				N_("pass --delta-islands to git-pack-objects")),
		OPT_STRING(0, "unpack-unreachable", &unpack_unreachable, N_("approxidate"),
				N_("with -A, do not loosen objects older than this")),
		OPT_BOOL('k', "keep-unreachable", &keep_unreachable,
				N_("with -a, repack unreachable objects")),
		OPT_STRING(0, "window", &opt_پنجره, N_("n"),
				N_("size of the window used for delta compression")),
		OPT_STRING(0, "window-memory", &opt_پنجره_حافظه, N_("bytes"),
				N_("same as the above, but limit memory size instead of entries count")),
		OPT_STRING(0, "depth", &opt_عمق, N_("n"),
				N_("limits the maximum delta depth")),
		OPT_STRING(0, "threads", &opt_نخها, N_("n"),
				N_("limits the maximum number of threads")),
		OPT_UNSIGNED(0, "max-pack-size", &po_آرگومانها.max_pack_اندازه,
			     N_("maximum size of each packfile")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&po_آرگومانها.filter_options),
		OPT_BOOL(0, "pack-kept-objects", &po_آرگومانها.pack_kept_objects,
				N_("repack objects in packs marked with .keep")),
		OPT_STRING_LIST(0, "keep-pack", &keep_pack_list, N_("name"),
				N_("do not repack this pack")),
		OPT_INTEGER('g', "geometric", &geometry.تقسیم_ضریب,
			    N_("find a geometric progression with factor <N>")),
		OPT_CALLBACK_F(0, "write-midx", &write_midx,
			   N_("mode"),
			   N_("write a multi-pack index of the resulting packs"),
			   PARSE_OPT_OPTARG, option_parse_write_midx),
		OPT_مجموعه_INT_F('m', عدم, &write_midx,
			   N_("write a multi-pack index of the resulting packs"),
			   REPACK_WRITE_MIDX_DEFAULT,
			   PARSE_OPT_HIDDEN),
		OPT_STRING(0, "expire-to", &expire_مقصد, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_STRING(0, "filter-to", &filter_مقصد, N_("dir"),
			   N_("pack prefix to store a pack containing filtered out objects")),
		OPT_BOOL(0, "drop-filtered", &drop_filtered,
				N_("delete filtered out objects (requires --filter)")),
		OPT_BOOL(0, "dry-run", &dry_پیاپی,
				N_("only show which objects would be dropped")),
		OPT_انتها()
	};

	list_objects_filter_init(&po_آرگومانها.filter_options);

	memset(&config_ctx, 0, حجم(config_ctx));
	config_ctx.po_آرگومانها = &po_آرگومانها;
	config_ctx.cruft_po_آرگومانها = &cruft_po_آرگومانها;
	config_ctx.midx_تقسیم_ضریب = DEFAULT_MIDX_تقسیم_ضریب;
	config_ctx.midx_new_layer_آستانه = DEFAULT_MIDX_NEW_LAYER_آستانه;

	repo_config(repo, repack_config, &config_ctx);

	argc = parse_options(argc, argv, پیشوند, builtin_repack_options,
				git_repack_usage, 0);

	po_آرگومانها.پنجره = xstrdup_or_null(opt_پنجره);
	po_آرگومانها.پنجره_حافظه = xstrdup_or_null(opt_پنجره_حافظه);
	po_آرگومانها.عمق = xstrdup_or_null(opt_عمق);
	po_آرگومانها.نخها = xstrdup_or_null(opt_نخها);

	die_for_incompatible_opt2(drop_filtered, "--drop-filtered",
		!!filter_مقصد, "--filter-to");

	إذا (dry_پیاپی && !drop_filtered)
		die(_("--dry-run only takes effect with --drop-filtered"));

	إذا (drop_filtered) {
		إذا (!po_آرگومانها.filter_options.choice)
			die(_("--drop-filtered requires --filter"));

		إذا (!(pack_everything & ALL_INTO_ONE))
			die(_("--drop-filtered requires -a"));

		/*
		 * Only blob:limit=<n> is supported for now. Reject other
		 * filter choices early, before walking the object database.
		 */
		إذا (po_آرگومانها.filter_options.choice != LOFC_BLOB_حد)
			die(_("--drop-filtered only supports --filter=blob:limit=<n> for now"));

		/*
		 * An explicit -b on the command line is a conflict we have to
		 * report; a bitmap setting from config is silently overridden
		 * for the duration of the command.
		 */
		إذا (write_bitmaps_given && write_bitmaps > 0)
			die(_("options '%s' and '%s' cannot be used together"),
				"--drop-filtered", "--write-bitmap-index");

		/*
		 * Without a promisor remote there is nowhere to re-fetch the
		 * dropped objects from, so dropping them would be permanent
		 * data loss.
		 */
		إذا (!repo_has_promisor_remote(repo))
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
		إذا (!is_bare_repository(repo)) {
			بنية wt_وضعیت_state state = { 0 };

			wt_وضعیت_get_state(repo, &state, 0);
			إذا (state.ادغام_درون_پیشرفت || state.revert_درون_پیشرفت ||
			    state.rebase_درون_پیشرفت || state.bisect_درون_پیشرفت ||
			    state.cherry_pick_درون_پیشرفت || state.am_درون_پیشرفت ||
			    state.rebase_interactive_درون_پیشرفت) {
				wt_وضعیت_state_free_buffers(&state);
				die(_("--drop-filtered cannot be used while "
				      "another operation (merge, rebase, am, "
				      "cherry-pick, revert, or bisect) is in "
				      "progress"));
			}
			wt_وضعیت_state_free_buffers(&state);
		}

		write_bitmaps = 0;

		/*
		 * Dropping objects means rebuilding the promisor packs
		 * without them and then removing the old packs, so the
		 * redundant packs must be deleted. Imply -d on a real run.
		 */
		إذا (!dry_پیاپی)
			delete_redundant = 1;

		ret = enumerate_promisor_blobs(repo, &po_آرگومانها.filter_options, &drop_oids);

		إذا (ret)
			اذهب cleanup;

		/*
		 * Refuse to drop blobs that the current index references.
		 * Such a blob would only be lazily re-fetched by the next
		 * command that touches the worktree, so dropping it reclaims
		 * nothing. This guard just avoids that churn. Bare
		 * repositories have no index, so the check is skipped there.
		 */
		إذا (!is_bare_repository(repo) && oidset_اندازه(&drop_oids)) {
			بنية شاخص_state *istate = repo->شاخص;
			بیعلامت صحيح i;

			إذا (repo_read_شاخص(repo) < 0)
				die(_("could not read the index"));

			لكل (i = 0; i < istate->cache_nr; i++) {
				ثابت بنية cache_مدخل *ce = istate->cache[i];

				إذا (oidset_شامل_است(&drop_oids, &ce->oid))
					die(_("cannot drop '%s' (%s): it is referenced by the current index"),
						ce->name, oid_مقصد_hex(&ce->oid));
			}
		}

		إذا (dry_پیاپی) {
			بنية oidset_iter iter;
			ثابت بنية object_id *oid;

			oidset_iter_init(&drop_oids, &iter);
			طالما ((oid = oidset_iter_بعدی(&iter)))
				چاپ_کن("%s\n", oid_مقصد_hex(oid));
		}
	}

	إذا (delete_redundant && repo->repository_format_precious_objects)
		die(_("cannot delete packs in a precious-objects repo"));

	die_for_incompatible_opt3(unpack_unreachable || (pack_everything & LOOSEN_UNREACHABLE), "-A",
				  keep_unreachable, "-k/--keep-unreachable",
				  pack_everything & PACK_CRUFT, "--cruft");

	إذا (pack_everything & PACK_CRUFT)
		pack_everything |= ALL_INTO_ONE;

	إذا (write_bitmaps < 0) {
		إذا (write_midx == REPACK_WRITE_MIDX_NONE &&
		    (!(pack_everything & ALL_INTO_ONE) || !is_bare_repository(repo)))
			write_bitmaps = 0;
	}
	إذا (po_آرگومانها.pack_kept_objects < 0)
		po_آرگومانها.pack_kept_objects = write_bitmaps > 0 &&
			write_midx == REPACK_WRITE_MIDX_NONE;

	إذا (write_bitmaps && !(pack_everything & ALL_INTO_ONE) &&
	    write_midx == REPACK_WRITE_MIDX_NONE)
		die(_(incremental_bitmap_conflict_error));

	إذا (write_bitmaps && po_آرگومانها.local &&
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

	إذا (config_ctx.midx_تقسیم_ضریب < 2)
		die(_("invalid value for %s: %d"), "--midx-split-factor",
		    config_ctx.midx_تقسیم_ضریب);
	إذا (config_ctx.midx_new_layer_آستانه < 1)
		die(_("invalid value for %s: %d"), "--midx-new-layer-threshold",
		    config_ctx.midx_new_layer_آستانه);

	إذا (write_midx != REPACK_WRITE_MIDX_NONE && write_bitmaps) {
		بنية strbuf مسیر = STRBUF_INIT;

		strbuf_addf(&مسیر, "%s/%s_XXXXXX",
			    repo_get_object_directory(repo),
			    "bitmap-ref-tips");

		refs_snapshot = xmks_tempfile(مسیر.buf);
		midx_snapshot_refs(repo, refs_snapshot);

		strbuf_release(&مسیر);
	}

	packdir = mkpathdup("%s/pack", repo_get_object_directory(repo));
	packtmp_name = xstrfmt(".tmp-%d-pack", (صحيح)getpid());
	packtmp = mkpathdup("%s/%s", packdir, packtmp_name);

	existing.repo = repo;
	existing_packs_collect(&existing, &keep_pack_list);

	إذا (geometry.تقسیم_ضریب) {
		إذا (pack_everything)
			die(_("options '%s' and '%s' cannot be used together"), "--geometric", "-A/-a");
		إذا (write_midx == REPACK_WRITE_MIDX_INCREMENTAL) {
			geometry.midx_layer_آستانه = config_ctx.midx_new_layer_آستانه;
			geometry.midx_layer_آستانه_مجموعه = true;
		}
		pack_geometry_init(&geometry, &existing, &po_آرگومانها);
		pack_geometry_تقسیم(&geometry);
	}

	prepare_pack_objects(&cmd, &po_آرگومانها, packtmp);

	show_پیشرفت = !po_آرگومانها.quiet && isatty(2);

	strvec_افزودن(&cmd.آرگومانها, "--keep-true-parents");
	لكل (i = 0; i < keep_pack_list.nr; i++)
		strvec_pushf(&cmd.آرگومانها, "--keep-pack=%s",
			     keep_pack_list.اقلام[i].string);
	strvec_افزودن(&cmd.آرگومانها, "--non-empty");
	إذا (!geometry.تقسیم_ضریب) {
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
		strvec_افزودن(&cmd.آرگومانها, "--all");
		strvec_افزودن(&cmd.آرگومانها, "--reflog");
		strvec_افزودن(&cmd.آرگومانها, "--indexed-objects");
	}
	إذا (repo_has_promisor_remote(repo))
		strvec_افزودن(&cmd.آرگومانها, "--exclude-promisor-objects");
	إذا (write_midx == REPACK_WRITE_MIDX_NONE) {
		إذا (write_bitmaps > 0)
			strvec_افزودن(&cmd.آرگومانها, "--write-bitmap-index");
		وإلا إذا (write_bitmaps < 0)
			strvec_افزودن(&cmd.آرگومانها, "--write-bitmap-index-quiet");
	}
	إذا (use_delta_islands)
		strvec_افزودن(&cmd.آرگومانها, "--delta-islands");

	إذا (pack_everything & ALL_INTO_ONE) {
		repack_promisor_objects(repo, &po_آرگومانها, &نامها, packtmp,
			(drop_filtered && !dry_پیاپی) ? &drop_oids : عدم);

		إذا (existing_packs_has_non_kept(&existing) &&
		    delete_redundant &&
		    !(pack_everything & PACK_CRUFT)) {
			for_each_string_list_قلم(قلم, &نامها) {
				strvec_pushf(&cmd.آرگومانها, "--keep-pack=%s-%s.pack",
					     packtmp_name, قلم->string);
			}
			إذا (unpack_unreachable) {
				strvec_pushf(&cmd.آرگومانها,
					     "--unpack-unreachable=%s",
					     unpack_unreachable);
			} وإلا إذا (pack_everything & LOOSEN_UNREACHABLE) {
				strvec_افزودن(&cmd.آرگومانها,
					    "--unpack-unreachable");
			} وإلا إذا (keep_unreachable) {
				strvec_افزودن(&cmd.آرگومانها, "--keep-unreachable");
			}
		}

		إذا (keep_unreachable && delete_redundant &&
		    !(pack_everything & PACK_CRUFT))
			strvec_افزودن(&cmd.آرگومانها, "--pack-loose-unreachable");
	} وإلا إذا (geometry.تقسیم_ضریب) {
		pack_geometry_repack_promisors(repo, &po_آرگومانها, &geometry,
					       &نامها, packtmp);

		إذا (midx_must_contain_cruft)
			strvec_افزودن(&cmd.آرگومانها, "--stdin-packs");
		وإلا
			strvec_افزودن(&cmd.آرگومانها, "--stdin-packs=follow");
		strvec_افزودن(&cmd.آرگومانها, "--unpacked");
	} وإلا {
		strvec_افزودن(&cmd.آرگومانها, "--unpacked");
		strvec_افزودن(&cmd.آرگومانها, "--incremental");
	}

	إذا (po_آرگومانها.filter_options.choice)
		strvec_pushf(&cmd.آرگومانها, "--filter=%s",
			     expand_list_objects_filter_spec(&po_آرگومانها.filter_options));
	وإلا إذا (filter_مقصد)
		die(_("option '%s' can only be used along with '%s'"), "--filter-to", "--filter");

	إذا (geometry.تقسیم_ضریب)
		cmd.درون = -1;
	وإلا
		cmd.no_stdin = 1;

	ret = شروع_command(&cmd);
	إذا (ret)
		اذهب cleanup;

	إذا (geometry.تقسیم_ضریب) {
		FILE *درون = xfdopen(cmd.درون, "w");
		/*
		 * The resulting pack should contain all objects in packs that
		 * are going to be rolled up, but exclude objects in packs which
		 * are being left alone.
		 */
		لكل (i = 0; i < geometry.تقسیم; i++)
			fprintf(درون, "%s\n", pack_basename(geometry.pack[i]));
		لكل (i = geometry.تقسیم; i < geometry.pack_nr; i++) {
			ثابت حرف *basename = pack_basename(geometry.pack[i]);
			حرف marker = '^';

			إذا (!midx_must_contain_cruft &&
			    !string_list_has_string(&existing.midx_packs,
						    basename)) {
				/*
				 * Assume non-MIDX'd packs are not
				 * necessarily closed under
				 * reachability.
				 */
				marker = '!';
			}

			fprintf(درون, "%c%s\n", marker, basename);
		}
		fclose(درون);
	}

	{
		بنية write_pack_opts opts = {
			.packdir = packdir,
			.destination = packdir,
			.packtmp = packtmp,
		};
		ret = پایان_pack_objects_cmd(repo->hash_algo, &opts, &cmd,
					      &نامها);
		إذا (ret)
			اذهب cleanup;
	}

	إذا (!نامها.nr) {
		بنية odb_منبع_files *files = odb_منبع_files_downcast(existing.منبع);

		إذا (!po_آرگومانها.quiet)
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
		إذا (!get_multi_pack_شاخص(files->dirs->packed))
			midx_must_contain_cruft = 1;
	}

	إذا (pack_everything & PACK_CRUFT) {
		بنية write_pack_opts opts = {
			.po_آرگومانها = &cruft_po_آرگومانها,
			.destination = packtmp,
			.packtmp = packtmp,
			.packdir = packdir,
		};

		إذا (!cruft_po_آرگومانها.پنجره)
			cruft_po_آرگومانها.پنجره = xstrdup_or_null(po_آرگومانها.پنجره);
		إذا (!cruft_po_آرگومانها.پنجره_حافظه)
			cruft_po_آرگومانها.پنجره_حافظه = xstrdup_or_null(po_آرگومانها.پنجره_حافظه);
		إذا (!cruft_po_آرگومانها.عمق)
			cruft_po_آرگومانها.عمق = xstrdup_or_null(po_آرگومانها.عمق);
		إذا (!cruft_po_آرگومانها.نخها)
			cruft_po_آرگومانها.نخها = xstrdup_or_null(po_آرگومانها.نخها);
		إذا (!cruft_po_آرگومانها.max_pack_اندازه)
			cruft_po_آرگومانها.max_pack_اندازه = po_آرگومانها.max_pack_اندازه;

		cruft_po_آرگومانها.local = po_آرگومانها.local;
		cruft_po_آرگومانها.quiet = po_آرگومانها.quiet;
		cruft_po_آرگومانها.delta_پایه_جابه_جایی = po_آرگومانها.delta_پایه_جابه_جایی;
		cruft_po_آرگومانها.pack_kept_objects = 0;

		ret = write_cruft_pack(&opts, cruft_expiration,
				       combine_cruft_زیر_اندازه, &نامها,
				       &existing);
		إذا (ret)
			اذهب cleanup;

		إذا (delete_redundant && expire_مقصد) {
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
			ret = write_cruft_pack(&opts, عدم, 0ul, &نامها,
					       &existing);
			إذا (ret)
				اذهب cleanup;
		}
	}

	إذا (po_آرگومانها.filter_options.choice && !drop_filtered) {
		بنية write_pack_opts opts = {
			.po_آرگومانها = &po_آرگومانها,
			.destination = filter_مقصد,
			.packdir = packdir,
			.packtmp = packtmp,
		};

		إذا (!opts.destination)
			opts.destination = packtmp;

		ret = write_filtered_pack(&opts, &existing, &نامها);
		إذا (ret)
			اذهب cleanup;
	}

	string_list_مرتب_سازی(&نامها);

	odb_close(repo->objects);

	/*
	 * Ok we have prepared all new packfiles.
	 */
	for_each_string_list_قلم(قلم, &نامها)
		generated_pack_install(قلم->util, قلم->string, packdir,
				       packtmp);
	/* End of pack replacement. */

	إذا (delete_redundant && pack_everything & ALL_INTO_ONE) {
		إذا (write_midx == REPACK_WRITE_MIDX_INCREMENTAL)
			existing_packs_retain_midx_packs(&existing);
		existing_packs_mark_for_deletion(&existing, &نامها);
	}

	إذا (write_midx != REPACK_WRITE_MIDX_NONE) {
		بنية repack_write_midx_opts opts = {
			.existing = &existing,
			.geometry = &geometry,
			.نامها = &نامها,
			.refs_snapshot = refs_snapshot ? get_tempfile_مسیر(refs_snapshot) : عدم,
			.packdir = packdir,
			.show_پیشرفت = show_پیشرفت,
			.write_bitmaps = write_bitmaps > 0,
			.midx_must_contain_cruft = midx_must_contain_cruft,
			.midx_تقسیم_ضریب = config_ctx.midx_تقسیم_ضریب,
			.midx_new_layer_آستانه = config_ctx.midx_new_layer_آستانه,
			.نما = write_midx,
		};

		ret = repack_write_midx(&opts);
		إذا (ret)
			اذهب cleanup;
	}

	odb_reprepare(repo->objects);

	إذا (delete_redundant) {
		صحيح opts = 0;
		bool wrote_incremental_midx = write_midx == REPACK_WRITE_MIDX_INCREMENTAL;

		existing_packs_remove_redundant(&existing, packdir,
						wrote_incremental_midx);

		إذا (geometry.تقسیم_ضریب)
			pack_geometry_remove_redundant(&geometry, &نامها,
						       &existing, packdir,
						       wrote_incremental_midx);
		إذا (show_پیشرفت)
			opts |= PRUNE_PACKED_VERBOSE;
		prune_packed_objects(opts);

		إذا (!keep_unreachable &&
		    (!(pack_everything & LOOSEN_UNREACHABLE) ||
		     unpack_unreachable) &&
		    is_repository_shallow(repo))
			prune_shallow(PRUNE_سریع);
	}

	إذا (پیاپی_update_server_info)
		update_server_info(repo, 0);

	إذا (git_env_bool(GIT_TEST_MULTI_PACK_شاخص, 0)) {
		بنية odb_منبع_files *files = odb_منبع_files_downcast(existing.منبع);
		بیعلامت flags = 0;

		إذا (git_env_bool(GIT_TEST_MULTI_PACK_شاخص_WRITE_INCREMENTAL, 0))
			flags |= MIDX_WRITE_INCREMENTAL;
		write_midx_file(files->dirs->packed, عدم, عدم, flags);
	}

cleanup:
	string_list_clear(&keep_pack_list, 0);
	string_list_clear(&نامها, 1);
	oidset_clear(&drop_oids);
	existing_packs_release(&existing);
	pack_geometry_release(&geometry);
	pack_objects_آرگومانها_release(&po_آرگومانها);
	pack_objects_آرگومانها_release(&cruft_po_آرگومانها);

	أرجع ret;
}
