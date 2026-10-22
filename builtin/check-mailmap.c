#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "ident.h"
#అన్తర్భావ "mailmap.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "write-or-die.h"

స్థిర పూర్ణాంకం use_stdin;
స్థిర స్థిరరాశి అక్షరం *mailmap_file, *mailmap_blob;
స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
ఖాళీ
};

స్థిర స్థిరరాశి నిర్మాణం option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_చివర()
};

స్థిర శూన్య check_mailmap(నిర్మాణం string_list *mailmap, స్థిరరాశి అక్షరం *contact)
{
	స్థిరరాశి అక్షరం *name, *mail;
	కొలత_t namelen, maillen;
	నిర్మాణం ident_విభజించు ident;

	ఒకవేళ (!విభజించు_ident_పంక్తి(&ident, contact, అక్షరశ్రేణి_పొడవు(contact))) {
		name = ident.name_begin;
		namelen = ident.name_చివర - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_చివర - ident.mail_begin;
	} లేకపోతే {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = అక్షరశ్రేణి_పొడవు(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	ఒకవేళ (namelen)
		రాయి("%.*s ", (పూర్ణాంకం)namelen, name);
	రాయి("<%.*s>\n", (పూర్ణాంకం)maillen, mail);
}

పూర్ణాంకం cmd_check_mailmap(పూర్ణాంకం argc,
		      స్థిరరాశి అక్షరం **argv,
		      స్థిరరాశి అక్షరం *ఉపసర్గ,
		      నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i;
	నిర్మాణం string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, ఖాళీ);
	argc = parse_options(argc, argv, ఉపసర్గ, check_mailmap_options,
			     check_mailmap_usage, 0);
	ఒకవేళ (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	ఒకవేళ (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	ఒకవేళ (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	కొరకు (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	ఒకవేళ (use_stdin) {
		నిర్మాణం strbuf buf = STRBUF_INIT;
		అయితే (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	తిరిగి 0;
}
