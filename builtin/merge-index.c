#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pgm;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ one_shot, quiet;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ err;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಲೀನ_ನಮೂದು(ಪೂರ್ಣಾಂಕ pos, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು_2;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arguments[] = { pgm, "", "", "", ಮಾರ್ಗ, "", "", "", ಖಾಲಿ };
	ಅಕ್ಷರ hexbuf[4][GIT_MAX_HEXSZ + 1];
	ಅಕ್ಷರ ownbuf[4][60];
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

	ಆದರೆ (pos >= the_repository->ಸೂಚ್ಯಂಕ->cache_nr)
		die("git merge-index: %s not in the cache", ಮಾರ್ಗ);
	ಸಿಕ್ಕಿತು_2 = 0;
	ಮಾಡು {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
		ಪೂರ್ಣಾಂಕ stage = ce_stage(ce);

		ಆದರೆ (strcmp(ce->name, ಮಾರ್ಗ))
			ನಿಲ್ಲಿಸು;
		ಸಿಕ್ಕಿತು_2++;
		oid_ಗಮ್ಯ_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], ಗಾತ್ರ(ownbuf[stage]), "%o", ce->ce_ಬಹುಲಕ);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} ಆಗ (++pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr);
	ಆದರೆ (!ಸಿಕ್ಕಿತು_2)
		die("git merge-index: %s not in the cache", ಮಾರ್ಗ);

	strvec_pushv(&cmd.ವಾದಗಳು, arguments);
	ಆದರೆ (ಸತತ_command(&cmd)) {
		ಆದರೆ (one_shot)
			err++;
		ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (!quiet)
				die("merge program failed");
			ನಿರ್ಗಮಿಸು(1);
		}
	}
	ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು_2;
}

ಸ್ಥಿರ ಶೂನ್ಯ ವಿಲೀನ_one_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಪೂರ್ಣಾಂಕ pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	ಆದರೆ (pos < 0)
		ವಿಲೀನ_ನಮೂದು(-pos-1, ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ವಿಲೀನ_all(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	ಗಾಗಿ (i = 0; i < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[i];
		ಆದರೆ (!ce_stage(ce))
			ಮುಂದುವರಿ;
		i += ವಿಲೀನ_ನಮೂದು(i, ce->name)-1;
	}
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

ಪೂರ್ಣಾಂಕ cmd_ವಿಲೀನ_ಸೂಚ್ಯಂಕ(ಪೂರ್ಣಾಂಕ argc,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
		    ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	ಸಂಕೇತ(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	ಆದರೆ (argc < 3)
		usage(usage_string);

	repo_read_ಸೂಚ್ಯಂಕ(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);

	i = 1;
	ಆದರೆ (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	ಆದರೆ (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	ಗಾಗಿ (; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];
		ಆದರೆ (!force_file && *arg == '-') {
			ಆದರೆ (!strcmp(arg, "--")) {
				force_file = 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "-a")) {
				ವಿಲೀನ_all();
				ಮುಂದುವರಿ;
			}
			die("git merge-index: unknown option %s", arg);
		}
		ವಿಲೀನ_one_ಮಾರ್ಗ(arg);
	}
	ಆದರೆ (err && !quiet)
		die("merge program failed");
	ಹಿಂತಿರುಗಿ err;
}
