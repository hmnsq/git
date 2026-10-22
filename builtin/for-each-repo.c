#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "string-list.h"

நிலையான மாறிலி எழுத்து * மாறிலி for_each_repo_usage[] = {
	N_("git for-each-repo --config=<config> [--] <arguments>"),
	வெற்று
};

நிலையான முழு run_command_on_repo(மாறிலி எழுத்து *path, மாறிலி எழுத்து **argv)
{
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;
	எழுத்து *abspath = interpolate_path(path, 0);

	sanitize_repo_env(&child.env);

	child.git_cmd = 1;
	strvec_pushl(&child.args, "-C", abspath, வெற்று);
	strvec_pushv(&child.args, argv);

	விடுவி(abspath);

	திரும்பு run_command(&child);
}

முழு cmd_for_each_repo(முழு argc,
		      மாறிலி எழுத்து **argv,
		      மாறிலி எழுத்து *prefix,
		      கட்டமைப்பு repository *repo UNUSED)
{
	நிலையான மாறிலி எழுத்து *config_சாவி = வெற்று;
	முழு keep_going = 0;
	முழு result = 0;
	மாறிலி கட்டமைப்பு string_list *மதிப்புகள்;
	முழு err;

	மாறிலி கட்டமைப்பு option options[] = {
		OPT_STRING(0, "config", &config_சாவி, N_("config"),
			   N_("config key storing a list of repository paths")),
		OPT_BOOL(0, "keep-going", &keep_going,
			 N_("keep going even if command fails in a repository")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, for_each_repo_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	எனில் (!config_சாவி)
		die(_("missing --config=<config>"));

	err = repo_config_get_string_multi(the_repository, config_சாவி, &மதிப்புகள்);
	எனில் (err < 0)
		usage_செய்தி_optf(_("got bad config --config=%s"),
			       for_each_repo_usage, options, config_சாவி);
	இல்லையெனில் எனில் (err)
		திரும்பு 0;

	க்கு (size_t i = 0; i < மதிப்புகள்->nr; i++) {
		முழு ret = run_command_on_repo(மதிப்புகள்->பொருட்கள்[i].string, argv);
		எனில் (ret) {
			எனில் (!keep_going)
					திரும்பு ret;
			result = 1;
		}
	}

	திரும்பு result;
}
