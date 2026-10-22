/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#அந்தர்பாவ "git-compat-util.h"
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "diff.h"

நிலையான மாறிலி எழுத்து builtin_merge_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

முழு cmd_merge_ours(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix UNUSED,
		   கட்டமைப்பு repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_merge_ours_usage);

	repo_config(repo, git_default_config, வெற்று);
	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	எனில் (repo_read_index(repo) < 0)
		die_errno("read_cache failed");
	எனில் (index_differs_from(repo, "HEAD", வெற்று, 0))
		திரும்பு 2;
	திரும்பு 0;
}
