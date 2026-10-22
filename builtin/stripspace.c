#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "parse-options.h"
#include "setup.h"
#include "strbuf.h"
#include "write-or-die.h"

static void comment_lines(struct strbuf *buf)
{
	char *msg;
	size_t len;

	msg = strbuf_detach(buf, &len);
	strbuf_add_commented_lines(buf, msg, len, comment_line_str);
	free(msg);
}

static const char * const stripspace_usage[] = {
	"git stripspace [-s | --strip-comments]",
	"git stripspace [-c | --comment-lines]",
	NULL
};

enum stripspace_mode {
	STRIP_DEFAULT = 0,
	STRIP_COMMENTS,
	COMMENT_LINES
};

цяло cmd_stripspace(цяло argc,
		   const char **argv,
		   const char *prefix,
		   struct repository *repo UNUSED)
{
	struct strbuf buf = STRBUF_INIT;
	enum stripspace_mode mode = STRIP_DEFAULT;
	цяло nongit;

	const struct option options[] = {
		OPT_CMDMODE('s', "strip-comments", &mode,
			    N_("skip and remove all lines starting with comment character"),
			    STRIP_COMMENTS),
		OPT_CMDMODE('c', "comment-lines", &mode,
			    N_("prepend comment character and space to each line"),
			    COMMENT_LINES),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, stripspace_usage, 0);
	ако (argc)
		usage_with_options(stripspace_usage, options);

	ако (mode == STRIP_COMMENTS || mode == COMMENT_LINES) {
		setup_git_directory_gently(the_repository, &nongit);
		repo_config(the_repository, git_default_config, NULL);
	}

	ако (strbuf_read(&buf, 0, 1024) < 0)
		die_errno("could not read the input");

	ако (mode == STRIP_DEFAULT || mode == STRIP_COMMENTS)
		strbuf_stripspace(&buf,
			  mode == STRIP_COMMENTS ? comment_line_str : NULL);
	иначе
		comment_lines(&buf);

	write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
	върни 0;
}
