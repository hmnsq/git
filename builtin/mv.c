/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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

تعداد update_نما {
	WORKING_DIRECTORY = (1 << 1),
	شاخص_2 = (1 << 2),
	SPARSE = (1 << 3),
	SKIP_WORKTREE_DIR = (1 << 4),
	/*
	 * A file gets moved implicitly via a move of one of its parent
	 * directories. This flag causes us to skip the check that we don't try
	 * to move a file and any of its parent directories at the same point
	 * in time.
	 */
	MOVE_از_طریق_والد_DIR = (1 << 5),
};

ساكن صحيح needs_worktree_rename(تعداد update_نما نما, تعداد update_نما dst_نما)
{
	أرجع !(نما & (شاخص_2 | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_نما & (SKIP_WORKTREE_DIR | SPARSE));
}

#تعریف_کن DUP_BASENAME 1
#تعریف_کن KEEP_TRAILING_SLASH 2

ساكن فراغ داخلی_پیشوند_pathspec(بنية strvec *بیرون,
				     ثابت حرف *پیشوند,
				     ثابت حرف **pathspec,
				     صحيح شمار, بیعلامت flags)
{
	صحيح prefixlen = پیشوند ? طول_رشته(پیشوند) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	لكل (صحيح i = 0; i < شمار; i++) {
		اندازه_t درازی = طول_رشته(pathspec[i]);
		اندازه_t مقصد_copy = درازی;
		ثابت حرف *maybe_basename;
		حرف *trimmed, *prefixed_مسیر;

		طالما (!(flags & KEEP_TRAILING_SLASH) &&
		       مقصد_copy > 0 && is_dir_sep(pathspec[i][مقصد_copy - 1]))
			مقصد_copy--;

		trimmed = xmemdupz(pathspec[i], مقصد_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_مسیر = پیشوند_مسیر(the_repository, پیشوند, prefixlen, maybe_basename);
		strvec_افزودن(بیرون, prefixed_مسیر);

		حرر(prefixed_مسیر);
		حرر(trimmed);
	}
}

ساكن حرف *جمع_کن_slash(ثابت حرف *مسیر)
{
	اندازه_t len = طول_رشته(مسیر);
	إذا (len && مسیر[len - 1] != '/') {
		حرف *with_slash = xmalloc(st_جمع_کن(len, 2));
		کپی_حافظه(with_slash, مسیر, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		أرجع with_slash;
	}
	أرجع xstrdup(مسیر);
}

#تعریف_کن SUBMODULE_WITH_GITDIR ((ثابت حرف *)1)

ساكن ثابت حرف *submodule_gitfile_مسیر(ثابت حرف *src, صحيح اولی)
{
	بنية strbuf submodule_dotgit = STRBUF_INIT;
	ثابت حرف *مسیر;

	إذا (!S_ISGITLINK(the_repository->شاخص->cache[اولی]->ce_نما))
		die(_("Directory %s is in index and no submodule?"), src);
	إذا (!is_staging_gitmodules_ok(the_repository->شاخص))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	مسیر = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	إذا (مسیر)
		أرجع مسیر;
	أرجع SUBMODULE_WITH_GITDIR;
}

ساكن صحيح شاخص_range_از_same_dir(ثابت حرف *src, صحيح درازی,
				   صحيح *اولی_p, صحيح *آخرین_p)
{
	حرف *src_w_slash = جمع_کن_slash(src);
	صحيح اولی, آخرین, len_w_slash = درازی + 1;

	اولی = شاخص_name_pos(the_repository->شاخص, src_w_slash, len_w_slash);
	إذا (اولی >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	اولی = -1 - اولی;
	لكل (آخرین = اولی; آخرین < the_repository->شاخص->cache_nr; آخرین++) {
		ثابت حرف *مسیر = the_repository->شاخص->cache[آخرین]->name;
		إذا (مقایسه_رشته(مسیر, src_w_slash, len_w_slash))
			اكسر;
	}

	حرر(src_w_slash);
	*اولی_p = اولی;
	*آخرین_p = آخرین;
	أرجع آخرین - اولی;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
ساكن صحيح خالی_dir_has_sparse_contents(ثابت حرف *name)
{
	صحيح ret = 0;
	حرف *with_slash = جمع_کن_slash(name);
	صحيح درازی = طول_رشته(with_slash);

	صحيح pos = شاخص_name_pos(the_repository->شاخص, with_slash, درازی);
	ثابت بنية cache_مدخل *ce;

	إذا (pos < 0) {
		pos = -pos - 1;
		إذا (pos >= the_repository->شاخص->cache_nr)
			اذهب free_return;
		ce = the_repository->شاخص->cache[pos];
		إذا (مقایسه_رشته(with_slash, ce->name, درازی))
			اذهب free_return;
		إذا (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	حرر(with_slash);
	أرجع ret;
}

ساكن فراغ remove_خالی_src_dirs(ثابت حرف **src_dir, اندازه_t src_dir_nr)
{
	اندازه_t i;
	بنية strbuf a_src_dir = STRBUF_INIT;

	لكل (i = 0; i < src_dir_nr; i++) {
		صحيح dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		إذا (شاخص_range_از_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

بنية pathmap_مدخل {
	بنية hashmap_مدخل ent;
	ثابت حرف *مسیر;
};

ساكن صحيح pathmap_cmp(ثابت فراغ *cmp_داده استفادهنشده,
		       ثابت بنية hashmap_مدخل *a,
		       ثابت بنية hashmap_مدخل *b,
		       ثابت فراغ *کلید استفادهنشده)
{
	ثابت بنية pathmap_مدخل *e1 = container_از(a, بنية pathmap_مدخل, ent);
	ثابت بنية pathmap_مدخل *e2 = container_از(b, بنية pathmap_مدخل, ent);
	أرجع fspathcmp(e1->مسیر, e2->مسیر);
}

صحيح cmd_mv(صحيح argc,
	   ثابت حرف **argv,
	   ثابت حرف *پیشوند,
	   بنية repository *repo استفادهنشده)
{
	صحيح i, flags, gitmodules_modified = 0;
	صحيح verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	بنية option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_DRY_پیاپی(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', عدم, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_انتها(),
	};
	بنية strvec sources = STRVEC_INIT;
	بنية strvec dest_paths = STRVEC_INIT;
	بنية strvec destinations = STRVEC_INIT;
	بنية strvec submodule_gitfiles_مقصد_free = STRVEC_INIT;
	ثابت حرف **submodule_gitfiles;
	حرف *dst_w_slash = عدم;
	بنية strvec src_dir = STRVEC_INIT;
	تعداد update_نما *modes, dst_نما = 0;
	بنية stat st, dest_st;
	بنية string_list src_for_dst = STRING_LIST_INIT_DUP;
	بنية قفل_file قفل_file = قفل_INIT;
	بنية cache_مدخل *ce;
	بنية string_list only_تطابق_skip_worktree = STRING_LIST_INIT_DUP;
	بنية string_list dirty_paths = STRING_LIST_INIT_DUP;
	بنية hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, عدم);
	بنية strbuf pathbuf = STRBUF_INIT;
	صحيح ret;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, پیشوند, builtin_mv_options,
			     builtin_mv_usage, 0);
	إذا (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_شاخص(the_repository, &قفل_file, قفل_DIE_ON_ERROR);
	إذا (repo_read_شاخص(the_repository) < 0)
		die(_("index file corrupt"));

	داخلی_پیشوند_pathspec(&sources, پیشوند, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	إذا (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	داخلی_پیشوند_pathspec(&dest_paths, پیشوند, argv + argc, 1, flags);
	dst_w_slash = جمع_کن_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, حجم(حرف *));

	إذا (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		داخلی_پیشوند_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	وإلا إذا (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_نما)) {
		داخلی_پیشوند_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} وإلا إذا (!مسیر_درون_sparse_checkout(dst_w_slash, the_repository->شاخص) &&
		   خالی_dir_has_sparse_contents(dst_w_slash)) {
		داخلی_پیشوند_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_نما = SKIP_WORKTREE_DIR;
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
		إذا (!مسیر_درون_cone_نما_sparse_checkout(destinations.v[0], the_repository->شاخص))
			dst_نما = SPARSE;
	}

	/* Checking */
	لكل (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		صحيح درازی;
		ثابت حرف *bad = عدم;
		صحيح skip_sparse = 0;

		إذا (show_only)
			چاپ_کن(_("Checking rename of '%s' to '%s'\n"), src, dst);

		درازی = طول_رشته(src);
		إذا (lstat(src, &st) < 0) {
			صحيح pos;
			ثابت بنية cache_مدخل *ce;

			pos = شاخص_name_pos(the_repository->شاخص, src, درازی);
			إذا (pos < 0) {
				حرف *src_w_slash = جمع_کن_slash(src);
				إذا (!مسیر_درون_sparse_checkout(src_w_slash, the_repository->شاخص) &&
				    خالی_dir_has_sparse_contents(src)) {
					حرر(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					اذهب dir_check;
				}
				حرر(src_w_slash);
				/* only error if existence is expected. */
				إذا (!(modes[i] & SPARSE))
					bad = _("bad source");
				اذهب act_on_مدخل;
			}
			ce = the_repository->شاخص->cache[pos];
			إذا (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				اذهب act_on_مدخل;
			}
			إذا (!ignore_sparse) {
				string_list_append(&only_تطابق_skip_worktree, src);
				اذهب act_on_مدخل;
			}
			/* Check if dst exists in index */
			إذا (شاخص_name_pos(the_repository->شاخص, dst, طول_رشته(dst)) < 0) {
				modes[i] |= SPARSE;
				اذهب act_on_مدخل;
			}
			إذا (!force) {
				bad = _("destination exists");
				اذهب act_on_مدخل;
			}
			modes[i] |= SPARSE;
			اذهب act_on_مدخل;
		}
		إذا (!مقایسه_رشته(src, dst, درازی) &&
		    (dst[درازی] == 0 || dst[درازی] == '/')) {
			bad = _("can not move directory into itself");
			اذهب act_on_مدخل;
		}
		إذا (S_ISDIR(st.st_نما)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			اذهب act_on_مدخل;
		}

dir_check:
		إذا (S_ISDIR(st.st_نما)) {
			بنية pathmap_مدخل *مدخل;
			حرف *dst_with_slash;
			اندازه_t dst_with_slash_len;
			صحيح j, n;
			صحيح اولی = شاخص_name_pos(the_repository->شاخص, src, درازی), آخرین;

			إذا (اولی >= 0) {
				ثابت حرف *مسیر = submodule_gitfile_مسیر(src, اولی);
				إذا (مسیر != SUBMODULE_WITH_GITDIR)
					مسیر = strvec_افزودن(&submodule_gitfiles_مقصد_free, مسیر);
				submodule_gitfiles[i] = مسیر;
				اذهب act_on_مدخل;
			} وإلا إذا (شاخص_range_از_same_dir(src, درازی,
							   &اولی, &آخرین) < 1) {
				bad = _("source directory is empty");
				اذهب act_on_مدخل;
			}

			مدخل = xmalloc(حجم(*مدخل));
			مدخل->مسیر = src;
			hashmap_مدخل_init(&مدخل->ent, fspathhash(src));
			hashmap_جمع_کن(&moved_dirs, &مدخل->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_افزودن(&src_dir, src);

			n = argc + آخرین - اولی;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = جمع_کن_slash(dst);
			dst_with_slash_len = طول_رشته(dst_with_slash);

			لكل (j = 0; j < آخرین - اولی; j++) {
				ثابت بنية cache_مدخل *ce = the_repository->شاخص->cache[اولی + j];
				ثابت حرف *مسیر = ce->name;
				حرف *prefixed_مسیر = پیشوند_مسیر(the_repository, dst_with_slash,
								  dst_with_slash_len, مسیر + درازی + 1);

				strvec_افزودن(&sources, مسیر);
				strvec_افزودن(&destinations, prefixed_مسیر);

				modes[argc + j] = MOVE_از_طریق_والد_DIR | (ce_skip_worktree(ce) ? SPARSE : شاخص_2);
				submodule_gitfiles[argc + j] = عدم;

				حرر(prefixed_مسیر);
			}

			حرر(dst_with_slash);
			argc += آخرین - اولی;
			اذهب act_on_مدخل;
		}
		إذا (!(ce = شاخص_file_exists(the_repository->شاخص, src, درازی, 0))) {
			bad = _("not under version control");
			اذهب act_on_مدخل;
		}
		إذا (ce_stage(ce)) {
			bad = _("conflicted");
			اذهب act_on_مدخل;
		}
		إذا (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			إذا (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				إذا (S_ISREG(st.st_نما) || S_ISLNK(st.st_نما)) {
					إذا (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = عدم;
				} وإلا
					bad = _("Cannot overwrite");
			}
			اذهب act_on_مدخل;
		}
		إذا (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			اذهب act_on_مدخل;
		}
		إذا (is_dir_sep(dst[طول_رشته(dst) - 1])) {
			bad = _("destination directory does not exist");
			اذهب act_on_مدخل;
		}
		إذا (has_symlink_leading_مسیر(dst, طول_رشته(dst))) {
			bad = _("destination is beyond a symbolic link");
			اذهب act_on_مدخل;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		إذا (needs_worktree_rename(modes[i], dst_نما)) {
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
				} وإلا إذا (!S_ISDIR(dir_st.st_نما)) {
					bad = _("destination is not a directory");
				}

				حرر(dst_dir);
			}

			إذا (bad)
				اذهب act_on_مدخل;
		}

		إذا (ignore_sparse &&
		    (dst_نما & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    شاخص_مدخل_exists(the_repository->شاخص, dst, طول_رشته(dst))) {
			bad = _("destination exists in the index");
			إذا (force) {
				إذا (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = عدم;
			} وإلا {
				اذهب act_on_مدخل;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		إذا (!ignore_sparse &&
		    !مسیر_درون_sparse_checkout(src, the_repository->شاخص)) {
			string_list_append(&only_تطابق_skip_worktree, src);
			skip_sparse = 1;
		}
		إذا (!ignore_sparse &&
		    !مسیر_درون_sparse_checkout(dst, the_repository->شاخص)) {
			string_list_append(&only_تطابق_skip_worktree, dst);
			skip_sparse = 1;
		}

		إذا (skip_sparse)
			اذهب remove_مدخل;

		string_list_درج(&src_for_dst, dst);

act_on_مدخل:
		إذا (!bad)
			استمر;
		إذا (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_مدخل:
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

		إذا (modes[i] & MOVE_از_طریق_والد_DIR)
			استمر;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		طالما (slash_pos > pathbuf.buf) {
			بنية pathmap_مدخل needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.مسیر = pathbuf.buf;
			hashmap_مدخل_init(&needle.ent, fspathhash(pathbuf.buf));

			إذا (hashmap_get_مدخل(&moved_dirs, &needle, ent, عدم))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	إذا (only_تطابق_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_تطابق_skip_worktree);
		إذا (!ignore_errors) {
			ret = 1;
			اذهب بیرون;
		}
	}

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		تعداد update_نما نما = modes[i];
		صحيح pos;
		صحيح sparse_and_dirty = 0;
		بنية checkout state = CHECKOUT_INIT;
		state.istate = the_repository->شاخص;

		إذا (force)
			state.force = 1;
		إذا (show_only || verbose)
			چاپ_کن(_("Renaming %s to %s\n"), src, dst);
		إذا (show_only)
			استمر;
		إذا (needs_worktree_rename(نما, dst_نما) &&
		    rename(src, dst) < 0) {
			إذا (ignore_errors)
				استمر;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		إذا (submodule_gitfiles[i]) {
			إذا (!update_مسیر_درون_gitmodules(src, dst))
				gitmodules_modified = 1;
			إذا (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_درخت_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		إذا (نما & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			استمر;

		pos = شاخص_name_pos(the_repository->شاخص, src, طول_رشته(src));
		إذا (pos < 0)
			BUG("could not find source in index: '%s'", src);
		إذا (!(نما & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->شاخص,
						       the_repository->شاخص->cache[pos],
						       &st,
						       0);
		rename_شاخص_مدخل_at(the_repository->شاخص, pos, dst);

		إذا (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			إذا ((نما & SPARSE) &&
			    مسیر_درون_sparse_checkout(dst, the_repository->شاخص)) {
				/* from out-of-cone to in-cone */
				صحيح dst_pos = شاخص_name_pos(the_repository->شاخص, dst,
							     طول_رشته(dst));
				بنية cache_مدخل *dst_ce = the_repository->شاخص->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				إذا (checkout_مدخل(dst_ce, &state, عدم, عدم))
					die(_("cannot checkout %s"), dst_ce->name);
			} وإلا إذا ((dst_نما & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(نما & SPARSE) &&
				   !مسیر_درون_sparse_checkout(dst, the_repository->شاخص)) {
				/* from in-cone to out-of-cone */
				صحيح dst_pos = شاخص_name_pos(the_repository->شاخص, dst,
							     طول_رشته(dst));
				بنية cache_مدخل *dst_ce = the_repository->شاخص->cache[dst_pos];

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
					امن_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_خالی_src_dirs(src_dir.v, src_dir.nr);

	إذا (dirty_paths.nr)
		advise_on_moving_dirty_مسیر(&dirty_paths);

	إذا (gitmodules_modified)
		stage_updated_gitmodules(the_repository->شاخص);

	إذا (write_locked_شاخص(the_repository->شاخص, &قفل_file,
			       COMMIT_قفل | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

بیرون:
	strvec_clear(&src_dir);
	حرر(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_تطابق_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_مقصد_free);
	hashmap_clear_and_free(&moved_dirs, بنية pathmap_مدخل, ent);
	strbuf_release(&pathbuf);
	حرر(submodule_gitfiles);
	حرر(modes);
	أرجع ret;
}
