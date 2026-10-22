/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#અન્તર્ભાવ "git-compat-util.h"
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "diff.h"

સ્થિર અચલ અક્ષર builtin_merge_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

પૂર્ણાંક cmd_merge_ours(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix UNUSED,
		   સંરચના repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_merge_ours_usage);

	repo_config(repo, git_default_config, ખાલી);
	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	જો (repo_read_index(repo) < 0)
		die_errno("read_cache failed");
	જો (index_differs_from(repo, "HEAD", ખાલી, 0))
		પાછા 2;
	પાછા 0;
}
