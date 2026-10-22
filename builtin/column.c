#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "column.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_స్తంభం_usage[] = {
	N_("git column [<options>]"),
	ఖాళీ
};
స్థిర గుర్తులేని పూర్ణాంకం colopts;

స్థిర పూర్ణాంకం స్తంభం_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			 స్థిరరాశి నిర్మాణం config_context *ctx వాడని, శూన్య *cb)
{
	తిరిగి git_స్తంభం_config(var, విలువ, cb, &colopts);
}

పూర్ణాంకం cmd_స్తంభం(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం string_list list = STRING_LIST_INIT_DUP;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	నిర్మాణం స్తంభం_options copts;
	స్థిరరాశి అక్షరం *command = ఖాళీ, *real_command = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_STRING(0, "command", &real_command, N_("name"), N_("lookup config vars")),
		OPT_స్తంభం(0, "mode", &colopts, N_("layout to use")),
		OPT_UNSIGNED(0, "raw-mode", &colopts, N_("layout to use")),
		OPT_INTEGER(0, "width", &copts.వెడల్పు, N_("maximum width")),
		OPT_STRING(0, "indent", &copts.indent, N_("string"), N_("padding space on left border")),
		OPT_STRING(0, "nl", &copts.nl, N_("string"), N_("padding space on right border")),
		OPT_INTEGER(0, "padding", &copts.padding, N_("padding space between columns")),
		OPT_చివర()
	};

	/* This one is special and must be the first one */
	ఒకవేళ (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, స్తంభం_config, (శూన్య *)command);
	} లేకపోతే
		repo_config(the_repository, స్తంభం_config, ఖాళీ);

	memset(&copts, 0, పరిమాణం(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, ఉపసర్గ, options, builtin_స్తంభం_usage, 0);
	ఒకవేళ (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	ఒకవేళ (argc)
		usage_with_options(builtin_స్తంభం_usage, options);
	ఒకవేళ (real_command || command) {
		ఒకవేళ (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	అయితే (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	తిరిగి 0;
}
