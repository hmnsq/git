#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "diff-merges.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "preload-index.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "setup.h"

স্থির ধ্রুবক অক্ষর diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

পূর্ণসংখ্যা cmd_diff_সূচক(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *prefix,
		   কাঠামো repository *repo UNUSED)
{
	কাঠামো rev_info rev;
	unsigned পূর্ণসংখ্যা option = 0;
	পূর্ণসংখ্যা i;
	পূর্ণসংখ্যা ফলাফল;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, ফাঁকা); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_সূচক = 0;

	repo_init_revisions(the_repository, &rev, prefix);
	rev.abbrev = 0;
	prefix = precompose_argv_prefix(argc, argv, prefix);

	/*
	 * We need (some of) diff for merges options (e.g., --cc), and we need
	 * to avoid conflict with our own meaning of "-m".
	 */
	diff_merges_suppress_m_parsing();

	argc = setup_revisions(argc, argv, &rev, ফাঁকা);
	জন্য (i = 1; i < argc; i++) {
		ধ্রুবক অক্ষর *arg = argv[i];

		যদি (!strcmp(arg, "--cached"))
			option |= DIFF_সূচক_CACHED;
		অন্যথা যদি (!strcmp(arg, "--merge-base"))
			option |= DIFF_সূচক_MERGE_BASE;
		অন্যথা যদি (!strcmp(arg, "-m"))
			rev.match_missing = 1;
		অন্যথা
			usage(diff_cache_usage);
	}
	যদি (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	যদি (rev.pending.nr != 1 ||
	    rev.max_গুনতি != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	যদি (!(option & DIFF_সূচক_CACHED)) {
		setup_work_বৃক্ষ(the_repository);
		যদি (repo_read_সূচক_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ফেরত -1;
		}
	} অন্যথা যদি (repo_read_সূচক(the_repository) < 0) {
		perror("repo_read_index");
		ফেরত -1;
	}
	run_diff_সূচক(&rev, option);
	ফলাফল = diff_ফলাফল_code(&rev);
	release_revisions(&rev);
	ফেরত ফলাফল;
}
