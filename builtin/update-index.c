/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/transaction.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "resolve-undo.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "pathspec.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "read-cache.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "split-index.h"
#அந்தர்பாவ "symlinks.h"
#அந்தர்பாவ "fsmonitor.h"
#அந்தர்பாவ "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
நிலையான முழு allow_add;
நிலையான முழு allow_remove;
நிலையான முழு allow_replace;
நிலையான முழு info_only;
நிலையான முழு force_remove;
நிலையான முழு verbose;
நிலையான முழு mark_valid_only;
நிலையான முழு mark_skip_worktree_only;
நிலையான முழு mark_fsmonitor_only;
நிலையான முழு ignore_skip_worktree_entries;
#define MARK_FLAG 1
#define UNMARK_FLAG 2
நிலையான கட்டமைப்பு strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
எண்ணிக்கை uc_mode {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (printf, 1, 2)))
நிலையான வெறுமை report(மாறிலி எழுத்து *fmt, ...)
{
	va_list vp;

	எனில் (!verbose)
		திரும்பு;

	va_start(vp, fmt);
	vprintf(fmt, vp);
	putchar('\n');
	va_end(vp);
}

நிலையான வெறுமை remove_test_directory(வெறுமை)
{
	எனில் (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

நிலையான மாறிலி எழுத்து *get_mtime_path(மாறிலி எழுத்து *path)
{
	நிலையான கட்டமைப்பு strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, path);
	திரும்பு sb.buf;
}

நிலையான வெறுமை xmkdir(மாறிலி எழுத்து *path)
{
	path = get_mtime_path(path);
	எனில் (mkdir(path, 0700))
		die_errno(_("failed to create directory %s"), path);
}

நிலையான முழு xstat_mtime_dir(கட்டமைப்பு stat *st)
{
	எனில் (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	திரும்பு 0;
}

நிலையான முழு create_file(மாறிலி எழுத்து *path)
{
	முழு fd;
	path = get_mtime_path(path);
	fd = xopen(path, O_CREAT | O_RDWR, 0644);
	திரும்பு fd;
}

நிலையான வெறுமை xunlink(மாறிலி எழுத்து *path)
{
	path = get_mtime_path(path);
	எனில் (unlink(path))
		die_errno(_("failed to delete file %s"), path);
}

நிலையான வெறுமை xrmdir(மாறிலி எழுத்து *path)
{
	path = get_mtime_path(path);
	எனில் (rmdir(path))
		die_errno(_("failed to delete directory %s"), path);
}

நிலையான வெறுமை avoid_racy(வெறுமை)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

நிலையான முழு test_if_untracked_cache_is_supported(வெறுமை)
{
	கட்டமைப்பு stat st;
	கட்டமைப்பு stat_data base;
	முழு fd, ret = 0;
	எழுத்து *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	எனில் (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	விடுவி(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	எனில் (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		செல் done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	எனில் (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		செல் done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	எனில் (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		செல் done;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	எனில் (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		செல் done;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	எனில் (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		செல் done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	எனில் (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		செல் done;
	}

	எனில் (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

done:
	strbuf_release(&mtime_dir);
	திரும்பு ret;
}

நிலையான முழு mark_ce_flags(மாறிலி எழுத்து *path, முழு flag, முழு mark)
{
	முழு namelen = strlen(path);
	முழு pos = index_name_pos(the_repository->index, path, namelen);
	எனில் (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->index, the_repository->index->cache[pos]);
		எனில் (mark)
			the_repository->index->cache[pos]->ce_flags |= flag;
		இல்லையெனில்
			the_repository->index->cache[pos]->ce_flags &= ~flag;
		the_repository->index->cache[pos]->ce_flags |= CE_UPDATE_IN_BASE;
		cache_மரம்_invalidate_path(the_repository->index, path);
		the_repository->index->cache_changed |= CE_ENTRY_CHANGED;
		திரும்பு 0;
	}
	திரும்பு -1;
}

நிலையான முழு remove_one_path(மாறிலி எழுத்து *path)
{
	எனில் (!allow_remove)
		திரும்பு error("%s: does not exist and --remove not passed", path);
	எனில் (remove_file_from_index(the_repository->index, path))
		திரும்பு error("%s: cannot remove from the index", path);
	திரும்பு 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
நிலையான முழு process_lstat_error(மாறிலி எழுத்து *path, முழு err)
{
	எனில் (is_missing_file_error(err))
		திரும்பு remove_one_path(path);
	திரும்பு error("lstat(\"%s\"): %s", path, strerror(err));
}

நிலையான முழு add_one_path(மாறிலி கட்டமைப்பு cache_entry *old, மாறிலி எழுத்து *path, முழு len, கட்டமைப்பு stat *st)
{
	முழு option;
	கட்டமைப்பு cache_entry *ce;

	/* Was the old index entry already up-to-date? */
	எனில் (old && !ce_stage(old) && !ie_match_stat(the_repository->index, old, st, 0))
		திரும்பு 0;

	ce = make_empty_cache_entry(the_repository->index, len);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->index, ce, st);
	ce->ce_mode = ce_mode_from_stat(the_repository, old, st->st_mode);

	எனில் (index_path(the_repository->index, &ce->oid, path, st,
		       info_only ? 0 : INDEX_WRITE_OBJECT)) {
		discard_cache_entry(ce);
		திரும்பு -1;
	}
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	எனில் (add_index_entry(the_repository->index, ce, option)) {
		discard_cache_entry(ce);
		திரும்பு error("%s: cannot add to the index - missing --add option?", path);
	}
	திரும்பு 0;
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
நிலையான முழு process_directory(மாறிலி எழுத்து *path, முழு len, கட்டமைப்பு stat *st)
{
	கட்டமைப்பு object_id oid;
	முழு pos = index_name_pos(the_repository->index, path, len);

	/* Exact match: file or existing gitlink */
	எனில் (pos >= 0) {
		மாறிலி கட்டமைப்பு cache_entry *ce = the_repository->index->cache[pos];
		எனில் (S_ISGITLINK(ce->ce_mode)) {

			/* Do nothing to the index if there is no HEAD! */
			எனில் (repo_resolve_gitlink_ref(the_repository, path,
						     "HEAD", &oid) < 0)
				திரும்பு 0;

			திரும்பு add_one_path(ce, path, len, st);
		}
		/* Should this be an unconditional error? */
		திரும்பு remove_one_path(path);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	போது (pos < the_repository->index->cache_nr) {
		மாறிலி கட்டமைப்பு cache_entry *ce = the_repository->index->cache[pos++];

		எனில் (strncmp(ce->name, path, len))
			நிறுத்து;
		எனில் (ce->name[len] > '/')
			நிறுத்து;
		எனில் (ce->name[len] < '/')
			தொடர்;

		/* Subdirectory match - error out */
		திரும்பு error("%s: is a directory - add individual files instead", path);
	}

	/* No match - should we add it as a gitlink? */
	எனில் (!repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid))
		திரும்பு add_one_path(வெற்று, path, len, st);

	/* Error out. */
	திரும்பு error("%s: is a directory - add files inside instead", path);
}

நிலையான முழு process_path(மாறிலி எழுத்து *path, கட்டமைப்பு stat *st, முழு stat_errno)
{
	முழு pos, len;
	மாறிலி கட்டமைப்பு cache_entry *ce;

	len = strlen(path);
	எனில் (has_symlink_leading_path(path, len))
		திரும்பு error("'%s' is beyond a symbolic link", path);

	pos = index_name_pos(the_repository->index, path, len);
	ce = pos < 0 ? வெற்று : the_repository->index->cache[pos];
	எனில் (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		எனில் (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_index(the_repository->index, path))
			திரும்பு error("%s: cannot remove from the index", path);
		திரும்பு 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	எனில் (stat_errno)
		திரும்பு process_lstat_error(path, stat_errno);

	எனில் (S_ISDIR(st->st_mode))
		திரும்பு process_directory(path, len, st);

	திரும்பு add_one_path(ce, path, len, st);
}

நிலையான முழு add_cacheinfo(unsigned முழு mode, மாறிலி கட்டமைப்பு object_id *oid,
			 மாறிலி எழுத்து *path, முழு stage)
{
	முழு len, option;
	கட்டமைப்பு cache_entry *ce;

	எனில் (!verify_path(path, mode))
		திரும்பு error("Invalid path '%s'", path);

	len = strlen(path);
	ce = make_empty_cache_entry(the_repository->index, len);

	oidcpy(&ce->oid, oid);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_mode = create_ce_mode(mode);
	எனில் (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	எனில் (add_index_entry(the_repository->index, ce, option))
		திரும்பு error("%s: cannot add to the index - missing --add option?",
			     path);
	report("add '%s'", path);
	திரும்பு 0;
}

நிலையான வெறுமை chmod_path(எழுத்து flip, மாறிலி எழுத்து *path)
{
	முழு pos;
	கட்டமைப்பு cache_entry *ce;

	pos = index_name_pos(the_repository->index, path, strlen(path));
	எனில் (pos < 0)
		செல் fail;
	ce = the_repository->index->cache[pos];
	எனில் (chmod_index_entry(the_repository->index, ce, flip) < 0)
		செல் fail;

	report("chmod %cx '%s'", flip, path);
	திரும்பு;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, path);
}

நிலையான வெறுமை update_one(மாறிலி எழுத்து *path)
{
	முழு stat_errno = 0;
	கட்டமைப்பு stat st;

	எனில் (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_mode = 0;
	இல்லையெனில் எனில் (lstat(path, &st) < 0) {
		st.st_mode = 0;
		stat_errno = errno;
	} /* else stat is valid */

	எனில் (!verify_path(path, st.st_mode)) {
		fprintf(stderr, "Ignoring path %s\n", path);
		திரும்பு;
	}
	எனில் (mark_valid_only) {
		எனில் (mark_ce_flags(path, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		திரும்பு;
	}
	எனில் (mark_skip_worktree_only) {
		எனில் (mark_ce_flags(path, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		திரும்பு;
	}
	எனில் (mark_fsmonitor_only) {
		எனில் (mark_ce_flags(path, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		திரும்பு;
	}

	எனில் (force_remove) {
		எனில் (remove_file_from_index(the_repository->index, path))
			die("git update-index: unable to remove %s", path);
		report("remove '%s'", path);
		திரும்பு;
	}
	எனில் (process_path(path, &st, stat_errno))
		die("Unable to process path %s", path);
	report("add '%s'", path);
}

நிலையான வெறுமை read_index_info(முழு nul_term_வரி)
{
	மாறிலி முழு hexsz = the_hash_algo->hexsz;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_வரி ? strbuf_getline_nul : strbuf_getline_lf;
	போது (getline_fn(&buf, stdin) != EOF) {
		எழுத்து *ptr, *tab;
		எழுத்து *path_name;
		கட்டமைப்பு object_id oid;
		unsigned முழு mode;
		unsigned long ul;
		முழு stage;

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
		எனில் (ptr == buf.buf || *ptr != ' '
		    || errno || (unsigned முழு) ul != ul)
			செல் bad_வரி;
		mode = ul;

		tab = strchr(ptr, '\t');
		எனில் (!tab || tab - ptr < hexsz + 1)
			செல் bad_வரி;

		எனில் (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		இல்லையெனில் {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		எனில் (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			செல் bad_வரி;

		path_name = ptr;
		எனில் (!nul_term_வரி && path_name[0] == '"') {
			strbuf_reset(&uq);
			எனில் (unquote_c_style(&uq, path_name, வெற்று)) {
				die("git update-index: bad quoting of path name");
			}
			path_name = uq.buf;
		}

		எனில் (!verify_path(path_name, mode)) {
			fprintf(stderr, "Ignoring path %s\n", path_name);
			தொடர்;
		}

		எனில் (!mode) {
			/* mode == 0 means there is no such path -- remove */
			எனில் (remove_file_from_index(the_repository->index, path_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		இல்லையெனில் {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			எனில் (add_cacheinfo(mode, &oid, path_name, stage))
				die("git update-index: unable to update %s",
				    path_name);
		}
		தொடர்;

	bad_வரி:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

நிலையான மாறிலி எழுத்து * மாறிலி update_index_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	வெற்று
};

நிலையான கட்டமைப்பு cache_entry *read_one_ent(மாறிலி எழுத்து *which,
					கட்டமைப்பு object_id *ent, மாறிலி எழுத்து *path,
					முழு namelen, முழு stage)
{
	unsigned short mode;
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு cache_entry *ce;

	எனில் (get_மரம்_entry(the_repository, ent, path, &oid, &mode)) {
		எனில் (which)
			error("%s: not in %s branch.", path, which);
		திரும்பு வெற்று;
	}
	எனில் (!the_repository->index->sparse_index && mode == S_IFDIR) {
		எனில் (which)
			error("%s: not a blob in %s branch.", path, which);
		திரும்பு வெற்று;
	}
	ce = make_empty_cache_entry(the_repository->index, namelen);

	oidcpy(&ce->oid, &oid);
	memcpy(ce->name, path, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_mode = create_ce_mode(mode);
	திரும்பு ce;
}

நிலையான முழு unresolve_one(மாறிலி எழுத்து *path)
{
	கட்டமைப்பு string_list_item *item;
	முழு res = 0;

	எனில் (!the_repository->index->resolve_undo)
		திரும்பு res;
	item = string_list_lookup(the_repository->index->resolve_undo, path);
	எனில் (!item)
		திரும்பு res; /* no resolve-undo record for the path */
	res = unmerge_index_entry(the_repository->index, path, item->util, 0);
	FREE_AND_NULL(item->util);
	திரும்பு res;
}

நிலையான முழு do_unresolve(முழு ac, மாறிலி எழுத்து **av,
			மாறிலி எழுத்து *prefix, முழு prefix_length)
{
	முழு i;
	முழு err = 0;

	க்கு (i = 1; i < ac; i++) {
		மாறிலி எழுத்து *arg = av[i];
		எழுத்து *p = prefix_path(the_repository, prefix, prefix_length, arg);
		err |= unresolve_one(p);
		விடுவி(p);
	}
	திரும்பு err;
}

நிலையான முழு do_reupdate(மாறிலி எழுத்து **paths,
		       மாறிலி எழுத்து *prefix)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	முழு pos;
	முழு has_head = 1;
	கட்டமைப்பு pathspec pathspec;
	கட்டமைப்பு object_id head_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, paths);

	எனில் (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &head_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_head = 0;
 redo:
	க்கு (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		மாறிலி கட்டமைப்பு cache_entry *ce = the_repository->index->cache[pos];
		கட்டமைப்பு cache_entry *old = வெற்று;
		முழு save_nr;
		எழுத்து *path;

		எனில் (ce_stage(ce) || !ce_path_match(the_repository->index, ce, &pathspec, வெற்று))
			தொடர்;
		எனில் (has_head)
			old = read_one_ent(வெற்று, &head_oid,
					   ce->name, ce_namelen(ce), 0);
		எனில் (old && ce->ce_mode == old->ce_mode &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_entry(old);
			தொடர்; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		எனில் (S_ISSPARSEDIR(ce->ce_mode)) {
			ensure_full_index(the_repository->index);
			செல் redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->index->cache_nr;
		path = xstrdup(ce->name);
		update_one(path);
		விடுவி(path);
		discard_cache_entry(old);
		எனில் (save_nr != the_repository->index->cache_nr)
			செல் redo;
	}
	clear_pathspec(&pathspec);
	திரும்பு 0;
}

கட்டமைப்பு refresh_params {
	unsigned முழு flags;
	முழு *has_errors;
};

நிலையான முழு refresh(கட்டமைப்பு refresh_params *o, unsigned முழு flag)
{
	setup_work_மரம்(the_repository);
	repo_read_index(the_repository);
	*o->has_errors |= refresh_index(the_repository->index, o->flags | flag, வெற்று,
					வெற்று, வெற்று);
	எனில் (has_racy_timestamp(the_repository->index)) {
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
	திரும்பு 0;
}

நிலையான முழு refresh_callback(மாறிலி கட்டமைப்பு option *opt,
				மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	திரும்பு refresh(opt->மதிப்பு, 0);
}

நிலையான முழு really_refresh_callback(மாறிலி கட்டமைப்பு option *opt,
				மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	திரும்பு refresh(opt->மதிப்பு, REFRESH_REALLY);
}

நிலையான முழு chmod_callback(மாறிலி கட்டமைப்பு option *opt,
				மாறிலி எழுத்து *arg, முழு unset)
{
	எழுத்து *flip = opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	எனில் ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		திரும்பு error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	திரும்பு 0;
}

நிலையான முழு resolve_undo_clear_callback(மாறிலி கட்டமைப்பு option *opt UNUSED,
				மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_index(the_repository->index);
	திரும்பு 0;
}

நிலையான முழு parse_new_style_cacheinfo(மாறிலி எழுத்து *arg,
				     unsigned முழு *mode,
				     கட்டமைப்பு object_id *oid,
				     மாறிலி எழுத்து **path)
{
	unsigned long ul;
	எழுத்து *endp;
	மாறிலி எழுத்து *p;

	எனில் (!arg)
		திரும்பு -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	எனில் (errno || endp == arg || *endp != ',' || (unsigned முழு) ul != ul)
		திரும்பு -1; /* not a new-style cacheinfo */
	*mode = ul;
	endp++;
	எனில் (parse_oid_hex(endp, oid, &p) || *p != ',')
		திரும்பு -1;
	*path = p + 1;
	திரும்பு 0;
}

நிலையான எண்ணிக்கை parse_opt_result cacheinfo_callback(
	கட்டமைப்பு parse_opt_ctx_t *ctx, மாறிலி கட்டமைப்பு option *opt UNUSED,
	மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு object_id oid;
	unsigned முழு mode;
	மாறிலி எழுத்து *path;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	எனில் (!parse_new_style_cacheinfo(ctx->argv[1], &mode, &oid, &path)) {
		எனில் (add_cacheinfo(mode, &oid, path, 0))
			die("git update-index: --cacheinfo cannot add %s", path);
		ctx->argv++;
		ctx->argc--;
		திரும்பு 0;
	}
	எனில் (ctx->argc <= 3)
		திரும்பு error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	எனில் (strtoul_ui(*++ctx->argv, 8, &mode) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    add_cacheinfo(mode, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	திரும்பு 0;
}

நிலையான எண்ணிக்கை parse_opt_result stdin_cacheinfo_callback(
	கட்டமைப்பு parse_opt_ctx_t *ctx, மாறிலி கட்டமைப்பு option *opt,
	மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *nul_term_வரி = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	எனில் (ctx->argc != 1)
		திரும்பு error("option '%s' must be the last argument", opt->long_name);
	allow_add = allow_replace = allow_remove = 1;
	read_index_info(*nul_term_வரி);
	திரும்பு 0;
}

நிலையான எண்ணிக்கை parse_opt_result stdin_callback(
	கட்டமைப்பு parse_opt_ctx_t *ctx, மாறிலி கட்டமைப்பு option *opt,
	மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *read_from_stdin = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	எனில் (ctx->argc != 1)
		திரும்பு error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	திரும்பு 0;
}

நிலையான எண்ணிக்கை parse_opt_result unresolve_callback(
	கட்டமைப்பு parse_opt_ctx_t *ctx, மாறிலி கட்டமைப்பு option *opt,
	மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *has_errors = opt->மதிப்பு;
	மாறிலி எழுத்து *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				prefix, prefix ? strlen(prefix) : 0);
	எனில் (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	திரும்பு 0;
}

நிலையான எண்ணிக்கை parse_opt_result reupdate_callback(
	கட்டமைப்பு parse_opt_ctx_t *ctx, மாறிலி கட்டமைப்பு option *opt,
	மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *has_errors = opt->மதிப்பு;
	மாறிலி எழுத்து *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	setup_work_மரம்(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, prefix);
	எனில் (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	திரும்பு 0;
}

முழு cmd_update_index(முழு argc,
		     மாறிலி எழுத்து **argv,
		     மாறிலி எழுத்து *prefix,
		     கட்டமைப்பு repository *repo UNUSED)
{
	முழு newfd, entries, has_errors = 0, nul_term_வரி = 0;
	எண்ணிக்கை uc_mode untracked_cache = UC_UNSPECIFIED;
	முழு read_from_stdin = 0;
	முழு prefix_length = prefix ? strlen(prefix) : 0;
	முழு preferred_index_format = 0;
	எழுத்து set_executable_bit = 0;
	கட்டமைப்பு refresh_params refresh_args = {0, &has_errors};
	முழு lock_error = 0;
	முழு split_index = -1;
	முழு force_write = 0;
	முழு fsmonitor = -1;
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	கட்டமைப்பு parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	முழு parseopt_state = PARSE_OPT_UNKNOWN;
	கட்டமைப்பு repository *r = the_repository;
	கட்டமைப்பு odb_transaction *transaction;
	கட்டமைப்பு option options[] = {
		OPT_BIT('q', வெற்று, &refresh_args.flags,
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
		OPT_CALLBACK_F(0, "refresh", &refresh_args, வெற்று,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_args, வெற்று,
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
			.மதிப்பு = &mark_valid_only,
			.precision = அளவு(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-assume-unchanged",
			.மதிப்பு = &mark_valid_only,
			.precision = அளவு(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "skip-worktree",
			.மதிப்பு = &mark_skip_worktree_only,
			.precision = அளவு(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-skip-worktree",
			.மதிப்பு = &mark_skip_worktree_only,
			.precision = அளவு(mark_skip_worktree_only),
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
		OPT_BOOL('z', வெற்று, &nul_term_வரி,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.மதிப்பு = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.மதிப்பு = &nul_term_வரி,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.மதிப்பு = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.மதிப்பு = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_BIT(0, "ignore-missing", &refresh_args.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_SET_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", வெற்று, வெற்று,
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
			.மதிப்பு = &mark_fsmonitor_only,
			.precision = அளவு(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-fsmonitor-valid",
			.மதிப்பு = &mark_fsmonitor_only,
			.precision = அளவு(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_index_usage, options);

	repo_config(the_repository, git_default_config, வெற்று);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_full_index = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_index(the_repository, &lock_file, 0);
	எனில் (newfd < 0)
		lock_error = errno;

	entries = repo_read_index(the_repository);
	எனில் (entries < 0)
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
	போது (ctx.argc) {
		எனில் (parseopt_state != PARSE_OPT_DONE)
			parseopt_state = parse_options_step(&ctx, options,
							    update_index_usage);
		எனில் (!ctx.argc)
			நிறுத்து;
		தேர்வு (parseopt_state) {
		நிலை PARSE_OPT_HELP:
			exit(0);
		நிலை PARSE_OPT_HELP_ERROR:
		நிலை PARSE_OPT_ERROR:
			exit(129);
		நிலை PARSE_OPT_COMPLETE:
			exit(0);
		நிலை PARSE_OPT_NON_OPTION:
		நிலை PARSE_OPT_DONE:
		{
			மாறிலி எழுத்து *path = ctx.argv[0];
			எழுத்து *p;

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
			எனில் (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = வெற்று;
			}

			setup_work_மரம்(the_repository);
			p = prefix_path(the_repository, prefix, prefix_length, path);
			update_one(p);
			எனில் (set_executable_bit)
				chmod_path(set_executable_bit, p);
			விடுவி(p);
			ctx.argc--;
			ctx.argv++;
			நிறுத்து;
		}
		நிலை PARSE_OPT_UNKNOWN:
			எனில் (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			இல்லையெனில்
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_index_usage, options);
		}
	}
	argc = parse_options_end(&ctx);

	getline_fn = nul_term_வரி ? strbuf_getline_nul : strbuf_getline_lf;
	எனில் (preferred_index_format) {
		எனில் (preferred_index_format < 0) {
			printf(_("%d\n"), the_repository->index->version);
		} இல்லையெனில் எனில் (preferred_index_format < INDEX_FORMAT_LB ||
			   INDEX_FORMAT_UB < preferred_index_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_index_format,
			    INDEX_FORMAT_LB, INDEX_FORMAT_UB);
		} இல்லையெனில் {
			எனில் (the_repository->index->version != preferred_index_format)
				the_repository->index->cache_changed |= SOMETHING_CHANGED;
			report(_("index-version: was %d, set to %d"),
			       the_repository->index->version, preferred_index_format);
			the_repository->index->version = preferred_index_format;
		}
	}

	எனில் (read_from_stdin) {
		கட்டமைப்பு strbuf buf = STRBUF_INIT;
		கட்டமைப்பு strbuf unquoted = STRBUF_INIT;

		setup_work_மரம்(the_repository);
		போது (getline_fn(&buf, stdin) != EOF) {
			எழுத்து *p;
			எனில் (!nul_term_வரி && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				எனில் (unquote_c_style(&unquoted, buf.buf, வெற்று))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(the_repository, prefix, prefix_length, buf.buf);
			update_one(p);
			எனில் (set_executable_bit)
				chmod_path(set_executable_bit, p);
			விடுவி(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	எனில் (split_index > 0) {
		எனில் (repo_config_get_split_index(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		எனில் (the_repository->index->split_index)
			the_repository->index->cache_changed |= SPLIT_INDEX_ORDERED;
		இல்லையெனில்
			add_split_index(the_repository->index);
	} இல்லையெனில் எனில் (!split_index) {
		எனில் (repo_config_get_split_index(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_split_index(the_repository->index);
	}

	prepare_repo_settings(r);
	தேர்வு (untracked_cache) {
	நிலை UC_UNSPECIFIED:
		நிறுத்து;
	நிலை UC_DISABLE:
		எனில் (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->index);
		report(_("Untracked cache disabled"));
		நிறுத்து;
	நிலை UC_TEST:
		setup_work_மரம்(the_repository);
		திரும்பு !test_if_untracked_cache_is_supported();
	நிலை UC_ENABLE:
	நிலை UC_FORCE:
		எனில் (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		add_untracked_cache(the_repository->index);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_மரம்(the_repository));
		நிறுத்து;
	இயல்பு:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	எனில் (fsmonitor > 0) {
		எண்ணிக்கை fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		எண்ணிக்கை fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		எனில் (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_செய்தி(r, reason));

		எனில் (fsm_mode == FSMONITOR_MODE_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		add_fsmonitor(the_repository->index);
		report(_("fsmonitor enabled"));
	} இல்லையெனில் எனில் (!fsmonitor) {
		எண்ணிக்கை fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		எனில் (fsm_mode > FSMONITOR_MODE_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->index);
		report(_("fsmonitor disabled"));
	}

	எனில் (the_repository->index->cache_changed || force_write) {
		எனில் (newfd < 0) {
			எனில் (refresh_args.flags & REFRESH_QUIET)
				exit(128);
			unable_to_lock_die(repo_get_index_file(the_repository), lock_error);
		}
		எனில் (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die("Unable to write new index file");
	}

	rollback_lock_file(&lock_file);

	திரும்பு has_errors ? 1 : 0;
}
