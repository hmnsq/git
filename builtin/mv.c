/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "name-hash.h"
#समावेश "object-file.h"
#समावेश "path.h"
#समावेश "pathspec.h"
#समावेश "lockfile.h"
#समावेश "dir.h"
#समावेश "string-list.h"
#समावेश "parse-options.h"
#समावेश "read-cache-ll.h"
#समावेश "symlinks.h"

#समावेश "setup.h"
#समावेश "strvec.h"
#समावेश "submodule.h"
#समावेश "entry.h"

अटल स्थिर अक्षर * स्थिर builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	शून्य
};

क्रमागत update_mode {
	WORKING_DIRECTORY = (1 << 1),
	सूचकांक_2 = (1 << 2),
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

अटल पूर्णांक needs_worktree_rename(क्रमागत update_mode mode, क्रमागत update_mode dst_mode)
{
	वापस !(mode & (सूचकांक_2 | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#घोषणा DUP_BASENAME 1
#घोषणा KEEP_TRAILING_SLASH 2

अटल व्योम internal_prefix_pathspec(काष्ठा strvec *out,
				     स्थिर अक्षर *prefix,
				     स्थिर अक्षर **pathspec,
				     पूर्णांक गिनती, अचिन्हित flags)
{
	पूर्णांक prefixlen = prefix ? म_माप(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	क्रम (पूर्णांक i = 0; i < गिनती; i++) {
		माप_प्रकार लंबाई = म_माप(pathspec[i]);
		माप_प्रकार to_copy = लंबाई;
		स्थिर अक्षर *maybe_basename;
		अक्षर *trimmed, *prefixed_path;

		जबतक (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_धकेलो(out, prefixed_path);

		मुक्त(prefixed_path);
		मुक्त(trimmed);
	}
}

अटल अक्षर *add_slash(स्थिर अक्षर *path)
{
	माप_प्रकार len = म_माप(path);
	अगर (len && path[len - 1] != '/') {
		अक्षर *with_slash = xmalloc(st_add(len, 2));
		स_नकल(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		वापस with_slash;
	}
	वापस xstrdup(path);
}

#घोषणा SUBMODULE_WITH_GITDIR ((स्थिर अक्षर *)1)

अटल स्थिर अक्षर *submodule_gitfile_path(स्थिर अक्षर *src, पूर्णांक first)
{
	काष्ठा strbuf submodule_dotgit = STRBUF_INIT;
	स्थिर अक्षर *path;

	अगर (!S_ISGITLINK(the_repository->सूचकांक->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	अगर (!is_staging_gitmodules_ok(the_repository->सूचकांक))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	अगर (path)
		वापस path;
	वापस SUBMODULE_WITH_GITDIR;
}

अटल पूर्णांक सूचकांक_range_of_same_dir(स्थिर अक्षर *src, पूर्णांक लंबाई,
				   पूर्णांक *first_p, पूर्णांक *last_p)
{
	अक्षर *src_w_slash = add_slash(src);
	पूर्णांक first, last, len_w_slash = लंबाई + 1;

	first = सूचकांक_name_pos(the_repository->सूचकांक, src_w_slash, len_w_slash);
	अगर (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	क्रम (last = first; last < the_repository->सूचकांक->cache_nr; last++) {
		स्थिर अक्षर *path = the_repository->सूचकांक->cache[last]->name;
		अगर (म_भेदन(path, src_w_slash, len_w_slash))
			अवरोध;
	}

	मुक्त(src_w_slash);
	*first_p = first;
	*last_p = last;
	वापस last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
अटल पूर्णांक empty_dir_has_sparse_contents(स्थिर अक्षर *name)
{
	पूर्णांक ret = 0;
	अक्षर *with_slash = add_slash(name);
	पूर्णांक लंबाई = म_माप(with_slash);

	पूर्णांक pos = सूचकांक_name_pos(the_repository->सूचकांक, with_slash, लंबाई);
	स्थिर काष्ठा cache_entry *ce;

	अगर (pos < 0) {
		pos = -pos - 1;
		अगर (pos >= the_repository->सूचकांक->cache_nr)
			जाओ free_return;
		ce = the_repository->सूचकांक->cache[pos];
		अगर (म_भेदन(with_slash, ce->name, लंबाई))
			जाओ free_return;
		अगर (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	मुक्त(with_slash);
	वापस ret;
}

अटल व्योम remove_empty_src_dirs(स्थिर अक्षर **src_dir, माप_प्रकार src_dir_nr)
{
	माप_प्रकार i;
	काष्ठा strbuf a_src_dir = STRBUF_INIT;

	क्रम (i = 0; i < src_dir_nr; i++) {
		पूर्णांक dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		अगर (सूचकांक_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

काष्ठा pathmap_entry {
	काष्ठा hashmap_entry ent;
	स्थिर अक्षर *path;
};

अटल पूर्णांक pathmap_cmp(स्थिर व्योम *cmp_तथ्य UNUSED,
		       स्थिर काष्ठा hashmap_entry *a,
		       स्थिर काष्ठा hashmap_entry *b,
		       स्थिर व्योम *कुंजी UNUSED)
{
	स्थिर काष्ठा pathmap_entry *e1 = container_of(a, काष्ठा pathmap_entry, ent);
	स्थिर काष्ठा pathmap_entry *e2 = container_of(b, काष्ठा pathmap_entry, ent);
	वापस fspathcmp(e1->path, e2->path);
}

पूर्णांक cmd_mv(पूर्णांक argc,
	   स्थिर अक्षर **argv,
	   स्थिर अक्षर *prefix,
	   काष्ठा repository *repo UNUSED)
{
	पूर्णांक i, flags, gitmodules_modified = 0;
	पूर्णांक verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	काष्ठा option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT__DRY_RUN(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', शून्य, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_END(),
	};
	काष्ठा strvec sources = STRVEC_INIT;
	काष्ठा strvec dest_paths = STRVEC_INIT;
	काष्ठा strvec destinations = STRVEC_INIT;
	काष्ठा strvec submodule_gitfiles_to_free = STRVEC_INIT;
	स्थिर अक्षर **submodule_gitfiles;
	अक्षर *dst_w_slash = शून्य;
	काष्ठा strvec src_dir = STRVEC_INIT;
	क्रमागत update_mode *modes, dst_mode = 0;
	काष्ठा stat st, dest_st;
	काष्ठा string_list src_for_dst = STRING_LIST_INIT_DUP;
	काष्ठा lock_file lock_file = LOCK_INIT;
	काष्ठा cache_entry *ce;
	काष्ठा string_list only_match_skip_worktree = STRING_LIST_INIT_DUP;
	काष्ठा string_list dirty_paths = STRING_LIST_INIT_DUP;
	काष्ठा hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, शून्य);
	काष्ठा strbuf pathbuf = STRBUF_INIT;
	पूर्णांक ret;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	repo_config(the_repository, git_default_config, शून्य);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	अगर (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_सूचकांक(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	अगर (repo_read_सूचकांक(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	अगर (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, माप(अक्षर *));

	अगर (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	अन्यथा अगर (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} अन्यथा अगर (!path_in_sparse_checkout(dst_w_slash, the_repository->सूचकांक) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} अन्यथा अगर (argc != 1) {
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
		अगर (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->सूचकांक))
			dst_mode = SPARSE;
	}

	/* Checking */
	क्रम (i = 0; i < argc; i++) {
		स्थिर अक्षर *src = sources.v[i], *dst = destinations.v[i];
		पूर्णांक लंबाई;
		स्थिर अक्षर *bad = शून्य;
		पूर्णांक skip_sparse = 0;

		अगर (show_only)
			म_लिखो(_("Checking rename of '%s' to '%s'\n"), src, dst);

		लंबाई = म_माप(src);
		अगर (lstat(src, &st) < 0) {
			पूर्णांक pos;
			स्थिर काष्ठा cache_entry *ce;

			pos = सूचकांक_name_pos(the_repository->सूचकांक, src, लंबाई);
			अगर (pos < 0) {
				अक्षर *src_w_slash = add_slash(src);
				अगर (!path_in_sparse_checkout(src_w_slash, the_repository->सूचकांक) &&
				    empty_dir_has_sparse_contents(src)) {
					मुक्त(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					जाओ dir_check;
				}
				मुक्त(src_w_slash);
				/* only error if existence is expected. */
				अगर (!(modes[i] & SPARSE))
					bad = _("bad source");
				जाओ act_on_entry;
			}
			ce = the_repository->सूचकांक->cache[pos];
			अगर (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				जाओ act_on_entry;
			}
			अगर (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				जाओ act_on_entry;
			}
			/* Check if dst exists in index */
			अगर (सूचकांक_name_pos(the_repository->सूचकांक, dst, म_माप(dst)) < 0) {
				modes[i] |= SPARSE;
				जाओ act_on_entry;
			}
			अगर (!force) {
				bad = _("destination exists");
				जाओ act_on_entry;
			}
			modes[i] |= SPARSE;
			जाओ act_on_entry;
		}
		अगर (!म_भेदन(src, dst, लंबाई) &&
		    (dst[लंबाई] == 0 || dst[लंबाई] == '/')) {
			bad = _("can not move directory into itself");
			जाओ act_on_entry;
		}
		अगर (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			जाओ act_on_entry;
		}

dir_check:
		अगर (S_ISDIR(st.st_mode)) {
			काष्ठा pathmap_entry *entry;
			अक्षर *dst_with_slash;
			माप_प्रकार dst_with_slash_len;
			पूर्णांक j, n;
			पूर्णांक first = सूचकांक_name_pos(the_repository->सूचकांक, src, लंबाई), last;

			अगर (first >= 0) {
				स्थिर अक्षर *path = submodule_gitfile_path(src, first);
				अगर (path != SUBMODULE_WITH_GITDIR)
					path = strvec_धकेलो(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				जाओ act_on_entry;
			} अन्यथा अगर (सूचकांक_range_of_same_dir(src, लंबाई,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				जाओ act_on_entry;
			}

			entry = xmalloc(माप(*entry));
			entry->path = src;
			hashmap_entry_init(&entry->ent, fspathhash(src));
			hashmap_add(&moved_dirs, &entry->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_धकेलो(&src_dir, src);

			n = argc + last - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = add_slash(dst);
			dst_with_slash_len = म_माप(dst_with_slash);

			क्रम (j = 0; j < last - first; j++) {
				स्थिर काष्ठा cache_entry *ce = the_repository->सूचकांक->cache[first + j];
				स्थिर अक्षर *path = ce->name;
				अक्षर *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + लंबाई + 1);

				strvec_धकेलो(&sources, path);
				strvec_धकेलो(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : सूचकांक_2);
				submodule_gitfiles[argc + j] = शून्य;

				मुक्त(prefixed_path);
			}

			मुक्त(dst_with_slash);
			argc += last - first;
			जाओ act_on_entry;
		}
		अगर (!(ce = सूचकांक_file_exists(the_repository->सूचकांक, src, लंबाई, 0))) {
			bad = _("not under version control");
			जाओ act_on_entry;
		}
		अगर (ce_stage(ce)) {
			bad = _("conflicted");
			जाओ act_on_entry;
		}
		अगर (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			अगर (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				अगर (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					अगर (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = शून्य;
				} अन्यथा
					bad = _("Cannot overwrite");
			}
			जाओ act_on_entry;
		}
		अगर (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			जाओ act_on_entry;
		}
		अगर (is_dir_sep(dst[म_माप(dst) - 1])) {
			bad = _("destination directory does not exist");
			जाओ act_on_entry;
		}
		अगर (has_symlink_leading_path(dst, म_माप(dst))) {
			bad = _("destination is beyond a symbolic link");
			जाओ act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		अगर (needs_worktree_rename(modes[i], dst_mode)) {
			स्थिर अक्षर *slash_ = म_खोजप(dst, '/');

			अगर (slash_) {
				काष्ठा stat dir_st;
				अक्षर *dst_dir = xstrdup(dst);
				अक्षर *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				अगर (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					अगर (त्रुटि_सं == ENOENT || त्रुटि_सं == ENOTDIR)
						bad = _("destination directory does not exist");
				} अन्यथा अगर (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				मुक्त(dst_dir);
			}

			अगर (bad)
				जाओ act_on_entry;
		}

		अगर (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    सूचकांक_entry_exists(the_repository->सूचकांक, dst, म_माप(dst))) {
			bad = _("destination exists in the index");
			अगर (force) {
				अगर (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = शून्य;
			} अन्यथा {
				जाओ act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		अगर (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->सूचकांक)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		अगर (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->सूचकांक)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		अगर (skip_sparse)
			जाओ remove_entry;

		string_list_जोड़ो(&src_for_dst, dst);

act_on_entry:
		अगर (!bad)
			जारी;
		अगर (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		अगर (--argc > 0) {
			पूर्णांक n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	क्रम (i = 0; i < argc; i++) {
		स्थिर अक्षर *slash_pos;

		अगर (modes[i] & MOVE_VIA_PARENT_DIR)
			जारी;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = म_खोजप(pathbuf.buf, '/');
		जबतक (slash_pos > pathbuf.buf) {
			काष्ठा pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			अगर (hashmap_get_entry(&moved_dirs, &needle, ent, शून्य))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = म_खोजप(pathbuf.buf, '/');
		}
	}

	अगर (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		अगर (!ignore_errors) {
			ret = 1;
			जाओ out;
		}
	}

	क्रम (i = 0; i < argc; i++) {
		स्थिर अक्षर *src = sources.v[i], *dst = destinations.v[i];
		क्रमागत update_mode mode = modes[i];
		पूर्णांक pos;
		पूर्णांक sparse_and_dirty = 0;
		काष्ठा checkout state = CHECKOUT_INIT;
		state.istate = the_repository->सूचकांक;

		अगर (force)
			state.force = 1;
		अगर (show_only || verbose)
			म_लिखो(_("Renaming %s to %s\n"), src, dst);
		अगर (show_only)
			जारी;
		अगर (needs_worktree_rename(mode, dst_mode) &&
		    नाम(src, dst) < 0) {
			अगर (ignore_errors)
				जारी;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		अगर (submodule_gitfiles[i]) {
			अगर (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			अगर (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_वृक्ष_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		अगर (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			जारी;

		pos = सूचकांक_name_pos(the_repository->सूचकांक, src, म_माप(src));
		अगर (pos < 0)
			BUG("could not find source in index: '%s'", src);
		अगर (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->सूचकांक,
						       the_repository->सूचकांक->cache[pos],
						       &st,
						       0);
		rename_सूचकांक_entry_at(the_repository->सूचकांक, pos, dst);

		अगर (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			अगर ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->सूचकांक)) {
				/* from out-of-cone to in-cone */
				पूर्णांक dst_pos = सूचकांक_name_pos(the_repository->सूचकांक, dst,
							     म_माप(dst));
				काष्ठा cache_entry *dst_ce = the_repository->सूचकांक->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				अगर (checkout_entry(dst_ce, &state, शून्य, शून्य))
					die(_("cannot checkout %s"), dst_ce->name);
			} अन्यथा अगर ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->सूचकांक)) {
				/* from in-cone to out-of-cone */
				पूर्णांक dst_pos = सूचकांक_name_pos(the_repository->सूचकांक, dst,
							     म_माप(dst));
				काष्ठा cache_entry *dst_ce = the_repository->सूचकांक->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				अगर (!sparse_and_dirty) {
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

	अगर (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	अगर (gitmodules_modified)
		stage_updated_gitmodules(the_repository->सूचकांक);

	अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock_file,
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
	hashmap_clear_and_free(&moved_dirs, काष्ठा pathmap_entry, ent);
	strbuf_release(&pathbuf);
	मुक्त(submodule_gitfiles);
	मुक्त(modes);
	वापस ret;
}
