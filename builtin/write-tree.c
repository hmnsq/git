/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "tree.h"
#ضمن "cache-tree.h"
#ضمن "parse-options.h"

ساكن ثابت حرف * ثابت write_درخت_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	عدم
};

صحيح cmd_write_درخت(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *cmd_پیشوند,
		   بنية repository *repo استفادهنشده)
{
	صحيح flags = 0, ret;
	ثابت حرف *درخت_پیشوند = عدم;
	بنية object_id oid;
	ثابت حرف *me = "git-write-tree";
	بنية option write_درخت_options[] = {
		OPT_بیت(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_درخت_MISSING_OK),
		OPT_STRING(0, "prefix", &درخت_پیشوند, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_بیت,
			.long_name = "ignore-cache-tree",
			.مقدار = &flags,
			.precision = حجم(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_درخت_IGNORE_CACHE_درخت,
		},
		OPT_انتها()
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, cmd_پیشوند, write_درخت_options,
			     write_درخت_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_پر_شاخص = 0;

	ret = write_شاخص_as_درخت(&oid, the_repository->شاخص,
				  repo_get_شاخص_file(the_repository),
				  flags, درخت_پیشوند);
	بدل (ret) {
	حالة 0:
		چاپ_کن("%s\n", oid_مقصد_hex(&oid));
		اكسر;
	حالة WRITE_درخت_UNREADABLE_شاخص:
		die("%s: error reading the index", me);
		اكسر;
	حالة WRITE_درخت_UNMERGED_شاخص:
		die("%s: error building trees", me);
		اكسر;
	حالة WRITE_درخت_پیشوند_ERROR:
		die("%s: prefix %s not found", me, درخت_پیشوند);
		اكسر;
	}
	أرجع ret;
}
