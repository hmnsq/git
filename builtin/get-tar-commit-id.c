/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "tar.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#define RECORDSIZE	(512)
#define HEADERSIZE (2 * RECORDSIZE)

പൂർണ്ണസംഖ്യ cmd_get_tar_commit_id(പൂർണ്ണസംഖ്യ argc,
			  സ്ഥിരം അക്ഷരം **argv,
			  സ്ഥിരം അക്ഷരം *prefix,
			  ഘടന repository *repo UNUSED)
{
	അക്ഷരം buffer[HEADERSIZE];
	ഘടന ustar_header *header = (ഘടന ustar_header *)buffer;
	അക്ഷരം *content = buffer + RECORDSIZE;
	സ്ഥിരം അക്ഷരം *comment;
	ssize_t n;
	long len;
	അക്ഷരം *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	എങ്കിൽ (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_in_full(0, buffer, HEADERSIZE);
	എങ്കിൽ (n < 0)
		die_errno("git get-tar-commit-id: read error");
	എങ്കിൽ (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	എങ്കിൽ (header->typeflag[0] != TYPEFLAG_GLOBAL_HEADER)
		തിരികെ 1;

	errno = 0;
	len = strtol(content, &end, 10);
	എങ്കിൽ (errno == ERANGE || end == content || len < 0)
		തിരികെ 1;
	എങ്കിൽ (!skip_prefix(end, " comment=", &comment))
		തിരികെ 1;
	len -= comment - content;
	എങ്കിൽ (len < 1 || !(len % 2) ||
	    hash_algo_by_length((len - 1) / 2) == GIT_HASH_UNKNOWN)
		തിരികെ 1;

	എങ്കിൽ (write_in_full(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	തിരികെ 0;
}
