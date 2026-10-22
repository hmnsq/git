#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "server-info.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "midx.h"
#অন্তর্ভাব "packfile.h"
#অন্তর্ভাব "prune-packed.h"
#অন্তর্ভাব "promisor-remote.h"
#অন্তর্ভাব "repack.h"
#অন্তর্ভাব "shallow.h"
#অন্তর্ভাব "list-objects-filter-options.h"
#অন্তর্ভাব "oidset.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "wt-status.h"
#অন্তর্ভাব "read-cache-ll.h"

#define ALL_INTO_ONE 1
#define LOOSEN_UNREACHABLE 2
#define PACK_CRUFT 4

#define DELETE_PACK 1
#define RETAIN_PACK 2

স্থির পূর্ণসংখ্যা pack_everything;
স্থির পূর্ণসংখ্যা write_bitmaps = -1;
স্থির পূর্ণসংখ্যা use_delta_islands;
স্থির পূর্ণসংখ্যা run_update_server_info = 1;
স্থির অক্ষর *packdir, *packtmp_name, *packtmp;
স্থির পূর্ণসংখ্যা midx_must_contain_cruft = 1;
স্থির পূর্ণসংখ্যা drop_filtered;
স্থির পূর্ণসংখ্যা dry_run;
স্থির পূর্ণসংখ্যা write_bitmaps_given;

