#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "parse-options.h"
#समावेश "server-info.h"

अटल स्थिर अक्षर * स्थिर update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	शून्य
};

पूर्णांक cmd_update_server_info(पूर्णांक argc,
			   स्थिर अक्षर **argv,
			   स्थिर अक्षर *prefix,
			   काष्ठा repository *repo)
{
	पूर्णांक force = 0;
	काष्ठा option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, शून्य);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	अगर (argc > 0)
		usage_with_options(update_server_info_usage, options);

	वापस !!update_server_info(repo, force);
}
