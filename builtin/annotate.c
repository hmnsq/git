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
		 ثابت حرف *prefix,
		 بنية repository *repo)
{
	بنية strvec حجوجي = STRVEC_INIT;
	ثابت حرف **حجوجي_copy;
	صحيح ret;

	strvec_pushl(&حجوجي, "annotate", "-c", عدم);
	لكل (صحيح i = 1; i < argc; i++)
		strvec_تور(&حجوجي, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(حجوجي_copy, حجوجي.nr + 1);
	COPY_ARRAY(حجوجي_copy, حجوجي.v, حجوجي.nr);

	ret = cmd_blame(حجوجي.nr, حجوجي_copy, prefix, repo);

	strvec_clear(&حجوجي);
	حرر(حجوجي_copy);
	أرجع ret;
}
