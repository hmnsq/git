#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "sparse-index.h"

स्थिर ध्रुव वर्ण *pgm;
स्थिर पूर्णाङ्क one_shot, quiet;
स्थिर पूर्णाङ्क err;

स्थिर पूर्णाङ्क merge_प्रविष्टिः(पूर्णाङ्क pos, ध्रुव वर्ण *मार्गः)
{
	पूर्णाङ्क प्राप्तम्_2;
	ध्रुव वर्ण *arguments[] = { pgm, "", "", "", मार्गः, "", "", "", रिक्त };
	वर्ण hexbuf[4][GIT_MAX_HEXSZ + 1];
	वर्ण ownbuf[4][60];
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	यदि (pos >= the_repository->सूचकांक->cache_nr)
		die("git merge-index: %s not in the cache", मार्गः);
	प्राप्तम्_2 = 0;
	कुरु {
		ध्रुव संरचना cache_प्रविष्टिः *ce = the_repository->सूचकांक->cache[pos];
		पूर्णाङ्क stage = ce_stage(ce);

		यदि (वाक्यं_तुलय(ce->name, मार्गः))
			भङ्ग;
		प्राप्तम्_2++;
		oid_गन्तव्यम्_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], परिमाण(ownbuf[stage]), "%o", ce->ce_बहुलकम्);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} यावत् (++pos < the_repository->सूचकांक->cache_nr);
	यदि (!प्राप्तम्_2)
		die("git merge-index: %s not in the cache", मार्गः);

	strvec_pushv(&cmd.तर्काः, arguments);
	यदि (सततम्_command(&cmd)) {
		यदि (one_shot)
			err++;
		अन्यथा {
			यदि (!quiet)
				die("merge program failed");
			निर्गच्छ(1);
		}
	}
	प्रत्यागम प्राप्तम्_2;
}

स्थिर शून्य merge_one_मार्गः(ध्रुव वर्ण *मार्गः)
{
	पूर्णाङ्क pos = सूचकांक_name_pos(the_repository->सूचकांक, मार्गः, वाक्यदैर्घ्यम्(मार्गः));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	यदि (pos < 0)
		merge_प्रविष्टिः(-pos-1, मार्गः);
}

स्थिर शून्य merge_all(शून्य)
{
	पूर्णाङ्क i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_पूर्णम्_सूचकांक(the_repository->सूचकांक);
	प्रति (i = 0; i < the_repository->सूचकांक->cache_nr; i++) {
		ध्रुव संरचना cache_प्रविष्टिः *ce = the_repository->सूचकांक->cache[i];
		यदि (!ce_stage(ce))
			अनुवर्त;
		i += merge_प्रविष्टिः(i, ce->name)-1;
	}
}

स्थिर ध्रुव वर्ण usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

पूर्णाङ्क cmd_merge_सूचकांक(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
		    संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	सङ्केत(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	यदि (argc < 3)
		usage(usage_string);

	repo_read_सूचकांक(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_पूर्णम्_सूचकांक(the_repository->सूचकांक);

	i = 1;
	यदि (!वाक्यं_तुलय(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	यदि (!वाक्यं_तुलय(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	प्रति (; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		यदि (!force_file && *arg == '-') {
			यदि (!वाक्यं_तुलय(arg, "--")) {
				force_file = 1;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "-a")) {
				merge_all();
				अनुवर्त;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_मार्गः(arg);
	}
	यदि (err && !quiet)
		die("merge program failed");
	प्रत्यागम err;
}
