#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "apply.h"

স্থির ধ্রুব বর্ণ * ধ্রুব apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	রিক্ত
};

পূর্ণাঙ্ক cmd_apply(পূর্ণাঙ্ক argc,
	      ধ্রুব বর্ণ **argv,
	      ধ্রুব বর্ণ *prefix,
	      সংরচনা repository *repo)
{
	পূর্ণাঙ্ক force_apply = 0;
	পূর্ণাঙ্ক options = 0;
	পূর্ণাঙ্ক ret;
	সংরচনা apply_state state;

	যদি (init_apply_state(&state, the_repository, prefix))
		exit(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	যদি (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	যদি (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_index = 0;
	}

	যদি (check_apply_state(&state, force_apply))
		exit(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	প্রত্যাগম ret;
}
