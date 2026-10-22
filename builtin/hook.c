#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hook.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "thread-utils.h"

#define BUILTIN_HOOK_RUN_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#define BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା list(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		ଖାଲି
	};
	ସଂରଚନା string_list *head;
	ସଂରଚନା string_list_item *item;
	ଅଚଳ ଅକ୍ଷର *hookname = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_terminator = '\n';
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_scope = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_unknown = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ସଂରଚନା option list_options[] = {
		OPT_SET_INT('z', ଖାଲି, &ଧାଡ଼ି_terminator,
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
	ଯଦି (argc != 1)
		usage_ସନ୍ଦେଶ_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	ଯଦି (!allow_unknown && !is_known_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		ଫେରନ୍ତୁ 1;
	}

	head = list_hooks(repo, hookname, ଖାଲି);

	ଯଦି (!head->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		ଯାଅ cleanup;
	}

	for_each_string_list_item(item, head) {
		ସଂରଚନା hook *h = item->util;

		ବାଛ (h->kind) {
		ସ୍ଥିତି HOOK_TRADITIONAL:
			printf("%s%c", _("hook from hookdir"), ଧାଡ଼ି_terminator);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି HOOK_CONFIGURED: {
			ଅଚଳ ଅକ୍ଷର *name = h->u.configured.friendly_name;
			ଅଚଳ ଅକ୍ଷର *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : ଖାଲି;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			ଅଚଳ ଅକ୍ଷର *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			ଯଦି (scope)
				printf("%s\t%s%s%c", scope, disability, name,
				       ଧାଡ଼ି_terminator);
			ଅନ୍ୟଥା
				printf("%s%s%c", disability, name,
				       ଧାଡ଼ି_terminator);
			ଭାଙ୍ଗ;
		}
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(head, hook_free);
	ମୁକ୍ତ(head);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_missing = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_unknown = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା jobs = 0;
	ଅଚଳ ଅକ୍ଷର *hook_name;
	ସଂରଚନା option run_options[] = {
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
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, run_options,
			     builtin_hook_run_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ଯଦି (jobs == -1)
		opt.jobs = online_cpus();
	ଅନ୍ୟଥା ଯଦି (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	ଅନ୍ୟଥା
		opt.jobs = jobs;

	ଯଦି (!argc)
		ଯାଅ usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	ଯଦି (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		ଯାଅ usage;

	/* Add our arguments, start after -- */
	ପାଇଁ (i = 2 ; i < argc; i++)
		strvec_ଠେଲ(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, ଖାଲି);

	hook_name = argv[0];

	ଯଦି (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		ଫେରନ୍ତୁ 1;
	}

	ଯଦି (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	ଯଦି (ret < 0) /* error() return */
		ret = 1;
	ଫେରନ୍ତୁ ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_hook(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	     ଅଚଳ ଅକ୍ଷର **argv,
	     ଅଚଳ ଅକ୍ଷର *prefix,
	     ସଂରଚନା repository *repo)
{
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, ଖାଲି, builtin_hook_options,
			     builtin_hook_usage, 0);

	ଫେରନ୍ତୁ fn(argc, argv, prefix, repo);
}
