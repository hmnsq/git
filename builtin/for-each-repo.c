#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

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

ساكن صحيح پرلپسې_command_on_repo(ثابت حرف *لار, ثابت حرف **argv)
{
	بنية ماشوم_process ماشوم = ماشوم_PROCESS_INIT;
	حرف *abspath = interpolate_لار(لار, 0);

	sanitize_repo_env(&ماشوم.env);

	ماشوم.git_cmd = 1;
	strvec_pushl(&ماشوم.دلیلونه, "-C", abspath, عدم);
	strvec_pushv(&ماشوم.دلیلونه, argv);

	حرر(abspath);

	أرجع پرلپسې_command(&ماشوم);
}

صحيح cmd_for_each_repo(صحيح argc,
		      ثابت حرف **argv,
		      ثابت حرف *مختاړی,
		      بنية repository *repo ناکارول_شوی)
{
	ساكن ثابت حرف *config_کلی = عدم;
	صحيح keep_going = 0;
	صحيح پایله = 0;
	ثابت بنية string_list *ارزښتونه;
	صحيح err;

	ثابت بنية option options[] = {
		OPT_STRING(0, "config", &config_کلی, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (!config_کلی)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_کلی, &ارزښتونه);
	إذا (err < 0)
		usage_پیغام_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_کلی);
	وإلا إذا (err)
		أرجع 0;

	لكل (اندازه_t i = 0; i < ارزښتونه->nr; i++) {
		صحيح ret = پرلپسې_command_on_repo(ارزښتونه->توکي[i].string, argv);
		إذا (ret) {
			إذا (!keep_going)
					أرجع ret;
			پایله = 1;
		}
	}

	أرجع پایله;
}
