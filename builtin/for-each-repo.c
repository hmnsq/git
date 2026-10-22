#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

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

ساكن صحيح لاڳيتو_command_on_repo(ثابت حرف *رستو, ثابت حرف **argv)
{
	بنية ٻار_process ٻار = ٻار_PROCESS_INIT;
	حرف *abspath = interpolate_رستو(رستو, 0);

	sanitize_repo_env(&ٻار.env);

	ٻار.git_cmd = 1;
	strvec_pushl(&ٻار.دليلون, "-C", abspath, عدم);
	strvec_pushv(&ٻار.دليلون, argv);

	حرر(abspath);

	أرجع لاڳيتو_command(&ٻار);
}

صحيح cmd_for_each_repo(صحيح argc,
		      ثابت حرف **argv,
		      ثابت حرف *اڳياڙي,
		      بنية repository *repo اڻ_استعمال_ٿيل)
{
	ساكن ثابت حرف *config_ڪنجي = عدم;
	صحيح keep_going = 0;
	صحيح نتيجو = 0;
	ثابت بنية string_list *قدرون;
	صحيح err;

	ثابت بنية option options[] = {
		OPT_STRING(0, "config", &config_ڪنجي, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (!config_ڪنجي)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_ڪنجي, &قدرون);
	إذا (err < 0)
		usage_نياپو_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_ڪنجي);
	وإلا إذا (err)
		أرجع 0;

	لكل (سائيز_t i = 0; i < قدرون->nr; i++) {
		صحيح ret = لاڳيتو_command_on_repo(قدرون->شيون[i].string, argv);
		إذا (ret) {
			إذا (!keep_going)
					أرجع ret;
			نتيجو = 1;
		}
	}

	أرجع نتيجو;
}
