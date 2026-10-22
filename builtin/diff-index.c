#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "diff-merges.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "preload-index.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "setup.h"

સ્થિર અચલ અક્ષર diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

પૂર્ણાંક cmd_diff_index(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix,
		   સંરચના repository *repo UNUSED)
{
	સંરચના rev_info rev;
	unsigned પૂર્ણાંક option = 0;
	પૂર્ણાંક i;
	પૂર્ણાંક result;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, ખાલી); /* no "diff" UI options */

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

	argc = setup_revisions(argc, argv, &rev, ખાલી);
	માટે (i = 1; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];

		જો (!strcmp(arg, "--cached"))
			option |= DIFF_INDEX_CACHED;
		નહીંતર જો (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		નહીંતર જો (!strcmp(arg, "-m"))
			rev.match_missing = 1;
		નહીંતર
			usage(diff_cache_usage);
	}
	જો (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	જો (rev.pending.nr != 1 ||
	    rev.max_ગણતરી != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	જો (!(option & DIFF_INDEX_CACHED)) {
		setup_work_વૃક્ષ(the_repository);
		જો (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			પાછા -1;
		}
	} નહીંતર જો (repo_read_index(the_repository) < 0) {
		perror("repo_read_index");
		પાછા -1;
	}
	run_diff_index(&rev, option);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	પાછા result;
}
