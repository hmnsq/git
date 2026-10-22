/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "strbuf.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ guess_repository_type(സ്ഥിരം അക്ഷരം *git_dir)
{
	സ്ഥിരം അക്ഷരം *slash;
	അക്ഷരം *cwd;
	പൂർണ്ണസംഖ്യ cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	എങ്കിൽ (!strcmp(".", git_dir))
		തിരികെ 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !strcmp(git_dir, cwd);
	സ്വതന്ത്രം(cwd);
	എങ്കിൽ (cwd_is_git_dir)
		തിരികെ 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	എങ്കിൽ (!strcmp(git_dir, ".git"))
		തിരികെ 0;
	slash = strrchr(git_dir, '/');
	എങ്കിൽ (slash && !strcmp(slash, "/.git"))
		തിരികെ 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ shared_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	*((പൂർണ്ണസംഖ്യ *) opt->മൂല്യം) = (arg) ? git_config_perm("arg", arg) : PERM_GROUP;
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	ശൂന്യ
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
പൂർണ്ണസംഖ്യ cmd_init_db(പൂർണ്ണസംഖ്യ argc,
		സ്ഥിരം അക്ഷരം **argv,
		സ്ഥിരം അക്ഷരം *prefix,
		ഘടന repository *repo UNUSED)
{
	അക്ഷരം *git_dir;
	സ്ഥിരം അക്ഷരം *real_git_dir = ശൂന്യ;
	അക്ഷരം *real_git_dir_to_free = ശൂന്യ;
	അക്ഷരം *work_വൃക്ഷം = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *template_dir = ശൂന്യ;
	അക്ഷരം *template_dir_to_free = ശൂന്യ;
	പൂർണ്ണസംഖ്യ quiet = 0;
	പൂർണ്ണസംഖ്യ bare = startup_info->force_bare_repository ? 1 : -1;
	സ്ഥിരം അക്ഷരം *object_format = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *ref_storage_format_uri = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *initial_branch = ശൂന്യ;
	പൂർണ്ണസംഖ്യ hash_algo = GIT_HASH_UNKNOWN;
	പൂർണ്ണസംഖ്യ init_shared_repository = -1;
	സ്ഥിരം ഘടന option init_db_options[] = {
		OPT_STRING(0, "template", &template_dir, N_("template-directory"),
				N_("directory from which templates will be used")),
		OPT_SET_INT(0, "bare", &bare,
				N_("create a bare repository"), 1),
		{
			.type = OPTION_CALLBACK,
			.long_name = "shared",
			.മൂല്യം = &init_shared_repository,
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
	പൂർണ്ണസംഖ്യ reinit;

	argc = parse_options(argc, argv, prefix, init_db_options, init_db_usage, 0);

	എങ്കിൽ (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	എങ്കിൽ (real_git_dir && !is_absolute_path(real_git_dir))
		real_git_dir = real_git_dir_to_free = real_pathdup(real_git_dir, 1);

	എങ്കിൽ (template_dir && *template_dir && !is_absolute_path(template_dir))
		template_dir = template_dir_to_free = absolute_pathdup(template_dir);

	എങ്കിൽ (argc == 1) {
		പൂർണ്ണസംഖ്യ mkdir_tried = 0;
	retry:
		എങ്കിൽ (chdir(argv[0]) < 0) {
			എങ്കിൽ (!mkdir_tried) {
				പൂർണ്ണസംഖ്യ saved;
				/*
				 * At this point we haven't read any configuration,
				 * and we know shared_repository should always be 0;
				 * but just in case we play safe.
				 */
				saved = repo_settings_get_shared_repository(the_repository);
				repo_settings_set_shared_repository(the_repository, 0);
				തിരഞ്ഞെടുപ്പ് (safe_create_leading_directories_const(the_repository, argv[0])) {
				അവസ്ഥ SCLD_OK:
				അവസ്ഥ SCLD_PERMS:
					നിർത്തുക;
				അവസ്ഥ SCLD_EXISTS:
					errno = EEXIST;
					/* fallthru */
				സ്ഥിരസ്ഥിതി:
					die_errno(_("cannot mkdir %s"), argv[0]);
					നിർത്തുക;
				}
				repo_settings_set_shared_repository(the_repository, saved);
				എങ്കിൽ (mkdir(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				പോകുക retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (0 < argc) {
		usage(init_db_usage[0]);
	}
	എങ്കിൽ (bare == 1) {
		അക്ഷരം *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		സ്വതന്ത്രം(cwd);
	}

	എങ്കിൽ (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		എങ്കിൽ (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	എങ്കിൽ (init_shared_repository != -1)
		repo_settings_set_shared_repository(the_repository, init_shared_repository);

	/*
	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR
	 * without --bare.  Catch the error early.
	 */
	git_dir = xstrdup_or_null(getenv(GIT_DIR_ENVIRONMENT));
	work_വൃക്ഷം = xstrdup_or_null(getenv(GIT_WORK_വൃക്ഷം_ENVIRONMENT));
	എങ്കിൽ ((!git_dir || bare == 1) && work_വൃക്ഷം)
		die(_("%s (or --work-tree=<directory>) not allowed without "
			  "specifying %s (or --git-dir=<directory>)"),
		    GIT_WORK_വൃക്ഷം_ENVIRONMENT,
		    GIT_DIR_ENVIRONMENT);

	/*
	 * Set up the default .git directory contents
	 */
	എങ്കിൽ (!git_dir)
		git_dir = xstrdup(DEFAULT_GIT_DIR_ENVIRONMENT);

	/*
	 * When --separate-git-dir is used inside a linked worktree, take
	 * care to ensure that the common .git/ directory is relocated, not
	 * the worktree-specific .git/worktrees/<id>/ directory.
	 */
	എങ്കിൽ (real_git_dir) {
		പൂർണ്ണസംഖ്യ err;
		സ്ഥിരം അക്ഷരം *p;
		ഘടന strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		എങ്കിൽ (p && get_common_dir(&sb, p)) {
			ഘടന strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			എങ്കിൽ (chdir(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			സ്വതന്ത്രം(git_dir);
			git_dir = strbuf_detach(&sb, ശൂന്യ);
		}
		strbuf_release(&sb);
	}

	എങ്കിൽ (bare < 0)
		bare = guess_repository_type(git_dir);

	എങ്കിൽ (!bare) {
		സ്ഥിരം അക്ഷരം *git_dir_parent = strrchr(git_dir, '/');

		എങ്കിൽ (!work_വൃക്ഷം) {
			എങ്കിൽ (git_dir_parent) {
				അക്ഷരം *rel = xstrndup(git_dir, git_dir_parent - git_dir);
				work_വൃക്ഷം = real_pathdup(rel, 1);
				സ്വതന്ത്രം(rel);
			} അല്ലെങ്കിൽ {
				work_വൃക്ഷം = xgetcwd();
			}
		}

		എങ്കിൽ (access(work_വൃക്ഷം, X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_വൃക്ഷം);
	} അല്ലെങ്കിൽ എങ്കിൽ (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_വൃക്ഷം,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, ശൂന്യ);

	എങ്കിൽ (!quiet) {
		പൂർണ്ണസംഖ്യ len = strlen(git_dir);

		എങ്കിൽ (reinit)
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Reinitialized existing shared Git repository in %s%s\n")
			       : _("Reinitialized existing Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
		അല്ലെങ്കിൽ
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Initialized empty shared Git repository in %s%s\n")
			       : _("Initialized empty Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
	}

	സ്വതന്ത്രം(template_dir_to_free);
	സ്വതന്ത്രം(real_git_dir_to_free);
	സ്വതന്ത്രം(work_വൃക്ഷം);
	സ്വതന്ത്രം(git_dir);
	തിരികെ 0;
}
