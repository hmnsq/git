/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"

ਸਥਿਰ ਪੂਰਨ guess_repository_type(ਅਚਲ ਅੱਖਰ *git_dir)
{
	ਅਚਲ ਅੱਖਰ *slash;
	ਅੱਖਰ *cwd;
	ਪੂਰਨ cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	ਜੇ (!strcmp(".", git_dir))
		ਵਾਪਸ 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !strcmp(git_dir, cwd);
	ਮੁਕਤ(cwd);
	ਜੇ (cwd_is_git_dir)
		ਵਾਪਸ 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	ਜੇ (!strcmp(git_dir, ".git"))
		ਵਾਪਸ 0;
	slash = strrchr(git_dir, '/');
	ਜੇ (slash && !strcmp(slash, "/.git"))
		ਵਾਪਸ 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ shared_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	*((ਪੂਰਨ *) opt->ਮੁੱਲ) = (arg) ? git_config_perm("arg", arg) : PERM_ਸਮੂਹ;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	ਨਿਰਾ
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
ਪੂਰਨ cmd_init_db(ਪੂਰਨ argc,
		ਅਚਲ ਅੱਖਰ **argv,
		ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅੱਖਰ *git_dir;
	ਅਚਲ ਅੱਖਰ *real_git_dir = ਨਿਰਾ;
	ਅੱਖਰ *real_git_dir_ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਅੱਖਰ *work_ਰੁੱਖ = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *template_dir = ਨਿਰਾ;
	ਅੱਖਰ *template_dir_ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਪੂਰਨ quiet = 0;
	ਪੂਰਨ bare = startup_info->force_bare_repository ? 1 : -1;
	ਅਚਲ ਅੱਖਰ *object_format = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ref_storage_format_uri = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *initial_branch = ਨਿਰਾ;
	ਪੂਰਨ hash_algo = GIT_HASH_UNKNOWN;
	ਪੂਰਨ init_shared_repository = -1;
	ਅਚਲ ਬਣਤਰ option init_db_options[] = {
		OPT_STRING(0, "template", &template_dir, N_("template-directory"),
				N_("directory from which templates will be used")),
		OPT_ਸੈੱਟ_INT(0, "bare", &bare,
				N_("create a bare repository"), 1),
		{
			.type = OPTION_CALLBACK,
			.long_name = "shared",
			.ਮੁੱਲ = &init_shared_repository,
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
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ reinit;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, init_db_options, init_db_usage, 0);

	ਜੇ (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	ਜੇ (real_git_dir && !is_absolute_ਰਸਤਾ(real_git_dir))
		real_git_dir = real_git_dir_ਮੰਜ਼ਿਲ_free = real_pathdup(real_git_dir, 1);

	ਜੇ (template_dir && *template_dir && !is_absolute_ਰਸਤਾ(template_dir))
		template_dir = template_dir_ਮੰਜ਼ਿਲ_free = absolute_pathdup(template_dir);

	ਜੇ (argc == 1) {
		ਪੂਰਨ mkdir_tried = 0;
	retry:
		ਜੇ (chdir(argv[0]) < 0) {
			ਜੇ (!mkdir_tried) {
				ਪੂਰਨ saved;
				/*
				 * At this point we haven't read any configuration,
				 * and we know shared_repository should always be 0;
				 * but just in case we play safe.
				 */
				saved = repo_settings_get_shared_repository(the_repository);
				repo_settings_ਸੈੱਟ_shared_repository(the_repository, 0);
				ਚੋਣ (ਸੁਰੱਖਿਅਤ_create_leading_directories_const(the_repository, argv[0])) {
				ਹਾਲਤ SCLD_OK:
				ਹਾਲਤ SCLD_PERMS:
					ਰੋਕੋ;
				ਹਾਲਤ SCLD_EXISTS:
					errno = EEXIST;
					/* fallthru */
				ਮੂਲ:
					die_errno(_("cannot mkdir %s"), argv[0]);
					ਰੋਕੋ;
				}
				repo_settings_ਸੈੱਟ_shared_repository(the_repository, saved);
				ਜੇ (mkdir(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				ਜਾਓ retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} ਨਹੀਂਤਾਂ ਜੇ (0 < argc) {
		usage(init_db_usage[0]);
	}
	ਜੇ (bare == 1) {
		ਅੱਖਰ *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		ਮੁਕਤ(cwd);
	}

	ਜੇ (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		ਜੇ (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	ਜੇ (init_shared_repository != -1)
		repo_settings_ਸੈੱਟ_shared_repository(the_repository, init_shared_repository);

	/*
	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR
	 * without --bare.  Catch the error early.
	 */
	git_dir = xstrdup_or_null(getenv(GIT_DIR_ENVIRONMENT));
	work_ਰੁੱਖ = xstrdup_or_null(getenv(GIT_WORK_ਰੁੱਖ_ENVIRONMENT));
	ਜੇ ((!git_dir || bare == 1) && work_ਰੁੱਖ)
		die(_("%s (or --work-tree=<directory>) not allowed without "
			  "specifying %s (or --git-dir=<directory>)"),
		    GIT_WORK_ਰੁੱਖ_ENVIRONMENT,
		    GIT_DIR_ENVIRONMENT);

	/*
	 * Set up the default .git directory contents
	 */
	ਜੇ (!git_dir)
		git_dir = xstrdup(DEFAULT_GIT_DIR_ENVIRONMENT);

	/*
	 * When --separate-git-dir is used inside a linked worktree, take
	 * care to ensure that the common .git/ directory is relocated, not
	 * the worktree-specific .git/worktrees/<id>/ directory.
	 */
	ਜੇ (real_git_dir) {
		ਪੂਰਨ err;
		ਅਚਲ ਅੱਖਰ *p;
		ਬਣਤਰ strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		ਜੇ (p && get_common_dir(&sb, p)) {
			ਬਣਤਰ strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			ਜੇ (chdir(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			ਮੁਕਤ(git_dir);
			git_dir = strbuf_detach(&sb, ਨਿਰਾ);
		}
		strbuf_release(&sb);
	}

	ਜੇ (bare < 0)
		bare = guess_repository_type(git_dir);

	ਜੇ (!bare) {
		ਅਚਲ ਅੱਖਰ *git_dir_ਮਾਪਾ = strrchr(git_dir, '/');

		ਜੇ (!work_ਰੁੱਖ) {
			ਜੇ (git_dir_ਮਾਪਾ) {
				ਅੱਖਰ *rel = xstrndup(git_dir, git_dir_ਮਾਪਾ - git_dir);
				work_ਰੁੱਖ = real_pathdup(rel, 1);
				ਮੁਕਤ(rel);
			} ਨਹੀਂਤਾਂ {
				work_ਰੁੱਖ = xgetcwd();
			}
		}

		ਜੇ (access(work_ਰੁੱਖ, X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_ਰੁੱਖ);
	} ਨਹੀਂਤਾਂ ਜੇ (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_ਰੁੱਖ,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, ਨਿਰਾ);

	ਜੇ (!quiet) {
		ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(git_dir);

		ਜੇ (reinit)
			ਲਿਖੋ(repo_settings_get_shared_repository(the_repository)
			       ? _("Reinitialized existing shared Git repository in %s%s\n")
			       : _("Reinitialized existing Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
		ਨਹੀਂਤਾਂ
			ਲਿਖੋ(repo_settings_get_shared_repository(the_repository)
			       ? _("Initialized empty shared Git repository in %s%s\n")
			       : _("Initialized empty Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
	}

	ਮੁਕਤ(template_dir_ਮੰਜ਼ਿਲ_free);
	ਮੁਕਤ(real_git_dir_ਮੰਜ਼ਿਲ_free);
	ਮੁਕਤ(work_ਰੁੱਖ);
	ਮੁਕਤ(git_dir);
	ਵਾਪਸ 0;
}
