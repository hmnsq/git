#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "diff.h"
#شامل "diff-merges.h"
#شامل "commit.h"
#شامل "preload-index.h"
#شامل "revision.h"
#شامل "setup.h"

ایستا ثابت نویسه diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

صحیح cmd_diff_index(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix,
		   ساختار repository *repo UNUSED)
{
	ساختار rev_info rev;
	unsigned صحیح option = 0;
	صحیح i;
	صحیح result;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, پوچ); /* no "diff" UI options */

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

	argc = setup_revisions(argc, argv, &rev, پوچ);
	برای (i = 1; i < argc; i++) {
		ثابت نویسه *arg = argv[i];

		اگر (!strcmp(arg, "--cached"))
			option |= DIFF_INDEX_CACHED;
		وگرنه اگر (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		وگرنه اگر (!strcmp(arg, "-m"))
			rev.match_missing = 1;
		وگرنه
			usage(diff_cache_usage);
	}
	اگر (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	اگر (rev.pending.nr != 1 ||
	    rev.max_شمار != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	اگر (!(option & DIFF_INDEX_CACHED)) {
		setup_work_درخت(the_repository);
		اگر (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			بازگشت -1;
		}
	} وگرنه اگر (repo_read_index(the_repository) < 0) {
		perror("repo_read_index");
		بازگشت -1;
	}
	run_diff_index(&rev, option);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	بازگشت result;
}
