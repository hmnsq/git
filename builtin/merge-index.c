#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "sparse-index.h"

స్థిర స్థిరరాశి అక్షరం *pgm;
స్థిర పూర్ణాంకం one_shot, quiet;
స్థిర పూర్ణాంకం err;

స్థిర పూర్ణాంకం విలీనం_నమోదు(పూర్ణాంకం pos, స్థిరరాశి అక్షరం *మార్గం)
{
	పూర్ణాంకం దొరికింది_2;
	స్థిరరాశి అక్షరం *arguments[] = { pgm, "", "", "", మార్గం, "", "", "", ఖాళీ };
	అక్షరం hexbuf[4][GIT_MAX_HEXSZ + 1];
	అక్షరం ownbuf[4][60];
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

	ఒకవేళ (pos >= the_repository->సూచీ->cache_nr)
		die("git merge-index: %s not in the cache", మార్గం);
	దొరికింది_2 = 0;
	చేయి {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[pos];
		పూర్ణాంకం stage = ce_stage(ce);

		ఒకవేళ (strcmp(ce->name, మార్గం))
			ఆపు;
		దొరికింది_2++;
		oid_గమ్యం_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], పరిమాణం(ownbuf[stage]), "%o", ce->ce_బహుళకం);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} అయితే (++pos < the_repository->సూచీ->cache_nr);
	ఒకవేళ (!దొరికింది_2)
		die("git merge-index: %s not in the cache", మార్గం);

	strvec_pushv(&cmd.వాదనలు, arguments);
	ఒకవేళ (నిరంతరం_command(&cmd)) {
		ఒకవేళ (one_shot)
			err++;
		లేకపోతే {
			ఒకవేళ (!quiet)
				die("merge program failed");
			నిష్క్రమించు(1);
		}
	}
	తిరిగి దొరికింది_2;
}

స్థిర శూన్య విలీనం_one_మార్గం(స్థిరరాశి అక్షరం *మార్గం)
{
	పూర్ణాంకం pos = సూచీ_name_pos(the_repository->సూచీ, మార్గం, అక్షరశ్రేణి_పొడవు(మార్గం));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	ఒకవేళ (pos < 0)
		విలీనం_నమోదు(-pos-1, మార్గం);
}

స్థిర శూన్య విలీనం_all(శూన్య)
{
	పూర్ణాంకం i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_నిండిన_సూచీ(the_repository->సూచీ);
	కొరకు (i = 0; i < the_repository->సూచీ->cache_nr; i++) {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[i];
		ఒకవేళ (!ce_stage(ce))
			కొనసాగు;
		i += విలీనం_నమోదు(i, ce->name)-1;
	}
}

స్థిర స్థిరరాశి అక్షరం usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

పూర్ణాంకం cmd_విలీనం_సూచీ(పూర్ణాంకం argc,
		    స్థిరరాశి అక్షరం **argv,
		    స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
		    నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	సంకేతం(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	ఒకవేళ (argc < 3)
		usage(usage_string);

	repo_read_సూచీ(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_నిండిన_సూచీ(the_repository->సూచీ);

	i = 1;
	ఒకవేళ (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	ఒకవేళ (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	కొరకు (; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];
		ఒకవేళ (!force_file && *arg == '-') {
			ఒకవేళ (!strcmp(arg, "--")) {
				force_file = 1;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "-a")) {
				విలీనం_all();
				కొనసాగు;
			}
			die("git merge-index: unknown option %s", arg);
		}
		విలీనం_one_మార్గం(arg);
	}
	ఒకవేళ (err && !quiet)
		die("merge program failed");
	తిరిగి err;
}
