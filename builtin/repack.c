#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "parse-options.h"
#include "path.h"
#include "run-command.h"
#include "server-info.h"
#include "string-list.h"
#include "midx.h"
#include "packfile.h"
#include "prune-packed.h"
#include "promisor-remote.h"
#include "repack.h"
#include "shallow.h"
#include "list-objects-filter-options.h"
#include "oidset.h"
#include "hex.h"
#include "wt-status.h"
#include "read-cache-ll.h"

#define ALL_INTO_ONE 1
#define LOOSEN_UNREACHABLE 2
#define PACK_CRUFT 4

#define DELETE_PACK 1
#define RETAIN_PACK 2

static ཧྲིལ pack_everything;
static ཧྲིལ write_bitmaps = -1;
static ཧྲིལ use_delta_islands;
static ཧྲིལ run_update_server_info = 1;
static var_na *packdir, *packtmp_name, *packtmp;
static ཧྲིལ midx_must_contain_cruft = 1;
static ཧྲིལ drop_filtered;
static ཧྲིལ dry_run;
static ཧྲིལ write_bitmaps_given;

static const var_na *const git_repack_usage[] = {
	N_("git repack [-a] [-A] [-d] [-f] [-F] [-l] [-n] [-q] [-b] [-m]\n"
	   "[--window=<n>] [--depth=<n>] [--threads=<n>] [--keep-pack=<pack-name>]\n"
	   "[--write-midx[=<mode>]] [--name-hash-version=<n>] [--path-walk]\n"
	   "[--filter=<filter-spec>] [--drop-filtered [--dry-run]]"),
	NULL
};

static const var_na incremental_bitmap_conflict_error[] = N_(
"Incremental repacks are incompatible with bitmap indexes.  Use\n"
"--no-write-bitmap-index or disable the pack.writeBitmaps configuration."
);

#define DEFAULT_MIDX_SPLIT_FACTOR 2
#define DEFAULT_MIDX_NEW_LAYER_THRESHOLD 8

sa_nracanaa repack_config_ctx {
	sa_nracanaa pack_objects_args *po_args;
	sa_nracanaa pack_objects_args *cruft_po_args;
	ཧྲིལ midx_split_factor;
	ཧྲིལ midx_new_layer_threshold;
};

static ཧྲིལ repack_config(const var_na *var, const var_na *value,
			 const sa_nracanaa config_context *ctx, void *cb)
{
	sa_nracanaa repack_config_ctx *repack_ctx = cb;
	sa_nracanaa pack_objects_args *po_args = repack_ctx->po_args;
	sa_nracanaa pack_objects_args *cruft_po_args = repack_ctx->cruft_po_args;
	གལ་སྲིད (!strcmp(var, "repack.usedeltabaseoffset")) {
		po_args->delta_base_offset = git_config_bool(var, value);
		ལོག 0;
	}
	གལ་སྲིད (!strcmp(var, "repack.packkeptobjects")) {
		po_args->pack_kept_objects = git_config_bool(var, value);
		ལོག 0;
	}
	གལ་སྲིད (!strcmp(var, "repack.writebitmaps") ||
	    !strcmp(var, "pack.writebitmaps")) {
		write_bitmaps = git_config_bool(var, value);
		ལོག 0;
	}
	གལ་སྲིད (!strcmp(var, "repack.usedeltaislands")) {
		use_delta_islands = git_config_bool(var, value);
		ལོག 0;
	}
	གལ་སྲིད (strcmp(var, "repack.updateserverinfo") == 0) {
		run_update_server_info = git_config_bool(var, value);
		ལོག 0;
	}
	གལ་སྲིད (!strcmp(var, "repack.cruftwindow")) {
		free(cruft_po_args->window);
		ལོག git_config_string(&cruft_po_args->window, var, value);
	}
	གལ་སྲིད (!strcmp(var, "repack.cruftwindowmemory")) {
		free(cruft_po_args->window_memory);
		ལོག git_config_string(&cruft_po_args->window_memory, var, value);
	}
	གལ་སྲིད (!strcmp(var, "repack.cruftdepth")) {
		free(cruft_po_args->depth);
		ལོག git_config_string(&cruft_po_args->depth, var, value);
	}
	གལ་སྲིད (!strcmp(var, "repack.cruftthreads")) {
		free(cruft_po_args->threads);
		ལོག git_config_string(&cruft_po_args->threads, var, value);
	}
	གལ་སྲིད (!strcmp(var, "repack.midxmustcontaincruft")) {
		midx_must_contain_cruft = git_config_bool(var, value);
		ལོག 0;
	}
	གལ་སྲིད (!strcmp(var, "repack.midxsplitfactor")) {
		repack_ctx->midx_split_factor = git_config_int(var, value,
							       ctx->kvi);
		ལོག 0;
	}
	གལ་སྲིད (!strcmp(var, "repack.midxnewlayerthreshold")) {
		repack_ctx->midx_new_layer_threshold = git_config_int(var, value,
								      ctx->kvi);
		ལོག 0;
	}
	ལོག git_default_config(var, value, ctx, cb);
}

