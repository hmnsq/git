/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "tree.h"
#ضمن "cache-tree.h"
#ضمن "parse-options.h"

ساكن ثابت حرف * ثابت write_ونه_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	عدم
};

صحيح cmd_write_ونه(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *cmd_مختاړی,
		   بنية repository *repo ناکارول_شوی)
{
	صحيح flags = 0, ret;
	ثابت حرف *ونه_مختاړی = عدم;
	بنية object_id oid;
	ثابت حرف *me = "git-write-tree";
	بنية option write_ونه_options[] = {
		OPT_بټ(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_ونه_MISSING_OK),
		OPT_STRING(0, "prefix", &ونه_مختاړی, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_بټ,
			.long_name = "ignore-cache-tree",
			.ارزښت = &flags,
			.precision = حجم(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_ونه_IGNORE_CACHE_ونه,
		},
		OPT_پای_ټکی()
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, cmd_مختاړی, write_ونه_options,
			     write_ونه_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ډک_شاخص = 0;

	ret = write_شاخص_as_ونه(&oid, the_repository->شاخص,
				  repo_get_شاخص_file(the_repository),
				  flags, ونه_مختاړی);
	بدل (ret) {
	حالة 0:
		چاپ_کړه("%s\n", oid_منزل_hex(&oid));
		اكسر;
	حالة WRITE_ونه_UNREADABLE_شاخص:
		die("%s: error reading the index", me);
		اكسر;
	حالة WRITE_ونه_UNMERGED_شاخص:
		die("%s: error building trees", me);
		اكسر;
	حالة WRITE_ونه_مختاړی_ERROR:
		die("%s: prefix %s not found", me, ونه_مختاړی);
		اكسر;
	}
	أرجع ret;
}
