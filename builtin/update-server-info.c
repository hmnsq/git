#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "server-info.h"

ساکن ثابت حرف * ثابت update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	تہی
};

صحیح cmd_update_server_info(صحیح argc,
			   ثابت حرف **argv,
			   ثابت حرف *سابقہ,
			   ساخت repository *repo)
{
	صحیح force = 0;
	ساخت option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_انتہا()
	};

	repo_config(repo, git_default_config, تہی);

	argc = parse_options(argc, argv, سابقہ, options,
			     update_server_info_usage, 0);
	اگر (argc > 0)
		usage_with_options(update_server_info_usage, options);

	واپس !!update_server_info(repo, force);
}
