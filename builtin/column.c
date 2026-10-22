#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "gettext.h"
#ضمن "strbuf.h"
#ضمن "parse-options.h"
#ضمن "string-list.h"
#ضمن "column.h"

ساكن ثابت حرف * ثابت builtin_کالم_usage[] = {
	N_("git column [<options>]"),
	عدم
};
ساكن بنانشانی صحيح colopts;

ساكن صحيح کالم_config(ثابت حرف *var, ثابت حرف *قدر,
			 ثابت بنية config_context *ctx بن_ورتیا, فراغ *cb)
{
	أرجع git_کالم_config(var, قدر, cb, &colopts);
}

صحيح cmd_کالم(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *اگیتر,
	       بنية repository *repo بن_ورتیا)
{
	بنية string_list list = STRING_LIST_INIT_DUP;
	بنية strbuf sb = STRBUF_INIT;
	بنية کالم_options copts;
	ثابت حرف *command = عدم, *real_command = عدم;
	بنية option options[] = {
		OPT_STRING(0, "command", &real_command, N_("name"), N_("lookup config vars")),
		OPT_کالم(0, "mode", &colopts, N_("layout to use")),
		OPT_UNSIGNED(0, "raw-mode", &colopts, N_("layout to use")),
		OPT_INTEGER(0, "width", &copts.چوڑائی, N_("maximum width")),
		OPT_STRING(0, "indent", &copts.indent, N_("string"), N_("padding space on left border")),
		OPT_STRING(0, "nl", &copts.nl, N_("string"), N_("padding space on right border")),
		OPT_INTEGER(0, "padding", &copts.padding, N_("padding space between columns")),
		OPT_سرا()
	};

	/* This one is special and must be the first one */
	إذا (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, کالم_config, (فراغ *)command);
	} وإلا
		repo_config(the_repository, کالم_config, عدم);

	memset(&copts, 0, حجم(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, اگیتر, options, builtin_کالم_usage, 0);
	إذا (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	إذا (argc)
		usage_with_options(builtin_کالم_usage, options);
	إذا (real_command || command) {
		إذا (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	طالما (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	أرجع 0;
}
