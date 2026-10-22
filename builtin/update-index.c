/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/transaction.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "resolve-undo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "split-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "symlinks.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsmonitor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
ਸਥਿਰ ਪੂਰਨ allow_ਜੋੜੋ;
ਸਥਿਰ ਪੂਰਨ allow_remove;
ਸਥਿਰ ਪੂਰਨ allow_replace;
ਸਥਿਰ ਪੂਰਨ info_only;
ਸਥਿਰ ਪੂਰਨ force_remove;
ਸਥਿਰ ਪੂਰਨ verbose;
ਸਥਿਰ ਪੂਰਨ mark_valid_only;
ਸਥਿਰ ਪੂਰਨ mark_skip_worktree_only;
ਸਥਿਰ ਪੂਰਨ mark_fsmonitor_only;
ਸਥਿਰ ਪੂਰਨ ignore_skip_worktree_entries;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MARK_FLAG 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNMARK_FLAG 2
ਸਥਿਰ ਬਣਤਰ strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
ਗਿਣਤੀ uc_ਬਹੁਲਕ {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (ਲਿਖੋ, 1, 2)))
ਸਥਿਰ ਖਾਲੀ report(ਅਚਲ ਅੱਖਰ *fmt, ...)
{
	va_list vp;

	ਜੇ (!verbose)
		ਵਾਪਸ;

	va_ਸ਼ੁਰੂ(vp, fmt);
	vprintf(fmt, vp);
	ਅੱਖਰ_ਲਿਖੋ('\n');
	va_ਸਿਰਾ(vp);
}

ਸਥਿਰ ਖਾਲੀ remove_test_directory(ਖਾਲੀ)
{
	ਜੇ (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_mtime_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਸਥਿਰ ਬਣਤਰ strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, ਰਸਤਾ);
	ਵਾਪਸ sb.buf;
}

ਸਥਿਰ ਖਾਲੀ xmkdir(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਰਸਤਾ = get_mtime_ਰਸਤਾ(ਰਸਤਾ);
	ਜੇ (mkdir(ਰਸਤਾ, 0700))
		die_errno(_("failed to create directory %s"), ਰਸਤਾ);
}

