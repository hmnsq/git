#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "setup.h"
#ضمن "strbuf.h"
#ضمن "write-or-die.h"

ساكن فراغ comment_سطرها(بنية strbuf *buf)
{
	حرف *پیام;
	اندازه_t len;

	پیام = strbuf_detach(buf, &len);
	strbuf_جمع_کن_commented_سطرها(buf, پیام, len, comment_سطر_str);
	حرر(پیام);
}

ساكن ثابت حرف * ثابت stripspace_usage[] = {
	"git stripspace [-s | --strip-comments]",
	"git stripspace [-c | --comment-lines]",
	عدم
};

تعداد stripspace_نما {
	STRIP_DEFAULT = 0,
	STRIP_COMMENTS,
	COMMENT_سطرها
};

صحيح cmd_stripspace(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *پیشوند,
		   بنية repository *repo استفادهنشده)
{
	بنية strbuf buf = STRBUF_INIT;
	تعداد stripspace_نما نما = STRIP_DEFAULT;
	صحيح nongit;

	ثابت بنية option options[] = {
		OPT_CMDMODE('s', "strip-comments", &نما,
			    N_("skip and remove all lines starting with comment character"),
			    STRIP_COMMENTS),
		OPT_CMDMODE('c', "comment-lines", &نما,
			    N_("prepend comment character and space to each line"),
			    COMMENT_سطرها),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options, stripspace_usage, 0);
	إذا (argc)
		usage_with_options(stripspace_usage, options);

	إذا (نما == STRIP_COMMENTS || نما == COMMENT_سطرها) {
		راهاندازی_git_directory_gently(the_repository, &nongit);
		repo_config(the_repository, git_default_config, عدم);
	}

	إذا (strbuf_read(&buf, 0, 1024) < 0)
		die_errno("could not read the input");

	إذا (نما == STRIP_DEFAULT || نما == STRIP_COMMENTS)
		strbuf_stripspace(&buf,
			  نما == STRIP_COMMENTS ? comment_سطر_str : عدم);
	وإلا
		comment_سطرها(&buf);

	write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
	أرجع 0;
}
