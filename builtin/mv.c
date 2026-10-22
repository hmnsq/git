/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "name-hash.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "pathspec.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "symlinks.h"

#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "entry.h"

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	রিক্ত
};

গণনা update_mode {
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

স্থির পূর্ণাঙ্ক needs_worktree_rename(গণনা update_mode mode, গণনা update_mode dst_mode)
{
	প্রত্যাগম !(mode & (INDEX | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#define DUP_BASENAME 1
#define KEEP_TRAILING_SLASH 2

স্থির শূন্য internal_prefix_pathspec(সংরচনা strvec *out,
				     ধ্রুব বর্ণ *prefix,
				     ধ্রুব বর্ণ **pathspec,
				     পূর্ণাঙ্ক গুনতি, unsigned flags)
{
	পূর্ণাঙ্ক prefixlen = prefix ? strlen(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	প্রতি (পূর্ণাঙ্ক i = 0; i < গুনতি; i++) {
		size_t length = strlen(pathspec[i]);
		size_t to_copy = length;
		ধ্রুব বর্ণ *maybe_basename;
		বর্ণ *trimmed, *prefixed_path;

		যাবত্ (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_ঠেলো(out, prefixed_path);

		মুক্ত(prefixed_path);
		মুক্ত(trimmed);
	}
}

স্থির বর্ণ *add_slash(ধ্রুব বর্ণ *path)
{
	size_t len = strlen(path);
	যদি (len && path[len - 1] != '/') {
		বর্ণ *with_slash = xmalloc(st_add(len, 2));
		memcpy(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		প্রত্যাগম with_slash;
	}
	প্রত্যাগম xstrdup(path);
}

#define SUBMODULE_WITH_GITDIR ((ধ্রুব বর্ণ *)1)

স্থির ধ্রুব বর্ণ *submodule_gitfile_path(ধ্রুব বর্ণ *src, পূর্ণাঙ্ক first)
{
	সংরচনা strbuf submodule_dotgit = STRBUF_INIT;
	ধ্রুব বর্ণ *path;

	যদি (!S_ISGITLINK(the_repository->index->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	যদি (!is_staging_gitmodules_ok(the_repository->index))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	যদি (path)
		প্রত্যাগম path;
	প্রত্যাগম SUBMODULE_WITH_GITDIR;
}

স্থির পূর্ণাঙ্ক index_range_of_same_dir(ধ্রুব বর্ণ *src, পূর্ণাঙ্ক length,
				   পূর্ণাঙ্ক *first_p, পূর্ণাঙ্ক *last_p)
{
	বর্ণ *src_w_slash = add_slash(src);
	পূর্ণাঙ্ক first, last, len_w_slash = length + 1;

	first = index_name_pos(the_repository->index, src_w_slash, len_w_slash);
	যদি (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	প্রতি (last = first; last < the_repository->index->cache_nr; last++) {
		ধ্রুব বর্ণ *path = the_repository->index->cache[last]->name;
		যদি (strncmp(path, src_w_slash, len_w_slash))
			ভঙ্গ;
	}

	মুক্ত(src_w_slash);
	*first_p = first;
	*last_p = last;
	প্রত্যাগম last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
স্থির পূর্ণাঙ্ক empty_dir_has_sparse_contents(ধ্রুব বর্ণ *name)
{
	পূর্ণাঙ্ক ret = 0;
	বর্ণ *with_slash = add_slash(name);
	পূর্ণাঙ্ক length = strlen(with_slash);

	পূর্ণাঙ্ক pos = index_name_pos(the_repository->index, with_slash, length);
	ধ্রুব সংরচনা cache_entry *ce;

	যদি (pos < 0) {
		pos = -pos - 1;
		যদি (pos >= the_repository->index->cache_nr)
			গচ্ছ free_return;
		ce = the_repository->index->cache[pos];
		যদি (strncmp(with_slash, ce->name, length))
			গচ্ছ free_return;
		যদি (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	মুক্ত(with_slash);
	প্রত্যাগম ret;
}

স্থির শূন্য remove_empty_src_dirs(ধ্রুব বর্ণ **src_dir, size_t src_dir_nr)
{
	size_t i;
	সংরচনা strbuf a_src_dir = STRBUF_INIT;

	প্রতি (i = 0; i < src_dir_nr; i++) {
		পূর্ণাঙ্ক dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		যদি (index_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

সংরচনা pathmap_entry {
	সংরচনা hashmap_entry ent;
	ধ্রুব বর্ণ *path;
};

স্থির পূর্ণাঙ্ক pathmap_cmp(ধ্রুব শূন্য *cmp_data UNUSED,
		       ধ্রুব সংরচনা hashmap_entry *a,
		       ধ্রুব সংরচনা hashmap_entry *b,
		       ধ্রুব শূন্য *চাবি UNUSED)
{
	ধ্রুব সংরচনা pathmap_entry *e1 = container_of(a, সংরচনা pathmap_entry, ent);
	ধ্রুব সংরচনা pathmap_entry *e2 = container_of(b, সংরচনা pathmap_entry, ent);
	প্রত্যাগম fspathcmp(e1->path, e2->path);
}

পূর্ণাঙ্ক cmd_mv(পূর্ণাঙ্ক argc,
	   ধ্রুব বর্ণ **argv,
	   ধ্রুব বর্ণ *prefix,
	   সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক i, flags, gitmodules_modified = 0;
	পূর্ণাঙ্ক verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	সংরচনা option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT__DRY_RUN(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', রিক্ত, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_END(),
	};
	সংরচনা strvec sources = STRVEC_INIT;
	সংরচনা strvec dest_paths = STRVEC_INIT;
	সংরচনা strvec destinations = STRVEC_INIT;
	সংরচনা strvec submodule_gitfiles_to_free = STRVEC_INIT;
	ধ্রুব বর্ণ **submodule_gitfiles;
	বর্ণ *dst_w_slash = রিক্ত;
	সংরচনা strvec src_dir = STRVEC_INIT;
	গণনা update_mode *modes, dst_mode = 0;
	সংরচনা stat st, dest_st;
	সংরচনা string_list src_for_dst = STRING_LIST_INIT_DUP;
	সংরচনা lock_file lock_file = LOCK_INIT;
	সংরচনা cache_entry *ce;
	সংরচনা string_list only_match_skip_worktree = STRING_LIST_INIT_DUP;
	সংরচনা string_list dirty_paths = STRING_LIST_INIT_DUP;
	সংরচনা hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, রিক্ত);
	সংরচনা strbuf pathbuf = STRBUF_INIT;
	পূর্ণাঙ্ক ret;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	repo_config(the_repository, git_default_config, রিক্ত);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	যদি (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	যদি (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	যদি (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, পরিমাণ(বর্ণ *));

	যদি (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	অন্যথা যদি (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} অন্যথা যদি (!path_in_sparse_checkout(dst_w_slash, the_repository->index) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} অন্যথা যদি (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} অন্যথা {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		যদি (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->index))
			dst_mode = SPARSE;
	}

	/* Checking */
	প্রতি (i = 0; i < argc; i++) {
		ধ্রুব বর্ণ *src = sources.v[i], *dst = destinations.v[i];
		পূর্ণাঙ্ক length;
		ধ্রুব বর্ণ *bad = রিক্ত;
		পূর্ণাঙ্ক skip_sparse = 0;

		যদি (show_only)
			printf(_("Checking rename of '%s' to '%s'\n"), src, dst);

		length = strlen(src);
		যদি (lstat(src, &st) < 0) {
			পূর্ণাঙ্ক pos;
			ধ্রুব সংরচনা cache_entry *ce;

			pos = index_name_pos(the_repository->index, src, length);
			যদি (pos < 0) {
				বর্ণ *src_w_slash = add_slash(src);
				যদি (!path_in_sparse_checkout(src_w_slash, the_repository->index) &&
				    empty_dir_has_sparse_contents(src)) {
					মুক্ত(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					গচ্ছ dir_check;
				}
				মুক্ত(src_w_slash);
				/* only error if existence is expected. */
				যদি (!(modes[i] & SPARSE))
					bad = _("bad source");
				গচ্ছ act_on_entry;
			}
			ce = the_repository->index->cache[pos];
			যদি (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				গচ্ছ act_on_entry;
			}
			যদি (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				গচ্ছ act_on_entry;
			}
			/* Check if dst exists in index */
			যদি (index_name_pos(the_repository->index, dst, strlen(dst)) < 0) {
				modes[i] |= SPARSE;
				গচ্ছ act_on_entry;
			}
			যদি (!force) {
				bad = _("destination exists");
				গচ্ছ act_on_entry;
			}
			modes[i] |= SPARSE;
			গচ্ছ act_on_entry;
		}
		যদি (!strncmp(src, dst, length) &&
		    (dst[length] == 0 || dst[length] == '/')) {
			bad = _("can not move directory into itself");
			গচ্ছ act_on_entry;
		}
		যদি (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			গচ্ছ act_on_entry;
		}

dir_check:
		যদি (S_ISDIR(st.st_mode)) {
			সংরচনা pathmap_entry *entry;
			বর্ণ *dst_with_slash;
			size_t dst_with_slash_len;
			পূর্ণাঙ্ক j, n;
			পূর্ণাঙ্ক first = index_name_pos(the_repository->index, src, length), last;

			যদি (first >= 0) {
				ধ্রুব বর্ণ *path = submodule_gitfile_path(src, first);
				যদি (path != SUBMODULE_WITH_GITDIR)
					path = strvec_ঠেলো(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				গচ্ছ act_on_entry;
			} অন্যথা যদি (index_range_of_same_dir(src, length,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				গচ্ছ act_on_entry;
			}

			entry = xmalloc(পরিমাণ(*entry));
			entry->path = src;
			hashmap_entry_init(&entry->ent, fspathhash(src));
			hashmap_add(&moved_dirs, &entry->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_ঠেলো(&src_dir, src);

			n = argc + last - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = add_slash(dst);
			dst_with_slash_len = strlen(dst_with_slash);

			প্রতি (j = 0; j < last - first; j++) {
				ধ্রুব সংরচনা cache_entry *ce = the_repository->index->cache[first + j];
				ধ্রুব বর্ণ *path = ce->name;
				বর্ণ *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + length + 1);

				strvec_ঠেলো(&sources, path);
				strvec_ঠেলো(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : INDEX);
				submodule_gitfiles[argc + j] = রিক্ত;

				মুক্ত(prefixed_path);
			}

			মুক্ত(dst_with_slash);
			argc += last - first;
			গচ্ছ act_on_entry;
		}
		যদি (!(ce = index_file_exists(the_repository->index, src, length, 0))) {
			bad = _("not under version control");
			গচ্ছ act_on_entry;
		}
		যদি (ce_stage(ce)) {
			bad = _("conflicted");
			গচ্ছ act_on_entry;
		}
		যদি (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			যদি (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				যদি (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					যদি (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = রিক্ত;
				} অন্যথা
					bad = _("Cannot overwrite");
			}
			গচ্ছ act_on_entry;
		}
		যদি (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			গচ্ছ act_on_entry;
		}
		যদি (is_dir_sep(dst[strlen(dst) - 1])) {
			bad = _("destination directory does not exist");
			গচ্ছ act_on_entry;
		}
		যদি (has_symlink_leading_path(dst, strlen(dst))) {
			bad = _("destination is beyond a symbolic link");
			গচ্ছ act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		যদি (needs_worktree_rename(modes[i], dst_mode)) {
			ধ্রুব বর্ণ *slash_ = strrchr(dst, '/');

			যদি (slash_) {
				সংরচনা stat dir_st;
				বর্ণ *dst_dir = xstrdup(dst);
				বর্ণ *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				যদি (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					যদি (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} অন্যথা যদি (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				মুক্ত(dst_dir);
			}

			যদি (bad)
				গচ্ছ act_on_entry;
		}

		যদি (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    index_entry_exists(the_repository->index, dst, strlen(dst))) {
			bad = _("destination exists in the index");
			যদি (force) {
				যদি (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = রিক্ত;
			} অন্যথা {
				গচ্ছ act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		যদি (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		যদি (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		যদি (skip_sparse)
			গচ্ছ remove_entry;

		string_list_ঢোকাও(&src_for_dst, dst);

act_on_entry:
		যদি (!bad)
			অনুবর্ত;
		যদি (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		যদি (--argc > 0) {
			পূর্ণাঙ্ক n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	প্রতি (i = 0; i < argc; i++) {
		ধ্রুব বর্ণ *slash_pos;

		যদি (modes[i] & MOVE_VIA_PARENT_DIR)
			অনুবর্ত;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		যাবত্ (slash_pos > pathbuf.buf) {
			সংরচনা pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			যদি (hashmap_get_entry(&moved_dirs, &needle, ent, রিক্ত))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	যদি (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		যদি (!ignore_errors) {
			ret = 1;
			গচ্ছ out;
		}
	}

	প্রতি (i = 0; i < argc; i++) {
		ধ্রুব বর্ণ *src = sources.v[i], *dst = destinations.v[i];
		গণনা update_mode mode = modes[i];
		পূর্ণাঙ্ক pos;
		পূর্ণাঙ্ক sparse_and_dirty = 0;
		সংরচনা checkout state = CHECKOUT_INIT;
		state.istate = the_repository->index;

		যদি (force)
			state.force = 1;
		যদি (show_only || verbose)
			printf(_("Renaming %s to %s\n"), src, dst);
		যদি (show_only)
			অনুবর্ত;
		যদি (needs_worktree_rename(mode, dst_mode) &&
		    rename(src, dst) < 0) {
			যদি (ignore_errors)
				অনুবর্ত;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		যদি (submodule_gitfiles[i]) {
			যদি (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			যদি (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_বৃক্ষ_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		যদি (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			অনুবর্ত;

		pos = index_name_pos(the_repository->index, src, strlen(src));
		যদি (pos < 0)
			BUG("could not find source in index: '%s'", src);
		যদি (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->index,
						       the_repository->index->cache[pos],
						       &st,
						       0);
		rename_index_entry_at(the_repository->index, pos, dst);

		যদি (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			যদি ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->index)) {
				/* from out-of-cone to in-cone */
				পূর্ণাঙ্ক dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				সংরচনা cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				যদি (checkout_entry(dst_ce, &state, রিক্ত, রিক্ত))
					die(_("cannot checkout %s"), dst_ce->name);
			} অন্যথা যদি ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->index)) {
				/* from in-cone to out-of-cone */
				পূর্ণাঙ্ক dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				সংরচনা cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				যদি (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} অন্যথা {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					বর্ণ *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					safe_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_empty_src_dirs(src_dir.v, src_dir.nr);

	যদি (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	যদি (gitmodules_modified)
		stage_updated_gitmodules(the_repository->index);

	যদি (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

out:
	strvec_clear(&src_dir);
	মুক্ত(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_match_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_to_free);
	hashmap_clear_and_free(&moved_dirs, সংরচনা pathmap_entry, ent);
	strbuf_release(&pathbuf);
	মুক্ত(submodule_gitfiles);
	মুক্ত(modes);
	প্রত্যাগম ret;
}
