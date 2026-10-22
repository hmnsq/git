/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "tar.h"

நிலையான மாறிலி எழுத்து builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

முழு cmd_get_tar_commit_id(முழு argc,
			  மாறிலி எழுத்து **argv,
			  மாறிலி எழுத்து *prefix,
			  கட்டமைப்பு repository *repo UNUSED)
{
	எழுத்து buffer[HEADERSIZE];
	கட்டமைப்பு ustar_header *header = (கட்டமைப்பு ustar_header *)buffer;
	எழுத்து *content = buffer + RECORDSIZE;
	மாறிலி எழுத்து *comment;
	ssize_t n;
	long len;
	எழுத்து *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	எனில் (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	எனில் (n < 0)
		die_errno("git get-tar-commit-id: read error");
	எனில் (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	எனில் (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		திரும்பு 1;

	errno = 0;
	len = strtol(content, &end, 10);
	எனில் (errno == ERANGE || end == content || len < 0)
		திரும்பு 1;
	எனில் (!skip_prefix(end, " comment=", &comment))
		திரும்பு 1;
	len -= comment - content;
	எனில் (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		திரும்பு 1;

	எனில் (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	திரும்பு 0;
}
