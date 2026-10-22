#include "builtin.h"
#include "config.h"
#include "fmt-merge-msg.h"
#include "gettext.h"
#include "parse-options.h"

static const char * const fmt_merge_msg_usage[] = {
	N_("git fmt-merge-msg [-m <message>] [--log[=<n>] | --no-log] [--file <file>]"),
	NULL
};

số_nguyên cmd_fmt_merge_msg(số_nguyên argc,
		      const char **argv,
		      const char *prefix,
		      struct repository *repo)
{
	char *inpath = NULL;
	const char *message = NULL;
	char *into_name = NULL;
	số_nguyên shortlog_len = -1;
	số_nguyên merge_log_config = -1;
	struct option options[] = {
		{
			.type = OPTION_INTEGER,
			.long_name = "log",
			.value = &shortlog_len,
			.precision = sizeof(shortlog_len),
			.argh = N_("n"),
			.help = N_("populate log with at most <n> entries from shortlog"),
			.flags = PARSE_OPT_OPTARG,
			.defval = DEFAULT_MERGE_LOG_LEN,
		},
		{
			.type = OPTION_INTEGER,
			.long_name = "summary",
			.value = &shortlog_len,
			.precision = sizeof(shortlog_len),
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
	struct strbuf input = STRBUF_INIT, output = STRBUF_INIT;
	số_nguyên ret;
	struct fmt_merge_msg_opts opts;

	repo_config(repo, fmt_merge_msg_config, &merge_log_config);
	argc = parse_options(argc, argv, prefix, options, fmt_merge_msg_usage,
			     0);
	nếu (argc > 0)
		usage_with_options(fmt_merge_msg_usage, options);
	nếu (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	nếu (inpath && strcmp(inpath, "-")) {
		in = fopen(inpath, "r");
		nếu (!in)
			die_errno("cannot open '%s'", inpath);
	}

	nếu (strbuf_read(&input, fileno(in), 0) < 0)
		die_errno("could not read input file");

	nếu (message)
		strbuf_addstr(&output, message);

	memset(&opts, 0, sizeof(opts));
	opts.add_title = !message;
	opts.credit_people = 1;
	opts.shortlog_len = shortlog_len;
	opts.into_name = into_name;

	ret = fmt_merge_msg(&input, &output, &opts);
	nếu (ret)
		trả_về ret;
	write_in_full(STDOUT_FILENO, output.buf, output.len);

	strbuf_release(&input);
	strbuf_release(&output);
	free(inpath);
	trả_về 0;
}
