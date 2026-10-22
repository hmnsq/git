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

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "date.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "tempfile.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "sigchain.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "commit-graph.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "reflog.h"
#ಅನ್ತರ್ಭಾವ "rerere.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "exec-cmd.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"
#ಅನ್ತರ್ಭಾವ "worktree.h"

#define FAILED_RUN "failed to run %s"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ timestamp_t gc_log_expire_time;
ಸ್ಥಿರ ಸಂರಚನಾ tempfile *pidfile;
ಸ್ಥಿರ ಸಂರಚನಾ lock_file log_lock;
ಸ್ಥಿರ ಸಂರಚನಾ string_list pack_garbage = STRING_LIST_INIT_DUP;

ಸ್ಥಿರ ಶೂನ್ಯ clean_pack_garbage(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪ್ರತಿ (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.ವಸ್ತುಗಳು[i].string);
	string_list_clear(&pack_garbage, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ report_pack_garbage(unsigned seen_bits, ಧ್ರುವ ವರ್ಣ *path)
{
	ಯದಿ (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

ಸ್ಥಿರ ಶೂನ್ಯ process_log_file(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ stat st;
	ಯದಿ (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} ಅನ್ಯಥಾ ಯದಿ (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} ಅನ್ಯಥಾ {
		ವರ್ಣ *path = repo_git_path(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(path);
		rollback_lock_file(&log_lock);
		ಮುಕ್ತ(path);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ process_log_file_at_exit(ಶೂನ್ಯ)
{
	fflush(stderr);
	process_log_file();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ gc_config_is_timestamp_never(ಧ್ರುವ ವರ್ಣ *var)
{
	ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ;
	timestamp_t expire;

	ಯದಿ (!repo_config_get_ಮೌಲ್ಯ(the_repository, var, &ಮೌಲ್ಯ) && ಮೌಲ್ಯ) {
		ಯದಿ (parse_expiry_date(ಮೌಲ್ಯ, &expire))
			die(_("failed to parse '%s' value '%s'"), var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ expire == 0;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ gc_config {
	ಪೂರ್ಣಾಙ್ಕ pack_refs;
	ಪೂರ್ಣಾಙ್ಕ prune_reflogs;
	ಪೂರ್ಣಾಙ್ಕ cruft_packs;
	unsigned long max_cruft_size;
	ಪೂರ್ಣಾಙ್ಕ aggressive_depth;
	ಪೂರ್ಣಾಙ್ಕ aggressive_window;
	ಪೂರ್ಣಾಙ್ಕ detach_auto;
	ವರ್ಣ *gc_log_expire;
	ವರ್ಣ *prune_expire;
	ವರ್ಣ *prune_worktrees_expire;
	ವರ್ಣ *repack_expire_to;
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

ಸ್ಥಿರ ಶೂನ್ಯ gc_config_release(ಸಂರಚನಾ gc_config *cfg)
{
	ಮುಕ್ತ(cfg->gc_log_expire);
	ಮುಕ್ತ(cfg->prune_expire);
	ಮುಕ್ತ(cfg->prune_worktrees_expire);
}

ಸ್ಥಿರ ಶೂನ್ಯ gc_config(ಸಂರಚನಾ gc_config *cfg)
{
	ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ;
	ವರ್ಣ *owned = ರಿಕ್ತ;

	ಯದಿ (!repo_config_get_ಮೌಲ್ಯ(the_repository, "gc.packrefs", &ಮೌಲ್ಯ)) {
		ಯದಿ (ಮೌಲ್ಯ && !strcmp(ಮೌಲ್ಯ, "notbare"))
			cfg->pack_refs = -1;
		ಅನ್ಯಥಾ
			cfg->pack_refs = git_config_bool("gc.packrefs", ಮೌಲ್ಯ);
	}

	ಯದಿ (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	ಯದಿ (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		ಮುಕ್ತ(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	ಯದಿ (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		ಮುಕ್ತ(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	ಯದಿ (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		ಮುಕ್ತ(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
}

ಗಣನಾ schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

ಸ್ಥಿರ ಗಣನಾ schedule_priority parse_schedule(ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಯದಿ (!ಮೌಲ್ಯ)
		ಪ್ರತ್ಯಾಗಮ SCHEDULE_NONE;
	ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "hourly"))
		ಪ್ರತ್ಯಾಗಮ SCHEDULE_HOURLY;
	ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "daily"))
		ಪ್ರತ್ಯಾಗಮ SCHEDULE_DAILY;
	ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "weekly"))
		ಪ್ರತ್ಯಾಗಮ SCHEDULE_WEEKLY;
	ಪ್ರತ್ಯಾಗಮ SCHEDULE_NONE;
}

ಗಣನಾ maintenance_task_label {
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
	TASK_ಎಣಿಕೆ
};

ಸಂರಚನಾ maintenance_run_opts {
	ಗಣನಾ maintenance_task_label *tasks;
	size_t tasks_nr, tasks_alloc;
	ಪೂರ್ಣಾಙ್ಕ auto_flag;
	ಪೂರ್ಣಾಙ್ಕ detach;
	ಪೂರ್ಣಾಙ್ಕ quiet;
	ಗಣನಾ schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

ಸ್ಥಿರ ಶೂನ್ಯ maintenance_run_opts_release(ಸಂರಚನಾ maintenance_run_opts *opts)
{
	ಮುಕ್ತ(opts->tasks);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pack_refs_condition(UNUSED ಸಂರಚನಾ gc_config *cfg)
{
	ಸಂರಚನಾ string_list included_refs = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	ಸಂರಚನಾ refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	ಯದಿ (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		ಪ್ರತ್ಯಾಗಮ 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	ಪ್ರತ್ಯಾಗಮ required;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_pack_refs(ಸಂರಚನಾ maintenance_run_opts *opts,
				      UNUSED ಸಂರಚನಾ gc_config *cfg)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", ರಿಕ್ತ);
	ಯದಿ (opts->auto_flag)
		strvec_ತಳ್ಳು(&cmd.args, "--auto");

	ಪ್ರತ್ಯಾಗಮ run_command(&cmd);
}

ಸಂರಚನಾ ಎಣಿಕೆ_reflog_entries_data {
	ಸಂರಚನಾ expire_reflog_policy_cb policy;
	size_t ಎಣಿಕೆ;
	size_t limit;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_reflog_entries(ಧ್ರುವ ವರ್ಣ *refname UNUSED,
				ಸಂರಚನಾ object_id *old_oid, ಸಂರಚನಾ object_id *new_oid,
				ಧ್ರುವ ವರ್ಣ *committer, timestamp_t timestamp,
				ಪೂರ್ಣಾಙ್ಕ tz, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ ಎಣಿಕೆ_reflog_entries_data *data = cb_data;
	ಯದಿ (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, ಸಂದೇಶ, &data->policy))
		data->ಎಣಿಕೆ++;
	ಪ್ರತ್ಯಾಗಮ data->ಎಣಿಕೆ >= data->limit;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reflog_expire_condition(ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	timestamp_t now = time(ರಿಕ್ತ);
	ಸಂರಚನಾ ಎಣಿಕೆ_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	ಪೂರ್ಣಾಙ್ಕ limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	ಯದಿ (!limit)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (limit < 0)
		ಪ್ರತ್ಯಾಗಮ 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 ಎಣಿಕೆ_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	ಪ್ರತ್ಯಾಗಮ data.ಎಣಿಕೆ >= data.limit;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_reflog_expire(ಸಂರಚನಾ maintenance_run_opts *opts UNUSED,
					  ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", ರಿಕ್ತ);
	ಪ್ರತ್ಯಾಗಮ run_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_worktree_prune(ಸಂರಚನಾ maintenance_run_opts *opts UNUSED,
					   ಸಂರಚನಾ gc_config *cfg)
{
	ಸಂರಚನಾ child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", ರಿಕ್ತ);
	strvec_ತಳ್ಳು(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	ಪ್ರತ್ಯಾಗಮ run_command(&prune_worktrees_cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ worktree_prune_condition(ಸಂರಚನಾ gc_config *cfg)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	ಸಂರಚನಾ dirent *d;
	DIR *dir = ರಿಕ್ತ;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	ಯದಿ (limit <= 0) {
		should_prune = limit < 0;
		ಗಚ್ಛ out;
	}

	ಯದಿ (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		ಗಚ್ಛ out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	ಯದಿ (!dir)
		ಗಚ್ಛ out;

	ಯಾವತ್ (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		ವರ್ಣ *wtpath;
		strbuf_reset(&buf);
		ಯದಿ (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		ಮುಕ್ತ(wtpath);
	}

	should_prune = !limit;

out:
	ಯದಿ (dir)
		closedir(dir);
	strbuf_release(&buf);
	ಪ್ರತ್ಯಾಗಮ should_prune;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_rerere_gc(ಸಂರಚನಾ maintenance_run_opts *opts UNUSED,
				      ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	ಸಂರಚನಾ child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", ರಿಕ್ತ);
	ಪ್ರತ್ಯಾಗಮ run_command(&rerere_cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rerere_gc_condition(ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	ಯದಿ (!limit)
		ಪ್ರತ್ಯಾಗಮ 0; /* never prune */
	ಯದಿ (limit < 0)
		ಪ್ರತ್ಯಾಗಮ 1; /* always prune */

	ಪ್ರತ್ಯಾಗಮ rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *lock_repo_for_gc(ಪೂರ್ಣಾಙ್ಕ force, pid_t* ret_pid)
{
	ಸಂರಚನಾ lock_file lock = LOCK_INIT;
	ವರ್ಣ my_host[HOST_NAME_MAX + 1];
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಸಂರಚನಾ stat st;
	uintmax_t pid;
	FILE *fp;
	ಪೂರ್ಣಾಙ್ಕ fd;
	ವರ್ಣ *pidfile_path;

	ಯದಿ (is_tempfile_active(pidfile))
		/* already locked */
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಯದಿ (xgethostname(my_host, ಪರಿಮಾಣ(my_host)))
		xsnprintf(my_host, ಪರಿಮಾಣ(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	ಯದಿ (!force) {
		ಸ್ಥಿರ ವರ್ಣ locking_host[HOST_NAME_MAX + 1];
		ಸ್ಥಿರ ವರ್ಣ *scan_fmt;
		ಪೂರ್ಣಾಙ್ಕ should_exit;

		ಯದಿ (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_path, "r");
		memset(locking_host, 0, ಪರಿಮಾಣ(locking_host));
		should_exit =
			fp != ರಿಕ್ತ &&
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
			time(ರಿಕ್ತ) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, scan_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		ಯದಿ (fp)
			fclose(fp);
		ಯದಿ (should_exit) {
			ಯದಿ (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			ಮುಕ್ತ(pidfile_path);
			ಪ್ರತ್ಯಾಗಮ locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	ಮುಕ್ತ(pidfile_path);
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ report_last_gc_error(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ssize_t len;
	ಸಂರಚನಾ stat st;
	ವರ್ಣ *gc_log_path = repo_git_path(the_repository, "gc.log");

	ಯದಿ (stat(gc_log_path, &st)) {
		ಯದಿ (errno == ENOENT)
			ಗಚ್ಛ done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		ಗಚ್ಛ done;
	}

	ಯದಿ (st.st_mtime < gc_log_expire_time)
		ಗಚ್ಛ done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	ಯದಿ (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	ಅನ್ಯಥಾ ಯದಿ (len > 0) {
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
	ಮುಕ್ತ(gc_log_path);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ gc_foreground_tasks(ಸಂರಚನಾ maintenance_run_opts *opts,
			       ಸಂರಚನಾ gc_config *cfg)
{
	ಯದಿ (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		ಪ್ರತ್ಯಾಗಮ error(FAILED_RUN, "pack-refs");
	ಯದಿ (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		ಪ್ರತ್ಯಾಗಮ error(FAILED_RUN, "reflog");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_odb(ಸಂರಚನಾ maintenance_run_opts *opts,
				ಸಂರಚನಾ gc_config *cfg,
				ಪೂರ್ಣಾಙ್ಕ keep_largest_pack,
				ಪೂರ್ಣಾಙ್ಕ aggressive)
{
	ಸಂರಚನಾ odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	ಯದಿ (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	ಯದಿ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	ಯದಿ (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	ಪ್ರತ್ಯಾಗಮ odb_optimize(the_repository->objects, &odb_opts);
}

ಪೂರ್ಣಾಙ್ಕ cmd_gc(ಪೂರ್ಣಾಙ್ಕ argc,
	   ಧ್ರುವ ವರ್ಣ **argv,
	   ಧ್ರುವ ವರ್ಣ *prefix,
	   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ aggressive = 0;
	ಪೂರ್ಣಾಙ್ಕ force = 0;
	ಧ್ರುವ ವರ್ಣ *name;
	pid_t pid;
	ಪೂರ್ಣಾಙ್ಕ daemonized = 0;
	ಪೂರ್ಣಾಙ್ಕ keep_largest_pack = -1;
	ಪೂರ್ಣಾಙ್ಕ skip_foreground_tasks = 0;
	timestamp_t dummy;
	ಸಂರಚನಾ maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ಸಂರಚನಾ gc_config cfg = GC_CONFIG_INIT;
	ಧ್ರುವ ವರ್ಣ *prune_expire_sentinel = "sentinel";
	ಧ್ರುವ ವರ್ಣ *prune_expire_arg = prune_expire_sentinel;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.ಮೌಲ್ಯ = &prune_expire_arg,
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

	ಯದಿ (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	ಯದಿ (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	ಯದಿ (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	ಯದಿ (prune_expire_arg != prune_expire_sentinel) {
		ಮುಕ್ತ(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	ಯದಿ (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	ಯದಿ (opts.auto_flag) {
		ಸಂರಚನಾ odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		ಯದಿ (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		ಯದಿ (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			ಗಚ್ಛ out;
		}

		ಯದಿ (!opts.quiet) {
			ಯದಿ (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			ಅನ್ಯಥಾ
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	ಯದಿ (opts.detach > 0) {
		ret = report_last_gc_error();
		ಯದಿ (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			ಗಚ್ಛ out;

		} ಅನ್ಯಥಾ ಯದಿ (ret) {
			/* an I/O error occurred, already reported */
			ಗಚ್ಛ out;
		}

		ಯದಿ (!skip_foreground_tasks) {
			ಯದಿ (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				ಗಚ್ಛ out;
			}

			ಯದಿ (gc_foreground_tasks(&opts, &cfg) < 0)
				die(ರಿಕ್ತ);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = lock_repo_for_gc(force, &pid);
	ಯದಿ (name) {
		ಯದಿ (opts.auto_flag) {
			ret = 0;
			ಗಚ್ಛ out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	ಯದಿ (daemonized) {
		ವರ್ಣ *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		ಮುಕ್ತ(path);
	}

	ಯದಿ (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	ಯದಿ (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	ಯದಿ (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	ಯದಿ (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(ರಿಕ್ತ);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	ಯದಿ (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	ಯದಿ (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     ರಿಕ್ತ);

	ಯದಿ (!daemonized) {
		ವರ್ಣ *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		ಮುಕ್ತ(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_opt_schedule(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
				    ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಗಣನಾ schedule_priority *priority = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	ಯದಿ (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ cg_auto_data {
	ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_not_in_graph;
	ಪೂರ್ಣಾಙ್ಕ limit;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ dfs_on_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ cg_auto_data *data = (ಸಂರಚನಾ cg_auto_data *)cb_data;
	ಪೂರ್ಣಾಙ್ಕ result = 0;
	ಧ್ರುವ ಸಂರಚನಾ object_id *maybe_peeled = ref->oid;
	ಸಂರಚನಾ object_id peeled;
	ಸಂರಚನಾ commit_list *ರಾಶಿ = ರಿಕ್ತ;
	ಸಂರಚನಾ commit *commit;

	ಯದಿ (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	ಯದಿ (odb_read_object_info(the_repository->objects, maybe_peeled, ರಿಕ್ತ) != OBJ_COMMIT)
		ಪ್ರತ್ಯಾಗಮ 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	ಯದಿ (!commit || commit->object.flags & SEEN)
		ಪ್ರತ್ಯಾಗಮ 0;
	commit->object.flags |= SEEN;

	ಯದಿ (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		ಪ್ರತ್ಯಾಗಮ 0;

	data->ಅಂಕೆ_not_in_graph++;

	ಯದಿ (data->ಅಂಕೆ_not_in_graph >= data->limit)
		ಪ್ರತ್ಯಾಗಮ 1;

	commit_list_ಸೇರಿಸು(commit, &ರಾಶಿ);

	ಯಾವತ್ (!result && ರಾಶಿ) {
		ಸಂರಚನಾ commit_list *parent;

		commit = ತೆಗೆ_commit(&ರಾಶಿ);

		ಪ್ರತಿ (parent = commit->parents; parent; parent = parent->next) {
			ಯದಿ (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				ಅನುವರ್ತ;

			parent->item->object.flags |= SEEN;
			data->ಅಂಕೆ_not_in_graph++;

			ಯದಿ (data->ಅಂಕೆ_not_in_graph >= data->limit) {
				result = 1;
				ಭಙ್ಗ;
			}

			commit_list_ಸೇರಿಸು(parent->item, &ರಾಶಿ);
		}
	}

	commit_list_free(ರಾಶಿ);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ should_write_commit_graph(ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ result;
	ಸಂರಚನಾ cg_auto_data data;

	data.ಅಂಕೆ_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	ಯದಿ (!data.limit)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (data.limit < 0)
		ಪ್ರತ್ಯಾಗಮ 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_write_commit_graph(ಸಂರಚನಾ maintenance_run_opts *opts)
{
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", ರಿಕ್ತ);

	ಯದಿ (opts->quiet)
		strvec_ತಳ್ಳು(&child.args, "--no-progress");
	ಅನ್ಯಥಾ
		strvec_ತಳ್ಳು(&child.args, "--progress");

	ಪ್ರತ್ಯಾಗಮ !!run_command(&child);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_commit_graph(ಸಂರಚನಾ maintenance_run_opts *opts,
					 ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	ಯದಿ (!the_repository->settings.core_commit_graph)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fetch_remote(ಸಂರಚನಾ remote *remote, ಶೂನ್ಯ *cbdata)
{
	ಸಂರಚನಾ maintenance_run_opts *opts = cbdata;
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	ಯದಿ (remote->skip_default_update)
		ಪ್ರತ್ಯಾಗಮ 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     ರಿಕ್ತ);

	ಯದಿ (opts->quiet)
		strvec_ತಳ್ಳು(&child.args, "--quiet");

	ಪ್ರತ್ಯಾಗಮ !!run_command(&child);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_prefetch(ಸಂರಚನಾ maintenance_run_opts *opts,
				     ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	ಯದಿ (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_gc_foreground(ಸಂರಚನಾ maintenance_run_opts *opts,
					  ಸಂರಚನಾ gc_config *cfg)
{
	ಪ್ರತ್ಯಾಗಮ gc_foreground_tasks(opts, cfg);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_gc_background(ಸಂರಚನಾ maintenance_run_opts *opts,
					  ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_ತಳ್ಳು(&child.args, "gc");

	ಯದಿ (opts->auto_flag)
		strvec_ತಳ್ಳು(&child.args, "--auto");
	ಯದಿ (opts->quiet)
		strvec_ತಳ್ಳು(&child.args, "--quiet");
	ಅನ್ಯಥಾ
		strvec_ತಳ್ಳು(&child.args, "--no-quiet");
	strvec_ತಳ್ಳು(&child.args, "--no-detach");
	strvec_ತಳ್ಳು(&child.args, "--skip-foreground-tasks");

	ಪ್ರತ್ಯಾಗಮ run_command(&child);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ gc_condition(ಸಂರಚನಾ gc_config *cfg)
{
	ಸಂರಚನಾ odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	ಪ್ರತ್ಯಾಗಮ odb_optimize_required(the_repository->objects, &opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prune_packed(ಸಂರಚನಾ maintenance_run_opts *opts)
{
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_ತಳ್ಳು(&child.args, "prune-packed");

	ಯದಿ (opts->quiet)
		strvec_ತಳ್ಳು(&child.args, "--quiet");

	ಪ್ರತ್ಯಾಗಮ !!run_command(&child);
}

ಸಂರಚನಾ write_loose_object_data {
	FILE *in;
	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ;
	ಪೂರ್ಣಾಙ್ಕ batch_size;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ loose_object_auto_limit = 100;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ loose_object_ಎಣಿಕೆ(ಧ್ರುವ ಸಂರಚನಾ object_id *oid UNUSED,
			      ಧ್ರುವ ವರ್ಣ *path UNUSED,
			      ಶೂನ್ಯ *data)
{
	ಪೂರ್ಣಾಙ್ಕ *ಎಣಿಕೆ = (ಪೂರ್ಣಾಙ್ಕ*)data;
	ಯದಿ (++(*ಎಣಿಕೆ) >= loose_object_auto_limit)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ loose_object_auto_condition(ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	ಯದಿ (!loose_object_auto_limit)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (loose_object_auto_limit < 0)
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತ್ಯಾಗಮ for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_ಎಣಿಕೆ,
					     ರಿಕ್ತ, ರಿಕ್ತ, &ಎಣಿಕೆ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bail_on_loose(ಧ್ರುವ ಸಂರಚನಾ object_id *oid UNUSED,
			 ಧ್ರುವ ವರ್ಣ *path UNUSED,
			 ಶೂನ್ಯ *data UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_loose_object_to_stdin(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				       ಧ್ರುವ ವರ್ಣ *path UNUSED,
				       ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ write_loose_object_data *d = (ಸಂರಚನಾ write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	ಪ್ರತ್ಯಾಗಮ ++(d->ಎಣಿಕೆ) > d->batch_size;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pack_loose(ಸಂರಚನಾ maintenance_run_opts *opts)
{
	ಸಂರಚನಾ repository *r = the_repository;
	ಪೂರ್ಣಾಙ್ಕ result = 0;
	ಸಂರಚನಾ write_loose_object_data data;
	ಸಂರಚನಾ child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	ಯದಿ (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ 0;

	pack_proc.git_cmd = 1;

	strvec_ತಳ್ಳು(&pack_proc.args, "pack-objects");
	ಯದಿ (opts->quiet)
		strvec_ತಳ್ಳು(&pack_proc.args, "--quiet");
	ಅನ್ಯಥಾ
		strvec_ತಳ್ಳು(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	ಯದಿ (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.ಎಣಿಕೆ = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	ಯದಿ (!data.batch_size)
		data.batch_size = INT_MAX;
	ಅನ್ಯಥಾ ಯದಿ (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      ರಿಕ್ತ, ರಿಕ್ತ, &data);

	fclose(data.in);

	ಯದಿ (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_loose_objects(ಸಂರಚನಾ maintenance_run_opts *opts,
					  ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ prune_packed(opts) || pack_loose(opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ incremental_repack_auto_condition(ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	ಸಂರಚನಾ packed_git *p;
	ಪೂರ್ಣಾಙ್ಕ incremental_repack_auto_limit = 10;
	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;

	prepare_repo_settings(the_repository);
	ಯದಿ (!the_repository->settings.core_multi_pack_index)
		ಪ್ರತ್ಯಾಗಮ 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	ಯದಿ (!incremental_repack_auto_limit)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (incremental_repack_auto_limit < 0)
		ಪ್ರತ್ಯಾಗಮ 1;

	repo_for_each_pack(the_repository, p) {
		ಯದಿ (ಎಣಿಕೆ >= incremental_repack_auto_limit)
			ಭಙ್ಗ;
		ಯದಿ (!p->multi_pack_index)
			ಎಣಿಕೆ++;
	}

	ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ >= incremental_repack_auto_limit;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ multi_pack_index_write(ಸಂರಚನಾ maintenance_run_opts *opts)
{
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", ರಿಕ್ತ);

	ಯದಿ (opts->quiet)
		strvec_ತಳ್ಳು(&child.args, "--no-progress");
	ಅನ್ಯಥಾ
		strvec_ತಳ್ಳು(&child.args, "--progress");

	ಯದಿ (run_command(&child))
		ಪ್ರತ್ಯಾಗಮ error(_("failed to write multi-pack-index"));

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ multi_pack_index_expire(ಸಂರಚನಾ maintenance_run_opts *opts)
{
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", ರಿಕ್ತ);

	ಯದಿ (opts->quiet)
		strvec_ತಳ್ಳು(&child.args, "--no-progress");
	ಅನ್ಯಥಾ
		strvec_ತಳ್ಳು(&child.args, "--progress");

	ಯದಿ (run_command(&child))
		ಪ್ರತ್ಯಾಗಮ error(_("'git multi-pack-index expire' failed"));

	ಪ್ರತ್ಯಾಗಮ 0;
}

#define TWO_GIGABYTES (INT32_MAX)

ಸ್ಥಿರ off_t get_auto_pack_size(ಶೂನ್ಯ)
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
	ಸಂರಚನಾ packed_git *p;
	ಸಂರಚನಾ repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		ಯದಿ (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} ಅನ್ಯಥಾ ಯದಿ (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	ಯದಿ (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	ಪ್ರತ್ಯಾಗಮ result_size;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ multi_pack_index_repack(ಸಂರಚನಾ maintenance_run_opts *opts)
{
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", ರಿಕ್ತ);

	ಯದಿ (opts->quiet)
		strvec_ತಳ್ಳು(&child.args, "--no-progress");
	ಅನ್ಯಥಾ
		strvec_ತಳ್ಳು(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	ಯದಿ (run_command(&child))
		ಪ್ರತ್ಯಾಗಮ error(_("'git multi-pack-index repack' failed"));

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_incremental_repack(ಸಂರಚನಾ maintenance_run_opts *opts,
					       ಸಂರಚನಾ gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	ಯದಿ (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (multi_pack_index_write(opts))
		ಪ್ರತ್ಯಾಗಮ 1;
	ಯದಿ (multi_pack_index_expire(opts))
		ಪ್ರತ್ಯಾಗಮ 1;
	ಯದಿ (multi_pack_index_repack(opts))
		ಪ್ರತ್ಯಾಗಮ 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_task_geometric_repack(ಸಂರಚನಾ maintenance_run_opts *opts,
					     ಸಂರಚನಾ gc_config *cfg)
{
	ಸಂರಚನಾ odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	ಯದಿ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	ಪ್ರತ್ಯಾಗಮ odb_optimize(the_repository->objects, &odb_opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ geometric_repack_auto_condition(ಸಂರಚನಾ gc_config *cfg)
{
	ಸಂರಚನಾ odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	ಪ್ರತ್ಯಾಗಮ odb_optimize_required(the_repository->objects, &opts);
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*maintenance_task_fn)(ಸಂರಚನಾ maintenance_run_opts *opts,
				   ಸಂರಚನಾ gc_config *cfg);
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*maintenance_auto_fn)(ಸಂರಚನಾ gc_config *cfg);

ಸಂರಚನಾ maintenance_task {
	ಧ್ರುವ ವರ್ಣ *name;

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

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ maintenance_task tasks[] = {
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

ಗಣನಾ task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

ಗಣನಾ auto_gc_hook_result {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maybe_run_task(ಧ್ರುವ ಸಂರಚನಾ maintenance_task *task,
			  ಸಂರಚನಾ repository *repo,
			  ಸಂರಚನಾ maintenance_run_opts *opts,
			  ಸಂರಚನಾ gc_config *cfg,
			  ಗಣನಾ task_phase phase,
			  ಗಣನಾ auto_gc_hook_result *auto_gc_hook_result)
{
	ಪೂರ್ಣಾಙ್ಕ foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ಧ್ರುವ ವರ್ಣ *region = foreground ? "maintenance foreground" : "maintenance";
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಯದಿ (!fn)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (opts->auto_flag) {
		ಯದಿ (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			ಪ್ರತ್ಯಾಗಮ 0;

		ಯದಿ (!task->auto_condition || !task->auto_condition(cfg))
			ಪ್ರತ್ಯಾಗಮ 0;

		ಯದಿ (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		ಯದಿ (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			ಪ್ರತ್ಯಾಗಮ 0;
	}

	trace2_region_enter(region, task->name, repo);
	ಯದಿ (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_run_tasks(ಸಂರಚನಾ maintenance_run_opts *opts,
				 ಸಂರಚನಾ gc_config *cfg)
{
	ಪೂರ್ಣಾಙ್ಕ result = 0;
	ಸಂರಚನಾ lock_file lk;
	ಸಂರಚನಾ repository *r = the_repository;
	ವರ್ಣ *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	ಗಣನಾ auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	ಯದಿ (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		ಯದಿ (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		ಮುಕ್ತ(lock_path);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಮುಕ್ತ(lock_path);

	ಪ್ರತಿ (size_t i = 0; i < opts->tasks_nr; i++)
		ಯದಿ (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	ಯದಿ (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	ಪ್ರತಿ (size_t i = 0; i < opts->tasks_nr; i++)
		ಯದಿ (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಗಣನಾ maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

ಸಂರಚನಾ maintenance_strategy {
	ಸಂರಚನಾ {
		unsigned type;
		ಗಣನಾ schedule_priority schedule;
	} tasks[TASK_ಎಣಿಕೆ];
};

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ maintenance_strategy none_strategy = { 0 };

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ maintenance_strategy incremental_strategy = {
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

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ maintenance_strategy geometric_strategy = {
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

ಸ್ಥಿರ ಸಂರಚನಾ maintenance_strategy parse_maintenance_strategy(ಧ್ರುವ ವರ್ಣ *name)
{
	ಯದಿ (!strcasecmp(name, "incremental"))
		ಪ್ರತ್ಯಾಗಮ incremental_strategy;
	ಯದಿ (!strcasecmp(name, "gc"))
		ಪ್ರತ್ಯಾಗಮ gc_strategy;
	ಯದಿ (!strcasecmp(name, "geometric"))
		ಪ್ರತ್ಯಾಗಮ geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

ಸ್ಥಿರ ಶೂನ್ಯ initialize_task_config(ಸಂರಚನಾ maintenance_run_opts *opts,
				   ಧ್ರುವ ಸಂರಚನಾ string_list *selected_tasks)
{
	ಸಂರಚನಾ strbuf config_name = STRBUF_INIT;
	ಸಂರಚನಾ maintenance_strategy strategy;
	ಗಣನಾ maintenance_type type;
	ಧ್ರುವ ವರ್ಣ *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	ಯದಿ (selected_tasks->nr) {
		ಪ್ರತಿ (size_t i = 0; i < selected_tasks->nr; i++) {
			ಗಣನಾ maintenance_task_label label = (intptr_t)selected_tasks->ವಸ್ತುಗಳು[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		ಪ್ರತ್ಯಾಗಮ;
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
	ಯದಿ (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} ಅನ್ಯಥಾ {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	ಯದಿ (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	ಪ್ರತಿ (size_t i = 0; i < TASK_ಎಣಿಕೆ; i++) {
		ಪೂರ್ಣಾಙ್ಕ config_ಮೌಲ್ಯ;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		ಯದಿ (!repo_config_get_bool(the_repository, config_name.buf, &config_ಮೌಲ್ಯ))
			strategy.tasks[i].type = config_ಮೌಲ್ಯ ? type : 0;
		ಯದಿ (!(strategy.tasks[i].type & type))
			ಅನುವರ್ತ;

		ಯದಿ (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			ಯದಿ (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			ಯದಿ (strategy.tasks[i].schedule < opts->schedule)
				ಅನುವರ್ತ;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ task_option_parse(ಧ್ರುವ ಸಂರಚನಾ option *opt,
			     ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ string_list *selected_tasks = opt->ಮೌಲ್ಯ;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	ಪ್ರತಿ (i = 0; i < TASK_ಎಣಿಕೆ; i++)
		ಯದಿ (!strcasecmp(tasks[i].name, arg))
			ಭಙ್ಗ;
	ಯದಿ (i >= TASK_ಎಣಿಕೆ) {
		error(_("'%s' is not a valid task"), arg);
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಯದಿ (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	string_list_append(selected_tasks, arg)->util = (ಶೂನ್ಯ *)(intptr_t)i;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_run(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ಸಂರಚನಾ string_list selected_tasks = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ gc_config cfg = GC_CONFIG_INIT;
	ಸಂರಚನಾ option builtin_maintenance_run_options[] = {
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
	ಪೂರ್ಣಾಙ್ಕ ret;

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

	ಯದಿ (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ವರ್ಣ *get_maintpath(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&sb, ರಿಕ್ತ);
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_register(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				ಸಂರಚನಾ repository *repo UNUSED)
{
	ವರ್ಣ *config_file = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	ಪೂರ್ಣಾಙ್ಕ found = 0;
	ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ = "maintenance.repo";
	ವರ್ಣ *maintpath = get_maintpath();
	ಸಂರಚನಾ string_list_item *item;
	ಧ್ರುವ ಸಂರಚನಾ string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	ಯದಿ (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	ಯದಿ (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	ಯದಿ (!repo_config_get_string_multi(the_repository, ಕೀಲಿ, &list)) {
		for_each_string_list_item(item, list) {
			ಯದಿ (!strcmp(maintpath, item->string)) {
				found = 1;
				ಭಙ್ಗ;
			}
		}
	}

	ಯದಿ (!found) {
		ಪೂರ್ಣಾಙ್ಕ rc;
		ವರ್ಣ *global_config_file = ರಿಕ್ತ;

		ಯದಿ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ಯದಿ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, ರಿಕ್ತ, 0);
		ಮುಕ್ತ(global_config_file);

		ಯದಿ (rc)
			die(_("unable to add '%s' value of '%s'"),
			    ಕೀಲಿ, maintpath);
	}

	ಮುಕ್ತ(maintpath);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_unregister(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ force = 0;
	ವರ್ಣ *config_file = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ = "maintenance.repo";
	ವರ್ಣ *maintpath = get_maintpath();
	ಪೂರ್ಣಾಙ್ಕ found = 0;
	ಸಂರಚನಾ string_list_item *item;
	ಧ್ರುವ ಸಂರಚನಾ string_list *list;
	ಸಂರಚನಾ config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	ಯದಿ (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	ಯದಿ (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	ಯದಿ (!(config_file
	      ? git_configset_get_string_multi(&cs, ಕೀಲಿ, &list)
	      : repo_config_get_string_multi(the_repository, ಕೀಲಿ, &list))) {
		for_each_string_list_item(item, list) {
			ಯದಿ (!strcmp(maintpath, item->string)) {
				found = 1;
				ಭಙ್ಗ;
			}
		}
	}

	ಯದಿ (found) {
		ಪೂರ್ಣಾಙ್ಕ rc;
		ವರ್ಣ *global_config_file = ರಿಕ್ತ;

		ಯದಿ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ಯದಿ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, ಕೀಲಿ, ರಿಕ್ತ, maintpath, ರಿಕ್ತ,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ);
		ಮುಕ್ತ(global_config_file);

		ಯದಿ (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    ಕೀಲಿ, maintpath);
	} ಅನ್ಯಥಾ ಯದಿ (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	ಮುಕ್ತ(maintpath);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_frequency(ಗಣನಾ schedule_priority schedule)
{
	ವಿಕಲ್ಪ (schedule) {
	ಪಕ್ಷ SCHEDULE_HOURLY:
		ಪ್ರತ್ಯಾಗಮ "hourly";
	ಪಕ್ಷ SCHEDULE_DAILY:
		ಪ್ರತ್ಯಾಗಮ "daily";
	ಪಕ್ಷ SCHEDULE_WEEKLY:
		ಪ್ರತ್ಯಾಗಮ "weekly";
	ಸ್ವಭಾವ:
		BUG("invalid schedule %d", schedule);
	}
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_extra_config_parameters(ಶೂನ್ಯ) {
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *result = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf builder = STRBUF_INIT;

	ಯದಿ (result)
		ಪ್ರತ್ಯಾಗಮ result;

	ಪ್ರತಿ (ಧ್ರುವ ವರ್ಣ **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, ರಿಕ್ತ);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_extra_launchctl_strings(ಶೂನ್ಯ) {
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *result = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf builder = STRBUF_INIT;

	ಯದಿ (result)
		ಪ್ರತ್ಯಾಗಮ result;

	ಪ್ರತಿ (ಧ್ರುವ ವರ್ಣ **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, ರಿಕ್ತ);
	ಪ್ರತ್ಯಾಗಮ result;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_schedule_cmd(ಧ್ರುವ ವರ್ಣ *cmd, ಪೂರ್ಣಾಙ್ಕ *is_available, ವರ್ಣ **out)
{
	ವರ್ಣ *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ string_list list = STRING_LIST_INIT_NODUP;

	ಯದಿ (!testing) {
		ಯದಿ (out)
			*out = xstrdup(cmd);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		ಸಂರಚನಾ string_list pair = STRING_LIST_INIT_NODUP;

		ಯದಿ (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			ಅನುವರ್ತ;

		ಯದಿ (!strcmp(cmd, pair.ವಸ್ತುಗಳು[0].string)) {
			ಯದಿ (out)
				*out = xstrdup(pair.ವಸ್ತುಗಳು[1].string);
			ಯದಿ (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			ಗಚ್ಛ out;
		}

		string_list_clear(&pair, 0);
	}

	ಯದಿ (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	ಮುಕ್ತ(testing);
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_random_minute(ಶೂನ್ಯ)
{
	/* Use a static value when under tests. */
	ಯದಿ (getenv("GIT_TEST_MAINT_SCHEDULER"))
		ಪ್ರತ್ಯಾಗಮ 13;

	ಪ್ರತ್ಯಾಗಮ git_rand(0) % 60;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_launchctl_available(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ is_available;
	ಯದಿ (get_schedule_cmd("launchctl", &is_available, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ is_available;

#ifdef __APPLE__
	ಪ್ರತ್ಯಾಗಮ 1;
#ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

ಸ್ಥಿರ ವರ್ಣ *launchctl_service_name(ಧ್ರುವ ವರ್ಣ *frequency)
{
	ಸಂರಚನಾ strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&label, ರಿಕ್ತ);
}

ಸ್ಥಿರ ವರ್ಣ *launchctl_service_filename(ಧ್ರುವ ವರ್ಣ *name)
{
	ವರ್ಣ *expanded;
	ಸಂರಚನಾ strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	ಯದಿ (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	ಪ್ರತ್ಯಾಗಮ expanded;
}

ಸ್ಥಿರ ವರ್ಣ *launchctl_get_uid(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ xstrfmt("gui/%d", getuid());
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ launchctl_boot_plist(ಪೂರ್ಣಾಙ್ಕ enable, ಧ್ರುವ ವರ್ಣ *filename)
{
	ವರ್ಣ *cmd;
	ಪೂರ್ಣಾಙ್ಕ result;
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;
	ವರ್ಣ *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", ರಿಕ್ತ, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, ರಿಕ್ತ);

	child.no_stderr = 1;
	child.no_stdout = 1;

	ಯದಿ (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	ಮುಕ್ತ(cmd);
	ಮುಕ್ತ(uid);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ launchctl_remove_plist(ಗಣನಾ schedule_priority schedule)
{
	ಧ್ರುವ ವರ್ಣ *frequency = get_frequency(schedule);
	ವರ್ಣ *name = launchctl_service_name(frequency);
	ವರ್ಣ *filename = launchctl_service_filename(name);
	ಪೂರ್ಣಾಙ್ಕ result = launchctl_boot_plist(0, filename);
	unlink(filename);
	ಮುಕ್ತ(filename);
	ಮುಕ್ತ(name);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ launchctl_remove_plists(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ launchctl_list_contains_plist(ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *cmd)
{
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, ರಿಕ್ತ);

	child.no_stderr = 1;
	child.no_stdout = 1;

	ಯದಿ (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	ಪ್ರತ್ಯಾಗಮ !finish_command(&child);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ launchctl_schedule_plist(ಧ್ರುವ ವರ್ಣ *exec_path, ಗಣನಾ schedule_priority schedule)
{
	ಪೂರ್ಣಾಙ್ಕ i, fd;
	ಧ್ರುವ ವರ್ಣ *preamble, *repeat;
	ಧ್ರುವ ವರ್ಣ *frequency = get_frequency(schedule);
	ವರ್ಣ *name = launchctl_service_name(frequency);
	ವರ್ಣ *filename = launchctl_service_filename(name);
	ಸಂರಚನಾ lock_file lk = LOCK_INIT;
	ಸ್ಥಿರ unsigned long lock_file_timeout_ms = ULONG_MAX;
	ಸಂರಚನಾ strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	ಸಂರಚನಾ stat st;
	ವರ್ಣ *cmd;
	ಪೂರ್ಣಾಙ್ಕ minute = get_random_minute();

	get_schedule_cmd("launchctl", ರಿಕ್ತ, &cmd);
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

	ವಿಕಲ್ಪ (schedule) {
	ಪಕ್ಷ SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		ಪ್ರತಿ (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ಭಙ್ಗ;

	ಪಕ್ಷ SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		ಪ್ರತಿ (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ಭಙ್ಗ;

	ಪಕ್ಷ SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		ಭಙ್ಗ;

	ಸ್ವಭಾವ:
		/* unreachable */
		ಭಙ್ಗ;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	ಯದಿ (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	ಯದಿ ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	ಯದಿ (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	ಅನ್ಯಥಾ {
		ಯದಿ (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		ಯದಿ (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	ಮುಕ್ತ(filename);
	ಮುಕ್ತ(name);
	ಮುಕ್ತ(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ launchctl_add_plists(ಶೂನ್ಯ)
{
	ಧ್ರುವ ವರ್ಣ *exec_path = git_exec_path();

	ಪ್ರತ್ಯಾಗಮ launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ launchctl_update_schedule(ಪೂರ್ಣಾಙ್ಕ run_maintenance, ಪೂರ್ಣಾಙ್ಕ fd UNUSED)
{
	ಯದಿ (run_maintenance)
		ಪ್ರತ್ಯಾಗಮ launchctl_add_plists();
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ launchctl_remove_plists();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_schtasks_available(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ is_available;
	ಯದಿ (get_schedule_cmd("schtasks", &is_available, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ is_available;

#ifdef GIT_WINDOWS_NATIVE
	ಪ್ರತ್ಯಾಗಮ 1;
#ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

ಸ್ಥಿರ ವರ್ಣ *schtasks_task_name(ಧ್ರುವ ವರ್ಣ *frequency)
{
	ಸಂರಚನಾ strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&label, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ schtasks_remove_task(ಗಣನಾ schedule_priority schedule)
{
	ವರ್ಣ *cmd;
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;
	ಧ್ರುವ ವರ್ಣ *frequency = get_frequency(schedule);
	ವರ್ಣ *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", ರಿಕ್ತ, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", ರಿಕ್ತ);
	ಮುಕ್ತ(name);
	ಮುಕ್ತ(cmd);

	ಪ್ರತ್ಯಾಗಮ run_command(&child);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ schtasks_remove_tasks(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ schtasks_schedule_task(ಧ್ರುವ ವರ್ಣ *exec_path, ಗಣನಾ schedule_priority schedule)
{
	ವರ್ಣ *cmd;
	ಪೂರ್ಣಾಙ್ಕ result;
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;
	ಧ್ರುವ ವರ್ಣ *xml;
	ಸಂರಚನಾ tempfile *tfile;
	ಧ್ರುವ ವರ್ಣ *frequency = get_frequency(schedule);
	ವರ್ಣ *name = schtasks_task_name(frequency);
	ಸಂರಚನಾ strbuf tfilename = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ minute = get_random_minute();

	get_schedule_cmd("schtasks", ರಿಕ್ತ, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	ಯದಿ (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	ವಿಕಲ್ಪ (schedule) {
	ಪಕ್ಷ SCHEDULE_HOURLY:
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
		ಭಙ್ಗ;

	ಪಕ್ಷ SCHEDULE_DAILY:
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
		ಭಙ್ಗ;

	ಪಕ್ಷ SCHEDULE_WEEKLY:
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
		ಭಙ್ಗ;

	ಸ್ವಭಾವ:
		ಭಙ್ಗ;
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
				  get_tempfile_path(tfile), ರಿಕ್ತ);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	ಯದಿ (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	ಮುಕ್ತ(name);
	ಮುಕ್ತ(cmd);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ schtasks_schedule_tasks(ಶೂನ್ಯ)
{
	ಧ್ರುವ ವರ್ಣ *exec_path = git_exec_path();

	ಪ್ರತ್ಯಾಗಮ schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ schtasks_update_schedule(ಪೂರ್ಣಾಙ್ಕ run_maintenance, ಪೂರ್ಣಾಙ್ಕ fd UNUSED)
{
	ಯದಿ (run_maintenance)
		ಪ್ರತ್ಯಾಗಮ schtasks_schedule_tasks();
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ schtasks_remove_tasks();
}

MAYBE_UNUSED
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_crontab_process(ಧ್ರುವ ವರ್ಣ *cmd)
{
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_ತಳ್ಳು(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	ಯದಿ (start_command(&child))
		ಪ್ರತ್ಯಾಗಮ 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_crontab_available(ಶೂನ್ಯ)
{
	ವರ್ಣ *cmd;
	ಪೂರ್ಣಾಙ್ಕ is_available;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		ಗಚ್ಛ out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#ಅನ್ಯಥಾ
	ret = check_crontab_process(cmd);
#endif

out:
	ಮುಕ್ತ(cmd);
	ಪ್ರತ್ಯಾಗಮ ret;
}

#define BEGIN_ಸಾಲು "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_ಸಾಲು "# END GIT MAINTENANCE SCHEDULE"

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ crontab_update_schedule(ಪೂರ್ಣಾಙ್ಕ run_maintenance, ಪೂರ್ಣಾಙ್ಕ fd)
{
	ವರ್ಣ *cmd;
	ಪೂರ್ಣಾಙ್ಕ result = 0;
	ಪೂರ್ಣಾಙ್ಕ in_old_region = 0;
	ಸಂರಚನಾ child_process crontab_list = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	ಸಂರಚನಾ strbuf ಸಾಲು = STRBUF_INIT;
	ಸಂರಚನಾ tempfile *tmpedit = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ minute = get_random_minute();

	get_schedule_cmd("crontab", ರಿಕ್ತ, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_ತಳ್ಳು(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	ಯದಿ (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		ಗಚ್ಛ out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	ಯದಿ (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		ಗಚ್ಛ out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	ಯದಿ (!cron_in) {
		result = error(_("failed to open temporary file"));
		ಗಚ್ಛ out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	ಯಾವತ್ (!strbuf_getline_lf(&ಸಾಲು, cron_list)) {
		ಯದಿ (!in_old_region && !strcmp(ಸಾಲು.buf, BEGIN_ಸಾಲು))
			in_old_region = 1;
		ಅನ್ಯಥಾ ಯದಿ (in_old_region && !strcmp(ಸಾಲು.buf, END_ಸಾಲು))
			in_old_region = 0;
		ಅನ್ಯಥಾ ಯದಿ (!in_old_region)
			fprintf(cron_in, "%s\n", ಸಾಲು.buf);
	}
	strbuf_release(&ಸಾಲು);

	ಯದಿ (run_maintenance) {
		ಸಂರಚನಾ strbuf ಸಾಲು_format = STRBUF_INIT;
		ಧ್ರುವ ವರ್ಣ *exec_path = git_exec_path();

		fprintf(cron_in, "%s\n", BEGIN_ಸಾಲು);
		fprintf(cron_in,
			"# The following schedule was created by Git\n");
		fprintf(cron_in, "# Any edits made in this region might be\n");
		fprintf(cron_in,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&ಸಾಲು_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_path, exec_path, get_extra_config_parameters());
		fprintf(cron_in, ಸಾಲು_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_in, ಸಾಲು_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_in, ಸಾಲು_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&ಸಾಲು_format);

		fprintf(cron_in, "\n%s\n", END_ಸಾಲು);
	}

	fflush(cron_in);

	strvec_split(&crontab_edit.args, cmd);
	strvec_ತಳ್ಳು(&crontab_edit.args, get_tempfile_path(tmpedit));
	crontab_edit.git_cmd = 0;

	ಯದಿ (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		ಗಚ್ಛ out;
	}

	ಯದಿ (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	ಅನ್ಯಥಾ
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	ಮುಕ್ತ(cmd);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ real_is_systemd_timer_available(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", ರಿಕ್ತ);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	ಯದಿ (start_command(&child))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (finish_command(&child))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_systemd_timer_available(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ is_available;

	ಯದಿ (get_schedule_cmd("systemctl", &is_available, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ is_available;

	ಪ್ರತ್ಯಾಗಮ real_is_systemd_timer_available();
}

ಸ್ಥಿರ ವರ್ಣ *xdg_config_home_systemd(ಧ್ರುವ ವರ್ಣ *filename)
{
	ಪ್ರತ್ಯಾಗಮ xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ systemd_timer_delete_timer_file(ಗಣನಾ schedule_priority priority)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಧ್ರುವ ವರ್ಣ *frequency = get_frequency(priority);
	ವರ್ಣ *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	ವರ್ಣ *filename = xdg_config_home_systemd(local_timer_name);

	ಯದಿ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	ಮುಕ್ತ(filename);
	ಮುಕ್ತ(local_timer_name);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ systemd_timer_delete_service_template(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ವರ್ಣ *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	ವರ್ಣ *filename = xdg_config_home_systemd(local_service_name);
	ಯದಿ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	ಮುಕ್ತ(filename);
	ಮುಕ್ತ(local_service_name);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ systemd_timer_write_timer_file(ಗಣನಾ schedule_priority schedule,
					  ಪೂರ್ಣಾಙ್ಕ minute)
{
	ಪೂರ್ಣಾಙ್ಕ res = -1;
	ವರ್ಣ *filename;
	FILE *file;
	ಧ್ರುವ ವರ್ಣ *unit;
	ವರ್ಣ *schedule_pattern = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *frequency = get_frequency(schedule);
	ವರ್ಣ *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	ಯದಿ (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		ಗಚ್ಛ error;
	}
	file = fopen_or_warn(filename, "w");
	ಯದಿ (!file)
		ಗಚ್ಛ error;

	ವಿಕಲ್ಪ (schedule) {
	ಪಕ್ಷ SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		ಭಙ್ಗ;

	ಪಕ್ಷ SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		ಭಙ್ಗ;

	ಪಕ್ಷ SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		ಭಙ್ಗ;

	ಸ್ವಭಾವ:
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
	ಯದಿ (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		ಗಚ್ಛ error;
	}
	ಯದಿ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		ಗಚ್ಛ error;
	}

	res = 0;

error:
	ಮುಕ್ತ(schedule_pattern);
	ಮುಕ್ತ(local_timer_name);
	ಮುಕ್ತ(filename);
	ಪ್ರತ್ಯಾಗಮ res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ systemd_timer_write_service_template(ಧ್ರುವ ವರ್ಣ *exec_path)
{
	ಪೂರ್ಣಾಙ್ಕ res = -1;
	ವರ್ಣ *filename;
	FILE *file;
	ಧ್ರುವ ವರ್ಣ *unit;
	ವರ್ಣ *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	ಯದಿ (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		ಗಚ್ಛ error;
	}
	file = fopen_or_warn(filename, "w");
	ಯದಿ (!file)
		ಗಚ್ಛ error;

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
	ಯದಿ (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		ಗಚ್ಛ error;
	}
	ಯದಿ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		ಗಚ್ಛ error;
	}

	res = 0;

error:
	ಮುಕ್ತ(local_service_name);
	ಮುಕ್ತ(filename);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ systemd_timer_enable_unit(ಪೂರ್ಣಾಙ್ಕ enable,
				     ಗಣನಾ schedule_priority schedule,
				     ಪೂರ್ಣಾಙ್ಕ minute)
{
	ವರ್ಣ *cmd = ರಿಕ್ತ;
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;
	ಧ್ರುವ ವರ್ಣ *frequency = get_frequency(schedule);
	ಪೂರ್ಣಾಙ್ಕ ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	ಯದಿ (!enable) {
		child.no_stderr = 1;
	} ಅನ್ಯಥಾ ಯದಿ (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		ಗಚ್ಛ out;
	}

	get_schedule_cmd("systemctl", ರಿಕ್ತ, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", ರಿಕ್ತ);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	ಯದಿ (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		ಗಚ್ಛ out;
	}

	ಯದಿ (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		ಯದಿ (enable) {
			ret = error(_("failed to run systemctl"));
			ಗಚ್ಛ out;
		}
	}

	ret = 0;

out:
	ಮುಕ್ತ(cmd);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
ಸ್ಥಿರ ಶೂನ್ಯ systemd_timer_delete_stale_timer_templates(ಶೂನ್ಯ)
{
	ವರ್ಣ *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	ವರ್ಣ *filename = xdg_config_home_systemd(timer_template_name);

	ಯದಿ (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	ಮುಕ್ತ(filename);
	ಮುಕ್ತ(timer_template_name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ systemd_timer_delete_unit_files(ಶೂನ್ಯ)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	ಪ್ರತ್ಯಾಗಮ systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ systemd_timer_delete_units(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	ಪ್ರತ್ಯಾಗಮ systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ systemd_timer_setup_units(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ minute = get_random_minute();
	ಧ್ರುವ ವರ್ಣ *exec_path = git_exec_path();

	ಪೂರ್ಣಾಙ್ಕ ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	ಯದಿ (ret)
		systemd_timer_delete_units();
	ಅನ್ಯಥಾ
		systemd_timer_delete_stale_timer_templates();

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ systemd_timer_update_schedule(ಪೂರ್ಣಾಙ್ಕ run_maintenance, ಪೂರ್ಣಾಙ್ಕ fd UNUSED)
{
	ಯದಿ (run_maintenance)
		ಪ್ರತ್ಯಾಗಮ systemd_timer_setup_units();
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ systemd_timer_delete_units();
}

ಗಣನಾ scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ {
	ಧ್ರುವ ವರ್ಣ *name;
	ಪೂರ್ಣಾಙ್ಕ (*is_available)(ಶೂನ್ಯ);
	ಪೂರ್ಣಾಙ್ಕ (*update_schedule)(ಪೂರ್ಣಾಙ್ಕ run_maintenance, ಪೂರ್ಣಾಙ್ಕ fd);
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

ಸ್ಥಿರ ಗಣನಾ scheduler parse_scheduler(ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಯದಿ (!ಮೌಲ್ಯ)
		ಪ್ರತ್ಯಾಗಮ SCHEDULER_INVALID;
	ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "auto"))
		ಪ್ರತ್ಯಾಗಮ SCHEDULER_AUTO;
	ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "cron") || !strcasecmp(ಮೌಲ್ಯ, "crontab"))
		ಪ್ರತ್ಯಾಗಮ SCHEDULER_CRON;
	ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "systemd") ||
		 !strcasecmp(ಮೌಲ್ಯ, "systemd-timer"))
		ಪ್ರತ್ಯಾಗಮ SCHEDULER_SYSTEMD;
	ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "launchctl"))
		ಪ್ರತ್ಯಾಗಮ SCHEDULER_LAUNCHCTL;
	ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "schtasks"))
		ಪ್ರತ್ಯಾಗಮ SCHEDULER_SCHTASKS;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ SCHEDULER_INVALID;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_opt_scheduler(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
				     ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಗಣನಾ scheduler *scheduler = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	ಯದಿ (*scheduler == SCHEDULER_INVALID)
		ಪ್ರತ್ಯಾಗಮ error(_("unrecognized --scheduler argument '%s'"), arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ maintenance_start_opts {
	ಗಣನಾ scheduler scheduler;
};

ಸ್ಥಿರ ಗಣನಾ scheduler resolve_scheduler(ಗಣನಾ scheduler scheduler)
{
	ಯದಿ (scheduler != SCHEDULER_AUTO)
		ಪ್ರತ್ಯಾಗಮ scheduler;

#ಯದಿ defined(__APPLE__)
	ಪ್ರತ್ಯಾಗಮ SCHEDULER_LAUNCHCTL;

#ನೋಚೇದ್ಯದಿ defined(GIT_WINDOWS_NATIVE)
	ಪ್ರತ್ಯಾಗಮ SCHEDULER_SCHTASKS;

#ನೋಚೇದ್ಯದಿ defined(__linux__)
	ಯದಿ (is_systemd_timer_available())
		ಪ್ರತ್ಯಾಗಮ SCHEDULER_SYSTEMD;
	ಅನ್ಯಥಾ ಯದಿ (is_crontab_available())
		ಪ್ರತ್ಯಾಗಮ SCHEDULER_CRON;
	ಅನ್ಯಥಾ
		die(_("neither systemd timers nor crontab are available"));

#ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ SCHEDULER_CRON;
#endif
}

ಸ್ಥಿರ ಶೂನ್ಯ validate_scheduler(ಗಣನಾ scheduler scheduler)
{
	ಯದಿ (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	ಯದಿ (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	ಯದಿ (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_background_schedule(ಧ್ರುವ ಸಂರಚನಾ maintenance_start_opts *opts,
				      ಪೂರ್ಣಾಙ್ಕ enable)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ result = 0;
	ಸಂರಚನಾ lock_file lk;
	ವರ್ಣ *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	ಯದಿ (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		ಯದಿ (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		ಅನ್ಯಥಾ
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		ಮುಕ್ತ(lock_path);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಪ್ರತಿ (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		ಯದಿ (enable && opts->scheduler == i)
			ಅನುವರ್ತ;
		ಯದಿ (!scheduler_fn[i].is_available())
			ಅನುವರ್ತ;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	ಯದಿ (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	ಮುಕ್ತ(lock_path);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_start(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			     ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ maintenance_start_opts opts = { 0 };
	ಸಂರಚನಾ option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	ಧ್ರುವ ವರ್ಣ *register_args[] = { "register", ರಿಕ್ತ };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	ಯದಿ (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	ಯದಿ (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	ಯದಿ (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, ರಿಕ್ತ, repo))
		warning(_("failed to add repo to global config"));
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_stop(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	ಯದಿ (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	ಪ್ರತ್ಯಾಗಮ update_background_schedule(ರಿಕ್ತ, 0);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maintenance_is_needed(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ಸಂರಚನಾ string_list selected_tasks = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ gc_config cfg = GC_CONFIG_INIT;
	ಸಂರಚನಾ option options[] = {
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
	ಯದಿ (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	ಯದಿ (opts.auto_flag) {
		ಪ್ರತಿ (size_t i = 0; i < opts.tasks_nr; i++) {
			ಯದಿ (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				ಭಙ್ಗ;
			}
		}
	} ಅನ್ಯಥಾ {
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

	ಯದಿ (is_needed)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	ರಿಕ್ತ,
};

ಪೂರ್ಣಾಙ್ಕ cmd_maintenance(ಪೂರ್ಣಾಙ್ಕ argc,
		    ಧ್ರುವ ವರ್ಣ **argv,
		    ಧ್ರುವ ವರ್ಣ *prefix,
		    ಸಂರಚನಾ repository *repo)
{
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option builtin_maintenance_options[] = {
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
	ಪ್ರತ್ಯಾಗಮ fn(argc, argv, prefix, repo);
}
