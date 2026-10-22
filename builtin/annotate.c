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
		 ثابت حرف *پیشوند,
		 بنية repository *repo)
{
	بنية strvec آرگومانها = STRVEC_INIT;
	ثابت حرف **آرگومانها_copy;
	صحيح ret;

	strvec_pushl(&آرگومانها, "annotate", "-c", عدم);
	لكل (صحيح i = 1; i < argc; i++)
		strvec_افزودن(&آرگومانها, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(آرگومانها_copy, آرگومانها.nr + 1);
	COPY_ARRAY(آرگومانها_copy, آرگومانها.v, آرگومانها.nr);

	ret = cmd_blame(آرگومانها.nr, آرگومانها_copy, پیشوند, repo);

	strvec_clear(&آرگومانها);
	حرر(آرگومانها_copy);
	أرجع ret;
}
