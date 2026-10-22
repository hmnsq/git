#شامل "builtin.h"
#شامل "commit.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "for-each-ref.h"
#شامل "gettext.h"
#شامل "object.h"
#شامل "parse-options.h"
#شامل "ref-filter.h"
#شامل "strbuf.h"
#شامل "strvec.h"

صحیح for_each_ref_core(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix, ساختار repository *repo, ثابت نویسه *ثابت *usage)
{
	ساختار ref_sorting *sorting;
	ساختار string_list sorting_options = STRING_LIST_INIT_DUP;
	صحیح icase = 0, include_ریشه_refs = 0, from_stdin = 0;
	ساختار ref_filter filter = REF_FILTER_INIT;
	ساختار ref_format format = REF_FORMAT_INIT;
	unsigned صحیح flags = FILTER_REFS_REGULAR;
	ساختار strvec vec = STRVEC_INIT;

	ساختار option opts[] = {
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
		OPT_INTEGER( 0 , "count", &format.array_opts.max_شمار, N_("show only <n> matched refs")),
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
		OPT_BOOL(0, "include-root-refs", &include_ریشه_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_END(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, پوچ);

	/* Set default (refname) sorting */
	string_list_append(&sorting_options, "refname");

	parse_options(argc, argv, prefix, opts, usage, 0);
	اگر (format.array_opts.max_شمار < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_شمار);
		usage_with_options(usage, opts);
	}
	اگر (HAS_MULTI_BITS(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	اگر (verify_ref_format(&format))
		usage_with_options(usage, opts);

	اگر (filter.start_after && sorting_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;

	اگر (from_stdin) {
		ساختار strbuf سطر = STRBUF_INIT;

		اگر (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		وقتی (strbuf_getline(&سطر, stdin) != EOF)
			strvec_افزودن(&vec, سطر.buf);

		strbuf_release(&سطر);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} وگرنه {
		filter.name_patterns = argv;
	}

	اگر (filter.start_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	اگر (include_ریشه_refs)
		flags |= FILTER_REFS_ریشه_REFS | FILTER_REFS_DETACHED_HEAD;

	filter.match_as_path = 1;
	filter_and_format_refs(&filter, flags, sorting, &format);

	ref_filter_clear(&filter);
	ref_sorting_release(sorting);
	strvec_clear(&vec);
	بازگشت 0;
}

صحیح cmd_for_each_ref(صحیح argc,
		     ثابت نویسه **argv,
		     ثابت نویسه *prefix,
		     ساختار repository *repo)
{
	ایستا نویسه ثابت * ثابت for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		پوچ
	};

	بازگشت for_each_ref_core(argc, argv, prefix, repo, for_each_ref_usage);
}
