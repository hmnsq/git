#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "diff-merges.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "preload-index.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "setup.h"

நிலையான மாறிலி எழுத்து diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

முழு cmd_diff_index(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு rev_info rev;
	unsigned முழு option = 0;
	முழு i;
	முழு result;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, வெற்று); /* no "diff" UI options */

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

	argc = setup_revisions(argc, argv, &rev, வெற்று);
	க்கு (i = 1; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];

		எனில் (!strcmp(arg, "--cached"))
			option |= DIFF_INDEX_CACHED;
		இல்லையெனில் எனில் (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		இல்லையெனில் எனில் (!strcmp(arg, "-m"))
			rev.match_missing = 1;
		இல்லையெனில்
			usage(diff_cache_usage);
	}
	எனில் (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	எனில் (rev.pending.nr != 1 ||
	    rev.max_மொத்தம் != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	எனில் (!(option & DIFF_INDEX_CACHED)) {
		setup_work_மரம்(the_repository);
		எனில் (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			திரும்பு -1;
		}
	} இல்லையெனில் எனில் (repo_read_index(the_repository) < 0) {
		perror("repo_read_index");
		திரும்பு -1;
	}
	run_diff_index(&rev, option);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	திரும்பு result;
}
