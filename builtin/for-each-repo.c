#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "run-command.h"
#समावेश "string-list.h"

अटल स्थिर अक्षर * स्थिर for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	शून्य
};

अटल पूर्णांक run_command_on_repo(स्थिर अक्षर *path, स्थिर अक्षर **argv)
{
	काष्ठा child_process child = CHILD_PROCESS_INIT;
	अक्षर *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, शून्य);
	strvec_pushv(&child.args, argv);

	मुक्त(abspath);

	वापस run_command(&child);
}

पूर्णांक cmd_for_each_repo(पूर्णांक argc,
		      स्थिर अक्षर **argv,
		      स्थिर अक्षर *prefix,
		      काष्ठा repository *repo UNUSED)
{
	अटल स्थिर अक्षर *config_कुंजी = शून्य;
	पूर्णांक keep_going = 0;
	पूर्णांक परिणाम = 0;
	स्थिर काष्ठा string_list *मूल्यावली;
	पूर्णांक err;

	स्थिर काष्ठा option options[] = {
		OPT_STRING(0, "config", &config_कुंजी, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	अगर (!config_कुंजी)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_कुंजी, &मूल्यावली);
	अगर (err < 0)
		usage_संदेश_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_कुंजी);
	अन्यथा अगर (err)
		वापस 0;

	क्रम (माप_प्रकार i = 0; i < मूल्यावली->nr; i++) {
		पूर्णांक ret = run_command_on_repo(मूल्यावली->वस्तुएँ[i].string, argv);
		अगर (ret) {
			अगर (!keep_going)
					वापस ret;
			परिणाम = 1;
		}
	}

	वापस परिणाम;
}
