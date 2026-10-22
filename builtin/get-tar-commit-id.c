/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tar.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RECORDSIZE	(512)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HEADERSIZE (2 * RECORDSIZE)

ਪੂਰਨ cmd_get_tar_commit_id(ਪੂਰਨ argc,
			  ਅਚਲ ਅੱਖਰ **argv,
			  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅੱਖਰ ਬਫਰ[HEADERSIZE];
	ਬਣਤਰ ustar_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ = (ਬਣਤਰ ustar_ਸਿਰਲੇਖ *)ਬਫਰ;
	ਅੱਖਰ *content = ਬਫਰ + RECORDSIZE;
	ਅਚਲ ਅੱਖਰ *comment;
	ssize_t n;
	ਲੰਮਾ len;
	ਅੱਖਰ *ਸਿਰਾ;

	BUG_ON_NON_ਖਾਲੀ_ਥਾਵਾਂ_ਅਗੇਤਰ(ਅਗੇਤਰ);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	ਜੇ (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_ਵਿੱਚ_ਭਰਿਆ(0, ਬਫਰ, HEADERSIZE);
	ਜੇ (n < 0)
		die_errno("git get-tar-commit-id: read error");
	ਜੇ (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	ਜੇ (ਸਿਰਲੇਖ->typeflag[0] != TYPEFLAG_GLOBAL_ਸਿਰਲੇਖ)
		ਵਾਪਸ 1;

	errno = 0;
	len = strtol(content, &ਸਿਰਾ, 10);
	ਜੇ (errno == ERANGE || ਸਿਰਾ == content || len < 0)
		ਵਾਪਸ 1;
	ਜੇ (!skip_ਅਗੇਤਰ(ਸਿਰਾ, " comment=", &comment))
		ਵਾਪਸ 1;
	len -= comment - content;
	ਜੇ (len < 1 || !(len % 2) ||
	    hash_algo_by_ਲੰਮਾਈ((len - 1) / 2) == GIT_HASH_UNKNOWN)
		ਵਾਪਸ 1;

	ਜੇ (write_ਵਿੱਚ_ਭਰਿਆ(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	ਵਾਪਸ 0;
}
