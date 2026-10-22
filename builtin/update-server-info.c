#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "server-info.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	ಖಾಲಿ
};

ಪೂರ್ಣಾಂಕ cmd_update_server_info(ಪೂರ್ಣಾಂಕ argc,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ force = 0;
	ರಚನೆ option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_ಕೊನೆ()
	};

	repo_config(repo, git_default_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     update_server_info_usage, 0);
	ಆದರೆ (argc > 0)
		usage_with_options(update_server_info_usage, options);

	ಹಿಂತಿರುಗಿ !!update_server_info(repo, force);
}
