#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "thread-utils.h"

#define BUILTIN_HOOK_RUN_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#define BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ list(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		ರಿಕ್ತ
	};
	ಸಂರಚನಾ string_list *head;
	ಸಂರಚನಾ string_list_item *item;
	ಧ್ರುವ ವರ್ಣ *hookname = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_terminator = '\n';
	ಪೂರ್ಣಾಙ್ಕ show_scope = 0;
	ಪೂರ್ಣಾಙ್ಕ allow_unknown = 0;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಸಂರಚನಾ option list_options[] = {
		OPT_SET_INT('z', ರಿಕ್ತ, &ಸಾಲು_terminator,
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
	ಯದಿ (argc != 1)
		usage_ಸಂದೇಶ_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	ಯದಿ (!allow_unknown && !is_known_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	head = list_hooks(repo, hookname, ರಿಕ್ತ);

	ಯದಿ (!head->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		ಗಚ್ಛ cleanup;
	}

	for_each_string_list_item(item, head) {
		ಸಂರಚನಾ hook *h = item->util;

		ವಿಕಲ್ಪ (h->kind) {
		ಪಕ್ಷ HOOK_TRADITIONAL:
			printf("%s%c", _("hook from hookdir"), ಸಾಲು_terminator);
			ಭಙ್ಗ;
		ಪಕ್ಷ HOOK_CONFIGURED: {
			ಧ್ರುವ ವರ್ಣ *name = h->u.configured.friendly_name;
			ಧ್ರುವ ವರ್ಣ *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : ರಿಕ್ತ;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			ಧ್ರುವ ವರ್ಣ *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			ಯದಿ (scope)
				printf("%s\t%s%s%c", scope, disability, name,
				       ಸಾಲು_terminator);
			ಅನ್ಯಥಾ
				printf("%s%s%c", disability, name,
				       ಸಾಲು_terminator);
			ಭಙ್ಗ;
		}
		ಸ್ವಭಾವ:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(head, hook_free);
	ಮುಕ್ತ(head);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ಪೂರ್ಣಾಙ್ಕ ignore_missing = 0;
	ಪೂರ್ಣಾಙ್ಕ allow_unknown = 0;
	ಪೂರ್ಣಾಙ್ಕ jobs = 0;
	ಧ್ರುವ ವರ್ಣ *hook_name;
	ಸಂರಚನಾ option run_options[] = {
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
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, run_options,
			     builtin_hook_run_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ಯದಿ (jobs == -1)
		opt.jobs = online_cpus();
	ಅನ್ಯಥಾ ಯದಿ (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	ಅನ್ಯಥಾ
		opt.jobs = jobs;

	ಯದಿ (!argc)
		ಗಚ್ಛ usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	ಯದಿ (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		ಗಚ್ಛ usage;

	/* Add our arguments, start after -- */
	ಪ್ರತಿ (i = 2 ; i < argc; i++)
		strvec_ತಳ್ಳು(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	hook_name = argv[0];

	ಯದಿ (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಯದಿ (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	ಯದಿ (ret < 0) /* error() return */
		ret = 1;
	ಪ್ರತ್ಯಾಗಮ ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

ಪೂರ್ಣಾಙ್ಕ cmd_hook(ಪೂರ್ಣಾಙ್ಕ argc,
	     ಧ್ರುವ ವರ್ಣ **argv,
	     ಧ್ರುವ ವರ್ಣ *prefix,
	     ಸಂರಚನಾ repository *repo)
{
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, ರಿಕ್ತ, builtin_hook_options,
			     builtin_hook_usage, 0);

	ಪ್ರತ್ಯಾಗಮ fn(argc, argv, prefix, repo);
}
