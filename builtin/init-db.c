/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "abspath.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "refs.h"
#شامل "setup.h"
#شامل "strbuf.h"

ایستا صحیح guess_repository_type(ثابت نویسه *git_dir)
{
	ثابت نویسه *slash;
	نویسه *cwd;
	صحیح cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	اگر (!strcmp(".", git_dir))
		بازگشت 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !strcmp(git_dir, cwd);
	آزاد(cwd);
	اگر (cwd_is_git_dir)
		بازگشت 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	اگر (!strcmp(git_dir, ".git"))
		بازگشت 0;
	slash = strrchr(git_dir, '/');
	اگر (slash && !strcmp(slash, "/.git"))
		بازگشت 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	بازگشت 1;
}

ایستا صحیح shared_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	*((صحیح *) opt->مقدار) = (arg) ? git_config_perm("arg", arg) : PERM_GROUP;
	بازگشت 0;
}

ایستا ثابت نویسه *ثابت init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	پوچ
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
صحیح cmd_init_db(صحیح argc,
		ثابت نویسه **argv,
		ثابت نویسه *prefix,
		ساختار repository *repo UNUSED)
{
	نویسه *git_dir;
	ثابت نویسه *real_git_dir = پوچ;
	نویسه *real_git_dir_to_free = پوچ;
	نویسه *work_درخت = پوچ;
	ثابت نویسه *template_dir = پوچ;
	نویسه *template_dir_to_free = پوچ;
	صحیح quiet = 0;
	صحیح bare = startup_info->force_bare_repository ? 1 : -1;
	ثابت نویسه *object_format = پوچ;
	ثابت نویسه *ref_storage_format_uri = پوچ;
	ثابت نویسه *initial_branch = پوچ;
	صحیح hash_algo = GIT_HASH_UNKNOWN;
	صحیح init_shared_repository = -1;
	ثابت ساختار option init_db_options[] = {
		OPT_STRING(0, "template", &template_dir, N_("template-directory"),
				N_("directory from which templates will be used")),
		OPT_SET_INT(0, "bare", &bare,
				N_("create a bare repository"), 1),
		{
			.type = OPTION_CALLBACK,
			.long_name = "shared",
			.مقدار = &init_shared_repository,
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
		OPT_END()
	};
	صحیح reinit;

	argc = parse_options(argc, argv, prefix, init_db_options, init_db_usage, 0);

	اگر (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	اگر (real_git_dir && !is_absolute_path(real_git_dir))
		real_git_dir = real_git_dir_to_free = real_pathdup(real_git_dir, 1);

	اگر (template_dir && *template_dir && !is_absolute_path(template_dir))
		template_dir = template_dir_to_free = absolute_pathdup(template_dir);

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
				repo_settings_set_shared_repository(the_repository, 0);
				انتخاب (safe_create_leading_directories_const(the_repository, argv[0])) {
				حالت SCLD_OK:
				حالت SCLD_PERMS:
					بشکن;
				حالت SCLD_EXISTS:
					errno = EEXIST;
					/* fallthru */
				پیشفرض:
					die_errno(_("cannot mkdir %s"), argv[0]);
					بشکن;
				}
				repo_settings_set_shared_repository(the_repository, saved);
				اگر (mkdir(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				برو retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} وگرنه اگر (0 < argc) {
		usage(init_db_usage[0]);
	}
	اگر (bare == 1) {
		نویسه *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		آزاد(cwd);
	}

	اگر (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		اگر (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	اگر (init_shared_repository != -1)
		repo_settings_set_shared_repository(the_repository, init_shared_repository);

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
		ثابت نویسه *p;
		ساختار strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		اگر (p && get_common_dir(&sb, p)) {
			ساختار strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			اگر (chdir(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			آزاد(git_dir);
			git_dir = strbuf_detach(&sb, پوچ);
		}
		strbuf_release(&sb);
	}

	اگر (bare < 0)
		bare = guess_repository_type(git_dir);

	اگر (!bare) {
		ثابت نویسه *git_dir_parent = strrchr(git_dir, '/');

		اگر (!work_درخت) {
			اگر (git_dir_parent) {
				نویسه *rel = xstrndup(git_dir, git_dir_parent - git_dir);
				work_درخت = real_pathdup(rel, 1);
				آزاد(rel);
			} وگرنه {
				work_درخت = xgetcwd();
			}
		}

		اگر (access(work_درخت, X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_درخت);
	} وگرنه اگر (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_درخت,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, پوچ);

	اگر (!quiet) {
		صحیح len = strlen(git_dir);

		اگر (reinit)
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Reinitialized existing shared Git repository in %s%s\n")
			       : _("Reinitialized existing Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
		وگرنه
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Initialized empty shared Git repository in %s%s\n")
			       : _("Initialized empty Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
	}

	آزاد(template_dir_to_free);
	آزاد(real_git_dir_to_free);
	آزاد(work_درخت);
	آزاد(git_dir);
	بازگشت 0;
}
