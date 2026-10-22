#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "server-info.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	ফাঁকা
};

পূর্ণসংখ্যা cmd_update_server_info(পূর্ণসংখ্যা argc,
			   ধ্রুবক অক্ষর **argv,
			   ধ্রুবক অক্ষর *prefix,
			   কাঠামো repository *repo)
{
	পূর্ণসংখ্যা force = 0;
	কাঠামো option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, ফাঁকা);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	যদি (argc > 0)
		usage_with_options(update_server_info_usage, options);

	ফেরত !!update_server_info(repo, force);
}
