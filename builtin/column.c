#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "column.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_column_usage[] = {
	N_("git column [<options>]"),
	ശൂന്യ
};
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ colopts;

സ്ഥിതം പൂർണ്ണസംഖ്യ column_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			 സ്ഥിരം ഘടന config_context *ctx UNUSED, ശൂന്യം *cb)
{
	തിരികെ git_column_config(var, മൂല്യം, cb, &colopts);
}

പൂർണ്ണസംഖ്യ cmd_column(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo UNUSED)
{
	ഘടന string_list list = STRING_LIST_INIT_DUP;
	ഘടന strbuf sb = STRBUF_INIT;
	ഘടന column_options copts;
	സ്ഥിരം അക്ഷരം *command = ശൂന്യ, *real_command = ശൂന്യ;
	ഘടന option options[] = {
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
	എങ്കിൽ (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, column_config, (ശൂന്യം *)command);
	} അല്ലെങ്കിൽ
		repo_config(the_repository, column_config, ശൂന്യ);

	memset(&copts, 0, വലുപ്പം(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, prefix, options, builtin_column_usage, 0);
	എങ്കിൽ (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	എങ്കിൽ (argc)
		usage_with_options(builtin_column_usage, options);
	എങ്കിൽ (real_command || command) {
		എങ്കിൽ (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	ആകുമ്പോൾ (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	തിരികെ 0;
}
