/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ guess_repository_type(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *git_dir)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *slash;
	ಅಕ್ಷರ *cwd;
	ಪೂರ್ಣಾಂಕ cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	ಆದರೆ (!strcmp(".", git_dir))
		ಹಿಂತಿರುಗಿ 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !strcmp(git_dir, cwd);
	ಬಿಡುಗಡೆ(cwd);
	ಆದರೆ (cwd_is_git_dir)
		ಹಿಂತಿರುಗಿ 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	ಆದರೆ (!strcmp(git_dir, ".git"))
		ಹಿಂತಿರುಗಿ 0;
	slash = strrchr(git_dir, '/');
	ಆದರೆ (slash && !strcmp(slash, "/.git"))
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ shared_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	*((ಪೂರ್ಣಾಂಕ *) opt->ಮೌಲ್ಯ) = (arg) ? git_config_perm("arg", arg) : PERM_ಗುಂಪು;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	ಖಾಲಿ
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
ಪೂರ್ಣಾಂಕ cmd_init_db(ಪೂರ್ಣಾಂಕ argc,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		ರಚನೆ repository *repo ಬಳಸದ)
{
	ಅಕ್ಷರ *git_dir;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *real_git_dir = ಖಾಲಿ;
	ಅಕ್ಷರ *real_git_dir_ಗಮ್ಯ_free = ಖಾಲಿ;
	ಅಕ್ಷರ *work_ವೃಕ್ಷ = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *template_dir = ಖಾಲಿ;
	ಅಕ್ಷರ *template_dir_ಗಮ್ಯ_free = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ಪೂರ್ಣಾಂಕ bare = startup_info->force_bare_repository ? 1 : -1;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_format = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref_storage_format_uri = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *initial_branch = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ hash_algo = GIT_HASH_UNKNOWN;
	ಪೂರ್ಣಾಂಕ init_shared_repository = -1;
	ಸ್ಥಿರಾಂಕ ರಚನೆ option init_db_options[] = {
		OPT_STRING(0, "template", &template_dir, N_("template-directory"),
				N_("directory from which templates will be used")),
		OPT_ಗಣ_INT(0, "bare", &bare,
				N_("create a bare repository"), 1),
		{
			.type = OPTION_CALLBACK,
			.long_name = "shared",
			.ಮೌಲ್ಯ = &init_shared_repository,
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
		OPT_ಕೊನೆ()
	};
	ಪೂರ್ಣಾಂಕ reinit;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, init_db_options, init_db_usage, 0);

	ಆದರೆ (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	ಆದರೆ (real_git_dir && !is_absolute_ಮಾರ್ಗ(real_git_dir))
		real_git_dir = real_git_dir_ಗಮ್ಯ_free = real_pathdup(real_git_dir, 1);

	ಆದರೆ (template_dir && *template_dir && !is_absolute_ಮಾರ್ಗ(template_dir))
		template_dir = template_dir_ಗಮ್ಯ_free = absolute_pathdup(template_dir);

	ಆದರೆ (argc == 1) {
		ಪೂರ್ಣಾಂಕ mkdir_tried = 0;
	retry:
		ಆದರೆ (chdir(argv[0]) < 0) {
			ಆದರೆ (!mkdir_tried) {
				ಪೂರ್ಣಾಂಕ saved;
				/*
				 * At this point we haven't read any configuration,
				 * and we know shared_repository should always be 0;
				 * but just in case we play safe.
				 */
				saved = repo_settings_get_shared_repository(the_repository);
				repo_settings_ಗಣ_shared_repository(the_repository, 0);
				ಆಯ್ಕೆ (ಸುರಕ್ಷಿತ_create_leading_directories_const(the_repository, argv[0])) {
				ಸ್ಥಿತಿ SCLD_OK:
				ಸ್ಥಿತಿ SCLD_PERMS:
					ನಿಲ್ಲಿಸು;
				ಸ್ಥಿತಿ SCLD_EXISTS:
					errno = EEXIST;
					/* fallthru */
				ಪೂರ್ವನಿಯೋಜಿತ:
					die_errno(_("cannot mkdir %s"), argv[0]);
					ನಿಲ್ಲಿಸು;
				}
				repo_settings_ಗಣ_shared_repository(the_repository, saved);
				ಆದರೆ (mkdir(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				ಹೋಗು retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 < argc) {
		usage(init_db_usage[0]);
	}
	ಆದರೆ (bare == 1) {
		ಅಕ್ಷರ *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		ಬಿಡುಗಡೆ(cwd);
	}

	ಆದರೆ (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		ಆದರೆ (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	ಆದರೆ (init_shared_repository != -1)
		repo_settings_ಗಣ_shared_repository(the_repository, init_shared_repository);

	/*
	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR
	 * without --bare.  Catch the error early.
	 */
	git_dir = xstrdup_or_null(getenv(GIT_DIR_ENVIRONMENT));
	work_ವೃಕ್ಷ = xstrdup_or_null(getenv(GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT));
	ಆದರೆ ((!git_dir || bare == 1) && work_ವೃಕ್ಷ)
		die(_("%s (or --work-tree=<directory>) not allowed without "
			  "specifying %s (or --git-dir=<directory>)"),
		    GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT,
		    GIT_DIR_ENVIRONMENT);

	/*
	 * Set up the default .git directory contents
	 */
	ಆದರೆ (!git_dir)
		git_dir = xstrdup(DEFAULT_GIT_DIR_ENVIRONMENT);

	/*
	 * When --separate-git-dir is used inside a linked worktree, take
	 * care to ensure that the common .git/ directory is relocated, not
	 * the worktree-specific .git/worktrees/<id>/ directory.
	 */
	ಆದರೆ (real_git_dir) {
		ಪೂರ್ಣಾಂಕ err;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
		ರಚನೆ strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		ಆದರೆ (p && get_common_dir(&sb, p)) {
			ರಚನೆ strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			ಆದರೆ (chdir(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			ಬಿಡುಗಡೆ(git_dir);
			git_dir = strbuf_detach(&sb, ಖಾಲಿ);
		}
		strbuf_release(&sb);
	}

	ಆದರೆ (bare < 0)
		bare = guess_repository_type(git_dir);

	ಆದರೆ (!bare) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *git_dir_ಪೋಷಕ = strrchr(git_dir, '/');

		ಆದರೆ (!work_ವೃಕ್ಷ) {
			ಆದರೆ (git_dir_ಪೋಷಕ) {
				ಅಕ್ಷರ *rel = xstrndup(git_dir, git_dir_ಪೋಷಕ - git_dir);
				work_ವೃಕ್ಷ = real_pathdup(rel, 1);
				ಬಿಡುಗಡೆ(rel);
			} ಇಲ್ಲದಿದ್ದರೆ {
				work_ವೃಕ್ಷ = xgetcwd();
			}
		}

		ಆದರೆ (access(work_ವೃಕ್ಷ, X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_ವೃಕ್ಷ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_ವೃಕ್ಷ,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, ಖಾಲಿ);

	ಆದರೆ (!quiet) {
		ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(git_dir);

		ಆದರೆ (reinit)
			ಬರೆ(repo_settings_get_shared_repository(the_repository)
			       ? _("Reinitialized existing shared Git repository in %s%s\n")
			       : _("Reinitialized existing Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
		ಇಲ್ಲದಿದ್ದರೆ
			ಬರೆ(repo_settings_get_shared_repository(the_repository)
			       ? _("Initialized empty shared Git repository in %s%s\n")
			       : _("Initialized empty Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
	}

	ಬಿಡುಗಡೆ(template_dir_ಗಮ್ಯ_free);
	ಬಿಡುಗಡೆ(real_git_dir_ಗಮ್ಯ_free);
	ಬಿಡುಗಡೆ(work_ವೃಕ್ಷ);
	ಬಿಡುಗಡೆ(git_dir);
	ಹಿಂತಿರುಗಿ 0;
}
