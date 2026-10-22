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

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "date.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "tempfile.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "sigchain.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "commit-graph.h"
#అన్తర్భావ "packfile.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "reflog.h"
#అన్తర్భావ "rerere.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "exec-cmd.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hook.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "trace2.h"
#అన్తర్భావ "worktree.h"

#నిర్వచించు FAILED_నిరంతరం "failed to run %s"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	ఖాళీ
};

స్థిర timestamp_t gc_log_expire_సమయం;
స్థిర నిర్మాణం tempfile *pidfile;
స్థిర నిర్మాణం తాళం_file log_తాళం;
స్థిర నిర్మాణం string_list pack_garbage = STRING_LIST_INIT_DUP;

స్థిర శూన్య clean_pack_garbage(శూన్య)
{
	పూర్ణాంకం i;
	కొరకు (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.వస్తువులు[i].string);
	string_list_clear(&pack_garbage, 0);
}

స్థిర శూన్య report_pack_garbage(గుర్తులేని చూసిన_బిట్లు, స్థిరరాశి అక్షరం *మార్గం)
{
	ఒకవేళ (చూసిన_బిట్లు == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, మార్గం);
}

స్థిర శూన్య process_log_file(శూన్య)
{
	నిర్మాణం stat st;
	ఒకవేళ (fstat(get_తాళం_file_fd(&log_తాళం), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		పూర్ణాంకం saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_తాళం_file_మార్గం(&log_తాళం),
			strerror(saved_errno));
		fflush(stderr);
		commit_తాళం_file(&log_తాళం);
		errno = saved_errno;
	} లేకపోతే ఒకవేళ (st.st_కొలత) {
		/* There was some error recorded in the lock file */
		commit_తాళం_file(&log_తాళం);
	} లేకపోతే {
		అక్షరం *మార్గం = repo_git_మార్గం(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(మార్గం);
		rollback_తాళం_file(&log_తాళం);
		విడుదల(మార్గం);
	}
}

స్థిర శూన్య process_log_file_at_exit(శూన్య)
{
	fflush(stderr);
	process_log_file();
}

స్థిర పూర్ణాంకం gc_config_is_timestamp_never(స్థిరరాశి అక్షరం *var)
{
	స్థిరరాశి అక్షరం *విలువ;
	timestamp_t expire;

	ఒకవేళ (!repo_config_get_విలువ(the_repository, var, &విలువ) && విలువ) {
		ఒకవేళ (parse_expiry_date(విలువ, &expire))
			die(_("failed to parse '%s' value '%s'"), var, విలువ);
		తిరిగి expire == 0;
	}
	తిరిగి 0;
}

నిర్మాణం gc_config {
	పూర్ణాంకం pack_refs;
	పూర్ణాంకం prune_reflogs;
	పూర్ణాంకం cruft_packs;
	గుర్తులేని దీర్ఘ max_cruft_కొలత;
	పూర్ణాంకం aggressive_లోతు;
	పూర్ణాంకం aggressive_కిటికీ;
	పూర్ణాంకం detach_auto;
	అక్షరం *gc_log_expire;
	అక్షరం *prune_expire;
	అక్షరం *prune_worktrees_expire;
	అక్షరం *repack_expire_గమ్యం;
};

#నిర్వచించు GC_CONFIG_INIT { \
	.pack_refs = 1, \
	.prune_reflogs = 1, \
	.cruft_packs = 1, \
	.aggressive_లోతు = 50, \
	.aggressive_కిటికీ = 250, \
	.detach_auto = 1, \
	.gc_log_expire = xstrdup("1.day.ago"), \
	.prune_expire = xstrdup("2.weeks.ago"), \
	.prune_worktrees_expire = xstrdup("3.months.ago"), \
}

స్థిర శూన్య gc_config_release(నిర్మాణం gc_config *cfg)
{
	విడుదల(cfg->gc_log_expire);
	విడుదల(cfg->prune_expire);
	విడుదల(cfg->prune_worktrees_expire);
}

స్థిర శూన్య gc_config(నిర్మాణం gc_config *cfg)
{
	స్థిరరాశి అక్షరం *విలువ;
	అక్షరం *owned = ఖాళీ;

	ఒకవేళ (!repo_config_get_విలువ(the_repository, "gc.packrefs", &విలువ)) {
		ఒకవేళ (విలువ && !strcmp(విలువ, "notbare"))
			cfg->pack_refs = -1;
		లేకపోతే
			cfg->pack_refs = git_config_bool("gc.packrefs", విలువ);
	}

	ఒకవేళ (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_కిటికీ);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_లోతు);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_కొలత);

	ఒకవేళ (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		విడుదల(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	ఒకవేళ (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		విడుదల(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	ఒకవేళ (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		విడుదల(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, ఖాళీ);
}

గణన కాలపట్టిక_ప్రాధాన్యత {
	కాలపట్టిక_NONE = 0,
	కాలపట్టిక_WEEKLY = 1,
	కాలపట్టిక_DAILY = 2,
	కాలపట్టిక_HOURLY = 3,
};

స్థిర గణన కాలపట్టిక_ప్రాధాన్యత parse_కాలపట్టిక(స్థిరరాశి అక్షరం *విలువ)
{
	ఒకవేళ (!విలువ)
		తిరిగి కాలపట్టిక_NONE;
	ఒకవేళ (!strcasecmp(విలువ, "hourly"))
		తిరిగి కాలపట్టిక_HOURLY;
	ఒకవేళ (!strcasecmp(విలువ, "daily"))
		తిరిగి కాలపట్టిక_DAILY;
	ఒకవేళ (!strcasecmp(విలువ, "weekly"))
		తిరిగి కాలపట్టిక_WEEKLY;
	తిరిగి కాలపట్టిక_NONE;
}

గణన maintenance_task_label {
	TASK_PREFETCH,
	TASK_LOOSE_OBJECTS,
	TASK_INCREMENTAL_REPACK,
	TASK_GEOMETRIC_REPACK,
	TASK_GC,
	TASK_COMMIT_లేఖాచిత్రం,
	TASK_PACK_REFS,
	TASK_REFLOG_EXPIRE,
	TASK_WORKTREE_PRUNE,
	TASK_RERERE_GC,

	/* Leave as final value */
	TASK_లెక్క
};

నిర్మాణం maintenance_నిరంతరం_opts {
	గణన maintenance_task_label *tasks;
	కొలత_t tasks_nr, tasks_alloc;
	పూర్ణాంకం auto_flag;
	పూర్ణాంకం detach;
	పూర్ణాంకం quiet;
	గణన కాలపట్టిక_ప్రాధాన్యత కాలపట్టిక;
};
#నిర్వచించు MAINTENANCE_నిరంతరం_OPTS_INIT { \
	.detach = -1, \
}

స్థిర శూన్య maintenance_నిరంతరం_opts_release(నిర్మాణం maintenance_నిరంతరం_opts *opts)
{
	విడుదల(opts->tasks);
}

స్థిర పూర్ణాంకం pack_refs_condition(వాడని నిర్మాణం gc_config *cfg)
{
	నిర్మాణం string_list included_refs = STRING_LIST_INIT_NODUP;
	నిర్మాణం ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	నిర్మాణం refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	ఒకవేళ (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		తిరిగి 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	తిరిగి required;
}

స్థిర పూర్ణాంకం maintenance_task_pack_refs(నిర్మాణం maintenance_నిరంతరం_opts *opts,
				      వాడని నిర్మాణం gc_config *cfg)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.వాదనలు, "pack-refs", "--all", "--prune", ఖాళీ);
	ఒకవేళ (opts->auto_flag)
		strvec_నెట్టు(&cmd.వాదనలు, "--auto");

	తిరిగి నిరంతరం_command(&cmd);
}

నిర్మాణం లెక్క_reflog_entries_దత్తాంశం {
	నిర్మాణం expire_reflog_policy_cb policy;
	కొలత_t లెక్క;
	కొలత_t హద్దు;
};

స్థిర పూర్ణాంకం లెక్క_reflog_entries(స్థిరరాశి అక్షరం *refname వాడని,
				నిర్మాణం object_id *old_oid, నిర్మాణం object_id *new_oid,
				స్థిరరాశి అక్షరం *committer, timestamp_t timestamp,
				పూర్ణాంకం tz, స్థిరరాశి అక్షరం *సందేశం, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం లెక్క_reflog_entries_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;
	ఒకవేళ (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, సందేశం, &దత్తాంశం->policy))
		దత్తాంశం->లెక్క++;
	తిరిగి దత్తాంశం->లెక్క >= దత్తాంశం->హద్దు;
}

స్థిర పూర్ణాంకం reflog_expire_condition(నిర్మాణం gc_config *cfg వాడని)
{
	timestamp_t now = సమయం(ఖాళీ);
	నిర్మాణం లెక్క_reflog_entries_దత్తాంశం దత్తాంశం = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	పూర్ణాంకం హద్దు = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &హద్దు);
	ఒకవేళ (!హద్దు)
		తిరిగి 0;
	ఒకవేళ (హద్దు < 0)
		తిరిగి 1;
	దత్తాంశం.హద్దు = హద్దు;

	repo_config(the_repository, reflog_expire_config, &దత్తాంశం.policy.opts);

	reflog_expire_options_సమితి_refname(&దత్తాంశం.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 లెక్క_reflog_entries, &దత్తాంశం);

	reflog_expiry_cleanup(&దత్తాంశం.policy);
	reflog_clear_expire_config(&దత్తాంశం.policy.opts);
	తిరిగి దత్తాంశం.లెక్క >= దత్తాంశం.హద్దు;
}

స్థిర పూర్ణాంకం maintenance_task_reflog_expire(నిర్మాణం maintenance_నిరంతరం_opts *opts వాడని,
					  నిర్మాణం gc_config *cfg వాడని)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.వాదనలు, "reflog", "expire", "--all", ఖాళీ);
	తిరిగి నిరంతరం_command(&cmd);
}

స్థిర పూర్ణాంకం maintenance_task_worktree_prune(నిర్మాణం maintenance_నిరంతరం_opts *opts వాడని,
					   నిర్మాణం gc_config *cfg)
{
	నిర్మాణం బిడ్డ_process prune_worktrees_cmd = బిడ్డ_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.వాదనలు, "worktree", "prune", "--expire", ఖాళీ);
	strvec_నెట్టు(&prune_worktrees_cmd.వాదనలు, cfg->prune_worktrees_expire);

	తిరిగి నిరంతరం_command(&prune_worktrees_cmd);
}

స్థిర పూర్ణాంకం worktree_prune_condition(నిర్మాణం gc_config *cfg)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	పూర్ణాంకం should_prune = 0, హద్దు = 1;
	timestamp_t expiry_date;
	నిర్మాణం dirent *d;
	DIR *dir = ఖాళీ;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &హద్దు);
	ఒకవేళ (హద్దు <= 0) {
		should_prune = హద్దు < 0;
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		వెళ్ళు బహిర్గతం;

	dir = opendir(repo_git_మార్గం_replace(the_repository, &buf, "worktrees"));
	ఒకవేళ (!dir)
		వెళ్ళు బహిర్గతం;

	అయితే (హద్దు && (d = readdir_skip_dot_and_dotdot(dir))) {
		అక్షరం *wtpath;
		strbuf_reset(&buf);
		ఒకవేళ (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			హద్దు--;
		విడుదల(wtpath);
	}

	should_prune = !హద్దు;

బహిర్గతం:
	ఒకవేళ (dir)
		closedir(dir);
	strbuf_release(&buf);
	తిరిగి should_prune;
}

స్థిర పూర్ణాంకం maintenance_task_rerere_gc(నిర్మాణం maintenance_నిరంతరం_opts *opts వాడని,
				      నిర్మాణం gc_config *cfg వాడని)
{
	నిర్మాణం బిడ్డ_process rerere_cmd = బిడ్డ_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.వాదనలు, "rerere", "gc", ఖాళీ);
	తిరిగి నిరంతరం_command(&rerere_cmd);
}

