#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "column.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_column_usage[] = {
	N_("git column [<options>]"),
	ଖାଲି
};
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା colopts;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା column_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			 ଅଚଳ ସଂରଚନା config_context *ctx UNUSED, ଶୂନ୍ୟ *cb)
{
	ଫେରନ୍ତୁ git_column_config(var, ମୂଲ୍ୟ, cb, &colopts);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_column(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	       ଅଚଳ ଅକ୍ଷର **argv,
	       ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା string_list list = STRING_LIST_INIT_DUP;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ସଂରଚନା column_options copts;
	ଅଚଳ ଅକ୍ଷର *command = ଖାଲି, *real_command = ଖାଲି;
	ସଂରଚନା option options[] = {
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
	ଯଦି (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, column_config, (ଶୂନ୍ୟ *)command);
	} ଅନ୍ୟଥା
		repo_config(the_repository, column_config, ଖାଲି);

	memset(&copts, 0, ଆକାର(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, prefix, options, builtin_column_usage, 0);
	ଯଦି (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	ଯଦି (argc)
		usage_with_options(builtin_column_usage, options);
	ଯଦି (real_command || command) {
		ଯଦି (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	ଯେତେବେଳେ (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	ଫେରନ୍ତୁ 0;
}
