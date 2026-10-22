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

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "date.h"
#ضمن "dir.h"
#ضمن "environment.h"
#ضمن "hex.h"
#ضمن "config.h"
#ضمن "tempfile.h"
#ضمن "lockfile.h"
#ضمن "parse-options.h"
#ضمن "run-command.h"
#ضمن "sigchain.h"
#ضمن "strvec.h"
#ضمن "commit.h"
#ضمن "commit-graph.h"
#ضمن "packfile.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "path.h"
#ضمن "reflog.h"
#ضمن "rerere.h"
#ضمن "revision.h"
#ضمن "refs.h"
#ضمن "remote.h"
#ضمن "exec-cmd.h"
#ضمن "gettext.h"
#ضمن "hook.h"
#ضمن "setup.h"
#ضمن "trace2.h"
#ضمن "worktree.h"

#define FAILED_RUN "failed to run %s"

ساكن ثابت حرف * ثابت builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	عدم
};

ساكن timestamp_t gc_log_expire_time;
ساكن بنية tempfile *pidfile;
ساكن بنية lock_file log_lock;
ساكن بنية string_list pack_garbage = STRING_LIST_INIT_DUP;

ساكن فراغ clean_pack_garbage(فراغ)
{
	صحيح i;
	لكل (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.items[i].string);
	string_list_clear(&pack_garbage, 0);
}

