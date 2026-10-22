#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "diff-merges.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "setup.h"

स्थिर ध्रुव वर्ण diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

पूर्णाङ्क cmd_diff_index(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *prefix,
		   संरचना repository *repo UNUSED)
{
	संरचना rev_info rev;
	unsigned पूर्णाङ्क option = 0;
	पूर्णाङ्क i;
	पूर्णाङ्क result;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, रिक्त); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_init_revisions(the_repository, &rev, prefix);
	rev.abbrev = 0;
	prefix = precompose_argv_prefix(argc, argv, prefix);

	/*
	 * We need (some of) diff for merges options (e.g., --cc), and we need
	 * to avoid conflict with our own meaning of "-m".
	 */
	diff_merges_suppress_m_parsing();

	argc = setup_revisions(argc, argv, &rev, रिक्त);
	प्रति (i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];

		जइ (!strcmp(arg, "--cached"))
			option |= DIFF_INDEX_CACHED;
		अण्णहा जइ (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		अण्णहा जइ (!strcmp(arg, "-m"))
			rev.match_missing = 1;
		अण्णहा
			usage(diff_cache_usage);
	}
	जइ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	जइ (rev.pending.nr != 1 ||
	    rev.max_गणनम् != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	जइ (!(option & DIFF_INDEX_CACHED)) {
		setup_work_वृक्षः(the_repository);
		जइ (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			प्रत्यागम -1;
		}
	} अण्णहा जइ (repo_read_index(the_repository) < 0) {
		perror("repo_read_index");
		प्रत्यागम -1;
	}
	run_diff_index(&rev, option);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	प्रत्यागम result;
}
