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

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "date.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "tempfile.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "sigchain.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "commit-graph.h"
#അന്തര്ഭാവ "packfile.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "reflog.h"
#അന്തര്ഭാവ "rerere.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "exec-cmd.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hook.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "trace2.h"
#അന്തര്ഭാവ "worktree.h"

#define FAILED_RUN "failed to run %s"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	ശൂന്യ
};

സ്ഥിതം timestamp_t gc_log_expire_time;
സ്ഥിതം ഘടന tempfile *pidfile;
സ്ഥിതം ഘടന lock_file log_lock;
സ്ഥിതം ഘടന string_list pack_garbage = STRING_LIST_INIT_DUP;

സ്ഥിതം ശൂന്യം clean_pack_garbage(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ i;
	വേണ്ടി (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.ഇനങ്ങൾ[i].string);
	string_list_clear(&pack_garbage, 0);
}

സ്ഥിതം ശൂന്യം report_pack_garbage(unsigned seen_bits, സ്ഥിരം അക്ഷരം *path)
{
	എങ്കിൽ (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

സ്ഥിതം ശൂന്യം process_log_file(ശൂന്യം)
{
	ഘടന stat st;
	എങ്കിൽ (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		പൂർണ്ണസംഖ്യ saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} അല്ലെങ്കിൽ എങ്കിൽ (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} അല്ലെങ്കിൽ {
		അക്ഷരം *path = repo_git_path(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(path);
		rollback_lock_file(&log_lock);
		സ്വതന്ത്രം(path);
	}
}

സ്ഥിതം ശൂന്യം process_log_file_at_exit(ശൂന്യം)
{
	fflush(stderr);
	process_log_file();
}

സ്ഥിതം പൂർണ്ണസംഖ്യ gc_config_is_timestamp_never(സ്ഥിരം അക്ഷരം *var)
{
	സ്ഥിരം അക്ഷരം *മൂല്യം;
	timestamp_t expire;

	എങ്കിൽ (!repo_config_get_മൂല്യം(the_repository, var, &മൂല്യം) && മൂല്യം) {
		എങ്കിൽ (parse_expiry_date(മൂല്യം, &expire))
			die(_("failed to parse '%s' value '%s'"), var, മൂല്യം);
		തിരികെ expire == 0;
	}
	തിരികെ 0;
}

ഘടന gc_config {
	പൂർണ്ണസംഖ്യ pack_refs;
	പൂർണ്ണസംഖ്യ prune_reflogs;
	പൂർണ്ണസംഖ്യ cruft_packs;
	unsigned long max_cruft_size;
	പൂർണ്ണസംഖ്യ aggressive_depth;
	പൂർണ്ണസംഖ്യ aggressive_window;
	പൂർണ്ണസംഖ്യ detach_auto;
	അക്ഷരം *gc_log_expire;
	അക്ഷരം *prune_expire;
	അക്ഷരം *prune_worktrees_expire;
	അക്ഷരം *repack_expire_to;
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

സ്ഥിതം ശൂന്യം gc_config_release(ഘടന gc_config *cfg)
{
	സ്വതന്ത്രം(cfg->gc_log_expire);
	സ്വതന്ത്രം(cfg->prune_expire);
	സ്വതന്ത്രം(cfg->prune_worktrees_expire);
}

സ്ഥിതം ശൂന്യം gc_config(ഘടന gc_config *cfg)
{
	സ്ഥിരം അക്ഷരം *മൂല്യം;
	അക്ഷരം *owned = ശൂന്യ;

	എങ്കിൽ (!repo_config_get_മൂല്യം(the_repository, "gc.packrefs", &മൂല്യം)) {
		എങ്കിൽ (മൂല്യം && !strcmp(മൂല്യം, "notbare"))
			cfg->pack_refs = -1;
		അല്ലെങ്കിൽ
			cfg->pack_refs = git_config_bool("gc.packrefs", മൂല്യം);
	}

	എങ്കിൽ (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	എങ്കിൽ (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		സ്വതന്ത്രം(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	എങ്കിൽ (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		സ്വതന്ത്രം(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	എങ്കിൽ (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		സ്വതന്ത്രം(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, ശൂന്യ);
}

എണ്ണൽ schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

സ്ഥിതം എണ്ണൽ schedule_priority parse_schedule(സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	എങ്കിൽ (!മൂല്യം)
		തിരികെ SCHEDULE_NONE;
	എങ്കിൽ (!strcasecmp(മൂല്യം, "hourly"))
		തിരികെ SCHEDULE_HOURLY;
	എങ്കിൽ (!strcasecmp(മൂല്യം, "daily"))
		തിരികെ SCHEDULE_DAILY;
	എങ്കിൽ (!strcasecmp(മൂല്യം, "weekly"))
		തിരികെ SCHEDULE_WEEKLY;
	തിരികെ SCHEDULE_NONE;
}

എണ്ണൽ maintenance_task_label {
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
	TASK_എണ്ണം
};

ഘടന maintenance_run_opts {
	എണ്ണൽ maintenance_task_label *tasks;
	size_t tasks_nr, tasks_alloc;
	പൂർണ്ണസംഖ്യ auto_flag;
	പൂർണ്ണസംഖ്യ detach;
	പൂർണ്ണസംഖ്യ quiet;
	എണ്ണൽ schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

സ്ഥിതം ശൂന്യം maintenance_run_opts_release(ഘടന maintenance_run_opts *opts)
{
	സ്വതന്ത്രം(opts->tasks);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ pack_refs_condition(UNUSED ഘടന gc_config *cfg)
{
	ഘടന string_list included_refs = STRING_LIST_INIT_NODUP;
	ഘടന ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	ഘടന refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	എങ്കിൽ (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		തിരികെ 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	തിരികെ required;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_pack_refs(ഘടന maintenance_run_opts *opts,
				      UNUSED ഘടന gc_config *cfg)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", ശൂന്യ);
	എങ്കിൽ (opts->auto_flag)
		strvec_തള്ളുക(&cmd.args, "--auto");

	തിരികെ run_command(&cmd);
}

ഘടന എണ്ണം_reflog_entries_data {
	ഘടന expire_reflog_policy_cb policy;
	size_t എണ്ണം;
	size_t limit;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ എണ്ണം_reflog_entries(സ്ഥിരം അക്ഷരം *refname UNUSED,
				ഘടന object_id *old_oid, ഘടന object_id *new_oid,
				സ്ഥിരം അക്ഷരം *committer, timestamp_t timestamp,
				പൂർണ്ണസംഖ്യ tz, സ്ഥിരം അക്ഷരം *സന്ദേശം, ശൂന്യം *cb_data)
{
	ഘടന എണ്ണം_reflog_entries_data *data = cb_data;
	എങ്കിൽ (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, സന്ദേശം, &data->policy))
		data->എണ്ണം++;
	തിരികെ data->എണ്ണം >= data->limit;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ reflog_expire_condition(ഘടന gc_config *cfg UNUSED)
{
	timestamp_t now = time(ശൂന്യ);
	ഘടന എണ്ണം_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	പൂർണ്ണസംഖ്യ limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	എങ്കിൽ (!limit)
		തിരികെ 0;
	എങ്കിൽ (limit < 0)
		തിരികെ 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 എണ്ണം_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	തിരികെ data.എണ്ണം >= data.limit;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_reflog_expire(ഘടന maintenance_run_opts *opts UNUSED,
					  ഘടന gc_config *cfg UNUSED)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", ശൂന്യ);
	തിരികെ run_command(&cmd);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_worktree_prune(ഘടന maintenance_run_opts *opts UNUSED,
					   ഘടന gc_config *cfg)
{
	ഘടന child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", ശൂന്യ);
	strvec_തള്ളുക(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	തിരികെ run_command(&prune_worktrees_cmd);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ worktree_prune_condition(ഘടന gc_config *cfg)
{
	ഘടന strbuf buf = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	ഘടന dirent *d;
	DIR *dir = ശൂന്യ;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	എങ്കിൽ (limit <= 0) {
		should_prune = limit < 0;
		പോകുക out;
	}

	എങ്കിൽ (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		പോകുക out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	എങ്കിൽ (!dir)
		പോകുക out;

	ആകുമ്പോൾ (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		അക്ഷരം *wtpath;
		strbuf_reset(&buf);
		എങ്കിൽ (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		സ്വതന്ത്രം(wtpath);
	}

	should_prune = !limit;

out:
	എങ്കിൽ (dir)
		closedir(dir);
	strbuf_release(&buf);
	തിരികെ should_prune;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_rerere_gc(ഘടന maintenance_run_opts *opts UNUSED,
				      ഘടന gc_config *cfg UNUSED)
{
	ഘടന child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", ശൂന്യ);
	തിരികെ run_command(&rerere_cmd);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rerere_gc_condition(ഘടന gc_config *cfg UNUSED)
{
	പൂർണ്ണസംഖ്യ limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	എങ്കിൽ (!limit)
		തിരികെ 0; /* never prune */
	എങ്കിൽ (limit < 0)
		തിരികെ 1; /* always prune */

	തിരികെ rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
സ്ഥിതം സ്ഥിരം അക്ഷരം *lock_repo_for_gc(പൂർണ്ണസംഖ്യ force, pid_t* ret_pid)
{
	ഘടന lock_file lock = LOCK_INIT;
	അക്ഷരം my_host[HOST_NAME_MAX + 1];
	ഘടന strbuf sb = STRBUF_INIT;
	ഘടന stat st;
	uintmax_t pid;
	FILE *fp;
	പൂർണ്ണസംഖ്യ fd;
	അക്ഷരം *pidfile_path;

	എങ്കിൽ (is_tempfile_active(pidfile))
		/* already locked */
		തിരികെ ശൂന്യ;

	എങ്കിൽ (xgethostname(my_host, വലുപ്പം(my_host)))
		xsnprintf(my_host, വലുപ്പം(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	എങ്കിൽ (!force) {
		സ്ഥിതം അക്ഷരം locking_host[HOST_NAME_MAX + 1];
		സ്ഥിതം അക്ഷരം *scan_fmt;
		പൂർണ്ണസംഖ്യ should_exit;

		എങ്കിൽ (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_path, "r");
		memset(locking_host, 0, വലുപ്പം(locking_host));
		should_exit =
			fp != ശൂന്യ &&
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
			time(ശൂന്യ) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, scan_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		എങ്കിൽ (fp)
			fclose(fp);
		എങ്കിൽ (should_exit) {
			എങ്കിൽ (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			സ്വതന്ത്രം(pidfile_path);
			തിരികെ locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	സ്വതന്ത്രം(pidfile_path);
	തിരികെ ശൂന്യ;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ report_last_gc_error(ശൂന്യം)
{
	ഘടന strbuf sb = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret = 0;
	ssize_t len;
	ഘടന stat st;
	അക്ഷരം *gc_log_path = repo_git_path(the_repository, "gc.log");

	എങ്കിൽ (stat(gc_log_path, &st)) {
		എങ്കിൽ (errno == ENOENT)
			പോകുക done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		പോകുക done;
	}

	എങ്കിൽ (st.st_mtime < gc_log_expire_time)
		പോകുക done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	എങ്കിൽ (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	അല്ലെങ്കിൽ എങ്കിൽ (len > 0) {
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
	സ്വതന്ത്രം(gc_log_path);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ gc_foreground_tasks(ഘടന maintenance_run_opts *opts,
			       ഘടന gc_config *cfg)
{
	എങ്കിൽ (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		തിരികെ error(FAILED_RUN, "pack-refs");
	എങ്കിൽ (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		തിരികെ error(FAILED_RUN, "reflog");
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_odb(ഘടന maintenance_run_opts *opts,
				ഘടന gc_config *cfg,
				പൂർണ്ണസംഖ്യ keep_largest_pack,
				പൂർണ്ണസംഖ്യ aggressive)
{
	ഘടന odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	എങ്കിൽ (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	എങ്കിൽ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	എങ്കിൽ (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	തിരികെ odb_optimize(the_repository->objects, &odb_opts);
}

പൂർണ്ണസംഖ്യ cmd_gc(പൂർണ്ണസംഖ്യ argc,
	   സ്ഥിരം അക്ഷരം **argv,
	   സ്ഥിരം അക്ഷരം *prefix,
	   ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ aggressive = 0;
	പൂർണ്ണസംഖ്യ force = 0;
	സ്ഥിരം അക്ഷരം *name;
	pid_t pid;
	പൂർണ്ണസംഖ്യ daemonized = 0;
	പൂർണ്ണസംഖ്യ keep_largest_pack = -1;
	പൂർണ്ണസംഖ്യ skip_foreground_tasks = 0;
	timestamp_t dummy;
	ഘടന maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ഘടന gc_config cfg = GC_CONFIG_INIT;
	സ്ഥിരം അക്ഷരം *prune_expire_sentinel = "sentinel";
	സ്ഥിരം അക്ഷരം *prune_expire_arg = prune_expire_sentinel;
	പൂർണ്ണസംഖ്യ ret;
	ഘടന option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.മൂല്യം = &prune_expire_arg,
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

	എങ്കിൽ (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	എങ്കിൽ (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	എങ്കിൽ (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	എങ്കിൽ (prune_expire_arg != prune_expire_sentinel) {
		സ്വതന്ത്രം(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	എങ്കിൽ (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	എങ്കിൽ (opts.auto_flag) {
		ഘടന odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		എങ്കിൽ (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		എങ്കിൽ (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			പോകുക out;
		}

		എങ്കിൽ (!opts.quiet) {
			എങ്കിൽ (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			അല്ലെങ്കിൽ
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	എങ്കിൽ (opts.detach > 0) {
		ret = report_last_gc_error();
		എങ്കിൽ (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			പോകുക out;

		} അല്ലെങ്കിൽ എങ്കിൽ (ret) {
			/* an I/O error occurred, already reported */
			പോകുക out;
		}

		എങ്കിൽ (!skip_foreground_tasks) {
			എങ്കിൽ (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				പോകുക out;
			}

			എങ്കിൽ (gc_foreground_tasks(&opts, &cfg) < 0)
				die(ശൂന്യ);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = lock_repo_for_gc(force, &pid);
	എങ്കിൽ (name) {
		എങ്കിൽ (opts.auto_flag) {
			ret = 0;
			പോകുക out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	എങ്കിൽ (daemonized) {
		അക്ഷരം *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		സ്വതന്ത്രം(path);
	}

	എങ്കിൽ (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	എങ്കിൽ (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	എങ്കിൽ (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	എങ്കിൽ (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(ശൂന്യ);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	എങ്കിൽ (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	എങ്കിൽ (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     ശൂന്യ);

	എങ്കിൽ (!daemonized) {
		അക്ഷരം *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		സ്വതന്ത്രം(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_opt_schedule(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
				    പൂർണ്ണസംഖ്യ unset)
{
	എണ്ണൽ schedule_priority *priority = opt->മൂല്യം;

	എങ്കിൽ (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	എങ്കിൽ (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	തിരികെ 0;
}

ഘടന cg_auto_data {
	പൂർണ്ണസംഖ്യ അക്കം_not_in_graph;
	പൂർണ്ണസംഖ്യ limit;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ dfs_on_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	ഘടന cg_auto_data *data = (ഘടന cg_auto_data *)cb_data;
	പൂർണ്ണസംഖ്യ result = 0;
	സ്ഥിരം ഘടന object_id *maybe_peeled = ref->oid;
	ഘടന object_id peeled;
	ഘടന commit_list *അട്ടി = ശൂന്യ;
	ഘടന commit *commit;

	എങ്കിൽ (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	എങ്കിൽ (odb_read_object_info(the_repository->objects, maybe_peeled, ശൂന്യ) != OBJ_COMMIT)
		തിരികെ 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	എങ്കിൽ (!commit || commit->object.flags & SEEN)
		തിരികെ 0;
	commit->object.flags |= SEEN;

	എങ്കിൽ (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		തിരികെ 0;

	data->അക്കം_not_in_graph++;

	എങ്കിൽ (data->അക്കം_not_in_graph >= data->limit)
		തിരികെ 1;

	commit_list_തിരുകുക(commit, &അട്ടി);

	ആകുമ്പോൾ (!result && അട്ടി) {
		ഘടന commit_list *parent;

		commit = എടുക്കുക_commit(&അട്ടി);

		വേണ്ടി (parent = commit->parents; parent; parent = parent->next) {
			എങ്കിൽ (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				തുടരുക;

			parent->item->object.flags |= SEEN;
			data->അക്കം_not_in_graph++;

			എങ്കിൽ (data->അക്കം_not_in_graph >= data->limit) {
				result = 1;
				നിർത്തുക;
			}

			commit_list_തിരുകുക(parent->item, &അട്ടി);
		}
	}

	commit_list_free(അട്ടി);
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ should_write_commit_graph(ഘടന gc_config *cfg UNUSED)
{
	പൂർണ്ണസംഖ്യ result;
	ഘടന cg_auto_data data;

	data.അക്കം_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	എങ്കിൽ (!data.limit)
		തിരികെ 0;
	എങ്കിൽ (data.limit < 0)
		തിരികെ 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_write_commit_graph(ഘടന maintenance_run_opts *opts)
{
	ഘടന child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", ശൂന്യ);

	എങ്കിൽ (opts->quiet)
		strvec_തള്ളുക(&child.args, "--no-progress");
	അല്ലെങ്കിൽ
		strvec_തള്ളുക(&child.args, "--progress");

	തിരികെ !!run_command(&child);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_commit_graph(ഘടന maintenance_run_opts *opts,
					 ഘടന gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	എങ്കിൽ (!the_repository->settings.core_commit_graph)
		തിരികെ 0;

	എങ്കിൽ (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		തിരികെ 1;
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fetch_remote(ഘടന remote *remote, ശൂന്യം *cbdata)
{
	ഘടന maintenance_run_opts *opts = cbdata;
	ഘടന child_process child = CHILD_PROCESS_INIT;

	എങ്കിൽ (remote->skip_default_update)
		തിരികെ 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     ശൂന്യ);

	എങ്കിൽ (opts->quiet)
		strvec_തള്ളുക(&child.args, "--quiet");

	തിരികെ !!run_command(&child);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_prefetch(ഘടന maintenance_run_opts *opts,
				     ഘടന gc_config *cfg UNUSED)
{
	എങ്കിൽ (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		തിരികെ 1;
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_gc_foreground(ഘടന maintenance_run_opts *opts,
					  ഘടന gc_config *cfg)
{
	തിരികെ gc_foreground_tasks(opts, cfg);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_gc_background(ഘടന maintenance_run_opts *opts,
					  ഘടന gc_config *cfg UNUSED)
{
	ഘടന child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_തള്ളുക(&child.args, "gc");

	എങ്കിൽ (opts->auto_flag)
		strvec_തള്ളുക(&child.args, "--auto");
	എങ്കിൽ (opts->quiet)
		strvec_തള്ളുക(&child.args, "--quiet");
	അല്ലെങ്കിൽ
		strvec_തള്ളുക(&child.args, "--no-quiet");
	strvec_തള്ളുക(&child.args, "--no-detach");
	strvec_തള്ളുക(&child.args, "--skip-foreground-tasks");

	തിരികെ run_command(&child);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ gc_condition(ഘടന gc_config *cfg)
{
	ഘടന odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	തിരികെ odb_optimize_required(the_repository->objects, &opts);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ prune_packed(ഘടന maintenance_run_opts *opts)
{
	ഘടന child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_തള്ളുക(&child.args, "prune-packed");

	എങ്കിൽ (opts->quiet)
		strvec_തള്ളുക(&child.args, "--quiet");

	തിരികെ !!run_command(&child);
}

ഘടന write_loose_object_data {
	FILE *in;
	പൂർണ്ണസംഖ്യ എണ്ണം;
	പൂർണ്ണസംഖ്യ batch_size;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ loose_object_auto_limit = 100;

സ്ഥിതം പൂർണ്ണസംഖ്യ loose_object_എണ്ണം(സ്ഥിരം ഘടന object_id *oid UNUSED,
			      സ്ഥിരം അക്ഷരം *path UNUSED,
			      ശൂന്യം *data)
{
	പൂർണ്ണസംഖ്യ *എണ്ണം = (പൂർണ്ണസംഖ്യ*)data;
	എങ്കിൽ (++(*എണ്ണം) >= loose_object_auto_limit)
		തിരികെ 1;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ loose_object_auto_condition(ഘടന gc_config *cfg UNUSED)
{
	പൂർണ്ണസംഖ്യ എണ്ണം = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	എങ്കിൽ (!loose_object_auto_limit)
		തിരികെ 0;
	എങ്കിൽ (loose_object_auto_limit < 0)
		തിരികെ 1;

	തിരികെ for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_എണ്ണം,
					     ശൂന്യ, ശൂന്യ, &എണ്ണം);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bail_on_loose(സ്ഥിരം ഘടന object_id *oid UNUSED,
			 സ്ഥിരം അക്ഷരം *path UNUSED,
			 ശൂന്യം *data UNUSED)
{
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ write_loose_object_to_stdin(സ്ഥിരം ഘടന object_id *oid,
				       സ്ഥിരം അക്ഷരം *path UNUSED,
				       ശൂന്യം *data)
{
	ഘടന write_loose_object_data *d = (ഘടന write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	തിരികെ ++(d->എണ്ണം) > d->batch_size;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ pack_loose(ഘടന maintenance_run_opts *opts)
{
	ഘടന repository *r = the_repository;
	പൂർണ്ണസംഖ്യ result = 0;
	ഘടന write_loose_object_data data;
	ഘടന child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	എങ്കിൽ (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   ശൂന്യ, ശൂന്യ, ശൂന്യ))
		തിരികെ 0;

	pack_proc.git_cmd = 1;

	strvec_തള്ളുക(&pack_proc.args, "pack-objects");
	എങ്കിൽ (opts->quiet)
		strvec_തള്ളുക(&pack_proc.args, "--quiet");
	അല്ലെങ്കിൽ
		strvec_തള്ളുക(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	എങ്കിൽ (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		തിരികെ 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.എണ്ണം = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	എങ്കിൽ (!data.batch_size)
		data.batch_size = INT_MAX;
	അല്ലെങ്കിൽ എങ്കിൽ (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      ശൂന്യ, ശൂന്യ, &data);

	fclose(data.in);

	എങ്കിൽ (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_loose_objects(ഘടന maintenance_run_opts *opts,
					  ഘടന gc_config *cfg UNUSED)
{
	തിരികെ prune_packed(opts) || pack_loose(opts);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ incremental_repack_auto_condition(ഘടന gc_config *cfg UNUSED)
{
	ഘടന packed_git *p;
	പൂർണ്ണസംഖ്യ incremental_repack_auto_limit = 10;
	പൂർണ്ണസംഖ്യ എണ്ണം = 0;

	prepare_repo_settings(the_repository);
	എങ്കിൽ (!the_repository->settings.core_multi_pack_index)
		തിരികെ 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	എങ്കിൽ (!incremental_repack_auto_limit)
		തിരികെ 0;
	എങ്കിൽ (incremental_repack_auto_limit < 0)
		തിരികെ 1;

	repo_for_each_pack(the_repository, p) {
		എങ്കിൽ (എണ്ണം >= incremental_repack_auto_limit)
			നിർത്തുക;
		എങ്കിൽ (!p->multi_pack_index)
			എണ്ണം++;
	}

	തിരികെ എണ്ണം >= incremental_repack_auto_limit;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ multi_pack_index_write(ഘടന maintenance_run_opts *opts)
{
	ഘടന child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", ശൂന്യ);

	എങ്കിൽ (opts->quiet)
		strvec_തള്ളുക(&child.args, "--no-progress");
	അല്ലെങ്കിൽ
		strvec_തള്ളുക(&child.args, "--progress");

	എങ്കിൽ (run_command(&child))
		തിരികെ error(_("failed to write multi-pack-index"));

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ multi_pack_index_expire(ഘടന maintenance_run_opts *opts)
{
	ഘടന child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", ശൂന്യ);

	എങ്കിൽ (opts->quiet)
		strvec_തള്ളുക(&child.args, "--no-progress");
	അല്ലെങ്കിൽ
		strvec_തള്ളുക(&child.args, "--progress");

	എങ്കിൽ (run_command(&child))
		തിരികെ error(_("'git multi-pack-index expire' failed"));

	തിരികെ 0;
}

#define TWO_GIGABYTES (INT32_MAX)

സ്ഥിതം off_t get_auto_pack_size(ശൂന്യം)
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
	ഘടന packed_git *p;
	ഘടന repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		എങ്കിൽ (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} അല്ലെങ്കിൽ എങ്കിൽ (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	എങ്കിൽ (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	തിരികെ result_size;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ multi_pack_index_repack(ഘടന maintenance_run_opts *opts)
{
	ഘടന child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", ശൂന്യ);

	എങ്കിൽ (opts->quiet)
		strvec_തള്ളുക(&child.args, "--no-progress");
	അല്ലെങ്കിൽ
		strvec_തള്ളുക(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	എങ്കിൽ (run_command(&child))
		തിരികെ error(_("'git multi-pack-index repack' failed"));

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_incremental_repack(ഘടന maintenance_run_opts *opts,
					       ഘടന gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	എങ്കിൽ (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		തിരികെ 0;
	}

	എങ്കിൽ (multi_pack_index_write(opts))
		തിരികെ 1;
	എങ്കിൽ (multi_pack_index_expire(opts))
		തിരികെ 1;
	എങ്കിൽ (multi_pack_index_repack(opts))
		തിരികെ 1;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_task_geometric_repack(ഘടന maintenance_run_opts *opts,
					     ഘടന gc_config *cfg)
{
	ഘടന odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	എങ്കിൽ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	തിരികെ odb_optimize(the_repository->objects, &odb_opts);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ geometric_repack_auto_condition(ഘടന gc_config *cfg)
{
	ഘടന odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	തിരികെ odb_optimize_required(the_repository->objects, &opts);
}

തരനിർവചനം പൂർണ്ണസംഖ്യ (*maintenance_task_fn)(ഘടന maintenance_run_opts *opts,
				   ഘടന gc_config *cfg);
തരനിർവചനം പൂർണ്ണസംഖ്യ (*maintenance_auto_fn)(ഘടന gc_config *cfg);

ഘടന maintenance_task {
	സ്ഥിരം അക്ഷരം *name;

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

സ്ഥിതം സ്ഥിരം ഘടന maintenance_task tasks[] = {
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

എണ്ണൽ task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

എണ്ണൽ auto_gc_hook_result {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

സ്ഥിതം പൂർണ്ണസംഖ്യ maybe_run_task(സ്ഥിരം ഘടന maintenance_task *task,
			  ഘടന repository *repo,
			  ഘടന maintenance_run_opts *opts,
			  ഘടന gc_config *cfg,
			  എണ്ണൽ task_phase phase,
			  എണ്ണൽ auto_gc_hook_result *auto_gc_hook_result)
{
	പൂർണ്ണസംഖ്യ foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	സ്ഥിരം അക്ഷരം *region = foreground ? "maintenance foreground" : "maintenance";
	പൂർണ്ണസംഖ്യ ret = 0;

	എങ്കിൽ (!fn)
		തിരികെ 0;
	എങ്കിൽ (opts->auto_flag) {
		എങ്കിൽ (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			തിരികെ 0;

		എങ്കിൽ (!task->auto_condition || !task->auto_condition(cfg))
			തിരികെ 0;

		എങ്കിൽ (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		എങ്കിൽ (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			തിരികെ 0;
	}

	trace2_region_enter(region, task->name, repo);
	എങ്കിൽ (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_run_tasks(ഘടന maintenance_run_opts *opts,
				 ഘടന gc_config *cfg)
{
	പൂർണ്ണസംഖ്യ result = 0;
	ഘടന lock_file lk;
	ഘടന repository *r = the_repository;
	അക്ഷരം *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	എണ്ണൽ auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	എങ്കിൽ (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		എങ്കിൽ (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		സ്വതന്ത്രം(lock_path);
		തിരികെ 0;
	}
	സ്വതന്ത്രം(lock_path);

	വേണ്ടി (size_t i = 0; i < opts->tasks_nr; i++)
		എങ്കിൽ (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	എങ്കിൽ (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	വേണ്ടി (size_t i = 0; i < opts->tasks_nr; i++)
		എങ്കിൽ (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	തിരികെ result;
}

എണ്ണൽ maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

ഘടന maintenance_strategy {
	ഘടന {
		unsigned type;
		എണ്ണൽ schedule_priority schedule;
	} tasks[TASK_എണ്ണം];
};

സ്ഥിതം സ്ഥിരം ഘടന maintenance_strategy none_strategy = { 0 };

സ്ഥിതം സ്ഥിരം ഘടന maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

സ്ഥിതം സ്ഥിരം ഘടന maintenance_strategy incremental_strategy = {
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

സ്ഥിതം സ്ഥിരം ഘടന maintenance_strategy geometric_strategy = {
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

സ്ഥിതം ഘടന maintenance_strategy parse_maintenance_strategy(സ്ഥിരം അക്ഷരം *name)
{
	എങ്കിൽ (!strcasecmp(name, "incremental"))
		തിരികെ incremental_strategy;
	എങ്കിൽ (!strcasecmp(name, "gc"))
		തിരികെ gc_strategy;
	എങ്കിൽ (!strcasecmp(name, "geometric"))
		തിരികെ geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

സ്ഥിതം ശൂന്യം initialize_task_config(ഘടന maintenance_run_opts *opts,
				   സ്ഥിരം ഘടന string_list *selected_tasks)
{
	ഘടന strbuf config_name = STRBUF_INIT;
	ഘടന maintenance_strategy strategy;
	എണ്ണൽ maintenance_type type;
	സ്ഥിരം അക്ഷരം *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	എങ്കിൽ (selected_tasks->nr) {
		വേണ്ടി (size_t i = 0; i < selected_tasks->nr; i++) {
			എണ്ണൽ maintenance_task_label label = (intptr_t)selected_tasks->ഇനങ്ങൾ[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		തിരികെ;
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
	എങ്കിൽ (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} അല്ലെങ്കിൽ {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	എങ്കിൽ (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	വേണ്ടി (size_t i = 0; i < TASK_എണ്ണം; i++) {
		പൂർണ്ണസംഖ്യ config_മൂല്യം;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		എങ്കിൽ (!repo_config_get_bool(the_repository, config_name.buf, &config_മൂല്യം))
			strategy.tasks[i].type = config_മൂല്യം ? type : 0;
		എങ്കിൽ (!(strategy.tasks[i].type & type))
			തുടരുക;

		എങ്കിൽ (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			എങ്കിൽ (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			എങ്കിൽ (strategy.tasks[i].schedule < opts->schedule)
				തുടരുക;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ task_option_parse(സ്ഥിരം ഘടന option *opt,
			     സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന string_list *selected_tasks = opt->മൂല്യം;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	വേണ്ടി (i = 0; i < TASK_എണ്ണം; i++)
		എങ്കിൽ (!strcasecmp(tasks[i].name, arg))
			നിർത്തുക;
	എങ്കിൽ (i >= TASK_എണ്ണം) {
		error(_("'%s' is not a valid task"), arg);
		തിരികെ 1;
	}

	എങ്കിൽ (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		തിരികെ 1;
	}

	string_list_append(selected_tasks, arg)->util = (ശൂന്യം *)(intptr_t)i;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_run(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo UNUSED)
{
	ഘടന maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ഘടന string_list selected_tasks = STRING_LIST_INIT_DUP;
	ഘടന gc_config cfg = GC_CONFIG_INIT;
	ഘടന option builtin_maintenance_run_options[] = {
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
	പൂർണ്ണസംഖ്യ ret;

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

	എങ്കിൽ (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	തിരികെ ret;
}

സ്ഥിതം അക്ഷരം *get_maintpath(ശൂന്യം)
{
	ഘടന strbuf sb = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	തിരികെ strbuf_detach(&sb, ശൂന്യ);
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_register(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				ഘടന repository *repo UNUSED)
{
	അക്ഷരം *config_file = ശൂന്യ;
	ഘടന option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	പൂർണ്ണസംഖ്യ found = 0;
	സ്ഥിരം അക്ഷരം *താക്കോൽ = "maintenance.repo";
	അക്ഷരം *maintpath = get_maintpath();
	ഘടന string_list_item *item;
	സ്ഥിരം ഘടന string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	എങ്കിൽ (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	എങ്കിൽ (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	എങ്കിൽ (!repo_config_get_string_multi(the_repository, താക്കോൽ, &list)) {
		for_each_string_list_item(item, list) {
			എങ്കിൽ (!strcmp(maintpath, item->string)) {
				found = 1;
				നിർത്തുക;
			}
		}
	}

	എങ്കിൽ (!found) {
		പൂർണ്ണസംഖ്യ rc;
		അക്ഷരം *global_config_file = ശൂന്യ;

		എങ്കിൽ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		എങ്കിൽ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, ശൂന്യ, 0);
		സ്വതന്ത്രം(global_config_file);

		എങ്കിൽ (rc)
			die(_("unable to add '%s' value of '%s'"),
			    താക്കോൽ, maintpath);
	}

	സ്വതന്ത്രം(maintpath);
	തിരികെ 0;
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_unregister(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				  ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ force = 0;
	അക്ഷരം *config_file = ശൂന്യ;
	ഘടന option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	സ്ഥിരം അക്ഷരം *താക്കോൽ = "maintenance.repo";
	അക്ഷരം *maintpath = get_maintpath();
	പൂർണ്ണസംഖ്യ found = 0;
	ഘടന string_list_item *item;
	സ്ഥിരം ഘടന string_list *list;
	ഘടന config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	എങ്കിൽ (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	എങ്കിൽ (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	എങ്കിൽ (!(config_file
	      ? git_configset_get_string_multi(&cs, താക്കോൽ, &list)
	      : repo_config_get_string_multi(the_repository, താക്കോൽ, &list))) {
		for_each_string_list_item(item, list) {
			എങ്കിൽ (!strcmp(maintpath, item->string)) {
				found = 1;
				നിർത്തുക;
			}
		}
	}

	എങ്കിൽ (found) {
		പൂർണ്ണസംഖ്യ rc;
		അക്ഷരം *global_config_file = ശൂന്യ;

		എങ്കിൽ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		എങ്കിൽ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, താക്കോൽ, ശൂന്യ, maintpath, ശൂന്യ,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_മൂല്യം);
		സ്വതന്ത്രം(global_config_file);

		എങ്കിൽ (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    താക്കോൽ, maintpath);
	} അല്ലെങ്കിൽ എങ്കിൽ (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	സ്വതന്ത്രം(maintpath);
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_frequency(എണ്ണൽ schedule_priority schedule)
{
	തിരഞ്ഞെടുപ്പ് (schedule) {
	അവസ്ഥ SCHEDULE_HOURLY:
		തിരികെ "hourly";
	അവസ്ഥ SCHEDULE_DAILY:
		തിരികെ "daily";
	അവസ്ഥ SCHEDULE_WEEKLY:
		തിരികെ "weekly";
	സ്ഥിരസ്ഥിതി:
		BUG("invalid schedule %d", schedule);
	}
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_extra_config_parameters(ശൂന്യം) {
	സ്ഥിതം സ്ഥിരം അക്ഷരം *result = ശൂന്യ;
	ഘടന strbuf builder = STRBUF_INIT;

	എങ്കിൽ (result)
		തിരികെ result;

	വേണ്ടി (സ്ഥിരം അക്ഷരം **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, ശൂന്യ);
	തിരികെ result;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_extra_launchctl_strings(ശൂന്യം) {
	സ്ഥിതം സ്ഥിരം അക്ഷരം *result = ശൂന്യ;
	ഘടന strbuf builder = STRBUF_INIT;

	എങ്കിൽ (result)
		തിരികെ result;

	വേണ്ടി (സ്ഥിരം അക്ഷരം **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, ശൂന്യ);
	തിരികെ result;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ get_schedule_cmd(സ്ഥിരം അക്ഷരം *cmd, പൂർണ്ണസംഖ്യ *is_available, അക്ഷരം **out)
{
	അക്ഷരം *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	ഘടന string_list_item *item;
	ഘടന string_list list = STRING_LIST_INIT_NODUP;

	എങ്കിൽ (!testing) {
		എങ്കിൽ (out)
			*out = xstrdup(cmd);
		തിരികെ 0;
	}

	എങ്കിൽ (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		ഘടന string_list pair = STRING_LIST_INIT_NODUP;

		എങ്കിൽ (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			തുടരുക;

		എങ്കിൽ (!strcmp(cmd, pair.ഇനങ്ങൾ[0].string)) {
			എങ്കിൽ (out)
				*out = xstrdup(pair.ഇനങ്ങൾ[1].string);
			എങ്കിൽ (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			പോകുക out;
		}

		string_list_clear(&pair, 0);
	}

	എങ്കിൽ (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	സ്വതന്ത്രം(testing);
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_random_minute(ശൂന്യം)
{
	/* Use a static value when under tests. */
	എങ്കിൽ (getenv("GIT_TEST_MAINT_SCHEDULER"))
		തിരികെ 13;

	തിരികെ git_rand(0) % 60;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_launchctl_available(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ is_available;
	എങ്കിൽ (get_schedule_cmd("launchctl", &is_available, ശൂന്യ))
		തിരികെ is_available;

#ifdef __APPLE__
	തിരികെ 1;
#അല്ലെങ്കിൽ
	തിരികെ 0;
#endif
}

സ്ഥിതം അക്ഷരം *launchctl_service_name(സ്ഥിരം അക്ഷരം *frequency)
{
	ഘടന strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	തിരികെ strbuf_detach(&label, ശൂന്യ);
}

സ്ഥിതം അക്ഷരം *launchctl_service_filename(സ്ഥിരം അക്ഷരം *name)
{
	അക്ഷരം *expanded;
	ഘടന strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	എങ്കിൽ (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	തിരികെ expanded;
}

സ്ഥിതം അക്ഷരം *launchctl_get_uid(ശൂന്യം)
{
	തിരികെ xstrfmt("gui/%d", getuid());
}

സ്ഥിതം പൂർണ്ണസംഖ്യ launchctl_boot_plist(പൂർണ്ണസംഖ്യ enable, സ്ഥിരം അക്ഷരം *filename)
{
	അക്ഷരം *cmd;
	പൂർണ്ണസംഖ്യ result;
	ഘടന child_process child = CHILD_PROCESS_INIT;
	അക്ഷരം *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", ശൂന്യ, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, ശൂന്യ);

	child.no_stderr = 1;
	child.no_stdout = 1;

	എങ്കിൽ (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	സ്വതന്ത്രം(cmd);
	സ്വതന്ത്രം(uid);
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ launchctl_remove_plist(എണ്ണൽ schedule_priority schedule)
{
	സ്ഥിരം അക്ഷരം *frequency = get_frequency(schedule);
	അക്ഷരം *name = launchctl_service_name(frequency);
	അക്ഷരം *filename = launchctl_service_filename(name);
	പൂർണ്ണസംഖ്യ result = launchctl_boot_plist(0, filename);
	unlink(filename);
	സ്വതന്ത്രം(filename);
	സ്വതന്ത്രം(name);
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ launchctl_remove_plists(ശൂന്യം)
{
	തിരികെ launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ launchctl_list_contains_plist(സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *cmd)
{
	ഘടന child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, ശൂന്യ);

	child.no_stderr = 1;
	child.no_stdout = 1;

	എങ്കിൽ (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	തിരികെ !finish_command(&child);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ launchctl_schedule_plist(സ്ഥിരം അക്ഷരം *exec_path, എണ്ണൽ schedule_priority schedule)
{
	പൂർണ്ണസംഖ്യ i, fd;
	സ്ഥിരം അക്ഷരം *preamble, *repeat;
	സ്ഥിരം അക്ഷരം *frequency = get_frequency(schedule);
	അക്ഷരം *name = launchctl_service_name(frequency);
	അക്ഷരം *filename = launchctl_service_filename(name);
	ഘടന lock_file lk = LOCK_INIT;
	സ്ഥിതം unsigned long lock_file_timeout_ms = ULONG_MAX;
	ഘടന strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	ഘടന stat st;
	അക്ഷരം *cmd;
	പൂർണ്ണസംഖ്യ minute = get_random_minute();

	get_schedule_cmd("launchctl", ശൂന്യ, &cmd);
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

	തിരഞ്ഞെടുപ്പ് (schedule) {
	അവസ്ഥ SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		വേണ്ടി (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		നിർത്തുക;

	അവസ്ഥ SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		വേണ്ടി (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		നിർത്തുക;

	അവസ്ഥ SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		നിർത്തുക;

	സ്ഥിരസ്ഥിതി:
		/* unreachable */
		നിർത്തുക;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	എങ്കിൽ (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	എങ്കിൽ ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	എങ്കിൽ (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	അല്ലെങ്കിൽ {
		എങ്കിൽ (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		എങ്കിൽ (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	സ്വതന്ത്രം(filename);
	സ്വതന്ത്രം(name);
	സ്വതന്ത്രം(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ launchctl_add_plists(ശൂന്യം)
{
	സ്ഥിരം അക്ഷരം *exec_path = git_exec_path();

	തിരികെ launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ launchctl_update_schedule(പൂർണ്ണസംഖ്യ run_maintenance, പൂർണ്ണസംഖ്യ fd UNUSED)
{
	എങ്കിൽ (run_maintenance)
		തിരികെ launchctl_add_plists();
	അല്ലെങ്കിൽ
		തിരികെ launchctl_remove_plists();
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_schtasks_available(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ is_available;
	എങ്കിൽ (get_schedule_cmd("schtasks", &is_available, ശൂന്യ))
		തിരികെ is_available;

#ifdef GIT_WINDOWS_NATIVE
	തിരികെ 1;
#അല്ലെങ്കിൽ
	തിരികെ 0;
#endif
}

സ്ഥിതം അക്ഷരം *schtasks_task_name(സ്ഥിരം അക്ഷരം *frequency)
{
	ഘടന strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	തിരികെ strbuf_detach(&label, ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ schtasks_remove_task(എണ്ണൽ schedule_priority schedule)
{
	അക്ഷരം *cmd;
	ഘടന child_process child = CHILD_PROCESS_INIT;
	സ്ഥിരം അക്ഷരം *frequency = get_frequency(schedule);
	അക്ഷരം *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", ശൂന്യ, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", ശൂന്യ);
	സ്വതന്ത്രം(name);
	സ്വതന്ത്രം(cmd);

	തിരികെ run_command(&child);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ schtasks_remove_tasks(ശൂന്യം)
{
	തിരികെ schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ schtasks_schedule_task(സ്ഥിരം അക്ഷരം *exec_path, എണ്ണൽ schedule_priority schedule)
{
	അക്ഷരം *cmd;
	പൂർണ്ണസംഖ്യ result;
	ഘടന child_process child = CHILD_PROCESS_INIT;
	സ്ഥിരം അക്ഷരം *xml;
	ഘടന tempfile *tfile;
	സ്ഥിരം അക്ഷരം *frequency = get_frequency(schedule);
	അക്ഷരം *name = schtasks_task_name(frequency);
	ഘടന strbuf tfilename = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ minute = get_random_minute();

	get_schedule_cmd("schtasks", ശൂന്യ, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	എങ്കിൽ (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	തിരഞ്ഞെടുപ്പ് (schedule) {
	അവസ്ഥ SCHEDULE_HOURLY:
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
		നിർത്തുക;

	അവസ്ഥ SCHEDULE_DAILY:
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
		നിർത്തുക;

	അവസ്ഥ SCHEDULE_WEEKLY:
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
		നിർത്തുക;

	സ്ഥിരസ്ഥിതി:
		നിർത്തുക;
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
				  get_tempfile_path(tfile), ശൂന്യ);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	എങ്കിൽ (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	സ്വതന്ത്രം(name);
	സ്വതന്ത്രം(cmd);
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ schtasks_schedule_tasks(ശൂന്യം)
{
	സ്ഥിരം അക്ഷരം *exec_path = git_exec_path();

	തിരികെ schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ schtasks_update_schedule(പൂർണ്ണസംഖ്യ run_maintenance, പൂർണ്ണസംഖ്യ fd UNUSED)
{
	എങ്കിൽ (run_maintenance)
		തിരികെ schtasks_schedule_tasks();
	അല്ലെങ്കിൽ
		തിരികെ schtasks_remove_tasks();
}

MAYBE_UNUSED
സ്ഥിതം പൂർണ്ണസംഖ്യ check_crontab_process(സ്ഥിരം അക്ഷരം *cmd)
{
	ഘടന child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_തള്ളുക(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	എങ്കിൽ (start_command(&child))
		തിരികെ 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_crontab_available(ശൂന്യം)
{
	അക്ഷരം *cmd;
	പൂർണ്ണസംഖ്യ is_available;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		പോകുക out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#അല്ലെങ്കിൽ
	ret = check_crontab_process(cmd);
#endif

out:
	സ്വതന്ത്രം(cmd);
	തിരികെ ret;
}

#define BEGIN_വരി "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_വരി "# END GIT MAINTENANCE SCHEDULE"

സ്ഥിതം പൂർണ്ണസംഖ്യ crontab_update_schedule(പൂർണ്ണസംഖ്യ run_maintenance, പൂർണ്ണസംഖ്യ fd)
{
	അക്ഷരം *cmd;
	പൂർണ്ണസംഖ്യ result = 0;
	പൂർണ്ണസംഖ്യ in_old_region = 0;
	ഘടന child_process crontab_list = CHILD_PROCESS_INIT;
	ഘടന child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	ഘടന strbuf വരി = STRBUF_INIT;
	ഘടന tempfile *tmpedit = ശൂന്യ;
	പൂർണ്ണസംഖ്യ minute = get_random_minute();

	get_schedule_cmd("crontab", ശൂന്യ, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_തള്ളുക(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	എങ്കിൽ (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		പോകുക out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	എങ്കിൽ (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		പോകുക out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	എങ്കിൽ (!cron_in) {
		result = error(_("failed to open temporary file"));
		പോകുക out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	ആകുമ്പോൾ (!strbuf_getline_lf(&വരി, cron_list)) {
		എങ്കിൽ (!in_old_region && !strcmp(വരി.buf, BEGIN_വരി))
			in_old_region = 1;
		അല്ലെങ്കിൽ എങ്കിൽ (in_old_region && !strcmp(വരി.buf, END_വരി))
			in_old_region = 0;
		അല്ലെങ്കിൽ എങ്കിൽ (!in_old_region)
			fprintf(cron_in, "%s\n", വരി.buf);
	}
	strbuf_release(&വരി);

	എങ്കിൽ (run_maintenance) {
		ഘടന strbuf വരി_format = STRBUF_INIT;
		സ്ഥിരം അക്ഷരം *exec_path = git_exec_path();

		fprintf(cron_in, "%s\n", BEGIN_വരി);
		fprintf(cron_in,
			"# The following schedule was created by Git\n");
		fprintf(cron_in, "# Any edits made in this region might be\n");
		fprintf(cron_in,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&വരി_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_path, exec_path, get_extra_config_parameters());
		fprintf(cron_in, വരി_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_in, വരി_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_in, വരി_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&വരി_format);

		fprintf(cron_in, "\n%s\n", END_വരി);
	}

	fflush(cron_in);

	strvec_split(&crontab_edit.args, cmd);
	strvec_തള്ളുക(&crontab_edit.args, get_tempfile_path(tmpedit));
	crontab_edit.git_cmd = 0;

	എങ്കിൽ (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		പോകുക out;
	}

	എങ്കിൽ (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	അല്ലെങ്കിൽ
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	സ്വതന്ത്രം(cmd);
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ real_is_systemd_timer_available(ശൂന്യം)
{
	ഘടന child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", ശൂന്യ);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	എങ്കിൽ (start_command(&child))
		തിരികെ 0;
	എങ്കിൽ (finish_command(&child))
		തിരികെ 0;
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_systemd_timer_available(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ is_available;

	എങ്കിൽ (get_schedule_cmd("systemctl", &is_available, ശൂന്യ))
		തിരികെ is_available;

	തിരികെ real_is_systemd_timer_available();
}

സ്ഥിതം അക്ഷരം *xdg_config_home_systemd(സ്ഥിരം അക്ഷരം *filename)
{
	തിരികെ xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

സ്ഥിതം പൂർണ്ണസംഖ്യ systemd_timer_delete_timer_file(എണ്ണൽ schedule_priority priority)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	സ്ഥിരം അക്ഷരം *frequency = get_frequency(priority);
	അക്ഷരം *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	അക്ഷരം *filename = xdg_config_home_systemd(local_timer_name);

	എങ്കിൽ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	സ്വതന്ത്രം(filename);
	സ്വതന്ത്രം(local_timer_name);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ systemd_timer_delete_service_template(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	അക്ഷരം *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	അക്ഷരം *filename = xdg_config_home_systemd(local_service_name);
	എങ്കിൽ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	സ്വതന്ത്രം(filename);
	സ്വതന്ത്രം(local_service_name);
	തിരികെ ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ systemd_timer_write_timer_file(എണ്ണൽ schedule_priority schedule,
					  പൂർണ്ണസംഖ്യ minute)
{
	പൂർണ്ണസംഖ്യ res = -1;
	അക്ഷരം *filename;
	FILE *file;
	സ്ഥിരം അക്ഷരം *unit;
	അക്ഷരം *schedule_pattern = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *frequency = get_frequency(schedule);
	അക്ഷരം *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	എങ്കിൽ (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		പോകുക error;
	}
	file = fopen_or_warn(filename, "w");
	എങ്കിൽ (!file)
		പോകുക error;

	തിരഞ്ഞെടുപ്പ് (schedule) {
	അവസ്ഥ SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		നിർത്തുക;

	അവസ്ഥ SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		നിർത്തുക;

	അവസ്ഥ SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		നിർത്തുക;

	സ്ഥിരസ്ഥിതി:
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
	എങ്കിൽ (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		പോകുക error;
	}
	എങ്കിൽ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		പോകുക error;
	}

	res = 0;

error:
	സ്വതന്ത്രം(schedule_pattern);
	സ്വതന്ത്രം(local_timer_name);
	സ്വതന്ത്രം(filename);
	തിരികെ res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ systemd_timer_write_service_template(സ്ഥിരം അക്ഷരം *exec_path)
{
	പൂർണ്ണസംഖ്യ res = -1;
	അക്ഷരം *filename;
	FILE *file;
	സ്ഥിരം അക്ഷരം *unit;
	അക്ഷരം *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	എങ്കിൽ (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		പോകുക error;
	}
	file = fopen_or_warn(filename, "w");
	എങ്കിൽ (!file)
		പോകുക error;

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
	എങ്കിൽ (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		പോകുക error;
	}
	എങ്കിൽ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		പോകുക error;
	}

	res = 0;

error:
	സ്വതന്ത്രം(local_service_name);
	സ്വതന്ത്രം(filename);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ systemd_timer_enable_unit(പൂർണ്ണസംഖ്യ enable,
				     എണ്ണൽ schedule_priority schedule,
				     പൂർണ്ണസംഖ്യ minute)
{
	അക്ഷരം *cmd = ശൂന്യ;
	ഘടന child_process child = CHILD_PROCESS_INIT;
	സ്ഥിരം അക്ഷരം *frequency = get_frequency(schedule);
	പൂർണ്ണസംഖ്യ ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	എങ്കിൽ (!enable) {
		child.no_stderr = 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		പോകുക out;
	}

	get_schedule_cmd("systemctl", ശൂന്യ, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", ശൂന്യ);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	എങ്കിൽ (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		പോകുക out;
	}

	എങ്കിൽ (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		എങ്കിൽ (enable) {
			ret = error(_("failed to run systemctl"));
			പോകുക out;
		}
	}

	ret = 0;

out:
	സ്വതന്ത്രം(cmd);
	തിരികെ ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
സ്ഥിതം ശൂന്യം systemd_timer_delete_stale_timer_templates(ശൂന്യം)
{
	അക്ഷരം *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	അക്ഷരം *filename = xdg_config_home_systemd(timer_template_name);

	എങ്കിൽ (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	സ്വതന്ത്രം(filename);
	സ്വതന്ത്രം(timer_template_name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ systemd_timer_delete_unit_files(ശൂന്യം)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	തിരികെ systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

സ്ഥിതം പൂർണ്ണസംഖ്യ systemd_timer_delete_units(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	തിരികെ systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

സ്ഥിതം പൂർണ്ണസംഖ്യ systemd_timer_setup_units(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ minute = get_random_minute();
	സ്ഥിരം അക്ഷരം *exec_path = git_exec_path();

	പൂർണ്ണസംഖ്യ ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	എങ്കിൽ (ret)
		systemd_timer_delete_units();
	അല്ലെങ്കിൽ
		systemd_timer_delete_stale_timer_templates();

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ systemd_timer_update_schedule(പൂർണ്ണസംഖ്യ run_maintenance, പൂർണ്ണസംഖ്യ fd UNUSED)
{
	എങ്കിൽ (run_maintenance)
		തിരികെ systemd_timer_setup_units();
	അല്ലെങ്കിൽ
		തിരികെ systemd_timer_delete_units();
}

എണ്ണൽ scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

സ്ഥിതം സ്ഥിരം ഘടന {
	സ്ഥിരം അക്ഷരം *name;
	പൂർണ്ണസംഖ്യ (*is_available)(ശൂന്യം);
	പൂർണ്ണസംഖ്യ (*update_schedule)(പൂർണ്ണസംഖ്യ run_maintenance, പൂർണ്ണസംഖ്യ fd);
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

സ്ഥിതം എണ്ണൽ scheduler parse_scheduler(സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	എങ്കിൽ (!മൂല്യം)
		തിരികെ SCHEDULER_INVALID;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(മൂല്യം, "auto"))
		തിരികെ SCHEDULER_AUTO;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(മൂല്യം, "cron") || !strcasecmp(മൂല്യം, "crontab"))
		തിരികെ SCHEDULER_CRON;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(മൂല്യം, "systemd") ||
		 !strcasecmp(മൂല്യം, "systemd-timer"))
		തിരികെ SCHEDULER_SYSTEMD;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(മൂല്യം, "launchctl"))
		തിരികെ SCHEDULER_LAUNCHCTL;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(മൂല്യം, "schtasks"))
		തിരികെ SCHEDULER_SCHTASKS;
	അല്ലെങ്കിൽ
		തിരികെ SCHEDULER_INVALID;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_opt_scheduler(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
				     പൂർണ്ണസംഖ്യ unset)
{
	എണ്ണൽ scheduler *scheduler = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	എങ്കിൽ (*scheduler == SCHEDULER_INVALID)
		തിരികെ error(_("unrecognized --scheduler argument '%s'"), arg);
	തിരികെ 0;
}

ഘടന maintenance_start_opts {
	എണ്ണൽ scheduler scheduler;
};

സ്ഥിതം എണ്ണൽ scheduler resolve_scheduler(എണ്ണൽ scheduler scheduler)
{
	എങ്കിൽ (scheduler != SCHEDULER_AUTO)
		തിരികെ scheduler;

#എങ്കിൽ defined(__APPLE__)
	തിരികെ SCHEDULER_LAUNCHCTL;

#അല്ലെങ്കിലെങ്കിൽ defined(GIT_WINDOWS_NATIVE)
	തിരികെ SCHEDULER_SCHTASKS;

#അല്ലെങ്കിലെങ്കിൽ defined(__linux__)
	എങ്കിൽ (is_systemd_timer_available())
		തിരികെ SCHEDULER_SYSTEMD;
	അല്ലെങ്കിൽ എങ്കിൽ (is_crontab_available())
		തിരികെ SCHEDULER_CRON;
	അല്ലെങ്കിൽ
		die(_("neither systemd timers nor crontab are available"));

#അല്ലെങ്കിൽ
	തിരികെ SCHEDULER_CRON;
#endif
}

സ്ഥിതം ശൂന്യം validate_scheduler(എണ്ണൽ scheduler scheduler)
{
	എങ്കിൽ (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	എങ്കിൽ (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	എങ്കിൽ (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_background_schedule(സ്ഥിരം ഘടന maintenance_start_opts *opts,
				      പൂർണ്ണസംഖ്യ enable)
{
	unsigned പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ result = 0;
	ഘടന lock_file lk;
	അക്ഷരം *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	എങ്കിൽ (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		എങ്കിൽ (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		അല്ലെങ്കിൽ
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		സ്വതന്ത്രം(lock_path);
		തിരികെ -1;
	}

	വേണ്ടി (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		എങ്കിൽ (enable && opts->scheduler == i)
			തുടരുക;
		എങ്കിൽ (!scheduler_fn[i].is_available())
			തുടരുക;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	എങ്കിൽ (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	സ്വതന്ത്രം(lock_path);
	തിരികെ result;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_start(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			     ഘടന repository *repo)
{
	ഘടന maintenance_start_opts opts = { 0 };
	ഘടന option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *register_args[] = { "register", ശൂന്യ };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	എങ്കിൽ (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	എങ്കിൽ (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	എങ്കിൽ (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, ശൂന്യ, repo))
		warning(_("failed to add repo to global config"));
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_stop(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			    ഘടന repository *repo UNUSED)
{
	ഘടന option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	എങ്കിൽ (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	തിരികെ update_background_schedule(ശൂന്യ, 0);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ maintenance_is_needed(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				 ഘടന repository *repo UNUSED)
{
	ഘടന maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ഘടന string_list selected_tasks = STRING_LIST_INIT_DUP;
	ഘടന gc_config cfg = GC_CONFIG_INIT;
	ഘടന option options[] = {
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
	എങ്കിൽ (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	എങ്കിൽ (opts.auto_flag) {
		വേണ്ടി (size_t i = 0; i < opts.tasks_nr; i++) {
			എങ്കിൽ (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				നിർത്തുക;
			}
		}
	} അല്ലെങ്കിൽ {
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

	എങ്കിൽ (is_needed)
		തിരികെ 0;
	തിരികെ 1;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	ശൂന്യ,
};

പൂർണ്ണസംഖ്യ cmd_maintenance(പൂർണ്ണസംഖ്യ argc,
		    സ്ഥിരം അക്ഷരം **argv,
		    സ്ഥിരം അക്ഷരം *prefix,
		    ഘടന repository *repo)
{
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option builtin_maintenance_options[] = {
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
	തിരികെ fn(argc, argv, prefix, repo);
}
