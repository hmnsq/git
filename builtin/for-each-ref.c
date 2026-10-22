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

صحيح for_each_ref_core(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي, بنية repository *repo, ثابت حرف *ثابت *usage)
{
	بنية ref_ترتيب_ڏيڻ *ترتيب_ڏيڻ;
	بنية string_list ترتيب_ڏيڻ_options = STRING_LIST_INIT_DUP;
	صحيح icase = 0, include_پاڙ_refs = 0, from_stdin = 0;
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_format format = REF_FORMAT_INIT;
	بنانشان صحيح flags = FILTER_REFS_REGULAR;
	بنية strvec vec = STRVEC_INIT;

	بنية option opts[] = {
		OPT_ٻٽو_انگ('s', "shell", &format.quote_style,
			N_("quote placeholders suitably for shells"), QUOTE_SHELL),
		OPT_ٻٽو_انگ('p', "perl",  &format.quote_style,
			N_("quote placeholders suitably for perl"), QUOTE_PERL),
		OPT_ٻٽو_انگ(0 , "python", &format.quote_style,
			N_("quote placeholders suitably for python"), QUOTE_PYTHON),
		OPT_ٻٽو_انگ(0 , "tcl",  &format.quote_style,
			N_("quote placeholders suitably for Tcl"), QUOTE_TCL),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_خالي_جايون,
			N_("do not output a newline after empty formatted refs")),

		OPT_گروهه(""),
		OPT_INTEGER( 0 , "count", &format.array_opts.max_ڳڻپ, N_("show only <n> matched refs")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_STRING(  0 , "start-after", &filter.شروعات_after, N_("marker"), N_("start iteration after the provided marker")),
		OPT__COLOR(&format.use_color, N_("respect format colors")),
		OPT_REF_FILTER_EXCLUDE(&filter),
		OPT_REF_ترتيب_ڏيو(&ترتيب_ڏيڻ_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at,
			     N_("object"), N_("print only refs which points at the given object"),
			     parse_opt_object_name),
		OPT_MERGED(&filter, N_("print only refs that are merged")),
		OPT_NO_MERGED(&filter, N_("print only refs that are not merged")),
		OPT_شامل_آهي(&filter.with_commit, N_("print only refs which contain the commit")),
		OPT_NO_شامل_آهي(&filter.no_commit, N_("print only refs which don't contain the commit")),
		OPT_BOOL(0, "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read reference patterns from stdin")),
		OPT_BOOL(0, "include-root-refs", &include_پاڙ_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_پڇاڙي(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, عدم);

	/* Set default (refname) sorting */
	string_list_append(&ترتيب_ڏيڻ_options, "refname");

	parse_options(argc, argv, اڳياڙي, opts, usage, 0);
	إذا (format.array_opts.max_ڳڻپ < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_ڳڻپ);
		usage_with_options(usage, opts);
	}
	إذا (HAS_MULTI_بٽ(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	إذا (verify_ref_format(&format))
		usage_with_options(usage, opts);

	إذا (filter.شروعات_after && ترتيب_ڏيڻ_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	ترتيب_ڏيڻ = ref_ترتيب_ڏيڻ_options(&ترتيب_ڏيڻ_options);
	ref_ترتيب_ڏيڻ_سيٽ_ترتيب_ڏيو_flags_all(ترتيب_ڏيڻ, REF_ترتيب_ڏيڻ_ICASE, icase);
	filter.ignore_case = icase;

	إذا (from_stdin) {
		بنية strbuf سٽ = STRBUF_INIT;

		إذا (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		طالما (strbuf_getline(&سٽ, stdin) != EOF)
			strvec_ڌڪو(&vec, سٽ.buf);

		strbuf_release(&سٽ);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} وإلا {
		filter.name_patterns = argv;
	}

	إذا (filter.شروعات_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	إذا (include_پاڙ_refs)
		flags |= FILTER_REFS_پاڙ_REFS | FILTER_REFS_DETACHED_سر;

	filter.ميلاپ_as_رستو = 1;
	filter_and_format_refs(&filter, flags, ترتيب_ڏيڻ, &format);

	ref_filter_clear(&filter);
	ref_ترتيب_ڏيڻ_release(ترتيب_ڏيڻ);
	strvec_clear(&vec);
	أرجع 0;
}

صحيح cmd_for_each_ref(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *اڳياڙي,
		     بنية repository *repo)
{
	ساكن حرف ثابت * ثابت for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		عدم
	};

	أرجع for_each_ref_core(argc, argv, اڳياڙي, repo, for_each_ref_usage);
}
