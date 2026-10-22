/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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
		   ثابت حرف *cmd_اگیتر,
		   بنية repository *repo بن_ورتیا)
{
	صحيح flags = 0, ret;
	ثابت حرف *درخت_اگیتر = عدم;
	بنية object_id oid;
	ثابت حرف *me = "git-write-tree";
	بنية option write_درخت_options[] = {
		OPT_ثنائی_ہندسہ(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_درخت_MISSING_OK),
		OPT_STRING(0, "prefix", &درخت_اگیتر, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_ثنائی_ہندسہ,
			.long_name = "ignore-cache-tree",
			.قدر = &flags,
			.precision = حجم(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_درخت_IGNORE_CACHE_درخت,
		},
		OPT_سرا()
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, cmd_اگیتر, write_درخت_options,
			     write_درخت_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھریا_اشاریہ = 0;

	ret = write_اشاریہ_as_درخت(&oid, the_repository->اشاریہ,
				  repo_get_اشاریہ_file(the_repository),
				  flags, درخت_اگیتر);
	بدل (ret) {
	حالة 0:
		لکھو("%s\n", oid_منزل_hex(&oid));
		اكسر;
	حالة WRITE_درخت_UNREADABLE_اشاریہ:
		die("%s: error reading the index", me);
		اكسر;
	حالة WRITE_درخت_UNMERGED_اشاریہ:
		die("%s: error building trees", me);
		اكسر;
	حالة WRITE_درخت_اگیتر_ERROR:
		die("%s: prefix %s not found", me, درخت_اگیتر);
		اكسر;
	}
	أرجع ret;
}
