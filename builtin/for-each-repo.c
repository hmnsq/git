#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "string-list.h"

स्थिर अचर वर्ण * अचर for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	रिक्त
};

स्थिर पूर्णाङ्क run_command_on_repo(अचर वर्ण *path, अचर वर्ण **argv)
{
	संरचना child_process child = CHILD_PROCESS_INIT;
	वर्ण *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, रिक्त);
	strvec_pushv(&child.args, argv);

	मुक्त(abspath);

	फर्काऊ run_command(&child);
}

पूर्णाङ्क cmd_for_each_repo(पूर्णाङ्क argc,
		      अचर वर्ण **argv,
		      अचर वर्ण *prefix,
		      संरचना repository *repo UNUSED)
{
	स्थिर अचर वर्ण *config_साँचो = रिक्त;
	पूर्णाङ्क keep_going = 0;
	पूर्णाङ्क result = 0;
	अचर संरचना string_list *मानहरू;
	पूर्णाङ्क err;

	अचर संरचना option options[] = {
		OPT_STRING(0, "config", &config_साँचो, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	यदि (!config_साँचो)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_साँचो, &मानहरू);
	यदि (err < 0)
		usage_सन्देश_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_साँचो);
	अन्यथा यदि (err)
		फर्काऊ 0;

	लागि (माप_प्रकार i = 0; i < मानहरू->nr; i++) {
		पूर्णाङ्क ret = run_command_on_repo(मानहरू->वस्तुहरू[i].string, argv);
		यदि (ret) {
			यदि (!keep_going)
					फर्काऊ ret;
			result = 1;
		}
	}

	फर्काऊ result;
}
