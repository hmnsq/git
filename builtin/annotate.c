/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#অন্তর্ভাব "git-compat-util.h"
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "strvec.h"

পূর্ণাঙ্ক cmd_annotate(পূর্ণাঙ্ক argc,
		 ধ্রুব বর্ণ **argv,
		 ধ্রুব বর্ণ *prefix,
		 সংরচনা repository *repo)
{
	সংরচনা strvec args = STRVEC_INIT;
	ধ্রুব বর্ণ **args_copy;
	পূর্ণাঙ্ক ret;

	strvec_pushl(&args, "annotate", "-c", রিক্ত);
	প্রতি (পূর্ণাঙ্ক i = 1; i < argc; i++)
		strvec_ঠেলো(&args, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(args_copy, args.nr + 1);
	COPY_ARRAY(args_copy, args.v, args.nr);

	ret = cmd_blame(args.nr, args_copy, prefix, repo);

	strvec_clear(&args);
	মুক্ত(args_copy);
	প্রত্যাগম ret;
}
