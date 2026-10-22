#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "attr.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "write-or-die.h"

સ્થિર પૂર્ણાંક all_attrs;
સ્થિર પૂર્ણાંક cached_attrs;
સ્થિર પૂર્ણાંક stdin_paths;
સ્થિર અક્ષર *source;
સ્થિર અચલ અક્ષર * અચલ check_attr_usage[] = {
N_("git check-attr [--source <tree-ish>] [-a | --all | <attr>...] [--] <pathname>..."),
N_("git check-attr --stdin [-z] [--source <tree-ish>] [-a | --all | <attr>...]"),
ખાલી
};

સ્થિર પૂર્ણાંક nul_term_લીટી;

સ્થિર અચલ સંરચના option check_attr_options[] = {
	OPT_BOOL('a', "all", &all_attrs, N_("report all attributes set on file")),
	OPT_BOOL(0,  "cached", &cached_attrs, N_("use .gitattributes only from the index")),
	OPT_BOOL(0 , "stdin", &stdin_paths, N_("read file names from stdin")),
	OPT_BOOL('z', ખાલી, &nul_term_લીટી,
		 N_("terminate input and output records by a NUL character")),
	OPT_STRING(0, "source", &source, N_("<tree-ish>"), N_("which tree-ish to check attributes at")),
	OPT_END()
};

સ્થિર શૂન્ય output_attr(સંરચના attr_check *check, અચલ અક્ષર *file)
{
	પૂર્ણાંક j;
	પૂર્ણાંક cnt = check->nr;

	માટે (j = 0; j < cnt; j++) {
		અચલ અક્ષર *કિંમત = check->વસ્તુઓ[j].કિંમત;

		જો (ATTR_TRUE(કિંમત))
			કિંમત = "set";
		નહીંતર જો (ATTR_FALSE(કિંમત))
			કિંમત = "unset";
		નહીંતર જો (ATTR_UNSET(કિંમત))
			કિંમત = "unspecified";

		જો (nul_term_લીટી) {
			printf("%s%c" /* path */
			       "%s%c" /* attrname */
			       "%s%c" /* attrvalue */,
			       file, 0,
			       git_attr_name(check->વસ્તુઓ[j].attr), 0, કિંમત, 0);
		} નહીંતર {
			quote_c_style(file, ખાલી, stdout, 0);
			printf(": %s: %s\n",
			       git_attr_name(check->વસ્તુઓ[j].attr), કિંમત);
		}
	}
}

સ્થિર શૂન્ય check_attr(અચલ અક્ષર *prefix, સંરચના attr_check *check,
		       પૂર્ણાંક collect_all,
		       અચલ અક્ષર *file)

{
	અક્ષર *full_path =
		prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, file);

	જો (collect_all) {
		git_all_attrs(the_repository->index, full_path, check);
	} નહીંતર {
		git_check_attr(the_repository->index, full_path, check);
	}
	output_attr(check, file);

	મુક્ત(full_path);
}

સ્થિર શૂન્ય check_attr_stdin_paths(અચલ અક્ષર *prefix, સંરચના attr_check *check,
				   પૂર્ણાંક collect_all)
{
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના strbuf unquoted = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_લીટી ? strbuf_getline_nul : strbuf_getline_lf;
	જ્યારે (getline_fn(&buf, stdin) != EOF) {
		જો (!nul_term_લીટી && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			જો (unquote_c_style(&unquoted, buf.buf, ખાલી))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		check_attr(prefix, check, collect_all, buf.buf);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

સ્થિર NORETURN શૂન્ય error_with_usage(અચલ અક્ષર *સંદેશ)
{
	error("%s", સંદેશ);
	usage_with_options(check_attr_usage, check_attr_options);
}

પૂર્ણાંક cmd_check_attr(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix,
		   સંરચના repository *repo UNUSED)
{
	સંરચના attr_check *check;
	સંરચના object_id initialized_oid;
	પૂર્ણાંક cnt, i, doubledash, filei;

	જો (!is_bare_repository(the_repository))
		setup_work_વૃક્ષ(the_repository);

	repo_config(the_repository, git_default_config, ખાલી);

	argc = parse_options(argc, argv, prefix, check_attr_options,
			     check_attr_usage, PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	જો (repo_read_index(the_repository) < 0) {
		die("invalid cache");
	}

	જો (cached_attrs)
		git_attr_set_direction(GIT_ATTR_INDEX);

	doubledash = -1;
	માટે (i = 0; doubledash < 0 && i < argc; i++) {
		જો (!strcmp(argv[i], "--"))
			doubledash = i;
	}

	/* Process --all and/or attribute arguments: */
	જો (all_attrs) {
		જો (doubledash >= 1)
			error_with_usage("Attributes and --all both specified");

		cnt = 0;
		filei = doubledash + 1;
	} નહીંતર જો (doubledash == 0) {
		error_with_usage("No attribute specified");
	} નહીંતર જો (doubledash < 0) {
		જો (!argc)
			error_with_usage("No attribute specified");

		જો (stdin_paths) {
			/* Treat all arguments as attribute names. */
			cnt = argc;
			filei = argc;
		} નહીંતર {
			/* Treat exactly one argument as an attribute name. */
			cnt = 1;
			filei = 1;
		}
	} નહીંતર {
		cnt = doubledash;
		filei = doubledash + 1;
	}

	/* Check file argument(s): */
	જો (stdin_paths) {
		જો (filei < argc)
			error_with_usage("Can't specify files with --stdin");
	} નહીંતર {
		જો (filei >= argc)
			error_with_usage("No file specified");
	}

	check = attr_check_alloc();
	જો (!all_attrs) {
		માટે (i = 0; i < cnt; i++) {
			અચલ સંરચના git_attr *a = git_attr(argv[i]);

			જો (!a)
				પાછા error("%s: not a valid attribute name",
					     argv[i]);
			attr_check_append(check, a);
		}
	}

	જો (source) {
		જો (repo_get_oid_વૃક્ષ(the_repository, source, &initialized_oid))
			die("%s: not a valid tree-ish source", source);
		set_git_attr_source(source);
	}

	જો (stdin_paths)
		check_attr_stdin_paths(prefix, check, all_attrs);
	નહીંતર {
		માટે (i = filei; i < argc; i++)
			check_attr(prefix, check, all_attrs, argv[i]);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}

	attr_check_free(check);
	પાછા 0;
}
