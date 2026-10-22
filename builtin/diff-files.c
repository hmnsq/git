/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "diff-merges.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "revision.h"

स्थिर ध्रुव वर्ण diff_files_usage[] =
"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

पूर्णाङ्क cmd_diff_files(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *उपसर्गः,
		   संरचना repository *repo अप्रयुक्तम्)
{
	संरचना rev_info rev;
	पूर्णाङ्क परिणाम;
	अचिह्नित options = 0;

	show_usage_if_asked(argc, argv, diff_files_usage);

	repo_config(the_repository, git_diff_basic_config, रिक्त); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	repo_init_revisions(the_repository, &rev, उपसर्गः);
	rev.abbrev = 0;

	/*
	 * Consider "intent-to-add" files as new by default, unless
	 * explicitly specified in the command line or anywhere else.
	 */
	rev.diffopt.ita_invisible_अन्तर्_सूचकांक = 1;

	उपसर्गः = precompose_argv_उपसर्गः(argc, argv, उपसर्गः);

	argc = सज्जता_revisions(argc, argv, &rev, रिक्त);
	यावत् (1 < argc && argv[1][0] == '-') {
		यदि (!वाक्यं_तुलय(argv[1], "--base"))
			rev.max_गणनम् = 1;
		अन्यथा यदि (!वाक्यं_तुलय(argv[1], "--ours"))
			rev.max_गणनम् = 2;
		अन्यथा यदि (!वाक्यं_तुलय(argv[1], "--theirs"))
			rev.max_गणनम् = 3;
		अन्यथा यदि (!वाक्यं_तुलय(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		अन्यथा
			usage(diff_files_usage);
		argv++; argc--;
	}
	यदि (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;
	rev.diffopt.rotate_गन्तव्यम्_strict = 1;

	/*
	 * Make sure there are NO revision (i.e. pending object) parameter,
	 * rev.max_count is reasonable (0 <= n <= 3), and
	 * there is no other revision filtering parameters.
	 */
	यदि (rev.pending.nr ||
	    rev.min_age != -1 || rev.max_age != -1 ||
	    3 < rev.max_गणनम्)
		usage(diff_files_usage);

	/*
	 * "diff-files --base -p" should not combine merges because it
	 * was not asked to.  "diff-files -c -p" should not densify
	 * (the user should ask with "diff-files --cc" explicitly).
	 */
	यदि (rev.max_गणनम् == -1 &&
	    (rev.diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_समुच्चयः_dense_combined_if_unset(&rev);

	यदि (repo_read_सूचकांक_preload(the_repository, &rev.diffopt.pathspec, 0) < 0)
		die_errno("repo_read_index_preload");
	सततम्_diff_files(&rev, options);
	परिणाम = diff_परिणाम_संहिता(&rev);
	release_revisions(&rev);
	प्रत्यागम परिणाम;
}
