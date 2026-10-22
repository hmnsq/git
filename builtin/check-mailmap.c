#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "ident.h"
#شامل "mailmap.h"
#شامل "parse-options.h"
#شامل "strbuf.h"
#شامل "string-list.h"
#شامل "write-or-die.h"

ایستا صحیح use_stdin;
ایستا ثابت نویسه *mailmap_file, *mailmap_blob;
ایستا ثابت نویسه * ثابت check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
پوچ
};

ایستا ثابت ساختار option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_END()
};

ایستا تهی check_mailmap(ساختار string_list *mailmap, ثابت نویسه *contact)
{
	ثابت نویسه *name, *mail;
	size_t namelen, maillen;
	ساختار ident_split ident;

	اگر (!split_ident_سطر(&ident, contact, strlen(contact))) {
		name = ident.name_begin;
		namelen = ident.name_end - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_end - ident.mail_begin;
	} وگرنه {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = strlen(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	اگر (namelen)
		printf("%.*s ", (صحیح)namelen, name);
	printf("<%.*s>\n", (صحیح)maillen, mail);
}

صحیح cmd_check_mailmap(صحیح argc,
		      ثابت نویسه **argv,
		      ثابت نویسه *prefix,
		      ساختار repository *repo UNUSED)
{
	صحیح i;
	ساختار string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, پوچ);
	argc = parse_options(argc, argv, prefix, check_mailmap_options,
			     check_mailmap_usage, 0);
	اگر (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	اگر (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	اگر (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	برای (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	اگر (use_stdin) {
		ساختار strbuf buf = STRBUF_INIT;
		وقتی (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	بازگشت 0;
}
