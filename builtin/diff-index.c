#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff-merges.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "preload-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ਪੂਰਨ cmd_diff_ਸੂਚਕਾਂਕ(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ rev_info rev;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ option = 0;
	ਪੂਰਨ i;
	ਪੂਰਨ ਨਤੀਜਾ;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, ਨਿਰਾ); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	repo_init_revisions(the_repository, &rev, ਅਗੇਤਰ);
	rev.abbrev = 0;
	ਅਗੇਤਰ = precompose_argv_ਅਗੇਤਰ(argc, argv, ਅਗੇਤਰ);

	/*
	 * We need (some of) diff for merges options (e.g., --cc), and we need
	 * to avoid conflict with our own meaning of "-m".
	 */
	diff_merges_suppress_m_parsing();

	argc = ਤਿਆਰੀ_revisions(argc, argv, &rev, ਨਿਰਾ);
	ਲਈ (i = 1; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];

		ਜੇ (!strcmp(arg, "--cached"))
			option |= DIFF_ਸੂਚਕਾਂਕ_CACHED;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--merge-base"))
			option |= DIFF_ਸੂਚਕਾਂਕ_ਮਿਲਾਓ_ਅਧਾਰ;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "-m"))
			rev.ਮੇਲ_missing = 1;
		ਨਹੀਂਤਾਂ
			usage(diff_cache_usage);
	}
	ਜੇ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_ਮੰਜ਼ਿਲ_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	ਜੇ (rev.pending.nr != 1 ||
	    rev.max_ਗਿਣਤੀ != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	ਜੇ (!(option & DIFF_ਸੂਚਕਾਂਕ_CACHED)) {
		ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
		ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ਵਾਪਸ -1;
		}
	} ਨਹੀਂਤਾਂ ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0) {
		perror("repo_read_index");
		ਵਾਪਸ -1;
	}
	ਲੜੀਵਾਰ_diff_ਸੂਚਕਾਂਕ(&rev, option);
	ਨਤੀਜਾ = diff_ਨਤੀਜਾ_ਕੋਡ(&rev);
	release_revisions(&rev);
	ਵਾਪਸ ਨਤੀਜਾ;
}
