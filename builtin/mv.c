/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "name-hash.h"
#شامل "object-file.h"
#شامل "path.h"
#شامل "pathspec.h"
#شامل "lockfile.h"
#شامل "dir.h"
#شامل "string-list.h"
#شامل "parse-options.h"
#شامل "read-cache-ll.h"
#شامل "symlinks.h"

#شامل "setup.h"
#شامل "strvec.h"
#شامل "submodule.h"
#شامل "entry.h"

ایستا ثابت نویسه * ثابت builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	پوچ
};

شمارش update_mode {
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

ایستا صحیح needs_worktree_rename(شمارش update_mode mode, شمارش update_mode dst_mode)
{
	بازگشت !(mode & (INDEX | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#define DUP_BASENAME 1
#define KEEP_TRAILING_SLASH 2

ایستا تهی internal_prefix_pathspec(ساختار strvec *out,
				     ثابت نویسه *prefix,
				     ثابت نویسه **pathspec,
				     صحیح شمار, unsigned flags)
{
	صحیح prefixlen = prefix ? strlen(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	برای (صحیح i = 0; i < شمار; i++) {
		size_t length = strlen(pathspec[i]);
		size_t to_copy = length;
		ثابت نویسه *maybe_basename;
		نویسه *trimmed, *prefixed_path;

		وقتی (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_افزودن(out, prefixed_path);

		آزاد(prefixed_path);
		آزاد(trimmed);
	}
}

ایستا نویسه *add_slash(ثابت نویسه *path)
{
	size_t len = strlen(path);
	اگر (len && path[len - 1] != '/') {
		نویسه *with_slash = xmalloc(st_add(len, 2));
		memcpy(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		بازگشت with_slash;
	}
	بازگشت xstrdup(path);
}

#define SUBMODULE_WITH_GITDIR ((ثابت نویسه *)1)

ایستا ثابت نویسه *submodule_gitfile_path(ثابت نویسه *src, صحیح first)
{
	ساختار strbuf submodule_dotgit = STRBUF_INIT;
	ثابت نویسه *path;

	اگر (!S_ISGITLINK(the_repository->index->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	اگر (!is_staging_gitmodules_ok(the_repository->index))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	اگر (path)
		بازگشت path;
	بازگشت SUBMODULE_WITH_GITDIR;
}

ایستا صحیح index_range_of_same_dir(ثابت نویسه *src, صحیح length,
				   صحیح *first_p, صحیح *last_p)
{
	نویسه *src_w_slash = add_slash(src);
	صحیح first, last, len_w_slash = length + 1;

	first = index_name_pos(the_repository->index, src_w_slash, len_w_slash);
	اگر (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	برای (last = first; last < the_repository->index->cache_nr; last++) {
		ثابت نویسه *path = the_repository->index->cache[last]->name;
		اگر (strncmp(path, src_w_slash, len_w_slash))
			بشکن;
	}

	آزاد(src_w_slash);
	*first_p = first;
	*last_p = last;
	بازگشت last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
ایستا صحیح empty_dir_has_sparse_contents(ثابت نویسه *name)
{
	صحیح ret = 0;
	نویسه *with_slash = add_slash(name);
	صحیح length = strlen(with_slash);

	صحیح pos = index_name_pos(the_repository->index, with_slash, length);
	ثابت ساختار cache_entry *ce;

	اگر (pos < 0) {
		pos = -pos - 1;
		اگر (pos >= the_repository->index->cache_nr)
			برو free_return;
		ce = the_repository->index->cache[pos];
		اگر (strncmp(with_slash, ce->name, length))
			برو free_return;
		اگر (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	آزاد(with_slash);
	بازگشت ret;
}

ایستا تهی remove_empty_src_dirs(ثابت نویسه **src_dir, size_t src_dir_nr)
{
	size_t i;
	ساختار strbuf a_src_dir = STRBUF_INIT;

	برای (i = 0; i < src_dir_nr; i++) {
		صحیح dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		اگر (index_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

ساختار pathmap_entry {
	ساختار hashmap_entry ent;
	ثابت نویسه *path;
};

ایستا صحیح pathmap_cmp(ثابت تهی *cmp_data UNUSED,
		       ثابت ساختار hashmap_entry *a,
		       ثابت ساختار hashmap_entry *b,
		       ثابت تهی *کلید UNUSED)
{
	ثابت ساختار pathmap_entry *e1 = container_of(a, ساختار pathmap_entry, ent);
	ثابت ساختار pathmap_entry *e2 = container_of(b, ساختار pathmap_entry, ent);
	بازگشت fspathcmp(e1->path, e2->path);
}

صحیح cmd_mv(صحیح argc,
	   ثابت نویسه **argv,
	   ثابت نویسه *prefix,
	   ساختار repository *repo UNUSED)
{
	صحیح i, flags, gitmodules_modified = 0;
	صحیح verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	ساختار option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT__DRY_RUN(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', پوچ, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_END(),
	};
	ساختار strvec sources = STRVEC_INIT;
	ساختار strvec dest_paths = STRVEC_INIT;
	ساختار strvec destinations = STRVEC_INIT;
	ساختار strvec submodule_gitfiles_to_free = STRVEC_INIT;
	ثابت نویسه **submodule_gitfiles;
	نویسه *dst_w_slash = پوچ;
	ساختار strvec src_dir = STRVEC_INIT;
	شمارش update_mode *modes, dst_mode = 0;
	ساختار stat st, dest_st;
	ساختار string_list src_for_dst = STRING_LIST_INIT_DUP;
	ساختار lock_file lock_file = LOCK_INIT;
	ساختار cache_entry *ce;
	ساختار string_list only_match_skip_worktree = STRING_LIST_INIT_DUP;
	ساختار string_list dirty_paths = STRING_LIST_INIT_DUP;
	ساختار hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, پوچ);
	ساختار strbuf pathbuf = STRBUF_INIT;
	صحیح ret;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	repo_config(the_repository, git_default_config, پوچ);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	اگر (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	اگر (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	اگر (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, اندازه(نویسه *));

	اگر (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	وگرنه اگر (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} وگرنه اگر (!path_in_sparse_checkout(dst_w_slash, the_repository->index) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} وگرنه اگر (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} وگرنه {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		اگر (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->index))
			dst_mode = SPARSE;
	}

	/* Checking */
	برای (i = 0; i < argc; i++) {
		ثابت نویسه *src = sources.v[i], *dst = destinations.v[i];
		صحیح length;
		ثابت نویسه *bad = پوچ;
		صحیح skip_sparse = 0;

		اگر (show_only)
			printf(_("Checking rename of '%s' to '%s'\n"), src, dst);

		length = strlen(src);
		اگر (lstat(src, &st) < 0) {
			صحیح pos;
			ثابت ساختار cache_entry *ce;

			pos = index_name_pos(the_repository->index, src, length);
			اگر (pos < 0) {
				نویسه *src_w_slash = add_slash(src);
				اگر (!path_in_sparse_checkout(src_w_slash, the_repository->index) &&
				    empty_dir_has_sparse_contents(src)) {
					آزاد(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					برو dir_check;
				}
				آزاد(src_w_slash);
				/* only error if existence is expected. */
				اگر (!(modes[i] & SPARSE))
					bad = _("bad source");
				برو act_on_entry;
			}
			ce = the_repository->index->cache[pos];
			اگر (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				برو act_on_entry;
			}
			اگر (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				برو act_on_entry;
			}
			/* Check if dst exists in index */
			اگر (index_name_pos(the_repository->index, dst, strlen(dst)) < 0) {
				modes[i] |= SPARSE;
				برو act_on_entry;
			}
			اگر (!force) {
				bad = _("destination exists");
				برو act_on_entry;
			}
			modes[i] |= SPARSE;
			برو act_on_entry;
		}
		اگر (!strncmp(src, dst, length) &&
		    (dst[length] == 0 || dst[length] == '/')) {
			bad = _("can not move directory into itself");
			برو act_on_entry;
		}
		اگر (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			برو act_on_entry;
		}

dir_check:
		اگر (S_ISDIR(st.st_mode)) {
			ساختار pathmap_entry *entry;
			نویسه *dst_with_slash;
			size_t dst_with_slash_len;
			صحیح j, n;
			صحیح first = index_name_pos(the_repository->index, src, length), last;

			اگر (first >= 0) {
				ثابت نویسه *path = submodule_gitfile_path(src, first);
				اگر (path != SUBMODULE_WITH_GITDIR)
					path = strvec_افزودن(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				برو act_on_entry;
			} وگرنه اگر (index_range_of_same_dir(src, length,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				برو act_on_entry;
			}

			entry = xmalloc(اندازه(*entry));
			entry->path = src;
			hashmap_entry_init(&entry->ent, fspathhash(src));
			hashmap_add(&moved_dirs, &entry->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_افزودن(&src_dir, src);

			n = argc + last - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = add_slash(dst);
			dst_with_slash_len = strlen(dst_with_slash);

			برای (j = 0; j < last - first; j++) {
				ثابت ساختار cache_entry *ce = the_repository->index->cache[first + j];
				ثابت نویسه *path = ce->name;
				نویسه *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + length + 1);

				strvec_افزودن(&sources, path);
				strvec_افزودن(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : INDEX);
				submodule_gitfiles[argc + j] = پوچ;

				آزاد(prefixed_path);
			}

			آزاد(dst_with_slash);
			argc += last - first;
			برو act_on_entry;
		}
		اگر (!(ce = index_file_exists(the_repository->index, src, length, 0))) {
			bad = _("not under version control");
			برو act_on_entry;
		}
		اگر (ce_stage(ce)) {
			bad = _("conflicted");
			برو act_on_entry;
		}
		اگر (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			اگر (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				اگر (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					اگر (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = پوچ;
				} وگرنه
					bad = _("Cannot overwrite");
			}
			برو act_on_entry;
		}
		اگر (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			برو act_on_entry;
		}
		اگر (is_dir_sep(dst[strlen(dst) - 1])) {
			bad = _("destination directory does not exist");
			برو act_on_entry;
		}
		اگر (has_symlink_leading_path(dst, strlen(dst))) {
			bad = _("destination is beyond a symbolic link");
			برو act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		اگر (needs_worktree_rename(modes[i], dst_mode)) {
			ثابت نویسه *slash_ = strrchr(dst, '/');

			اگر (slash_) {
				ساختار stat dir_st;
				نویسه *dst_dir = xstrdup(dst);
				نویسه *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				اگر (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					اگر (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} وگرنه اگر (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				آزاد(dst_dir);
			}

			اگر (bad)
				برو act_on_entry;
		}

		اگر (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    index_entry_exists(the_repository->index, dst, strlen(dst))) {
			bad = _("destination exists in the index");
			اگر (force) {
				اگر (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = پوچ;
			} وگرنه {
				برو act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		اگر (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		اگر (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		اگر (skip_sparse)
			برو remove_entry;

		string_list_درج(&src_for_dst, dst);

act_on_entry:
		اگر (!bad)
			ادامه;
		اگر (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		اگر (--argc > 0) {
			صحیح n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	برای (i = 0; i < argc; i++) {
		ثابت نویسه *slash_pos;

		اگر (modes[i] & MOVE_VIA_PARENT_DIR)
			ادامه;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		وقتی (slash_pos > pathbuf.buf) {
			ساختار pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			اگر (hashmap_get_entry(&moved_dirs, &needle, ent, پوچ))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	اگر (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		اگر (!ignore_errors) {
			ret = 1;
			برو out;
		}
	}

	برای (i = 0; i < argc; i++) {
		ثابت نویسه *src = sources.v[i], *dst = destinations.v[i];
		شمارش update_mode mode = modes[i];
		صحیح pos;
		صحیح sparse_and_dirty = 0;
		ساختار checkout state = CHECKOUT_INIT;
		state.istate = the_repository->index;

		اگر (force)
			state.force = 1;
		اگر (show_only || verbose)
			printf(_("Renaming %s to %s\n"), src, dst);
		اگر (show_only)
			ادامه;
		اگر (needs_worktree_rename(mode, dst_mode) &&
		    rename(src, dst) < 0) {
			اگر (ignore_errors)
				ادامه;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		اگر (submodule_gitfiles[i]) {
			اگر (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			اگر (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_درخت_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		اگر (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			ادامه;

		pos = index_name_pos(the_repository->index, src, strlen(src));
		اگر (pos < 0)
			BUG("could not find source in index: '%s'", src);
		اگر (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->index,
						       the_repository->index->cache[pos],
						       &st,
						       0);
		rename_index_entry_at(the_repository->index, pos, dst);

		اگر (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			اگر ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->index)) {
				/* from out-of-cone to in-cone */
				صحیح dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				ساختار cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				اگر (checkout_entry(dst_ce, &state, پوچ, پوچ))
					die(_("cannot checkout %s"), dst_ce->name);
			} وگرنه اگر ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->index)) {
				/* from in-cone to out-of-cone */
				صحیح dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				ساختار cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				اگر (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} وگرنه {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					نویسه *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					safe_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_empty_src_dirs(src_dir.v, src_dir.nr);

	اگر (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	اگر (gitmodules_modified)
		stage_updated_gitmodules(the_repository->index);

	اگر (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

out:
	strvec_clear(&src_dir);
	آزاد(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_match_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_to_free);
	hashmap_clear_and_free(&moved_dirs, ساختار pathmap_entry, ent);
	strbuf_release(&pathbuf);
	آزاد(submodule_gitfiles);
	آزاد(modes);
	بازگشت ret;
}
