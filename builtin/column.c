#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "column.h"

நிலையான மாறிலி எழுத்து * மாறிலி builtin_column_usage[] = {
	N_("git column [<options>]"),
	வெற்று
};
நிலையான unsigned முழு colopts;

நிலையான முழு column_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			 மாறிலி கட்டமைப்பு config_context *ctx UNUSED, வெறுமை *cb)
{
	திரும்பு git_column_config(var, மதிப்பு, cb, &colopts);
}

முழு cmd_column(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு string_list list = STRING_LIST_INIT_DUP;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	கட்டமைப்பு column_options copts;
	மாறிலி எழுத்து *command = வெற்று, *real_command = வெற்று;
	கட்டமைப்பு option options[] = {
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
	எனில் (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, column_config, (வெறுமை *)command);
	} இல்லையெனில்
		repo_config(the_repository, column_config, வெற்று);

	memset(&copts, 0, அளவு(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, prefix, options, builtin_column_usage, 0);
	எனில் (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	எனில் (argc)
		usage_with_options(builtin_column_usage, options);
	எனில் (real_command || command) {
		எனில் (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	போது (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	திரும்பு 0;
}
