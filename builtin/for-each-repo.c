#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "parse-options.h"
#include "path.h"
#include "run-command.h"
#include "string-list.h"

static const char * const for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	NULL
};

static ціле run_command_on_repo(const char *path, const char **argv)
{
	struct child_process child = CHILD_PROCESS_INIT;
	char *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, NULL);
	strvec_pushv(&child.args, argv);

	free(abspath);

	повернути run_command(&child);
}

ціле cmd_for_each_repo(ціле argc,
		      const char **argv,
		      const char *prefix,
		      struct repository *repo UNUSED)
{
	static const char *config_key = NULL;
	ціле keep_going = 0;
	ціле result = 0;
	const struct string_list *values;
	ціле err;

	const struct option options[] = {
		OPT_STRING(0, "config", &config_key, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	якщо (!config_key)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_key, &values);
	якщо (err < 0)
		usage_msg_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_key);
	інакше якщо (err)
		повернути 0;

	для (size_t i = 0; i < values->nr; i++) {
		ціле ret = run_command_on_repo(values->items[i].string, argv);
		якщо (ret) {
			якщо (!keep_going)
					повернути ret;
			result = 1;
		}
	}

	повернути result;
}
