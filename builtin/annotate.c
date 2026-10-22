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
	بنية strvec args = STRVEC_INIT;
	ثابت حرف **args_copy;
	صحيح ret;

	strvec_pushl(&args, "annotate", "-c", عدم);
	لكل (صحيح i = 1; i < argc; i++)
		strvec_ادفع(&args, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(args_copy, args.nr + 1);
	COPY_ARRAY(args_copy, args.v, args.nr);

	ret = cmd_blame(args.nr, args_copy, prefix, repo);

	strvec_clear(&args);
	حرر(args_copy);
	إرجاع ret;
}
