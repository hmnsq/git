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
		 ثابت حرف *اڳياڙي,
		 بنية repository *repo)
{
	بنية strvec دليلون = STRVEC_INIT;
	ثابت حرف **دليلون_copy;
	صحيح ret;

	strvec_pushl(&دليلون, "annotate", "-c", عدم);
	لكل (صحيح i = 1; i < argc; i++)
		strvec_ڌڪو(&دليلون, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(دليلون_copy, دليلون.nr + 1);
	COPY_ARRAY(دليلون_copy, دليلون.v, دليلون.nr);

	ret = cmd_blame(دليلون.nr, دليلون_copy, اڳياڙي, repo);

	strvec_clear(&دليلون);
	حرر(دليلون_copy);
	أرجع ret;
}
