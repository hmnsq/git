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

#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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

#परिभाषय FAILED_सततम् "failed to run %s"

स्थिर ध्रुव वर्ण * ध्रुव builtin_gc_usage[] = {
	N_("git gc [<options>]"),
	रिक्त
};

स्थिर timestamp_t gc_log_expire_time;
स्थिर संरचना tempfile *pidfile;
स्थिर संरचना तालकम्_file log_तालकम्;
स्थिर संरचना string_list pack_garbage = STRING_LIST_INIT_DUP;

स्थिर शून्य clean_pack_garbage(शून्य)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < pack_garbage.nr; i++)
		unlink_or_warn(pack_garbage.वस्तूनि[i].string);
	string_list_clear(&pack_garbage, 0);
}

स्थिर शून्य report_pack_garbage(अचिह्नित आलोकितम्_द्वयङ्काः, ध्रुव वर्ण *मार्गः)
{
	यदि (आलोकितम्_द्वयङ्काः == PACKDIR_FILE_IDX)
		string_list_append(&pack_garbage, मार्गः);
}

स्थिर शून्य process_log_file(शून्य)
{
	संरचना stat st;
	यदि (fstat(get_तालकम्_file_fd(&log_तालकम्), &st)) {
		/*
		 * Perhaps there was an i/o error or another
		 * unlikely situation.  Try to make a note of
		 * this in gc.log along with any existing
		 * messages.
		 */
		पूर्णाङ्क saved_errno = errno;
		fprintf(stderr, _("Failed to fstat %s: %s"),
			get_तालकम्_file_मार्गः(&log_तालकम्),
			strerror(saved_errno));
		fflush(stderr);
		commit_तालकम्_file(&log_तालकम्);
		errno = saved_errno;
	} अन्यथा यदि (st.st_आकार) {
		/* There was some error recorded in the lock file */
		commit_तालकम्_file(&log_तालकम्);
	} अन्यथा {
		वर्ण *मार्गः = repo_git_मार्गः(the_repository, "gc.log");
		/* No error, clean up any old gc.log */
		unlink(मार्गः);
		rollback_तालकम्_file(&log_तालकम्);
		मुक्त(मार्गः);
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

	यदि (!repo_config_get_मानम्(the_repository, var, &मानम्) && मानम्) {
		यदि (parse_expiry_date(मानम्, &expire))
			die(_("failed to parse '%s' value '%s'"), var, मानम्);
		प्रत्यागम expire == 0;
	}
	प्रत्यागम 0;
}

संरचना gc_config {
	पूर्णाङ्क pack_refs;
	पूर्णाङ्क prune_reflogs;
	पूर्णाङ्क cruft_packs;
	अचिह्नित दीर्घ max_cruft_आकार;
	पूर्णाङ्क aggressive_गभीरता;
	पूर्णाङ्क aggressive_गवाक्षः;
	पूर्णाङ्क detach_auto;
	वर्ण *gc_log_expire;
	वर्ण *prune_expire;
	वर्ण *prune_worktrees_expire;
	वर्ण *repack_expire_गन्तव्यम्;
};

#परिभाषय GC_CONFIG_INIT { \
	.pack_refs = 1, \
	.prune_reflogs = 1, \
	.cruft_packs = 1, \
	.aggressive_गभीरता = 50, \
	.aggressive_गवाक्षः = 250, \
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

	यदि (!repo_config_get_मानम्(the_repository, "gc.packrefs", &मानम्)) {
		यदि (मानम् && !वाक्यं_तुलय(मानम्, "notbare"))
			cfg->pack_refs = -1;
		अन्यथा
			cfg->pack_refs = git_config_bool("gc.packrefs", मानम्);
	}

	यदि (gc_config_is_timestamp_never("gc.reflogexpire") &&
	    gc_config_is_timestamp_never("gc.reflogexpireunreachable"))
		cfg->prune_reflogs = 0;

	repo_config_get_int(the_repository, "gc.aggressivewindow", &cfg->aggressive_गवाक्षः);
	repo_config_get_int(the_repository, "gc.aggressivedepth", &cfg->aggressive_गभीरता);
	repo_config_get_bool(the_repository, "gc.autodetach", &cfg->detach_auto);
	repo_config_get_bool(the_repository, "gc.cruftpacks", &cfg->cruft_packs);
	repo_config_get_ulong(the_repository, "gc.maxcruftsize", &cfg->max_cruft_आकार);

	यदि (!repo_config_get_expiry(the_repository, "gc.pruneexpire", &owned)) {
		मुक्त(cfg->prune_expire);
		cfg->prune_expire = owned;
	}

	यदि (!repo_config_get_expiry(the_repository, "gc.worktreepruneexpire", &owned)) {
		मुक्त(cfg->prune_worktrees_expire);
		cfg->prune_worktrees_expire = owned;
	}

	यदि (!repo_config_get_expiry(the_repository, "gc.logexpiry", &owned)) {
		मुक्त(cfg->gc_log_expire);
		cfg->gc_log_expire = owned;
	}

	repo_config(the_repository, git_default_config, रिक्त);
}

गणना कालसारणी_प्राथमिकता {
	कालसारणी_NONE = 0,
	कालसारणी_WEEKLY = 1,
	कालसारणी_DAILY = 2,
	कालसारणी_HOURLY = 3,
};

स्थिर गणना कालसारणी_प्राथमिकता parse_कालसारणी(ध्रुव वर्ण *मानम्)
{
	यदि (!मानम्)
		प्रत्यागम कालसारणी_NONE;
	यदि (!strcasecmp(मानम्, "hourly"))
		प्रत्यागम कालसारणी_HOURLY;
	यदि (!strcasecmp(मानम्, "daily"))
		प्रत्यागम कालसारणी_DAILY;
	यदि (!strcasecmp(मानम्, "weekly"))
		प्रत्यागम कालसारणी_WEEKLY;
	प्रत्यागम कालसारणी_NONE;
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

संरचना maintenance_सततम्_opts {
	गणना maintenance_task_label *tasks;
	आकार_t tasks_nr, tasks_alloc;
	पूर्णाङ्क auto_flag;
	पूर्णाङ्क detach;
	पूर्णाङ्क quiet;
	गणना कालसारणी_प्राथमिकता कालसारणी;
};
#परिभाषय MAINTENANCE_सततम्_OPTS_INIT { \
	.detach = -1, \
}

स्थिर शून्य maintenance_सततम्_opts_release(संरचना maintenance_सततम्_opts *opts)
{
	मुक्त(opts->tasks);
}

स्थिर पूर्णाङ्क pack_refs_condition(अप्रयुक्तम् संरचना gc_config *cfg)
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

	यदि (refs_optimize_required(get_main_ref_store(the_repository),
				   &optimize_opts, &required))
		प्रत्यागम 0;

	clear_ref_exclusions(&excludes);
	string_list_clear(&included_refs, 0);

	प्रत्यागम required;
}

स्थिर पूर्णाङ्क maintenance_task_pack_refs(संरचना maintenance_सततम्_opts *opts,
				      अप्रयुक्तम् संरचना gc_config *cfg)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.तर्काः, "pack-refs", "--all", "--prune", रिक्त);
	यदि (opts->auto_flag)
		strvec_प्रक्षिप(&cmd.तर्काः, "--auto");

	प्रत्यागम सततम्_command(&cmd);
}

संरचना गणनम्_reflog_entries_दत्त {
	संरचना expire_reflog_policy_cb policy;
	आकार_t गणनम्;
	आकार_t सीमा;
};

स्थिर पूर्णाङ्क गणनम्_reflog_entries(ध्रुव वर्ण *refname अप्रयुक्तम्,
				संरचना object_id *old_oid, संरचना object_id *new_oid,
				ध्रुव वर्ण *committer, timestamp_t timestamp,
				पूर्णाङ्क tz, ध्रुव वर्ण *सन्देशः, शून्य *cb_दत्त)
{
	संरचना गणनम्_reflog_entries_दत्त *दत्त = cb_दत्त;
	यदि (should_expire_reflog_ent(old_oid, new_oid, committer, timestamp, tz, सन्देशः, &दत्त->policy))
		दत्त->गणनम्++;
	प्रत्यागम दत्त->गणनम् >= दत्त->सीमा;
}

स्थिर पूर्णाङ्क reflog_expire_condition(संरचना gc_config *cfg अप्रयुक्तम्)
{
	timestamp_t now = time(रिक्त);
	संरचना गणनम्_reflog_entries_दत्त दत्त = {
		.policy = {
			.opts = REFLOG_EXPIRE_OPTIONS_INIT(now),
		},
	};
	पूर्णाङ्क सीमा = 100;

	repo_config_get_int(the_repository, "maintenance.reflog-expire.auto", &सीमा);
	यदि (!सीमा)
		प्रत्यागम 0;
	यदि (सीमा < 0)
		प्रत्यागम 1;
	दत्त.सीमा = सीमा;

	repo_config(the_repository, reflog_expire_config, &दत्त.policy.opts);

	reflog_expire_options_समुच्चयः_refname(&दत्त.policy.opts, "HEAD");
	refs_for_each_reflog_ent(get_main_ref_store(the_repository), "HEAD",
				 गणनम्_reflog_entries, &दत्त);

	reflog_expiry_cleanup(&दत्त.policy);
	reflog_clear_expire_config(&दत्त.policy.opts);
	प्रत्यागम दत्त.गणनम् >= दत्त.सीमा;
}

स्थिर पूर्णाङ्क maintenance_task_reflog_expire(संरचना maintenance_सततम्_opts *opts अप्रयुक्तम्,
					  संरचना gc_config *cfg अप्रयुक्तम्)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	cmd.git_cmd = 1;
	strvec_pushl(&cmd.तर्काः, "reflog", "expire", "--all", रिक्त);
	प्रत्यागम सततम्_command(&cmd);
}

स्थिर पूर्णाङ्क maintenance_task_worktree_prune(संरचना maintenance_सततम्_opts *opts अप्रयुक्तम्,
					   संरचना gc_config *cfg)
{
	संरचना child_process prune_worktrees_cmd = CHILD_PROCESS_INIT;

	prune_worktrees_cmd.git_cmd = 1;
	strvec_pushl(&prune_worktrees_cmd.तर्काः, "worktree", "prune", "--expire", रिक्त);
	strvec_प्रक्षिप(&prune_worktrees_cmd.तर्काः, cfg->prune_worktrees_expire);

	प्रत्यागम सततम्_command(&prune_worktrees_cmd);
}

स्थिर पूर्णाङ्क worktree_prune_condition(संरचना gc_config *cfg)
{
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क should_prune = 0, सीमा = 1;
	timestamp_t expiry_date;
	संरचना dirent *d;
	DIR *dir = रिक्त;

	repo_config_get_int(the_repository, "maintenance.worktree-prune.auto", &सीमा);
	यदि (सीमा <= 0) {
		should_prune = सीमा < 0;
		गच्छ बहिर्गमनम्;
	}

	यदि (parse_expiry_date(cfg->prune_worktrees_expire, &expiry_date))
		गच्छ बहिर्गमनम्;

	dir = opendir(repo_git_मार्गः_replace(the_repository, &buf, "worktrees"));
	यदि (!dir)
		गच्छ बहिर्गमनम्;

	यावत् (सीमा && (d = readdir_skip_dot_and_dotdot(dir))) {
		वर्ण *wtpath;
		strbuf_reset(&buf);
		यदि (should_prune_worktree(the_repository, d->d_name, &buf, &wtpath, expiry_date))
			सीमा--;
		मुक्त(wtpath);
	}

	should_prune = !सीमा;

बहिर्गमनम्:
	यदि (dir)
		closedir(dir);
	strbuf_release(&buf);
	प्रत्यागम should_prune;
}

स्थिर पूर्णाङ्क maintenance_task_rerere_gc(संरचना maintenance_सततम्_opts *opts अप्रयुक्तम्,
				      संरचना gc_config *cfg अप्रयुक्तम्)
{
	संरचना child_process rerere_cmd = CHILD_PROCESS_INIT;
	rerere_cmd.git_cmd = 1;
	strvec_pushl(&rerere_cmd.तर्काः, "rerere", "gc", रिक्त);
	प्रत्यागम सततम्_command(&rerere_cmd);
}

