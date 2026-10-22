#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "for-each-ref.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "ref-filter.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"

ಪೂರ್ಣಾಂಕ for_each_ref_core(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ರಚನೆ repository *repo, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *usage)
{
	ರಚನೆ ref_ವಿಂಗಡಣೆ *ವಿಂಗಡಣೆ;
	ರಚನೆ string_list ವಿಂಗಡಣೆ_options = STRING_LIST_INIT_DUP;
	ಪೂರ್ಣಾಂಕ icase = 0, include_ಬೇರು_refs = 0, from_stdin = 0;
	ರಚನೆ ref_filter filter = REF_FILTER_INIT;
	ರಚನೆ ref_format format = REF_FORMAT_INIT;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = FILTER_REFS_REGULAR;
	ರಚನೆ strvec vec = STRVEC_INIT;

	ರಚನೆ option opts[] = {
		OPT_ಬಿಟ್('s', "shell", &format.quote_style,
			N_("quote placeholders suitably for shells"), QUOTE_SHELL),
		OPT_ಬಿಟ್('p', "perl",  &format.quote_style,
			N_("quote placeholders suitably for perl"), QUOTE_PERL),
		OPT_ಬಿಟ್(0 , "python", &format.quote_style,
			N_("quote placeholders suitably for python"), QUOTE_PYTHON),
		OPT_ಬಿಟ್(0 , "tcl",  &format.quote_style,
			N_("quote placeholders suitably for Tcl"), QUOTE_TCL),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_ಖಾಲಿಗಳು,
			N_("do not output a newline after empty formatted refs")),

		OPT_ಗುಂಪು(""),
		OPT_INTEGER( 0 , "count", &format.array_opts.max_ಎಣಿಕೆ, N_("show only <n> matched refs")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_STRING(  0 , "start-after", &filter.ಆರಂಭ_after, N_("marker"), N_("start iteration after the provided marker")),
		OPT__COLOR(&format.use_color, N_("respect format colors")),
		OPT_REF_FILTER_EXCLUDE(&filter),
		OPT_REF_ವಿಂಗಡಿಸು(&ವಿಂಗಡಣೆ_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at,
			     N_("object"), N_("print only refs which points at the given object"),
			     parse_opt_object_name),
		OPT_MERGED(&filter, N_("print only refs that are merged")),
		OPT_NO_MERGED(&filter, N_("print only refs that are not merged")),
		OPT_ಒಳಗೊಂಡಿದೆ(&filter.with_commit, N_("print only refs which contain the commit")),
		OPT_NO_ಒಳಗೊಂಡಿದೆ(&filter.no_commit, N_("print only refs which don't contain the commit")),
		OPT_BOOL(0, "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read reference patterns from stdin")),
		OPT_BOOL(0, "include-root-refs", &include_ಬೇರು_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_ಕೊನೆ(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, ಖಾಲಿ);

	/* Set default (refname) sorting */
	string_list_append(&ವಿಂಗಡಣೆ_options, "refname");

	parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, usage, 0);
	ಆದರೆ (format.array_opts.max_ಎಣಿಕೆ < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_ಎಣಿಕೆ);
		usage_with_options(usage, opts);
	}
	ಆದರೆ (HAS_MULTI_ಬಿಟ್ಟುಗಳು(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	ಆದರೆ (verify_ref_format(&format))
		usage_with_options(usage, opts);

	ಆದರೆ (filter.ಆರಂಭ_after && ವಿಂಗಡಣೆ_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	ವಿಂಗಡಣೆ = ref_ವಿಂಗಡಣೆ_options(&ವಿಂಗಡಣೆ_options);
	ref_ವಿಂಗಡಣೆ_ಗಣ_ವಿಂಗಡಿಸು_flags_all(ವಿಂಗಡಣೆ, REF_ವಿಂಗಡಣೆ_ICASE, icase);
	filter.ignore_case = icase;

	ಆದರೆ (from_stdin) {
		ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;

		ಆದರೆ (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		ಆಗ (strbuf_getline(&ಸಾಲು, stdin) != EOF)
			strvec_ತಳ್ಳು(&vec, ಸಾಲು.buf);

		strbuf_release(&ಸಾಲು);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} ಇಲ್ಲದಿದ್ದರೆ {
		filter.name_patterns = argv;
	}

	ಆದರೆ (filter.ಆರಂಭ_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	ಆದರೆ (include_ಬೇರು_refs)
		flags |= FILTER_REFS_ಬೇರು_REFS | FILTER_REFS_DETACHED_ತಲೆ;

	filter.ಹೊಂದಿಕೆ_as_ಮಾರ್ಗ = 1;
	filter_and_format_refs(&filter, flags, ವಿಂಗಡಣೆ, &format);

	ref_filter_clear(&filter);
	ref_ವಿಂಗಡಣೆ_release(ವಿಂಗಡಣೆ);
	strvec_clear(&vec);
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_for_each_ref(ಪೂರ್ಣಾಂಕ argc,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		     ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		ಖಾಲಿ
	};

	ಹಿಂತಿರುಗಿ for_each_ref_core(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo, for_each_ref_usage);
}
