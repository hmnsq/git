#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "column.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_column_usage[] = {
	N_("git column [<options>]"),
	ರಿಕ್ತ
};
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ colopts;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ column_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			 ಧ್ರುವ ಸಂರಚನಾ config_context *ctx UNUSED, ಶೂನ್ಯ *cb)
{
	ಪ್ರತ್ಯಾಗಮ git_column_config(var, ಮೌಲ್ಯ, cb, &colopts);
}

ಪೂರ್ಣಾಙ್ಕ cmd_column(ಪೂರ್ಣಾಙ್ಕ argc,
	       ಧ್ರುವ ವರ್ಣ **argv,
	       ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ string_list list = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಸಂರಚನಾ column_options copts;
	ಧ್ರುವ ವರ್ಣ *command = ರಿಕ್ತ, *real_command = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
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
	ಯದಿ (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, column_config, (ಶೂನ್ಯ *)command);
	} ಅನ್ಯಥಾ
		repo_config(the_repository, column_config, ರಿಕ್ತ);

	memset(&copts, 0, ಪರಿಮಾಣ(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, prefix, options, builtin_column_usage, 0);
	ಯದಿ (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	ಯದಿ (argc)
		usage_with_options(builtin_column_usage, options);
	ಯದಿ (real_command || command) {
		ಯದಿ (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	ಯಾವತ್ (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	ಪ್ರತ್ಯಾಗಮ 0;
}
