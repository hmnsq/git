/*
 * Another stupid program, this one parsing the headers of an
 * email to figure out authorship and subject
 */
#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "mailinfo.h"
#അന്തര്ഭാവ "parse-options.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം mailinfo_usage[] = {
	/* TRANSLATORS: keep <> in "<" mail ">" info. */
	N_("git mailinfo [<options>] <msg> <patch> < mail >info"),
	ശൂന്യ,
};

ഘടന metainfo_charset
{
	എണ്ണൽ {
		CHARSET_DEFAULT,
		CHARSET_NO_REENCODE,
		CHARSET_EXPLICIT,
	} policy;
	സ്ഥിരം അക്ഷരം *charset;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_explicit_encoding(സ്ഥിരം ഘടന option *opt,
				       സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന metainfo_charset *meta_charset = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	meta_charset->policy = CHARSET_EXPLICIT;
	meta_charset->charset = arg;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_quoted_cr(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (mailinfo_parse_quoted_cr_action(arg, opt->മൂല്യം) != 0)
		തിരികെ error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_mailinfo(പൂർണ്ണസംഖ്യ argc,
		 സ്ഥിരം അക്ഷരം **argv,
		 സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo UNUSED)
{
	ഘടന metainfo_charset meta_charset;
	ഘടന mailinfo mi;
	പൂർണ്ണസംഖ്യ status;
	അക്ഷരം *msgfile, *patchfile;

	ഘടന option options[] = {
		OPT_BOOL('k', ശൂന്യ, &mi.keep_subject, N_("keep subject")),
		OPT_BOOL('b', ശൂന്യ, &mi.keep_non_patch_brackets_in_subject,
			 N_("keep non patch brackets in subject")),
		OPT_BOOL('m', "message-id", &mi.add_message_id,
			 N_("copy Message-ID to the end of commit message")),
		OPT_SET_INT_F('u', ശൂന്യ, &meta_charset.policy,
			      N_("re-code metadata to i18n.commitEncoding"),
			      CHARSET_DEFAULT, PARSE_OPT_NONEG),
		OPT_SET_INT_F('n', ശൂന്യ, &meta_charset.policy,
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

	എങ്കിൽ (argc != 2)
		usage_with_options(mailinfo_usage, options);

	തിരഞ്ഞെടുപ്പ് (meta_charset.policy) {
	അവസ്ഥ CHARSET_DEFAULT:
		mi.metainfo_charset = get_commit_output_encoding();
		നിർത്തുക;
	അവസ്ഥ CHARSET_NO_REENCODE:
		mi.metainfo_charset = ശൂന്യ;
		നിർത്തുക;
	അവസ്ഥ CHARSET_EXPLICIT:
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("invalid meta_charset.policy");
	}

	mi.നിവേശം = stdin;
	mi.output = stdout;

	msgfile = prefix_filename(prefix, argv[0]);
	patchfile = prefix_filename(prefix, argv[1]);

	status = !!mailinfo(&mi, msgfile, patchfile);
	clear_mailinfo(&mi);

	സ്വതന്ത്രം(msgfile);
	സ്വതന്ത്രം(patchfile);
	തിരികെ status;
}
