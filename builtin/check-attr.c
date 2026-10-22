#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "attr.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "write-or-die.h"

நிலையான முழு all_attrs;
நிலையான முழு cached_attrs;
நிலையான முழு stdin_paths;
நிலையான எழுத்து *source;
நிலையான மாறிலி எழுத்து * மாறிலி check_attr_usage[] = {
N_("git check-attr [--source <tree-ish>] [-a | --all | <attr>...] [--] <pathname>..."),
N_("git check-attr --stdin [-z] [--source <tree-ish>] [-a | --all | <attr>...]"),
வெற்று
};

நிலையான முழு nul_term_வரி;

நிலையான மாறிலி கட்டமைப்பு option check_attr_options[] = {
	OPT_BOOL('a', "all", &all_attrs, N_("report all attributes set on file")),
	OPT_BOOL(0,  "cached", &cached_attrs, N_("use .gitattributes only from the index")),
	OPT_BOOL(0 , "stdin", &stdin_paths, N_("read file names from stdin")),
	OPT_BOOL('z', வெற்று, &nul_term_வரி,
		 N_("terminate input and output records by a NUL character")),
	OPT_STRING(0, "source", &source, N_("<tree-ish>"), N_("which tree-ish to check attributes at")),
	OPT_END()
};

நிலையான வெறுமை output_attr(கட்டமைப்பு attr_check *check, மாறிலி எழுத்து *file)
{
	முழு j;
	முழு cnt = check->nr;

	க்கு (j = 0; j < cnt; j++) {
		மாறிலி எழுத்து *மதிப்பு = check->பொருட்கள்[j].மதிப்பு;

		எனில் (ATTR_TRUE(மதிப்பு))
			மதிப்பு = "set";
		இல்லையெனில் எனில் (ATTR_FALSE(மதிப்பு))
			மதிப்பு = "unset";
		இல்லையெனில் எனில் (ATTR_UNSET(மதிப்பு))
			மதிப்பு = "unspecified";

		எனில் (nul_term_வரி) {
			printf("%s%c" /* path */
			       "%s%c" /* attrname */
			       "%s%c" /* attrvalue */,
			       file, 0,
			       git_attr_name(check->பொருட்கள்[j].attr), 0, மதிப்பு, 0);
		} இல்லையெனில் {
			quote_c_style(file, வெற்று, stdout, 0);
			printf(": %s: %s\n",
			       git_attr_name(check->பொருட்கள்[j].attr), மதிப்பு);
		}
	}
}

நிலையான வெறுமை check_attr(மாறிலி எழுத்து *prefix, கட்டமைப்பு attr_check *check,
		       முழு collect_all,
		       மாறிலி எழுத்து *file)

{
	எழுத்து *full_path =
		prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, file);

	எனில் (collect_all) {
		git_all_attrs(the_repository->index, full_path, check);
	} இல்லையெனில் {
		git_check_attr(the_repository->index, full_path, check);
	}
	output_attr(check, file);

	விடுவி(full_path);
}

நிலையான வெறுமை check_attr_stdin_paths(மாறிலி எழுத்து *prefix, கட்டமைப்பு attr_check *check,
				   முழு collect_all)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு strbuf unquoted = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_வரி ? strbuf_getline_nul : strbuf_getline_lf;
	போது (getline_fn(&buf, stdin) != EOF) {
		எனில் (!nul_term_வரி && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			எனில் (unquote_c_style(&unquoted, buf.buf, வெற்று))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		check_attr(prefix, check, collect_all, buf.buf);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

நிலையான NORETURN வெறுமை error_with_usage(மாறிலி எழுத்து *செய்தி)
{
	error("%s", செய்தி);
	usage_with_options(check_attr_usage, check_attr_options);
}

முழு cmd_check_attr(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு attr_check *check;
	கட்டமைப்பு object_id initialized_oid;
	முழு cnt, i, doubledash, filei;

	எனில் (!is_bare_repository(the_repository))
		setup_work_மரம்(the_repository);

	repo_config(the_repository, git_default_config, வெற்று);

	argc = parse_options(argc, argv, prefix, check_attr_options,
			     check_attr_usage, PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	எனில் (repo_read_index(the_repository) < 0) {
		die("invalid cache");
	}

	எனில் (cached_attrs)
		git_attr_set_direction(GIT_ATTR_INDEX);

	doubledash = -1;
	க்கு (i = 0; doubledash < 0 && i < argc; i++) {
		எனில் (!strcmp(argv[i], "--"))
			doubledash = i;
	}

	/* Process --all and/or attribute arguments: */
	எனில் (all_attrs) {
		எனில் (doubledash >= 1)
			error_with_usage("Attributes and --all both specified");

		cnt = 0;
		filei = doubledash + 1;
	} இல்லையெனில் எனில் (doubledash == 0) {
		error_with_usage("No attribute specified");
	} இல்லையெனில் எனில் (doubledash < 0) {
		எனில் (!argc)
			error_with_usage("No attribute specified");

		எனில் (stdin_paths) {
			/* Treat all arguments as attribute names. */
			cnt = argc;
			filei = argc;
		} இல்லையெனில் {
			/* Treat exactly one argument as an attribute name. */
			cnt = 1;
			filei = 1;
		}
	} இல்லையெனில் {
		cnt = doubledash;
		filei = doubledash + 1;
	}

	/* Check file argument(s): */
	எனில் (stdin_paths) {
		எனில் (filei < argc)
			error_with_usage("Can't specify files with --stdin");
	} இல்லையெனில் {
		எனில் (filei >= argc)
			error_with_usage("No file specified");
	}

	check = attr_check_alloc();
	எனில் (!all_attrs) {
		க்கு (i = 0; i < cnt; i++) {
			மாறிலி கட்டமைப்பு git_attr *a = git_attr(argv[i]);

			எனில் (!a)
				திரும்பு error("%s: not a valid attribute name",
					     argv[i]);
			attr_check_append(check, a);
		}
	}

	எனில் (source) {
		எனில் (repo_get_oid_மரம்(the_repository, source, &initialized_oid))
			die("%s: not a valid tree-ish source", source);
		set_git_attr_source(source);
	}

	எனில் (stdin_paths)
		check_attr_stdin_paths(prefix, check, all_attrs);
	இல்லையெனில் {
		க்கு (i = filei; i < argc; i++)
			check_attr(prefix, check, all_attrs, argv[i]);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}

	attr_check_free(check);
	திரும்பு 0;
}
