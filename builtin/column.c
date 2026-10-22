#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "column.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_ਕਾਲਮ_usage[] = {
	N_("git column [<options>]"),
	ਨਿਰਾ
};
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ colopts;

ਸਥਿਰ ਪੂਰਨ ਕਾਲਮ_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			 ਅਚਲ ਬਣਤਰ config_context *ctx ਅਣਵਰਤਿਆ, ਖਾਲੀ *cb)
{
	ਵਾਪਸ git_ਕਾਲਮ_config(var, ਮੁੱਲ, cb, &colopts);
}

ਪੂਰਨ cmd_ਕਾਲਮ(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ string_list list = STRING_LIST_INIT_DUP;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਬਣਤਰ ਕਾਲਮ_options copts;
	ਅਚਲ ਅੱਖਰ *command = ਨਿਰਾ, *real_command = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_STRING(0, "command", &real_command, N_("name"), N_("lookup config vars")),
		OPT_ਕਾਲਮ(0, "mode", &colopts, N_("layout to use")),
		OPT_UNSIGNED(0, "raw-mode", &colopts, N_("layout to use")),
		OPT_INTEGER(0, "width", &copts.ਚੌੜਾਈ, N_("maximum width")),
		OPT_STRING(0, "indent", &copts.indent, N_("string"), N_("padding space on left border")),
		OPT_STRING(0, "nl", &copts.nl, N_("string"), N_("padding space on right border")),
		OPT_INTEGER(0, "padding", &copts.padding, N_("padding space between columns")),
		OPT_ਸਿਰਾ()
	};

	/* This one is special and must be the first one */
	ਜੇ (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, ਕਾਲਮ_config, (ਖਾਲੀ *)command);
	} ਨਹੀਂਤਾਂ
		repo_config(the_repository, ਕਾਲਮ_config, ਨਿਰਾ);

	memset(&copts, 0, ਆਕਾਰ(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, builtin_ਕਾਲਮ_usage, 0);
	ਜੇ (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	ਜੇ (argc)
		usage_with_options(builtin_ਕਾਲਮ_usage, options);
	ਜੇ (real_command || command) {
		ਜੇ (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	ਜਦੋਂ (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	ਵਾਪਸ 0;
}
