/*
 * Another stupid program, this one parsing the headers of an
 * email to figure out authorship and subject
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "abspath.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "strbuf.h"
#شامل "mailinfo.h"
#شامل "parse-options.h"

ساکن ثابت حرف * ثابت mailinfo_usage[] = {
	/* TRANSLATORS: keep <> in "<" mail ">" info. */
	N_("git mailinfo [<options>] <msg> <patch> < mail >info"),
	تہی,
};

ساخت metainfo_charset
{
	شمار {
		CHARSET_DEFAULT,
		CHARSET_NO_REENCODE,
		CHARSET_EXPLICIT,
	} policy;
	ثابت حرف *charset;
};

ساکن صحیح parse_opt_explicit_encoding(ثابت ساخت option *opt,
				       ثابت حرف *arg, صحیح unset)
{
	ساخت metainfo_charset *meta_charset = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	meta_charset->policy = CHARSET_EXPLICIT;
	meta_charset->charset = arg;

	واپس 0;
}

ساکن صحیح parse_opt_quoted_cr(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);

	اگر (mailinfo_parse_quoted_cr_action(arg, opt->قدر) != 0)
		واپس error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	واپس 0;
}

صحیح cmd_mailinfo(صحیح argc,
		 ثابت حرف **argv,
		 ثابت حرف *سابقہ,
		 ساخت repository *repo غیر_مستعمل)
{
	ساخت metainfo_charset meta_charset;
	ساخت mailinfo mi;
	صحیح حالت_2;
	حرف *msgfile, *patchfile;

	ساخت option options[] = {
		OPT_BOOL('k', تہی, &mi.keep_subject, N_("keep subject")),
		OPT_BOOL('b', تہی, &mi.keep_non_patch_brackets_اندر_subject,
			 N_("keep non patch brackets in subject")),
		OPT_BOOL('m', "message-id", &mi.جوڑو_پیغام_id,
			 N_("copy Message-ID to the end of commit message")),
		OPT_سیٹ_INT_F('u', تہی, &meta_charset.policy,
			      N_("re-code metadata to i18n.commitEncoding"),
			      CHARSET_DEFAULT, PARSE_OPT_NONEG),
		OPT_سیٹ_INT_F('n', تہی, &meta_charset.policy,
			      N_("disable charset re-coding of metadata"),
			      CHARSET_NO_REENCODE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "encoding", &meta_charset, N_("encoding"),
			       N_("re-code metadata to this encoding"),
			       PARSE_OPT_NONEG, parse_opt_explicit_encoding),
		OPT_BOOL(0, "scissors", &mi.use_scissors, N_("use scissors")),
		OPT_CALLBACK_F(0, "quoted-cr", &mi.quoted_cr, N_("<action>"),
			       N_("action when quoted CR is found"),
			       PARSE_OPT_NONEG, parse_opt_quoted_cr),
		OPT_HIDDEN_BOOL(0, "inbody-headers", &mi.use_inbody_سرخیاں,
			 N_("use headers in message's body")),
		OPT_انتہا()
	};

	تیاری_mailinfo(the_repository, &mi);
	meta_charset.policy = CHARSET_DEFAULT;

	argc = parse_options(argc, argv, سابقہ, options, mailinfo_usage, 0);

	اگر (argc != 2)
		usage_with_options(mailinfo_usage, options);

	انتخاب (meta_charset.policy) {
	حالت CHARSET_DEFAULT:
		mi.metainfo_charset = get_commit_output_encoding();
		توڑو;
	حالت CHARSET_NO_REENCODE:
		mi.metainfo_charset = تہی;
		توڑو;
	حالت CHARSET_EXPLICIT:
		توڑو;
	طےشدہ:
		BUG("invalid meta_charset.policy");
	}

	mi.داخلہ = stdin;
	mi.output = stdout;

	msgfile = سابقہ_filename(سابقہ, argv[0]);
	patchfile = سابقہ_filename(سابقہ, argv[1]);

	حالت_2 = !!mailinfo(&mi, msgfile, patchfile);
	clear_mailinfo(&mi);

	آزاد(msgfile);
	آزاد(patchfile);
	واپس حالت_2;
}
