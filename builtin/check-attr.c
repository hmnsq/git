#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "attr.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ all_attrs;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cached_attrs;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stdin_paths;
ಸ್ಥಿರ ಅಕ್ಷರ *ಮೂಲ;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ check_attr_usage[] = {
N_("git check-attr [--source <tree-ish>] [-a | --all | <attr>...] [--] <pathname>..."),
N_("git check-attr --stdin [-z] [--source <tree-ish>] [-a | --all | <attr>...]"),
ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nul_ಪದ_ಸಾಲು;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ option check_attr_options[] = {
	OPT_BOOL('a', "all", &all_attrs, N_("report all attributes set on file")),
	OPT_BOOL(0,  "cached", &cached_attrs, N_("use .gitattributes only from the index")),
	OPT_BOOL(0 , "stdin", &stdin_paths, N_("read file names from stdin")),
	OPT_BOOL('z', ಖಾಲಿ, &nul_ಪದ_ಸಾಲು,
		 N_("terminate input and output records by a NUL character")),
	OPT_STRING(0, "source", &ಮೂಲ, N_("<tree-ish>"), N_("which tree-ish to check attributes at")),
	OPT_ಕೊನೆ()
};

ಸ್ಥಿರ ಶೂನ್ಯ output_attr(ರಚನೆ attr_check *check, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file)
{
	ಪೂರ್ಣಾಂಕ j;
	ಪೂರ್ಣಾಂಕ cnt = check->nr;

	ಗಾಗಿ (j = 0; j < cnt; j++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ = check->ವಸ್ತುಗಳು[j].ಮೌಲ್ಯ;

		ಆದರೆ (ATTR_TRUE(ಮೌಲ್ಯ))
			ಮೌಲ್ಯ = "set";
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ATTR_FALSE(ಮೌಲ್ಯ))
			ಮೌಲ್ಯ = "unset";
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ATTR_UNSET(ಮೌಲ್ಯ))
			ಮೌಲ್ಯ = "unspecified";

		ಆದರೆ (nul_ಪದ_ಸಾಲು) {
			ಬರೆ("%s%c" /*ಮಾರ್ಗ*/
			       "%s%c" /* attrname */
			       "%s%c" /* attrvalue */,
			       file, 0,
			       git_attr_name(check->ವಸ್ತುಗಳು[j].attr), 0, ಮೌಲ್ಯ, 0);
		} ಇಲ್ಲದಿದ್ದರೆ {
			quote_c_style(file, ಖಾಲಿ, stdout, 0);
			ಬರೆ(": %s: %s\n",
			       git_attr_name(check->ವಸ್ತುಗಳು[j].attr), ಮೌಲ್ಯ);
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ check_attr(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ರಚನೆ attr_check *check,
		       ಪೂರ್ಣಾಂಕ collect_all,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file)

{
	ಅಕ್ಷರ *ತುಂಬಿದ_ಮಾರ್ಗ =
		ಪೂರ್ವಪ್ರತ್ಯಯ_ಮಾರ್ಗ(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ) : 0, file);

	ಆದರೆ (collect_all) {
		git_all_attrs(the_repository->ಸೂಚ್ಯಂಕ, ತುಂಬಿದ_ಮಾರ್ಗ, check);
	} ಇಲ್ಲದಿದ್ದರೆ {
		git_check_attr(the_repository->ಸೂಚ್ಯಂಕ, ತುಂಬಿದ_ಮಾರ್ಗ, check);
	}
	output_attr(check, file);

	ಬಿಡುಗಡೆ(ತುಂಬಿದ_ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ check_attr_stdin_paths(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ರಚನೆ attr_check *check,
				   ಪೂರ್ಣಾಂಕ collect_all)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ strbuf unquoted = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_ಪದ_ಸಾಲು ? strbuf_getline_nul : strbuf_getline_lf;
	ಆಗ (getline_fn(&buf, stdin) != EOF) {
		ಆದರೆ (!nul_ಪದ_ಸಾಲು && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ಆದರೆ (unquote_c_style(&unquoted, buf.buf, ಖಾಲಿ))
				die("line is badly quoted");
			strbuf_ಅದಲುಬದಲು(&buf, &unquoted);
		}
		check_attr(ಪೂರ್ವಪ್ರತ್ಯಯ, check, collect_all, buf.buf);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

ಸ್ಥಿರ NORETURN ಶೂನ್ಯ error_with_usage(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ)
{
	error("%s", ಸಂದೇಶ);
	usage_with_options(check_attr_usage, check_attr_options);
}

ಪೂರ್ಣಾಂಕ cmd_check_attr(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ attr_check *check;
	ರಚನೆ object_id initialized_oid;
	ಪೂರ್ಣಾಂಕ cnt, i, doubledash, filei;

	ಆದರೆ (!is_bare_repository(the_repository))
		ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, check_attr_options,
			     check_attr_usage, PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0) {
		die("invalid cache");
	}

	ಆದರೆ (cached_attrs)
		git_attr_ಗಣ_direction(GIT_ATTR_ಸೂಚ್ಯಂಕ);

	doubledash = -1;
	ಗಾಗಿ (i = 0; doubledash < 0 && i < argc; i++) {
		ಆದರೆ (!strcmp(argv[i], "--"))
			doubledash = i;
	}

	/* Process --all and/or attribute arguments: */
	ಆದರೆ (all_attrs) {
		ಆದರೆ (doubledash >= 1)
			error_with_usage("Attributes and --all both specified");

		cnt = 0;
		filei = doubledash + 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (doubledash == 0) {
		error_with_usage("No attribute specified");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (doubledash < 0) {
		ಆದರೆ (!argc)
			error_with_usage("No attribute specified");

		ಆದರೆ (stdin_paths) {
			/* Treat all arguments as attribute names. */
			cnt = argc;
			filei = argc;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* Treat exactly one argument as an attribute name. */
			cnt = 1;
			filei = 1;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		cnt = doubledash;
		filei = doubledash + 1;
	}

	/* Check file argument(s): */
	ಆದರೆ (stdin_paths) {
		ಆದರೆ (filei < argc)
			error_with_usage("Can't specify files with --stdin");
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (filei >= argc)
			error_with_usage("No file specified");
	}

	check = attr_check_alloc();
	ಆದರೆ (!all_attrs) {
		ಗಾಗಿ (i = 0; i < cnt; i++) {
			ಸ್ಥಿರಾಂಕ ರಚನೆ git_attr *a = git_attr(argv[i]);

			ಆದರೆ (!a)
				ಹಿಂತಿರುಗಿ error("%s: not a valid attribute name",
					     argv[i]);
			attr_check_append(check, a);
		}
	}

	ಆದರೆ (ಮೂಲ) {
		ಆದರೆ (repo_get_oid_ವೃಕ್ಷ(the_repository, ಮೂಲ, &initialized_oid))
			die("%s: not a valid tree-ish source", ಮೂಲ);
		ಗಣ_git_attr_ಮೂಲ(ಮೂಲ);
	}

	ಆದರೆ (stdin_paths)
		check_attr_stdin_paths(ಪೂರ್ವಪ್ರತ್ಯಯ, check, all_attrs);
	ಇಲ್ಲದಿದ್ದರೆ {
		ಗಾಗಿ (i = filei; i < argc; i++)
			check_attr(ಪೂರ್ವಪ್ರತ್ಯಯ, check, all_attrs, argv[i]);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}

	attr_check_free(check);
	ಹಿಂತಿರುಗಿ 0;
}
