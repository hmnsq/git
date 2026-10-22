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

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "date.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "tempfile.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "sigchain.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "commit-graph.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "reflog.h"
#अन्तर्भाव "rerere.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "exec-cmd.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "trace2.h"
#अन्तर्भाव "worktree.h"

#define FAILED_RUN "failed to run %s"

स्थिर ध्रुव वर्ण * ध्रुव builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	रिक्त
};

स्थिर timestamp_t gc_log_expire_time;
स्थिर संरचना tempfile *pidfile;
स्थिर संरचना lock_file log_lock;
स्थिर संरचना string_list pack_garbage = STRING_LIST_INIT_DUP;

स्थिर शून्य clean_pack_garbage(शून्य)
{
	पूर्णाङ्क i;
	अत्थाय (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.वस्तूनि[i].string);
	string_list_clear(&pack_garbage, 0);
}

स्थिर शून्य report_pack_garbage(unsigned seen_bits, ध्रुव वर्ण *path)
{
	सचे (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

स्थिर शून्य process_log_file(शून्य)
{
	संरचना stat st;
	सचे (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		पूर्णाङ्क saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} अञ्ञथा सचे (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} अञ्ञथा {
		वर्ण *path = repo_git_path(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(path);
		rollback_lock_file(&log_lock);
		मुक्त(path);
	}
}

स्थिर शून्य process_log_file_at_exit(शून्य)
{
	fflush(stderr);
	process_log_file();
}

स्थिर पूर्णाङ्क gc_config_is_timestamp_never(ध्रुव वर्ण *var)
{
	ध्रुव वर्ण *मानम्;
	timestamp_t expire;

	सचे (!repo_config_get_मानम्(the_repository, var, &मानम्) && मानम्) {
		सचे (parse_expiry_date(मानम्, &expire))
			die(_("failed to parse '%s' value '%s'"), var, मानम्);
		पटिनिवत्त expire == 0;
	}
	पटिनिवत्त 0;
}

संरचना gc_config {
	पूर्णाङ्क pack_refs;
	पूर्णाङ्क prune_reflogs;
	पूर्णाङ्क cruft_packs;
	unsigned long max_cruft_size;
	पूर्णाङ्क aggressive_depth;
	पूर्णाङ्क aggressive_window;
	पूर्णाङ्क detach_auto;
	वर्ण *gc_log_expire;
	वर्ण *prune_expire;
	वर्ण *prune_worktrees_expire;
	वर्ण *repack_expire_to;
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

स्थिर शून्य gc_config_release(संरचना gc_config *cfg)
{
	मुक्त(cfg->gc_log_expire);
	मुक्त(cfg->prune_expire);
	मुक्त(cfg->prune_worktrees_expire);
}

स्थिर शून्य gc_config(संरचना gc_config *cfg)
{
	ध्रुव वर्ण *मानम्;
	वर्ण *owned = रिक्त;

	सचे (!repo_config_get_मानम्(the_repository, "gc.packrefs", &मानम्)) {
		सचे (मानम् && !strcmp(मानम्, "notbare"))
			cfg->pack_refs = -1;
		अञ्ञथा
			cfg->pack_refs = git_config_bool("gc.packrefs", मानम्);
	}

	सचे (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	सचे (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		मुक्त(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	सचे (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		मुक्त(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	सचे (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		मुक्त(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, रिक्त);
}

गणना schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

स्थिर गणना schedule_priority parse_schedule(ध्रुव वर्ण *मानम्)
{
	सचे (!मानम्)
		पटिनिवत्त SCHEDULE_NONE;
	सचे (!strcasecmp(मानम्, "hourly"))
		पटिनिवत्त SCHEDULE_HOURLY;
	सचे (!strcasecmp(मानम्, "daily"))
		पटिनिवत्त SCHEDULE_DAILY;
	सचे (!strcasecmp(मानम्, "weekly"))
		पटिनिवत्त SCHEDULE_WEEKLY;
	पटिनिवत्त SCHEDULE_NONE;
}

गणना maintenance_task_label {
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
	TASK_गणनम्
};

संरचना maintenance_run_opts {
	गणना maintenance_task_label *tasks;
	size_t tasks_nr, tasks_alloc;
	पूर्णाङ्क auto_flag;
	पूर्णाङ्क detach;
	पूर्णाङ्क quiet;
	गणना schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

स्थिर शून्य maintenance_run_opts_release(संरचना maintenance_run_opts *opts)
{
	मुक्त(opts->tasks);
}

स्थिर पूर्णाङ्क pack_refs_condition(UNUSED संरचना gc_config *cfg)
{
	संरचना string_list included_refs = STRING_LIST_INIT_NODUP;
	संरचना ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	संरचना refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	सचे (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		पटिनिवत्त 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	पटिनिवत्त required;
}

स्थिर पूर्णाङ्क maintenance_task_pack_refs(संरचना maintenance_run_opts *opts,
				      UNUSED संरचना gc_config *cfg)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", रिक्त);
	सचे (opts->auto_flag)
		strvec_प्रक्षिप(&cmd.args, "--auto");

	पटिनिवत्त run_command(&cmd);
}

संरचना गणनम्_reflog_entries_data {
	संरचना expire_reflog_policy_cb policy;
	size_t गणनम्;
	size_t limit;
};

स्थिर पूर्णाङ्क गणनम्_reflog_entries(ध्रुव वर्ण *refname UNUSED,
				संरचना object_id *old_oid, संरचना object_id *new_oid,
				ध्रुव वर्ण *committer, timestamp_t timestamp,
				पूर्णाङ्क tz, ध्रुव वर्ण *सन्देशः, शून्य *cb_data)
{
	संरचना गणनम्_reflog_entries_data *data = cb_data;
	सचे (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, सन्देशः, &data->policy))
		data->गणनम्++;
	पटिनिवत्त data->गणनम् >= data->limit;
}

स्थिर पूर्णाङ्क reflog_expire_condition(संरचना gc_config *cfg UNUSED)
{
	timestamp_t now = time(रिक्त);
	संरचना गणनम्_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	पूर्णाङ्क limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	सचे (!limit)
		पटिनिवत्त 0;
	सचे (limit < 0)
		पटिनिवत्त 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 गणनम्_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	पटिनिवत्त data.गणनम् >= data.limit;
}

स्थिर पूर्णाङ्क maintenance_task_reflog_expire(संरचना maintenance_run_opts *opts UNUSED,
					  संरचना gc_config *cfg UNUSED)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", रिक्त);
	पटिनिवत्त run_command(&cmd);
}

स्थिर पूर्णाङ्क maintenance_task_worktree_prune(संरचना maintenance_run_opts *opts UNUSED,
					   संरचना gc_config *cfg)
{
	संरचना child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", रिक्त);
	strvec_प्रक्षिप(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	पटिनिवत्त run_command(&prune_worktrees_cmd);
}

स्थिर पूर्णाङ्क worktree_prune_condition(संरचना gc_config *cfg)
{
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	संरचना dirent *d;
	DIR *dir = रिक्त;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	सचे (limit <= 0) {
		should_prune = limit < 0;
		गच्छ out;
	}

	सचे (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		गच्छ out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	सचे (!dir)
		गच्छ out;

	याव (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		वर्ण *wtpath;
		strbuf_reset(&buf);
		सचे (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		मुक्त(wtpath);
	}

	should_prune = !limit;

out:
	सचे (dir)
		closedir(dir);
	strbuf_release(&buf);
	पटिनिवत्त should_prune;
}

स्थिर पूर्णाङ्क maintenance_task_rerere_gc(संरचना maintenance_run_opts *opts UNUSED,
				      संरचना gc_config *cfg UNUSED)
{
	संरचना child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", रिक्त);
	पटिनिवत्त run_command(&rerere_cmd);
}

स्थिर पूर्णाङ्क rerere_gc_condition(संरचना gc_config *cfg UNUSED)
{
	पूर्णाङ्क limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	सचे (!limit)
		पटिनिवत्त 0; /* never prune */
	सचे (limit < 0)
		पटिनिवत्त 1; /* always prune */

	पटिनिवत्त rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
स्थिर ध्रुव वर्ण *lock_repo_for_gc(पूर्णाङ्क force, pid_t* ret_pid)
{
	संरचना lock_file lock = LOCK_INIT;
	वर्ण my_host[HOST_NAME_MAX + 1];
	संरचना strbuf sb = STRBUF_INIT;
	संरचना stat st;
	uintmax_t pid;
	FILE *fp;
	पूर्णाङ्क fd;
	वर्ण *pidfile_path;

	सचे (is_tempfile_active(pidfile))
		/* already locked */
		पटिनिवत्त रिक्त;

	सचे (xgethostname(my_host, परिमाण(my_host)))
		xsnprintf(my_host, परिमाण(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	सचे (!force) {
		स्थिर वर्ण locking_host[HOST_NAME_MAX + 1];
		स्थिर वर्ण *scan_fmt;
		पूर्णाङ्क should_exit;

		सचे (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_path, "r");
		memset(locking_host, 0, परिमाण(locking_host));
		should_exit =
			fp != रिक्त &&
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
			time(रिक्त) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, scan_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		सचे (fp)
			fclose(fp);
		सचे (should_exit) {
			सचे (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			मुक्त(pidfile_path);
			पटिनिवत्त locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	मुक्त(pidfile_path);
	पटिनिवत्त रिक्त;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
स्थिर पूर्णाङ्क report_last_gc_error(शून्य)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क ret = 0;
	ssize_t len;
	संरचना stat st;
	वर्ण *gc_log_path = repo_git_path(the_repository, "gc.log");

	सचे (stat(gc_log_path, &st)) {
		सचे (errno == ENOENT)
			गच्छ done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		गच्छ done;
	}

	सचे (st.st_mtime < gc_log_expire_time)
		गच्छ done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	सचे (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	अञ्ञथा सचे (len > 0) {
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
	मुक्त(gc_log_path);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क gc_foreground_tasks(संरचना maintenance_run_opts *opts,
			       संरचना gc_config *cfg)
{
	सचे (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		पटिनिवत्त error(FAILED_RUN, "pack-refs");
	सचे (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		पटिनिवत्त error(FAILED_RUN, "reflog");
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क maintenance_task_odb(संरचना maintenance_run_opts *opts,
				संरचना gc_config *cfg,
				पूर्णाङ्क keep_largest_pack,
				पूर्णाङ्क aggressive)
{
	संरचना odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	सचे (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	सचे (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	सचे (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	पटिनिवत्त odb_optimize(the_repository->objects, &odb_opts);
}

पूर्णाङ्क cmd_gc(पूर्णाङ्क argc,
	   ध्रुव वर्ण **argv,
	   ध्रुव वर्ण *prefix,
	   संरचना repository *repo UNUSED)
{
	पूर्णाङ्क aggressive = 0;
	पूर्णाङ्क force = 0;
	ध्रुव वर्ण *name;
	pid_t pid;
	पूर्णाङ्क daemonized = 0;
	पूर्णाङ्क keep_largest_pack = -1;
	पूर्णाङ्क skip_foreground_tasks = 0;
	timestamp_t dummy;
	संरचना maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	संरचना gc_config cfg = GC_CONFIG_INIT;
	ध्रुव वर्ण *prune_expire_sentinel = "sentinel";
	ध्रुव वर्ण *prune_expire_arg = prune_expire_sentinel;
	पूर्णाङ्क ret;
	संरचना option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.मानम् = &prune_expire_arg,
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

	सचे (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	सचे (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	सचे (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	सचे (prune_expire_arg != prune_expire_sentinel) {
		मुक्त(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	सचे (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	सचे (opts.auto_flag) {
		संरचना odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		सचे (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		सचे (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			गच्छ out;
		}

		सचे (!opts.quiet) {
			सचे (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			अञ्ञथा
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	सचे (opts.detach > 0) {
		ret = report_last_gc_error();
		सचे (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			गच्छ out;

		} अञ्ञथा सचे (ret) {
			/* an I/O error occurred, already reported */
			गच्छ out;
		}

		सचे (!skip_foreground_tasks) {
			सचे (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				गच्छ out;
			}

			सचे (gc_foreground_tasks(&opts, &cfg) < 0)
				die(रिक्त);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = lock_repo_for_gc(force, &pid);
	सचे (name) {
		सचे (opts.auto_flag) {
			ret = 0;
			गच्छ out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	सचे (daemonized) {
		वर्ण *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		मुक्त(path);
	}

	सचे (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	सचे (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	सचे (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	सचे (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(रिक्त);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	सचे (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	सचे (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     रिक्त);

	सचे (!daemonized) {
		वर्ण *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		मुक्त(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण *ध्रुव builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_opt_schedule(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				    पूर्णाङ्क unset)
{
	गणना schedule_priority *priority = opt->मानम्;

	सचे (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	सचे (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	पटिनिवत्त 0;
}

संरचना cg_auto_data {
	पूर्णाङ्क अङ्कः_not_in_graph;
	पूर्णाङ्क limit;
};

स्थिर पूर्णाङ्क dfs_on_ref(ध्रुव संरचना reference *ref, शून्य *cb_data)
{
	संरचना cg_auto_data *data = (संरचना cg_auto_data *)cb_data;
	पूर्णाङ्क result = 0;
	ध्रुव संरचना object_id *maybe_peeled = ref->oid;
	संरचना object_id peeled;
	संरचना commit_list *राशिः = रिक्त;
	संरचना commit *commit;

	सचे (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	सचे (odb_read_object_info(the_repository->objects, maybe_peeled, रिक्त) != OBJ_COMMIT)
		पटिनिवत्त 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	सचे (!commit || commit->object.flags & SEEN)
		पटिनिवत्त 0;
	commit->object.flags |= SEEN;

	सचे (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		पटिनिवत्त 0;

	data->अङ्कः_not_in_graph++;

	सचे (data->अङ्कः_not_in_graph >= data->limit)
		पटिनिवत्त 1;

	commit_list_निवेशय(commit, &राशिः);

	याव (!result && राशिः) {
		संरचना commit_list *parent;

		commit = निष्कासय_commit(&राशिः);

		अत्थाय (parent = commit->parents; parent; parent = parent->next) {
			सचे (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				अनुवर्त;

			parent->item->object.flags |= SEEN;
			data->अङ्कः_not_in_graph++;

			सचे (data->अङ्कः_not_in_graph >= data->limit) {
				result = 1;
				भङ्ग;
			}

			commit_list_निवेशय(parent->item, &राशिः);
		}
	}

	commit_list_free(राशिः);
	पटिनिवत्त result;
}

स्थिर पूर्णाङ्क should_write_commit_graph(संरचना gc_config *cfg UNUSED)
{
	पूर्णाङ्क result;
	संरचना cg_auto_data data;

	data.अङ्कः_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	सचे (!data.limit)
		पटिनिवत्त 0;
	सचे (data.limit < 0)
		पटिनिवत्त 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	पटिनिवत्त result;
}

स्थिर पूर्णाङ्क run_write_commit_graph(संरचना maintenance_run_opts *opts)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", रिक्त);

	सचे (opts->quiet)
		strvec_प्रक्षिप(&child.args, "--no-progress");
	अञ्ञथा
		strvec_प्रक्षिप(&child.args, "--progress");

	पटिनिवत्त !!run_command(&child);
}

स्थिर पूर्णाङ्क maintenance_task_commit_graph(संरचना maintenance_run_opts *opts,
					 संरचना gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	सचे (!the_repository->settings.core_commit_graph)
		पटिनिवत्त 0;

	सचे (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		पटिनिवत्त 1;
	}

	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क fetch_remote(संरचना remote *remote, शून्य *cbdata)
{
	संरचना maintenance_run_opts *opts = cbdata;
	संरचना child_process child = CHILD_PROCESS_INIT;

	सचे (remote->skip_default_update)
		पटिनिवत्त 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     रिक्त);

	सचे (opts->quiet)
		strvec_प्रक्षिप(&child.args, "--quiet");

	पटिनिवत्त !!run_command(&child);
}

स्थिर पूर्णाङ्क maintenance_task_prefetch(संरचना maintenance_run_opts *opts,
				     संरचना gc_config *cfg UNUSED)
{
	सचे (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		पटिनिवत्त 1;
	}

	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क maintenance_task_gc_foreground(संरचना maintenance_run_opts *opts,
					  संरचना gc_config *cfg)
{
	पटिनिवत्त gc_foreground_tasks(opts, cfg);
}

स्थिर पूर्णाङ्क maintenance_task_gc_background(संरचना maintenance_run_opts *opts,
					  संरचना gc_config *cfg UNUSED)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_प्रक्षिप(&child.args, "gc");

	सचे (opts->auto_flag)
		strvec_प्रक्षिप(&child.args, "--auto");
	सचे (opts->quiet)
		strvec_प्रक्षिप(&child.args, "--quiet");
	अञ्ञथा
		strvec_प्रक्षिप(&child.args, "--no-quiet");
	strvec_प्रक्षिप(&child.args, "--no-detach");
	strvec_प्रक्षिप(&child.args, "--skip-foreground-tasks");

	पटिनिवत्त run_command(&child);
}

स्थिर पूर्णाङ्क gc_condition(संरचना gc_config *cfg)
{
	संरचना odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	पटिनिवत्त odb_optimize_required(the_repository->objects, &opts);
}

स्थिर पूर्णाङ्क prune_packed(संरचना maintenance_run_opts *opts)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_प्रक्षिप(&child.args, "prune-packed");

	सचे (opts->quiet)
		strvec_प्रक्षिप(&child.args, "--quiet");

	पटिनिवत्त !!run_command(&child);
}

संरचना write_loose_object_data {
	FILE *in;
	पूर्णाङ्क गणनम्;
	पूर्णाङ्क batch_size;
};

स्थिर पूर्णाङ्क loose_object_auto_limit = 100;

स्थिर पूर्णाङ्क loose_object_गणनम्(ध्रुव संरचना object_id *oid UNUSED,
			      ध्रुव वर्ण *path UNUSED,
			      शून्य *data)
{
	पूर्णाङ्क *गणनम् = (पूर्णाङ्क*)data;
	सचे (++(*गणनम्) >= loose_object_auto_limit)
		पटिनिवत्त 1;
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क loose_object_auto_condition(संरचना gc_config *cfg UNUSED)
{
	पूर्णाङ्क गणनम् = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	सचे (!loose_object_auto_limit)
		पटिनिवत्त 0;
	सचे (loose_object_auto_limit < 0)
		पटिनिवत्त 1;

	पटिनिवत्त for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_गणनम्,
					     रिक्त, रिक्त, &गणनम्);
}

स्थिर पूर्णाङ्क bail_on_loose(ध्रुव संरचना object_id *oid UNUSED,
			 ध्रुव वर्ण *path UNUSED,
			 शून्य *data UNUSED)
{
	पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क write_loose_object_to_stdin(ध्रुव संरचना object_id *oid,
				       ध्रुव वर्ण *path UNUSED,
				       शून्य *data)
{
	संरचना write_loose_object_data *d = (संरचना write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	पटिनिवत्त ++(d->गणनम्) > d->batch_size;
}

स्थिर पूर्णाङ्क pack_loose(संरचना maintenance_run_opts *opts)
{
	संरचना repository *r = the_repository;
	पूर्णाङ्क result = 0;
	संरचना write_loose_object_data data;
	संरचना child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	सचे (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   रिक्त, रिक्त, रिक्त))
		पटिनिवत्त 0;

	pack_proc.git_cmd = 1;

	strvec_प्रक्षिप(&pack_proc.args, "pack-objects");
	सचे (opts->quiet)
		strvec_प्रक्षिप(&pack_proc.args, "--quiet");
	अञ्ञथा
		strvec_प्रक्षिप(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	सचे (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		पटिनिवत्त 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.गणनम् = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	सचे (!data.batch_size)
		data.batch_size = INT_MAX;
	अञ्ञथा सचे (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      रिक्त, रिक्त, &data);

	fclose(data.in);

	सचे (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	पटिनिवत्त result;
}

स्थिर पूर्णाङ्क maintenance_task_loose_objects(संरचना maintenance_run_opts *opts,
					  संरचना gc_config *cfg UNUSED)
{
	पटिनिवत्त prune_packed(opts) || pack_loose(opts);
}

स्थिर पूर्णाङ्क incremental_repack_auto_condition(संरचना gc_config *cfg UNUSED)
{
	संरचना packed_git *p;
	पूर्णाङ्क incremental_repack_auto_limit = 10;
	पूर्णाङ्क गणनम् = 0;

	prepare_repo_settings(the_repository);
	सचे (!the_repository->settings.core_multi_pack_index)
		पटिनिवत्त 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	सचे (!incremental_repack_auto_limit)
		पटिनिवत्त 0;
	सचे (incremental_repack_auto_limit < 0)
		पटिनिवत्त 1;

	repo_for_each_pack(the_repository, p) {
		सचे (गणनम् >= incremental_repack_auto_limit)
			भङ्ग;
		सचे (!p->multi_pack_index)
			गणनम्++;
	}

	पटिनिवत्त गणनम् >= incremental_repack_auto_limit;
}

स्थिर पूर्णाङ्क multi_pack_index_write(संरचना maintenance_run_opts *opts)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", रिक्त);

	सचे (opts->quiet)
		strvec_प्रक्षिप(&child.args, "--no-progress");
	अञ्ञथा
		strvec_प्रक्षिप(&child.args, "--progress");

	सचे (run_command(&child))
		पटिनिवत्त error(_("failed to write multi-pack-index"));

	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क multi_pack_index_expire(संरचना maintenance_run_opts *opts)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", रिक्त);

	सचे (opts->quiet)
		strvec_प्रक्षिप(&child.args, "--no-progress");
	अञ्ञथा
		strvec_प्रक्षिप(&child.args, "--progress");

	सचे (run_command(&child))
		पटिनिवत्त error(_("'git multi-pack-index expire' failed"));

	पटिनिवत्त 0;
}

#define TWO_GIGABYTES (INT32_MAX)

स्थिर off_t get_auto_pack_size(शून्य)
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
	संरचना packed_git *p;
	संरचना repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		सचे (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} अञ्ञथा सचे (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	सचे (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	पटिनिवत्त result_size;
}

स्थिर पूर्णाङ्क multi_pack_index_repack(संरचना maintenance_run_opts *opts)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", रिक्त);

	सचे (opts->quiet)
		strvec_प्रक्षिप(&child.args, "--no-progress");
	अञ्ञथा
		strvec_प्रक्षिप(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	सचे (run_command(&child))
		पटिनिवत्त error(_("'git multi-pack-index repack' failed"));

	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क maintenance_task_incremental_repack(संरचना maintenance_run_opts *opts,
					       संरचना gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	सचे (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		पटिनिवत्त 0;
	}

	सचे (multi_pack_index_write(opts))
		पटिनिवत्त 1;
	सचे (multi_pack_index_expire(opts))
		पटिनिवत्त 1;
	सचे (multi_pack_index_repack(opts))
		पटिनिवत्त 1;
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क maintenance_task_geometric_repack(संरचना maintenance_run_opts *opts,
					     संरचना gc_config *cfg)
{
	संरचना odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	सचे (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	पटिनिवत्त odb_optimize(the_repository->objects, &odb_opts);
}

स्थिर पूर्णाङ्क geometric_repack_auto_condition(संरचना gc_config *cfg)
{
	संरचना odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	पटिनिवत्त odb_optimize_required(the_repository->objects, &opts);
}

प्रकारनिर्देश पूर्णाङ्क (*maintenance_task_fn)(संरचना maintenance_run_opts *opts,
				   संरचना gc_config *cfg);
प्रकारनिर्देश पूर्णाङ्क (*maintenance_auto_fn)(संरचना gc_config *cfg);

संरचना maintenance_task {
	ध्रुव वर्ण *name;

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

स्थिर ध्रुव संरचना maintenance_task tasks[] = {
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

गणना task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

गणना auto_gc_hook_result {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

स्थिर पूर्णाङ्क maybe_run_task(ध्रुव संरचना maintenance_task *task,
			  संरचना repository *repo,
			  संरचना maintenance_run_opts *opts,
			  संरचना gc_config *cfg,
			  गणना task_phase phase,
			  गणना auto_gc_hook_result *auto_gc_hook_result)
{
	पूर्णाङ्क foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ध्रुव वर्ण *region = foreground ? "maintenance foreground" : "maintenance";
	पूर्णाङ्क ret = 0;

	सचे (!fn)
		पटिनिवत्त 0;
	सचे (opts->auto_flag) {
		सचे (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			पटिनिवत्त 0;

		सचे (!task->auto_condition || !task->auto_condition(cfg))
			पटिनिवत्त 0;

		सचे (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		सचे (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			पटिनिवत्त 0;
	}

	trace2_region_enter(region, task->name, repo);
	सचे (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क maintenance_run_tasks(संरचना maintenance_run_opts *opts,
				 संरचना gc_config *cfg)
{
	पूर्णाङ्क result = 0;
	संरचना lock_file lk;
	संरचना repository *r = the_repository;
	वर्ण *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	गणना auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	सचे (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		सचे (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		मुक्त(lock_path);
		पटिनिवत्त 0;
	}
	मुक्त(lock_path);

	अत्थाय (size_t i = 0; i < opts->tasks_nr; i++)
		सचे (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	सचे (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	अत्थाय (size_t i = 0; i < opts->tasks_nr; i++)
		सचे (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	पटिनिवत्त result;
}

गणना maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

संरचना maintenance_strategy {
	संरचना {
		unsigned type;
		गणना schedule_priority schedule;
	} tasks[TASK_गणनम्];
};

स्थिर ध्रुव संरचना maintenance_strategy none_strategy = { 0 };

स्थिर ध्रुव संरचना maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

स्थिर ध्रुव संरचना maintenance_strategy incremental_strategy = {
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

स्थिर ध्रुव संरचना maintenance_strategy geometric_strategy = {
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

स्थिर संरचना maintenance_strategy parse_maintenance_strategy(ध्रुव वर्ण *name)
{
	सचे (!strcasecmp(name, "incremental"))
		पटिनिवत्त incremental_strategy;
	सचे (!strcasecmp(name, "gc"))
		पटिनिवत्त gc_strategy;
	सचे (!strcasecmp(name, "geometric"))
		पटिनिवत्त geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

स्थिर शून्य initialize_task_config(संरचना maintenance_run_opts *opts,
				   ध्रुव संरचना string_list *selected_tasks)
{
	संरचना strbuf config_name = STRBUF_INIT;
	संरचना maintenance_strategy strategy;
	गणना maintenance_type type;
	ध्रुव वर्ण *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	सचे (selected_tasks->nr) {
		अत्थाय (size_t i = 0; i < selected_tasks->nr; i++) {
			गणना maintenance_task_label label = (intptr_t)selected_tasks->वस्तूनि[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		पटिनिवत्त;
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
	सचे (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} अञ्ञथा {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	सचे (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	अत्थाय (size_t i = 0; i < TASK_गणनम्; i++) {
		पूर्णाङ्क config_मानम्;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		सचे (!repo_config_get_bool(the_repository, config_name.buf, &config_मानम्))
			strategy.tasks[i].type = config_मानम् ? type : 0;
		सचे (!(strategy.tasks[i].type & type))
			अनुवर्त;

		सचे (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			सचे (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			सचे (strategy.tasks[i].schedule < opts->schedule)
				अनुवर्त;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

स्थिर पूर्णाङ्क task_option_parse(ध्रुव संरचना option *opt,
			     ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना string_list *selected_tasks = opt->मानम्;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	अत्थाय (i = 0; i < TASK_गणनम्; i++)
		सचे (!strcasecmp(tasks[i].name, arg))
			भङ्ग;
	सचे (i >= TASK_गणनम्) {
		error(_("'%s' is not a valid task"), arg);
		पटिनिवत्त 1;
	}

	सचे (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		पटिनिवत्त 1;
	}

	string_list_append(selected_tasks, arg)->util = (शून्य *)(intptr_t)i;

	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क maintenance_run(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			   संरचना repository *repo UNUSED)
{
	संरचना maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	संरचना string_list selected_tasks = STRING_LIST_INIT_DUP;
	संरचना gc_config cfg = GC_CONFIG_INIT;
	संरचना option builtin_maintenance_run_options[] = {
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
	पूर्णाङ्क ret;

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

	सचे (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	पटिनिवत्त ret;
}

स्थिर वर्ण *get_maintpath(शून्य)
{
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	पटिनिवत्त strbuf_detach(&sb, रिक्त);
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_register(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
				संरचना repository *repo UNUSED)
{
	वर्ण *config_file = रिक्त;
	संरचना option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	पूर्णाङ्क found = 0;
	ध्रुव वर्ण *कुञ्चिका = "maintenance.repo";
	वर्ण *maintpath = get_maintpath();
	संरचना string_list_item *item;
	ध्रुव संरचना string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	सचे (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	सचे (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	सचे (!repo_config_get_string_multi(the_repository, कुञ्चिका, &list)) {
		for_each_string_list_item(item, list) {
			सचे (!strcmp(maintpath, item->string)) {
				found = 1;
				भङ्ग;
			}
		}
	}

	सचे (!found) {
		पूर्णाङ्क rc;
		वर्ण *global_config_file = रिक्त;

		सचे (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		सचे (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, रिक्त, 0);
		मुक्त(global_config_file);

		सचे (rc)
			die(_("unable to add '%s' value of '%s'"),
			    कुञ्चिका, maintpath);
	}

	मुक्त(maintpath);
	पटिनिवत्त 0;
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_unregister(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
				  संरचना repository *repo UNUSED)
{
	पूर्णाङ्क force = 0;
	वर्ण *config_file = रिक्त;
	संरचना option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	ध्रुव वर्ण *कुञ्चिका = "maintenance.repo";
	वर्ण *maintpath = get_maintpath();
	पूर्णाङ्क found = 0;
	संरचना string_list_item *item;
	ध्रुव संरचना string_list *list;
	संरचना config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	सचे (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	सचे (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	सचे (!(config_file
	      ? git_configset_get_string_multi(&cs, कुञ्चिका, &list)
	      : repo_config_get_string_multi(the_repository, कुञ्चिका, &list))) {
		for_each_string_list_item(item, list) {
			सचे (!strcmp(maintpath, item->string)) {
				found = 1;
				भङ्ग;
			}
		}
	}

	सचे (found) {
		पूर्णाङ्क rc;
		वर्ण *global_config_file = रिक्त;

		सचे (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		सचे (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, कुञ्चिका, रिक्त, maintpath, रिक्त,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_मानम्);
		मुक्त(global_config_file);

		सचे (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    कुञ्चिका, maintpath);
	} अञ्ञथा सचे (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	मुक्त(maintpath);
	पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण *get_frequency(गणना schedule_priority schedule)
{
	विकल्प (schedule) {
	पक्ष SCHEDULE_HOURLY:
		पटिनिवत्त "hourly";
	पक्ष SCHEDULE_DAILY:
		पटिनिवत्त "daily";
	पक्ष SCHEDULE_WEEKLY:
		पटिनिवत्त "weekly";
	स्वभाव:
		BUG("invalid schedule %d", schedule);
	}
}

स्थिर ध्रुव वर्ण *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	रिक्त
};

स्थिर ध्रुव वर्ण *get_extra_config_parameters(शून्य) {
	स्थिर ध्रुव वर्ण *result = रिक्त;
	संरचना strbuf builder = STRBUF_INIT;

	सचे (result)
		पटिनिवत्त result;

	अत्थाय (ध्रुव वर्ण **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, रिक्त);
	पटिनिवत्त result;
}

स्थिर ध्रुव वर्ण *get_extra_launchctl_strings(शून्य) {
	स्थिर ध्रुव वर्ण *result = रिक्त;
	संरचना strbuf builder = STRBUF_INIT;

	सचे (result)
		पटिनिवत्त result;

	अत्थाय (ध्रुव वर्ण **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, रिक्त);
	पटिनिवत्त result;
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
स्थिर पूर्णाङ्क get_schedule_cmd(ध्रुव वर्ण *cmd, पूर्णाङ्क *is_available, वर्ण **out)
{
	वर्ण *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	संरचना string_list_item *item;
	संरचना string_list list = STRING_LIST_INIT_NODUP;

	सचे (!testing) {
		सचे (out)
			*out = xstrdup(cmd);
		पटिनिवत्त 0;
	}

	सचे (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		संरचना string_list pair = STRING_LIST_INIT_NODUP;

		सचे (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			अनुवर्त;

		सचे (!strcmp(cmd, pair.वस्तूनि[0].string)) {
			सचे (out)
				*out = xstrdup(pair.वस्तूनि[1].string);
			सचे (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			गच्छ out;
		}

		string_list_clear(&pair, 0);
	}

	सचे (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	मुक्त(testing);
	पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क get_random_minute(शून्य)
{
	/* Use a static value when under tests. */
	सचे (getenv("GIT_TEST_MAINT_SCHEDULER"))
		पटिनिवत्त 13;

	पटिनिवत्त git_rand(0) % 60;
}

स्थिर पूर्णाङ्क is_launchctl_available(शून्य)
{
	पूर्णाङ्क is_available;
	सचे (get_schedule_cmd("launchctl", &is_available, रिक्त))
		पटिनिवत्त is_available;

#ifdef __APPLE__
	पटिनिवत्त 1;
#अञ्ञथा
	पटिनिवत्त 0;
#endif
}

स्थिर वर्ण *launchctl_service_name(ध्रुव वर्ण *frequency)
{
	संरचना strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	पटिनिवत्त strbuf_detach(&label, रिक्त);
}

स्थिर वर्ण *launchctl_service_filename(ध्रुव वर्ण *name)
{
	वर्ण *expanded;
	संरचना strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	सचे (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	पटिनिवत्त expanded;
}

स्थिर वर्ण *launchctl_get_uid(शून्य)
{
	पटिनिवत्त xstrfmt("gui/%d", getuid());
}

स्थिर पूर्णाङ्क launchctl_boot_plist(पूर्णाङ्क enable, ध्रुव वर्ण *filename)
{
	वर्ण *cmd;
	पूर्णाङ्क result;
	संरचना child_process child = CHILD_PROCESS_INIT;
	वर्ण *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", रिक्त, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, रिक्त);

	child.no_stderr = 1;
	child.no_stdout = 1;

	सचे (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	मुक्त(cmd);
	मुक्त(uid);
	पटिनिवत्त result;
}

स्थिर पूर्णाङ्क launchctl_remove_plist(गणना schedule_priority schedule)
{
	ध्रुव वर्ण *frequency = get_frequency(schedule);
	वर्ण *name = launchctl_service_name(frequency);
	वर्ण *filename = launchctl_service_filename(name);
	पूर्णाङ्क result = launchctl_boot_plist(0, filename);
	unlink(filename);
	मुक्त(filename);
	मुक्त(name);
	पटिनिवत्त result;
}

स्थिर पूर्णाङ्क launchctl_remove_plists(शून्य)
{
	पटिनिवत्त launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

स्थिर पूर्णाङ्क launchctl_list_contains_plist(ध्रुव वर्ण *name, ध्रुव वर्ण *cmd)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, रिक्त);

	child.no_stderr = 1;
	child.no_stdout = 1;

	सचे (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	पटिनिवत्त !finish_command(&child);
}

स्थिर पूर्णाङ्क launchctl_schedule_plist(ध्रुव वर्ण *exec_path, गणना schedule_priority schedule)
{
	पूर्णाङ्क i, fd;
	ध्रुव वर्ण *preamble, *repeat;
	ध्रुव वर्ण *frequency = get_frequency(schedule);
	वर्ण *name = launchctl_service_name(frequency);
	वर्ण *filename = launchctl_service_filename(name);
	संरचना lock_file lk = LOCK_INIT;
	स्थिर unsigned long lock_file_timeout_ms = ULONG_MAX;
	संरचना strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	संरचना stat st;
	वर्ण *cmd;
	पूर्णाङ्क minute = get_random_minute();

	get_schedule_cmd("launchctl", रिक्त, &cmd);
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

	विकल्प (schedule) {
	पक्ष SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		अत्थाय (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		भङ्ग;

	पक्ष SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		अत्थाय (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		भङ्ग;

	पक्ष SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		भङ्ग;

	स्वभाव:
		/* unreachable */
		भङ्ग;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	सचे (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	सचे ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	सचे (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	अञ्ञथा {
		सचे (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		सचे (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	मुक्त(filename);
	मुक्त(name);
	मुक्त(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क launchctl_add_plists(शून्य)
{
	ध्रुव वर्ण *exec_path = git_exec_path();

	पटिनिवत्त launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

स्थिर पूर्णाङ्क launchctl_update_schedule(पूर्णाङ्क run_maintenance, पूर्णाङ्क fd UNUSED)
{
	सचे (run_maintenance)
		पटिनिवत्त launchctl_add_plists();
	अञ्ञथा
		पटिनिवत्त launchctl_remove_plists();
}

स्थिर पूर्णाङ्क is_schtasks_available(शून्य)
{
	पूर्णाङ्क is_available;
	सचे (get_schedule_cmd("schtasks", &is_available, रिक्त))
		पटिनिवत्त is_available;

#ifdef GIT_WINDOWS_NATIVE
	पटिनिवत्त 1;
#अञ्ञथा
	पटिनिवत्त 0;
#endif
}

स्थिर वर्ण *schtasks_task_name(ध्रुव वर्ण *frequency)
{
	संरचना strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	पटिनिवत्त strbuf_detach(&label, रिक्त);
}

स्थिर पूर्णाङ्क schtasks_remove_task(गणना schedule_priority schedule)
{
	वर्ण *cmd;
	संरचना child_process child = CHILD_PROCESS_INIT;
	ध्रुव वर्ण *frequency = get_frequency(schedule);
	वर्ण *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", रिक्त, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", रिक्त);
	मुक्त(name);
	मुक्त(cmd);

	पटिनिवत्त run_command(&child);
}

स्थिर पूर्णाङ्क schtasks_remove_tasks(शून्य)
{
	पटिनिवत्त schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

स्थिर पूर्णाङ्क schtasks_schedule_task(ध्रुव वर्ण *exec_path, गणना schedule_priority schedule)
{
	वर्ण *cmd;
	पूर्णाङ्क result;
	संरचना child_process child = CHILD_PROCESS_INIT;
	ध्रुव वर्ण *xml;
	संरचना tempfile *tfile;
	ध्रुव वर्ण *frequency = get_frequency(schedule);
	वर्ण *name = schtasks_task_name(frequency);
	संरचना strbuf tfilename = STRBUF_INIT;
	पूर्णाङ्क minute = get_random_minute();

	get_schedule_cmd("schtasks", रिक्त, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	सचे (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	विकल्प (schedule) {
	पक्ष SCHEDULE_HOURLY:
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
		भङ्ग;

	पक्ष SCHEDULE_DAILY:
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
		भङ्ग;

	पक्ष SCHEDULE_WEEKLY:
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
		भङ्ग;

	स्वभाव:
		भङ्ग;
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
				  get_tempfile_path(tfile), रिक्त);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	सचे (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	मुक्त(name);
	मुक्त(cmd);
	पटिनिवत्त result;
}

स्थिर पूर्णाङ्क schtasks_schedule_tasks(शून्य)
{
	ध्रुव वर्ण *exec_path = git_exec_path();

	पटिनिवत्त schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

स्थिर पूर्णाङ्क schtasks_update_schedule(पूर्णाङ्क run_maintenance, पूर्णाङ्क fd UNUSED)
{
	सचे (run_maintenance)
		पटिनिवत्त schtasks_schedule_tasks();
	अञ्ञथा
		पटिनिवत्त schtasks_remove_tasks();
}

MAYBE_UNUSED
स्थिर पूर्णाङ्क check_crontab_process(ध्रुव वर्ण *cmd)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_प्रक्षिप(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	सचे (start_command(&child))
		पटिनिवत्त 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क is_crontab_available(शून्य)
{
	वर्ण *cmd;
	पूर्णाङ्क is_available;
	पूर्णाङ्क ret;

	सचे (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		गच्छ out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#अञ्ञथा
	ret = check_crontab_process(cmd);
#endif

out:
	मुक्त(cmd);
	पटिनिवत्त ret;
}

#define BEGIN_पङ्क्तिः "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_पङ्क्तिः "# END GIT MAINTENANCE SCHEDULE"

स्थिर पूर्णाङ्क crontab_update_schedule(पूर्णाङ्क run_maintenance, पूर्णाङ्क fd)
{
	वर्ण *cmd;
	पूर्णाङ्क result = 0;
	पूर्णाङ्क in_old_region = 0;
	संरचना child_process crontab_list = CHILD_PROCESS_INIT;
	संरचना child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	संरचना strbuf पङ्क्तिः = STRBUF_INIT;
	संरचना tempfile *tmpedit = रिक्त;
	पूर्णाङ्क minute = get_random_minute();

	get_schedule_cmd("crontab", रिक्त, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_प्रक्षिप(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	सचे (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		गच्छ out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	सचे (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		गच्छ out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	सचे (!cron_in) {
		result = error(_("failed to open temporary file"));
		गच्छ out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	याव (!strbuf_getline_lf(&पङ्क्तिः, cron_list)) {
		सचे (!in_old_region && !strcmp(पङ्क्तिः.buf, BEGIN_पङ्क्तिः))
			in_old_region = 1;
		अञ्ञथा सचे (in_old_region && !strcmp(पङ्क्तिः.buf, END_पङ्क्तिः))
			in_old_region = 0;
		अञ्ञथा सचे (!in_old_region)
			fprintf(cron_in, "%s\n", पङ्क्तिः.buf);
	}
	strbuf_release(&पङ्क्तिः);

	सचे (run_maintenance) {
		संरचना strbuf पङ्क्तिः_format = STRBUF_INIT;
		ध्रुव वर्ण *exec_path = git_exec_path();

		fprintf(cron_in, "%s\n", BEGIN_पङ्क्तिः);
		fprintf(cron_in,
			"# The following schedule was created by Git\n");
		fprintf(cron_in, "# Any edits made in this region might be\n");
		fprintf(cron_in,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&पङ्क्तिः_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_path, exec_path, get_extra_config_parameters());
		fprintf(cron_in, पङ्क्तिः_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_in, पङ्क्तिः_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_in, पङ्क्तिः_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&पङ्क्तिः_format);

		fprintf(cron_in, "\n%s\n", END_पङ्क्तिः);
	}

	fflush(cron_in);

	strvec_split(&crontab_edit.args, cmd);
	strvec_प्रक्षिप(&crontab_edit.args, get_tempfile_path(tmpedit));
	crontab_edit.git_cmd = 0;

	सचे (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		गच्छ out;
	}

	सचे (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	अञ्ञथा
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	मुक्त(cmd);
	पटिनिवत्त result;
}

स्थिर पूर्णाङ्क real_is_systemd_timer_available(शून्य)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", रिक्त);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	सचे (start_command(&child))
		पटिनिवत्त 0;
	सचे (finish_command(&child))
		पटिनिवत्त 0;
	पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क is_systemd_timer_available(शून्य)
{
	पूर्णाङ्क is_available;

	सचे (get_schedule_cmd("systemctl", &is_available, रिक्त))
		पटिनिवत्त is_available;

	पटिनिवत्त real_is_systemd_timer_available();
}

स्थिर वर्ण *xdg_config_home_systemd(ध्रुव वर्ण *filename)
{
	पटिनिवत्त xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

स्थिर पूर्णाङ्क systemd_timer_delete_timer_file(गणना schedule_priority priority)
{
	पूर्णाङ्क ret = 0;
	ध्रुव वर्ण *frequency = get_frequency(priority);
	वर्ण *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	वर्ण *filename = xdg_config_home_systemd(local_timer_name);

	सचे (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	मुक्त(filename);
	मुक्त(local_timer_name);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क systemd_timer_delete_service_template(शून्य)
{
	पूर्णाङ्क ret = 0;
	वर्ण *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	वर्ण *filename = xdg_config_home_systemd(local_service_name);
	सचे (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	मुक्त(filename);
	मुक्त(local_service_name);
	पटिनिवत्त ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
स्थिर पूर्णाङ्क systemd_timer_write_timer_file(गणना schedule_priority schedule,
					  पूर्णाङ्क minute)
{
	पूर्णाङ्क res = -1;
	वर्ण *filename;
	FILE *file;
	ध्रुव वर्ण *unit;
	वर्ण *schedule_pattern = रिक्त;
	ध्रुव वर्ण *frequency = get_frequency(schedule);
	वर्ण *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	सचे (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		गच्छ error;
	}
	file = fopen_or_warn(filename, "w");
	सचे (!file)
		गच्छ error;

	विकल्प (schedule) {
	पक्ष SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		भङ्ग;

	पक्ष SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		भङ्ग;

	पक्ष SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		भङ्ग;

	स्वभाव:
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
	सचे (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		गच्छ error;
	}
	सचे (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		गच्छ error;
	}

	res = 0;

error:
	मुक्त(schedule_pattern);
	मुक्त(local_timer_name);
	मुक्त(filename);
	पटिनिवत्त res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
स्थिर पूर्णाङ्क systemd_timer_write_service_template(ध्रुव वर्ण *exec_path)
{
	पूर्णाङ्क res = -1;
	वर्ण *filename;
	FILE *file;
	ध्रुव वर्ण *unit;
	वर्ण *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	सचे (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		गच्छ error;
	}
	file = fopen_or_warn(filename, "w");
	सचे (!file)
		गच्छ error;

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
	सचे (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		गच्छ error;
	}
	सचे (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		गच्छ error;
	}

	res = 0;

error:
	मुक्त(local_service_name);
	मुक्त(filename);
	पटिनिवत्त res;
}

स्थिर पूर्णाङ्क systemd_timer_enable_unit(पूर्णाङ्क enable,
				     गणना schedule_priority schedule,
				     पूर्णाङ्क minute)
{
	वर्ण *cmd = रिक्त;
	संरचना child_process child = CHILD_PROCESS_INIT;
	ध्रुव वर्ण *frequency = get_frequency(schedule);
	पूर्णाङ्क ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	सचे (!enable) {
		child.no_stderr = 1;
	} अञ्ञथा सचे (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		गच्छ out;
	}

	get_schedule_cmd("systemctl", रिक्त, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", रिक्त);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	सचे (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		गच्छ out;
	}

	सचे (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		सचे (enable) {
			ret = error(_("failed to run systemctl"));
			गच्छ out;
		}
	}

	ret = 0;

out:
	मुक्त(cmd);
	पटिनिवत्त ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
स्थिर शून्य systemd_timer_delete_stale_timer_templates(शून्य)
{
	वर्ण *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	वर्ण *filename = xdg_config_home_systemd(timer_template_name);

	सचे (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	मुक्त(filename);
	मुक्त(timer_template_name);
}

स्थिर पूर्णाङ्क systemd_timer_delete_unit_files(शून्य)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	पटिनिवत्त systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

स्थिर पूर्णाङ्क systemd_timer_delete_units(शून्य)
{
	पूर्णाङ्क minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	पटिनिवत्त systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

स्थिर पूर्णाङ्क systemd_timer_setup_units(शून्य)
{
	पूर्णाङ्क minute = get_random_minute();
	ध्रुव वर्ण *exec_path = git_exec_path();

	पूर्णाङ्क ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	सचे (ret)
		systemd_timer_delete_units();
	अञ्ञथा
		systemd_timer_delete_stale_timer_templates();

	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क systemd_timer_update_schedule(पूर्णाङ्क run_maintenance, पूर्णाङ्क fd UNUSED)
{
	सचे (run_maintenance)
		पटिनिवत्त systemd_timer_setup_units();
	अञ्ञथा
		पटिनिवत्त systemd_timer_delete_units();
}

गणना scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

स्थिर ध्रुव संरचना {
	ध्रुव वर्ण *name;
	पूर्णाङ्क (*is_available)(शून्य);
	पूर्णाङ्क (*update_schedule)(पूर्णाङ्क run_maintenance, पूर्णाङ्क fd);
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

स्थिर गणना scheduler parse_scheduler(ध्रुव वर्ण *मानम्)
{
	सचे (!मानम्)
		पटिनिवत्त SCHEDULER_INVALID;
	अञ्ञथा सचे (!strcasecmp(मानम्, "auto"))
		पटिनिवत्त SCHEDULER_AUTO;
	अञ्ञथा सचे (!strcasecmp(मानम्, "cron") || !strcasecmp(मानम्, "crontab"))
		पटिनिवत्त SCHEDULER_CRON;
	अञ्ञथा सचे (!strcasecmp(मानम्, "systemd") ||
		 !strcasecmp(मानम्, "systemd-timer"))
		पटिनिवत्त SCHEDULER_SYSTEMD;
	अञ्ञथा सचे (!strcasecmp(मानम्, "launchctl"))
		पटिनिवत्त SCHEDULER_LAUNCHCTL;
	अञ्ञथा सचे (!strcasecmp(मानम्, "schtasks"))
		पटिनिवत्त SCHEDULER_SCHTASKS;
	अञ्ञथा
		पटिनिवत्त SCHEDULER_INVALID;
}

स्थिर पूर्णाङ्क maintenance_opt_scheduler(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				     पूर्णाङ्क unset)
{
	गणना scheduler *scheduler = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	सचे (*scheduler == SCHEDULER_INVALID)
		पटिनिवत्त error(_("unrecognized --scheduler argument '%s'"), arg);
	पटिनिवत्त 0;
}

संरचना maintenance_start_opts {
	गणना scheduler scheduler;
};

स्थिर गणना scheduler resolve_scheduler(गणना scheduler scheduler)
{
	सचे (scheduler != SCHEDULER_AUTO)
		पटिनिवत्त scheduler;

#सचे defined(__APPLE__)
	पटिनिवत्त SCHEDULER_LAUNCHCTL;

#नोचेद्यदि defined(GIT_WINDOWS_NATIVE)
	पटिनिवत्त SCHEDULER_SCHTASKS;

#नोचेद्यदि defined(__linux__)
	सचे (is_systemd_timer_available())
		पटिनिवत्त SCHEDULER_SYSTEMD;
	अञ्ञथा सचे (is_crontab_available())
		पटिनिवत्त SCHEDULER_CRON;
	अञ्ञथा
		die(_("neither systemd timers nor crontab are available"));

#अञ्ञथा
	पटिनिवत्त SCHEDULER_CRON;
#endif
}

स्थिर शून्य validate_scheduler(गणना scheduler scheduler)
{
	सचे (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	सचे (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	सचे (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

स्थिर पूर्णाङ्क update_background_schedule(ध्रुव संरचना maintenance_start_opts *opts,
				      पूर्णाङ्क enable)
{
	unsigned पूर्णाङ्क i;
	पूर्णाङ्क result = 0;
	संरचना lock_file lk;
	वर्ण *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	सचे (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		सचे (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		अञ्ञथा
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		मुक्त(lock_path);
		पटिनिवत्त -1;
	}

	अत्थाय (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		सचे (enable && opts->scheduler == i)
			अनुवर्त;
		सचे (!scheduler_fn[i].is_available())
			अनुवर्त;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	सचे (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	मुक्त(lock_path);
	पटिनिवत्त result;
}

स्थिर ध्रुव वर्ण *ध्रुव builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_start(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			     संरचना repository *repo)
{
	संरचना maintenance_start_opts opts = { 0 };
	संरचना option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	ध्रुव वर्ण *register_args[] = { "register", रिक्त };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	सचे (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	सचे (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	सचे (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, रिक्त, repo))
		warning(_("failed to add repo to global config"));
	पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण *ध्रुव builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_stop(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			    संरचना repository *repo UNUSED)
{
	संरचना option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	सचे (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	पटिनिवत्त update_background_schedule(रिक्त, 0);
}

स्थिर ध्रुव वर्ण *ध्रुव builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_is_needed(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
				 संरचना repository *repo UNUSED)
{
	संरचना maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	संरचना string_list selected_tasks = STRING_LIST_INIT_DUP;
	संरचना gc_config cfg = GC_CONFIG_INIT;
	संरचना option options[] = {
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
	सचे (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	सचे (opts.auto_flag) {
		अत्थाय (size_t i = 0; i < opts.tasks_nr; i++) {
			सचे (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				भङ्ग;
			}
		}
	} अञ्ञथा {
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

	सचे (is_needed)
		पटिनिवत्त 0;
	पटिनिवत्त 1;
}

स्थिर ध्रुव वर्ण *ध्रुव builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	रिक्त,
};

पूर्णाङ्क cmd_maintenance(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *prefix,
		    संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option builtin_maintenance_options[] = {
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
	पटिनिवत्त fn(argc, argv, prefix, repo);
}
