#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "thread-utils.h"

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_HOOK_ಸತತ_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_hook_usage[] = {
	BUILTIN_HOOK_ಸತತ_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_hook_ಸತತ_usage[] = {
	BUILTIN_HOOK_ಸತತ_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ list(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		ಖಾಲಿ
	};
	ರಚನೆ string_list *ತಲೆ;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hookname = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ಸಾಲು_terminator = '\n';
	ಪೂರ್ಣಾಂಕ show_scope = 0;
	ಪೂರ್ಣಾಂಕ allow_unknown = 0;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ರಚನೆ option list_options[] = {
		OPT_ಗಣ_INT('z', ಖಾಲಿ, &ಸಾಲು_terminator,
			    N_("use NUL as line terminator"), '\0'),
		OPT_BOOL(0, "show-scope", &show_scope,
			 N_("show the config scope that defined each hook")),
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_ಕೊನೆ(),
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, list_options,
			     builtin_hook_list_usage, 0);

	/*
	 * The only unnamed argument provided should be the hook-name; if we add
	 * arguments later they probably should be caught by parse_options.
	 */
	ಆದರೆ (argc != 1)
		usage_ಸಂದೇಶ_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	ಆದರೆ (!allow_unknown && !is_ತಿಳಿದ_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		ಹಿಂತಿರುಗಿ 1;
	}

	ತಲೆ = list_hooks(repo, hookname, ಖಾಲಿ);

	ಆದರೆ (!ತಲೆ->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		ಹೋಗು cleanup;
	}

	for_each_string_list_ಅಂಶ(ಅಂಶ, ತಲೆ) {
		ರಚನೆ hook *h = ಅಂಶ->util;

		ಆಯ್ಕೆ (h->ಬಗೆ) {
		ಸ್ಥಿತಿ HOOK_TRADITIONAL:
			ಬರೆ("%s%c", _("hook from hookdir"), ಸಾಲು_terminator);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ HOOK_CONFIGURED: {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = h->u.configured.friendly_name;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : ಖಾಲಿ;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			ಆದರೆ (scope)
				ಬರೆ("%s\t%s%s%c", scope, disability, name,
				       ಸಾಲು_terminator);
			ಇಲ್ಲದಿದ್ದರೆ
				ಬರೆ("%s%s%c", disability, name,
				       ಸಾಲು_terminator);
			ನಿಲ್ಲಿಸು;
		}
		ಪೂರ್ವನಿಯೋಜಿತ:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(ತಲೆ, hook_free);
	ಬಿಡುಗಡೆ(ತಲೆ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ ಸತತ_hooks_opt opt = ಸತತ_HOOKS_OPT_INIT;
	ಪೂರ್ಣಾಂಕ ignore_missing = 0;
	ಪೂರ್ಣಾಂಕ allow_unknown = 0;
	ಪೂರ್ಣಾಂಕ jobs = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hook_name;
	ರಚನೆ option ಸತತ_options[] = {
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_BOOL(0, "ignore-missing", &ignore_missing,
			 N_("silently ignore missing requested <hook-name>")),
		OPT_STRING(0, "to-stdin", &opt.ಮಾರ್ಗ_ಗಮ್ಯ_stdin, N_("path"),
			   N_("file to read into hooks' stdin")),
		OPT_INTEGER('j', "jobs", &jobs,
			    N_("run up to <n> hooks simultaneously (-1 for CPU count)")),
		OPT_ಕೊನೆ(),
	};
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, ಸತತ_options,
			     builtin_hook_ಸತತ_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ಆದರೆ (jobs == -1)
		opt.jobs = online_cpus();
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	ಇಲ್ಲದಿದ್ದರೆ
		opt.jobs = jobs;

	ಆದರೆ (!argc)
		ಹೋಗು usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	ಆದರೆ (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		ಹೋಗು usage;

	/* Add our arguments, start after -- */
	ಗಾಗಿ (i = 2 ; i < argc; i++)
		strvec_ತಳ್ಳು(&opt.ವಾದಗಳು, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	hook_name = argv[0];

	ಆದರೆ (!allow_unknown && !is_ತಿಳಿದ_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (!ignore_missing)
		opt.error_if_missing = 1;
	ret = ಸತತ_hooks_opt(the_repository, hook_name, &opt);
	ಆದರೆ (ret < 0) /* error() return */
		ret = 1;
	ಹಿಂತಿರುಗಿ ret;
usage:
	usage_with_options(builtin_hook_ಸತತ_usage, ಸತತ_options);
}

ಪೂರ್ಣಾಂಕ cmd_hook(ಪೂರ್ಣಾಂಕ argc,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	     ರಚನೆ repository *repo)
{
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, ಸತತ),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_ಕೊನೆ(),
	};

	argc = parse_options(argc, argv, ಖಾಲಿ, builtin_hook_options,
			     builtin_hook_usage, 0);

	ಹಿಂತಿರುಗಿ fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
}
