/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساکن ثابت حرف * ثابت builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	تہی
};

شمار update_منوال {
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
	MOVE_بذریعہ_والد_DIR = (1 << 5),
};

ساکن صحیح needs_worktree_rename(شمار update_منوال منوال, شمار update_منوال dst_منوال)
{
	واپس !(منوال & (اشاریہ_2 | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_منوال & (SKIP_WORKTREE_DIR | SPARSE));
}

#تعریف_کرو DUP_BASENAME 1
#تعریف_کرو KEEP_TRAILING_SLASH 2

ساکن خالی اندرونی_سابقہ_pathspec(ساخت strvec *برآمد,
				     ثابت حرف *سابقہ,
				     ثابت حرف **pathspec,
				     صحیح گنتی, غیرعلامتی flags)
{
	صحیح prefixlen = سابقہ ? لڑی_لمبائی(سابقہ) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	برائے (صحیح i = 0; i < گنتی; i++) {
		سائز_t لمبائی = لڑی_لمبائی(pathspec[i]);
		سائز_t مقصد_copy = لمبائی;
		ثابت حرف *maybe_basename;
		حرف *trimmed, *prefixed_راستہ;

		جب (!(flags & KEEP_TRAILING_SLASH) &&
		       مقصد_copy > 0 && is_dir_sep(pathspec[i][مقصد_copy - 1]))
			مقصد_copy--;

		trimmed = xmemdupz(pathspec[i], مقصد_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_راستہ = سابقہ_راستہ(the_repository, سابقہ, prefixlen, maybe_basename);
		strvec_ڈالو(برآمد, prefixed_راستہ);

		آزاد(prefixed_راستہ);
		آزاد(trimmed);
	}
}

ساکن حرف *جوڑو_slash(ثابت حرف *راستہ)
{
	سائز_t len = لڑی_لمبائی(راستہ);
	اگر (len && راستہ[len - 1] != '/') {
		حرف *with_slash = xmalloc(st_جوڑو(len, 2));
		حافظہ_نقل(with_slash, راستہ, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		واپس with_slash;
	}
	واپس xstrdup(راستہ);
}

#تعریف_کرو SUBMODULE_WITH_GITDIR ((ثابت حرف *)1)

ساکن ثابت حرف *submodule_gitfile_راستہ(ثابت حرف *src, صحیح پہلا)
{
	ساخت strbuf submodule_dotgit = STRBUF_INIT;
	ثابت حرف *راستہ;

	اگر (!S_ISGITLINK(the_repository->اشاریہ->cache[پہلا]->ce_منوال))
		die(_("Directory %s is in index and no submodule?"), src);
	اگر (!is_staging_gitmodules_ok(the_repository->اشاریہ))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	راستہ = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	اگر (راستہ)
		واپس راستہ;
	واپس SUBMODULE_WITH_GITDIR;
}

ساکن صحیح اشاریہ_range_کا_same_dir(ثابت حرف *src, صحیح لمبائی,
				   صحیح *پہلا_p, صحیح *آخری_p)
{
	حرف *src_w_slash = جوڑو_slash(src);
	صحیح پہلا, آخری, len_w_slash = لمبائی + 1;

	پہلا = اشاریہ_name_pos(the_repository->اشاریہ, src_w_slash, len_w_slash);
	اگر (پہلا >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	پہلا = -1 - پہلا;
	برائے (آخری = پہلا; آخری < the_repository->اشاریہ->cache_nr; آخری++) {
		ثابت حرف *راستہ = the_repository->اشاریہ->cache[آخری]->name;
		اگر (لڑی_موازنہ(راستہ, src_w_slash, len_w_slash))
			توڑو;
	}

	آزاد(src_w_slash);
	*پہلا_p = پہلا;
	*آخری_p = آخری;
	واپس آخری - پہلا;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
ساکن صحیح خالی_خانے_dir_has_sparse_contents(ثابت حرف *name)
{
	صحیح ret = 0;
	حرف *with_slash = جوڑو_slash(name);
	صحیح لمبائی = لڑی_لمبائی(with_slash);

	صحیح pos = اشاریہ_name_pos(the_repository->اشاریہ, with_slash, لمبائی);
	ثابت ساخت cache_اندراج *ce;

	اگر (pos < 0) {
		pos = -pos - 1;
		اگر (pos >= the_repository->اشاریہ->cache_nr)
			جاؤ free_return;
		ce = the_repository->اشاریہ->cache[pos];
		اگر (لڑی_موازنہ(with_slash, ce->name, لمبائی))
			جاؤ free_return;
		اگر (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	آزاد(with_slash);
	واپس ret;
}

ساکن خالی remove_خالی_خانے_src_dirs(ثابت حرف **src_dir, سائز_t src_dir_nr)
{
	سائز_t i;
	ساخت strbuf a_src_dir = STRBUF_INIT;

	برائے (i = 0; i < src_dir_nr; i++) {
		صحیح dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		اگر (اشاریہ_range_کا_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

ساخت pathmap_اندراج {
	ساخت hashmap_اندراج ent;
	ثابت حرف *راستہ;
};

ساکن صحیح pathmap_cmp(ثابت خالی *cmp_ڈیٹا غیر_مستعمل,
		       ثابت ساخت hashmap_اندراج *a,
		       ثابت ساخت hashmap_اندراج *b,
		       ثابت خالی *کلید غیر_مستعمل)
{
	ثابت ساخت pathmap_اندراج *e1 = container_کا(a, ساخت pathmap_اندراج, ent);
	ثابت ساخت pathmap_اندراج *e2 = container_کا(b, ساخت pathmap_اندراج, ent);
	واپس fspathcmp(e1->راستہ, e2->راستہ);
}

صحیح cmd_mv(صحیح argc,
	   ثابت حرف **argv,
	   ثابت حرف *سابقہ,
	   ساخت repository *repo غیر_مستعمل)
{
	صحیح i, flags, gitmodules_modified = 0;
	صحیح verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	ساخت option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_DRY_تسلسل(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', تہی, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_انتہا(),
	};
	ساخت strvec sources = STRVEC_INIT;
	ساخت strvec dest_paths = STRVEC_INIT;
	ساخت strvec destinations = STRVEC_INIT;
	ساخت strvec submodule_gitfiles_مقصد_free = STRVEC_INIT;
	ثابت حرف **submodule_gitfiles;
	حرف *dst_w_slash = تہی;
	ساخت strvec src_dir = STRVEC_INIT;
	شمار update_منوال *modes, dst_منوال = 0;
	ساخت stat st, dest_st;
	ساخت string_list src_for_dst = STRING_LIST_INIT_DUP;
	ساخت تالا_file تالا_file = تالا_INIT;
	ساخت cache_اندراج *ce;
	ساخت string_list only_مطابقت_skip_worktree = STRING_LIST_INIT_DUP;
	ساخت string_list dirty_paths = STRING_LIST_INIT_DUP;
	ساخت hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, تہی);
	ساخت strbuf pathbuf = STRBUF_INIT;
	صحیح ret;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	repo_config(the_repository, git_default_config, تہی);

	argc = parse_options(argc, argv, سابقہ, builtin_mv_options,
			     builtin_mv_usage, 0);
	اگر (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_اشاریہ(the_repository, &تالا_file, تالا_DIE_ON_ERROR);
	اگر (repo_read_اشاریہ(the_repository) < 0)
		die(_("index file corrupt"));

	اندرونی_سابقہ_pathspec(&sources, سابقہ, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	اگر (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	اندرونی_سابقہ_pathspec(&dest_paths, سابقہ, argv + argc, 1, flags);
	dst_w_slash = جوڑو_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, حجم(حرف *));

	اگر (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		اندرونی_سابقہ_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	ورنہ اگر (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_منوال)) {
		اندرونی_سابقہ_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} ورنہ اگر (!راستہ_اندر_sparse_checkout(dst_w_slash, the_repository->اشاریہ) &&
		   خالی_خانے_dir_has_sparse_contents(dst_w_slash)) {
		اندرونی_سابقہ_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_منوال = SKIP_WORKTREE_DIR;
	} ورنہ اگر (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} ورنہ {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		اگر (!راستہ_اندر_cone_منوال_sparse_checkout(destinations.v[0], the_repository->اشاریہ))
			dst_منوال = SPARSE;
	}

	/* Checking */
	برائے (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		صحیح لمبائی;
		ثابت حرف *bad = تہی;
		صحیح skip_sparse = 0;

		اگر (show_only)
			لکھو(_("Checking rename of '%s' to '%s'\n"), src, dst);

		لمبائی = لڑی_لمبائی(src);
		اگر (lstat(src, &st) < 0) {
			صحیح pos;
			ثابت ساخت cache_اندراج *ce;

			pos = اشاریہ_name_pos(the_repository->اشاریہ, src, لمبائی);
			اگر (pos < 0) {
				حرف *src_w_slash = جوڑو_slash(src);
				اگر (!راستہ_اندر_sparse_checkout(src_w_slash, the_repository->اشاریہ) &&
				    خالی_خانے_dir_has_sparse_contents(src)) {
					آزاد(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					جاؤ dir_check;
				}
				آزاد(src_w_slash);
				/* only error if existence is expected. */
				اگر (!(modes[i] & SPARSE))
					bad = _("bad source");
				جاؤ act_on_اندراج;
			}
			ce = the_repository->اشاریہ->cache[pos];
			اگر (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				جاؤ act_on_اندراج;
			}
			اگر (!ignore_sparse) {
				string_list_append(&only_مطابقت_skip_worktree, src);
				جاؤ act_on_اندراج;
			}
			/* Check if dst exists in index */
			اگر (اشاریہ_name_pos(the_repository->اشاریہ, dst, لڑی_لمبائی(dst)) < 0) {
				modes[i] |= SPARSE;
				جاؤ act_on_اندراج;
			}
			اگر (!force) {
				bad = _("destination exists");
				جاؤ act_on_اندراج;
			}
			modes[i] |= SPARSE;
			جاؤ act_on_اندراج;
		}
		اگر (!لڑی_موازنہ(src, dst, لمبائی) &&
		    (dst[لمبائی] == 0 || dst[لمبائی] == '/')) {
			bad = _("can not move directory into itself");
			جاؤ act_on_اندراج;
		}
		اگر (S_ISDIR(st.st_منوال)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			جاؤ act_on_اندراج;
		}

dir_check:
		اگر (S_ISDIR(st.st_منوال)) {
			ساخت pathmap_اندراج *اندراج_2;
			حرف *dst_with_slash;
			سائز_t dst_with_slash_len;
			صحیح j, n;
			صحیح پہلا = اشاریہ_name_pos(the_repository->اشاریہ, src, لمبائی), آخری;

			اگر (پہلا >= 0) {
				ثابت حرف *راستہ = submodule_gitfile_راستہ(src, پہلا);
				اگر (راستہ != SUBMODULE_WITH_GITDIR)
					راستہ = strvec_ڈالو(&submodule_gitfiles_مقصد_free, راستہ);
				submodule_gitfiles[i] = راستہ;
				جاؤ act_on_اندراج;
			} ورنہ اگر (اشاریہ_range_کا_same_dir(src, لمبائی,
							   &پہلا, &آخری) < 1) {
				bad = _("source directory is empty");
				جاؤ act_on_اندراج;
			}

			اندراج_2 = xmalloc(حجم(*اندراج_2));
			اندراج_2->راستہ = src;
			hashmap_اندراج_init(&اندراج_2->ent, fspathhash(src));
			hashmap_جوڑو(&moved_dirs, &اندراج_2->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_ڈالو(&src_dir, src);

			n = argc + آخری - پہلا;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = جوڑو_slash(dst);
			dst_with_slash_len = لڑی_لمبائی(dst_with_slash);

			برائے (j = 0; j < آخری - پہلا; j++) {
				ثابت ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[پہلا + j];
				ثابت حرف *راستہ = ce->name;
				حرف *prefixed_راستہ = سابقہ_راستہ(the_repository, dst_with_slash,
								  dst_with_slash_len, راستہ + لمبائی + 1);

				strvec_ڈالو(&sources, راستہ);
				strvec_ڈالو(&destinations, prefixed_راستہ);

				modes[argc + j] = MOVE_بذریعہ_والد_DIR | (ce_skip_worktree(ce) ? SPARSE : اشاریہ_2);
				submodule_gitfiles[argc + j] = تہی;

				آزاد(prefixed_راستہ);
			}

			آزاد(dst_with_slash);
			argc += آخری - پہلا;
			جاؤ act_on_اندراج;
		}
		اگر (!(ce = اشاریہ_file_exists(the_repository->اشاریہ, src, لمبائی, 0))) {
			bad = _("not under version control");
			جاؤ act_on_اندراج;
		}
		اگر (ce_stage(ce)) {
			bad = _("conflicted");
			جاؤ act_on_اندراج;
		}
		اگر (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			اگر (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				اگر (S_ISREG(st.st_منوال) || S_ISLNK(st.st_منوال)) {
					اگر (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = تہی;
				} ورنہ
					bad = _("Cannot overwrite");
			}
			جاؤ act_on_اندراج;
		}
		اگر (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			جاؤ act_on_اندراج;
		}
		اگر (is_dir_sep(dst[لڑی_لمبائی(dst) - 1])) {
			bad = _("destination directory does not exist");
			جاؤ act_on_اندراج;
		}
		اگر (has_symlink_leading_راستہ(dst, لڑی_لمبائی(dst))) {
			bad = _("destination is beyond a symbolic link");
			جاؤ act_on_اندراج;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		اگر (needs_worktree_rename(modes[i], dst_منوال)) {
			ثابت حرف *slash_ = strrchr(dst, '/');

			اگر (slash_) {
				ساخت stat dir_st;
				حرف *dst_dir = xstrdup(dst);
				حرف *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				اگر (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					اگر (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} ورنہ اگر (!S_ISDIR(dir_st.st_منوال)) {
					bad = _("destination is not a directory");
				}

				آزاد(dst_dir);
			}

			اگر (bad)
				جاؤ act_on_اندراج;
		}

		اگر (ignore_sparse &&
		    (dst_منوال & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    اشاریہ_اندراج_exists(the_repository->اشاریہ, dst, لڑی_لمبائی(dst))) {
			bad = _("destination exists in the index");
			اگر (force) {
				اگر (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = تہی;
			} ورنہ {
				جاؤ act_on_اندراج;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		اگر (!ignore_sparse &&
		    !راستہ_اندر_sparse_checkout(src, the_repository->اشاریہ)) {
			string_list_append(&only_مطابقت_skip_worktree, src);
			skip_sparse = 1;
		}
		اگر (!ignore_sparse &&
		    !راستہ_اندر_sparse_checkout(dst, the_repository->اشاریہ)) {
			string_list_append(&only_مطابقت_skip_worktree, dst);
			skip_sparse = 1;
		}

		اگر (skip_sparse)
			جاؤ remove_اندراج;

		string_list_داخل_کرو(&src_for_dst, dst);

act_on_اندراج:
		اگر (!bad)
			جاری;
		اگر (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_اندراج:
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

	برائے (i = 0; i < argc; i++) {
		ثابت حرف *slash_pos;

		اگر (modes[i] & MOVE_بذریعہ_والد_DIR)
			جاری;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		جب (slash_pos > pathbuf.buf) {
			ساخت pathmap_اندراج needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.راستہ = pathbuf.buf;
			hashmap_اندراج_init(&needle.ent, fspathhash(pathbuf.buf));

			اگر (hashmap_get_اندراج(&moved_dirs, &needle, ent, تہی))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	اگر (only_مطابقت_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_مطابقت_skip_worktree);
		اگر (!ignore_errors) {
			ret = 1;
			جاؤ برآمد;
		}
	}

	برائے (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		شمار update_منوال منوال = modes[i];
		صحیح pos;
		صحیح sparse_and_dirty = 0;
		ساخت checkout state = CHECKOUT_INIT;
		state.istate = the_repository->اشاریہ;

		اگر (force)
			state.force = 1;
		اگر (show_only || verbose)
			لکھو(_("Renaming %s to %s\n"), src, dst);
		اگر (show_only)
			جاری;
		اگر (needs_worktree_rename(منوال, dst_منوال) &&
		    rename(src, dst) < 0) {
			اگر (ignore_errors)
				جاری;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		اگر (submodule_gitfiles[i]) {
			اگر (!update_راستہ_اندر_gitmodules(src, dst))
				gitmodules_modified = 1;
			اگر (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_درخت_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		اگر (منوال & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			جاری;

		pos = اشاریہ_name_pos(the_repository->اشاریہ, src, لڑی_لمبائی(src));
		اگر (pos < 0)
			BUG("could not find source in index: '%s'", src);
		اگر (!(منوال & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->اشاریہ,
						       the_repository->اشاریہ->cache[pos],
						       &st,
						       0);
		rename_اشاریہ_اندراج_at(the_repository->اشاریہ, pos, dst);

		اگر (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			اگر ((منوال & SPARSE) &&
			    راستہ_اندر_sparse_checkout(dst, the_repository->اشاریہ)) {
				/* from out-of-cone to in-cone */
				صحیح dst_pos = اشاریہ_name_pos(the_repository->اشاریہ, dst,
							     لڑی_لمبائی(dst));
				ساخت cache_اندراج *dst_ce = the_repository->اشاریہ->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				اگر (checkout_اندراج(dst_ce, &state, تہی, تہی))
					die(_("cannot checkout %s"), dst_ce->name);
			} ورنہ اگر ((dst_منوال & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(منوال & SPARSE) &&
				   !راستہ_اندر_sparse_checkout(dst, the_repository->اشاریہ)) {
				/* from in-cone to out-of-cone */
				صحیح dst_pos = اشاریہ_name_pos(the_repository->اشاریہ, dst,
							     لڑی_لمبائی(dst));
				ساخت cache_اندراج *dst_ce = the_repository->اشاریہ->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				اگر (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} ورنہ {
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

	remove_خالی_خانے_src_dirs(src_dir.v, src_dir.nr);

	اگر (dirty_paths.nr)
		advise_on_moving_dirty_راستہ(&dirty_paths);

	اگر (gitmodules_modified)
		stage_updated_gitmodules(the_repository->اشاریہ);

	اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا_file,
			       COMMIT_تالا | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

برآمد:
	strvec_clear(&src_dir);
	آزاد(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_مطابقت_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_مقصد_free);
	hashmap_clear_and_free(&moved_dirs, ساخت pathmap_اندراج, ent);
	strbuf_release(&pathbuf);
	آزاد(submodule_gitfiles);
	آزاد(modes);
	واپس ret;
}
