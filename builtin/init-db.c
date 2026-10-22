/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "strbuf.h"

સ્થિર પૂર્ણાંક guess_repository_type(અચલ અક્ષર *git_dir)
{
	અચલ અક્ષર *slash;
	અક્ષર *cwd;
	પૂર્ણાંક cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	જો (!strcmp(".", git_dir))
		પાછા 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !strcmp(git_dir, cwd);
	મુક્ત(cwd);
	જો (cwd_is_git_dir)
		પાછા 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	જો (!strcmp(git_dir, ".git"))
		પાછા 0;
	slash = strrchr(git_dir, '/');
	જો (slash && !strcmp(slash, "/.git"))
		પાછા 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	પાછા 1;
}

સ્થિર પૂર્ણાંક shared_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	*((પૂર્ણાંક *) opt->કિંમત) = (arg) ? git_config_perm("arg", arg) : PERM_GROUP;
	પાછા 0;
}

સ્થિર અચલ અક્ષર *અચલ init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	ખાલી
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
પૂર્ણાંક cmd_init_db(પૂર્ણાંક argc,
		અચલ અક્ષર **argv,
		અચલ અક્ષર *prefix,
		સંરચના repository *repo UNUSED)
{
	અક્ષર *git_dir;
	અચલ અક્ષર *real_git_dir = ખાલી;
	અક્ષર *real_git_dir_to_free = ખાલી;
	અક્ષર *work_વૃક્ષ = ખાલી;
	અચલ અક્ષર *template_dir = ખાલી;
	અક્ષર *template_dir_to_free = ખાલી;
	પૂર્ણાંક quiet = 0;
	પૂર્ણાંક bare = startup_info->force_bare_repository ? 1 : -1;
	અચલ અક્ષર *object_format = ખાલી;
	અચલ અક્ષર *ref_storage_format_uri = ખાલી;
	અચલ અક્ષર *initial_branch = ખાલી;
	પૂર્ણાંક hash_algo = GIT_HASH_UNKNOWN;
	પૂર્ણાંક init_shared_repository = -1;
	અચલ સંરચના option init_db_options[] = {
		OPT_STRING(0, "template", &template_dir, N_("template-directory"),
				N_("directory from which templates will be used")),
		OPT_SET_INT(0, "bare", &bare,
				N_("create a bare repository"), 1),
		{
			.type = OPTION_CALLBACK,
			.long_name = "shared",
			.કિંમત = &init_shared_repository,
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
	પૂર્ણાંક reinit;

	argc = parse_options(argc, argv, prefix, init_db_options, init_db_usage, 0);

	જો (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	જો (real_git_dir && !is_absolute_path(real_git_dir))
		real_git_dir = real_git_dir_to_free = real_pathdup(real_git_dir, 1);

	જો (template_dir && *template_dir && !is_absolute_path(template_dir))
		template_dir = template_dir_to_free = absolute_pathdup(template_dir);

	જો (argc == 1) {
		પૂર્ણાંક mkdir_tried = 0;
	retry:
		જો (chdir(argv[0]) < 0) {
			જો (!mkdir_tried) {
				પૂર્ણાંક saved;
				/*
				 * At this point we haven't read any configuration,
				 * and we know shared_repository should always be 0;
				 * but just in case we play safe.
				 */
				saved = repo_settings_get_shared_repository(the_repository);
				repo_settings_set_shared_repository(the_repository, 0);
				પસંદગી (safe_create_leading_directories_const(the_repository, argv[0])) {
				સ્થિતિ SCLD_OK:
				સ્થિતિ SCLD_PERMS:
					તોડો;
				સ્થિતિ SCLD_EXISTS:
					errno = EEXIST;
					/* fallthru */
				મૂળભૂત:
					die_errno(_("cannot mkdir %s"), argv[0]);
					તોડો;
				}
				repo_settings_set_shared_repository(the_repository, saved);
				જો (mkdir(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				જાઓ retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} નહીંતર જો (0 < argc) {
		usage(init_db_usage[0]);
	}
	જો (bare == 1) {
		અક્ષર *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		મુક્ત(cwd);
	}

	જો (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		જો (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	જો (init_shared_repository != -1)
		repo_settings_set_shared_repository(the_repository, init_shared_repository);

	/*
	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR
	 * without --bare.  Catch the error early.
	 */
	git_dir = xstrdup_or_null(getenv(GIT_DIR_ENVIRONMENT));
	work_વૃક્ષ = xstrdup_or_null(getenv(GIT_WORK_વૃક્ષ_ENVIRONMENT));
	જો ((!git_dir || bare == 1) && work_વૃક્ષ)
		die(_("%s (or --work-tree=<directory>) not allowed without "
			  "specifying %s (or --git-dir=<directory>)"),
		    GIT_WORK_વૃક્ષ_ENVIRONMENT,
		    GIT_DIR_ENVIRONMENT);

	/*
	 * Set up the default .git directory contents
	 */
	જો (!git_dir)
		git_dir = xstrdup(DEFAULT_GIT_DIR_ENVIRONMENT);

	/*
	 * When --separate-git-dir is used inside a linked worktree, take
	 * care to ensure that the common .git/ directory is relocated, not
	 * the worktree-specific .git/worktrees/<id>/ directory.
	 */
	જો (real_git_dir) {
		પૂર્ણાંક err;
		અચલ અક્ષર *p;
		સંરચના strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		જો (p && get_common_dir(&sb, p)) {
			સંરચના strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			જો (chdir(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			મુક્ત(git_dir);
			git_dir = strbuf_detach(&sb, ખાલી);
		}
		strbuf_release(&sb);
	}

	જો (bare < 0)
		bare = guess_repository_type(git_dir);

	જો (!bare) {
		અચલ અક્ષર *git_dir_parent = strrchr(git_dir, '/');

		જો (!work_વૃક્ષ) {
			જો (git_dir_parent) {
				અક્ષર *rel = xstrndup(git_dir, git_dir_parent - git_dir);
				work_વૃક્ષ = real_pathdup(rel, 1);
				મુક્ત(rel);
			} નહીંતર {
				work_વૃક્ષ = xgetcwd();
			}
		}

		જો (access(work_વૃક્ષ, X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_વૃક્ષ);
	} નહીંતર જો (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_વૃક્ષ,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, ખાલી);

	જો (!quiet) {
		પૂર્ણાંક len = strlen(git_dir);

		જો (reinit)
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Reinitialized existing shared Git repository in %s%s\n")
			       : _("Reinitialized existing Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
		નહીંતર
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Initialized empty shared Git repository in %s%s\n")
			       : _("Initialized empty Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
	}

	મુક્ત(template_dir_to_free);
	મુક્ત(real_git_dir_to_free);
	મુક્ત(work_વૃક્ષ);
	મુક્ત(git_dir);
	પાછા 0;
}
