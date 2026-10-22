#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "ident.h"
#ضمن "mailmap.h"
#ضمن "parse-options.h"
#ضمن "strbuf.h"
#ضمن "string-list.h"
#ضمن "write-or-die.h"

ساكن صحيح use_stdin;
ساكن ثابت حرف *mailmap_file, *mailmap_blob;
ساكن ثابت حرف * ثابت check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
عدم
};

ساكن ثابت بنية option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_سرا()
};

ساكن فراغ check_mailmap(بنية string_list *mailmap, ثابت حرف *contact)
{
	ثابت حرف *name, *mail;
	ناپ_t namelen, maillen;
	بنية ident_ونڈو ident;

	إذا (!ونڈو_ident_سطر(&ident, contact, لڑی_لمبائی(contact))) {
		name = ident.name_begin;
		namelen = ident.name_سرا - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_سرا - ident.mail_begin;
	} وإلا {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = لڑی_لمبائی(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	إذا (namelen)
		لکھو("%.*s ", (صحيح)namelen, name);
	لکھو("<%.*s>\n", (صحيح)maillen, mail);
}

صحيح cmd_check_mailmap(صحيح argc,
		      ثابت حرف **argv,
		      ثابت حرف *اگیتر,
		      بنية repository *repo بن_ورتیا)
{
	صحيح i;
	بنية string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, اگیتر, check_mailmap_options,
			     check_mailmap_usage, 0);
	إذا (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	إذا (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	إذا (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	لكل (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	إذا (use_stdin) {
		بنية strbuf buf = STRBUF_INIT;
		طالما (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	أرجع 0;
}
