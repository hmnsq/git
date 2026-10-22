#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "server-info.h"

ساكن ثابت حرف * ثابت update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	عدم
};

صحيح cmd_update_server_info(صحيح argc,
			   ثابت حرف **argv,
			   ثابت حرف *اڳياڙي,
			   بنية repository *repo)
{
	صحيح force = 0;
	بنية option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_پڇاڙي()
	};

	repo_config(repo, git_default_config, عدم);

	argc = parse_options(argc, argv, اڳياڙي, options,
			     update_server_info_usage, 0);
	إذا (argc > 0)
		usage_with_options(update_server_info_usage, options);

	أرجع !!update_server_info(repo, force);
}
