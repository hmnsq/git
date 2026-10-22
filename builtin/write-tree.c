/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "tree.h"
#include "cache-tree.h"
#include "parse-options.h"

static const var_na * const write_tree_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	NULL
};

จำนวนเต็ม cmd_write_tree(จำนวนเต็ม argc,
		   const var_na **argv,
		   const var_na *cmd_prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	จำนวนเต็ม flags = 0, ret;
	const var_na *tree_prefix = NULL;
	sa_nracanaa object_id oid;
	const var_na *me = "git-write-tree";
	sa_nracanaa option write_tree_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_TREE_MISSING_OK),
		OPT_STRING(0, "prefix", &tree_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.value = &flags,
			.precision = parimaa_na(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_TREE_IGNORE_CACHE_TREE,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, NULL);
	argc = parse_options(argc, argv, cmd_prefix, write_tree_options,
			     write_tree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_tree(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, tree_prefix);
	switch (ret) {
	pak_sa 0:
		printf("%s\n", oid_to_hex(&oid));
		bha_nga;
	pak_sa WRITE_TREE_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		bha_nga;
	pak_sa WRITE_TREE_UNMERGED_INDEX:
		die("%s: error building trees", me);
		bha_nga;
	pak_sa WRITE_TREE_PREFIX_ERROR:
		die("%s: prefix %s not found", me, tree_prefix);
		bha_nga;
	}
	คืน ret;
}
