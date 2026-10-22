/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "tar.h"

স্থির ধ্রুব বর্ণ builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

পূর্ণাঙ্ক cmd_get_tar_commit_id(পূর্ণাঙ্ক argc,
			  ধ্রুব বর্ণ **argv,
			  ধ্রুব বর্ণ *prefix,
			  সংরচনা repository *repo UNUSED)
{
	বর্ণ buffer[HEADERSIZE];
	সংরচনা ustar_header *header = (সংরচনা ustar_header *)buffer;
	বর্ণ *content = buffer + RECORDSIZE;
	ধ্রুব বর্ণ *comment;
	ssize_t n;
	long len;
	বর্ণ *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	যদি (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	যদি (n < 0)
		die_errno("git get-tar-commit-id: read error");
	যদি (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	যদি (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		প্রত্যাগম 1;

	errno = 0;
	len = strtol(content, &end, 10);
	যদি (errno == ERANGE || end == content || len < 0)
		প্রত্যাগম 1;
	যদি (!skip_prefix(end, " comment=", &comment))
		প্রত্যাগম 1;
	len -= comment - content;
	যদি (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		প্রত্যাগম 1;

	যদি (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	প্রত্যাগম 0;
}
