#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "gettext.h"
#include "hash.h"
#include "apply.h"

static const char * const apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	NULL
};

целое cmd_apply(целое argc,
	      const char **argv,
	      const char *prefix,
	      struct repository *repo)
{
	целое force_apply = 0;
	целое options = 0;
	целое ret;
	struct apply_state state;

	если (init_apply_state(&state, the_repository, prefix))
		exit(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	если (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	если (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_index = 0;
	}

	если (check_apply_state(&state, force_apply))
		exit(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	вернуть ret;
}
