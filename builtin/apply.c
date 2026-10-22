#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "apply.h"

स्थिर स्थिरांक अक्षर * स्थिरांक apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	रिक्त
};

पूर्णांक cmd_apply(पूर्णांक argc,
	      स्थिरांक अक्षर **argv,
	      स्थिरांक अक्षर *prefix,
	      संरचना repository *repo)
{
	पूर्णांक force_apply = 0;
	पूर्णांक options = 0;
	पूर्णांक ret;
	संरचना apply_state state;

	जर (init_apply_state(&state, the_repository, prefix))
		निकास(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	जर (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	जर (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_index = 0;
	}

	जर (check_apply_state(&state, force_apply))
		निकास(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	परत ret;
}
