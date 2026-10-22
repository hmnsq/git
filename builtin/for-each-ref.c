#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "for-each-ref.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "ref-filter.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା for_each_ref_core(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix, ସଂରଚନା repository *repo, ଅଚଳ ଅକ୍ଷର *ଅଚଳ *usage)
{
	ସଂରଚନା ref_sorting *sorting;
	ସଂରଚନା string_list sorting_options = STRING_LIST_INIT_DUP;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା icase = 0, include_ମୂଳ_refs = 0, from_stdin = 0;
	ସଂରଚନା ref_filter filter = REF_FILTER_INIT;
	ସଂରଚନା ref_format format = REF_FORMAT_INIT;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = FILTER_REFS_REGULAR;
	ସଂରଚନା strvec vec = STRVEC_INIT;

	ସଂରଚନା option opts[] = {
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
		OPT_INTEGER( 0 , "count", &format.array_opts.max_ଗଣନା, N_("show only <n> matched refs")),
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
		OPT_BOOL(0, "include-root-refs", &include_ମୂଳ_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_END(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, ଖାଲି);

	/* Set default (refname) sorting */
	string_list_append(&sorting_options, "refname");

	parse_options(argc, argv, prefix, opts, usage, 0);
	ଯଦି (format.array_opts.max_ଗଣନା < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_ଗଣନା);
		usage_with_options(usage, opts);
	}
	ଯଦି (HAS_MULTI_BITS(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	ଯଦି (verify_ref_format(&format))
		usage_with_options(usage, opts);

	ଯଦି (filter.start_after && sorting_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;

	ଯଦି (from_stdin) {
		ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;

		ଯଦି (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		ଯେତେବେଳେ (strbuf_getline(&ଧାଡ଼ି, stdin) != EOF)
			strvec_ଠେଲ(&vec, ଧାଡ଼ି.buf);

		strbuf_release(&ଧାଡ଼ି);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} ଅନ୍ୟଥା {
		filter.name_patterns = argv;
	}

	ଯଦି (filter.start_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	ଯଦି (include_ମୂଳ_refs)
		flags |= FILTER_REFS_ମୂଳ_REFS | FILTER_REFS_DETACHED_HEAD;

	filter.match_as_path = 1;
	filter_and_format_refs(&filter, flags, sorting, &format);

	ref_filter_clear(&filter);
	ref_sorting_release(sorting);
	strvec_clear(&vec);
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_for_each_ref(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		     ଅଚଳ ଅକ୍ଷର **argv,
		     ଅଚଳ ଅକ୍ଷର *prefix,
		     ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		ଖାଲି
	};

	ଫେରନ୍ତୁ for_each_ref_core(argc, argv, prefix, repo, for_each_ref_usage);
}
