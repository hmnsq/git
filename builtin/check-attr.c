#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

ਸਥਿਰ ਪੂਰਨ all_attrs;
ਸਥਿਰ ਪੂਰਨ cached_attrs;
ਸਥਿਰ ਪੂਰਨ stdin_paths;
ਸਥਿਰ ਅੱਖਰ *ਸਰੋਤ;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ check_attr_usage[] = {
N_("git check-attr [--source <tree-ish>] [-a | --all | <attr>...] [--] <pathname>..."),
N_("git check-attr --stdin [-z] [--source <tree-ish>] [-a | --all | <attr>...]"),
ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ nul_ਪਦ_ਸਤਰ;

ਸਥਿਰ ਅਚਲ ਬਣਤਰ option check_attr_options[] = {
	OPT_BOOL('a', "all", &all_attrs, N_("report all attributes set on file")),
	OPT_BOOL(0,  "cached", &cached_attrs, N_("use .gitattributes only from the index")),
	OPT_BOOL(0 , "stdin", &stdin_paths, N_("read file names from stdin")),
	OPT_BOOL('z', ਨਿਰਾ, &nul_ਪਦ_ਸਤਰ,
		 N_("terminate input and output records by a NUL character")),
	OPT_STRING(0, "source", &ਸਰੋਤ, N_("<tree-ish>"), N_("which tree-ish to check attributes at")),
	OPT_ਸਿਰਾ()
};

ਸਥਿਰ ਖਾਲੀ output_attr(ਬਣਤਰ attr_check *check, ਅਚਲ ਅੱਖਰ *file)
{
	ਪੂਰਨ j;
	ਪੂਰਨ cnt = check->nr;

	ਲਈ (j = 0; j < cnt; j++) {
		ਅਚਲ ਅੱਖਰ *ਮੁੱਲ = check->ਚੀਜ਼ਾਂ[j].ਮੁੱਲ;

		ਜੇ (ATTR_TRUE(ਮੁੱਲ))
			ਮੁੱਲ = "set";
		ਨਹੀਂਤਾਂ ਜੇ (ATTR_FALSE(ਮੁੱਲ))
			ਮੁੱਲ = "unset";
		ਨਹੀਂਤਾਂ ਜੇ (ATTR_UNSET(ਮੁੱਲ))
			ਮੁੱਲ = "unspecified";

		ਜੇ (nul_ਪਦ_ਸਤਰ) {
			ਲਿਖੋ("%s%c" /*ਰਸਤਾ*/
			       "%s%c" /* attrname */
			       "%s%c" /* attrvalue */,
			       file, 0,
			       git_attr_name(check->ਚੀਜ਼ਾਂ[j].attr), 0, ਮੁੱਲ, 0);
		} ਨਹੀਂਤਾਂ {
			quote_c_style(file, ਨਿਰਾ, stdout, 0);
			ਲਿਖੋ(": %s: %s\n",
			       git_attr_name(check->ਚੀਜ਼ਾਂ[j].attr), ਮੁੱਲ);
		}
	}
}

ਸਥਿਰ ਖਾਲੀ check_attr(ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਬਣਤਰ attr_check *check,
		       ਪੂਰਨ collect_all,
		       ਅਚਲ ਅੱਖਰ *file)

{
	ਅੱਖਰ *ਭਰਿਆ_ਰਸਤਾ =
		ਅਗੇਤਰ_ਰਸਤਾ(the_repository, ਅਗੇਤਰ, ਅਗੇਤਰ ? ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ) : 0, file);

	ਜੇ (collect_all) {
		git_all_attrs(the_repository->ਸੂਚਕਾਂਕ, ਭਰਿਆ_ਰਸਤਾ, check);
	} ਨਹੀਂਤਾਂ {
		git_check_attr(the_repository->ਸੂਚਕਾਂਕ, ਭਰਿਆ_ਰਸਤਾ, check);
	}
	output_attr(check, file);

	ਮੁਕਤ(ਭਰਿਆ_ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ check_attr_stdin_paths(ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਬਣਤਰ attr_check *check,
				   ਪੂਰਨ collect_all)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ strbuf unquoted = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_ਪਦ_ਸਤਰ ? strbuf_getline_nul : strbuf_getline_lf;
	ਜਦੋਂ (getline_fn(&buf, stdin) != EOF) {
		ਜੇ (!nul_ਪਦ_ਸਤਰ && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ਜੇ (unquote_c_style(&unquoted, buf.buf, ਨਿਰਾ))
				die("line is badly quoted");
			strbuf_ਅਦਲਾ_ਬਦਲੀ(&buf, &unquoted);
		}
		check_attr(ਅਗੇਤਰ, check, collect_all, buf.buf);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

ਸਥਿਰ NORETURN ਖਾਲੀ error_with_usage(ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ)
{
	error("%s", ਸੁਨੇਹਾ);
	usage_with_options(check_attr_usage, check_attr_options);
}

ਪੂਰਨ cmd_check_attr(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ attr_check *check;
	ਬਣਤਰ object_id initialized_oid;
	ਪੂਰਨ cnt, i, doubledash, filei;

	ਜੇ (!is_bare_repository(the_repository))
		ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, check_attr_options,
			     check_attr_usage, PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0) {
		die("invalid cache");
	}

	ਜੇ (cached_attrs)
		git_attr_ਸੈੱਟ_direction(GIT_ATTR_ਸੂਚਕਾਂਕ);

	doubledash = -1;
	ਲਈ (i = 0; doubledash < 0 && i < argc; i++) {
		ਜੇ (!strcmp(argv[i], "--"))
			doubledash = i;
	}

	/* Process --all and/or attribute arguments: */
	ਜੇ (all_attrs) {
		ਜੇ (doubledash >= 1)
			error_with_usage("Attributes and --all both specified");

		cnt = 0;
		filei = doubledash + 1;
	} ਨਹੀਂਤਾਂ ਜੇ (doubledash == 0) {
		error_with_usage("No attribute specified");
	} ਨਹੀਂਤਾਂ ਜੇ (doubledash < 0) {
		ਜੇ (!argc)
			error_with_usage("No attribute specified");

		ਜੇ (stdin_paths) {
			/* Treat all arguments as attribute names. */
			cnt = argc;
			filei = argc;
		} ਨਹੀਂਤਾਂ {
			/* Treat exactly one argument as an attribute name. */
			cnt = 1;
			filei = 1;
		}
	} ਨਹੀਂਤਾਂ {
		cnt = doubledash;
		filei = doubledash + 1;
	}

	/* Check file argument(s): */
	ਜੇ (stdin_paths) {
		ਜੇ (filei < argc)
			error_with_usage("Can't specify files with --stdin");
	} ਨਹੀਂਤਾਂ {
		ਜੇ (filei >= argc)
			error_with_usage("No file specified");
	}

	check = attr_check_alloc();
	ਜੇ (!all_attrs) {
		ਲਈ (i = 0; i < cnt; i++) {
			ਅਚਲ ਬਣਤਰ git_attr *a = git_attr(argv[i]);

			ਜੇ (!a)
				ਵਾਪਸ error("%s: not a valid attribute name",
					     argv[i]);
			attr_check_append(check, a);
		}
	}

	ਜੇ (ਸਰੋਤ) {
		ਜੇ (repo_get_oid_ਰੁੱਖ(the_repository, ਸਰੋਤ, &initialized_oid))
			die("%s: not a valid tree-ish source", ਸਰੋਤ);
		ਸੈੱਟ_git_attr_ਸਰੋਤ(ਸਰੋਤ);
	}

	ਜੇ (stdin_paths)
		check_attr_stdin_paths(ਅਗੇਤਰ, check, all_attrs);
	ਨਹੀਂਤਾਂ {
		ਲਈ (i = filei; i < argc; i++)
			check_attr(ਅਗੇਤਰ, check, all_attrs, argv[i]);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}

	attr_check_free(check);
	ਵਾਪਸ 0;
}
