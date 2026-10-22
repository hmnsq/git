#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "ident.h"
#अन्तर्भाव "mailmap.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "write-or-die.h"

स्थिर पूर्णांक use_stdin;
स्थिर स्थिरांक अक्षर *mailmap_file, *mailmap_blob;
स्थिर स्थिरांक अक्षर * स्थिरांक check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
रिक्त
};

स्थिर स्थिरांक संरचना option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_END()
};

स्थिर शून्य check_mailmap(संरचना string_list *mailmap, स्थिरांक अक्षर *contact)
{
	स्थिरांक अक्षर *name, *mail;
	माप_प्रकार namelen, maillen;
	संरचना ident_split ident;

	जर (!split_ident_ओळ(&ident, contact, म_माप(contact))) {
		name = ident.name_begin;
		namelen = ident.name_end - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_end - ident.mail_begin;
	} अन्यथा {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = म_माप(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	जर (namelen)
		म_लिखो("%.*s ", (पूर्णांक)namelen, name);
	म_लिखो("<%.*s>\n", (पूर्णांक)maillen, mail);
}

पूर्णांक cmd_check_mailmap(पूर्णांक argc,
		      स्थिरांक अक्षर **argv,
		      स्थिरांक अक्षर *prefix,
		      संरचना repository *repo UNUSED)
{
	पूर्णांक i;
	संरचना string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, prefix, check_mailmap_options,
			     check_mailmap_usage, 0);
	जर (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	जर (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	जर (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	साठी (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(मानक_निकास, "stdout");

	जर (use_stdin) {
		संरचना strbuf buf = STRBUF_INIT;
		पर्यंत (strbuf_getline_lf(&buf, मानक_निवेश) != खातापूर्ण) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(मानक_निकास, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	परत 0;
}
