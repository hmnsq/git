#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "gettext.h"
#include "hash.h"
#include "apply.h"

static const char * const apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	NULL
};

정수 cmd_apply(정수 argc,
	      const char **argv,
	      const char *prefix,
	      struct repository *repo)
{
	정수 force_apply = 0;
	정수 options = 0;
	정수 ret;
	struct apply_state state;

	만약 (init_apply_state(&state, the_repository, prefix))
		exit(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	만약 (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	만약 (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_index = 0;
	}

	만약 (check_apply_state(&state, force_apply))
		exit(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	반환 ret;
}
