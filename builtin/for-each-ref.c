#अन्तर्भाव "builtin.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "for-each-ref.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "ref-filter.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "strvec.h"

पूर्णाङ्क for_each_ref_core(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix, संरचना repository *repo, ध्रुव वर्ण *ध्रुव *usage)
{
	संरचना ref_sorting *sorting;
	संरचना string_list sorting_options = STRING_LIST_INIT_DUP;
	पूर्णाङ्क icase = 0, include_मूल_refs = 0, from_stdin = 0;
	संरचना ref_filter filter = REF_FILTER_INIT;
	संरचना ref_format format = REF_FORMAT_INIT;
	अचिन्हित पूर्णाङ्क flags = FILTER_REFS_REGULAR;
	संरचना strvec vec = STRVEC_INIT;

	संरचना option opts[] = {
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
		OPT_INTEGER( 0 , "count", &format.array_opts.max_गिनती, N_("show only <n> matched refs")),
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
		OPT_BOOL(0, "include-root-refs", &include_मूल_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_END(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, रिक्त);

	/* Set default (refname) sorting */
	string_list_append(&sorting_options, "refname");

	parse_options(argc, argv, prefix, opts, usage, 0);
	यदि (format.array_opts.max_गिनती < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_गिनती);
		usage_with_options(usage, opts);
	}
	यदि (HAS_MULTI_BITS(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	यदि (verify_ref_format(&format))
		usage_with_options(usage, opts);

	यदि (filter.start_after && sorting_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;

	यदि (from_stdin) {
		संरचना strbuf पंक्ति = STRBUF_INIT;

		यदि (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		यावत् (strbuf_getline(&पंक्ति, मानक_निवेश) != खातापूर्ण)
			strvec_धकेलो(&vec, पंक्ति.buf);

		strbuf_release(&पंक्ति);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} अन्यथा {
		filter.name_patterns = argv;
	}

	यदि (filter.start_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	यदि (include_मूल_refs)
		flags |= FILTER_REFS_मूल_REFS | FILTER_REFS_DETACHED_HEAD;

	filter.match_as_path = 1;
	filter_and_format_refs(&filter, flags, sorting, &format);

	ref_filter_clear(&filter);
	ref_sorting_release(sorting);
	strvec_clear(&vec);
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_for_each_ref(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *prefix,
		     संरचना repository *repo)
{
	स्थिर वर्ण ध्रुव * ध्रुव for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		रिक्त
	};

	प्रत्यागम for_each_ref_core(argc, argv, prefix, repo, for_each_ref_usage);
}
