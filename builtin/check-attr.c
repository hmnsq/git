#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "attr.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "object-name.h"
#شامل "quote.h"
#شامل "setup.h"
#شامل "parse-options.h"
#شامل "write-or-die.h"

ایستا صحیح all_attrs;
ایستا صحیح cached_attrs;
ایستا صحیح stdin_paths;
ایستا نویسه *source;
ایستا ثابت نویسه * ثابت check_attr_usage[] = {
N_("git check-attr [--source <tree-ish>] [-a | --all | <attr>...] [--] <pathname>..."),
N_("git check-attr --stdin [-z] [--source <tree-ish>] [-a | --all | <attr>...]"),
پوچ
};

ایستا صحیح nul_term_سطر;

ایستا ثابت ساختار option check_attr_options[] = {
	OPT_BOOL('a', "all", &all_attrs, N_("report all attributes set on file")),
	OPT_BOOL(0,  "cached", &cached_attrs, N_("use .gitattributes only from the index")),
	OPT_BOOL(0 , "stdin", &stdin_paths, N_("read file names from stdin")),
	OPT_BOOL('z', پوچ, &nul_term_سطر,
		 N_("terminate input and output records by a NUL character")),
	OPT_STRING(0, "source", &source, N_("<tree-ish>"), N_("which tree-ish to check attributes at")),
	OPT_END()
};

ایستا تهی output_attr(ساختار attr_check *check, ثابت نویسه *file)
{
	صحیح j;
	صحیح cnt = check->nr;

	برای (j = 0; j < cnt; j++) {
		ثابت نویسه *مقدار = check->اقلام[j].مقدار;

		اگر (ATTR_TRUE(مقدار))
			مقدار = "set";
		وگرنه اگر (ATTR_FALSE(مقدار))
			مقدار = "unset";
		وگرنه اگر (ATTR_UNSET(مقدار))
			مقدار = "unspecified";

		اگر (nul_term_سطر) {
			printf("%s%c" /* path */
			       "%s%c" /* attrname */
			       "%s%c" /* attrvalue */,
			       file, 0,
			       git_attr_name(check->اقلام[j].attr), 0, مقدار, 0);
		} وگرنه {
			quote_c_style(file, پوچ, stdout, 0);
			printf(": %s: %s\n",
			       git_attr_name(check->اقلام[j].attr), مقدار);
		}
	}
}

ایستا تهی check_attr(ثابت نویسه *prefix, ساختار attr_check *check,
		       صحیح collect_all,
		       ثابت نویسه *file)

{
	نویسه *full_path =
		prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, file);

	اگر (collect_all) {
		git_all_attrs(the_repository->index, full_path, check);
	} وگرنه {
		git_check_attr(the_repository->index, full_path, check);
	}
	output_attr(check, file);

	آزاد(full_path);
}

ایستا تهی check_attr_stdin_paths(ثابت نویسه *prefix, ساختار attr_check *check,
				   صحیح collect_all)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار strbuf unquoted = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	وقتی (getline_fn(&buf, stdin) != EOF) {
		اگر (!nul_term_سطر && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			اگر (unquote_c_style(&unquoted, buf.buf, پوچ))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		check_attr(prefix, check, collect_all, buf.buf);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

ایستا NORETURN تهی error_with_usage(ثابت نویسه *پیام)
{
	error("%s", پیام);
	usage_with_options(check_attr_usage, check_attr_options);
}

صحیح cmd_check_attr(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix,
		   ساختار repository *repo UNUSED)
{
	ساختار attr_check *check;
	ساختار object_id initialized_oid;
	صحیح cnt, i, doubledash, filei;

	اگر (!is_bare_repository(the_repository))
		setup_work_درخت(the_repository);

	repo_config(the_repository, git_default_config, پوچ);

	argc = parse_options(argc, argv, prefix, check_attr_options,
			     check_attr_usage, PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	اگر (repo_read_index(the_repository) < 0) {
		die("invalid cache");
	}

	اگر (cached_attrs)
		git_attr_set_direction(GIT_ATTR_INDEX);

	doubledash = -1;
	برای (i = 0; doubledash < 0 && i < argc; i++) {
		اگر (!strcmp(argv[i], "--"))
			doubledash = i;
	}

	/* Process --all and/or attribute arguments: */
	اگر (all_attrs) {
		اگر (doubledash >= 1)
			error_with_usage("Attributes and --all both specified");

		cnt = 0;
		filei = doubledash + 1;
	} وگرنه اگر (doubledash == 0) {
		error_with_usage("No attribute specified");
	} وگرنه اگر (doubledash < 0) {
		اگر (!argc)
			error_with_usage("No attribute specified");

		اگر (stdin_paths) {
			/* Treat all arguments as attribute names. */
			cnt = argc;
			filei = argc;
		} وگرنه {
			/* Treat exactly one argument as an attribute name. */
			cnt = 1;
			filei = 1;
		}
	} وگرنه {
		cnt = doubledash;
		filei = doubledash + 1;
	}

	/* Check file argument(s): */
	اگر (stdin_paths) {
		اگر (filei < argc)
			error_with_usage("Can't specify files with --stdin");
	} وگرنه {
		اگر (filei >= argc)
			error_with_usage("No file specified");
	}

	check = attr_check_alloc();
	اگر (!all_attrs) {
		برای (i = 0; i < cnt; i++) {
			ثابت ساختار git_attr *a = git_attr(argv[i]);

			اگر (!a)
				بازگشت error("%s: not a valid attribute name",
					     argv[i]);
			attr_check_append(check, a);
		}
	}

	اگر (source) {
		اگر (repo_get_oid_درخت(the_repository, source, &initialized_oid))
			die("%s: not a valid tree-ish source", source);
		set_git_attr_source(source);
	}

	اگر (stdin_paths)
		check_attr_stdin_paths(prefix, check, all_attrs);
	وگرنه {
		برای (i = filei; i < argc; i++)
			check_attr(prefix, check, all_attrs, argv[i]);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}

	attr_check_free(check);
	بازگشت 0;
}
