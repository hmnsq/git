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

static số_nguyên run_command_on_repo(const char *path, const char **argv)
{
	struct child_process child = CHILD_PROCESS_INIT;
	char *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, NULL);
	strvec_pushv(&child.args, argv);

	free(abspath);

	trả_về run_command(&child);
}

số_nguyên cmd_for_each_repo(số_nguyên argc,
		      const char **argv,
		      const char *prefix,
		      struct repository *repo UNUSED)
{
	static const char *config_key = NULL;
	số_nguyên keep_going = 0;
	số_nguyên result = 0;
	const struct string_list *values;
	số_nguyên err;

	const struct option options[] = {
		OPT_STRING(0, "config", &config_key, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	nếu (!config_key)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_key, &values);
	nếu (err < 0)
		usage_msg_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_key);
	không_thì nếu (err)
		trả_về 0;

	cho (size_t i = 0; i < values->nr; i++) {
		số_nguyên ret = run_command_on_repo(values->items[i].string, argv);
		nếu (ret) {
			nếu (!keep_going)
					trả_về ret;
			result = 1;
		}
	}

	trả_về result;
}
