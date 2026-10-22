#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hook.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "thread-utils.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_HOOK_ਲੜੀਵਾਰ_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_hook_usage[] = {
	BUILTIN_HOOK_ਲੜੀਵਾਰ_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_hook_ਲੜੀਵਾਰ_usage[] = {
	BUILTIN_HOOK_ਲੜੀਵਾਰ_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ list(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		ਨਿਰਾ
	};
	ਬਣਤਰ string_list *ਸਿਰ;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਅਚਲ ਅੱਖਰ *hookname = ਨਿਰਾ;
	ਪੂਰਨ ਸਤਰ_terminator = '\n';
	ਪੂਰਨ show_scope = 0;
	ਪੂਰਨ allow_unknown = 0;
	ਪੂਰਨ ret = 0;

	ਬਣਤਰ option list_options[] = {
		OPT_ਸੈੱਟ_INT('z', ਨਿਰਾ, &ਸਤਰ_terminator,
			    N_("use NUL as line terminator"), '\0'),
		OPT_BOOL(0, "show-scope", &show_scope,
			 N_("show the config scope that defined each hook")),
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_ਸਿਰਾ(),
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, list_options,
			     builtin_hook_list_usage, 0);

	/*
	 * The only unnamed argument provided should be the hook-name; if we add
	 * arguments later they probably should be caught by parse_options.
	 */
	ਜੇ (argc != 1)
		usage_ਸੁਨੇਹਾ_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	ਜੇ (!allow_unknown && !is_ਗਿਆਤ_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		ਵਾਪਸ 1;
	}

	ਸਿਰ = list_hooks(repo, hookname, ਨਿਰਾ);

	ਜੇ (!ਸਿਰ->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		ਜਾਓ cleanup;
	}

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, ਸਿਰ) {
		ਬਣਤਰ hook *h = ਵਸਤੂ->util;

		ਚੋਣ (h->ਕਿਸਮ) {
		ਹਾਲਤ HOOK_TRADITIONAL:
			ਲਿਖੋ("%s%c", _("hook from hookdir"), ਸਤਰ_terminator);
			ਰੋਕੋ;
		ਹਾਲਤ HOOK_CONFIGURED: {
			ਅਚਲ ਅੱਖਰ *name = h->u.configured.friendly_name;
			ਅਚਲ ਅੱਖਰ *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : ਨਿਰਾ;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			ਅਚਲ ਅੱਖਰ *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			ਜੇ (scope)
				ਲਿਖੋ("%s\t%s%s%c", scope, disability, name,
				       ਸਤਰ_terminator);
			ਨਹੀਂਤਾਂ
				ਲਿਖੋ("%s%s%c", disability, name,
				       ਸਤਰ_terminator);
			ਰੋਕੋ;
		}
		ਮੂਲ:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(ਸਿਰ, hook_free);
	ਮੁਕਤ(ਸਿਰ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i;
	ਬਣਤਰ ਲੜੀਵਾਰ_hooks_opt opt = ਲੜੀਵਾਰ_HOOKS_OPT_INIT;
	ਪੂਰਨ ignore_missing = 0;
	ਪੂਰਨ allow_unknown = 0;
	ਪੂਰਨ jobs = 0;
	ਅਚਲ ਅੱਖਰ *hook_name;
	ਬਣਤਰ option ਲੜੀਵਾਰ_options[] = {
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_BOOL(0, "ignore-missing", &ignore_missing,
			 N_("silently ignore missing requested <hook-name>")),
		OPT_STRING(0, "to-stdin", &opt.ਰਸਤਾ_ਮੰਜ਼ਿਲ_stdin, N_("path"),
			   N_("file to read into hooks' stdin")),
		OPT_INTEGER('j', "jobs", &jobs,
			    N_("run up to <n> hooks simultaneously (-1 for CPU count)")),
		OPT_ਸਿਰਾ(),
	};
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, ਲੜੀਵਾਰ_options,
			     builtin_hook_ਲੜੀਵਾਰ_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ਜੇ (jobs == -1)
		opt.jobs = online_cpus();
	ਨਹੀਂਤਾਂ ਜੇ (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	ਨਹੀਂਤਾਂ
		opt.jobs = jobs;

	ਜੇ (!argc)
		ਜਾਓ usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	ਜੇ (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		ਜਾਓ usage;

	/* Add our arguments, start after -- */
	ਲਈ (i = 2 ; i < argc; i++)
		strvec_ਧੱਕੋ(&opt.ਦਲੀਲਾਂ, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	hook_name = argv[0];

	ਜੇ (!allow_unknown && !is_ਗਿਆਤ_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		ਵਾਪਸ 1;
	}

	ਜੇ (!ignore_missing)
		opt.error_if_missing = 1;
	ret = ਲੜੀਵਾਰ_hooks_opt(the_repository, hook_name, &opt);
	ਜੇ (ret < 0) /* error() return */
		ret = 1;
	ਵਾਪਸ ret;
usage:
	usage_with_options(builtin_hook_ਲੜੀਵਾਰ_usage, ਲੜੀਵਾਰ_options);
}

ਪੂਰਨ cmd_hook(ਪੂਰਨ argc,
	     ਅਚਲ ਅੱਖਰ **argv,
	     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	     ਬਣਤਰ repository *repo)
{
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, ਲੜੀਵਾਰ),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_ਸਿਰਾ(),
	};

	argc = parse_options(argc, argv, ਨਿਰਾ, builtin_hook_options,
			     builtin_hook_usage, 0);

	ਵਾਪਸ fn(argc, argv, ਅਗੇਤਰ, repo);
}
