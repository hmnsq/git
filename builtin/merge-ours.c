/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#অন্তর্ভাৱ "git-compat-util.h"
#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "diff.h"

স্থির ধ্রুৱ ৱর্ণ builtin_merge_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

পূর্ণাঙ্ক cmd_merge_ours(পূর্ণাঙ্ক argc,
		   ধ্রুৱ ৱর্ণ **argv,
		   ধ্রুৱ ৱর্ণ *prefix UNUSED,
		   সংরচনা repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_merge_ours_usage);

	repo_config(repo, git_default_config, রিক্ত);
	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	যদি (repo_read_index(repo) < 0)
		die_errno("read_cache failed");
	যদি (index_differs_from(repo, "HEAD", রিক্ত, 0))
		ঘূৰাই 2;
	ঘূৰাই 0;
}
