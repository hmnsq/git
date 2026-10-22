/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/transaction.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "resolve-undo.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "read-cache.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "split-index.h"
#ಅನ್ತರ್ಭಾವ "symlinks.h"
#ಅನ್ತರ್ಭಾವ "fsmonitor.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ allow_add;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ allow_remove;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ allow_replace;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ info_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ force_remove;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ verbose;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_valid_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_skip_worktree_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_fsmonitor_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ignore_skip_worktree_entries;
#define MARK_FLAG 1
#define UNMARK_FLAG 2
ಸ್ಥಿರ ಸಂರಚನಾ strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
ಗಣನಾ uc_mode {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (printf, 1, 2)))
ಸ್ಥಿರ ಶೂನ್ಯ report(ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
	va_list vp;

	ಯದಿ (!verbose)
		ಪ್ರತ್ಯಾಗಮ;

	va_start(vp, fmt);
	vprintf(fmt, vp);
	putchar('\n');
	va_end(vp);
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_test_directory(ಶೂನ್ಯ)
{
	ಯದಿ (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_mtime_path(ಧ್ರುವ ವರ್ಣ *path)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, path);
	ಪ್ರತ್ಯಾಗಮ sb.buf;
}

ಸ್ಥಿರ ಶೂನ್ಯ xmkdir(ಧ್ರುವ ವರ್ಣ *path)
{
	path = get_mtime_path(path);
	ಯದಿ (mkdir(path, 0700))
		die_errno(_("failed to create directory %s"), path);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ xstat_mtime_dir(ಸಂರಚನಾ stat *st)
{
	ಯದಿ (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ create_file(ಧ್ರುವ ವರ್ಣ *path)
{
	ಪೂರ್ಣಾಙ್ಕ fd;
	path = get_mtime_path(path);
	fd = xopen(path, O_CREAT | O_RDWR, 0644);
	ಪ್ರತ್ಯಾಗಮ fd;
}

ಸ್ಥಿರ ಶೂನ್ಯ xunlink(ಧ್ರುವ ವರ್ಣ *path)
{
	path = get_mtime_path(path);
	ಯದಿ (unlink(path))
		die_errno(_("failed to delete file %s"), path);
}

ಸ್ಥಿರ ಶೂನ್ಯ xrmdir(ಧ್ರುವ ವರ್ಣ *path)
{
	path = get_mtime_path(path);
	ಯದಿ (rmdir(path))
		die_errno(_("failed to delete directory %s"), path);
}

ಸ್ಥಿರ ಶೂನ್ಯ avoid_racy(ಶೂನ್ಯ)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ test_if_untracked_cache_is_supported(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ stat st;
	ಸಂರಚನಾ stat_data base;
	ಪೂರ್ಣಾಙ್ಕ fd, ret = 0;
	ವರ್ಣ *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	ಯದಿ (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	ಮುಕ್ತ(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	ಯದಿ (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		ಗಚ್ಛ done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	ಯದಿ (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		ಗಚ್ಛ done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	ಯದಿ (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		ಗಚ್ಛ done;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	ಯದಿ (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		ಗಚ್ಛ done;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	ಯದಿ (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		ಗಚ್ಛ done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	ಯದಿ (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		ಗಚ್ಛ done;
	}

	ಯದಿ (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

done:
	strbuf_release(&mtime_dir);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_ce_flags(ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ flag, ಪೂರ್ಣಾಙ್ಕ mark)
{
	ಪೂರ್ಣಾಙ್ಕ namelen = strlen(path);
	ಪೂರ್ಣಾಙ್ಕ pos = index_name_pos(the_repository->index, path, namelen);
	ಯದಿ (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->index, the_repository->index->cache[pos]);
		ಯದಿ (mark)
			the_repository->index->cache[pos]->ce_flags |= flag;
		ಅನ್ಯಥಾ
			the_repository->index->cache[pos]->ce_flags &= ~flag;
		the_repository->index->cache[pos]->ce_flags |= CE_UPDATE_IN_BASE;
		cache_ವೃಕ್ಷ_invalidate_path(the_repository->index, path);
		the_repository->index->cache_changed |= CE_ENTRY_CHANGED;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ remove_one_path(ಧ್ರುವ ವರ್ಣ *path)
{
	ಯದಿ (!allow_remove)
		ಪ್ರತ್ಯಾಗಮ error("%s: does not exist and --remove not passed", path);
	ಯದಿ (remove_file_from_index(the_repository->index, path))
		ಪ್ರತ್ಯಾಗಮ error("%s: cannot remove from the index", path);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ process_lstat_error(ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ err)
{
	ಯದಿ (is_missing_file_error(err))
		ಪ್ರತ್ಯಾಗಮ remove_one_path(path);
	ಪ್ರತ್ಯಾಗಮ error("lstat(\"%s\"): %s", path, strerror(err));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_one_path(ಧ್ರುವ ಸಂರಚನಾ cache_entry *old, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ len, ಸಂರಚನಾ stat *st)
{
	ಪೂರ್ಣಾಙ್ಕ option;
	ಸಂರಚನಾ cache_entry *ce;

	/* Was the old index entry already up-to-date? */
	ಯದಿ (old && !ce_stage(old) && !ie_match_stat(the_repository->index, old, st, 0))
		ಪ್ರತ್ಯಾಗಮ 0;

	ce = make_empty_cache_entry(the_repository->index, len);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->index, ce, st);
	ce->ce_mode = ce_mode_from_stat(the_repository, old, st->st_mode);

	ಯದಿ (index_path(the_repository->index, &ce->oid, path, st,
		       info_only ? 0 : INDEX_WRITE_OBJECT)) {
		discard_cache_entry(ce);
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	ಯದಿ (add_index_entry(the_repository->index, ce, option)) {
		discard_cache_entry(ce);
		ಪ್ರತ್ಯಾಗಮ error("%s: cannot add to the index - missing --add option?", path);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ process_directory(ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ len, ಸಂರಚನಾ stat *st)
{
	ಸಂರಚನಾ object_id oid;
	ಪೂರ್ಣಾಙ್ಕ pos = index_name_pos(the_repository->index, path, len);

	/* Exact match: file or existing gitlink */
	ಯದಿ (pos >= 0) {
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = the_repository->index->cache[pos];
		ಯದಿ (S_ISGITLINK(ce->ce_mode)) {

			/* Do nothing to the index if there is no HEAD! */
			ಯದಿ (repo_resolve_gitlink_ref(the_repository, path,
						     "HEAD", &oid) < 0)
				ಪ್ರತ್ಯಾಗಮ 0;

			ಪ್ರತ್ಯಾಗಮ add_one_path(ce, path, len, st);
		}
		/* Should this be an unconditional error? */
		ಪ್ರತ್ಯಾಗಮ remove_one_path(path);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	ಯಾವತ್ (pos < the_repository->index->cache_nr) {
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = the_repository->index->cache[pos++];

		ಯದಿ (strncmp(ce->name, path, len))
			ಭಙ್ಗ;
		ಯದಿ (ce->name[len] > '/')
			ಭಙ್ಗ;
		ಯದಿ (ce->name[len] < '/')
			ಅನುವರ್ತ;

		/* Subdirectory match - error out */
		ಪ್ರತ್ಯಾಗಮ error("%s: is a directory - add individual files instead", path);
	}

	/* No match - should we add it as a gitlink? */
	ಯದಿ (!repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid))
		ಪ್ರತ್ಯಾಗಮ add_one_path(ರಿಕ್ತ, path, len, st);

	/* Error out. */
	ಪ್ರತ್ಯಾಗಮ error("%s: is a directory - add files inside instead", path);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ process_path(ಧ್ರುವ ವರ್ಣ *path, ಸಂರಚನಾ stat *st, ಪೂರ್ಣಾಙ್ಕ stat_errno)
{
	ಪೂರ್ಣಾಙ್ಕ pos, len;
	ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce;

	len = strlen(path);
	ಯದಿ (has_symlink_leading_path(path, len))
		ಪ್ರತ್ಯಾಗಮ error("'%s' is beyond a symbolic link", path);

	pos = index_name_pos(the_repository->index, path, len);
	ce = pos < 0 ? ರಿಕ್ತ : the_repository->index->cache[pos];
	ಯದಿ (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		ಯದಿ (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_index(the_repository->index, path))
			ಪ್ರತ್ಯಾಗಮ error("%s: cannot remove from the index", path);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	ಯದಿ (stat_errno)
		ಪ್ರತ್ಯಾಗಮ process_lstat_error(path, stat_errno);

	ಯದಿ (S_ISDIR(st->st_mode))
		ಪ್ರತ್ಯಾಗಮ process_directory(path, len, st);

	ಪ್ರತ್ಯಾಗಮ add_one_path(ce, path, len, st);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_cacheinfo(unsigned ಪೂರ್ಣಾಙ್ಕ mode, ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
			 ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ stage)
{
	ಪೂರ್ಣಾಙ್ಕ len, option;
	ಸಂರಚನಾ cache_entry *ce;

	ಯದಿ (!verify_path(path, mode))
		ಪ್ರತ್ಯಾಗಮ error("Invalid path '%s'", path);

	len = strlen(path);
	ce = make_empty_cache_entry(the_repository->index, len);

	oidcpy(&ce->oid, oid);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_mode = create_ce_mode(mode);
	ಯದಿ (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	ಯದಿ (add_index_entry(the_repository->index, ce, option))
		ಪ್ರತ್ಯಾಗಮ error("%s: cannot add to the index - missing --add option?",
			     path);
	report("add '%s'", path);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ chmod_path(ವರ್ಣ flip, ಧ್ರುವ ವರ್ಣ *path)
{
	ಪೂರ್ಣಾಙ್ಕ pos;
	ಸಂರಚನಾ cache_entry *ce;

	pos = index_name_pos(the_repository->index, path, strlen(path));
	ಯದಿ (pos < 0)
		ಗಚ್ಛ fail;
	ce = the_repository->index->cache[pos];
	ಯದಿ (chmod_index_entry(the_repository->index, ce, flip) < 0)
		ಗಚ್ಛ fail;

	report("chmod %cx '%s'", flip, path);
	ಪ್ರತ್ಯಾಗಮ;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, path);
}

ಸ್ಥಿರ ಶೂನ್ಯ update_one(ಧ್ರುವ ವರ್ಣ *path)
{
	ಪೂರ್ಣಾಙ್ಕ stat_errno = 0;
	ಸಂರಚನಾ stat st;

	ಯದಿ (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_mode = 0;
	ಅನ್ಯಥಾ ಯದಿ (lstat(path, &st) < 0) {
		st.st_mode = 0;
		stat_errno = errno;
	} /* else stat is valid */

	ಯದಿ (!verify_path(path, st.st_mode)) {
		fprintf(stderr, "Ignoring path %s\n", path);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಯದಿ (mark_valid_only) {
		ಯದಿ (mark_ce_flags(path, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಯದಿ (mark_skip_worktree_only) {
		ಯದಿ (mark_ce_flags(path, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಯದಿ (mark_fsmonitor_only) {
		ಯದಿ (mark_ce_flags(path, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (force_remove) {
		ಯದಿ (remove_file_from_index(the_repository->index, path))
			die("git update-index: unable to remove %s", path);
		report("remove '%s'", path);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಯದಿ (process_path(path, &st, stat_errno))
		die("Unable to process path %s", path);
	report("add '%s'", path);
}

ಸ್ಥಿರ ಶೂನ್ಯ read_index_info(ಪೂರ್ಣಾಙ್ಕ nul_term_ಸಾಲು)
{
	ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ hexsz = the_hash_algo->hexsz;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_ಸಾಲು ? strbuf_getline_nul : strbuf_getline_lf;
	ಯಾವತ್ (getline_fn(&buf, stdin) != EOF) {
		ವರ್ಣ *ptr, *tab;
		ವರ್ಣ *path_name;
		ಸಂರಚನಾ object_id oid;
		unsigned ಪೂರ್ಣಾಙ್ಕ mode;
		unsigned long ul;
		ಪೂರ್ಣಾಙ್ಕ stage;

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
		ಯದಿ (ptr == buf.buf || *ptr != ' '
		    || errno || (unsigned ಪೂರ್ಣಾಙ್ಕ) ul != ul)
			ಗಚ್ಛ bad_ಸಾಲು;
		mode = ul;

		tab = strchr(ptr, '\t');
		ಯದಿ (!tab || tab - ptr < hexsz + 1)
			ಗಚ್ಛ bad_ಸಾಲು;

		ಯದಿ (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		ಅನ್ಯಥಾ {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		ಯದಿ (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			ಗಚ್ಛ bad_ಸಾಲು;

		path_name = ptr;
		ಯದಿ (!nul_term_ಸಾಲು && path_name[0] == '"') {
			strbuf_reset(&uq);
			ಯದಿ (unquote_c_style(&uq, path_name, ರಿಕ್ತ)) {
				die("git update-index: bad quoting of path name");
			}
			path_name = uq.buf;
		}

		ಯದಿ (!verify_path(path_name, mode)) {
			fprintf(stderr, "Ignoring path %s\n", path_name);
			ಅನುವರ್ತ;
		}

		ಯದಿ (!mode) {
			/* mode == 0 means there is no such path -- remove */
			ಯದಿ (remove_file_from_index(the_repository->index, path_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		ಅನ್ಯಥಾ {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			ಯದಿ (add_cacheinfo(mode, &oid, path_name, stage))
				die("git update-index: unable to update %s",
				    path_name);
		}
		ಅನುವರ್ತ;

	bad_ಸಾಲು:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ update_index_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಸಂರಚನಾ cache_entry *read_one_ent(ಧ್ರುವ ವರ್ಣ *which,
					ಸಂರಚನಾ object_id *ent, ಧ್ರುವ ವರ್ಣ *path,
					ಪೂರ್ಣಾಙ್ಕ namelen, ಪೂರ್ಣಾಙ್ಕ stage)
{
	unsigned short mode;
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ cache_entry *ce;

	ಯದಿ (get_ವೃಕ್ಷ_entry(the_repository, ent, path, &oid, &mode)) {
		ಯದಿ (which)
			error("%s: not in %s branch.", path, which);
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}
	ಯದಿ (!the_repository->index->sparse_index && mode == S_IFDIR) {
		ಯದಿ (which)
			error("%s: not a blob in %s branch.", path, which);
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}
	ce = make_empty_cache_entry(the_repository->index, namelen);

	oidcpy(&ce->oid, &oid);
	memcpy(ce->name, path, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_mode = create_ce_mode(mode);
	ಪ್ರತ್ಯಾಗಮ ce;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ unresolve_one(ಧ್ರುವ ವರ್ಣ *path)
{
	ಸಂರಚನಾ string_list_item *item;
	ಪೂರ್ಣಾಙ್ಕ res = 0;

	ಯದಿ (!the_repository->index->resolve_undo)
		ಪ್ರತ್ಯಾಗಮ res;
	item = string_list_lookup(the_repository->index->resolve_undo, path);
	ಯದಿ (!item)
		ಪ್ರತ್ಯಾಗಮ res; /* no resolve-undo record for the path */
	res = unmerge_index_entry(the_repository->index, path, item->util, 0);
	FREE_AND_NULL(item->util);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_unresolve(ಪೂರ್ಣಾಙ್ಕ ac, ಧ್ರುವ ವರ್ಣ **av,
			ಧ್ರುವ ವರ್ಣ *prefix, ಪೂರ್ಣಾಙ್ಕ prefix_length)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ err = 0;

	ಪ್ರತಿ (i = 1; i < ac; i++) {
		ಧ್ರುವ ವರ್ಣ *arg = av[i];
		ವರ್ಣ *p = prefix_path(the_repository, prefix, prefix_length, arg);
		err |= unresolve_one(p);
		ಮುಕ್ತ(p);
	}
	ಪ್ರತ್ಯಾಗಮ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_reupdate(ಧ್ರುವ ವರ್ಣ **paths,
		       ಧ್ರುವ ವರ್ಣ *prefix)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	ಪೂರ್ಣಾಙ್ಕ pos;
	ಪೂರ್ಣಾಙ್ಕ has_head = 1;
	ಸಂರಚನಾ pathspec pathspec;
	ಸಂರಚನಾ object_id head_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, paths);

	ಯದಿ (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &head_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_head = 0;
 redo:
	ಪ್ರತಿ (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = the_repository->index->cache[pos];
		ಸಂರಚನಾ cache_entry *old = ರಿಕ್ತ;
		ಪೂರ್ಣಾಙ್ಕ save_nr;
		ವರ್ಣ *path;

		ಯದಿ (ce_stage(ce) || !ce_path_match(the_repository->index, ce, &pathspec, ರಿಕ್ತ))
			ಅನುವರ್ತ;
		ಯದಿ (has_head)
			old = read_one_ent(ರಿಕ್ತ, &head_oid,
					   ce->name, ce_namelen(ce), 0);
		ಯದಿ (old && ce->ce_mode == old->ce_mode &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_entry(old);
			ಅನುವರ್ತ; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		ಯದಿ (S_ISSPARSEDIR(ce->ce_mode)) {
			ensure_full_index(the_repository->index);
			ಗಚ್ಛ redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->index->cache_nr;
		path = xstrdup(ce->name);
		update_one(path);
		ಮುಕ್ತ(path);
		discard_cache_entry(old);
		ಯದಿ (save_nr != the_repository->index->cache_nr)
			ಗಚ್ಛ redo;
	}
	clear_pathspec(&pathspec);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ refresh_params {
	unsigned ಪೂರ್ಣಾಙ್ಕ flags;
	ಪೂರ್ಣಾಙ್ಕ *has_errors;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ refresh(ಸಂರಚನಾ refresh_params *o, unsigned ಪೂರ್ಣಾಙ್ಕ flag)
{
	setup_work_ವೃಕ್ಷ(the_repository);
	repo_read_index(the_repository);
	*o->has_errors |= refresh_index(the_repository->index, o->flags | flag, ರಿಕ್ತ,
					ರಿಕ್ತ, ರಿಕ್ತ);
	ಯದಿ (has_racy_timestamp(the_repository->index)) {
		/*
		 * Even if nothing else has changed, updating the file
		 * increases the chance that racy timestamps become
		 * non-racy, helping future run-time performance.
		 * We do that even in case of "errors" returned by
		 * refresh_index() as these are no actual errors.
		 * cmd_status() does the same.
		 */
		the_repository->index->cache_changed |= SOMETHING_CHANGED;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ refresh_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ಪ್ರತ್ಯಾಗಮ refresh(opt->ಮೌಲ್ಯ, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ really_refresh_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ಪ್ರತ್ಯಾಗಮ refresh(opt->ಮೌಲ್ಯ, REFRESH_REALLY);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ chmod_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ವರ್ಣ *flip = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	ಯದಿ ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		ಪ್ರತ್ಯಾಗಮ error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ resolve_undo_clear_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
				ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_index(the_repository->index);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_new_style_cacheinfo(ಧ್ರುವ ವರ್ಣ *arg,
				     unsigned ಪೂರ್ಣಾಙ್ಕ *mode,
				     ಸಂರಚನಾ object_id *oid,
				     ಧ್ರುವ ವರ್ಣ **path)
{
	unsigned long ul;
	ವರ್ಣ *endp;
	ಧ್ರುವ ವರ್ಣ *p;

	ಯದಿ (!arg)
		ಪ್ರತ್ಯಾಗಮ -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	ಯದಿ (errno || endp == arg || *endp != ',' || (unsigned ಪೂರ್ಣಾಙ್ಕ) ul != ul)
		ಪ್ರತ್ಯಾಗಮ -1; /* not a new-style cacheinfo */
	*mode = ul;
	endp++;
	ಯದಿ (parse_oid_hex(endp, oid, &p) || *p != ',')
		ಪ್ರತ್ಯಾಗಮ -1;
	*path = p + 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಗಣನಾ parse_opt_result cacheinfo_callback(
	ಸಂರಚನಾ parse_opt_ctx_t *ctx, ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
	ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ object_id oid;
	unsigned ಪೂರ್ಣಾಙ್ಕ mode;
	ಧ್ರುವ ವರ್ಣ *path;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಯದಿ (!parse_new_style_cacheinfo(ctx->argv[1], &mode, &oid, &path)) {
		ಯದಿ (add_cacheinfo(mode, &oid, path, 0))
			die("git update-index: --cacheinfo cannot add %s", path);
		ctx->argv++;
		ctx->argc--;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (ctx->argc <= 3)
		ಪ್ರತ್ಯಾಗಮ error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	ಯದಿ (strtoul_ui(*++ctx->argv, 8, &mode) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    add_cacheinfo(mode, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಗಣನಾ parse_opt_result stdin_cacheinfo_callback(
	ಸಂರಚನಾ parse_opt_ctx_t *ctx, ಧ್ರುವ ಸಂರಚನಾ option *opt,
	ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *nul_term_ಸಾಲು = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಯದಿ (ctx->argc != 1)
		ಪ್ರತ್ಯಾಗಮ error("option '%s' must be the last argument", opt->long_name);
	allow_add = allow_replace = allow_remove = 1;
	read_index_info(*nul_term_ಸಾಲು);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಗಣನಾ parse_opt_result stdin_callback(
	ಸಂರಚನಾ parse_opt_ctx_t *ctx, ಧ್ರುವ ಸಂರಚನಾ option *opt,
	ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *read_from_stdin = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಯದಿ (ctx->argc != 1)
		ಪ್ರತ್ಯಾಗಮ error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಗಣನಾ parse_opt_result unresolve_callback(
	ಸಂರಚನಾ parse_opt_ctx_t *ctx, ಧ್ರುವ ಸಂರಚನಾ option *opt,
	ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *has_errors = opt->ಮೌಲ್ಯ;
	ಧ್ರುವ ವರ್ಣ *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				prefix, prefix ? strlen(prefix) : 0);
	ಯದಿ (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಗಣನಾ parse_opt_result reupdate_callback(
	ಸಂರಚನಾ parse_opt_ctx_t *ctx, ಧ್ರುವ ಸಂರಚನಾ option *opt,
	ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *has_errors = opt->ಮೌಲ್ಯ;
	ಧ್ರುವ ವರ್ಣ *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	setup_work_ವೃಕ್ಷ(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, prefix);
	ಯದಿ (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_update_index(ಪೂರ್ಣಾಙ್ಕ argc,
		     ಧ್ರುವ ವರ್ಣ **argv,
		     ಧ್ರುವ ವರ್ಣ *prefix,
		     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ newfd, entries, has_errors = 0, nul_term_ಸಾಲು = 0;
	ಗಣನಾ uc_mode untracked_cache = UC_UNSPECIFIED;
	ಪೂರ್ಣಾಙ್ಕ read_from_stdin = 0;
	ಪೂರ್ಣಾಙ್ಕ prefix_length = prefix ? strlen(prefix) : 0;
	ಪೂರ್ಣಾಙ್ಕ preferred_index_format = 0;
	ವರ್ಣ set_executable_bit = 0;
	ಸಂರಚನಾ refresh_params refresh_args = {0, &has_errors};
	ಪೂರ್ಣಾಙ್ಕ lock_error = 0;
	ಪೂರ್ಣಾಙ್ಕ split_index = -1;
	ಪೂರ್ಣಾಙ್ಕ force_write = 0;
	ಪೂರ್ಣಾಙ್ಕ fsmonitor = -1;
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಸಂರಚನಾ parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	ಪೂರ್ಣಾಙ್ಕ parseopt_state = PARSE_OPT_UNKNOWN;
	ಸಂರಚನಾ repository *r = the_repository;
	ಸಂರಚನಾ odb_transaction *transaction;
	ಸಂರಚನಾ option options[] = {
		OPT_BIT('q', ರಿಕ್ತ, &refresh_args.flags,
			N_("continue refresh even when index needs update"),
			REFRESH_QUIET),
		OPT_BIT(0, "ignore-submodules", &refresh_args.flags,
			N_("refresh: ignore submodules"),
			REFRESH_IGNORE_SUBMODULES),
		OPT_SET_INT(0, "add", &allow_add,
			N_("do not ignore new files"), 1),
		OPT_SET_INT(0, "replace", &allow_replace,
			N_("let files replace directories and vice-versa"), 1),
		OPT_SET_INT(0, "remove", &allow_remove,
			N_("notice files missing from worktree"), 1),
		OPT_BIT(0, "unmerged", &refresh_args.flags,
			N_("refresh even if index contains unmerged entries"),
			REFRESH_UNMERGED),
		OPT_CALLBACK_F(0, "refresh", &refresh_args, ರಿಕ್ತ,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_args, ರಿಕ್ತ,
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
		OPT_CALLBACK_F(0, "chmod", &set_executable_bit, "(+|-)x",
			N_("override the executable bit of the listed files"),
			PARSE_OPT_NONEG,
			chmod_callback),
		{
			.type = OPTION_SET_INT,
			.long_name = "assume-unchanged",
			.ಮೌಲ್ಯ = &mark_valid_only,
			.precision = ಪರಿಮಾಣ(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-assume-unchanged",
			.ಮೌಲ್ಯ = &mark_valid_only,
			.precision = ಪರಿಮಾಣ(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "skip-worktree",
			.ಮೌಲ್ಯ = &mark_skip_worktree_only,
			.precision = ಪರಿಮಾಣ(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-skip-worktree",
			.ಮೌಲ್ಯ = &mark_skip_worktree_only,
			.precision = ಪರಿಮಾಣ(mark_skip_worktree_only),
			.help = N_("clear skip-worktree bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_BOOL(0, "ignore-skip-worktree-entries", &ignore_skip_worktree_entries,
			 N_("do not touch index-only entries")),
		OPT_SET_INT(0, "info-only", &info_only,
			N_("add to index only; do not add content to object database"), 1),
		OPT_SET_INT(0, "force-remove", &force_remove,
			N_("remove named paths even if present in worktree"), 1),
		OPT_BOOL('z', ರಿಕ್ತ, &nul_term_ಸಾಲು,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.ಮೌಲ್ಯ = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.ಮೌಲ್ಯ = &nul_term_ಸಾಲು,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.ಮೌಲ್ಯ = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.ಮೌಲ್ಯ = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_BIT(0, "ignore-missing", &refresh_args.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_SET_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", ರಿಕ್ತ, ರಿಕ್ತ,
			N_("(for porcelains) forget saved unresolved conflicts"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			resolve_undo_clear_callback),
		OPT_INTEGER(0, "index-version", &preferred_index_format,
			N_("write index in this format")),
		OPT_SET_INT(0, "show-index-version", &preferred_index_format,
			    N_("report on-disk index format version"), -1),
		OPT_BOOL(0, "split-index", &split_index,
			N_("enable or disable split index")),
		OPT_BOOL(0, "untracked-cache", &untracked_cache,
			N_("enable/disable untracked cache")),
		OPT_SET_INT(0, "test-untracked-cache", &untracked_cache,
			    N_("test if the filesystem supports untracked cache"), UC_TEST),
		OPT_SET_INT(0, "force-untracked-cache", &untracked_cache,
			    N_("enable untracked cache without testing the filesystem"), UC_FORCE),
		OPT_SET_INT(0, "force-write-index", &force_write,
			N_("write out the index even if is not flagged as changed"), 1),
		OPT_BOOL(0, "fsmonitor", &fsmonitor,
			N_("enable or disable file system monitor")),
		{
			.type = OPTION_SET_INT,
			.long_name = "fsmonitor-valid",
			.ಮೌಲ್ಯ = &mark_fsmonitor_only,
			.precision = ಪರಿಮಾಣ(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-fsmonitor-valid",
			.ಮೌಲ್ಯ = &mark_fsmonitor_only,
			.precision = ಪರಿಮಾಣ(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_index_usage, options);

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_full_index = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_index(the_repository, &lock_file, 0);
	ಯದಿ (newfd < 0)
		lock_error = errno;

	entries = repo_read_index(the_repository);
	ಯದಿ (entries < 0)
		die("cache corrupted");

	the_repository->index->updated_skipworktree = 1;

	/*
	 * Custom copy of parse_options() because we want to handle
	 * filename arguments as they come.
	 */
	parse_options_start(&ctx, argc, argv, prefix,
			    options, PARSE_OPT_STOP_AT_NON_OPTION);

	/*
	 * Allow the object layer to optimize adding multiple objects in
	 * a batch.
	 */
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	ಯಾವತ್ (ctx.argc) {
		ಯದಿ (parseopt_state != PARSE_OPT_DONE)
			parseopt_state = parse_options_step(&ctx, options,
							    update_index_usage);
		ಯದಿ (!ctx.argc)
			ಭಙ್ಗ;
		ವಿಕಲ್ಪ (parseopt_state) {
		ಪಕ್ಷ PARSE_OPT_HELP:
			exit(0);
		ಪಕ್ಷ PARSE_OPT_HELP_ERROR:
		ಪಕ್ಷ PARSE_OPT_ERROR:
			exit(129);
		ಪಕ್ಷ PARSE_OPT_COMPLETE:
			exit(0);
		ಪಕ್ಷ PARSE_OPT_NON_OPTION:
		ಪಕ್ಷ PARSE_OPT_DONE:
		{
			ಧ್ರುವ ವರ್ಣ *path = ctx.argv[0];
			ವರ್ಣ *p;

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
			ಯದಿ (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = ರಿಕ್ತ;
			}

			setup_work_ವೃಕ್ಷ(the_repository);
			p = prefix_path(the_repository, prefix, prefix_length, path);
			update_one(p);
			ಯದಿ (set_executable_bit)
				chmod_path(set_executable_bit, p);
			ಮುಕ್ತ(p);
			ctx.argc--;
			ctx.argv++;
			ಭಙ್ಗ;
		}
		ಪಕ್ಷ PARSE_OPT_UNKNOWN:
			ಯದಿ (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			ಅನ್ಯಥಾ
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_index_usage, options);
		}
	}
	argc = parse_options_end(&ctx);

	getline_fn = nul_term_ಸಾಲು ? strbuf_getline_nul : strbuf_getline_lf;
	ಯದಿ (preferred_index_format) {
		ಯದಿ (preferred_index_format < 0) {
			printf(_("%d\n"), the_repository->index->version);
		} ಅನ್ಯಥಾ ಯದಿ (preferred_index_format < INDEX_FORMAT_LB ||
			   INDEX_FORMAT_UB < preferred_index_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_index_format,
			    INDEX_FORMAT_LB, INDEX_FORMAT_UB);
		} ಅನ್ಯಥಾ {
			ಯದಿ (the_repository->index->version != preferred_index_format)
				the_repository->index->cache_changed |= SOMETHING_CHANGED;
			report(_("index-version: was %d, set to %d"),
			       the_repository->index->version, preferred_index_format);
			the_repository->index->version = preferred_index_format;
		}
	}

	ಯದಿ (read_from_stdin) {
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
		ಸಂರಚನಾ strbuf unquoted = STRBUF_INIT;

		setup_work_ವೃಕ್ಷ(the_repository);
		ಯಾವತ್ (getline_fn(&buf, stdin) != EOF) {
			ವರ್ಣ *p;
			ಯದಿ (!nul_term_ಸಾಲು && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ಯದಿ (unquote_c_style(&unquoted, buf.buf, ರಿಕ್ತ))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(the_repository, prefix, prefix_length, buf.buf);
			update_one(p);
			ಯದಿ (set_executable_bit)
				chmod_path(set_executable_bit, p);
			ಮುಕ್ತ(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	ಯದಿ (split_index > 0) {
		ಯದಿ (repo_config_get_split_index(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		ಯದಿ (the_repository->index->split_index)
			the_repository->index->cache_changed |= SPLIT_INDEX_ORDERED;
		ಅನ್ಯಥಾ
			add_split_index(the_repository->index);
	} ಅನ್ಯಥಾ ಯದಿ (!split_index) {
		ಯದಿ (repo_config_get_split_index(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_split_index(the_repository->index);
	}

	prepare_repo_settings(r);
	ವಿಕಲ್ಪ (untracked_cache) {
	ಪಕ್ಷ UC_UNSPECIFIED:
		ಭಙ್ಗ;
	ಪಕ್ಷ UC_DISABLE:
		ಯದಿ (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->index);
		report(_("Untracked cache disabled"));
		ಭಙ್ಗ;
	ಪಕ್ಷ UC_TEST:
		setup_work_ವೃಕ್ಷ(the_repository);
		ಪ್ರತ್ಯಾಗಮ !test_if_untracked_cache_is_supported();
	ಪಕ್ಷ UC_ENABLE:
	ಪಕ್ಷ UC_FORCE:
		ಯದಿ (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		add_untracked_cache(the_repository->index);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_ವೃಕ್ಷ(the_repository));
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	ಯದಿ (fsmonitor > 0) {
		ಗಣನಾ fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		ಗಣನಾ fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		ಯದಿ (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_ಸಂದೇಶ(r, reason));

		ಯದಿ (fsm_mode == FSMONITOR_MODE_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		add_fsmonitor(the_repository->index);
		report(_("fsmonitor enabled"));
	} ಅನ್ಯಥಾ ಯದಿ (!fsmonitor) {
		ಗಣನಾ fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		ಯದಿ (fsm_mode > FSMONITOR_MODE_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->index);
		report(_("fsmonitor disabled"));
	}

	ಯದಿ (the_repository->index->cache_changed || force_write) {
		ಯದಿ (newfd < 0) {
			ಯದಿ (refresh_args.flags & REFRESH_QUIET)
				exit(128);
			unable_to_lock_die(repo_get_index_file(the_repository), lock_error);
		}
		ಯದಿ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die("Unable to write new index file");
	}

	rollback_lock_file(&lock_file);

	ಪ್ರತ್ಯಾಗಮ has_errors ? 1 : 0;
}
