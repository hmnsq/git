#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "fmt-merge-msg.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ fmt_ವಿಲೀನ_ಸಂದೇಶ_usage[] = {
	N_("git fmt-merge-msg [-m <message>] [--log[=<n>] | --no-log] [--file <file>]"),
	ಖಾಲಿ
};

ಪೂರ್ಣಾಂಕ cmd_fmt_ವಿಲೀನ_ಸಂದೇಶ(ಪೂರ್ಣಾಂಕ argc,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		      ರಚನೆ repository *repo)
{
	ಅಕ್ಷರ *inpath = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2 = ಖಾಲಿ;
	ಅಕ್ಷರ *into_name = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ shortlog_len = -1;
	ಪೂರ್ಣಾಂಕ ವಿಲೀನ_log_config = -1;
	ರಚನೆ option options[] = {
		{
			.type = OPTION_INTEGER,
			.long_name = "log",
			.ಮೌಲ್ಯ = &shortlog_len,
			.precision = ಗಾತ್ರ(shortlog_len),
			.argh = N_("n"),
			.help = N_("populate log with at most <n> entries from shortlog"),
			.flags = PARSE_OPT_OPTARG,
			.defval = DEFAULT_ವಿಲೀನ_LOG_LEN,
		},
		{
			.type = OPTION_INTEGER,
			.long_name = "summary",
			.ಮೌಲ್ಯ = &shortlog_len,
			.precision = ಗಾತ್ರ(shortlog_len),
			.argh = N_("n"),
			.help = N_("alias for --log (deprecated)"),
			.flags = PARSE_OPT_OPTARG | PARSE_OPT_HIDDEN,
			.defval = DEFAULT_ವಿಲೀನ_LOG_LEN,
		},
		OPT_STRING('m', "message", &ಸಂದೇಶ_2, N_("text"),
			N_("use <text> as start of message")),
		OPT_STRING(0, "into-name", &into_name, N_("name"),
			   N_("use <name> instead of the real target branch")),
		OPT_FILENAME('F', "file", &inpath, N_("file to read from")),
		OPT_ಕೊನೆ()
	};

	FILE *ಒಳ = stdin;
	ರಚನೆ strbuf ಒಳಹರಿವು = STRBUF_INIT, output = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ fmt_ವಿಲೀನ_ಸಂದೇಶ_opts opts;

	repo_config(repo, fmt_ವಿಲೀನ_ಸಂದೇಶ_config, &ವಿಲೀನ_log_config);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, fmt_ವಿಲೀನ_ಸಂದೇಶ_usage,
			     0);
	ಆದರೆ (argc > 0)
		usage_with_options(fmt_ವಿಲೀನ_ಸಂದೇಶ_usage, options);
	ಆದರೆ (shortlog_len < 0)
		shortlog_len = (ವಿಲೀನ_log_config > 0) ? ವಿಲೀನ_log_config : 0;

	ಆದರೆ (inpath && strcmp(inpath, "-")) {
		ಒಳ = fopen(inpath, "r");
		ಆದರೆ (!ಒಳ)
			die_errno("cannot open '%s'", inpath);
	}

	ಆದರೆ (strbuf_read(&ಒಳಹರಿವು, fileno(ಒಳ), 0) < 0)
		die_errno("could not read input file");

	ಆದರೆ (ಸಂದೇಶ_2)
		strbuf_addstr(&output, ಸಂದೇಶ_2);

	memset(&opts, 0, ಗಾತ್ರ(opts));
	opts.ಕೂಡಿಸು_title = !ಸಂದೇಶ_2;
	opts.credit_people = 1;
	opts.shortlog_len = shortlog_len;
	opts.into_name = into_name;

	ret = fmt_ವಿಲೀನ_ಸಂದೇಶ(&ಒಳಹರಿವು, &output, &opts);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;
	write_ಒಳ_ತುಂಬಿದ(STDOUT_FILENO, output.buf, output.len);

	strbuf_release(&ಒಳಹರಿವು);
	strbuf_release(&output);
	ಬಿಡುಗಡೆ(inpath);
	ಹಿಂತಿರುಗಿ 0;
}
