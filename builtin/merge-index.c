#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "hex.h"
#شامل "read-cache-ll.h"
#شامل "run-command.h"
#شامل "sparse-index.h"

ساکن ثابت حرف *pgm;
ساکن صحیح one_shot, quiet;
ساکن صحیح err;

ساکن صحیح ملاؤ_اندراج(صحیح pos, ثابت حرف *راستہ)
{
	صحیح ملا_2;
	ثابت حرف *arguments[] = { pgm, "", "", "", راستہ, "", "", "", تہی };
	حرف hexbuf[4][GIT_MAX_HEXSZ + 1];
	حرف ownbuf[4][60];
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

	اگر (pos >= the_repository->اشاریہ->cache_nr)
		die("git merge-index: %s not in the cache", راستہ);
	ملا_2 = 0;
	کرو {
		ثابت ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[pos];
		صحیح stage = ce_stage(ce);

		اگر (strcmp(ce->name, راستہ))
			توڑو;
		ملا_2++;
		oid_مقصد_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], حجم(ownbuf[stage]), "%o", ce->ce_منوال);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} جب (++pos < the_repository->اشاریہ->cache_nr);
	اگر (!ملا_2)
		die("git merge-index: %s not in the cache", راستہ);

	strvec_pushv(&cmd.دلائل, arguments);
	اگر (تسلسل_command(&cmd)) {
		اگر (one_shot)
			err++;
		ورنہ {
			اگر (!quiet)
				die("merge program failed");
			باہر_نکلو(1);
		}
	}
	واپس ملا_2;
}

ساکن خالی ملاؤ_one_راستہ(ثابت حرف *راستہ)
{
	صحیح pos = اشاریہ_name_pos(the_repository->اشاریہ, راستہ, لڑی_لمبائی(راستہ));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	اگر (pos < 0)
		ملاؤ_اندراج(-pos-1, راستہ);
}

ساکن خالی ملاؤ_all(خالی)
{
	صحیح i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_بھرا_اشاریہ(the_repository->اشاریہ);
	برائے (i = 0; i < the_repository->اشاریہ->cache_nr; i++) {
		ثابت ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[i];
		اگر (!ce_stage(ce))
			جاری;
		i += ملاؤ_اندراج(i, ce->name)-1;
	}
}

ساکن ثابت حرف usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

صحیح cmd_ملاؤ_اشاریہ(صحیح argc,
		    ثابت حرف **argv,
		    ثابت حرف *سابقہ غیر_مستعمل,
		    ساخت repository *repo غیر_مستعمل)
{
	صحیح i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	اشارہ(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	اگر (argc < 3)
		usage(usage_string);

	repo_read_اشاریہ(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_بھرا_اشاریہ(the_repository->اشاریہ);

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
	برائے (; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		اگر (!force_file && *arg == '-') {
			اگر (!strcmp(arg, "--")) {
				force_file = 1;
				جاری;
			}
			اگر (!strcmp(arg, "-a")) {
				ملاؤ_all();
				جاری;
			}
			die("git merge-index: unknown option %s", arg);
		}
		ملاؤ_one_راستہ(arg);
	}
	اگر (err && !quiet)
		die("merge program failed");
	واپس err;
}
