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

ціле cmd_get_tar_commit_id(ціле argc,
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

	якщо (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	якщо (n < 0)
		die_errno("git get-tar-commit-id: read error");
	якщо (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	якщо (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		повернути 1;

	errno = 0;
	len = strtol(content, &end, 10);
	якщо (errno == ERANGE || end == content || len < 0)
		повернути 1;
	якщо (!skip_prefix(end, " comment=", &comment))
		повернути 1;
	len -= comment - content;
	якщо (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		повернути 1;

	якщо (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	повернути 0;
}
