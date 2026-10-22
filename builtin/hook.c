#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hook.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "thread-utils.h"

#define BUILTIN_HOOK_RUN_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#define BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

நிலையான மாறிலி எழுத்து * மாறிலி builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	வெற்று
};

நிலையான முழு list(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo)
{
	நிலையான மாறிலி எழுத்து *மாறிலி builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		வெற்று
	};
	கட்டமைப்பு string_list *head;
	கட்டமைப்பு string_list_item *item;
	மாறிலி எழுத்து *hookname = வெற்று;
	முழு வரி_terminator = '\n';
	முழு show_scope = 0;
	முழு allow_unknown = 0;
	முழு ret = 0;

	கட்டமைப்பு option list_options[] = {
		OPT_SET_INT('z', வெற்று, &வரி_terminator,
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
	எனில் (argc != 1)
		usage_செய்தி_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	எனில் (!allow_unknown && !is_known_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		திரும்பு 1;
	}

	head = list_hooks(repo, hookname, வெற்று);

	எனில் (!head->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		செல் cleanup;
	}

	for_each_string_list_item(item, head) {
		கட்டமைப்பு hook *h = item->util;

		தேர்வு (h->kind) {
		நிலை HOOK_TRADITIONAL:
			printf("%s%c", _("hook from hookdir"), வரி_terminator);
			நிறுத்து;
		நிலை HOOK_CONFIGURED: {
			மாறிலி எழுத்து *name = h->u.configured.friendly_name;
			மாறிலி எழுத்து *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : வெற்று;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			மாறிலி எழுத்து *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			எனில் (scope)
				printf("%s\t%s%s%c", scope, disability, name,
				       வரி_terminator);
			இல்லையெனில்
				printf("%s%s%c", disability, name,
				       வரி_terminator);
			நிறுத்து;
		}
		இயல்பு:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(head, hook_free);
	விடுவி(head);
	திரும்பு ret;
}

நிலையான முழு run(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo UNUSED)
{
	முழு i;
	கட்டமைப்பு run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	முழு ignore_missing = 0;
	முழு allow_unknown = 0;
	முழு jobs = 0;
	மாறிலி எழுத்து *hook_name;
	கட்டமைப்பு option run_options[] = {
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
	முழு ret;

	argc = parse_options(argc, argv, prefix, run_options,
			     builtin_hook_run_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	எனில் (jobs == -1)
		opt.jobs = online_cpus();
	இல்லையெனில் எனில் (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	இல்லையெனில்
		opt.jobs = jobs;

	எனில் (!argc)
		செல் usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	எனில் (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		செல் usage;

	/* Add our arguments, start after -- */
	க்கு (i = 2 ; i < argc; i++)
		strvec_தள்ளு(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, வெற்று);

	hook_name = argv[0];

	எனில் (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		திரும்பு 1;
	}

	எனில் (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	எனில் (ret < 0) /* error() return */
		ret = 1;
	திரும்பு ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

முழு cmd_hook(முழு argc,
	     மாறிலி எழுத்து **argv,
	     மாறிலி எழுத்து *prefix,
	     கட்டமைப்பு repository *repo)
{
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, வெற்று, builtin_hook_options,
			     builtin_hook_usage, 0);

	திரும்பு fn(argc, argv, prefix, repo);
}
