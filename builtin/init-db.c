/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "strbuf.h"

स्थिर पूर्णाङ्क guess_repository_type(ध्रुव वर्ण *git_dir)
{
	ध्रुव वर्ण *slash;
	वर्ण *cwd;
	पूर्णाङ्क cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	यदि (!म_भेद(".", git_dir))
		प्रत्यागम 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !म_भेद(git_dir, cwd);
	मुक्त(cwd);
	यदि (cwd_is_git_dir)
		प्रत्यागम 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	यदि (!म_भेद(git_dir, ".git"))
		प्रत्यागम 0;
	slash = म_खोजप(git_dir, '/');
	यदि (slash && !म_भेद(slash, "/.git"))
		प्रत्यागम 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क shared_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	*((पूर्णाङ्क *) opt->मूल्य) = (arg) ? git_config_perm("arg", arg) : PERM_GROUP;
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *ध्रुव init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	रिक्त
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
पूर्णाङ्क cmd_init_db(पूर्णाङ्क argc,
		ध्रुव वर्ण **argv,
		ध्रुव वर्ण *prefix,
		संरचना repository *repo UNUSED)
{
	वर्ण *git_dir;
	ध्रुव वर्ण *real_git_dir = रिक्त;
	वर्ण *real_git_dir_to_free = रिक्त;
	वर्ण *work_वृक्ष = रिक्त;
	ध्रुव वर्ण *template_dir = रिक्त;
	वर्ण *template_dir_to_free = रिक्त;
	पूर्णाङ्क quiet = 0;
	पूर्णाङ्क bare = startup_info->force_bare_repository ? 1 : -1;
	ध्रुव वर्ण *object_format = रिक्त;
	ध्रुव वर्ण *ref_storage_format_uri = रिक्त;
	ध्रुव वर्ण *initial_branch = रिक्त;
	पूर्णाङ्क hash_algo = GIT_HASH_UNKNOWN;
	पूर्णाङ्क init_shared_repository = -1;
	ध्रुव संरचना option init_db_options[] = {
		OPT_STRING(0, "template", &template_dir, N_("template-directory"),
				N_("directory from which templates will be used")),
		OPT_SET_INT(0, "bare", &bare,
				N_("create a bare repository"), 1),
		{
			.type = OPTION_CALLBACK,
			.long_name = "shared",
			.मूल्य = &init_shared_repository,
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
	पूर्णाङ्क reinit;

	argc = parse_options(argc, argv, prefix, init_db_options, init_db_usage, 0);

	यदि (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	यदि (real_git_dir && !is_absolute_path(real_git_dir))
		real_git_dir = real_git_dir_to_free = real_pathdup(real_git_dir, 1);

	यदि (template_dir && *template_dir && !is_absolute_path(template_dir))
		template_dir = template_dir_to_free = absolute_pathdup(template_dir);

	यदि (argc == 1) {
		पूर्णाङ्क mkdir_tried = 0;
	retry:
		यदि (स_बदलो(argv[0]) < 0) {
			यदि (!mkdir_tried) {
				पूर्णाङ्क saved;
				/*
				 * At this point we haven't read any configuration,
				 * and we know shared_repository should always be 0;
				 * but just in case we play safe.
				 */
				saved = repo_settings_get_shared_repository(the_repository);
				repo_settings_set_shared_repository(the_repository, 0);
				विकल्प (safe_create_leading_directories_const(the_repository, argv[0])) {
				पक्ष SCLD_OK:
				पक्ष SCLD_PERMS:
					भङ्ग;
				पक्ष SCLD_EXISTS:
					त्रुटि_सं = EEXIST;
					/* fallthru */
				स्वभाव:
					die_errno(_("cannot mkdir %s"), argv[0]);
					भङ्ग;
				}
				repo_settings_set_shared_repository(the_repository, saved);
				यदि (सूची_गढ़ो(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				गच्छ retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} अन्यथा यदि (0 < argc) {
		usage(init_db_usage[0]);
	}
	यदि (bare == 1) {
		वर्ण *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		मुक्त(cwd);
	}

	यदि (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		यदि (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	यदि (init_shared_repository != -1)
		repo_settings_set_shared_repository(the_repository, init_shared_repository);

	/*
	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR
	 * without --bare.  Catch the error early.
	 */
	git_dir = xstrdup_or_null(दो_पर्या(GIT_DIR_ENVIRONMENT));
	work_वृक्ष = xstrdup_or_null(दो_पर्या(GIT_WORK_वृक्ष_ENVIRONMENT));
	यदि ((!git_dir || bare == 1) && work_वृक्ष)
		die(_("%s (or --work-tree=<directory>) not allowed without "
			  "specifying %s (or --git-dir=<directory>)"),
		    GIT_WORK_वृक्ष_ENVIRONMENT,
		    GIT_DIR_ENVIRONMENT);

	/*
	 * Set up the default .git directory contents
	 */
	यदि (!git_dir)
		git_dir = xstrdup(DEFAULT_GIT_DIR_ENVIRONMENT);

	/*
	 * When --separate-git-dir is used inside a linked worktree, take
	 * care to ensure that the common .git/ directory is relocated, not
	 * the worktree-specific .git/worktrees/<id>/ directory.
	 */
	यदि (real_git_dir) {
		पूर्णाङ्क err;
		ध्रुव वर्ण *p;
		संरचना strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		यदि (p && get_common_dir(&sb, p)) {
			संरचना strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			यदि (स_बदलो(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			मुक्त(git_dir);
			git_dir = strbuf_detach(&sb, रिक्त);
		}
		strbuf_release(&sb);
	}

	यदि (bare < 0)
		bare = guess_repository_type(git_dir);

	यदि (!bare) {
		ध्रुव वर्ण *git_dir_parent = म_खोजप(git_dir, '/');

		यदि (!work_वृक्ष) {
			यदि (git_dir_parent) {
				वर्ण *rel = xstrndup(git_dir, git_dir_parent - git_dir);
				work_वृक्ष = real_pathdup(rel, 1);
				मुक्त(rel);
			} अन्यथा {
				work_वृक्ष = xgetcwd();
			}
		}

		यदि (access(work_वृक्ष, X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_वृक्ष);
	} अन्यथा यदि (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_वृक्ष,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, रिक्त);

	यदि (!quiet) {
		पूर्णाङ्क len = म_माप(git_dir);

		यदि (reinit)
			म_लिखो(repo_settings_get_shared_repository(the_repository)
			       ? _("Reinitialized existing shared Git repository in %s%s\n")
			       : _("Reinitialized existing Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
		अन्यथा
			म_लिखो(repo_settings_get_shared_repository(the_repository)
			       ? _("Initialized empty shared Git repository in %s%s\n")
			       : _("Initialized empty Git repository in %s%s\n"),
			       git_dir, len && git_dir[len-1] != '/' ? "/" : "");
	}

	मुक्त(template_dir_to_free);
	मुक्त(real_git_dir_to_free);
	मुक्त(work_वृक्ष);
	मुक्त(git_dir);
	प्रत्यागम 0;
}
