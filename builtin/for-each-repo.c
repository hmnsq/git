#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "string-list.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ run_command_on_repo(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം **argv)
{
	ഘടന child_process child = CHILD_PROCESS_INIT;
	അക്ഷരം *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, ശൂന്യ);
	strvec_pushv(&child.args, argv);

	സ്വതന്ത്രം(abspath);

	തിരികെ run_command(&child);
}

പൂർണ്ണസംഖ്യ cmd_for_each_repo(പൂർണ്ണസംഖ്യ argc,
		      സ്ഥിരം അക്ഷരം **argv,
		      സ്ഥിരം അക്ഷരം *prefix,
		      ഘടന repository *repo UNUSED)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം *config_താക്കോൽ = ശൂന്യ;
	പൂർണ്ണസംഖ്യ keep_going = 0;
	പൂർണ്ണസംഖ്യ result = 0;
	സ്ഥിരം ഘടന string_list *മൂല്യങ്ങൾ;
	പൂർണ്ണസംഖ്യ err;

	സ്ഥിരം ഘടന option options[] = {
		OPT_STRING(0, "config", &config_താക്കോൽ, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	എങ്കിൽ (!config_താക്കോൽ)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_താക്കോൽ, &മൂല്യങ്ങൾ);
	എങ്കിൽ (err < 0)
		usage_സന്ദേശം_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_താക്കോൽ);
	അല്ലെങ്കിൽ എങ്കിൽ (err)
		തിരികെ 0;

	വേണ്ടി (size_t i = 0; i < മൂല്യങ്ങൾ->nr; i++) {
		പൂർണ്ണസംഖ്യ ret = run_command_on_repo(മൂല്യങ്ങൾ->ഇനങ്ങൾ[i].string, argv);
		എങ്കിൽ (ret) {
			എങ്കിൽ (!keep_going)
					തിരികെ ret;
			result = 1;
		}
	}

	തിരികെ result;
}
