#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "apply.h"

நிலையான மாறிலி எழுத்து * மாறிலி apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	வெற்று
};

முழு cmd_apply(முழு argc,
	      மாறிலி எழுத்து **argv,
	      மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repo)
{
	முழு force_apply = 0;
	முழு options = 0;
	முழு ret;
	கட்டமைப்பு apply_state state;

	எனில் (init_apply_state(&state, the_repository, prefix))
		exit(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	எனில் (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	எனில் (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_index = 0;
	}

	எனில் (check_apply_state(&state, force_apply))
		exit(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	திரும்பு ret;
}
