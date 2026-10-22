/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "abspath.h"
#include "environment.h"
#include "gettext.h"
#include "parse-options.h"
#include "path.h"
#include "refs.h"
#include "setup.h"
#include "strbuf.h"

static puur_naa_nka guess_repository_type(const var_na *git_dir)
{
	const var_na *slash;
	var_na *cwd;
	puur_naa_nka cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	ᯀᯮᯝ (!strcmp(".", git_dir))
		ᯀᯮᯞ 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !strcmp(git_dir, cwd);
	free(cwd);
	ᯀᯮᯝ (cwd_is_git_dir)
		ᯀᯮᯞ 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	ᯀᯮᯝ (!strcmp(git_dir, ".git"))
		ᯀᯮᯞ 0;
	slash = strrchr(git_dir, '/');
	ᯀᯮᯝ (slash && !strcmp(slash, "/.git"))
		ᯀᯮᯞ 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	ᯀᯮᯞ 1;
}

static puur_naa_nka shared_callback(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	BUG_ON_OPT_NEG(unset);
	*((puur_naa_nka *) opt->value) = (arg) ? git_config_perm("arg", arg) : PERM_GROUP;
	ᯀᯮᯞ 0;
}

static const var_na *const init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	NULL
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
puur_naa_nka cmd_init_db(puur_naa_nka argc,
		const var_na **argv,
		const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	var_na *git_dir;
	const var_na *real_git_dir = NULL;
	var_na *real_git_dir_to_free = NULL;
	var_na *work_tree = NULL;
	const var_na *template_dir = NULL;
	var_na *template_dir_to_free = NULL;
	puur_naa_nka quiet = 0;
	puur_naa_nka bare = startup_info->force_bare_repository ? 1 : -1;
	const var_na *object_format = NULL;
	const var_na *ref_storage_format_uri = NULL;
	const var_na *initial_branch = NULL;
	puur_naa_nka hash_algo = GIT_HASH_UNKNOWN;
	puur_naa_nka init_shared_repository = -1;
	const sa_nracanaa option init_db_options[] = {
		OPT_STRING(0, "template", &template_dir, N_("template-directory"),
				N_("directory from which templates will be used")),
		OPT_SET_INT(0, "bare", &bare,
				N_("create a bare repository"), 1),
		{
			.type = OPTION_CALLBACK,
			.long_name = "shared",
			.value = &init_shared_repository,
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
	puur_naa_nka reinit;

	argc = parse_options(argc, argv, prefix, init_db_options, init_db_usage, 0);

	ᯀᯮᯝ (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	ᯀᯮᯝ (real_git_dir && !is_absolute_path(real_git_dir))
		real_git_dir = real_git_dir_to_free = real_pathdup(real_git_dir, 1);

	ᯀᯮᯝ (template_dir && *template_dir && !is_absolute_path(template_dir))
		template_dir = template_dir_to_free = absolute_pathdup(template_dir);

	ᯀᯮᯝ (argc == 1) {
		puur_naa_nka mkdir_tried = 0;
	retry:
		ᯀᯮᯝ (chdir(argv[0]) < 0) {
			ᯀᯮᯝ (!mkdir_tried) {
				puur_naa_nka saved;
				/*
				 * At this point we haven't read any configuration,
				 * and we know shared_repository should always be 0;
				 * but just in case we play safe.
				 */
				saved = repo_settings_get_shared_repository(the_repository);
				repo_settings_set_shared_repository(the_repository, 0);
				switch (safe_create_leading_directories_const(the_repository, argv[0])) {
				pak_sa SCLD_OK:
				pak_sa SCLD_PERMS:
					bha_nga;
				pak_sa SCLD_EXISTS:
					errno = EEXIST;
					/* fallthru */
				default:
					die_errno(_("cannot mkdir %s"), argv[0]);
					bha_nga;
				}
				repo_settings_set_shared_repository(the_repository, saved);
				ᯀᯮᯝ (mkdir(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				goto retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (0 < argc) {
		usage(init_db_usage[0]);
	}
	ᯀᯮᯝ (bare == 1) {
		var_na *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		free(cwd);
	}

	ᯀᯮᯝ (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		ᯀᯮᯝ (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	ᯀᯮᯝ (init_shared_repository != -1)
		repo_settings_set_shared_repository(the_repository, init_shared_repository);

	/*
	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR
	 * without --bare.  Catch the error early.
	 */
	git_dir = xstrdup_or_null(getenv(GIT_DIR_ENVIRONMENT));
	work_tree = xstrdup_or_null(getenv(GIT_WORK_TREE_ENVIRONMENT));
	ᯀᯮᯝ ((!git_dir || bare == 1) && work_tree)
		die(_("%s (or --work-tree=<directory>) not allowed without "
			  "specifying %s (or --git-dir=<directory>)"),
		    GIT_WORK_TREE_ENVIRONMENT,
		    GIT_DIR_ENVIRONMENT);

	/*
	 * Set up the default .git directory contents
	 */
	ᯀᯮᯝ (!git_dir)
		git_dir = xstrdup(DEFAULT_GIT_DIR_ENVIRONMENT);

	/*
	 * When --separate-git-dir is used inside a linked worktree, take
	 * care to ensure that the common .git/ directory is relocated, not
	 * the worktree-specific .git/worktrees/<id>/ directory.
	 */
	ᯀᯮᯝ (real_git_dir) {
		puur_naa_nka err;
		const var_na *p;
		sa_nracanaa strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		ᯀᯮᯝ (p && get_common_dir(&sb, p)) {
			sa_nracanaa strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			ᯀᯮᯝ (chdir(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			free(git_dir);
			git_dir = strbuf_detach(&sb, NULL);
		}
		strbuf_release(&sb);
	}

	ᯀᯮᯝ (bare < 0)
		bare = guess_repository_type(git_dir);

	ᯀᯮᯝ (!bare) {
		const var_na *git_dir_parent = strrchr(git_dir, '/');

		ᯀᯮᯝ (!work_tree) {
			ᯀᯮᯝ (git_dir_parent) {
				var_na *rel = xstrndup(git_dir, git_dir_parent - git_dir);
				work_tree = real_pathdup(rel, 1);
				free(rel);
			} ᯀᯮᯝᯂᯪ {
				work_tree = xgetcwd();
			}
		}

		ᯀᯮᯝ (access(work_tree, X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_tree);
	} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_tree,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, NULL);

	ᯀᯮᯝ (!quiet) {
		puur_naa_nka len = strlen(git_dir);

		ᯀᯮᯝ (reinit)
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Reinitialized existing shared Git repository in %s%s\n")
			       : _("Reinitialized existing Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
		ᯀᯮᯝᯂᯪ
			printf(repo_settings_get_shared_repository(the_repository)
			       ? _("Initialized empty shared Git repository in %s%s\n")
			       : _("Initialized empty Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
	}

	free(template_dir_to_free);
	free(real_git_dir_to_free);
	free(work_tree);
	free(git_dir);
	ᯀᯮᯞ 0;
}
