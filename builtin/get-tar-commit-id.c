/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#समावेश "builtin.h"
#समावेश "commit.h"
#समावेश "tar.h"

अटल स्थिर अक्षर builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#घोषणा RECORDSIZE	(512)
#घोषणा HEADERSIZE (2 * RECORDSIZE)

पूर्णांक cmd_get_tar_commit_id(पूर्णांक argc,
			  स्थिर अक्षर **argv,
			  स्थिर अक्षर *prefix,
			  काष्ठा repository *repo UNUSED)
{
	अक्षर बफर[HEADERSIZE];
	काष्ठा ustar_header *header = (काष्ठा ustar_header *)बफर;
	अक्षर *content = बफर + RECORDSIZE;
	स्थिर अक्षर *comment;
	ssize_t n;
	दीर्घ len;
	अक्षर *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	अगर (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, बफर, HEADERSIZE);
	अगर (n < 0)
		die_errno("git get-tar-commit-id: read error");
	अगर (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	अगर (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		वापस 1;

	त्रुटि_सं = 0;
	len = म_से_दीर्घ(content, &end, 10);
	अगर (त्रुटि_सं == दुस्फल || end == content || len < 0)
		वापस 1;
	अगर (!skip_prefix(end, " comment=", &comment))
		वापस 1;
	len -= comment - content;
	अगर (len < 1 || !(len % 2) ||
	    hash_algo_by_लंबाई((len - 1) / 2) == GIT_HASH_UNKNOWN)
		वापस 1;

	अगर (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	वापस 0;
}
