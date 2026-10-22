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

स्थिर अचर वर्ण * अचर mailinfo_usage[] = {
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
	अचर वर्ण *charset;
};

स्थिर पूर्णाङ्क parse_opt_explicit_encoding(अचर संरचना option *opt,
				       अचर वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना metainfo_charset *meta_charset = opt->मान;

	BUG_ON_OPT_NEG(unset);

	meta_charset->policy = CHARSET_EXPLICIT;
	meta_charset->charset = arg;

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क parse_opt_quoted_cr(अचर संरचना option *opt, अचर वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);

	यदि (mailinfo_parse_quoted_cr_action(arg, opt->मान) != 0)
		फर्काऊ error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	फर्काऊ 0;
}

पूर्णाङ्क cmd_mailinfo(पूर्णाङ्क argc,
		 अचर वर्ण **argv,
		 अचर वर्ण *prefix,
		 संरचना repository *repo UNUSED)
{
	संरचना metainfo_charset meta_charset;
	संरचना mailinfo mi;
	पूर्णाङ्क status;
	वर्ण *msgfile, *patchfile;

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

	यदि (argc != 2)
		usage_with_options(mailinfo_usage, options);

	विकल्प (meta_charset.policy) {
	पक्ष CHARSET_DEFAULT:
		mi.metainfo_charset = get_commit_output_encoding();
		रोक;
	पक्ष CHARSET_NO_REENCODE:
		mi.metainfo_charset = रिक्त;
		रोक;
	पक्ष CHARSET_EXPLICIT:
		रोक;
	स्वभाव:
		BUG("invalid meta_charset.policy");
	}

	mi.आगत = मानक_निवेश;
	mi.output = मानक_निकास;

	msgfile = prefix_filename(prefix, argv[0]);
	patchfile = prefix_filename(prefix, argv[1]);

	status = !!mailinfo(&mi, msgfile, patchfile);
	clear_mailinfo(&mi);

	मुक्त(msgfile);
	मुक्त(patchfile);
	फर्काऊ status;
}
