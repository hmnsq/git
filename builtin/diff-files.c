/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff-merges.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "preload-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ diff_files_usage[] =
"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ਪੂਰਨ cmd_diff_files(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ rev_info rev;
	ਪੂਰਨ ਨਤੀਜਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ options = 0;

	show_usage_if_asked(argc, argv, diff_files_usage);

	repo_config(the_repository, git_diff_basic_config, ਨਿਰਾ); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	repo_init_revisions(the_repository, &rev, ਅਗੇਤਰ);
	rev.abbrev = 0;

	/*
	 * Consider "intent-to-add" files as new by default, unless
	 * explicitly specified in the command line or anywhere else.
	 */
	rev.diffopt.ita_invisible_ਵਿੱਚ_ਸੂਚਕਾਂਕ = 1;

	ਅਗੇਤਰ = precompose_argv_ਅਗੇਤਰ(argc, argv, ਅਗੇਤਰ);

	argc = ਤਿਆਰੀ_revisions(argc, argv, &rev, ਨਿਰਾ);
	ਜਦੋਂ (1 < argc && argv[1][0] == '-') {
		ਜੇ (!strcmp(argv[1], "--base"))
			rev.max_ਗਿਣਤੀ = 1;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(argv[1], "--ours"))
			rev.max_ਗਿਣਤੀ = 2;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(argv[1], "--theirs"))
			rev.max_ਗਿਣਤੀ = 3;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		ਨਹੀਂਤਾਂ
			usage(diff_files_usage);
		argv++; argc--;
	}
	ਜੇ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;
	rev.diffopt.rotate_ਮੰਜ਼ਿਲ_strict = 1;

	/*
	 * Make sure there are NO revision (i.e. pending object) parameter,
	 * rev.max_count is reasonable (0 <= n <= 3), and
	 * there is no other revision filtering parameters.
	 */
	ਜੇ (rev.pending.nr ||
	    rev.min_age != -1 || rev.max_age != -1 ||
	    3 < rev.max_ਗਿਣਤੀ)
		usage(diff_files_usage);

	/*
	 * "diff-files --base -p" should not combine merges because it
	 * was not asked to.  "diff-files -c -p" should not densify
	 * (the user should ask with "diff-files --cc" explicitly).
	 */
	ਜੇ (rev.max_ਗਿਣਤੀ == -1 &&
	    (rev.diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_ਸੈੱਟ_dense_combined_if_unset(&rev);

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0)
		die_errno("repo_read_index_preload");
	ਲੜੀਵਾਰ_diff_files(&rev, options);
	ਨਤੀਜਾ = diff_ਨਤੀਜਾ_ਕੋਡ(&rev);
	release_revisions(&rev);
	ਵਾਪਸ ਨਤੀਜਾ;
}
