/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "diff-merges.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "revision.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ diff_files_usage[] =
"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ಪೂರ್ಣಾಙ್ಕ cmd_diff_files(ಪೂರ್ಣಾಙ್ಕ argc,
		   ಧ್ರುವ ವರ್ಣ **argv,
		   ಧ್ರುವ ವರ್ಣ *prefix,
		   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ rev_info rev;
	ಪೂರ್ಣಾಙ್ಕ result;
	unsigned options = 0;

	show_usage_if_asked(argc, argv, diff_files_usage);

	repo_config(the_repository, git_diff_basic_config, ರಿಕ್ತ); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_init_revisions(the_repository, &rev, prefix);
	rev.abbrev = 0;

	/*
	 * Consider "intent-to-add" files as new by default, unless
	 * explicitly specified in the command line or anywhere else.
	 */
	rev.diffopt.ita_invisible_in_index = 1;

	prefix = precompose_argv_prefix(argc, argv, prefix);

	argc = setup_revisions(argc, argv, &rev, ರಿಕ್ತ);
	ಯಾವತ್ (1 < argc && argv[1][0] == '-') {
		ಯದಿ (!strcmp(argv[1], "--base"))
			rev.max_ಎಣಿಕೆ = 1;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(argv[1], "--ours"))
			rev.max_ಎಣಿಕೆ = 2;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(argv[1], "--theirs"))
			rev.max_ಎಣಿಕೆ = 3;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		ಅನ್ಯಥಾ
			usage(diff_files_usage);
		argv++; argc--;
	}
	ಯದಿ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;
	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there are NO revision (i.e. pending object) parameter,
	 * rev.max_count is reasonable (0 <= n <= 3), and
	 * there is no other revision filtering parameters.
	 */
	ಯದಿ (rev.pending.nr ||
	    rev.min_age != -1 || rev.max_age != -1 ||
	    3 < rev.max_ಎಣಿಕೆ)
		usage(diff_files_usage);

	/*
	 * "diff-files --base -p" should not combine merges because it
	 * was not asked to.  "diff-files -c -p" should not densify
	 * (the user should ask with "diff-files --cc" explicitly).
	 */
	ಯದಿ (rev.max_ಎಣಿಕೆ == -1 &&
	    (rev.diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(&rev);

	ಯದಿ (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0)
		die_errno("repo_read_index_preload");
	run_diff_files(&rev, options);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	ಪ್ರತ್ಯಾಗಮ result;
}
