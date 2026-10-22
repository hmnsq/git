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

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "date.h"
#समावेश "dir.h"
#समावेश "environment.h"
#समावेश "hex.h"
#समावेश "config.h"
#समावेश "tempfile.h"
#समावेश "lockfile.h"
#समावेश "parse-options.h"
#समावेश "run-command.h"
#समावेश "sigchain.h"
#समावेश "strvec.h"
#समावेश "commit.h"
#समावेश "commit-graph.h"
#समावेश "packfile.h"
#समावेश "object-file.h"
#समावेश "odb.h"
#समावेश "path.h"
#समावेश "reflog.h"
#समावेश "rerere.h"
#समावेश "revision.h"
#समावेश "refs.h"
#समावेश "remote.h"
#समावेश "exec-cmd.h"
#समावेश "gettext.h"
#समावेश "hook.h"
#समावेश "setup.h"
#समावेश "trace2.h"
#समावेश "worktree.h"

#घोषणा FAILED_RUN "failed to run %s"

अटल स्थिर अक्षर * स्थिर builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	शून्य
};

अटल timestamp_t gc_log_expire_time;
अटल काष्ठा tempfile *pidfile;
अटल काष्ठा lock_file log_lock;
अटल काष्ठा string_list pack_garbage = STRING_LIST_INIT_DUP;

अटल व्योम clean_pack_garbage(व्योम)
{
	पूर्णांक i;
	क्रम (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.वस्तुएँ[i].string);
	string_list_clear(&pack_garbage, 0);
}

