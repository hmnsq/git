#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "apply.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	ফাঁকা
};

পূর্ণসংখ্যা cmd_apply(পূর্ণসংখ্যা argc,
	      ধ্রুবক অক্ষর **argv,
	      ধ্রুবক অক্ষর *prefix,
	      কাঠামো repository *repo)
{
	পূর্ণসংখ্যা force_apply = 0;
	পূর্ণসংখ্যা options = 0;
	পূর্ণসংখ্যা ret;
	কাঠামো apply_state state;

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
		repo->settings.command_requires_full_সূচক = 0;
	}

	যদি (check_apply_state(&state, force_apply))
		exit(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	ফেরত ret;
}
