#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "diff-merges.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "preload-index.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_diff_index(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		   ଅଚଳ ଅକ୍ଷର **argv,
		   ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା rev_info rev;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା option = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, ଖାଲି); /* no "diff" UI options */

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

	argc = setup_revisions(argc, argv, &rev, ଖାଲି);
	ପାଇଁ (i = 1; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];

		ଯଦି (!strcmp(arg, "--cached"))
			option |= DIFF_INDEX_CACHED;
		ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "-m"))
			rev.match_missing = 1;
		ଅନ୍ୟଥା
			usage(diff_cache_usage);
	}
	ଯଦି (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	ଯଦି (rev.pending.nr != 1 ||
	    rev.max_ଗଣନା != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	ଯଦି (!(option & DIFF_INDEX_CACHED)) {
		setup_work_ବୃକ୍ଷ(the_repository);
		ଯଦି (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ଫେରନ୍ତୁ -1;
		}
	} ଅନ୍ୟଥା ଯଦି (repo_read_index(the_repository) < 0) {
		perror("repo_read_index");
		ଫେରନ୍ତୁ -1;
	}
	run_diff_index(&rev, option);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	ଫେରନ୍ତୁ result;
}
