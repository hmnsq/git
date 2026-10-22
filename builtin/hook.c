#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hook.h"
#شامل "parse-options.h"
#شامل "thread-utils.h"

#تعریف_کرو BUILTIN_HOOK_تسلسل_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#تعریف_کرو BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

ساکن ثابت حرف * ثابت builtin_hook_usage[] = {
	BUILTIN_HOOK_تسلسل_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت builtin_hook_تسلسل_usage[] = {
	BUILTIN_HOOK_تسلسل_USAGE,
	تہی
};

ساکن صحیح list(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		 ساخت repository *repo)
{
	ساکن ثابت حرف *ثابت builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		تہی
	};
	ساخت string_list *سر;
	ساخت string_list_شے *شے;
	ثابت حرف *hookname = تہی;
	صحیح سطر_terminator = '\n';
	صحیح show_scope = 0;
	صحیح allow_unknown = 0;
	صحیح ret = 0;

	ساخت option list_options[] = {
		OPT_سیٹ_INT('z', تہی, &سطر_terminator,
			    N_("use NUL as line terminator"), '\0'),
		OPT_BOOL(0, "show-scope", &show_scope,
			 N_("show the config scope that defined each hook")),
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_انتہا(),
	};

	argc = parse_options(argc, argv, سابقہ, list_options,
			     builtin_hook_list_usage, 0);

	/*
	 * The only unnamed argument provided should be the hook-name; if we add
	 * arguments later they probably should be caught by parse_options.
	 */
	اگر (argc != 1)
		usage_پیغام_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	اگر (!allow_unknown && !is_معلوم_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		واپس 1;
	}

	سر = list_hooks(repo, hookname, تہی);

	اگر (!سر->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		جاؤ cleanup;
	}

	for_each_string_list_شے(شے, سر) {
		ساخت hook *h = شے->util;

		انتخاب (h->قسم) {
		حالت HOOK_TRADITIONAL:
			لکھو("%s%c", _("hook from hookdir"), سطر_terminator);
			توڑو;
		حالت HOOK_CONFIGURED: {
			ثابت حرف *name = h->u.configured.friendly_name;
			ثابت حرف *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : تہی;
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
			اگر (scope)
				لکھو("%s\t%s%s%c", scope, disability, name,
				       سطر_terminator);
			ورنہ
				لکھو("%s%s%c", disability, name,
				       سطر_terminator);
			توڑو;
		}
		طےشدہ:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(سر, hook_free);
	آزاد(سر);
	واپس ret;
}

ساکن صحیح تسلسل(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
	       ساخت repository *repo غیر_مستعمل)
{
	صحیح i;
	ساخت تسلسل_hooks_opt opt = تسلسل_HOOKS_OPT_INIT;
	صحیح ignore_missing = 0;
	صحیح allow_unknown = 0;
	صحیح jobs = 0;
	ثابت حرف *hook_name;
	ساخت option تسلسل_options[] = {
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_BOOL(0, "ignore-missing", &ignore_missing,
			 N_("silently ignore missing requested <hook-name>")),
		OPT_STRING(0, "to-stdin", &opt.راستہ_مقصد_stdin, N_("path"),
			   N_("file to read into hooks' stdin")),
		OPT_INTEGER('j', "jobs", &jobs,
			    N_("run up to <n> hooks simultaneously (-1 for CPU count)")),
		OPT_انتہا(),
	};
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, تسلسل_options,
			     builtin_hook_تسلسل_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	اگر (jobs == -1)
		opt.jobs = online_cpus();
	ورنہ اگر (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	ورنہ
		opt.jobs = jobs;

	اگر (!argc)
		جاؤ usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	اگر (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		جاؤ usage;

	/* Add our arguments, start after -- */
	برائے (i = 2 ; i < argc; i++)
		strvec_ڈالو(&opt.دلائل, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, تہی);

	hook_name = argv[0];

	اگر (!allow_unknown && !is_معلوم_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		واپس 1;
	}

	اگر (!ignore_missing)
		opt.error_if_missing = 1;
	ret = تسلسل_hooks_opt(the_repository, hook_name, &opt);
	اگر (ret < 0) /* error() return */
		ret = 1;
	واپس ret;
usage:
	usage_with_options(builtin_hook_تسلسل_usage, تسلسل_options);
}

صحیح cmd_hook(صحیح argc,
	     ثابت حرف **argv,
	     ثابت حرف *سابقہ,
	     ساخت repository *repo)
{
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, تسلسل),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_انتہا(),
	};

	argc = parse_options(argc, argv, تہی, builtin_hook_options,
			     builtin_hook_usage, 0);

	واپس fn(argc, argv, سابقہ, repo);
}
