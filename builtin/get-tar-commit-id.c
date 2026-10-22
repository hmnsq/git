/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#ضمن "builtin.h"
#ضمن "commit.h"
#ضمن "tar.h"

ساكن ثابت حرف builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

صحيح cmd_get_tar_commit_id(صحيح argc,
			  ثابت حرف **argv,
			  ثابت حرف *prefix,
			  بنية repository *repo UNUSED)
{
	حرف buffer[HEADERSIZE];
	بنية ustar_header *header = (بنية ustar_header *)buffer;
	حرف *content = buffer + RECORDSIZE;
	ثابت حرف *comment;
	ssize_t n;
	long len;
	حرف *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	אם (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	אם (n < 0)
		die_errno("git get-tar-commit-id: read error");
	אם (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	אם (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		החזר 1;

	errno = 0;
	len = strtol(content, &end, 10);
	אם (errno == ERANGE || end == content || len < 0)
		החזר 1;
	אם (!skip_prefix(end, " comment=", &comment))
		החזר 1;
	len -= comment - content;
	אם (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		החזר 1;

	אם (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	החזר 0;
}
