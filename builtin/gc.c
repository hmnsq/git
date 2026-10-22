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

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "date.h"
#شامل "dir.h"
#شامل "environment.h"
#شامل "hex.h"
#شامل "config.h"
#شامل "tempfile.h"
#شامل "lockfile.h"
#شامل "parse-options.h"
#شامل "run-command.h"
#شامل "sigchain.h"
#شامل "strvec.h"
#شامل "commit.h"
#شامل "commit-graph.h"
#شامل "packfile.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "path.h"
#شامل "reflog.h"
#شامل "rerere.h"
#شامل "revision.h"
#شامل "refs.h"
#شامل "remote.h"
#شامل "exec-cmd.h"
#شامل "gettext.h"
#شامل "hook.h"
#شامل "setup.h"
#شامل "trace2.h"
#شامل "worktree.h"

#تعریف_کرو FAILED_تسلسل "failed to run %s"

ساکن ثابت حرف * ثابت builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	تہی
};

ساکن timestamp_t gc_log_expire_وقت;
ساکن ساخت tempfile *pidfile;
ساکن ساخت تالا_file log_تالا;
ساکن ساخت string_list pack_garbage = STRING_LIST_INIT_DUP;

ساکن خالی clean_pack_garbage(خالی)
{
	صحیح i;
	برائے (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.اشیاء[i].string);
	string_list_clear(&pack_garbage, 0);
}

ساکن خالی report_pack_garbage(غیرعلامتی نظر_آیا_بٹ, ثابت حرف *راستہ)
{
	اگر (نظر_آیا_بٹ == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, راستہ);
}

ساکن خالی process_log_file(خالی)
{
	ساخت stat st;
	اگر (fstat(get_تالا_file_fd(&log_تالا), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		صحیح saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_تالا_file_راستہ(&log_تالا),
			strerror(saved_errno));
		fflush(stderr);
		commit_تالا_file(&log_تالا);
		errno = saved_errno;
	} ورنہ اگر (st.st_سائز) {
		/* There was some error recorded in the lock file */
		commit_تالا_file(&log_تالا);
	} ورنہ {
		حرف *راستہ = repo_git_راستہ(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(راستہ);
		rollback_تالا_file(&log_تالا);
		آزاد(راستہ);
	}
}

ساکن خالی process_log_file_at_exit(خالی)
{
	fflush(stderr);
	process_log_file();
}

ساکن صحیح gc_config_is_timestamp_never(ثابت حرف *var)
{
	ثابت حرف *قدر;
	timestamp_t expire;

	اگر (!repo_config_get_قدر(the_repository, var, &قدر) && قدر) {
		اگر (parse_expiry_date(قدر, &expire))
			die(_("failed to parse '%s' value '%s'"), var, قدر);
		واپس expire == 0;
	}
	واپس 0;
}

ساخت gc_config {
	صحیح pack_refs;
	صحیح prune_reflogs;
	صحیح cruft_packs;
	غیرعلامتی طویل max_cruft_سائز;
	صحیح aggressive_گہرائی;
	صحیح aggressive_کھڑکی;
	صحیح detach_auto;
	حرف *gc_log_expire;
	حرف *prune_expire;
	حرف *prune_worktrees_expire;
	حرف *repack_expire_مقصد;
};

#تعریف_کرو GC_CONFIG_INIT { \
	.pack_refs = 1, \
	.prune_reflogs = 1, \
	.cruft_packs = 1, \
	.aggressive_گہرائی = 50, \
	.aggressive_کھڑکی = 250, \
	.detach_auto = 1, \
	.gc_log_expire = xstrdup("1.day.ago"), \
	.prune_expire = xstrdup("2.weeks.ago"), \
	.prune_worktrees_expire = xstrdup("3.months.ago"), \
}

ساکن خالی gc_config_release(ساخت gc_config *cfg)
{
	آزاد(cfg->gc_log_expire);
	آزاد(cfg->prune_expire);
	آزاد(cfg->prune_worktrees_expire);
}

ساکن خالی gc_config(ساخت gc_config *cfg)
{
	ثابت حرف *قدر;
	حرف *owned = تہی;

	اگر (!repo_config_get_قدر(the_repository, "gc.packrefs", &قدر)) {
		اگر (قدر && !strcmp(قدر, "notbare"))
			cfg->pack_refs = -1;
		ورنہ
			cfg->pack_refs = git_config_bool("gc.packrefs", قدر);
	}

	اگر (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_کھڑکی);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_گہرائی);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_سائز);

	اگر (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		آزاد(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	اگر (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		آزاد(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	اگر (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		آزاد(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, تہی);
}

شمار جدول_وقت_ترجیح {
	جدول_وقت_NONE = 0,
	جدول_وقت_WEEKLY = 1,
	جدول_وقت_DAILY = 2,
	جدول_وقت_HOURLY = 3,
};

ساکن شمار جدول_وقت_ترجیح parse_جدول_وقت(ثابت حرف *قدر)
{
	اگر (!قدر)
		واپس جدول_وقت_NONE;
	اگر (!strcasecmp(قدر, "hourly"))
		واپس جدول_وقت_HOURLY;
	اگر (!strcasecmp(قدر, "daily"))
		واپس جدول_وقت_DAILY;
	اگر (!strcasecmp(قدر, "weekly"))
		واپس جدول_وقت_WEEKLY;
	واپس جدول_وقت_NONE;
}

شمار maintenance_task_label {
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
	TASK_گنتی
};

ساخت maintenance_تسلسل_opts {
	شمار maintenance_task_label *tasks;
	سائز_t tasks_nr, tasks_alloc;
	صحیح auto_flag;
	صحیح detach;
	صحیح quiet;
	شمار جدول_وقت_ترجیح جدول_وقت;
};
#تعریف_کرو MAINTENANCE_تسلسل_OPTS_INIT { \
	.detach = -1, \
}

ساکن خالی maintenance_تسلسل_opts_release(ساخت maintenance_تسلسل_opts *opts)
{
	آزاد(opts->tasks);
}

ساکن صحیح pack_refs_condition(غیر_مستعمل ساخت gc_config *cfg)
{
	ساخت string_list included_refs = STRING_LIST_INIT_NODUP;
	ساخت ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	ساخت refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	اگر (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		واپس 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	واپس required;
}

ساکن صحیح maintenance_task_pack_refs(ساخت maintenance_تسلسل_opts *opts,
				      غیر_مستعمل ساخت gc_config *cfg)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.دلائل, "pack-refs", "--all", "--prune", تہی);
	اگر (opts->auto_flag)
		strvec_ڈالو(&cmd.دلائل, "--auto");

	واپس تسلسل_command(&cmd);
}

ساخت گنتی_reflog_entries_ڈیٹا {
	ساخت expire_reflog_policy_cb policy;
	سائز_t گنتی;
	سائز_t حد_بندی;
};

ساکن صحیح گنتی_reflog_entries(ثابت حرف *refname غیر_مستعمل,
				ساخت object_id *old_oid, ساخت object_id *new_oid,
				ثابت حرف *committer, timestamp_t timestamp,
				صحیح tz, ثابت حرف *پیغام, خالی *cb_ڈیٹا)
{
	ساخت گنتی_reflog_entries_ڈیٹا *ڈیٹا = cb_ڈیٹا;
	اگر (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, پیغام, &ڈیٹا->policy))
		ڈیٹا->گنتی++;
	واپس ڈیٹا->گنتی >= ڈیٹا->حد_بندی;
}

ساکن صحیح reflog_expire_condition(ساخت gc_config *cfg غیر_مستعمل)
{
	timestamp_t now = وقت(تہی);
	ساخت گنتی_reflog_entries_ڈیٹا ڈیٹا = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	صحیح حد_بندی = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &حد_بندی);
	اگر (!حد_بندی)
		واپس 0;
	اگر (حد_بندی < 0)
		واپس 1;
	ڈیٹا.حد_بندی = حد_بندی;

	repo_config(the_repository, reflog_expire_config, &ڈیٹا.policy.opts);

	reflog_expire_options_سیٹ_refname(&ڈیٹا.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 گنتی_reflog_entries, &ڈیٹا);

	reflog_expiry_cleanup(&ڈیٹا.policy);
	reflog_clear_expire_config(&ڈیٹا.policy.opts);
	واپس ڈیٹا.گنتی >= ڈیٹا.حد_بندی;
}

ساکن صحیح maintenance_task_reflog_expire(ساخت maintenance_تسلسل_opts *opts غیر_مستعمل,
					  ساخت gc_config *cfg غیر_مستعمل)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.دلائل, "reflog", "expire", "--all", تہی);
	واپس تسلسل_command(&cmd);
}

ساکن صحیح maintenance_task_worktree_prune(ساخت maintenance_تسلسل_opts *opts غیر_مستعمل,
					   ساخت gc_config *cfg)
{
	ساخت بچہ_process prune_worktrees_cmd = بچہ_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.دلائل, "worktree", "prune", "--expire", تہی);
	strvec_ڈالو(&prune_worktrees_cmd.دلائل, cfg->prune_worktrees_expire);

	واپس تسلسل_command(&prune_worktrees_cmd);
}

