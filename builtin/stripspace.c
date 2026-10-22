#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "setup.h"
#شامل "strbuf.h"
#شامل "write-or-die.h"

ایستا تهی comment_سطرها(ساختار strbuf *buf)
{
	نویسه *پیام;
	size_t len;

	پیام = strbuf_detach(buf, &len);
	strbuf_add_commented_سطرها(buf, پیام, len, comment_سطر_str);
	آزاد(پیام);
}

ایستا ثابت نویسه * ثابت stripspace_usage[] = {
	"git stripspace [-s | --strip-comments]",
	"git stripspace [-c | --comment-lines]",
	پوچ
};

شمارش stripspace_mode {
	STRIP_DEFAULT = 0,
	STRIP_COMMENTS,
	COMMENT_سطرها
};

صحیح cmd_stripspace(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix,
		   ساختار repository *repo UNUSED)
{
	ساختار strbuf buf = STRBUF_INIT;
	شمارش stripspace_mode mode = STRIP_DEFAULT;
	صحیح nongit;

	ثابت ساختار option options[] = {
		OPT_CMDMODE('s', "strip-comments", &mode,
			    N_("skip and remove all lines starting with comment character"),
			    STRIP_COMMENTS),
		OPT_CMDMODE('c', "comment-lines", &mode,
			    N_("prepend comment character and space to each line"),
			    COMMENT_سطرها),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, stripspace_usage, 0);
	اگر (argc)
		usage_with_options(stripspace_usage, options);

	اگر (mode == STRIP_COMMENTS || mode == COMMENT_سطرها) {
		setup_git_directory_gently(the_repository, &nongit);
		repo_config(the_repository, git_default_config, پوچ);
	}

	اگر (strbuf_read(&buf, 0, 1024) < 0)
		die_errno("could not read the input");

	اگر (mode == STRIP_DEFAULT || mode == STRIP_COMMENTS)
		strbuf_stripspace(&buf,
			  mode == STRIP_COMMENTS ? comment_سطر_str : پوچ);
	وگرنه
		comment_سطرها(&buf);

	write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
	بازگشت 0;
}
