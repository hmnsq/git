/*
 * Implementation of git-merge-ours.sh as builtin
 *
 * Copyright (c) 2007 Thomas Harning Jr
 * Original:
 * Original Copyright (c) 2005 Junio C Hamano
 *
 * Pretend we resolved the heads, but declare our tree trumps everybody else.
 */

#समावेश "git-compat-util.h"
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "diff.h"

अटल स्थिर अक्षर builtin_merge_ours_usage[] =
	"git merge-ours <base>... -- HEAD <remote>...";

पूर्णांक cmd_merge_ours(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *prefix UNUSED,
		   काष्ठा repository *repo)
{
	show_usage_if_asked(argc, argv, builtin_merge_ours_usage);

	repo_config(repo, git_default_config, शून्य);
	prepare_repo_settings(repo);
	repo->settings.command_requires_full_सूचकांक = 0;

	/*
	 * The contents of the current index becomes the tree we
	 * commit.  The index must match HEAD, or this merge cannot go
	 * through.
	 */
	अगर (repo_read_सूचकांक(repo) < 0)
		die_errno("read_cache failed");
	अगर (सूचकांक_differs_from(repo, "HEAD", शून्य, 0))
		वापस 2;
	वापस 0;
}
