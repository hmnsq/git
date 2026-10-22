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

static const char * const write_पुनो_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	NULL
};

int cmd_write_पुनो(int argc,
		   const char **argv,
		   const char *cmd_prefix,
		   struct repository *repo UNUSED)
{
	int flags = 0, ret;
	const char *पुनो_prefix = NULL;
	struct object_id oid;
	const char *me = "git-write-tree";
	struct option write_पुनो_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_पुनो_MISSING_OK),
		OPT_STRING(0, "prefix", &पुनो_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.हालागा = &flags,
			.precision = sizeof(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_पुनो_IGNORE_CACHE_पुनो,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, NULL);
	argc = parse_options(argc, argv, cmd_prefix, write_पुनो_options,
			     write_पुनो_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_पुनो(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, पुनो_prefix);
	switch (ret) {
	case 0:
		printf("%s\n", oid_to_hex(&oid));
		break;
	case WRITE_पुनो_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		break;
	case WRITE_पुनो_UNMERGED_INDEX:
		die("%s: error building trees", me);
		break;
	case WRITE_पुनो_PREFIX_ERROR:
		die("%s: prefix %s not found", me, पुनो_prefix);
		break;
	}
	return ret;
}
