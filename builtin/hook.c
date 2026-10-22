#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hook.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "thread-utils.h"

#define BUILTIN_HOOK_RUN_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#define BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

સ્થિર અચલ અક્ષર * અચલ builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	ખાલી
};

સ્થિર પૂર્ણાંક list(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		 સંરચના repository *repo)
{
	સ્થિર અચલ અક્ષર *અચલ builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		ખાલી
	};
	સંરચના string_list *head;
	સંરચના string_list_item *item;
	અચલ અક્ષર *hookname = ખાલી;
	પૂર્ણાંક લીટી_terminator = '\n';
	પૂર્ણાંક show_scope = 0;
	પૂર્ણાંક allow_unknown = 0;
	પૂર્ણાંક ret = 0;

	સંરચના option list_options[] = {
		OPT_SET_INT('z', ખાલી, &લીટી_terminator,
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
	જો (argc != 1)
		usage_સંદેશ_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	જો (!allow_unknown && !is_known_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		પાછા 1;
	}

	head = list_hooks(repo, hookname, ખાલી);

	જો (!head->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		જાઓ cleanup;
	}

	for_each_string_list_item(item, head) {
		સંરચના hook *h = item->util;

		પસંદગી (h->kind) {
		સ્થિતિ HOOK_TRADITIONAL:
			printf("%s%c", _("hook from hookdir"), લીટી_terminator);
			તોડો;
		સ્થિતિ HOOK_CONFIGURED: {
			અચલ અક્ષર *name = h->u.configured.friendly_name;
			અચલ અક્ષર *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : ખાલી;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			અચલ અક્ષર *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			જો (scope)
				printf("%s\t%s%s%c", scope, disability, name,
				       લીટી_terminator);
			નહીંતર
				printf("%s%s%c", disability, name,
				       લીટી_terminator);
			તોડો;
		}
		મૂળભૂત:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(head, hook_free);
	મુક્ત(head);
	પાછા ret;
}

સ્થિર પૂર્ણાંક run(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
	       સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i;
	સંરચના run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	પૂર્ણાંક ignore_missing = 0;
	પૂર્ણાંક allow_unknown = 0;
	પૂર્ણાંક jobs = 0;
	અચલ અક્ષર *hook_name;
	સંરચના option run_options[] = {
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
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, run_options,
			     builtin_hook_run_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	જો (jobs == -1)
		opt.jobs = online_cpus();
	નહીંતર જો (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	નહીંતર
		opt.jobs = jobs;

	જો (!argc)
		જાઓ usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	જો (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		જાઓ usage;

	/* Add our arguments, start after -- */
	માટે (i = 2 ; i < argc; i++)
		strvec_ધકેલો(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, ખાલી);

	hook_name = argv[0];

	જો (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		પાછા 1;
	}

	જો (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	જો (ret < 0) /* error() return */
		ret = 1;
	પાછા ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

પૂર્ણાંક cmd_hook(પૂર્ણાંક argc,
	     અચલ અક્ષર **argv,
	     અચલ અક્ષર *prefix,
	     સંરચના repository *repo)
{
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, ખાલી, builtin_hook_options,
			     builtin_hook_usage, 0);

	પાછા fn(argc, argv, prefix, repo);
}
