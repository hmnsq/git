/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "diff-merges.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "preload-index.h"
#അന്തര്ഭാവ "revision.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം diff_files_usage[] =
"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

പൂർണ്ണസംഖ്യ cmd_diff_files(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	ഘടന rev_info rev;
	പൂർണ്ണസംഖ്യ result;
	unsigned options = 0;

	show_usage_if_asked(argc, argv, diff_files_usage);

	repo_config(the_repository, git_diff_basic_config, ശൂന്യ); /* no "diff" UI options */

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

	argc = setup_revisions(argc, argv, &rev, ശൂന്യ);
	ആകുമ്പോൾ (1 < argc && argv[1][0] == '-') {
		എങ്കിൽ (!strcmp(argv[1], "--base"))
			rev.max_എണ്ണം = 1;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(argv[1], "--ours"))
			rev.max_എണ്ണം = 2;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(argv[1], "--theirs"))
			rev.max_എണ്ണം = 3;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		അല്ലെങ്കിൽ
			usage(diff_files_usage);
		argv++; argc--;
	}
	എങ്കിൽ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;
	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there are NO revision (i.e. pending object) parameter,
	 * rev.max_count is reasonable (0 <= n <= 3), and
	 * there is no other revision filtering parameters.
	 */
	എങ്കിൽ (rev.pending.nr ||
	    rev.min_age != -1 || rev.max_age != -1 ||
	    3 < rev.max_എണ്ണം)
		usage(diff_files_usage);

	/*
	 * "diff-files --base -p" should not combine merges because it
	 * was not asked to.  "diff-files -c -p" should not densify
	 * (the user should ask with "diff-files --cc" explicitly).
	 */
	എങ്കിൽ (rev.max_എണ്ണം == -1 &&
	    (rev.diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(&rev);

	എങ്കിൽ (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0)
		die_errno("repo_read_index_preload");
	run_diff_files(&rev, options);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	തിരികെ result;
}
