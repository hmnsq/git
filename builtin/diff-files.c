/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "diff.h"
#شامل "diff-merges.h"
#شامل "commit.h"
#شامل "preload-index.h"
#شامل "revision.h"

ساکن ثابت حرف diff_files_usage[] =
"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

صحیح cmd_diff_files(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	ساخت rev_info rev;
	صحیح نتیجہ;
	غیرعلامتی options = 0;

	show_usage_if_asked(argc, argv, diff_files_usage);

	repo_config(the_repository, git_diff_basic_config, تہی); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	repo_init_revisions(the_repository, &rev, سابقہ);
	rev.abbrev = 0;

	/*
	 * Consider "intent-to-add" files as new by default, unless
	 * explicitly specified in the command line or anywhere else.
	 */
	rev.diffopt.ita_invisible_اندر_اشاریہ = 1;

	سابقہ = precompose_argv_سابقہ(argc, argv, سابقہ);

	argc = تیاری_revisions(argc, argv, &rev, تہی);
	جب (1 < argc && argv[1][0] == '-') {
		اگر (!strcmp(argv[1], "--base"))
			rev.max_گنتی = 1;
		ورنہ اگر (!strcmp(argv[1], "--ours"))
			rev.max_گنتی = 2;
		ورنہ اگر (!strcmp(argv[1], "--theirs"))
			rev.max_گنتی = 3;
		ورنہ اگر (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		ورنہ
			usage(diff_files_usage);
		argv++; argc--;
	}
	اگر (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;
	rev.diffopt.rotate_مقصد_strict = 1;

	/*
	 * Make sure there are NO revision (i.e. pending object) parameter,
	 * rev.max_count is reasonable (0 <= n <= 3), and
	 * there is no other revision filtering parameters.
	 */
	اگر (rev.pending.nr ||
	    rev.min_age != -1 || rev.max_age != -1 ||
	    3 < rev.max_گنتی)
		usage(diff_files_usage);

	/*
	 * "diff-files --base -p" should not combine merges because it
	 * was not asked to.  "diff-files -c -p" should not densify
	 * (the user should ask with "diff-files --cc" explicitly).
	 */
	اگر (rev.max_گنتی == -1 &&
	    (rev.diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_سیٹ_dense_combined_if_unset(&rev);

	اگر (repo_read_اشاریہ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0)
		die_errno("repo_read_index_preload");
	تسلسل_diff_files(&rev, options);
	نتیجہ = diff_نتیجہ_رمز(&rev);
	release_revisions(&rev);
	واپس نتیجہ;
}
