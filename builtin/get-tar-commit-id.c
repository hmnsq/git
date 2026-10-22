/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tar.h"

स्थिर ध्रुव वर्ण builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

पूर्णाङ्क cmd_get_tar_commit_id(पूर्णाङ्क argc,
			  ध्रुव वर्ण **argv,
			  ध्रुव वर्ण *prefix,
			  संरचना repository *repo UNUSED)
{
	वर्ण buffer[HEADERSIZE];
	संरचना ustar_header *header = (संरचना ustar_header *)buffer;
	वर्ण *content = buffer + RECORDSIZE;
	ध्रुव वर्ण *comment;
	ssize_t n;
	long len;
	वर्ण *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	यदि (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	यदि (n < 0)
		die_errno("git get-tar-commit-id: read error");
	यदि (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	यदि (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		प्रत्यागम 1;

	errno = 0;
	len = strtol(content, &end, 10);
	यदि (errno == ERANGE || end == content || len < 0)
		प्रत्यागम 1;
	यदि (!skip_prefix(end, " comment=", &comment))
		प्रत्यागम 1;
	len -= comment - content;
	यदि (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		प्रत्यागम 1;

	यदि (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	प्रत्यागम 0;
}
