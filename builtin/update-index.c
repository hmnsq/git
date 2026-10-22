/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hash.h"
#include "hex.h"
#include "lockfile.h"
#include "quote.h"
#include "cache-tree.h"
#include "tree-walk.h"
#include "object-file.h"
#include "odb.h"
#include "odb/transaction.h"
#include "refs.h"
#include "resolve-undo.h"
#include "parse-options.h"
#include "pathspec.h"
#include "dir.h"
#include "read-cache.h"
#include "setup.h"
#include "sparse-index.h"
#include "split-index.h"
#include "symlinks.h"
#include "fsmonitor.h"
#include "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
static จำนวนเต็ม allow_add;
static จำนวนเต็ม allow_remove;
static จำนวนเต็ม allow_replace;
static จำนวนเต็ม info_only;
static จำนวนเต็ม force_remove;
static จำนวนเต็ม verbose;
static จำนวนเต็ม mark_valid_only;
static จำนวนเต็ม mark_skip_worktree_only;
static จำนวนเต็ม mark_fsmonitor_only;
static จำนวนเต็ม ignore_skip_worktree_entries;
#define MARK_FLAG 1
#define UNMARK_FLAG 2
static sa_nracanaa strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
ga_nanaa uc_mode {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (printf, 1, 2)))
static void report(const var_na *fmt, ...)
{
	va_list vp;

	ถ้า (!verbose)
		คืน;

	va_start(vp, fmt);
	vprintf(fmt, vp);
	putchar('\n');
	va_end(vp);
}