स्थिर पूर्णाङ्क rerere_gc_condition(संरचना gc_config *cfg अप्रयुक्तम्)
{
	पूर्णाङ्क सीमा = 512;

	repo_config_get_int(the_repository, "maintenance.rerere-gc.auto", &सीमा);
	यदि (!सीमा)
		प्रत्यागम 0; /* never prune */
	यदि (सीमा < 0)
		प्रत्यागम 1; /* always prune */

	प्रत्यागम rerere_gc_needed(the_repository, (आकार_t)सीमा);
}

#परिभाषय OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive) \
	.prune_expire = (cfg)->prune_expire, \
	.expire_गन्तव्यम् = (cfg)->repack_expire_गन्तव्यम्, \
	.cruft_packs = (cfg)->cruft_packs, \
	.max_cruft_आकार = (cfg)->max_cruft_आकार, \
	.गवाक्षः = (aggressive) ? (cfg)->aggressive_गवाक्षः : 0, \
	.गभीरता = (aggressive) ? (cfg)->aggressive_गभीरता : 0

/* return NULL on success, else hostname running the gc */
स्थिर ध्रुव वर्ण *तालकम्_repo_for_gc(पूर्णाङ्क force, pid_t* ret_pid)
{
	संरचना तालकम्_file तालकम् = तालकम्_INIT;
	वर्ण my_host[HOST_NAME_MAX + 1];
	संरचना strbuf sb = STRBUF_INIT;
	संरचना stat st;
	uintmax_t pid;
	FILE *fp;
	पूर्णाङ्क fd;
	वर्ण *pidfile_मार्गः;

	यदि (is_tempfile_सक्रिय(pidfile))
		/* already locked */
		प्रत्यागम रिक्त;

	यदि (xgethostname(my_host, परिमाण(my_host)))
		xsnprintf(my_host, परिमाण(my_host), "unknown");

	pidfile_मार्गः = repo_git_मार्गः(the_repository, "gc.pid");
	fd = hold_तालकम्_file_for_update(&तालकम्, pidfile_मार्गः,
				       तालकम्_DIE_ON_ERROR);
	यदि (!force) {
		स्थिर वर्ण locking_host[HOST_NAME_MAX + 1];
		स्थिर वर्ण *अवलोकनम्_fmt;
		पूर्णाङ्क should_exit;

		यदि (!अवलोकनम्_fmt)
			अवलोकनम्_fmt = xstrfmt("%s %%%ds", "%"SCNuMAX, HOST_NAME_MAX);
		fp = fopen(pidfile_मार्गः, "r");
		स्मृतिं_पूरय(locking_host, 0, परिमाण(locking_host));
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
			fscanf(fp, अवलोकनम्_fmt, &pid, locking_host) == 2 &&
			/* be gentle to concurrent "gc" on remote hosts */
			(वाक्यं_तुलय(locking_host, my_host) || !kill(pid, 0) || errno == EPERM);
		यदि (fp)
			fclose(fp);
		यदि (should_exit) {
			यदि (fd >= 0)
				rollback_तालकम्_file(&तालकम्);
			*ret_pid = pid;
			मुक्त(pidfile_मार्गः);
			प्रत्यागम locking_host;
		}
	}

	strbuf_addf(&sb, "%"PRIuMAX" %s",
		    (uintmax_t) getpid(), my_host);
	write_अन्तर्_पूर्णम्(fd, sb.buf, sb.len);
	strbuf_release(&sb);
	commit_तालकम्_file(&तालकम्);
	pidfile = पञ्जिकाकोशः_tempfile(pidfile_मार्गः);
	मुक्त(pidfile_मार्गः);
	प्रत्यागम रिक्त;
}

/*
 * Returns 0 if there was no previous error and gc can proceed, 1 if
 * gc should not proceed due to an error in the last run. Prints a
 * message and returns with a non-[01] status code if an error occurred
 * while reading gc.log
 */
स्थिर पूर्णाङ्क report_चरमः_gc_error(शून्य)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क ret = 0;
	ssize_t len;
	संरचना stat st;
	वर्ण *gc_log_मार्गः = repo_git_मार्गः(the_repository, "gc.log");

	यदि (stat(gc_log_मार्गः, &st)) {
		यदि (errno == ENOENT)
			गच्छ समाप्तम्;

		ret = die_सन्देशः_errno(_("cannot stat '%s'"), gc_log_मार्गः);
		गच्छ समाप्तम्;
	}

	यदि (st.st_mtime < gc_log_expire_time)
		गच्छ समाप्तम्;

	len = strbuf_read_file(&sb, gc_log_मार्गः, 0);
	यदि (len < 0)
		ret = die_सन्देशः_errno(_("cannot read '%s'"), gc_log_मार्गः);
	अन्यथा यदि (len > 0) {
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
			    gc_log_मार्गः, sb.buf);
		ret = 1;
	}
	strbuf_release(&sb);
समाप्तम्:
	मुक्त(gc_log_मार्गः);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क gc_foreground_tasks(संरचना maintenance_सततम्_opts *opts,
			       संरचना gc_config *cfg)
{
	यदि (cfg->pack_refs && maintenance_task_pack_refs(opts, cfg))
		प्रत्यागम error(FAILED_सततम्, "pack-refs");
	यदि (cfg->prune_reflogs && maintenance_task_reflog_expire(opts, cfg))
		प्रत्यागम error(FAILED_सततम्, "reflog");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क maintenance_task_odb(संरचना maintenance_सततम्_opts *opts,
				संरचना gc_config *cfg,
				पूर्णाङ्क keep_largest_pack,
				पूर्णाङ्क aggressive)
{
	संरचना odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		.keep_largest_pack = keep_largest_pack,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, aggressive),
	};

	यदि (opts->auto_flag)
		odb_opts.flags |= ODB_OPTIMIZE_AUTO;
	यदि (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;
	यदि (aggressive)
		odb_opts.flags |= ODB_OPTIMIZE_NO_REUSE_DELTAS;

	प्रत्यागम odb_optimize(the_repository->objects, &odb_opts);
}

पूर्णाङ्क cmd_gc(पूर्णाङ्क argc,
	   ध्रुव वर्ण **argv,
	   ध्रुव वर्ण *उपसर्गः,
	   संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क aggressive = 0;
	पूर्णाङ्क force = 0;
	ध्रुव वर्ण *name;
	pid_t pid;
	पूर्णाङ्क daemonized = 0;
	पूर्णाङ्क keep_largest_pack = -1;
	पूर्णाङ्क skip_foreground_tasks = 0;
	timestamp_t dummy;
	संरचना maintenance_सततम्_opts opts = MAINTENANCE_सततम्_OPTS_INIT;
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
		OPT_STRING(0, "expire-to", &cfg.repack_expire_गन्तव्यम्, N_("dir"),
			   N_("pack prefix to store a pack containing pruned objects")),
		OPT_HIDDEN_BOOL(0, "skip-foreground-tasks", &skip_foreground_tasks,
			   N_("skip maintenance tasks typically done in the foreground")),
		OPT_अन्तः()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_gc_usage, builtin_gc_options);

	gc_config(&cfg);

	यदि (parse_expiry_date(cfg.gc_log_expire, &gc_log_expire_time))
		die(_("failed to parse gc.logExpiry value %s"), cfg.gc_log_expire);

	यदि (cfg.pack_refs < 0)
		cfg.pack_refs = !is_bare_repository(the_repository);

	argc = parse_options(argc, argv, उपसर्गः, builtin_gc_options,
			     builtin_gc_usage, 0);
	यदि (argc > 0)
		usage_with_options(builtin_gc_usage, builtin_gc_options);

	यदि (prune_expire_arg != prune_expire_sentinel) {
		मुक्त(cfg.prune_expire);
		cfg.prune_expire = xstrdup_or_null(prune_expire_arg);
	}
	यदि (cfg.prune_expire && parse_expiry_date(cfg.prune_expire, &dummy))
		die(_("failed to parse prune expiry value %s"), cfg.prune_expire);

	यदि (opts.auto_flag) {
		संरचना odb_optimize_options optimize_opts = {
			.strategy = ODB_OPTIMIZE_INCREMENTAL,
			OPTIMIZE_FIELDS_FROM_GC_CONFIG(&cfg, 0),
		};

		यदि (cfg.detach_auto && opts.detach < 0)
			opts.detach = 1;

		/*
		 * Auto-gc should be least intrusive as possible.
		 */
		यदि (!odb_optimize_required(the_repository->objects, &optimize_opts) ||
		    सततम्_hooks(the_repository, "pre-auto-gc")) {
			ret = 0;
			गच्छ बहिर्गमनम्;
		}

		यदि (!opts.quiet) {
			यदि (opts.detach > 0)
				fprintf(stderr, _("Auto packing the repository in background for optimum performance.\n"));
			अन्यथा
				fprintf(stderr, _("Auto packing the repository for optimum performance.\n"));
			fprintf(stderr, _("See \"git help gc\" for manual housekeeping.\n"));
		}
	}

	यदि (opts.detach > 0) {
		ret = report_चरमः_gc_error();
		यदि (ret == 1) {
			/* Last gc --auto failed. Skip this one. */
			ret = 0;
			गच्छ बहिर्गमनम्;

		} अन्यथा यदि (ret) {
			/* an I/O error occurred, already reported */
			गच्छ बहिर्गमनम्;
		}

		यदि (!skip_foreground_tasks) {
			यदि (तालकम्_repo_for_gc(force, &pid)) {
				ret = 0;
				गच्छ बहिर्गमनम्;
			}

			यदि (gc_foreground_tasks(&opts, &cfg) < 0)
				die(रिक्त);
			delete_tempfile(&pidfile);
		}

		/*
		 * failure to daemonize is ok, we'll continue
		 * in foreground
		 */
		daemonized = !daemonize();
	}

	name = तालकम्_repo_for_gc(force, &pid);
	यदि (name) {
		यदि (opts.auto_flag) {
			ret = 0;
			गच्छ बहिर्गमनम्; /* be quiet on --auto */
		}

		die(_("gc is already running on machine '%s' pid %"PRIuMAX" (use --force if not)"),
		    name, (uintmax_t)pid);
	}

	यदि (daemonized) {
		वर्ण *मार्गः = repo_git_मार्गः(the_repository, "gc.log");
		hold_तालकम्_file_for_update(&log_तालकम्, मार्गः,
					  तालकम्_DIE_ON_ERROR);
		dup2(get_तालकम्_file_fd(&log_तालकम्), 2);
		atexit(process_log_file_at_exit);
		मुक्त(मार्गः);
	}

	यदि (opts.detach <= 0 && !skip_foreground_tasks)
		gc_foreground_tasks(&opts, &cfg);

	यदि (cfg.prune_worktrees_expire &&
	    maintenance_task_worktree_prune(&opts, &cfg))
		die(FAILED_सततम्, "worktree");

	यदि (maintenance_task_rerere_gc(&opts, &cfg))
		die(FAILED_सततम्, "rerere");

	यदि (maintenance_task_odb(&opts, &cfg, keep_largest_pack, aggressive))
		die(रिक्त);

	report_garbage = report_pack_garbage;
	odb_reprepare(the_repository->objects);
	यदि (pack_garbage.nr > 0) {
		odb_close(the_repository->objects);
		clean_pack_garbage();
	}

	यदि (the_repository->settings.gc_write_commit_graph == 1)
		write_commit_graph_reachable(the_repository,
					     the_repository->objects->source->मार्गः,
					     !opts.quiet && !daemonized ? COMMIT_GRAPH_WRITE_प्रगतिः : 0,
					     रिक्त);

	यदि (!daemonized) {
		वर्ण *मार्गः = repo_git_मार्गः(the_repository, "gc.log");
		unlink(मार्गः);
		मुक्त(मार्गः);
	}

