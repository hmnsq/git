/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tar.h"

स्थिर ध्रुव वर्ण builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#परिभाषय RECORDSIZE	(512)
#परिभाषय HEADERSIZE (2 * RECORDSIZE)

पूर्णाङ्क cmd_get_tar_commit_id(पूर्णाङ्क argc,
			  ध्रुव वर्ण **argv,
			  ध्रुव वर्ण *उपसर्गः,
			  संरचना repository *repo अप्रयुक्तम्)
{
	वर्ण बफर[HEADERSIZE];
	संरचना ustar_शीर्षकम् *शीर्षकम् = (संरचना ustar_शीर्षकम् *)बफर;
	वर्ण *content = बफर + RECORDSIZE;
	ध्रुव वर्ण *comment;
	ssize_t n;
	दीर्घ len;
	वर्ण *अन्तः;

	BUG_ON_NON_रिक्तानि_उपसर्गः(उपसर्गः);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	यदि (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_अन्तर्_पूर्णम्(0, बफर, HEADERSIZE);
	यदि (n < 0)
		die_errno("git get-tar-commit-id: read error");
	यदि (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	यदि (शीर्षकम्->typeflag[0] != TYPEFLAG_GLOBAL_शीर्षकम्)
		प्रत्यागम 1;

	errno = 0;
	len = strtol(content, &अन्तः, 10);
	यदि (errno == ERANGE || अन्तः == content || len < 0)
		प्रत्यागम 1;
	यदि (!skip_उपसर्गः(अन्तः, " comment=", &comment))
		प्रत्यागम 1;
	len -= comment - content;
	यदि (len < 1 || !(len % 2) ||
	    hash_algo_by_दीर्घता((len - 1) / 2) == GIT_HASH_UNKNOWN)
		प्रत्यागम 1;

	यदि (write_अन्तर्_पूर्णम्(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	प्रत्यागम 0;
}
