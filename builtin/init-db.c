/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "strbuf.h"

நிலையான முழு guess_repository_type(மாறிலி எழுத்து *git_dir)
{
	மாறிலி எழுத்து *slash;
	எழுத்து *cwd;
	முழு cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	எனில் (!strcmp(".", git_dir))
		திரும்பு 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !strcmp(git_dir, cwd);
	விடுவி(cwd);
	எனில் (cwd_is_git_dir)
		திரும்பு 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	எனில் (!strcmp(git_dir, ".git"))
		திரும்பு 0;
	slash = strrchr(git_dir, '/');
	எனில் (slash && !strcmp(slash, "/.git"))
		திரும்பு 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	திரும்பு 1;
}

நிலையான முழு shared_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	*((முழு *) opt->மதிப்பு) = (arg) ? git_config_perm("arg", arg) : PERM_GROUP;
	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *மாறிலி init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	வெற்று
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
முழு cmd_init_db(முழு argc,
		மாறிலி எழுத்து **argv,
		மாறிலி எழுத்து *prefix,
		கட்டமைப்பு repository *repo UNUSED)
{
	எழுத்து *git_dir;
	மாறிலி எழுத்து *real_git_dir = வெற்று;
	எழுத்து *real_git_dir_to_free = வெற்று;
	எழுத்து *work_மரம் = வெற்று;
	மாறிலி எழுத்து *template_dir = வெற்று;
	எழுத்து *template_dir_to_free = வெற்று;
	முழு quiet = 0;
	முழு bare = startup_info->force_bare_repository ? 1 : -1;
	மாறிலி எழுத்து *object_format = வெற்று;
	மாறிலி எழுத்து *ref_storage_format_uri = வெற்று;
	மாறிலி எழுத்து *initial_branch = வெற்று;
	முழு hash_algo = GIT_HASH_UNKNOWN;
	முழு init_shared_repository = -1;
	மாறிலி கட்டமைப்பு option init_db_options[] = {
		OPT_STRING(0, "template", &template_dir, N_("template-directory"),
				N_("directory from which templates will be used")),
		OPT_SET_INT(0, "bare", &bare,
				N_("create a bare repository"), 1),
		{
			.type = OPTION_CALLBACK,
			.long_name = "shared",
			.மதிப்பு = &init_shared_repository,
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
	முழு reinit;

	argc = parse_options(argc, argv, prefix, init_db_options, init_db_usage, 0);

	எனில் (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	எனில் (real_git_dir && !is_absolute_path(real_git_dir))
		real_git_dir = real_git_dir_to_free = real_pathdup(real_git_dir, 1);

	எனில் (template_dir && *template_dir && !is_absolute_path(template_dir))
		template_dir = template_dir_to_free = absolute_pathdup(template_dir);

	எனில் (argc == 1) {
		முழு mkdir_tried = 0;
	retry:
		எனில் (chdir(argv[0]) < 0) {
			எனில் (!mkdir_tried) {
				முழு saved;
				/*
				 * At this point we haven't read any configuration,
				 * and we know shared_repository should always be 0;
				 * but just in case we play safe.
				 */
				saved = repo_settings_get_shared_repository(the_repository);
				repo_settings_set_shared_repository(the_repository, 0);
				தேர்வு (safe_create_leading_directories_const(the_repository, argv[0])) {
				நிலை SCLD_OK:
				நிலை SCLD_PERMS:
					நிறுத்து;
				நிலை SCLD_EXISTS:
					errno = EEXIST;
					/* fallthru */
				இயல்பு:
					die_errno(_("cannot mkdir %s"), argv[0]);
					நிறுத்து;
				}
				repo_settings_set_shared_repository(the_repository, saved);
				எனில் (mkdir(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				செல் retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} இல்லையெனில் எனில் (0 < argc) {
		usage(init_db_usage[0]);
	}
	எனில் (bare == 1) {
		எழுத்து *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		விடுவி(cwd);
	}

	எனில் (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		எனில் (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	எனில் (init_shared_repository != -1)
		repo_settings_set_shared_repository(the_repository, init_shared_repository);

	/*
	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR
	 * without --bare.  Catch the error early.
	 */
	git_dir = xstrdup_or_null(getenv(GIT_DIR_ENVIRONMENT));
	work_மரம் = xstrdup_or_null(getenv(GIT_WORK_மரம்_ENVIRONMENT));
	எனில் ((!git_dir || bare == 1) && work_மரம்)
		die(_("%s (or --work-tree=<directory>) not allowed without "
			  "specifying %s (or --git-dir=<directory>)"),
		    GIT_WORK_மரம்_ENVIRONMENT,
		    GIT_DIR_ENVIRONMENT);

	/*
	 * Set up the default .git directory contents
	 */
	எனில் (!git_dir)
		git_dir = xstrdup(DEFAULT_GIT_DIR_ENVIRONMENT);

	/*
	 * When --separate-git-dir is used inside a linked worktree, take
	 * care to ensure that the common .git/ directory is relocated, not
	 * the worktree-specific .git/worktrees/<id>/ directory.
	 */
	எனில் (real_git_dir) {
		முழு err;
		மாறிலி எழுத்து *p;
		கட்டமைப்பு strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		எனில் (p && get_common_dir(&sb, p)) {
			கட்டமைப்பு strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			எனில் (chdir(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			விடுவி(git_dir);
			git_dir = strbuf_detach(&sb, வெற்று);
		}
		strbuf_release(&sb);
	}

	எனில் (bare < 0)
		bare = guess_repository_type(git_dir);

	எனில் (!bare) {
		மாறிலி எழுத்து *git_dir_parent = strrchr(git_dir, '/');

		எனில் (!work_மரம்) {
			எனில் (git_dir_parent) {
				எழுத்து *rel = xstrndup(git_dir, git_dir_parent - git_dir);
				work_மரம் = real_pathdup(rel, 1);
				விடுவி(rel);
			} இல்லையெனில் {
				work_மரம் = xgetcwd();
			}
		}

		எனில் (access(work_மரம், X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_மரம்);
	} இல்லையெனில் எனில் (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_மரம்,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, வெற்று);

	எனில் (!quiet) {
		முழு len = strlen(git_dir);

		எனில் (reinit)
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Reinitialized existing shared Git repository in %s%s\n")
			       : _("Reinitialized existing Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
		இல்லையெனில்
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Initialized empty shared Git repository in %s%s\n")
			       : _("Initialized empty Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
	}

	விடுவி(template_dir_to_free);
	விடுவி(real_git_dir_to_free);
	விடுவி(work_மரம்);
	விடுவி(git_dir);
	திரும்பு 0;
}
