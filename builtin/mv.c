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

स्थिर अचर वर्ण * अचर builtin_mv_usage[] = {
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

स्थिर पूर्णाङ्क needs_worktree_rename(गणना update_mode mode, गणना update_mode dst_mode)
{
	फर्काऊ !(mode & (INDEX | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#define DUP_BASENAME 1
#define KEEP_TRAILING_SLASH 2

स्थिर शून्य internal_prefix_pathspec(संरचना strvec *out,
				     अचर वर्ण *prefix,
				     अचर वर्ण **pathspec,
				     पूर्णाङ्क गन्ती, अचिन्हित flags)
{
	पूर्णाङ्क prefixlen = prefix ? म_माप(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	लागि (पूर्णाङ्क i = 0; i < गन्ती; i++) {
		माप_प्रकार length = म_माप(pathspec[i]);
		माप_प्रकार to_copy = length;
		अचर वर्ण *maybe_basename;
		वर्ण *trimmed, *prefixed_path;

		जबसम्म (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_थप(out, prefixed_path);

		मुक्त(prefixed_path);
		मुक्त(trimmed);
	}
}

स्थिर वर्ण *add_slash(अचर वर्ण *path)
{
	माप_प्रकार len = म_माप(path);
	यदि (len && path[len - 1] != '/') {
		वर्ण *with_slash = xmalloc(st_add(len, 2));
		स_नकल(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		फर्काऊ with_slash;
	}
	फर्काऊ xstrdup(path);
}

#define SUBMODULE_WITH_GITDIR ((अचर वर्ण *)1)

स्थिर अचर वर्ण *submodule_gitfile_path(अचर वर्ण *src, पूर्णाङ्क first)
{
	संरचना strbuf submodule_dotgit = STRBUF_INIT;
	अचर वर्ण *path;

	यदि (!S_ISGITLINK(the_repository->index->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	यदि (!is_staging_gitmodules_ok(the_repository->index))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	यदि (path)
		फर्काऊ path;
	फर्काऊ SUBMODULE_WITH_GITDIR;
}

स्थिर पूर्णाङ्क index_range_of_same_dir(अचर वर्ण *src, पूर्णाङ्क length,
				   पूर्णाङ्क *first_p, पूर्णाङ्क *last_p)
{
	वर्ण *src_w_slash = add_slash(src);
	पूर्णाङ्क first, last, len_w_slash = length + 1;

	first = index_name_pos(the_repository->index, src_w_slash, len_w_slash);
	यदि (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	लागि (last = first; last < the_repository->index->cache_nr; last++) {
		अचर वर्ण *path = the_repository->index->cache[last]->name;
		यदि (म_भेदन(path, src_w_slash, len_w_slash))
			रोक;
	}

	मुक्त(src_w_slash);
	*first_p = first;
	*last_p = last;
	फर्काऊ last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
स्थिर पूर्णाङ्क empty_dir_has_sparse_contents(अचर वर्ण *name)
{
	पूर्णाङ्क ret = 0;
	वर्ण *with_slash = add_slash(name);
	पूर्णाङ्क length = म_माप(with_slash);

	पूर्णाङ्क pos = index_name_pos(the_repository->index, with_slash, length);
	अचर संरचना cache_entry *ce;

	यदि (pos < 0) {
		pos = -pos - 1;
		यदि (pos >= the_repository->index->cache_nr)
			गच्छ free_return;
		ce = the_repository->index->cache[pos];
		यदि (म_भेदन(with_slash, ce->name, length))
			गच्छ free_return;
		यदि (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	मुक्त(with_slash);
	फर्काऊ ret;
}

स्थिर शून्य remove_empty_src_dirs(अचर वर्ण **src_dir, माप_प्रकार src_dir_nr)
{
	माप_प्रकार i;
	संरचना strbuf a_src_dir = STRBUF_INIT;

	लागि (i = 0; i < src_dir_nr; i++) {
		पूर्णाङ्क dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		यदि (index_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

संरचना pathmap_entry {
	संरचना hashmap_entry ent;
	अचर वर्ण *path;
};

स्थिर पूर्णाङ्क pathmap_cmp(अचर शून्य *cmp_data UNUSED,
		       अचर संरचना hashmap_entry *a,
		       अचर संरचना hashmap_entry *b,
		       अचर शून्य *साँचो UNUSED)
{
	अचर संरचना pathmap_entry *e1 = container_of(a, संरचना pathmap_entry, ent);
	अचर संरचना pathmap_entry *e2 = container_of(b, संरचना pathmap_entry, ent);
	फर्काऊ fspathcmp(e1->path, e2->path);
}

पूर्णाङ्क cmd_mv(पूर्णाङ्क argc,
	   अचर वर्ण **argv,
	   अचर वर्ण *prefix,
	   संरचना repository *repo UNUSED)
{
	पूर्णाङ्क i, flags, gitmodules_modified = 0;
	पूर्णाङ्क verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
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
	अचर वर्ण **submodule_gitfiles;
	वर्ण *dst_w_slash = रिक्त;
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
	पूर्णाङ्क ret;
	संरचना repo_config_मानहरू *cfg = repo_config_मानहरू(the_repository);

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	यदि (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	यदि (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	यदि (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, परिमाण(वर्ण *));

	यदि (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	अन्यथा यदि (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} अन्यथा यदि (!path_in_sparse_checkout(dst_w_slash, the_repository->index) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} अन्यथा यदि (argc != 1) {
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
		यदि (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->index))
			dst_mode = SPARSE;
	}

	/* Checking */
	लागि (i = 0; i < argc; i++) {
		अचर वर्ण *src = sources.v[i], *dst = destinations.v[i];
		पूर्णाङ्क length;
		अचर वर्ण *bad = रिक्त;
		पूर्णाङ्क skip_sparse = 0;

		यदि (show_only)
			म_लिखो(_("Checking rename of '%s' to '%s'\n"), src, dst);

		length = म_माप(src);
		यदि (lstat(src, &st) < 0) {
			पूर्णाङ्क pos;
			अचर संरचना cache_entry *ce;

			pos = index_name_pos(the_repository->index, src, length);
			यदि (pos < 0) {
				वर्ण *src_w_slash = add_slash(src);
				यदि (!path_in_sparse_checkout(src_w_slash, the_repository->index) &&
				    empty_dir_has_sparse_contents(src)) {
					मुक्त(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					गच्छ dir_check;
				}
				मुक्त(src_w_slash);
				/* only error if existence is expected. */
				यदि (!(modes[i] & SPARSE))
					bad = _("bad source");
				गच्छ act_on_entry;
			}
			ce = the_repository->index->cache[pos];
			यदि (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				गच्छ act_on_entry;
			}
			यदि (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				गच्छ act_on_entry;
			}
			/* Check if dst exists in index */
			यदि (index_name_pos(the_repository->index, dst, म_माप(dst)) < 0) {
				modes[i] |= SPARSE;
				गच्छ act_on_entry;
			}
			यदि (!force) {
				bad = _("destination exists");
				गच्छ act_on_entry;
			}
			modes[i] |= SPARSE;
			गच्छ act_on_entry;
		}
		यदि (!म_भेदन(src, dst, length) &&
		    (dst[length] == 0 || dst[length] == '/')) {
			bad = _("can not move directory into itself");
			गच्छ act_on_entry;
		}
		यदि (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			गच्छ act_on_entry;
		}

dir_check:
		यदि (S_ISDIR(st.st_mode)) {
			संरचना pathmap_entry *entry;
			वर्ण *dst_with_slash;
			माप_प्रकार dst_with_slash_len;
			पूर्णाङ्क j, n;
			पूर्णाङ्क first = index_name_pos(the_repository->index, src, length), last;

			यदि (first >= 0) {
				अचर वर्ण *path = submodule_gitfile_path(src, first);
				यदि (path != SUBMODULE_WITH_GITDIR)
					path = strvec_थप(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				गच्छ act_on_entry;
			} अन्यथा यदि (index_range_of_same_dir(src, length,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				गच्छ act_on_entry;
			}

			entry = xmalloc(परिमाण(*entry));
			entry->path = src;
			hashmap_entry_init(&entry->ent, fspathhash(src));
			hashmap_add(&moved_dirs, &entry->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_थप(&src_dir, src);

			n = argc + last - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = add_slash(dst);
			dst_with_slash_len = म_माप(dst_with_slash);

			लागि (j = 0; j < last - first; j++) {
				अचर संरचना cache_entry *ce = the_repository->index->cache[first + j];
				अचर वर्ण *path = ce->name;
				वर्ण *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + length + 1);

				strvec_थप(&sources, path);
				strvec_थप(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : INDEX);
				submodule_gitfiles[argc + j] = रिक्त;

				मुक्त(prefixed_path);
			}

			मुक्त(dst_with_slash);
			argc += last - first;
			गच्छ act_on_entry;
		}
		यदि (!(ce = index_file_exists(the_repository->index, src, length, 0))) {
			bad = _("not under version control");
			गच्छ act_on_entry;
		}
		यदि (ce_stage(ce)) {
			bad = _("conflicted");
			गच्छ act_on_entry;
		}
		यदि (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			यदि (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				यदि (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					यदि (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = रिक्त;
				} अन्यथा
					bad = _("Cannot overwrite");
			}
			गच्छ act_on_entry;
		}
		यदि (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			गच्छ act_on_entry;
		}
		यदि (is_dir_sep(dst[म_माप(dst) - 1])) {
			bad = _("destination directory does not exist");
			गच्छ act_on_entry;
		}
		यदि (has_symlink_leading_path(dst, म_माप(dst))) {
			bad = _("destination is beyond a symbolic link");
			गच्छ act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		यदि (needs_worktree_rename(modes[i], dst_mode)) {
			अचर वर्ण *slash_ = म_खोजप(dst, '/');

			यदि (slash_) {
				संरचना stat dir_st;
				वर्ण *dst_dir = xstrdup(dst);
				वर्ण *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				यदि (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					यदि (त्रुटि_सं == ENOENT || त्रुटि_सं == ENOTDIR)
						bad = _("destination directory does not exist");
				} अन्यथा यदि (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				मुक्त(dst_dir);
			}

			यदि (bad)
				गच्छ act_on_entry;
		}

		यदि (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    index_entry_exists(the_repository->index, dst, म_माप(dst))) {
			bad = _("destination exists in the index");
			यदि (force) {
				यदि (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = रिक्त;
			} अन्यथा {
				गच्छ act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		यदि (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		यदि (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		यदि (skip_sparse)
			गच्छ remove_entry;

		string_list_घुसाउ(&src_for_dst, dst);

act_on_entry:
		यदि (!bad)
			जारी;
		यदि (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		यदि (--argc > 0) {
			पूर्णाङ्क n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	लागि (i = 0; i < argc; i++) {
		अचर वर्ण *slash_pos;

		यदि (modes[i] & MOVE_VIA_PARENT_DIR)
			जारी;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = म_खोजप(pathbuf.buf, '/');
		जबसम्म (slash_pos > pathbuf.buf) {
			संरचना pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			यदि (hashmap_get_entry(&moved_dirs, &needle, ent, रिक्त))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = म_खोजप(pathbuf.buf, '/');
		}
	}

	यदि (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		यदि (!ignore_errors) {
			ret = 1;
			गच्छ out;
		}
	}

	लागि (i = 0; i < argc; i++) {
		अचर वर्ण *src = sources.v[i], *dst = destinations.v[i];
		गणना update_mode mode = modes[i];
		पूर्णाङ्क pos;
		पूर्णाङ्क sparse_and_dirty = 0;
		संरचना checkout state = CHECKOUT_INIT;
		state.istate = the_repository->index;

		यदि (force)
			state.force = 1;
		यदि (show_only || verbose)
			म_लिखो(_("Renaming %s to %s\n"), src, dst);
		यदि (show_only)
			जारी;
		यदि (needs_worktree_rename(mode, dst_mode) &&
		    नाम(src, dst) < 0) {
			यदि (ignore_errors)
				जारी;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		यदि (submodule_gitfiles[i]) {
			यदि (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			यदि (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_रूख_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		यदि (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			जारी;

		pos = index_name_pos(the_repository->index, src, म_माप(src));
		यदि (pos < 0)
			BUG("could not find source in index: '%s'", src);
		यदि (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->index,
						       the_repository->index->cache[pos],
						       &st,
						       0);
		rename_index_entry_at(the_repository->index, pos, dst);

		यदि (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			यदि ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->index)) {
				/* from out-of-cone to in-cone */
				पूर्णाङ्क dst_pos = index_name_pos(the_repository->index, dst,
							     म_माप(dst));
				संरचना cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				यदि (checkout_entry(dst_ce, &state, रिक्त, रिक्त))
					die(_("cannot checkout %s"), dst_ce->name);
			} अन्यथा यदि ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->index)) {
				/* from in-cone to out-of-cone */
				पूर्णाङ्क dst_pos = index_name_pos(the_repository->index, dst,
							     म_माप(dst));
				संरचना cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				यदि (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} अन्यथा {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					वर्ण *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					safe_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					नाम(src, dst);
				}
			}
		}
	}

	remove_empty_src_dirs(src_dir.v, src_dir.nr);

	यदि (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	यदि (gitmodules_modified)
		stage_updated_gitmodules(the_repository->index);

	यदि (write_locked_index(the_repository->index, &lock_file,
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
	फर्काऊ ret;
}
