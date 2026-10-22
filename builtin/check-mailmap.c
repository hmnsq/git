#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "ident.h"
#અન્તર્ભાવ "mailmap.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "write-or-die.h"

સ્થિર પૂર્ણાંક use_stdin;
સ્થિર અચલ અક્ષર *mailmap_file, *mailmap_blob;
સ્થિર અચલ અક્ષર * અચલ check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
ખાલી
};

સ્થિર અચલ સંરચના option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_END()
};

સ્થિર શૂન્ય check_mailmap(સંરચના string_list *mailmap, અચલ અક્ષર *contact)
{
	અચલ અક્ષર *name, *mail;
	size_t namelen, maillen;
	સંરચના ident_split ident;

	જો (!split_ident_લીટી(&ident, contact, strlen(contact))) {
		name = ident.name_begin;
		namelen = ident.name_end - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_end - ident.mail_begin;
	} નહીંતર {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = strlen(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	જો (namelen)
		printf("%.*s ", (પૂર્ણાંક)namelen, name);
	printf("<%.*s>\n", (પૂર્ણાંક)maillen, mail);
}

પૂર્ણાંક cmd_check_mailmap(પૂર્ણાંક argc,
		      અચલ અક્ષર **argv,
		      અચલ અક્ષર *prefix,
		      સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i;
	સંરચના string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, ખાલી);
	argc = parse_options(argc, argv, prefix, check_mailmap_options,
			     check_mailmap_usage, 0);
	જો (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	જો (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	જો (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	માટે (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	જો (use_stdin) {
		સંરચના strbuf buf = STRBUF_INIT;
		જ્યારે (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	પાછા 0;
}
