#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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

ساکن صحیح use_stdin;
ساکن ثابت حرف *mailmap_file, *mailmap_blob;
ساکن ثابت حرف * ثابت check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
تہی
};

ساکن ثابت ساخت option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_انتہا()
};

ساکن خالی check_mailmap(ساخت string_list *mailmap, ثابت حرف *contact)
{
	ثابت حرف *name, *mail;
	سائز_t namelen, maillen;
	ساخت ident_بانٹو ident;

	اگر (!بانٹو_ident_سطر(&ident, contact, لڑی_لمبائی(contact))) {
		name = ident.name_begin;
		namelen = ident.name_انتہا - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_انتہا - ident.mail_begin;
	} ورنہ {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = لڑی_لمبائی(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	اگر (namelen)
		لکھو("%.*s ", (صحیح)namelen, name);
	لکھو("<%.*s>\n", (صحیح)maillen, mail);
}

صحیح cmd_check_mailmap(صحیح argc,
		      ثابت حرف **argv,
		      ثابت حرف *سابقہ,
		      ساخت repository *repo غیر_مستعمل)
{
	صحیح i;
	ساخت string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, تہی);
	argc = parse_options(argc, argv, سابقہ, check_mailmap_options,
			     check_mailmap_usage, 0);
	اگر (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	اگر (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	اگر (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	برائے (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	اگر (use_stdin) {
		ساخت strbuf buf = STRBUF_INIT;
		جب (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	واپس 0;
}
