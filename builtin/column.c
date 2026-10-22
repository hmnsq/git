#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "gettext.h"
#شامل "strbuf.h"
#شامل "parse-options.h"
#شامل "string-list.h"
#شامل "column.h"

ایستا ثابت نویسه * ثابت builtin_column_usage[] = {
	N_("git column [<options>]"),
	پوچ
};
ایستا unsigned صحیح colopts;

ایستا صحیح column_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			 ثابت ساختار config_context *ctx UNUSED, تهی *cb)
{
	بازگشت git_column_config(var, مقدار, cb, &colopts);
}

صحیح cmd_column(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo UNUSED)
{
	ساختار string_list list = STRING_LIST_INIT_DUP;
	ساختار strbuf sb = STRBUF_INIT;
	ساختار column_options copts;
	ثابت نویسه *command = پوچ, *real_command = پوچ;
	ساختار option options[] = {
		OPT_STRING(0, "command", &real_command, N_("name"), N_("lookup config vars")),
		OPT_COLUMN(0, "mode", &colopts, N_("layout to use")),
		OPT_UNSIGNED(0, "raw-mode", &colopts, N_("layout to use")),
		OPT_INTEGER(0, "width", &copts.width, N_("maximum width")),
		OPT_STRING(0, "indent", &copts.indent, N_("string"), N_("padding space on left border")),
		OPT_STRING(0, "nl", &copts.nl, N_("string"), N_("padding space on right border")),
		OPT_INTEGER(0, "padding", &copts.padding, N_("padding space between columns")),
		OPT_END()
	};

	/* This one is special and must be the first one */
	اگر (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, column_config, (تهی *)command);
	} وگرنه
		repo_config(the_repository, column_config, پوچ);

	memset(&copts, 0, اندازه(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, prefix, options, builtin_column_usage, 0);
	اگر (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	اگر (argc)
		usage_with_options(builtin_column_usage, options);
	اگر (real_command || command) {
		اگر (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	وقتی (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	بازگشت 0;
}
