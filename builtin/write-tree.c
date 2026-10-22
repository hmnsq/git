/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "cache-tree.h"
#അന്തര്ഭാവ "parse-options.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം write_വൃക്ഷം_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	ശൂന്യ
};

പൂർണ്ണസംഖ്യ cmd_write_വൃക്ഷം(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *cmd_prefix,
		   ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ flags = 0, ret;
	സ്ഥിരം അക്ഷരം *വൃക്ഷം_prefix = ശൂന്യ;
	ഘടന object_id oid;
	സ്ഥിരം അക്ഷരം *me = "git-write-tree";
	ഘടന option write_വൃക്ഷം_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_വൃക്ഷം_MISSING_OK),
		OPT_STRING(0, "prefix", &വൃക്ഷം_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.മൂല്യം = &flags,
			.precision = വലുപ്പം(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_വൃക്ഷം_IGNORE_CACHE_വൃക്ഷം,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, ശൂന്യ);
	argc = parse_options(argc, argv, cmd_prefix, write_വൃക്ഷം_options,
			     write_വൃക്ഷം_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_വൃക്ഷം(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, വൃക്ഷം_prefix);
	തിരഞ്ഞെടുപ്പ് (ret) {
	അവസ്ഥ 0:
		printf("%s\n", oid_to_hex(&oid));
		നിർത്തുക;
	അവസ്ഥ WRITE_വൃക്ഷം_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		നിർത്തുക;
	അവസ്ഥ WRITE_വൃക്ഷം_UNMERGED_INDEX:
		die("%s: error building trees", me);
		നിർത്തുക;
	അവസ്ഥ WRITE_വൃക്ഷം_PREFIX_ERROR:
		die("%s: prefix %s not found", me, വൃക്ഷം_prefix);
		നിർത്തുക;
	}
	തിരികെ ret;
}
