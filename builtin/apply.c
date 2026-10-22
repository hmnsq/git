#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "apply.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	ଖାଲି
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_apply(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	      ଅଚଳ ଅକ୍ଷର **argv,
	      ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_apply = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା options = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା apply_state state;

	ଯଦି (init_apply_state(&state, the_repository, prefix))
		exit(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	ଯଦି (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	ଯଦି (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_index = 0;
	}

	ଯଦି (check_apply_state(&state, force_apply))
		exit(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	ଫେରନ୍ତୁ ret;
}
