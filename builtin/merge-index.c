#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "hex.h"
#شامل "read-cache-ll.h"
#شامل "run-command.h"
#شامل "sparse-index.h"

ایستا ثابت نویسه *pgm;
ایستا صحیح one_shot, quiet;
ایستا صحیح err;

ایستا صحیح merge_entry(صحیح pos, ثابت نویسه *path)
{
	صحیح found;
	ثابت نویسه *arguments[] = { pgm, "", "", "", path, "", "", "", پوچ };
	نویسه hexbuf[4][GIT_MAX_HEXSZ + 1];
	نویسه ownbuf[4][60];
	ساختار child_process cmd = CHILD_PROCESS_INIT;

	اگر (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	انجامبده {
		ثابت ساختار cache_entry *ce = the_repository->index->cache[pos];
		صحیح stage = ce_stage(ce);

		اگر (strcmp(ce->name, path))
			بشکن;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], اندازه(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} وقتی (++pos < the_repository->index->cache_nr);
	اگر (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	اگر (run_command(&cmd)) {
		اگر (one_shot)
			err++;
		وگرنه {
			اگر (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	بازگشت found;
}

ایستا تهی merge_one_path(ثابت نویسه *path)
{
	صحیح pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	اگر (pos < 0)
		merge_entry(-pos-1, path);
}

ایستا تهی merge_all(تهی)
{
	صحیح i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	برای (i = 0; i < the_repository->index->cache_nr; i++) {
		ثابت ساختار cache_entry *ce = the_repository->index->cache[i];
		اگر (!ce_stage(ce))
			ادامه;
		i += merge_entry(i, ce->name)-1;
	}
}

ایستا ثابت نویسه usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

صحیح cmd_merge_index(صحیح argc,
		    ثابت نویسه **argv,
		    ثابت نویسه *prefix UNUSED,
		    ساختار repository *repo UNUSED)
{
	صحیح i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	سیگنال(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	اگر (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	اگر (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	اگر (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	برای (; i < argc; i++) {
		ثابت نویسه *arg = argv[i];
		اگر (!force_file && *arg == '-') {
			اگر (!strcmp(arg, "--")) {
				force_file = 1;
				ادامه;
			}
			اگر (!strcmp(arg, "-a")) {
				merge_all();
				ادامه;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	اگر (err && !quiet)
		die("merge program failed");
	بازگشت err;
}