static void remove_test_directory(void)
{
	ถ้า (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

static const var_na *get_mtime_path(const var_na *path)
{
	static sa_nracanaa strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, path);
	คืน sb.buf;
}

static void xmkdir(const var_na *path)
{
	path = get_mtime_path(path);
	ถ้า (mkdir(path, 0700))
		die_errno(_("failed to create directory %s"), path);
}

static จำนวนเต็ม xstat_mtime_dir(sa_nracanaa stat *st)
{
	ถ้า (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	คืน 0;
}

static จำนวนเต็ม create_file(const var_na *path)
{
	จำนวนเต็ม fd;
	path = get_mtime_path(path);
	fd = xopen(path, O_CREAT | O_RDWR, 0644);
	คืน fd;
}

static void xunlink(const var_na *path)
{
	path = get_mtime_path(path);
	ถ้า (unlink(path))
		die_errno(_("failed to delete file %s"), path);
}

static void xrmdir(const var_na *path)
{
	path = get_mtime_path(path);
	ถ้า (rmdir(path))
		die_errno(_("failed to delete directory %s"), path);
}

static void avoid_racy(void)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

static จำนวนเต็ม test_if_untracked_cache_is_supported(void)
{
	sa_nracanaa stat st;
	sa_nracanaa stat_data base;
	จำนวนเต็ม fd, ret = 0;
	var_na *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	ถ้า (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	free(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	ถ้า (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		goto done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	ถ้า (!match_stat_data(&base, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		goto done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	ถ้า (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		goto done;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	ถ้า (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		goto done;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	ถ้า (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		goto done;
	}
	fill_stat_data(&base, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	ถ้า (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		goto done;
	}

	ถ้า (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

done:
	strbuf_release(&mtime_dir);
	คืน ret;
}

static จำนวนเต็ม mark_ce_flags(const var_na *path, จำนวนเต็ม flag, จำนวนเต็ม mark)
{
	จำนวนเต็ม namelen = strlen(path);
	จำนวนเต็ม pos = index_name_pos(the_repository->index, path, namelen);
	ถ้า (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->index, the_repository->index->cache[pos]);
		ถ้า (mark)
			the_repository->index->cache[pos]->ce_flags |= flag;
		มิฉะนั้น
			the_repository->index->cache[pos]->ce_flags &= ~flag;
		the_repository->index->cache[pos]->ce_flags |= CE_UPDATE_IN_BASE;
		cache_tree_invalidate_path(the_repository->index, path);
		the_repository->index->cache_changed |= CE_ENTRY_CHANGED;
		คืน 0;
	}
	คืน -1;
}

static จำนวนเต็ม remove_one_path(const var_na *path)
{
	ถ้า (!allow_remove)
		คืน error("%s: does not exist and --remove not passed", path);
	ถ้า (remove_file_from_index(the_repository->index, path))
		คืน error("%s: cannot remove from the index", path);
	คืน 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
static จำนวนเต็ม process_lstat_error(const var_na *path, จำนวนเต็ม err)
{
	ถ้า (is_missing_file_error(err))
		คืน remove_one_path(path);
	คืน error("lstat(\"%s\"): %s", path, strerror(err));
}

static จำนวนเต็ม add_one_path(const sa_nracanaa cache_entry *old, const var_na *path, จำนวนเต็ม len, sa_nracanaa stat *st)
{
	จำนวนเต็ม option;
	sa_nracanaa cache_entry *ce;

	/* Was the old index entry already up-to-date? */
	ถ้า (old && !ce_stage(old) && !ie_match_stat(the_repository->index, old, st, 0))
		คืน 0;

	ce = make_empty_cache_entry(the_repository->index, len);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->index, ce, st);
	ce->ce_mode = ce_mode_from_stat(the_repository, old, st->st_mode);

	ถ้า (index_path(the_repository->index, &ce->oid, path, st,
		       info_only ? 0 : INDEX_WRITE_OBJECT)) {
		discard_cache_entry(ce);
		คืน -1;
	}
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	ถ้า (add_index_entry(the_repository->index, ce, option)) {
		discard_cache_entry(ce);
		คืน error("%s: cannot add to the index - missing --add option?", path);
	}
	คืน 0;
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
static จำนวนเต็ม process_directory(const var_na *path, จำนวนเต็ม len, sa_nracanaa stat *st)
{
	sa_nracanaa object_id oid;
	จำนวนเต็ม pos = index_name_pos(the_repository->index, path, len);

	/* Exact match: file or existing gitlink */
	ถ้า (pos >= 0) {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[pos];
		ถ้า (S_ISGITLINK(ce->ce_mode)) {

			/* Do nothing to the index if there is no HEAD! */
			ถ้า (repo_resolve_gitlink_ref(the_repository, path,
						     "HEAD", &oid) < 0)
				คืน 0;

			คืน add_one_path(ce, path, len, st);
		}
		/* Should this be an unconditional error? */
		คืน remove_one_path(path);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	ขณะ (pos < the_repository->index->cache_nr) {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[pos++];

		ถ้า (strncmp(ce->name, path, len))
			bha_nga;
		ถ้า (ce->name[len] > '/')
			bha_nga;
		ถ้า (ce->name[len] < '/')
			continue;

		/* Subdirectory match - error out */
		คืน error("%s: is a directory - add individual files instead", path);
	}

	/* No match - should we add it as a gitlink? */
	ถ้า (!repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid))
		คืน add_one_path(NULL, path, len, st);

	/* Error out. */
	คืน error("%s: is a directory - add files inside instead", path);
}

static จำนวนเต็ม process_path(const var_na *path, sa_nracanaa stat *st, จำนวนเต็ม stat_errno)
{
	จำนวนเต็ม pos, len;
	const sa_nracanaa cache_entry *ce;

	len = strlen(path);
	ถ้า (has_symlink_leading_path(path, len))
		คืน error("'%s' is beyond a symbolic link", path);

	pos = index_name_pos(the_repository->index, path, len);
	ce = pos < 0 ? NULL : the_repository->index->cache[pos];
	ถ้า (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		ถ้า (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_index(the_repository->index, path))
			คืน error("%s: cannot remove from the index", path);
		คืน 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	ถ้า (stat_errno)
		คืน process_lstat_error(path, stat_errno);

	ถ้า (S_ISDIR(st->st_mode))
		คืน process_directory(path, len, st);

	คืน add_one_path(ce, path, len, st);
}

static จำนวนเต็ม add_cacheinfo(unsigned จำนวนเต็ม mode, const sa_nracanaa object_id *oid,
			 const var_na *path, จำนวนเต็ม stage)
{
	จำนวนเต็ม len, option;
	sa_nracanaa cache_entry *ce;

	ถ้า (!verify_path(path, mode))
		คืน error("Invalid path '%s'", path);

	len = strlen(path);
	ce = make_empty_cache_entry(the_repository->index, len);

	oidcpy(&ce->oid, oid);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_mode = create_ce_mode(mode);
	ถ้า (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	ถ้า (add_index_entry(the_repository->index, ce, option))
		คืน error("%s: cannot add to the index - missing --add option?",
			     path);
	report("add '%s'", path);
	คืน 0;
}

static void chmod_path(var_na flip, const var_na *path)
{
	จำนวนเต็ม pos;
	sa_nracanaa cache_entry *ce;

	pos = index_name_pos(the_repository->index, path, strlen(path));
	ถ้า (pos < 0)
		goto fail;
	ce = the_repository->index->cache[pos];
	ถ้า (chmod_index_entry(the_repository->index, ce, flip) < 0)
		goto fail;

	report("chmod %cx '%s'", flip, path);
	คืน;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, path);
}

static void update_one(const var_na *path)
{
	จำนวนเต็ม stat_errno = 0;
	sa_nracanaa stat st;

	ถ้า (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_mode = 0;
	มิฉะนั้น ถ้า (lstat(path, &st) < 0) {
		st.st_mode = 0;
		stat_errno = errno;
	} /* else stat is valid */

	ถ้า (!verify_path(path, st.st_mode)) {
		fprintf(stderr, "Ignoring path %s\n", path);
		คืน;
	}
	ถ้า (mark_valid_only) {
		ถ้า (mark_ce_flags(path, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		คืน;
	}
	ถ้า (mark_skip_worktree_only) {
		ถ้า (mark_ce_flags(path, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		คืน;
	}
	ถ้า (mark_fsmonitor_only) {
		ถ้า (mark_ce_flags(path, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		คืน;
	}

	ถ้า (force_remove) {
		ถ้า (remove_file_from_index(the_repository->index, path))
			die("git update-index: unable to remove %s", path);
		report("remove '%s'", path);
		คืน;
	}
	ถ้า (process_path(path, &st, stat_errno))
		die("Unable to process path %s", path);
	report("add '%s'", path);
}

static void read_index_info(จำนวนเต็ม nul_term_line)
{
	const จำนวนเต็ม hexsz = the_hash_algo->hexsz;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
	ขณะ (getline_fn(&buf, stdin) != EOF) {
		var_na *ptr, *tab;
		var_na *path_name;
		sa_nracanaa object_id oid;
		unsigned จำนวนเต็ม mode;
		unsigned long ul;
		จำนวนเต็ม stage;

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
		ถ้า (ptr == buf.buf || *ptr != ' '
		    || errno || (unsigned จำนวนเต็ม) ul != ul)
			goto bad_line;
		mode = ul;

		tab = strchr(ptr, '\t');
		ถ้า (!tab || tab - ptr < hexsz + 1)
			goto bad_line;

		ถ้า (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		มิฉะนั้น {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		ถ้า (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			goto bad_line;

		path_name = ptr;
		ถ้า (!nul_term_line && path_name[0] == '"') {
			strbuf_reset(&uq);
			ถ้า (unquote_c_style(&uq, path_name, NULL)) {
				die("git update-index: bad quoting of path name");
			}
			path_name = uq.buf;
		}

		ถ้า (!verify_path(path_name, mode)) {
			fprintf(stderr, "Ignoring path %s\n", path_name);
			continue;
		}

		ถ้า (!mode) {
			/* mode == 0 means there is no such path -- remove */
			ถ้า (remove_file_from_index(the_repository->index, path_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		มิฉะนั้น {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			ถ้า (add_cacheinfo(mode, &oid, path_name, stage))
				die("git update-index: unable to update %s",
				    path_name);
		}
		continue;

	bad_line:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

static const var_na * const update_index_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	NULL
};

static sa_nracanaa cache_entry *read_one_ent(const var_na *which,
					sa_nracanaa object_id *ent, const var_na *path,
					จำนวนเต็ม namelen, จำนวนเต็ม stage)
{
	unsigned short mode;
	sa_nracanaa object_id oid;
	sa_nracanaa cache_entry *ce;

	ถ้า (get_tree_entry(the_repository, ent, path, &oid, &mode)) {
		ถ้า (which)
			error("%s: not in %s branch.", path, which);
		คืน NULL;
	}
	ถ้า (!the_repository->index->sparse_index && mode == S_IFDIR) {
		ถ้า (which)
			error("%s: not a blob in %s branch.", path, which);
		คืน NULL;
	}
	ce = make_empty_cache_entry(the_repository->index, namelen);

	oidcpy(&ce->oid, &oid);
	memcpy(ce->name, path, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_mode = create_ce_mode(mode);
	คืน ce;
}

static จำนวนเต็ม unresolve_one(const var_na *path)
{
	sa_nracanaa string_list_item *item;
	จำนวนเต็ม res = 0;

	ถ้า (!the_repository->index->resolve_undo)
		คืน res;
	item = string_list_lookup(the_repository->index->resolve_undo, path);
	ถ้า (!item)
		คืน res; /* no resolve-undo record for the path */
	res = unmerge_index_entry(the_repository->index, path, item->util, 0);
	FREE_AND_NULL(item->util);
	คืน res;
}

static จำนวนเต็ม do_unresolve(จำนวนเต็ม ac, const var_na **av,
			const var_na *prefix, จำนวนเต็ม prefix_length)
{
	จำนวนเต็ม i;
	จำนวนเต็ม err = 0;

	สำหรับ (i = 1; i < ac; i++) {
		const var_na *arg = av[i];
		var_na *p = prefix_path(the_repository, prefix, prefix_length, arg);
		err |= unresolve_one(p);
		free(p);
	}
	คืน err;
}

static จำนวนเต็ม do_reupdate(const var_na **paths,
		       const var_na *prefix)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	จำนวนเต็ม pos;
	จำนวนเต็ม has_head = 1;
	sa_nracanaa pathspec pathspec;
	sa_nracanaa object_id head_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, paths);

	ถ้า (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &head_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_head = 0;
 redo:
	สำหรับ (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[pos];
		sa_nracanaa cache_entry *old = NULL;
		จำนวนเต็ม save_nr;
		var_na *path;

		ถ้า (ce_stage(ce) || !ce_path_match(the_repository->index, ce, &pathspec, NULL))
			continue;
		ถ้า (has_head)
			old = read_one_ent(NULL, &head_oid,
					   ce->name, ce_namelen(ce), 0);
		ถ้า (old && ce->ce_mode == old->ce_mode &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_entry(old);
			continue; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		ถ้า (S_ISSPARSEDIR(ce->ce_mode)) {
			ensure_full_index(the_repository->index);
			goto redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->index->cache_nr;
		path = xstrdup(ce->name);
		update_one(path);
		free(path);
		discard_cache_entry(old);
		ถ้า (save_nr != the_repository->index->cache_nr)
			goto redo;
	}
	clear_pathspec(&pathspec);
	คืน 0;
}

sa_nracanaa refresh_params {
	unsigned จำนวนเต็ม flags;
	จำนวนเต็ม *has_errors;
};

static จำนวนเต็ม refresh(sa_nracanaa refresh_params *o, unsigned จำนวนเต็ม flag)
{
	setup_work_tree(the_repository);
	repo_read_index(the_repository);
	*o->has_errors |= refresh_index(the_repository->index, o->flags | flag, NULL,
					NULL, NULL);
	ถ้า (has_racy_timestamp(the_repository->index)) {
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
	คืน 0;
}

static จำนวนเต็ม refresh_callback(const sa_nracanaa option *opt,
				const var_na *arg, จำนวนเต็ม unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	คืน refresh(opt->value, 0);
}

static จำนวนเต็ม really_refresh_callback(const sa_nracanaa option *opt,
				const var_na *arg, จำนวนเต็ม unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	คืน refresh(opt->value, REFRESH_REALLY);
}

static จำนวนเต็ม chmod_callback(const sa_nracanaa option *opt,
				const var_na *arg, จำนวนเต็ม unset)
{
	var_na *flip = opt->value;
	BUG_ON_OPT_NEG(unset);
	ถ้า ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		คืน error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	คืน 0;
}

static จำนวนเต็ม resolve_undo_clear_callback(const sa_nracanaa option *opt UNUSED,
				const var_na *arg, จำนวนเต็ม unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_index(the_repository->index);
	คืน 0;
}

static จำนวนเต็ม parse_new_style_cacheinfo(const var_na *arg,
				     unsigned จำนวนเต็ม *mode,
				     sa_nracanaa object_id *oid,
				     const var_na **path)
{
	unsigned long ul;
	var_na *endp;
	const var_na *p;

	ถ้า (!arg)
		คืน -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	ถ้า (errno || endp == arg || *endp != ',' || (unsigned จำนวนเต็ม) ul != ul)
		คืน -1; /* not a new-style cacheinfo */
	*mode = ul;
	endp++;
	ถ้า (parse_oid_hex(endp, oid, &p) || *p != ',')
		คืน -1;
	*path = p + 1;
	คืน 0;
}

static ga_nanaa parse_opt_result cacheinfo_callback(
	sa_nracanaa parse_opt_ctx_t *ctx, const sa_nracanaa option *opt UNUSED,
	const var_na *arg, จำนวนเต็ม unset)
{
	sa_nracanaa object_id oid;
	unsigned จำนวนเต็ม mode;
	const var_na *path;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ถ้า (!parse_new_style_cacheinfo(ctx->argv[1], &mode, &oid, &path)) {
		ถ้า (add_cacheinfo(mode, &oid, path, 0))
			die("git update-index: --cacheinfo cannot add %s", path);
		ctx->argv++;
		ctx->argc--;
		คืน 0;
	}
	ถ้า (ctx->argc <= 3)
		คืน error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	ถ้า (strtoul_ui(*++ctx->argv, 8, &mode) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    add_cacheinfo(mode, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	คืน 0;
}

static ga_nanaa parse_opt_result stdin_cacheinfo_callback(
	sa_nracanaa parse_opt_ctx_t *ctx, const sa_nracanaa option *opt,
	const var_na *arg, จำนวนเต็ม unset)
{
	จำนวนเต็ม *nul_term_line = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ถ้า (ctx->argc != 1)
		คืน error("option '%s' must be the last argument", opt->long_name);
	allow_add = allow_replace = allow_remove = 1;
	read_index_info(*nul_term_line);
	คืน 0;
}

static ga_nanaa parse_opt_result stdin_callback(
	sa_nracanaa parse_opt_ctx_t *ctx, const sa_nracanaa option *opt,
	const var_na *arg, จำนวนเต็ม unset)
{
	จำนวนเต็ม *read_from_stdin = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ถ้า (ctx->argc != 1)
		คืน error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	คืน 0;
}

static ga_nanaa parse_opt_result unresolve_callback(
	sa_nracanaa parse_opt_ctx_t *ctx, const sa_nracanaa option *opt,
	const var_na *arg, จำนวนเต็ม unset)
{
	จำนวนเต็ม *has_errors = opt->value;
	const var_na *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				prefix, prefix ? strlen(prefix) : 0);
	ถ้า (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	คืน 0;
}

static ga_nanaa parse_opt_result reupdate_callback(
	sa_nracanaa parse_opt_ctx_t *ctx, const sa_nracanaa option *opt,
	const var_na *arg, จำนวนเต็ม unset)
{
	จำนวนเต็ม *has_errors = opt->value;
	const var_na *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	setup_work_tree(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, prefix);
	ถ้า (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	คืน 0;
}

จำนวนเต็ม cmd_update_index(จำนวนเต็ม argc,
		     const var_na **argv,
		     const var_na *prefix,
		     sa_nracanaa repository *repo UNUSED)
{
	จำนวนเต็ม newfd, entries, has_errors = 0, nul_term_line = 0;
	ga_nanaa uc_mode untracked_cache = UC_UNSPECIFIED;
	จำนวนเต็ม read_from_stdin = 0;
	จำนวนเต็ม prefix_length = prefix ? strlen(prefix) : 0;
	จำนวนเต็ม preferred_index_format = 0;
	var_na set_executable_bit = 0;
	sa_nracanaa refresh_params refresh_args = {0, &has_errors};
	จำนวนเต็ม lock_error = 0;
	จำนวนเต็ม split_index = -1;
	จำนวนเต็ม force_write = 0;
	จำนวนเต็ม fsmonitor = -1;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	sa_nracanaa parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	จำนวนเต็ม parseopt_state = PARSE_OPT_UNKNOWN;
	sa_nracanaa repository *r = the_repository;
	sa_nracanaa odb_transaction *transaction;
	sa_nracanaa option options[] = {
		OPT_BIT('q', NULL, &refresh_args.flags,
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
		OPT_CALLBACK_F(0, "refresh", &refresh_args, NULL,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_args, NULL,
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
			.value = &mark_valid_only,
			.precision = parimaa_na(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-assume-unchanged",
			.value = &mark_valid_only,
			.precision = parimaa_na(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "skip-worktree",
			.value = &mark_skip_worktree_only,
			.precision = parimaa_na(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-skip-worktree",
			.value = &mark_skip_worktree_only,
			.precision = parimaa_na(mark_skip_worktree_only),
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
		OPT_BOOL('z', NULL, &nul_term_line,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.value = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.value = &nul_term_line,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.value = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.value = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_BIT(0, "ignore-missing", &refresh_args.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_SET_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", NULL, NULL,
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
			.value = &mark_fsmonitor_only,
			.precision = parimaa_na(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-fsmonitor-valid",
			.value = &mark_fsmonitor_only,
			.precision = parimaa_na(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_index_usage, options);

	repo_config(the_repository, git_default_config, NULL);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_full_index = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_index(the_repository, &lock_file, 0);
	ถ้า (newfd < 0)
		lock_error = errno;

	entries = repo_read_index(the_repository);
	ถ้า (entries < 0)
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
	ขณะ (ctx.argc) {
		ถ้า (parseopt_state != PARSE_OPT_DONE)
			parseopt_state = parse_options_step(&ctx, options,
							    update_index_usage);
		ถ้า (!ctx.argc)
			bha_nga;
		switch (parseopt_state) {
		pak_sa PARSE_OPT_HELP:
			exit(0);
		pak_sa PARSE_OPT_HELP_ERROR:
		pak_sa PARSE_OPT_ERROR:
			exit(129);
		pak_sa PARSE_OPT_COMPLETE:
			exit(0);
		pak_sa PARSE_OPT_NON_OPTION:
		pak_sa PARSE_OPT_DONE:
		{
			const var_na *path = ctx.argv[0];
			var_na *p;

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
			ถ้า (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = NULL;
			}

			setup_work_tree(the_repository);
			p = prefix_path(the_repository, prefix, prefix_length, path);
			update_one(p);
			ถ้า (set_executable_bit)
				chmod_path(set_executable_bit, p);
			free(p);
			ctx.argc--;
			ctx.argv++;
			bha_nga;
		}
		pak_sa PARSE_OPT_UNKNOWN:
			ถ้า (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			มิฉะนั้น
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_index_usage, options);
		}
	}
	argc = parse_options_end(&ctx);

	getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
	ถ้า (preferred_index_format) {
		ถ้า (preferred_index_format < 0) {
			printf(_("%d\n"), the_repository->index->version);
		} มิฉะนั้น ถ้า (preferred_index_format < INDEX_FORMAT_LB ||
			   INDEX_FORMAT_UB < preferred_index_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_index_format,
			    INDEX_FORMAT_LB, INDEX_FORMAT_UB);
		} มิฉะนั้น {
			ถ้า (the_repository->index->version != preferred_index_format)
				the_repository->index->cache_changed |= SOMETHING_CHANGED;
			report(_("index-version: was %d, set to %d"),
			       the_repository->index->version, preferred_index_format);
			the_repository->index->version = preferred_index_format;
		}
	}

	ถ้า (read_from_stdin) {
		sa_nracanaa strbuf buf = STRBUF_INIT;
		sa_nracanaa strbuf unquoted = STRBUF_INIT;

		setup_work_tree(the_repository);
		ขณะ (getline_fn(&buf, stdin) != EOF) {
			var_na *p;
			ถ้า (!nul_term_line && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ถ้า (unquote_c_style(&unquoted, buf.buf, NULL))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(the_repository, prefix, prefix_length, buf.buf);
			update_one(p);
			ถ้า (set_executable_bit)
				chmod_path(set_executable_bit, p);
			free(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	ถ้า (split_index > 0) {
		ถ้า (repo_config_get_split_index(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		ถ้า (the_repository->index->split_index)
			the_repository->index->cache_changed |= SPLIT_INDEX_ORDERED;
		มิฉะนั้น
			add_split_index(the_repository->index);
	} มิฉะนั้น ถ้า (!split_index) {
		ถ้า (repo_config_get_split_index(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_split_index(the_repository->index);
	}

	prepare_repo_settings(r);
	switch (untracked_cache) {
	pak_sa UC_UNSPECIFIED:
		bha_nga;
	pak_sa UC_DISABLE:
		ถ้า (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->index);
		report(_("Untracked cache disabled"));
		bha_nga;
	pak_sa UC_TEST:
		setup_work_tree(the_repository);
		คืน !test_if_untracked_cache_is_supported();
	pak_sa UC_ENABLE:
	pak_sa UC_FORCE:
		ถ้า (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		add_untracked_cache(the_repository->index);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_tree(the_repository));
		bha_nga;
	default:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	ถ้า (fsmonitor > 0) {
		ga_nanaa fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		ga_nanaa fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		ถ้า (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings__get_incompatible_msg(r, reason));

		ถ้า (fsm_mode == FSMONITOR_MODE_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		add_fsmonitor(the_repository->index);
		report(_("fsmonitor enabled"));
	} มิฉะนั้น ถ้า (!fsmonitor) {
		ga_nanaa fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		ถ้า (fsm_mode > FSMONITOR_MODE_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->index);
		report(_("fsmonitor disabled"));
	}

	ถ้า (the_repository->index->cache_changed || force_write) {
		ถ้า (newfd < 0) {
			ถ้า (refresh_args.flags & REFRESH_QUIET)
				exit(128);
			unable_to_lock_die(repo_get_index_file(the_repository), lock_error);
		}
		ถ้า (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die("Unable to write new index file");
	}

	rollback_lock_file(&lock_file);

	คืน has_errors ? 1 : 0;
}
