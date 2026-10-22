/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#ಅನ್ತರ್ಭಾವ "git-compat-util.h"
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "diff.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ builtin_merge_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

ಪೂರ್ಣಾಙ್ಕ cmd_merge_ours(ಪೂರ್ಣಾಙ್ಕ argc,
		   ಧ್ರುವ ವರ್ಣ **argv,
		   ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
		   ಸಂರಚನಾ repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_merge_ours_usage);

	repo_config(repo, git_default_config, ರಿಕ್ತ);
	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	ಯದಿ (repo_read_index(repo) < 0)
		die_errno("read_cache failed");
	ಯದಿ (index_differs_from(repo, "HEAD", ರಿಕ್ತ, 0))
		ಪ್ರತ್ಯಾಗಮ 2;
	ಪ್ರತ್ಯಾಗಮ 0;
}
