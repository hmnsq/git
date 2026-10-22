#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "attr.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_attrs;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cached_attrs;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdin_paths;
ସ୍ଥିର ଅକ୍ଷର *source;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ check_attr_usage[] = {
N_("git check-attr [--source <tree-ish>] [-a | --all | <attr>...] [--] <pathname>..."),
N_("git check-attr --stdin [-z] [--source <tree-ish>] [-a | --all | <attr>...]"),
ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nul_term_ଧାଡ଼ି;

ସ୍ଥିର ଅଚଳ ସଂରଚନା option check_attr_options[] = {
	OPT_BOOL('a', "all", &all_attrs, N_("report all attributes set on file")),
	OPT_BOOL(0,  "cached", &cached_attrs, N_("use .gitattributes only from the index")),
	OPT_BOOL(0 , "stdin", &stdin_paths, N_("read file names from stdin")),
	OPT_BOOL('z', ଖାଲି, &nul_term_ଧାଡ଼ି,
		 N_("terminate input and output records by a NUL character")),
	OPT_STRING(0, "source", &source, N_("<tree-ish>"), N_("which tree-ish to check attributes at")),
	OPT_END()
};

ସ୍ଥିର ଶୂନ୍ୟ output_attr(ସଂରଚନା attr_check *check, ଅଚଳ ଅକ୍ଷର *file)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnt = check->nr;

	ପାଇଁ (j = 0; j < cnt; j++) {
		ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ = check->ବସ୍ତୁଗୁଡ଼ିକ[j].ମୂଲ୍ୟ;

		ଯଦି (ATTR_TRUE(ମୂଲ୍ୟ))
			ମୂଲ୍ୟ = "set";
		ଅନ୍ୟଥା ଯଦି (ATTR_FALSE(ମୂଲ୍ୟ))
			ମୂଲ୍ୟ = "unset";
		ଅନ୍ୟଥା ଯଦି (ATTR_UNSET(ମୂଲ୍ୟ))
			ମୂଲ୍ୟ = "unspecified";

		ଯଦି (nul_term_ଧାଡ଼ି) {
			printf("%s%c" /* path */
			       "%s%c" /* attrname */
			       "%s%c" /* attrvalue */,
			       file, 0,
			       git_attr_name(check->ବସ୍ତୁଗୁଡ଼ିକ[j].attr), 0, ମୂଲ୍ୟ, 0);
		} ଅନ୍ୟଥା {
			quote_c_style(file, ଖାଲି, stdout, 0);
			printf(": %s: %s\n",
			       git_attr_name(check->ବସ୍ତୁଗୁଡ଼ିକ[j].attr), ମୂଲ୍ୟ);
		}
	}
}

ସ୍ଥିର ଶୂନ୍ୟ check_attr(ଅଚଳ ଅକ୍ଷର *prefix, ସଂରଚନା attr_check *check,
		       ପୂର୍ଣ୍ଣସଂଖ୍ୟା collect_all,
		       ଅଚଳ ଅକ୍ଷର *file)

{
	ଅକ୍ଷର *full_path =
		prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, file);

	ଯଦି (collect_all) {
		git_all_attrs(the_repository->index, full_path, check);
	} ଅନ୍ୟଥା {
		git_check_attr(the_repository->index, full_path, check);
	}
	output_attr(check, file);

	ମୁକ୍ତ(full_path);
}

ସ୍ଥିର ଶୂନ୍ୟ check_attr_stdin_paths(ଅଚଳ ଅକ୍ଷର *prefix, ସଂରଚନା attr_check *check,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା collect_all)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା strbuf unquoted = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_ଧାଡ଼ି ? strbuf_getline_nul : strbuf_getline_lf;
	ଯେତେବେଳେ (getline_fn(&buf, stdin) != EOF) {
		ଯଦି (!nul_term_ଧାଡ଼ି && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ଯଦି (unquote_c_style(&unquoted, buf.buf, ଖାଲି))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		check_attr(prefix, check, collect_all, buf.buf);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

ସ୍ଥିର NORETURN ଶୂନ୍ୟ error_with_usage(ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ)
{
	error("%s", ସନ୍ଦେଶ);
	usage_with_options(check_attr_usage, check_attr_options);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_check_attr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		   ଅଚଳ ଅକ୍ଷର **argv,
		   ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା attr_check *check;
	ସଂରଚନା object_id initialized_oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnt, i, doubledash, filei;

	ଯଦି (!is_bare_repository(the_repository))
		setup_work_ବୃକ୍ଷ(the_repository);

	repo_config(the_repository, git_default_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, check_attr_options,
			     check_attr_usage, PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ଯଦି (repo_read_index(the_repository) < 0) {
		die("invalid cache");
	}

	ଯଦି (cached_attrs)
		git_attr_set_direction(GIT_ATTR_INDEX);

	doubledash = -1;
	ପାଇଁ (i = 0; doubledash < 0 && i < argc; i++) {
		ଯଦି (!strcmp(argv[i], "--"))
			doubledash = i;
	}

	/* Process --all and/or attribute arguments: */
	ଯଦି (all_attrs) {
		ଯଦି (doubledash >= 1)
			error_with_usage("Attributes and --all both specified");

		cnt = 0;
		filei = doubledash + 1;
	} ଅନ୍ୟଥା ଯଦି (doubledash == 0) {
		error_with_usage("No attribute specified");
	} ଅନ୍ୟଥା ଯଦି (doubledash < 0) {
		ଯଦି (!argc)
			error_with_usage("No attribute specified");

		ଯଦି (stdin_paths) {
			/* Treat all arguments as attribute names. */
			cnt = argc;
			filei = argc;
		} ଅନ୍ୟଥା {
			/* Treat exactly one argument as an attribute name. */
			cnt = 1;
			filei = 1;
		}
	} ଅନ୍ୟଥା {
		cnt = doubledash;
		filei = doubledash + 1;
	}

	/* Check file argument(s): */
	ଯଦି (stdin_paths) {
		ଯଦି (filei < argc)
			error_with_usage("Can't specify files with --stdin");
	} ଅନ୍ୟଥା {
		ଯଦି (filei >= argc)
			error_with_usage("No file specified");
	}

	check = attr_check_alloc();
	ଯଦି (!all_attrs) {
		ପାଇଁ (i = 0; i < cnt; i++) {
			ଅଚଳ ସଂରଚନା git_attr *a = git_attr(argv[i]);

			ଯଦି (!a)
				ଫେରନ୍ତୁ error("%s: not a valid attribute name",
					     argv[i]);
			attr_check_append(check, a);
		}
	}

	ଯଦି (source) {
		ଯଦି (repo_get_oid_ବୃକ୍ଷ(the_repository, source, &initialized_oid))
			die("%s: not a valid tree-ish source", source);
		set_git_attr_source(source);
	}

	ଯଦି (stdin_paths)
		check_attr_stdin_paths(prefix, check, all_attrs);
	ଅନ୍ୟଥା {
		ପାଇଁ (i = filei; i < argc; i++)
			check_attr(prefix, check, all_attrs, argv[i]);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}

	attr_check_free(check);
	ଫେରନ୍ତୁ 0;
}
