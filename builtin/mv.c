/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

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

تعداد update_ډېرپېښ {
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
	MOVE_له_لارې_پلار_DIR = (1 << 5),
};

ساكن صحيح needs_worktree_rename(تعداد update_ډېرپېښ ډېرپېښ, تعداد update_ډېرپېښ dst_ډېرپېښ)
{
	أرجع !(ډېرپېښ & (شاخص_2 | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_ډېرپېښ & (SKIP_WORKTREE_DIR | SPARSE));
}

#تعریف_کړه DUP_BASENAME 1
#تعریف_کړه KEEP_TRAILING_SLASH 2

ساكن فراغ دننی_مختاړی_pathspec(بنية strvec *بهر,
				     ثابت حرف *مختاړی,
				     ثابت حرف **pathspec,
				     صحيح شمېر, بینښه flags)
{
	صحيح prefixlen = مختاړی ? مزي_اوږدوالی(مختاړی) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	لكل (صحيح i = 0; i < شمېر; i++) {
		اندازه_t اوږدتیا = مزي_اوږدوالی(pathspec[i]);
		اندازه_t منزل_copy = اوږدتیا;
		ثابت حرف *maybe_basename;
		حرف *trimmed, *prefixed_لار;

		طالما (!(flags & KEEP_TRAILING_SLASH) &&
		       منزل_copy > 0 && is_dir_sep(pathspec[i][منزل_copy - 1]))
			منزل_copy--;

		trimmed = xmemdupz(pathspec[i], منزل_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_لار = مختاړی_لار(the_repository, مختاړی, prefixlen, maybe_basename);
		strvec_ټېل(بهر, prefixed_لار);

		حرر(prefixed_لار);
		حرر(trimmed);
	}
}

ساكن حرف *جمع_کړه_slash(ثابت حرف *لار)
{
	اندازه_t len = مزي_اوږدوالی(لار);
	إذا (len && لار[len - 1] != '/') {
		حرف *with_slash = xmalloc(st_جمع_کړه(len, 2));
		حافظه_نقل(with_slash, لار, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		أرجع with_slash;
	}
	أرجع xstrdup(لار);
}

#تعریف_کړه SUBMODULE_WITH_GITDIR ((ثابت حرف *)1)

ساكن ثابت حرف *submodule_gitfile_لار(ثابت حرف *src, صحيح لومړی)
{
	بنية strbuf submodule_dotgit = STRBUF_INIT;
	ثابت حرف *لار;

	إذا (!S_ISGITLINK(the_repository->شاخص->cache[لومړی]->ce_ډېرپېښ))
		die(_("Directory %s is in index and no submodule?"), src);
	إذا (!is_staging_gitmodules_ok(the_repository->شاخص))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	لار = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	إذا (لار)
		أرجع لار;
	أرجع SUBMODULE_WITH_GITDIR;
}

ساكن صحيح شاخص_range_د_same_dir(ثابت حرف *src, صحيح اوږدتیا,
				   صحيح *لومړی_p, صحيح *وروستی_p)
{
	حرف *src_w_slash = جمع_کړه_slash(src);
	صحيح لومړی, وروستی, len_w_slash = اوږدتیا + 1;

	لومړی = شاخص_name_pos(the_repository->شاخص, src_w_slash, len_w_slash);
	إذا (لومړی >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	لومړی = -1 - لومړی;
	لكل (وروستی = لومړی; وروستی < the_repository->شاخص->cache_nr; وروستی++) {
		ثابت حرف *لار = the_repository->شاخص->cache[وروستی]->name;
		إذا (مزي_پرتله(لار, src_w_slash, len_w_slash))
			اكسر;
	}

	حرر(src_w_slash);
	*لومړی_p = لومړی;
	*وروستی_p = وروستی;
	أرجع وروستی - لومړی;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
ساكن صحيح تشه_dir_has_sparse_contents(ثابت حرف *name)
{
	صحيح ret = 0;
	حرف *with_slash = جمع_کړه_slash(name);
	صحيح اوږدتیا = مزي_اوږدوالی(with_slash);

	صحيح pos = شاخص_name_pos(the_repository->شاخص, with_slash, اوږدتیا);
	ثابت بنية cache_ثبت *ce;

	إذا (pos < 0) {
		pos = -pos - 1;
		إذا (pos >= the_repository->شاخص->cache_nr)
			اذهب free_return;
		ce = the_repository->شاخص->cache[pos];
		إذا (مزي_پرتله(with_slash, ce->name, اوږدتیا))
			اذهب free_return;
		إذا (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	حرر(with_slash);
	أرجع ret;
}

ساكن فراغ remove_تشه_src_dirs(ثابت حرف **src_dir, اندازه_t src_dir_nr)
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
		إذا (شاخص_range_د_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

بنية pathmap_ثبت {
	بنية hashmap_ثبت ent;
	ثابت حرف *لار;
};

ساكن صحيح pathmap_cmp(ثابت فراغ *cmp_معلومات ناکارول_شوی,
		       ثابت بنية hashmap_ثبت *a,
		       ثابت بنية hashmap_ثبت *b,
		       ثابت فراغ *کلی ناکارول_شوی)
{
	ثابت بنية pathmap_ثبت *e1 = container_د(a, بنية pathmap_ثبت, ent);
	ثابت بنية pathmap_ثبت *e2 = container_د(b, بنية pathmap_ثبت, ent);
	أرجع fspathcmp(e1->لار, e2->لار);
}

صحيح cmd_mv(صحيح argc,
	   ثابت حرف **argv,
	   ثابت حرف *مختاړی,
	   بنية repository *repo ناکارول_شوی)
{
	صحيح i, flags, gitmodules_modified = 0;
	صحيح verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	بنية option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_DRY_پرلپسې(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', عدم, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_پای_ټکی(),
	};
	بنية strvec sources = STRVEC_INIT;
	بنية strvec dest_paths = STRVEC_INIT;
	بنية strvec destinations = STRVEC_INIT;
	بنية strvec submodule_gitfiles_منزل_free = STRVEC_INIT;
	ثابت حرف **submodule_gitfiles;
	حرف *dst_w_slash = عدم;
	بنية strvec src_dir = STRVEC_INIT;
	تعداد update_ډېرپېښ *modes, dst_ډېرپېښ = 0;
	بنية stat st, dest_st;
	بنية string_list src_for_dst = STRING_LIST_INIT_DUP;
	بنية قلف_file قلف_file = قلف_INIT;
	بنية cache_ثبت *ce;
	بنية string_list only_سمون_skip_worktree = STRING_LIST_INIT_DUP;
	بنية string_list dirty_paths = STRING_LIST_INIT_DUP;
	بنية hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, عدم);
	بنية strbuf pathbuf = STRBUF_INIT;
	صحيح ret;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, مختاړی, builtin_mv_options,
			     builtin_mv_usage, 0);
	إذا (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_شاخص(the_repository, &قلف_file, قلف_DIE_ON_ERROR);
	إذا (repo_read_شاخص(the_repository) < 0)
		die(_("index file corrupt"));

	دننی_مختاړی_pathspec(&sources, مختاړی, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	إذا (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	دننی_مختاړی_pathspec(&dest_paths, مختاړی, argv + argc, 1, flags);
	dst_w_slash = جمع_کړه_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, حجم(حرف *));

	إذا (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		دننی_مختاړی_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	وإلا إذا (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_ډېرپېښ)) {
		دننی_مختاړی_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} وإلا إذا (!لار_دننه_sparse_checkout(dst_w_slash, the_repository->شاخص) &&
		   تشه_dir_has_sparse_contents(dst_w_slash)) {
		دننی_مختاړی_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_ډېرپېښ = SKIP_WORKTREE_DIR;
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
		إذا (!لار_دننه_cone_ډېرپېښ_sparse_checkout(destinations.v[0], the_repository->شاخص))
			dst_ډېرپېښ = SPARSE;
	}

	/* Checking */
	لكل (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		صحيح اوږدتیا;
		ثابت حرف *bad = عدم;
		صحيح skip_sparse = 0;

		إذا (show_only)
			چاپ_کړه(_("Checking rename of '%s' to '%s'\n"), src, dst);

		اوږدتیا = مزي_اوږدوالی(src);
		إذا (lstat(src, &st) < 0) {
			صحيح pos;
			ثابت بنية cache_ثبت *ce;

			pos = شاخص_name_pos(the_repository->شاخص, src, اوږدتیا);
			إذا (pos < 0) {
				حرف *src_w_slash = جمع_کړه_slash(src);
				إذا (!لار_دننه_sparse_checkout(src_w_slash, the_repository->شاخص) &&
				    تشه_dir_has_sparse_contents(src)) {
					حرر(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					اذهب dir_check;
				}
				حرر(src_w_slash);
				/* only error if existence is expected. */
				إذا (!(modes[i] & SPARSE))
					bad = _("bad source");
				اذهب act_on_ثبت;
			}
			ce = the_repository->شاخص->cache[pos];
			إذا (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				اذهب act_on_ثبت;
			}
			إذا (!ignore_sparse) {
				string_list_append(&only_سمون_skip_worktree, src);
				اذهب act_on_ثبت;
			}
			/* Check if dst exists in index */
			إذا (شاخص_name_pos(the_repository->شاخص, dst, مزي_اوږدوالی(dst)) < 0) {
				modes[i] |= SPARSE;
				اذهب act_on_ثبت;
			}
			إذا (!force) {
				bad = _("destination exists");
				اذهب act_on_ثبت;
			}
			modes[i] |= SPARSE;
			اذهب act_on_ثبت;
		}
		إذا (!مزي_پرتله(src, dst, اوږدتیا) &&
		    (dst[اوږدتیا] == 0 || dst[اوږدتیا] == '/')) {
			bad = _("can not move directory into itself");
			اذهب act_on_ثبت;
		}
		إذا (S_ISDIR(st.st_ډېرپېښ)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			اذهب act_on_ثبت;
		}

dir_check:
		إذا (S_ISDIR(st.st_ډېرپېښ)) {
			بنية pathmap_ثبت *ثبت;
			حرف *dst_with_slash;
			اندازه_t dst_with_slash_len;
			صحيح j, n;
			صحيح لومړی = شاخص_name_pos(the_repository->شاخص, src, اوږدتیا), وروستی;

			إذا (لومړی >= 0) {
				ثابت حرف *لار = submodule_gitfile_لار(src, لومړی);
				إذا (لار != SUBMODULE_WITH_GITDIR)
					لار = strvec_ټېل(&submodule_gitfiles_منزل_free, لار);
				submodule_gitfiles[i] = لار;
				اذهب act_on_ثبت;
			} وإلا إذا (شاخص_range_د_same_dir(src, اوږدتیا,
							   &لومړی, &وروستی) < 1) {
				bad = _("source directory is empty");
				اذهب act_on_ثبت;
			}

			ثبت = xmalloc(حجم(*ثبت));
			ثبت->لار = src;
			hashmap_ثبت_init(&ثبت->ent, fspathhash(src));
			hashmap_جمع_کړه(&moved_dirs, &ثبت->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_ټېل(&src_dir, src);

			n = argc + وروستی - لومړی;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = جمع_کړه_slash(dst);
			dst_with_slash_len = مزي_اوږدوالی(dst_with_slash);

			لكل (j = 0; j < وروستی - لومړی; j++) {
				ثابت بنية cache_ثبت *ce = the_repository->شاخص->cache[لومړی + j];
				ثابت حرف *لار = ce->name;
				حرف *prefixed_لار = مختاړی_لار(the_repository, dst_with_slash,
								  dst_with_slash_len, لار + اوږدتیا + 1);

				strvec_ټېل(&sources, لار);
				strvec_ټېل(&destinations, prefixed_لار);

				modes[argc + j] = MOVE_له_لارې_پلار_DIR | (ce_skip_worktree(ce) ? SPARSE : شاخص_2);
				submodule_gitfiles[argc + j] = عدم;

				حرر(prefixed_لار);
			}

			حرر(dst_with_slash);
			argc += وروستی - لومړی;
			اذهب act_on_ثبت;
		}
		إذا (!(ce = شاخص_file_exists(the_repository->شاخص, src, اوږدتیا, 0))) {
			bad = _("not under version control");
			اذهب act_on_ثبت;
		}
		إذا (ce_stage(ce)) {
			bad = _("conflicted");
			اذهب act_on_ثبت;
		}
		إذا (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			إذا (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				إذا (S_ISREG(st.st_ډېرپېښ) || S_ISLNK(st.st_ډېرپېښ)) {
					إذا (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = عدم;
				} وإلا
					bad = _("Cannot overwrite");
			}
			اذهب act_on_ثبت;
		}
		إذا (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			اذهب act_on_ثبت;
		}
		إذا (is_dir_sep(dst[مزي_اوږدوالی(dst) - 1])) {
			bad = _("destination directory does not exist");
			اذهب act_on_ثبت;
		}
		إذا (has_symlink_leading_لار(dst, مزي_اوږدوالی(dst))) {
			bad = _("destination is beyond a symbolic link");
			اذهب act_on_ثبت;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		إذا (needs_worktree_rename(modes[i], dst_ډېرپېښ)) {
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
				} وإلا إذا (!S_ISDIR(dir_st.st_ډېرپېښ)) {
					bad = _("destination is not a directory");
				}

				حرر(dst_dir);
			}

			إذا (bad)
				اذهب act_on_ثبت;
		}

		إذا (ignore_sparse &&
		    (dst_ډېرپېښ & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    شاخص_ثبت_exists(the_repository->شاخص, dst, مزي_اوږدوالی(dst))) {
			bad = _("destination exists in the index");
			إذا (force) {
				إذا (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = عدم;
			} وإلا {
				اذهب act_on_ثبت;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		إذا (!ignore_sparse &&
		    !لار_دننه_sparse_checkout(src, the_repository->شاخص)) {
			string_list_append(&only_سمون_skip_worktree, src);
			skip_sparse = 1;
		}
		إذا (!ignore_sparse &&
		    !لار_دننه_sparse_checkout(dst, the_repository->شاخص)) {
			string_list_append(&only_سمون_skip_worktree, dst);
			skip_sparse = 1;
		}

		إذا (skip_sparse)
			اذهب remove_ثبت;

		string_list_ورننباسه(&src_for_dst, dst);

act_on_ثبت:
		إذا (!bad)
			استمر;
		إذا (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_ثبت:
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

		إذا (modes[i] & MOVE_له_لارې_پلار_DIR)
			استمر;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		طالما (slash_pos > pathbuf.buf) {
			بنية pathmap_ثبت needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.لار = pathbuf.buf;
			hashmap_ثبت_init(&needle.ent, fspathhash(pathbuf.buf));

			إذا (hashmap_get_ثبت(&moved_dirs, &needle, ent, عدم))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	إذا (only_سمون_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_سمون_skip_worktree);
		إذا (!ignore_errors) {
			ret = 1;
			اذهب بهر;
		}
	}

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *src = sources.v[i], *dst = destinations.v[i];
		تعداد update_ډېرپېښ ډېرپېښ = modes[i];
		صحيح pos;
		صحيح sparse_and_dirty = 0;
		بنية checkout state = CHECKOUT_INIT;
		state.istate = the_repository->شاخص;

		إذا (force)
			state.force = 1;
		إذا (show_only || verbose)
			چاپ_کړه(_("Renaming %s to %s\n"), src, dst);
		إذا (show_only)
			استمر;
		إذا (needs_worktree_rename(ډېرپېښ, dst_ډېرپېښ) &&
		    rename(src, dst) < 0) {
			إذا (ignore_errors)
				استمر;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		إذا (submodule_gitfiles[i]) {
			إذا (!update_لار_دننه_gitmodules(src, dst))
				gitmodules_modified = 1;
			إذا (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_ونه_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		إذا (ډېرپېښ & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			استمر;

		pos = شاخص_name_pos(the_repository->شاخص, src, مزي_اوږدوالی(src));
		إذا (pos < 0)
			BUG("could not find source in index: '%s'", src);
		إذا (!(ډېرپېښ & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->شاخص,
						       the_repository->شاخص->cache[pos],
						       &st,
						       0);
		rename_شاخص_ثبت_at(the_repository->شاخص, pos, dst);

		إذا (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			إذا ((ډېرپېښ & SPARSE) &&
			    لار_دننه_sparse_checkout(dst, the_repository->شاخص)) {
				/* from out-of-cone to in-cone */
				صحيح dst_pos = شاخص_name_pos(the_repository->شاخص, dst,
							     مزي_اوږدوالی(dst));
				بنية cache_ثبت *dst_ce = the_repository->شاخص->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				إذا (checkout_ثبت(dst_ce, &state, عدم, عدم))
					die(_("cannot checkout %s"), dst_ce->name);
			} وإلا إذا ((dst_ډېرپېښ & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(ډېرپېښ & SPARSE) &&
				   !لار_دننه_sparse_checkout(dst, the_repository->شاخص)) {
				/* from in-cone to out-of-cone */
				صحيح dst_pos = شاخص_name_pos(the_repository->شاخص, dst,
							     مزي_اوږدوالی(dst));
				بنية cache_ثبت *dst_ce = the_repository->شاخص->cache[dst_pos];

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
					خوندي_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_تشه_src_dirs(src_dir.v, src_dir.nr);

	إذا (dirty_paths.nr)
		advise_on_moving_dirty_لار(&dirty_paths);

	إذا (gitmodules_modified)
		stage_updated_gitmodules(the_repository->شاخص);

	إذا (write_locked_شاخص(the_repository->شاخص, &قلف_file,
			       COMMIT_قلف | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

بهر:
	strvec_clear(&src_dir);
	حرر(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_سمون_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_منزل_free);
	hashmap_clear_and_free(&moved_dirs, بنية pathmap_ثبت, ent);
	strbuf_release(&pathbuf);
	حرر(submodule_gitfiles);
	حرر(modes);
	أرجع ret;
}
