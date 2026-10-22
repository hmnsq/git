#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_command_on_repo(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର **argv)
{
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;
	ଅକ୍ଷର *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, ଖାଲି);
	strvec_pushv(&child.args, argv);

	ମୁକ୍ତ(abspath);

	ଫେରନ୍ତୁ run_command(&child);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_for_each_repo(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		      ଅଚଳ ଅକ୍ଷର **argv,
		      ଅଚଳ ଅକ୍ଷର *prefix,
		      ସଂରଚନା repository *repo UNUSED)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *config_ଚାବି = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_going = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ଅଚଳ ସଂରଚନା string_list *ମୂଲ୍ୟଗୁଡ଼ିକ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

	ଅଚଳ ସଂରଚନା option options[] = {
		OPT_STRING(0, "config", &config_ଚାବି, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ଯଦି (!config_ଚାବି)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_ଚାବି, &ମୂଲ୍ୟଗୁଡ଼ିକ);
	ଯଦି (err < 0)
		usage_ସନ୍ଦେଶ_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_ଚାବି);
	ଅନ୍ୟଥା ଯଦି (err)
		ଫେରନ୍ତୁ 0;

	ପାଇଁ (size_t i = 0; i < ମୂଲ୍ୟଗୁଡ଼ିକ->nr; i++) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = run_command_on_repo(ମୂଲ୍ୟଗୁଡ଼ିକ->ବସ୍ତୁଗୁଡ଼ିକ[i].string, argv);
		ଯଦି (ret) {
			ଯଦି (!keep_going)
					ଫେରନ୍ତୁ ret;
			result = 1;
		}
	}

	ଫେରନ୍ତୁ result;
}
