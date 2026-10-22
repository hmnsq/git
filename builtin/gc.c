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

#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کن FAILED_پیاپی "failed to run %s"

ساكن ثابت حرف * ثابت builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	عدم
};

ساكن timestamp_t gc_log_expire_زمان;
ساكن بنية tempfile *pidfile;
ساكن بنية قفل_file log_قفل;
ساكن بنية string_list pack_garbage = STRING_LIST_INIT_DUP;

ساكن فراغ clean_pack_garbage(فراغ)
{
	صحيح i;
	لكل (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.اقلام[i].string);
	string_list_clear(&pack_garbage, 0);
}

ساكن فراغ report_pack_garbage(بیعلامت دیده_بیتها, ثابت حرف *مسیر)
{
	إذا (دیده_بیتها == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, مسیر);
}

ساكن فراغ process_log_file(فراغ)
{
	بنية stat st;
	إذا (fstat(get_قفل_file_fd(&log_قفل), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		صحيح saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_قفل_file_مسیر(&log_قفل),
			strerror(saved_errno));
		fflush(stderr);
		commit_قفل_file(&log_قفل);
		errno = saved_errno;
	} وإلا إذا (st.st_اندازه) {
		/* There was some error recorded in the lock file */
		commit_قفل_file(&log_قفل);
	} وإلا {
		حرف *مسیر = repo_git_مسیر(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(مسیر);
		rollback_قفل_file(&log_قفل);
		حرر(مسیر);
	}
}

ساكن فراغ process_log_file_at_exit(فراغ)
{
	fflush(stderr);
	process_log_file();
}

ساكن صحيح gc_config_is_timestamp_never(ثابت حرف *var)
{
	ثابت حرف *مقدار;
	timestamp_t expire;

	إذا (!repo_config_get_مقدار(the_repository, var, &مقدار) && مقدار) {
		إذا (parse_expiry_date(مقدار, &expire))
			die(_("failed to parse '%s' value '%s'"), var, مقدار);
		أرجع expire == 0;
	}
	أرجع 0;
}

بنية gc_config {
	صحيح pack_refs;
	صحيح prune_reflogs;
	صحيح cruft_packs;
	بیعلامت دراز max_cruft_اندازه;
	صحيح aggressive_عمق;
	صحيح aggressive_پنجره;
	صحيح detach_auto;
	حرف *gc_log_expire;
	حرف *prune_expire;
	حرف *prune_worktrees_expire;
	حرف *repack_expire_مقصد;
};

#تعریف_کن GC_CONFIG_INIT { \
	.pack_refs = 1, \
	.prune_reflogs = 1, \
	.cruft_packs = 1, \
	.aggressive_عمق = 50, \
	.aggressive_پنجره = 250, \
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
	ثابت حرف *مقدار;
	حرف *owned = عدم;

	إذا (!repo_config_get_مقدار(the_repository, "gc.packrefs", &مقدار)) {
		إذا (مقدار && !strcmp(مقدار, "notbare"))
			cfg->pack_refs = -1;
		وإلا
			cfg->pack_refs = git_config_bool("gc.packrefs", مقدار);
	}

	إذا (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_پنجره);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_عمق);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_اندازه);

	إذا (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		حرر(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	إذا (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		حرر(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	إذا (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		حرر(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, عدم);
}

تعداد زمانبندی_اولویت {
	زمانبندی_NONE = 0,
	زمانبندی_WEEKLY = 1,
	زمانبندی_DAILY = 2,
	زمانبندی_HOURLY = 3,
};

ساكن تعداد زمانبندی_اولویت parse_زمانبندی(ثابت حرف *مقدار)
{
	إذا (!مقدار)
		أرجع زمانبندی_NONE;
	إذا (!strcasecmp(مقدار, "hourly"))
		أرجع زمانبندی_HOURLY;
	إذا (!strcasecmp(مقدار, "daily"))
		أرجع زمانبندی_DAILY;
	إذا (!strcasecmp(مقدار, "weekly"))
		أرجع زمانبندی_WEEKLY;
	أرجع زمانبندی_NONE;
}

تعداد maintenance_task_label {
	TASK_PREFETCH,
	TASK_LOOSE_OBJECTS,
	TASK_INCREMENTAL_REPACK,
	TASK_GEOMETRIC_REPACK,
	TASK_GC,
	TASK_COMMIT_گراف,
	TASK_PACK_REFS,
	TASK_REFLOG_EXPIRE,
	TASK_WORKTREE_PRUNE,
	TASK_RERERE_GC,

	/* Leave as final value */
	TASK_شمار
};

بنية maintenance_پیاپی_opts {
	تعداد maintenance_task_label *tasks;
	اندازه_t tasks_nr, tasks_alloc;
	صحيح auto_flag;
	صحيح detach;
	صحيح quiet;
	تعداد زمانبندی_اولویت زمانبندی;
};
#تعریف_کن MAINTENANCE_پیاپی_OPTS_INIT { \
	.detach = -1, \
}

ساكن فراغ maintenance_پیاپی_opts_release(بنية maintenance_پیاپی_opts *opts)
{
	حرر(opts->tasks);
}

ساكن صحيح pack_refs_condition(استفادهنشده بنية gc_config *cfg)
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

	إذا (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		أرجع 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	أرجع required;
}

ساكن صحيح maintenance_task_pack_refs(بنية maintenance_پیاپی_opts *opts,
				      استفادهنشده بنية gc_config *cfg)
{
	بنية فرزند_process cmd = فرزند_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.آرگومانها, "pack-refs", "--all", "--prune", عدم);
	إذا (opts->auto_flag)
		strvec_افزودن(&cmd.آرگومانها, "--auto");

	أرجع پیاپی_command(&cmd);
}

بنية شمار_reflog_entries_داده {
	بنية expire_reflog_policy_cb policy;
	اندازه_t شمار;
	اندازه_t حد;
};

ساكن صحيح شمار_reflog_entries(ثابت حرف *refname استفادهنشده,
				بنية object_id *old_oid, بنية object_id *new_oid,
				ثابت حرف *committer, timestamp_t timestamp,
				صحيح tz, ثابت حرف *پیام, فراغ *cb_داده)
{
	بنية شمار_reflog_entries_داده *داده = cb_داده;
	إذا (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, پیام, &داده->policy))
		داده->شمار++;
	أرجع داده->شمار >= داده->حد;
}

ساكن صحيح reflog_expire_condition(بنية gc_config *cfg استفادهنشده)
{
	timestamp_t now = زمان(عدم);
	بنية شمار_reflog_entries_داده داده = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	صحيح حد = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &حد);
	إذا (!حد)
		أرجع 0;
	إذا (حد < 0)
		أرجع 1;
	داده.حد = حد;

	repo_config(the_repository, reflog_expire_config, &داده.policy.opts);

	reflog_expire_options_مجموعه_refname(&داده.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 شمار_reflog_entries, &داده);

	reflog_expiry_cleanup(&داده.policy);
	reflog_clear_expire_config(&داده.policy.opts);
	أرجع داده.شمار >= داده.حد;
}

ساكن صحيح maintenance_task_reflog_expire(بنية maintenance_پیاپی_opts *opts استفادهنشده,
					  بنية gc_config *cfg استفادهنشده)
{
	بنية فرزند_process cmd = فرزند_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.آرگومانها, "reflog", "expire", "--all", عدم);
	أرجع پیاپی_command(&cmd);
}

ساكن صحيح maintenance_task_worktree_prune(بنية maintenance_پیاپی_opts *opts استفادهنشده,
					   بنية gc_config *cfg)
{
	بنية فرزند_process prune_worktrees_cmd = فرزند_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.آرگومانها, "worktree", "prune", "--expire", عدم);
	strvec_افزودن(&prune_worktrees_cmd.آرگومانها, cfg->prune_worktrees_expire);

	أرجع پیاپی_command(&prune_worktrees_cmd);
}

ساكن صحيح worktree_prune_condition(بنية gc_config *cfg)
{
	بنية strbuf buf = STRBUF_INIT;
	صحيح should_prune = 0, حد = 1;
	timestamp_t expiry_date;
	بنية dirent *d;
	DIR *dir = عدم;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &حد);
	إذا (حد <= 0) {
		should_prune = حد < 0;
		اذهب بیرون;
	}

	إذا (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		اذهب بیرون;

	dir = opendir(repo_git_مسیر_replace(the_repository, &buf, "worktrees"));
	إذا (!dir)
		اذهب بیرون;

	طالما (حد && (d = readdir_skip_dot_and_dotdot(dir))) {
		حرف *wtpath;
		strbuf_reset(&buf);
		إذا (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			حد--;
		حرر(wtpath);
	}

	should_prune = !حد;

بیرون:
	إذا (dir)
		closedir(dir);
	strbuf_release(&buf);
	أرجع should_prune;
}

ساكن صحيح maintenance_task_rerere_gc(بنية maintenance_پیاپی_opts *opts استفادهنشده,
				      بنية gc_config *cfg استفادهنشده)
{
	بنية فرزند_process rerere_cmd = فرزند_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.آرگومانها, "rerere", "gc", عدم);
	أرجع پیاپی_command(&rerere_cmd);
}

