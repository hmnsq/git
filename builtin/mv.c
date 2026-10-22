/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "name-hash.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "pathspec.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "symlinks.h"

#అన్తర్భావ "setup.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "entry.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	ఖాళీ
};

గణన update_బహుళకం {
	WORKING_DIRECTORY = (1 << 1),
	సూచీ_2 = (1 << 2),
	SPARSE = (1 << 3),
	SKIP_WORKTREE_DIR = (1 << 4),
	/*
	 * A file gets moved implicitly via a move of one of its parent
	 * directories. This flag causes us to skip the check that we don't try
	 * to move a file and any of its parent directories at the same point
	 * in time.
	 */
	MOVE_ద్వారా_మాతృ_DIR = (1 << 5),
};

స్థిర పూర్ణాంకం needs_worktree_rename(గణన update_బహుళకం బహుళకం, గణన update_బహుళకం dst_బహుళకం)
{
	తిరిగి !(బహుళకం & (సూచీ_2 | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_బహుళకం & (SKIP_WORKTREE_DIR | SPARSE));
}

#నిర్వచించు DUP_BASENAME 1
#నిర్వచించు KEEP_TRAILING_SLASH 2

స్థిర శూన్య అంతర్గత_ఉపసర్గ_pathspec(నిర్మాణం strvec *బహిర్గతం,
				     స్థిరరాశి అక్షరం *ఉపసర్గ,
				     స్థిరరాశి అక్షరం **pathspec,
				     పూర్ణాంకం లెక్క, గుర్తులేని flags)
{
	పూర్ణాంకం prefixlen = ఉపసర్గ ? అక్షరశ్రేణి_పొడవు(ఉపసర్గ) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	కొరకు (పూర్ణాంకం i = 0; i < లెక్క; i++) {
		కొలత_t నిడివి = అక్షరశ్రేణి_పొడవు(pathspec[i]);
		కొలత_t గమ్యం_copy = నిడివి;
		స్థిరరాశి అక్షరం *maybe_basename;
		అక్షరం *trimmed, *prefixed_మార్గం;

		అయితే (!(flags & KEEP_TRAILING_SLASH) &&
		       గమ్యం_copy > 0 && is_dir_sep(pathspec[i][గమ్యం_copy - 1]))
			గమ్యం_copy--;

		trimmed = xmemdupz(pathspec[i], గమ్యం_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_మార్గం = ఉపసర్గ_మార్గం(the_repository, ఉపసర్గ, prefixlen, maybe_basename);
		strvec_నెట్టు(బహిర్గతం, prefixed_మార్గం);

		విడుదల(prefixed_మార్గం);
		విడుదల(trimmed);
	}
}

స్థిర అక్షరం *కూడు_slash(స్థిరరాశి అక్షరం *మార్గం)
{
	కొలత_t len = అక్షరశ్రేణి_పొడవు(మార్గం);
	ఒకవేళ (len && మార్గం[len - 1] != '/') {
		అక్షరం *with_slash = xmalloc(st_కూడు(len, 2));
		జ్ఞాపకం_నకలు(with_slash, మార్గం, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		తిరిగి with_slash;
	}
	తిరిగి xstrdup(మార్గం);
}

#నిర్వచించు SUBMODULE_WITH_GITDIR ((స్థిరరాశి అక్షరం *)1)

స్థిర స్థిరరాశి అక్షరం *submodule_gitfile_మార్గం(స్థిరరాశి అక్షరం *src, పూర్ణాంకం మొదటి)
{
	నిర్మాణం strbuf submodule_dotgit = STRBUF_INIT;
	స్థిరరాశి అక్షరం *మార్గం;

	ఒకవేళ (!S_ISGITLINK(the_repository->సూచీ->cache[మొదటి]->ce_బహుళకం))
		die(_("Directory %s is in index and no submodule?"), src);
	ఒకవేళ (!is_staging_gitmodules_ok(the_repository->సూచీ))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	మార్గం = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	ఒకవేళ (మార్గం)
		తిరిగి మార్గం;
	తిరిగి SUBMODULE_WITH_GITDIR;
}

స్థిర పూర్ణాంకం సూచీ_range_యొక్క_same_dir(స్థిరరాశి అక్షరం *src, పూర్ణాంకం నిడివి,
				   పూర్ణాంకం *మొదటి_p, పూర్ణాంకం *చివరి_p)
{
	అక్షరం *src_w_slash = కూడు_slash(src);
	పూర్ణాంకం మొదటి, చివరి, len_w_slash = నిడివి + 1;

	మొదటి = సూచీ_name_pos(the_repository->సూచీ, src_w_slash, len_w_slash);
	ఒకవేళ (మొదటి >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	మొదటి = -1 - మొదటి;
	కొరకు (చివరి = మొదటి; చివరి < the_repository->సూచీ->cache_nr; చివరి++) {
		స్థిరరాశి అక్షరం *మార్గం = the_repository->సూచీ->cache[చివరి]->name;
		ఒకవేళ (అక్షరశ్రేణి_పోలిక(మార్గం, src_w_slash, len_w_slash))
			ఆపు;
	}

	విడుదల(src_w_slash);
	*మొదటి_p = మొదటి;
	*చివరి_p = చివరి;
	తిరిగి చివరి - మొదటి;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
స్థిర పూర్ణాంకం ఖాళీలు_dir_has_sparse_contents(స్థిరరాశి అక్షరం *name)
{
	పూర్ణాంకం ret = 0;
	అక్షరం *with_slash = కూడు_slash(name);
	పూర్ణాంకం నిడివి = అక్షరశ్రేణి_పొడవు(with_slash);

	పూర్ణాంకం pos = సూచీ_name_pos(the_repository->సూచీ, with_slash, నిడివి);
	స్థిరరాశి నిర్మాణం cache_నమోదు *ce;

	ఒకవేళ (pos < 0) {
		pos = -pos - 1;
		ఒకవేళ (pos >= the_repository->సూచీ->cache_nr)
			వెళ్ళు free_return;
		ce = the_repository->సూచీ->cache[pos];
		ఒకవేళ (అక్షరశ్రేణి_పోలిక(with_slash, ce->name, నిడివి))
			వెళ్ళు free_return;
		ఒకవేళ (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	విడుదల(with_slash);
	తిరిగి ret;
}

స్థిర శూన్య remove_ఖాళీలు_src_dirs(స్థిరరాశి అక్షరం **src_dir, కొలత_t src_dir_nr)
{
	కొలత_t i;
	నిర్మాణం strbuf a_src_dir = STRBUF_INIT;

	కొరకు (i = 0; i < src_dir_nr; i++) {
		పూర్ణాంకం dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		ఒకవేళ (సూచీ_range_యొక్క_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

నిర్మాణం pathmap_నమోదు {
	నిర్మాణం hashmap_నమోదు ent;
	స్థిరరాశి అక్షరం *మార్గం;
};

స్థిర పూర్ణాంకం pathmap_cmp(స్థిరరాశి శూన్య *cmp_దత్తాంశం వాడని,
		       స్థిరరాశి నిర్మాణం hashmap_నమోదు *a,
		       స్థిరరాశి నిర్మాణం hashmap_నమోదు *b,
		       స్థిరరాశి శూన్య *కీలకం వాడని)
{
	స్థిరరాశి నిర్మాణం pathmap_నమోదు *e1 = container_యొక్క(a, నిర్మాణం pathmap_నమోదు, ent);
	స్థిరరాశి నిర్మాణం pathmap_నమోదు *e2 = container_యొక్క(b, నిర్మాణం pathmap_నమోదు, ent);
	తిరిగి fspathcmp(e1->మార్గం, e2->మార్గం);
}

పూర్ణాంకం cmd_mv(పూర్ణాంకం argc,
	   స్థిరరాశి అక్షరం **argv,
	   స్థిరరాశి అక్షరం *ఉపసర్గ,
	   నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i, flags, gitmodules_modified = 0;
	పూర్ణాంకం verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	నిర్మాణం option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_DRY_నిరంతరం(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', ఖాళీ, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_చివర(),
	};
	నిర్మాణం strvec sources = STRVEC_INIT;
	నిర్మాణం strvec dest_paths = STRVEC_INIT;
	నిర్మాణం strvec destinations = STRVEC_INIT;
	నిర్మాణం strvec submodule_gitfiles_గమ్యం_free = STRVEC_INIT;
	స్థిరరాశి అక్షరం **submodule_gitfiles;
	అక్షరం *dst_w_slash = ఖాళీ;
	నిర్మాణం strvec src_dir = STRVEC_INIT;
	గణన update_బహుళకం *modes, dst_బహుళకం = 0;
	నిర్మాణం stat st, dest_st;
	నిర్మాణం string_list src_for_dst = STRING_LIST_INIT_DUP;
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	నిర్మాణం cache_నమోదు *ce;
	నిర్మాణం string_list only_సరిపోలిక_skip_worktree = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list dirty_paths = STRING_LIST_INIT_DUP;
	నిర్మాణం hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, ఖాళీ);
	నిర్మాణం strbuf pathbuf = STRBUF_INIT;
	పూర్ణాంకం ret;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	repo_config(the_repository, git_default_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_mv_options,
			     builtin_mv_usage, 0);
	ఒకవేళ (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_సూచీ(the_repository, &తాళం_file, తాళం_DIE_ON_ERROR);
	ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
		die(_("index file corrupt"));

	అంతర్గత_ఉపసర్గ_pathspec(&sources, ఉపసర్గ, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	ఒకవేళ (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	అంతర్గత_ఉపసర్గ_pathspec(&dest_paths, ఉపసర్గ, argv + argc, 1, flags);
	dst_w_slash = కూడు_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, పరిమాణం(అక్షరం *));

	ఒకవేళ (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		అంతర్గత_ఉపసర్గ_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	లేకపోతే ఒకవేళ (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_బహుళకం)) {
		అంతర్గత_ఉపసర్గ_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} లేకపోతే ఒకవేళ (!మార్గం_లోపల_sparse_checkout(dst_w_slash, the_repository->సూచీ) &&
		   ఖాళీలు_dir_has_sparse_contents(dst_w_slash)) {
		అంతర్గత_ఉపసర్గ_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_బహుళకం = SKIP_WORKTREE_DIR;
	} లేకపోతే ఒకవేళ (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} లేకపోతే {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		ఒకవేళ (!మార్గం_లోపల_cone_బహుళకం_sparse_checkout(destinations.v[0], the_repository->సూచీ))
			dst_బహుళకం = SPARSE;
	}

	/* Checking */
	కొరకు (i = 0; i < argc; i++) {
		స్థిరరాశి అక్షరం *src = sources.v[i], *dst = destinations.v[i];
		పూర్ణాంకం నిడివి;
		స్థిరరాశి అక్షరం *bad = ఖాళీ;
		పూర్ణాంకం skip_sparse = 0;

		ఒకవేళ (show_only)
			రాయి(_("Checking rename of '%s' to '%s'\n"), src, dst);

		నిడివి = అక్షరశ్రేణి_పొడవు(src);
		ఒకవేళ (lstat(src, &st) < 0) {
			పూర్ణాంకం pos;
			స్థిరరాశి నిర్మాణం cache_నమోదు *ce;

			pos = సూచీ_name_pos(the_repository->సూచీ, src, నిడివి);
			ఒకవేళ (pos < 0) {
				అక్షరం *src_w_slash = కూడు_slash(src);
				ఒకవేళ (!మార్గం_లోపల_sparse_checkout(src_w_slash, the_repository->సూచీ) &&
				    ఖాళీలు_dir_has_sparse_contents(src)) {
					విడుదల(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					వెళ్ళు dir_check;
				}
				విడుదల(src_w_slash);
				/* only error if existence is expected. */
				ఒకవేళ (!(modes[i] & SPARSE))
					bad = _("bad source");
				వెళ్ళు act_on_నమోదు;
			}
			ce = the_repository->సూచీ->cache[pos];
			ఒకవేళ (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				వెళ్ళు act_on_నమోదు;
			}
			ఒకవేళ (!ignore_sparse) {
				string_list_append(&only_సరిపోలిక_skip_worktree, src);
				వెళ్ళు act_on_నమోదు;
			}
			/* Check if dst exists in index */
			ఒకవేళ (సూచీ_name_pos(the_repository->సూచీ, dst, అక్షరశ్రేణి_పొడవు(dst)) < 0) {
				modes[i] |= SPARSE;
				వెళ్ళు act_on_నమోదు;
			}
			ఒకవేళ (!force) {
				bad = _("destination exists");
				వెళ్ళు act_on_నమోదు;
			}
			modes[i] |= SPARSE;
			వెళ్ళు act_on_నమోదు;
		}
		ఒకవేళ (!అక్షరశ్రేణి_పోలిక(src, dst, నిడివి) &&
		    (dst[నిడివి] == 0 || dst[నిడివి] == '/')) {
			bad = _("can not move directory into itself");
			వెళ్ళు act_on_నమోదు;
		}
		ఒకవేళ (S_ISDIR(st.st_బహుళకం)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			వెళ్ళు act_on_నమోదు;
		}

dir_check:
		ఒకవేళ (S_ISDIR(st.st_బహుళకం)) {
			నిర్మాణం pathmap_నమోదు *నమోదు_2;
			అక్షరం *dst_with_slash;
			కొలత_t dst_with_slash_len;
			పూర్ణాంకం j, n;
			పూర్ణాంకం మొదటి = సూచీ_name_pos(the_repository->సూచీ, src, నిడివి), చివరి;

			ఒకవేళ (మొదటి >= 0) {
				స్థిరరాశి అక్షరం *మార్గం = submodule_gitfile_మార్గం(src, మొదటి);
				ఒకవేళ (మార్గం != SUBMODULE_WITH_GITDIR)
					మార్గం = strvec_నెట్టు(&submodule_gitfiles_గమ్యం_free, మార్గం);
				submodule_gitfiles[i] = మార్గం;
				వెళ్ళు act_on_నమోదు;
			} లేకపోతే ఒకవేళ (సూచీ_range_యొక్క_same_dir(src, నిడివి,
							   &మొదటి, &చివరి) < 1) {
				bad = _("source directory is empty");
				వెళ్ళు act_on_నమోదు;
			}

			నమోదు_2 = xmalloc(పరిమాణం(*నమోదు_2));
			నమోదు_2->మార్గం = src;
			hashmap_నమోదు_init(&నమోదు_2->ent, fspathhash(src));
			hashmap_కూడు(&moved_dirs, &నమోదు_2->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_నెట్టు(&src_dir, src);

			n = argc + చివరి - మొదటి;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = కూడు_slash(dst);
			dst_with_slash_len = అక్షరశ్రేణి_పొడవు(dst_with_slash);

			కొరకు (j = 0; j < చివరి - మొదటి; j++) {
				స్థిరరాశి నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[మొదటి + j];
				స్థిరరాశి అక్షరం *మార్గం = ce->name;
				అక్షరం *prefixed_మార్గం = ఉపసర్గ_మార్గం(the_repository, dst_with_slash,
								  dst_with_slash_len, మార్గం + నిడివి + 1);

				strvec_నెట్టు(&sources, మార్గం);
				strvec_నెట్టు(&destinations, prefixed_మార్గం);

				modes[argc + j] = MOVE_ద్వారా_మాతృ_DIR | (ce_skip_worktree(ce) ? SPARSE : సూచీ_2);
				submodule_gitfiles[argc + j] = ఖాళీ;

				విడుదల(prefixed_మార్గం);
			}

			విడుదల(dst_with_slash);
			argc += చివరి - మొదటి;
			వెళ్ళు act_on_నమోదు;
		}
		ఒకవేళ (!(ce = సూచీ_file_exists(the_repository->సూచీ, src, నిడివి, 0))) {
			bad = _("not under version control");
			వెళ్ళు act_on_నమోదు;
		}
		ఒకవేళ (ce_stage(ce)) {
			bad = _("conflicted");
			వెళ్ళు act_on_నమోదు;
		}
		ఒకవేళ (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			ఒకవేళ (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				ఒకవేళ (S_ISREG(st.st_బహుళకం) || S_ISLNK(st.st_బహుళకం)) {
					ఒకవేళ (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = ఖాళీ;
				} లేకపోతే
					bad = _("Cannot overwrite");
			}
			వెళ్ళు act_on_నమోదు;
		}
		ఒకవేళ (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			వెళ్ళు act_on_నమోదు;
		}
		ఒకవేళ (is_dir_sep(dst[అక్షరశ్రేణి_పొడవు(dst) - 1])) {
			bad = _("destination directory does not exist");
			వెళ్ళు act_on_నమోదు;
		}
		ఒకవేళ (has_symlink_leading_మార్గం(dst, అక్షరశ్రేణి_పొడవు(dst))) {
			bad = _("destination is beyond a symbolic link");
			వెళ్ళు act_on_నమోదు;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		ఒకవేళ (needs_worktree_rename(modes[i], dst_బహుళకం)) {
			స్థిరరాశి అక్షరం *slash_ = strrchr(dst, '/');

			ఒకవేళ (slash_) {
				నిర్మాణం stat dir_st;
				అక్షరం *dst_dir = xstrdup(dst);
				అక్షరం *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				ఒకవేళ (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					ఒకవేళ (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} లేకపోతే ఒకవేళ (!S_ISDIR(dir_st.st_బహుళకం)) {
					bad = _("destination is not a directory");
				}

				విడుదల(dst_dir);
			}

			ఒకవేళ (bad)
				వెళ్ళు act_on_నమోదు;
		}

		ఒకవేళ (ignore_sparse &&
		    (dst_బహుళకం & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    సూచీ_నమోదు_exists(the_repository->సూచీ, dst, అక్షరశ్రేణి_పొడవు(dst))) {
			bad = _("destination exists in the index");
			ఒకవేళ (force) {
				ఒకవేళ (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = ఖాళీ;
			} లేకపోతే {
				వెళ్ళు act_on_నమోదు;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		ఒకవేళ (!ignore_sparse &&
		    !మార్గం_లోపల_sparse_checkout(src, the_repository->సూచీ)) {
			string_list_append(&only_సరిపోలిక_skip_worktree, src);
			skip_sparse = 1;
		}
		ఒకవేళ (!ignore_sparse &&
		    !మార్గం_లోపల_sparse_checkout(dst, the_repository->సూచీ)) {
			string_list_append(&only_సరిపోలిక_skip_worktree, dst);
			skip_sparse = 1;
		}

		ఒకవేళ (skip_sparse)
			వెళ్ళు remove_నమోదు;

		string_list_చొప్పించు(&src_for_dst, dst);

act_on_నమోదు:
		ఒకవేళ (!bad)
			కొనసాగు;
		ఒకవేళ (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_నమోదు:
		ఒకవేళ (--argc > 0) {
			పూర్ణాంకం n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	కొరకు (i = 0; i < argc; i++) {
		స్థిరరాశి అక్షరం *slash_pos;

		ఒకవేళ (modes[i] & MOVE_ద్వారా_మాతృ_DIR)
			కొనసాగు;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		అయితే (slash_pos > pathbuf.buf) {
			నిర్మాణం pathmap_నమోదు needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.మార్గం = pathbuf.buf;
			hashmap_నమోదు_init(&needle.ent, fspathhash(pathbuf.buf));

			ఒకవేళ (hashmap_get_నమోదు(&moved_dirs, &needle, ent, ఖాళీ))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	ఒకవేళ (only_సరిపోలిక_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_సరిపోలిక_skip_worktree);
		ఒకవేళ (!ignore_errors) {
			ret = 1;
			వెళ్ళు బహిర్గతం;
		}
	}

	కొరకు (i = 0; i < argc; i++) {
		స్థిరరాశి అక్షరం *src = sources.v[i], *dst = destinations.v[i];
		గణన update_బహుళకం బహుళకం = modes[i];
		పూర్ణాంకం pos;
		పూర్ణాంకం sparse_and_dirty = 0;
		నిర్మాణం checkout state = CHECKOUT_INIT;
		state.istate = the_repository->సూచీ;

		ఒకవేళ (force)
			state.force = 1;
		ఒకవేళ (show_only || verbose)
			రాయి(_("Renaming %s to %s\n"), src, dst);
		ఒకవేళ (show_only)
			కొనసాగు;
		ఒకవేళ (needs_worktree_rename(బహుళకం, dst_బహుళకం) &&
		    rename(src, dst) < 0) {
			ఒకవేళ (ignore_errors)
				కొనసాగు;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		ఒకవేళ (submodule_gitfiles[i]) {
			ఒకవేళ (!update_మార్గం_లోపల_gitmodules(src, dst))
				gitmodules_modified = 1;
			ఒకవేళ (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_వృక్షం_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		ఒకవేళ (బహుళకం & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			కొనసాగు;

		pos = సూచీ_name_pos(the_repository->సూచీ, src, అక్షరశ్రేణి_పొడవు(src));
		ఒకవేళ (pos < 0)
			BUG("could not find source in index: '%s'", src);
		ఒకవేళ (!(బహుళకం & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->సూచీ,
						       the_repository->సూచీ->cache[pos],
						       &st,
						       0);
		rename_సూచీ_నమోదు_at(the_repository->సూచీ, pos, dst);

		ఒకవేళ (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			ఒకవేళ ((బహుళకం & SPARSE) &&
			    మార్గం_లోపల_sparse_checkout(dst, the_repository->సూచీ)) {
				/* from out-of-cone to in-cone */
				పూర్ణాంకం dst_pos = సూచీ_name_pos(the_repository->సూచీ, dst,
							     అక్షరశ్రేణి_పొడవు(dst));
				నిర్మాణం cache_నమోదు *dst_ce = the_repository->సూచీ->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				ఒకవేళ (checkout_నమోదు(dst_ce, &state, ఖాళీ, ఖాళీ))
					die(_("cannot checkout %s"), dst_ce->name);
			} లేకపోతే ఒకవేళ ((dst_బహుళకం & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(బహుళకం & SPARSE) &&
				   !మార్గం_లోపల_sparse_checkout(dst, the_repository->సూచీ)) {
				/* from in-cone to out-of-cone */
				పూర్ణాంకం dst_pos = సూచీ_name_pos(the_repository->సూచీ, dst,
							     అక్షరశ్రేణి_పొడవు(dst));
				నిర్మాణం cache_నమోదు *dst_ce = the_repository->సూచీ->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				ఒకవేళ (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} లేకపోతే {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					అక్షరం *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					సురక్షిత_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_ఖాళీలు_src_dirs(src_dir.v, src_dir.nr);

	ఒకవేళ (dirty_paths.nr)
		advise_on_moving_dirty_మార్గం(&dirty_paths);

	ఒకవేళ (gitmodules_modified)
		stage_updated_gitmodules(the_repository->సూచీ);

	ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం_file,
			       COMMIT_తాళం | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

బహిర్గతం:
	strvec_clear(&src_dir);
	విడుదల(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_సరిపోలిక_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_గమ్యం_free);
	hashmap_clear_and_free(&moved_dirs, నిర్మాణం pathmap_నమోదు, ent);
	strbuf_release(&pathbuf);
	విడుదల(submodule_gitfiles);
	విడుదల(modes);
	తిరిగి ret;
}
