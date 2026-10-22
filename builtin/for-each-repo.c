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

static ຈຳນວນເຕັມ run_command_on_repo(const var_na *path, const var_na **argv)
{
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;
	var_na *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, NULL);
	strvec_pushv(&child.args, argv);

	free(abspath);

	ສົ່ງຄືນ run_command(&child);
}

ຈຳນວນເຕັມ cmd_for_each_repo(ຈຳນວນເຕັມ argc,
		      const var_na **argv,
		      const var_na *prefix,
		      sa_nracanaa repository *repo UNUSED)
{
	static const var_na *config_key = NULL;
	ຈຳນວນເຕັມ keep_going = 0;
	ຈຳນວນເຕັມ result = 0;
	const sa_nracanaa string_list *values;
	ຈຳນວນເຕັມ err;

	const sa_nracanaa option options[] = {
		OPT_STRING(0, "config", &config_key, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ຖ້າ (!config_key)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_key, &values);
	ຖ້າ (err < 0)
		usage_msg_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_key);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (err)
		ສົ່ງຄືນ 0;

	ສຳລັບ (size_t i = 0; i < values->nr; i++) {
		ຈຳນວນເຕັມ ret = run_command_on_repo(values->items[i].string, argv);
		ຖ້າ (ret) {
			ຖ້າ (!keep_going)
					ສົ່ງຄືນ ret;
			result = 1;
		}
	}

	ສົ່ງຄືນ result;
}
