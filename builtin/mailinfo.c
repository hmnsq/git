/*
 * Another stupid program, this one parsing the headers of an
 * email to figure out authorship and subject
 */
#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "mailinfo.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ mailinfo_usage[] = {
	/* TRANSLATORS: keep <> in "<" mail ">" info. */
	N_("git mailinfo [<options>] <msg> <patch> < mail >info"),
	ಖಾಲಿ,
};

ರಚನೆ metainfo_charset
{
	ಎಣಿಕೆ {
		CHARSET_DEFAULT,
		CHARSET_NO_REENCODE,
		CHARSET_EXPLICIT,
	} policy;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *charset;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_explicit_encoding(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ metainfo_charset *meta_charset = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	meta_charset->policy = CHARSET_EXPLICIT;
	meta_charset->charset = arg;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_quoted_cr(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (mailinfo_parse_quoted_cr_action(arg, opt->ಮೌಲ್ಯ) != 0)
		ಹಿಂತಿರುಗಿ error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_mailinfo(ಪೂರ್ಣಾಂಕ argc,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ metainfo_charset meta_charset;
	ರಚನೆ mailinfo mi;
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
	ಅಕ್ಷರ *msgfile, *patchfile;

	ರಚನೆ option options[] = {
		OPT_BOOL('k', ಖಾಲಿ, &mi.keep_subject, N_("keep subject")),
		OPT_BOOL('b', ಖಾಲಿ, &mi.keep_non_patch_brackets_ಒಳ_subject,
			 N_("keep non patch brackets in subject")),
		OPT_BOOL('m', "message-id", &mi.ಕೂಡಿಸು_ಸಂದೇಶ_id,
			 N_("copy Message-ID to the end of commit message")),
		OPT_ಗಣ_INT_F('u', ಖಾಲಿ, &meta_charset.policy,
			      N_("re-code metadata to i18n.commitEncoding"),
			      CHARSET_DEFAULT, PARSE_OPT_NONEG),
		OPT_ಗಣ_INT_F('n', ಖಾಲಿ, &meta_charset.policy,
			      N_("disable charset re-coding of metadata"),
			      CHARSET_NO_REENCODE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "encoding", &meta_charset, N_("encoding"),
			       N_("re-code metadata to this encoding"),
			       PARSE_OPT_NONEG, parse_opt_explicit_encoding),
		OPT_BOOL(0, "scissors", &mi.use_scissors, N_("use scissors")),
		OPT_CALLBACK_F(0, "quoted-cr", &mi.quoted_cr, N_("<action>"),
			       N_("action when quoted CR is found"),
			       PARSE_OPT_NONEG, parse_opt_quoted_cr),
		OPT_HIDDEN_BOOL(0, "inbody-headers", &mi.use_inbody_ಶೀರ್ಷಿಕೆಗಳು,
			 N_("use headers in message's body")),
		OPT_ಕೊನೆ()
	};

	ಸಿದ್ಧತೆ_mailinfo(the_repository, &mi);
	meta_charset.policy = CHARSET_DEFAULT;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, mailinfo_usage, 0);

	ಆದರೆ (argc != 2)
		usage_with_options(mailinfo_usage, options);

	ಆಯ್ಕೆ (meta_charset.policy) {
	ಸ್ಥಿತಿ CHARSET_DEFAULT:
		mi.metainfo_charset = get_commit_output_encoding();
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ CHARSET_NO_REENCODE:
		mi.metainfo_charset = ಖಾಲಿ;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ CHARSET_EXPLICIT:
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid meta_charset.policy");
	}

	mi.ಒಳಹರಿವು = stdin;
	mi.output = stdout;

	msgfile = ಪೂರ್ವಪ್ರತ್ಯಯ_filename(ಪೂರ್ವಪ್ರತ್ಯಯ, argv[0]);
	patchfile = ಪೂರ್ವಪ್ರತ್ಯಯ_filename(ಪೂರ್ವಪ್ರತ್ಯಯ, argv[1]);

	ಸ್ಥಿತಿ_2 = !!mailinfo(&mi, msgfile, patchfile);
	clear_mailinfo(&mi);

	ಬಿಡುಗಡೆ(msgfile);
	ಬಿಡುಗಡೆ(patchfile);
	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}
