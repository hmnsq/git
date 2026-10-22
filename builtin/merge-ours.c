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

ساکن ثابت حرف builtin_ملاؤ_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

صحیح cmd_ملاؤ_ours(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ غیر_مستعمل,
		   ساخت repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_ملاؤ_ours_usage);

	repo_config(repo, git_default_config, تہی);
	prepare_repo_settings(repo);
	repo->settings.command_requires_بھرا_اشاریہ = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	اگر (repo_read_اشاریہ(repo) < 0)
		die_errno("read_cache failed");
	اگر (اشاریہ_differs_from(repo, "HEAD", تہی, 0))
		واپس 2;
	واپس 0;
}
