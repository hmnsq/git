#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "diff.h"
#ضمن "diff-merges.h"
#ضمن "commit.h"
#ضمن "preload-index.h"
#ضمن "revision.h"
#ضمن "setup.h"

ساكن ثابت حرف diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

صحيح cmd_diff_شاخص(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *مختاړی,
		   بنية repository *repo ناکارول_شوی)
{
	بنية rev_info rev;
	بینښه صحيح option = 0;
	صحيح i;
	صحيح پایله;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, عدم); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ډک_شاخص = 0;

	repo_init_revisions(the_repository, &rev, مختاړی);
	rev.abbrev = 0;
	مختاړی = precompose_argv_مختاړی(argc, argv, مختاړی);

	/*
	 * We need (some of) diff for merges options (e.g., --cc), and we need
	 * to avoid conflict with our own meaning of "-m".
	 */
	diff_merges_suppress_m_parsing();

	argc = تیاری_revisions(argc, argv, &rev, عدم);
	لكل (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		إذا (!strcmp(arg, "--cached"))
			option |= DIFF_شاخص_CACHED;
		وإلا إذا (!strcmp(arg, "--merge-base"))
			option |= DIFF_شاخص_یوځای_بنسټ;
		وإلا إذا (!strcmp(arg, "-m"))
			rev.سمون_missing = 1;
		وإلا
			usage(diff_cache_usage);
	}
	إذا (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_منزل_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	إذا (rev.pending.nr != 1 ||
	    rev.max_شمېر != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	إذا (!(option & DIFF_شاخص_CACHED)) {
		تیاری_work_ونه(the_repository);
		إذا (repo_read_شاخص_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			أرجع -1;
		}
	} وإلا إذا (repo_read_شاخص(the_repository) < 0) {
		perror("repo_read_index");
		أرجع -1;
	}
	پرلپسې_diff_شاخص(&rev, option);
	پایله = diff_پایله_کوډ(&rev);
	release_revisions(&rev);
	أرجع پایله;
}
