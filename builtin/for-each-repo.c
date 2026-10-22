#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "parse-options.h"
#include "path.h"
#include "run-command.h"
#include "string-list.h"

static const var_na * const for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	NULL
};

static ཧྲིལ run_command_on_repo(const var_na *path, const var_na **argv)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;
	var_na *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, NULL);
	strvec_pushv(&child.args, argv);

	free(abspath);

	ཕྱིར run_command(&child);
}

ཧྲིལ cmd_for_each_repo(ཧྲིལ argc,
		      const var_na **argv,
		      const var_na *prefix,
		      sa_nracanaa repository *repo UNUSED)
{
	static const var_na *config_key = NULL;
	ཧྲིལ keep_going = 0;
	ཧྲིལ result = 0;
	const sa_nracanaa string_list *values;
	ཧྲིལ err;

	const sa_nracanaa option options[] = {
		OPT_STRING(0, "config", &config_key, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	གལ་ཏེ (!config_key)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_key, &values);
	གལ་ཏེ (err < 0)
		usage_msg_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_key);
	དེ་མིན གལ་ཏེ (err)
		ཕྱིར 0;

	ཆེད་དུ (size_t i = 0; i < values->nr; i++) {
		ཧྲིལ ret = run_command_on_repo(values->items[i].string, argv);
		གལ་ཏེ (ret) {
			གལ་ཏེ (!keep_going)
					ཕྱིར ret;
			result = 1;
		}
	}

	ཕྱིར result;
}
