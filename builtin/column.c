#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "gettext.h"
#شامل "strbuf.h"
#شامل "parse-options.h"
#شامل "string-list.h"
#شامل "column.h"

ساکن ثابت حرف * ثابت builtin_عمود_usage[] = {
	N_("git column [<options>]"),
	تہی
};
ساکن غیرعلامتی صحیح colopts;

ساکن صحیح عمود_config(ثابت حرف *var, ثابت حرف *قدر,
			 ثابت ساخت config_context *ctx غیر_مستعمل, خالی *cb)
{
	واپس git_عمود_config(var, قدر, cb, &colopts);
}

صحیح cmd_عمود(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo غیر_مستعمل)
{
	ساخت string_list list = STRING_LIST_INIT_DUP;
	ساخت strbuf sb = STRBUF_INIT;
	ساخت عمود_options copts;
	ثابت حرف *command = تہی, *real_command = تہی;
	ساخت option options[] = {
		OPT_STRING(0, "command", &real_command, N_("name"), N_("lookup config vars")),
		OPT_عمود(0, "mode", &colopts, N_("layout to use")),
		OPT_UNSIGNED(0, "raw-mode", &colopts, N_("layout to use")),
		OPT_INTEGER(0, "width", &copts.چوڑائی, N_("maximum width")),
		OPT_STRING(0, "indent", &copts.indent, N_("string"), N_("padding space on left border")),
		OPT_STRING(0, "nl", &copts.nl, N_("string"), N_("padding space on right border")),
		OPT_INTEGER(0, "padding", &copts.padding, N_("padding space between columns")),
		OPT_انتہا()
	};

	/* This one is special and must be the first one */
	اگر (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, عمود_config, (خالی *)command);
	} ورنہ
		repo_config(the_repository, عمود_config, تہی);

	memset(&copts, 0, حجم(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, سابقہ, options, builtin_عمود_usage, 0);
	اگر (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	اگر (argc)
		usage_with_options(builtin_عمود_usage, options);
	اگر (real_command || command) {
		اگر (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	جب (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	واپس 0;
}
