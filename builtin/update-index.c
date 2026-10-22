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
static 정수 allow_add;
static 정수 allow_remove;
static 정수 allow_replace;
static 정수 info_only;
static 정수 force_remove;
static 정수 verbose;
static 정수 mark_valid_only;
static 정수 mark_skip_worktree_only;
static 정수 mark_fsmonitor_only;
static 정수 ignore_skip_worktree_entries;
#define MARK_FLAG 1
#define UNMARK_FLAG 2
static struct strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
enum uc_mode {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (printf, 1, 2)))
static void report(const char *fmt, ...)
{
	va_list vp;

	만약 (!verbose)
		반환;

	va_start(vp, fmt);
	vprintf(fmt, vp);
	putchar('\n');
	va_end(vp);
}

static void remove_test_directory(void)
{
	만약 (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

static const char *get_mtime_path(const char *path)
{
	static struct strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, path);
	반환 sb.buf;
}

static void xmkdir(const char *path)
{
	path = get_mtime_path(path);
	만약 (mkdir(path, 0700))
		die_errno(_("failed to create directory %s"), path);
}

static 정수 xstat_mtime_dir(struct stat *st)
{
	만약 (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	반환 0;
}

static 정수 create_file(const char *path)
{
	정수 fd;
	path = get_mtime_path(path);
	fd = xopen(path, O_CREAT | O_RDWR, 0644);
	반환 fd;
}

static void xunlink(const char *path)
{
	path = get_mtime_path(path);
	만약 (unlink(path))
		die_errno(_("failed to delete file %s"), path);
}

static void xrmdir(const char *path)
{
	path = get_mtime_path(path);
	만약 (rmdir(path))
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

static 정수 test_if_untracked_cache_is_supported(void)
{
	struct stat st;
	struct stat_data base;
	정수 fd, ret = 0;
	char *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	만약 (!mkdtemp(mtime_dir.buf))
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
	만약 (!match_stat_data(&base, &st)) {
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
	만약 (!match_stat_data(&base, &st)) {
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
	만약 (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		goto done;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	만약 (match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		goto done;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	만약 (!match_stat_data(&base, &st)) {
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
	만약 (!match_stat_data(&base, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		goto done;
	}

	만약 (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

done:
	strbuf_release(&mtime_dir);
	반환 ret;
}

static 정수 mark_ce_flags(const char *path, 정수 flag, 정수 mark)
{
	정수 namelen = strlen(path);
	정수 pos = index_name_pos(the_repository->index, path, namelen);
	만약 (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->index, the_repository->index->cache[pos]);
		만약 (mark)
			the_repository->index->cache[pos]->ce_flags |= flag;
		아니면
			the_repository->index->cache[pos]->ce_flags &= ~flag;
		the_repository->index->cache[pos]->ce_flags |= CE_UPDATE_IN_BASE;
		cache_tree_invalidate_path(the_repository->index, path);
		the_repository->index->cache_changed |= CE_ENTRY_CHANGED;
		반환 0;
	}
	반환 -1;
}

static 정수 remove_one_path(const char *path)
{
	만약 (!allow_remove)
		반환 error("%s: does not exist and --remove not passed", path);
	만약 (remove_file_from_index(the_repository->index, path))
		반환 error("%s: cannot remove from the index", path);
	반환 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
static 정수 process_lstat_error(const char *path, 정수 err)
{
	만약 (is_missing_file_error(err))
		반환 remove_one_path(path);
	반환 error("lstat(\"%s\"): %s", path, strerror(err));
}

static 정수 add_one_path(const struct cache_entry *old, const char *path, 정수 len, struct stat *st)
{
	정수 option;
	struct cache_entry *ce;

	/* Was the old index entry already up-to-date? */
	만약 (old && !ce_stage(old) && !ie_match_stat(the_repository->index, old, st, 0))
		반환 0;

	ce = make_empty_cache_entry(the_repository->index, len);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->index, ce, st);
	ce->ce_mode = ce_mode_from_stat(the_repository, old, st->st_mode);

	만약 (index_path(the_repository->index, &ce->oid, path, st,
		       info_only ? 0 : INDEX_WRITE_OBJECT)) {
		discard_cache_entry(ce);
		반환 -1;
	}
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	만약 (add_index_entry(the_repository->index, ce, option)) {
		discard_cache_entry(ce);
		반환 error("%s: cannot add to the index - missing --add option?", path);
	}
	반환 0;
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
static 정수 process_directory(const char *path, 정수 len, struct stat *st)
{
	struct object_id oid;
	정수 pos = index_name_pos(the_repository->index, path, len);

	/* Exact match: file or existing gitlink */
	만약 (pos >= 0) {
		const struct cache_entry *ce = the_repository->index->cache[pos];
		만약 (S_ISGITLINK(ce->ce_mode)) {

			/* Do nothing to the index if there is no HEAD! */
			만약 (repo_resolve_gitlink_ref(the_repository, path,
						     "HEAD", &oid) < 0)
				반환 0;

			반환 add_one_path(ce, path, len, st);
		}
		/* Should this be an unconditional error? */
		반환 remove_one_path(path);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	동안 (pos < the_repository->index->cache_nr) {
		const struct cache_entry *ce = the_repository->index->cache[pos++];

		만약 (strncmp(ce->name, path, len))
			break;
		만약 (ce->name[len] > '/')
			break;
		만약 (ce->name[len] < '/')
			continue;

		/* Subdirectory match - error out */
		반환 error("%s: is a directory - add individual files instead", path);
	}

	/* No match - should we add it as a gitlink? */
	만약 (!repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid))
		반환 add_one_path(NULL, path, len, st);

	/* Error out. */
	반환 error("%s: is a directory - add files inside instead", path);
}

static 정수 process_path(const char *path, struct stat *st, 정수 stat_errno)
{
	정수 pos, len;
	const struct cache_entry *ce;

	len = strlen(path);
	만약 (has_symlink_leading_path(path, len))
		반환 error("'%s' is beyond a symbolic link", path);

	pos = index_name_pos(the_repository->index, path, len);
	ce = pos < 0 ? NULL : the_repository->index->cache[pos];
	만약 (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		만약 (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_index(the_repository->index, path))
			반환 error("%s: cannot remove from the index", path);
		반환 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	만약 (stat_errno)
		반환 process_lstat_error(path, stat_errno);

	만약 (S_ISDIR(st->st_mode))
		반환 process_directory(path, len, st);

	반환 add_one_path(ce, path, len, st);
}

static 정수 add_cacheinfo(unsigned 정수 mode, const struct object_id *oid,
			 const char *path, 정수 stage)
{
	정수 len, option;
	struct cache_entry *ce;

	만약 (!verify_path(path, mode))
		반환 error("Invalid path '%s'", path);

	len = strlen(path);
	ce = make_empty_cache_entry(the_repository->index, len);

	oidcpy(&ce->oid, oid);
	memcpy(ce->name, path, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_mode = create_ce_mode(mode);
	만약 (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	만약 (add_index_entry(the_repository->index, ce, option))
		반환 error("%s: cannot add to the index - missing --add option?",
			     path);
	report("add '%s'", path);
	반환 0;
}

static void chmod_path(char flip, const char *path)
{
	정수 pos;
	struct cache_entry *ce;

	pos = index_name_pos(the_repository->index, path, strlen(path));
	만약 (pos < 0)
		goto fail;
	ce = the_repository->index->cache[pos];
	만약 (chmod_index_entry(the_repository->index, ce, flip) < 0)
		goto fail;

	report("chmod %cx '%s'", flip, path);
	반환;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, path);
}

static void update_one(const char *path)
{
	정수 stat_errno = 0;
	struct stat st;

	만약 (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_mode = 0;
	아니면 만약 (lstat(path, &st) < 0) {
		st.st_mode = 0;
		stat_errno = errno;
	} /* else stat is valid */

	만약 (!verify_path(path, st.st_mode)) {
		fprintf(stderr, "Ignoring path %s\n", path);
		반환;
	}
	만약 (mark_valid_only) {
		만약 (mark_ce_flags(path, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		반환;
	}
	만약 (mark_skip_worktree_only) {
		만약 (mark_ce_flags(path, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		반환;
	}
	만약 (mark_fsmonitor_only) {
		만약 (mark_ce_flags(path, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		반환;
	}

	만약 (force_remove) {
		만약 (remove_file_from_index(the_repository->index, path))
			die("git update-index: unable to remove %s", path);
		report("remove '%s'", path);
		반환;
	}
	만약 (process_path(path, &st, stat_errno))
		die("Unable to process path %s", path);
	report("add '%s'", path);
}

static void read_index_info(정수 nul_term_line)
{
	const 정수 hexsz = the_hash_algo->hexsz;
	struct strbuf buf = STRBUF_INIT;
	struct strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
	동안 (getline_fn(&buf, stdin) != EOF) {
		char *ptr, *tab;
		char *path_name;
		struct object_id oid;
		unsigned 정수 mode;
		unsigned long ul;
		정수 stage;

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
		만약 (ptr == buf.buf || *ptr != ' '
		    || errno || (unsigned 정수) ul != ul)
			goto bad_line;
		mode = ul;

		tab = strchr(ptr, '\t');
		만약 (!tab || tab - ptr < hexsz + 1)
			goto bad_line;

		만약 (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		아니면 {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		만약 (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			goto bad_line;

		path_name = ptr;
		만약 (!nul_term_line && path_name[0] == '"') {
			strbuf_reset(&uq);
			만약 (unquote_c_style(&uq, path_name, NULL)) {
				die("git update-index: bad quoting of path name");
			}
			path_name = uq.buf;
		}

		만약 (!verify_path(path_name, mode)) {
			fprintf(stderr, "Ignoring path %s\n", path_name);
			continue;
		}

		만약 (!mode) {
			/* mode == 0 means there is no such path -- remove */
			만약 (remove_file_from_index(the_repository->index, path_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		아니면 {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			만약 (add_cacheinfo(mode, &oid, path_name, stage))
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

static const char * const update_index_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	NULL
};

static struct cache_entry *read_one_ent(const char *which,
					struct object_id *ent, const char *path,
					정수 namelen, 정수 stage)
{
	unsigned short mode;
	struct object_id oid;
	struct cache_entry *ce;

	만약 (get_tree_entry(the_repository, ent, path, &oid, &mode)) {
		만약 (which)
			error("%s: not in %s branch.", path, which);
		반환 NULL;
	}
	만약 (!the_repository->index->sparse_index && mode == S_IFDIR) {
		만약 (which)
			error("%s: not a blob in %s branch.", path, which);
		반환 NULL;
	}
	ce = make_empty_cache_entry(the_repository->index, namelen);

	oidcpy(&ce->oid, &oid);
	memcpy(ce->name, path, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_mode = create_ce_mode(mode);
	반환 ce;
}

static 정수 unresolve_one(const char *path)
{
	struct string_list_item *item;
	정수 res = 0;

	만약 (!the_repository->index->resolve_undo)
		반환 res;
	item = string_list_lookup(the_repository->index->resolve_undo, path);
	만약 (!item)
		반환 res; /* no resolve-undo record for the path */
	res = unmerge_index_entry(the_repository->index, path, item->util, 0);
	FREE_AND_NULL(item->util);
	반환 res;
}

static 정수 do_unresolve(정수 ac, const char **av,
			const char *prefix, 정수 prefix_length)
{
	정수 i;
	정수 err = 0;

	위해 (i = 1; i < ac; i++) {
		const char *arg = av[i];
		char *p = prefix_path(the_repository, prefix, prefix_length, arg);
		err |= unresolve_one(p);
		free(p);
	}
	반환 err;
}

static 정수 do_reupdate(const char **paths,
		       const char *prefix)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	정수 pos;
	정수 has_head = 1;
	struct pathspec pathspec;
	struct object_id head_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, paths);

	만약 (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &head_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_head = 0;
 redo:
	위해 (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		const struct cache_entry *ce = the_repository->index->cache[pos];
		struct cache_entry *old = NULL;
		정수 save_nr;
		char *path;

		만약 (ce_stage(ce) || !ce_path_match(the_repository->index, ce, &pathspec, NULL))
			continue;
		만약 (has_head)
			old = read_one_ent(NULL, &head_oid,
					   ce->name, ce_namelen(ce), 0);
		만약 (old && ce->ce_mode == old->ce_mode &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_entry(old);
			continue; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		만약 (S_ISSPARSEDIR(ce->ce_mode)) {
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
		만약 (save_nr != the_repository->index->cache_nr)
			goto redo;
	}
	clear_pathspec(&pathspec);
	반환 0;
}

struct refresh_params {
	unsigned 정수 flags;
	정수 *has_errors;
};

static 정수 refresh(struct refresh_params *o, unsigned 정수 flag)
{
	setup_work_tree(the_repository);
	repo_read_index(the_repository);
	*o->has_errors |= refresh_index(the_repository->index, o->flags | flag, NULL,
					NULL, NULL);
	만약 (has_racy_timestamp(the_repository->index)) {
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
	반환 0;
}

static 정수 refresh_callback(const struct option *opt,
				const char *arg, 정수 unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	반환 refresh(opt->value, 0);
}

static 정수 really_refresh_callback(const struct option *opt,
				const char *arg, 정수 unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	반환 refresh(opt->value, REFRESH_REALLY);
}

static 정수 chmod_callback(const struct option *opt,
				const char *arg, 정수 unset)
{
	char *flip = opt->value;
	BUG_ON_OPT_NEG(unset);
	만약 ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		반환 error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	반환 0;
}

static 정수 resolve_undo_clear_callback(const struct option *opt UNUSED,
				const char *arg, 정수 unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_index(the_repository->index);
	반환 0;
}

static 정수 parse_new_style_cacheinfo(const char *arg,
				     unsigned 정수 *mode,
				     struct object_id *oid,
				     const char **path)
{
	unsigned long ul;
	char *endp;
	const char *p;

	만약 (!arg)
		반환 -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	만약 (errno || endp == arg || *endp != ',' || (unsigned 정수) ul != ul)
		반환 -1; /* not a new-style cacheinfo */
	*mode = ul;
	endp++;
	만약 (parse_oid_hex(endp, oid, &p) || *p != ',')
		반환 -1;
	*path = p + 1;
	반환 0;
}

static enum parse_opt_result cacheinfo_callback(
	struct parse_opt_ctx_t *ctx, const struct option *opt UNUSED,
	const char *arg, 정수 unset)
{
	struct object_id oid;
	unsigned 정수 mode;
	const char *path;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	만약 (!parse_new_style_cacheinfo(ctx->argv[1], &mode, &oid, &path)) {
		만약 (add_cacheinfo(mode, &oid, path, 0))
			die("git update-index: --cacheinfo cannot add %s", path);
		ctx->argv++;
		ctx->argc--;
		반환 0;
	}
	만약 (ctx->argc <= 3)
		반환 error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	만약 (strtoul_ui(*++ctx->argv, 8, &mode) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    add_cacheinfo(mode, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	반환 0;
}

static enum parse_opt_result stdin_cacheinfo_callback(
	struct parse_opt_ctx_t *ctx, const struct option *opt,
	const char *arg, 정수 unset)
{
	정수 *nul_term_line = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	만약 (ctx->argc != 1)
		반환 error("option '%s' must be the last argument", opt->long_name);
	allow_add = allow_replace = allow_remove = 1;
	read_index_info(*nul_term_line);
	반환 0;
}

static enum parse_opt_result stdin_callback(
	struct parse_opt_ctx_t *ctx, const struct option *opt,
	const char *arg, 정수 unset)
{
	정수 *read_from_stdin = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	만약 (ctx->argc != 1)
		반환 error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	반환 0;
}

static enum parse_opt_result unresolve_callback(
	struct parse_opt_ctx_t *ctx, const struct option *opt,
	const char *arg, 정수 unset)
{
	정수 *has_errors = opt->value;
	const char *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				prefix, prefix ? strlen(prefix) : 0);
	만약 (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	반환 0;
}

static enum parse_opt_result reupdate_callback(
	struct parse_opt_ctx_t *ctx, const struct option *opt,
	const char *arg, 정수 unset)
{
	정수 *has_errors = opt->value;
	const char *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	setup_work_tree(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, prefix);
	만약 (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	반환 0;
}

정수 cmd_update_index(정수 argc,
		     const char **argv,
		     const char *prefix,
		     struct repository *repo UNUSED)
{
	정수 newfd, entries, has_errors = 0, nul_term_line = 0;
	enum uc_mode untracked_cache = UC_UNSPECIFIED;
	정수 read_from_stdin = 0;
	정수 prefix_length = prefix ? strlen(prefix) : 0;
	정수 preferred_index_format = 0;
	char set_executable_bit = 0;
	struct refresh_params refresh_args = {0, &has_errors};
	정수 lock_error = 0;
	정수 split_index = -1;
	정수 force_write = 0;
	정수 fsmonitor = -1;
	struct lock_file lock_file = LOCK_INIT;
	struct parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	정수 parseopt_state = PARSE_OPT_UNKNOWN;
	struct repository *r = the_repository;
	struct odb_transaction *transaction;
	struct option options[] = {
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
			.precision = sizeof(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-assume-unchanged",
			.value = &mark_valid_only,
			.precision = sizeof(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "skip-worktree",
			.value = &mark_skip_worktree_only,
			.precision = sizeof(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-skip-worktree",
			.value = &mark_skip_worktree_only,
			.precision = sizeof(mark_skip_worktree_only),
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
			.precision = sizeof(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-fsmonitor-valid",
			.value = &mark_fsmonitor_only,
			.precision = sizeof(mark_fsmonitor_only),
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
	만약 (newfd < 0)
		lock_error = errno;

	entries = repo_read_index(the_repository);
	만약 (entries < 0)
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
	동안 (ctx.argc) {
		만약 (parseopt_state != PARSE_OPT_DONE)
			parseopt_state = parse_options_step(&ctx, options,
							    update_index_usage);
		만약 (!ctx.argc)
			break;
		switch (parseopt_state) {
		case PARSE_OPT_HELP:
			exit(0);
		case PARSE_OPT_HELP_ERROR:
		case PARSE_OPT_ERROR:
			exit(129);
		case PARSE_OPT_COMPLETE:
			exit(0);
		case PARSE_OPT_NON_OPTION:
		case PARSE_OPT_DONE:
		{
			const char *path = ctx.argv[0];
			char *p;

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
			만약 (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = NULL;
			}

			setup_work_tree(the_repository);
			p = prefix_path(the_repository, prefix, prefix_length, path);
			update_one(p);
			만약 (set_executable_bit)
				chmod_path(set_executable_bit, p);
			free(p);
			ctx.argc--;
			ctx.argv++;
			break;
		}
		case PARSE_OPT_UNKNOWN:
			만약 (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			아니면
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_index_usage, options);
		}
	}
	argc = parse_options_end(&ctx);

	getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
	만약 (preferred_index_format) {
		만약 (preferred_index_format < 0) {
			printf(_("%d\n"), the_repository->index->version);
		} 아니면 만약 (preferred_index_format < INDEX_FORMAT_LB ||
			   INDEX_FORMAT_UB < preferred_index_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_index_format,
			    INDEX_FORMAT_LB, INDEX_FORMAT_UB);
		} 아니면 {
			만약 (the_repository->index->version != preferred_index_format)
				the_repository->index->cache_changed |= SOMETHING_CHANGED;
			report(_("index-version: was %d, set to %d"),
			       the_repository->index->version, preferred_index_format);
			the_repository->index->version = preferred_index_format;
		}
	}

	만약 (read_from_stdin) {
		struct strbuf buf = STRBUF_INIT;
		struct strbuf unquoted = STRBUF_INIT;

		setup_work_tree(the_repository);
		동안 (getline_fn(&buf, stdin) != EOF) {
			char *p;
			만약 (!nul_term_line && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				만약 (unquote_c_style(&unquoted, buf.buf, NULL))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(the_repository, prefix, prefix_length, buf.buf);
			update_one(p);
			만약 (set_executable_bit)
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

	만약 (split_index > 0) {
		만약 (repo_config_get_split_index(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		만약 (the_repository->index->split_index)
			the_repository->index->cache_changed |= SPLIT_INDEX_ORDERED;
		아니면
			add_split_index(the_repository->index);
	} 아니면 만약 (!split_index) {
		만약 (repo_config_get_split_index(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_split_index(the_repository->index);
	}

	prepare_repo_settings(r);
	switch (untracked_cache) {
	case UC_UNSPECIFIED:
		break;
	case UC_DISABLE:
		만약 (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->index);
		report(_("Untracked cache disabled"));
		break;
	case UC_TEST:
		setup_work_tree(the_repository);
		반환 !test_if_untracked_cache_is_supported();
	case UC_ENABLE:
	case UC_FORCE:
		만약 (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		add_untracked_cache(the_repository->index);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_tree(the_repository));
		break;
	default:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	만약 (fsmonitor > 0) {
		enum fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		enum fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		만약 (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings__get_incompatible_msg(r, reason));

		만약 (fsm_mode == FSMONITOR_MODE_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		add_fsmonitor(the_repository->index);
		report(_("fsmonitor enabled"));
	} 아니면 만약 (!fsmonitor) {
		enum fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		만약 (fsm_mode > FSMONITOR_MODE_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->index);
		report(_("fsmonitor disabled"));
	}

	만약 (the_repository->index->cache_changed || force_write) {
		만약 (newfd < 0) {
			만약 (refresh_args.flags & REFRESH_QUIET)
				exit(128);
			unable_to_lock_die(repo_get_index_file(the_repository), lock_error);
		}
		만약 (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die("Unable to write new index file");
	}

	rollback_lock_file(&lock_file);

	반환 has_errors ? 1 : 0;
}