ساکن صحیح worktree_prune_condition(ساخت gc_config *cfg)
{
	ساخت strbuf buf = STRBUF_INIT;
	صحیح should_prune = 0, حد_بندی = 1;
	timestamp_t expiry_date;
	ساخت dirent *d;
	DIR *dir = تہی;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &حد_بندی);
	اگر (حد_بندی <= 0) {
		should_prune = حد_بندی < 0;
		جاؤ برآمد;
	}

	اگر (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		جاؤ برآمد;

	dir = opendir(repo_git_راستہ_replace(the_repository, &buf, "worktrees"));
	اگر (!dir)
		جاؤ برآمد;

	جب (حد_بندی && (d = readdir_skip_dot_and_dotdot(dir))) {
		حرف *wtpath;
		strbuf_reset(&buf);
		اگر (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			حد_بندی--;
		آزاد(wtpath);
	}

	should_prune = !حد_بندی;

برآمد:
	اگر (dir)
		closedir(dir);
	strbuf_release(&buf);
	واپس should_prune;
}

ساکن صحیح maintenance_task_rerere_gc(ساخت maintenance_تسلسل_opts *opts غیر_مستعمل,
				      ساخت gc_config *cfg غیر_مستعمل)
{
	ساخت بچہ_process rerere_cmd = بچہ_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.دلائل, "rerere", "gc", تہی);
	واپس تسلسل_command(&rerere_cmd);
}

ساکن صحیح rerere_gc_condition(ساخت gc_config *cfg غیر_مستعمل)
{
	صحیح حد_بندی = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &حد_بندی);
	اگر (!حد_بندی)
		واپس 0; /* never prune */
	اگر (حد_بندی < 0)
		واپس 1; /* always prune */

	واپس rerere_gc_needed(the_repository, (سائز_t)حد_بندی);
}

#تعریف_کرو OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_مقصد = (cfg)->repack_expire_مقصد, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_سائز = (cfg)->max_cruft_سائز, \
	.کھڑکی = (aggressive) ? (cfg)->aggressive_کھڑکی : 0, \
	.گہرائی = (aggressive) ? (cfg)->aggressive_گہرائی : 0

