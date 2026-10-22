#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "hex.h"
#ضمن "read-cache-ll.h"
#ضمن "run-command.h"
#ضمن "sparse-index.h"

ساكن ثابت حرف *pgm;
ساكن صحيح one_shot, quiet;
ساكن صحيح err;

ساكن صحيح merge_entry(صحيح pos, ثابت حرف *path)
{
	صحيح found;
	ثابت حرف *arguments[] = { pgm, "", "", "", path, "", "", "", عدم };
	حرف hexbuf[4][GIT_MAX_HEXSZ + 1];
	حرف ownbuf[4][60];
	بنية child_process cmd = CHILD_PROCESS_INIT;

	إذا (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	افعل {
		ثابت بنية cache_entry *ce = the_repository->index->cache[pos];
		صحيح stage = ce_stage(ce);

		إذا (strcmp(ce->name, path))
			اكسر;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], حجم(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} طالما (++pos < the_repository->index->cache_nr);
	إذا (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.حجوجي, arguments);
	إذا (run_command(&cmd)) {
		إذا (one_shot)
			err++;
		وإلا {
			إذا (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	أرجع found;
}

ساكن فراغ merge_one_path(ثابت حرف *path)
{
	صحيح pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	إذا (pos < 0)
		merge_entry(-pos-1, path);
}

ساكن فراغ merge_all(فراغ)
{
	صحيح i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	لكل (i = 0; i < the_repository->index->cache_nr; i++) {
		ثابت بنية cache_entry *ce = the_repository->index->cache[i];
		إذا (!ce_stage(ce))
			استمر;
		i += merge_entry(i, ce->name)-1;
	}
}

ساكن ثابت حرف usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

صحيح cmd_merge_index(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *prefix UNUSED,
		    بنية repository *repo UNUSED)
{
	صحيح i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	إشارة(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	إذا (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	إذا (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	إذا (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	لكل (; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		إذا (!force_file && *arg == '-') {
			إذا (!strcmp(arg, "--")) {
				force_file = 1;
				استمر;
			}
			إذا (!strcmp(arg, "-a")) {
				merge_all();
				استمر;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	إذا (err && !quiet)
		die("merge program failed");
	أرجع err;
}
