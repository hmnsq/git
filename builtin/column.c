#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "strbuf.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "string-list.h"
#অন্তর্ভাৱ "column.h"

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ builtin_column_usage[] = {
	N_("git column [<options>]"),
	রিক্ত
};
স্থির unsigned পূর্ণাঙ্ক colopts;

স্থির পূর্ণাঙ্ক column_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
			 ধ্রুৱ সংরচনা config_context *ctx UNUSED, শূন্য *cb)
{
	ঘূৰাই git_column_config(var, মান, cb, &colopts);
}

পূর্ণাঙ্ক cmd_column(পূর্ণাঙ্ক argc,
	       ধ্রুৱ ৱর্ণ **argv,
	       ধ্রুৱ ৱর্ণ *prefix,
	       সংরচনা repository *repo UNUSED)
{
	সংরচনা string_list list = STRING_LIST_INIT_DUP;
	সংরচনা strbuf sb = STRBUF_INIT;
	সংরচনা column_options copts;
	ধ্রুৱ ৱর্ণ *command = রিক্ত, *real_command = রিক্ত;
	সংরচনা option options[] = {
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
	যদি (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, column_config, (শূন্য *)command);
	} অন্যথা
		repo_config(the_repository, column_config, রিক্ত);

	memset(&copts, 0, পরিমাণ(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, prefix, options, builtin_column_usage, 0);
	যদি (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	যদি (argc)
		usage_with_options(builtin_column_usage, options);
	যদি (real_command || command) {
		যদি (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	যেতিয়ালৈ (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	ঘূৰাই 0;
}
