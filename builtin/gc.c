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

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "date.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "tempfile.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "sigchain.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "commit-graph.h"
#ଅନ୍ତର୍ଭାଵ "packfile.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "reflog.h"
#ଅନ୍ତର୍ଭାଵ "rerere.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "exec-cmd.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hook.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "trace2.h"
#ଅନ୍ତର୍ଭାଵ "worktree.h"

#define FAILED_RUN "failed to run %s"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	ଖାଲି
};

ସ୍ଥିର timestamp_t gc_log_expire_time;
ସ୍ଥିର ସଂରଚନା tempfile *pidfile;
ସ୍ଥିର ସଂରଚନା lock_file log_lock;
ସ୍ଥିର ସଂରଚନା string_list pack_garbage = STRING_LIST_INIT_DUP;

ସ୍ଥିର ଶୂନ୍ୟ clean_pack_garbage(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପାଇଁ (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
	string_list_clear(&pack_garbage, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ report_pack_garbage(unsigned seen_bits, ଅଚଳ ଅକ୍ଷର *path)
{
	ଯଦି (seen_bits == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, path);
}

ସ୍ଥିର ଶୂନ୍ୟ process_log_file(ଶୂନ୍ୟ)
{
	ସଂରଚନା stat st;
	ଯଦି (fstat(get_lock_file_fd(&log_lock), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_lock_file_path(&log_lock),
			strerror(saved_errno));
		fflush(stderr);
		commit_lock_file(&log_lock);
		errno = saved_errno;
	} ଅନ୍ୟଥା ଯଦି (st.st_size) {
		/* There was some error recorded in the lock file */
		commit_lock_file(&log_lock);
	} ଅନ୍ୟଥା {
		ଅକ୍ଷର *path = repo_git_path(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(path);
		rollback_lock_file(&log_lock);
		ମୁକ୍ତ(path);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ process_log_file_at_exit(ଶୂନ୍ୟ)
{
	fflush(stderr);
	process_log_file();
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା gc_config_is_timestamp_never(ଅଚଳ ଅକ୍ଷର *var)
{
	ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ;
	timestamp_t expire;

	ଯଦି (!repo_config_get_ମୂଲ୍ୟ(the_repository, var, &ମୂଲ୍ୟ) && ମୂଲ୍ୟ) {
		ଯଦି (parse_expiry_date(ମୂଲ୍ୟ, &expire))
			die(_("failed to parse '%s' value '%s'"), var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ expire == 0;
	}
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା gc_config {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_refs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune_reflogs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cruft_packs;
	unsigned long max_cruft_size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା aggressive_depth;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା aggressive_window;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach_auto;
	ଅକ୍ଷର *gc_log_expire;
	ଅକ୍ଷର *prune_expire;
	ଅକ୍ଷର *prune_worktrees_expire;
	ଅକ୍ଷର *repack_expire_to;
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

ସ୍ଥିର ଶୂନ୍ୟ gc_config_release(ସଂରଚନା gc_config *cfg)
{
	ମୁକ୍ତ(cfg->gc_log_expire);
	ମୁକ୍ତ(cfg->prune_expire);
	ମୁକ୍ତ(cfg->prune_worktrees_expire);
}

ସ୍ଥିର ଶୂନ୍ୟ gc_config(ସଂରଚନା gc_config *cfg)
{
	ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ;
	ଅକ୍ଷର *owned = ଖାଲି;

	ଯଦି (!repo_config_get_ମୂଲ୍ୟ(the_repository, "gc.packrefs", &ମୂଲ୍ୟ)) {
		ଯଦି (ମୂଲ୍ୟ && !strcmp(ମୂଲ୍ୟ, "notbare"))
			cfg->pack_refs = -1;
		ଅନ୍ୟଥା
			cfg->pack_refs = git_config_bool("gc.packrefs", ମୂଲ୍ୟ);
	}

	ଯଦି (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_window);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_depth);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_size);

	ଯଦି (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		ମୁକ୍ତ(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	ଯଦି (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		ମୁକ୍ତ(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	ଯଦି (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		ମୁକ୍ତ(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, ଖାଲି);
}

ଗଣନା schedule_priority {
	SCHEDULE_NONE = 0,
	SCHEDULE_WEEKLY = 1,
	SCHEDULE_DAILY = 2,
	SCHEDULE_HOURLY = 3,
};

ସ୍ଥିର ଗଣନା schedule_priority parse_schedule(ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ଯଦି (!ମୂଲ୍ୟ)
		ଫେରନ୍ତୁ SCHEDULE_NONE;
	ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "hourly"))
		ଫେରନ୍ତୁ SCHEDULE_HOURLY;
	ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "daily"))
		ଫେରନ୍ତୁ SCHEDULE_DAILY;
	ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "weekly"))
		ଫେରନ୍ତୁ SCHEDULE_WEEKLY;
	ଫେରନ୍ତୁ SCHEDULE_NONE;
}

ଗଣନା maintenance_task_label {
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
	TASK_ଗଣନା
};

ସଂରଚନା maintenance_run_opts {
	ଗଣନା maintenance_task_label *tasks;
	size_t tasks_nr, tasks_alloc;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_flag;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet;
	ଗଣନା schedule_priority schedule;
};
#define MAINTENANCE_RUN_OPTS_INIT { \
	.detach = -1, \
}

ସ୍ଥିର ଶୂନ୍ୟ maintenance_run_opts_release(ସଂରଚନା maintenance_run_opts *opts)
{
	ମୁକ୍ତ(opts->tasks);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_refs_condition(UNUSED ସଂରଚନା gc_config *cfg)
{
	ସଂରଚନା string_list included_refs = STRING_LIST_INIT_NODUP;
	ସଂରଚନା ref_exclusions excludes = REF_EXCLUSIONS_INIT;
	ସଂରଚନା refs_optimize_opts optimize_opts = {
		.exclusions = &excludes,
		.includes = &included_refs,
		.flags = REFS_OPTIMIZE_PRUNE | REFS_OPTIMIZE_AUTO,
	};
	bool required;

	/* Check for all refs, similar to 'git refs optimize --all'. */
	string_list_append(optimize_opts.includes, "*");

	ଯଦି (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		ଫେରନ୍ତୁ 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	ଫେରନ୍ତୁ required;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_pack_refs(ସଂରଚନା maintenance_run_opts *opts,
				      UNUSED ସଂରଚନା gc_config *cfg)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "pack-refs", "--all", "--prune", ଖାଲି);
	ଯଦି (opts->auto_flag)
		strvec_ଠେଲ(&cmd.args, "--auto");

	ଫେରନ୍ତୁ run_command(&cmd);
}

ସଂରଚନା ଗଣନା_reflog_entries_data {
	ସଂରଚନା expire_reflog_policy_cb policy;
	size_t ଗଣନା_2;
	size_t limit;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_reflog_entries(ଅଚଳ ଅକ୍ଷର *refname UNUSED,
				ସଂରଚନା object_id *old_oid, ସଂରଚନା object_id *new_oid,
				ଅଚଳ ଅକ୍ଷର *committer, timestamp_t timestamp,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା tz, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା ଗଣନା_reflog_entries_data *data = cb_data;
	ଯଦି (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, ସନ୍ଦେଶ, &data->policy))
		data->ଗଣନା_2++;
	ଫେରନ୍ତୁ data->ଗଣନା_2 >= data->limit;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reflog_expire_condition(ସଂରଚନା gc_config *cfg UNUSED)
{
	timestamp_t now = time(ଖାଲି);
	ସଂରଚନା ଗଣନା_reflog_entries_data data = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା limit = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &limit);
	ଯଦି (!limit)
		ଫେରନ୍ତୁ 0;
	ଯଦି (limit < 0)
		ଫେରନ୍ତୁ 1;
	data.limit = limit;

	repo_config(the_repository, reflog_expire_config, &data.policy.opts);

	reflog_expire_options_set_refname(&data.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 ଗଣନା_reflog_entries, &data);

	reflog_expiry_cleanup(&data.policy);
	reflog_clear_expire_config(&data.policy.opts);
	ଫେରନ୍ତୁ data.ଗଣନା_2 >= data.limit;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_reflog_expire(ସଂରଚନା maintenance_run_opts *opts UNUSED,
					  ସଂରଚନା gc_config *cfg UNUSED)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "reflog", "expire", "--all", ଖାଲି);
	ଫେରନ୍ତୁ run_command(&cmd);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_worktree_prune(ସଂରଚନା maintenance_run_opts *opts UNUSED,
					   ସଂରଚନା gc_config *cfg)
{
	ସଂରଚନା child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.args, "worktree", "prune", "--expire", ଖାଲି);
	strvec_ଠେଲ(&prune_worktrees_cmd.args, cfg->prune_worktrees_expire);

	ଫେରନ୍ତୁ run_command(&prune_worktrees_cmd);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା worktree_prune_condition(ସଂରଚନା gc_config *cfg)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା should_prune = 0, limit = 1;
	timestamp_t expiry_date;
	ସଂରଚନା dirent *d;
	DIR *dir = ଖାଲି;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &limit);
	ଯଦି (limit <= 0) {
		should_prune = limit < 0;
		ଯାଅ out;
	}

	ଯଦି (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		ଯାଅ out;

	dir = opendir(repo_git_path_replace(the_repository, &buf, "worktrees"));
	ଯଦି (!dir)
		ଯାଅ out;

	ଯେତେବେଳେ (limit && (d = readdir_skip_dot_and_dotdot(dir))) {
		ଅକ୍ଷର *wtpath;
		strbuf_reset(&buf);
		ଯଦି (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			limit--;
		ମୁକ୍ତ(wtpath);
	}

	should_prune = !limit;

out:
	ଯଦି (dir)
		closedir(dir);
	strbuf_release(&buf);
	ଫେରନ୍ତୁ should_prune;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_rerere_gc(ସଂରଚନା maintenance_run_opts *opts UNUSED,
				      ସଂରଚନା gc_config *cfg UNUSED)
{
	ସଂରଚନା child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.args, "rerere", "gc", ଖାଲି);
	ଫେରନ୍ତୁ run_command(&rerere_cmd);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rerere_gc_condition(ସଂରଚନା gc_config *cfg UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା limit = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &limit);
	ଯଦି (!limit)
		ଫେରନ୍ତୁ 0; /* never prune */
	ଯଦି (limit < 0)
		ଫେରନ୍ତୁ 1; /* always prune */

	ଫେରନ୍ତୁ rerere_gc_needed(the_repository, (size_t)limit);
}

#define OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_to = (cfg)->repack_expire_to, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_size = (cfg)->max_cruft_size, \
	.window = (aggressive) ? (cfg)->aggressive_window : 0, \
	.depth = (aggressive) ? (cfg)->aggressive_depth : 0

/* return NULL on success, else hostname running the gc */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *lock_repo_for_gc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା force, pid_t* ret_pid)
{
	ସଂରଚନା lock_file lock = LOCK_INIT;
	ଅକ୍ଷର my_host[HOST_NAME_MAX + 1];
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ସଂରଚନା stat st;
	uintmax_t pid;
	FILE *fp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	ଅକ୍ଷର *pidfile_path;

	ଯଦି (is_tempfile_active(pidfile))
		/* already locked */
		ଫେରନ୍ତୁ ଖାଲି;

	ଯଦି (xgethostname(my_host, ଆକାର(my_host)))
		xsnprintf(my_host, ଆକାର(my_host), "unknown");

	pidfile_path = repo_git_path(the_repository, "gc.pid");
	fd = hold_lock_file_for_update(&lock, pidfile_path,
				       LOCK_DIE_ON_ERROR);
	ଯଦି (!force) {
		ସ୍ଥିର ଅକ୍ଷର locking_host[HOST_NAME_MAX + 1];
		ସ୍ଥିର ଅକ୍ଷର *scan_fmt;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା should_exit;

		ଯଦି (!scan_fmt)
			scan_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_path, "r");
		memset(locking_host, 0, ଆକାର(locking_host));
		should_exit =
			fp != ଖାଲି &&
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
			time(ଖାଲି) - st.st_mtime <= 12 * 3600 &&
			fscanf(fp, scan_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(strcmp(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		ଯଦି (fp)
			fclose(fp);
		ଯଦି (should_exit) {
			ଯଦି (fd >= 0)
				rollback_lock_file(&lock);
			*ret_pid = pid;
			ମୁକ୍ତ(pidfile_path);
			ଫେରନ୍ତୁ locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_in_full(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_lock_file(&lock);
	pidfile = register_tempfile(pidfile_path);
	ମୁକ୍ତ(pidfile_path);
	ଫେରନ୍ତୁ ଖାଲି;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା report_last_gc_error(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ssize_t len;
	ସଂରଚନା stat st;
	ଅକ୍ଷର *gc_log_path = repo_git_path(the_repository, "gc.log");

	ଯଦି (stat(gc_log_path, &st)) {
		ଯଦି (errno == ENOENT)
			ଯାଅ done;

		ret = die_message_errno(_("cannot stat '%s'"), gc_log_path);
		ଯାଅ done;
	}

	ଯଦି (st.st_mtime < gc_log_expire_time)
		ଯାଅ done;

	len = strbuf_read_file(&sb, gc_log_path, 0);
	ଯଦି (len < 0)
		ret = die_message_errno(_("cannot read '%s'"), gc_log_path);
	ଅନ୍ୟଥା ଯଦି (len > 0) {
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
	ମୁକ୍ତ(gc_log_path);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା gc_foreground_tasks(ସଂରଚନା maintenance_run_opts *opts,
			       ସଂରଚନା gc_config *cfg)
{
	ଯଦି (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		ଫେରନ୍ତୁ error(FAILED_RUN, "pack-refs");
	ଯଦି (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		ଫେରନ୍ତୁ error(FAILED_RUN, "reflog");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_odb(ସଂରଚନା maintenance_run_opts *opts,
				ସଂରଚନା gc_config *cfg,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_largest_pack,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା aggressive)
{
	ସଂରଚନା odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	ଯଦି (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	ଯଦି (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	ଯଦି (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	ଫେରନ୍ତୁ odb_optimize(the_repository->objects, &odb_opts);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_gc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	   ଅଚଳ ଅକ୍ଷର **argv,
	   ଅଚଳ ଅକ୍ଷର *prefix,
	   ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା aggressive = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = 0;
	ଅଚଳ ଅକ୍ଷର *name;
	pid_t pid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା daemonized = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_largest_pack = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip_foreground_tasks = 0;
	timestamp_t dummy;
	ସଂରଚନା maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ସଂରଚନା gc_config cfg = GC_CONFIG_INIT;
	ଅଚଳ ଅକ୍ଷର *prune_expire_sentinel = "sentinel";
	ଅଚଳ ଅକ୍ଷର *prune_expire_arg = prune_expire_sentinel;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା option builtin_gc_options[] = {
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		{
			.type = OPTION_STRING,
			.long_name = "prune",
			.ମୂଲ୍ୟ = &prune_expire_arg,
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

	ଯଦି (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	ଯଦି (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, prefix, builtin_gc_options,
			     builtin_gc_usage, 0);
	ଯଦି (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	ଯଦି (prune_expire_arg != prune_expire_sentinel) {
		ମୁକ୍ତ(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	ଯଦି (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	ଯଦି (opts.auto_flag) {
		ସଂରଚନା odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		ଯଦି (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		ଯଦି (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    run_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			ଯାଅ out;
		}

		ଯଦି (!opts.quiet) {
			ଯଦି (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			ଅନ୍ୟଥା
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	ଯଦି (opts.detach > 0) {
		ret = report_last_gc_error();
		ଯଦି (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			ଯାଅ out;

		} ଅନ୍ୟଥା ଯଦି (ret) {
			/* an I/O error occurred, already reported */
			ଯାଅ out;
		}

		ଯଦି (!skip_foreground_tasks) {
			ଯଦି (lock_repo_for_gc(force, &pid)) {
				ret = 0;
				ଯାଅ out;
			}

			ଯଦି (gc_foreground_tasks(&opts, &cfg) < 0)
				die(ଖାଲି);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = lock_repo_for_gc(force, &pid);
	ଯଦି (name) {
		ଯଦି (opts.auto_flag) {
			ret = 0;
			ଯାଅ out; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	ଯଦି (daemonized) {
		ଅକ୍ଷର *path = repo_git_path(the_repository, "gc.log");
		hold_lock_file_for_update(&log_lock, path,
					  LOCK_DIE_ON_ERROR);
		dup2(get_lock_file_fd(&log_lock), 2);
		atexit(process_log_file_at_exit);
		ମୁକ୍ତ(path);
	}

	ଯଦି (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	ଯଦି (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_RUN, "worktree");

	ଯଦି (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_RUN, "rerere");

	ଯଦି (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(ଖାଲି);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	ଯଦି (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	ଯଦି (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->path,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_PROGRESS : 0,
					     ଖାଲି);

	ଯଦି (!daemonized) {
		ଅକ୍ଷର *path = repo_git_path(the_repository, "gc.log");
		unlink(path);
		ମୁକ୍ତ(path);
	}

out:
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_maintenance_run_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_opt_schedule(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଗଣନା schedule_priority *priority = opt->ମୂଲ୍ୟ;

	ଯଦି (unset)
		die(_("--no-schedule is not allowed"));

	*priority = parse_schedule(arg);

	ଯଦି (!*priority)
		die(_("unrecognized --schedule argument '%s'"), arg);

	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା cg_auto_data {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_not_in_graph;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା limit;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfs_on_ref(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା cg_auto_data *data = (ସଂରଚନା cg_auto_data *)cb_data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ଅଚଳ ସଂରଚନା object_id *maybe_peeled = ref->oid;
	ସଂରଚନା object_id peeled;
	ସଂରଚନା commit_list *ଗଦା = ଖାଲି;
	ସଂରଚନା commit *commit;

	ଯଦି (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	ଯଦି (odb_read_object_info(the_repository->objects, maybe_peeled, ଖାଲି) != OBJ_COMMIT)
		ଫେରନ୍ତୁ 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	ଯଦି (!commit || commit->object.flags & SEEN)
		ଫେରନ୍ତୁ 0;
	commit->object.flags |= SEEN;

	ଯଦି (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		ଫେରନ୍ତୁ 0;

	data->ଅଙ୍କ_not_in_graph++;

	ଯଦି (data->ଅଙ୍କ_not_in_graph >= data->limit)
		ଫେରନ୍ତୁ 1;

	commit_list_ଭର୍ତ୍ତି(commit, &ଗଦା);

	ଯେତେବେଳେ (!result && ଗଦା) {
		ସଂରଚନା commit_list *parent;

		commit = କାଢ଼_commit(&ଗଦା);

		ପାଇଁ (parent = commit->parents; parent; parent = parent->next) {
			ଯଦି (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & SEEN)
				ଜାରି;

			parent->item->object.flags |= SEEN;
			data->ଅଙ୍କ_not_in_graph++;

			ଯଦି (data->ଅଙ୍କ_not_in_graph >= data->limit) {
				result = 1;
				ଭାଙ୍ଗ;
			}

			commit_list_ଭର୍ତ୍ତି(parent->item, &ଗଦା);
		}
	}

	commit_list_free(ଗଦା);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା should_write_commit_graph(ସଂରଚନା gc_config *cfg UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;
	ସଂରଚନା cg_auto_data data;

	data.ଅଙ୍କ_not_in_graph = 0;
	data.limit = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &data.limit);

	ଯଦି (!data.limit)
		ଫେରନ୍ତୁ 0;
	ଯଦି (data.limit < 0)
		ଫେରନ୍ତୁ 1;

	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &data);

	repo_clear_commit_marks(the_repository, SEEN);

	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_write_commit_graph(ସଂରଚନା maintenance_run_opts *opts)
{
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "commit-graph", "write",
		     "--split", "--reachable", ଖାଲି);

	ଯଦି (opts->quiet)
		strvec_ଠେଲ(&child.args, "--no-progress");
	ଅନ୍ୟଥା
		strvec_ଠେଲ(&child.args, "--progress");

	ଫେରନ୍ତୁ !!run_command(&child);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_commit_graph(ସଂରଚନା maintenance_run_opts *opts,
					 ସଂରଚନା gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	ଯଦି (!the_repository->settings.core_commit_graph)
		ଫେରନ୍ତୁ 0;

	ଯଦି (run_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		ଫେରନ୍ତୁ 1;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetch_remote(ସଂରଚନା remote *remote, ଶୂନ୍ୟ *cbdata)
{
	ସଂରଚନା maintenance_run_opts *opts = cbdata;
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	ଯଦି (remote->skip_default_update)
		ଫେରନ୍ତୁ 0;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     ଖାଲି);

	ଯଦି (opts->quiet)
		strvec_ଠେଲ(&child.args, "--quiet");

	ଫେରନ୍ତୁ !!run_command(&child);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_prefetch(ସଂରଚନା maintenance_run_opts *opts,
				     ସଂରଚନା gc_config *cfg UNUSED)
{
	ଯଦି (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		ଫେରନ୍ତୁ 1;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_gc_foreground(ସଂରଚନା maintenance_run_opts *opts,
					  ସଂରଚନା gc_config *cfg)
{
	ଫେରନ୍ତୁ gc_foreground_tasks(opts, cfg);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_gc_background(ସଂରଚନା maintenance_run_opts *opts,
					  ସଂରଚନା gc_config *cfg UNUSED)
{
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_ଠେଲ(&child.args, "gc");

	ଯଦି (opts->auto_flag)
		strvec_ଠେଲ(&child.args, "--auto");
	ଯଦି (opts->quiet)
		strvec_ଠେଲ(&child.args, "--quiet");
	ଅନ୍ୟଥା
		strvec_ଠେଲ(&child.args, "--no-quiet");
	strvec_ଠେଲ(&child.args, "--no-detach");
	strvec_ଠେଲ(&child.args, "--skip-foreground-tasks");

	ଫେରନ୍ତୁ run_command(&child);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା gc_condition(ସଂରଚନା gc_config *cfg)
{
	ସଂରଚନା odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	ଫେରନ୍ତୁ odb_optimize_required(the_repository->objects, &opts);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune_packed(ସଂରଚନା maintenance_run_opts *opts)
{
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_ଠେଲ(&child.args, "prune-packed");

	ଯଦି (opts->quiet)
		strvec_ଠେଲ(&child.args, "--quiet");

	ଫେରନ୍ତୁ !!run_command(&child);
}

ସଂରଚନା write_loose_object_data {
	FILE *in;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_size;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା loose_object_auto_limit = 100;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା loose_object_ଗଣନା(ଅଚଳ ସଂରଚନା object_id *oid UNUSED,
			      ଅଚଳ ଅକ୍ଷର *path UNUSED,
			      ଶୂନ୍ୟ *data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଗଣନା_2 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)data;
	ଯଦି (++(*ଗଣନା_2) >= loose_object_auto_limit)
		ଫେରନ୍ତୁ 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା loose_object_auto_condition(ସଂରଚନା gc_config *cfg UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_limit);

	ଯଦି (!loose_object_auto_limit)
		ଫେରନ୍ତୁ 0;
	ଯଦି (loose_object_auto_limit < 0)
		ଫେରନ୍ତୁ 1;

	ଫେରନ୍ତୁ for_each_loose_file_in_source(the_repository->objects->source,
					     loose_object_ଗଣନା,
					     ଖାଲି, ଖାଲି, &ଗଣନା_2);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bail_on_loose(ଅଚଳ ସଂରଚନା object_id *oid UNUSED,
			 ଅଚଳ ଅକ୍ଷର *path UNUSED,
			 ଶୂନ୍ୟ *data UNUSED)
{
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_loose_object_to_stdin(ଅଚଳ ସଂରଚନା object_id *oid,
				       ଅଚଳ ଅକ୍ଷର *path UNUSED,
				       ଶୂନ୍ୟ *data)
{
	ସଂରଚନା write_loose_object_data *d = (ସଂରଚନା write_loose_object_data *)data;

	fprintf(d->in, "%s\n", oid_to_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	ଫେରନ୍ତୁ ++(d->ଗଣନା_2) > d->batch_size;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_loose(ସଂରଚନା maintenance_run_opts *opts)
{
	ସଂରଚନା repository *r = the_repository;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ସଂରଚନା write_loose_object_data data;
	ସଂରଚନା child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	ଯଦି (!for_each_loose_file_in_source(r->objects->source,
					   bail_on_loose,
					   ଖାଲି, ଖାଲି, ଖାଲି))
		ଫେରନ୍ତୁ 0;

	pack_proc.git_cmd = 1;

	strvec_ଠେଲ(&pack_proc.args, "pack-objects");
	ଯଦି (opts->quiet)
		strvec_ଠେଲ(&pack_proc.args, "--quiet");
	ଅନ୍ୟଥା
		strvec_ଠେଲ(&pack_proc.args, "--no-quiet");
	strvec_pushf(&pack_proc.args, "%s/pack/loose", r->objects->source->path);

	pack_proc.in = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.out = -1;

	ଯଦି (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		ଫେରନ୍ତୁ 1;
	}

	data.in = xfdopen(pack_proc.in, "w");
	data.ଗଣନା_2 = 0;
	data.batch_size = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &data.batch_size);

	/* If configured as 0, then remove limit. */
	ଯଦି (!data.batch_size)
		data.batch_size = INT_MAX;
	ଅନ୍ୟଥା ଯଦି (data.batch_size > 0)
		data.batch_size--; /* Decrease for equality on limit. */

	for_each_loose_file_in_source(r->objects->source,
				      write_loose_object_to_stdin,
				      ଖାଲି, ଖାଲି, &data);

	fclose(data.in);

	ଯଦି (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		result = 1;
	}

	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_loose_objects(ସଂରଚନା maintenance_run_opts *opts,
					  ସଂରଚନା gc_config *cfg UNUSED)
{
	ଫେରନ୍ତୁ prune_packed(opts) || pack_loose(opts);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା incremental_repack_auto_condition(ସଂରଚନା gc_config *cfg UNUSED)
{
	ସଂରଚନା packed_git *p;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା incremental_repack_auto_limit = 10;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;

	prepare_repo_settings(the_repository);
	ଯଦି (!the_repository->settings.core_multi_pack_index)
		ଫେରନ୍ତୁ 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_limit);

	ଯଦି (!incremental_repack_auto_limit)
		ଫେରନ୍ତୁ 0;
	ଯଦି (incremental_repack_auto_limit < 0)
		ଫେରନ୍ତୁ 1;

	repo_for_each_pack(the_repository, p) {
		ଯଦି (ଗଣନା_2 >= incremental_repack_auto_limit)
			ଭାଙ୍ଗ;
		ଯଦି (!p->multi_pack_index)
			ଗଣନା_2++;
	}

	ଫେରନ୍ତୁ ଗଣନା_2 >= incremental_repack_auto_limit;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା multi_pack_index_write(ସଂରଚନା maintenance_run_opts *opts)
{
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.args, "multi-pack-index", "write", ଖାଲି);

	ଯଦି (opts->quiet)
		strvec_ଠେଲ(&child.args, "--no-progress");
	ଅନ୍ୟଥା
		strvec_ଠେଲ(&child.args, "--progress");

	ଯଦି (run_command(&child))
		ଫେରନ୍ତୁ error(_("failed to write multi-pack-index"));

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା multi_pack_index_expire(ସଂରଚନା maintenance_run_opts *opts)
{
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "expire", ଖାଲି);

	ଯଦି (opts->quiet)
		strvec_ଠେଲ(&child.args, "--no-progress");
	ଅନ୍ୟଥା
		strvec_ଠେଲ(&child.args, "--progress");

	ଯଦି (run_command(&child))
		ଫେରନ୍ତୁ error(_("'git multi-pack-index expire' failed"));

	ଫେରନ୍ତୁ 0;
}

#define TWO_GIGABYTES (INT32_MAX)

ସ୍ଥିର off_t get_auto_pack_size(ଶୂନ୍ୟ)
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
	ସଂରଚନା packed_git *p;
	ସଂରଚନା repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		ଯଦି (p->pack_size > max_size) {
			second_largest_size = max_size;
			max_size = p->pack_size;
		} ଅନ୍ୟଥା ଯଦି (p->pack_size > second_largest_size)
			second_largest_size = p->pack_size;
	}

	result_size = second_largest_size + 1;

	/* But limit ourselves to a batch size of 2g */
	ଯଦି (result_size > TWO_GIGABYTES)
		result_size = TWO_GIGABYTES;

	ଫେରନ୍ତୁ result_size;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା multi_pack_index_repack(ସଂରଚନା maintenance_run_opts *opts)
{
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_to_close = the_repository->objects;
	strvec_pushl(&child.args, "multi-pack-index", "repack", ଖାଲି);

	ଯଦି (opts->quiet)
		strvec_ଠେଲ(&child.args, "--no-progress");
	ଅନ୍ୟଥା
		strvec_ଠେଲ(&child.args, "--progress");

	strvec_pushf(&child.args, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_size());

	ଯଦି (run_command(&child))
		ଫେରନ୍ତୁ error(_("'git multi-pack-index repack' failed"));

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_incremental_repack(ସଂରଚନା maintenance_run_opts *opts,
					       ସଂରଚନା gc_config *cfg UNUSED)
{
	prepare_repo_settings(the_repository);
	ଯଦି (!the_repository->settings.core_multi_pack_index) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (multi_pack_index_write(opts))
		ଫେରନ୍ତୁ 1;
	ଯଦି (multi_pack_index_expire(opts))
		ଫେରନ୍ତୁ 1;
	ଯଦି (multi_pack_index_repack(opts))
		ଫେରନ୍ତୁ 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_task_geometric_repack(ସଂରଚନା maintenance_run_opts *opts,
					     ସଂରଚନା gc_config *cfg)
{
	ସଂରଚନା odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	ଯଦି (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	ଫେରନ୍ତୁ odb_optimize(the_repository->objects, &odb_opts);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା geometric_repack_auto_condition(ସଂରଚନା gc_config *cfg)
{
	ସଂରଚନା odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	ଫେରନ୍ତୁ odb_optimize_required(the_repository->objects, &opts);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*maintenance_task_fn)(ସଂରଚନା maintenance_run_opts *opts,
				   ସଂରଚନା gc_config *cfg);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*maintenance_auto_fn)(ସଂରଚନା gc_config *cfg);

ସଂରଚନା maintenance_task {
	ଅଚଳ ଅକ୍ଷର *name;

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

ସ୍ଥିର ଅଚଳ ସଂରଚନା maintenance_task tasks[] = {
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

ଗଣନା task_phase {
	TASK_PHASE_FOREGROUND,
	TASK_PHASE_BACKGROUND,
};

ଗଣନା auto_gc_hook_result {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_RUN = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maybe_run_task(ଅଚଳ ସଂରଚନା maintenance_task *task,
			  ସଂରଚନା repository *repo,
			  ସଂରଚନା maintenance_run_opts *opts,
			  ସଂରଚନା gc_config *cfg,
			  ଗଣନା task_phase phase,
			  ଗଣନା auto_gc_hook_result *auto_gc_hook_result)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ଅଚଳ ଅକ୍ଷର *region = foreground ? "maintenance foreground" : "maintenance";
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ଯଦି (!fn)
		ଫେରନ୍ତୁ 0;
	ଯଦି (opts->auto_flag) {
		ଯଦି (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			ଫେରନ୍ତୁ 0;

		ଯଦି (!task->auto_condition || !task->auto_condition(cfg))
			ଫେରନ୍ତୁ 0;

		ଯଦି (*auto_gc_hook_result == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_result = run_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_RUN;
		ଯଦି (*auto_gc_hook_result == AUTO_GC_HOOK_SKIP)
			ଫେରନ୍ତୁ 0;
	}

	trace2_region_enter(region, task->name, repo);
	ଯଦି (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_run_tasks(ସଂରଚନା maintenance_run_opts *opts,
				 ସଂରଚନା gc_config *cfg)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ସଂରଚନା lock_file lk;
	ସଂରଚନା repository *r = the_repository;
	ଅକ୍ଷର *lock_path = xstrfmt("%s/maintenance", r->objects->source->path);
	ଗଣନା auto_gc_hook_result auto_gc_hook_result = AUTO_GC_HOOK_UNDECIDED;

	ଯଦି (repo_hold_lock_file_for_update(r, &lk, lock_path, LOCK_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		ଯଦି (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				lock_path);
		ମୁକ୍ତ(lock_path);
		ଫେରନ୍ତୁ 0;
	}
	ମୁକ୍ତ(lock_path);

	ପାଇଁ (size_t i = 0; i < opts->tasks_nr; i++)
		ଯଦି (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_result))
			result = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	ଯଦି (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	ପାଇଁ (size_t i = 0; i < opts->tasks_nr; i++)
		ଯଦି (maybe_run_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_result))
			result = 1;

	rollback_lock_file(&lk);
	ଫେରନ୍ତୁ result;
}

ଗଣନା maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

ସଂରଚନା maintenance_strategy {
	ସଂରଚନା {
		unsigned type;
		ଗଣନା schedule_priority schedule;
	} tasks[TASK_ଗଣନା];
};

ସ୍ଥିର ଅଚଳ ସଂରଚନା maintenance_strategy none_strategy = { 0 };

ସ୍ଥିର ଅଚଳ ସଂରଚନା maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.schedule = SCHEDULE_DAILY,
		},
	},
};

ସ୍ଥିର ଅଚଳ ସଂରଚନା maintenance_strategy incremental_strategy = {
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

ସ୍ଥିର ଅଚଳ ସଂରଚନା maintenance_strategy geometric_strategy = {
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

ସ୍ଥିର ସଂରଚନା maintenance_strategy parse_maintenance_strategy(ଅଚଳ ଅକ୍ଷର *name)
{
	ଯଦି (!strcasecmp(name, "incremental"))
		ଫେରନ୍ତୁ incremental_strategy;
	ଯଦି (!strcasecmp(name, "gc"))
		ଫେରନ୍ତୁ gc_strategy;
	ଯଦି (!strcasecmp(name, "geometric"))
		ଫେରନ୍ତୁ geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

ସ୍ଥିର ଶୂନ୍ୟ initialize_task_config(ସଂରଚନା maintenance_run_opts *opts,
				   ଅଚଳ ସଂରଚନା string_list *selected_tasks)
{
	ସଂରଚନା strbuf config_name = STRBUF_INIT;
	ସଂରଚନା maintenance_strategy strategy;
	ଗଣନା maintenance_type type;
	ଅଚଳ ଅକ୍ଷର *config_str;

	/*
	 * In case the user has asked us to run tasks explicitly we only use
	 * those specified tasks. Specifically, we do _not_ want to consult the
	 * config or maintenance strategy.
	 */
	ଯଦି (selected_tasks->nr) {
		ପାଇଁ (size_t i = 0; i < selected_tasks->nr; i++) {
			ଗଣନା maintenance_task_label label = (intptr_t)selected_tasks->ବସ୍ତୁଗୁଡ଼ିକ[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		ଫେରନ୍ତୁ;
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
	ଯଦି (opts->schedule) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} ଅନ୍ୟଥା {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	ଯଦି (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	ପାଇଁ (size_t i = 0; i < TASK_ଗଣନା; i++) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା config_ମୂଲ୍ୟ;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		ଯଦି (!repo_config_get_bool(the_repository, config_name.buf, &config_ମୂଲ୍ୟ))
			strategy.tasks[i].type = config_ମୂଲ୍ୟ ? type : 0;
		ଯଦି (!(strategy.tasks[i].type & type))
			ଜାରି;

		ଯଦି (opts->schedule) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			ଯଦି (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].schedule = parse_schedule(config_str);
			ଯଦି (strategy.tasks[i].schedule < opts->schedule)
				ଜାରି;
		}

		ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
		opts->tasks[opts->tasks_nr++] = i;
	}

	strbuf_release(&config_name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_option_parse(ଅଚଳ ସଂରଚନା option *opt,
			     ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା string_list *selected_tasks = opt->ମୂଲ୍ୟ;
	size_t i;

	BUG_ON_OPT_NEG(unset);

	ପାଇଁ (i = 0; i < TASK_ଗଣନା; i++)
		ଯଦି (!strcasecmp(tasks[i].name, arg))
			ଭାଙ୍ଗ;
	ଯଦି (i >= TASK_ଗଣନା) {
		error(_("'%s' is not a valid task"), arg);
		ଫେରନ୍ତୁ 1;
	}

	ଯଦି (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		ଫେରନ୍ତୁ 1;
	}

	string_list_append(selected_tasks, arg)->util = (ଶୂନ୍ୟ *)(intptr_t)i;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_run(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ସଂରଚନା string_list selected_tasks = STRING_LIST_INIT_DUP;
	ସଂରଚନା gc_config cfg = GC_CONFIG_INIT;
	ସଂରଚନା option builtin_maintenance_run_options[] = {
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
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

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

	ଯଦି (argc != 0)
		usage_with_options(builtin_maintenance_run_usage,
				   builtin_maintenance_run_options);

	ret = maintenance_run_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_run_opts_release(&opts);
	gc_config_release(&cfg);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଅକ୍ଷର *get_maintpath(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	ଫେରନ୍ତୁ strbuf_detach(&sb, ଖାଲି);
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_maintenance_register_usage[] = {
	"git maintenance register [--config-file <path>]",
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_register(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				ସଂରଚନା repository *repo UNUSED)
{
	ଅକ୍ଷର *config_file = ଖାଲି;
	ସଂରଚନା option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_END(),
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା found = 0;
	ଅଚଳ ଅକ୍ଷର *ଚାବି = "maintenance.repo";
	ଅକ୍ଷର *maintpath = get_maintpath();
	ସଂରଚନା string_list_item *item;
	ଅଚଳ ସଂରଚନା string_list *list;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_register_usage, 0);
	ଯଦି (argc)
		usage_with_options(builtin_maintenance_register_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_set(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	ଯଦି (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_set(the_repository, "maintenance.strategy", "incremental");

	ଯଦି (!repo_config_get_string_multi(the_repository, ଚାବି, &list)) {
		for_each_string_list_item(item, list) {
			ଯଦି (!strcmp(maintpath, item->string)) {
				found = 1;
				ଭାଙ୍ଗ;
			}
		}
	}

	ଯଦି (!found) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc;
		ଅକ୍ଷର *global_config_file = ଖାଲି;

		ଯଦି (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ଯଦି (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, ଖାଲି, 0);
		ମୁକ୍ତ(global_config_file);

		ଯଦି (rc)
			die(_("unable to add '%s' value of '%s'"),
			    ଚାବି, maintpath);
	}

	ମୁକ୍ତ(maintpath);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_unregister(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				  ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = 0;
	ଅକ୍ଷର *config_file = ଖାଲି;
	ସଂରଚନା option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_END(),
	};
	ଅଚଳ ଅକ୍ଷର *ଚାବି = "maintenance.repo";
	ଅକ୍ଷର *maintpath = get_maintpath();
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା found = 0;
	ସଂରଚନା string_list_item *item;
	ଅଚଳ ସଂରଚନା string_list *list;
	ସଂରଚନା config_set cs = { { 0 } };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_unregister_usage, 0);
	ଯଦି (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	ଯଦି (config_file) {
		git_configset_init(&cs);
		git_configset_add_file(&cs, config_file);
	}
	ଯଦି (!(config_file
	      ? git_configset_get_string_multi(&cs, ଚାବି, &list)
	      : repo_config_get_string_multi(the_repository, ଚାବି, &list))) {
		for_each_string_list_item(item, list) {
			ଯଦି (!strcmp(maintpath, item->string)) {
				found = 1;
				ଭାଙ୍ଗ;
			}
		}
	}

	ଯଦି (found) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc;
		ଅକ୍ଷର *global_config_file = ଖାଲି;

		ଯଦି (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		ଯଦି (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_set_multivar_in_file_gently(the_repository,
			config_file, ଚାବି, ଖାଲି, maintpath, ଖାଲି,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_ମୂଲ୍ୟ);
		ମୁକ୍ତ(global_config_file);

		ଯଦି (rc &&
		    (!force || rc == CONFIG_NOTHING_SET))
			die(_("unable to unset '%s' value of '%s'"),
			    ଚାବି, maintpath);
	} ଅନ୍ୟଥା ଯଦି (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	ମୁକ୍ତ(maintpath);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_frequency(ଗଣନା schedule_priority schedule)
{
	ବାଛ (schedule) {
	ସ୍ଥିତି SCHEDULE_HOURLY:
		ଫେରନ୍ତୁ "hourly";
	ସ୍ଥିତି SCHEDULE_DAILY:
		ଫେରନ୍ତୁ "daily";
	ସ୍ଥିତି SCHEDULE_WEEKLY:
		ଫେରନ୍ତୁ "weekly";
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid schedule %d", schedule);
	}
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_extra_config_parameters(ଶୂନ୍ୟ) {
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *result = ଖାଲି;
	ସଂରଚନା strbuf builder = STRBUF_INIT;

	ଯଦି (result)
		ଫେରନ୍ତୁ result;

	ପାଇଁ (ଅଚଳ ଅକ୍ଷର **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	result = strbuf_detach(&builder, ଖାଲି);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_extra_launchctl_strings(ଶୂନ୍ୟ) {
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *result = ଖାଲି;
	ସଂରଚନା strbuf builder = STRBUF_INIT;

	ଯଦି (result)
		ଫେରନ୍ତୁ result;

	ପାଇଁ (ଅଚଳ ଅକ୍ଷର **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	result = strbuf_detach(&builder, ଖାଲି);
	ଫେରନ୍ତୁ result;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_schedule_cmd(ଅଚଳ ଅକ୍ଷର *cmd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *is_available, ଅକ୍ଷର **out)
{
	ଅକ୍ଷର *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	ସଂରଚନା string_list_item *item;
	ସଂରଚନା string_list list = STRING_LIST_INIT_NODUP;

	ଯଦି (!testing) {
		ଯଦି (out)
			*out = xstrdup(cmd);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (is_available)
		*is_available = 0;

	string_list_split_in_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		ସଂରଚନା string_list pair = STRING_LIST_INIT_NODUP;

		ଯଦି (string_list_split_in_place(&pair, item->string, ":", 2) != 2)
			ଜାରି;

		ଯଦି (!strcmp(cmd, pair.ବସ୍ତୁଗୁଡ଼ିକ[0].string)) {
			ଯଦି (out)
				*out = xstrdup(pair.ବସ୍ତୁଗୁଡ଼ିକ[1].string);
			ଯଦି (is_available)
				*is_available = 1;
			string_list_clear(&pair, 0);
			ଯାଅ out;
		}

		string_list_clear(&pair, 0);
	}

	ଯଦି (out)
		*out = xstrdup(cmd);

out:
	string_list_clear(&list, 0);
	ମୁକ୍ତ(testing);
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_random_minute(ଶୂନ୍ୟ)
{
	/* Use a static value when under tests. */
	ଯଦି (getenv("GIT_TEST_MAINT_SCHEDULER"))
		ଫେରନ୍ତୁ 13;

	ଫେରନ୍ତୁ git_rand(0) % 60;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_launchctl_available(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_available;
	ଯଦି (get_schedule_cmd("launchctl", &is_available, ଖାଲି))
		ଫେରନ୍ତୁ is_available;

#ifdef __APPLE__
	ଫେରନ୍ତୁ 1;
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ 0;
#endif
}

ସ୍ଥିର ଅକ୍ଷର *launchctl_service_name(ଅଚଳ ଅକ୍ଷର *frequency)
{
	ସଂରଚନା strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	ଫେରନ୍ତୁ strbuf_detach(&label, ଖାଲି);
}

ସ୍ଥିର ଅକ୍ଷର *launchctl_service_filename(ଅଚଳ ଅକ୍ଷର *name)
{
	ଅକ୍ଷର *expanded;
	ସଂରଚନା strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_path(filename.buf, 1);
	ଯଦି (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	ଫେରନ୍ତୁ expanded;
}

ସ୍ଥିର ଅକ୍ଷର *launchctl_get_uid(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ xstrfmt("gui/%d", getuid());
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା launchctl_boot_plist(ପୂର୍ଣ୍ଣସଂଖ୍ୟା enable, ଅଚଳ ଅକ୍ଷର *filename)
{
	ଅକ୍ଷର *cmd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;
	ଅକ୍ଷର *uid = launchctl_get_uid();

	get_schedule_cmd("launchctl", ଖାଲି, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, enable ? "bootstrap" : "bootout", uid,
		     filename, ଖାଲି);

	child.no_stderr = 1;
	child.no_stdout = 1;

	ଯଦି (start_command(&child))
		die(_("failed to start launchctl"));

	result = finish_command(&child);

	ମୁକ୍ତ(cmd);
	ମୁକ୍ତ(uid);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା launchctl_remove_plist(ଗଣନା schedule_priority schedule)
{
	ଅଚଳ ଅକ୍ଷର *frequency = get_frequency(schedule);
	ଅକ୍ଷର *name = launchctl_service_name(frequency);
	ଅକ୍ଷର *filename = launchctl_service_filename(name);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = launchctl_boot_plist(0, filename);
	unlink(filename);
	ମୁକ୍ତ(filename);
	ମୁକ୍ତ(name);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା launchctl_remove_plists(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ launchctl_remove_plist(SCHEDULE_HOURLY) ||
	       launchctl_remove_plist(SCHEDULE_DAILY) ||
	       launchctl_remove_plist(SCHEDULE_WEEKLY);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା launchctl_list_contains_plist(ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *cmd)
{
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "list", name, ଖାଲି);

	child.no_stderr = 1;
	child.no_stdout = 1;

	ଯଦି (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	ଫେରନ୍ତୁ !finish_command(&child);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା launchctl_schedule_plist(ଅଚଳ ଅକ୍ଷର *exec_path, ଗଣନା schedule_priority schedule)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, fd;
	ଅଚଳ ଅକ୍ଷର *preamble, *repeat;
	ଅଚଳ ଅକ୍ଷର *frequency = get_frequency(schedule);
	ଅକ୍ଷର *name = launchctl_service_name(frequency);
	ଅକ୍ଷର *filename = launchctl_service_filename(name);
	ସଂରଚନା lock_file lk = LOCK_INIT;
	ସ୍ଥିର unsigned long lock_file_timeout_ms = ULONG_MAX;
	ସଂରଚନା strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	ସଂରଚନା stat st;
	ଅକ୍ଷର *cmd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା minute = get_random_minute();

	get_schedule_cmd("launchctl", ଖାଲି, &cmd);
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

	ବାଛ (schedule) {
	ସ୍ଥିତି SCHEDULE_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		ପାଇଁ (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି SCHEDULE_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		ପାଇଁ (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି SCHEDULE_WEEKLY:
		strbuf_addf(&plist,
			    "<dict>\n"
			    "<key>Weekday</key><integer>0</integer>\n"
			    "<key>Hour</key><integer>0</integer>\n"
			    "<key>Minute</key><integer>%d</integer>\n"
			    "</dict>\n",
			    minute);
		ଭାଙ୍ଗ;

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		/* unreachable */
		ଭାଙ୍ଗ;
	}
	strbuf_addstr(&plist, "</array>\n</dict>\n</plist>\n");

	ଯଦି (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	ଯଦି ((long)lock_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &lock_file_timeout_ms))
		lock_file_timeout_ms = 150;

	fd = hold_lock_file_for_update_timeout(&lk, filename, LOCK_DIE_ON_ERROR,
					       lock_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	ଯଦି (!stat(filename, &st) && st.st_size == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_contains_plist(name, cmd))
		rollback_lock_file(&lk);
	ଅନ୍ୟଥା {
		ଯଦି (write_in_full(fd, plist.buf, plist.len) < 0 ||
		    commit_lock_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		ଯଦି (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	ମୁକ୍ତ(filename);
	ମୁକ୍ତ(name);
	ମୁକ୍ତ(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା launchctl_add_plists(ଶୂନ୍ୟ)
{
	ଅଚଳ ଅକ୍ଷର *exec_path = git_exec_path();

	ଫେରନ୍ତୁ launchctl_schedule_plist(exec_path, SCHEDULE_HOURLY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_DAILY) ||
	       launchctl_schedule_plist(exec_path, SCHEDULE_WEEKLY);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା launchctl_update_schedule(ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_maintenance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd UNUSED)
{
	ଯଦି (run_maintenance)
		ଫେରନ୍ତୁ launchctl_add_plists();
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ launchctl_remove_plists();
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_schtasks_available(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_available;
	ଯଦି (get_schedule_cmd("schtasks", &is_available, ଖାଲି))
		ଫେରନ୍ତୁ is_available;

#ifdef GIT_WINDOWS_NATIVE
	ଫେରନ୍ତୁ 1;
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ 0;
#endif
}

ସ୍ଥିର ଅକ୍ଷର *schtasks_task_name(ଅଚଳ ଅକ୍ଷର *frequency)
{
	ସଂରଚନା strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	ଫେରନ୍ତୁ strbuf_detach(&label, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା schtasks_remove_task(ଗଣନା schedule_priority schedule)
{
	ଅକ୍ଷର *cmd;
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;
	ଅଚଳ ଅକ୍ଷର *frequency = get_frequency(schedule);
	ଅକ୍ଷର *name = schtasks_task_name(frequency);

	get_schedule_cmd("schtasks", ଖାଲି, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "/delete", "/tn", name, "/f", ଖାଲି);
	ମୁକ୍ତ(name);
	ମୁକ୍ତ(cmd);

	ଫେରନ୍ତୁ run_command(&child);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା schtasks_remove_tasks(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ schtasks_remove_task(SCHEDULE_HOURLY) ||
	       schtasks_remove_task(SCHEDULE_DAILY) ||
	       schtasks_remove_task(SCHEDULE_WEEKLY);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା schtasks_schedule_task(ଅଚଳ ଅକ୍ଷର *exec_path, ଗଣନା schedule_priority schedule)
{
	ଅକ୍ଷର *cmd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;
	ଅଚଳ ଅକ୍ଷର *xml;
	ସଂରଚନା tempfile *tfile;
	ଅଚଳ ଅକ୍ଷର *frequency = get_frequency(schedule);
	ଅକ୍ଷର *name = schtasks_task_name(frequency);
	ସଂରଚନା strbuf tfilename = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା minute = get_random_minute();

	get_schedule_cmd("schtasks", ଖାଲି, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	ଯଦି (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	ବାଛ (schedule) {
	ସ୍ଥିତି SCHEDULE_HOURLY:
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
		ଭାଙ୍ଗ;

	ସ୍ଥିତି SCHEDULE_DAILY:
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
		ଭାଙ୍ଗ;

	ସ୍ଥିତି SCHEDULE_WEEKLY:
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
		ଭାଙ୍ଗ;

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		ଭାଙ୍ଗ;
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
				  get_tempfile_path(tfile), ଖାଲି);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	ଯଦି (start_command(&child))
		die(_("failed to start schtasks"));
	result = finish_command(&child);

	delete_tempfile(&tfile);
	ମୁକ୍ତ(name);
	ମୁକ୍ତ(cmd);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା schtasks_schedule_tasks(ଶୂନ୍ୟ)
{
	ଅଚଳ ଅକ୍ଷର *exec_path = git_exec_path();

	ଫେରନ୍ତୁ schtasks_schedule_task(exec_path, SCHEDULE_HOURLY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_DAILY) ||
	       schtasks_schedule_task(exec_path, SCHEDULE_WEEKLY);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା schtasks_update_schedule(ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_maintenance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd UNUSED)
{
	ଯଦି (run_maintenance)
		ଫେରନ୍ତୁ schtasks_schedule_tasks();
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ schtasks_remove_tasks();
}

MAYBE_UNUSED
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_crontab_process(ଅଚଳ ଅକ୍ଷର *cmd)
{
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	strvec_split(&child.args, cmd);
	strvec_ଠେଲ(&child.args, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	ଯଦି (start_command(&child))
		ଫେରନ୍ତୁ 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_crontab_available(ଶୂନ୍ୟ)
{
	ଅକ୍ଷର *cmd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_available;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (get_schedule_cmd("crontab", &is_available, &cmd)) {
		ret = is_available;
		ଯାଅ out;
	}

#ifdef __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#ଅନ୍ୟଥା
	ret = check_crontab_process(cmd);
#endif

out:
	ମୁକ୍ତ(cmd);
	ଫେରନ୍ତୁ ret;
}

#define BEGIN_ଧାଡ଼ି "# BEGIN GIT MAINTENANCE SCHEDULE"
#define END_ଧାଡ଼ି "# END GIT MAINTENANCE SCHEDULE"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା crontab_update_schedule(ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_maintenance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
	ଅକ୍ଷର *cmd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_old_region = 0;
	ସଂରଚନା child_process crontab_list = CHILD_PROCESS_INIT;
	ସଂରଚନା child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_in;
	ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;
	ସଂରଚନା tempfile *tmpedit = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା minute = get_random_minute();

	get_schedule_cmd("crontab", ଖାଲି, &cmd);
	strvec_split(&crontab_list.args, cmd);
	strvec_ଠେଲ(&crontab_list.args, "-l");
	crontab_list.in = -1;
	crontab_list.out = dup(fd);
	crontab_list.git_cmd = 0;

	ଯଦି (start_command(&crontab_list)) {
		result = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		ଯାଅ out;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	ଯଦି (!tmpedit) {
		result = error(_("failed to create crontab temporary file"));
		ଯାଅ out;
	}
	cron_in = fdopen_tempfile(tmpedit, "w");
	ଯଦି (!cron_in) {
		result = error(_("failed to open temporary file"));
		ଯାଅ out;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	ଯେତେବେଳେ (!strbuf_getline_lf(&ଧାଡ଼ି, cron_list)) {
		ଯଦି (!in_old_region && !strcmp(ଧାଡ଼ି.buf, BEGIN_ଧାଡ଼ି))
			in_old_region = 1;
		ଅନ୍ୟଥା ଯଦି (in_old_region && !strcmp(ଧାଡ଼ି.buf, END_ଧାଡ଼ି))
			in_old_region = 0;
		ଅନ୍ୟଥା ଯଦି (!in_old_region)
			fprintf(cron_in, "%s\n", ଧାଡ଼ି.buf);
	}
	strbuf_release(&ଧାଡ଼ି);

	ଯଦି (run_maintenance) {
		ସଂରଚନା strbuf ଧାଡ଼ି_format = STRBUF_INIT;
		ଅଚଳ ଅକ୍ଷର *exec_path = git_exec_path();

		fprintf(cron_in, "%s\n", BEGIN_ଧାଡ଼ି);
		fprintf(cron_in,
			"# The following schedule was created by Git\n");
		fprintf(cron_in, "# Any edits made in this region might be\n");
		fprintf(cron_in,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&ଧାଡ଼ି_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_path, exec_path, get_extra_config_parameters());
		fprintf(cron_in, ଧାଡ଼ି_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_in, ଧାଡ଼ି_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_in, ଧାଡ଼ି_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&ଧାଡ଼ି_format);

		fprintf(cron_in, "\n%s\n", END_ଧାଡ଼ି);
	}

	fflush(cron_in);

	strvec_split(&crontab_edit.args, cmd);
	strvec_ଠେଲ(&crontab_edit.args, get_tempfile_path(tmpedit));
	crontab_edit.git_cmd = 0;

	ଯଦି (start_command(&crontab_edit)) {
		result = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		ଯାଅ out;
	}

	ଯଦି (finish_command(&crontab_edit))
		result = error(_("'crontab' died"));
	ଅନ୍ୟଥା
		fclose(cron_list);

out:
	delete_tempfile(&tmpedit);
	ମୁକ୍ତ(cmd);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା real_is_systemd_timer_available(ଶୂନ୍ୟ)
{
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.args, "systemctl", "--user", "list-timers", ଖାଲି);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	ଯଦି (start_command(&child))
		ଫେରନ୍ତୁ 0;
	ଯଦି (finish_command(&child))
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_systemd_timer_available(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_available;

	ଯଦି (get_schedule_cmd("systemctl", &is_available, ଖାଲି))
		ଫେରନ୍ତୁ is_available;

	ଫେରନ୍ତୁ real_is_systemd_timer_available();
}

ସ୍ଥିର ଅକ୍ଷର *xdg_config_home_systemd(ଅଚଳ ଅକ୍ଷର *filename)
{
	ଫେରନ୍ତୁ xdg_config_home_for("systemd/user", filename);
}

#define SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା systemd_timer_delete_timer_file(ଗଣନା schedule_priority priority)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ଅଚଳ ଅକ୍ଷର *frequency = get_frequency(priority);
	ଅକ୍ଷର *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	ଅକ୍ଷର *filename = xdg_config_home_systemd(local_timer_name);

	ଯଦି (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	ମୁକ୍ତ(filename);
	ମୁକ୍ତ(local_timer_name);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା systemd_timer_delete_service_template(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ଅକ୍ଷର *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	ଅକ୍ଷର *filename = xdg_config_home_systemd(local_service_name);
	ଯଦି (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	ମୁକ୍ତ(filename);
	ମୁକ୍ତ(local_service_name);
	ଫେରନ୍ତୁ ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା systemd_timer_write_timer_file(ଗଣନା schedule_priority schedule,
					  ପୂର୍ଣ୍ଣସଂଖ୍ୟା minute)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = -1;
	ଅକ୍ଷର *filename;
	FILE *file;
	ଅଚଳ ଅକ୍ଷର *unit;
	ଅକ୍ଷର *schedule_pattern = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *frequency = get_frequency(schedule);
	ଅକ୍ଷର *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	ଯଦି (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		ଯାଅ error;
	}
	file = fopen_or_warn(filename, "w");
	ଯଦି (!file)
		ଯାଅ error;

	ବାଛ (schedule) {
	ସ୍ଥିତି SCHEDULE_HOURLY:
		schedule_pattern = xstrfmt("*-*-* 1..23:%02d:00", minute);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି SCHEDULE_DAILY:
		schedule_pattern = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି SCHEDULE_WEEKLY:
		schedule_pattern = xstrfmt("Mon 0:%02d:00", minute);
		ଭାଙ୍ଗ;

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
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
	ଯଦି (fprintf(file, unit, schedule_pattern) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		ଯାଅ error;
	}
	ଯଦି (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		ଯାଅ error;
	}

	res = 0;

error:
	ମୁକ୍ତ(schedule_pattern);
	ମୁକ୍ତ(local_timer_name);
	ମୁକ୍ତ(filename);
	ଫେରନ୍ତୁ res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା systemd_timer_write_service_template(ଅଚଳ ଅକ୍ଷର *exec_path)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = -1;
	ଅକ୍ଷର *filename;
	FILE *file;
	ଅଚଳ ଅକ୍ଷର *unit;
	ଅକ୍ଷର *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	ଯଦି (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		ଯାଅ error;
	}
	file = fopen_or_warn(filename, "w");
	ଯଦି (!file)
		ଯାଅ error;

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
	ଯଦି (fprintf(file, unit, exec_path, exec_path, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		ଯାଅ error;
	}
	ଯଦି (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		ଯାଅ error;
	}

	res = 0;

error:
	ମୁକ୍ତ(local_service_name);
	ମୁକ୍ତ(filename);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା systemd_timer_enable_unit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା enable,
				     ଗଣନା schedule_priority schedule,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା minute)
{
	ଅକ୍ଷର *cmd = ଖାଲି;
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;
	ଅଚଳ ଅକ୍ଷର *frequency = get_frequency(schedule);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	/*
	 * Disabling the systemd unit while it is already disabled makes
	 * systemctl print an error.
	 * Let's ignore it since it means we already are in the expected state:
	 * the unit is disabled.
	 *
	 * On the other hand, enabling a systemd unit which is already enabled
	 * produces no error.
	 */
	ଯଦି (!enable) {
		child.no_stderr = 1;
	} ଅନ୍ୟଥା ଯଦି (systemd_timer_write_timer_file(schedule, minute)) {
		ret = -1;
		ଯାଅ out;
	}

	get_schedule_cmd("systemctl", ଖାଲି, &cmd);
	strvec_split(&child.args, cmd);
	strvec_pushl(&child.args, "--user", enable ? "enable" : "disable",
		     "--now", ଖାଲି);
	strvec_pushf(&child.args, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	ଯଦି (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		ଯାଅ out;
	}

	ଯଦି (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		ଯଦି (enable) {
			ret = error(_("failed to run systemctl"));
			ଯାଅ out;
		}
	}

	ret = 0;

out:
	ମୁକ୍ତ(cmd);
	ଫେରନ୍ତୁ ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
ସ୍ଥିର ଶୂନ୍ୟ systemd_timer_delete_stale_timer_templates(ଶୂନ୍ୟ)
{
	ଅକ୍ଷର *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	ଅକ୍ଷର *filename = xdg_config_home_systemd(timer_template_name);

	ଯଦି (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	ମୁକ୍ତ(filename);
	ମୁକ୍ତ(timer_template_name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା systemd_timer_delete_unit_files(ଶୂନ୍ୟ)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	ଫେରନ୍ତୁ systemd_timer_delete_timer_file(SCHEDULE_HOURLY) |
	       systemd_timer_delete_timer_file(SCHEDULE_DAILY) |
	       systemd_timer_delete_timer_file(SCHEDULE_WEEKLY) |
	       systemd_timer_delete_service_template();
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା systemd_timer_delete_units(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	ଫେରନ୍ତୁ systemd_timer_enable_unit(0, SCHEDULE_HOURLY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_DAILY, minute) |
	       systemd_timer_enable_unit(0, SCHEDULE_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା systemd_timer_setup_units(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା minute = get_random_minute();
	ଅଚଳ ଅକ୍ଷର *exec_path = git_exec_path();

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = systemd_timer_write_service_template(exec_path) ||
		  systemd_timer_enable_unit(1, SCHEDULE_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_DAILY, minute) ||
		  systemd_timer_enable_unit(1, SCHEDULE_WEEKLY, minute);

	ଯଦି (ret)
		systemd_timer_delete_units();
	ଅନ୍ୟଥା
		systemd_timer_delete_stale_timer_templates();

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା systemd_timer_update_schedule(ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_maintenance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd UNUSED)
{
	ଯଦି (run_maintenance)
		ଫେରନ୍ତୁ systemd_timer_setup_units();
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ systemd_timer_delete_units();
}

ଗଣନା scheduler {
	SCHEDULER_INVALID = -1,
	SCHEDULER_AUTO,
	SCHEDULER_CRON,
	SCHEDULER_SYSTEMD,
	SCHEDULER_LAUNCHCTL,
	SCHEDULER_SCHTASKS,
};

ସ୍ଥିର ଅଚଳ ସଂରଚନା {
	ଅଚଳ ଅକ୍ଷର *name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*is_available)(ଶୂନ୍ୟ);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*update_schedule)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_maintenance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd);
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

ସ୍ଥିର ଗଣନା scheduler parse_scheduler(ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ଯଦି (!ମୂଲ୍ୟ)
		ଫେରନ୍ତୁ SCHEDULER_INVALID;
	ଅନ୍ୟଥା ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "auto"))
		ଫେରନ୍ତୁ SCHEDULER_AUTO;
	ଅନ୍ୟଥା ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "cron") || !strcasecmp(ମୂଲ୍ୟ, "crontab"))
		ଫେରନ୍ତୁ SCHEDULER_CRON;
	ଅନ୍ୟଥା ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "systemd") ||
		 !strcasecmp(ମୂଲ୍ୟ, "systemd-timer"))
		ଫେରନ୍ତୁ SCHEDULER_SYSTEMD;
	ଅନ୍ୟଥା ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "launchctl"))
		ଫେରନ୍ତୁ SCHEDULER_LAUNCHCTL;
	ଅନ୍ୟଥା ଯଦି (!strcasecmp(ମୂଲ୍ୟ, "schtasks"))
		ଫେରନ୍ତୁ SCHEDULER_SCHTASKS;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ SCHEDULER_INVALID;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_opt_scheduler(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଗଣନା scheduler *scheduler = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	ଯଦି (*scheduler == SCHEDULER_INVALID)
		ଫେରନ୍ତୁ error(_("unrecognized --scheduler argument '%s'"), arg);
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା maintenance_start_opts {
	ଗଣନା scheduler scheduler;
};

ସ୍ଥିର ଗଣନା scheduler resolve_scheduler(ଗଣନା scheduler scheduler)
{
	ଯଦି (scheduler != SCHEDULER_AUTO)
		ଫେରନ୍ତୁ scheduler;

#ଯଦି defined(__APPLE__)
	ଫେରନ୍ତୁ SCHEDULER_LAUNCHCTL;

#ନଚେତଯଦି defined(GIT_WINDOWS_NATIVE)
	ଫେରନ୍ତୁ SCHEDULER_SCHTASKS;

#ନଚେତଯଦି defined(__linux__)
	ଯଦି (is_systemd_timer_available())
		ଫେରନ୍ତୁ SCHEDULER_SYSTEMD;
	ଅନ୍ୟଥା ଯଦି (is_crontab_available())
		ଫେରନ୍ତୁ SCHEDULER_CRON;
	ଅନ୍ୟଥା
		die(_("neither systemd timers nor crontab are available"));

#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ SCHEDULER_CRON;
#endif
}

ସ୍ଥିର ଶୂନ୍ୟ validate_scheduler(ଗଣନା scheduler scheduler)
{
	ଯଦି (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	ଯଦି (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	ଯଦି (!scheduler_fn[scheduler].is_available())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_background_schedule(ଅଚଳ ସଂରଚନା maintenance_start_opts *opts,
				      ପୂର୍ଣ୍ଣସଂଖ୍ୟା enable)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ସଂରଚନା lock_file lk;
	ଅକ୍ଷର *lock_path = xstrfmt("%s/schedule", the_repository->objects->source->path);

	ଯଦି (hold_lock_file_for_update(&lk, lock_path, LOCK_NO_DEREF) < 0) {
		ଯଦି (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_path(lock_path), strerror(errno));
		ଅନ୍ୟଥା
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		ମୁକ୍ତ(lock_path);
		ଫେରନ୍ତୁ -1;
	}

	ପାଇଁ (i = 1; i < ARRAY_SIZE(scheduler_fn); i++) {
		ଯଦି (enable && opts->scheduler == i)
			ଜାରି;
		ଯଦି (!scheduler_fn[i].is_available())
			ଜାରି;
		scheduler_fn[i].update_schedule(0, get_lock_file_fd(&lk));
	}

	ଯଦି (enable)
		result = scheduler_fn[opts->scheduler].update_schedule(
			1, get_lock_file_fd(&lk));

	rollback_lock_file(&lk);

	ମୁକ୍ତ(lock_path);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_start(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			     ସଂରଚନା repository *repo)
{
	ସଂରଚନା maintenance_start_opts opts = { 0 };
	ସଂରଚନା option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର *register_args[] = { "register", ଖାଲି };

	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_start_usage, 0);
	ଯଦି (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	ଯଦି (update_background_schedule(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	ଯଦି (maintenance_register(ARRAY_SIZE(register_args)-1, register_args, ଖାଲି, repo))
		warning(_("failed to add repo to global config"));
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_stop(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			    ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option options[] = {
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_maintenance_stop_usage, 0);
	ଯଦି (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	ଫେରନ୍ତୁ update_background_schedule(ଖାଲି, 0);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maintenance_is_needed(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				 ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା maintenance_run_opts opts = MAINTENANCE_RUN_OPTS_INIT;
	ସଂରଚନା string_list selected_tasks = STRING_LIST_INIT_DUP;
	ସଂରଚନା gc_config cfg = GC_CONFIG_INIT;
	ସଂରଚନା option options[] = {
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
	ଯଦି (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	ଯଦି (opts.auto_flag) {
		ପାଇଁ (size_t i = 0; i < opts.tasks_nr; i++) {
			ଯଦି (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				ଭାଙ୍ଗ;
			}
		}
	} ଅନ୍ୟଥା {
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

	ଯଦି (is_needed)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	ଖାଲି,
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_maintenance(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		    ଅଚଳ ଅକ୍ଷର **argv,
		    ଅଚଳ ଅକ୍ଷର *prefix,
		    ସଂରଚନା repository *repo)
{
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option builtin_maintenance_options[] = {
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
	ଫେରନ୍ତୁ fn(argc, argv, prefix, repo);
}
