#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "apply.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	ಖಾಲಿ
};

ಪೂರ್ಣಾಂಕ cmd_apply(ಪೂರ್ಣಾಂಕ argc,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ force_apply = 0;
	ಪೂರ್ಣಾಂಕ options = 0;
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ apply_state state;

	ಆದರೆ (init_apply_state(&state, the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ))
		ನಿರ್ಗಮಿಸು(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	ಆದರೆ (!the_hash_algo)
		repo_ಗಣ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	ಆದರೆ (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;
	}

	ಆದರೆ (check_apply_state(&state, force_apply))
		ನಿರ್ಗಮಿಸು(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	ಹಿಂತಿರುಗಿ ret;
}
