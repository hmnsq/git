#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "server-info.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "midx.h"
#అన్తర్భావ "packfile.h"
#అన్తర్భావ "prune-packed.h"
#అన్తర్భావ "promisor-remote.h"
#అన్తర్భావ "repack.h"
#అన్తర్భావ "shallow.h"
#అన్తర్భావ "list-objects-filter-options.h"
#అన్తర్భావ "oidset.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "wt-status.h"
#అన్తర్భావ "read-cache-ll.h"

#నిర్వచించు ALL_INTO_ONE 1
#నిర్వచించు LOOSEN_UNREACHABLE 2
#నిర్వచించు PACK_CRUFT 4

#నిర్వచించు DELETE_PACK 1
#నిర్వచించు RETAIN_PACK 2

స్థిర పూర్ణాంకం pack_everything;
స్థిర పూర్ణాంకం write_bitmaps = -1;
స్థిర పూర్ణాంకం use_delta_islands;
స్థిర పూర్ణాంకం నిరంతరం_update_server_info = 1;
స్థిర అక్షరం *packdir, *packtmp_name, *packtmp;
స్థిర పూర్ణాంకం midx_must_contain_cruft = 1;
స్థిర పూర్ణాంకం drop_filtered;
స్థిర పూర్ణాంకం dry_నిరంతరం;
స్థిర పూర్ణాంకం write_bitmaps_given;

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి git_repack_usage[] = {
	N_("git repack [-a] [-A] [-d] [-f] [-F] [-l] [-n] [-q] [-b] [-m]\n"
	   "[--window=<n>] [--depth=<n>] [--threads=<n>] [--keep-pack=<pack-name>]\n"
	   "[--write-midx[=<mode>]] [--name-hash-version=<n>] [--path-walk]\n"
	   "[--filter=<filter-spec>] [--drop-filtered [--dry-run]]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం incremental_bitmap_conflict_error[] = N_(
"Incremental repacks are incompatible with bitmap indexes.  Use\n"
"--no-write-bitmap-index or disable the pack.writeBitmaps configuration."
);

#నిర్వచించు DEFAULT_MIDX_విభజించు_గుణకం 2
#నిర్వచించు DEFAULT_MIDX_NEW_LAYER_పరిమితి 8

నిర్మాణం repack_config_ctx {
	నిర్మాణం pack_objects_వాదనలు *po_వాదనలు;
	నిర్మాణం pack_objects_వాదనలు *cruft_po_వాదనలు;
	పూర్ణాంకం midx_విభజించు_గుణకం;
	పూర్ణాంకం midx_new_layer_పరిమితి;
};

స్థిర పూర్ణాంకం repack_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			 స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	నిర్మాణం repack_config_ctx *repack_ctx = cb;
	నిర్మాణం pack_objects_వాదనలు *po_వాదనలు = repack_ctx->po_వాదనలు;
	నిర్మాణం pack_objects_వాదనలు *cruft_po_వాదనలు = repack_ctx->cruft_po_వాదనలు;
	ఒకవేళ (!strcmp(var, "repack.usedeltabaseoffset")) {
		po_వాదనలు->delta_పునాది_స్థానభ్రంశం = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "repack.packkeptobjects")) {
		po_వాదనలు->pack_kept_objects = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "repack.writebitmaps") ||
	    !strcmp(var, "pack.writebitmaps")) {
		write_bitmaps = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "repack.usedeltaislands")) {
		use_delta_islands = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (strcmp(var, "repack.updateserverinfo") == 0) {
		నిరంతరం_update_server_info = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "repack.cruftwindow")) {
		విడుదల(cruft_po_వాదనలు->కిటికీ);
		తిరిగి git_config_string(&cruft_po_వాదనలు->కిటికీ, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "repack.cruftwindowmemory")) {
		విడుదల(cruft_po_వాదనలు->కిటికీ_జ్ఞాపకం);
		తిరిగి git_config_string(&cruft_po_వాదనలు->కిటికీ_జ్ఞాపకం, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "repack.cruftdepth")) {
		విడుదల(cruft_po_వాదనలు->లోతు);
		తిరిగి git_config_string(&cruft_po_వాదనలు->లోతు, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "repack.cruftthreads")) {
		విడుదల(cruft_po_వాదనలు->దారాలు);
		తిరిగి git_config_string(&cruft_po_వాదనలు->దారాలు, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "repack.midxmustcontaincruft")) {
		midx_must_contain_cruft = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "repack.midxsplitfactor")) {
		repack_ctx->midx_విభజించు_గుణకం = git_config_int(var, విలువ,
							       ctx->kvi);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "repack.midxnewlayerthreshold")) {
		repack_ctx->midx_new_layer_పరిమితి = git_config_int(var, విలువ,
								      ctx->kvi);
		తిరిగి 0;
	}
	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర పూర్ణాంకం option_parse_write_bitmaps(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				      పూర్ణాంకం unset)
{
	పూర్ణాంకం *విలువ = opt->విలువ;

	BUG_ON_OPT_ARG(arg);
	ఒకవేళ (unset)
		*విలువ = 0;
	లేకపోతే
		*విలువ = 1;

	write_bitmaps_given = 1;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_write_midx(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				   పూర్ణాంకం unset)
{
	గణన repack_write_midx_బహుళకం *cfg = opt->విలువ;

	ఒకవేళ (unset) {
		*cfg = REPACK_WRITE_MIDX_NONE;
		తిరిగి 0;
	}

	ఒకవేళ (!arg || !*arg)
		*cfg = REPACK_WRITE_MIDX_DEFAULT;
	లేకపోతే ఒకవేళ (!strcmp(arg, "incremental"))
		*cfg = REPACK_WRITE_MIDX_INCREMENTAL;
	లేకపోతే
		తిరిగి error(_("unknown value for %s: %s"), opt->long_name, arg);

	తిరిగి 0;
}

పూర్ణాంకం cmd_repack(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం string_list పేర్లు = STRING_LIST_INIT_DUP;
	నిర్మాణం existing_packs existing = EXISTING_PACKS_INIT;
	నిర్మాణం oidset drop_oids = OIDSET_INIT;
	నిర్మాణం pack_geometry geometry = { 0 };
	నిర్మాణం tempfile *refs_snapshot = ఖాళీ;
	పూర్ణాంకం i, ret;
	పూర్ణాంకం show_పురోగతి;

	/* variables to be filled by option parsing */
	నిర్మాణం repack_config_ctx config_ctx;
	పూర్ణాంకం delete_redundant = 0;
	స్థిరరాశి అక్షరం *unpack_unreachable = ఖాళీ;
	పూర్ణాంకం keep_unreachable = 0;
	నిర్మాణం string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	నిర్మాణం pack_objects_వాదనలు po_వాదనలు = PACK_OBJECTS_వాదనలు_INIT;
	నిర్మాణం pack_objects_వాదనలు cruft_po_వాదనలు = PACK_OBJECTS_వాదనలు_INIT;
	గణన repack_write_midx_బహుళకం write_midx = REPACK_WRITE_MIDX_NONE;
	స్థిరరాశి అక్షరం *cruft_expiration = ఖాళీ;
	స్థిరరాశి అక్షరం *expire_గమ్యం = ఖాళీ;
	స్థిరరాశి అక్షరం *filter_గమ్యం = ఖాళీ;
	స్థిరరాశి అక్షరం *opt_కిటికీ = ఖాళీ;
	స్థిరరాశి అక్షరం *opt_కిటికీ_జ్ఞాపకం = ఖాళీ;
	స్థిరరాశి అక్షరం *opt_లోతు = ఖాళీ;
	స్థిరరాశి అక్షరం *opt_దారాలు = ఖాళీ;
	గుర్తులేని దీర్ఘ combine_cruft_క్రింద_కొలత = 0ul;

	నిర్మాణం option builtin_repack_options[] = {
		OPT_బిట్టు('a', ఖాళీ, &pack_everything,
				N_("pack everything in a single pack"), ALL_INTO_ONE),
		OPT_బిట్టు('A', ఖాళీ, &pack_everything,
				N_("same as -a, and turn unreachable objects loose"),
				   LOOSEN_UNREACHABLE | ALL_INTO_ONE),
		OPT_బిట్టు(0, "cruft", &pack_everything,
				N_("same as -a, pack unreachable cruft objects separately"),
				   PACK_CRUFT),
		OPT_STRING(0, "cruft-expiration", &cruft_expiration, N_("approxidate"),
				N_("with --cruft, expire objects older than this")),
		OPT_UNSIGNED(0, "combine-cruft-below-size",
			     &combine_cruft_క్రింద_కొలత,
			     N_("with --cruft, only repack cruft packs smaller than this")),
		OPT_UNSIGNED(0, "max-cruft-size", &cruft_po_వాదనలు.max_pack_కొలత,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL('d', ఖాళీ, &delete_redundant,
				N_("remove redundant packs, and run git-prune-packed")),
		OPT_BOOL('f', ఖాళీ, &po_వాదనలు.no_reuse_delta,
				N_("pass --no-reuse-delta to git-pack-objects")),
		OPT_BOOL('F', ఖాళీ, &po_వాదనలు.no_reuse_object,
				N_("pass --no-reuse-object to git-pack-objects")),
		OPT_INTEGER(0, "name-hash-version", &po_వాదనలు.name_hash_సంస్కరణ,
				N_("specify the name hash version to use for grouping similar objects by path")),
		OPT_BOOL(0, "path-walk", &po_వాదనలు.మార్గం_walk,
				N_("pass --path-walk to git-pack-objects")),
		OPT_NEGBIT('n', ఖాళీ, &నిరంతరం_update_server_info,
				N_("do not run git-update-server-info"), 1),
		OPT__QUIET(&po_వాదనలు.quiet, N_("be quiet")),
		OPT_BOOL('l', "local", &po_వాదనలు.local,
				N_("pass --local to git-pack-objects")),
		OPT_CALLBACK_F('b', "write-bitmap-index", &write_bitmaps, ఖాళీ,
				N_("write bitmap index"),
				PARSE_OPT_NOARG, option_parse_write_bitmaps),
		OPT_BOOL('i', "delta-islands", &use_delta_islands,
				N_("pass --delta-islands to git-pack-objects")),
		OPT_STRING(0, "unpack-unreachable", &unpack_unreachable, N_("approxidate"),
				N_("with -A, do not loosen objects older than this")),
		OPT_BOOL('k', "keep-unreachable", &keep_unreachable,
				N_("with -a, repack unreachable objects")),
		OPT_STRING(0, "window", &opt_కిటికీ, N_("n"),
				N_("size of the window used for delta compression")),
		OPT_STRING(0, "window-memory", &opt_కిటికీ_జ్ఞాపకం, N_("bytes"),
				N_("same as the above, but limit memory size instead of entries count")),
		OPT_STRING(0, "depth", &opt_లోతు, N_("n"),
				N_("limits the maximum delta depth")),
		OPT_STRING(0, "threads", &opt_దారాలు, N_("n"),
				N_("limits the maximum number of threads")),
		OPT_UNSIGNED(0, "max-pack-size", &po_వాదనలు.max_pack_కొలత,
			     N_("maximum size of each packfile")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&po_వాదనలు.filter_options),
		OPT_BOOL(0, "pack-kept-objects", &po_వాదనలు.pack_kept_objects,
				N_("repack objects in packs marked with .keep")),
		OPT_STRING_LIST(0, "keep-pack", &keep_pack_list, N_("name"),
				N_("do not repack this pack")),
		OPT_INTEGER('g', "geometric", &geometry.విభజించు_గుణకం,
			    N_("find a geometric progression with factor <N>")),
		OPT_CALLBACK_F(0, "write-midx", &write_midx,
			   N_("mode"),
			   N_("write a multi-pack index of the resulting packs"),
			   PARSE_OPT_OPTARG, option_parse_write_midx),
		OPT_సమితి_INT_F('m', ఖాళీ, &write_midx,
			   N_("write a multi-pack index of the resulting packs"),
			   REPACK_WRITE_MIDX_DEFAULT,
			   PARSE_OPT_HIDDEN),
		OPT_STRING(0, "expire-to", &expire_గమ్యం, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_STRING(0, "filter-to", &filter_గమ్యం, N_("dir"),
			   N_("pack prefix to store a pack containing filtered out objects")),
		OPT_BOOL(0, "drop-filtered", &drop_filtered,
				N_("delete filtered out objects (requires --filter)")),
		OPT_BOOL(0, "dry-run", &dry_నిరంతరం,
				N_("only show which objects would be dropped")),
		OPT_చివర()
	};

	list_objects_filter_init(&po_వాదనలు.filter_options);

	memset(&config_ctx, 0, పరిమాణం(config_ctx));
	config_ctx.po_వాదనలు = &po_వాదనలు;
	config_ctx.cruft_po_వాదనలు = &cruft_po_వాదనలు;
	config_ctx.midx_విభజించు_గుణకం = DEFAULT_MIDX_విభజించు_గుణకం;
	config_ctx.midx_new_layer_పరిమితి = DEFAULT_MIDX_NEW_LAYER_పరిమితి;

	repo_config(repo, repack_config, &config_ctx);

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_repack_options,
				git_repack_usage, 0);

	po_వాదనలు.కిటికీ = xstrdup_or_null(opt_కిటికీ);
	po_వాదనలు.కిటికీ_జ్ఞాపకం = xstrdup_or_null(opt_కిటికీ_జ్ఞాపకం);
	po_వాదనలు.లోతు = xstrdup_or_null(opt_లోతు);
	po_వాదనలు.దారాలు = xstrdup_or_null(opt_దారాలు);

	die_for_incompatible_opt2(drop_filtered, "--drop-filtered",
		!!filter_గమ్యం, "--filter-to");

	ఒకవేళ (dry_నిరంతరం && !drop_filtered)
		die(_("--dry-run only takes effect with --drop-filtered"));

	ఒకవేళ (drop_filtered) {
		ఒకవేళ (!po_వాదనలు.filter_options.choice)
			die(_("--drop-filtered requires --filter"));

		ఒకవేళ (!(pack_everything & ALL_INTO_ONE))
			die(_("--drop-filtered requires -a"));

		/*
		 * Only blob:limit=<n> is supported for now. Reject other
		 * filter choices early, before walking the object database.
		 */
		ఒకవేళ (po_వాదనలు.filter_options.choice != LOFC_BLOB_హద్దు)
			die(_("--drop-filtered only supports --filter=blob:limit=<n> for now"));

		/*
		 * An explicit -b on the command line is a conflict we have to
		 * report; a bitmap setting from config is silently overridden
		 * for the duration of the command.
		 */
		ఒకవేళ (write_bitmaps_given && write_bitmaps > 0)
			die(_("options '%s' and '%s' cannot be used together"),
				"--drop-filtered", "--write-bitmap-index");

		/*
		 * Without a promisor remote there is nowhere to re-fetch the
		 * dropped objects from, so dropping them would be permanent
		 * data loss.
		 */
		ఒకవేళ (!repo_has_promisor_remote(repo))
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
		ఒకవేళ (!is_bare_repository(repo)) {
			నిర్మాణం wt_స్థితి_state state = { 0 };

			wt_స్థితి_get_state(repo, &state, 0);
			ఒకవేళ (state.విలీనం_లోపల_పురోగతి || state.revert_లోపల_పురోగతి ||
			    state.rebase_లోపల_పురోగతి || state.bisect_లోపల_పురోగతి ||
			    state.cherry_pick_లోపల_పురోగతి || state.am_లోపల_పురోగతి ||
			    state.rebase_interactive_లోపల_పురోగతి) {
				wt_స్థితి_state_free_buffers(&state);
				die(_("--drop-filtered cannot be used while "
				      "another operation (merge, rebase, am, "
				      "cherry-pick, revert, or bisect) is in "
				      "progress"));
			}
			wt_స్థితి_state_free_buffers(&state);
		}

		write_bitmaps = 0;

		/*
		 * Dropping objects means rebuilding the promisor packs
		 * without them and then removing the old packs, so the
		 * redundant packs must be deleted. Imply -d on a real run.
		 */
		ఒకవేళ (!dry_నిరంతరం)
			delete_redundant = 1;

		ret = enumerate_promisor_blobs(repo, &po_వాదనలు.filter_options, &drop_oids);

		ఒకవేళ (ret)
			వెళ్ళు cleanup;

		/*
		 * Refuse to drop blobs that the current index references.
		 * Such a blob would only be lazily re-fetched by the next
		 * command that touches the worktree, so dropping it reclaims
		 * nothing. This guard just avoids that churn. Bare
		 * repositories have no index, so the check is skipped there.
		 */
		ఒకవేళ (!is_bare_repository(repo) && oidset_కొలత(&drop_oids)) {
			నిర్మాణం సూచీ_state *istate = repo->సూచీ;
			గుర్తులేని పూర్ణాంకం i;

			ఒకవేళ (repo_read_సూచీ(repo) < 0)
				die(_("could not read the index"));

			కొరకు (i = 0; i < istate->cache_nr; i++) {
				స్థిరరాశి నిర్మాణం cache_నమోదు *ce = istate->cache[i];

				ఒకవేళ (oidset_కలిగియుంది(&drop_oids, &ce->oid))
					die(_("cannot drop '%s' (%s): it is referenced by the current index"),
						ce->name, oid_గమ్యం_hex(&ce->oid));
			}
		}

		ఒకవేళ (dry_నిరంతరం) {
			నిర్మాణం oidset_iter iter;
			స్థిరరాశి నిర్మాణం object_id *oid;

			oidset_iter_init(&drop_oids, &iter);
			అయితే ((oid = oidset_iter_తదుపరి(&iter)))
				రాయి("%s\n", oid_గమ్యం_hex(oid));
		}
	}

	ఒకవేళ (delete_redundant && repo->repository_format_precious_objects)
		die(_("cannot delete packs in a precious-objects repo"));

	die_for_incompatible_opt3(unpack_unreachable || (pack_everything & LOOSEN_UNREACHABLE), "-A",
				  keep_unreachable, "-k/--keep-unreachable",
				  pack_everything & PACK_CRUFT, "--cruft");

	ఒకవేళ (pack_everything & PACK_CRUFT)
		pack_everything |= ALL_INTO_ONE;

	ఒకవేళ (write_bitmaps < 0) {
		ఒకవేళ (write_midx == REPACK_WRITE_MIDX_NONE &&
		    (!(pack_everything & ALL_INTO_ONE) || !is_bare_repository(repo)))
			write_bitmaps = 0;
	}
	ఒకవేళ (po_వాదనలు.pack_kept_objects < 0)
		po_వాదనలు.pack_kept_objects = write_bitmaps > 0 &&
			write_midx == REPACK_WRITE_MIDX_NONE;

	ఒకవేళ (write_bitmaps && !(pack_everything & ALL_INTO_ONE) &&
	    write_midx == REPACK_WRITE_MIDX_NONE)
		die(_(incremental_bitmap_conflict_error));

	ఒకవేళ (write_bitmaps && po_వాదనలు.local &&
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

	ఒకవేళ (config_ctx.midx_విభజించు_గుణకం < 2)
		die(_("invalid value for %s: %d"), "--midx-split-factor",
		    config_ctx.midx_విభజించు_గుణకం);
	ఒకవేళ (config_ctx.midx_new_layer_పరిమితి < 1)
		die(_("invalid value for %s: %d"), "--midx-new-layer-threshold",
		    config_ctx.midx_new_layer_పరిమితి);

	ఒకవేళ (write_midx != REPACK_WRITE_MIDX_NONE && write_bitmaps) {
		నిర్మాణం strbuf మార్గం = STRBUF_INIT;

		strbuf_addf(&మార్గం, "%s/%s_XXXXXX",
			    repo_get_object_directory(repo),
			    "bitmap-ref-tips");

		refs_snapshot = xmks_tempfile(మార్గం.buf);
		midx_snapshot_refs(repo, refs_snapshot);

		strbuf_release(&మార్గం);
	}

	packdir = mkpathdup("%s/pack", repo_get_object_directory(repo));
	packtmp_name = xstrfmt(".tmp-%d-pack", (పూర్ణాంకం)getpid());
	packtmp = mkpathdup("%s/%s", packdir, packtmp_name);

	existing.repo = repo;
	existing_packs_collect(&existing, &keep_pack_list);

	ఒకవేళ (geometry.విభజించు_గుణకం) {
		ఒకవేళ (pack_everything)
			die(_("options '%s' and '%s' cannot be used together"), "--geometric", "-A/-a");
		ఒకవేళ (write_midx == REPACK_WRITE_MIDX_INCREMENTAL) {
			geometry.midx_layer_పరిమితి = config_ctx.midx_new_layer_పరిమితి;
			geometry.midx_layer_పరిమితి_సమితి = true;
		}
		pack_geometry_init(&geometry, &existing, &po_వాదనలు);
		pack_geometry_విభజించు(&geometry);
	}

	prepare_pack_objects(&cmd, &po_వాదనలు, packtmp);

	show_పురోగతి = !po_వాదనలు.quiet && isatty(2);

	strvec_నెట్టు(&cmd.వాదనలు, "--keep-true-parents");
	కొరకు (i = 0; i < keep_pack_list.nr; i++)
		strvec_pushf(&cmd.వాదనలు, "--keep-pack=%s",
			     keep_pack_list.వస్తువులు[i].string);
	strvec_నెట్టు(&cmd.వాదనలు, "--non-empty");
	ఒకవేళ (!geometry.విభజించు_గుణకం) {
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
		strvec_నెట్టు(&cmd.వాదనలు, "--all");
		strvec_నెట్టు(&cmd.వాదనలు, "--reflog");
		strvec_నెట్టు(&cmd.వాదనలు, "--indexed-objects");
	}
	ఒకవేళ (repo_has_promisor_remote(repo))
		strvec_నెట్టు(&cmd.వాదనలు, "--exclude-promisor-objects");
	ఒకవేళ (write_midx == REPACK_WRITE_MIDX_NONE) {
		ఒకవేళ (write_bitmaps > 0)
			strvec_నెట్టు(&cmd.వాదనలు, "--write-bitmap-index");
		లేకపోతే ఒకవేళ (write_bitmaps < 0)
			strvec_నెట్టు(&cmd.వాదనలు, "--write-bitmap-index-quiet");
	}
	ఒకవేళ (use_delta_islands)
		strvec_నెట్టు(&cmd.వాదనలు, "--delta-islands");

	ఒకవేళ (pack_everything & ALL_INTO_ONE) {
		repack_promisor_objects(repo, &po_వాదనలు, &పేర్లు, packtmp,
			(drop_filtered && !dry_నిరంతరం) ? &drop_oids : ఖాళీ);

		ఒకవేళ (existing_packs_has_non_kept(&existing) &&
		    delete_redundant &&
		    !(pack_everything & PACK_CRUFT)) {
			for_each_string_list_అంశం(అంశం, &పేర్లు) {
				strvec_pushf(&cmd.వాదనలు, "--keep-pack=%s-%s.pack",
					     packtmp_name, అంశం->string);
			}
			ఒకవేళ (unpack_unreachable) {
				strvec_pushf(&cmd.వాదనలు,
					     "--unpack-unreachable=%s",
					     unpack_unreachable);
			} లేకపోతే ఒకవేళ (pack_everything & LOOSEN_UNREACHABLE) {
				strvec_నెట్టు(&cmd.వాదనలు,
					    "--unpack-unreachable");
			} లేకపోతే ఒకవేళ (keep_unreachable) {
				strvec_నెట్టు(&cmd.వాదనలు, "--keep-unreachable");
			}
		}

		ఒకవేళ (keep_unreachable && delete_redundant &&
		    !(pack_everything & PACK_CRUFT))
			strvec_నెట్టు(&cmd.వాదనలు, "--pack-loose-unreachable");
	} లేకపోతే ఒకవేళ (geometry.విభజించు_గుణకం) {
		pack_geometry_repack_promisors(repo, &po_వాదనలు, &geometry,
					       &పేర్లు, packtmp);

		ఒకవేళ (midx_must_contain_cruft)
			strvec_నెట్టు(&cmd.వాదనలు, "--stdin-packs");
		లేకపోతే
			strvec_నెట్టు(&cmd.వాదనలు, "--stdin-packs=follow");
		strvec_నెట్టు(&cmd.వాదనలు, "--unpacked");
	} లేకపోతే {
		strvec_నెట్టు(&cmd.వాదనలు, "--unpacked");
		strvec_నెట్టు(&cmd.వాదనలు, "--incremental");
	}

	ఒకవేళ (po_వాదనలు.filter_options.choice)
		strvec_pushf(&cmd.వాదనలు, "--filter=%s",
			     expand_list_objects_filter_spec(&po_వాదనలు.filter_options));
	లేకపోతే ఒకవేళ (filter_గమ్యం)
		die(_("option '%s' can only be used along with '%s'"), "--filter-to", "--filter");

	ఒకవేళ (geometry.విభజించు_గుణకం)
		cmd.లోపల = -1;
	లేకపోతే
		cmd.no_stdin = 1;

	ret = ప్రారంభం_command(&cmd);
	ఒకవేళ (ret)
		వెళ్ళు cleanup;

	ఒకవేళ (geometry.విభజించు_గుణకం) {
		FILE *లోపల = xfdopen(cmd.లోపల, "w");
		/*
		 * The resulting pack should contain all objects in packs that
		 * are going to be rolled up, but exclude objects in packs which
		 * are being left alone.
		 */
		కొరకు (i = 0; i < geometry.విభజించు; i++)
			fprintf(లోపల, "%s\n", pack_basename(geometry.pack[i]));
		కొరకు (i = geometry.విభజించు; i < geometry.pack_nr; i++) {
			స్థిరరాశి అక్షరం *basename = pack_basename(geometry.pack[i]);
			అక్షరం marker = '^';

			ఒకవేళ (!midx_must_contain_cruft &&
			    !string_list_has_string(&existing.midx_packs,
						    basename)) {
				/*
				 * Assume non-MIDX'd packs are not
				 * necessarily closed under
				 * reachability.
				 */
				marker = '!';
			}

			fprintf(లోపల, "%c%s\n", marker, basename);
		}
		fclose(లోపల);
	}

	{
		నిర్మాణం write_pack_opts opts = {
			.packdir = packdir,
			.destination = packdir,
			.packtmp = packtmp,
		};
		ret = ముగింపు_pack_objects_cmd(repo->hash_algo, &opts, &cmd,
					      &పేర్లు);
		ఒకవేళ (ret)
			వెళ్ళు cleanup;
	}

	ఒకవేళ (!పేర్లు.nr) {
		నిర్మాణం odb_ఆధారం_files *files = odb_ఆధారం_files_downcast(existing.ఆధారం);

		ఒకవేళ (!po_వాదనలు.quiet)
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
		ఒకవేళ (!get_multi_pack_సూచీ(files->dirs->packed))
			midx_must_contain_cruft = 1;
	}

	ఒకవేళ (pack_everything & PACK_CRUFT) {
		నిర్మాణం write_pack_opts opts = {
			.po_వాదనలు = &cruft_po_వాదనలు,
			.destination = packtmp,
			.packtmp = packtmp,
			.packdir = packdir,
		};

		ఒకవేళ (!cruft_po_వాదనలు.కిటికీ)
			cruft_po_వాదనలు.కిటికీ = xstrdup_or_null(po_వాదనలు.కిటికీ);
		ఒకవేళ (!cruft_po_వాదనలు.కిటికీ_జ్ఞాపకం)
			cruft_po_వాదనలు.కిటికీ_జ్ఞాపకం = xstrdup_or_null(po_వాదనలు.కిటికీ_జ్ఞాపకం);
		ఒకవేళ (!cruft_po_వాదనలు.లోతు)
			cruft_po_వాదనలు.లోతు = xstrdup_or_null(po_వాదనలు.లోతు);
		ఒకవేళ (!cruft_po_వాదనలు.దారాలు)
			cruft_po_వాదనలు.దారాలు = xstrdup_or_null(po_వాదనలు.దారాలు);
		ఒకవేళ (!cruft_po_వాదనలు.max_pack_కొలత)
			cruft_po_వాదనలు.max_pack_కొలత = po_వాదనలు.max_pack_కొలత;

		cruft_po_వాదనలు.local = po_వాదనలు.local;
		cruft_po_వాదనలు.quiet = po_వాదనలు.quiet;
		cruft_po_వాదనలు.delta_పునాది_స్థానభ్రంశం = po_వాదనలు.delta_పునాది_స్థానభ్రంశం;
		cruft_po_వాదనలు.pack_kept_objects = 0;

		ret = write_cruft_pack(&opts, cruft_expiration,
				       combine_cruft_క్రింద_కొలత, &పేర్లు,
				       &existing);
		ఒకవేళ (ret)
			వెళ్ళు cleanup;

		ఒకవేళ (delete_redundant && expire_గమ్యం) {
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
			opts.destination = expire_గమ్యం;
			ret = write_cruft_pack(&opts, ఖాళీ, 0ul, &పేర్లు,
					       &existing);
			ఒకవేళ (ret)
				వెళ్ళు cleanup;
		}
	}

	ఒకవేళ (po_వాదనలు.filter_options.choice && !drop_filtered) {
		నిర్మాణం write_pack_opts opts = {
			.po_వాదనలు = &po_వాదనలు,
			.destination = filter_గమ్యం,
			.packdir = packdir,
			.packtmp = packtmp,
		};

		ఒకవేళ (!opts.destination)
			opts.destination = packtmp;

		ret = write_filtered_pack(&opts, &existing, &పేర్లు);
		ఒకవేళ (ret)
			వెళ్ళు cleanup;
	}

	string_list_క్రమీకరించు(&పేర్లు);

	odb_close(repo->objects);

	/*
	 * Ok we have prepared all new packfiles.
	 */
	for_each_string_list_అంశం(అంశం, &పేర్లు)
		generated_pack_install(అంశం->util, అంశం->string, packdir,
				       packtmp);
	/* End of pack replacement. */

	ఒకవేళ (delete_redundant && pack_everything & ALL_INTO_ONE) {
		ఒకవేళ (write_midx == REPACK_WRITE_MIDX_INCREMENTAL)
			existing_packs_retain_midx_packs(&existing);
		existing_packs_mark_for_deletion(&existing, &పేర్లు);
	}

	ఒకవేళ (write_midx != REPACK_WRITE_MIDX_NONE) {
		నిర్మాణం repack_write_midx_opts opts = {
			.existing = &existing,
			.geometry = &geometry,
			.పేర్లు = &పేర్లు,
			.refs_snapshot = refs_snapshot ? get_tempfile_మార్గం(refs_snapshot) : ఖాళీ,
			.packdir = packdir,
			.show_పురోగతి = show_పురోగతి,
			.write_bitmaps = write_bitmaps > 0,
			.midx_must_contain_cruft = midx_must_contain_cruft,
			.midx_విభజించు_గుణకం = config_ctx.midx_విభజించు_గుణకం,
			.midx_new_layer_పరిమితి = config_ctx.midx_new_layer_పరిమితి,
			.బహుళకం = write_midx,
		};

		ret = repack_write_midx(&opts);
		ఒకవేళ (ret)
			వెళ్ళు cleanup;
	}

	odb_reprepare(repo->objects);

	ఒకవేళ (delete_redundant) {
		పూర్ణాంకం opts = 0;
		bool wrote_incremental_midx = write_midx == REPACK_WRITE_MIDX_INCREMENTAL;

		existing_packs_remove_redundant(&existing, packdir,
						wrote_incremental_midx);

		ఒకవేళ (geometry.విభజించు_గుణకం)
			pack_geometry_remove_redundant(&geometry, &పేర్లు,
						       &existing, packdir,
						       wrote_incremental_midx);
		ఒకవేళ (show_పురోగతి)
			opts |= PRUNE_PACKED_VERBOSE;
		prune_packed_objects(opts);

		ఒకవేళ (!keep_unreachable &&
		    (!(pack_everything & LOOSEN_UNREACHABLE) ||
		     unpack_unreachable) &&
		    is_repository_shallow(repo))
			prune_shallow(PRUNE_శీఘ్ర);
	}

	ఒకవేళ (నిరంతరం_update_server_info)
		update_server_info(repo, 0);

	ఒకవేళ (git_env_bool(GIT_TEST_MULTI_PACK_సూచీ, 0)) {
		నిర్మాణం odb_ఆధారం_files *files = odb_ఆధారం_files_downcast(existing.ఆధారం);
		గుర్తులేని flags = 0;

		ఒకవేళ (git_env_bool(GIT_TEST_MULTI_PACK_సూచీ_WRITE_INCREMENTAL, 0))
			flags |= MIDX_WRITE_INCREMENTAL;
		write_midx_file(files->dirs->packed, ఖాళీ, ఖాళీ, flags);
	}

cleanup:
	string_list_clear(&keep_pack_list, 0);
	string_list_clear(&పేర్లు, 1);
	oidset_clear(&drop_oids);
	existing_packs_release(&existing);
	pack_geometry_release(&geometry);
	pack_objects_వాదనలు_release(&po_వాదనలు);
	pack_objects_వాదనలు_release(&cruft_po_వాదనలు);

	తిరిగి ret;
}
