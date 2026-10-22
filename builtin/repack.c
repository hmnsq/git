#define DISABLE_SIGN_COMPARE_WARNINGS

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

#define ALL_INTO_ONE 1
#define LOOSEN_UNREACHABLE 2
#define PACK_CRUFT 4

#define DELETE_PACK 1
#define RETAIN_PACK 2

स्थिर पूर्णांक pack_everything;
स्थिर पूर्णांक write_bitmaps = -1;
स्थिर पूर्णांक use_delta_islands;
स्थिर पूर्णांक run_update_server_info = 1;
स्थिर अक्षर *packdir, *packtmp_name, *packtmp;
स्थिर पूर्णांक midx_must_contain_cruft = 1;
स्थिर पूर्णांक drop_filtered;
स्थिर पूर्णांक dry_run;
स्थिर पूर्णांक write_bitmaps_given;

स्थिर स्थिरांक अक्षर *स्थिरांक git_repack_usage[] = {
	N_("git repack [-a] [-A] [-d] [-f] [-F] [-l] [-n] [-q] [-b] [-m]\n"
	   "[--window=<n>] [--depth=<n>] [--threads=<n>] [--keep-pack=<pack-name>]\n"
	   "[--write-midx[=<mode>]] [--name-hash-version=<n>] [--path-walk]\n"
	   "[--filter=<filter-spec>] [--drop-filtered [--dry-run]]"),
	रिक्त
};

स्थिर स्थिरांक अक्षर incremental_bitmap_conflict_error[] = N_(
"Incremental repacks are incompatible with bitmap indexes.  Use\n"
"--no-write-bitmap-index or disable the pack.writeBitmaps configuration."
);

#define DEFAULT_MIDX_SPLIT_FACTOR 2
#define DEFAULT_MIDX_NEW_LAYER_THRESHOLD 8

संरचना repack_config_ctx {
	संरचना pack_objects_args *po_args;
	संरचना pack_objects_args *cruft_po_args;
	पूर्णांक midx_split_factor;
	पूर्णांक midx_new_layer_threshold;
};

स्थिर पूर्णांक repack_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
			 स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	संरचना repack_config_ctx *repack_ctx = cb;
	संरचना pack_objects_args *po_args = repack_ctx->po_args;
	संरचना pack_objects_args *cruft_po_args = repack_ctx->cruft_po_args;
	जर (!म_भेद(var, "repack.usedeltabaseoffset")) {
		po_args->delta_base_offset = git_config_bool(var, मूल्य);
		परत 0;
	}
	जर (!म_भेद(var, "repack.packkeptobjects")) {
		po_args->pack_kept_objects = git_config_bool(var, मूल्य);
		परत 0;
	}
	जर (!म_भेद(var, "repack.writebitmaps") ||
	    !म_भेद(var, "pack.writebitmaps")) {
		write_bitmaps = git_config_bool(var, मूल्य);
		परत 0;
	}
	जर (!म_भेद(var, "repack.usedeltaislands")) {
		use_delta_islands = git_config_bool(var, मूल्य);
		परत 0;
	}
	जर (म_भेद(var, "repack.updateserverinfo") == 0) {
		run_update_server_info = git_config_bool(var, मूल्य);
		परत 0;
	}
	जर (!म_भेद(var, "repack.cruftwindow")) {
		मुक्त(cruft_po_args->window);
		परत git_config_string(&cruft_po_args->window, var, मूल्य);
	}
	जर (!म_भेद(var, "repack.cruftwindowmemory")) {
		मुक्त(cruft_po_args->window_memory);
		परत git_config_string(&cruft_po_args->window_memory, var, मूल्य);
	}
	जर (!म_भेद(var, "repack.cruftdepth")) {
		मुक्त(cruft_po_args->depth);
		परत git_config_string(&cruft_po_args->depth, var, मूल्य);
	}
	जर (!म_भेद(var, "repack.cruftthreads")) {
		मुक्त(cruft_po_args->threads);
		परत git_config_string(&cruft_po_args->threads, var, मूल्य);
	}
	जर (!म_भेद(var, "repack.midxmustcontaincruft")) {
		midx_must_contain_cruft = git_config_bool(var, मूल्य);
		परत 0;
	}
	जर (!म_भेद(var, "repack.midxsplitfactor")) {
		repack_ctx->midx_split_factor = git_config_int(var, मूल्य,
							       ctx->kvi);
		परत 0;
	}
	जर (!म_भेद(var, "repack.midxnewlayerthreshold")) {
		repack_ctx->midx_new_layer_threshold = git_config_int(var, मूल्य,
								      ctx->kvi);
		परत 0;
	}
	परत git_default_config(var, मूल्य, ctx, cb);
}

