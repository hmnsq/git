/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#अन्तर्भाव "git-compat-util.h"
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "diff.h"

स्थिर ध्रुव वर्ण builtin_merge_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

पूर्णाङ्क cmd_merge_ours(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *prefix UNUSED,
		   संरचना repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_merge_ours_usage);

	repo_config(repo, git_default_config, रिक्त);
	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	जइ (repo_read_index(repo) < 0)
		die_errno("read_cache failed");
	जइ (index_differs_from(repo, "HEAD", रिक्त, 0))
		प्रत्यागम 2;
	प्रत्यागम 0;
}
