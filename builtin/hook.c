#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hook.h"
#ضمن "parse-options.h"
#ضمن "thread-utils.h"

#define BUILTIN_HOOK_RUN_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#define BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

ساكن ثابت حرف * ثابت builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	عدم
};

ساكن صحيح list(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		 بنية repository *repo)
{
	ساكن ثابت حرف *ثابت builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		عدم
	};
	بنية string_list *head;
	بنية string_list_item *item;
	ثابت حرف *hookname = عدم;
	صحيح قۇر_terminator = '\n';
	صحيح show_scope = 0;
	صحيح allow_unknown = 0;
	صحيح ret = 0;

	بنية option list_options[] = {
		OPT_SET_INT('z', عدم, &قۇر_terminator,
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
	إذا (argc != 1)
		usage_ئۇچۇر_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	إذا (!allow_unknown && !is_known_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		أرجع 1;
	}

	head = list_hooks(repo, hookname, عدم);

	إذا (!head->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		اذهب cleanup;
	}

	for_each_string_list_item(item, head) {
		بنية hook *h = item->util;

		بدل (h->kind) {
		حالة HOOK_TRADITIONAL:
			printf("%s%c", _("hook from hookdir"), قۇر_terminator);
			اكسر;
		حالة HOOK_CONFIGURED: {
			ثابت حرف *name = h->u.configured.friendly_name;
			ثابت حرف *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : عدم;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			ثابت حرف *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			إذا (scope)
				printf("%s\t%s%s%c", scope, disability, name,
				       قۇر_terminator);
			وإلا
				printf("%s%s%c", disability, name,
				       قۇر_terminator);
			اكسر;
		}
		افتراضي:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(head, hook_free);
	حرر(head);
	أرجع ret;
}

ساكن صحيح run(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
	       بنية repository *repo UNUSED)
{
	صحيح i;
	بنية run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	صحيح ignore_missing = 0;
	صحيح allow_unknown = 0;
	صحيح jobs = 0;
	ثابت حرف *hook_name;
	بنية option run_options[] = {
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
	صحيح ret;

	argc = parse_options(argc, argv, prefix, run_options,
			     builtin_hook_run_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (jobs == -1)
		opt.jobs = online_cpus();
	وإلا إذا (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	وإلا
		opt.jobs = jobs;

	إذا (!argc)
		اذهب usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	إذا (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		اذهب usage;

	/* Add our arguments, start after -- */
	لكل (i = 2 ; i < argc; i++)
		strvec_ئىتتىرىش(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, عدم);

	hook_name = argv[0];

	إذا (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		أرجع 1;
	}

	إذا (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	إذا (ret < 0) /* error() return */
		ret = 1;
	أرجع ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

صحيح cmd_hook(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *prefix,
	     بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, عدم, builtin_hook_options,
			     builtin_hook_usage, 0);

	أرجع fn(argc, argv, prefix, repo);
}
