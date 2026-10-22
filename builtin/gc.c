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

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "date.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "tempfile.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "sigchain.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "commit-graph.h"
#અન્તર્ભાવ "packfile.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "reflog.h"
#અન્તર્ભાવ "rerere.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "exec-cmd.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hook.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "trace2.h"
#અન્તર્ભાવ "worktree.h"

#define FAILED_RUN "failed to run %s"

સ્થિર અચલ અક્ષર * અચલ builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	ખાલી
};

સ્થિર timestamp_t gc_log_expire_time;
સ્થિર સંરચના tempfile *pidfile;
સ્થિર સંરચના lock_file log_lock;
સ્થિર સંરચના string_list pack_garbage = STRING_LIST_INIT_DUP;

સ્થિર શૂન્ય clean_pack_garbage(શૂન્ય)
{
	પૂર્ણાંક i;
	માટે (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.વસ્તુઓ[i].string);
	string_list_clear(&pack_garbage, 0);
}

સ્થિર શૂન્ય report_pack_garbage(unsigned seen_bits, અચલ અક્ષર *path)
{
	જો (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

સ્થિર શૂન્ય process_log_file(શૂન્ય)
{
	સંરચના stat st;
	જો (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		પૂર્ણાંક saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} નહીંતર જો (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} નહીંતર {
		અક્ષર *path = repo_git_path(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(path);
		rollback_lock_file(&log_lock);
		મુક્ત(path);
	}
}

સ્થિર શૂન્ય process_log_file_at_exit(શૂન્ય)
{
	fflush(stderr);
	process_log_file();
}

સ્થિર પૂર્ણાંક gc_config_is_timestamp_never(અચલ અક્ષર *var)
{
	અચલ અક્ષર *કિંમત;
	timestamp_t expire;

	જો (!repo_config_get_કિંમત(the_repository, var, &કિંમત) && કિંમત) {
		જો (parse_expiry_date(કિંમત, &expire))
			die(_("failed to parse '%s' value '%s'"), var, કિંમત);
		પાછા expire == 0;
	}
	પાછા 0;
}

સંરચના gc_config {
	પૂર્ણાંક pack_refs;
	પૂર્ણાંક prune_reflogs;
	પૂર્ણાંક cruft_packs;
	unsigned long max_cruft_size;
	પૂર્ણાંક aggressive_depth;
	પૂર્ણાંક aggressive_window;
	પૂર્ણાંક detach_auto;
	અક્ષર *gc_log_expire;
	અક્ષર *prune_expire;
	અક્ષર *prune_worktrees_expire;
	અક્ષર *repack_expire_to;
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

સ્થિર શૂન્ય gc_config_release(સંરચના gc_config *cfg)
{
	મુક્ત(cfg->gc_log_expire);
	મુક્ત(cfg->prune_expire);
	મુક્ત(cfg->prune_worktrees_expire);
}

સ્થિર શૂન્ય gc_config(સંરચના gc_config *cfg)
{
	અચલ અક્ષર *કિંમત;
	અક્ષર *owned = ખાલી;

	જો (!repo_config_get_કિંમત(the_repository, "gc.packrefs", &કિંમત)) {
		જો (કિંમત && !strcmp(કિંમત, "notbare"))
			cfg->pack_refs = -1;
		નહીંતર
			cfg->pack_refs = git_config_bool("gc.packrefs", કિંમત);
	}

	જો (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	જો (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		મુક્ત(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	જો (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		મુક્ત(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	જો (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		મુક્ત(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, ખાલી);
}

ગણના schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

સ્થિર ગણના schedule_priority parse_schedule(અચલ અક્ષર *કિંમત)
{
	જો (!કિંમત)
		પાછા SCHEDULE_NONE;
	જો (!strcasecmp(કિંમત, "hourly"))
		પાછા SCHEDULE_HOURLY;
	જો (!strcasecmp(કિંમત, "daily"))
		પાછા SCHEDULE_DAILY;
	જો (!strcasecmp(કિંમત, "weekly"))
		પાછા SCHEDULE_WEEKLY;
	પાછા SCHEDULE_NONE;
}

ગણના maintenance_task_label {
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
	TASK_ગણતરી
};

સંરચના maintenance_run_opts {
	ગણના maintenance_task_label *tasks;
	size_t tasks_nr, tasks_alloc;
	પૂર્ણાંક auto_flag;
	પૂર્ણાંક detach;
	પૂર્ણાંક quiet;
	ગણના schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

સ્થિર શૂન્ય maintenance_run_opts_release(સંરચના maintenance_run_opts *opts)
{
	મુક્ત(opts->tasks);
}

સ્થિર પૂર્ણાંક pack_refs_condition(UNUSED સંરચના gc_config *cfg)
{
	સંરચના string_list included_refs = STRING_LIST_INIT_NODUP;
	સંરચના ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	સંરચના refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	જો (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		પાછા 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	પાછા required;
}

સ્થિર પૂર્ણાંક maintenance_task_pack_refs(સંરચના maintenance_run_opts *opts,
				      UNUSED સંરચના gc_config *cfg)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", ખાલી);
	જો (opts->auto_flag)
		strvec_ધકેલો(&cmd.args, "--auto");

	પાછા run_command(&cmd);
}

સંરચના ગણતરી_reflog_entries_data {
	સંરચના expire_reflog_policy_cb policy;
	size_t ગણતરી;
	size_t limit;
};

સ્થિર પૂર્ણાંક ગણતરી_reflog_entries(અચલ અક્ષર *refname UNUSED,
				સંરચના object_id *old_oid, સંરચના object_id *new_oid,
				અચલ અક્ષર *committer, timestamp_t timestamp,
				પૂર્ણાંક tz, અચલ અક્ષર *સંદેશ, શૂન્ય *cb_data)
{
	સંરચના ગણતરી_reflog_entries_data *data = cb_data;
	જો (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, સંદેશ, &data->policy))
		data->ગણતરી++;
	પાછા data->ગણતરી >= data->limit;
}

સ્થિર પૂર્ણાંક reflog_expire_condition(સંરચના gc_config *cfg UNUSED)
{
	timestamp_t now = time(ખાલી);
	સંરચના ગણતરી_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	પૂર્ણાંક limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	જો (!limit)
		પાછા 0;
	જો (limit < 0)
		પાછા 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 ગણતરી_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	પાછા data.ગણતરી >= data.limit;
}

સ્થિર પૂર્ણાંક maintenance_task_reflog_expire(સંરચના maintenance_run_opts *opts UNUSED,
					  સંરચના gc_config *cfg UNUSED)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", ખાલી);
	પાછા run_command(&cmd);
}

સ્થિર પૂર્ણાંક maintenance_task_worktree_prune(સંરચના maintenance_run_opts *opts UNUSED,
					   સંરચના gc_config *cfg)
{
	સંરચના child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", ખાલી);
	strvec_ધકેલો(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	પાછા run_command(&prune_worktrees_cmd);
}

સ્થિર પૂર્ણાંક worktree_prune_condition(સંરચના gc_config *cfg)
{
	સંરચના strbuf buf = STRBUF_INIT;
	પૂર્ણાંક should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	સંરચના dirent *d;
	DIR *dir = ખાલી;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	જો (limit <= 0) {
		should_prune = limit < 0;
		જાઓ out;
	}

	જો (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		જાઓ out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	જો (!dir)
		જાઓ out;

	જ્યારે (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		અક્ષર *wtpath;
		strbuf_reset(&buf);
		જો (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		મુક્ત(wtpath);
	}

	should_prune = !limit;

out:
	જો (dir)
		closedir(dir);
	strbuf_release(&buf);
	પાછા should_prune;
}

સ્થિર પૂર્ણાંક maintenance_task_rerere_gc(સંરચના maintenance_run_opts *opts UNUSED,
				      સંરચના gc_config *cfg UNUSED)
{
	સંરચના child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", ખાલી);
	પાછા run_command(&rerere_cmd);
}

સ્થિર પૂર્ણાંક rerere_gc_condition(સંરચના gc_config *cfg UNUSED)
{
	પૂર્ણાંક limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	જો (!limit)
		પાછા 0; /* never prune */
	જો (limit < 0)
		પાછા 1; /* always prune */

	પાછા rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
સ્થિર અચલ અક્ષર *lock_repo_for_gc(પૂર્ણાંક force, pid_t* ret_pid)
{
	સંરચના lock_file lock = LOCK_INIT;
	અક્ષર my_host[HOST_NAME_MAX + 1];
	સંરચના strbuf sb = STRBUF_INIT;
	સંરચના stat st;
	uintmax_t pid;
	FILE *fp;
	પૂર્ણાંક fd;
	અક્ષર *pidfile_path;

	જો (is_tempfile_active(pidfile))
		/* already locked */
		પાછા ખાલી;

	જો (xgethostname(my_host, કદ(my_host)))
		xsnprintf(my_host, કદ(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	જો (!force) {
		સ્થિર અક્ષર locking_host[HOST_NAME_MAX + 1];
		સ્થિર અક્ષર *scan_fmt;
		પૂર્ણાંક should_exit;

		જો (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_path, "r");
		memset(locking_host, 0, કદ(locking_host));
		should_exit =
			fp != ખાલી &&
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
			time(ખાલી) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, scan_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		જો (fp)
			fclose(fp);
		જો (should_exit) {
			જો (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			મુક્ત(pidfile_path);
			પાછા locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	મુક્ત(pidfile_path);
	પાછા ખાલી;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
સ્થિર પૂર્ણાંક report_last_gc_error(શૂન્ય)
{
	સંરચના strbuf sb = STRBUF_INIT;
	પૂર્ણાંક ret = 0;
	ssize_t len;
	સંરચના stat st;
	અક્ષર *gc_log_path = repo_git_path(the_repository, "gc.log");

	જો (stat(gc_log_path, &st)) {
		જો (errno == ENOENT)
			જાઓ done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		જાઓ done;
	}

	જો (st.st_mtime < gc_log_expire_time)
		જાઓ done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	જો (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	નહીંતર જો (len > 0) {
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
	મુક્ત(gc_log_path);
	પાછા ret;
}

સ્થિર પૂર્ણાંક gc_foreground_tasks(સંરચના maintenance_run_opts *opts,
			       સંરચના gc_config *cfg)
{
	જો (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		પાછા error(FAILED_RUN, "pack-refs");
	જો (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		પાછા error(FAILED_RUN, "reflog");
	પાછા 0;
}

સ્થિર પૂર્ણાંક maintenance_task_odb(સંરચના maintenance_run_opts *opts,
				સંરચના gc_config *cfg,
				પૂર્ણાંક keep_largest_pack,
				પૂર્ણાંક aggressive)
{
	સંરચના odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	જો (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	જો (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	જો (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	પાછા odb_optimize(the_repository->objects, &odb_opts);
}

પૂર્ણાંક cmd_gc(પૂર્ણાંક argc,
	   અચલ અક્ષર **argv,
	   અચલ અક્ષર *prefix,
	   સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક aggressive = 0;
	પૂર્ણાંક force = 0;
	અચલ અક્ષર *name;
	pid_t pid;
	પૂર્ણાંક daemonized = 0;
	પૂર્ણાંક keep_largest_pack = -1;
	પૂર્ણાંક skip_foreground_tasks = 0;
	timestamp_t dummy;
	સંરચના maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	સંરચના gc_config cfg = GC_CONFIG_INIT;
	અચલ અક્ષર *prune_expire_sentinel = "sentinel";
	અચલ અક્ષર *prune_expire_arg = prune_expire_sentinel;
	પૂર્ણાંક ret;
	સંરચના option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.કિંમત = &prune_expire_arg,
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

	જો (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	જો (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	જો (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	જો (prune_expire_arg != prune_expire_sentinel) {
		મુક્ત(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	જો (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	જો (opts.auto_flag) {
		સંરચના odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		જો (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		જો (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			જાઓ out;
		}

		જો (!opts.quiet) {
			જો (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			નહીંતર
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	જો (opts.detach > 0) {
		ret = report_last_gc_error();
		જો (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			જાઓ out;

		} નહીંતર જો (ret) {
			/* an I/O error occurred, already reported */
			જાઓ out;
		}

		જો (!skip_foreground_tasks) {
			જો (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				જાઓ out;
			}

			જો (gc_foreground_tasks(&opts, &cfg) < 0)
				die(ખાલી);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = lock_repo_for_gc(force, &pid);
	જો (name) {
		જો (opts.auto_flag) {
			ret = 0;
			જાઓ out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	જો (daemonized) {
		અક્ષર *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		મુક્ત(path);
	}

	જો (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	જો (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	જો (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	જો (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(ખાલી);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	જો (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	જો (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     ખાલી);

	જો (!daemonized) {
		અક્ષર *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		મુક્ત(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	પાછા 0;
}

સ્થિર અચલ અક્ષર *અચલ builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	ખાલી
};

સ્થિર પૂર્ણાંક maintenance_opt_schedule(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
				    પૂર્ણાંક unset)
{
	ગણના schedule_priority *priority = opt->કિંમત;

	જો (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	જો (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	પાછા 0;
}

સંરચના cg_auto_data {
	પૂર્ણાંક અંક_not_in_graph;
	પૂર્ણાંક limit;
};

સ્થિર પૂર્ણાંક dfs_on_ref(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	સંરચના cg_auto_data *data = (સંરચના cg_auto_data *)cb_data;
	પૂર્ણાંક result = 0;
	અચલ સંરચના object_id *maybe_peeled = ref->oid;
	સંરચના object_id peeled;
	સંરચના commit_list *ઢગલો = ખાલી;
	સંરચના commit *commit;

	જો (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	જો (odb_read_object_info(the_repository->objects, maybe_peeled, ખાલી) != OBJ_COMMIT)
		પાછા 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	જો (!commit || commit->object.flags & SEEN)
		પાછા 0;
	commit->object.flags |= SEEN;

	જો (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		પાછા 0;

	data->અંક_not_in_graph++;

	જો (data->અંક_not_in_graph >= data->limit)
		પાછા 1;

	commit_list_ઉમેરો(commit, &ઢગલો);

	જ્યારે (!result && ઢગલો) {
		સંરચના commit_list *parent;

		commit = કાઢો_commit(&ઢગલો);

		માટે (parent = commit->parents; parent; parent = parent->next) {
			જો (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				ચાલુ;

			parent->item->object.flags |= SEEN;
			data->અંક_not_in_graph++;

			જો (data->અંક_not_in_graph >= data->limit) {
				result = 1;
				તોડો;
			}

			commit_list_ઉમેરો(parent->item, &ઢગલો);
		}
	}

	commit_list_free(ઢગલો);
	પાછા result;
}

સ્થિર પૂર્ણાંક should_write_commit_graph(સંરચના gc_config *cfg UNUSED)
{
	પૂર્ણાંક result;
	સંરચના cg_auto_data data;

	data.અંક_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	જો (!data.limit)
		પાછા 0;
	જો (data.limit < 0)
		પાછા 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	પાછા result;
}

સ્થિર પૂર્ણાંક run_write_commit_graph(સંરચના maintenance_run_opts *opts)
{
	સંરચના child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", ખાલી);

	જો (opts->quiet)
		strvec_ધકેલો(&child.args, "--no-progress");
	નહીંતર
		strvec_ધકેલો(&child.args, "--progress");

	પાછા !!run_command(&child);
}

સ્થિર પૂર્ણાંક maintenance_task_commit_graph(સંરચના maintenance_run_opts *opts,
					 સંરચના gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	જો (!the_repository->settings.core_commit_graph)
		પાછા 0;

	જો (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		પાછા 1;
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક fetch_remote(સંરચના remote *remote, શૂન્ય *cbdata)
{
	સંરચના maintenance_run_opts *opts = cbdata;
	સંરચના child_process child = CHILD_PROCESS_INIT;

	જો (remote->skip_default_update)
		પાછા 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     ખાલી);

	જો (opts->quiet)
		strvec_ધકેલો(&child.args, "--quiet");

	પાછા !!run_command(&child);
}

સ્થિર પૂર્ણાંક maintenance_task_prefetch(સંરચના maintenance_run_opts *opts,
				     સંરચના gc_config *cfg UNUSED)
{
	જો (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		પાછા 1;
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક maintenance_task_gc_foreground(સંરચના maintenance_run_opts *opts,
					  સંરચના gc_config *cfg)
{
	પાછા gc_foreground_tasks(opts, cfg);
}

સ્થિર પૂર્ણાંક maintenance_task_gc_background(સંરચના maintenance_run_opts *opts,
					  સંરચના gc_config *cfg UNUSED)
{
	સંરચના child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_ધકેલો(&child.args, "gc");

	જો (opts->auto_flag)
		strvec_ધકેલો(&child.args, "--auto");
	જો (opts->quiet)
		strvec_ધકેલો(&child.args, "--quiet");
	નહીંતર
		strvec_ધકેલો(&child.args, "--no-quiet");
	strvec_ધકેલો(&child.args, "--no-detach");
	strvec_ધકેલો(&child.args, "--skip-foreground-tasks");

	પાછા run_command(&child);
}

સ્થિર પૂર્ણાંક gc_condition(સંરચના gc_config *cfg)
{
	સંરચના odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	પાછા odb_optimize_required(the_repository->objects, &opts);
}

સ્થિર પૂર્ણાંક prune_packed(સંરચના maintenance_run_opts *opts)
{
	સંરચના child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_ધકેલો(&child.args, "prune-packed");

	જો (opts->quiet)
		strvec_ધકેલો(&child.args, "--quiet");

	પાછા !!run_command(&child);
}

સંરચના write_loose_object_data {
	FILE *in;
	પૂર્ણાંક ગણતરી;
	પૂર્ણાંક batch_size;
};

સ્થિર પૂર્ણાંક loose_object_auto_limit = 100;

સ્થિર પૂર્ણાંક loose_object_ગણતરી(અચલ સંરચના object_id *oid UNUSED,
			      અચલ અક્ષર *path UNUSED,
			      શૂન્ય *data)
{
	પૂર્ણાંક *ગણતરી = (પૂર્ણાંક*)data;
	જો (++(*ગણતરી) >= loose_object_auto_limit)
		પાછા 1;
	પાછા 0;
}

સ્થિર પૂર્ણાંક loose_object_auto_condition(સંરચના gc_config *cfg UNUSED)
{
	પૂર્ણાંક ગણતરી = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	જો (!loose_object_auto_limit)
		પાછા 0;
	જો (loose_object_auto_limit < 0)
		પાછા 1;

	પાછા for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_ગણતરી,
					     ખાલી, ખાલી, &ગણતરી);
}

સ્થિર પૂર્ણાંક bail_on_loose(અચલ સંરચના object_id *oid UNUSED,
			 અચલ અક્ષર *path UNUSED,
			 શૂન્ય *data UNUSED)
{
	પાછા 1;
}

સ્થિર પૂર્ણાંક write_loose_object_to_stdin(અચલ સંરચના object_id *oid,
				       અચલ અક્ષર *path UNUSED,
				       શૂન્ય *data)
{
	સંરચના write_loose_object_data *d = (સંરચના write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	પાછા ++(d->ગણતરી) > d->batch_size;
}

સ્થિર પૂર્ણાંક pack_loose(સંરચના maintenance_run_opts *opts)
{
	સંરચના repository *r = the_repository;
	પૂર્ણાંક result = 0;
	સંરચના write_loose_object_data data;
	સંરચના child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	જો (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   ખાલી, ખાલી, ખાલી))
		પાછા 0;

	pack_proc.git_cmd = 1;

	strvec_ધકેલો(&pack_proc.args, "pack-objects");
	જો (opts->quiet)
		strvec_ધકેલો(&pack_proc.args, "--quiet");
	નહીંતર
		strvec_ધકેલો(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	જો (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		પાછા 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.ગણતરી = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	જો (!data.batch_size)
		data.batch_size = INT_MAX;
	નહીંતર જો (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      ખાલી, ખાલી, &data);

	fclose(data.in);

	જો (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	પાછા result;
}

સ્થિર પૂર્ણાંક maintenance_task_loose_objects(સંરચના maintenance_run_opts *opts,
					  સંરચના gc_config *cfg UNUSED)
{
	પાછા prune_packed(opts) || pack_loose(opts);
}

સ્થિર પૂર્ણાંક incremental_repack_auto_condition(સંરચના gc_config *cfg UNUSED)
{
	સંરચના packed_git *p;
	પૂર્ણાંક incremental_repack_auto_limit = 10;
	પૂર્ણાંક ગણતરી = 0;

	prepare_repo_settings(the_repository);
	જો (!the_repository->settings.core_multi_pack_index)
		પાછા 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	જો (!incremental_repack_auto_limit)
		પાછા 0;
	જો (incremental_repack_auto_limit < 0)
		પાછા 1;

	repo_for_each_pack(the_repository, p) {
		જો (ગણતરી >= incremental_repack_auto_limit)
			તોડો;
		જો (!p->multi_pack_index)
			ગણતરી++;
	}

	પાછા ગણતરી >= incremental_repack_auto_limit;
}

સ્થિર પૂર્ણાંક multi_pack_index_write(સંરચના maintenance_run_opts *opts)
{
	સંરચના child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", ખાલી);

	જો (opts->quiet)
		strvec_ધકેલો(&child.args, "--no-progress");
	નહીંતર
		strvec_ધકેલો(&child.args, "--progress");

	જો (run_command(&child))
		પાછા error(_("failed to write multi-pack-index"));

	પાછા 0;
}

સ્થિર પૂર્ણાંક multi_pack_index_expire(સંરચના maintenance_run_opts *opts)
{
	સંરચના child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", ખાલી);

	જો (opts->quiet)
		strvec_ધકેલો(&child.args, "--no-progress");
	નહીંતર
		strvec_ધકેલો(&child.args, "--progress");

	જો (run_command(&child))
		પાછા error(_("'git multi-pack-index expire' failed"));

	પાછા 0;
}

#define TWO_GIGABYTES (INT32_MAX)

સ્થિર off_t get_auto_pack_size(શૂન્ય)
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
	સંરચના packed_git *p;
	સંરચના repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		જો (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} નહીંતર જો (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	જો (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	પાછા result_size;
}

સ્થિર પૂર્ણાંક multi_pack_index_repack(સંરચના maintenance_run_opts *opts)
{
	સંરચના child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", ખાલી);

	જો (opts->quiet)
		strvec_ધકેલો(&child.args, "--no-progress");
	નહીંતર
		strvec_ધકેલો(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	જો (run_command(&child))
		પાછા error(_("'git multi-pack-index repack' failed"));

	પાછા 0;
}

સ્થિર પૂર્ણાંક maintenance_task_incremental_repack(સંરચના maintenance_run_opts *opts,
					       સંરચના gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	જો (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		પાછા 0;
	}

	જો (multi_pack_index_write(opts))
		પાછા 1;
	જો (multi_pack_index_expire(opts))
		પાછા 1;
	જો (multi_pack_index_repack(opts))
		પાછા 1;
	પાછા 0;
}

સ્થિર પૂર્ણાંક maintenance_task_geometric_repack(સંરચના maintenance_run_opts *opts,
					     સંરચના gc_config *cfg)
{
	સંરચના odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	જો (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	પાછા odb_optimize(the_repository->objects, &odb_opts);
}

સ્થિર પૂર્ણાંક geometric_repack_auto_condition(સંરચના gc_config *cfg)
{
	સંરચના odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	પાછા odb_optimize_required(the_repository->objects, &opts);
}

પ્રકારવ્યાખ્યા પૂર્ણાંક (*maintenance_task_fn)(સંરચના maintenance_run_opts *opts,
				   સંરચના gc_config *cfg);
પ્રકારવ્યાખ્યા પૂર્ણાંક (*maintenance_auto_fn)(સંરચના gc_config *cfg);

સંરચના maintenance_task {
	અચલ અક્ષર *name;

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

સ્થિર અચલ સંરચના maintenance_task tasks[] = {
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

ગણના task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

ગણના auto_gc_hook_result {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

સ્થિર પૂર્ણાંક maybe_run_task(અચલ સંરચના maintenance_task *task,
			  સંરચના repository *repo,
			  સંરચના maintenance_run_opts *opts,
			  સંરચના gc_config *cfg,
			  ગણના task_phase phase,
			  ગણના auto_gc_hook_result *auto_gc_hook_result)
{
	પૂર્ણાંક foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	અચલ અક્ષર *region = foreground ? "maintenance foreground" : "maintenance";
	પૂર્ણાંક ret = 0;

	જો (!fn)
		પાછા 0;
	જો (opts->auto_flag) {
		જો (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			પાછા 0;

		જો (!task->auto_condition || !task->auto_condition(cfg))
			પાછા 0;

		જો (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		જો (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			પાછા 0;
	}

	trace2_region_enter(region, task->name, repo);
	જો (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	પાછા ret;
}

સ્થિર પૂર્ણાંક maintenance_run_tasks(સંરચના maintenance_run_opts *opts,
				 સંરચના gc_config *cfg)
{
	પૂર્ણાંક result = 0;
	સંરચના lock_file lk;
	સંરચના repository *r = the_repository;
	અક્ષર *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	ગણના auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	જો (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		જો (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		મુક્ત(lock_path);
		પાછા 0;
	}
	મુક્ત(lock_path);

	માટે (size_t i = 0; i < opts->tasks_nr; i++)
		જો (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	જો (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	માટે (size_t i = 0; i < opts->tasks_nr; i++)
		જો (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	પાછા result;
}

ગણના maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

સંરચના maintenance_strategy {
	સંરચના {
		unsigned type;
		ગણના schedule_priority schedule;
	} tasks[TASK_ગણતરી];
};

સ્થિર અચલ સંરચના maintenance_strategy none_strategy = { 0 };

સ્થિર અચલ સંરચના maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

સ્થિર અચલ સંરચના maintenance_strategy incremental_strategy = {
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

સ્થિર અચલ સંરચના maintenance_strategy geometric_strategy = {
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

સ્થિર સંરચના maintenance_strategy parse_maintenance_strategy(અચલ અક્ષર *name)
{
	જો (!strcasecmp(name, "incremental"))
		પાછા incremental_strategy;
	જો (!strcasecmp(name, "gc"))
		પાછા gc_strategy;
	જો (!strcasecmp(name, "geometric"))
		પાછા geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

સ્થિર શૂન્ય initialize_task_config(સંરચના maintenance_run_opts *opts,
				   અચલ સંરચના string_list *selected_tasks)
{
	સંરચના strbuf config_name = STRBUF_INIT;
	સંરચના maintenance_strategy strategy;
	ગણના maintenance_type type;
	અચલ અક્ષર *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	જો (selected_tasks->nr) {
		માટે (size_t i = 0; i < selected_tasks->nr; i++) {
			ગણના maintenance_task_label label = (intptr_t)selected_tasks->વસ્તુઓ[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		પાછા;
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
	જો (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} નહીંતર {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	જો (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	માટે (size_t i = 0; i < TASK_ગણતરી; i++) {
		પૂર્ણાંક config_કિંમત;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		જો (!repo_config_get_bool(the_repository, config_name.buf, &config_કિંમત))
			strategy.tasks[i].type = config_કિંમત ? type : 0;
		જો (!(strategy.tasks[i].type & type))
			ચાલુ;

		જો (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			જો (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			જો (strategy.tasks[i].schedule < opts->schedule)
				ચાલુ;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

સ્થિર પૂર્ણાંક task_option_parse(અચલ સંરચના option *opt,
			     અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના string_list *selected_tasks = opt->કિંમત;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	માટે (i = 0; i < TASK_ગણતરી; i++)
		જો (!strcasecmp(tasks[i].name, arg))
			તોડો;
	જો (i >= TASK_ગણતરી) {
		error(_("'%s' is not a valid task"), arg);
		પાછા 1;
	}

	જો (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		પાછા 1;
	}

	string_list_append(selected_tasks, arg)->util = (શૂન્ય *)(intptr_t)i;

	પાછા 0;
}

સ્થિર પૂર્ણાંક maintenance_run(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			   સંરચના repository *repo UNUSED)
{
	સંરચના maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	સંરચના string_list selected_tasks = STRING_LIST_INIT_DUP;
	સંરચના gc_config cfg = GC_CONFIG_INIT;
	સંરચના option builtin_maintenance_run_options[] = {
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
	પૂર્ણાંક ret;

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

	જો (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	પાછા ret;
}

સ્થિર અક્ષર *get_maintpath(શૂન્ય)
{
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	પાછા strbuf_detach(&sb, ખાલી);
}

સ્થિર અક્ષર અચલ * અચલ builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	ખાલી
};

સ્થિર પૂર્ણાંક maintenance_register(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				સંરચના repository *repo UNUSED)
{
	અક્ષર *config_file = ખાલી;
	સંરચના option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	પૂર્ણાંક found = 0;
	અચલ અક્ષર *ચાવી = "maintenance.repo";
	અક્ષર *maintpath = get_maintpath();
	સંરચના string_list_item *item;
	અચલ સંરચના string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	જો (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	જો (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	જો (!repo_config_get_string_multi(the_repository, ચાવી, &list)) {
		for_each_string_list_item(item, list) {
			જો (!strcmp(maintpath, item->string)) {
				found = 1;
				તોડો;
			}
		}
	}

	જો (!found) {
		પૂર્ણાંક rc;
		અક્ષર *global_config_file = ખાલી;

		જો (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		જો (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, ખાલી, 0);
		મુક્ત(global_config_file);

		જો (rc)
			die(_("unable to add '%s' value of '%s'"),
			    ચાવી, maintpath);
	}

	મુક્ત(maintpath);
	પાછા 0;
}

સ્થિર અક્ષર અચલ * અચલ builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	ખાલી
};

સ્થિર પૂર્ણાંક maintenance_unregister(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				  સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક force = 0;
	અક્ષર *config_file = ખાલી;
	સંરચના option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	અચલ અક્ષર *ચાવી = "maintenance.repo";
	અક્ષર *maintpath = get_maintpath();
	પૂર્ણાંક found = 0;
	સંરચના string_list_item *item;
	અચલ સંરચના string_list *list;
	સંરચના config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	જો (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	જો (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	જો (!(config_file
	      ? git_configset_get_string_multi(&cs, ચાવી, &list)
	      : repo_config_get_string_multi(the_repository, ચાવી, &list))) {
		for_each_string_list_item(item, list) {
			જો (!strcmp(maintpath, item->string)) {
				found = 1;
				તોડો;
			}
		}
	}

	જો (found) {
		પૂર્ણાંક rc;
		અક્ષર *global_config_file = ખાલી;

		જો (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		જો (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, ચાવી, ખાલી, maintpath, ખાલી,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_કિંમત);
		મુક્ત(global_config_file);

		જો (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    ચાવી, maintpath);
	} નહીંતર જો (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	મુક્ત(maintpath);
	પાછા 0;
}

સ્થિર અચલ અક્ષર *get_frequency(ગણના schedule_priority schedule)
{
	પસંદગી (schedule) {
	સ્થિતિ SCHEDULE_HOURLY:
		પાછા "hourly";
	સ્થિતિ SCHEDULE_DAILY:
		પાછા "daily";
	સ્થિતિ SCHEDULE_WEEKLY:
		પાછા "weekly";
	મૂળભૂત:
		BUG("invalid schedule %d", schedule);
	}
}

સ્થિર અચલ અક્ષર *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	ખાલી
};

સ્થિર અચલ અક્ષર *get_extra_config_parameters(શૂન્ય) {
	સ્થિર અચલ અક્ષર *result = ખાલી;
	સંરચના strbuf builder = STRBUF_INIT;

	જો (result)
		પાછા result;

	માટે (અચલ અક્ષર **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, ખાલી);
	પાછા result;
}

સ્થિર અચલ અક્ષર *get_extra_launchctl_strings(શૂન્ય) {
	સ્થિર અચલ અક્ષર *result = ખાલી;
	સંરચના strbuf builder = STRBUF_INIT;

	જો (result)
		પાછા result;

	માટે (અચલ અક્ષર **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, ખાલી);
	પાછા result;
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
સ્થિર પૂર્ણાંક get_schedule_cmd(અચલ અક્ષર *cmd, પૂર્ણાંક *is_available, અક્ષર **out)
{
	અક્ષર *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	સંરચના string_list_item *item;
	સંરચના string_list list = STRING_LIST_INIT_NODUP;

	જો (!testing) {
		જો (out)
			*out = xstrdup(cmd);
		પાછા 0;
	}

	જો (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		સંરચના string_list pair = STRING_LIST_INIT_NODUP;

		જો (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			ચાલુ;

		જો (!strcmp(cmd, pair.વસ્તુઓ[0].string)) {
			જો (out)
				*out = xstrdup(pair.વસ્તુઓ[1].string);
			જો (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			જાઓ out;
		}

		string_list_clear(&pair, 0);
	}

	જો (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	મુક્ત(testing);
	પાછા 1;
}

સ્થિર પૂર્ણાંક get_random_minute(શૂન્ય)
{
	/* Use a static value when under tests. */
	જો (getenv("GIT_TEST_MAINT_SCHEDULER"))
		પાછા 13;

	પાછા git_rand(0) % 60;
}

સ્થિર પૂર્ણાંક is_launchctl_available(શૂન્ય)
{
	પૂર્ણાંક is_available;
	જો (get_schedule_cmd("launchctl", &is_available, ખાલી))
		પાછા is_available;

#ifdef __APPLE__
	પાછા 1;
#નહીંતર
	પાછા 0;
#endif
}

સ્થિર અક્ષર *launchctl_service_name(અચલ અક્ષર *frequency)
{
	સંરચના strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	પાછા strbuf_detach(&label, ખાલી);
}

સ્થિર અક્ષર *launchctl_service_filename(અચલ અક્ષર *name)
{
	અક્ષર *expanded;
	સંરચના strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	જો (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	પાછા expanded;
}

સ્થિર અક્ષર *launchctl_get_uid(શૂન્ય)
{
	પાછા xstrfmt("gui/%d", getuid());
}

સ્થિર પૂર્ણાંક launchctl_boot_plist(પૂર્ણાંક enable, અચલ અક્ષર *filename)
{
	અક્ષર *cmd;
	પૂર્ણાંક result;
	સંરચના child_process child = CHILD_PROCESS_INIT;
	અક્ષર *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", ખાલી, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, ખાલી);

	child.no_stderr = 1;
	child.no_stdout = 1;

	જો (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	મુક્ત(cmd);
	મુક્ત(uid);
	પાછા result;
}

સ્થિર પૂર્ણાંક launchctl_remove_plist(ગણના schedule_priority schedule)
{
	અચલ અક્ષર *frequency = get_frequency(schedule);
	અક્ષર *name = launchctl_service_name(frequency);
	અક્ષર *filename = launchctl_service_filename(name);
	પૂર્ણાંક result = launchctl_boot_plist(0, filename);
	unlink(filename);
	મુક્ત(filename);
	મુક્ત(name);
	પાછા result;
}

સ્થિર પૂર્ણાંક launchctl_remove_plists(શૂન્ય)
{
	પાછા launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

સ્થિર પૂર્ણાંક launchctl_list_contains_plist(અચલ અક્ષર *name, અચલ અક્ષર *cmd)
{
	સંરચના child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, ખાલી);

	child.no_stderr = 1;
	child.no_stdout = 1;

	જો (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	પાછા !finish_command(&child);
}

સ્થિર પૂર્ણાંક launchctl_schedule_plist(અચલ અક્ષર *exec_path, ગણના schedule_priority schedule)
{
	પૂર્ણાંક i, fd;
	અચલ અક્ષર *preamble, *repeat;
	અચલ અક્ષર *frequency = get_frequency(schedule);
	અક્ષર *name = launchctl_service_name(frequency);
	અક્ષર *filename = launchctl_service_filename(name);
	સંરચના lock_file lk = LOCK_INIT;
	સ્થિર unsigned long lock_file_timeout_ms = ULONG_MAX;
	સંરચના strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	સંરચના stat st;
	અક્ષર *cmd;
	પૂર્ણાંક minute = get_random_minute();

	get_schedule_cmd("launchctl", ખાલી, &cmd);
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

	પસંદગી (schedule) {
	સ્થિતિ SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		માટે (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		તોડો;

	સ્થિતિ SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		માટે (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		તોડો;

	સ્થિતિ SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		તોડો;

	મૂળભૂત:
		/* unreachable */
		તોડો;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	જો (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	જો ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	જો (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	નહીંતર {
		જો (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		જો (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	મુક્ત(filename);
	મુક્ત(name);
	મુક્ત(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	પાછા 0;
}

સ્થિર પૂર્ણાંક launchctl_add_plists(શૂન્ય)
{
	અચલ અક્ષર *exec_path = git_exec_path();

	પાછા launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

સ્થિર પૂર્ણાંક launchctl_update_schedule(પૂર્ણાંક run_maintenance, પૂર્ણાંક fd UNUSED)
{
	જો (run_maintenance)
		પાછા launchctl_add_plists();
	નહીંતર
		પાછા launchctl_remove_plists();
}

સ્થિર પૂર્ણાંક is_schtasks_available(શૂન્ય)
{
	પૂર્ણાંક is_available;
	જો (get_schedule_cmd("schtasks", &is_available, ખાલી))
		પાછા is_available;

#ifdef GIT_WINDOWS_NATIVE
	પાછા 1;
#નહીંતર
	પાછા 0;
#endif
}

સ્થિર અક્ષર *schtasks_task_name(અચલ અક્ષર *frequency)
{
	સંરચના strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	પાછા strbuf_detach(&label, ખાલી);
}

સ્થિર પૂર્ણાંક schtasks_remove_task(ગણના schedule_priority schedule)
{
	અક્ષર *cmd;
	સંરચના child_process child = CHILD_PROCESS_INIT;
	અચલ અક્ષર *frequency = get_frequency(schedule);
	અક્ષર *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", ખાલી, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", ખાલી);
	મુક્ત(name);
	મુક્ત(cmd);

	પાછા run_command(&child);
}

સ્થિર પૂર્ણાંક schtasks_remove_tasks(શૂન્ય)
{
	પાછા schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

સ્થિર પૂર્ણાંક schtasks_schedule_task(અચલ અક્ષર *exec_path, ગણના schedule_priority schedule)
{
	અક્ષર *cmd;
	પૂર્ણાંક result;
	સંરચના child_process child = CHILD_PROCESS_INIT;
	અચલ અક્ષર *xml;
	સંરચના tempfile *tfile;
	અચલ અક્ષર *frequency = get_frequency(schedule);
	અક્ષર *name = schtasks_task_name(frequency);
	સંરચના strbuf tfilename = STRBUF_INIT;
	પૂર્ણાંક minute = get_random_minute();

	get_schedule_cmd("schtasks", ખાલી, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	જો (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	પસંદગી (schedule) {
	સ્થિતિ SCHEDULE_HOURLY:
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
		તોડો;

	સ્થિતિ SCHEDULE_DAILY:
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
		તોડો;

	સ્થિતિ SCHEDULE_WEEKLY:
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
		તોડો;

	મૂળભૂત:
		તોડો;
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
				  get_tempfile_path(tfile), ખાલી);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	જો (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	મુક્ત(name);
	મુક્ત(cmd);
	પાછા result;
}

સ્થિર પૂર્ણાંક schtasks_schedule_tasks(શૂન્ય)
{
	અચલ અક્ષર *exec_path = git_exec_path();

	પાછા schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

સ્થિર પૂર્ણાંક schtasks_update_schedule(પૂર્ણાંક run_maintenance, પૂર્ણાંક fd UNUSED)
{
	જો (run_maintenance)
		પાછા schtasks_schedule_tasks();
	નહીંતર
		પાછા schtasks_remove_tasks();
}

MAYBE_UNUSED
સ્થિર પૂર્ણાંક check_crontab_process(અચલ અક્ષર *cmd)
{
	સંરચના child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_ધકેલો(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	જો (start_command(&child))
		પાછા 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	પાછા 1;
}

સ્થિર પૂર્ણાંક is_crontab_available(શૂન્ય)
{
	અક્ષર *cmd;
	પૂર્ણાંક is_available;
	પૂર્ણાંક ret;

	જો (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		જાઓ out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#નહીંતર
	ret = check_crontab_process(cmd);
#endif

out:
	મુક્ત(cmd);
	પાછા ret;
}

#define BEGIN_લીટી "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_લીટી "# END GIT MAINTENANCE SCHEDULE"

સ્થિર પૂર્ણાંક crontab_update_schedule(પૂર્ણાંક run_maintenance, પૂર્ણાંક fd)
{
	અક્ષર *cmd;
	પૂર્ણાંક result = 0;
	પૂર્ણાંક in_old_region = 0;
	સંરચના child_process crontab_list = CHILD_PROCESS_INIT;
	સંરચના child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	સંરચના strbuf લીટી = STRBUF_INIT;
	સંરચના tempfile *tmpedit = ખાલી;
	પૂર્ણાંક minute = get_random_minute();

	get_schedule_cmd("crontab", ખાલી, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_ધકેલો(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	જો (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		જાઓ out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	જો (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		જાઓ out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	જો (!cron_in) {
		result = error(_("failed to open temporary file"));
		જાઓ out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	જ્યારે (!strbuf_getline_lf(&લીટી, cron_list)) {
		જો (!in_old_region && !strcmp(લીટી.buf, BEGIN_લીટી))
			in_old_region = 1;
		નહીંતર જો (in_old_region && !strcmp(લીટી.buf, END_લીટી))
			in_old_region = 0;
		નહીંતર જો (!in_old_region)
			fprintf(cron_in, "%s\n", લીટી.buf);
	}
	strbuf_release(&લીટી);

	જો (run_maintenance) {
		સંરચના strbuf લીટી_format = STRBUF_INIT;
		અચલ અક્ષર *exec_path = git_exec_path();

		fprintf(cron_in, "%s\n", BEGIN_લીટી);
		fprintf(cron_in,
			"# The following schedule was created by Git\n");
		fprintf(cron_in, "# Any edits made in this region might be\n");
		fprintf(cron_in,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&લીટી_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_path, exec_path, get_extra_config_parameters());
		fprintf(cron_in, લીટી_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_in, લીટી_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_in, લીટી_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&લીટી_format);

		fprintf(cron_in, "\n%s\n", END_લીટી);
	}

	fflush(cron_in);

	strvec_split(&crontab_edit.args, cmd);
	strvec_ધકેલો(&crontab_edit.args, get_tempfile_path(tmpedit));
	crontab_edit.git_cmd = 0;

	જો (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		જાઓ out;
	}

	જો (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	નહીંતર
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	મુક્ત(cmd);
	પાછા result;
}

સ્થિર પૂર્ણાંક real_is_systemd_timer_available(શૂન્ય)
{
	સંરચના child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", ખાલી);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	જો (start_command(&child))
		પાછા 0;
	જો (finish_command(&child))
		પાછા 0;
	પાછા 1;
}

સ્થિર પૂર્ણાંક is_systemd_timer_available(શૂન્ય)
{
	પૂર્ણાંક is_available;

	જો (get_schedule_cmd("systemctl", &is_available, ખાલી))
		પાછા is_available;

	પાછા real_is_systemd_timer_available();
}

સ્થિર અક્ષર *xdg_config_home_systemd(અચલ અક્ષર *filename)
{
	પાછા xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

સ્થિર પૂર્ણાંક systemd_timer_delete_timer_file(ગણના schedule_priority priority)
{
	પૂર્ણાંક ret = 0;
	અચલ અક્ષર *frequency = get_frequency(priority);
	અક્ષર *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	અક્ષર *filename = xdg_config_home_systemd(local_timer_name);

	જો (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	મુક્ત(filename);
	મુક્ત(local_timer_name);
	પાછા ret;
}

સ્થિર પૂર્ણાંક systemd_timer_delete_service_template(શૂન્ય)
{
	પૂર્ણાંક ret = 0;
	અક્ષર *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	અક્ષર *filename = xdg_config_home_systemd(local_service_name);
	જો (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	મુક્ત(filename);
	મુક્ત(local_service_name);
	પાછા ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
સ્થિર પૂર્ણાંક systemd_timer_write_timer_file(ગણના schedule_priority schedule,
					  પૂર્ણાંક minute)
{
	પૂર્ણાંક res = -1;
	અક્ષર *filename;
	FILE *file;
	અચલ અક્ષર *unit;
	અક્ષર *schedule_pattern = ખાલી;
	અચલ અક્ષર *frequency = get_frequency(schedule);
	અક્ષર *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	જો (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		જાઓ error;
	}
	file = fopen_or_warn(filename, "w");
	જો (!file)
		જાઓ error;

	પસંદગી (schedule) {
	સ્થિતિ SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		તોડો;

	સ્થિતિ SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		તોડો;

	સ્થિતિ SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		તોડો;

	મૂળભૂત:
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
	જો (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		જાઓ error;
	}
	જો (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		જાઓ error;
	}

	res = 0;

error:
	મુક્ત(schedule_pattern);
	મુક્ત(local_timer_name);
	મુક્ત(filename);
	પાછા res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
સ્થિર પૂર્ણાંક systemd_timer_write_service_template(અચલ અક્ષર *exec_path)
{
	પૂર્ણાંક res = -1;
	અક્ષર *filename;
	FILE *file;
	અચલ અક્ષર *unit;
	અક્ષર *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	જો (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		જાઓ error;
	}
	file = fopen_or_warn(filename, "w");
	જો (!file)
		જાઓ error;

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
	જો (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		જાઓ error;
	}
	જો (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		જાઓ error;
	}

	res = 0;

error:
	મુક્ત(local_service_name);
	મુક્ત(filename);
	પાછા res;
}

સ્થિર પૂર્ણાંક systemd_timer_enable_unit(પૂર્ણાંક enable,
				     ગણના schedule_priority schedule,
				     પૂર્ણાંક minute)
{
	અક્ષર *cmd = ખાલી;
	સંરચના child_process child = CHILD_PROCESS_INIT;
	અચલ અક્ષર *frequency = get_frequency(schedule);
	પૂર્ણાંક ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	જો (!enable) {
		child.no_stderr = 1;
	} નહીંતર જો (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		જાઓ out;
	}

	get_schedule_cmd("systemctl", ખાલી, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", ખાલી);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	જો (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		જાઓ out;
	}

	જો (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		જો (enable) {
			ret = error(_("failed to run systemctl"));
			જાઓ out;
		}
	}

	ret = 0;

out:
	મુક્ત(cmd);
	પાછા ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
સ્થિર શૂન્ય systemd_timer_delete_stale_timer_templates(શૂન્ય)
{
	અક્ષર *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	અક્ષર *filename = xdg_config_home_systemd(timer_template_name);

	જો (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	મુક્ત(filename);
	મુક્ત(timer_template_name);
}

સ્થિર પૂર્ણાંક systemd_timer_delete_unit_files(શૂન્ય)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	પાછા systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

સ્થિર પૂર્ણાંક systemd_timer_delete_units(શૂન્ય)
{
	પૂર્ણાંક minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	પાછા systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

સ્થિર પૂર્ણાંક systemd_timer_setup_units(શૂન્ય)
{
	પૂર્ણાંક minute = get_random_minute();
	અચલ અક્ષર *exec_path = git_exec_path();

	પૂર્ણાંક ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	જો (ret)
		systemd_timer_delete_units();
	નહીંતર
		systemd_timer_delete_stale_timer_templates();

	પાછા ret;
}

સ્થિર પૂર્ણાંક systemd_timer_update_schedule(પૂર્ણાંક run_maintenance, પૂર્ણાંક fd UNUSED)
{
	જો (run_maintenance)
		પાછા systemd_timer_setup_units();
	નહીંતર
		પાછા systemd_timer_delete_units();
}

ગણના scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

સ્થિર અચલ સંરચના {
	અચલ અક્ષર *name;
	પૂર્ણાંક (*is_available)(શૂન્ય);
	પૂર્ણાંક (*update_schedule)(પૂર્ણાંક run_maintenance, પૂર્ણાંક fd);
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

સ્થિર ગણના scheduler parse_scheduler(અચલ અક્ષર *કિંમત)
{
	જો (!કિંમત)
		પાછા SCHEDULER_INVALID;
	નહીંતર જો (!strcasecmp(કિંમત, "auto"))
		પાછા SCHEDULER_AUTO;
	નહીંતર જો (!strcasecmp(કિંમત, "cron") || !strcasecmp(કિંમત, "crontab"))
		પાછા SCHEDULER_CRON;
	નહીંતર જો (!strcasecmp(કિંમત, "systemd") ||
		 !strcasecmp(કિંમત, "systemd-timer"))
		પાછા SCHEDULER_SYSTEMD;
	નહીંતર જો (!strcasecmp(કિંમત, "launchctl"))
		પાછા SCHEDULER_LAUNCHCTL;
	નહીંતર જો (!strcasecmp(કિંમત, "schtasks"))
		પાછા SCHEDULER_SCHTASKS;
	નહીંતર
		પાછા SCHEDULER_INVALID;
}

સ્થિર પૂર્ણાંક maintenance_opt_scheduler(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
				     પૂર્ણાંક unset)
{
	ગણના scheduler *scheduler = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	જો (*scheduler == SCHEDULER_INVALID)
		પાછા error(_("unrecognized --scheduler argument '%s'"), arg);
	પાછા 0;
}

સંરચના maintenance_start_opts {
	ગણના scheduler scheduler;
};

સ્થિર ગણના scheduler resolve_scheduler(ગણના scheduler scheduler)
{
	જો (scheduler != SCHEDULER_AUTO)
		પાછા scheduler;

#જો defined(__APPLE__)
	પાછા SCHEDULER_LAUNCHCTL;

#અથવાજો defined(GIT_WINDOWS_NATIVE)
	પાછા SCHEDULER_SCHTASKS;

#અથવાજો defined(__linux__)
	જો (is_systemd_timer_available())
		પાછા SCHEDULER_SYSTEMD;
	નહીંતર જો (is_crontab_available())
		પાછા SCHEDULER_CRON;
	નહીંતર
		die(_("neither systemd timers nor crontab are available"));

#નહીંતર
	પાછા SCHEDULER_CRON;
#endif
}

સ્થિર શૂન્ય validate_scheduler(ગણના scheduler scheduler)
{
	જો (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	જો (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	જો (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

સ્થિર પૂર્ણાંક update_background_schedule(અચલ સંરચના maintenance_start_opts *opts,
				      પૂર્ણાંક enable)
{
	unsigned પૂર્ણાંક i;
	પૂર્ણાંક result = 0;
	સંરચના lock_file lk;
	અક્ષર *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	જો (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		જો (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		નહીંતર
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		મુક્ત(lock_path);
		પાછા -1;
	}

	માટે (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		જો (enable && opts->scheduler == i)
			ચાલુ;
		જો (!scheduler_fn[i].is_available())
			ચાલુ;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	જો (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	મુક્ત(lock_path);
	પાછા result;
}

સ્થિર અચલ અક્ષર *અચલ builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	ખાલી
};

સ્થિર પૂર્ણાંક maintenance_start(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			     સંરચના repository *repo)
{
	સંરચના maintenance_start_opts opts = { 0 };
	સંરચના option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	અચલ અક્ષર *register_args[] = { "register", ખાલી };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	જો (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	જો (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	જો (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, ખાલી, repo))
		warning(_("failed to add repo to global config"));
	પાછા 0;
}

સ્થિર અચલ અક્ષર *અચલ builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	ખાલી
};

સ્થિર પૂર્ણાંક maintenance_stop(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			    સંરચના repository *repo UNUSED)
{
	સંરચના option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	જો (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	પાછા update_background_schedule(ખાલી, 0);
}

સ્થિર અચલ અક્ષર *અચલ builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	ખાલી
};

સ્થિર પૂર્ણાંક maintenance_is_needed(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				 સંરચના repository *repo UNUSED)
{
	સંરચના maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	સંરચના string_list selected_tasks = STRING_LIST_INIT_DUP;
	સંરચના gc_config cfg = GC_CONFIG_INIT;
	સંરચના option options[] = {
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
	જો (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	જો (opts.auto_flag) {
		માટે (size_t i = 0; i < opts.tasks_nr; i++) {
			જો (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				તોડો;
			}
		}
	} નહીંતર {
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

	જો (is_needed)
		પાછા 0;
	પાછા 1;
}

સ્થિર અચલ અક્ષર *અચલ builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	ખાલી,
};

પૂર્ણાંક cmd_maintenance(પૂર્ણાંક argc,
		    અચલ અક્ષર **argv,
		    અચલ અક્ષર *prefix,
		    સંરચના repository *repo)
{
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option builtin_maintenance_options[] = {
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
	પાછા fn(argc, argv, prefix, repo);
}
