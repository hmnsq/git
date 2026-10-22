/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#ضمن "git-compat-util.h"
#ضمن "builtin.h"
#ضمن "strvec.h"

صحيح cmd_annotate(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *اگیتر,
		 بنية repository *repo)
{
	بنية strvec دلیلاں = STRVEC_INIT;
	ثابت حرف **دلیلاں_copy;
	صحيح ret;

	strvec_pushl(&دلیلاں, "annotate", "-c", عدم);
	لكل (صحيح i = 1; i < argc; i++)
		strvec_ڈالو(&دلیلاں, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(دلیلاں_copy, دلیلاں.nr + 1);
	COPY_ARRAY(دلیلاں_copy, دلیلاں.v, دلیلاں.nr);

	ret = cmd_blame(دلیلاں.nr, دلیلاں_copy, اگیتر, repo);

	strvec_clear(&دلیلاں);
	حرر(دلیلاں_copy);
	أرجع ret;
}
