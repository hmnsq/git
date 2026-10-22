#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hook.h"
#समावेश "parse-options.h"
#समावेश "thread-utils.h"

#घोषणा BUILTIN_HOOK_RUN_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#घोषणा BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

अटल स्थिर अक्षर * स्थिर builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	शून्य
};

अटल पूर्णांक list(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		 काष्ठा repository *repo)
{
	अटल स्थिर अक्षर *स्थिर builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		शून्य
	};
	काष्ठा string_list *शीर्ष;
	काष्ठा string_list_item *item;
	स्थिर अक्षर *hookname = शून्य;
	पूर्णांक पंक्ति_terminator = '\n';
	पूर्णांक show_scope = 0;
	पूर्णांक allow_unknown = 0;
	पूर्णांक ret = 0;

	काष्ठा option list_options[] = {
		OPT_SET_INT('z', शून्य, &पंक्ति_terminator,
			    N_("use NUL as line terminator"), '\0'),
		OPT_BOOL(0, "show-scope", &show_scope,
			 N_("show the config scope that defined each hook")),
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, list_options,
			     builtin_hook_list_usage, 0);

	/*
	 * The only unnamed argument provided should be the hook-name; if we add
	 * arguments later they probably should be caught by parse_options.
	 */
	अगर (argc != 1)
		usage_संदेश_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	अगर (!allow_unknown && !is_known_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		वापस 1;
	}

	शीर्ष = list_hooks(repo, hookname, शून्य);

	अगर (!शीर्ष->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		जाओ cleanup;
	}

	for_each_string_list_item(item, शीर्ष) {
		काष्ठा hook *h = item->util;

		चयन (h->kind) {
		हाल HOOK_TRADITIONAL:
			म_लिखो("%s%c", _("hook from hookdir"), पंक्ति_terminator);
			अवरोध;
		हाल HOOK_CONFIGURED: {
			स्थिर अक्षर *name = h->u.configured.friendly_name;
			स्थिर अक्षर *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : शून्य;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			स्थिर अक्षर *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			अगर (scope)
				म_लिखो("%s\t%s%s%c", scope, disability, name,
				       पंक्ति_terminator);
			अन्यथा
				म_लिखो("%s%s%c", disability, name,
				       पंक्ति_terminator);
			अवरोध;
		}
		शेष:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(शीर्ष, hook_free);
	मुक्त(शीर्ष);
	वापस ret;
}

अटल पूर्णांक run(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
	       काष्ठा repository *repo UNUSED)
{
	पूर्णांक i;
	काष्ठा run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	पूर्णांक ignore_missing = 0;
	पूर्णांक allow_unknown = 0;
	पूर्णांक jobs = 0;
	स्थिर अक्षर *hook_name;
	काष्ठा option run_options[] = {
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_BOOL(0, "ignore-missing", &ignore_missing,
			 N_("silently ignore missing requested <hook-name>")),
		OPT_STRING(0, "to-stdin", &opt.path_to_stdin, N_("path"),
			   N_("file to read into hooks' stdin")),
		OPT_INTEGER('j', "jobs", &jobs,
			    N_("run up to <n> hooks simultaneously (-1 for CPU count)")),
		OPT_END(),
	};
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, run_options,
			     builtin_hook_run_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	अगर (jobs == -1)
		opt.jobs = online_cpus();
	अन्यथा अगर (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	अन्यथा
		opt.jobs = jobs;

	अगर (!argc)
		जाओ usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	अगर (argc > 1 && म_भेद(argv[1], "--") &&
	    म_भेद(argv[1], "--end-of-options"))
		जाओ usage;

	/* Add our arguments, start after -- */
	क्रम (i = 2 ; i < argc; i++)
		strvec_धकेलो(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, शून्य);

	hook_name = argv[0];

	अगर (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		वापस 1;
	}

	अगर (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	अगर (ret < 0) /* error() return */
		ret = 1;
	वापस ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

पूर्णांक cmd_hook(पूर्णांक argc,
	     स्थिर अक्षर **argv,
	     स्थिर अक्षर *prefix,
	     काष्ठा repository *repo)
{
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, शून्य, builtin_hook_options,
			     builtin_hook_usage, 0);

	वापस fn(argc, argv, prefix, repo);
}
