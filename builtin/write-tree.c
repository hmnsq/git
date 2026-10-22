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

স্থির ধ্রুবক অক্ষর * ধ্রুবক write_বৃক্ষ_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	ফাঁকা
};

পূর্ণসংখ্যা cmd_write_বৃক্ষ(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *cmd_prefix,
		   কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা flags = 0, ret;
	ধ্রুবক অক্ষর *বৃক্ষ_prefix = ফাঁকা;
	কাঠামো object_id oid;
	ধ্রুবক অক্ষর *me = "git-write-tree";
	কাঠামো option write_বৃক্ষ_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_বৃক্ষ_MISSING_OK),
		OPT_STRING(0, "prefix", &বৃক্ষ_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.মান = &flags,
			.precision = আকার(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_বৃক্ষ_IGNORE_CACHE_বৃক্ষ,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, ফাঁকা);
	argc = parse_options(argc, argv, cmd_prefix, write_বৃক্ষ_options,
			     write_বৃক্ষ_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_সূচক = 0;

	ret = write_সূচক_as_বৃক্ষ(&oid, the_repository->সূচক,
				  repo_get_সূচক_file(the_repository),
				  flags, বৃক্ষ_prefix);
	নির্বাচন (ret) {
	ক্ষেত্র 0:
		printf("%s\n", oid_to_hex(&oid));
		বিরতি;
	ক্ষেত্র WRITE_বৃক্ষ_UNREADABLE_সূচক:
		die("%s: error reading the index", me);
		বিরতি;
	ক্ষেত্র WRITE_বৃক্ষ_UNMERGED_সূচক:
		die("%s: error building trees", me);
		বিরতি;
	ক্ষেত্র WRITE_বৃক্ষ_PREFIX_ERROR:
		die("%s: prefix %s not found", me, বৃক্ষ_prefix);
		বিরতি;
	}
	ফেরত ret;
}
