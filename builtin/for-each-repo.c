#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "string-list.h"

स्थिर स्थिरांक अक्षर * स्थिरांक for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	रिक्त
};

स्थिर पूर्णांक run_command_on_repo(स्थिरांक अक्षर *path, स्थिरांक अक्षर **argv)
{
	संरचना child_process child = CHILD_PROCESS_INIT;
	अक्षर *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, रिक्त);
	strvec_pushv(&child.args, argv);

	मुक्त(abspath);

	परत run_command(&child);
}

पूर्णांक cmd_for_each_repo(पूर्णांक argc,
		      स्थिरांक अक्षर **argv,
		      स्थिरांक अक्षर *prefix,
		      संरचना repository *repo UNUSED)
{
	स्थिर स्थिरांक अक्षर *config_किल्ली = रिक्त;
	पूर्णांक keep_going = 0;
	पूर्णांक result = 0;
	स्थिरांक संरचना string_list *मूल्ये;
	पूर्णांक err;

	स्थिरांक संरचना option options[] = {
		OPT_STRING(0, "config", &config_किल्ली, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	जर (!config_किल्ली)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_किल्ली, &मूल्ये);
	जर (err < 0)
		usage_संदेश_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_किल्ली);
	अन्यथा जर (err)
		परत 0;

	साठी (माप_प्रकार i = 0; i < मूल्ये->nr; i++) {
		पूर्णांक ret = run_command_on_repo(मूल्ये->वस्तुसंच[i].string, argv);
		जर (ret) {
			जर (!keep_going)
					परत ret;
			result = 1;
		}
	}

	परत result;
}
