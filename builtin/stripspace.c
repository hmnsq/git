#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "setup.h"
#ضمن "strbuf.h"
#ضمن "write-or-die.h"

ساكن فراغ comment_کرښې(بنية strbuf *buf)
{
	حرف *پیغام;
	اندازه_t len;

	پیغام = strbuf_detach(buf, &len);
	strbuf_جمع_کړه_commented_کرښې(buf, پیغام, len, comment_کرښه_str);
	حرر(پیغام);
}

ساكن ثابت حرف * ثابت stripspace_usage[] = {
	"git stripspace [-s | --strip-comments]",
	"git stripspace [-c | --comment-lines]",
	عدم
};

تعداد stripspace_ډېرپېښ {
	STRIP_DEFAULT = 0,
	STRIP_COMMENTS,
	COMMENT_کرښې
};

صحيح cmd_stripspace(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *مختاړی,
		   بنية repository *repo ناکارول_شوی)
{
	بنية strbuf buf = STRBUF_INIT;
	تعداد stripspace_ډېرپېښ ډېرپېښ = STRIP_DEFAULT;
	صحيح nongit;

	ثابت بنية option options[] = {
		OPT_CMDMODE('s', "strip-comments", &ډېرپېښ,
			    N_("skip and remove all lines starting with comment character"),
			    STRIP_COMMENTS),
		OPT_CMDMODE('c', "comment-lines", &ډېرپېښ,
			    N_("prepend comment character and space to each line"),
			    COMMENT_کرښې),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, stripspace_usage, 0);
	إذا (argc)
		usage_with_options(stripspace_usage, options);

	إذا (ډېرپېښ == STRIP_COMMENTS || ډېرپېښ == COMMENT_کرښې) {
		تیاری_git_directory_gently(the_repository, &nongit);
		repo_config(the_repository, git_default_config, عدم);
	}

	إذا (strbuf_read(&buf, 0, 1024) < 0)
		die_errno("could not read the input");

	إذا (ډېرپېښ == STRIP_DEFAULT || ډېرپېښ == STRIP_COMMENTS)
		strbuf_stripspace(&buf,
			  ډېرپېښ == STRIP_COMMENTS ? comment_کرښه_str : عدم);
	وإلا
		comment_کرښې(&buf);

	write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
	أرجع 0;
}
