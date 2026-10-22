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

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	ফাঁকা
};

স্থির পূর্ণসংখ্যা list(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		 কাঠামো repository *repo)
{
	স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		ফাঁকা
	};
	কাঠামো string_list *শীর্ষ;
	কাঠামো string_list_item *item;
	ধ্রুবক অক্ষর *hookname = ফাঁকা;
	পূর্ণসংখ্যা সারি_terminator = '\n';
	পূর্ণসংখ্যা show_scope = 0;
	পূর্ণসংখ্যা allow_unknown = 0;
	পূর্ণসংখ্যা ret = 0;

	কাঠামো option list_options[] = {
		OPT_SET_INT('z', ফাঁকা, &সারি_terminator,
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
		ফেরত 1;
	}

	শীর্ষ = list_hooks(repo, hookname, ফাঁকা);

	যদি (!শীর্ষ->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		যাও cleanup;
	}

	for_each_string_list_item(item, শীর্ষ) {
		কাঠামো hook *h = item->util;

		নির্বাচন (h->kind) {
		ক্ষেত্র HOOK_TRADITIONAL:
			printf("%s%c", _("hook from hookdir"), সারি_terminator);
			বিরতি;
		ক্ষেত্র HOOK_CONFIGURED: {
			ধ্রুবক অক্ষর *name = h->u.configured.friendly_name;
			ধ্রুবক অক্ষর *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : ফাঁকা;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			ধ্রুবক অক্ষর *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			যদি (scope)
				printf("%s\t%s%s%c", scope, disability, name,
				       সারি_terminator);
			অন্যথা
				printf("%s%s%c", disability, name,
				       সারি_terminator);
			বিরতি;
		}
		পূর্বনির্ধারিত:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(শীর্ষ, hook_free);
	মুক্ত(শীর্ষ);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা run(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
	       কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা i;
	কাঠামো run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	পূর্ণসংখ্যা ignore_missing = 0;
	পূর্ণসংখ্যা allow_unknown = 0;
	পূর্ণসংখ্যা jobs = 0;
	ধ্রুবক অক্ষর *hook_name;
	কাঠামো option run_options[] = {
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
	পূর্ণসংখ্যা ret;

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
		যাও usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	যদি (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		যাও usage;

	/* Add our arguments, start after -- */
	জন্য (i = 2 ; i < argc; i++)
		strvec_ঠেলো(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, ফাঁকা);

	hook_name = argv[0];

	যদি (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		ফেরত 1;
	}

	যদি (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	যদি (ret < 0) /* error() return */
		ret = 1;
	ফেরত ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

পূর্ণসংখ্যা cmd_hook(পূর্ণসংখ্যা argc,
	     ধ্রুবক অক্ষর **argv,
	     ধ্রুবক অক্ষর *prefix,
	     কাঠামো repository *repo)
{
	parse_opt_subcommand_fn *fn = ফাঁকা;
	কাঠামো option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, ফাঁকা, builtin_hook_options,
			     builtin_hook_usage, 0);

	ফেরত fn(argc, argv, prefix, repo);
}
