#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "run-command.h"
#ضمن "string-list.h"

ساكن ثابت حرف * ثابت for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	عدم
};

ساكن صحيح لگاتار_command_on_repo(ثابت حرف *رستہ, ثابت حرف **argv)
{
	بنية بچہ_process بچہ = بچہ_PROCESS_INIT;
	حرف *abspath = interpolate_رستہ(رستہ, 0);

	sanitize_repo_env(&بچہ.env);

	بچہ.git_cmd = 1;
	strvec_pushl(&بچہ.دلیلاں, "-C", abspath, عدم);
	strvec_pushv(&بچہ.دلیلاں, argv);

	حرر(abspath);

	أرجع لگاتار_command(&بچہ);
}

صحيح cmd_for_each_repo(صحيح argc,
		      ثابت حرف **argv,
		      ثابت حرف *اگیتر,
		      بنية repository *repo بن_ورتیا)
{
	ساكن ثابت حرف *config_کلید = عدم;
	صحيح keep_going = 0;
	صحيح نتیجہ = 0;
	ثابت بنية string_list *اقدار;
	صحيح err;

	ثابت بنية option options[] = {
		OPT_STRING(0, "config", &config_کلید, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (!config_کلید)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_کلید, &اقدار);
	إذا (err < 0)
		usage_پیغام_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_کلید);
	وإلا إذا (err)
		أرجع 0;

	لكل (ناپ_t i = 0; i < اقدار->nr; i++) {
		صحيح ret = لگاتار_command_on_repo(اقدار->اشیاء[i].string, argv);
		إذا (ret) {
			إذا (!keep_going)
					أرجع ret;
			نتیجہ = 1;
		}
	}

	أرجع نتیجہ;
}
