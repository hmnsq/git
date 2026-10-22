/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "strbuf.h"

స్థిర పూర్ణాంకం guess_repository_type(స్థిరరాశి అక్షరం *git_dir)
{
	స్థిరరాశి అక్షరం *slash;
	అక్షరం *cwd;
	పూర్ణాంకం cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	ఒకవేళ (!strcmp(".", git_dir))
		తిరిగి 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !strcmp(git_dir, cwd);
	విడుదల(cwd);
	ఒకవేళ (cwd_is_git_dir)
		తిరిగి 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	ఒకవేళ (!strcmp(git_dir, ".git"))
		తిరిగి 0;
	slash = strrchr(git_dir, '/');
	ఒకవేళ (slash && !strcmp(slash, "/.git"))
		తిరిగి 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	తిరిగి 1;
}

స్థిర పూర్ణాంకం shared_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	*((పూర్ణాంకం *) opt->విలువ) = (arg) ? git_config_perm("arg", arg) : PERM_సమూహం;
	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	ఖాళీ
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
పూర్ణాంకం cmd_init_db(పూర్ణాంకం argc,
		స్థిరరాశి అక్షరం **argv,
		స్థిరరాశి అక్షరం *ఉపసర్గ,
		నిర్మాణం repository *repo వాడని)
{
	అక్షరం *git_dir;
	స్థిరరాశి అక్షరం *real_git_dir = ఖాళీ;
	అక్షరం *real_git_dir_గమ్యం_free = ఖాళీ;
	అక్షరం *work_వృక్షం = ఖాళీ;
	స్థిరరాశి అక్షరం *template_dir = ఖాళీ;
	అక్షరం *template_dir_గమ్యం_free = ఖాళీ;
	పూర్ణాంకం quiet = 0;
	పూర్ణాంకం bare = startup_info->force_bare_repository ? 1 : -1;
	స్థిరరాశి అక్షరం *object_format = ఖాళీ;
	స్థిరరాశి అక్షరం *ref_storage_format_uri = ఖాళీ;
	స్థిరరాశి అక్షరం *initial_branch = ఖాళీ;
	పూర్ణాంకం hash_algo = GIT_HASH_UNKNOWN;
	పూర్ణాంకం init_shared_repository = -1;
	స్థిరరాశి నిర్మాణం option init_db_options[] = {
		OPT_STRING(0, "template", &template_dir, N_("template-directory"),
				N_("directory from which templates will be used")),
		OPT_సమితి_INT(0, "bare", &bare,
				N_("create a bare repository"), 1),
		{
			.type = OPTION_CALLBACK,
			.long_name = "shared",
			.విలువ = &init_shared_repository,
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
		OPT_చివర()
	};
	పూర్ణాంకం reinit;

	argc = parse_options(argc, argv, ఉపసర్గ, init_db_options, init_db_usage, 0);

	ఒకవేళ (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	ఒకవేళ (real_git_dir && !is_absolute_మార్గం(real_git_dir))
		real_git_dir = real_git_dir_గమ్యం_free = real_pathdup(real_git_dir, 1);

	ఒకవేళ (template_dir && *template_dir && !is_absolute_మార్గం(template_dir))
		template_dir = template_dir_గమ్యం_free = absolute_pathdup(template_dir);

	ఒకవేళ (argc == 1) {
		పూర్ణాంకం mkdir_tried = 0;
	retry:
		ఒకవేళ (chdir(argv[0]) < 0) {
			ఒకవేళ (!mkdir_tried) {
				పూర్ణాంకం saved;
				/*
				 * At this point we haven't read any configuration,
				 * and we know shared_repository should always be 0;
				 * but just in case we play safe.
				 */
				saved = repo_settings_get_shared_repository(the_repository);
				repo_settings_సమితి_shared_repository(the_repository, 0);
				ఎంపిక (సురక్షిత_create_leading_directories_const(the_repository, argv[0])) {
				స్థితి SCLD_OK:
				స్థితి SCLD_PERMS:
					ఆపు;
				స్థితి SCLD_EXISTS:
					errno = EEXIST;
					/* fallthru */
				అప్రమేయ:
					die_errno(_("cannot mkdir %s"), argv[0]);
					ఆపు;
				}
				repo_settings_సమితి_shared_repository(the_repository, saved);
				ఒకవేళ (mkdir(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				వెళ్ళు retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} లేకపోతే ఒకవేళ (0 < argc) {
		usage(init_db_usage[0]);
	}
	ఒకవేళ (bare == 1) {
		అక్షరం *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		విడుదల(cwd);
	}

	ఒకవేళ (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		ఒకవేళ (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	ఒకవేళ (init_shared_repository != -1)
		repo_settings_సమితి_shared_repository(the_repository, init_shared_repository);

	/*
	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR
	 * without --bare.  Catch the error early.
	 */
	git_dir = xstrdup_or_null(getenv(GIT_DIR_ENVIRONMENT));
	work_వృక్షం = xstrdup_or_null(getenv(GIT_WORK_వృక్షం_ENVIRONMENT));
	ఒకవేళ ((!git_dir || bare == 1) && work_వృక్షం)
		die(_("%s (or --work-tree=<directory>) not allowed without "
			  "specifying %s (or --git-dir=<directory>)"),
		    GIT_WORK_వృక్షం_ENVIRONMENT,
		    GIT_DIR_ENVIRONMENT);

	/*
	 * Set up the default .git directory contents
	 */
	ఒకవేళ (!git_dir)
		git_dir = xstrdup(DEFAULT_GIT_DIR_ENVIRONMENT);

	/*
	 * When --separate-git-dir is used inside a linked worktree, take
	 * care to ensure that the common .git/ directory is relocated, not
	 * the worktree-specific .git/worktrees/<id>/ directory.
	 */
	ఒకవేళ (real_git_dir) {
		పూర్ణాంకం err;
		స్థిరరాశి అక్షరం *p;
		నిర్మాణం strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		ఒకవేళ (p && get_common_dir(&sb, p)) {
			నిర్మాణం strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			ఒకవేళ (chdir(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			విడుదల(git_dir);
			git_dir = strbuf_detach(&sb, ఖాళీ);
		}
		strbuf_release(&sb);
	}

	ఒకవేళ (bare < 0)
		bare = guess_repository_type(git_dir);

	ఒకవేళ (!bare) {
		స్థిరరాశి అక్షరం *git_dir_మాతృ = strrchr(git_dir, '/');

		ఒకవేళ (!work_వృక్షం) {
			ఒకవేళ (git_dir_మాతృ) {
				అక్షరం *rel = xstrndup(git_dir, git_dir_మాతృ - git_dir);
				work_వృక్షం = real_pathdup(rel, 1);
				విడుదల(rel);
			} లేకపోతే {
				work_వృక్షం = xgetcwd();
			}
		}

		ఒకవేళ (access(work_వృక్షం, X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_వృక్షం);
	} లేకపోతే ఒకవేళ (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_వృక్షం,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, ఖాళీ);

	ఒకవేళ (!quiet) {
		పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(git_dir);

		ఒకవేళ (reinit)
			రాయి(repo_settings_get_shared_repository(the_repository)
			       ? _("Reinitialized existing shared Git repository in %s%s\n")
			       : _("Reinitialized existing Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
		లేకపోతే
			రాయి(repo_settings_get_shared_repository(the_repository)
			       ? _("Initialized empty shared Git repository in %s%s\n")
			       : _("Initialized empty Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
	}

	విడుదల(template_dir_గమ్యం_free);
	విడుదల(real_git_dir_గమ్యం_free);
	విడుదల(work_వృక్షం);
	విడుదల(git_dir);
	తిరిగి 0;
}