अटल व्योम report_pack_garbage(अचिन्हित seen_bits, स्थिर अक्षर *path)
{
	अगर (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

अटल व्योम process_log_file(व्योम)
{
	काष्ठा stat st;
	अगर (ख_स्थिति(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		पूर्णांक saved_errno = त्रुटि_सं;
		ख_लिखो(मानक_त्रुटि, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			म_त्रुटि(saved_errno));
		ख_साफ(मानक_त्रुटि);
		commit_lock_file(&log_lock);
		त्रुटि_सं = saved_errno;
	} अन्यथा अगर (st.st_आकार) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} अन्यथा {
		अक्षर *path = repo_git_path(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(path);
		rollback_lock_file(&log_lock);
		मुक्त(path);
	}
}

अटल व्योम process_log_file_at_exit(व्योम)
{
	ख_साफ(मानक_त्रुटि);
	process_log_file();
}

अटल पूर्णांक gc_config_is_timestamp_never(स्थिर अक्षर *var)
{
	स्थिर अक्षर *मूल्य;
	timestamp_t expire;

	अगर (!repo_config_get_मूल्य(the_repository, var, &मूल्य) && मूल्य) {
		अगर (parse_expiry_date(मूल्य, &expire))
			die(_("failed to parse '%s' value '%s'"), var, मूल्य);
		वापस expire == 0;
	}
	वापस 0;
}

काष्ठा gc_config {
	पूर्णांक pack_refs;
	पूर्णांक prune_reflogs;
	पूर्णांक cruft_packs;
	अचिन्हित दीर्घ max_cruft_आकार;
	पूर्णांक aggressive_depth;
	पूर्णांक aggressive_window;
	पूर्णांक detach_auto;
	अक्षर *gc_log_expire;
	अक्षर *prune_expire;
	अक्षर *prune_worktrees_expire;
	अक्षर *repack_expire_to;
};

#घोषणा GC_CONFIG_INIT { \
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

अटल व्योम gc_config_release(काष्ठा gc_config *cfg)
{
	मुक्त(cfg->gc_log_expire);
	मुक्त(cfg->prune_expire);
	मुक्त(cfg->prune_worktrees_expire);
}

अटल व्योम gc_config(काष्ठा gc_config *cfg)
{
	स्थिर अक्षर *मूल्य;
	अक्षर *owned = शून्य;

	अगर (!repo_config_get_मूल्य(the_repository, "gc.packrefs", &मूल्य)) {
		अगर (मूल्य && !म_भेद(मूल्य, "notbare"))
			cfg->pack_refs = -1;
		अन्यथा
			cfg->pack_refs = git_config_bool("gc.packrefs", मूल्य);
	}

	अगर (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_आकार);

	अगर (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		मुक्त(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	अगर (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		मुक्त(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	अगर (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		मुक्त(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, शून्य);
}

क्रमागत schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

अटल क्रमागत schedule_priority parse_schedule(स्थिर अक्षर *मूल्य)
{
	अगर (!मूल्य)
		वापस SCHEDULE_NONE;
	अगर (!strcasecmp(मूल्य, "hourly"))
		वापस SCHEDULE_HOURLY;
	अगर (!strcasecmp(मूल्य, "daily"))
		वापस SCHEDULE_DAILY;
	अगर (!strcasecmp(मूल्य, "weekly"))
		वापस SCHEDULE_WEEKLY;
	वापस SCHEDULE_NONE;
}

क्रमागत maintenance_task_label {
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
	TASK_गिनती
};

काष्ठा maintenance_run_opts {
	क्रमागत maintenance_task_label *tasks;
	माप_प्रकार tasks_nr, tasks_alloc;
	पूर्णांक auto_flag;
	पूर्णांक detach;
	पूर्णांक quiet;
	क्रमागत schedule_priority schedule;
};
#घोषणा MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

अटल व्योम maintenance_run_opts_release(काष्ठा maintenance_run_opts *opts)
{
	मुक्त(opts->tasks);
}

अटल पूर्णांक pack_refs_condition(UNUSED काष्ठा gc_config *cfg)
{
	काष्ठा string_list included_refs = STRING_LIST_INIT_NODUP;
	काष्ठा ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	काष्ठा refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	अगर (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		वापस 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	वापस required;
}

अटल पूर्णांक maintenance_task_pack_refs(काष्ठा maintenance_run_opts *opts,
				      UNUSED काष्ठा gc_config *cfg)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", शून्य);
	अगर (opts->auto_flag)
		strvec_धकेलो(&cmd.args, "--auto");

	वापस run_command(&cmd);
}

काष्ठा गिनती_reflog_entries_तथ्य {
	काष्ठा expire_reflog_policy_cb policy;
	माप_प्रकार गिनती;
	माप_प्रकार limit;
};

अटल पूर्णांक गिनती_reflog_entries(स्थिर अक्षर *refname UNUSED,
				काष्ठा object_id *old_oid, काष्ठा object_id *new_oid,
				स्थिर अक्षर *committer, timestamp_t timestamp,
				पूर्णांक tz, स्थिर अक्षर *संदेश, व्योम *cb_तथ्य)
{
	काष्ठा गिनती_reflog_entries_तथ्य *तथ्य = cb_तथ्य;
	अगर (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, संदेश, &तथ्य->policy))
		तथ्य->गिनती++;
	वापस तथ्य->गिनती >= तथ्य->limit;
}

अटल पूर्णांक reflog_expire_condition(काष्ठा gc_config *cfg UNUSED)
{
	timestamp_t now = समय(शून्य);
	काष्ठा गिनती_reflog_entries_तथ्य तथ्य = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	पूर्णांक limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	अगर (!limit)
		वापस 0;
	अगर (limit < 0)
		वापस 1;
	तथ्य.limit = limit;

	repo_config(the_repository, reflog_expire_config, &तथ्य.policy.opts);

	reflog_expire_options_set_refname(&तथ्य.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 गिनती_reflog_entries, &तथ्य);

	reflog_expiry_cleanup(&तथ्य.policy);
	reflog_clear_expire_config(&तथ्य.policy.opts);
	वापस तथ्य.गिनती >= तथ्य.limit;
}

अटल पूर्णांक maintenance_task_reflog_expire(काष्ठा maintenance_run_opts *opts UNUSED,
					  काष्ठा gc_config *cfg UNUSED)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", शून्य);
	वापस run_command(&cmd);
}

अटल पूर्णांक maintenance_task_worktree_prune(काष्ठा maintenance_run_opts *opts UNUSED,
					   काष्ठा gc_config *cfg)
{
	काष्ठा child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", शून्य);
	strvec_धकेलो(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	वापस run_command(&prune_worktrees_cmd);
}

अटल पूर्णांक worktree_prune_condition(काष्ठा gc_config *cfg)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	पूर्णांक should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	काष्ठा dirent *d;
	सूची *dir = शून्य;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	अगर (limit <= 0) {
		should_prune = limit < 0;
		जाओ out;
	}

	अगर (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		जाओ out;

	dir = सूची_खोलो(repo_git_path_replace(the_repository, &buf, "worktrees"));
	अगर (!dir)
		जाओ out;

	जबतक (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		अक्षर *wtpath;
		strbuf_reset(&buf);
		अगर (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		मुक्त(wtpath);
	}

	should_prune = !limit;

out:
	अगर (dir)
		बंद_सूची(dir);
	strbuf_release(&buf);
	वापस should_prune;
}

अटल पूर्णांक maintenance_task_rerere_gc(काष्ठा maintenance_run_opts *opts UNUSED,
				      काष्ठा gc_config *cfg UNUSED)
{
	काष्ठा child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", शून्य);
	वापस run_command(&rerere_cmd);
}

अटल पूर्णांक rerere_gc_condition(काष्ठा gc_config *cfg UNUSED)
{
	पूर्णांक limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	अगर (!limit)
		वापस 0; /* never prune */
	अगर (limit < 0)
		वापस 1; /* always prune */

	वापस rerere_gc_needed(the_repository, (माप_प्रकार)limit);
}

#घोषणा OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_आकार = (cfg)->max_cruft_आकार, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
अटल स्थिर अक्षर *lock_repo_for_gc(पूर्णांक force, pid_t* ret_pid)
{
	काष्ठा lock_file lock = LOCK_INIT;
	अक्षर my_host[HOST_NAME_MAX + 1];
	काष्ठा strbuf sb = STRBUF_INIT;
	काष्ठा stat st;
	uintmax_t pid;
	खाता *fp;
	पूर्णांक fd;
	अक्षर *pidfile_path;

	अगर (is_tempfile_active(pidfile))
		/* already locked */
		वापस शून्य;

	अगर (xgethostname(my_host, माप(my_host)))
		xsnprintf(my_host, माप(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	अगर (!force) {
		अटल अक्षर locking_host[HOST_NAME_MAX + 1];
		अटल अक्षर *scan_fmt;
		पूर्णांक should_exit;

		अगर (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = ख_खोलो(pidfile_path, "r");
		स_रखो(locking_host, 0, माप(locking_host));
		should_exit =
			fp != शून्य &&
			!ख_स्थिति(fileno(fp), &st) &&
			/*
			 * 12 hour limit is very generous as gc should
			 * never take that long. On the other hand we
			 * don't really need a strict limit here,
			 * running gc --auto one day late is not a big
			 * problem. --force can be used in manual gc
			 * after the user verifies that no gc is
			 * running.
			 */
			समय(शून्य) - st.st_mtime <= 12 * 3600 &&
			ख_पूछो(fp, scan_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(म_भेद(locking_host, my_host) || !समाप्त(pid, 0) || त्रुटि_सं == EPERM);
		अगर (fp)
			ख_बंद(fp);
		अगर (should_exit) {
			अगर (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			मुक्त(pidfile_path);
			वापस locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	मुक्त(pidfile_path);
	वापस शून्य;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
अटल पूर्णांक report_last_gc_error(व्योम)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	पूर्णांक ret = 0;
	ssize_t len;
	काष्ठा stat st;
	अक्षर *gc_log_path = repo_git_path(the_repository, "gc.log");

	अगर (stat(gc_log_path, &st)) {
		अगर (त्रुटि_सं == ENOENT)
			जाओ done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		जाओ done;
	}

	अगर (st.st_mtime < gc_log_expire_time)
		जाओ done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	अगर (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	अन्यथा अगर (len > 0) {
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
	वापस ret;
}

अटल पूर्णांक gc_foreground_tasks(काष्ठा maintenance_run_opts *opts,
			       काष्ठा gc_config *cfg)
{
	अगर (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		वापस error(FAILED_RUN, "pack-refs");
	अगर (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		वापस error(FAILED_RUN, "reflog");
	वापस 0;
}

अटल पूर्णांक maintenance_task_odb(काष्ठा maintenance_run_opts *opts,
				काष्ठा gc_config *cfg,
				पूर्णांक keep_largest_pack,
				पूर्णांक aggressive)
{
	काष्ठा odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	अगर (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	अगर (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	अगर (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	वापस odb_optimize(the_repository->objects, &odb_opts);
}

पूर्णांक cmd_gc(पूर्णांक argc,
	   स्थिर अक्षर **argv,
	   स्थिर अक्षर *prefix,
	   काष्ठा repository *repo UNUSED)
{
	पूर्णांक aggressive = 0;
	पूर्णांक force = 0;
	स्थिर अक्षर *name;
	pid_t pid;
	पूर्णांक daemonized = 0;
	पूर्णांक keep_largest_pack = -1;
	पूर्णांक skip_foreground_tasks = 0;
	timestamp_t dummy;
	काष्ठा maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	काष्ठा gc_config cfg = GC_CONFIG_INIT;
	स्थिर अक्षर *prune_expire_sentinel = "sentinel";
	स्थिर अक्षर *prune_expire_arg = prune_expire_sentinel;
	पूर्णांक ret;
	काष्ठा option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.मूल्य = &prune_expire_arg,
			.argh = N_("date"),
			.help = N_("prune unreferenced objects"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)prune_expire_arg,
		},
		OPT_BOOL(0, "cruft", &cfg.cruft_packs, N_("pack unreferenced objects separately")),
		OPT_UNSIGNED(0, "max-cruft-size", &cfg.max_cruft_आकार,
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

	अगर (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	अगर (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	अगर (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	अगर (prune_expire_arg != prune_expire_sentinel) {
		मुक्त(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	अगर (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	अगर (opts.auto_flag) {
		काष्ठा odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		अगर (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		अगर (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			जाओ out;
		}

		अगर (!opts.quiet) {
			अगर (opts.detach > 0)
				ख_लिखो(मानक_त्रुटि, _("Auto packing the repository in background for optimum performance.\n"));
			अन्यथा
				ख_लिखो(मानक_त्रुटि, _("Auto packing the repository for optimum performance.\n"));
			ख_लिखो(मानक_त्रुटि, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	अगर (opts.detach > 0) {
		ret = report_last_gc_error();
		अगर (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			जाओ out;

		} अन्यथा अगर (ret) {
			/* an I/O error occurred, already reported */
			जाओ out;
		}

		अगर (!skip_foreground_tasks) {
			अगर (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				जाओ out;
			}

			अगर (gc_foreground_tasks(&opts, &cfg) < 0)
				die(शून्य);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = lock_repo_for_gc(force, &pid);
	अगर (name) {
		अगर (opts.auto_flag) {
			ret = 0;
			जाओ out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	अगर (daemonized) {
		अक्षर *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		निकास_पर(process_log_file_at_exit);
		मुक्त(path);
	}

	अगर (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	अगर (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	अगर (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	अगर (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(शून्य);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	अगर (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	अगर (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     शून्य);

	अगर (!daemonized) {
		अक्षर *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		मुक्त(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	वापस 0;
}

अटल स्थिर अक्षर *स्थिर builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	शून्य
};

अटल पूर्णांक maintenance_opt_schedule(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
				    पूर्णांक unset)
{
	क्रमागत schedule_priority *priority = opt->मूल्य;

	अगर (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	अगर (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	वापस 0;
}

काष्ठा cg_auto_तथ्य {
	पूर्णांक अंक_not_in_graph;
	पूर्णांक limit;
};

अटल पूर्णांक dfs_on_ref(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	काष्ठा cg_auto_तथ्य *तथ्य = (काष्ठा cg_auto_तथ्य *)cb_तथ्य;
	पूर्णांक परिणाम = 0;
	स्थिर काष्ठा object_id *maybe_peeled = ref->oid;
	काष्ठा object_id peeled;
	काष्ठा commit_list *ढेर = शून्य;
	काष्ठा commit *commit;

	अगर (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	अगर (odb_read_object_info(the_repository->objects, maybe_peeled, शून्य) != OBJ_COMMIT)
		वापस 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	अगर (!commit || commit->object.flags & SEEN)
		वापस 0;
	commit->object.flags |= SEEN;

	अगर (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		वापस 0;

	तथ्य->अंक_not_in_graph++;

	अगर (तथ्य->अंक_not_in_graph >= तथ्य->limit)
		वापस 1;

	commit_list_जोड़ो(commit, &ढेर);

	जबतक (!परिणाम && ढेर) {
		काष्ठा commit_list *parent;

		commit = निकालो_commit(&ढेर);

		क्रम (parent = commit->parents; parent; parent = parent->अग्र) {
			अगर (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				जारी;

			parent->item->object.flags |= SEEN;
			तथ्य->अंक_not_in_graph++;

			अगर (तथ्य->अंक_not_in_graph >= तथ्य->limit) {
				परिणाम = 1;
				अवरोध;
			}

			commit_list_जोड़ो(parent->item, &ढेर);
		}
	}

	commit_list_free(ढेर);
	वापस परिणाम;
}

अटल पूर्णांक should_write_commit_graph(काष्ठा gc_config *cfg UNUSED)
{
	पूर्णांक परिणाम;
	काष्ठा cg_auto_तथ्य तथ्य;

	तथ्य.अंक_not_in_graph = 0;
	तथ्य.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &तथ्य.limit);

	अगर (!तथ्य.limit)
		वापस 0;
	अगर (तथ्य.limit < 0)
		वापस 1;

	परिणाम = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &तथ्य);

	repo_clear_commit_marks(the_repository, SEEN);

	वापस परिणाम;
}

अटल पूर्णांक run_write_commit_graph(काष्ठा maintenance_run_opts *opts)
{
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", शून्य);

	अगर (opts->quiet)
		strvec_धकेलो(&child.args, "--no-progress");
	अन्यथा
		strvec_धकेलो(&child.args, "--progress");

	वापस !!run_command(&child);
}

अटल पूर्णांक maintenance_task_commit_graph(काष्ठा maintenance_run_opts *opts,
					 काष्ठा gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	अगर (!the_repository->settings.core_commit_graph)
		वापस 0;

	अगर (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		वापस 1;
	}

	वापस 0;
}

अटल पूर्णांक fetch_remote(काष्ठा remote *remote, व्योम *cbdata)
{
	काष्ठा maintenance_run_opts *opts = cbdata;
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	अगर (remote->skip_default_update)
		वापस 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     शून्य);

	अगर (opts->quiet)
		strvec_धकेलो(&child.args, "--quiet");

	वापस !!run_command(&child);
}

अटल पूर्णांक maintenance_task_prefetch(काष्ठा maintenance_run_opts *opts,
				     काष्ठा gc_config *cfg UNUSED)
{
	अगर (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		वापस 1;
	}

	वापस 0;
}

अटल पूर्णांक maintenance_task_gc_foreground(काष्ठा maintenance_run_opts *opts,
					  काष्ठा gc_config *cfg)
{
	वापस gc_foreground_tasks(opts, cfg);
}

अटल पूर्णांक maintenance_task_gc_background(काष्ठा maintenance_run_opts *opts,
					  काष्ठा gc_config *cfg UNUSED)
{
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_धकेलो(&child.args, "gc");

	अगर (opts->auto_flag)
		strvec_धकेलो(&child.args, "--auto");
	अगर (opts->quiet)
		strvec_धकेलो(&child.args, "--quiet");
	अन्यथा
		strvec_धकेलो(&child.args, "--no-quiet");
	strvec_धकेलो(&child.args, "--no-detach");
	strvec_धकेलो(&child.args, "--skip-foreground-tasks");

	वापस run_command(&child);
}

अटल पूर्णांक gc_condition(काष्ठा gc_config *cfg)
{
	काष्ठा odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	वापस odb_optimize_required(the_repository->objects, &opts);
}

अटल पूर्णांक prune_packed(काष्ठा maintenance_run_opts *opts)
{
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_धकेलो(&child.args, "prune-packed");

	अगर (opts->quiet)
		strvec_धकेलो(&child.args, "--quiet");

	वापस !!run_command(&child);
}

काष्ठा write_loose_object_तथ्य {
	खाता *in;
	पूर्णांक गिनती;
	पूर्णांक batch_आकार;
};

अटल पूर्णांक loose_object_auto_limit = 100;

अटल पूर्णांक loose_object_गिनती(स्थिर काष्ठा object_id *oid UNUSED,
			      स्थिर अक्षर *path UNUSED,
			      व्योम *तथ्य)
{
	पूर्णांक *गिनती = (पूर्णांक*)तथ्य;
	अगर (++(*गिनती) >= loose_object_auto_limit)
		वापस 1;
	वापस 0;
}

अटल पूर्णांक loose_object_auto_condition(काष्ठा gc_config *cfg UNUSED)
{
	पूर्णांक गिनती = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	अगर (!loose_object_auto_limit)
		वापस 0;
	अगर (loose_object_auto_limit < 0)
		वापस 1;

	वापस for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_गिनती,
					     शून्य, शून्य, &गिनती);
}

अटल पूर्णांक bail_on_loose(स्थिर काष्ठा object_id *oid UNUSED,
			 स्थिर अक्षर *path UNUSED,
			 व्योम *तथ्य UNUSED)
{
	वापस 1;
}

अटल पूर्णांक write_loose_object_to_stdin(स्थिर काष्ठा object_id *oid,
				       स्थिर अक्षर *path UNUSED,
				       व्योम *तथ्य)
{
	काष्ठा write_loose_object_तथ्य *d = (काष्ठा write_loose_object_तथ्य *)तथ्य;

	ख_लिखो(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	वापस ++(d->गिनती) > d->batch_आकार;
}

अटल पूर्णांक pack_loose(काष्ठा maintenance_run_opts *opts)
{
	काष्ठा repository *r = the_repository;
	पूर्णांक परिणाम = 0;
	काष्ठा write_loose_object_तथ्य तथ्य;
	काष्ठा child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	अगर (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   शून्य, शून्य, शून्य))
		वापस 0;

	pack_proc.git_cmd = 1;

	strvec_धकेलो(&pack_proc.args, "pack-objects");
	अगर (opts->quiet)
		strvec_धकेलो(&pack_proc.args, "--quiet");
	अन्यथा
		strvec_धकेलो(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	अगर (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		वापस 1;
	}

	तथ्य.in = xfdopen(pack_proc.in, "w");
	तथ्य.गिनती = 0;
	तथ्य.batch_आकार = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &तथ्य.batch_आकार);

	/* If configured as 0, then remove limit. */
	अगर (!तथ्य.batch_आकार)
		तथ्य.batch_आकार = पूर्णांक_उच्च;
	अन्यथा अगर (तथ्य.batch_आकार > 0)
		तथ्य.batch_आकार--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      शून्य, शून्य, &तथ्य);

	ख_बंद(तथ्य.in);

	अगर (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		परिणाम = 1;
	}

	वापस परिणाम;
}

अटल पूर्णांक maintenance_task_loose_objects(काष्ठा maintenance_run_opts *opts,
					  काष्ठा gc_config *cfg UNUSED)
{
	वापस prune_packed(opts) || pack_loose(opts);
}

अटल पूर्णांक incremental_repack_auto_condition(काष्ठा gc_config *cfg UNUSED)
{
	काष्ठा packed_git *p;
	पूर्णांक incremental_repack_auto_limit = 10;
	पूर्णांक गिनती = 0;

	prepare_repo_settings(the_repository);
	अगर (!the_repository->settings.core_multi_pack_सूचकांक)
		वापस 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	अगर (!incremental_repack_auto_limit)
		वापस 0;
	अगर (incremental_repack_auto_limit < 0)
		वापस 1;

	repo_for_each_pack(the_repository, p) {
		अगर (गिनती >= incremental_repack_auto_limit)
			अवरोध;
		अगर (!p->multi_pack_सूचकांक)
			गिनती++;
	}

	वापस गिनती >= incremental_repack_auto_limit;
}

अटल पूर्णांक multi_pack_सूचकांक_write(काष्ठा maintenance_run_opts *opts)
{
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", शून्य);

	अगर (opts->quiet)
		strvec_धकेलो(&child.args, "--no-progress");
	अन्यथा
		strvec_धकेलो(&child.args, "--progress");

	अगर (run_command(&child))
		वापस error(_("failed to write multi-pack-index"));

	वापस 0;
}

अटल पूर्णांक multi_pack_सूचकांक_expire(काष्ठा maintenance_run_opts *opts)
{
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", शून्य);

	अगर (opts->quiet)
		strvec_धकेलो(&child.args, "--no-progress");
	अन्यथा
		strvec_धकेलो(&child.args, "--progress");

	अगर (run_command(&child))
		वापस error(_("'git multi-pack-index expire' failed"));

	वापस 0;
}

#घोषणा TWO_GIGABYTES (INT32_MAX)

अटल off_t get_auto_pack_आकार(व्योम)
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
	off_t max_आकार = 0;
	off_t second_largest_आकार = 0;
	off_t परिणाम_आकार;
	काष्ठा packed_git *p;
	काष्ठा repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		अगर (p->pack_आकार > max_आकार) {
			second_largest_आकार = max_आकार;
			max_आकार = p->pack_आकार;
		} अन्यथा अगर (p->pack_आकार > second_largest_आकार)
			second_largest_आकार = p->pack_आकार;
	}

	परिणाम_आकार = second_largest_आकार + 1;

	/* But limit ourselves to a batch size of 2g */
	अगर (परिणाम_आकार > TWO_GIGABYTES)
		परिणाम_आकार = TWO_GIGABYTES;

	वापस परिणाम_आकार;
}

अटल पूर्णांक multi_pack_सूचकांक_repack(काष्ठा maintenance_run_opts *opts)
{
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", शून्य);

	अगर (opts->quiet)
		strvec_धकेलो(&child.args, "--no-progress");
	अन्यथा
		strvec_धकेलो(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_आकार());

	अगर (run_command(&child))
		वापस error(_("'git multi-pack-index repack' failed"));

	वापस 0;
}

अटल पूर्णांक maintenance_task_incremental_repack(काष्ठा maintenance_run_opts *opts,
					       काष्ठा gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	अगर (!the_repository->settings.core_multi_pack_सूचकांक) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		वापस 0;
	}

	अगर (multi_pack_सूचकांक_write(opts))
		वापस 1;
	अगर (multi_pack_सूचकांक_expire(opts))
		वापस 1;
	अगर (multi_pack_सूचकांक_repack(opts))
		वापस 1;
	वापस 0;
}

अटल पूर्णांक maintenance_task_geometric_repack(काष्ठा maintenance_run_opts *opts,
					     काष्ठा gc_config *cfg)
{
	काष्ठा odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	अगर (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	वापस odb_optimize(the_repository->objects, &odb_opts);
}

अटल पूर्णांक geometric_repack_auto_condition(काष्ठा gc_config *cfg)
{
	काष्ठा odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	वापस odb_optimize_required(the_repository->objects, &opts);
}

प्रकार पूर्णांक (*maintenance_task_fn)(काष्ठा maintenance_run_opts *opts,
				   काष्ठा gc_config *cfg);
प्रकार पूर्णांक (*maintenance_auto_fn)(काष्ठा gc_config *cfg);

काष्ठा maintenance_task {
	स्थिर अक्षर *name;

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

अटल स्थिर काष्ठा maintenance_task tasks[] = {
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

क्रमागत task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

क्रमागत auto_gc_hook_परिणाम {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

अटल पूर्णांक maybe_run_task(स्थिर काष्ठा maintenance_task *task,
			  काष्ठा repository *repo,
			  काष्ठा maintenance_run_opts *opts,
			  काष्ठा gc_config *cfg,
			  क्रमागत task_phase phase,
			  क्रमागत auto_gc_hook_परिणाम *auto_gc_hook_परिणाम)
{
	पूर्णांक foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	स्थिर अक्षर *region = foreground ? "maintenance foreground" : "maintenance";
	पूर्णांक ret = 0;

	अगर (!fn)
		वापस 0;
	अगर (opts->auto_flag) {
		अगर (*auto_gc_hook_परिणाम == AUTO_GC_HOOK_SKIP)
			वापस 0;

		अगर (!task->auto_condition || !task->auto_condition(cfg))
			वापस 0;

		अगर (*auto_gc_hook_परिणाम == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_परिणाम = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		अगर (*auto_gc_hook_परिणाम == AUTO_GC_HOOK_SKIP)
			वापस 0;
	}

	trace2_region_enter(region, task->name, repo);
	अगर (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	वापस ret;
}

अटल पूर्णांक maintenance_run_tasks(काष्ठा maintenance_run_opts *opts,
				 काष्ठा gc_config *cfg)
{
	पूर्णांक परिणाम = 0;
	काष्ठा lock_file lk;
	काष्ठा repository *r = the_repository;
	अक्षर *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	क्रमागत auto_gc_hook_परिणाम auto_gc_hook_परिणाम = AUTO_GC_HOOK_UNDECIDED;

	अगर (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		अगर (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		मुक्त(lock_path);
		वापस 0;
	}
	मुक्त(lock_path);

	क्रम (माप_प्रकार i = 0; i < opts->tasks_nr; i++)
		अगर (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_परिणाम))
			परिणाम = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	अगर (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	क्रम (माप_प्रकार i = 0; i < opts->tasks_nr; i++)
		अगर (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_परिणाम))
			परिणाम = 1;

	rollback_lock_file(&lk);
	वापस परिणाम;
}

क्रमागत maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

काष्ठा maintenance_strategy {
	काष्ठा {
		अचिन्हित type;
		क्रमागत schedule_priority schedule;
	} tasks[TASK_गिनती];
};

अटल स्थिर काष्ठा maintenance_strategy none_strategy = { 0 };

अटल स्थिर काष्ठा maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

अटल स्थिर काष्ठा maintenance_strategy incremental_strategy = {
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

अटल स्थिर काष्ठा maintenance_strategy geometric_strategy = {
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

अटल काष्ठा maintenance_strategy parse_maintenance_strategy(स्थिर अक्षर *name)
{
	अगर (!strcasecmp(name, "incremental"))
		वापस incremental_strategy;
	अगर (!strcasecmp(name, "gc"))
		वापस gc_strategy;
	अगर (!strcasecmp(name, "geometric"))
		वापस geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

अटल व्योम initialize_task_config(काष्ठा maintenance_run_opts *opts,
				   स्थिर काष्ठा string_list *selected_tasks)
{
	काष्ठा strbuf config_name = STRBUF_INIT;
	काष्ठा maintenance_strategy strategy;
	क्रमागत maintenance_type type;
	स्थिर अक्षर *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	अगर (selected_tasks->nr) {
		क्रम (माप_प्रकार i = 0; i < selected_tasks->nr; i++) {
			क्रमागत maintenance_task_label label = (intptr_t)selected_tasks->वस्तुएँ[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		वापस;
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
	अगर (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} अन्यथा {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	अगर (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	क्रम (माप_प्रकार i = 0; i < TASK_गिनती; i++) {
		पूर्णांक config_मूल्य;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		अगर (!repo_config_get_bool(the_repository, config_name.buf, &config_मूल्य))
			strategy.tasks[i].type = config_मूल्य ? type : 0;
		अगर (!(strategy.tasks[i].type & type))
			जारी;

		अगर (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			अगर (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			अगर (strategy.tasks[i].schedule < opts->schedule)
				जारी;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

अटल पूर्णांक task_option_parse(स्थिर काष्ठा option *opt,
			     स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा string_list *selected_tasks = opt->मूल्य;
	माप_प्रकार i;

	BUG_ON_OPT_NEG(unset);

	क्रम (i = 0; i < TASK_गिनती; i++)
		अगर (!strcasecmp(tasks[i].name, arg))
			अवरोध;
	अगर (i >= TASK_गिनती) {
		error(_("'%s' is not a valid task"), arg);
		वापस 1;
	}

	अगर (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		वापस 1;
	}

	string_list_append(selected_tasks, arg)->util = (व्योम *)(intptr_t)i;

	वापस 0;
}

अटल पूर्णांक maintenance_run(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo UNUSED)
{
	काष्ठा maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	काष्ठा string_list selected_tasks = STRING_LIST_INIT_DUP;
	काष्ठा gc_config cfg = GC_CONFIG_INIT;
	काष्ठा option builtin_maintenance_run_options[] = {
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
	पूर्णांक ret;

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

	अगर (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	वापस ret;
}

अटल अक्षर *get_maintpath(व्योम)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	वापस strbuf_detach(&sb, शून्य);
}

अटल अक्षर स्थिर * स्थिर builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	शून्य
};

अटल पूर्णांक maintenance_register(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				काष्ठा repository *repo UNUSED)
{
	अक्षर *config_file = शून्य;
	काष्ठा option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	पूर्णांक found = 0;
	स्थिर अक्षर *कुंजी = "maintenance.repo";
	अक्षर *maintpath = get_maintpath();
	काष्ठा string_list_item *item;
	स्थिर काष्ठा string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	अगर (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	अगर (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	अगर (!repo_config_get_string_multi(the_repository, कुंजी, &list)) {
		for_each_string_list_item(item, list) {
			अगर (!म_भेद(maintpath, item->string)) {
				found = 1;
				अवरोध;
			}
		}
	}

	अगर (!found) {
		पूर्णांक rc;
		अक्षर *global_config_file = शून्य;

		अगर (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		अगर (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, शून्य, 0);
		मुक्त(global_config_file);

		अगर (rc)
			die(_("unable to add '%s' value of '%s'"),
			    कुंजी, maintpath);
	}

	मुक्त(maintpath);
	वापस 0;
}

अटल अक्षर स्थिर * स्थिर builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	शून्य
};

अटल पूर्णांक maintenance_unregister(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				  काष्ठा repository *repo UNUSED)
{
	पूर्णांक force = 0;
	अक्षर *config_file = शून्य;
	काष्ठा option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	स्थिर अक्षर *कुंजी = "maintenance.repo";
	अक्षर *maintpath = get_maintpath();
	पूर्णांक found = 0;
	काष्ठा string_list_item *item;
	स्थिर काष्ठा string_list *list;
	काष्ठा config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	अगर (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	अगर (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	अगर (!(config_file
	      ? git_configset_get_string_multi(&cs, कुंजी, &list)
	      : repo_config_get_string_multi(the_repository, कुंजी, &list))) {
		for_each_string_list_item(item, list) {
			अगर (!म_भेद(maintpath, item->string)) {
				found = 1;
				अवरोध;
			}
		}
	}

	अगर (found) {
		पूर्णांक rc;
		अक्षर *global_config_file = शून्य;

		अगर (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		अगर (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, कुंजी, शून्य, maintpath, शून्य,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_मूल्य);
		मुक्त(global_config_file);

		अगर (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    कुंजी, maintpath);
	} अन्यथा अगर (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	मुक्त(maintpath);
	वापस 0;
}

अटल स्थिर अक्षर *get_frequency(क्रमागत schedule_priority schedule)
{
	चयन (schedule) {
	हाल SCHEDULE_HOURLY:
		वापस "hourly";
	हाल SCHEDULE_DAILY:
		वापस "daily";
	हाल SCHEDULE_WEEKLY:
		वापस "weekly";
	शेष:
		BUG("invalid schedule %d", schedule);
	}
}

अटल स्थिर अक्षर *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	शून्य
};

अटल स्थिर अक्षर *get_extra_config_parameters(व्योम) {
	अटल स्थिर अक्षर *परिणाम = शून्य;
	काष्ठा strbuf builder = STRBUF_INIT;

	अगर (परिणाम)
		वापस परिणाम;

	क्रम (स्थिर अक्षर **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	परिणाम = strbuf_detach(&builder, शून्य);
	वापस परिणाम;
}

अटल स्थिर अक्षर *get_extra_launchctl_strings(व्योम) {
	अटल स्थिर अक्षर *परिणाम = शून्य;
	काष्ठा strbuf builder = STRBUF_INIT;

	अगर (परिणाम)
		वापस परिणाम;

	क्रम (स्थिर अक्षर **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	परिणाम = strbuf_detach(&builder, शून्य);
	वापस परिणाम;
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
अटल पूर्णांक get_schedule_cmd(स्थिर अक्षर *cmd, पूर्णांक *is_available, अक्षर **out)
{
	अक्षर *testing = xstrdup_or_null(दो_पर्या("GIT_TEST_MAINT_SCHEDULER"));
	काष्ठा string_list_item *item;
	काष्ठा string_list list = STRING_LIST_INIT_NODUP;

	अगर (!testing) {
		अगर (out)
			*out = xstrdup(cmd);
		वापस 0;
	}

	अगर (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		काष्ठा string_list pair = STRING_LIST_INIT_NODUP;

		अगर (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			जारी;

		अगर (!म_भेद(cmd, pair.वस्तुएँ[0].string)) {
			अगर (out)
				*out = xstrdup(pair.वस्तुएँ[1].string);
			अगर (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			जाओ out;
		}

		string_list_clear(&pair, 0);
	}

	अगर (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	मुक्त(testing);
	वापस 1;
}

अटल पूर्णांक get_random_minute(व्योम)
{
	/* Use a static value when under tests. */
	अगर (दो_पर्या("GIT_TEST_MAINT_SCHEDULER"))
		वापस 13;

	वापस git_rand(0) % 60;
}

अटल पूर्णांक is_launchctl_available(व्योम)
{
	पूर्णांक is_available;
	अगर (get_schedule_cmd("launchctl", &is_available, शून्य))
		वापस is_available;

#अगर_घोषित __APPLE__
	वापस 1;
#अन्यथा
	वापस 0;
#पूर्ण_अगर
}

अटल अक्षर *launchctl_service_name(स्थिर अक्षर *frequency)
{
	काष्ठा strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	वापस strbuf_detach(&label, शून्य);
}

अटल अक्षर *launchctl_service_filename(स्थिर अक्षर *name)
{
	अक्षर *expanded;
	काष्ठा strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	अगर (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	वापस expanded;
}

अटल अक्षर *launchctl_get_uid(व्योम)
{
	वापस xstrfmt("gui/%d", getuid());
}

अटल पूर्णांक launchctl_boot_plist(पूर्णांक enable, स्थिर अक्षर *filename)
{
	अक्षर *cmd;
	पूर्णांक परिणाम;
	काष्ठा child_process child = CHILD_PROCESS_INIT;
	अक्षर *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", शून्य, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, शून्य);

	child.no_stderr = 1;
	child.no_stdout = 1;

	अगर (start_command(&child))
		die(_("failed to start launchctl"));

	परिणाम = finish_command(&child);

	मुक्त(cmd);
	मुक्त(uid);
	वापस परिणाम;
}

अटल पूर्णांक launchctl_remove_plist(क्रमागत schedule_priority schedule)
{
	स्थिर अक्षर *frequency = get_frequency(schedule);
	अक्षर *name = launchctl_service_name(frequency);
	अक्षर *filename = launchctl_service_filename(name);
	पूर्णांक परिणाम = launchctl_boot_plist(0, filename);
	unlink(filename);
	मुक्त(filename);
	मुक्त(name);
	वापस परिणाम;
}

अटल पूर्णांक launchctl_remove_plists(व्योम)
{
	वापस launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

अटल पूर्णांक launchctl_list_contains_plist(स्थिर अक्षर *name, स्थिर अक्षर *cmd)
{
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, शून्य);

	child.no_stderr = 1;
	child.no_stdout = 1;

	अगर (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	वापस !finish_command(&child);
}

अटल पूर्णांक launchctl_schedule_plist(स्थिर अक्षर *exec_path, क्रमागत schedule_priority schedule)
{
	पूर्णांक i, fd;
	स्थिर अक्षर *preamble, *repeat;
	स्थिर अक्षर *frequency = get_frequency(schedule);
	अक्षर *name = launchctl_service_name(frequency);
	अक्षर *filename = launchctl_service_filename(name);
	काष्ठा lock_file lk = LOCK_INIT;
	अटल अचिन्हित दीर्घ lock_file_timeout_ms = अच_दीर्घ_उच्च;
	काष्ठा strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	काष्ठा stat st;
	अक्षर *cmd;
	पूर्णांक minute = get_random_minute();

	get_schedule_cmd("launchctl", शून्य, &cmd);
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

	चयन (schedule) {
	हाल SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		क्रम (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		अवरोध;

	हाल SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		क्रम (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		अवरोध;

	हाल SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		अवरोध;

	शेष:
		/* unreachable */
		अवरोध;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	अगर (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	अगर ((दीर्घ)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	अगर (!stat(filename, &st) && st.st_आकार == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	अन्यथा {
		अगर (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		अगर (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	मुक्त(filename);
	मुक्त(name);
	मुक्त(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	वापस 0;
}

अटल पूर्णांक launchctl_add_plists(व्योम)
{
	स्थिर अक्षर *exec_path = git_exec_path();

	वापस launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

अटल पूर्णांक launchctl_update_schedule(पूर्णांक run_maintenance, पूर्णांक fd UNUSED)
{
	अगर (run_maintenance)
		वापस launchctl_add_plists();
	अन्यथा
		वापस launchctl_remove_plists();
}

अटल पूर्णांक is_schtasks_available(व्योम)
{
	पूर्णांक is_available;
	अगर (get_schedule_cmd("schtasks", &is_available, शून्य))
		वापस is_available;

#अगर_घोषित GIT_WINDOWS_NATIVE
	वापस 1;
#अन्यथा
	वापस 0;
#पूर्ण_अगर
}

अटल अक्षर *schtasks_task_name(स्थिर अक्षर *frequency)
{
	काष्ठा strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	वापस strbuf_detach(&label, शून्य);
}

अटल पूर्णांक schtasks_remove_task(क्रमागत schedule_priority schedule)
{
	अक्षर *cmd;
	काष्ठा child_process child = CHILD_PROCESS_INIT;
	स्थिर अक्षर *frequency = get_frequency(schedule);
	अक्षर *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", शून्य, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", शून्य);
	मुक्त(name);
	मुक्त(cmd);

	वापस run_command(&child);
}

अटल पूर्णांक schtasks_remove_tasks(व्योम)
{
	वापस schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

अटल पूर्णांक schtasks_schedule_task(स्थिर अक्षर *exec_path, क्रमागत schedule_priority schedule)
{
	अक्षर *cmd;
	पूर्णांक परिणाम;
	काष्ठा child_process child = CHILD_PROCESS_INIT;
	स्थिर अक्षर *xml;
	काष्ठा tempfile *tfile;
	स्थिर अक्षर *frequency = get_frequency(schedule);
	अक्षर *name = schtasks_task_name(frequency);
	काष्ठा strbuf tfilename = STRBUF_INIT;
	पूर्णांक minute = get_random_minute();

	get_schedule_cmd("schtasks", शून्य, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	अगर (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	ख_माला_दो(xml, tfile->fp);

	चयन (schedule) {
	हाल SCHEDULE_HOURLY:
		ख_लिखो(tfile->fp,
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
		अवरोध;

	हाल SCHEDULE_DAILY:
		ख_लिखो(tfile->fp,
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
		अवरोध;

	हाल SCHEDULE_WEEKLY:
		ख_लिखो(tfile->fp,
			"<StartBoundary>2020-01-01T00:%02d:00</StartBoundary>\n"
			"<Enabled>true</Enabled>\n"
			"<ScheduleByWeek>\n"
			"<DaysOfWeek>\n"
			"<Sunday />\n"
			"</DaysOfWeek>\n"
			"<WeeksInterval>1</WeeksInterval>\n"
			"</ScheduleByWeek>\n",
			minute);
		अवरोध;

	शेष:
		अवरोध;
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
	ख_लिखो(tfile->fp, xml, exec_path, exec_path,
		get_extra_config_parameters(), frequency);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/create", "/tn", name, "/f", "/xml",
				  get_tempfile_path(tfile), शून्य);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	अगर (start_command(&child))
		die(_("failed to start schtasks"));
	परिणाम = finish_command(&child);

	delete_tempfile(&tfile);
	मुक्त(name);
	मुक्त(cmd);
	वापस परिणाम;
}

अटल पूर्णांक schtasks_schedule_tasks(व्योम)
{
	स्थिर अक्षर *exec_path = git_exec_path();

	वापस schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

अटल पूर्णांक schtasks_update_schedule(पूर्णांक run_maintenance, पूर्णांक fd UNUSED)
{
	अगर (run_maintenance)
		वापस schtasks_schedule_tasks();
	अन्यथा
		वापस schtasks_remove_tasks();
}

MAYBE_UNUSED
अटल पूर्णांक check_crontab_process(स्थिर अक्षर *cmd)
{
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_धकेलो(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	अगर (start_command(&child))
		वापस 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	वापस 1;
}

अटल पूर्णांक is_crontab_available(व्योम)
{
	अक्षर *cmd;
	पूर्णांक is_available;
	पूर्णांक ret;

	अगर (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		जाओ out;
	}

#अगर_घोषित __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#अन्यथा
	ret = check_crontab_process(cmd);
#पूर्ण_अगर

out:
	मुक्त(cmd);
	वापस ret;
}

#घोषणा BEGIN_पंक्ति "# BEGIN GIT MAINTENANCE SCHEDULE"
#घोषणा END_पंक्ति "# END GIT MAINTENANCE SCHEDULE"

अटल पूर्णांक crontab_update_schedule(पूर्णांक run_maintenance, पूर्णांक fd)
{
	अक्षर *cmd;
	पूर्णांक परिणाम = 0;
	पूर्णांक in_old_region = 0;
	काष्ठा child_process crontab_list = CHILD_PROCESS_INIT;
	काष्ठा child_process crontab_edit = CHILD_PROCESS_INIT;
	खाता *cron_list, *cron_in;
	काष्ठा strbuf पंक्ति = STRBUF_INIT;
	काष्ठा tempfile *tmpedit = शून्य;
	पूर्णांक minute = get_random_minute();

	get_schedule_cmd("crontab", शून्य, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_धकेलो(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	अगर (start_command(&crontab_list)) {
		परिणाम = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		जाओ out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	अगर (!tmpedit) {
		परिणाम = error(_("failed to create crontab temporary file"));
		जाओ out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	अगर (!cron_in) {
		परिणाम = error(_("failed to open temporary file"));
		जाओ out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	शुरुआत(cron_list);

	जबतक (!strbuf_getline_lf(&पंक्ति, cron_list)) {
		अगर (!in_old_region && !म_भेद(पंक्ति.buf, BEGIN_पंक्ति))
			in_old_region = 1;
		अन्यथा अगर (in_old_region && !म_भेद(पंक्ति.buf, END_पंक्ति))
			in_old_region = 0;
		अन्यथा अगर (!in_old_region)
			ख_लिखो(cron_in, "%s\n", पंक्ति.buf);
	}
	strbuf_release(&पंक्ति);

	अगर (run_maintenance) {
		काष्ठा strbuf पंक्ति_format = STRBUF_INIT;
		स्थिर अक्षर *exec_path = git_exec_path();

		ख_लिखो(cron_in, "%s\n", BEGIN_पंक्ति);
		ख_लिखो(cron_in,
			"# The following schedule was created by Git\n");
		ख_लिखो(cron_in, "# Any edits made in this region might be\n");
		ख_लिखो(cron_in,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&पंक्ति_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_path, exec_path, get_extra_config_parameters());
		ख_लिखो(cron_in, पंक्ति_format.buf, minute, "1-23", "*", "hourly");
		ख_लिखो(cron_in, पंक्ति_format.buf, minute, "0", "1-6", "daily");
		ख_लिखो(cron_in, पंक्ति_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&पंक्ति_format);

		ख_लिखो(cron_in, "\n%s\n", END_पंक्ति);
	}

	ख_साफ(cron_in);

	strvec_split(&crontab_edit.args, cmd);
	strvec_धकेलो(&crontab_edit.args, get_tempfile_path(tmpedit));
	crontab_edit.git_cmd = 0;

	अगर (start_command(&crontab_edit)) {
		परिणाम = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		जाओ out;
	}

	अगर (finish_command(&crontab_edit))
		परिणाम = error(_("'crontab' died"));
	अन्यथा
		ख_बंद(cron_list);

out:
	delete_tempfile(&tmpedit);
	मुक्त(cmd);
	वापस परिणाम;
}

अटल पूर्णांक real_is_systemd_timer_available(व्योम)
{
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", शून्य);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	अगर (start_command(&child))
		वापस 0;
	अगर (finish_command(&child))
		वापस 0;
	वापस 1;
}

अटल पूर्णांक is_systemd_timer_available(व्योम)
{
	पूर्णांक is_available;

	अगर (get_schedule_cmd("systemctl", &is_available, शून्य))
		वापस is_available;

	वापस real_is_systemd_timer_available();
}

अटल अक्षर *xdg_config_home_systemd(स्थिर अक्षर *filename)
{
	वापस xdg_config_home_for("systemd/user", filename);
}

#घोषणा SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

अटल पूर्णांक systemd_timer_delete_timer_file(क्रमागत schedule_priority priority)
{
	पूर्णांक ret = 0;
	स्थिर अक्षर *frequency = get_frequency(priority);
	अक्षर *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	अक्षर *filename = xdg_config_home_systemd(local_timer_name);

	अगर (unlink(filename) && !is_missing_file_error(त्रुटि_सं))
		ret = error_errno(_("failed to delete '%s'"), filename);

	मुक्त(filename);
	मुक्त(local_timer_name);
	वापस ret;
}

अटल पूर्णांक systemd_timer_delete_service_template(व्योम)
{
	पूर्णांक ret = 0;
	अक्षर *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	अक्षर *filename = xdg_config_home_systemd(local_service_name);
	अगर (unlink(filename) && !is_missing_file_error(त्रुटि_सं))
		ret = error_errno(_("failed to delete '%s'"), filename);

	मुक्त(filename);
	मुक्त(local_service_name);
	वापस ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
अटल पूर्णांक systemd_timer_write_timer_file(क्रमागत schedule_priority schedule,
					  पूर्णांक minute)
{
	पूर्णांक res = -1;
	अक्षर *filename;
	खाता *file;
	स्थिर अक्षर *unit;
	अक्षर *schedule_pattern = शून्य;
	स्थिर अक्षर *frequency = get_frequency(schedule);
	अक्षर *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	अगर (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		जाओ error;
	}
	file = fopen_or_warn(filename, "w");
	अगर (!file)
		जाओ error;

	चयन (schedule) {
	हाल SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		अवरोध;

	हाल SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		अवरोध;

	हाल SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		अवरोध;

	शेष:
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
	अगर (ख_लिखो(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		ख_बंद(file);
		जाओ error;
	}
	अगर (ख_बंद(file) == खातापूर्ण) {
		error_errno(_("failed to flush '%s'"), filename);
		जाओ error;
	}

	res = 0;

error:
	मुक्त(schedule_pattern);
	मुक्त(local_timer_name);
	मुक्त(filename);
	वापस res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
अटल पूर्णांक systemd_timer_write_service_template(स्थिर अक्षर *exec_path)
{
	पूर्णांक res = -1;
	अक्षर *filename;
	खाता *file;
	स्थिर अक्षर *unit;
	अक्षर *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	अगर (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		जाओ error;
	}
	file = fopen_or_warn(filename, "w");
	अगर (!file)
		जाओ error;

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
	अगर (ख_लिखो(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		ख_बंद(file);
		जाओ error;
	}
	अगर (ख_बंद(file) == खातापूर्ण) {
		error_errno(_("failed to flush '%s'"), filename);
		जाओ error;
	}

	res = 0;

error:
	मुक्त(local_service_name);
	मुक्त(filename);
	वापस res;
}

अटल पूर्णांक systemd_timer_enable_unit(पूर्णांक enable,
				     क्रमागत schedule_priority schedule,
				     पूर्णांक minute)
{
	अक्षर *cmd = शून्य;
	काष्ठा child_process child = CHILD_PROCESS_INIT;
	स्थिर अक्षर *frequency = get_frequency(schedule);
	पूर्णांक ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	अगर (!enable) {
		child.no_stderr = 1;
	} अन्यथा अगर (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		जाओ out;
	}

	get_schedule_cmd("systemctl", शून्य, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", शून्य);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	अगर (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		जाओ out;
	}

	अगर (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		अगर (enable) {
			ret = error(_("failed to run systemctl"));
			जाओ out;
		}
	}

	ret = 0;

out:
	मुक्त(cmd);
	वापस ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
अटल व्योम systemd_timer_delete_stale_timer_templates(व्योम)
{
	अक्षर *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	अक्षर *filename = xdg_config_home_systemd(timer_template_name);

	अगर (unlink(filename) && !is_missing_file_error(त्रुटि_सं))
		warning(_("failed to delete '%s'"), filename);

	मुक्त(filename);
	मुक्त(timer_template_name);
}

अटल पूर्णांक systemd_timer_delete_unit_files(व्योम)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	वापस systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

अटल पूर्णांक systemd_timer_delete_units(व्योम)
{
	पूर्णांक minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	वापस systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

अटल पूर्णांक systemd_timer_setup_units(व्योम)
{
	पूर्णांक minute = get_random_minute();
	स्थिर अक्षर *exec_path = git_exec_path();

	पूर्णांक ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	अगर (ret)
		systemd_timer_delete_units();
	अन्यथा
		systemd_timer_delete_stale_timer_templates();

	वापस ret;
}

अटल पूर्णांक systemd_timer_update_schedule(पूर्णांक run_maintenance, पूर्णांक fd UNUSED)
{
	अगर (run_maintenance)
		वापस systemd_timer_setup_units();
	अन्यथा
		वापस systemd_timer_delete_units();
}

क्रमागत scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

अटल स्थिर काष्ठा {
	स्थिर अक्षर *name;
	पूर्णांक (*is_available)(व्योम);
	पूर्णांक (*update_schedule)(पूर्णांक run_maintenance, पूर्णांक fd);
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

अटल क्रमागत scheduler parse_scheduler(स्थिर अक्षर *मूल्य)
{
	अगर (!मूल्य)
		वापस SCHEDULER_INVALID;
	अन्यथा अगर (!strcasecmp(मूल्य, "auto"))
		वापस SCHEDULER_AUTO;
	अन्यथा अगर (!strcasecmp(मूल्य, "cron") || !strcasecmp(मूल्य, "crontab"))
		वापस SCHEDULER_CRON;
	अन्यथा अगर (!strcasecmp(मूल्य, "systemd") ||
		 !strcasecmp(मूल्य, "systemd-timer"))
		वापस SCHEDULER_SYSTEMD;
	अन्यथा अगर (!strcasecmp(मूल्य, "launchctl"))
		वापस SCHEDULER_LAUNCHCTL;
	अन्यथा अगर (!strcasecmp(मूल्य, "schtasks"))
		वापस SCHEDULER_SCHTASKS;
	अन्यथा
		वापस SCHEDULER_INVALID;
}

अटल पूर्णांक maintenance_opt_scheduler(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
				     पूर्णांक unset)
{
	क्रमागत scheduler *scheduler = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	अगर (*scheduler == SCHEDULER_INVALID)
		वापस error(_("unrecognized --scheduler argument '%s'"), arg);
	वापस 0;
}

काष्ठा maintenance_start_opts {
	क्रमागत scheduler scheduler;
};

अटल क्रमागत scheduler resolve_scheduler(क्रमागत scheduler scheduler)
{
	अगर (scheduler != SCHEDULER_AUTO)
		वापस scheduler;

#अगर defined(__APPLE__)
	वापस SCHEDULER_LAUNCHCTL;

#या_अगर defined(GIT_WINDOWS_NATIVE)
	वापस SCHEDULER_SCHTASKS;

#या_अगर defined(__linux__)
	अगर (is_systemd_timer_available())
		वापस SCHEDULER_SYSTEMD;
	अन्यथा अगर (is_crontab_available())
		वापस SCHEDULER_CRON;
	अन्यथा
		die(_("neither systemd timers nor crontab are available"));

#अन्यथा
	वापस SCHEDULER_CRON;
#पूर्ण_अगर
}

अटल व्योम validate_scheduler(क्रमागत scheduler scheduler)
{
	अगर (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	अगर (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	अगर (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

अटल पूर्णांक update_background_schedule(स्थिर काष्ठा maintenance_start_opts *opts,
				      पूर्णांक enable)
{
	अचिन्हित पूर्णांक i;
	पूर्णांक परिणाम = 0;
	काष्ठा lock_file lk;
	अक्षर *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	अगर (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		अगर (त्रुटि_सं == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), म_त्रुटि(त्रुटि_सं));
		अन्यथा
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		मुक्त(lock_path);
		वापस -1;
	}

	क्रम (i = 1; i < ARRAY_आकार(scheduler_fn); i++) {
		अगर (enable && opts->scheduler == i)
			जारी;
		अगर (!scheduler_fn[i].is_available())
			जारी;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	अगर (enable)
		परिणाम = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	मुक्त(lock_path);
	वापस परिणाम;
}

अटल स्थिर अक्षर *स्थिर builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	शून्य
};

अटल पूर्णांक maintenance_start(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			     काष्ठा repository *repo)
{
	काष्ठा maintenance_start_opts opts = { 0 };
	काष्ठा option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	स्थिर अक्षर *register_args[] = { "register", शून्य };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	अगर (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	अगर (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	अगर (maintenance_register(ARRAY_आकार(register_args)-1, register_args, शून्य, repo))
		warning(_("failed to add repo to global config"));
	वापस 0;
}

अटल स्थिर अक्षर *स्थिर builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	शून्य
};

अटल पूर्णांक maintenance_stop(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			    काष्ठा repository *repo UNUSED)
{
	काष्ठा option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	अगर (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	वापस update_background_schedule(शून्य, 0);
}

अटल स्थिर अक्षर *स्थिर builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	शून्य
};

अटल पूर्णांक maintenance_is_needed(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				 काष्ठा repository *repo UNUSED)
{
	काष्ठा maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	काष्ठा string_list selected_tasks = STRING_LIST_INIT_DUP;
	काष्ठा gc_config cfg = GC_CONFIG_INIT;
	काष्ठा option options[] = {
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
	अगर (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	अगर (opts.auto_flag) {
		क्रम (माप_प्रकार i = 0; i < opts.tasks_nr; i++) {
			अगर (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				अवरोध;
			}
		}
	} अन्यथा {
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

	अगर (is_needed)
		वापस 0;
	वापस 1;
}

अटल स्थिर अक्षर *स्थिर builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	शून्य,
};

पूर्णांक cmd_maintenance(पूर्णांक argc,
		    स्थिर अक्षर **argv,
		    स्थिर अक्षर *prefix,
		    काष्ठा repository *repo)
{
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option builtin_maintenance_options[] = {
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
	वापस fn(argc, argv, prefix, repo);
}
