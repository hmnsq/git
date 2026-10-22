#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "write-or-die.h"

స్థిర శూన్య comment_పంక్తులు(నిర్మాణం strbuf *buf)
{
	అక్షరం *సందేశం;
	కొలత_t len;

	సందేశం = strbuf_detach(buf, &len);
	strbuf_కూడు_commented_పంక్తులు(buf, సందేశం, len, comment_పంక్తి_str);
	విడుదల(సందేశం);
}

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి stripspace_usage[] = {
	"git stripspace [-s | --strip-comments]",
	"git stripspace [-c | --comment-lines]",
	ఖాళీ
};

గణన stripspace_బహుళకం {
	STRIP_DEFAULT = 0,
	STRIP_COMMENTS,
	COMMENT_పంక్తులు
};

పూర్ణాంకం cmd_stripspace(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	గణన stripspace_బహుళకం బహుళకం = STRIP_DEFAULT;
	పూర్ణాంకం nongit;

	స్థిరరాశి నిర్మాణం option options[] = {
		OPT_CMDMODE('s', "strip-comments", &బహుళకం,
			    N_("skip and remove all lines starting with comment character"),
			    STRIP_COMMENTS),
		OPT_CMDMODE('c', "comment-lines", &బహుళకం,
			    N_("prepend comment character and space to each line"),
			    COMMENT_పంక్తులు),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, stripspace_usage, 0);
	ఒకవేళ (argc)
		usage_with_options(stripspace_usage, options);

	ఒకవేళ (బహుళకం == STRIP_COMMENTS || బహుళకం == COMMENT_పంక్తులు) {
		అమరిక_git_directory_gently(the_repository, &nongit);
		repo_config(the_repository, git_default_config, ఖాళీ);
	}

	ఒకవేళ (strbuf_read(&buf, 0, 1024) < 0)
		die_errno("could not read the input");

	ఒకవేళ (బహుళకం == STRIP_DEFAULT || బహుళకం == STRIP_COMMENTS)
		strbuf_stripspace(&buf,
			  బహుళకం == STRIP_COMMENTS ? comment_పంక్తి_str : ఖాళీ);
	లేకపోతే
		comment_పంక్తులు(&buf);

	write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
	తిరిగి 0;
}
