#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "ident.h"
#समावेश "mailmap.h"
#समावेश "parse-options.h"
#समावेश "strbuf.h"
#समावेश "string-list.h"
#समावेश "write-or-die.h"

अटल पूर्णांक use_stdin;
अटल स्थिर अक्षर *mailmap_file, *mailmap_blob;
अटल स्थिर अक्षर * स्थिर check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
शून्य
};

अटल स्थिर काष्ठा option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_END()
};

अटल व्योम check_mailmap(काष्ठा string_list *mailmap, स्थिर अक्षर *contact)
{
	स्थिर अक्षर *name, *mail;
	माप_प्रकार namelen, maillen;
	काष्ठा ident_split ident;

	अगर (!split_ident_पंक्ति(&ident, contact, म_माप(contact))) {
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

	अगर (namelen)
		म_लिखो("%.*s ", (पूर्णांक)namelen, name);
	म_लिखो("<%.*s>\n", (पूर्णांक)maillen, mail);
}

पूर्णांक cmd_check_mailmap(पूर्णांक argc,
		      स्थिर अक्षर **argv,
		      स्थिर अक्षर *prefix,
		      काष्ठा repository *repo UNUSED)
{
	पूर्णांक i;
	काष्ठा string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, शून्य);
	argc = parse_options(argc, argv, prefix, check_mailmap_options,
			     check_mailmap_usage, 0);
	अगर (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	अगर (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	अगर (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	क्रम (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(मानक_निकास, "stdout");

	अगर (use_stdin) {
		काष्ठा strbuf buf = STRBUF_INIT;
		जबतक (strbuf_getline_lf(&buf, मानक_निवेश) != खातापूर्ण) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(मानक_निकास, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	वापस 0;
}
