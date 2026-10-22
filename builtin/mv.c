/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "name-hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "symlinks.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "entry.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	ਨਿਰਾ
};

ਗਿਣਤੀ update_ਬਹੁਲਕ {
	WORKING_DIRECTORY = (1 << 1),
	ਸੂਚਕਾਂਕ_2 = (1 << 2),
	SPARSE = (1 << 3),
	SKIP_WORKTREE_DIR = (1 << 4),
	/*
	 * A file gets moved implicitly via a move of one of its parent
	 * directories. This flag causes us to skip the check that we don't try
	 * to move a file and any of its parent directories at the same point
	 * in time.
	 */
	MOVE_ਰਾਹੀਂ_ਮਾਪਾ_DIR = (1 << 5),
};

ਸਥਿਰ ਪੂਰਨ needs_worktree_rename(ਗਿਣਤੀ update_ਬਹੁਲਕ ਬਹੁਲਕ, ਗਿਣਤੀ update_ਬਹੁਲਕ dst_ਬਹੁਲਕ)
{
	ਵਾਪਸ !(ਬਹੁਲਕ & (ਸੂਚਕਾਂਕ_2 | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_ਬਹੁਲਕ & (SKIP_WORKTREE_DIR | SPARSE));
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DUP_BASENAME 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KEEP_TRAILING_SLASH 2

ਸਥਿਰ ਖਾਲੀ ਅੰਦਰੂਨੀ_ਅਗੇਤਰ_pathspec(ਬਣਤਰ strvec *ਬਾਹਰਲਾ,
				     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				     ਅਚਲ ਅੱਖਰ **pathspec,
				     ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ flags)
{
	ਪੂਰਨ prefixlen = ਅਗੇਤਰ ? ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	ਲਈ (ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++) {
		ਮਾਪ_t ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ(pathspec[i]);
		ਮਾਪ_t ਮੰਜ਼ਿਲ_copy = ਲੰਮਾਈ;
		ਅਚਲ ਅੱਖਰ *maybe_basename;
		ਅੱਖਰ *trimmed, *prefixed_ਰਸਤਾ;

		ਜਦੋਂ (!(flags & KEEP_TRAILING_SLASH) &&
		       ਮੰਜ਼ਿਲ_copy > 0 && is_dir_sep(pathspec[i][ਮੰਜ਼ਿਲ_copy - 1]))
			ਮੰਜ਼ਿਲ_copy--;

		trimmed = xmemdupz(pathspec[i], ਮੰਜ਼ਿਲ_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_ਰਸਤਾ = ਅਗੇਤਰ_ਰਸਤਾ(the_repository, ਅਗੇਤਰ, prefixlen, maybe_basename);
		strvec_ਧੱਕੋ(ਬਾਹਰਲਾ, prefixed_ਰਸਤਾ);

		ਮੁਕਤ(prefixed_ਰਸਤਾ);
		ਮੁਕਤ(trimmed);
	}
}

ਸਥਿਰ ਅੱਖਰ *ਜੋੜੋ_slash(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਮਾਪ_t len = ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ);
	ਜੇ (len && ਰਸਤਾ[len - 1] != '/') {
		ਅੱਖਰ *with_slash = xmalloc(st_ਜੋੜੋ(len, 2));
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(with_slash, ਰਸਤਾ, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		ਵਾਪਸ with_slash;
	}
	ਵਾਪਸ xstrdup(ਰਸਤਾ);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SUBMODULE_WITH_GITDIR ((ਅਚਲ ਅੱਖਰ *)1)

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *submodule_gitfile_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *src, ਪੂਰਨ ਪਹਿਲਾ)
{
	ਬਣਤਰ strbuf submodule_dotgit = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;

	ਜੇ (!S_ISGITLINK(the_repository->ਸੂਚਕਾਂਕ->cache[ਪਹਿਲਾ]->ce_ਬਹੁਲਕ))
		die(_("Directory %s is in index and no submodule?"), src);
	ਜੇ (!is_staging_gitmodules_ok(the_repository->ਸੂਚਕਾਂਕ))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	ਰਸਤਾ = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	ਜੇ (ਰਸਤਾ)
		ਵਾਪਸ ਰਸਤਾ;
	ਵਾਪਸ SUBMODULE_WITH_GITDIR;
}

ਸਥਿਰ ਪੂਰਨ ਸੂਚਕਾਂਕ_range_ਦਾ_same_dir(ਅਚਲ ਅੱਖਰ *src, ਪੂਰਨ ਲੰਮਾਈ,
				   ਪੂਰਨ *ਪਹਿਲਾ_p, ਪੂਰਨ *ਆਖਰੀ_p)
{
	ਅੱਖਰ *src_w_slash = ਜੋੜੋ_slash(src);
	ਪੂਰਨ ਪਹਿਲਾ, ਆਖਰੀ, len_w_slash = ਲੰਮਾਈ + 1;

	ਪਹਿਲਾ = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, src_w_slash, len_w_slash);
	ਜੇ (ਪਹਿਲਾ >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	ਪਹਿਲਾ = -1 - ਪਹਿਲਾ;
	ਲਈ (ਆਖਰੀ = ਪਹਿਲਾ; ਆਖਰੀ < the_repository->ਸੂਚਕਾਂਕ->cache_nr; ਆਖਰੀ++) {
		ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = the_repository->ਸੂਚਕਾਂਕ->cache[ਆਖਰੀ]->name;
		ਜੇ (ਲੜੀ_ਤੁਲਨਾ(ਰਸਤਾ, src_w_slash, len_w_slash))
			ਰੋਕੋ;
	}

	ਮੁਕਤ(src_w_slash);
	*ਪਹਿਲਾ_p = ਪਹਿਲਾ;
	*ਆਖਰੀ_p = ਆਖਰੀ;
	ਵਾਪਸ ਆਖਰੀ - ਪਹਿਲਾ;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
ਸਥਿਰ ਪੂਰਨ ਖਾਲੀ_ਥਾਵਾਂ_dir_has_sparse_contents(ਅਚਲ ਅੱਖਰ *name)
{
	ਪੂਰਨ ret = 0;
	ਅੱਖਰ *with_slash = ਜੋੜੋ_slash(name);
	ਪੂਰਨ ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ(with_slash);

	ਪੂਰਨ pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, with_slash, ਲੰਮਾਈ);
	ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;

	ਜੇ (pos < 0) {
		pos = -pos - 1;
		ਜੇ (pos >= the_repository->ਸੂਚਕਾਂਕ->cache_nr)
			ਜਾਓ free_return;
		ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
		ਜੇ (ਲੜੀ_ਤੁਲਨਾ(with_slash, ce->name, ਲੰਮਾਈ))
			ਜਾਓ free_return;
		ਜੇ (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	ਮੁਕਤ(with_slash);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ remove_ਖਾਲੀ_ਥਾਵਾਂ_src_dirs(ਅਚਲ ਅੱਖਰ **src_dir, ਮਾਪ_t src_dir_nr)
{
	ਮਾਪ_t i;
	ਬਣਤਰ strbuf a_src_dir = STRBUF_INIT;

	ਲਈ (i = 0; i < src_dir_nr; i++) {
		ਪੂਰਨ dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		ਜੇ (ਸੂਚਕਾਂਕ_range_ਦਾ_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

ਬਣਤਰ pathmap_ਇੰਦਰਾਜ {
	ਬਣਤਰ hashmap_ਇੰਦਰਾਜ ent;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
};

ਸਥਿਰ ਪੂਰਨ pathmap_cmp(ਅਚਲ ਖਾਲੀ *cmp_ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
		       ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *a,
		       ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *b,
		       ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਬਣਤਰ pathmap_ਇੰਦਰਾਜ *e1 = container_ਦਾ(a, ਬਣਤਰ pathmap_ਇੰਦਰਾਜ, ent);
	ਅਚਲ ਬਣਤਰ pathmap_ਇੰਦਰਾਜ *e2 = container_ਦਾ(b, ਬਣਤਰ pathmap_ਇੰਦਰਾਜ, ent);
	ਵਾਪਸ fspathcmp(e1->ਰਸਤਾ, e2->ਰਸਤਾ);
}

ਪੂਰਨ cmd_mv(ਪੂਰਨ argc,
	   ਅਚਲ ਅੱਖਰ **argv,
	   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i, flags, gitmodules_modified = 0;
	ਪੂਰਨ verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	ਬਣਤਰ option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_DRY_ਲੜੀਵਾਰ(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', ਨਿਰਾ, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ strvec sources = STRVEC_INIT;
	ਬਣਤਰ strvec dest_paths = STRVEC_INIT;
	ਬਣਤਰ strvec destinations = STRVEC_INIT;
	ਬਣਤਰ strvec submodule_gitfiles_ਮੰਜ਼ਿਲ_free = STRVEC_INIT;
	ਅਚਲ ਅੱਖਰ **submodule_gitfiles;
	ਅੱਖਰ *dst_w_slash = ਨਿਰਾ;
	ਬਣਤਰ strvec src_dir = STRVEC_INIT;
	ਗਿਣਤੀ update_ਬਹੁਲਕ *modes, dst_ਬਹੁਲਕ = 0;
	ਬਣਤਰ stat st, dest_st;
	ਬਣਤਰ string_list src_for_dst = STRING_LIST_INIT_DUP;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;
	ਬਣਤਰ string_list only_ਮੇਲ_skip_worktree = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list dirty_paths = STRING_LIST_INIT_DUP;
	ਬਣਤਰ hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, ਨਿਰਾ);
	ਬਣਤਰ strbuf pathbuf = STRBUF_INIT;
	ਪੂਰਨ ret;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_mv_options,
			     builtin_mv_usage, 0);
	ਜੇ (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, ਤਾਲਾ_DIE_ON_ERROR);
	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
		die(_("index file corrupt"));

	ਅੰਦਰੂਨੀ_ਅਗੇਤਰ_pathspec(&sources, ਅਗੇਤਰ, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	ਜੇ (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	ਅੰਦਰੂਨੀ_ਅਗੇਤਰ_pathspec(&dest_paths, ਅਗੇਤਰ, argv + argc, 1, flags);
	dst_w_slash = ਜੋੜੋ_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, ਆਕਾਰ(ਅੱਖਰ *));

	ਜੇ (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		ਅੰਦਰੂਨੀ_ਅਗੇਤਰ_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	ਨਹੀਂਤਾਂ ਜੇ (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_ਬਹੁਲਕ)) {
		ਅੰਦਰੂਨੀ_ਅਗੇਤਰ_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} ਨਹੀਂਤਾਂ ਜੇ (!ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(dst_w_slash, the_repository->ਸੂਚਕਾਂਕ) &&
		   ਖਾਲੀ_ਥਾਵਾਂ_dir_has_sparse_contents(dst_w_slash)) {
		ਅੰਦਰੂਨੀ_ਅਗੇਤਰ_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_ਬਹੁਲਕ = SKIP_WORKTREE_DIR;
	} ਨਹੀਂਤਾਂ ਜੇ (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} ਨਹੀਂਤਾਂ {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		ਜੇ (!ਰਸਤਾ_ਵਿੱਚ_cone_ਬਹੁਲਕ_sparse_checkout(destinations.v[0], the_repository->ਸੂਚਕਾਂਕ))
			dst_ਬਹੁਲਕ = SPARSE;
	}

	/* Checking */
	ਲਈ (i = 0; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *src = sources.v[i], *dst = destinations.v[i];
		ਪੂਰਨ ਲੰਮਾਈ;
		ਅਚਲ ਅੱਖਰ *bad = ਨਿਰਾ;
		ਪੂਰਨ skip_sparse = 0;

		ਜੇ (show_only)
			ਲਿਖੋ(_("Checking rename of '%s' to '%s'\n"), src, dst);

		ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ(src);
		ਜੇ (lstat(src, &st) < 0) {
			ਪੂਰਨ pos;
			ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;

			pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, src, ਲੰਮਾਈ);
			ਜੇ (pos < 0) {
				ਅੱਖਰ *src_w_slash = ਜੋੜੋ_slash(src);
				ਜੇ (!ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(src_w_slash, the_repository->ਸੂਚਕਾਂਕ) &&
				    ਖਾਲੀ_ਥਾਵਾਂ_dir_has_sparse_contents(src)) {
					ਮੁਕਤ(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					ਜਾਓ dir_check;
				}
				ਮੁਕਤ(src_w_slash);
				/* only error if existence is expected. */
				ਜੇ (!(modes[i] & SPARSE))
					bad = _("bad source");
				ਜਾਓ act_on_ਇੰਦਰਾਜ;
			}
			ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
			ਜੇ (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				ਜਾਓ act_on_ਇੰਦਰਾਜ;
			}
			ਜੇ (!ignore_sparse) {
				string_list_append(&only_ਮੇਲ_skip_worktree, src);
				ਜਾਓ act_on_ਇੰਦਰਾਜ;
			}
			/* Check if dst exists in index */
			ਜੇ (ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, dst, ਲੜੀ_ਲੰਬਾਈ(dst)) < 0) {
				modes[i] |= SPARSE;
				ਜਾਓ act_on_ਇੰਦਰਾਜ;
			}
			ਜੇ (!force) {
				bad = _("destination exists");
				ਜਾਓ act_on_ਇੰਦਰਾਜ;
			}
			modes[i] |= SPARSE;
			ਜਾਓ act_on_ਇੰਦਰਾਜ;
		}
		ਜੇ (!ਲੜੀ_ਤੁਲਨਾ(src, dst, ਲੰਮਾਈ) &&
		    (dst[ਲੰਮਾਈ] == 0 || dst[ਲੰਮਾਈ] == '/')) {
			bad = _("can not move directory into itself");
			ਜਾਓ act_on_ਇੰਦਰਾਜ;
		}
		ਜੇ (S_ISDIR(st.st_ਬਹੁਲਕ)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			ਜਾਓ act_on_ਇੰਦਰਾਜ;
		}

dir_check:
		ਜੇ (S_ISDIR(st.st_ਬਹੁਲਕ)) {
			ਬਣਤਰ pathmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
			ਅੱਖਰ *dst_with_slash;
			ਮਾਪ_t dst_with_slash_len;
			ਪੂਰਨ j, n;
			ਪੂਰਨ ਪਹਿਲਾ = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, src, ਲੰਮਾਈ), ਆਖਰੀ;

			ਜੇ (ਪਹਿਲਾ >= 0) {
				ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = submodule_gitfile_ਰਸਤਾ(src, ਪਹਿਲਾ);
				ਜੇ (ਰਸਤਾ != SUBMODULE_WITH_GITDIR)
					ਰਸਤਾ = strvec_ਧੱਕੋ(&submodule_gitfiles_ਮੰਜ਼ਿਲ_free, ਰਸਤਾ);
				submodule_gitfiles[i] = ਰਸਤਾ;
				ਜਾਓ act_on_ਇੰਦਰਾਜ;
			} ਨਹੀਂਤਾਂ ਜੇ (ਸੂਚਕਾਂਕ_range_ਦਾ_same_dir(src, ਲੰਮਾਈ,
							   &ਪਹਿਲਾ, &ਆਖਰੀ) < 1) {
				bad = _("source directory is empty");
				ਜਾਓ act_on_ਇੰਦਰਾਜ;
			}

			ਇੰਦਰਾਜ = xmalloc(ਆਕਾਰ(*ਇੰਦਰਾਜ));
			ਇੰਦਰਾਜ->ਰਸਤਾ = src;
			hashmap_ਇੰਦਰਾਜ_init(&ਇੰਦਰਾਜ->ent, fspathhash(src));
			hashmap_ਜੋੜੋ(&moved_dirs, &ਇੰਦਰਾਜ->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_ਧੱਕੋ(&src_dir, src);

			n = argc + ਆਖਰੀ - ਪਹਿਲਾ;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = ਜੋੜੋ_slash(dst);
			dst_with_slash_len = ਲੜੀ_ਲੰਬਾਈ(dst_with_slash);

			ਲਈ (j = 0; j < ਆਖਰੀ - ਪਹਿਲਾ; j++) {
				ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[ਪਹਿਲਾ + j];
				ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = ce->name;
				ਅੱਖਰ *prefixed_ਰਸਤਾ = ਅਗੇਤਰ_ਰਸਤਾ(the_repository, dst_with_slash,
								  dst_with_slash_len, ਰਸਤਾ + ਲੰਮਾਈ + 1);

				strvec_ਧੱਕੋ(&sources, ਰਸਤਾ);
				strvec_ਧੱਕੋ(&destinations, prefixed_ਰਸਤਾ);

				modes[argc + j] = MOVE_ਰਾਹੀਂ_ਮਾਪਾ_DIR | (ce_skip_worktree(ce) ? SPARSE : ਸੂਚਕਾਂਕ_2);
				submodule_gitfiles[argc + j] = ਨਿਰਾ;

				ਮੁਕਤ(prefixed_ਰਸਤਾ);
			}

			ਮੁਕਤ(dst_with_slash);
			argc += ਆਖਰੀ - ਪਹਿਲਾ;
			ਜਾਓ act_on_ਇੰਦਰਾਜ;
		}
		ਜੇ (!(ce = ਸੂਚਕਾਂਕ_file_exists(the_repository->ਸੂਚਕਾਂਕ, src, ਲੰਮਾਈ, 0))) {
			bad = _("not under version control");
			ਜਾਓ act_on_ਇੰਦਰਾਜ;
		}
		ਜੇ (ce_stage(ce)) {
			bad = _("conflicted");
			ਜਾਓ act_on_ਇੰਦਰਾਜ;
		}
		ਜੇ (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			ਜੇ (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				ਜੇ (S_ISREG(st.st_ਬਹੁਲਕ) || S_ISLNK(st.st_ਬਹੁਲਕ)) {
					ਜੇ (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = ਨਿਰਾ;
				} ਨਹੀਂਤਾਂ
					bad = _("Cannot overwrite");
			}
			ਜਾਓ act_on_ਇੰਦਰਾਜ;
		}
		ਜੇ (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			ਜਾਓ act_on_ਇੰਦਰਾਜ;
		}
		ਜੇ (is_dir_sep(dst[ਲੜੀ_ਲੰਬਾਈ(dst) - 1])) {
			bad = _("destination directory does not exist");
			ਜਾਓ act_on_ਇੰਦਰਾਜ;
		}
		ਜੇ (has_symlink_leading_ਰਸਤਾ(dst, ਲੜੀ_ਲੰਬਾਈ(dst))) {
			bad = _("destination is beyond a symbolic link");
			ਜਾਓ act_on_ਇੰਦਰਾਜ;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		ਜੇ (needs_worktree_rename(modes[i], dst_ਬਹੁਲਕ)) {
			ਅਚਲ ਅੱਖਰ *slash_ = strrchr(dst, '/');

			ਜੇ (slash_) {
				ਬਣਤਰ stat dir_st;
				ਅੱਖਰ *dst_dir = xstrdup(dst);
				ਅੱਖਰ *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				ਜੇ (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					ਜੇ (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} ਨਹੀਂਤਾਂ ਜੇ (!S_ISDIR(dir_st.st_ਬਹੁਲਕ)) {
					bad = _("destination is not a directory");
				}

				ਮੁਕਤ(dst_dir);
			}

			ਜੇ (bad)
				ਜਾਓ act_on_ਇੰਦਰਾਜ;
		}

		ਜੇ (ignore_sparse &&
		    (dst_ਬਹੁਲਕ & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ_exists(the_repository->ਸੂਚਕਾਂਕ, dst, ਲੜੀ_ਲੰਬਾਈ(dst))) {
			bad = _("destination exists in the index");
			ਜੇ (force) {
				ਜੇ (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = ਨਿਰਾ;
			} ਨਹੀਂਤਾਂ {
				ਜਾਓ act_on_ਇੰਦਰਾਜ;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		ਜੇ (!ignore_sparse &&
		    !ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(src, the_repository->ਸੂਚਕਾਂਕ)) {
			string_list_append(&only_ਮੇਲ_skip_worktree, src);
			skip_sparse = 1;
		}
		ਜੇ (!ignore_sparse &&
		    !ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(dst, the_repository->ਸੂਚਕਾਂਕ)) {
			string_list_append(&only_ਮੇਲ_skip_worktree, dst);
			skip_sparse = 1;
		}

		ਜੇ (skip_sparse)
			ਜਾਓ remove_ਇੰਦਰਾਜ;

		string_list_ਪਾਓ(&src_for_dst, dst);

act_on_ਇੰਦਰਾਜ:
		ਜੇ (!bad)
			ਜਾਰੀ;
		ਜੇ (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_ਇੰਦਰਾਜ:
		ਜੇ (--argc > 0) {
			ਪੂਰਨ n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	ਲਈ (i = 0; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *slash_pos;

		ਜੇ (modes[i] & MOVE_ਰਾਹੀਂ_ਮਾਪਾ_DIR)
			ਜਾਰੀ;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		ਜਦੋਂ (slash_pos > pathbuf.buf) {
			ਬਣਤਰ pathmap_ਇੰਦਰਾਜ needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.ਰਸਤਾ = pathbuf.buf;
			hashmap_ਇੰਦਰਾਜ_init(&needle.ent, fspathhash(pathbuf.buf));

			ਜੇ (hashmap_get_ਇੰਦਰਾਜ(&moved_dirs, &needle, ent, ਨਿਰਾ))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	ਜੇ (only_ਮੇਲ_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_ਮੇਲ_skip_worktree);
		ਜੇ (!ignore_errors) {
			ret = 1;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	ਲਈ (i = 0; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *src = sources.v[i], *dst = destinations.v[i];
		ਗਿਣਤੀ update_ਬਹੁਲਕ ਬਹੁਲਕ = modes[i];
		ਪੂਰਨ pos;
		ਪੂਰਨ sparse_and_dirty = 0;
		ਬਣਤਰ checkout state = CHECKOUT_INIT;
		state.istate = the_repository->ਸੂਚਕਾਂਕ;

		ਜੇ (force)
			state.force = 1;
		ਜੇ (show_only || verbose)
			ਲਿਖੋ(_("Renaming %s to %s\n"), src, dst);
		ਜੇ (show_only)
			ਜਾਰੀ;
		ਜੇ (needs_worktree_rename(ਬਹੁਲਕ, dst_ਬਹੁਲਕ) &&
		    rename(src, dst) < 0) {
			ਜੇ (ignore_errors)
				ਜਾਰੀ;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		ਜੇ (submodule_gitfiles[i]) {
			ਜੇ (!update_ਰਸਤਾ_ਵਿੱਚ_gitmodules(src, dst))
				gitmodules_modified = 1;
			ਜੇ (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_ਰੁੱਖ_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		ਜੇ (ਬਹੁਲਕ & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			ਜਾਰੀ;

		pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, src, ਲੜੀ_ਲੰਬਾਈ(src));
		ਜੇ (pos < 0)
			BUG("could not find source in index: '%s'", src);
		ਜੇ (!(ਬਹੁਲਕ & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->ਸੂਚਕਾਂਕ,
						       the_repository->ਸੂਚਕਾਂਕ->cache[pos],
						       &st,
						       0);
		rename_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ_at(the_repository->ਸੂਚਕਾਂਕ, pos, dst);

		ਜੇ (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			ਜੇ ((ਬਹੁਲਕ & SPARSE) &&
			    ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(dst, the_repository->ਸੂਚਕਾਂਕ)) {
				/* from out-of-cone to in-cone */
				ਪੂਰਨ dst_pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, dst,
							     ਲੜੀ_ਲੰਬਾਈ(dst));
				ਬਣਤਰ cache_ਇੰਦਰਾਜ *dst_ce = the_repository->ਸੂਚਕਾਂਕ->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				ਜੇ (checkout_ਇੰਦਰਾਜ(dst_ce, &state, ਨਿਰਾ, ਨਿਰਾ))
					die(_("cannot checkout %s"), dst_ce->name);
			} ਨਹੀਂਤਾਂ ਜੇ ((dst_ਬਹੁਲਕ & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(ਬਹੁਲਕ & SPARSE) &&
				   !ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(dst, the_repository->ਸੂਚਕਾਂਕ)) {
				/* from in-cone to out-of-cone */
				ਪੂਰਨ dst_pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, dst,
							     ਲੜੀ_ਲੰਬਾਈ(dst));
				ਬਣਤਰ cache_ਇੰਦਰਾਜ *dst_ce = the_repository->ਸੂਚਕਾਂਕ->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				ਜੇ (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} ਨਹੀਂਤਾਂ {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					ਅੱਖਰ *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					ਸੁਰੱਖਿਅਤ_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_ਖਾਲੀ_ਥਾਵਾਂ_src_dirs(src_dir.v, src_dir.nr);

	ਜੇ (dirty_paths.nr)
		advise_on_moving_dirty_ਰਸਤਾ(&dirty_paths);

	ਜੇ (gitmodules_modified)
		stage_updated_gitmodules(the_repository->ਸੂਚਕਾਂਕ);

	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file,
			       COMMIT_ਤਾਲਾ | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

ਬਾਹਰਲਾ:
	strvec_clear(&src_dir);
	ਮੁਕਤ(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_ਮੇਲ_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_ਮੰਜ਼ਿਲ_free);
	hashmap_clear_and_free(&moved_dirs, ਬਣਤਰ pathmap_ਇੰਦਰਾਜ, ent);
	strbuf_release(&pathbuf);
	ਮੁਕਤ(submodule_gitfiles);
	ਮੁਕਤ(modes);
	ਵਾਪਸ ret;
}
