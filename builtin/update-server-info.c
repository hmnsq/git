#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "server-info.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	ശൂന്യ
};

പൂർണ്ണസംഖ്യ cmd_update_server_info(പൂർണ്ണസംഖ്യ argc,
			   സ്ഥിരം അക്ഷരം **argv,
			   സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ force = 0;
	ഘടന option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	എങ്കിൽ (argc > 0)
		usage_with_options(update_server_info_usage, options);

	തിരികെ !!update_server_info(repo, force);
}
