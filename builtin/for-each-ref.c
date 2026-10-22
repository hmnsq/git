#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "for-each-ref.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "ref-filter.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "strvec.h"

முழு for_each_ref_core(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix, கட்டமைப்பு repository *repo, மாறிலி எழுத்து *மாறிலி *usage)
{
	கட்டமைப்பு ref_sorting *sorting;
	கட்டமைப்பு string_list sorting_options = STRING_LIST_INIT_DUP;
	முழு icase = 0, include_வேர்_refs = 0, from_stdin = 0;
	கட்டமைப்பு ref_filter filter = REF_FILTER_INIT;
	கட்டமைப்பு ref_format format = REF_FORMAT_INIT;
	unsigned முழு flags = FILTER_REFS_REGULAR;
	கட்டமைப்பு strvec vec = STRVEC_INIT;

	கட்டமைப்பு option opts[] = {
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
		OPT_INTEGER( 0 , "count", &format.array_opts.max_மொத்தம், N_("show only <n> matched refs")),
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
		OPT_BOOL(0, "include-root-refs", &include_வேர்_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_END(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, வெற்று);

	/* Set default (refname) sorting */
	string_list_append(&sorting_options, "refname");

	parse_options(argc, argv, prefix, opts, usage, 0);
	எனில் (format.array_opts.max_மொத்தம் < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_மொத்தம்);
		usage_with_options(usage, opts);
	}
	எனில் (HAS_MULTI_BITS(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	எனில் (verify_ref_format(&format))
		usage_with_options(usage, opts);

	எனில் (filter.start_after && sorting_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;

	எனில் (from_stdin) {
		கட்டமைப்பு strbuf வரி = STRBUF_INIT;

		எனில் (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		போது (strbuf_getline(&வரி, stdin) != EOF)
			strvec_தள்ளு(&vec, வரி.buf);

		strbuf_release(&வரி);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} இல்லையெனில் {
		filter.name_patterns = argv;
	}

	எனில் (filter.start_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	எனில் (include_வேர்_refs)
		flags |= FILTER_REFS_வேர்_REFS | FILTER_REFS_DETACHED_HEAD;

	filter.match_as_path = 1;
	filter_and_format_refs(&filter, flags, sorting, &format);

	ref_filter_clear(&filter);
	ref_sorting_release(sorting);
	strvec_clear(&vec);
	திரும்பு 0;
}

முழு cmd_for_each_ref(முழு argc,
		     மாறிலி எழுத்து **argv,
		     மாறிலி எழுத்து *prefix,
		     கட்டமைப்பு repository *repo)
{
	நிலையான எழுத்து மாறிலி * மாறிலி for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		வெற்று
	};

	திரும்பு for_each_ref_core(argc, argv, prefix, repo, for_each_ref_usage);
}