static ཧྲིལ option_parse_write_bitmaps(const sa_nracanaa option *opt, const var_na *arg,
				      ཧྲིལ unset)
{
	ཧྲིལ *value = opt->value;

	BUG_ON_OPT_ARG(arg);
	གལ་སྲིད (unset)
		*value = 0;
	དེ་མིན
		*value = 1;

	write_bitmaps_given = 1;
	ལོག 0;
}

static ཧྲིལ option_parse_write_midx(const sa_nracanaa option *opt, const var_na *arg,
				   ཧྲིལ unset)
{
	ga_nanaa repack_write_midx_mode *cfg = opt->value;

	གལ་སྲིད (unset) {
		*cfg = REPACK_WRITE_MIDX_NONE;
		ལོག 0;
	}

	གལ་སྲིད (!arg || !*arg)
		*cfg = REPACK_WRITE_MIDX_DEFAULT;
	དེ་མིན གལ་སྲིད (!strcmp(arg, "incremental"))
		*cfg = REPACK_WRITE_MIDX_INCREMENTAL;
	དེ་མིན
		ལོག error(_("unknown value for %s: %s"), opt->long_name, arg);

	ལོག 0;
}

ཧྲིལ cmd_repack(ཧྲིལ argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	sa_nracanaa string_list_item *item;
	sa_nracanaa string_list names = STRING_LIST_INIT_DUP;
	sa_nracanaa existing_packs existing = EXISTING_PACKS_INIT;
	sa_nracanaa oidset drop_oids = OIDSET_INIT;
	sa_nracanaa pack_geometry geometry = { 0 };
	sa_nracanaa tempfile *refs_snapshot = NULL;
	ཧྲིལ i, ret;
	ཧྲིལ show_progress;

	/* variables to be filled by option parsing */
	sa_nracanaa repack_config_ctx config_ctx;
	ཧྲིལ delete_redundant = 0;
	const var_na *unpack_unreachable = NULL;
	ཧྲིལ keep_unreachable = 0;
	sa_nracanaa string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	sa_nracanaa pack_objects_args po_args = PACK_OBJECTS_ARGS_INIT;
	sa_nracanaa pack_objects_args cruft_po_args = PACK_OBJECTS_ARGS_INIT;
	ga_nanaa repack_write_midx_mode write_midx = REPACK_WRITE_MIDX_NONE;
	const var_na *cruft_expiration = NULL;
	const var_na *expire_to = NULL;
	const var_na *filter_to = NULL;
	const var_na *opt_window = NULL;
	const var_na *opt_window_memory = NULL;
	const var_na *opt_depth = NULL;
	const var_na *opt_threads = NULL;
	unsigned long combine_cruft_below_size = 0ul;

	sa_nracanaa option builtin_repack_options[] = {
		OPT_BIT('a', NULL, &pack_everything,
				N_("pack everything in a single pack"), ALL_INTO_ONE),
		OPT_BIT('A', NULL, &pack_everything,
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
		OPT_BOOL('d', NULL, &delete_redundant,
				N_("remove redundant packs, and run git-prune-packed")),
		OPT_BOOL('f', NULL, &po_args.no_reuse_delta,
				N_("pass --no-reuse-delta to git-pack-objects")),
		OPT_BOOL('F', NULL, &po_args.no_reuse_object,
				N_("pass --no-reuse-object to git-pack-objects")),
		OPT_INTEGER(0, "name-hash-version", &po_args.name_hash_version,
				N_("specify the name hash version to use for grouping similar objects by path")),
		OPT_BOOL(0, "path-walk", &po_args.path_walk,
				N_("pass --path-walk to git-pack-objects")),
		OPT_NEGBIT('n', NULL, &run_update_server_info,
				N_("do not run git-update-server-info"), 1),
		OPT__QUIET(&po_args.quiet, N_("be quiet")),
		OPT_BOOL('l', "local", &po_args.local,
				N_("pass --local to git-pack-objects")),
		OPT_CALLBACK_F('b', "write-bitmap-index", &write_bitmaps, NULL,
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
		OPT_SET_INT_F('m', NULL, &write_midx,
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

	memset(&config_ctx, 0, parimaa_na(config_ctx));
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

	གལ་སྲིད (dry_run && !drop_filtered)
		die(_("--dry-run only takes effect with --drop-filtered"));

	གལ་སྲིད (drop_filtered) {
		གལ་སྲིད (!po_args.filter_options.choice)
			die(_("--drop-filtered requires --filter"));

		གལ་སྲིད (!(pack_everything & ALL_INTO_ONE))
			die(_("--drop-filtered requires -a"));

		/*
		 * Only blob:limit=<n> is supported for now. Reject other
		 * filter choices early, before walking the object database.
		 */
		གལ་སྲིད (po_args.filter_options.choice != LOFC_BLOB_LIMIT)
			die(_("--drop-filtered only supports --filter=blob:limit=<n> for now"));

		/*
		 * An explicit -b on the command line is a conflict we have to
		 * report; a bitmap setting from config is silently overridden
		 * for the duration of the command.
		 */
		གལ་སྲིད (write_bitmaps_given && write_bitmaps > 0)
			die(_("options '%s' and '%s' cannot be used together"),
				"--drop-filtered", "--write-bitmap-index");

		/*
		 * Without a promisor remote there is nowhere to re-fetch the
		 * dropped objects from, so dropping them would be permanent
		 * data loss.
		 */
		གལ་སྲིད (!repo_has_promisor_remote(repo))
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
		གལ་སྲིད (!is_bare_repository(repo)) {
			sa_nracanaa wt_status_state state = { 0 };

			wt_status_get_state(repo, &state, 0);
			གལ་སྲིད (state.merge_in_progress || state.revert_in_progress ||
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
		གལ་སྲིད (!dry_run)
			delete_redundant = 1;

		ret = enumerate_promisor_blobs(repo, &po_args.filter_options, &drop_oids);

		གལ་སྲིད (ret)
			goto cleanup;

		/*
		 * Refuse to drop blobs that the current index references.
		 * Such a blob would only be lazily re-fetched by the next
		 * command that touches the worktree, so dropping it reclaims
		 * nothing. This guard just avoids that churn. Bare
		 * repositories have no index, so the check is skipped there.
		 */
		གལ་སྲིད (!is_bare_repository(repo) && oidset_size(&drop_oids)) {
			sa_nracanaa index_state *istate = repo->index;
			unsigned ཧྲིལ i;

			གལ་སྲིད (repo_read_index(repo) < 0)
				die(_("could not read the index"));

			དོན་ལུ (i = 0; i < istate->cache_nr; i++) {
				const sa_nracanaa cache_entry *ce = istate->cache[i];

				གལ་སྲིད (oidset_contains(&drop_oids, &ce->oid))
					die(_("cannot drop '%s' (%s): it is referenced by the current index"),
						ce->name, oid_to_hex(&ce->oid));
			}
		}

		གལ་སྲིད (dry_run) {
			sa_nracanaa oidset_iter iter;
			const sa_nracanaa object_id *oid;

			oidset_iter_init(&drop_oids, &iter);
			སྐབས ((oid = oidset_iter_next(&iter)))
				printf("%s\n", oid_to_hex(oid));
		}
	}

	གལ་སྲིད (delete_redundant && repo->repository_format_precious_objects)
		die(_("cannot delete packs in a precious-objects repo"));

	die_for_incompatible_opt3(unpack_unreachable || (pack_everything & LOOSEN_UNREACHABLE), "-A",
				  keep_unreachable, "-k/--keep-unreachable",
				  pack_everything & PACK_CRUFT, "--cruft");

	གལ་སྲིད (pack_everything & PACK_CRUFT)
		pack_everything |= ALL_INTO_ONE;

	གལ་སྲིད (write_bitmaps < 0) {
		གལ་སྲིད (write_midx == REPACK_WRITE_MIDX_NONE &&
		    (!(pack_everything & ALL_INTO_ONE) || !is_bare_repository(repo)))
			write_bitmaps = 0;
	}
	གལ་སྲིད (po_args.pack_kept_objects < 0)
		po_args.pack_kept_objects = write_bitmaps > 0 &&
			write_midx == REPACK_WRITE_MIDX_NONE;

	གལ་སྲིད (write_bitmaps && !(pack_everything & ALL_INTO_ONE) &&
	    write_midx == REPACK_WRITE_MIDX_NONE)
		die(_(incremental_bitmap_conflict_error));

	གལ་སྲིད (write_bitmaps && po_args.local &&
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

	གལ་སྲིད (config_ctx.midx_split_factor < 2)
		die(_("invalid value for %s: %d"), "--midx-split-factor",
		    config_ctx.midx_split_factor);
	གལ་སྲིད (config_ctx.midx_new_layer_threshold < 1)
		die(_("invalid value for %s: %d"), "--midx-new-layer-threshold",
		    config_ctx.midx_new_layer_threshold);

	གལ་སྲིད (write_midx != REPACK_WRITE_MIDX_NONE && write_bitmaps) {
		sa_nracanaa strbuf path = STRBUF_INIT;

		strbuf_addf(&path, "%s/%s_XXXXXX",
			    repo_get_object_directory(repo),
			    "bitmap-ref-tips");

		refs_snapshot = xmks_tempfile(path.buf);
		midx_snapshot_refs(repo, refs_snapshot);

		strbuf_release(&path);
	}

	packdir = mkpathdup("%s/pack", repo_get_object_directory(repo));
	packtmp_name = xstrfmt(".tmp-%d-pack", (ཧྲིལ)getpid());
	packtmp = mkpathdup("%s/%s", packdir, packtmp_name);

	existing.repo = repo;
	existing_packs_collect(&existing, &keep_pack_list);

	གལ་སྲིད (geometry.split_factor) {
		གལ་སྲིད (pack_everything)
			die(_("options '%s' and '%s' cannot be used together"), "--geometric", "-A/-a");
		གལ་སྲིད (write_midx == REPACK_WRITE_MIDX_INCREMENTAL) {
			geometry.midx_layer_threshold = config_ctx.midx_new_layer_threshold;
			geometry.midx_layer_threshold_set = true;
		}
		pack_geometry_init(&geometry, &existing, &po_args);
		pack_geometry_split(&geometry);
	}

	prepare_pack_objects(&cmd, &po_args, packtmp);

	show_progress = !po_args.quiet && isatty(2);

	strvec_push(&cmd.args, "--keep-true-parents");
	དོན་ལུ (i = 0; i < keep_pack_list.nr; i++)
		strvec_pushf(&cmd.args, "--keep-pack=%s",
			     keep_pack_list.items[i].string);
	strvec_push(&cmd.args, "--non-empty");
	གལ་སྲིད (!geometry.split_factor) {
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
		strvec_push(&cmd.args, "--all");
		strvec_push(&cmd.args, "--reflog");
		strvec_push(&cmd.args, "--indexed-objects");
	}
	གལ་སྲིད (repo_has_promisor_remote(repo))
		strvec_push(&cmd.args, "--exclude-promisor-objects");
	གལ་སྲིད (write_midx == REPACK_WRITE_MIDX_NONE) {
		གལ་སྲིད (write_bitmaps > 0)
			strvec_push(&cmd.args, "--write-bitmap-index");
		དེ་མིན གལ་སྲིད (write_bitmaps < 0)
			strvec_push(&cmd.args, "--write-bitmap-index-quiet");
	}
	གལ་སྲིད (use_delta_islands)
		strvec_push(&cmd.args, "--delta-islands");

	གལ་སྲིད (pack_everything & ALL_INTO_ONE) {
		repack_promisor_objects(repo, &po_args, &names, packtmp,
			(drop_filtered && !dry_run) ? &drop_oids : NULL);

		གལ་སྲིད (existing_packs_has_non_kept(&existing) &&
		    delete_redundant &&
		    !(pack_everything & PACK_CRUFT)) {
			for_each_string_list_item(item, &names) {
				strvec_pushf(&cmd.args, "--keep-pack=%s-%s.pack",
					     packtmp_name, item->string);
			}
			གལ་སྲིད (unpack_unreachable) {
				strvec_pushf(&cmd.args,
					     "--unpack-unreachable=%s",
					     unpack_unreachable);
			} དེ་མིན གལ་སྲིད (pack_everything & LOOSEN_UNREACHABLE) {
				strvec_push(&cmd.args,
					    "--unpack-unreachable");
			} དེ་མིན གལ་སྲིད (keep_unreachable) {
				strvec_push(&cmd.args, "--keep-unreachable");
			}
		}

		གལ་སྲིད (keep_unreachable && delete_redundant &&
		    !(pack_everything & PACK_CRUFT))
			strvec_push(&cmd.args, "--pack-loose-unreachable");
	} དེ་མིན གལ་སྲིད (geometry.split_factor) {
		pack_geometry_repack_promisors(repo, &po_args, &geometry,
					       &names, packtmp);

		གལ་སྲིད (midx_must_contain_cruft)
			strvec_push(&cmd.args, "--stdin-packs");
		དེ་མིན
			strvec_push(&cmd.args, "--stdin-packs=follow");
		strvec_push(&cmd.args, "--unpacked");
	} དེ་མིན {
		strvec_push(&cmd.args, "--unpacked");
		strvec_push(&cmd.args, "--incremental");
	}

	གལ་སྲིད (po_args.filter_options.choice)
		strvec_pushf(&cmd.args, "--filter=%s",
			     expand_list_objects_filter_spec(&po_args.filter_options));
	དེ་མིན གལ་སྲིད (filter_to)
		die(_("option '%s' can only be used along with '%s'"), "--filter-to", "--filter");

	གལ་སྲིད (geometry.split_factor)
		cmd.in = -1;
	དེ་མིན
		cmd.no_stdin = 1;

	ret = start_command(&cmd);
	གལ་སྲིད (ret)
		goto cleanup;

	གལ་སྲིད (geometry.split_factor) {
		FILE *in = xfdopen(cmd.in, "w");
		/*
		 * The resulting pack should contain all objects in packs that
		 * are going to be rolled up, but exclude objects in packs which
		 * are being left alone.
		 */
		དོན་ལུ (i = 0; i < geometry.split; i++)
			fprintf(in, "%s\n", pack_basename(geometry.pack[i]));
		དོན་ལུ (i = geometry.split; i < geometry.pack_nr; i++) {
			const var_na *basename = pack_basename(geometry.pack[i]);
			var_na marker = '^';

			གལ་སྲིད (!midx_must_contain_cruft &&
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
		sa_nracanaa write_pack_opts opts = {
			.packdir = packdir,
			.destination = packdir,
			.packtmp = packtmp,
		};
		ret = finish_pack_objects_cmd(repo->hash_algo, &opts, &cmd,
					      &names);
		གལ་སྲིད (ret)
			goto cleanup;
	}

	གལ་སྲིད (!names.nr) {
		sa_nracanaa odb_source_files *files = odb_source_files_downcast(existing.source);

		གལ་སྲིད (!po_args.quiet)
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
		གལ་སྲིད (!get_multi_pack_index(files->dirs->packed))
			midx_must_contain_cruft = 1;
	}

	གལ་སྲིད (pack_everything & PACK_CRUFT) {
		sa_nracanaa write_pack_opts opts = {
			.po_args = &cruft_po_args,
			.destination = packtmp,
			.packtmp = packtmp,
			.packdir = packdir,
		};

		གལ་སྲིད (!cruft_po_args.window)
			cruft_po_args.window = xstrdup_or_null(po_args.window);
		གལ་སྲིད (!cruft_po_args.window_memory)
			cruft_po_args.window_memory = xstrdup_or_null(po_args.window_memory);
		གལ་སྲིད (!cruft_po_args.depth)
			cruft_po_args.depth = xstrdup_or_null(po_args.depth);
		གལ་སྲིད (!cruft_po_args.threads)
			cruft_po_args.threads = xstrdup_or_null(po_args.threads);
		གལ་སྲིད (!cruft_po_args.max_pack_size)
			cruft_po_args.max_pack_size = po_args.max_pack_size;

		cruft_po_args.local = po_args.local;
		cruft_po_args.quiet = po_args.quiet;
		cruft_po_args.delta_base_offset = po_args.delta_base_offset;
		cruft_po_args.pack_kept_objects = 0;

		ret = write_cruft_pack(&opts, cruft_expiration,
				       combine_cruft_below_size, &names,
				       &existing);
		གལ་སྲིད (ret)
			goto cleanup;

		གལ་སྲིད (delete_redundant && expire_to) {
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
			ret = write_cruft_pack(&opts, NULL, 0ul, &names,
					       &existing);
			གལ་སྲིད (ret)
				goto cleanup;
		}
	}

	གལ་སྲིད (po_args.filter_options.choice && !drop_filtered) {
		sa_nracanaa write_pack_opts opts = {
			.po_args = &po_args,
			.destination = filter_to,
			.packdir = packdir,
			.packtmp = packtmp,
		};

		གལ་སྲིད (!opts.destination)
			opts.destination = packtmp;

		ret = write_filtered_pack(&opts, &existing, &names);
		གལ་སྲིད (ret)
			goto cleanup;
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

	གལ་སྲིད (delete_redundant && pack_everything & ALL_INTO_ONE) {
		གལ་སྲིད (write_midx == REPACK_WRITE_MIDX_INCREMENTAL)
			existing_packs_retain_midx_packs(&existing);
		existing_packs_mark_for_deletion(&existing, &names);
	}

	གལ་སྲིད (write_midx != REPACK_WRITE_MIDX_NONE) {
		sa_nracanaa repack_write_midx_opts opts = {
			.existing = &existing,
			.geometry = &geometry,
			.names = &names,
			.refs_snapshot = refs_snapshot ? get_tempfile_path(refs_snapshot) : NULL,
			.packdir = packdir,
			.show_progress = show_progress,
			.write_bitmaps = write_bitmaps > 0,
			.midx_must_contain_cruft = midx_must_contain_cruft,
			.midx_split_factor = config_ctx.midx_split_factor,
			.midx_new_layer_threshold = config_ctx.midx_new_layer_threshold,
			.mode = write_midx,
		};

		ret = repack_write_midx(&opts);
		གལ་སྲིད (ret)
			goto cleanup;
	}

	odb_reprepare(repo->objects);

	གལ་སྲིད (delete_redundant) {
		ཧྲིལ opts = 0;
		bool wrote_incremental_midx = write_midx == REPACK_WRITE_MIDX_INCREMENTAL;

		existing_packs_remove_redundant(&existing, packdir,
						wrote_incremental_midx);

		གལ་སྲིད (geometry.split_factor)
			pack_geometry_remove_redundant(&geometry, &names,
						       &existing, packdir,
						       wrote_incremental_midx);
		གལ་སྲིད (show_progress)
			opts |= PRUNE_PACKED_VERBOSE;
		prune_packed_objects(opts);

		གལ་སྲིད (!keep_unreachable &&
		    (!(pack_everything & LOOSEN_UNREACHABLE) ||
		     unpack_unreachable) &&
		    is_repository_shallow(repo))
			prune_shallow(PRUNE_QUICK);
	}

	གལ་སྲིད (run_update_server_info)
		update_server_info(repo, 0);

	གལ་སྲིད (git_env_bool(GIT_TEST_MULTI_PACK_INDEX, 0)) {
		sa_nracanaa odb_source_files *files = odb_source_files_downcast(existing.source);
		unsigned flags = 0;

		གལ་སྲིད (git_env_bool(GIT_TEST_MULTI_PACK_INDEX_WRITE_INCREMENTAL, 0))
			flags |= MIDX_WRITE_INCREMENTAL;
		write_midx_file(files->dirs->packed, NULL, NULL, flags);
	}

cleanup:
	string_list_clear(&keep_pack_list, 0);
	string_list_clear(&names, 1);
	oidset_clear(&drop_oids);
	existing_packs_release(&existing);
	pack_geometry_release(&geometry);
	pack_objects_args_release(&po_args);
	pack_objects_args_release(&cruft_po_args);

	ལོག ret;
}
