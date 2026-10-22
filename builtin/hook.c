#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "thread-utils.h"

#define BUILTIN_HOOK_RUN_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#define BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	रिक्त
};

स्थिर पूर्णांक list(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		 संरचना repository *repo)
{
	स्थिर स्थिरांक अक्षर *स्थिरांक builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		रिक्त
	};
	संरचना string_list *head;
	संरचना string_list_item *item;
	स्थिरांक अक्षर *hookname = रिक्त;
	पूर्णांक ओळ_terminator = '\n';
	पूर्णांक show_scope = 0;
	पूर्णांक allow_unknown = 0;
	पूर्णांक ret = 0;

	संरचना option list_options[] = {
		OPT_SET_INT('z', रिक्त, &ओळ_terminator,
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
	जर (argc != 1)
		usage_संदेश_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	जर (!allow_unknown && !is_known_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		परत 1;
	}

	head = list_hooks(repo, hookname, रिक्त);

	जर (!head->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		जा cleanup;
	}

	for_each_string_list_item(item, head) {
		संरचना hook *h = item->util;

		निवड (h->kind) {
		स्थिती HOOK_TRADITIONAL:
			म_लिखो("%s%c", _("hook from hookdir"), ओळ_terminator);
			थांबा;
		स्थिती HOOK_CONFIGURED: {
			स्थिरांक अक्षर *name = h->u.configured.friendly_name;
			स्थिरांक अक्षर *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : रिक्त;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			स्थिरांक अक्षर *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			जर (scope)
				म_लिखो("%s\t%s%s%c", scope, disability, name,
				       ओळ_terminator);
			अन्यथा
				म_लिखो("%s%s%c", disability, name,
				       ओळ_terminator);
			थांबा;
		}
		पूर्वनिर्धारित:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(head, hook_free);
	मुक्त(head);
	परत ret;
}

स्थिर पूर्णांक run(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
	       संरचना repository *repo UNUSED)
{
	पूर्णांक i;
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	पूर्णांक ignore_missing = 0;
	पूर्णांक allow_unknown = 0;
	पूर्णांक jobs = 0;
	स्थिरांक अक्षर *hook_name;
	संरचना option run_options[] = {
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

	जर (jobs == -1)
		opt.jobs = online_cpus();
	अन्यथा जर (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	अन्यथा
		opt.jobs = jobs;

	जर (!argc)
		जा usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	जर (argc > 1 && म_भेद(argv[1], "--") &&
	    म_भेद(argv[1], "--end-of-options"))
		जा usage;

	/* Add our arguments, start after -- */
	साठी (i = 2 ; i < argc; i++)
		strvec_ढकला(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, रिक्त);

	hook_name = argv[0];

	जर (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		परत 1;
	}

	जर (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	जर (ret < 0) /* error() return */
		ret = 1;
	परत ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

पूर्णांक cmd_hook(पूर्णांक argc,
	     स्थिरांक अक्षर **argv,
	     स्थिरांक अक्षर *prefix,
	     संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, रिक्त, builtin_hook_options,
			     builtin_hook_usage, 0);

	परत fn(argc, argv, prefix, repo);
}
