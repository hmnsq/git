#شامل "builtin.h"
#شامل "config.h"
#شامل "fmt-merge-msg.h"
#شامل "gettext.h"
#شامل "parse-options.h"

ایستا ثابت نویسه * ثابت fmt_merge_پیام_usage[] = {
	N_("git fmt-merge-msg [-m <message>] [--log[=<n>] | --no-log] [--file <file>]"),
	پوچ
};

صحیح cmd_fmt_merge_پیام(صحیح argc,
		      ثابت نویسه **argv,
		      ثابت نویسه *prefix,
		      ساختار repository *repo)
{
	نویسه *inpath = پوچ;
	ثابت نویسه *message = پوچ;
	نویسه *into_name = پوچ;
	صحیح shortlog_len = -1;
	صحیح merge_log_config = -1;
	ساختار option options[] = {
		{
			.type = OPTION_INTEGER,
			.long_name = "log",
			.مقدار = &shortlog_len,
			.precision = اندازه(shortlog_len),
			.argh = N_("n"),
			.help = N_("populate log with at most <n> entries from shortlog"),
			.flags = PARSE_OPT_OPTARG,
			.defval = DEFAULT_MERGE_LOG_LEN,
		},
		{
			.type = OPTION_INTEGER,
			.long_name = "summary",
			.مقدار = &shortlog_len,
			.precision = اندازه(shortlog_len),
			.argh = N_("n"),
			.help = N_("alias for --log (deprecated)"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_HIDDEN,
			.defval = DEFAULT_MERGE_LOG_LEN,
		},
		OPT_STRING('m', "message", &message, N_("text"),
			N_("use <text> as start of message")),
		OPT_STRING(0, "into-name", &into_name, N_("name"),
			   N_("use <name> instead of the real target branch")),
		OPT_FILENAME('F', "file", &inpath, N_("file to read from")),
		OPT_END()
	};

	FILE *in = stdin;
	ساختار strbuf درون_داد = STRBUF_INIT, output = STRBUF_INIT;
	صحیح ret;
	ساختار fmt_merge_پیام_opts opts;

	repo_config(repo, fmt_merge_پیام_config, &merge_log_config);
	argc = parse_options(argc, argv, prefix, options, fmt_merge_پیام_usage,
			     0);
	اگر (argc > 0)
		usage_with_options(fmt_merge_پیام_usage, options);
	اگر (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	اگر (inpath && strcmp(inpath, "-")) {
		in = fopen(inpath, "r");
		اگر (!in)
			die_errno("cannot open '%s'", inpath);
	}

	اگر (strbuf_read(&درون_داد, fileno(in), 0) < 0)
		die_errno("could not read input file");

	اگر (message)
		strbuf_addstr(&output, message);

	memset(&opts, 0, اندازه(opts));
	opts.add_title = !message;
	opts.credit_people = 1;
	opts.shortlog_len = shortlog_len;
	opts.into_name = into_name;

	ret = fmt_merge_پیام(&درون_داد, &output, &opts);
	اگر (ret)
		بازگشت ret;
	write_in_full(STDOUT_FILENO, output.buf, output.len);

	strbuf_release(&درون_داد);
	strbuf_release(&output);
	آزاد(inpath);
	بازگشت 0;
}
