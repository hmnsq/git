/*
 * git gc builtin command
 *
 * Cleanup unreachable files and optimize the repository.
 *
 * Copyright (c) 2007 James Bowes
 *
 * Based on git-gc.sh, which is
 *
 * Copyright (c) 2006 Shawn O. Pearce
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "date.h"
#شامل "dir.h"
#شامل "environment.h"
#شامل "hex.h"
#شامل "config.h"
#شامل "tempfile.h"
#شامل "lockfile.h"
#شامل "parse-options.h"
#شامل "run-command.h"
#شامل "sigchain.h"
#شامل "strvec.h"
#شامل "commit.h"
#شامل "commit-graph.h"
#شامل "packfile.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "path.h"
#شامل "reflog.h"
#شامل "rerere.h"
#شامل "revision.h"
#شامل "refs.h"
#شامل "remote.h"
#شامل "exec-cmd.h"
#شامل "gettext.h"
#شامل "hook.h"
#شامل "setup.h"
#شامل "trace2.h"
#شامل "worktree.h"

#define FAILED_RUN "failed to run %s"

ایستا ثابت نویسه * ثابت builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	پوچ
};

ایستا timestamp_t gc_log_expire_time;
ایستا ساختار tempfile *pidfile;
ایستا ساختار lock_file log_lock;
ایستا ساختار string_list pack_garbage = STRING_LIST_INIT_DUP;

ایستا تهی clean_pack_garbage(تهی)
{
	صحیح i;
	برای (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.اقلام[i].string);
	string_list_clear(&pack_garbage, 0);
}

ایستا تهی report_pack_garbage(unsigned seen_bits, ثابت نویسه *path)
{
	اگر (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

ایستا تهی process_log_file(تهی)
{
	ساختار stat st;
	اگر (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		صحیح saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} وگرنه اگر (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} وگرنه {
		نویسه *path = repo_git_path(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(path);
		rollback_lock_file(&log_lock);
		آزاد(path);
	}
}

ایستا تهی process_log_file_at_exit(تهی)
{
	fflush(stderr);
	process_log_file();
}

ایستا صحیح gc_config_is_timestamp_never(ثابت نویسه *var)
{
	ثابت نویسه *مقدار;
	timestamp_t expire;

	اگر (!repo_config_get_مقدار(the_repository, var, &مقدار) && مقدار) {
		اگر (parse_expiry_date(مقدار, &expire))
			die(_("failed to parse '%s' value '%s'"), var, مقدار);
		بازگشت expire == 0;
	}
	بازگشت 0;
}

ساختار gc_config {
	صحیح pack_refs;
	صحیح prune_reflogs;
	صحیح cruft_packs;
	unsigned long max_cruft_size;
	صحیح aggressive_depth;
	صحیح aggressive_window;
	صحیح detach_auto;
	نویسه *gc_log_expire;
	نویسه *prune_expire;
	نویسه *prune_worktrees_expire;
	نویسه *repack_expire_to;
};

#define GC_CONFIG_INIT { \
	.pack_refs = 1, \
	.prune_reflogs = 1, \
	.cruft_packs = 1, \
	.aggressive_depth = 50, \
	.aggressive_window = 250, \
	.detach_auto = 1, \
	.gc_log_expire = xstrdup("1.day.ago"), \
	.prune_expire = xstrdup("2.weeks.ago"), \
	.prune_worktrees_expire = xstrdup("3.months.ago"), \
}

ایستا تهی gc_config_release(ساختار gc_config *cfg)
{
	آزاد(cfg->gc_log_expire);
	آزاد(cfg->prune_expire);
	آزاد(cfg->prune_worktrees_expire);
}

ایستا تهی gc_config(ساختار gc_config *cfg)
{
	ثابت نویسه *مقدار;
	نویسه *owned = پوچ;

	اگر (!repo_config_get_مقدار(the_repository, "gc.packrefs", &مقدار)) {
		اگر (مقدار && !strcmp(مقدار, "notbare"))
			cfg->pack_refs = -1;
		وگرنه
			cfg->pack_refs = git_config_bool("gc.packrefs", مقدار);
	}

	اگر (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	اگر (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		آزاد(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	اگر (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		آزاد(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	اگر (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		آزاد(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, پوچ);
}

شمارش schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

ایستا شمارش schedule_priority parse_schedule(ثابت نویسه *مقدار)
{
	اگر (!مقدار)
		بازگشت SCHEDULE_NONE;
	اگر (!strcasecmp(مقدار, "hourly"))
		بازگشت SCHEDULE_HOURLY;
	اگر (!strcasecmp(مقدار, "daily"))
		بازگشت SCHEDULE_DAILY;
	اگر (!strcasecmp(مقدار, "weekly"))
		بازگشت SCHEDULE_WEEKLY;
	بازگشت SCHEDULE_NONE;
}

شمارش maintenance_task_label {
	TASK_PREFETCH,
	TASK_LOOSE_OBJECTS,
	TASK_INCREMENTAL_REPACK,
	TASK_GEOMETRIC_REPACK,
	TASK_GC,
	TASK_COMMIT_GRAPH,
	TASK_PACK_REFS,
	TASK_REFLOG_EXPIRE,
	TASK_WORKTREE_PRUNE,
	TASK_RERERE_GC,

	/* Leave as final value */
	TASK_شمار
};

ساختار maintenance_run_opts {
	شمارش maintenance_task_label *tasks;
	size_t tasks_nr, tasks_alloc;
	صحیح auto_flag;
	صحیح detach;
	صحیح quiet;
	شمارش schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

ایستا تهی maintenance_run_opts_release(ساختار maintenance_run_opts *opts)
{
	آزاد(opts->tasks);
}

ایستا صحیح pack_refs_condition(UNUSED ساختار gc_config *cfg)
{
	ساختار string_list included_refs = STRING_LIST_INIT_NODUP;
	ساختار ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	ساختار refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	اگر (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		بازگشت 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	بازگشت required;
}

ایستا صحیح maintenance_task_pack_refs(ساختار maintenance_run_opts *opts,
				      UNUSED ساختار gc_config *cfg)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", پوچ);
	اگر (opts->auto_flag)
		strvec_افزودن(&cmd.args, "--auto");

	بازگشت run_command(&cmd);
}

ساختار شمار_reflog_entries_data {
	ساختار expire_reflog_policy_cb policy;
	size_t شمار;
	size_t limit;
};

ایستا صحیح شمار_reflog_entries(ثابت نویسه *refname UNUSED,
				ساختار object_id *old_oid, ساختار object_id *new_oid,
				ثابت نویسه *committer, timestamp_t timestamp,
				صحیح tz, ثابت نویسه *پیام, تهی *cb_data)
{
	ساختار شمار_reflog_entries_data *data = cb_data;
	اگر (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, پیام, &data->policy))
		data->شمار++;
	بازگشت data->شمار >= data->limit;
}

ایستا صحیح reflog_expire_condition(ساختار gc_config *cfg UNUSED)
{
	timestamp_t now = time(پوچ);
	ساختار شمار_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	صحیح limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	اگر (!limit)
		بازگشت 0;
	اگر (limit < 0)
		بازگشت 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 شمار_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	بازگشت data.شمار >= data.limit;
}

ایستا صحیح maintenance_task_reflog_expire(ساختار maintenance_run_opts *opts UNUSED,
					  ساختار gc_config *cfg UNUSED)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", پوچ);
	بازگشت run_command(&cmd);
}

