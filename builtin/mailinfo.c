/*
 * Another stupid program, this one parsing the headers of an
 * email to figure out authorship and subject
 */
#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "strbuf.h"
#ضمن "mailinfo.h"
#ضمن "parse-options.h"

ساكن ثابت حرف * ثابت mailinfo_usage[] = {
	/* TRANSLATORS: keep <> in "<" mail ">" info. */
	N_("git mailinfo [<options>] <msg> <patch> < mail >info"),
	عدم,
};

بنية metainfo_charset
{
	تعداد {
		CHARSET_DEFAULT,
		CHARSET_NO_REENCODE,
		CHARSET_EXPLICIT,
	} policy;
	ثابت حرف *charset;
};

ساكن صحيح parse_opt_explicit_encoding(ثابت بنية option *opt,
				       ثابت حرف *arg, صحيح unset)
{
	بنية metainfo_charset *meta_charset = opt->قيمة;

	BUG_ON_OPT_NEG(unset);

	meta_charset->policy = CHARSET_EXPLICIT;
	meta_charset->charset = arg;

	إرجاع 0;
}

ساكن صحيح parse_opt_quoted_cr(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);

	إذا (mailinfo_parse_quoted_cr_action(arg, opt->قيمة) != 0)
		إرجاع error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	إرجاع 0;
}

صحيح cmd_mailinfo(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *prefix,
		 بنية repository *repo UNUSED)
{
	بنية metainfo_charset meta_charset;
	بنية mailinfo mi;
	صحيح status;
	حرف *msgfile, *patchfile;

	بنية option options[] = {
		OPT_BOOL('k', عدم, &mi.keep_subject, N_("keep subject")),
		OPT_BOOL('b', عدم, &mi.keep_non_patch_brackets_in_subject,
			 N_("keep non patch brackets in subject")),
		OPT_BOOL('m', "message-id", &mi.add_message_id,
			 N_("copy Message-ID to the end of commit message")),
		OPT_SET_INT_F('u', عدم, &meta_charset.policy,
			      N_("re-code metadata to i18n.commitEncoding"),
			      CHARSET_DEFAULT, PARSE_OPT_NONEG),
		OPT_SET_INT_F('n', عدم, &meta_charset.policy,
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

	إذا (argc != 2)
		usage_with_options(mailinfo_usage, options);

	بدل (meta_charset.policy) {
	حالة CHARSET_DEFAULT:
		mi.metainfo_charset = get_commit_output_encoding();
		اكسر;
	حالة CHARSET_NO_REENCODE:
		mi.metainfo_charset = عدم;
		اكسر;
	حالة CHARSET_EXPLICIT:
		اكسر;
	افتراضي:
		BUG("invalid meta_charset.policy");
	}

	mi.إدخال = stdin;
	mi.output = stdout;

	msgfile = prefix_filename(prefix, argv[0]);
	patchfile = prefix_filename(prefix, argv[1]);

	status = !!mailinfo(&mi, msgfile, patchfile);
	clear_mailinfo(&mi);

	حرر(msgfile);
	حرر(patchfile);
	إرجاع status;
}
