#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "server-info.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	ਨਿਰਾ
};

ਪੂਰਨ cmd_update_server_info(ਪੂਰਨ argc,
			   ਅਚਲ ਅੱਖਰ **argv,
			   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ force = 0;
	ਬਣਤਰ option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_ਸਿਰਾ()
	};

	repo_config(repo, git_default_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     update_server_info_usage, 0);
	ਜੇ (argc > 0)
		usage_with_options(update_server_info_usage, options);

	ਵਾਪਸ !!update_server_info(repo, force);
}
