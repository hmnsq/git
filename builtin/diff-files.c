/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "diff-merges.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "revision.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ diff_files_usage[] =
"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ಪೂರ್ಣಾಂಕ cmd_diff_files(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ rev_info rev;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
	ಚಿಹ್ನೆರಹಿತ options = 0;

	show_usage_if_asked(argc, argv, diff_files_usage);

	repo_config(the_repository, git_diff_basic_config, ಖಾಲಿ); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	repo_init_revisions(the_repository, &rev, ಪೂರ್ವಪ್ರತ್ಯಯ);
	rev.abbrev = 0;

	/*
	 * Consider "intent-to-add" files as new by default, unless
	 * explicitly specified in the command line or anywhere else.
	 */
	rev.diffopt.ita_invisible_ಒಳ_ಸೂಚ್ಯಂಕ = 1;

	ಪೂರ್ವಪ್ರತ್ಯಯ = precompose_argv_ಪೂರ್ವಪ್ರತ್ಯಯ(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ);

	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, &rev, ಖಾಲಿ);
	ಆಗ (1 < argc && argv[1][0] == '-') {
		ಆದರೆ (!strcmp(argv[1], "--base"))
			rev.max_ಎಣಿಕೆ = 1;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(argv[1], "--ours"))
			rev.max_ಎಣಿಕೆ = 2;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(argv[1], "--theirs"))
			rev.max_ಎಣಿಕೆ = 3;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		ಇಲ್ಲದಿದ್ದರೆ
			usage(diff_files_usage);
		argv++; argc--;
	}
	ಆದರೆ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;
	rev.diffopt.rotate_ಗಮ್ಯ_strict = 1;

	/*
	 * Make sure there are NO revision (i.e. pending object) parameter,
	 * rev.max_count is reasonable (0 <= n <= 3), and
	 * there is no other revision filtering parameters.
	 */
	ಆದರೆ (rev.pending.nr ||
	    rev.min_age != -1 || rev.max_age != -1 ||
	    3 < rev.max_ಎಣಿಕೆ)
		usage(diff_files_usage);

	/*
	 * "diff-files --base -p" should not combine merges because it
	 * was not asked to.  "diff-files -c -p" should not densify
	 * (the user should ask with "diff-files --cc" explicitly).
	 */
	ಆದರೆ (rev.max_ಎಣಿಕೆ == -1 &&
	    (rev.diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_ಗಣ_dense_combined_if_unset(&rev);

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0)
		die_errno("repo_read_index_preload");
	ಸತತ_diff_files(&rev, options);
	ಫಲಿತಾಂಶ = diff_ಫಲಿತಾಂಶ_ಸಂಕೇತ(&rev);
	release_revisions(&rev);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
