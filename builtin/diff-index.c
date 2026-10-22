#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

صحيح cmd_diff_اشاریہ(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *اگیتر,
		   بنية repository *repo بن_ورتیا)
{
	بنية rev_info rev;
	بنانشانی صحيح option = 0;
	صحيح i;
	صحيح نتیجہ;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, عدم); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھریا_اشاریہ = 0;

	repo_init_revisions(the_repository, &rev, اگیتر);
	rev.abbrev = 0;
	اگیتر = precompose_argv_اگیتر(argc, argv, اگیتر);

	/*
	 * We need (some of) diff for merges options (e.g., --cc), and we need
	 * to avoid conflict with our own meaning of "-m".
	 */
	diff_merges_suppress_m_parsing();

	argc = تیاری_revisions(argc, argv, &rev, عدم);
	لكل (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		إذا (!strcmp(arg, "--cached"))
			option |= DIFF_اشاریہ_CACHED;
		وإلا إذا (!strcmp(arg, "--merge-base"))
			option |= DIFF_اشاریہ_ملاؤ_ادھار;
		وإلا إذا (!strcmp(arg, "-m"))
			rev.میل_missing = 1;
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
	    rev.max_گنتی != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	إذا (!(option & DIFF_اشاریہ_CACHED)) {
		تیاری_work_درخت(the_repository);
		إذا (repo_read_اشاریہ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			أرجع -1;
		}
	} وإلا إذا (repo_read_اشاریہ(the_repository) < 0) {
		perror("repo_read_index");
		أرجع -1;
	}
	لگاتار_diff_اشاریہ(&rev, option);
	نتیجہ = diff_نتیجہ_کوڈ(&rev);
	release_revisions(&rev);
	أرجع نتیجہ;
}
