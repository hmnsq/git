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

ಪೂರ್ಣಾಙ್ಕ for_each_ref_core(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix, ಸಂರಚನಾ repository *repo, ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ *usage)
{
	ಸಂರಚನಾ ref_sorting *sorting;
	ಸಂರಚನಾ string_list sorting_options = STRING_LIST_INIT_DUP;
	ಪೂರ್ಣಾಙ್ಕ icase = 0, include_ಬೇರು_refs = 0, from_stdin = 0;
	ಸಂರಚನಾ ref_filter filter = REF_FILTER_INIT;
	ಸಂರಚನಾ ref_format format = REF_FORMAT_INIT;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags = FILTER_REFS_REGULAR;
	ಸಂರಚನಾ strvec vec = STRVEC_INIT;

	ಸಂರಚನಾ option opts[] = {
		OPT_BIT('s', "shell", &format.quote_style,
			N_("quote placeholders suitably for shells"), QUOTE_SHELL),
		OPT_BIT('p', "perl",  &format.quote_style,
			N_("quote placeholders suitably for perl"), QUOTE_PERL),
		OPT_BIT(0 , "python", &format.quote_style,
			N_("quote placeholders suitably for python"), QUOTE_PYTHON),
		OPT_BIT(0 , "tcl",  &format.quote_style,
			N_("quote placeholders suitably for Tcl"), QUOTE_TCL),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),

		OPT_GROUP(""),
		OPT_INTEGER( 0 , "count", &format.array_opts.max_ಎಣಿಕೆ, N_("show only <n> matched refs")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_STRING(  0 , "start-after", &filter.start_after, N_("marker"), N_("start iteration after the provided marker")),
		OPT__COLOR(&format.use_color, N_("respect format colors")),
		OPT_REF_FILTER_EXCLUDE(&filter),
		OPT_REF_SORT(&sorting_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at,
			     N_("object"), N_("print only refs which points at the given object"),
			     parse_opt_object_name),
		OPT_MERGED(&filter, N_("print only refs that are merged")),
		OPT_NO_MERGED(&filter, N_("print only refs that are not merged")),
		OPT_CONTAINS(&filter.with_commit, N_("print only refs which contain the commit")),
		OPT_NO_CONTAINS(&filter.no_commit, N_("print only refs which don't contain the commit")),
		OPT_BOOL(0, "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read reference patterns from stdin")),
		OPT_BOOL(0, "include-root-refs", &include_ಬೇರು_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_END(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, ರಿಕ್ತ);

	/* Set default (refname) sorting */
	string_list_append(&sorting_options, "refname");

	parse_options(argc, argv, prefix, opts, usage, 0);
	ಯದಿ (format.array_opts.max_ಎಣಿಕೆ < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_ಎಣಿಕೆ);
		usage_with_options(usage, opts);
	}
	ಯದಿ (HAS_MULTI_BITS(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	ಯದಿ (verify_ref_format(&format))
		usage_with_options(usage, opts);

	ಯದಿ (filter.start_after && sorting_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;

	ಯದಿ (from_stdin) {
		ಸಂರಚನಾ strbuf ಸಾಲು = STRBUF_INIT;

		ಯದಿ (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		ಯಾವತ್ (strbuf_getline(&ಸಾಲು, stdin) != EOF)
			strvec_ತಳ್ಳು(&vec, ಸಾಲು.buf);

		strbuf_release(&ಸಾಲು);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} ಅನ್ಯಥಾ {
		filter.name_patterns = argv;
	}

	ಯದಿ (filter.start_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	ಯದಿ (include_ಬೇರು_refs)
		flags |= FILTER_REFS_ಬೇರು_REFS | FILTER_REFS_DETACHED_HEAD;

	filter.match_as_path = 1;
	filter_and_format_refs(&filter, flags, sorting, &format);

	ref_filter_clear(&filter);
	ref_sorting_release(sorting);
	strvec_clear(&vec);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_for_each_ref(ಪೂರ್ಣಾಙ್ಕ argc,
		     ಧ್ರುವ ವರ್ಣ **argv,
		     ಧ್ರುವ ವರ್ಣ *prefix,
		     ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		ರಿಕ್ತ
	};

	ಪ್ರತ್ಯಾಗಮ for_each_ref_core(argc, argv, prefix, repo, for_each_ref_usage);
}
