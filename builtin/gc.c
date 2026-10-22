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

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "date.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "tempfile.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "sigchain.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "commit-graph.h"
#অন্তর্ভাব "packfile.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "reflog.h"
#অন্তর্ভাব "rerere.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "exec-cmd.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hook.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "trace2.h"
#অন্তর্ভাব "worktree.h"

#define FAILED_RUN "failed to run %s"

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	রিক্ত
};

স্থির timestamp_t gc_log_expire_time;
স্থির সংরচনা tempfile *pidfile;
স্থির সংরচনা lock_file log_lock;
স্থির সংরচনা string_list pack_garbage = STRING_LIST_INIT_DUP;

স্থির শূন্য clean_pack_garbage(শূন্য)
{
	পূর্ণাঙ্ক i;
	প্রতি (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.বস্তুসমূহ[i].string);
	string_list_clear(&pack_garbage, 0);
}

স্থির শূন্য report_pack_garbage(unsigned seen_bits, ধ্রুব বর্ণ *path)
{
	যদি (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

স্থির শূন্য process_log_file(শূন্য)
{
	সংরচনা stat st;
	যদি (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		পূর্ণাঙ্ক saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} অন্যথা যদি (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} অন্যথা {
		বর্ণ *path = repo_git_path(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(path);
		rollback_lock_file(&log_lock);
		মুক্ত(path);
	}
}

স্থির শূন্য process_log_file_at_exit(শূন্য)
{
	fflush(stderr);
	process_log_file();
}

স্থির পূর্ণাঙ্ক gc_config_is_timestamp_never(ধ্রুব বর্ণ *var)
{
	ধ্রুব বর্ণ *মান;
	timestamp_t expire;

	যদি (!repo_config_get_মান(the_repository, var, &মান) && মান) {
		যদি (parse_expiry_date(মান, &expire))
			die(_("failed to parse '%s' value '%s'"), var, মান);
		প্রত্যাগম expire == 0;
	}
	প্রত্যাগম 0;
}

সংরচনা gc_config {
	পূর্ণাঙ্ক pack_refs;
	পূর্ণাঙ্ক prune_reflogs;
	পূর্ণাঙ্ক cruft_packs;
	unsigned long max_cruft_size;
	পূর্ণাঙ্ক aggressive_depth;
	পূর্ণাঙ্ক aggressive_window;
	পূর্ণাঙ্ক detach_auto;
	বর্ণ *gc_log_expire;
	বর্ণ *prune_expire;
	বর্ণ *prune_worktrees_expire;
	বর্ণ *repack_expire_to;
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

স্থির শূন্য gc_config_release(সংরচনা gc_config *cfg)
{
	মুক্ত(cfg->gc_log_expire);
	মুক্ত(cfg->prune_expire);
	মুক্ত(cfg->prune_worktrees_expire);
}

স্থির শূন্য gc_config(সংরচনা gc_config *cfg)
{
	ধ্রুব বর্ণ *মান;
	বর্ণ *owned = রিক্ত;

	যদি (!repo_config_get_মান(the_repository, "gc.packrefs", &মান)) {
		যদি (মান && !strcmp(মান, "notbare"))
			cfg->pack_refs = -1;
		অন্যথা
			cfg->pack_refs = git_config_bool("gc.packrefs", মান);
	}

	যদি (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	যদি (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		মুক্ত(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	যদি (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		মুক্ত(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	যদি (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		মুক্ত(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, রিক্ত);
}

গণনা schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

স্থির গণনা schedule_priority parse_schedule(ধ্রুব বর্ণ *মান)
{
	যদি (!মান)
		প্রত্যাগম SCHEDULE_NONE;
	যদি (!strcasecmp(মান, "hourly"))
		প্রত্যাগম SCHEDULE_HOURLY;
	যদি (!strcasecmp(মান, "daily"))
		প্রত্যাগম SCHEDULE_DAILY;
	যদি (!strcasecmp(মান, "weekly"))
		প্রত্যাগম SCHEDULE_WEEKLY;
	প্রত্যাগম SCHEDULE_NONE;
}

গণনা maintenance_task_label {
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
	TASK_গুনতি
};

সংরচনা maintenance_run_opts {
	গণনা maintenance_task_label *tasks;
	size_t tasks_nr, tasks_alloc;
	পূর্ণাঙ্ক auto_flag;
	পূর্ণাঙ্ক detach;
	পূর্ণাঙ্ক quiet;
	গণনা schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

স্থির শূন্য maintenance_run_opts_release(সংরচনা maintenance_run_opts *opts)
{
	মুক্ত(opts->tasks);
}

স্থির পূর্ণাঙ্ক pack_refs_condition(UNUSED সংরচনা gc_config *cfg)
{
	সংরচনা string_list included_refs = STRING_LIST_INIT_NODUP;
	সংরচনা ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	সংরচনা refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	যদি (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		প্রত্যাগম 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	প্রত্যাগম required;
}

স্থির পূর্ণাঙ্ক maintenance_task_pack_refs(সংরচনা maintenance_run_opts *opts,
				      UNUSED সংরচনা gc_config *cfg)
{
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", রিক্ত);
	যদি (opts->auto_flag)
		strvec_ঠেলো(&cmd.args, "--auto");

	প্রত্যাগম run_command(&cmd);
}

সংরচনা গুনতি_reflog_entries_data {
	সংরচনা expire_reflog_policy_cb policy;
	size_t গুনতি;
	size_t limit;
};

স্থির পূর্ণাঙ্ক গুনতি_reflog_entries(ধ্রুব বর্ণ *refname UNUSED,
				সংরচনা object_id *old_oid, সংরচনা object_id *new_oid,
				ধ্রুব বর্ণ *committer, timestamp_t timestamp,
				পূর্ণাঙ্ক tz, ধ্রুব বর্ণ *বার্তা, শূন্য *cb_data)
{
	সংরচনা গুনতি_reflog_entries_data *data = cb_data;
	যদি (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, বার্তা, &data->policy))
		data->গুনতি++;
	প্রত্যাগম data->গুনতি >= data->limit;
}

স্থির পূর্ণাঙ্ক reflog_expire_condition(সংরচনা gc_config *cfg UNUSED)
{
	timestamp_t now = time(রিক্ত);
	সংরচনা গুনতি_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	পূর্ণাঙ্ক limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	যদি (!limit)
		প্রত্যাগম 0;
	যদি (limit < 0)
		প্রত্যাগম 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 গুনতি_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	প্রত্যাগম data.গুনতি >= data.limit;
}

স্থির পূর্ণাঙ্ক maintenance_task_reflog_expire(সংরচনা maintenance_run_opts *opts UNUSED,
					  সংরচনা gc_config *cfg UNUSED)
{
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", রিক্ত);
	প্রত্যাগম run_command(&cmd);
}

স্থির পূর্ণাঙ্ক maintenance_task_worktree_prune(সংরচনা maintenance_run_opts *opts UNUSED,
					   সংরচনা gc_config *cfg)
{
	সংরচনা child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", রিক্ত);
	strvec_ঠেলো(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	প্রত্যাগম run_command(&prune_worktrees_cmd);
}

স্থির পূর্ণাঙ্ক worktree_prune_condition(সংরচনা gc_config *cfg)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	পূর্ণাঙ্ক should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	সংরচনা dirent *d;
	DIR *dir = রিক্ত;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	যদি (limit <= 0) {
		should_prune = limit < 0;
		গচ্ছ out;
	}

	যদি (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		গচ্ছ out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	যদি (!dir)
		গচ্ছ out;

	যাবত্ (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		বর্ণ *wtpath;
		strbuf_reset(&buf);
		যদি (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		মুক্ত(wtpath);
	}

	should_prune = !limit;

out:
	যদি (dir)
		closedir(dir);
	strbuf_release(&buf);
	প্রত্যাগম should_prune;
}

স্থির পূর্ণাঙ্ক maintenance_task_rerere_gc(সংরচনা maintenance_run_opts *opts UNUSED,
				      সংরচনা gc_config *cfg UNUSED)
{
	সংরচনা child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", রিক্ত);
	প্রত্যাগম run_command(&rerere_cmd);
}

স্থির পূর্ণাঙ্ক rerere_gc_condition(সংরচনা gc_config *cfg UNUSED)
{
	পূর্ণাঙ্ক limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	যদি (!limit)
		প্রত্যাগম 0; /* never prune */
	যদি (limit < 0)
		প্রত্যাগম 1; /* always prune */

	প্রত্যাগম rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
স্থির ধ্রুব বর্ণ *lock_repo_for_gc(পূর্ণাঙ্ক force, pid_t* ret_pid)
{
	সংরচনা lock_file lock = LOCK_INIT;
	বর্ণ my_host[HOST_NAME_MAX + 1];
	সংরচনা strbuf sb = STRBUF_INIT;
	সংরচনা stat st;
	uintmax_t pid;
	FILE *fp;
	পূর্ণাঙ্ক fd;
	বর্ণ *pidfile_path;

	যদি (is_tempfile_active(pidfile))
		/* already locked */
		প্রত্যাগম রিক্ত;

	যদি (xgethostname(my_host, পরিমাণ(my_host)))
		xsnprintf(my_host, পরিমাণ(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	যদি (!force) {
		স্থির বর্ণ locking_host[HOST_NAME_MAX + 1];
		স্থির বর্ণ *scan_fmt;
		পূর্ণাঙ্ক should_exit;

		যদি (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_path, "r");
		memset(locking_host, 0, পরিমাণ(locking_host));
		should_exit =
			fp != রিক্ত &&
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
			time(রিক্ত) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, scan_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		যদি (fp)
			fclose(fp);
		যদি (should_exit) {
			যদি (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			মুক্ত(pidfile_path);
			প্রত্যাগম locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	মুক্ত(pidfile_path);
	প্রত্যাগম রিক্ত;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
স্থির পূর্ণাঙ্ক report_last_gc_error(শূন্য)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	পূর্ণাঙ্ক ret = 0;
	ssize_t len;
	সংরচনা stat st;
	বর্ণ *gc_log_path = repo_git_path(the_repository, "gc.log");

	যদি (stat(gc_log_path, &st)) {
		যদি (errno == ENOENT)
			গচ্ছ done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		গচ্ছ done;
	}

	যদি (st.st_mtime < gc_log_expire_time)
		গচ্ছ done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	যদি (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	অন্যথা যদি (len > 0) {
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
	মুক্ত(gc_log_path);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক gc_foreground_tasks(সংরচনা maintenance_run_opts *opts,
			       সংরচনা gc_config *cfg)
{
	যদি (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		প্রত্যাগম error(FAILED_RUN, "pack-refs");
	যদি (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		প্রত্যাগম error(FAILED_RUN, "reflog");
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক maintenance_task_odb(সংরচনা maintenance_run_opts *opts,
				সংরচনা gc_config *cfg,
				পূর্ণাঙ্ক keep_largest_pack,
				পূর্ণাঙ্ক aggressive)
{
	সংরচনা odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	যদি (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	যদি (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	যদি (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	প্রত্যাগম odb_optimize(the_repository->objects, &odb_opts);
}

পূর্ণাঙ্ক cmd_gc(পূর্ণাঙ্ক argc,
	   ধ্রুব বর্ণ **argv,
	   ধ্রুব বর্ণ *prefix,
	   সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক aggressive = 0;
	পূর্ণাঙ্ক force = 0;
	ধ্রুব বর্ণ *name;
	pid_t pid;
	পূর্ণাঙ্ক daemonized = 0;
	পূর্ণাঙ্ক keep_largest_pack = -1;
	পূর্ণাঙ্ক skip_foreground_tasks = 0;
	timestamp_t dummy;
	সংরচনা maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	সংরচনা gc_config cfg = GC_CONFIG_INIT;
	ধ্রুব বর্ণ *prune_expire_sentinel = "sentinel";
	ধ্রুব বর্ণ *prune_expire_arg = prune_expire_sentinel;
	পূর্ণাঙ্ক ret;
	সংরচনা option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.মান = &prune_expire_arg,
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

	যদি (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	যদি (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	যদি (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	যদি (prune_expire_arg != prune_expire_sentinel) {
		মুক্ত(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	যদি (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	যদি (opts.auto_flag) {
		সংরচনা odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		যদি (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		যদি (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			গচ্ছ out;
		}

		যদি (!opts.quiet) {
			যদি (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			অন্যথা
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	যদি (opts.detach > 0) {
		ret = report_last_gc_error();
		যদি (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			গচ্ছ out;

		} অন্যথা যদি (ret) {
			/* an I/O error occurred, already reported */
			গচ্ছ out;
		}

		যদি (!skip_foreground_tasks) {
			যদি (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				গচ্ছ out;
			}

			যদি (gc_foreground_tasks(&opts, &cfg) < 0)
				die(রিক্ত);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = lock_repo_for_gc(force, &pid);
	যদি (name) {
		যদি (opts.auto_flag) {
			ret = 0;
			গচ্ছ out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	যদি (daemonized) {
		বর্ণ *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		মুক্ত(path);
	}

	যদি (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	যদি (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	যদি (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	যদি (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(রিক্ত);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	যদি (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	যদি (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     রিক্ত);

	যদি (!daemonized) {
		বর্ণ *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		মুক্ত(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	প্রত্যাগম 0;
}

স্থির ধ্রুব বর্ণ *ধ্রুব builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	রিক্ত
};

স্থির পূর্ণাঙ্ক maintenance_opt_schedule(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
				    পূর্ণাঙ্ক unset)
{
	গণনা schedule_priority *priority = opt->মান;

	যদি (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	যদি (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	প্রত্যাগম 0;
}

সংরচনা cg_auto_data {
	পূর্ণাঙ্ক অঙ্ক_not_in_graph;
	পূর্ণাঙ্ক limit;
};

স্থির পূর্ণাঙ্ক dfs_on_ref(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data)
{
	সংরচনা cg_auto_data *data = (সংরচনা cg_auto_data *)cb_data;
	পূর্ণাঙ্ক result = 0;
	ধ্রুব সংরচনা object_id *maybe_peeled = ref->oid;
	সংরচনা object_id peeled;
	সংরচনা commit_list *স্তূপ = রিক্ত;
	সংরচনা commit *commit;

	যদি (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	যদি (odb_read_object_info(the_repository->objects, maybe_peeled, রিক্ত) != OBJ_COMMIT)
		প্রত্যাগম 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	যদি (!commit || commit->object.flags & SEEN)
		প্রত্যাগম 0;
	commit->object.flags |= SEEN;

	যদি (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		প্রত্যাগম 0;

	data->অঙ্ক_not_in_graph++;

	যদি (data->অঙ্ক_not_in_graph >= data->limit)
		প্রত্যাগম 1;

	commit_list_ঢোকাও(commit, &স্তূপ);

	যাবত্ (!result && স্তূপ) {
		সংরচনা commit_list *parent;

		commit = বের_করো_commit(&স্তূপ);

		প্রতি (parent = commit->parents; parent; parent = parent->next) {
			যদি (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				অনুবর্ত;

			parent->item->object.flags |= SEEN;
			data->অঙ্ক_not_in_graph++;

			যদি (data->অঙ্ক_not_in_graph >= data->limit) {
				result = 1;
				ভঙ্গ;
			}

			commit_list_ঢোকাও(parent->item, &স্তূপ);
		}
	}

	commit_list_free(স্তূপ);
	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক should_write_commit_graph(সংরচনা gc_config *cfg UNUSED)
{
	পূর্ণাঙ্ক result;
	সংরচনা cg_auto_data data;

	data.অঙ্ক_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	যদি (!data.limit)
		প্রত্যাগম 0;
	যদি (data.limit < 0)
		প্রত্যাগম 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক run_write_commit_graph(সংরচনা maintenance_run_opts *opts)
{
	সংরচনা child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", রিক্ত);

	যদি (opts->quiet)
		strvec_ঠেলো(&child.args, "--no-progress");
	অন্যথা
		strvec_ঠেলো(&child.args, "--progress");

	প্রত্যাগম !!run_command(&child);
}

স্থির পূর্ণাঙ্ক maintenance_task_commit_graph(সংরচনা maintenance_run_opts *opts,
					 সংরচনা gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	যদি (!the_repository->settings.core_commit_graph)
		প্রত্যাগম 0;

	যদি (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		প্রত্যাগম 1;
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক fetch_remote(সংরচনা remote *remote, শূন্য *cbdata)
{
	সংরচনা maintenance_run_opts *opts = cbdata;
	সংরচনা child_process child = CHILD_PROCESS_INIT;

	যদি (remote->skip_default_update)
		প্রত্যাগম 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     রিক্ত);

	যদি (opts->quiet)
		strvec_ঠেলো(&child.args, "--quiet");

	প্রত্যাগম !!run_command(&child);
}

স্থির পূর্ণাঙ্ক maintenance_task_prefetch(সংরচনা maintenance_run_opts *opts,
				     সংরচনা gc_config *cfg UNUSED)
{
	যদি (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		প্রত্যাগম 1;
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক maintenance_task_gc_foreground(সংরচনা maintenance_run_opts *opts,
					  সংরচনা gc_config *cfg)
{
	প্রত্যাগম gc_foreground_tasks(opts, cfg);
}

স্থির পূর্ণাঙ্ক maintenance_task_gc_background(সংরচনা maintenance_run_opts *opts,
					  সংরচনা gc_config *cfg UNUSED)
{
	সংরচনা child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_ঠেলো(&child.args, "gc");

	যদি (opts->auto_flag)
		strvec_ঠেলো(&child.args, "--auto");
	যদি (opts->quiet)
		strvec_ঠেলো(&child.args, "--quiet");
	অন্যথা
		strvec_ঠেলো(&child.args, "--no-quiet");
	strvec_ঠেলো(&child.args, "--no-detach");
	strvec_ঠেলো(&child.args, "--skip-foreground-tasks");

	প্রত্যাগম run_command(&child);
}

স্থির পূর্ণাঙ্ক gc_condition(সংরচনা gc_config *cfg)
{
	সংরচনা odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	প্রত্যাগম odb_optimize_required(the_repository->objects, &opts);
}

স্থির পূর্ণাঙ্ক prune_packed(সংরচনা maintenance_run_opts *opts)
{
	সংরচনা child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_ঠেলো(&child.args, "prune-packed");

	যদি (opts->quiet)
		strvec_ঠেলো(&child.args, "--quiet");

	প্রত্যাগম !!run_command(&child);
}

সংরচনা write_loose_object_data {
	FILE *in;
	পূর্ণাঙ্ক গুনতি;
	পূর্ণাঙ্ক batch_size;
};

স্থির পূর্ণাঙ্ক loose_object_auto_limit = 100;

স্থির পূর্ণাঙ্ক loose_object_গুনতি(ধ্রুব সংরচনা object_id *oid UNUSED,
			      ধ্রুব বর্ণ *path UNUSED,
			      শূন্য *data)
{
	পূর্ণাঙ্ক *গুনতি = (পূর্ণাঙ্ক*)data;
	যদি (++(*গুনতি) >= loose_object_auto_limit)
		প্রত্যাগম 1;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক loose_object_auto_condition(সংরচনা gc_config *cfg UNUSED)
{
	পূর্ণাঙ্ক গুনতি = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	যদি (!loose_object_auto_limit)
		প্রত্যাগম 0;
	যদি (loose_object_auto_limit < 0)
		প্রত্যাগম 1;

	প্রত্যাগম for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_গুনতি,
					     রিক্ত, রিক্ত, &গুনতি);
}

স্থির পূর্ণাঙ্ক bail_on_loose(ধ্রুব সংরচনা object_id *oid UNUSED,
			 ধ্রুব বর্ণ *path UNUSED,
			 শূন্য *data UNUSED)
{
	প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক write_loose_object_to_stdin(ধ্রুব সংরচনা object_id *oid,
				       ধ্রুব বর্ণ *path UNUSED,
				       শূন্য *data)
{
	সংরচনা write_loose_object_data *d = (সংরচনা write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	প্রত্যাগম ++(d->গুনতি) > d->batch_size;
}

স্থির পূর্ণাঙ্ক pack_loose(সংরচনা maintenance_run_opts *opts)
{
	সংরচনা repository *r = the_repository;
	পূর্ণাঙ্ক result = 0;
	সংরচনা write_loose_object_data data;
	সংরচনা child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	যদি (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   রিক্ত, রিক্ত, রিক্ত))
		প্রত্যাগম 0;

	pack_proc.git_cmd = 1;

	strvec_ঠেলো(&pack_proc.args, "pack-objects");
	যদি (opts->quiet)
		strvec_ঠেলো(&pack_proc.args, "--quiet");
	অন্যথা
		strvec_ঠেলো(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	যদি (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		প্রত্যাগম 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.গুনতি = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	যদি (!data.batch_size)
		data.batch_size = INT_MAX;
	অন্যথা যদি (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      রিক্ত, রিক্ত, &data);

	fclose(data.in);

	যদি (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক maintenance_task_loose_objects(সংরচনা maintenance_run_opts *opts,
					  সংরচনা gc_config *cfg UNUSED)
{
	প্রত্যাগম prune_packed(opts) || pack_loose(opts);
}

স্থির পূর্ণাঙ্ক incremental_repack_auto_condition(সংরচনা gc_config *cfg UNUSED)
{
	সংরচনা packed_git *p;
	পূর্ণাঙ্ক incremental_repack_auto_limit = 10;
	পূর্ণাঙ্ক গুনতি = 0;

	prepare_repo_settings(the_repository);
	যদি (!the_repository->settings.core_multi_pack_index)
		প্রত্যাগম 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	যদি (!incremental_repack_auto_limit)
		প্রত্যাগম 0;
	যদি (incremental_repack_auto_limit < 0)
		প্রত্যাগম 1;

	repo_for_each_pack(the_repository, p) {
		যদি (গুনতি >= incremental_repack_auto_limit)
			ভঙ্গ;
		যদি (!p->multi_pack_index)
			গুনতি++;
	}

	প্রত্যাগম গুনতি >= incremental_repack_auto_limit;
}

স্থির পূর্ণাঙ্ক multi_pack_index_write(সংরচনা maintenance_run_opts *opts)
{
	সংরচনা child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", রিক্ত);

	যদি (opts->quiet)
		strvec_ঠেলো(&child.args, "--no-progress");
	অন্যথা
		strvec_ঠেলো(&child.args, "--progress");

	যদি (run_command(&child))
		প্রত্যাগম error(_("failed to write multi-pack-index"));

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক multi_pack_index_expire(সংরচনা maintenance_run_opts *opts)
{
	সংরচনা child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", রিক্ত);

	যদি (opts->quiet)
		strvec_ঠেলো(&child.args, "--no-progress");
	অন্যথা
		strvec_ঠেলো(&child.args, "--progress");

	যদি (run_command(&child))
		প্রত্যাগম error(_("'git multi-pack-index expire' failed"));

	প্রত্যাগম 0;
}

#define TWO_GIGABYTES (INT32_MAX)

স্থির off_t get_auto_pack_size(শূন্য)
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
	সংরচনা packed_git *p;
	সংরচনা repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		যদি (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} অন্যথা যদি (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	যদি (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	প্রত্যাগম result_size;
}

স্থির পূর্ণাঙ্ক multi_pack_index_repack(সংরচনা maintenance_run_opts *opts)
{
	সংরচনা child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", রিক্ত);

	যদি (opts->quiet)
		strvec_ঠেলো(&child.args, "--no-progress");
	অন্যথা
		strvec_ঠেলো(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	যদি (run_command(&child))
		প্রত্যাগম error(_("'git multi-pack-index repack' failed"));

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক maintenance_task_incremental_repack(সংরচনা maintenance_run_opts *opts,
					       সংরচনা gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	যদি (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		প্রত্যাগম 0;
	}

	যদি (multi_pack_index_write(opts))
		প্রত্যাগম 1;
	যদি (multi_pack_index_expire(opts))
		প্রত্যাগম 1;
	যদি (multi_pack_index_repack(opts))
		প্রত্যাগম 1;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক maintenance_task_geometric_repack(সংরচনা maintenance_run_opts *opts,
					     সংরচনা gc_config *cfg)
{
	সংরচনা odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	যদি (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	প্রত্যাগম odb_optimize(the_repository->objects, &odb_opts);
}

স্থির পূর্ণাঙ্ক geometric_repack_auto_condition(সংরচনা gc_config *cfg)
{
	সংরচনা odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	প্রত্যাগম odb_optimize_required(the_repository->objects, &opts);
}

প্রকারনির্দেশ পূর্ণাঙ্ক (*maintenance_task_fn)(সংরচনা maintenance_run_opts *opts,
				   সংরচনা gc_config *cfg);
প্রকারনির্দেশ পূর্ণাঙ্ক (*maintenance_auto_fn)(সংরচনা gc_config *cfg);

সংরচনা maintenance_task {
	ধ্রুব বর্ণ *name;

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

স্থির ধ্রুব সংরচনা maintenance_task tasks[] = {
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

গণনা task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

গণনা auto_gc_hook_result {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

স্থির পূর্ণাঙ্ক maybe_run_task(ধ্রুব সংরচনা maintenance_task *task,
			  সংরচনা repository *repo,
			  সংরচনা maintenance_run_opts *opts,
			  সংরচনা gc_config *cfg,
			  গণনা task_phase phase,
			  গণনা auto_gc_hook_result *auto_gc_hook_result)
{
	পূর্ণাঙ্ক foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ধ্রুব বর্ণ *region = foreground ? "maintenance foreground" : "maintenance";
	পূর্ণাঙ্ক ret = 0;

	যদি (!fn)
		প্রত্যাগম 0;
	যদি (opts->auto_flag) {
		যদি (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			প্রত্যাগম 0;

		যদি (!task->auto_condition || !task->auto_condition(cfg))
			প্রত্যাগম 0;

		যদি (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		যদি (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			প্রত্যাগম 0;
	}

	trace2_region_enter(region, task->name, repo);
	যদি (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক maintenance_run_tasks(সংরচনা maintenance_run_opts *opts,
				 সংরচনা gc_config *cfg)
{
	পূর্ণাঙ্ক result = 0;
	সংরচনা lock_file lk;
	সংরচনা repository *r = the_repository;
	বর্ণ *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	গণনা auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	যদি (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		যদি (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		মুক্ত(lock_path);
		প্রত্যাগম 0;
	}
	মুক্ত(lock_path);

	প্রতি (size_t i = 0; i < opts->tasks_nr; i++)
		যদি (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	যদি (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	প্রতি (size_t i = 0; i < opts->tasks_nr; i++)
		যদি (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	প্রত্যাগম result;
}

গণনা maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

সংরচনা maintenance_strategy {
	সংরচনা {
		unsigned type;
		গণনা schedule_priority schedule;
	} tasks[TASK_গুনতি];
};

স্থির ধ্রুব সংরচনা maintenance_strategy none_strategy = { 0 };

স্থির ধ্রুব সংরচনা maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

স্থির ধ্রুব সংরচনা maintenance_strategy incremental_strategy = {
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

স্থির ধ্রুব সংরচনা maintenance_strategy geometric_strategy = {
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

স্থির সংরচনা maintenance_strategy parse_maintenance_strategy(ধ্রুব বর্ণ *name)
{
	যদি (!strcasecmp(name, "incremental"))
		প্রত্যাগম incremental_strategy;
	যদি (!strcasecmp(name, "gc"))
		প্রত্যাগম gc_strategy;
	যদি (!strcasecmp(name, "geometric"))
		প্রত্যাগম geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

স্থির শূন্য initialize_task_config(সংরচনা maintenance_run_opts *opts,
				   ধ্রুব সংরচনা string_list *selected_tasks)
{
	সংরচনা strbuf config_name = STRBUF_INIT;
	সংরচনা maintenance_strategy strategy;
	গণনা maintenance_type type;
	ধ্রুব বর্ণ *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	যদি (selected_tasks->nr) {
		প্রতি (size_t i = 0; i < selected_tasks->nr; i++) {
			গণনা maintenance_task_label label = (intptr_t)selected_tasks->বস্তুসমূহ[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		প্রত্যাগম;
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
	যদি (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} অন্যথা {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	যদি (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	প্রতি (size_t i = 0; i < TASK_গুনতি; i++) {
		পূর্ণাঙ্ক config_মান;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		যদি (!repo_config_get_bool(the_repository, config_name.buf, &config_মান))
			strategy.tasks[i].type = config_মান ? type : 0;
		যদি (!(strategy.tasks[i].type & type))
			অনুবর্ত;

		যদি (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			যদি (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			যদি (strategy.tasks[i].schedule < opts->schedule)
				অনুবর্ত;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

স্থির পূর্ণাঙ্ক task_option_parse(ধ্রুব সংরচনা option *opt,
			     ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা string_list *selected_tasks = opt->মান;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	প্রতি (i = 0; i < TASK_গুনতি; i++)
		যদি (!strcasecmp(tasks[i].name, arg))
			ভঙ্গ;
	যদি (i >= TASK_গুনতি) {
		error(_("'%s' is not a valid task"), arg);
		প্রত্যাগম 1;
	}

	যদি (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		প্রত্যাগম 1;
	}

	string_list_append(selected_tasks, arg)->util = (শূন্য *)(intptr_t)i;

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক maintenance_run(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			   সংরচনা repository *repo UNUSED)
{
	সংরচনা maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	সংরচনা string_list selected_tasks = STRING_LIST_INIT_DUP;
	সংরচনা gc_config cfg = GC_CONFIG_INIT;
	সংরচনা option builtin_maintenance_run_options[] = {
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
	পূর্ণাঙ্ক ret;

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

	যদি (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	প্রত্যাগম ret;
}

স্থির বর্ণ *get_maintpath(শূন্য)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	ধ্রুব বর্ণ *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	প্রত্যাগম strbuf_detach(&sb, রিক্ত);
}

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	রিক্ত
};

স্থির পূর্ণাঙ্ক maintenance_register(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
				সংরচনা repository *repo UNUSED)
{
	বর্ণ *config_file = রিক্ত;
	সংরচনা option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	পূর্ণাঙ্ক found = 0;
	ধ্রুব বর্ণ *চাবি = "maintenance.repo";
	বর্ণ *maintpath = get_maintpath();
	সংরচনা string_list_item *item;
	ধ্রুব সংরচনা string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	যদি (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	যদি (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	যদি (!repo_config_get_string_multi(the_repository, চাবি, &list)) {
		for_each_string_list_item(item, list) {
			যদি (!strcmp(maintpath, item->string)) {
				found = 1;
				ভঙ্গ;
			}
		}
	}

	যদি (!found) {
		পূর্ণাঙ্ক rc;
		বর্ণ *global_config_file = রিক্ত;

		যদি (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		যদি (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, রিক্ত, 0);
		মুক্ত(global_config_file);

		যদি (rc)
			die(_("unable to add '%s' value of '%s'"),
			    চাবি, maintpath);
	}

	মুক্ত(maintpath);
	প্রত্যাগম 0;
}

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	রিক্ত
};

স্থির পূর্ণাঙ্ক maintenance_unregister(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
				  সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক force = 0;
	বর্ণ *config_file = রিক্ত;
	সংরচনা option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	ধ্রুব বর্ণ *চাবি = "maintenance.repo";
	বর্ণ *maintpath = get_maintpath();
	পূর্ণাঙ্ক found = 0;
	সংরচনা string_list_item *item;
	ধ্রুব সংরচনা string_list *list;
	সংরচনা config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	যদি (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	যদি (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	যদি (!(config_file
	      ? git_configset_get_string_multi(&cs, চাবি, &list)
	      : repo_config_get_string_multi(the_repository, চাবি, &list))) {
		for_each_string_list_item(item, list) {
			যদি (!strcmp(maintpath, item->string)) {
				found = 1;
				ভঙ্গ;
			}
		}
	}

	যদি (found) {
		পূর্ণাঙ্ক rc;
		বর্ণ *global_config_file = রিক্ত;

		যদি (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		যদি (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, চাবি, রিক্ত, maintpath, রিক্ত,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_মান);
		মুক্ত(global_config_file);

		যদি (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    চাবি, maintpath);
	} অন্যথা যদি (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	মুক্ত(maintpath);
	প্রত্যাগম 0;
}

স্থির ধ্রুব বর্ণ *get_frequency(গণনা schedule_priority schedule)
{
	বিকল্প (schedule) {
	পক্ষ SCHEDULE_HOURLY:
		প্রত্যাগম "hourly";
	পক্ষ SCHEDULE_DAILY:
		প্রত্যাগম "daily";
	পক্ষ SCHEDULE_WEEKLY:
		প্রত্যাগম "weekly";
	স্বভাব:
		BUG("invalid schedule %d", schedule);
	}
}

স্থির ধ্রুব বর্ণ *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	রিক্ত
};

স্থির ধ্রুব বর্ণ *get_extra_config_parameters(শূন্য) {
	স্থির ধ্রুব বর্ণ *result = রিক্ত;
	সংরচনা strbuf builder = STRBUF_INIT;

	যদি (result)
		প্রত্যাগম result;

	প্রতি (ধ্রুব বর্ণ **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, রিক্ত);
	প্রত্যাগম result;
}

স্থির ধ্রুব বর্ণ *get_extra_launchctl_strings(শূন্য) {
	স্থির ধ্রুব বর্ণ *result = রিক্ত;
	সংরচনা strbuf builder = STRBUF_INIT;

	যদি (result)
		প্রত্যাগম result;

	প্রতি (ধ্রুব বর্ণ **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, রিক্ত);
	প্রত্যাগম result;
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
স্থির পূর্ণাঙ্ক get_schedule_cmd(ধ্রুব বর্ণ *cmd, পূর্ণাঙ্ক *is_available, বর্ণ **out)
{
	বর্ণ *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	সংরচনা string_list_item *item;
	সংরচনা string_list list = STRING_LIST_INIT_NODUP;

	যদি (!testing) {
		যদি (out)
			*out = xstrdup(cmd);
		প্রত্যাগম 0;
	}

	যদি (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		সংরচনা string_list pair = STRING_LIST_INIT_NODUP;

		যদি (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			অনুবর্ত;

		যদি (!strcmp(cmd, pair.বস্তুসমূহ[0].string)) {
			যদি (out)
				*out = xstrdup(pair.বস্তুসমূহ[1].string);
			যদি (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			গচ্ছ out;
		}

		string_list_clear(&pair, 0);
	}

	যদি (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	মুক্ত(testing);
	প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক get_random_minute(শূন্য)
{
	/* Use a static value when under tests. */
	যদি (getenv("GIT_TEST_MAINT_SCHEDULER"))
		প্রত্যাগম 13;

	প্রত্যাগম git_rand(0) % 60;
}

স্থির পূর্ণাঙ্ক is_launchctl_available(শূন্য)
{
	পূর্ণাঙ্ক is_available;
	যদি (get_schedule_cmd("launchctl", &is_available, রিক্ত))
		প্রত্যাগম is_available;

#ifdef __APPLE__
	প্রত্যাগম 1;
#অন্যথা
	প্রত্যাগম 0;
#endif
}

স্থির বর্ণ *launchctl_service_name(ধ্রুব বর্ণ *frequency)
{
	সংরচনা strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	প্রত্যাগম strbuf_detach(&label, রিক্ত);
}

স্থির বর্ণ *launchctl_service_filename(ধ্রুব বর্ণ *name)
{
	বর্ণ *expanded;
	সংরচনা strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	যদি (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	প্রত্যাগম expanded;
}

স্থির বর্ণ *launchctl_get_uid(শূন্য)
{
	প্রত্যাগম xstrfmt("gui/%d", getuid());
}

স্থির পূর্ণাঙ্ক launchctl_boot_plist(পূর্ণাঙ্ক enable, ধ্রুব বর্ণ *filename)
{
	বর্ণ *cmd;
	পূর্ণাঙ্ক result;
	সংরচনা child_process child = CHILD_PROCESS_INIT;
	বর্ণ *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", রিক্ত, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, রিক্ত);

	child.no_stderr = 1;
	child.no_stdout = 1;

	যদি (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	মুক্ত(cmd);
	মুক্ত(uid);
	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক launchctl_remove_plist(গণনা schedule_priority schedule)
{
	ধ্রুব বর্ণ *frequency = get_frequency(schedule);
	বর্ণ *name = launchctl_service_name(frequency);
	বর্ণ *filename = launchctl_service_filename(name);
	পূর্ণাঙ্ক result = launchctl_boot_plist(0, filename);
	unlink(filename);
	মুক্ত(filename);
	মুক্ত(name);
	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক launchctl_remove_plists(শূন্য)
{
	প্রত্যাগম launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

স্থির পূর্ণাঙ্ক launchctl_list_contains_plist(ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *cmd)
{
	সংরচনা child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, রিক্ত);

	child.no_stderr = 1;
	child.no_stdout = 1;

	যদি (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	প্রত্যাগম !finish_command(&child);
}

স্থির পূর্ণাঙ্ক launchctl_schedule_plist(ধ্রুব বর্ণ *exec_path, গণনা schedule_priority schedule)
{
	পূর্ণাঙ্ক i, fd;
	ধ্রুব বর্ণ *preamble, *repeat;
	ধ্রুব বর্ণ *frequency = get_frequency(schedule);
	বর্ণ *name = launchctl_service_name(frequency);
	বর্ণ *filename = launchctl_service_filename(name);
	সংরচনা lock_file lk = LOCK_INIT;
	স্থির unsigned long lock_file_timeout_ms = ULONG_MAX;
	সংরচনা strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	সংরচনা stat st;
	বর্ণ *cmd;
	পূর্ণাঙ্ক minute = get_random_minute();

	get_schedule_cmd("launchctl", রিক্ত, &cmd);
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

	বিকল্প (schedule) {
	পক্ষ SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		প্রতি (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ভঙ্গ;

	পক্ষ SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		প্রতি (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ভঙ্গ;

	পক্ষ SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		ভঙ্গ;

	স্বভাব:
		/* unreachable */
		ভঙ্গ;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	যদি (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	যদি ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	যদি (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	অন্যথা {
		যদি (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		যদি (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	মুক্ত(filename);
	মুক্ত(name);
	মুক্ত(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক launchctl_add_plists(শূন্য)
{
	ধ্রুব বর্ণ *exec_path = git_exec_path();

	প্রত্যাগম launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

স্থির পূর্ণাঙ্ক launchctl_update_schedule(পূর্ণাঙ্ক run_maintenance, পূর্ণাঙ্ক fd UNUSED)
{
	যদি (run_maintenance)
		প্রত্যাগম launchctl_add_plists();
	অন্যথা
		প্রত্যাগম launchctl_remove_plists();
}

স্থির পূর্ণাঙ্ক is_schtasks_available(শূন্য)
{
	পূর্ণাঙ্ক is_available;
	যদি (get_schedule_cmd("schtasks", &is_available, রিক্ত))
		প্রত্যাগম is_available;

#ifdef GIT_WINDOWS_NATIVE
	প্রত্যাগম 1;
#অন্যথা
	প্রত্যাগম 0;
#endif
}

স্থির বর্ণ *schtasks_task_name(ধ্রুব বর্ণ *frequency)
{
	সংরচনা strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	প্রত্যাগম strbuf_detach(&label, রিক্ত);
}

স্থির পূর্ণাঙ্ক schtasks_remove_task(গণনা schedule_priority schedule)
{
	বর্ণ *cmd;
	সংরচনা child_process child = CHILD_PROCESS_INIT;
	ধ্রুব বর্ণ *frequency = get_frequency(schedule);
	বর্ণ *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", রিক্ত, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", রিক্ত);
	মুক্ত(name);
	মুক্ত(cmd);

	প্রত্যাগম run_command(&child);
}

স্থির পূর্ণাঙ্ক schtasks_remove_tasks(শূন্য)
{
	প্রত্যাগম schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

স্থির পূর্ণাঙ্ক schtasks_schedule_task(ধ্রুব বর্ণ *exec_path, গণনা schedule_priority schedule)
{
	বর্ণ *cmd;
	পূর্ণাঙ্ক result;
	সংরচনা child_process child = CHILD_PROCESS_INIT;
	ধ্রুব বর্ণ *xml;
	সংরচনা tempfile *tfile;
	ধ্রুব বর্ণ *frequency = get_frequency(schedule);
	বর্ণ *name = schtasks_task_name(frequency);
	সংরচনা strbuf tfilename = STRBUF_INIT;
	পূর্ণাঙ্ক minute = get_random_minute();

	get_schedule_cmd("schtasks", রিক্ত, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	যদি (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	বিকল্প (schedule) {
	পক্ষ SCHEDULE_HOURLY:
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
		ভঙ্গ;

	পক্ষ SCHEDULE_DAILY:
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
		ভঙ্গ;

	পক্ষ SCHEDULE_WEEKLY:
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
		ভঙ্গ;

	স্বভাব:
		ভঙ্গ;
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
				  get_tempfile_path(tfile), রিক্ত);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	যদি (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	মুক্ত(name);
	মুক্ত(cmd);
	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক schtasks_schedule_tasks(শূন্য)
{
	ধ্রুব বর্ণ *exec_path = git_exec_path();

	প্রত্যাগম schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

স্থির পূর্ণাঙ্ক schtasks_update_schedule(পূর্ণাঙ্ক run_maintenance, পূর্ণাঙ্ক fd UNUSED)
{
	যদি (run_maintenance)
		প্রত্যাগম schtasks_schedule_tasks();
	অন্যথা
		প্রত্যাগম schtasks_remove_tasks();
}

MAYBE_UNUSED
স্থির পূর্ণাঙ্ক check_crontab_process(ধ্রুব বর্ণ *cmd)
{
	সংরচনা child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_ঠেলো(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	যদি (start_command(&child))
		প্রত্যাগম 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক is_crontab_available(শূন্য)
{
	বর্ণ *cmd;
	পূর্ণাঙ্ক is_available;
	পূর্ণাঙ্ক ret;

	যদি (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		গচ্ছ out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#অন্যথা
	ret = check_crontab_process(cmd);
#endif

out:
	মুক্ত(cmd);
	প্রত্যাগম ret;
}

#define BEGIN_সারি "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_সারি "# END GIT MAINTENANCE SCHEDULE"

স্থির পূর্ণাঙ্ক crontab_update_schedule(পূর্ণাঙ্ক run_maintenance, পূর্ণাঙ্ক fd)
{
	বর্ণ *cmd;
	পূর্ণাঙ্ক result = 0;
	পূর্ণাঙ্ক in_old_region = 0;
	সংরচনা child_process crontab_list = CHILD_PROCESS_INIT;
	সংরচনা child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	সংরচনা strbuf সারি = STRBUF_INIT;
	সংরচনা tempfile *tmpedit = রিক্ত;
	পূর্ণাঙ্ক minute = get_random_minute();

	get_schedule_cmd("crontab", রিক্ত, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_ঠেলো(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	যদি (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		গচ্ছ out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	যদি (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		গচ্ছ out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	যদি (!cron_in) {
		result = error(_("failed to open temporary file"));
		গচ্ছ out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	যাবত্ (!strbuf_getline_lf(&সারি, cron_list)) {
		যদি (!in_old_region && !strcmp(সারি.buf, BEGIN_সারি))
			in_old_region = 1;
		অন্যথা যদি (in_old_region && !strcmp(সারি.buf, END_সারি))
			in_old_region = 0;
		অন্যথা যদি (!in_old_region)
			fprintf(cron_in, "%s\n", সারি.buf);
	}
	strbuf_release(&সারি);

	যদি (run_maintenance) {
		সংরচনা strbuf সারি_format = STRBUF_INIT;
		ধ্রুব বর্ণ *exec_path = git_exec_path();

		fprintf(cron_in, "%s\n", BEGIN_সারি);
		fprintf(cron_in,
			"# The following schedule was created by Git\n");
		fprintf(cron_in, "# Any edits made in this region might be\n");
		fprintf(cron_in,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&সারি_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_path, exec_path, get_extra_config_parameters());
		fprintf(cron_in, সারি_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_in, সারি_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_in, সারি_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&সারি_format);

		fprintf(cron_in, "\n%s\n", END_সারি);
	}

	fflush(cron_in);

	strvec_split(&crontab_edit.args, cmd);
	strvec_ঠেলো(&crontab_edit.args, get_tempfile_path(tmpedit));
	crontab_edit.git_cmd = 0;

	যদি (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		গচ্ছ out;
	}

	যদি (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	অন্যথা
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	মুক্ত(cmd);
	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক real_is_systemd_timer_available(শূন্য)
{
	সংরচনা child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", রিক্ত);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	যদি (start_command(&child))
		প্রত্যাগম 0;
	যদি (finish_command(&child))
		প্রত্যাগম 0;
	প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক is_systemd_timer_available(শূন্য)
{
	পূর্ণাঙ্ক is_available;

	যদি (get_schedule_cmd("systemctl", &is_available, রিক্ত))
		প্রত্যাগম is_available;

	প্রত্যাগম real_is_systemd_timer_available();
}

স্থির বর্ণ *xdg_config_home_systemd(ধ্রুব বর্ণ *filename)
{
	প্রত্যাগম xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

স্থির পূর্ণাঙ্ক systemd_timer_delete_timer_file(গণনা schedule_priority priority)
{
	পূর্ণাঙ্ক ret = 0;
	ধ্রুব বর্ণ *frequency = get_frequency(priority);
	বর্ণ *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	বর্ণ *filename = xdg_config_home_systemd(local_timer_name);

	যদি (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	মুক্ত(filename);
	মুক্ত(local_timer_name);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক systemd_timer_delete_service_template(শূন্য)
{
	পূর্ণাঙ্ক ret = 0;
	বর্ণ *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	বর্ণ *filename = xdg_config_home_systemd(local_service_name);
	যদি (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	মুক্ত(filename);
	মুক্ত(local_service_name);
	প্রত্যাগম ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
স্থির পূর্ণাঙ্ক systemd_timer_write_timer_file(গণনা schedule_priority schedule,
					  পূর্ণাঙ্ক minute)
{
	পূর্ণাঙ্ক res = -1;
	বর্ণ *filename;
	FILE *file;
	ধ্রুব বর্ণ *unit;
	বর্ণ *schedule_pattern = রিক্ত;
	ধ্রুব বর্ণ *frequency = get_frequency(schedule);
	বর্ণ *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	যদি (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		গচ্ছ error;
	}
	file = fopen_or_warn(filename, "w");
	যদি (!file)
		গচ্ছ error;

	বিকল্প (schedule) {
	পক্ষ SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		ভঙ্গ;

	পক্ষ SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		ভঙ্গ;

	পক্ষ SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		ভঙ্গ;

	স্বভাব:
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
	যদি (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		গচ্ছ error;
	}
	যদি (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		গচ্ছ error;
	}

	res = 0;

error:
	মুক্ত(schedule_pattern);
	মুক্ত(local_timer_name);
	মুক্ত(filename);
	প্রত্যাগম res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
স্থির পূর্ণাঙ্ক systemd_timer_write_service_template(ধ্রুব বর্ণ *exec_path)
{
	পূর্ণাঙ্ক res = -1;
	বর্ণ *filename;
	FILE *file;
	ধ্রুব বর্ণ *unit;
	বর্ণ *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	যদি (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		গচ্ছ error;
	}
	file = fopen_or_warn(filename, "w");
	যদি (!file)
		গচ্ছ error;

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
	যদি (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		গচ্ছ error;
	}
	যদি (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		গচ্ছ error;
	}

	res = 0;

error:
	মুক্ত(local_service_name);
	মুক্ত(filename);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক systemd_timer_enable_unit(পূর্ণাঙ্ক enable,
				     গণনা schedule_priority schedule,
				     পূর্ণাঙ্ক minute)
{
	বর্ণ *cmd = রিক্ত;
	সংরচনা child_process child = CHILD_PROCESS_INIT;
	ধ্রুব বর্ণ *frequency = get_frequency(schedule);
	পূর্ণাঙ্ক ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	যদি (!enable) {
		child.no_stderr = 1;
	} অন্যথা যদি (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		গচ্ছ out;
	}

	get_schedule_cmd("systemctl", রিক্ত, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", রিক্ত);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	যদি (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		গচ্ছ out;
	}

	যদি (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		যদি (enable) {
			ret = error(_("failed to run systemctl"));
			গচ্ছ out;
		}
	}

	ret = 0;

out:
	মুক্ত(cmd);
	প্রত্যাগম ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
স্থির শূন্য systemd_timer_delete_stale_timer_templates(শূন্য)
{
	বর্ণ *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	বর্ণ *filename = xdg_config_home_systemd(timer_template_name);

	যদি (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	মুক্ত(filename);
	মুক্ত(timer_template_name);
}

স্থির পূর্ণাঙ্ক systemd_timer_delete_unit_files(শূন্য)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	প্রত্যাগম systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

স্থির পূর্ণাঙ্ক systemd_timer_delete_units(শূন্য)
{
	পূর্ণাঙ্ক minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	প্রত্যাগম systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

স্থির পূর্ণাঙ্ক systemd_timer_setup_units(শূন্য)
{
	পূর্ণাঙ্ক minute = get_random_minute();
	ধ্রুব বর্ণ *exec_path = git_exec_path();

	পূর্ণাঙ্ক ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	যদি (ret)
		systemd_timer_delete_units();
	অন্যথা
		systemd_timer_delete_stale_timer_templates();

	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক systemd_timer_update_schedule(পূর্ণাঙ্ক run_maintenance, পূর্ণাঙ্ক fd UNUSED)
{
	যদি (run_maintenance)
		প্রত্যাগম systemd_timer_setup_units();
	অন্যথা
		প্রত্যাগম systemd_timer_delete_units();
}

গণনা scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

স্থির ধ্রুব সংরচনা {
	ধ্রুব বর্ণ *name;
	পূর্ণাঙ্ক (*is_available)(শূন্য);
	পূর্ণাঙ্ক (*update_schedule)(পূর্ণাঙ্ক run_maintenance, পূর্ণাঙ্ক fd);
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

স্থির গণনা scheduler parse_scheduler(ধ্রুব বর্ণ *মান)
{
	যদি (!মান)
		প্রত্যাগম SCHEDULER_INVALID;
	অন্যথা যদি (!strcasecmp(মান, "auto"))
		প্রত্যাগম SCHEDULER_AUTO;
	অন্যথা যদি (!strcasecmp(মান, "cron") || !strcasecmp(মান, "crontab"))
		প্রত্যাগম SCHEDULER_CRON;
	অন্যথা যদি (!strcasecmp(মান, "systemd") ||
		 !strcasecmp(মান, "systemd-timer"))
		প্রত্যাগম SCHEDULER_SYSTEMD;
	অন্যথা যদি (!strcasecmp(মান, "launchctl"))
		প্রত্যাগম SCHEDULER_LAUNCHCTL;
	অন্যথা যদি (!strcasecmp(মান, "schtasks"))
		প্রত্যাগম SCHEDULER_SCHTASKS;
	অন্যথা
		প্রত্যাগম SCHEDULER_INVALID;
}

স্থির পূর্ণাঙ্ক maintenance_opt_scheduler(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
				     পূর্ণাঙ্ক unset)
{
	গণনা scheduler *scheduler = opt->মান;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	যদি (*scheduler == SCHEDULER_INVALID)
		প্রত্যাগম error(_("unrecognized --scheduler argument '%s'"), arg);
	প্রত্যাগম 0;
}

সংরচনা maintenance_start_opts {
	গণনা scheduler scheduler;
};

স্থির গণনা scheduler resolve_scheduler(গণনা scheduler scheduler)
{
	যদি (scheduler != SCHEDULER_AUTO)
		প্রত্যাগম scheduler;

#যদি defined(__APPLE__)
	প্রত্যাগম SCHEDULER_LAUNCHCTL;

#নোচেদ্যদি defined(GIT_WINDOWS_NATIVE)
	প্রত্যাগম SCHEDULER_SCHTASKS;

#নোচেদ্যদি defined(__linux__)
	যদি (is_systemd_timer_available())
		প্রত্যাগম SCHEDULER_SYSTEMD;
	অন্যথা যদি (is_crontab_available())
		প্রত্যাগম SCHEDULER_CRON;
	অন্যথা
		die(_("neither systemd timers nor crontab are available"));

#অন্যথা
	প্রত্যাগম SCHEDULER_CRON;
#endif
}

স্থির শূন্য validate_scheduler(গণনা scheduler scheduler)
{
	যদি (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	যদি (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	যদি (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

স্থির পূর্ণাঙ্ক update_background_schedule(ধ্রুব সংরচনা maintenance_start_opts *opts,
				      পূর্ণাঙ্ক enable)
{
	unsigned পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক result = 0;
	সংরচনা lock_file lk;
	বর্ণ *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	যদি (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		যদি (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		অন্যথা
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		মুক্ত(lock_path);
		প্রত্যাগম -1;
	}

	প্রতি (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		যদি (enable && opts->scheduler == i)
			অনুবর্ত;
		যদি (!scheduler_fn[i].is_available())
			অনুবর্ত;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	যদি (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	মুক্ত(lock_path);
	প্রত্যাগম result;
}

স্থির ধ্রুব বর্ণ *ধ্রুব builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	রিক্ত
};

স্থির পূর্ণাঙ্ক maintenance_start(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			     সংরচনা repository *repo)
{
	সংরচনা maintenance_start_opts opts = { 0 };
	সংরচনা option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	ধ্রুব বর্ণ *register_args[] = { "register", রিক্ত };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	যদি (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	যদি (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	যদি (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, রিক্ত, repo))
		warning(_("failed to add repo to global config"));
	প্রত্যাগম 0;
}

স্থির ধ্রুব বর্ণ *ধ্রুব builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	রিক্ত
};

স্থির পূর্ণাঙ্ক maintenance_stop(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			    সংরচনা repository *repo UNUSED)
{
	সংরচনা option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	যদি (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	প্রত্যাগম update_background_schedule(রিক্ত, 0);
}

স্থির ধ্রুব বর্ণ *ধ্রুব builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	রিক্ত
};

স্থির পূর্ণাঙ্ক maintenance_is_needed(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
				 সংরচনা repository *repo UNUSED)
{
	সংরচনা maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	সংরচনা string_list selected_tasks = STRING_LIST_INIT_DUP;
	সংরচনা gc_config cfg = GC_CONFIG_INIT;
	সংরচনা option options[] = {
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
	যদি (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	যদি (opts.auto_flag) {
		প্রতি (size_t i = 0; i < opts.tasks_nr; i++) {
			যদি (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				ভঙ্গ;
			}
		}
	} অন্যথা {
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

	যদি (is_needed)
		প্রত্যাগম 0;
	প্রত্যাগম 1;
}

স্থির ধ্রুব বর্ণ *ধ্রুব builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	রিক্ত,
};

পূর্ণাঙ্ক cmd_maintenance(পূর্ণাঙ্ক argc,
		    ধ্রুব বর্ণ **argv,
		    ধ্রুব বর্ণ *prefix,
		    সংরচনা repository *repo)
{
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option builtin_maintenance_options[] = {
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
	প্রত্যাগম fn(argc, argv, prefix, repo);
}
