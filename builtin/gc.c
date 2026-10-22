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
	số_nguyên i;
	cho (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.items[i].string);
	string_list_clear(&pack_garbage, 0);
}

static void report_pack_garbage(unsigned seen_bits, const char *path)
{
	nếu (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

static void process_log_file(void)
{
	struct stat st;
	nếu (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		số_nguyên saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} không_thì nếu (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} không_thì {
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

static số_nguyên gc_config_is_timestamp_never(const char *var)
{
	const char *value;
	timestamp_t expire;

	nếu (!repo_config_get_value(the_repository, var, &value) && value) {
		nếu (parse_expiry_date(value, &expire))
			die(_("failed to parse '%s' value '%s'"), var, value);
		trả_về expire == 0;
	}
	trả_về 0;
}

struct gc_config {
	số_nguyên pack_refs;
	số_nguyên prune_reflogs;
	số_nguyên cruft_packs;
	unsigned long max_cruft_size;
	số_nguyên aggressive_depth;
	số_nguyên aggressive_window;
	số_nguyên detach_auto;
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

	nếu (!repo_config_get_value(the_repository, "gc.packrefs", &value)) {
		nếu (value && !strcmp(value, "notbare"))
			cfg->pack_refs = -1;
		không_thì
			cfg->pack_refs = git_config_bool("gc.packrefs", value);
	}

	nếu (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	nếu (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		free(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	nếu (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		free(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	nếu (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
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
	nếu (!value)
		trả_về SCHEDULE_NONE;
	nếu (!strcasecmp(value, "hourly"))
		trả_về SCHEDULE_HOURLY;
	nếu (!strcasecmp(value, "daily"))
		trả_về SCHEDULE_DAILY;
	nếu (!strcasecmp(value, "weekly"))
		trả_về SCHEDULE_WEEKLY;
	trả_về SCHEDULE_NONE;
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
	số_nguyên auto_flag;
	số_nguyên detach;
	số_nguyên quiet;
	enum schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

static void maintenance_run_opts_release(struct maintenance_run_opts *opts)
{
	free(opts->tasks);
}

static số_nguyên pack_refs_condition(UNUSED struct gc_config *cfg)
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

	nếu (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		trả_về 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	trả_về required;
}

static số_nguyên maintenance_task_pack_refs(struct maintenance_run_opts *opts,
				      UNUSED struct gc_config *cfg)
{
	struct child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", NULL);
	nếu (opts->auto_flag)
		strvec_push(&cmd.args, "--auto");

	trả_về run_command(&cmd);
}

struct count_reflog_entries_data {
	struct expire_reflog_policy_cb policy;
	size_t count;
	size_t limit;
};

static số_nguyên count_reflog_entries(const char *refname UNUSED,
				struct object_id *old_oid, struct object_id *new_oid,
				const char *committer, timestamp_t timestamp,
				số_nguyên tz, const char *msg, void *cb_data)
{
	struct count_reflog_entries_data *data = cb_data;
	nếu (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, msg, &data->policy))
		data->count++;
	trả_về data->count >= data->limit;
}

static số_nguyên reflog_expire_condition(struct gc_config *cfg UNUSED)
{
	timestamp_t now = time(NULL);
	struct count_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	số_nguyên limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	nếu (!limit)
		trả_về 0;
	nếu (limit < 0)
		trả_về 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 count_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	trả_về data.count >= data.limit;
}

static số_nguyên maintenance_task_reflog_expire(struct maintenance_run_opts *opts UNUSED,
					  struct gc_config *cfg UNUSED)
{
	struct child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", NULL);
	trả_về run_command(&cmd);
}

static số_nguyên maintenance_task_worktree_prune(struct maintenance_run_opts *opts UNUSED,
					   struct gc_config *cfg)
{
	struct child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", NULL);
	strvec_push(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	trả_về run_command(&prune_worktrees_cmd);
}

static số_nguyên worktree_prune_condition(struct gc_config *cfg)
{
	struct strbuf buf = STRBUF_INIT;
	số_nguyên should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	struct dirent *d;
	DIR *dir = NULL;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	nếu (limit <= 0) {
		should_prune = limit < 0;
		goto out;
	}

	nếu (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		goto out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	nếu (!dir)
		goto out;

	trong_khi (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		char *wtpath;
		strbuf_reset(&buf);
		nếu (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		free(wtpath);
	}

	should_prune = !limit;

out:
	nếu (dir)
		closedir(dir);
	strbuf_release(&buf);
	trả_về should_prune;
}

static số_nguyên maintenance_task_rerere_gc(struct maintenance_run_opts *opts UNUSED,
				      struct gc_config *cfg UNUSED)
{
	struct child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", NULL);
	trả_về run_command(&rerere_cmd);
}

static số_nguyên rerere_gc_condition(struct gc_config *cfg UNUSED)
{
	số_nguyên limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	nếu (!limit)
		trả_về 0; /* never prune */
	nếu (limit < 0)
		trả_về 1; /* always prune */

	trả_về rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
static const char *lock_repo_for_gc(số_nguyên force, pid_t* ret_pid)
{
	struct lock_file lock = LOCK_INIT;
	char my_host[HOST_NAME_MAX + 1];
	struct strbuf sb = STRBUF_INIT;
	struct stat st;
	uintmax_t pid;
	FILE *fp;
	số_nguyên fd;
	char *pidfile_path;

	nếu (is_tempfile_active(pidfile))
		/* already locked */
		trả_về NULL;

	nếu (xgethostname(my_host, sizeof(my_host)))
		xsnprintf(my_host, sizeof(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	nếu (!force) {
		static char locking_host[HOST_NAME_MAX + 1];
		static char *scan_fmt;
		số_nguyên should_exit;

		nếu (!scan_fmt)
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
		nếu (fp)
			fclose(fp);
		nếu (should_exit) {
			nếu (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			free(pidfile_path);
			trả_về locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	free(pidfile_path);
	trả_về NULL;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
static số_nguyên report_last_gc_error(void)
{
	struct strbuf sb = STRBUF_INIT;
	số_nguyên ret = 0;
	ssize_t len;
	struct stat st;
	char *gc_log_path = repo_git_path(the_repository, "gc.log");

	nếu (stat(gc_log_path, &st)) {
		nếu (errno == ENOENT)
			goto done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		goto done;
	}

	nếu (st.st_mtime < gc_log_expire_time)
		goto done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	nếu (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	không_thì nếu (len > 0) {
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
	trả_về ret;
}

static số_nguyên gc_foreground_tasks(struct maintenance_run_opts *opts,
			       struct gc_config *cfg)
{
	nếu (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		trả_về error(FAILED_RUN, "pack-refs");
	nếu (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		trả_về error(FAILED_RUN, "reflog");
	trả_về 0;
}

static số_nguyên maintenance_task_odb(struct maintenance_run_opts *opts,
				struct gc_config *cfg,
				số_nguyên keep_largest_pack,
				số_nguyên aggressive)
{
	struct odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	nếu (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	nếu (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	nếu (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	trả_về odb_optimize(the_repository->objects, &odb_opts);
}

số_nguyên cmd_gc(số_nguyên argc,
	   const char **argv,
	   const char *prefix,
	   struct repository *repo UNUSED)
{
	số_nguyên aggressive = 0;
	số_nguyên force = 0;
	const char *name;
	pid_t pid;
	số_nguyên daemonized = 0;
	số_nguyên keep_largest_pack = -1;
	số_nguyên skip_foreground_tasks = 0;
	timestamp_t dummy;
	struct maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	struct gc_config cfg = GC_CONFIG_INIT;
	const char *prune_expire_sentinel = "sentinel";
	const char *prune_expire_arg = prune_expire_sentinel;
	số_nguyên ret;
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

	nếu (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	nếu (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	nếu (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	nếu (prune_expire_arg != prune_expire_sentinel) {
		free(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	nếu (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	nếu (opts.auto_flag) {
		struct odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		nếu (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		nếu (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			goto out;
		}

		nếu (!opts.quiet) {
			nếu (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			không_thì
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	nếu (opts.detach > 0) {
		ret = report_last_gc_error();
		nếu (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			goto out;

		} không_thì nếu (ret) {
			/* an I/O error occurred, already reported */
			goto out;
		}

		nếu (!skip_foreground_tasks) {
			nếu (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				goto out;
			}

			nếu (gc_foreground_tasks(&opts, &cfg) < 0)
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
	nếu (name) {
		nếu (opts.auto_flag) {
			ret = 0;
			goto out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	nếu (daemonized) {
		char *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		free(path);
	}

	nếu (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	nếu (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	nếu (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	nếu (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(NULL);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	nếu (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	nếu (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     NULL);

	nếu (!daemonized) {
		char *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		free(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	trả_về 0;
}

static const char *const builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	NULL
};

static số_nguyên maintenance_opt_schedule(const struct option *opt, const char *arg,
				    số_nguyên unset)
{
	enum schedule_priority *priority = opt->value;

	nếu (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	nếu (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	trả_về 0;
}

struct cg_auto_data {
	số_nguyên num_not_in_graph;
	số_nguyên limit;
};

static số_nguyên dfs_on_ref(const struct reference *ref, void *cb_data)
{
	struct cg_auto_data *data = (struct cg_auto_data *)cb_data;
	số_nguyên result = 0;
	const struct object_id *maybe_peeled = ref->oid;
	struct object_id peeled;
	struct commit_list *stack = NULL;
	struct commit *commit;

	nếu (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	nếu (odb_read_object_info(the_repository->objects, maybe_peeled, NULL) != OBJ_COMMIT)
		trả_về 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	nếu (!commit || commit->object.flags & SEEN)
		trả_về 0;
	commit->object.flags |= SEEN;

	nếu (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		trả_về 0;

	data->num_not_in_graph++;

	nếu (data->num_not_in_graph >= data->limit)
		trả_về 1;

	commit_list_insert(commit, &stack);

	trong_khi (!result && stack) {
		struct commit_list *parent;

		commit = pop_commit(&stack);

		cho (parent = commit->parents; parent; parent = parent->next) {
			nếu (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				continue;

			parent->item->object.flags |= SEEN;
			data->num_not_in_graph++;

			nếu (data->num_not_in_graph >= data->limit) {
				result = 1;
				break;
			}

			commit_list_insert(parent->item, &stack);
		}
	}

	commit_list_free(stack);
	trả_về result;
}

static số_nguyên should_write_commit_graph(struct gc_config *cfg UNUSED)
{
	số_nguyên result;
	struct cg_auto_data data;

	data.num_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	nếu (!data.limit)
		trả_về 0;
	nếu (data.limit < 0)
		trả_về 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	trả_về result;
}

static số_nguyên run_write_commit_graph(struct maintenance_run_opts *opts)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", NULL);

	nếu (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	không_thì
		strvec_push(&child.args, "--progress");

	trả_về !!run_command(&child);
}

static số_nguyên maintenance_task_commit_graph(struct maintenance_run_opts *opts,
					 struct gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	nếu (!the_repository->settings.core_commit_graph)
		trả_về 0;

	nếu (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		trả_về 1;
	}

	trả_về 0;
}

static số_nguyên fetch_remote(struct remote *remote, void *cbdata)
{
	struct maintenance_run_opts *opts = cbdata;
	struct child_process child = CHILD_PROCESS_INIT;

	nếu (remote->skip_default_update)
		trả_về 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     NULL);

	nếu (opts->quiet)
		strvec_push(&child.args, "--quiet");

	trả_về !!run_command(&child);
}

static số_nguyên maintenance_task_prefetch(struct maintenance_run_opts *opts,
				     struct gc_config *cfg UNUSED)
{
	nếu (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		trả_về 1;
	}

	trả_về 0;
}

static số_nguyên maintenance_task_gc_foreground(struct maintenance_run_opts *opts,
					  struct gc_config *cfg)
{
	trả_về gc_foreground_tasks(opts, cfg);
}

static số_nguyên maintenance_task_gc_background(struct maintenance_run_opts *opts,
					  struct gc_config *cfg UNUSED)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_push(&child.args, "gc");

	nếu (opts->auto_flag)
		strvec_push(&child.args, "--auto");
	nếu (opts->quiet)
		strvec_push(&child.args, "--quiet");
	không_thì
		strvec_push(&child.args, "--no-quiet");
	strvec_push(&child.args, "--no-detach");
	strvec_push(&child.args, "--skip-foreground-tasks");

	trả_về run_command(&child);
}

static số_nguyên gc_condition(struct gc_config *cfg)
{
	struct odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	trả_về odb_optimize_required(the_repository->objects, &opts);
}

static số_nguyên prune_packed(struct maintenance_run_opts *opts)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_push(&child.args, "prune-packed");

	nếu (opts->quiet)
		strvec_push(&child.args, "--quiet");

	trả_về !!run_command(&child);
}

struct write_loose_object_data {
	FILE *in;
	số_nguyên count;
	số_nguyên batch_size;
};

static số_nguyên loose_object_auto_limit = 100;

static số_nguyên loose_object_count(const struct object_id *oid UNUSED,
			      const char *path UNUSED,
			      void *data)
{
	số_nguyên *count = (số_nguyên*)data;
	nếu (++(*count) >= loose_object_auto_limit)
		trả_về 1;
	trả_về 0;
}

static số_nguyên loose_object_auto_condition(struct gc_config *cfg UNUSED)
{
	số_nguyên count = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	nếu (!loose_object_auto_limit)
		trả_về 0;
	nếu (loose_object_auto_limit < 0)
		trả_về 1;

	trả_về for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_count,
					     NULL, NULL, &count);
}

static số_nguyên bail_on_loose(const struct object_id *oid UNUSED,
			 const char *path UNUSED,
			 void *data UNUSED)
{
	trả_về 1;
}

static số_nguyên write_loose_object_to_stdin(const struct object_id *oid,
				       const char *path UNUSED,
				       void *data)
{
	struct write_loose_object_data *d = (struct write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	trả_về ++(d->count) > d->batch_size;
}

static số_nguyên pack_loose(struct maintenance_run_opts *opts)
{
	struct repository *r = the_repository;
	số_nguyên result = 0;
	struct write_loose_object_data data;
	struct child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	nếu (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   NULL, NULL, NULL))
		trả_về 0;

	pack_proc.git_cmd = 1;

	strvec_push(&pack_proc.args, "pack-objects");
	nếu (opts->quiet)
		strvec_push(&pack_proc.args, "--quiet");
	không_thì
		strvec_push(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	nếu (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		trả_về 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.count = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	nếu (!data.batch_size)
		data.batch_size = INT_MAX;
	không_thì nếu (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      NULL, NULL, &data);

	fclose(data.in);

	nếu (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	trả_về result;
}

static số_nguyên maintenance_task_loose_objects(struct maintenance_run_opts *opts,
					  struct gc_config *cfg UNUSED)
{
	trả_về prune_packed(opts) || pack_loose(opts);
}

static số_nguyên incremental_repack_auto_condition(struct gc_config *cfg UNUSED)
{
	struct packed_git *p;
	số_nguyên incremental_repack_auto_limit = 10;
	số_nguyên count = 0;

	prepare_repo_settings(the_repository);
	nếu (!the_repository->settings.core_multi_pack_index)
		trả_về 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	nếu (!incremental_repack_auto_limit)
		trả_về 0;
	nếu (incremental_repack_auto_limit < 0)
		trả_về 1;

	repo_for_each_pack(the_repository, p) {
		nếu (count >= incremental_repack_auto_limit)
			break;
		nếu (!p->multi_pack_index)
			count++;
	}

	trả_về count >= incremental_repack_auto_limit;
}

static số_nguyên multi_pack_index_write(struct maintenance_run_opts *opts)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", NULL);

	nếu (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	không_thì
		strvec_push(&child.args, "--progress");

	nếu (run_command(&child))
		trả_về error(_("failed to write multi-pack-index"));

	trả_về 0;
}

static số_nguyên multi_pack_index_expire(struct maintenance_run_opts *opts)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", NULL);

	nếu (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	không_thì
		strvec_push(&child.args, "--progress");

	nếu (run_command(&child))
		trả_về error(_("'git multi-pack-index expire' failed"));

	trả_về 0;
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
		nếu (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} không_thì nếu (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	nếu (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	trả_về result_size;
}

static số_nguyên multi_pack_index_repack(struct maintenance_run_opts *opts)
{
	struct child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", NULL);

	nếu (opts->quiet)
		strvec_push(&child.args, "--no-progress");
	không_thì
		strvec_push(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	nếu (run_command(&child))
		trả_về error(_("'git multi-pack-index repack' failed"));

	trả_về 0;
}

static số_nguyên maintenance_task_incremental_repack(struct maintenance_run_opts *opts,
					       struct gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	nếu (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		trả_về 0;
	}

	nếu (multi_pack_index_write(opts))
		trả_về 1;
	nếu (multi_pack_index_expire(opts))
		trả_về 1;
	nếu (multi_pack_index_repack(opts))
		trả_về 1;
	trả_về 0;
}

static số_nguyên maintenance_task_geometric_repack(struct maintenance_run_opts *opts,
					     struct gc_config *cfg)
{
	struct odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	nếu (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	trả_về odb_optimize(the_repository->objects, &odb_opts);
}

static số_nguyên geometric_repack_auto_condition(struct gc_config *cfg)
{
	struct odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	trả_về odb_optimize_required(the_repository->objects, &opts);
}

typedef số_nguyên (*maintenance_task_fn)(struct maintenance_run_opts *opts,
				   struct gc_config *cfg);
typedef số_nguyên (*maintenance_auto_fn)(struct gc_config *cfg);

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

static số_nguyên maybe_run_task(const struct maintenance_task *task,
			  struct repository *repo,
			  struct maintenance_run_opts *opts,
			  struct gc_config *cfg,
			  enum task_phase phase,
			  enum auto_gc_hook_result *auto_gc_hook_result)
{
	số_nguyên foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	const char *region = foreground ? "maintenance foreground" : "maintenance";
	số_nguyên ret = 0;

	nếu (!fn)
		trả_về 0;
	nếu (opts->auto_flag) {
		nếu (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			trả_về 0;

		nếu (!task->auto_condition || !task->auto_condition(cfg))
			trả_về 0;

		nếu (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		nếu (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			trả_về 0;
	}

	trace2_region_enter(region, task->name, repo);
	nếu (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	trả_về ret;
}

static số_nguyên maintenance_run_tasks(struct maintenance_run_opts *opts,
				 struct gc_config *cfg)
{
	số_nguyên result = 0;
	struct lock_file lk;
	struct repository *r = the_repository;
	char *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	enum auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	nếu (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		nếu (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		free(lock_path);
		trả_về 0;
	}
	free(lock_path);

	cho (size_t i = 0; i < opts->tasks_nr; i++)
		nếu (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	nếu (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	cho (size_t i = 0; i < opts->tasks_nr; i++)
		nếu (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	trả_về result;
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
	nếu (!strcasecmp(name, "incremental"))
		trả_về incremental_strategy;
	nếu (!strcasecmp(name, "gc"))
		trả_về gc_strategy;
	nếu (!strcasecmp(name, "geometric"))
		trả_về geometric_strategy;
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
	nếu (selected_tasks->nr) {
		cho (size_t i = 0; i < selected_tasks->nr; i++) {
			enum maintenance_task_label label = (intptr_t)selected_tasks->items[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		trả_về;
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
	nếu (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} không_thì {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	nếu (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	cho (size_t i = 0; i < TASK__COUNT; i++) {
		số_nguyên config_value;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		nếu (!repo_config_get_bool(the_repository, config_name.buf, &config_value))
			strategy.tasks[i].type = config_value ? type : 0;
		nếu (!(strategy.tasks[i].type & type))
			continue;

		nếu (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			nếu (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			nếu (strategy.tasks[i].schedule < opts->schedule)
				continue;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

static số_nguyên task_option_parse(const struct option *opt,
			     const char *arg, số_nguyên unset)
{
	struct string_list *selected_tasks = opt->value;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	cho (i = 0; i < TASK__COUNT; i++)
		nếu (!strcasecmp(tasks[i].name, arg))
			break;
	nếu (i >= TASK__COUNT) {
		error(_("'%s' is not a valid task"), arg);
		trả_về 1;
	}

	nếu (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		trả_về 1;
	}

	string_list_append(selected_tasks, arg)->util = (void *)(intptr_t)i;

	trả_về 0;
}

static số_nguyên maintenance_run(số_nguyên argc, const char **argv, const char *prefix,
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
	số_nguyên ret;

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

	nếu (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	trả_về ret;
}

static char *get_maintpath(void)
{
	struct strbuf sb = STRBUF_INIT;
	const char *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	trả_về strbuf_detach(&sb, NULL);
}

static char const * const builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	NULL
};

static số_nguyên maintenance_register(số_nguyên argc, const char **argv, const char *prefix,
				struct repository *repo UNUSED)
{
	char *config_file = NULL;
	struct option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	số_nguyên found = 0;
	const char *key = "maintenance.repo";
	char *maintpath = get_maintpath();
	struct string_list_item *item;
	const struct string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	nếu (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	nếu (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	nếu (!repo_config_get_string_multi(the_repository, key, &list)) {
		for_each_string_list_item(item, list) {
			nếu (!strcmp(maintpath, item->string)) {
				found = 1;
				break;
			}
		}
	}

	nếu (!found) {
		số_nguyên rc;
		char *global_config_file = NULL;

		nếu (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		nếu (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, NULL, 0);
		free(global_config_file);

		nếu (rc)
			die(_("unable to add '%s' value of '%s'"),
			    key, maintpath);
	}

	free(maintpath);
	trả_về 0;
}

static char const * const builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	NULL
};

static số_nguyên maintenance_unregister(số_nguyên argc, const char **argv, const char *prefix,
				  struct repository *repo UNUSED)
{
	số_nguyên force = 0;
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
	số_nguyên found = 0;
	struct string_list_item *item;
	const struct string_list *list;
	struct config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	nếu (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	nếu (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	nếu (!(config_file
	      ? git_configset_get_string_multi(&cs, key, &list)
	      : repo_config_get_string_multi(the_repository, key, &list))) {
		for_each_string_list_item(item, list) {
			nếu (!strcmp(maintpath, item->string)) {
				found = 1;
				break;
			}
		}
	}

	nếu (found) {
		số_nguyên rc;
		char *global_config_file = NULL;

		nếu (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		nếu (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, key, NULL, maintpath, NULL,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_VALUE);
		free(global_config_file);

		nếu (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    key, maintpath);
	} không_thì nếu (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	free(maintpath);
	trả_về 0;
}

static const char *get_frequency(enum schedule_priority schedule)
{
	switch (schedule) {
	case SCHEDULE_HOURLY:
		trả_về "hourly";
	case SCHEDULE_DAILY:
		trả_về "daily";
	case SCHEDULE_WEEKLY:
		trả_về "weekly";
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

	nếu (result)
		trả_về result;

	cho (const char **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, NULL);
	trả_về result;
}

static const char *get_extra_launchctl_strings(void) {
	static const char *result = NULL;
	struct strbuf builder = STRBUF_INIT;

	nếu (result)
		trả_về result;

	cho (const char **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, NULL);
	trả_về result;
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
static số_nguyên get_schedule_cmd(const char *cmd, số_nguyên *is_available, char **out)
{
	char *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	struct string_list_item *item;
	struct string_list list = STRING_LIST_INIT_NODUP;

	nếu (!testing) {
		nếu (out)
			*out = xstrdup(cmd);
		trả_về 0;
	}

	nếu (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		struct string_list pair = STRING_LIST_INIT_NODUP;

		nếu (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			continue;

		nếu (!strcmp(cmd, pair.items[0].string)) {
			nếu (out)
				*out = xstrdup(pair.items[1].string);
			nếu (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			goto out;
		}

		string_list_clear(&pair, 0);
	}

	nếu (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	free(testing);
	trả_về 1;
}

static số_nguyên get_random_minute(void)
{
	/* Use a static value when under tests. */
	nếu (getenv("GIT_TEST_MAINT_SCHEDULER"))
		trả_về 13;

	trả_về git_rand(0) % 60;
}

static số_nguyên is_launchctl_available(void)
{
	số_nguyên is_available;
	nếu (get_schedule_cmd("launchctl", &is_available, NULL))
		trả_về is_available;

#ifdef __APPLE__
	trả_về 1;
#không_thì
	trả_về 0;
#endif
}

static char *launchctl_service_name(const char *frequency)
{
	struct strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	trả_về strbuf_detach(&label, NULL);
}

static char *launchctl_service_filename(const char *name)
{
	char *expanded;
	struct strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	nếu (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	trả_về expanded;
}

static char *launchctl_get_uid(void)
{
	trả_về xstrfmt("gui/%d", getuid());
}

static số_nguyên launchctl_boot_plist(số_nguyên enable, const char *filename)
{
	char *cmd;
	số_nguyên result;
	struct child_process child = CHILD_PROCESS_INIT;
	char *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", NULL, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, NULL);

	child.no_stderr = 1;
	child.no_stdout = 1;

	nếu (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	free(cmd);
	free(uid);
	trả_về result;
}

static số_nguyên launchctl_remove_plist(enum schedule_priority schedule)
{
	const char *frequency = get_frequency(schedule);
	char *name = launchctl_service_name(frequency);
	char *filename = launchctl_service_filename(name);
	số_nguyên result = launchctl_boot_plist(0, filename);
	unlink(filename);
	free(filename);
	free(name);
	trả_về result;
}

static số_nguyên launchctl_remove_plists(void)
{
	trả_về launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

static số_nguyên launchctl_list_contains_plist(const char *name, const char *cmd)
{
	struct child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, NULL);

	child.no_stderr = 1;
	child.no_stdout = 1;

	nếu (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	trả_về !finish_command(&child);
}

static số_nguyên launchctl_schedule_plist(const char *exec_path, enum schedule_priority schedule)
{
	số_nguyên i, fd;
	const char *preamble, *repeat;
	const char *frequency = get_frequency(schedule);
	char *name = launchctl_service_name(frequency);
	char *filename = launchctl_service_filename(name);
	struct lock_file lk = LOCK_INIT;
	static unsigned long lock_file_timeout_ms = ULONG_MAX;
	struct strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	struct stat st;
	char *cmd;
	số_nguyên minute = get_random_minute();

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
		cho (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		break;

	case SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		cho (i = 1; i <= 6; i++)
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

	nếu (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	nếu ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	nếu (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	không_thì {
		nếu (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		nếu (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	free(filename);
	free(name);
	free(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	trả_về 0;
}

static số_nguyên launchctl_add_plists(void)
{
	const char *exec_path = git_exec_path();

	trả_về launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

static số_nguyên launchctl_update_schedule(số_nguyên run_maintenance, số_nguyên fd UNUSED)
{
	nếu (run_maintenance)
		trả_về launchctl_add_plists();
	không_thì
		trả_về launchctl_remove_plists();
}

static số_nguyên is_schtasks_available(void)
{
	số_nguyên is_available;
	nếu (get_schedule_cmd("schtasks", &is_available, NULL))
		trả_về is_available;

#ifdef GIT_WINDOWS_NATIVE
	trả_về 1;
#không_thì
	trả_về 0;
#endif
}

static char *schtasks_task_name(const char *frequency)
{
	struct strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	trả_về strbuf_detach(&label, NULL);
}

static số_nguyên schtasks_remove_task(enum schedule_priority schedule)
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

	trả_về run_command(&child);
}

static số_nguyên schtasks_remove_tasks(void)
{
	trả_về schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

static số_nguyên schtasks_schedule_task(const char *exec_path, enum schedule_priority schedule)
{
	char *cmd;
	số_nguyên result;
	struct child_process child = CHILD_PROCESS_INIT;
	const char *xml;
	struct tempfile *tfile;
	const char *frequency = get_frequency(schedule);
	char *name = schtasks_task_name(frequency);
	struct strbuf tfilename = STRBUF_INIT;
	số_nguyên minute = get_random_minute();

	get_schedule_cmd("schtasks", NULL, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	nếu (!fdopen_tempfile(tfile, "w"))
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

	nếu (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	free(name);
	free(cmd);
	trả_về result;
}

static số_nguyên schtasks_schedule_tasks(void)
{
	const char *exec_path = git_exec_path();

	trả_về schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

static số_nguyên schtasks_update_schedule(số_nguyên run_maintenance, số_nguyên fd UNUSED)
{
	nếu (run_maintenance)
		trả_về schtasks_schedule_tasks();
	không_thì
		trả_về schtasks_remove_tasks();
}

MAYBE_UNUSED
static số_nguyên check_crontab_process(const char *cmd)
{
	struct child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_push(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	nếu (start_command(&child))
		trả_về 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	trả_về 1;
}

static số_nguyên is_crontab_available(void)
{
	char *cmd;
	số_nguyên is_available;
	số_nguyên ret;

	nếu (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		goto out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#không_thì
	ret = check_crontab_process(cmd);
#endif

out:
	free(cmd);
	trả_về ret;
}

#define BEGIN_LINE "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_LINE "# END GIT MAINTENANCE SCHEDULE"

static số_nguyên crontab_update_schedule(số_nguyên run_maintenance, số_nguyên fd)
{
	char *cmd;
	số_nguyên result = 0;
	số_nguyên in_old_region = 0;
	struct child_process crontab_list = CHILD_PROCESS_INIT;
	struct child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	struct strbuf line = STRBUF_INIT;
	struct tempfile *tmpedit = NULL;
	số_nguyên minute = get_random_minute();

	get_schedule_cmd("crontab", NULL, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_push(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	nếu (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		goto out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	nếu (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		goto out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	nếu (!cron_in) {
		result = error(_("failed to open temporary file"));
		goto out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	trong_khi (!strbuf_getline_lf(&line, cron_list)) {
		nếu (!in_old_region && !strcmp(line.buf, BEGIN_LINE))
			in_old_region = 1;
		không_thì nếu (in_old_region && !strcmp(line.buf, END_LINE))
			in_old_region = 0;
		không_thì nếu (!in_old_region)
			fprintf(cron_in, "%s\n", line.buf);
	}
	strbuf_release(&line);

	nếu (run_maintenance) {
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

	nếu (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		goto out;
	}

	nếu (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	không_thì
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	free(cmd);
	trả_về result;
}

static số_nguyên real_is_systemd_timer_available(void)
{
	struct child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", NULL);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	nếu (start_command(&child))
		trả_về 0;
	nếu (finish_command(&child))
		trả_về 0;
	trả_về 1;
}

static số_nguyên is_systemd_timer_available(void)
{
	số_nguyên is_available;

	nếu (get_schedule_cmd("systemctl", &is_available, NULL))
		trả_về is_available;

	trả_về real_is_systemd_timer_available();
}

static char *xdg_config_home_systemd(const char *filename)
{
	trả_về xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

static số_nguyên systemd_timer_delete_timer_file(enum schedule_priority priority)
{
	số_nguyên ret = 0;
	const char *frequency = get_frequency(priority);
	char *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	char *filename = xdg_config_home_systemd(local_timer_name);

	nếu (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	free(filename);
	free(local_timer_name);
	trả_về ret;
}

static số_nguyên systemd_timer_delete_service_template(void)
{
	số_nguyên ret = 0;
	char *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	char *filename = xdg_config_home_systemd(local_service_name);
	nếu (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	free(filename);
	free(local_service_name);
	trả_về ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
static số_nguyên systemd_timer_write_timer_file(enum schedule_priority schedule,
					  số_nguyên minute)
{
	số_nguyên res = -1;
	char *filename;
	FILE *file;
	const char *unit;
	char *schedule_pattern = NULL;
	const char *frequency = get_frequency(schedule);
	char *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	nếu (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		goto error;
	}
	file = fopen_or_warn(filename, "w");
	nếu (!file)
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
	nếu (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		goto error;
	}
	nếu (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		goto error;
	}

	res = 0;

error:
	free(schedule_pattern);
	free(local_timer_name);
	free(filename);
	trả_về res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
static số_nguyên systemd_timer_write_service_template(const char *exec_path)
{
	số_nguyên res = -1;
	char *filename;
	FILE *file;
	const char *unit;
	char *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	nếu (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		goto error;
	}
	file = fopen_or_warn(filename, "w");
	nếu (!file)
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
	nếu (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		goto error;
	}
	nếu (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		goto error;
	}

	res = 0;

error:
	free(local_service_name);
	free(filename);
	trả_về res;
}

static số_nguyên systemd_timer_enable_unit(số_nguyên enable,
				     enum schedule_priority schedule,
				     số_nguyên minute)
{
	char *cmd = NULL;
	struct child_process child = CHILD_PROCESS_INIT;
	const char *frequency = get_frequency(schedule);
	số_nguyên ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	nếu (!enable) {
		child.no_stderr = 1;
	} không_thì nếu (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		goto out;
	}

	get_schedule_cmd("systemctl", NULL, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", NULL);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	nếu (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		goto out;
	}

	nếu (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		nếu (enable) {
			ret = error(_("failed to run systemctl"));
			goto out;
		}
	}

	ret = 0;

out:
	free(cmd);
	trả_về ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
static void systemd_timer_delete_stale_timer_templates(void)
{
	char *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	char *filename = xdg_config_home_systemd(timer_template_name);

	nếu (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	free(filename);
	free(timer_template_name);
}

static số_nguyên systemd_timer_delete_unit_files(void)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	trả_về systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

static số_nguyên systemd_timer_delete_units(void)
{
	số_nguyên minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	trả_về systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

static số_nguyên systemd_timer_setup_units(void)
{
	số_nguyên minute = get_random_minute();
	const char *exec_path = git_exec_path();

	số_nguyên ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	nếu (ret)
		systemd_timer_delete_units();
	không_thì
		systemd_timer_delete_stale_timer_templates();

	trả_về ret;
}

static số_nguyên systemd_timer_update_schedule(số_nguyên run_maintenance, số_nguyên fd UNUSED)
{
	nếu (run_maintenance)
		trả_về systemd_timer_setup_units();
	không_thì
		trả_về systemd_timer_delete_units();
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
	số_nguyên (*is_available)(void);
	số_nguyên (*update_schedule)(số_nguyên run_maintenance, số_nguyên fd);
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
	nếu (!value)
		trả_về SCHEDULER_INVALID;
	không_thì nếu (!strcasecmp(value, "auto"))
		trả_về SCHEDULER_AUTO;
	không_thì nếu (!strcasecmp(value, "cron") || !strcasecmp(value, "crontab"))
		trả_về SCHEDULER_CRON;
	không_thì nếu (!strcasecmp(value, "systemd") ||
		 !strcasecmp(value, "systemd-timer"))
		trả_về SCHEDULER_SYSTEMD;
	không_thì nếu (!strcasecmp(value, "launchctl"))
		trả_về SCHEDULER_LAUNCHCTL;
	không_thì nếu (!strcasecmp(value, "schtasks"))
		trả_về SCHEDULER_SCHTASKS;
	không_thì
		trả_về SCHEDULER_INVALID;
}

static số_nguyên maintenance_opt_scheduler(const struct option *opt, const char *arg,
				     số_nguyên unset)
{
	enum scheduler *scheduler = opt->value;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	nếu (*scheduler == SCHEDULER_INVALID)
		trả_về error(_("unrecognized --scheduler argument '%s'"), arg);
	trả_về 0;
}

struct maintenance_start_opts {
	enum scheduler scheduler;
};

static enum scheduler resolve_scheduler(enum scheduler scheduler)
{
	nếu (scheduler != SCHEDULER_AUTO)
		trả_về scheduler;

#nếu defined(__APPLE__)
	trả_về SCHEDULER_LAUNCHCTL;

#elif defined(GIT_WINDOWS_NATIVE)
	trả_về SCHEDULER_SCHTASKS;

#elif defined(__linux__)
	nếu (is_systemd_timer_available())
		trả_về SCHEDULER_SYSTEMD;
	không_thì nếu (is_crontab_available())
		trả_về SCHEDULER_CRON;
	không_thì
		die(_("neither systemd timers nor crontab are available"));

#không_thì
	trả_về SCHEDULER_CRON;
#endif
}

static void validate_scheduler(enum scheduler scheduler)
{
	nếu (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	nếu (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	nếu (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

static số_nguyên update_background_schedule(const struct maintenance_start_opts *opts,
				      số_nguyên enable)
{
	unsigned số_nguyên i;
	số_nguyên result = 0;
	struct lock_file lk;
	char *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	nếu (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		nếu (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		không_thì
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		free(lock_path);
		trả_về -1;
	}

	cho (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		nếu (enable && opts->scheduler == i)
			continue;
		nếu (!scheduler_fn[i].is_available())
			continue;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	nếu (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	free(lock_path);
	trả_về result;
}

static const char *const builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	NULL
};

static số_nguyên maintenance_start(số_nguyên argc, const char **argv, const char *prefix,
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
	nếu (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	nếu (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	nếu (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, NULL, repo))
		warning(_("failed to add repo to global config"));
	trả_về 0;
}

static const char *const builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	NULL
};

static số_nguyên maintenance_stop(số_nguyên argc, const char **argv, const char *prefix,
			    struct repository *repo UNUSED)
{
	struct option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	nếu (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	trả_về update_background_schedule(NULL, 0);
}

static const char *const builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	NULL
};

static số_nguyên maintenance_is_needed(số_nguyên argc, const char **argv, const char *prefix,
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
	nếu (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	nếu (opts.auto_flag) {
		cho (size_t i = 0; i < opts.tasks_nr; i++) {
			nếu (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				break;
			}
		}
	} không_thì {
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

	nếu (is_needed)
		trả_về 0;
	trả_về 1;
}

static const char *const builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	NULL,
};

số_nguyên cmd_maintenance(số_nguyên argc,
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
	trả_về fn(argc, argv, prefix, repo);
}
