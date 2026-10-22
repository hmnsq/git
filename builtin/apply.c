#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "apply.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	ശൂന്യ
};

പൂർണ്ണസംഖ്യ cmd_apply(പൂർണ്ണസംഖ്യ argc,
	      സ്ഥിരം അക്ഷരം **argv,
	      സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ force_apply = 0;
	പൂർണ്ണസംഖ്യ options = 0;
	പൂർണ്ണസംഖ്യ ret;
	ഘടന apply_state state;

	എങ്കിൽ (init_apply_state(&state, the_repository, prefix))
		exit(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	എങ്കിൽ (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	എങ്കിൽ (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_full_index = 0;
	}

	എങ്കിൽ (check_apply_state(&state, force_apply))
		exit(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	തിരികെ ret;
}
