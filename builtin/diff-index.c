#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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

पूर्णाङ्क cmd_diff_सूचकांक(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *उपसर्गः,
		   संरचना repository *repo अप्रयुक्तम्)
{
	संरचना rev_info rev;
	अचिह्नित पूर्णाङ्क option = 0;
	पूर्णाङ्क i;
	पूर्णाङ्क परिणाम;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, रिक्त); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	repo_init_revisions(the_repository, &rev, उपसर्गः);
	rev.abbrev = 0;
	उपसर्गः = precompose_argv_उपसर्गः(argc, argv, उपसर्गः);

	/*
	 * We need (some of) diff for merges options (e.g., --cc), and we need
	 * to avoid conflict with our own meaning of "-m".
	 */
	diff_merges_suppress_m_parsing();

	argc = सज्जता_revisions(argc, argv, &rev, रिक्त);
	प्रति (i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];

		यदि (!वाक्यं_तुलय(arg, "--cached"))
			option |= DIFF_सूचकांक_CACHED;
		अन्यथा यदि (!वाक्यं_तुलय(arg, "--merge-base"))
			option |= DIFF_सूचकांक_MERGE_आधारः;
		अन्यथा यदि (!वाक्यं_तुलय(arg, "-m"))
			rev.मेलनम्_missing = 1;
		अन्यथा
			usage(diff_cache_usage);
	}
	यदि (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_गन्तव्यम्_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	यदि (rev.pending.nr != 1 ||
	    rev.max_गणनम् != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	यदि (!(option & DIFF_सूचकांक_CACHED)) {
		सज्जता_work_वृक्षः(the_repository);
		यदि (repo_read_सूचकांक_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			प्रत्यागम -1;
		}
	} अन्यथा यदि (repo_read_सूचकांक(the_repository) < 0) {
		perror("repo_read_index");
		प्रत्यागम -1;
	}
	सततम्_diff_सूचकांक(&rev, option);
	परिणाम = diff_परिणाम_संहिता(&rev);
	release_revisions(&rev);
	प्रत्यागम परिणाम;
}
