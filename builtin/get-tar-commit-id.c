/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "tar.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_get_tar_commit_id(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			  ଅଚଳ ଅକ୍ଷର **argv,
			  ଅଚଳ ଅକ୍ଷର *prefix,
			  ସଂରଚନା repository *repo UNUSED)
{
	ଅକ୍ଷର buffer[HEADERSIZE];
	ସଂରଚନା ustar_header *header = (ସଂରଚନା ustar_header *)buffer;
	ଅକ୍ଷର *content = buffer + RECORDSIZE;
	ଅଚଳ ଅକ୍ଷର *comment;
	ssize_t n;
	long len;
	ଅକ୍ଷର *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	ଯଦି (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	ଯଦି (n < 0)
		die_errno("git get-tar-commit-id: read error");
	ଯଦି (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	ଯଦି (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		ଫେରନ୍ତୁ 1;

	errno = 0;
	len = strtol(content, &end, 10);
	ଯଦି (errno == ERANGE || end == content || len < 0)
		ଫେରନ୍ତୁ 1;
	ଯଦି (!skip_prefix(end, " comment=", &comment))
		ଫେରନ୍ତୁ 1;
	len -= comment - content;
	ଯଦି (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		ଫେରନ୍ତୁ 1;

	ଯଦି (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	ଫେରନ୍ତୁ 0;
}
