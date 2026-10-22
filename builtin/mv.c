/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	ಖಾಲಿ
};

ಎಣಿಕೆ update_ಬಹುಲಕ {
	WORKING_DIRECTORY = (1 << 1),
	ಸೂಚ್ಯಂಕ_2 = (1 << 2),
	SPARSE = (1 << 3),
	SKIP_WORKTREE_DIR = (1 << 4),
	/*
	 * A file gets moved implicitly via a move of one of its parent
	 * directories. This flag causes us to skip the check that we don't try
	 * to move a file and any of its parent directories at the same point
	 * in time.
	 */
	MOVE_ಮೂಲಕ_ಪೋಷಕ_DIR = (1 << 5),
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ needs_worktree_rename(ಎಣಿಕೆ update_ಬಹುಲಕ ಬಹುಲಕ, ಎಣಿಕೆ update_ಬಹುಲಕ dst_ಬಹುಲಕ)
{
	ಹಿಂತಿರುಗಿ !(ಬಹುಲಕ & (ಸೂಚ್ಯಂಕ_2 | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_ಬಹುಲಕ & (SKIP_WORKTREE_DIR | SPARSE));
}

#ವ್ಯಾಖ್ಯಾನಿಸು DUP_BASENAME 1
#ವ್ಯಾಖ್ಯಾನಿಸು KEEP_TRAILING_SLASH 2

ಸ್ಥಿರ ಶೂನ್ಯ ಆಂತರಿಕ_ಪೂರ್ವಪ್ರತ್ಯಯ_pathspec(ರಚನೆ strvec *ಹೊರಹೋಗು,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **pathspec,
				     ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಚಿಹ್ನೆರಹಿತ flags)
{
	ಪೂರ್ಣಾಂಕ prefixlen = ಪೂರ್ವಪ್ರತ್ಯಯ ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಎಣಿಕೆ_2; i++) {
		ಗಾತ್ರ_t ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(pathspec[i]);
		ಗಾತ್ರ_t ಗಮ್ಯ_copy = ಉದ್ದಳತೆ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *maybe_basename;
		ಅಕ್ಷರ *trimmed, *prefixed_ಮಾರ್ಗ;

		ಆಗ (!(flags & KEEP_TRAILING_SLASH) &&
		       ಗಮ್ಯ_copy > 0 && is_dir_sep(pathspec[i][ಗಮ್ಯ_copy - 1]))
			ಗಮ್ಯ_copy--;

		trimmed = xmemdupz(pathspec[i], ಗಮ್ಯ_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_ಮಾರ್ಗ = ಪೂರ್ವಪ್ರತ್ಯಯ_ಮಾರ್ಗ(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, prefixlen, maybe_basename);
		strvec_ತಳ್ಳು(ಹೊರಹೋಗು, prefixed_ಮಾರ್ಗ);

		ಬಿಡುಗಡೆ(prefixed_ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(trimmed);
	}
}

ಸ್ಥಿರ ಅಕ್ಷರ *ಕೂಡಿಸು_slash(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಗಾತ್ರ_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ);
	ಆದರೆ (len && ಮಾರ್ಗ[len - 1] != '/') {
		ಅಕ್ಷರ *with_slash = xmalloc(st_ಕೂಡಿಸು(len, 2));
		ಸ್ಮರಣೆ_ನಕಲು(with_slash, ಮಾರ್ಗ, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		ಹಿಂತಿರುಗಿ with_slash;
	}
	ಹಿಂತಿರುಗಿ xstrdup(ಮಾರ್ಗ);
}

#ವ್ಯಾಖ್ಯಾನಿಸು SUBMODULE_WITH_GITDIR ((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)1)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *submodule_gitfile_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src, ಪೂರ್ಣಾಂಕ ಮೊದಲ)
{
	ರಚನೆ strbuf submodule_dotgit = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;

	ಆದರೆ (!S_ISGITLINK(the_repository->ಸೂಚ್ಯಂಕ->cache[ಮೊದಲ]->ce_ಬಹುಲಕ))
		die(_("Directory %s is in index and no submodule?"), src);
	ಆದರೆ (!is_staging_gitmodules_ok(the_repository->ಸೂಚ್ಯಂಕ))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	ಮಾರ್ಗ = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	ಆದರೆ (ಮಾರ್ಗ)
		ಹಿಂತಿರುಗಿ ಮಾರ್ಗ;
	ಹಿಂತಿರುಗಿ SUBMODULE_WITH_GITDIR;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ_range_ದ_same_dir(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src, ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ,
				   ಪೂರ್ಣಾಂಕ *ಮೊದಲ_p, ಪೂರ್ಣಾಂಕ *ಕೊನೆಯ_p)
{
	ಅಕ್ಷರ *src_w_slash = ಕೂಡಿಸು_slash(src);
	ಪೂರ್ಣಾಂಕ ಮೊದಲ, ಕೊನೆಯ, len_w_slash = ಉದ್ದಳತೆ + 1;

	ಮೊದಲ = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, src_w_slash, len_w_slash);
	ಆದರೆ (ಮೊದಲ >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	ಮೊದಲ = -1 - ಮೊದಲ;
	ಗಾಗಿ (ಕೊನೆಯ = ಮೊದಲ; ಕೊನೆಯ < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; ಕೊನೆಯ++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = the_repository->ಸೂಚ್ಯಂಕ->cache[ಕೊನೆಯ]->name;
		ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ಮಾರ್ಗ, src_w_slash, len_w_slash))
			ನಿಲ್ಲಿಸು;
	}

	ಬಿಡುಗಡೆ(src_w_slash);
	*ಮೊದಲ_p = ಮೊದಲ;
	*ಕೊನೆಯ_p = ಕೊನೆಯ;
	ಹಿಂತಿರುಗಿ ಕೊನೆಯ - ಮೊದಲ;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಖಾಲಿಗಳು_dir_has_sparse_contents(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಅಕ್ಷರ *with_slash = ಕೂಡಿಸು_slash(name);
	ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(with_slash);

	ಪೂರ್ಣಾಂಕ pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, with_slash, ಉದ್ದಳತೆ);
	ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce;

	ಆದರೆ (pos < 0) {
		pos = -pos - 1;
		ಆದರೆ (pos >= the_repository->ಸೂಚ್ಯಂಕ->cache_nr)
			ಹೋಗು free_return;
		ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
		ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(with_slash, ce->name, ಉದ್ದಳತೆ))
			ಹೋಗು free_return;
		ಆದರೆ (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	ಬಿಡುಗಡೆ(with_slash);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_ಖಾಲಿಗಳು_src_dirs(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **src_dir, ಗಾತ್ರ_t src_dir_nr)
{
	ಗಾತ್ರ_t i;
	ರಚನೆ strbuf a_src_dir = STRBUF_INIT;

	ಗಾಗಿ (i = 0; i < src_dir_nr; i++) {
		ಪೂರ್ಣಾಂಕ dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		ಆದರೆ (ಸೂಚ್ಯಂಕ_range_ದ_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

ರಚನೆ pathmap_ನಮೂದು {
	ರಚನೆ hashmap_ನಮೂದು ent;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pathmap_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *cmp_ದತ್ತಾಂಶ ಬಳಸದ,
		       ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *a,
		       ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *b,
		       ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ pathmap_ನಮೂದು *e1 = container_ದ(a, ರಚನೆ pathmap_ನಮೂದು, ent);
	ಸ್ಥಿರಾಂಕ ರಚನೆ pathmap_ನಮೂದು *e2 = container_ದ(b, ರಚನೆ pathmap_ನಮೂದು, ent);
	ಹಿಂತಿರುಗಿ fspathcmp(e1->ಮಾರ್ಗ, e2->ಮಾರ್ಗ);
}

ಪೂರ್ಣಾಂಕ cmd_mv(ಪೂರ್ಣಾಂಕ argc,
	   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i, flags, gitmodules_modified = 0;
	ಪೂರ್ಣಾಂಕ verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	ರಚನೆ option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_DRY_ಸತತ(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', ಖಾಲಿ, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ strvec sources = STRVEC_INIT;
	ರಚನೆ strvec dest_paths = STRVEC_INIT;
	ರಚನೆ strvec destinations = STRVEC_INIT;
	ರಚನೆ strvec submodule_gitfiles_ಗಮ್ಯ_free = STRVEC_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **submodule_gitfiles;
	ಅಕ್ಷರ *dst_w_slash = ಖಾಲಿ;
	ರಚನೆ strvec src_dir = STRVEC_INIT;
	ಎಣಿಕೆ update_ಬಹುಲಕ *modes, dst_ಬಹುಲಕ = 0;
	ರಚನೆ stat st, dest_st;
	ರಚನೆ string_list src_for_dst = STRING_LIST_INIT_DUP;
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ರಚನೆ cache_ನಮೂದು *ce;
	ರಚನೆ string_list only_ಹೊಂದಿಕೆ_skip_worktree = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list dirty_paths = STRING_LIST_INIT_DUP;
	ರಚನೆ hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, ಖಾಲಿ);
	ರಚನೆ strbuf pathbuf = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_mv_options,
			     builtin_mv_usage, 0);
	ಆದರೆ (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, ಬೀಗ_DIE_ON_ERROR);
	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
		die(_("index file corrupt"));

	ಆಂತರಿಕ_ಪೂರ್ವಪ್ರತ್ಯಯ_pathspec(&sources, ಪೂರ್ವಪ್ರತ್ಯಯ, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	ಆದರೆ (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	ಆಂತರಿಕ_ಪೂರ್ವಪ್ರತ್ಯಯ_pathspec(&dest_paths, ಪೂರ್ವಪ್ರತ್ಯಯ, argv + argc, 1, flags);
	dst_w_slash = ಕೂಡಿಸು_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, ಗಾತ್ರ(ಅಕ್ಷರ *));

	ಆದರೆ (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		ಆಂತರಿಕ_ಪೂರ್ವಪ್ರತ್ಯಯ_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_ಬಹುಲಕ)) {
		ಆಂತರಿಕ_ಪೂರ್ವಪ್ರತ್ಯಯ_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಮಾರ್ಗ_ಒಳ_sparse_checkout(dst_w_slash, the_repository->ಸೂಚ್ಯಂಕ) &&
		   ಖಾಲಿಗಳು_dir_has_sparse_contents(dst_w_slash)) {
		ಆಂತರಿಕ_ಪೂರ್ವಪ್ರತ್ಯಯ_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_ಬಹುಲಕ = SKIP_WORKTREE_DIR;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		ಆದರೆ (!ಮಾರ್ಗ_ಒಳ_cone_ಬಹುಲಕ_sparse_checkout(destinations.v[0], the_repository->ಸೂಚ್ಯಂಕ))
			dst_ಬಹುಲಕ = SPARSE;
	}

	/* Checking */
	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src = sources.v[i], *dst = destinations.v[i];
		ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bad = ಖಾಲಿ;
		ಪೂರ್ಣಾಂಕ skip_sparse = 0;

		ಆದರೆ (show_only)
			ಬರೆ(_("Checking rename of '%s' to '%s'\n"), src, dst);

		ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(src);
		ಆದರೆ (lstat(src, &st) < 0) {
			ಪೂರ್ಣಾಂಕ pos;
			ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce;

			pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, src, ಉದ್ದಳತೆ);
			ಆದರೆ (pos < 0) {
				ಅಕ್ಷರ *src_w_slash = ಕೂಡಿಸು_slash(src);
				ಆದರೆ (!ಮಾರ್ಗ_ಒಳ_sparse_checkout(src_w_slash, the_repository->ಸೂಚ್ಯಂಕ) &&
				    ಖಾಲಿಗಳು_dir_has_sparse_contents(src)) {
					ಬಿಡುಗಡೆ(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					ಹೋಗು dir_check;
				}
				ಬಿಡುಗಡೆ(src_w_slash);
				/* only error if existence is expected. */
				ಆದರೆ (!(modes[i] & SPARSE))
					bad = _("bad source");
				ಹೋಗು act_on_ನಮೂದು;
			}
			ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
			ಆದರೆ (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				ಹೋಗು act_on_ನಮೂದು;
			}
			ಆದರೆ (!ignore_sparse) {
				string_list_append(&only_ಹೊಂದಿಕೆ_skip_worktree, src);
				ಹೋಗು act_on_ನಮೂದು;
			}
			/* Check if dst exists in index */
			ಆದರೆ (ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, dst, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dst)) < 0) {
				modes[i] |= SPARSE;
				ಹೋಗು act_on_ನಮೂದು;
			}
			ಆದರೆ (!force) {
				bad = _("destination exists");
				ಹೋಗು act_on_ನಮೂದು;
			}
			modes[i] |= SPARSE;
			ಹೋಗು act_on_ನಮೂದು;
		}
		ಆದರೆ (!ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(src, dst, ಉದ್ದಳತೆ) &&
		    (dst[ಉದ್ದಳತೆ] == 0 || dst[ಉದ್ದಳತೆ] == '/')) {
			bad = _("can not move directory into itself");
			ಹೋಗು act_on_ನಮೂದು;
		}
		ಆದರೆ (S_ISDIR(st.st_ಬಹುಲಕ)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			ಹೋಗು act_on_ನಮೂದು;
		}

dir_check:
		ಆದರೆ (S_ISDIR(st.st_ಬಹುಲಕ)) {
			ರಚನೆ pathmap_ನಮೂದು *ನಮೂದು;
			ಅಕ್ಷರ *dst_with_slash;
			ಗಾತ್ರ_t dst_with_slash_len;
			ಪೂರ್ಣಾಂಕ j, n;
			ಪೂರ್ಣಾಂಕ ಮೊದಲ = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, src, ಉದ್ದಳತೆ), ಕೊನೆಯ;

			ಆದರೆ (ಮೊದಲ >= 0) {
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = submodule_gitfile_ಮಾರ್ಗ(src, ಮೊದಲ);
				ಆದರೆ (ಮಾರ್ಗ != SUBMODULE_WITH_GITDIR)
					ಮಾರ್ಗ = strvec_ತಳ್ಳು(&submodule_gitfiles_ಗಮ್ಯ_free, ಮಾರ್ಗ);
				submodule_gitfiles[i] = ಮಾರ್ಗ;
				ಹೋಗು act_on_ನಮೂದು;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸೂಚ್ಯಂಕ_range_ದ_same_dir(src, ಉದ್ದಳತೆ,
							   &ಮೊದಲ, &ಕೊನೆಯ) < 1) {
				bad = _("source directory is empty");
				ಹೋಗು act_on_ನಮೂದು;
			}

			ನಮೂದು = xmalloc(ಗಾತ್ರ(*ನಮೂದು));
			ನಮೂದು->ಮಾರ್ಗ = src;
			hashmap_ನಮೂದು_init(&ನಮೂದು->ent, fspathhash(src));
			hashmap_ಕೂಡಿಸು(&moved_dirs, &ನಮೂದು->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_ತಳ್ಳು(&src_dir, src);

			n = argc + ಕೊನೆಯ - ಮೊದಲ;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = ಕೂಡಿಸು_slash(dst);
			dst_with_slash_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dst_with_slash);

			ಗಾಗಿ (j = 0; j < ಕೊನೆಯ - ಮೊದಲ; j++) {
				ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[ಮೊದಲ + j];
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = ce->name;
				ಅಕ್ಷರ *prefixed_ಮಾರ್ಗ = ಪೂರ್ವಪ್ರತ್ಯಯ_ಮಾರ್ಗ(the_repository, dst_with_slash,
								  dst_with_slash_len, ಮಾರ್ಗ + ಉದ್ದಳತೆ + 1);

				strvec_ತಳ್ಳು(&sources, ಮಾರ್ಗ);
				strvec_ತಳ್ಳು(&destinations, prefixed_ಮಾರ್ಗ);

				modes[argc + j] = MOVE_ಮೂಲಕ_ಪೋಷಕ_DIR | (ce_skip_worktree(ce) ? SPARSE : ಸೂಚ್ಯಂಕ_2);
				submodule_gitfiles[argc + j] = ಖಾಲಿ;

				ಬಿಡುಗಡೆ(prefixed_ಮಾರ್ಗ);
			}

			ಬಿಡುಗಡೆ(dst_with_slash);
			argc += ಕೊನೆಯ - ಮೊದಲ;
			ಹೋಗು act_on_ನಮೂದು;
		}
		ಆದರೆ (!(ce = ಸೂಚ್ಯಂಕ_file_exists(the_repository->ಸೂಚ್ಯಂಕ, src, ಉದ್ದಳತೆ, 0))) {
			bad = _("not under version control");
			ಹೋಗು act_on_ನಮೂದು;
		}
		ಆದರೆ (ce_stage(ce)) {
			bad = _("conflicted");
			ಹೋಗು act_on_ನಮೂದು;
		}
		ಆದರೆ (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			ಆದರೆ (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				ಆದರೆ (S_ISREG(st.st_ಬಹುಲಕ) || S_ISLNK(st.st_ಬಹುಲಕ)) {
					ಆದರೆ (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = ಖಾಲಿ;
				} ಇಲ್ಲದಿದ್ದರೆ
					bad = _("Cannot overwrite");
			}
			ಹೋಗು act_on_ನಮೂದು;
		}
		ಆದರೆ (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			ಹೋಗು act_on_ನಮೂದು;
		}
		ಆದರೆ (is_dir_sep(dst[ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dst) - 1])) {
			bad = _("destination directory does not exist");
			ಹೋಗು act_on_ನಮೂದು;
		}
		ಆದರೆ (has_symlink_leading_ಮಾರ್ಗ(dst, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dst))) {
			bad = _("destination is beyond a symbolic link");
			ಹೋಗು act_on_ನಮೂದು;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		ಆದರೆ (needs_worktree_rename(modes[i], dst_ಬಹುಲಕ)) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *slash_ = strrchr(dst, '/');

			ಆದರೆ (slash_) {
				ರಚನೆ stat dir_st;
				ಅಕ್ಷರ *dst_dir = xstrdup(dst);
				ಅಕ್ಷರ *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				ಆದರೆ (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					ಆದರೆ (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!S_ISDIR(dir_st.st_ಬಹುಲಕ)) {
					bad = _("destination is not a directory");
				}

				ಬಿಡುಗಡೆ(dst_dir);
			}

			ಆದರೆ (bad)
				ಹೋಗು act_on_ನಮೂದು;
		}

		ಆದರೆ (ignore_sparse &&
		    (dst_ಬಹುಲಕ & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    ಸೂಚ್ಯಂಕ_ನಮೂದು_exists(the_repository->ಸೂಚ್ಯಂಕ, dst, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dst))) {
			bad = _("destination exists in the index");
			ಆದರೆ (force) {
				ಆದರೆ (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = ಖಾಲಿ;
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಹೋಗು act_on_ನಮೂದು;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		ಆದರೆ (!ignore_sparse &&
		    !ಮಾರ್ಗ_ಒಳ_sparse_checkout(src, the_repository->ಸೂಚ್ಯಂಕ)) {
			string_list_append(&only_ಹೊಂದಿಕೆ_skip_worktree, src);
			skip_sparse = 1;
		}
		ಆದರೆ (!ignore_sparse &&
		    !ಮಾರ್ಗ_ಒಳ_sparse_checkout(dst, the_repository->ಸೂಚ್ಯಂಕ)) {
			string_list_append(&only_ಹೊಂದಿಕೆ_skip_worktree, dst);
			skip_sparse = 1;
		}

		ಆದರೆ (skip_sparse)
			ಹೋಗು remove_ನಮೂದು;

		string_list_ಸೇರಿಸು(&src_for_dst, dst);

act_on_ನಮೂದು:
		ಆದರೆ (!bad)
			ಮುಂದುವರಿ;
		ಆದರೆ (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_ನಮೂದು:
		ಆದರೆ (--argc > 0) {
			ಪೂರ್ಣಾಂಕ n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *slash_pos;

		ಆದರೆ (modes[i] & MOVE_ಮೂಲಕ_ಪೋಷಕ_DIR)
			ಮುಂದುವರಿ;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		ಆಗ (slash_pos > pathbuf.buf) {
			ರಚನೆ pathmap_ನಮೂದು needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.ಮಾರ್ಗ = pathbuf.buf;
			hashmap_ನಮೂದು_init(&needle.ent, fspathhash(pathbuf.buf));

			ಆದರೆ (hashmap_get_ನಮೂದು(&moved_dirs, &needle, ent, ಖಾಲಿ))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	ಆದರೆ (only_ಹೊಂದಿಕೆ_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_ಹೊಂದಿಕೆ_skip_worktree);
		ಆದರೆ (!ignore_errors) {
			ret = 1;
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src = sources.v[i], *dst = destinations.v[i];
		ಎಣಿಕೆ update_ಬಹುಲಕ ಬಹುಲಕ = modes[i];
		ಪೂರ್ಣಾಂಕ pos;
		ಪೂರ್ಣಾಂಕ sparse_and_dirty = 0;
		ರಚನೆ checkout state = CHECKOUT_INIT;
		state.istate = the_repository->ಸೂಚ್ಯಂಕ;

		ಆದರೆ (force)
			state.force = 1;
		ಆದರೆ (show_only || verbose)
			ಬರೆ(_("Renaming %s to %s\n"), src, dst);
		ಆದರೆ (show_only)
			ಮುಂದುವರಿ;
		ಆದರೆ (needs_worktree_rename(ಬಹುಲಕ, dst_ಬಹುಲಕ) &&
		    rename(src, dst) < 0) {
			ಆದರೆ (ignore_errors)
				ಮುಂದುವರಿ;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		ಆದರೆ (submodule_gitfiles[i]) {
			ಆದರೆ (!update_ಮಾರ್ಗ_ಒಳ_gitmodules(src, dst))
				gitmodules_modified = 1;
			ಆದರೆ (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_ವೃಕ್ಷ_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		ಆದರೆ (ಬಹುಲಕ & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			ಮುಂದುವರಿ;

		pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, src, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(src));
		ಆದರೆ (pos < 0)
			BUG("could not find source in index: '%s'", src);
		ಆದರೆ (!(ಬಹುಲಕ & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->ಸೂಚ್ಯಂಕ,
						       the_repository->ಸೂಚ್ಯಂಕ->cache[pos],
						       &st,
						       0);
		rename_ಸೂಚ್ಯಂಕ_ನಮೂದು_at(the_repository->ಸೂಚ್ಯಂಕ, pos, dst);

		ಆದರೆ (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			ಆದರೆ ((ಬಹುಲಕ & SPARSE) &&
			    ಮಾರ್ಗ_ಒಳ_sparse_checkout(dst, the_repository->ಸೂಚ್ಯಂಕ)) {
				/* from out-of-cone to in-cone */
				ಪೂರ್ಣಾಂಕ dst_pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, dst,
							     ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dst));
				ರಚನೆ cache_ನಮೂದು *dst_ce = the_repository->ಸೂಚ್ಯಂಕ->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				ಆದರೆ (checkout_ನಮೂದು(dst_ce, &state, ಖಾಲಿ, ಖಾಲಿ))
					die(_("cannot checkout %s"), dst_ce->name);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((dst_ಬಹುಲಕ & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(ಬಹುಲಕ & SPARSE) &&
				   !ಮಾರ್ಗ_ಒಳ_sparse_checkout(dst, the_repository->ಸೂಚ್ಯಂಕ)) {
				/* from in-cone to out-of-cone */
				ಪೂರ್ಣಾಂಕ dst_pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, dst,
							     ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dst));
				ರಚನೆ cache_ನಮೂದು *dst_ce = the_repository->ಸೂಚ್ಯಂಕ->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				ಆದರೆ (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} ಇಲ್ಲದಿದ್ದರೆ {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					ಅಕ್ಷರ *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					ಸುರಕ್ಷಿತ_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_ಖಾಲಿಗಳು_src_dirs(src_dir.v, src_dir.nr);

	ಆದರೆ (dirty_paths.nr)
		advise_on_moving_dirty_ಮಾರ್ಗ(&dirty_paths);

	ಆದರೆ (gitmodules_modified)
		stage_updated_gitmodules(the_repository->ಸೂಚ್ಯಂಕ);

	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file,
			       COMMIT_ಬೀಗ | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

ಹೊರಹೋಗು:
	strvec_clear(&src_dir);
	ಬಿಡುಗಡೆ(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_ಹೊಂದಿಕೆ_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_ಗಮ್ಯ_free);
	hashmap_clear_and_free(&moved_dirs, ರಚನೆ pathmap_ನಮೂದು, ent);
	strbuf_release(&pathbuf);
	ಬಿಡುಗಡೆ(submodule_gitfiles);
	ಬಿಡುಗಡೆ(modes);
	ಹಿಂತಿರುಗಿ ret;
}
