/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#అన్తర్భావ "git-compat-util.h"
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "diff.h"

స్థిర స్థిరరాశి అక్షరం builtin_విలీనం_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

పూర్ణాంకం cmd_విలీనం_ours(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
		   నిర్మాణం repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_విలీనం_ours_usage);

	repo_config(repo, git_default_config, ఖాళీ);
	prepare_repo_settings(repo);
	repo->settings.command_requires_నిండిన_సూచీ = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	ఒకవేళ (repo_read_సూచీ(repo) < 0)
		die_errno("read_cache failed");
	ఒకవేళ (సూచీ_differs_from(repo, "HEAD", ఖాళీ, 0))
		తిరిగి 2;
	తిరిగి 0;
}
