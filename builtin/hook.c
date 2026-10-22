#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hook.h"
#ضمن "parse-options.h"
#ضمن "thread-utils.h"

#تعریف_کن BUILTIN_HOOK_پیاپی_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#تعریف_کن BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

ساكن ثابت حرف * ثابت builtin_hook_usage[] = {
	BUILTIN_HOOK_پیاپی_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت builtin_hook_پیاپی_usage[] = {
	BUILTIN_HOOK_پیاپی_USAGE,
	عدم
};

ساكن صحيح list(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		 بنية repository *repo)
{
	ساكن ثابت حرف *ثابت builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		عدم
	};
	بنية string_list *سر;
	بنية string_list_قلم *قلم;
	ثابت حرف *hookname = عدم;
	صحيح سطر_terminator = '\n';
	صحيح show_scope = 0;
	صحيح allow_unknown = 0;
	صحيح ret = 0;

	بنية option list_options[] = {
		OPT_مجموعه_INT('z', عدم, &سطر_terminator,
			    N_("use NUL as line terminator"), '\0'),
		OPT_BOOL(0, "show-scope", &show_scope,
			 N_("show the config scope that defined each hook")),
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_انتها(),
	};

	argc = parse_options(argc, argv, پیشوند, list_options,
			     builtin_hook_list_usage, 0);

	/*
	 * The only unnamed argument provided should be the hook-name; if we add
	 * arguments later they probably should be caught by parse_options.
	 */
	إذا (argc != 1)
		usage_پیام_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	إذا (!allow_unknown && !is_معلوم_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		أرجع 1;
	}

	سر = list_hooks(repo, hookname, عدم);

	إذا (!سر->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		اذهب cleanup;
	}

	for_each_string_list_قلم(قلم, سر) {
		بنية hook *h = قلم->util;

		بدل (h->نوع) {
		حالة HOOK_TRADITIONAL:
			چاپ_کن("%s%c", _("hook from hookdir"), سطر_terminator);
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
				چاپ_کن("%s\t%s%s%c", scope, disability, name,
				       سطر_terminator);
			وإلا
				چاپ_کن("%s%s%c", disability, name,
				       سطر_terminator);
			اكسر;
		}
		افتراضي:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(سر, hook_free);
	حرر(سر);
	أرجع ret;
}

ساكن صحيح پیاپی(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
	       بنية repository *repo استفادهنشده)
{
	صحيح i;
	بنية پیاپی_hooks_opt opt = پیاپی_HOOKS_OPT_INIT;
	صحيح ignore_missing = 0;
	صحيح allow_unknown = 0;
	صحيح jobs = 0;
	ثابت حرف *hook_name;
	بنية option پیاپی_options[] = {
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_BOOL(0, "ignore-missing", &ignore_missing,
			 N_("silently ignore missing requested <hook-name>")),
		OPT_STRING(0, "to-stdin", &opt.مسیر_مقصد_stdin, N_("path"),
			   N_("file to read into hooks' stdin")),
		OPT_INTEGER('j', "jobs", &jobs,
			    N_("run up to <n> hooks simultaneously (-1 for CPU count)")),
		OPT_انتها(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, پیاپی_options,
			     builtin_hook_پیاپی_usage,
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
		strvec_افزودن(&opt.آرگومانها, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, عدم);

	hook_name = argv[0];

	إذا (!allow_unknown && !is_معلوم_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		أرجع 1;
	}

	إذا (!ignore_missing)
		opt.error_if_missing = 1;
	ret = پیاپی_hooks_opt(the_repository, hook_name, &opt);
	إذا (ret < 0) /* error() return */
		ret = 1;
	أرجع ret;
usage:
	usage_with_options(builtin_hook_پیاپی_usage, پیاپی_options);
}

صحيح cmd_hook(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *پیشوند,
	     بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, پیاپی),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_انتها(),
	};

	argc = parse_options(argc, argv, عدم, builtin_hook_options,
			     builtin_hook_usage, 0);

	أرجع fn(argc, argv, پیشوند, repo);
}
