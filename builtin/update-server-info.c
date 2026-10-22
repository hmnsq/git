#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "server-info.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	ರಿಕ್ತ
};

ಪೂರ್ಣಾಙ್ಕ cmd_update_server_info(ಪೂರ್ಣಾಙ್ಕ argc,
			   ಧ್ರುವ ವರ್ಣ **argv,
			   ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ force = 0;
	ಸಂರಚನಾ option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	ಯದಿ (argc > 0)
		usage_with_options(update_server_info_usage, options);

	ಪ್ರತ್ಯಾಗಮ !!update_server_info(repo, force);
}
