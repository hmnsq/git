/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#شامل "git-compat-util.h"
#شامل "builtin.h"
#شامل "strvec.h"

صحیح cmd_annotate(صحیح argc,
		 ثابت حرف **argv,
		 ثابت حرف *سابقہ,
		 ساخت repository *repo)
{
	ساخت strvec دلائل = STRVEC_INIT;
	ثابت حرف **دلائل_copy;
	صحیح ret;

	strvec_pushl(&دلائل, "annotate", "-c", تہی);
	برائے (صحیح i = 1; i < argc; i++)
		strvec_ڈالو(&دلائل, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(دلائل_copy, دلائل.nr + 1);
	COPY_ARRAY(دلائل_copy, دلائل.v, دلائل.nr);

	ret = cmd_blame(دلائل.nr, دلائل_copy, سابقہ, repo);

	strvec_clear(&دلائل);
	آزاد(دلائل_copy);
	واپس ret;
}
