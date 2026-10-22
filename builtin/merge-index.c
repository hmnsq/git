#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "sparse-index.h"

स्थिर अचर वर्ण *pgm;
स्थिर पूर्णाङ्क one_shot, quiet;
स्थिर पूर्णाङ्क err;

स्थिर पूर्णाङ्क merge_entry(पूर्णाङ्क pos, अचर वर्ण *path)
{
	पूर्णाङ्क found;
	अचर वर्ण *arguments[] = { pgm, "", "", "", path, "", "", "", रिक्त };
	वर्ण hexbuf[4][GIT_MAX_HEXSZ + 1];
	वर्ण ownbuf[4][60];
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	यदि (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	गर {
		अचर संरचना cache_entry *ce = the_repository->index->cache[pos];
		पूर्णाङ्क stage = ce_stage(ce);

		यदि (म_भेद(ce->name, path))
			रोक;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], परिमाण(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} जबसम्म (++pos < the_repository->index->cache_nr);
	यदि (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	यदि (run_command(&cmd)) {
		यदि (one_shot)
			err++;
		अन्यथा {
			यदि (!quiet)
				die("merge program failed");
			निकास(1);
		}
	}
	फर्काऊ found;
}

स्थिर शून्य merge_one_path(अचर वर्ण *path)
{
	पूर्णाङ्क pos = index_name_pos(the_repository->index, path, म_माप(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	यदि (pos < 0)
		merge_entry(-pos-1, path);
}

स्थिर शून्य merge_all(शून्य)
{
	पूर्णाङ्क i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	लागि (i = 0; i < the_repository->index->cache_nr; i++) {
		अचर संरचना cache_entry *ce = the_repository->index->cache[i];
		यदि (!ce_stage(ce))
			जारी;
		i += merge_entry(i, ce->name)-1;
	}
}

स्थिर अचर वर्ण usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

पूर्णाङ्क cmd_merge_index(पूर्णाङ्क argc,
		    अचर वर्ण **argv,
		    अचर वर्ण *prefix UNUSED,
		    संरचना repository *repo UNUSED)
{
	पूर्णाङ्क i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	सङ्केत(SIGCHLD, संक_पूर्व);

	show_usage_if_asked(argc, argv, usage_string);

	यदि (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	यदि (!म_भेद(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	यदि (!म_भेद(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	लागि (; i < argc; i++) {
		अचर वर्ण *arg = argv[i];
		यदि (!force_file && *arg == '-') {
			यदि (!म_भेद(arg, "--")) {
				force_file = 1;
				जारी;
			}
			यदि (!म_भेद(arg, "-a")) {
				merge_all();
				जारी;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	यदि (err && !quiet)
		die("merge program failed");
	फर्काऊ err;
}
