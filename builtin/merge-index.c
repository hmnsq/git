#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "sparse-index.h"

স্থির ধ্রুবক অক্ষর *pgm;
স্থির পূর্ণসংখ্যা one_shot, quiet;
স্থির পূর্ণসংখ্যা err;

স্থির পূর্ণসংখ্যা merge_entry(পূর্ণসংখ্যা pos, ধ্রুবক অক্ষর *path)
{
	পূর্ণসংখ্যা found;
	ধ্রুবক অক্ষর *arguments[] = { pgm, "", "", "", path, "", "", "", ফাঁকা };
	অক্ষর hexbuf[4][GIT_MAX_HEXSZ + 1];
	অক্ষর ownbuf[4][60];
	কাঠামো child_process cmd = CHILD_PROCESS_INIT;

	যদি (pos >= the_repository->সূচক->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	করো {
		ধ্রুবক কাঠামো cache_entry *ce = the_repository->সূচক->cache[pos];
		পূর্ণসংখ্যা stage = ce_stage(ce);

		যদি (strcmp(ce->name, path))
			বিরতি;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], আকার(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} যখন (++pos < the_repository->সূচক->cache_nr);
	যদি (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	যদি (run_command(&cmd)) {
		যদি (one_shot)
			err++;
		অন্যথা {
			যদি (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	ফেরত found;
}

স্থির শূন্য merge_one_path(ধ্রুবক অক্ষর *path)
{
	পূর্ণসংখ্যা pos = সূচক_name_pos(the_repository->সূচক, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	যদি (pos < 0)
		merge_entry(-pos-1, path);
}

স্থির শূন্য merge_all(শূন্য)
{
	পূর্ণসংখ্যা i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_সূচক(the_repository->সূচক);
	জন্য (i = 0; i < the_repository->সূচক->cache_nr; i++) {
		ধ্রুবক কাঠামো cache_entry *ce = the_repository->সূচক->cache[i];
		যদি (!ce_stage(ce))
			চালু;
		i += merge_entry(i, ce->name)-1;
	}
}

স্থির ধ্রুবক অক্ষর usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

পূর্ণসংখ্যা cmd_merge_সূচক(পূর্ণসংখ্যা argc,
		    ধ্রুবক অক্ষর **argv,
		    ধ্রুবক অক্ষর *prefix UNUSED,
		    কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	সংকেত(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	যদি (argc < 3)
		usage(usage_string);

	repo_read_সূচক(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_সূচক(the_repository->সূচক);

	i = 1;
	যদি (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	যদি (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	জন্য (; i < argc; i++) {
		ধ্রুবক অক্ষর *arg = argv[i];
		যদি (!force_file && *arg == '-') {
			যদি (!strcmp(arg, "--")) {
				force_file = 1;
				চালু;
			}
			যদি (!strcmp(arg, "-a")) {
				merge_all();
				চালু;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	যদি (err && !quiet)
		die("merge program failed");
	ফেরত err;
}
