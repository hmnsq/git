#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "string-list.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	ఖాళీ
};

స్థిర పూర్ణాంకం నిరంతరం_command_on_repo(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం **argv)
{
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;
	అక్షరం *abspath = interpolate_మార్గం(మార్గం, 0);

	sanitize_repo_env(&బిడ్డ.env);

	బిడ్డ.git_cmd = 1;
	strvec_pushl(&బిడ్డ.వాదనలు, "-C", abspath, ఖాళీ);
	strvec_pushv(&బిడ్డ.వాదనలు, argv);

	విడుదల(abspath);

	తిరిగి నిరంతరం_command(&బిడ్డ);
}

పూర్ణాంకం cmd_for_each_repo(పూర్ణాంకం argc,
		      స్థిరరాశి అక్షరం **argv,
		      స్థిరరాశి అక్షరం *ఉపసర్గ,
		      నిర్మాణం repository *repo వాడని)
{
	స్థిర స్థిరరాశి అక్షరం *config_కీలకం = ఖాళీ;
	పూర్ణాంకం keep_going = 0;
	పూర్ణాంకం ఫలితం = 0;
	స్థిరరాశి నిర్మాణం string_list *విలువలు;
	పూర్ణాంకం err;

	స్థిరరాశి నిర్మాణం option options[] = {
		OPT_STRING(0, "config", &config_కీలకం, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ఒకవేళ (!config_కీలకం)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_కీలకం, &విలువలు);
	ఒకవేళ (err < 0)
		usage_సందేశం_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_కీలకం);
	లేకపోతే ఒకవేళ (err)
		తిరిగి 0;

	కొరకు (కొలత_t i = 0; i < విలువలు->nr; i++) {
		పూర్ణాంకం ret = నిరంతరం_command_on_repo(విలువలు->వస్తువులు[i].string, argv);
		ఒకవేళ (ret) {
			ఒకవేళ (!keep_going)
					తిరిగి ret;
			ఫలితం = 1;
		}
	}

	తిరిగి ఫలితం;
}
