/*
 * Copyright (c) 2005, 2006 Rene Scharfe
 */
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "tar.h"

స్థిర స్థిరరాశి అక్షరం builtin_get_tar_commit_id_usage[] =
"git get-tar-commit-id";

/* ustar header + extended global header content */
#నిర్వచించు RECORDSIZE	(512)
#నిర్వచించు HEADERSIZE (2 * RECORDSIZE)

పూర్ణాంకం cmd_get_tar_commit_id(పూర్ణాంకం argc,
			  స్థిరరాశి అక్షరం **argv,
			  స్థిరరాశి అక్షరం *ఉపసర్గ,
			  నిర్మాణం repository *repo వాడని)
{
	అక్షరం బఫరు[HEADERSIZE];
	నిర్మాణం ustar_శీర్షిక *శీర్షిక = (నిర్మాణం ustar_శీర్షిక *)బఫరు;
	అక్షరం *content = బఫరు + RECORDSIZE;
	స్థిరరాశి అక్షరం *comment;
	ssize_t n;
	దీర్ఘ len;
	అక్షరం *చివర;

	BUG_ON_NON_ఖాళీలు_ఉపసర్గ(ఉపసర్గ);

	show_usage_if_asked(argc, argv, builtin_get_tar_commit_id_usage);

	ఒకవేళ (argc != 1)
		usage(builtin_get_tar_commit_id_usage);

	n = read_లోపల_నిండిన(0, బఫరు, HEADERSIZE);
	ఒకవేళ (n < 0)
		die_errno("git get-tar-commit-id: read error");
	ఒకవేళ (n != HEADERSIZE)
		die_errno("git get-tar-commit-id: EOF before reading tar header");
	ఒకవేళ (శీర్షిక->typeflag[0] != TYPEFLAG_GLOBAL_శీర్షిక)
		తిరిగి 1;

	errno = 0;
	len = strtol(content, &చివర, 10);
	ఒకవేళ (errno == ERANGE || చివర == content || len < 0)
		తిరిగి 1;
	ఒకవేళ (!skip_ఉపసర్గ(చివర, " comment=", &comment))
		తిరిగి 1;
	len -= comment - content;
	ఒకవేళ (len < 1 || !(len % 2) ||
	    hash_algo_by_నిడివి((len - 1) / 2) == GIT_HASH_UNKNOWN)
		తిరిగి 1;

	ఒకవేళ (write_లోపల_నిండిన(1, comment, len) < 0)
		die_errno("git get-tar-commit-id: write error");

	తిరిగి 0;
}
