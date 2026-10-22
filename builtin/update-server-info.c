#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "server-info.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	ଖାଲି
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_update_server_info(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			   ଅଚଳ ଅକ୍ଷର **argv,
			   ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = 0;
	ସଂରଚନା option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	ଯଦି (argc > 0)
		usage_with_options(update_server_info_usage, options);

	ଫେରନ୍ତୁ !!update_server_info(repo, force);
}
