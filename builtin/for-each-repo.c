#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "string-list.h"

સ્થિર અચલ અક્ષર * અચલ for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	ખાલી
};

સ્થિર પૂર્ણાંક run_command_on_repo(અચલ અક્ષર *path, અચલ અક્ષર **argv)
{
	સંરચના child_process child = CHILD_PROCESS_INIT;
	અક્ષર *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, ખાલી);
	strvec_pushv(&child.args, argv);

	મુક્ત(abspath);

	પાછા run_command(&child);
}

પૂર્ણાંક cmd_for_each_repo(પૂર્ણાંક argc,
		      અચલ અક્ષર **argv,
		      અચલ અક્ષર *prefix,
		      સંરચના repository *repo UNUSED)
{
	સ્થિર અચલ અક્ષર *config_ચાવી = ખાલી;
	પૂર્ણાંક keep_going = 0;
	પૂર્ણાંક result = 0;
	અચલ સંરચના string_list *કિંમતો;
	પૂર્ણાંક err;

	અચલ સંરચના option options[] = {
		OPT_STRING(0, "config", &config_ચાવી, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	જો (!config_ચાવી)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_ચાવી, &કિંમતો);
	જો (err < 0)
		usage_સંદેશ_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_ચાવી);
	નહીંતર જો (err)
		પાછા 0;

	માટે (size_t i = 0; i < કિંમતો->nr; i++) {
		પૂર્ણાંક ret = run_command_on_repo(કિંમતો->વસ્તુઓ[i].string, argv);
		જો (ret) {
			જો (!keep_going)
					પાછા ret;
			result = 1;
		}
	}

	પાછા result;
}
