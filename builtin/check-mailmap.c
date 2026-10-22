#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "ident.h"
#അന്തര്ഭാവ "mailmap.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "write-or-die.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ use_stdin;
സ്ഥിതം സ്ഥിരം അക്ഷരം *mailmap_file, *mailmap_blob;
സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
ശൂന്യ
};

സ്ഥിതം സ്ഥിരം ഘടന option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_END()
};

സ്ഥിതം ശൂന്യം check_mailmap(ഘടന string_list *mailmap, സ്ഥിരം അക്ഷരം *contact)
{
	സ്ഥിരം അക്ഷരം *name, *mail;
	size_t namelen, maillen;
	ഘടന ident_split ident;

	എങ്കിൽ (!split_ident_വരി(&ident, contact, strlen(contact))) {
		name = ident.name_begin;
		namelen = ident.name_end - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_end - ident.mail_begin;
	} അല്ലെങ്കിൽ {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = strlen(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	എങ്കിൽ (namelen)
		printf("%.*s ", (പൂർണ്ണസംഖ്യ)namelen, name);
	printf("<%.*s>\n", (പൂർണ്ണസംഖ്യ)maillen, mail);
}

പൂർണ്ണസംഖ്യ cmd_check_mailmap(പൂർണ്ണസംഖ്യ argc,
		      സ്ഥിരം അക്ഷരം **argv,
		      സ്ഥിരം അക്ഷരം *prefix,
		      ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, ശൂന്യ);
	argc = parse_options(argc, argv, prefix, check_mailmap_options,
			     check_mailmap_usage, 0);
	എങ്കിൽ (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	എങ്കിൽ (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	എങ്കിൽ (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	വേണ്ടി (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	എങ്കിൽ (use_stdin) {
		ഘടന strbuf buf = STRBUF_INIT;
		ആകുമ്പോൾ (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	തിരികെ 0;
}
