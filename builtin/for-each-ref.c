#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "commit.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "for-each-ref.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "object.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "ref-filter.h"
#অন্তর্ভাৱ "strbuf.h"
#অন্তর্ভাৱ "strvec.h"

পূর্ণাঙ্ক for_each_ref_core(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix, সংরচনা repository *repo, ধ্রুৱ ৱর্ণ *ধ্রুৱ *usage)
{
	সংরচনা ref_sorting *sorting;
	সংরচনা string_list sorting_options = STRING_LIST_INIT_DUP;
	পূর্ণাঙ্ক icase = 0, include_মূল_refs = 0, from_stdin = 0;
	সংরচনা ref_filter filter = REF_FILTER_INIT;
	সংরচনা ref_format format = REF_FORMAT_INIT;
	unsigned পূর্ণাঙ্ক flags = FILTER_REFS_REGULAR;
	সংরচনা strvec vec = STRVEC_INIT;

	সংরচনা option opts[] = {
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
		OPT_INTEGER( 0 , "count", &format.array_opts.max_গুনতি, N_("show only <n> matched refs")),
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
		OPT_BOOL(0, "include-root-refs", &include_মূল_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_END(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, রিক্ত);

	/* Set default (refname) sorting */
	string_list_append(&sorting_options, "refname");

	parse_options(argc, argv, prefix, opts, usage, 0);
	যদি (format.array_opts.max_গুনতি < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_গুনতি);
		usage_with_options(usage, opts);
	}
	যদি (HAS_MULTI_BITS(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	যদি (verify_ref_format(&format))
		usage_with_options(usage, opts);

	যদি (filter.start_after && sorting_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;

	যদি (from_stdin) {
		সংরচনা strbuf সাৰি = STRBUF_INIT;

		যদি (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		যেতিয়ালৈ (strbuf_getline(&সাৰি, stdin) != EOF)
			strvec_ঠেলো(&vec, সাৰি.buf);

		strbuf_release(&সাৰি);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} অন্যথা {
		filter.name_patterns = argv;
	}

	যদি (filter.start_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	যদি (include_মূল_refs)
		flags |= FILTER_REFS_মূল_REFS | FILTER_REFS_DETACHED_HEAD;

	filter.match_as_path = 1;
	filter_and_format_refs(&filter, flags, sorting, &format);

	ref_filter_clear(&filter);
	ref_sorting_release(sorting);
	strvec_clear(&vec);
	ঘূৰাই 0;
}

পূর্ণাঙ্ক cmd_for_each_ref(পূর্ণাঙ্ক argc,
		     ধ্রুৱ ৱর্ণ **argv,
		     ধ্রুৱ ৱর্ণ *prefix,
		     সংরচনা repository *repo)
{
	স্থির ৱর্ণ ধ্রুৱ * ধ্রুৱ for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		রিক্ত
	};

	ঘূৰাই for_each_ref_core(argc, argv, prefix, repo, for_each_ref_usage);
}
