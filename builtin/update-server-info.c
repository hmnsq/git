#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "parse-options.h"
#include "server-info.h"

static const char * const update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	NULL
};

整数 cmd_update_server_info(整数 argc,
			   const char **argv,
			   const char *prefix,
			   struct repository *repo)
{
	整数 force = 0;
	struct option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	もし (argc > 0)
		usage_with_options(update_server_info_usage, options);

	戻る !!update_server_info(repo, force);
}
