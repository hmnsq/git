/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#شامل "builtin.h"
#شامل "commit.h"
#شامل "tar.h"

ساکن ثابت حرف builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#تعریف_کرو RECORDSIZE	(512)
#تعریف_کرو HEADERSIZE (2 * RECORDSIZE)

صحیح cmd_get_tar_commit_id(صحیح argc,
			  ثابت حرف **argv,
			  ثابت حرف *سابقہ,
			  ساخت repository *repo غیر_مستعمل)
{
	حرف بفر[HEADERSIZE];
	ساخت ustar_سرخی *سرخی = (ساخت ustar_سرخی *)بفر;
	حرف *content = بفر + RECORDSIZE;
	ثابت حرف *comment;
	ssize_t n;
	طویل len;
	حرف *انتہا;

	BUG_ON_NON_خالی_خانے_سابقہ(سابقہ);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	اگر (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_اندر_بھرا(0, بفر, HEADERSIZE);
	اگر (n < 0)
		die_errno("git get-tar-commit-id: read error");
	اگر (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	اگر (سرخی->typeflag[0] != TYPEFLAG_GLOBAL_سرخی)
		واپس 1;

	errno = 0;
	len = strtol(content, &انتہا, 10);
	اگر (errno == ERANGE || انتہا == content || len < 0)
		واپس 1;
	اگر (!skip_سابقہ(انتہا, " comment=", &comment))
		واپس 1;
	len -= comment - content;
	اگر (len < 1 || !(len % 2) ||
	    hash_algo_by_لمبائی((len - 1) / 2) == GIT_HASH_UNKNOWN)
		واپس 1;

	اگر (write_اندر_بھرا(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	واپس 0;
}
