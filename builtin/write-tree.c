/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "tree.h"
#ضمن "cache-tree.h"
#ضمن "parse-options.h"

ساكن ثابت حرف * ثابت write_ڤوکوق_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	عدم
};

صحيح cmd_write_ڤوکوق(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *cmd_prefix,
		   بنية repository *repo UNUSED)
{
	صحيح flags = 0, ret;
	ثابت حرف *ڤوکوق_prefix = عدم;
	بنية object_id oid;
	ثابت حرف *me = "git-write-tree";
	بنية option write_ڤوکوق_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_ڤوکوق_MISSING_OK),
		OPT_STRING(0, "prefix", &ڤوکوق_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.نيلاي = &flags,
			.precision = حجم(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_ڤوکوق_IGNORE_CACHE_ڤوکوق,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, cmd_prefix, write_ڤوکوق_options,
			     write_ڤوکوق_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_ڤوکوق(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, ڤوکوق_prefix);
	بدل (ret) {
	حالة 0:
		printf("%s\n", oid_to_hex(&oid));
		اكسر;
	حالة WRITE_ڤوکوق_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		اكسر;
	حالة WRITE_ڤوکوق_UNMERGED_INDEX:
		die("%s: error building trees", me);
		اكسر;
	حالة WRITE_ڤوکوق_PREFIX_ERROR:
		die("%s: prefix %s not found", me, ڤوکوق_prefix);
		اكسر;
	}
	أرجع ret;
}
