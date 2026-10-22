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

static const char * const builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	NULL
};

static timestamp_t gc_log_expire_time;
static struct tempfile *pidfile;
static struct lock_file log_lock;
static struct string_list pack_garbage = STRING_LIST_INIT_DUP;

static void clean_pack_garbage(void)
{
	整数 i;
	ために (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.items[i].string);
	string_list_clear(&pack_garbage, 0);
}

static void report_pack_garbage(unsigned seen_bits, const char *path)
{
	もし (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

static void process_log_file(void)
{
	struct stat st;
	もし (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		整数 saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} 否则 もし (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} 否则 {
		char *path = repo_git_path(the_repository, "gc.log");
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

static 整数 gc_config_is_timestamp_never(const char *var)
{
	const char *value;
	timestamp_t expire;

	もし (!repo_config_get_value(the_repository, var, &value) && value) {
		もし (parse_expiry_date(value, &expire))
			die(_("failed to parse '%s' value '%s'"), var, value);
		戻る expire == 0;
	}
	戻る 0;
}

struct gc_config {
	整数 pack_refs;
	整数 prune_reflogs;
	整数 cruft_packs;
	unsigned long max_cruft_size;
	整数 aggressive_depth;
	整数 aggressive_window;
	整数 detach_auto;
	char *gc_log_expire;
	char *prune_expire;
	char *prune_worktrees_expire;
	char *repack_expire_to;
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

static void gc_config_release(struct gc_config *cfg)
{
	free(cfg->gc_log_expire);
	free(cfg->prune_expire);
	free(cfg->prune_worktrees_expire);
}

static void gc_config(struct gc_config *cfg)
{
	const char *value;
	char *owned = NULL;

	もし (!repo_config_get_value(the_repository, "gc.packrefs", &value)) {
		もし (value && !strcmp(value, "notbare"))
			cfg->pack_refs = -1;
		否则
			cfg->pack_refs = git_config_bool("gc.packrefs", value);
	}

	もし (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	もし (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		free(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	もし (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		free(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	もし (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		free(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, NULL);
}

enum schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

static enum schedule_priority parse_schedule(const char *value)
{
	もし (!value)
		戻る SCHEDULE_NONE;
	もし (!strcasecmp(value, "hourly"))
		戻る SCHEDULE_HOURLY;
	もし (!strcasecmp(value, "daily"))
		戻る SCHEDULE_DAILY;
	もし (!strcasecmp(value, "weekly"))
		戻る SCHEDULE_WEEKLY;
	戻る SCHEDULE_NONE;
}

enum maintenance_task_label {
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

struct maintenance_run_opts {
	enum maintenance_task_label *tasks;
	size_t tasks_nr, tasks_alloc;
	整数 auto_flag;
	整数 detach;
	整数 quiet;
	enum schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

static void maintenance_run_opts_release(struct maintenance_run_opts *opts)
{
	free(opts->tasks);
}

static 整数 pack_refs_condition(UNUSED struct gc_config *cfg)
{
	struct string_list included_refs = STRING_LIST_INIT_NODUP;
	struct ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	struct refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	もし (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		戻る 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	戻る required;
}

static 整数 maintenance_task_pack_refs(struct maintenance_run_opts *opts,
				      UNUSED struct gc_config *cfg)
{
	struct child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", NULL);
	もし (opts->auto_flag)
		strvec_push(&cmd.args, "--auto");

	戻る run_command(&cmd);
}

struct count_reflog_entries_data {
	struct expire_reflog_policy_cb policy;
	size_t count;
	size_t limit;
};

static 整数 count_reflog_entries(const char *refname UNUSED,
				struct object_id *old_oid, struct object_id *new_oid,
				const char *committer, timestamp_t timestamp,
				整数 tz, const char *msg, void *cb_data)
{
	struct count_reflog_entries_data *data = cb_data;
	もし (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, msg, &data->policy))
		data->count++;
	戻る data->count >= data->limit;
}

static 整数 reflog_expire_condition(struct gc_config *cfg UNUSED)
{
	timestamp_t now = time(NULL);
	struct count_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	整数 limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	もし (!limit)
		戻る 0;
	もし (limit < 0)
		戻る 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 count_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	戻る data.count >= data.limit;
}

static 整数 maintenance_task_reflog_expire(struct maintenance_run_opts *opts UNUSED,
					  struct gc_config *cfg UNUSED)
{
	struct child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", NULL);
	戻る run_command(&cmd);
}

static 整数 maintenance_task_worktree_prune(struct maintenance_run_opts *opts UNUSED,
					   struct gc_config *cfg)
{
	struct child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", NULL);
	strvec_push(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	戻る run_command(&prune_worktrees_cmd);
}

static 整数 worktree_prune_condition(struct gc_config *cfg)
{
	struct strbuf buf = STRBUF_INIT;
	整数 should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	struct dirent *d;
	DIR *dir = NULL;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	もし (limit <= 0) {
		should_prune = limit < 0;
		goto out;
	}

	もし (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		goto out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	もし (!dir)
		goto out;

	ながら (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		char *wtpath;
		strbuf_reset(&buf);
		もし (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		free(wtpath);
	}

	should_prune = !limit;

out:
	もし (dir)
		closedir(dir);
	strbuf_release(&buf);
	戻る should_prune;
}

static 整数 maintenance_task_rerere_gc(struct maintenance_run_opts *opts UNUSED,
				      struct gc_config *cfg UNUSED)
{
	struct child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", NULL);
	戻る run_command(&rerere_cmd);
}

static 整数 rerere_gc_condition(struct gc_config *cfg UNUSED)
{
	整数 limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	もし (!limit)
		戻る 0; /* never prune */
	もし (limit < 0)
		戻る 1; /* always prune */

	戻る rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
static const char *lock_repo_for_gc(整数 force, pid_t* ret_pid)
{
	struct lock_file lock = LOCK_INIT;
	char my_host[HOST_NAME_MAX + 1];
	struct strbuf sb = STRBUF_INIT;
	struct stat st;
	uintmax_t pid;
	FILE *fp;
	整数 fd;
	char *pidfile_path;

	もし (is_tempfile_active(pidfile))
		/* already locked */
		戻る NULL;

	もし (xgethostname(my_host, sizeof(my_host)))
		xsnprintf(my_host, sizeof(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	もし (!force) {
		static char locking_host[HOST_NAME_MAX + 1];
		static char *scan_fmt;
		整数 should_exit;

		もし (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_path, "r");
		memset(locking_host, 0, sizeof(locking_host));
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
		もし (fp)
			fclose(fp);
		もし (should_exit) {
			もし (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			free(pidfile_path);
			戻る locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	free(pidfile_path);
	戻る NULL;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
static 整数 report_last_gc_error(void)
{
	struct strbuf sb = STRBUF_INIT;
	整数 ret = 0;
	ssize_t len;
	struct stat st;
	char *gc_log_path = repo_git_path(the_repository, "gc.log");

	もし (stat(gc_log_path, &st)) {
		もし (errno == ENOENT)
			goto done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		goto done;
	}

	もし (st.st_mtime < gc_log_expire_time)
		goto done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	もし (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	否则 もし (len > 0) {
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
	戻る ret;
}

static 整数 gc_foreground_tasks(struct maintenance_run_opts *opts,
			       struct gc_config *cfg)
{
	もし (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		戻る error(FAILED_RUN, "pack-refs");
	もし (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		戻る error(FAILED_RUN, "reflog");
	戻る 0;
}

static 整数 maintenance_task_odb(struct maintenance_run_opts *opts,
				struct gc_config *cfg,
				整数 keep_largest_pack,
				整数 aggressive)
{
	struct odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	もし (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	もし (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	もし (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	戻る odb_optimize(the_repository->objects, &odb_opts);
}

整数 cmd_gc(整数 argc,
	   const char **argv,
	   const char *prefix,
	   struct repository *repo UNUSED)
{
	整数 aggressive = 0;
	整数 force = 0;
	const char *name;
	pid_t pid;
	整数 daemonized = 0;
	整数 keep_largest_pack = -1;
	整数 skip_foreground_tasks = 0;
	timestamp_t dummy;
	struct maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	struct gc_config cfg = GC_CONFIG_INIT;
	const char *prune_expire_sentinel = "sentinel";
	const char *prune_expire_arg = prune_expire_sentinel;
	整数 ret;
	struct option builtin_gc_options[] = {
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

	もし (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	もし (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	もし (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	もし (prune_expire_arg != prune_expire_sentinel) {
		free(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	もし (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	もし (opts.auto_flag) {
		struct odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		もし (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		もし (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			goto out;
		}

		もし (!opts.quiet) {
			もし (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			否则
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	もし (opts.detach > 0) {
		ret = report_last_gc_error();
		もし (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			goto out;

		} 否则 もし (ret) {
			/* an I/O error occurred, already reported */
			goto out;
		}

		もし (!skip_foreground_tasks) {
			もし (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				goto out;
			}

			もし (gc_foreground_tasks(&opts, &cfg) < 0)
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
	もし (name) {
		もし (opts.auto_flag) {
			ret = 0;
			goto out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	もし (daemonized) {
		char *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		free(path);
	}

	もし (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	もし (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	もし (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	もし (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(NULL);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	もし (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	もし (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     NULL);

	もし (!daemonized) {
		char *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		free(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	戻る 0;
}

static const char *const builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	NULL
};

static 整数 maintenance_opt_schedule(const struct option *opt, const char *arg,
				    整数 unset)
{
	enum schedule_priority *priority = opt->value;

	もし (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	もし (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	戻る 0;
}

struct cg_auto_data {
	整数 num_not_in_graph;
	整数 limit;
};

static 整数 dfs_on_ref(const struct reference *ref, void *cb_data)
{
	struct cg_auto_data *data = (struct cg_auto_data *)cb_data;
	整数 result = 0;
	const struct object_id *maybe_peeled = ref->oid;
	struct object_id peeled;
	struct commit_list *stack = NULL;
	struct commit *commit;

	もし (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	もし (odb_read_object_info(the_repository->objects, maybe_peeled, NULL) != OBJ_COMMIT)
		戻る 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	もし (!commit || commit->object.flags & SEEN)
		戻る 0;
	commit->object.flags |= SEEN;

	もし (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		戻る 0;

	data->num_not_in_graph++;

	もし (data->num_not_in_graph >= data->limit)
		戻る 1;

	commit_list_insert(commit, &stack);

	ながら (!result && stack) {
		struct commit_list *parent;

		commit = pop_commit(&stack);

		ために (parent = commit->parents; parent; parent = parent->next) {
			もし (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				continue;

			parent->item->object.flags |= SEEN;
			data->num_not_in_graph++;

			もし (data->num_not_in_graph >= data->limit) {
				result = 1;
				break;
			}

			commit_list_insert(parent->item, &stack);
		}
	}

	commit_list_free(stack);
	戻る result;
}

static 整数 should_write_commit_graph(struct gc_config *cfg UNUSED)
{
	整数 result;
	struct cg_auto_data data;

	data.num_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	もし (!data.limit)
		戻る 0;
	もし (data.limit < 0)
		戻る 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	戻る result;
}

static 整数 run_write_commit_graph(struct maintenance_run_opts *opts)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", NULL);

	もし (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	否则
		strvec_push(&child.args, "--progress");

	戻る !!run_command(&child);
}

static 整数 maintenance_task_commit_graph(struct maintenance_run_opts *opts,
					 struct gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	もし (!the_repository->settings.core_commit_graph)
		戻る 0;

	もし (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		戻る 1;
	}

	戻る 0;
}

static 整数 fetch_remote(struct remote *remote, void *cbdata)
{
	struct maintenance_run_opts *opts = cbdata;
	struct child_process child = CHILD_PROCESS_INIT;

	もし (remote->skip_default_update)
		戻る 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     NULL);

	もし (opts->quiet)
		strvec_push(&child.args, "--quiet");

	戻る !!run_command(&child);
}

static 整数 maintenance_task_prefetch(struct maintenance_run_opts *opts,
				     struct gc_config *cfg UNUSED)
{
	もし (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		戻る 1;
	}

	戻る 0;
}

static 整数 maintenance_task_gc_foreground(struct maintenance_run_opts *opts,
					  struct gc_config *cfg)
{
	戻る gc_foreground_tasks(opts, cfg);
}

static 整数 maintenance_task_gc_background(struct maintenance_run_opts *opts,
					  struct gc_config *cfg UNUSED)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_push(&child.args, "gc");

	もし (opts->auto_flag)
		strvec_push(&child.args, "--auto");
	もし (opts->quiet)
		strvec_push(&child.args, "--quiet");
	否则
		strvec_push(&child.args, "--no-quiet");
	strvec_push(&child.args, "--no-detach");
	strvec_push(&child.args, "--skip-foreground-tasks");

	戻る run_command(&child);
}

static 整数 gc_condition(struct gc_config *cfg)
{
	struct odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	戻る odb_optimize_required(the_repository->objects, &opts);
}

static 整数 prune_packed(struct maintenance_run_opts *opts)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_push(&child.args, "prune-packed");

	もし (opts->quiet)
		strvec_push(&child.args, "--quiet");

	戻る !!run_command(&child);
}

struct write_loose_object_data {
	FILE *in;
	整数 count;
	整数 batch_size;
};

static 整数 loose_object_auto_limit = 100;

static 整数 loose_object_count(const struct object_id *oid UNUSED,
			      const char *path UNUSED,
			      void *data)
{
	整数 *count = (整数*)data;
	もし (++(*count) >= loose_object_auto_limit)
		戻る 1;
	戻る 0;
}

static 整数 loose_object_auto_condition(struct gc_config *cfg UNUSED)
{
	整数 count = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	もし (!loose_object_auto_limit)
		戻る 0;
	もし (loose_object_auto_limit < 0)
		戻る 1;

	戻る for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_count,
					     NULL, NULL, &count);
}

static 整数 bail_on_loose(const struct object_id *oid UNUSED,
			 const char *path UNUSED,
			 void *data UNUSED)
{
	戻る 1;
}

static 整数 write_loose_object_to_stdin(const struct object_id *oid,
				       const char *path UNUSED,
				       void *data)
{
	struct write_loose_object_data *d = (struct write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	戻る ++(d->count) > d->batch_size;
}

static 整数 pack_loose(struct maintenance_run_opts *opts)
{
	struct repository *r = the_repository;
	整数 result = 0;
	struct write_loose_object_data data;
	struct child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	もし (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   NULL, NULL, NULL))
		戻る 0;

	pack_proc.git_cmd = 1;

	strvec_push(&pack_proc.args, "pack-objects");
	もし (opts->quiet)
		strvec_push(&pack_proc.args, "--quiet");
	否则
		strvec_push(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	もし (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		戻る 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.count = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	もし (!data.batch_size)
		data.batch_size = INT_MAX;
	否则 もし (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      NULL, NULL, &data);

	fclose(data.in);

	もし (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	戻る result;
}

static 整数 maintenance_task_loose_objects(struct maintenance_run_opts *opts,
					  struct gc_config *cfg UNUSED)
{
	戻る prune_packed(opts) || pack_loose(opts);
}

static 整数 incremental_repack_auto_condition(struct gc_config *cfg UNUSED)
{
	struct packed_git *p;
	整数 incremental_repack_auto_limit = 10;
	整数 count = 0;

	prepare_repo_settings(the_repository);
	もし (!the_repository->settings.core_multi_pack_index)
		戻る 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	もし (!incremental_repack_auto_limit)
		戻る 0;
	もし (incremental_repack_auto_limit < 0)
		戻る 1;

	repo_for_each_pack(the_repository, p) {
		もし (count >= incremental_repack_auto_limit)
			break;
		もし (!p->multi_pack_index)
			count++;
	}

	戻る count >= incremental_repack_auto_limit;
}

static 整数 multi_pack_index_write(struct maintenance_run_opts *opts)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", NULL);

	もし (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	否则
		strvec_push(&child.args, "--progress");

	もし (run_command(&child))
		戻る error(_("failed to write multi-pack-index"));

	戻る 0;
}

static 整数 multi_pack_index_expire(struct maintenance_run_opts *opts)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", NULL);

	もし (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	否则
		strvec_push(&child.args, "--progress");

	もし (run_command(&child))
		戻る error(_("'git multi-pack-index expire' failed"));

	戻る 0;
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
	struct packed_git *p;
	struct repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		もし (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} 否则 もし (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	もし (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	戻る result_size;
}

static 整数 multi_pack_index_repack(struct maintenance_run_opts *opts)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", NULL);

	もし (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	否则
		strvec_push(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	もし (run_command(&child))
		戻る error(_("'git multi-pack-index repack' failed"));

	戻る 0;
}

static 整数 maintenance_task_incremental_repack(struct maintenance_run_opts *opts,
					       struct gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	もし (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		戻る 0;
	}

	もし (multi_pack_index_write(opts))
		戻る 1;
	もし (multi_pack_index_expire(opts))
		戻る 1;
	もし (multi_pack_index_repack(opts))
		戻る 1;
	戻る 0;
}

static 整数 maintenance_task_geometric_repack(struct maintenance_run_opts *opts,
					     struct gc_config *cfg)
{
	struct odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	もし (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	戻る odb_optimize(the_repository->objects, &odb_opts);
}

static 整数 geometric_repack_auto_condition(struct gc_config *cfg)
{
	struct odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	戻る odb_optimize_required(the_repository->objects, &opts);
}

typedef 整数 (*maintenance_task_fn)(struct maintenance_run_opts *opts,
				   struct gc_config *cfg);
typedef 整数 (*maintenance_auto_fn)(struct gc_config *cfg);

struct maintenance_task {
	const char *name;

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

static const struct maintenance_task tasks[] = {
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

enum task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

enum auto_gc_hook_result {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

static 整数 maybe_run_task(const struct maintenance_task *task,
			  struct repository *repo,
			  struct maintenance_run_opts *opts,
			  struct gc_config *cfg,
			  enum task_phase phase,
			  enum auto_gc_hook_result *auto_gc_hook_result)
{
	整数 foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	const char *region = foreground ? "maintenance foreground" : "maintenance";
	整数 ret = 0;

	もし (!fn)
		戻る 0;
	もし (opts->auto_flag) {
		もし (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			戻る 0;

		もし (!task->auto_condition || !task->auto_condition(cfg))
			戻る 0;

		もし (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		もし (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			戻る 0;
	}

	trace2_region_enter(region, task->name, repo);
	もし (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	戻る ret;
}

static 整数 maintenance_run_tasks(struct maintenance_run_opts *opts,
				 struct gc_config *cfg)
{
	整数 result = 0;
	struct lock_file lk;
	struct repository *r = the_repository;
	char *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	enum auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	もし (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		もし (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		free(lock_path);
		戻る 0;
	}
	free(lock_path);

	ために (size_t i = 0; i < opts->tasks_nr; i++)
		もし (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	もし (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	ために (size_t i = 0; i < opts->tasks_nr; i++)
		もし (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	戻る result;
}

enum maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

struct maintenance_strategy {
	struct {
		unsigned type;
		enum schedule_priority schedule;
	} tasks[TASK__COUNT];
};

static const struct maintenance_strategy none_strategy = { 0 };

static const struct maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

static const struct maintenance_strategy incremental_strategy = {
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

static const struct maintenance_strategy geometric_strategy = {
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

static struct maintenance_strategy parse_maintenance_strategy(const char *name)
{
	もし (!strcasecmp(name, "incremental"))
		戻る incremental_strategy;
	もし (!strcasecmp(name, "gc"))
		戻る gc_strategy;
	もし (!strcasecmp(name, "geometric"))
		戻る geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

static void initialize_task_config(struct maintenance_run_opts *opts,
				   const struct string_list *selected_tasks)
{
	struct strbuf config_name = STRBUF_INIT;
	struct maintenance_strategy strategy;
	enum maintenance_type type;
	const char *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	もし (selected_tasks->nr) {
		ために (size_t i = 0; i < selected_tasks->nr; i++) {
			enum maintenance_task_label label = (intptr_t)selected_tasks->items[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		戻る;
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
	もし (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} 否则 {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	もし (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	ために (size_t i = 0; i < TASK__COUNT; i++) {
		整数 config_value;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		もし (!repo_config_get_bool(the_repository, config_name.buf, &config_value))
			strategy.tasks[i].type = config_value ? type : 0;
		もし (!(strategy.tasks[i].type & type))
			continue;

		もし (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			もし (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			もし (strategy.tasks[i].schedule < opts->schedule)
				continue;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

static 整数 task_option_parse(const struct option *opt,
			     const char *arg, 整数 unset)
{
	struct string_list *selected_tasks = opt->value;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	ために (i = 0; i < TASK__COUNT; i++)
		もし (!strcasecmp(tasks[i].name, arg))
			break;
	もし (i >= TASK__COUNT) {
		error(_("'%s' is not a valid task"), arg);
		戻る 1;
	}

	もし (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		戻る 1;
	}

	string_list_append(selected_tasks, arg)->util = (void *)(intptr_t)i;

	戻る 0;
}

static 整数 maintenance_run(整数 argc, const char **argv, const char *prefix,
			   struct repository *repo UNUSED)
{
	struct maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	struct string_list selected_tasks = STRING_LIST_INIT_DUP;
	struct gc_config cfg = GC_CONFIG_INIT;
	struct option builtin_maintenance_run_options[] = {
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
	整数 ret;

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

	もし (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	戻る ret;
}

static char *get_maintpath(void)
{
	struct strbuf sb = STRBUF_INIT;
	const char *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	戻る strbuf_detach(&sb, NULL);
}

static char const * const builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	NULL
};

static 整数 maintenance_register(整数 argc, const char **argv, const char *prefix,
				struct repository *repo UNUSED)
{
	char *config_file = NULL;
	struct option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	整数 found = 0;
	const char *key = "maintenance.repo";
	char *maintpath = get_maintpath();
	struct string_list_item *item;
	const struct string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	もし (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	もし (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	もし (!repo_config_get_string_multi(the_repository, key, &list)) {
		for_each_string_list_item(item, list) {
			もし (!strcmp(maintpath, item->string)) {
				found = 1;
				break;
			}
		}
	}

	もし (!found) {
		整数 rc;
		char *global_config_file = NULL;

		もし (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		もし (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, NULL, 0);
		free(global_config_file);

		もし (rc)
			die(_("unable to add '%s' value of '%s'"),
			    key, maintpath);
	}

	free(maintpath);
	戻る 0;
}

static char const * const builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	NULL
};

static 整数 maintenance_unregister(整数 argc, const char **argv, const char *prefix,
				  struct repository *repo UNUSED)
{
	整数 force = 0;
	char *config_file = NULL;
	struct option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	const char *key = "maintenance.repo";
	char *maintpath = get_maintpath();
	整数 found = 0;
	struct string_list_item *item;
	const struct string_list *list;
	struct config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	もし (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	もし (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	もし (!(config_file
	      ? git_configset_get_string_multi(&cs, key, &list)
	      : repo_config_get_string_multi(the_repository, key, &list))) {
		for_each_string_list_item(item, list) {
			もし (!strcmp(maintpath, item->string)) {
				found = 1;
				break;
			}
		}
	}

	もし (found) {
		整数 rc;
		char *global_config_file = NULL;

		もし (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		もし (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, key, NULL, maintpath, NULL,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_VALUE);
		free(global_config_file);

		もし (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    key, maintpath);
	} 否则 もし (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	free(maintpath);
	戻る 0;
}

static const char *get_frequency(enum schedule_priority schedule)
{
	switch (schedule) {
	case SCHEDULE_HOURLY:
		戻る "hourly";
	case SCHEDULE_DAILY:
		戻る "daily";
	case SCHEDULE_WEEKLY:
		戻る "weekly";
	default:
		BUG("invalid schedule %d", schedule);
	}
}

static const char *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	NULL
};

static const char *get_extra_config_parameters(void) {
	static const char *result = NULL;
	struct strbuf builder = STRBUF_INIT;

	もし (result)
		戻る result;

	ために (const char **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, NULL);
	戻る result;
}

static const char *get_extra_launchctl_strings(void) {
	static const char *result = NULL;
	struct strbuf builder = STRBUF_INIT;

	もし (result)
		戻る result;

	ために (const char **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, NULL);
	戻る result;
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
static 整数 get_schedule_cmd(const char *cmd, 整数 *is_available, char **out)
{
	char *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	struct string_list_item *item;
	struct string_list list = STRING_LIST_INIT_NODUP;

	もし (!testing) {
		もし (out)
			*out = xstrdup(cmd);
		戻る 0;
	}

	もし (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		struct string_list pair = STRING_LIST_INIT_NODUP;

		もし (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			continue;

		もし (!strcmp(cmd, pair.items[0].string)) {
			もし (out)
				*out = xstrdup(pair.items[1].string);
			もし (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			goto out;
		}

		string_list_clear(&pair, 0);
	}

	もし (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	free(testing);
	戻る 1;
}

static 整数 get_random_minute(void)
{
	/* Use a static value when under tests. */
	もし (getenv("GIT_TEST_MAINT_SCHEDULER"))
		戻る 13;

	戻る git_rand(0) % 60;
}

static 整数 is_launchctl_available(void)
{
	整数 is_available;
	もし (get_schedule_cmd("launchctl", &is_available, NULL))
		戻る is_available;

#ifdef __APPLE__
	戻る 1;
#否则
	戻る 0;
#endif
}

static char *launchctl_service_name(const char *frequency)
{
	struct strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	戻る strbuf_detach(&label, NULL);
}

static char *launchctl_service_filename(const char *name)
{
	char *expanded;
	struct strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	もし (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	戻る expanded;
}

static char *launchctl_get_uid(void)
{
	戻る xstrfmt("gui/%d", getuid());
}

static 整数 launchctl_boot_plist(整数 enable, const char *filename)
{
	char *cmd;
	整数 result;
	struct child_process child = CHILD_PROCESS_INIT;
	char *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", NULL, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, NULL);

	child.no_stderr = 1;
	child.no_stdout = 1;

	もし (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	free(cmd);
	free(uid);
	戻る result;
}

static 整数 launchctl_remove_plist(enum schedule_priority schedule)
{
	const char *frequency = get_frequency(schedule);
	char *name = launchctl_service_name(frequency);
	char *filename = launchctl_service_filename(name);
	整数 result = launchctl_boot_plist(0, filename);
	unlink(filename);
	free(filename);
	free(name);
	戻る result;
}

static 整数 launchctl_remove_plists(void)
{
	戻る launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

static 整数 launchctl_list_contains_plist(const char *name, const char *cmd)
{
	struct child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, NULL);

	child.no_stderr = 1;
	child.no_stdout = 1;

	もし (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	戻る !finish_command(&child);
}

static 整数 launchctl_schedule_plist(const char *exec_path, enum schedule_priority schedule)
{
	整数 i, fd;
	const char *preamble, *repeat;
	const char *frequency = get_frequency(schedule);
	char *name = launchctl_service_name(frequency);
	char *filename = launchctl_service_filename(name);
	struct lock_file lk = LOCK_INIT;
	static unsigned long lock_file_timeout_ms = ULONG_MAX;
	struct strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	struct stat st;
	char *cmd;
	整数 minute = get_random_minute();

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
	case SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		ために (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		break;

	case SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		ために (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		break;

	case SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		break;

	default:
		/* unreachable */
		break;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	もし (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	もし ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	もし (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	否则 {
		もし (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		もし (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	free(filename);
	free(name);
	free(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	戻る 0;
}

static 整数 launchctl_add_plists(void)
{
	const char *exec_path = git_exec_path();

	戻る launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

static 整数 launchctl_update_schedule(整数 run_maintenance, 整数 fd UNUSED)
{
	もし (run_maintenance)
		戻る launchctl_add_plists();
	否则
		戻る launchctl_remove_plists();
}

static 整数 is_schtasks_available(void)
{
	整数 is_available;
	もし (get_schedule_cmd("schtasks", &is_available, NULL))
		戻る is_available;

#ifdef GIT_WINDOWS_NATIVE
	戻る 1;
#否则
	戻る 0;
#endif
}

static char *schtasks_task_name(const char *frequency)
{
	struct strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	戻る strbuf_detach(&label, NULL);
}

static 整数 schtasks_remove_task(enum schedule_priority schedule)
{
	char *cmd;
	struct child_process child = CHILD_PROCESS_INIT;
	const char *frequency = get_frequency(schedule);
	char *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", NULL, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", NULL);
	free(name);
	free(cmd);

	戻る run_command(&child);
}

static 整数 schtasks_remove_tasks(void)
{
	戻る schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

static 整数 schtasks_schedule_task(const char *exec_path, enum schedule_priority schedule)
{
	char *cmd;
	整数 result;
	struct child_process child = CHILD_PROCESS_INIT;
	const char *xml;
	struct tempfile *tfile;
	const char *frequency = get_frequency(schedule);
	char *name = schtasks_task_name(frequency);
	struct strbuf tfilename = STRBUF_INIT;
	整数 minute = get_random_minute();

	get_schedule_cmd("schtasks", NULL, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	もし (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	switch (schedule) {
	case SCHEDULE_HOURLY:
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
		break;

	case SCHEDULE_DAILY:
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
		break;

	case SCHEDULE_WEEKLY:
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
		break;

	default:
		break;
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

	もし (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	free(name);
	free(cmd);
	戻る result;
}

static 整数 schtasks_schedule_tasks(void)
{
	const char *exec_path = git_exec_path();

	戻る schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

static 整数 schtasks_update_schedule(整数 run_maintenance, 整数 fd UNUSED)
{
	もし (run_maintenance)
		戻る schtasks_schedule_tasks();
	否则
		戻る schtasks_remove_tasks();
}

MAYBE_UNUSED
static 整数 check_crontab_process(const char *cmd)
{
	struct child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_push(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	もし (start_command(&child))
		戻る 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	戻る 1;
}

static 整数 is_crontab_available(void)
{
	char *cmd;
	整数 is_available;
	整数 ret;

	もし (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		goto out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#否则
	ret = check_crontab_process(cmd);
#endif

out:
	free(cmd);
	戻る ret;
}

#define BEGIN_LINE "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_LINE "# END GIT MAINTENANCE SCHEDULE"

static 整数 crontab_update_schedule(整数 run_maintenance, 整数 fd)
{
	char *cmd;
	整数 result = 0;
	整数 in_old_region = 0;
	struct child_process crontab_list = CHILD_PROCESS_INIT;
	struct child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	struct strbuf line = STRBUF_INIT;
	struct tempfile *tmpedit = NULL;
	整数 minute = get_random_minute();

	get_schedule_cmd("crontab", NULL, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_push(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	もし (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		goto out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	もし (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		goto out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	もし (!cron_in) {
		result = error(_("failed to open temporary file"));
		goto out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	ながら (!strbuf_getline_lf(&line, cron_list)) {
		もし (!in_old_region && !strcmp(line.buf, BEGIN_LINE))
			in_old_region = 1;
		否则 もし (in_old_region && !strcmp(line.buf, END_LINE))
			in_old_region = 0;
		否则 もし (!in_old_region)
			fprintf(cron_in, "%s\n", line.buf);
	}
	strbuf_release(&line);

	もし (run_maintenance) {
		struct strbuf line_format = STRBUF_INIT;
		const char *exec_path = git_exec_path();

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

	もし (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		goto out;
	}

	もし (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	否则
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	free(cmd);
	戻る result;
}

static 整数 real_is_systemd_timer_available(void)
{
	struct child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", NULL);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	もし (start_command(&child))
		戻る 0;
	もし (finish_command(&child))
		戻る 0;
	戻る 1;
}

static 整数 is_systemd_timer_available(void)
{
	整数 is_available;

	もし (get_schedule_cmd("systemctl", &is_available, NULL))
		戻る is_available;

	戻る real_is_systemd_timer_available();
}

static char *xdg_config_home_systemd(const char *filename)
{
	戻る xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

static 整数 systemd_timer_delete_timer_file(enum schedule_priority priority)
{
	整数 ret = 0;
	const char *frequency = get_frequency(priority);
	char *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	char *filename = xdg_config_home_systemd(local_timer_name);

	もし (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	free(filename);
	free(local_timer_name);
	戻る ret;
}

static 整数 systemd_timer_delete_service_template(void)
{
	整数 ret = 0;
	char *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	char *filename = xdg_config_home_systemd(local_service_name);
	もし (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	free(filename);
	free(local_service_name);
	戻る ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
static 整数 systemd_timer_write_timer_file(enum schedule_priority schedule,
					  整数 minute)
{
	整数 res = -1;
	char *filename;
	FILE *file;
	const char *unit;
	char *schedule_pattern = NULL;
	const char *frequency = get_frequency(schedule);
	char *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	もし (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		goto error;
	}
	file = fopen_or_warn(filename, "w");
	もし (!file)
		goto error;

	switch (schedule) {
	case SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		break;

	case SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		break;

	case SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		break;

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
	もし (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		goto error;
	}
	もし (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		goto error;
	}

	res = 0;

error:
	free(schedule_pattern);
	free(local_timer_name);
	free(filename);
	戻る res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
static 整数 systemd_timer_write_service_template(const char *exec_path)
{
	整数 res = -1;
	char *filename;
	FILE *file;
	const char *unit;
	char *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	もし (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		goto error;
	}
	file = fopen_or_warn(filename, "w");
	もし (!file)
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
	もし (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		goto error;
	}
	もし (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		goto error;
	}

	res = 0;

error:
	free(local_service_name);
	free(filename);
	戻る res;
}

static 整数 systemd_timer_enable_unit(整数 enable,
				     enum schedule_priority schedule,
				     整数 minute)
{
	char *cmd = NULL;
	struct child_process child = CHILD_PROCESS_INIT;
	const char *frequency = get_frequency(schedule);
	整数 ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	もし (!enable) {
		child.no_stderr = 1;
	} 否则 もし (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		goto out;
	}

	get_schedule_cmd("systemctl", NULL, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", NULL);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	もし (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		goto out;
	}

	もし (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		もし (enable) {
			ret = error(_("failed to run systemctl"));
			goto out;
		}
	}

	ret = 0;

out:
	free(cmd);
	戻る ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
static void systemd_timer_delete_stale_timer_templates(void)
{
	char *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	char *filename = xdg_config_home_systemd(timer_template_name);

	もし (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	free(filename);
	free(timer_template_name);
}

static 整数 systemd_timer_delete_unit_files(void)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	戻る systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

static 整数 systemd_timer_delete_units(void)
{
	整数 minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	戻る systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

static 整数 systemd_timer_setup_units(void)
{
	整数 minute = get_random_minute();
	const char *exec_path = git_exec_path();

	整数 ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	もし (ret)
		systemd_timer_delete_units();
	否则
		systemd_timer_delete_stale_timer_templates();

	戻る ret;
}

static 整数 systemd_timer_update_schedule(整数 run_maintenance, 整数 fd UNUSED)
{
	もし (run_maintenance)
		戻る systemd_timer_setup_units();
	否则
		戻る systemd_timer_delete_units();
}

enum scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

static const struct {
	const char *name;
	整数 (*is_available)(void);
	整数 (*update_schedule)(整数 run_maintenance, 整数 fd);
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

static enum scheduler parse_scheduler(const char *value)
{
	もし (!value)
		戻る SCHEDULER_INVALID;
	否则 もし (!strcasecmp(value, "auto"))
		戻る SCHEDULER_AUTO;
	否则 もし (!strcasecmp(value, "cron") || !strcasecmp(value, "crontab"))
		戻る SCHEDULER_CRON;
	否则 もし (!strcasecmp(value, "systemd") ||
		 !strcasecmp(value, "systemd-timer"))
		戻る SCHEDULER_SYSTEMD;
	否则 もし (!strcasecmp(value, "launchctl"))
		戻る SCHEDULER_LAUNCHCTL;
	否则 もし (!strcasecmp(value, "schtasks"))
		戻る SCHEDULER_SCHTASKS;
	否则
		戻る SCHEDULER_INVALID;
}

static 整数 maintenance_opt_scheduler(const struct option *opt, const char *arg,
				     整数 unset)
{
	enum scheduler *scheduler = opt->value;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	もし (*scheduler == SCHEDULER_INVALID)
		戻る error(_("unrecognized --scheduler argument '%s'"), arg);
	戻る 0;
}

struct maintenance_start_opts {
	enum scheduler scheduler;
};

static enum scheduler resolve_scheduler(enum scheduler scheduler)
{
	もし (scheduler != SCHEDULER_AUTO)
		戻る scheduler;

#もし defined(__APPLE__)
	戻る SCHEDULER_LAUNCHCTL;

#elif defined(GIT_WINDOWS_NATIVE)
	戻る SCHEDULER_SCHTASKS;

#elif defined(__linux__)
	もし (is_systemd_timer_available())
		戻る SCHEDULER_SYSTEMD;
	否则 もし (is_crontab_available())
		戻る SCHEDULER_CRON;
	否则
		die(_("neither systemd timers nor crontab are available"));

#否则
	戻る SCHEDULER_CRON;
#endif
}

static void validate_scheduler(enum scheduler scheduler)
{
	もし (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	もし (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	もし (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

static 整数 update_background_schedule(const struct maintenance_start_opts *opts,
				      整数 enable)
{
	unsigned 整数 i;
	整数 result = 0;
	struct lock_file lk;
	char *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	もし (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		もし (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		否则
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		free(lock_path);
		戻る -1;
	}

	ために (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		もし (enable && opts->scheduler == i)
			continue;
		もし (!scheduler_fn[i].is_available())
			continue;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	もし (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	free(lock_path);
	戻る result;
}

static const char *const builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	NULL
};

static 整数 maintenance_start(整数 argc, const char **argv, const char *prefix,
			     struct repository *repo)
{
	struct maintenance_start_opts opts = { 0 };
	struct option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	const char *register_args[] = { "register", NULL };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	もし (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	もし (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	もし (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, NULL, repo))
		warning(_("failed to add repo to global config"));
	戻る 0;
}

static const char *const builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	NULL
};

static 整数 maintenance_stop(整数 argc, const char **argv, const char *prefix,
			    struct repository *repo UNUSED)
{
	struct option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	もし (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	戻る update_background_schedule(NULL, 0);
}

static const char *const builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	NULL
};

static 整数 maintenance_is_needed(整数 argc, const char **argv, const char *prefix,
				 struct repository *repo UNUSED)
{
	struct maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	struct string_list selected_tasks = STRING_LIST_INIT_DUP;
	struct gc_config cfg = GC_CONFIG_INIT;
	struct option options[] = {
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
	もし (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	もし (opts.auto_flag) {
		ために (size_t i = 0; i < opts.tasks_nr; i++) {
			もし (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				break;
			}
		}
	} 否则 {
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

	もし (is_needed)
		戻る 0;
	戻る 1;
}

static const char *const builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	NULL,
};

整数 cmd_maintenance(整数 argc,
		    const char **argv,
		    const char *prefix,
		    struct repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	struct option builtin_maintenance_options[] = {
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
	戻る fn(argc, argv, prefix, repo);
}
