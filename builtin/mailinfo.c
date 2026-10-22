/*
 * Another stupid program, this one parsing the headers of an
 * email to figure out authorship and subject
 */
#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "mailinfo.h"
#અન્તર્ભાવ "parse-options.h"

સ્થિર અચલ અક્ષર * અચલ mailinfo_usage[] = {
	/* TRANSLATORS: keep <> in "<" mail ">" info. */
	N_("git mailinfo [<options>] <msg> <patch> < mail >info"),
	ખાલી,
};

સંરચના metainfo_charset
{
	ગણના {
		CHARSET_DEFAULT,
		CHARSET_NO_REENCODE,
		CHARSET_EXPLICIT,
	} policy;
	અચલ અક્ષર *charset;
};

સ્થિર પૂર્ણાંક parse_opt_explicit_encoding(અચલ સંરચના option *opt,
				       અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના metainfo_charset *meta_charset = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	meta_charset->policy = CHARSET_EXPLICIT;
	meta_charset->charset = arg;

	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_opt_quoted_cr(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);

	જો (mailinfo_parse_quoted_cr_action(arg, opt->કિંમત) != 0)
		પાછા error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	પાછા 0;
}

પૂર્ણાંક cmd_mailinfo(પૂર્ણાંક argc,
		 અચલ અક્ષર **argv,
		 અચલ અક્ષર *prefix,
		 સંરચના repository *repo UNUSED)
{
	સંરચના metainfo_charset meta_charset;
	સંરચના mailinfo mi;
	પૂર્ણાંક status;
	અક્ષર *msgfile, *patchfile;

	સંરચના option options[] = {
		OPT_BOOL('k', ખાલી, &mi.keep_subject, N_("keep subject")),
		OPT_BOOL('b', ખાલી, &mi.keep_non_patch_brackets_in_subject,
			 N_("keep non patch brackets in subject")),
		OPT_BOOL('m', "message-id", &mi.add_message_id,
			 N_("copy Message-ID to the end of commit message")),
		OPT_SET_INT_F('u', ખાલી, &meta_charset.policy,
			      N_("re-code metadata to i18n.commitEncoding"),
			      CHARSET_DEFAULT, PARSE_OPT_NONEG),
		OPT_SET_INT_F('n', ખાલી, &meta_charset.policy,
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

	જો (argc != 2)
		usage_with_options(mailinfo_usage, options);

	પસંદગી (meta_charset.policy) {
	સ્થિતિ CHARSET_DEFAULT:
		mi.metainfo_charset = get_commit_output_encoding();
		તોડો;
	સ્થિતિ CHARSET_NO_REENCODE:
		mi.metainfo_charset = ખાલી;
		તોડો;
	સ્થિતિ CHARSET_EXPLICIT:
		તોડો;
	મૂળભૂત:
		BUG("invalid meta_charset.policy");
	}

	mi.આગત = stdin;
	mi.output = stdout;

	msgfile = prefix_filename(prefix, argv[0]);
	patchfile = prefix_filename(prefix, argv[1]);

	status = !!mailinfo(&mi, msgfile, patchfile);
	clear_mailinfo(&mi);

	મુક્ત(msgfile);
	મુક્ત(patchfile);
	પાછા status;
}
