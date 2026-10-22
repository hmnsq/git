#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_command_on_repo(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;
	ਅੱਖਰ *abspath = interpolate_ਰਸਤਾ(ਰਸਤਾ, 0);

	sanitize_repo_env(&ਬੱਚਾ.env);

	ਬੱਚਾ.git_cmd = 1;
	strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "-C", abspath, ਨਿਰਾ);
	strvec_pushv(&ਬੱਚਾ.ਦਲੀਲਾਂ, argv);

	ਮੁਕਤ(abspath);

	ਵਾਪਸ ਲੜੀਵਾਰ_command(&ਬੱਚਾ);
}

ਪੂਰਨ cmd_for_each_repo(ਪੂਰਨ argc,
		      ਅਚਲ ਅੱਖਰ **argv,
		      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *config_ਕੁੰਜੀ = ਨਿਰਾ;
	ਪੂਰਨ keep_going = 0;
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਅਚਲ ਬਣਤਰ string_list *ਮੁੱਲਾਂ;
	ਪੂਰਨ err;

	ਅਚਲ ਬਣਤਰ option options[] = {
		OPT_STRING(0, "config", &config_ਕੁੰਜੀ, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ਜੇ (!config_ਕੁੰਜੀ)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_ਕੁੰਜੀ, &ਮੁੱਲਾਂ);
	ਜੇ (err < 0)
		usage_ਸੁਨੇਹਾ_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_ਕੁੰਜੀ);
	ਨਹੀਂਤਾਂ ਜੇ (err)
		ਵਾਪਸ 0;

	ਲਈ (ਮਾਪ_t i = 0; i < ਮੁੱਲਾਂ->nr; i++) {
		ਪੂਰਨ ret = ਲੜੀਵਾਰ_command_on_repo(ਮੁੱਲਾਂ->ਚੀਜ਼ਾਂ[i].string, argv);
		ਜੇ (ret) {
			ਜੇ (!keep_going)
					ਵਾਪਸ ret;
			ਨਤੀਜਾ = 1;
		}
	}

	ਵਾਪਸ ਨਤੀਜਾ;
}
