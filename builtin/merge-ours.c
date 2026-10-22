/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#شامل "git-compat-util.h"
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "diff.h"

ایستا ثابت نویسه builtin_merge_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

صحیح cmd_merge_ours(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix UNUSED,
		   ساختار repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_merge_ours_usage);

	repo_config(repo, git_default_config, پوچ);
	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	اگر (repo_read_index(repo) < 0)
		die_errno("read_cache failed");
	اگر (index_differs_from(repo, "HEAD", پوچ, 0))
		بازگشت 2;
	بازگشت 0;
}
