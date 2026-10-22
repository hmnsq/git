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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "date.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tempfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sigchain.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-graph.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reflog.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rerere.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "exec-cmd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hook.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "worktree.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FAILED_ਲੜੀਵਾਰ "failed to run %s"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	ਨਿਰਾ
};

ਸਥਿਰ timestamp_t gc_log_expire_ਸਮਾਂ;
ਸਥਿਰ ਬਣਤਰ tempfile *pidfile;
ਸਥਿਰ ਬਣਤਰ ਤਾਲਾ_file log_ਤਾਲਾ;
ਸਥਿਰ ਬਣਤਰ string_list pack_garbage = STRING_LIST_INIT_DUP;

ਸਥਿਰ ਖਾਲੀ clean_pack_garbage(ਖਾਲੀ)
{
	ਪੂਰਨ i;
	ਲਈ (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.ਚੀਜ਼ਾਂ[i].string);
	string_list_clear(&pack_garbage, 0);
}

ਸਥਿਰ ਖਾਲੀ report_pack_garbage(ਬਿਨਾਨਿਸ਼ਾਨੀ ਵੇਖਿਆ_ਬਿੱਟ, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਜੇ (ਵੇਖਿਆ_ਬਿੱਟ == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ process_log_file(ਖਾਲੀ)
{
	ਬਣਤਰ stat st;
	ਜੇ (fstat(get_ਤਾਲਾ_file_fd(&log_ਤਾਲਾ), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		ਪੂਰਨ saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_ਤਾਲਾ_file_ਰਸਤਾ(&log_ਤਾਲਾ),
			strerror(saved_errno));
		fflush(stderr);
		commit_ਤਾਲਾ_file(&log_ਤਾਲਾ);
		errno = saved_errno;
	} ਨਹੀਂਤਾਂ ਜੇ (st.st_ਮਾਪ) {
		/* There was some error recorded in the lock file */
		commit_ਤਾਲਾ_file(&log_ਤਾਲਾ);
	} ਨਹੀਂਤਾਂ {
		ਅੱਖਰ *ਰਸਤਾ = repo_git_ਰਸਤਾ(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(ਰਸਤਾ);
		rollback_ਤਾਲਾ_file(&log_ਤਾਲਾ);
		ਮੁਕਤ(ਰਸਤਾ);
	}
}

ਸਥਿਰ ਖਾਲੀ process_log_file_at_exit(ਖਾਲੀ)
{
	fflush(stderr);
	process_log_file();
}

ਸਥਿਰ ਪੂਰਨ gc_config_is_timestamp_never(ਅਚਲ ਅੱਖਰ *var)
{
	ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;
	timestamp_t expire;

	ਜੇ (!repo_config_get_ਮੁੱਲ(the_repository, var, &ਮੁੱਲ) && ਮੁੱਲ) {
		ਜੇ (parse_expiry_date(ਮੁੱਲ, &expire))
			die(_("failed to parse '%s' value '%s'"), var, ਮੁੱਲ);
		ਵਾਪਸ expire == 0;
	}
	ਵਾਪਸ 0;
}

ਬਣਤਰ gc_config {
	ਪੂਰਨ pack_refs;
	ਪੂਰਨ prune_reflogs;
	ਪੂਰਨ cruft_packs;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_cruft_ਮਾਪ;
	ਪੂਰਨ aggressive_ਡੂੰਘਾਈ;
	ਪੂਰਨ aggressive_ਖਿੜਕੀ;
	ਪੂਰਨ detach_auto;
	ਅੱਖਰ *gc_log_expire;
	ਅੱਖਰ *prune_expire;
	ਅੱਖਰ *prune_worktrees_expire;
	ਅੱਖਰ *repack_expire_ਮੰਜ਼ਿਲ;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GC_CONFIG_INIT { \
	.pack_refs = 1, \
	.prune_reflogs = 1, \
	.cruft_packs = 1, \
	.aggressive_ਡੂੰਘਾਈ = 50, \
	.aggressive_ਖਿੜਕੀ = 250, \
	.detach_auto = 1, \
	.gc_log_expire = xstrdup("1.day.ago"), \
	.prune_expire = xstrdup("2.weeks.ago"), \
	.prune_worktrees_expire = xstrdup("3.months.ago"), \
}

ਸਥਿਰ ਖਾਲੀ gc_config_release(ਬਣਤਰ gc_config *cfg)
{
	ਮੁਕਤ(cfg->gc_log_expire);
	ਮੁਕਤ(cfg->prune_expire);
	ਮੁਕਤ(cfg->prune_worktrees_expire);
}

ਸਥਿਰ ਖਾਲੀ gc_config(ਬਣਤਰ gc_config *cfg)
{
	ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;
	ਅੱਖਰ *owned = ਨਿਰਾ;

	ਜੇ (!repo_config_get_ਮੁੱਲ(the_repository, "gc.packrefs", &ਮੁੱਲ)) {
		ਜੇ (ਮੁੱਲ && !strcmp(ਮੁੱਲ, "notbare"))
			cfg->pack_refs = -1;
		ਨਹੀਂਤਾਂ
			cfg->pack_refs = git_config_bool("gc.packrefs", ਮੁੱਲ);
	}

	ਜੇ (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_ਖਿੜਕੀ);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_ਡੂੰਘਾਈ);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_ਮਾਪ);

	ਜੇ (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		ਮੁਕਤ(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	ਜੇ (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		ਮੁਕਤ(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	ਜੇ (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		ਮੁਕਤ(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
}

ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ {
	ਸਮਾਂ_ਸਾਰਣੀ_NONE = 0,
	ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY = 1,
	ਸਮਾਂ_ਸਾਰਣੀ_DAILY = 2,
	ਸਮਾਂ_ਸਾਰਣੀ_HOURLY = 3,
};

ਸਥਿਰ ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ parse_ਸਮਾਂ_ਸਾਰਣੀ(ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਜੇ (!ਮੁੱਲ)
		ਵਾਪਸ ਸਮਾਂ_ਸਾਰਣੀ_NONE;
	ਜੇ (!strcasecmp(ਮੁੱਲ, "hourly"))
		ਵਾਪਸ ਸਮਾਂ_ਸਾਰਣੀ_HOURLY;
	ਜੇ (!strcasecmp(ਮੁੱਲ, "daily"))
		ਵਾਪਸ ਸਮਾਂ_ਸਾਰਣੀ_DAILY;
	ਜੇ (!strcasecmp(ਮੁੱਲ, "weekly"))
		ਵਾਪਸ ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY;
	ਵਾਪਸ ਸਮਾਂ_ਸਾਰਣੀ_NONE;
}

ਗਿਣਤੀ maintenance_task_label {
	TASK_PREFETCH,
	TASK_LOOSE_OBJECTS,
	TASK_INCREMENTAL_REPACK,
	TASK_GEOMETRIC_REPACK,
	TASK_GC,
	TASK_COMMIT_ਗ੍ਰਾਫ,
	TASK_PACK_REFS,
	TASK_REFLOG_EXPIRE,
	TASK_WORKTREE_PRUNE,
	TASK_RERERE_GC,

	/* Leave as final value */
	TASK_ਗਿਣਤੀ
};

ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts {
	ਗਿਣਤੀ maintenance_task_label *tasks;
	ਮਾਪ_t tasks_nr, tasks_alloc;
	ਪੂਰਨ auto_flag;
	ਪੂਰਨ detach;
	ਪੂਰਨ quiet;
	ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ ਸਮਾਂ_ਸਾਰਣੀ;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAINTENANCE_ਲੜੀਵਾਰ_OPTS_INIT { \
	.detach = -1, \
}

ਸਥਿਰ ਖਾਲੀ maintenance_ਲੜੀਵਾਰ_opts_release(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts)
{
	ਮੁਕਤ(opts->tasks);
}

ਸਥਿਰ ਪੂਰਨ pack_refs_condition(ਅਣਵਰਤਿਆ ਬਣਤਰ gc_config *cfg)
{
	ਬਣਤਰ string_list included_refs = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	ਬਣਤਰ refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	ਜੇ (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		ਵਾਪਸ 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	ਵਾਪਸ required;
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_pack_refs(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
				      ਅਣਵਰਤਿਆ ਬਣਤਰ gc_config *cfg)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.ਦਲੀਲਾਂ, "pack-refs", "--all", "--prune", ਨਿਰਾ);
	ਜੇ (opts->auto_flag)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--auto");

	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cmd);
}

ਬਣਤਰ ਗਿਣਤੀ_reflog_entries_ਅੰਕੜੇ {
	ਬਣਤਰ expire_reflog_policy_cb policy;
	ਮਾਪ_t ਗਿਣਤੀ_2;
	ਮਾਪ_t ਹੱਦਬੰਦੀ;
};

ਸਥਿਰ ਪੂਰਨ ਗਿਣਤੀ_reflog_entries(ਅਚਲ ਅੱਖਰ *refname ਅਣਵਰਤਿਆ,
				ਬਣਤਰ object_id *old_oid, ਬਣਤਰ object_id *new_oid,
				ਅਚਲ ਅੱਖਰ *committer, timestamp_t timestamp,
				ਪੂਰਨ tz, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ ਗਿਣਤੀ_reflog_entries_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;
	ਜੇ (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, ਸੁਨੇਹਾ, &ਅੰਕੜੇ->policy))
		ਅੰਕੜੇ->ਗਿਣਤੀ_2++;
	ਵਾਪਸ ਅੰਕੜੇ->ਗਿਣਤੀ_2 >= ਅੰਕੜੇ->ਹੱਦਬੰਦੀ;
}

ਸਥਿਰ ਪੂਰਨ reflog_expire_condition(ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	timestamp_t now = ਸਮਾਂ(ਨਿਰਾ);
	ਬਣਤਰ ਗਿਣਤੀ_reflog_entries_ਅੰਕੜੇ ਅੰਕੜੇ = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	ਪੂਰਨ ਹੱਦਬੰਦੀ = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &ਹੱਦਬੰਦੀ);
	ਜੇ (!ਹੱਦਬੰਦੀ)
		ਵਾਪਸ 0;
	ਜੇ (ਹੱਦਬੰਦੀ < 0)
		ਵਾਪਸ 1;
	ਅੰਕੜੇ.ਹੱਦਬੰਦੀ = ਹੱਦਬੰਦੀ;

	repo_config(the_repository, reflog_expire_config, &ਅੰਕੜੇ.policy.opts);

	reflog_expire_options_ਸੈੱਟ_refname(&ਅੰਕੜੇ.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 ਗਿਣਤੀ_reflog_entries, &ਅੰਕੜੇ);

	reflog_expiry_cleanup(&ਅੰਕੜੇ.policy);
	reflog_clear_expire_config(&ਅੰਕੜੇ.policy.opts);
	ਵਾਪਸ ਅੰਕੜੇ.ਗਿਣਤੀ_2 >= ਅੰਕੜੇ.ਹੱਦਬੰਦੀ;
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_reflog_expire(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts ਅਣਵਰਤਿਆ,
					  ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.ਦਲੀਲਾਂ, "reflog", "expire", "--all", ਨਿਰਾ);
	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cmd);
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_worktree_prune(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts ਅਣਵਰਤਿਆ,
					   ਬਣਤਰ gc_config *cfg)
{
	ਬਣਤਰ ਬੱਚਾ_process prune_worktrees_cmd = ਬੱਚਾ_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.ਦਲੀਲਾਂ, "worktree", "prune", "--expire", ਨਿਰਾ);
	strvec_ਧੱਕੋ(&prune_worktrees_cmd.ਦਲੀਲਾਂ, cfg->prune_worktrees_expire);

	ਵਾਪਸ ਲੜੀਵਾਰ_command(&prune_worktrees_cmd);
}

ਸਥਿਰ ਪੂਰਨ worktree_prune_condition(ਬਣਤਰ gc_config *cfg)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਪੂਰਨ should_prune = 0, ਹੱਦਬੰਦੀ = 1;
	timestamp_t expiry_date;
	ਬਣਤਰ dirent *d;
	DIR *dir = ਨਿਰਾ;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &ਹੱਦਬੰਦੀ);
	ਜੇ (ਹੱਦਬੰਦੀ <= 0) {
		should_prune = ਹੱਦਬੰਦੀ < 0;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		ਜਾਓ ਬਾਹਰਲਾ;

	dir = opendir(repo_git_ਰਸਤਾ_replace(the_repository, &buf, "worktrees"));
	ਜੇ (!dir)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜਦੋਂ (ਹੱਦਬੰਦੀ && (d = readdir_skip_dot_and_dotdot(dir))) {
		ਅੱਖਰ *wtpath;
		strbuf_reset(&buf);
		ਜੇ (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			ਹੱਦਬੰਦੀ--;
		ਮੁਕਤ(wtpath);
	}

	should_prune = !ਹੱਦਬੰਦੀ;

ਬਾਹਰਲਾ:
	ਜੇ (dir)
		closedir(dir);
	strbuf_release(&buf);
	ਵਾਪਸ should_prune;
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_rerere_gc(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts ਅਣਵਰਤਿਆ,
				      ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ ਬੱਚਾ_process rerere_cmd = ਬੱਚਾ_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.ਦਲੀਲਾਂ, "rerere", "gc", ਨਿਰਾ);
	ਵਾਪਸ ਲੜੀਵਾਰ_command(&rerere_cmd);
}

ਸਥਿਰ ਪੂਰਨ rerere_gc_condition(ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ਹੱਦਬੰਦੀ = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &ਹੱਦਬੰਦੀ);
	ਜੇ (!ਹੱਦਬੰਦੀ)
		ਵਾਪਸ 0; /* never prune */
	ਜੇ (ਹੱਦਬੰਦੀ < 0)
		ਵਾਪਸ 1; /* always prune */

	ਵਾਪਸ rerere_gc_needed(the_repository, (ਮਾਪ_t)ਹੱਦਬੰਦੀ);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_ਮੰਜ਼ਿਲ = (cfg)->repack_expire_ਮੰਜ਼ਿਲ, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_ਮਾਪ = (cfg)->max_cruft_ਮਾਪ, \
	.ਖਿੜਕੀ = (aggressive) ? (cfg)->aggressive_ਖਿੜਕੀ : 0, \
	.ਡੂੰਘਾਈ = (aggressive) ? (cfg)->aggressive_ਡੂੰਘਾਈ : 0

/* return NULL on success, else hostname running the gc */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਤਾਲਾ_repo_for_gc(ਪੂਰਨ force, pid_t* ret_pid)
{
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ = ਤਾਲਾ_INIT;
	ਅੱਖਰ my_host[HOST_NAME_MAX + 1];
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਬਣਤਰ stat st;
	uintmax_t pid;
	FILE *fp;
	ਪੂਰਨ fd;
	ਅੱਖਰ *pidfile_ਰਸਤਾ;

	ਜੇ (is_tempfile_ਸਰਗਰਮ(pidfile))
		/* already locked */
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (xgethostname(my_host, ਆਕਾਰ(my_host)))
		xsnprintf(my_host, ਆਕਾਰ(my_host), "unknown");

	pidfile_ਰਸਤਾ = repo_git_ਰਸਤਾ(the_repository, "gc.pid");
	fd = hold_ਤਾਲਾ_file_for_update(&ਤਾਲਾ, pidfile_ਰਸਤਾ,
				       ਤਾਲਾ_DIE_ON_ERROR);
	ਜੇ (!force) {
		ਸਥਿਰ ਅੱਖਰ locking_host[HOST_NAME_MAX + 1];
		ਸਥਿਰ ਅੱਖਰ *ਜਾਂਚ_fmt;
		ਪੂਰਨ should_exit;

		ਜੇ (!ਜਾਂਚ_fmt)
			ਜਾਂਚ_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_ਰਸਤਾ, "r");
		memset(locking_host, 0, ਆਕਾਰ(locking_host));
		should_exit =
			fp != ਨਿਰਾ &&
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
			ਸਮਾਂ(ਨਿਰਾ) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, ਜਾਂਚ_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		ਜੇ (fp)
			fclose(fp);
		ਜੇ (should_exit) {
			ਜੇ (fd >= 0)
				rollback_ਤਾਲਾ_file(&ਤਾਲਾ);
			*ret_pid = pid;
			ਮੁਕਤ(pidfile_ਰਸਤਾ);
			ਵਾਪਸ locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_ਵਿੱਚ_ਭਰਿਆ(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_ਤਾਲਾ_file(&ਤਾਲਾ);
	pidfile = ਰਜਿਸਟਰ_tempfile(pidfile_ਰਸਤਾ);
	ਮੁਕਤ(pidfile_ਰਸਤਾ);
	ਵਾਪਸ ਨਿਰਾ;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
ਸਥਿਰ ਪੂਰਨ report_ਆਖਰੀ_gc_error(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਪੂਰਨ ret = 0;
	ssize_t len;
	ਬਣਤਰ stat st;
	ਅੱਖਰ *gc_log_ਰਸਤਾ = repo_git_ਰਸਤਾ(the_repository, "gc.log");

	ਜੇ (stat(gc_log_ਰਸਤਾ, &st)) {
		ਜੇ (errno == ENOENT)
			ਜਾਓ ਪੂਰਾ;

		ret = die_ਸੁਨੇਹਾ_errno(_("cannot stat '%s'"), gc_log_ਰਸਤਾ);
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (st.st_mtime < gc_log_expire_ਸਮਾਂ)
		ਜਾਓ ਪੂਰਾ;

	len = strbuf_read_file(&sb, gc_log_ਰਸਤਾ, 0);
	ਜੇ (len < 0)
		ret = die_ਸੁਨੇਹਾ_errno(_("cannot read '%s'"), gc_log_ਰਸਤਾ);
	ਨਹੀਂਤਾਂ ਜੇ (len > 0) {
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
			    gc_log_ਰਸਤਾ, sb.buf);
		ret = 1;
	}
	strbuf_release(&sb);
ਪੂਰਾ:
	ਮੁਕਤ(gc_log_ਰਸਤਾ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ gc_foreground_tasks(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
			       ਬਣਤਰ gc_config *cfg)
{
	ਜੇ (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		ਵਾਪਸ error(FAILED_ਲੜੀਵਾਰ, "pack-refs");
	ਜੇ (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		ਵਾਪਸ error(FAILED_ਲੜੀਵਾਰ, "reflog");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_odb(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
				ਬਣਤਰ gc_config *cfg,
				ਪੂਰਨ keep_ਸਭ_ਤੋਂ_ਵੱਡਾ_pack,
				ਪੂਰਨ aggressive)
{
	ਬਣਤਰ odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_ਸਭ_ਤੋਂ_ਵੱਡਾ_pack = keep_ਸਭ_ਤੋਂ_ਵੱਡਾ_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	ਜੇ (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	ਜੇ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	ਜੇ (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	ਵਾਪਸ odb_optimize(the_repository->objects, &odb_opts);
}

ਪੂਰਨ cmd_gc(ਪੂਰਨ argc,
	   ਅਚਲ ਅੱਖਰ **argv,
	   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ aggressive = 0;
	ਪੂਰਨ force = 0;
	ਅਚਲ ਅੱਖਰ *name;
	pid_t pid;
	ਪੂਰਨ daemonized = 0;
	ਪੂਰਨ keep_ਸਭ_ਤੋਂ_ਵੱਡਾ_pack = -1;
	ਪੂਰਨ skip_foreground_tasks = 0;
	timestamp_t dummy;
	ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts opts = MAINTENANCE_ਲੜੀਵਾਰ_OPTS_INIT;
	ਬਣਤਰ gc_config cfg = GC_CONFIG_INIT;
	ਅਚਲ ਅੱਖਰ *prune_expire_sentinel = "sentinel";
	ਅਚਲ ਅੱਖਰ *prune_expire_arg = prune_expire_sentinel;
	ਪੂਰਨ ret;
	ਬਣਤਰ option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.ਮੁੱਲ = &prune_expire_arg,
			.argh = N_("date"),
			.help = N_("prune unreferenced objects"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)prune_expire_arg,
		},
		OPT_BOOL(0, "cruft", &cfg.cruft_packs, N_("pack unreferenced objects separately")),
		OPT_UNSIGNED(0, "max-cruft-size", &cfg.max_cruft_ਮਾਪ,
			     N_("with --cruft, limit the size of new cruft packs")),
		OPT_BOOL(0, "aggressive", &aggressive, N_("be more thorough (increased runtime)")),
		OPT_BOOL_F(0, "auto", &opts.auto_flag, N_("enable auto-gc mode"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform garbage collection in the background")),
		OPT_BOOL_F(0, "force", &force,
			   N_("force running gc even if there may be another gc running"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "keep-largest-pack", &keep_ਸਭ_ਤੋਂ_ਵੱਡਾ_pack,
			 N_("repack all other packs except the largest pack")),
		OPT_STRING(0, "expire-to", &cfg.repack_expire_ਮੰਜ਼ਿਲ, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_HIDDEN_BOOL(0, "skip-foreground-tasks", &skip_foreground_tasks,
			   N_("skip maintenance tasks typically done in the foreground")),
		OPT_ਸਿਰਾ()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_gc_usage, builtin_gc_options);

	gc_config(&cfg);

	ਜੇ (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_ਸਮਾਂ))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	ਜੇ (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_gc_options,
			     builtin_gc_usage, 0);
	ਜੇ (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	ਜੇ (prune_expire_arg != prune_expire_sentinel) {
		ਮੁਕਤ(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	ਜੇ (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	ਜੇ (opts.auto_flag) {
		ਬਣਤਰ odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		ਜੇ (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		ਜੇ (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    ਲੜੀਵਾਰ_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਜੇ (!opts.quiet) {
			ਜੇ (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			ਨਹੀਂਤਾਂ
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	ਜੇ (opts.detach > 0) {
		ret = report_ਆਖਰੀ_gc_error();
		ਜੇ (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			ਜਾਓ ਬਾਹਰਲਾ;

		} ਨਹੀਂਤਾਂ ਜੇ (ret) {
			/* an I/O error occurred, already reported */
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਜੇ (!skip_foreground_tasks) {
			ਜੇ (ਤਾਲਾ_repo_for_gc(force, &pid)) {
				ret = 0;
				ਜਾਓ ਬਾਹਰਲਾ;
			}

			ਜੇ (gc_foreground_tasks(&opts, &cfg) < 0)
				die(ਨਿਰਾ);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = ਤਾਲਾ_repo_for_gc(force, &pid);
	ਜੇ (name) {
		ਜੇ (opts.auto_flag) {
			ret = 0;
			ਜਾਓ ਬਾਹਰਲਾ; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	ਜੇ (daemonized) {
		ਅੱਖਰ *ਰਸਤਾ = repo_git_ਰਸਤਾ(the_repository, "gc.log");
		hold_ਤਾਲਾ_file_for_update(&log_ਤਾਲਾ, ਰਸਤਾ,
					  ਤਾਲਾ_DIE_ON_ERROR);
		dup2(get_ਤਾਲਾ_file_fd(&log_ਤਾਲਾ), 2);
		atexit(process_log_file_at_exit);
		ਮੁਕਤ(ਰਸਤਾ);
	}

	ਜੇ (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	ਜੇ (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_ਲੜੀਵਾਰ, "worktree");

	ਜੇ (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_ਲੜੀਵਾਰ, "rerere");

	ਜੇ (maintenance_task_odb(&opts, &cfg, keep_ਸਭ_ਤੋਂ_ਵੱਡਾ_pack, aggressive))
		die(ਨਿਰਾ);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	ਜੇ (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	ਜੇ (the_repository->settings.gc_write_commit_ਗ੍ਰਾਫ == 1)
		write_commit_ਗ੍ਰਾਫ_reachable(the_repository,
					     the_repository->objects->ਸਰੋਤ->ਰਸਤਾ,
					     !opts.quiet && !daemonized ? COMMIT_ਗ੍ਰਾਫ_WRITE_ਤਰੱਕੀ : 0,
					     ਨਿਰਾ);

	ਜੇ (!daemonized) {
		ਅੱਖਰ *ਰਸਤਾ = repo_git_ਰਸਤਾ(the_repository, "gc.log");
		unlink(ਰਸਤਾ);
		ਮੁਕਤ(ਰਸਤਾ);
	}

ਬਾਹਰਲਾ:
	maintenance_ਲੜੀਵਾਰ_opts_release(&opts);
	gc_config_release(&cfg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_maintenance_ਲੜੀਵਾਰ_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ maintenance_opt_ਸਮਾਂ_ਸਾਰਣੀ(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				    ਪੂਰਨ unset)
{
	ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ *ਤਰਜੀਹ = opt->ਮੁੱਲ;

	ਜੇ (unset)
		die(_("--no-schedule is not allowed"));

	*ਤਰਜੀਹ = parse_ਸਮਾਂ_ਸਾਰਣੀ(arg);

	ਜੇ (!*ਤਰਜੀਹ)
		die(_("unrecognized --schedule argument '%s'"), arg);

	ਵਾਪਸ 0;
}

ਬਣਤਰ cg_auto_ਅੰਕੜੇ {
	ਪੂਰਨ ਅੰਕ_not_ਵਿੱਚ_ਗ੍ਰਾਫ;
	ਪੂਰਨ ਹੱਦਬੰਦੀ;
};

ਸਥਿਰ ਪੂਰਨ dfs_on_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ cg_auto_ਅੰਕੜੇ *ਅੰਕੜੇ = (ਬਣਤਰ cg_auto_ਅੰਕੜੇ *)cb_ਅੰਕੜੇ;
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਅਚਲ ਬਣਤਰ object_id *maybe_peeled = ref->oid;
	ਬਣਤਰ object_id peeled;
	ਬਣਤਰ commit_list *ਢੇਰ = ਨਿਰਾ;
	ਬਣਤਰ commit *commit;

	ਜੇ (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	ਜੇ (odb_read_object_info(the_repository->objects, maybe_peeled, ਨਿਰਾ) != OBJ_COMMIT)
		ਵਾਪਸ 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	ਜੇ (!commit || commit->object.flags & ਵੇਖਿਆ_2)
		ਵਾਪਸ 0;
	commit->object.flags |= ਵੇਖਿਆ_2;

	ਜੇ (repo_parse_commit(the_repository, commit) ||
	    commit_ਗ੍ਰਾਫ_position(commit) != COMMIT_NOT_FROM_ਗ੍ਰਾਫ)
		ਵਾਪਸ 0;

	ਅੰਕੜੇ->ਅੰਕ_not_ਵਿੱਚ_ਗ੍ਰਾਫ++;

	ਜੇ (ਅੰਕੜੇ->ਅੰਕ_not_ਵਿੱਚ_ਗ੍ਰਾਫ >= ਅੰਕੜੇ->ਹੱਦਬੰਦੀ)
		ਵਾਪਸ 1;

	commit_list_ਪਾਓ(commit, &ਢੇਰ);

	ਜਦੋਂ (!ਨਤੀਜਾ && ਢੇਰ) {
		ਬਣਤਰ commit_list *ਮਾਪਾ;

		commit = ਕੱਢੋ_commit(&ਢੇਰ);

		ਲਈ (ਮਾਪਾ = commit->parents; ਮਾਪਾ; ਮਾਪਾ = ਮਾਪਾ->ਅਗਲਾ) {
			ਜੇ (repo_parse_commit(the_repository, ਮਾਪਾ->ਵਸਤੂ) ||
			    commit_ਗ੍ਰਾਫ_position(ਮਾਪਾ->ਵਸਤੂ) != COMMIT_NOT_FROM_ਗ੍ਰਾਫ ||
			    ਮਾਪਾ->ਵਸਤੂ->object.flags & ਵੇਖਿਆ_2)
				ਜਾਰੀ;

			ਮਾਪਾ->ਵਸਤੂ->object.flags |= ਵੇਖਿਆ_2;
			ਅੰਕੜੇ->ਅੰਕ_not_ਵਿੱਚ_ਗ੍ਰਾਫ++;

			ਜੇ (ਅੰਕੜੇ->ਅੰਕ_not_ਵਿੱਚ_ਗ੍ਰਾਫ >= ਅੰਕੜੇ->ਹੱਦਬੰਦੀ) {
				ਨਤੀਜਾ = 1;
				ਰੋਕੋ;
			}

			commit_list_ਪਾਓ(ਮਾਪਾ->ਵਸਤੂ, &ਢੇਰ);
		}
	}

	commit_list_free(ਢੇਰ);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ should_write_commit_ਗ੍ਰਾਫ(ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ਨਤੀਜਾ;
	ਬਣਤਰ cg_auto_ਅੰਕੜੇ ਅੰਕੜੇ;

	ਅੰਕੜੇ.ਅੰਕ_not_ਵਿੱਚ_ਗ੍ਰਾਫ = 0;
	ਅੰਕੜੇ.ਹੱਦਬੰਦੀ = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &ਅੰਕੜੇ.ਹੱਦਬੰਦੀ);

	ਜੇ (!ਅੰਕੜੇ.ਹੱਦਬੰਦੀ)
		ਵਾਪਸ 0;
	ਜੇ (ਅੰਕੜੇ.ਹੱਦਬੰਦੀ < 0)
		ਵਾਪਸ 1;

	ਨਤੀਜਾ = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &ਅੰਕੜੇ);

	repo_clear_commit_marks(the_repository, ਵੇਖਿਆ_2);

	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_write_commit_ਗ੍ਰਾਫ(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts)
{
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	ਬੱਚਾ.git_cmd = 1;
	ਬੱਚਾ.odb_ਮੰਜ਼ਿਲ_close = the_repository->objects;
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "commit-graph", "write",
		     "--split", "--reachable", ਨਿਰਾ);

	ਜੇ (opts->quiet)
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--no-progress");
	ਨਹੀਂਤਾਂ
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--progress");

	ਵਾਪਸ !!ਲੜੀਵਾਰ_command(&ਬੱਚਾ);
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_commit_ਗ੍ਰਾਫ(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
					 ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	prepare_repo_settings(the_repository);
	ਜੇ (!the_repository->settings.core_commit_ਗ੍ਰਾਫ)
		ਵਾਪਸ 0;

	ਜੇ (ਲੜੀਵਾਰ_write_commit_ਗ੍ਰਾਫ(opts)) {
		error(_("failed to write commit-graph"));
		ਵਾਪਸ 1;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ fetch_remote(ਬਣਤਰ remote *remote, ਖਾਲੀ *cbdata)
{
	ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts = cbdata;
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	ਜੇ (remote->skip_default_update)
		ਵਾਪਸ 0;

	ਬੱਚਾ.git_cmd = 1;
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     ਨਿਰਾ);

	ਜੇ (opts->quiet)
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--quiet");

	ਵਾਪਸ !!ਲੜੀਵਾਰ_command(&ਬੱਚਾ);
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_prefetch(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
				     ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	ਜੇ (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		ਵਾਪਸ 1;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_gc_foreground(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
					  ਬਣਤਰ gc_config *cfg)
{
	ਵਾਪਸ gc_foreground_tasks(opts, cfg);
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_gc_background(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
					  ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	ਬੱਚਾ.git_cmd = 1;
	ਬੱਚਾ.odb_ਮੰਜ਼ਿਲ_close = the_repository->objects;
	strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "gc");

	ਜੇ (opts->auto_flag)
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--auto");
	ਜੇ (opts->quiet)
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--quiet");
	ਨਹੀਂਤਾਂ
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--no-quiet");
	strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--no-detach");
	strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--skip-foreground-tasks");

	ਵਾਪਸ ਲੜੀਵਾਰ_command(&ਬੱਚਾ);
}

ਸਥਿਰ ਪੂਰਨ gc_condition(ਬਣਤਰ gc_config *cfg)
{
	ਬਣਤਰ odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	ਵਾਪਸ odb_optimize_required(the_repository->objects, &opts);
}

ਸਥਿਰ ਪੂਰਨ prune_packed(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts)
{
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	ਬੱਚਾ.git_cmd = 1;
	strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "prune-packed");

	ਜੇ (opts->quiet)
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--quiet");

	ਵਾਪਸ !!ਲੜੀਵਾਰ_command(&ਬੱਚਾ);
}

ਬਣਤਰ write_loose_object_ਅੰਕੜੇ {
	FILE *ਵਿੱਚ;
	ਪੂਰਨ ਗਿਣਤੀ_2;
	ਪੂਰਨ batch_ਮਾਪ;
};

ਸਥਿਰ ਪੂਰਨ loose_object_auto_ਹੱਦਬੰਦੀ = 100;

ਸਥਿਰ ਪੂਰਨ loose_object_ਗਿਣਤੀ(ਅਚਲ ਬਣਤਰ object_id *oid ਅਣਵਰਤਿਆ,
			      ਅਚਲ ਅੱਖਰ *ਰਸਤਾ ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਪੂਰਨ *ਗਿਣਤੀ_2 = (ਪੂਰਨ*)ਅੰਕੜੇ;
	ਜੇ (++(*ਗਿਣਤੀ_2) >= loose_object_auto_ਹੱਦਬੰਦੀ)
		ਵਾਪਸ 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ loose_object_auto_condition(ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ਗਿਣਤੀ_2 = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_ਹੱਦਬੰਦੀ);

	ਜੇ (!loose_object_auto_ਹੱਦਬੰਦੀ)
		ਵਾਪਸ 0;
	ਜੇ (loose_object_auto_ਹੱਦਬੰਦੀ < 0)
		ਵਾਪਸ 1;

	ਵਾਪਸ for_each_loose_file_ਵਿੱਚ_ਸਰੋਤ(the_repository->objects->ਸਰੋਤ,
					     loose_object_ਗਿਣਤੀ,
					     ਨਿਰਾ, ਨਿਰਾ, &ਗਿਣਤੀ_2);
}

ਸਥਿਰ ਪੂਰਨ bail_on_loose(ਅਚਲ ਬਣਤਰ object_id *oid ਅਣਵਰਤਿਆ,
			 ਅਚਲ ਅੱਖਰ *ਰਸਤਾ ਅਣਵਰਤਿਆ,
			 ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ write_loose_object_ਮੰਜ਼ਿਲ_stdin(ਅਚਲ ਬਣਤਰ object_id *oid,
				       ਅਚਲ ਅੱਖਰ *ਰਸਤਾ ਅਣਵਰਤਿਆ,
				       ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ write_loose_object_ਅੰਕੜੇ *d = (ਬਣਤਰ write_loose_object_ਅੰਕੜੇ *)ਅੰਕੜੇ;

	fprintf(d->ਵਿੱਚ, "%s\n", oid_ਮੰਜ਼ਿਲ_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	ਵਾਪਸ ++(d->ਗਿਣਤੀ_2) > d->batch_ਮਾਪ;
}

ਸਥਿਰ ਪੂਰਨ pack_loose(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts)
{
	ਬਣਤਰ repository *r = the_repository;
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਬਣਤਰ write_loose_object_ਅੰਕੜੇ ਅੰਕੜੇ;
	ਬਣਤਰ ਬੱਚਾ_process pack_proc = ਬੱਚਾ_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	ਜੇ (!for_each_loose_file_ਵਿੱਚ_ਸਰੋਤ(r->objects->ਸਰੋਤ,
					   bail_on_loose,
					   ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ))
		ਵਾਪਸ 0;

	pack_proc.git_cmd = 1;

	strvec_ਧੱਕੋ(&pack_proc.ਦਲੀਲਾਂ, "pack-objects");
	ਜੇ (opts->quiet)
		strvec_ਧੱਕੋ(&pack_proc.ਦਲੀਲਾਂ, "--quiet");
	ਨਹੀਂਤਾਂ
		strvec_ਧੱਕੋ(&pack_proc.ਦਲੀਲਾਂ, "--no-quiet");
	strvec_pushf(&pack_proc.ਦਲੀਲਾਂ, "%s/pack/loose", r->objects->ਸਰੋਤ->ਰਸਤਾ);

	pack_proc.ਵਿੱਚ = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.ਬਾਹਰਲਾ = -1;

	ਜੇ (ਸ਼ੁਰੂ_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		ਵਾਪਸ 1;
	}

	ਅੰਕੜੇ.ਵਿੱਚ = xfdopen(pack_proc.ਵਿੱਚ, "w");
	ਅੰਕੜੇ.ਗਿਣਤੀ_2 = 0;
	ਅੰਕੜੇ.batch_ਮਾਪ = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &ਅੰਕੜੇ.batch_ਮਾਪ);

	/* If configured as 0, then remove limit. */
	ਜੇ (!ਅੰਕੜੇ.batch_ਮਾਪ)
		ਅੰਕੜੇ.batch_ਮਾਪ = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
	ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕੜੇ.batch_ਮਾਪ > 0)
		ਅੰਕੜੇ.batch_ਮਾਪ--; /* Decrease for equality on limit. */

	for_each_loose_file_ਵਿੱਚ_ਸਰੋਤ(r->objects->ਸਰੋਤ,
				      write_loose_object_ਮੰਜ਼ਿਲ_stdin,
				      ਨਿਰਾ, ਨਿਰਾ, &ਅੰਕੜੇ);

	fclose(ਅੰਕੜੇ.ਵਿੱਚ);

	ਜੇ (ਅੰਤ_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		ਨਤੀਜਾ = 1;
	}

	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_loose_objects(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
					  ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ prune_packed(opts) || pack_loose(opts);
}

ਸਥਿਰ ਪੂਰਨ incremental_repack_auto_condition(ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ packed_git *p;
	ਪੂਰਨ incremental_repack_auto_ਹੱਦਬੰਦੀ = 10;
	ਪੂਰਨ ਗਿਣਤੀ_2 = 0;

	prepare_repo_settings(the_repository);
	ਜੇ (!the_repository->settings.core_multi_pack_ਸੂਚਕਾਂਕ)
		ਵਾਪਸ 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_ਹੱਦਬੰਦੀ);

	ਜੇ (!incremental_repack_auto_ਹੱਦਬੰਦੀ)
		ਵਾਪਸ 0;
	ਜੇ (incremental_repack_auto_ਹੱਦਬੰਦੀ < 0)
		ਵਾਪਸ 1;

	repo_for_each_pack(the_repository, p) {
		ਜੇ (ਗਿਣਤੀ_2 >= incremental_repack_auto_ਹੱਦਬੰਦੀ)
			ਰੋਕੋ;
		ਜੇ (!p->multi_pack_ਸੂਚਕਾਂਕ)
			ਗਿਣਤੀ_2++;
	}

	ਵਾਪਸ ਗਿਣਤੀ_2 >= incremental_repack_auto_ਹੱਦਬੰਦੀ;
}

ਸਥਿਰ ਪੂਰਨ multi_pack_ਸੂਚਕਾਂਕ_write(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts)
{
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	ਬੱਚਾ.git_cmd = 1;
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "multi-pack-index", "write", ਨਿਰਾ);

	ਜੇ (opts->quiet)
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--no-progress");
	ਨਹੀਂਤਾਂ
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--progress");

	ਜੇ (ਲੜੀਵਾਰ_command(&ਬੱਚਾ))
		ਵਾਪਸ error(_("failed to write multi-pack-index"));

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ multi_pack_ਸੂਚਕਾਂਕ_expire(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts)
{
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	ਬੱਚਾ.git_cmd = 1;
	ਬੱਚਾ.odb_ਮੰਜ਼ਿਲ_close = the_repository->objects;
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "multi-pack-index", "expire", ਨਿਰਾ);

	ਜੇ (opts->quiet)
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--no-progress");
	ਨਹੀਂਤਾਂ
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--progress");

	ਜੇ (ਲੜੀਵਾਰ_command(&ਬੱਚਾ))
		ਵਾਪਸ error(_("'git multi-pack-index expire' failed"));

	ਵਾਪਸ 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TWO_GIGABYTES (INT32_MAX)

ਸਥਿਰ off_t get_auto_pack_ਮਾਪ(ਖਾਲੀ)
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
	off_t max_ਮਾਪ = 0;
	off_t ਦੂਜਾ_ਸਭ_ਤੋਂ_ਵੱਡਾ_ਮਾਪ = 0;
	off_t ਨਤੀਜਾ_ਮਾਪ;
	ਬਣਤਰ packed_git *p;
	ਬਣਤਰ repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		ਜੇ (p->pack_ਮਾਪ > max_ਮਾਪ) {
			ਦੂਜਾ_ਸਭ_ਤੋਂ_ਵੱਡਾ_ਮਾਪ = max_ਮਾਪ;
			max_ਮਾਪ = p->pack_ਮਾਪ;
		} ਨਹੀਂਤਾਂ ਜੇ (p->pack_ਮਾਪ > ਦੂਜਾ_ਸਭ_ਤੋਂ_ਵੱਡਾ_ਮਾਪ)
			ਦੂਜਾ_ਸਭ_ਤੋਂ_ਵੱਡਾ_ਮਾਪ = p->pack_ਮਾਪ;
	}

	ਨਤੀਜਾ_ਮਾਪ = ਦੂਜਾ_ਸਭ_ਤੋਂ_ਵੱਡਾ_ਮਾਪ + 1;

	/* But limit ourselves to a batch size of 2g */
	ਜੇ (ਨਤੀਜਾ_ਮਾਪ > TWO_GIGABYTES)
		ਨਤੀਜਾ_ਮਾਪ = TWO_GIGABYTES;

	ਵਾਪਸ ਨਤੀਜਾ_ਮਾਪ;
}

ਸਥਿਰ ਪੂਰਨ multi_pack_ਸੂਚਕਾਂਕ_repack(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts)
{
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	ਬੱਚਾ.git_cmd = 1;
	ਬੱਚਾ.odb_ਮੰਜ਼ਿਲ_close = the_repository->objects;
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "multi-pack-index", "repack", ਨਿਰਾ);

	ਜੇ (opts->quiet)
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--no-progress");
	ਨਹੀਂਤਾਂ
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--progress");

	strvec_pushf(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_ਮਾਪ());

	ਜੇ (ਲੜੀਵਾਰ_command(&ਬੱਚਾ))
		ਵਾਪਸ error(_("'git multi-pack-index repack' failed"));

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_incremental_repack(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
					       ਬਣਤਰ gc_config *cfg ਅਣਵਰਤਿਆ)
{
	prepare_repo_settings(the_repository);
	ਜੇ (!the_repository->settings.core_multi_pack_ਸੂਚਕਾਂਕ) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		ਵਾਪਸ 0;
	}

	ਜੇ (multi_pack_ਸੂਚਕਾਂਕ_write(opts))
		ਵਾਪਸ 1;
	ਜੇ (multi_pack_ਸੂਚਕਾਂਕ_expire(opts))
		ਵਾਪਸ 1;
	ਜੇ (multi_pack_ਸੂਚਕਾਂਕ_repack(opts))
		ਵਾਪਸ 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ maintenance_task_geometric_repack(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
					     ਬਣਤਰ gc_config *cfg)
{
	ਬਣਤਰ odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	ਜੇ (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	ਵਾਪਸ odb_optimize(the_repository->objects, &odb_opts);
}

ਸਥਿਰ ਪੂਰਨ geometric_repack_auto_condition(ਬਣਤਰ gc_config *cfg)
{
	ਬਣਤਰ odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	ਵਾਪਸ odb_optimize_required(the_repository->objects, &opts);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*maintenance_task_fn)(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
				   ਬਣਤਰ gc_config *cfg);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*maintenance_auto_fn)(ਬਣਤਰ gc_config *cfg);

ਬਣਤਰ maintenance_task {
	ਅਚਲ ਅੱਖਰ *name;

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

ਸਥਿਰ ਅਚਲ ਬਣਤਰ maintenance_task tasks[] = {
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
	[TASK_COMMIT_ਗ੍ਰਾਫ] = {
		.name = "commit-graph",
		.background = maintenance_task_commit_ਗ੍ਰਾਫ,
		.auto_condition = should_write_commit_ਗ੍ਰਾਫ,
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

ਗਿਣਤੀ task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

ਗਿਣਤੀ auto_gc_hook_ਨਤੀਜਾ {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_ਲੜੀਵਾਰ = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

ਸਥਿਰ ਪੂਰਨ maybe_ਲੜੀਵਾਰ_task(ਅਚਲ ਬਣਤਰ maintenance_task *task,
			  ਬਣਤਰ repository *repo,
			  ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
			  ਬਣਤਰ gc_config *cfg,
			  ਗਿਣਤੀ task_phase phase,
			  ਗਿਣਤੀ auto_gc_hook_ਨਤੀਜਾ *auto_gc_hook_ਨਤੀਜਾ)
{
	ਪੂਰਨ foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ਅਚਲ ਅੱਖਰ *region = foreground ? "maintenance foreground" : "maintenance";
	ਪੂਰਨ ret = 0;

	ਜੇ (!fn)
		ਵਾਪਸ 0;
	ਜੇ (opts->auto_flag) {
		ਜੇ (*auto_gc_hook_ਨਤੀਜਾ == AUTO_GC_HOOK_SKIP)
			ਵਾਪਸ 0;

		ਜੇ (!task->auto_condition || !task->auto_condition(cfg))
			ਵਾਪਸ 0;

		ਜੇ (*auto_gc_hook_ਨਤੀਜਾ == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_ਨਤੀਜਾ = ਲੜੀਵਾਰ_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_ਲੜੀਵਾਰ;
		ਜੇ (*auto_gc_hook_ਨਤੀਜਾ == AUTO_GC_HOOK_SKIP)
			ਵਾਪਸ 0;
	}

	trace2_region_enter(region, task->name, repo);
	ਜੇ (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ maintenance_ਲੜੀਵਾਰ_tasks(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
				 ਬਣਤਰ gc_config *cfg)
{
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਬਣਤਰ ਤਾਲਾ_file lk;
	ਬਣਤਰ repository *r = the_repository;
	ਅੱਖਰ *ਤਾਲਾ_ਰਸਤਾ = xstrfmt("%s/maintenance", r->objects->ਸਰੋਤ->ਰਸਤਾ);
	ਗਿਣਤੀ auto_gc_hook_ਨਤੀਜਾ auto_gc_hook_ਨਤੀਜਾ = AUTO_GC_HOOK_UNDECIDED;

	ਜੇ (repo_hold_ਤਾਲਾ_file_for_update(r, &lk, ਤਾਲਾ_ਰਸਤਾ, ਤਾਲਾ_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		ਜੇ (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				ਤਾਲਾ_ਰਸਤਾ);
		ਮੁਕਤ(ਤਾਲਾ_ਰਸਤਾ);
		ਵਾਪਸ 0;
	}
	ਮੁਕਤ(ਤਾਲਾ_ਰਸਤਾ);

	ਲਈ (ਮਾਪ_t i = 0; i < opts->tasks_nr; i++)
		ਜੇ (maybe_ਲੜੀਵਾਰ_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_ਨਤੀਜਾ))
			ਨਤੀਜਾ = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	ਜੇ (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	ਲਈ (ਮਾਪ_t i = 0; i < opts->tasks_nr; i++)
		ਜੇ (maybe_ਲੜੀਵਾਰ_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_ਨਤੀਜਾ))
			ਨਤੀਜਾ = 1;

	rollback_ਤਾਲਾ_file(&lk);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਗਿਣਤੀ maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

ਬਣਤਰ maintenance_strategy {
	ਬਣਤਰ {
		ਬਿਨਾਨਿਸ਼ਾਨੀ type;
		ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ ਸਮਾਂ_ਸਾਰਣੀ;
	} tasks[TASK_ਗਿਣਤੀ];
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ maintenance_strategy none_strategy = { 0 };

ਸਥਿਰ ਅਚਲ ਬਣਤਰ maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_DAILY,
		},
	},
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ maintenance_strategy incremental_strategy = {
	.tasks = {
		[TASK_COMMIT_ਗ੍ਰਾਫ] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_HOURLY,
		},
		[TASK_PREFETCH] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_HOURLY,
		},
		[TASK_INCREMENTAL_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_DAILY,
		},
		[TASK_LOOSE_OBJECTS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY,
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

ਸਥਿਰ ਅਚਲ ਬਣਤਰ maintenance_strategy geometric_strategy = {
	.tasks = {
		[TASK_COMMIT_ਗ੍ਰਾਫ] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_HOURLY,
		},
		[TASK_GEOMETRIC_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_DAILY,
		},
		[TASK_RERERE_GC] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY,
		},
		[TASK_REFLOG_EXPIRE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY,
		},
		[TASK_WORKTREE_PRUNE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.ਸਮਾਂ_ਸਾਰਣੀ = ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY,
		},
	},
};

ਸਥਿਰ ਬਣਤਰ maintenance_strategy parse_maintenance_strategy(ਅਚਲ ਅੱਖਰ *name)
{
	ਜੇ (!strcasecmp(name, "incremental"))
		ਵਾਪਸ incremental_strategy;
	ਜੇ (!strcasecmp(name, "gc"))
		ਵਾਪਸ gc_strategy;
	ਜੇ (!strcasecmp(name, "geometric"))
		ਵਾਪਸ geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

ਸਥਿਰ ਖਾਲੀ initialize_task_config(ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts *opts,
				   ਅਚਲ ਬਣਤਰ string_list *selected_tasks)
{
	ਬਣਤਰ strbuf config_name = STRBUF_INIT;
	ਬਣਤਰ maintenance_strategy strategy;
	ਗਿਣਤੀ maintenance_type type;
	ਅਚਲ ਅੱਖਰ *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	ਜੇ (selected_tasks->nr) {
		ਲਈ (ਮਾਪ_t i = 0; i < selected_tasks->nr; i++) {
			ਗਿਣਤੀ maintenance_task_label label = (intptr_t)selected_tasks->ਚੀਜ਼ਾਂ[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		ਵਾਪਸ;
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
	ਜੇ (opts->ਸਮਾਂ_ਸਾਰਣੀ) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} ਨਹੀਂਤਾਂ {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	ਜੇ (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	ਲਈ (ਮਾਪ_t i = 0; i < TASK_ਗਿਣਤੀ; i++) {
		ਪੂਰਨ config_ਮੁੱਲ;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		ਜੇ (!repo_config_get_bool(the_repository, config_name.buf, &config_ਮੁੱਲ))
			strategy.tasks[i].type = config_ਮੁੱਲ ? type : 0;
		ਜੇ (!(strategy.tasks[i].type & type))
			ਜਾਰੀ;

		ਜੇ (opts->ਸਮਾਂ_ਸਾਰਣੀ) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			ਜੇ (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].ਸਮਾਂ_ਸਾਰਣੀ = parse_ਸਮਾਂ_ਸਾਰਣੀ(config_str);
			ਜੇ (strategy.tasks[i].ਸਮਾਂ_ਸਾਰਣੀ < opts->ਸਮਾਂ_ਸਾਰਣੀ)
				ਜਾਰੀ;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

ਸਥਿਰ ਪੂਰਨ task_option_parse(ਅਚਲ ਬਣਤਰ option *opt,
			     ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ string_list *selected_tasks = opt->ਮੁੱਲ;
	ਮਾਪ_t i;

	BUG_ON_OPT_NEG(unset);

	ਲਈ (i = 0; i < TASK_ਗਿਣਤੀ; i++)
		ਜੇ (!strcasecmp(tasks[i].name, arg))
			ਰੋਕੋ;
	ਜੇ (i >= TASK_ਗਿਣਤੀ) {
		error(_("'%s' is not a valid task"), arg);
		ਵਾਪਸ 1;
	}

	ਜੇ (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		ਵਾਪਸ 1;
	}

	string_list_append(selected_tasks, arg)->util = (ਖਾਲੀ *)(intptr_t)i;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ maintenance_ਲੜੀਵਾਰ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts opts = MAINTENANCE_ਲੜੀਵਾਰ_OPTS_INIT;
	ਬਣਤਰ string_list selected_tasks = STRING_LIST_INIT_DUP;
	ਬਣਤਰ gc_config cfg = GC_CONFIG_INIT;
	ਬਣਤਰ option builtin_maintenance_ਲੜੀਵਾਰ_options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform maintenance in the background")),
		OPT_CALLBACK(0, "schedule", &opts.ਸਮਾਂ_ਸਾਰਣੀ, N_("frequency"),
			     N_("run tasks based on frequency"),
			     maintenance_opt_ਸਮਾਂ_ਸਾਰਣੀ),
		OPT_BOOL(0, "quiet", &opts.quiet,
			 N_("do not report progress or other information over stderr")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			N_("run a specific task"),
			PARSE_OPT_NONEG, task_option_parse),
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ ret;

	opts.quiet = !isatty(2);

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_maintenance_ਲੜੀਵਾਰ_options,
			     builtin_maintenance_ਲੜੀਵਾਰ_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	die_for_incompatible_opt2(opts.auto_flag, "--auto",
				  opts.ਸਮਾਂ_ਸਾਰਣੀ, "--schedule=");
	die_for_incompatible_opt2(selected_tasks.nr, "--task=",
				  opts.ਸਮਾਂ_ਸਾਰਣੀ, "--schedule=");

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	ਜੇ (argc != 0)
		usage_with_options(builtin_maintenance_ਲੜੀਵਾਰ_usage,
				   builtin_maintenance_ਲੜੀਵਾਰ_options);

	ret = maintenance_ਲੜੀਵਾਰ_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_ਲੜੀਵਾਰ_opts_release(&opts);
	gc_config_release(&cfg);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਅੱਖਰ *get_maintpath(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	ਵਾਪਸ strbuf_detach(&sb, ਨਿਰਾ);
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_maintenance_ਰਜਿਸਟਰ_usage[] = {
	"git maintenance register [--config-file <path>]",
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ maintenance_ਰਜਿਸਟਰ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅੱਖਰ *config_file = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_ਸਿਰਾ(),
	};
	ਪੂਰਨ ਮਿਲਿਆ_2 = 0;
	ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ = "maintenance.repo";
	ਅੱਖਰ *maintpath = get_maintpath();
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਅਚਲ ਬਣਤਰ string_list *list;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_maintenance_ਰਜਿਸਟਰ_usage, 0);
	ਜੇ (argc)
		usage_with_options(builtin_maintenance_ਰਜਿਸਟਰ_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_ਸੈੱਟ(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	ਜੇ (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_ਸੈੱਟ(the_repository, "maintenance.strategy", "incremental");

	ਜੇ (!repo_config_get_string_multi(the_repository, ਕੁੰਜੀ, &list)) {
		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, list) {
			ਜੇ (!strcmp(maintpath, ਵਸਤੂ->string)) {
				ਮਿਲਿਆ_2 = 1;
				ਰੋਕੋ;
			}
		}
	}

	ਜੇ (!ਮਿਲਿਆ_2) {
		ਪੂਰਨ rc;
		ਅੱਖਰ *global_config_file = ਨਿਰਾ;

		ਜੇ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ਜੇ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_ਸੈੱਟ_multivar_ਵਿੱਚ_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, ਨਿਰਾ, 0);
		ਮੁਕਤ(global_config_file);

		ਜੇ (rc)
			die(_("unable to add '%s' value of '%s'"),
			    ਕੁੰਜੀ, maintpath);
	}

	ਮੁਕਤ(maintpath);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ maintenance_unregister(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ force = 0;
	ਅੱਖਰ *config_file = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_ਸਿਰਾ(),
	};
	ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ = "maintenance.repo";
	ਅੱਖਰ *maintpath = get_maintpath();
	ਪੂਰਨ ਮਿਲਿਆ_2 = 0;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਅਚਲ ਬਣਤਰ string_list *list;
	ਬਣਤਰ config_ਸੈੱਟ cs = { { 0 } };

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_maintenance_unregister_usage, 0);
	ਜੇ (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	ਜੇ (config_file) {
		git_configset_init(&cs);
		git_configset_ਜੋੜੋ_file(&cs, config_file);
	}
	ਜੇ (!(config_file
	      ? git_configset_get_string_multi(&cs, ਕੁੰਜੀ, &list)
	      : repo_config_get_string_multi(the_repository, ਕੁੰਜੀ, &list))) {
		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, list) {
			ਜੇ (!strcmp(maintpath, ਵਸਤੂ->string)) {
				ਮਿਲਿਆ_2 = 1;
				ਰੋਕੋ;
			}
		}
	}

	ਜੇ (ਮਿਲਿਆ_2) {
		ਪੂਰਨ rc;
		ਅੱਖਰ *global_config_file = ਨਿਰਾ;

		ਜੇ (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ਜੇ (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_ਸੈੱਟ_multivar_ਵਿੱਚ_file_gently(the_repository,
			config_file, ਕੁੰਜੀ, ਨਿਰਾ, maintpath, ਨਿਰਾ,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_ਮੁੱਲ);
		ਮੁਕਤ(global_config_file);

		ਜੇ (rc &&
		    (!force || rc == CONFIG_NOTHING_ਸੈੱਟ))
			die(_("unable to unset '%s' value of '%s'"),
			    ਕੁੰਜੀ, maintpath);
	} ਨਹੀਂਤਾਂ ਜੇ (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	ਮੁਕਤ(maintpath);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_frequency(ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ ਸਮਾਂ_ਸਾਰਣੀ)
{
	ਚੋਣ (ਸਮਾਂ_ਸਾਰਣੀ) {
	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_HOURLY:
		ਵਾਪਸ "hourly";
	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_DAILY:
		ਵਾਪਸ "daily";
	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY:
		ਵਾਪਸ "weekly";
	ਮੂਲ:
		BUG("invalid schedule %d", ਸਮਾਂ_ਸਾਰਣੀ);
	}
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_extra_config_parameters(ਖਾਲੀ) {
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਨਤੀਜਾ = ਨਿਰਾ;
	ਬਣਤਰ strbuf builder = STRBUF_INIT;

	ਜੇ (ਨਤੀਜਾ)
		ਵਾਪਸ ਨਤੀਜਾ;

	ਲਈ (ਅਚਲ ਅੱਖਰ **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	ਨਤੀਜਾ = strbuf_detach(&builder, ਨਿਰਾ);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_extra_launchctl_ਲੜੀਆਂ(ਖਾਲੀ) {
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਨਤੀਜਾ = ਨਿਰਾ;
	ਬਣਤਰ strbuf builder = STRBUF_INIT;

	ਜੇ (ਨਤੀਜਾ)
		ਵਾਪਸ ਨਤੀਜਾ;

	ਲਈ (ਅਚਲ ਅੱਖਰ **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	ਨਤੀਜਾ = strbuf_detach(&builder, ਨਿਰਾ);
	ਵਾਪਸ ਨਤੀਜਾ;
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
ਸਥਿਰ ਪੂਰਨ get_ਸਮਾਂ_ਸਾਰਣੀ_cmd(ਅਚਲ ਅੱਖਰ *cmd, ਪੂਰਨ *is_ਉਪਲਬਧ, ਅੱਖਰ **ਬਾਹਰਲਾ)
{
	ਅੱਖਰ *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ string_list list = STRING_LIST_INIT_NODUP;

	ਜੇ (!testing) {
		ਜੇ (ਬਾਹਰਲਾ)
			*ਬਾਹਰਲਾ = xstrdup(cmd);
		ਵਾਪਸ 0;
	}

	ਜੇ (is_ਉਪਲਬਧ)
		*is_ਉਪਲਬਧ = 0;

	string_list_ਵੰਡੋ_ਵਿੱਚ_ਰੱਖੋ(&list, testing, ",", -1);
	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &list) {
		ਬਣਤਰ string_list pair = STRING_LIST_INIT_NODUP;

		ਜੇ (string_list_ਵੰਡੋ_ਵਿੱਚ_ਰੱਖੋ(&pair, ਵਸਤੂ->string, ":", 2) != 2)
			ਜਾਰੀ;

		ਜੇ (!strcmp(cmd, pair.ਚੀਜ਼ਾਂ[0].string)) {
			ਜੇ (ਬਾਹਰਲਾ)
				*ਬਾਹਰਲਾ = xstrdup(pair.ਚੀਜ਼ਾਂ[1].string);
			ਜੇ (is_ਉਪਲਬਧ)
				*is_ਉਪਲਬਧ = 1;
			string_list_clear(&pair, 0);
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		string_list_clear(&pair, 0);
	}

	ਜੇ (ਬਾਹਰਲਾ)
		*ਬਾਹਰਲਾ = xstrdup(cmd);

ਬਾਹਰਲਾ:
	string_list_clear(&list, 0);
	ਮੁਕਤ(testing);
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ get_random_minute(ਖਾਲੀ)
{
	/* Use a static value when under tests. */
	ਜੇ (getenv("GIT_TEST_MAINT_SCHEDULER"))
		ਵਾਪਸ 13;

	ਵਾਪਸ git_rand(0) % 60;
}

ਸਥਿਰ ਪੂਰਨ is_launchctl_ਉਪਲਬਧ(ਖਾਲੀ)
{
	ਪੂਰਨ is_ਉਪਲਬਧ;
	ਜੇ (get_ਸਮਾਂ_ਸਾਰਣੀ_cmd("launchctl", &is_ਉਪਲਬਧ, ਨਿਰਾ))
		ਵਾਪਸ is_ਉਪਲਬਧ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
	ਵਾਪਸ 1;
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਅੱਖਰ *launchctl_service_name(ਅਚਲ ਅੱਖਰ *frequency)
{
	ਬਣਤਰ strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	ਵਾਪਸ strbuf_detach(&label, ਨਿਰਾ);
}

ਸਥਿਰ ਅੱਖਰ *launchctl_service_filename(ਅਚਲ ਅੱਖਰ *name)
{
	ਅੱਖਰ *expanded;
	ਬਣਤਰ strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_ਰਸਤਾ(filename.buf, 1);
	ਜੇ (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	ਵਾਪਸ expanded;
}

ਸਥਿਰ ਅੱਖਰ *launchctl_get_uid(ਖਾਲੀ)
{
	ਵਾਪਸ xstrfmt("gui/%d", getuid());
}

ਸਥਿਰ ਪੂਰਨ launchctl_boot_plist(ਪੂਰਨ enable, ਅਚਲ ਅੱਖਰ *filename)
{
	ਅੱਖਰ *cmd;
	ਪੂਰਨ ਨਤੀਜਾ;
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;
	ਅੱਖਰ *uid = launchctl_get_uid();

	get_ਸਮਾਂ_ਸਾਰਣੀ_cmd("launchctl", ਨਿਰਾ, &cmd);
	strvec_ਵੰਡੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, cmd);
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, enable ? "bootstrap" : "bootout", uid,
		     filename, ਨਿਰਾ);

	ਬੱਚਾ.no_stderr = 1;
	ਬੱਚਾ.no_stdout = 1;

	ਜੇ (ਸ਼ੁਰੂ_command(&ਬੱਚਾ))
		die(_("failed to start launchctl"));

	ਨਤੀਜਾ = ਅੰਤ_command(&ਬੱਚਾ);

	ਮੁਕਤ(cmd);
	ਮੁਕਤ(uid);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ launchctl_remove_plist(ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ ਸਮਾਂ_ਸਾਰਣੀ)
{
	ਅਚਲ ਅੱਖਰ *frequency = get_frequency(ਸਮਾਂ_ਸਾਰਣੀ);
	ਅੱਖਰ *name = launchctl_service_name(frequency);
	ਅੱਖਰ *filename = launchctl_service_filename(name);
	ਪੂਰਨ ਨਤੀਜਾ = launchctl_boot_plist(0, filename);
	unlink(filename);
	ਮੁਕਤ(filename);
	ਮੁਕਤ(name);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ launchctl_remove_plists(ਖਾਲੀ)
{
	ਵਾਪਸ launchctl_remove_plist(ਸਮਾਂ_ਸਾਰਣੀ_HOURLY) ||
	       launchctl_remove_plist(ਸਮਾਂ_ਸਾਰਣੀ_DAILY) ||
	       launchctl_remove_plist(ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY);
}

ਸਥਿਰ ਪੂਰਨ launchctl_list_ਸ਼ਾਮਲ_ਹੈ_plist(ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *cmd)
{
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	strvec_ਵੰਡੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, cmd);
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "list", name, ਨਿਰਾ);

	ਬੱਚਾ.no_stderr = 1;
	ਬੱਚਾ.no_stdout = 1;

	ਜੇ (ਸ਼ੁਰੂ_command(&ਬੱਚਾ))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	ਵਾਪਸ !ਅੰਤ_command(&ਬੱਚਾ);
}

ਸਥਿਰ ਪੂਰਨ launchctl_ਸਮਾਂ_ਸਾਰਣੀ_plist(ਅਚਲ ਅੱਖਰ *exec_ਰਸਤਾ, ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ ਸਮਾਂ_ਸਾਰਣੀ)
{
	ਪੂਰਨ i, fd;
	ਅਚਲ ਅੱਖਰ *preamble, *repeat;
	ਅਚਲ ਅੱਖਰ *frequency = get_frequency(ਸਮਾਂ_ਸਾਰਣੀ);
	ਅੱਖਰ *name = launchctl_service_name(frequency);
	ਅੱਖਰ *filename = launchctl_service_filename(name);
	ਬਣਤਰ ਤਾਲਾ_file lk = ਤਾਲਾ_INIT;
	ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਤਾਲਾ_file_timeout_ms = ULONG_MAX;
	ਬਣਤਰ strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	ਬਣਤਰ stat st;
	ਅੱਖਰ *cmd;
	ਪੂਰਨ minute = get_random_minute();

	get_ਸਮਾਂ_ਸਾਰਣੀ_cmd("launchctl", ਨਿਰਾ, &cmd);
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
	strbuf_addf(&plist, preamble, name, exec_ਰਸਤਾ, exec_ਰਸਤਾ,
		    get_extra_launchctl_ਲੜੀਆਂ(), frequency);

	ਚੋਣ (ਸਮਾਂ_ਸਾਰਣੀ) {
	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		ਲਈ (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ਰੋਕੋ;

	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		ਲਈ (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ਰੋਕੋ;

	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		ਰੋਕੋ;

	ਮੂਲ:
		/* unreachable */
		ਰੋਕੋ;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	ਜੇ ((ਲੰਮਾ)ਤਾਲਾ_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &ਤਾਲਾ_file_timeout_ms))
		ਤਾਲਾ_file_timeout_ms = 150;

	fd = hold_ਤਾਲਾ_file_for_update_timeout(&lk, filename, ਤਾਲਾ_DIE_ON_ERROR,
					       ਤਾਲਾ_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	ਜੇ (!stat(filename, &st) && st.st_ਮਾਪ == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_ਸ਼ਾਮਲ_ਹੈ_plist(name, cmd))
		rollback_ਤਾਲਾ_file(&lk);
	ਨਹੀਂਤਾਂ {
		ਜੇ (write_ਵਿੱਚ_ਭਰਿਆ(fd, plist.buf, plist.len) < 0 ||
		    commit_ਤਾਲਾ_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		ਜੇ (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	ਮੁਕਤ(filename);
	ਮੁਕਤ(name);
	ਮੁਕਤ(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ launchctl_ਜੋੜੋ_plists(ਖਾਲੀ)
{
	ਅਚਲ ਅੱਖਰ *exec_ਰਸਤਾ = git_exec_ਰਸਤਾ();

	ਵਾਪਸ launchctl_ਸਮਾਂ_ਸਾਰਣੀ_plist(exec_ਰਸਤਾ, ਸਮਾਂ_ਸਾਰਣੀ_HOURLY) ||
	       launchctl_ਸਮਾਂ_ਸਾਰਣੀ_plist(exec_ਰਸਤਾ, ਸਮਾਂ_ਸਾਰਣੀ_DAILY) ||
	       launchctl_ਸਮਾਂ_ਸਾਰਣੀ_plist(exec_ਰਸਤਾ, ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY);
}

ਸਥਿਰ ਪੂਰਨ launchctl_update_ਸਮਾਂ_ਸਾਰਣੀ(ਪੂਰਨ ਲੜੀਵਾਰ_maintenance, ਪੂਰਨ fd ਅਣਵਰਤਿਆ)
{
	ਜੇ (ਲੜੀਵਾਰ_maintenance)
		ਵਾਪਸ launchctl_ਜੋੜੋ_plists();
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ launchctl_remove_plists();
}

ਸਥਿਰ ਪੂਰਨ is_schtasks_ਉਪਲਬਧ(ਖਾਲੀ)
{
	ਪੂਰਨ is_ਉਪਲਬਧ;
	ਜੇ (get_ਸਮਾਂ_ਸਾਰਣੀ_cmd("schtasks", &is_ਉਪਲਬਧ, ਨਿਰਾ))
		ਵਾਪਸ is_ਉਪਲਬਧ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GIT_WINDOWS_NATIVE
	ਵਾਪਸ 1;
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਅੱਖਰ *schtasks_task_name(ਅਚਲ ਅੱਖਰ *frequency)
{
	ਬਣਤਰ strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	ਵਾਪਸ strbuf_detach(&label, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ schtasks_remove_task(ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ ਸਮਾਂ_ਸਾਰਣੀ)
{
	ਅੱਖਰ *cmd;
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;
	ਅਚਲ ਅੱਖਰ *frequency = get_frequency(ਸਮਾਂ_ਸਾਰਣੀ);
	ਅੱਖਰ *name = schtasks_task_name(frequency);

	get_ਸਮਾਂ_ਸਾਰਣੀ_cmd("schtasks", ਨਿਰਾ, &cmd);
	strvec_ਵੰਡੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, cmd);
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "/delete", "/tn", name, "/f", ਨਿਰਾ);
	ਮੁਕਤ(name);
	ਮੁਕਤ(cmd);

	ਵਾਪਸ ਲੜੀਵਾਰ_command(&ਬੱਚਾ);
}

ਸਥਿਰ ਪੂਰਨ schtasks_remove_tasks(ਖਾਲੀ)
{
	ਵਾਪਸ schtasks_remove_task(ਸਮਾਂ_ਸਾਰਣੀ_HOURLY) ||
	       schtasks_remove_task(ਸਮਾਂ_ਸਾਰਣੀ_DAILY) ||
	       schtasks_remove_task(ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY);
}

ਸਥਿਰ ਪੂਰਨ schtasks_ਸਮਾਂ_ਸਾਰਣੀ_task(ਅਚਲ ਅੱਖਰ *exec_ਰਸਤਾ, ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ ਸਮਾਂ_ਸਾਰਣੀ)
{
	ਅੱਖਰ *cmd;
	ਪੂਰਨ ਨਤੀਜਾ;
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;
	ਅਚਲ ਅੱਖਰ *xml;
	ਬਣਤਰ tempfile *tfile;
	ਅਚਲ ਅੱਖਰ *frequency = get_frequency(ਸਮਾਂ_ਸਾਰਣੀ);
	ਅੱਖਰ *name = schtasks_task_name(frequency);
	ਬਣਤਰ strbuf tfilename = STRBUF_INIT;
	ਪੂਰਨ minute = get_random_minute();

	get_ਸਮਾਂ_ਸਾਰਣੀ_cmd("schtasks", ਨਿਰਾ, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	ਜੇ (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	ਚੋਣ (ਸਮਾਂ_ਸਾਰਣੀ) {
	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_HOURLY:
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
		ਰੋਕੋ;

	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_DAILY:
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
		ਰੋਕੋ;

	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY:
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
		ਰੋਕੋ;

	ਮੂਲ:
		ਰੋਕੋ;
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
	fprintf(tfile->fp, xml, exec_ਰਸਤਾ, exec_ਰਸਤਾ,
		get_extra_config_parameters(), frequency);
	strvec_ਵੰਡੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, cmd);
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "/create", "/tn", name, "/f", "/xml",
				  get_tempfile_ਰਸਤਾ(tfile), ਨਿਰਾ);
	close_tempfile_gently(tfile);

	ਬੱਚਾ.no_stdout = 1;
	ਬੱਚਾ.no_stderr = 1;

	ਜੇ (ਸ਼ੁਰੂ_command(&ਬੱਚਾ))
		die(_("failed to start schtasks"));
	ਨਤੀਜਾ = ਅੰਤ_command(&ਬੱਚਾ);

	delete_tempfile(&tfile);
	ਮੁਕਤ(name);
	ਮੁਕਤ(cmd);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ schtasks_ਸਮਾਂ_ਸਾਰਣੀ_tasks(ਖਾਲੀ)
{
	ਅਚਲ ਅੱਖਰ *exec_ਰਸਤਾ = git_exec_ਰਸਤਾ();

	ਵਾਪਸ schtasks_ਸਮਾਂ_ਸਾਰਣੀ_task(exec_ਰਸਤਾ, ਸਮਾਂ_ਸਾਰਣੀ_HOURLY) ||
	       schtasks_ਸਮਾਂ_ਸਾਰਣੀ_task(exec_ਰਸਤਾ, ਸਮਾਂ_ਸਾਰਣੀ_DAILY) ||
	       schtasks_ਸਮਾਂ_ਸਾਰਣੀ_task(exec_ਰਸਤਾ, ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY);
}

ਸਥਿਰ ਪੂਰਨ schtasks_update_ਸਮਾਂ_ਸਾਰਣੀ(ਪੂਰਨ ਲੜੀਵਾਰ_maintenance, ਪੂਰਨ fd ਅਣਵਰਤਿਆ)
{
	ਜੇ (ਲੜੀਵਾਰ_maintenance)
		ਵਾਪਸ schtasks_ਸਮਾਂ_ਸਾਰਣੀ_tasks();
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ schtasks_remove_tasks();
}

MAYBE_ਅਣਵਰਤਿਆ
ਸਥਿਰ ਪੂਰਨ check_crontab_process(ਅਚਲ ਅੱਖਰ *cmd)
{
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	strvec_ਵੰਡੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, cmd);
	strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "-l");
	ਬੱਚਾ.no_stdin = 1;
	ਬੱਚਾ.no_stdout = 1;
	ਬੱਚਾ.no_stderr = 1;
	ਬੱਚਾ.silent_exec_failure = 1;

	ਜੇ (ਸ਼ੁਰੂ_command(&ਬੱਚਾ))
		ਵਾਪਸ 0;
	/* Ignore exit code, as an empty crontab will return error. */
	ਅੰਤ_command(&ਬੱਚਾ);
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ is_crontab_ਉਪਲਬਧ(ਖਾਲੀ)
{
	ਅੱਖਰ *cmd;
	ਪੂਰਨ is_ਉਪਲਬਧ;
	ਪੂਰਨ ret;

	ਜੇ (get_ਸਮਾਂ_ਸਾਰਣੀ_cmd("crontab", &is_ਉਪਲਬਧ, &cmd)) {
		ret = is_ਉਪਲਬਧ;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#ਨਹੀਂਤਾਂ
	ret = check_crontab_process(cmd);
#ਜੇ_ਅੰਤ

ਬਾਹਰਲਾ:
	ਮੁਕਤ(cmd);
	ਵਾਪਸ ret;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BEGIN_ਸਤਰ "# BEGIN GIT MAINTENANCE SCHEDULE"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਿਰਾ_ਸਤਰ "# END GIT MAINTENANCE SCHEDULE"

ਸਥਿਰ ਪੂਰਨ crontab_update_ਸਮਾਂ_ਸਾਰਣੀ(ਪੂਰਨ ਲੜੀਵਾਰ_maintenance, ਪੂਰਨ fd)
{
	ਅੱਖਰ *cmd;
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਪੂਰਨ ਵਿੱਚ_old_region = 0;
	ਬਣਤਰ ਬੱਚਾ_process crontab_list = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ ਬੱਚਾ_process crontab_edit = ਬੱਚਾ_PROCESS_INIT;
	FILE *cron_list, *cron_ਵਿੱਚ;
	ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;
	ਬਣਤਰ tempfile *tmpedit = ਨਿਰਾ;
	ਪੂਰਨ minute = get_random_minute();

	get_ਸਮਾਂ_ਸਾਰਣੀ_cmd("crontab", ਨਿਰਾ, &cmd);
	strvec_ਵੰਡੋ(&crontab_list.ਦਲੀਲਾਂ, cmd);
	strvec_ਧੱਕੋ(&crontab_list.ਦਲੀਲਾਂ, "-l");
	crontab_list.ਵਿੱਚ = -1;
	crontab_list.ਬਾਹਰਲਾ = dup(fd);
	crontab_list.git_cmd = 0;

	ਜੇ (ਸ਼ੁਰੂ_command(&crontab_list)) {
		ਨਤੀਜਾ = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	ਅੰਤ_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	ਜੇ (!tmpedit) {
		ਨਤੀਜਾ = error(_("failed to create crontab temporary file"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	cron_ਵਿੱਚ = fdopen_tempfile(tmpedit, "w");
	ਜੇ (!cron_ਵਿੱਚ) {
		ਨਤੀਜਾ = error(_("failed to open temporary file"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	ਜਦੋਂ (!strbuf_getline_lf(&ਸਤਰ, cron_list)) {
		ਜੇ (!ਵਿੱਚ_old_region && !strcmp(ਸਤਰ.buf, BEGIN_ਸਤਰ))
			ਵਿੱਚ_old_region = 1;
		ਨਹੀਂਤਾਂ ਜੇ (ਵਿੱਚ_old_region && !strcmp(ਸਤਰ.buf, ਸਿਰਾ_ਸਤਰ))
			ਵਿੱਚ_old_region = 0;
		ਨਹੀਂਤਾਂ ਜੇ (!ਵਿੱਚ_old_region)
			fprintf(cron_ਵਿੱਚ, "%s\n", ਸਤਰ.buf);
	}
	strbuf_release(&ਸਤਰ);

	ਜੇ (ਲੜੀਵਾਰ_maintenance) {
		ਬਣਤਰ strbuf ਸਤਰ_format = STRBUF_INIT;
		ਅਚਲ ਅੱਖਰ *exec_ਰਸਤਾ = git_exec_ਰਸਤਾ();

		fprintf(cron_ਵਿੱਚ, "%s\n", BEGIN_ਸਤਰ);
		fprintf(cron_ਵਿੱਚ,
			"# The following schedule was created by Git\n");
		fprintf(cron_ਵਿੱਚ, "# Any edits made in this region might be\n");
		fprintf(cron_ਵਿੱਚ,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&ਸਤਰ_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_ਰਸਤਾ, exec_ਰਸਤਾ, get_extra_config_parameters());
		fprintf(cron_ਵਿੱਚ, ਸਤਰ_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_ਵਿੱਚ, ਸਤਰ_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_ਵਿੱਚ, ਸਤਰ_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&ਸਤਰ_format);

		fprintf(cron_ਵਿੱਚ, "\n%s\n", ਸਿਰਾ_ਸਤਰ);
	}

	fflush(cron_ਵਿੱਚ);

	strvec_ਵੰਡੋ(&crontab_edit.ਦਲੀਲਾਂ, cmd);
	strvec_ਧੱਕੋ(&crontab_edit.ਦਲੀਲਾਂ, get_tempfile_ਰਸਤਾ(tmpedit));
	crontab_edit.git_cmd = 0;

	ਜੇ (ਸ਼ੁਰੂ_command(&crontab_edit)) {
		ਨਤੀਜਾ = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (ਅੰਤ_command(&crontab_edit))
		ਨਤੀਜਾ = error(_("'crontab' died"));
	ਨਹੀਂਤਾਂ
		fclose(cron_list);

ਬਾਹਰਲਾ:
	delete_tempfile(&tmpedit);
	ਮੁਕਤ(cmd);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ real_is_systemd_timer_ਉਪਲਬਧ(ਖਾਲੀ)
{
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "systemctl", "--user", "list-timers", ਨਿਰਾ);
	ਬੱਚਾ.no_stdin = 1;
	ਬੱਚਾ.no_stdout = 1;
	ਬੱਚਾ.no_stderr = 1;
	ਬੱਚਾ.silent_exec_failure = 1;

	ਜੇ (ਸ਼ੁਰੂ_command(&ਬੱਚਾ))
		ਵਾਪਸ 0;
	ਜੇ (ਅੰਤ_command(&ਬੱਚਾ))
		ਵਾਪਸ 0;
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ is_systemd_timer_ਉਪਲਬਧ(ਖਾਲੀ)
{
	ਪੂਰਨ is_ਉਪਲਬਧ;

	ਜੇ (get_ਸਮਾਂ_ਸਾਰਣੀ_cmd("systemctl", &is_ਉਪਲਬਧ, ਨਿਰਾ))
		ਵਾਪਸ is_ਉਪਲਬਧ;

	ਵਾਪਸ real_is_systemd_timer_ਉਪਲਬਧ();
}

ਸਥਿਰ ਅੱਖਰ *xdg_config_home_systemd(ਅਚਲ ਅੱਖਰ *filename)
{
	ਵਾਪਸ xdg_config_home_for("systemd/user", filename);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

ਸਥਿਰ ਪੂਰਨ systemd_timer_delete_timer_file(ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ ਤਰਜੀਹ)
{
	ਪੂਰਨ ret = 0;
	ਅਚਲ ਅੱਖਰ *frequency = get_frequency(ਤਰਜੀਹ);
	ਅੱਖਰ *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	ਅੱਖਰ *filename = xdg_config_home_systemd(local_timer_name);

	ਜੇ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	ਮੁਕਤ(filename);
	ਮੁਕਤ(local_timer_name);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ systemd_timer_delete_service_template(ਖਾਲੀ)
{
	ਪੂਰਨ ret = 0;
	ਅੱਖਰ *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	ਅੱਖਰ *filename = xdg_config_home_systemd(local_service_name);
	ਜੇ (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	ਮੁਕਤ(filename);
	ਮੁਕਤ(local_service_name);
	ਵਾਪਸ ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
ਸਥਿਰ ਪੂਰਨ systemd_timer_write_timer_file(ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ ਸਮਾਂ_ਸਾਰਣੀ,
					  ਪੂਰਨ minute)
{
	ਪੂਰਨ res = -1;
	ਅੱਖਰ *filename;
	FILE *file;
	ਅਚਲ ਅੱਖਰ *unit;
	ਅੱਖਰ *ਸਮਾਂ_ਸਾਰਣੀ_ਨਮੂਨਾ = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *frequency = get_frequency(ਸਮਾਂ_ਸਾਰਣੀ);
	ਅੱਖਰ *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		ਜਾਓ error;
	}
	file = fopen_or_warn(filename, "w");
	ਜੇ (!file)
		ਜਾਓ error;

	ਚੋਣ (ਸਮਾਂ_ਸਾਰਣੀ) {
	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_HOURLY:
		ਸਮਾਂ_ਸਾਰਣੀ_ਨਮੂਨਾ = xstrfmt("*-*-* 1..23:%02d:00", minute);
		ਰੋਕੋ;

	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_DAILY:
		ਸਮਾਂ_ਸਾਰਣੀ_ਨਮੂਨਾ = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		ਰੋਕੋ;

	ਹਾਲਤ ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY:
		ਸਮਾਂ_ਸਾਰਣੀ_ਨਮੂਨਾ = xstrfmt("Mon 0:%02d:00", minute);
		ਰੋਕੋ;

	ਮੂਲ:
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
	ਜੇ (fprintf(file, unit, ਸਮਾਂ_ਸਾਰਣੀ_ਨਮੂਨਾ) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		ਜਾਓ error;
	}
	ਜੇ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		ਜਾਓ error;
	}

	res = 0;

error:
	ਮੁਕਤ(ਸਮਾਂ_ਸਾਰਣੀ_ਨਮੂਨਾ);
	ਮੁਕਤ(local_timer_name);
	ਮੁਕਤ(filename);
	ਵਾਪਸ res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
ਸਥਿਰ ਪੂਰਨ systemd_timer_write_service_template(ਅਚਲ ਅੱਖਰ *exec_ਰਸਤਾ)
{
	ਪੂਰਨ res = -1;
	ਅੱਖਰ *filename;
	FILE *file;
	ਅਚਲ ਅੱਖਰ *unit;
	ਅੱਖਰ *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		ਜਾਓ error;
	}
	file = fopen_or_warn(filename, "w");
	ਜੇ (!file)
		ਜਾਓ error;

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
	ਜੇ (fprintf(file, unit, exec_ਰਸਤਾ, exec_ਰਸਤਾ, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		ਜਾਓ error;
	}
	ਜੇ (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		ਜਾਓ error;
	}

	res = 0;

error:
	ਮੁਕਤ(local_service_name);
	ਮੁਕਤ(filename);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ systemd_timer_enable_unit(ਪੂਰਨ enable,
				     ਗਿਣਤੀ ਸਮਾਂ_ਸਾਰਣੀ_ਤਰਜੀਹ ਸਮਾਂ_ਸਾਰਣੀ,
				     ਪੂਰਨ minute)
{
	ਅੱਖਰ *cmd = ਨਿਰਾ;
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;
	ਅਚਲ ਅੱਖਰ *frequency = get_frequency(ਸਮਾਂ_ਸਾਰਣੀ);
	ਪੂਰਨ ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	ਜੇ (!enable) {
		ਬੱਚਾ.no_stderr = 1;
	} ਨਹੀਂਤਾਂ ਜੇ (systemd_timer_write_timer_file(ਸਮਾਂ_ਸਾਰਣੀ, minute)) {
		ret = -1;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	get_ਸਮਾਂ_ਸਾਰਣੀ_cmd("systemctl", ਨਿਰਾ, &cmd);
	strvec_ਵੰਡੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, cmd);
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--user", enable ? "enable" : "disable",
		     "--now", ਨਿਰਾ);
	strvec_pushf(&ਬੱਚਾ.ਦਲੀਲਾਂ, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	ਜੇ (ਸ਼ੁਰੂ_command(&ਬੱਚਾ)) {
		ret = error(_("failed to start systemctl"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (ਅੰਤ_command(&ਬੱਚਾ)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		ਜੇ (enable) {
			ret = error(_("failed to run systemctl"));
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	ret = 0;

ਬਾਹਰਲਾ:
	ਮੁਕਤ(cmd);
	ਵਾਪਸ ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
ਸਥਿਰ ਖਾਲੀ systemd_timer_delete_stale_timer_templates(ਖਾਲੀ)
{
	ਅੱਖਰ *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	ਅੱਖਰ *filename = xdg_config_home_systemd(timer_template_name);

	ਜੇ (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	ਮੁਕਤ(filename);
	ਮੁਕਤ(timer_template_name);
}

ਸਥਿਰ ਪੂਰਨ systemd_timer_delete_unit_files(ਖਾਲੀ)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	ਵਾਪਸ systemd_timer_delete_timer_file(ਸਮਾਂ_ਸਾਰਣੀ_HOURLY) |
	       systemd_timer_delete_timer_file(ਸਮਾਂ_ਸਾਰਣੀ_DAILY) |
	       systemd_timer_delete_timer_file(ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY) |
	       systemd_timer_delete_service_template();
}

ਸਥਿਰ ਪੂਰਨ systemd_timer_delete_units(ਖਾਲੀ)
{
	ਪੂਰਨ minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	ਵਾਪਸ systemd_timer_enable_unit(0, ਸਮਾਂ_ਸਾਰਣੀ_HOURLY, minute) |
	       systemd_timer_enable_unit(0, ਸਮਾਂ_ਸਾਰਣੀ_DAILY, minute) |
	       systemd_timer_enable_unit(0, ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

ਸਥਿਰ ਪੂਰਨ systemd_timer_ਤਿਆਰੀ_units(ਖਾਲੀ)
{
	ਪੂਰਨ minute = get_random_minute();
	ਅਚਲ ਅੱਖਰ *exec_ਰਸਤਾ = git_exec_ਰਸਤਾ();

	ਪੂਰਨ ret = systemd_timer_write_service_template(exec_ਰਸਤਾ) ||
		  systemd_timer_enable_unit(1, ਸਮਾਂ_ਸਾਰਣੀ_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, ਸਮਾਂ_ਸਾਰਣੀ_DAILY, minute) ||
		  systemd_timer_enable_unit(1, ਸਮਾਂ_ਸਾਰਣੀ_WEEKLY, minute);

	ਜੇ (ret)
		systemd_timer_delete_units();
	ਨਹੀਂਤਾਂ
		systemd_timer_delete_stale_timer_templates();

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ systemd_timer_update_ਸਮਾਂ_ਸਾਰਣੀ(ਪੂਰਨ ਲੜੀਵਾਰ_maintenance, ਪੂਰਨ fd ਅਣਵਰਤਿਆ)
{
	ਜੇ (ਲੜੀਵਾਰ_maintenance)
		ਵਾਪਸ systemd_timer_ਤਿਆਰੀ_units();
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ systemd_timer_delete_units();
}

ਗਿਣਤੀ scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ {
	ਅਚਲ ਅੱਖਰ *name;
	ਪੂਰਨ (*is_ਉਪਲਬਧ)(ਖਾਲੀ);
	ਪੂਰਨ (*update_ਸਮਾਂ_ਸਾਰਣੀ)(ਪੂਰਨ ਲੜੀਵਾਰ_maintenance, ਪੂਰਨ fd);
} scheduler_fn[] = {
	[SCHEDULER_CRON] = {
		.name = "crontab",
		.is_ਉਪਲਬਧ = is_crontab_ਉਪਲਬਧ,
		.update_ਸਮਾਂ_ਸਾਰਣੀ = crontab_update_ਸਮਾਂ_ਸਾਰਣੀ,
	},
	[SCHEDULER_SYSTEMD] = {
		.name = "systemctl",
		.is_ਉਪਲਬਧ = is_systemd_timer_ਉਪਲਬਧ,
		.update_ਸਮਾਂ_ਸਾਰਣੀ = systemd_timer_update_ਸਮਾਂ_ਸਾਰਣੀ,
	},
	[SCHEDULER_LAUNCHCTL] = {
		.name = "launchctl",
		.is_ਉਪਲਬਧ = is_launchctl_ਉਪਲਬਧ,
		.update_ਸਮਾਂ_ਸਾਰਣੀ = launchctl_update_ਸਮਾਂ_ਸਾਰਣੀ,
	},
	[SCHEDULER_SCHTASKS] = {
		.name = "schtasks",
		.is_ਉਪਲਬਧ = is_schtasks_ਉਪਲਬਧ,
		.update_ਸਮਾਂ_ਸਾਰਣੀ = schtasks_update_ਸਮਾਂ_ਸਾਰਣੀ,
	},
};

ਸਥਿਰ ਗਿਣਤੀ scheduler parse_scheduler(ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਜੇ (!ਮੁੱਲ)
		ਵਾਪਸ SCHEDULER_INVALID;
	ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(ਮੁੱਲ, "auto"))
		ਵਾਪਸ SCHEDULER_AUTO;
	ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(ਮੁੱਲ, "cron") || !strcasecmp(ਮੁੱਲ, "crontab"))
		ਵਾਪਸ SCHEDULER_CRON;
	ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(ਮੁੱਲ, "systemd") ||
		 !strcasecmp(ਮੁੱਲ, "systemd-timer"))
		ਵਾਪਸ SCHEDULER_SYSTEMD;
	ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(ਮੁੱਲ, "launchctl"))
		ਵਾਪਸ SCHEDULER_LAUNCHCTL;
	ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(ਮੁੱਲ, "schtasks"))
		ਵਾਪਸ SCHEDULER_SCHTASKS;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ SCHEDULER_INVALID;
}

ਸਥਿਰ ਪੂਰਨ maintenance_opt_scheduler(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				     ਪੂਰਨ unset)
{
	ਗਿਣਤੀ scheduler *scheduler = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	ਜੇ (*scheduler == SCHEDULER_INVALID)
		ਵਾਪਸ error(_("unrecognized --scheduler argument '%s'"), arg);
	ਵਾਪਸ 0;
}

ਬਣਤਰ maintenance_ਸ਼ੁਰੂ_opts {
	ਗਿਣਤੀ scheduler scheduler;
};

ਸਥਿਰ ਗਿਣਤੀ scheduler resolve_scheduler(ਗਿਣਤੀ scheduler scheduler)
{
	ਜੇ (scheduler != SCHEDULER_AUTO)
		ਵਾਪਸ scheduler;

#ਜੇ defined(__APPLE__)
	ਵਾਪਸ SCHEDULER_LAUNCHCTL;

#ਜਾਂਜੇ defined(GIT_WINDOWS_NATIVE)
	ਵਾਪਸ SCHEDULER_SCHTASKS;

#ਜਾਂਜੇ defined(__linux__)
	ਜੇ (is_systemd_timer_ਉਪਲਬਧ())
		ਵਾਪਸ SCHEDULER_SYSTEMD;
	ਨਹੀਂਤਾਂ ਜੇ (is_crontab_ਉਪਲਬਧ())
		ਵਾਪਸ SCHEDULER_CRON;
	ਨਹੀਂਤਾਂ
		die(_("neither systemd timers nor crontab are available"));

#ਨਹੀਂਤਾਂ
	ਵਾਪਸ SCHEDULER_CRON;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ validate_scheduler(ਗਿਣਤੀ scheduler scheduler)
{
	ਜੇ (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	ਜੇ (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	ਜੇ (!scheduler_fn[scheduler].is_ਉਪਲਬਧ())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

ਸਥਿਰ ਪੂਰਨ update_background_ਸਮਾਂ_ਸਾਰਣੀ(ਅਚਲ ਬਣਤਰ maintenance_ਸ਼ੁਰੂ_opts *opts,
				      ਪੂਰਨ enable)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਬਣਤਰ ਤਾਲਾ_file lk;
	ਅੱਖਰ *ਤਾਲਾ_ਰਸਤਾ = xstrfmt("%s/schedule", the_repository->objects->ਸਰੋਤ->ਰਸਤਾ);

	ਜੇ (hold_ਤਾਲਾ_file_for_update(&lk, ਤਾਲਾ_ਰਸਤਾ, ਤਾਲਾ_NO_DEREF) < 0) {
		ਜੇ (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_ਰਸਤਾ(ਤਾਲਾ_ਰਸਤਾ), strerror(errno));
		ਨਹੀਂਤਾਂ
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		ਮੁਕਤ(ਤਾਲਾ_ਰਸਤਾ);
		ਵਾਪਸ -1;
	}

	ਲਈ (i = 1; i < ARRAY_ਮਾਪ(scheduler_fn); i++) {
		ਜੇ (enable && opts->scheduler == i)
			ਜਾਰੀ;
		ਜੇ (!scheduler_fn[i].is_ਉਪਲਬਧ())
			ਜਾਰੀ;
		scheduler_fn[i].update_ਸਮਾਂ_ਸਾਰਣੀ(0, get_ਤਾਲਾ_file_fd(&lk));
	}

	ਜੇ (enable)
		ਨਤੀਜਾ = scheduler_fn[opts->scheduler].update_ਸਮਾਂ_ਸਾਰਣੀ(
			1, get_ਤਾਲਾ_file_fd(&lk));

	rollback_ਤਾਲਾ_file(&lk);

	ਮੁਕਤ(ਤਾਲਾ_ਰਸਤਾ);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_maintenance_ਸ਼ੁਰੂ_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ maintenance_ਸ਼ੁਰੂ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ maintenance_ਸ਼ੁਰੂ_opts opts = { 0 };
	ਬਣਤਰ option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਰਜਿਸਟਰ_ਦਲੀਲਾਂ[] = { "register", ਨਿਰਾ };

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_maintenance_ਸ਼ੁਰੂ_usage, 0);
	ਜੇ (argc)
		usage_with_options(builtin_maintenance_ਸ਼ੁਰੂ_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	ਜੇ (update_background_ਸਮਾਂ_ਸਾਰਣੀ(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	ਜੇ (maintenance_ਰਜਿਸਟਰ(ARRAY_ਮਾਪ(ਰਜਿਸਟਰ_ਦਲੀਲਾਂ)-1, ਰਜਿਸਟਰ_ਦਲੀਲਾਂ, ਨਿਰਾ, repo))
		warning(_("failed to add repo to global config"));
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ maintenance_stop(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_maintenance_stop_usage, 0);
	ਜੇ (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	ਵਾਪਸ update_background_ਸਮਾਂ_ਸਾਰਣੀ(ਨਿਰਾ, 0);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ maintenance_is_needed(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ maintenance_ਲੜੀਵਾਰ_opts opts = MAINTENANCE_ਲੜੀਵਾਰ_OPTS_INIT;
	ਬਣਤਰ string_list selected_tasks = STRING_LIST_INIT_DUP;
	ਬਣਤਰ gc_config cfg = GC_CONFIG_INIT;
	ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			       N_("check a specific task"),
			       PARSE_OPT_NONEG, task_option_parse),
		OPT_ਸਿਰਾ()
	};
	bool is_needed = false;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_maintenance_is_needed_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ਜੇ (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	ਜੇ (opts.auto_flag) {
		ਲਈ (ਮਾਪ_t i = 0; i < opts.tasks_nr; i++) {
			ਜੇ (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				ਰੋਕੋ;
			}
		}
	} ਨਹੀਂਤਾਂ {
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
	maintenance_ਲੜੀਵਾਰ_opts_release(&opts);
	gc_config_release(&cfg);

	ਜੇ (is_needed)
		ਵਾਪਸ 0;
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	ਨਿਰਾ,
};

ਪੂਰਨ cmd_maintenance(ਪੂਰਨ argc,
		    ਅਚਲ ਅੱਖਰ **argv,
		    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		    ਬਣਤਰ repository *repo)
{
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option builtin_maintenance_options[] = {
		OPT_SUBCOMMAND("run", &fn, maintenance_ਲੜੀਵਾਰ),
		OPT_SUBCOMMAND("start", &fn, maintenance_ਸ਼ੁਰੂ),
		OPT_SUBCOMMAND("stop", &fn, maintenance_stop),
		OPT_SUBCOMMAND("register", &fn, maintenance_ਰਜਿਸਟਰ),
		OPT_SUBCOMMAND("unregister", &fn, maintenance_unregister),
		OPT_SUBCOMMAND("is-needed", &fn, maintenance_is_needed),
		OPT_ਸਿਰਾ(),
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_maintenance_options,
			     builtin_maintenance_usage, 0);
	ਵਾਪਸ fn(argc, argv, ਅਗੇਤਰ, repo);
}
