/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "tar.h"

স্থির ধ্রুবক অক্ষর builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

পূর্ণসংখ্যা cmd_get_tar_commit_id(পূর্ণসংখ্যা argc,
			  ধ্রুবক অক্ষর **argv,
			  ধ্রুবক অক্ষর *prefix,
			  কাঠামো repository *repo UNUSED)
{
	অক্ষর বাফার[HEADERSIZE];
	কাঠামো ustar_header *header = (কাঠামো ustar_header *)বাফার;
	অক্ষর *content = বাফার + RECORDSIZE;
	ধ্রুবক অক্ষর *comment;
	ssize_t n;
	long len;
	অক্ষর *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	যদি (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, বাফার, HEADERSIZE);
	যদি (n < 0)
		die_errno("git get-tar-commit-id: read error");
	যদি (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	যদি (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		ফেরত 1;

	errno = 0;
	len = strtol(content, &end, 10);
	যদি (errno == ERANGE || end == content || len < 0)
		ফেরত 1;
	যদি (!skip_prefix(end, " comment=", &comment))
		ফেরত 1;
	len -= comment - content;
	যদি (len < 1 || !(len % 2) ||
	    hash_algo_by_দৈর্ঘ্য((len - 1) / 2) == GIT_HASH_UNKNOWN)
		ফেরত 1;

	যদি (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	ফেরত 0;
}
