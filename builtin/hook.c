#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hook.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "thread-utils.h"

#define BUILTIN_HOOK_RUN_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#define BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ list(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		ശൂന്യ
	};
	ഘടന string_list *head;
	ഘടന string_list_item *item;
	സ്ഥിരം അക്ഷരം *hookname = ശൂന്യ;
	പൂർണ്ണസംഖ്യ വരി_terminator = '\n';
	പൂർണ്ണസംഖ്യ show_scope = 0;
	പൂർണ്ണസംഖ്യ allow_unknown = 0;
	പൂർണ്ണസംഖ്യ ret = 0;

	ഘടന option list_options[] = {
		OPT_SET_INT('z', ശൂന്യ, &വരി_terminator,
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
	എങ്കിൽ (argc != 1)
		usage_സന്ദേശം_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	എങ്കിൽ (!allow_unknown && !is_known_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		തിരികെ 1;
	}

	head = list_hooks(repo, hookname, ശൂന്യ);

	എങ്കിൽ (!head->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		പോകുക cleanup;
	}

	for_each_string_list_item(item, head) {
		ഘടന hook *h = item->util;

		തിരഞ്ഞെടുപ്പ് (h->kind) {
		അവസ്ഥ HOOK_TRADITIONAL:
			printf("%s%c", _("hook from hookdir"), വരി_terminator);
			നിർത്തുക;
		അവസ്ഥ HOOK_CONFIGURED: {
			സ്ഥിരം അക്ഷരം *name = h->u.configured.friendly_name;
			സ്ഥിരം അക്ഷരം *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : ശൂന്യ;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			സ്ഥിരം അക്ഷരം *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			എങ്കിൽ (scope)
				printf("%s\t%s%s%c", scope, disability, name,
				       വരി_terminator);
			അല്ലെങ്കിൽ
				printf("%s%s%c", disability, name,
				       വരി_terminator);
			നിർത്തുക;
		}
		സ്ഥിരസ്ഥിതി:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(head, hook_free);
	സ്വതന്ത്രം(head);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	പൂർണ്ണസംഖ്യ ignore_missing = 0;
	പൂർണ്ണസംഖ്യ allow_unknown = 0;
	പൂർണ്ണസംഖ്യ jobs = 0;
	സ്ഥിരം അക്ഷരം *hook_name;
	ഘടന option run_options[] = {
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
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, run_options,
			     builtin_hook_run_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	എങ്കിൽ (jobs == -1)
		opt.jobs = online_cpus();
	അല്ലെങ്കിൽ എങ്കിൽ (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	അല്ലെങ്കിൽ
		opt.jobs = jobs;

	എങ്കിൽ (!argc)
		പോകുക usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	എങ്കിൽ (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		പോകുക usage;

	/* Add our arguments, start after -- */
	വേണ്ടി (i = 2 ; i < argc; i++)
		strvec_തള്ളുക(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, ശൂന്യ);

	hook_name = argv[0];

	എങ്കിൽ (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		തിരികെ 1;
	}

	എങ്കിൽ (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	എങ്കിൽ (ret < 0) /* error() return */
		ret = 1;
	തിരികെ ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

പൂർണ്ണസംഖ്യ cmd_hook(പൂർണ്ണസംഖ്യ argc,
	     സ്ഥിരം അക്ഷരം **argv,
	     സ്ഥിരം അക്ഷരം *prefix,
	     ഘടന repository *repo)
{
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, ശൂന്യ, builtin_hook_options,
			     builtin_hook_usage, 0);

	തിരികെ fn(argc, argv, prefix, repo);
}
