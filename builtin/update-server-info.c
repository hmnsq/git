#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "parse-options.h"
#include "server-info.h"

static const var_na * const update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	NULL
};

පූර්ණ_සංඛ්‍යා cmd_update_server_info(පූර්ණ_සංඛ්‍යා argc,
			   const var_na **argv,
			   const var_na *prefix,
			   sa_nracanaa repository *repo)
{
	පූර්ණ_සංඛ්‍යා force = 0;
	sa_nracanaa option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	නම් (argc > 0)
		usage_with_options(update_server_info_usage, options);

	ආපසු !!update_server_info(repo, force);
}
