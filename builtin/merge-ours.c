/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#অন্তর্ভাব "git-compat-util.h"
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "diff.h"

স্থির ধ্রুবক অক্ষর builtin_merge_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

পূর্ণসংখ্যা cmd_merge_ours(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *prefix UNUSED,
		   কাঠামো repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_merge_ours_usage);

	repo_config(repo, git_default_config, ফাঁকা);
	prepare_repo_settings(repo);
	repo->settings.command_requires_full_সূচক = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	যদি (repo_read_সূচক(repo) < 0)
		die_errno("read_cache failed");
	যদি (সূচক_differs_from(repo, "HEAD", ফাঁকা, 0))
		ফেরত 2;
	ফেরত 0;
}
