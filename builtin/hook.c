#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hook.h"
#شامل "parse-options.h"
#شامل "thread-utils.h"

#define BUILTIN_HOOK_RUN_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#define BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

ایستا ثابت نویسه * ثابت builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	پوچ
};

ایستا صحیح list(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		 ساختار repository *repo)
{
	ایستا ثابت نویسه *ثابت builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		پوچ
	};
	ساختار string_list *head;
	ساختار string_list_item *item;
	ثابت نویسه *hookname = پوچ;
	صحیح سطر_terminator = '\n';
	صحیح show_scope = 0;
	صحیح allow_unknown = 0;
	صحیح ret = 0;

	ساختار option list_options[] = {
		OPT_SET_INT('z', پوچ, &سطر_terminator,
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
	اگر (argc != 1)
		usage_پیام_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	اگر (!allow_unknown && !is_known_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		بازگشت 1;
	}

	head = list_hooks(repo, hookname, پوچ);

	اگر (!head->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		برو cleanup;
	}

	for_each_string_list_item(item, head) {
		ساختار hook *h = item->util;

		انتخاب (h->kind) {
		حالت HOOK_TRADITIONAL:
			printf("%s%c", _("hook from hookdir"), سطر_terminator);
			بشکن;
		حالت HOOK_CONFIGURED: {
			ثابت نویسه *name = h->u.configured.friendly_name;
			ثابت نویسه *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : پوچ;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			ثابت نویسه *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			اگر (scope)
				printf("%s\t%s%s%c", scope, disability, name,
				       سطر_terminator);
			وگرنه
				printf("%s%s%c", disability, name,
				       سطر_terminator);
			بشکن;
		}
		پیشفرض:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(head, hook_free);
	آزاد(head);
	بازگشت ret;
}

ایستا صحیح run(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
	       ساختار repository *repo UNUSED)
{
	صحیح i;
	ساختار run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	صحیح ignore_missing = 0;
	صحیح allow_unknown = 0;
	صحیح jobs = 0;
	ثابت نویسه *hook_name;
	ساختار option run_options[] = {
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
	صحیح ret;

	argc = parse_options(argc, argv, prefix, run_options,
			     builtin_hook_run_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	اگر (jobs == -1)
		opt.jobs = online_cpus();
	وگرنه اگر (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	وگرنه
		opt.jobs = jobs;

	اگر (!argc)
		برو usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	اگر (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		برو usage;

	/* Add our arguments, start after -- */
	برای (i = 2 ; i < argc; i++)
		strvec_افزودن(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, پوچ);

	hook_name = argv[0];

	اگر (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		بازگشت 1;
	}

	اگر (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	اگر (ret < 0) /* error() return */
		ret = 1;
	بازگشت ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

صحیح cmd_hook(صحیح argc,
	     ثابت نویسه **argv,
	     ثابت نویسه *prefix,
	     ساختار repository *repo)
{
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, پوچ, builtin_hook_options,
			     builtin_hook_usage, 0);

	بازگشت fn(argc, argv, prefix, repo);
}
