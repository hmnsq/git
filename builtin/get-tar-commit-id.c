/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tar.h"

स्थिर अचर वर्ण builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

पूर्णाङ्क cmd_get_tar_commit_id(पूर्णाङ्क argc,
			  अचर वर्ण **argv,
			  अचर वर्ण *prefix,
			  संरचना repository *repo UNUSED)
{
	वर्ण buffer[HEADERSIZE];
	संरचना ustar_header *header = (संरचना ustar_header *)buffer;
	वर्ण *content = buffer + RECORDSIZE;
	अचर वर्ण *comment;
	ssize_t n;
	दीर्घ len;
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
		फर्काऊ 1;

	त्रुटि_सं = 0;
	len = म_से_दीर्घ(content, &end, 10);
	यदि (त्रुटि_सं == दुस्फल || end == content || len < 0)
		फर्काऊ 1;
	यदि (!skip_prefix(end, " comment=", &comment))
		फर्काऊ 1;
	len -= comment - content;
	यदि (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		फर्काऊ 1;

	यदि (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	फर्काऊ 0;
}
