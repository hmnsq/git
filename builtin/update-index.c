/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "lockfile.h"
#شامل "quote.h"
#شامل "cache-tree.h"
#شامل "tree-walk.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "odb/transaction.h"
#شامل "refs.h"
#شامل "resolve-undo.h"
#شامل "parse-options.h"
#شامل "pathspec.h"
#شامل "dir.h"
#شامل "read-cache.h"
#شامل "setup.h"
#شامل "sparse-index.h"
#شامل "split-index.h"
#شامل "symlinks.h"
#شامل "fsmonitor.h"
#شامل "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
ایستا صحیح allow_add;
ایستا صحیح allow_remove;
ایستا صحیح allow_replace;
ایستا صحیح info_only;
ایستا صحیح force_remove;
ایستا صحیح verbose;
ایستا صحیح mark_valid_only;
ایستا صحیح mark_skip_worktree_only;
ایستا صحیح mark_fsmonitor_only;
ایستا صحیح ignore_skip_worktree_entries;
#define MARK_FLAG 1
#define UNMARK_FLAG 2
ایستا ساختار strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
شمارش uc_mode {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (printf, 1, 2)))
ایستا تهی report(ثابت نویسه *fmt, ...)
{
	va_list vp;

	اگر (!verbose)
		بازگشت;

	va_start(vp, fmt);
	vprintf(fmt, vp);
	putchar('\n');
	va_end(vp);
}

