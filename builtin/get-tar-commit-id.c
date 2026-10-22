/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#include "builtin.h"
#include "commit.h"
#include "tar.h"

static const char builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

целое cmd_get_tar_commit_id(целое argc,
			  const char **argv,
			  const char *prefix,
			  struct repository *repo UNUSED)
{
	char buffer[HEADERSIZE];
	struct ustar_header *header = (struct ustar_header *)buffer;
	char *content = buffer + RECORDSIZE;
	const char *comment;
	ssize_t n;
	long len;
	char *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	если (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	если (n < 0)
		die_errno("git get-tar-commit-id: read error");
	если (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	если (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		вернуть 1;

	errno = 0;
	len = strtol(content, &end, 10);
	если (errno == ERANGE || end == content || len < 0)
		вернуть 1;
	если (!skip_prefix(end, " comment=", &comment))
		вернуть 1;
	len -= comment - content;
	если (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		вернуть 1;

	если (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	вернуть 0;
}
