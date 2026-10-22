#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "apply.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి apply_usage[] = {
	N_("git apply [<options>] [<patch>...]"),
	ఖాళీ
};

పూర్ణాంకం cmd_apply(పూర్ణాంకం argc,
	      స్థిరరాశి అక్షరం **argv,
	      స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repo)
{
	పూర్ణాంకం force_apply = 0;
	పూర్ణాంకం options = 0;
	పూర్ణాంకం ret;
	నిర్మాణం apply_state state;

	ఒకవేళ (init_apply_state(&state, the_repository, ఉపసర్గ))
		నిష్క్రమించు(128);

	/*
	 * We could to redo the "apply.c" machinery to make this
	 * arbitrary fallback unnecessary, but it is dubious that it
	 * is worth the effort.
	 * cf. https://lore.kernel.org/git/xmqqcypfcmn4.fsf@gitster.g/
	 */
	ఒకవేళ (!the_hash_algo)
		repo_సమితి_hash_algo(the_repository, GIT_HASH_DEFAULT);

	argc = apply_parse_options(argc, argv,
				   &state, &force_apply, &options,
				   apply_usage);

	ఒకవేళ (repo) {
		prepare_repo_settings(repo);
		repo->settings.command_requires_నిండిన_సూచీ = 0;
	}

	ఒకవేళ (check_apply_state(&state, force_apply))
		నిష్క్రమించు(128);

	ret = apply_all_patches(&state, argc, argv, options);

	clear_apply_state(&state);

	తిరిగి ret;
}