ایستا تهی remove_test_directory(تهی)
{
	اگر (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

ایستا ثابت نویسه *get_mtime_path(ثابت نویسه *path)
{
	ایستا ساختار strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, path);
	بازگشت sb.buf;
}

ایستا تهی xmkdir(ثابت نویسه *path)
{
	path = get_mtime_path(path);
	اگر (mkdir(path, 0700))
		die_errno(_("failed to create directory %s"), path);
}

ایستا صحیح xstat_mtime_dir(ساختار stat *st)
{
	اگر (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	بازگشت 0;
}

ایستا صحیح create_file(ثابت نویسه *path)
{
	صحیح fd;
	path = get_mtime_path(path);
	fd = xopen(path, O_CREAT | O_RDWR, 0644);
	بازگشت fd;
}

ایستا تهی xunlink(ثابت نویسه *path)
{
	path = get_mtime_path(path);
	اگر (unlink(path))
		die_errno(_("failed to delete file %s"), path);
}

ایستا تهی xrmdir(ثابت نویسه *path)
{
	path = get_mtime_path(path);
	اگر (rmdir(path))
		die_errno(_("failed to delete directory %s"), path);
}

ایستا تهی avoid_racy(تهی)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

ایستا صحیح test_if_untracked_cache_is_supported(تهی)
{
	ساختار stat st;
	ساختار stat_data base;
	صحیح fd, ret = 0;
	نویسه *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	اگر (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	آزاد(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	اگر (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		برو done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	اگر (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		برو done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	اگر (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		برو done;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	اگر (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		برو done;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	اگر (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		برو done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	اگر (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		برو done;
	}

	اگر (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

done:
	strbuf_release(&mtime_dir);
	بازگشت ret;
}

ایستا صحیح mark_ce_flags(ثابت نویسه *path, صحیح flag, صحیح mark)
{
	صحیح namelen = strlen(path);
	صحیح pos = index_name_pos(the_repository->index, path, namelen);
	اگر (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->index, the_repository->index->cache[pos]);
		اگر (mark)
			the_repository->index->cache[pos]->ce_flags |= flag;
		وگرنه
			the_repository->index->cache[pos]->ce_flags &= ~flag;
		the_repository->index->cache[pos]->ce_flags |= CE_UPDATE_IN_BASE;
		cache_درخت_invalidate_path(the_repository->index, path);
		the_repository->index->cache_changed |= CE_ENTRY_CHANGED;
		بازگشت 0;
	}
	بازگشت -1;
}

ایستا صحیح remove_one_path(ثابت نویسه *path)
{
	اگر (!allow_remove)
		بازگشت error("%s: does not exist and --remove not passed", path);
	اگر (remove_file_from_index(the_repository->index, path))
		بازگشت error("%s: cannot remove from the index", path);
	بازگشت 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
ایستا صحیح process_lstat_error(ثابت نویسه *path, صحیح err)
{
	اگر (is_missing_file_error(err))
		بازگشت remove_one_path(path);
	بازگشت error("lstat(\"%s\"): %s", path, strerror(err));
}

ایستا صحیح add_one_path(ثابت ساختار cache_entry *old, ثابت نویسه *path, صحیح len, ساختار stat *st)
{
	صحیح option;
	ساختار cache_entry *ce;

	/* Was the old index entry already up-to-date? */
	اگر (old && !ce_stage(old) && !ie_match_stat(the_repository->index, old, st, 0))
		بازگشت 0;

	ce = make_empty_cache_entry(the_repository->index, len);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->index, ce, st);
	ce->ce_mode = ce_mode_from_stat(the_repository, old, st->st_mode);

	اگر (index_path(the_repository->index, &ce->oid, path, st,
		       info_only ? 0 : INDEX_WRITE_OBJECT)) {
		discard_cache_entry(ce);
		بازگشت -1;
	}
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	اگر (add_index_entry(the_repository->index, ce, option)) {
		discard_cache_entry(ce);
		بازگشت error("%s: cannot add to the index - missing --add option?", path);
	}
	بازگشت 0;
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
ایستا صحیح process_directory(ثابت نویسه *path, صحیح len, ساختار stat *st)
{
	ساختار object_id oid;
	صحیح pos = index_name_pos(the_repository->index, path, len);

	/* Exact match: file or existing gitlink */
	اگر (pos >= 0) {
		ثابت ساختار cache_entry *ce = the_repository->index->cache[pos];
		اگر (S_ISGITLINK(ce->ce_mode)) {

			/* Do nothing to the index if there is no HEAD! */
			اگر (repo_resolve_gitlink_ref(the_repository, path,
						     "HEAD", &oid) < 0)
				بازگشت 0;

			بازگشت add_one_path(ce, path, len, st);
		}
		/* Should this be an unconditional error? */
		بازگشت remove_one_path(path);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	وقتی (pos < the_repository->index->cache_nr) {
		ثابت ساختار cache_entry *ce = the_repository->index->cache[pos++];

		اگر (strncmp(ce->name, path, len))
			بشکن;
		اگر (ce->name[len] > '/')
			بشکن;
		اگر (ce->name[len] < '/')
			ادامه;

		/* Subdirectory match - error out */
		بازگشت error("%s: is a directory - add individual files instead", path);
	}

	/* No match - should we add it as a gitlink? */
	اگر (!repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid))
		بازگشت add_one_path(پوچ, path, len, st);

	/* Error out. */
	بازگشت error("%s: is a directory - add files inside instead", path);
}

ایستا صحیح process_path(ثابت نویسه *path, ساختار stat *st, صحیح stat_errno)
{
	صحیح pos, len;
	ثابت ساختار cache_entry *ce;

	len = strlen(path);
	اگر (has_symlink_leading_path(path, len))
		بازگشت error("'%s' is beyond a symbolic link", path);

	pos = index_name_pos(the_repository->index, path, len);
	ce = pos < 0 ? پوچ : the_repository->index->cache[pos];
	اگر (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		اگر (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_index(the_repository->index, path))
			بازگشت error("%s: cannot remove from the index", path);
		بازگشت 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	اگر (stat_errno)
		بازگشت process_lstat_error(path, stat_errno);

	اگر (S_ISDIR(st->st_mode))
		بازگشت process_directory(path, len, st);

	بازگشت add_one_path(ce, path, len, st);
}

ایستا صحیح add_cacheinfo(unsigned صحیح mode, ثابت ساختار object_id *oid,
			 ثابت نویسه *path, صحیح stage)
{
	صحیح len, option;
	ساختار cache_entry *ce;

	اگر (!verify_path(path, mode))
		بازگشت error("Invalid path '%s'", path);

	len = strlen(path);
	ce = make_empty_cache_entry(the_repository->index, len);

	oidcpy(&ce->oid, oid);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_mode = create_ce_mode(mode);
	اگر (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	اگر (add_index_entry(the_repository->index, ce, option))
		بازگشت error("%s: cannot add to the index - missing --add option?",
			     path);
	report("add '%s'", path);
	بازگشت 0;
}

ایستا تهی chmod_path(نویسه flip, ثابت نویسه *path)
{
	صحیح pos;
	ساختار cache_entry *ce;

	pos = index_name_pos(the_repository->index, path, strlen(path));
	اگر (pos < 0)
		برو fail;
	ce = the_repository->index->cache[pos];
	اگر (chmod_index_entry(the_repository->index, ce, flip) < 0)
		برو fail;

	report("chmod %cx '%s'", flip, path);
	بازگشت;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, path);
}

ایستا تهی update_one(ثابت نویسه *path)
{
	صحیح stat_errno = 0;
	ساختار stat st;

	اگر (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_mode = 0;
	وگرنه اگر (lstat(path, &st) < 0) {
		st.st_mode = 0;
		stat_errno = errno;
	} /* else stat is valid */

	اگر (!verify_path(path, st.st_mode)) {
		fprintf(stderr, "Ignoring path %s\n", path);
		بازگشت;
	}
	اگر (mark_valid_only) {
		اگر (mark_ce_flags(path, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		بازگشت;
	}
	اگر (mark_skip_worktree_only) {
		اگر (mark_ce_flags(path, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		بازگشت;
	}
	اگر (mark_fsmonitor_only) {
		اگر (mark_ce_flags(path, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		بازگشت;
	}

	اگر (force_remove) {
		اگر (remove_file_from_index(the_repository->index, path))
			die("git update-index: unable to remove %s", path);
		report("remove '%s'", path);
		بازگشت;
	}
	اگر (process_path(path, &st, stat_errno))
		die("Unable to process path %s", path);
	report("add '%s'", path);
}

ایستا تهی read_index_info(صحیح nul_term_سطر)
{
	ثابت صحیح hexsz = the_hash_algo->hexsz;
	ساختار strbuf buf = STRBUF_INIT;
	ساختار strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	وقتی (getline_fn(&buf, stdin) != EOF) {
		نویسه *ptr, *tab;
		نویسه *path_name;
		ساختار object_id oid;
		unsigned صحیح mode;
		unsigned long ul;
		صحیح stage;

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
		اگر (ptr == buf.buf || *ptr != ' '
		    || errno || (unsigned صحیح) ul != ul)
			برو bad_سطر;
		mode = ul;

		tab = strchr(ptr, '\t');
		اگر (!tab || tab - ptr < hexsz + 1)
			برو bad_سطر;

		اگر (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		وگرنه {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		اگر (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			برو bad_سطر;

		path_name = ptr;
		اگر (!nul_term_سطر && path_name[0] == '"') {
			strbuf_reset(&uq);
			اگر (unquote_c_style(&uq, path_name, پوچ)) {
				die("git update-index: bad quoting of path name");
			}
			path_name = uq.buf;
		}

		اگر (!verify_path(path_name, mode)) {
			fprintf(stderr, "Ignoring path %s\n", path_name);
			ادامه;
		}

		اگر (!mode) {
			/* mode == 0 means there is no such path -- remove */
			اگر (remove_file_from_index(the_repository->index, path_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		وگرنه {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			اگر (add_cacheinfo(mode, &oid, path_name, stage))
				die("git update-index: unable to update %s",
				    path_name);
		}
		ادامه;

	bad_سطر:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

ایستا ثابت نویسه * ثابت update_index_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	پوچ
};

ایستا ساختار cache_entry *read_one_ent(ثابت نویسه *which,
					ساختار object_id *ent, ثابت نویسه *path,
					صحیح namelen, صحیح stage)
{
	unsigned short mode;
	ساختار object_id oid;
	ساختار cache_entry *ce;

	اگر (get_درخت_entry(the_repository, ent, path, &oid, &mode)) {
		اگر (which)
			error("%s: not in %s branch.", path, which);
		بازگشت پوچ;
	}
	اگر (!the_repository->index->sparse_index && mode == S_IFDIR) {
		اگر (which)
			error("%s: not a blob in %s branch.", path, which);
		بازگشت پوچ;
	}
	ce = make_empty_cache_entry(the_repository->index, namelen);

	oidcpy(&ce->oid, &oid);
	memcpy(ce->name, path, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_mode = create_ce_mode(mode);
	بازگشت ce;
}

ایستا صحیح unresolve_one(ثابت نویسه *path)
{
	ساختار string_list_item *item;
	صحیح res = 0;

	اگر (!the_repository->index->resolve_undo)
		بازگشت res;
	item = string_list_lookup(the_repository->index->resolve_undo, path);
	اگر (!item)
		بازگشت res; /* no resolve-undo record for the path */
	res = unmerge_index_entry(the_repository->index, path, item->util, 0);
	FREE_AND_NULL(item->util);
	بازگشت res;
}

ایستا صحیح do_unresolve(صحیح ac, ثابت نویسه **av,
			ثابت نویسه *prefix, صحیح prefix_length)
{
	صحیح i;
	صحیح err = 0;

	برای (i = 1; i < ac; i++) {
		ثابت نویسه *arg = av[i];
		نویسه *p = prefix_path(the_repository, prefix, prefix_length, arg);
		err |= unresolve_one(p);
		آزاد(p);
	}
	بازگشت err;
}

ایستا صحیح do_reupdate(ثابت نویسه **paths,
		       ثابت نویسه *prefix)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	صحیح pos;
	صحیح has_head = 1;
	ساختار pathspec pathspec;
	ساختار object_id head_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, paths);

	اگر (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &head_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_head = 0;
 redo:
	برای (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ثابت ساختار cache_entry *ce = the_repository->index->cache[pos];
		ساختار cache_entry *old = پوچ;
		صحیح save_nr;
		نویسه *path;

		اگر (ce_stage(ce) || !ce_path_match(the_repository->index, ce, &pathspec, پوچ))
			ادامه;
		اگر (has_head)
			old = read_one_ent(پوچ, &head_oid,
					   ce->name, ce_namelen(ce), 0);
		اگر (old && ce->ce_mode == old->ce_mode &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_entry(old);
			ادامه; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		اگر (S_ISSPARSEDIR(ce->ce_mode)) {
			ensure_full_index(the_repository->index);
			برو redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->index->cache_nr;
		path = xstrdup(ce->name);
		update_one(path);
		آزاد(path);
		discard_cache_entry(old);
		اگر (save_nr != the_repository->index->cache_nr)
			برو redo;
	}
	clear_pathspec(&pathspec);
	بازگشت 0;
}

ساختار refresh_params {
	unsigned صحیح flags;
	صحیح *has_errors;
};

ایستا صحیح refresh(ساختار refresh_params *o, unsigned صحیح flag)
{
	setup_work_درخت(the_repository);
	repo_read_index(the_repository);
	*o->has_errors |= refresh_index(the_repository->index, o->flags | flag, پوچ,
					پوچ, پوچ);
	اگر (has_racy_timestamp(the_repository->index)) {
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
	بازگشت 0;
}

ایستا صحیح refresh_callback(ثابت ساختار option *opt,
				ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	بازگشت refresh(opt->مقدار, 0);
}

ایستا صحیح really_refresh_callback(ثابت ساختار option *opt,
				ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	بازگشت refresh(opt->مقدار, REFRESH_REALLY);
}

ایستا صحیح chmod_callback(ثابت ساختار option *opt,
				ثابت نویسه *arg, صحیح unset)
{
	نویسه *flip = opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	اگر ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		بازگشت error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	بازگشت 0;
}

ایستا صحیح resolve_undo_clear_callback(ثابت ساختار option *opt UNUSED,
				ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_index(the_repository->index);
	بازگشت 0;
}

ایستا صحیح parse_new_style_cacheinfo(ثابت نویسه *arg,
				     unsigned صحیح *mode,
				     ساختار object_id *oid,
				     ثابت نویسه **path)
{
	unsigned long ul;
	نویسه *endp;
	ثابت نویسه *p;

	اگر (!arg)
		بازگشت -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	اگر (errno || endp == arg || *endp != ',' || (unsigned صحیح) ul != ul)
		بازگشت -1; /* not a new-style cacheinfo */
	*mode = ul;
	endp++;
	اگر (parse_oid_hex(endp, oid, &p) || *p != ',')
		بازگشت -1;
	*path = p + 1;
	بازگشت 0;
}

ایستا شمارش parse_opt_result cacheinfo_callback(
	ساختار parse_opt_ctx_t *ctx, ثابت ساختار option *opt UNUSED,
	ثابت نویسه *arg, صحیح unset)
{
	ساختار object_id oid;
	unsigned صحیح mode;
	ثابت نویسه *path;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (!parse_new_style_cacheinfo(ctx->argv[1], &mode, &oid, &path)) {
		اگر (add_cacheinfo(mode, &oid, path, 0))
			die("git update-index: --cacheinfo cannot add %s", path);
		ctx->argv++;
		ctx->argc--;
		بازگشت 0;
	}
	اگر (ctx->argc <= 3)
		بازگشت error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	اگر (strtoul_ui(*++ctx->argv, 8, &mode) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    add_cacheinfo(mode, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	بازگشت 0;
}

ایستا شمارش parse_opt_result stdin_cacheinfo_callback(
	ساختار parse_opt_ctx_t *ctx, ثابت ساختار option *opt,
	ثابت نویسه *arg, صحیح unset)
{
	صحیح *nul_term_سطر = opt->مقدار;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (ctx->argc != 1)
		بازگشت error("option '%s' must be the last argument", opt->long_name);
	allow_add = allow_replace = allow_remove = 1;
	read_index_info(*nul_term_سطر);
	بازگشت 0;
}

ایستا شمارش parse_opt_result stdin_callback(
	ساختار parse_opt_ctx_t *ctx, ثابت ساختار option *opt,
	ثابت نویسه *arg, صحیح unset)
{
	صحیح *read_from_stdin = opt->مقدار;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (ctx->argc != 1)
		بازگشت error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	بازگشت 0;
}

ایستا شمارش parse_opt_result unresolve_callback(
	ساختار parse_opt_ctx_t *ctx, ثابت ساختار option *opt,
	ثابت نویسه *arg, صحیح unset)
{
	صحیح *has_errors = opt->مقدار;
	ثابت نویسه *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				prefix, prefix ? strlen(prefix) : 0);
	اگر (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	بازگشت 0;
}

ایستا شمارش parse_opt_result reupdate_callback(
	ساختار parse_opt_ctx_t *ctx, ثابت ساختار option *opt,
	ثابت نویسه *arg, صحیح unset)
{
	صحیح *has_errors = opt->مقدار;
	ثابت نویسه *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	setup_work_درخت(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, prefix);
	اگر (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	بازگشت 0;
}

صحیح cmd_update_index(صحیح argc,
		     ثابت نویسه **argv,
		     ثابت نویسه *prefix,
		     ساختار repository *repo UNUSED)
{
	صحیح newfd, entries, has_errors = 0, nul_term_سطر = 0;
	شمارش uc_mode untracked_cache = UC_UNSPECIFIED;
	صحیح read_from_stdin = 0;
	صحیح prefix_length = prefix ? strlen(prefix) : 0;
	صحیح preferred_index_format = 0;
	نویسه set_executable_bit = 0;
	ساختار refresh_params refresh_args = {0, &has_errors};
	صحیح lock_error = 0;
	صحیح split_index = -1;
	صحیح force_write = 0;
	صحیح fsmonitor = -1;
	ساختار lock_file lock_file = LOCK_INIT;
	ساختار parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	صحیح parseopt_state = PARSE_OPT_UNKNOWN;
	ساختار repository *r = the_repository;
	ساختار odb_transaction *transaction;
	ساختار option options[] = {
		OPT_BIT('q', پوچ, &refresh_args.flags,
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
		OPT_CALLBACK_F(0, "refresh", &refresh_args, پوچ,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_args, پوچ,
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
			.مقدار = &mark_valid_only,
			.precision = اندازه(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-assume-unchanged",
			.مقدار = &mark_valid_only,
			.precision = اندازه(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "skip-worktree",
			.مقدار = &mark_skip_worktree_only,
			.precision = اندازه(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-skip-worktree",
			.مقدار = &mark_skip_worktree_only,
			.precision = اندازه(mark_skip_worktree_only),
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
		OPT_BOOL('z', پوچ, &nul_term_سطر,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.مقدار = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.مقدار = &nul_term_سطر,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.مقدار = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.مقدار = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_BIT(0, "ignore-missing", &refresh_args.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_SET_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", پوچ, پوچ,
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
			.مقدار = &mark_fsmonitor_only,
			.precision = اندازه(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-fsmonitor-valid",
			.مقدار = &mark_fsmonitor_only,
			.precision = اندازه(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_index_usage, options);

	repo_config(the_repository, git_default_config, پوچ);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_full_index = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_index(the_repository, &lock_file, 0);
	اگر (newfd < 0)
		lock_error = errno;

	entries = repo_read_index(the_repository);
	اگر (entries < 0)
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
	وقتی (ctx.argc) {
		اگر (parseopt_state != PARSE_OPT_DONE)
			parseopt_state = parse_options_step(&ctx, options,
							    update_index_usage);
		اگر (!ctx.argc)
			بشکن;
		انتخاب (parseopt_state) {
		حالت PARSE_OPT_HELP:
			exit(0);
		حالت PARSE_OPT_HELP_ERROR:
		حالت PARSE_OPT_ERROR:
			exit(129);
		حالت PARSE_OPT_COMPLETE:
			exit(0);
		حالت PARSE_OPT_NON_OPTION:
		حالت PARSE_OPT_DONE:
		{
			ثابت نویسه *path = ctx.argv[0];
			نویسه *p;

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
			اگر (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = پوچ;
			}

			setup_work_درخت(the_repository);
			p = prefix_path(the_repository, prefix, prefix_length, path);
			update_one(p);
			اگر (set_executable_bit)
				chmod_path(set_executable_bit, p);
			آزاد(p);
			ctx.argc--;
			ctx.argv++;
			بشکن;
		}
		حالت PARSE_OPT_UNKNOWN:
			اگر (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			وگرنه
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_index_usage, options);
		}
	}
	argc = parse_options_end(&ctx);

	getline_fn = nul_term_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	اگر (preferred_index_format) {
		اگر (preferred_index_format < 0) {
			printf(_("%d\n"), the_repository->index->version);
		} وگرنه اگر (preferred_index_format < INDEX_FORMAT_LB ||
			   INDEX_FORMAT_UB < preferred_index_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_index_format,
			    INDEX_FORMAT_LB, INDEX_FORMAT_UB);
		} وگرنه {
			اگر (the_repository->index->version != preferred_index_format)
				the_repository->index->cache_changed |= SOMETHING_CHANGED;
			report(_("index-version: was %d, set to %d"),
			       the_repository->index->version, preferred_index_format);
			the_repository->index->version = preferred_index_format;
		}
	}

	اگر (read_from_stdin) {
		ساختار strbuf buf = STRBUF_INIT;
		ساختار strbuf unquoted = STRBUF_INIT;

		setup_work_درخت(the_repository);
		وقتی (getline_fn(&buf, stdin) != EOF) {
			نویسه *p;
			اگر (!nul_term_سطر && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				اگر (unquote_c_style(&unquoted, buf.buf, پوچ))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(the_repository, prefix, prefix_length, buf.buf);
			update_one(p);
			اگر (set_executable_bit)
				chmod_path(set_executable_bit, p);
			آزاد(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	اگر (split_index > 0) {
		اگر (repo_config_get_split_index(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		اگر (the_repository->index->split_index)
			the_repository->index->cache_changed |= SPLIT_INDEX_ORDERED;
		وگرنه
			add_split_index(the_repository->index);
	} وگرنه اگر (!split_index) {
		اگر (repo_config_get_split_index(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_split_index(the_repository->index);
	}

	prepare_repo_settings(r);
	انتخاب (untracked_cache) {
	حالت UC_UNSPECIFIED:
		بشکن;
	حالت UC_DISABLE:
		اگر (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->index);
		report(_("Untracked cache disabled"));
		بشکن;
	حالت UC_TEST:
		setup_work_درخت(the_repository);
		بازگشت !test_if_untracked_cache_is_supported();
	حالت UC_ENABLE:
	حالت UC_FORCE:
		اگر (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		add_untracked_cache(the_repository->index);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_درخت(the_repository));
		بشکن;
	پیشفرض:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	اگر (fsmonitor > 0) {
		شمارش fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		شمارش fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		اگر (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_پیام(r, reason));

		اگر (fsm_mode == FSMONITOR_MODE_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		add_fsmonitor(the_repository->index);
		report(_("fsmonitor enabled"));
	} وگرنه اگر (!fsmonitor) {
		شمارش fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		اگر (fsm_mode > FSMONITOR_MODE_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->index);
		report(_("fsmonitor disabled"));
	}

	اگر (the_repository->index->cache_changed || force_write) {
		اگر (newfd < 0) {
			اگر (refresh_args.flags & REFRESH_QUIET)
				exit(128);
			unable_to_lock_die(repo_get_index_file(the_repository), lock_error);
		}
		اگر (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die("Unable to write new index file");
	}

	rollback_lock_file(&lock_file);

	بازگشت has_errors ? 1 : 0;
}