ایستا صحیح maintenance_task_worktree_prune(ساختار maintenance_run_opts *opts UNUSED,
					   ساختار gc_config *cfg)
{
	ساختار child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", پوچ);
	strvec_افزودن(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	بازگشت run_command(&prune_worktrees_cmd);
}

ایستا صحیح worktree_prune_condition(ساختار gc_config *cfg)
{
	ساختار strbuf buf = STRBUF_INIT;
	صحیح should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	ساختار dirent *d;
	DIR *dir = پوچ;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	اگر (limit <= 0) {
		should_prune = limit < 0;
		برو out;
	}

	اگر (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		برو out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	اگر (!dir)
		برو out;

	وقتی (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		نویسه *wtpath;
		strbuf_reset(&buf);
		اگر (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		آزاد(wtpath);
	}

	should_prune = !limit;

out:
	اگر (dir)
		closedir(dir);
	strbuf_release(&buf);
	بازگشت should_prune;
}

ایستا صحیح maintenance_task_rerere_gc(ساختار maintenance_run_opts *opts UNUSED,
				      ساختار gc_config *cfg UNUSED)
{
	ساختار child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", پوچ);
	بازگشت run_command(&rerere_cmd);
}

ایستا صحیح rerere_gc_condition(ساختار gc_config *cfg UNUSED)
{
	صحیح limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	اگر (!limit)
		بازگشت 0; /* never prune */
	اگر (limit < 0)
		بازگشت 1; /* always prune */

	بازگشت rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
ایستا ثابت نویسه *lock_repo_for_gc(صحیح force, pid_t* ret_pid)
{
	ساختار lock_file lock = LOCK_INIT;
	نویسه my_host[HOST_NAME_MAX + 1];
	ساختار strbuf sb = STRBUF_INIT;
	ساختار stat st;
	uintmax_t pid;
	FILE *fp;
	صحیح fd;
	نویسه *pidfile_path;

	اگر (is_tempfile_active(pidfile))
		/* already locked */
		بازگشت پوچ;

	اگر (xgethostname(my_host, اندازه(my_host)))
		xsnprintf(my_host, اندازه(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	اگر (!force) {
		ایستا نویسه locking_host[HOST_NAME_MAX + 1];
		ایستا نویسه *scan_fmt;
		صحیح should_exit;

		اگر (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_path, "r");
		memset(locking_host, 0, اندازه(locking_host));
		should_exit =
			fp != پوچ &&
			!fstat(fileno(fp), &st) &&
			/*
			 * 12 hour limit is very generous as gc should
			 * never take that long. On the other hand we
			 * don't really need a strict limit here,
			 * running gc --auto one day late is not a big
			 * problem. --force can be used in manual gc
			 * after the user verifies that no gc is
			 * running.
			 */
			time(پوچ) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, scan_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		اگر (fp)
			fclose(fp);
		اگر (should_exit) {
			اگر (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			آزاد(pidfile_path);
			بازگشت locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	آزاد(pidfile_path);
	بازگشت پوچ;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
ایستا صحیح report_last_gc_error(تهی)
{
	ساختار strbuf sb = STRBUF_INIT;
	صحیح ret = 0;
	ssize_t len;
	ساختار stat st;
	نویسه *gc_log_path = repo_git_path(the_repository, "gc.log");

	اگر (stat(gc_log_path, &st)) {
		اگر (errno == ENOENT)
			برو done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		برو done;
	}

	اگر (st.st_mtime < gc_log_expire_time)
		برو done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	اگر (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	وگرنه اگر (len > 0) {
		/*
		 * A previous gc failed.  Report the error, and don't
		 * bother with an automatic gc run since it is likely
		 * to fail in the same way.
		 */
		warning(_("The last gc run reported the following. "
			       "Please correct the root cause\n"
			       "and remove %s\n"
			       "Automatic cleanup will not be performed "
			       "until the file is removed.\n\n"
			       "%s"),
			    gc_log_path, sb.buf);
		ret = 1;
	}
	strbuf_release(&sb);
done:
	آزاد(gc_log_path);
	بازگشت ret;
}

ایستا صحیح gc_foreground_tasks(ساختار maintenance_run_opts *opts,
			       ساختار gc_config *cfg)
{
	اگر (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		بازگشت error(FAILED_RUN, "pack-refs");
	اگر (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		بازگشت error(FAILED_RUN, "reflog");
	بازگشت 0;
}

ایستا صحیح maintenance_task_odb(ساختار maintenance_run_opts *opts,
				ساختار gc_config *cfg,
				صحیح keep_largest_pack,
				صحیح aggressive)
{
	ساختار odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	اگر (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	اگر (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	اگر (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	بازگشت odb_optimize(the_repository->objects, &odb_opts);
}

صحیح cmd_gc(صحیح argc,
	   ثابت نویسه **argv,
	   ثابت نویسه *prefix,
	   ساختار repository *repo UNUSED)
{
	صحیح aggressive = 0;
	صحیح force = 0;
	ثابت نویسه *name;
	pid_t pid;
	صحیح daemonized = 0;
	صحیح keep_largest_pack = -1;
	صحیح skip_foreground_tasks = 0;
	timestamp_t dummy;
	ساختار maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ساختار gc_config cfg = GC_CONFIG_INIT;
	ثابت نویسه *prune_expire_sentinel = "sentinel";
	ثابت نویسه *prune_expire_arg = prune_expire_sentinel;
	صحیح ret;
	ساختار option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.مقدار = &prune_expire_arg,
			.argh = N_("date"),
			.help = N_("prune unreferenced objects"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)prune_expire_arg,
		},
		OPT_BOOL(0, "cruft", &cfg.cruft_packs, N_("pack unreferenced objects separately")),
		OPT_UNSIGNED(0, "max-cruft-size", &cfg.max_cruft_size,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL(0, "aggressive", &aggressive, N_("be more thorough (increased runtime)")),
		OPT_BOOL_F(0, "auto", &opts.auto_flag, N_("enable auto-gc mode"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform garbage collection in the background")),
		OPT_BOOL_F(0, "force", &force,
			   N_("force running gc even if there may be another gc running"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "keep-largest-pack", &keep_largest_pack,
			 N_("repack all other packs except the largest pack")),
		OPT_STRING(0, "expire-to", &cfg.repack_expire_to, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_HIDDEN_BOOL(0, "skip-foreground-tasks", &skip_foreground_tasks,
			   N_("skip maintenance tasks typically done in the foreground")),
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_gc_usage, builtin_gc_options);

	gc_config(&cfg);

	اگر (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	اگر (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	اگر (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	اگر (prune_expire_arg != prune_expire_sentinel) {
		آزاد(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	اگر (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	اگر (opts.auto_flag) {
		ساختار odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		اگر (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		اگر (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			برو out;
		}

		اگر (!opts.quiet) {
			اگر (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			وگرنه
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	اگر (opts.detach > 0) {
		ret = report_last_gc_error();
		اگر (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			برو out;

		} وگرنه اگر (ret) {
			/* an I/O error occurred, already reported */
			برو out;
		}

		اگر (!skip_foreground_tasks) {
			اگر (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				برو out;
			}

			اگر (gc_foreground_tasks(&opts, &cfg) < 0)
				die(پوچ);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = lock_repo_for_gc(force, &pid);
	اگر (name) {
		اگر (opts.auto_flag) {
			ret = 0;
			برو out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	اگر (daemonized) {
		نویسه *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		آزاد(path);
	}

	اگر (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	اگر (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	اگر (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	اگر (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(پوچ);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	اگر (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	اگر (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     پوچ);

	اگر (!daemonized) {
		نویسه *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		آزاد(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	بازگشت 0;
}

ایستا ثابت نویسه *ثابت builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	پوچ
};

ایستا صحیح maintenance_opt_schedule(ثابت ساختار option *opt, ثابت نویسه *arg,
				    صحیح unset)
{
	شمارش schedule_priority *priority = opt->مقدار;

	اگر (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	اگر (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	بازگشت 0;
}

ساختار cg_auto_data {
	صحیح رقم_not_in_graph;
	صحیح limit;
};

ایستا صحیح dfs_on_ref(ثابت ساختار reference *ref, تهی *cb_data)
{
	ساختار cg_auto_data *data = (ساختار cg_auto_data *)cb_data;
	صحیح result = 0;
	ثابت ساختار object_id *maybe_peeled = ref->oid;
	ساختار object_id peeled;
	ساختار commit_list *پشته = پوچ;
	ساختار commit *commit;

	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	اگر (odb_read_object_info(the_repository->objects, maybe_peeled, پوچ) != OBJ_COMMIT)
		بازگشت 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	اگر (!commit || commit->object.flags & SEEN)
		بازگشت 0;
	commit->object.flags |= SEEN;

	اگر (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		بازگشت 0;

	data->رقم_not_in_graph++;

	اگر (data->رقم_not_in_graph >= data->limit)
		بازگشت 1;

	commit_list_درج(commit, &پشته);

	وقتی (!result && پشته) {
		ساختار commit_list *parent;

		commit = برداشت_commit(&پشته);

		برای (parent = commit->parents; parent; parent = parent->next) {
			اگر (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				ادامه;

			parent->item->object.flags |= SEEN;
			data->رقم_not_in_graph++;

			اگر (data->رقم_not_in_graph >= data->limit) {
				result = 1;
				بشکن;
			}

			commit_list_درج(parent->item, &پشته);
		}
	}

	commit_list_free(پشته);
	بازگشت result;
}

ایستا صحیح should_write_commit_graph(ساختار gc_config *cfg UNUSED)
{
	صحیح result;
	ساختار cg_auto_data data;

	data.رقم_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	اگر (!data.limit)
		بازگشت 0;
	اگر (data.limit < 0)
		بازگشت 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	بازگشت result;
}

ایستا صحیح run_write_commit_graph(ساختار maintenance_run_opts *opts)
{
	ساختار child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", پوچ);

	اگر (opts->quiet)
		strvec_افزودن(&child.args, "--no-progress");
	وگرنه
		strvec_افزودن(&child.args, "--progress");

	بازگشت !!run_command(&child);
}

ایستا صحیح maintenance_task_commit_graph(ساختار maintenance_run_opts *opts,
					 ساختار gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	اگر (!the_repository->settings.core_commit_graph)
		بازگشت 0;

	اگر (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		بازگشت 1;
	}

	بازگشت 0;
}

ایستا صحیح fetch_remote(ساختار remote *remote, تهی *cbdata)
{
	ساختار maintenance_run_opts *opts = cbdata;
	ساختار child_process child = CHILD_PROCESS_INIT;

	اگر (remote->skip_default_update)
		بازگشت 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     پوچ);

	اگر (opts->quiet)
		strvec_افزودن(&child.args, "--quiet");

	بازگشت !!run_command(&child);
}

ایستا صحیح maintenance_task_prefetch(ساختار maintenance_run_opts *opts,
				     ساختار gc_config *cfg UNUSED)
{
	اگر (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		بازگشت 1;
	}

	بازگشت 0;
}

ایستا صحیح maintenance_task_gc_foreground(ساختار maintenance_run_opts *opts,
					  ساختار gc_config *cfg)
{
	بازگشت gc_foreground_tasks(opts, cfg);
}

ایستا صحیح maintenance_task_gc_background(ساختار maintenance_run_opts *opts,
					  ساختار gc_config *cfg UNUSED)
{
	ساختار child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_افزودن(&child.args, "gc");

	اگر (opts->auto_flag)
		strvec_افزودن(&child.args, "--auto");
	اگر (opts->quiet)
		strvec_افزودن(&child.args, "--quiet");
	وگرنه
		strvec_افزودن(&child.args, "--no-quiet");
	strvec_افزودن(&child.args, "--no-detach");
	strvec_افزودن(&child.args, "--skip-foreground-tasks");

	بازگشت run_command(&child);
}

ایستا صحیح gc_condition(ساختار gc_config *cfg)
{
	ساختار odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	بازگشت odb_optimize_required(the_repository->objects, &opts);
}

ایستا صحیح prune_packed(ساختار maintenance_run_opts *opts)
{
	ساختار child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_افزودن(&child.args, "prune-packed");

	اگر (opts->quiet)
		strvec_افزودن(&child.args, "--quiet");

	بازگشت !!run_command(&child);
}

ساختار write_loose_object_data {
	FILE *in;
	صحیح شمار;
	صحیح batch_size;
};

ایستا صحیح loose_object_auto_limit = 100;

ایستا صحیح loose_object_شمار(ثابت ساختار object_id *oid UNUSED,
			      ثابت نویسه *path UNUSED,
			      تهی *data)
{
	صحیح *شمار = (صحیح*)data;
	اگر (++(*شمار) >= loose_object_auto_limit)
		بازگشت 1;
	بازگشت 0;
}

ایستا صحیح loose_object_auto_condition(ساختار gc_config *cfg UNUSED)
{
	صحیح شمار = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	اگر (!loose_object_auto_limit)
		بازگشت 0;
	اگر (loose_object_auto_limit < 0)
		بازگشت 1;

	بازگشت for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_شمار,
					     پوچ, پوچ, &شمار);
}

ایستا صحیح bail_on_loose(ثابت ساختار object_id *oid UNUSED,
			 ثابت نویسه *path UNUSED,
			 تهی *data UNUSED)
{
	بازگشت 1;
}

ایستا صحیح write_loose_object_to_stdin(ثابت ساختار object_id *oid,
				       ثابت نویسه *path UNUSED,
				       تهی *data)
{
	ساختار write_loose_object_data *d = (ساختار write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	بازگشت ++(d->شمار) > d->batch_size;
}

ایستا صحیح pack_loose(ساختار maintenance_run_opts *opts)
{
	ساختار repository *r = the_repository;
	صحیح result = 0;
	ساختار write_loose_object_data data;
	ساختار child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	اگر (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   پوچ, پوچ, پوچ))
		بازگشت 0;

	pack_proc.git_cmd = 1;

	strvec_افزودن(&pack_proc.args, "pack-objects");
	اگر (opts->quiet)
		strvec_افزودن(&pack_proc.args, "--quiet");
	وگرنه
		strvec_افزودن(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	اگر (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		بازگشت 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.شمار = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	اگر (!data.batch_size)
		data.batch_size = INT_MAX;
	وگرنه اگر (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      پوچ, پوچ, &data);

	fclose(data.in);

	اگر (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	بازگشت result;
}

ایستا صحیح maintenance_task_loose_objects(ساختار maintenance_run_opts *opts,
					  ساختار gc_config *cfg UNUSED)
{
	بازگشت prune_packed(opts) || pack_loose(opts);
}

ایستا صحیح incremental_repack_auto_condition(ساختار gc_config *cfg UNUSED)
{
	ساختار packed_git *p;
	صحیح incremental_repack_auto_limit = 10;
	صحیح شمار = 0;

	prepare_repo_settings(the_repository);
	اگر (!the_repository->settings.core_multi_pack_index)
		بازگشت 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	اگر (!incremental_repack_auto_limit)
		بازگشت 0;
	اگر (incremental_repack_auto_limit < 0)
		بازگشت 1;

	repo_for_each_pack(the_repository, p) {
		اگر (شمار >= incremental_repack_auto_limit)
			بشکن;
		اگر (!p->multi_pack_index)
			شمار++;
	}

	بازگشت شمار >= incremental_repack_auto_limit;
}

ایستا صحیح multi_pack_index_write(ساختار maintenance_run_opts *opts)
{
	ساختار child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", پوچ);

	اگر (opts->quiet)
		strvec_افزودن(&child.args, "--no-progress");
	وگرنه
		strvec_افزودن(&child.args, "--progress");

	اگر (run_command(&child))
		بازگشت error(_("failed to write multi-pack-index"));

	بازگشت 0;
}

ایستا صحیح multi_pack_index_expire(ساختار maintenance_run_opts *opts)
{
	ساختار child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", پوچ);

	اگر (opts->quiet)
		strvec_افزودن(&child.args, "--no-progress");
	وگرنه
		strvec_افزودن(&child.args, "--progress");

	اگر (run_command(&child))
		بازگشت error(_("'git multi-pack-index expire' failed"));

	بازگشت 0;
}

#define TWO_GIGABYTES (INT32_MAX)

ایستا off_t get_auto_pack_size(تهی)
{
	/*
	 * The "auto" value is special: we optimize for
	 * one large pack-file (i.e. from a clone) and
	 * expect the rest to be small and they can be
	 * repacked quickly.
	 *
	 * The strategy we select here is to select a
	 * size that is one more than the second largest
	 * pack-file. This ensures that we will repack
	 * at least two packs if there are three or more
	 * packs.
	 */
	off_t max_size = 0;
	off_t second_largest_size = 0;
	off_t result_size;
	ساختار packed_git *p;
	ساختار repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		اگر (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} وگرنه اگر (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	اگر (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	بازگشت result_size;
}

ایستا صحیح multi_pack_index_repack(ساختار maintenance_run_opts *opts)
{
	ساختار child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", پوچ);

	اگر (opts->quiet)
		strvec_افزودن(&child.args, "--no-progress");
	وگرنه
		strvec_افزودن(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	اگر (run_command(&child))
		بازگشت error(_("'git multi-pack-index repack' failed"));

	بازگشت 0;
}

ایستا صحیح maintenance_task_incremental_repack(ساختار maintenance_run_opts *opts,
					       ساختار gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	اگر (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		بازگشت 0;
	}

	اگر (multi_pack_index_write(opts))
		بازگشت 1;
	اگر (multi_pack_index_expire(opts))
		بازگشت 1;
	اگر (multi_pack_index_repack(opts))
		بازگشت 1;
	بازگشت 0;
}

ایستا صحیح maintenance_task_geometric_repack(ساختار maintenance_run_opts *opts,
					     ساختار gc_config *cfg)
{
	ساختار odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	اگر (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	بازگشت odb_optimize(the_repository->objects, &odb_opts);
}

ایستا صحیح geometric_repack_auto_condition(ساختار gc_config *cfg)
{
	ساختار odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	بازگشت odb_optimize_required(the_repository->objects, &opts);
}

تعریفنوع صحیح (*maintenance_task_fn)(ساختار maintenance_run_opts *opts,
				   ساختار gc_config *cfg);
تعریفنوع صحیح (*maintenance_auto_fn)(ساختار gc_config *cfg);

ساختار maintenance_task {
	ثابت نویسه *name;

	/*
	 * Work that will be executed before detaching. This should not include
	 * tasks that may run for an extended amount of time as it does cause
	 * auto-maintenance to block until foreground tasks have been run.
	 */
	maintenance_task_fn foreground;

	/*
	 * Work that will be executed after detaching. When not detaching the
	 * work will be run in the foreground, as well.
	 */
	maintenance_task_fn background;

	/*
	 * An auto condition function returns 1 if the task should run and 0 if
	 * the task should NOT run. See needs_to_gc() for an example.
	 */
	maintenance_auto_fn auto_condition;
};

ایستا ثابت ساختار maintenance_task tasks[] = {
	[TASK_PREFETCH] = {
		.name = "prefetch",
		.background = maintenance_task_prefetch,
	},
	[TASK_LOOSE_OBJECTS] = {
		.name = "loose-objects",
		.background = maintenance_task_loose_objects,
		.auto_condition = loose_object_auto_condition,
	},
	[TASK_INCREMENTAL_REPACK] = {
		.name = "incremental-repack",
		.background = maintenance_task_incremental_repack,
		.auto_condition = incremental_repack_auto_condition,
	},
	[TASK_GEOMETRIC_REPACK] = {
		.name = "geometric-repack",
		.background = maintenance_task_geometric_repack,
		.auto_condition = geometric_repack_auto_condition,
	},
	[TASK_GC] = {
		.name = "gc",
		.foreground = maintenance_task_gc_foreground,
		.background = maintenance_task_gc_background,
		.auto_condition = gc_condition,
	},
	[TASK_COMMIT_GRAPH] = {
		.name = "commit-graph",
		.background = maintenance_task_commit_graph,
		.auto_condition = should_write_commit_graph,
	},
	[TASK_PACK_REFS] = {
		.name = "pack-refs",
		.foreground = maintenance_task_pack_refs,
		.auto_condition = pack_refs_condition,
	},
	[TASK_REFLOG_EXPIRE] = {
		.name = "reflog-expire",
		.foreground = maintenance_task_reflog_expire,
		.auto_condition = reflog_expire_condition,
	},
	[TASK_WORKTREE_PRUNE] = {
		.name = "worktree-prune",
		.background = maintenance_task_worktree_prune,
		.auto_condition = worktree_prune_condition,
	},
	[TASK_RERERE_GC] = {
		.name = "rerere-gc",
		.background = maintenance_task_rerere_gc,
		.auto_condition = rerere_gc_condition,
	},
};

شمارش task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

شمارش auto_gc_hook_result {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

ایستا صحیح maybe_run_task(ثابت ساختار maintenance_task *task,
			  ساختار repository *repo,
			  ساختار maintenance_run_opts *opts,
			  ساختار gc_config *cfg,
			  شمارش task_phase phase,
			  شمارش auto_gc_hook_result *auto_gc_hook_result)
{
	صحیح foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ثابت نویسه *region = foreground ? "maintenance foreground" : "maintenance";
	صحیح ret = 0;

	اگر (!fn)
		بازگشت 0;
	اگر (opts->auto_flag) {
		اگر (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			بازگشت 0;

		اگر (!task->auto_condition || !task->auto_condition(cfg))
			بازگشت 0;

		اگر (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		اگر (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			بازگشت 0;
	}

	trace2_region_enter(region, task->name, repo);
	اگر (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	بازگشت ret;
}

ایستا صحیح maintenance_run_tasks(ساختار maintenance_run_opts *opts,
				 ساختار gc_config *cfg)
{
	صحیح result = 0;
	ساختار lock_file lk;
	ساختار repository *r = the_repository;
	نویسه *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	شمارش auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	اگر (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		اگر (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		آزاد(lock_path);
		بازگشت 0;
	}
	آزاد(lock_path);

	برای (size_t i = 0; i < opts->tasks_nr; i++)
		اگر (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	اگر (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	برای (size_t i = 0; i < opts->tasks_nr; i++)
		اگر (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	بازگشت result;
}

شمارش maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

ساختار maintenance_strategy {
	ساختار {
		unsigned type;
		شمارش schedule_priority schedule;
	} tasks[TASK_شمار];
};

ایستا ثابت ساختار maintenance_strategy none_strategy = { 0 };

ایستا ثابت ساختار maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

ایستا ثابت ساختار maintenance_strategy incremental_strategy = {
	.tasks = {
		[TASK_COMMIT_GRAPH] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_HOURLY,
		},
		[TASK_PREFETCH] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_HOURLY,
		},
		[TASK_INCREMENTAL_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
		[TASK_LOOSE_OBJECTS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_WEEKLY,
		},
		/*
		 * Historically, the "incremental" strategy was only available
		 * in the context of scheduled maintenance when set up via
		 * "maintenance.strategy". We have later expanded that config
		 * to also cover manual maintenance.
		 *
		 * To retain backwards compatibility with the previous status
		 * quo we thus run git-gc(1) in case manual maintenance was
		 * requested. This is the same as the default strategy, which
		 * would have been in use beforehand.
		 */
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL,
		},
	},
};

ایستا ثابت ساختار maintenance_strategy geometric_strategy = {
	.tasks = {
		[TASK_COMMIT_GRAPH] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.schedule = SCHEDULE_HOURLY,
		},
		[TASK_GEOMETRIC_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.schedule = SCHEDULE_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.schedule = SCHEDULE_DAILY,
		},
		[TASK_RERERE_GC] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.schedule = SCHEDULE_WEEKLY,
		},
		[TASK_REFLOG_EXPIRE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.schedule = SCHEDULE_WEEKLY,
		},
		[TASK_WORKTREE_PRUNE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.schedule = SCHEDULE_WEEKLY,
		},
	},
};

ایستا ساختار maintenance_strategy parse_maintenance_strategy(ثابت نویسه *name)
{
	اگر (!strcasecmp(name, "incremental"))
		بازگشت incremental_strategy;
	اگر (!strcasecmp(name, "gc"))
		بازگشت gc_strategy;
	اگر (!strcasecmp(name, "geometric"))
		بازگشت geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

ایستا تهی initialize_task_config(ساختار maintenance_run_opts *opts,
				   ثابت ساختار string_list *selected_tasks)
{
	ساختار strbuf config_name = STRBUF_INIT;
	ساختار maintenance_strategy strategy;
	شمارش maintenance_type type;
	ثابت نویسه *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	اگر (selected_tasks->nr) {
		برای (size_t i = 0; i < selected_tasks->nr; i++) {
			شمارش maintenance_task_label label = (intptr_t)selected_tasks->اقلام[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		بازگشت;
	}

	/*
	 * Otherwise, the strategy depends on whether we run as part of a
	 * scheduled job or not:
	 *
	 *   - Scheduled maintenance does not perform any housekeeping by
	 *     default, but requires the user to pick a maintenance strategy.
	 *
	 *   - Unscheduled maintenance uses our default strategy.
	 *
	 * Both of these are affected by the gitconfig though, which may
	 * override specific aspects of our strategy. Furthermore, both
	 * strategies can be overridden by setting "maintenance.strategy".
	 */
	اگر (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} وگرنه {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	اگر (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	برای (size_t i = 0; i < TASK_شمار; i++) {
		صحیح config_مقدار;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		اگر (!repo_config_get_bool(the_repository, config_name.buf, &config_مقدار))
			strategy.tasks[i].type = config_مقدار ? type : 0;
		اگر (!(strategy.tasks[i].type & type))
			ادامه;

		اگر (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			اگر (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			اگر (strategy.tasks[i].schedule < opts->schedule)
				ادامه;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

ایستا صحیح task_option_parse(ثابت ساختار option *opt,
			     ثابت نویسه *arg, صحیح unset)
{
	ساختار string_list *selected_tasks = opt->مقدار;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	برای (i = 0; i < TASK_شمار; i++)
		اگر (!strcasecmp(tasks[i].name, arg))
			بشکن;
	اگر (i >= TASK_شمار) {
		error(_("'%s' is not a valid task"), arg);
		بازگشت 1;
	}

	اگر (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		بازگشت 1;
	}

	string_list_append(selected_tasks, arg)->util = (تهی *)(intptr_t)i;

	بازگشت 0;
}

ایستا صحیح maintenance_run(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			   ساختار repository *repo UNUSED)
{
	ساختار maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ساختار string_list selected_tasks = STRING_LIST_INIT_DUP;
	ساختار gc_config cfg = GC_CONFIG_INIT;
	ساختار option builtin_maintenance_run_options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform maintenance in the background")),
		OPT_CALLBACK(0, "schedule", &opts.schedule, N_("frequency"),
			     N_("run tasks based on frequency"),
			     maintenance_opt_schedule),
		OPT_BOOL(0, "quiet", &opts.quiet,
			 N_("do not report progress or other information over stderr")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			N_("run a specific task"),
			PARSE_OPT_NONEG, task_option_parse),
		OPT_END()
	};
	صحیح ret;

	opts.quiet = !isatty(2);

	argc = parse_options(argc, argv, prefix,
			     builtin_maintenance_run_options,
			     builtin_maintenance_run_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	die_for_incompatible_opt2(opts.auto_flag, "--auto",
				  opts.schedule, "--schedule=");
	die_for_incompatible_opt2(selected_tasks.nr, "--task=",
				  opts.schedule, "--schedule=");

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	اگر (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	بازگشت ret;
}

ایستا نویسه *get_maintpath(تهی)
{
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	بازگشت strbuf_detach(&sb, پوچ);
}

ایستا نویسه ثابت * ثابت builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	پوچ
};

ایستا صحیح maintenance_register(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				ساختار repository *repo UNUSED)
{
	نویسه *config_file = پوچ;
	ساختار option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	صحیح found = 0;
	ثابت نویسه *کلید = "maintenance.repo";
	نویسه *maintpath = get_maintpath();
	ساختار string_list_item *item;
	ثابت ساختار string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	اگر (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	اگر (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	اگر (!repo_config_get_string_multi(the_repository, کلید, &list)) {
		for_each_string_list_item(item, list) {
			اگر (!strcmp(maintpath, item->string)) {
				found = 1;
				بشکن;
			}
		}
	}

	اگر (!found) {
		صحیح rc;
		نویسه *global_config_file = پوچ;

		اگر (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		اگر (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, پوچ, 0);
		آزاد(global_config_file);

		اگر (rc)
			die(_("unable to add '%s' value of '%s'"),
			    کلید, maintpath);
	}

	آزاد(maintpath);
	بازگشت 0;
}

ایستا نویسه ثابت * ثابت builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	پوچ
};

ایستا صحیح maintenance_unregister(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				  ساختار repository *repo UNUSED)
{
	صحیح force = 0;
	نویسه *config_file = پوچ;
	ساختار option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	ثابت نویسه *کلید = "maintenance.repo";
	نویسه *maintpath = get_maintpath();
	صحیح found = 0;
	ساختار string_list_item *item;
	ثابت ساختار string_list *list;
	ساختار config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	اگر (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	اگر (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	اگر (!(config_file
	      ? git_configset_get_string_multi(&cs, کلید, &list)
	      : repo_config_get_string_multi(the_repository, کلید, &list))) {
		for_each_string_list_item(item, list) {
			اگر (!strcmp(maintpath, item->string)) {
				found = 1;
				بشکن;
			}
		}
	}

	اگر (found) {
		صحیح rc;
		نویسه *global_config_file = پوچ;

		اگر (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		اگر (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, کلید, پوچ, maintpath, پوچ,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_مقدار);
		آزاد(global_config_file);

		اگر (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    کلید, maintpath);
	} وگرنه اگر (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	آزاد(maintpath);
	بازگشت 0;
}

ایستا ثابت نویسه *get_frequency(شمارش schedule_priority schedule)
{
	انتخاب (schedule) {
	حالت SCHEDULE_HOURLY:
		بازگشت "hourly";
	حالت SCHEDULE_DAILY:
		بازگشت "daily";
	حالت SCHEDULE_WEEKLY:
		بازگشت "weekly";
	پیشفرض:
		BUG("invalid schedule %d", schedule);
	}
}

ایستا ثابت نویسه *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	پوچ
};

ایستا ثابت نویسه *get_extra_config_parameters(تهی) {
	ایستا ثابت نویسه *result = پوچ;
	ساختار strbuf builder = STRBUF_INIT;

	اگر (result)
		بازگشت result;

	برای (ثابت نویسه **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, پوچ);
	بازگشت result;
}

ایستا ثابت نویسه *get_extra_launchctl_strings(تهی) {
	ایستا ثابت نویسه *result = پوچ;
	ساختار strbuf builder = STRBUF_INIT;

	اگر (result)
		بازگشت result;

	برای (ثابت نویسه **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, پوچ);
	بازگشت result;
}

/*
 * get_schedule_cmd` reads the GIT_TEST_MAINT_SCHEDULER environment variable
 * to mock the schedulers that `git maintenance start` rely on.
 *
 * For test purpose, GIT_TEST_MAINT_SCHEDULER can be set to a comma-separated
 * list of colon-separated key/value pairs where each pair contains a scheduler
 * and its corresponding mock.
 *
 * * If $GIT_TEST_MAINT_SCHEDULER is not set, return false and leave the
 *   arguments unmodified.
 *
 * * If $GIT_TEST_MAINT_SCHEDULER is set, return true.
 *   In this case, the *cmd value is read as input.
 *
 *   * if the input value cmd is the key of one of the comma-separated list
 *     item, then *is_available is set to true and *out is set to
 *     the mock command.
 *
 *   * if the input value *cmd isn’t the key of any of the comma-separated list
 *     item, then *is_available is set to false and *out is set to the original
 *     command.
 *
 * Ex.:
 *   GIT_TEST_MAINT_SCHEDULER not set
 *     +-------+-------------------------------------------------+
 *     | Input |                     Output                      |
 *     | *cmd  | return code |       *out        | *is_available |
 *     +-------+-------------+-------------------+---------------+
 *     | "foo" |    false    | "foo" (allocated) |  (unchanged)  |
 *     +-------+-------------+-------------------+---------------+
 *
 *   GIT_TEST_MAINT_SCHEDULER set to “foo:./mock_foo.sh,bar:./mock_bar.sh”
 *     +-------+-------------------------------------------------+
 *     | Input |                     Output                      |
 *     | *cmd  | return code |       *out        | *is_available |
 *     +-------+-------------+-------------------+---------------+
 *     | "foo" |    true     |  "./mock.foo.sh"  |     true      |
 *     | "qux" |    true     | "qux" (allocated) |     false     |
 *     +-------+-------------+-------------------+---------------+
 */
ایستا صحیح get_schedule_cmd(ثابت نویسه *cmd, صحیح *is_available, نویسه **out)
{
	نویسه *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	ساختار string_list_item *item;
	ساختار string_list list = STRING_LIST_INIT_NODUP;

	اگر (!testing) {
		اگر (out)
			*out = xstrdup(cmd);
		بازگشت 0;
	}

	اگر (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		ساختار string_list pair = STRING_LIST_INIT_NODUP;

		اگر (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			ادامه;

		اگر (!strcmp(cmd, pair.اقلام[0].string)) {
			اگر (out)
				*out = xstrdup(pair.اقلام[1].string);
			اگر (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			برو out;
		}

		string_list_clear(&pair, 0);
	}

	اگر (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	آزاد(testing);
	بازگشت 1;
}

ایستا صحیح get_random_minute(تهی)
{
	/* Use a static value when under tests. */
	اگر (getenv("GIT_TEST_MAINT_SCHEDULER"))
		بازگشت 13;

	بازگشت git_rand(0) % 60;
}

ایستا صحیح is_launchctl_available(تهی)
{
	صحیح is_available;
	اگر (get_schedule_cmd("launchctl", &is_available, پوچ))
		بازگشت is_available;

#ifdef __APPLE__
	بازگشت 1;
#وگرنه
	بازگشت 0;
#endif
}

ایستا نویسه *launchctl_service_name(ثابت نویسه *frequency)
{
	ساختار strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	بازگشت strbuf_detach(&label, پوچ);
}

ایستا نویسه *launchctl_service_filename(ثابت نویسه *name)
{
	نویسه *expanded;
	ساختار strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	اگر (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	بازگشت expanded;
}

ایستا نویسه *launchctl_get_uid(تهی)
{
	بازگشت xstrfmt("gui/%d", getuid());
}

ایستا صحیح launchctl_boot_plist(صحیح enable, ثابت نویسه *filename)
{
	نویسه *cmd;
	صحیح result;
	ساختار child_process child = CHILD_PROCESS_INIT;
	نویسه *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", پوچ, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, پوچ);

	child.no_stderr = 1;
	child.no_stdout = 1;

	اگر (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	آزاد(cmd);
	آزاد(uid);
	بازگشت result;
}

ایستا صحیح launchctl_remove_plist(شمارش schedule_priority schedule)
{
	ثابت نویسه *frequency = get_frequency(schedule);
	نویسه *name = launchctl_service_name(frequency);
	نویسه *filename = launchctl_service_filename(name);
	صحیح result = launchctl_boot_plist(0, filename);
	unlink(filename);
	آزاد(filename);
	آزاد(name);
	بازگشت result;
}

ایستا صحیح launchctl_remove_plists(تهی)
{
	بازگشت launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

ایستا صحیح launchctl_list_contains_plist(ثابت نویسه *name, ثابت نویسه *cmd)
{
	ساختار child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, پوچ);

	child.no_stderr = 1;
	child.no_stdout = 1;

	اگر (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	بازگشت !finish_command(&child);
}

ایستا صحیح launchctl_schedule_plist(ثابت نویسه *exec_path, شمارش schedule_priority schedule)
{
	صحیح i, fd;
	ثابت نویسه *preamble, *repeat;
	ثابت نویسه *frequency = get_frequency(schedule);
	نویسه *name = launchctl_service_name(frequency);
	نویسه *filename = launchctl_service_filename(name);
	ساختار lock_file lk = LOCK_INIT;
	ایستا unsigned long lock_file_timeout_ms = ULONG_MAX;
	ساختار strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	ساختار stat st;
	نویسه *cmd;
	صحیح minute = get_random_minute();

	get_schedule_cmd("launchctl", پوچ, &cmd);
	preamble = "<?xml version=\"1.0\"?>\n"
		   "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
		   "<plist version=\"1.0\">"
		   "<dict>\n"
		   "<key>Label</key><string>%s</string>\n"
		   "<key>ProgramArguments</key>\n"
		   "<array>\n"
		   "<string>%s/git</string>\n"
		   "<string>--exec-path=%s</string>\n"
		   "%s" /* For extra config parameters. */
		   "<string>for-each-repo</string>\n"
		   "<string>--keep-going</string>\n"
		   "<string>--config=maintenance.repo</string>\n"
		   "<string>maintenance</string>\n"
		   "<string>run</string>\n"
		   "<string>--schedule=%s</string>\n"
		   "</array>\n"
		   "<key>StartCalendarInterval</key>\n"
		   "<array>\n";
	strbuf_addf(&plist, preamble, name, exec_path, exec_path,
		    get_extra_launchctl_strings(), frequency);

	انتخاب (schedule) {
	حالت SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		برای (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		بشکن;

	حالت SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		برای (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		بشکن;

	حالت SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		بشکن;

	پیشفرض:
		/* unreachable */
		بشکن;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	اگر (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	اگر ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	اگر (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	وگرنه {
		اگر (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		اگر (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	آزاد(filename);
	آزاد(name);
	آزاد(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	بازگشت 0;
}

ایستا صحیح launchctl_add_plists(تهی)
{
	ثابت نویسه *exec_path = git_exec_path();

	بازگشت launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

ایستا صحیح launchctl_update_schedule(صحیح run_maintenance, صحیح fd UNUSED)
{
	اگر (run_maintenance)
		بازگشت launchctl_add_plists();
	وگرنه
		بازگشت launchctl_remove_plists();
}

ایستا صحیح is_schtasks_available(تهی)
{
	صحیح is_available;
	اگر (get_schedule_cmd("schtasks", &is_available, پوچ))
		بازگشت is_available;

#ifdef GIT_WINDOWS_NATIVE
	بازگشت 1;
#وگرنه
	بازگشت 0;
#endif
}

ایستا نویسه *schtasks_task_name(ثابت نویسه *frequency)
{
	ساختار strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	بازگشت strbuf_detach(&label, پوچ);
}

ایستا صحیح schtasks_remove_task(شمارش schedule_priority schedule)
{
	نویسه *cmd;
	ساختار child_process child = CHILD_PROCESS_INIT;
	ثابت نویسه *frequency = get_frequency(schedule);
	نویسه *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", پوچ, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", پوچ);
	آزاد(name);
	آزاد(cmd);

	بازگشت run_command(&child);
}

ایستا صحیح schtasks_remove_tasks(تهی)
{
	بازگشت schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

ایستا صحیح schtasks_schedule_task(ثابت نویسه *exec_path, شمارش schedule_priority schedule)
{
	نویسه *cmd;
	صحیح result;
	ساختار child_process child = CHILD_PROCESS_INIT;
	ثابت نویسه *xml;
	ساختار tempfile *tfile;
	ثابت نویسه *frequency = get_frequency(schedule);
	نویسه *name = schtasks_task_name(frequency);
	ساختار strbuf tfilename = STRBUF_INIT;
	صحیح minute = get_random_minute();

	get_schedule_cmd("schtasks", پوچ, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	اگر (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	انتخاب (schedule) {
	حالت SCHEDULE_HOURLY:
		fprintf(tfile->fp,
			"<StartBoundary>2020-01-01T01:%02d:00</StartBoundary>\n"
			"<Enabled>true</Enabled>\n"
			"<ScheduleByDay>\n"
			"<DaysInterval>1</DaysInterval>\n"
			"</ScheduleByDay>\n"
			"<Repetition>\n"
			"<Interval>PT1H</Interval>\n"
			"<Duration>PT23H</Duration>\n"
			"<StopAtDurationEnd>false</StopAtDurationEnd>\n"
			"</Repetition>\n",
			minute);
		بشکن;

	حالت SCHEDULE_DAILY:
		fprintf(tfile->fp,
			"<StartBoundary>2020-01-01T00:%02d:00</StartBoundary>\n"
			"<Enabled>true</Enabled>\n"
			"<ScheduleByWeek>\n"
			"<DaysOfWeek>\n"
			"<Monday />\n"
			"<Tuesday />\n"
			"<Wednesday />\n"
			"<Thursday />\n"
			"<Friday />\n"
			"<Saturday />\n"
			"</DaysOfWeek>\n"
			"<WeeksInterval>1</WeeksInterval>\n"
			"</ScheduleByWeek>\n",
			minute);
		بشکن;

	حالت SCHEDULE_WEEKLY:
		fprintf(tfile->fp,
			"<StartBoundary>2020-01-01T00:%02d:00</StartBoundary>\n"
			"<Enabled>true</Enabled>\n"
			"<ScheduleByWeek>\n"
			"<DaysOfWeek>\n"
			"<Sunday />\n"
			"</DaysOfWeek>\n"
			"<WeeksInterval>1</WeeksInterval>\n"
			"</ScheduleByWeek>\n",
			minute);
		بشکن;

	پیشفرض:
		بشکن;
	}

	xml = "</CalendarTrigger>\n"
	      "</Triggers>\n"
	      "<Principals>\n"
	      "<Principal id=\"Author\">\n"
	      "<LogonType>InteractiveToken</LogonType>\n"
	      "<RunLevel>LeastPrivilege</RunLevel>\n"
	      "</Principal>\n"
	      "</Principals>\n"
	      "<Settings>\n"
	      "<MultipleInstancesPolicy>IgnoreNew</MultipleInstancesPolicy>\n"
	      "<Enabled>true</Enabled>\n"
	      "<Hidden>true</Hidden>\n"
	      "<UseUnifiedSchedulingEngine>true</UseUnifiedSchedulingEngine>\n"
	      "<WakeToRun>false</WakeToRun>\n"
	      "<ExecutionTimeLimit>PT72H</ExecutionTimeLimit>\n"
	      "<Priority>7</Priority>\n"
	      "</Settings>\n"
	      "<Actions Context=\"Author\">\n"
	      "<Exec>\n"
	      "<Command>\"%s\\headless-git.exe\"</Command>\n"
	      "<Arguments>--exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%s</Arguments>\n"
	      "</Exec>\n"
	      "</Actions>\n"
	      "</Task>\n";
	fprintf(tfile->fp, xml, exec_path, exec_path,
		get_extra_config_parameters(), frequency);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/create", "/tn", name, "/f", "/xml",
				  get_tempfile_path(tfile), پوچ);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	اگر (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	آزاد(name);
	آزاد(cmd);
	بازگشت result;
}

ایستا صحیح schtasks_schedule_tasks(تهی)
{
	ثابت نویسه *exec_path = git_exec_path();

	بازگشت schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

ایستا صحیح schtasks_update_schedule(صحیح run_maintenance, صحیح fd UNUSED)
{
	اگر (run_maintenance)
		بازگشت schtasks_schedule_tasks();
	وگرنه
		بازگشت schtasks_remove_tasks();
}

MAYBE_UNUSED
ایستا صحیح check_crontab_process(ثابت نویسه *cmd)
{
	ساختار child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_افزودن(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	اگر (start_command(&child))
		بازگشت 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	بازگشت 1;
}

ایستا صحیح is_crontab_available(تهی)
{
	نویسه *cmd;
	صحیح is_available;
	صحیح ret;

	اگر (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		برو out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#وگرنه
	ret = check_crontab_process(cmd);
#endif

out:
	آزاد(cmd);
	بازگشت ret;
}

#define BEGIN_سطر "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_سطر "# END GIT MAINTENANCE SCHEDULE"

ایستا صحیح crontab_update_schedule(صحیح run_maintenance, صحیح fd)
{
	نویسه *cmd;
	صحیح result = 0;
	صحیح in_old_region = 0;
	ساختار child_process crontab_list = CHILD_PROCESS_INIT;
	ساختار child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	ساختار strbuf سطر = STRBUF_INIT;
	ساختار tempfile *tmpedit = پوچ;
	صحیح minute = get_random_minute();

	get_schedule_cmd("crontab", پوچ, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_افزودن(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	اگر (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		برو out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	اگر (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		برو out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	اگر (!cron_in) {
		result = error(_("failed to open temporary file"));
		برو out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	وقتی (!strbuf_getline_lf(&سطر, cron_list)) {
		اگر (!in_old_region && !strcmp(سطر.buf, BEGIN_سطر))
			in_old_region = 1;
		وگرنه اگر (in_old_region && !strcmp(سطر.buf, END_سطر))
			in_old_region = 0;
		وگرنه اگر (!in_old_region)
			fprintf(cron_in, "%s\n", سطر.buf);
	}
	strbuf_release(&سطر);

	اگر (run_maintenance) {
		ساختار strbuf سطر_format = STRBUF_INIT;
		ثابت نویسه *exec_path = git_exec_path();

		fprintf(cron_in, "%s\n", BEGIN_سطر);
		fprintf(cron_in,
			"# The following schedule was created by Git\n");
		fprintf(cron_in, "# Any edits made in this region might be\n");
		fprintf(cron_in,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&سطر_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_path, exec_path, get_extra_config_parameters());
		fprintf(cron_in, سطر_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_in, سطر_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_in, سطر_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&سطر_format);

		fprintf(cron_in, "\n%s\n", END_سطر);
	}

	fflush(cron_in);

	strvec_split(&crontab_edit.args, cmd);
	strvec_افزودن(&crontab_edit.args, get_tempfile_path(tmpedit));
	crontab_edit.git_cmd = 0;

	اگر (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		برو out;
	}

	اگر (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	وگرنه
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	آزاد(cmd);
	بازگشت result;
}

ایستا صحیح real_is_systemd_timer_available(تهی)
{
	ساختار child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", پوچ);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	اگر (start_command(&child))
		بازگشت 0;
	اگر (finish_command(&child))
		بازگشت 0;
	بازگشت 1;
}

ایستا صحیح is_systemd_timer_available(تهی)
{
	صحیح is_available;

	اگر (get_schedule_cmd("systemctl", &is_available, پوچ))
		بازگشت is_available;

	بازگشت real_is_systemd_timer_available();
}

ایستا نویسه *xdg_config_home_systemd(ثابت نویسه *filename)
{
	بازگشت xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

ایستا صحیح systemd_timer_delete_timer_file(شمارش schedule_priority priority)
{
	صحیح ret = 0;
	ثابت نویسه *frequency = get_frequency(priority);
	نویسه *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	نویسه *filename = xdg_config_home_systemd(local_timer_name);

	اگر (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	آزاد(filename);
	آزاد(local_timer_name);
	بازگشت ret;
}

ایستا صحیح systemd_timer_delete_service_template(تهی)
{
	صحیح ret = 0;
	نویسه *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	نویسه *filename = xdg_config_home_systemd(local_service_name);
	اگر (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	آزاد(filename);
	آزاد(local_service_name);
	بازگشت ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
ایستا صحیح systemd_timer_write_timer_file(شمارش schedule_priority schedule,
					  صحیح minute)
{
	صحیح res = -1;
	نویسه *filename;
	FILE *file;
	ثابت نویسه *unit;
	نویسه *schedule_pattern = پوچ;
	ثابت نویسه *frequency = get_frequency(schedule);
	نویسه *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	اگر (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		برو error;
	}
	file = fopen_or_warn(filename, "w");
	اگر (!file)
		برو error;

	انتخاب (schedule) {
	حالت SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		بشکن;

	حالت SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		بشکن;

	حالت SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		بشکن;

	پیشفرض:
		BUG("Unhandled schedule_priority");
	}

	unit = "# This file was created and is maintained by Git.\n"
	       "# Any edits made in this file might be replaced in the future\n"
	       "# by a Git command.\n"
	       "\n"
	       "[Unit]\n"
	       "Description=Optimize Git repositories data\n"
	       "\n"
	       "[Timer]\n"
	       "OnCalendar=%s\n"
	       "Persistent=true\n"
	       "\n"
	       "[Install]\n"
	       "WantedBy=timers.target\n";
	اگر (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		برو error;
	}
	اگر (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		برو error;
	}

	res = 0;

error:
	آزاد(schedule_pattern);
	آزاد(local_timer_name);
	آزاد(filename);
	بازگشت res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
ایستا صحیح systemd_timer_write_service_template(ثابت نویسه *exec_path)
{
	صحیح res = -1;
	نویسه *filename;
	FILE *file;
	ثابت نویسه *unit;
	نویسه *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	اگر (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		برو error;
	}
	file = fopen_or_warn(filename, "w");
	اگر (!file)
		برو error;

	unit = "# This file was created and is maintained by Git.\n"
	       "# Any edits made in this file might be replaced in the future\n"
	       "# by a Git command.\n"
	       "\n"
	       "[Unit]\n"
	       "Description=Optimize Git repositories data\n"
	       "\n"
	       "[Service]\n"
	       "Type=oneshot\n"
	       "ExecStart=\"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%i\n"
	       "LockPersonality=yes\n"
	       "MemoryDenyWriteExecute=yes\n"
	       "NoNewPrivileges=yes\n"
	       "RestrictAddressFamilies=AF_UNIX AF_INET AF_INET6 AF_VSOCK\n"
	       "RestrictNamespaces=yes\n"
	       "RestrictRealtime=yes\n"
	       "RestrictSUIDSGID=yes\n"
	       "SystemCallArchitectures=native\n"
	       "SystemCallFilter=@system-service\n";
	اگر (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		برو error;
	}
	اگر (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		برو error;
	}

	res = 0;

error:
	آزاد(local_service_name);
	آزاد(filename);
	بازگشت res;
}

ایستا صحیح systemd_timer_enable_unit(صحیح enable,
				     شمارش schedule_priority schedule,
				     صحیح minute)
{
	نویسه *cmd = پوچ;
	ساختار child_process child = CHILD_PROCESS_INIT;
	ثابت نویسه *frequency = get_frequency(schedule);
	صحیح ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	اگر (!enable) {
		child.no_stderr = 1;
	} وگرنه اگر (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		برو out;
	}

	get_schedule_cmd("systemctl", پوچ, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", پوچ);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	اگر (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		برو out;
	}

	اگر (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		اگر (enable) {
			ret = error(_("failed to run systemctl"));
			برو out;
		}
	}

	ret = 0;

out:
	آزاد(cmd);
	بازگشت ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
ایستا تهی systemd_timer_delete_stale_timer_templates(تهی)
{
	نویسه *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	نویسه *filename = xdg_config_home_systemd(timer_template_name);

	اگر (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	آزاد(filename);
	آزاد(timer_template_name);
}

ایستا صحیح systemd_timer_delete_unit_files(تهی)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	بازگشت systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

ایستا صحیح systemd_timer_delete_units(تهی)
{
	صحیح minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	بازگشت systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

ایستا صحیح systemd_timer_setup_units(تهی)
{
	صحیح minute = get_random_minute();
	ثابت نویسه *exec_path = git_exec_path();

	صحیح ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	اگر (ret)
		systemd_timer_delete_units();
	وگرنه
		systemd_timer_delete_stale_timer_templates();

	بازگشت ret;
}

ایستا صحیح systemd_timer_update_schedule(صحیح run_maintenance, صحیح fd UNUSED)
{
	اگر (run_maintenance)
		بازگشت systemd_timer_setup_units();
	وگرنه
		بازگشت systemd_timer_delete_units();
}

شمارش scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

ایستا ثابت ساختار {
	ثابت نویسه *name;
	صحیح (*is_available)(تهی);
	صحیح (*update_schedule)(صحیح run_maintenance, صحیح fd);
} scheduler_fn[] = {
	[SCHEDULER_CRON] = {
		.name = "crontab",
		.is_available = is_crontab_available,
		.update_schedule = crontab_update_schedule,
	},
	[SCHEDULER_SYSTEMD] = {
		.name = "systemctl",
		.is_available = is_systemd_timer_available,
		.update_schedule = systemd_timer_update_schedule,
	},
	[SCHEDULER_LAUNCHCTL] = {
		.name = "launchctl",
		.is_available = is_launchctl_available,
		.update_schedule = launchctl_update_schedule,
	},
	[SCHEDULER_SCHTASKS] = {
		.name = "schtasks",
		.is_available = is_schtasks_available,
		.update_schedule = schtasks_update_schedule,
	},
};

ایستا شمارش scheduler parse_scheduler(ثابت نویسه *مقدار)
{
	اگر (!مقدار)
		بازگشت SCHEDULER_INVALID;
	وگرنه اگر (!strcasecmp(مقدار, "auto"))
		بازگشت SCHEDULER_AUTO;
	وگرنه اگر (!strcasecmp(مقدار, "cron") || !strcasecmp(مقدار, "crontab"))
		بازگشت SCHEDULER_CRON;
	وگرنه اگر (!strcasecmp(مقدار, "systemd") ||
		 !strcasecmp(مقدار, "systemd-timer"))
		بازگشت SCHEDULER_SYSTEMD;
	وگرنه اگر (!strcasecmp(مقدار, "launchctl"))
		بازگشت SCHEDULER_LAUNCHCTL;
	وگرنه اگر (!strcasecmp(مقدار, "schtasks"))
		بازگشت SCHEDULER_SCHTASKS;
	وگرنه
		بازگشت SCHEDULER_INVALID;
}

ایستا صحیح maintenance_opt_scheduler(ثابت ساختار option *opt, ثابت نویسه *arg,
				     صحیح unset)
{
	شمارش scheduler *scheduler = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	اگر (*scheduler == SCHEDULER_INVALID)
		بازگشت error(_("unrecognized --scheduler argument '%s'"), arg);
	بازگشت 0;
}

ساختار maintenance_start_opts {
	شمارش scheduler scheduler;
};

ایستا شمارش scheduler resolve_scheduler(شمارش scheduler scheduler)
{
	اگر (scheduler != SCHEDULER_AUTO)
		بازگشت scheduler;

#اگر defined(__APPLE__)
	بازگشت SCHEDULER_LAUNCHCTL;

#وگرنهاگر defined(GIT_WINDOWS_NATIVE)
	بازگشت SCHEDULER_SCHTASKS;

#وگرنهاگر defined(__linux__)
	اگر (is_systemd_timer_available())
		بازگشت SCHEDULER_SYSTEMD;
	وگرنه اگر (is_crontab_available())
		بازگشت SCHEDULER_CRON;
	وگرنه
		die(_("neither systemd timers nor crontab are available"));

#وگرنه
	بازگشت SCHEDULER_CRON;
#endif
}

ایستا تهی validate_scheduler(شمارش scheduler scheduler)
{
	اگر (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	اگر (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	اگر (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

ایستا صحیح update_background_schedule(ثابت ساختار maintenance_start_opts *opts,
				      صحیح enable)
{
	unsigned صحیح i;
	صحیح result = 0;
	ساختار lock_file lk;
	نویسه *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	اگر (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		اگر (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		وگرنه
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		آزاد(lock_path);
		بازگشت -1;
	}

	برای (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		اگر (enable && opts->scheduler == i)
			ادامه;
		اگر (!scheduler_fn[i].is_available())
			ادامه;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	اگر (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	آزاد(lock_path);
	بازگشت result;
}

ایستا ثابت نویسه *ثابت builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	پوچ
};

ایستا صحیح maintenance_start(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			     ساختار repository *repo)
{
	ساختار maintenance_start_opts opts = { 0 };
	ساختار option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	ثابت نویسه *register_args[] = { "register", پوچ };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	اگر (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	اگر (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	اگر (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, پوچ, repo))
		warning(_("failed to add repo to global config"));
	بازگشت 0;
}

ایستا ثابت نویسه *ثابت builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	پوچ
};

ایستا صحیح maintenance_stop(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			    ساختار repository *repo UNUSED)
{
	ساختار option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	اگر (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	بازگشت update_background_schedule(پوچ, 0);
}

ایستا ثابت نویسه *ثابت builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	پوچ
};

ایستا صحیح maintenance_is_needed(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				 ساختار repository *repo UNUSED)
{
	ساختار maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ساختار string_list selected_tasks = STRING_LIST_INIT_DUP;
	ساختار gc_config cfg = GC_CONFIG_INIT;
	ساختار option options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			       N_("check a specific task"),
			       PARSE_OPT_NONEG, task_option_parse),
		OPT_END()
	};
	bool is_needed = false;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_is_needed_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	اگر (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	اگر (opts.auto_flag) {
		برای (size_t i = 0; i < opts.tasks_nr; i++) {
			اگر (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				بشکن;
			}
		}
	} وگرنه {
		/*
		 * When not using --auto we always require maintenance right now.
		 *
		 * TODO: this certainly is too eager, as some maintenance tasks may
		 * decide to not do anything because the data structures are already
		 * fully optimized. We may eventually want to extend the auto
		 * condition to also cover non-auto runs so that we can detect such
		 * cases.
		 */
		is_needed = true;
	}

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);

	اگر (is_needed)
		بازگشت 0;
	بازگشت 1;
}

ایستا ثابت نویسه *ثابت builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	پوچ,
};

صحیح cmd_maintenance(صحیح argc,
		    ثابت نویسه **argv,
		    ثابت نویسه *prefix,
		    ساختار repository *repo)
{
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option builtin_maintenance_options[] = {
		OPT_SUBCOMMAND("run", &fn, maintenance_run),
		OPT_SUBCOMMAND("start", &fn, maintenance_start),
		OPT_SUBCOMMAND("stop", &fn, maintenance_stop),
		OPT_SUBCOMMAND("register", &fn, maintenance_register),
		OPT_SUBCOMMAND("unregister", &fn, maintenance_unregister),
		OPT_SUBCOMMAND("is-needed", &fn, maintenance_is_needed),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, builtin_maintenance_options,
			     builtin_maintenance_usage, 0);
	بازگشت fn(argc, argv, prefix, repo);
}
