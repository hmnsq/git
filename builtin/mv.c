/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "name-hash.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "symlinks.h"

#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "entry.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	ರಿಕ್ತ
};

ಗಣನಾ update_mode {
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ needs_worktree_rename(ಗಣನಾ update_mode mode, ಗಣನಾ update_mode dst_mode)
{
	ಪ್ರತ್ಯಾಗಮ !(mode & (INDEX | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#define DUP_BASENAME 1
#define KEEP_TRAILING_SLASH 2

ಸ್ಥಿರ ಶೂನ್ಯ internal_prefix_pathspec(ಸಂರಚನಾ strvec *out,
				     ಧ್ರುವ ವರ್ಣ *prefix,
				     ಧ್ರುವ ವರ್ಣ **pathspec,
				     ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned flags)
{
	ಪೂರ್ಣಾಙ್ಕ prefixlen = prefix ? strlen(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++) {
		size_t length = strlen(pathspec[i]);
		size_t to_copy = length;
		ಧ್ರುವ ವರ್ಣ *maybe_basename;
		ವರ್ಣ *trimmed, *prefixed_path;

		ಯಾವತ್ (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_ತಳ್ಳು(out, prefixed_path);

		ಮುಕ್ತ(prefixed_path);
		ಮುಕ್ತ(trimmed);
	}
}

ಸ್ಥಿರ ವರ್ಣ *add_slash(ಧ್ರುವ ವರ್ಣ *path)
{
	size_t len = strlen(path);
	ಯದಿ (len && path[len - 1] != '/') {
		ವರ್ಣ *with_slash = xmalloc(st_add(len, 2));
		memcpy(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		ಪ್ರತ್ಯಾಗಮ with_slash;
	}
	ಪ್ರತ್ಯಾಗಮ xstrdup(path);
}

#define SUBMODULE_WITH_GITDIR ((ಧ್ರುವ ವರ್ಣ *)1)

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *submodule_gitfile_path(ಧ್ರುವ ವರ್ಣ *src, ಪೂರ್ಣಾಙ್ಕ first)
{
	ಸಂರಚನಾ strbuf submodule_dotgit = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *path;

	ಯದಿ (!S_ISGITLINK(the_repository->index->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	ಯದಿ (!is_staging_gitmodules_ok(the_repository->index))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	ಯದಿ (path)
		ಪ್ರತ್ಯಾಗಮ path;
	ಪ್ರತ್ಯಾಗಮ SUBMODULE_WITH_GITDIR;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ index_range_of_same_dir(ಧ್ರುವ ವರ್ಣ *src, ಪೂರ್ಣಾಙ್ಕ length,
				   ಪೂರ್ಣಾಙ್ಕ *first_p, ಪೂರ್ಣಾಙ್ಕ *last_p)
{
	ವರ್ಣ *src_w_slash = add_slash(src);
	ಪೂರ್ಣಾಙ್ಕ first, last, len_w_slash = length + 1;

	first = index_name_pos(the_repository->index, src_w_slash, len_w_slash);
	ಯದಿ (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	ಪ್ರತಿ (last = first; last < the_repository->index->cache_nr; last++) {
		ಧ್ರುವ ವರ್ಣ *path = the_repository->index->cache[last]->name;
		ಯದಿ (strncmp(path, src_w_slash, len_w_slash))
			ಭಙ್ಗ;
	}

	ಮುಕ್ತ(src_w_slash);
	*first_p = first;
	*last_p = last;
	ಪ್ರತ್ಯಾಗಮ last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ empty_dir_has_sparse_contents(ಧ್ರುವ ವರ್ಣ *name)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ವರ್ಣ *with_slash = add_slash(name);
	ಪೂರ್ಣಾಙ್ಕ length = strlen(with_slash);

	ಪೂರ್ಣಾಙ್ಕ pos = index_name_pos(the_repository->index, with_slash, length);
	ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce;

	ಯದಿ (pos < 0) {
		pos = -pos - 1;
		ಯದಿ (pos >= the_repository->index->cache_nr)
			ಗಚ್ಛ free_return;
		ce = the_repository->index->cache[pos];
		ಯದಿ (strncmp(with_slash, ce->name, length))
			ಗಚ್ಛ free_return;
		ಯದಿ (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	ಮುಕ್ತ(with_slash);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_empty_src_dirs(ಧ್ರುವ ವರ್ಣ **src_dir, size_t src_dir_nr)
{
	size_t i;
	ಸಂರಚನಾ strbuf a_src_dir = STRBUF_INIT;

	ಪ್ರತಿ (i = 0; i < src_dir_nr; i++) {
		ಪೂರ್ಣಾಙ್ಕ dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		ಯದಿ (index_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

ಸಂರಚನಾ pathmap_entry {
	ಸಂರಚನಾ hashmap_entry ent;
	ಧ್ರುವ ವರ್ಣ *path;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pathmap_cmp(ಧ್ರುವ ಶೂನ್ಯ *cmp_data UNUSED,
		       ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *a,
		       ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *b,
		       ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ UNUSED)
{
	ಧ್ರುವ ಸಂರಚನಾ pathmap_entry *e1 = container_of(a, ಸಂರಚನಾ pathmap_entry, ent);
	ಧ್ರುವ ಸಂರಚನಾ pathmap_entry *e2 = container_of(b, ಸಂರಚನಾ pathmap_entry, ent);
	ಪ್ರತ್ಯಾಗಮ fspathcmp(e1->path, e2->path);
}

ಪೂರ್ಣಾಙ್ಕ cmd_mv(ಪೂರ್ಣಾಙ್ಕ argc,
	   ಧ್ರುವ ವರ್ಣ **argv,
	   ಧ್ರುವ ವರ್ಣ *prefix,
	   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ i, flags, gitmodules_modified = 0;
	ಪೂರ್ಣಾಙ್ಕ verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	ಸಂರಚನಾ option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT__DRY_RUN(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', ರಿಕ್ತ, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_END(),
	};
	ಸಂರಚನಾ strvec sources = STRVEC_INIT;
	ಸಂರಚನಾ strvec dest_paths = STRVEC_INIT;
	ಸಂರಚನಾ strvec destinations = STRVEC_INIT;
	ಸಂರಚನಾ strvec submodule_gitfiles_to_free = STRVEC_INIT;
	ಧ್ರುವ ವರ್ಣ **submodule_gitfiles;
	ವರ್ಣ *dst_w_slash = ರಿಕ್ತ;
	ಸಂರಚನಾ strvec src_dir = STRVEC_INIT;
	ಗಣನಾ update_mode *modes, dst_mode = 0;
	ಸಂರಚನಾ stat st, dest_st;
	ಸಂರಚನಾ string_list src_for_dst = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಸಂರಚನಾ cache_entry *ce;
	ಸಂರಚನಾ string_list only_match_skip_worktree = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ string_list dirty_paths = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, ರಿಕ್ತ);
	ಸಂರಚನಾ strbuf pathbuf = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	ಯದಿ (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	ಯದಿ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	ಯದಿ (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, ಪರಿಮಾಣ(ವರ್ಣ *));

	ಯದಿ (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	ಅನ್ಯಥಾ ಯದಿ (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} ಅನ್ಯಥಾ ಯದಿ (!path_in_sparse_checkout(dst_w_slash, the_repository->index) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} ಅನ್ಯಥಾ ಯದಿ (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} ಅನ್ಯಥಾ {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		ಯದಿ (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->index))
			dst_mode = SPARSE;
	}

	/* Checking */
	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *src = sources.v[i], *dst = destinations.v[i];
		ಪೂರ್ಣಾಙ್ಕ length;
		ಧ್ರುವ ವರ್ಣ *bad = ರಿಕ್ತ;
		ಪೂರ್ಣಾಙ್ಕ skip_sparse = 0;

		ಯದಿ (show_only)
			printf(_("Checking rename of '%s' to '%s'\n"), src, dst);

		length = strlen(src);
		ಯದಿ (lstat(src, &st) < 0) {
			ಪೂರ್ಣಾಙ್ಕ pos;
			ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce;

			pos = index_name_pos(the_repository->index, src, length);
			ಯದಿ (pos < 0) {
				ವರ್ಣ *src_w_slash = add_slash(src);
				ಯದಿ (!path_in_sparse_checkout(src_w_slash, the_repository->index) &&
				    empty_dir_has_sparse_contents(src)) {
					ಮುಕ್ತ(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					ಗಚ್ಛ dir_check;
				}
				ಮುಕ್ತ(src_w_slash);
				/* only error if existence is expected. */
				ಯದಿ (!(modes[i] & SPARSE))
					bad = _("bad source");
				ಗಚ್ಛ act_on_entry;
			}
			ce = the_repository->index->cache[pos];
			ಯದಿ (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				ಗಚ್ಛ act_on_entry;
			}
			ಯದಿ (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				ಗಚ್ಛ act_on_entry;
			}
			/* Check if dst exists in index */
			ಯದಿ (index_name_pos(the_repository->index, dst, strlen(dst)) < 0) {
				modes[i] |= SPARSE;
				ಗಚ್ಛ act_on_entry;
			}
			ಯದಿ (!force) {
				bad = _("destination exists");
				ಗಚ್ಛ act_on_entry;
			}
			modes[i] |= SPARSE;
			ಗಚ್ಛ act_on_entry;
		}
		ಯದಿ (!strncmp(src, dst, length) &&
		    (dst[length] == 0 || dst[length] == '/')) {
			bad = _("can not move directory into itself");
			ಗಚ್ಛ act_on_entry;
		}
		ಯದಿ (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			ಗಚ್ಛ act_on_entry;
		}

dir_check:
		ಯದಿ (S_ISDIR(st.st_mode)) {
			ಸಂರಚನಾ pathmap_entry *entry;
			ವರ್ಣ *dst_with_slash;
			size_t dst_with_slash_len;
			ಪೂರ್ಣಾಙ್ಕ j, n;
			ಪೂರ್ಣಾಙ್ಕ first = index_name_pos(the_repository->index, src, length), last;

			ಯದಿ (first >= 0) {
				ಧ್ರುವ ವರ್ಣ *path = submodule_gitfile_path(src, first);
				ಯದಿ (path != SUBMODULE_WITH_GITDIR)
					path = strvec_ತಳ್ಳು(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				ಗಚ್ಛ act_on_entry;
			} ಅನ್ಯಥಾ ಯದಿ (index_range_of_same_dir(src, length,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				ಗಚ್ಛ act_on_entry;
			}

			entry = xmalloc(ಪರಿಮಾಣ(*entry));
			entry->path = src;
			hashmap_entry_init(&entry->ent, fspathhash(src));
			hashmap_add(&moved_dirs, &entry->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_ತಳ್ಳು(&src_dir, src);

			n = argc + last - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = add_slash(dst);
			dst_with_slash_len = strlen(dst_with_slash);

			ಪ್ರತಿ (j = 0; j < last - first; j++) {
				ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = the_repository->index->cache[first + j];
				ಧ್ರುವ ವರ್ಣ *path = ce->name;
				ವರ್ಣ *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + length + 1);

				strvec_ತಳ್ಳು(&sources, path);
				strvec_ತಳ್ಳು(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : INDEX);
				submodule_gitfiles[argc + j] = ರಿಕ್ತ;

				ಮುಕ್ತ(prefixed_path);
			}

			ಮುಕ್ತ(dst_with_slash);
			argc += last - first;
			ಗಚ್ಛ act_on_entry;
		}
		ಯದಿ (!(ce = index_file_exists(the_repository->index, src, length, 0))) {
			bad = _("not under version control");
			ಗಚ್ಛ act_on_entry;
		}
		ಯದಿ (ce_stage(ce)) {
			bad = _("conflicted");
			ಗಚ್ಛ act_on_entry;
		}
		ಯದಿ (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			ಯದಿ (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				ಯದಿ (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					ಯದಿ (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = ರಿಕ್ತ;
				} ಅನ್ಯಥಾ
					bad = _("Cannot overwrite");
			}
			ಗಚ್ಛ act_on_entry;
		}
		ಯದಿ (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			ಗಚ್ಛ act_on_entry;
		}
		ಯದಿ (is_dir_sep(dst[strlen(dst) - 1])) {
			bad = _("destination directory does not exist");
			ಗಚ್ಛ act_on_entry;
		}
		ಯದಿ (has_symlink_leading_path(dst, strlen(dst))) {
			bad = _("destination is beyond a symbolic link");
			ಗಚ್ಛ act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		ಯದಿ (needs_worktree_rename(modes[i], dst_mode)) {
			ಧ್ರುವ ವರ್ಣ *slash_ = strrchr(dst, '/');

			ಯದಿ (slash_) {
				ಸಂರಚನಾ stat dir_st;
				ವರ್ಣ *dst_dir = xstrdup(dst);
				ವರ್ಣ *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				ಯದಿ (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					ಯದಿ (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} ಅನ್ಯಥಾ ಯದಿ (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				ಮುಕ್ತ(dst_dir);
			}

			ಯದಿ (bad)
				ಗಚ್ಛ act_on_entry;
		}

		ಯದಿ (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    index_entry_exists(the_repository->index, dst, strlen(dst))) {
			bad = _("destination exists in the index");
			ಯದಿ (force) {
				ಯದಿ (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = ರಿಕ್ತ;
			} ಅನ್ಯಥಾ {
				ಗಚ್ಛ act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		ಯದಿ (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		ಯದಿ (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		ಯದಿ (skip_sparse)
			ಗಚ್ಛ remove_entry;

		string_list_ಸೇರಿಸು(&src_for_dst, dst);

act_on_entry:
		ಯದಿ (!bad)
			ಅನುವರ್ತ;
		ಯದಿ (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		ಯದಿ (--argc > 0) {
			ಪೂರ್ಣಾಙ್ಕ n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *slash_pos;

		ಯದಿ (modes[i] & MOVE_VIA_PARENT_DIR)
			ಅನುವರ್ತ;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		ಯಾವತ್ (slash_pos > pathbuf.buf) {
			ಸಂರಚನಾ pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			ಯದಿ (hashmap_get_entry(&moved_dirs, &needle, ent, ರಿಕ್ತ))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	ಯದಿ (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ಯದಿ (!ignore_errors) {
			ret = 1;
			ಗಚ್ಛ out;
		}
	}

	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *src = sources.v[i], *dst = destinations.v[i];
		ಗಣನಾ update_mode mode = modes[i];
		ಪೂರ್ಣಾಙ್ಕ pos;
		ಪೂರ್ಣಾಙ್ಕ sparse_and_dirty = 0;
		ಸಂರಚನಾ checkout state = CHECKOUT_INIT;
		state.istate = the_repository->index;

		ಯದಿ (force)
			state.force = 1;
		ಯದಿ (show_only || verbose)
			printf(_("Renaming %s to %s\n"), src, dst);
		ಯದಿ (show_only)
			ಅನುವರ್ತ;
		ಯದಿ (needs_worktree_rename(mode, dst_mode) &&
		    rename(src, dst) < 0) {
			ಯದಿ (ignore_errors)
				ಅನುವರ್ತ;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		ಯದಿ (submodule_gitfiles[i]) {
			ಯದಿ (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			ಯದಿ (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_ವೃಕ್ಷ_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		ಯದಿ (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			ಅನುವರ್ತ;

		pos = index_name_pos(the_repository->index, src, strlen(src));
		ಯದಿ (pos < 0)
			BUG("could not find source in index: '%s'", src);
		ಯದಿ (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->index,
						       the_repository->index->cache[pos],
						       &st,
						       0);
		rename_index_entry_at(the_repository->index, pos, dst);

		ಯದಿ (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			ಯದಿ ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->index)) {
				/* from out-of-cone to in-cone */
				ಪೂರ್ಣಾಙ್ಕ dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				ಸಂರಚನಾ cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				ಯದಿ (checkout_entry(dst_ce, &state, ರಿಕ್ತ, ರಿಕ್ತ))
					die(_("cannot checkout %s"), dst_ce->name);
			} ಅನ್ಯಥಾ ಯದಿ ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->index)) {
				/* from in-cone to out-of-cone */
				ಪೂರ್ಣಾಙ್ಕ dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				ಸಂರಚನಾ cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				ಯದಿ (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} ಅನ್ಯಥಾ {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					ವರ್ಣ *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					safe_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_empty_src_dirs(src_dir.v, src_dir.nr);

	ಯದಿ (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	ಯದಿ (gitmodules_modified)
		stage_updated_gitmodules(the_repository->index);

	ಯದಿ (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

out:
	strvec_clear(&src_dir);
	ಮುಕ್ತ(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_match_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_to_free);
	hashmap_clear_and_free(&moved_dirs, ಸಂರಚನಾ pathmap_entry, ent);
	strbuf_release(&pathbuf);
	ಮುಕ್ತ(submodule_gitfiles);
	ಮುಕ್ತ(modes);
	ಪ್ರತ್ಯಾಗಮ ret;
}
