#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "apply.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	ರಿಕ್ತ
};

ಪೂರ್ಣಾಙ್ಕ cmd_apply(ಪೂರ್ಣಾಙ್ಕ argc,
	      ಧ್ರುವ ವರ್ಣ **argv,
	      ಧ್ರುವ ವರ್ಣ *prefix,
	      ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ force_apply = 0;
	ಪೂರ್ಣಾಙ್ಕ options = 0;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ apply_state state;

	ಯದಿ (init_apply_state(&state, the_repository, prefix))
		exit(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	ಯದಿ (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	ಯದಿ (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_index = 0;
	}

	ಯದಿ (check_apply_state(&state, force_apply))
		exit(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	ಪ್ರತ್ಯಾಗಮ ret;
}
