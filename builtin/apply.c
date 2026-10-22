#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "gettext.h"
#समावेश "hash.h"
#समावेश "apply.h"

अटल स्थिर अक्षर * स्थिर apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	शून्य
};

पूर्णांक cmd_apply(पूर्णांक argc,
	      स्थिर अक्षर **argv,
	      स्थिर अक्षर *prefix,
	      काष्ठा repository *repo)
{
	पूर्णांक force_apply = 0;
	पूर्णांक options = 0;
	पूर्णांक ret;
	काष्ठा apply_state state;

	अगर (init_apply_state(&state, the_repository, prefix))
		निकास(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	अगर (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	अगर (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_सूचकांक = 0;
	}

	अगर (check_apply_state(&state, force_apply))
		निकास(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	वापस ret;
}
