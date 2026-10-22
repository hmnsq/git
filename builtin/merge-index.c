#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "hex.h"
#ضمن "read-cache-ll.h"
#ضمن "run-command.h"
#ضمن "sparse-index.h"

ساكن ثابت حرف *pgm;
ساكن صحيح one_shot, quiet;
ساكن صحيح err;

ساكن صحيح یوځای_ثبت(صحيح pos, ثابت حرف *لار)
{
	صحيح وموندل_2;
	ثابت حرف *arguments[] = { pgm, "", "", "", لار, "", "", "", عدم };
	حرف hexbuf[4][GIT_MAX_HEXSZ + 1];
	حرف ownbuf[4][60];
	بنية ماشوم_process cmd = ماشوم_PROCESS_INIT;

	إذا (pos >= the_repository->شاخص->cache_nr)
		die("git merge-index: %s not in the cache", لار);
	وموندل_2 = 0;
	افعل {
		ثابت بنية cache_ثبت *ce = the_repository->شاخص->cache[pos];
		صحيح stage = ce_stage(ce);

		إذا (strcmp(ce->name, لار))
			اكسر;
		وموندل_2++;
		oid_منزل_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], حجم(ownbuf[stage]), "%o", ce->ce_ډېرپېښ);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} طالما (++pos < the_repository->شاخص->cache_nr);
	إذا (!وموندل_2)
		die("git merge-index: %s not in the cache", لار);

	strvec_pushv(&cmd.دلیلونه, arguments);
	إذا (پرلپسې_command(&cmd)) {
		إذا (one_shot)
			err++;
		وإلا {
			إذا (!quiet)
				die("merge program failed");
			وتل(1);
		}
	}
	أرجع وموندل_2;
}

ساكن فراغ یوځای_one_لار(ثابت حرف *لار)
{
	صحيح pos = شاخص_name_pos(the_repository->شاخص, لار, مزي_اوږدوالی(لار));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	إذا (pos < 0)
		یوځای_ثبت(-pos-1, لار);
}

ساكن فراغ یوځای_all(فراغ)
{
	صحيح i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_ډک_شاخص(the_repository->شاخص);
	لكل (i = 0; i < the_repository->شاخص->cache_nr; i++) {
		ثابت بنية cache_ثبت *ce = the_repository->شاخص->cache[i];
		إذا (!ce_stage(ce))
			استمر;
		i += یوځای_ثبت(i, ce->name)-1;
	}
}

ساكن ثابت حرف usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

صحيح cmd_یوځای_شاخص(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *مختاړی ناکارول_شوی,
		    بنية repository *repo ناکارول_شوی)
{
	صحيح i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	إشارة(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	إذا (argc < 3)
		usage(usage_string);

	repo_read_شاخص(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_ډک_شاخص(the_repository->شاخص);

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
				یوځای_all();
				استمر;
			}
			die("git merge-index: unknown option %s", arg);
		}
		یوځای_one_لار(arg);
	}
	إذا (err && !quiet)
		die("merge program failed");
	أرجع err;
}
