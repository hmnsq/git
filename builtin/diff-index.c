#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "diff-merges.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "setup.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ಪೂರ್ಣಾಂಕ cmd_diff_ಸೂಚ್ಯಂಕ(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ rev_info rev;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ option = 0;
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, ಖಾಲಿ); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	repo_init_revisions(the_repository, &rev, ಪೂರ್ವಪ್ರತ್ಯಯ);
	rev.abbrev = 0;
	ಪೂರ್ವಪ್ರತ್ಯಯ = precompose_argv_ಪೂರ್ವಪ್ರತ್ಯಯ(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ);

	/*
	 * We need (some of) diff for merges options (e.g., --cc), and we need
	 * to avoid conflict with our own meaning of "-m".
	 */
	diff_merges_suppress_m_parsing();

	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, &rev, ಖಾಲಿ);
	ಗಾಗಿ (i = 1; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];

		ಆದರೆ (!strcmp(arg, "--cached"))
			option |= DIFF_ಸೂಚ್ಯಂಕ_CACHED;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--merge-base"))
			option |= DIFF_ಸೂಚ್ಯಂಕ_ವಿಲೀನ_ತಳ;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "-m"))
			rev.ಹೊಂದಿಕೆ_missing = 1;
		ಇಲ್ಲದಿದ್ದರೆ
			usage(diff_cache_usage);
	}
	ಆದರೆ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_ಗಮ್ಯ_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	ಆದರೆ (rev.pending.nr != 1 ||
	    rev.max_ಎಣಿಕೆ != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	ಆದರೆ (!(option & DIFF_ಸೂಚ್ಯಂಕ_CACHED)) {
		ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
		ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ಹಿಂತಿರುಗಿ -1;
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0) {
		perror("repo_read_index");
		ಹಿಂತಿರುಗಿ -1;
	}
	ಸತತ_diff_ಸೂಚ್ಯಂಕ(&rev, option);
	ಫಲಿತಾಂಶ = diff_ಫಲಿತಾಂಶ_ಸಂಕೇತ(&rev);
	release_revisions(&rev);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
