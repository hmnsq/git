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

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "date.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "tempfile.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "sigchain.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "commit-graph.h"
#அந்தர்பாவ "packfile.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "reflog.h"
#அந்தர்பாவ "rerere.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "exec-cmd.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hook.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "trace2.h"
#அந்தர்பாவ "worktree.h"

#define FAILED_RUN "failed to run %s"

நிலையான மாறிலி எழுத்து * மாறிலி builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	வெற்று
};

நிலையான timestamp_t gc_log_expire_time;
நிலையான கட்டமைப்பு tempfile *pidfile;
நிலையான கட்டமைப்பு lock_file log_lock;
நிலையான கட்டமைப்பு string_list pack_garbage = STRING_LIST_INIT_DUP;

நிலையான வெறுமை clean_pack_garbage(வெறுமை)
{
	முழு i;
	க்கு (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.பொருட்கள்[i].string);
	string_list_clear(&pack_garbage, 0);
}

நிலையான வெறுமை report_pack_garbage(unsigned seen_bits, மாறிலி எழுத்து *path)
{
	எனில் (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

நிலையான வெறுமை process_log_file(வெறுமை)
{
	கட்டமைப்பு stat st;
	எனில் (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		முழு saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} இல்லையெனில் எனில் (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} இல்லையெனில் {
		எழுத்து *path = repo_git_path(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(path);
		rollback_lock_file(&log_lock);
		விடுவி(path);
	}
}

நிலையான வெறுமை process_log_file_at_exit(வெறுமை)
{
	fflush(stderr);
	process_log_file();
}

நிலையான முழு gc_config_is_timestamp_never(மாறிலி எழுத்து *var)
{
	மாறிலி எழுத்து *மதிப்பு;
	timestamp_t expire;

	எனில் (!repo_config_get_மதிப்பு(the_repository, var, &மதிப்பு) && மதிப்பு) {
		எனில் (parse_expiry_date(மதிப்பு, &expire))
			die(_("failed to parse '%s' value '%s'"), var, மதிப்பு);
		திரும்பு expire == 0;
	}
	திரும்பு 0;
}

கட்டமைப்பு gc_config {
	முழு pack_refs;
	முழு prune_reflogs;
	முழு cruft_packs;
	unsigned long max_cruft_size;
	முழு aggressive_depth;
	முழு aggressive_window;
	முழு detach_auto;
	எழுத்து *gc_log_expire;
	எழுத்து *prune_expire;
	எழுத்து *prune_worktrees_expire;
	எழுத்து *repack_expire_to;
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

நிலையான வெறுமை gc_config_release(கட்டமைப்பு gc_config *cfg)
{
	விடுவி(cfg->gc_log_expire);
	விடுவி(cfg->prune_expire);
	விடுவி(cfg->prune_worktrees_expire);
}

நிலையான வெறுமை gc_config(கட்டமைப்பு gc_config *cfg)
{
	மாறிலி எழுத்து *மதிப்பு;
	எழுத்து *owned = வெற்று;

	எனில் (!repo_config_get_மதிப்பு(the_repository, "gc.packrefs", &மதிப்பு)) {
		எனில் (மதிப்பு && !strcmp(மதிப்பு, "notbare"))
			cfg->pack_refs = -1;
		இல்லையெனில்
			cfg->pack_refs = git_config_bool("gc.packrefs", மதிப்பு);
	}

	எனில் (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	எனில் (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		விடுவி(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	எனில் (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		விடுவி(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	எனில் (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		விடுவி(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, வெற்று);
}

எண்ணிக்கை schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

நிலையான எண்ணிக்கை schedule_priority parse_schedule(மாறிலி எழுத்து *மதிப்பு)
{
	எனில் (!மதிப்பு)
		திரும்பு SCHEDULE_NONE;
	எனில் (!strcasecmp(மதிப்பு, "hourly"))
		திரும்பு SCHEDULE_HOURLY;
	எனில் (!strcasecmp(மதிப்பு, "daily"))
		திரும்பு SCHEDULE_DAILY;
	எனில் (!strcasecmp(மதிப்பு, "weekly"))
		திரும்பு SCHEDULE_WEEKLY;
	திரும்பு SCHEDULE_NONE;
}

எண்ணிக்கை maintenance_task_label {
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
	TASK_மொத்தம்
};

கட்டமைப்பு maintenance_run_opts {
	எண்ணிக்கை maintenance_task_label *tasks;
	size_t tasks_nr, tasks_alloc;
	முழு auto_flag;
	முழு detach;
	முழு quiet;
	எண்ணிக்கை schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

நிலையான வெறுமை maintenance_run_opts_release(கட்டமைப்பு maintenance_run_opts *opts)
{
	விடுவி(opts->tasks);
}

நிலையான முழு pack_refs_condition(UNUSED கட்டமைப்பு gc_config *cfg)
{
	கட்டமைப்பு string_list included_refs = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	கட்டமைப்பு refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	எனில் (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		திரும்பு 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	திரும்பு required;
}

நிலையான முழு maintenance_task_pack_refs(கட்டமைப்பு maintenance_run_opts *opts,
				      UNUSED கட்டமைப்பு gc_config *cfg)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", வெற்று);
	எனில் (opts->auto_flag)
		strvec_தள்ளு(&cmd.args, "--auto");

	திரும்பு run_command(&cmd);
}

கட்டமைப்பு மொத்தம்_reflog_entries_data {
	கட்டமைப்பு expire_reflog_policy_cb policy;
	size_t மொத்தம்;
	size_t limit;
};

நிலையான முழு மொத்தம்_reflog_entries(மாறிலி எழுத்து *refname UNUSED,
				கட்டமைப்பு object_id *old_oid, கட்டமைப்பு object_id *new_oid,
				மாறிலி எழுத்து *committer, timestamp_t timestamp,
				முழு tz, மாறிலி எழுத்து *செய்தி, வெறுமை *cb_data)
{
	கட்டமைப்பு மொத்தம்_reflog_entries_data *data = cb_data;
	எனில் (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, செய்தி, &data->policy))
		data->மொத்தம்++;
	திரும்பு data->மொத்தம் >= data->limit;
}

நிலையான முழு reflog_expire_condition(கட்டமைப்பு gc_config *cfg UNUSED)
{
	timestamp_t now = time(வெற்று);
	கட்டமைப்பு மொத்தம்_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	முழு limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	எனில் (!limit)
		திரும்பு 0;
	எனில் (limit < 0)
		திரும்பு 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 மொத்தம்_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	திரும்பு data.மொத்தம் >= data.limit;
}

நிலையான முழு maintenance_task_reflog_expire(கட்டமைப்பு maintenance_run_opts *opts UNUSED,
					  கட்டமைப்பு gc_config *cfg UNUSED)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", வெற்று);
	திரும்பு run_command(&cmd);
}

நிலையான முழு maintenance_task_worktree_prune(கட்டமைப்பு maintenance_run_opts *opts UNUSED,
					   கட்டமைப்பு gc_config *cfg)
{
	கட்டமைப்பு child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", வெற்று);
	strvec_தள்ளு(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	திரும்பு run_command(&prune_worktrees_cmd);
}

நிலையான முழு worktree_prune_condition(கட்டமைப்பு gc_config *cfg)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	முழு should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	கட்டமைப்பு dirent *d;
	DIR *dir = வெற்று;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	எனில் (limit <= 0) {
		should_prune = limit < 0;
		செல் out;
	}

	எனில் (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		செல் out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	எனில் (!dir)
		செல் out;

	போது (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		எழுத்து *wtpath;
		strbuf_reset(&buf);
		எனில் (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		விடுவி(wtpath);
	}

	should_prune = !limit;

out:
	எனில் (dir)
		closedir(dir);
	strbuf_release(&buf);
	திரும்பு should_prune;
}

நிலையான முழு maintenance_task_rerere_gc(கட்டமைப்பு maintenance_run_opts *opts UNUSED,
				      கட்டமைப்பு gc_config *cfg UNUSED)
{
	கட்டமைப்பு child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", வெற்று);
	திரும்பு run_command(&rerere_cmd);
}

நிலையான முழு rerere_gc_condition(கட்டமைப்பு gc_config *cfg UNUSED)
{
	முழு limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	எனில் (!limit)
		திரும்பு 0; /* never prune */
	எனில் (limit < 0)
		திரும்பு 1; /* always prune */

	திரும்பு rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
நிலையான மாறிலி எழுத்து *lock_repo_for_gc(முழு force, pid_t* ret_pid)
{
	கட்டமைப்பு lock_file lock = LOCK_INIT;
	எழுத்து my_host[HOST_NAME_MAX + 1];
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	கட்டமைப்பு stat st;
	uintmax_t pid;
	FILE *fp;
	முழு fd;
	எழுத்து *pidfile_path;

	எனில் (is_tempfile_active(pidfile))
		/* already locked */
		திரும்பு வெற்று;

	எனில் (xgethostname(my_host, அளவு(my_host)))
		xsnprintf(my_host, அளவு(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	எனில் (!force) {
		நிலையான எழுத்து locking_host[HOST_NAME_MAX + 1];
		நிலையான எழுத்து *scan_fmt;
		முழு should_exit;

		எனில் (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_path, "r");
		memset(locking_host, 0, அளவு(locking_host));
		should_exit =
			fp != வெற்று &&
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
			time(வெற்று) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, scan_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		எனில் (fp)
			fclose(fp);
		எனில் (should_exit) {
			எனில் (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			விடுவி(pidfile_path);
			திரும்பு locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	விடுவி(pidfile_path);
	திரும்பு வெற்று;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
நிலையான முழு report_last_gc_error(வெறுமை)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	முழு ret = 0;
	ssize_t len;
	கட்டமைப்பு stat st;
	எழுத்து *gc_log_path = repo_git_path(the_repository, "gc.log");

	எனில் (stat(gc_log_path, &st)) {
		எனில் (errno == ENOENT)
			செல் done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		செல் done;
	}

	எனில் (st.st_mtime < gc_log_expire_time)
		செல் done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	எனில் (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	இல்லையெனில் எனில் (len > 0) {
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
	விடுவி(gc_log_path);
	திரும்பு ret;
}

நிலையான முழு gc_foreground_tasks(கட்டமைப்பு maintenance_run_opts *opts,
			       கட்டமைப்பு gc_config *cfg)
{
	எனில் (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		திரும்பு error(FAILED_RUN, "pack-refs");
	எனில் (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		திரும்பு error(FAILED_RUN, "reflog");
	திரும்பு 0;
}

நிலையான முழு maintenance_task_odb(கட்டமைப்பு maintenance_run_opts *opts,
				கட்டமைப்பு gc_config *cfg,
				முழு keep_largest_pack,
				முழு aggressive)
{
	கட்டமைப்பு odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	எனில் (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	எனில் (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	எனில் (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	திரும்பு odb_optimize(the_repository->objects, &odb_opts);
}

முழு cmd_gc(முழு argc,
	   மாறிலி எழுத்து **argv,
	   மாறிலி எழுத்து *prefix,
	   கட்டமைப்பு repository *repo UNUSED)
{
	முழு aggressive = 0;
	முழு force = 0;
	மாறிலி எழுத்து *name;
	pid_t pid;
	முழு daemonized = 0;
	முழு keep_largest_pack = -1;
	முழு skip_foreground_tasks = 0;
	timestamp_t dummy;
	கட்டமைப்பு maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	கட்டமைப்பு gc_config cfg = GC_CONFIG_INIT;
	மாறிலி எழுத்து *prune_expire_sentinel = "sentinel";
	மாறிலி எழுத்து *prune_expire_arg = prune_expire_sentinel;
	முழு ret;
	கட்டமைப்பு option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.மதிப்பு = &prune_expire_arg,
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

	எனில் (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	எனில் (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	எனில் (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	எனில் (prune_expire_arg != prune_expire_sentinel) {
		விடுவி(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	எனில் (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	எனில் (opts.auto_flag) {
		கட்டமைப்பு odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		எனில் (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		எனில் (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			செல் out;
		}

		எனில் (!opts.quiet) {
			எனில் (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			இல்லையெனில்
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	எனில் (opts.detach > 0) {
		ret = report_last_gc_error();
		எனில் (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			செல் out;

		} இல்லையெனில் எனில் (ret) {
			/* an I/O error occurred, already reported */
			செல் out;
		}

		எனில் (!skip_foreground_tasks) {
			எனில் (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				செல் out;
			}

			எனில் (gc_foreground_tasks(&opts, &cfg) < 0)
				die(வெற்று);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = lock_repo_for_gc(force, &pid);
	எனில் (name) {
		எனில் (opts.auto_flag) {
			ret = 0;
			செல் out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	எனில் (daemonized) {
		எழுத்து *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		விடுவி(path);
	}

	எனில் (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	எனில் (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	எனில் (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	எனில் (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(வெற்று);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	எனில் (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	எனில் (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     வெற்று);

	எனில் (!daemonized) {
		எழுத்து *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		விடுவி(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *மாறிலி builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	வெற்று
};

நிலையான முழு maintenance_opt_schedule(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
				    முழு unset)
{
	எண்ணிக்கை schedule_priority *priority = opt->மதிப்பு;

	எனில் (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	எனில் (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	திரும்பு 0;
}

கட்டமைப்பு cg_auto_data {
	முழு இலக்கம்_not_in_graph;
	முழு limit;
};

நிலையான முழு dfs_on_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு cg_auto_data *data = (கட்டமைப்பு cg_auto_data *)cb_data;
	முழு result = 0;
	மாறிலி கட்டமைப்பு object_id *maybe_peeled = ref->oid;
	கட்டமைப்பு object_id peeled;
	கட்டமைப்பு commit_list *அடுக்கு = வெற்று;
	கட்டமைப்பு commit *commit;

	எனில் (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	எனில் (odb_read_object_info(the_repository->objects, maybe_peeled, வெற்று) != OBJ_COMMIT)
		திரும்பு 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	எனில் (!commit || commit->object.flags & SEEN)
		திரும்பு 0;
	commit->object.flags |= SEEN;

	எனில் (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		திரும்பு 0;

	data->இலக்கம்_not_in_graph++;

	எனில் (data->இலக்கம்_not_in_graph >= data->limit)
		திரும்பு 1;

	commit_list_செருகு(commit, &அடுக்கு);

	போது (!result && அடுக்கு) {
		கட்டமைப்பு commit_list *parent;

		commit = எடு_commit(&அடுக்கு);

		க்கு (parent = commit->parents; parent; parent = parent->next) {
			எனில் (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				தொடர்;

			parent->item->object.flags |= SEEN;
			data->இலக்கம்_not_in_graph++;

			எனில் (data->இலக்கம்_not_in_graph >= data->limit) {
				result = 1;
				நிறுத்து;
			}

			commit_list_செருகு(parent->item, &அடுக்கு);
		}
	}

	commit_list_free(அடுக்கு);
	திரும்பு result;
}

நிலையான முழு should_write_commit_graph(கட்டமைப்பு gc_config *cfg UNUSED)
{
	முழு result;
	கட்டமைப்பு cg_auto_data data;

	data.இலக்கம்_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	எனில் (!data.limit)
		திரும்பு 0;
	எனில் (data.limit < 0)
		திரும்பு 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	திரும்பு result;
}

நிலையான முழு run_write_commit_graph(கட்டமைப்பு maintenance_run_opts *opts)
{
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", வெற்று);

	எனில் (opts->quiet)
		strvec_தள்ளு(&child.args, "--no-progress");
	இல்லையெனில்
		strvec_தள்ளு(&child.args, "--progress");

	திரும்பு !!run_command(&child);
}

நிலையான முழு maintenance_task_commit_graph(கட்டமைப்பு maintenance_run_opts *opts,
					 கட்டமைப்பு gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	எனில் (!the_repository->settings.core_commit_graph)
		திரும்பு 0;

	எனில் (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		திரும்பு 1;
	}

	திரும்பு 0;
}

நிலையான முழு fetch_remote(கட்டமைப்பு remote *remote, வெறுமை *cbdata)
{
	கட்டமைப்பு maintenance_run_opts *opts = cbdata;
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	எனில் (remote->skip_default_update)
		திரும்பு 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     வெற்று);

	எனில் (opts->quiet)
		strvec_தள்ளு(&child.args, "--quiet");

	திரும்பு !!run_command(&child);
}

நிலையான முழு maintenance_task_prefetch(கட்டமைப்பு maintenance_run_opts *opts,
				     கட்டமைப்பு gc_config *cfg UNUSED)
{
	எனில் (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		திரும்பு 1;
	}

	திரும்பு 0;
}

நிலையான முழு maintenance_task_gc_foreground(கட்டமைப்பு maintenance_run_opts *opts,
					  கட்டமைப்பு gc_config *cfg)
{
	திரும்பு gc_foreground_tasks(opts, cfg);
}

நிலையான முழு maintenance_task_gc_background(கட்டமைப்பு maintenance_run_opts *opts,
					  கட்டமைப்பு gc_config *cfg UNUSED)
{
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_தள்ளு(&child.args, "gc");

	எனில் (opts->auto_flag)
		strvec_தள்ளு(&child.args, "--auto");
	எனில் (opts->quiet)
		strvec_தள்ளு(&child.args, "--quiet");
	இல்லையெனில்
		strvec_தள்ளு(&child.args, "--no-quiet");
	strvec_தள்ளு(&child.args, "--no-detach");
	strvec_தள்ளு(&child.args, "--skip-foreground-tasks");

	திரும்பு run_command(&child);
}

நிலையான முழு gc_condition(கட்டமைப்பு gc_config *cfg)
{
	கட்டமைப்பு odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	திரும்பு odb_optimize_required(the_repository->objects, &opts);
}

நிலையான முழு prune_packed(கட்டமைப்பு maintenance_run_opts *opts)
{
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_தள்ளு(&child.args, "prune-packed");

	எனில் (opts->quiet)
		strvec_தள்ளு(&child.args, "--quiet");

	திரும்பு !!run_command(&child);
}

கட்டமைப்பு write_loose_object_data {
	FILE *in;
	முழு மொத்தம்;
	முழு batch_size;
};

நிலையான முழு loose_object_auto_limit = 100;

நிலையான முழு loose_object_மொத்தம்(மாறிலி கட்டமைப்பு object_id *oid UNUSED,
			      மாறிலி எழுத்து *path UNUSED,
			      வெறுமை *data)
{
	முழு *மொத்தம் = (முழு*)data;
	எனில் (++(*மொத்தம்) >= loose_object_auto_limit)
		திரும்பு 1;
	திரும்பு 0;
}

நிலையான முழு loose_object_auto_condition(கட்டமைப்பு gc_config *cfg UNUSED)
{
	முழு மொத்தம் = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	எனில் (!loose_object_auto_limit)
		திரும்பு 0;
	எனில் (loose_object_auto_limit < 0)
		திரும்பு 1;

	திரும்பு for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_மொத்தம்,
					     வெற்று, வெற்று, &மொத்தம்);
}

நிலையான முழு bail_on_loose(மாறிலி கட்டமைப்பு object_id *oid UNUSED,
			 மாறிலி எழுத்து *path UNUSED,
			 வெறுமை *data UNUSED)
{
	திரும்பு 1;
}

நிலையான முழு write_loose_object_to_stdin(மாறிலி கட்டமைப்பு object_id *oid,
				       மாறிலி எழுத்து *path UNUSED,
				       வெறுமை *data)
{
	கட்டமைப்பு write_loose_object_data *d = (கட்டமைப்பு write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	திரும்பு ++(d->மொத்தம்) > d->batch_size;
}

நிலையான முழு pack_loose(கட்டமைப்பு maintenance_run_opts *opts)
{
	கட்டமைப்பு repository *r = the_repository;
	முழு result = 0;
	கட்டமைப்பு write_loose_object_data data;
	கட்டமைப்பு child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	எனில் (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   வெற்று, வெற்று, வெற்று))
		திரும்பு 0;

	pack_proc.git_cmd = 1;

	strvec_தள்ளு(&pack_proc.args, "pack-objects");
	எனில் (opts->quiet)
		strvec_தள்ளு(&pack_proc.args, "--quiet");
	இல்லையெனில்
		strvec_தள்ளு(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	எனில் (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		திரும்பு 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.மொத்தம் = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	எனில் (!data.batch_size)
		data.batch_size = INT_MAX;
	இல்லையெனில் எனில் (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      வெற்று, வெற்று, &data);

	fclose(data.in);

	எனில் (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	திரும்பு result;
}

நிலையான முழு maintenance_task_loose_objects(கட்டமைப்பு maintenance_run_opts *opts,
					  கட்டமைப்பு gc_config *cfg UNUSED)
{
	திரும்பு prune_packed(opts) || pack_loose(opts);
}

நிலையான முழு incremental_repack_auto_condition(கட்டமைப்பு gc_config *cfg UNUSED)
{
	கட்டமைப்பு packed_git *p;
	முழு incremental_repack_auto_limit = 10;
	முழு மொத்தம் = 0;

	prepare_repo_settings(the_repository);
	எனில் (!the_repository->settings.core_multi_pack_index)
		திரும்பு 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	எனில் (!incremental_repack_auto_limit)
		திரும்பு 0;
	எனில் (incremental_repack_auto_limit < 0)
		திரும்பு 1;

	repo_for_each_pack(the_repository, p) {
		எனில் (மொத்தம் >= incremental_repack_auto_limit)
			நிறுத்து;
		எனில் (!p->multi_pack_index)
			மொத்தம்++;
	}

	திரும்பு மொத்தம் >= incremental_repack_auto_limit;
}

நிலையான முழு multi_pack_index_write(கட்டமைப்பு maintenance_run_opts *opts)
{
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", வெற்று);

	எனில் (opts->quiet)
		strvec_தள்ளு(&child.args, "--no-progress");
	இல்லையெனில்
		strvec_தள்ளு(&child.args, "--progress");

	எனில் (run_command(&child))
		திரும்பு error(_("failed to write multi-pack-index"));

	திரும்பு 0;
}

நிலையான முழு multi_pack_index_expire(கட்டமைப்பு maintenance_run_opts *opts)
{
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", வெற்று);

	எனில் (opts->quiet)
		strvec_தள்ளு(&child.args, "--no-progress");
	இல்லையெனில்
		strvec_தள்ளு(&child.args, "--progress");

	எனில் (run_command(&child))
		திரும்பு error(_("'git multi-pack-index expire' failed"));

	திரும்பு 0;
}

#define TWO_GIGABYTES (INT32_MAX)

நிலையான off_t get_auto_pack_size(வெறுமை)
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
	கட்டமைப்பு packed_git *p;
	கட்டமைப்பு repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		எனில் (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} இல்லையெனில் எனில் (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	எனில் (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	திரும்பு result_size;
}

நிலையான முழு multi_pack_index_repack(கட்டமைப்பு maintenance_run_opts *opts)
{
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", வெற்று);

	எனில் (opts->quiet)
		strvec_தள்ளு(&child.args, "--no-progress");
	இல்லையெனில்
		strvec_தள்ளு(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	எனில் (run_command(&child))
		திரும்பு error(_("'git multi-pack-index repack' failed"));

	திரும்பு 0;
}

நிலையான முழு maintenance_task_incremental_repack(கட்டமைப்பு maintenance_run_opts *opts,
					       கட்டமைப்பு gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	எனில் (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		திரும்பு 0;
	}

	எனில் (multi_pack_index_write(opts))
		திரும்பு 1;
	எனில் (multi_pack_index_expire(opts))
		திரும்பு 1;
	எனில் (multi_pack_index_repack(opts))
		திரும்பு 1;
	திரும்பு 0;
}

நிலையான முழு maintenance_task_geometric_repack(கட்டமைப்பு maintenance_run_opts *opts,
					     கட்டமைப்பு gc_config *cfg)
{
	கட்டமைப்பு odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	எனில் (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	திரும்பு odb_optimize(the_repository->objects, &odb_opts);
}

நிலையான முழு geometric_repack_auto_condition(கட்டமைப்பு gc_config *cfg)
{
	கட்டமைப்பு odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	திரும்பு odb_optimize_required(the_repository->objects, &opts);
}

வகைவரை முழு (*maintenance_task_fn)(கட்டமைப்பு maintenance_run_opts *opts,
				   கட்டமைப்பு gc_config *cfg);
வகைவரை முழு (*maintenance_auto_fn)(கட்டமைப்பு gc_config *cfg);

கட்டமைப்பு maintenance_task {
	மாறிலி எழுத்து *name;

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

நிலையான மாறிலி கட்டமைப்பு maintenance_task tasks[] = {
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

எண்ணிக்கை task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

எண்ணிக்கை auto_gc_hook_result {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

நிலையான முழு maybe_run_task(மாறிலி கட்டமைப்பு maintenance_task *task,
			  கட்டமைப்பு repository *repo,
			  கட்டமைப்பு maintenance_run_opts *opts,
			  கட்டமைப்பு gc_config *cfg,
			  எண்ணிக்கை task_phase phase,
			  எண்ணிக்கை auto_gc_hook_result *auto_gc_hook_result)
{
	முழு foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	மாறிலி எழுத்து *region = foreground ? "maintenance foreground" : "maintenance";
	முழு ret = 0;

	எனில் (!fn)
		திரும்பு 0;
	எனில் (opts->auto_flag) {
		எனில் (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			திரும்பு 0;

		எனில் (!task->auto_condition || !task->auto_condition(cfg))
			திரும்பு 0;

		எனில் (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		எனில் (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			திரும்பு 0;
	}

	trace2_region_enter(region, task->name, repo);
	எனில் (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	திரும்பு ret;
}

நிலையான முழு maintenance_run_tasks(கட்டமைப்பு maintenance_run_opts *opts,
				 கட்டமைப்பு gc_config *cfg)
{
	முழு result = 0;
	கட்டமைப்பு lock_file lk;
	கட்டமைப்பு repository *r = the_repository;
	எழுத்து *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	எண்ணிக்கை auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	எனில் (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		எனில் (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		விடுவி(lock_path);
		திரும்பு 0;
	}
	விடுவி(lock_path);

	க்கு (size_t i = 0; i < opts->tasks_nr; i++)
		எனில் (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	எனில் (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	க்கு (size_t i = 0; i < opts->tasks_nr; i++)
		எனில் (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	திரும்பு result;
}

எண்ணிக்கை maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

கட்டமைப்பு maintenance_strategy {
	கட்டமைப்பு {
		unsigned type;
		எண்ணிக்கை schedule_priority schedule;
	} tasks[TASK_மொத்தம்];
};

நிலையான மாறிலி கட்டமைப்பு maintenance_strategy none_strategy = { 0 };

நிலையான மாறிலி கட்டமைப்பு maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

நிலையான மாறிலி கட்டமைப்பு maintenance_strategy incremental_strategy = {
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

நிலையான மாறிலி கட்டமைப்பு maintenance_strategy geometric_strategy = {
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

நிலையான கட்டமைப்பு maintenance_strategy parse_maintenance_strategy(மாறிலி எழுத்து *name)
{
	எனில் (!strcasecmp(name, "incremental"))
		திரும்பு incremental_strategy;
	எனில் (!strcasecmp(name, "gc"))
		திரும்பு gc_strategy;
	எனில் (!strcasecmp(name, "geometric"))
		திரும்பு geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

நிலையான வெறுமை initialize_task_config(கட்டமைப்பு maintenance_run_opts *opts,
				   மாறிலி கட்டமைப்பு string_list *selected_tasks)
{
	கட்டமைப்பு strbuf config_name = STRBUF_INIT;
	கட்டமைப்பு maintenance_strategy strategy;
	எண்ணிக்கை maintenance_type type;
	மாறிலி எழுத்து *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	எனில் (selected_tasks->nr) {
		க்கு (size_t i = 0; i < selected_tasks->nr; i++) {
			எண்ணிக்கை maintenance_task_label label = (intptr_t)selected_tasks->பொருட்கள்[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		திரும்பு;
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
	எனில் (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} இல்லையெனில் {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	எனில் (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	க்கு (size_t i = 0; i < TASK_மொத்தம்; i++) {
		முழு config_மதிப்பு;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		எனில் (!repo_config_get_bool(the_repository, config_name.buf, &config_மதிப்பு))
			strategy.tasks[i].type = config_மதிப்பு ? type : 0;
		எனில் (!(strategy.tasks[i].type & type))
			தொடர்;

		எனில் (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			எனில் (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			எனில் (strategy.tasks[i].schedule < opts->schedule)
				தொடர்;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

நிலையான முழு task_option_parse(மாறிலி கட்டமைப்பு option *opt,
			     மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு string_list *selected_tasks = opt->மதிப்பு;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	க்கு (i = 0; i < TASK_மொத்தம்; i++)
		எனில் (!strcasecmp(tasks[i].name, arg))
			நிறுத்து;
	எனில் (i >= TASK_மொத்தம்) {
		error(_("'%s' is not a valid task"), arg);
		திரும்பு 1;
	}

	எனில் (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		திரும்பு 1;
	}

	string_list_append(selected_tasks, arg)->util = (வெறுமை *)(intptr_t)i;

	திரும்பு 0;
}

நிலையான முழு maintenance_run(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	கட்டமைப்பு string_list selected_tasks = STRING_LIST_INIT_DUP;
	கட்டமைப்பு gc_config cfg = GC_CONFIG_INIT;
	கட்டமைப்பு option builtin_maintenance_run_options[] = {
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
	முழு ret;

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

	எனில் (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	திரும்பு ret;
}

நிலையான எழுத்து *get_maintpath(வெறுமை)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	திரும்பு strbuf_detach(&sb, வெற்று);
}

நிலையான எழுத்து மாறிலி * மாறிலி builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	வெற்று
};

நிலையான முழு maintenance_register(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				கட்டமைப்பு repository *repo UNUSED)
{
	எழுத்து *config_file = வெற்று;
	கட்டமைப்பு option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	முழு found = 0;
	மாறிலி எழுத்து *சாவி = "maintenance.repo";
	எழுத்து *maintpath = get_maintpath();
	கட்டமைப்பு string_list_item *item;
	மாறிலி கட்டமைப்பு string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	எனில் (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	எனில் (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	எனில் (!repo_config_get_string_multi(the_repository, சாவி, &list)) {
		for_each_string_list_item(item, list) {
			எனில் (!strcmp(maintpath, item->string)) {
				found = 1;
				நிறுத்து;
			}
		}
	}

	எனில் (!found) {
		முழு rc;
		எழுத்து *global_config_file = வெற்று;

		எனில் (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		எனில் (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, வெற்று, 0);
		விடுவி(global_config_file);

		எனில் (rc)
			die(_("unable to add '%s' value of '%s'"),
			    சாவி, maintpath);
	}

	விடுவி(maintpath);
	திரும்பு 0;
}

நிலையான எழுத்து மாறிலி * மாறிலி builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	வெற்று
};

நிலையான முழு maintenance_unregister(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				  கட்டமைப்பு repository *repo UNUSED)
{
	முழு force = 0;
	எழுத்து *config_file = வெற்று;
	கட்டமைப்பு option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	மாறிலி எழுத்து *சாவி = "maintenance.repo";
	எழுத்து *maintpath = get_maintpath();
	முழு found = 0;
	கட்டமைப்பு string_list_item *item;
	மாறிலி கட்டமைப்பு string_list *list;
	கட்டமைப்பு config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	எனில் (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	எனில் (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	எனில் (!(config_file
	      ? git_configset_get_string_multi(&cs, சாவி, &list)
	      : repo_config_get_string_multi(the_repository, சாவி, &list))) {
		for_each_string_list_item(item, list) {
			எனில் (!strcmp(maintpath, item->string)) {
				found = 1;
				நிறுத்து;
			}
		}
	}

	எனில் (found) {
		முழு rc;
		எழுத்து *global_config_file = வெற்று;

		எனில் (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		எனில் (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, சாவி, வெற்று, maintpath, வெற்று,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_மதிப்பு);
		விடுவி(global_config_file);

		எனில் (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    சாவி, maintpath);
	} இல்லையெனில் எனில் (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	விடுவி(maintpath);
	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *get_frequency(எண்ணிக்கை schedule_priority schedule)
{
	தேர்வு (schedule) {
	நிலை SCHEDULE_HOURLY:
		திரும்பு "hourly";
	நிலை SCHEDULE_DAILY:
		திரும்பு "daily";
	நிலை SCHEDULE_WEEKLY:
		திரும்பு "weekly";
	இயல்பு:
		BUG("invalid schedule %d", schedule);
	}
}

நிலையான மாறிலி எழுத்து *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	வெற்று
};

நிலையான மாறிலி எழுத்து *get_extra_config_parameters(வெறுமை) {
	நிலையான மாறிலி எழுத்து *result = வெற்று;
	கட்டமைப்பு strbuf builder = STRBUF_INIT;

	எனில் (result)
		திரும்பு result;

	க்கு (மாறிலி எழுத்து **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, வெற்று);
	திரும்பு result;
}

நிலையான மாறிலி எழுத்து *get_extra_launchctl_strings(வெறுமை) {
	நிலையான மாறிலி எழுத்து *result = வெற்று;
	கட்டமைப்பு strbuf builder = STRBUF_INIT;

	எனில் (result)
		திரும்பு result;

	க்கு (மாறிலி எழுத்து **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, வெற்று);
	திரும்பு result;
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
நிலையான முழு get_schedule_cmd(மாறிலி எழுத்து *cmd, முழு *is_available, எழுத்து **out)
{
	எழுத்து *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	கட்டமைப்பு string_list_item *item;
	கட்டமைப்பு string_list list = STRING_LIST_INIT_NODUP;

	எனில் (!testing) {
		எனில் (out)
			*out = xstrdup(cmd);
		திரும்பு 0;
	}

	எனில் (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		கட்டமைப்பு string_list pair = STRING_LIST_INIT_NODUP;

		எனில் (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			தொடர்;

		எனில் (!strcmp(cmd, pair.பொருட்கள்[0].string)) {
			எனில் (out)
				*out = xstrdup(pair.பொருட்கள்[1].string);
			எனில் (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			செல் out;
		}

		string_list_clear(&pair, 0);
	}

	எனில் (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	விடுவி(testing);
	திரும்பு 1;
}

நிலையான முழு get_random_minute(வெறுமை)
{
	/* Use a static value when under tests. */
	எனில் (getenv("GIT_TEST_MAINT_SCHEDULER"))
		திரும்பு 13;

	திரும்பு git_rand(0) % 60;
}

நிலையான முழு is_launchctl_available(வெறுமை)
{
	முழு is_available;
	எனில் (get_schedule_cmd("launchctl", &is_available, வெற்று))
		திரும்பு is_available;

#ifdef __APPLE__
	திரும்பு 1;
#இல்லையெனில்
	திரும்பு 0;
#endif
}

நிலையான எழுத்து *launchctl_service_name(மாறிலி எழுத்து *frequency)
{
	கட்டமைப்பு strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	திரும்பு strbuf_detach(&label, வெற்று);
}

நிலையான எழுத்து *launchctl_service_filename(மாறிலி எழுத்து *name)
{
	எழுத்து *expanded;
	கட்டமைப்பு strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	எனில் (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	திரும்பு expanded;
}

நிலையான எழுத்து *launchctl_get_uid(வெறுமை)
{
	திரும்பு xstrfmt("gui/%d", getuid());
}

நிலையான முழு launchctl_boot_plist(முழு enable, மாறிலி எழுத்து *filename)
{
	எழுத்து *cmd;
	முழு result;
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;
	எழுத்து *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", வெற்று, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, வெற்று);

	child.no_stderr = 1;
	child.no_stdout = 1;

	எனில் (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	விடுவி(cmd);
	விடுவி(uid);
	திரும்பு result;
}

நிலையான முழு launchctl_remove_plist(எண்ணிக்கை schedule_priority schedule)
{
	மாறிலி எழுத்து *frequency = get_frequency(schedule);
	எழுத்து *name = launchctl_service_name(frequency);
	எழுத்து *filename = launchctl_service_filename(name);
	முழு result = launchctl_boot_plist(0, filename);
	unlink(filename);
	விடுவி(filename);
	விடுவி(name);
	திரும்பு result;
}

நிலையான முழு launchctl_remove_plists(வெறுமை)
{
	திரும்பு launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

நிலையான முழு launchctl_list_contains_plist(மாறிலி எழுத்து *name, மாறிலி எழுத்து *cmd)
{
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, வெற்று);

	child.no_stderr = 1;
	child.no_stdout = 1;

	எனில் (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	திரும்பு !finish_command(&child);
}

நிலையான முழு launchctl_schedule_plist(மாறிலி எழுத்து *exec_path, எண்ணிக்கை schedule_priority schedule)
{
	முழு i, fd;
	மாறிலி எழுத்து *preamble, *repeat;
	மாறிலி எழுத்து *frequency = get_frequency(schedule);
	எழுத்து *name = launchctl_service_name(frequency);
	எழுத்து *filename = launchctl_service_filename(name);
	கட்டமைப்பு lock_file lk = LOCK_INIT;
	நிலையான unsigned long lock_file_timeout_ms = ULONG_MAX;
	கட்டமைப்பு strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	கட்டமைப்பு stat st;
	எழுத்து *cmd;
	முழு minute = get_random_minute();

	get_schedule_cmd("launchctl", வெற்று, &cmd);
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

	தேர்வு (schedule) {
	நிலை SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		க்கு (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		நிறுத்து;

	நிலை SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		க்கு (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		நிறுத்து;

	நிலை SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		நிறுத்து;

	இயல்பு:
		/* unreachable */
		நிறுத்து;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	எனில் (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	எனில் ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	எனில் (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	இல்லையெனில் {
		எனில் (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		எனில் (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	விடுவி(filename);
	விடுவி(name);
	விடுவி(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	திரும்பு 0;
}

நிலையான முழு launchctl_add_plists(வெறுமை)
{
	மாறிலி எழுத்து *exec_path = git_exec_path();

	திரும்பு launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

நிலையான முழு launchctl_update_schedule(முழு run_maintenance, முழு fd UNUSED)
{
	எனில் (run_maintenance)
		திரும்பு launchctl_add_plists();
	இல்லையெனில்
		திரும்பு launchctl_remove_plists();
}

நிலையான முழு is_schtasks_available(வெறுமை)
{
	முழு is_available;
	எனில் (get_schedule_cmd("schtasks", &is_available, வெற்று))
		திரும்பு is_available;

#ifdef GIT_WINDOWS_NATIVE
	திரும்பு 1;
#இல்லையெனில்
	திரும்பு 0;
#endif
}

நிலையான எழுத்து *schtasks_task_name(மாறிலி எழுத்து *frequency)
{
	கட்டமைப்பு strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	திரும்பு strbuf_detach(&label, வெற்று);
}

நிலையான முழு schtasks_remove_task(எண்ணிக்கை schedule_priority schedule)
{
	எழுத்து *cmd;
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;
	மாறிலி எழுத்து *frequency = get_frequency(schedule);
	எழுத்து *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", வெற்று, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", வெற்று);
	விடுவி(name);
	விடுவி(cmd);

	திரும்பு run_command(&child);
}

நிலையான முழு schtasks_remove_tasks(வெறுமை)
{
	திரும்பு schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

நிலையான முழு schtasks_schedule_task(மாறிலி எழுத்து *exec_path, எண்ணிக்கை schedule_priority schedule)
{
	எழுத்து *cmd;
	முழு result;
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;
	மாறிலி எழுத்து *xml;
	கட்டமைப்பு tempfile *tfile;
	மாறிலி எழுத்து *frequency = get_frequency(schedule);
	எழுத்து *name = schtasks_task_name(frequency);
	கட்டமைப்பு strbuf tfilename = STRBUF_INIT;
	முழு minute = get_random_minute();

	get_schedule_cmd("schtasks", வெற்று, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	எனில் (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	தேர்வு (schedule) {
	நிலை SCHEDULE_HOURLY:
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
		நிறுத்து;

	நிலை SCHEDULE_DAILY:
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
		நிறுத்து;

	நிலை SCHEDULE_WEEKLY:
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
		நிறுத்து;

	இயல்பு:
		நிறுத்து;
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
				  get_tempfile_path(tfile), வெற்று);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	எனில் (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	விடுவி(name);
	விடுவி(cmd);
	திரும்பு result;
}

நிலையான முழு schtasks_schedule_tasks(வெறுமை)
{
	மாறிலி எழுத்து *exec_path = git_exec_path();

	திரும்பு schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

நிலையான முழு schtasks_update_schedule(முழு run_maintenance, முழு fd UNUSED)
{
	எனில் (run_maintenance)
		திரும்பு schtasks_schedule_tasks();
	இல்லையெனில்
		திரும்பு schtasks_remove_tasks();
}

MAYBE_UNUSED
நிலையான முழு check_crontab_process(மாறிலி எழுத்து *cmd)
{
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_தள்ளு(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	எனில் (start_command(&child))
		திரும்பு 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	திரும்பு 1;
}

நிலையான முழு is_crontab_available(வெறுமை)
{
	எழுத்து *cmd;
	முழு is_available;
	முழு ret;

	எனில் (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		செல் out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#இல்லையெனில்
	ret = check_crontab_process(cmd);
#endif

out:
	விடுவி(cmd);
	திரும்பு ret;
}

#define BEGIN_வரி "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_வரி "# END GIT MAINTENANCE SCHEDULE"

நிலையான முழு crontab_update_schedule(முழு run_maintenance, முழு fd)
{
	எழுத்து *cmd;
	முழு result = 0;
	முழு in_old_region = 0;
	கட்டமைப்பு child_process crontab_list = CHILD_PROCESS_INIT;
	கட்டமைப்பு child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	கட்டமைப்பு strbuf வரி = STRBUF_INIT;
	கட்டமைப்பு tempfile *tmpedit = வெற்று;
	முழு minute = get_random_minute();

	get_schedule_cmd("crontab", வெற்று, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_தள்ளு(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	எனில் (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		செல் out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	எனில் (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		செல் out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	எனில் (!cron_in) {
		result = error(_("failed to open temporary file"));
		செல் out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	போது (!strbuf_getline_lf(&வரி, cron_list)) {
		எனில் (!in_old_region && !strcmp(வரி.buf, BEGIN_வரி))
			in_old_region = 1;
		இல்லையெனில் எனில் (in_old_region && !strcmp(வரி.buf, END_வரி))
			in_old_region = 0;
		இல்லையெனில் எனில் (!in_old_region)
			fprintf(cron_in, "%s\n", வரி.buf);
	}
	strbuf_release(&வரி);

	எனில் (run_maintenance) {
		கட்டமைப்பு strbuf வரி_format = STRBUF_INIT;
		மாறிலி எழுத்து *exec_path = git_exec_path();

		fprintf(cron_in, "%s\n", BEGIN_வரி);
		fprintf(cron_in,
			"# The following schedule was created by Git\n");
		fprintf(cron_in, "# Any edits made in this region might be\n");
		fprintf(cron_in,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&வரி_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_path, exec_path, get_extra_config_parameters());
		fprintf(cron_in, வரி_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_in, வரி_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_in, வரி_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&வரி_format);

		fprintf(cron_in, "\n%s\n", END_வரி);
	}

	fflush(cron_in);

	strvec_split(&crontab_edit.args, cmd);
	strvec_தள்ளு(&crontab_edit.args, get_tempfile_path(tmpedit));
	crontab_edit.git_cmd = 0;

	எனில் (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		செல் out;
	}

	எனில் (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	இல்லையெனில்
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	விடுவி(cmd);
	திரும்பு result;
}

நிலையான முழு real_is_systemd_timer_available(வெறுமை)
{
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", வெற்று);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	எனில் (start_command(&child))
		திரும்பு 0;
	எனில் (finish_command(&child))
		திரும்பு 0;
	திரும்பு 1;
}

நிலையான முழு is_systemd_timer_available(வெறுமை)
{
	முழு is_available;

	எனில் (get_schedule_cmd("systemctl", &is_available, வெற்று))
		திரும்பு is_available;

	திரும்பு real_is_systemd_timer_available();
}

நிலையான எழுத்து *xdg_config_home_systemd(மாறிலி எழுத்து *filename)
{
	திரும்பு xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

நிலையான முழு systemd_timer_delete_timer_file(எண்ணிக்கை schedule_priority priority)
{
	முழு ret = 0;
	மாறிலி எழுத்து *frequency = get_frequency(priority);
	எழுத்து *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	எழுத்து *filename = xdg_config_home_systemd(local_timer_name);

	எனில் (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	விடுவி(filename);
	விடுவி(local_timer_name);
	திரும்பு ret;
}

நிலையான முழு systemd_timer_delete_service_template(வெறுமை)
{
	முழு ret = 0;
	எழுத்து *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	எழுத்து *filename = xdg_config_home_systemd(local_service_name);
	எனில் (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	விடுவி(filename);
	விடுவி(local_service_name);
	திரும்பு ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
நிலையான முழு systemd_timer_write_timer_file(எண்ணிக்கை schedule_priority schedule,
					  முழு minute)
{
	முழு res = -1;
	எழுத்து *filename;
	FILE *file;
	மாறிலி எழுத்து *unit;
	எழுத்து *schedule_pattern = வெற்று;
	மாறிலி எழுத்து *frequency = get_frequency(schedule);
	எழுத்து *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	எனில் (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		செல் error;
	}
	file = fopen_or_warn(filename, "w");
	எனில் (!file)
		செல் error;

	தேர்வு (schedule) {
	நிலை SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		நிறுத்து;

	நிலை SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		நிறுத்து;

	நிலை SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		நிறுத்து;

	இயல்பு:
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
	எனில் (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		செல் error;
	}
	எனில் (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		செல் error;
	}

	res = 0;

error:
	விடுவி(schedule_pattern);
	விடுவி(local_timer_name);
	விடுவி(filename);
	திரும்பு res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
நிலையான முழு systemd_timer_write_service_template(மாறிலி எழுத்து *exec_path)
{
	முழு res = -1;
	எழுத்து *filename;
	FILE *file;
	மாறிலி எழுத்து *unit;
	எழுத்து *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	எனில் (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		செல் error;
	}
	file = fopen_or_warn(filename, "w");
	எனில் (!file)
		செல் error;

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
	எனில் (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		செல் error;
	}
	எனில் (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		செல் error;
	}

	res = 0;

error:
	விடுவி(local_service_name);
	விடுவி(filename);
	திரும்பு res;
}

நிலையான முழு systemd_timer_enable_unit(முழு enable,
				     எண்ணிக்கை schedule_priority schedule,
				     முழு minute)
{
	எழுத்து *cmd = வெற்று;
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;
	மாறிலி எழுத்து *frequency = get_frequency(schedule);
	முழு ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	எனில் (!enable) {
		child.no_stderr = 1;
	} இல்லையெனில் எனில் (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		செல் out;
	}

	get_schedule_cmd("systemctl", வெற்று, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", வெற்று);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	எனில் (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		செல் out;
	}

	எனில் (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		எனில் (enable) {
			ret = error(_("failed to run systemctl"));
			செல் out;
		}
	}

	ret = 0;

out:
	விடுவி(cmd);
	திரும்பு ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
நிலையான வெறுமை systemd_timer_delete_stale_timer_templates(வெறுமை)
{
	எழுத்து *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	எழுத்து *filename = xdg_config_home_systemd(timer_template_name);

	எனில் (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	விடுவி(filename);
	விடுவி(timer_template_name);
}

நிலையான முழு systemd_timer_delete_unit_files(வெறுமை)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	திரும்பு systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

நிலையான முழு systemd_timer_delete_units(வெறுமை)
{
	முழு minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	திரும்பு systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

நிலையான முழு systemd_timer_setup_units(வெறுமை)
{
	முழு minute = get_random_minute();
	மாறிலி எழுத்து *exec_path = git_exec_path();

	முழு ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	எனில் (ret)
		systemd_timer_delete_units();
	இல்லையெனில்
		systemd_timer_delete_stale_timer_templates();

	திரும்பு ret;
}

நிலையான முழு systemd_timer_update_schedule(முழு run_maintenance, முழு fd UNUSED)
{
	எனில் (run_maintenance)
		திரும்பு systemd_timer_setup_units();
	இல்லையெனில்
		திரும்பு systemd_timer_delete_units();
}

எண்ணிக்கை scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

நிலையான மாறிலி கட்டமைப்பு {
	மாறிலி எழுத்து *name;
	முழு (*is_available)(வெறுமை);
	முழு (*update_schedule)(முழு run_maintenance, முழு fd);
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

நிலையான எண்ணிக்கை scheduler parse_scheduler(மாறிலி எழுத்து *மதிப்பு)
{
	எனில் (!மதிப்பு)
		திரும்பு SCHEDULER_INVALID;
	இல்லையெனில் எனில் (!strcasecmp(மதிப்பு, "auto"))
		திரும்பு SCHEDULER_AUTO;
	இல்லையெனில் எனில் (!strcasecmp(மதிப்பு, "cron") || !strcasecmp(மதிப்பு, "crontab"))
		திரும்பு SCHEDULER_CRON;
	இல்லையெனில் எனில் (!strcasecmp(மதிப்பு, "systemd") ||
		 !strcasecmp(மதிப்பு, "systemd-timer"))
		திரும்பு SCHEDULER_SYSTEMD;
	இல்லையெனில் எனில் (!strcasecmp(மதிப்பு, "launchctl"))
		திரும்பு SCHEDULER_LAUNCHCTL;
	இல்லையெனில் எனில் (!strcasecmp(மதிப்பு, "schtasks"))
		திரும்பு SCHEDULER_SCHTASKS;
	இல்லையெனில்
		திரும்பு SCHEDULER_INVALID;
}

நிலையான முழு maintenance_opt_scheduler(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
				     முழு unset)
{
	எண்ணிக்கை scheduler *scheduler = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	எனில் (*scheduler == SCHEDULER_INVALID)
		திரும்பு error(_("unrecognized --scheduler argument '%s'"), arg);
	திரும்பு 0;
}

கட்டமைப்பு maintenance_start_opts {
	எண்ணிக்கை scheduler scheduler;
};

நிலையான எண்ணிக்கை scheduler resolve_scheduler(எண்ணிக்கை scheduler scheduler)
{
	எனில் (scheduler != SCHEDULER_AUTO)
		திரும்பு scheduler;

#எனில் defined(__APPLE__)
	திரும்பு SCHEDULER_LAUNCHCTL;

#அல்லதெனில் defined(GIT_WINDOWS_NATIVE)
	திரும்பு SCHEDULER_SCHTASKS;

#அல்லதெனில் defined(__linux__)
	எனில் (is_systemd_timer_available())
		திரும்பு SCHEDULER_SYSTEMD;
	இல்லையெனில் எனில் (is_crontab_available())
		திரும்பு SCHEDULER_CRON;
	இல்லையெனில்
		die(_("neither systemd timers nor crontab are available"));

#இல்லையெனில்
	திரும்பு SCHEDULER_CRON;
#endif
}

நிலையான வெறுமை validate_scheduler(எண்ணிக்கை scheduler scheduler)
{
	எனில் (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	எனில் (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	எனில் (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

நிலையான முழு update_background_schedule(மாறிலி கட்டமைப்பு maintenance_start_opts *opts,
				      முழு enable)
{
	unsigned முழு i;
	முழு result = 0;
	கட்டமைப்பு lock_file lk;
	எழுத்து *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	எனில் (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		எனில் (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		இல்லையெனில்
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		விடுவி(lock_path);
		திரும்பு -1;
	}

	க்கு (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		எனில் (enable && opts->scheduler == i)
			தொடர்;
		எனில் (!scheduler_fn[i].is_available())
			தொடர்;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	எனில் (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	விடுவி(lock_path);
	திரும்பு result;
}

நிலையான மாறிலி எழுத்து *மாறிலி builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	வெற்று
};

நிலையான முழு maintenance_start(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			     கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு maintenance_start_opts opts = { 0 };
	கட்டமைப்பு option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	மாறிலி எழுத்து *register_args[] = { "register", வெற்று };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	எனில் (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	எனில் (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	எனில் (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, வெற்று, repo))
		warning(_("failed to add repo to global config"));
	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *மாறிலி builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	வெற்று
};

நிலையான முழு maintenance_stop(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			    கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	எனில் (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	திரும்பு update_background_schedule(வெற்று, 0);
}

நிலையான மாறிலி எழுத்து *மாறிலி builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	வெற்று
};

நிலையான முழு maintenance_is_needed(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				 கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	கட்டமைப்பு string_list selected_tasks = STRING_LIST_INIT_DUP;
	கட்டமைப்பு gc_config cfg = GC_CONFIG_INIT;
	கட்டமைப்பு option options[] = {
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
	எனில் (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	எனில் (opts.auto_flag) {
		க்கு (size_t i = 0; i < opts.tasks_nr; i++) {
			எனில் (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				நிறுத்து;
			}
		}
	} இல்லையெனில் {
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

	எனில் (is_needed)
		திரும்பு 0;
	திரும்பு 1;
}

நிலையான மாறிலி எழுத்து *மாறிலி builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	வெற்று,
};

முழு cmd_maintenance(முழு argc,
		    மாறிலி எழுத்து **argv,
		    மாறிலி எழுத்து *prefix,
		    கட்டமைப்பு repository *repo)
{
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option builtin_maintenance_options[] = {
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
	திரும்பு fn(argc, argv, prefix, repo);
}
