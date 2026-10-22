#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "ident.h"
#ଅନ୍ତର୍ଭାଵ "mailmap.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_stdin;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *mailmap_file, *mailmap_blob;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
ଖାଲି
};

ସ୍ଥିର ଅଚଳ ସଂରଚନା option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_END()
};

ସ୍ଥିର ଶୂନ୍ୟ check_mailmap(ସଂରଚନା string_list *mailmap, ଅଚଳ ଅକ୍ଷର *contact)
{
	ଅଚଳ ଅକ୍ଷର *name, *mail;
	size_t namelen, maillen;
	ସଂରଚନା ident_split ident;

	ଯଦି (!split_ident_ଧାଡ଼ି(&ident, contact, strlen(contact))) {
		name = ident.name_begin;
		namelen = ident.name_end - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_end - ident.mail_begin;
	} ଅନ୍ୟଥା {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = strlen(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	ଯଦି (namelen)
		printf("%.*s ", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)namelen, name);
	printf("<%.*s>\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)maillen, mail);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_check_mailmap(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		      ଅଚଳ ଅକ୍ଷର **argv,
		      ଅଚଳ ଅକ୍ଷର *prefix,
		      ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, ଖାଲି);
	argc = parse_options(argc, argv, prefix, check_mailmap_options,
			     check_mailmap_usage, 0);
	ଯଦି (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	ଯଦି (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	ଯଦି (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	ପାଇଁ (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	ଯଦି (use_stdin) {
		ସଂରଚନା strbuf buf = STRBUF_INIT;
		ଯେତେବେଳେ (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	ଫେରନ୍ତୁ 0;
}
