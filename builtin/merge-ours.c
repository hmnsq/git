/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "git-compat-util.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ builtin_ਮਿਲਾਓ_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

ਪੂਰਨ cmd_ਮਿਲਾਓ_ours(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
		   ਬਣਤਰ repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_ਮਿਲਾਓ_ours_usage);

	repo_config(repo, git_default_config, ਨਿਰਾ);
	prepare_repo_settings(repo);
	repo->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(repo) < 0)
		die_errno("read_cache failed");
	ਜੇ (ਸੂਚਕਾਂਕ_differs_from(repo, "HEAD", ਨਿਰਾ, 0))
		ਵਾਪਸ 2;
	ਵਾਪਸ 0;
}
