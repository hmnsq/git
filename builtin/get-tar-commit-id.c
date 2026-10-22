/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#ضمن "builtin.h"
#ضمن "commit.h"
#ضمن "tar.h"

ساكن ثابت حرف builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#تعریف_کړه RECORDSIZE	(512)
#تعریف_کړه HEADERSIZE (2 * RECORDSIZE)

صحيح cmd_get_tar_commit_id(صحيح argc,
			  ثابت حرف **argv,
			  ثابت حرف *مختاړی,
			  بنية repository *repo ناکارول_شوی)
{
	حرف بفر[HEADERSIZE];
	بنية ustar_سرلیک *سرلیک = (بنية ustar_سرلیک *)بفر;
	حرف *content = بفر + RECORDSIZE;
	ثابت حرف *comment;
	ssize_t n;
	اوږد len;
	حرف *پای_ټکی;

	BUG_ON_NON_تشه_مختاړی(مختاړی);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	إذا (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_دننه_ډک(0, بفر, HEADERSIZE);
	إذا (n < 0)
		die_errno("git get-tar-commit-id: read error");
	إذا (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	إذا (سرلیک->typeflag[0] != TYPEFLAG_GLOBAL_سرلیک)
		أرجع 1;

	errno = 0;
	len = strtol(content, &پای_ټکی, 10);
	إذا (errno == ERANGE || پای_ټکی == content || len < 0)
		أرجع 1;
	إذا (!skip_مختاړی(پای_ټکی, " comment=", &comment))
		أرجع 1;
	len -= comment - content;
	إذا (len < 1 || !(len % 2) ||
	    hash_algo_by_اوږدتیا((len - 1) / 2) == GIT_HASH_UNKNOWN)
		أرجع 1;

	إذا (write_دننه_ډک(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	أرجع 0;
}
