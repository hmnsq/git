#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fmt-merge-msg.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ fmt_ਮਿਲਾਓ_ਸੁਨੇਹਾ_usage[] = {
	N_("git fmt-merge-msg [-m <message>] [--log[=<n>] | --no-log] [--file <file>]"),
	ਨਿਰਾ
};

ਪੂਰਨ cmd_fmt_ਮਿਲਾਓ_ਸੁਨੇਹਾ(ਪੂਰਨ argc,
		      ਅਚਲ ਅੱਖਰ **argv,
		      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਬਣਤਰ repository *repo)
{
	ਅੱਖਰ *inpath = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2 = ਨਿਰਾ;
	ਅੱਖਰ *into_name = ਨਿਰਾ;
	ਪੂਰਨ shortlog_len = -1;
	ਪੂਰਨ ਮਿਲਾਓ_log_config = -1;
	ਬਣਤਰ option options[] = {
		{
			.type = OPTION_INTEGER,
			.long_name = "log",
			.ਮੁੱਲ = &shortlog_len,
			.precision = ਆਕਾਰ(shortlog_len),
			.argh = N_("n"),
			.help = N_("populate log with at most <n> entries from shortlog"),
			.flags = PARSE_OPT_OPTARG,
			.defval = DEFAULT_ਮਿਲਾਓ_LOG_LEN,
		},
		{
			.type = OPTION_INTEGER,
			.long_name = "summary",
			.ਮੁੱਲ = &shortlog_len,
			.precision = ਆਕਾਰ(shortlog_len),
			.argh = N_("n"),
			.help = N_("alias for --log (deprecated)"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_HIDDEN,
			.defval = DEFAULT_ਮਿਲਾਓ_LOG_LEN,
		},
		OPT_STRING('m', "message", &ਸੁਨੇਹਾ_2, N_("text"),
			N_("use <text> as start of message")),
		OPT_STRING(0, "into-name", &into_name, N_("name"),
			   N_("use <name> instead of the real target branch")),
		OPT_FILENAME('F', "file", &inpath, N_("file to read from")),
		OPT_ਸਿਰਾ()
	};

	FILE *ਵਿੱਚ = stdin;
	ਬਣਤਰ strbuf ਨਿਵੇਸ਼ = STRBUF_INIT, output = STRBUF_INIT;
	ਪੂਰਨ ret;
	ਬਣਤਰ fmt_ਮਿਲਾਓ_ਸੁਨੇਹਾ_opts opts;

	repo_config(repo, fmt_ਮਿਲਾਓ_ਸੁਨੇਹਾ_config, &ਮਿਲਾਓ_log_config);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, fmt_ਮਿਲਾਓ_ਸੁਨੇਹਾ_usage,
			     0);
	ਜੇ (argc > 0)
		usage_with_options(fmt_ਮਿਲਾਓ_ਸੁਨੇਹਾ_usage, options);
	ਜੇ (shortlog_len < 0)
		shortlog_len = (ਮਿਲਾਓ_log_config > 0) ? ਮਿਲਾਓ_log_config : 0;

	ਜੇ (inpath && strcmp(inpath, "-")) {
		ਵਿੱਚ = fopen(inpath, "r");
		ਜੇ (!ਵਿੱਚ)
			die_errno("cannot open '%s'", inpath);
	}

	ਜੇ (strbuf_read(&ਨਿਵੇਸ਼, fileno(ਵਿੱਚ), 0) < 0)
		die_errno("could not read input file");

	ਜੇ (ਸੁਨੇਹਾ_2)
		strbuf_addstr(&output, ਸੁਨੇਹਾ_2);

	memset(&opts, 0, ਆਕਾਰ(opts));
	opts.ਜੋੜੋ_title = !ਸੁਨੇਹਾ_2;
	opts.credit_people = 1;
	opts.shortlog_len = shortlog_len;
	opts.into_name = into_name;

	ret = fmt_ਮਿਲਾਓ_ਸੁਨੇਹਾ(&ਨਿਵੇਸ਼, &output, &opts);
	ਜੇ (ret)
		ਵਾਪਸ ret;
	write_ਵਿੱਚ_ਭਰਿਆ(STDOUT_FILENO, output.buf, output.len);

	strbuf_release(&ਨਿਵੇਸ਼);
	strbuf_release(&output);
	ਮੁਕਤ(inpath);
	ਵਾਪਸ 0;
}
