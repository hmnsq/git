/*
 * Another stupid program, this one parsing the headers of an
 * email to figure out authorship and subject
 */
#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "mailinfo.h"
#அந்தர்பாவ "parse-options.h"

நிலையான மாறிலி எழுத்து * மாறிலி mailinfo_usage[] = {
	/* TRANSLATORS: keep <> in "<" mail ">" info. */
	N_("git mailinfo [<options>] <msg> <patch> < mail >info"),
	வெற்று,
};

கட்டமைப்பு metainfo_charset
{
	எண்ணிக்கை {
		CHARSET_DEFAULT,
		CHARSET_NO_REENCODE,
		CHARSET_EXPLICIT,
	} policy;
	மாறிலி எழுத்து *charset;
};

நிலையான முழு parse_opt_explicit_encoding(மாறிலி கட்டமைப்பு option *opt,
				       மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு metainfo_charset *meta_charset = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	meta_charset->policy = CHARSET_EXPLICIT;
	meta_charset->charset = arg;

	திரும்பு 0;
}

நிலையான முழு parse_opt_quoted_cr(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);

	எனில் (mailinfo_parse_quoted_cr_action(arg, opt->மதிப்பு) != 0)
		திரும்பு error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	திரும்பு 0;
}

முழு cmd_mailinfo(முழு argc,
		 மாறிலி எழுத்து **argv,
		 மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு metainfo_charset meta_charset;
	கட்டமைப்பு mailinfo mi;
	முழு status;
	எழுத்து *msgfile, *patchfile;

	கட்டமைப்பு option options[] = {
		OPT_BOOL('k', வெற்று, &mi.keep_subject, N_("keep subject")),
		OPT_BOOL('b', வெற்று, &mi.keep_non_patch_brackets_in_subject,
			 N_("keep non patch brackets in subject")),
		OPT_BOOL('m', "message-id", &mi.add_message_id,
			 N_("copy Message-ID to the end of commit message")),
		OPT_SET_INT_F('u', வெற்று, &meta_charset.policy,
			      N_("re-code metadata to i18n.commitEncoding"),
			      CHARSET_DEFAULT, PARSE_OPT_NONEG),
		OPT_SET_INT_F('n', வெற்று, &meta_charset.policy,
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

	எனில் (argc != 2)
		usage_with_options(mailinfo_usage, options);

	தேர்வு (meta_charset.policy) {
	நிலை CHARSET_DEFAULT:
		mi.metainfo_charset = get_commit_output_encoding();
		நிறுத்து;
	நிலை CHARSET_NO_REENCODE:
		mi.metainfo_charset = வெற்று;
		நிறுத்து;
	நிலை CHARSET_EXPLICIT:
		நிறுத்து;
	இயல்பு:
		BUG("invalid meta_charset.policy");
	}

	mi.உள்ளீடு = stdin;
	mi.output = stdout;

	msgfile = prefix_filename(prefix, argv[0]);
	patchfile = prefix_filename(prefix, argv[1]);

	status = !!mailinfo(&mi, msgfile, patchfile);
	clear_mailinfo(&mi);

	விடுவி(msgfile);
	விடுவி(patchfile);
	திரும்பு status;
}
