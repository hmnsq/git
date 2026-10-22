#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "column.h"

स्थिर ध्रुव वर्ण * ध्रुव builtin_column_usage[] = {
	N_("git column [<options>]"),
	रिक्त
};
स्थिर unsigned पूर्णाङ्क colopts;

स्थिर पूर्णाङ्क column_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			 ध्रुव संरचना config_context *ctx UNUSED, शून्य *cb)
{
	पटिनिवत्त git_column_config(var, मानम्, cb, &colopts);
}

पूर्णाङ्क cmd_column(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *prefix,
	       संरचना repository *repo UNUSED)
{
	संरचना string_list list = STRING_LIST_INIT_DUP;
	संरचना strbuf sb = STRBUF_INIT;
	संरचना column_options copts;
	ध्रुव वर्ण *command = रिक्त, *real_command = रिक्त;
	संरचना option options[] = {
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
	सचे (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, column_config, (शून्य *)command);
	} अञ्ञथा
		repo_config(the_repository, column_config, रिक्त);

	memset(&copts, 0, परिमाण(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, prefix, options, builtin_column_usage, 0);
	सचे (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	सचे (argc)
		usage_with_options(builtin_column_usage, options);
	सचे (real_command || command) {
		सचे (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	याव (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	पटिनिवत्त 0;
}
