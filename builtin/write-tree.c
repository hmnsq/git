/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "tree.h"
#شامل "cache-tree.h"
#شامل "parse-options.h"

ایستا ثابت نویسه * ثابت write_درخت_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	پوچ
};

صحیح cmd_write_درخت(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *cmd_prefix,
		   ساختار repository *repo UNUSED)
{
	صحیح flags = 0, ret;
	ثابت نویسه *درخت_prefix = پوچ;
	ساختار object_id oid;
	ثابت نویسه *me = "git-write-tree";
	ساختار option write_درخت_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_درخت_MISSING_OK),
		OPT_STRING(0, "prefix", &درخت_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.مقدار = &flags,
			.precision = اندازه(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_درخت_IGNORE_CACHE_درخت,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, پوچ);
	argc = parse_options(argc, argv, cmd_prefix, write_درخت_options,
			     write_درخت_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_درخت(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, درخت_prefix);
	انتخاب (ret) {
	حالت 0:
		printf("%s\n", oid_to_hex(&oid));
		بشکن;
	حالت WRITE_درخت_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		بشکن;
	حالت WRITE_درخت_UNMERGED_INDEX:
		die("%s: error building trees", me);
		بشکن;
	حالت WRITE_درخت_PREFIX_ERROR:
		die("%s: prefix %s not found", me, درخت_prefix);
		بشکن;
	}
	بازگشت ret;
}
