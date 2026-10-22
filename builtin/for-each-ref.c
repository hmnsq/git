#ضمن "builtin.h"
#ضمن "commit.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "for-each-ref.h"
#ضمن "gettext.h"
#ضمن "object.h"
#ضمن "parse-options.h"
#ضمن "ref-filter.h"
#ضمن "strbuf.h"
#ضمن "strvec.h"

صحيح for_each_ref_core(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی, بنية repository *repo, ثابت حرف *ثابت *usage)
{
	بنية ref_ترتیبونه *ترتیبونه;
	بنية string_list ترتیبونه_options = STRING_LIST_INIT_DUP;
	صحيح icase = 0, include_ریښه_refs = 0, from_stdin = 0;
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_format format = REF_FORMAT_INIT;
	بینښه صحيح flags = FILTER_REFS_REGULAR;
	بنية strvec vec = STRVEC_INIT;

	بنية option opts[] = {
		OPT_بټ('s', "shell", &format.quote_style,
			N_("quote placeholders suitably for shells"), QUOTE_SHELL),
		OPT_بټ('p', "perl",  &format.quote_style,
			N_("quote placeholders suitably for perl"), QUOTE_PERL),
		OPT_بټ(0 , "python", &format.quote_style,
			N_("quote placeholders suitably for python"), QUOTE_PYTHON),
		OPT_بټ(0 , "tcl",  &format.quote_style,
			N_("quote placeholders suitably for Tcl"), QUOTE_TCL),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_تشه,
			N_("do not output a newline after empty formatted refs")),

		OPT_ډله(""),
		OPT_INTEGER( 0 , "count", &format.array_opts.max_شمېر, N_("show only <n> matched refs")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_STRING(  0 , "start-after", &filter.پیل_after, N_("marker"), N_("start iteration after the provided marker")),
		OPT__COLOR(&format.use_color, N_("respect format colors")),
		OPT_REF_FILTER_EXCLUDE(&filter),
		OPT_REF_ترتیبول(&ترتیبونه_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at,
			     N_("object"), N_("print only refs which points at the given object"),
			     parse_opt_object_name),
		OPT_MERGED(&filter, N_("print only refs that are merged")),
		OPT_NO_MERGED(&filter, N_("print only refs that are not merged")),
		OPT_لري(&filter.with_commit, N_("print only refs which contain the commit")),
		OPT_NO_لري(&filter.no_commit, N_("print only refs which don't contain the commit")),
		OPT_BOOL(0, "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read reference patterns from stdin")),
		OPT_BOOL(0, "include-root-refs", &include_ریښه_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_پای_ټکی(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, عدم);

	/* Set default (refname) sorting */
	string_list_append(&ترتیبونه_options, "refname");

	parse_options(argc, argv, مختاړی, opts, usage, 0);
	إذا (format.array_opts.max_شمېر < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_شمېر);
		usage_with_options(usage, opts);
	}
	إذا (HAS_MULTI_بټونه(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	إذا (verify_ref_format(&format))
		usage_with_options(usage, opts);

	إذا (filter.پیل_after && ترتیبونه_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	ترتیبونه = ref_ترتیبونه_options(&ترتیبونه_options);
	ref_ترتیبونه_سېټ_ترتیبول_flags_all(ترتیبونه, REF_ترتیبونه_ICASE, icase);
	filter.ignore_case = icase;

	إذا (from_stdin) {
		بنية strbuf کرښه = STRBUF_INIT;

		إذا (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		طالما (strbuf_getline(&کرښه, stdin) != EOF)
			strvec_ټېل(&vec, کرښه.buf);

		strbuf_release(&کرښه);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} وإلا {
		filter.name_patterns = argv;
	}

	إذا (filter.پیل_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	إذا (include_ریښه_refs)
		flags |= FILTER_REFS_ریښه_REFS | FILTER_REFS_DETACHED_سر;

	filter.سمون_as_لار = 1;
	filter_and_format_refs(&filter, flags, ترتیبونه, &format);

	ref_filter_clear(&filter);
	ref_ترتیبونه_release(ترتیبونه);
	strvec_clear(&vec);
	أرجع 0;
}

صحيح cmd_for_each_ref(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *مختاړی,
		     بنية repository *repo)
{
	ساكن حرف ثابت * ثابت for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		عدم
	};

	أرجع for_each_ref_core(argc, argv, مختاړی, repo, for_each_ref_usage);
}
