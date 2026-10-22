/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "tree.h"
#ضمن "cache-tree.h"
#ضمن "parse-options.h"

ساكن ثابت حرف * ثابت write_وڻ_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	عدم
};

صحيح cmd_write_وڻ(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *cmd_اڳياڙي,
		   بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح flags = 0, ret;
	ثابت حرف *وڻ_اڳياڙي = عدم;
	بنية object_id oid;
	ثابت حرف *me = "git-write-tree";
	بنية option write_وڻ_options[] = {
		OPT_ٻٽو_انگ(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_وڻ_MISSING_OK),
		OPT_STRING(0, "prefix", &وڻ_اڳياڙي, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_ٻٽو_انگ,
			.long_name = "ignore-cache-tree",
			.قدر = &flags,
			.precision = حجم(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_وڻ_IGNORE_CACHE_وڻ,
		},
		OPT_پڇاڙي()
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, cmd_اڳياڙي, write_وڻ_options,
			     write_وڻ_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ڀريل_اشاريو = 0;

	ret = write_اشاريو_as_وڻ(&oid, the_repository->اشاريو,
				  repo_get_اشاريو_file(the_repository),
				  flags, وڻ_اڳياڙي);
	بدل (ret) {
	حالة 0:
		لکو("%s\n", oid_منزل_hex(&oid));
		اكسر;
	حالة WRITE_وڻ_UNREADABLE_اشاريو:
		die("%s: error reading the index", me);
		اكسر;
	حالة WRITE_وڻ_UNMERGED_اشاريو:
		die("%s: error building trees", me);
		اكسر;
	حالة WRITE_وڻ_اڳياڙي_ERROR:
		die("%s: prefix %s not found", me, وڻ_اڳياڙي);
		اكسر;
	}
	أرجع ret;
}
