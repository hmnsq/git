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

صحیح for_each_ref_core(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ, ساخت repository *repo, ثابت حرف *ثابت *usage)
{
	ساخت ref_ترتیب_کاری *ترتیب_کاری;
	ساخت string_list ترتیب_کاری_options = STRING_LIST_INIT_DUP;
	صحیح icase = 0, include_جڑ_refs = 0, from_stdin = 0;
	ساخت ref_filter filter = REF_FILTER_INIT;
	ساخت ref_format format = REF_FORMAT_INIT;
	غیرعلامتی صحیح flags = FILTER_REFS_REGULAR;
	ساخت strvec vec = STRVEC_INIT;

	ساخت option opts[] = {
		OPT_ثنائی_ہندسہ('s', "shell", &format.quote_style,
			N_("quote placeholders suitably for shells"), QUOTE_SHELL),
		OPT_ثنائی_ہندسہ('p', "perl",  &format.quote_style,
			N_("quote placeholders suitably for perl"), QUOTE_PERL),
		OPT_ثنائی_ہندسہ(0 , "python", &format.quote_style,
			N_("quote placeholders suitably for python"), QUOTE_PYTHON),
		OPT_ثنائی_ہندسہ(0 , "tcl",  &format.quote_style,
			N_("quote placeholders suitably for Tcl"), QUOTE_TCL),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_خالی_خانے,
			N_("do not output a newline after empty formatted refs")),

		OPT_گروہ(""),
		OPT_INTEGER( 0 , "count", &format.array_opts.max_گنتی, N_("show only <n> matched refs")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_STRING(  0 , "start-after", &filter.آغاز_after, N_("marker"), N_("start iteration after the provided marker")),
		OPT__COLOR(&format.use_color, N_("respect format colors")),
		OPT_REF_FILTER_EXCLUDE(&filter),
		OPT_REF_ترتیب_دو(&ترتیب_کاری_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at,
			     N_("object"), N_("print only refs which points at the given object"),
			     parse_opt_object_name),
		OPT_MERGED(&filter, N_("print only refs that are merged")),
		OPT_NO_MERGED(&filter, N_("print only refs that are not merged")),
		OPT_شامل_ہے(&filter.with_commit, N_("print only refs which contain the commit")),
		OPT_NO_شامل_ہے(&filter.no_commit, N_("print only refs which don't contain the commit")),
		OPT_BOOL(0, "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read reference patterns from stdin")),
		OPT_BOOL(0, "include-root-refs", &include_جڑ_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_انتہا(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, تہی);

	/* Set default (refname) sorting */
	string_list_append(&ترتیب_کاری_options, "refname");

	parse_options(argc, argv, سابقہ, opts, usage, 0);
	اگر (format.array_opts.max_گنتی < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_گنتی);
		usage_with_options(usage, opts);
	}
	اگر (HAS_MULTI_بٹ(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	اگر (verify_ref_format(&format))
		usage_with_options(usage, opts);

	اگر (filter.آغاز_after && ترتیب_کاری_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	ترتیب_کاری = ref_ترتیب_کاری_options(&ترتیب_کاری_options);
	ref_ترتیب_کاری_سیٹ_ترتیب_دو_flags_all(ترتیب_کاری, REF_ترتیب_کاری_ICASE, icase);
	filter.ignore_case = icase;

	اگر (from_stdin) {
		ساخت strbuf سطر = STRBUF_INIT;

		اگر (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		جب (strbuf_getline(&سطر, stdin) != EOF)
			strvec_ڈالو(&vec, سطر.buf);

		strbuf_release(&سطر);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} ورنہ {
		filter.name_patterns = argv;
	}

	اگر (filter.آغاز_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	اگر (include_جڑ_refs)
		flags |= FILTER_REFS_جڑ_REFS | FILTER_REFS_DETACHED_سر;

	filter.مطابقت_as_راستہ = 1;
	filter_and_format_refs(&filter, flags, ترتیب_کاری, &format);

	ref_filter_clear(&filter);
	ref_ترتیب_کاری_release(ترتیب_کاری);
	strvec_clear(&vec);
	واپس 0;
}

صحیح cmd_for_each_ref(صحیح argc,
		     ثابت حرف **argv,
		     ثابت حرف *سابقہ,
		     ساخت repository *repo)
{
	ساکن حرف ثابت * ثابت for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		تہی
	};

	واپس for_each_ref_core(argc, argv, سابقہ, repo, for_each_ref_usage);
}
