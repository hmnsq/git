/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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

स्थिर ध्रुव वर्ण * ध्रुव builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	रिक्त
};

गणना update_बहुलकम् {
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
	MOVE_द्वारा_PARENT_DIR = (1 << 5),
};

स्थिर पूर्णाङ्क needs_worktree_rename(गणना update_बहुलकम् बहुलकम्, गणना update_बहुलकम् dst_बहुलकम्)
{
	प्रत्यागम !(बहुलकम् & (सूचकांक_2 | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_बहुलकम् & (SKIP_WORKTREE_DIR | SPARSE));
}

#परिभाषय DUP_BASENAME 1
#परिभाषय KEEP_TRAILING_SLASH 2

स्थिर शून्य आन्तरिकम्_उपसर्गः_pathspec(संरचना strvec *बहिर्गमनम्,
				     ध्रुव वर्ण *उपसर्गः,
				     ध्रुव वर्ण **pathspec,
				     पूर्णाङ्क गणनम्, अचिह्नित flags)
{
	पूर्णाङ्क prefixlen = उपसर्गः ? वाक्यदैर्घ्यम्(उपसर्गः) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	प्रति (पूर्णाङ्क i = 0; i < गणनम्; i++) {
		आकार_t दीर्घता = वाक्यदैर्घ्यम्(pathspec[i]);
		आकार_t गन्तव्यम्_copy = दीर्घता;
		ध्रुव वर्ण *maybe_basename;
		वर्ण *trimmed, *prefixed_मार्गः;

		यावत् (!(flags & KEEP_TRAILING_SLASH) &&
		       गन्तव्यम्_copy > 0 && is_dir_sep(pathspec[i][गन्तव्यम्_copy - 1]))
			गन्तव्यम्_copy--;

		trimmed = xmemdupz(pathspec[i], गन्तव्यम्_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_मार्गः = उपसर्गः_मार्गः(the_repository, उपसर्गः, prefixlen, maybe_basename);
		strvec_प्रक्षिप(बहिर्गमनम्, prefixed_मार्गः);

		मुक्त(prefixed_मार्गः);
		मुक्त(trimmed);
	}
}

स्थिर वर्ण *सङ्कलय_slash(ध्रुव वर्ण *मार्गः)
{
	आकार_t len = वाक्यदैर्घ्यम्(मार्गः);
	यदि (len && मार्गः[len - 1] != '/') {
		वर्ण *with_slash = xmalloc(st_सङ्कलय(len, 2));
		स्मृतिं_प्रतिलिख(with_slash, मार्गः, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		प्रत्यागम with_slash;
	}
	प्रत्यागम xstrdup(मार्गः);
}

#परिभाषय SUBMODULE_WITH_GITDIR ((ध्रुव वर्ण *)1)

स्थिर ध्रुव वर्ण *submodule_gitfile_मार्गः(ध्रुव वर्ण *src, पूर्णाङ्क first)
{
	संरचना strbuf submodule_dotgit = STRBUF_INIT;
	ध्रुव वर्ण *मार्गः;

	यदि (!S_ISGITLINK(the_repository->सूचकांक->cache[first]->ce_बहुलकम्))
		die(_("Directory %s is in index and no submodule?"), src);
	यदि (!is_staging_gitmodules_ok(the_repository->सूचकांक))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	मार्गः = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	यदि (मार्गः)
		प्रत्यागम मार्गः;
	प्रत्यागम SUBMODULE_WITH_GITDIR;
}

स्थिर पूर्णाङ्क सूचकांक_range_सम्बन्धि_same_dir(ध्रुव वर्ण *src, पूर्णाङ्क दीर्घता,
				   पूर्णाङ्क *first_p, पूर्णाङ्क *चरमः_p)
{
	वर्ण *src_w_slash = सङ्कलय_slash(src);
	पूर्णाङ्क first, चरमः, len_w_slash = दीर्घता + 1;

	first = सूचकांक_name_pos(the_repository->सूचकांक, src_w_slash, len_w_slash);
	यदि (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	प्रति (चरमः = first; चरमः < the_repository->सूचकांक->cache_nr; चरमः++) {
		ध्रुव वर्ण *मार्गः = the_repository->सूचकांक->cache[चरमः]->name;
		यदि (वाक्ये_तुलय(मार्गः, src_w_slash, len_w_slash))
			भङ्ग;
	}

	मुक्त(src_w_slash);
	*first_p = first;
	*चरमः_p = चरमः;
	प्रत्यागम चरमः - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
स्थिर पूर्णाङ्क रिक्तानि_dir_has_sparse_contents(ध्रुव वर्ण *name)
{
	पूर्णाङ्क ret = 0;
	वर्ण *with_slash = सङ्कलय_slash(name);
	पूर्णाङ्क दीर्घता = वाक्यदैर्घ्यम्(with_slash);

	पूर्णाङ्क pos = सूचकांक_name_pos(the_repository->सूचकांक, with_slash, दीर्घता);
	ध्रुव संरचना cache_प्रविष्टिः *ce;

	यदि (pos < 0) {
		pos = -pos - 1;
		यदि (pos >= the_repository->सूचकांक->cache_nr)
			गच्छ free_return;
		ce = the_repository->सूचकांक->cache[pos];
		यदि (वाक्ये_तुलय(with_slash, ce->name, दीर्घता))
			गच्छ free_return;
		यदि (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	मुक्त(with_slash);
	प्रत्यागम ret;
}

स्थिर शून्य remove_रिक्तानि_src_dirs(ध्रुव वर्ण **src_dir, आकार_t src_dir_nr)
{
	आकार_t i;
	संरचना strbuf a_src_dir = STRBUF_INIT;

	प्रति (i = 0; i < src_dir_nr; i++) {
		पूर्णाङ्क dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		यदि (सूचकांक_range_सम्बन्धि_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

संरचना pathmap_प्रविष्टिः {
	संरचना hashmap_प्रविष्टिः ent;
	ध्रुव वर्ण *मार्गः;
};

स्थिर पूर्णाङ्क pathmap_cmp(ध्रुव शून्य *cmp_दत्त अप्रयुक्तम्,
		       ध्रुव संरचना hashmap_प्रविष्टिः *a,
		       ध्रुव संरचना hashmap_प्रविष्टिः *b,
		       ध्रुव शून्य *कुञ्चिका अप्रयुक्तम्)
{
	ध्रुव संरचना pathmap_प्रविष्टिः *e1 = container_सम्बन्धि(a, संरचना pathmap_प्रविष्टिः, ent);
	ध्रुव संरचना pathmap_प्रविष्टिः *e2 = container_सम्बन्धि(b, संरचना pathmap_प्रविष्टिः, ent);
	प्रत्यागम fspathcmp(e1->मार्गः, e2->मार्गः);
}

पूर्णाङ्क cmd_mv(पूर्णाङ्क argc,
	   ध्रुव वर्ण **argv,
	   ध्रुव वर्ण *उपसर्गः,
	   संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क i, flags, gitmodules_modified = 0;
	पूर्णाङ्क verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	संरचना option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_DRY_सततम्(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', रिक्त, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_अन्तः(),
	};
	संरचना strvec sources = STRVEC_INIT;
	संरचना strvec dest_paths = STRVEC_INIT;
	संरचना strvec destinations = STRVEC_INIT;
	संरचना strvec submodule_gitfiles_गन्तव्यम्_free = STRVEC_INIT;
	ध्रुव वर्ण **submodule_gitfiles;
	वर्ण *dst_w_slash = रिक्त;
	संरचना strvec src_dir = STRVEC_INIT;
	गणना update_बहुलकम् *modes, dst_बहुलकम् = 0;
	संरचना stat st, dest_st;
	संरचना string_list src_for_dst = STRING_LIST_INIT_DUP;
	संरचना तालकम्_file तालकम्_file = तालकम्_INIT;
	संरचना cache_प्रविष्टिः *ce;
	संरचना string_list only_मेलनम्_skip_worktree = STRING_LIST_INIT_DUP;
	संरचना string_list dirty_paths = STRING_LIST_INIT_DUP;
	संरचना hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, रिक्त);
	संरचना strbuf pathbuf = STRBUF_INIT;
	पूर्णाङ्क ret;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, उपसर्गः, builtin_mv_options,
			     builtin_mv_usage, 0);
	यदि (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_सूचकांक(the_repository, &तालकम्_file, तालकम्_DIE_ON_ERROR);
	यदि (repo_read_सूचकांक(the_repository) < 0)
		die(_("index file corrupt"));

	आन्तरिकम्_उपसर्गः_pathspec(&sources, उपसर्गः, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	यदि (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	आन्तरिकम्_उपसर्गः_pathspec(&dest_paths, उपसर्गः, argv + argc, 1, flags);
	dst_w_slash = सङ्कलय_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, परिमाण(वर्ण *));

	यदि (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		आन्तरिकम्_उपसर्गः_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	अन्यथा यदि (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_बहुलकम्)) {
		आन्तरिकम्_उपसर्गः_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} अन्यथा यदि (!मार्गः_अन्तर्_sparse_checkout(dst_w_slash, the_repository->सूचकांक) &&
		   रिक्तानि_dir_has_sparse_contents(dst_w_slash)) {
		आन्तरिकम्_उपसर्गः_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_बहुलकम् = SKIP_WORKTREE_DIR;
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
		यदि (!मार्गः_अन्तर्_cone_बहुलकम्_sparse_checkout(destinations.v[0], the_repository->सूचकांक))
			dst_बहुलकम् = SPARSE;
	}

	/* Checking */
	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *src = sources.v[i], *dst = destinations.v[i];
		पूर्णाङ्क दीर्घता;
		ध्रुव वर्ण *bad = रिक्त;
		पूर्णाङ्क skip_sparse = 0;

		यदि (show_only)
			प्रारूप_लिख(_("Checking rename of '%s' to '%s'\n"), src, dst);

		दीर्घता = वाक्यदैर्घ्यम्(src);
		यदि (lstat(src, &st) < 0) {
			पूर्णाङ्क pos;
			ध्रुव संरचना cache_प्रविष्टिः *ce;

			pos = सूचकांक_name_pos(the_repository->सूचकांक, src, दीर्घता);
			यदि (pos < 0) {
				वर्ण *src_w_slash = सङ्कलय_slash(src);
				यदि (!मार्गः_अन्तर्_sparse_checkout(src_w_slash, the_repository->सूचकांक) &&
				    रिक्तानि_dir_has_sparse_contents(src)) {
					मुक्त(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					गच्छ dir_check;
				}
				मुक्त(src_w_slash);
				/* only error if existence is expected. */
				यदि (!(modes[i] & SPARSE))
					bad = _("bad source");
				गच्छ act_on_प्रविष्टिः;
			}
			ce = the_repository->सूचकांक->cache[pos];
			यदि (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				गच्छ act_on_प्रविष्टिः;
			}
			यदि (!ignore_sparse) {
				string_list_append(&only_मेलनम्_skip_worktree, src);
				गच्छ act_on_प्रविष्टिः;
			}
			/* Check if dst exists in index */
			यदि (सूचकांक_name_pos(the_repository->सूचकांक, dst, वाक्यदैर्घ्यम्(dst)) < 0) {
				modes[i] |= SPARSE;
				गच्छ act_on_प्रविष्टिः;
			}
			यदि (!force) {
				bad = _("destination exists");
				गच्छ act_on_प्रविष्टिः;
			}
			modes[i] |= SPARSE;
			गच्छ act_on_प्रविष्टिः;
		}
		यदि (!वाक्ये_तुलय(src, dst, दीर्घता) &&
		    (dst[दीर्घता] == 0 || dst[दीर्घता] == '/')) {
			bad = _("can not move directory into itself");
			गच्छ act_on_प्रविष्टिः;
		}
		यदि (S_ISDIR(st.st_बहुलकम्)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			गच्छ act_on_प्रविष्टिः;
		}

dir_check:
		यदि (S_ISDIR(st.st_बहुलकम्)) {
			संरचना pathmap_प्रविष्टिः *प्रविष्टिः;
			वर्ण *dst_with_slash;
			आकार_t dst_with_slash_len;
			पूर्णाङ्क j, n;
			पूर्णाङ्क first = सूचकांक_name_pos(the_repository->सूचकांक, src, दीर्घता), चरमः;

			यदि (first >= 0) {
				ध्रुव वर्ण *मार्गः = submodule_gitfile_मार्गः(src, first);
				यदि (मार्गः != SUBMODULE_WITH_GITDIR)
					मार्गः = strvec_प्रक्षिप(&submodule_gitfiles_गन्तव्यम्_free, मार्गः);
				submodule_gitfiles[i] = मार्गः;
				गच्छ act_on_प्रविष्टिः;
			} अन्यथा यदि (सूचकांक_range_सम्बन्धि_same_dir(src, दीर्घता,
							   &first, &चरमः) < 1) {
				bad = _("source directory is empty");
				गच्छ act_on_प्रविष्टिः;
			}

			प्रविष्टिः = xmalloc(परिमाण(*प्रविष्टिः));
			प्रविष्टिः->मार्गः = src;
			hashmap_प्रविष्टिः_init(&प्रविष्टिः->ent, fspathhash(src));
			hashmap_सङ्कलय(&moved_dirs, &प्रविष्टिः->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_प्रक्षिप(&src_dir, src);

			n = argc + चरमः - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = सङ्कलय_slash(dst);
			dst_with_slash_len = वाक्यदैर्घ्यम्(dst_with_slash);

			प्रति (j = 0; j < चरमः - first; j++) {
				ध्रुव संरचना cache_प्रविष्टिः *ce = the_repository->सूचकांक->cache[first + j];
				ध्रुव वर्ण *मार्गः = ce->name;
				वर्ण *prefixed_मार्गः = उपसर्गः_मार्गः(the_repository, dst_with_slash,
								  dst_with_slash_len, मार्गः + दीर्घता + 1);

				strvec_प्रक्षिप(&sources, मार्गः);
				strvec_प्रक्षिप(&destinations, prefixed_मार्गः);

				modes[argc + j] = MOVE_द्वारा_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : सूचकांक_2);
				submodule_gitfiles[argc + j] = रिक्त;

				मुक्त(prefixed_मार्गः);
			}

			मुक्त(dst_with_slash);
			argc += चरमः - first;
			गच्छ act_on_प्रविष्टिः;
		}
		यदि (!(ce = सूचकांक_file_exists(the_repository->सूचकांक, src, दीर्घता, 0))) {
			bad = _("not under version control");
			गच्छ act_on_प्रविष्टिः;
		}
		यदि (ce_stage(ce)) {
			bad = _("conflicted");
			गच्छ act_on_प्रविष्टिः;
		}
		यदि (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			यदि (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				यदि (S_ISREG(st.st_बहुलकम्) || S_ISLNK(st.st_बहुलकम्)) {
					यदि (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = रिक्त;
				} अन्यथा
					bad = _("Cannot overwrite");
			}
			गच्छ act_on_प्रविष्टिः;
		}
		यदि (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			गच्छ act_on_प्रविष्टिः;
		}
		यदि (is_dir_sep(dst[वाक्यदैर्घ्यम्(dst) - 1])) {
			bad = _("destination directory does not exist");
			गच्छ act_on_प्रविष्टिः;
		}
		यदि (has_symlink_leading_मार्गः(dst, वाक्यदैर्घ्यम्(dst))) {
			bad = _("destination is beyond a symbolic link");
			गच्छ act_on_प्रविष्टिः;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		यदि (needs_worktree_rename(modes[i], dst_बहुलकम्)) {
			ध्रुव वर्ण *slash_ = strrchr(dst, '/');

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
					यदि (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} अन्यथा यदि (!S_ISDIR(dir_st.st_बहुलकम्)) {
					bad = _("destination is not a directory");
				}

				मुक्त(dst_dir);
			}

			यदि (bad)
				गच्छ act_on_प्रविष्टिः;
		}

		यदि (ignore_sparse &&
		    (dst_बहुलकम् & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    सूचकांक_प्रविष्टिः_exists(the_repository->सूचकांक, dst, वाक्यदैर्घ्यम्(dst))) {
			bad = _("destination exists in the index");
			यदि (force) {
				यदि (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = रिक्त;
			} अन्यथा {
				गच्छ act_on_प्रविष्टिः;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		यदि (!ignore_sparse &&
		    !मार्गः_अन्तर्_sparse_checkout(src, the_repository->सूचकांक)) {
			string_list_append(&only_मेलनम्_skip_worktree, src);
			skip_sparse = 1;
		}
		यदि (!ignore_sparse &&
		    !मार्गः_अन्तर्_sparse_checkout(dst, the_repository->सूचकांक)) {
			string_list_append(&only_मेलनम्_skip_worktree, dst);
			skip_sparse = 1;
		}

		यदि (skip_sparse)
			गच्छ remove_प्रविष्टिः;

		string_list_निवेशय(&src_for_dst, dst);

act_on_प्रविष्टिः:
		यदि (!bad)
			अनुवर्त;
		यदि (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_प्रविष्टिः:
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

	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *slash_pos;

		यदि (modes[i] & MOVE_द्वारा_PARENT_DIR)
			अनुवर्त;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		यावत् (slash_pos > pathbuf.buf) {
			संरचना pathmap_प्रविष्टिः needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.मार्गः = pathbuf.buf;
			hashmap_प्रविष्टिः_init(&needle.ent, fspathhash(pathbuf.buf));

			यदि (hashmap_get_प्रविष्टिः(&moved_dirs, &needle, ent, रिक्त))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	यदि (only_मेलनम्_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_मेलनम्_skip_worktree);
		यदि (!ignore_errors) {
			ret = 1;
			गच्छ बहिर्गमनम्;
		}
	}

	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *src = sources.v[i], *dst = destinations.v[i];
		गणना update_बहुलकम् बहुलकम् = modes[i];
		पूर्णाङ्क pos;
		पूर्णाङ्क sparse_and_dirty = 0;
		संरचना checkout state = CHECKOUT_INIT;
		state.istate = the_repository->सूचकांक;

		यदि (force)
			state.force = 1;
		यदि (show_only || verbose)
			प्रारूप_लिख(_("Renaming %s to %s\n"), src, dst);
		यदि (show_only)
			अनुवर्त;
		यदि (needs_worktree_rename(बहुलकम्, dst_बहुलकम्) &&
		    rename(src, dst) < 0) {
			यदि (ignore_errors)
				अनुवर्त;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		यदि (submodule_gitfiles[i]) {
			यदि (!update_मार्गः_अन्तर्_gitmodules(src, dst))
				gitmodules_modified = 1;
			यदि (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_वृक्षः_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		यदि (बहुलकम् & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			अनुवर्त;

		pos = सूचकांक_name_pos(the_repository->सूचकांक, src, वाक्यदैर्घ्यम्(src));
		यदि (pos < 0)
			BUG("could not find source in index: '%s'", src);
		यदि (!(बहुलकम् & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->सूचकांक,
						       the_repository->सूचकांक->cache[pos],
						       &st,
						       0);
		rename_सूचकांक_प्रविष्टिः_at(the_repository->सूचकांक, pos, dst);

		यदि (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			यदि ((बहुलकम् & SPARSE) &&
			    मार्गः_अन्तर्_sparse_checkout(dst, the_repository->सूचकांक)) {
				/* from out-of-cone to in-cone */
				पूर्णाङ्क dst_pos = सूचकांक_name_pos(the_repository->सूचकांक, dst,
							     वाक्यदैर्घ्यम्(dst));
				संरचना cache_प्रविष्टिः *dst_ce = the_repository->सूचकांक->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				यदि (checkout_प्रविष्टिः(dst_ce, &state, रिक्त, रिक्त))
					die(_("cannot checkout %s"), dst_ce->name);
			} अन्यथा यदि ((dst_बहुलकम् & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(बहुलकम् & SPARSE) &&
				   !मार्गः_अन्तर्_sparse_checkout(dst, the_repository->सूचकांक)) {
				/* from in-cone to out-of-cone */
				पूर्णाङ्क dst_pos = सूचकांक_name_pos(the_repository->सूचकांक, dst,
							     वाक्यदैर्घ्यम्(dst));
				संरचना cache_प्रविष्टिः *dst_ce = the_repository->सूचकांक->cache[dst_pos];

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
					rename(src, dst);
				}
			}
		}
	}

	remove_रिक्तानि_src_dirs(src_dir.v, src_dir.nr);

	यदि (dirty_paths.nr)
		advise_on_moving_dirty_मार्गः(&dirty_paths);

	यदि (gitmodules_modified)
		stage_updated_gitmodules(the_repository->सूचकांक);

	यदि (write_locked_सूचकांक(the_repository->सूचकांक, &तालकम्_file,
			       COMMIT_तालकम् | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

बहिर्गमनम्:
	strvec_clear(&src_dir);
	मुक्त(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_मेलनम्_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_गन्तव्यम्_free);
	hashmap_clear_and_free(&moved_dirs, संरचना pathmap_प्रविष्टिः, ent);
	strbuf_release(&pathbuf);
	मुक्त(submodule_gitfiles);
	मुक्त(modes);
	प्रत्यागम ret;
}
