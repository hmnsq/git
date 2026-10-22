/*
 * Another stupid program, this one parsing the headers of an
 * email to figure out authorship and subject
 */
#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "strbuf.h"
#समावेश "mailinfo.h"
#समावेश "parse-options.h"

अटल स्थिर अक्षर * स्थिर mailinfo_usage[] = {
	/* TRANSLATORS: keep <> in "<" mail ">" info. */
	N_("git mailinfo [<options>] <msg> <patch> < mail >info"),
	शून्य,
};

काष्ठा metainfo_charset
{
	क्रमागत {
		CHARSET_DEFAULT,
		CHARSET_NO_REENCODE,
		CHARSET_EXPLICIT,
	} policy;
	स्थिर अक्षर *charset;
};

अटल पूर्णांक parse_opt_explicit_encoding(स्थिर काष्ठा option *opt,
				       स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा metainfo_charset *meta_charset = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	meta_charset->policy = CHARSET_EXPLICIT;
	meta_charset->charset = arg;

	वापस 0;
}

अटल पूर्णांक parse_opt_quoted_cr(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);

	अगर (mailinfo_parse_quoted_cr_action(arg, opt->मूल्य) != 0)
		वापस error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	वापस 0;
}

पूर्णांक cmd_mailinfo(पूर्णांक argc,
		 स्थिर अक्षर **argv,
		 स्थिर अक्षर *prefix,
		 काष्ठा repository *repo UNUSED)
{
	काष्ठा metainfo_charset meta_charset;
	काष्ठा mailinfo mi;
	पूर्णांक status;
	अक्षर *msgfile, *patchfile;

	काष्ठा option options[] = {
		OPT_BOOL('k', शून्य, &mi.keep_subject, N_("keep subject")),
		OPT_BOOL('b', शून्य, &mi.keep_non_patch_brackets_in_subject,
			 N_("keep non patch brackets in subject")),
		OPT_BOOL('m', "message-id", &mi.add_message_id,
			 N_("copy Message-ID to the end of commit message")),
		OPT_SET_INT_F('u', शून्य, &meta_charset.policy,
			      N_("re-code metadata to i18n.commitEncoding"),
			      CHARSET_DEFAULT, PARSE_OPT_NONEG),
		OPT_SET_INT_F('n', शून्य, &meta_charset.policy,
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

	अगर (argc != 2)
		usage_with_options(mailinfo_usage, options);

	चयन (meta_charset.policy) {
	हाल CHARSET_DEFAULT:
		mi.metainfo_charset = get_commit_output_encoding();
		अवरोध;
	हाल CHARSET_NO_REENCODE:
		mi.metainfo_charset = शून्य;
		अवरोध;
	हाल CHARSET_EXPLICIT:
		अवरोध;
	शेष:
		BUG("invalid meta_charset.policy");
	}

	mi.निवेश = मानक_निवेश;
	mi.output = मानक_निकास;

	msgfile = prefix_filename(prefix, argv[0]);
	patchfile = prefix_filename(prefix, argv[1]);

	status = !!mailinfo(&mi, msgfile, patchfile);
	clear_mailinfo(&mi);

	मुक्त(msgfile);
	मुक्त(patchfile);
	वापस status;
}
