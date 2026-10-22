#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hook.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "thread-utils.h"

#నిర్వచించు BUILTIN_HOOK_నిరంతరం_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#నిర్వచించు BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_hook_usage[] = {
	BUILTIN_HOOK_నిరంతరం_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_hook_నిరంతరం_usage[] = {
	BUILTIN_HOOK_నిరంతరం_USAGE,
	ఖాళీ
};

స్థిర పూర్ణాంకం list(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo)
{
	స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		ఖాళీ
	};
	నిర్మాణం string_list *తల;
	నిర్మాణం string_list_అంశం *అంశం;
	స్థిరరాశి అక్షరం *hookname = ఖాళీ;
	పూర్ణాంకం పంక్తి_terminator = '\n';
	పూర్ణాంకం show_scope = 0;
	పూర్ణాంకం allow_unknown = 0;
	పూర్ణాంకం ret = 0;

	నిర్మాణం option list_options[] = {
		OPT_సమితి_INT('z', ఖాళీ, &పంక్తి_terminator,
			    N_("use NUL as line terminator"), '\0'),
		OPT_BOOL(0, "show-scope", &show_scope,
			 N_("show the config scope that defined each hook")),
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_చివర(),
	};

	argc = parse_options(argc, argv, ఉపసర్గ, list_options,
			     builtin_hook_list_usage, 0);

	/*
	 * The only unnamed argument provided should be the hook-name; if we add
	 * arguments later they probably should be caught by parse_options.
	 */
	ఒకవేళ (argc != 1)
		usage_సందేశం_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	ఒకవేళ (!allow_unknown && !is_తెలిసిన_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		తిరిగి 1;
	}

	తల = list_hooks(repo, hookname, ఖాళీ);

	ఒకవేళ (!తల->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		వెళ్ళు cleanup;
	}

	for_each_string_list_అంశం(అంశం, తల) {
		నిర్మాణం hook *h = అంశం->util;

		ఎంపిక (h->రకం) {
		స్థితి HOOK_TRADITIONAL:
			రాయి("%s%c", _("hook from hookdir"), పంక్తి_terminator);
			ఆపు;
		స్థితి HOOK_CONFIGURED: {
			స్థిరరాశి అక్షరం *name = h->u.configured.friendly_name;
			స్థిరరాశి అక్షరం *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : ఖాళీ;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			స్థిరరాశి అక్షరం *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			ఒకవేళ (scope)
				రాయి("%s\t%s%s%c", scope, disability, name,
				       పంక్తి_terminator);
			లేకపోతే
				రాయి("%s%s%c", disability, name,
				       పంక్తి_terminator);
			ఆపు;
		}
		అప్రమేయ:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(తల, hook_free);
	విడుదల(తల);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం నిరంతరం(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i;
	నిర్మాణం నిరంతరం_hooks_opt opt = నిరంతరం_HOOKS_OPT_INIT;
	పూర్ణాంకం ignore_missing = 0;
	పూర్ణాంకం allow_unknown = 0;
	పూర్ణాంకం jobs = 0;
	స్థిరరాశి అక్షరం *hook_name;
	నిర్మాణం option నిరంతరం_options[] = {
		OPT_BOOL(0, "allow-unknown-hook-name", &allow_unknown,
			 N_("allow running a hook with a non-native hook name")),
		OPT_BOOL(0, "ignore-missing", &ignore_missing,
			 N_("silently ignore missing requested <hook-name>")),
		OPT_STRING(0, "to-stdin", &opt.మార్గం_గమ్యం_stdin, N_("path"),
			   N_("file to read into hooks' stdin")),
		OPT_INTEGER('j', "jobs", &jobs,
			    N_("run up to <n> hooks simultaneously (-1 for CPU count)")),
		OPT_చివర(),
	};
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, నిరంతరం_options,
			     builtin_hook_నిరంతరం_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ఒకవేళ (jobs == -1)
		opt.jobs = online_cpus();
	లేకపోతే ఒకవేళ (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	లేకపోతే
		opt.jobs = jobs;

	ఒకవేళ (!argc)
		వెళ్ళు usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	ఒకవేళ (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		వెళ్ళు usage;

	/* Add our arguments, start after -- */
	కొరకు (i = 2 ; i < argc; i++)
		strvec_నెట్టు(&opt.వాదనలు, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, ఖాళీ);

	hook_name = argv[0];

	ఒకవేళ (!allow_unknown && !is_తెలిసిన_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		తిరిగి 1;
	}

	ఒకవేళ (!ignore_missing)
		opt.error_if_missing = 1;
	ret = నిరంతరం_hooks_opt(the_repository, hook_name, &opt);
	ఒకవేళ (ret < 0) /* error() return */
		ret = 1;
	తిరిగి ret;
usage:
	usage_with_options(builtin_hook_నిరంతరం_usage, నిరంతరం_options);
}

పూర్ణాంకం cmd_hook(పూర్ణాంకం argc,
	     స్థిరరాశి అక్షరం **argv,
	     స్థిరరాశి అక్షరం *ఉపసర్గ,
	     నిర్మాణం repository *repo)
{
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, నిరంతరం),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_చివర(),
	};

	argc = parse_options(argc, argv, ఖాళీ, builtin_hook_options,
			     builtin_hook_usage, 0);

	తిరిగి fn(argc, argv, ఉపసర్గ, repo);
}
