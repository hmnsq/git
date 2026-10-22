/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#അന്തര്ഭാവ "git-compat-util.h"
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "diff.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം builtin_merge_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

പൂർണ്ണസംഖ്യ cmd_merge_ours(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *prefix UNUSED,
		   ഘടന repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_merge_ours_usage);

	repo_config(repo, git_default_config, ശൂന്യ);
	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	എങ്കിൽ (repo_read_index(repo) < 0)
		die_errno("read_cache failed");
	എങ്കിൽ (index_differs_from(repo, "HEAD", ശൂന്യ, 0))
		തിരികെ 2;
	തിരികെ 0;
}
