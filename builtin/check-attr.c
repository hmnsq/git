#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "attr.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "write-or-die.h"

స్థిర పూర్ణాంకం all_attrs;
స్థిర పూర్ణాంకం cached_attrs;
స్థిర పూర్ణాంకం stdin_paths;
స్థిర అక్షరం *ఆధారం;
స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి check_attr_usage[] = {
N_("git check-attr [--source <tree-ish>] [-a | --all | <attr>...] [--] <pathname>..."),
N_("git check-attr --stdin [-z] [--source <tree-ish>] [-a | --all | <attr>...]"),
ఖాళీ
};

స్థిర పూర్ణాంకం nul_పదం_పంక్తి;

స్థిర స్థిరరాశి నిర్మాణం option check_attr_options[] = {
	OPT_BOOL('a', "all", &all_attrs, N_("report all attributes set on file")),
	OPT_BOOL(0,  "cached", &cached_attrs, N_("use .gitattributes only from the index")),
	OPT_BOOL(0 , "stdin", &stdin_paths, N_("read file names from stdin")),
	OPT_BOOL('z', ఖాళీ, &nul_పదం_పంక్తి,
		 N_("terminate input and output records by a NUL character")),
	OPT_STRING(0, "source", &ఆధారం, N_("<tree-ish>"), N_("which tree-ish to check attributes at")),
	OPT_చివర()
};

స్థిర శూన్య output_attr(నిర్మాణం attr_check *check, స్థిరరాశి అక్షరం *file)
{
	పూర్ణాంకం j;
	పూర్ణాంకం cnt = check->nr;

	కొరకు (j = 0; j < cnt; j++) {
		స్థిరరాశి అక్షరం *విలువ = check->వస్తువులు[j].విలువ;

		ఒకవేళ (ATTR_TRUE(విలువ))
			విలువ = "set";
		లేకపోతే ఒకవేళ (ATTR_FALSE(విలువ))
			విలువ = "unset";
		లేకపోతే ఒకవేళ (ATTR_UNSET(విలువ))
			విలువ = "unspecified";

		ఒకవేళ (nul_పదం_పంక్తి) {
			రాయి("%s%c" /*మార్గం*/
			       "%s%c" /* attrname */
			       "%s%c" /* attrvalue */,
			       file, 0,
			       git_attr_name(check->వస్తువులు[j].attr), 0, విలువ, 0);
		} లేకపోతే {
			quote_c_style(file, ఖాళీ, stdout, 0);
			రాయి(": %s: %s\n",
			       git_attr_name(check->వస్తువులు[j].attr), విలువ);
		}
	}
}

స్థిర శూన్య check_attr(స్థిరరాశి అక్షరం *ఉపసర్గ, నిర్మాణం attr_check *check,
		       పూర్ణాంకం collect_all,
		       స్థిరరాశి అక్షరం *file)

{
	అక్షరం *నిండిన_మార్గం =
		ఉపసర్గ_మార్గం(the_repository, ఉపసర్గ, ఉపసర్గ ? అక్షరశ్రేణి_పొడవు(ఉపసర్గ) : 0, file);

	ఒకవేళ (collect_all) {
		git_all_attrs(the_repository->సూచీ, నిండిన_మార్గం, check);
	} లేకపోతే {
		git_check_attr(the_repository->సూచీ, నిండిన_మార్గం, check);
	}
	output_attr(check, file);

	విడుదల(నిండిన_మార్గం);
}

స్థిర శూన్య check_attr_stdin_paths(స్థిరరాశి అక్షరం *ఉపసర్గ, నిర్మాణం attr_check *check,
				   పూర్ణాంకం collect_all)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం strbuf unquoted = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_పదం_పంక్తి ? strbuf_getline_nul : strbuf_getline_lf;
	అయితే (getline_fn(&buf, stdin) != EOF) {
		ఒకవేళ (!nul_పదం_పంక్తి && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ఒకవేళ (unquote_c_style(&unquoted, buf.buf, ఖాళీ))
				die("line is badly quoted");
			strbuf_మార్పిడి(&buf, &unquoted);
		}
		check_attr(ఉపసర్గ, check, collect_all, buf.buf);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

స్థిర NORETURN శూన్య error_with_usage(స్థిరరాశి అక్షరం *సందేశం)
{
	error("%s", సందేశం);
	usage_with_options(check_attr_usage, check_attr_options);
}

పూర్ణాంకం cmd_check_attr(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం attr_check *check;
	నిర్మాణం object_id initialized_oid;
	పూర్ణాంకం cnt, i, doubledash, filei;

	ఒకవేళ (!is_bare_repository(the_repository))
		అమరిక_work_వృక్షం(the_repository);

	repo_config(the_repository, git_default_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, check_attr_options,
			     check_attr_usage, PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	ఒకవేళ (repo_read_సూచీ(the_repository) < 0) {
		die("invalid cache");
	}

	ఒకవేళ (cached_attrs)
		git_attr_సమితి_direction(GIT_ATTR_సూచీ);

	doubledash = -1;
	కొరకు (i = 0; doubledash < 0 && i < argc; i++) {
		ఒకవేళ (!strcmp(argv[i], "--"))
			doubledash = i;
	}

	/* Process --all and/or attribute arguments: */
	ఒకవేళ (all_attrs) {
		ఒకవేళ (doubledash >= 1)
			error_with_usage("Attributes and --all both specified");

		cnt = 0;
		filei = doubledash + 1;
	} లేకపోతే ఒకవేళ (doubledash == 0) {
		error_with_usage("No attribute specified");
	} లేకపోతే ఒకవేళ (doubledash < 0) {
		ఒకవేళ (!argc)
			error_with_usage("No attribute specified");

		ఒకవేళ (stdin_paths) {
			/* Treat all arguments as attribute names. */
			cnt = argc;
			filei = argc;
		} లేకపోతే {
			/* Treat exactly one argument as an attribute name. */
			cnt = 1;
			filei = 1;
		}
	} లేకపోతే {
		cnt = doubledash;
		filei = doubledash + 1;
	}

	/* Check file argument(s): */
	ఒకవేళ (stdin_paths) {
		ఒకవేళ (filei < argc)
			error_with_usage("Can't specify files with --stdin");
	} లేకపోతే {
		ఒకవేళ (filei >= argc)
			error_with_usage("No file specified");
	}

	check = attr_check_alloc();
	ఒకవేళ (!all_attrs) {
		కొరకు (i = 0; i < cnt; i++) {
			స్థిరరాశి నిర్మాణం git_attr *a = git_attr(argv[i]);

			ఒకవేళ (!a)
				తిరిగి error("%s: not a valid attribute name",
					     argv[i]);
			attr_check_append(check, a);
		}
	}

	ఒకవేళ (ఆధారం) {
		ఒకవేళ (repo_get_oid_వృక్షం(the_repository, ఆధారం, &initialized_oid))
			die("%s: not a valid tree-ish source", ఆధారం);
		సమితి_git_attr_ఆధారం(ఆధారం);
	}

	ఒకవేళ (stdin_paths)
		check_attr_stdin_paths(ఉపసర్గ, check, all_attrs);
	లేకపోతే {
		కొరకు (i = filei; i < argc; i++)
			check_attr(ఉపసర్గ, check, all_attrs, argv[i]);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}

	attr_check_free(check);
	తిరిగి 0;
}
