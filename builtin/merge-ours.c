/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#ಅನ್ತರ್ಭಾವ "git-compat-util.h"
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "diff.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ builtin_ವಿಲೀನ_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

ಪೂರ್ಣಾಂಕ cmd_ವಿಲೀನ_ours(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
		   ರಚನೆ repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_ವಿಲೀನ_ours_usage);

	repo_config(repo, git_default_config, ಖಾಲಿ);
	prepare_repo_settings(repo);
	repo->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(repo) < 0)
		die_errno("read_cache failed");
	ಆದರೆ (ಸೂಚ್ಯಂಕ_differs_from(repo, "HEAD", ಖಾಲಿ, 0))
		ಹಿಂತಿರುಗಿ 2;
	ಹಿಂತಿರುಗಿ 0;
}
