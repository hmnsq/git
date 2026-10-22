#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "diff.h"
#include "diff-merges.h"
#include "commit.h"
#include "preload-index.h"
#include "revision.h"
#include "setup.h"

static const char diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ціле cmd_diff_index(ціле argc,
		   const char **argv,
		   const char *prefix,
		   struct repository *repo UNUSED)
{
	struct rev_info rev;
	unsigned ціле option = 0;
	ціле i;
	ціле result;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, NULL); /* no "diff" UI options */

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

	argc = setup_revisions(argc, argv, &rev, NULL);
	для (i = 1; i < argc; i++) {
		const char *arg = argv[i];

		якщо (!strcmp(arg, "--cached"))
			option |= DIFF_INDEX_CACHED;
		інакше якщо (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		інакше якщо (!strcmp(arg, "-m"))
			rev.match_missing = 1;
		інакше
			usage(diff_cache_usage);
	}
	якщо (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	якщо (rev.pending.nr != 1 ||
	    rev.max_count != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	якщо (!(option & DIFF_INDEX_CACHED)) {
		setup_work_tree(the_repository);
		якщо (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			повернути -1;
		}
	} інакше якщо (repo_read_index(the_repository) < 0) {
		perror("repo_read_index");
		повернути -1;
	}
	run_diff_index(&rev, option);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	повернути result;
}
