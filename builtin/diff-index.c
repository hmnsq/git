#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "diff-merges.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "preload-index.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "setup.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

പൂർണ്ണസംഖ്യ cmd_diff_index(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	ഘടന rev_info rev;
	unsigned പൂർണ്ണസംഖ്യ option = 0;
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ result;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, ശൂന്യ); /* no "diff" UI options */

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

	argc = setup_revisions(argc, argv, &rev, ശൂന്യ);
	വേണ്ടി (i = 1; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];

		എങ്കിൽ (!strcmp(arg, "--cached"))
			option |= DIFF_INDEX_CACHED;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "-m"))
			rev.match_missing = 1;
		അല്ലെങ്കിൽ
			usage(diff_cache_usage);
	}
	എങ്കിൽ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	എങ്കിൽ (rev.pending.nr != 1 ||
	    rev.max_എണ്ണം != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	എങ്കിൽ (!(option & DIFF_INDEX_CACHED)) {
		setup_work_വൃക്ഷം(the_repository);
		എങ്കിൽ (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			തിരികെ -1;
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (repo_read_index(the_repository) < 0) {
		perror("repo_read_index");
		തിരികെ -1;
	}
	run_diff_index(&rev, option);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	തിരികെ result;
}
