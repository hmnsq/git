#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "run-command.h"
#شامل "string-list.h"

ایستا ثابت نویسه * ثابت for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	پوچ
};

ایستا صحیح run_command_on_repo(ثابت نویسه *path, ثابت نویسه **argv)
{
	ساختار child_process child = CHILD_PROCESS_INIT;
	نویسه *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, پوچ);
	strvec_pushv(&child.args, argv);

	آزاد(abspath);

	بازگشت run_command(&child);
}

صحیح cmd_for_each_repo(صحیح argc,
		      ثابت نویسه **argv,
		      ثابت نویسه *prefix,
		      ساختار repository *repo UNUSED)
{
	ایستا ثابت نویسه *config_کلید = پوچ;
	صحیح keep_going = 0;
	صحیح result = 0;
	ثابت ساختار string_list *مقادیر;
	صحیح err;

	ثابت ساختار option options[] = {
		OPT_STRING(0, "config", &config_کلید, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	اگر (!config_کلید)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_کلید, &مقادیر);
	اگر (err < 0)
		usage_پیام_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_کلید);
	وگرنه اگر (err)
		بازگشت 0;

	برای (size_t i = 0; i < مقادیر->nr; i++) {
		صحیح ret = run_command_on_repo(مقادیر->اقلام[i].string, argv);
		اگر (ret) {
			اگر (!keep_going)
					بازگشت ret;
			result = 1;
		}
	}

	بازگشت result;
}
