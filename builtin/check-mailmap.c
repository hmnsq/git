#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "ident.h"
#அந்தர்பாவ "mailmap.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "write-or-die.h"

நிலையான முழு use_stdin;
நிலையான மாறிலி எழுத்து *mailmap_file, *mailmap_blob;
நிலையான மாறிலி எழுத்து * மாறிலி check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
வெற்று
};

நிலையான மாறிலி கட்டமைப்பு option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_END()
};

நிலையான வெறுமை check_mailmap(கட்டமைப்பு string_list *mailmap, மாறிலி எழுத்து *contact)
{
	மாறிலி எழுத்து *name, *mail;
	size_t namelen, maillen;
	கட்டமைப்பு ident_split ident;

	எனில் (!split_ident_வரி(&ident, contact, strlen(contact))) {
		name = ident.name_begin;
		namelen = ident.name_end - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_end - ident.mail_begin;
	} இல்லையெனில் {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = strlen(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	எனில் (namelen)
		printf("%.*s ", (முழு)namelen, name);
	printf("<%.*s>\n", (முழு)maillen, mail);
}

முழு cmd_check_mailmap(முழு argc,
		      மாறிலி எழுத்து **argv,
		      மாறிலி எழுத்து *prefix,
		      கட்டமைப்பு repository *repo UNUSED)
{
	முழு i;
	கட்டமைப்பு string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, வெற்று);
	argc = parse_options(argc, argv, prefix, check_mailmap_options,
			     check_mailmap_usage, 0);
	எனில் (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	எனில் (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	எனில் (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	க்கு (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	எனில் (use_stdin) {
		கட்டமைப்பு strbuf buf = STRBUF_INIT;
		போது (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	திரும்பு 0;
}
