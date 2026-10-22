#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "fmt-merge-msg.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"

ساكن ثابت حرف * ثابت fmt_ادغام_پیام_usage[] = {
	N_("git fmt-merge-msg [-m <message>] [--log[=<n>] | --no-log] [--file <file>]"),
	عدم
};

صحيح cmd_fmt_ادغام_پیام(صحيح argc,
		      ثابت حرف **argv,
		      ثابت حرف *پیشوند,
		      بنية repository *repo)
{
	حرف *inpath = عدم;
	ثابت حرف *پیام_2 = عدم;
	حرف *into_name = عدم;
	صحيح shortlog_len = -1;
	صحيح ادغام_log_config = -1;
	بنية option options[] = {
		{
			.type = OPTION_INTEGER,
			.long_name = "log",
			.مقدار = &shortlog_len,
			.precision = حجم(shortlog_len),
			.argh = N_("n"),
			.help = N_("populate log with at most <n> entries from shortlog"),
			.flags = PARSE_OPT_OPTARG,
			.defval = DEFAULT_ادغام_LOG_LEN,
		},
		{
			.type = OPTION_INTEGER,
			.long_name = "summary",
			.مقدار = &shortlog_len,
			.precision = حجم(shortlog_len),
			.argh = N_("n"),
			.help = N_("alias for --log (deprecated)"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_HIDDEN,
			.defval = DEFAULT_ادغام_LOG_LEN,
		},
		OPT_STRING('m', "message", &پیام_2, N_("text"),
			N_("use <text> as start of message")),
		OPT_STRING(0, "into-name", &into_name, N_("name"),
			   N_("use <name> instead of the real target branch")),
		OPT_FILENAME('F', "file", &inpath, N_("file to read from")),
		OPT_انتها()
	};

	FILE *درون = stdin;
	بنية strbuf درون_داد = STRBUF_INIT, output = STRBUF_INIT;
	صحيح ret;
	بنية fmt_ادغام_پیام_opts opts;

	repo_config(repo, fmt_ادغام_پیام_config, &ادغام_log_config);
	argc = parse_options(argc, argv, پیشوند, options, fmt_ادغام_پیام_usage,
			     0);
	إذا (argc > 0)
		usage_with_options(fmt_ادغام_پیام_usage, options);
	إذا (shortlog_len < 0)
		shortlog_len = (ادغام_log_config > 0) ? ادغام_log_config : 0;

	إذا (inpath && strcmp(inpath, "-")) {
		درون = fopen(inpath, "r");
		إذا (!درون)
			die_errno("cannot open '%s'", inpath);
	}

	إذا (strbuf_read(&درون_داد, fileno(درون), 0) < 0)
		die_errno("could not read input file");

	إذا (پیام_2)
		strbuf_addstr(&output, پیام_2);

	memset(&opts, 0, حجم(opts));
	opts.جمع_کن_title = !پیام_2;
	opts.credit_people = 1;
	opts.shortlog_len = shortlog_len;
	opts.into_name = into_name;

	ret = fmt_ادغام_پیام(&درون_داد, &output, &opts);
	إذا (ret)
		أرجع ret;
	write_درون_پر(STDOUT_FILENO, output.buf, output.len);

	strbuf_release(&درون_داد);
	strbuf_release(&output);
	حرر(inpath);
	أرجع 0;
}
