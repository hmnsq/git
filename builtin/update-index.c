/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ allow_ಕೂಡಿಸು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ allow_remove;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ allow_replace;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ info_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ force_remove;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verbose;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_valid_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_skip_worktree_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_fsmonitor_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ignore_skip_worktree_entries;
#ವ್ಯಾಖ್ಯಾನಿಸು MARK_FLAG 1
#ವ್ಯಾಖ್ಯಾನಿಸು UNMARK_FLAG 2
ಸ್ಥಿರ ರಚನೆ strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
ಎಣಿಕೆ uc_ಬಹುಲಕ {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (ಬರೆ, 1, 2)))
ಸ್ಥಿರ ಶೂನ್ಯ report(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
	va_list vp;

	ಆದರೆ (!verbose)
		ಹಿಂತಿರುಗಿ;

	va_ಆರಂಭ(vp, fmt);
	vprintf(fmt, vp);
	ಅಕ್ಷರ_ಬರೆ('\n');
	va_ಕೊನೆ(vp);
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_test_directory(ಶೂನ್ಯ)
{
	ಆದರೆ (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_mtime_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಸ್ಥಿರ ರಚನೆ strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ sb.buf;
}

ಸ್ಥಿರ ಶೂನ್ಯ xmkdir(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಮಾರ್ಗ = get_mtime_ಮಾರ್ಗ(ಮಾರ್ಗ);
	ಆದರೆ (mkdir(ಮಾರ್ಗ, 0700))
		die_errno(_("failed to create directory %s"), ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ xstat_mtime_dir(ರಚನೆ stat *st)
{
	ಆದರೆ (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ create_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಪೂರ್ಣಾಂಕ fd;
	ಮಾರ್ಗ = get_mtime_ಮಾರ್ಗ(ಮಾರ್ಗ);
	fd = xopen(ಮಾರ್ಗ, O_CREAT | O_RDWR, 0644);
	ಹಿಂತಿರುಗಿ fd;
}

ಸ್ಥಿರ ಶೂನ್ಯ xunlink(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಮಾರ್ಗ = get_mtime_ಮಾರ್ಗ(ಮಾರ್ಗ);
	ಆದರೆ (unlink(ಮಾರ್ಗ))
		die_errno(_("failed to delete file %s"), ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ xrmdir(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಮಾರ್ಗ = get_mtime_ಮಾರ್ಗ(ಮಾರ್ಗ);
	ಆದರೆ (rmdir(ಮಾರ್ಗ))
		die_errno(_("failed to delete directory %s"), ಮಾರ್ಗ);
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

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ test_if_untracked_cache_is_supported(ಶೂನ್ಯ)
{
	ರಚನೆ stat st;
	ರಚನೆ stat_ದತ್ತಾಂಶ ತಳ;
	ಪೂರ್ಣಾಂಕ fd, ret = 0;
	ಅಕ್ಷರ *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	ಆದರೆ (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	ಬಿಡುಗಡೆ(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_ದತ್ತಾಂಶ(&ತಳ, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	ಆದರೆ (!ಹೊಂದಿಕೆ_stat_ದತ್ತಾಂಶ(&ತಳ, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		ಹೋಗು ಮುಗಿದ;
	}
	fill_stat_ದತ್ತಾಂಶ(&ತಳ, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	ಆದರೆ (!ಹೊಂದಿಕೆ_stat_ದತ್ತಾಂಶ(&ತಳ, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		ಹೋಗು ಮುಗಿದ;
	}
	fill_stat_ದತ್ತಾಂಶ(&ತಳ, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	ಆದರೆ (ಹೊಂದಿಕೆ_stat_ದತ್ತಾಂಶ(&ತಳ, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		ಹೋಗು ಮುಗಿದ;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	ಆದರೆ (ಹೊಂದಿಕೆ_stat_ದತ್ತಾಂಶ(&ತಳ, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		ಹೋಗು ಮುಗಿದ;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	ಆದರೆ (!ಹೊಂದಿಕೆ_stat_ದತ್ತಾಂಶ(&ತಳ, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		ಹೋಗು ಮುಗಿದ;
	}
	fill_stat_ದತ್ತಾಂಶ(&ತಳ, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	ಆದರೆ (!ಹೊಂದಿಕೆ_stat_ದತ್ತಾಂಶ(&ತಳ, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

ಮುಗಿದ:
	strbuf_release(&mtime_dir);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_ce_flags(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ flag, ಪೂರ್ಣಾಂಕ mark)
{
	ಪೂರ್ಣಾಂಕ namelen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ);
	ಪೂರ್ಣಾಂಕ pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ, namelen);
	ಆದರೆ (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->ಸೂಚ್ಯಂಕ, the_repository->ಸೂಚ್ಯಂಕ->cache[pos]);
		ಆದರೆ (mark)
			the_repository->ಸೂಚ್ಯಂಕ->cache[pos]->ce_flags |= flag;
		ಇಲ್ಲದಿದ್ದರೆ
			the_repository->ಸೂಚ್ಯಂಕ->cache[pos]->ce_flags &= ~flag;
		the_repository->ಸೂಚ್ಯಂಕ->cache[pos]->ce_flags |= CE_UPDATE_ಒಳ_ತಳ;
		cache_ವೃಕ್ಷ_invalidate_ಮಾರ್ಗ(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ);
		the_repository->ಸೂಚ್ಯಂಕ->cache_ಬದಲಾಯಿತು |= CE_ನಮೂದು_ಬದಲಾಯಿತು;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ remove_one_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಆದರೆ (!allow_remove)
		ಹಿಂತಿರುಗಿ error("%s: does not exist and --remove not passed", ಮಾರ್ಗ);
	ಆದರೆ (remove_file_from_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ))
		ಹಿಂತಿರುಗಿ error("%s: cannot remove from the index", ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ process_lstat_error(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ err)
{
	ಆದರೆ (is_missing_file_error(err))
		ಹಿಂತಿರುಗಿ remove_one_ಮಾರ್ಗ(ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ error("lstat(\"%s\"): %s", ಮಾರ್ಗ, strerror(err));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_one_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *old, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ len, ರಚನೆ stat *st)
{
	ಪೂರ್ಣಾಂಕ option;
	ರಚನೆ cache_ನಮೂದು *ce;

	/* Was the old index entry already up-to-date? */
	ಆದರೆ (old && !ce_stage(old) && !ie_ಹೊಂದಿಕೆ_stat(the_repository->ಸೂಚ್ಯಂಕ, old, st, 0))
		ಹಿಂತಿರುಗಿ 0;

	ce = make_ಖಾಲಿಗಳು_cache_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, len);
	ಸ್ಮರಣೆ_ನಕಲು(ce->name, ಮಾರ್ಗ, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->ಸೂಚ್ಯಂಕ, ce, st);
	ce->ce_ಬಹುಲಕ = ce_ಬಹುಲಕ_from_stat(the_repository, old, st->st_ಬಹುಲಕ);

	ಆದರೆ (ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ(the_repository->ಸೂಚ್ಯಂಕ, &ce->oid, ಮಾರ್ಗ, st,
		       info_only ? 0 : ಸೂಚ್ಯಂಕ_WRITE_OBJECT)) {
		discard_cache_ನಮೂದು(ce);
		ಹಿಂತಿರುಗಿ -1;
	}
	option = allow_ಕೂಡಿಸು ? ಕೂಡಿಸು_CACHE_OK_ಗಮ್ಯ_ಕೂಡಿಸು : 0;
	option |= allow_replace ? ಕೂಡಿಸು_CACHE_OK_ಗಮ್ಯ_REPLACE : 0;
	ಆದರೆ (ಕೂಡಿಸು_ಸೂಚ್ಯಂಕ_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, ce, option)) {
		discard_cache_ನಮೂದು(ce);
		ಹಿಂತಿರುಗಿ error("%s: cannot add to the index - missing --add option?", ಮಾರ್ಗ);
	}
	ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ process_directory(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ len, ರಚನೆ stat *st)
{
	ರಚನೆ object_id oid;
	ಪೂರ್ಣಾಂಕ pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ, len);

	/* Exact match: file or existing gitlink */
	ಆದರೆ (pos >= 0) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
		ಆದರೆ (S_ISGITLINK(ce->ce_ಬಹುಲಕ)) {

			/* Do nothing to the index if there is no HEAD! */
			ಆದರೆ (repo_resolve_gitlink_ref(the_repository, ಮಾರ್ಗ,
						     "HEAD", &oid) < 0)
				ಹಿಂತಿರುಗಿ 0;

			ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_one_ಮಾರ್ಗ(ce, ಮಾರ್ಗ, len, st);
		}
		/* Should this be an unconditional error? */
		ಹಿಂತಿರುಗಿ remove_one_ಮಾರ್ಗ(ಮಾರ್ಗ);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	ಆಗ (pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos++];

		ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ce->name, ಮಾರ್ಗ, len))
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (ce->name[len] > '/')
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (ce->name[len] < '/')
			ಮುಂದುವರಿ;

		/* Subdirectory match - error out */
		ಹಿಂತಿರುಗಿ error("%s: is a directory - add individual files instead", ಮಾರ್ಗ);
	}

	/* No match - should we add it as a gitlink? */
	ಆದರೆ (!repo_resolve_gitlink_ref(the_repository, ಮಾರ್ಗ, "HEAD", &oid))
		ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_one_ಮಾರ್ಗ(ಖಾಲಿ, ಮಾರ್ಗ, len, st);

	/* Error out. */
	ಹಿಂತಿರುಗಿ error("%s: is a directory - add files inside instead", ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ process_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ರಚನೆ stat *st, ಪೂರ್ಣಾಂಕ stat_errno)
{
	ಪೂರ್ಣಾಂಕ pos, len;
	ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce;

	len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ);
	ಆದರೆ (has_symlink_leading_ಮಾರ್ಗ(ಮಾರ್ಗ, len))
		ಹಿಂತಿರುಗಿ error("'%s' is beyond a symbolic link", ಮಾರ್ಗ);

	pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ, len);
	ce = pos < 0 ? ಖಾಲಿ : the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
	ಆದರೆ (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		ಆದರೆ (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ))
			ಹಿಂತಿರುಗಿ error("%s: cannot remove from the index", ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	ಆದರೆ (stat_errno)
		ಹಿಂತಿರುಗಿ process_lstat_error(ಮಾರ್ಗ, stat_errno);

	ಆದರೆ (S_ISDIR(st->st_ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ process_directory(ಮಾರ್ಗ, len, st);

	ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_one_ಮಾರ್ಗ(ce, ಮಾರ್ಗ, len, st);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_cacheinfo(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ stage)
{
	ಪೂರ್ಣಾಂಕ len, option;
	ರಚನೆ cache_ನಮೂದು *ce;

	ಆದರೆ (!verify_ಮಾರ್ಗ(ಮಾರ್ಗ, ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ error("Invalid path '%s'", ಮಾರ್ಗ);

	len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ);
	ce = make_ಖಾಲಿಗಳು_cache_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, len);

	oidcpy(&ce->oid, oid);
	ಸ್ಮರಣೆ_ನಕಲು(ce->name, ಮಾರ್ಗ, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_ಬಹುಲಕ = create_ce_ಬಹುಲಕ(ಬಹುಲಕ);
	ಆದರೆ (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_ಕೂಡಿಸು ? ಕೂಡಿಸು_CACHE_OK_ಗಮ್ಯ_ಕೂಡಿಸು : 0;
	option |= allow_replace ? ಕೂಡಿಸು_CACHE_OK_ಗಮ್ಯ_REPLACE : 0;
	ಆದರೆ (ಕೂಡಿಸು_ಸೂಚ್ಯಂಕ_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, ce, option))
		ಹಿಂತಿರುಗಿ error("%s: cannot add to the index - missing --add option?",
			     ಮಾರ್ಗ);
	report("add '%s'", ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ chmod_ಮಾರ್ಗ(ಅಕ್ಷರ flip, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಪೂರ್ಣಾಂಕ pos;
	ರಚನೆ cache_ನಮೂದು *ce;

	pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ));
	ಆದರೆ (pos < 0)
		ಹೋಗು fail;
	ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
	ಆದರೆ (chmod_ಸೂಚ್ಯಂಕ_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, ce, flip) < 0)
		ಹೋಗು fail;

	report("chmod %cx '%s'", flip, ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ update_one(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಪೂರ್ಣಾಂಕ stat_errno = 0;
	ರಚನೆ stat st;

	ಆದರೆ (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_ಬಹುಲಕ = 0;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (lstat(ಮಾರ್ಗ, &st) < 0) {
		st.st_ಬಹುಲಕ = 0;
		stat_errno = errno;
	} /* else stat is valid */

	ಆದರೆ (!verify_ಮಾರ್ಗ(ಮಾರ್ಗ, st.st_ಬಹುಲಕ)) {
		fprintf(stderr, "Ignoring path %s\n", ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ;
	}
	ಆದರೆ (mark_valid_only) {
		ಆದರೆ (mark_ce_flags(ಮಾರ್ಗ, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ;
	}
	ಆದರೆ (mark_skip_worktree_only) {
		ಆದರೆ (mark_ce_flags(ಮಾರ್ಗ, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ;
	}
	ಆದರೆ (mark_fsmonitor_only) {
		ಆದರೆ (mark_ce_flags(ಮಾರ್ಗ, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (force_remove) {
		ಆದರೆ (remove_file_from_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ))
			die("git update-index: unable to remove %s", ಮಾರ್ಗ);
		report("remove '%s'", ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ;
	}
	ಆದರೆ (process_ಮಾರ್ಗ(ಮಾರ್ಗ, &st, stat_errno))
		die("Unable to process path %s", ಮಾರ್ಗ);
	report("add '%s'", ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ read_ಸೂಚ್ಯಂಕ_info(ಪೂರ್ಣಾಂಕ nul_ಪದ_ಸಾಲು)
{
	ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ hexsz = the_hash_algo->hexsz;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_ಪದ_ಸಾಲು ? strbuf_getline_nul : strbuf_getline_lf;
	ಆಗ (getline_fn(&buf, stdin) != EOF) {
		ಅಕ್ಷರ *ptr, *tab;
		ಅಕ್ಷರ *ಮಾರ್ಗ_name;
		ರಚನೆ object_id oid;
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ul;
		ಪೂರ್ಣಾಂಕ stage;

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
		ಆದರೆ (ptr == buf.buf || *ptr != ' '
		    || errno || (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) ul != ul)
			ಹೋಗು bad_ಸಾಲು;
		ಬಹುಲಕ = ul;

		tab = strchr(ptr, '\t');
		ಆದರೆ (!tab || tab - ptr < hexsz + 1)
			ಹೋಗು bad_ಸಾಲು;

		ಆದರೆ (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		ಇಲ್ಲದಿದ್ದರೆ {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		ಆದರೆ (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			ಹೋಗು bad_ಸಾಲು;

		ಮಾರ್ಗ_name = ptr;
		ಆದರೆ (!nul_ಪದ_ಸಾಲು && ಮಾರ್ಗ_name[0] == '"') {
			strbuf_reset(&uq);
			ಆದರೆ (unquote_c_style(&uq, ಮಾರ್ಗ_name, ಖಾಲಿ)) {
				die("git update-index: bad quoting of path name");
			}
			ಮಾರ್ಗ_name = uq.buf;
		}

		ಆದರೆ (!verify_ಮಾರ್ಗ(ಮಾರ್ಗ_name, ಬಹುಲಕ)) {
			fprintf(stderr, "Ignoring path %s\n", ಮಾರ್ಗ_name);
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (!ಬಹುಲಕ) {
			/* mode == 0 means there is no such path -- remove */
			ಆದರೆ (remove_file_from_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		ಇಲ್ಲದಿದ್ದರೆ {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			ಆದರೆ (ಕೂಡಿಸು_cacheinfo(ಬಹುಲಕ, &oid, ಮಾರ್ಗ_name, stage))
				die("git update-index: unable to update %s",
				    ಮಾರ್ಗ_name);
		}
		ಮುಂದುವರಿ;

	bad_ಸಾಲು:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ update_ಸೂಚ್ಯಂಕ_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ರಚನೆ cache_ನಮೂದು *read_one_ent(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *which,
					ರಚನೆ object_id *ent, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
					ಪೂರ್ಣಾಂಕ namelen, ಪೂರ್ಣಾಂಕ stage)
{
	ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ಬಹುಲಕ;
	ರಚನೆ object_id oid;
	ರಚನೆ cache_ನಮೂದು *ce;

	ಆದರೆ (get_ವೃಕ್ಷ_ನಮೂದು(the_repository, ent, ಮಾರ್ಗ, &oid, &ಬಹುಲಕ)) {
		ಆದರೆ (which)
			error("%s: not in %s branch.", ಮಾರ್ಗ, which);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}
	ಆದರೆ (!the_repository->ಸೂಚ್ಯಂಕ->sparse_ಸೂಚ್ಯಂಕ && ಬಹುಲಕ == S_IFDIR) {
		ಆದರೆ (which)
			error("%s: not a blob in %s branch.", ಮಾರ್ಗ, which);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}
	ce = make_ಖಾಲಿಗಳು_cache_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, namelen);

	oidcpy(&ce->oid, &oid);
	ಸ್ಮರಣೆ_ನಕಲು(ce->name, ಮಾರ್ಗ, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_ಬಹುಲಕ = create_ce_ಬಹುಲಕ(ಬಹುಲಕ);
	ಹಿಂತಿರುಗಿ ce;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ unresolve_one(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಪೂರ್ಣಾಂಕ res = 0;

	ಆದರೆ (!the_repository->ಸೂಚ್ಯಂಕ->resolve_undo)
		ಹಿಂತಿರುಗಿ res;
	ಅಂಶ = string_list_lookup(the_repository->ಸೂಚ್ಯಂಕ->resolve_undo, ಮಾರ್ಗ);
	ಆದರೆ (!ಅಂಶ)
		ಹಿಂತಿರುಗಿ res; /* no resolve-undo record for the path */
	res = unmerge_ಸೂಚ್ಯಂಕ_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ, ಅಂಶ->util, 0);
	FREE_AND_NULL(ಅಂಶ->util);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_unresolve(ಪೂರ್ಣಾಂಕ ac, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **av,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ err = 0;

	ಗಾಗಿ (i = 1; i < ac; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = av[i];
		ಅಕ್ಷರ *p = ಪೂರ್ವಪ್ರತ್ಯಯ_ಮಾರ್ಗ(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ, arg);
		err |= unresolve_one(p);
		ಬಿಡುಗಡೆ(p);
	}
	ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_reupdate(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **paths,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	ಪೂರ್ಣಾಂಕ pos;
	ಪೂರ್ಣಾಂಕ has_ತಲೆ = 1;
	ರಚನೆ pathspec pathspec;
	ರಚನೆ object_id ತಲೆ_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, paths);

	ಆದರೆ (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &ತಲೆ_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_ತಲೆ = 0;
 redo:
	ಗಾಗಿ (pos = 0; pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; pos++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
		ರಚನೆ cache_ನಮೂದು *old = ಖಾಲಿ;
		ಪೂರ್ಣಾಂಕ save_nr;
		ಅಕ್ಷರ *ಮಾರ್ಗ;

		ಆದರೆ (ce_stage(ce) || !ce_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(the_repository->ಸೂಚ್ಯಂಕ, ce, &pathspec, ಖಾಲಿ))
			ಮುಂದುವರಿ;
		ಆದರೆ (has_ತಲೆ)
			old = read_one_ent(ಖಾಲಿ, &ತಲೆ_oid,
					   ce->name, ce_namelen(ce), 0);
		ಆದರೆ (old && ce->ce_ಬಹುಲಕ == old->ce_ಬಹುಲಕ &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_ನಮೂದು(old);
			ಮುಂದುವರಿ; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		ಆದರೆ (S_ISSPARSEDIR(ce->ce_ಬಹುಲಕ)) {
			ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
			ಹೋಗು redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->ಸೂಚ್ಯಂಕ->cache_nr;
		ಮಾರ್ಗ = xstrdup(ce->name);
		update_one(ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
		discard_cache_ನಮೂದು(old);
		ಆದರೆ (save_nr != the_repository->ಸೂಚ್ಯಂಕ->cache_nr)
			ಹೋಗು redo;
	}
	clear_pathspec(&pathspec);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ refresh_params {
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags;
	ಪೂರ್ಣಾಂಕ *has_errors;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ refresh(ರಚನೆ refresh_params *o, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flag)
{
	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
	repo_read_ಸೂಚ್ಯಂಕ(the_repository);
	*o->has_errors |= refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, o->flags | flag, ಖಾಲಿ,
					ಖಾಲಿ, ಖಾಲಿ);
	ಆದರೆ (has_racy_timestamp(the_repository->ಸೂಚ್ಯಂಕ)) {
		/*
		 * Even if nothing else has changed, updating the file
		 * increases the chance that racy timestamps become
		 * non-racy, helping future run-time performance.
		 * We do that even in case of "errors" returned by
		 * refresh_index() as these are no actual errors.
		 * cmd_status() does the same.
		 */
		the_repository->ಸೂಚ್ಯಂಕ->cache_ಬದಲಾಯಿತು |= SOMETHING_ಬದಲಾಯಿತು;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ refresh_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ಹಿಂತಿರುಗಿ refresh(opt->ಮೌಲ್ಯ, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ really_refresh_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ಹಿಂತಿರುಗಿ refresh(opt->ಮೌಲ್ಯ, REFRESH_REALLY);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ chmod_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಅಕ್ಷರ *flip = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	ಆದರೆ ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		ಹಿಂತಿರುಗಿ error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ resolve_undo_clear_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_new_style_cacheinfo(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *ಬಹುಲಕ,
				     ರಚನೆ object_id *oid,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮಾರ್ಗ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ul;
	ಅಕ್ಷರ *endp;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;

	ಆದರೆ (!arg)
		ಹಿಂತಿರುಗಿ -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	ಆದರೆ (errno || endp == arg || *endp != ',' || (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) ul != ul)
		ಹಿಂತಿರುಗಿ -1; /* not a new-style cacheinfo */
	*ಬಹುಲಕ = ul;
	endp++;
	ಆದರೆ (parse_oid_hex(endp, oid, &p) || *p != ',')
		ಹಿಂತಿರುಗಿ -1;
	*ಮಾರ್ಗ = p + 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಎಣಿಕೆ parse_opt_ಫಲಿತಾಂಶ cacheinfo_callback(
	ರಚನೆ parse_opt_ctx_t *ctx, ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ object_id oid;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಆದರೆ (!parse_new_style_cacheinfo(ctx->argv[1], &ಬಹುಲಕ, &oid, &ಮಾರ್ಗ)) {
		ಆದರೆ (ಕೂಡಿಸು_cacheinfo(ಬಹುಲಕ, &oid, ಮಾರ್ಗ, 0))
			die("git update-index: --cacheinfo cannot add %s", ಮಾರ್ಗ);
		ctx->argv++;
		ctx->argc--;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (ctx->argc <= 3)
		ಹಿಂತಿರುಗಿ error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	ಆದರೆ (strtoul_ui(*++ctx->argv, 8, &ಬಹುಲಕ) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    ಕೂಡಿಸು_cacheinfo(ಬಹುಲಕ, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಎಣಿಕೆ parse_opt_ಫಲಿತಾಂಶ stdin_cacheinfo_callback(
	ರಚನೆ parse_opt_ctx_t *ctx, ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *nul_ಪದ_ಸಾಲು = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಆದರೆ (ctx->argc != 1)
		ಹಿಂತಿರುಗಿ error("option '%s' must be the last argument", opt->long_name);
	allow_ಕೂಡಿಸು = allow_replace = allow_remove = 1;
	read_ಸೂಚ್ಯಂಕ_info(*nul_ಪದ_ಸಾಲು);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಎಣಿಕೆ parse_opt_ಫಲಿತಾಂಶ stdin_callback(
	ರಚನೆ parse_opt_ctx_t *ctx, ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *read_from_stdin = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಆದರೆ (ctx->argc != 1)
		ಹಿಂತಿರುಗಿ error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಎಣಿಕೆ parse_opt_ಫಲಿತಾಂಶ unresolve_callback(
	ರಚನೆ parse_opt_ctx_t *ctx, ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *has_errors = opt->ಮೌಲ್ಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = the_repository->ಪೂರ್ವಪ್ರತ್ಯಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ) : 0);
	ಆದರೆ (*has_errors)
		the_repository->ಸೂಚ್ಯಂಕ->cache_ಬದಲಾಯಿತು = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಎಣಿಕೆ parse_opt_ಫಲಿತಾಂಶ reupdate_callback(
	ರಚನೆ parse_opt_ctx_t *ctx, ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *has_errors = opt->ಮೌಲ್ಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = the_repository->ಪೂರ್ವಪ್ರತ್ಯಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, ಪೂರ್ವಪ್ರತ್ಯಯ);
	ಆದರೆ (*has_errors)
		the_repository->ಸೂಚ್ಯಂಕ->cache_ಬದಲಾಯಿತು = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_update_ಸೂಚ್ಯಂಕ(ಪೂರ್ಣಾಂಕ argc,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ newfd, entries, has_errors = 0, nul_ಪದ_ಸಾಲು = 0;
	ಎಣಿಕೆ uc_ಬಹುಲಕ untracked_cache = UC_UNSPECIFIED;
	ಪೂರ್ಣಾಂಕ read_from_stdin = 0;
	ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ = ಪೂರ್ವಪ್ರತ್ಯಯ ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ) : 0;
	ಪೂರ್ಣಾಂಕ preferred_ಸೂಚ್ಯಂಕ_format = 0;
	ಅಕ್ಷರ ಗಣ_executable_ಬಿಟ್ = 0;
	ರಚನೆ refresh_params refresh_ವಾದಗಳು = {0, &has_errors};
	ಪೂರ್ಣಾಂಕ ಬೀಗ_error = 0;
	ಪೂರ್ಣಾಂಕ ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ = -1;
	ಪೂರ್ಣಾಂಕ force_write = 0;
	ಪೂರ್ಣಾಂಕ fsmonitor = -1;
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ರಚನೆ parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	ಪೂರ್ಣಾಂಕ parseopt_state = PARSE_OPT_UNKNOWN;
	ರಚನೆ repository *r = the_repository;
	ರಚನೆ odb_transaction *transaction;
	ರಚನೆ option options[] = {
		OPT_ಬಿಟ್('q', ಖಾಲಿ, &refresh_ವಾದಗಳು.flags,
			N_("continue refresh even when index needs update"),
			REFRESH_QUIET),
		OPT_ಬಿಟ್(0, "ignore-submodules", &refresh_ವಾದಗಳು.flags,
			N_("refresh: ignore submodules"),
			REFRESH_IGNORE_SUBMODULES),
		OPT_ಗಣ_INT(0, "add", &allow_ಕೂಡಿಸು,
			N_("do not ignore new files"), 1),
		OPT_ಗಣ_INT(0, "replace", &allow_replace,
			N_("let files replace directories and vice-versa"), 1),
		OPT_ಗಣ_INT(0, "remove", &allow_remove,
			N_("notice files missing from worktree"), 1),
		OPT_ಬಿಟ್(0, "unmerged", &refresh_ವಾದಗಳು.flags,
			N_("refresh even if index contains unmerged entries"),
			REFRESH_UNMERGED),
		OPT_CALLBACK_F(0, "refresh", &refresh_ವಾದಗಳು, ಖಾಲಿ,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_ವಾದಗಳು, ಖಾಲಿ,
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
		OPT_CALLBACK_F(0, "chmod", &ಗಣ_executable_ಬಿಟ್, "(+|-)x",
			N_("override the executable bit of the listed files"),
			PARSE_OPT_NONEG,
			chmod_callback),
		{
			.type = OPTION_ಗಣ_INT,
			.long_name = "assume-unchanged",
			.ಮೌಲ್ಯ = &mark_valid_only,
			.precision = ಗಾತ್ರ(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_ಗಣ_INT,
			.long_name = "no-assume-unchanged",
			.ಮೌಲ್ಯ = &mark_valid_only,
			.precision = ಗಾತ್ರ(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_ಗಣ_INT,
			.long_name = "skip-worktree",
			.ಮೌಲ್ಯ = &mark_skip_worktree_only,
			.precision = ಗಾತ್ರ(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_ಗಣ_INT,
			.long_name = "no-skip-worktree",
			.ಮೌಲ್ಯ = &mark_skip_worktree_only,
			.precision = ಗಾತ್ರ(mark_skip_worktree_only),
			.help = N_("clear skip-worktree bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_BOOL(0, "ignore-skip-worktree-entries", &ignore_skip_worktree_entries,
			 N_("do not touch index-only entries")),
		OPT_ಗಣ_INT(0, "info-only", &info_only,
			N_("add to index only; do not add content to object database"), 1),
		OPT_ಗಣ_INT(0, "force-remove", &force_remove,
			N_("remove named paths even if present in worktree"), 1),
		OPT_BOOL('z', ಖಾಲಿ, &nul_ಪದ_ಸಾಲು,
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
			.ಮೌಲ್ಯ = &nul_ಪದ_ಸಾಲು,
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
		OPT_ಬಿಟ್(0, "ignore-missing", &refresh_ವಾದಗಳು.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_ಗಣ_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", ಖಾಲಿ, ಖಾಲಿ,
			N_("(for porcelains) forget saved unresolved conflicts"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			resolve_undo_clear_callback),
		OPT_INTEGER(0, "index-version", &preferred_ಸೂಚ್ಯಂಕ_format,
			N_("write index in this format")),
		OPT_ಗಣ_INT(0, "show-index-version", &preferred_ಸೂಚ್ಯಂಕ_format,
			    N_("report on-disk index format version"), -1),
		OPT_BOOL(0, "split-index", &ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ,
			N_("enable or disable split index")),
		OPT_BOOL(0, "untracked-cache", &untracked_cache,
			N_("enable/disable untracked cache")),
		OPT_ಗಣ_INT(0, "test-untracked-cache", &untracked_cache,
			    N_("test if the filesystem supports untracked cache"), UC_TEST),
		OPT_ಗಣ_INT(0, "force-untracked-cache", &untracked_cache,
			    N_("enable untracked cache without testing the filesystem"), UC_FORCE),
		OPT_ಗಣ_INT(0, "force-write-index", &force_write,
			N_("write out the index even if is not flagged as changed"), 1),
		OPT_BOOL(0, "fsmonitor", &fsmonitor,
			N_("enable or disable file system monitor")),
		{
			.type = OPTION_ಗಣ_INT,
			.long_name = "fsmonitor-valid",
			.ಮೌಲ್ಯ = &mark_fsmonitor_only,
			.precision = ಗಾತ್ರ(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_ಗಣ_INT,
			.long_name = "no-fsmonitor-valid",
			.ಮೌಲ್ಯ = &mark_fsmonitor_only,
			.precision = ಗಾತ್ರ(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_ಕೊನೆ()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_ಸೂಚ್ಯಂಕ_usage, options);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, 0);
	ಆದರೆ (newfd < 0)
		ಬೀಗ_error = errno;

	entries = repo_read_ಸೂಚ್ಯಂಕ(the_repository);
	ಆದರೆ (entries < 0)
		die("cache corrupted");

	the_repository->ಸೂಚ್ಯಂಕ->updated_skipworktree = 1;

	/*
	 * Custom copy of parse_options() because we want to handle
	 * filename arguments as they come.
	 */
	parse_options_ಆರಂಭ(&ctx, argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			    options, PARSE_OPT_STOP_AT_NON_OPTION);

	/*
	 * Allow the object layer to optimize adding multiple objects in
	 * a batch.
	 */
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	ಆಗ (ctx.argc) {
		ಆದರೆ (parseopt_state != PARSE_OPT_ಮುಗಿದ)
			parseopt_state = parse_options_ಹೆಜ್ಜೆ(&ctx, options,
							    update_ಸೂಚ್ಯಂಕ_usage);
		ಆದರೆ (!ctx.argc)
			ನಿಲ್ಲಿಸು;
		ಆಯ್ಕೆ (parseopt_state) {
		ಸ್ಥಿತಿ PARSE_OPT_HELP:
			ನಿರ್ಗಮಿಸು(0);
		ಸ್ಥಿತಿ PARSE_OPT_HELP_ERROR:
		ಸ್ಥಿತಿ PARSE_OPT_ERROR:
			ನಿರ್ಗಮಿಸು(129);
		ಸ್ಥಿತಿ PARSE_OPT_COMPLETE:
			ನಿರ್ಗಮಿಸು(0);
		ಸ್ಥಿತಿ PARSE_OPT_NON_OPTION:
		ಸ್ಥಿತಿ PARSE_OPT_ಮುಗಿದ:
		{
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = ctx.argv[0];
			ಅಕ್ಷರ *p;

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
			ಆದರೆ (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = ಖಾಲಿ;
			}

			ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
			p = ಪೂರ್ವಪ್ರತ್ಯಯ_ಮಾರ್ಗ(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ, ಮಾರ್ಗ);
			update_one(p);
			ಆದರೆ (ಗಣ_executable_ಬಿಟ್)
				chmod_ಮಾರ್ಗ(ಗಣ_executable_ಬಿಟ್, p);
			ಬಿಡುಗಡೆ(p);
			ctx.argc--;
			ctx.argv++;
			ನಿಲ್ಲಿಸು;
		}
		ಸ್ಥಿತಿ PARSE_OPT_UNKNOWN:
			ಆದರೆ (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			ಇಲ್ಲದಿದ್ದರೆ
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_ಸೂಚ್ಯಂಕ_usage, options);
		}
	}
	argc = parse_options_ಕೊನೆ(&ctx);

	getline_fn = nul_ಪದ_ಸಾಲು ? strbuf_getline_nul : strbuf_getline_lf;
	ಆದರೆ (preferred_ಸೂಚ್ಯಂಕ_format) {
		ಆದರೆ (preferred_ಸೂಚ್ಯಂಕ_format < 0) {
			ಬರೆ(_("%d\n"), the_repository->ಸೂಚ್ಯಂಕ->ಆವೃತ್ತಿ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (preferred_ಸೂಚ್ಯಂಕ_format < ಸೂಚ್ಯಂಕ_FORMAT_LB ||
			   ಸೂಚ್ಯಂಕ_FORMAT_UB < preferred_ಸೂಚ್ಯಂಕ_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_ಸೂಚ್ಯಂಕ_format,
			    ಸೂಚ್ಯಂಕ_FORMAT_LB, ಸೂಚ್ಯಂಕ_FORMAT_UB);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (the_repository->ಸೂಚ್ಯಂಕ->ಆವೃತ್ತಿ != preferred_ಸೂಚ್ಯಂಕ_format)
				the_repository->ಸೂಚ್ಯಂಕ->cache_ಬದಲಾಯಿತು |= SOMETHING_ಬದಲಾಯಿತು;
			report(_("index-version: was %d, set to %d"),
			       the_repository->ಸೂಚ್ಯಂಕ->ಆವೃತ್ತಿ, preferred_ಸೂಚ್ಯಂಕ_format);
			the_repository->ಸೂಚ್ಯಂಕ->ಆವೃತ್ತಿ = preferred_ಸೂಚ್ಯಂಕ_format;
		}
	}

	ಆದರೆ (read_from_stdin) {
		ರಚನೆ strbuf buf = STRBUF_INIT;
		ರಚನೆ strbuf unquoted = STRBUF_INIT;

		ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
		ಆಗ (getline_fn(&buf, stdin) != EOF) {
			ಅಕ್ಷರ *p;
			ಆದರೆ (!nul_ಪದ_ಸಾಲು && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ಆದರೆ (unquote_c_style(&unquoted, buf.buf, ಖಾಲಿ))
					die("line is badly quoted");
				strbuf_ಅದಲುಬದಲು(&buf, &unquoted);
			}
			p = ಪೂರ್ವಪ್ರತ್ಯಯ_ಮಾರ್ಗ(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ, buf.buf);
			update_one(p);
			ಆದರೆ (ಗಣ_executable_ಬಿಟ್)
				chmod_ಮಾರ್ಗ(ಗಣ_executable_ಬಿಟ್, p);
			ಬಿಡುಗಡೆ(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	ಆದರೆ (ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ > 0) {
		ಆದರೆ (repo_config_get_ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		ಆದರೆ (the_repository->ಸೂಚ್ಯಂಕ->ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ)
			the_repository->ಸೂಚ್ಯಂಕ->cache_ಬದಲಾಯಿತು |= ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ_ORDERED;
		ಇಲ್ಲದಿದ್ದರೆ
			ಕೂಡಿಸು_ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ) {
		ಆದರೆ (repo_config_get_ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	}

	prepare_repo_settings(r);
	ಆಯ್ಕೆ (untracked_cache) {
	ಸ್ಥಿತಿ UC_UNSPECIFIED:
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ UC_DISABLE:
		ಆದರೆ (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->ಸೂಚ್ಯಂಕ);
		report(_("Untracked cache disabled"));
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ UC_TEST:
		ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
		ಹಿಂತಿರುಗಿ !test_if_untracked_cache_is_supported();
	ಸ್ಥಿತಿ UC_ENABLE:
	ಸ್ಥಿತಿ UC_FORCE:
		ಆದರೆ (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		ಕೂಡಿಸು_untracked_cache(the_repository->ಸೂಚ್ಯಂಕ);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_ವೃಕ್ಷ(the_repository));
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	ಆದರೆ (fsmonitor > 0) {
		ಎಣಿಕೆ fsmonitor_ಬಹುಲಕ fsm_ಬಹುಲಕ = fsm_settings_get_ಬಹುಲಕ(r);
		ಎಣಿಕೆ fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		ಆದರೆ (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_ಸಂದೇಶ(r, reason));

		ಆದರೆ (fsm_ಬಹುಲಕ == FSMONITOR_ಬಹುಲಕ_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		ಕೂಡಿಸು_fsmonitor(the_repository->ಸೂಚ್ಯಂಕ);
		report(_("fsmonitor enabled"));
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!fsmonitor) {
		ಎಣಿಕೆ fsmonitor_ಬಹುಲಕ fsm_ಬಹುಲಕ = fsm_settings_get_ಬಹುಲಕ(r);
		ಆದರೆ (fsm_ಬಹುಲಕ > FSMONITOR_ಬಹುಲಕ_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->ಸೂಚ್ಯಂಕ);
		report(_("fsmonitor disabled"));
	}

	ಆದರೆ (the_repository->ಸೂಚ್ಯಂಕ->cache_ಬದಲಾಯಿತು || force_write) {
		ಆದರೆ (newfd < 0) {
			ಆದರೆ (refresh_ವಾದಗಳು.flags & REFRESH_QUIET)
				ನಿರ್ಗಮಿಸು(128);
			unable_ಗಮ್ಯ_ಬೀಗ_die(repo_get_ಸೂಚ್ಯಂಕ_file(the_repository), ಬೀಗ_error);
		}
		ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file, COMMIT_ಬೀಗ))
			die("Unable to write new index file");
	}

	rollback_ಬೀಗ_file(&ಬೀಗ_file);

	ಹಿಂತಿರುಗಿ has_errors ? 1 : 0;
}
