#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "server-info.h"

স্থির ধ্রুব বর্ণ * ধ্রুব update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	রিক্ত
};

পূর্ণাঙ্ক cmd_update_server_info(পূর্ণাঙ্ক argc,
			   ধ্রুব বর্ণ **argv,
			   ধ্রুব বর্ণ *prefix,
			   সংরচনা repository *repo)
{
	পূর্ণাঙ্ক force = 0;
	সংরচনা option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, রিক্ত);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	যদি (argc > 0)
		usage_with_options(update_server_info_usage, options);

	প্রত্যাগম !!update_server_info(repo, force);
}
