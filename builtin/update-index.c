/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/transaction.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "resolve-undo.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "pathspec.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "read-cache.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "split-index.h"
#అన్తర్భావ "symlinks.h"
#అన్తర్భావ "fsmonitor.h"
#అన్తర్భావ "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
స్థిర పూర్ణాంకం allow_కూడు;
స్థిర పూర్ణాంకం allow_remove;
స్థిర పూర్ణాంకం allow_replace;
స్థిర పూర్ణాంకం info_only;
స్థిర పూర్ణాంకం force_remove;
స్థిర పూర్ణాంకం verbose;
స్థిర పూర్ణాంకం mark_valid_only;
స్థిర పూర్ణాంకం mark_skip_worktree_only;
స్థిర పూర్ణాంకం mark_fsmonitor_only;
స్థిర పూర్ణాంకం ignore_skip_worktree_entries;
#నిర్వచించు MARK_FLAG 1
#నిర్వచించు UNMARK_FLAG 2
స్థిర నిర్మాణం strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
గణన uc_బహుళకం {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (రాయి, 1, 2)))
స్థిర శూన్య report(స్థిరరాశి అక్షరం *fmt, ...)
{
	va_list vp;

	ఒకవేళ (!verbose)
		తిరిగి;

	va_ప్రారంభం(vp, fmt);
	vprintf(fmt, vp);
	అక్షరం_రాయి('\n');
	va_చివర(vp);
}

