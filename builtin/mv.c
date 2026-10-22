/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

تعداد update_گهڻائي {
	WORKING_DIRECTORY = (1 << 1),
	اشاريو_2 = (1 << 2),
	SPARSE = (1 << 3),
	SKIP_WORKTREE_DIR = (1 << 4),
	/*
	 * A file gets moved implicitly via a move of one of its parent
	 * directories. This flag causes us to skip the check that we don't try
	 * to move a file and any of its parent directories at the same point
	 * in time.
	 */
	MOVE_ذريعي_والد_DIR = (1 << 5),
};

ساكن صحيح needs_worktree_rename(تعداد update_گهڻائي گهڻائي, تعداد update_گهڻائي dst_گهڻائي)
{
	أرجع !(گهڻائي & (اشاريو_2 | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_گهڻائي & (SKIP_WORKTREE_DIR | SPARSE));
}

#وصف_ڪريو DUP_BASENAME 1
#وصف_ڪريو KEEP_TRAILING_SLASH 2

ساكن فراغ اندروني_اڳياڙي_pathspec(بنية strvec *ٻاهريون,
				     ثابت حرف *اڳياڙي,
				     ثابت حرف **pathspec,
				     صحيح ڳڻپ, بنانشان flags)
{
	صحيح prefixlen = اڳياڙي ? لڙي_ڊيگهه(اڳياڙي) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	لكل (صحيح i = 0; i < ڳڻپ; i++) {
		سائيز_t ڊيگهائي = لڙي_ڊيگهه(pathspec[i]);
		سائيز_t منزل_copy = ڊيگهائي;
		ثابت حرف *maybe_basename;
		حرف *trimmed, *prefixed_رستو;

		طالما (!(flags & KEEP_TRAILING_SLASH) &&
		       منزل_copy > 0 && is_dir_sep(pathspec[i][منزل_copy - 1]))
			منزل_copy--;

		trimmed = xmemdupz(pathspec[i], منزل_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_رستو = اڳياڙي_رستو(the_repository, اڳياڙي, prefixlen, maybe_basename);
		strvec_ڌڪو(ٻاهريون, prefixed_رستو);

		حرر(prefixed_رستو);
		حرر(trimmed);
	}
}

ساكن حرف *جوڙيو_slash(ثابت حرف *رستو)
{
	سائيز_t len = لڙي_ڊيگهه(رستو);
	إذا (len && رستو[len - 1] != '/') {
		حرف *with_slash = xmalloc(st_جوڙيو(len, 2));
		يادگيري_نقل(with_slash, رستو, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		أرجع with_slash;
	}
	أرجع xstrdup(رستو);
}

#وصف_ڪريو SUBMODULE_WITH_GITDIR ((ثابت حرف *)1)

ساكن ثابت حرف *submodule_gitfile_رستو(ثابت حرف *src, صحيح پهريون)
{
	بنية strbuf submodule_dotgit = STRBUF_INIT;
	ثابت حرف *رستو;

	إذا (!S_ISGITLINK(the_repository->اشاريو->cache[پهريون]->ce_گهڻائي))
		die(_("Directory %s is in index and no submodule?"), src);
	إذا (!is_staging_gitmodules_ok(the_repository->اشاريو))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	رستو = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	إذا (رستو)
		أرجع رستو;
	أرجع SUBMODULE_WITH_GITDIR;
}

ساكن صحيح اشاريو_range_جو_same_dir(ثابت حرف *src, صحيح ڊيگهائي,
				   صحيح *پهريون_p, صحيح *آخري_p)
{
	حرف *src_w_slash = جوڙيو_slash(src);
	صحيح پهريون, آخري, len_w_slash = ڊيگهائي + 1;

	پهريون = اشاريو_name_pos(the_repository->اشاريو, src_w_slash, len_w_slash);
	إذا (پهريون >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	پهريون = -1 - پهريون;
	لكل (آخري = پهريون; آخري < the_repository->اشاريو->cache_nr; آخري++) {
		ثابت حرف *رستو = the_repository->اشاريو->cache[آخري]->name;
		إذا (لڙي_ڀيٽ(رستو, src_w_slash, len_w_slash))
			اكسر;
	}

	حرر(src_w_slash);
	*پهريون_p = پهريون;
	*آخري_p = آخري;
	أرجع آخري - پهريون;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
ساكن صحيح خالي_جايون_dir_has_sparse_contents(ثابت حرف *name)
{
	صحيح ret = 0;
	حرف *with_slash = جوڙيو_slash(name);
	صحيح ڊيگهائي = لڙي_ڊيگهه(with_slash);

	صحيح pos = اشاريو_name_pos(the_repository->اشاريو, with_slash, ڊيگهائي);
	ثابت بنية cache_داخلا *ce;

	إذا (pos < 0) {
		pos = -pos - 1;
		إذا (pos >= the_repository->اشاريو->cache_nr)
			اذهب free_return;
		ce = the_repository->اشاريو->cache[pos];
		إذا (لڙي_ڀيٽ(with_slash, ce->name, ڊيگهائي))
			اذهب free_return;
		إذا (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	حرر(with_slash);
	أرجع ret;
}

ساكن فراغ remove_خالي_جايون_src_dirs(ثابت حرف **src_dir, سائيز_t src_dir_nr)
{
	سائيز_t i;
	بنية strbuf a_src_dir = STRBUF_INIT;

	لكل (i = 0; i < src_dir_nr; i++) {
		صحيح dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		إذا (اشاريو_range_جو_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

بنية pathmap_داخلا {
	بنية hashmap_داخلا ent;
	ثابت حرف *رستو;
};

ساكن صحيح pathmap_cmp(ثابت فراغ *cmp_معلومات اڻ_استعمال_ٿيل,
		       ثابت بنية hashmap_داخلا *a,
		       ثابت بنية hashmap_داخلا *b,
		       ثابت فراغ *ڪنجي اڻ_استعمال_ٿيل)
{
	ثابت بنية pathmap_داخلا *e1 = container_جو(a, بنية pathmap_داخلا, ent);
	ثابت بنية pathmap_داخلا *e2 = container_جو(b, بنية pathmap_داخلا, ent);
	أرجع fspathcmp(e1->رستو, e2->رستو);
}

صحيح cmd_mv(صحيح argc,
	   ثابت حرف **argv,
	   ثابت حرف *اڳياڙي,
	   بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح i, flags, gitmodules_modified = 0;
	صحيح verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	بنية option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_DRY_لاڳيتو(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', عدم, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_پڇاڙي(),
	};
	بنية strvec sources = STRVEC_INIT;
	بنية strvec dest_paths = STRVEC_INIT;
	بنية strvec destinations = STRVEC_INIT;
	بنية strvec submodule_gitfiles_منزل_free = STRVEC_INIT;
	ثابت حرف **submodule_gitfiles;
	حرف *dst_w_slash = عدم;
	بنية strvec src_dir = STRVEC_INIT;
	تعداد update_گهڻائي *modes, dst_گهڻائي = 0;
	بنية stat st, dest_st;
	بنية string_list src_for_dst = STRING_LIST_INIT_DUP;
	بنية تالو_file تالو_file = تالو_INIT;
	بنية cache_داخلا *ce;
	بنية string_list only_ميلاپ_skip_worktree = STRING_LIST_INIT_DUP;
	بنية string_list dirty_paths = STRING_LIST_INIT_DUP;
	بنية hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, عدم);
	بنية strbuf pathbuf = STRBUF_INIT;
	صحيح ret;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, اڳياڙي, builtin_mv_options,
			     builtin_mv_usage, 0);
	إذا (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_اشاريو(the_repository, &تالو_file, تالو_DIE_ON_ERROR);
	إذا (repo_read_اشاريو(the_repository) < 0)
		die(_("index file corrupt"));

	اندروني_اڳياڙي_pathspec(&sources, اڳياڙي, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	إذا (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	اندروني_اڳياڙي_pathspec(&dest_paths, اڳياڙي, argv + argc, 1, flags);
	dst_w_slash = جوڙيو_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, حجم(حرف *));

	إذا (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		اندروني_اڳياڙي_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	وإلا إذا (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_گهڻائي)) {
		اندروني_اڳياڙي_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} وإلا إذا (!رستو_اندريون_sparse_checkout(dst_w_slash, the_repository->اشاريو) &&
		   خالي_جايون_dir_has_sparse_contents(dst_w_slash)) {
		اندروني_اڳياڙي_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_گهڻائي = SKIP_WORKTREE_DIR;
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
		إذا (!رستو_اندريون_cone_گهڻائي_sparse_checkout(destinations.v[0], the_repository->اشاريو))
			dst_گهڻائي = SPARSE;
	}

	/* Checking */
	لكل (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		صحيح ڊيگهائي;
		ثابت حرف *bad = عدم;
		صحيح skip_sparse = 0;

		إذا (show_only)
			لکو(_("Checking rename of '%s' to '%s'\n"), src, dst);

		ڊيگهائي = لڙي_ڊيگهه(src);
		إذا (lstat(src, &st) < 0) {
			صحيح pos;
			ثابت بنية cache_داخلا *ce;

			pos = اشاريو_name_pos(the_repository->اشاريو, src, ڊيگهائي);
			إذا (pos < 0) {
				حرف *src_w_slash = جوڙيو_slash(src);
				إذا (!رستو_اندريون_sparse_checkout(src_w_slash, the_repository->اشاريو) &&
				    خالي_جايون_dir_has_sparse_contents(src)) {
					حرر(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					اذهب dir_check;
				}
				حرر(src_w_slash);
				/* only error if existence is expected. */
				إذا (!(modes[i] & SPARSE))
					bad = _("bad source");
				اذهب act_on_داخلا;
			}
			ce = the_repository->اشاريو->cache[pos];
			إذا (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				اذهب act_on_داخلا;
			}
			إذا (!ignore_sparse) {
				string_list_append(&only_ميلاپ_skip_worktree, src);
				اذهب act_on_داخلا;
			}
			/* Check if dst exists in index */
			إذا (اشاريو_name_pos(the_repository->اشاريو, dst, لڙي_ڊيگهه(dst)) < 0) {
				modes[i] |= SPARSE;
				اذهب act_on_داخلا;
			}
			إذا (!force) {
				bad = _("destination exists");
				اذهب act_on_داخلا;
			}
			modes[i] |= SPARSE;
			اذهب act_on_داخلا;
		}
		إذا (!لڙي_ڀيٽ(src, dst, ڊيگهائي) &&
		    (dst[ڊيگهائي] == 0 || dst[ڊيگهائي] == '/')) {
			bad = _("can not move directory into itself");
			اذهب act_on_داخلا;
		}
		إذا (S_ISDIR(st.st_گهڻائي)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			اذهب act_on_داخلا;
		}

dir_check:
		إذا (S_ISDIR(st.st_گهڻائي)) {
			بنية pathmap_داخلا *داخلا;
			حرف *dst_with_slash;
			سائيز_t dst_with_slash_len;
			صحيح j, n;
			صحيح پهريون = اشاريو_name_pos(the_repository->اشاريو, src, ڊيگهائي), آخري;

			إذا (پهريون >= 0) {
				ثابت حرف *رستو = submodule_gitfile_رستو(src, پهريون);
				إذا (رستو != SUBMODULE_WITH_GITDIR)
					رستو = strvec_ڌڪو(&submodule_gitfiles_منزل_free, رستو);
				submodule_gitfiles[i] = رستو;
				اذهب act_on_داخلا;
			} وإلا إذا (اشاريو_range_جو_same_dir(src, ڊيگهائي,
							   &پهريون, &آخري) < 1) {
				bad = _("source directory is empty");
				اذهب act_on_داخلا;
			}

			داخلا = xmalloc(حجم(*داخلا));
			داخلا->رستو = src;
			hashmap_داخلا_init(&داخلا->ent, fspathhash(src));
			hashmap_جوڙيو(&moved_dirs, &داخلا->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_ڌڪو(&src_dir, src);

			n = argc + آخري - پهريون;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = جوڙيو_slash(dst);
			dst_with_slash_len = لڙي_ڊيگهه(dst_with_slash);

			لكل (j = 0; j < آخري - پهريون; j++) {
				ثابت بنية cache_داخلا *ce = the_repository->اشاريو->cache[پهريون + j];
				ثابت حرف *رستو = ce->name;
				حرف *prefixed_رستو = اڳياڙي_رستو(the_repository, dst_with_slash,
								  dst_with_slash_len, رستو + ڊيگهائي + 1);

				strvec_ڌڪو(&sources, رستو);
				strvec_ڌڪو(&destinations, prefixed_رستو);

				modes[argc + j] = MOVE_ذريعي_والد_DIR | (ce_skip_worktree(ce) ? SPARSE : اشاريو_2);
				submodule_gitfiles[argc + j] = عدم;

				حرر(prefixed_رستو);
			}

			حرر(dst_with_slash);
			argc += آخري - پهريون;
			اذهب act_on_داخلا;
		}
		إذا (!(ce = اشاريو_file_exists(the_repository->اشاريو, src, ڊيگهائي, 0))) {
			bad = _("not under version control");
			اذهب act_on_داخلا;
		}
		إذا (ce_stage(ce)) {
			bad = _("conflicted");
			اذهب act_on_داخلا;
		}
		إذا (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			إذا (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				إذا (S_ISREG(st.st_گهڻائي) || S_ISLNK(st.st_گهڻائي)) {
					إذا (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = عدم;
				} وإلا
					bad = _("Cannot overwrite");
			}
			اذهب act_on_داخلا;
		}
		إذا (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			اذهب act_on_داخلا;
		}
		إذا (is_dir_sep(dst[لڙي_ڊيگهه(dst) - 1])) {
			bad = _("destination directory does not exist");
			اذهب act_on_داخلا;
		}
		إذا (has_symlink_leading_رستو(dst, لڙي_ڊيگهه(dst))) {
			bad = _("destination is beyond a symbolic link");
			اذهب act_on_داخلا;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		إذا (needs_worktree_rename(modes[i], dst_گهڻائي)) {
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
				} وإلا إذا (!S_ISDIR(dir_st.st_گهڻائي)) {
					bad = _("destination is not a directory");
				}

				حرر(dst_dir);
			}

			إذا (bad)
				اذهب act_on_داخلا;
		}

		إذا (ignore_sparse &&
		    (dst_گهڻائي & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    اشاريو_داخلا_exists(the_repository->اشاريو, dst, لڙي_ڊيگهه(dst))) {
			bad = _("destination exists in the index");
			إذا (force) {
				إذا (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = عدم;
			} وإلا {
				اذهب act_on_داخلا;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		إذا (!ignore_sparse &&
		    !رستو_اندريون_sparse_checkout(src, the_repository->اشاريو)) {
			string_list_append(&only_ميلاپ_skip_worktree, src);
			skip_sparse = 1;
		}
		إذا (!ignore_sparse &&
		    !رستو_اندريون_sparse_checkout(dst, the_repository->اشاريو)) {
			string_list_append(&only_ميلاپ_skip_worktree, dst);
			skip_sparse = 1;
		}

		إذا (skip_sparse)
			اذهب remove_داخلا;

		string_list_وجهو(&src_for_dst, dst);

act_on_داخلا:
		إذا (!bad)
			استمر;
		إذا (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_داخلا:
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

		إذا (modes[i] & MOVE_ذريعي_والد_DIR)
			استمر;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		طالما (slash_pos > pathbuf.buf) {
			بنية pathmap_داخلا needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.رستو = pathbuf.buf;
			hashmap_داخلا_init(&needle.ent, fspathhash(pathbuf.buf));

			إذا (hashmap_get_داخلا(&moved_dirs, &needle, ent, عدم))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	إذا (only_ميلاپ_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_ميلاپ_skip_worktree);
		إذا (!ignore_errors) {
			ret = 1;
			اذهب ٻاهريون;
		}
	}

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		تعداد update_گهڻائي گهڻائي = modes[i];
		صحيح pos;
		صحيح sparse_and_dirty = 0;
		بنية checkout state = CHECKOUT_INIT;
		state.istate = the_repository->اشاريو;

		إذا (force)
			state.force = 1;
		إذا (show_only || verbose)
			لکو(_("Renaming %s to %s\n"), src, dst);
		إذا (show_only)
			استمر;
		إذا (needs_worktree_rename(گهڻائي, dst_گهڻائي) &&
		    rename(src, dst) < 0) {
			إذا (ignore_errors)
				استمر;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		إذا (submodule_gitfiles[i]) {
			إذا (!update_رستو_اندريون_gitmodules(src, dst))
				gitmodules_modified = 1;
			إذا (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_وڻ_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		إذا (گهڻائي & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			استمر;

		pos = اشاريو_name_pos(the_repository->اشاريو, src, لڙي_ڊيگهه(src));
		إذا (pos < 0)
			BUG("could not find source in index: '%s'", src);
		إذا (!(گهڻائي & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->اشاريو,
						       the_repository->اشاريو->cache[pos],
						       &st,
						       0);
		rename_اشاريو_داخلا_at(the_repository->اشاريو, pos, dst);

		إذا (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			إذا ((گهڻائي & SPARSE) &&
			    رستو_اندريون_sparse_checkout(dst, the_repository->اشاريو)) {
				/* from out-of-cone to in-cone */
				صحيح dst_pos = اشاريو_name_pos(the_repository->اشاريو, dst,
							     لڙي_ڊيگهه(dst));
				بنية cache_داخلا *dst_ce = the_repository->اشاريو->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				إذا (checkout_داخلا(dst_ce, &state, عدم, عدم))
					die(_("cannot checkout %s"), dst_ce->name);
			} وإلا إذا ((dst_گهڻائي & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(گهڻائي & SPARSE) &&
				   !رستو_اندريون_sparse_checkout(dst, the_repository->اشاريو)) {
				/* from in-cone to out-of-cone */
				صحيح dst_pos = اشاريو_name_pos(the_repository->اشاريو, dst,
							     لڙي_ڊيگهه(dst));
				بنية cache_داخلا *dst_ce = the_repository->اشاريو->cache[dst_pos];

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

	remove_خالي_جايون_src_dirs(src_dir.v, src_dir.nr);

	إذا (dirty_paths.nr)
		advise_on_moving_dirty_رستو(&dirty_paths);

	إذا (gitmodules_modified)
		stage_updated_gitmodules(the_repository->اشاريو);

	إذا (write_locked_اشاريو(the_repository->اشاريو, &تالو_file,
			       COMMIT_تالو | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

ٻاهريون:
	strvec_clear(&src_dir);
	حرر(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_ميلاپ_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_منزل_free);
	hashmap_clear_and_free(&moved_dirs, بنية pathmap_داخلا, ent);
	strbuf_release(&pathbuf);
	حرر(submodule_gitfiles);
	حرر(modes);
	أرجع ret;
}
