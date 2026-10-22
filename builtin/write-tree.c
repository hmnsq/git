/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "tree.h"
#شامل "cache-tree.h"
#شامل "parse-options.h"

ساکن ثابت حرف * ثابت write_درخت_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	تہی
};

صحیح cmd_write_درخت(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *cmd_سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	صحیح flags = 0, ret;
	ثابت حرف *درخت_سابقہ = تہی;
	ساخت object_id oid;
	ثابت حرف *me = "git-write-tree";
	ساخت option write_درخت_options[] = {
		OPT_ثنائی_ہندسہ(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_درخت_MISSING_OK),
		OPT_STRING(0, "prefix", &درخت_سابقہ, N_("<prefix>/"),
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
		OPT_انتہا()
	};

	repo_config(the_repository, git_default_config, تہی);
	argc = parse_options(argc, argv, cmd_سابقہ, write_درخت_options,
			     write_درخت_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	ret = write_اشاریہ_as_درخت(&oid, the_repository->اشاریہ,
				  repo_get_اشاریہ_file(the_repository),
				  flags, درخت_سابقہ);
	انتخاب (ret) {
	حالت 0:
		لکھو("%s\n", oid_مقصد_hex(&oid));
		توڑو;
	حالت WRITE_درخت_UNREADABLE_اشاریہ:
		die("%s: error reading the index", me);
		توڑو;
	حالت WRITE_درخت_UNMERGED_اشاریہ:
		die("%s: error building trees", me);
		توڑو;
	حالت WRITE_درخت_سابقہ_ERROR:
		die("%s: prefix %s not found", me, درخت_سابقہ);
		توڑو;
	}
	واپس ret;
}
