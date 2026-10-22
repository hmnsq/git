#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "sparse-index.h"

स्थिर स्थिरांक अक्षर *pgm;
स्थिर पूर्णांक one_shot, quiet;
स्थिर पूर्णांक err;

स्थिर पूर्णांक merge_entry(पूर्णांक pos, स्थिरांक अक्षर *path)
{
	पूर्णांक found;
	स्थिरांक अक्षर *arguments[] = { pgm, "", "", "", path, "", "", "", रिक्त };
	अक्षर hexbuf[4][GIT_MAX_HEXSZ + 1];
	अक्षर ownbuf[4][60];
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	जर (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	करा {
		स्थिरांक संरचना cache_entry *ce = the_repository->index->cache[pos];
		पूर्णांक stage = ce_stage(ce);

		जर (म_भेद(ce->name, path))
			थांबा;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], आकार(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} पर्यंत (++pos < the_repository->index->cache_nr);
	जर (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	जर (run_command(&cmd)) {
		जर (one_shot)
			err++;
		अन्यथा {
			जर (!quiet)
				die("merge program failed");
			निकास(1);
		}
	}
	परत found;
}

स्थिर शून्य merge_one_path(स्थिरांक अक्षर *path)
{
	पूर्णांक pos = index_name_pos(the_repository->index, path, म_माप(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	जर (pos < 0)
		merge_entry(-pos-1, path);
}

स्थिर शून्य merge_all(शून्य)
{
	पूर्णांक i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	साठी (i = 0; i < the_repository->index->cache_nr; i++) {
		स्थिरांक संरचना cache_entry *ce = the_repository->index->cache[i];
		जर (!ce_stage(ce))
			सुरूठेवा;
		i += merge_entry(i, ce->name)-1;
	}
}

स्थिर स्थिरांक अक्षर usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

पूर्णांक cmd_merge_index(पूर्णांक argc,
		    स्थिरांक अक्षर **argv,
		    स्थिरांक अक्षर *prefix UNUSED,
		    संरचना repository *repo UNUSED)
{
	पूर्णांक i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	संकेत(SIGCHLD, संक_पूर्व);

	show_usage_if_asked(argc, argv, usage_string);

	जर (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	जर (!म_भेद(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	जर (!म_भेद(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	साठी (; i < argc; i++) {
		स्थिरांक अक्षर *arg = argv[i];
		जर (!force_file && *arg == '-') {
			जर (!म_भेद(arg, "--")) {
				force_file = 1;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "-a")) {
				merge_all();
				सुरूठेवा;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	जर (err && !quiet)
		die("merge program failed");
	परत err;
}
