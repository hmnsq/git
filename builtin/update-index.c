/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "tree-walk.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/transaction.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "resolve-undo.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "pathspec.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "read-cache.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "split-index.h"
#ଅନ୍ତର୍ଭାଵ "symlinks.h"
#ଅନ୍ତର୍ଭାଵ "fsmonitor.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_add;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_remove;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_replace;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା info_only;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_remove;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_valid_only;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_skip_worktree_only;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_fsmonitor_only;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_skip_worktree_entries;
#define MARK_FLAG 1
#define UNMARK_FLAG 2
ସ୍ଥିର ସଂରଚନା strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
ଗଣନା uc_mode {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (printf, 1, 2)))
ସ୍ଥିର ଶୂନ୍ୟ report(ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
	va_list vp;

	ଯଦି (!verbose)
		ଫେରନ୍ତୁ;

	va_start(vp, fmt);
	vprintf(fmt, vp);
	putchar('\n');
	va_end(vp);
}

ସ୍ଥିର ଶୂନ୍ୟ remove_test_directory(ଶୂନ୍ୟ)
{
	ଯଦି (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_mtime_path(ଅଚଳ ଅକ୍ଷର *path)
{
	ସ୍ଥିର ସଂରଚନା strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, path);
	ଫେରନ୍ତୁ sb.buf;
}

ସ୍ଥିର ଶୂନ୍ୟ xmkdir(ଅଚଳ ଅକ୍ଷର *path)
{
	path = get_mtime_path(path);
	ଯଦି (mkdir(path, 0700))
		die_errno(_("failed to create directory %s"), path);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା xstat_mtime_dir(ସଂରଚନା stat *st)
{
	ଯଦି (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା create_file(ଅଚଳ ଅକ୍ଷର *path)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	path = get_mtime_path(path);
	fd = xopen(path, O_CREAT | O_RDWR, 0644);
	ଫେରନ୍ତୁ fd;
}

ସ୍ଥିର ଶୂନ୍ୟ xunlink(ଅଚଳ ଅକ୍ଷର *path)
{
	path = get_mtime_path(path);
	ଯଦି (unlink(path))
		die_errno(_("failed to delete file %s"), path);
}

ସ୍ଥିର ଶୂନ୍ୟ xrmdir(ଅଚଳ ଅକ୍ଷର *path)
{
	path = get_mtime_path(path);
	ଯଦି (rmdir(path))
		die_errno(_("failed to delete directory %s"), path);
}

ସ୍ଥିର ଶୂନ୍ୟ avoid_racy(ଶୂନ୍ୟ)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_if_untracked_cache_is_supported(ଶୂନ୍ୟ)
{
	ସଂରଚନା stat st;
	ସଂରଚନା stat_data base;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ret = 0;
	ଅକ୍ଷର *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	ଯଦି (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	ମୁକ୍ତ(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	ଯଦି (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		ଯାଅ done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	ଯଦି (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		ଯାଅ done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	ଯଦି (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		ଯାଅ done;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	ଯଦି (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		ଯାଅ done;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	ଯଦି (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		ଯାଅ done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	ଯଦି (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		ଯାଅ done;
	}

	ଯଦି (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

done:
	strbuf_release(&mtime_dir);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_ce_flags(ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା namelen = strlen(path);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = index_name_pos(the_repository->index, path, namelen);
	ଯଦି (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->index, the_repository->index->cache[pos]);
		ଯଦି (mark)
			the_repository->index->cache[pos]->ce_flags |= flag;
		ଅନ୍ୟଥା
			the_repository->index->cache[pos]->ce_flags &= ~flag;
		the_repository->index->cache[pos]->ce_flags |= CE_UPDATE_IN_BASE;
		cache_ବୃକ୍ଷ_invalidate_path(the_repository->index, path);
		the_repository->index->cache_changed |= CE_ENTRY_CHANGED;
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା remove_one_path(ଅଚଳ ଅକ୍ଷର *path)
{
	ଯଦି (!allow_remove)
		ଫେରନ୍ତୁ error("%s: does not exist and --remove not passed", path);
	ଯଦି (remove_file_from_index(the_repository->index, path))
		ଫେରନ୍ତୁ error("%s: cannot remove from the index", path);
	ଫେରନ୍ତୁ 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା process_lstat_error(ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା err)
{
	ଯଦି (is_missing_file_error(err))
		ଫେରନ୍ତୁ remove_one_path(path);
	ଫେରନ୍ତୁ error("lstat(\"%s\"): %s", path, strerror(err));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_one_path(ଅଚଳ ସଂରଚନା cache_entry *old, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ସଂରଚନା stat *st)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା option;
	ସଂରଚନା cache_entry *ce;

	/* Was the old index entry already up-to-date? */
	ଯଦି (old && !ce_stage(old) && !ie_match_stat(the_repository->index, old, st, 0))
		ଫେରନ୍ତୁ 0;

	ce = make_empty_cache_entry(the_repository->index, len);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->index, ce, st);
	ce->ce_mode = ce_mode_from_stat(the_repository, old, st->st_mode);

	ଯଦି (index_path(the_repository->index, &ce->oid, path, st,
		       info_only ? 0 : INDEX_WRITE_OBJECT)) {
		discard_cache_entry(ce);
		ଫେରନ୍ତୁ -1;
	}
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	ଯଦି (add_index_entry(the_repository->index, ce, option)) {
		discard_cache_entry(ce);
		ଫେରନ୍ତୁ error("%s: cannot add to the index - missing --add option?", path);
	}
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା process_directory(ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ସଂରଚନା stat *st)
{
	ସଂରଚନା object_id oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = index_name_pos(the_repository->index, path, len);

	/* Exact match: file or existing gitlink */
	ଯଦି (pos >= 0) {
		ଅଚଳ ସଂରଚନା cache_entry *ce = the_repository->index->cache[pos];
		ଯଦି (S_ISGITLINK(ce->ce_mode)) {

			/* Do nothing to the index if there is no HEAD! */
			ଯଦି (repo_resolve_gitlink_ref(the_repository, path,
						     "HEAD", &oid) < 0)
				ଫେରନ୍ତୁ 0;

			ଫେରନ୍ତୁ add_one_path(ce, path, len, st);
		}
		/* Should this be an unconditional error? */
		ଫେରନ୍ତୁ remove_one_path(path);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	ଯେତେବେଳେ (pos < the_repository->index->cache_nr) {
		ଅଚଳ ସଂରଚନା cache_entry *ce = the_repository->index->cache[pos++];

		ଯଦି (strncmp(ce->name, path, len))
			ଭାଙ୍ଗ;
		ଯଦି (ce->name[len] > '/')
			ଭାଙ୍ଗ;
		ଯଦି (ce->name[len] < '/')
			ଜାରି;

		/* Subdirectory match - error out */
		ଫେରନ୍ତୁ error("%s: is a directory - add individual files instead", path);
	}

	/* No match - should we add it as a gitlink? */
	ଯଦି (!repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid))
		ଫେରନ୍ତୁ add_one_path(ଖାଲି, path, len, st);

	/* Error out. */
	ଫେରନ୍ତୁ error("%s: is a directory - add files inside instead", path);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା process_path(ଅଚଳ ଅକ୍ଷର *path, ସଂରଚନା stat *st, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stat_errno)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos, len;
	ଅଚଳ ସଂରଚନା cache_entry *ce;

	len = strlen(path);
	ଯଦି (has_symlink_leading_path(path, len))
		ଫେରନ୍ତୁ error("'%s' is beyond a symbolic link", path);

	pos = index_name_pos(the_repository->index, path, len);
	ce = pos < 0 ? ଖାଲି : the_repository->index->cache[pos];
	ଯଦି (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		ଯଦି (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_index(the_repository->index, path))
			ଫେରନ୍ତୁ error("%s: cannot remove from the index", path);
		ଫେରନ୍ତୁ 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	ଯଦି (stat_errno)
		ଫେରନ୍ତୁ process_lstat_error(path, stat_errno);

	ଯଦି (S_ISDIR(st->st_mode))
		ଫେରନ୍ତୁ process_directory(path, len, st);

	ଫେରନ୍ତୁ add_one_path(ce, path, len, st);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_cacheinfo(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, ଅଚଳ ସଂରଚନା object_id *oid,
			 ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stage)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, option;
	ସଂରଚନା cache_entry *ce;

	ଯଦି (!verify_path(path, mode))
		ଫେରନ୍ତୁ error("Invalid path '%s'", path);

	len = strlen(path);
	ce = make_empty_cache_entry(the_repository->index, len);

	oidcpy(&ce->oid, oid);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_mode = create_ce_mode(mode);
	ଯଦି (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	ଯଦି (add_index_entry(the_repository->index, ce, option))
		ଫେରନ୍ତୁ error("%s: cannot add to the index - missing --add option?",
			     path);
	report("add '%s'", path);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ chmod_path(ଅକ୍ଷର flip, ଅଚଳ ଅକ୍ଷର *path)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
	ସଂରଚନା cache_entry *ce;

	pos = index_name_pos(the_repository->index, path, strlen(path));
	ଯଦି (pos < 0)
		ଯାଅ fail;
	ce = the_repository->index->cache[pos];
	ଯଦି (chmod_index_entry(the_repository->index, ce, flip) < 0)
		ଯାଅ fail;

	report("chmod %cx '%s'", flip, path);
	ଫେରନ୍ତୁ;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, path);
}

ସ୍ଥିର ଶୂନ୍ୟ update_one(ଅଚଳ ଅକ୍ଷର *path)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା stat_errno = 0;
	ସଂରଚନା stat st;

	ଯଦି (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_mode = 0;
	ଅନ୍ୟଥା ଯଦି (lstat(path, &st) < 0) {
		st.st_mode = 0;
		stat_errno = errno;
	} /* else stat is valid */

	ଯଦି (!verify_path(path, st.st_mode)) {
		fprintf(stderr, "Ignoring path %s\n", path);
		ଫେରନ୍ତୁ;
	}
	ଯଦି (mark_valid_only) {
		ଯଦି (mark_ce_flags(path, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		ଫେରନ୍ତୁ;
	}
	ଯଦି (mark_skip_worktree_only) {
		ଯଦି (mark_ce_flags(path, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		ଫେରନ୍ତୁ;
	}
	ଯଦି (mark_fsmonitor_only) {
		ଯଦି (mark_ce_flags(path, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (force_remove) {
		ଯଦି (remove_file_from_index(the_repository->index, path))
			die("git update-index: unable to remove %s", path);
		report("remove '%s'", path);
		ଫେରନ୍ତୁ;
	}
	ଯଦି (process_path(path, &st, stat_errno))
		die("Unable to process path %s", path);
	report("add '%s'", path);
}

ସ୍ଥିର ଶୂନ୍ୟ read_index_info(ପୂର୍ଣ୍ଣସଂଖ୍ୟା nul_term_ଧାଡ଼ି)
{
	ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା hexsz = the_hash_algo->hexsz;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_ଧାଡ଼ି ? strbuf_getline_nul : strbuf_getline_lf;
	ଯେତେବେଳେ (getline_fn(&buf, stdin) != EOF) {
		ଅକ୍ଷର *ptr, *tab;
		ଅକ୍ଷର *path_name;
		ସଂରଚନା object_id oid;
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;
		unsigned long ul;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା stage;

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
		ଯଦି (ptr == buf.buf || *ptr != ' '
		    || errno || (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ul != ul)
			ଯାଅ bad_ଧାଡ଼ି;
		mode = ul;

		tab = strchr(ptr, '\t');
		ଯଦି (!tab || tab - ptr < hexsz + 1)
			ଯାଅ bad_ଧାଡ଼ି;

		ଯଦି (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		ଅନ୍ୟଥା {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		ଯଦି (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			ଯାଅ bad_ଧାଡ଼ି;

		path_name = ptr;
		ଯଦି (!nul_term_ଧାଡ଼ି && path_name[0] == '"') {
			strbuf_reset(&uq);
			ଯଦି (unquote_c_style(&uq, path_name, ଖାଲି)) {
				die("git update-index: bad quoting of path name");
			}
			path_name = uq.buf;
		}

		ଯଦି (!verify_path(path_name, mode)) {
			fprintf(stderr, "Ignoring path %s\n", path_name);
			ଜାରି;
		}

		ଯଦି (!mode) {
			/* mode == 0 means there is no such path -- remove */
			ଯଦି (remove_file_from_index(the_repository->index, path_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		ଅନ୍ୟଥା {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			ଯଦି (add_cacheinfo(mode, &oid, path_name, stage))
				die("git update-index: unable to update %s",
				    path_name);
		}
		ଜାରି;

	bad_ଧାଡ଼ି:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ update_index_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	ଖାଲି
};

ସ୍ଥିର ସଂରଚନା cache_entry *read_one_ent(ଅଚଳ ଅକ୍ଷର *which,
					ସଂରଚନା object_id *ent, ଅଚଳ ଅକ୍ଷର *path,
					ପୂର୍ଣ୍ଣସଂଖ୍ୟା namelen, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stage)
{
	unsigned short mode;
	ସଂରଚନା object_id oid;
	ସଂରଚନା cache_entry *ce;

	ଯଦି (get_ବୃକ୍ଷ_entry(the_repository, ent, path, &oid, &mode)) {
		ଯଦି (which)
			error("%s: not in %s branch.", path, which);
		ଫେରନ୍ତୁ ଖାଲି;
	}
	ଯଦି (!the_repository->index->sparse_index && mode == S_IFDIR) {
		ଯଦି (which)
			error("%s: not a blob in %s branch.", path, which);
		ଫେରନ୍ତୁ ଖାଲି;
	}
	ce = make_empty_cache_entry(the_repository->index, namelen);

	oidcpy(&ce->oid, &oid);
	memcpy(ce->name, path, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_mode = create_ce_mode(mode);
	ଫେରନ୍ତୁ ce;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା unresolve_one(ଅଚଳ ଅକ୍ଷର *path)
{
	ସଂରଚନା string_list_item *item;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;

	ଯଦି (!the_repository->index->resolve_undo)
		ଫେରନ୍ତୁ res;
	item = string_list_lookup(the_repository->index->resolve_undo, path);
	ଯଦି (!item)
		ଫେରନ୍ତୁ res; /* no resolve-undo record for the path */
	res = unmerge_index_entry(the_repository->index, path, item->util, 0);
	FREE_AND_NULL(item->util);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_unresolve(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac, ଅଚଳ ଅକ୍ଷର **av,
			ଅଚଳ ଅକ୍ଷର *prefix, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefix_length)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;

	ପାଇଁ (i = 1; i < ac; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = av[i];
		ଅକ୍ଷର *p = prefix_path(the_repository, prefix, prefix_length, arg);
		err |= unresolve_one(p);
		ମୁକ୍ତ(p);
	}
	ଫେରନ୍ତୁ err;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_reupdate(ଅଚଳ ଅକ୍ଷର **paths,
		       ଅଚଳ ଅକ୍ଷର *prefix)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_head = 1;
	ସଂରଚନା pathspec pathspec;
	ସଂରଚନା object_id head_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, paths);

	ଯଦି (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &head_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_head = 0;
 redo:
	ପାଇଁ (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ଅଚଳ ସଂରଚନା cache_entry *ce = the_repository->index->cache[pos];
		ସଂରଚନା cache_entry *old = ଖାଲି;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା save_nr;
		ଅକ୍ଷର *path;

		ଯଦି (ce_stage(ce) || !ce_path_match(the_repository->index, ce, &pathspec, ଖାଲି))
			ଜାରି;
		ଯଦି (has_head)
			old = read_one_ent(ଖାଲି, &head_oid,
					   ce->name, ce_namelen(ce), 0);
		ଯଦି (old && ce->ce_mode == old->ce_mode &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_entry(old);
			ଜାରି; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		ଯଦି (S_ISSPARSEDIR(ce->ce_mode)) {
			ensure_full_index(the_repository->index);
			ଯାଅ redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->index->cache_nr;
		path = xstrdup(ce->name);
		update_one(path);
		ମୁକ୍ତ(path);
		discard_cache_entry(old);
		ଯଦି (save_nr != the_repository->index->cache_nr)
			ଯାଅ redo;
	}
	clear_pathspec(&pathspec);
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା refresh_params {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *has_errors;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା refresh(ସଂରଚନା refresh_params *o, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag)
{
	setup_work_ବୃକ୍ଷ(the_repository);
	repo_read_index(the_repository);
	*o->has_errors |= refresh_index(the_repository->index, o->flags | flag, ଖାଲି,
					ଖାଲି, ଖାଲି);
	ଯଦି (has_racy_timestamp(the_repository->index)) {
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
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା refresh_callback(ଅଚଳ ସଂରଚନା option *opt,
				ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ଫେରନ୍ତୁ refresh(opt->ମୂଲ୍ୟ, 0);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା really_refresh_callback(ଅଚଳ ସଂରଚନା option *opt,
				ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ଫେରନ୍ତୁ refresh(opt->ମୂଲ୍ୟ, REFRESH_REALLY);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା chmod_callback(ଅଚଳ ସଂରଚନା option *opt,
				ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଅକ୍ଷର *flip = opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	ଯଦି ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		ଫେରନ୍ତୁ error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା resolve_undo_clear_callback(ଅଚଳ ସଂରଚନା option *opt UNUSED,
				ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_index(the_repository->index);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_new_style_cacheinfo(ଅଚଳ ଅକ୍ଷର *arg,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mode,
				     ସଂରଚନା object_id *oid,
				     ଅଚଳ ଅକ୍ଷର **path)
{
	unsigned long ul;
	ଅକ୍ଷର *endp;
	ଅଚଳ ଅକ୍ଷର *p;

	ଯଦି (!arg)
		ଫେରନ୍ତୁ -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	ଯଦି (errno || endp == arg || *endp != ',' || (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ul != ul)
		ଫେରନ୍ତୁ -1; /* not a new-style cacheinfo */
	*mode = ul;
	endp++;
	ଯଦି (parse_oid_hex(endp, oid, &p) || *p != ',')
		ଫେରନ୍ତୁ -1;
	*path = p + 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଗଣନା parse_opt_result cacheinfo_callback(
	ସଂରଚନା parse_opt_ctx_t *ctx, ଅଚଳ ସଂରଚନା option *opt UNUSED,
	ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା object_id oid;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;
	ଅଚଳ ଅକ୍ଷର *path;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ଯଦି (!parse_new_style_cacheinfo(ctx->argv[1], &mode, &oid, &path)) {
		ଯଦି (add_cacheinfo(mode, &oid, path, 0))
			die("git update-index: --cacheinfo cannot add %s", path);
		ctx->argv++;
		ctx->argc--;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (ctx->argc <= 3)
		ଫେରନ୍ତୁ error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	ଯଦି (strtoul_ui(*++ctx->argv, 8, &mode) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    add_cacheinfo(mode, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଗଣନା parse_opt_result stdin_cacheinfo_callback(
	ସଂରଚନା parse_opt_ctx_t *ctx, ଅଚଳ ସଂରଚନା option *opt,
	ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *nul_term_ଧାଡ଼ି = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ଯଦି (ctx->argc != 1)
		ଫେରନ୍ତୁ error("option '%s' must be the last argument", opt->long_name);
	allow_add = allow_replace = allow_remove = 1;
	read_index_info(*nul_term_ଧାଡ଼ି);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଗଣନା parse_opt_result stdin_callback(
	ସଂରଚନା parse_opt_ctx_t *ctx, ଅଚଳ ସଂରଚନା option *opt,
	ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *read_from_stdin = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ଯଦି (ctx->argc != 1)
		ଫେରନ୍ତୁ error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଗଣନା parse_opt_result unresolve_callback(
	ସଂରଚନା parse_opt_ctx_t *ctx, ଅଚଳ ସଂରଚନା option *opt,
	ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *has_errors = opt->ମୂଲ୍ୟ;
	ଅଚଳ ଅକ୍ଷର *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				prefix, prefix ? strlen(prefix) : 0);
	ଯଦି (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଗଣନା parse_opt_result reupdate_callback(
	ସଂରଚନା parse_opt_ctx_t *ctx, ଅଚଳ ସଂରଚନା option *opt,
	ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *has_errors = opt->ମୂଲ୍ୟ;
	ଅଚଳ ଅକ୍ଷର *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	setup_work_ବୃକ୍ଷ(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, prefix);
	ଯଦି (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_update_index(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		     ଅଚଳ ଅକ୍ଷର **argv,
		     ଅଚଳ ଅକ୍ଷର *prefix,
		     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା newfd, entries, has_errors = 0, nul_term_ଧାଡ଼ି = 0;
	ଗଣନା uc_mode untracked_cache = UC_UNSPECIFIED;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_from_stdin = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefix_length = prefix ? strlen(prefix) : 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା preferred_index_format = 0;
	ଅକ୍ଷର set_executable_bit = 0;
	ସଂରଚନା refresh_params refresh_args = {0, &has_errors};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା lock_error = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_index = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_write = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsmonitor = -1;
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ସଂରଚନା parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା parseopt_state = PARSE_OPT_UNKNOWN;
	ସଂରଚନା repository *r = the_repository;
	ସଂରଚନା odb_transaction *transaction;
	ସଂରଚନା option options[] = {
		OPT_BIT('q', ଖାଲି, &refresh_args.flags,
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
		OPT_CALLBACK_F(0, "refresh", &refresh_args, ଖାଲି,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_args, ଖାଲି,
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
			.ମୂଲ୍ୟ = &mark_valid_only,
			.precision = ଆକାର(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-assume-unchanged",
			.ମୂଲ୍ୟ = &mark_valid_only,
			.precision = ଆକାର(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "skip-worktree",
			.ମୂଲ୍ୟ = &mark_skip_worktree_only,
			.precision = ଆକାର(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-skip-worktree",
			.ମୂଲ୍ୟ = &mark_skip_worktree_only,
			.precision = ଆକାର(mark_skip_worktree_only),
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
		OPT_BOOL('z', ଖାଲି, &nul_term_ଧାଡ଼ି,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.ମୂଲ୍ୟ = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.ମୂଲ୍ୟ = &nul_term_ଧାଡ଼ି,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.ମୂଲ୍ୟ = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.ମୂଲ୍ୟ = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_BIT(0, "ignore-missing", &refresh_args.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_SET_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", ଖାଲି, ଖାଲି,
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
			.ମୂଲ୍ୟ = &mark_fsmonitor_only,
			.precision = ଆକାର(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-fsmonitor-valid",
			.ମୂଲ୍ୟ = &mark_fsmonitor_only,
			.precision = ଆକାର(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_index_usage, options);

	repo_config(the_repository, git_default_config, ଖାଲି);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_full_index = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_index(the_repository, &lock_file, 0);
	ଯଦି (newfd < 0)
		lock_error = errno;

	entries = repo_read_index(the_repository);
	ଯଦି (entries < 0)
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
	ଯେତେବେଳେ (ctx.argc) {
		ଯଦି (parseopt_state != PARSE_OPT_DONE)
			parseopt_state = parse_options_step(&ctx, options,
							    update_index_usage);
		ଯଦି (!ctx.argc)
			ଭାଙ୍ଗ;
		ବାଛ (parseopt_state) {
		ସ୍ଥିତି PARSE_OPT_HELP:
			exit(0);
		ସ୍ଥିତି PARSE_OPT_HELP_ERROR:
		ସ୍ଥିତି PARSE_OPT_ERROR:
			exit(129);
		ସ୍ଥିତି PARSE_OPT_COMPLETE:
			exit(0);
		ସ୍ଥିତି PARSE_OPT_NON_OPTION:
		ସ୍ଥିତି PARSE_OPT_DONE:
		{
			ଅଚଳ ଅକ୍ଷର *path = ctx.argv[0];
			ଅକ୍ଷର *p;

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
			ଯଦି (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = ଖାଲି;
			}

			setup_work_ବୃକ୍ଷ(the_repository);
			p = prefix_path(the_repository, prefix, prefix_length, path);
			update_one(p);
			ଯଦି (set_executable_bit)
				chmod_path(set_executable_bit, p);
			ମୁକ୍ତ(p);
			ctx.argc--;
			ctx.argv++;
			ଭାଙ୍ଗ;
		}
		ସ୍ଥିତି PARSE_OPT_UNKNOWN:
			ଯଦି (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			ଅନ୍ୟଥା
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_index_usage, options);
		}
	}
	argc = parse_options_end(&ctx);

	getline_fn = nul_term_ଧାଡ଼ି ? strbuf_getline_nul : strbuf_getline_lf;
	ଯଦି (preferred_index_format) {
		ଯଦି (preferred_index_format < 0) {
			printf(_("%d\n"), the_repository->index->version);
		} ଅନ୍ୟଥା ଯଦି (preferred_index_format < INDEX_FORMAT_LB ||
			   INDEX_FORMAT_UB < preferred_index_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_index_format,
			    INDEX_FORMAT_LB, INDEX_FORMAT_UB);
		} ଅନ୍ୟଥା {
			ଯଦି (the_repository->index->version != preferred_index_format)
				the_repository->index->cache_changed |= SOMETHING_CHANGED;
			report(_("index-version: was %d, set to %d"),
			       the_repository->index->version, preferred_index_format);
			the_repository->index->version = preferred_index_format;
		}
	}

	ଯଦି (read_from_stdin) {
		ସଂରଚନା strbuf buf = STRBUF_INIT;
		ସଂରଚନା strbuf unquoted = STRBUF_INIT;

		setup_work_ବୃକ୍ଷ(the_repository);
		ଯେତେବେଳେ (getline_fn(&buf, stdin) != EOF) {
			ଅକ୍ଷର *p;
			ଯଦି (!nul_term_ଧାଡ଼ି && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ଯଦି (unquote_c_style(&unquoted, buf.buf, ଖାଲି))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(the_repository, prefix, prefix_length, buf.buf);
			update_one(p);
			ଯଦି (set_executable_bit)
				chmod_path(set_executable_bit, p);
			ମୁକ୍ତ(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	ଯଦି (split_index > 0) {
		ଯଦି (repo_config_get_split_index(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		ଯଦି (the_repository->index->split_index)
			the_repository->index->cache_changed |= SPLIT_INDEX_ORDERED;
		ଅନ୍ୟଥା
			add_split_index(the_repository->index);
	} ଅନ୍ୟଥା ଯଦି (!split_index) {
		ଯଦି (repo_config_get_split_index(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_split_index(the_repository->index);
	}

	prepare_repo_settings(r);
	ବାଛ (untracked_cache) {
	ସ୍ଥିତି UC_UNSPECIFIED:
		ଭାଙ୍ଗ;
	ସ୍ଥିତି UC_DISABLE:
		ଯଦି (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->index);
		report(_("Untracked cache disabled"));
		ଭାଙ୍ଗ;
	ସ୍ଥିତି UC_TEST:
		setup_work_ବୃକ୍ଷ(the_repository);
		ଫେରନ୍ତୁ !test_if_untracked_cache_is_supported();
	ସ୍ଥିତି UC_ENABLE:
	ସ୍ଥିତି UC_FORCE:
		ଯଦି (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		add_untracked_cache(the_repository->index);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_ବୃକ୍ଷ(the_repository));
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	ଯଦି (fsmonitor > 0) {
		ଗଣନା fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		ଗଣନା fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		ଯଦି (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_ସନ୍ଦେଶ(r, reason));

		ଯଦି (fsm_mode == FSMONITOR_MODE_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		add_fsmonitor(the_repository->index);
		report(_("fsmonitor enabled"));
	} ଅନ୍ୟଥା ଯଦି (!fsmonitor) {
		ଗଣନା fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		ଯଦି (fsm_mode > FSMONITOR_MODE_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->index);
		report(_("fsmonitor disabled"));
	}

	ଯଦି (the_repository->index->cache_changed || force_write) {
		ଯଦି (newfd < 0) {
			ଯଦି (refresh_args.flags & REFRESH_QUIET)
				exit(128);
			unable_to_lock_die(repo_get_index_file(the_repository), lock_error);
		}
		ଯଦି (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die("Unable to write new index file");
	}

	rollback_lock_file(&lock_file);

	ଫେରନ୍ତୁ has_errors ? 1 : 0;
}
