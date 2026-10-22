/*
 * Another stupid program, this one parsing the headers of an
 * email to figure out authorship and subject
 */
#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "mailinfo.h"
#अन्तर्भाव "parse-options.h"

स्थिर स्थिरांक अक्षर * स्थिरांक mailinfo_usage[] = {
	/* TRANSLATORS: keep <> in "<" mail ">" info. */
	N_("git mailinfo [<options>] <msg> <patch> < mail >info"),
	रिक्त,
};

संरचना metainfo_charset
{
	गणना {
		CHARSET_DEFAULT,
		CHARSET_NO_REENCODE,
		CHARSET_EXPLICIT,
	} policy;
	स्थिरांक अक्षर *charset;
};

स्थिर पूर्णांक parse_opt_explicit_encoding(स्थिरांक संरचना option *opt,
				       स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना metainfo_charset *meta_charset = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	meta_charset->policy = CHARSET_EXPLICIT;
	meta_charset->charset = arg;

	परत 0;
}

स्थिर पूर्णांक parse_opt_quoted_cr(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);

	जर (mailinfo_parse_quoted_cr_action(arg, opt->मूल्य) != 0)
		परत error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	परत 0;
}

पूर्णांक cmd_mailinfo(पूर्णांक argc,
		 स्थिरांक अक्षर **argv,
		 स्थिरांक अक्षर *prefix,
		 संरचना repository *repo UNUSED)
{
	संरचना metainfo_charset meta_charset;
	संरचना mailinfo mi;
	पूर्णांक status;
	अक्षर *msgfile, *patchfile;

	संरचना option options[] = {
		OPT_BOOL('k', रिक्त, &mi.keep_subject, N_("keep subject")),
		OPT_BOOL('b', रिक्त, &mi.keep_non_patch_brackets_in_subject,
			 N_("keep non patch brackets in subject")),
		OPT_BOOL('m', "message-id", &mi.add_message_id,
			 N_("copy Message-ID to the end of commit message")),
		OPT_SET_INT_F('u', रिक्त, &meta_charset.policy,
			      N_("re-code metadata to i18n.commitEncoding"),
			      CHARSET_DEFAULT, PARSE_OPT_NONEG),
		OPT_SET_INT_F('n', रिक्त, &meta_charset.policy,
			      N_("disable charset re-coding of metadata"),
			      CHARSET_NO_REENCODE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "encoding", &meta_charset, N_("encoding"),
			       N_("re-code metadata to this encoding"),
			       PARSE_OPT_NONEG, parse_opt_explicit_encoding),
		OPT_BOOL(0, "scissors", &mi.use_scissors, N_("use scissors")),
		OPT_CALLBACK_F(0, "quoted-cr", &mi.quoted_cr, N_("<action>"),
			       N_("action when quoted CR is found"),
			       PARSE_OPT_NONEG, parse_opt_quoted_cr),
		OPT_HIDDEN_BOOL(0, "inbody-headers", &mi.use_inbody_headers,
			 N_("use headers in message's body")),
		OPT_END()
	};

	setup_mailinfo(the_repository, &mi);
	meta_charset.policy = CHARSET_DEFAULT;

	argc = parse_options(argc, argv, prefix, options, mailinfo_usage, 0);

	जर (argc != 2)
		usage_with_options(mailinfo_usage, options);

	निवड (meta_charset.policy) {
	स्थिती CHARSET_DEFAULT:
		mi.metainfo_charset = get_commit_output_encoding();
		थांबा;
	स्थिती CHARSET_NO_REENCODE:
		mi.metainfo_charset = रिक्त;
		थांबा;
	स्थिती CHARSET_EXPLICIT:
		थांबा;
	पूर्वनिर्धारित:
		BUG("invalid meta_charset.policy");
	}

	mi.आदान = मानक_निवेश;
	mi.output = मानक_निकास;

	msgfile = prefix_filename(prefix, argv[0]);
	patchfile = prefix_filename(prefix, argv[1]);

	status = !!mailinfo(&mi, msgfile, patchfile);
	clear_mailinfo(&mi);

	मुक्त(msgfile);
	मुक्त(patchfile);
	परत status;
}
