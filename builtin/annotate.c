/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#অন্তর্ভাব "git-compat-util.h"
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "strvec.h"

পূর্ণসংখ্যা cmd_annotate(পূর্ণসংখ্যা argc,
		 ধ্রুবক অক্ষর **argv,
		 ধ্রুবক অক্ষর *prefix,
		 কাঠামো repository *repo)
{
	কাঠামো strvec args = STRVEC_INIT;
	ধ্রুবক অক্ষর **args_copy;
	পূর্ণসংখ্যা ret;

	strvec_pushl(&args, "annotate", "-c", ফাঁকা);
	জন্য (পূর্ণসংখ্যা i = 1; i < argc; i++)
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
	ফেরত ret;
}
