/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#ଅନ୍ତର୍ଭାଵ "git-compat-util.h"
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର builtin_merge_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_merge_ours(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		   ଅଚଳ ଅକ୍ଷର **argv,
		   ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
		   ସଂରଚନା repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_merge_ours_usage);

	repo_config(repo, git_default_config, ଖାଲି);
	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	ଯଦି (repo_read_index(repo) < 0)
		die_errno("read_cache failed");
	ଯଦି (index_differs_from(repo, "HEAD", ଖାଲି, 0))
		ଫେରନ୍ତୁ 2;
	ଫେରନ୍ତୁ 0;
}
