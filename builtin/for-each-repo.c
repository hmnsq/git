#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "string-list.h"

स्थिर ध्रुव वर्ण * ध्रुव for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	रिक्त
};

स्थिर पूर्णाङ्क run_command_on_repo(ध्रुव वर्ण *path, ध्रुव वर्ण **argv)
{
	संरचना child_process child = CHILD_PROCESS_INIT;
	वर्ण *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, रिक्त);
	strvec_pushv(&child.args, argv);

	मुक्त(abspath);

	प्रत्यागम run_command(&child);
}

पूर्णाङ्क cmd_for_each_repo(पूर्णाङ्क argc,
		      ध्रुव वर्ण **argv,
		      ध्रुव वर्ण *prefix,
		      संरचना repository *repo UNUSED)
{
	स्थिर ध्रुव वर्ण *config_कुंजी = रिक्त;
	पूर्णाङ्क keep_going = 0;
	पूर्णाङ्क result = 0;
	ध्रुव संरचना string_list *मूल्यावली;
	पूर्णाङ्क err;

	ध्रुव संरचना option options[] = {
		OPT_STRING(0, "config", &config_कुंजी, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	यदि (!config_कुंजी)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_कुंजी, &मूल्यावली);
	यदि (err < 0)
		usage_संदेश_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_कुंजी);
	अन्यथा यदि (err)
		प्रत्यागम 0;

	प्रति (size_t i = 0; i < मूल्यावली->nr; i++) {
		पूर्णाङ्क ret = run_command_on_repo(मूल्यावली->वस्तुएँ[i].string, argv);
		यदि (ret) {
			यदि (!keep_going)
					प्रत्यागम ret;
			result = 1;
		}
	}

	प्रत्यागम result;
}
