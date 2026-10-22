/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#ضمن "builtin.h"
#ضمن "commit.h"
#ضمن "tar.h"

ساكن ثابت حرف builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#تعریف_کن RECORDSIZE	(512)
#تعریف_کن HEADERSIZE (2 * RECORDSIZE)

صحيح cmd_get_tar_commit_id(صحيح argc,
			  ثابت حرف **argv,
			  ثابت حرف *پیشوند,
			  بنية repository *repo استفادهنشده)
{
	حرف میانگیر[HEADERSIZE];
	بنية ustar_سرآیند *سرآیند = (بنية ustar_سرآیند *)میانگیر;
	حرف *content = میانگیر + RECORDSIZE;
	ثابت حرف *comment;
	ssize_t n;
	دراز len;
	حرف *انتها;

	BUG_ON_NON_خالی_پیشوند(پیشوند);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	إذا (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_درون_پر(0, میانگیر, HEADERSIZE);
	إذا (n < 0)
		die_errno("git get-tar-commit-id: read error");
	إذا (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	إذا (سرآیند->typeflag[0] != TYPEFLAG_GLOBAL_سرآیند)
		أرجع 1;

	errno = 0;
	len = strtol(content, &انتها, 10);
	إذا (errno == ERANGE || انتها == content || len < 0)
		أرجع 1;
	إذا (!skip_پیشوند(انتها, " comment=", &comment))
		أرجع 1;
	len -= comment - content;
	إذا (len < 1 || !(len % 2) ||
	    hash_algo_by_درازی((len - 1) / 2) == GIT_HASH_UNKNOWN)
		أرجع 1;

	إذا (write_درون_پر(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	أرجع 0;
}
