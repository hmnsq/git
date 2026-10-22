#تعریف_کن USE_THE_REPOSITORY_VARIABLE

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

ساكن صحيح پیاپی_command_on_repo(ثابت حرف *مسیر, ثابت حرف **argv)
{
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;
	حرف *abspath = interpolate_مسیر(مسیر, 0);

	sanitize_repo_env(&فرزند.env);

	فرزند.git_cmd = 1;
	strvec_pushl(&فرزند.آرگومانها, "-C", abspath, عدم);
	strvec_pushv(&فرزند.آرگومانها, argv);

	حرر(abspath);

	أرجع پیاپی_command(&فرزند);
}

صحيح cmd_for_each_repo(صحيح argc,
		      ثابت حرف **argv,
		      ثابت حرف *پیشوند,
		      بنية repository *repo استفادهنشده)
{
	ساكن ثابت حرف *config_کلید = عدم;
	صحيح keep_going = 0;
	صحيح نتیجه = 0;
	ثابت بنية string_list *مقادیر;
	صحيح err;

	ثابت بنية option options[] = {
		OPT_STRING(0, "config", &config_کلید, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (!config_کلید)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_کلید, &مقادیر);
	إذا (err < 0)
		usage_پیام_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_کلید);
	وإلا إذا (err)
		أرجع 0;

	لكل (اندازه_t i = 0; i < مقادیر->nr; i++) {
		صحيح ret = پیاپی_command_on_repo(مقادیر->اقلام[i].string, argv);
		إذا (ret) {
			إذا (!keep_going)
					أرجع ret;
			نتیجه = 1;
		}
	}

	أرجع نتیجه;
}
