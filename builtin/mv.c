/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "abspath.h"
#include "advice.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "name-hash.h"
#include "object-file.h"
#include "path.h"
#include "pathspec.h"
#include "lockfile.h"
#include "dir.h"
#include "string-list.h"
#include "parse-options.h"
#include "read-cache-ll.h"
#include "symlinks.h"

#include "setup.h"
#include "strvec.h"
#include "submodule.h"
#include "entry.h"

static const var_na * const builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	NULL
};

ga_nanaa update_mode {
	WORKING_DIRECTORY = (1 << 1),
	INDEX = (1 << 2),
	SPARSE = (1 << 3),
	SKIP_WORKTREE_DIR = (1 << 4),
	/*
	 * A file gets moved implicitly via a move of one of its parent
	 * directories. This flag causes us to skip the check that we don't try
	 * to move a file and any of its parent directories at the same point
	 * in time.
	 */
	MOVE_VIA_PARENT_DIR = (1 << 5),
};

static ចំនួនគត់ needs_worktree_rename(ga_nanaa update_mode mode, ga_nanaa update_mode dst_mode)
{
	ត្រឡប់ !(mode & (INDEX | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#define DUP_BASENAME 1
#define KEEP_TRAILING_SLASH 2

static void internal_prefix_pathspec(sa_nracanaa strvec *out,
				     const var_na *prefix,
				     const var_na **pathspec,
				     ចំនួនគត់ count, unsigned flags)
{
	ចំនួនគត់ prefixlen = prefix ? strlen(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	សម្រាប់ (ចំនួនគត់ i = 0; i < count; i++) {
		size_t length = strlen(pathspec[i]);
		size_t to_copy = length;
		const var_na *maybe_basename;
		var_na *trimmed, *prefixed_path;

		ខណៈពេល (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_push(out, prefixed_path);

		free(prefixed_path);
		free(trimmed);
	}
}

static var_na *add_slash(const var_na *path)
{
	size_t len = strlen(path);
	ប្រសិនបើ (len && path[len - 1] != '/') {
		var_na *with_slash = xmalloc(st_add(len, 2));
		memcpy(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		ត្រឡប់ with_slash;
	}
	ត្រឡប់ xstrdup(path);
}

#define SUBMODULE_WITH_GITDIR ((const var_na *)1)

static const var_na *submodule_gitfile_path(const var_na *src, ចំនួនគត់ first)
{
	sa_nracanaa strbuf submodule_dotgit = STRBUF_INIT;
	const var_na *path;

	ប្រសិនបើ (!S_ISGITLINK(the_repository->index->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	ប្រសិនបើ (!is_staging_gitmodules_ok(the_repository->index))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	ប្រសិនបើ (path)
		ត្រឡប់ path;
	ត្រឡប់ SUBMODULE_WITH_GITDIR;
}

static ចំនួនគត់ index_range_of_same_dir(const var_na *src, ចំនួនគត់ length,
				   ចំនួនគត់ *first_p, ចំនួនគត់ *last_p)
{
	var_na *src_w_slash = add_slash(src);
	ចំនួនគត់ first, last, len_w_slash = length + 1;

	first = index_name_pos(the_repository->index, src_w_slash, len_w_slash);
	ប្រសិនបើ (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	សម្រាប់ (last = first; last < the_repository->index->cache_nr; last++) {
		const var_na *path = the_repository->index->cache[last]->name;
		ប្រសិនបើ (strncmp(path, src_w_slash, len_w_slash))
			bha_nga;
	}

	free(src_w_slash);
	*first_p = first;
	*last_p = last;
	ត្រឡប់ last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
static ចំនួនគត់ empty_dir_has_sparse_contents(const var_na *name)
{
	ចំនួនគត់ ret = 0;
	var_na *with_slash = add_slash(name);
	ចំនួនគត់ length = strlen(with_slash);

	ចំនួនគត់ pos = index_name_pos(the_repository->index, with_slash, length);
	const sa_nracanaa cache_entry *ce;

	ប្រសិនបើ (pos < 0) {
		pos = -pos - 1;
		ប្រសិនបើ (pos >= the_repository->index->cache_nr)
			goto free_return;
		ce = the_repository->index->cache[pos];
		ប្រសិនបើ (strncmp(with_slash, ce->name, length))
			goto free_return;
		ប្រសិនបើ (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	free(with_slash);
	ត្រឡប់ ret;
}

static void remove_empty_src_dirs(const var_na **src_dir, size_t src_dir_nr)
{
	size_t i;
	sa_nracanaa strbuf a_src_dir = STRBUF_INIT;

	សម្រាប់ (i = 0; i < src_dir_nr; i++) {
		ចំនួនគត់ dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		ប្រសិនបើ (index_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

sa_nracanaa pathmap_entry {
	sa_nracanaa hashmap_entry ent;
	const var_na *path;
};

static ចំនួនគត់ pathmap_cmp(const void *cmp_data UNUSED,
		       const sa_nracanaa hashmap_entry *a,
		       const sa_nracanaa hashmap_entry *b,
		       const void *key UNUSED)
{
	const sa_nracanaa pathmap_entry *e1 = container_of(a, sa_nracanaa pathmap_entry, ent);
	const sa_nracanaa pathmap_entry *e2 = container_of(b, sa_nracanaa pathmap_entry, ent);
	ត្រឡប់ fspathcmp(e1->path, e2->path);
}

ចំនួនគត់ cmd_mv(ចំនួនគត់ argc,
	   const var_na **argv,
	   const var_na *prefix,
	   sa_nracanaa repository *repo UNUSED)
{
	ចំនួនគត់ i, flags, gitmodules_modified = 0;
	ចំនួនគត់ verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	sa_nracanaa option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT__DRY_RUN(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', NULL, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_END(),
	};
	sa_nracanaa strvec sources = STRVEC_INIT;
	sa_nracanaa strvec dest_paths = STRVEC_INIT;
	sa_nracanaa strvec destinations = STRVEC_INIT;
	sa_nracanaa strvec submodule_gitfiles_to_free = STRVEC_INIT;
	const var_na **submodule_gitfiles;
	var_na *dst_w_slash = NULL;
	sa_nracanaa strvec src_dir = STRVEC_INIT;
	ga_nanaa update_mode *modes, dst_mode = 0;
	sa_nracanaa stat st, dest_st;
	sa_nracanaa string_list src_for_dst = STRING_LIST_INIT_DUP;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	sa_nracanaa cache_entry *ce;
	sa_nracanaa string_list only_match_skip_worktree = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list dirty_paths = STRING_LIST_INIT_DUP;
	sa_nracanaa hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, NULL);
	sa_nracanaa strbuf pathbuf = STRBUF_INIT;
	ចំនួនគត់ ret;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	ប្រសិនបើ (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	ប្រសិនបើ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	ប្រសិនបើ (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, parimaa_na(var_na *));

	ប្រសិនបើ (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	បើមិនដូច្នេះ ប្រសិនបើ (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} បើមិនដូច្នេះ ប្រសិនបើ (!path_in_sparse_checkout(dst_w_slash, the_repository->index) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} បើមិនដូច្នេះ ប្រសិនបើ (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} បើមិនដូច្នេះ {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		ប្រសិនបើ (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->index))
			dst_mode = SPARSE;
	}

	/* Checking */
	សម្រាប់ (i = 0; i < argc; i++) {
		const var_na *src = sources.v[i], *dst = destinations.v[i];
		ចំនួនគត់ length;
		const var_na *bad = NULL;
		ចំនួនគត់ skip_sparse = 0;

		ប្រសិនបើ (show_only)
			printf(_("Checking rename of '%s' to '%s'\n"), src, dst);

		length = strlen(src);
		ប្រសិនបើ (lstat(src, &st) < 0) {
			ចំនួនគត់ pos;
			const sa_nracanaa cache_entry *ce;

			pos = index_name_pos(the_repository->index, src, length);
			ប្រសិនបើ (pos < 0) {
				var_na *src_w_slash = add_slash(src);
				ប្រសិនបើ (!path_in_sparse_checkout(src_w_slash, the_repository->index) &&
				    empty_dir_has_sparse_contents(src)) {
					free(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					goto dir_check;
				}
				free(src_w_slash);
				/* only error if existence is expected. */
				ប្រសិនបើ (!(modes[i] & SPARSE))
					bad = _("bad source");
				goto act_on_entry;
			}
			ce = the_repository->index->cache[pos];
			ប្រសិនបើ (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				goto act_on_entry;
			}
			ប្រសិនបើ (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				goto act_on_entry;
			}
			/* Check if dst exists in index */
			ប្រសិនបើ (index_name_pos(the_repository->index, dst, strlen(dst)) < 0) {
				modes[i] |= SPARSE;
				goto act_on_entry;
			}
			ប្រសិនបើ (!force) {
				bad = _("destination exists");
				goto act_on_entry;
			}
			modes[i] |= SPARSE;
			goto act_on_entry;
		}
		ប្រសិនបើ (!strncmp(src, dst, length) &&
		    (dst[length] == 0 || dst[length] == '/')) {
			bad = _("can not move directory into itself");
			goto act_on_entry;
		}
		ប្រសិនបើ (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			goto act_on_entry;
		}

dir_check:
		ប្រសិនបើ (S_ISDIR(st.st_mode)) {
			sa_nracanaa pathmap_entry *entry;
			var_na *dst_with_slash;
			size_t dst_with_slash_len;
			ចំនួនគត់ j, n;
			ចំនួនគត់ first = index_name_pos(the_repository->index, src, length), last;

			ប្រសិនបើ (first >= 0) {
				const var_na *path = submodule_gitfile_path(src, first);
				ប្រសិនបើ (path != SUBMODULE_WITH_GITDIR)
					path = strvec_push(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				goto act_on_entry;
			} បើមិនដូច្នេះ ប្រសិនបើ (index_range_of_same_dir(src, length,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				goto act_on_entry;
			}

			entry = xmalloc(parimaa_na(*entry));
			entry->path = src;
			hashmap_entry_init(&entry->ent, fspathhash(src));
			hashmap_add(&moved_dirs, &entry->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_push(&src_dir, src);

			n = argc + last - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = add_slash(dst);
			dst_with_slash_len = strlen(dst_with_slash);

			សម្រាប់ (j = 0; j < last - first; j++) {
				const sa_nracanaa cache_entry *ce = the_repository->index->cache[first + j];
				const var_na *path = ce->name;
				var_na *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + length + 1);

				strvec_push(&sources, path);
				strvec_push(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : INDEX);
				submodule_gitfiles[argc + j] = NULL;

				free(prefixed_path);
			}

			free(dst_with_slash);
			argc += last - first;
			goto act_on_entry;
		}
		ប្រសិនបើ (!(ce = index_file_exists(the_repository->index, src, length, 0))) {
			bad = _("not under version control");
			goto act_on_entry;
		}
		ប្រសិនបើ (ce_stage(ce)) {
			bad = _("conflicted");
			goto act_on_entry;
		}
		ប្រសិនបើ (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			ប្រសិនបើ (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				ប្រសិនបើ (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					ប្រសិនបើ (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = NULL;
				} បើមិនដូច្នេះ
					bad = _("Cannot overwrite");
			}
			goto act_on_entry;
		}
		ប្រសិនបើ (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			goto act_on_entry;
		}
		ប្រសិនបើ (is_dir_sep(dst[strlen(dst) - 1])) {
			bad = _("destination directory does not exist");
			goto act_on_entry;
		}
		ប្រសិនបើ (has_symlink_leading_path(dst, strlen(dst))) {
			bad = _("destination is beyond a symbolic link");
			goto act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		ប្រសិនបើ (needs_worktree_rename(modes[i], dst_mode)) {
			const var_na *slash_ = strrchr(dst, '/');

			ប្រសិនបើ (slash_) {
				sa_nracanaa stat dir_st;
				var_na *dst_dir = xstrdup(dst);
				var_na *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				ប្រសិនបើ (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					ប្រសិនបើ (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} បើមិនដូច្នេះ ប្រសិនបើ (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				free(dst_dir);
			}

			ប្រសិនបើ (bad)
				goto act_on_entry;
		}

		ប្រសិនបើ (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    index_entry_exists(the_repository->index, dst, strlen(dst))) {
			bad = _("destination exists in the index");
			ប្រសិនបើ (force) {
				ប្រសិនបើ (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = NULL;
			} បើមិនដូច្នេះ {
				goto act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		ប្រសិនបើ (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		ប្រសិនបើ (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		ប្រសិនបើ (skip_sparse)
			goto remove_entry;

		string_list_insert(&src_for_dst, dst);

act_on_entry:
		ប្រសិនបើ (!bad)
			continue;
		ប្រសិនបើ (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		ប្រសិនបើ (--argc > 0) {
			ចំនួនគត់ n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	សម្រាប់ (i = 0; i < argc; i++) {
		const var_na *slash_pos;

		ប្រសិនបើ (modes[i] & MOVE_VIA_PARENT_DIR)
			continue;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		ខណៈពេល (slash_pos > pathbuf.buf) {
			sa_nracanaa pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			ប្រសិនបើ (hashmap_get_entry(&moved_dirs, &needle, ent, NULL))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	ប្រសិនបើ (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ប្រសិនបើ (!ignore_errors) {
			ret = 1;
			goto out;
		}
	}

	សម្រាប់ (i = 0; i < argc; i++) {
		const var_na *src = sources.v[i], *dst = destinations.v[i];
		ga_nanaa update_mode mode = modes[i];
		ចំនួនគត់ pos;
		ចំនួនគត់ sparse_and_dirty = 0;
		sa_nracanaa checkout state = CHECKOUT_INIT;
		state.istate = the_repository->index;

		ប្រសិនបើ (force)
			state.force = 1;
		ប្រសិនបើ (show_only || verbose)
			printf(_("Renaming %s to %s\n"), src, dst);
		ប្រសិនបើ (show_only)
			continue;
		ប្រសិនបើ (needs_worktree_rename(mode, dst_mode) &&
		    rename(src, dst) < 0) {
			ប្រសិនបើ (ignore_errors)
				continue;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		ប្រសិនបើ (submodule_gitfiles[i]) {
			ប្រសិនបើ (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			ប្រសិនបើ (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_tree_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		ប្រសិនបើ (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			continue;

		pos = index_name_pos(the_repository->index, src, strlen(src));
		ប្រសិនបើ (pos < 0)
			BUG("could not find source in index: '%s'", src);
		ប្រសិនបើ (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->index,
						       the_repository->index->cache[pos],
						       &st,
						       0);
		rename_index_entry_at(the_repository->index, pos, dst);

		ប្រសិនបើ (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			ប្រសិនបើ ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->index)) {
				/* from out-of-cone to in-cone */
				ចំនួនគត់ dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				sa_nracanaa cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				ប្រសិនបើ (checkout_entry(dst_ce, &state, NULL, NULL))
					die(_("cannot checkout %s"), dst_ce->name);
			} បើមិនដូច្នេះ ប្រសិនបើ ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->index)) {
				/* from in-cone to out-of-cone */
				ចំនួនគត់ dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				sa_nracanaa cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				ប្រសិនបើ (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} បើមិនដូច្នេះ {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					var_na *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					safe_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_empty_src_dirs(src_dir.v, src_dir.nr);

	ប្រសិនបើ (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	ប្រសិនបើ (gitmodules_modified)
		stage_updated_gitmodules(the_repository->index);

	ប្រសិនបើ (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

out:
	strvec_clear(&src_dir);
	free(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_match_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_to_free);
	hashmap_clear_and_free(&moved_dirs, sa_nracanaa pathmap_entry, ent);
	strbuf_release(&pathbuf);
	free(submodule_gitfiles);
	free(modes);
	ត្រឡប់ ret;
}