ਸਥਿਰ ਪੂਰਨ xstat_mtime_dir(ਬਣਤਰ stat *st)
{
	ਜੇ (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ create_file(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਪੂਰਨ fd;
	ਰਸਤਾ = get_mtime_ਰਸਤਾ(ਰਸਤਾ);
	fd = xopen(ਰਸਤਾ, O_CREAT | O_RDWR, 0644);
	ਵਾਪਸ fd;
}

ਸਥਿਰ ਖਾਲੀ xunlink(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਰਸਤਾ = get_mtime_ਰਸਤਾ(ਰਸਤਾ);
	ਜੇ (unlink(ਰਸਤਾ))
		die_errno(_("failed to delete file %s"), ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ xrmdir(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਰਸਤਾ = get_mtime_ਰਸਤਾ(ਰਸਤਾ);
	ਜੇ (rmdir(ਰਸਤਾ))
		die_errno(_("failed to delete directory %s"), ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ avoid_racy(ਖਾਲੀ)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

ਸਥਿਰ ਪੂਰਨ test_if_untracked_cache_is_supported(ਖਾਲੀ)
{
	ਬਣਤਰ stat st;
	ਬਣਤਰ stat_ਅੰਕੜੇ ਅਧਾਰ;
	ਪੂਰਨ fd, ret = 0;
	ਅੱਖਰ *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	ਜੇ (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	ਮੁਕਤ(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_ਅੰਕੜੇ(&ਅਧਾਰ, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	ਜੇ (!ਮੇਲ_stat_ਅੰਕੜੇ(&ਅਧਾਰ, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		ਜਾਓ ਪੂਰਾ;
	}
	fill_stat_ਅੰਕੜੇ(&ਅਧਾਰ, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	ਜੇ (!ਮੇਲ_stat_ਅੰਕੜੇ(&ਅਧਾਰ, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		ਜਾਓ ਪੂਰਾ;
	}
	fill_stat_ਅੰਕੜੇ(&ਅਧਾਰ, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	ਜੇ (ਮੇਲ_stat_ਅੰਕੜੇ(&ਅਧਾਰ, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		ਜਾਓ ਪੂਰਾ;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	ਜੇ (ਮੇਲ_stat_ਅੰਕੜੇ(&ਅਧਾਰ, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		ਜਾਓ ਪੂਰਾ;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	ਜੇ (!ਮੇਲ_stat_ਅੰਕੜੇ(&ਅਧਾਰ, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		ਜਾਓ ਪੂਰਾ;
	}
	fill_stat_ਅੰਕੜੇ(&ਅਧਾਰ, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	ਜੇ (!ਮੇਲ_stat_ਅੰਕੜੇ(&ਅਧਾਰ, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

ਪੂਰਾ:
	strbuf_release(&mtime_dir);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ mark_ce_flags(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ flag, ਪੂਰਨ mark)
{
	ਪੂਰਨ namelen = ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ);
	ਪੂਰਨ pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ, namelen);
	ਜੇ (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->ਸੂਚਕਾਂਕ, the_repository->ਸੂਚਕਾਂਕ->cache[pos]);
		ਜੇ (mark)
			the_repository->ਸੂਚਕਾਂਕ->cache[pos]->ce_flags |= flag;
		ਨਹੀਂਤਾਂ
			the_repository->ਸੂਚਕਾਂਕ->cache[pos]->ce_flags &= ~flag;
		the_repository->ਸੂਚਕਾਂਕ->cache[pos]->ce_flags |= CE_UPDATE_ਵਿੱਚ_ਅਧਾਰ;
		cache_ਰੁੱਖ_invalidate_ਰਸਤਾ(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ);
		the_repository->ਸੂਚਕਾਂਕ->cache_ਬਦਲਿਆ |= CE_ਇੰਦਰਾਜ_ਬਦਲਿਆ;
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ -1;
}

ਸਥਿਰ ਪੂਰਨ remove_one_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਜੇ (!allow_remove)
		ਵਾਪਸ error("%s: does not exist and --remove not passed", ਰਸਤਾ);
	ਜੇ (remove_file_from_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ))
		ਵਾਪਸ error("%s: cannot remove from the index", ਰਸਤਾ);
	ਵਾਪਸ 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
ਸਥਿਰ ਪੂਰਨ process_lstat_error(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ err)
{
	ਜੇ (is_missing_file_error(err))
		ਵਾਪਸ remove_one_ਰਸਤਾ(ਰਸਤਾ);
	ਵਾਪਸ error("lstat(\"%s\"): %s", ਰਸਤਾ, strerror(err));
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_one_ਰਸਤਾ(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *old, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ len, ਬਣਤਰ stat *st)
{
	ਪੂਰਨ option;
	ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;

	/* Was the old index entry already up-to-date? */
	ਜੇ (old && !ce_stage(old) && !ie_ਮੇਲ_stat(the_repository->ਸੂਚਕਾਂਕ, old, st, 0))
		ਵਾਪਸ 0;

	ce = make_ਖਾਲੀ_ਥਾਵਾਂ_cache_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, len);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ce->name, ਰਸਤਾ, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->ਸੂਚਕਾਂਕ, ce, st);
	ce->ce_ਬਹੁਲਕ = ce_ਬਹੁਲਕ_from_stat(the_repository, old, st->st_ਬਹੁਲਕ);

	ਜੇ (ਸੂਚਕਾਂਕ_ਰਸਤਾ(the_repository->ਸੂਚਕਾਂਕ, &ce->oid, ਰਸਤਾ, st,
		       info_only ? 0 : ਸੂਚਕਾਂਕ_WRITE_OBJECT)) {
		discard_cache_ਇੰਦਰਾਜ(ce);
		ਵਾਪਸ -1;
	}
	option = allow_ਜੋੜੋ ? ਜੋੜੋ_CACHE_OK_ਮੰਜ਼ਿਲ_ਜੋੜੋ : 0;
	option |= allow_replace ? ਜੋੜੋ_CACHE_OK_ਮੰਜ਼ਿਲ_REPLACE : 0;
	ਜੇ (ਜੋੜੋ_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, ce, option)) {
		discard_cache_ਇੰਦਰਾਜ(ce);
		ਵਾਪਸ error("%s: cannot add to the index - missing --add option?", ਰਸਤਾ);
	}
	ਵਾਪਸ 0;
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
ਸਥਿਰ ਪੂਰਨ process_directory(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ len, ਬਣਤਰ stat *st)
{
	ਬਣਤਰ object_id oid;
	ਪੂਰਨ pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ, len);

	/* Exact match: file or existing gitlink */
	ਜੇ (pos >= 0) {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
		ਜੇ (S_ISGITLINK(ce->ce_ਬਹੁਲਕ)) {

			/* Do nothing to the index if there is no HEAD! */
			ਜੇ (repo_resolve_gitlink_ref(the_repository, ਰਸਤਾ,
						     "HEAD", &oid) < 0)
				ਵਾਪਸ 0;

			ਵਾਪਸ ਜੋੜੋ_one_ਰਸਤਾ(ce, ਰਸਤਾ, len, st);
		}
		/* Should this be an unconditional error? */
		ਵਾਪਸ remove_one_ਰਸਤਾ(ਰਸਤਾ);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	ਜਦੋਂ (pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr) {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos++];

		ਜੇ (ਲੜੀ_ਤੁਲਨਾ(ce->name, ਰਸਤਾ, len))
			ਰੋਕੋ;
		ਜੇ (ce->name[len] > '/')
			ਰੋਕੋ;
		ਜੇ (ce->name[len] < '/')
			ਜਾਰੀ;

		/* Subdirectory match - error out */
		ਵਾਪਸ error("%s: is a directory - add individual files instead", ਰਸਤਾ);
	}

	/* No match - should we add it as a gitlink? */
	ਜੇ (!repo_resolve_gitlink_ref(the_repository, ਰਸਤਾ, "HEAD", &oid))
		ਵਾਪਸ ਜੋੜੋ_one_ਰਸਤਾ(ਨਿਰਾ, ਰਸਤਾ, len, st);

	/* Error out. */
	ਵਾਪਸ error("%s: is a directory - add files inside instead", ਰਸਤਾ);
}

ਸਥਿਰ ਪੂਰਨ process_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਬਣਤਰ stat *st, ਪੂਰਨ stat_errno)
{
	ਪੂਰਨ pos, len;
	ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;

	len = ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ);
	ਜੇ (has_symlink_leading_ਰਸਤਾ(ਰਸਤਾ, len))
		ਵਾਪਸ error("'%s' is beyond a symbolic link", ਰਸਤਾ);

	pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ, len);
	ce = pos < 0 ? ਨਿਰਾ : the_repository->ਸੂਚਕਾਂਕ->cache[pos];
	ਜੇ (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		ਜੇ (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ))
			ਵਾਪਸ error("%s: cannot remove from the index", ਰਸਤਾ);
		ਵਾਪਸ 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	ਜੇ (stat_errno)
		ਵਾਪਸ process_lstat_error(ਰਸਤਾ, stat_errno);

	ਜੇ (S_ISDIR(st->st_ਬਹੁਲਕ))
		ਵਾਪਸ process_directory(ਰਸਤਾ, len, st);

	ਵਾਪਸ ਜੋੜੋ_one_ਰਸਤਾ(ce, ਰਸਤਾ, len, st);
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_cacheinfo(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ, ਅਚਲ ਬਣਤਰ object_id *oid,
			 ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ stage)
{
	ਪੂਰਨ len, option;
	ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;

	ਜੇ (!verify_ਰਸਤਾ(ਰਸਤਾ, ਬਹੁਲਕ))
		ਵਾਪਸ error("Invalid path '%s'", ਰਸਤਾ);

	len = ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ);
	ce = make_ਖਾਲੀ_ਥਾਵਾਂ_cache_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, len);

	oidcpy(&ce->oid, oid);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ce->name, ਰਸਤਾ, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_ਬਹੁਲਕ = create_ce_ਬਹੁਲਕ(ਬਹੁਲਕ);
	ਜੇ (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_ਜੋੜੋ ? ਜੋੜੋ_CACHE_OK_ਮੰਜ਼ਿਲ_ਜੋੜੋ : 0;
	option |= allow_replace ? ਜੋੜੋ_CACHE_OK_ਮੰਜ਼ਿਲ_REPLACE : 0;
	ਜੇ (ਜੋੜੋ_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, ce, option))
		ਵਾਪਸ error("%s: cannot add to the index - missing --add option?",
			     ਰਸਤਾ);
	report("add '%s'", ਰਸਤਾ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ chmod_ਰਸਤਾ(ਅੱਖਰ flip, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਪੂਰਨ pos;
	ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;

	pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ, ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ));
	ਜੇ (pos < 0)
		ਜਾਓ fail;
	ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
	ਜੇ (chmod_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, ce, flip) < 0)
		ਜਾਓ fail;

	report("chmod %cx '%s'", flip, ਰਸਤਾ);
	ਵਾਪਸ;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ update_one(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਪੂਰਨ stat_errno = 0;
	ਬਣਤਰ stat st;

	ਜੇ (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_ਬਹੁਲਕ = 0;
	ਨਹੀਂਤਾਂ ਜੇ (lstat(ਰਸਤਾ, &st) < 0) {
		st.st_ਬਹੁਲਕ = 0;
		stat_errno = errno;
	} /* else stat is valid */

	ਜੇ (!verify_ਰਸਤਾ(ਰਸਤਾ, st.st_ਬਹੁਲਕ)) {
		fprintf(stderr, "Ignoring path %s\n", ਰਸਤਾ);
		ਵਾਪਸ;
	}
	ਜੇ (mark_valid_only) {
		ਜੇ (mark_ce_flags(ਰਸਤਾ, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", ਰਸਤਾ);
		ਵਾਪਸ;
	}
	ਜੇ (mark_skip_worktree_only) {
		ਜੇ (mark_ce_flags(ਰਸਤਾ, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", ਰਸਤਾ);
		ਵਾਪਸ;
	}
	ਜੇ (mark_fsmonitor_only) {
		ਜੇ (mark_ce_flags(ਰਸਤਾ, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", ਰਸਤਾ);
		ਵਾਪਸ;
	}

	ਜੇ (force_remove) {
		ਜੇ (remove_file_from_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ))
			die("git update-index: unable to remove %s", ਰਸਤਾ);
		report("remove '%s'", ਰਸਤਾ);
		ਵਾਪਸ;
	}
	ਜੇ (process_ਰਸਤਾ(ਰਸਤਾ, &st, stat_errno))
		die("Unable to process path %s", ਰਸਤਾ);
	report("add '%s'", ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ read_ਸੂਚਕਾਂਕ_info(ਪੂਰਨ nul_ਪਦ_ਸਤਰ)
{
	ਅਚਲ ਪੂਰਨ hexsz = the_hash_algo->hexsz;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_ਪਦ_ਸਤਰ ? strbuf_getline_nul : strbuf_getline_lf;
	ਜਦੋਂ (getline_fn(&buf, stdin) != EOF) {
		ਅੱਖਰ *ptr, *tab;
		ਅੱਖਰ *ਰਸਤਾ_name;
		ਬਣਤਰ object_id oid;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ul;
		ਪੂਰਨ stage;

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
		ਜੇ (ptr == buf.buf || *ptr != ' '
		    || errno || (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ul != ul)
			ਜਾਓ bad_ਸਤਰ;
		ਬਹੁਲਕ = ul;

		tab = strchr(ptr, '\t');
		ਜੇ (!tab || tab - ptr < hexsz + 1)
			ਜਾਓ bad_ਸਤਰ;

		ਜੇ (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		ਨਹੀਂਤਾਂ {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		ਜੇ (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			ਜਾਓ bad_ਸਤਰ;

		ਰਸਤਾ_name = ptr;
		ਜੇ (!nul_ਪਦ_ਸਤਰ && ਰਸਤਾ_name[0] == '"') {
			strbuf_reset(&uq);
			ਜੇ (unquote_c_style(&uq, ਰਸਤਾ_name, ਨਿਰਾ)) {
				die("git update-index: bad quoting of path name");
			}
			ਰਸਤਾ_name = uq.buf;
		}

		ਜੇ (!verify_ਰਸਤਾ(ਰਸਤਾ_name, ਬਹੁਲਕ)) {
			fprintf(stderr, "Ignoring path %s\n", ਰਸਤਾ_name);
			ਜਾਰੀ;
		}

		ਜੇ (!ਬਹੁਲਕ) {
			/* mode == 0 means there is no such path -- remove */
			ਜੇ (remove_file_from_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		ਨਹੀਂਤਾਂ {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			ਜੇ (ਜੋੜੋ_cacheinfo(ਬਹੁਲਕ, &oid, ਰਸਤਾ_name, stage))
				die("git update-index: unable to update %s",
				    ਰਸਤਾ_name);
		}
		ਜਾਰੀ;

	bad_ਸਤਰ:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ update_ਸੂਚਕਾਂਕ_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਬਣਤਰ cache_ਇੰਦਰਾਜ *read_one_ent(ਅਚਲ ਅੱਖਰ *which,
					ਬਣਤਰ object_id *ent, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
					ਪੂਰਨ namelen, ਪੂਰਨ stage)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਬਹੁਲਕ;
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;

	ਜੇ (get_ਰੁੱਖ_ਇੰਦਰਾਜ(the_repository, ent, ਰਸਤਾ, &oid, &ਬਹੁਲਕ)) {
		ਜੇ (which)
			error("%s: not in %s branch.", ਰਸਤਾ, which);
		ਵਾਪਸ ਨਿਰਾ;
	}
	ਜੇ (!the_repository->ਸੂਚਕਾਂਕ->sparse_ਸੂਚਕਾਂਕ && ਬਹੁਲਕ == S_IFDIR) {
		ਜੇ (which)
			error("%s: not a blob in %s branch.", ਰਸਤਾ, which);
		ਵਾਪਸ ਨਿਰਾ;
	}
	ce = make_ਖਾਲੀ_ਥਾਵਾਂ_cache_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, namelen);

	oidcpy(&ce->oid, &oid);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ce->name, ਰਸਤਾ, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_ਬਹੁਲਕ = create_ce_ਬਹੁਲਕ(ਬਹੁਲਕ);
	ਵਾਪਸ ce;
}

ਸਥਿਰ ਪੂਰਨ unresolve_one(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਪੂਰਨ res = 0;

	ਜੇ (!the_repository->ਸੂਚਕਾਂਕ->resolve_undo)
		ਵਾਪਸ res;
	ਵਸਤੂ = string_list_lookup(the_repository->ਸੂਚਕਾਂਕ->resolve_undo, ਰਸਤਾ);
	ਜੇ (!ਵਸਤੂ)
		ਵਾਪਸ res; /* no resolve-undo record for the path */
	res = unmerge_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ, ਵਸਤੂ->util, 0);
	FREE_AND_NULL(ਵਸਤੂ->util);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ do_unresolve(ਪੂਰਨ ac, ਅਚਲ ਅੱਖਰ **av,
			ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਪੂਰਨ ਅਗੇਤਰ_ਲੰਮਾਈ)
{
	ਪੂਰਨ i;
	ਪੂਰਨ err = 0;

	ਲਈ (i = 1; i < ac; i++) {
		ਅਚਲ ਅੱਖਰ *arg = av[i];
		ਅੱਖਰ *p = ਅਗੇਤਰ_ਰਸਤਾ(the_repository, ਅਗੇਤਰ, ਅਗੇਤਰ_ਲੰਮਾਈ, arg);
		err |= unresolve_one(p);
		ਮੁਕਤ(p);
	}
	ਵਾਪਸ err;
}

ਸਥਿਰ ਪੂਰਨ do_reupdate(ਅਚਲ ਅੱਖਰ **paths,
		       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	ਪੂਰਨ pos;
	ਪੂਰਨ has_ਸਿਰ = 1;
	ਬਣਤਰ pathspec pathspec;
	ਬਣਤਰ object_id ਸਿਰ_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ਅਗੇਤਰ, paths);

	ਜੇ (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &ਸਿਰ_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_ਸਿਰ = 0;
 redo:
	ਲਈ (pos = 0; pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr; pos++) {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *old = ਨਿਰਾ;
		ਪੂਰਨ save_nr;
		ਅੱਖਰ *ਰਸਤਾ;

		ਜੇ (ce_stage(ce) || !ce_ਰਸਤਾ_ਮੇਲ(the_repository->ਸੂਚਕਾਂਕ, ce, &pathspec, ਨਿਰਾ))
			ਜਾਰੀ;
		ਜੇ (has_ਸਿਰ)
			old = read_one_ent(ਨਿਰਾ, &ਸਿਰ_oid,
					   ce->name, ce_namelen(ce), 0);
		ਜੇ (old && ce->ce_ਬਹੁਲਕ == old->ce_ਬਹੁਲਕ &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_ਇੰਦਰਾਜ(old);
			ਜਾਰੀ; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		ਜੇ (S_ISSPARSEDIR(ce->ce_ਬਹੁਲਕ)) {
			ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
			ਜਾਓ redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->ਸੂਚਕਾਂਕ->cache_nr;
		ਰਸਤਾ = xstrdup(ce->name);
		update_one(ਰਸਤਾ);
		ਮੁਕਤ(ਰਸਤਾ);
		discard_cache_ਇੰਦਰਾਜ(old);
		ਜੇ (save_nr != the_repository->ਸੂਚਕਾਂਕ->cache_nr)
			ਜਾਓ redo;
	}
	clear_pathspec(&pathspec);
	ਵਾਪਸ 0;
}

ਬਣਤਰ refresh_params {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags;
	ਪੂਰਨ *has_errors;
};

ਸਥਿਰ ਪੂਰਨ refresh(ਬਣਤਰ refresh_params *o, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flag)
{
	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
	repo_read_ਸੂਚਕਾਂਕ(the_repository);
	*o->has_errors |= refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, o->flags | flag, ਨਿਰਾ,
					ਨਿਰਾ, ਨਿਰਾ);
	ਜੇ (has_racy_timestamp(the_repository->ਸੂਚਕਾਂਕ)) {
		/*
		 * Even if nothing else has changed, updating the file
		 * increases the chance that racy timestamps become
		 * non-racy, helping future run-time performance.
		 * We do that even in case of "errors" returned by
		 * refresh_index() as these are no actual errors.
		 * cmd_status() does the same.
		 */
		the_repository->ਸੂਚਕਾਂਕ->cache_ਬਦਲਿਆ |= SOMETHING_ਬਦਲਿਆ;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ refresh_callback(ਅਚਲ ਬਣਤਰ option *opt,
				ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ਵਾਪਸ refresh(opt->ਮੁੱਲ, 0);
}

ਸਥਿਰ ਪੂਰਨ really_refresh_callback(ਅਚਲ ਬਣਤਰ option *opt,
				ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ਵਾਪਸ refresh(opt->ਮੁੱਲ, REFRESH_REALLY);
}

ਸਥਿਰ ਪੂਰਨ chmod_callback(ਅਚਲ ਬਣਤਰ option *opt,
				ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਅੱਖਰ *flip = opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	ਜੇ ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		ਵਾਪਸ error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ resolve_undo_clear_callback(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
				ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_new_style_cacheinfo(ਅਚਲ ਅੱਖਰ *arg,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਬਹੁਲਕ,
				     ਬਣਤਰ object_id *oid,
				     ਅਚਲ ਅੱਖਰ **ਰਸਤਾ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ul;
	ਅੱਖਰ *endp;
	ਅਚਲ ਅੱਖਰ *p;

	ਜੇ (!arg)
		ਵਾਪਸ -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	ਜੇ (errno || endp == arg || *endp != ',' || (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ul != ul)
		ਵਾਪਸ -1; /* not a new-style cacheinfo */
	*ਬਹੁਲਕ = ul;
	endp++;
	ਜੇ (parse_oid_hex(endp, oid, &p) || *p != ',')
		ਵਾਪਸ -1;
	*ਰਸਤਾ = p + 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਗਿਣਤੀ parse_opt_ਨਤੀਜਾ cacheinfo_callback(
	ਬਣਤਰ parse_opt_ctx_t *ctx, ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
	ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ object_id oid;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ਜੇ (!parse_new_style_cacheinfo(ctx->argv[1], &ਬਹੁਲਕ, &oid, &ਰਸਤਾ)) {
		ਜੇ (ਜੋੜੋ_cacheinfo(ਬਹੁਲਕ, &oid, ਰਸਤਾ, 0))
			die("git update-index: --cacheinfo cannot add %s", ਰਸਤਾ);
		ctx->argv++;
		ctx->argc--;
		ਵਾਪਸ 0;
	}
	ਜੇ (ctx->argc <= 3)
		ਵਾਪਸ error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	ਜੇ (strtoul_ui(*++ctx->argv, 8, &ਬਹੁਲਕ) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    ਜੋੜੋ_cacheinfo(ਬਹੁਲਕ, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਗਿਣਤੀ parse_opt_ਨਤੀਜਾ stdin_cacheinfo_callback(
	ਬਣਤਰ parse_opt_ctx_t *ctx, ਅਚਲ ਬਣਤਰ option *opt,
	ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *nul_ਪਦ_ਸਤਰ = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ਜੇ (ctx->argc != 1)
		ਵਾਪਸ error("option '%s' must be the last argument", opt->long_name);
	allow_ਜੋੜੋ = allow_replace = allow_remove = 1;
	read_ਸੂਚਕਾਂਕ_info(*nul_ਪਦ_ਸਤਰ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਗਿਣਤੀ parse_opt_ਨਤੀਜਾ stdin_callback(
	ਬਣਤਰ parse_opt_ctx_t *ctx, ਅਚਲ ਬਣਤਰ option *opt,
	ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *read_from_stdin = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ਜੇ (ctx->argc != 1)
		ਵਾਪਸ error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਗਿਣਤੀ parse_opt_ਨਤੀਜਾ unresolve_callback(
	ਬਣਤਰ parse_opt_ctx_t *ctx, ਅਚਲ ਬਣਤਰ option *opt,
	ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *has_errors = opt->ਮੁੱਲ;
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ = the_repository->ਅਗੇਤਰ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				ਅਗੇਤਰ, ਅਗੇਤਰ ? ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ) : 0);
	ਜੇ (*has_errors)
		the_repository->ਸੂਚਕਾਂਕ->cache_ਬਦਲਿਆ = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਗਿਣਤੀ parse_opt_ਨਤੀਜਾ reupdate_callback(
	ਬਣਤਰ parse_opt_ctx_t *ctx, ਅਚਲ ਬਣਤਰ option *opt,
	ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *has_errors = opt->ਮੁੱਲ;
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ = the_repository->ਅਗੇਤਰ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, ਅਗੇਤਰ);
	ਜੇ (*has_errors)
		the_repository->ਸੂਚਕਾਂਕ->cache_ਬਦਲਿਆ = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_update_ਸੂਚਕਾਂਕ(ਪੂਰਨ argc,
		     ਅਚਲ ਅੱਖਰ **argv,
		     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ newfd, entries, has_errors = 0, nul_ਪਦ_ਸਤਰ = 0;
	ਗਿਣਤੀ uc_ਬਹੁਲਕ untracked_cache = UC_UNSPECIFIED;
	ਪੂਰਨ read_from_stdin = 0;
	ਪੂਰਨ ਅਗੇਤਰ_ਲੰਮਾਈ = ਅਗੇਤਰ ? ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ) : 0;
	ਪੂਰਨ preferred_ਸੂਚਕਾਂਕ_format = 0;
	ਅੱਖਰ ਸੈੱਟ_executable_ਦੋਆਧਾਰੀ_ਅੰਕ = 0;
	ਬਣਤਰ refresh_params refresh_ਦਲੀਲਾਂ = {0, &has_errors};
	ਪੂਰਨ ਤਾਲਾ_error = 0;
	ਪੂਰਨ ਵੰਡੋ_ਸੂਚਕਾਂਕ = -1;
	ਪੂਰਨ force_write = 0;
	ਪੂਰਨ fsmonitor = -1;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਬਣਤਰ parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	ਪੂਰਨ parseopt_state = PARSE_OPT_UNKNOWN;
	ਬਣਤਰ repository *r = the_repository;
	ਬਣਤਰ odb_transaction *transaction;
	ਬਣਤਰ option options[] = {
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('q', ਨਿਰਾ, &refresh_ਦਲੀਲਾਂ.flags,
			N_("continue refresh even when index needs update"),
			REFRESH_QUIET),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "ignore-submodules", &refresh_ਦਲੀਲਾਂ.flags,
			N_("refresh: ignore submodules"),
			REFRESH_IGNORE_SUBMODULES),
		OPT_ਸੈੱਟ_INT(0, "add", &allow_ਜੋੜੋ,
			N_("do not ignore new files"), 1),
		OPT_ਸੈੱਟ_INT(0, "replace", &allow_replace,
			N_("let files replace directories and vice-versa"), 1),
		OPT_ਸੈੱਟ_INT(0, "remove", &allow_remove,
			N_("notice files missing from worktree"), 1),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "unmerged", &refresh_ਦਲੀਲਾਂ.flags,
			N_("refresh even if index contains unmerged entries"),
			REFRESH_UNMERGED),
		OPT_CALLBACK_F(0, "refresh", &refresh_ਦਲੀਲਾਂ, ਨਿਰਾ,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_ਦਲੀਲਾਂ, ਨਿਰਾ,
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
		OPT_CALLBACK_F(0, "chmod", &ਸੈੱਟ_executable_ਦੋਆਧਾਰੀ_ਅੰਕ, "(+|-)x",
			N_("override the executable bit of the listed files"),
			PARSE_OPT_NONEG,
			chmod_callback),
		{
			.type = OPTION_ਸੈੱਟ_INT,
			.long_name = "assume-unchanged",
			.ਮੁੱਲ = &mark_valid_only,
			.precision = ਆਕਾਰ(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_ਸੈੱਟ_INT,
			.long_name = "no-assume-unchanged",
			.ਮੁੱਲ = &mark_valid_only,
			.precision = ਆਕਾਰ(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_ਸੈੱਟ_INT,
			.long_name = "skip-worktree",
			.ਮੁੱਲ = &mark_skip_worktree_only,
			.precision = ਆਕਾਰ(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_ਸੈੱਟ_INT,
			.long_name = "no-skip-worktree",
			.ਮੁੱਲ = &mark_skip_worktree_only,
			.precision = ਆਕਾਰ(mark_skip_worktree_only),
			.help = N_("clear skip-worktree bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_BOOL(0, "ignore-skip-worktree-entries", &ignore_skip_worktree_entries,
			 N_("do not touch index-only entries")),
		OPT_ਸੈੱਟ_INT(0, "info-only", &info_only,
			N_("add to index only; do not add content to object database"), 1),
		OPT_ਸੈੱਟ_INT(0, "force-remove", &force_remove,
			N_("remove named paths even if present in worktree"), 1),
		OPT_BOOL('z', ਨਿਰਾ, &nul_ਪਦ_ਸਤਰ,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.ਮੁੱਲ = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.ਮੁੱਲ = &nul_ਪਦ_ਸਤਰ,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.ਮੁੱਲ = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.ਮੁੱਲ = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "ignore-missing", &refresh_ਦਲੀਲਾਂ.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_ਸੈੱਟ_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", ਨਿਰਾ, ਨਿਰਾ,
			N_("(for porcelains) forget saved unresolved conflicts"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			resolve_undo_clear_callback),
		OPT_INTEGER(0, "index-version", &preferred_ਸੂਚਕਾਂਕ_format,
			N_("write index in this format")),
		OPT_ਸੈੱਟ_INT(0, "show-index-version", &preferred_ਸੂਚਕਾਂਕ_format,
			    N_("report on-disk index format version"), -1),
		OPT_BOOL(0, "split-index", &ਵੰਡੋ_ਸੂਚਕਾਂਕ,
			N_("enable or disable split index")),
		OPT_BOOL(0, "untracked-cache", &untracked_cache,
			N_("enable/disable untracked cache")),
		OPT_ਸੈੱਟ_INT(0, "test-untracked-cache", &untracked_cache,
			    N_("test if the filesystem supports untracked cache"), UC_TEST),
		OPT_ਸੈੱਟ_INT(0, "force-untracked-cache", &untracked_cache,
			    N_("enable untracked cache without testing the filesystem"), UC_FORCE),
		OPT_ਸੈੱਟ_INT(0, "force-write-index", &force_write,
			N_("write out the index even if is not flagged as changed"), 1),
		OPT_BOOL(0, "fsmonitor", &fsmonitor,
			N_("enable or disable file system monitor")),
		{
			.type = OPTION_ਸੈੱਟ_INT,
			.long_name = "fsmonitor-valid",
			.ਮੁੱਲ = &mark_fsmonitor_only,
			.precision = ਆਕਾਰ(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_ਸੈੱਟ_INT,
			.long_name = "no-fsmonitor-valid",
			.ਮੁੱਲ = &mark_fsmonitor_only,
			.precision = ਆਕਾਰ(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_ਸਿਰਾ()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_ਸੂਚਕਾਂਕ_usage, options);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, 0);
	ਜੇ (newfd < 0)
		ਤਾਲਾ_error = errno;

	entries = repo_read_ਸੂਚਕਾਂਕ(the_repository);
	ਜੇ (entries < 0)
		die("cache corrupted");

	the_repository->ਸੂਚਕਾਂਕ->updated_skipworktree = 1;

	/*
	 * Custom copy of parse_options() because we want to handle
	 * filename arguments as they come.
	 */
	parse_options_ਸ਼ੁਰੂ(&ctx, argc, argv, ਅਗੇਤਰ,
			    options, PARSE_OPT_STOP_AT_NON_OPTION);

	/*
	 * Allow the object layer to optimize adding multiple objects in
	 * a batch.
	 */
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	ਜਦੋਂ (ctx.argc) {
		ਜੇ (parseopt_state != PARSE_OPT_ਪੂਰਾ)
			parseopt_state = parse_options_ਕਦਮ(&ctx, options,
							    update_ਸੂਚਕਾਂਕ_usage);
		ਜੇ (!ctx.argc)
			ਰੋਕੋ;
		ਚੋਣ (parseopt_state) {
		ਹਾਲਤ PARSE_OPT_HELP:
			ਬਾਹਰ_ਨਿਕਲੋ(0);
		ਹਾਲਤ PARSE_OPT_HELP_ERROR:
		ਹਾਲਤ PARSE_OPT_ERROR:
			ਬਾਹਰ_ਨਿਕਲੋ(129);
		ਹਾਲਤ PARSE_OPT_COMPLETE:
			ਬਾਹਰ_ਨਿਕਲੋ(0);
		ਹਾਲਤ PARSE_OPT_NON_OPTION:
		ਹਾਲਤ PARSE_OPT_ਪੂਰਾ:
		{
			ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = ctx.argv[0];
			ਅੱਖਰ *p;

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
			ਜੇ (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = ਨਿਰਾ;
			}

			ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
			p = ਅਗੇਤਰ_ਰਸਤਾ(the_repository, ਅਗੇਤਰ, ਅਗੇਤਰ_ਲੰਮਾਈ, ਰਸਤਾ);
			update_one(p);
			ਜੇ (ਸੈੱਟ_executable_ਦੋਆਧਾਰੀ_ਅੰਕ)
				chmod_ਰਸਤਾ(ਸੈੱਟ_executable_ਦੋਆਧਾਰੀ_ਅੰਕ, p);
			ਮੁਕਤ(p);
			ctx.argc--;
			ctx.argv++;
			ਰੋਕੋ;
		}
		ਹਾਲਤ PARSE_OPT_UNKNOWN:
			ਜੇ (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			ਨਹੀਂਤਾਂ
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_ਸੂਚਕਾਂਕ_usage, options);
		}
	}
	argc = parse_options_ਸਿਰਾ(&ctx);

	getline_fn = nul_ਪਦ_ਸਤਰ ? strbuf_getline_nul : strbuf_getline_lf;
	ਜੇ (preferred_ਸੂਚਕਾਂਕ_format) {
		ਜੇ (preferred_ਸੂਚਕਾਂਕ_format < 0) {
			ਲਿਖੋ(_("%d\n"), the_repository->ਸੂਚਕਾਂਕ->ਸੰਸਕਰਣ);
		} ਨਹੀਂਤਾਂ ਜੇ (preferred_ਸੂਚਕਾਂਕ_format < ਸੂਚਕਾਂਕ_FORMAT_LB ||
			   ਸੂਚਕਾਂਕ_FORMAT_UB < preferred_ਸੂਚਕਾਂਕ_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_ਸੂਚਕਾਂਕ_format,
			    ਸੂਚਕਾਂਕ_FORMAT_LB, ਸੂਚਕਾਂਕ_FORMAT_UB);
		} ਨਹੀਂਤਾਂ {
			ਜੇ (the_repository->ਸੂਚਕਾਂਕ->ਸੰਸਕਰਣ != preferred_ਸੂਚਕਾਂਕ_format)
				the_repository->ਸੂਚਕਾਂਕ->cache_ਬਦਲਿਆ |= SOMETHING_ਬਦਲਿਆ;
			report(_("index-version: was %d, set to %d"),
			       the_repository->ਸੂਚਕਾਂਕ->ਸੰਸਕਰਣ, preferred_ਸੂਚਕਾਂਕ_format);
			the_repository->ਸੂਚਕਾਂਕ->ਸੰਸਕਰਣ = preferred_ਸੂਚਕਾਂਕ_format;
		}
	}

	ਜੇ (read_from_stdin) {
		ਬਣਤਰ strbuf buf = STRBUF_INIT;
		ਬਣਤਰ strbuf unquoted = STRBUF_INIT;

		ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
		ਜਦੋਂ (getline_fn(&buf, stdin) != EOF) {
			ਅੱਖਰ *p;
			ਜੇ (!nul_ਪਦ_ਸਤਰ && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ਜੇ (unquote_c_style(&unquoted, buf.buf, ਨਿਰਾ))
					die("line is badly quoted");
				strbuf_ਅਦਲਾ_ਬਦਲੀ(&buf, &unquoted);
			}
			p = ਅਗੇਤਰ_ਰਸਤਾ(the_repository, ਅਗੇਤਰ, ਅਗੇਤਰ_ਲੰਮਾਈ, buf.buf);
			update_one(p);
			ਜੇ (ਸੈੱਟ_executable_ਦੋਆਧਾਰੀ_ਅੰਕ)
				chmod_ਰਸਤਾ(ਸੈੱਟ_executable_ਦੋਆਧਾਰੀ_ਅੰਕ, p);
			ਮੁਕਤ(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	ਜੇ (ਵੰਡੋ_ਸੂਚਕਾਂਕ > 0) {
		ਜੇ (repo_config_get_ਵੰਡੋ_ਸੂਚਕਾਂਕ(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		ਜੇ (the_repository->ਸੂਚਕਾਂਕ->ਵੰਡੋ_ਸੂਚਕਾਂਕ)
			the_repository->ਸੂਚਕਾਂਕ->cache_ਬਦਲਿਆ |= ਵੰਡੋ_ਸੂਚਕਾਂਕ_ORDERED;
		ਨਹੀਂਤਾਂ
			ਜੋੜੋ_ਵੰਡੋ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	} ਨਹੀਂਤਾਂ ਜੇ (!ਵੰਡੋ_ਸੂਚਕਾਂਕ) {
		ਜੇ (repo_config_get_ਵੰਡੋ_ਸੂਚਕਾਂਕ(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_ਵੰਡੋ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	}

	prepare_repo_settings(r);
	ਚੋਣ (untracked_cache) {
	ਹਾਲਤ UC_UNSPECIFIED:
		ਰੋਕੋ;
	ਹਾਲਤ UC_DISABLE:
		ਜੇ (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->ਸੂਚਕਾਂਕ);
		report(_("Untracked cache disabled"));
		ਰੋਕੋ;
	ਹਾਲਤ UC_TEST:
		ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
		ਵਾਪਸ !test_if_untracked_cache_is_supported();
	ਹਾਲਤ UC_ENABLE:
	ਹਾਲਤ UC_FORCE:
		ਜੇ (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		ਜੋੜੋ_untracked_cache(the_repository->ਸੂਚਕਾਂਕ);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_ਰੁੱਖ(the_repository));
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	ਜੇ (fsmonitor > 0) {
		ਗਿਣਤੀ fsmonitor_ਬਹੁਲਕ fsm_ਬਹੁਲਕ = fsm_settings_get_ਬਹੁਲਕ(r);
		ਗਿਣਤੀ fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		ਜੇ (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_ਸੁਨੇਹਾ(r, reason));

		ਜੇ (fsm_ਬਹੁਲਕ == FSMONITOR_ਬਹੁਲਕ_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		ਜੋੜੋ_fsmonitor(the_repository->ਸੂਚਕਾਂਕ);
		report(_("fsmonitor enabled"));
	} ਨਹੀਂਤਾਂ ਜੇ (!fsmonitor) {
		ਗਿਣਤੀ fsmonitor_ਬਹੁਲਕ fsm_ਬਹੁਲਕ = fsm_settings_get_ਬਹੁਲਕ(r);
		ਜੇ (fsm_ਬਹੁਲਕ > FSMONITOR_ਬਹੁਲਕ_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->ਸੂਚਕਾਂਕ);
		report(_("fsmonitor disabled"));
	}

	ਜੇ (the_repository->ਸੂਚਕਾਂਕ->cache_ਬਦਲਿਆ || force_write) {
		ਜੇ (newfd < 0) {
			ਜੇ (refresh_ਦਲੀਲਾਂ.flags & REFRESH_QUIET)
				ਬਾਹਰ_ਨਿਕਲੋ(128);
			unable_ਮੰਜ਼ਿਲ_ਤਾਲਾ_die(repo_get_ਸੂਚਕਾਂਕ_file(the_repository), ਤਾਲਾ_error);
		}
		ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file, COMMIT_ਤਾਲਾ))
			die("Unable to write new index file");
	}

	rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);

	ਵਾਪਸ has_errors ? 1 : 0;
}