बहिर्गमनम्:
	maintenance_सततम्_opts_release(&opts);
	gc_config_release(&cfg);
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *ध्रुव builtin_maintenance_सततम्_usage[] = {
	N_("git maintenance run [--auto] [--[no-]quiet] [--task=<task>] [--schedule]"),
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_opt_कालसारणी(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				    पूर्णाङ्क unset)
{
	गणना कालसारणी_प्राथमिकता *प्राथमिकता = opt->मानम्;

	यदि (unset)
		die(_("--no-schedule is not allowed"));

	*प्राथमिकता = parse_कालसारणी(arg);

	यदि (!*प्राथमिकता)
		die(_("unrecognized --schedule argument '%s'"), arg);

	प्रत्यागम 0;
}

संरचना cg_auto_दत्त {
	पूर्णाङ्क अङ्कः_not_अन्तर्_graph;
	पूर्णाङ्क सीमा;
};

स्थिर पूर्णाङ्क dfs_on_ref(ध्रुव संरचना reference *ref, शून्य *cb_दत्त)
{
	संरचना cg_auto_दत्त *दत्त = (संरचना cg_auto_दत्त *)cb_दत्त;
	पूर्णाङ्क परिणाम = 0;
	ध्रुव संरचना object_id *maybe_peeled = ref->oid;
	संरचना object_id peeled;
	संरचना commit_list *राशिः = रिक्त;
	संरचना commit *commit;

	यदि (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;
	यदि (odb_read_object_info(the_repository->objects, maybe_peeled, रिक्त) != OBJ_COMMIT)
		प्रत्यागम 0;

	commit = lookup_commit(the_repository, maybe_peeled);
	यदि (!commit || commit->object.flags & आलोकितम्_2)
		प्रत्यागम 0;
	commit->object.flags |= आलोकितम्_2;

	यदि (repo_parse_commit(the_repository, commit) ||
	    commit_graph_position(commit) != COMMIT_NOT_FROM_GRAPH)
		प्रत्यागम 0;

	दत्त->अङ्कः_not_अन्तर्_graph++;

	यदि (दत्त->अङ्कः_not_अन्तर्_graph >= दत्त->सीमा)
		प्रत्यागम 1;

	commit_list_निवेशय(commit, &राशिः);

	यावत् (!परिणाम && राशिः) {
		संरचना commit_list *parent;

		commit = निष्कासय_commit(&राशिः);

		प्रति (parent = commit->parents; parent; parent = parent->अग्र) {
			यदि (repo_parse_commit(the_repository, parent->item) ||
			    commit_graph_position(parent->item) != COMMIT_NOT_FROM_GRAPH ||
			    parent->item->object.flags & आलोकितम्_2)
				अनुवर्त;

			parent->item->object.flags |= आलोकितम्_2;
			दत्त->अङ्कः_not_अन्तर्_graph++;

			यदि (दत्त->अङ्कः_not_अन्तर्_graph >= दत्त->सीमा) {
				परिणाम = 1;
				भङ्ग;
			}

			commit_list_निवेशय(parent->item, &राशिः);
		}
	}

	commit_list_free(राशिः);
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क should_write_commit_graph(संरचना gc_config *cfg अप्रयुक्तम्)
{
	पूर्णाङ्क परिणाम;
	संरचना cg_auto_दत्त दत्त;

	दत्त.अङ्कः_not_अन्तर्_graph = 0;
	दत्त.सीमा = 100;
	repo_config_get_int(the_repository, "maintenance.commit-graph.auto",
			    &दत्त.सीमा);

	यदि (!दत्त.सीमा)
		प्रत्यागम 0;
	यदि (दत्त.सीमा < 0)
		प्रत्यागम 1;

	परिणाम = refs_for_each_ref(get_main_ref_store(the_repository),
				   dfs_on_ref, &दत्त);

	repo_clear_commit_marks(the_repository, आलोकितम्_2);

	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क सततम्_write_commit_graph(संरचना maintenance_सततम्_opts *opts)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_गन्तव्यम्_close = the_repository->objects;
	strvec_pushl(&child.तर्काः, "commit-graph", "write",
		     "--split", "--reachable", रिक्त);

	यदि (opts->quiet)
		strvec_प्रक्षिप(&child.तर्काः, "--no-progress");
	अन्यथा
		strvec_प्रक्षिप(&child.तर्काः, "--progress");

	प्रत्यागम !!सततम्_command(&child);
}

स्थिर पूर्णाङ्क maintenance_task_commit_graph(संरचना maintenance_सततम्_opts *opts,
					 संरचना gc_config *cfg अप्रयुक्तम्)
{
	prepare_repo_settings(the_repository);
	यदि (!the_repository->settings.core_commit_graph)
		प्रत्यागम 0;

	यदि (सततम्_write_commit_graph(opts)) {
		error(_("failed to write commit-graph"));
		प्रत्यागम 1;
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क fetch_remote(संरचना remote *remote, शून्य *cbdata)
{
	संरचना maintenance_सततम्_opts *opts = cbdata;
	संरचना child_process child = CHILD_PROCESS_INIT;

	यदि (remote->skip_default_update)
		प्रत्यागम 0;

	child.git_cmd = 1;
	strvec_pushl(&child.तर्काः, "fetch", remote->name,
		     "--prefetch", "--prune", "--no-tags",
		     "--no-write-fetch-head", "--recurse-submodules=no",
		     रिक्त);

	यदि (opts->quiet)
		strvec_प्रक्षिप(&child.तर्काः, "--quiet");

	प्रत्यागम !!सततम्_command(&child);
}

स्थिर पूर्णाङ्क maintenance_task_prefetch(संरचना maintenance_सततम्_opts *opts,
				     संरचना gc_config *cfg अप्रयुक्तम्)
{
	यदि (for_each_remote(fetch_remote, opts)) {
		error(_("failed to prefetch remotes"));
		प्रत्यागम 1;
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क maintenance_task_gc_foreground(संरचना maintenance_सततम्_opts *opts,
					  संरचना gc_config *cfg)
{
	प्रत्यागम gc_foreground_tasks(opts, cfg);
}

स्थिर पूर्णाङ्क maintenance_task_gc_background(संरचना maintenance_सततम्_opts *opts,
					  संरचना gc_config *cfg अप्रयुक्तम्)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_गन्तव्यम्_close = the_repository->objects;
	strvec_प्रक्षिप(&child.तर्काः, "gc");

	यदि (opts->auto_flag)
		strvec_प्रक्षिप(&child.तर्काः, "--auto");
	यदि (opts->quiet)
		strvec_प्रक्षिप(&child.तर्काः, "--quiet");
	अन्यथा
		strvec_प्रक्षिप(&child.तर्काः, "--no-quiet");
	strvec_प्रक्षिप(&child.तर्काः, "--no-detach");
	strvec_प्रक्षिप(&child.तर्काः, "--skip-foreground-tasks");

	प्रत्यागम सततम्_command(&child);
}

स्थिर पूर्णाङ्क gc_condition(संरचना gc_config *cfg)
{
	संरचना odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_INCREMENTAL,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	प्रत्यागम odb_optimize_required(the_repository->objects, &opts);
}

स्थिर पूर्णाङ्क prune_packed(संरचना maintenance_सततम्_opts *opts)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_प्रक्षिप(&child.तर्काः, "prune-packed");

	यदि (opts->quiet)
		strvec_प्रक्षिप(&child.तर्काः, "--quiet");

	प्रत्यागम !!सततम्_command(&child);
}

संरचना write_loose_object_दत्त {
	FILE *अन्तर्;
	पूर्णाङ्क गणनम्;
	पूर्णाङ्क batch_आकार;
};

स्थिर पूर्णाङ्क loose_object_auto_सीमा = 100;

स्थिर पूर्णाङ्क loose_object_गणनम्(ध्रुव संरचना object_id *oid अप्रयुक्तम्,
			      ध्रुव वर्ण *मार्गः अप्रयुक्तम्,
			      शून्य *दत्त)
{
	पूर्णाङ्क *गणनम् = (पूर्णाङ्क*)दत्त;
	यदि (++(*गणनम्) >= loose_object_auto_सीमा)
		प्रत्यागम 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क loose_object_auto_condition(संरचना gc_config *cfg अप्रयुक्तम्)
{
	पूर्णाङ्क गणनम् = 0;

	repo_config_get_int(the_repository, "maintenance.loose-objects.auto",
			    &loose_object_auto_सीमा);

	यदि (!loose_object_auto_सीमा)
		प्रत्यागम 0;
	यदि (loose_object_auto_सीमा < 0)
		प्रत्यागम 1;

	प्रत्यागम for_each_loose_file_अन्तर्_source(the_repository->objects->source,
					     loose_object_गणनम्,
					     रिक्त, रिक्त, &गणनम्);
}

स्थिर पूर्णाङ्क bail_on_loose(ध्रुव संरचना object_id *oid अप्रयुक्तम्,
			 ध्रुव वर्ण *मार्गः अप्रयुक्तम्,
			 शून्य *दत्त अप्रयुक्तम्)
{
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क write_loose_object_गन्तव्यम्_stdin(ध्रुव संरचना object_id *oid,
				       ध्रुव वर्ण *मार्गः अप्रयुक्तम्,
				       शून्य *दत्त)
{
	संरचना write_loose_object_दत्त *d = (संरचना write_loose_object_दत्त *)दत्त;

	fprintf(d->अन्तर्, "%s\n", oid_गन्तव्यम्_hex(oid));

	/* If batch_size is INT_MAX, then this will return 0 always. */
	प्रत्यागम ++(d->गणनम्) > d->batch_आकार;
}

स्थिर पूर्णाङ्क pack_loose(संरचना maintenance_सततम्_opts *opts)
{
	संरचना repository *r = the_repository;
	पूर्णाङ्क परिणाम = 0;
	संरचना write_loose_object_दत्त दत्त;
	संरचना child_process pack_proc = CHILD_PROCESS_INIT;

	/*
	 * Do not start pack-objects process
	 * if there are no loose objects.
	 */
	यदि (!for_each_loose_file_अन्तर्_source(r->objects->source,
					   bail_on_loose,
					   रिक्त, रिक्त, रिक्त))
		प्रत्यागम 0;

	pack_proc.git_cmd = 1;

	strvec_प्रक्षिप(&pack_proc.तर्काः, "pack-objects");
	यदि (opts->quiet)
		strvec_प्रक्षिप(&pack_proc.तर्काः, "--quiet");
	अन्यथा
		strvec_प्रक्षिप(&pack_proc.तर्काः, "--no-quiet");
	strvec_pushf(&pack_proc.तर्काः, "%s/pack/loose", r->objects->source->मार्गः);

	pack_proc.अन्तर् = -1;

	/*
	 * git-pack-objects(1) ends up writing the pack hash to stdout, which
	 * we do not care for.
	 */
	pack_proc.बहिर्गमनम् = -1;

	यदि (start_command(&pack_proc)) {
		error(_("failed to start 'git pack-objects' process"));
		प्रत्यागम 1;
	}

	दत्त.अन्तर् = xfdopen(pack_proc.अन्तर्, "w");
	दत्त.गणनम् = 0;
	दत्त.batch_आकार = 50000;

	repo_config_get_int(r, "maintenance.loose-objects.batchSize",
			    &दत्त.batch_आकार);

	/* If configured as 0, then remove limit. */
	यदि (!दत्त.batch_आकार)
		दत्त.batch_आकार = अधिकतम_पूर्णाङ्कः;
	अन्यथा यदि (दत्त.batch_आकार > 0)
		दत्त.batch_आकार--; /* Decrease for equality on limit. */

	for_each_loose_file_अन्तर्_source(r->objects->source,
				      write_loose_object_गन्तव्यम्_stdin,
				      रिक्त, रिक्त, &दत्त);

	fclose(दत्त.अन्तर्);

	यदि (finish_command(&pack_proc)) {
		error(_("failed to finish 'git pack-objects' process"));
		परिणाम = 1;
	}

	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क maintenance_task_loose_objects(संरचना maintenance_सततम्_opts *opts,
					  संरचना gc_config *cfg अप्रयुक्तम्)
{
	प्रत्यागम prune_packed(opts) || pack_loose(opts);
}

स्थिर पूर्णाङ्क incremental_repack_auto_condition(संरचना gc_config *cfg अप्रयुक्तम्)
{
	संरचना packed_git *p;
	पूर्णाङ्क incremental_repack_auto_सीमा = 10;
	पूर्णाङ्क गणनम् = 0;

	prepare_repo_settings(the_repository);
	यदि (!the_repository->settings.core_multi_pack_सूचकांक)
		प्रत्यागम 0;

	repo_config_get_int(the_repository, "maintenance.incremental-repack.auto",
			    &incremental_repack_auto_सीमा);

	यदि (!incremental_repack_auto_सीमा)
		प्रत्यागम 0;
	यदि (incremental_repack_auto_सीमा < 0)
		प्रत्यागम 1;

	repo_for_each_pack(the_repository, p) {
		यदि (गणनम् >= incremental_repack_auto_सीमा)
			भङ्ग;
		यदि (!p->multi_pack_सूचकांक)
			गणनम्++;
	}

	प्रत्यागम गणनम् >= incremental_repack_auto_सीमा;
}

स्थिर पूर्णाङ्क multi_pack_सूचकांक_write(संरचना maintenance_सततम्_opts *opts)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	strvec_pushl(&child.तर्काः, "multi-pack-index", "write", रिक्त);

	यदि (opts->quiet)
		strvec_प्रक्षिप(&child.तर्काः, "--no-progress");
	अन्यथा
		strvec_प्रक्षिप(&child.तर्काः, "--progress");

	यदि (सततम्_command(&child))
		प्रत्यागम error(_("failed to write multi-pack-index"));

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क multi_pack_सूचकांक_expire(संरचना maintenance_सततम्_opts *opts)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_गन्तव्यम्_close = the_repository->objects;
	strvec_pushl(&child.तर्काः, "multi-pack-index", "expire", रिक्त);

	यदि (opts->quiet)
		strvec_प्रक्षिप(&child.तर्काः, "--no-progress");
	अन्यथा
		strvec_प्रक्षिप(&child.तर्काः, "--progress");

	यदि (सततम्_command(&child))
		प्रत्यागम error(_("'git multi-pack-index expire' failed"));

	प्रत्यागम 0;
}

#परिभाषय TWO_GIGABYTES (INT32_MAX)

स्थिर off_t get_auto_pack_आकार(शून्य)
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
	संरचना packed_git *p;
	संरचना repository *r = the_repository;

	odb_reprepare(r->objects);
	repo_for_each_pack(r, p) {
		यदि (p->pack_आकार > max_आकार) {
			second_largest_आकार = max_आकार;
			max_आकार = p->pack_आकार;
		} अन्यथा यदि (p->pack_आकार > second_largest_आकार)
			second_largest_आकार = p->pack_आकार;
	}

	परिणाम_आकार = second_largest_आकार + 1;

	/* But limit ourselves to a batch size of 2g */
	यदि (परिणाम_आकार > TWO_GIGABYTES)
		परिणाम_आकार = TWO_GIGABYTES;

	प्रत्यागम परिणाम_आकार;
}

स्थिर पूर्णाङ्क multi_pack_सूचकांक_repack(संरचना maintenance_सततम्_opts *opts)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	child.git_cmd = 1;
	child.odb_गन्तव्यम्_close = the_repository->objects;
	strvec_pushl(&child.तर्काः, "multi-pack-index", "repack", रिक्त);

	यदि (opts->quiet)
		strvec_प्रक्षिप(&child.तर्काः, "--no-progress");
	अन्यथा
		strvec_प्रक्षिप(&child.तर्काः, "--progress");

	strvec_pushf(&child.तर्काः, "--batch-size=%"PRIuMAX,
				  (uintmax_t)get_auto_pack_आकार());

	यदि (सततम्_command(&child))
		प्रत्यागम error(_("'git multi-pack-index repack' failed"));

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क maintenance_task_incremental_repack(संरचना maintenance_सततम्_opts *opts,
					       संरचना gc_config *cfg अप्रयुक्तम्)
{
	prepare_repo_settings(the_repository);
	यदि (!the_repository->settings.core_multi_pack_सूचकांक) {
		warning(_("skipping incremental-repack task because core.multiPackIndex is disabled"));
		प्रत्यागम 0;
	}

	यदि (multi_pack_सूचकांक_write(opts))
		प्रत्यागम 1;
	यदि (multi_pack_सूचकांक_expire(opts))
		प्रत्यागम 1;
	यदि (multi_pack_सूचकांक_repack(opts))
		प्रत्यागम 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क maintenance_task_geometric_repack(संरचना maintenance_सततम्_opts *opts,
					     संरचना gc_config *cfg)
{
	संरचना odb_optimize_options odb_opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};

	यदि (!opts->quiet)
		odb_opts.flags |= ODB_OPTIMIZE_VERBOSE;

	प्रत्यागम odb_optimize(the_repository->objects, &odb_opts);
}

स्थिर पूर्णाङ्क geometric_repack_auto_condition(संरचना gc_config *cfg)
{
	संरचना odb_optimize_options opts = {
		.strategy = ODB_OPTIMIZE_GEOMETRIC,
		OPTIMIZE_FIELDS_FROM_GC_CONFIG(cfg, 0),
	};
	प्रत्यागम odb_optimize_required(the_repository->objects, &opts);
}

प्रकारनिर्देश पूर्णाङ्क (*maintenance_task_fn)(संरचना maintenance_सततम्_opts *opts,
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

गणना auto_gc_hook_परिणाम {
	AUTO_GC_HOOK_UNDECIDED = 0,
	AUTO_GC_HOOK_सततम् = 1,
	AUTO_GC_HOOK_SKIP = 2,
};

स्थिर पूर्णाङ्क maybe_सततम्_task(ध्रुव संरचना maintenance_task *task,
			  संरचना repository *repo,
			  संरचना maintenance_सततम्_opts *opts,
			  संरचना gc_config *cfg,
			  गणना task_phase phase,
			  गणना auto_gc_hook_परिणाम *auto_gc_hook_परिणाम)
{
	पूर्णाङ्क foreground = (phase == TASK_PHASE_FOREGROUND);
	maintenance_task_fn fn = foreground ? task->foreground : task->background;
	ध्रुव वर्ण *region = foreground ? "maintenance foreground" : "maintenance";
	पूर्णाङ्क ret = 0;

	यदि (!fn)
		प्रत्यागम 0;
	यदि (opts->auto_flag) {
		यदि (*auto_gc_hook_परिणाम == AUTO_GC_HOOK_SKIP)
			प्रत्यागम 0;

		यदि (!task->auto_condition || !task->auto_condition(cfg))
			प्रत्यागम 0;

		यदि (*auto_gc_hook_परिणाम == AUTO_GC_HOOK_UNDECIDED)
			*auto_gc_hook_परिणाम = सततम्_hooks(repo, "pre-auto-gc") ?
				AUTO_GC_HOOK_SKIP : AUTO_GC_HOOK_सततम्;
		यदि (*auto_gc_hook_परिणाम == AUTO_GC_HOOK_SKIP)
			प्रत्यागम 0;
	}

	trace2_region_enter(region, task->name, repo);
	यदि (fn(opts, cfg)) {
		error(_("task '%s' failed"), task->name);
		ret = 1;
	}
	trace2_region_leave(region, task->name, repo);

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क maintenance_सततम्_tasks(संरचना maintenance_सततम्_opts *opts,
				 संरचना gc_config *cfg)
{
	पूर्णाङ्क परिणाम = 0;
	संरचना तालकम्_file lk;
	संरचना repository *r = the_repository;
	वर्ण *तालकम्_मार्गः = xstrfmt("%s/maintenance", r->objects->source->मार्गः);
	गणना auto_gc_hook_परिणाम auto_gc_hook_परिणाम = AUTO_GC_HOOK_UNDECIDED;

	यदि (repo_hold_तालकम्_file_for_update(r, &lk, तालकम्_मार्गः, तालकम्_NO_DEREF) < 0) {
		/*
		 * Another maintenance command is running.
		 *
		 * If --auto was provided, then it is likely due to a
		 * recursive process stack. Do not report an error in
		 * that case.
		 */
		यदि (!opts->auto_flag && !opts->quiet)
			warning(_("lock file '%s' exists, skipping maintenance"),
				तालकम्_मार्गः);
		मुक्त(तालकम्_मार्गः);
		प्रत्यागम 0;
	}
	मुक्त(तालकम्_मार्गः);

	प्रति (आकार_t i = 0; i < opts->tasks_nr; i++)
		यदि (maybe_सततम्_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_FOREGROUND, &auto_gc_hook_परिणाम))
			परिणाम = 1;

	/* Failure to daemonize is ok, we'll continue in foreground. */
	यदि (opts->detach > 0) {
		trace2_region_enter("maintenance", "detach", the_repository);
		daemonize();
		trace2_region_leave("maintenance", "detach", the_repository);
	}

	प्रति (आकार_t i = 0; i < opts->tasks_nr; i++)
		यदि (maybe_सततम्_task(&tasks[opts->tasks[i]], r, opts, cfg,
				   TASK_PHASE_BACKGROUND, &auto_gc_hook_परिणाम))
			परिणाम = 1;

	rollback_तालकम्_file(&lk);
	प्रत्यागम परिणाम;
}

गणना maintenance_type {
	/* As invoked via `git maintenance run --schedule=`. */
	MAINTENANCE_TYPE_SCHEDULED = (1 << 0),
	/* As invoked via `git maintenance run` and with `--auto`. */
	MAINTENANCE_TYPE_MANUAL    = (1 << 1),
};

संरचना maintenance_strategy {
	संरचना {
		अचिह्नित type;
		गणना कालसारणी_प्राथमिकता कालसारणी;
	} tasks[TASK_गणनम्];
};

स्थिर ध्रुव संरचना maintenance_strategy none_strategy = { 0 };

स्थिर ध्रुव संरचना maintenance_strategy gc_strategy = {
	.tasks = {
		[TASK_GC] = {
			.type = MAINTENANCE_TYPE_MANUAL | MAINTENANCE_TYPE_SCHEDULED,
			.कालसारणी = कालसारणी_DAILY,
		},
	},
};

स्थिर ध्रुव संरचना maintenance_strategy incremental_strategy = {
	.tasks = {
		[TASK_COMMIT_GRAPH] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.कालसारणी = कालसारणी_HOURLY,
		},
		[TASK_PREFETCH] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.कालसारणी = कालसारणी_HOURLY,
		},
		[TASK_INCREMENTAL_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.कालसारणी = कालसारणी_DAILY,
		},
		[TASK_LOOSE_OBJECTS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.कालसारणी = कालसारणी_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED,
			.कालसारणी = कालसारणी_WEEKLY,
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
			.कालसारणी = कालसारणी_HOURLY,
		},
		[TASK_GEOMETRIC_REPACK] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.कालसारणी = कालसारणी_DAILY,
		},
		[TASK_PACK_REFS] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.कालसारणी = कालसारणी_DAILY,
		},
		[TASK_RERERE_GC] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.कालसारणी = कालसारणी_WEEKLY,
		},
		[TASK_REFLOG_EXPIRE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.कालसारणी = कालसारणी_WEEKLY,
		},
		[TASK_WORKTREE_PRUNE] = {
			.type = MAINTENANCE_TYPE_SCHEDULED | MAINTENANCE_TYPE_MANUAL,
			.कालसारणी = कालसारणी_WEEKLY,
		},
	},
};

स्थिर संरचना maintenance_strategy parse_maintenance_strategy(ध्रुव वर्ण *name)
{
	यदि (!strcasecmp(name, "incremental"))
		प्रत्यागम incremental_strategy;
	यदि (!strcasecmp(name, "gc"))
		प्रत्यागम gc_strategy;
	यदि (!strcasecmp(name, "geometric"))
		प्रत्यागम geometric_strategy;
	die(_("unknown maintenance strategy: '%s'"), name);
}

स्थिर शून्य initialize_task_config(संरचना maintenance_सततम्_opts *opts,
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
	यदि (selected_tasks->nr) {
		प्रति (आकार_t i = 0; i < selected_tasks->nr; i++) {
			गणना maintenance_task_label label = (intptr_t)selected_tasks->वस्तूनि[i].util;;
			ALLOC_GROW(opts->tasks, opts->tasks_nr + 1, opts->tasks_alloc);
			opts->tasks[opts->tasks_nr++] = label;
		}

		प्रत्यागम;
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
	यदि (opts->कालसारणी) {
		strategy = none_strategy;
		type = MAINTENANCE_TYPE_SCHEDULED;
	} अन्यथा {
		strategy = geometric_strategy;
		type = MAINTENANCE_TYPE_MANUAL;
	}

	यदि (!repo_config_get_string_tmp(the_repository, "maintenance.strategy", &config_str))
		strategy = parse_maintenance_strategy(config_str);

	प्रति (आकार_t i = 0; i < TASK_गणनम्; i++) {
		पूर्णाङ्क config_मानम्;

		strbuf_reset(&config_name);
		strbuf_addf(&config_name, "maintenance.%s.enabled",
			    tasks[i].name);
		यदि (!repo_config_get_bool(the_repository, config_name.buf, &config_मानम्))
			strategy.tasks[i].type = config_मानम् ? type : 0;
		यदि (!(strategy.tasks[i].type & type))
			अनुवर्त;

		यदि (opts->कालसारणी) {
			strbuf_reset(&config_name);
			strbuf_addf(&config_name, "maintenance.%s.schedule",
				    tasks[i].name);
			यदि (!repo_config_get_string_tmp(the_repository, config_name.buf, &config_str))
				strategy.tasks[i].कालसारणी = parse_कालसारणी(config_str);
			यदि (strategy.tasks[i].कालसारणी < opts->कालसारणी)
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
	आकार_t i;

	BUG_ON_OPT_NEG(unset);

	प्रति (i = 0; i < TASK_गणनम्; i++)
		यदि (!strcasecmp(tasks[i].name, arg))
			भङ्ग;
	यदि (i >= TASK_गणनम्) {
		error(_("'%s' is not a valid task"), arg);
		प्रत्यागम 1;
	}

	यदि (unsorted_string_list_has_string(selected_tasks, arg)) {
		error(_("task '%s' cannot be selected multiple times"), arg);
		प्रत्यागम 1;
	}

	string_list_append(selected_tasks, arg)->util = (शून्य *)(intptr_t)i;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क maintenance_सततम्(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			   संरचना repository *repo अप्रयुक्तम्)
{
	संरचना maintenance_सततम्_opts opts = MAINTENANCE_सततम्_OPTS_INIT;
	संरचना string_list selected_tasks = STRING_LIST_INIT_DUP;
	संरचना gc_config cfg = GC_CONFIG_INIT;
	संरचना option builtin_maintenance_सततम्_options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_BOOL(0, "detach", &opts.detach,
			 N_("perform maintenance in the background")),
		OPT_CALLBACK(0, "schedule", &opts.कालसारणी, N_("frequency"),
			     N_("run tasks based on frequency"),
			     maintenance_opt_कालसारणी),
		OPT_BOOL(0, "quiet", &opts.quiet,
			 N_("do not report progress or other information over stderr")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			N_("run a specific task"),
			PARSE_OPT_NONEG, task_option_parse),
		OPT_अन्तः()
	};
	पूर्णाङ्क ret;

	opts.quiet = !isatty(2);

	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_maintenance_सततम्_options,
			     builtin_maintenance_सततम्_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	die_for_incompatible_opt2(opts.auto_flag, "--auto",
				  opts.कालसारणी, "--schedule=");
	die_for_incompatible_opt2(selected_tasks.nr, "--task=",
				  opts.कालसारणी, "--schedule=");

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	यदि (argc != 0)
		usage_with_options(builtin_maintenance_सततम्_usage,
				   builtin_maintenance_सततम्_options);

	ret = maintenance_सततम्_tasks(&opts, &cfg);

	string_list_clear(&selected_tasks, 0);
	maintenance_सततम्_opts_release(&opts);
	gc_config_release(&cfg);
	प्रत्यागम ret;
}

स्थिर वर्ण *get_maintpath(शून्य)
{
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *p = the_repository->worktree ?
		the_repository->worktree : the_repository->gitdir;

	strbuf_realpath(&sb, p, 1);
	प्रत्यागम strbuf_detach(&sb, रिक्त);
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_maintenance_पञ्जिकाकोशः_usage[] = {
	"git maintenance register [--config-file <path>]",
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_पञ्जिकाकोशः(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				संरचना repository *repo अप्रयुक्तम्)
{
	वर्ण *config_file = रिक्त;
	संरचना option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT_अन्तः(),
	};
	पूर्णाङ्क प्राप्तम्_2 = 0;
	ध्रुव वर्ण *कुञ्चिका = "maintenance.repo";
	वर्ण *maintpath = get_maintpath();
	संरचना string_list_item *item;
	ध्रुव संरचना string_list *list;

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_maintenance_पञ्जिकाकोशः_usage, 0);
	यदि (argc)
		usage_with_options(builtin_maintenance_पञ्जिकाकोशः_usage,
				   options);

	/* Disable foreground maintenance */
	repo_config_समुच्चयः(the_repository, "maintenance.auto", "false");

	/* Set maintenance strategy, if unset */
	यदि (repo_config_get(the_repository, "maintenance.strategy"))
		repo_config_समुच्चयः(the_repository, "maintenance.strategy", "incremental");

	यदि (!repo_config_get_string_multi(the_repository, कुञ्चिका, &list)) {
		for_each_string_list_item(item, list) {
			यदि (!वाक्यं_तुलय(maintpath, item->string)) {
				प्राप्तम्_2 = 1;
				भङ्ग;
			}
		}
	}

	यदि (!प्राप्तम्_2) {
		पूर्णाङ्क rc;
		वर्ण *global_config_file = रिक्त;

		यदि (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		यदि (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_समुच्चयः_multivar_अन्तर्_file_gently(the_repository,
			config_file, "maintenance.repo", maintpath,
			CONFIG_REGEX_NONE, रिक्त, 0);
		मुक्त(global_config_file);

		यदि (rc)
			die(_("unable to add '%s' value of '%s'"),
			    कुञ्चिका, maintpath);
	}

	मुक्त(maintpath);
	प्रत्यागम 0;
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_maintenance_unregister_usage[] = {
	"git maintenance unregister [--config-file <path>] [--force]",
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_unregister(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				  संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क force = 0;
	वर्ण *config_file = रिक्त;
	संरचना option options[] = {
		OPT_STRING(0, "config-file", &config_file, N_("file"), N_("use given config file")),
		OPT__FORCE(&force,
			   N_("return success even if repository was not registered"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_अन्तः(),
	};
	ध्रुव वर्ण *कुञ्चिका = "maintenance.repo";
	वर्ण *maintpath = get_maintpath();
	पूर्णाङ्क प्राप्तम्_2 = 0;
	संरचना string_list_item *item;
	ध्रुव संरचना string_list *list;
	संरचना config_समुच्चयः cs = { { 0 } };

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_maintenance_unregister_usage, 0);
	यदि (argc)
		usage_with_options(builtin_maintenance_unregister_usage,
				   options);

	यदि (config_file) {
		git_configset_init(&cs);
		git_configset_सङ्कलय_file(&cs, config_file);
	}
	यदि (!(config_file
	      ? git_configset_get_string_multi(&cs, कुञ्चिका, &list)
	      : repo_config_get_string_multi(the_repository, कुञ्चिका, &list))) {
		for_each_string_list_item(item, list) {
			यदि (!वाक्यं_तुलय(maintpath, item->string)) {
				प्राप्तम्_2 = 1;
				भङ्ग;
			}
		}
	}

	यदि (प्राप्तम्_2) {
		पूर्णाङ्क rc;
		वर्ण *global_config_file = रिक्त;

		यदि (!config_file) {
			global_config_file = git_global_config();
			config_file = global_config_file;
		}
		यदि (!config_file)
			die(_("$HOME not set"));
		rc = repo_config_समुच्चयः_multivar_अन्तर्_file_gently(the_repository,
			config_file, कुञ्चिका, रिक्त, maintpath, रिक्त,
			CONFIG_FLAGS_MULTI_REPLACE | CONFIG_FLAGS_FIXED_मानम्);
		मुक्त(global_config_file);

		यदि (rc &&
		    (!force || rc == CONFIG_NOTHING_समुच्चयः))
			die(_("unable to unset '%s' value of '%s'"),
			    कुञ्चिका, maintpath);
	} अन्यथा यदि (!force) {
		die(_("repository '%s' is not registered"), maintpath);
	}

	git_configset_clear(&cs);
	मुक्त(maintpath);
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *get_frequency(गणना कालसारणी_प्राथमिकता कालसारणी)
{
	विकल्प (कालसारणी) {
	पक्ष कालसारणी_HOURLY:
		प्रत्यागम "hourly";
	पक्ष कालसारणी_DAILY:
		प्रत्यागम "daily";
	पक्ष कालसारणी_WEEKLY:
		प्रत्यागम "weekly";
	स्वभाव:
		BUG("invalid schedule %d", कालसारणी);
	}
}

स्थिर ध्रुव वर्ण *extraconfig[] = {
	"credential.interactive=false",
	"core.askPass=true", /* 'true' returns success, but no output. */
	रिक्त
};

स्थिर ध्रुव वर्ण *get_extra_config_parameters(शून्य) {
	स्थिर ध्रुव वर्ण *परिणाम = रिक्त;
	संरचना strbuf builder = STRBUF_INIT;

	यदि (परिणाम)
		प्रत्यागम परिणाम;

	प्रति (ध्रुव वर्ण **s = extraconfig; s && *s; s++)
		strbuf_addf(&builder, "-c %s ", *s);

	परिणाम = strbuf_detach(&builder, रिक्त);
	प्रत्यागम परिणाम;
}

स्थिर ध्रुव वर्ण *get_extra_launchctl_वाक्यानि(शून्य) {
	स्थिर ध्रुव वर्ण *परिणाम = रिक्त;
	संरचना strbuf builder = STRBUF_INIT;

	यदि (परिणाम)
		प्रत्यागम परिणाम;

	प्रति (ध्रुव वर्ण **s = extraconfig; s && *s; s++) {
		strbuf_addstr(&builder, "<string>-c</string>\n");
		strbuf_addf(&builder, "<string>%s</string>\n", *s);
	}

	परिणाम = strbuf_detach(&builder, रिक्त);
	प्रत्यागम परिणाम;
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
स्थिर पूर्णाङ्क get_कालसारणी_cmd(ध्रुव वर्ण *cmd, पूर्णाङ्क *is_उपलब्धम्, वर्ण **बहिर्गमनम्)
{
	वर्ण *testing = xstrdup_or_null(getenv("GIT_TEST_MAINT_SCHEDULER"));
	संरचना string_list_item *item;
	संरचना string_list list = STRING_LIST_INIT_NODUP;

	यदि (!testing) {
		यदि (बहिर्गमनम्)
			*बहिर्गमनम् = xstrdup(cmd);
		प्रत्यागम 0;
	}

	यदि (is_उपलब्धम्)
		*is_उपलब्धम् = 0;

	string_list_विभज_अन्तर्_place(&list, testing, ",", -1);
	for_each_string_list_item(item, &list) {
		संरचना string_list pair = STRING_LIST_INIT_NODUP;

		यदि (string_list_विभज_अन्तर्_place(&pair, item->string, ":", 2) != 2)
			अनुवर्त;

		यदि (!वाक्यं_तुलय(cmd, pair.वस्तूनि[0].string)) {
			यदि (बहिर्गमनम्)
				*बहिर्गमनम् = xstrdup(pair.वस्तूनि[1].string);
			यदि (is_उपलब्धम्)
				*is_उपलब्धम् = 1;
			string_list_clear(&pair, 0);
			गच्छ बहिर्गमनम्;
		}

		string_list_clear(&pair, 0);
	}

	यदि (बहिर्गमनम्)
		*बहिर्गमनम् = xstrdup(cmd);

बहिर्गमनम्:
	string_list_clear(&list, 0);
	मुक्त(testing);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क get_random_minute(शून्य)
{
	/* Use a static value when under tests. */
	यदि (getenv("GIT_TEST_MAINT_SCHEDULER"))
		प्रत्यागम 13;

	प्रत्यागम git_rand(0) % 60;
}

स्थिर पूर्णाङ्क is_launchctl_उपलब्धम्(शून्य)
{
	पूर्णाङ्क is_उपलब्धम्;
	यदि (get_कालसारणी_cmd("launchctl", &is_उपलब्धम्, रिक्त))
		प्रत्यागम is_उपलब्धम्;

#यदि_परिभाषितम् __APPLE__
	प्रत्यागम 1;
#अन्यथा
	प्रत्यागम 0;
#यदि_अन्तः
}

स्थिर वर्ण *launchctl_service_name(ध्रुव वर्ण *frequency)
{
	संरचना strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "org.git-scm.git.%s", frequency);
	प्रत्यागम strbuf_detach(&label, रिक्त);
}

स्थिर वर्ण *launchctl_service_filename(ध्रुव वर्ण *name)
{
	वर्ण *expanded;
	संरचना strbuf filename = STRBUF_INIT;
	strbuf_addf(&filename, "~/Library/LaunchAgents/%s.plist", name);

	expanded = interpolate_मार्गः(filename.buf, 1);
	यदि (!expanded)
		die(_("failed to expand path '%s'"), filename.buf);

	strbuf_release(&filename);
	प्रत्यागम expanded;
}

स्थिर वर्ण *launchctl_get_uid(शून्य)
{
	प्रत्यागम xstrfmt("gui/%d", getuid());
}

स्थिर पूर्णाङ्क launchctl_boot_plist(पूर्णाङ्क enable, ध्रुव वर्ण *filename)
{
	वर्ण *cmd;
	पूर्णाङ्क परिणाम;
	संरचना child_process child = CHILD_PROCESS_INIT;
	वर्ण *uid = launchctl_get_uid();

	get_कालसारणी_cmd("launchctl", रिक्त, &cmd);
	strvec_विभज(&child.तर्काः, cmd);
	strvec_pushl(&child.तर्काः, enable ? "bootstrap" : "bootout", uid,
		     filename, रिक्त);

	child.no_stderr = 1;
	child.no_stdout = 1;

	यदि (start_command(&child))
		die(_("failed to start launchctl"));

	परिणाम = finish_command(&child);

	मुक्त(cmd);
	मुक्त(uid);
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क launchctl_remove_plist(गणना कालसारणी_प्राथमिकता कालसारणी)
{
	ध्रुव वर्ण *frequency = get_frequency(कालसारणी);
	वर्ण *name = launchctl_service_name(frequency);
	वर्ण *filename = launchctl_service_filename(name);
	पूर्णाङ्क परिणाम = launchctl_boot_plist(0, filename);
	unlink(filename);
	मुक्त(filename);
	मुक्त(name);
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क launchctl_remove_plists(शून्य)
{
	प्रत्यागम launchctl_remove_plist(कालसारणी_HOURLY) ||
	       launchctl_remove_plist(कालसारणी_DAILY) ||
	       launchctl_remove_plist(कालसारणी_WEEKLY);
}

स्थिर पूर्णाङ्क launchctl_list_अन्तर्भवति_plist(ध्रुव वर्ण *name, ध्रुव वर्ण *cmd)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	strvec_विभज(&child.तर्काः, cmd);
	strvec_pushl(&child.तर्काः, "list", name, रिक्त);

	child.no_stderr = 1;
	child.no_stdout = 1;

	यदि (start_command(&child))
		die(_("failed to start launchctl"));

	/* Returns failure if 'name' doesn't exist. */
	प्रत्यागम !finish_command(&child);
}

स्थिर पूर्णाङ्क launchctl_कालसारणी_plist(ध्रुव वर्ण *exec_मार्गः, गणना कालसारणी_प्राथमिकता कालसारणी)
{
	पूर्णाङ्क i, fd;
	ध्रुव वर्ण *preamble, *repeat;
	ध्रुव वर्ण *frequency = get_frequency(कालसारणी);
	वर्ण *name = launchctl_service_name(frequency);
	वर्ण *filename = launchctl_service_filename(name);
	संरचना तालकम्_file lk = तालकम्_INIT;
	स्थिर अचिह्नित दीर्घ तालकम्_file_timeout_ms = ULONG_MAX;
	संरचना strbuf plist = STRBUF_INIT, plist2 = STRBUF_INIT;
	संरचना stat st;
	वर्ण *cmd;
	पूर्णाङ्क minute = get_random_minute();

	get_कालसारणी_cmd("launchctl", रिक्त, &cmd);
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
	strbuf_addf(&plist, preamble, name, exec_मार्गः, exec_मार्गः,
		    get_extra_launchctl_वाक्यानि(), frequency);

	विकल्प (कालसारणी) {
	पक्ष कालसारणी_HOURLY:
		repeat = "<dict>\n"
			 "<key>Hour</key><integer>%d</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		प्रति (i = 1; i <= 23; i++)
			strbuf_addf(&plist, repeat, i, minute);
		भङ्ग;

	पक्ष कालसारणी_DAILY:
		repeat = "<dict>\n"
			 "<key>Weekday</key><integer>%d</integer>\n"
			 "<key>Hour</key><integer>0</integer>\n"
			 "<key>Minute</key><integer>%d</integer>\n"
			 "</dict>\n";
		प्रति (i = 1; i <= 6; i++)
			strbuf_addf(&plist, repeat, i, minute);
		भङ्ग;

	पक्ष कालसारणी_WEEKLY:
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

	यदि (safe_create_leading_directories(the_repository, filename))
		die(_("failed to create directories for '%s'"), filename);

	यदि ((दीर्घ)तालकम्_file_timeout_ms < 0 &&
	    repo_config_get_ulong(the_repository, "gc.launchctlplistlocktimeoutms",
				 &तालकम्_file_timeout_ms))
		तालकम्_file_timeout_ms = 150;

	fd = hold_तालकम्_file_for_update_timeout(&lk, filename, तालकम्_DIE_ON_ERROR,
					       तालकम्_file_timeout_ms);

	/*
	 * Does this file already exist? With the intended contents? Is it
	 * registered already? Then it does not need to be re-registered.
	 */
	यदि (!stat(filename, &st) && st.st_आकार == plist.len &&
	    strbuf_read_file(&plist2, filename, plist.len) == plist.len &&
	    !strbuf_cmp(&plist, &plist2) &&
	    launchctl_list_अन्तर्भवति_plist(name, cmd))
		rollback_तालकम्_file(&lk);
	अन्यथा {
		यदि (write_अन्तर्_पूर्णम्(fd, plist.buf, plist.len) < 0 ||
		    commit_तालकम्_file(&lk))
			die_errno(_("could not write '%s'"), filename);

		/* bootout might fail if not already running, so ignore */
		launchctl_boot_plist(0, filename);
		यदि (launchctl_boot_plist(1, filename))
			die(_("failed to bootstrap service %s"), filename);
	}

	मुक्त(filename);
	मुक्त(name);
	मुक्त(cmd);
	strbuf_release(&plist);
	strbuf_release(&plist2);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क launchctl_सङ्कलय_plists(शून्य)
{
	ध्रुव वर्ण *exec_मार्गः = git_exec_मार्गः();

	प्रत्यागम launchctl_कालसारणी_plist(exec_मार्गः, कालसारणी_HOURLY) ||
	       launchctl_कालसारणी_plist(exec_मार्गः, कालसारणी_DAILY) ||
	       launchctl_कालसारणी_plist(exec_मार्गः, कालसारणी_WEEKLY);
}

स्थिर पूर्णाङ्क launchctl_update_कालसारणी(पूर्णाङ्क सततम्_maintenance, पूर्णाङ्क fd अप्रयुक्तम्)
{
	यदि (सततम्_maintenance)
		प्रत्यागम launchctl_सङ्कलय_plists();
	अन्यथा
		प्रत्यागम launchctl_remove_plists();
}

स्थिर पूर्णाङ्क is_schtasks_उपलब्धम्(शून्य)
{
	पूर्णाङ्क is_उपलब्धम्;
	यदि (get_कालसारणी_cmd("schtasks", &is_उपलब्धम्, रिक्त))
		प्रत्यागम is_उपलब्धम्;

#यदि_परिभाषितम् GIT_WINDOWS_NATIVE
	प्रत्यागम 1;
#अन्यथा
	प्रत्यागम 0;
#यदि_अन्तः
}

स्थिर वर्ण *schtasks_task_name(ध्रुव वर्ण *frequency)
{
	संरचना strbuf label = STRBUF_INIT;
	strbuf_addf(&label, "Git Maintenance (%s)", frequency);
	प्रत्यागम strbuf_detach(&label, रिक्त);
}

स्थिर पूर्णाङ्क schtasks_remove_task(गणना कालसारणी_प्राथमिकता कालसारणी)
{
	वर्ण *cmd;
	संरचना child_process child = CHILD_PROCESS_INIT;
	ध्रुव वर्ण *frequency = get_frequency(कालसारणी);
	वर्ण *name = schtasks_task_name(frequency);

	get_कालसारणी_cmd("schtasks", रिक्त, &cmd);
	strvec_विभज(&child.तर्काः, cmd);
	strvec_pushl(&child.तर्काः, "/delete", "/tn", name, "/f", रिक्त);
	मुक्त(name);
	मुक्त(cmd);

	प्रत्यागम सततम्_command(&child);
}

स्थिर पूर्णाङ्क schtasks_remove_tasks(शून्य)
{
	प्रत्यागम schtasks_remove_task(कालसारणी_HOURLY) ||
	       schtasks_remove_task(कालसारणी_DAILY) ||
	       schtasks_remove_task(कालसारणी_WEEKLY);
}

स्थिर पूर्णाङ्क schtasks_कालसारणी_task(ध्रुव वर्ण *exec_मार्गः, गणना कालसारणी_प्राथमिकता कालसारणी)
{
	वर्ण *cmd;
	पूर्णाङ्क परिणाम;
	संरचना child_process child = CHILD_PROCESS_INIT;
	ध्रुव वर्ण *xml;
	संरचना tempfile *tfile;
	ध्रुव वर्ण *frequency = get_frequency(कालसारणी);
	वर्ण *name = schtasks_task_name(frequency);
	संरचना strbuf tfilename = STRBUF_INIT;
	पूर्णाङ्क minute = get_random_minute();

	get_कालसारणी_cmd("schtasks", रिक्त, &cmd);

	strbuf_addf(&tfilename, "%s/schedule_%s_XXXXXX",
		    repo_get_common_dir(the_repository), frequency);
	tfile = xmks_tempfile(tfilename.buf);
	strbuf_release(&tfilename);

	यदि (!fdopen_tempfile(tfile, "w"))
		die(_("failed to create temp xml file"));

	xml = "<?xml version=\"1.0\" ?>\n"
	      "<Task version=\"1.4\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
	      "<Triggers>\n"
	      "<CalendarTrigger>\n";
	fputs(xml, tfile->fp);

	विकल्प (कालसारणी) {
	पक्ष कालसारणी_HOURLY:
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

	पक्ष कालसारणी_DAILY:
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

	पक्ष कालसारणी_WEEKLY:
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
	fprintf(tfile->fp, xml, exec_मार्गः, exec_मार्गः,
		get_extra_config_parameters(), frequency);
	strvec_विभज(&child.तर्काः, cmd);
	strvec_pushl(&child.तर्काः, "/create", "/tn", name, "/f", "/xml",
				  get_tempfile_मार्गः(tfile), रिक्त);
	close_tempfile_gently(tfile);

	child.no_stdout = 1;
	child.no_stderr = 1;

	यदि (start_command(&child))
		die(_("failed to start schtasks"));
	परिणाम = finish_command(&child);

	delete_tempfile(&tfile);
	मुक्त(name);
	मुक्त(cmd);
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क schtasks_कालसारणी_tasks(शून्य)
{
	ध्रुव वर्ण *exec_मार्गः = git_exec_मार्गः();

	प्रत्यागम schtasks_कालसारणी_task(exec_मार्गः, कालसारणी_HOURLY) ||
	       schtasks_कालसारणी_task(exec_मार्गः, कालसारणी_DAILY) ||
	       schtasks_कालसारणी_task(exec_मार्गः, कालसारणी_WEEKLY);
}

स्थिर पूर्णाङ्क schtasks_update_कालसारणी(पूर्णाङ्क सततम्_maintenance, पूर्णाङ्क fd अप्रयुक्तम्)
{
	यदि (सततम्_maintenance)
		प्रत्यागम schtasks_कालसारणी_tasks();
	अन्यथा
		प्रत्यागम schtasks_remove_tasks();
}

MAYBE_अप्रयुक्तम्
स्थिर पूर्णाङ्क check_crontab_process(ध्रुव वर्ण *cmd)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	strvec_विभज(&child.तर्काः, cmd);
	strvec_प्रक्षिप(&child.तर्काः, "-l");
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	यदि (start_command(&child))
		प्रत्यागम 0;
	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&child);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क is_crontab_उपलब्धम्(शून्य)
{
	वर्ण *cmd;
	पूर्णाङ्क is_उपलब्धम्;
	पूर्णाङ्क ret;

	यदि (get_कालसारणी_cmd("crontab", &is_उपलब्धम्, &cmd)) {
		ret = is_उपलब्धम्;
		गच्छ बहिर्गमनम्;
	}

#यदि_परिभाषितम् __APPLE__
	/*
	 * macOS has cron, but it requires special permissions and will
	 * create a UI alert when attempting to run this command.
	 */
	ret = 0;
#अन्यथा
	ret = check_crontab_process(cmd);
#यदि_अन्तः

बहिर्गमनम्:
	मुक्त(cmd);
	प्रत्यागम ret;
}

#परिभाषय BEGIN_पङ्क्तिः "# BEGIN GIT MAINTENANCE SCHEDULE"
#परिभाषय अन्तः_पङ्क्तिः "# END GIT MAINTENANCE SCHEDULE"

स्थिर पूर्णाङ्क crontab_update_कालसारणी(पूर्णाङ्क सततम्_maintenance, पूर्णाङ्क fd)
{
	वर्ण *cmd;
	पूर्णाङ्क परिणाम = 0;
	पूर्णाङ्क अन्तर्_old_region = 0;
	संरचना child_process crontab_list = CHILD_PROCESS_INIT;
	संरचना child_process crontab_edit = CHILD_PROCESS_INIT;
	FILE *cron_list, *cron_अन्तर्;
	संरचना strbuf पङ्क्तिः = STRBUF_INIT;
	संरचना tempfile *tmpedit = रिक्त;
	पूर्णाङ्क minute = get_random_minute();

	get_कालसारणी_cmd("crontab", रिक्त, &cmd);
	strvec_विभज(&crontab_list.तर्काः, cmd);
	strvec_प्रक्षिप(&crontab_list.तर्काः, "-l");
	crontab_list.अन्तर् = -1;
	crontab_list.बहिर्गमनम् = dup(fd);
	crontab_list.git_cmd = 0;

	यदि (start_command(&crontab_list)) {
		परिणाम = error(_("failed to run 'crontab -l'; your system might not support 'cron'"));
		गच्छ बहिर्गमनम्;
	}

	/* Ignore exit code, as an empty crontab will return error. */
	finish_command(&crontab_list);

	tmpedit = mks_tempfile_t(".git_cron_edit_tmpXXXXXX");
	यदि (!tmpedit) {
		परिणाम = error(_("failed to create crontab temporary file"));
		गच्छ बहिर्गमनम्;
	}
	cron_अन्तर् = fdopen_tempfile(tmpedit, "w");
	यदि (!cron_अन्तर्) {
		परिणाम = error(_("failed to open temporary file"));
		गच्छ बहिर्गमनम्;
	}

	/*
	 * Read from the .lock file, filtering out the old
	 * schedule while appending the new schedule.
	 */
	cron_list = fdopen(fd, "r");
	rewind(cron_list);

	यावत् (!strbuf_getline_lf(&पङ्क्तिः, cron_list)) {
		यदि (!अन्तर्_old_region && !वाक्यं_तुलय(पङ्क्तिः.buf, BEGIN_पङ्क्तिः))
			अन्तर्_old_region = 1;
		अन्यथा यदि (अन्तर्_old_region && !वाक्यं_तुलय(पङ्क्तिः.buf, अन्तः_पङ्क्तिः))
			अन्तर्_old_region = 0;
		अन्यथा यदि (!अन्तर्_old_region)
			fprintf(cron_अन्तर्, "%s\n", पङ्क्तिः.buf);
	}
	strbuf_release(&पङ्क्तिः);

	यदि (सततम्_maintenance) {
		संरचना strbuf पङ्क्तिः_format = STRBUF_INIT;
		ध्रुव वर्ण *exec_मार्गः = git_exec_मार्गः();

		fprintf(cron_अन्तर्, "%s\n", BEGIN_पङ्क्तिः);
		fprintf(cron_अन्तर्,
			"# The following schedule was created by Git\n");
		fprintf(cron_अन्तर्, "# Any edits made in this region might be\n");
		fprintf(cron_अन्तर्,
			"# replaced in the future by a Git command.\n\n");

		strbuf_addf(&पङ्क्तिः_format,
			    "%%d %%s * * %%s \"%s/git\" --exec-path=\"%s\" %s for-each-repo --keep-going --config=maintenance.repo maintenance run --schedule=%%s\n",
			    exec_मार्गः, exec_मार्गः, get_extra_config_parameters());
		fprintf(cron_अन्तर्, पङ्क्तिः_format.buf, minute, "1-23", "*", "hourly");
		fprintf(cron_अन्तर्, पङ्क्तिः_format.buf, minute, "0", "1-6", "daily");
		fprintf(cron_अन्तर्, पङ्क्तिः_format.buf, minute, "0", "0", "weekly");
		strbuf_release(&पङ्क्तिः_format);

		fprintf(cron_अन्तर्, "\n%s\n", अन्तः_पङ्क्तिः);
	}

	fflush(cron_अन्तर्);

	strvec_विभज(&crontab_edit.तर्काः, cmd);
	strvec_प्रक्षिप(&crontab_edit.तर्काः, get_tempfile_मार्गः(tmpedit));
	crontab_edit.git_cmd = 0;

	यदि (start_command(&crontab_edit)) {
		परिणाम = error(_("failed to run 'crontab'; your system might not support 'cron'"));
		गच्छ बहिर्गमनम्;
	}

	यदि (finish_command(&crontab_edit))
		परिणाम = error(_("'crontab' died"));
	अन्यथा
		fclose(cron_list);

बहिर्गमनम्:
	delete_tempfile(&tmpedit);
	मुक्त(cmd);
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क real_is_systemd_timer_उपलब्धम्(शून्य)
{
	संरचना child_process child = CHILD_PROCESS_INIT;

	strvec_pushl(&child.तर्काः, "systemctl", "--user", "list-timers", रिक्त);
	child.no_stdin = 1;
	child.no_stdout = 1;
	child.no_stderr = 1;
	child.silent_exec_failure = 1;

	यदि (start_command(&child))
		प्रत्यागम 0;
	यदि (finish_command(&child))
		प्रत्यागम 0;
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क is_systemd_timer_उपलब्धम्(शून्य)
{
	पूर्णाङ्क is_उपलब्धम्;

	यदि (get_कालसारणी_cmd("systemctl", &is_उपलब्धम्, रिक्त))
		प्रत्यागम is_उपलब्धम्;

	प्रत्यागम real_is_systemd_timer_उपलब्धम्();
}

स्थिर वर्ण *xdg_config_home_systemd(ध्रुव वर्ण *filename)
{
	प्रत्यागम xdg_config_home_for("systemd/user", filename);
}

#परिभाषय SYSTEMD_UNIT_FORMAT "git-maintenance@%s.%s"

स्थिर पूर्णाङ्क systemd_timer_delete_timer_file(गणना कालसारणी_प्राथमिकता प्राथमिकता)
{
	पूर्णाङ्क ret = 0;
	ध्रुव वर्ण *frequency = get_frequency(प्राथमिकता);
	वर्ण *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");
	वर्ण *filename = xdg_config_home_systemd(local_timer_name);

	यदि (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	मुक्त(filename);
	मुक्त(local_timer_name);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क systemd_timer_delete_service_template(शून्य)
{
	पूर्णाङ्क ret = 0;
	वर्ण *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");
	वर्ण *filename = xdg_config_home_systemd(local_service_name);
	यदि (unlink(filename) && !is_missing_file_error(errno))
		ret = error_errno(_("failed to delete '%s'"), filename);

	मुक्त(filename);
	मुक्त(local_service_name);
	प्रत्यागम ret;
}

/*
 * Write the schedule information into a git-maintenance@<schedule>.timer
 * file using a custom minute. This timer file cannot use the templating
 * system, so we generate a specific file for each.
 */
स्थिर पूर्णाङ्क systemd_timer_write_timer_file(गणना कालसारणी_प्राथमिकता कालसारणी,
					  पूर्णाङ्क minute)
{
	पूर्णाङ्क res = -1;
	वर्ण *filename;
	FILE *file;
	ध्रुव वर्ण *unit;
	वर्ण *कालसारणी_आदर्शः = रिक्त;
	ध्रुव वर्ण *frequency = get_frequency(कालसारणी);
	वर्ण *local_timer_name = xstrfmt(SYSTEMD_UNIT_FORMAT, frequency, "timer");

	filename = xdg_config_home_systemd(local_timer_name);

	यदि (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		गच्छ error;
	}
	file = fopen_or_warn(filename, "w");
	यदि (!file)
		गच्छ error;

	विकल्प (कालसारणी) {
	पक्ष कालसारणी_HOURLY:
		कालसारणी_आदर्शः = xstrfmt("*-*-* 1..23:%02d:00", minute);
		भङ्ग;

	पक्ष कालसारणी_DAILY:
		कालसारणी_आदर्शः = xstrfmt("Tue..Sun *-*-* 0:%02d:00", minute);
		भङ्ग;

	पक्ष कालसारणी_WEEKLY:
		कालसारणी_आदर्शः = xstrfmt("Mon 0:%02d:00", minute);
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
	यदि (fprintf(file, unit, कालसारणी_आदर्शः) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		गच्छ error;
	}
	यदि (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		गच्छ error;
	}

	res = 0;

error:
	मुक्त(कालसारणी_आदर्शः);
	मुक्त(local_timer_name);
	मुक्त(filename);
	प्रत्यागम res;
}

/*
 * No matter the schedule, we use the same service and can make use of the
 * templating system. When installing git-maintenance@<schedule>.timer,
 * systemd will notice that git-maintenance@.service exists as a template
 * and will use this file and insert the <schedule> into the template at
 * the position of "%i".
 */
स्थिर पूर्णाङ्क systemd_timer_write_service_template(ध्रुव वर्ण *exec_मार्गः)
{
	पूर्णाङ्क res = -1;
	वर्ण *filename;
	FILE *file;
	ध्रुव वर्ण *unit;
	वर्ण *local_service_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "service");

	filename = xdg_config_home_systemd(local_service_name);
	यदि (safe_create_leading_directories(the_repository, filename)) {
		error(_("failed to create directories for '%s'"), filename);
		गच्छ error;
	}
	file = fopen_or_warn(filename, "w");
	यदि (!file)
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
	यदि (fprintf(file, unit, exec_मार्गः, exec_मार्गः, get_extra_config_parameters()) < 0) {
		error(_("failed to write to '%s'"), filename);
		fclose(file);
		गच्छ error;
	}
	यदि (fclose(file) == EOF) {
		error_errno(_("failed to flush '%s'"), filename);
		गच्छ error;
	}

	res = 0;

error:
	मुक्त(local_service_name);
	मुक्त(filename);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क systemd_timer_enable_unit(पूर्णाङ्क enable,
				     गणना कालसारणी_प्राथमिकता कालसारणी,
				     पूर्णाङ्क minute)
{
	वर्ण *cmd = रिक्त;
	संरचना child_process child = CHILD_PROCESS_INIT;
	ध्रुव वर्ण *frequency = get_frequency(कालसारणी);
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
	यदि (!enable) {
		child.no_stderr = 1;
	} अन्यथा यदि (systemd_timer_write_timer_file(कालसारणी, minute)) {
		ret = -1;
		गच्छ बहिर्गमनम्;
	}

	get_कालसारणी_cmd("systemctl", रिक्त, &cmd);
	strvec_विभज(&child.तर्काः, cmd);
	strvec_pushl(&child.तर्काः, "--user", enable ? "enable" : "disable",
		     "--now", रिक्त);
	strvec_pushf(&child.तर्काः, SYSTEMD_UNIT_FORMAT, frequency, "timer");

	यदि (start_command(&child)) {
		ret = error(_("failed to start systemctl"));
		गच्छ बहिर्गमनम्;
	}

	यदि (finish_command(&child)) {
		/*
		 * Disabling an already disabled systemd unit makes
		 * systemctl fail.
		 * Let's ignore this failure.
		 *
		 * Enabling an enabled systemd unit doesn't fail.
		 */
		यदि (enable) {
			ret = error(_("failed to run systemctl"));
			गच्छ बहिर्गमनम्;
		}
	}

	ret = 0;

बहिर्गमनम्:
	मुक्त(cmd);
	प्रत्यागम ret;
}

/*
 * A previous version of Git wrote the timer units as template files.
 * Clean these up, if they exist.
 */
स्थिर शून्य systemd_timer_delete_stale_timer_templates(शून्य)
{
	वर्ण *timer_template_name = xstrfmt(SYSTEMD_UNIT_FORMAT, "", "timer");
	वर्ण *filename = xdg_config_home_systemd(timer_template_name);

	यदि (unlink(filename) && !is_missing_file_error(errno))
		warning(_("failed to delete '%s'"), filename);

	मुक्त(filename);
	मुक्त(timer_template_name);
}

स्थिर पूर्णाङ्क systemd_timer_delete_unit_files(शून्य)
{
	systemd_timer_delete_stale_timer_templates();

	/* Purposefully not short-circuited to make sure all are called. */
	प्रत्यागम systemd_timer_delete_timer_file(कालसारणी_HOURLY) |
	       systemd_timer_delete_timer_file(कालसारणी_DAILY) |
	       systemd_timer_delete_timer_file(कालसारणी_WEEKLY) |
	       systemd_timer_delete_service_template();
}

स्थिर पूर्णाङ्क systemd_timer_delete_units(शून्य)
{
	पूर्णाङ्क minute = get_random_minute();
	/* Purposefully not short-circuited to make sure all are called. */
	प्रत्यागम systemd_timer_enable_unit(0, कालसारणी_HOURLY, minute) |
	       systemd_timer_enable_unit(0, कालसारणी_DAILY, minute) |
	       systemd_timer_enable_unit(0, कालसारणी_WEEKLY, minute) |
	       systemd_timer_delete_unit_files();
}

स्थिर पूर्णाङ्क systemd_timer_सज्जता_units(शून्य)
{
	पूर्णाङ्क minute = get_random_minute();
	ध्रुव वर्ण *exec_मार्गः = git_exec_मार्गः();

	पूर्णाङ्क ret = systemd_timer_write_service_template(exec_मार्गः) ||
		  systemd_timer_enable_unit(1, कालसारणी_HOURLY, minute) ||
		  systemd_timer_enable_unit(1, कालसारणी_DAILY, minute) ||
		  systemd_timer_enable_unit(1, कालसारणी_WEEKLY, minute);

	यदि (ret)
		systemd_timer_delete_units();
	अन्यथा
		systemd_timer_delete_stale_timer_templates();

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क systemd_timer_update_कालसारणी(पूर्णाङ्क सततम्_maintenance, पूर्णाङ्क fd अप्रयुक्तम्)
{
	यदि (सततम्_maintenance)
		प्रत्यागम systemd_timer_सज्जता_units();
	अन्यथा
		प्रत्यागम systemd_timer_delete_units();
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
	पूर्णाङ्क (*is_उपलब्धम्)(शून्य);
	पूर्णाङ्क (*update_कालसारणी)(पूर्णाङ्क सततम्_maintenance, पूर्णाङ्क fd);
} scheduler_fn[] = {
	[SCHEDULER_CRON] = {
		.name = "crontab",
		.is_उपलब्धम् = is_crontab_उपलब्धम्,
		.update_कालसारणी = crontab_update_कालसारणी,
	},
	[SCHEDULER_SYSTEMD] = {
		.name = "systemctl",
		.is_उपलब्धम् = is_systemd_timer_उपलब्धम्,
		.update_कालसारणी = systemd_timer_update_कालसारणी,
	},
	[SCHEDULER_LAUNCHCTL] = {
		.name = "launchctl",
		.is_उपलब्धम् = is_launchctl_उपलब्धम्,
		.update_कालसारणी = launchctl_update_कालसारणी,
	},
	[SCHEDULER_SCHTASKS] = {
		.name = "schtasks",
		.is_उपलब्धम् = is_schtasks_उपलब्धम्,
		.update_कालसारणी = schtasks_update_कालसारणी,
	},
};

स्थिर गणना scheduler parse_scheduler(ध्रुव वर्ण *मानम्)
{
	यदि (!मानम्)
		प्रत्यागम SCHEDULER_INVALID;
	अन्यथा यदि (!strcasecmp(मानम्, "auto"))
		प्रत्यागम SCHEDULER_AUTO;
	अन्यथा यदि (!strcasecmp(मानम्, "cron") || !strcasecmp(मानम्, "crontab"))
		प्रत्यागम SCHEDULER_CRON;
	अन्यथा यदि (!strcasecmp(मानम्, "systemd") ||
		 !strcasecmp(मानम्, "systemd-timer"))
		प्रत्यागम SCHEDULER_SYSTEMD;
	अन्यथा यदि (!strcasecmp(मानम्, "launchctl"))
		प्रत्यागम SCHEDULER_LAUNCHCTL;
	अन्यथा यदि (!strcasecmp(मानम्, "schtasks"))
		प्रत्यागम SCHEDULER_SCHTASKS;
	अन्यथा
		प्रत्यागम SCHEDULER_INVALID;
}

स्थिर पूर्णाङ्क maintenance_opt_scheduler(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				     पूर्णाङ्क unset)
{
	गणना scheduler *scheduler = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	*scheduler = parse_scheduler(arg);
	यदि (*scheduler == SCHEDULER_INVALID)
		प्रत्यागम error(_("unrecognized --scheduler argument '%s'"), arg);
	प्रत्यागम 0;
}

संरचना maintenance_start_opts {
	गणना scheduler scheduler;
};

स्थिर गणना scheduler resolve_scheduler(गणना scheduler scheduler)
{
	यदि (scheduler != SCHEDULER_AUTO)
		प्रत्यागम scheduler;

#यदि defined(__APPLE__)
	प्रत्यागम SCHEDULER_LAUNCHCTL;

#नोचेद्यदि defined(GIT_WINDOWS_NATIVE)
	प्रत्यागम SCHEDULER_SCHTASKS;

#नोचेद्यदि defined(__linux__)
	यदि (is_systemd_timer_उपलब्धम्())
		प्रत्यागम SCHEDULER_SYSTEMD;
	अन्यथा यदि (is_crontab_उपलब्धम्())
		प्रत्यागम SCHEDULER_CRON;
	अन्यथा
		die(_("neither systemd timers nor crontab are available"));

#अन्यथा
	प्रत्यागम SCHEDULER_CRON;
#यदि_अन्तः
}

स्थिर शून्य validate_scheduler(गणना scheduler scheduler)
{
	यदि (scheduler == SCHEDULER_INVALID)
		BUG("invalid scheduler");
	यदि (scheduler == SCHEDULER_AUTO)
		BUG("resolve_scheduler should have been called before");

	यदि (!scheduler_fn[scheduler].is_उपलब्धम्())
		die(_("%s scheduler is not available"),
		    scheduler_fn[scheduler].name);
}

स्थिर पूर्णाङ्क update_background_कालसारणी(ध्रुव संरचना maintenance_start_opts *opts,
				      पूर्णाङ्क enable)
{
	अचिह्नित पूर्णाङ्क i;
	पूर्णाङ्क परिणाम = 0;
	संरचना तालकम्_file lk;
	वर्ण *तालकम्_मार्गः = xstrfmt("%s/schedule", the_repository->objects->source->मार्गः);

	यदि (hold_तालकम्_file_for_update(&lk, तालकम्_मार्गः, तालकम्_NO_DEREF) < 0) {
		यदि (errno == EEXIST)
			error(_("unable to create '%s.lock': %s.\n\n"
			    "Another scheduled git-maintenance(1) process seems to be running in this\n"
			    "repository. Please make sure no other maintenance processes are running and\n"
			    "then try again. If it still fails, a git-maintenance(1) process may have\n"
			    "crashed in this repository earlier: remove the file manually to continue."),
			    absolute_मार्गः(तालकम्_मार्गः), strerror(errno));
		अन्यथा
			error_errno(_("cannot acquire lock for scheduled background maintenance"));
		मुक्त(तालकम्_मार्गः);
		प्रत्यागम -1;
	}

	प्रति (i = 1; i < ARRAY_आकार(scheduler_fn); i++) {
		यदि (enable && opts->scheduler == i)
			अनुवर्त;
		यदि (!scheduler_fn[i].is_उपलब्धम्())
			अनुवर्त;
		scheduler_fn[i].update_कालसारणी(0, get_तालकम्_file_fd(&lk));
	}

	यदि (enable)
		परिणाम = scheduler_fn[opts->scheduler].update_कालसारणी(
			1, get_तालकम्_file_fd(&lk));

	rollback_तालकम्_file(&lk);

	मुक्त(तालकम्_मार्गः);
	प्रत्यागम परिणाम;
}

स्थिर ध्रुव वर्ण *ध्रुव builtin_maintenance_start_usage[] = {
	N_("git maintenance start [--scheduler=<scheduler>]"),
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_start(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			     संरचना repository *repo)
{
	संरचना maintenance_start_opts opts = { 0 };
	संरचना option options[] = {
		OPT_CALLBACK_F(
			0, "scheduler", &opts.scheduler, N_("scheduler"),
			N_("scheduler to trigger git maintenance run"),
			PARSE_OPT_NONEG, maintenance_opt_scheduler),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *पञ्जिकाकोशः_तर्काः[] = { "register", रिक्त };

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_maintenance_start_usage, 0);
	यदि (argc)
		usage_with_options(builtin_maintenance_start_usage, options);

	opts.scheduler = resolve_scheduler(opts.scheduler);
	validate_scheduler(opts.scheduler);

	यदि (update_background_कालसारणी(&opts, 1))
		die(_("failed to set up maintenance schedule"));

	यदि (maintenance_पञ्जिकाकोशः(ARRAY_आकार(पञ्जिकाकोशः_तर्काः)-1, पञ्जिकाकोशः_तर्काः, रिक्त, repo))
		warning(_("failed to add repo to global config"));
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *ध्रुव builtin_maintenance_stop_usage[] = {
	"git maintenance stop",
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_stop(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			    संरचना repository *repo अप्रयुक्तम्)
{
	संरचना option options[] = {
		OPT_अन्तः()
	};
	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_maintenance_stop_usage, 0);
	यदि (argc)
		usage_with_options(builtin_maintenance_stop_usage, options);
	प्रत्यागम update_background_कालसारणी(रिक्त, 0);
}

स्थिर ध्रुव वर्ण *ध्रुव builtin_maintenance_is_needed_usage[] = {
	"git maintenance is-needed [--task=<task>] [--schedule]",
	रिक्त
};

स्थिर पूर्णाङ्क maintenance_is_needed(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				 संरचना repository *repo अप्रयुक्तम्)
{
	संरचना maintenance_सततम्_opts opts = MAINTENANCE_सततम्_OPTS_INIT;
	संरचना string_list selected_tasks = STRING_LIST_INIT_DUP;
	संरचना gc_config cfg = GC_CONFIG_INIT;
	संरचना option options[] = {
		OPT_BOOL(0, "auto", &opts.auto_flag,
			 N_("run tasks based on the state of the repository")),
		OPT_CALLBACK_F(0, "task", &selected_tasks, N_("task"),
			       N_("check a specific task"),
			       PARSE_OPT_NONEG, task_option_parse),
		OPT_अन्तः()
	};
	bool is_needed = false;

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_maintenance_is_needed_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	यदि (argc)
		usage_with_options(builtin_maintenance_is_needed_usage, options);

	gc_config(&cfg);
	initialize_task_config(&opts, &selected_tasks);

	यदि (opts.auto_flag) {
		प्रति (आकार_t i = 0; i < opts.tasks_nr; i++) {
			यदि (tasks[opts.tasks[i]].auto_condition &&
			    tasks[opts.tasks[i]].auto_condition(&cfg)) {
				is_needed = true;
				भङ्ग;
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
	maintenance_सततम्_opts_release(&opts);
	gc_config_release(&cfg);

	यदि (is_needed)
		प्रत्यागम 0;
	प्रत्यागम 1;
}

स्थिर ध्रुव वर्ण *ध्रुव builtin_maintenance_usage[] = {
	N_("git maintenance <subcommand> [<options>]"),
	रिक्त,
};

पूर्णाङ्क cmd_maintenance(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *उपसर्गः,
		    संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option builtin_maintenance_options[] = {
		OPT_SUBCOMMAND("run", &fn, maintenance_सततम्),
		OPT_SUBCOMMAND("start", &fn, maintenance_start),
		OPT_SUBCOMMAND("stop", &fn, maintenance_stop),
		OPT_SUBCOMMAND("register", &fn, maintenance_पञ्जिकाकोशः),
		OPT_SUBCOMMAND("unregister", &fn, maintenance_unregister),
		OPT_SUBCOMMAND("is-needed", &fn, maintenance_is_needed),
		OPT_अन्तः(),
	};

	argc = parse_options(argc, argv, उपसर्गः, builtin_maintenance_options,
			     builtin_maintenance_usage, 0);
	प्रत्यागम fn(argc, argv, उपसर्गः, repo);
}