ساكن صحيح rerere_gc_condition(بنية gc_config *cfg استفادهنشده)
{
	صحيح حد = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &حد);
	إذا (!حد)
		أرجع 0; /* never prune */
	إذا (حد < 0)
		أرجع 1; /* always prune */

	أرجع rerere_gc_needed(the_repository, (اندازه_t)حد);
}

#تعریف_کن OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_مقصد = (cfg)->repack_expire_مقصد, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_اندازه = (cfg)->max_cruft_اندازه, \
	.پنجره = (aggressive) ? (cfg)->aggressive_پنجره : 0, \
	.عمق = (aggressive) ? (cfg)->aggressive_عمق : 0

/* return NULL on success, else hostname running the gc */
ساكن ثابت حرف *قفل_repo_for_gc(صحيح force, pid_t* ret_pid)
{
	بنية قفل_file قفل = قفل_INIT;
	حرف my_host[HOST_NAME_MAX + 1];
	بنية strbuf sb = STRBUF_INIT;
	بنية stat st;
	uintmax_t pid;
	FILE *fp;
	صحيح fd;
	حرف *pidfile_مسیر;

	إذا (is_tempfile_فعال(pidfile))
		/* already locked */
		أرجع عدم;

	إذا (xgethostname(my_host, حجم(my_host)))
		xsnprintf(my_host, حجم(my_host), "unknown");

	pidfile_مسیر = repo_git_مسیر(the_repository, "gc.pid");
	fd = hold_قفل_file_for_update(&قفل, pidfile_مسیر,
				       قفل_DIE_ON_ERROR);
	إذا (!force) {
		ساكن حرف locking_host[HOST_NAME_MAX + 1];
		ساكن حرف *پیمایش_fmt;
		صحيح should_exit;

		إذا (!پیمایش_fmt)
			پیمایش_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_مسیر, "r");
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
			زمان(عدم) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, پیمایش_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		إذا (fp)
			fclose(fp);
		إذا (should_exit) {
			إذا (fd >= 0)
				rollback_قفل_file(&قفل);
			*ret_pid = pid;
			حرر(pidfile_مسیر);
			أرجع locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_درون_پر(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_قفل_file(&قفل);
	pidfile = ثبات_tempfile(pidfile_مسیر);
	حرر(pidfile_مسیر);
	أرجع عدم;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
ساكن صحيح report_آخرین_gc_error(فراغ)
{
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret = 0;
	ssize_t len;
	بنية stat st;
	حرف *gc_log_مسیر = repo_git_مسیر(the_repository, "gc.log");

	إذا (stat(gc_log_مسیر, &st)) {
		إذا (errno == ENOENT)
			اذهب انجامشده;

		ret = die_پیام_errno(_("cannot stat '%s'"), gc_log_مسیر);
		اذهب انجامشده;
	}

	إذا (st.st_mtime < gc_log_expire_زمان)
		اذهب انجامشده;

	len = strbuf_read_file(&sb, gc_log_مسیر, 0);
	إذا (len < 0)
		ret = die_پیام_errno(_("cannot read '%s'"), gc_log_مسیر);
	وإلا إذا (len > 0) {
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
			    gc_log_مسیر, sb.buf);
		ret = 1;
	}
	strbuf_release(&sb);
انجامشده:
	حرر(gc_log_مسیر);
	أرجع ret;
}

ساكن صحيح gc_foreground_tasks(بنية maintenance_پیاپی_opts *opts,
			       بنية gc_config *cfg)
{
	إذا (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		أرجع error(FAILED_پیاپی, "pack-refs");
	إذا (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		أرجع error(FAILED_پیاپی, "reflog");
	أرجع 0;
}

ساكن صحيح maintenance_task_odb(بنية maintenance_پیاپی_opts *opts,
				بنية gc_config *cfg,
				صحيح keep_بزرگترین_pack,
				صحيح aggressive)
{
	بنية odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_بزرگترین_pack = keep_بزرگترین_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	إذا (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	إذا (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	إذا (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	أرجع odb_optimize(the_repository->objects, &odb_opts);
}

صحيح cmd_gc(صحيح argc,
	   ثابت حرف **argv,
	   ثابت حرف *پیشوند,
	   بنية repository *repo استفادهنشده)
{
	صحيح aggressive = 0;
	صحيح force = 0;
	ثابت حرف *name;
	pid_t pid;
	صحيح daemonized = 0;
	صحيح keep_بزرگترین_pack = -1;
	صحيح skip_foreground_tasks = 0;
	timestamp_t dummy;
	بنية maintenance_پیاپی_opts opts = MAINTENANCE_پیاپی_OPTS_INIT;
	بنية gc_config cfg = GC_CONFIG_INIT;
	ثابت حرف *prune_expire_sentinel = "sentinel";
	ثابت حرف *prune_expire_arg = prune_expire_sentinel;
	صحيح ret;
	بنية option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.مقدار = &prune_expire_arg,
			.argh = N_("date"),
			.help = N_("prune unreferenced objects"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)prune_expire_arg,
		},
		OPT_BOOL(0, "cruft", &cfg.cruft_packs, N_("pack unreferenced objects separately")),
		OPT_UNSIGNED(0, "max-cruft-size", &cfg.max_cruft_اندازه,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL(0, "aggressive", &aggressive, N_("be more thorough (increased runtime)")),
		OPT_BOOL_F(0, "auto", &opts.auto_flag, N_("enable auto-gc mode"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform garbage collection in the background")),
		OPT_BOOL_F(0, "force", &force,
			   N_("force running gc even if there may be another gc running"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "keep-largest-pack", &keep_بزرگترین_pack,
			 N_("repack all other packs except the largest pack")),
		OPT_STRING(0, "expire-to", &cfg.repack_expire_مقصد, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_HIDDEN_BOOL(0, "skip-foreground-tasks", &skip_foreground_tasks,
			   N_("skip maintenance tasks typically done in the foreground")),
		OPT_انتها()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_gc_usage, builtin_gc_options);

	gc_config(&cfg);

	إذا (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_زمان))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	إذا (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, پیشوند, builtin_gc_options,
			     builtin_gc_usage, 0);
	إذا (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	إذا (prune_expire_arg != prune_expire_sentinel) {
		حرر(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	إذا (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	إذا (opts.auto_flag) {
		بنية odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		إذا (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		إذا (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    پیاپی_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			اذهب بیرون;
		}

		إذا (!opts.quiet) {
			إذا (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			وإلا
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	إذا (opts.detach > 0) {
		ret = report_آخرین_gc_error();
		إذا (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			اذهب بیرون;

		} وإلا إذا (ret) {
			/* an I/O error occurred, already reported */
			اذهب بیرون;
		}

		إذا (!skip_foreground_tasks) {
			إذا (قفل_repo_for_gc(force, &pid)) {
				ret = 0;
				اذهب بیرون;
			}

			إذا (gc_foreground_tasks(&opts, &cfg) < 0)
				die(عدم);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = قفل_repo_for_gc(force, &pid);
	إذا (name) {
		إذا (opts.auto_flag) {
			ret = 0;
			اذهب بیرون; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	إذا (daemonized) {
		حرف *مسیر = repo_git_مسیر(the_repository, "gc.log");
		hold_قفل_file_for_update(&log_قفل, مسیر,
					  قفل_DIE_ON_ERROR);
		dup2(get_قفل_file_fd(&log_قفل), 2);
		atexit(process_log_file_at_exit);
		حرر(مسیر);
	}

	إذا (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	إذا (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_پیاپی, "worktree");

	إذا (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_پیاپی, "rerere");

	إذا (maintenance_task_odb(&opts, &cfg, keep_بزرگترین_pack, aggressive))
		die(عدم);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	إذا (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	إذا (the_repository->settings.gc_write_commit_گراف == 1)
		write_commit_گراف_reachable(the_repository,
					     the_repository->objects->منبع->مسیر,
					     !opts.quiet && !daemonized ? COMMIT_گراف_WRITE_پیشرفت : 0,
					     عدم);

	إذا (!daemonized) {
		حرف *مسیر = repo_git_مسیر(the_repository, "gc.log");
		unlink(مسیر);
		حرر(مسیر);
	}

بیرون:
	maintenance_پیاپی_opts_release(&opts);
	gc_config_release(&cfg);
	أرجع 0;
}

ساكن ثابت حرف *ثابت builtin_maintenance_پیاپی_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	عدم
};

ساكن صحيح maintenance_opt_زمانبندی(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	تعداد زمانبندی_اولویت *اولویت = opt->مقدار;

	إذا (unset)
		die(_("--no-schedule is not allowed"));

	*اولویت = parse_زمانبندی(arg);

	إذا (!*اولویت)
		die(_("unrecognized --schedule argument '%s'"), arg);

	أرجع 0;
}

بنية cg_auto_داده {
	صحيح رقم_not_درون_گراف;
	صحيح حد;
};

ساكن صحيح dfs_on_ref(ثابت بنية reference *ref, فراغ *cb_داده)
{
	بنية cg_auto_داده *داده = (بنية cg_auto_داده *)cb_داده;
	صحيح نتیجه = 0;
	ثابت بنية object_id *maybe_peeled = ref->oid;
	بنية object_id peeled;
	بنية commit_list *پشته = عدم;
	بنية commit *commit;

	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	إذا (odb_read_object_info(the_repository->objects, maybe_peeled, عدم) != OBJ_COMMIT)
		أرجع 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	إذا (!commit || commit->object.flags & دیده_2)
		أرجع 0;
	commit->object.flags |= دیده_2;

	إذا (repo_parse_commit(the_repository, commit) ||
	    commit_گراف_position(commit) != COMMIT_NOT_FROM_گراف)
		أرجع 0;

	داده->رقم_not_درون_گراف++;

	إذا (داده->رقم_not_درون_گراف >= داده->حد)
		أرجع 1;

	commit_list_درج(commit, &پشته);

	طالما (!نتیجه && پشته) {
		بنية commit_list *والد;

		commit = برداشت_commit(&پشته);

		لكل (والد = commit->parents; والد; والد = والد->بعدی) {
			إذا (repo_parse_commit(the_repository, والد->قلم) ||
			    commit_گراف_position(والد->قلم) != COMMIT_NOT_FROM_گراف ||
			    والد->قلم->object.flags & دیده_2)
				استمر;

			والد->قلم->object.flags |= دیده_2;
			داده->رقم_not_درون_گراف++;

			إذا (داده->رقم_not_درون_گراف >= داده->حد) {
				نتیجه = 1;
				اكسر;
			}

			commit_list_درج(والد->قلم, &پشته);
		}
	}

	commit_list_free(پشته);
	أرجع نتیجه;
}

ساكن صحيح should_write_commit_گراف(بنية gc_config *cfg استفادهنشده)
{
	صحيح نتیجه;
	بنية cg_auto_داده داده;

	داده.رقم_not_درون_گراف = 0;
	داده.حد = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &داده.حد);

	إذا (!داده.حد)
		أرجع 0;
	إذا (داده.حد < 0)
		أرجع 1;

	نتیجه = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &داده);

	repo_clear_commit_marks(the_repository, دیده_2);

	أرجع نتیجه;
}

ساكن صحيح پیاپی_write_commit_گراف(بنية maintenance_پیاپی_opts *opts)
{
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;

	فرزند.git_cmd = 1;
	فرزند.odb_مقصد_close = the_repository->objects;
	strvec_pushl(&فرزند.آرگومانها, "commit-graph", "write",
		     "--split", "--reachable", عدم);

	إذا (opts->quiet)
		strvec_افزودن(&فرزند.آرگومانها, "--no-progress");
	وإلا
		strvec_افزودن(&فرزند.آرگومانها, "--progress");

	أرجع !!پیاپی_command(&فرزند);
}

ساكن صحيح maintenance_task_commit_گراف(بنية maintenance_پیاپی_opts *opts,
					 بنية gc_config *cfg استفادهنشده)
{
	prepare_repo_settings(the_repository);
	إذا (!the_repository->settings.core_commit_گراف)
		أرجع 0;

	إذا (پیاپی_write_commit_گراف(opts)) {
		error(_("failed to write commit-graph"));
		أرجع 1;
	}

	أرجع 0;
}

ساكن صحيح fetch_remote(بنية remote *remote, فراغ *cbdata)
{
	بنية maintenance_پیاپی_opts *opts = cbdata;
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;

	إذا (remote->skip_default_update)
		أرجع 0;

	فرزند.git_cmd = 1;
	strvec_pushl(&فرزند.آرگومانها, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     عدم);

	إذا (opts->quiet)
		strvec_افزودن(&فرزند.آرگومانها, "--quiet");

	أرجع !!پیاپی_command(&فرزند);
}

ساكن صحيح maintenance_task_prefetch(بنية maintenance_پیاپی_opts *opts,
				     بنية gc_config *cfg استفادهنشده)
{
	إذا (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		أرجع 1;
	}

	أرجع 0;
}

ساكن صحيح maintenance_task_gc_foreground(بنية maintenance_پیاپی_opts *opts,
					  بنية gc_config *cfg)
{
	أرجع gc_foreground_tasks(opts, cfg);
}

ساكن صحيح maintenance_task_gc_background(بنية maintenance_پیاپی_opts *opts,
					  بنية gc_config *cfg استفادهنشده)
{
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;

	فرزند.git_cmd = 1;
	فرزند.odb_مقصد_close = the_repository->objects;
	strvec_افزودن(&فرزند.آرگومانها, "gc");

	إذا (opts->auto_flag)
		strvec_افزودن(&فرزند.آرگومانها, "--auto");
	إذا (opts->quiet)
		strvec_افزودن(&فرزند.آرگومانها, "--quiet");
	وإلا
		strvec_افزودن(&فرزند.آرگومانها, "--no-quiet");
	strvec_افزودن(&فرزند.آرگومانها, "--no-detach");
	strvec_افزودن(&فرزند.آرگومانها, "--skip-foreground-tasks");

	أرجع پیاپی_command(&فرزند);
}

ساكن صحيح gc_condition(بنية gc_config *cfg)
{
	بنية odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	أرجع odb_optimize_required(the_repository->objects, &opts);
}

ساكن صحيح prune_packed(بنية maintenance_پیاپی_opts *opts)
{
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;

	فرزند.git_cmd = 1;
	strvec_افزودن(&فرزند.آرگومانها, "prune-packed");

	إذا (opts->quiet)
		strvec_افزودن(&فرزند.آرگومانها, "--quiet");

	أرجع !!پیاپی_command(&فرزند);
}

بنية write_loose_object_داده {
	FILE *درون;
	صحيح شمار;
	صحيح batch_اندازه;
};

ساكن صحيح loose_object_auto_حد = 100;

ساكن صحيح loose_object_شمار(ثابت بنية object_id *oid استفادهنشده,
			      ثابت حرف *مسیر استفادهنشده,
			      فراغ *داده)
{
	صحيح *شمار = (صحيح*)داده;
	إذا (++(*شمار) >= loose_object_auto_حد)
		أرجع 1;
	أرجع 0;
}

ساكن صحيح loose_object_auto_condition(بنية gc_config *cfg استفادهنشده)
{
	صحيح شمار = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_حد);

	إذا (!loose_object_auto_حد)
		أرجع 0;
	إذا (loose_object_auto_حد < 0)
		أرجع 1;

	أرجع for_each_loose_file_درون_منبع(the_repository->objects->منبع,
					     loose_object_شمار,
					     عدم, عدم, &شمار);
}

ساكن صحيح bail_on_loose(ثابت بنية object_id *oid استفادهنشده,
			 ثابت حرف *مسیر استفادهنشده,
			 فراغ *داده استفادهنشده)
{
	أرجع 1;
}

ساكن صحيح write_loose_object_مقصد_stdin(ثابت بنية object_id *oid,
				       ثابت حرف *مسیر استفادهنشده,
				       فراغ *داده)
{
	بنية write_loose_object_داده *d = (بنية write_loose_object_داده *)داده;

	fprintf(d->درون, "%s\n", oid_مقصد_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	أرجع ++(d->شمار) > d->batch_اندازه;
}

ساكن صحيح pack_loose(بنية maintenance_پیاپی_opts *opts)
{
	بنية repository *r = the_repository;
	صحيح نتیجه = 0;
	بنية write_loose_object_داده داده;
	بنية فرزند_process pack_proc = فرزند_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	إذا (!for_each_loose_file_درون_منبع(r->objects->منبع,
					   bail_on_loose,
					   عدم, عدم, عدم))
		أرجع 0;

	pack_proc.git_cmd = 1;

	strvec_افزودن(&pack_proc.آرگومانها, "pack-objects");
	إذا (opts->quiet)
		strvec_افزودن(&pack_proc.آرگومانها, "--quiet");
	وإلا
		strvec_افزودن(&pack_proc.آرگومانها, "--no-quiet");
	strvec_pushf(&pack_proc.آرگومانها, "%s/pack/loose", r->objects->منبع->مسیر);

	pack_proc.درون = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.بیرون = -1;

	إذا (شروع_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		أرجع 1;
	}

	داده.درون = xfdopen(pack_proc.درون, "w");
	داده.شمار = 0;
	داده.batch_اندازه = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &داده.batch_اندازه);

	/* If configured as 0, then remove limit. */
	إذا (!داده.batch_اندازه)
		داده.batch_اندازه = بیشترین_صحیح;
	وإلا إذا (داده.batch_اندازه > 0)
		داده.batch_اندازه--; /* Decrease for equality on limit. */

	for_each_loose_file_درون_منبع(r->objects->منبع,
				      write_loose_object_مقصد_stdin,
				      عدم, عدم, &داده);

	fclose(داده.درون);

	إذا (پایان_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		نتیجه = 1;
	}

	أرجع نتیجه;
}

ساكن صحيح maintenance_task_loose_objects(بنية maintenance_پیاپی_opts *opts,
					  بنية gc_config *cfg استفادهنشده)
{
	أرجع prune_packed(opts) || pack_loose(opts);
}

ساكن صحيح incremental_repack_auto_condition(بنية gc_config *cfg استفادهنشده)
{
	بنية packed_git *p;
	صحيح incremental_repack_auto_حد = 10;
	صحيح شمار = 0;

	prepare_repo_settings(the_repository);
	إذا (!the_repository->settings.core_multi_pack_شاخص)
		أرجع 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_حد);

	إذا (!incremental_repack_auto_حد)
		أرجع 0;
	إذا (incremental_repack_auto_حد < 0)
		أرجع 1;

	repo_for_each_pack(the_repository, p) {
		إذا (شمار >= incremental_repack_auto_حد)
			اكسر;
		إذا (!p->multi_pack_شاخص)
			شمار++;
	}

	أرجع شمار >= incremental_repack_auto_حد;
}

ساكن صحيح multi_pack_شاخص_write(بنية maintenance_پیاپی_opts *opts)
{
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;

	فرزند.git_cmd = 1;
	strvec_pushl(&فرزند.آرگومانها, "multi-pack-index", "write", عدم);

	إذا (opts->quiet)
		strvec_افزودن(&فرزند.آرگومانها, "--no-progress");
	وإلا
		strvec_افزودن(&فرزند.آرگومانها, "--progress");

	إذا (پیاپی_command(&فرزند))
		أرجع error(_("failed to write multi-pack-index"));

	أرجع 0;
}

ساكن صحيح multi_pack_شاخص_expire(بنية maintenance_پیاپی_opts *opts)
{
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;

	فرزند.git_cmd = 1;
	فرزند.odb_مقصد_close = the_repository->objects;
	strvec_pushl(&فرزند.آرگومانها, "multi-pack-index", "expire", عدم);

	إذا (opts->quiet)
		strvec_افزودن(&فرزند.آرگومانها, "--no-progress");
	وإلا
		strvec_افزودن(&فرزند.آرگومانها, "--progress");

	إذا (پیاپی_command(&فرزند))
		أرجع error(_("'git multi-pack-index expire' failed"));

	أرجع 0;
}

#تعریف_کن TWO_GIGABYTES (INT32_MAX)

ساكن off_t get_auto_pack_اندازه(فراغ)
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
	off_t max_اندازه = 0;
	off_t دومی_بزرگترین_اندازه = 0;
	off_t نتیجه_اندازه;
	بنية packed_git *p;
	بنية repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		إذا (p->pack_اندازه > max_اندازه) {
			دومی_بزرگترین_اندازه = max_اندازه;
			max_اندازه = p->pack_اندازه;
		} وإلا إذا (p->pack_اندازه > دومی_بزرگترین_اندازه)
			دومی_بزرگترین_اندازه = p->pack_اندازه;
	}

	نتیجه_اندازه = دومی_بزرگترین_اندازه + 1;

	/* But limit ourselves to a batch size of 2g */
	إذا (نتیجه_اندازه > TWO_GIGABYTES)
		نتیجه_اندازه = TWO_GIGABYTES;

	أرجع نتیجه_اندازه;
}

ساكن صحيح multi_pack_شاخص_repack(بنية maintenance_پیاپی_opts *opts)
{
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;

	فرزند.git_cmd = 1;
	فرزند.odb_مقصد_close = the_repository->objects;
	strvec_pushl(&فرزند.آرگومانها, "multi-pack-index", "repack", عدم);

	إذا (opts->quiet)
		strvec_افزودن(&فرزند.آرگومانها, "--no-progress");
	وإلا
		strvec_افزودن(&فرزند.آرگومانها, "--progress");

	strvec_pushf(&فرزند.آرگومانها, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_اندازه());

	إذا (پیاپی_command(&فرزند))
		أرجع error(_("'git multi-pack-index repack' failed"));

	أرجع 0;
}

ساكن صحيح maintenance_task_incremental_repack(بنية maintenance_پیاپی_opts *opts,
					       بنية gc_config *cfg استفادهنشده)
{
	prepare_repo_settings(the_repository);
	إذا (!the_repository->settings.core_multi_pack_شاخص) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		أرجع 0;
	}

	إذا (multi_pack_شاخص_write(opts))
		أرجع 1;
	إذا (multi_pack_شاخص_expire(opts))
		أرجع 1;
	إذا (multi_pack_شاخص_repack(opts))
		أرجع 1;
	أرجع 0;
}

ساكن صحيح maintenance_task_geometric_repack(بنية maintenance_پیاپی_opts *opts,
					     بنية gc_config *cfg)
{
	بنية odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	إذا (!opts->quiet)
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

تعريفنوع صحيح (*maintenance_task_fn)(بنية maintenance_پیاپی_opts *opts,
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
	[TASK_COMMIT_گراف] = {
		.name = "commit-graph",
		.background = maintenance_task_commit_گراف,
		.auto_condition = should_write_commit_گراف,
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

تعداد auto_gc_hook_نتیجه {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_پیاپی = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

ساكن صحيح maybe_پیاپی_task(ثابت بنية maintenance_task *task,
			  بنية repository *repo,
			  بنية maintenance_پیاپی_opts *opts,
			  بنية gc_config *cfg,
			  تعداد task_phase phase,
			  تعداد auto_gc_hook_نتیجه *auto_gc_hook_نتیجه)
{
	صحيح foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ثابت حرف *region = foreground ? "maintenance foreground" : "maintenance";
	صحيح ret = 0;

	إذا (!fn)
		أرجع 0;
	إذا (opts->auto_flag) {
		إذا (*auto_gc_hook_نتیجه == AUTO_GC_HOOK_SKIP)
			أرجع 0;

		إذا (!task->auto_condition || !task->auto_condition(cfg))
			أرجع 0;

		إذا (*auto_gc_hook_نتیجه == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_نتیجه = پیاپی_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_پیاپی;
		إذا (*auto_gc_hook_نتیجه == AUTO_GC_HOOK_SKIP)
			أرجع 0;
	}

	trace2_region_enter(region, task->name, repo);
	إذا (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	أرجع ret;
}

ساكن صحيح maintenance_پیاپی_tasks(بنية maintenance_پیاپی_opts *opts,
				 بنية gc_config *cfg)
{
	صحيح نتیجه = 0;
	بنية قفل_file lk;
	بنية repository *r = the_repository;
	حرف *قفل_مسیر = xstrfmt("%s/maintenance", r->objects->منبع->مسیر);
	تعداد auto_gc_hook_نتیجه auto_gc_hook_نتیجه = AUTO_GC_HOOK_UNDECIDED;

	إذا (repo_hold_قفل_file_for_update(r, &lk, قفل_مسیر, قفل_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		إذا (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				قفل_مسیر);
		حرر(قفل_مسیر);
		أرجع 0;
	}
	حرر(قفل_مسیر);

	لكل (اندازه_t i = 0; i < opts->tasks_nr; i++)
		إذا (maybe_پیاپی_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_نتیجه))
			نتیجه = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	إذا (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	لكل (اندازه_t i = 0; i < opts->tasks_nr; i++)
		إذا (maybe_پیاپی_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_نتیجه))
			نتیجه = 1;

	rollback_قفل_file(&lk);
	أرجع نتیجه;
}

تعداد maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

بنية maintenance_strategy {
	بنية {
		بیعلامت type;
		تعداد زمانبندی_اولویت زمانبندی;
	} tasks[TASK_شمار];
};

ساكن ثابت بنية maintenance_strategy none_strategy = { 0 };

ساكن ثابت بنية maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.زمانبندی = زمانبندی_DAILY,
		},
	},
};

ساكن ثابت بنية maintenance_strategy incremental_strategy = {
	.tasks = {
		[TASK_COMMIT_گراف] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.زمانبندی = زمانبندی_HOURLY,
		},
		[TASK_PREFETCH] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.زمانبندی = زمانبندی_HOURLY,
		},
		[TASK_INCREMENTAL_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.زمانبندی = زمانبندی_DAILY,
		},
		[TASK_LOOSE_OBJECTS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.زمانبندی = زمانبندی_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.زمانبندی = زمانبندی_WEEKLY,
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
		[TASK_COMMIT_گراف] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.زمانبندی = زمانبندی_HOURLY,
		},
		[TASK_GEOMETRIC_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.زمانبندی = زمانبندی_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.زمانبندی = زمانبندی_DAILY,
		},
		[TASK_RERERE_GC] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.زمانبندی = زمانبندی_WEEKLY,
		},
		[TASK_REFLOG_EXPIRE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.زمانبندی = زمانبندی_WEEKLY,
		},
		[TASK_WORKTREE_PRUNE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.زمانبندی = زمانبندی_WEEKLY,
		},
	},
};

ساكن بنية maintenance_strategy parse_maintenance_strategy(ثابت حرف *name)
{
	إذا (!strcasecmp(name, "incremental"))
		أرجع incremental_strategy;
	إذا (!strcasecmp(name, "gc"))
		أرجع gc_strategy;
	إذا (!strcasecmp(name, "geometric"))
		أرجع geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

ساكن فراغ initialize_task_config(بنية maintenance_پیاپی_opts *opts,
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
	إذا (selected_tasks->nr) {
		لكل (اندازه_t i = 0; i < selected_tasks->nr; i++) {
			تعداد maintenance_task_label label = (intptr_t)selected_tasks->اقلام[i].util;;
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
	إذا (opts->زمانبندی) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} وإلا {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	إذا (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	لكل (اندازه_t i = 0; i < TASK_شمار; i++) {
		صحيح config_مقدار;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		إذا (!repo_config_get_bool(the_repository, config_name.buf, &config_مقدار))
			strategy.tasks[i].type = config_مقدار ? type : 0;
		إذا (!(strategy.tasks[i].type & type))
			استمر;

		إذا (opts->زمانبندی) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			إذا (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].زمانبندی = parse_زمانبندی(config_str);
			إذا (strategy.tasks[i].زمانبندی < opts->زمانبندی)
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
	بنية string_list *selected_tasks = opt->مقدار;
	اندازه_t i;

	BUG_ON_OPT_NEG(unset);

	لكل (i = 0; i < TASK_شمار; i++)
		إذا (!strcasecmp(tasks[i].name, arg))
			اكسر;
	إذا (i >= TASK_شمار) {
		error(_("'%s' is not a valid task"), arg);
		أرجع 1;
	}

	إذا (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		أرجع 1;
	}

	string_list_append(selected_tasks, arg)->util = (فراغ *)(intptr_t)i;

	أرجع 0;
}

ساكن صحيح maintenance_پیاپی(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			   بنية repository *repo استفادهنشده)
{
	بنية maintenance_پیاپی_opts opts = MAINTENANCE_پیاپی_OPTS_INIT;
	بنية string_list selected_tasks = STRING_LIST_INIT_DUP;
	بنية gc_config cfg = GC_CONFIG_INIT;
	بنية option builtin_maintenance_پیاپی_options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform maintenance in the background")),
		OPT_CALLBACK(0, "schedule", &opts.زمانبندی, N_("frequency"),
			     N_("run tasks based on frequency"),
			     maintenance_opt_زمانبندی),
		OPT_BOOL(0, "quiet", &opts.quiet,
			 N_("do not report progress or other information over stderr")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			N_("run a specific task"),
			PARSE_OPT_NONEG, task_option_parse),
		OPT_انتها()
	};
	صحيح ret;

	opts.quiet = !isatty(2);

	argc = parse_options(argc, argv, پیشوند,
			     builtin_maintenance_پیاپی_options,
			     builtin_maintenance_پیاپی_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	die_for_incompatible_opt2(opts.auto_flag, "--auto",
				  opts.زمانبندی, "--schedule=");
	die_for_incompatible_opt2(selected_tasks.nr, "--task=",
				  opts.زمانبندی, "--schedule=");

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	إذا (argc != 0)
		usage_with_options(builtin_maintenance_پیاپی_usage,
				   builtin_maintenance_پیاپی_options);

	ret = maintenance_پیاپی_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_پیاپی_opts_release(&opts);
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

ساكن حرف ثابت * ثابت builtin_maintenance_ثبات_usage[] = {
	"git maintenance register [--config-file <path>]",
	عدم
};

ساكن صحيح maintenance_ثبات(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
				بنية repository *repo استفادهنشده)
{
	حرف *config_file = عدم;
	بنية option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_انتها(),
	};
	صحيح یافت_2 = 0;
	ثابت حرف *کلید = "maintenance.repo";
	حرف *maintpath = get_maintpath();
	بنية string_list_قلم *قلم;
	ثابت بنية string_list *list;

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_maintenance_ثبات_usage, 0);
	إذا (argc)
		usage_with_options(builtin_maintenance_ثبات_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_مجموعه(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	إذا (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_مجموعه(the_repository, "maintenance.strategy", "incremental");

	إذا (!repo_config_get_string_multi(the_repository, کلید, &list)) {
		for_each_string_list_قلم(قلم, list) {
			إذا (!strcmp(maintpath, قلم->string)) {
				یافت_2 = 1;
				اكسر;
			}
		}
	}

	إذا (!یافت_2) {
		صحيح rc;
		حرف *global_config_file = عدم;

		إذا (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		إذا (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_مجموعه_multivar_درون_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, عدم, 0);
		حرر(global_config_file);

		إذا (rc)
			die(_("unable to add '%s' value of '%s'"),
			    کلید, maintpath);
	}

	حرر(maintpath);
	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	عدم
};

ساكن صحيح maintenance_unregister(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
				  بنية repository *repo استفادهنشده)
{
	صحيح force = 0;
	حرف *config_file = عدم;
	بنية option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_انتها(),
	};
	ثابت حرف *کلید = "maintenance.repo";
	حرف *maintpath = get_maintpath();
	صحيح یافت_2 = 0;
	بنية string_list_قلم *قلم;
	ثابت بنية string_list *list;
	بنية config_مجموعه cs = { { 0 } };

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_maintenance_unregister_usage, 0);
	إذا (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	إذا (config_file) {
		git_configset_init(&cs);
		git_configset_جمع_کن_file(&cs, config_file);
	}
	إذا (!(config_file
	      ? git_configset_get_string_multi(&cs, کلید, &list)
	      : repo_config_get_string_multi(the_repository, کلید, &list))) {
		for_each_string_list_قلم(قلم, list) {
			إذا (!strcmp(maintpath, قلم->string)) {
				یافت_2 = 1;
				اكسر;
			}
		}
	}

	إذا (یافت_2) {
		صحيح rc;
		حرف *global_config_file = عدم;

		إذا (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		إذا (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_مجموعه_multivar_درون_file_gently(the_repository,
			config_file, کلید, عدم, maintpath, عدم,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_مقدار);
		حرر(global_config_file);

		إذا (rc &&
		    (!force || rc == CONFIG_NOTHING_مجموعه))
			die(_("unable to unset '%s' value of '%s'"),
			    کلید, maintpath);
	} وإلا إذا (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	حرر(maintpath);
	أرجع 0;
}

ساكن ثابت حرف *get_frequency(تعداد زمانبندی_اولویت زمانبندی)
{
	بدل (زمانبندی) {
	حالة زمانبندی_HOURLY:
		أرجع "hourly";
	حالة زمانبندی_DAILY:
		أرجع "daily";
	حالة زمانبندی_WEEKLY:
		أرجع "weekly";
	افتراضي:
		BUG("invalid schedule %d", زمانبندی);
	}
}

ساكن ثابت حرف *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	عدم
};

ساكن ثابت حرف *get_extra_config_parameters(فراغ) {
	ساكن ثابت حرف *نتیجه = عدم;
	بنية strbuf builder = STRBUF_INIT;

	إذا (نتیجه)
		أرجع نتیجه;

	لكل (ثابت حرف **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	نتیجه = strbuf_detach(&builder, عدم);
	أرجع نتیجه;
}

ساكن ثابت حرف *get_extra_launchctl_رشتهها(فراغ) {
	ساكن ثابت حرف *نتیجه = عدم;
	بنية strbuf builder = STRBUF_INIT;

	إذا (نتیجه)
		أرجع نتیجه;

	لكل (ثابت حرف **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	نتیجه = strbuf_detach(&builder, عدم);
	أرجع نتیجه;
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
ساكن صحيح get_زمانبندی_cmd(ثابت حرف *cmd, صحيح *is_موجود, حرف **بیرون)
{
	حرف *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	بنية string_list_قلم *قلم;
	بنية string_list list = STRING_LIST_INIT_NODUP;

	إذا (!testing) {
		إذا (بیرون)
			*بیرون = xstrdup(cmd);
		أرجع 0;
	}

	إذا (is_موجود)
		*is_موجود = 0;

	string_list_تقسیم_درون_قراربده(&list, testing, ",", -1);
	for_each_string_list_قلم(قلم, &list) {
		بنية string_list pair = STRING_LIST_INIT_NODUP;

		إذا (string_list_تقسیم_درون_قراربده(&pair, قلم->string, ":", 2) != 2)
			استمر;

		إذا (!strcmp(cmd, pair.اقلام[0].string)) {
			إذا (بیرون)
				*بیرون = xstrdup(pair.اقلام[1].string);
			إذا (is_موجود)
				*is_موجود = 1;
			string_list_clear(&pair, 0);
			اذهب بیرون;
		}

		string_list_clear(&pair, 0);
	}

	إذا (بیرون)
		*بیرون = xstrdup(cmd);

بیرون:
	string_list_clear(&list, 0);
	حرر(testing);
	أرجع 1;
}

ساكن صحيح get_random_minute(فراغ)
{
	/* Use a static value when under tests. */
	إذا (getenv("GIT_TEST_MAINT_SCHEDULER"))
		أرجع 13;

	أرجع git_rand(0) % 60;
}

ساكن صحيح is_launchctl_موجود(فراغ)
{
	صحيح is_موجود;
	إذا (get_زمانبندی_cmd("launchctl", &is_موجود, عدم))
		أرجع is_موجود;

#اگر_تعریف_شده __APPLE__
	أرجع 1;
#وإلا
	أرجع 0;
#پایان_اگر
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

	expanded = interpolate_مسیر(filename.buf, 1);
	إذا (!expanded)
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
	صحيح نتیجه;
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;
	حرف *uid = launchctl_get_uid();

	get_زمانبندی_cmd("launchctl", عدم, &cmd);
	strvec_تقسیم(&فرزند.آرگومانها, cmd);
	strvec_pushl(&فرزند.آرگومانها, enable ? "bootstrap" : "bootout", uid,
		     filename, عدم);

	فرزند.no_stderr = 1;
	فرزند.no_stdout = 1;

	إذا (شروع_command(&فرزند))
		die(_("failed to start launchctl"));

	نتیجه = پایان_command(&فرزند);

	حرر(cmd);
	حرر(uid);
	أرجع نتیجه;
}

ساكن صحيح launchctl_remove_plist(تعداد زمانبندی_اولویت زمانبندی)
{
	ثابت حرف *frequency = get_frequency(زمانبندی);
	حرف *name = launchctl_service_name(frequency);
	حرف *filename = launchctl_service_filename(name);
	صحيح نتیجه = launchctl_boot_plist(0, filename);
	unlink(filename);
	حرر(filename);
	حرر(name);
	أرجع نتیجه;
}

ساكن صحيح launchctl_remove_plists(فراغ)
{
	أرجع launchctl_remove_plist(زمانبندی_HOURLY) ||
	       launchctl_remove_plist(زمانبندی_DAILY) ||
	       launchctl_remove_plist(زمانبندی_WEEKLY);
}

ساكن صحيح launchctl_list_شامل_است_plist(ثابت حرف *name, ثابت حرف *cmd)
{
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;

	strvec_تقسیم(&فرزند.آرگومانها, cmd);
	strvec_pushl(&فرزند.آرگومانها, "list", name, عدم);

	فرزند.no_stderr = 1;
	فرزند.no_stdout = 1;

	إذا (شروع_command(&فرزند))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	أرجع !پایان_command(&فرزند);
}

ساكن صحيح launchctl_زمانبندی_plist(ثابت حرف *exec_مسیر, تعداد زمانبندی_اولویت زمانبندی)
{
	صحيح i, fd;
	ثابت حرف *preamble, *repeat;
	ثابت حرف *frequency = get_frequency(زمانبندی);
	حرف *name = launchctl_service_name(frequency);
	حرف *filename = launchctl_service_filename(name);
	بنية قفل_file lk = قفل_INIT;
	ساكن بیعلامت دراز قفل_file_timeout_ms = ULONG_MAX;
	بنية strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	بنية stat st;
	حرف *cmd;
	صحيح minute = get_random_minute();

	get_زمانبندی_cmd("launchctl", عدم, &cmd);
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
	strbuf_addf(&plist, preamble, name, exec_مسیر, exec_مسیر,
		    get_extra_launchctl_رشتهها(), frequency);

	بدل (زمانبندی) {
	حالة زمانبندی_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		لكل (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		اكسر;

	حالة زمانبندی_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		لكل (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		اكسر;

	حالة زمانبندی_WEEKLY:
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

	إذا (امن_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	إذا ((دراز)قفل_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &قفل_file_timeout_ms))
		قفل_file_timeout_ms = 150;

	fd = hold_قفل_file_for_update_timeout(&lk, filename, قفل_DIE_ON_ERROR,
					       قفل_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	إذا (!stat(filename, &st) && st.st_اندازه == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_شامل_است_plist(name, cmd))
		rollback_قفل_file(&lk);
	وإلا {
		إذا (write_درون_پر(fd, plist.buf, plist.len) < 0 ||
		    commit_قفل_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		إذا (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	حرر(filename);
	حرر(name);
	حرر(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	أرجع 0;
}

ساكن صحيح launchctl_جمع_کن_plists(فراغ)
{
	ثابت حرف *exec_مسیر = git_exec_مسیر();

	أرجع launchctl_زمانبندی_plist(exec_مسیر, زمانبندی_HOURLY) ||
	       launchctl_زمانبندی_plist(exec_مسیر, زمانبندی_DAILY) ||
	       launchctl_زمانبندی_plist(exec_مسیر, زمانبندی_WEEKLY);
}

ساكن صحيح launchctl_update_زمانبندی(صحيح پیاپی_maintenance, صحيح fd استفادهنشده)
{
	إذا (پیاپی_maintenance)
		أرجع launchctl_جمع_کن_plists();
	وإلا
		أرجع launchctl_remove_plists();
}

ساكن صحيح is_schtasks_موجود(فراغ)
{
	صحيح is_موجود;
	إذا (get_زمانبندی_cmd("schtasks", &is_موجود, عدم))
		أرجع is_موجود;

#اگر_تعریف_شده GIT_WINDOWS_NATIVE
	أرجع 1;
#وإلا
	أرجع 0;
#پایان_اگر
}

ساكن حرف *schtasks_task_name(ثابت حرف *frequency)
{
	بنية strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	أرجع strbuf_detach(&label, عدم);
}

ساكن صحيح schtasks_remove_task(تعداد زمانبندی_اولویت زمانبندی)
{
	حرف *cmd;
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;
	ثابت حرف *frequency = get_frequency(زمانبندی);
	حرف *name = schtasks_task_name(frequency);

	get_زمانبندی_cmd("schtasks", عدم, &cmd);
	strvec_تقسیم(&فرزند.آرگومانها, cmd);
	strvec_pushl(&فرزند.آرگومانها, "/delete", "/tn", name, "/f", عدم);
	حرر(name);
	حرر(cmd);

	أرجع پیاپی_command(&فرزند);
}

ساكن صحيح schtasks_remove_tasks(فراغ)
{
	أرجع schtasks_remove_task(زمانبندی_HOURLY) ||
	       schtasks_remove_task(زمانبندی_DAILY) ||
	       schtasks_remove_task(زمانبندی_WEEKLY);
}

ساكن صحيح schtasks_زمانبندی_task(ثابت حرف *exec_مسیر, تعداد زمانبندی_اولویت زمانبندی)
{
	حرف *cmd;
	صحيح نتیجه;
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;
	ثابت حرف *xml;
	بنية tempfile *tfile;
	ثابت حرف *frequency = get_frequency(زمانبندی);
	حرف *name = schtasks_task_name(frequency);
	بنية strbuf tfilename = STRBUF_INIT;
	صحيح minute = get_random_minute();

	get_زمانبندی_cmd("schtasks", عدم, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	إذا (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	بدل (زمانبندی) {
	حالة زمانبندی_HOURLY:
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

	حالة زمانبندی_DAILY:
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

	حالة زمانبندی_WEEKLY:
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
	fprintf(tfile->fp, xml, exec_مسیر, exec_مسیر,
		get_extra_config_parameters(), frequency);
	strvec_تقسیم(&فرزند.آرگومانها, cmd);
	strvec_pushl(&فرزند.آرگومانها, "/create", "/tn", name, "/f", "/xml",
				  get_tempfile_مسیر(tfile), عدم);
	close_tempfile_gently(tfile);

	فرزند.no_stdout = 1;
	فرزند.no_stderr = 1;

	إذا (شروع_command(&فرزند))
		die(_("failed to start schtasks"));
	نتیجه = پایان_command(&فرزند);

	delete_tempfile(&tfile);
	حرر(name);
	حرر(cmd);
	أرجع نتیجه;
}

ساكن صحيح schtasks_زمانبندی_tasks(فراغ)
{
	ثابت حرف *exec_مسیر = git_exec_مسیر();

	أرجع schtasks_زمانبندی_task(exec_مسیر, زمانبندی_HOURLY) ||
	       schtasks_زمانبندی_task(exec_مسیر, زمانبندی_DAILY) ||
	       schtasks_زمانبندی_task(exec_مسیر, زمانبندی_WEEKLY);
}

ساكن صحيح schtasks_update_زمانبندی(صحيح پیاپی_maintenance, صحيح fd استفادهنشده)
{
	إذا (پیاپی_maintenance)
		أرجع schtasks_زمانبندی_tasks();
	وإلا
		أرجع schtasks_remove_tasks();
}

MAYBE_استفادهنشده
ساكن صحيح check_crontab_process(ثابت حرف *cmd)
{
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;

	strvec_تقسیم(&فرزند.آرگومانها, cmd);
	strvec_افزودن(&فرزند.آرگومانها, "-l");
	فرزند.no_stdin = 1;
	فرزند.no_stdout = 1;
	فرزند.no_stderr = 1;
	فرزند.silent_exec_failure = 1;

	إذا (شروع_command(&فرزند))
		أرجع 0;
	/* Ignore exit code, as an empty crontab will return error. */
	پایان_command(&فرزند);
	أرجع 1;
}

ساكن صحيح is_crontab_موجود(فراغ)
{
	حرف *cmd;
	صحيح is_موجود;
	صحيح ret;

	إذا (get_زمانبندی_cmd("crontab", &is_موجود, &cmd)) {
		ret = is_موجود;
		اذهب بیرون;
	}

#اگر_تعریف_شده __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#وإلا
	ret = check_crontab_process(cmd);
#پایان_اگر

بیرون:
	حرر(cmd);
	أرجع ret;
}

#تعریف_کن BEGIN_سطر "# BEGIN GIT MAINTENANCE SCHEDULE"
#تعریف_کن انتها_سطر "# END GIT MAINTENANCE SCHEDULE"

ساكن صحيح crontab_update_زمانبندی(صحيح پیاپی_maintenance, صحيح fd)
{
	حرف *cmd;
	صحيح نتیجه = 0;
	صحيح درون_old_region = 0;
	بنية فرزند_process crontab_list = فرزند_PROCESS_INIT;
	بنية فرزند_process crontab_edit = فرزند_PROCESS_INIT;
	FILE *cron_list, *cron_درون;
	بنية strbuf سطر = STRBUF_INIT;
	بنية tempfile *tmpedit = عدم;
	صحيح minute = get_random_minute();

	get_زمانبندی_cmd("crontab", عدم, &cmd);
	strvec_تقسیم(&crontab_list.آرگومانها, cmd);
	strvec_افزودن(&crontab_list.آرگومانها, "-l");
	crontab_list.درون = -1;
	crontab_list.بیرون = dup(fd);
	crontab_list.git_cmd = 0;

	إذا (شروع_command(&crontab_list)) {
		نتیجه = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		اذهب بیرون;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	پایان_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	إذا (!tmpedit) {
		نتیجه = error(_("failed to create crontab temporary file"));
		اذهب بیرون;
	}
	cron_درون = fdopen_tempfile(tmpedit, "w");
	إذا (!cron_درون) {
		نتیجه = error(_("failed to open temporary file"));
		اذهب بیرون;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	طالما (!strbuf_getline_lf(&سطر, cron_list)) {
		إذا (!درون_old_region && !strcmp(سطر.buf, BEGIN_سطر))
			درون_old_region = 1;
		وإلا إذا (درون_old_region && !strcmp(سطر.buf, انتها_سطر))
			درون_old_region = 0;
		وإلا إذا (!درون_old_region)
			fprintf(cron_درون, "%s\n", سطر.buf);
	}
	strbuf_release(&سطر);

	إذا (پیاپی_maintenance) {
		بنية strbuf سطر_format = STRBUF_INIT;
		ثابت حرف *exec_مسیر = git_exec_مسیر();

		fprintf(cron_درون, "%s\n", BEGIN_سطر);
		fprintf(cron_درون,
			"# The following schedule was created by Git\n");
		fprintf(cron_درون, "# Any edits made in this region might be\n");
		fprintf(cron_درون,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&سطر_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_مسیر, exec_مسیر, get_extra_config_parameters());
		fprintf(cron_درون, سطر_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_درون, سطر_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_درون, سطر_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&سطر_format);

		fprintf(cron_درون, "\n%s\n", انتها_سطر);
	}

	fflush(cron_درون);

	strvec_تقسیم(&crontab_edit.آرگومانها, cmd);
	strvec_افزودن(&crontab_edit.آرگومانها, get_tempfile_مسیر(tmpedit));
	crontab_edit.git_cmd = 0;

	إذا (شروع_command(&crontab_edit)) {
		نتیجه = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		اذهب بیرون;
	}

	إذا (پایان_command(&crontab_edit))
		نتیجه = error(_("'crontab' died"));
	وإلا
		fclose(cron_list);

بیرون:
	delete_tempfile(&tmpedit);
	حرر(cmd);
	أرجع نتیجه;
}

ساكن صحيح real_is_systemd_timer_موجود(فراغ)
{
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;

	strvec_pushl(&فرزند.آرگومانها, "systemctl", "--user", "list-timers", عدم);
	فرزند.no_stdin = 1;
	فرزند.no_stdout = 1;
	فرزند.no_stderr = 1;
	فرزند.silent_exec_failure = 1;

	إذا (شروع_command(&فرزند))
		أرجع 0;
	إذا (پایان_command(&فرزند))
		أرجع 0;
	أرجع 1;
}

ساكن صحيح is_systemd_timer_موجود(فراغ)
{
	صحيح is_موجود;

	إذا (get_زمانبندی_cmd("systemctl", &is_موجود, عدم))
		أرجع is_موجود;

	أرجع real_is_systemd_timer_موجود();
}

ساكن حرف *xdg_config_home_systemd(ثابت حرف *filename)
{
	أرجع xdg_config_home_for("systemd/user", filename);
}

#تعریف_کن SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

ساكن صحيح systemd_timer_delete_timer_file(تعداد زمانبندی_اولویت اولویت)
{
	صحيح ret = 0;
	ثابت حرف *frequency = get_frequency(اولویت);
	حرف *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	حرف *filename = xdg_config_home_systemd(local_timer_name);

	إذا (unlink(filename) && !is_missing_file_error(errno))
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
	إذا (unlink(filename) && !is_missing_file_error(errno))
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
ساكن صحيح systemd_timer_write_timer_file(تعداد زمانبندی_اولویت زمانبندی,
					  صحيح minute)
{
	صحيح res = -1;
	حرف *filename;
	FILE *file;
	ثابت حرف *unit;
	حرف *زمانبندی_الگو = عدم;
	ثابت حرف *frequency = get_frequency(زمانبندی);
	حرف *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	إذا (امن_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		اذهب error;
	}
	file = fopen_or_warn(filename, "w");
	إذا (!file)
		اذهب error;

	بدل (زمانبندی) {
	حالة زمانبندی_HOURLY:
		زمانبندی_الگو = xstrfmt("*-*-* 1..23:%02d:00", minute);
		اكسر;

	حالة زمانبندی_DAILY:
		زمانبندی_الگو = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		اكسر;

	حالة زمانبندی_WEEKLY:
		زمانبندی_الگو = xstrfmt("Mon 0:%02d:00", minute);
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
	إذا (fprintf(file, unit, زمانبندی_الگو) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		اذهب error;
	}
	إذا (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		اذهب error;
	}

	res = 0;

error:
	حرر(زمانبندی_الگو);
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
ساكن صحيح systemd_timer_write_service_template(ثابت حرف *exec_مسیر)
{
	صحيح res = -1;
	حرف *filename;
	FILE *file;
	ثابت حرف *unit;
	حرف *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	إذا (امن_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		اذهب error;
	}
	file = fopen_or_warn(filename, "w");
	إذا (!file)
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
	إذا (fprintf(file, unit, exec_مسیر, exec_مسیر, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		اذهب error;
	}
	إذا (fclose(file) == EOF) {
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
				     تعداد زمانبندی_اولویت زمانبندی,
				     صحيح minute)
{
	حرف *cmd = عدم;
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;
	ثابت حرف *frequency = get_frequency(زمانبندی);
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
	إذا (!enable) {
		فرزند.no_stderr = 1;
	} وإلا إذا (systemd_timer_write_timer_file(زمانبندی, minute)) {
		ret = -1;
		اذهب بیرون;
	}

	get_زمانبندی_cmd("systemctl", عدم, &cmd);
	strvec_تقسیم(&فرزند.آرگومانها, cmd);
	strvec_pushl(&فرزند.آرگومانها, "--user", enable ? "enable" : "disable",
		     "--now", عدم);
	strvec_pushf(&فرزند.آرگومانها, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	إذا (شروع_command(&فرزند)) {
		ret = error(_("failed to start systemctl"));
		اذهب بیرون;
	}

	إذا (پایان_command(&فرزند)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		إذا (enable) {
			ret = error(_("failed to run systemctl"));
			اذهب بیرون;
		}
	}

	ret = 0;

بیرون:
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

	إذا (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	حرر(filename);
	حرر(timer_template_name);
}

ساكن صحيح systemd_timer_delete_unit_files(فراغ)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	أرجع systemd_timer_delete_timer_file(زمانبندی_HOURLY) |
	       systemd_timer_delete_timer_file(زمانبندی_DAILY) |
	       systemd_timer_delete_timer_file(زمانبندی_WEEKLY) |
	       systemd_timer_delete_service_template();
}

ساكن صحيح systemd_timer_delete_units(فراغ)
{
	صحيح minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	أرجع systemd_timer_enable_unit(0, زمانبندی_HOURLY, minute) |
	       systemd_timer_enable_unit(0, زمانبندی_DAILY, minute) |
	       systemd_timer_enable_unit(0, زمانبندی_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

ساكن صحيح systemd_timer_راهاندازی_units(فراغ)
{
	صحيح minute = get_random_minute();
	ثابت حرف *exec_مسیر = git_exec_مسیر();

	صحيح ret = systemd_timer_write_service_template(exec_مسیر) ||
		  systemd_timer_enable_unit(1, زمانبندی_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, زمانبندی_DAILY, minute) ||
		  systemd_timer_enable_unit(1, زمانبندی_WEEKLY, minute);

	إذا (ret)
		systemd_timer_delete_units();
	وإلا
		systemd_timer_delete_stale_timer_templates();

	أرجع ret;
}

ساكن صحيح systemd_timer_update_زمانبندی(صحيح پیاپی_maintenance, صحيح fd استفادهنشده)
{
	إذا (پیاپی_maintenance)
		أرجع systemd_timer_راهاندازی_units();
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
	صحيح (*is_موجود)(فراغ);
	صحيح (*update_زمانبندی)(صحيح پیاپی_maintenance, صحيح fd);
} scheduler_fn[] = {
	[SCHEDULER_CRON] = {
		.name = "crontab",
		.is_موجود = is_crontab_موجود,
		.update_زمانبندی = crontab_update_زمانبندی,
	},
	[SCHEDULER_SYSTEMD] = {
		.name = "systemctl",
		.is_موجود = is_systemd_timer_موجود,
		.update_زمانبندی = systemd_timer_update_زمانبندی,
	},
	[SCHEDULER_LAUNCHCTL] = {
		.name = "launchctl",
		.is_موجود = is_launchctl_موجود,
		.update_زمانبندی = launchctl_update_زمانبندی,
	},
	[SCHEDULER_SCHTASKS] = {
		.name = "schtasks",
		.is_موجود = is_schtasks_موجود,
		.update_زمانبندی = schtasks_update_زمانبندی,
	},
};

ساكن تعداد scheduler parse_scheduler(ثابت حرف *مقدار)
{
	إذا (!مقدار)
		أرجع SCHEDULER_INVALID;
	وإلا إذا (!strcasecmp(مقدار, "auto"))
		أرجع SCHEDULER_AUTO;
	وإلا إذا (!strcasecmp(مقدار, "cron") || !strcasecmp(مقدار, "crontab"))
		أرجع SCHEDULER_CRON;
	وإلا إذا (!strcasecmp(مقدار, "systemd") ||
		 !strcasecmp(مقدار, "systemd-timer"))
		أرجع SCHEDULER_SYSTEMD;
	وإلا إذا (!strcasecmp(مقدار, "launchctl"))
		أرجع SCHEDULER_LAUNCHCTL;
	وإلا إذا (!strcasecmp(مقدار, "schtasks"))
		أرجع SCHEDULER_SCHTASKS;
	وإلا
		أرجع SCHEDULER_INVALID;
}

ساكن صحيح maintenance_opt_scheduler(ثابت بنية option *opt, ثابت حرف *arg,
				     صحيح unset)
{
	تعداد scheduler *scheduler = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	إذا (*scheduler == SCHEDULER_INVALID)
		أرجع error(_("unrecognized --scheduler argument '%s'"), arg);
	أرجع 0;
}

بنية maintenance_شروع_opts {
	تعداد scheduler scheduler;
};

ساكن تعداد scheduler resolve_scheduler(تعداد scheduler scheduler)
{
	إذا (scheduler != SCHEDULER_AUTO)
		أرجع scheduler;

#إذا defined(__APPLE__)
	أرجع SCHEDULER_LAUNCHCTL;

#وإلاإذا defined(GIT_WINDOWS_NATIVE)
	أرجع SCHEDULER_SCHTASKS;

#وإلاإذا defined(__linux__)
	إذا (is_systemd_timer_موجود())
		أرجع SCHEDULER_SYSTEMD;
	وإلا إذا (is_crontab_موجود())
		أرجع SCHEDULER_CRON;
	وإلا
		die(_("neither systemd timers nor crontab are available"));

#وإلا
	أرجع SCHEDULER_CRON;
#پایان_اگر
}

ساكن فراغ validate_scheduler(تعداد scheduler scheduler)
{
	إذا (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	إذا (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	إذا (!scheduler_fn[scheduler].is_موجود())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

ساكن صحيح update_background_زمانبندی(ثابت بنية maintenance_شروع_opts *opts,
				      صحيح enable)
{
	بیعلامت صحيح i;
	صحيح نتیجه = 0;
	بنية قفل_file lk;
	حرف *قفل_مسیر = xstrfmt("%s/schedule", the_repository->objects->منبع->مسیر);

	إذا (hold_قفل_file_for_update(&lk, قفل_مسیر, قفل_NO_DEREF) < 0) {
		إذا (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_مسیر(قفل_مسیر), strerror(errno));
		وإلا
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		حرر(قفل_مسیر);
		أرجع -1;
	}

	لكل (i = 1; i < ARRAY_اندازه(scheduler_fn); i++) {
		إذا (enable && opts->scheduler == i)
			استمر;
		إذا (!scheduler_fn[i].is_موجود())
			استمر;
		scheduler_fn[i].update_زمانبندی(0, get_قفل_file_fd(&lk));
	}

	إذا (enable)
		نتیجه = scheduler_fn[opts->scheduler].update_زمانبندی(
			1, get_قفل_file_fd(&lk));

	rollback_قفل_file(&lk);

	حرر(قفل_مسیر);
	أرجع نتیجه;
}

ساكن ثابت حرف *ثابت builtin_maintenance_شروع_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	عدم
};

ساكن صحيح maintenance_شروع(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			     بنية repository *repo)
{
	بنية maintenance_شروع_opts opts = { 0 };
	بنية option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_انتها()
	};
	ثابت حرف *ثبات_آرگومانها[] = { "register", عدم };

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_maintenance_شروع_usage, 0);
	إذا (argc)
		usage_with_options(builtin_maintenance_شروع_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	إذا (update_background_زمانبندی(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	إذا (maintenance_ثبات(ARRAY_اندازه(ثبات_آرگومانها)-1, ثبات_آرگومانها, عدم, repo))
		warning(_("failed to add repo to global config"));
	أرجع 0;
}

ساكن ثابت حرف *ثابت builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	عدم
};

ساكن صحيح maintenance_stop(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			    بنية repository *repo استفادهنشده)
{
	بنية option options[] = {
		OPT_انتها()
	};
	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_maintenance_stop_usage, 0);
	إذا (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	أرجع update_background_زمانبندی(عدم, 0);
}

ساكن ثابت حرف *ثابت builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	عدم
};

ساكن صحيح maintenance_is_needed(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
				 بنية repository *repo استفادهنشده)
{
	بنية maintenance_پیاپی_opts opts = MAINTENANCE_پیاپی_OPTS_INIT;
	بنية string_list selected_tasks = STRING_LIST_INIT_DUP;
	بنية gc_config cfg = GC_CONFIG_INIT;
	بنية option options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			       N_("check a specific task"),
			       PARSE_OPT_NONEG, task_option_parse),
		OPT_انتها()
	};
	bool is_needed = false;

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_maintenance_is_needed_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	إذا (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	إذا (opts.auto_flag) {
		لكل (اندازه_t i = 0; i < opts.tasks_nr; i++) {
			إذا (tasks[opts.tasks[i]].auto_condition &&
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
	maintenance_پیاپی_opts_release(&opts);
	gc_config_release(&cfg);

	إذا (is_needed)
		أرجع 0;
	أرجع 1;
}

ساكن ثابت حرف *ثابت builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	عدم,
};

صحيح cmd_maintenance(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *پیشوند,
		    بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_maintenance_options[] = {
		OPT_SUBCOMMAND("run", &fn, maintenance_پیاپی),
		OPT_SUBCOMMAND("start", &fn, maintenance_شروع),
		OPT_SUBCOMMAND("stop", &fn, maintenance_stop),
		OPT_SUBCOMMAND("register", &fn, maintenance_ثبات),
		OPT_SUBCOMMAND("unregister", &fn, maintenance_unregister),
		OPT_SUBCOMMAND("is-needed", &fn, maintenance_is_needed),
		OPT_انتها(),
	};

	argc = parse_options(argc, argv, پیشوند, builtin_maintenance_options,
			     builtin_maintenance_usage, 0);
	أرجع fn(argc, argv, پیشوند, repo);
}
