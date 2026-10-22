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
	بنية strvec وسائط = STRVEC_INIT;
	ثابت حرف **وسائط_copy;
	صحيح ret;

	strvec_pushl(&وسائط, "annotate", "-c", عدم);
	لكل (صحيح i = 1; i < argc; i++)
		strvec_ادفع(&وسائط, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(وسائط_copy, وسائط.nr + 1);
	COPY_ARRAY(وسائط_copy, وسائط.v, وسائط.nr);

	ret = cmd_blame(وسائط.nr, وسائط_copy, prefix, repo);

	strvec_clear(&وسائط);
	حرر(وسائط_copy);
	أرجع ret;
}
