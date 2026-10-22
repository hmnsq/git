#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "diff.h"
#समावेश "diff-merges.h"
#समावेश "commit.h"
#समावेश "preload-index.h"
#समावेश "revision.h"
#समावेश "setup.h"

अटल स्थिर अक्षर diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

पूर्णांक cmd_diff_सूचकांक(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *prefix,
		   काष्ठा repository *repo UNUSED)
{
	काष्ठा rev_info rev;
	अचिन्हित पूर्णांक option = 0;
	पूर्णांक i;
	पूर्णांक परिणाम;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, शून्य); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_सूचकांक = 0;

	repo_init_revisions(the_repository, &rev, prefix);
	rev.abbrev = 0;
	prefix = precompose_argv_prefix(argc, argv, prefix);

	/*
	 * We need (some of) diff for merges options (e.g., --cc), and we need
	 * to avoid conflict with our own meaning of "-m".
	 */
	diff_merges_suppress_m_parsing();

	argc = setup_revisions(argc, argv, &rev, शून्य);
	क्रम (i = 1; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];

		अगर (!म_भेद(arg, "--cached"))
			option |= DIFF_सूचकांक_CACHED;
		अन्यथा अगर (!म_भेद(arg, "--merge-base"))
			option |= DIFF_सूचकांक_MERGE_BASE;
		अन्यथा अगर (!म_भेद(arg, "-m"))
			rev.match_missing = 1;
		अन्यथा
			usage(diff_cache_usage);
	}
	अगर (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	अगर (rev.pending.nr != 1 ||
	    rev.max_गिनती != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	अगर (!(option & DIFF_सूचकांक_CACHED)) {
		setup_work_वृक्ष(the_repository);
		अगर (repo_read_सूचकांक_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			लिखो_त्रुटि("repo_read_index_preload");
			वापस -1;
		}
	} अन्यथा अगर (repo_read_सूचकांक(the_repository) < 0) {
		लिखो_त्रुटि("repo_read_index");
		वापस -1;
	}
	run_diff_सूचकांक(&rev, option);
	परिणाम = diff_परिणाम_code(&rev);
	release_revisions(&rev);
	वापस परिणाम;
}
