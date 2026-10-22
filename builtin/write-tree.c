/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "tree.h"
#অন্তর্ভাব "cache-tree.h"
#অন্তর্ভাব "parse-options.h"

স্থির ধ্রুব বর্ণ * ধ্রুব write_বৃক্ষ_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	রিক্ত
};

পূর্ণাঙ্ক cmd_write_বৃক্ষ(পূর্ণাঙ্ক argc,
		   ধ্রুব বর্ণ **argv,
		   ধ্রুব বর্ণ *cmd_prefix,
		   সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক flags = 0, ret;
	ধ্রুব বর্ণ *বৃক্ষ_prefix = রিক্ত;
	সংরচনা object_id oid;
	ধ্রুব বর্ণ *me = "git-write-tree";
	সংরচনা option write_বৃক্ষ_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_বৃক্ষ_MISSING_OK),
		OPT_STRING(0, "prefix", &বৃক্ষ_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.মান = &flags,
			.precision = পরিমাণ(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_বৃক্ষ_IGNORE_CACHE_বৃক্ষ,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, রিক্ত);
	argc = parse_options(argc, argv, cmd_prefix, write_বৃক্ষ_options,
			     write_বৃক্ষ_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_বৃক্ষ(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, বৃক্ষ_prefix);
	বিকল্প (ret) {
	পক্ষ 0:
		printf("%s\n", oid_to_hex(&oid));
		ভঙ্গ;
	পক্ষ WRITE_বৃক্ষ_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		ভঙ্গ;
	পক্ষ WRITE_বৃক্ষ_UNMERGED_INDEX:
		die("%s: error building trees", me);
		ভঙ্গ;
	পক্ষ WRITE_বৃক্ষ_PREFIX_ERROR:
		die("%s: prefix %s not found", me, বৃক্ষ_prefix);
		ভঙ্গ;
	}
	প্রত্যাগম ret;
}
