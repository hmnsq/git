#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "gettext.h"
#include "hash.h"
#include "apply.h"

static const var_na * const apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	NULL
};

ຈຳນວນເຕັມ cmd_apply(ຈຳນວນເຕັມ argc,
	      const var_na **argv,
	      const var_na *prefix,
	      sa_nracanaa repository *repo)
{
	ຈຳນວນເຕັມ force_apply = 0;
	ຈຳນວນເຕັມ options = 0;
	ຈຳນວນເຕັມ ret;
	sa_nracanaa apply_state state;

	ຖ້າ (init_apply_state(&state, the_repository, prefix))
		exit(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	ຖ້າ (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	ຖ້າ (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_index = 0;
	}

	ຖ້າ (check_apply_state(&state, force_apply))
		exit(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	ສົ່ງຄືນ ret;
}
