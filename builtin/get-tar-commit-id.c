/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "tar.h"

સ્થિર અચલ અક્ષર builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

પૂર્ણાંક cmd_get_tar_commit_id(પૂર્ણાંક argc,
			  અચલ અક્ષર **argv,
			  અચલ અક્ષર *prefix,
			  સંરચના repository *repo UNUSED)
{
	અક્ષર buffer[HEADERSIZE];
	સંરચના ustar_header *header = (સંરચના ustar_header *)buffer;
	અક્ષર *content = buffer + RECORDSIZE;
	અચલ અક્ષર *comment;
	ssize_t n;
	long len;
	અક્ષર *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	જો (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	જો (n < 0)
		die_errno("git get-tar-commit-id: read error");
	જો (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	જો (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		પાછા 1;

	errno = 0;
	len = strtol(content, &end, 10);
	જો (errno == ERANGE || end == content || len < 0)
		પાછા 1;
	જો (!skip_prefix(end, " comment=", &comment))
		પાછા 1;
	len -= comment - content;
	જો (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		પાછા 1;

	જો (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	પાછા 0;
}
