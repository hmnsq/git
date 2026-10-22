/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "name-hash.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "pathspec.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "symlinks.h"

#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "entry.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	ശൂന്യ
};

എണ്ണൽ update_mode {
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

സ്ഥിതം പൂർണ്ണസംഖ്യ needs_worktree_rename(എണ്ണൽ update_mode mode, എണ്ണൽ update_mode dst_mode)
{
	തിരികെ !(mode & (INDEX | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#define DUP_BASENAME 1
#define KEEP_TRAILING_SLASH 2

സ്ഥിതം ശൂന്യം internal_prefix_pathspec(ഘടന strvec *out,
				     സ്ഥിരം അക്ഷരം *prefix,
				     സ്ഥിരം അക്ഷരം **pathspec,
				     പൂർണ്ണസംഖ്യ എണ്ണം, unsigned flags)
{
	പൂർണ്ണസംഖ്യ prefixlen = prefix ? strlen(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++) {
		size_t length = strlen(pathspec[i]);
		size_t to_copy = length;
		സ്ഥിരം അക്ഷരം *maybe_basename;
		അക്ഷരം *trimmed, *prefixed_path;

		ആകുമ്പോൾ (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_തള്ളുക(out, prefixed_path);

		സ്വതന്ത്രം(prefixed_path);
		സ്വതന്ത്രം(trimmed);
	}
}

സ്ഥിതം അക്ഷരം *add_slash(സ്ഥിരം അക്ഷരം *path)
{
	size_t len = strlen(path);
	എങ്കിൽ (len && path[len - 1] != '/') {
		അക്ഷരം *with_slash = xmalloc(st_add(len, 2));
		memcpy(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		തിരികെ with_slash;
	}
	തിരികെ xstrdup(path);
}

#define SUBMODULE_WITH_GITDIR ((സ്ഥിരം അക്ഷരം *)1)

സ്ഥിതം സ്ഥിരം അക്ഷരം *submodule_gitfile_path(സ്ഥിരം അക്ഷരം *src, പൂർണ്ണസംഖ്യ first)
{
	ഘടന strbuf submodule_dotgit = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *path;

	എങ്കിൽ (!S_ISGITLINK(the_repository->index->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	എങ്കിൽ (!is_staging_gitmodules_ok(the_repository->index))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	എങ്കിൽ (path)
		തിരികെ path;
	തിരികെ SUBMODULE_WITH_GITDIR;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ index_range_of_same_dir(സ്ഥിരം അക്ഷരം *src, പൂർണ്ണസംഖ്യ length,
				   പൂർണ്ണസംഖ്യ *first_p, പൂർണ്ണസംഖ്യ *last_p)
{
	അക്ഷരം *src_w_slash = add_slash(src);
	പൂർണ്ണസംഖ്യ first, last, len_w_slash = length + 1;

	first = index_name_pos(the_repository->index, src_w_slash, len_w_slash);
	എങ്കിൽ (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	വേണ്ടി (last = first; last < the_repository->index->cache_nr; last++) {
		സ്ഥിരം അക്ഷരം *path = the_repository->index->cache[last]->name;
		എങ്കിൽ (strncmp(path, src_w_slash, len_w_slash))
			നിർത്തുക;
	}

	സ്വതന്ത്രം(src_w_slash);
	*first_p = first;
	*last_p = last;
	തിരികെ last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ empty_dir_has_sparse_contents(സ്ഥിരം അക്ഷരം *name)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	അക്ഷരം *with_slash = add_slash(name);
	പൂർണ്ണസംഖ്യ length = strlen(with_slash);

	പൂർണ്ണസംഖ്യ pos = index_name_pos(the_repository->index, with_slash, length);
	സ്ഥിരം ഘടന cache_entry *ce;

	എങ്കിൽ (pos < 0) {
		pos = -pos - 1;
		എങ്കിൽ (pos >= the_repository->index->cache_nr)
			പോകുക free_return;
		ce = the_repository->index->cache[pos];
		എങ്കിൽ (strncmp(with_slash, ce->name, length))
			പോകുക free_return;
		എങ്കിൽ (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	സ്വതന്ത്രം(with_slash);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം remove_empty_src_dirs(സ്ഥിരം അക്ഷരം **src_dir, size_t src_dir_nr)
{
	size_t i;
	ഘടന strbuf a_src_dir = STRBUF_INIT;

	വേണ്ടി (i = 0; i < src_dir_nr; i++) {
		പൂർണ്ണസംഖ്യ dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		എങ്കിൽ (index_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

ഘടന pathmap_entry {
	ഘടന hashmap_entry ent;
	സ്ഥിരം അക്ഷരം *path;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ pathmap_cmp(സ്ഥിരം ശൂന്യം *cmp_data UNUSED,
		       സ്ഥിരം ഘടന hashmap_entry *a,
		       സ്ഥിരം ഘടന hashmap_entry *b,
		       സ്ഥിരം ശൂന്യം *താക്കോൽ UNUSED)
{
	സ്ഥിരം ഘടന pathmap_entry *e1 = container_of(a, ഘടന pathmap_entry, ent);
	സ്ഥിരം ഘടന pathmap_entry *e2 = container_of(b, ഘടന pathmap_entry, ent);
	തിരികെ fspathcmp(e1->path, e2->path);
}

പൂർണ്ണസംഖ്യ cmd_mv(പൂർണ്ണസംഖ്യ argc,
	   സ്ഥിരം അക്ഷരം **argv,
	   സ്ഥിരം അക്ഷരം *prefix,
	   ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i, flags, gitmodules_modified = 0;
	പൂർണ്ണസംഖ്യ verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	ഘടന option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT__DRY_RUN(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', ശൂന്യ, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_END(),
	};
	ഘടന strvec sources = STRVEC_INIT;
	ഘടന strvec dest_paths = STRVEC_INIT;
	ഘടന strvec destinations = STRVEC_INIT;
	ഘടന strvec submodule_gitfiles_to_free = STRVEC_INIT;
	സ്ഥിരം അക്ഷരം **submodule_gitfiles;
	അക്ഷരം *dst_w_slash = ശൂന്യ;
	ഘടന strvec src_dir = STRVEC_INIT;
	എണ്ണൽ update_mode *modes, dst_mode = 0;
	ഘടന stat st, dest_st;
	ഘടന string_list src_for_dst = STRING_LIST_INIT_DUP;
	ഘടന lock_file lock_file = LOCK_INIT;
	ഘടന cache_entry *ce;
	ഘടന string_list only_match_skip_worktree = STRING_LIST_INIT_DUP;
	ഘടന string_list dirty_paths = STRING_LIST_INIT_DUP;
	ഘടന hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, ശൂന്യ);
	ഘടന strbuf pathbuf = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	repo_config(the_repository, git_default_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	എങ്കിൽ (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	എങ്കിൽ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	എങ്കിൽ (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, വലുപ്പം(അക്ഷരം *));

	എങ്കിൽ (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	അല്ലെങ്കിൽ എങ്കിൽ (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} അല്ലെങ്കിൽ എങ്കിൽ (!path_in_sparse_checkout(dst_w_slash, the_repository->index) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} അല്ലെങ്കിൽ എങ്കിൽ (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} അല്ലെങ്കിൽ {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		എങ്കിൽ (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->index))
			dst_mode = SPARSE;
	}

	/* Checking */
	വേണ്ടി (i = 0; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *src = sources.v[i], *dst = destinations.v[i];
		പൂർണ്ണസംഖ്യ length;
		സ്ഥിരം അക്ഷരം *bad = ശൂന്യ;
		പൂർണ്ണസംഖ്യ skip_sparse = 0;

		എങ്കിൽ (show_only)
			printf(_("Checking rename of '%s' to '%s'\n"), src, dst);

		length = strlen(src);
		എങ്കിൽ (lstat(src, &st) < 0) {
			പൂർണ്ണസംഖ്യ pos;
			സ്ഥിരം ഘടന cache_entry *ce;

			pos = index_name_pos(the_repository->index, src, length);
			എങ്കിൽ (pos < 0) {
				അക്ഷരം *src_w_slash = add_slash(src);
				എങ്കിൽ (!path_in_sparse_checkout(src_w_slash, the_repository->index) &&
				    empty_dir_has_sparse_contents(src)) {
					സ്വതന്ത്രം(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					പോകുക dir_check;
				}
				സ്വതന്ത്രം(src_w_slash);
				/* only error if existence is expected. */
				എങ്കിൽ (!(modes[i] & SPARSE))
					bad = _("bad source");
				പോകുക act_on_entry;
			}
			ce = the_repository->index->cache[pos];
			എങ്കിൽ (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				പോകുക act_on_entry;
			}
			എങ്കിൽ (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				പോകുക act_on_entry;
			}
			/* Check if dst exists in index */
			എങ്കിൽ (index_name_pos(the_repository->index, dst, strlen(dst)) < 0) {
				modes[i] |= SPARSE;
				പോകുക act_on_entry;
			}
			എങ്കിൽ (!force) {
				bad = _("destination exists");
				പോകുക act_on_entry;
			}
			modes[i] |= SPARSE;
			പോകുക act_on_entry;
		}
		എങ്കിൽ (!strncmp(src, dst, length) &&
		    (dst[length] == 0 || dst[length] == '/')) {
			bad = _("can not move directory into itself");
			പോകുക act_on_entry;
		}
		എങ്കിൽ (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			പോകുക act_on_entry;
		}

dir_check:
		എങ്കിൽ (S_ISDIR(st.st_mode)) {
			ഘടന pathmap_entry *entry;
			അക്ഷരം *dst_with_slash;
			size_t dst_with_slash_len;
			പൂർണ്ണസംഖ്യ j, n;
			പൂർണ്ണസംഖ്യ first = index_name_pos(the_repository->index, src, length), last;

			എങ്കിൽ (first >= 0) {
				സ്ഥിരം അക്ഷരം *path = submodule_gitfile_path(src, first);
				എങ്കിൽ (path != SUBMODULE_WITH_GITDIR)
					path = strvec_തള്ളുക(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				പോകുക act_on_entry;
			} അല്ലെങ്കിൽ എങ്കിൽ (index_range_of_same_dir(src, length,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				പോകുക act_on_entry;
			}

			entry = xmalloc(വലുപ്പം(*entry));
			entry->path = src;
			hashmap_entry_init(&entry->ent, fspathhash(src));
			hashmap_add(&moved_dirs, &entry->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_തള്ളുക(&src_dir, src);

			n = argc + last - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = add_slash(dst);
			dst_with_slash_len = strlen(dst_with_slash);

			വേണ്ടി (j = 0; j < last - first; j++) {
				സ്ഥിരം ഘടന cache_entry *ce = the_repository->index->cache[first + j];
				സ്ഥിരം അക്ഷരം *path = ce->name;
				അക്ഷരം *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + length + 1);

				strvec_തള്ളുക(&sources, path);
				strvec_തള്ളുക(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : INDEX);
				submodule_gitfiles[argc + j] = ശൂന്യ;

				സ്വതന്ത്രം(prefixed_path);
			}

			സ്വതന്ത്രം(dst_with_slash);
			argc += last - first;
			പോകുക act_on_entry;
		}
		എങ്കിൽ (!(ce = index_file_exists(the_repository->index, src, length, 0))) {
			bad = _("not under version control");
			പോകുക act_on_entry;
		}
		എങ്കിൽ (ce_stage(ce)) {
			bad = _("conflicted");
			പോകുക act_on_entry;
		}
		എങ്കിൽ (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			എങ്കിൽ (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				എങ്കിൽ (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					എങ്കിൽ (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = ശൂന്യ;
				} അല്ലെങ്കിൽ
					bad = _("Cannot overwrite");
			}
			പോകുക act_on_entry;
		}
		എങ്കിൽ (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			പോകുക act_on_entry;
		}
		എങ്കിൽ (is_dir_sep(dst[strlen(dst) - 1])) {
			bad = _("destination directory does not exist");
			പോകുക act_on_entry;
		}
		എങ്കിൽ (has_symlink_leading_path(dst, strlen(dst))) {
			bad = _("destination is beyond a symbolic link");
			പോകുക act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		എങ്കിൽ (needs_worktree_rename(modes[i], dst_mode)) {
			സ്ഥിരം അക്ഷരം *slash_ = strrchr(dst, '/');

			എങ്കിൽ (slash_) {
				ഘടന stat dir_st;
				അക്ഷരം *dst_dir = xstrdup(dst);
				അക്ഷരം *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				എങ്കിൽ (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					എങ്കിൽ (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} അല്ലെങ്കിൽ എങ്കിൽ (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				സ്വതന്ത്രം(dst_dir);
			}

			എങ്കിൽ (bad)
				പോകുക act_on_entry;
		}

		എങ്കിൽ (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    index_entry_exists(the_repository->index, dst, strlen(dst))) {
			bad = _("destination exists in the index");
			എങ്കിൽ (force) {
				എങ്കിൽ (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = ശൂന്യ;
			} അല്ലെങ്കിൽ {
				പോകുക act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		എങ്കിൽ (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		എങ്കിൽ (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		എങ്കിൽ (skip_sparse)
			പോകുക remove_entry;

		string_list_തിരുകുക(&src_for_dst, dst);

act_on_entry:
		എങ്കിൽ (!bad)
			തുടരുക;
		എങ്കിൽ (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		എങ്കിൽ (--argc > 0) {
			പൂർണ്ണസംഖ്യ n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	വേണ്ടി (i = 0; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *slash_pos;

		എങ്കിൽ (modes[i] & MOVE_VIA_PARENT_DIR)
			തുടരുക;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		ആകുമ്പോൾ (slash_pos > pathbuf.buf) {
			ഘടന pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			എങ്കിൽ (hashmap_get_entry(&moved_dirs, &needle, ent, ശൂന്യ))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	എങ്കിൽ (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		എങ്കിൽ (!ignore_errors) {
			ret = 1;
			പോകുക out;
		}
	}

	വേണ്ടി (i = 0; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *src = sources.v[i], *dst = destinations.v[i];
		എണ്ണൽ update_mode mode = modes[i];
		പൂർണ്ണസംഖ്യ pos;
		പൂർണ്ണസംഖ്യ sparse_and_dirty = 0;
		ഘടന checkout state = CHECKOUT_INIT;
		state.istate = the_repository->index;

		എങ്കിൽ (force)
			state.force = 1;
		എങ്കിൽ (show_only || verbose)
			printf(_("Renaming %s to %s\n"), src, dst);
		എങ്കിൽ (show_only)
			തുടരുക;
		എങ്കിൽ (needs_worktree_rename(mode, dst_mode) &&
		    rename(src, dst) < 0) {
			എങ്കിൽ (ignore_errors)
				തുടരുക;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		എങ്കിൽ (submodule_gitfiles[i]) {
			എങ്കിൽ (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			എങ്കിൽ (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_വൃക്ഷം_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		എങ്കിൽ (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			തുടരുക;

		pos = index_name_pos(the_repository->index, src, strlen(src));
		എങ്കിൽ (pos < 0)
			BUG("could not find source in index: '%s'", src);
		എങ്കിൽ (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->index,
						       the_repository->index->cache[pos],
						       &st,
						       0);
		rename_index_entry_at(the_repository->index, pos, dst);

		എങ്കിൽ (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			എങ്കിൽ ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->index)) {
				/* from out-of-cone to in-cone */
				പൂർണ്ണസംഖ്യ dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				ഘടന cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				എങ്കിൽ (checkout_entry(dst_ce, &state, ശൂന്യ, ശൂന്യ))
					die(_("cannot checkout %s"), dst_ce->name);
			} അല്ലെങ്കിൽ എങ്കിൽ ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->index)) {
				/* from in-cone to out-of-cone */
				പൂർണ്ണസംഖ്യ dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				ഘടന cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				എങ്കിൽ (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} അല്ലെങ്കിൽ {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					അക്ഷരം *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					safe_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_empty_src_dirs(src_dir.v, src_dir.nr);

	എങ്കിൽ (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	എങ്കിൽ (gitmodules_modified)
		stage_updated_gitmodules(the_repository->index);

	എങ്കിൽ (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

out:
	strvec_clear(&src_dir);
	സ്വതന്ത്രം(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_match_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_to_free);
	hashmap_clear_and_free(&moved_dirs, ഘടന pathmap_entry, ent);
	strbuf_release(&pathbuf);
	സ്വതന്ത്രം(submodule_gitfiles);
	സ്വതന്ത്രം(modes);
	തിരികെ ret;
}
