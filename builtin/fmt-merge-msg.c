#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "fmt-merge-msg.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "parse-options.h"

நிலையான மாறிலி எழுத்து * மாறிலி fmt_merge_செய்தி_usage[] = {
	N_("git fmt-merge-msg [-m <message>] [--log[=<n>] | --no-log] [--file <file>]"),
	வெற்று
};

முழு cmd_fmt_merge_செய்தி(முழு argc,
		      மாறிலி எழுத்து **argv,
		      மாறிலி எழுத்து *prefix,
		      கட்டமைப்பு repository *repo)
{
	எழுத்து *inpath = வெற்று;
	மாறிலி எழுத்து *message = வெற்று;
	எழுத்து *into_name = வெற்று;
	முழு shortlog_len = -1;
	முழு merge_log_config = -1;
	கட்டமைப்பு option options[] = {
		{
			.type = OPTION_INTEGER,
			.long_name = "log",
			.மதிப்பு = &shortlog_len,
			.precision = அளவு(shortlog_len),
			.argh = N_("n"),
			.help = N_("populate log with at most <n> entries from shortlog"),
			.flags = PARSE_OPT_OPTARG,
			.defval = DEFAULT_MERGE_LOG_LEN,
		},
		{
			.type = OPTION_INTEGER,
			.long_name = "summary",
			.மதிப்பு = &shortlog_len,
			.precision = அளவு(shortlog_len),
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
	கட்டமைப்பு strbuf உள்ளீடு = STRBUF_INIT, output = STRBUF_INIT;
	முழு ret;
	கட்டமைப்பு fmt_merge_செய்தி_opts opts;

	repo_config(repo, fmt_merge_செய்தி_config, &merge_log_config);
	argc = parse_options(argc, argv, prefix, options, fmt_merge_செய்தி_usage,
			     0);
	எனில் (argc > 0)
		usage_with_options(fmt_merge_செய்தி_usage, options);
	எனில் (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	எனில் (inpath && strcmp(inpath, "-")) {
		in = fopen(inpath, "r");
		எனில் (!in)
			die_errno("cannot open '%s'", inpath);
	}

	எனில் (strbuf_read(&உள்ளீடு, fileno(in), 0) < 0)
		die_errno("could not read input file");

	எனில் (message)
		strbuf_addstr(&output, message);

	memset(&opts, 0, அளவு(opts));
	opts.add_title = !message;
	opts.credit_people = 1;
	opts.shortlog_len = shortlog_len;
	opts.into_name = into_name;

	ret = fmt_merge_செய்தி(&உள்ளீடு, &output, &opts);
	எனில் (ret)
		திரும்பு ret;
	write_in_full(STDOUT_FILENO, output.buf, output.len);

	strbuf_release(&உள்ளீடு);
	strbuf_release(&output);
	விடுவி(inpath);
	திரும்பு 0;
}
