/*
 * Another stupid program, this one parsing the headers of an
 * email to figure out authorship and subject
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mailinfo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ mailinfo_usage[] = {
	/* TRANSLATORS: keep <> in "<" mail ">" info. */
	N_("git mailinfo [<options>] <msg> <patch> < mail >info"),
	ਨਿਰਾ,
};

ਬਣਤਰ metainfo_charset
{
	ਗਿਣਤੀ {
		CHARSET_DEFAULT,
		CHARSET_NO_REENCODE,
		CHARSET_EXPLICIT,
	} policy;
	ਅਚਲ ਅੱਖਰ *charset;
};

ਸਥਿਰ ਪੂਰਨ parse_opt_explicit_encoding(ਅਚਲ ਬਣਤਰ option *opt,
				       ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ metainfo_charset *meta_charset = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	meta_charset->policy = CHARSET_EXPLICIT;
	meta_charset->charset = arg;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_opt_quoted_cr(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);

	ਜੇ (mailinfo_parse_quoted_cr_action(arg, opt->ਮੁੱਲ) != 0)
		ਵਾਪਸ error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_mailinfo(ਪੂਰਨ argc,
		 ਅਚਲ ਅੱਖਰ **argv,
		 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ metainfo_charset meta_charset;
	ਬਣਤਰ mailinfo mi;
	ਪੂਰਨ ਸਥਿਤੀ;
	ਅੱਖਰ *msgfile, *patchfile;

	ਬਣਤਰ option options[] = {
		OPT_BOOL('k', ਨਿਰਾ, &mi.keep_subject, N_("keep subject")),
		OPT_BOOL('b', ਨਿਰਾ, &mi.keep_non_patch_brackets_ਵਿੱਚ_subject,
			 N_("keep non patch brackets in subject")),
		OPT_BOOL('m', "message-id", &mi.ਜੋੜੋ_ਸੁਨੇਹਾ_id,
			 N_("copy Message-ID to the end of commit message")),
		OPT_ਸੈੱਟ_INT_F('u', ਨਿਰਾ, &meta_charset.policy,
			      N_("re-code metadata to i18n.commitEncoding"),
			      CHARSET_DEFAULT, PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT_F('n', ਨਿਰਾ, &meta_charset.policy,
			      N_("disable charset re-coding of metadata"),
			      CHARSET_NO_REENCODE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "encoding", &meta_charset, N_("encoding"),
			       N_("re-code metadata to this encoding"),
			       PARSE_OPT_NONEG, parse_opt_explicit_encoding),
		OPT_BOOL(0, "scissors", &mi.use_scissors, N_("use scissors")),
		OPT_CALLBACK_F(0, "quoted-cr", &mi.quoted_cr, N_("<action>"),
			       N_("action when quoted CR is found"),
			       PARSE_OPT_NONEG, parse_opt_quoted_cr),
		OPT_HIDDEN_BOOL(0, "inbody-headers", &mi.use_inbody_ਸਿਰਲੇਖਾਂ,
			 N_("use headers in message's body")),
		OPT_ਸਿਰਾ()
	};

	ਤਿਆਰੀ_mailinfo(the_repository, &mi);
	meta_charset.policy = CHARSET_DEFAULT;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, mailinfo_usage, 0);

	ਜੇ (argc != 2)
		usage_with_options(mailinfo_usage, options);

	ਚੋਣ (meta_charset.policy) {
	ਹਾਲਤ CHARSET_DEFAULT:
		mi.metainfo_charset = get_commit_output_encoding();
		ਰੋਕੋ;
	ਹਾਲਤ CHARSET_NO_REENCODE:
		mi.metainfo_charset = ਨਿਰਾ;
		ਰੋਕੋ;
	ਹਾਲਤ CHARSET_EXPLICIT:
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("invalid meta_charset.policy");
	}

	mi.ਨਿਵੇਸ਼ = stdin;
	mi.output = stdout;

	msgfile = ਅਗੇਤਰ_filename(ਅਗੇਤਰ, argv[0]);
	patchfile = ਅਗੇਤਰ_filename(ਅਗੇਤਰ, argv[1]);

	ਸਥਿਤੀ = !!mailinfo(&mi, msgfile, patchfile);
	clear_mailinfo(&mi);

	ਮੁਕਤ(msgfile);
	ਮੁਕਤ(patchfile);
	ਵਾਪਸ ਸਥਿਤੀ;
}
