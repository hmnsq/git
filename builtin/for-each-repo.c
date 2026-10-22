#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_command_on_repo(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;
	ಅಕ್ಷರ *abspath = interpolate_ಮಾರ್ಗ(ಮಾರ್ಗ, 0);

	sanitize_repo_env(&ಮಗು.env);

	ಮಗು.git_cmd = 1;
	strvec_pushl(&ಮಗು.ವಾದಗಳು, "-C", abspath, ಖಾಲಿ);
	strvec_pushv(&ಮಗು.ವಾದಗಳು, argv);

	ಬಿಡುಗಡೆ(abspath);

	ಹಿಂತಿರುಗಿ ಸತತ_command(&ಮಗು);
}

ಪೂರ್ಣಾಂಕ cmd_for_each_repo(ಪೂರ್ಣಾಂಕ argc,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		      ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *config_ಕೀಲಿ = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ keep_going = 0;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *ಮೌಲ್ಯಗಳು;
	ಪೂರ್ಣಾಂಕ err;

	ಸ್ಥಿರಾಂಕ ರಚನೆ option options[] = {
		OPT_STRING(0, "config", &config_ಕೀಲಿ, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ಆದರೆ (!config_ಕೀಲಿ)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_ಕೀಲಿ, &ಮೌಲ್ಯಗಳು);
	ಆದರೆ (err < 0)
		usage_ಸಂದೇಶ_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_ಕೀಲಿ);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ 0;

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ಮೌಲ್ಯಗಳು->nr; i++) {
		ಪೂರ್ಣಾಂಕ ret = ಸತತ_command_on_repo(ಮೌಲ್ಯಗಳು->ವಸ್ತುಗಳು[i].string, argv);
		ಆದರೆ (ret) {
			ಆದರೆ (!keep_going)
					ಹಿಂತಿರುಗಿ ret;
			ಫಲಿತಾಂಶ = 1;
		}
	}

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
