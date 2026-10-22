/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "diff.h"
#समावेश "diff-merges.h"
#समावेश "commit.h"
#समावेश "preload-index.h"
#समावेश "revision.h"

अटल स्थिर अक्षर diff_files_usage[] =
"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

पूर्णांक cmd_diff_files(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *prefix,
		   काष्ठा repository *repo UNUSED)
{
	काष्ठा rev_info rev;
	पूर्णांक परिणाम;
	अचिन्हित options = 0;

	show_usage_if_asked(argc, argv, diff_files_usage);

	repo_config(the_repository, git_diff_basic_config, शून्य); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_सूचकांक = 0;

	repo_init_revisions(the_repository, &rev, prefix);
	rev.abbrev = 0;

	/*
	 * Consider "intent-to-add" files as new by default, unless
	 * explicitly specified in the command line or anywhere else.
	 */
	rev.diffopt.ita_invisible_in_सूचकांक = 1;

	prefix = precompose_argv_prefix(argc, argv, prefix);

	argc = setup_revisions(argc, argv, &rev, शून्य);
	जबतक (1 < argc && argv[1][0] == '-') {
		अगर (!म_भेद(argv[1], "--base"))
			rev.max_गिनती = 1;
		अन्यथा अगर (!म_भेद(argv[1], "--ours"))
			rev.max_गिनती = 2;
		अन्यथा अगर (!म_भेद(argv[1], "--theirs"))
			rev.max_गिनती = 3;
		अन्यथा अगर (!म_भेद(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		अन्यथा
			usage(diff_files_usage);
		argv++; argc--;
	}
	अगर (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;
	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there are NO revision (i.e. pending object) parameter,
	 * rev.max_count is reasonable (0 <= n <= 3), and
	 * there is no other revision filtering parameters.
	 */
	अगर (rev.pending.nr ||
	    rev.min_age != -1 || rev.max_age != -1 ||
	    3 < rev.max_गिनती)
		usage(diff_files_usage);

	/*
	 * "diff-files --base -p" should not combine merges because it
	 * was not asked to.  "diff-files -c -p" should not densify
	 * (the user should ask with "diff-files --cc" explicitly).
	 */
	अगर (rev.max_गिनती == -1 &&
	    (rev.diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(&rev);

	अगर (repo_read_सूचकांक_preload(the_repository, &rev.diffopt.pathspec, 0) < 0)
		die_errno("repo_read_index_preload");
	run_diff_files(&rev, options);
	परिणाम = diff_परिणाम_code(&rev);
	release_revisions(&rev);
	वापस परिणाम;
}
