#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "sparse-index.h"

நிலையான மாறிலி எழுத்து *pgm;
நிலையான முழு one_shot, quiet;
நிலையான முழு err;

நிலையான முழு merge_entry(முழு pos, மாறிலி எழுத்து *path)
{
	முழு found;
	மாறிலி எழுத்து *arguments[] = { pgm, "", "", "", path, "", "", "", வெற்று };
	எழுத்து hexbuf[4][GIT_MAX_HEXSZ + 1];
	எழுத்து ownbuf[4][60];
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

	எனில் (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	செய் {
		மாறிலி கட்டமைப்பு cache_entry *ce = the_repository->index->cache[pos];
		முழு stage = ce_stage(ce);

		எனில் (strcmp(ce->name, path))
			நிறுத்து;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], அளவு(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} போது (++pos < the_repository->index->cache_nr);
	எனில் (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	எனில் (run_command(&cmd)) {
		எனில் (one_shot)
			err++;
		இல்லையெனில் {
			எனில் (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	திரும்பு found;
}

நிலையான வெறுமை merge_one_path(மாறிலி எழுத்து *path)
{
	முழு pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	எனில் (pos < 0)
		merge_entry(-pos-1, path);
}

நிலையான வெறுமை merge_all(வெறுமை)
{
	முழு i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	க்கு (i = 0; i < the_repository->index->cache_nr; i++) {
		மாறிலி கட்டமைப்பு cache_entry *ce = the_repository->index->cache[i];
		எனில் (!ce_stage(ce))
			தொடர்;
		i += merge_entry(i, ce->name)-1;
	}
}

நிலையான மாறிலி எழுத்து usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

முழு cmd_merge_index(முழு argc,
		    மாறிலி எழுத்து **argv,
		    மாறிலி எழுத்து *prefix UNUSED,
		    கட்டமைப்பு repository *repo UNUSED)
{
	முழு i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	சமிக்ஞை(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	எனில் (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	எனில் (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	எனில் (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	க்கு (; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];
		எனில் (!force_file && *arg == '-') {
			எனில் (!strcmp(arg, "--")) {
				force_file = 1;
				தொடர்;
			}
			எனில் (!strcmp(arg, "-a")) {
				merge_all();
				தொடர்;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	எனில் (err && !quiet)
		die("merge program failed");
	திரும்பு err;
}
