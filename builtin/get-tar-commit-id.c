/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "tar.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#ವ್ಯಾಖ್ಯಾನಿಸು RECORDSIZE	(512)
#ವ್ಯಾಖ್ಯಾನಿಸು HEADERSIZE (2 * RECORDSIZE)

ಪೂರ್ಣಾಂಕ cmd_get_tar_commit_id(ಪೂರ್ಣಾಂಕ argc,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			  ರಚನೆ repository *repo ಬಳಸದ)
{
	ಅಕ್ಷರ ಬಫರ್[HEADERSIZE];
	ರಚನೆ ustar_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ = (ರಚನೆ ustar_ಶೀರ್ಷಿಕೆ *)ಬಫರ್;
	ಅಕ್ಷರ *content = ಬಫರ್ + RECORDSIZE;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *comment;
	ssize_t n;
	ದೀರ್ಘ len;
	ಅಕ್ಷರ *ಕೊನೆ;

	BUG_ON_NON_ಖಾಲಿಗಳು_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	ಆದರೆ (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_ಒಳ_ತುಂಬಿದ(0, ಬಫರ್, HEADERSIZE);
	ಆದರೆ (n < 0)
		die_errno("git get-tar-commit-id: read error");
	ಆದರೆ (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	ಆದರೆ (ಶೀರ್ಷಿಕೆ->typeflag[0] != TYPEFLAG_GLOBAL_ಶೀರ್ಷಿಕೆ)
		ಹಿಂತಿರುಗಿ 1;

	errno = 0;
	len = strtol(content, &ಕೊನೆ, 10);
	ಆದರೆ (errno == ERANGE || ಕೊನೆ == content || len < 0)
		ಹಿಂತಿರುಗಿ 1;
	ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ಕೊನೆ, " comment=", &comment))
		ಹಿಂತಿರುಗಿ 1;
	len -= comment - content;
	ಆದರೆ (len < 1 || !(len % 2) ||
	    hash_algo_by_ಉದ್ದಳತೆ((len - 1) / 2) == GIT_HASH_UNKNOWN)
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (write_ಒಳ_ತುಂಬಿದ(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	ಹಿಂತಿರುಗಿ 0;
}
