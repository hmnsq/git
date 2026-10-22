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

صحيح for_each_ref_core(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر, بنية repository *repo, ثابت حرف *ثابت *usage)
{
	بنية ref_چھانٹی *چھانٹی;
	بنية string_list چھانٹی_options = STRING_LIST_INIT_DUP;
	صحيح icase = 0, include_جڑ_refs = 0, from_stdin = 0;
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_format format = REF_FORMAT_INIT;
	بنانشانی صحيح flags = FILTER_REFS_REGULAR;
	بنية strvec vec = STRVEC_INIT;

	بنية option opts[] = {
		OPT_ثنائی_ہندسہ('s', "shell", &format.quote_style,
			N_("quote placeholders suitably for shells"), QUOTE_SHELL),
		OPT_ثنائی_ہندسہ('p', "perl",  &format.quote_style,
			N_("quote placeholders suitably for perl"), QUOTE_PERL),
		OPT_ثنائی_ہندسہ(0 , "python", &format.quote_style,
			N_("quote placeholders suitably for python"), QUOTE_PYTHON),
		OPT_ثنائی_ہندسہ(0 , "tcl",  &format.quote_style,
			N_("quote placeholders suitably for Tcl"), QUOTE_TCL),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_خالی_تھاواں,
			N_("do not output a newline after empty formatted refs")),

		OPT_گروہ(""),
		OPT_INTEGER( 0 , "count", &format.array_opts.max_گنتی, N_("show only <n> matched refs")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_STRING(  0 , "start-after", &filter.شروع_after, N_("marker"), N_("start iteration after the provided marker")),
		OPT__COLOR(&format.use_color, N_("respect format colors")),
		OPT_REF_FILTER_EXCLUDE(&filter),
		OPT_REF_ترتیب_دیو(&چھانٹی_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at,
			     N_("object"), N_("print only refs which points at the given object"),
			     parse_opt_object_name),
		OPT_MERGED(&filter, N_("print only refs that are merged")),
		OPT_NO_MERGED(&filter, N_("print only refs that are not merged")),
		OPT_شامل_اے(&filter.with_commit, N_("print only refs which contain the commit")),
		OPT_NO_شامل_اے(&filter.no_commit, N_("print only refs which don't contain the commit")),
		OPT_BOOL(0, "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read reference patterns from stdin")),
		OPT_BOOL(0, "include-root-refs", &include_جڑ_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_سرا(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, عدم);

	/* Set default (refname) sorting */
	string_list_append(&چھانٹی_options, "refname");

	parse_options(argc, argv, اگیتر, opts, usage, 0);
	إذا (format.array_opts.max_گنتی < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_گنتی);
		usage_with_options(usage, opts);
	}
	إذا (HAS_MULTI_بٹ(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	إذا (verify_ref_format(&format))
		usage_with_options(usage, opts);

	إذا (filter.شروع_after && چھانٹی_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	چھانٹی = ref_چھانٹی_options(&چھانٹی_options);
	ref_چھانٹی_سیٹ_ترتیب_دیو_flags_all(چھانٹی, REF_چھانٹی_ICASE, icase);
	filter.ignore_case = icase;

	إذا (from_stdin) {
		بنية strbuf سطر = STRBUF_INIT;

		إذا (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		طالما (strbuf_getline(&سطر, stdin) != EOF)
			strvec_ڈالو(&vec, سطر.buf);

		strbuf_release(&سطر);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} وإلا {
		filter.name_patterns = argv;
	}

	إذا (filter.شروع_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	إذا (include_جڑ_refs)
		flags |= FILTER_REFS_جڑ_REFS | FILTER_REFS_DETACHED_سر;

	filter.میل_as_رستہ = 1;
	filter_and_format_refs(&filter, flags, چھانٹی, &format);

	ref_filter_clear(&filter);
	ref_چھانٹی_release(چھانٹی);
	strvec_clear(&vec);
	أرجع 0;
}

صحيح cmd_for_each_ref(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *اگیتر,
		     بنية repository *repo)
{
	ساكن حرف ثابت * ثابت for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		عدم
	};

	أرجع for_each_ref_core(argc, argv, اگیتر, repo, for_each_ref_usage);
}
