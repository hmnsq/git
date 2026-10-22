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

स्थिर स्थिरांक अक्षर diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

पूर्णांक cmd_diff_index(पूर्णांक argc,
		   स्थिरांक अक्षर **argv,
		   स्थिरांक अक्षर *prefix,
		   संरचना repository *repo UNUSED)
{
	संरचना rev_info rev;
	अचिन्हित पूर्णांक option = 0;
	पूर्णांक i;
	पूर्णांक result;

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
	साठी (i = 1; i < argc; i++) {
		स्थिरांक अक्षर *arg = argv[i];

		जर (!म_भेद(arg, "--cached"))
			option |= DIFF_INDEX_CACHED;
		अन्यथा जर (!म_भेद(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		अन्यथा जर (!म_भेद(arg, "-m"))
			rev.match_missing = 1;
		अन्यथा
			usage(diff_cache_usage);
	}
	जर (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	जर (rev.pending.nr != 1 ||
	    rev.max_मोजणी != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	जर (!(option & DIFF_INDEX_CACHED)) {
		setup_work_वृक्ष(the_repository);
		जर (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			लिखो_त्रुटि("repo_read_index_preload");
			परत -1;
		}
	} अन्यथा जर (repo_read_index(the_repository) < 0) {
		लिखो_त्रुटि("repo_read_index");
		परत -1;
	}
	run_diff_index(&rev, option);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	परत result;
}
