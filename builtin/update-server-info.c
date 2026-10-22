#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "server-info.h"

स्थिर अचर वर्ण * अचर update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	रिक्त
};

पूर्णाङ्क cmd_update_server_info(पूर्णाङ्क argc,
			   अचर वर्ण **argv,
			   अचर वर्ण *prefix,
			   संरचना repository *repo)
{
	पूर्णाङ्क force = 0;
	संरचना option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	यदि (argc > 0)
		usage_with_options(update_server_info_usage, options);

	फर्काऊ !!update_server_info(repo, force);
}
