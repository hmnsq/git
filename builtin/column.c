#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "column.h"

સ્થિર અચલ અક્ષર * અચલ builtin_column_usage[] = {
	N_("git column [<options>]"),
	ખાલી
};
સ્થિર unsigned પૂર્ણાંક colopts;

સ્થિર પૂર્ણાંક column_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			 અચલ સંરચના config_context *ctx UNUSED, શૂન્ય *cb)
{
	પાછા git_column_config(var, કિંમત, cb, &colopts);
}

પૂર્ણાંક cmd_column(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo UNUSED)
{
	સંરચના string_list list = STRING_LIST_INIT_DUP;
	સંરચના strbuf sb = STRBUF_INIT;
	સંરચના column_options copts;
	અચલ અક્ષર *command = ખાલી, *real_command = ખાલી;
	સંરચના option options[] = {
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
	જો (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, column_config, (શૂન્ય *)command);
	} નહીંતર
		repo_config(the_repository, column_config, ખાલી);

	memset(&copts, 0, કદ(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, prefix, options, builtin_column_usage, 0);
	જો (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	જો (argc)
		usage_with_options(builtin_column_usage, options);
	જો (real_command || command) {
		જો (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	જ્યારે (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	પાછા 0;
}
