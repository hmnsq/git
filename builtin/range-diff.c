#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "range-diff.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "parse.h"
#অন্তর্ভাব "color.h"


স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_range_diff_usage[] = {
N_("git range-diff [<options>] <old-base>..<old-tip> <new-base>..<new-tip>"),
N_("git range-diff [<options>] <old-tip>...<new-tip>"),
N_("git range-diff [<options>] <base> <old-tip> <new-tip>"),
ফাঁকা
};

স্থির পূর্ণসংখ্যা parse_max_memory(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	আকার_t *max_memory = opt->মান;
	uintmax_t val;

	যদি (unset)
		ফেরত 0;

	যদি (!git_parse_unsigned(arg, &val, আকার_MAX))
		ফেরত error(_("invalid max-memory value: %s"), arg);

	*max_memory = (আকার_t)val;
	ফেরত 0;
}

পূর্ণসংখ্যা cmd_range_diff(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *prefix,
		   কাঠামো repository *repo UNUSED)
{
	কাঠামো diff_options diffopt = { ফাঁকা };
	কাঠামো strvec log_arg = STRVEC_INIT;
	কাঠামো strvec diff_merges_arg = STRVEC_INIT;
	কাঠামো range_diff_options range_diff_opts = {
		.creation_factor = RANGE_DIFF_CREATION_FACTOR_DEFAULT,
		.max_memory = RANGE_DIFF_MAX_MEMORY_DEFAULT,
		.diffopt = &diffopt,
		.log_arg = &log_arg
	};
	পূর্ণসংখ্যা simple_color = -1, বাম_only = 0, ডান_only = 0;
	কাঠামো option range_diff_options[] = {
		OPT_INTEGER(0, "creation-factor",
			    &range_diff_opts.creation_factor,
			    N_("percentage by which creation is weighted")),
		OPT_BOOL(0, "no-dual-color", &simple_color,
			    N_("use simple diff colors")),
		OPT_PASSTHRU_ARGV(0, "notes", &log_arg,
				  N_("notes"), N_("passed to 'git log'"),
				  PARSE_OPT_OPTARG),
		OPT_PASSTHRU_ARGV(0, "diff-merges", &diff_merges_arg,
				  N_("style"), N_("passed to 'git log'"), 0),
		OPT_CALLBACK(0, "max-memory", &range_diff_opts.max_memory,
			     N_("size"),
			     N_("maximum memory for cost matrix (default 4G)"),
			     parse_max_memory),
		OPT_PASSTHRU_ARGV(0, "remerge-diff", &diff_merges_arg, ফাঁকা,
				  N_("passed to 'git log'"), PARSE_OPT_NOARG),
		OPT_BOOL(0, "left-only", &বাম_only,
			 N_("only emit output related to the first range")),
		OPT_BOOL(0, "right-only", &ডান_only,
			 N_("only emit output related to the second range")),
		OPT_END()
	};
	কাঠামো option *options;
	পূর্ণসংখ্যা i, dash_dash = -1, res = 0;
	কাঠামো strbuf range1 = STRBUF_INIT, range2 = STRBUF_INIT;
	কাঠামো object_id oid;
	ধ্রুবক অক্ষর *three_dots = ফাঁকা;

	repo_config(the_repository, git_diff_ui_config, ফাঁকা);

	repo_diff_setup(the_repository, &diffopt);

	options = add_diff_options(range_diff_options, &diffopt);
	argc = parse_options(argc, argv, prefix, options,
			     builtin_range_diff_usage, PARSE_OPT_KEEP_DASHDASH);

	diff_setup_done(&diffopt);

	/* force color when --dual-color was used */
	যদি (!simple_color)
		diffopt.use_color = GIT_COLOR_ALWAYS;

	/* If `--diff-merges` was specified, imply `--merges` */
	যদি (diff_merges_arg.nr) {
		range_diff_opts.include_merges = 1;
		strvec_pushv(&log_arg, diff_merges_arg.v);
	}

	জন্য (i = 0; i < argc; i++)
		যদি (!strcmp(argv[i], "--")) {
			dash_dash = i;
			বিরতি;
		}

	যদি (dash_dash == 3 ||
	    (dash_dash < 0 && argc > 2 &&
	     !repo_get_oid_committish(the_repository, argv[0], &oid) &&
	     !repo_get_oid_committish(the_repository, argv[1], &oid) &&
	     !repo_get_oid_committish(the_repository, argv[2], &oid))) {
		যদি (dash_dash < 0)
			; /* already validated arguments */
		অন্যথা যদি (repo_get_oid_committish(the_repository, argv[0], &oid))
			usage_বার্তা_optf(_("not a revision: '%s'"),
				       builtin_range_diff_usage, options,
				       argv[0]);
		অন্যথা যদি (repo_get_oid_committish(the_repository, argv[1], &oid))
			usage_বার্তা_optf(_("not a revision: '%s'"),
				       builtin_range_diff_usage, options,
				       argv[1]);
		অন্যথা যদি (repo_get_oid_committish(the_repository, argv[2], &oid))
			usage_বার্তা_optf(_("not a revision: '%s'"),
				       builtin_range_diff_usage, options,
				       argv[2]);

		strbuf_addf(&range1, "%s..%s", argv[0], argv[1]);
		strbuf_addf(&range2, "%s..%s", argv[0], argv[2]);

		strvec_pushv(&log_arg, argv +
			     (dash_dash < 0 ? 3 : dash_dash));
	} অন্যথা যদি (dash_dash == 2 ||
		   (dash_dash < 0 && argc > 1 &&
		    is_range_diff_range(argv[0]) &&
		    is_range_diff_range(argv[1]))) {
		যদি (dash_dash < 0)
			; /* already validated arguments */
		অন্যথা যদি (!is_range_diff_range(argv[0]))
			usage_বার্তা_optf(_("not a commit range: '%s'"),
				       builtin_range_diff_usage, options,
				       argv[0]);
		অন্যথা যদি (!is_range_diff_range(argv[1]))
			usage_বার্তা_optf(_("not a commit range: '%s'"),
				       builtin_range_diff_usage, options,
				       argv[1]);

		strbuf_addstr(&range1, argv[0]);
		strbuf_addstr(&range2, argv[1]);

		strvec_pushv(&log_arg, argv +
			     (dash_dash < 0 ? 2 : dash_dash));
	} অন্যথা যদি (dash_dash == 1 ||
		   (dash_dash < 0 && argc > 0 &&
		    (three_dots = strstr(argv[0], "...")))) {
		ধ্রুবক অক্ষর *a, *b;
		পূর্ণসংখ্যা a_len;

		যদি (dash_dash < 0)
			; /* already validated arguments */
		অন্যথা যদি (!(three_dots = strstr(argv[0], "...")))
			usage_বার্তা_optf(_("not a symmetric range: '%s'"),
					 builtin_range_diff_usage, options,
					 argv[0]);

		যদি (three_dots == argv[0]) {
			a = "HEAD";
			a_len = strlen(a);
		} অন্যথা {
			a = argv[0];
			a_len = (পূর্ণসংখ্যা)(three_dots - a);
		}

		যদি (three_dots[3])
			b = three_dots + 3;
		অন্যথা
			b = "HEAD";

		strbuf_addf(&range1, "%s..%.*s", b, a_len, a);
		strbuf_addf(&range2, "%.*s..%s", a_len, a, b);

		strvec_pushv(&log_arg, argv +
			     (dash_dash < 0 ? 1 : dash_dash));
	} অন্যথা
		usage_বার্তা_opt(_("need two commit ranges"),
			      builtin_range_diff_usage, options);
	FREE_AND_NULL(options);

	range_diff_opts.dual_color = simple_color < 1;
	range_diff_opts.বাম_only = বাম_only;
	range_diff_opts.ডান_only = ডান_only;
	res = show_range_diff(range1.buf, range2.buf, &range_diff_opts);

	strvec_clear(&log_arg);
	strvec_clear(&diff_merges_arg);
	strbuf_release(&range1);
	strbuf_release(&range2);

	ফেরত res;
}
