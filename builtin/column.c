#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "gettext.h"
#समावेश "strbuf.h"
#समावेश "parse-options.h"
#समावेश "string-list.h"
#समावेश "column.h"

अटल स्थिर अक्षर * स्थिर builtin_column_usage[] = {
	N_("git column [<options>]"),
	शून्य
};
अटल अचिन्हित पूर्णांक colopts;

अटल पूर्णांक column_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			 स्थिर काष्ठा config_context *ctx UNUSED, व्योम *cb)
{
	वापस git_column_config(var, मूल्य, cb, &colopts);
}

पूर्णांक cmd_column(पूर्णांक argc,
	       स्थिर अक्षर **argv,
	       स्थिर अक्षर *prefix,
	       काष्ठा repository *repo UNUSED)
{
	काष्ठा string_list list = STRING_LIST_INIT_DUP;
	काष्ठा strbuf sb = STRBUF_INIT;
	काष्ठा column_options copts;
	स्थिर अक्षर *command = शून्य, *real_command = शून्य;
	काष्ठा option options[] = {
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
	अगर (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, column_config, (व्योम *)command);
	} अन्यथा
		repo_config(the_repository, column_config, शून्य);

	स_रखो(&copts, 0, माप(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, prefix, options, builtin_column_usage, 0);
	अगर (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	अगर (argc)
		usage_with_options(builtin_column_usage, options);
	अगर (real_command || command) {
		अगर (!real_command || !command || म_भेद(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	जबतक (!strbuf_getline(&sb, मानक_निवेश))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	वापस 0;
}
