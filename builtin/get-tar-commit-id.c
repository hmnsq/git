/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#ضمن "builtin.h"
#ضمن "commit.h"
#ضمن "tar.h"

ساكن ثابت حرف builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#وصف_ڪريو RECORDSIZE	(512)
#وصف_ڪريو HEADERSIZE (2 * RECORDSIZE)

صحيح cmd_get_tar_commit_id(صحيح argc,
			  ثابت حرف **argv,
			  ثابت حرف *اڳياڙي,
			  بنية repository *repo اڻ_استعمال_ٿيل)
{
	حرف بفر[HEADERSIZE];
	بنية ustar_سرو *سرو = (بنية ustar_سرو *)بفر;
	حرف *content = بفر + RECORDSIZE;
	ثابت حرف *comment;
	ssize_t n;
	ڊگهو len;
	حرف *پڇاڙي;

	BUG_ON_NON_خالي_جايون_اڳياڙي(اڳياڙي);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	إذا (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_اندريون_ڀريل(0, بفر, HEADERSIZE);
	إذا (n < 0)
		die_errno("git get-tar-commit-id: read error");
	إذا (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	إذا (سرو->typeflag[0] != TYPEFLAG_GLOBAL_سرو)
		أرجع 1;

	errno = 0;
	len = strtol(content, &پڇاڙي, 10);
	إذا (errno == ERANGE || پڇاڙي == content || len < 0)
		أرجع 1;
	إذا (!skip_اڳياڙي(پڇاڙي, " comment=", &comment))
		أرجع 1;
	len -= comment - content;
	إذا (len < 1 || !(len % 2) ||
	    hash_algo_by_ڊيگهائي((len - 1) / 2) == GIT_HASH_UNKNOWN)
		أرجع 1;

	إذا (write_اندريون_ڀريل(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	أرجع 0;
}
