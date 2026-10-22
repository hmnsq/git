/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "refs.h"
#समावेश "setup.h"
#समावेश "strbuf.h"

अटल पूर्णांक guess_repository_type(स्थिर अक्षर *git_dir)
{
	स्थिर अक्षर *slash;
	अक्षर *cwd;
	पूर्णांक cwd_is_git_dir;

	/*
	 * "GIT_DIR=. git init" is always bare.
	 * "GIT_DIR=`pwd` git init" too.
	 */
	अगर (!म_भेद(".", git_dir))
		वापस 1;
	cwd = xgetcwd();
	cwd_is_git_dir = !म_भेद(git_dir, cwd);
	मुक्त(cwd);
	अगर (cwd_is_git_dir)
		वापस 1;
	/*
	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not.
	 */
	अगर (!म_भेद(git_dir, ".git"))
		वापस 0;
	slash = म_खोजप(git_dir, '/');
	अगर (slash && !म_भेद(slash, "/.git"))
		वापस 0;

	/*
	 * Otherwise it is often bare.  At this point
	 * we are just guessing.
	 */
	वापस 1;
}

अटल पूर्णांक shared_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	*((पूर्णांक *) opt->मूल्य) = (arg) ? git_config_perm("arg", arg) : PERM_GROUP;
	वापस 0;
}

अटल स्थिर अक्षर *स्थिर init_db_usage[] = {
	N_("git init [-q | --quiet] [--bare] [--template=<template-directory>]\n"
	   "         [--separate-git-dir <git-dir>] [--object-format=<format>]\n"
	   "         [--ref-storage-format=<format>]\n"
	   "         [-b <branch-name> | --initial-branch=<branch-name>]\n"
	   "         [--shared[=<permissions>]] [<directory>]"),
	शून्य
};

/*
 * If you want to, you can share the DB area with any number of branches.
 * That has advantages: you can save space by sharing all the SHA1 objects.
 * On the other hand, it might just make lookup slower and messier. You
 * be the judge.  The default case is to have one DB per managed directory.
 */
