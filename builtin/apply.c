#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "apply.h"

ایستا ثابت نویسه * ثابت apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	پوچ
};

صحیح cmd_apply(صحیح argc,
	      ثابت نویسه **argv,
	      ثابت نویسه *prefix,
	      ساختار repository *repo)
{
	صحیح force_apply = 0;
	صحیح options = 0;
	صحیح ret;
	ساختار apply_state state;

	اگر (init_apply_state(&state, the_repository, prefix))
		exit(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	اگر (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	اگر (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_index = 0;
	}

	اگر (check_apply_state(&state, force_apply))
		exit(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	بازگشت ret;
}
