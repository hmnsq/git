#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "diff.h"
#شامل "diff-merges.h"
#شامل "commit.h"
#شامل "preload-index.h"
#شامل "revision.h"
#شامل "setup.h"

ساکن ثابت حرف diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

صحیح cmd_diff_اشاریہ(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	ساخت rev_info rev;
	غیرعلامتی صحیح option = 0;
	صحیح i;
	صحیح نتیجہ;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, تہی); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	repo_init_revisions(the_repository, &rev, سابقہ);
	rev.abbrev = 0;
	سابقہ = precompose_argv_سابقہ(argc, argv, سابقہ);

	/*
	 * We need (some of) diff for merges options (e.g., --cc), and we need
	 * to avoid conflict with our own meaning of "-m".
	 */
	diff_merges_suppress_m_parsing();

	argc = تیاری_revisions(argc, argv, &rev, تہی);
	برائے (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		اگر (!strcmp(arg, "--cached"))
			option |= DIFF_اشاریہ_CACHED;
		ورنہ اگر (!strcmp(arg, "--merge-base"))
			option |= DIFF_اشاریہ_ملاؤ_بنیاد;
		ورنہ اگر (!strcmp(arg, "-m"))
			rev.مطابقت_missing = 1;
		ورنہ
			usage(diff_cache_usage);
	}
	اگر (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_مقصد_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	اگر (rev.pending.nr != 1 ||
	    rev.max_گنتی != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	اگر (!(option & DIFF_اشاریہ_CACHED)) {
		تیاری_work_درخت(the_repository);
		اگر (repo_read_اشاریہ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			واپس -1;
		}
	} ورنہ اگر (repo_read_اشاریہ(the_repository) < 0) {
		perror("repo_read_index");
		واپس -1;
	}
	تسلسل_diff_اشاریہ(&rev, option);
	نتیجہ = diff_نتیجہ_رمز(&rev);
	release_revisions(&rev);
	واپس نتیجہ;
}
