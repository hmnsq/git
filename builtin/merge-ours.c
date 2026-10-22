/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#ضمن "git-compat-util.h"
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "diff.h"

ساكن ثابت حرف builtin_merge_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

صحيح cmd_merge_ours(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *prefix UNUSED,
		   بنية repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_merge_ours_usage);

	repo_config(repo, git_default_config, عدم);
	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	إذا (repo_read_index(repo) < 0)
		die_errno("read_cache failed");
	إذا (index_differs_from(repo, "HEAD", عدم, 0))
		أرجع 2;
	أرجع 0;
}
