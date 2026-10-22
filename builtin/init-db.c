/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "abspath.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "setup.h"
#অন্তর্ভাৱ "strbuf.h"

স্থির পূর্ণাঙ্ক guess_repository_type(ধ্রুৱ ৱর্ণ *git_dir)
{
	ধ্রুৱ ৱর্ণ *slash;
	ৱর্ণ *cwd;
	পূর্ণাঙ্ক cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	যদি (!strcmp(".", git_dir))
		ঘূৰাই 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !strcmp(git_dir, cwd);
	মুক্ত(cwd);
	যদি (cwd_is_git_dir)
		ঘূৰাই 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	যদি (!strcmp(git_dir, ".git"))
		ঘূৰাই 0;
	slash = strrchr(git_dir, '/');
	যদি (slash && !strcmp(slash, "/.git"))
		ঘূৰাই 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	ঘূৰাই 1;
}

স্থির পূর্ণাঙ্ক shared_callback(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	*((পূর্ণাঙ্ক *) opt->মান) = (arg) ? git_config_perm("arg", arg) : PERM_GROUP;
	ঘূৰাই 0;
}

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	রিক্ত
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
পূর্ণাঙ্ক cmd_init_db(পূর্ণাঙ্ক argc,
		ধ্রুৱ ৱর্ণ **argv,
		ধ্রুৱ ৱর্ণ *prefix,
		সংরচনা repository *repo UNUSED)
{
	ৱর্ণ *git_dir;
	ধ্রুৱ ৱর্ণ *real_git_dir = রিক্ত;
	ৱর্ণ *real_git_dir_to_free = রিক্ত;
	ৱর্ণ *work_বৃক্ষ = রিক্ত;
	ধ্রুৱ ৱর্ণ *template_dir = রিক্ত;
	ৱর্ণ *template_dir_to_free = রিক্ত;
	পূর্ণাঙ্ক quiet = 0;
	পূর্ণাঙ্ক bare = startup_info->force_bare_repository ? 1 : -1;
	ধ্রুৱ ৱর্ণ *object_format = রিক্ত;
	ধ্রুৱ ৱর্ণ *ref_storage_format_uri = রিক্ত;
	ধ্রুৱ ৱর্ণ *initial_branch = রিক্ত;
	পূর্ণাঙ্ক hash_algo = GIT_HASH_UNKNOWN;
	পূর্ণাঙ্ক init_shared_repository = -1;
	ধ্রুৱ সংরচনা option init_db_options[] = {
		OPT_STRING(0, "template", &template_dir, N_("template-directory"),
				N_("directory from which templates will be used")),
		OPT_SET_INT(0, "bare", &bare,
				N_("create a bare repository"), 1),
		{
			.type = OPTION_CALLBACK,
			.long_name = "shared",
			.মান = &init_shared_repository,
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
	পূর্ণাঙ্ক reinit;

	argc = parse_options(argc, argv, prefix, init_db_options, init_db_usage, 0);

	যদি (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	যদি (real_git_dir && !is_absolute_path(real_git_dir))
		real_git_dir = real_git_dir_to_free = real_pathdup(real_git_dir, 1);

	যদি (template_dir && *template_dir && !is_absolute_path(template_dir))
		template_dir = template_dir_to_free = absolute_pathdup(template_dir);

	যদি (argc == 1) {
		পূর্ণাঙ্ক mkdir_tried = 0;
	retry:
		যদি (chdir(argv[0]) < 0) {
			যদি (!mkdir_tried) {
				পূর্ণাঙ্ক saved;
				/*
				 * At this point we haven't read any configuration,
				 * and we know shared_repository should always be 0;
				 * but just in case we play safe.
				 */
				saved = repo_settings_get_shared_repository(the_repository);
				repo_settings_set_shared_repository(the_repository, 0);
				ৱিকল্প (safe_create_leading_directories_const(the_repository, argv[0])) {
				পক্ষ SCLD_OK:
				পক্ষ SCLD_PERMS:
					ভঙ্গ;
				পক্ষ SCLD_EXISTS:
					errno = EEXIST;
					/* fallthru */
				স্ৱভাৱ:
					die_errno(_("cannot mkdir %s"), argv[0]);
					ভঙ্গ;
				}
				repo_settings_set_shared_repository(the_repository, saved);
				যদি (mkdir(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				গচ্ছ retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} অন্যথা যদি (0 < argc) {
		usage(init_db_usage[0]);
	}
	যদি (bare == 1) {
		ৱর্ণ *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		মুক্ত(cwd);
	}

	যদি (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		যদি (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	যদি (init_shared_repository != -1)
		repo_settings_set_shared_repository(the_repository, init_shared_repository);

	/*
	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR
	 * without --bare.  Catch the error early.
	 */
	git_dir = xstrdup_or_null(getenv(GIT_DIR_ENVIRONMENT));
	work_বৃক্ষ = xstrdup_or_null(getenv(GIT_WORK_বৃক্ষ_ENVIRONMENT));
	যদি ((!git_dir || bare == 1) && work_বৃক্ষ)
		die(_("%s (or --work-tree=<directory>) not allowed without "
			  "specifying %s (or --git-dir=<directory>)"),
		    GIT_WORK_বৃক্ষ_ENVIRONMENT,
		    GIT_DIR_ENVIRONMENT);

	/*
	 * Set up the default .git directory contents
	 */
	যদি (!git_dir)
		git_dir = xstrdup(DEFAULT_GIT_DIR_ENVIRONMENT);

	/*
	 * When --separate-git-dir is used inside a linked worktree, take
	 * care to ensure that the common .git/ directory is relocated, not
	 * the worktree-specific .git/worktrees/<id>/ directory.
	 */
	যদি (real_git_dir) {
		পূর্ণাঙ্ক err;
		ধ্রুৱ ৱর্ণ *p;
		সংরচনা strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		যদি (p && get_common_dir(&sb, p)) {
			সংরচনা strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			যদি (chdir(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			মুক্ত(git_dir);
			git_dir = strbuf_detach(&sb, রিক্ত);
		}
		strbuf_release(&sb);
	}

	যদি (bare < 0)
		bare = guess_repository_type(git_dir);

	যদি (!bare) {
		ধ্রুৱ ৱর্ণ *git_dir_parent = strrchr(git_dir, '/');

		যদি (!work_বৃক্ষ) {
			যদি (git_dir_parent) {
				ৱর্ণ *rel = xstrndup(git_dir, git_dir_parent - git_dir);
				work_বৃক্ষ = real_pathdup(rel, 1);
				মুক্ত(rel);
			} অন্যথা {
				work_বৃক্ষ = xgetcwd();
			}
		}

		যদি (access(work_বৃক্ষ, X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_বৃক্ষ);
	} অন্যথা যদি (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_বৃক্ষ,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, রিক্ত);

	যদি (!quiet) {
		পূর্ণাঙ্ক len = strlen(git_dir);

		যদি (reinit)
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Reinitialized existing shared Git repository in %s%s\n")
			       : _("Reinitialized existing Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
		অন্যথা
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Initialized empty shared Git repository in %s%s\n")
			       : _("Initialized empty Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
	}

	মুক্ত(template_dir_to_free);
	মুক্ত(real_git_dir_to_free);
	মুক্ত(work_বৃক্ষ);
	মুক্ত(git_dir);
	ঘূৰাই 0;
}
