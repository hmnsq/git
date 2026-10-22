#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "ident.h"
#ಅನ್ತರ್ಭಾವ "mailmap.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_stdin;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mailmap_file, *mailmap_blob;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_ಕೊನೆ()
};

ಸ್ಥಿರ ಶೂನ್ಯ check_mailmap(ರಚನೆ string_list *mailmap, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *contact)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, *mail;
	ಗಾತ್ರ_t namelen, maillen;
	ರಚನೆ ident_ವಿಭಜಿಸು ident;

	ಆದರೆ (!ವಿಭಜಿಸು_ident_ಸಾಲು(&ident, contact, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(contact))) {
		name = ident.name_begin;
		namelen = ident.name_ಕೊನೆ - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_ಕೊನೆ - ident.mail_begin;
	} ಇಲ್ಲದಿದ್ದರೆ {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	ಆದರೆ (namelen)
		ಬರೆ("%.*s ", (ಪೂರ್ಣಾಂಕ)namelen, name);
	ಬರೆ("<%.*s>\n", (ಪೂರ್ಣಾಂಕ)maillen, mail);
}

ಪೂರ್ಣಾಂಕ cmd_check_mailmap(ಪೂರ್ಣಾಂಕ argc,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		      ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, check_mailmap_options,
			     check_mailmap_usage, 0);
	ಆದರೆ (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	ಆದರೆ (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	ಆದರೆ (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	ಗಾಗಿ (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	ಆದರೆ (use_stdin) {
		ರಚನೆ strbuf buf = STRBUF_INIT;
		ಆಗ (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	ಹಿಂತಿರುಗಿ 0;
}
