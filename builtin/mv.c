/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "name-hash.h"
#ضمن "object-file.h"
#ضمن "path.h"
#ضمن "pathspec.h"
#ضمن "lockfile.h"
#ضمن "dir.h"
#ضمن "string-list.h"
#ضمن "parse-options.h"
#ضمن "read-cache-ll.h"
#ضمن "symlinks.h"

#ضمن "setup.h"
#ضمن "strvec.h"
#ضمن "submodule.h"
#ضمن "entry.h"

ساكن ثابت حرف * ثابت builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	عدم
};

تعداد update_mode {
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

ساكن صحيح needs_worktree_rename(تعداد update_mode mode, تعداد update_mode dst_mode)
{
	החזר !(mode & (INDEX | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#define DUP_BASENAME 1
#define KEEP_TRAILING_SLASH 2

ساكن فراغ internal_prefix_pathspec(بنية strvec *out,
				     ثابت حرف *prefix,
				     ثابت حرف **pathspec,
				     صحيح count, unsigned flags)
{
	صحيح prefixlen = prefix ? strlen(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	עבור (صحيح i = 0; i < count; i++) {
		size_t length = strlen(pathspec[i]);
		size_t to_copy = length;
		ثابت حرف *maybe_basename;
		حرف *trimmed, *prefixed_path;

		כלעוד (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_push(out, prefixed_path);

		حرر(prefixed_path);
		حرر(trimmed);
	}
}

ساكن حرف *add_slash(ثابت حرف *path)
{
	size_t len = strlen(path);
	אם (len && path[len - 1] != '/') {
		حرف *with_slash = xmalloc(st_add(len, 2));
		memcpy(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		החזר with_slash;
	}
	החזר xstrdup(path);
}

#define SUBMODULE_WITH_GITDIR ((ثابت حرف *)1)

ساكن ثابت حرف *submodule_gitfile_path(ثابت حرف *src, صحيح first)
{
	بنية strbuf submodule_dotgit = STRBUF_INIT;
	ثابت حرف *path;

	אם (!S_ISGITLINK(the_repository->index->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	אם (!is_staging_gitmodules_ok(the_repository->index))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	אם (path)
		החזר path;
	החזר SUBMODULE_WITH_GITDIR;
}

ساكن صحيح index_range_of_same_dir(ثابت حرف *src, صحيح length,
				   صحيح *first_p, صحيح *last_p)
{
	حرف *src_w_slash = add_slash(src);
	صحيح first, last, len_w_slash = length + 1;

	first = index_name_pos(the_repository->index, src_w_slash, len_w_slash);
	אם (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	עבור (last = first; last < the_repository->index->cache_nr; last++) {
		ثابت حرف *path = the_repository->index->cache[last]->name;
		אם (strncmp(path, src_w_slash, len_w_slash))
			اكسر;
	}

	حرر(src_w_slash);
	*first_p = first;
	*last_p = last;
	החזר last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
ساكن صحيح empty_dir_has_sparse_contents(ثابت حرف *name)
{
	صحيح ret = 0;
	حرف *with_slash = add_slash(name);
	صحيح length = strlen(with_slash);

	صحيح pos = index_name_pos(the_repository->index, with_slash, length);
	ثابت بنية cache_entry *ce;

	אם (pos < 0) {
		pos = -pos - 1;
		אם (pos >= the_repository->index->cache_nr)
			اذهب free_return;
		ce = the_repository->index->cache[pos];
		אם (strncmp(with_slash, ce->name, length))
			اذهب free_return;
		אם (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	حرر(with_slash);
	החזר ret;
}

ساكن فراغ remove_empty_src_dirs(ثابت حرف **src_dir, size_t src_dir_nr)
{
	size_t i;
	بنية strbuf a_src_dir = STRBUF_INIT;

	עבור (i = 0; i < src_dir_nr; i++) {
		صحيح dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		אם (index_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

بنية pathmap_entry {
	بنية hashmap_entry ent;
	ثابت حرف *path;
};

ساكن صحيح pathmap_cmp(ثابت فراغ *cmp_data UNUSED,
		       ثابت بنية hashmap_entry *a,
		       ثابت بنية hashmap_entry *b,
		       ثابت فراغ *key UNUSED)
{
	ثابت بنية pathmap_entry *e1 = container_of(a, بنية pathmap_entry, ent);
	ثابت بنية pathmap_entry *e2 = container_of(b, بنية pathmap_entry, ent);
	החזר fspathcmp(e1->path, e2->path);
}

صحيح cmd_mv(صحيح argc,
	   ثابت حرف **argv,
	   ثابت حرف *prefix,
	   بنية repository *repo UNUSED)
{
	صحيح i, flags, gitmodules_modified = 0;
	صحيح verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	بنية option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT__DRY_RUN(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', عدم, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_END(),
	};
	بنية strvec sources = STRVEC_INIT;
	بنية strvec dest_paths = STRVEC_INIT;
	بنية strvec destinations = STRVEC_INIT;
	بنية strvec submodule_gitfiles_to_free = STRVEC_INIT;
	ثابت حرف **submodule_gitfiles;
	حرف *dst_w_slash = عدم;
	بنية strvec src_dir = STRVEC_INIT;
	تعداد update_mode *modes, dst_mode = 0;
	بنية stat st, dest_st;
	بنية string_list src_for_dst = STRING_LIST_INIT_DUP;
	بنية lock_file lock_file = LOCK_INIT;
	بنية cache_entry *ce;
	بنية string_list only_match_skip_worktree = STRING_LIST_INIT_DUP;
	بنية string_list dirty_paths = STRING_LIST_INIT_DUP;
	بنية hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, عدم);
	بنية strbuf pathbuf = STRBUF_INIT;
	صحيح ret;
	بنية repo_config_values *cfg = repo_config_values(the_repository);

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	אם (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	אם (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	אם (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, حجم(حرف *));

	אם (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	אחרת אם (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} אחרת אם (!path_in_sparse_checkout(dst_w_slash, the_repository->index) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} אחרת אם (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} אחרת {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		אם (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->index))
			dst_mode = SPARSE;
	}

	/* Checking */
	עבור (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		صحيح length;
		ثابت حرف *bad = عدم;
		صحيح skip_sparse = 0;

		אם (show_only)
			printf(_("Checking rename of '%s' to '%s'\n"), src, dst);

		length = strlen(src);
		אם (lstat(src, &st) < 0) {
			صحيح pos;
			ثابت بنية cache_entry *ce;

			pos = index_name_pos(the_repository->index, src, length);
			אם (pos < 0) {
				حرف *src_w_slash = add_slash(src);
				אם (!path_in_sparse_checkout(src_w_slash, the_repository->index) &&
				    empty_dir_has_sparse_contents(src)) {
					حرر(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					اذهب dir_check;
				}
				حرر(src_w_slash);
				/* only error if existence is expected. */
				אם (!(modes[i] & SPARSE))
					bad = _("bad source");
				اذهب act_on_entry;
			}
			ce = the_repository->index->cache[pos];
			אם (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				اذهب act_on_entry;
			}
			אם (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				اذهب act_on_entry;
			}
			/* Check if dst exists in index */
			אם (index_name_pos(the_repository->index, dst, strlen(dst)) < 0) {
				modes[i] |= SPARSE;
				اذهب act_on_entry;
			}
			אם (!force) {
				bad = _("destination exists");
				اذهب act_on_entry;
			}
			modes[i] |= SPARSE;
			اذهب act_on_entry;
		}
		אם (!strncmp(src, dst, length) &&
		    (dst[length] == 0 || dst[length] == '/')) {
			bad = _("can not move directory into itself");
			اذهب act_on_entry;
		}
		אם (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			اذهب act_on_entry;
		}

dir_check:
		אם (S_ISDIR(st.st_mode)) {
			بنية pathmap_entry *entry;
			حرف *dst_with_slash;
			size_t dst_with_slash_len;
			صحيح j, n;
			صحيح first = index_name_pos(the_repository->index, src, length), last;

			אם (first >= 0) {
				ثابت حرف *path = submodule_gitfile_path(src, first);
				אם (path != SUBMODULE_WITH_GITDIR)
					path = strvec_push(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				اذهب act_on_entry;
			} אחרת אם (index_range_of_same_dir(src, length,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				اذهب act_on_entry;
			}

			entry = xmalloc(حجم(*entry));
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

			עבור (j = 0; j < last - first; j++) {
				ثابت بنية cache_entry *ce = the_repository->index->cache[first + j];
				ثابت حرف *path = ce->name;
				حرف *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + length + 1);

				strvec_push(&sources, path);
				strvec_push(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : INDEX);
				submodule_gitfiles[argc + j] = عدم;

				حرر(prefixed_path);
			}

			حرر(dst_with_slash);
			argc += last - first;
			اذهب act_on_entry;
		}
		אם (!(ce = index_file_exists(the_repository->index, src, length, 0))) {
			bad = _("not under version control");
			اذهب act_on_entry;
		}
		אם (ce_stage(ce)) {
			bad = _("conflicted");
			اذهب act_on_entry;
		}
		אם (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			אם (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				אם (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					אם (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = عدم;
				} אחרת
					bad = _("Cannot overwrite");
			}
			اذهب act_on_entry;
		}
		אם (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			اذهب act_on_entry;
		}
		אם (is_dir_sep(dst[strlen(dst) - 1])) {
			bad = _("destination directory does not exist");
			اذهب act_on_entry;
		}
		אם (has_symlink_leading_path(dst, strlen(dst))) {
			bad = _("destination is beyond a symbolic link");
			اذهب act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		אם (needs_worktree_rename(modes[i], dst_mode)) {
			ثابت حرف *slash_ = strrchr(dst, '/');

			אם (slash_) {
				بنية stat dir_st;
				حرف *dst_dir = xstrdup(dst);
				حرف *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				אם (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					אם (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} אחרת אם (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				حرر(dst_dir);
			}

			אם (bad)
				اذهب act_on_entry;
		}

		אם (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    index_entry_exists(the_repository->index, dst, strlen(dst))) {
			bad = _("destination exists in the index");
			אם (force) {
				אם (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = عدم;
			} אחרת {
				اذهب act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		אם (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		אם (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		אם (skip_sparse)
			اذهب remove_entry;

		string_list_insert(&src_for_dst, dst);

act_on_entry:
		אם (!bad)
			استمر;
		אם (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		אם (--argc > 0) {
			صحيح n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	עבור (i = 0; i < argc; i++) {
		ثابت حرف *slash_pos;

		אם (modes[i] & MOVE_VIA_PARENT_DIR)
			استمر;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		כלעוד (slash_pos > pathbuf.buf) {
			بنية pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			אם (hashmap_get_entry(&moved_dirs, &needle, ent, عدم))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	אם (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		אם (!ignore_errors) {
			ret = 1;
			اذهب out;
		}
	}

	עבור (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		تعداد update_mode mode = modes[i];
		صحيح pos;
		صحيح sparse_and_dirty = 0;
		بنية checkout state = CHECKOUT_INIT;
		state.istate = the_repository->index;

		אם (force)
			state.force = 1;
		אם (show_only || verbose)
			printf(_("Renaming %s to %s\n"), src, dst);
		אם (show_only)
			استمر;
		אם (needs_worktree_rename(mode, dst_mode) &&
		    rename(src, dst) < 0) {
			אם (ignore_errors)
				استمر;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		אם (submodule_gitfiles[i]) {
			אם (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			אם (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_tree_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		אם (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			استمر;

		pos = index_name_pos(the_repository->index, src, strlen(src));
		אם (pos < 0)
			BUG("could not find source in index: '%s'", src);
		אם (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->index,
						       the_repository->index->cache[pos],
						       &st,
						       0);
		rename_index_entry_at(the_repository->index, pos, dst);

		אם (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			אם ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->index)) {
				/* from out-of-cone to in-cone */
				صحيح dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				بنية cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				אם (checkout_entry(dst_ce, &state, عدم, عدم))
					die(_("cannot checkout %s"), dst_ce->name);
			} אחרת אם ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->index)) {
				/* from in-cone to out-of-cone */
				صحيح dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				بنية cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				אם (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} אחרת {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					حرف *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					safe_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_empty_src_dirs(src_dir.v, src_dir.nr);

	אם (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	אם (gitmodules_modified)
		stage_updated_gitmodules(the_repository->index);

	אם (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

out:
	strvec_clear(&src_dir);
	حرر(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_match_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_to_free);
	hashmap_clear_and_free(&moved_dirs, بنية pathmap_entry, ent);
	strbuf_release(&pathbuf);
	حرر(submodule_gitfiles);
	حرر(modes);
	החזר ret;
}
