#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *pgm;
ਸਥਿਰ ਪੂਰਨ one_shot, quiet;
ਸਥਿਰ ਪੂਰਨ err;

ਸਥਿਰ ਪੂਰਨ ਮਿਲਾਓ_ਇੰਦਰਾਜ(ਪੂਰਨ pos, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਪੂਰਨ ਮਿਲਿਆ_2;
	ਅਚਲ ਅੱਖਰ *arguments[] = { pgm, "", "", "", ਰਸਤਾ, "", "", "", ਨਿਰਾ };
	ਅੱਖਰ hexbuf[4][GIT_MAX_HEXSZ + 1];
	ਅੱਖਰ ownbuf[4][60];
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

	ਜੇ (pos >= the_repository->ਸੂਚਕਾਂਕ->cache_nr)
		die("git merge-index: %s not in the cache", ਰਸਤਾ);
	ਮਿਲਿਆ_2 = 0;
	ਕਰੋ {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
		ਪੂਰਨ stage = ce_stage(ce);

		ਜੇ (strcmp(ce->name, ਰਸਤਾ))
			ਰੋਕੋ;
		ਮਿਲਿਆ_2++;
		oid_ਮੰਜ਼ਿਲ_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], ਆਕਾਰ(ownbuf[stage]), "%o", ce->ce_ਬਹੁਲਕ);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} ਜਦੋਂ (++pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr);
	ਜੇ (!ਮਿਲਿਆ_2)
		die("git merge-index: %s not in the cache", ਰਸਤਾ);

	strvec_pushv(&cmd.ਦਲੀਲਾਂ, arguments);
	ਜੇ (ਲੜੀਵਾਰ_command(&cmd)) {
		ਜੇ (one_shot)
			err++;
		ਨਹੀਂਤਾਂ {
			ਜੇ (!quiet)
				die("merge program failed");
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
	}
	ਵਾਪਸ ਮਿਲਿਆ_2;
}

ਸਥਿਰ ਖਾਲੀ ਮਿਲਾਓ_one_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਪੂਰਨ pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ, ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	ਜੇ (pos < 0)
		ਮਿਲਾਓ_ਇੰਦਰਾਜ(-pos-1, ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ ਮਿਲਾਓ_all(ਖਾਲੀ)
{
	ਪੂਰਨ i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	ਲਈ (i = 0; i < the_repository->ਸੂਚਕਾਂਕ->cache_nr; i++) {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[i];
		ਜੇ (!ce_stage(ce))
			ਜਾਰੀ;
		i += ਮਿਲਾਓ_ਇੰਦਰਾਜ(i, ce->name)-1;
	}
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

ਪੂਰਨ cmd_ਮਿਲਾਓ_ਸੂਚਕਾਂਕ(ਪੂਰਨ argc,
		    ਅਚਲ ਅੱਖਰ **argv,
		    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
		    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	ਸੰਕੇਤ(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	ਜੇ (argc < 3)
		usage(usage_string);

	repo_read_ਸੂਚਕਾਂਕ(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);

	i = 1;
	ਜੇ (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	ਜੇ (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	ਲਈ (; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];
		ਜੇ (!force_file && *arg == '-') {
			ਜੇ (!strcmp(arg, "--")) {
				force_file = 1;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "-a")) {
				ਮਿਲਾਓ_all();
				ਜਾਰੀ;
			}
			die("git merge-index: unknown option %s", arg);
		}
		ਮਿਲਾਓ_one_ਰਸਤਾ(arg);
	}
	ਜੇ (err && !quiet)
		die("merge program failed");
	ਵਾਪਸ err;
}
