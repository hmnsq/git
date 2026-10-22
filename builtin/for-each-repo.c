#परिभाषय USE_THE_REPOSITORY_VARIABLE

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

स्थिर पूर्णाङ्क सततम्_command_on_repo(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण **argv)
{
	संरचना child_process child = CHILD_PROCESS_INIT;
	वर्ण *abspath = interpolate_मार्गः(मार्गः, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.तर्काः, "-C", abspath, रिक्त);
	strvec_pushv(&child.तर्काः, argv);

	मुक्त(abspath);

	प्रत्यागम सततम्_command(&child);
}

पूर्णाङ्क cmd_for_each_repo(पूर्णाङ्क argc,
		      ध्रुव वर्ण **argv,
		      ध्रुव वर्ण *उपसर्गः,
		      संरचना repository *repo अप्रयुक्तम्)
{
	स्थिर ध्रुव वर्ण *config_कुञ्चिका = रिक्त;
	पूर्णाङ्क keep_going = 0;
	पूर्णाङ्क परिणाम = 0;
	ध्रुव संरचना string_list *मानानि;
	पूर्णाङ्क err;

	ध्रुव संरचना option options[] = {
		OPT_STRING(0, "config", &config_कुञ्चिका, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	यदि (!config_कुञ्चिका)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_कुञ्चिका, &मानानि);
	यदि (err < 0)
		usage_सन्देशः_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_कुञ्चिका);
	अन्यथा यदि (err)
		प्रत्यागम 0;

	प्रति (आकार_t i = 0; i < मानानि->nr; i++) {
		पूर्णाङ्क ret = सततम्_command_on_repo(मानानि->वस्तूनि[i].string, argv);
		यदि (ret) {
			यदि (!keep_going)
					प्रत्यागम ret;
			परिणाम = 1;
		}
	}

	प्रत्यागम परिणाम;
}
