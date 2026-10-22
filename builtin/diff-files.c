/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "diff-merges.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "preload-index.h"
#অন্তর্ভাব "revision.h"

স্থির ধ্রুবক অক্ষর diff_files_usage[] =
"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

পূর্ণসংখ্যা cmd_diff_files(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *prefix,
		   কাঠামো repository *repo UNUSED)
{
	কাঠামো rev_info rev;
	পূর্ণসংখ্যা ফলাফল;
	unsigned options = 0;

	show_usage_if_asked(argc, argv, diff_files_usage);

	repo_config(the_repository, git_diff_basic_config, ফাঁকা); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_সূচক = 0;

	repo_init_revisions(the_repository, &rev, prefix);
	rev.abbrev = 0;

	/*
	 * Consider "intent-to-add" files as new by default, unless
	 * explicitly specified in the command line or anywhere else.
	 */
	rev.diffopt.ita_invisible_in_সূচক = 1;

	prefix = precompose_argv_prefix(argc, argv, prefix);

	argc = setup_revisions(argc, argv, &rev, ফাঁকা);
	যখন (1 < argc && argv[1][0] == '-') {
		যদি (!strcmp(argv[1], "--base"))
			rev.max_গুনতি = 1;
		অন্যথা যদি (!strcmp(argv[1], "--ours"))
			rev.max_গুনতি = 2;
		অন্যথা যদি (!strcmp(argv[1], "--theirs"))
			rev.max_গুনতি = 3;
		অন্যথা যদি (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		অন্যথা
			usage(diff_files_usage);
		argv++; argc--;
	}
	যদি (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;
	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there are NO revision (i.e. pending object) parameter,
	 * rev.max_count is reasonable (0 <= n <= 3), and
	 * there is no other revision filtering parameters.
	 */
	যদি (rev.pending.nr ||
	    rev.min_age != -1 || rev.max_age != -1 ||
	    3 < rev.max_গুনতি)
		usage(diff_files_usage);

	/*
	 * "diff-files --base -p" should not combine merges because it
	 * was not asked to.  "diff-files -c -p" should not densify
	 * (the user should ask with "diff-files --cc" explicitly).
	 */
	যদি (rev.max_গুনতি == -1 &&
	    (rev.diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(&rev);

	যদি (repo_read_সূচক_preload(the_repository, &rev.diffopt.pathspec, 0) < 0)
		die_errno("repo_read_index_preload");
	run_diff_files(&rev, options);
	ফলাফল = diff_ফলাফল_code(&rev);
	release_revisions(&rev);
	ফেরত ফলাফল;
}
