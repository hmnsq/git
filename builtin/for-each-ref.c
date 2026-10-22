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

صحيح for_each_ref_core(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند, بنية repository *repo, ثابت حرف *ثابت *usage)
{
	بنية ref_ترتیبدهی *ترتیبدهی;
	بنية string_list ترتیبدهی_options = STRING_LIST_INIT_DUP;
	صحيح icase = 0, include_ریشه_refs = 0, from_stdin = 0;
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_format format = REF_FORMAT_INIT;
	بیعلامت صحيح flags = FILTER_REFS_REGULAR;
	بنية strvec vec = STRVEC_INIT;

	بنية option opts[] = {
		OPT_بیت('s', "shell", &format.quote_style,
			N_("quote placeholders suitably for shells"), QUOTE_SHELL),
		OPT_بیت('p', "perl",  &format.quote_style,
			N_("quote placeholders suitably for perl"), QUOTE_PERL),
		OPT_بیت(0 , "python", &format.quote_style,
			N_("quote placeholders suitably for python"), QUOTE_PYTHON),
		OPT_بیت(0 , "tcl",  &format.quote_style,
			N_("quote placeholders suitably for Tcl"), QUOTE_TCL),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_خالی,
			N_("do not output a newline after empty formatted refs")),

		OPT_گروه(""),
		OPT_INTEGER( 0 , "count", &format.array_opts.max_شمار, N_("show only <n> matched refs")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_STRING(  0 , "start-after", &filter.شروع_after, N_("marker"), N_("start iteration after the provided marker")),
		OPT__COLOR(&format.use_color, N_("respect format colors")),
		OPT_REF_FILTER_EXCLUDE(&filter),
		OPT_REF_مرتب_سازی(&ترتیبدهی_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at,
			     N_("object"), N_("print only refs which points at the given object"),
			     parse_opt_object_name),
		OPT_MERGED(&filter, N_("print only refs that are merged")),
		OPT_NO_MERGED(&filter, N_("print only refs that are not merged")),
		OPT_شامل_است(&filter.with_commit, N_("print only refs which contain the commit")),
		OPT_NO_شامل_است(&filter.no_commit, N_("print only refs which don't contain the commit")),
		OPT_BOOL(0, "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read reference patterns from stdin")),
		OPT_BOOL(0, "include-root-refs", &include_ریشه_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_انتها(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, عدم);

	/* Set default (refname) sorting */
	string_list_append(&ترتیبدهی_options, "refname");

	parse_options(argc, argv, پیشوند, opts, usage, 0);
	إذا (format.array_opts.max_شمار < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_شمار);
		usage_with_options(usage, opts);
	}
	إذا (HAS_MULTI_بیتها(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	إذا (verify_ref_format(&format))
		usage_with_options(usage, opts);

	إذا (filter.شروع_after && ترتیبدهی_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	ترتیبدهی = ref_ترتیبدهی_options(&ترتیبدهی_options);
	ref_ترتیبدهی_مجموعه_مرتب_سازی_flags_all(ترتیبدهی, REF_ترتیبدهی_ICASE, icase);
	filter.ignore_case = icase;

	إذا (from_stdin) {
		بنية strbuf سطر = STRBUF_INIT;

		إذا (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		طالما (strbuf_getline(&سطر, stdin) != EOF)
			strvec_افزودن(&vec, سطر.buf);

		strbuf_release(&سطر);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} وإلا {
		filter.name_patterns = argv;
	}

	إذا (filter.شروع_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	إذا (include_ریشه_refs)
		flags |= FILTER_REFS_ریشه_REFS | FILTER_REFS_DETACHED_سر;

	filter.تطابق_as_مسیر = 1;
	filter_and_format_refs(&filter, flags, ترتیبدهی, &format);

	ref_filter_clear(&filter);
	ref_ترتیبدهی_release(ترتیبدهی);
	strvec_clear(&vec);
	أرجع 0;
}

صحيح cmd_for_each_ref(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *پیشوند,
		     بنية repository *repo)
{
	ساكن حرف ثابت * ثابت for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		عدم
	};

	أرجع for_each_ref_core(argc, argv, پیشوند, repo, for_each_ref_usage);
}
