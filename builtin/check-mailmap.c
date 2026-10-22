#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "ident.h"
#include "mailmap.h"
#include "parse-options.h"
#include "strbuf.h"
#include "string-list.h"
#include "write-or-die.h"

static целое use_stdin;
static const char *mailmap_file, *mailmap_blob;
static const char * const check_mailmap_usage[] = {
N_("git check-mailmap [<options>] <contact>..."),
NULL
};

static const struct option check_mailmap_options[] = {
	OPT_BOOL(0, "stdin", &use_stdin, N_("also read contacts from stdin")),
	OPT_FILENAME(0, "mailmap-file", &mailmap_file, N_("read additional mailmap entries from file")),
	OPT_STRING(0, "mailmap-blob", &mailmap_blob, N_("blob"), N_("read additional mailmap entries from blob")),
	OPT_END()
};

static void check_mailmap(struct string_list *mailmap, const char *contact)
{
	const char *name, *mail;
	size_t namelen, maillen;
	struct ident_split ident;

	если (!split_ident_line(&ident, contact, strlen(contact))) {
		name = ident.name_begin;
		namelen = ident.name_end - ident.name_begin;
		mail = ident.mail_begin;
		maillen = ident.mail_end - ident.mail_begin;
	} иначе {
		name = "";
		namelen = 0;
		mail = contact;
		maillen = strlen(contact);
	}

	map_user(mailmap, &mail, &maillen, &name, &namelen);

	если (namelen)
		printf("%.*s ", (целое)namelen, name);
	printf("<%.*s>\n", (целое)maillen, mail);
}

целое cmd_check_mailmap(целое argc,
		      const char **argv,
		      const char *prefix,
		      struct repository *repo UNUSED)
{
	целое i;
	struct string_list mailmap = STRING_LIST_INIT_NODUP;

	repo_config(the_repository, git_default_config, NULL);
	argc = parse_options(argc, argv, prefix, check_mailmap_options,
			     check_mailmap_usage, 0);
	если (argc == 0 && !use_stdin)
		die(_("no contacts specified"));

	read_mailmap(the_repository, &mailmap);
	если (mailmap_blob)
		read_mailmap_blob(the_repository, &mailmap, mailmap_blob);
	если (mailmap_file)
		read_mailmap_file(&mailmap, mailmap_file, 0);

	для (i = 0; i < argc; ++i)
		check_mailmap(&mailmap, argv[i]);
	maybe_flush_or_die(stdout, "stdout");

	если (use_stdin) {
		struct strbuf buf = STRBUF_INIT;
		пока (strbuf_getline_lf(&buf, stdin) != EOF) {
			check_mailmap(&mailmap, buf.buf);
			maybe_flush_or_die(stdout, "stdout");
		}
		strbuf_release(&buf);
	}

	clear_mailmap(&mailmap);
	вернуть 0;
}
