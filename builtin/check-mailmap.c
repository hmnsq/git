#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ident.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mailmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

ਸਥਿਰ ਪੂਰਨ use_stdin;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *mailmap_file, *mailmap_blob;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_ਸਿਰਾ()
};

ਸਥਿਰ ਖਾਲੀ check_mailmap(ਬਣਤਰ string_list *mailmap, ਅਚਲ ਅੱਖਰ *contact)
{
	ਅਚਲ ਅੱਖਰ *name, *mail;
	ਮਾਪ_t namelen, maillen;
	ਬਣਤਰ ident_ਵੰਡੋ ident;

	ਜੇ (!ਵੰਡੋ_ident_ਸਤਰ(&ident, contact, ਲੜੀ_ਲੰਬਾਈ(contact))) {
		name = ident.name_begin;
		namelen = ident.name_ਸਿਰਾ - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_ਸਿਰਾ - ident.mail_begin;
	} ਨਹੀਂਤਾਂ {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = ਲੜੀ_ਲੰਬਾਈ(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	ਜੇ (namelen)
		ਲਿਖੋ("%.*s ", (ਪੂਰਨ)namelen, name);
	ਲਿਖੋ("<%.*s>\n", (ਪੂਰਨ)maillen, mail);
}

ਪੂਰਨ cmd_check_mailmap(ਪੂਰਨ argc,
		      ਅਚਲ ਅੱਖਰ **argv,
		      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i;
	ਬਣਤਰ string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, check_mailmap_options,
			     check_mailmap_usage, 0);
	ਜੇ (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	ਜੇ (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	ਜੇ (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	ਲਈ (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	ਜੇ (use_stdin) {
		ਬਣਤਰ strbuf buf = STRBUF_INIT;
		ਜਦੋਂ (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	ਵਾਪਸ 0;
}
