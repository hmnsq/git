#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "run-command.h"
#شامل "string-list.h"

ساکن ثابت حرف * ثابت for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	تہی
};

ساکن صحیح تسلسل_command_on_repo(ثابت حرف *راستہ, ثابت حرف **argv)
{
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;
	حرف *abspath = interpolate_راستہ(راستہ, 0);

	sanitize_repo_env(&بچہ.env);

	بچہ.git_cmd = 1;
	strvec_pushl(&بچہ.دلائل, "-C", abspath, تہی);
	strvec_pushv(&بچہ.دلائل, argv);

	آزاد(abspath);

	واپس تسلسل_command(&بچہ);
}

صحیح cmd_for_each_repo(صحیح argc,
		      ثابت حرف **argv,
		      ثابت حرف *سابقہ,
		      ساخت repository *repo غیر_مستعمل)
{
	ساکن ثابت حرف *config_کلید = تہی;
	صحیح keep_going = 0;
	صحیح نتیجہ = 0;
	ثابت ساخت string_list *اقدار;
	صحیح err;

	ثابت ساخت option options[] = {
		OPT_STRING(0, "config", &config_کلید, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	اگر (!config_کلید)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_کلید, &اقدار);
	اگر (err < 0)
		usage_پیغام_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_کلید);
	ورنہ اگر (err)
		واپس 0;

	برائے (سائز_t i = 0; i < اقدار->nr; i++) {
		صحیح ret = تسلسل_command_on_repo(اقدار->اشیاء[i].string, argv);
		اگر (ret) {
			اگر (!keep_going)
					واپس ret;
			نتیجہ = 1;
		}
	}

	واپس نتیجہ;
}
