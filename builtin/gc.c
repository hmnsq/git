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

#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

#وصف_ڪريو FAILED_لاڳيتو "failed to run %s"

ساكن ثابت حرف * ثابت builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	عدم
};

ساكن timestamp_t gc_log_expire_وقت;
ساكن بنية tempfile *pidfile;
ساكن بنية تالو_file log_تالو;
ساكن بنية string_list pack_garbage = STRING_LIST_INIT_DUP;

ساكن فراغ clean_pack_garbage(فراغ)
{
	صحيح i;
	لكل (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.شيون[i].string);
	string_list_clear(&pack_garbage, 0);
}

ساكن فراغ report_pack_garbage(بنانشان ڏٺل_بٽ, ثابت حرف *رستو)
{
	إذا (ڏٺل_بٽ == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, رستو);
}

ساكن فراغ process_log_file(فراغ)
{
	بنية stat st;
	إذا (fstat(get_تالو_file_fd(&log_تالو), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		صحيح saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_تالو_file_رستو(&log_تالو),
			strerror(saved_errno));
		fflush(stderr);
		commit_تالو_file(&log_تالو);
		errno = saved_errno;
	} وإلا إذا (st.st_سائيز) {
		/* There was some error recorded in the lock file */
		commit_تالو_file(&log_تالو);
	} وإلا {
		حرف *رستو = repo_git_رستو(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(رستو);
		rollback_تالو_file(&log_تالو);
		حرر(رستو);
	}
}

ساكن فراغ process_log_file_at_exit(فراغ)
{
	fflush(stderr);
	process_log_file();
}

ساكن صحيح gc_config_is_timestamp_never(ثابت حرف *var)
{
	ثابت حرف *قدر;
	timestamp_t expire;

	إذا (!repo_config_get_قدر(the_repository, var, &قدر) && قدر) {
		إذا (parse_expiry_date(قدر, &expire))
			die(_("failed to parse '%s' value '%s'"), var, قدر);
		أرجع expire == 0;
	}
	أرجع 0;
}

بنية gc_config {
	صحيح pack_refs;
	صحيح prune_reflogs;
	صحيح cruft_packs;
	بنانشان ڊگهو max_cruft_سائيز;
	صحيح aggressive_اونهائي;
	صحيح aggressive_دري;
	صحيح detach_auto;
	حرف *gc_log_expire;
	حرف *prune_expire;
	حرف *prune_worktrees_expire;
	حرف *repack_expire_منزل;
};

#وصف_ڪريو GC_CONFIG_INIT { \
	.pack_refs = 1, \
	.prune_reflogs = 1, \
	.cruft_packs = 1, \
	.aggressive_اونهائي = 50, \
	.aggressive_دري = 250, \
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
	ثابت حرف *قدر;
	حرف *owned = عدم;

	إذا (!repo_config_get_قدر(the_repository, "gc.packrefs", &قدر)) {
		إذا (قدر && !strcmp(قدر, "notbare"))
			cfg->pack_refs = -1;
		وإلا
			cfg->pack_refs = git_config_bool("gc.packrefs", قدر);
	}

	إذا (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_دري);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_اونهائي);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_سائيز);

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

تعداد وقت_جدول_ترجيح {
	وقت_جدول_NONE = 0,
	وقت_جدول_WEEKLY = 1,
	وقت_جدول_DAILY = 2,
	وقت_جدول_HOURLY = 3,
};

ساكن تعداد وقت_جدول_ترجيح parse_وقت_جدول(ثابت حرف *قدر)
{
	إذا (!قدر)
		أرجع وقت_جدول_NONE;
	إذا (!strcasecmp(قدر, "hourly"))
		أرجع وقت_جدول_HOURLY;
	إذا (!strcasecmp(قدر, "daily"))
		أرجع وقت_جدول_DAILY;
	إذا (!strcasecmp(قدر, "weekly"))
		أرجع وقت_جدول_WEEKLY;
	أرجع وقت_جدول_NONE;
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
	TASK_ڳڻپ
};

بنية maintenance_لاڳيتو_opts {
	تعداد maintenance_task_label *tasks;
	سائيز_t tasks_nr, tasks_alloc;
	صحيح auto_flag;
	صحيح detach;
	صحيح quiet;
	تعداد وقت_جدول_ترجيح وقت_جدول;
};
#وصف_ڪريو MAINTENANCE_لاڳيتو_OPTS_INIT { \
	.detach = -1, \
}

ساكن فراغ maintenance_لاڳيتو_opts_release(بنية maintenance_لاڳيتو_opts *opts)
{
	حرر(opts->tasks);
}

ساكن صحيح pack_refs_condition(اڻ_استعمال_ٿيل بنية gc_config *cfg)
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

ساكن صحيح maintenance_task_pack_refs(بنية maintenance_لاڳيتو_opts *opts,
				      اڻ_استعمال_ٿيل بنية gc_config *cfg)
{
	بنية ٻار_process cmd = ٻار_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.دليلون, "pack-refs", "--all", "--prune", عدم);
	إذا (opts->auto_flag)
		strvec_ڌڪو(&cmd.دليلون, "--auto");

	أرجع لاڳيتو_command(&cmd);
}

بنية ڳڻپ_reflog_entries_معلومات {
	بنية expire_reflog_policy_cb policy;
	سائيز_t ڳڻپ;
	سائيز_t حد_بندي;
};

ساكن صحيح ڳڻپ_reflog_entries(ثابت حرف *refname اڻ_استعمال_ٿيل,
				بنية object_id *old_oid, بنية object_id *new_oid,
				ثابت حرف *committer, timestamp_t timestamp,
				صحيح tz, ثابت حرف *نياپو, فراغ *cb_معلومات)
{
	بنية ڳڻپ_reflog_entries_معلومات *معلومات = cb_معلومات;
	إذا (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, نياپو, &معلومات->policy))
		معلومات->ڳڻپ++;
	أرجع معلومات->ڳڻپ >= معلومات->حد_بندي;
}

ساكن صحيح reflog_expire_condition(بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	timestamp_t now = وقت(عدم);
	بنية ڳڻپ_reflog_entries_معلومات معلومات = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	صحيح حد_بندي = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &حد_بندي);
	إذا (!حد_بندي)
		أرجع 0;
	إذا (حد_بندي < 0)
		أرجع 1;
	معلومات.حد_بندي = حد_بندي;

	repo_config(the_repository, reflog_expire_config, &معلومات.policy.opts);

	reflog_expire_options_سيٽ_refname(&معلومات.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 ڳڻپ_reflog_entries, &معلومات);

	reflog_expiry_cleanup(&معلومات.policy);
	reflog_clear_expire_config(&معلومات.policy.opts);
	أرجع معلومات.ڳڻپ >= معلومات.حد_بندي;
}

ساكن صحيح maintenance_task_reflog_expire(بنية maintenance_لاڳيتو_opts *opts اڻ_استعمال_ٿيل,
					  بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	بنية ٻار_process cmd = ٻار_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.دليلون, "reflog", "expire", "--all", عدم);
	أرجع لاڳيتو_command(&cmd);
}

ساكن صحيح maintenance_task_worktree_prune(بنية maintenance_لاڳيتو_opts *opts اڻ_استعمال_ٿيل,
					   بنية gc_config *cfg)
{
	بنية ٻار_process prune_worktrees_cmd = ٻار_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.دليلون, "worktree", "prune", "--expire", عدم);
	strvec_ڌڪو(&prune_worktrees_cmd.دليلون, cfg->prune_worktrees_expire);

	أرجع لاڳيتو_command(&prune_worktrees_cmd);
}

ساكن صحيح worktree_prune_condition(بنية gc_config *cfg)
{
	بنية strbuf buf = STRBUF_INIT;
	صحيح should_prune = 0, حد_بندي = 1;
	timestamp_t expiry_date;
	بنية dirent *d;
	DIR *dir = عدم;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &حد_بندي);
	إذا (حد_بندي <= 0) {
		should_prune = حد_بندي < 0;
		اذهب ٻاهريون;
	}

	إذا (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		اذهب ٻاهريون;

	dir = opendir(repo_git_رستو_replace(the_repository, &buf, "worktrees"));
	إذا (!dir)
		اذهب ٻاهريون;

	طالما (حد_بندي && (d = readdir_skip_dot_and_dotdot(dir))) {
		حرف *wtpath;
		strbuf_reset(&buf);
		إذا (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			حد_بندي--;
		حرر(wtpath);
	}

	should_prune = !حد_بندي;

ٻاهريون:
	إذا (dir)
		closedir(dir);
	strbuf_release(&buf);
	أرجع should_prune;
}

ساكن صحيح maintenance_task_rerere_gc(بنية maintenance_لاڳيتو_opts *opts اڻ_استعمال_ٿيل,
				      بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	بنية ٻار_process rerere_cmd = ٻار_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.دليلون, "rerere", "gc", عدم);
	أرجع لاڳيتو_command(&rerere_cmd);
}

