#परिभाषय USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "thread-utils.h"

#परिभाषय BUILTIN_HOOK_सततम्_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#परिभाषय BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

स्थिर ध्रुव वर्ण * ध्रुव builtin_hook_usage[] = {
	BUILTIN_HOOK_सततम्_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_hook_सततम्_usage[] = {
	BUILTIN_HOOK_सततम्_USAGE,
	रिक्त
};

स्थिर पूर्णाङ्क list(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo)
{
	स्थिर ध्रुव वर्ण *ध्रुव builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		रिक्त
	};
	संरचना string_list *शीर्ष;
	संरचना string_list_item *item;
	ध्रुव वर्ण *hookname = रिक्त;
	पूर्णाङ्क पङ्क्तिः_terminator = '\n';
	पूर्णाङ्क show_scope = 0;
	पूर्णाङ्क allow_unknown = 0;
	पूर्णाङ्क ret = 0;

	संरचना option list_options[] = {
		OPT_समुच्चयः_INT('z', रिक्त, &पङ्क्तिः_terminator,
			    N_("use NUL as line terminator"), '\0'),
		OPT_BOOL(0, "show-scope", &show_scope,
			 N_("show the config scope that defined each hook")),
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_अन्तः(),
	};

	argc = parse_options(argc, argv, उपसर्गः, list_options,
			     builtin_hook_list_usage, 0);

	/*
	 * The only unnamed argument provided should be the hook-name; if we add
	 * arguments later they probably should be caught by parse_options.
	 */
	यदि (argc != 1)
		usage_सन्देशः_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	यदि (!allow_unknown && !is_ज्ञातम्_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		प्रत्यागम 1;
	}

	शीर्ष = list_hooks(repo, hookname, रिक्त);

	यदि (!शीर्ष->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		गच्छ cleanup;
	}

	for_each_string_list_item(item, शीर्ष) {
		संरचना hook *h = item->util;

		विकल्प (h->विधा) {
		पक्ष HOOK_TRADITIONAL:
			प्रारूप_लिख("%s%c", _("hook from hookdir"), पङ्क्तिः_terminator);
			भङ्ग;
		पक्ष HOOK_CONFIGURED: {
			ध्रुव वर्ण *name = h->u.configured.friendly_name;
			ध्रुव वर्ण *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : रिक्त;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			ध्रुव वर्ण *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			यदि (scope)
				प्रारूप_लिख("%s\t%s%s%c", scope, disability, name,
				       पङ्क्तिः_terminator);
			अन्यथा
				प्रारूप_लिख("%s%s%c", disability, name,
				       पङ्क्तिः_terminator);
			भङ्ग;
		}
		स्वभाव:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(शीर्ष, hook_free);
	मुक्त(शीर्ष);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क सततम्(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क i;
	संरचना सततम्_hooks_opt opt = सततम्_HOOKS_OPT_INIT;
	पूर्णाङ्क ignore_missing = 0;
	पूर्णाङ्क allow_unknown = 0;
	पूर्णाङ्क jobs = 0;
	ध्रुव वर्ण *hook_name;
	संरचना option सततम्_options[] = {
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_BOOL(0, "ignore-missing", &ignore_missing,
			 N_("silently ignore missing requested <hook-name>")),
		OPT_STRING(0, "to-stdin", &opt.मार्गः_गन्तव्यम्_stdin, N_("path"),
			   N_("file to read into hooks' stdin")),
		OPT_INTEGER('j', "jobs", &jobs,
			    N_("run up to <n> hooks simultaneously (-1 for CPU count)")),
		OPT_अन्तः(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, सततम्_options,
			     builtin_hook_सततम्_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	यदि (jobs == -1)
		opt.jobs = online_cpus();
	अन्यथा यदि (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	अन्यथा
		opt.jobs = jobs;

	यदि (!argc)
		गच्छ usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	यदि (argc > 1 && वाक्यं_तुलय(argv[1], "--") &&
	    वाक्यं_तुलय(argv[1], "--end-of-options"))
		गच्छ usage;

	/* Add our arguments, start after -- */
	प्रति (i = 2 ; i < argc; i++)
		strvec_प्रक्षिप(&opt.तर्काः, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, रिक्त);

	hook_name = argv[0];

	यदि (!allow_unknown && !is_ज्ञातम्_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		प्रत्यागम 1;
	}

	यदि (!ignore_missing)
		opt.error_if_missing = 1;
	ret = सततम्_hooks_opt(the_repository, hook_name, &opt);
	यदि (ret < 0) /* error() return */
		ret = 1;
	प्रत्यागम ret;
usage:
	usage_with_options(builtin_hook_सततम्_usage, सततम्_options);
}

पूर्णाङ्क cmd_hook(पूर्णाङ्क argc,
	     ध्रुव वर्ण **argv,
	     ध्रुव वर्ण *उपसर्गः,
	     संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, सततम्),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_अन्तः(),
	};

	argc = parse_options(argc, argv, रिक्त, builtin_hook_options,
			     builtin_hook_usage, 0);

	प्रत्यागम fn(argc, argv, उपसर्गः, repo);
}
