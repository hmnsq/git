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
		 ثابت حرف *مختاړی,
		 بنية repository *repo)
{
	بنية strvec دلیلونه = STRVEC_INIT;
	ثابت حرف **دلیلونه_copy;
	صحيح ret;

	strvec_pushl(&دلیلونه, "annotate", "-c", عدم);
	لكل (صحيح i = 1; i < argc; i++)
		strvec_ټېل(&دلیلونه, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(دلیلونه_copy, دلیلونه.nr + 1);
	COPY_ARRAY(دلیلونه_copy, دلیلونه.v, دلیلونه.nr);

	ret = cmd_blame(دلیلونه.nr, دلیلونه_copy, مختاړی, repo);

	strvec_clear(&دلیلونه);
	حرر(دلیلونه_copy);
	أرجع ret;
}
