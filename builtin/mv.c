/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

تعداد update_منوال {
	WORKING_DIRECTORY = (1 << 1),
	اشاریہ_2 = (1 << 2),
	SPARSE = (1 << 3),
	SKIP_WORKTREE_DIR = (1 << 4),
	/*
	 * A file gets moved implicitly via a move of one of its parent
	 * directories. This flag causes us to skip the check that we don't try
	 * to move a file and any of its parent directories at the same point
	 * in time.
	 */
	MOVE_راہیں_ماپا_DIR = (1 << 5),
};

ساكن صحيح needs_worktree_rename(تعداد update_منوال منوال, تعداد update_منوال dst_منوال)
{
	أرجع !(منوال & (اشاریہ_2 | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_منوال & (SKIP_WORKTREE_DIR | SPARSE));
}

#تعریف_کرو DUP_BASENAME 1
#تعریف_کرو KEEP_TRAILING_SLASH 2

ساكن فراغ اندرونی_اگیتر_pathspec(بنية strvec *باہرلا,
				     ثابت حرف *اگیتر,
				     ثابت حرف **pathspec,
				     صحيح گنتی, بنانشانی flags)
{
	صحيح prefixlen = اگیتر ? لڑی_لمبائی(اگیتر) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	لكل (صحيح i = 0; i < گنتی; i++) {
		ناپ_t لمائی = لڑی_لمبائی(pathspec[i]);
		ناپ_t منزل_copy = لمائی;
		ثابت حرف *maybe_basename;
		حرف *trimmed, *prefixed_رستہ;

		طالما (!(flags & KEEP_TRAILING_SLASH) &&
		       منزل_copy > 0 && is_dir_sep(pathspec[i][منزل_copy - 1]))
			منزل_copy--;

		trimmed = xmemdupz(pathspec[i], منزل_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_رستہ = اگیتر_رستہ(the_repository, اگیتر, prefixlen, maybe_basename);
		strvec_ڈالو(باہرلا, prefixed_رستہ);

		حرر(prefixed_رستہ);
		حرر(trimmed);
	}
}

ساكن حرف *جوڑو_slash(ثابت حرف *رستہ)
{
	ناپ_t len = لڑی_لمبائی(رستہ);
	إذا (len && رستہ[len - 1] != '/') {
		حرف *with_slash = xmalloc(st_جوڑو(len, 2));
		حافظہ_نقل(with_slash, رستہ, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		أرجع with_slash;
	}
	أرجع xstrdup(رستہ);
}

#تعریف_کرو SUBMODULE_WITH_GITDIR ((ثابت حرف *)1)

ساكن ثابت حرف *submodule_gitfile_رستہ(ثابت حرف *src, صحيح پہلا)
{
	بنية strbuf submodule_dotgit = STRBUF_INIT;
	ثابت حرف *رستہ;

	إذا (!S_ISGITLINK(the_repository->اشاریہ->cache[پہلا]->ce_منوال))
		die(_("Directory %s is in index and no submodule?"), src);
	إذا (!is_staging_gitmodules_ok(the_repository->اشاریہ))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	رستہ = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	إذا (رستہ)
		أرجع رستہ;
	أرجع SUBMODULE_WITH_GITDIR;
}

ساكن صحيح اشاریہ_range_دا_same_dir(ثابت حرف *src, صحيح لمائی,
				   صحيح *پہلا_p, صحيح *آخری_p)
{
	حرف *src_w_slash = جوڑو_slash(src);
	صحيح پہلا, آخری, len_w_slash = لمائی + 1;

	پہلا = اشاریہ_name_pos(the_repository->اشاریہ, src_w_slash, len_w_slash);
	إذا (پہلا >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	پہلا = -1 - پہلا;
	لكل (آخری = پہلا; آخری < the_repository->اشاریہ->cache_nr; آخری++) {
		ثابت حرف *رستہ = the_repository->اشاریہ->cache[آخری]->name;
		إذا (لڑی_مقابلہ(رستہ, src_w_slash, len_w_slash))
			اكسر;
	}

	حرر(src_w_slash);
	*پہلا_p = پہلا;
	*آخری_p = آخری;
	أرجع آخری - پہلا;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
ساكن صحيح خالی_تھاواں_dir_has_sparse_contents(ثابت حرف *name)
{
	صحيح ret = 0;
	حرف *with_slash = جوڑو_slash(name);
	صحيح لمائی = لڑی_لمبائی(with_slash);

	صحيح pos = اشاریہ_name_pos(the_repository->اشاریہ, with_slash, لمائی);
	ثابت بنية cache_اندراج *ce;

	إذا (pos < 0) {
		pos = -pos - 1;
		إذا (pos >= the_repository->اشاریہ->cache_nr)
			اذهب free_return;
		ce = the_repository->اشاریہ->cache[pos];
		إذا (لڑی_مقابلہ(with_slash, ce->name, لمائی))
			اذهب free_return;
		إذا (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	حرر(with_slash);
	أرجع ret;
}

ساكن فراغ remove_خالی_تھاواں_src_dirs(ثابت حرف **src_dir, ناپ_t src_dir_nr)
{
	ناپ_t i;
	بنية strbuf a_src_dir = STRBUF_INIT;

	لكل (i = 0; i < src_dir_nr; i++) {
		صحيح dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		إذا (اشاریہ_range_دا_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

بنية pathmap_اندراج {
	بنية hashmap_اندراج ent;
	ثابت حرف *رستہ;
};

ساكن صحيح pathmap_cmp(ثابت فراغ *cmp_انکڑے بن_ورتیا,
		       ثابت بنية hashmap_اندراج *a,
		       ثابت بنية hashmap_اندراج *b,
		       ثابت فراغ *کلید بن_ورتیا)
{
	ثابت بنية pathmap_اندراج *e1 = container_دا(a, بنية pathmap_اندراج, ent);
	ثابت بنية pathmap_اندراج *e2 = container_دا(b, بنية pathmap_اندراج, ent);
	أرجع fspathcmp(e1->رستہ, e2->رستہ);
}

صحيح cmd_mv(صحيح argc,
	   ثابت حرف **argv,
	   ثابت حرف *اگیتر,
	   بنية repository *repo بن_ورتیا)
{
	صحيح i, flags, gitmodules_modified = 0;
	صحيح verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	بنية option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_DRY_لگاتار(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', عدم, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_سرا(),
	};
	بنية strvec sources = STRVEC_INIT;
	بنية strvec dest_paths = STRVEC_INIT;
	بنية strvec destinations = STRVEC_INIT;
	بنية strvec submodule_gitfiles_منزل_free = STRVEC_INIT;
	ثابت حرف **submodule_gitfiles;
	حرف *dst_w_slash = عدم;
	بنية strvec src_dir = STRVEC_INIT;
	تعداد update_منوال *modes, dst_منوال = 0;
	بنية stat st, dest_st;
	بنية string_list src_for_dst = STRING_LIST_INIT_DUP;
	بنية جندرا_file جندرا_file = جندرا_INIT;
	بنية cache_اندراج *ce;
	بنية string_list only_میل_skip_worktree = STRING_LIST_INIT_DUP;
	بنية string_list dirty_paths = STRING_LIST_INIT_DUP;
	بنية hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, عدم);
	بنية strbuf pathbuf = STRBUF_INIT;
	صحيح ret;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, اگیتر, builtin_mv_options,
			     builtin_mv_usage, 0);
	إذا (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_اشاریہ(the_repository, &جندرا_file, جندرا_DIE_ON_ERROR);
	إذا (repo_read_اشاریہ(the_repository) < 0)
		die(_("index file corrupt"));

	اندرونی_اگیتر_pathspec(&sources, اگیتر, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	إذا (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	اندرونی_اگیتر_pathspec(&dest_paths, اگیتر, argv + argc, 1, flags);
	dst_w_slash = جوڑو_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, حجم(حرف *));

	إذا (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		اندرونی_اگیتر_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	وإلا إذا (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_منوال)) {
		اندرونی_اگیتر_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} وإلا إذا (!رستہ_وچ_sparse_checkout(dst_w_slash, the_repository->اشاریہ) &&
		   خالی_تھاواں_dir_has_sparse_contents(dst_w_slash)) {
		اندرونی_اگیتر_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_منوال = SKIP_WORKTREE_DIR;
	} وإلا إذا (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} وإلا {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		إذا (!رستہ_وچ_cone_منوال_sparse_checkout(destinations.v[0], the_repository->اشاریہ))
			dst_منوال = SPARSE;
	}

	/* Checking */
	لكل (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		صحيح لمائی;
		ثابت حرف *bad = عدم;
		صحيح skip_sparse = 0;

		إذا (show_only)
			لکھو(_("Checking rename of '%s' to '%s'\n"), src, dst);

		لمائی = لڑی_لمبائی(src);
		إذا (lstat(src, &st) < 0) {
			صحيح pos;
			ثابت بنية cache_اندراج *ce;

			pos = اشاریہ_name_pos(the_repository->اشاریہ, src, لمائی);
			إذا (pos < 0) {
				حرف *src_w_slash = جوڑو_slash(src);
				إذا (!رستہ_وچ_sparse_checkout(src_w_slash, the_repository->اشاریہ) &&
				    خالی_تھاواں_dir_has_sparse_contents(src)) {
					حرر(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					اذهب dir_check;
				}
				حرر(src_w_slash);
				/* only error if existence is expected. */
				إذا (!(modes[i] & SPARSE))
					bad = _("bad source");
				اذهب act_on_اندراج;
			}
			ce = the_repository->اشاریہ->cache[pos];
			إذا (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				اذهب act_on_اندراج;
			}
			إذا (!ignore_sparse) {
				string_list_append(&only_میل_skip_worktree, src);
				اذهب act_on_اندراج;
			}
			/* Check if dst exists in index */
			إذا (اشاریہ_name_pos(the_repository->اشاریہ, dst, لڑی_لمبائی(dst)) < 0) {
				modes[i] |= SPARSE;
				اذهب act_on_اندراج;
			}
			إذا (!force) {
				bad = _("destination exists");
				اذهب act_on_اندراج;
			}
			modes[i] |= SPARSE;
			اذهب act_on_اندراج;
		}
		إذا (!لڑی_مقابلہ(src, dst, لمائی) &&
		    (dst[لمائی] == 0 || dst[لمائی] == '/')) {
			bad = _("can not move directory into itself");
			اذهب act_on_اندراج;
		}
		إذا (S_ISDIR(st.st_منوال)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			اذهب act_on_اندراج;
		}

dir_check:
		إذا (S_ISDIR(st.st_منوال)) {
			بنية pathmap_اندراج *اندراج;
			حرف *dst_with_slash;
			ناپ_t dst_with_slash_len;
			صحيح j, n;
			صحيح پہلا = اشاریہ_name_pos(the_repository->اشاریہ, src, لمائی), آخری;

			إذا (پہلا >= 0) {
				ثابت حرف *رستہ = submodule_gitfile_رستہ(src, پہلا);
				إذا (رستہ != SUBMODULE_WITH_GITDIR)
					رستہ = strvec_ڈالو(&submodule_gitfiles_منزل_free, رستہ);
				submodule_gitfiles[i] = رستہ;
				اذهب act_on_اندراج;
			} وإلا إذا (اشاریہ_range_دا_same_dir(src, لمائی,
							   &پہلا, &آخری) < 1) {
				bad = _("source directory is empty");
				اذهب act_on_اندراج;
			}

			اندراج = xmalloc(حجم(*اندراج));
			اندراج->رستہ = src;
			hashmap_اندراج_init(&اندراج->ent, fspathhash(src));
			hashmap_جوڑو(&moved_dirs, &اندراج->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_ڈالو(&src_dir, src);

			n = argc + آخری - پہلا;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = جوڑو_slash(dst);
			dst_with_slash_len = لڑی_لمبائی(dst_with_slash);

			لكل (j = 0; j < آخری - پہلا; j++) {
				ثابت بنية cache_اندراج *ce = the_repository->اشاریہ->cache[پہلا + j];
				ثابت حرف *رستہ = ce->name;
				حرف *prefixed_رستہ = اگیتر_رستہ(the_repository, dst_with_slash,
								  dst_with_slash_len, رستہ + لمائی + 1);

				strvec_ڈالو(&sources, رستہ);
				strvec_ڈالو(&destinations, prefixed_رستہ);

				modes[argc + j] = MOVE_راہیں_ماپا_DIR | (ce_skip_worktree(ce) ? SPARSE : اشاریہ_2);
				submodule_gitfiles[argc + j] = عدم;

				حرر(prefixed_رستہ);
			}

			حرر(dst_with_slash);
			argc += آخری - پہلا;
			اذهب act_on_اندراج;
		}
		إذا (!(ce = اشاریہ_file_exists(the_repository->اشاریہ, src, لمائی, 0))) {
			bad = _("not under version control");
			اذهب act_on_اندراج;
		}
		إذا (ce_stage(ce)) {
			bad = _("conflicted");
			اذهب act_on_اندراج;
		}
		إذا (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			إذا (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				إذا (S_ISREG(st.st_منوال) || S_ISLNK(st.st_منوال)) {
					إذا (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = عدم;
				} وإلا
					bad = _("Cannot overwrite");
			}
			اذهب act_on_اندراج;
		}
		إذا (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			اذهب act_on_اندراج;
		}
		إذا (is_dir_sep(dst[لڑی_لمبائی(dst) - 1])) {
			bad = _("destination directory does not exist");
			اذهب act_on_اندراج;
		}
		إذا (has_symlink_leading_رستہ(dst, لڑی_لمبائی(dst))) {
			bad = _("destination is beyond a symbolic link");
			اذهب act_on_اندراج;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		إذا (needs_worktree_rename(modes[i], dst_منوال)) {
			ثابت حرف *slash_ = strrchr(dst, '/');

			إذا (slash_) {
				بنية stat dir_st;
				حرف *dst_dir = xstrdup(dst);
				حرف *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				إذا (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					إذا (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} وإلا إذا (!S_ISDIR(dir_st.st_منوال)) {
					bad = _("destination is not a directory");
				}

				حرر(dst_dir);
			}

			إذا (bad)
				اذهب act_on_اندراج;
		}

		إذا (ignore_sparse &&
		    (dst_منوال & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    اشاریہ_اندراج_exists(the_repository->اشاریہ, dst, لڑی_لمبائی(dst))) {
			bad = _("destination exists in the index");
			إذا (force) {
				إذا (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = عدم;
			} وإلا {
				اذهب act_on_اندراج;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		إذا (!ignore_sparse &&
		    !رستہ_وچ_sparse_checkout(src, the_repository->اشاریہ)) {
			string_list_append(&only_میل_skip_worktree, src);
			skip_sparse = 1;
		}
		إذا (!ignore_sparse &&
		    !رستہ_وچ_sparse_checkout(dst, the_repository->اشاریہ)) {
			string_list_append(&only_میل_skip_worktree, dst);
			skip_sparse = 1;
		}

		إذا (skip_sparse)
			اذهب remove_اندراج;

		string_list_داخل_کرو(&src_for_dst, dst);

act_on_اندراج:
		إذا (!bad)
			استمر;
		إذا (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_اندراج:
		إذا (--argc > 0) {
			صحيح n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *slash_pos;

		إذا (modes[i] & MOVE_راہیں_ماپا_DIR)
			استمر;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		طالما (slash_pos > pathbuf.buf) {
			بنية pathmap_اندراج needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.رستہ = pathbuf.buf;
			hashmap_اندراج_init(&needle.ent, fspathhash(pathbuf.buf));

			إذا (hashmap_get_اندراج(&moved_dirs, &needle, ent, عدم))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	إذا (only_میل_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_میل_skip_worktree);
		إذا (!ignore_errors) {
			ret = 1;
			اذهب باہرلا;
		}
	}

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		تعداد update_منوال منوال = modes[i];
		صحيح pos;
		صحيح sparse_and_dirty = 0;
		بنية checkout state = CHECKOUT_INIT;
		state.istate = the_repository->اشاریہ;

		إذا (force)
			state.force = 1;
		إذا (show_only || verbose)
			لکھو(_("Renaming %s to %s\n"), src, dst);
		إذا (show_only)
			استمر;
		إذا (needs_worktree_rename(منوال, dst_منوال) &&
		    rename(src, dst) < 0) {
			إذا (ignore_errors)
				استمر;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		إذا (submodule_gitfiles[i]) {
			إذا (!update_رستہ_وچ_gitmodules(src, dst))
				gitmodules_modified = 1;
			إذا (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_درخت_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		إذا (منوال & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			استمر;

		pos = اشاریہ_name_pos(the_repository->اشاریہ, src, لڑی_لمبائی(src));
		إذا (pos < 0)
			BUG("could not find source in index: '%s'", src);
		إذا (!(منوال & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->اشاریہ,
						       the_repository->اشاریہ->cache[pos],
						       &st,
						       0);
		rename_اشاریہ_اندراج_at(the_repository->اشاریہ, pos, dst);

		إذا (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			إذا ((منوال & SPARSE) &&
			    رستہ_وچ_sparse_checkout(dst, the_repository->اشاریہ)) {
				/* from out-of-cone to in-cone */
				صحيح dst_pos = اشاریہ_name_pos(the_repository->اشاریہ, dst,
							     لڑی_لمبائی(dst));
				بنية cache_اندراج *dst_ce = the_repository->اشاریہ->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				إذا (checkout_اندراج(dst_ce, &state, عدم, عدم))
					die(_("cannot checkout %s"), dst_ce->name);
			} وإلا إذا ((dst_منوال & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(منوال & SPARSE) &&
				   !رستہ_وچ_sparse_checkout(dst, the_repository->اشاریہ)) {
				/* from in-cone to out-of-cone */
				صحيح dst_pos = اشاریہ_name_pos(the_repository->اشاریہ, dst,
							     لڑی_لمبائی(dst));
				بنية cache_اندراج *dst_ce = the_repository->اشاریہ->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				إذا (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} وإلا {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					حرف *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					محفوظ_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_خالی_تھاواں_src_dirs(src_dir.v, src_dir.nr);

	إذا (dirty_paths.nr)
		advise_on_moving_dirty_رستہ(&dirty_paths);

	إذا (gitmodules_modified)
		stage_updated_gitmodules(the_repository->اشاریہ);

	إذا (write_locked_اشاریہ(the_repository->اشاریہ, &جندرا_file,
			       COMMIT_جندرا | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

باہرلا:
	strvec_clear(&src_dir);
	حرر(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_میل_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_منزل_free);
	hashmap_clear_and_free(&moved_dirs, بنية pathmap_اندراج, ent);
	strbuf_release(&pathbuf);
	حرر(submodule_gitfiles);
	حرر(modes);
	أرجع ret;
}