స్థిర పూర్ణాంకం rerere_gc_condition(నిర్మాణం gc_config *cfg వాడని)
{
	పూర్ణాంకం హద్దు = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &హద్దు);
	ఒకవేళ (!హద్దు)
		తిరిగి 0; /* never prune */
	ఒకవేళ (హద్దు < 0)
		తిరిగి 1; /* always prune */

	తిరిగి rerere_gc_needed(the_repository, (కొలత_t)హద్దు);
}

#నిర్వచించు OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_గమ్యం = (cfg)->repack_expire_గమ్యం, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_కొలత = (cfg)->max_cruft_కొలత, \
	.కిటికీ = (aggressive) ? (cfg)->aggressive_కిటికీ : 0, \
	.లోతు = (aggressive) ? (cfg)->aggressive_లోతు : 0

/* return NULL on success, else hostname running the gc */
స్థిర స్థిరరాశి అక్షరం *తాళం_repo_for_gc(పూర్ణాంకం force, pid_t* ret_pid)
{
	నిర్మాణం తాళం_file తాళం = తాళం_INIT;
	అక్షరం my_host[HOST_NAME_MAX + 1];
	నిర్మాణం strbuf sb = STRBUF_INIT;
	నిర్మాణం stat st;
	uintmax_t pid;
	FILE *fp;
	పూర్ణాంకం fd;
	అక్షరం *pidfile_మార్గం;

	ఒకవేళ (is_tempfile_చురుకైన(pidfile))
		/* already locked */
		తిరిగి ఖాళీ;

	ఒకవేళ (xgethostname(my_host, పరిమాణం(my_host)))
		xsnprintf(my_host, పరిమాణం(my_host), "unknown");

	pidfile_మార్గం = repo_git_మార్గం(the_repository, "gc.pid");
	fd = hold_తాళం_file_for_update(&తాళం, pidfile_మార్గం,
				       తాళం_DIE_ON_ERROR);
	ఒకవేళ (!force) {
		స్థిర అక్షరం locking_host[HOST_NAME_MAX + 1];
		స్థిర అక్షరం *పరిశీలన_fmt;
		పూర్ణాంకం should_exit;

		ఒకవేళ (!పరిశీలన_fmt)
			పరిశీలన_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_మార్గం, "r");
		memset(locking_host, 0, పరిమాణం(locking_host));
		should_exit =
			fp != ఖాళీ &&
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
			సమయం(ఖాళీ) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, పరిశీలన_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		ఒకవేళ (fp)
			fclose(fp);
		ఒకవేళ (should_exit) {
			ఒకవేళ (fd >= 0)
				rollback_తాళం_file(&తాళం);
			*ret_pid = pid;
			విడుదల(pidfile_మార్గం);
			తిరిగి locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_లోపల_నిండిన(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_తాళం_file(&తాళం);
	pidfile = రిజిస్టరు_tempfile(pidfile_మార్గం);
	విడుదల(pidfile_మార్గం);
	తిరిగి ఖాళీ;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
స్థిర పూర్ణాంకం report_చివరి_gc_error(శూన్య)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	పూర్ణాంకం ret = 0;
	ssize_t len;
	నిర్మాణం stat st;
	అక్షరం *gc_log_మార్గం = repo_git_మార్గం(the_repository, "gc.log");

	ఒకవేళ (stat(gc_log_మార్గం, &st)) {
		ఒకవేళ (errno == ENOENT)
			వెళ్ళు పూర్తి;

		ret = die_సందేశం_errno(_("cannot stat '%s'"), gc_log_మార్గం);
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (st.st_mtime < gc_log_expire_సమయం)
		వెళ్ళు పూర్తి;

	len = strbuf_read_file(&sb, gc_log_మార్గం, 0);
	ఒకవేళ (len < 0)
		ret = die_సందేశం_errno(_("cannot read '%s'"), gc_log_మార్గం);
	లేకపోతే ఒకవేళ (len > 0) {
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
			    gc_log_మార్గం, sb.buf);
		ret = 1;
	}
	strbuf_release(&sb);
పూర్తి:
	విడుదల(gc_log_మార్గం);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం gc_foreground_tasks(నిర్మాణం maintenance_నిరంతరం_opts *opts,
			       నిర్మాణం gc_config *cfg)
{
	ఒకవేళ (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		తిరిగి error(FAILED_నిరంతరం, "pack-refs");
	ఒకవేళ (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		తిరిగి error(FAILED_నిరంతరం, "reflog");
	తిరిగి 0;
}

స్థిర పూర్ణాంకం maintenance_task_odb(నిర్మాణం maintenance_నిరంతరం_opts *opts,
				నిర్మాణం gc_config *cfg,
				పూర్ణాంకం keep_అతిపెద్ద_pack,
				పూర్ణాంకం aggressive)
{
	నిర్మాణం odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_అతిపెద్ద_pack = keep_అతిపెద్ద_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	ఒకవేళ (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	ఒకవేళ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	ఒకవేళ (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	తిరిగి odb_optimize(the_repository->objects, &odb_opts);
}

పూర్ణాంకం cmd_gc(పూర్ణాంకం argc,
	   స్థిరరాశి అక్షరం **argv,
	   స్థిరరాశి అక్షరం *ఉపసర్గ,
	   నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం aggressive = 0;
	పూర్ణాంకం force = 0;
	స్థిరరాశి అక్షరం *name;
	pid_t pid;
	పూర్ణాంకం daemonized = 0;
	పూర్ణాంకం keep_అతిపెద్ద_pack = -1;
	పూర్ణాంకం skip_foreground_tasks = 0;
	timestamp_t dummy;
	నిర్మాణం maintenance_నిరంతరం_opts opts = MAINTENANCE_నిరంతరం_OPTS_INIT;
	నిర్మాణం gc_config cfg = GC_CONFIG_INIT;
	స్థిరరాశి అక్షరం *prune_expire_sentinel = "sentinel";
	స్థిరరాశి అక్షరం *prune_expire_arg = prune_expire_sentinel;
	పూర్ణాంకం ret;
	నిర్మాణం option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.విలువ = &prune_expire_arg,
			.argh = N_("date"),
			.help = N_("prune unreferenced objects"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)prune_expire_arg,
		},
		OPT_BOOL(0, "cruft", &cfg.cruft_packs, N_("pack unreferenced objects separately")),
		OPT_UNSIGNED(0, "max-cruft-size", &cfg.max_cruft_కొలత,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL(0, "aggressive", &aggressive, N_("be more thorough (increased runtime)")),
		OPT_BOOL_F(0, "auto", &opts.auto_flag, N_("enable auto-gc mode"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform garbage collection in the background")),
		OPT_BOOL_F(0, "force", &force,
			   N_("force running gc even if there may be another gc running"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "keep-largest-pack", &keep_అతిపెద్ద_pack,
			 N_("repack all other packs except the largest pack")),
		OPT_STRING(0, "expire-to", &cfg.repack_expire_గమ్యం, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_HIDDEN_BOOL(0, "skip-foreground-tasks", &skip_foreground_tasks,
			   N_("skip maintenance tasks typically done in the foreground")),
		OPT_చివర()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_gc_usage, builtin_gc_options);

	gc_config(&cfg);

	ఒకవేళ (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_సమయం))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	ఒకవేళ (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_gc_options,
			     builtin_gc_usage, 0);
	ఒకవేళ (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	ఒకవేళ (prune_expire_arg != prune_expire_sentinel) {
		విడుదల(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	ఒకవేళ (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	ఒకవేళ (opts.auto_flag) {
		నిర్మాణం odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		ఒకవేళ (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		ఒకవేళ (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    నిరంతరం_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			వెళ్ళు బహిర్గతం;
		}

		ఒకవేళ (!opts.quiet) {
			ఒకవేళ (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			లేకపోతే
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	ఒకవేళ (opts.detach > 0) {
		ret = report_చివరి_gc_error();
		ఒకవేళ (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			వెళ్ళు బహిర్గతం;

		} లేకపోతే ఒకవేళ (ret) {
			/* an I/O error occurred, already reported */
			వెళ్ళు బహిర్గతం;
		}

		ఒకవేళ (!skip_foreground_tasks) {
			ఒకవేళ (తాళం_repo_for_gc(force, &pid)) {
				ret = 0;
				వెళ్ళు బహిర్గతం;
			}

			ఒకవేళ (gc_foreground_tasks(&opts, &cfg) < 0)
				die(ఖాళీ);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = తాళం_repo_for_gc(force, &pid);
	ఒకవేళ (name) {
		ఒకవేళ (opts.auto_flag) {
			ret = 0;
			వెళ్ళు బహిర్గతం; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	ఒకవేళ (daemonized) {
		అక్షరం *మార్గం = repo_git_మార్గం(the_repository, "gc.log");
		hold_తాళం_file_for_update(&log_తాళం, మార్గం,
					  తాళం_DIE_ON_ERROR);
		dup2(get_తాళం_file_fd(&log_తాళం), 2);
		atexit(process_log_file_at_exit);
		విడుదల(మార్గం);
	}

	ఒకవేళ (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	ఒకవేళ (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_నిరంతరం, "worktree");

	ఒకవేళ (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_నిరంతరం, "rerere");

	ఒకవేళ (maintenance_task_odb(&opts, &cfg, keep_అతిపెద్ద_pack, aggressive))
		die(ఖాళీ);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	ఒకవేళ (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	ఒకవేళ (the_repository->settings.gc_write_commit_లేఖాచిత్రం == 1)
		write_commit_లేఖాచిత్రం_reachable(the_repository,
					     the_repository->objects->ఆధారం->మార్గం,
					     !opts.quiet && !daemonized ? COMMIT_లేఖాచిత్రం_WRITE_పురోగతి : 0,
					     ఖాళీ);

	ఒకవేళ (!daemonized) {
		అక్షరం *మార్గం = repo_git_మార్గం(the_repository, "gc.log");
		unlink(మార్గం);
		విడుదల(మార్గం);
	}

బహిర్గతం:
	maintenance_నిరంతరం_opts_release(&opts);
	gc_config_release(&cfg);
	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_maintenance_నిరంతరం_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	ఖాళీ
};

స్థిర పూర్ణాంకం maintenance_opt_కాలపట్టిక(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				    పూర్ణాంకం unset)
{
	గణన కాలపట్టిక_ప్రాధాన్యత *ప్రాధాన్యత = opt->విలువ;

	ఒకవేళ (unset)
		die(_("--no-schedule is not allowed"));

	*ప్రాధాన్యత = parse_కాలపట్టిక(arg);

	ఒకవేళ (!*ప్రాధాన్యత)
		die(_("unrecognized --schedule argument '%s'"), arg);

	తిరిగి 0;
}

నిర్మాణం cg_auto_దత్తాంశం {
	పూర్ణాంకం అంకె_not_లోపల_లేఖాచిత్రం;
	పూర్ణాంకం హద్దు;
};

స్థిర పూర్ణాంకం dfs_on_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం cg_auto_దత్తాంశం *దత్తాంశం = (నిర్మాణం cg_auto_దత్తాంశం *)cb_దత్తాంశం;
	పూర్ణాంకం ఫలితం = 0;
	స్థిరరాశి నిర్మాణం object_id *maybe_peeled = ref->oid;
	నిర్మాణం object_id peeled;
	నిర్మాణం commit_list *దొంతర = ఖాళీ;
	నిర్మాణం commit *commit;

	ఒకవేళ (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	ఒకవేళ (odb_read_object_info(the_repository->objects, maybe_peeled, ఖాళీ) != OBJ_COMMIT)
		తిరిగి 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	ఒకవేళ (!commit || commit->object.flags & చూసిన_2)
		తిరిగి 0;
	commit->object.flags |= చూసిన_2;

	ఒకవేళ (repo_parse_commit(the_repository, commit) ||
	    commit_లేఖాచిత్రం_position(commit) != COMMIT_NOT_FROM_లేఖాచిత్రం)
		తిరిగి 0;

	దత్తాంశం->అంకె_not_లోపల_లేఖాచిత్రం++;

	ఒకవేళ (దత్తాంశం->అంకె_not_లోపల_లేఖాచిత్రం >= దత్తాంశం->హద్దు)
		తిరిగి 1;

	commit_list_చొప్పించు(commit, &దొంతర);

	అయితే (!ఫలితం && దొంతర) {
		నిర్మాణం commit_list *మాతృ;

		commit = తీయి_commit(&దొంతర);

		కొరకు (మాతృ = commit->parents; మాతృ; మాతృ = మాతృ->తదుపరి) {
			ఒకవేళ (repo_parse_commit(the_repository, మాతృ->అంశం) ||
			    commit_లేఖాచిత్రం_position(మాతృ->అంశం) != COMMIT_NOT_FROM_లేఖాచిత్రం ||
			    మాతృ->అంశం->object.flags & చూసిన_2)
				కొనసాగు;

			మాతృ->అంశం->object.flags |= చూసిన_2;
			దత్తాంశం->అంకె_not_లోపల_లేఖాచిత్రం++;

			ఒకవేళ (దత్తాంశం->అంకె_not_లోపల_లేఖాచిత్రం >= దత్తాంశం->హద్దు) {
				ఫలితం = 1;
				ఆపు;
			}

			commit_list_చొప్పించు(మాతృ->అంశం, &దొంతర);
		}
	}

	commit_list_free(దొంతర);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం should_write_commit_లేఖాచిత్రం(నిర్మాణం gc_config *cfg వాడని)
{
	పూర్ణాంకం ఫలితం;
	నిర్మాణం cg_auto_దత్తాంశం దత్తాంశం;

	దత్తాంశం.అంకె_not_లోపల_లేఖాచిత్రం = 0;
	దత్తాంశం.హద్దు = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &దత్తాంశం.హద్దు);

	ఒకవేళ (!దత్తాంశం.హద్దు)
		తిరిగి 0;
	ఒకవేళ (దత్తాంశం.హద్దు < 0)
		తిరిగి 1;

	ఫలితం = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &దత్తాంశం);

	repo_clear_commit_marks(the_repository, చూసిన_2);

	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం నిరంతరం_write_commit_లేఖాచిత్రం(నిర్మాణం maintenance_నిరంతరం_opts *opts)
{
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	బిడ్డ.git_cmd = 1;
	బిడ్డ.odb_గమ్యం_close = the_repository->objects;
	strvec_pushl(&బిడ్డ.వాదనలు, "commit-graph", "write",
		     "--split", "--reachable", ఖాళీ);

	ఒకవేళ (opts->quiet)
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--no-progress");
	లేకపోతే
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--progress");

	తిరిగి !!నిరంతరం_command(&బిడ్డ);
}

స్థిర పూర్ణాంకం maintenance_task_commit_లేఖాచిత్రం(నిర్మాణం maintenance_నిరంతరం_opts *opts,
					 నిర్మాణం gc_config *cfg వాడని)
{
	prepare_repo_settings(the_repository);
	ఒకవేళ (!the_repository->settings.core_commit_లేఖాచిత్రం)
		తిరిగి 0;

	ఒకవేళ (నిరంతరం_write_commit_లేఖాచిత్రం(opts)) {
		error(_("failed to write commit-graph"));
		తిరిగి 1;
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం fetch_remote(నిర్మాణం remote *remote, శూన్య *cbdata)
{
	నిర్మాణం maintenance_నిరంతరం_opts *opts = cbdata;
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	ఒకవేళ (remote->skip_default_update)
		తిరిగి 0;

	బిడ్డ.git_cmd = 1;
	strvec_pushl(&బిడ్డ.వాదనలు, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     ఖాళీ);

	ఒకవేళ (opts->quiet)
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--quiet");

	తిరిగి !!నిరంతరం_command(&బిడ్డ);
}

స్థిర పూర్ణాంకం maintenance_task_prefetch(నిర్మాణం maintenance_నిరంతరం_opts *opts,
				     నిర్మాణం gc_config *cfg వాడని)
{
	ఒకవేళ (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		తిరిగి 1;
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం maintenance_task_gc_foreground(నిర్మాణం maintenance_నిరంతరం_opts *opts,
					  నిర్మాణం gc_config *cfg)
{
	తిరిగి gc_foreground_tasks(opts, cfg);
}

స్థిర పూర్ణాంకం maintenance_task_gc_background(నిర్మాణం maintenance_నిరంతరం_opts *opts,
					  నిర్మాణం gc_config *cfg వాడని)
{
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	బిడ్డ.git_cmd = 1;
	బిడ్డ.odb_గమ్యం_close = the_repository->objects;
	strvec_నెట్టు(&బిడ్డ.వాదనలు, "gc");

	ఒకవేళ (opts->auto_flag)
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--auto");
	ఒకవేళ (opts->quiet)
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--quiet");
	లేకపోతే
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--no-quiet");
	strvec_నెట్టు(&బిడ్డ.వాదనలు, "--no-detach");
	strvec_నెట్టు(&బిడ్డ.వాదనలు, "--skip-foreground-tasks");

	తిరిగి నిరంతరం_command(&బిడ్డ);
}

స్థిర పూర్ణాంకం gc_condition(నిర్మాణం gc_config *cfg)
{
	నిర్మాణం odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	తిరిగి odb_optimize_required(the_repository->objects, &opts);
}

స్థిర పూర్ణాంకం prune_packed(నిర్మాణం maintenance_నిరంతరం_opts *opts)
{
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	బిడ్డ.git_cmd = 1;
	strvec_నెట్టు(&బిడ్డ.వాదనలు, "prune-packed");

	ఒకవేళ (opts->quiet)
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--quiet");

	తిరిగి !!నిరంతరం_command(&బిడ్డ);
}

నిర్మాణం write_loose_object_దత్తాంశం {
	FILE *లోపల;
	పూర్ణాంకం లెక్క;
	పూర్ణాంకం batch_కొలత;
};

స్థిర పూర్ణాంకం loose_object_auto_హద్దు = 100;

స్థిర పూర్ణాంకం loose_object_లెక్క(స్థిరరాశి నిర్మాణం object_id *oid వాడని,
			      స్థిరరాశి అక్షరం *మార్గం వాడని,
			      శూన్య *దత్తాంశం)
{
	పూర్ణాంకం *లెక్క = (పూర్ణాంకం*)దత్తాంశం;
	ఒకవేళ (++(*లెక్క) >= loose_object_auto_హద్దు)
		తిరిగి 1;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం loose_object_auto_condition(నిర్మాణం gc_config *cfg వాడని)
{
	పూర్ణాంకం లెక్క = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_హద్దు);

	ఒకవేళ (!loose_object_auto_హద్దు)
		తిరిగి 0;
	ఒకవేళ (loose_object_auto_హద్దు < 0)
		తిరిగి 1;

	తిరిగి for_each_loose_file_లోపల_ఆధారం(the_repository->objects->ఆధారం,
					     loose_object_లెక్క,
					     ఖాళీ, ఖాళీ, &లెక్క);
}

స్థిర పూర్ణాంకం bail_on_loose(స్థిరరాశి నిర్మాణం object_id *oid వాడని,
			 స్థిరరాశి అక్షరం *మార్గం వాడని,
			 శూన్య *దత్తాంశం వాడని)
{
	తిరిగి 1;
}

స్థిర పూర్ణాంకం write_loose_object_గమ్యం_stdin(స్థిరరాశి నిర్మాణం object_id *oid,
				       స్థిరరాశి అక్షరం *మార్గం వాడని,
				       శూన్య *దత్తాంశం)
{
	నిర్మాణం write_loose_object_దత్తాంశం *d = (నిర్మాణం write_loose_object_దత్తాంశం *)దత్తాంశం;

	fprintf(d->లోపల, "%s\n", oid_గమ్యం_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	తిరిగి ++(d->లెక్క) > d->batch_కొలత;
}

స్థిర పూర్ణాంకం pack_loose(నిర్మాణం maintenance_నిరంతరం_opts *opts)
{
	నిర్మాణం repository *r = the_repository;
	పూర్ణాంకం ఫలితం = 0;
	నిర్మాణం write_loose_object_దత్తాంశం దత్తాంశం;
	నిర్మాణం బిడ్డ_process pack_proc = బిడ్డ_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	ఒకవేళ (!for_each_loose_file_లోపల_ఆధారం(r->objects->ఆధారం,
					   bail_on_loose,
					   ఖాళీ, ఖాళీ, ఖాళీ))
		తిరిగి 0;

	pack_proc.git_cmd = 1;

	strvec_నెట్టు(&pack_proc.వాదనలు, "pack-objects");
	ఒకవేళ (opts->quiet)
		strvec_నెట్టు(&pack_proc.వాదనలు, "--quiet");
	లేకపోతే
		strvec_నెట్టు(&pack_proc.వాదనలు, "--no-quiet");
	strvec_pushf(&pack_proc.వాదనలు, "%s/pack/loose", r->objects->ఆధారం->మార్గం);

	pack_proc.లోపల = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.బహిర్గతం = -1;

	ఒకవేళ (ప్రారంభం_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		తిరిగి 1;
	}

	దత్తాంశం.లోపల = xfdopen(pack_proc.లోపల, "w");
	దత్తాంశం.లెక్క = 0;
	దత్తాంశం.batch_కొలత = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &దత్తాంశం.batch_కొలత);

	/* If configured as 0, then remove limit. */
	ఒకవేళ (!దత్తాంశం.batch_కొలత)
		దత్తాంశం.batch_కొలత = గరిష్ఠ_పూర్ణాంకం;
	లేకపోతే ఒకవేళ (దత్తాంశం.batch_కొలత > 0)
		దత్తాంశం.batch_కొలత--; /* Decrease for equality on limit. */

	for_each_loose_file_లోపల_ఆధారం(r->objects->ఆధారం,
				      write_loose_object_గమ్యం_stdin,
				      ఖాళీ, ఖాళీ, &దత్తాంశం);

	fclose(దత్తాంశం.లోపల);

	ఒకవేళ (ముగింపు_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		ఫలితం = 1;
	}

	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం maintenance_task_loose_objects(నిర్మాణం maintenance_నిరంతరం_opts *opts,
					  నిర్మాణం gc_config *cfg వాడని)
{
	తిరిగి prune_packed(opts) || pack_loose(opts);
}

స్థిర పూర్ణాంకం incremental_repack_auto_condition(నిర్మాణం gc_config *cfg వాడని)
{
	నిర్మాణం packed_git *p;
	పూర్ణాంకం incremental_repack_auto_హద్దు = 10;
	పూర్ణాంకం లెక్క = 0;

	prepare_repo_settings(the_repository);
	ఒకవేళ (!the_repository->settings.core_multi_pack_సూచీ)
		తిరిగి 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_హద్దు);

	ఒకవేళ (!incremental_repack_auto_హద్దు)
		తిరిగి 0;
	ఒకవేళ (incremental_repack_auto_హద్దు < 0)
		తిరిగి 1;

	repo_for_each_pack(the_repository, p) {
		ఒకవేళ (లెక్క >= incremental_repack_auto_హద్దు)
			ఆపు;
		ఒకవేళ (!p->multi_pack_సూచీ)
			లెక్క++;
	}

	తిరిగి లెక్క >= incremental_repack_auto_హద్దు;
}

స్థిర పూర్ణాంకం multi_pack_సూచీ_write(నిర్మాణం maintenance_నిరంతరం_opts *opts)
{
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	బిడ్డ.git_cmd = 1;
	strvec_pushl(&బిడ్డ.వాదనలు, "multi-pack-index", "write", ఖాళీ);

	ఒకవేళ (opts->quiet)
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--no-progress");
	లేకపోతే
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--progress");

	ఒకవేళ (నిరంతరం_command(&బిడ్డ))
		తిరిగి error(_("failed to write multi-pack-index"));

	తిరిగి 0;
}

స్థిర పూర్ణాంకం multi_pack_సూచీ_expire(నిర్మాణం maintenance_నిరంతరం_opts *opts)
{
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	బిడ్డ.git_cmd = 1;
	బిడ్డ.odb_గమ్యం_close = the_repository->objects;
	strvec_pushl(&బిడ్డ.వాదనలు, "multi-pack-index", "expire", ఖాళీ);

	ఒకవేళ (opts->quiet)
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--no-progress");
	లేకపోతే
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--progress");

	ఒకవేళ (నిరంతరం_command(&బిడ్డ))
		తిరిగి error(_("'git multi-pack-index expire' failed"));

	తిరిగి 0;
}

#నిర్వచించు TWO_GIGABYTES (INT32_MAX)

స్థిర off_t get_auto_pack_కొలత(శూన్య)
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
	off_t max_కొలత = 0;
	off_t రెండవ_అతిపెద్ద_కొలత = 0;
	off_t ఫలితం_కొలత;
	నిర్మాణం packed_git *p;
	నిర్మాణం repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		ఒకవేళ (p->pack_కొలత > max_కొలత) {
			రెండవ_అతిపెద్ద_కొలత = max_కొలత;
			max_కొలత = p->pack_కొలత;
		} లేకపోతే ఒకవేళ (p->pack_కొలత > రెండవ_అతిపెద్ద_కొలత)
			రెండవ_అతిపెద్ద_కొలత = p->pack_కొలత;
	}

	ఫలితం_కొలత = రెండవ_అతిపెద్ద_కొలత + 1;

	/* But limit ourselves to a batch size of 2g */
	ఒకవేళ (ఫలితం_కొలత > TWO_GIGABYTES)
		ఫలితం_కొలత = TWO_GIGABYTES;

	తిరిగి ఫలితం_కొలత;
}

స్థిర పూర్ణాంకం multi_pack_సూచీ_repack(నిర్మాణం maintenance_నిరంతరం_opts *opts)
{
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	బిడ్డ.git_cmd = 1;
	బిడ్డ.odb_గమ్యం_close = the_repository->objects;
	strvec_pushl(&బిడ్డ.వాదనలు, "multi-pack-index", "repack", ఖాళీ);

	ఒకవేళ (opts->quiet)
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--no-progress");
	లేకపోతే
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--progress");

	strvec_pushf(&బిడ్డ.వాదనలు, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_కొలత());

	ఒకవేళ (నిరంతరం_command(&బిడ్డ))
		తిరిగి error(_("'git multi-pack-index repack' failed"));

	తిరిగి 0;
}

స్థిర పూర్ణాంకం maintenance_task_incremental_repack(నిర్మాణం maintenance_నిరంతరం_opts *opts,
					       నిర్మాణం gc_config *cfg వాడని)
{
	prepare_repo_settings(the_repository);
	ఒకవేళ (!the_repository->settings.core_multi_pack_సూచీ) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		తిరిగి 0;
	}

	ఒకవేళ (multi_pack_సూచీ_write(opts))
		తిరిగి 1;
	ఒకవేళ (multi_pack_సూచీ_expire(opts))
		తిరిగి 1;
	ఒకవేళ (multi_pack_సూచీ_repack(opts))
		తిరిగి 1;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం maintenance_task_geometric_repack(నిర్మాణం maintenance_నిరంతరం_opts *opts,
					     నిర్మాణం gc_config *cfg)
{
	నిర్మాణం odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	ఒకవేళ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	తిరిగి odb_optimize(the_repository->objects, &odb_opts);
}

స్థిర పూర్ణాంకం geometric_repack_auto_condition(నిర్మాణం gc_config *cfg)
{
	నిర్మాణం odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	తిరిగి odb_optimize_required(the_repository->objects, &opts);
}

రకనిర్వచనం పూర్ణాంకం (*maintenance_task_fn)(నిర్మాణం maintenance_నిరంతరం_opts *opts,
				   నిర్మాణం gc_config *cfg);
రకనిర్వచనం పూర్ణాంకం (*maintenance_auto_fn)(నిర్మాణం gc_config *cfg);

నిర్మాణం maintenance_task {
	స్థిరరాశి అక్షరం *name;

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

స్థిర స్థిరరాశి నిర్మాణం maintenance_task tasks[] = {
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
	[TASK_COMMIT_లేఖాచిత్రం] = {
		.name = "commit-graph",
		.background = maintenance_task_commit_లేఖాచిత్రం,
		.auto_condition = should_write_commit_లేఖాచిత్రం,
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

గణన task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

గణన auto_gc_hook_ఫలితం {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_నిరంతరం = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

స్థిర పూర్ణాంకం maybe_నిరంతరం_task(స్థిరరాశి నిర్మాణం maintenance_task *task,
			  నిర్మాణం repository *repo,
			  నిర్మాణం maintenance_నిరంతరం_opts *opts,
			  నిర్మాణం gc_config *cfg,
			  గణన task_phase phase,
			  గణన auto_gc_hook_ఫలితం *auto_gc_hook_ఫలితం)
{
	పూర్ణాంకం foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	స్థిరరాశి అక్షరం *region = foreground ? "maintenance foreground" : "maintenance";
	పూర్ణాంకం ret = 0;

	ఒకవేళ (!fn)
		తిరిగి 0;
	ఒకవేళ (opts->auto_flag) {
		ఒకవేళ (*auto_gc_hook_ఫలితం == AUTO_GC_HOOK_SKIP)
			తిరిగి 0;

		ఒకవేళ (!task->auto_condition || !task->auto_condition(cfg))
			తిరిగి 0;

		ఒకవేళ (*auto_gc_hook_ఫలితం == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_ఫలితం = నిరంతరం_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_నిరంతరం;
		ఒకవేళ (*auto_gc_hook_ఫలితం == AUTO_GC_HOOK_SKIP)
			తిరిగి 0;
	}

	trace2_region_enter(region, task->name, repo);
	ఒకవేళ (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	తిరిగి ret;
}

స్థిర పూర్ణాంకం maintenance_నిరంతరం_tasks(నిర్మాణం maintenance_నిరంతరం_opts *opts,
				 నిర్మాణం gc_config *cfg)
{
	పూర్ణాంకం ఫలితం = 0;
	నిర్మాణం తాళం_file lk;
	నిర్మాణం repository *r = the_repository;
	అక్షరం *తాళం_మార్గం = xstrfmt("%s/maintenance", r->objects->ఆధారం->మార్గం);
	గణన auto_gc_hook_ఫలితం auto_gc_hook_ఫలితం = AUTO_GC_HOOK_UNDECIDED;

	ఒకవేళ (repo_hold_తాళం_file_for_update(r, &lk, తాళం_మార్గం, తాళం_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		ఒకవేళ (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				తాళం_మార్గం);
		విడుదల(తాళం_మార్గం);
		తిరిగి 0;
	}
	విడుదల(తాళం_మార్గం);

	కొరకు (కొలత_t i = 0; i < opts->tasks_nr; i++)
		ఒకవేళ (maybe_నిరంతరం_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_ఫలితం))
			ఫలితం = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	ఒకవేళ (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	కొరకు (కొలత_t i = 0; i < opts->tasks_nr; i++)
		ఒకవేళ (maybe_నిరంతరం_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_ఫలితం))
			ఫలితం = 1;

	rollback_తాళం_file(&lk);
	తిరిగి ఫలితం;
}

గణన maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

నిర్మాణం maintenance_strategy {
	నిర్మాణం {
		గుర్తులేని type;
		గణన కాలపట్టిక_ప్రాధాన్యత కాలపట్టిక;
	} tasks[TASK_లెక్క];
};

స్థిర స్థిరరాశి నిర్మాణం maintenance_strategy none_strategy = { 0 };

స్థిర స్థిరరాశి నిర్మాణం maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.కాలపట్టిక = కాలపట్టిక_DAILY,
		},
	},
};

స్థిర స్థిరరాశి నిర్మాణం maintenance_strategy incremental_strategy = {
	.tasks = {
		[TASK_COMMIT_లేఖాచిత్రం] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.కాలపట్టిక = కాలపట్టిక_HOURLY,
		},
		[TASK_PREFETCH] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.కాలపట్టిక = కాలపట్టిక_HOURLY,
		},
		[TASK_INCREMENTAL_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.కాలపట్టిక = కాలపట్టిక_DAILY,
		},
		[TASK_LOOSE_OBJECTS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.కాలపట్టిక = కాలపట్టిక_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.కాలపట్టిక = కాలపట్టిక_WEEKLY,
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

స్థిర స్థిరరాశి నిర్మాణం maintenance_strategy geometric_strategy = {
	.tasks = {
		[TASK_COMMIT_లేఖాచిత్రం] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.కాలపట్టిక = కాలపట్టిక_HOURLY,
		},
		[TASK_GEOMETRIC_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.కాలపట్టిక = కాలపట్టిక_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.కాలపట్టిక = కాలపట్టిక_DAILY,
		},
		[TASK_RERERE_GC] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.కాలపట్టిక = కాలపట్టిక_WEEKLY,
		},
		[TASK_REFLOG_EXPIRE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.కాలపట్టిక = కాలపట్టిక_WEEKLY,
		},
		[TASK_WORKTREE_PRUNE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.కాలపట్టిక = కాలపట్టిక_WEEKLY,
		},
	},
};

స్థిర నిర్మాణం maintenance_strategy parse_maintenance_strategy(స్థిరరాశి అక్షరం *name)
{
	ఒకవేళ (!strcasecmp(name, "incremental"))
		తిరిగి incremental_strategy;
	ఒకవేళ (!strcasecmp(name, "gc"))
		తిరిగి gc_strategy;
	ఒకవేళ (!strcasecmp(name, "geometric"))
		తిరిగి geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

స్థిర శూన్య initialize_task_config(నిర్మాణం maintenance_నిరంతరం_opts *opts,
				   స్థిరరాశి నిర్మాణం string_list *selected_tasks)
{
	నిర్మాణం strbuf config_name = STRBUF_INIT;
	నిర్మాణం maintenance_strategy strategy;
	గణన maintenance_type type;
	స్థిరరాశి అక్షరం *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	ఒకవేళ (selected_tasks->nr) {
		కొరకు (కొలత_t i = 0; i < selected_tasks->nr; i++) {
			గణన maintenance_task_label label = (intptr_t)selected_tasks->వస్తువులు[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		తిరిగి;
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
	ఒకవేళ (opts->కాలపట్టిక) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} లేకపోతే {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	ఒకవేళ (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	కొరకు (కొలత_t i = 0; i < TASK_లెక్క; i++) {
		పూర్ణాంకం config_విలువ;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		ఒకవేళ (!repo_config_get_bool(the_repository, config_name.buf, &config_విలువ))
			strategy.tasks[i].type = config_విలువ ? type : 0;
		ఒకవేళ (!(strategy.tasks[i].type & type))
			కొనసాగు;

		ఒకవేళ (opts->కాలపట్టిక) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			ఒకవేళ (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].కాలపట్టిక = parse_కాలపట్టిక(config_str);
			ఒకవేళ (strategy.tasks[i].కాలపట్టిక < opts->కాలపట్టిక)
				కొనసాగు;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

స్థిర పూర్ణాంకం task_option_parse(స్థిరరాశి నిర్మాణం option *opt,
			     స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం string_list *selected_tasks = opt->విలువ;
	కొలత_t i;

	BUG_ON_OPT_NEG(unset);

	కొరకు (i = 0; i < TASK_లెక్క; i++)
		ఒకవేళ (!strcasecmp(tasks[i].name, arg))
			ఆపు;
	ఒకవేళ (i >= TASK_లెక్క) {
		error(_("'%s' is not a valid task"), arg);
		తిరిగి 1;
	}

	ఒకవేళ (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		తిరిగి 1;
	}

	string_list_append(selected_tasks, arg)->util = (శూన్య *)(intptr_t)i;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం maintenance_నిరంతరం(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం maintenance_నిరంతరం_opts opts = MAINTENANCE_నిరంతరం_OPTS_INIT;
	నిర్మాణం string_list selected_tasks = STRING_LIST_INIT_DUP;
	నిర్మాణం gc_config cfg = GC_CONFIG_INIT;
	నిర్మాణం option builtin_maintenance_నిరంతరం_options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform maintenance in the background")),
		OPT_CALLBACK(0, "schedule", &opts.కాలపట్టిక, N_("frequency"),
			     N_("run tasks based on frequency"),
			     maintenance_opt_కాలపట్టిక),
		OPT_BOOL(0, "quiet", &opts.quiet,
			 N_("do not report progress or other information over stderr")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			N_("run a specific task"),
			PARSE_OPT_NONEG, task_option_parse),
		OPT_చివర()
	};
	పూర్ణాంకం ret;

	opts.quiet = !isatty(2);

	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_maintenance_నిరంతరం_options,
			     builtin_maintenance_నిరంతరం_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	die_for_incompatible_opt2(opts.auto_flag, "--auto",
				  opts.కాలపట్టిక, "--schedule=");
	die_for_incompatible_opt2(selected_tasks.nr, "--task=",
				  opts.కాలపట్టిక, "--schedule=");

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	ఒకవేళ (argc != 0)
		usage_with_options(builtin_maintenance_నిరంతరం_usage,
				   builtin_maintenance_నిరంతరం_options);

	ret = maintenance_నిరంతరం_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_నిరంతరం_opts_release(&opts);
	gc_config_release(&cfg);
	తిరిగి ret;
}

స్థిర అక్షరం *get_maintpath(శూన్య)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	తిరిగి strbuf_detach(&sb, ఖాళీ);
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_maintenance_రిజిస్టరు_usage[] = {
	"git maintenance register [--config-file <path>]",
	ఖాళీ
};

స్థిర పూర్ణాంకం maintenance_రిజిస్టరు(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				నిర్మాణం repository *repo వాడని)
{
	అక్షరం *config_file = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_చివర(),
	};
	పూర్ణాంకం దొరికింది_2 = 0;
	స్థిరరాశి అక్షరం *కీలకం = "maintenance.repo";
	అక్షరం *maintpath = get_maintpath();
	నిర్మాణం string_list_అంశం *అంశం;
	స్థిరరాశి నిర్మాణం string_list *list;

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_maintenance_రిజిస్టరు_usage, 0);
	ఒకవేళ (argc)
		usage_with_options(builtin_maintenance_రిజిస్టరు_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_సమితి(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	ఒకవేళ (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_సమితి(the_repository, "maintenance.strategy", "incremental");

	ఒకవేళ (!repo_config_get_string_multi(the_repository, కీలకం, &list)) {
		for_each_string_list_అంశం(అంశం, list) {
			ఒకవేళ (!strcmp(maintpath, అంశం->string)) {
				దొరికింది_2 = 1;
				ఆపు;
			}
		}
	}

	ఒకవేళ (!దొరికింది_2) {
		పూర్ణాంకం rc;
		అక్షరం *global_config_file = ఖాళీ;

		ఒకవేళ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ఒకవేళ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_సమితి_multivar_లోపల_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, ఖాళీ, 0);
		విడుదల(global_config_file);

		ఒకవేళ (rc)
			die(_("unable to add '%s' value of '%s'"),
			    కీలకం, maintpath);
	}

	విడుదల(maintpath);
	తిరిగి 0;
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	ఖాళీ
};

స్థిర పూర్ణాంకం maintenance_unregister(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				  నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం force = 0;
	అక్షరం *config_file = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_చివర(),
	};
	స్థిరరాశి అక్షరం *కీలకం = "maintenance.repo";
	అక్షరం *maintpath = get_maintpath();
	పూర్ణాంకం దొరికింది_2 = 0;
	నిర్మాణం string_list_అంశం *అంశం;
	స్థిరరాశి నిర్మాణం string_list *list;
	నిర్మాణం config_సమితి cs = { { 0 } };

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_maintenance_unregister_usage, 0);
	ఒకవేళ (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	ఒకవేళ (config_file) {
		git_configset_init(&cs);
		git_configset_కూడు_file(&cs, config_file);
	}
	ఒకవేళ (!(config_file
	      ? git_configset_get_string_multi(&cs, కీలకం, &list)
	      : repo_config_get_string_multi(the_repository, కీలకం, &list))) {
		for_each_string_list_అంశం(అంశం, list) {
			ఒకవేళ (!strcmp(maintpath, అంశం->string)) {
				దొరికింది_2 = 1;
				ఆపు;
			}
		}
	}

	ఒకవేళ (దొరికింది_2) {
		పూర్ణాంకం rc;
		అక్షరం *global_config_file = ఖాళీ;

		ఒకవేళ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ఒకవేళ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_సమితి_multivar_లోపల_file_gently(the_repository,
			config_file, కీలకం, ఖాళీ, maintpath, ఖాళీ,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_విలువ);
		విడుదల(global_config_file);

		ఒకవేళ (rc &&
		    (!force || rc == CONFIG_NOTHING_సమితి))
			die(_("unable to unset '%s' value of '%s'"),
			    కీలకం, maintpath);
	} లేకపోతే ఒకవేళ (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	విడుదల(maintpath);
	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *get_frequency(గణన కాలపట్టిక_ప్రాధాన్యత కాలపట్టిక)
{
	ఎంపిక (కాలపట్టిక) {
	స్థితి కాలపట్టిక_HOURLY:
		తిరిగి "hourly";
	స్థితి కాలపట్టిక_DAILY:
		తిరిగి "daily";
	స్థితి కాలపట్టిక_WEEKLY:
		తిరిగి "weekly";
	అప్రమేయ:
		BUG("invalid schedule %d", కాలపట్టిక);
	}
}

స్థిర స్థిరరాశి అక్షరం *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *get_extra_config_parameters(శూన్య) {
	స్థిర స్థిరరాశి అక్షరం *ఫలితం = ఖాళీ;
	నిర్మాణం strbuf builder = STRBUF_INIT;

	ఒకవేళ (ఫలితం)
		తిరిగి ఫలితం;

	కొరకు (స్థిరరాశి అక్షరం **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	ఫలితం = strbuf_detach(&builder, ఖాళీ);
	తిరిగి ఫలితం;
}

స్థిర స్థిరరాశి అక్షరం *get_extra_launchctl_అక్షరశ్రేణులు(శూన్య) {
	స్థిర స్థిరరాశి అక్షరం *ఫలితం = ఖాళీ;
	నిర్మాణం strbuf builder = STRBUF_INIT;

	ఒకవేళ (ఫలితం)
		తిరిగి ఫలితం;

	కొరకు (స్థిరరాశి అక్షరం **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	ఫలితం = strbuf_detach(&builder, ఖాళీ);
	తిరిగి ఫలితం;
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
స్థిర పూర్ణాంకం get_కాలపట్టిక_cmd(స్థిరరాశి అక్షరం *cmd, పూర్ణాంకం *is_అందుబాటు, అక్షరం **బహిర్గతం)
{
	అక్షరం *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం string_list list = STRING_LIST_INIT_NODUP;

	ఒకవేళ (!testing) {
		ఒకవేళ (బహిర్గతం)
			*బహిర్గతం = xstrdup(cmd);
		తిరిగి 0;
	}

	ఒకవేళ (is_అందుబాటు)
		*is_అందుబాటు = 0;

	string_list_విభజించు_లోపల_ఉంచు(&list, testing, ",", -1);
	for_each_string_list_అంశం(అంశం, &list) {
		నిర్మాణం string_list pair = STRING_LIST_INIT_NODUP;

		ఒకవేళ (string_list_విభజించు_లోపల_ఉంచు(&pair, అంశం->string, ":", 2) != 2)
			కొనసాగు;

		ఒకవేళ (!strcmp(cmd, pair.వస్తువులు[0].string)) {
			ఒకవేళ (బహిర్గతం)
				*బహిర్గతం = xstrdup(pair.వస్తువులు[1].string);
			ఒకవేళ (is_అందుబాటు)
				*is_అందుబాటు = 1;
			string_list_clear(&pair, 0);
			వెళ్ళు బహిర్గతం;
		}

		string_list_clear(&pair, 0);
	}

	ఒకవేళ (బహిర్గతం)
		*బహిర్గతం = xstrdup(cmd);

బహిర్గతం:
	string_list_clear(&list, 0);
	విడుదల(testing);
	తిరిగి 1;
}

స్థిర పూర్ణాంకం get_random_minute(శూన్య)
{
	/* Use a static value when under tests. */
	ఒకవేళ (getenv("GIT_TEST_MAINT_SCHEDULER"))
		తిరిగి 13;

	తిరిగి git_rand(0) % 60;
}

స్థిర పూర్ణాంకం is_launchctl_అందుబాటు(శూన్య)
{
	పూర్ణాంకం is_అందుబాటు;
	ఒకవేళ (get_కాలపట్టిక_cmd("launchctl", &is_అందుబాటు, ఖాళీ))
		తిరిగి is_అందుబాటు;

#నిర్వచితమైతే __APPLE__
	తిరిగి 1;
#లేకపోతే
	తిరిగి 0;
#అయితే_ముగింపు
}

స్థిర అక్షరం *launchctl_service_name(స్థిరరాశి అక్షరం *frequency)
{
	నిర్మాణం strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	తిరిగి strbuf_detach(&label, ఖాళీ);
}

స్థిర అక్షరం *launchctl_service_filename(స్థిరరాశి అక్షరం *name)
{
	అక్షరం *expanded;
	నిర్మాణం strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_మార్గం(filename.buf, 1);
	ఒకవేళ (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	తిరిగి expanded;
}

స్థిర అక్షరం *launchctl_get_uid(శూన్య)
{
	తిరిగి xstrfmt("gui/%d", getuid());
}

స్థిర పూర్ణాంకం launchctl_boot_plist(పూర్ణాంకం enable, స్థిరరాశి అక్షరం *filename)
{
	అక్షరం *cmd;
	పూర్ణాంకం ఫలితం;
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;
	అక్షరం *uid = launchctl_get_uid();

	get_కాలపట్టిక_cmd("launchctl", ఖాళీ, &cmd);
	strvec_విభజించు(&బిడ్డ.వాదనలు, cmd);
	strvec_pushl(&బిడ్డ.వాదనలు, enable ? "bootstrap" : "bootout", uid,
		     filename, ఖాళీ);

	బిడ్డ.no_stderr = 1;
	బిడ్డ.no_stdout = 1;

	ఒకవేళ (ప్రారంభం_command(&బిడ్డ))
		die(_("failed to start launchctl"));

	ఫలితం = ముగింపు_command(&బిడ్డ);

	విడుదల(cmd);
	విడుదల(uid);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం launchctl_remove_plist(గణన కాలపట్టిక_ప్రాధాన్యత కాలపట్టిక)
{
	స్థిరరాశి అక్షరం *frequency = get_frequency(కాలపట్టిక);
	అక్షరం *name = launchctl_service_name(frequency);
	అక్షరం *filename = launchctl_service_filename(name);
	పూర్ణాంకం ఫలితం = launchctl_boot_plist(0, filename);
	unlink(filename);
	విడుదల(filename);
	విడుదల(name);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం launchctl_remove_plists(శూన్య)
{
	తిరిగి launchctl_remove_plist(కాలపట్టిక_HOURLY) ||
	       launchctl_remove_plist(కాలపట్టిక_DAILY) ||
	       launchctl_remove_plist(కాలపట్టిక_WEEKLY);
}

స్థిర పూర్ణాంకం launchctl_list_కలిగియుంది_plist(స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *cmd)
{
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	strvec_విభజించు(&బిడ్డ.వాదనలు, cmd);
	strvec_pushl(&బిడ్డ.వాదనలు, "list", name, ఖాళీ);

	బిడ్డ.no_stderr = 1;
	బిడ్డ.no_stdout = 1;

	ఒకవేళ (ప్రారంభం_command(&బిడ్డ))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	తిరిగి !ముగింపు_command(&బిడ్డ);
}

స్థిర పూర్ణాంకం launchctl_కాలపట్టిక_plist(స్థిరరాశి అక్షరం *exec_మార్గం, గణన కాలపట్టిక_ప్రాధాన్యత కాలపట్టిక)
{
	పూర్ణాంకం i, fd;
	స్థిరరాశి అక్షరం *preamble, *repeat;
	స్థిరరాశి అక్షరం *frequency = get_frequency(కాలపట్టిక);
	అక్షరం *name = launchctl_service_name(frequency);
	అక్షరం *filename = launchctl_service_filename(name);
	నిర్మాణం తాళం_file lk = తాళం_INIT;
	స్థిర గుర్తులేని దీర్ఘ తాళం_file_timeout_ms = ULONG_MAX;
	నిర్మాణం strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	నిర్మాణం stat st;
	అక్షరం *cmd;
	పూర్ణాంకం minute = get_random_minute();

	get_కాలపట్టిక_cmd("launchctl", ఖాళీ, &cmd);
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
	strbuf_addf(&plist, preamble, name, exec_మార్గం, exec_మార్గం,
		    get_extra_launchctl_అక్షరశ్రేణులు(), frequency);

	ఎంపిక (కాలపట్టిక) {
	స్థితి కాలపట్టిక_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		కొరకు (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ఆపు;

	స్థితి కాలపట్టిక_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		కొరకు (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ఆపు;

	స్థితి కాలపట్టిక_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		ఆపు;

	అప్రమేయ:
		/* unreachable */
		ఆపు;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	ఒకవేళ (సురక్షిత_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	ఒకవేళ ((దీర్ఘ)తాళం_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &తాళం_file_timeout_ms))
		తాళం_file_timeout_ms = 150;

	fd = hold_తాళం_file_for_update_timeout(&lk, filename, తాళం_DIE_ON_ERROR,
					       తాళం_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	ఒకవేళ (!stat(filename, &st) && st.st_కొలత == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_కలిగియుంది_plist(name, cmd))
		rollback_తాళం_file(&lk);
	లేకపోతే {
		ఒకవేళ (write_లోపల_నిండిన(fd, plist.buf, plist.len) < 0 ||
		    commit_తాళం_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		ఒకవేళ (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	విడుదల(filename);
	విడుదల(name);
	విడుదల(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం launchctl_కూడు_plists(శూన్య)
{
	స్థిరరాశి అక్షరం *exec_మార్గం = git_exec_మార్గం();

	తిరిగి launchctl_కాలపట్టిక_plist(exec_మార్గం, కాలపట్టిక_HOURLY) ||
	       launchctl_కాలపట్టిక_plist(exec_మార్గం, కాలపట్టిక_DAILY) ||
	       launchctl_కాలపట్టిక_plist(exec_మార్గం, కాలపట్టిక_WEEKLY);
}

స్థిర పూర్ణాంకం launchctl_update_కాలపట్టిక(పూర్ణాంకం నిరంతరం_maintenance, పూర్ణాంకం fd వాడని)
{
	ఒకవేళ (నిరంతరం_maintenance)
		తిరిగి launchctl_కూడు_plists();
	లేకపోతే
		తిరిగి launchctl_remove_plists();
}

స్థిర పూర్ణాంకం is_schtasks_అందుబాటు(శూన్య)
{
	పూర్ణాంకం is_అందుబాటు;
	ఒకవేళ (get_కాలపట్టిక_cmd("schtasks", &is_అందుబాటు, ఖాళీ))
		తిరిగి is_అందుబాటు;

#నిర్వచితమైతే GIT_WINDOWS_NATIVE
	తిరిగి 1;
#లేకపోతే
	తిరిగి 0;
#అయితే_ముగింపు
}

స్థిర అక్షరం *schtasks_task_name(స్థిరరాశి అక్షరం *frequency)
{
	నిర్మాణం strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	తిరిగి strbuf_detach(&label, ఖాళీ);
}

స్థిర పూర్ణాంకం schtasks_remove_task(గణన కాలపట్టిక_ప్రాధాన్యత కాలపట్టిక)
{
	అక్షరం *cmd;
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;
	స్థిరరాశి అక్షరం *frequency = get_frequency(కాలపట్టిక);
	అక్షరం *name = schtasks_task_name(frequency);

	get_కాలపట్టిక_cmd("schtasks", ఖాళీ, &cmd);
	strvec_విభజించు(&బిడ్డ.వాదనలు, cmd);
	strvec_pushl(&బిడ్డ.వాదనలు, "/delete", "/tn", name, "/f", ఖాళీ);
	విడుదల(name);
	విడుదల(cmd);

	తిరిగి నిరంతరం_command(&బిడ్డ);
}

స్థిర పూర్ణాంకం schtasks_remove_tasks(శూన్య)
{
	తిరిగి schtasks_remove_task(కాలపట్టిక_HOURLY) ||
	       schtasks_remove_task(కాలపట్టిక_DAILY) ||
	       schtasks_remove_task(కాలపట్టిక_WEEKLY);
}

స్థిర పూర్ణాంకం schtasks_కాలపట్టిక_task(స్థిరరాశి అక్షరం *exec_మార్గం, గణన కాలపట్టిక_ప్రాధాన్యత కాలపట్టిక)
{
	అక్షరం *cmd;
	పూర్ణాంకం ఫలితం;
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;
	స్థిరరాశి అక్షరం *xml;
	నిర్మాణం tempfile *tfile;
	స్థిరరాశి అక్షరం *frequency = get_frequency(కాలపట్టిక);
	అక్షరం *name = schtasks_task_name(frequency);
	నిర్మాణం strbuf tfilename = STRBUF_INIT;
	పూర్ణాంకం minute = get_random_minute();

	get_కాలపట్టిక_cmd("schtasks", ఖాళీ, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	ఒకవేళ (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	ఎంపిక (కాలపట్టిక) {
	స్థితి కాలపట్టిక_HOURLY:
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
		ఆపు;

	స్థితి కాలపట్టిక_DAILY:
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
		ఆపు;

	స్థితి కాలపట్టిక_WEEKLY:
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
		ఆపు;

	అప్రమేయ:
		ఆపు;
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
	fprintf(tfile->fp, xml, exec_మార్గం, exec_మార్గం,
		get_extra_config_parameters(), frequency);
	strvec_విభజించు(&బిడ్డ.వాదనలు, cmd);
	strvec_pushl(&బిడ్డ.వాదనలు, "/create", "/tn", name, "/f", "/xml",
				  get_tempfile_మార్గం(tfile), ఖాళీ);
	close_tempfile_gently(tfile);

	బిడ్డ.no_stdout = 1;
	బిడ్డ.no_stderr = 1;

	ఒకవేళ (ప్రారంభం_command(&బిడ్డ))
		die(_("failed to start schtasks"));
	ఫలితం = ముగింపు_command(&బిడ్డ);

	delete_tempfile(&tfile);
	విడుదల(name);
	విడుదల(cmd);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం schtasks_కాలపట్టిక_tasks(శూన్య)
{
	స్థిరరాశి అక్షరం *exec_మార్గం = git_exec_మార్గం();

	తిరిగి schtasks_కాలపట్టిక_task(exec_మార్గం, కాలపట్టిక_HOURLY) ||
	       schtasks_కాలపట్టిక_task(exec_మార్గం, కాలపట్టిక_DAILY) ||
	       schtasks_కాలపట్టిక_task(exec_మార్గం, కాలపట్టిక_WEEKLY);
}

స్థిర పూర్ణాంకం schtasks_update_కాలపట్టిక(పూర్ణాంకం నిరంతరం_maintenance, పూర్ణాంకం fd వాడని)
{
	ఒకవేళ (నిరంతరం_maintenance)
		తిరిగి schtasks_కాలపట్టిక_tasks();
	లేకపోతే
		తిరిగి schtasks_remove_tasks();
}

MAYBE_వాడని
స్థిర పూర్ణాంకం check_crontab_process(స్థిరరాశి అక్షరం *cmd)
{
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	strvec_విభజించు(&బిడ్డ.వాదనలు, cmd);
	strvec_నెట్టు(&బిడ్డ.వాదనలు, "-l");
	బిడ్డ.no_stdin = 1;
	బిడ్డ.no_stdout = 1;
	బిడ్డ.no_stderr = 1;
	బిడ్డ.silent_exec_failure = 1;

	ఒకవేళ (ప్రారంభం_command(&బిడ్డ))
		తిరిగి 0;
	/* Ignore exit code, as an empty crontab will return error. */
	ముగింపు_command(&బిడ్డ);
	తిరిగి 1;
}

స్థిర పూర్ణాంకం is_crontab_అందుబాటు(శూన్య)
{
	అక్షరం *cmd;
	పూర్ణాంకం is_అందుబాటు;
	పూర్ణాంకం ret;

	ఒకవేళ (get_కాలపట్టిక_cmd("crontab", &is_అందుబాటు, &cmd)) {
		ret = is_అందుబాటు;
		వెళ్ళు బహిర్గతం;
	}

#నిర్వచితమైతే __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#లేకపోతే
	ret = check_crontab_process(cmd);
#అయితే_ముగింపు

బహిర్గతం:
	విడుదల(cmd);
	తిరిగి ret;
}

#నిర్వచించు BEGIN_పంక్తి "# BEGIN GIT MAINTENANCE SCHEDULE"
#నిర్వచించు చివర_పంక్తి "# END GIT MAINTENANCE SCHEDULE"

స్థిర పూర్ణాంకం crontab_update_కాలపట్టిక(పూర్ణాంకం నిరంతరం_maintenance, పూర్ణాంకం fd)
{
	అక్షరం *cmd;
	పూర్ణాంకం ఫలితం = 0;
	పూర్ణాంకం లోపల_old_region = 0;
	నిర్మాణం బిడ్డ_process crontab_list = బిడ్డ_PROCESS_INIT;
	నిర్మాణం బిడ్డ_process crontab_edit = బిడ్డ_PROCESS_INIT;
	FILE *cron_list, *cron_లోపల;
	నిర్మాణం strbuf పంక్తి = STRBUF_INIT;
	నిర్మాణం tempfile *tmpedit = ఖాళీ;
	పూర్ణాంకం minute = get_random_minute();

	get_కాలపట్టిక_cmd("crontab", ఖాళీ, &cmd);
	strvec_విభజించు(&crontab_list.వాదనలు, cmd);
	strvec_నెట్టు(&crontab_list.వాదనలు, "-l");
	crontab_list.లోపల = -1;
	crontab_list.బహిర్గతం = dup(fd);
	crontab_list.git_cmd = 0;

	ఒకవేళ (ప్రారంభం_command(&crontab_list)) {
		ఫలితం = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		వెళ్ళు బహిర్గతం;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	ముగింపు_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	ఒకవేళ (!tmpedit) {
		ఫలితం = error(_("failed to create crontab temporary file"));
		వెళ్ళు బహిర్గతం;
	}
	cron_లోపల = fdopen_tempfile(tmpedit, "w");
	ఒకవేళ (!cron_లోపల) {
		ఫలితం = error(_("failed to open temporary file"));
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	అయితే (!strbuf_getline_lf(&పంక్తి, cron_list)) {
		ఒకవేళ (!లోపల_old_region && !strcmp(పంక్తి.buf, BEGIN_పంక్తి))
			లోపల_old_region = 1;
		లేకపోతే ఒకవేళ (లోపల_old_region && !strcmp(పంక్తి.buf, చివర_పంక్తి))
			లోపల_old_region = 0;
		లేకపోతే ఒకవేళ (!లోపల_old_region)
			fprintf(cron_లోపల, "%s\n", పంక్తి.buf);
	}
	strbuf_release(&పంక్తి);

	ఒకవేళ (నిరంతరం_maintenance) {
		నిర్మాణం strbuf పంక్తి_format = STRBUF_INIT;
		స్థిరరాశి అక్షరం *exec_మార్గం = git_exec_మార్గం();

		fprintf(cron_లోపల, "%s\n", BEGIN_పంక్తి);
		fprintf(cron_లోపల,
			"# The following schedule was created by Git\n");
		fprintf(cron_లోపల, "# Any edits made in this region might be\n");
		fprintf(cron_లోపల,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&పంక్తి_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_మార్గం, exec_మార్గం, get_extra_config_parameters());
		fprintf(cron_లోపల, పంక్తి_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_లోపల, పంక్తి_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_లోపల, పంక్తి_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&పంక్తి_format);

		fprintf(cron_లోపల, "\n%s\n", చివర_పంక్తి);
	}

	fflush(cron_లోపల);

	strvec_విభజించు(&crontab_edit.వాదనలు, cmd);
	strvec_నెట్టు(&crontab_edit.వాదనలు, get_tempfile_మార్గం(tmpedit));
	crontab_edit.git_cmd = 0;

	ఒకవేళ (ప్రారంభం_command(&crontab_edit)) {
		ఫలితం = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (ముగింపు_command(&crontab_edit))
		ఫలితం = error(_("'crontab' died"));
	లేకపోతే
		fclose(cron_list);

బహిర్గతం:
	delete_tempfile(&tmpedit);
	విడుదల(cmd);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం real_is_systemd_timer_అందుబాటు(శూన్య)
{
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	strvec_pushl(&బిడ్డ.వాదనలు, "systemctl", "--user", "list-timers", ఖాళీ);
	బిడ్డ.no_stdin = 1;
	బిడ్డ.no_stdout = 1;
	బిడ్డ.no_stderr = 1;
	బిడ్డ.silent_exec_failure = 1;

	ఒకవేళ (ప్రారంభం_command(&బిడ్డ))
		తిరిగి 0;
	ఒకవేళ (ముగింపు_command(&బిడ్డ))
		తిరిగి 0;
	తిరిగి 1;
}

స్థిర పూర్ణాంకం is_systemd_timer_అందుబాటు(శూన్య)
{
	పూర్ణాంకం is_అందుబాటు;

	ఒకవేళ (get_కాలపట్టిక_cmd("systemctl", &is_అందుబాటు, ఖాళీ))
		తిరిగి is_అందుబాటు;

	తిరిగి real_is_systemd_timer_అందుబాటు();
}

స్థిర అక్షరం *xdg_config_home_systemd(స్థిరరాశి అక్షరం *filename)
{
	తిరిగి xdg_config_home_for("systemd/user", filename);
}

#నిర్వచించు SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

స్థిర పూర్ణాంకం systemd_timer_delete_timer_file(గణన కాలపట్టిక_ప్రాధాన్యత ప్రాధాన్యత)
{
	పూర్ణాంకం ret = 0;
	స్థిరరాశి అక్షరం *frequency = get_frequency(ప్రాధాన్యత);
	అక్షరం *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	అక్షరం *filename = xdg_config_home_systemd(local_timer_name);

	ఒకవేళ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	విడుదల(filename);
	విడుదల(local_timer_name);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం systemd_timer_delete_service_template(శూన్య)
{
	పూర్ణాంకం ret = 0;
	అక్షరం *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	అక్షరం *filename = xdg_config_home_systemd(local_service_name);
	ఒకవేళ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	విడుదల(filename);
	విడుదల(local_service_name);
	తిరిగి ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
స్థిర పూర్ణాంకం systemd_timer_write_timer_file(గణన కాలపట్టిక_ప్రాధాన్యత కాలపట్టిక,
					  పూర్ణాంకం minute)
{
	పూర్ణాంకం res = -1;
	అక్షరం *filename;
	FILE *file;
	స్థిరరాశి అక్షరం *unit;
	అక్షరం *కాలపట్టిక_నమూనా = ఖాళీ;
	స్థిరరాశి అక్షరం *frequency = get_frequency(కాలపట్టిక);
	అక్షరం *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	ఒకవేళ (సురక్షిత_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		వెళ్ళు error;
	}
	file = fopen_or_warn(filename, "w");
	ఒకవేళ (!file)
		వెళ్ళు error;

	ఎంపిక (కాలపట్టిక) {
	స్థితి కాలపట్టిక_HOURLY:
		కాలపట్టిక_నమూనా = xstrfmt("*-*-* 1..23:%02d:00", minute);
		ఆపు;

	స్థితి కాలపట్టిక_DAILY:
		కాలపట్టిక_నమూనా = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		ఆపు;

	స్థితి కాలపట్టిక_WEEKLY:
		కాలపట్టిక_నమూనా = xstrfmt("Mon 0:%02d:00", minute);
		ఆపు;

	అప్రమేయ:
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
	ఒకవేళ (fprintf(file, unit, కాలపట్టిక_నమూనా) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		వెళ్ళు error;
	}
	ఒకవేళ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		వెళ్ళు error;
	}

	res = 0;

error:
	విడుదల(కాలపట్టిక_నమూనా);
	విడుదల(local_timer_name);
	విడుదల(filename);
	తిరిగి res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
స్థిర పూర్ణాంకం systemd_timer_write_service_template(స్థిరరాశి అక్షరం *exec_మార్గం)
{
	పూర్ణాంకం res = -1;
	అక్షరం *filename;
	FILE *file;
	స్థిరరాశి అక్షరం *unit;
	అక్షరం *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	ఒకవేళ (సురక్షిత_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		వెళ్ళు error;
	}
	file = fopen_or_warn(filename, "w");
	ఒకవేళ (!file)
		వెళ్ళు error;

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
	ఒకవేళ (fprintf(file, unit, exec_మార్గం, exec_మార్గం, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		వెళ్ళు error;
	}
	ఒకవేళ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		వెళ్ళు error;
	}

	res = 0;

error:
	విడుదల(local_service_name);
	విడుదల(filename);
	తిరిగి res;
}

స్థిర పూర్ణాంకం systemd_timer_enable_unit(పూర్ణాంకం enable,
				     గణన కాలపట్టిక_ప్రాధాన్యత కాలపట్టిక,
				     పూర్ణాంకం minute)
{
	అక్షరం *cmd = ఖాళీ;
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;
	స్థిరరాశి అక్షరం *frequency = get_frequency(కాలపట్టిక);
	పూర్ణాంకం ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	ఒకవేళ (!enable) {
		బిడ్డ.no_stderr = 1;
	} లేకపోతే ఒకవేళ (systemd_timer_write_timer_file(కాలపట్టిక, minute)) {
		ret = -1;
		వెళ్ళు బహిర్గతం;
	}

	get_కాలపట్టిక_cmd("systemctl", ఖాళీ, &cmd);
	strvec_విభజించు(&బిడ్డ.వాదనలు, cmd);
	strvec_pushl(&బిడ్డ.వాదనలు, "--user", enable ? "enable" : "disable",
		     "--now", ఖాళీ);
	strvec_pushf(&బిడ్డ.వాదనలు, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	ఒకవేళ (ప్రారంభం_command(&బిడ్డ)) {
		ret = error(_("failed to start systemctl"));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (ముగింపు_command(&బిడ్డ)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		ఒకవేళ (enable) {
			ret = error(_("failed to run systemctl"));
			వెళ్ళు బహిర్గతం;
		}
	}

	ret = 0;

బహిర్గతం:
	విడుదల(cmd);
	తిరిగి ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
స్థిర శూన్య systemd_timer_delete_stale_timer_templates(శూన్య)
{
	అక్షరం *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	అక్షరం *filename = xdg_config_home_systemd(timer_template_name);

	ఒకవేళ (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	విడుదల(filename);
	విడుదల(timer_template_name);
}

స్థిర పూర్ణాంకం systemd_timer_delete_unit_files(శూన్య)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	తిరిగి systemd_timer_delete_timer_file(కాలపట్టిక_HOURLY) |
	       systemd_timer_delete_timer_file(కాలపట్టిక_DAILY) |
	       systemd_timer_delete_timer_file(కాలపట్టిక_WEEKLY) |
	       systemd_timer_delete_service_template();
}

స్థిర పూర్ణాంకం systemd_timer_delete_units(శూన్య)
{
	పూర్ణాంకం minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	తిరిగి systemd_timer_enable_unit(0, కాలపట్టిక_HOURLY, minute) |
	       systemd_timer_enable_unit(0, కాలపట్టిక_DAILY, minute) |
	       systemd_timer_enable_unit(0, కాలపట్టిక_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

స్థిర పూర్ణాంకం systemd_timer_అమరిక_units(శూన్య)
{
	పూర్ణాంకం minute = get_random_minute();
	స్థిరరాశి అక్షరం *exec_మార్గం = git_exec_మార్గం();

	పూర్ణాంకం ret = systemd_timer_write_service_template(exec_మార్గం) ||
		  systemd_timer_enable_unit(1, కాలపట్టిక_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, కాలపట్టిక_DAILY, minute) ||
		  systemd_timer_enable_unit(1, కాలపట్టిక_WEEKLY, minute);

	ఒకవేళ (ret)
		systemd_timer_delete_units();
	లేకపోతే
		systemd_timer_delete_stale_timer_templates();

	తిరిగి ret;
}

స్థిర పూర్ణాంకం systemd_timer_update_కాలపట్టిక(పూర్ణాంకం నిరంతరం_maintenance, పూర్ణాంకం fd వాడని)
{
	ఒకవేళ (నిరంతరం_maintenance)
		తిరిగి systemd_timer_అమరిక_units();
	లేకపోతే
		తిరిగి systemd_timer_delete_units();
}

గణన scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

స్థిర స్థిరరాశి నిర్మాణం {
	స్థిరరాశి అక్షరం *name;
	పూర్ణాంకం (*is_అందుబాటు)(శూన్య);
	పూర్ణాంకం (*update_కాలపట్టిక)(పూర్ణాంకం నిరంతరం_maintenance, పూర్ణాంకం fd);
} scheduler_fn[] = {
	[SCHEDULER_CRON] = {
		.name = "crontab",
		.is_అందుబాటు = is_crontab_అందుబాటు,
		.update_కాలపట్టిక = crontab_update_కాలపట్టిక,
	},
	[SCHEDULER_SYSTEMD] = {
		.name = "systemctl",
		.is_అందుబాటు = is_systemd_timer_అందుబాటు,
		.update_కాలపట్టిక = systemd_timer_update_కాలపట్టిక,
	},
	[SCHEDULER_LAUNCHCTL] = {
		.name = "launchctl",
		.is_అందుబాటు = is_launchctl_అందుబాటు,
		.update_కాలపట్టిక = launchctl_update_కాలపట్టిక,
	},
	[SCHEDULER_SCHTASKS] = {
		.name = "schtasks",
		.is_అందుబాటు = is_schtasks_అందుబాటు,
		.update_కాలపట్టిక = schtasks_update_కాలపట్టిక,
	},
};

స్థిర గణన scheduler parse_scheduler(స్థిరరాశి అక్షరం *విలువ)
{
	ఒకవేళ (!విలువ)
		తిరిగి SCHEDULER_INVALID;
	లేకపోతే ఒకవేళ (!strcasecmp(విలువ, "auto"))
		తిరిగి SCHEDULER_AUTO;
	లేకపోతే ఒకవేళ (!strcasecmp(విలువ, "cron") || !strcasecmp(విలువ, "crontab"))
		తిరిగి SCHEDULER_CRON;
	లేకపోతే ఒకవేళ (!strcasecmp(విలువ, "systemd") ||
		 !strcasecmp(విలువ, "systemd-timer"))
		తిరిగి SCHEDULER_SYSTEMD;
	లేకపోతే ఒకవేళ (!strcasecmp(విలువ, "launchctl"))
		తిరిగి SCHEDULER_LAUNCHCTL;
	లేకపోతే ఒకవేళ (!strcasecmp(విలువ, "schtasks"))
		తిరిగి SCHEDULER_SCHTASKS;
	లేకపోతే
		తిరిగి SCHEDULER_INVALID;
}

స్థిర పూర్ణాంకం maintenance_opt_scheduler(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				     పూర్ణాంకం unset)
{
	గణన scheduler *scheduler = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	ఒకవేళ (*scheduler == SCHEDULER_INVALID)
		తిరిగి error(_("unrecognized --scheduler argument '%s'"), arg);
	తిరిగి 0;
}

నిర్మాణం maintenance_ప్రారంభం_opts {
	గణన scheduler scheduler;
};

స్థిర గణన scheduler resolve_scheduler(గణన scheduler scheduler)
{
	ఒకవేళ (scheduler != SCHEDULER_AUTO)
		తిరిగి scheduler;

#ఒకవేళ defined(__APPLE__)
	తిరిగి SCHEDULER_LAUNCHCTL;

#లేదావేళ defined(GIT_WINDOWS_NATIVE)
	తిరిగి SCHEDULER_SCHTASKS;

#లేదావేళ defined(__linux__)
	ఒకవేళ (is_systemd_timer_అందుబాటు())
		తిరిగి SCHEDULER_SYSTEMD;
	లేకపోతే ఒకవేళ (is_crontab_అందుబాటు())
		తిరిగి SCHEDULER_CRON;
	లేకపోతే
		die(_("neither systemd timers nor crontab are available"));

#లేకపోతే
	తిరిగి SCHEDULER_CRON;
#అయితే_ముగింపు
}

స్థిర శూన్య validate_scheduler(గణన scheduler scheduler)
{
	ఒకవేళ (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	ఒకవేళ (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	ఒకవేళ (!scheduler_fn[scheduler].is_అందుబాటు())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

స్థిర పూర్ణాంకం update_background_కాలపట్టిక(స్థిరరాశి నిర్మాణం maintenance_ప్రారంభం_opts *opts,
				      పూర్ణాంకం enable)
{
	గుర్తులేని పూర్ణాంకం i;
	పూర్ణాంకం ఫలితం = 0;
	నిర్మాణం తాళం_file lk;
	అక్షరం *తాళం_మార్గం = xstrfmt("%s/schedule", the_repository->objects->ఆధారం->మార్గం);

	ఒకవేళ (hold_తాళం_file_for_update(&lk, తాళం_మార్గం, తాళం_NO_DEREF) < 0) {
		ఒకవేళ (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_మార్గం(తాళం_మార్గం), strerror(errno));
		లేకపోతే
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		విడుదల(తాళం_మార్గం);
		తిరిగి -1;
	}

	కొరకు (i = 1; i < ARRAY_కొలత(scheduler_fn); i++) {
		ఒకవేళ (enable && opts->scheduler == i)
			కొనసాగు;
		ఒకవేళ (!scheduler_fn[i].is_అందుబాటు())
			కొనసాగు;
		scheduler_fn[i].update_కాలపట్టిక(0, get_తాళం_file_fd(&lk));
	}

	ఒకవేళ (enable)
		ఫలితం = scheduler_fn[opts->scheduler].update_కాలపట్టిక(
			1, get_తాళం_file_fd(&lk));

	rollback_తాళం_file(&lk);

	విడుదల(తాళం_మార్గం);
	తిరిగి ఫలితం;
}

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_maintenance_ప్రారంభం_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	ఖాళీ
};

స్థిర పూర్ణాంకం maintenance_ప్రారంభం(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			     నిర్మాణం repository *repo)
{
	నిర్మాణం maintenance_ప్రారంభం_opts opts = { 0 };
	నిర్మాణం option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *రిజిస్టరు_వాదనలు[] = { "register", ఖాళీ };

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_maintenance_ప్రారంభం_usage, 0);
	ఒకవేళ (argc)
		usage_with_options(builtin_maintenance_ప్రారంభం_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	ఒకవేళ (update_background_కాలపట్టిక(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	ఒకవేళ (maintenance_రిజిస్టరు(ARRAY_కొలత(రిజిస్టరు_వాదనలు)-1, రిజిస్టరు_వాదనలు, ఖాళీ, repo))
		warning(_("failed to add repo to global config"));
	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	ఖాళీ
};

స్థిర పూర్ణాంకం maintenance_stop(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			    నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option options[] = {
		OPT_చివర()
	};
	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_maintenance_stop_usage, 0);
	ఒకవేళ (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	తిరిగి update_background_కాలపట్టిక(ఖాళీ, 0);
}

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	ఖాళీ
};

స్థిర పూర్ణాంకం maintenance_is_needed(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				 నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం maintenance_నిరంతరం_opts opts = MAINTENANCE_నిరంతరం_OPTS_INIT;
	నిర్మాణం string_list selected_tasks = STRING_LIST_INIT_DUP;
	నిర్మాణం gc_config cfg = GC_CONFIG_INIT;
	నిర్మాణం option options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			       N_("check a specific task"),
			       PARSE_OPT_NONEG, task_option_parse),
		OPT_చివర()
	};
	bool is_needed = false;

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_maintenance_is_needed_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ఒకవేళ (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	ఒకవేళ (opts.auto_flag) {
		కొరకు (కొలత_t i = 0; i < opts.tasks_nr; i++) {
			ఒకవేళ (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				ఆపు;
			}
		}
	} లేకపోతే {
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
	maintenance_నిరంతరం_opts_release(&opts);
	gc_config_release(&cfg);

	ఒకవేళ (is_needed)
		తిరిగి 0;
	తిరిగి 1;
}

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	ఖాళీ,
};

పూర్ణాంకం cmd_maintenance(పూర్ణాంకం argc,
		    స్థిరరాశి అక్షరం **argv,
		    స్థిరరాశి అక్షరం *ఉపసర్గ,
		    నిర్మాణం repository *repo)
{
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option builtin_maintenance_options[] = {
		OPT_SUBCOMMAND("run", &fn, maintenance_నిరంతరం),
		OPT_SUBCOMMAND("start", &fn, maintenance_ప్రారంభం),
		OPT_SUBCOMMAND("stop", &fn, maintenance_stop),
		OPT_SUBCOMMAND("register", &fn, maintenance_రిజిస్టరు),
		OPT_SUBCOMMAND("unregister", &fn, maintenance_unregister),
		OPT_SUBCOMMAND("is-needed", &fn, maintenance_is_needed),
		OPT_చివర(),
	};

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_maintenance_options,
			     builtin_maintenance_usage, 0);
	తిరిగి fn(argc, argv, ఉపసర్గ, repo);
}
