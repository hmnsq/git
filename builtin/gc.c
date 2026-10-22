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

#include "builtin.h"
#include "abspath.h"
#include "date.h"
#include "dir.h"
#include "environment.h"
#include "hex.h"
#include "config.h"
#include "tempfile.h"
#include "lockfile.h"
#include "parse-options.h"
#include "run-command.h"
#include "sigchain.h"
#include "strvec.h"
#include "commit.h"
#include "commit-graph.h"
#include "packfile.h"
#include "object-file.h"
#include "odb.h"
#include "path.h"
#include "reflog.h"
#include "rerere.h"
#include "revision.h"
#include "refs.h"
#include "remote.h"
#include "exec-cmd.h"
#include "gettext.h"
#include "hook.h"
#include "setup.h"
#include "trace2.h"
#include "worktree.h"

#define FAILED_RUN "failed to run %s"

static const var_na * const builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	NULL
};

static timestamp_t gc_log_expire_time;
static sa_nracanaa tempfile *pidfile;
static sa_nracanaa lock_file log_lock;
static sa_nracanaa string_list pack_garbage = STRING_LIST_INIT_DUP;

static void clean_pack_garbage(void)
{
	puur_naa_nka i;
	for (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.items[i].string);
	string_list_clear(&pack_garbage, 0);
}

