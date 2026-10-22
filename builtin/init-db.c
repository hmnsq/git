/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "abspath.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "refs.h"
#شامل "setup.h"
#شامل "strbuf.h"

ساکن صحیح guess_repository_type(ثابت حرف *git_dir)
{
	ثابت حرف *slash;
	حرف *cwd;
	صحیح cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	اگر (!strcmp(".", git_dir))
		واپس 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !strcmp(git_dir, cwd);
	آزاد(cwd);
	اگر (cwd_is_git_dir)
		واپس 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	اگر (!strcmp(git_dir, ".git"))
		واپس 0;
	slash = strrchr(git_dir, '/');
	اگر (slash && !strcmp(slash, "/.git"))
		واپس 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	واپس 1;
}

ساکن صحیح shared_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	*((صحیح *) opt->قدر) = (arg) ? git_config_perm("arg", arg) : PERM_گروہ;
	واپس 0;
}

ساکن ثابت حرف *ثابت init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	تہی
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
صحیح cmd_init_db(صحیح argc,
		ثابت حرف **argv,
		ثابت حرف *سابقہ,
		ساخت repository *repo غیر_مستعمل)
{
	حرف *git_dir;
	ثابت حرف *real_git_dir = تہی;
	حرف *real_git_dir_مقصد_free = تہی;
	حرف *work_درخت = تہی;
	ثابت حرف *template_dir = تہی;
	حرف *template_dir_مقصد_free = تہی;
	صحیح quiet = 0;
	صحیح bare = startup_info->force_bare_repository ? 1 : -1;
	ثابت حرف *object_format = تہی;
	ثابت حرف *ref_storage_format_uri = تہی;
	ثابت حرف *initial_branch = تہی;
	صحیح hash_algo = GIT_HASH_UNKNOWN;
	صحیح init_shared_repository = -1;
	ثابت ساخت option init_db_options[] = {
		OPT_STRING(0, "template", &template_dir, N_("template-directory"),
				N_("directory from which templates will be used")),
		OPT_سیٹ_INT(0, "bare", &bare,
				N_("create a bare repository"), 1),
		{
			.type = OPTION_CALLBACK,
			.long_name = "shared",
			.قدر = &init_shared_repository,
			.argh = N_("permissions"),
			.help = N_("specify that the git repository is to be shared amongst several users"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			.callback = shared_callback
		},
		OPT_BOOL('q', "quiet", &quiet, N_("be quiet")),
		OPT_STRING(0, "separate-git-dir", &real_git_dir, N_("gitdir"),
			   N_("separate git dir from working tree")),
		OPT_STRING('b', "initial-branch", &initial_branch, N_("name"),
			   N_("override the name of the initial branch")),
		OPT_STRING(0, "object-format", &object_format, N_("hash"),
			   N_("specify the hash algorithm to use")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format_uri, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_انتہا()
	};
	صحیح reinit;

	argc = parse_options(argc, argv, سابقہ, init_db_options, init_db_usage, 0);

	اگر (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	اگر (real_git_dir && !is_absolute_راستہ(real_git_dir))
		real_git_dir = real_git_dir_مقصد_free = real_pathdup(real_git_dir, 1);

	اگر (template_dir && *template_dir && !is_absolute_راستہ(template_dir))
		template_dir = template_dir_مقصد_free = absolute_pathdup(template_dir);

	اگر (argc == 1) {
		صحیح mkdir_tried = 0;
	retry:
		اگر (chdir(argv[0]) < 0) {
			اگر (!mkdir_tried) {
				صحیح saved;
				/*
				 * At this point we haven't read any configuration,
				 * and we know shared_repository should always be 0;
				 * but just in case we play safe.
				 */
				saved = repo_settings_get_shared_repository(the_repository);
				repo_settings_سیٹ_shared_repository(the_repository, 0);
				انتخاب (محفوظ_create_leading_directories_const(the_repository, argv[0])) {
				حالت SCLD_OK:
				حالت SCLD_PERMS:
					توڑو;
				حالت SCLD_EXISTS:
					errno = EEXIST;
					/* fallthru */
				طےشدہ:
					die_errno(_("cannot mkdir %s"), argv[0]);
					توڑو;
				}
				repo_settings_سیٹ_shared_repository(the_repository, saved);
				اگر (mkdir(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				جاؤ retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} ورنہ اگر (0 < argc) {
		usage(init_db_usage[0]);
	}
	اگر (bare == 1) {
		حرف *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		آزاد(cwd);
	}

	اگر (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		اگر (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	اگر (init_shared_repository != -1)
		repo_settings_سیٹ_shared_repository(the_repository, init_shared_repository);

	/*
	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR
	 * without --bare.  Catch the error early.
	 */
	git_dir = xstrdup_or_null(getenv(GIT_DIR_ENVIRONMENT));
	work_درخت = xstrdup_or_null(getenv(GIT_WORK_درخت_ENVIRONMENT));
	اگر ((!git_dir || bare == 1) && work_درخت)
		die(_("%s (or --work-tree=<directory>) not allowed without "
			  "specifying %s (or --git-dir=<directory>)"),
		    GIT_WORK_درخت_ENVIRONMENT,
		    GIT_DIR_ENVIRONMENT);

	/*
	 * Set up the default .git directory contents
	 */
	اگر (!git_dir)
		git_dir = xstrdup(DEFAULT_GIT_DIR_ENVIRONMENT);

	/*
	 * When --separate-git-dir is used inside a linked worktree, take
	 * care to ensure that the common .git/ directory is relocated, not
	 * the worktree-specific .git/worktrees/<id>/ directory.
	 */
	اگر (real_git_dir) {
		صحیح err;
		ثابت حرف *p;
		ساخت strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		اگر (p && get_common_dir(&sb, p)) {
			ساخت strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			اگر (chdir(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			آزاد(git_dir);
			git_dir = strbuf_detach(&sb, تہی);
		}
		strbuf_release(&sb);
	}

	اگر (bare < 0)
		bare = guess_repository_type(git_dir);

	اگر (!bare) {
		ثابت حرف *git_dir_والد = strrchr(git_dir, '/');

		اگر (!work_درخت) {
			اگر (git_dir_والد) {
				حرف *rel = xstrndup(git_dir, git_dir_والد - git_dir);
				work_درخت = real_pathdup(rel, 1);
				آزاد(rel);
			} ورنہ {
				work_درخت = xgetcwd();
			}
		}

		اگر (access(work_درخت, X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_درخت);
	} ورنہ اگر (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_درخت,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, تہی);

	اگر (!quiet) {
		صحیح len = لڑی_لمبائی(git_dir);

		اگر (reinit)
			لکھو(repo_settings_get_shared_repository(the_repository)
			       ? _("Reinitialized existing shared Git repository in %s%s\n")
			       : _("Reinitialized existing Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
		ورنہ
			لکھو(repo_settings_get_shared_repository(the_repository)
			       ? _("Initialized empty shared Git repository in %s%s\n")
			       : _("Initialized empty Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
	}

	آزاد(template_dir_مقصد_free);
	آزاد(real_git_dir_مقصد_free);
	آزاد(work_درخت);
	آزاد(git_dir);
	واپس 0;
}
