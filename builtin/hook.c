#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hook.h"
#include "parse-options.h"
#include "thread-utils.h"

#define BUILTIN_HOOK_RUN_USAGE \
	N_("git hook run [--allow-unknown-hook-name] [--ignore-missing] [--to-stdin=<path>] [(-j|--jobs) <n>]\n" \
	   "<hook-name> [-- <hook-args>]")
#define BUILTIN_HOOK_LIST_USAGE \
	N_("git hook list [--allow-unknown-hook-name] [-z] [--show-scope] <hook-name>")

static const char * const builtin_hook_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	BUILTIN_HOOK_LIST_USAGE,
	NULL
};

static const char * const builtin_hook_run_usage[] = {
	BUILTIN_HOOK_RUN_USAGE,
	NULL
};

static ціле list(ціле argc, const char **argv, const char *prefix,
		 struct repository *repo)
{
	static const char *const builtin_hook_list_usage[] = {
		BUILTIN_HOOK_LIST_USAGE,
		NULL
	};
	struct string_list *head;
	struct string_list_item *item;
	const char *hookname = NULL;
	ціле line_terminator = '\n';
	ціле show_scope = 0;
	ціле allow_unknown = 0;
	ціле ret = 0;

	struct option list_options[] = {
		OPT_SET_INT('z', NULL, &line_terminator,
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
	якщо (argc != 1)
		usage_msg_opt(_("you must specify a hook event name to list"),
			      builtin_hook_list_usage, list_options);

	hookname = argv[0];

	якщо (!allow_unknown && !is_known_hook(hookname)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hookname);
		повернути 1;
	}

	head = list_hooks(repo, hookname, NULL);

	якщо (!head->nr) {
		warning(_("no hooks found for event '%s'"), hookname);
		ret = 1; /* no hooks found */
		goto cleanup;
	}

	for_each_string_list_item(item, head) {
		struct hook *h = item->util;

		switch (h->kind) {
		case HOOK_TRADITIONAL:
			printf("%s%c", _("hook from hookdir"), line_terminator);
			break;
		case HOOK_CONFIGURED: {
			const char *name = h->u.configured.friendly_name;
			const char *scope = show_scope ?
				config_scope_name(h->u.configured.scope) : NULL;
			/*
			 * Show the most relevant disable reason. Event-level
			 * takes precedence: if the whole event is off, that
			 * is what the user needs to know. The per-hook
			 * "disabled" surfaces once the event is re-enabled.
			 */
			const char *disability =
				h->u.configured.event_disabled ? "event-disabled\t" :
				h->u.configured.disabled       ? "disabled\t"       :
								 "";
			якщо (scope)
				printf("%s\t%s%s%c", scope, disability, name,
				       line_terminator);
			інакше
				printf("%s%s%c", disability, name,
				       line_terminator);
			break;
		}
		default:
			BUG("unknown hook kind");
		}
	}

cleanup:
	string_list_clear_func(head, hook_free);
	free(head);
	повернути ret;
}

static ціле run(ціле argc, const char **argv, const char *prefix,
	       struct repository *repo UNUSED)
{
	ціле i;
	struct run_hooks_opt opt = RUN_HOOKS_OPT_INIT;
	ціле ignore_missing = 0;
	ціле allow_unknown = 0;
	ціле jobs = 0;
	const char *hook_name;
	struct option run_options[] = {
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
	ціле ret;

	argc = parse_options(argc, argv, prefix, run_options,
			     builtin_hook_run_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	якщо (jobs == -1)
		opt.jobs = online_cpus();
	інакше якщо (jobs < 0)
		die(_("invalid value for -j: %d"
		     " (use -1 for CPU count or a"
		     " positive integer)"), jobs);
	інакше
		opt.jobs = jobs;

	якщо (!argc)
		goto usage;

	/*
	 * Having a -- for "run" when providing <hook-args> is
	 * mandatory.
	 */
	якщо (argc > 1 && strcmp(argv[1], "--") &&
	    strcmp(argv[1], "--end-of-options"))
		goto usage;

	/* Add our arguments, start after -- */
	для (i = 2 ; i < argc; i++)
		strvec_push(&opt.args, argv[i]);

	/* Need to take into account core.hooksPath */
	repo_config(the_repository, git_default_config, NULL);

	hook_name = argv[0];

	якщо (!allow_unknown && !is_known_hook(hook_name)) {
		error(_("unknown hook event '%s';\n"
			"use --allow-unknown-hook-name to allow non-native hook names"),
		      hook_name);
		повернути 1;
	}

	якщо (!ignore_missing)
		opt.error_if_missing = 1;
	ret = run_hooks_opt(the_repository, hook_name, &opt);
	якщо (ret < 0) /* error() return */
		ret = 1;
	повернути ret;
usage:
	usage_with_options(builtin_hook_run_usage, run_options);
}

ціле cmd_hook(ціле argc,
	     const char **argv,
	     const char *prefix,
	     struct repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	struct option builtin_hook_options[] = {
		OPT_SUBCOMMAND("run", &fn, run),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_END(),
	};

	argc = parse_options(argc, argv, NULL, builtin_hook_options,
			     builtin_hook_usage, 0);

	повернути fn(argc, argv, prefix, repo);
}