స్థిర శూన్య remove_test_directory(శూన్య)
{
	ఒకవేళ (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

స్థిర స్థిరరాశి అక్షరం *get_mtime_మార్గం(స్థిరరాశి అక్షరం *మార్గం)
{
	స్థిర నిర్మాణం strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, మార్గం);
	తిరిగి sb.buf;
}

స్థిర శూన్య xmkdir(స్థిరరాశి అక్షరం *మార్గం)
{
	మార్గం = get_mtime_మార్గం(మార్గం);
	ఒకవేళ (mkdir(మార్గం, 0700))
		die_errno(_("failed to create directory %s"), మార్గం);
}

స్థిర పూర్ణాంకం xstat_mtime_dir(నిర్మాణం stat *st)
{
	ఒకవేళ (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం create_file(స్థిరరాశి అక్షరం *మార్గం)
{
	పూర్ణాంకం fd;
	మార్గం = get_mtime_మార్గం(మార్గం);
	fd = xopen(మార్గం, O_CREAT | O_RDWR, 0644);
	తిరిగి fd;
}

స్థిర శూన్య xunlink(స్థిరరాశి అక్షరం *మార్గం)
{
	మార్గం = get_mtime_మార్గం(మార్గం);
	ఒకవేళ (unlink(మార్గం))
		die_errno(_("failed to delete file %s"), మార్గం);
}

స్థిర శూన్య xrmdir(స్థిరరాశి అక్షరం *మార్గం)
{
	మార్గం = get_mtime_మార్గం(మార్గం);
	ఒకవేళ (rmdir(మార్గం))
		die_errno(_("failed to delete directory %s"), మార్గం);
}

స్థిర శూన్య avoid_racy(శూన్య)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

స్థిర పూర్ణాంకం test_if_untracked_cache_is_supported(శూన్య)
{
	నిర్మాణం stat st;
	నిర్మాణం stat_దత్తాంశం పునాది;
	పూర్ణాంకం fd, ret = 0;
	అక్షరం *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	ఒకవేళ (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	విడుదల(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_దత్తాంశం(&పునాది, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	ఒకవేళ (!సరిపోలిక_stat_దత్తాంశం(&పునాది, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		వెళ్ళు పూర్తి;
	}
	fill_stat_దత్తాంశం(&పునాది, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	ఒకవేళ (!సరిపోలిక_stat_దత్తాంశం(&పునాది, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		వెళ్ళు పూర్తి;
	}
	fill_stat_దత్తాంశం(&పునాది, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	ఒకవేళ (సరిపోలిక_stat_దత్తాంశం(&పునాది, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		వెళ్ళు పూర్తి;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	ఒకవేళ (సరిపోలిక_stat_దత్తాంశం(&పునాది, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		వెళ్ళు పూర్తి;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	ఒకవేళ (!సరిపోలిక_stat_దత్తాంశం(&పునాది, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		వెళ్ళు పూర్తి;
	}
	fill_stat_దత్తాంశం(&పునాది, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	ఒకవేళ (!సరిపోలిక_stat_దత్తాంశం(&పునాది, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

పూర్తి:
	strbuf_release(&mtime_dir);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం mark_ce_flags(స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం flag, పూర్ణాంకం mark)
{
	పూర్ణాంకం namelen = అక్షరశ్రేణి_పొడవు(మార్గం);
	పూర్ణాంకం pos = సూచీ_name_pos(the_repository->సూచీ, మార్గం, namelen);
	ఒకవేళ (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->సూచీ, the_repository->సూచీ->cache[pos]);
		ఒకవేళ (mark)
			the_repository->సూచీ->cache[pos]->ce_flags |= flag;
		లేకపోతే
			the_repository->సూచీ->cache[pos]->ce_flags &= ~flag;
		the_repository->సూచీ->cache[pos]->ce_flags |= CE_UPDATE_లోపల_పునాది;
		cache_వృక్షం_invalidate_మార్గం(the_repository->సూచీ, మార్గం);
		the_repository->సూచీ->cache_మారింది |= CE_నమోదు_మారింది;
		తిరిగి 0;
	}
	తిరిగి -1;
}

స్థిర పూర్ణాంకం remove_one_మార్గం(స్థిరరాశి అక్షరం *మార్గం)
{
	ఒకవేళ (!allow_remove)
		తిరిగి error("%s: does not exist and --remove not passed", మార్గం);
	ఒకవేళ (remove_file_from_సూచీ(the_repository->సూచీ, మార్గం))
		తిరిగి error("%s: cannot remove from the index", మార్గం);
	తిరిగి 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
స్థిర పూర్ణాంకం process_lstat_error(స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం err)
{
	ఒకవేళ (is_missing_file_error(err))
		తిరిగి remove_one_మార్గం(మార్గం);
	తిరిగి error("lstat(\"%s\"): %s", మార్గం, strerror(err));
}

స్థిర పూర్ణాంకం కూడు_one_మార్గం(స్థిరరాశి నిర్మాణం cache_నమోదు *old, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం len, నిర్మాణం stat *st)
{
	పూర్ణాంకం option;
	నిర్మాణం cache_నమోదు *ce;

	/* Was the old index entry already up-to-date? */
	ఒకవేళ (old && !ce_stage(old) && !ie_సరిపోలిక_stat(the_repository->సూచీ, old, st, 0))
		తిరిగి 0;

	ce = make_ఖాళీలు_cache_నమోదు(the_repository->సూచీ, len);
	జ్ఞాపకం_నకలు(ce->name, మార్గం, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->సూచీ, ce, st);
	ce->ce_బహుళకం = ce_బహుళకం_from_stat(the_repository, old, st->st_బహుళకం);

	ఒకవేళ (సూచీ_మార్గం(the_repository->సూచీ, &ce->oid, మార్గం, st,
		       info_only ? 0 : సూచీ_WRITE_OBJECT)) {
		discard_cache_నమోదు(ce);
		తిరిగి -1;
	}
	option = allow_కూడు ? కూడు_CACHE_OK_గమ్యం_కూడు : 0;
	option |= allow_replace ? కూడు_CACHE_OK_గమ్యం_REPLACE : 0;
	ఒకవేళ (కూడు_సూచీ_నమోదు(the_repository->సూచీ, ce, option)) {
		discard_cache_నమోదు(ce);
		తిరిగి error("%s: cannot add to the index - missing --add option?", మార్గం);
	}
	తిరిగి 0;
}

/*
 * Handle a path that was a directory. Four cases:
 *
 *  - it's already a gitlink in the index, and we keep it that
 *    way, and update it if we can (if we cannot find the HEAD,
 *    we're going to keep it unchanged in the index!)
 *
 *  - it's a *file* in the index, in which case it should be
 *    removed as a file if removal is allowed, since it doesn't
 *    exist as such any more. If removal isn't allowed, it's
 *    an error.
 *
 *    (NOTE! This is old and arguably fairly strange behaviour.
 *    We might want to make this an error unconditionally, and
 *    use "--force-remove" if you actually want to force removal).
 *
 *  - it used to exist as a subdirectory (ie multiple files with
 *    this particular prefix) in the index, in which case it's wrong
 *    to try to update it as a directory.
 *
 *  - it doesn't exist at all in the index, but it is a valid
 *    git directory, and it should be *added* as a gitlink.
 */
స్థిర పూర్ణాంకం process_directory(స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం len, నిర్మాణం stat *st)
{
	నిర్మాణం object_id oid;
	పూర్ణాంకం pos = సూచీ_name_pos(the_repository->సూచీ, మార్గం, len);

	/* Exact match: file or existing gitlink */
	ఒకవేళ (pos >= 0) {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[pos];
		ఒకవేళ (S_ISGITLINK(ce->ce_బహుళకం)) {

			/* Do nothing to the index if there is no HEAD! */
			ఒకవేళ (repo_resolve_gitlink_ref(the_repository, మార్గం,
						     "HEAD", &oid) < 0)
				తిరిగి 0;

			తిరిగి కూడు_one_మార్గం(ce, మార్గం, len, st);
		}
		/* Should this be an unconditional error? */
		తిరిగి remove_one_మార్గం(మార్గం);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	అయితే (pos < the_repository->సూచీ->cache_nr) {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[pos++];

		ఒకవేళ (అక్షరశ్రేణి_పోలిక(ce->name, మార్గం, len))
			ఆపు;
		ఒకవేళ (ce->name[len] > '/')
			ఆపు;
		ఒకవేళ (ce->name[len] < '/')
			కొనసాగు;

		/* Subdirectory match - error out */
		తిరిగి error("%s: is a directory - add individual files instead", మార్గం);
	}

	/* No match - should we add it as a gitlink? */
	ఒకవేళ (!repo_resolve_gitlink_ref(the_repository, మార్గం, "HEAD", &oid))
		తిరిగి కూడు_one_మార్గం(ఖాళీ, మార్గం, len, st);

	/* Error out. */
	తిరిగి error("%s: is a directory - add files inside instead", మార్గం);
}

స్థిర పూర్ణాంకం process_మార్గం(స్థిరరాశి అక్షరం *మార్గం, నిర్మాణం stat *st, పూర్ణాంకం stat_errno)
{
	పూర్ణాంకం pos, len;
	స్థిరరాశి నిర్మాణం cache_నమోదు *ce;

	len = అక్షరశ్రేణి_పొడవు(మార్గం);
	ఒకవేళ (has_symlink_leading_మార్గం(మార్గం, len))
		తిరిగి error("'%s' is beyond a symbolic link", మార్గం);

	pos = సూచీ_name_pos(the_repository->సూచీ, మార్గం, len);
	ce = pos < 0 ? ఖాళీ : the_repository->సూచీ->cache[pos];
	ఒకవేళ (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		ఒకవేళ (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_సూచీ(the_repository->సూచీ, మార్గం))
			తిరిగి error("%s: cannot remove from the index", మార్గం);
		తిరిగి 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	ఒకవేళ (stat_errno)
		తిరిగి process_lstat_error(మార్గం, stat_errno);

	ఒకవేళ (S_ISDIR(st->st_బహుళకం))
		తిరిగి process_directory(మార్గం, len, st);

	తిరిగి కూడు_one_మార్గం(ce, మార్గం, len, st);
}

స్థిర పూర్ణాంకం కూడు_cacheinfo(గుర్తులేని పూర్ణాంకం బహుళకం, స్థిరరాశి నిర్మాణం object_id *oid,
			 స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం stage)
{
	పూర్ణాంకం len, option;
	నిర్మాణం cache_నమోదు *ce;

	ఒకవేళ (!verify_మార్గం(మార్గం, బహుళకం))
		తిరిగి error("Invalid path '%s'", మార్గం);

	len = అక్షరశ్రేణి_పొడవు(మార్గం);
	ce = make_ఖాళీలు_cache_నమోదు(the_repository->సూచీ, len);

	oidcpy(&ce->oid, oid);
	జ్ఞాపకం_నకలు(ce->name, మార్గం, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_బహుళకం = create_ce_బహుళకం(బహుళకం);
	ఒకవేళ (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_కూడు ? కూడు_CACHE_OK_గమ్యం_కూడు : 0;
	option |= allow_replace ? కూడు_CACHE_OK_గమ్యం_REPLACE : 0;
	ఒకవేళ (కూడు_సూచీ_నమోదు(the_repository->సూచీ, ce, option))
		తిరిగి error("%s: cannot add to the index - missing --add option?",
			     మార్గం);
	report("add '%s'", మార్గం);
	తిరిగి 0;
}

స్థిర శూన్య chmod_మార్గం(అక్షరం flip, స్థిరరాశి అక్షరం *మార్గం)
{
	పూర్ణాంకం pos;
	నిర్మాణం cache_నమోదు *ce;

	pos = సూచీ_name_pos(the_repository->సూచీ, మార్గం, అక్షరశ్రేణి_పొడవు(మార్గం));
	ఒకవేళ (pos < 0)
		వెళ్ళు fail;
	ce = the_repository->సూచీ->cache[pos];
	ఒకవేళ (chmod_సూచీ_నమోదు(the_repository->సూచీ, ce, flip) < 0)
		వెళ్ళు fail;

	report("chmod %cx '%s'", flip, మార్గం);
	తిరిగి;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, మార్గం);
}

స్థిర శూన్య update_one(స్థిరరాశి అక్షరం *మార్గం)
{
	పూర్ణాంకం stat_errno = 0;
	నిర్మాణం stat st;

	ఒకవేళ (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_బహుళకం = 0;
	లేకపోతే ఒకవేళ (lstat(మార్గం, &st) < 0) {
		st.st_బహుళకం = 0;
		stat_errno = errno;
	} /* else stat is valid */

	ఒకవేళ (!verify_మార్గం(మార్గం, st.st_బహుళకం)) {
		fprintf(stderr, "Ignoring path %s\n", మార్గం);
		తిరిగి;
	}
	ఒకవేళ (mark_valid_only) {
		ఒకవేళ (mark_ce_flags(మార్గం, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", మార్గం);
		తిరిగి;
	}
	ఒకవేళ (mark_skip_worktree_only) {
		ఒకవేళ (mark_ce_flags(మార్గం, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", మార్గం);
		తిరిగి;
	}
	ఒకవేళ (mark_fsmonitor_only) {
		ఒకవేళ (mark_ce_flags(మార్గం, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", మార్గం);
		తిరిగి;
	}

	ఒకవేళ (force_remove) {
		ఒకవేళ (remove_file_from_సూచీ(the_repository->సూచీ, మార్గం))
			die("git update-index: unable to remove %s", మార్గం);
		report("remove '%s'", మార్గం);
		తిరిగి;
	}
	ఒకవేళ (process_మార్గం(మార్గం, &st, stat_errno))
		die("Unable to process path %s", మార్గం);
	report("add '%s'", మార్గం);
}

స్థిర శూన్య read_సూచీ_info(పూర్ణాంకం nul_పదం_పంక్తి)
{
	స్థిరరాశి పూర్ణాంకం hexsz = the_hash_algo->hexsz;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_పదం_పంక్తి ? strbuf_getline_nul : strbuf_getline_lf;
	అయితే (getline_fn(&buf, stdin) != EOF) {
		అక్షరం *ptr, *tab;
		అక్షరం *మార్గం_name;
		నిర్మాణం object_id oid;
		గుర్తులేని పూర్ణాంకం బహుళకం;
		గుర్తులేని దీర్ఘ ul;
		పూర్ణాంకం stage;

		/* This reads lines formatted in one of three formats:
		 *
		 * (1) mode         SP sha1          TAB path
		 * The first format is what "git apply --index-info"
		 * reports, and used to reconstruct a partial tree
		 * that is used for phony merge base tree when falling
		 * back on 3-way merge.
		 *
		 * (2) mode SP type SP sha1          TAB path
		 * The second format is to stuff "git ls-tree" output
		 * into the index file.
		 *
		 * (3) mode         SP sha1 SP stage TAB path
		 * This format is to put higher order stages into the
		 * index file and matches "git ls-files --stage" output.
		 */
		errno = 0;
		ul = strtoul(buf.buf, &ptr, 8);
		ఒకవేళ (ptr == buf.buf || *ptr != ' '
		    || errno || (గుర్తులేని పూర్ణాంకం) ul != ul)
			వెళ్ళు bad_పంక్తి;
		బహుళకం = ul;

		tab = strchr(ptr, '\t');
		ఒకవేళ (!tab || tab - ptr < hexsz + 1)
			వెళ్ళు bad_పంక్తి;

		ఒకవేళ (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		లేకపోతే {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		ఒకవేళ (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			వెళ్ళు bad_పంక్తి;

		మార్గం_name = ptr;
		ఒకవేళ (!nul_పదం_పంక్తి && మార్గం_name[0] == '"') {
			strbuf_reset(&uq);
			ఒకవేళ (unquote_c_style(&uq, మార్గం_name, ఖాళీ)) {
				die("git update-index: bad quoting of path name");
			}
			మార్గం_name = uq.buf;
		}

		ఒకవేళ (!verify_మార్గం(మార్గం_name, బహుళకం)) {
			fprintf(stderr, "Ignoring path %s\n", మార్గం_name);
			కొనసాగు;
		}

		ఒకవేళ (!బహుళకం) {
			/* mode == 0 means there is no such path -- remove */
			ఒకవేళ (remove_file_from_సూచీ(the_repository->సూచీ, మార్గం_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		లేకపోతే {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			ఒకవేళ (కూడు_cacheinfo(బహుళకం, &oid, మార్గం_name, stage))
				die("git update-index: unable to update %s",
				    మార్గం_name);
		}
		కొనసాగు;

	bad_పంక్తి:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి update_సూచీ_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	ఖాళీ
};

స్థిర నిర్మాణం cache_నమోదు *read_one_ent(స్థిరరాశి అక్షరం *which,
					నిర్మాణం object_id *ent, స్థిరరాశి అక్షరం *మార్గం,
					పూర్ణాంకం namelen, పూర్ణాంకం stage)
{
	గుర్తులేని పొట్టి బహుళకం;
	నిర్మాణం object_id oid;
	నిర్మాణం cache_నమోదు *ce;

	ఒకవేళ (get_వృక్షం_నమోదు(the_repository, ent, మార్గం, &oid, &బహుళకం)) {
		ఒకవేళ (which)
			error("%s: not in %s branch.", మార్గం, which);
		తిరిగి ఖాళీ;
	}
	ఒకవేళ (!the_repository->సూచీ->sparse_సూచీ && బహుళకం == S_IFDIR) {
		ఒకవేళ (which)
			error("%s: not a blob in %s branch.", మార్గం, which);
		తిరిగి ఖాళీ;
	}
	ce = make_ఖాళీలు_cache_నమోదు(the_repository->సూచీ, namelen);

	oidcpy(&ce->oid, &oid);
	జ్ఞాపకం_నకలు(ce->name, మార్గం, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_బహుళకం = create_ce_బహుళకం(బహుళకం);
	తిరిగి ce;
}

స్థిర పూర్ణాంకం unresolve_one(స్థిరరాశి అక్షరం *మార్గం)
{
	నిర్మాణం string_list_అంశం *అంశం;
	పూర్ణాంకం res = 0;

	ఒకవేళ (!the_repository->సూచీ->resolve_undo)
		తిరిగి res;
	అంశం = string_list_lookup(the_repository->సూచీ->resolve_undo, మార్గం);
	ఒకవేళ (!అంశం)
		తిరిగి res; /* no resolve-undo record for the path */
	res = unmerge_సూచీ_నమోదు(the_repository->సూచీ, మార్గం, అంశం->util, 0);
	FREE_AND_NULL(అంశం->util);
	తిరిగి res;
}

స్థిర పూర్ణాంకం do_unresolve(పూర్ణాంకం ac, స్థిరరాశి అక్షరం **av,
			స్థిరరాశి అక్షరం *ఉపసర్గ, పూర్ణాంకం ఉపసర్గ_నిడివి)
{
	పూర్ణాంకం i;
	పూర్ణాంకం err = 0;

	కొరకు (i = 1; i < ac; i++) {
		స్థిరరాశి అక్షరం *arg = av[i];
		అక్షరం *p = ఉపసర్గ_మార్గం(the_repository, ఉపసర్గ, ఉపసర్గ_నిడివి, arg);
		err |= unresolve_one(p);
		విడుదల(p);
	}
	తిరిగి err;
}

స్థిర పూర్ణాంకం do_reupdate(స్థిరరాశి అక్షరం **paths,
		       స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	పూర్ణాంకం pos;
	పూర్ణాంకం has_తల = 1;
	నిర్మాణం pathspec pathspec;
	నిర్మాణం object_id తల_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ఉపసర్గ, paths);

	ఒకవేళ (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &తల_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_తల = 0;
 redo:
	కొరకు (pos = 0; pos < the_repository->సూచీ->cache_nr; pos++) {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[pos];
		నిర్మాణం cache_నమోదు *old = ఖాళీ;
		పూర్ణాంకం save_nr;
		అక్షరం *మార్గం;

		ఒకవేళ (ce_stage(ce) || !ce_మార్గం_సరిపోలిక(the_repository->సూచీ, ce, &pathspec, ఖాళీ))
			కొనసాగు;
		ఒకవేళ (has_తల)
			old = read_one_ent(ఖాళీ, &తల_oid,
					   ce->name, ce_namelen(ce), 0);
		ఒకవేళ (old && ce->ce_బహుళకం == old->ce_బహుళకం &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_నమోదు(old);
			కొనసాగు; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		ఒకవేళ (S_ISSPARSEDIR(ce->ce_బహుళకం)) {
			ensure_నిండిన_సూచీ(the_repository->సూచీ);
			వెళ్ళు redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->సూచీ->cache_nr;
		మార్గం = xstrdup(ce->name);
		update_one(మార్గం);
		విడుదల(మార్గం);
		discard_cache_నమోదు(old);
		ఒకవేళ (save_nr != the_repository->సూచీ->cache_nr)
			వెళ్ళు redo;
	}
	clear_pathspec(&pathspec);
	తిరిగి 0;
}

నిర్మాణం refresh_params {
	గుర్తులేని పూర్ణాంకం flags;
	పూర్ణాంకం *has_errors;
};

స్థిర పూర్ణాంకం refresh(నిర్మాణం refresh_params *o, గుర్తులేని పూర్ణాంకం flag)
{
	అమరిక_work_వృక్షం(the_repository);
	repo_read_సూచీ(the_repository);
	*o->has_errors |= refresh_సూచీ(the_repository->సూచీ, o->flags | flag, ఖాళీ,
					ఖాళీ, ఖాళీ);
	ఒకవేళ (has_racy_timestamp(the_repository->సూచీ)) {
		/*
		 * Even if nothing else has changed, updating the file
		 * increases the chance that racy timestamps become
		 * non-racy, helping future run-time performance.
		 * We do that even in case of "errors" returned by
		 * refresh_index() as these are no actual errors.
		 * cmd_status() does the same.
		 */
		the_repository->సూచీ->cache_మారింది |= SOMETHING_మారింది;
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం refresh_callback(స్థిరరాశి నిర్మాణం option *opt,
				స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	తిరిగి refresh(opt->విలువ, 0);
}

స్థిర పూర్ణాంకం really_refresh_callback(స్థిరరాశి నిర్మాణం option *opt,
				స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	తిరిగి refresh(opt->విలువ, REFRESH_REALLY);
}

స్థిర పూర్ణాంకం chmod_callback(స్థిరరాశి నిర్మాణం option *opt,
				స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	అక్షరం *flip = opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	ఒకవేళ ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		తిరిగి error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	తిరిగి 0;
}

స్థిర పూర్ణాంకం resolve_undo_clear_callback(స్థిరరాశి నిర్మాణం option *opt వాడని,
				స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_సూచీ(the_repository->సూచీ);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_new_style_cacheinfo(స్థిరరాశి అక్షరం *arg,
				     గుర్తులేని పూర్ణాంకం *బహుళకం,
				     నిర్మాణం object_id *oid,
				     స్థిరరాశి అక్షరం **మార్గం)
{
	గుర్తులేని దీర్ఘ ul;
	అక్షరం *endp;
	స్థిరరాశి అక్షరం *p;

	ఒకవేళ (!arg)
		తిరిగి -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	ఒకవేళ (errno || endp == arg || *endp != ',' || (గుర్తులేని పూర్ణాంకం) ul != ul)
		తిరిగి -1; /* not a new-style cacheinfo */
	*బహుళకం = ul;
	endp++;
	ఒకవేళ (parse_oid_hex(endp, oid, &p) || *p != ',')
		తిరిగి -1;
	*మార్గం = p + 1;
	తిరిగి 0;
}

స్థిర గణన parse_opt_ఫలితం cacheinfo_callback(
	నిర్మాణం parse_opt_ctx_t *ctx, స్థిరరాశి నిర్మాణం option *opt వాడని,
	స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం object_id oid;
	గుర్తులేని పూర్ణాంకం బహుళకం;
	స్థిరరాశి అక్షరం *మార్గం;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ఒకవేళ (!parse_new_style_cacheinfo(ctx->argv[1], &బహుళకం, &oid, &మార్గం)) {
		ఒకవేళ (కూడు_cacheinfo(బహుళకం, &oid, మార్గం, 0))
			die("git update-index: --cacheinfo cannot add %s", మార్గం);
		ctx->argv++;
		ctx->argc--;
		తిరిగి 0;
	}
	ఒకవేళ (ctx->argc <= 3)
		తిరిగి error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	ఒకవేళ (strtoul_ui(*++ctx->argv, 8, &బహుళకం) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    కూడు_cacheinfo(బహుళకం, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	తిరిగి 0;
}

స్థిర గణన parse_opt_ఫలితం stdin_cacheinfo_callback(
	నిర్మాణం parse_opt_ctx_t *ctx, స్థిరరాశి నిర్మాణం option *opt,
	స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *nul_పదం_పంక్తి = opt->విలువ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ఒకవేళ (ctx->argc != 1)
		తిరిగి error("option '%s' must be the last argument", opt->long_name);
	allow_కూడు = allow_replace = allow_remove = 1;
	read_సూచీ_info(*nul_పదం_పంక్తి);
	తిరిగి 0;
}

స్థిర గణన parse_opt_ఫలితం stdin_callback(
	నిర్మాణం parse_opt_ctx_t *ctx, స్థిరరాశి నిర్మాణం option *opt,
	స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *read_from_stdin = opt->విలువ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ఒకవేళ (ctx->argc != 1)
		తిరిగి error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	తిరిగి 0;
}

స్థిర గణన parse_opt_ఫలితం unresolve_callback(
	నిర్మాణం parse_opt_ctx_t *ctx, స్థిరరాశి నిర్మాణం option *opt,
	స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *has_errors = opt->విలువ;
	స్థిరరాశి అక్షరం *ఉపసర్గ = the_repository->ఉపసర్గ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				ఉపసర్గ, ఉపసర్గ ? అక్షరశ్రేణి_పొడవు(ఉపసర్గ) : 0);
	ఒకవేళ (*has_errors)
		the_repository->సూచీ->cache_మారింది = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	తిరిగి 0;
}

స్థిర గణన parse_opt_ఫలితం reupdate_callback(
	నిర్మాణం parse_opt_ctx_t *ctx, స్థిరరాశి నిర్మాణం option *opt,
	స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *has_errors = opt->విలువ;
	స్థిరరాశి అక్షరం *ఉపసర్గ = the_repository->ఉపసర్గ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	అమరిక_work_వృక్షం(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, ఉపసర్గ);
	ఒకవేళ (*has_errors)
		the_repository->సూచీ->cache_మారింది = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	తిరిగి 0;
}

పూర్ణాంకం cmd_update_సూచీ(పూర్ణాంకం argc,
		     స్థిరరాశి అక్షరం **argv,
		     స్థిరరాశి అక్షరం *ఉపసర్గ,
		     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం newfd, entries, has_errors = 0, nul_పదం_పంక్తి = 0;
	గణన uc_బహుళకం untracked_cache = UC_UNSPECIFIED;
	పూర్ణాంకం read_from_stdin = 0;
	పూర్ణాంకం ఉపసర్గ_నిడివి = ఉపసర్గ ? అక్షరశ్రేణి_పొడవు(ఉపసర్గ) : 0;
	పూర్ణాంకం preferred_సూచీ_format = 0;
	అక్షరం సమితి_executable_బిట్టు = 0;
	నిర్మాణం refresh_params refresh_వాదనలు = {0, &has_errors};
	పూర్ణాంకం తాళం_error = 0;
	పూర్ణాంకం విభజించు_సూచీ = -1;
	పూర్ణాంకం force_write = 0;
	పూర్ణాంకం fsmonitor = -1;
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	నిర్మాణం parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	పూర్ణాంకం parseopt_state = PARSE_OPT_UNKNOWN;
	నిర్మాణం repository *r = the_repository;
	నిర్మాణం odb_transaction *transaction;
	నిర్మాణం option options[] = {
		OPT_బిట్టు('q', ఖాళీ, &refresh_వాదనలు.flags,
			N_("continue refresh even when index needs update"),
			REFRESH_QUIET),
		OPT_బిట్టు(0, "ignore-submodules", &refresh_వాదనలు.flags,
			N_("refresh: ignore submodules"),
			REFRESH_IGNORE_SUBMODULES),
		OPT_సమితి_INT(0, "add", &allow_కూడు,
			N_("do not ignore new files"), 1),
		OPT_సమితి_INT(0, "replace", &allow_replace,
			N_("let files replace directories and vice-versa"), 1),
		OPT_సమితి_INT(0, "remove", &allow_remove,
			N_("notice files missing from worktree"), 1),
		OPT_బిట్టు(0, "unmerged", &refresh_వాదనలు.flags,
			N_("refresh even if index contains unmerged entries"),
			REFRESH_UNMERGED),
		OPT_CALLBACK_F(0, "refresh", &refresh_వాదనలు, ఖాళీ,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_వాదనలు, ఖాళీ,
			N_("like --refresh, but ignore assume-unchanged setting"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			really_refresh_callback),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "cacheinfo",
			.argh = N_("<mode>,<object>,<path>"),
			.help = N_("add the specified entry to the index"),
			.flags = PARSE_OPT_NOARG | /* disallow --cacheinfo=<mode> form */
				 PARSE_OPT_NONEG | PARSE_OPT_LITERAL_ARGHELP,
			.ll_callback = cacheinfo_callback,
		},
		OPT_CALLBACK_F(0, "chmod", &సమితి_executable_బిట్టు, "(+|-)x",
			N_("override the executable bit of the listed files"),
			PARSE_OPT_NONEG,
			chmod_callback),
		{
			.type = OPTION_సమితి_INT,
			.long_name = "assume-unchanged",
			.విలువ = &mark_valid_only,
			.precision = పరిమాణం(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_సమితి_INT,
			.long_name = "no-assume-unchanged",
			.విలువ = &mark_valid_only,
			.precision = పరిమాణం(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_సమితి_INT,
			.long_name = "skip-worktree",
			.విలువ = &mark_skip_worktree_only,
			.precision = పరిమాణం(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_సమితి_INT,
			.long_name = "no-skip-worktree",
			.విలువ = &mark_skip_worktree_only,
			.precision = పరిమాణం(mark_skip_worktree_only),
			.help = N_("clear skip-worktree bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_BOOL(0, "ignore-skip-worktree-entries", &ignore_skip_worktree_entries,
			 N_("do not touch index-only entries")),
		OPT_సమితి_INT(0, "info-only", &info_only,
			N_("add to index only; do not add content to object database"), 1),
		OPT_సమితి_INT(0, "force-remove", &force_remove,
			N_("remove named paths even if present in worktree"), 1),
		OPT_BOOL('z', ఖాళీ, &nul_పదం_పంక్తి,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.విలువ = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.విలువ = &nul_పదం_పంక్తి,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.విలువ = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.విలువ = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_బిట్టు(0, "ignore-missing", &refresh_వాదనలు.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_సమితి_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", ఖాళీ, ఖాళీ,
			N_("(for porcelains) forget saved unresolved conflicts"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			resolve_undo_clear_callback),
		OPT_INTEGER(0, "index-version", &preferred_సూచీ_format,
			N_("write index in this format")),
		OPT_సమితి_INT(0, "show-index-version", &preferred_సూచీ_format,
			    N_("report on-disk index format version"), -1),
		OPT_BOOL(0, "split-index", &విభజించు_సూచీ,
			N_("enable or disable split index")),
		OPT_BOOL(0, "untracked-cache", &untracked_cache,
			N_("enable/disable untracked cache")),
		OPT_సమితి_INT(0, "test-untracked-cache", &untracked_cache,
			    N_("test if the filesystem supports untracked cache"), UC_TEST),
		OPT_సమితి_INT(0, "force-untracked-cache", &untracked_cache,
			    N_("enable untracked cache without testing the filesystem"), UC_FORCE),
		OPT_సమితి_INT(0, "force-write-index", &force_write,
			N_("write out the index even if is not flagged as changed"), 1),
		OPT_BOOL(0, "fsmonitor", &fsmonitor,
			N_("enable or disable file system monitor")),
		{
			.type = OPTION_సమితి_INT,
			.long_name = "fsmonitor-valid",
			.విలువ = &mark_fsmonitor_only,
			.precision = పరిమాణం(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_సమితి_INT,
			.long_name = "no-fsmonitor-valid",
			.విలువ = &mark_fsmonitor_only,
			.precision = పరిమాణం(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_చివర()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_సూచీ_usage, options);

	repo_config(the_repository, git_default_config, ఖాళీ);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_సూచీ(the_repository, &తాళం_file, 0);
	ఒకవేళ (newfd < 0)
		తాళం_error = errno;

	entries = repo_read_సూచీ(the_repository);
	ఒకవేళ (entries < 0)
		die("cache corrupted");

	the_repository->సూచీ->updated_skipworktree = 1;

	/*
	 * Custom copy of parse_options() because we want to handle
	 * filename arguments as they come.
	 */
	parse_options_ప్రారంభం(&ctx, argc, argv, ఉపసర్గ,
			    options, PARSE_OPT_STOP_AT_NON_OPTION);

	/*
	 * Allow the object layer to optimize adding multiple objects in
	 * a batch.
	 */
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	అయితే (ctx.argc) {
		ఒకవేళ (parseopt_state != PARSE_OPT_పూర్తి)
			parseopt_state = parse_options_అడుగు(&ctx, options,
							    update_సూచీ_usage);
		ఒకవేళ (!ctx.argc)
			ఆపు;
		ఎంపిక (parseopt_state) {
		స్థితి PARSE_OPT_HELP:
			నిష్క్రమించు(0);
		స్థితి PARSE_OPT_HELP_ERROR:
		స్థితి PARSE_OPT_ERROR:
			నిష్క్రమించు(129);
		స్థితి PARSE_OPT_COMPLETE:
			నిష్క్రమించు(0);
		స్థితి PARSE_OPT_NON_OPTION:
		స్థితి PARSE_OPT_పూర్తి:
		{
			స్థిరరాశి అక్షరం *మార్గం = ctx.argv[0];
			అక్షరం *p;

			/*
			 * It is possible, though unlikely, that a caller could
			 * use the verbose output to synchronize with addition
			 * of objects to the object database. The current
			 * implementation of ODB transactions leaves objects
			 * invisible while a transaction is active, so end the
			 * transaction here early before processing the next
			 * update. All further updates are performed outside of
			 * a transaction.
			 */
			ఒకవేళ (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = ఖాళీ;
			}

			అమరిక_work_వృక్షం(the_repository);
			p = ఉపసర్గ_మార్గం(the_repository, ఉపసర్గ, ఉపసర్గ_నిడివి, మార్గం);
			update_one(p);
			ఒకవేళ (సమితి_executable_బిట్టు)
				chmod_మార్గం(సమితి_executable_బిట్టు, p);
			విడుదల(p);
			ctx.argc--;
			ctx.argv++;
			ఆపు;
		}
		స్థితి PARSE_OPT_UNKNOWN:
			ఒకవేళ (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			లేకపోతే
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_సూచీ_usage, options);
		}
	}
	argc = parse_options_చివర(&ctx);

	getline_fn = nul_పదం_పంక్తి ? strbuf_getline_nul : strbuf_getline_lf;
	ఒకవేళ (preferred_సూచీ_format) {
		ఒకవేళ (preferred_సూచీ_format < 0) {
			రాయి(_("%d\n"), the_repository->సూచీ->సంస్కరణ);
		} లేకపోతే ఒకవేళ (preferred_సూచీ_format < సూచీ_FORMAT_LB ||
			   సూచీ_FORMAT_UB < preferred_సూచీ_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_సూచీ_format,
			    సూచీ_FORMAT_LB, సూచీ_FORMAT_UB);
		} లేకపోతే {
			ఒకవేళ (the_repository->సూచీ->సంస్కరణ != preferred_సూచీ_format)
				the_repository->సూచీ->cache_మారింది |= SOMETHING_మారింది;
			report(_("index-version: was %d, set to %d"),
			       the_repository->సూచీ->సంస్కరణ, preferred_సూచీ_format);
			the_repository->సూచీ->సంస్కరణ = preferred_సూచీ_format;
		}
	}

	ఒకవేళ (read_from_stdin) {
		నిర్మాణం strbuf buf = STRBUF_INIT;
		నిర్మాణం strbuf unquoted = STRBUF_INIT;

		అమరిక_work_వృక్షం(the_repository);
		అయితే (getline_fn(&buf, stdin) != EOF) {
			అక్షరం *p;
			ఒకవేళ (!nul_పదం_పంక్తి && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ఒకవేళ (unquote_c_style(&unquoted, buf.buf, ఖాళీ))
					die("line is badly quoted");
				strbuf_మార్పిడి(&buf, &unquoted);
			}
			p = ఉపసర్గ_మార్గం(the_repository, ఉపసర్గ, ఉపసర్గ_నిడివి, buf.buf);
			update_one(p);
			ఒకవేళ (సమితి_executable_బిట్టు)
				chmod_మార్గం(సమితి_executable_బిట్టు, p);
			విడుదల(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	ఒకవేళ (విభజించు_సూచీ > 0) {
		ఒకవేళ (repo_config_get_విభజించు_సూచీ(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		ఒకవేళ (the_repository->సూచీ->విభజించు_సూచీ)
			the_repository->సూచీ->cache_మారింది |= విభజించు_సూచీ_ORDERED;
		లేకపోతే
			కూడు_విభజించు_సూచీ(the_repository->సూచీ);
	} లేకపోతే ఒకవేళ (!విభజించు_సూచీ) {
		ఒకవేళ (repo_config_get_విభజించు_సూచీ(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_విభజించు_సూచీ(the_repository->సూచీ);
	}

	prepare_repo_settings(r);
	ఎంపిక (untracked_cache) {
	స్థితి UC_UNSPECIFIED:
		ఆపు;
	స్థితి UC_DISABLE:
		ఒకవేళ (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->సూచీ);
		report(_("Untracked cache disabled"));
		ఆపు;
	స్థితి UC_TEST:
		అమరిక_work_వృక్షం(the_repository);
		తిరిగి !test_if_untracked_cache_is_supported();
	స్థితి UC_ENABLE:
	స్థితి UC_FORCE:
		ఒకవేళ (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		కూడు_untracked_cache(the_repository->సూచీ);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_వృక్షం(the_repository));
		ఆపు;
	అప్రమేయ:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	ఒకవేళ (fsmonitor > 0) {
		గణన fsmonitor_బహుళకం fsm_బహుళకం = fsm_settings_get_బహుళకం(r);
		గణన fsmonitor_reason reason = fsm_settings__get_reason(r);

		/*
		 * The user wants to turn on FSMonitor using the command
		 * line argument.  (We don't know (or care) whether that
		 * is the IPC or HOOK version.)
		 *
		 * Use one of the __get routines to force load the FSMonitor
		 * config settings into the repo-settings.  That will detect
		 * whether the file system is compatible so that we can stop
		 * here with a nice error message.
		 */
		ఒకవేళ (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_సందేశం(r, reason));

		ఒకవేళ (fsm_బహుళకం == FSMONITOR_బహుళకం_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		కూడు_fsmonitor(the_repository->సూచీ);
		report(_("fsmonitor enabled"));
	} లేకపోతే ఒకవేళ (!fsmonitor) {
		గణన fsmonitor_బహుళకం fsm_బహుళకం = fsm_settings_get_బహుళకం(r);
		ఒకవేళ (fsm_బహుళకం > FSMONITOR_బహుళకం_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->సూచీ);
		report(_("fsmonitor disabled"));
	}

	ఒకవేళ (the_repository->సూచీ->cache_మారింది || force_write) {
		ఒకవేళ (newfd < 0) {
			ఒకవేళ (refresh_వాదనలు.flags & REFRESH_QUIET)
				నిష్క్రమించు(128);
			unable_గమ్యం_తాళం_die(repo_get_సూచీ_file(the_repository), తాళం_error);
		}
		ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం_file, COMMIT_తాళం))
			die("Unable to write new index file");
	}

	rollback_తాళం_file(&తాళం_file);

	తిరిగి has_errors ? 1 : 0;
}
