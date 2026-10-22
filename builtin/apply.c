#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "apply.h"

ساكن ثابت حرف * ثابت apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	عدم
};

صحيح cmd_apply(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *prefix,
	      بنية repository *repo)
{
	صحيح force_apply = 0;
	صحيح options = 0;
	صحيح ret;
	بنية apply_state state;

	ܐܢ (init_apply_state(&state, the_repository, prefix))
		exit(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	ܐܢ (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	ܐܢ (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_index = 0;
	}

	ܐܢ (check_apply_state(&state, force_apply))
		exit(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	ܦܢܐ ret;
}