/* return NULL on success, else hostname running the gc */
ساکن ثابت حرف *تالا_repo_for_gc(صحیح force, pid_t* ret_pid)
{
	ساخت تالا_file تالا = تالا_INIT;
	حرف my_host[HOST_NAME_MAX + 1];
	ساخت strbuf sb = STRBUF_INIT;
	ساخت stat st;
	uintmax_t pid;
	FILE *fp;
	صحیح fd;
	حرف *pidfile_راستہ;

	اگر (is_tempfile_فعال(pidfile))
		/* already locked */
		واپس تہی;

	اگر (xgethostname(my_host, حجم(my_host)))
		xsnprintf(my_host, حجم(my_host), "unknown");

	pidfile_راستہ = repo_git_راستہ(the_repository, "gc.pid");
	fd = hold_تالا_file_for_update(&تالا, pidfile_راستہ,
				       تالا_DIE_ON_ERROR);
	اگر (!force) {
		ساکن حرف locking_host[HOST_NAME_MAX + 1];
		ساکن حرف *جائزہ_fmt;
		صحیح should_exit;

		اگر (!جائزہ_fmt)
			جائزہ_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_راستہ, "r");
		memset(locking_host, 0, حجم(locking_host));
		should_exit =
			fp != تہی &&
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
			وقت(تہی) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, جائزہ_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		اگر (fp)
			fclose(fp);
		اگر (should_exit) {
			اگر (fd >= 0)
				rollback_تالا_file(&تالا);
			*ret_pid = pid;
			آزاد(pidfile_راستہ);
			واپس locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_اندر_بھرا(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_تالا_file(&تالا);
	pidfile = رجسٹر_tempfile(pidfile_راستہ);
	آزاد(pidfile_راستہ);
	واپس تہی;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
ساکن صحیح report_آخری_gc_error(خالی)
{
	ساخت strbuf sb = STRBUF_INIT;
	صحیح ret = 0;
	ssize_t len;
	ساخت stat st;
	حرف *gc_log_راستہ = repo_git_راستہ(the_repository, "gc.log");

	اگر (stat(gc_log_راستہ, &st)) {
		اگر (errno == ENOENT)
			جاؤ مکمل;

		ret = die_پیغام_errno(_("cannot stat '%s'"), gc_log_راستہ);
		جاؤ مکمل;
	}

	اگر (st.st_mtime < gc_log_expire_وقت)
		جاؤ مکمل;

	len = strbuf_read_file(&sb, gc_log_راستہ, 0);
	اگر (len < 0)
		ret = die_پیغام_errno(_("cannot read '%s'"), gc_log_راستہ);
	ورنہ اگر (len > 0) {
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
			    gc_log_راستہ, sb.buf);
		ret = 1;
	}
	strbuf_release(&sb);
مکمل:
	آزاد(gc_log_راستہ);
	واپس ret;
}

ساکن صحیح gc_foreground_tasks(ساخت maintenance_تسلسل_opts *opts,
			       ساخت gc_config *cfg)
{
	اگر (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		واپس error(FAILED_تسلسل, "pack-refs");
	اگر (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		واپس error(FAILED_تسلسل, "reflog");
	واپس 0;
}

ساکن صحیح maintenance_task_odb(ساخت maintenance_تسلسل_opts *opts,
				ساخت gc_config *cfg,
				صحیح keep_سب_سے_بڑا_pack,
				صحیح aggressive)
{
	ساخت odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_سب_سے_بڑا_pack = keep_سب_سے_بڑا_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	اگر (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	اگر (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	اگر (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	واپس odb_optimize(the_repository->objects, &odb_opts);
}

صحیح cmd_gc(صحیح argc,
	   ثابت حرف **argv,
	   ثابت حرف *سابقہ,
	   ساخت repository *repo غیر_مستعمل)
{
	صحیح aggressive = 0;
	صحیح force = 0;
	ثابت حرف *name;
	pid_t pid;
	صحیح daemonized = 0;
	صحیح keep_سب_سے_بڑا_pack = -1;
	صحیح skip_foreground_tasks = 0;
	timestamp_t dummy;
	ساخت maintenance_تسلسل_opts opts = MAINTENANCE_تسلسل_OPTS_INIT;
	ساخت gc_config cfg = GC_CONFIG_INIT;
	ثابت حرف *prune_expire_sentinel = "sentinel";
	ثابت حرف *prune_expire_arg = prune_expire_sentinel;
	صحیح ret;
	ساخت option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.قدر = &prune_expire_arg,
			.argh = N_("date"),
			.help = N_("prune unreferenced objects"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)prune_expire_arg,
		},
		OPT_BOOL(0, "cruft", &cfg.cruft_packs, N_("pack unreferenced objects separately")),
		OPT_UNSIGNED(0, "max-cruft-size", &cfg.max_cruft_سائز,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL(0, "aggressive", &aggressive, N_("be more thorough (increased runtime)")),
		OPT_BOOL_F(0, "auto", &opts.auto_flag, N_("enable auto-gc mode"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform garbage collection in the background")),
		OPT_BOOL_F(0, "force", &force,
			   N_("force running gc even if there may be another gc running"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "keep-largest-pack", &keep_سب_سے_بڑا_pack,
			 N_("repack all other packs except the largest pack")),
		OPT_STRING(0, "expire-to", &cfg.repack_expire_مقصد, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_HIDDEN_BOOL(0, "skip-foreground-tasks", &skip_foreground_tasks,
			   N_("skip maintenance tasks typically done in the foreground")),
		OPT_انتہا()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_gc_usage, builtin_gc_options);

	gc_config(&cfg);

	اگر (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_وقت))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	اگر (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, سابقہ, builtin_gc_options,
			     builtin_gc_usage, 0);
	اگر (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	اگر (prune_expire_arg != prune_expire_sentinel) {
		آزاد(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	اگر (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	اگر (opts.auto_flag) {
		ساخت odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		اگر (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		اگر (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    تسلسل_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			جاؤ برآمد;
		}

		اگر (!opts.quiet) {
			اگر (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			ورنہ
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	اگر (opts.detach > 0) {
		ret = report_آخری_gc_error();
		اگر (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			جاؤ برآمد;

		} ورنہ اگر (ret) {
			/* an I/O error occurred, already reported */
			جاؤ برآمد;
		}

		اگر (!skip_foreground_tasks) {
			اگر (تالا_repo_for_gc(force, &pid)) {
				ret = 0;
				جاؤ برآمد;
			}

			اگر (gc_foreground_tasks(&opts, &cfg) < 0)
				die(تہی);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = تالا_repo_for_gc(force, &pid);
	اگر (name) {
		اگر (opts.auto_flag) {
			ret = 0;
			جاؤ برآمد; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	اگر (daemonized) {
		حرف *راستہ = repo_git_راستہ(the_repository, "gc.log");
		hold_تالا_file_for_update(&log_تالا, راستہ,
					  تالا_DIE_ON_ERROR);
		dup2(get_تالا_file_fd(&log_تالا), 2);
		atexit(process_log_file_at_exit);
		آزاد(راستہ);
	}

	اگر (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	اگر (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_تسلسل, "worktree");

	اگر (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_تسلسل, "rerere");

	اگر (maintenance_task_odb(&opts, &cfg, keep_سب_سے_بڑا_pack, aggressive))
		die(تہی);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	اگر (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	اگر (the_repository->settings.gc_write_commit_گراف == 1)
		write_commit_گراف_reachable(the_repository,
					     the_repository->objects->ماخذ->راستہ,
					     !opts.quiet && !daemonized ? COMMIT_گراف_WRITE_پیش_رفت : 0,
					     تہی);

	اگر (!daemonized) {
		حرف *راستہ = repo_git_راستہ(the_repository, "gc.log");
		unlink(راستہ);
		آزاد(راستہ);
	}

برآمد:
	maintenance_تسلسل_opts_release(&opts);
	gc_config_release(&cfg);
	واپس 0;
}

ساکن ثابت حرف *ثابت builtin_maintenance_تسلسل_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	تہی
};

ساکن صحیح maintenance_opt_جدول_وقت(ثابت ساخت option *opt, ثابت حرف *arg,
				    صحیح unset)
{
	شمار جدول_وقت_ترجیح *ترجیح = opt->قدر;

	اگر (unset)
		die(_("--no-schedule is not allowed"));

	*ترجیح = parse_جدول_وقت(arg);

	اگر (!*ترجیح)
		die(_("unrecognized --schedule argument '%s'"), arg);

	واپس 0;
}

ساخت cg_auto_ڈیٹا {
	صحیح عدد_not_اندر_گراف;
	صحیح حد_بندی;
};

ساکن صحیح dfs_on_ref(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	ساخت cg_auto_ڈیٹا *ڈیٹا = (ساخت cg_auto_ڈیٹا *)cb_ڈیٹا;
	صحیح نتیجہ = 0;
	ثابت ساخت object_id *maybe_peeled = ref->oid;
	ساخت object_id peeled;
	ساخت commit_list *ڈھیر = تہی;
	ساخت commit *commit;

	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	اگر (odb_read_object_info(the_repository->objects, maybe_peeled, تہی) != OBJ_COMMIT)
		واپس 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	اگر (!commit || commit->object.flags & نظر_آیا_2)
		واپس 0;
	commit->object.flags |= نظر_آیا_2;

	اگر (repo_parse_commit(the_repository, commit) ||
	    commit_گراف_position(commit) != COMMIT_NOT_FROM_گراف)
		واپس 0;

	ڈیٹا->عدد_not_اندر_گراف++;

	اگر (ڈیٹا->عدد_not_اندر_گراف >= ڈیٹا->حد_بندی)
		واپس 1;

	commit_list_داخل_کرو(commit, &ڈھیر);

	جب (!نتیجہ && ڈھیر) {
		ساخت commit_list *والد;

		commit = نکالو_commit(&ڈھیر);

		برائے (والد = commit->parents; والد; والد = والد->اگلا) {
			اگر (repo_parse_commit(the_repository, والد->شے) ||
			    commit_گراف_position(والد->شے) != COMMIT_NOT_FROM_گراف ||
			    والد->شے->object.flags & نظر_آیا_2)
				جاری;

			والد->شے->object.flags |= نظر_آیا_2;
			ڈیٹا->عدد_not_اندر_گراف++;

			اگر (ڈیٹا->عدد_not_اندر_گراف >= ڈیٹا->حد_بندی) {
				نتیجہ = 1;
				توڑو;
			}

			commit_list_داخل_کرو(والد->شے, &ڈھیر);
		}
	}

	commit_list_free(ڈھیر);
	واپس نتیجہ;
}

ساکن صحیح should_write_commit_گراف(ساخت gc_config *cfg غیر_مستعمل)
{
	صحیح نتیجہ;
	ساخت cg_auto_ڈیٹا ڈیٹا;

	ڈیٹا.عدد_not_اندر_گراف = 0;
	ڈیٹا.حد_بندی = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &ڈیٹا.حد_بندی);

	اگر (!ڈیٹا.حد_بندی)
		واپس 0;
	اگر (ڈیٹا.حد_بندی < 0)
		واپس 1;

	نتیجہ = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &ڈیٹا);

	repo_clear_commit_marks(the_repository, نظر_آیا_2);

	واپس نتیجہ;
}

ساکن صحیح تسلسل_write_commit_گراف(ساخت maintenance_تسلسل_opts *opts)
{
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	بچہ.git_cmd = 1;
	بچہ.odb_مقصد_close = the_repository->objects;
	strvec_pushl(&بچہ.دلائل, "commit-graph", "write",
		     "--split", "--reachable", تہی);

	اگر (opts->quiet)
		strvec_ڈالو(&بچہ.دلائل, "--no-progress");
	ورنہ
		strvec_ڈالو(&بچہ.دلائل, "--progress");

	واپس !!تسلسل_command(&بچہ);
}

ساکن صحیح maintenance_task_commit_گراف(ساخت maintenance_تسلسل_opts *opts,
					 ساخت gc_config *cfg غیر_مستعمل)
{
	prepare_repo_settings(the_repository);
	اگر (!the_repository->settings.core_commit_گراف)
		واپس 0;

	اگر (تسلسل_write_commit_گراف(opts)) {
		error(_("failed to write commit-graph"));
		واپس 1;
	}

	واپس 0;
}

ساکن صحیح fetch_remote(ساخت remote *remote, خالی *cbdata)
{
	ساخت maintenance_تسلسل_opts *opts = cbdata;
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	اگر (remote->skip_default_update)
		واپس 0;

	بچہ.git_cmd = 1;
	strvec_pushl(&بچہ.دلائل, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     تہی);

	اگر (opts->quiet)
		strvec_ڈالو(&بچہ.دلائل, "--quiet");

	واپس !!تسلسل_command(&بچہ);
}

ساکن صحیح maintenance_task_prefetch(ساخت maintenance_تسلسل_opts *opts,
				     ساخت gc_config *cfg غیر_مستعمل)
{
	اگر (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		واپس 1;
	}

	واپس 0;
}

ساکن صحیح maintenance_task_gc_foreground(ساخت maintenance_تسلسل_opts *opts,
					  ساخت gc_config *cfg)
{
	واپس gc_foreground_tasks(opts, cfg);
}

ساکن صحیح maintenance_task_gc_background(ساخت maintenance_تسلسل_opts *opts,
					  ساخت gc_config *cfg غیر_مستعمل)
{
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	بچہ.git_cmd = 1;
	بچہ.odb_مقصد_close = the_repository->objects;
	strvec_ڈالو(&بچہ.دلائل, "gc");

	اگر (opts->auto_flag)
		strvec_ڈالو(&بچہ.دلائل, "--auto");
	اگر (opts->quiet)
		strvec_ڈالو(&بچہ.دلائل, "--quiet");
	ورنہ
		strvec_ڈالو(&بچہ.دلائل, "--no-quiet");
	strvec_ڈالو(&بچہ.دلائل, "--no-detach");
	strvec_ڈالو(&بچہ.دلائل, "--skip-foreground-tasks");

	واپس تسلسل_command(&بچہ);
}

ساکن صحیح gc_condition(ساخت gc_config *cfg)
{
	ساخت odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	واپس odb_optimize_required(the_repository->objects, &opts);
}

ساکن صحیح prune_packed(ساخت maintenance_تسلسل_opts *opts)
{
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	بچہ.git_cmd = 1;
	strvec_ڈالو(&بچہ.دلائل, "prune-packed");

	اگر (opts->quiet)
		strvec_ڈالو(&بچہ.دلائل, "--quiet");

	واپس !!تسلسل_command(&بچہ);
}

ساخت write_loose_object_ڈیٹا {
	FILE *اندر;
	صحیح گنتی;
	صحیح batch_سائز;
};

ساکن صحیح loose_object_auto_حد_بندی = 100;

ساکن صحیح loose_object_گنتی(ثابت ساخت object_id *oid غیر_مستعمل,
			      ثابت حرف *راستہ غیر_مستعمل,
			      خالی *ڈیٹا)
{
	صحیح *گنتی = (صحیح*)ڈیٹا;
	اگر (++(*گنتی) >= loose_object_auto_حد_بندی)
		واپس 1;
	واپس 0;
}

ساکن صحیح loose_object_auto_condition(ساخت gc_config *cfg غیر_مستعمل)
{
	صحیح گنتی = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_حد_بندی);

	اگر (!loose_object_auto_حد_بندی)
		واپس 0;
	اگر (loose_object_auto_حد_بندی < 0)
		واپس 1;

	واپس for_each_loose_file_اندر_ماخذ(the_repository->objects->ماخذ,
					     loose_object_گنتی,
					     تہی, تہی, &گنتی);
}

ساکن صحیح bail_on_loose(ثابت ساخت object_id *oid غیر_مستعمل,
			 ثابت حرف *راستہ غیر_مستعمل,
			 خالی *ڈیٹا غیر_مستعمل)
{
	واپس 1;
}

ساکن صحیح write_loose_object_مقصد_stdin(ثابت ساخت object_id *oid,
				       ثابت حرف *راستہ غیر_مستعمل,
				       خالی *ڈیٹا)
{
	ساخت write_loose_object_ڈیٹا *d = (ساخت write_loose_object_ڈیٹا *)ڈیٹا;

	fprintf(d->اندر, "%s\n", oid_مقصد_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	واپس ++(d->گنتی) > d->batch_سائز;
}

ساکن صحیح pack_loose(ساخت maintenance_تسلسل_opts *opts)
{
	ساخت repository *r = the_repository;
	صحیح نتیجہ = 0;
	ساخت write_loose_object_ڈیٹا ڈیٹا;
	ساخت بچہ_process pack_proc = بچہ_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	اگر (!for_each_loose_file_اندر_ماخذ(r->objects->ماخذ,
					   bail_on_loose,
					   تہی, تہی, تہی))
		واپس 0;

	pack_proc.git_cmd = 1;

	strvec_ڈالو(&pack_proc.دلائل, "pack-objects");
	اگر (opts->quiet)
		strvec_ڈالو(&pack_proc.دلائل, "--quiet");
	ورنہ
		strvec_ڈالو(&pack_proc.دلائل, "--no-quiet");
	strvec_pushf(&pack_proc.دلائل, "%s/pack/loose", r->objects->ماخذ->راستہ);

	pack_proc.اندر = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.برآمد = -1;

	اگر (آغاز_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		واپس 1;
	}

	ڈیٹا.اندر = xfdopen(pack_proc.اندر, "w");
	ڈیٹا.گنتی = 0;
	ڈیٹا.batch_سائز = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &ڈیٹا.batch_سائز);

	/* If configured as 0, then remove limit. */
	اگر (!ڈیٹا.batch_سائز)
		ڈیٹا.batch_سائز = زیادہ_سے_زیادہ_عدد;
	ورنہ اگر (ڈیٹا.batch_سائز > 0)
		ڈیٹا.batch_سائز--; /* Decrease for equality on limit. */

	for_each_loose_file_اندر_ماخذ(r->objects->ماخذ,
				      write_loose_object_مقصد_stdin,
				      تہی, تہی, &ڈیٹا);

	fclose(ڈیٹا.اندر);

	اگر (اختتام_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		نتیجہ = 1;
	}

	واپس نتیجہ;
}

ساکن صحیح maintenance_task_loose_objects(ساخت maintenance_تسلسل_opts *opts,
					  ساخت gc_config *cfg غیر_مستعمل)
{
	واپس prune_packed(opts) || pack_loose(opts);
}

ساکن صحیح incremental_repack_auto_condition(ساخت gc_config *cfg غیر_مستعمل)
{
	ساخت packed_git *p;
	صحیح incremental_repack_auto_حد_بندی = 10;
	صحیح گنتی = 0;

	prepare_repo_settings(the_repository);
	اگر (!the_repository->settings.core_multi_pack_اشاریہ)
		واپس 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_حد_بندی);

	اگر (!incremental_repack_auto_حد_بندی)
		واپس 0;
	اگر (incremental_repack_auto_حد_بندی < 0)
		واپس 1;

	repo_for_each_pack(the_repository, p) {
		اگر (گنتی >= incremental_repack_auto_حد_بندی)
			توڑو;
		اگر (!p->multi_pack_اشاریہ)
			گنتی++;
	}

	واپس گنتی >= incremental_repack_auto_حد_بندی;
}

ساکن صحیح multi_pack_اشاریہ_write(ساخت maintenance_تسلسل_opts *opts)
{
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	بچہ.git_cmd = 1;
	strvec_pushl(&بچہ.دلائل, "multi-pack-index", "write", تہی);

	اگر (opts->quiet)
		strvec_ڈالو(&بچہ.دلائل, "--no-progress");
	ورنہ
		strvec_ڈالو(&بچہ.دلائل, "--progress");

	اگر (تسلسل_command(&بچہ))
		واپس error(_("failed to write multi-pack-index"));

	واپس 0;
}

ساکن صحیح multi_pack_اشاریہ_expire(ساخت maintenance_تسلسل_opts *opts)
{
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	بچہ.git_cmd = 1;
	بچہ.odb_مقصد_close = the_repository->objects;
	strvec_pushl(&بچہ.دلائل, "multi-pack-index", "expire", تہی);

	اگر (opts->quiet)
		strvec_ڈالو(&بچہ.دلائل, "--no-progress");
	ورنہ
		strvec_ڈالو(&بچہ.دلائل, "--progress");

	اگر (تسلسل_command(&بچہ))
		واپس error(_("'git multi-pack-index expire' failed"));

	واپس 0;
}

#تعریف_کرو TWO_GIGABYTES (INT32_MAX)

ساکن off_t get_auto_pack_سائز(خالی)
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
	off_t max_سائز = 0;
	off_t دوسرا_سب_سے_بڑا_سائز = 0;
	off_t نتیجہ_سائز;
	ساخت packed_git *p;
	ساخت repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		اگر (p->pack_سائز > max_سائز) {
			دوسرا_سب_سے_بڑا_سائز = max_سائز;
			max_سائز = p->pack_سائز;
		} ورنہ اگر (p->pack_سائز > دوسرا_سب_سے_بڑا_سائز)
			دوسرا_سب_سے_بڑا_سائز = p->pack_سائز;
	}

	نتیجہ_سائز = دوسرا_سب_سے_بڑا_سائز + 1;

	/* But limit ourselves to a batch size of 2g */
	اگر (نتیجہ_سائز > TWO_GIGABYTES)
		نتیجہ_سائز = TWO_GIGABYTES;

	واپس نتیجہ_سائز;
}

ساکن صحیح multi_pack_اشاریہ_repack(ساخت maintenance_تسلسل_opts *opts)
{
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	بچہ.git_cmd = 1;
	بچہ.odb_مقصد_close = the_repository->objects;
	strvec_pushl(&بچہ.دلائل, "multi-pack-index", "repack", تہی);

	اگر (opts->quiet)
		strvec_ڈالو(&بچہ.دلائل, "--no-progress");
	ورنہ
		strvec_ڈالو(&بچہ.دلائل, "--progress");

	strvec_pushf(&بچہ.دلائل, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_سائز());

	اگر (تسلسل_command(&بچہ))
		واپس error(_("'git multi-pack-index repack' failed"));

	واپس 0;
}

ساکن صحیح maintenance_task_incremental_repack(ساخت maintenance_تسلسل_opts *opts,
					       ساخت gc_config *cfg غیر_مستعمل)
{
	prepare_repo_settings(the_repository);
	اگر (!the_repository->settings.core_multi_pack_اشاریہ) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		واپس 0;
	}

	اگر (multi_pack_اشاریہ_write(opts))
		واپس 1;
	اگر (multi_pack_اشاریہ_expire(opts))
		واپس 1;
	اگر (multi_pack_اشاریہ_repack(opts))
		واپس 1;
	واپس 0;
}

ساکن صحیح maintenance_task_geometric_repack(ساخت maintenance_تسلسل_opts *opts,
					     ساخت gc_config *cfg)
{
	ساخت odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	اگر (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	واپس odb_optimize(the_repository->objects, &odb_opts);
}

ساکن صحیح geometric_repack_auto_condition(ساخت gc_config *cfg)
{
	ساخت odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	واپس odb_optimize_required(the_repository->objects, &opts);
}

قسمتعریف صحیح (*maintenance_task_fn)(ساخت maintenance_تسلسل_opts *opts,
				   ساخت gc_config *cfg);
قسمتعریف صحیح (*maintenance_auto_fn)(ساخت gc_config *cfg);

ساخت maintenance_task {
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

ساکن ثابت ساخت maintenance_task tasks[] = {
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

شمار task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

شمار auto_gc_hook_نتیجہ {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_تسلسل = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

ساکن صحیح maybe_تسلسل_task(ثابت ساخت maintenance_task *task,
			  ساخت repository *repo,
			  ساخت maintenance_تسلسل_opts *opts,
			  ساخت gc_config *cfg,
			  شمار task_phase phase,
			  شمار auto_gc_hook_نتیجہ *auto_gc_hook_نتیجہ)
{
	صحیح foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ثابت حرف *region = foreground ? "maintenance foreground" : "maintenance";
	صحیح ret = 0;

	اگر (!fn)
		واپس 0;
	اگر (opts->auto_flag) {
		اگر (*auto_gc_hook_نتیجہ == AUTO_GC_HOOK_SKIP)
			واپس 0;

		اگر (!task->auto_condition || !task->auto_condition(cfg))
			واپس 0;

		اگر (*auto_gc_hook_نتیجہ == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_نتیجہ = تسلسل_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_تسلسل;
		اگر (*auto_gc_hook_نتیجہ == AUTO_GC_HOOK_SKIP)
			واپس 0;
	}

	trace2_region_enter(region, task->name, repo);
	اگر (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	واپس ret;
}

ساکن صحیح maintenance_تسلسل_tasks(ساخت maintenance_تسلسل_opts *opts,
				 ساخت gc_config *cfg)
{
	صحیح نتیجہ = 0;
	ساخت تالا_file lk;
	ساخت repository *r = the_repository;
	حرف *تالا_راستہ = xstrfmt("%s/maintenance", r->objects->ماخذ->راستہ);
	شمار auto_gc_hook_نتیجہ auto_gc_hook_نتیجہ = AUTO_GC_HOOK_UNDECIDED;

	اگر (repo_hold_تالا_file_for_update(r, &lk, تالا_راستہ, تالا_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		اگر (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				تالا_راستہ);
		آزاد(تالا_راستہ);
		واپس 0;
	}
	آزاد(تالا_راستہ);

	برائے (سائز_t i = 0; i < opts->tasks_nr; i++)
		اگر (maybe_تسلسل_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_نتیجہ))
			نتیجہ = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	اگر (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	برائے (سائز_t i = 0; i < opts->tasks_nr; i++)
		اگر (maybe_تسلسل_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_نتیجہ))
			نتیجہ = 1;

	rollback_تالا_file(&lk);
	واپس نتیجہ;
}

شمار maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

ساخت maintenance_strategy {
	ساخت {
		غیرعلامتی type;
		شمار جدول_وقت_ترجیح جدول_وقت;
	} tasks[TASK_گنتی];
};

ساکن ثابت ساخت maintenance_strategy none_strategy = { 0 };

ساکن ثابت ساخت maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.جدول_وقت = جدول_وقت_DAILY,
		},
	},
};

ساکن ثابت ساخت maintenance_strategy incremental_strategy = {
	.tasks = {
		[TASK_COMMIT_گراف] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.جدول_وقت = جدول_وقت_HOURLY,
		},
		[TASK_PREFETCH] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.جدول_وقت = جدول_وقت_HOURLY,
		},
		[TASK_INCREMENTAL_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.جدول_وقت = جدول_وقت_DAILY,
		},
		[TASK_LOOSE_OBJECTS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.جدول_وقت = جدول_وقت_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.جدول_وقت = جدول_وقت_WEEKLY,
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

ساکن ثابت ساخت maintenance_strategy geometric_strategy = {
	.tasks = {
		[TASK_COMMIT_گراف] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.جدول_وقت = جدول_وقت_HOURLY,
		},
		[TASK_GEOMETRIC_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.جدول_وقت = جدول_وقت_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.جدول_وقت = جدول_وقت_DAILY,
		},
		[TASK_RERERE_GC] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.جدول_وقت = جدول_وقت_WEEKLY,
		},
		[TASK_REFLOG_EXPIRE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.جدول_وقت = جدول_وقت_WEEKLY,
		},
		[TASK_WORKTREE_PRUNE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.جدول_وقت = جدول_وقت_WEEKLY,
		},
	},
};

ساکن ساخت maintenance_strategy parse_maintenance_strategy(ثابت حرف *name)
{
	اگر (!strcasecmp(name, "incremental"))
		واپس incremental_strategy;
	اگر (!strcasecmp(name, "gc"))
		واپس gc_strategy;
	اگر (!strcasecmp(name, "geometric"))
		واپس geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

ساکن خالی initialize_task_config(ساخت maintenance_تسلسل_opts *opts,
				   ثابت ساخت string_list *selected_tasks)
{
	ساخت strbuf config_name = STRBUF_INIT;
	ساخت maintenance_strategy strategy;
	شمار maintenance_type type;
	ثابت حرف *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	اگر (selected_tasks->nr) {
		برائے (سائز_t i = 0; i < selected_tasks->nr; i++) {
			شمار maintenance_task_label label = (intptr_t)selected_tasks->اشیاء[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		واپس;
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
	اگر (opts->جدول_وقت) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} ورنہ {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	اگر (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	برائے (سائز_t i = 0; i < TASK_گنتی; i++) {
		صحیح config_قدر;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		اگر (!repo_config_get_bool(the_repository, config_name.buf, &config_قدر))
			strategy.tasks[i].type = config_قدر ? type : 0;
		اگر (!(strategy.tasks[i].type & type))
			جاری;

		اگر (opts->جدول_وقت) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			اگر (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].جدول_وقت = parse_جدول_وقت(config_str);
			اگر (strategy.tasks[i].جدول_وقت < opts->جدول_وقت)
				جاری;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

ساکن صحیح task_option_parse(ثابت ساخت option *opt,
			     ثابت حرف *arg, صحیح unset)
{
	ساخت string_list *selected_tasks = opt->قدر;
	سائز_t i;

	BUG_ON_OPT_NEG(unset);

	برائے (i = 0; i < TASK_گنتی; i++)
		اگر (!strcasecmp(tasks[i].name, arg))
			توڑو;
	اگر (i >= TASK_گنتی) {
		error(_("'%s' is not a valid task"), arg);
		واپس 1;
	}

	اگر (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		واپس 1;
	}

	string_list_append(selected_tasks, arg)->util = (خالی *)(intptr_t)i;

	واپس 0;
}

ساکن صحیح maintenance_تسلسل(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo غیر_مستعمل)
{
	ساخت maintenance_تسلسل_opts opts = MAINTENANCE_تسلسل_OPTS_INIT;
	ساخت string_list selected_tasks = STRING_LIST_INIT_DUP;
	ساخت gc_config cfg = GC_CONFIG_INIT;
	ساخت option builtin_maintenance_تسلسل_options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform maintenance in the background")),
		OPT_CALLBACK(0, "schedule", &opts.جدول_وقت, N_("frequency"),
			     N_("run tasks based on frequency"),
			     maintenance_opt_جدول_وقت),
		OPT_BOOL(0, "quiet", &opts.quiet,
			 N_("do not report progress or other information over stderr")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			N_("run a specific task"),
			PARSE_OPT_NONEG, task_option_parse),
		OPT_انتہا()
	};
	صحیح ret;

	opts.quiet = !isatty(2);

	argc = parse_options(argc, argv, سابقہ,
			     builtin_maintenance_تسلسل_options,
			     builtin_maintenance_تسلسل_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	die_for_incompatible_opt2(opts.auto_flag, "--auto",
				  opts.جدول_وقت, "--schedule=");
	die_for_incompatible_opt2(selected_tasks.nr, "--task=",
				  opts.جدول_وقت, "--schedule=");

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	اگر (argc != 0)
		usage_with_options(builtin_maintenance_تسلسل_usage,
				   builtin_maintenance_تسلسل_options);

	ret = maintenance_تسلسل_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_تسلسل_opts_release(&opts);
	gc_config_release(&cfg);
	واپس ret;
}

ساکن حرف *get_maintpath(خالی)
{
	ساخت strbuf sb = STRBUF_INIT;
	ثابت حرف *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	واپس strbuf_detach(&sb, تہی);
}

ساکن حرف ثابت * ثابت builtin_maintenance_رجسٹر_usage[] = {
	"git maintenance register [--config-file <path>]",
	تہی
};

ساکن صحیح maintenance_رجسٹر(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				ساخت repository *repo غیر_مستعمل)
{
	حرف *config_file = تہی;
	ساخت option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_انتہا(),
	};
	صحیح ملا_2 = 0;
	ثابت حرف *کلید = "maintenance.repo";
	حرف *maintpath = get_maintpath();
	ساخت string_list_شے *شے;
	ثابت ساخت string_list *list;

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_maintenance_رجسٹر_usage, 0);
	اگر (argc)
		usage_with_options(builtin_maintenance_رجسٹر_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_سیٹ(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	اگر (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_سیٹ(the_repository, "maintenance.strategy", "incremental");

	اگر (!repo_config_get_string_multi(the_repository, کلید, &list)) {
		for_each_string_list_شے(شے, list) {
			اگر (!strcmp(maintpath, شے->string)) {
				ملا_2 = 1;
				توڑو;
			}
		}
	}

	اگر (!ملا_2) {
		صحیح rc;
		حرف *global_config_file = تہی;

		اگر (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		اگر (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_سیٹ_multivar_اندر_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, تہی, 0);
		آزاد(global_config_file);

		اگر (rc)
			die(_("unable to add '%s' value of '%s'"),
			    کلید, maintpath);
	}

	آزاد(maintpath);
	واپس 0;
}

ساکن حرف ثابت * ثابت builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	تہی
};

ساکن صحیح maintenance_unregister(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				  ساخت repository *repo غیر_مستعمل)
{
	صحیح force = 0;
	حرف *config_file = تہی;
	ساخت option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_انتہا(),
	};
	ثابت حرف *کلید = "maintenance.repo";
	حرف *maintpath = get_maintpath();
	صحیح ملا_2 = 0;
	ساخت string_list_شے *شے;
	ثابت ساخت string_list *list;
	ساخت config_سیٹ cs = { { 0 } };

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_maintenance_unregister_usage, 0);
	اگر (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	اگر (config_file) {
		git_configset_init(&cs);
		git_configset_جوڑو_file(&cs, config_file);
	}
	اگر (!(config_file
	      ? git_configset_get_string_multi(&cs, کلید, &list)
	      : repo_config_get_string_multi(the_repository, کلید, &list))) {
		for_each_string_list_شے(شے, list) {
			اگر (!strcmp(maintpath, شے->string)) {
				ملا_2 = 1;
				توڑو;
			}
		}
	}

	اگر (ملا_2) {
		صحیح rc;
		حرف *global_config_file = تہی;

		اگر (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		اگر (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_سیٹ_multivar_اندر_file_gently(the_repository,
			config_file, کلید, تہی, maintpath, تہی,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_قدر);
		آزاد(global_config_file);

		اگر (rc &&
		    (!force || rc == CONFIG_NOTHING_سیٹ))
			die(_("unable to unset '%s' value of '%s'"),
			    کلید, maintpath);
	} ورنہ اگر (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	آزاد(maintpath);
	واپس 0;
}

ساکن ثابت حرف *get_frequency(شمار جدول_وقت_ترجیح جدول_وقت)
{
	انتخاب (جدول_وقت) {
	حالت جدول_وقت_HOURLY:
		واپس "hourly";
	حالت جدول_وقت_DAILY:
		واپس "daily";
	حالت جدول_وقت_WEEKLY:
		واپس "weekly";
	طےشدہ:
		BUG("invalid schedule %d", جدول_وقت);
	}
}

ساکن ثابت حرف *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	تہی
};

ساکن ثابت حرف *get_extra_config_parameters(خالی) {
	ساکن ثابت حرف *نتیجہ = تہی;
	ساخت strbuf builder = STRBUF_INIT;

	اگر (نتیجہ)
		واپس نتیجہ;

	برائے (ثابت حرف **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	نتیجہ = strbuf_detach(&builder, تہی);
	واپس نتیجہ;
}

ساکن ثابت حرف *get_extra_launchctl_لڑیاں(خالی) {
	ساکن ثابت حرف *نتیجہ = تہی;
	ساخت strbuf builder = STRBUF_INIT;

	اگر (نتیجہ)
		واپس نتیجہ;

	برائے (ثابت حرف **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	نتیجہ = strbuf_detach(&builder, تہی);
	واپس نتیجہ;
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
ساکن صحیح get_جدول_وقت_cmd(ثابت حرف *cmd, صحیح *is_دستیاب, حرف **برآمد)
{
	حرف *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	ساخت string_list_شے *شے;
	ساخت string_list list = STRING_LIST_INIT_NODUP;

	اگر (!testing) {
		اگر (برآمد)
			*برآمد = xstrdup(cmd);
		واپس 0;
	}

	اگر (is_دستیاب)
		*is_دستیاب = 0;

	string_list_بانٹو_اندر_رکھو(&list, testing, ",", -1);
	for_each_string_list_شے(شے, &list) {
		ساخت string_list pair = STRING_LIST_INIT_NODUP;

		اگر (string_list_بانٹو_اندر_رکھو(&pair, شے->string, ":", 2) != 2)
			جاری;

		اگر (!strcmp(cmd, pair.اشیاء[0].string)) {
			اگر (برآمد)
				*برآمد = xstrdup(pair.اشیاء[1].string);
			اگر (is_دستیاب)
				*is_دستیاب = 1;
			string_list_clear(&pair, 0);
			جاؤ برآمد;
		}

		string_list_clear(&pair, 0);
	}

	اگر (برآمد)
		*برآمد = xstrdup(cmd);

برآمد:
	string_list_clear(&list, 0);
	آزاد(testing);
	واپس 1;
}

ساکن صحیح get_random_minute(خالی)
{
	/* Use a static value when under tests. */
	اگر (getenv("GIT_TEST_MAINT_SCHEDULER"))
		واپس 13;

	واپس git_rand(0) % 60;
}

ساکن صحیح is_launchctl_دستیاب(خالی)
{
	صحیح is_دستیاب;
	اگر (get_جدول_وقت_cmd("launchctl", &is_دستیاب, تہی))
		واپس is_دستیاب;

#اگر_تعریف_شدہ __APPLE__
	واپس 1;
#ورنہ
	واپس 0;
#اگر_ختم
}

ساکن حرف *launchctl_service_name(ثابت حرف *frequency)
{
	ساخت strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	واپس strbuf_detach(&label, تہی);
}

ساکن حرف *launchctl_service_filename(ثابت حرف *name)
{
	حرف *expanded;
	ساخت strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_راستہ(filename.buf, 1);
	اگر (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	واپس expanded;
}

ساکن حرف *launchctl_get_uid(خالی)
{
	واپس xstrfmt("gui/%d", getuid());
}

ساکن صحیح launchctl_boot_plist(صحیح enable, ثابت حرف *filename)
{
	حرف *cmd;
	صحیح نتیجہ;
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;
	حرف *uid = launchctl_get_uid();

	get_جدول_وقت_cmd("launchctl", تہی, &cmd);
	strvec_بانٹو(&بچہ.دلائل, cmd);
	strvec_pushl(&بچہ.دلائل, enable ? "bootstrap" : "bootout", uid,
		     filename, تہی);

	بچہ.no_stderr = 1;
	بچہ.no_stdout = 1;

	اگر (آغاز_command(&بچہ))
		die(_("failed to start launchctl"));

	نتیجہ = اختتام_command(&بچہ);

	آزاد(cmd);
	آزاد(uid);
	واپس نتیجہ;
}

ساکن صحیح launchctl_remove_plist(شمار جدول_وقت_ترجیح جدول_وقت)
{
	ثابت حرف *frequency = get_frequency(جدول_وقت);
	حرف *name = launchctl_service_name(frequency);
	حرف *filename = launchctl_service_filename(name);
	صحیح نتیجہ = launchctl_boot_plist(0, filename);
	unlink(filename);
	آزاد(filename);
	آزاد(name);
	واپس نتیجہ;
}

ساکن صحیح launchctl_remove_plists(خالی)
{
	واپس launchctl_remove_plist(جدول_وقت_HOURLY) ||
	       launchctl_remove_plist(جدول_وقت_DAILY) ||
	       launchctl_remove_plist(جدول_وقت_WEEKLY);
}

ساکن صحیح launchctl_list_شامل_ہے_plist(ثابت حرف *name, ثابت حرف *cmd)
{
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	strvec_بانٹو(&بچہ.دلائل, cmd);
	strvec_pushl(&بچہ.دلائل, "list", name, تہی);

	بچہ.no_stderr = 1;
	بچہ.no_stdout = 1;

	اگر (آغاز_command(&بچہ))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	واپس !اختتام_command(&بچہ);
}

ساکن صحیح launchctl_جدول_وقت_plist(ثابت حرف *exec_راستہ, شمار جدول_وقت_ترجیح جدول_وقت)
{
	صحیح i, fd;
	ثابت حرف *preamble, *repeat;
	ثابت حرف *frequency = get_frequency(جدول_وقت);
	حرف *name = launchctl_service_name(frequency);
	حرف *filename = launchctl_service_filename(name);
	ساخت تالا_file lk = تالا_INIT;
	ساکن غیرعلامتی طویل تالا_file_timeout_ms = ULONG_MAX;
	ساخت strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	ساخت stat st;
	حرف *cmd;
	صحیح minute = get_random_minute();

	get_جدول_وقت_cmd("launchctl", تہی, &cmd);
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
	strbuf_addf(&plist, preamble, name, exec_راستہ, exec_راستہ,
		    get_extra_launchctl_لڑیاں(), frequency);

	انتخاب (جدول_وقت) {
	حالت جدول_وقت_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		برائے (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		توڑو;

	حالت جدول_وقت_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		برائے (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		توڑو;

	حالت جدول_وقت_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		توڑو;

	طےشدہ:
		/* unreachable */
		توڑو;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	اگر (محفوظ_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	اگر ((طویل)تالا_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &تالا_file_timeout_ms))
		تالا_file_timeout_ms = 150;

	fd = hold_تالا_file_for_update_timeout(&lk, filename, تالا_DIE_ON_ERROR,
					       تالا_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	اگر (!stat(filename, &st) && st.st_سائز == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_شامل_ہے_plist(name, cmd))
		rollback_تالا_file(&lk);
	ورنہ {
		اگر (write_اندر_بھرا(fd, plist.buf, plist.len) < 0 ||
		    commit_تالا_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		اگر (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	آزاد(filename);
	آزاد(name);
	آزاد(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	واپس 0;
}

ساکن صحیح launchctl_جوڑو_plists(خالی)
{
	ثابت حرف *exec_راستہ = git_exec_راستہ();

	واپس launchctl_جدول_وقت_plist(exec_راستہ, جدول_وقت_HOURLY) ||
	       launchctl_جدول_وقت_plist(exec_راستہ, جدول_وقت_DAILY) ||
	       launchctl_جدول_وقت_plist(exec_راستہ, جدول_وقت_WEEKLY);
}

ساکن صحیح launchctl_update_جدول_وقت(صحیح تسلسل_maintenance, صحیح fd غیر_مستعمل)
{
	اگر (تسلسل_maintenance)
		واپس launchctl_جوڑو_plists();
	ورنہ
		واپس launchctl_remove_plists();
}

ساکن صحیح is_schtasks_دستیاب(خالی)
{
	صحیح is_دستیاب;
	اگر (get_جدول_وقت_cmd("schtasks", &is_دستیاب, تہی))
		واپس is_دستیاب;

#اگر_تعریف_شدہ GIT_WINDOWS_NATIVE
	واپس 1;
#ورنہ
	واپس 0;
#اگر_ختم
}

ساکن حرف *schtasks_task_name(ثابت حرف *frequency)
{
	ساخت strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	واپس strbuf_detach(&label, تہی);
}

ساکن صحیح schtasks_remove_task(شمار جدول_وقت_ترجیح جدول_وقت)
{
	حرف *cmd;
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;
	ثابت حرف *frequency = get_frequency(جدول_وقت);
	حرف *name = schtasks_task_name(frequency);

	get_جدول_وقت_cmd("schtasks", تہی, &cmd);
	strvec_بانٹو(&بچہ.دلائل, cmd);
	strvec_pushl(&بچہ.دلائل, "/delete", "/tn", name, "/f", تہی);
	آزاد(name);
	آزاد(cmd);

	واپس تسلسل_command(&بچہ);
}

ساکن صحیح schtasks_remove_tasks(خالی)
{
	واپس schtasks_remove_task(جدول_وقت_HOURLY) ||
	       schtasks_remove_task(جدول_وقت_DAILY) ||
	       schtasks_remove_task(جدول_وقت_WEEKLY);
}

ساکن صحیح schtasks_جدول_وقت_task(ثابت حرف *exec_راستہ, شمار جدول_وقت_ترجیح جدول_وقت)
{
	حرف *cmd;
	صحیح نتیجہ;
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;
	ثابت حرف *xml;
	ساخت tempfile *tfile;
	ثابت حرف *frequency = get_frequency(جدول_وقت);
	حرف *name = schtasks_task_name(frequency);
	ساخت strbuf tfilename = STRBUF_INIT;
	صحیح minute = get_random_minute();

	get_جدول_وقت_cmd("schtasks", تہی, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	اگر (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	انتخاب (جدول_وقت) {
	حالت جدول_وقت_HOURLY:
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
		توڑو;

	حالت جدول_وقت_DAILY:
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
		توڑو;

	حالت جدول_وقت_WEEKLY:
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
		توڑو;

	طےشدہ:
		توڑو;
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
	fprintf(tfile->fp, xml, exec_راستہ, exec_راستہ,
		get_extra_config_parameters(), frequency);
	strvec_بانٹو(&بچہ.دلائل, cmd);
	strvec_pushl(&بچہ.دلائل, "/create", "/tn", name, "/f", "/xml",
				  get_tempfile_راستہ(tfile), تہی);
	close_tempfile_gently(tfile);

	بچہ.no_stdout = 1;
	بچہ.no_stderr = 1;

	اگر (آغاز_command(&بچہ))
		die(_("failed to start schtasks"));
	نتیجہ = اختتام_command(&بچہ);

	delete_tempfile(&tfile);
	آزاد(name);
	آزاد(cmd);
	واپس نتیجہ;
}

ساکن صحیح schtasks_جدول_وقت_tasks(خالی)
{
	ثابت حرف *exec_راستہ = git_exec_راستہ();

	واپس schtasks_جدول_وقت_task(exec_راستہ, جدول_وقت_HOURLY) ||
	       schtasks_جدول_وقت_task(exec_راستہ, جدول_وقت_DAILY) ||
	       schtasks_جدول_وقت_task(exec_راستہ, جدول_وقت_WEEKLY);
}

ساکن صحیح schtasks_update_جدول_وقت(صحیح تسلسل_maintenance, صحیح fd غیر_مستعمل)
{
	اگر (تسلسل_maintenance)
		واپس schtasks_جدول_وقت_tasks();
	ورنہ
		واپس schtasks_remove_tasks();
}

MAYBE_غیر_مستعمل
ساکن صحیح check_crontab_process(ثابت حرف *cmd)
{
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	strvec_بانٹو(&بچہ.دلائل, cmd);
	strvec_ڈالو(&بچہ.دلائل, "-l");
	بچہ.no_stdin = 1;
	بچہ.no_stdout = 1;
	بچہ.no_stderr = 1;
	بچہ.silent_exec_failure = 1;

	اگر (آغاز_command(&بچہ))
		واپس 0;
	/* Ignore exit code, as an empty crontab will return error. */
	اختتام_command(&بچہ);
	واپس 1;
}

ساکن صحیح is_crontab_دستیاب(خالی)
{
	حرف *cmd;
	صحیح is_دستیاب;
	صحیح ret;

	اگر (get_جدول_وقت_cmd("crontab", &is_دستیاب, &cmd)) {
		ret = is_دستیاب;
		جاؤ برآمد;
	}

#اگر_تعریف_شدہ __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#ورنہ
	ret = check_crontab_process(cmd);
#اگر_ختم

برآمد:
	آزاد(cmd);
	واپس ret;
}

#تعریف_کرو BEGIN_سطر "# BEGIN GIT MAINTENANCE SCHEDULE"
#تعریف_کرو انتہا_سطر "# END GIT MAINTENANCE SCHEDULE"

ساکن صحیح crontab_update_جدول_وقت(صحیح تسلسل_maintenance, صحیح fd)
{
	حرف *cmd;
	صحیح نتیجہ = 0;
	صحیح اندر_old_region = 0;
	ساخت بچہ_process crontab_list = بچہ_PROCESS_INIT;
	ساخت بچہ_process crontab_edit = بچہ_PROCESS_INIT;
	FILE *cron_list, *cron_اندر;
	ساخت strbuf سطر = STRBUF_INIT;
	ساخت tempfile *tmpedit = تہی;
	صحیح minute = get_random_minute();

	get_جدول_وقت_cmd("crontab", تہی, &cmd);
	strvec_بانٹو(&crontab_list.دلائل, cmd);
	strvec_ڈالو(&crontab_list.دلائل, "-l");
	crontab_list.اندر = -1;
	crontab_list.برآمد = dup(fd);
	crontab_list.git_cmd = 0;

	اگر (آغاز_command(&crontab_list)) {
		نتیجہ = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		جاؤ برآمد;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	اختتام_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	اگر (!tmpedit) {
		نتیجہ = error(_("failed to create crontab temporary file"));
		جاؤ برآمد;
	}
	cron_اندر = fdopen_tempfile(tmpedit, "w");
	اگر (!cron_اندر) {
		نتیجہ = error(_("failed to open temporary file"));
		جاؤ برآمد;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	جب (!strbuf_getline_lf(&سطر, cron_list)) {
		اگر (!اندر_old_region && !strcmp(سطر.buf, BEGIN_سطر))
			اندر_old_region = 1;
		ورنہ اگر (اندر_old_region && !strcmp(سطر.buf, انتہا_سطر))
			اندر_old_region = 0;
		ورنہ اگر (!اندر_old_region)
			fprintf(cron_اندر, "%s\n", سطر.buf);
	}
	strbuf_release(&سطر);

	اگر (تسلسل_maintenance) {
		ساخت strbuf سطر_format = STRBUF_INIT;
		ثابت حرف *exec_راستہ = git_exec_راستہ();

		fprintf(cron_اندر, "%s\n", BEGIN_سطر);
		fprintf(cron_اندر,
			"# The following schedule was created by Git\n");
		fprintf(cron_اندر, "# Any edits made in this region might be\n");
		fprintf(cron_اندر,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&سطر_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_راستہ, exec_راستہ, get_extra_config_parameters());
		fprintf(cron_اندر, سطر_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_اندر, سطر_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_اندر, سطر_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&سطر_format);

		fprintf(cron_اندر, "\n%s\n", انتہا_سطر);
	}

	fflush(cron_اندر);

	strvec_بانٹو(&crontab_edit.دلائل, cmd);
	strvec_ڈالو(&crontab_edit.دلائل, get_tempfile_راستہ(tmpedit));
	crontab_edit.git_cmd = 0;

	اگر (آغاز_command(&crontab_edit)) {
		نتیجہ = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		جاؤ برآمد;
	}

	اگر (اختتام_command(&crontab_edit))
		نتیجہ = error(_("'crontab' died"));
	ورنہ
		fclose(cron_list);

برآمد:
	delete_tempfile(&tmpedit);
	آزاد(cmd);
	واپس نتیجہ;
}

ساکن صحیح real_is_systemd_timer_دستیاب(خالی)
{
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	strvec_pushl(&بچہ.دلائل, "systemctl", "--user", "list-timers", تہی);
	بچہ.no_stdin = 1;
	بچہ.no_stdout = 1;
	بچہ.no_stderr = 1;
	بچہ.silent_exec_failure = 1;

	اگر (آغاز_command(&بچہ))
		واپس 0;
	اگر (اختتام_command(&بچہ))
		واپس 0;
	واپس 1;
}

ساکن صحیح is_systemd_timer_دستیاب(خالی)
{
	صحیح is_دستیاب;

	اگر (get_جدول_وقت_cmd("systemctl", &is_دستیاب, تہی))
		واپس is_دستیاب;

	واپس real_is_systemd_timer_دستیاب();
}

ساکن حرف *xdg_config_home_systemd(ثابت حرف *filename)
{
	واپس xdg_config_home_for("systemd/user", filename);
}

#تعریف_کرو SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

ساکن صحیح systemd_timer_delete_timer_file(شمار جدول_وقت_ترجیح ترجیح)
{
	صحیح ret = 0;
	ثابت حرف *frequency = get_frequency(ترجیح);
	حرف *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	حرف *filename = xdg_config_home_systemd(local_timer_name);

	اگر (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	آزاد(filename);
	آزاد(local_timer_name);
	واپس ret;
}

ساکن صحیح systemd_timer_delete_service_template(خالی)
{
	صحیح ret = 0;
	حرف *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	حرف *filename = xdg_config_home_systemd(local_service_name);
	اگر (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	آزاد(filename);
	آزاد(local_service_name);
	واپس ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
ساکن صحیح systemd_timer_write_timer_file(شمار جدول_وقت_ترجیح جدول_وقت,
					  صحیح minute)
{
	صحیح res = -1;
	حرف *filename;
	FILE *file;
	ثابت حرف *unit;
	حرف *جدول_وقت_نمونہ = تہی;
	ثابت حرف *frequency = get_frequency(جدول_وقت);
	حرف *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	اگر (محفوظ_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		جاؤ error;
	}
	file = fopen_or_warn(filename, "w");
	اگر (!file)
		جاؤ error;

	انتخاب (جدول_وقت) {
	حالت جدول_وقت_HOURLY:
		جدول_وقت_نمونہ = xstrfmt("*-*-* 1..23:%02d:00", minute);
		توڑو;

	حالت جدول_وقت_DAILY:
		جدول_وقت_نمونہ = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		توڑو;

	حالت جدول_وقت_WEEKLY:
		جدول_وقت_نمونہ = xstrfmt("Mon 0:%02d:00", minute);
		توڑو;

	طےشدہ:
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
	اگر (fprintf(file, unit, جدول_وقت_نمونہ) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		جاؤ error;
	}
	اگر (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		جاؤ error;
	}

	res = 0;

error:
	آزاد(جدول_وقت_نمونہ);
	آزاد(local_timer_name);
	آزاد(filename);
	واپس res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
ساکن صحیح systemd_timer_write_service_template(ثابت حرف *exec_راستہ)
{
	صحیح res = -1;
	حرف *filename;
	FILE *file;
	ثابت حرف *unit;
	حرف *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	اگر (محفوظ_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		جاؤ error;
	}
	file = fopen_or_warn(filename, "w");
	اگر (!file)
		جاؤ error;

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
	اگر (fprintf(file, unit, exec_راستہ, exec_راستہ, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		جاؤ error;
	}
	اگر (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		جاؤ error;
	}

	res = 0;

error:
	آزاد(local_service_name);
	آزاد(filename);
	واپس res;
}

ساکن صحیح systemd_timer_enable_unit(صحیح enable,
				     شمار جدول_وقت_ترجیح جدول_وقت,
				     صحیح minute)
{
	حرف *cmd = تہی;
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;
	ثابت حرف *frequency = get_frequency(جدول_وقت);
	صحیح ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	اگر (!enable) {
		بچہ.no_stderr = 1;
	} ورنہ اگر (systemd_timer_write_timer_file(جدول_وقت, minute)) {
		ret = -1;
		جاؤ برآمد;
	}

	get_جدول_وقت_cmd("systemctl", تہی, &cmd);
	strvec_بانٹو(&بچہ.دلائل, cmd);
	strvec_pushl(&بچہ.دلائل, "--user", enable ? "enable" : "disable",
		     "--now", تہی);
	strvec_pushf(&بچہ.دلائل, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	اگر (آغاز_command(&بچہ)) {
		ret = error(_("failed to start systemctl"));
		جاؤ برآمد;
	}

	اگر (اختتام_command(&بچہ)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		اگر (enable) {
			ret = error(_("failed to run systemctl"));
			جاؤ برآمد;
		}
	}

	ret = 0;

برآمد:
	آزاد(cmd);
	واپس ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
ساکن خالی systemd_timer_delete_stale_timer_templates(خالی)
{
	حرف *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	حرف *filename = xdg_config_home_systemd(timer_template_name);

	اگر (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	آزاد(filename);
	آزاد(timer_template_name);
}

ساکن صحیح systemd_timer_delete_unit_files(خالی)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	واپس systemd_timer_delete_timer_file(جدول_وقت_HOURLY) |
	       systemd_timer_delete_timer_file(جدول_وقت_DAILY) |
	       systemd_timer_delete_timer_file(جدول_وقت_WEEKLY) |
	       systemd_timer_delete_service_template();
}

ساکن صحیح systemd_timer_delete_units(خالی)
{
	صحیح minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	واپس systemd_timer_enable_unit(0, جدول_وقت_HOURLY, minute) |
	       systemd_timer_enable_unit(0, جدول_وقت_DAILY, minute) |
	       systemd_timer_enable_unit(0, جدول_وقت_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

ساکن صحیح systemd_timer_تیاری_units(خالی)
{
	صحیح minute = get_random_minute();
	ثابت حرف *exec_راستہ = git_exec_راستہ();

	صحیح ret = systemd_timer_write_service_template(exec_راستہ) ||
		  systemd_timer_enable_unit(1, جدول_وقت_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, جدول_وقت_DAILY, minute) ||
		  systemd_timer_enable_unit(1, جدول_وقت_WEEKLY, minute);

	اگر (ret)
		systemd_timer_delete_units();
	ورنہ
		systemd_timer_delete_stale_timer_templates();

	واپس ret;
}

ساکن صحیح systemd_timer_update_جدول_وقت(صحیح تسلسل_maintenance, صحیح fd غیر_مستعمل)
{
	اگر (تسلسل_maintenance)
		واپس systemd_timer_تیاری_units();
	ورنہ
		واپس systemd_timer_delete_units();
}

شمار scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

ساکن ثابت ساخت {
	ثابت حرف *name;
	صحیح (*is_دستیاب)(خالی);
	صحیح (*update_جدول_وقت)(صحیح تسلسل_maintenance, صحیح fd);
} scheduler_fn[] = {
	[SCHEDULER_CRON] = {
		.name = "crontab",
		.is_دستیاب = is_crontab_دستیاب,
		.update_جدول_وقت = crontab_update_جدول_وقت,
	},
	[SCHEDULER_SYSTEMD] = {
		.name = "systemctl",
		.is_دستیاب = is_systemd_timer_دستیاب,
		.update_جدول_وقت = systemd_timer_update_جدول_وقت,
	},
	[SCHEDULER_LAUNCHCTL] = {
		.name = "launchctl",
		.is_دستیاب = is_launchctl_دستیاب,
		.update_جدول_وقت = launchctl_update_جدول_وقت,
	},
	[SCHEDULER_SCHTASKS] = {
		.name = "schtasks",
		.is_دستیاب = is_schtasks_دستیاب,
		.update_جدول_وقت = schtasks_update_جدول_وقت,
	},
};

ساکن شمار scheduler parse_scheduler(ثابت حرف *قدر)
{
	اگر (!قدر)
		واپس SCHEDULER_INVALID;
	ورنہ اگر (!strcasecmp(قدر, "auto"))
		واپس SCHEDULER_AUTO;
	ورنہ اگر (!strcasecmp(قدر, "cron") || !strcasecmp(قدر, "crontab"))
		واپس SCHEDULER_CRON;
	ورنہ اگر (!strcasecmp(قدر, "systemd") ||
		 !strcasecmp(قدر, "systemd-timer"))
		واپس SCHEDULER_SYSTEMD;
	ورنہ اگر (!strcasecmp(قدر, "launchctl"))
		واپس SCHEDULER_LAUNCHCTL;
	ورنہ اگر (!strcasecmp(قدر, "schtasks"))
		واپس SCHEDULER_SCHTASKS;
	ورنہ
		واپس SCHEDULER_INVALID;
}

ساکن صحیح maintenance_opt_scheduler(ثابت ساخت option *opt, ثابت حرف *arg,
				     صحیح unset)
{
	شمار scheduler *scheduler = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	اگر (*scheduler == SCHEDULER_INVALID)
		واپس error(_("unrecognized --scheduler argument '%s'"), arg);
	واپس 0;
}

ساخت maintenance_آغاز_opts {
	شمار scheduler scheduler;
};

ساکن شمار scheduler resolve_scheduler(شمار scheduler scheduler)
{
	اگر (scheduler != SCHEDULER_AUTO)
		واپس scheduler;

#اگر defined(__APPLE__)
	واپس SCHEDULER_LAUNCHCTL;

#وگرنہ_اگر defined(GIT_WINDOWS_NATIVE)
	واپس SCHEDULER_SCHTASKS;

#وگرنہ_اگر defined(__linux__)
	اگر (is_systemd_timer_دستیاب())
		واپس SCHEDULER_SYSTEMD;
	ورنہ اگر (is_crontab_دستیاب())
		واپس SCHEDULER_CRON;
	ورنہ
		die(_("neither systemd timers nor crontab are available"));

#ورنہ
	واپس SCHEDULER_CRON;
#اگر_ختم
}

ساکن خالی validate_scheduler(شمار scheduler scheduler)
{
	اگر (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	اگر (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	اگر (!scheduler_fn[scheduler].is_دستیاب())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

ساکن صحیح update_background_جدول_وقت(ثابت ساخت maintenance_آغاز_opts *opts,
				      صحیح enable)
{
	غیرعلامتی صحیح i;
	صحیح نتیجہ = 0;
	ساخت تالا_file lk;
	حرف *تالا_راستہ = xstrfmt("%s/schedule", the_repository->objects->ماخذ->راستہ);

	اگر (hold_تالا_file_for_update(&lk, تالا_راستہ, تالا_NO_DEREF) < 0) {
		اگر (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_راستہ(تالا_راستہ), strerror(errno));
		ورنہ
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		آزاد(تالا_راستہ);
		واپس -1;
	}

	برائے (i = 1; i < ARRAY_سائز(scheduler_fn); i++) {
		اگر (enable && opts->scheduler == i)
			جاری;
		اگر (!scheduler_fn[i].is_دستیاب())
			جاری;
		scheduler_fn[i].update_جدول_وقت(0, get_تالا_file_fd(&lk));
	}

	اگر (enable)
		نتیجہ = scheduler_fn[opts->scheduler].update_جدول_وقت(
			1, get_تالا_file_fd(&lk));

	rollback_تالا_file(&lk);

	آزاد(تالا_راستہ);
	واپس نتیجہ;
}

ساکن ثابت حرف *ثابت builtin_maintenance_آغاز_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	تہی
};

ساکن صحیح maintenance_آغاز(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			     ساخت repository *repo)
{
	ساخت maintenance_آغاز_opts opts = { 0 };
	ساخت option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_انتہا()
	};
	ثابت حرف *رجسٹر_دلائل[] = { "register", تہی };

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_maintenance_آغاز_usage, 0);
	اگر (argc)
		usage_with_options(builtin_maintenance_آغاز_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	اگر (update_background_جدول_وقت(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	اگر (maintenance_رجسٹر(ARRAY_سائز(رجسٹر_دلائل)-1, رجسٹر_دلائل, تہی, repo))
		warning(_("failed to add repo to global config"));
	واپس 0;
}

ساکن ثابت حرف *ثابت builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	تہی
};

ساکن صحیح maintenance_stop(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			    ساخت repository *repo غیر_مستعمل)
{
	ساخت option options[] = {
		OPT_انتہا()
	};
	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_maintenance_stop_usage, 0);
	اگر (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	واپس update_background_جدول_وقت(تہی, 0);
}

ساکن ثابت حرف *ثابت builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	تہی
};

ساکن صحیح maintenance_is_needed(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				 ساخت repository *repo غیر_مستعمل)
{
	ساخت maintenance_تسلسل_opts opts = MAINTENANCE_تسلسل_OPTS_INIT;
	ساخت string_list selected_tasks = STRING_LIST_INIT_DUP;
	ساخت gc_config cfg = GC_CONFIG_INIT;
	ساخت option options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			       N_("check a specific task"),
			       PARSE_OPT_NONEG, task_option_parse),
		OPT_انتہا()
	};
	bool is_needed = false;

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_maintenance_is_needed_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	اگر (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	اگر (opts.auto_flag) {
		برائے (سائز_t i = 0; i < opts.tasks_nr; i++) {
			اگر (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				توڑو;
			}
		}
	} ورنہ {
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
	maintenance_تسلسل_opts_release(&opts);
	gc_config_release(&cfg);

	اگر (is_needed)
		واپس 0;
	واپس 1;
}

ساکن ثابت حرف *ثابت builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	تہی,
};

صحیح cmd_maintenance(صحیح argc,
		    ثابت حرف **argv,
		    ثابت حرف *سابقہ,
		    ساخت repository *repo)
{
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option builtin_maintenance_options[] = {
		OPT_SUBCOMMAND("run", &fn, maintenance_تسلسل),
		OPT_SUBCOMMAND("start", &fn, maintenance_آغاز),
		OPT_SUBCOMMAND("stop", &fn, maintenance_stop),
		OPT_SUBCOMMAND("register", &fn, maintenance_رجسٹر),
		OPT_SUBCOMMAND("unregister", &fn, maintenance_unregister),
		OPT_SUBCOMMAND("is-needed", &fn, maintenance_is_needed),
		OPT_انتہا(),
	};

	argc = parse_options(argc, argv, سابقہ, builtin_maintenance_options,
			     builtin_maintenance_usage, 0);
	واپس fn(argc, argv, سابقہ, repo);
}
