#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "write-or-die.h"

સ્થિર શૂન્ય comment_લીટીઓ(સંરચના strbuf *buf)
{
	અક્ષર *સંદેશ;
	size_t len;

	સંદેશ = strbuf_detach(buf, &len);
	strbuf_add_commented_લીટીઓ(buf, સંદેશ, len, comment_લીટી_str);
	મુક્ત(સંદેશ);
}

સ્થિર અચલ અક્ષર * અચલ stripspace_usage[] = {
	"git stripspace [-s | --strip-comments]",
	"git stripspace [-c | --comment-lines]",
	ખાલી
};

ગણના stripspace_mode {
	STRIP_DEFAULT = 0,
	STRIP_COMMENTS,
	COMMENT_લીટીઓ
};

પૂર્ણાંક cmd_stripspace(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix,
		   સંરચના repository *repo UNUSED)
{
	સંરચના strbuf buf = STRBUF_INIT;
	ગણના stripspace_mode mode = STRIP_DEFAULT;
	પૂર્ણાંક nongit;

	અચલ સંરચના option options[] = {
		OPT_CMDMODE('s', "strip-comments", &mode,
			    N_("skip and remove all lines starting with comment character"),
			    STRIP_COMMENTS),
		OPT_CMDMODE('c', "comment-lines", &mode,
			    N_("prepend comment character and space to each line"),
			    COMMENT_લીટીઓ),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, stripspace_usage, 0);
	જો (argc)
		usage_with_options(stripspace_usage, options);

	જો (mode == STRIP_COMMENTS || mode == COMMENT_લીટીઓ) {
		setup_git_directory_gently(the_repository, &nongit);
		repo_config(the_repository, git_default_config, ખાલી);
	}

	જો (strbuf_read(&buf, 0, 1024) < 0)
		die_errno("could not read the input");

	જો (mode == STRIP_DEFAULT || mode == STRIP_COMMENTS)
		strbuf_stripspace(&buf,
			  mode == STRIP_COMMENTS ? comment_લીટી_str : ખાલી);
	નહીંતર
		comment_લીટીઓ(&buf);

	write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
	પાછા 0;
}
