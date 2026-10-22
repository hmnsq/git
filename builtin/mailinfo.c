/*
 * Another stupid program, this one parsing the headers of an
 * email to figure out authorship and subject
 */
#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "mailinfo.h"
#অন্তর্ভাব "parse-options.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক mailinfo_usage[] = {
	/* TRANSLATORS: keep <> in "<" mail ">" info. */
	N_("git mailinfo [<options>] <msg> <patch> < mail >info"),
	ফাঁকা,
};

কাঠামো metainfo_charset
{
	গণনা {
		CHARSET_DEFAULT,
		CHARSET_NO_REENCODE,
		CHARSET_EXPLICIT,
	} policy;
	ধ্রুবক অক্ষর *charset;
};

স্থির পূর্ণসংখ্যা parse_opt_explicit_encoding(ধ্রুবক কাঠামো option *opt,
				       ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো metainfo_charset *meta_charset = opt->মান;

	BUG_ON_OPT_NEG(unset);

	meta_charset->policy = CHARSET_EXPLICIT;
	meta_charset->charset = arg;

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা parse_opt_quoted_cr(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);

	যদি (mailinfo_parse_quoted_cr_action(arg, opt->মান) != 0)
		ফেরত error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	ফেরত 0;
}

পূর্ণসংখ্যা cmd_mailinfo(পূর্ণসংখ্যা argc,
		 ধ্রুবক অক্ষর **argv,
		 ধ্রুবক অক্ষর *prefix,
		 কাঠামো repository *repo UNUSED)
{
	কাঠামো metainfo_charset meta_charset;
	কাঠামো mailinfo mi;
	পূর্ণসংখ্যা status;
	অক্ষর *msgfile, *patchfile;

	কাঠামো option options[] = {
		OPT_BOOL('k', ফাঁকা, &mi.keep_subject, N_("keep subject")),
		OPT_BOOL('b', ফাঁকা, &mi.keep_non_patch_brackets_in_subject,
			 N_("keep non patch brackets in subject")),
		OPT_BOOL('m', "message-id", &mi.add_message_id,
			 N_("copy Message-ID to the end of commit message")),
		OPT_SET_INT_F('u', ফাঁকা, &meta_charset.policy,
			      N_("re-code metadata to i18n.commitEncoding"),
			      CHARSET_DEFAULT, PARSE_OPT_NONEG),
		OPT_SET_INT_F('n', ফাঁকা, &meta_charset.policy,
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

	যদি (argc != 2)
		usage_with_options(mailinfo_usage, options);

	নির্বাচন (meta_charset.policy) {
	ক্ষেত্র CHARSET_DEFAULT:
		mi.metainfo_charset = get_commit_output_encoding();
		বিরতি;
	ক্ষেত্র CHARSET_NO_REENCODE:
		mi.metainfo_charset = ফাঁকা;
		বিরতি;
	ক্ষেত্র CHARSET_EXPLICIT:
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("invalid meta_charset.policy");
	}

	mi.প্রবেশ = stdin;
	mi.output = stdout;

	msgfile = prefix_filename(prefix, argv[0]);
	patchfile = prefix_filename(prefix, argv[1]);

	status = !!mailinfo(&mi, msgfile, patchfile);
	clear_mailinfo(&mi);

	মুক্ত(msgfile);
	মুক্ত(patchfile);
	ফেরত status;
}