ساكن فراغ report_pack_garbage(unsigned seen_bits, ثابت حرف *path)
{
	𐡄𐡍 (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

ساكن فراغ process_log_file(فراغ)
{
	بنية stat st;
	𐡄𐡍 (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		صحيح saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} وإلا 𐡄𐡍 (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} وإلا {
		حرف *path = repo_git_path(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(path);
		rollback_lock_file(&log_lock);
		حرر(path);
	}
}

ساكن فراغ process_log_file_at_exit(فراغ)
{
	fflush(stderr);
	process_log_file();
}

ساكن صحيح gc_config_is_timestamp_never(ثابت حرف *var)
{
	ثابت حرف *value;
	timestamp_t expire;

	𐡄𐡍 (!repo_config_get_value(the_repository, var, &value) && value) {
		𐡄𐡍 (parse_expiry_date(value, &expire))
			die(_("failed to parse '%s' value '%s'"), var, value);
		أرجع expire == 0;
	}
	أرجع 0;
}

بنية gc_config {
	صحيح pack_refs;
	صحيح prune_reflogs;
	صحيح cruft_packs;
	unsigned long max_cruft_size;
	صحيح aggressive_depth;
	صحيح aggressive_window;
	صحيح detach_auto;
	حرف *gc_log_expire;
	حرف *prune_expire;
	حرف *prune_worktrees_expire;
	حرف *repack_expire_to;
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

ساكن فراغ gc_config_release(بنية gc_config *cfg)
{
	حرر(cfg->gc_log_expire);
	حرر(cfg->prune_expire);
	حرر(cfg->prune_worktrees_expire);
}

ساكن فراغ gc_config(بنية gc_config *cfg)
{
	ثابت حرف *value;
	حرف *owned = عدم;

	𐡄𐡍 (!repo_config_get_value(the_repository, "gc.packrefs", &value)) {
		𐡄𐡍 (value && !strcmp(value, "notbare"))
			cfg->pack_refs = -1;
		وإلا
			cfg->pack_refs = git_config_bool("gc.packrefs", value);
	}

	𐡄𐡍 (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	𐡄𐡍 (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		حرر(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	𐡄𐡍 (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		حرر(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	𐡄𐡍 (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		حرر(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, عدم);
}

تعداد schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

ساكن تعداد schedule_priority parse_schedule(ثابت حرف *value)
{
	𐡄𐡍 (!value)
		أرجع SCHEDULE_NONE;
	𐡄𐡍 (!strcasecmp(value, "hourly"))
		أرجع SCHEDULE_HOURLY;
	𐡄𐡍 (!strcasecmp(value, "daily"))
		أرجع SCHEDULE_DAILY;
	𐡄𐡍 (!strcasecmp(value, "weekly"))
		أرجع SCHEDULE_WEEKLY;
	أرجع SCHEDULE_NONE;
}

تعداد maintenance_task_label {
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
	TASK__COUNT
};

بنية maintenance_run_opts {
	تعداد maintenance_task_label *tasks;
	size_t tasks_nr, tasks_alloc;
	صحيح auto_flag;
	صحيح detach;
	صحيح quiet;
	تعداد schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

ساكن فراغ maintenance_run_opts_release(بنية maintenance_run_opts *opts)
{
	حرر(opts->tasks);
}

ساكن صحيح pack_refs_condition(UNUSED بنية gc_config *cfg)
{
	بنية string_list included_refs = STRING_LIST_INIT_NODUP;
	بنية ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	بنية refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	𐡄𐡍 (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		أرجع 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	أرجع required;
}

ساكن صحيح maintenance_task_pack_refs(بنية maintenance_run_opts *opts,
				      UNUSED بنية gc_config *cfg)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", عدم);
	𐡄𐡍 (opts->auto_flag)
		strvec_push(&cmd.args, "--auto");

	أرجع run_command(&cmd);
}

بنية count_reflog_entries_data {
	بنية expire_reflog_policy_cb policy;
	size_t count;
	size_t limit;
};

ساكن صحيح count_reflog_entries(ثابت حرف *refname UNUSED,
				بنية object_id *old_oid, بنية object_id *new_oid,
				ثابت حرف *committer, timestamp_t timestamp,
				صحيح tz, ثابت حرف *msg, فراغ *cb_data)
{
	بنية count_reflog_entries_data *data = cb_data;
	𐡄𐡍 (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, msg, &data->policy))
		data->count++;
	أرجع data->count >= data->limit;
}

ساكن صحيح reflog_expire_condition(بنية gc_config *cfg UNUSED)
{
	timestamp_t now = time(عدم);
	بنية count_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	صحيح limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	𐡄𐡍 (!limit)
		أرجع 0;
	𐡄𐡍 (limit < 0)
		أرجع 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 count_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	أرجع data.count >= data.limit;
}

ساكن صحيح maintenance_task_reflog_expire(بنية maintenance_run_opts *opts UNUSED,
					  بنية gc_config *cfg UNUSED)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", عدم);
	أرجع run_command(&cmd);
}

ساكن صحيح maintenance_task_worktree_prune(بنية maintenance_run_opts *opts UNUSED,
					   بنية gc_config *cfg)
{
	بنية child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", عدم);
	strvec_push(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	أرجع run_command(&prune_worktrees_cmd);
}

ساكن صحيح worktree_prune_condition(بنية gc_config *cfg)
{
	بنية strbuf buf = STRBUF_INIT;
	صحيح should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	بنية dirent *d;
	DIR *dir = عدم;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	𐡄𐡍 (limit <= 0) {
		should_prune = limit < 0;
		اذهب out;
	}

	𐡄𐡍 (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		اذهب out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	𐡄𐡍 (!dir)
		اذهب out;

	طالما (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		حرف *wtpath;
		strbuf_reset(&buf);
		𐡄𐡍 (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		حرر(wtpath);
	}

	should_prune = !limit;

out:
	𐡄𐡍 (dir)
		closedir(dir);
	strbuf_release(&buf);
	أرجع should_prune;
}

ساكن صحيح maintenance_task_rerere_gc(بنية maintenance_run_opts *opts UNUSED,
				      بنية gc_config *cfg UNUSED)
{
	بنية child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", عدم);
	أرجع run_command(&rerere_cmd);
}

ساكن صحيح rerere_gc_condition(بنية gc_config *cfg UNUSED)
{
	صحيح limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	𐡄𐡍 (!limit)
		أرجع 0; /* never prune */
	𐡄𐡍 (limit < 0)
		أرجع 1; /* always prune */

	أرجع rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
ساكن ثابت حرف *lock_repo_for_gc(صحيح force, pid_t* ret_pid)
{
	بنية lock_file lock = LOCK_INIT;
	حرف my_host[HOST_NAME_MAX + 1];
	بنية strbuf sb = STRBUF_INIT;
	بنية stat st;
	uintmax_t pid;
	FILE *fp;
	صحيح fd;
	حرف *pidfile_path;

	𐡄𐡍 (is_tempfile_active(pidfile))
		/* already locked */
		أرجع عدم;

	𐡄𐡍 (xgethostname(my_host, حجم(my_host)))
		xsnprintf(my_host, حجم(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	𐡄𐡍 (!force) {
		ساكن حرف locking_host[HOST_NAME_MAX + 1];
		ساكن حرف *scan_fmt;
		صحيح should_exit;

		𐡄𐡍 (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_path, "r");
		memset(locking_host, 0, حجم(locking_host));
		should_exit =
			fp != عدم &&
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
			time(عدم) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, scan_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		𐡄𐡍 (fp)
			fclose(fp);
		𐡄𐡍 (should_exit) {
			𐡄𐡍 (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			حرر(pidfile_path);
			أرجع locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	حرر(pidfile_path);
	أرجع عدم;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
ساكن صحيح report_last_gc_error(فراغ)
{
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret = 0;
	ssize_t len;
	بنية stat st;
	حرف *gc_log_path = repo_git_path(the_repository, "gc.log");

	𐡄𐡍 (stat(gc_log_path, &st)) {
		𐡄𐡍 (errno == ENOENT)
			اذهب done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		اذهب done;
	}

	𐡄𐡍 (st.st_mtime < gc_log_expire_time)
		اذهب done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	𐡄𐡍 (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	وإلا 𐡄𐡍 (len > 0) {
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
	حرر(gc_log_path);
	أرجع ret;
}

ساكن صحيح gc_foreground_tasks(بنية maintenance_run_opts *opts,
			       بنية gc_config *cfg)
{
	𐡄𐡍 (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		أرجع error(FAILED_RUN, "pack-refs");
	𐡄𐡍 (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		أرجع error(FAILED_RUN, "reflog");
	أرجع 0;
}

ساكن صحيح maintenance_task_odb(بنية maintenance_run_opts *opts,
				بنية gc_config *cfg,
				صحيح keep_largest_pack,
				صحيح aggressive)
{
	بنية odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	𐡄𐡍 (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	𐡄𐡍 (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	𐡄𐡍 (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	أرجع odb_optimize(the_repository->objects, &odb_opts);
}

صحيح cmd_gc(صحيح argc,
	   ثابت حرف **argv,
	   ثابت حرف *prefix,
	   بنية repository *repo UNUSED)
{
	صحيح aggressive = 0;
	صحيح force = 0;
	ثابت حرف *name;
	pid_t pid;
	صحيح daemonized = 0;
	صحيح keep_largest_pack = -1;
	صحيح skip_foreground_tasks = 0;
	timestamp_t dummy;
	بنية maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	بنية gc_config cfg = GC_CONFIG_INIT;
	ثابت حرف *prune_expire_sentinel = "sentinel";
	ثابت حرف *prune_expire_arg = prune_expire_sentinel;
	صحيح ret;
	بنية option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.value = &prune_expire_arg,
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

	𐡄𐡍 (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	𐡄𐡍 (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	𐡄𐡍 (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	𐡄𐡍 (prune_expire_arg != prune_expire_sentinel) {
		حرر(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	𐡄𐡍 (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	𐡄𐡍 (opts.auto_flag) {
		بنية odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		𐡄𐡍 (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		𐡄𐡍 (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			اذهب out;
		}

		𐡄𐡍 (!opts.quiet) {
			𐡄𐡍 (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			وإلا
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	𐡄𐡍 (opts.detach > 0) {
		ret = report_last_gc_error();
		𐡄𐡍 (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			اذهب out;

		} وإلا 𐡄𐡍 (ret) {
			/* an I/O error occurred, already reported */
			اذهب out;
		}

		𐡄𐡍 (!skip_foreground_tasks) {
			𐡄𐡍 (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				اذهب out;
			}

			𐡄𐡍 (gc_foreground_tasks(&opts, &cfg) < 0)
				die(عدم);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = lock_repo_for_gc(force, &pid);
	𐡄𐡍 (name) {
		𐡄𐡍 (opts.auto_flag) {
			ret = 0;
			اذهب out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	𐡄𐡍 (daemonized) {
		حرف *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		حرر(path);
	}

	𐡄𐡍 (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	𐡄𐡍 (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	𐡄𐡍 (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	𐡄𐡍 (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(عدم);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	𐡄𐡍 (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	𐡄𐡍 (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     عدم);

	𐡄𐡍 (!daemonized) {
		حرف *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		حرر(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	أرجع 0;
}

ساكن ثابت حرف *ثابت builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	عدم
};

ساكن صحيح maintenance_opt_schedule(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	تعداد schedule_priority *priority = opt->value;

	𐡄𐡍 (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	𐡄𐡍 (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	أرجع 0;
}

بنية cg_auto_data {
	صحيح num_not_in_graph;
	صحيح limit;
};

ساكن صحيح dfs_on_ref(ثابت بنية reference *ref, فراغ *cb_data)
{
	بنية cg_auto_data *data = (بنية cg_auto_data *)cb_data;
	صحيح result = 0;
	ثابت بنية object_id *maybe_peeled = ref->oid;
	بنية object_id peeled;
	بنية commit_list *stack = عدم;
	بنية commit *commit;

	𐡄𐡍 (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	𐡄𐡍 (odb_read_object_info(the_repository->objects, maybe_peeled, عدم) != OBJ_COMMIT)
		أرجع 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	𐡄𐡍 (!commit || commit->object.flags & SEEN)
		أرجع 0;
	commit->object.flags |= SEEN;

	𐡄𐡍 (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		أرجع 0;

	data->num_not_in_graph++;

	𐡄𐡍 (data->num_not_in_graph >= data->limit)
		أرجع 1;

	commit_list_insert(commit, &stack);

	طالما (!result && stack) {
		بنية commit_list *parent;

		commit = pop_commit(&stack);

		لكل (parent = commit->parents; parent; parent = parent->next) {
			𐡄𐡍 (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				استمر;

			parent->item->object.flags |= SEEN;
			data->num_not_in_graph++;

			𐡄𐡍 (data->num_not_in_graph >= data->limit) {
				result = 1;
				اكسر;
			}

			commit_list_insert(parent->item, &stack);
		}
	}

	commit_list_free(stack);
	أرجع result;
}

ساكن صحيح should_write_commit_graph(بنية gc_config *cfg UNUSED)
{
	صحيح result;
	بنية cg_auto_data data;

	data.num_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	𐡄𐡍 (!data.limit)
		أرجع 0;
	𐡄𐡍 (data.limit < 0)
		أرجع 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	أرجع result;
}

ساكن صحيح run_write_commit_graph(بنية maintenance_run_opts *opts)
{
	بنية child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", عدم);

	𐡄𐡍 (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	وإلا
		strvec_push(&child.args, "--progress");

	أرجع !!run_command(&child);
}

ساكن صحيح maintenance_task_commit_graph(بنية maintenance_run_opts *opts,
					 بنية gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	𐡄𐡍 (!the_repository->settings.core_commit_graph)
		أرجع 0;

	𐡄𐡍 (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		أرجع 1;
	}

	أرجع 0;
}

ساكن صحيح fetch_remote(بنية remote *remote, فراغ *cbdata)
{
	بنية maintenance_run_opts *opts = cbdata;
	بنية child_process child = CHILD_PROCESS_INIT;

	𐡄𐡍 (remote->skip_default_update)
		أرجع 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     عدم);

	𐡄𐡍 (opts->quiet)
		strvec_push(&child.args, "--quiet");

	أرجع !!run_command(&child);
}

ساكن صحيح maintenance_task_prefetch(بنية maintenance_run_opts *opts,
				     بنية gc_config *cfg UNUSED)
{
	𐡄𐡍 (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		أرجع 1;
	}

	أرجع 0;
}

ساكن صحيح maintenance_task_gc_foreground(بنية maintenance_run_opts *opts,
					  بنية gc_config *cfg)
{
	أرجع gc_foreground_tasks(opts, cfg);
}

ساكن صحيح maintenance_task_gc_background(بنية maintenance_run_opts *opts,
					  بنية gc_config *cfg UNUSED)
{
	بنية child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_push(&child.args, "gc");

	𐡄𐡍 (opts->auto_flag)
		strvec_push(&child.args, "--auto");
	𐡄𐡍 (opts->quiet)
		strvec_push(&child.args, "--quiet");
	وإلا
		strvec_push(&child.args, "--no-quiet");
	strvec_push(&child.args, "--no-detach");
	strvec_push(&child.args, "--skip-foreground-tasks");

	أرجع run_command(&child);
}

ساكن صحيح gc_condition(بنية gc_config *cfg)
{
	بنية odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	أرجع odb_optimize_required(the_repository->objects, &opts);
}

ساكن صحيح prune_packed(بنية maintenance_run_opts *opts)
{
	بنية child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_push(&child.args, "prune-packed");

	𐡄𐡍 (opts->quiet)
		strvec_push(&child.args, "--quiet");

	أرجع !!run_command(&child);
}

بنية write_loose_object_data {
	FILE *in;
	صحيح count;
	صحيح batch_size;
};

ساكن صحيح loose_object_auto_limit = 100;

ساكن صحيح loose_object_count(ثابت بنية object_id *oid UNUSED,
			      ثابت حرف *path UNUSED,
			      فراغ *data)
{
	صحيح *count = (صحيح*)data;
	𐡄𐡍 (++(*count) >= loose_object_auto_limit)
		أرجع 1;
	أرجع 0;
}

ساكن صحيح loose_object_auto_condition(بنية gc_config *cfg UNUSED)
{
	صحيح count = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	𐡄𐡍 (!loose_object_auto_limit)
		أرجع 0;
	𐡄𐡍 (loose_object_auto_limit < 0)
		أرجع 1;

	أرجع for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_count,
					     عدم, عدم, &count);
}

ساكن صحيح bail_on_loose(ثابت بنية object_id *oid UNUSED,
			 ثابت حرف *path UNUSED,
			 فراغ *data UNUSED)
{
	أرجع 1;
}

ساكن صحيح write_loose_object_to_stdin(ثابت بنية object_id *oid,
				       ثابت حرف *path UNUSED,
				       فراغ *data)
{
	بنية write_loose_object_data *d = (بنية write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	أرجع ++(d->count) > d->batch_size;
}

ساكن صحيح pack_loose(بنية maintenance_run_opts *opts)
{
	بنية repository *r = the_repository;
	صحيح result = 0;
	بنية write_loose_object_data data;
	بنية child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	𐡄𐡍 (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   عدم, عدم, عدم))
		أرجع 0;

	pack_proc.git_cmd = 1;

	strvec_push(&pack_proc.args, "pack-objects");
	𐡄𐡍 (opts->quiet)
		strvec_push(&pack_proc.args, "--quiet");
	وإلا
		strvec_push(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	𐡄𐡍 (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		أرجع 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.count = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	𐡄𐡍 (!data.batch_size)
		data.batch_size = INT_MAX;
	وإلا 𐡄𐡍 (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      عدم, عدم, &data);

	fclose(data.in);

	𐡄𐡍 (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	أرجع result;
}

ساكن صحيح maintenance_task_loose_objects(بنية maintenance_run_opts *opts,
					  بنية gc_config *cfg UNUSED)
{
	أرجع prune_packed(opts) || pack_loose(opts);
}

ساكن صحيح incremental_repack_auto_condition(بنية gc_config *cfg UNUSED)
{
	بنية packed_git *p;
	صحيح incremental_repack_auto_limit = 10;
	صحيح count = 0;

	prepare_repo_settings(the_repository);
	𐡄𐡍 (!the_repository->settings.core_multi_pack_index)
		أرجع 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	𐡄𐡍 (!incremental_repack_auto_limit)
		أرجع 0;
	𐡄𐡍 (incremental_repack_auto_limit < 0)
		أرجع 1;

	repo_for_each_pack(the_repository, p) {
		𐡄𐡍 (count >= incremental_repack_auto_limit)
			اكسر;
		𐡄𐡍 (!p->multi_pack_index)
			count++;
	}

	أرجع count >= incremental_repack_auto_limit;
}

ساكن صحيح multi_pack_index_write(بنية maintenance_run_opts *opts)
{
	بنية child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", عدم);

	𐡄𐡍 (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	وإلا
		strvec_push(&child.args, "--progress");

	𐡄𐡍 (run_command(&child))
		أرجع error(_("failed to write multi-pack-index"));

	أرجع 0;
}

ساكن صحيح multi_pack_index_expire(بنية maintenance_run_opts *opts)
{
	بنية child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", عدم);

	𐡄𐡍 (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	وإلا
		strvec_push(&child.args, "--progress");

	𐡄𐡍 (run_command(&child))
		أرجع error(_("'git multi-pack-index expire' failed"));

	أرجع 0;
}

#define TWO_GIGABYTES (INT32_MAX)

ساكن off_t get_auto_pack_size(فراغ)
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
	بنية packed_git *p;
	بنية repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		𐡄𐡍 (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} وإلا 𐡄𐡍 (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	𐡄𐡍 (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	أرجع result_size;
}

ساكن صحيح multi_pack_index_repack(بنية maintenance_run_opts *opts)
{
	بنية child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", عدم);

	𐡄𐡍 (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	وإلا
		strvec_push(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	𐡄𐡍 (run_command(&child))
		أرجع error(_("'git multi-pack-index repack' failed"));

	أرجع 0;
}

ساكن صحيح maintenance_task_incremental_repack(بنية maintenance_run_opts *opts,
					       بنية gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	𐡄𐡍 (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		أرجع 0;
	}

	𐡄𐡍 (multi_pack_index_write(opts))
		أرجع 1;
	𐡄𐡍 (multi_pack_index_expire(opts))
		أرجع 1;
	𐡄𐡍 (multi_pack_index_repack(opts))
		أرجع 1;
	أرجع 0;
}

ساكن صحيح maintenance_task_geometric_repack(بنية maintenance_run_opts *opts,
					     بنية gc_config *cfg)
{
	بنية odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	𐡄𐡍 (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	أرجع odb_optimize(the_repository->objects, &odb_opts);
}

ساكن صحيح geometric_repack_auto_condition(بنية gc_config *cfg)
{
	بنية odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	أرجع odb_optimize_required(the_repository->objects, &opts);
}

تعريفنوع صحيح (*maintenance_task_fn)(بنية maintenance_run_opts *opts,
				   بنية gc_config *cfg);
تعريفنوع صحيح (*maintenance_auto_fn)(بنية gc_config *cfg);

بنية maintenance_task {
	ثابت حرف *name;

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

ساكن ثابت بنية maintenance_task tasks[] = {
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

تعداد task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

تعداد auto_gc_hook_result {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

ساكن صحيح maybe_run_task(ثابت بنية maintenance_task *task,
			  بنية repository *repo,
			  بنية maintenance_run_opts *opts,
			  بنية gc_config *cfg,
			  تعداد task_phase phase,
			  تعداد auto_gc_hook_result *auto_gc_hook_result)
{
	صحيح foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ثابت حرف *region = foreground ? "maintenance foreground" : "maintenance";
	صحيح ret = 0;

	𐡄𐡍 (!fn)
		أرجع 0;
	𐡄𐡍 (opts->auto_flag) {
		𐡄𐡍 (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			أرجع 0;

		𐡄𐡍 (!task->auto_condition || !task->auto_condition(cfg))
			أرجع 0;

		𐡄𐡍 (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		𐡄𐡍 (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			أرجع 0;
	}

	trace2_region_enter(region, task->name, repo);
	𐡄𐡍 (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	أرجع ret;
}

ساكن صحيح maintenance_run_tasks(بنية maintenance_run_opts *opts,
				 بنية gc_config *cfg)
{
	صحيح result = 0;
	بنية lock_file lk;
	بنية repository *r = the_repository;
	حرف *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	تعداد auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	𐡄𐡍 (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		𐡄𐡍 (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		حرر(lock_path);
		أرجع 0;
	}
	حرر(lock_path);

	لكل (size_t i = 0; i < opts->tasks_nr; i++)
		𐡄𐡍 (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	𐡄𐡍 (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	لكل (size_t i = 0; i < opts->tasks_nr; i++)
		𐡄𐡍 (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	أرجع result;
}

تعداد maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

بنية maintenance_strategy {
	بنية {
		unsigned type;
		تعداد schedule_priority schedule;
	} tasks[TASK__COUNT];
};

ساكن ثابت بنية maintenance_strategy none_strategy = { 0 };

ساكن ثابت بنية maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

ساكن ثابت بنية maintenance_strategy incremental_strategy = {
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

ساكن ثابت بنية maintenance_strategy geometric_strategy = {
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

ساكن بنية maintenance_strategy parse_maintenance_strategy(ثابت حرف *name)
{
	𐡄𐡍 (!strcasecmp(name, "incremental"))
		أرجع incremental_strategy;
	𐡄𐡍 (!strcasecmp(name, "gc"))
		أرجع gc_strategy;
	𐡄𐡍 (!strcasecmp(name, "geometric"))
		أرجع geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

ساكن فراغ initialize_task_config(بنية maintenance_run_opts *opts,
				   ثابت بنية string_list *selected_tasks)
{
	بنية strbuf config_name = STRBUF_INIT;
	بنية maintenance_strategy strategy;
	تعداد maintenance_type type;
	ثابت حرف *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	𐡄𐡍 (selected_tasks->nr) {
		لكل (size_t i = 0; i < selected_tasks->nr; i++) {
			تعداد maintenance_task_label label = (intptr_t)selected_tasks->items[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		أرجع;
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
	𐡄𐡍 (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} وإلا {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	𐡄𐡍 (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	لكل (size_t i = 0; i < TASK__COUNT; i++) {
		صحيح config_value;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		𐡄𐡍 (!repo_config_get_bool(the_repository, config_name.buf, &config_value))
			strategy.tasks[i].type = config_value ? type : 0;
		𐡄𐡍 (!(strategy.tasks[i].type & type))
			استمر;

		𐡄𐡍 (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			𐡄𐡍 (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			𐡄𐡍 (strategy.tasks[i].schedule < opts->schedule)
				استمر;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

ساكن صحيح task_option_parse(ثابت بنية option *opt,
			     ثابت حرف *arg, صحيح unset)
{
	بنية string_list *selected_tasks = opt->value;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	لكل (i = 0; i < TASK__COUNT; i++)
		𐡄𐡍 (!strcasecmp(tasks[i].name, arg))
			اكسر;
	𐡄𐡍 (i >= TASK__COUNT) {
		error(_("'%s' is not a valid task"), arg);
		أرجع 1;
	}

	𐡄𐡍 (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		أرجع 1;
	}

	string_list_append(selected_tasks, arg)->util = (فراغ *)(intptr_t)i;

	أرجع 0;
}

ساكن صحيح maintenance_run(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			   بنية repository *repo UNUSED)
{
	بنية maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	بنية string_list selected_tasks = STRING_LIST_INIT_DUP;
	بنية gc_config cfg = GC_CONFIG_INIT;
	بنية option builtin_maintenance_run_options[] = {
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
	صحيح ret;

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

	𐡄𐡍 (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	أرجع ret;
}

ساكن حرف *get_maintpath(فراغ)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	أرجع strbuf_detach(&sb, عدم);
}

ساكن حرف ثابت * ثابت builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	عدم
};

ساكن صحيح maintenance_register(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				بنية repository *repo UNUSED)
{
	حرف *config_file = عدم;
	بنية option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	صحيح found = 0;
	ثابت حرف *key = "maintenance.repo";
	حرف *maintpath = get_maintpath();
	بنية string_list_item *item;
	ثابت بنية string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	𐡄𐡍 (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	𐡄𐡍 (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	𐡄𐡍 (!repo_config_get_string_multi(the_repository, key, &list)) {
		for_each_string_list_item(item, list) {
			𐡄𐡍 (!strcmp(maintpath, item->string)) {
				found = 1;
				اكسر;
			}
		}
	}

	𐡄𐡍 (!found) {
		صحيح rc;
		حرف *global_config_file = عدم;

		𐡄𐡍 (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		𐡄𐡍 (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, عدم, 0);
		حرر(global_config_file);

		𐡄𐡍 (rc)
			die(_("unable to add '%s' value of '%s'"),
			    key, maintpath);
	}

	حرر(maintpath);
	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	عدم
};

ساكن صحيح maintenance_unregister(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				  بنية repository *repo UNUSED)
{
	صحيح force = 0;
	حرف *config_file = عدم;
	بنية option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	ثابت حرف *key = "maintenance.repo";
	حرف *maintpath = get_maintpath();
	صحيح found = 0;
	بنية string_list_item *item;
	ثابت بنية string_list *list;
	بنية config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	𐡄𐡍 (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	𐡄𐡍 (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	𐡄𐡍 (!(config_file
	      ? git_configset_get_string_multi(&cs, key, &list)
	      : repo_config_get_string_multi(the_repository, key, &list))) {
		for_each_string_list_item(item, list) {
			𐡄𐡍 (!strcmp(maintpath, item->string)) {
				found = 1;
				اكسر;
			}
		}
	}

	𐡄𐡍 (found) {
		صحيح rc;
		حرف *global_config_file = عدم;

		𐡄𐡍 (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		𐡄𐡍 (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, key, عدم, maintpath, عدم,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_VALUE);
		حرر(global_config_file);

		𐡄𐡍 (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    key, maintpath);
	} وإلا 𐡄𐡍 (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	حرر(maintpath);
	أرجع 0;
}

ساكن ثابت حرف *get_frequency(تعداد schedule_priority schedule)
{
	بدل (schedule) {
	حالة SCHEDULE_HOURLY:
		أرجع "hourly";
	حالة SCHEDULE_DAILY:
		أرجع "daily";
	حالة SCHEDULE_WEEKLY:
		أرجع "weekly";
	افتراضي:
		BUG("invalid schedule %d", schedule);
	}
}

ساكن ثابت حرف *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	عدم
};

ساكن ثابت حرف *get_extra_config_parameters(فراغ) {
	ساكن ثابت حرف *result = عدم;
	بنية strbuf builder = STRBUF_INIT;

	𐡄𐡍 (result)
		أرجع result;

	لكل (ثابت حرف **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, عدم);
	أرجع result;
}

ساكن ثابت حرف *get_extra_launchctl_strings(فراغ) {
	ساكن ثابت حرف *result = عدم;
	بنية strbuf builder = STRBUF_INIT;

	𐡄𐡍 (result)
		أرجع result;

	لكل (ثابت حرف **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, عدم);
	أرجع result;
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
ساكن صحيح get_schedule_cmd(ثابت حرف *cmd, صحيح *is_available, حرف **out)
{
	حرف *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	بنية string_list_item *item;
	بنية string_list list = STRING_LIST_INIT_NODUP;

	𐡄𐡍 (!testing) {
		𐡄𐡍 (out)
			*out = xstrdup(cmd);
		أرجع 0;
	}

	𐡄𐡍 (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		بنية string_list pair = STRING_LIST_INIT_NODUP;

		𐡄𐡍 (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			استمر;

		𐡄𐡍 (!strcmp(cmd, pair.items[0].string)) {
			𐡄𐡍 (out)
				*out = xstrdup(pair.items[1].string);
			𐡄𐡍 (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			اذهب out;
		}

		string_list_clear(&pair, 0);
	}

	𐡄𐡍 (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	حرر(testing);
	أرجع 1;
}

ساكن صحيح get_random_minute(فراغ)
{
	/* Use a static value when under tests. */
	𐡄𐡍 (getenv("GIT_TEST_MAINT_SCHEDULER"))
		أرجع 13;

	أرجع git_rand(0) % 60;
}

ساكن صحيح is_launchctl_available(فراغ)
{
	صحيح is_available;
	𐡄𐡍 (get_schedule_cmd("launchctl", &is_available, عدم))
		أرجع is_available;

#ifdef __APPLE__
	أرجع 1;
#وإلا
	أرجع 0;
#endif
}

ساكن حرف *launchctl_service_name(ثابت حرف *frequency)
{
	بنية strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	أرجع strbuf_detach(&label, عدم);
}

ساكن حرف *launchctl_service_filename(ثابت حرف *name)
{
	حرف *expanded;
	بنية strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	𐡄𐡍 (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	أرجع expanded;
}

ساكن حرف *launchctl_get_uid(فراغ)
{
	أرجع xstrfmt("gui/%d", getuid());
}

ساكن صحيح launchctl_boot_plist(صحيح enable, ثابت حرف *filename)
{
	حرف *cmd;
	صحيح result;
	بنية child_process child = CHILD_PROCESS_INIT;
	حرف *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", عدم, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, عدم);

	child.no_stderr = 1;
	child.no_stdout = 1;

	𐡄𐡍 (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	حرر(cmd);
	حرر(uid);
	أرجع result;
}

ساكن صحيح launchctl_remove_plist(تعداد schedule_priority schedule)
{
	ثابت حرف *frequency = get_frequency(schedule);
	حرف *name = launchctl_service_name(frequency);
	حرف *filename = launchctl_service_filename(name);
	صحيح result = launchctl_boot_plist(0, filename);
	unlink(filename);
	حرر(filename);
	حرر(name);
	أرجع result;
}

ساكن صحيح launchctl_remove_plists(فراغ)
{
	أرجع launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

ساكن صحيح launchctl_list_contains_plist(ثابت حرف *name, ثابت حرف *cmd)
{
	بنية child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, عدم);

	child.no_stderr = 1;
	child.no_stdout = 1;

	𐡄𐡍 (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	أرجع !finish_command(&child);
}

ساكن صحيح launchctl_schedule_plist(ثابت حرف *exec_path, تعداد schedule_priority schedule)
{
	صحيح i, fd;
	ثابت حرف *preamble, *repeat;
	ثابت حرف *frequency = get_frequency(schedule);
	حرف *name = launchctl_service_name(frequency);
	حرف *filename = launchctl_service_filename(name);
	بنية lock_file lk = LOCK_INIT;
	ساكن unsigned long lock_file_timeout_ms = ULONG_MAX;
	بنية strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	بنية stat st;
	حرف *cmd;
	صحيح minute = get_random_minute();

	get_schedule_cmd("launchctl", عدم, &cmd);
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

	بدل (schedule) {
	حالة SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		لكل (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		اكسر;

	حالة SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		لكل (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		اكسر;

	حالة SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		اكسر;

	افتراضي:
		/* unreachable */
		اكسر;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	𐡄𐡍 (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	𐡄𐡍 ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	𐡄𐡍 (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	وإلا {
		𐡄𐡍 (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		𐡄𐡍 (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	حرر(filename);
	حرر(name);
	حرر(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	أرجع 0;
}

ساكن صحيح launchctl_add_plists(فراغ)
{
	ثابت حرف *exec_path = git_exec_path();

	أرجع launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

ساكن صحيح launchctl_update_schedule(صحيح run_maintenance, صحيح fd UNUSED)
{
	𐡄𐡍 (run_maintenance)
		أرجع launchctl_add_plists();
	وإلا
		أرجع launchctl_remove_plists();
}

ساكن صحيح is_schtasks_available(فراغ)
{
	صحيح is_available;
	𐡄𐡍 (get_schedule_cmd("schtasks", &is_available, عدم))
		أرجع is_available;

#ifdef GIT_WINDOWS_NATIVE
	أرجع 1;
#وإلا
	أرجع 0;
#endif
}

ساكن حرف *schtasks_task_name(ثابت حرف *frequency)
{
	بنية strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	أرجع strbuf_detach(&label, عدم);
}

ساكن صحيح schtasks_remove_task(تعداد schedule_priority schedule)
{
	حرف *cmd;
	بنية child_process child = CHILD_PROCESS_INIT;
	ثابت حرف *frequency = get_frequency(schedule);
	حرف *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", عدم, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", عدم);
	حرر(name);
	حرر(cmd);

	أرجع run_command(&child);
}

ساكن صحيح schtasks_remove_tasks(فراغ)
{
	أرجع schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

ساكن صحيح schtasks_schedule_task(ثابت حرف *exec_path, تعداد schedule_priority schedule)
{
	حرف *cmd;
	صحيح result;
	بنية child_process child = CHILD_PROCESS_INIT;
	ثابت حرف *xml;
	بنية tempfile *tfile;
	ثابت حرف *frequency = get_frequency(schedule);
	حرف *name = schtasks_task_name(frequency);
	بنية strbuf tfilename = STRBUF_INIT;
	صحيح minute = get_random_minute();

	get_schedule_cmd("schtasks", عدم, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	𐡄𐡍 (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	بدل (schedule) {
	حالة SCHEDULE_HOURLY:
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
		اكسر;

	حالة SCHEDULE_DAILY:
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
		اكسر;

	حالة SCHEDULE_WEEKLY:
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
		اكسر;

	افتراضي:
		اكسر;
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
				  get_tempfile_path(tfile), عدم);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	𐡄𐡍 (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	حرر(name);
	حرر(cmd);
	أرجع result;
}

ساكن صحيح schtasks_schedule_tasks(فراغ)
{
	ثابت حرف *exec_path = git_exec_path();

	أرجع schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

ساكن صحيح schtasks_update_schedule(صحيح run_maintenance, صحيح fd UNUSED)
{
	𐡄𐡍 (run_maintenance)
		أرجع schtasks_schedule_tasks();
	وإلا
		أرجع schtasks_remove_tasks();
}

MAYBE_UNUSED
ساكن صحيح check_crontab_process(ثابت حرف *cmd)
{
	بنية child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_push(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	𐡄𐡍 (start_command(&child))
		أرجع 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	أرجع 1;
}

ساكن صحيح is_crontab_available(فراغ)
{
	حرف *cmd;
	صحيح is_available;
	صحيح ret;

	𐡄𐡍 (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		اذهب out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#وإلا
	ret = check_crontab_process(cmd);
#endif

out:
	حرر(cmd);
	أرجع ret;
}

#define BEGIN_LINE "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_LINE "# END GIT MAINTENANCE SCHEDULE"

ساكن صحيح crontab_update_schedule(صحيح run_maintenance, صحيح fd)
{
	حرف *cmd;
	صحيح result = 0;
	صحيح in_old_region = 0;
	بنية child_process crontab_list = CHILD_PROCESS_INIT;
	بنية child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	بنية strbuf line = STRBUF_INIT;
	بنية tempfile *tmpedit = عدم;
	صحيح minute = get_random_minute();

	get_schedule_cmd("crontab", عدم, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_push(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	𐡄𐡍 (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		اذهب out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	𐡄𐡍 (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		اذهب out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	𐡄𐡍 (!cron_in) {
		result = error(_("failed to open temporary file"));
		اذهب out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	طالما (!strbuf_getline_lf(&line, cron_list)) {
		𐡄𐡍 (!in_old_region && !strcmp(line.buf, BEGIN_LINE))
			in_old_region = 1;
		وإلا 𐡄𐡍 (in_old_region && !strcmp(line.buf, END_LINE))
			in_old_region = 0;
		وإلا 𐡄𐡍 (!in_old_region)
			fprintf(cron_in, "%s\n", line.buf);
	}
	strbuf_release(&line);

	𐡄𐡍 (run_maintenance) {
		بنية strbuf line_format = STRBUF_INIT;
		ثابت حرف *exec_path = git_exec_path();

		fprintf(cron_in, "%s\n", BEGIN_LINE);
		fprintf(cron_in,
			"# The following schedule was created by Git\n");
		fprintf(cron_in, "# Any edits made in this region might be\n");
		fprintf(cron_in,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&line_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_path, exec_path, get_extra_config_parameters());
		fprintf(cron_in, line_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_in, line_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_in, line_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&line_format);

		fprintf(cron_in, "\n%s\n", END_LINE);
	}

	fflush(cron_in);

	strvec_split(&crontab_edit.args, cmd);
	strvec_push(&crontab_edit.args, get_tempfile_path(tmpedit));
	crontab_edit.git_cmd = 0;

	𐡄𐡍 (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		اذهب out;
	}

	𐡄𐡍 (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	وإلا
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	حرر(cmd);
	أرجع result;
}

ساكن صحيح real_is_systemd_timer_available(فراغ)
{
	بنية child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", عدم);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	𐡄𐡍 (start_command(&child))
		أرجع 0;
	𐡄𐡍 (finish_command(&child))
		أرجع 0;
	أرجع 1;
}

ساكن صحيح is_systemd_timer_available(فراغ)
{
	صحيح is_available;

	𐡄𐡍 (get_schedule_cmd("systemctl", &is_available, عدم))
		أرجع is_available;

	أرجع real_is_systemd_timer_available();
}

ساكن حرف *xdg_config_home_systemd(ثابت حرف *filename)
{
	أرجع xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

ساكن صحيح systemd_timer_delete_timer_file(تعداد schedule_priority priority)
{
	صحيح ret = 0;
	ثابت حرف *frequency = get_frequency(priority);
	حرف *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	حرف *filename = xdg_config_home_systemd(local_timer_name);

	𐡄𐡍 (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	حرر(filename);
	حرر(local_timer_name);
	أرجع ret;
}

ساكن صحيح systemd_timer_delete_service_template(فراغ)
{
	صحيح ret = 0;
	حرف *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	حرف *filename = xdg_config_home_systemd(local_service_name);
	𐡄𐡍 (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	حرر(filename);
	حرر(local_service_name);
	أرجع ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
ساكن صحيح systemd_timer_write_timer_file(تعداد schedule_priority schedule,
					  صحيح minute)
{
	صحيح res = -1;
	حرف *filename;
	FILE *file;
	ثابت حرف *unit;
	حرف *schedule_pattern = عدم;
	ثابت حرف *frequency = get_frequency(schedule);
	حرف *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	𐡄𐡍 (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		اذهب error;
	}
	file = fopen_or_warn(filename, "w");
	𐡄𐡍 (!file)
		اذهب error;

	بدل (schedule) {
	حالة SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		اكسر;

	حالة SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		اكسر;

	حالة SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		اكسر;

	افتراضي:
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
	𐡄𐡍 (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		اذهب error;
	}
	𐡄𐡍 (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		اذهب error;
	}

	res = 0;

error:
	حرر(schedule_pattern);
	حرر(local_timer_name);
	حرر(filename);
	أرجع res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
ساكن صحيح systemd_timer_write_service_template(ثابت حرف *exec_path)
{
	صحيح res = -1;
	حرف *filename;
	FILE *file;
	ثابت حرف *unit;
	حرف *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	𐡄𐡍 (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		اذهب error;
	}
	file = fopen_or_warn(filename, "w");
	𐡄𐡍 (!file)
		اذهب error;

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
	𐡄𐡍 (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		اذهب error;
	}
	𐡄𐡍 (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		اذهب error;
	}

	res = 0;

error:
	حرر(local_service_name);
	حرر(filename);
	أرجع res;
}

ساكن صحيح systemd_timer_enable_unit(صحيح enable,
				     تعداد schedule_priority schedule,
				     صحيح minute)
{
	حرف *cmd = عدم;
	بنية child_process child = CHILD_PROCESS_INIT;
	ثابت حرف *frequency = get_frequency(schedule);
	صحيح ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	𐡄𐡍 (!enable) {
		child.no_stderr = 1;
	} وإلا 𐡄𐡍 (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		اذهب out;
	}

	get_schedule_cmd("systemctl", عدم, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", عدم);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	𐡄𐡍 (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		اذهب out;
	}

	𐡄𐡍 (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		𐡄𐡍 (enable) {
			ret = error(_("failed to run systemctl"));
			اذهب out;
		}
	}

	ret = 0;

out:
	حرر(cmd);
	أرجع ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
ساكن فراغ systemd_timer_delete_stale_timer_templates(فراغ)
{
	حرف *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	حرف *filename = xdg_config_home_systemd(timer_template_name);

	𐡄𐡍 (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	حرر(filename);
	حرر(timer_template_name);
}

ساكن صحيح systemd_timer_delete_unit_files(فراغ)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	أرجع systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

ساكن صحيح systemd_timer_delete_units(فراغ)
{
	صحيح minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	أرجع systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

ساكن صحيح systemd_timer_setup_units(فراغ)
{
	صحيح minute = get_random_minute();
	ثابت حرف *exec_path = git_exec_path();

	صحيح ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	𐡄𐡍 (ret)
		systemd_timer_delete_units();
	وإلا
		systemd_timer_delete_stale_timer_templates();

	أرجع ret;
}

ساكن صحيح systemd_timer_update_schedule(صحيح run_maintenance, صحيح fd UNUSED)
{
	𐡄𐡍 (run_maintenance)
		أرجع systemd_timer_setup_units();
	وإلا
		أرجع systemd_timer_delete_units();
}

تعداد scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

ساكن ثابت بنية {
	ثابت حرف *name;
	صحيح (*is_available)(فراغ);
	صحيح (*update_schedule)(صحيح run_maintenance, صحيح fd);
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

ساكن تعداد scheduler parse_scheduler(ثابت حرف *value)
{
	𐡄𐡍 (!value)
		أرجع SCHEDULER_INVALID;
	وإلا 𐡄𐡍 (!strcasecmp(value, "auto"))
		أرجع SCHEDULER_AUTO;
	وإلا 𐡄𐡍 (!strcasecmp(value, "cron") || !strcasecmp(value, "crontab"))
		أرجع SCHEDULER_CRON;
	وإلا 𐡄𐡍 (!strcasecmp(value, "systemd") ||
		 !strcasecmp(value, "systemd-timer"))
		أرجع SCHEDULER_SYSTEMD;
	وإلا 𐡄𐡍 (!strcasecmp(value, "launchctl"))
		أرجع SCHEDULER_LAUNCHCTL;
	وإلا 𐡄𐡍 (!strcasecmp(value, "schtasks"))
		أرجع SCHEDULER_SCHTASKS;
	وإلا
		أرجع SCHEDULER_INVALID;
}

ساكن صحيح maintenance_opt_scheduler(ثابت بنية option *opt, ثابت حرف *arg,
				     صحيح unset)
{
	تعداد scheduler *scheduler = opt->value;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	𐡄𐡍 (*scheduler == SCHEDULER_INVALID)
		أرجع error(_("unrecognized --scheduler argument '%s'"), arg);
	أرجع 0;
}

بنية maintenance_start_opts {
	تعداد scheduler scheduler;
};

ساكن تعداد scheduler resolve_scheduler(تعداد scheduler scheduler)
{
	𐡄𐡍 (scheduler != SCHEDULER_AUTO)
		أرجع scheduler;

#𐡄𐡍 defined(__APPLE__)
	أرجع SCHEDULER_LAUNCHCTL;

#وإلاإذا defined(GIT_WINDOWS_NATIVE)
	أرجع SCHEDULER_SCHTASKS;

#وإلاإذا defined(__linux__)
	𐡄𐡍 (is_systemd_timer_available())
		أرجع SCHEDULER_SYSTEMD;
	وإلا 𐡄𐡍 (is_crontab_available())
		أرجع SCHEDULER_CRON;
	وإلا
		die(_("neither systemd timers nor crontab are available"));

#وإلا
	أرجع SCHEDULER_CRON;
#endif
}

ساكن فراغ validate_scheduler(تعداد scheduler scheduler)
{
	𐡄𐡍 (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	𐡄𐡍 (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	𐡄𐡍 (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

ساكن صحيح update_background_schedule(ثابت بنية maintenance_start_opts *opts,
				      صحيح enable)
{
	unsigned صحيح i;
	صحيح result = 0;
	بنية lock_file lk;
	حرف *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	𐡄𐡍 (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		𐡄𐡍 (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		وإلا
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		حرر(lock_path);
		أرجع -1;
	}

	لكل (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		𐡄𐡍 (enable && opts->scheduler == i)
			استمر;
		𐡄𐡍 (!scheduler_fn[i].is_available())
			استمر;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	𐡄𐡍 (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	حرر(lock_path);
	أرجع result;
}

ساكن ثابت حرف *ثابت builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	عدم
};

ساكن صحيح maintenance_start(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			     بنية repository *repo)
{
	بنية maintenance_start_opts opts = { 0 };
	بنية option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	ثابت حرف *register_args[] = { "register", عدم };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	𐡄𐡍 (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	𐡄𐡍 (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	𐡄𐡍 (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, عدم, repo))
		warning(_("failed to add repo to global config"));
	أرجع 0;
}

ساكن ثابت حرف *ثابت builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	عدم
};

ساكن صحيح maintenance_stop(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			    بنية repository *repo UNUSED)
{
	بنية option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	𐡄𐡍 (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	أرجع update_background_schedule(عدم, 0);
}

ساكن ثابت حرف *ثابت builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	عدم
};

ساكن صحيح maintenance_is_needed(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				 بنية repository *repo UNUSED)
{
	بنية maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	بنية string_list selected_tasks = STRING_LIST_INIT_DUP;
	بنية gc_config cfg = GC_CONFIG_INIT;
	بنية option options[] = {
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
	𐡄𐡍 (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	𐡄𐡍 (opts.auto_flag) {
		لكل (size_t i = 0; i < opts.tasks_nr; i++) {
			𐡄𐡍 (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				اكسر;
			}
		}
	} وإلا {
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

	𐡄𐡍 (is_needed)
		أرجع 0;
	أرجع 1;
}

ساكن ثابت حرف *ثابت builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	عدم,
};

صحيح cmd_maintenance(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *prefix,
		    بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_maintenance_options[] = {
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
	أرجع fn(argc, argv, prefix, repo);
}
