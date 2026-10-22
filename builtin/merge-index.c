#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *pgm;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା one_shot, quiet;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge_entry(ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos, ଅଚଳ ଅକ୍ଷର *path)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା found;
	ଅଚଳ ଅକ୍ଷର *arguments[] = { pgm, "", "", "", path, "", "", "", ଖାଲି };
	ଅକ୍ଷର hexbuf[4][GIT_MAX_HEXSZ + 1];
	ଅକ୍ଷର ownbuf[4][60];
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

	ଯଦି (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	କର {
		ଅଚଳ ସଂରଚନା cache_entry *ce = the_repository->index->cache[pos];
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା stage = ce_stage(ce);

		ଯଦି (strcmp(ce->name, path))
			ଭାଙ୍ଗ;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], ଆକାର(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} ଯେତେବେଳେ (++pos < the_repository->index->cache_nr);
	ଯଦି (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	ଯଦି (run_command(&cmd)) {
		ଯଦି (one_shot)
			err++;
		ଅନ୍ୟଥା {
			ଯଦି (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	ଫେରନ୍ତୁ found;
}

ସ୍ଥିର ଶୂନ୍ୟ merge_one_path(ଅଚଳ ଅକ୍ଷର *path)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	ଯଦି (pos < 0)
		merge_entry(-pos-1, path);
}

ସ୍ଥିର ଶୂନ୍ୟ merge_all(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	ପାଇଁ (i = 0; i < the_repository->index->cache_nr; i++) {
		ଅଚଳ ସଂରଚନା cache_entry *ce = the_repository->index->cache[i];
		ଯଦି (!ce_stage(ce))
			ଜାରି;
		i += merge_entry(i, ce->name)-1;
	}
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_merge_index(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		    ଅଚଳ ଅକ୍ଷର **argv,
		    ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
		    ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	ସଙ୍କେତ(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	ଯଦି (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	ଯଦି (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	ଯଦି (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	ପାଇଁ (; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];
		ଯଦି (!force_file && *arg == '-') {
			ଯଦି (!strcmp(arg, "--")) {
				force_file = 1;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "-a")) {
				merge_all();
				ଜାରି;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	ଯଦି (err && !quiet)
		die("merge program failed");
	ଫେରନ୍ତୁ err;
}
