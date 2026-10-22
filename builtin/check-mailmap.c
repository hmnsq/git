#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "ident.h"
#অন্তর্ভাব "mailmap.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "write-or-die.h"

স্থির পূর্ণসংখ্যা use_stdin;
স্থির ধ্রুবক অক্ষর *mailmap_file, *mailmap_blob;
স্থির ধ্রুবক অক্ষর * ধ্রুবক check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
ফাঁকা
};

স্থির ধ্রুবক কাঠামো option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_END()
};

স্থির শূন্য check_mailmap(কাঠামো string_list *mailmap, ধ্রুবক অক্ষর *contact)
{
	ধ্রুবক অক্ষর *name, *mail;
	আকার_t namelen, maillen;
	কাঠামো ident_split ident;

	যদি (!split_ident_সারি(&ident, contact, strlen(contact))) {
		name = ident.name_begin;
		namelen = ident.name_end - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_end - ident.mail_begin;
	} অন্যথা {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = strlen(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	যদি (namelen)
		printf("%.*s ", (পূর্ণসংখ্যা)namelen, name);
	printf("<%.*s>\n", (পূর্ণসংখ্যা)maillen, mail);
}

পূর্ণসংখ্যা cmd_check_mailmap(পূর্ণসংখ্যা argc,
		      ধ্রুবক অক্ষর **argv,
		      ধ্রুবক অক্ষর *prefix,
		      কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা i;
	কাঠামো string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, ফাঁকা);
	argc = parse_options(argc, argv, prefix, check_mailmap_options,
			     check_mailmap_usage, 0);
	যদি (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	যদি (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	যদি (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	জন্য (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	যদি (use_stdin) {
		কাঠামো strbuf buf = STRBUF_INIT;
		যখন (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	ফেরত 0;
}
