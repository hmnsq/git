#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "apply.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	ਨਿਰਾ
};

ਪੂਰਨ cmd_apply(ਪੂਰਨ argc,
	      ਅਚਲ ਅੱਖਰ **argv,
	      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ force_apply = 0;
	ਪੂਰਨ options = 0;
	ਪੂਰਨ ret;
	ਬਣਤਰ apply_state state;

	ਜੇ (init_apply_state(&state, the_repository, ਅਗੇਤਰ))
		ਬਾਹਰ_ਨਿਕਲੋ(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	ਜੇ (!the_hash_algo)
		repo_ਸੈੱਟ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	ਜੇ (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;
	}

	ਜੇ (check_apply_state(&state, force_apply))
		ਬਾਹਰ_ਨਿਕਲੋ(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	ਵਾਪਸ ret;
}
