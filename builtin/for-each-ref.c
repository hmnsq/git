#అన్తర్భావ "builtin.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "for-each-ref.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "ref-filter.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "strvec.h"

పూర్ణాంకం for_each_ref_core(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ, నిర్మాణం repository *repo, స్థిరరాశి అక్షరం *స్థిరరాశి *usage)
{
	నిర్మాణం ref_క్రమీకరణ *క్రమీకరణ;
	నిర్మాణం string_list క్రమీకరణ_options = STRING_LIST_INIT_DUP;
	పూర్ణాంకం icase = 0, include_మూలం_refs = 0, from_stdin = 0;
	నిర్మాణం ref_filter filter = REF_FILTER_INIT;
	నిర్మాణం ref_format format = REF_FORMAT_INIT;
	గుర్తులేని పూర్ణాంకం flags = FILTER_REFS_REGULAR;
	నిర్మాణం strvec vec = STRVEC_INIT;

	నిర్మాణం option opts[] = {
		OPT_బిట్టు('s', "shell", &format.quote_style,
			N_("quote placeholders suitably for shells"), QUOTE_SHELL),
		OPT_బిట్టు('p', "perl",  &format.quote_style,
			N_("quote placeholders suitably for perl"), QUOTE_PERL),
		OPT_బిట్టు(0 , "python", &format.quote_style,
			N_("quote placeholders suitably for python"), QUOTE_PYTHON),
		OPT_బిట్టు(0 , "tcl",  &format.quote_style,
			N_("quote placeholders suitably for Tcl"), QUOTE_TCL),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_ఖాళీలు,
			N_("do not output a newline after empty formatted refs")),

		OPT_సమూహం(""),
		OPT_INTEGER( 0 , "count", &format.array_opts.max_లెక్క, N_("show only <n> matched refs")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_STRING(  0 , "start-after", &filter.ప్రారంభం_after, N_("marker"), N_("start iteration after the provided marker")),
		OPT__COLOR(&format.use_color, N_("respect format colors")),
		OPT_REF_FILTER_EXCLUDE(&filter),
		OPT_REF_క్రమీకరించు(&క్రమీకరణ_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at,
			     N_("object"), N_("print only refs which points at the given object"),
			     parse_opt_object_name),
		OPT_MERGED(&filter, N_("print only refs that are merged")),
		OPT_NO_MERGED(&filter, N_("print only refs that are not merged")),
		OPT_కలిగియుంది(&filter.with_commit, N_("print only refs which contain the commit")),
		OPT_NO_కలిగియుంది(&filter.no_commit, N_("print only refs which don't contain the commit")),
		OPT_BOOL(0, "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read reference patterns from stdin")),
		OPT_BOOL(0, "include-root-refs", &include_మూలం_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_చివర(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, ఖాళీ);

	/* Set default (refname) sorting */
	string_list_append(&క్రమీకరణ_options, "refname");

	parse_options(argc, argv, ఉపసర్గ, opts, usage, 0);
	ఒకవేళ (format.array_opts.max_లెక్క < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_లెక్క);
		usage_with_options(usage, opts);
	}
	ఒకవేళ (HAS_MULTI_బిట్లు(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	ఒకవేళ (verify_ref_format(&format))
		usage_with_options(usage, opts);

	ఒకవేళ (filter.ప్రారంభం_after && క్రమీకరణ_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	క్రమీకరణ = ref_క్రమీకరణ_options(&క్రమీకరణ_options);
	ref_క్రమీకరణ_సమితి_క్రమీకరించు_flags_all(క్రమీకరణ, REF_క్రమీకరణ_ICASE, icase);
	filter.ignore_case = icase;

	ఒకవేళ (from_stdin) {
		నిర్మాణం strbuf పంక్తి = STRBUF_INIT;

		ఒకవేళ (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		అయితే (strbuf_getline(&పంక్తి, stdin) != EOF)
			strvec_నెట్టు(&vec, పంక్తి.buf);

		strbuf_release(&పంక్తి);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} లేకపోతే {
		filter.name_patterns = argv;
	}

	ఒకవేళ (filter.ప్రారంభం_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	ఒకవేళ (include_మూలం_refs)
		flags |= FILTER_REFS_మూలం_REFS | FILTER_REFS_DETACHED_తల;

	filter.సరిపోలిక_as_మార్గం = 1;
	filter_and_format_refs(&filter, flags, క్రమీకరణ, &format);

	ref_filter_clear(&filter);
	ref_క్రమీకరణ_release(క్రమీకరణ);
	strvec_clear(&vec);
	తిరిగి 0;
}

పూర్ణాంకం cmd_for_each_ref(పూర్ణాంకం argc,
		     స్థిరరాశి అక్షరం **argv,
		     స్థిరరాశి అక్షరం *ఉపసర్గ,
		     నిర్మాణం repository *repo)
{
	స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		ఖాళీ
	};

	తిరిగి for_each_ref_core(argc, argv, ఉపసర్గ, repo, for_each_ref_usage);
}