স্থির ধ্রুবক অক্ষর *ধ্রুবক git_repack_usage[] = {
	N_("git repack [-a] [-A] [-d] [-f] [-F] [-l] [-n] [-q] [-b] [-m]\n"
	   "[--window=<n>] [--depth=<n>] [--threads=<n>] [--keep-pack=<pack-name>]\n"
	   "[--write-midx[=<mode>]] [--name-hash-version=<n>] [--path-walk]\n"
	   "[--filter=<filter-spec>] [--drop-filtered [--dry-run]]"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর incremental_bitmap_conflict_error[] = N_(
"Incremental repacks are incompatible with bitmap indexes.  Use\n"
"--no-write-bitmap-index or disable the pack.writeBitmaps configuration."
);

#define DEFAULT_MIDX_SPLIT_FACTOR 2
#define DEFAULT_MIDX_NEW_LAYER_THRESHOLD 8

কাঠামো repack_config_ctx {
	কাঠামো pack_objects_args *po_args;
	কাঠামো pack_objects_args *cruft_po_args;
	পূর্ণসংখ্যা midx_split_factor;
	পূর্ণসংখ্যা midx_new_layer_threshold;
};

স্থির পূর্ণসংখ্যা repack_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			 ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	কাঠামো repack_config_ctx *repack_ctx = cb;
	কাঠামো pack_objects_args *po_args = repack_ctx->po_args;
	কাঠামো pack_objects_args *cruft_po_args = repack_ctx->cruft_po_args;
	যদি (!strcmp(var, "repack.usedeltabaseoffset")) {
		po_args->delta_base_offset = git_config_bool(var, মান);
		ফেরত 0;
	}
	যদি (!strcmp(var, "repack.packkeptobjects")) {
		po_args->pack_kept_objects = git_config_bool(var, মান);
		ফেরত 0;
	}
	যদি (!strcmp(var, "repack.writebitmaps") ||
	    !strcmp(var, "pack.writebitmaps")) {
		write_bitmaps = git_config_bool(var, মান);
		ফেরত 0;
	}
	যদি (!strcmp(var, "repack.usedeltaislands")) {
		use_delta_islands = git_config_bool(var, মান);
		ফেরত 0;
	}
	যদি (strcmp(var, "repack.updateserverinfo") == 0) {
		run_update_server_info = git_config_bool(var, মান);
		ফেরত 0;
	}
	যদি (!strcmp(var, "repack.cruftwindow")) {
		মুক্ত(cruft_po_args->window);
		ফেরত git_config_string(&cruft_po_args->window, var, মান);
	}
	যদি (!strcmp(var, "repack.cruftwindowmemory")) {
		মুক্ত(cruft_po_args->window_memory);
		ফেরত git_config_string(&cruft_po_args->window_memory, var, মান);
	}
	যদি (!strcmp(var, "repack.cruftdepth")) {
		মুক্ত(cruft_po_args->depth);
		ফেরত git_config_string(&cruft_po_args->depth, var, মান);
	}
	যদি (!strcmp(var, "repack.cruftthreads")) {
		মুক্ত(cruft_po_args->threads);
		ফেরত git_config_string(&cruft_po_args->threads, var, মান);
	}
	যদি (!strcmp(var, "repack.midxmustcontaincruft")) {
		midx_must_contain_cruft = git_config_bool(var, মান);
		ফেরত 0;
	}
	যদি (!strcmp(var, "repack.midxsplitfactor")) {
		repack_ctx->midx_split_factor = git_config_int(var, মান,
							       ctx->kvi);
		ফেরত 0;
	}
	যদি (!strcmp(var, "repack.midxnewlayerthreshold")) {
		repack_ctx->midx_new_layer_threshold = git_config_int(var, মান,
								      ctx->kvi);
		ফেরত 0;
	}
	ফেরত git_default_config(var, মান, ctx, cb);
}

স্থির পূর্ণসংখ্যা option_parse_write_bitmaps(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
				      পূর্ণসংখ্যা unset)
{
	পূর্ণসংখ্যা *মান = opt->মান;

	BUG_ON_OPT_ARG(arg);
	যদি (unset)
		*মান = 0;
	অন্যথা
		*মান = 1;

	write_bitmaps_given = 1;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_write_midx(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
				   পূর্ণসংখ্যা unset)
{
	গণনা repack_write_midx_mode *cfg = opt->মান;

	যদি (unset) {
		*cfg = REPACK_WRITE_MIDX_NONE;
		ফেরত 0;
	}

	যদি (!arg || !*arg)
		*cfg = REPACK_WRITE_MIDX_DEFAULT;
	অন্যথা যদি (!strcmp(arg, "incremental"))
		*cfg = REPACK_WRITE_MIDX_INCREMENTAL;
	অন্যথা
		ফেরত error(_("unknown value for %s: %s"), opt->long_name, arg);

	ফেরত 0;
}

পূর্ণসংখ্যা cmd_repack(পূর্ণসংখ্যা argc,
	       ধ্রুবক অক্ষর **argv,
	       ধ্রুবক অক্ষর *prefix,
	       কাঠামো repository *repo)
{
	কাঠামো child_process cmd = CHILD_PROCESS_INIT;
	কাঠামো string_list_item *item;
	কাঠামো string_list names = STRING_LIST_INIT_DUP;
	কাঠামো existing_packs existing = EXISTING_PACKS_INIT;
	কাঠামো oidset drop_oids = OIDSET_INIT;
	কাঠামো pack_geometry geometry = { 0 };
	কাঠামো tempfile *refs_snapshot = ফাঁকা;
	পূর্ণসংখ্যা i, ret;
	পূর্ণসংখ্যা show_progress;

	/* variables to be filled by option parsing */
	কাঠামো repack_config_ctx config_ctx;
	পূর্ণসংখ্যা delete_redundant = 0;
	ধ্রুবক অক্ষর *unpack_unreachable = ফাঁকা;
	পূর্ণসংখ্যা keep_unreachable = 0;
	কাঠামো string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	কাঠামো pack_objects_args po_args = PACK_OBJECTS_ARGS_INIT;
	কাঠামো pack_objects_args cruft_po_args = PACK_OBJECTS_ARGS_INIT;
	গণনা repack_write_midx_mode write_midx = REPACK_WRITE_MIDX_NONE;
	ধ্রুবক অক্ষর *cruft_expiration = ফাঁকা;
	ধ্রুবক অক্ষর *expire_to = ফাঁকা;
	ধ্রুবক অক্ষর *filter_to = ফাঁকা;
	ধ্রুবক অক্ষর *opt_window = ফাঁকা;
	ধ্রুবক অক্ষর *opt_window_memory = ফাঁকা;
	ধ্রুবক অক্ষর *opt_depth = ফাঁকা;
	ধ্রুবক অক্ষর *opt_threads = ফাঁকা;
	unsigned long combine_cruft_below_আকার = 0ul;

	কাঠামো option builtin_repack_options[] = {
		OPT_BIT('a', ফাঁকা, &pack_everything,
				N_("pack everything in a single pack"), ALL_INTO_ONE),
		OPT_BIT('A', ফাঁকা, &pack_everything,
				N_("same as -a, and turn unreachable objects loose"),
				   LOOSEN_UNREACHABLE | ALL_INTO_ONE),
		OPT_BIT(0, "cruft", &pack_everything,
				N_("same as -a, pack unreachable cruft objects separately"),
				   PACK_CRUFT),
		OPT_STRING(0, "cruft-expiration", &cruft_expiration, N_("approxidate"),
				N_("with --cruft, expire objects older than this")),
		OPT_UNSIGNED(0, "combine-cruft-below-size",
			     &combine_cruft_below_আকার,
			     N_("with --cruft, only repack cruft packs smaller than this")),
		OPT_UNSIGNED(0, "max-cruft-size", &cruft_po_args.max_pack_আকার,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL('d', ফাঁকা, &delete_redundant,
				N_("remove redundant packs, and run git-prune-packed")),
		OPT_BOOL('f', ফাঁকা, &po_args.no_reuse_delta,
				N_("pass --no-reuse-delta to git-pack-objects")),
		OPT_BOOL('F', ফাঁকা, &po_args.no_reuse_object,
				N_("pass --no-reuse-object to git-pack-objects")),
		OPT_INTEGER(0, "name-hash-version", &po_args.name_hash_version,
				N_("specify the name hash version to use for grouping similar objects by path")),
		OPT_BOOL(0, "path-walk", &po_args.path_walk,
				N_("pass --path-walk to git-pack-objects")),
		OPT_NEGBIT('n', ফাঁকা, &run_update_server_info,
				N_("do not run git-update-server-info"), 1),
		OPT__QUIET(&po_args.quiet, N_("be quiet")),
		OPT_BOOL('l', "local", &po_args.local,
				N_("pass --local to git-pack-objects")),
		OPT_CALLBACK_F('b', "write-bitmap-index", &write_bitmaps, ফাঁকা,
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
		OPT_UNSIGNED(0, "max-pack-size", &po_args.max_pack_আকার,
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
		OPT_SET_INT_F('m', ফাঁকা, &write_midx,
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

	memset(&config_ctx, 0, আকার(config_ctx));
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

	যদি (dry_run && !drop_filtered)
		die(_("--dry-run only takes effect with --drop-filtered"));

	যদি (drop_filtered) {
		যদি (!po_args.filter_options.choice)
			die(_("--drop-filtered requires --filter"));

		যদি (!(pack_everything & ALL_INTO_ONE))
			die(_("--drop-filtered requires -a"));

		/*
		 * Only blob:limit=<n> is supported for now. Reject other
		 * filter choices early, before walking the object database.
		 */
		যদি (po_args.filter_options.choice != LOFC_BLOB_LIMIT)
			die(_("--drop-filtered only supports --filter=blob:limit=<n> for now"));

		/*
		 * An explicit -b on the command line is a conflict we have to
		 * report; a bitmap setting from config is silently overridden
		 * for the duration of the command.
		 */
		যদি (write_bitmaps_given && write_bitmaps > 0)
			die(_("options '%s' and '%s' cannot be used together"),
				"--drop-filtered", "--write-bitmap-index");

		/*
		 * Without a promisor remote there is nowhere to re-fetch the
		 * dropped objects from, so dropping them would be permanent
		 * data loss.
		 */
		যদি (!repo_has_promisor_remote(repo))
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
		যদি (!is_bare_repository(repo)) {
			কাঠামো wt_status_state state = { 0 };

			wt_status_get_state(repo, &state, 0);
			যদি (state.merge_in_progress || state.revert_in_progress ||
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
		যদি (!dry_run)
			delete_redundant = 1;

		ret = enumerate_promisor_blobs(repo, &po_args.filter_options, &drop_oids);

		যদি (ret)
			যাও cleanup;

		/*
		 * Refuse to drop blobs that the current index references.
		 * Such a blob would only be lazily re-fetched by the next
		 * command that touches the worktree, so dropping it reclaims
		 * nothing. This guard just avoids that churn. Bare
		 * repositories have no index, so the check is skipped there.
		 */
		যদি (!is_bare_repository(repo) && oidset_আকার(&drop_oids)) {
			কাঠামো সূচক_state *istate = repo->সূচক;
			unsigned পূর্ণসংখ্যা i;

			যদি (repo_read_সূচক(repo) < 0)
				die(_("could not read the index"));

			জন্য (i = 0; i < istate->cache_nr; i++) {
				ধ্রুবক কাঠামো cache_entry *ce = istate->cache[i];

				যদি (oidset_contains(&drop_oids, &ce->oid))
					die(_("cannot drop '%s' (%s): it is referenced by the current index"),
						ce->name, oid_to_hex(&ce->oid));
			}
		}

		যদি (dry_run) {
			কাঠামো oidset_iter iter;
			ধ্রুবক কাঠামো object_id *oid;

			oidset_iter_init(&drop_oids, &iter);
			যখন ((oid = oidset_iter_পরবর্তী(&iter)))
				printf("%s\n", oid_to_hex(oid));
		}
	}

	যদি (delete_redundant && repo->repository_format_precious_objects)
		die(_("cannot delete packs in a precious-objects repo"));

	die_for_incompatible_opt3(unpack_unreachable || (pack_everything & LOOSEN_UNREACHABLE), "-A",
				  keep_unreachable, "-k/--keep-unreachable",
				  pack_everything & PACK_CRUFT, "--cruft");

	যদি (pack_everything & PACK_CRUFT)
		pack_everything |= ALL_INTO_ONE;

	যদি (write_bitmaps < 0) {
		যদি (write_midx == REPACK_WRITE_MIDX_NONE &&
		    (!(pack_everything & ALL_INTO_ONE) || !is_bare_repository(repo)))
			write_bitmaps = 0;
	}
	যদি (po_args.pack_kept_objects < 0)
		po_args.pack_kept_objects = write_bitmaps > 0 &&
			write_midx == REPACK_WRITE_MIDX_NONE;

	যদি (write_bitmaps && !(pack_everything & ALL_INTO_ONE) &&
	    write_midx == REPACK_WRITE_MIDX_NONE)
		die(_(incremental_bitmap_conflict_error));

	যদি (write_bitmaps && po_args.local &&
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

	যদি (config_ctx.midx_split_factor < 2)
		die(_("invalid value for %s: %d"), "--midx-split-factor",
		    config_ctx.midx_split_factor);
	যদি (config_ctx.midx_new_layer_threshold < 1)
		die(_("invalid value for %s: %d"), "--midx-new-layer-threshold",
		    config_ctx.midx_new_layer_threshold);

	যদি (write_midx != REPACK_WRITE_MIDX_NONE && write_bitmaps) {
		কাঠামো strbuf path = STRBUF_INIT;

		strbuf_addf(&path, "%s/%s_XXXXXX",
			    repo_get_object_directory(repo),
			    "bitmap-ref-tips");

		refs_snapshot = xmks_tempfile(path.buf);
		midx_snapshot_refs(repo, refs_snapshot);

		strbuf_release(&path);
	}

	packdir = mkpathdup("%s/pack", repo_get_object_directory(repo));
	packtmp_name = xstrfmt(".tmp-%d-pack", (পূর্ণসংখ্যা)getpid());
	packtmp = mkpathdup("%s/%s", packdir, packtmp_name);

	existing.repo = repo;
	existing_packs_collect(&existing, &keep_pack_list);

	যদি (geometry.split_factor) {
		যদি (pack_everything)
			die(_("options '%s' and '%s' cannot be used together"), "--geometric", "-A/-a");
		যদি (write_midx == REPACK_WRITE_MIDX_INCREMENTAL) {
			geometry.midx_layer_threshold = config_ctx.midx_new_layer_threshold;
			geometry.midx_layer_threshold_set = true;
		}
		pack_geometry_init(&geometry, &existing, &po_args);
		pack_geometry_split(&geometry);
	}

	prepare_pack_objects(&cmd, &po_args, packtmp);

	show_progress = !po_args.quiet && isatty(2);

	strvec_ঠেলো(&cmd.args, "--keep-true-parents");
	জন্য (i = 0; i < keep_pack_list.nr; i++)
		strvec_pushf(&cmd.args, "--keep-pack=%s",
			     keep_pack_list.বস্তুসমূহ[i].string);
	strvec_ঠেলো(&cmd.args, "--non-empty");
	যদি (!geometry.split_factor) {
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
		strvec_ঠেলো(&cmd.args, "--all");
		strvec_ঠেলো(&cmd.args, "--reflog");
		strvec_ঠেলো(&cmd.args, "--indexed-objects");
	}
	যদি (repo_has_promisor_remote(repo))
		strvec_ঠেলো(&cmd.args, "--exclude-promisor-objects");
	যদি (write_midx == REPACK_WRITE_MIDX_NONE) {
		যদি (write_bitmaps > 0)
			strvec_ঠেলো(&cmd.args, "--write-bitmap-index");
		অন্যথা যদি (write_bitmaps < 0)
			strvec_ঠেলো(&cmd.args, "--write-bitmap-index-quiet");
	}
	যদি (use_delta_islands)
		strvec_ঠেলো(&cmd.args, "--delta-islands");

	যদি (pack_everything & ALL_INTO_ONE) {
		repack_promisor_objects(repo, &po_args, &names, packtmp,
			(drop_filtered && !dry_run) ? &drop_oids : ফাঁকা);

		যদি (existing_packs_has_non_kept(&existing) &&
		    delete_redundant &&
		    !(pack_everything & PACK_CRUFT)) {
			for_each_string_list_item(item, &names) {
				strvec_pushf(&cmd.args, "--keep-pack=%s-%s.pack",
					     packtmp_name, item->string);
			}
			যদি (unpack_unreachable) {
				strvec_pushf(&cmd.args,
					     "--unpack-unreachable=%s",
					     unpack_unreachable);
			} অন্যথা যদি (pack_everything & LOOSEN_UNREACHABLE) {
				strvec_ঠেলো(&cmd.args,
					    "--unpack-unreachable");
			} অন্যথা যদি (keep_unreachable) {
				strvec_ঠেলো(&cmd.args, "--keep-unreachable");
			}
		}

		যদি (keep_unreachable && delete_redundant &&
		    !(pack_everything & PACK_CRUFT))
			strvec_ঠেলো(&cmd.args, "--pack-loose-unreachable");
	} অন্যথা যদি (geometry.split_factor) {
		pack_geometry_repack_promisors(repo, &po_args, &geometry,
					       &names, packtmp);

		যদি (midx_must_contain_cruft)
			strvec_ঠেলো(&cmd.args, "--stdin-packs");
		অন্যথা
			strvec_ঠেলো(&cmd.args, "--stdin-packs=follow");
		strvec_ঠেলো(&cmd.args, "--unpacked");
	} অন্যথা {
		strvec_ঠেলো(&cmd.args, "--unpacked");
		strvec_ঠেলো(&cmd.args, "--incremental");
	}

	যদি (po_args.filter_options.choice)
		strvec_pushf(&cmd.args, "--filter=%s",
			     expand_list_objects_filter_spec(&po_args.filter_options));
	অন্যথা যদি (filter_to)
		die(_("option '%s' can only be used along with '%s'"), "--filter-to", "--filter");

	যদি (geometry.split_factor)
		cmd.in = -1;
	অন্যথা
		cmd.no_stdin = 1;

	ret = start_command(&cmd);
	যদি (ret)
		যাও cleanup;

	যদি (geometry.split_factor) {
		FILE *in = xfdopen(cmd.in, "w");
		/*
		 * The resulting pack should contain all objects in packs that
		 * are going to be rolled up, but exclude objects in packs which
		 * are being left alone.
		 */
		জন্য (i = 0; i < geometry.split; i++)
			fprintf(in, "%s\n", pack_basename(geometry.pack[i]));
		জন্য (i = geometry.split; i < geometry.pack_nr; i++) {
			ধ্রুবক অক্ষর *basename = pack_basename(geometry.pack[i]);
			অক্ষর marker = '^';

			যদি (!midx_must_contain_cruft &&
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
		কাঠামো write_pack_opts opts = {
			.packdir = packdir,
			.destination = packdir,
			.packtmp = packtmp,
		};
		ret = finish_pack_objects_cmd(repo->hash_algo, &opts, &cmd,
					      &names);
		যদি (ret)
			যাও cleanup;
	}

	যদি (!names.nr) {
		কাঠামো odb_source_files *files = odb_source_files_downcast(existing.source);

		যদি (!po_args.quiet)
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
		যদি (!get_multi_pack_সূচক(files->dirs->packed))
			midx_must_contain_cruft = 1;
	}

	যদি (pack_everything & PACK_CRUFT) {
		কাঠামো write_pack_opts opts = {
			.po_args = &cruft_po_args,
			.destination = packtmp,
			.packtmp = packtmp,
			.packdir = packdir,
		};

		যদি (!cruft_po_args.window)
			cruft_po_args.window = xstrdup_or_null(po_args.window);
		যদি (!cruft_po_args.window_memory)
			cruft_po_args.window_memory = xstrdup_or_null(po_args.window_memory);
		যদি (!cruft_po_args.depth)
			cruft_po_args.depth = xstrdup_or_null(po_args.depth);
		যদি (!cruft_po_args.threads)
			cruft_po_args.threads = xstrdup_or_null(po_args.threads);
		যদি (!cruft_po_args.max_pack_আকার)
			cruft_po_args.max_pack_আকার = po_args.max_pack_আকার;

		cruft_po_args.local = po_args.local;
		cruft_po_args.quiet = po_args.quiet;
		cruft_po_args.delta_base_offset = po_args.delta_base_offset;
		cruft_po_args.pack_kept_objects = 0;

		ret = write_cruft_pack(&opts, cruft_expiration,
				       combine_cruft_below_আকার, &names,
				       &existing);
		যদি (ret)
			যাও cleanup;

		যদি (delete_redundant && expire_to) {
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
			ret = write_cruft_pack(&opts, ফাঁকা, 0ul, &names,
					       &existing);
			যদি (ret)
				যাও cleanup;
		}
	}

	যদি (po_args.filter_options.choice && !drop_filtered) {
		কাঠামো write_pack_opts opts = {
			.po_args = &po_args,
			.destination = filter_to,
			.packdir = packdir,
			.packtmp = packtmp,
		};

		যদি (!opts.destination)
			opts.destination = packtmp;

		ret = write_filtered_pack(&opts, &existing, &names);
		যদি (ret)
			যাও cleanup;
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

	যদি (delete_redundant && pack_everything & ALL_INTO_ONE) {
		যদি (write_midx == REPACK_WRITE_MIDX_INCREMENTAL)
			existing_packs_retain_midx_packs(&existing);
		existing_packs_mark_for_deletion(&existing, &names);
	}

	যদি (write_midx != REPACK_WRITE_MIDX_NONE) {
		কাঠামো repack_write_midx_opts opts = {
			.existing = &existing,
			.geometry = &geometry,
			.names = &names,
			.refs_snapshot = refs_snapshot ? get_tempfile_path(refs_snapshot) : ফাঁকা,
			.packdir = packdir,
			.show_progress = show_progress,
			.write_bitmaps = write_bitmaps > 0,
			.midx_must_contain_cruft = midx_must_contain_cruft,
			.midx_split_factor = config_ctx.midx_split_factor,
			.midx_new_layer_threshold = config_ctx.midx_new_layer_threshold,
			.mode = write_midx,
		};

		ret = repack_write_midx(&opts);
		যদি (ret)
			যাও cleanup;
	}

	odb_reprepare(repo->objects);

	যদি (delete_redundant) {
		পূর্ণসংখ্যা opts = 0;
		bool wrote_incremental_midx = write_midx == REPACK_WRITE_MIDX_INCREMENTAL;

		existing_packs_remove_redundant(&existing, packdir,
						wrote_incremental_midx);

		যদি (geometry.split_factor)
			pack_geometry_remove_redundant(&geometry, &names,
						       &existing, packdir,
						       wrote_incremental_midx);
		যদি (show_progress)
			opts |= PRUNE_PACKED_VERBOSE;
		prune_packed_objects(opts);

		যদি (!keep_unreachable &&
		    (!(pack_everything & LOOSEN_UNREACHABLE) ||
		     unpack_unreachable) &&
		    is_repository_shallow(repo))
			prune_shallow(PRUNE_QUICK);
	}

	যদি (run_update_server_info)
		update_server_info(repo, 0);

	যদি (git_env_bool(GIT_TEST_MULTI_PACK_সূচক, 0)) {
		কাঠামো odb_source_files *files = odb_source_files_downcast(existing.source);
		unsigned flags = 0;

		যদি (git_env_bool(GIT_TEST_MULTI_PACK_সূচক_WRITE_INCREMENTAL, 0))
			flags |= MIDX_WRITE_INCREMENTAL;
		write_midx_file(files->dirs->packed, ফাঁকা, ফাঁকা, flags);
	}

cleanup:
	string_list_clear(&keep_pack_list, 0);
	string_list_clear(&names, 1);
	oidset_clear(&drop_oids);
	existing_packs_release(&existing);
	pack_geometry_release(&geometry);
	pack_objects_args_release(&po_args);
	pack_objects_args_release(&cruft_po_args);

	ফেরত ret;
}
