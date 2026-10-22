#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "server-info.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	ఖాళీ
};

పూర్ణాంకం cmd_update_server_info(పూర్ణాంకం argc,
			   స్థిరరాశి అక్షరం **argv,
			   స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo)
{
	పూర్ణాంకం force = 0;
	నిర్మాణం option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_చివర()
	};

	repo_config(repo, git_default_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     update_server_info_usage, 0);
	ఒకవేళ (argc > 0)
		usage_with_options(update_server_info_usage, options);

	తిరిగి !!update_server_info(repo, force);
}
