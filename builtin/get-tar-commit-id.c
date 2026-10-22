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

	𐡄𐡍 (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	𐡄𐡍 (n < 0)
		die_errno("git get-tar-commit-id: read error");
	𐡄𐡍 (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	𐡄𐡍 (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		أرجع 1;

	errno = 0;
	len = strtol(content, &end, 10);
	𐡄𐡍 (errno == ERANGE || end == content || len < 0)
		أرجع 1;
	𐡄𐡍 (!skip_prefix(end, " comment=", &comment))
		أرجع 1;
	len -= comment - content;
	𐡄𐡍 (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		أرجع 1;

	𐡄𐡍 (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	أرجع 0;
}