पूर्णांक cmd_init_db(पूर्णांक argc,
		स्थिर अक्षर **argv,
		स्थिर अक्षर *prefix,
		काष्ठा repository *repo UNUSED)
{
	अक्षर *git_dir;
	स्थिर अक्षर *real_git_dir = शून्य;
	अक्षर *real_git_dir_to_free = शून्य;
	अक्षर *work_वृक्ष = शून्य;
	स्थिर अक्षर *template_dir = शून्य;
	अक्षर *template_dir_to_free = शून्य;
	पूर्णांक quiet = 0;
	पूर्णांक bare = startup_info->force_bare_repository ? 1 : -1;
	स्थिर अक्षर *object_format = शून्य;
	स्थिर अक्षर *ref_storage_format_uri = शून्य;
	स्थिर अक्षर *initial_branch = शून्य;
	पूर्णांक hash_algo = GIT_HASH_UNKNOWN;
	पूर्णांक init_shared_repository = -1;
	स्थिर काष्ठा option init_db_options[] = {
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
	पूर्णांक reinit;

	argc = parse_options(argc, argv, prefix, init_db_options, init_db_usage, 0);

	अगर (real_git_dir && bare == 1)
		die(_("options '%s' and '%s' cannot be used together"), "--separate-git-dir", "--bare");

	अगर (real_git_dir && !is_absolute_path(real_git_dir))
		real_git_dir = real_git_dir_to_free = real_pathdup(real_git_dir, 1);

	अगर (template_dir && *template_dir && !is_absolute_path(template_dir))
		template_dir = template_dir_to_free = absolute_pathdup(template_dir);

	अगर (argc == 1) {
		पूर्णांक mkdir_tried = 0;
	retry:
		अगर (स_बदलो(argv[0]) < 0) {
			अगर (!mkdir_tried) {
				पूर्णांक saved;
				/*
				 * At this point we haven't read any configuration,
				 * and we know shared_repository should always be 0;
				 * but just in case we play safe.
				 */
				saved = repo_settings_get_shared_repository(the_repository);
				repo_settings_set_shared_repository(the_repository, 0);
				चयन (safe_create_leading_directories_const(the_repository, argv[0])) {
				हाल SCLD_OK:
				हाल SCLD_PERMS:
					अवरोध;
				हाल SCLD_EXISTS:
					त्रुटि_सं = EEXIST;
					/* fallthru */
				शेष:
					die_errno(_("cannot mkdir %s"), argv[0]);
					अवरोध;
				}
				repo_settings_set_shared_repository(the_repository, saved);
				अगर (सूची_गढ़ो(argv[0], 0777) < 0)
					die_errno(_("cannot mkdir %s"), argv[0]);
				mkdir_tried = 1;
				जाओ retry;
			}
			die_errno(_("cannot chdir to %s"), argv[0]);
		}
	} अन्यथा अगर (0 < argc) {
		usage(init_db_usage[0]);
	}
	अगर (bare == 1) {
		अक्षर *cwd = xgetcwd();
		setenv(GIT_DIR_ENVIRONMENT, cwd, argc > 0);
		मुक्त(cwd);
	}

	अगर (object_format) {
		hash_algo = hash_algo_by_name(object_format);
		अगर (hash_algo == GIT_HASH_UNKNOWN)
			die(_("unknown hash algorithm '%s'"), object_format);
	}

	अगर (init_shared_repository != -1)
		repo_settings_set_shared_repository(the_repository, init_shared_repository);

	/*
	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR
	 * without --bare.  Catch the error early.
	 */
	git_dir = xstrdup_or_null(दो_पर्या(GIT_DIR_ENVIRONMENT));
	work_वृक्ष = xstrdup_or_null(दो_पर्या(GIT_WORK_वृक्ष_ENVIRONMENT));
	अगर ((!git_dir || bare == 1) && work_वृक्ष)
		die(_("%s (or --work-tree=<directory>) not allowed without "
			  "specifying %s (or --git-dir=<directory>)"),
		    GIT_WORK_वृक्ष_ENVIRONMENT,
		    GIT_DIR_ENVIRONMENT);

	/*
	 * Set up the default .git directory contents
	 */
	अगर (!git_dir)
		git_dir = xstrdup(DEFAULT_GIT_DIR_ENVIRONMENT);

	/*
	 * When --separate-git-dir is used inside a linked worktree, take
	 * care to ensure that the common .git/ directory is relocated, not
	 * the worktree-specific .git/worktrees/<id>/ directory.
	 */
	अगर (real_git_dir) {
		पूर्णांक err;
		स्थिर अक्षर *p;
		काष्ठा strbuf sb = STRBUF_INIT;

		p = read_gitfile_gently(git_dir, &err);
		अगर (p && get_common_dir(&sb, p)) {
			काष्ठा strbuf mainwt = STRBUF_INIT;

			strbuf_addbuf(&mainwt, &sb);
			strbuf_strip_suffix(&mainwt, "/.git");
			अगर (स_बदलो(mainwt.buf) < 0)
				die_errno(_("cannot chdir to %s"), mainwt.buf);
			strbuf_release(&mainwt);
			मुक्त(git_dir);
			git_dir = strbuf_detach(&sb, शून्य);
		}
		strbuf_release(&sb);
	}

	अगर (bare < 0)
		bare = guess_repository_type(git_dir);

	अगर (!bare) {
		स्थिर अक्षर *git_dir_parent = म_खोजप(git_dir, '/');

		अगर (!work_वृक्ष) {
			अगर (git_dir_parent) {
				अक्षर *rel = xstrndup(git_dir, git_dir_parent - git_dir);
				work_वृक्ष = real_pathdup(rel, 1);
				मुक्त(rel);
			} अन्यथा {
				work_वृक्ष = xgetcwd();
			}
		}

		अगर (access(work_वृक्ष, X_OK))
			die_errno (_("Cannot access work tree '%s'"), work_वृक्ष);
	} अन्यथा अगर (real_git_dir) {
		die(_("--separate-git-dir incompatible with bare repository"));
	}

	create_repository(the_repository, git_dir, real_git_dir, work_वृक्ष,
			  template_dir, hash_algo, ref_storage_format_uri,
			  init_shared_repository, &reinit);
	create_reference_database(the_repository, initial_branch, quiet);
	create_object_database(the_repository, शून्य);

	अगर (!quiet) {
		पूर्णांक len = म_माप(git_dir);

		अगर (reinit)
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
	वापस 0;
}