ساكن صحيح rerere_gc_condition(بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	صحيح حد_بندي = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &حد_بندي);
	إذا (!حد_بندي)
		أرجع 0; /* never prune */
	إذا (حد_بندي < 0)
		أرجع 1; /* always prune */

	أرجع rerere_gc_needed(the_repository, (سائيز_t)حد_بندي);
}

#وصف_ڪريو OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_منزل = (cfg)->repack_expire_منزل, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_سائيز = (cfg)->max_cruft_سائيز, \
	.دري = (aggressive) ? (cfg)->aggressive_دري : 0, \
	.اونهائي = (aggressive) ? (cfg)->aggressive_اونهائي : 0

/* return NULL on success, else hostname running the gc */
ساكن ثابت حرف *تالو_repo_for_gc(صحيح force, pid_t* ret_pid)
{
	بنية تالو_file تالو = تالو_INIT;
	حرف my_host[HOST_NAME_MAX + 1];
	بنية strbuf sb = STRBUF_INIT;
	بنية stat st;
	uintmax_t pid;
	FILE *fp;
	صحيح fd;
	حرف *pidfile_رستو;

	إذا (is_tempfile_سرگرم(pidfile))
		/* already locked */
		أرجع عدم;

	إذا (xgethostname(my_host, حجم(my_host)))
		xsnprintf(my_host, حجم(my_host), "unknown");

	pidfile_رستو = repo_git_رستو(the_repository, "gc.pid");
	fd = hold_تالو_file_for_update(&تالو, pidfile_رستو,
				       تالو_DIE_ON_ERROR);
	إذا (!force) {
		ساكن حرف locking_host[HOST_NAME_MAX + 1];
		ساكن حرف *جاچ_fmt;
		صحيح should_exit;

		إذا (!جاچ_fmt)
			جاچ_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_رستو, "r");
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
			وقت(عدم) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, جاچ_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		إذا (fp)
			fclose(fp);
		إذا (should_exit) {
			إذا (fd >= 0)
				rollback_تالو_file(&تالو);
			*ret_pid = pid;
			حرر(pidfile_رستو);
			أرجع locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_اندريون_ڀريل(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_تالو_file(&تالو);
	pidfile = رجسٽر_tempfile(pidfile_رستو);
	حرر(pidfile_رستو);
	أرجع عدم;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
ساكن صحيح report_آخري_gc_error(فراغ)
{
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret = 0;
	ssize_t len;
	بنية stat st;
	حرف *gc_log_رستو = repo_git_رستو(the_repository, "gc.log");

	إذا (stat(gc_log_رستو, &st)) {
		إذا (errno == ENOENT)
			اذهب پورو;

		ret = die_پيغام_errno(_("cannot stat '%s'"), gc_log_رستو);
		اذهب پورو;
	}

	إذا (st.st_mtime < gc_log_expire_وقت)
		اذهب پورو;

	len = strbuf_read_file(&sb, gc_log_رستو, 0);
	إذا (len < 0)
		ret = die_پيغام_errno(_("cannot read '%s'"), gc_log_رستو);
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
			    gc_log_رستو, sb.buf);
		ret = 1;
	}
	strbuf_release(&sb);
پورو:
	حرر(gc_log_رستو);
	أرجع ret;
}

ساكن صحيح gc_foreground_tasks(بنية maintenance_لاڳيتو_opts *opts,
			       بنية gc_config *cfg)
{
	إذا (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		أرجع error(FAILED_لاڳيتو, "pack-refs");
	إذا (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		أرجع error(FAILED_لاڳيتو, "reflog");
	أرجع 0;
}

ساكن صحيح maintenance_task_odb(بنية maintenance_لاڳيتو_opts *opts,
				بنية gc_config *cfg,
				صحيح keep_سڀ_کان_وڏو_pack,
				صحيح aggressive)
{
	بنية odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_سڀ_کان_وڏو_pack = keep_سڀ_کان_وڏو_pack,
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
	   ثابت حرف *اڳياڙي,
	   بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح aggressive = 0;
	صحيح force = 0;
	ثابت حرف *name;
	pid_t pid;
	صحيح daemonized = 0;
	صحيح keep_سڀ_کان_وڏو_pack = -1;
	صحيح skip_foreground_tasks = 0;
	timestamp_t dummy;
	بنية maintenance_لاڳيتو_opts opts = MAINTENANCE_لاڳيتو_OPTS_INIT;
	بنية gc_config cfg = GC_CONFIG_INIT;
	ثابت حرف *prune_expire_sentinel = "sentinel";
	ثابت حرف *prune_expire_arg = prune_expire_sentinel;
	صحيح ret;
	بنية option builtin_gc_options[] = {
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
		OPT_UNSIGNED(0, "max-cruft-size", &cfg.max_cruft_سائيز,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL(0, "aggressive", &aggressive, N_("be more thorough (increased runtime)")),
		OPT_BOOL_F(0, "auto", &opts.auto_flag, N_("enable auto-gc mode"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform garbage collection in the background")),
		OPT_BOOL_F(0, "force", &force,
			   N_("force running gc even if there may be another gc running"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "keep-largest-pack", &keep_سڀ_کان_وڏو_pack,
			 N_("repack all other packs except the largest pack")),
		OPT_STRING(0, "expire-to", &cfg.repack_expire_منزل, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_HIDDEN_BOOL(0, "skip-foreground-tasks", &skip_foreground_tasks,
			   N_("skip maintenance tasks typically done in the foreground")),
		OPT_پڇاڙي()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_gc_usage, builtin_gc_options);

	gc_config(&cfg);

	إذا (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_وقت))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	إذا (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, اڳياڙي, builtin_gc_options,
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
		    لاڳيتو_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			اذهب ٻاهريون;
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
		ret = report_آخري_gc_error();
		إذا (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			اذهب ٻاهريون;

		} وإلا إذا (ret) {
			/* an I/O error occurred, already reported */
			اذهب ٻاهريون;
		}

		إذا (!skip_foreground_tasks) {
			إذا (تالو_repo_for_gc(force, &pid)) {
				ret = 0;
				اذهب ٻاهريون;
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

	name = تالو_repo_for_gc(force, &pid);
	إذا (name) {
		إذا (opts.auto_flag) {
			ret = 0;
			اذهب ٻاهريون; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	إذا (daemonized) {
		حرف *رستو = repo_git_رستو(the_repository, "gc.log");
		hold_تالو_file_for_update(&log_تالو, رستو,
					  تالو_DIE_ON_ERROR);
		dup2(get_تالو_file_fd(&log_تالو), 2);
		atexit(process_log_file_at_exit);
		حرر(رستو);
	}

	إذا (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	إذا (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_لاڳيتو, "worktree");

	إذا (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_لاڳيتو, "rerere");

	إذا (maintenance_task_odb(&opts, &cfg, keep_سڀ_کان_وڏو_pack, aggressive))
		die(عدم);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	إذا (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	إذا (the_repository->settings.gc_write_commit_گراف == 1)
		write_commit_گراف_reachable(the_repository,
					     the_repository->objects->ذريعو->رستو,
					     !opts.quiet && !daemonized ? COMMIT_گراف_WRITE_ترقي : 0,
					     عدم);

	إذا (!daemonized) {
		حرف *رستو = repo_git_رستو(the_repository, "gc.log");
		unlink(رستو);
		حرر(رستو);
	}

ٻاهريون:
	maintenance_لاڳيتو_opts_release(&opts);
	gc_config_release(&cfg);
	أرجع 0;
}

ساكن ثابت حرف *ثابت builtin_maintenance_لاڳيتو_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	عدم
};

ساكن صحيح maintenance_opt_وقت_جدول(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	تعداد وقت_جدول_ترجيح *ترجيح = opt->قدر;

	إذا (unset)
		die(_("--no-schedule is not allowed"));

	*ترجيح = parse_وقت_جدول(arg);

	إذا (!*ترجيح)
		die(_("unrecognized --schedule argument '%s'"), arg);

	أرجع 0;
}

بنية cg_auto_معلومات {
	صحيح انگ_not_اندريون_گراف;
	صحيح حد_بندي;
};

ساكن صحيح dfs_on_ref(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية cg_auto_معلومات *معلومات = (بنية cg_auto_معلومات *)cb_معلومات;
	صحيح نتيجو = 0;
	ثابت بنية object_id *maybe_peeled = ref->oid;
	بنية object_id peeled;
	بنية commit_list *ڍير = عدم;
	بنية commit *commit;

	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	إذا (odb_read_object_info(the_repository->objects, maybe_peeled, عدم) != OBJ_COMMIT)
		أرجع 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	إذا (!commit || commit->object.flags & ڏٺل_2)
		أرجع 0;
	commit->object.flags |= ڏٺل_2;

	إذا (repo_parse_commit(the_repository, commit) ||
	    commit_گراف_position(commit) != COMMIT_NOT_FROM_گراف)
		أرجع 0;

	معلومات->انگ_not_اندريون_گراف++;

	إذا (معلومات->انگ_not_اندريون_گراف >= معلومات->حد_بندي)
		أرجع 1;

	commit_list_وجهو(commit, &ڍير);

	طالما (!نتيجو && ڍير) {
		بنية commit_list *والد;

		commit = ڪڍو_commit(&ڍير);

		لكل (والد = commit->parents; والد; والد = والد->ايندڙ) {
			إذا (repo_parse_commit(the_repository, والد->شي) ||
			    commit_گراف_position(والد->شي) != COMMIT_NOT_FROM_گراف ||
			    والد->شي->object.flags & ڏٺل_2)
				استمر;

			والد->شي->object.flags |= ڏٺل_2;
			معلومات->انگ_not_اندريون_گراف++;

			إذا (معلومات->انگ_not_اندريون_گراف >= معلومات->حد_بندي) {
				نتيجو = 1;
				اكسر;
			}

			commit_list_وجهو(والد->شي, &ڍير);
		}
	}

	commit_list_free(ڍير);
	أرجع نتيجو;
}

ساكن صحيح should_write_commit_گراف(بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	صحيح نتيجو;
	بنية cg_auto_معلومات معلومات;

	معلومات.انگ_not_اندريون_گراف = 0;
	معلومات.حد_بندي = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &معلومات.حد_بندي);

	إذا (!معلومات.حد_بندي)
		أرجع 0;
	إذا (معلومات.حد_بندي < 0)
		أرجع 1;

	نتيجو = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &معلومات);

	repo_clear_commit_marks(the_repository, ڏٺل_2);

	أرجع نتيجو;
}

ساكن صحيح لاڳيتو_write_commit_گراف(بنية maintenance_لاڳيتو_opts *opts)
{
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;

	ٻار.git_cmd = 1;
	ٻار.odb_منزل_close = the_repository->objects;
	strvec_pushl(&ٻار.دليلون, "commit-graph", "write",
		     "--split", "--reachable", عدم);

	إذا (opts->quiet)
		strvec_ڌڪو(&ٻار.دليلون, "--no-progress");
	وإلا
		strvec_ڌڪو(&ٻار.دليلون, "--progress");

	أرجع !!لاڳيتو_command(&ٻار);
}

ساكن صحيح maintenance_task_commit_گراف(بنية maintenance_لاڳيتو_opts *opts,
					 بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	prepare_repo_settings(the_repository);
	إذا (!the_repository->settings.core_commit_گراف)
		أرجع 0;

	إذا (لاڳيتو_write_commit_گراف(opts)) {
		error(_("failed to write commit-graph"));
		أرجع 1;
	}

	أرجع 0;
}

ساكن صحيح fetch_remote(بنية remote *remote, فراغ *cbdata)
{
	بنية maintenance_لاڳيتو_opts *opts = cbdata;
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;

	إذا (remote->skip_default_update)
		أرجع 0;

	ٻار.git_cmd = 1;
	strvec_pushl(&ٻار.دليلون, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     عدم);

	إذا (opts->quiet)
		strvec_ڌڪو(&ٻار.دليلون, "--quiet");

	أرجع !!لاڳيتو_command(&ٻار);
}

ساكن صحيح maintenance_task_prefetch(بنية maintenance_لاڳيتو_opts *opts,
				     بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	إذا (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		أرجع 1;
	}

	أرجع 0;
}

ساكن صحيح maintenance_task_gc_foreground(بنية maintenance_لاڳيتو_opts *opts,
					  بنية gc_config *cfg)
{
	أرجع gc_foreground_tasks(opts, cfg);
}

ساكن صحيح maintenance_task_gc_background(بنية maintenance_لاڳيتو_opts *opts,
					  بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;

	ٻار.git_cmd = 1;
	ٻار.odb_منزل_close = the_repository->objects;
	strvec_ڌڪو(&ٻار.دليلون, "gc");

	إذا (opts->auto_flag)
		strvec_ڌڪو(&ٻار.دليلون, "--auto");
	إذا (opts->quiet)
		strvec_ڌڪو(&ٻار.دليلون, "--quiet");
	وإلا
		strvec_ڌڪو(&ٻار.دليلون, "--no-quiet");
	strvec_ڌڪو(&ٻار.دليلون, "--no-detach");
	strvec_ڌڪو(&ٻار.دليلون, "--skip-foreground-tasks");

	أرجع لاڳيتو_command(&ٻار);
}

ساكن صحيح gc_condition(بنية gc_config *cfg)
{
	بنية odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	أرجع odb_optimize_required(the_repository->objects, &opts);
}

ساكن صحيح prune_packed(بنية maintenance_لاڳيتو_opts *opts)
{
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;

	ٻار.git_cmd = 1;
	strvec_ڌڪو(&ٻار.دليلون, "prune-packed");

	إذا (opts->quiet)
		strvec_ڌڪو(&ٻار.دليلون, "--quiet");

	أرجع !!لاڳيتو_command(&ٻار);
}

بنية write_loose_object_معلومات {
	FILE *اندريون;
	صحيح ڳڻپ;
	صحيح batch_سائيز;
};

ساكن صحيح loose_object_auto_حد_بندي = 100;

ساكن صحيح loose_object_ڳڻپ(ثابت بنية object_id *oid اڻ_استعمال_ٿيل,
			      ثابت حرف *رستو اڻ_استعمال_ٿيل,
			      فراغ *معلومات)
{
	صحيح *ڳڻپ = (صحيح*)معلومات;
	إذا (++(*ڳڻپ) >= loose_object_auto_حد_بندي)
		أرجع 1;
	أرجع 0;
}

ساكن صحيح loose_object_auto_condition(بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	صحيح ڳڻپ = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_حد_بندي);

	إذا (!loose_object_auto_حد_بندي)
		أرجع 0;
	إذا (loose_object_auto_حد_بندي < 0)
		أرجع 1;

	أرجع for_each_loose_file_اندريون_ذريعو(the_repository->objects->ذريعو,
					     loose_object_ڳڻپ,
					     عدم, عدم, &ڳڻپ);
}

ساكن صحيح bail_on_loose(ثابت بنية object_id *oid اڻ_استعمال_ٿيل,
			 ثابت حرف *رستو اڻ_استعمال_ٿيل,
			 فراغ *معلومات اڻ_استعمال_ٿيل)
{
	أرجع 1;
}

ساكن صحيح write_loose_object_منزل_stdin(ثابت بنية object_id *oid,
				       ثابت حرف *رستو اڻ_استعمال_ٿيل,
				       فراغ *معلومات)
{
	بنية write_loose_object_معلومات *d = (بنية write_loose_object_معلومات *)معلومات;

	fprintf(d->اندريون, "%s\n", oid_منزل_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	أرجع ++(d->ڳڻپ) > d->batch_سائيز;
}

ساكن صحيح pack_loose(بنية maintenance_لاڳيتو_opts *opts)
{
	بنية repository *r = the_repository;
	صحيح نتيجو = 0;
	بنية write_loose_object_معلومات معلومات;
	بنية ٻار_process pack_proc = ٻار_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	إذا (!for_each_loose_file_اندريون_ذريعو(r->objects->ذريعو,
					   bail_on_loose,
					   عدم, عدم, عدم))
		أرجع 0;

	pack_proc.git_cmd = 1;

	strvec_ڌڪو(&pack_proc.دليلون, "pack-objects");
	إذا (opts->quiet)
		strvec_ڌڪو(&pack_proc.دليلون, "--quiet");
	وإلا
		strvec_ڌڪو(&pack_proc.دليلون, "--no-quiet");
	strvec_pushf(&pack_proc.دليلون, "%s/pack/loose", r->objects->ذريعو->رستو);

	pack_proc.اندريون = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.ٻاهريون = -1;

	إذا (شروعات_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		أرجع 1;
	}

	معلومات.اندريون = xfdopen(pack_proc.اندريون, "w");
	معلومات.ڳڻپ = 0;
	معلومات.batch_سائيز = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &معلومات.batch_سائيز);

	/* If configured as 0, then remove limit. */
	إذا (!معلومات.batch_سائيز)
		معلومات.batch_سائيز = وڌ_کان_وڌ_عدد;
	وإلا إذا (معلومات.batch_سائيز > 0)
		معلومات.batch_سائيز--; /* Decrease for equality on limit. */

	for_each_loose_file_اندريون_ذريعو(r->objects->ذريعو,
				      write_loose_object_منزل_stdin,
				      عدم, عدم, &معلومات);

	fclose(معلومات.اندريون);

	إذا (پڄاڻي_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		نتيجو = 1;
	}

	أرجع نتيجو;
}

ساكن صحيح maintenance_task_loose_objects(بنية maintenance_لاڳيتو_opts *opts,
					  بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	أرجع prune_packed(opts) || pack_loose(opts);
}

ساكن صحيح incremental_repack_auto_condition(بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	بنية packed_git *p;
	صحيح incremental_repack_auto_حد_بندي = 10;
	صحيح ڳڻپ = 0;

	prepare_repo_settings(the_repository);
	إذا (!the_repository->settings.core_multi_pack_اشاريو)
		أرجع 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_حد_بندي);

	إذا (!incremental_repack_auto_حد_بندي)
		أرجع 0;
	إذا (incremental_repack_auto_حد_بندي < 0)
		أرجع 1;

	repo_for_each_pack(the_repository, p) {
		إذا (ڳڻپ >= incremental_repack_auto_حد_بندي)
			اكسر;
		إذا (!p->multi_pack_اشاريو)
			ڳڻپ++;
	}

	أرجع ڳڻپ >= incremental_repack_auto_حد_بندي;
}

ساكن صحيح multi_pack_اشاريو_write(بنية maintenance_لاڳيتو_opts *opts)
{
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;

	ٻار.git_cmd = 1;
	strvec_pushl(&ٻار.دليلون, "multi-pack-index", "write", عدم);

	إذا (opts->quiet)
		strvec_ڌڪو(&ٻار.دليلون, "--no-progress");
	وإلا
		strvec_ڌڪو(&ٻار.دليلون, "--progress");

	إذا (لاڳيتو_command(&ٻار))
		أرجع error(_("failed to write multi-pack-index"));

	أرجع 0;
}

ساكن صحيح multi_pack_اشاريو_expire(بنية maintenance_لاڳيتو_opts *opts)
{
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;

	ٻار.git_cmd = 1;
	ٻار.odb_منزل_close = the_repository->objects;
	strvec_pushl(&ٻار.دليلون, "multi-pack-index", "expire", عدم);

	إذا (opts->quiet)
		strvec_ڌڪو(&ٻار.دليلون, "--no-progress");
	وإلا
		strvec_ڌڪو(&ٻار.دليلون, "--progress");

	إذا (لاڳيتو_command(&ٻار))
		أرجع error(_("'git multi-pack-index expire' failed"));

	أرجع 0;
}

#وصف_ڪريو TWO_GIGABYTES (INT32_MAX)

ساكن off_t get_auto_pack_سائيز(فراغ)
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
	off_t max_سائيز = 0;
	off_t ٻيو_سڀ_کان_وڏو_سائيز = 0;
	off_t نتيجو_سائيز;
	بنية packed_git *p;
	بنية repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		إذا (p->pack_سائيز > max_سائيز) {
			ٻيو_سڀ_کان_وڏو_سائيز = max_سائيز;
			max_سائيز = p->pack_سائيز;
		} وإلا إذا (p->pack_سائيز > ٻيو_سڀ_کان_وڏو_سائيز)
			ٻيو_سڀ_کان_وڏو_سائيز = p->pack_سائيز;
	}

	نتيجو_سائيز = ٻيو_سڀ_کان_وڏو_سائيز + 1;

	/* But limit ourselves to a batch size of 2g */
	إذا (نتيجو_سائيز > TWO_GIGABYTES)
		نتيجو_سائيز = TWO_GIGABYTES;

	أرجع نتيجو_سائيز;
}

ساكن صحيح multi_pack_اشاريو_repack(بنية maintenance_لاڳيتو_opts *opts)
{
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;

	ٻار.git_cmd = 1;
	ٻار.odb_منزل_close = the_repository->objects;
	strvec_pushl(&ٻار.دليلون, "multi-pack-index", "repack", عدم);

	إذا (opts->quiet)
		strvec_ڌڪو(&ٻار.دليلون, "--no-progress");
	وإلا
		strvec_ڌڪو(&ٻار.دليلون, "--progress");

	strvec_pushf(&ٻار.دليلون, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_سائيز());

	إذا (لاڳيتو_command(&ٻار))
		أرجع error(_("'git multi-pack-index repack' failed"));

	أرجع 0;
}

ساكن صحيح maintenance_task_incremental_repack(بنية maintenance_لاڳيتو_opts *opts,
					       بنية gc_config *cfg اڻ_استعمال_ٿيل)
{
	prepare_repo_settings(the_repository);
	إذا (!the_repository->settings.core_multi_pack_اشاريو) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		أرجع 0;
	}

	إذا (multi_pack_اشاريو_write(opts))
		أرجع 1;
	إذا (multi_pack_اشاريو_expire(opts))
		أرجع 1;
	إذا (multi_pack_اشاريو_repack(opts))
		أرجع 1;
	أرجع 0;
}

ساكن صحيح maintenance_task_geometric_repack(بنية maintenance_لاڳيتو_opts *opts,
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

تعريفنوع صحيح (*maintenance_task_fn)(بنية maintenance_لاڳيتو_opts *opts,
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

تعداد auto_gc_hook_نتيجو {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_لاڳيتو = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

ساكن صحيح maybe_لاڳيتو_task(ثابت بنية maintenance_task *task,
			  بنية repository *repo,
			  بنية maintenance_لاڳيتو_opts *opts,
			  بنية gc_config *cfg,
			  تعداد task_phase phase,
			  تعداد auto_gc_hook_نتيجو *auto_gc_hook_نتيجو)
{
	صحيح foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ثابت حرف *region = foreground ? "maintenance foreground" : "maintenance";
	صحيح ret = 0;

	إذا (!fn)
		أرجع 0;
	إذا (opts->auto_flag) {
		إذا (*auto_gc_hook_نتيجو == AUTO_GC_HOOK_SKIP)
			أرجع 0;

		إذا (!task->auto_condition || !task->auto_condition(cfg))
			أرجع 0;

		إذا (*auto_gc_hook_نتيجو == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_نتيجو = لاڳيتو_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_لاڳيتو;
		إذا (*auto_gc_hook_نتيجو == AUTO_GC_HOOK_SKIP)
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

ساكن صحيح maintenance_لاڳيتو_tasks(بنية maintenance_لاڳيتو_opts *opts,
				 بنية gc_config *cfg)
{
	صحيح نتيجو = 0;
	بنية تالو_file lk;
	بنية repository *r = the_repository;
	حرف *تالو_رستو = xstrfmt("%s/maintenance", r->objects->ذريعو->رستو);
	تعداد auto_gc_hook_نتيجو auto_gc_hook_نتيجو = AUTO_GC_HOOK_UNDECIDED;

	إذا (repo_hold_تالو_file_for_update(r, &lk, تالو_رستو, تالو_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		إذا (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				تالو_رستو);
		حرر(تالو_رستو);
		أرجع 0;
	}
	حرر(تالو_رستو);

	لكل (سائيز_t i = 0; i < opts->tasks_nr; i++)
		إذا (maybe_لاڳيتو_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_نتيجو))
			نتيجو = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	إذا (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	لكل (سائيز_t i = 0; i < opts->tasks_nr; i++)
		إذا (maybe_لاڳيتو_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_نتيجو))
			نتيجو = 1;

	rollback_تالو_file(&lk);
	أرجع نتيجو;
}

تعداد maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

بنية maintenance_strategy {
	بنية {
		بنانشان type;
		تعداد وقت_جدول_ترجيح وقت_جدول;
	} tasks[TASK_ڳڻپ];
};

ساكن ثابت بنية maintenance_strategy none_strategy = { 0 };

ساكن ثابت بنية maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.وقت_جدول = وقت_جدول_DAILY,
		},
	},
};

ساكن ثابت بنية maintenance_strategy incremental_strategy = {
	.tasks = {
		[TASK_COMMIT_گراف] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.وقت_جدول = وقت_جدول_HOURLY,
		},
		[TASK_PREFETCH] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.وقت_جدول = وقت_جدول_HOURLY,
		},
		[TASK_INCREMENTAL_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.وقت_جدول = وقت_جدول_DAILY,
		},
		[TASK_LOOSE_OBJECTS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.وقت_جدول = وقت_جدول_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.وقت_جدول = وقت_جدول_WEEKLY,
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
			.وقت_جدول = وقت_جدول_HOURLY,
		},
		[TASK_GEOMETRIC_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.وقت_جدول = وقت_جدول_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.وقت_جدول = وقت_جدول_DAILY,
		},
		[TASK_RERERE_GC] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.وقت_جدول = وقت_جدول_WEEKLY,
		},
		[TASK_REFLOG_EXPIRE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.وقت_جدول = وقت_جدول_WEEKLY,
		},
		[TASK_WORKTREE_PRUNE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.وقت_جدول = وقت_جدول_WEEKLY,
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

ساكن فراغ initialize_task_config(بنية maintenance_لاڳيتو_opts *opts,
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
		لكل (سائيز_t i = 0; i < selected_tasks->nr; i++) {
			تعداد maintenance_task_label label = (intptr_t)selected_tasks->شيون[i].util;;
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
	إذا (opts->وقت_جدول) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} وإلا {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	إذا (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	لكل (سائيز_t i = 0; i < TASK_ڳڻپ; i++) {
		صحيح config_قدر;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		إذا (!repo_config_get_bool(the_repository, config_name.buf, &config_قدر))
			strategy.tasks[i].type = config_قدر ? type : 0;
		إذا (!(strategy.tasks[i].type & type))
			استمر;

		إذا (opts->وقت_جدول) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			إذا (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].وقت_جدول = parse_وقت_جدول(config_str);
			إذا (strategy.tasks[i].وقت_جدول < opts->وقت_جدول)
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
	بنية string_list *selected_tasks = opt->قدر;
	سائيز_t i;

	BUG_ON_OPT_NEG(unset);

	لكل (i = 0; i < TASK_ڳڻپ; i++)
		إذا (!strcasecmp(tasks[i].name, arg))
			اكسر;
	إذا (i >= TASK_ڳڻپ) {
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

ساكن صحيح maintenance_لاڳيتو(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			   بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية maintenance_لاڳيتو_opts opts = MAINTENANCE_لاڳيتو_OPTS_INIT;
	بنية string_list selected_tasks = STRING_LIST_INIT_DUP;
	بنية gc_config cfg = GC_CONFIG_INIT;
	بنية option builtin_maintenance_لاڳيتو_options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform maintenance in the background")),
		OPT_CALLBACK(0, "schedule", &opts.وقت_جدول, N_("frequency"),
			     N_("run tasks based on frequency"),
			     maintenance_opt_وقت_جدول),
		OPT_BOOL(0, "quiet", &opts.quiet,
			 N_("do not report progress or other information over stderr")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			N_("run a specific task"),
			PARSE_OPT_NONEG, task_option_parse),
		OPT_پڇاڙي()
	};
	صحيح ret;

	opts.quiet = !isatty(2);

	argc = parse_options(argc, argv, اڳياڙي,
			     builtin_maintenance_لاڳيتو_options,
			     builtin_maintenance_لاڳيتو_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	die_for_incompatible_opt2(opts.auto_flag, "--auto",
				  opts.وقت_جدول, "--schedule=");
	die_for_incompatible_opt2(selected_tasks.nr, "--task=",
				  opts.وقت_جدول, "--schedule=");

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	إذا (argc != 0)
		usage_with_options(builtin_maintenance_لاڳيتو_usage,
				   builtin_maintenance_لاڳيتو_options);

	ret = maintenance_لاڳيتو_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_لاڳيتو_opts_release(&opts);
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

ساكن حرف ثابت * ثابت builtin_maintenance_رجسٽر_usage[] = {
	"git maintenance register [--config-file <path>]",
	عدم
};

ساكن صحيح maintenance_رجسٽر(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
				بنية repository *repo اڻ_استعمال_ٿيل)
{
	حرف *config_file = عدم;
	بنية option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_پڇاڙي(),
	};
	صحيح مليو_2 = 0;
	ثابت حرف *ڪنجي = "maintenance.repo";
	حرف *maintpath = get_maintpath();
	بنية string_list_شي *شي;
	ثابت بنية string_list *list;

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_maintenance_رجسٽر_usage, 0);
	إذا (argc)
		usage_with_options(builtin_maintenance_رجسٽر_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_سيٽ(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	إذا (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_سيٽ(the_repository, "maintenance.strategy", "incremental");

	إذا (!repo_config_get_string_multi(the_repository, ڪنجي, &list)) {
		for_each_string_list_شي(شي, list) {
			إذا (!strcmp(maintpath, شي->string)) {
				مليو_2 = 1;
				اكسر;
			}
		}
	}

	إذا (!مليو_2) {
		صحيح rc;
		حرف *global_config_file = عدم;

		إذا (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		إذا (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_سيٽ_multivar_اندريون_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, عدم, 0);
		حرر(global_config_file);

		إذا (rc)
			die(_("unable to add '%s' value of '%s'"),
			    ڪنجي, maintpath);
	}

	حرر(maintpath);
	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	عدم
};

ساكن صحيح maintenance_unregister(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
				  بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح force = 0;
	حرف *config_file = عدم;
	بنية option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_پڇاڙي(),
	};
	ثابت حرف *ڪنجي = "maintenance.repo";
	حرف *maintpath = get_maintpath();
	صحيح مليو_2 = 0;
	بنية string_list_شي *شي;
	ثابت بنية string_list *list;
	بنية config_سيٽ cs = { { 0 } };

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_maintenance_unregister_usage, 0);
	إذا (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	إذا (config_file) {
		git_configset_init(&cs);
		git_configset_جوڙيو_file(&cs, config_file);
	}
	إذا (!(config_file
	      ? git_configset_get_string_multi(&cs, ڪنجي, &list)
	      : repo_config_get_string_multi(the_repository, ڪنجي, &list))) {
		for_each_string_list_شي(شي, list) {
			إذا (!strcmp(maintpath, شي->string)) {
				مليو_2 = 1;
				اكسر;
			}
		}
	}

	إذا (مليو_2) {
		صحيح rc;
		حرف *global_config_file = عدم;

		إذا (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		إذا (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_سيٽ_multivar_اندريون_file_gently(the_repository,
			config_file, ڪنجي, عدم, maintpath, عدم,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_قدر);
		حرر(global_config_file);

		إذا (rc &&
		    (!force || rc == CONFIG_NOTHING_سيٽ))
			die(_("unable to unset '%s' value of '%s'"),
			    ڪنجي, maintpath);
	} وإلا إذا (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	حرر(maintpath);
	أرجع 0;
}

ساكن ثابت حرف *get_frequency(تعداد وقت_جدول_ترجيح وقت_جدول)
{
	بدل (وقت_جدول) {
	حالة وقت_جدول_HOURLY:
		أرجع "hourly";
	حالة وقت_جدول_DAILY:
		أرجع "daily";
	حالة وقت_جدول_WEEKLY:
		أرجع "weekly";
	افتراضي:
		BUG("invalid schedule %d", وقت_جدول);
	}
}

ساكن ثابت حرف *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	عدم
};

ساكن ثابت حرف *get_extra_config_parameters(فراغ) {
	ساكن ثابت حرف *نتيجو = عدم;
	بنية strbuf builder = STRBUF_INIT;

	إذا (نتيجو)
		أرجع نتيجو;

	لكل (ثابت حرف **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	نتيجو = strbuf_detach(&builder, عدم);
	أرجع نتيجو;
}

ساكن ثابت حرف *get_extra_launchctl_لڙيون(فراغ) {
	ساكن ثابت حرف *نتيجو = عدم;
	بنية strbuf builder = STRBUF_INIT;

	إذا (نتيجو)
		أرجع نتيجو;

	لكل (ثابت حرف **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	نتيجو = strbuf_detach(&builder, عدم);
	أرجع نتيجو;
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
ساكن صحيح get_وقت_جدول_cmd(ثابت حرف *cmd, صحيح *is_موجود, حرف **ٻاهريون)
{
	حرف *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	بنية string_list_شي *شي;
	بنية string_list list = STRING_LIST_INIT_NODUP;

	إذا (!testing) {
		إذا (ٻاهريون)
			*ٻاهريون = xstrdup(cmd);
		أرجع 0;
	}

	إذا (is_موجود)
		*is_موجود = 0;

	string_list_ورهايو_اندريون_رکو(&list, testing, ",", -1);
	for_each_string_list_شي(شي, &list) {
		بنية string_list pair = STRING_LIST_INIT_NODUP;

		إذا (string_list_ورهايو_اندريون_رکو(&pair, شي->string, ":", 2) != 2)
			استمر;

		إذا (!strcmp(cmd, pair.شيون[0].string)) {
			إذا (ٻاهريون)
				*ٻاهريون = xstrdup(pair.شيون[1].string);
			إذا (is_موجود)
				*is_موجود = 1;
			string_list_clear(&pair, 0);
			اذهب ٻاهريون;
		}

		string_list_clear(&pair, 0);
	}

	إذا (ٻاهريون)
		*ٻاهريون = xstrdup(cmd);

ٻاهريون:
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
	إذا (get_وقت_جدول_cmd("launchctl", &is_موجود, عدم))
		أرجع is_موجود;

#جيڪڏهن_وصف_ٿيل __APPLE__
	أرجع 1;
#وإلا
	أرجع 0;
#جيڪڏهن_پورو
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

	expanded = interpolate_رستو(filename.buf, 1);
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
	صحيح نتيجو;
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;
	حرف *uid = launchctl_get_uid();

	get_وقت_جدول_cmd("launchctl", عدم, &cmd);
	strvec_ورهايو(&ٻار.دليلون, cmd);
	strvec_pushl(&ٻار.دليلون, enable ? "bootstrap" : "bootout", uid,
		     filename, عدم);

	ٻار.no_stderr = 1;
	ٻار.no_stdout = 1;

	إذا (شروعات_command(&ٻار))
		die(_("failed to start launchctl"));

	نتيجو = پڄاڻي_command(&ٻار);

	حرر(cmd);
	حرر(uid);
	أرجع نتيجو;
}

ساكن صحيح launchctl_remove_plist(تعداد وقت_جدول_ترجيح وقت_جدول)
{
	ثابت حرف *frequency = get_frequency(وقت_جدول);
	حرف *name = launchctl_service_name(frequency);
	حرف *filename = launchctl_service_filename(name);
	صحيح نتيجو = launchctl_boot_plist(0, filename);
	unlink(filename);
	حرر(filename);
	حرر(name);
	أرجع نتيجو;
}

ساكن صحيح launchctl_remove_plists(فراغ)
{
	أرجع launchctl_remove_plist(وقت_جدول_HOURLY) ||
	       launchctl_remove_plist(وقت_جدول_DAILY) ||
	       launchctl_remove_plist(وقت_جدول_WEEKLY);
}

ساكن صحيح launchctl_list_شامل_آهي_plist(ثابت حرف *name, ثابت حرف *cmd)
{
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;

	strvec_ورهايو(&ٻار.دليلون, cmd);
	strvec_pushl(&ٻار.دليلون, "list", name, عدم);

	ٻار.no_stderr = 1;
	ٻار.no_stdout = 1;

	إذا (شروعات_command(&ٻار))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	أرجع !پڄاڻي_command(&ٻار);
}

ساكن صحيح launchctl_وقت_جدول_plist(ثابت حرف *exec_رستو, تعداد وقت_جدول_ترجيح وقت_جدول)
{
	صحيح i, fd;
	ثابت حرف *preamble, *repeat;
	ثابت حرف *frequency = get_frequency(وقت_جدول);
	حرف *name = launchctl_service_name(frequency);
	حرف *filename = launchctl_service_filename(name);
	بنية تالو_file lk = تالو_INIT;
	ساكن بنانشان ڊگهو تالو_file_timeout_ms = ULONG_MAX;
	بنية strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	بنية stat st;
	حرف *cmd;
	صحيح minute = get_random_minute();

	get_وقت_جدول_cmd("launchctl", عدم, &cmd);
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
	strbuf_addf(&plist, preamble, name, exec_رستو, exec_رستو,
		    get_extra_launchctl_لڙيون(), frequency);

	بدل (وقت_جدول) {
	حالة وقت_جدول_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		لكل (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		اكسر;

	حالة وقت_جدول_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		لكل (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		اكسر;

	حالة وقت_جدول_WEEKLY:
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

	إذا (محفوظ_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	إذا ((ڊگهو)تالو_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &تالو_file_timeout_ms))
		تالو_file_timeout_ms = 150;

	fd = hold_تالو_file_for_update_timeout(&lk, filename, تالو_DIE_ON_ERROR,
					       تالو_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	إذا (!stat(filename, &st) && st.st_سائيز == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_شامل_آهي_plist(name, cmd))
		rollback_تالو_file(&lk);
	وإلا {
		إذا (write_اندريون_ڀريل(fd, plist.buf, plist.len) < 0 ||
		    commit_تالو_file(&lk))
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

ساكن صحيح launchctl_جوڙيو_plists(فراغ)
{
	ثابت حرف *exec_رستو = git_exec_رستو();

	أرجع launchctl_وقت_جدول_plist(exec_رستو, وقت_جدول_HOURLY) ||
	       launchctl_وقت_جدول_plist(exec_رستو, وقت_جدول_DAILY) ||
	       launchctl_وقت_جدول_plist(exec_رستو, وقت_جدول_WEEKLY);
}

ساكن صحيح launchctl_update_وقت_جدول(صحيح لاڳيتو_maintenance, صحيح fd اڻ_استعمال_ٿيل)
{
	إذا (لاڳيتو_maintenance)
		أرجع launchctl_جوڙيو_plists();
	وإلا
		أرجع launchctl_remove_plists();
}

ساكن صحيح is_schtasks_موجود(فراغ)
{
	صحيح is_موجود;
	إذا (get_وقت_جدول_cmd("schtasks", &is_موجود, عدم))
		أرجع is_موجود;

#جيڪڏهن_وصف_ٿيل GIT_WINDOWS_NATIVE
	أرجع 1;
#وإلا
	أرجع 0;
#جيڪڏهن_پورو
}

ساكن حرف *schtasks_task_name(ثابت حرف *frequency)
{
	بنية strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	أرجع strbuf_detach(&label, عدم);
}

ساكن صحيح schtasks_remove_task(تعداد وقت_جدول_ترجيح وقت_جدول)
{
	حرف *cmd;
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;
	ثابت حرف *frequency = get_frequency(وقت_جدول);
	حرف *name = schtasks_task_name(frequency);

	get_وقت_جدول_cmd("schtasks", عدم, &cmd);
	strvec_ورهايو(&ٻار.دليلون, cmd);
	strvec_pushl(&ٻار.دليلون, "/delete", "/tn", name, "/f", عدم);
	حرر(name);
	حرر(cmd);

	أرجع لاڳيتو_command(&ٻار);
}

ساكن صحيح schtasks_remove_tasks(فراغ)
{
	أرجع schtasks_remove_task(وقت_جدول_HOURLY) ||
	       schtasks_remove_task(وقت_جدول_DAILY) ||
	       schtasks_remove_task(وقت_جدول_WEEKLY);
}

ساكن صحيح schtasks_وقت_جدول_task(ثابت حرف *exec_رستو, تعداد وقت_جدول_ترجيح وقت_جدول)
{
	حرف *cmd;
	صحيح نتيجو;
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;
	ثابت حرف *xml;
	بنية tempfile *tfile;
	ثابت حرف *frequency = get_frequency(وقت_جدول);
	حرف *name = schtasks_task_name(frequency);
	بنية strbuf tfilename = STRBUF_INIT;
	صحيح minute = get_random_minute();

	get_وقت_جدول_cmd("schtasks", عدم, &cmd);

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

	بدل (وقت_جدول) {
	حالة وقت_جدول_HOURLY:
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

	حالة وقت_جدول_DAILY:
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

	حالة وقت_جدول_WEEKLY:
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
	fprintf(tfile->fp, xml, exec_رستو, exec_رستو,
		get_extra_config_parameters(), frequency);
	strvec_ورهايو(&ٻار.دليلون, cmd);
	strvec_pushl(&ٻار.دليلون, "/create", "/tn", name, "/f", "/xml",
				  get_tempfile_رستو(tfile), عدم);
	close_tempfile_gently(tfile);

	ٻار.no_stdout = 1;
	ٻار.no_stderr = 1;

	إذا (شروعات_command(&ٻار))
		die(_("failed to start schtasks"));
	نتيجو = پڄاڻي_command(&ٻار);

	delete_tempfile(&tfile);
	حرر(name);
	حرر(cmd);
	أرجع نتيجو;
}

ساكن صحيح schtasks_وقت_جدول_tasks(فراغ)
{
	ثابت حرف *exec_رستو = git_exec_رستو();

	أرجع schtasks_وقت_جدول_task(exec_رستو, وقت_جدول_HOURLY) ||
	       schtasks_وقت_جدول_task(exec_رستو, وقت_جدول_DAILY) ||
	       schtasks_وقت_جدول_task(exec_رستو, وقت_جدول_WEEKLY);
}

ساكن صحيح schtasks_update_وقت_جدول(صحيح لاڳيتو_maintenance, صحيح fd اڻ_استعمال_ٿيل)
{
	إذا (لاڳيتو_maintenance)
		أرجع schtasks_وقت_جدول_tasks();
	وإلا
		أرجع schtasks_remove_tasks();
}

MAYBE_اڻ_استعمال_ٿيل
ساكن صحيح check_crontab_process(ثابت حرف *cmd)
{
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;

	strvec_ورهايو(&ٻار.دليلون, cmd);
	strvec_ڌڪو(&ٻار.دليلون, "-l");
	ٻار.no_stdin = 1;
	ٻار.no_stdout = 1;
	ٻار.no_stderr = 1;
	ٻار.silent_exec_failure = 1;

	إذا (شروعات_command(&ٻار))
		أرجع 0;
	/* Ignore exit code, as an empty crontab will return error. */
	پڄاڻي_command(&ٻار);
	أرجع 1;
}

ساكن صحيح is_crontab_موجود(فراغ)
{
	حرف *cmd;
	صحيح is_موجود;
	صحيح ret;

	إذا (get_وقت_جدول_cmd("crontab", &is_موجود, &cmd)) {
		ret = is_موجود;
		اذهب ٻاهريون;
	}

#جيڪڏهن_وصف_ٿيل __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#وإلا
	ret = check_crontab_process(cmd);
#جيڪڏهن_پورو

ٻاهريون:
	حرر(cmd);
	أرجع ret;
}

#وصف_ڪريو BEGIN_سٽ "# BEGIN GIT MAINTENANCE SCHEDULE"
#وصف_ڪريو پڇاڙي_سٽ "# END GIT MAINTENANCE SCHEDULE"

ساكن صحيح crontab_update_وقت_جدول(صحيح لاڳيتو_maintenance, صحيح fd)
{
	حرف *cmd;
	صحيح نتيجو = 0;
	صحيح اندريون_old_region = 0;
	بنية ٻار_process crontab_list = ٻار_PROCESS_INIT;
	بنية ٻار_process crontab_edit = ٻار_PROCESS_INIT;
	FILE *cron_list, *cron_اندريون;
	بنية strbuf سٽ = STRBUF_INIT;
	بنية tempfile *tmpedit = عدم;
	صحيح minute = get_random_minute();

	get_وقت_جدول_cmd("crontab", عدم, &cmd);
	strvec_ورهايو(&crontab_list.دليلون, cmd);
	strvec_ڌڪو(&crontab_list.دليلون, "-l");
	crontab_list.اندريون = -1;
	crontab_list.ٻاهريون = dup(fd);
	crontab_list.git_cmd = 0;

	إذا (شروعات_command(&crontab_list)) {
		نتيجو = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		اذهب ٻاهريون;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	پڄاڻي_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	إذا (!tmpedit) {
		نتيجو = error(_("failed to create crontab temporary file"));
		اذهب ٻاهريون;
	}
	cron_اندريون = fdopen_tempfile(tmpedit, "w");
	إذا (!cron_اندريون) {
		نتيجو = error(_("failed to open temporary file"));
		اذهب ٻاهريون;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	طالما (!strbuf_getline_lf(&سٽ, cron_list)) {
		إذا (!اندريون_old_region && !strcmp(سٽ.buf, BEGIN_سٽ))
			اندريون_old_region = 1;
		وإلا إذا (اندريون_old_region && !strcmp(سٽ.buf, پڇاڙي_سٽ))
			اندريون_old_region = 0;
		وإلا إذا (!اندريون_old_region)
			fprintf(cron_اندريون, "%s\n", سٽ.buf);
	}
	strbuf_release(&سٽ);

	إذا (لاڳيتو_maintenance) {
		بنية strbuf سٽ_format = STRBUF_INIT;
		ثابت حرف *exec_رستو = git_exec_رستو();

		fprintf(cron_اندريون, "%s\n", BEGIN_سٽ);
		fprintf(cron_اندريون,
			"# The following schedule was created by Git\n");
		fprintf(cron_اندريون, "# Any edits made in this region might be\n");
		fprintf(cron_اندريون,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&سٽ_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_رستو, exec_رستو, get_extra_config_parameters());
		fprintf(cron_اندريون, سٽ_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_اندريون, سٽ_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_اندريون, سٽ_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&سٽ_format);

		fprintf(cron_اندريون, "\n%s\n", پڇاڙي_سٽ);
	}

	fflush(cron_اندريون);

	strvec_ورهايو(&crontab_edit.دليلون, cmd);
	strvec_ڌڪو(&crontab_edit.دليلون, get_tempfile_رستو(tmpedit));
	crontab_edit.git_cmd = 0;

	إذا (شروعات_command(&crontab_edit)) {
		نتيجو = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		اذهب ٻاهريون;
	}

	إذا (پڄاڻي_command(&crontab_edit))
		نتيجو = error(_("'crontab' died"));
	وإلا
		fclose(cron_list);

ٻاهريون:
	delete_tempfile(&tmpedit);
	حرر(cmd);
	أرجع نتيجو;
}

ساكن صحيح real_is_systemd_timer_موجود(فراغ)
{
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;

	strvec_pushl(&ٻار.دليلون, "systemctl", "--user", "list-timers", عدم);
	ٻار.no_stdin = 1;
	ٻار.no_stdout = 1;
	ٻار.no_stderr = 1;
	ٻار.silent_exec_failure = 1;

	إذا (شروعات_command(&ٻار))
		أرجع 0;
	إذا (پڄاڻي_command(&ٻار))
		أرجع 0;
	أرجع 1;
}

ساكن صحيح is_systemd_timer_موجود(فراغ)
{
	صحيح is_موجود;

	إذا (get_وقت_جدول_cmd("systemctl", &is_موجود, عدم))
		أرجع is_موجود;

	أرجع real_is_systemd_timer_موجود();
}

ساكن حرف *xdg_config_home_systemd(ثابت حرف *filename)
{
	أرجع xdg_config_home_for("systemd/user", filename);
}

#وصف_ڪريو SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

ساكن صحيح systemd_timer_delete_timer_file(تعداد وقت_جدول_ترجيح ترجيح)
{
	صحيح ret = 0;
	ثابت حرف *frequency = get_frequency(ترجيح);
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
ساكن صحيح systemd_timer_write_timer_file(تعداد وقت_جدول_ترجيح وقت_جدول,
					  صحيح minute)
{
	صحيح res = -1;
	حرف *filename;
	FILE *file;
	ثابت حرف *unit;
	حرف *وقت_جدول_نمونو = عدم;
	ثابت حرف *frequency = get_frequency(وقت_جدول);
	حرف *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	إذا (محفوظ_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		اذهب error;
	}
	file = fopen_or_warn(filename, "w");
	إذا (!file)
		اذهب error;

	بدل (وقت_جدول) {
	حالة وقت_جدول_HOURLY:
		وقت_جدول_نمونو = xstrfmt("*-*-* 1..23:%02d:00", minute);
		اكسر;

	حالة وقت_جدول_DAILY:
		وقت_جدول_نمونو = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		اكسر;

	حالة وقت_جدول_WEEKLY:
		وقت_جدول_نمونو = xstrfmt("Mon 0:%02d:00", minute);
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
	إذا (fprintf(file, unit, وقت_جدول_نمونو) < 0) {
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
	حرر(وقت_جدول_نمونو);
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
ساكن صحيح systemd_timer_write_service_template(ثابت حرف *exec_رستو)
{
	صحيح res = -1;
	حرف *filename;
	FILE *file;
	ثابت حرف *unit;
	حرف *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	إذا (محفوظ_create_leading_directories(the_repository, filename)) {
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
	إذا (fprintf(file, unit, exec_رستو, exec_رستو, get_extra_config_parameters()) < 0) {
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
				     تعداد وقت_جدول_ترجيح وقت_جدول,
				     صحيح minute)
{
	حرف *cmd = عدم;
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;
	ثابت حرف *frequency = get_frequency(وقت_جدول);
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
		ٻار.no_stderr = 1;
	} وإلا إذا (systemd_timer_write_timer_file(وقت_جدول, minute)) {
		ret = -1;
		اذهب ٻاهريون;
	}

	get_وقت_جدول_cmd("systemctl", عدم, &cmd);
	strvec_ورهايو(&ٻار.دليلون, cmd);
	strvec_pushl(&ٻار.دليلون, "--user", enable ? "enable" : "disable",
		     "--now", عدم);
	strvec_pushf(&ٻار.دليلون, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	إذا (شروعات_command(&ٻار)) {
		ret = error(_("failed to start systemctl"));
		اذهب ٻاهريون;
	}

	إذا (پڄاڻي_command(&ٻار)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		إذا (enable) {
			ret = error(_("failed to run systemctl"));
			اذهب ٻاهريون;
		}
	}

	ret = 0;

ٻاهريون:
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
	أرجع systemd_timer_delete_timer_file(وقت_جدول_HOURLY) |
	       systemd_timer_delete_timer_file(وقت_جدول_DAILY) |
	       systemd_timer_delete_timer_file(وقت_جدول_WEEKLY) |
	       systemd_timer_delete_service_template();
}

ساكن صحيح systemd_timer_delete_units(فراغ)
{
	صحيح minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	أرجع systemd_timer_enable_unit(0, وقت_جدول_HOURLY, minute) |
	       systemd_timer_enable_unit(0, وقت_جدول_DAILY, minute) |
	       systemd_timer_enable_unit(0, وقت_جدول_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

ساكن صحيح systemd_timer_تياري_units(فراغ)
{
	صحيح minute = get_random_minute();
	ثابت حرف *exec_رستو = git_exec_رستو();

	صحيح ret = systemd_timer_write_service_template(exec_رستو) ||
		  systemd_timer_enable_unit(1, وقت_جدول_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, وقت_جدول_DAILY, minute) ||
		  systemd_timer_enable_unit(1, وقت_جدول_WEEKLY, minute);

	إذا (ret)
		systemd_timer_delete_units();
	وإلا
		systemd_timer_delete_stale_timer_templates();

	أرجع ret;
}

ساكن صحيح systemd_timer_update_وقت_جدول(صحيح لاڳيتو_maintenance, صحيح fd اڻ_استعمال_ٿيل)
{
	إذا (لاڳيتو_maintenance)
		أرجع systemd_timer_تياري_units();
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
	صحيح (*update_وقت_جدول)(صحيح لاڳيتو_maintenance, صحيح fd);
} scheduler_fn[] = {
	[SCHEDULER_CRON] = {
		.name = "crontab",
		.is_موجود = is_crontab_موجود,
		.update_وقت_جدول = crontab_update_وقت_جدول,
	},
	[SCHEDULER_SYSTEMD] = {
		.name = "systemctl",
		.is_موجود = is_systemd_timer_موجود,
		.update_وقت_جدول = systemd_timer_update_وقت_جدول,
	},
	[SCHEDULER_LAUNCHCTL] = {
		.name = "launchctl",
		.is_موجود = is_launchctl_موجود,
		.update_وقت_جدول = launchctl_update_وقت_جدول,
	},
	[SCHEDULER_SCHTASKS] = {
		.name = "schtasks",
		.is_موجود = is_schtasks_موجود,
		.update_وقت_جدول = schtasks_update_وقت_جدول,
	},
};

ساكن تعداد scheduler parse_scheduler(ثابت حرف *قدر)
{
	إذا (!قدر)
		أرجع SCHEDULER_INVALID;
	وإلا إذا (!strcasecmp(قدر, "auto"))
		أرجع SCHEDULER_AUTO;
	وإلا إذا (!strcasecmp(قدر, "cron") || !strcasecmp(قدر, "crontab"))
		أرجع SCHEDULER_CRON;
	وإلا إذا (!strcasecmp(قدر, "systemd") ||
		 !strcasecmp(قدر, "systemd-timer"))
		أرجع SCHEDULER_SYSTEMD;
	وإلا إذا (!strcasecmp(قدر, "launchctl"))
		أرجع SCHEDULER_LAUNCHCTL;
	وإلا إذا (!strcasecmp(قدر, "schtasks"))
		أرجع SCHEDULER_SCHTASKS;
	وإلا
		أرجع SCHEDULER_INVALID;
}

ساكن صحيح maintenance_opt_scheduler(ثابت بنية option *opt, ثابت حرف *arg,
				     صحيح unset)
{
	تعداد scheduler *scheduler = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	إذا (*scheduler == SCHEDULER_INVALID)
		أرجع error(_("unrecognized --scheduler argument '%s'"), arg);
	أرجع 0;
}

بنية maintenance_شروعات_opts {
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
#جيڪڏهن_پورو
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

ساكن صحيح update_background_وقت_جدول(ثابت بنية maintenance_شروعات_opts *opts,
				      صحيح enable)
{
	بنانشان صحيح i;
	صحيح نتيجو = 0;
	بنية تالو_file lk;
	حرف *تالو_رستو = xstrfmt("%s/schedule", the_repository->objects->ذريعو->رستو);

	إذا (hold_تالو_file_for_update(&lk, تالو_رستو, تالو_NO_DEREF) < 0) {
		إذا (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_رستو(تالو_رستو), strerror(errno));
		وإلا
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		حرر(تالو_رستو);
		أرجع -1;
	}

	لكل (i = 1; i < ARRAY_سائيز(scheduler_fn); i++) {
		إذا (enable && opts->scheduler == i)
			استمر;
		إذا (!scheduler_fn[i].is_موجود())
			استمر;
		scheduler_fn[i].update_وقت_جدول(0, get_تالو_file_fd(&lk));
	}

	إذا (enable)
		نتيجو = scheduler_fn[opts->scheduler].update_وقت_جدول(
			1, get_تالو_file_fd(&lk));

	rollback_تالو_file(&lk);

	حرر(تالو_رستو);
	أرجع نتيجو;
}

ساكن ثابت حرف *ثابت builtin_maintenance_شروعات_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	عدم
};

ساكن صحيح maintenance_شروعات(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			     بنية repository *repo)
{
	بنية maintenance_شروعات_opts opts = { 0 };
	بنية option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_پڇاڙي()
	};
	ثابت حرف *رجسٽر_دليلون[] = { "register", عدم };

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_maintenance_شروعات_usage, 0);
	إذا (argc)
		usage_with_options(builtin_maintenance_شروعات_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	إذا (update_background_وقت_جدول(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	إذا (maintenance_رجسٽر(ARRAY_سائيز(رجسٽر_دليلون)-1, رجسٽر_دليلون, عدم, repo))
		warning(_("failed to add repo to global config"));
	أرجع 0;
}

ساكن ثابت حرف *ثابت builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	عدم
};

ساكن صحيح maintenance_stop(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			    بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية option options[] = {
		OPT_پڇاڙي()
	};
	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_maintenance_stop_usage, 0);
	إذا (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	أرجع update_background_وقت_جدول(عدم, 0);
}

ساكن ثابت حرف *ثابت builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	عدم
};

ساكن صحيح maintenance_is_needed(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
				 بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية maintenance_لاڳيتو_opts opts = MAINTENANCE_لاڳيتو_OPTS_INIT;
	بنية string_list selected_tasks = STRING_LIST_INIT_DUP;
	بنية gc_config cfg = GC_CONFIG_INIT;
	بنية option options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			       N_("check a specific task"),
			       PARSE_OPT_NONEG, task_option_parse),
		OPT_پڇاڙي()
	};
	bool is_needed = false;

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_maintenance_is_needed_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	إذا (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	إذا (opts.auto_flag) {
		لكل (سائيز_t i = 0; i < opts.tasks_nr; i++) {
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
	maintenance_لاڳيتو_opts_release(&opts);
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
		    ثابت حرف *اڳياڙي,
		    بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_maintenance_options[] = {
		OPT_SUBCOMMAND("run", &fn, maintenance_لاڳيتو),
		OPT_SUBCOMMAND("start", &fn, maintenance_شروعات),
		OPT_SUBCOMMAND("stop", &fn, maintenance_stop),
		OPT_SUBCOMMAND("register", &fn, maintenance_رجسٽر),
		OPT_SUBCOMMAND("unregister", &fn, maintenance_unregister),
		OPT_SUBCOMMAND("is-needed", &fn, maintenance_is_needed),
		OPT_پڇاڙي(),
	};

	argc = parse_options(argc, argv, اڳياڙي, builtin_maintenance_options,
			     builtin_maintenance_usage, 0);
	أرجع fn(argc, argv, اڳياڙي, repo);
}
