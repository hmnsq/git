#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "server-info.h"

ایستا ثابت نویسه * ثابت update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	پوچ
};

صحیح cmd_update_server_info(صحیح argc,
			   ثابت نویسه **argv,
			   ثابت نویسه *prefix,
			   ساختار repository *repo)
{
	صحیح force = 0;
	ساختار option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, پوچ);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	اگر (argc > 0)
		usage_with_options(update_server_info_usage, options);

	بازگشت !!update_server_info(repo, force);
}
