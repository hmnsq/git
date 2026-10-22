/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "name-hash.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "symlinks.h"

#अन्तर्भाव "setup.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "entry.h"

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	रिक्त
};

गणना update_mode {
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

स्थिर पूर्णांक needs_worktree_rename(गणना update_mode mode, गणना update_mode dst_mode)
{
	परत !(mode & (INDEX | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#define DUP_BASENAME 1
#define KEEP_TRAILING_SLASH 2

स्थिर शून्य internal_prefix_pathspec(संरचना strvec *out,
				     स्थिरांक अक्षर *prefix,
				     स्थिरांक अक्षर **pathspec,
				     पूर्णांक मोजणी, अचिन्हित flags)
{
	पूर्णांक prefixlen = prefix ? म_माप(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	साठी (पूर्णांक i = 0; i < मोजणी; i++) {
		माप_प्रकार length = म_माप(pathspec[i]);
		माप_प्रकार to_copy = length;
		स्थिरांक अक्षर *maybe_basename;
		अक्षर *trimmed, *prefixed_path;

		पर्यंत (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_ढकला(out, prefixed_path);

		मुक्त(prefixed_path);
		मुक्त(trimmed);
	}
}

स्थिर अक्षर *add_slash(स्थिरांक अक्षर *path)
{
	माप_प्रकार len = म_माप(path);
	जर (len && path[len - 1] != '/') {
		अक्षर *with_slash = xmalloc(st_add(len, 2));
		स_नकल(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		परत with_slash;
	}
	परत xstrdup(path);
}

#define SUBMODULE_WITH_GITDIR ((स्थिरांक अक्षर *)1)

स्थिर स्थिरांक अक्षर *submodule_gitfile_path(स्थिरांक अक्षर *src, पूर्णांक first)
{
	संरचना strbuf submodule_dotgit = STRBUF_INIT;
	स्थिरांक अक्षर *path;

	जर (!S_ISGITLINK(the_repository->index->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	जर (!is_staging_gitmodules_ok(the_repository->index))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	जर (path)
		परत path;
	परत SUBMODULE_WITH_GITDIR;
}

स्थिर पूर्णांक index_range_of_same_dir(स्थिरांक अक्षर *src, पूर्णांक length,
				   पूर्णांक *first_p, पूर्णांक *last_p)
{
	अक्षर *src_w_slash = add_slash(src);
	पूर्णांक first, last, len_w_slash = length + 1;

	first = index_name_pos(the_repository->index, src_w_slash, len_w_slash);
	जर (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	साठी (last = first; last < the_repository->index->cache_nr; last++) {
		स्थिरांक अक्षर *path = the_repository->index->cache[last]->name;
		जर (म_भेदन(path, src_w_slash, len_w_slash))
			थांबा;
	}

	मुक्त(src_w_slash);
	*first_p = first;
	*last_p = last;
	परत last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
स्थिर पूर्णांक empty_dir_has_sparse_contents(स्थिरांक अक्षर *name)
{
	पूर्णांक ret = 0;
	अक्षर *with_slash = add_slash(name);
	पूर्णांक length = म_माप(with_slash);

	पूर्णांक pos = index_name_pos(the_repository->index, with_slash, length);
	स्थिरांक संरचना cache_entry *ce;

	जर (pos < 0) {
		pos = -pos - 1;
		जर (pos >= the_repository->index->cache_nr)
			जा free_return;
		ce = the_repository->index->cache[pos];
		जर (म_भेदन(with_slash, ce->name, length))
			जा free_return;
		जर (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	मुक्त(with_slash);
	परत ret;
}

स्थिर शून्य remove_empty_src_dirs(स्थिरांक अक्षर **src_dir, माप_प्रकार src_dir_nr)
{
	माप_प्रकार i;
	संरचना strbuf a_src_dir = STRBUF_INIT;

	साठी (i = 0; i < src_dir_nr; i++) {
		पूर्णांक dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		जर (index_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

संरचना pathmap_entry {
	संरचना hashmap_entry ent;
	स्थिरांक अक्षर *path;
};

स्थिर पूर्णांक pathmap_cmp(स्थिरांक शून्य *cmp_data UNUSED,
		       स्थिरांक संरचना hashmap_entry *a,
		       स्थिरांक संरचना hashmap_entry *b,
		       स्थिरांक शून्य *किल्ली UNUSED)
{
	स्थिरांक संरचना pathmap_entry *e1 = container_of(a, संरचना pathmap_entry, ent);
	स्थिरांक संरचना pathmap_entry *e2 = container_of(b, संरचना pathmap_entry, ent);
	परत fspathcmp(e1->path, e2->path);
}

पूर्णांक cmd_mv(पूर्णांक argc,
	   स्थिरांक अक्षर **argv,
	   स्थिरांक अक्षर *prefix,
	   संरचना repository *repo UNUSED)
{
	पूर्णांक i, flags, gitmodules_modified = 0;
	पूर्णांक verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	संरचना option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT__DRY_RUN(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', रिक्त, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_END(),
	};
	संरचना strvec sources = STRVEC_INIT;
	संरचना strvec dest_paths = STRVEC_INIT;
	संरचना strvec destinations = STRVEC_INIT;
	संरचना strvec submodule_gitfiles_to_free = STRVEC_INIT;
	स्थिरांक अक्षर **submodule_gitfiles;
	अक्षर *dst_w_slash = रिक्त;
	संरचना strvec src_dir = STRVEC_INIT;
	गणना update_mode *modes, dst_mode = 0;
	संरचना stat st, dest_st;
	संरचना string_list src_for_dst = STRING_LIST_INIT_DUP;
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना cache_entry *ce;
	संरचना string_list only_match_skip_worktree = STRING_LIST_INIT_DUP;
	संरचना string_list dirty_paths = STRING_LIST_INIT_DUP;
	संरचना hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, रिक्त);
	संरचना strbuf pathbuf = STRBUF_INIT;
	पूर्णांक ret;
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	जर (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	जर (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	जर (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, आकार(अक्षर *));

	जर (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	अन्यथा जर (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} अन्यथा जर (!path_in_sparse_checkout(dst_w_slash, the_repository->index) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} अन्यथा जर (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} अन्यथा {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		जर (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->index))
			dst_mode = SPARSE;
	}

	/* Checking */
	साठी (i = 0; i < argc; i++) {
		स्थिरांक अक्षर *src = sources.v[i], *dst = destinations.v[i];
		पूर्णांक length;
		स्थिरांक अक्षर *bad = रिक्त;
		पूर्णांक skip_sparse = 0;

		जर (show_only)
			म_लिखो(_("Checking rename of '%s' to '%s'\n"), src, dst);

		length = म_माप(src);
		जर (lstat(src, &st) < 0) {
			पूर्णांक pos;
			स्थिरांक संरचना cache_entry *ce;

			pos = index_name_pos(the_repository->index, src, length);
			जर (pos < 0) {
				अक्षर *src_w_slash = add_slash(src);
				जर (!path_in_sparse_checkout(src_w_slash, the_repository->index) &&
				    empty_dir_has_sparse_contents(src)) {
					मुक्त(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					जा dir_check;
				}
				मुक्त(src_w_slash);
				/* only error if existence is expected. */
				जर (!(modes[i] & SPARSE))
					bad = _("bad source");
				जा act_on_entry;
			}
			ce = the_repository->index->cache[pos];
			जर (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				जा act_on_entry;
			}
			जर (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				जा act_on_entry;
			}
			/* Check if dst exists in index */
			जर (index_name_pos(the_repository->index, dst, म_माप(dst)) < 0) {
				modes[i] |= SPARSE;
				जा act_on_entry;
			}
			जर (!force) {
				bad = _("destination exists");
				जा act_on_entry;
			}
			modes[i] |= SPARSE;
			जा act_on_entry;
		}
		जर (!म_भेदन(src, dst, length) &&
		    (dst[length] == 0 || dst[length] == '/')) {
			bad = _("can not move directory into itself");
			जा act_on_entry;
		}
		जर (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			जा act_on_entry;
		}

dir_check:
		जर (S_ISDIR(st.st_mode)) {
			संरचना pathmap_entry *entry;
			अक्षर *dst_with_slash;
			माप_प्रकार dst_with_slash_len;
			पूर्णांक j, n;
			पूर्णांक first = index_name_pos(the_repository->index, src, length), last;

			जर (first >= 0) {
				स्थिरांक अक्षर *path = submodule_gitfile_path(src, first);
				जर (path != SUBMODULE_WITH_GITDIR)
					path = strvec_ढकला(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				जा act_on_entry;
			} अन्यथा जर (index_range_of_same_dir(src, length,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				जा act_on_entry;
			}

			entry = xmalloc(आकार(*entry));
			entry->path = src;
			hashmap_entry_init(&entry->ent, fspathhash(src));
			hashmap_add(&moved_dirs, &entry->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_ढकला(&src_dir, src);

			n = argc + last - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = add_slash(dst);
			dst_with_slash_len = म_माप(dst_with_slash);

			साठी (j = 0; j < last - first; j++) {
				स्थिरांक संरचना cache_entry *ce = the_repository->index->cache[first + j];
				स्थिरांक अक्षर *path = ce->name;
				अक्षर *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + length + 1);

				strvec_ढकला(&sources, path);
				strvec_ढकला(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : INDEX);
				submodule_gitfiles[argc + j] = रिक्त;

				मुक्त(prefixed_path);
			}

			मुक्त(dst_with_slash);
			argc += last - first;
			जा act_on_entry;
		}
		जर (!(ce = index_file_exists(the_repository->index, src, length, 0))) {
			bad = _("not under version control");
			जा act_on_entry;
		}
		जर (ce_stage(ce)) {
			bad = _("conflicted");
			जा act_on_entry;
		}
		जर (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			जर (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				जर (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					जर (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = रिक्त;
				} अन्यथा
					bad = _("Cannot overwrite");
			}
			जा act_on_entry;
		}
		जर (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			जा act_on_entry;
		}
		जर (is_dir_sep(dst[म_माप(dst) - 1])) {
			bad = _("destination directory does not exist");
			जा act_on_entry;
		}
		जर (has_symlink_leading_path(dst, म_माप(dst))) {
			bad = _("destination is beyond a symbolic link");
			जा act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		जर (needs_worktree_rename(modes[i], dst_mode)) {
			स्थिरांक अक्षर *slash_ = म_खोजप(dst, '/');

			जर (slash_) {
				संरचना stat dir_st;
				अक्षर *dst_dir = xstrdup(dst);
				अक्षर *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				जर (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					जर (त्रुटि_सं == ENOENT || त्रुटि_सं == ENOTDIR)
						bad = _("destination directory does not exist");
				} अन्यथा जर (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				मुक्त(dst_dir);
			}

			जर (bad)
				जा act_on_entry;
		}

		जर (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    index_entry_exists(the_repository->index, dst, म_माप(dst))) {
			bad = _("destination exists in the index");
			जर (force) {
				जर (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = रिक्त;
			} अन्यथा {
				जा act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		जर (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		जर (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		जर (skip_sparse)
			जा remove_entry;

		string_list_घाला(&src_for_dst, dst);

act_on_entry:
		जर (!bad)
			सुरूठेवा;
		जर (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		जर (--argc > 0) {
			पूर्णांक n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	साठी (i = 0; i < argc; i++) {
		स्थिरांक अक्षर *slash_pos;

		जर (modes[i] & MOVE_VIA_PARENT_DIR)
			सुरूठेवा;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = म_खोजप(pathbuf.buf, '/');
		पर्यंत (slash_pos > pathbuf.buf) {
			संरचना pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			जर (hashmap_get_entry(&moved_dirs, &needle, ent, रिक्त))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = म_खोजप(pathbuf.buf, '/');
		}
	}

	जर (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		जर (!ignore_errors) {
			ret = 1;
			जा out;
		}
	}

	साठी (i = 0; i < argc; i++) {
		स्थिरांक अक्षर *src = sources.v[i], *dst = destinations.v[i];
		गणना update_mode mode = modes[i];
		पूर्णांक pos;
		पूर्णांक sparse_and_dirty = 0;
		संरचना checkout state = CHECKOUT_INIT;
		state.istate = the_repository->index;

		जर (force)
			state.force = 1;
		जर (show_only || verbose)
			म_लिखो(_("Renaming %s to %s\n"), src, dst);
		जर (show_only)
			सुरूठेवा;
		जर (needs_worktree_rename(mode, dst_mode) &&
		    नाम(src, dst) < 0) {
			जर (ignore_errors)
				सुरूठेवा;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		जर (submodule_gitfiles[i]) {
			जर (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			जर (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_वृक्ष_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		जर (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			सुरूठेवा;

		pos = index_name_pos(the_repository->index, src, म_माप(src));
		जर (pos < 0)
			BUG("could not find source in index: '%s'", src);
		जर (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->index,
						       the_repository->index->cache[pos],
						       &st,
						       0);
		rename_index_entry_at(the_repository->index, pos, dst);

		जर (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			जर ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->index)) {
				/* from out-of-cone to in-cone */
				पूर्णांक dst_pos = index_name_pos(the_repository->index, dst,
							     म_माप(dst));
				संरचना cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				जर (checkout_entry(dst_ce, &state, रिक्त, रिक्त))
					die(_("cannot checkout %s"), dst_ce->name);
			} अन्यथा जर ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->index)) {
				/* from in-cone to out-of-cone */
				पूर्णांक dst_pos = index_name_pos(the_repository->index, dst,
							     म_माप(dst));
				संरचना cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				जर (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} अन्यथा {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					अक्षर *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					safe_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					नाम(src, dst);
				}
			}
		}
	}

	remove_empty_src_dirs(src_dir.v, src_dir.nr);

	जर (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	जर (gitmodules_modified)
		stage_updated_gitmodules(the_repository->index);

	जर (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

out:
	strvec_clear(&src_dir);
	मुक्त(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_match_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_to_free);
	hashmap_clear_and_free(&moved_dirs, संरचना pathmap_entry, ent);
	strbuf_release(&pathbuf);
	मुक्त(submodule_gitfiles);
	मुक्त(modes);
	परत ret;
}
