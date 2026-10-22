/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#شامل "builtin.h"
#شامل "commit.h"
#شامل "tar.h"

ایستا ثابت نویسه builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

صحیح cmd_get_tar_commit_id(صحیح argc,
			  ثابت نویسه **argv,
			  ثابت نویسه *prefix,
			  ساختار repository *repo UNUSED)
{
	نویسه buffer[HEADERSIZE];
	ساختار ustar_header *header = (ساختار ustar_header *)buffer;
	نویسه *content = buffer + RECORDSIZE;
	ثابت نویسه *comment;
	ssize_t n;
	long len;
	نویسه *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	اگر (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	اگر (n < 0)
		die_errno("git get-tar-commit-id: read error");
	اگر (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	اگر (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		بازگشت 1;

	errno = 0;
	len = strtol(content, &end, 10);
	اگر (errno == ERANGE || end == content || len < 0)
		بازگشت 1;
	اگر (!skip_prefix(end, " comment=", &comment))
		بازگشت 1;
	len -= comment - content;
	اگر (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		بازگشت 1;

	اگر (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	بازگشت 0;
}
