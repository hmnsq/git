#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "server-info.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "midx.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "prune-packed.h"
#अन्तर्भाव "promisor-remote.h"
#अन्तर्भाव "repack.h"
#अन्तर्भाव "shallow.h"
#अन्तर्भाव "list-objects-filter-options.h"
#अन्तर्भाव "oidset.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "wt-status.h"
#अन्तर्भाव "read-cache-ll.h"

#परिभाषय ALL_INTO_ONE 1
#परिभाषय LOOSEN_UNREACHABLE 2
#परिभाषय PACK_CRUFT 4

#परिभाषय DELETE_PACK 1
#परिभाषय RETAIN_PACK 2

स्थिर पूर्णाङ्क pack_everything;
स्थिर पूर्णाङ्क write_bitmaps = -1;
स्थिर पूर्णाङ्क use_delta_islands;
स्थिर पूर्णाङ्क सततम्_update_server_info = 1;
स्थिर वर्ण *packdir, *packtmp_name, *packtmp;
स्थिर पूर्णाङ्क midx_must_contain_cruft = 1;
स्थिर पूर्णाङ्क drop_filtered;
स्थिर पूर्णाङ्क dry_सततम्;
स्थिर पूर्णाङ्क write_bitmaps_given;

स्थिर ध्रुव वर्ण *ध्रुव git_repack_usage[] = {
	N_("git repack [-a] [-A] [-d] [-f] [-F] [-l] [-n] [-q] [-b] [-m]\n"
	   "[--window=<n>] [--depth=<n>] [--threads=<n>] [--keep-pack=<pack-name>]\n"
	   "[--write-midx[=<mode>]] [--name-hash-version=<n>] [--path-walk]\n"
	   "[--filter=<filter-spec>] [--drop-filtered [--dry-run]]"),
	रिक्त
};

स्थिर ध्रुव वर्ण incremental_bitmap_conflict_error[] = N_(
"Incremental repacks are incompatible with bitmap indexes.  Use\n"
"--no-write-bitmap-index or disable the pack.writeBitmaps configuration."
);

#परिभाषय DEFAULT_MIDX_विभज_गुणकः 2
#परिभाषय DEFAULT_MIDX_NEW_LAYER_देहली 8

संरचना repack_config_ctx {
	संरचना pack_objects_तर्काः *po_तर्काः;
	संरचना pack_objects_तर्काः *cruft_po_तर्काः;
	पूर्णाङ्क midx_विभज_गुणकः;
	पूर्णाङ्क midx_new_layer_देहली;
};

स्थिर पूर्णाङ्क repack_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			 ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना repack_config_ctx *repack_ctx = cb;
	संरचना pack_objects_तर्काः *po_तर्काः = repack_ctx->po_तर्काः;
	संरचना pack_objects_तर्काः *cruft_po_तर्काः = repack_ctx->cruft_po_तर्काः;
	यदि (!वाक्यं_तुलय(var, "repack.usedeltabaseoffset")) {
		po_तर्काः->delta_आधारः_विस्थापनम् = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "repack.packkeptobjects")) {
		po_तर्काः->pack_kept_objects = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "repack.writebitmaps") ||
	    !वाक्यं_तुलय(var, "pack.writebitmaps")) {
		write_bitmaps = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "repack.usedeltaislands")) {
		use_delta_islands = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (वाक्यं_तुलय(var, "repack.updateserverinfo") == 0) {
		सततम्_update_server_info = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "repack.cruftwindow")) {
		मुक्त(cruft_po_तर्काः->गवाक्षः);
		प्रत्यागम git_config_string(&cruft_po_तर्काः->गवाक्षः, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "repack.cruftwindowmemory")) {
		मुक्त(cruft_po_तर्काः->गवाक्षः_स्मृतिः);
		प्रत्यागम git_config_string(&cruft_po_तर्काः->गवाक्षः_स्मृतिः, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "repack.cruftdepth")) {
		मुक्त(cruft_po_तर्काः->गभीरता);
		प्रत्यागम git_config_string(&cruft_po_तर्काः->गभीरता, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "repack.cruftthreads")) {
		मुक्त(cruft_po_तर्काः->तन्तवः);
		प्रत्यागम git_config_string(&cruft_po_तर्काः->तन्तवः, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "repack.midxmustcontaincruft")) {
		midx_must_contain_cruft = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "repack.midxsplitfactor")) {
		repack_ctx->midx_विभज_गुणकः = git_config_int(var, मानम्,
							       ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "repack.midxnewlayerthreshold")) {
		repack_ctx->midx_new_layer_देहली = git_config_int(var, मानम्,
								      ctx->kvi);
		प्रत्यागम 0;
	}
	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

