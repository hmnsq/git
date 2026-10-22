#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "column.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_ಸ್ತಂಭ_usage[] = {
	N_("git column [<options>]"),
	ಖಾಲಿ
};
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ colopts;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸ್ತಂಭ_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx ಬಳಸದ, ಶೂನ್ಯ *cb)
{
	ಹಿಂತಿರುಗಿ git_ಸ್ತಂಭ_config(var, ಮೌಲ್ಯ, cb, &colopts);
}

ಪೂರ್ಣಾಂಕ cmd_ಸ್ತಂಭ(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ string_list list = STRING_LIST_INIT_DUP;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ರಚನೆ ಸ್ತಂಭ_options copts;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *command = ಖಾಲಿ, *real_command = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_STRING(0, "command", &real_command, N_("name"), N_("lookup config vars")),
		OPT_ಸ್ತಂಭ(0, "mode", &colopts, N_("layout to use")),
		OPT_UNSIGNED(0, "raw-mode", &colopts, N_("layout to use")),
		OPT_INTEGER(0, "width", &copts.ಅಗಲ, N_("maximum width")),
		OPT_STRING(0, "indent", &copts.indent, N_("string"), N_("padding space on left border")),
		OPT_STRING(0, "nl", &copts.nl, N_("string"), N_("padding space on right border")),
		OPT_INTEGER(0, "padding", &copts.padding, N_("padding space between columns")),
		OPT_ಕೊನೆ()
	};

	/* This one is special and must be the first one */
	ಆದರೆ (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, ಸ್ತಂಭ_config, (ಶೂನ್ಯ *)command);
	} ಇಲ್ಲದಿದ್ದರೆ
		repo_config(the_repository, ಸ್ತಂಭ_config, ಖಾಲಿ);

	memset(&copts, 0, ಗಾತ್ರ(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, builtin_ಸ್ತಂಭ_usage, 0);
	ಆದರೆ (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	ಆದರೆ (argc)
		usage_with_options(builtin_ಸ್ತಂಭ_usage, options);
	ಆದರೆ (real_command || command) {
		ಆದರೆ (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	ಆಗ (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	ಹಿಂತಿರುಗಿ 0;
}
