#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hook.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "thread-utils.h"

#define BUILTIN_HOOK_RUN_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#define BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	রিক্ত
};

স্থির পূর্ণাঙ্ক list(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		 সংরচনা repository *repo)
{
	স্থির ধ্রুব বর্ণ *ধ্রুব builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		রিক্ত
	};
	সংরচনা string_list *head;
	সংরচনা string_list_item *item;
	ধ্রুব বর্ণ *hookname = রিক্ত;
	পূর্ণাঙ্ক সারি_terminator = '\n';
	পূর্ণাঙ্ক show_scope = 0;
	পূর্ণাঙ্ক allow_unknown = 0;
	পূর্ণাঙ্ক ret = 0;

	সংরচনা option list_options[] = {
		OPT_SET_INT('z', রিক্ত, &সারি_terminator,
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
	যদি (argc != 1)
		usage_বার্তা_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	যদি (!allow_unknown && !is_known_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		প্রত্যাগম 1;
	}

	head = list_hooks(repo, hookname, রিক্ত);

	যদি (!head->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		গচ্ছ cleanup;
	}

	for_each_string_list_item(item, head) {
		সংরচনা hook *h = item->util;

		বিকল্প (h->kind) {
		পক্ষ HOOK_TRADITIONAL:
			printf("%s%c", _("hook from hookdir"), সারি_terminator);
			ভঙ্গ;
		পক্ষ HOOK_CONFIGURED: {
			ধ্রুব বর্ণ *name = h->u.configured.friendly_name;
			ধ্রুব বর্ণ *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : রিক্ত;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			ধ্রুব বর্ণ *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			যদি (scope)
				printf("%s\t%s%s%c", scope, disability, name,
				       সারি_terminator);
			অন্যথা
				printf("%s%s%c", disability, name,
				       সারি_terminator);
			ভঙ্গ;
		}
		স্বভাব:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(head, hook_free);
	মুক্ত(head);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক run(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
	       সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক i;
	সংরচনা run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	পূর্ণাঙ্ক ignore_missing = 0;
	পূর্ণাঙ্ক allow_unknown = 0;
	পূর্ণাঙ্ক jobs = 0;
	ধ্রুব বর্ণ *hook_name;
	সংরচনা option run_options[] = {
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
	পূর্ণাঙ্ক ret;

	argc = parse_options(argc, argv, prefix, run_options,
			     builtin_hook_run_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	যদি (jobs == -1)
		opt.jobs = online_cpus();
	অন্যথা যদি (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	অন্যথা
		opt.jobs = jobs;

	যদি (!argc)
		গচ্ছ usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	যদি (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		গচ্ছ usage;

	/* Add our arguments, start after -- */
	প্রতি (i = 2 ; i < argc; i++)
		strvec_ঠেলো(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, রিক্ত);

	hook_name = argv[0];

	যদি (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		প্রত্যাগম 1;
	}

	যদি (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	যদি (ret < 0) /* error() return */
		ret = 1;
	প্রত্যাগম ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

পূর্ণাঙ্ক cmd_hook(পূর্ণাঙ্ক argc,
	     ধ্রুব বর্ণ **argv,
	     ধ্রুব বর্ণ *prefix,
	     সংরচনা repository *repo)
{
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, রিক্ত, builtin_hook_options,
			     builtin_hook_usage, 0);

	প্রত্যাগম fn(argc, argv, prefix, repo);
}
