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

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "date.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "tempfile.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "sigchain.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "commit-graph.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "reflog.h"
#ಅನ್ತರ್ಭಾವ "rerere.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "exec-cmd.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"
#ಅನ್ತರ್ಭಾವ "worktree.h"

#ವ್ಯಾಖ್ಯಾನಿಸು FAILED_ಸತತ "failed to run %s"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ timestamp_t gc_log_expire_ಸಮಯ;
ಸ್ಥಿರ ರಚನೆ tempfile *pidfile;
ಸ್ಥಿರ ರಚನೆ ಬೀಗ_file log_ಬೀಗ;
ಸ್ಥಿರ ರಚನೆ string_list pack_garbage = STRING_LIST_INIT_DUP;

ಸ್ಥಿರ ಶೂನ್ಯ clean_pack_garbage(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಗಾಗಿ (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.ವಸ್ತುಗಳು[i].string);
	string_list_clear(&pack_garbage, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ report_pack_garbage(ಚಿಹ್ನೆರಹಿತ ನೋಡಿದ_ಬಿಟ್ಟುಗಳು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಆದರೆ (ನೋಡಿದ_ಬಿಟ್ಟುಗಳು == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ process_log_file(ಶೂನ್ಯ)
{
	ರಚನೆ stat st;
	ಆದರೆ (fstat(get_ಬೀಗ_file_fd(&log_ಬೀಗ), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		ಪೂರ್ಣಾಂಕ saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_ಬೀಗ_file_ಮಾರ್ಗ(&log_ಬೀಗ),
			strerror(saved_errno));
		fflush(stderr);
		commit_ಬೀಗ_file(&log_ಬೀಗ);
		errno = saved_errno;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (st.st_ಗಾತ್ರ) {
		/* There was some error recorded in the lock file */
		commit_ಬೀಗ_file(&log_ಬೀಗ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಅಕ್ಷರ *ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(ಮಾರ್ಗ);
		rollback_ಬೀಗ_file(&log_ಬೀಗ);
		ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ process_log_file_at_exit(ಶೂನ್ಯ)
{
	fflush(stderr);
	process_log_file();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ gc_config_is_timestamp_never(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;
	timestamp_t expire;

	ಆದರೆ (!repo_config_get_ಮೌಲ್ಯ(the_repository, var, &ಮೌಲ್ಯ) && ಮೌಲ್ಯ) {
		ಆದರೆ (parse_expiry_date(ಮೌಲ್ಯ, &expire))
			die(_("failed to parse '%s' value '%s'"), var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ expire == 0;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ gc_config {
	ಪೂರ್ಣಾಂಕ pack_refs;
	ಪೂರ್ಣಾಂಕ prune_reflogs;
	ಪೂರ್ಣಾಂಕ cruft_packs;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_cruft_ಗಾತ್ರ;
	ಪೂರ್ಣಾಂಕ aggressive_ಆಳ;
	ಪೂರ್ಣಾಂಕ aggressive_ಕಿಟಕಿ;
	ಪೂರ್ಣಾಂಕ detach_auto;
	ಅಕ್ಷರ *gc_log_expire;
	ಅಕ್ಷರ *prune_expire;
	ಅಕ್ಷರ *prune_worktrees_expire;
	ಅಕ್ಷರ *repack_expire_ಗಮ್ಯ;
};

#ವ್ಯಾಖ್ಯಾನಿಸು GC_CONFIG_INIT { \
	.pack_refs = 1, \
	.prune_reflogs = 1, \
	.cruft_packs = 1, \
	.aggressive_ಆಳ = 50, \
	.aggressive_ಕಿಟಕಿ = 250, \
	.detach_auto = 1, \
	.gc_log_expire = xstrdup("1.day.ago"), \
	.prune_expire = xstrdup("2.weeks.ago"), \
	.prune_worktrees_expire = xstrdup("3.months.ago"), \
}

ಸ್ಥಿರ ಶೂನ್ಯ gc_config_release(ರಚನೆ gc_config *cfg)
{
	ಬಿಡುಗಡೆ(cfg->gc_log_expire);
	ಬಿಡುಗಡೆ(cfg->prune_expire);
	ಬಿಡುಗಡೆ(cfg->prune_worktrees_expire);
}

ಸ್ಥಿರ ಶೂನ್ಯ gc_config(ರಚನೆ gc_config *cfg)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;
	ಅಕ್ಷರ *owned = ಖಾಲಿ;

	ಆದರೆ (!repo_config_get_ಮೌಲ್ಯ(the_repository, "gc.packrefs", &ಮೌಲ್ಯ)) {
		ಆದರೆ (ಮೌಲ್ಯ && !strcmp(ಮೌಲ್ಯ, "notbare"))
			cfg->pack_refs = -1;
		ಇಲ್ಲದಿದ್ದರೆ
			cfg->pack_refs = git_config_bool("gc.packrefs", ಮೌಲ್ಯ);
	}

	ಆದರೆ (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_ಕಿಟಕಿ);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_ಆಳ);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_ಗಾತ್ರ);

	ಆದರೆ (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		ಬಿಡುಗಡೆ(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	ಆದರೆ (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		ಬಿಡುಗಡೆ(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	ಆದರೆ (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		ಬಿಡುಗಡೆ(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
}

ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ {
	ವೇಳಾಪಟ್ಟಿ_NONE = 0,
	ವೇಳಾಪಟ್ಟಿ_WEEKLY = 1,
	ವೇಳಾಪಟ್ಟಿ_DAILY = 2,
	ವೇಳಾಪಟ್ಟಿ_HOURLY = 3,
};

ಸ್ಥಿರ ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ parse_ವೇಳಾಪಟ್ಟಿ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಆದರೆ (!ಮೌಲ್ಯ)
		ಹಿಂತಿರುಗಿ ವೇಳಾಪಟ್ಟಿ_NONE;
	ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "hourly"))
		ಹಿಂತಿರುಗಿ ವೇಳಾಪಟ್ಟಿ_HOURLY;
	ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "daily"))
		ಹಿಂತಿರುಗಿ ವೇಳಾಪಟ್ಟಿ_DAILY;
	ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "weekly"))
		ಹಿಂತಿರುಗಿ ವೇಳಾಪಟ್ಟಿ_WEEKLY;
	ಹಿಂತಿರುಗಿ ವೇಳಾಪಟ್ಟಿ_NONE;
}

ಎಣಿಕೆ maintenance_task_label {
	TASK_PREFETCH,
	TASK_LOOSE_OBJECTS,
	TASK_INCREMENTAL_REPACK,
	TASK_GEOMETRIC_REPACK,
	TASK_GC,
	TASK_COMMIT_ನಕ್ಷೆ,
	TASK_PACK_REFS,
	TASK_REFLOG_EXPIRE,
	TASK_WORKTREE_PRUNE,
	TASK_RERERE_GC,

	/* Leave as final value */
	TASK_ಎಣಿಕೆ
};

ರಚನೆ maintenance_ಸತತ_opts {
	ಎಣಿಕೆ maintenance_task_label *tasks;
	ಗಾತ್ರ_t tasks_nr, tasks_alloc;
	ಪೂರ್ಣಾಂಕ auto_flag;
	ಪೂರ್ಣಾಂಕ detach;
	ಪೂರ್ಣಾಂಕ quiet;
	ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ ವೇಳಾಪಟ್ಟಿ;
};
#ವ್ಯಾಖ್ಯಾನಿಸು MAINTENANCE_ಸತತ_OPTS_INIT { \
	.detach = -1, \
}

ಸ್ಥಿರ ಶೂನ್ಯ maintenance_ಸತತ_opts_release(ರಚನೆ maintenance_ಸತತ_opts *opts)
{
	ಬಿಡುಗಡೆ(opts->tasks);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pack_refs_condition(ಬಳಸದ ರಚನೆ gc_config *cfg)
{
	ರಚನೆ string_list included_refs = STRING_LIST_INIT_NODUP;
	ರಚನೆ ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	ರಚನೆ refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	ಆದರೆ (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		ಹಿಂತಿರುಗಿ 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	ಹಿಂತಿರುಗಿ required;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_pack_refs(ರಚನೆ maintenance_ಸತತ_opts *opts,
				      ಬಳಸದ ರಚನೆ gc_config *cfg)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.ವಾದಗಳು, "pack-refs", "--all", "--prune", ಖಾಲಿ);
	ಆದರೆ (opts->auto_flag)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--auto");

	ಹಿಂತಿರುಗಿ ಸತತ_command(&cmd);
}

ರಚನೆ ಎಣಿಕೆ_reflog_entries_ದತ್ತಾಂಶ {
	ರಚನೆ expire_reflog_policy_cb policy;
	ಗಾತ್ರ_t ಎಣಿಕೆ_2;
	ಗಾತ್ರ_t ಎಲ್ಲೆ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_reflog_entries(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname ಬಳಸದ,
				ರಚನೆ object_id *old_oid, ರಚನೆ object_id *new_oid,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *committer, timestamp_t timestamp,
				ಪೂರ್ಣಾಂಕ tz, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ ಎಣಿಕೆ_reflog_entries_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;
	ಆದರೆ (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, ಸಂದೇಶ, &ದತ್ತಾಂಶ->policy))
		ದತ್ತಾಂಶ->ಎಣಿಕೆ_2++;
	ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ->ಎಣಿಕೆ_2 >= ದತ್ತಾಂಶ->ಎಲ್ಲೆ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reflog_expire_condition(ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	timestamp_t now = ಸಮಯ(ಖಾಲಿ);
	ರಚನೆ ಎಣಿಕೆ_reflog_entries_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	ಪೂರ್ಣಾಂಕ ಎಲ್ಲೆ = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &ಎಲ್ಲೆ);
	ಆದರೆ (!ಎಲ್ಲೆ)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (ಎಲ್ಲೆ < 0)
		ಹಿಂತಿರುಗಿ 1;
	ದತ್ತಾಂಶ.ಎಲ್ಲೆ = ಎಲ್ಲೆ;

	repo_config(the_repository, reflog_expire_config, &ದತ್ತಾಂಶ.policy.opts);

	reflog_expire_options_ಗಣ_refname(&ದತ್ತಾಂಶ.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 ಎಣಿಕೆ_reflog_entries, &ದತ್ತಾಂಶ);

	reflog_expiry_cleanup(&ದತ್ತಾಂಶ.policy);
	reflog_clear_expire_config(&ದತ್ತಾಂಶ.policy.opts);
	ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ.ಎಣಿಕೆ_2 >= ದತ್ತಾಂಶ.ಎಲ್ಲೆ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_reflog_expire(ರಚನೆ maintenance_ಸತತ_opts *opts ಬಳಸದ,
					  ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.ವಾದಗಳು, "reflog", "expire", "--all", ಖಾಲಿ);
	ಹಿಂತಿರುಗಿ ಸತತ_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_worktree_prune(ರಚನೆ maintenance_ಸತತ_opts *opts ಬಳಸದ,
					   ರಚನೆ gc_config *cfg)
{
	ರಚನೆ ಮಗು_process prune_worktrees_cmd = ಮಗು_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.ವಾದಗಳು, "worktree", "prune", "--expire", ಖಾಲಿ);
	strvec_ತಳ್ಳು(&prune_worktrees_cmd.ವಾದಗಳು, cfg->prune_worktrees_expire);

	ಹಿಂತಿರುಗಿ ಸತತ_command(&prune_worktrees_cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ worktree_prune_condition(ರಚನೆ gc_config *cfg)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ should_prune = 0, ಎಲ್ಲೆ = 1;
	timestamp_t expiry_date;
	ರಚನೆ dirent *d;
	DIR *dir = ಖಾಲಿ;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &ಎಲ್ಲೆ);
	ಆದರೆ (ಎಲ್ಲೆ <= 0) {
		should_prune = ಎಲ್ಲೆ < 0;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		ಹೋಗು ಹೊರಹೋಗು;

	dir = opendir(repo_git_ಮಾರ್ಗ_replace(the_repository, &buf, "worktrees"));
	ಆದರೆ (!dir)
		ಹೋಗು ಹೊರಹೋಗು;

	ಆಗ (ಎಲ್ಲೆ && (d = readdir_skip_dot_and_dotdot(dir))) {
		ಅಕ್ಷರ *wtpath;
		strbuf_reset(&buf);
		ಆದರೆ (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			ಎಲ್ಲೆ--;
		ಬಿಡುಗಡೆ(wtpath);
	}

	should_prune = !ಎಲ್ಲೆ;

ಹೊರಹೋಗು:
	ಆದರೆ (dir)
		closedir(dir);
	strbuf_release(&buf);
	ಹಿಂತಿರುಗಿ should_prune;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_rerere_gc(ರಚನೆ maintenance_ಸತತ_opts *opts ಬಳಸದ,
				      ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	ರಚನೆ ಮಗು_process rerere_cmd = ಮಗು_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.ವಾದಗಳು, "rerere", "gc", ಖಾಲಿ);
	ಹಿಂತಿರುಗಿ ಸತತ_command(&rerere_cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rerere_gc_condition(ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ಎಲ್ಲೆ = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &ಎಲ್ಲೆ);
	ಆದರೆ (!ಎಲ್ಲೆ)
		ಹಿಂತಿರುಗಿ 0; /* never prune */
	ಆದರೆ (ಎಲ್ಲೆ < 0)
		ಹಿಂತಿರುಗಿ 1; /* always prune */

	ಹಿಂತಿರುಗಿ rerere_gc_needed(the_repository, (ಗಾತ್ರ_t)ಎಲ್ಲೆ);
}

#ವ್ಯಾಖ್ಯಾನಿಸು OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_ಗಮ್ಯ = (cfg)->repack_expire_ಗಮ್ಯ, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_ಗಾತ್ರ = (cfg)->max_cruft_ಗಾತ್ರ, \
	.ಕಿಟಕಿ = (aggressive) ? (cfg)->aggressive_ಕಿಟಕಿ : 0, \
	.ಆಳ = (aggressive) ? (cfg)->aggressive_ಆಳ : 0

/* return NULL on success, else hostname running the gc */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬೀಗ_repo_for_gc(ಪೂರ್ಣಾಂಕ force, pid_t* ret_pid)
{
	ರಚನೆ ಬೀಗ_file ಬೀಗ = ಬೀಗ_INIT;
	ಅಕ್ಷರ my_host[HOST_NAME_MAX + 1];
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ರಚನೆ stat st;
	uintmax_t pid;
	FILE *fp;
	ಪೂರ್ಣಾಂಕ fd;
	ಅಕ್ಷರ *pidfile_ಮಾರ್ಗ;

	ಆದರೆ (is_tempfile_ಸಕ್ರಿಯ(pidfile))
		/* already locked */
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (xgethostname(my_host, ಗಾತ್ರ(my_host)))
		xsnprintf(my_host, ಗಾತ್ರ(my_host), "unknown");

	pidfile_ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ(the_repository, "gc.pid");
	fd = hold_ಬೀಗ_file_for_update(&ಬೀಗ, pidfile_ಮಾರ್ಗ,
				       ಬೀಗ_DIE_ON_ERROR);
	ಆದರೆ (!force) {
		ಸ್ಥಿರ ಅಕ್ಷರ locking_host[HOST_NAME_MAX + 1];
		ಸ್ಥಿರ ಅಕ್ಷರ *ಪರಿಶೀಲನೆ_fmt;
		ಪೂರ್ಣಾಂಕ should_exit;

		ಆದರೆ (!ಪರಿಶೀಲನೆ_fmt)
			ಪರಿಶೀಲನೆ_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_ಮಾರ್ಗ, "r");
		memset(locking_host, 0, ಗಾತ್ರ(locking_host));
		should_exit =
			fp != ಖಾಲಿ &&
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
			ಸಮಯ(ಖಾಲಿ) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, ಪರಿಶೀಲನೆ_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		ಆದರೆ (fp)
			fclose(fp);
		ಆದರೆ (should_exit) {
			ಆದರೆ (fd >= 0)
				rollback_ಬೀಗ_file(&ಬೀಗ);
			*ret_pid = pid;
			ಬಿಡುಗಡೆ(pidfile_ಮಾರ್ಗ);
			ಹಿಂತಿರುಗಿ locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_ಒಳ_ತುಂಬಿದ(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_ಬೀಗ_file(&ಬೀಗ);
	pidfile = ರಿಜಿಸ್ಟರ್_tempfile(pidfile_ಮಾರ್ಗ);
	ಬಿಡುಗಡೆ(pidfile_ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ report_ಕೊನೆಯ_gc_error(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret = 0;
	ssize_t len;
	ರಚನೆ stat st;
	ಅಕ್ಷರ *gc_log_ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ(the_repository, "gc.log");

	ಆದರೆ (stat(gc_log_ಮಾರ್ಗ, &st)) {
		ಆದರೆ (errno == ENOENT)
			ಹೋಗು ಮುಗಿದ;

		ret = die_ಸಂದೇಶ_errno(_("cannot stat '%s'"), gc_log_ಮಾರ್ಗ);
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (st.st_mtime < gc_log_expire_ಸಮಯ)
		ಹೋಗು ಮುಗಿದ;

	len = strbuf_read_file(&sb, gc_log_ಮಾರ್ಗ, 0);
	ಆದರೆ (len < 0)
		ret = die_ಸಂದೇಶ_errno(_("cannot read '%s'"), gc_log_ಮಾರ್ಗ);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len > 0) {
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
			    gc_log_ಮಾರ್ಗ, sb.buf);
		ret = 1;
	}
	strbuf_release(&sb);
ಮುಗಿದ:
	ಬಿಡುಗಡೆ(gc_log_ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ gc_foreground_tasks(ರಚನೆ maintenance_ಸತತ_opts *opts,
			       ರಚನೆ gc_config *cfg)
{
	ಆದರೆ (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		ಹಿಂತಿರುಗಿ error(FAILED_ಸತತ, "pack-refs");
	ಆದರೆ (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		ಹಿಂತಿರುಗಿ error(FAILED_ಸತತ, "reflog");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_odb(ರಚನೆ maintenance_ಸತತ_opts *opts,
				ರಚನೆ gc_config *cfg,
				ಪೂರ್ಣಾಂಕ keep_ಅತಿದೊಡ್ಡ_pack,
				ಪೂರ್ಣಾಂಕ aggressive)
{
	ರಚನೆ odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_ಅತಿದೊಡ್ಡ_pack = keep_ಅತಿದೊಡ್ಡ_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	ಆದರೆ (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	ಆದರೆ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	ಆದರೆ (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	ಹಿಂತಿರುಗಿ odb_optimize(the_repository->objects, &odb_opts);
}

ಪೂರ್ಣಾಂಕ cmd_gc(ಪೂರ್ಣಾಂಕ argc,
	   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ aggressive = 0;
	ಪೂರ್ಣಾಂಕ force = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	pid_t pid;
	ಪೂರ್ಣಾಂಕ daemonized = 0;
	ಪೂರ್ಣಾಂಕ keep_ಅತಿದೊಡ್ಡ_pack = -1;
	ಪೂರ್ಣಾಂಕ skip_foreground_tasks = 0;
	timestamp_t dummy;
	ರಚನೆ maintenance_ಸತತ_opts opts = MAINTENANCE_ಸತತ_OPTS_INIT;
	ರಚನೆ gc_config cfg = GC_CONFIG_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *prune_expire_sentinel = "sentinel";
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *prune_expire_arg = prune_expire_sentinel;
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.ಮೌಲ್ಯ = &prune_expire_arg,
			.argh = N_("date"),
			.help = N_("prune unreferenced objects"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)prune_expire_arg,
		},
		OPT_BOOL(0, "cruft", &cfg.cruft_packs, N_("pack unreferenced objects separately")),
		OPT_UNSIGNED(0, "max-cruft-size", &cfg.max_cruft_ಗಾತ್ರ,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL(0, "aggressive", &aggressive, N_("be more thorough (increased runtime)")),
		OPT_BOOL_F(0, "auto", &opts.auto_flag, N_("enable auto-gc mode"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform garbage collection in the background")),
		OPT_BOOL_F(0, "force", &force,
			   N_("force running gc even if there may be another gc running"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "keep-largest-pack", &keep_ಅತಿದೊಡ್ಡ_pack,
			 N_("repack all other packs except the largest pack")),
		OPT_STRING(0, "expire-to", &cfg.repack_expire_ಗಮ್ಯ, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_HIDDEN_BOOL(0, "skip-foreground-tasks", &skip_foreground_tasks,
			   N_("skip maintenance tasks typically done in the foreground")),
		OPT_ಕೊನೆ()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_gc_usage, builtin_gc_options);

	gc_config(&cfg);

	ಆದರೆ (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_ಸಮಯ))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	ಆದರೆ (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_gc_options,
			     builtin_gc_usage, 0);
	ಆದರೆ (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	ಆದರೆ (prune_expire_arg != prune_expire_sentinel) {
		ಬಿಡುಗಡೆ(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	ಆದರೆ (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	ಆದರೆ (opts.auto_flag) {
		ರಚನೆ odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		ಆದರೆ (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		ಆದರೆ (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    ಸತತ_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			ಹೋಗು ಹೊರಹೋಗು;
		}

		ಆದರೆ (!opts.quiet) {
			ಆದರೆ (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			ಇಲ್ಲದಿದ್ದರೆ
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	ಆದರೆ (opts.detach > 0) {
		ret = report_ಕೊನೆಯ_gc_error();
		ಆದರೆ (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			ಹೋಗು ಹೊರಹೋಗು;

		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ret) {
			/* an I/O error occurred, already reported */
			ಹೋಗು ಹೊರಹೋಗು;
		}

		ಆದರೆ (!skip_foreground_tasks) {
			ಆದರೆ (ಬೀಗ_repo_for_gc(force, &pid)) {
				ret = 0;
				ಹೋಗು ಹೊರಹೋಗು;
			}

			ಆದರೆ (gc_foreground_tasks(&opts, &cfg) < 0)
				die(ಖಾಲಿ);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = ಬೀಗ_repo_for_gc(force, &pid);
	ಆದರೆ (name) {
		ಆದರೆ (opts.auto_flag) {
			ret = 0;
			ಹೋಗು ಹೊರಹೋಗು; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	ಆದರೆ (daemonized) {
		ಅಕ್ಷರ *ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ(the_repository, "gc.log");
		hold_ಬೀಗ_file_for_update(&log_ಬೀಗ, ಮಾರ್ಗ,
					  ಬೀಗ_DIE_ON_ERROR);
		dup2(get_ಬೀಗ_file_fd(&log_ಬೀಗ), 2);
		atexit(process_log_file_at_exit);
		ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	}

	ಆದರೆ (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	ಆದರೆ (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_ಸತತ, "worktree");

	ಆದರೆ (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_ಸತತ, "rerere");

	ಆದರೆ (maintenance_task_odb(&opts, &cfg, keep_ಅತಿದೊಡ್ಡ_pack, aggressive))
		die(ಖಾಲಿ);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	ಆದರೆ (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	ಆದರೆ (the_repository->settings.gc_write_commit_ನಕ್ಷೆ == 1)
		write_commit_ನಕ್ಷೆ_reachable(the_repository,
					     the_repository->objects->ಮೂಲ->ಮಾರ್ಗ,
					     !opts.quiet && !daemonized ? COMMIT_ನಕ್ಷೆ_WRITE_ಪ್ರಗತಿ : 0,
					     ಖಾಲಿ);

	ಆದರೆ (!daemonized) {
		ಅಕ್ಷರ *ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ(the_repository, "gc.log");
		unlink(ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	}

ಹೊರಹೋಗು:
	maintenance_ಸತತ_opts_release(&opts);
	gc_config_release(&cfg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_maintenance_ಸತತ_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_opt_ವೇಳಾಪಟ್ಟಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				    ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ *ಆದ್ಯತೆ = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset)
		die(_("--no-schedule is not allowed"));

	*ಆದ್ಯತೆ = parse_ವೇಳಾಪಟ್ಟಿ(arg);

	ಆದರೆ (!*ಆದ್ಯತೆ)
		die(_("unrecognized --schedule argument '%s'"), arg);

	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ cg_auto_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ ಅಂಕೆ_not_ಒಳ_ನಕ್ಷೆ;
	ಪೂರ್ಣಾಂಕ ಎಲ್ಲೆ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dfs_on_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ cg_auto_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = (ರಚನೆ cg_auto_ದತ್ತಾಂಶ *)cb_ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *maybe_peeled = ref->oid;
	ರಚನೆ object_id peeled;
	ರಚನೆ commit_list *ರಾಶಿ = ಖಾಲಿ;
	ರಚನೆ commit *commit;

	ಆದರೆ (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	ಆದರೆ (odb_read_object_info(the_repository->objects, maybe_peeled, ಖಾಲಿ) != OBJ_COMMIT)
		ಹಿಂತಿರುಗಿ 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	ಆದರೆ (!commit || commit->object.flags & ನೋಡಿದ_2)
		ಹಿಂತಿರುಗಿ 0;
	commit->object.flags |= ನೋಡಿದ_2;

	ಆದರೆ (repo_parse_commit(the_repository, commit) ||
	    commit_ನಕ್ಷೆ_position(commit) != COMMIT_NOT_FROM_ನಕ್ಷೆ)
		ಹಿಂತಿರುಗಿ 0;

	ದತ್ತಾಂಶ->ಅಂಕೆ_not_ಒಳ_ನಕ್ಷೆ++;

	ಆದರೆ (ದತ್ತಾಂಶ->ಅಂಕೆ_not_ಒಳ_ನಕ್ಷೆ >= ದತ್ತಾಂಶ->ಎಲ್ಲೆ)
		ಹಿಂತಿರುಗಿ 1;

	commit_list_ಸೇರಿಸು(commit, &ರಾಶಿ);

	ಆಗ (!ಫಲಿತಾಂಶ && ರಾಶಿ) {
		ರಚನೆ commit_list *ಪೋಷಕ;

		commit = ತೆಗೆ_commit(&ರಾಶಿ);

		ಗಾಗಿ (ಪೋಷಕ = commit->parents; ಪೋಷಕ; ಪೋಷಕ = ಪೋಷಕ->ಮುಂದಿನ) {
			ಆದರೆ (repo_parse_commit(the_repository, ಪೋಷಕ->ಅಂಶ) ||
			    commit_ನಕ್ಷೆ_position(ಪೋಷಕ->ಅಂಶ) != COMMIT_NOT_FROM_ನಕ್ಷೆ ||
			    ಪೋಷಕ->ಅಂಶ->object.flags & ನೋಡಿದ_2)
				ಮುಂದುವರಿ;

			ಪೋಷಕ->ಅಂಶ->object.flags |= ನೋಡಿದ_2;
			ದತ್ತಾಂಶ->ಅಂಕೆ_not_ಒಳ_ನಕ್ಷೆ++;

			ಆದರೆ (ದತ್ತಾಂಶ->ಅಂಕೆ_not_ಒಳ_ನಕ್ಷೆ >= ದತ್ತಾಂಶ->ಎಲ್ಲೆ) {
				ಫಲಿತಾಂಶ = 1;
				ನಿಲ್ಲಿಸು;
			}

			commit_list_ಸೇರಿಸು(ಪೋಷಕ->ಅಂಶ, &ರಾಶಿ);
		}
	}

	commit_list_free(ರಾಶಿ);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ should_write_commit_ನಕ್ಷೆ(ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
	ರಚನೆ cg_auto_ದತ್ತಾಂಶ ದತ್ತಾಂಶ;

	ದತ್ತಾಂಶ.ಅಂಕೆ_not_ಒಳ_ನಕ್ಷೆ = 0;
	ದತ್ತಾಂಶ.ಎಲ್ಲೆ = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &ದತ್ತಾಂಶ.ಎಲ್ಲೆ);

	ಆದರೆ (!ದತ್ತಾಂಶ.ಎಲ್ಲೆ)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (ದತ್ತಾಂಶ.ಎಲ್ಲೆ < 0)
		ಹಿಂತಿರುಗಿ 1;

	ಫಲಿತಾಂಶ = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &ದತ್ತಾಂಶ);

	repo_clear_commit_marks(the_repository, ನೋಡಿದ_2);

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_write_commit_ನಕ್ಷೆ(ರಚನೆ maintenance_ಸತತ_opts *opts)
{
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	ಮಗು.git_cmd = 1;
	ಮಗು.odb_ಗಮ್ಯ_close = the_repository->objects;
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "commit-graph", "write",
		     "--split", "--reachable", ಖಾಲಿ);

	ಆದರೆ (opts->quiet)
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--no-progress");
	ಇಲ್ಲದಿದ್ದರೆ
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--progress");

	ಹಿಂತಿರುಗಿ !!ಸತತ_command(&ಮಗು);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_commit_ನಕ್ಷೆ(ರಚನೆ maintenance_ಸತತ_opts *opts,
					 ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	prepare_repo_settings(the_repository);
	ಆದರೆ (!the_repository->settings.core_commit_ನಕ್ಷೆ)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (ಸತತ_write_commit_ನಕ್ಷೆ(opts)) {
		error(_("failed to write commit-graph"));
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fetch_remote(ರಚನೆ remote *remote, ಶೂನ್ಯ *cbdata)
{
	ರಚನೆ maintenance_ಸತತ_opts *opts = cbdata;
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	ಆದರೆ (remote->skip_default_update)
		ಹಿಂತಿರುಗಿ 0;

	ಮಗು.git_cmd = 1;
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     ಖಾಲಿ);

	ಆದರೆ (opts->quiet)
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--quiet");

	ಹಿಂತಿರುಗಿ !!ಸತತ_command(&ಮಗು);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_prefetch(ರಚನೆ maintenance_ಸತತ_opts *opts,
				     ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	ಆದರೆ (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_gc_foreground(ರಚನೆ maintenance_ಸತತ_opts *opts,
					  ರಚನೆ gc_config *cfg)
{
	ಹಿಂತಿರುಗಿ gc_foreground_tasks(opts, cfg);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_gc_background(ರಚನೆ maintenance_ಸತತ_opts *opts,
					  ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	ಮಗು.git_cmd = 1;
	ಮಗು.odb_ಗಮ್ಯ_close = the_repository->objects;
	strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "gc");

	ಆದರೆ (opts->auto_flag)
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--auto");
	ಆದರೆ (opts->quiet)
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--quiet");
	ಇಲ್ಲದಿದ್ದರೆ
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--no-quiet");
	strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--no-detach");
	strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--skip-foreground-tasks");

	ಹಿಂತಿರುಗಿ ಸತತ_command(&ಮಗು);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ gc_condition(ರಚನೆ gc_config *cfg)
{
	ರಚನೆ odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	ಹಿಂತಿರುಗಿ odb_optimize_required(the_repository->objects, &opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prune_packed(ರಚನೆ maintenance_ಸತತ_opts *opts)
{
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	ಮಗು.git_cmd = 1;
	strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "prune-packed");

	ಆದರೆ (opts->quiet)
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--quiet");

	ಹಿಂತಿರುಗಿ !!ಸತತ_command(&ಮಗು);
}

ರಚನೆ write_loose_object_ದತ್ತಾಂಶ {
	FILE *ಒಳ;
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2;
	ಪೂರ್ಣಾಂಕ batch_ಗಾತ್ರ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ loose_object_auto_ಎಲ್ಲೆ = 100;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ loose_object_ಎಣಿಕೆ(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid ಬಳಸದ,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ ಬಳಸದ,
			      ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ *ಎಣಿಕೆ_2 = (ಪೂರ್ಣಾಂಕ*)ದತ್ತಾಂಶ;
	ಆದರೆ (++(*ಎಣಿಕೆ_2) >= loose_object_auto_ಎಲ್ಲೆ)
		ಹಿಂತಿರುಗಿ 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ loose_object_auto_condition(ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_ಎಲ್ಲೆ);

	ಆದರೆ (!loose_object_auto_ಎಲ್ಲೆ)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (loose_object_auto_ಎಲ್ಲೆ < 0)
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ for_each_loose_file_ಒಳ_ಮೂಲ(the_repository->objects->ಮೂಲ,
					     loose_object_ಎಣಿಕೆ,
					     ಖಾಲಿ, ಖಾಲಿ, &ಎಣಿಕೆ_2);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bail_on_loose(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid ಬಳಸದ,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ ಬಳಸದ,
			 ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_loose_object_ಗಮ್ಯ_stdin(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ ಬಳಸದ,
				       ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ write_loose_object_ದತ್ತಾಂಶ *d = (ರಚನೆ write_loose_object_ದತ್ತಾಂಶ *)ದತ್ತಾಂಶ;

	fprintf(d->ಒಳ, "%s\n", oid_ಗಮ್ಯ_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	ಹಿಂತಿರುಗಿ ++(d->ಎಣಿಕೆ_2) > d->batch_ಗಾತ್ರ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pack_loose(ರಚನೆ maintenance_ಸತತ_opts *opts)
{
	ರಚನೆ repository *r = the_repository;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ರಚನೆ write_loose_object_ದತ್ತಾಂಶ ದತ್ತಾಂಶ;
	ರಚನೆ ಮಗು_process pack_proc = ಮಗು_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	ಆದರೆ (!for_each_loose_file_ಒಳ_ಮೂಲ(r->objects->ಮೂಲ,
					   bail_on_loose,
					   ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ 0;

	pack_proc.git_cmd = 1;

	strvec_ತಳ್ಳು(&pack_proc.ವಾದಗಳು, "pack-objects");
	ಆದರೆ (opts->quiet)
		strvec_ತಳ್ಳು(&pack_proc.ವಾದಗಳು, "--quiet");
	ಇಲ್ಲದಿದ್ದರೆ
		strvec_ತಳ್ಳು(&pack_proc.ವಾದಗಳು, "--no-quiet");
	strvec_pushf(&pack_proc.ವಾದಗಳು, "%s/pack/loose", r->objects->ಮೂಲ->ಮಾರ್ಗ);

	pack_proc.ಒಳ = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.ಹೊರಹೋಗು = -1;

	ಆದರೆ (ಆರಂಭ_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		ಹಿಂತಿರುಗಿ 1;
	}

	ದತ್ತಾಂಶ.ಒಳ = xfdopen(pack_proc.ಒಳ, "w");
	ದತ್ತಾಂಶ.ಎಣಿಕೆ_2 = 0;
	ದತ್ತಾಂಶ.batch_ಗಾತ್ರ = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &ದತ್ತಾಂಶ.batch_ಗಾತ್ರ);

	/* If configured as 0, then remove limit. */
	ಆದರೆ (!ದತ್ತಾಂಶ.batch_ಗಾತ್ರ)
		ದತ್ತಾಂಶ.batch_ಗಾತ್ರ = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ದತ್ತಾಂಶ.batch_ಗಾತ್ರ > 0)
		ದತ್ತಾಂಶ.batch_ಗಾತ್ರ--; /* Decrease for equality on limit. */

	for_each_loose_file_ಒಳ_ಮೂಲ(r->objects->ಮೂಲ,
				      write_loose_object_ಗಮ್ಯ_stdin,
				      ಖಾಲಿ, ಖಾಲಿ, &ದತ್ತಾಂಶ);

	fclose(ದತ್ತಾಂಶ.ಒಳ);

	ಆದರೆ (ಮುಕ್ತಾಯ_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		ಫಲಿತಾಂಶ = 1;
	}

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_loose_objects(ರಚನೆ maintenance_ಸತತ_opts *opts,
					  ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ prune_packed(opts) || pack_loose(opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ incremental_repack_auto_condition(ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	ರಚನೆ packed_git *p;
	ಪೂರ್ಣಾಂಕ incremental_repack_auto_ಎಲ್ಲೆ = 10;
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;

	prepare_repo_settings(the_repository);
	ಆದರೆ (!the_repository->settings.core_multi_pack_ಸೂಚ್ಯಂಕ)
		ಹಿಂತಿರುಗಿ 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_ಎಲ್ಲೆ);

	ಆದರೆ (!incremental_repack_auto_ಎಲ್ಲೆ)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (incremental_repack_auto_ಎಲ್ಲೆ < 0)
		ಹಿಂತಿರುಗಿ 1;

	repo_for_each_pack(the_repository, p) {
		ಆದರೆ (ಎಣಿಕೆ_2 >= incremental_repack_auto_ಎಲ್ಲೆ)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (!p->multi_pack_ಸೂಚ್ಯಂಕ)
			ಎಣಿಕೆ_2++;
	}

	ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2 >= incremental_repack_auto_ಎಲ್ಲೆ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ multi_pack_ಸೂಚ್ಯಂಕ_write(ರಚನೆ maintenance_ಸತತ_opts *opts)
{
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	ಮಗು.git_cmd = 1;
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "multi-pack-index", "write", ಖಾಲಿ);

	ಆದರೆ (opts->quiet)
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--no-progress");
	ಇಲ್ಲದಿದ್ದರೆ
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--progress");

	ಆದರೆ (ಸತತ_command(&ಮಗು))
		ಹಿಂತಿರುಗಿ error(_("failed to write multi-pack-index"));

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ multi_pack_ಸೂಚ್ಯಂಕ_expire(ರಚನೆ maintenance_ಸತತ_opts *opts)
{
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	ಮಗು.git_cmd = 1;
	ಮಗು.odb_ಗಮ್ಯ_close = the_repository->objects;
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "multi-pack-index", "expire", ಖಾಲಿ);

	ಆದರೆ (opts->quiet)
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--no-progress");
	ಇಲ್ಲದಿದ್ದರೆ
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--progress");

	ಆದರೆ (ಸತತ_command(&ಮಗು))
		ಹಿಂತಿರುಗಿ error(_("'git multi-pack-index expire' failed"));

	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು TWO_GIGABYTES (INT32_MAX)

ಸ್ಥಿರ off_t get_auto_pack_ಗಾತ್ರ(ಶೂನ್ಯ)
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
	off_t max_ಗಾತ್ರ = 0;
	off_t ಎರಡನೇ_ಅತಿದೊಡ್ಡ_ಗಾತ್ರ = 0;
	off_t ಫಲಿತಾಂಶ_ಗಾತ್ರ;
	ರಚನೆ packed_git *p;
	ರಚನೆ repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		ಆದರೆ (p->pack_ಗಾತ್ರ > max_ಗಾತ್ರ) {
			ಎರಡನೇ_ಅತಿದೊಡ್ಡ_ಗಾತ್ರ = max_ಗಾತ್ರ;
			max_ಗಾತ್ರ = p->pack_ಗಾತ್ರ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p->pack_ಗಾತ್ರ > ಎರಡನೇ_ಅತಿದೊಡ್ಡ_ಗಾತ್ರ)
			ಎರಡನೇ_ಅತಿದೊಡ್ಡ_ಗಾತ್ರ = p->pack_ಗಾತ್ರ;
	}

	ಫಲಿತಾಂಶ_ಗಾತ್ರ = ಎರಡನೇ_ಅತಿದೊಡ್ಡ_ಗಾತ್ರ + 1;

	/* But limit ourselves to a batch size of 2g */
	ಆದರೆ (ಫಲಿತಾಂಶ_ಗಾತ್ರ > TWO_GIGABYTES)
		ಫಲಿತಾಂಶ_ಗಾತ್ರ = TWO_GIGABYTES;

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ_ಗಾತ್ರ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ multi_pack_ಸೂಚ್ಯಂಕ_repack(ರಚನೆ maintenance_ಸತತ_opts *opts)
{
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	ಮಗು.git_cmd = 1;
	ಮಗು.odb_ಗಮ್ಯ_close = the_repository->objects;
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "multi-pack-index", "repack", ಖಾಲಿ);

	ಆದರೆ (opts->quiet)
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--no-progress");
	ಇಲ್ಲದಿದ್ದರೆ
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--progress");

	strvec_pushf(&ಮಗು.ವಾದಗಳು, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_ಗಾತ್ರ());

	ಆದರೆ (ಸತತ_command(&ಮಗು))
		ಹಿಂತಿರುಗಿ error(_("'git multi-pack-index repack' failed"));

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_incremental_repack(ರಚನೆ maintenance_ಸತತ_opts *opts,
					       ರಚನೆ gc_config *cfg ಬಳಸದ)
{
	prepare_repo_settings(the_repository);
	ಆದರೆ (!the_repository->settings.core_multi_pack_ಸೂಚ್ಯಂಕ) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (multi_pack_ಸೂಚ್ಯಂಕ_write(opts))
		ಹಿಂತಿರುಗಿ 1;
	ಆದರೆ (multi_pack_ಸೂಚ್ಯಂಕ_expire(opts))
		ಹಿಂತಿರುಗಿ 1;
	ಆದರೆ (multi_pack_ಸೂಚ್ಯಂಕ_repack(opts))
		ಹಿಂತಿರುಗಿ 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_task_geometric_repack(ರಚನೆ maintenance_ಸತತ_opts *opts,
					     ರಚನೆ gc_config *cfg)
{
	ರಚನೆ odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	ಆದರೆ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	ಹಿಂತಿರುಗಿ odb_optimize(the_repository->objects, &odb_opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ geometric_repack_auto_condition(ರಚನೆ gc_config *cfg)
{
	ರಚನೆ odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	ಹಿಂತಿರುಗಿ odb_optimize_required(the_repository->objects, &opts);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*maintenance_task_fn)(ರಚನೆ maintenance_ಸತತ_opts *opts,
				   ರಚನೆ gc_config *cfg);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*maintenance_auto_fn)(ರಚನೆ gc_config *cfg);

ರಚನೆ maintenance_task {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;

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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ maintenance_task tasks[] = {
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
	[TASK_COMMIT_ನಕ್ಷೆ] = {
		.name = "commit-graph",
		.background = maintenance_task_commit_ನಕ್ಷೆ,
		.auto_condition = should_write_commit_ನಕ್ಷೆ,
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

ಎಣಿಕೆ task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

ಎಣಿಕೆ auto_gc_hook_ಫಲಿತಾಂಶ {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_ಸತತ = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maybe_ಸತತ_task(ಸ್ಥಿರಾಂಕ ರಚನೆ maintenance_task *task,
			  ರಚನೆ repository *repo,
			  ರಚನೆ maintenance_ಸತತ_opts *opts,
			  ರಚನೆ gc_config *cfg,
			  ಎಣಿಕೆ task_phase phase,
			  ಎಣಿಕೆ auto_gc_hook_ಫಲಿತಾಂಶ *auto_gc_hook_ಫಲಿತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *region = foreground ? "maintenance foreground" : "maintenance";
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಆದರೆ (!fn)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (opts->auto_flag) {
		ಆದರೆ (*auto_gc_hook_ಫಲಿತಾಂಶ == AUTO_GC_HOOK_SKIP)
			ಹಿಂತಿರುಗಿ 0;

		ಆದರೆ (!task->auto_condition || !task->auto_condition(cfg))
			ಹಿಂತಿರುಗಿ 0;

		ಆದರೆ (*auto_gc_hook_ಫಲಿತಾಂಶ == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_ಫಲಿತಾಂಶ = ಸತತ_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_ಸತತ;
		ಆದರೆ (*auto_gc_hook_ಫಲಿತಾಂಶ == AUTO_GC_HOOK_SKIP)
			ಹಿಂತಿರುಗಿ 0;
	}

	trace2_region_enter(region, task->name, repo);
	ಆದರೆ (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_ಸತತ_tasks(ರಚನೆ maintenance_ಸತತ_opts *opts,
				 ರಚನೆ gc_config *cfg)
{
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ರಚನೆ ಬೀಗ_file lk;
	ರಚನೆ repository *r = the_repository;
	ಅಕ್ಷರ *ಬೀಗ_ಮಾರ್ಗ = xstrfmt("%s/maintenance", r->objects->ಮೂಲ->ಮಾರ್ಗ);
	ಎಣಿಕೆ auto_gc_hook_ಫಲಿತಾಂಶ auto_gc_hook_ಫಲಿತಾಂಶ = AUTO_GC_HOOK_UNDECIDED;

	ಆದರೆ (repo_hold_ಬೀಗ_file_for_update(r, &lk, ಬೀಗ_ಮಾರ್ಗ, ಬೀಗ_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		ಆದರೆ (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				ಬೀಗ_ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(ಬೀಗ_ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಬಿಡುಗಡೆ(ಬೀಗ_ಮಾರ್ಗ);

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < opts->tasks_nr; i++)
		ಆದರೆ (maybe_ಸತತ_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_ಫಲಿತಾಂಶ))
			ಫಲಿತಾಂಶ = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	ಆದರೆ (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < opts->tasks_nr; i++)
		ಆದರೆ (maybe_ಸತತ_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_ಫಲಿತಾಂಶ))
			ಫಲಿತಾಂಶ = 1;

	rollback_ಬೀಗ_file(&lk);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಎಣಿಕೆ maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

ರಚನೆ maintenance_strategy {
	ರಚನೆ {
		ಚಿಹ್ನೆರಹಿತ type;
		ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ ವೇಳಾಪಟ್ಟಿ;
	} tasks[TASK_ಎಣಿಕೆ];
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ maintenance_strategy none_strategy = { 0 };

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_DAILY,
		},
	},
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ maintenance_strategy incremental_strategy = {
	.tasks = {
		[TASK_COMMIT_ನಕ್ಷೆ] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_HOURLY,
		},
		[TASK_PREFETCH] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_HOURLY,
		},
		[TASK_INCREMENTAL_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_DAILY,
		},
		[TASK_LOOSE_OBJECTS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_WEEKLY,
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ maintenance_strategy geometric_strategy = {
	.tasks = {
		[TASK_COMMIT_ನಕ್ಷೆ] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_HOURLY,
		},
		[TASK_GEOMETRIC_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_DAILY,
		},
		[TASK_RERERE_GC] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_WEEKLY,
		},
		[TASK_REFLOG_EXPIRE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_WEEKLY,
		},
		[TASK_WORKTREE_PRUNE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ವೇಳಾಪಟ್ಟಿ = ವೇಳಾಪಟ್ಟಿ_WEEKLY,
		},
	},
};

ಸ್ಥಿರ ರಚನೆ maintenance_strategy parse_maintenance_strategy(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಆದರೆ (!strcasecmp(name, "incremental"))
		ಹಿಂತಿರುಗಿ incremental_strategy;
	ಆದರೆ (!strcasecmp(name, "gc"))
		ಹಿಂತಿರುಗಿ gc_strategy;
	ಆದರೆ (!strcasecmp(name, "geometric"))
		ಹಿಂತಿರುಗಿ geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

ಸ್ಥಿರ ಶೂನ್ಯ initialize_task_config(ರಚನೆ maintenance_ಸತತ_opts *opts,
				   ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *selected_tasks)
{
	ರಚನೆ strbuf config_name = STRBUF_INIT;
	ರಚನೆ maintenance_strategy strategy;
	ಎಣಿಕೆ maintenance_type type;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	ಆದರೆ (selected_tasks->nr) {
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < selected_tasks->nr; i++) {
			ಎಣಿಕೆ maintenance_task_label label = (intptr_t)selected_tasks->ವಸ್ತುಗಳು[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		ಹಿಂತಿರುಗಿ;
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
	ಆದರೆ (opts->ವೇಳಾಪಟ್ಟಿ) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} ಇಲ್ಲದಿದ್ದರೆ {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	ಆದರೆ (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < TASK_ಎಣಿಕೆ; i++) {
		ಪೂರ್ಣಾಂಕ config_ಮೌಲ್ಯ;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		ಆದರೆ (!repo_config_get_bool(the_repository, config_name.buf, &config_ಮೌಲ್ಯ))
			strategy.tasks[i].type = config_ಮೌಲ್ಯ ? type : 0;
		ಆದರೆ (!(strategy.tasks[i].type & type))
			ಮುಂದುವರಿ;

		ಆದರೆ (opts->ವೇಳಾಪಟ್ಟಿ) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			ಆದರೆ (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].ವೇಳಾಪಟ್ಟಿ = parse_ವೇಳಾಪಟ್ಟಿ(config_str);
			ಆದರೆ (strategy.tasks[i].ವೇಳಾಪಟ್ಟಿ < opts->ವೇಳಾಪಟ್ಟಿ)
				ಮುಂದುವರಿ;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ task_option_parse(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ string_list *selected_tasks = opt->ಮೌಲ್ಯ;
	ಗಾತ್ರ_t i;

	BUG_ON_OPT_NEG(unset);

	ಗಾಗಿ (i = 0; i < TASK_ಎಣಿಕೆ; i++)
		ಆದರೆ (!strcasecmp(tasks[i].name, arg))
			ನಿಲ್ಲಿಸು;
	ಆದರೆ (i >= TASK_ಎಣಿಕೆ) {
		error(_("'%s' is not a valid task"), arg);
		ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		ಹಿಂತಿರುಗಿ 1;
	}

	string_list_append(selected_tasks, arg)->util = (ಶೂನ್ಯ *)(intptr_t)i;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_ಸತತ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ maintenance_ಸತತ_opts opts = MAINTENANCE_ಸತತ_OPTS_INIT;
	ರಚನೆ string_list selected_tasks = STRING_LIST_INIT_DUP;
	ರಚನೆ gc_config cfg = GC_CONFIG_INIT;
	ರಚನೆ option builtin_maintenance_ಸತತ_options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform maintenance in the background")),
		OPT_CALLBACK(0, "schedule", &opts.ವೇಳಾಪಟ್ಟಿ, N_("frequency"),
			     N_("run tasks based on frequency"),
			     maintenance_opt_ವೇಳಾಪಟ್ಟಿ),
		OPT_BOOL(0, "quiet", &opts.quiet,
			 N_("do not report progress or other information over stderr")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			N_("run a specific task"),
			PARSE_OPT_NONEG, task_option_parse),
		OPT_ಕೊನೆ()
	};
	ಪೂರ್ಣಾಂಕ ret;

	opts.quiet = !isatty(2);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_maintenance_ಸತತ_options,
			     builtin_maintenance_ಸತತ_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	die_for_incompatible_opt2(opts.auto_flag, "--auto",
				  opts.ವೇಳಾಪಟ್ಟಿ, "--schedule=");
	die_for_incompatible_opt2(selected_tasks.nr, "--task=",
				  opts.ವೇಳಾಪಟ್ಟಿ, "--schedule=");

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	ಆದರೆ (argc != 0)
		usage_with_options(builtin_maintenance_ಸತತ_usage,
				   builtin_maintenance_ಸತತ_options);

	ret = maintenance_ಸತತ_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_ಸತತ_opts_release(&opts);
	gc_config_release(&cfg);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಅಕ್ಷರ *get_maintpath(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	ಹಿಂತಿರುಗಿ strbuf_detach(&sb, ಖಾಲಿ);
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_maintenance_ರಿಜಿಸ್ಟರ್_usage[] = {
	"git maintenance register [--config-file <path>]",
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_ರಿಜಿಸ್ಟರ್(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				ರಚನೆ repository *repo ಬಳಸದ)
{
	ಅಕ್ಷರ *config_file = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_ಕೊನೆ(),
	};
	ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು_2 = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ = "maintenance.repo";
	ಅಕ್ಷರ *maintpath = get_maintpath();
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *list;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_maintenance_ರಿಜಿಸ್ಟರ್_usage, 0);
	ಆದರೆ (argc)
		usage_with_options(builtin_maintenance_ರಿಜಿಸ್ಟರ್_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_ಗಣ(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	ಆದರೆ (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_ಗಣ(the_repository, "maintenance.strategy", "incremental");

	ಆದರೆ (!repo_config_get_string_multi(the_repository, ಕೀಲಿ, &list)) {
		for_each_string_list_ಅಂಶ(ಅಂಶ, list) {
			ಆದರೆ (!strcmp(maintpath, ಅಂಶ->string)) {
				ಸಿಕ್ಕಿತು_2 = 1;
				ನಿಲ್ಲಿಸು;
			}
		}
	}

	ಆದರೆ (!ಸಿಕ್ಕಿತು_2) {
		ಪೂರ್ಣಾಂಕ rc;
		ಅಕ್ಷರ *global_config_file = ಖಾಲಿ;

		ಆದರೆ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ಆದರೆ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_ಗಣ_multivar_ಒಳ_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, ಖಾಲಿ, 0);
		ಬಿಡುಗಡೆ(global_config_file);

		ಆದರೆ (rc)
			die(_("unable to add '%s' value of '%s'"),
			    ಕೀಲಿ, maintpath);
	}

	ಬಿಡುಗಡೆ(maintpath);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_unregister(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				  ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ force = 0;
	ಅಕ್ಷರ *config_file = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_ಕೊನೆ(),
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ = "maintenance.repo";
	ಅಕ್ಷರ *maintpath = get_maintpath();
	ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು_2 = 0;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *list;
	ರಚನೆ config_ಗಣ cs = { { 0 } };

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_maintenance_unregister_usage, 0);
	ಆದರೆ (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	ಆದರೆ (config_file) {
		git_configset_init(&cs);
		git_configset_ಕೂಡಿಸು_file(&cs, config_file);
	}
	ಆದರೆ (!(config_file
	      ? git_configset_get_string_multi(&cs, ಕೀಲಿ, &list)
	      : repo_config_get_string_multi(the_repository, ಕೀಲಿ, &list))) {
		for_each_string_list_ಅಂಶ(ಅಂಶ, list) {
			ಆದರೆ (!strcmp(maintpath, ಅಂಶ->string)) {
				ಸಿಕ್ಕಿತು_2 = 1;
				ನಿಲ್ಲಿಸು;
			}
		}
	}

	ಆದರೆ (ಸಿಕ್ಕಿತು_2) {
		ಪೂರ್ಣಾಂಕ rc;
		ಅಕ್ಷರ *global_config_file = ಖಾಲಿ;

		ಆದರೆ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ಆದರೆ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_ಗಣ_multivar_ಒಳ_file_gently(the_repository,
			config_file, ಕೀಲಿ, ಖಾಲಿ, maintpath, ಖಾಲಿ,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ);
		ಬಿಡುಗಡೆ(global_config_file);

		ಆದರೆ (rc &&
		    (!force || rc == CONFIG_NOTHING_ಗಣ))
			die(_("unable to unset '%s' value of '%s'"),
			    ಕೀಲಿ, maintpath);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	ಬಿಡುಗಡೆ(maintpath);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_frequency(ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ ವೇಳಾಪಟ್ಟಿ)
{
	ಆಯ್ಕೆ (ವೇಳಾಪಟ್ಟಿ) {
	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_HOURLY:
		ಹಿಂತಿರುಗಿ "hourly";
	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_DAILY:
		ಹಿಂತಿರುಗಿ "daily";
	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_WEEKLY:
		ಹಿಂತಿರುಗಿ "weekly";
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid schedule %d", ವೇಳಾಪಟ್ಟಿ);
	}
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_extra_config_parameters(ಶೂನ್ಯ) {
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಫಲಿತಾಂಶ = ಖಾಲಿ;
	ರಚನೆ strbuf builder = STRBUF_INIT;

	ಆದರೆ (ಫಲಿತಾಂಶ)
		ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;

	ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	ಫಲಿತಾಂಶ = strbuf_detach(&builder, ಖಾಲಿ);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_extra_launchctl_ಅಕ್ಷರಸರಣಿಗಳು(ಶೂನ್ಯ) {
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಫಲಿತಾಂಶ = ಖಾಲಿ;
	ರಚನೆ strbuf builder = STRBUF_INIT;

	ಆದರೆ (ಫಲಿತಾಂಶ)
		ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;

	ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	ಫಲಿತಾಂಶ = strbuf_detach(&builder, ಖಾಲಿ);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ವೇಳಾಪಟ್ಟಿ_cmd(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd, ಪೂರ್ಣಾಂಕ *is_ಲಭ್ಯ, ಅಕ್ಷರ **ಹೊರಹೋಗು)
{
	ಅಕ್ಷರ *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ string_list list = STRING_LIST_INIT_NODUP;

	ಆದರೆ (!testing) {
		ಆದರೆ (ಹೊರಹೋಗು)
			*ಹೊರಹೋಗು = xstrdup(cmd);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (is_ಲಭ್ಯ)
		*is_ಲಭ್ಯ = 0;

	string_list_ವಿಭಜಿಸು_ಒಳ_ಇರಿಸು(&list, testing, ",", -1);
	for_each_string_list_ಅಂಶ(ಅಂಶ, &list) {
		ರಚನೆ string_list pair = STRING_LIST_INIT_NODUP;

		ಆದರೆ (string_list_ವಿಭಜಿಸು_ಒಳ_ಇರಿಸು(&pair, ಅಂಶ->string, ":", 2) != 2)
			ಮುಂದುವರಿ;

		ಆದರೆ (!strcmp(cmd, pair.ವಸ್ತುಗಳು[0].string)) {
			ಆದರೆ (ಹೊರಹೋಗು)
				*ಹೊರಹೋಗು = xstrdup(pair.ವಸ್ತುಗಳು[1].string);
			ಆದರೆ (is_ಲಭ್ಯ)
				*is_ಲಭ್ಯ = 1;
			string_list_clear(&pair, 0);
			ಹೋಗು ಹೊರಹೋಗು;
		}

		string_list_clear(&pair, 0);
	}

	ಆದರೆ (ಹೊರಹೋಗು)
		*ಹೊರಹೋಗು = xstrdup(cmd);

ಹೊರಹೋಗು:
	string_list_clear(&list, 0);
	ಬಿಡುಗಡೆ(testing);
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_random_minute(ಶೂನ್ಯ)
{
	/* Use a static value when under tests. */
	ಆದರೆ (getenv("GIT_TEST_MAINT_SCHEDULER"))
		ಹಿಂತಿರುಗಿ 13;

	ಹಿಂತಿರುಗಿ git_rand(0) % 60;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_launchctl_ಲಭ್ಯ(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ is_ಲಭ್ಯ;
	ಆದರೆ (get_ವೇಳಾಪಟ್ಟಿ_cmd("launchctl", &is_ಲಭ್ಯ, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ is_ಲಭ್ಯ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
	ಹಿಂತಿರುಗಿ 1;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಅಕ್ಷರ *launchctl_service_name(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *frequency)
{
	ರಚನೆ strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	ಹಿಂತಿರುಗಿ strbuf_detach(&label, ಖಾಲಿ);
}

ಸ್ಥಿರ ಅಕ್ಷರ *launchctl_service_filename(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಅಕ್ಷರ *expanded;
	ರಚನೆ strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_ಮಾರ್ಗ(filename.buf, 1);
	ಆದರೆ (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	ಹಿಂತಿರುಗಿ expanded;
}

ಸ್ಥಿರ ಅಕ್ಷರ *launchctl_get_uid(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ xstrfmt("gui/%d", getuid());
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ launchctl_boot_plist(ಪೂರ್ಣಾಂಕ enable, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename)
{
	ಅಕ್ಷರ *cmd;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;
	ಅಕ್ಷರ *uid = launchctl_get_uid();

	get_ವೇಳಾಪಟ್ಟಿ_cmd("launchctl", ಖಾಲಿ, &cmd);
	strvec_ವಿಭಜಿಸು(&ಮಗು.ವಾದಗಳು, cmd);
	strvec_pushl(&ಮಗು.ವಾದಗಳು, enable ? "bootstrap" : "bootout", uid,
		     filename, ಖಾಲಿ);

	ಮಗು.no_stderr = 1;
	ಮಗು.no_stdout = 1;

	ಆದರೆ (ಆರಂಭ_command(&ಮಗು))
		die(_("failed to start launchctl"));

	ಫಲಿತಾಂಶ = ಮುಕ್ತಾಯ_command(&ಮಗು);

	ಬಿಡುಗಡೆ(cmd);
	ಬಿಡುಗಡೆ(uid);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ launchctl_remove_plist(ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ ವೇಳಾಪಟ್ಟಿ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *frequency = get_frequency(ವೇಳಾಪಟ್ಟಿ);
	ಅಕ್ಷರ *name = launchctl_service_name(frequency);
	ಅಕ್ಷರ *filename = launchctl_service_filename(name);
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = launchctl_boot_plist(0, filename);
	unlink(filename);
	ಬಿಡುಗಡೆ(filename);
	ಬಿಡುಗಡೆ(name);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ launchctl_remove_plists(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ launchctl_remove_plist(ವೇಳಾಪಟ್ಟಿ_HOURLY) ||
	       launchctl_remove_plist(ವೇಳಾಪಟ್ಟಿ_DAILY) ||
	       launchctl_remove_plist(ವೇಳಾಪಟ್ಟಿ_WEEKLY);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ launchctl_list_ಒಳಗೊಂಡಿದೆ_plist(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd)
{
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	strvec_ವಿಭಜಿಸು(&ಮಗು.ವಾದಗಳು, cmd);
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "list", name, ಖಾಲಿ);

	ಮಗು.no_stderr = 1;
	ಮಗು.no_stdout = 1;

	ಆದರೆ (ಆರಂಭ_command(&ಮಗು))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	ಹಿಂತಿರುಗಿ !ಮುಕ್ತಾಯ_command(&ಮಗು);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ launchctl_ವೇಳಾಪಟ್ಟಿ_plist(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exec_ಮಾರ್ಗ, ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ ವೇಳಾಪಟ್ಟಿ)
{
	ಪೂರ್ಣಾಂಕ i, fd;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *preamble, *repeat;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *frequency = get_frequency(ವೇಳಾಪಟ್ಟಿ);
	ಅಕ್ಷರ *name = launchctl_service_name(frequency);
	ಅಕ್ಷರ *filename = launchctl_service_filename(name);
	ರಚನೆ ಬೀಗ_file lk = ಬೀಗ_INIT;
	ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಬೀಗ_file_timeout_ms = ULONG_MAX;
	ರಚನೆ strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	ರಚನೆ stat st;
	ಅಕ್ಷರ *cmd;
	ಪೂರ್ಣಾಂಕ minute = get_random_minute();

	get_ವೇಳಾಪಟ್ಟಿ_cmd("launchctl", ಖಾಲಿ, &cmd);
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
	strbuf_addf(&plist, preamble, name, exec_ಮಾರ್ಗ, exec_ಮಾರ್ಗ,
		    get_extra_launchctl_ಅಕ್ಷರಸರಣಿಗಳು(), frequency);

	ಆಯ್ಕೆ (ವೇಳಾಪಟ್ಟಿ) {
	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		ಗಾಗಿ (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		ಗಾಗಿ (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		ನಿಲ್ಲಿಸು;

	ಪೂರ್ವನಿಯೋಜಿತ:
		/* unreachable */
		ನಿಲ್ಲಿಸು;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	ಆದರೆ ((ದೀರ್ಘ)ಬೀಗ_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &ಬೀಗ_file_timeout_ms))
		ಬೀಗ_file_timeout_ms = 150;

	fd = hold_ಬೀಗ_file_for_update_timeout(&lk, filename, ಬೀಗ_DIE_ON_ERROR,
					       ಬೀಗ_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	ಆದರೆ (!stat(filename, &st) && st.st_ಗಾತ್ರ == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_ಒಳಗೊಂಡಿದೆ_plist(name, cmd))
		rollback_ಬೀಗ_file(&lk);
	ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (write_ಒಳ_ತುಂಬಿದ(fd, plist.buf, plist.len) < 0 ||
		    commit_ಬೀಗ_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		ಆದರೆ (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	ಬಿಡುಗಡೆ(filename);
	ಬಿಡುಗಡೆ(name);
	ಬಿಡುಗಡೆ(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ launchctl_ಕೂಡಿಸು_plists(ಶೂನ್ಯ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exec_ಮಾರ್ಗ = git_exec_ಮಾರ್ಗ();

	ಹಿಂತಿರುಗಿ launchctl_ವೇಳಾಪಟ್ಟಿ_plist(exec_ಮಾರ್ಗ, ವೇಳಾಪಟ್ಟಿ_HOURLY) ||
	       launchctl_ವೇಳಾಪಟ್ಟಿ_plist(exec_ಮಾರ್ಗ, ವೇಳಾಪಟ್ಟಿ_DAILY) ||
	       launchctl_ವೇಳಾಪಟ್ಟಿ_plist(exec_ಮಾರ್ಗ, ವೇಳಾಪಟ್ಟಿ_WEEKLY);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ launchctl_update_ವೇಳಾಪಟ್ಟಿ(ಪೂರ್ಣಾಂಕ ಸತತ_maintenance, ಪೂರ್ಣಾಂಕ fd ಬಳಸದ)
{
	ಆದರೆ (ಸತತ_maintenance)
		ಹಿಂತಿರುಗಿ launchctl_ಕೂಡಿಸು_plists();
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ launchctl_remove_plists();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_schtasks_ಲಭ್ಯ(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ is_ಲಭ್ಯ;
	ಆದರೆ (get_ವೇಳಾಪಟ್ಟಿ_cmd("schtasks", &is_ಲಭ್ಯ, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ is_ಲಭ್ಯ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GIT_WINDOWS_NATIVE
	ಹಿಂತಿರುಗಿ 1;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಅಕ್ಷರ *schtasks_task_name(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *frequency)
{
	ರಚನೆ strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	ಹಿಂತಿರುಗಿ strbuf_detach(&label, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ schtasks_remove_task(ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ ವೇಳಾಪಟ್ಟಿ)
{
	ಅಕ್ಷರ *cmd;
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *frequency = get_frequency(ವೇಳಾಪಟ್ಟಿ);
	ಅಕ್ಷರ *name = schtasks_task_name(frequency);

	get_ವೇಳಾಪಟ್ಟಿ_cmd("schtasks", ಖಾಲಿ, &cmd);
	strvec_ವಿಭಜಿಸು(&ಮಗು.ವಾದಗಳು, cmd);
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "/delete", "/tn", name, "/f", ಖಾಲಿ);
	ಬಿಡುಗಡೆ(name);
	ಬಿಡುಗಡೆ(cmd);

	ಹಿಂತಿರುಗಿ ಸತತ_command(&ಮಗು);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ schtasks_remove_tasks(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ schtasks_remove_task(ವೇಳಾಪಟ್ಟಿ_HOURLY) ||
	       schtasks_remove_task(ವೇಳಾಪಟ್ಟಿ_DAILY) ||
	       schtasks_remove_task(ವೇಳಾಪಟ್ಟಿ_WEEKLY);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ schtasks_ವೇಳಾಪಟ್ಟಿ_task(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exec_ಮಾರ್ಗ, ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ ವೇಳಾಪಟ್ಟಿ)
{
	ಅಕ್ಷರ *cmd;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *xml;
	ರಚನೆ tempfile *tfile;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *frequency = get_frequency(ವೇಳಾಪಟ್ಟಿ);
	ಅಕ್ಷರ *name = schtasks_task_name(frequency);
	ರಚನೆ strbuf tfilename = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ minute = get_random_minute();

	get_ವೇಳಾಪಟ್ಟಿ_cmd("schtasks", ಖಾಲಿ, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	ಆದರೆ (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	ಆಯ್ಕೆ (ವೇಳಾಪಟ್ಟಿ) {
	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_HOURLY:
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
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_DAILY:
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
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_WEEKLY:
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
		ನಿಲ್ಲಿಸು;

	ಪೂರ್ವನಿಯೋಜಿತ:
		ನಿಲ್ಲಿಸು;
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
	fprintf(tfile->fp, xml, exec_ಮಾರ್ಗ, exec_ಮಾರ್ಗ,
		get_extra_config_parameters(), frequency);
	strvec_ವಿಭಜಿಸು(&ಮಗು.ವಾದಗಳು, cmd);
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "/create", "/tn", name, "/f", "/xml",
				  get_tempfile_ಮಾರ್ಗ(tfile), ಖಾಲಿ);
	close_tempfile_gently(tfile);

	ಮಗು.no_stdout = 1;
	ಮಗು.no_stderr = 1;

	ಆದರೆ (ಆರಂಭ_command(&ಮಗು))
		die(_("failed to start schtasks"));
	ಫಲಿತಾಂಶ = ಮುಕ್ತಾಯ_command(&ಮಗು);

	delete_tempfile(&tfile);
	ಬಿಡುಗಡೆ(name);
	ಬಿಡುಗಡೆ(cmd);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ schtasks_ವೇಳಾಪಟ್ಟಿ_tasks(ಶೂನ್ಯ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exec_ಮಾರ್ಗ = git_exec_ಮಾರ್ಗ();

	ಹಿಂತಿರುಗಿ schtasks_ವೇಳಾಪಟ್ಟಿ_task(exec_ಮಾರ್ಗ, ವೇಳಾಪಟ್ಟಿ_HOURLY) ||
	       schtasks_ವೇಳಾಪಟ್ಟಿ_task(exec_ಮಾರ್ಗ, ವೇಳಾಪಟ್ಟಿ_DAILY) ||
	       schtasks_ವೇಳಾಪಟ್ಟಿ_task(exec_ಮಾರ್ಗ, ವೇಳಾಪಟ್ಟಿ_WEEKLY);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ schtasks_update_ವೇಳಾಪಟ್ಟಿ(ಪೂರ್ಣಾಂಕ ಸತತ_maintenance, ಪೂರ್ಣಾಂಕ fd ಬಳಸದ)
{
	ಆದರೆ (ಸತತ_maintenance)
		ಹಿಂತಿರುಗಿ schtasks_ವೇಳಾಪಟ್ಟಿ_tasks();
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ schtasks_remove_tasks();
}

MAYBE_ಬಳಸದ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_crontab_process(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd)
{
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	strvec_ವಿಭಜಿಸು(&ಮಗು.ವಾದಗಳು, cmd);
	strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "-l");
	ಮಗು.no_stdin = 1;
	ಮಗು.no_stdout = 1;
	ಮಗು.no_stderr = 1;
	ಮಗು.silent_exec_failure = 1;

	ಆದರೆ (ಆರಂಭ_command(&ಮಗು))
		ಹಿಂತಿರುಗಿ 0;
	/* Ignore exit code, as an empty crontab will return error. */
	ಮುಕ್ತಾಯ_command(&ಮಗು);
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_crontab_ಲಭ್ಯ(ಶೂನ್ಯ)
{
	ಅಕ್ಷರ *cmd;
	ಪೂರ್ಣಾಂಕ is_ಲಭ್ಯ;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (get_ವೇಳಾಪಟ್ಟಿ_cmd("crontab", &is_ಲಭ್ಯ, &cmd)) {
		ret = is_ಲಭ್ಯ;
		ಹೋಗು ಹೊರಹೋಗು;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#ಇಲ್ಲದಿದ್ದರೆ
	ret = check_crontab_process(cmd);
#ಆದರೆ_ಅಂತ್ಯ

ಹೊರಹೋಗು:
	ಬಿಡುಗಡೆ(cmd);
	ಹಿಂತಿರುಗಿ ret;
}

#ವ್ಯಾಖ್ಯಾನಿಸು BEGIN_ಸಾಲು "# BEGIN GIT MAINTENANCE SCHEDULE"
#ವ್ಯಾಖ್ಯಾನಿಸು ಕೊನೆ_ಸಾಲು "# END GIT MAINTENANCE SCHEDULE"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ crontab_update_ವೇಳಾಪಟ್ಟಿ(ಪೂರ್ಣಾಂಕ ಸತತ_maintenance, ಪೂರ್ಣಾಂಕ fd)
{
	ಅಕ್ಷರ *cmd;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ಪೂರ್ಣಾಂಕ ಒಳ_old_region = 0;
	ರಚನೆ ಮಗು_process crontab_list = ಮಗು_PROCESS_INIT;
	ರಚನೆ ಮಗು_process crontab_edit = ಮಗು_PROCESS_INIT;
	FILE *cron_list, *cron_ಒಳ;
	ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;
	ರಚನೆ tempfile *tmpedit = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ minute = get_random_minute();

	get_ವೇಳಾಪಟ್ಟಿ_cmd("crontab", ಖಾಲಿ, &cmd);
	strvec_ವಿಭಜಿಸು(&crontab_list.ವಾದಗಳು, cmd);
	strvec_ತಳ್ಳು(&crontab_list.ವಾದಗಳು, "-l");
	crontab_list.ಒಳ = -1;
	crontab_list.ಹೊರಹೋಗು = dup(fd);
	crontab_list.git_cmd = 0;

	ಆದರೆ (ಆರಂಭ_command(&crontab_list)) {
		ಫಲಿತಾಂಶ = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	ಮುಕ್ತಾಯ_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	ಆದರೆ (!tmpedit) {
		ಫಲಿತಾಂಶ = error(_("failed to create crontab temporary file"));
		ಹೋಗು ಹೊರಹೋಗು;
	}
	cron_ಒಳ = fdopen_tempfile(tmpedit, "w");
	ಆದರೆ (!cron_ಒಳ) {
		ಫಲಿತಾಂಶ = error(_("failed to open temporary file"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	ಆಗ (!strbuf_getline_lf(&ಸಾಲು, cron_list)) {
		ಆದರೆ (!ಒಳ_old_region && !strcmp(ಸಾಲು.buf, BEGIN_ಸಾಲು))
			ಒಳ_old_region = 1;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಳ_old_region && !strcmp(ಸಾಲು.buf, ಕೊನೆ_ಸಾಲು))
			ಒಳ_old_region = 0;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಒಳ_old_region)
			fprintf(cron_ಒಳ, "%s\n", ಸಾಲು.buf);
	}
	strbuf_release(&ಸಾಲು);

	ಆದರೆ (ಸತತ_maintenance) {
		ರಚನೆ strbuf ಸಾಲು_format = STRBUF_INIT;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exec_ಮಾರ್ಗ = git_exec_ಮಾರ್ಗ();

		fprintf(cron_ಒಳ, "%s\n", BEGIN_ಸಾಲು);
		fprintf(cron_ಒಳ,
			"# The following schedule was created by Git\n");
		fprintf(cron_ಒಳ, "# Any edits made in this region might be\n");
		fprintf(cron_ಒಳ,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&ಸಾಲು_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_ಮಾರ್ಗ, exec_ಮಾರ್ಗ, get_extra_config_parameters());
		fprintf(cron_ಒಳ, ಸಾಲು_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_ಒಳ, ಸಾಲು_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_ಒಳ, ಸಾಲು_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&ಸಾಲು_format);

		fprintf(cron_ಒಳ, "\n%s\n", ಕೊನೆ_ಸಾಲು);
	}

	fflush(cron_ಒಳ);

	strvec_ವಿಭಜಿಸು(&crontab_edit.ವಾದಗಳು, cmd);
	strvec_ತಳ್ಳು(&crontab_edit.ವಾದಗಳು, get_tempfile_ಮಾರ್ಗ(tmpedit));
	crontab_edit.git_cmd = 0;

	ಆದರೆ (ಆರಂಭ_command(&crontab_edit)) {
		ಫಲಿತಾಂಶ = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (ಮುಕ್ತಾಯ_command(&crontab_edit))
		ಫಲಿತಾಂಶ = error(_("'crontab' died"));
	ಇಲ್ಲದಿದ್ದರೆ
		fclose(cron_list);

ಹೊರಹೋಗು:
	delete_tempfile(&tmpedit);
	ಬಿಡುಗಡೆ(cmd);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ real_is_systemd_timer_ಲಭ್ಯ(ಶೂನ್ಯ)
{
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	strvec_pushl(&ಮಗು.ವಾದಗಳು, "systemctl", "--user", "list-timers", ಖಾಲಿ);
	ಮಗು.no_stdin = 1;
	ಮಗು.no_stdout = 1;
	ಮಗು.no_stderr = 1;
	ಮಗು.silent_exec_failure = 1;

	ಆದರೆ (ಆರಂಭ_command(&ಮಗು))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (ಮುಕ್ತಾಯ_command(&ಮಗು))
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_systemd_timer_ಲಭ್ಯ(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ is_ಲಭ್ಯ;

	ಆದರೆ (get_ವೇಳಾಪಟ್ಟಿ_cmd("systemctl", &is_ಲಭ್ಯ, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ is_ಲಭ್ಯ;

	ಹಿಂತಿರುಗಿ real_is_systemd_timer_ಲಭ್ಯ();
}

ಸ್ಥಿರ ಅಕ್ಷರ *xdg_config_home_systemd(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename)
{
	ಹಿಂತಿರುಗಿ xdg_config_home_for("systemd/user", filename);
}

#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ systemd_timer_delete_timer_file(ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ ಆದ್ಯತೆ)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *frequency = get_frequency(ಆದ್ಯತೆ);
	ಅಕ್ಷರ *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	ಅಕ್ಷರ *filename = xdg_config_home_systemd(local_timer_name);

	ಆದರೆ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	ಬಿಡುಗಡೆ(filename);
	ಬಿಡುಗಡೆ(local_timer_name);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ systemd_timer_delete_service_template(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಅಕ್ಷರ *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	ಅಕ್ಷರ *filename = xdg_config_home_systemd(local_service_name);
	ಆದರೆ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	ಬಿಡುಗಡೆ(filename);
	ಬಿಡುಗಡೆ(local_service_name);
	ಹಿಂತಿರುಗಿ ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ systemd_timer_write_timer_file(ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ ವೇಳಾಪಟ್ಟಿ,
					  ಪೂರ್ಣಾಂಕ minute)
{
	ಪೂರ್ಣಾಂಕ res = -1;
	ಅಕ್ಷರ *filename;
	FILE *file;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *unit;
	ಅಕ್ಷರ *ವೇಳಾಪಟ್ಟಿ_ನಮೂನೆ = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *frequency = get_frequency(ವೇಳಾಪಟ್ಟಿ);
	ಅಕ್ಷರ *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		ಹೋಗು error;
	}
	file = fopen_or_warn(filename, "w");
	ಆದರೆ (!file)
		ಹೋಗು error;

	ಆಯ್ಕೆ (ವೇಳಾಪಟ್ಟಿ) {
	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_HOURLY:
		ವೇಳಾಪಟ್ಟಿ_ನಮೂನೆ = xstrfmt("*-*-* 1..23:%02d:00", minute);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_DAILY:
		ವೇಳಾಪಟ್ಟಿ_ನಮೂನೆ = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ವೇಳಾಪಟ್ಟಿ_WEEKLY:
		ವೇಳಾಪಟ್ಟಿ_ನಮೂನೆ = xstrfmt("Mon 0:%02d:00", minute);
		ನಿಲ್ಲಿಸು;

	ಪೂರ್ವನಿಯೋಜಿತ:
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
	ಆದರೆ (fprintf(file, unit, ವೇಳಾಪಟ್ಟಿ_ನಮೂನೆ) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		ಹೋಗು error;
	}
	ಆದರೆ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		ಹೋಗು error;
	}

	res = 0;

error:
	ಬಿಡುಗಡೆ(ವೇಳಾಪಟ್ಟಿ_ನಮೂನೆ);
	ಬಿಡುಗಡೆ(local_timer_name);
	ಬಿಡುಗಡೆ(filename);
	ಹಿಂತಿರುಗಿ res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ systemd_timer_write_service_template(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exec_ಮಾರ್ಗ)
{
	ಪೂರ್ಣಾಂಕ res = -1;
	ಅಕ್ಷರ *filename;
	FILE *file;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *unit;
	ಅಕ್ಷರ *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		ಹೋಗು error;
	}
	file = fopen_or_warn(filename, "w");
	ಆದರೆ (!file)
		ಹೋಗು error;

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
	ಆದರೆ (fprintf(file, unit, exec_ಮಾರ್ಗ, exec_ಮಾರ್ಗ, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		ಹೋಗು error;
	}
	ಆದರೆ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		ಹೋಗು error;
	}

	res = 0;

error:
	ಬಿಡುಗಡೆ(local_service_name);
	ಬಿಡುಗಡೆ(filename);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ systemd_timer_enable_unit(ಪೂರ್ಣಾಂಕ enable,
				     ಎಣಿಕೆ ವೇಳಾಪಟ್ಟಿ_ಆದ್ಯತೆ ವೇಳಾಪಟ್ಟಿ,
				     ಪೂರ್ಣಾಂಕ minute)
{
	ಅಕ್ಷರ *cmd = ಖಾಲಿ;
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *frequency = get_frequency(ವೇಳಾಪಟ್ಟಿ);
	ಪೂರ್ಣಾಂಕ ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	ಆದರೆ (!enable) {
		ಮಗು.no_stderr = 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (systemd_timer_write_timer_file(ವೇಳಾಪಟ್ಟಿ, minute)) {
		ret = -1;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	get_ವೇಳಾಪಟ್ಟಿ_cmd("systemctl", ಖಾಲಿ, &cmd);
	strvec_ವಿಭಜಿಸು(&ಮಗು.ವಾದಗಳು, cmd);
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "--user", enable ? "enable" : "disable",
		     "--now", ಖಾಲಿ);
	strvec_pushf(&ಮಗು.ವಾದಗಳು, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	ಆದರೆ (ಆರಂಭ_command(&ಮಗು)) {
		ret = error(_("failed to start systemctl"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (ಮುಕ್ತಾಯ_command(&ಮಗು)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		ಆದರೆ (enable) {
			ret = error(_("failed to run systemctl"));
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	ret = 0;

ಹೊರಹೋಗು:
	ಬಿಡುಗಡೆ(cmd);
	ಹಿಂತಿರುಗಿ ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
ಸ್ಥಿರ ಶೂನ್ಯ systemd_timer_delete_stale_timer_templates(ಶೂನ್ಯ)
{
	ಅಕ್ಷರ *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	ಅಕ್ಷರ *filename = xdg_config_home_systemd(timer_template_name);

	ಆದರೆ (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	ಬಿಡುಗಡೆ(filename);
	ಬಿಡುಗಡೆ(timer_template_name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ systemd_timer_delete_unit_files(ಶೂನ್ಯ)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	ಹಿಂತಿರುಗಿ systemd_timer_delete_timer_file(ವೇಳಾಪಟ್ಟಿ_HOURLY) |
	       systemd_timer_delete_timer_file(ವೇಳಾಪಟ್ಟಿ_DAILY) |
	       systemd_timer_delete_timer_file(ವೇಳಾಪಟ್ಟಿ_WEEKLY) |
	       systemd_timer_delete_service_template();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ systemd_timer_delete_units(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	ಹಿಂತಿರುಗಿ systemd_timer_enable_unit(0, ವೇಳಾಪಟ್ಟಿ_HOURLY, minute) |
	       systemd_timer_enable_unit(0, ವೇಳಾಪಟ್ಟಿ_DAILY, minute) |
	       systemd_timer_enable_unit(0, ವೇಳಾಪಟ್ಟಿ_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ systemd_timer_ಸಿದ್ಧತೆ_units(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ minute = get_random_minute();
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exec_ಮಾರ್ಗ = git_exec_ಮಾರ್ಗ();

	ಪೂರ್ಣಾಂಕ ret = systemd_timer_write_service_template(exec_ಮಾರ್ಗ) ||
		  systemd_timer_enable_unit(1, ವೇಳಾಪಟ್ಟಿ_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, ವೇಳಾಪಟ್ಟಿ_DAILY, minute) ||
		  systemd_timer_enable_unit(1, ವೇಳಾಪಟ್ಟಿ_WEEKLY, minute);

	ಆದರೆ (ret)
		systemd_timer_delete_units();
	ಇಲ್ಲದಿದ್ದರೆ
		systemd_timer_delete_stale_timer_templates();

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ systemd_timer_update_ವೇಳಾಪಟ್ಟಿ(ಪೂರ್ಣಾಂಕ ಸತತ_maintenance, ಪೂರ್ಣಾಂಕ fd ಬಳಸದ)
{
	ಆದರೆ (ಸತತ_maintenance)
		ಹಿಂತಿರುಗಿ systemd_timer_ಸಿದ್ಧತೆ_units();
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ systemd_timer_delete_units();
}

ಎಣಿಕೆ scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	ಪೂರ್ಣಾಂಕ (*is_ಲಭ್ಯ)(ಶೂನ್ಯ);
	ಪೂರ್ಣಾಂಕ (*update_ವೇಳಾಪಟ್ಟಿ)(ಪೂರ್ಣಾಂಕ ಸತತ_maintenance, ಪೂರ್ಣಾಂಕ fd);
} scheduler_fn[] = {
	[SCHEDULER_CRON] = {
		.name = "crontab",
		.is_ಲಭ್ಯ = is_crontab_ಲಭ್ಯ,
		.update_ವೇಳಾಪಟ್ಟಿ = crontab_update_ವೇಳಾಪಟ್ಟಿ,
	},
	[SCHEDULER_SYSTEMD] = {
		.name = "systemctl",
		.is_ಲಭ್ಯ = is_systemd_timer_ಲಭ್ಯ,
		.update_ವೇಳಾಪಟ್ಟಿ = systemd_timer_update_ವೇಳಾಪಟ್ಟಿ,
	},
	[SCHEDULER_LAUNCHCTL] = {
		.name = "launchctl",
		.is_ಲಭ್ಯ = is_launchctl_ಲಭ್ಯ,
		.update_ವೇಳಾಪಟ್ಟಿ = launchctl_update_ವೇಳಾಪಟ್ಟಿ,
	},
	[SCHEDULER_SCHTASKS] = {
		.name = "schtasks",
		.is_ಲಭ್ಯ = is_schtasks_ಲಭ್ಯ,
		.update_ವೇಳಾಪಟ್ಟಿ = schtasks_update_ವೇಳಾಪಟ್ಟಿ,
	},
};

ಸ್ಥಿರ ಎಣಿಕೆ scheduler parse_scheduler(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಆದರೆ (!ಮೌಲ್ಯ)
		ಹಿಂತಿರುಗಿ SCHEDULER_INVALID;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "auto"))
		ಹಿಂತಿರುಗಿ SCHEDULER_AUTO;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "cron") || !strcasecmp(ಮೌಲ್ಯ, "crontab"))
		ಹಿಂತಿರುಗಿ SCHEDULER_CRON;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "systemd") ||
		 !strcasecmp(ಮೌಲ್ಯ, "systemd-timer"))
		ಹಿಂತಿರುಗಿ SCHEDULER_SYSTEMD;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "launchctl"))
		ಹಿಂತಿರುಗಿ SCHEDULER_LAUNCHCTL;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "schtasks"))
		ಹಿಂತಿರುಗಿ SCHEDULER_SCHTASKS;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ SCHEDULER_INVALID;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_opt_scheduler(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				     ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ scheduler *scheduler = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	ಆದರೆ (*scheduler == SCHEDULER_INVALID)
		ಹಿಂತಿರುಗಿ error(_("unrecognized --scheduler argument '%s'"), arg);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ maintenance_ಆರಂಭ_opts {
	ಎಣಿಕೆ scheduler scheduler;
};

ಸ್ಥಿರ ಎಣಿಕೆ scheduler resolve_scheduler(ಎಣಿಕೆ scheduler scheduler)
{
	ಆದರೆ (scheduler != SCHEDULER_AUTO)
		ಹಿಂತಿರುಗಿ scheduler;

#ಆದರೆ defined(__APPLE__)
	ಹಿಂತಿರುಗಿ SCHEDULER_LAUNCHCTL;

#ಅಥವಾಆದರೆ defined(GIT_WINDOWS_NATIVE)
	ಹಿಂತಿರುಗಿ SCHEDULER_SCHTASKS;

#ಅಥವಾಆದರೆ defined(__linux__)
	ಆದರೆ (is_systemd_timer_ಲಭ್ಯ())
		ಹಿಂತಿರುಗಿ SCHEDULER_SYSTEMD;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_crontab_ಲಭ್ಯ())
		ಹಿಂತಿರುಗಿ SCHEDULER_CRON;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("neither systemd timers nor crontab are available"));

#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ SCHEDULER_CRON;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಶೂನ್ಯ validate_scheduler(ಎಣಿಕೆ scheduler scheduler)
{
	ಆದರೆ (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	ಆದರೆ (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	ಆದರೆ (!scheduler_fn[scheduler].is_ಲಭ್ಯ())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_background_ವೇಳಾಪಟ್ಟಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ maintenance_ಆರಂಭ_opts *opts,
				      ಪೂರ್ಣಾಂಕ enable)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ರಚನೆ ಬೀಗ_file lk;
	ಅಕ್ಷರ *ಬೀಗ_ಮಾರ್ಗ = xstrfmt("%s/schedule", the_repository->objects->ಮೂಲ->ಮಾರ್ಗ);

	ಆದರೆ (hold_ಬೀಗ_file_for_update(&lk, ಬೀಗ_ಮಾರ್ಗ, ಬೀಗ_NO_DEREF) < 0) {
		ಆದರೆ (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_ಮಾರ್ಗ(ಬೀಗ_ಮಾರ್ಗ), strerror(errno));
		ಇಲ್ಲದಿದ್ದರೆ
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		ಬಿಡುಗಡೆ(ಬೀಗ_ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ -1;
	}

	ಗಾಗಿ (i = 1; i < ARRAY_ಗಾತ್ರ(scheduler_fn); i++) {
		ಆದರೆ (enable && opts->scheduler == i)
			ಮುಂದುವರಿ;
		ಆದರೆ (!scheduler_fn[i].is_ಲಭ್ಯ())
			ಮುಂದುವರಿ;
		scheduler_fn[i].update_ವೇಳಾಪಟ್ಟಿ(0, get_ಬೀಗ_file_fd(&lk));
	}

	ಆದರೆ (enable)
		ಫಲಿತಾಂಶ = scheduler_fn[opts->scheduler].update_ವೇಳಾಪಟ್ಟಿ(
			1, get_ಬೀಗ_file_fd(&lk));

	rollback_ಬೀಗ_file(&lk);

	ಬಿಡುಗಡೆ(ಬೀಗ_ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_maintenance_ಆರಂಭ_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_ಆರಂಭ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ರಚನೆ repository *repo)
{
	ರಚನೆ maintenance_ಆರಂಭ_opts opts = { 0 };
	ರಚನೆ option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ರಿಜಿಸ್ಟರ್_ವಾದಗಳು[] = { "register", ಖಾಲಿ };

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_maintenance_ಆರಂಭ_usage, 0);
	ಆದರೆ (argc)
		usage_with_options(builtin_maintenance_ಆರಂಭ_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	ಆದರೆ (update_background_ವೇಳಾಪಟ್ಟಿ(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	ಆದರೆ (maintenance_ರಿಜಿಸ್ಟರ್(ARRAY_ಗಾತ್ರ(ರಿಜಿಸ್ಟರ್_ವಾದಗಳು)-1, ರಿಜಿಸ್ಟರ್_ವಾದಗಳು, ಖಾಲಿ, repo))
		warning(_("failed to add repo to global config"));
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_stop(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			    ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_maintenance_stop_usage, 0);
	ಆದರೆ (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	ಹಿಂತಿರುಗಿ update_background_ವೇಳಾಪಟ್ಟಿ(ಖಾಲಿ, 0);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ maintenance_is_needed(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				 ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ maintenance_ಸತತ_opts opts = MAINTENANCE_ಸತತ_OPTS_INIT;
	ರಚನೆ string_list selected_tasks = STRING_LIST_INIT_DUP;
	ರಚನೆ gc_config cfg = GC_CONFIG_INIT;
	ರಚನೆ option options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			       N_("check a specific task"),
			       PARSE_OPT_NONEG, task_option_parse),
		OPT_ಕೊನೆ()
	};
	bool is_needed = false;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_maintenance_is_needed_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ಆದರೆ (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	ಆದರೆ (opts.auto_flag) {
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < opts.tasks_nr; i++) {
			ಆದರೆ (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				ನಿಲ್ಲಿಸು;
			}
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
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
	maintenance_ಸತತ_opts_release(&opts);
	gc_config_release(&cfg);

	ಆದರೆ (is_needed)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	ಖಾಲಿ,
};

ಪೂರ್ಣಾಂಕ cmd_maintenance(ಪೂರ್ಣಾಂಕ argc,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		    ರಚನೆ repository *repo)
{
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option builtin_maintenance_options[] = {
		OPT_SUBCOMMAND("run", &fn, maintenance_ಸತತ),
		OPT_SUBCOMMAND("start", &fn, maintenance_ಆರಂಭ),
		OPT_SUBCOMMAND("stop", &fn, maintenance_stop),
		OPT_SUBCOMMAND("register", &fn, maintenance_ರಿಜಿಸ್ಟರ್),
		OPT_SUBCOMMAND("unregister", &fn, maintenance_unregister),
		OPT_SUBCOMMAND("is-needed", &fn, maintenance_is_needed),
		OPT_ಕೊನೆ(),
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_maintenance_options,
			     builtin_maintenance_usage, 0);
	ಹಿಂತಿರುಗಿ fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
}
