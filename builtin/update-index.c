/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/transaction.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "resolve-undo.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "read-cache.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "split-index.h"
#अन्तर्भाव "symlinks.h"
#अन्तर्भाव "fsmonitor.h"
#अन्तर्भाव "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
स्थिर पूर्णाङ्क allow_सङ्कलय;
स्थिर पूर्णाङ्क allow_remove;
स्थिर पूर्णाङ्क allow_replace;
स्थिर पूर्णाङ्क info_only;
स्थिर पूर्णाङ्क force_remove;
स्थिर पूर्णाङ्क verbose;
स्थिर पूर्णाङ्क mark_valid_only;
स्थिर पूर्णाङ्क mark_skip_worktree_only;
स्थिर पूर्णाङ्क mark_fsmonitor_only;
स्थिर पूर्णाङ्क ignore_skip_worktree_entries;
#परिभाषय MARK_FLAG 1
#परिभाषय UNMARK_FLAG 2
स्थिर संरचना strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
गणना uc_बहुलकम् {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (प्रारूप_लिख, 1, 2)))
स्थिर शून्य report(ध्रुव वर्ण *fmt, ...)
{
	va_list vp;

	यदि (!verbose)
		प्रत्यागम;

	va_start(vp, fmt);
	vprintf(fmt, vp);
	अक्षरं_लिख('\n');
	va_अन्तः(vp);
}

