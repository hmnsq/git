/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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
स्थिर पूर्णाङ्क allow_add;
स्थिर पूर्णाङ्क allow_remove;
स्थिर पूर्णाङ्क allow_replace;
स्थिर पूर्णाङ्क info_only;
स्थिर पूर्णाङ्क force_remove;
स्थिर पूर्णाङ्क verbose;
स्थिर पूर्णाङ्क mark_valid_only;
स्थिर पूर्णाङ्क mark_skip_worktree_only;
स्थिर पूर्णाङ्क mark_fsmonitor_only;
स्थिर पूर्णाङ्क ignore_skip_worktree_entries;
#define MARK_FLAG 1
#define UNMARK_FLAG 2
स्थिर संरचना strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
गणना uc_mode {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (printf, 1, 2)))
स्थिर शून्य report(ध्रुव वर्ण *fmt, ...)
{
	va_list vp;

	सचे (!verbose)
		पटिनिवत्त;

	va_start(vp, fmt);
	vprintf(fmt, vp);
	putchar('\n');
	va_end(vp);
}

स्थिर शून्य remove_test_directory(शून्य)
{
	सचे (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

स्थिर ध्रुव वर्ण *get_mtime_path(ध्रुव वर्ण *path)
{
	स्थिर संरचना strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, path);
	पटिनिवत्त sb.buf;
}

स्थिर शून्य xmkdir(ध्रुव वर्ण *path)
{
	path = get_mtime_path(path);
	सचे (mkdir(path, 0700))
		die_errno(_("failed to create directory %s"), path);
}

स्थिर पूर्णाङ्क xstat_mtime_dir(संरचना stat *st)
{
	सचे (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क create_file(ध्रुव वर्ण *path)
{
	पूर्णाङ्क fd;
	path = get_mtime_path(path);
	fd = xopen(path, O_CREAT | O_RDWR, 0644);
	पटिनिवत्त fd;
}

स्थिर शून्य xunlink(ध्रुव वर्ण *path)
{
	path = get_mtime_path(path);
	सचे (unlink(path))
		die_errno(_("failed to delete file %s"), path);
}

स्थिर शून्य xrmdir(ध्रुव वर्ण *path)
{
	path = get_mtime_path(path);
	सचे (rmdir(path))
		die_errno(_("failed to delete directory %s"), path);
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
	संरचना stat_data base;
	पूर्णाङ्क fd, ret = 0;
	वर्ण *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	सचे (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	मुक्त(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	सचे (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		गच्छ done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	सचे (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		गच्छ done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	सचे (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		गच्छ done;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	सचे (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		गच्छ done;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	सचे (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		गच्छ done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	सचे (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		गच्छ done;
	}

	सचे (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

done:
	strbuf_release(&mtime_dir);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क mark_ce_flags(ध्रुव वर्ण *path, पूर्णाङ्क flag, पूर्णाङ्क mark)
{
	पूर्णाङ्क namelen = strlen(path);
	पूर्णाङ्क pos = index_name_pos(the_repository->index, path, namelen);
	सचे (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->index, the_repository->index->cache[pos]);
		सचे (mark)
			the_repository->index->cache[pos]->ce_flags |= flag;
		अञ्ञथा
			the_repository->index->cache[pos]->ce_flags &= ~flag;
		the_repository->index->cache[pos]->ce_flags |= CE_UPDATE_IN_BASE;
		cache_वृक्षः_invalidate_path(the_repository->index, path);
		the_repository->index->cache_changed |= CE_ENTRY_CHANGED;
		पटिनिवत्त 0;
	}
	पटिनिवत्त -1;
}

स्थिर पूर्णाङ्क remove_one_path(ध्रुव वर्ण *path)
{
	सचे (!allow_remove)
		पटिनिवत्त error("%s: does not exist and --remove not passed", path);
	सचे (remove_file_from_index(the_repository->index, path))
		पटिनिवत्त error("%s: cannot remove from the index", path);
	पटिनिवत्त 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
स्थिर पूर्णाङ्क process_lstat_error(ध्रुव वर्ण *path, पूर्णाङ्क err)
{
	सचे (is_missing_file_error(err))
		पटिनिवत्त remove_one_path(path);
	पटिनिवत्त error("lstat(\"%s\"): %s", path, strerror(err));
}

स्थिर पूर्णाङ्क add_one_path(ध्रुव संरचना cache_entry *old, ध्रुव वर्ण *path, पूर्णाङ्क len, संरचना stat *st)
{
	पूर्णाङ्क option;
	संरचना cache_entry *ce;

	/* Was the old index entry already up-to-date? */
	सचे (old && !ce_stage(old) && !ie_match_stat(the_repository->index, old, st, 0))
		पटिनिवत्त 0;

	ce = make_empty_cache_entry(the_repository->index, len);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->index, ce, st);
	ce->ce_mode = ce_mode_from_stat(the_repository, old, st->st_mode);

	सचे (index_path(the_repository->index, &ce->oid, path, st,
		       info_only ? 0 : INDEX_WRITE_OBJECT)) {
		discard_cache_entry(ce);
		पटिनिवत्त -1;
	}
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	सचे (add_index_entry(the_repository->index, ce, option)) {
		discard_cache_entry(ce);
		पटिनिवत्त error("%s: cannot add to the index - missing --add option?", path);
	}
	पटिनिवत्त 0;
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
स्थिर पूर्णाङ्क process_directory(ध्रुव वर्ण *path, पूर्णाङ्क len, संरचना stat *st)
{
	संरचना object_id oid;
	पूर्णाङ्क pos = index_name_pos(the_repository->index, path, len);

	/* Exact match: file or existing gitlink */
	सचे (pos >= 0) {
		ध्रुव संरचना cache_entry *ce = the_repository->index->cache[pos];
		सचे (S_ISGITLINK(ce->ce_mode)) {

			/* Do nothing to the index if there is no HEAD! */
			सचे (repo_resolve_gitlink_ref(the_repository, path,
						     "HEAD", &oid) < 0)
				पटिनिवत्त 0;

			पटिनिवत्त add_one_path(ce, path, len, st);
		}
		/* Should this be an unconditional error? */
		पटिनिवत्त remove_one_path(path);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	याव (pos < the_repository->index->cache_nr) {
		ध्रुव संरचना cache_entry *ce = the_repository->index->cache[pos++];

		सचे (strncmp(ce->name, path, len))
			भङ्ग;
		सचे (ce->name[len] > '/')
			भङ्ग;
		सचे (ce->name[len] < '/')
			अनुवर्त;

		/* Subdirectory match - error out */
		पटिनिवत्त error("%s: is a directory - add individual files instead", path);
	}

	/* No match - should we add it as a gitlink? */
	सचे (!repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid))
		पटिनिवत्त add_one_path(रिक्त, path, len, st);

	/* Error out. */
	पटिनिवत्त error("%s: is a directory - add files inside instead", path);
}

स्थिर पूर्णाङ्क process_path(ध्रुव वर्ण *path, संरचना stat *st, पूर्णाङ्क stat_errno)
{
	पूर्णाङ्क pos, len;
	ध्रुव संरचना cache_entry *ce;

	len = strlen(path);
	सचे (has_symlink_leading_path(path, len))
		पटिनिवत्त error("'%s' is beyond a symbolic link", path);

	pos = index_name_pos(the_repository->index, path, len);
	ce = pos < 0 ? रिक्त : the_repository->index->cache[pos];
	सचे (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		सचे (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_index(the_repository->index, path))
			पटिनिवत्त error("%s: cannot remove from the index", path);
		पटिनिवत्त 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	सचे (stat_errno)
		पटिनिवत्त process_lstat_error(path, stat_errno);

	सचे (S_ISDIR(st->st_mode))
		पटिनिवत्त process_directory(path, len, st);

	पटिनिवत्त add_one_path(ce, path, len, st);
}

स्थिर पूर्णाङ्क add_cacheinfo(unsigned पूर्णाङ्क mode, ध्रुव संरचना object_id *oid,
			 ध्रुव वर्ण *path, पूर्णाङ्क stage)
{
	पूर्णाङ्क len, option;
	संरचना cache_entry *ce;

	सचे (!verify_path(path, mode))
		पटिनिवत्त error("Invalid path '%s'", path);

	len = strlen(path);
	ce = make_empty_cache_entry(the_repository->index, len);

	oidcpy(&ce->oid, oid);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_mode = create_ce_mode(mode);
	सचे (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	सचे (add_index_entry(the_repository->index, ce, option))
		पटिनिवत्त error("%s: cannot add to the index - missing --add option?",
			     path);
	report("add '%s'", path);
	पटिनिवत्त 0;
}

स्थिर शून्य chmod_path(वर्ण flip, ध्रुव वर्ण *path)
{
	पूर्णाङ्क pos;
	संरचना cache_entry *ce;

	pos = index_name_pos(the_repository->index, path, strlen(path));
	सचे (pos < 0)
		गच्छ fail;
	ce = the_repository->index->cache[pos];
	सचे (chmod_index_entry(the_repository->index, ce, flip) < 0)
		गच्छ fail;

	report("chmod %cx '%s'", flip, path);
	पटिनिवत्त;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, path);
}

स्थिर शून्य update_one(ध्रुव वर्ण *path)
{
	पूर्णाङ्क stat_errno = 0;
	संरचना stat st;

	सचे (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_mode = 0;
	अञ्ञथा सचे (lstat(path, &st) < 0) {
		st.st_mode = 0;
		stat_errno = errno;
	} /* else stat is valid */

	सचे (!verify_path(path, st.st_mode)) {
		fprintf(stderr, "Ignoring path %s\n", path);
		पटिनिवत्त;
	}
	सचे (mark_valid_only) {
		सचे (mark_ce_flags(path, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		पटिनिवत्त;
	}
	सचे (mark_skip_worktree_only) {
		सचे (mark_ce_flags(path, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		पटिनिवत्त;
	}
	सचे (mark_fsmonitor_only) {
		सचे (mark_ce_flags(path, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		पटिनिवत्त;
	}

	सचे (force_remove) {
		सचे (remove_file_from_index(the_repository->index, path))
			die("git update-index: unable to remove %s", path);
		report("remove '%s'", path);
		पटिनिवत्त;
	}
	सचे (process_path(path, &st, stat_errno))
		die("Unable to process path %s", path);
	report("add '%s'", path);
}

स्थिर शून्य read_index_info(पूर्णाङ्क nul_term_पङ्क्तिः)
{
	ध्रुव पूर्णाङ्क hexsz = the_hash_algo->hexsz;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_पङ्क्तिः ? strbuf_getline_nul : strbuf_getline_lf;
	याव (getline_fn(&buf, stdin) != EOF) {
		वर्ण *ptr, *tab;
		वर्ण *path_name;
		संरचना object_id oid;
		unsigned पूर्णाङ्क mode;
		unsigned long ul;
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
		सचे (ptr == buf.buf || *ptr != ' '
		    || errno || (unsigned पूर्णाङ्क) ul != ul)
			गच्छ bad_पङ्क्तिः;
		mode = ul;

		tab = strchr(ptr, '\t');
		सचे (!tab || tab - ptr < hexsz + 1)
			गच्छ bad_पङ्क्तिः;

		सचे (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		अञ्ञथा {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		सचे (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			गच्छ bad_पङ्क्तिः;

		path_name = ptr;
		सचे (!nul_term_पङ्क्तिः && path_name[0] == '"') {
			strbuf_reset(&uq);
			सचे (unquote_c_style(&uq, path_name, रिक्त)) {
				die("git update-index: bad quoting of path name");
			}
			path_name = uq.buf;
		}

		सचे (!verify_path(path_name, mode)) {
			fprintf(stderr, "Ignoring path %s\n", path_name);
			अनुवर्त;
		}

		सचे (!mode) {
			/* mode == 0 means there is no such path -- remove */
			सचे (remove_file_from_index(the_repository->index, path_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		अञ्ञथा {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			सचे (add_cacheinfo(mode, &oid, path_name, stage))
				die("git update-index: unable to update %s",
				    path_name);
		}
		अनुवर्त;

	bad_पङ्क्तिः:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

स्थिर ध्रुव वर्ण * ध्रुव update_index_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	रिक्त
};

स्थिर संरचना cache_entry *read_one_ent(ध्रुव वर्ण *which,
					संरचना object_id *ent, ध्रुव वर्ण *path,
					पूर्णाङ्क namelen, पूर्णाङ्क stage)
{
	unsigned short mode;
	संरचना object_id oid;
	संरचना cache_entry *ce;

	सचे (get_वृक्षः_entry(the_repository, ent, path, &oid, &mode)) {
		सचे (which)
			error("%s: not in %s branch.", path, which);
		पटिनिवत्त रिक्त;
	}
	सचे (!the_repository->index->sparse_index && mode == S_IFDIR) {
		सचे (which)
			error("%s: not a blob in %s branch.", path, which);
		पटिनिवत्त रिक्त;
	}
	ce = make_empty_cache_entry(the_repository->index, namelen);

	oidcpy(&ce->oid, &oid);
	memcpy(ce->name, path, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_mode = create_ce_mode(mode);
	पटिनिवत्त ce;
}

स्थिर पूर्णाङ्क unresolve_one(ध्रुव वर्ण *path)
{
	संरचना string_list_item *item;
	पूर्णाङ्क res = 0;

	सचे (!the_repository->index->resolve_undo)
		पटिनिवत्त res;
	item = string_list_lookup(the_repository->index->resolve_undo, path);
	सचे (!item)
		पटिनिवत्त res; /* no resolve-undo record for the path */
	res = unmerge_index_entry(the_repository->index, path, item->util, 0);
	FREE_AND_NULL(item->util);
	पटिनिवत्त res;
}

स्थिर पूर्णाङ्क do_unresolve(पूर्णाङ्क ac, ध्रुव वर्ण **av,
			ध्रुव वर्ण *prefix, पूर्णाङ्क prefix_length)
{
	पूर्णाङ्क i;
	पूर्णाङ्क err = 0;

	अत्थाय (i = 1; i < ac; i++) {
		ध्रुव वर्ण *arg = av[i];
		वर्ण *p = prefix_path(the_repository, prefix, prefix_length, arg);
		err |= unresolve_one(p);
		मुक्त(p);
	}
	पटिनिवत्त err;
}

स्थिर पूर्णाङ्क do_reupdate(ध्रुव वर्ण **paths,
		       ध्रुव वर्ण *prefix)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	पूर्णाङ्क pos;
	पूर्णाङ्क has_head = 1;
	संरचना pathspec pathspec;
	संरचना object_id head_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, paths);

	सचे (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &head_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_head = 0;
 redo:
	अत्थाय (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ध्रुव संरचना cache_entry *ce = the_repository->index->cache[pos];
		संरचना cache_entry *old = रिक्त;
		पूर्णाङ्क save_nr;
		वर्ण *path;

		सचे (ce_stage(ce) || !ce_path_match(the_repository->index, ce, &pathspec, रिक्त))
			अनुवर्त;
		सचे (has_head)
			old = read_one_ent(रिक्त, &head_oid,
					   ce->name, ce_namelen(ce), 0);
		सचे (old && ce->ce_mode == old->ce_mode &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_entry(old);
			अनुवर्त; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		सचे (S_ISSPARSEDIR(ce->ce_mode)) {
			ensure_full_index(the_repository->index);
			गच्छ redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->index->cache_nr;
		path = xstrdup(ce->name);
		update_one(path);
		मुक्त(path);
		discard_cache_entry(old);
		सचे (save_nr != the_repository->index->cache_nr)
			गच्छ redo;
	}
	clear_pathspec(&pathspec);
	पटिनिवत्त 0;
}

संरचना refresh_params {
	unsigned पूर्णाङ्क flags;
	पूर्णाङ्क *has_errors;
};

स्थिर पूर्णाङ्क refresh(संरचना refresh_params *o, unsigned पूर्णाङ्क flag)
{
	setup_work_वृक्षः(the_repository);
	repo_read_index(the_repository);
	*o->has_errors |= refresh_index(the_repository->index, o->flags | flag, रिक्त,
					रिक्त, रिक्त);
	सचे (has_racy_timestamp(the_repository->index)) {
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
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क refresh_callback(ध्रुव संरचना option *opt,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	पटिनिवत्त refresh(opt->मानम्, 0);
}

स्थिर पूर्णाङ्क really_refresh_callback(ध्रुव संरचना option *opt,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	पटिनिवत्त refresh(opt->मानम्, REFRESH_REALLY);
}

स्थिर पूर्णाङ्क chmod_callback(ध्रुव संरचना option *opt,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	वर्ण *flip = opt->मानम्;
	BUG_ON_OPT_NEG(unset);
	सचे ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		पटिनिवत्त error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क resolve_undo_clear_callback(ध्रुव संरचना option *opt UNUSED,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_index(the_repository->index);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क parse_new_style_cacheinfo(ध्रुव वर्ण *arg,
				     unsigned पूर्णाङ्क *mode,
				     संरचना object_id *oid,
				     ध्रुव वर्ण **path)
{
	unsigned long ul;
	वर्ण *endp;
	ध्रुव वर्ण *p;

	सचे (!arg)
		पटिनिवत्त -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	सचे (errno || endp == arg || *endp != ',' || (unsigned पूर्णाङ्क) ul != ul)
		पटिनिवत्त -1; /* not a new-style cacheinfo */
	*mode = ul;
	endp++;
	सचे (parse_oid_hex(endp, oid, &p) || *p != ',')
		पटिनिवत्त -1;
	*path = p + 1;
	पटिनिवत्त 0;
}

स्थिर गणना parse_opt_result cacheinfo_callback(
	संरचना parse_opt_ctx_t *ctx, ध्रुव संरचना option *opt UNUSED,
	ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना object_id oid;
	unsigned पूर्णाङ्क mode;
	ध्रुव वर्ण *path;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	सचे (!parse_new_style_cacheinfo(ctx->argv[1], &mode, &oid, &path)) {
		सचे (add_cacheinfo(mode, &oid, path, 0))
			die("git update-index: --cacheinfo cannot add %s", path);
		ctx->argv++;
		ctx->argc--;
		पटिनिवत्त 0;
	}
	सचे (ctx->argc <= 3)
		पटिनिवत्त error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	सचे (strtoul_ui(*++ctx->argv, 8, &mode) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    add_cacheinfo(mode, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	पटिनिवत्त 0;
}

स्थिर गणना parse_opt_result stdin_cacheinfo_callback(
	संरचना parse_opt_ctx_t *ctx, ध्रुव संरचना option *opt,
	ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *nul_term_पङ्क्तिः = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	सचे (ctx->argc != 1)
		पटिनिवत्त error("option '%s' must be the last argument", opt->long_name);
	allow_add = allow_replace = allow_remove = 1;
	read_index_info(*nul_term_पङ्क्तिः);
	पटिनिवत्त 0;
}

स्थिर गणना parse_opt_result stdin_callback(
	संरचना parse_opt_ctx_t *ctx, ध्रुव संरचना option *opt,
	ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *read_from_stdin = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	सचे (ctx->argc != 1)
		पटिनिवत्त error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	पटिनिवत्त 0;
}

स्थिर गणना parse_opt_result unresolve_callback(
	संरचना parse_opt_ctx_t *ctx, ध्रुव संरचना option *opt,
	ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *has_errors = opt->मानम्;
	ध्रुव वर्ण *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				prefix, prefix ? strlen(prefix) : 0);
	सचे (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	पटिनिवत्त 0;
}

स्थिर गणना parse_opt_result reupdate_callback(
	संरचना parse_opt_ctx_t *ctx, ध्रुव संरचना option *opt,
	ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *has_errors = opt->मानम्;
	ध्रुव वर्ण *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	setup_work_वृक्षः(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, prefix);
	सचे (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	पटिनिवत्त 0;
}

पूर्णाङ्क cmd_update_index(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *prefix,
		     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क newfd, entries, has_errors = 0, nul_term_पङ्क्तिः = 0;
	गणना uc_mode untracked_cache = UC_UNSPECIFIED;
	पूर्णाङ्क read_from_stdin = 0;
	पूर्णाङ्क prefix_length = prefix ? strlen(prefix) : 0;
	पूर्णाङ्क preferred_index_format = 0;
	वर्ण set_executable_bit = 0;
	संरचना refresh_params refresh_args = {0, &has_errors};
	पूर्णाङ्क lock_error = 0;
	पूर्णाङ्क split_index = -1;
	पूर्णाङ्क force_write = 0;
	पूर्णाङ्क fsmonitor = -1;
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	पूर्णाङ्क parseopt_state = PARSE_OPT_UNKNOWN;
	संरचना repository *r = the_repository;
	संरचना odb_transaction *transaction;
	संरचना option options[] = {
		OPT_BIT('q', रिक्त, &refresh_args.flags,
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
		OPT_CALLBACK_F(0, "refresh", &refresh_args, रिक्त,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_args, रिक्त,
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
			.मानम् = &mark_valid_only,
			.precision = परिमाण(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-assume-unchanged",
			.मानम् = &mark_valid_only,
			.precision = परिमाण(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "skip-worktree",
			.मानम् = &mark_skip_worktree_only,
			.precision = परिमाण(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-skip-worktree",
			.मानम् = &mark_skip_worktree_only,
			.precision = परिमाण(mark_skip_worktree_only),
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
		OPT_BOOL('z', रिक्त, &nul_term_पङ्क्तिः,
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
			.मानम् = &nul_term_पङ्क्तिः,
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
		OPT_BIT(0, "ignore-missing", &refresh_args.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_SET_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", रिक्त, रिक्त,
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
			.मानम् = &mark_fsmonitor_only,
			.precision = परिमाण(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-fsmonitor-valid",
			.मानम् = &mark_fsmonitor_only,
			.precision = परिमाण(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_index_usage, options);

	repo_config(the_repository, git_default_config, रिक्त);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_full_index = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_index(the_repository, &lock_file, 0);
	सचे (newfd < 0)
		lock_error = errno;

	entries = repo_read_index(the_repository);
	सचे (entries < 0)
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
	याव (ctx.argc) {
		सचे (parseopt_state != PARSE_OPT_DONE)
			parseopt_state = parse_options_step(&ctx, options,
							    update_index_usage);
		सचे (!ctx.argc)
			भङ्ग;
		विकल्प (parseopt_state) {
		पक्ष PARSE_OPT_HELP:
			exit(0);
		पक्ष PARSE_OPT_HELP_ERROR:
		पक्ष PARSE_OPT_ERROR:
			exit(129);
		पक्ष PARSE_OPT_COMPLETE:
			exit(0);
		पक्ष PARSE_OPT_NON_OPTION:
		पक्ष PARSE_OPT_DONE:
		{
			ध्रुव वर्ण *path = ctx.argv[0];
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
			सचे (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = रिक्त;
			}

			setup_work_वृक्षः(the_repository);
			p = prefix_path(the_repository, prefix, prefix_length, path);
			update_one(p);
			सचे (set_executable_bit)
				chmod_path(set_executable_bit, p);
			मुक्त(p);
			ctx.argc--;
			ctx.argv++;
			भङ्ग;
		}
		पक्ष PARSE_OPT_UNKNOWN:
			सचे (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			अञ्ञथा
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_index_usage, options);
		}
	}
	argc = parse_options_end(&ctx);

	getline_fn = nul_term_पङ्क्तिः ? strbuf_getline_nul : strbuf_getline_lf;
	सचे (preferred_index_format) {
		सचे (preferred_index_format < 0) {
			printf(_("%d\n"), the_repository->index->version);
		} अञ्ञथा सचे (preferred_index_format < INDEX_FORMAT_LB ||
			   INDEX_FORMAT_UB < preferred_index_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_index_format,
			    INDEX_FORMAT_LB, INDEX_FORMAT_UB);
		} अञ्ञथा {
			सचे (the_repository->index->version != preferred_index_format)
				the_repository->index->cache_changed |= SOMETHING_CHANGED;
			report(_("index-version: was %d, set to %d"),
			       the_repository->index->version, preferred_index_format);
			the_repository->index->version = preferred_index_format;
		}
	}

	सचे (read_from_stdin) {
		संरचना strbuf buf = STRBUF_INIT;
		संरचना strbuf unquoted = STRBUF_INIT;

		setup_work_वृक्षः(the_repository);
		याव (getline_fn(&buf, stdin) != EOF) {
			वर्ण *p;
			सचे (!nul_term_पङ्क्तिः && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				सचे (unquote_c_style(&unquoted, buf.buf, रिक्त))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(the_repository, prefix, prefix_length, buf.buf);
			update_one(p);
			सचे (set_executable_bit)
				chmod_path(set_executable_bit, p);
			मुक्त(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	सचे (split_index > 0) {
		सचे (repo_config_get_split_index(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		सचे (the_repository->index->split_index)
			the_repository->index->cache_changed |= SPLIT_INDEX_ORDERED;
		अञ्ञथा
			add_split_index(the_repository->index);
	} अञ्ञथा सचे (!split_index) {
		सचे (repo_config_get_split_index(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_split_index(the_repository->index);
	}

	prepare_repo_settings(r);
	विकल्प (untracked_cache) {
	पक्ष UC_UNSPECIFIED:
		भङ्ग;
	पक्ष UC_DISABLE:
		सचे (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->index);
		report(_("Untracked cache disabled"));
		भङ्ग;
	पक्ष UC_TEST:
		setup_work_वृक्षः(the_repository);
		पटिनिवत्त !test_if_untracked_cache_is_supported();
	पक्ष UC_ENABLE:
	पक्ष UC_FORCE:
		सचे (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		add_untracked_cache(the_repository->index);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_वृक्षः(the_repository));
		भङ्ग;
	स्वभाव:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	सचे (fsmonitor > 0) {
		गणना fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
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
		सचे (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_सन्देशः(r, reason));

		सचे (fsm_mode == FSMONITOR_MODE_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		add_fsmonitor(the_repository->index);
		report(_("fsmonitor enabled"));
	} अञ्ञथा सचे (!fsmonitor) {
		गणना fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		सचे (fsm_mode > FSMONITOR_MODE_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->index);
		report(_("fsmonitor disabled"));
	}

	सचे (the_repository->index->cache_changed || force_write) {
		सचे (newfd < 0) {
			सचे (refresh_args.flags & REFRESH_QUIET)
				exit(128);
			unable_to_lock_die(repo_get_index_file(the_repository), lock_error);
		}
		सचे (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die("Unable to write new index file");
	}

	rollback_lock_file(&lock_file);

	पटिनिवत्त has_errors ? 1 : 0;
}
