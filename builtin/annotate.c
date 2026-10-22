/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#شامل "git-compat-util.h"
#شامل "builtin.h"
#شامل "strvec.h"

صحیح cmd_annotate(صحیح argc,
		 ثابت نویسه **argv,
		 ثابت نویسه *prefix,
		 ساختار repository *repo)
{
	ساختار strvec args = STRVEC_INIT;
	ثابت نویسه **args_copy;
	صحیح ret;

	strvec_pushl(&args, "annotate", "-c", پوچ);
	برای (صحیح i = 1; i < argc; i++)
		strvec_افزودن(&args, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(args_copy, args.nr + 1);
	COPY_ARRAY(args_copy, args.v, args.nr);

	ret = cmd_blame(args.nr, args_copy, prefix, repo);

	strvec_clear(&args);
	آزاد(args_copy);
	بازگشت ret;
}