स्थिर शून्य remove_test_directory(शून्य)
{
	यदि (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

स्थिर ध्रुव वर्ण *get_mtime_मार्गः(ध्रुव वर्ण *मार्गः)
{
	स्थिर संरचना strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, मार्गः);
	प्रत्यागम sb.buf;
}

स्थिर शून्य xmkdir(ध्रुव वर्ण *मार्गः)
{
	मार्गः = get_mtime_मार्गः(मार्गः);
	यदि (mkdir(मार्गः, 0700))
		die_errno(_("failed to create directory %s"), मार्गः);
}

स्थिर पूर्णाङ्क xstat_mtime_dir(संरचना stat *st)
{
	यदि (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क create_file(ध्रुव वर्ण *मार्गः)
{
	पूर्णाङ्क fd;
	मार्गः = get_mtime_मार्गः(मार्गः);
	fd = xopen(मार्गः, O_CREAT | O_RDWR, 0644);
	प्रत्यागम fd;
}

स्थिर शून्य xunlink(ध्रुव वर्ण *मार्गः)
{
	मार्गः = get_mtime_मार्गः(मार्गः);
	यदि (unlink(मार्गः))
		die_errno(_("failed to delete file %s"), मार्गः);
}

स्थिर शून्य xrmdir(ध्रुव वर्ण *मार्गः)
{
	मार्गः = get_mtime_मार्गः(मार्गः);
	यदि (rmdir(मार्गः))
		die_errno(_("failed to delete directory %s"), मार्गः);
}

स्थिर शून्य avoid_racy(शून्य)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

स्थिर पूर्णाङ्क test_if_untracked_cache_is_supported(शून्य)
{
	संरचना stat st;
	संरचना stat_दत्त आधारः;
	पूर्णाङ्क fd, ret = 0;
	वर्ण *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	यदि (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	मुक्त(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_दत्त(&आधारः, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	यदि (!मेलनम्_stat_दत्त(&आधारः, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		गच्छ समाप्तम्;
	}
	fill_stat_दत्त(&आधारः, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	यदि (!मेलनम्_stat_दत्त(&आधारः, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		गच्छ समाप्तम्;
	}
	fill_stat_दत्त(&आधारः, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	यदि (मेलनम्_stat_दत्त(&आधारः, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		गच्छ समाप्तम्;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	यदि (मेलनम्_stat_दत्त(&आधारः, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		गच्छ समाप्तम्;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	यदि (!मेलनम्_stat_दत्त(&आधारः, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		गच्छ समाप्तम्;
	}
	fill_stat_दत्त(&आधारः, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	यदि (!मेलनम्_stat_दत्त(&आधारः, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		गच्छ समाप्तम्;
	}

	यदि (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

समाप्तम्:
	strbuf_release(&mtime_dir);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क mark_ce_flags(ध्रुव वर्ण *मार्गः, पूर्णाङ्क flag, पूर्णाङ्क mark)
{
	पूर्णाङ्क namelen = वाक्यदैर्घ्यम्(मार्गः);
	पूर्णाङ्क pos = सूचकांक_name_pos(the_repository->सूचकांक, मार्गः, namelen);
	यदि (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->सूचकांक, the_repository->सूचकांक->cache[pos]);
		यदि (mark)
			the_repository->सूचकांक->cache[pos]->ce_flags |= flag;
		अन्यथा
			the_repository->सूचकांक->cache[pos]->ce_flags &= ~flag;
		the_repository->सूचकांक->cache[pos]->ce_flags |= CE_UPDATE_अन्तर्_आधारः;
		cache_वृक्षः_invalidate_मार्गः(the_repository->सूचकांक, मार्गः);
		the_repository->सूचकांक->cache_परिवर्तितम् |= CE_प्रविष्टिः_परिवर्तितम्;
		प्रत्यागम 0;
	}
	प्रत्यागम -1;
}

स्थिर पूर्णाङ्क remove_one_मार्गः(ध्रुव वर्ण *मार्गः)
{
	यदि (!allow_remove)
		प्रत्यागम error("%s: does not exist and --remove not passed", मार्गः);
	यदि (remove_file_from_सूचकांक(the_repository->सूचकांक, मार्गः))
		प्रत्यागम error("%s: cannot remove from the index", मार्गः);
	प्रत्यागम 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
स्थिर पूर्णाङ्क process_lstat_error(ध्रुव वर्ण *मार्गः, पूर्णाङ्क err)
{
	यदि (is_missing_file_error(err))
		प्रत्यागम remove_one_मार्गः(मार्गः);
	प्रत्यागम error("lstat(\"%s\"): %s", मार्गः, strerror(err));
}

स्थिर पूर्णाङ्क सङ्कलय_one_मार्गः(ध्रुव संरचना cache_प्रविष्टिः *old, ध्रुव वर्ण *मार्गः, पूर्णाङ्क len, संरचना stat *st)
{
	पूर्णाङ्क option;
	संरचना cache_प्रविष्टिः *ce;

	/* Was the old index entry already up-to-date? */
	यदि (old && !ce_stage(old) && !ie_मेलनम्_stat(the_repository->सूचकांक, old, st, 0))
		प्रत्यागम 0;

	ce = make_रिक्तानि_cache_प्रविष्टिः(the_repository->सूचकांक, len);
	स्मृतिं_प्रतिलिख(ce->name, मार्गः, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->सूचकांक, ce, st);
	ce->ce_बहुलकम् = ce_बहुलकम्_from_stat(the_repository, old, st->st_बहुलकम्);

	यदि (सूचकांक_मार्गः(the_repository->सूचकांक, &ce->oid, मार्गः, st,
		       info_only ? 0 : सूचकांक_WRITE_OBJECT)) {
		discard_cache_प्रविष्टिः(ce);
		प्रत्यागम -1;
	}
	option = allow_सङ्कलय ? सङ्कलय_CACHE_OK_गन्तव्यम्_सङ्कलय : 0;
	option |= allow_replace ? सङ्कलय_CACHE_OK_गन्तव्यम्_REPLACE : 0;
	यदि (सङ्कलय_सूचकांक_प्रविष्टिः(the_repository->सूचकांक, ce, option)) {
		discard_cache_प्रविष्टिः(ce);
		प्रत्यागम error("%s: cannot add to the index - missing --add option?", मार्गः);
	}
	प्रत्यागम 0;
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
स्थिर पूर्णाङ्क process_directory(ध्रुव वर्ण *मार्गः, पूर्णाङ्क len, संरचना stat *st)
{
	संरचना object_id oid;
	पूर्णाङ्क pos = सूचकांक_name_pos(the_repository->सूचकांक, मार्गः, len);

	/* Exact match: file or existing gitlink */
	यदि (pos >= 0) {
		ध्रुव संरचना cache_प्रविष्टिः *ce = the_repository->सूचकांक->cache[pos];
		यदि (S_ISGITLINK(ce->ce_बहुलकम्)) {

			/* Do nothing to the index if there is no HEAD! */
			यदि (repo_resolve_gitlink_ref(the_repository, मार्गः,
						     "HEAD", &oid) < 0)
				प्रत्यागम 0;

			प्रत्यागम सङ्कलय_one_मार्गः(ce, मार्गः, len, st);
		}
		/* Should this be an unconditional error? */
		प्रत्यागम remove_one_मार्गः(मार्गः);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	यावत् (pos < the_repository->सूचकांक->cache_nr) {
		ध्रुव संरचना cache_प्रविष्टिः *ce = the_repository->सूचकांक->cache[pos++];

		यदि (वाक्ये_तुलय(ce->name, मार्गः, len))
			भङ्ग;
		यदि (ce->name[len] > '/')
			भङ्ग;
		यदि (ce->name[len] < '/')
			अनुवर्त;

		/* Subdirectory match - error out */
		प्रत्यागम error("%s: is a directory - add individual files instead", मार्गः);
	}

	/* No match - should we add it as a gitlink? */
	यदि (!repo_resolve_gitlink_ref(the_repository, मार्गः, "HEAD", &oid))
		प्रत्यागम सङ्कलय_one_मार्गः(रिक्त, मार्गः, len, st);

	/* Error out. */
	प्रत्यागम error("%s: is a directory - add files inside instead", मार्गः);
}

स्थिर पूर्णाङ्क process_मार्गः(ध्रुव वर्ण *मार्गः, संरचना stat *st, पूर्णाङ्क stat_errno)
{
	पूर्णाङ्क pos, len;
	ध्रुव संरचना cache_प्रविष्टिः *ce;

	len = वाक्यदैर्घ्यम्(मार्गः);
	यदि (has_symlink_leading_मार्गः(मार्गः, len))
		प्रत्यागम error("'%s' is beyond a symbolic link", मार्गः);

	pos = सूचकांक_name_pos(the_repository->सूचकांक, मार्गः, len);
	ce = pos < 0 ? रिक्त : the_repository->सूचकांक->cache[pos];
	यदि (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		यदि (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_सूचकांक(the_repository->सूचकांक, मार्गः))
			प्रत्यागम error("%s: cannot remove from the index", मार्गः);
		प्रत्यागम 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	यदि (stat_errno)
		प्रत्यागम process_lstat_error(मार्गः, stat_errno);

	यदि (S_ISDIR(st->st_बहुलकम्))
		प्रत्यागम process_directory(मार्गः, len, st);

	प्रत्यागम सङ्कलय_one_मार्गः(ce, मार्गः, len, st);
}

स्थिर पूर्णाङ्क सङ्कलय_cacheinfo(अचिह्नित पूर्णाङ्क बहुलकम्, ध्रुव संरचना object_id *oid,
			 ध्रुव वर्ण *मार्गः, पूर्णाङ्क stage)
{
	पूर्णाङ्क len, option;
	संरचना cache_प्रविष्टिः *ce;

	यदि (!verify_मार्गः(मार्गः, बहुलकम्))
		प्रत्यागम error("Invalid path '%s'", मार्गः);

	len = वाक्यदैर्घ्यम्(मार्गः);
	ce = make_रिक्तानि_cache_प्रविष्टिः(the_repository->सूचकांक, len);

	oidcpy(&ce->oid, oid);
	स्मृतिं_प्रतिलिख(ce->name, मार्गः, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_बहुलकम् = create_ce_बहुलकम्(बहुलकम्);
	यदि (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_सङ्कलय ? सङ्कलय_CACHE_OK_गन्तव्यम्_सङ्कलय : 0;
	option |= allow_replace ? सङ्कलय_CACHE_OK_गन्तव्यम्_REPLACE : 0;
	यदि (सङ्कलय_सूचकांक_प्रविष्टिः(the_repository->सूचकांक, ce, option))
		प्रत्यागम error("%s: cannot add to the index - missing --add option?",
			     मार्गः);
	report("add '%s'", मार्गः);
	प्रत्यागम 0;
}

स्थिर शून्य chmod_मार्गः(वर्ण flip, ध्रुव वर्ण *मार्गः)
{
	पूर्णाङ्क pos;
	संरचना cache_प्रविष्टिः *ce;

	pos = सूचकांक_name_pos(the_repository->सूचकांक, मार्गः, वाक्यदैर्घ्यम्(मार्गः));
	यदि (pos < 0)
		गच्छ fail;
	ce = the_repository->सूचकांक->cache[pos];
	यदि (chmod_सूचकांक_प्रविष्टिः(the_repository->सूचकांक, ce, flip) < 0)
		गच्छ fail;

	report("chmod %cx '%s'", flip, मार्गः);
	प्रत्यागम;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, मार्गः);
}

स्थिर शून्य update_one(ध्रुव वर्ण *मार्गः)
{
	पूर्णाङ्क stat_errno = 0;
	संरचना stat st;

	यदि (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_बहुलकम् = 0;
	अन्यथा यदि (lstat(मार्गः, &st) < 0) {
		st.st_बहुलकम् = 0;
		stat_errno = errno;
	} /* else stat is valid */

	यदि (!verify_मार्गः(मार्गः, st.st_बहुलकम्)) {
		fprintf(stderr, "Ignoring path %s\n", मार्गः);
		प्रत्यागम;
	}
	यदि (mark_valid_only) {
		यदि (mark_ce_flags(मार्गः, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", मार्गः);
		प्रत्यागम;
	}
	यदि (mark_skip_worktree_only) {
		यदि (mark_ce_flags(मार्गः, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", मार्गः);
		प्रत्यागम;
	}
	यदि (mark_fsmonitor_only) {
		यदि (mark_ce_flags(मार्गः, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", मार्गः);
		प्रत्यागम;
	}

	यदि (force_remove) {
		यदि (remove_file_from_सूचकांक(the_repository->सूचकांक, मार्गः))
			die("git update-index: unable to remove %s", मार्गः);
		report("remove '%s'", मार्गः);
		प्रत्यागम;
	}
	यदि (process_मार्गः(मार्गः, &st, stat_errno))
		die("Unable to process path %s", मार्गः);
	report("add '%s'", मार्गः);
}

स्थिर शून्य read_सूचकांक_info(पूर्णाङ्क nul_पदांशः_पङ्क्तिः)
{
	ध्रुव पूर्णाङ्क hexsz = the_hash_algo->hexsz;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_पदांशः_पङ्क्तिः ? strbuf_getline_nul : strbuf_getline_lf;
	यावत् (getline_fn(&buf, stdin) != EOF) {
		वर्ण *ptr, *tab;
		वर्ण *मार्गः_name;
		संरचना object_id oid;
		अचिह्नित पूर्णाङ्क बहुलकम्;
		अचिह्नित दीर्घ ul;
		पूर्णाङ्क stage;

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
		यदि (ptr == buf.buf || *ptr != ' '
		    || errno || (अचिह्नित पूर्णाङ्क) ul != ul)
			गच्छ bad_पङ्क्तिः;
		बहुलकम् = ul;

		tab = strchr(ptr, '\t');
		यदि (!tab || tab - ptr < hexsz + 1)
			गच्छ bad_पङ्क्तिः;

		यदि (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		अन्यथा {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		यदि (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			गच्छ bad_पङ्क्तिः;

		मार्गः_name = ptr;
		यदि (!nul_पदांशः_पङ्क्तिः && मार्गः_name[0] == '"') {
			strbuf_reset(&uq);
			यदि (unquote_c_style(&uq, मार्गः_name, रिक्त)) {
				die("git update-index: bad quoting of path name");
			}
			मार्गः_name = uq.buf;
		}

		यदि (!verify_मार्गः(मार्गः_name, बहुलकम्)) {
			fprintf(stderr, "Ignoring path %s\n", मार्गः_name);
			अनुवर्त;
		}

		यदि (!बहुलकम्) {
			/* mode == 0 means there is no such path -- remove */
			यदि (remove_file_from_सूचकांक(the_repository->सूचकांक, मार्गः_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		अन्यथा {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			यदि (सङ्कलय_cacheinfo(बहुलकम्, &oid, मार्गः_name, stage))
				die("git update-index: unable to update %s",
				    मार्गः_name);
		}
		अनुवर्त;

	bad_पङ्क्तिः:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

स्थिर ध्रुव वर्ण * ध्रुव update_सूचकांक_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	रिक्त
};

स्थिर संरचना cache_प्रविष्टिः *read_one_ent(ध्रुव वर्ण *which,
					संरचना object_id *ent, ध्रुव वर्ण *मार्गः,
					पूर्णाङ्क namelen, पूर्णाङ्क stage)
{
	अचिह्नित ह्रस्व बहुलकम्;
	संरचना object_id oid;
	संरचना cache_प्रविष्टिः *ce;

	यदि (get_वृक्षः_प्रविष्टिः(the_repository, ent, मार्गः, &oid, &बहुलकम्)) {
		यदि (which)
			error("%s: not in %s branch.", मार्गः, which);
		प्रत्यागम रिक्त;
	}
	यदि (!the_repository->सूचकांक->sparse_सूचकांक && बहुलकम् == S_IFDIR) {
		यदि (which)
			error("%s: not a blob in %s branch.", मार्गः, which);
		प्रत्यागम रिक्त;
	}
	ce = make_रिक्तानि_cache_प्रविष्टिः(the_repository->सूचकांक, namelen);

	oidcpy(&ce->oid, &oid);
	स्मृतिं_प्रतिलिख(ce->name, मार्गः, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_बहुलकम् = create_ce_बहुलकम्(बहुलकम्);
	प्रत्यागम ce;
}

स्थिर पूर्णाङ्क unresolve_one(ध्रुव वर्ण *मार्गः)
{
	संरचना string_list_item *item;
	पूर्णाङ्क res = 0;

	यदि (!the_repository->सूचकांक->resolve_undo)
		प्रत्यागम res;
	item = string_list_lookup(the_repository->सूचकांक->resolve_undo, मार्गः);
	यदि (!item)
		प्रत्यागम res; /* no resolve-undo record for the path */
	res = unmerge_सूचकांक_प्रविष्टिः(the_repository->सूचकांक, मार्गः, item->util, 0);
	FREE_AND_NULL(item->util);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क do_unresolve(पूर्णाङ्क ac, ध्रुव वर्ण **av,
			ध्रुव वर्ण *उपसर्गः, पूर्णाङ्क उपसर्गः_दीर्घता)
{
	पूर्णाङ्क i;
	पूर्णाङ्क err = 0;

	प्रति (i = 1; i < ac; i++) {
		ध्रुव वर्ण *arg = av[i];
		वर्ण *p = उपसर्गः_मार्गः(the_repository, उपसर्गः, उपसर्गः_दीर्घता, arg);
		err |= unresolve_one(p);
		मुक्त(p);
	}
	प्रत्यागम err;
}

स्थिर पूर्णाङ्क do_reupdate(ध्रुव वर्ण **paths,
		       ध्रुव वर्ण *उपसर्गः)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	पूर्णाङ्क pos;
	पूर्णाङ्क has_शीर्ष = 1;
	संरचना pathspec pathspec;
	संरचना object_id शीर्ष_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       उपसर्गः, paths);

	यदि (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &शीर्ष_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_शीर्ष = 0;
 redo:
	प्रति (pos = 0; pos < the_repository->सूचकांक->cache_nr; pos++) {
		ध्रुव संरचना cache_प्रविष्टिः *ce = the_repository->सूचकांक->cache[pos];
		संरचना cache_प्रविष्टिः *old = रिक्त;
		पूर्णाङ्क save_nr;
		वर्ण *मार्गः;

		यदि (ce_stage(ce) || !ce_मार्गः_मेलनम्(the_repository->सूचकांक, ce, &pathspec, रिक्त))
			अनुवर्त;
		यदि (has_शीर्ष)
			old = read_one_ent(रिक्त, &शीर्ष_oid,
					   ce->name, ce_namelen(ce), 0);
		यदि (old && ce->ce_बहुलकम् == old->ce_बहुलकम् &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_प्रविष्टिः(old);
			अनुवर्त; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		यदि (S_ISSPARSEDIR(ce->ce_बहुलकम्)) {
			ensure_पूर्णम्_सूचकांक(the_repository->सूचकांक);
			गच्छ redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->सूचकांक->cache_nr;
		मार्गः = xstrdup(ce->name);
		update_one(मार्गः);
		मुक्त(मार्गः);
		discard_cache_प्रविष्टिः(old);
		यदि (save_nr != the_repository->सूचकांक->cache_nr)
			गच्छ redo;
	}
	clear_pathspec(&pathspec);
	प्रत्यागम 0;
}

संरचना refresh_params {
	अचिह्नित पूर्णाङ्क flags;
	पूर्णाङ्क *has_errors;
};

स्थिर पूर्णाङ्क refresh(संरचना refresh_params *o, अचिह्नित पूर्णाङ्क flag)
{
	सज्जता_work_वृक्षः(the_repository);
	repo_read_सूचकांक(the_repository);
	*o->has_errors |= refresh_सूचकांक(the_repository->सूचकांक, o->flags | flag, रिक्त,
					रिक्त, रिक्त);
	यदि (has_racy_timestamp(the_repository->सूचकांक)) {
		/*
		 * Even if nothing else has changed, updating the file
		 * increases the chance that racy timestamps become
		 * non-racy, helping future run-time performance.
		 * We do that even in case of "errors" returned by
		 * refresh_index() as these are no actual errors.
		 * cmd_status() does the same.
		 */
		the_repository->सूचकांक->cache_परिवर्तितम् |= SOMETHING_परिवर्तितम्;
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क refresh_callback(ध्रुव संरचना option *opt,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	प्रत्यागम refresh(opt->मानम्, 0);
}

स्थिर पूर्णाङ्क really_refresh_callback(ध्रुव संरचना option *opt,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	प्रत्यागम refresh(opt->मानम्, REFRESH_REALLY);
}

स्थिर पूर्णाङ्क chmod_callback(ध्रुव संरचना option *opt,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	वर्ण *flip = opt->मानम्;
	BUG_ON_OPT_NEG(unset);
	यदि ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		प्रत्यागम error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क resolve_undo_clear_callback(ध्रुव संरचना option *opt अप्रयुक्तम्,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_सूचकांक(the_repository->सूचकांक);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_new_style_cacheinfo(ध्रुव वर्ण *arg,
				     अचिह्नित पूर्णाङ्क *बहुलकम्,
				     संरचना object_id *oid,
				     ध्रुव वर्ण **मार्गः)
{
	अचिह्नित दीर्घ ul;
	वर्ण *endp;
	ध्रुव वर्ण *p;

	यदि (!arg)
		प्रत्यागम -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	यदि (errno || endp == arg || *endp != ',' || (अचिह्नित पूर्णाङ्क) ul != ul)
		प्रत्यागम -1; /* not a new-style cacheinfo */
	*बहुलकम् = ul;
	endp++;
	यदि (parse_oid_hex(endp, oid, &p) || *p != ',')
		प्रत्यागम -1;
	*मार्गः = p + 1;
	प्रत्यागम 0;
}

स्थिर गणना parse_opt_परिणाम cacheinfo_callback(
	संरचना parse_opt_ctx_t *ctx, ध्रुव संरचना option *opt अप्रयुक्तम्,
	ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना object_id oid;
	अचिह्नित पूर्णाङ्क बहुलकम्;
	ध्रुव वर्ण *मार्गः;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	यदि (!parse_new_style_cacheinfo(ctx->argv[1], &बहुलकम्, &oid, &मार्गः)) {
		यदि (सङ्कलय_cacheinfo(बहुलकम्, &oid, मार्गः, 0))
			die("git update-index: --cacheinfo cannot add %s", मार्गः);
		ctx->argv++;
		ctx->argc--;
		प्रत्यागम 0;
	}
	यदि (ctx->argc <= 3)
		प्रत्यागम error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	यदि (strtoul_ui(*++ctx->argv, 8, &बहुलकम्) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    सङ्कलय_cacheinfo(बहुलकम्, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	प्रत्यागम 0;
}

स्थिर गणना parse_opt_परिणाम stdin_cacheinfo_callback(
	संरचना parse_opt_ctx_t *ctx, ध्रुव संरचना option *opt,
	ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *nul_पदांशः_पङ्क्तिः = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	यदि (ctx->argc != 1)
		प्रत्यागम error("option '%s' must be the last argument", opt->long_name);
	allow_सङ्कलय = allow_replace = allow_remove = 1;
	read_सूचकांक_info(*nul_पदांशः_पङ्क्तिः);
	प्रत्यागम 0;
}

स्थिर गणना parse_opt_परिणाम stdin_callback(
	संरचना parse_opt_ctx_t *ctx, ध्रुव संरचना option *opt,
	ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *read_from_stdin = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	यदि (ctx->argc != 1)
		प्रत्यागम error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	प्रत्यागम 0;
}

स्थिर गणना parse_opt_परिणाम unresolve_callback(
	संरचना parse_opt_ctx_t *ctx, ध्रुव संरचना option *opt,
	ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *has_errors = opt->मानम्;
	ध्रुव वर्ण *उपसर्गः = the_repository->उपसर्गः;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				उपसर्गः, उपसर्गः ? वाक्यदैर्घ्यम्(उपसर्गः) : 0);
	यदि (*has_errors)
		the_repository->सूचकांक->cache_परिवर्तितम् = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	प्रत्यागम 0;
}

स्थिर गणना parse_opt_परिणाम reupdate_callback(
	संरचना parse_opt_ctx_t *ctx, ध्रुव संरचना option *opt,
	ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *has_errors = opt->मानम्;
	ध्रुव वर्ण *उपसर्गः = the_repository->उपसर्गः;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	सज्जता_work_वृक्षः(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, उपसर्गः);
	यदि (*has_errors)
		the_repository->सूचकांक->cache_परिवर्तितम् = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_update_सूचकांक(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *उपसर्गः,
		     संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क newfd, entries, has_errors = 0, nul_पदांशः_पङ्क्तिः = 0;
	गणना uc_बहुलकम् untracked_cache = UC_UNSPECIFIED;
	पूर्णाङ्क read_from_stdin = 0;
	पूर्णाङ्क उपसर्गः_दीर्घता = उपसर्गः ? वाक्यदैर्घ्यम्(उपसर्गः) : 0;
	पूर्णाङ्क preferred_सूचकांक_format = 0;
	वर्ण समुच्चयः_executable_bit = 0;
	संरचना refresh_params refresh_तर्काः = {0, &has_errors};
	पूर्णाङ्क तालकम्_error = 0;
	पूर्णाङ्क विभज_सूचकांक = -1;
	पूर्णाङ्क force_write = 0;
	पूर्णाङ्क fsmonitor = -1;
	संरचना तालकम्_file तालकम्_file = तालकम्_INIT;
	संरचना parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	पूर्णाङ्क parseopt_state = PARSE_OPT_UNKNOWN;
	संरचना repository *r = the_repository;
	संरचना odb_transaction *transaction;
	संरचना option options[] = {
		OPT_BIT('q', रिक्त, &refresh_तर्काः.flags,
			N_("continue refresh even when index needs update"),
			REFRESH_QUIET),
		OPT_BIT(0, "ignore-submodules", &refresh_तर्काः.flags,
			N_("refresh: ignore submodules"),
			REFRESH_IGNORE_SUBMODULES),
		OPT_समुच्चयः_INT(0, "add", &allow_सङ्कलय,
			N_("do not ignore new files"), 1),
		OPT_समुच्चयः_INT(0, "replace", &allow_replace,
			N_("let files replace directories and vice-versa"), 1),
		OPT_समुच्चयः_INT(0, "remove", &allow_remove,
			N_("notice files missing from worktree"), 1),
		OPT_BIT(0, "unmerged", &refresh_तर्काः.flags,
			N_("refresh even if index contains unmerged entries"),
			REFRESH_UNMERGED),
		OPT_CALLBACK_F(0, "refresh", &refresh_तर्काः, रिक्त,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_तर्काः, रिक्त,
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
		OPT_CALLBACK_F(0, "chmod", &समुच्चयः_executable_bit, "(+|-)x",
			N_("override the executable bit of the listed files"),
			PARSE_OPT_NONEG,
			chmod_callback),
		{
			.type = OPTION_समुच्चयः_INT,
			.long_name = "assume-unchanged",
			.मानम् = &mark_valid_only,
			.precision = परिमाण(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_समुच्चयः_INT,
			.long_name = "no-assume-unchanged",
			.मानम् = &mark_valid_only,
			.precision = परिमाण(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_समुच्चयः_INT,
			.long_name = "skip-worktree",
			.मानम् = &mark_skip_worktree_only,
			.precision = परिमाण(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_समुच्चयः_INT,
			.long_name = "no-skip-worktree",
			.मानम् = &mark_skip_worktree_only,
			.precision = परिमाण(mark_skip_worktree_only),
			.help = N_("clear skip-worktree bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_BOOL(0, "ignore-skip-worktree-entries", &ignore_skip_worktree_entries,
			 N_("do not touch index-only entries")),
		OPT_समुच्चयः_INT(0, "info-only", &info_only,
			N_("add to index only; do not add content to object database"), 1),
		OPT_समुच्चयः_INT(0, "force-remove", &force_remove,
			N_("remove named paths even if present in worktree"), 1),
		OPT_BOOL('z', रिक्त, &nul_पदांशः_पङ्क्तिः,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.मानम् = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.मानम् = &nul_पदांशः_पङ्क्तिः,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.मानम् = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.मानम् = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_BIT(0, "ignore-missing", &refresh_तर्काः.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_समुच्चयः_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", रिक्त, रिक्त,
			N_("(for porcelains) forget saved unresolved conflicts"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			resolve_undo_clear_callback),
		OPT_INTEGER(0, "index-version", &preferred_सूचकांक_format,
			N_("write index in this format")),
		OPT_समुच्चयः_INT(0, "show-index-version", &preferred_सूचकांक_format,
			    N_("report on-disk index format version"), -1),
		OPT_BOOL(0, "split-index", &विभज_सूचकांक,
			N_("enable or disable split index")),
		OPT_BOOL(0, "untracked-cache", &untracked_cache,
			N_("enable/disable untracked cache")),
		OPT_समुच्चयः_INT(0, "test-untracked-cache", &untracked_cache,
			    N_("test if the filesystem supports untracked cache"), UC_TEST),
		OPT_समुच्चयः_INT(0, "force-untracked-cache", &untracked_cache,
			    N_("enable untracked cache without testing the filesystem"), UC_FORCE),
		OPT_समुच्चयः_INT(0, "force-write-index", &force_write,
			N_("write out the index even if is not flagged as changed"), 1),
		OPT_BOOL(0, "fsmonitor", &fsmonitor,
			N_("enable or disable file system monitor")),
		{
			.type = OPTION_समुच्चयः_INT,
			.long_name = "fsmonitor-valid",
			.मानम् = &mark_fsmonitor_only,
			.precision = परिमाण(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_समुच्चयः_INT,
			.long_name = "no-fsmonitor-valid",
			.मानम् = &mark_fsmonitor_only,
			.precision = परिमाण(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_अन्तः()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_सूचकांक_usage, options);

	repo_config(the_repository, git_default_config, रिक्त);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_सूचकांक(the_repository, &तालकम्_file, 0);
	यदि (newfd < 0)
		तालकम्_error = errno;

	entries = repo_read_सूचकांक(the_repository);
	यदि (entries < 0)
		die("cache corrupted");

	the_repository->सूचकांक->updated_skipworktree = 1;

	/*
	 * Custom copy of parse_options() because we want to handle
	 * filename arguments as they come.
	 */
	parse_options_start(&ctx, argc, argv, उपसर्गः,
			    options, PARSE_OPT_STOP_AT_NON_OPTION);

	/*
	 * Allow the object layer to optimize adding multiple objects in
	 * a batch.
	 */
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	यावत् (ctx.argc) {
		यदि (parseopt_state != PARSE_OPT_समाप्तम्)
			parseopt_state = parse_options_चरणः(&ctx, options,
							    update_सूचकांक_usage);
		यदि (!ctx.argc)
			भङ्ग;
		विकल्प (parseopt_state) {
		पक्ष PARSE_OPT_HELP:
			निर्गच्छ(0);
		पक्ष PARSE_OPT_HELP_ERROR:
		पक्ष PARSE_OPT_ERROR:
			निर्गच्छ(129);
		पक्ष PARSE_OPT_COMPLETE:
			निर्गच्छ(0);
		पक्ष PARSE_OPT_NON_OPTION:
		पक्ष PARSE_OPT_समाप्तम्:
		{
			ध्रुव वर्ण *मार्गः = ctx.argv[0];
			वर्ण *p;

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
			यदि (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = रिक्त;
			}

			सज्जता_work_वृक्षः(the_repository);
			p = उपसर्गः_मार्गः(the_repository, उपसर्गः, उपसर्गः_दीर्घता, मार्गः);
			update_one(p);
			यदि (समुच्चयः_executable_bit)
				chmod_मार्गः(समुच्चयः_executable_bit, p);
			मुक्त(p);
			ctx.argc--;
			ctx.argv++;
			भङ्ग;
		}
		पक्ष PARSE_OPT_UNKNOWN:
			यदि (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			अन्यथा
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_सूचकांक_usage, options);
		}
	}
	argc = parse_options_अन्तः(&ctx);

	getline_fn = nul_पदांशः_पङ्क्तिः ? strbuf_getline_nul : strbuf_getline_lf;
	यदि (preferred_सूचकांक_format) {
		यदि (preferred_सूचकांक_format < 0) {
			प्रारूप_लिख(_("%d\n"), the_repository->सूचकांक->संस्करणम्);
		} अन्यथा यदि (preferred_सूचकांक_format < सूचकांक_FORMAT_LB ||
			   सूचकांक_FORMAT_UB < preferred_सूचकांक_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_सूचकांक_format,
			    सूचकांक_FORMAT_LB, सूचकांक_FORMAT_UB);
		} अन्यथा {
			यदि (the_repository->सूचकांक->संस्करणम् != preferred_सूचकांक_format)
				the_repository->सूचकांक->cache_परिवर्तितम् |= SOMETHING_परिवर्तितम्;
			report(_("index-version: was %d, set to %d"),
			       the_repository->सूचकांक->संस्करणम्, preferred_सूचकांक_format);
			the_repository->सूचकांक->संस्करणम् = preferred_सूचकांक_format;
		}
	}

	यदि (read_from_stdin) {
		संरचना strbuf buf = STRBUF_INIT;
		संरचना strbuf unquoted = STRBUF_INIT;

		सज्जता_work_वृक्षः(the_repository);
		यावत् (getline_fn(&buf, stdin) != EOF) {
			वर्ण *p;
			यदि (!nul_पदांशः_पङ्क्तिः && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				यदि (unquote_c_style(&unquoted, buf.buf, रिक्त))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = उपसर्गः_मार्गः(the_repository, उपसर्गः, उपसर्गः_दीर्घता, buf.buf);
			update_one(p);
			यदि (समुच्चयः_executable_bit)
				chmod_मार्गः(समुच्चयः_executable_bit, p);
			मुक्त(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	यदि (विभज_सूचकांक > 0) {
		यदि (repo_config_get_विभज_सूचकांक(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		यदि (the_repository->सूचकांक->विभज_सूचकांक)
			the_repository->सूचकांक->cache_परिवर्तितम् |= विभज_सूचकांक_ORDERED;
		अन्यथा
			सङ्कलय_विभज_सूचकांक(the_repository->सूचकांक);
	} अन्यथा यदि (!विभज_सूचकांक) {
		यदि (repo_config_get_विभज_सूचकांक(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_विभज_सूचकांक(the_repository->सूचकांक);
	}

	prepare_repo_settings(r);
	विकल्प (untracked_cache) {
	पक्ष UC_UNSPECIFIED:
		भङ्ग;
	पक्ष UC_DISABLE:
		यदि (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->सूचकांक);
		report(_("Untracked cache disabled"));
		भङ्ग;
	पक्ष UC_TEST:
		सज्जता_work_वृक्षः(the_repository);
		प्रत्यागम !test_if_untracked_cache_is_supported();
	पक्ष UC_ENABLE:
	पक्ष UC_FORCE:
		यदि (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		सङ्कलय_untracked_cache(the_repository->सूचकांक);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_वृक्षः(the_repository));
		भङ्ग;
	स्वभाव:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	यदि (fsmonitor > 0) {
		गणना fsmonitor_बहुलकम् fsm_बहुलकम् = fsm_settings_get_बहुलकम्(r);
		गणना fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		यदि (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_सन्देशः(r, reason));

		यदि (fsm_बहुलकम् == FSMONITOR_बहुलकम्_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		सङ्कलय_fsmonitor(the_repository->सूचकांक);
		report(_("fsmonitor enabled"));
	} अन्यथा यदि (!fsmonitor) {
		गणना fsmonitor_बहुलकम् fsm_बहुलकम् = fsm_settings_get_बहुलकम्(r);
		यदि (fsm_बहुलकम् > FSMONITOR_बहुलकम्_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->सूचकांक);
		report(_("fsmonitor disabled"));
	}

	यदि (the_repository->सूचकांक->cache_परिवर्तितम् || force_write) {
		यदि (newfd < 0) {
			यदि (refresh_तर्काः.flags & REFRESH_QUIET)
				निर्गच्छ(128);
			unable_गन्तव्यम्_तालकम्_die(repo_get_सूचकांक_file(the_repository), तालकम्_error);
		}
		यदि (write_locked_सूचकांक(the_repository->सूचकांक, &तालकम्_file, COMMIT_तालकम्))
			die("Unable to write new index file");
	}

	rollback_तालकम्_file(&तालकम्_file);

	प्रत्यागम has_errors ? 1 : 0;
}
