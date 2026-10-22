/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#include "builtin.h"
#include "commit.h"
#include "tar.h"

static const var_na builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

ཧྲིལ cmd_get_tar_commit_id(ཧྲིལ argc,
			  const var_na **argv,
			  const var_na *prefix,
			  sa_nracanaa repository *repo UNUSED)
{
	var_na buffer[HEADERSIZE];
	sa_nracanaa ustar_header *header = (sa_nracanaa ustar_header *)buffer;
	var_na *content = buffer + RECORDSIZE;
	const var_na *comment;
	ssize_t n;
	long len;
	var_na *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	གལ་ཏེ (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	གལ་ཏེ (n < 0)
		die_errno("git get-tar-commit-id: read error");
	གལ་ཏེ (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	གལ་ཏེ (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		ཕྱིར 1;

	errno = 0;
	len = strtol(content, &end, 10);
	གལ་ཏེ (errno == ERANGE || end == content || len < 0)
		ཕྱིར 1;
	གལ་ཏེ (!skip_prefix(end, " comment=", &comment))
		ཕྱིར 1;
	len -= comment - content;
	གལ་ཏེ (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		ཕྱིར 1;

	གལ་ཏེ (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	ཕྱིར 0;
}
