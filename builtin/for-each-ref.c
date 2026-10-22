#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "for-each-ref.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ref-filter.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"

ਪੂਰਨ for_each_ref_core(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਬਣਤਰ repository *repo, ਅਚਲ ਅੱਖਰ *ਅਚਲ *usage)
{
	ਬਣਤਰ ref_ਛਾਂਟੀ *ਛਾਂਟੀ;
	ਬਣਤਰ string_list ਛਾਂਟੀ_options = STRING_LIST_INIT_DUP;
	ਪੂਰਨ icase = 0, include_ਜੜ੍ਹ_refs = 0, from_stdin = 0;
	ਬਣਤਰ ref_filter filter = REF_FILTER_INIT;
	ਬਣਤਰ ref_format format = REF_FORMAT_INIT;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = FILTER_REFS_REGULAR;
	ਬਣਤਰ strvec vec = STRVEC_INIT;

	ਬਣਤਰ option opts[] = {
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('s', "shell", &format.quote_style,
			N_("quote placeholders suitably for shells"), QUOTE_SHELL),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('p', "perl",  &format.quote_style,
			N_("quote placeholders suitably for perl"), QUOTE_PERL),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0 , "python", &format.quote_style,
			N_("quote placeholders suitably for python"), QUOTE_PYTHON),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0 , "tcl",  &format.quote_style,
			N_("quote placeholders suitably for Tcl"), QUOTE_TCL),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_ਖਾਲੀ_ਥਾਵਾਂ,
			N_("do not output a newline after empty formatted refs")),

		OPT_ਸਮੂਹ(""),
		OPT_INTEGER( 0 , "count", &format.array_opts.max_ਗਿਣਤੀ, N_("show only <n> matched refs")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_STRING(  0 , "start-after", &filter.ਸ਼ੁਰੂ_after, N_("marker"), N_("start iteration after the provided marker")),
		OPT__COLOR(&format.use_color, N_("respect format colors")),
		OPT_REF_FILTER_EXCLUDE(&filter),
		OPT_REF_ਛਾਂਟੋ(&ਛਾਂਟੀ_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at,
			     N_("object"), N_("print only refs which points at the given object"),
			     parse_opt_object_name),
		OPT_MERGED(&filter, N_("print only refs that are merged")),
		OPT_NO_MERGED(&filter, N_("print only refs that are not merged")),
		OPT_ਸ਼ਾਮਲ_ਹੈ(&filter.with_commit, N_("print only refs which contain the commit")),
		OPT_NO_ਸ਼ਾਮਲ_ਹੈ(&filter.no_commit, N_("print only refs which don't contain the commit")),
		OPT_BOOL(0, "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read reference patterns from stdin")),
		OPT_BOOL(0, "include-root-refs", &include_ਜੜ੍ਹ_refs, N_("also include HEAD ref and pseudorefs")),
		OPT_ਸਿਰਾ(),
	};

	format.format = "%(objectname) %(objecttype)\t%(refname)";

	repo_config(repo, git_default_config, ਨਿਰਾ);

	/* Set default (refname) sorting */
	string_list_append(&ਛਾਂਟੀ_options, "refname");

	parse_options(argc, argv, ਅਗੇਤਰ, opts, usage, 0);
	ਜੇ (format.array_opts.max_ਗਿਣਤੀ < 0) {
		error("invalid --count argument: `%d'", format.array_opts.max_ਗਿਣਤੀ);
		usage_with_options(usage, opts);
	}
	ਜੇ (HAS_MULTI_ਬਿੱਟ(format.quote_style)) {
		error("more than one quoting style?");
		usage_with_options(usage, opts);
	}
	ਜੇ (verify_ref_format(&format))
		usage_with_options(usage, opts);

	ਜੇ (filter.ਸ਼ੁਰੂ_after && ਛਾਂਟੀ_options.nr > 1)
		die(_("cannot use --start-after with custom sort options"));

	ਛਾਂਟੀ = ref_ਛਾਂਟੀ_options(&ਛਾਂਟੀ_options);
	ref_ਛਾਂਟੀ_ਸੈੱਟ_ਛਾਂਟੋ_flags_all(ਛਾਂਟੀ, REF_ਛਾਂਟੀ_ICASE, icase);
	filter.ignore_case = icase;

	ਜੇ (from_stdin) {
		ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;

		ਜੇ (argv[0])
			die(_("unknown arguments supplied with --stdin"));

		ਜਦੋਂ (strbuf_getline(&ਸਤਰ, stdin) != EOF)
			strvec_ਧੱਕੋ(&vec, ਸਤਰ.buf);

		strbuf_release(&ਸਤਰ);

		/* vec.v is NULL-terminated, just like 'argv'. */
		filter.name_patterns = vec.v;
	} ਨਹੀਂਤਾਂ {
		filter.name_patterns = argv;
	}

	ਜੇ (filter.ਸ਼ੁਰੂ_after && filter.name_patterns && filter.name_patterns[0])
		die(_("cannot use --start-after with patterns"));

	ਜੇ (include_ਜੜ੍ਹ_refs)
		flags |= FILTER_REFS_ਜੜ੍ਹ_REFS | FILTER_REFS_DETACHED_ਸਿਰ;

	filter.ਮੇਲ_as_ਰਸਤਾ = 1;
	filter_and_format_refs(&filter, flags, ਛਾਂਟੀ, &format);

	ref_filter_clear(&filter);
	ref_ਛਾਂਟੀ_release(ਛਾਂਟੀ);
	strvec_clear(&vec);
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_for_each_ref(ਪੂਰਨ argc,
		     ਅਚਲ ਅੱਖਰ **argv,
		     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		     ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ for_each_ref_usage[] = {
		N_("git for-each-ref " COMMON_USAGE_FOR_EACH_REF),
		ਨਿਰਾ
	};

	ਵਾਪਸ for_each_ref_core(argc, argv, ਅਗੇਤਰ, repo, for_each_ref_usage);
}