स्थिर पूर्णाङ्क option_parse_write_bitmaps(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				      पूर्णाङ्क unset)
{
	पूर्णाङ्क *मानम् = opt->मानम्;

	BUG_ON_OPT_ARG(arg);
	यदि (unset)
		*मानम् = 0;
	अन्यथा
		*मानम् = 1;

	write_bitmaps_given = 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_write_midx(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				   पूर्णाङ्क unset)
{
	गणना repack_write_midx_बहुलकम् *cfg = opt->मानम्;

	यदि (unset) {
		*cfg = REPACK_WRITE_MIDX_NONE;
		प्रत्यागम 0;
	}

	यदि (!arg || !*arg)
		*cfg = REPACK_WRITE_MIDX_DEFAULT;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "incremental"))
		*cfg = REPACK_WRITE_MIDX_INCREMENTAL;
	अन्यथा
		प्रत्यागम error(_("unknown value for %s: %s"), opt->long_name, arg);

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_repack(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	संरचना string_list_item *item;
	संरचना string_list नामानि = STRING_LIST_INIT_DUP;
	संरचना existing_packs existing = EXISTING_PACKS_INIT;
	संरचना oidset drop_oids = OIDSET_INIT;
	संरचना pack_geometry geometry = { 0 };
	संरचना tempfile *refs_snapshot = रिक्त;
	पूर्णाङ्क i, ret;
	पूर्णाङ्क show_प्रगतिः;

	/* variables to be filled by option parsing */
	संरचना repack_config_ctx config_ctx;
	पूर्णाङ्क delete_redundant = 0;
	ध्रुव वर्ण *unpack_unreachable = रिक्त;
	पूर्णाङ्क keep_unreachable = 0;
	संरचना string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	संरचना pack_objects_तर्काः po_तर्काः = PACK_OBJECTS_तर्काः_INIT;
	संरचना pack_objects_तर्काः cruft_po_तर्काः = PACK_OBJECTS_तर्काः_INIT;
	गणना repack_write_midx_बहुलकम् write_midx = REPACK_WRITE_MIDX_NONE;
	ध्रुव वर्ण *cruft_expiration = रिक्त;
	ध्रुव वर्ण *expire_गन्तव्यम् = रिक्त;
	ध्रुव वर्ण *filter_गन्तव्यम् = रिक्त;
	ध्रुव वर्ण *opt_गवाक्षः = रिक्त;
	ध्रुव वर्ण *opt_गवाक्षः_स्मृतिः = रिक्त;
	ध्रुव वर्ण *opt_गभीरता = रिक्त;
	ध्रुव वर्ण *opt_तन्तवः = रिक्त;
	अचिह्नित दीर्घ combine_cruft_अधः_आकार = 0ul;

	संरचना option builtin_repack_options[] = {
		OPT_BIT('a', रिक्त, &pack_everything,
				N_("pack everything in a single pack"), ALL_INTO_ONE),
		OPT_BIT('A', रिक्त, &pack_everything,
				N_("same as -a, and turn unreachable objects loose"),
				   LOOSEN_UNREACHABLE | ALL_INTO_ONE),
		OPT_BIT(0, "cruft", &pack_everything,
				N_("same as -a, pack unreachable cruft objects separately"),
				   PACK_CRUFT),
		OPT_STRING(0, "cruft-expiration", &cruft_expiration, N_("approxidate"),
				N_("with --cruft, expire objects older than this")),
		OPT_UNSIGNED(0, "combine-cruft-below-size",
			     &combine_cruft_अधः_आकार,
			     N_("with --cruft, only repack cruft packs smaller than this")),
		OPT_UNSIGNED(0, "max-cruft-size", &cruft_po_तर्काः.max_pack_आकार,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL('d', रिक्त, &delete_redundant,
				N_("remove redundant packs, and run git-prune-packed")),
		OPT_BOOL('f', रिक्त, &po_तर्काः.no_reuse_delta,
				N_("pass --no-reuse-delta to git-pack-objects")),
		OPT_BOOL('F', रिक्त, &po_तर्काः.no_reuse_object,
				N_("pass --no-reuse-object to git-pack-objects")),
		OPT_INTEGER(0, "name-hash-version", &po_तर्काः.name_hash_संस्करणम्,
				N_("specify the name hash version to use for grouping similar objects by path")),
		OPT_BOOL(0, "path-walk", &po_तर्काः.मार्गः_walk,
				N_("pass --path-walk to git-pack-objects")),
		OPT_NEGBIT('n', रिक्त, &सततम्_update_server_info,
				N_("do not run git-update-server-info"), 1),
		OPT__QUIET(&po_तर्काः.quiet, N_("be quiet")),
		OPT_BOOL('l', "local", &po_तर्काः.local,
				N_("pass --local to git-pack-objects")),
		OPT_CALLBACK_F('b', "write-bitmap-index", &write_bitmaps, रिक्त,
				N_("write bitmap index"),
				PARSE_OPT_NOARG, option_parse_write_bitmaps),
		OPT_BOOL('i', "delta-islands", &use_delta_islands,
				N_("pass --delta-islands to git-pack-objects")),
		OPT_STRING(0, "unpack-unreachable", &unpack_unreachable, N_("approxidate"),
				N_("with -A, do not loosen objects older than this")),
		OPT_BOOL('k', "keep-unreachable", &keep_unreachable,
				N_("with -a, repack unreachable objects")),
		OPT_STRING(0, "window", &opt_गवाक्षः, N_("n"),
				N_("size of the window used for delta compression")),
		OPT_STRING(0, "window-memory", &opt_गवाक्षः_स्मृतिः, N_("bytes"),
				N_("same as the above, but limit memory size instead of entries count")),
		OPT_STRING(0, "depth", &opt_गभीरता, N_("n"),
				N_("limits the maximum delta depth")),
		OPT_STRING(0, "threads", &opt_तन्तवः, N_("n"),
				N_("limits the maximum number of threads")),
		OPT_UNSIGNED(0, "max-pack-size", &po_तर्काः.max_pack_आकार,
			     N_("maximum size of each packfile")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&po_तर्काः.filter_options),
		OPT_BOOL(0, "pack-kept-objects", &po_तर्काः.pack_kept_objects,
				N_("repack objects in packs marked with .keep")),
		OPT_STRING_LIST(0, "keep-pack", &keep_pack_list, N_("name"),
				N_("do not repack this pack")),
		OPT_INTEGER('g', "geometric", &geometry.विभज_गुणकः,
			    N_("find a geometric progression with factor <N>")),
		OPT_CALLBACK_F(0, "write-midx", &write_midx,
			   N_("mode"),
			   N_("write a multi-pack index of the resulting packs"),
			   PARSE_OPT_OPTARG, option_parse_write_midx),
		OPT_समुच्चयः_INT_F('m', रिक्त, &write_midx,
			   N_("write a multi-pack index of the resulting packs"),
			   REPACK_WRITE_MIDX_DEFAULT,
			   PARSE_OPT_HIDDEN),
		OPT_STRING(0, "expire-to", &expire_गन्तव्यम्, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_STRING(0, "filter-to", &filter_गन्तव्यम्, N_("dir"),
			   N_("pack prefix to store a pack containing filtered out objects")),
		OPT_BOOL(0, "drop-filtered", &drop_filtered,
				N_("delete filtered out objects (requires --filter)")),
		OPT_BOOL(0, "dry-run", &dry_सततम्,
				N_("only show which objects would be dropped")),
		OPT_अन्तः()
	};

	list_objects_filter_init(&po_तर्काः.filter_options);

	स्मृतिं_पूरय(&config_ctx, 0, परिमाण(config_ctx));
	config_ctx.po_तर्काः = &po_तर्काः;
	config_ctx.cruft_po_तर्काः = &cruft_po_तर्काः;
	config_ctx.midx_विभज_गुणकः = DEFAULT_MIDX_विभज_गुणकः;
	config_ctx.midx_new_layer_देहली = DEFAULT_MIDX_NEW_LAYER_देहली;

	repo_config(repo, repack_config, &config_ctx);

	argc = parse_options(argc, argv, उपसर्गः, builtin_repack_options,
				git_repack_usage, 0);

	po_तर्काः.गवाक्षः = xstrdup_or_null(opt_गवाक्षः);
	po_तर्काः.गवाक्षः_स्मृतिः = xstrdup_or_null(opt_गवाक्षः_स्मृतिः);
	po_तर्काः.गभीरता = xstrdup_or_null(opt_गभीरता);
	po_तर्काः.तन्तवः = xstrdup_or_null(opt_तन्तवः);

	die_for_incompatible_opt2(drop_filtered, "--drop-filtered",
		!!filter_गन्तव्यम्, "--filter-to");

	यदि (dry_सततम् && !drop_filtered)
		die(_("--dry-run only takes effect with --drop-filtered"));

	यदि (drop_filtered) {
		यदि (!po_तर्काः.filter_options.choice)
			die(_("--drop-filtered requires --filter"));

		यदि (!(pack_everything & ALL_INTO_ONE))
			die(_("--drop-filtered requires -a"));

		/*
		 * Only blob:limit=<n> is supported for now. Reject other
		 * filter choices early, before walking the object database.
		 */
		यदि (po_तर्काः.filter_options.choice != LOFC_BLOB_सीमा)
			die(_("--drop-filtered only supports --filter=blob:limit=<n> for now"));

		/*
		 * An explicit -b on the command line is a conflict we have to
		 * report; a bitmap setting from config is silently overridden
		 * for the duration of the command.
		 */
		यदि (write_bitmaps_given && write_bitmaps > 0)
			die(_("options '%s' and '%s' cannot be used together"),
				"--drop-filtered", "--write-bitmap-index");

		/*
		 * Without a promisor remote there is nowhere to re-fetch the
		 * dropped objects from, so dropping them would be permanent
		 * data loss.
		 */
		यदि (!repo_has_promisor_remote(repo))
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
		यदि (!is_bare_repository(repo)) {
			संरचना wt_स्थितिः_state state = { 0 };

			wt_स्थितिः_get_state(repo, &state, 0);
			यदि (state.merge_अन्तर्_प्रगतिः || state.revert_अन्तर्_प्रगतिः ||
			    state.rebase_अन्तर्_प्रगतिः || state.bisect_अन्तर्_प्रगतिः ||
			    state.cherry_pick_अन्तर्_प्रगतिः || state.am_अन्तर्_प्रगतिः ||
			    state.rebase_interactive_अन्तर्_प्रगतिः) {
				wt_स्थितिः_state_free_buffers(&state);
				die(_("--drop-filtered cannot be used while "
				      "another operation (merge, rebase, am, "
				      "cherry-pick, revert, or bisect) is in "
				      "progress"));
			}
			wt_स्थितिः_state_free_buffers(&state);
		}

		write_bitmaps = 0;

		/*
		 * Dropping objects means rebuilding the promisor packs
		 * without them and then removing the old packs, so the
		 * redundant packs must be deleted. Imply -d on a real run.
		 */
		यदि (!dry_सततम्)
			delete_redundant = 1;

		ret = enumerate_promisor_blobs(repo, &po_तर्काः.filter_options, &drop_oids);

		यदि (ret)
			गच्छ cleanup;

		/*
		 * Refuse to drop blobs that the current index references.
		 * Such a blob would only be lazily re-fetched by the next
		 * command that touches the worktree, so dropping it reclaims
		 * nothing. This guard just avoids that churn. Bare
		 * repositories have no index, so the check is skipped there.
		 */
		यदि (!is_bare_repository(repo) && oidset_आकार(&drop_oids)) {
			संरचना सूचकांक_state *istate = repo->सूचकांक;
			अचिह्नित पूर्णाङ्क i;

			यदि (repo_read_सूचकांक(repo) < 0)
				die(_("could not read the index"));

			प्रति (i = 0; i < istate->cache_nr; i++) {
				ध्रुव संरचना cache_प्रविष्टिः *ce = istate->cache[i];

				यदि (oidset_अन्तर्भवति(&drop_oids, &ce->oid))
					die(_("cannot drop '%s' (%s): it is referenced by the current index"),
						ce->name, oid_गन्तव्यम्_hex(&ce->oid));
			}
		}

		यदि (dry_सततम्) {
			संरचना oidset_iter iter;
			ध्रुव संरचना object_id *oid;

			oidset_iter_init(&drop_oids, &iter);
			यावत् ((oid = oidset_iter_अग्र(&iter)))
				प्रारूप_लिख("%s\n", oid_गन्तव्यम्_hex(oid));
		}
	}

	यदि (delete_redundant && repo->repository_format_precious_objects)
		die(_("cannot delete packs in a precious-objects repo"));

	die_for_incompatible_opt3(unpack_unreachable || (pack_everything & LOOSEN_UNREACHABLE), "-A",
				  keep_unreachable, "-k/--keep-unreachable",
				  pack_everything & PACK_CRUFT, "--cruft");

	यदि (pack_everything & PACK_CRUFT)
		pack_everything |= ALL_INTO_ONE;

	यदि (write_bitmaps < 0) {
		यदि (write_midx == REPACK_WRITE_MIDX_NONE &&
		    (!(pack_everything & ALL_INTO_ONE) || !is_bare_repository(repo)))
			write_bitmaps = 0;
	}
	यदि (po_तर्काः.pack_kept_objects < 0)
		po_तर्काः.pack_kept_objects = write_bitmaps > 0 &&
			write_midx == REPACK_WRITE_MIDX_NONE;

	यदि (write_bitmaps && !(pack_everything & ALL_INTO_ONE) &&
	    write_midx == REPACK_WRITE_MIDX_NONE)
		die(_(incremental_bitmap_conflict_error));

	यदि (write_bitmaps && po_तर्काः.local &&
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

	यदि (config_ctx.midx_विभज_गुणकः < 2)
		die(_("invalid value for %s: %d"), "--midx-split-factor",
		    config_ctx.midx_विभज_गुणकः);
	यदि (config_ctx.midx_new_layer_देहली < 1)
		die(_("invalid value for %s: %d"), "--midx-new-layer-threshold",
		    config_ctx.midx_new_layer_देहली);

	यदि (write_midx != REPACK_WRITE_MIDX_NONE && write_bitmaps) {
		संरचना strbuf मार्गः = STRBUF_INIT;

		strbuf_addf(&मार्गः, "%s/%s_XXXXXX",
			    repo_get_object_directory(repo),
			    "bitmap-ref-tips");

		refs_snapshot = xmks_tempfile(मार्गः.buf);
		midx_snapshot_refs(repo, refs_snapshot);

		strbuf_release(&मार्गः);
	}

	packdir = mkpathdup("%s/pack", repo_get_object_directory(repo));
	packtmp_name = xstrfmt(".tmp-%d-pack", (पूर्णाङ्क)getpid());
	packtmp = mkpathdup("%s/%s", packdir, packtmp_name);

	existing.repo = repo;
	existing_packs_collect(&existing, &keep_pack_list);

	यदि (geometry.विभज_गुणकः) {
		यदि (pack_everything)
			die(_("options '%s' and '%s' cannot be used together"), "--geometric", "-A/-a");
		यदि (write_midx == REPACK_WRITE_MIDX_INCREMENTAL) {
			geometry.midx_layer_देहली = config_ctx.midx_new_layer_देहली;
			geometry.midx_layer_देहली_समुच्चयः = true;
		}
		pack_geometry_init(&geometry, &existing, &po_तर्काः);
		pack_geometry_विभज(&geometry);
	}

	prepare_pack_objects(&cmd, &po_तर्काः, packtmp);

	show_प्रगतिः = !po_तर्काः.quiet && isatty(2);

	strvec_प्रक्षिप(&cmd.तर्काः, "--keep-true-parents");
	प्रति (i = 0; i < keep_pack_list.nr; i++)
		strvec_pushf(&cmd.तर्काः, "--keep-pack=%s",
			     keep_pack_list.वस्तूनि[i].string);
	strvec_प्रक्षिप(&cmd.तर्काः, "--non-empty");
	यदि (!geometry.विभज_गुणकः) {
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
		strvec_प्रक्षिप(&cmd.तर्काः, "--all");
		strvec_प्रक्षिप(&cmd.तर्काः, "--reflog");
		strvec_प्रक्षिप(&cmd.तर्काः, "--indexed-objects");
	}
	यदि (repo_has_promisor_remote(repo))
		strvec_प्रक्षिप(&cmd.तर्काः, "--exclude-promisor-objects");
	यदि (write_midx == REPACK_WRITE_MIDX_NONE) {
		यदि (write_bitmaps > 0)
			strvec_प्रक्षिप(&cmd.तर्काः, "--write-bitmap-index");
		अन्यथा यदि (write_bitmaps < 0)
			strvec_प्रक्षिप(&cmd.तर्काः, "--write-bitmap-index-quiet");
	}
	यदि (use_delta_islands)
		strvec_प्रक्षिप(&cmd.तर्काः, "--delta-islands");

	यदि (pack_everything & ALL_INTO_ONE) {
		repack_promisor_objects(repo, &po_तर्काः, &नामानि, packtmp,
			(drop_filtered && !dry_सततम्) ? &drop_oids : रिक्त);

		यदि (existing_packs_has_non_kept(&existing) &&
		    delete_redundant &&
		    !(pack_everything & PACK_CRUFT)) {
			for_each_string_list_item(item, &नामानि) {
				strvec_pushf(&cmd.तर्काः, "--keep-pack=%s-%s.pack",
					     packtmp_name, item->string);
			}
			यदि (unpack_unreachable) {
				strvec_pushf(&cmd.तर्काः,
					     "--unpack-unreachable=%s",
					     unpack_unreachable);
			} अन्यथा यदि (pack_everything & LOOSEN_UNREACHABLE) {
				strvec_प्रक्षिप(&cmd.तर्काः,
					    "--unpack-unreachable");
			} अन्यथा यदि (keep_unreachable) {
				strvec_प्रक्षिप(&cmd.तर्काः, "--keep-unreachable");
			}
		}

		यदि (keep_unreachable && delete_redundant &&
		    !(pack_everything & PACK_CRUFT))
			strvec_प्रक्षिप(&cmd.तर्काः, "--pack-loose-unreachable");
	} अन्यथा यदि (geometry.विभज_गुणकः) {
		pack_geometry_repack_promisors(repo, &po_तर्काः, &geometry,
					       &नामानि, packtmp);

		यदि (midx_must_contain_cruft)
			strvec_प्रक्षिप(&cmd.तर्काः, "--stdin-packs");
		अन्यथा
			strvec_प्रक्षिप(&cmd.तर्काः, "--stdin-packs=follow");
		strvec_प्रक्षिप(&cmd.तर्काः, "--unpacked");
	} अन्यथा {
		strvec_प्रक्षिप(&cmd.तर्काः, "--unpacked");
		strvec_प्रक्षिप(&cmd.तर्काः, "--incremental");
	}

	यदि (po_तर्काः.filter_options.choice)
		strvec_pushf(&cmd.तर्काः, "--filter=%s",
			     expand_list_objects_filter_spec(&po_तर्काः.filter_options));
	अन्यथा यदि (filter_गन्तव्यम्)
		die(_("option '%s' can only be used along with '%s'"), "--filter-to", "--filter");

	यदि (geometry.विभज_गुणकः)
		cmd.अन्तर् = -1;
	अन्यथा
		cmd.no_stdin = 1;

	ret = start_command(&cmd);
	यदि (ret)
		गच्छ cleanup;

	यदि (geometry.विभज_गुणकः) {
		FILE *अन्तर् = xfdopen(cmd.अन्तर्, "w");
		/*
		 * The resulting pack should contain all objects in packs that
		 * are going to be rolled up, but exclude objects in packs which
		 * are being left alone.
		 */
		प्रति (i = 0; i < geometry.विभज; i++)
			fprintf(अन्तर्, "%s\n", pack_basename(geometry.pack[i]));
		प्रति (i = geometry.विभज; i < geometry.pack_nr; i++) {
			ध्रुव वर्ण *basename = pack_basename(geometry.pack[i]);
			वर्ण marker = '^';

			यदि (!midx_must_contain_cruft &&
			    !string_list_has_string(&existing.midx_packs,
						    basename)) {
				/*
				 * Assume non-MIDX'd packs are not
				 * necessarily closed under
				 * reachability.
				 */
				marker = '!';
			}

			fprintf(अन्तर्, "%c%s\n", marker, basename);
		}
		fclose(अन्तर्);
	}

	{
		संरचना write_pack_opts opts = {
			.packdir = packdir,
			.destination = packdir,
			.packtmp = packtmp,
		};
		ret = finish_pack_objects_cmd(repo->hash_algo, &opts, &cmd,
					      &नामानि);
		यदि (ret)
			गच्छ cleanup;
	}

	यदि (!नामानि.nr) {
		संरचना odb_source_files *files = odb_source_files_downcast(existing.source);

		यदि (!po_तर्काः.quiet)
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
		यदि (!get_multi_pack_सूचकांक(files->dirs->packed))
			midx_must_contain_cruft = 1;
	}

	यदि (pack_everything & PACK_CRUFT) {
		संरचना write_pack_opts opts = {
			.po_तर्काः = &cruft_po_तर्काः,
			.destination = packtmp,
			.packtmp = packtmp,
			.packdir = packdir,
		};

		यदि (!cruft_po_तर्काः.गवाक्षः)
			cruft_po_तर्काः.गवाक्षः = xstrdup_or_null(po_तर्काः.गवाक्षः);
		यदि (!cruft_po_तर्काः.गवाक्षः_स्मृतिः)
			cruft_po_तर्काः.गवाक्षः_स्मृतिः = xstrdup_or_null(po_तर्काः.गवाक्षः_स्मृतिः);
		यदि (!cruft_po_तर्काः.गभीरता)
			cruft_po_तर्काः.गभीरता = xstrdup_or_null(po_तर्काः.गभीरता);
		यदि (!cruft_po_तर्काः.तन्तवः)
			cruft_po_तर्काः.तन्तवः = xstrdup_or_null(po_तर्काः.तन्तवः);
		यदि (!cruft_po_तर्काः.max_pack_आकार)
			cruft_po_तर्काः.max_pack_आकार = po_तर्काः.max_pack_आकार;

		cruft_po_तर्काः.local = po_तर्काः.local;
		cruft_po_तर्काः.quiet = po_तर्काः.quiet;
		cruft_po_तर्काः.delta_आधारः_विस्थापनम् = po_तर्काः.delta_आधारः_विस्थापनम्;
		cruft_po_तर्काः.pack_kept_objects = 0;

		ret = write_cruft_pack(&opts, cruft_expiration,
				       combine_cruft_अधः_आकार, &नामानि,
				       &existing);
		यदि (ret)
			गच्छ cleanup;

		यदि (delete_redundant && expire_गन्तव्यम्) {
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
			opts.destination = expire_गन्तव्यम्;
			ret = write_cruft_pack(&opts, रिक्त, 0ul, &नामानि,
					       &existing);
			यदि (ret)
				गच्छ cleanup;
		}
	}

	यदि (po_तर्काः.filter_options.choice && !drop_filtered) {
		संरचना write_pack_opts opts = {
			.po_तर्काः = &po_तर्काः,
			.destination = filter_गन्तव्यम्,
			.packdir = packdir,
			.packtmp = packtmp,
		};

		यदि (!opts.destination)
			opts.destination = packtmp;

		ret = write_filtered_pack(&opts, &existing, &नामानि);
		यदि (ret)
			गच्छ cleanup;
	}

	string_list_क्रमय(&नामानि);

	odb_close(repo->objects);

	/*
	 * Ok we have prepared all new packfiles.
	 */
	for_each_string_list_item(item, &नामानि)
		generated_pack_install(item->util, item->string, packdir,
				       packtmp);
	/* End of pack replacement. */

	यदि (delete_redundant && pack_everything & ALL_INTO_ONE) {
		यदि (write_midx == REPACK_WRITE_MIDX_INCREMENTAL)
			existing_packs_retain_midx_packs(&existing);
		existing_packs_mark_for_deletion(&existing, &नामानि);
	}

	यदि (write_midx != REPACK_WRITE_MIDX_NONE) {
		संरचना repack_write_midx_opts opts = {
			.existing = &existing,
			.geometry = &geometry,
			.नामानि = &नामानि,
			.refs_snapshot = refs_snapshot ? get_tempfile_मार्गः(refs_snapshot) : रिक्त,
			.packdir = packdir,
			.show_प्रगतिः = show_प्रगतिः,
			.write_bitmaps = write_bitmaps > 0,
			.midx_must_contain_cruft = midx_must_contain_cruft,
			.midx_विभज_गुणकः = config_ctx.midx_विभज_गुणकः,
			.midx_new_layer_देहली = config_ctx.midx_new_layer_देहली,
			.बहुलकम् = write_midx,
		};

		ret = repack_write_midx(&opts);
		यदि (ret)
			गच्छ cleanup;
	}

	odb_reprepare(repo->objects);

	यदि (delete_redundant) {
		पूर्णाङ्क opts = 0;
		bool wrote_incremental_midx = write_midx == REPACK_WRITE_MIDX_INCREMENTAL;

		existing_packs_remove_redundant(&existing, packdir,
						wrote_incremental_midx);

		यदि (geometry.विभज_गुणकः)
			pack_geometry_remove_redundant(&geometry, &नामानि,
						       &existing, packdir,
						       wrote_incremental_midx);
		यदि (show_प्रगतिः)
			opts |= PRUNE_PACKED_VERBOSE;
		prune_packed_objects(opts);

		यदि (!keep_unreachable &&
		    (!(pack_everything & LOOSEN_UNREACHABLE) ||
		     unpack_unreachable) &&
		    is_repository_shallow(repo))
			prune_shallow(PRUNE_शीघ्र);
	}

	यदि (सततम्_update_server_info)
		update_server_info(repo, 0);

	यदि (git_env_bool(GIT_TEST_MULTI_PACK_सूचकांक, 0)) {
		संरचना odb_source_files *files = odb_source_files_downcast(existing.source);
		अचिह्नित flags = 0;

		यदि (git_env_bool(GIT_TEST_MULTI_PACK_सूचकांक_WRITE_INCREMENTAL, 0))
			flags |= MIDX_WRITE_INCREMENTAL;
		write_midx_file(files->dirs->packed, रिक्त, रिक्त, flags);
	}

cleanup:
	string_list_clear(&keep_pack_list, 0);
	string_list_clear(&नामानि, 1);
	oidset_clear(&drop_oids);
	existing_packs_release(&existing);
	pack_geometry_release(&geometry);
	pack_objects_तर्काः_release(&po_तर्काः);
	pack_objects_तर्काः_release(&cruft_po_तर्काः);

	प्रत्यागम ret;
}
