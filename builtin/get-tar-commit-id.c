/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "tar.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

ಪೂರ್ಣಾಙ್ಕ cmd_get_tar_commit_id(ಪೂರ್ಣಾಙ್ಕ argc,
			  ಧ್ರುವ ವರ್ಣ **argv,
			  ಧ್ರುವ ವರ್ಣ *prefix,
			  ಸಂರಚನಾ repository *repo UNUSED)
{
	ವರ್ಣ buffer[HEADERSIZE];
	ಸಂರಚನಾ ustar_header *header = (ಸಂರಚನಾ ustar_header *)buffer;
	ವರ್ಣ *content = buffer + RECORDSIZE;
	ಧ್ರುವ ವರ್ಣ *comment;
	ssize_t n;
	long len;
	ವರ್ಣ *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	ಯದಿ (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	ಯದಿ (n < 0)
		die_errno("git get-tar-commit-id: read error");
	ಯದಿ (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	ಯದಿ (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		ಪ್ರತ್ಯಾಗಮ 1;

	errno = 0;
	len = strtol(content, &end, 10);
	ಯದಿ (errno == ERANGE || end == content || len < 0)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಯದಿ (!skip_prefix(end, " comment=", &comment))
		ಪ್ರತ್ಯಾಗಮ 1;
	len -= comment - content;
	ಯದಿ (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		ಪ್ರತ್ಯಾಗಮ 1;

	ಯದಿ (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	ಪ್ರತ್ಯಾಗಮ 0;
}
