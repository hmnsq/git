#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "string-list.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	ফাঁকা
};

স্থির পূর্ণসংখ্যা run_command_on_repo(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর **argv)
{
	কাঠামো child_process child = CHILD_PROCESS_INIT;
	অক্ষর *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, ফাঁকা);
	strvec_pushv(&child.args, argv);

	মুক্ত(abspath);

	ফেরত run_command(&child);
}

পূর্ণসংখ্যা cmd_for_each_repo(পূর্ণসংখ্যা argc,
		      ধ্রুবক অক্ষর **argv,
		      ধ্রুবক অক্ষর *prefix,
		      কাঠামো repository *repo UNUSED)
{
	স্থির ধ্রুবক অক্ষর *config_চাবি = ফাঁকা;
	পূর্ণসংখ্যা keep_going = 0;
	পূর্ণসংখ্যা ফলাফল = 0;
	ধ্রুবক কাঠামো string_list *মানসমূহ;
	পূর্ণসংখ্যা err;

	ধ্রুবক কাঠামো option options[] = {
		OPT_STRING(0, "config", &config_চাবি, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	যদি (!config_চাবি)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_চাবি, &মানসমূহ);
	যদি (err < 0)
		usage_বার্তা_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_চাবি);
	অন্যথা যদি (err)
		ফেরত 0;

	জন্য (আকার_t i = 0; i < মানসমূহ->nr; i++) {
		পূর্ণসংখ্যা ret = run_command_on_repo(মানসমূহ->বস্তুসমূহ[i].string, argv);
		যদি (ret) {
			যদি (!keep_going)
					ফেরত ret;
			ফলাফল = 1;
		}
	}

	ফেরত ফলাফল;
}