static void report_pack_garbage(unsigned seen_bits, const var_na *path)
{
	ᯀᯮᯝ (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

static void process_log_file(void)
{
	sa_nracanaa stat st;
	ᯀᯮᯝ (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		puur_naa_nka saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} ᯀᯮᯝᯂᯪ {
		var_na *path = repo_git_path(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(path);
		rollback_lock_file(&log_lock);
		free(path);
	}
}

static void process_log_file_at_exit(void)
{
	fflush(stderr);
	process_log_file();
}

static puur_naa_nka gc_config_is_timestamp_never(const var_na *var)
{
	const var_na *value;
	timestamp_t expire;

	ᯀᯮᯝ (!repo_config_get_value(the_repository, var, &value) && value) {
		ᯀᯮᯝ (parse_expiry_date(value, &expire))
			die(_("failed to parse '%s' value '%s'"), var, value);
		ᯀᯮᯞ expire == 0;
	}
	ᯀᯮᯞ 0;
}

sa_nracanaa gc_config {
	puur_naa_nka pack_refs;
	puur_naa_nka prune_reflogs;
	puur_naa_nka cruft_packs;
	unsigned long max_cruft_size;
	puur_naa_nka aggressive_depth;
	puur_naa_nka aggressive_window;
	puur_naa_nka detach_auto;
	var_na *gc_log_expire;
	var_na *prune_expire;
	var_na *prune_worktrees_expire;
	var_na *repack_expire_to;
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

static void gc_config_release(sa_nracanaa gc_config *cfg)
{
	free(cfg->gc_log_expire);
	free(cfg->prune_expire);
	free(cfg->prune_worktrees_expire);
}

static void gc_config(sa_nracanaa gc_config *cfg)
{
	const var_na *value;
	var_na *owned = NULL;

	ᯀᯮᯝ (!repo_config_get_value(the_repository, "gc.packrefs", &value)) {
		ᯀᯮᯝ (value && !strcmp(value, "notbare"))
			cfg->pack_refs = -1;
		ᯀᯮᯝᯂᯪ
			cfg->pack_refs = git_config_bool("gc.packrefs", value);
	}

	ᯀᯮᯝ (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	ᯀᯮᯝ (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		free(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	ᯀᯮᯝ (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		free(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	ᯀᯮᯝ (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		free(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, NULL);
}

ga_nanaa schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

static ga_nanaa schedule_priority parse_schedule(const var_na *value)
{
	ᯀᯮᯝ (!value)
		ᯀᯮᯞ SCHEDULE_NONE;
	ᯀᯮᯝ (!strcasecmp(value, "hourly"))
		ᯀᯮᯞ SCHEDULE_HOURLY;
	ᯀᯮᯝ (!strcasecmp(value, "daily"))
		ᯀᯮᯞ SCHEDULE_DAILY;
	ᯀᯮᯝ (!strcasecmp(value, "weekly"))
		ᯀᯮᯞ SCHEDULE_WEEKLY;
	ᯀᯮᯞ SCHEDULE_NONE;
}

ga_nanaa maintenance_task_label {
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

sa_nracanaa maintenance_run_opts {
	ga_nanaa maintenance_task_label *tasks;
	size_t tasks_nr, tasks_alloc;
	puur_naa_nka auto_flag;
	puur_naa_nka detach;
	puur_naa_nka quiet;
	ga_nanaa schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

static void maintenance_run_opts_release(sa_nracanaa maintenance_run_opts *opts)
{
	free(opts->tasks);
}

static puur_naa_nka pack_refs_condition(UNUSED sa_nracanaa gc_config *cfg)
{
	sa_nracanaa string_list included_refs = STRING_LIST_INIT_NODUP;
	sa_nracanaa ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	sa_nracanaa refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	ᯀᯮᯝ (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		ᯀᯮᯞ 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	ᯀᯮᯞ required;
}

static puur_naa_nka maintenance_task_pack_refs(sa_nracanaa maintenance_run_opts *opts,
				      UNUSED sa_nracanaa gc_config *cfg)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", NULL);
	ᯀᯮᯝ (opts->auto_flag)
		strvec_push(&cmd.args, "--auto");

	ᯀᯮᯞ run_command(&cmd);
}

sa_nracanaa count_reflog_entries_data {
	sa_nracanaa expire_reflog_policy_cb policy;
	size_t count;
	size_t limit;
};

static puur_naa_nka count_reflog_entries(const var_na *refname UNUSED,
				sa_nracanaa object_id *old_oid, sa_nracanaa object_id *new_oid,
				const var_na *committer, timestamp_t timestamp,
				puur_naa_nka tz, const var_na *msg, void *cb_data)
{
	sa_nracanaa count_reflog_entries_data *data = cb_data;
	ᯀᯮᯝ (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, msg, &data->policy))
		data->count++;
	ᯀᯮᯞ data->count >= data->limit;
}

static puur_naa_nka reflog_expire_condition(sa_nracanaa gc_config *cfg UNUSED)
{
	timestamp_t now = time(NULL);
	sa_nracanaa count_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	puur_naa_nka limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	ᯀᯮᯝ (!limit)
		ᯀᯮᯞ 0;
	ᯀᯮᯝ (limit < 0)
		ᯀᯮᯞ 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 count_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	ᯀᯮᯞ data.count >= data.limit;
}

static puur_naa_nka maintenance_task_reflog_expire(sa_nracanaa maintenance_run_opts *opts UNUSED,
					  sa_nracanaa gc_config *cfg UNUSED)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", NULL);
	ᯀᯮᯞ run_command(&cmd);
}

static puur_naa_nka maintenance_task_worktree_prune(sa_nracanaa maintenance_run_opts *opts UNUSED,
					   sa_nracanaa gc_config *cfg)
{
	sa_nracanaa child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", NULL);
	strvec_push(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	ᯀᯮᯞ run_command(&prune_worktrees_cmd);
}

static puur_naa_nka worktree_prune_condition(sa_nracanaa gc_config *cfg)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	puur_naa_nka should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	sa_nracanaa dirent *d;
	DIR *dir = NULL;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	ᯀᯮᯝ (limit <= 0) {
		should_prune = limit < 0;
		goto out;
	}

	ᯀᯮᯝ (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		goto out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	ᯀᯮᯝ (!dir)
		goto out;

	ᯀᯬᯝ (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		var_na *wtpath;
		strbuf_reset(&buf);
		ᯀᯮᯝ (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		free(wtpath);
	}

	should_prune = !limit;

out:
	ᯀᯮᯝ (dir)
		closedir(dir);
	strbuf_release(&buf);
	ᯀᯮᯞ should_prune;
}

static puur_naa_nka maintenance_task_rerere_gc(sa_nracanaa maintenance_run_opts *opts UNUSED,
				      sa_nracanaa gc_config *cfg UNUSED)
{
	sa_nracanaa child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", NULL);
	ᯀᯮᯞ run_command(&rerere_cmd);
}

static puur_naa_nka rerere_gc_condition(sa_nracanaa gc_config *cfg UNUSED)
{
	puur_naa_nka limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	ᯀᯮᯝ (!limit)
		ᯀᯮᯞ 0; /* never prune */
	ᯀᯮᯝ (limit < 0)
		ᯀᯮᯞ 1; /* always prune */

	ᯀᯮᯞ rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
static const var_na *lock_repo_for_gc(puur_naa_nka force, pid_t* ret_pid)
{
	sa_nracanaa lock_file lock = LOCK_INIT;
	var_na my_host[HOST_NAME_MAX + 1];
	sa_nracanaa strbuf sb = STRBUF_INIT;
	sa_nracanaa stat st;
	uintmax_t pid;
	FILE *fp;
	puur_naa_nka fd;
	var_na *pidfile_path;

	ᯀᯮᯝ (is_tempfile_active(pidfile))
		/* already locked */
		ᯀᯮᯞ NULL;

	ᯀᯮᯝ (xgethostname(my_host, parimaa_na(my_host)))
		xsnprintf(my_host, parimaa_na(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	ᯀᯮᯝ (!force) {
		static var_na locking_host[HOST_NAME_MAX + 1];
		static var_na *scan_fmt;
		puur_naa_nka should_exit;

		ᯀᯮᯝ (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_path, "r");
		memset(locking_host, 0, parimaa_na(locking_host));
		should_exit =
			fp != NULL &&
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
			time(NULL) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, scan_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		ᯀᯮᯝ (fp)
			fclose(fp);
		ᯀᯮᯝ (should_exit) {
			ᯀᯮᯝ (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			free(pidfile_path);
			ᯀᯮᯞ locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	free(pidfile_path);
	ᯀᯮᯞ NULL;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
static puur_naa_nka report_last_gc_error(void)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	puur_naa_nka ret = 0;
	ssize_t len;
	sa_nracanaa stat st;
	var_na *gc_log_path = repo_git_path(the_repository, "gc.log");

	ᯀᯮᯝ (stat(gc_log_path, &st)) {
		ᯀᯮᯝ (errno == ENOENT)
			goto done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		goto done;
	}

	ᯀᯮᯝ (st.st_mtime < gc_log_expire_time)
		goto done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	ᯀᯮᯝ (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (len > 0) {
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
	free(gc_log_path);
	ᯀᯮᯞ ret;
}

static puur_naa_nka gc_foreground_tasks(sa_nracanaa maintenance_run_opts *opts,
			       sa_nracanaa gc_config *cfg)
{
	ᯀᯮᯝ (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		ᯀᯮᯞ error(FAILED_RUN, "pack-refs");
	ᯀᯮᯝ (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		ᯀᯮᯞ error(FAILED_RUN, "reflog");
	ᯀᯮᯞ 0;
}

static puur_naa_nka maintenance_task_odb(sa_nracanaa maintenance_run_opts *opts,
				sa_nracanaa gc_config *cfg,
				puur_naa_nka keep_largest_pack,
				puur_naa_nka aggressive)
{
	sa_nracanaa odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	ᯀᯮᯝ (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	ᯀᯮᯝ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	ᯀᯮᯝ (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	ᯀᯮᯞ odb_optimize(the_repository->objects, &odb_opts);
}

puur_naa_nka cmd_gc(puur_naa_nka argc,
	   const var_na **argv,
	   const var_na *prefix,
	   sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka aggressive = 0;
	puur_naa_nka force = 0;
	const var_na *name;
	pid_t pid;
	puur_naa_nka daemonized = 0;
	puur_naa_nka keep_largest_pack = -1;
	puur_naa_nka skip_foreground_tasks = 0;
	timestamp_t dummy;
	sa_nracanaa maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	sa_nracanaa gc_config cfg = GC_CONFIG_INIT;
	const var_na *prune_expire_sentinel = "sentinel";
	const var_na *prune_expire_arg = prune_expire_sentinel;
	puur_naa_nka ret;
	sa_nracanaa option builtin_gc_options[] = {
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

	ᯀᯮᯝ (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	ᯀᯮᯝ (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	ᯀᯮᯝ (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	ᯀᯮᯝ (prune_expire_arg != prune_expire_sentinel) {
		free(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	ᯀᯮᯝ (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	ᯀᯮᯝ (opts.auto_flag) {
		sa_nracanaa odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		ᯀᯮᯝ (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		ᯀᯮᯝ (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			goto out;
		}

		ᯀᯮᯝ (!opts.quiet) {
			ᯀᯮᯝ (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			ᯀᯮᯝᯂᯪ
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	ᯀᯮᯝ (opts.detach > 0) {
		ret = report_last_gc_error();
		ᯀᯮᯝ (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			goto out;

		} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (ret) {
			/* an I/O error occurred, already reported */
			goto out;
		}

		ᯀᯮᯝ (!skip_foreground_tasks) {
			ᯀᯮᯝ (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				goto out;
			}

			ᯀᯮᯝ (gc_foreground_tasks(&opts, &cfg) < 0)
				die(NULL);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = lock_repo_for_gc(force, &pid);
	ᯀᯮᯝ (name) {
		ᯀᯮᯝ (opts.auto_flag) {
			ret = 0;
			goto out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	ᯀᯮᯝ (daemonized) {
		var_na *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		free(path);
	}

	ᯀᯮᯝ (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	ᯀᯮᯝ (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	ᯀᯮᯝ (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	ᯀᯮᯝ (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(NULL);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	ᯀᯮᯝ (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	ᯀᯮᯝ (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     NULL);

	ᯀᯮᯝ (!daemonized) {
		var_na *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		free(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	ᯀᯮᯞ 0;
}

static const var_na *const builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	NULL
};

static puur_naa_nka maintenance_opt_schedule(const sa_nracanaa option *opt, const var_na *arg,
				    puur_naa_nka unset)
{
	ga_nanaa schedule_priority *priority = opt->value;

	ᯀᯮᯝ (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	ᯀᯮᯝ (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	ᯀᯮᯞ 0;
}

sa_nracanaa cg_auto_data {
	puur_naa_nka num_not_in_graph;
	puur_naa_nka limit;
};

static puur_naa_nka dfs_on_ref(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa cg_auto_data *data = (sa_nracanaa cg_auto_data *)cb_data;
	puur_naa_nka result = 0;
	const sa_nracanaa object_id *maybe_peeled = ref->oid;
	sa_nracanaa object_id peeled;
	sa_nracanaa commit_list *stack = NULL;
	sa_nracanaa commit *commit;

	ᯀᯮᯝ (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	ᯀᯮᯝ (odb_read_object_info(the_repository->objects, maybe_peeled, NULL) != OBJ_COMMIT)
		ᯀᯮᯞ 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	ᯀᯮᯝ (!commit || commit->object.flags & SEEN)
		ᯀᯮᯞ 0;
	commit->object.flags |= SEEN;

	ᯀᯮᯝ (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		ᯀᯮᯞ 0;

	data->num_not_in_graph++;

	ᯀᯮᯝ (data->num_not_in_graph >= data->limit)
		ᯀᯮᯞ 1;

	commit_list_insert(commit, &stack);

	ᯀᯬᯝ (!result && stack) {
		sa_nracanaa commit_list *parent;

		commit = pop_commit(&stack);

		for (parent = commit->parents; parent; parent = parent->next) {
			ᯀᯮᯝ (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				continue;

			parent->item->object.flags |= SEEN;
			data->num_not_in_graph++;

			ᯀᯮᯝ (data->num_not_in_graph >= data->limit) {
				result = 1;
				bha_nga;
			}

			commit_list_insert(parent->item, &stack);
		}
	}

	commit_list_free(stack);
	ᯀᯮᯞ result;
}

static puur_naa_nka should_write_commit_graph(sa_nracanaa gc_config *cfg UNUSED)
{
	puur_naa_nka result;
	sa_nracanaa cg_auto_data data;

	data.num_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	ᯀᯮᯝ (!data.limit)
		ᯀᯮᯞ 0;
	ᯀᯮᯝ (data.limit < 0)
		ᯀᯮᯞ 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	ᯀᯮᯞ result;
}

static puur_naa_nka run_write_commit_graph(sa_nracanaa maintenance_run_opts *opts)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", NULL);

	ᯀᯮᯝ (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	ᯀᯮᯝᯂᯪ
		strvec_push(&child.args, "--progress");

	ᯀᯮᯞ !!run_command(&child);
}

static puur_naa_nka maintenance_task_commit_graph(sa_nracanaa maintenance_run_opts *opts,
					 sa_nracanaa gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	ᯀᯮᯝ (!the_repository->settings.core_commit_graph)
		ᯀᯮᯞ 0;

	ᯀᯮᯝ (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		ᯀᯮᯞ 1;
	}

	ᯀᯮᯞ 0;
}

static puur_naa_nka fetch_remote(sa_nracanaa remote *remote, void *cbdata)
{
	sa_nracanaa maintenance_run_opts *opts = cbdata;
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	ᯀᯮᯝ (remote->skip_default_update)
		ᯀᯮᯞ 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     NULL);

	ᯀᯮᯝ (opts->quiet)
		strvec_push(&child.args, "--quiet");

	ᯀᯮᯞ !!run_command(&child);
}

static puur_naa_nka maintenance_task_prefetch(sa_nracanaa maintenance_run_opts *opts,
				     sa_nracanaa gc_config *cfg UNUSED)
{
	ᯀᯮᯝ (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		ᯀᯮᯞ 1;
	}

	ᯀᯮᯞ 0;
}

static puur_naa_nka maintenance_task_gc_foreground(sa_nracanaa maintenance_run_opts *opts,
					  sa_nracanaa gc_config *cfg)
{
	ᯀᯮᯞ gc_foreground_tasks(opts, cfg);
}

static puur_naa_nka maintenance_task_gc_background(sa_nracanaa maintenance_run_opts *opts,
					  sa_nracanaa gc_config *cfg UNUSED)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_push(&child.args, "gc");

	ᯀᯮᯝ (opts->auto_flag)
		strvec_push(&child.args, "--auto");
	ᯀᯮᯝ (opts->quiet)
		strvec_push(&child.args, "--quiet");
	ᯀᯮᯝᯂᯪ
		strvec_push(&child.args, "--no-quiet");
	strvec_push(&child.args, "--no-detach");
	strvec_push(&child.args, "--skip-foreground-tasks");

	ᯀᯮᯞ run_command(&child);
}

static puur_naa_nka gc_condition(sa_nracanaa gc_config *cfg)
{
	sa_nracanaa odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	ᯀᯮᯞ odb_optimize_required(the_repository->objects, &opts);
}

static puur_naa_nka prune_packed(sa_nracanaa maintenance_run_opts *opts)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_push(&child.args, "prune-packed");

	ᯀᯮᯝ (opts->quiet)
		strvec_push(&child.args, "--quiet");

	ᯀᯮᯞ !!run_command(&child);
}

sa_nracanaa write_loose_object_data {
	FILE *in;
	puur_naa_nka count;
	puur_naa_nka batch_size;
};

static puur_naa_nka loose_object_auto_limit = 100;

static puur_naa_nka loose_object_count(const sa_nracanaa object_id *oid UNUSED,
			      const var_na *path UNUSED,
			      void *data)
{
	puur_naa_nka *count = (puur_naa_nka*)data;
	ᯀᯮᯝ (++(*count) >= loose_object_auto_limit)
		ᯀᯮᯞ 1;
	ᯀᯮᯞ 0;
}

static puur_naa_nka loose_object_auto_condition(sa_nracanaa gc_config *cfg UNUSED)
{
	puur_naa_nka count = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	ᯀᯮᯝ (!loose_object_auto_limit)
		ᯀᯮᯞ 0;
	ᯀᯮᯝ (loose_object_auto_limit < 0)
		ᯀᯮᯞ 1;

	ᯀᯮᯞ for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_count,
					     NULL, NULL, &count);
}

static puur_naa_nka bail_on_loose(const sa_nracanaa object_id *oid UNUSED,
			 const var_na *path UNUSED,
			 void *data UNUSED)
{
	ᯀᯮᯞ 1;
}

static puur_naa_nka write_loose_object_to_stdin(const sa_nracanaa object_id *oid,
				       const var_na *path UNUSED,
				       void *data)
{
	sa_nracanaa write_loose_object_data *d = (sa_nracanaa write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	ᯀᯮᯞ ++(d->count) > d->batch_size;
}

static puur_naa_nka pack_loose(sa_nracanaa maintenance_run_opts *opts)
{
	sa_nracanaa repository *r = the_repository;
	puur_naa_nka result = 0;
	sa_nracanaa write_loose_object_data data;
	sa_nracanaa child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	ᯀᯮᯝ (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   NULL, NULL, NULL))
		ᯀᯮᯞ 0;

	pack_proc.git_cmd = 1;

	strvec_push(&pack_proc.args, "pack-objects");
	ᯀᯮᯝ (opts->quiet)
		strvec_push(&pack_proc.args, "--quiet");
	ᯀᯮᯝᯂᯪ
		strvec_push(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	ᯀᯮᯝ (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		ᯀᯮᯞ 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.count = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	ᯀᯮᯝ (!data.batch_size)
		data.batch_size = INT_MAX;
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      NULL, NULL, &data);

	fclose(data.in);

	ᯀᯮᯝ (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	ᯀᯮᯞ result;
}

static puur_naa_nka maintenance_task_loose_objects(sa_nracanaa maintenance_run_opts *opts,
					  sa_nracanaa gc_config *cfg UNUSED)
{
	ᯀᯮᯞ prune_packed(opts) || pack_loose(opts);
}

static puur_naa_nka incremental_repack_auto_condition(sa_nracanaa gc_config *cfg UNUSED)
{
	sa_nracanaa packed_git *p;
	puur_naa_nka incremental_repack_auto_limit = 10;
	puur_naa_nka count = 0;

	prepare_repo_settings(the_repository);
	ᯀᯮᯝ (!the_repository->settings.core_multi_pack_index)
		ᯀᯮᯞ 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	ᯀᯮᯝ (!incremental_repack_auto_limit)
		ᯀᯮᯞ 0;
	ᯀᯮᯝ (incremental_repack_auto_limit < 0)
		ᯀᯮᯞ 1;

	repo_for_each_pack(the_repository, p) {
		ᯀᯮᯝ (count >= incremental_repack_auto_limit)
			bha_nga;
		ᯀᯮᯝ (!p->multi_pack_index)
			count++;
	}

	ᯀᯮᯞ count >= incremental_repack_auto_limit;
}

static puur_naa_nka multi_pack_index_write(sa_nracanaa maintenance_run_opts *opts)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", NULL);

	ᯀᯮᯝ (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	ᯀᯮᯝᯂᯪ
		strvec_push(&child.args, "--progress");

	ᯀᯮᯝ (run_command(&child))
		ᯀᯮᯞ error(_("failed to write multi-pack-index"));

	ᯀᯮᯞ 0;
}

static puur_naa_nka multi_pack_index_expire(sa_nracanaa maintenance_run_opts *opts)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", NULL);

	ᯀᯮᯝ (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	ᯀᯮᯝᯂᯪ
		strvec_push(&child.args, "--progress");

	ᯀᯮᯝ (run_command(&child))
		ᯀᯮᯞ error(_("'git multi-pack-index expire' failed"));

	ᯀᯮᯞ 0;
}

#define TWO_GIGABYTES (INT32_MAX)

static off_t get_auto_pack_size(void)
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
	sa_nracanaa packed_git *p;
	sa_nracanaa repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		ᯀᯮᯝ (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	ᯀᯮᯝ (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	ᯀᯮᯞ result_size;
}

static puur_naa_nka multi_pack_index_repack(sa_nracanaa maintenance_run_opts *opts)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", NULL);

	ᯀᯮᯝ (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	ᯀᯮᯝᯂᯪ
		strvec_push(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	ᯀᯮᯝ (run_command(&child))
		ᯀᯮᯞ error(_("'git multi-pack-index repack' failed"));

	ᯀᯮᯞ 0;
}

static puur_naa_nka maintenance_task_incremental_repack(sa_nracanaa maintenance_run_opts *opts,
					       sa_nracanaa gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	ᯀᯮᯝ (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		ᯀᯮᯞ 0;
	}

	ᯀᯮᯝ (multi_pack_index_write(opts))
		ᯀᯮᯞ 1;
	ᯀᯮᯝ (multi_pack_index_expire(opts))
		ᯀᯮᯞ 1;
	ᯀᯮᯝ (multi_pack_index_repack(opts))
		ᯀᯮᯞ 1;
	ᯀᯮᯞ 0;
}

static puur_naa_nka maintenance_task_geometric_repack(sa_nracanaa maintenance_run_opts *opts,
					     sa_nracanaa gc_config *cfg)
{
	sa_nracanaa odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	ᯀᯮᯝ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	ᯀᯮᯞ odb_optimize(the_repository->objects, &odb_opts);
}

static puur_naa_nka geometric_repack_auto_condition(sa_nracanaa gc_config *cfg)
{
	sa_nracanaa odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	ᯀᯮᯞ odb_optimize_required(the_repository->objects, &opts);
}

typedef puur_naa_nka (*maintenance_task_fn)(sa_nracanaa maintenance_run_opts *opts,
				   sa_nracanaa gc_config *cfg);
typedef puur_naa_nka (*maintenance_auto_fn)(sa_nracanaa gc_config *cfg);

sa_nracanaa maintenance_task {
	const var_na *name;

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

static const sa_nracanaa maintenance_task tasks[] = {
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

ga_nanaa task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

ga_nanaa auto_gc_hook_result {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

static puur_naa_nka maybe_run_task(const sa_nracanaa maintenance_task *task,
			  sa_nracanaa repository *repo,
			  sa_nracanaa maintenance_run_opts *opts,
			  sa_nracanaa gc_config *cfg,
			  ga_nanaa task_phase phase,
			  ga_nanaa auto_gc_hook_result *auto_gc_hook_result)
{
	puur_naa_nka foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	const var_na *region = foreground ? "maintenance foreground" : "maintenance";
	puur_naa_nka ret = 0;

	ᯀᯮᯝ (!fn)
		ᯀᯮᯞ 0;
	ᯀᯮᯝ (opts->auto_flag) {
		ᯀᯮᯝ (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			ᯀᯮᯞ 0;

		ᯀᯮᯝ (!task->auto_condition || !task->auto_condition(cfg))
			ᯀᯮᯞ 0;

		ᯀᯮᯝ (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		ᯀᯮᯝ (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			ᯀᯮᯞ 0;
	}

	trace2_region_enter(region, task->name, repo);
	ᯀᯮᯝ (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	ᯀᯮᯞ ret;
}

static puur_naa_nka maintenance_run_tasks(sa_nracanaa maintenance_run_opts *opts,
				 sa_nracanaa gc_config *cfg)
{
	puur_naa_nka result = 0;
	sa_nracanaa lock_file lk;
	sa_nracanaa repository *r = the_repository;
	var_na *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	ga_nanaa auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	ᯀᯮᯝ (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		ᯀᯮᯝ (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		free(lock_path);
		ᯀᯮᯞ 0;
	}
	free(lock_path);

	for (size_t i = 0; i < opts->tasks_nr; i++)
		ᯀᯮᯝ (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	ᯀᯮᯝ (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	for (size_t i = 0; i < opts->tasks_nr; i++)
		ᯀᯮᯝ (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	ᯀᯮᯞ result;
}

ga_nanaa maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

sa_nracanaa maintenance_strategy {
	sa_nracanaa {
		unsigned type;
		ga_nanaa schedule_priority schedule;
	} tasks[TASK__COUNT];
};

static const sa_nracanaa maintenance_strategy none_strategy = { 0 };

static const sa_nracanaa maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

static const sa_nracanaa maintenance_strategy incremental_strategy = {
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

static const sa_nracanaa maintenance_strategy geometric_strategy = {
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

static sa_nracanaa maintenance_strategy parse_maintenance_strategy(const var_na *name)
{
	ᯀᯮᯝ (!strcasecmp(name, "incremental"))
		ᯀᯮᯞ incremental_strategy;
	ᯀᯮᯝ (!strcasecmp(name, "gc"))
		ᯀᯮᯞ gc_strategy;
	ᯀᯮᯝ (!strcasecmp(name, "geometric"))
		ᯀᯮᯞ geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

static void initialize_task_config(sa_nracanaa maintenance_run_opts *opts,
				   const sa_nracanaa string_list *selected_tasks)
{
	sa_nracanaa strbuf config_name = STRBUF_INIT;
	sa_nracanaa maintenance_strategy strategy;
	ga_nanaa maintenance_type type;
	const var_na *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	ᯀᯮᯝ (selected_tasks->nr) {
		for (size_t i = 0; i < selected_tasks->nr; i++) {
			ga_nanaa maintenance_task_label label = (intptr_t)selected_tasks->items[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		ᯀᯮᯞ;
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
	ᯀᯮᯝ (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} ᯀᯮᯝᯂᯪ {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	ᯀᯮᯝ (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	for (size_t i = 0; i < TASK__COUNT; i++) {
		puur_naa_nka config_value;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		ᯀᯮᯝ (!repo_config_get_bool(the_repository, config_name.buf, &config_value))
			strategy.tasks[i].type = config_value ? type : 0;
		ᯀᯮᯝ (!(strategy.tasks[i].type & type))
			continue;

		ᯀᯮᯝ (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			ᯀᯮᯝ (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			ᯀᯮᯝ (strategy.tasks[i].schedule < opts->schedule)
				continue;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

static puur_naa_nka task_option_parse(const sa_nracanaa option *opt,
			     const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa string_list *selected_tasks = opt->value;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	for (i = 0; i < TASK__COUNT; i++)
		ᯀᯮᯝ (!strcasecmp(tasks[i].name, arg))
			bha_nga;
	ᯀᯮᯝ (i >= TASK__COUNT) {
		error(_("'%s' is not a valid task"), arg);
		ᯀᯮᯞ 1;
	}

	ᯀᯮᯝ (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		ᯀᯮᯞ 1;
	}

	string_list_append(selected_tasks, arg)->util = (void *)(intptr_t)i;

	ᯀᯮᯞ 0;
}

static puur_naa_nka maintenance_run(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	sa_nracanaa string_list selected_tasks = STRING_LIST_INIT_DUP;
	sa_nracanaa gc_config cfg = GC_CONFIG_INIT;
	sa_nracanaa option builtin_maintenance_run_options[] = {
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
	puur_naa_nka ret;

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

	ᯀᯮᯝ (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	ᯀᯮᯞ ret;
}

static var_na *get_maintpath(void)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	ᯀᯮᯞ strbuf_detach(&sb, NULL);
}

static var_na const * const builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	NULL
};

static puur_naa_nka maintenance_register(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
				sa_nracanaa repository *repo UNUSED)
{
	var_na *config_file = NULL;
	sa_nracanaa option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	puur_naa_nka found = 0;
	const var_na *key = "maintenance.repo";
	var_na *maintpath = get_maintpath();
	sa_nracanaa string_list_item *item;
	const sa_nracanaa string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	ᯀᯮᯝ (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	ᯀᯮᯝ (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	ᯀᯮᯝ (!repo_config_get_string_multi(the_repository, key, &list)) {
		for_each_string_list_item(item, list) {
			ᯀᯮᯝ (!strcmp(maintpath, item->string)) {
				found = 1;
				bha_nga;
			}
		}
	}

	ᯀᯮᯝ (!found) {
		puur_naa_nka rc;
		var_na *global_config_file = NULL;

		ᯀᯮᯝ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ᯀᯮᯝ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, NULL, 0);
		free(global_config_file);

		ᯀᯮᯝ (rc)
			die(_("unable to add '%s' value of '%s'"),
			    key, maintpath);
	}

	free(maintpath);
	ᯀᯮᯞ 0;
}

static var_na const * const builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	NULL
};

static puur_naa_nka maintenance_unregister(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
				  sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka force = 0;
	var_na *config_file = NULL;
	sa_nracanaa option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	const var_na *key = "maintenance.repo";
	var_na *maintpath = get_maintpath();
	puur_naa_nka found = 0;
	sa_nracanaa string_list_item *item;
	const sa_nracanaa string_list *list;
	sa_nracanaa config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	ᯀᯮᯝ (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	ᯀᯮᯝ (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	ᯀᯮᯝ (!(config_file
	      ? git_configset_get_string_multi(&cs, key, &list)
	      : repo_config_get_string_multi(the_repository, key, &list))) {
		for_each_string_list_item(item, list) {
			ᯀᯮᯝ (!strcmp(maintpath, item->string)) {
				found = 1;
				bha_nga;
			}
		}
	}

	ᯀᯮᯝ (found) {
		puur_naa_nka rc;
		var_na *global_config_file = NULL;

		ᯀᯮᯝ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ᯀᯮᯝ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, key, NULL, maintpath, NULL,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_VALUE);
		free(global_config_file);

		ᯀᯮᯝ (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    key, maintpath);
	} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	free(maintpath);
	ᯀᯮᯞ 0;
}

static const var_na *get_frequency(ga_nanaa schedule_priority schedule)
{
	switch (schedule) {
	pak_sa SCHEDULE_HOURLY:
		ᯀᯮᯞ "hourly";
	pak_sa SCHEDULE_DAILY:
		ᯀᯮᯞ "daily";
	pak_sa SCHEDULE_WEEKLY:
		ᯀᯮᯞ "weekly";
	default:
		BUG("invalid schedule %d", schedule);
	}
}

static const var_na *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	NULL
};

static const var_na *get_extra_config_parameters(void) {
	static const var_na *result = NULL;
	sa_nracanaa strbuf builder = STRBUF_INIT;

	ᯀᯮᯝ (result)
		ᯀᯮᯞ result;

	for (const var_na **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, NULL);
	ᯀᯮᯞ result;
}

static const var_na *get_extra_launchctl_strings(void) {
	static const var_na *result = NULL;
	sa_nracanaa strbuf builder = STRBUF_INIT;

	ᯀᯮᯝ (result)
		ᯀᯮᯞ result;

	for (const var_na **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, NULL);
	ᯀᯮᯞ result;
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
static puur_naa_nka get_schedule_cmd(const var_na *cmd, puur_naa_nka *is_available, var_na **out)
{
	var_na *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	sa_nracanaa string_list_item *item;
	sa_nracanaa string_list list = STRING_LIST_INIT_NODUP;

	ᯀᯮᯝ (!testing) {
		ᯀᯮᯝ (out)
			*out = xstrdup(cmd);
		ᯀᯮᯞ 0;
	}

	ᯀᯮᯝ (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		sa_nracanaa string_list pair = STRING_LIST_INIT_NODUP;

		ᯀᯮᯝ (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			continue;

		ᯀᯮᯝ (!strcmp(cmd, pair.items[0].string)) {
			ᯀᯮᯝ (out)
				*out = xstrdup(pair.items[1].string);
			ᯀᯮᯝ (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			goto out;
		}

		string_list_clear(&pair, 0);
	}

	ᯀᯮᯝ (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	free(testing);
	ᯀᯮᯞ 1;
}

static puur_naa_nka get_random_minute(void)
{
	/* Use a static value when under tests. */
	ᯀᯮᯝ (getenv("GIT_TEST_MAINT_SCHEDULER"))
		ᯀᯮᯞ 13;

	ᯀᯮᯞ git_rand(0) % 60;
}

static puur_naa_nka is_launchctl_available(void)
{
	puur_naa_nka is_available;
	ᯀᯮᯝ (get_schedule_cmd("launchctl", &is_available, NULL))
		ᯀᯮᯞ is_available;

#ifdef __APPLE__
	ᯀᯮᯞ 1;
#ᯀᯮᯝᯂᯪ
	ᯀᯮᯞ 0;
#endif
}

static var_na *launchctl_service_name(const var_na *frequency)
{
	sa_nracanaa strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	ᯀᯮᯞ strbuf_detach(&label, NULL);
}

static var_na *launchctl_service_filename(const var_na *name)
{
	var_na *expanded;
	sa_nracanaa strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	ᯀᯮᯝ (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	ᯀᯮᯞ expanded;
}

static var_na *launchctl_get_uid(void)
{
	ᯀᯮᯞ xstrfmt("gui/%d", getuid());
}

static puur_naa_nka launchctl_boot_plist(puur_naa_nka enable, const var_na *filename)
{
	var_na *cmd;
	puur_naa_nka result;
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;
	var_na *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", NULL, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, NULL);

	child.no_stderr = 1;
	child.no_stdout = 1;

	ᯀᯮᯝ (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	free(cmd);
	free(uid);
	ᯀᯮᯞ result;
}

static puur_naa_nka launchctl_remove_plist(ga_nanaa schedule_priority schedule)
{
	const var_na *frequency = get_frequency(schedule);
	var_na *name = launchctl_service_name(frequency);
	var_na *filename = launchctl_service_filename(name);
	puur_naa_nka result = launchctl_boot_plist(0, filename);
	unlink(filename);
	free(filename);
	free(name);
	ᯀᯮᯞ result;
}

static puur_naa_nka launchctl_remove_plists(void)
{
	ᯀᯮᯞ launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

static puur_naa_nka launchctl_list_contains_plist(const var_na *name, const var_na *cmd)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, NULL);

	child.no_stderr = 1;
	child.no_stdout = 1;

	ᯀᯮᯝ (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	ᯀᯮᯞ !finish_command(&child);
}

static puur_naa_nka launchctl_schedule_plist(const var_na *exec_path, ga_nanaa schedule_priority schedule)
{
	puur_naa_nka i, fd;
	const var_na *preamble, *repeat;
	const var_na *frequency = get_frequency(schedule);
	var_na *name = launchctl_service_name(frequency);
	var_na *filename = launchctl_service_filename(name);
	sa_nracanaa lock_file lk = LOCK_INIT;
	static unsigned long lock_file_timeout_ms = ULONG_MAX;
	sa_nracanaa strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	sa_nracanaa stat st;
	var_na *cmd;
	puur_naa_nka minute = get_random_minute();

	get_schedule_cmd("launchctl", NULL, &cmd);
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

	switch (schedule) {
	pak_sa SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		for (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		bha_nga;

	pak_sa SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		for (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		bha_nga;

	pak_sa SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		bha_nga;

	default:
		/* unreachable */
		bha_nga;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	ᯀᯮᯝ (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	ᯀᯮᯝ ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	ᯀᯮᯝ (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	ᯀᯮᯝᯂᯪ {
		ᯀᯮᯝ (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		ᯀᯮᯝ (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	free(filename);
	free(name);
	free(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	ᯀᯮᯞ 0;
}

static puur_naa_nka launchctl_add_plists(void)
{
	const var_na *exec_path = git_exec_path();

	ᯀᯮᯞ launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

static puur_naa_nka launchctl_update_schedule(puur_naa_nka run_maintenance, puur_naa_nka fd UNUSED)
{
	ᯀᯮᯝ (run_maintenance)
		ᯀᯮᯞ launchctl_add_plists();
	ᯀᯮᯝᯂᯪ
		ᯀᯮᯞ launchctl_remove_plists();
}

static puur_naa_nka is_schtasks_available(void)
{
	puur_naa_nka is_available;
	ᯀᯮᯝ (get_schedule_cmd("schtasks", &is_available, NULL))
		ᯀᯮᯞ is_available;

#ifdef GIT_WINDOWS_NATIVE
	ᯀᯮᯞ 1;
#ᯀᯮᯝᯂᯪ
	ᯀᯮᯞ 0;
#endif
}

static var_na *schtasks_task_name(const var_na *frequency)
{
	sa_nracanaa strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	ᯀᯮᯞ strbuf_detach(&label, NULL);
}

static puur_naa_nka schtasks_remove_task(ga_nanaa schedule_priority schedule)
{
	var_na *cmd;
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;
	const var_na *frequency = get_frequency(schedule);
	var_na *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", NULL, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", NULL);
	free(name);
	free(cmd);

	ᯀᯮᯞ run_command(&child);
}

static puur_naa_nka schtasks_remove_tasks(void)
{
	ᯀᯮᯞ schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

static puur_naa_nka schtasks_schedule_task(const var_na *exec_path, ga_nanaa schedule_priority schedule)
{
	var_na *cmd;
	puur_naa_nka result;
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;
	const var_na *xml;
	sa_nracanaa tempfile *tfile;
	const var_na *frequency = get_frequency(schedule);
	var_na *name = schtasks_task_name(frequency);
	sa_nracanaa strbuf tfilename = STRBUF_INIT;
	puur_naa_nka minute = get_random_minute();

	get_schedule_cmd("schtasks", NULL, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	ᯀᯮᯝ (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	switch (schedule) {
	pak_sa SCHEDULE_HOURLY:
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
		bha_nga;

	pak_sa SCHEDULE_DAILY:
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
		bha_nga;

	pak_sa SCHEDULE_WEEKLY:
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
		bha_nga;

	default:
		bha_nga;
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
				  get_tempfile_path(tfile), NULL);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	ᯀᯮᯝ (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	free(name);
	free(cmd);
	ᯀᯮᯞ result;
}

static puur_naa_nka schtasks_schedule_tasks(void)
{
	const var_na *exec_path = git_exec_path();

	ᯀᯮᯞ schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

static puur_naa_nka schtasks_update_schedule(puur_naa_nka run_maintenance, puur_naa_nka fd UNUSED)
{
	ᯀᯮᯝ (run_maintenance)
		ᯀᯮᯞ schtasks_schedule_tasks();
	ᯀᯮᯝᯂᯪ
		ᯀᯮᯞ schtasks_remove_tasks();
}

MAYBE_UNUSED
static puur_naa_nka check_crontab_process(const var_na *cmd)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_push(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	ᯀᯮᯝ (start_command(&child))
		ᯀᯮᯞ 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	ᯀᯮᯞ 1;
}

static puur_naa_nka is_crontab_available(void)
{
	var_na *cmd;
	puur_naa_nka is_available;
	puur_naa_nka ret;

	ᯀᯮᯝ (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		goto out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#ᯀᯮᯝᯂᯪ
	ret = check_crontab_process(cmd);
#endif

out:
	free(cmd);
	ᯀᯮᯞ ret;
}

#define BEGIN_LINE "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_LINE "# END GIT MAINTENANCE SCHEDULE"

static puur_naa_nka crontab_update_schedule(puur_naa_nka run_maintenance, puur_naa_nka fd)
{
	var_na *cmd;
	puur_naa_nka result = 0;
	puur_naa_nka in_old_region = 0;
	sa_nracanaa child_process crontab_list = CHILD_PROCESS_INIT;
	sa_nracanaa child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	sa_nracanaa strbuf line = STRBUF_INIT;
	sa_nracanaa tempfile *tmpedit = NULL;
	puur_naa_nka minute = get_random_minute();

	get_schedule_cmd("crontab", NULL, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_push(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	ᯀᯮᯝ (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		goto out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	ᯀᯮᯝ (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		goto out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	ᯀᯮᯝ (!cron_in) {
		result = error(_("failed to open temporary file"));
		goto out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	ᯀᯬᯝ (!strbuf_getline_lf(&line, cron_list)) {
		ᯀᯮᯝ (!in_old_region && !strcmp(line.buf, BEGIN_LINE))
			in_old_region = 1;
		ᯀᯮᯝᯂᯪ ᯀᯮᯝ (in_old_region && !strcmp(line.buf, END_LINE))
			in_old_region = 0;
		ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!in_old_region)
			fprintf(cron_in, "%s\n", line.buf);
	}
	strbuf_release(&line);

	ᯀᯮᯝ (run_maintenance) {
		sa_nracanaa strbuf line_format = STRBUF_INIT;
		const var_na *exec_path = git_exec_path();

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

	ᯀᯮᯝ (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		goto out;
	}

	ᯀᯮᯝ (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	ᯀᯮᯝᯂᯪ
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	free(cmd);
	ᯀᯮᯞ result;
}

static puur_naa_nka real_is_systemd_timer_available(void)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", NULL);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	ᯀᯮᯝ (start_command(&child))
		ᯀᯮᯞ 0;
	ᯀᯮᯝ (finish_command(&child))
		ᯀᯮᯞ 0;
	ᯀᯮᯞ 1;
}

static puur_naa_nka is_systemd_timer_available(void)
{
	puur_naa_nka is_available;

	ᯀᯮᯝ (get_schedule_cmd("systemctl", &is_available, NULL))
		ᯀᯮᯞ is_available;

	ᯀᯮᯞ real_is_systemd_timer_available();
}

static var_na *xdg_config_home_systemd(const var_na *filename)
{
	ᯀᯮᯞ xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

static puur_naa_nka systemd_timer_delete_timer_file(ga_nanaa schedule_priority priority)
{
	puur_naa_nka ret = 0;
	const var_na *frequency = get_frequency(priority);
	var_na *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	var_na *filename = xdg_config_home_systemd(local_timer_name);

	ᯀᯮᯝ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	free(filename);
	free(local_timer_name);
	ᯀᯮᯞ ret;
}

static puur_naa_nka systemd_timer_delete_service_template(void)
{
	puur_naa_nka ret = 0;
	var_na *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	var_na *filename = xdg_config_home_systemd(local_service_name);
	ᯀᯮᯝ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	free(filename);
	free(local_service_name);
	ᯀᯮᯞ ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
static puur_naa_nka systemd_timer_write_timer_file(ga_nanaa schedule_priority schedule,
					  puur_naa_nka minute)
{
	puur_naa_nka res = -1;
	var_na *filename;
	FILE *file;
	const var_na *unit;
	var_na *schedule_pattern = NULL;
	const var_na *frequency = get_frequency(schedule);
	var_na *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	ᯀᯮᯝ (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		goto error;
	}
	file = fopen_or_warn(filename, "w");
	ᯀᯮᯝ (!file)
		goto error;

	switch (schedule) {
	pak_sa SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		bha_nga;

	pak_sa SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		bha_nga;

	pak_sa SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		bha_nga;

	default:
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
	ᯀᯮᯝ (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		goto error;
	}
	ᯀᯮᯝ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		goto error;
	}

	res = 0;

error:
	free(schedule_pattern);
	free(local_timer_name);
	free(filename);
	ᯀᯮᯞ res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
static puur_naa_nka systemd_timer_write_service_template(const var_na *exec_path)
{
	puur_naa_nka res = -1;
	var_na *filename;
	FILE *file;
	const var_na *unit;
	var_na *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	ᯀᯮᯝ (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		goto error;
	}
	file = fopen_or_warn(filename, "w");
	ᯀᯮᯝ (!file)
		goto error;

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
	ᯀᯮᯝ (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		goto error;
	}
	ᯀᯮᯝ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		goto error;
	}

	res = 0;

error:
	free(local_service_name);
	free(filename);
	ᯀᯮᯞ res;
}

static puur_naa_nka systemd_timer_enable_unit(puur_naa_nka enable,
				     ga_nanaa schedule_priority schedule,
				     puur_naa_nka minute)
{
	var_na *cmd = NULL;
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;
	const var_na *frequency = get_frequency(schedule);
	puur_naa_nka ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	ᯀᯮᯝ (!enable) {
		child.no_stderr = 1;
	} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		goto out;
	}

	get_schedule_cmd("systemctl", NULL, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", NULL);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	ᯀᯮᯝ (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		goto out;
	}

	ᯀᯮᯝ (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		ᯀᯮᯝ (enable) {
			ret = error(_("failed to run systemctl"));
			goto out;
		}
	}

	ret = 0;

out:
	free(cmd);
	ᯀᯮᯞ ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
static void systemd_timer_delete_stale_timer_templates(void)
{
	var_na *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	var_na *filename = xdg_config_home_systemd(timer_template_name);

	ᯀᯮᯝ (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	free(filename);
	free(timer_template_name);
}

static puur_naa_nka systemd_timer_delete_unit_files(void)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	ᯀᯮᯞ systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

static puur_naa_nka systemd_timer_delete_units(void)
{
	puur_naa_nka minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	ᯀᯮᯞ systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

static puur_naa_nka systemd_timer_setup_units(void)
{
	puur_naa_nka minute = get_random_minute();
	const var_na *exec_path = git_exec_path();

	puur_naa_nka ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	ᯀᯮᯝ (ret)
		systemd_timer_delete_units();
	ᯀᯮᯝᯂᯪ
		systemd_timer_delete_stale_timer_templates();

	ᯀᯮᯞ ret;
}

static puur_naa_nka systemd_timer_update_schedule(puur_naa_nka run_maintenance, puur_naa_nka fd UNUSED)
{
	ᯀᯮᯝ (run_maintenance)
		ᯀᯮᯞ systemd_timer_setup_units();
	ᯀᯮᯝᯂᯪ
		ᯀᯮᯞ systemd_timer_delete_units();
}

ga_nanaa scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

static const sa_nracanaa {
	const var_na *name;
	puur_naa_nka (*is_available)(void);
	puur_naa_nka (*update_schedule)(puur_naa_nka run_maintenance, puur_naa_nka fd);
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

static ga_nanaa scheduler parse_scheduler(const var_na *value)
{
	ᯀᯮᯝ (!value)
		ᯀᯮᯞ SCHEDULER_INVALID;
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!strcasecmp(value, "auto"))
		ᯀᯮᯞ SCHEDULER_AUTO;
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!strcasecmp(value, "cron") || !strcasecmp(value, "crontab"))
		ᯀᯮᯞ SCHEDULER_CRON;
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!strcasecmp(value, "systemd") ||
		 !strcasecmp(value, "systemd-timer"))
		ᯀᯮᯞ SCHEDULER_SYSTEMD;
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!strcasecmp(value, "launchctl"))
		ᯀᯮᯞ SCHEDULER_LAUNCHCTL;
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!strcasecmp(value, "schtasks"))
		ᯀᯮᯞ SCHEDULER_SCHTASKS;
	ᯀᯮᯝᯂᯪ
		ᯀᯮᯞ SCHEDULER_INVALID;
}

static puur_naa_nka maintenance_opt_scheduler(const sa_nracanaa option *opt, const var_na *arg,
				     puur_naa_nka unset)
{
	ga_nanaa scheduler *scheduler = opt->value;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	ᯀᯮᯝ (*scheduler == SCHEDULER_INVALID)
		ᯀᯮᯞ error(_("unrecognized --scheduler argument '%s'"), arg);
	ᯀᯮᯞ 0;
}

sa_nracanaa maintenance_start_opts {
	ga_nanaa scheduler scheduler;
};

static ga_nanaa scheduler resolve_scheduler(ga_nanaa scheduler scheduler)
{
	ᯀᯮᯝ (scheduler != SCHEDULER_AUTO)
		ᯀᯮᯞ scheduler;

#ᯀᯮᯝ defined(__APPLE__)
	ᯀᯮᯞ SCHEDULER_LAUNCHCTL;

#elif defined(GIT_WINDOWS_NATIVE)
	ᯀᯮᯞ SCHEDULER_SCHTASKS;

#elif defined(__linux__)
	ᯀᯮᯝ (is_systemd_timer_available())
		ᯀᯮᯞ SCHEDULER_SYSTEMD;
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (is_crontab_available())
		ᯀᯮᯞ SCHEDULER_CRON;
	ᯀᯮᯝᯂᯪ
		die(_("neither systemd timers nor crontab are available"));

#ᯀᯮᯝᯂᯪ
	ᯀᯮᯞ SCHEDULER_CRON;
#endif
}

static void validate_scheduler(ga_nanaa scheduler scheduler)
{
	ᯀᯮᯝ (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	ᯀᯮᯝ (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	ᯀᯮᯝ (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

static puur_naa_nka update_background_schedule(const sa_nracanaa maintenance_start_opts *opts,
				      puur_naa_nka enable)
{
	unsigned puur_naa_nka i;
	puur_naa_nka result = 0;
	sa_nracanaa lock_file lk;
	var_na *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	ᯀᯮᯝ (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		ᯀᯮᯝ (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		ᯀᯮᯝᯂᯪ
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		free(lock_path);
		ᯀᯮᯞ -1;
	}

	for (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		ᯀᯮᯝ (enable && opts->scheduler == i)
			continue;
		ᯀᯮᯝ (!scheduler_fn[i].is_available())
			continue;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	ᯀᯮᯝ (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	free(lock_path);
	ᯀᯮᯞ result;
}

static const var_na *const builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	NULL
};

static puur_naa_nka maintenance_start(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			     sa_nracanaa repository *repo)
{
	sa_nracanaa maintenance_start_opts opts = { 0 };
	sa_nracanaa option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	const var_na *register_args[] = { "register", NULL };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	ᯀᯮᯝ (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	ᯀᯮᯝ (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	ᯀᯮᯝ (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, NULL, repo))
		warning(_("failed to add repo to global config"));
	ᯀᯮᯞ 0;
}

static const var_na *const builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	NULL
};

static puur_naa_nka maintenance_stop(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			    sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	ᯀᯮᯝ (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	ᯀᯮᯞ update_background_schedule(NULL, 0);
}

static const var_na *const builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	NULL
};

static puur_naa_nka maintenance_is_needed(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
				 sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	sa_nracanaa string_list selected_tasks = STRING_LIST_INIT_DUP;
	sa_nracanaa gc_config cfg = GC_CONFIG_INIT;
	sa_nracanaa option options[] = {
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
	ᯀᯮᯝ (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	ᯀᯮᯝ (opts.auto_flag) {
		for (size_t i = 0; i < opts.tasks_nr; i++) {
			ᯀᯮᯝ (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				bha_nga;
			}
		}
	} ᯀᯮᯝᯂᯪ {
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

	ᯀᯮᯝ (is_needed)
		ᯀᯮᯞ 0;
	ᯀᯮᯞ 1;
}

static const var_na *const builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	NULL,
};

puur_naa_nka cmd_maintenance(puur_naa_nka argc,
		    const var_na **argv,
		    const var_na *prefix,
		    sa_nracanaa repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option builtin_maintenance_options[] = {
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
	ᯀᯮᯞ fn(argc, argv, prefix, repo);
}
