#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hook.h"
#ضمن "parse-options.h"
#ضمن "thread-utils.h"

#وصف_ڪريو BUILTIN_HOOK_لاڳيتو_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#وصف_ڪريو BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

ساكن ثابت حرف * ثابت builtin_hook_usage[] = {
	BUILTIN_HOOK_لاڳيتو_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت builtin_hook_لاڳيتو_usage[] = {
	BUILTIN_HOOK_لاڳيتو_USAGE,
	عدم
};

ساكن صحيح list(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		 بنية repository *repo)
{
	ساكن ثابت حرف *ثابت builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		عدم
	};
	بنية string_list *سر;
	بنية string_list_شي *شي;
	ثابت حرف *hookname = عدم;
	صحيح سٽ_terminator = '\n';
	صحيح show_scope = 0;
	صحيح allow_unknown = 0;
	صحيح ret = 0;

	بنية option list_options[] = {
		OPT_سيٽ_INT('z', عدم, &سٽ_terminator,
			    N_("use NUL as line terminator"), '\0'),
		OPT_BOOL(0, "show-scope", &show_scope,
			 N_("show the config scope that defined each hook")),
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_پڇاڙي(),
	};

	argc = parse_options(argc, argv, اڳياڙي, list_options,
			     builtin_hook_list_usage, 0);

	/*
	 * The only unnamed argument provided should be the hook-name; if we add
	 * arguments later they probably should be caught by parse_options.
	 */
	إذا (argc != 1)
		usage_نياپو_opt(_("you must specify a hook event name to list"),
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

	for_each_string_list_شي(شي, سر) {
		بنية hook *h = شي->util;

		بدل (h->قسم) {
		حالة HOOK_TRADITIONAL:
			لکو("%s%c", _("hook from hookdir"), سٽ_terminator);
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
				لکو("%s\t%s%s%c", scope, disability, name,
				       سٽ_terminator);
			وإلا
				لکو("%s%s%c", disability, name,
				       سٽ_terminator);
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

ساكن صحيح لاڳيتو(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
	       بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح i;
	بنية لاڳيتو_hooks_opt opt = لاڳيتو_HOOKS_OPT_INIT;
	صحيح ignore_missing = 0;
	صحيح allow_unknown = 0;
	صحيح jobs = 0;
	ثابت حرف *hook_name;
	بنية option لاڳيتو_options[] = {
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_BOOL(0, "ignore-missing", &ignore_missing,
			 N_("silently ignore missing requested <hook-name>")),
		OPT_STRING(0, "to-stdin", &opt.رستو_منزل_stdin, N_("path"),
			   N_("file to read into hooks' stdin")),
		OPT_INTEGER('j', "jobs", &jobs,
			    N_("run up to <n> hooks simultaneously (-1 for CPU count)")),
		OPT_پڇاڙي(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, اڳياڙي, لاڳيتو_options,
			     builtin_hook_لاڳيتو_usage,
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
		strvec_ڌڪو(&opt.دليلون, argv[i]);

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
	ret = لاڳيتو_hooks_opt(the_repository, hook_name, &opt);
	إذا (ret < 0) /* error() return */
		ret = 1;
	أرجع ret;
usage:
	usage_with_options(builtin_hook_لاڳيتو_usage, لاڳيتو_options);
}

صحيح cmd_hook(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *اڳياڙي,
	     بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, لاڳيتو),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_پڇاڙي(),
	};

	argc = parse_options(argc, argv, عدم, builtin_hook_options,
			     builtin_hook_usage, 0);

	أرجع fn(argc, argv, اڳياڙي, repo);
}
