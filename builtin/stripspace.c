#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "write-or-die.h"

நிலையான வெறுமை comment_வரிகள்(கட்டமைப்பு strbuf *buf)
{
	எழுத்து *செய்தி;
	size_t len;

	செய்தி = strbuf_detach(buf, &len);
	strbuf_add_commented_வரிகள்(buf, செய்தி, len, comment_வரி_str);
	விடுவி(செய்தி);
}

நிலையான மாறிலி எழுத்து * மாறிலி stripspace_usage[] = {
	"git stripspace [-s | --strip-comments]",
	"git stripspace [-c | --comment-lines]",
	வெற்று
};

எண்ணிக்கை stripspace_mode {
	STRIP_DEFAULT = 0,
	STRIP_COMMENTS,
	COMMENT_வரிகள்
};

முழு cmd_stripspace(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	எண்ணிக்கை stripspace_mode mode = STRIP_DEFAULT;
	முழு nongit;

	மாறிலி கட்டமைப்பு option options[] = {
		OPT_CMDMODE('s', "strip-comments", &mode,
			    N_("skip and remove all lines starting with comment character"),
			    STRIP_COMMENTS),
		OPT_CMDMODE('c', "comment-lines", &mode,
			    N_("prepend comment character and space to each line"),
			    COMMENT_வரிகள்),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, stripspace_usage, 0);
	எனில் (argc)
		usage_with_options(stripspace_usage, options);

	எனில் (mode == STRIP_COMMENTS || mode == COMMENT_வரிகள்) {
		setup_git_directory_gently(the_repository, &nongit);
		repo_config(the_repository, git_default_config, வெற்று);
	}

	எனில் (strbuf_read(&buf, 0, 1024) < 0)
		die_errno("could not read the input");

	எனில் (mode == STRIP_DEFAULT || mode == STRIP_COMMENTS)
		strbuf_stripspace(&buf,
			  mode == STRIP_COMMENTS ? comment_வரி_str : வெற்று);
	இல்லையெனில்
		comment_வரிகள்(&buf);

	write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
	திரும்பு 0;
}
