#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "server-info.h"

સ્થિર અચલ અક્ષર * અચલ update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	ખાલી
};

પૂર્ણાંક cmd_update_server_info(પૂર્ણાંક argc,
			   અચલ અક્ષર **argv,
			   અચલ અક્ષર *prefix,
			   સંરચના repository *repo)
{
	પૂર્ણાંક force = 0;
	સંરચના option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, ખાલી);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	જો (argc > 0)
		usage_with_options(update_server_info_usage, options);

	પાછા !!update_server_info(repo, force);
}
