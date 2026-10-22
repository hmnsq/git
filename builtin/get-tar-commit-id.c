/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#ضمن "builtin.h"
#ضمن "commit.h"
#ضمن "tar.h"

ساكن ثابت حرف builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#تعریف_کرو RECORDSIZE	(512)
#تعریف_کرو HEADERSIZE (2 * RECORDSIZE)

صحيح cmd_get_tar_commit_id(صحيح argc,
			  ثابت حرف **argv,
			  ثابت حرف *اگیتر,
			  بنية repository *repo بن_ورتیا)
{
	حرف بفر[HEADERSIZE];
	بنية ustar_سرخی *سرخی = (بنية ustar_سرخی *)بفر;
	حرف *content = بفر + RECORDSIZE;
	ثابت حرف *comment;
	ssize_t n;
	لما len;
	حرف *سرا;

	BUG_ON_NON_خالی_تھاواں_اگیتر(اگیتر);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	إذا (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_وچ_بھریا(0, بفر, HEADERSIZE);
	إذا (n < 0)
		die_errno("git get-tar-commit-id: read error");
	إذا (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	إذا (سرخی->typeflag[0] != TYPEFLAG_GLOBAL_سرخی)
		أرجع 1;

	errno = 0;
	len = strtol(content, &سرا, 10);
	إذا (errno == ERANGE || سرا == content || len < 0)
		أرجع 1;
	إذا (!skip_اگیتر(سرا, " comment=", &comment))
		أرجع 1;
	len -= comment - content;
	إذا (len < 1 || !(len % 2) ||
	    hash_algo_by_لمائی((len - 1) / 2) == GIT_HASH_UNKNOWN)
		أرجع 1;

	إذا (write_وچ_بھریا(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	أرجع 0;
}