स्थिर पूर्णांक option_parse_write_bitmaps(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg,
				      पूर्णांक unset)
{
	पूर्णांक *मूल्य = opt->मूल्य;

	BUG_ON_OPT_ARG(arg);
	जर (unset)
		*मूल्य = 0;
	अन्यथा
		*मूल्य = 1;

	write_bitmaps_given = 1;
	परत 0;
}

स्थिर पूर्णांक option_parse_write_midx(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg,
				   पूर्णांक unset)
{
	गणना repack_write_midx_mode *cfg = opt->मूल्य;

	जर (unset) {
		*cfg = REPACK_WRITE_MIDX_NONE;
		परत 0;
	}

	जर (!arg || !*arg)
		*cfg = REPACK_WRITE_MIDX_DEFAULT;
	अन्यथा जर (!म_भेद(arg, "incremental"))
		*cfg = REPACK_WRITE_MIDX_INCREMENTAL;
	अन्यथा
		परत error(_("unknown value for %s: %s"), opt->long_name, arg);

	परत 0;
}

पूर्णांक cmd_repack(पूर्णांक argc,
	       स्थिरांक अक्षर **argv,
	       स्थिरांक अक्षर *prefix,
	       संरचना repository *repo)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	संरचना string_list_item *item;
	संरचना string_list names = STRING_LIST_INIT_DUP;
	संरचना existing_packs existing = EXISTING_PACKS_INIT;
	संरचना oidset drop_oids = OIDSET_INIT;
	संरचना pack_geometry geometry = { 0 };
	संरचना tempfile *refs_snapshot = रिक्त;
	पूर्णांक i, ret;
	पूर्णांक show_progress;

	/* variables to be filled by option parsing */
	संरचना repack_config_ctx config_ctx;
	पूर्णांक delete_redundant = 0;
	स्थिरांक अक्षर *unpack_unreachable = रिक्त;
	पूर्णांक keep_unreachable = 0;
	संरचना string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	संरचना pack_objects_args po_args = PACK_OBJECTS_ARGS_INIT;
	संरचना pack_objects_args cruft_po_args = PACK_OBJECTS_ARGS_INIT;
	गणना repack_write_midx_mode write_midx = REPACK_WRITE_MIDX_NONE;
	स्थिरांक अक्षर *cruft_expiration = रिक्त;
	स्थिरांक अक्षर *expire_to = रिक्त;
	स्थिरांक अक्षर *filter_to = रिक्त;
	स्थिरांक अक्षर *opt_window = रिक्त;
	स्थिरांक अक्षर *opt_window_memory = रिक्त;
	स्थिरांक अक्षर *opt_depth = रिक्त;
	स्थिरांक अक्षर *opt_threads = रिक्त;
	अचिन्हित दीर्घ combine_cruft_below_size = 0ul;

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
			     &combine_cruft_below_size,
			     N_("with --cruft, only repack cruft packs smaller than this")),
		OPT_UNSIGNED(0, "max-cruft-size", &cruft_po_args.max_pack_size,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL('d', रिक्त, &delete_redundant,
				N_("remove redundant packs, and run git-prune-packed")),
		OPT_BOOL('f', रिक्त, &po_args.no_reuse_delta,
				N_("pass --no-reuse-delta to git-pack-objects")),
		OPT_BOOL('F', रिक्त, &po_args.no_reuse_object,
				N_("pass --no-reuse-object to git-pack-objects")),
		OPT_INTEGER(0, "name-hash-version", &po_args.name_hash_version,
				N_("specify the name hash version to use for grouping similar objects by path")),
		OPT_BOOL(0, "path-walk", &po_args.path_walk,
				N_("pass --path-walk to git-pack-objects")),
		OPT_NEGBIT('n', रिक्त, &run_update_server_info,
				N_("do not run git-update-server-info"), 1),
		OPT__QUIET(&po_args.quiet, N_("be quiet")),
		OPT_BOOL('l', "local", &po_args.local,
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
		OPT_SET_INT_F('m', रिक्त, &write_midx,
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

	स_रखो(&config_ctx, 0, आकार(config_ctx));
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

	जर (dry_run && !drop_filtered)
		die(_("--dry-run only takes effect with --drop-filtered"));

	जर (drop_filtered) {
		जर (!po_args.filter_options.choice)
			die(_("--drop-filtered requires --filter"));

		जर (!(pack_everything & ALL_INTO_ONE))
			die(_("--drop-filtered requires -a"));

		/*
		 * Only blob:limit=<n> is supported for now. Reject other
		 * filter choices early, before walking the object database.
		 */
		जर (po_args.filter_options.choice != LOFC_BLOB_LIMIT)
			die(_("--drop-filtered only supports --filter=blob:limit=<n> for now"));

		/*
		 * An explicit -b on the command line is a conflict we have to
		 * report; a bitmap setting from config is silently overridden
		 * for the duration of the command.
		 */
		जर (write_bitmaps_given && write_bitmaps > 0)
			die(_("options '%s' and '%s' cannot be used together"),
				"--drop-filtered", "--write-bitmap-index");

		/*
		 * Without a promisor remote there is nowhere to re-fetch the
		 * dropped objects from, so dropping them would be permanent
		 * data loss.
		 */
		जर (!repo_has_promisor_remote(repo))
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
		जर (!is_bare_repository(repo)) {
			संरचना wt_status_state state = { 0 };

			wt_status_get_state(repo, &state, 0);
			जर (state.merge_in_progress || state.revert_in_progress ||
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
		जर (!dry_run)
			delete_redundant = 1;

		ret = enumerate_promisor_blobs(repo, &po_args.filter_options, &drop_oids);

		जर (ret)
			जा cleanup;

		/*
		 * Refuse to drop blobs that the current index references.
		 * Such a blob would only be lazily re-fetched by the next
		 * command that touches the worktree, so dropping it reclaims
		 * nothing. This guard just avoids that churn. Bare
		 * repositories have no index, so the check is skipped there.
		 */
		जर (!is_bare_repository(repo) && oidset_size(&drop_oids)) {
			संरचना index_state *istate = repo->index;
			अचिन्हित पूर्णांक i;

			जर (repo_read_index(repo) < 0)
				die(_("could not read the index"));

			साठी (i = 0; i < istate->cache_nr; i++) {
				स्थिरांक संरचना cache_entry *ce = istate->cache[i];

				जर (oidset_contains(&drop_oids, &ce->oid))
					die(_("cannot drop '%s' (%s): it is referenced by the current index"),
						ce->name, oid_to_hex(&ce->oid));
			}
		}

		जर (dry_run) {
			संरचना oidset_iter iter;
			स्थिरांक संरचना object_id *oid;

			oidset_iter_init(&drop_oids, &iter);
			पर्यंत ((oid = oidset_iter_next(&iter)))
				म_लिखो("%s\n", oid_to_hex(oid));
		}
	}

	जर (delete_redundant && repo->repository_format_precious_objects)
		die(_("cannot delete packs in a precious-objects repo"));

	die_for_incompatible_opt3(unpack_unreachable || (pack_everything & LOOSEN_UNREACHABLE), "-A",
				  keep_unreachable, "-k/--keep-unreachable",
				  pack_everything & PACK_CRUFT, "--cruft");

	जर (pack_everything & PACK_CRUFT)
		pack_everything |= ALL_INTO_ONE;

	जर (write_bitmaps < 0) {
		जर (write_midx == REPACK_WRITE_MIDX_NONE &&
		    (!(pack_everything & ALL_INTO_ONE) || !is_bare_repository(repo)))
			write_bitmaps = 0;
	}
	जर (po_args.pack_kept_objects < 0)
		po_args.pack_kept_objects = write_bitmaps > 0 &&
			write_midx == REPACK_WRITE_MIDX_NONE;

	जर (write_bitmaps && !(pack_everything & ALL_INTO_ONE) &&
	    write_midx == REPACK_WRITE_MIDX_NONE)
		die(_(incremental_bitmap_conflict_error));

	जर (write_bitmaps && po_args.local &&
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

	जर (config_ctx.midx_split_factor < 2)
		die(_("invalid value for %s: %d"), "--midx-split-factor",
		    config_ctx.midx_split_factor);
	जर (config_ctx.midx_new_layer_threshold < 1)
		die(_("invalid value for %s: %d"), "--midx-new-layer-threshold",
		    config_ctx.midx_new_layer_threshold);

	जर (write_midx != REPACK_WRITE_MIDX_NONE && write_bitmaps) {
		संरचना strbuf path = STRBUF_INIT;

		strbuf_addf(&path, "%s/%s_XXXXXX",
			    repo_get_object_directory(repo),
			    "bitmap-ref-tips");

		refs_snapshot = xmks_tempfile(path.buf);
		midx_snapshot_refs(repo, refs_snapshot);

		strbuf_release(&path);
	}

	packdir = mkpathdup("%s/pack", repo_get_object_directory(repo));
	packtmp_name = xstrfmt(".tmp-%d-pack", (पूर्णांक)getpid());
	packtmp = mkpathdup("%s/%s", packdir, packtmp_name);

	existing.repo = repo;
	existing_packs_collect(&existing, &keep_pack_list);

	जर (geometry.split_factor) {
		जर (pack_everything)
			die(_("options '%s' and '%s' cannot be used together"), "--geometric", "-A/-a");
		जर (write_midx == REPACK_WRITE_MIDX_INCREMENTAL) {
			geometry.midx_layer_threshold = config_ctx.midx_new_layer_threshold;
			geometry.midx_layer_threshold_set = true;
		}
		pack_geometry_init(&geometry, &existing, &po_args);
		pack_geometry_split(&geometry);
	}

	prepare_pack_objects(&cmd, &po_args, packtmp);

	show_progress = !po_args.quiet && isatty(2);

	strvec_ढकला(&cmd.args, "--keep-true-parents");
	साठी (i = 0; i < keep_pack_list.nr; i++)
		strvec_pushf(&cmd.args, "--keep-pack=%s",
			     keep_pack_list.वस्तुसंच[i].string);
	strvec_ढकला(&cmd.args, "--non-empty");
	जर (!geometry.split_factor) {
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
		strvec_ढकला(&cmd.args, "--all");
		strvec_ढकला(&cmd.args, "--reflog");
		strvec_ढकला(&cmd.args, "--indexed-objects");
	}
	जर (repo_has_promisor_remote(repo))
		strvec_ढकला(&cmd.args, "--exclude-promisor-objects");
	जर (write_midx == REPACK_WRITE_MIDX_NONE) {
		जर (write_bitmaps > 0)
			strvec_ढकला(&cmd.args, "--write-bitmap-index");
		अन्यथा जर (write_bitmaps < 0)
			strvec_ढकला(&cmd.args, "--write-bitmap-index-quiet");
	}
	जर (use_delta_islands)
		strvec_ढकला(&cmd.args, "--delta-islands");

	जर (pack_everything & ALL_INTO_ONE) {
		repack_promisor_objects(repo, &po_args, &names, packtmp,
			(drop_filtered && !dry_run) ? &drop_oids : रिक्त);

		जर (existing_packs_has_non_kept(&existing) &&
		    delete_redundant &&
		    !(pack_everything & PACK_CRUFT)) {
			for_each_string_list_item(item, &names) {
				strvec_pushf(&cmd.args, "--keep-pack=%s-%s.pack",
					     packtmp_name, item->string);
			}
			जर (unpack_unreachable) {
				strvec_pushf(&cmd.args,
					     "--unpack-unreachable=%s",
					     unpack_unreachable);
			} अन्यथा जर (pack_everything & LOOSEN_UNREACHABLE) {
				strvec_ढकला(&cmd.args,
					    "--unpack-unreachable");
			} अन्यथा जर (keep_unreachable) {
				strvec_ढकला(&cmd.args, "--keep-unreachable");
			}
		}

		जर (keep_unreachable && delete_redundant &&
		    !(pack_everything & PACK_CRUFT))
			strvec_ढकला(&cmd.args, "--pack-loose-unreachable");
	} अन्यथा जर (geometry.split_factor) {
		pack_geometry_repack_promisors(repo, &po_args, &geometry,
					       &names, packtmp);

		जर (midx_must_contain_cruft)
			strvec_ढकला(&cmd.args, "--stdin-packs");
		अन्यथा
			strvec_ढकला(&cmd.args, "--stdin-packs=follow");
		strvec_ढकला(&cmd.args, "--unpacked");
	} अन्यथा {
		strvec_ढकला(&cmd.args, "--unpacked");
		strvec_ढकला(&cmd.args, "--incremental");
	}

	जर (po_args.filter_options.choice)
		strvec_pushf(&cmd.args, "--filter=%s",
			     expand_list_objects_filter_spec(&po_args.filter_options));
	अन्यथा जर (filter_to)
		die(_("option '%s' can only be used along with '%s'"), "--filter-to", "--filter");

	जर (geometry.split_factor)
		cmd.in = -1;
	अन्यथा
		cmd.no_stdin = 1;

	ret = start_command(&cmd);
	जर (ret)
		जा cleanup;

	जर (geometry.split_factor) {
		खाता *in = xfdopen(cmd.in, "w");
		/*
		 * The resulting pack should contain all objects in packs that
		 * are going to be rolled up, but exclude objects in packs which
		 * are being left alone.
		 */
		साठी (i = 0; i < geometry.split; i++)
			ख_लिखो(in, "%s\n", pack_basename(geometry.pack[i]));
		साठी (i = geometry.split; i < geometry.pack_nr; i++) {
			स्थिरांक अक्षर *basename = pack_basename(geometry.pack[i]);
			अक्षर marker = '^';

			जर (!midx_must_contain_cruft &&
			    !string_list_has_string(&existing.midx_packs,
						    basename)) {
				/*
				 * Assume non-MIDX'd packs are not
				 * necessarily closed under
				 * reachability.
				 */
				marker = '!';
			}

			ख_लिखो(in, "%c%s\n", marker, basename);
		}
		ख_बंद(in);
	}

	{
		संरचना write_pack_opts opts = {
			.packdir = packdir,
			.destination = packdir,
			.packtmp = packtmp,
		};
		ret = finish_pack_objects_cmd(repo->hash_algo, &opts, &cmd,
					      &names);
		जर (ret)
			जा cleanup;
	}

	जर (!names.nr) {
		संरचना odb_source_files *files = odb_source_files_downcast(existing.source);

		जर (!po_args.quiet)
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
		जर (!get_multi_pack_index(files->dirs->packed))
			midx_must_contain_cruft = 1;
	}

	जर (pack_everything & PACK_CRUFT) {
		संरचना write_pack_opts opts = {
			.po_args = &cruft_po_args,
			.destination = packtmp,
			.packtmp = packtmp,
			.packdir = packdir,
		};

		जर (!cruft_po_args.window)
			cruft_po_args.window = xstrdup_or_null(po_args.window);
		जर (!cruft_po_args.window_memory)
			cruft_po_args.window_memory = xstrdup_or_null(po_args.window_memory);
		जर (!cruft_po_args.depth)
			cruft_po_args.depth = xstrdup_or_null(po_args.depth);
		जर (!cruft_po_args.threads)
			cruft_po_args.threads = xstrdup_or_null(po_args.threads);
		जर (!cruft_po_args.max_pack_size)
			cruft_po_args.max_pack_size = po_args.max_pack_size;

		cruft_po_args.local = po_args.local;
		cruft_po_args.quiet = po_args.quiet;
		cruft_po_args.delta_base_offset = po_args.delta_base_offset;
		cruft_po_args.pack_kept_objects = 0;

		ret = write_cruft_pack(&opts, cruft_expiration,
				       combine_cruft_below_size, &names,
				       &existing);
		जर (ret)
			जा cleanup;

		जर (delete_redundant && expire_to) {
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
			ret = write_cruft_pack(&opts, रिक्त, 0ul, &names,
					       &existing);
			जर (ret)
				जा cleanup;
		}
	}

	जर (po_args.filter_options.choice && !drop_filtered) {
		संरचना write_pack_opts opts = {
			.po_args = &po_args,
			.destination = filter_to,
			.packdir = packdir,
			.packtmp = packtmp,
		};

		जर (!opts.destination)
			opts.destination = packtmp;

		ret = write_filtered_pack(&opts, &existing, &names);
		जर (ret)
			जा cleanup;
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

	जर (delete_redundant && pack_everything & ALL_INTO_ONE) {
		जर (write_midx == REPACK_WRITE_MIDX_INCREMENTAL)
			existing_packs_retain_midx_packs(&existing);
		existing_packs_mark_for_deletion(&existing, &names);
	}

	जर (write_midx != REPACK_WRITE_MIDX_NONE) {
		संरचना repack_write_midx_opts opts = {
			.existing = &existing,
			.geometry = &geometry,
			.names = &names,
			.refs_snapshot = refs_snapshot ? get_tempfile_path(refs_snapshot) : रिक्त,
			.packdir = packdir,
			.show_progress = show_progress,
			.write_bitmaps = write_bitmaps > 0,
			.midx_must_contain_cruft = midx_must_contain_cruft,
			.midx_split_factor = config_ctx.midx_split_factor,
			.midx_new_layer_threshold = config_ctx.midx_new_layer_threshold,
			.mode = write_midx,
		};

		ret = repack_write_midx(&opts);
		जर (ret)
			जा cleanup;
	}

	odb_reprepare(repo->objects);

	जर (delete_redundant) {
		पूर्णांक opts = 0;
		bool wrote_incremental_midx = write_midx == REPACK_WRITE_MIDX_INCREMENTAL;

		existing_packs_remove_redundant(&existing, packdir,
						wrote_incremental_midx);

		जर (geometry.split_factor)
			pack_geometry_remove_redundant(&geometry, &names,
						       &existing, packdir,
						       wrote_incremental_midx);
		जर (show_progress)
			opts |= PRUNE_PACKED_VERBOSE;
		prune_packed_objects(opts);

		जर (!keep_unreachable &&
		    (!(pack_everything & LOOSEN_UNREACHABLE) ||
		     unpack_unreachable) &&
		    is_repository_shallow(repo))
			prune_shallow(PRUNE_QUICK);
	}

	जर (run_update_server_info)
		update_server_info(repo, 0);

	जर (git_env_bool(GIT_TEST_MULTI_PACK_INDEX, 0)) {
		संरचना odb_source_files *files = odb_source_files_downcast(existing.source);
		अचिन्हित flags = 0;

		जर (git_env_bool(GIT_TEST_MULTI_PACK_INDEX_WRITE_INCREMENTAL, 0))
			flags |= MIDX_WRITE_INCREMENTAL;
		write_midx_file(files->dirs->packed, रिक्त, रिक्त, flags);
	}

cleanup:
	string_list_clear(&keep_pack_list, 0);
	string_list_clear(&names, 1);
	oidset_clear(&drop_oids);
	existing_packs_release(&existing);
	pack_geometry_release(&geometry);
	pack_objects_args_release(&po_args);
	pack_objects_args_release(&cruft_po_args);

	परत ret;
}
