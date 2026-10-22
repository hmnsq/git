#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "server-info.h"

நிலையான மாறிலி எழுத்து * மாறிலி update_server_info_usage[] = {
	"git update-server-info [-f | --force]",
	வெற்று
};

முழு cmd_update_server_info(முழு argc,
			   மாறிலி எழுத்து **argv,
			   மாறிலி எழுத்து *prefix,
			   கட்டமைப்பு repository *repo)
{
	முழு force = 0;
	கட்டமைப்பு option options[] = {
		OPT__FORCE(&force, N_("update the info files from scratch"), 0),
		OPT_END()
	};

	repo_config(repo, git_default_config, வெற்று);

	argc = parse_options(argc, argv, prefix, options,
			     update_server_info_usage, 0);
	எனில் (argc > 0)
		usage_with_options(update_server_info_usage, options);

	திரும்பு !!update_server_info(repo, force);
}
