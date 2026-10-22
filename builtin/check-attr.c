#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "attr.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "write-or-die.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ all_attrs;
സ്ഥിതം പൂർണ്ണസംഖ്യ cached_attrs;
സ്ഥിതം പൂർണ്ണസംഖ്യ stdin_paths;
സ്ഥിതം അക്ഷരം *source;
സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം check_attr_usage[] = {
N_("git check-attr [--source <tree-ish>] [-a | --all | <attr>...] [--] <pathname>..."),
N_("git check-attr --stdin [-z] [--source <tree-ish>] [-a | --all | <attr>...]"),
ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ nul_term_വരി;

സ്ഥിതം സ്ഥിരം ഘടന option check_attr_options[] = {
	OPT_BOOL('a', "all", &all_attrs, N_("report all attributes set on file")),
	OPT_BOOL(0,  "cached", &cached_attrs, N_("use .gitattributes only from the index")),
	OPT_BOOL(0 , "stdin", &stdin_paths, N_("read file names from stdin")),
	OPT_BOOL('z', ശൂന്യ, &nul_term_വരി,
		 N_("terminate input and output records by a NUL character")),
	OPT_STRING(0, "source", &source, N_("<tree-ish>"), N_("which tree-ish to check attributes at")),
	OPT_END()
};

സ്ഥിതം ശൂന്യം output_attr(ഘടന attr_check *check, സ്ഥിരം അക്ഷരം *file)
{
	പൂർണ്ണസംഖ്യ j;
	പൂർണ്ണസംഖ്യ cnt = check->nr;

	വേണ്ടി (j = 0; j < cnt; j++) {
		സ്ഥിരം അക്ഷരം *മൂല്യം = check->ഇനങ്ങൾ[j].മൂല്യം;

		എങ്കിൽ (ATTR_TRUE(മൂല്യം))
			മൂല്യം = "set";
		അല്ലെങ്കിൽ എങ്കിൽ (ATTR_FALSE(മൂല്യം))
			മൂല്യം = "unset";
		അല്ലെങ്കിൽ എങ്കിൽ (ATTR_UNSET(മൂല്യം))
			മൂല്യം = "unspecified";

		എങ്കിൽ (nul_term_വരി) {
			printf("%s%c" /* path */
			       "%s%c" /* attrname */
			       "%s%c" /* attrvalue */,
			       file, 0,
			       git_attr_name(check->ഇനങ്ങൾ[j].attr), 0, മൂല്യം, 0);
		} അല്ലെങ്കിൽ {
			quote_c_style(file, ശൂന്യ, stdout, 0);
			printf(": %s: %s\n",
			       git_attr_name(check->ഇനങ്ങൾ[j].attr), മൂല്യം);
		}
	}
}

സ്ഥിതം ശൂന്യം check_attr(സ്ഥിരം അക്ഷരം *prefix, ഘടന attr_check *check,
		       പൂർണ്ണസംഖ്യ collect_all,
		       സ്ഥിരം അക്ഷരം *file)

{
	അക്ഷരം *full_path =
		prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, file);

	എങ്കിൽ (collect_all) {
		git_all_attrs(the_repository->index, full_path, check);
	} അല്ലെങ്കിൽ {
		git_check_attr(the_repository->index, full_path, check);
	}
	output_attr(check, file);

	സ്വതന്ത്രം(full_path);
}

സ്ഥിതം ശൂന്യം check_attr_stdin_paths(സ്ഥിരം അക്ഷരം *prefix, ഘടന attr_check *check,
				   പൂർണ്ണസംഖ്യ collect_all)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന strbuf unquoted = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_വരി ? strbuf_getline_nul : strbuf_getline_lf;
	ആകുമ്പോൾ (getline_fn(&buf, stdin) != EOF) {
		എങ്കിൽ (!nul_term_വരി && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			എങ്കിൽ (unquote_c_style(&unquoted, buf.buf, ശൂന്യ))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		check_attr(prefix, check, collect_all, buf.buf);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

സ്ഥിതം NORETURN ശൂന്യം error_with_usage(സ്ഥിരം അക്ഷരം *സന്ദേശം)
{
	error("%s", സന്ദേശം);
	usage_with_options(check_attr_usage, check_attr_options);
}

പൂർണ്ണസംഖ്യ cmd_check_attr(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	ഘടന attr_check *check;
	ഘടന object_id initialized_oid;
	പൂർണ്ണസംഖ്യ cnt, i, doubledash, filei;

	എങ്കിൽ (!is_bare_repository(the_repository))
		setup_work_വൃക്ഷം(the_repository);

	repo_config(the_repository, git_default_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, check_attr_options,
			     check_attr_usage, PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	എങ്കിൽ (repo_read_index(the_repository) < 0) {
		die("invalid cache");
	}

	എങ്കിൽ (cached_attrs)
		git_attr_set_direction(GIT_ATTR_INDEX);

	doubledash = -1;
	വേണ്ടി (i = 0; doubledash < 0 && i < argc; i++) {
		എങ്കിൽ (!strcmp(argv[i], "--"))
			doubledash = i;
	}

	/* Process --all and/or attribute arguments: */
	എങ്കിൽ (all_attrs) {
		എങ്കിൽ (doubledash >= 1)
			error_with_usage("Attributes and --all both specified");

		cnt = 0;
		filei = doubledash + 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (doubledash == 0) {
		error_with_usage("No attribute specified");
	} അല്ലെങ്കിൽ എങ്കിൽ (doubledash < 0) {
		എങ്കിൽ (!argc)
			error_with_usage("No attribute specified");

		എങ്കിൽ (stdin_paths) {
			/* Treat all arguments as attribute names. */
			cnt = argc;
			filei = argc;
		} അല്ലെങ്കിൽ {
			/* Treat exactly one argument as an attribute name. */
			cnt = 1;
			filei = 1;
		}
	} അല്ലെങ്കിൽ {
		cnt = doubledash;
		filei = doubledash + 1;
	}

	/* Check file argument(s): */
	എങ്കിൽ (stdin_paths) {
		എങ്കിൽ (filei < argc)
			error_with_usage("Can't specify files with --stdin");
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (filei >= argc)
			error_with_usage("No file specified");
	}

	check = attr_check_alloc();
	എങ്കിൽ (!all_attrs) {
		വേണ്ടി (i = 0; i < cnt; i++) {
			സ്ഥിരം ഘടന git_attr *a = git_attr(argv[i]);

			എങ്കിൽ (!a)
				തിരികെ error("%s: not a valid attribute name",
					     argv[i]);
			attr_check_append(check, a);
		}
	}

	എങ്കിൽ (source) {
		എങ്കിൽ (repo_get_oid_വൃക്ഷം(the_repository, source, &initialized_oid))
			die("%s: not a valid tree-ish source", source);
		set_git_attr_source(source);
	}

	എങ്കിൽ (stdin_paths)
		check_attr_stdin_paths(prefix, check, all_attrs);
	അല്ലെങ്കിൽ {
		വേണ്ടി (i = filei; i < argc; i++)
			check_attr(prefix, check, all_attrs, argv[i]);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}

	attr_check_free(check);
	തിരികെ 0;
}
