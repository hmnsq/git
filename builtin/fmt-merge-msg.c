#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "fmt-merge-msg.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"

સ્થિર અચલ અક્ષર * અચલ fmt_merge_સંદેશ_usage[] = {
	N_("git fmt-merge-msg [-m <message>] [--log[=<n>] | --no-log] [--file <file>]"),
	ખાલી
};

પૂર્ણાંક cmd_fmt_merge_સંદેશ(પૂર્ણાંક argc,
		      અચલ અક્ષર **argv,
		      અચલ અક્ષર *prefix,
		      સંરચના repository *repo)
{
	અક્ષર *inpath = ખાલી;
	અચલ અક્ષર *message = ખાલી;
	અક્ષર *into_name = ખાલી;
	પૂર્ણાંક shortlog_len = -1;
	પૂર્ણાંક merge_log_config = -1;
	સંરચના option options[] = {
		{
			.type = OPTION_INTEGER,
			.long_name = "log",
			.કિંમત = &shortlog_len,
			.precision = કદ(shortlog_len),
			.argh = N_("n"),
			.help = N_("populate log with at most <n> entries from shortlog"),
			.flags = PARSE_OPT_OPTARG,
			.defval = DEFAULT_MERGE_LOG_LEN,
		},
		{
			.type = OPTION_INTEGER,
			.long_name = "summary",
			.કિંમત = &shortlog_len,
			.precision = કદ(shortlog_len),
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
	સંરચના strbuf આગત = STRBUF_INIT, output = STRBUF_INIT;
	પૂર્ણાંક ret;
	સંરચના fmt_merge_સંદેશ_opts opts;

	repo_config(repo, fmt_merge_સંદેશ_config, &merge_log_config);
	argc = parse_options(argc, argv, prefix, options, fmt_merge_સંદેશ_usage,
			     0);
	જો (argc > 0)
		usage_with_options(fmt_merge_સંદેશ_usage, options);
	જો (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	જો (inpath && strcmp(inpath, "-")) {
		in = fopen(inpath, "r");
		જો (!in)
			die_errno("cannot open '%s'", inpath);
	}

	જો (strbuf_read(&આગત, fileno(in), 0) < 0)
		die_errno("could not read input file");

	જો (message)
		strbuf_addstr(&output, message);

	memset(&opts, 0, કદ(opts));
	opts.add_title = !message;
	opts.credit_people = 1;
	opts.shortlog_len = shortlog_len;
	opts.into_name = into_name;

	ret = fmt_merge_સંદેશ(&આગત, &output, &opts);
	જો (ret)
		પાછા ret;
	write_in_full(STDOUT_FILENO, output.buf, output.len);

	strbuf_release(&આગત);
	strbuf_release(&output);
	મુક્ત(inpath);
	પાછા 0;
}
