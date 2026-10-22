#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

ಸ್ಥಿರ ಶೂನ್ಯ comment_ಸಾಲುಗಳು(ರಚನೆ strbuf *buf)
{
	ಅಕ್ಷರ *ಸಂದೇಶ;
	ಗಾತ್ರ_t len;

	ಸಂದೇಶ = strbuf_detach(buf, &len);
	strbuf_ಕೂಡಿಸು_commented_ಸಾಲುಗಳು(buf, ಸಂದೇಶ, len, comment_ಸಾಲು_str);
	ಬಿಡುಗಡೆ(ಸಂದೇಶ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ stripspace_usage[] = {
	"git stripspace [-s | --strip-comments]",
	"git stripspace [-c | --comment-lines]",
	ಖಾಲಿ
};

ಎಣಿಕೆ stripspace_ಬಹುಲಕ {
	STRIP_DEFAULT = 0,
	STRIP_COMMENTS,
	COMMENT_ಸಾಲುಗಳು
};

ಪೂರ್ಣಾಂಕ cmd_stripspace(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಎಣಿಕೆ stripspace_ಬಹುಲಕ ಬಹುಲಕ = STRIP_DEFAULT;
	ಪೂರ್ಣಾಂಕ nongit;

	ಸ್ಥಿರಾಂಕ ರಚನೆ option options[] = {
		OPT_CMDMODE('s', "strip-comments", &ಬಹುಲಕ,
			    N_("skip and remove all lines starting with comment character"),
			    STRIP_COMMENTS),
		OPT_CMDMODE('c', "comment-lines", &ಬಹುಲಕ,
			    N_("prepend comment character and space to each line"),
			    COMMENT_ಸಾಲುಗಳು),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, stripspace_usage, 0);
	ಆದರೆ (argc)
		usage_with_options(stripspace_usage, options);

	ಆದರೆ (ಬಹುಲಕ == STRIP_COMMENTS || ಬಹುಲಕ == COMMENT_ಸಾಲುಗಳು) {
		ಸಿದ್ಧತೆ_git_directory_gently(the_repository, &nongit);
		repo_config(the_repository, git_default_config, ಖಾಲಿ);
	}

	ಆದರೆ (strbuf_read(&buf, 0, 1024) < 0)
		die_errno("could not read the input");

	ಆದರೆ (ಬಹುಲಕ == STRIP_DEFAULT || ಬಹುಲಕ == STRIP_COMMENTS)
		strbuf_stripspace(&buf,
			  ಬಹುಲಕ == STRIP_COMMENTS ? comment_ಸಾಲು_str : ಖಾಲಿ);
	ಇಲ್ಲದಿದ್ದರೆ
		comment_ಸಾಲುಗಳು(&buf);

	write_or_die(1, buf.buf, buf.len);
	strbuf_release(&buf);
	ಹಿಂತಿರುಗಿ 0;
}
