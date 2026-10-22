/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#ಅನ್ತರ್ಭಾವ "git-compat-util.h"
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"

ಪೂರ್ಣಾಙ್ಕ cmd_annotate(ಪೂರ್ಣಾಙ್ಕ argc,
		 ಧ್ರುವ ವರ್ಣ **argv,
		 ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ strvec args = STRVEC_INIT;
	ಧ್ರುವ ವರ್ಣ **args_copy;
	ಪೂರ್ಣಾಙ್ಕ ret;

	strvec_pushl(&args, "annotate", "-c", ರಿಕ್ತ);
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i < argc; i++)
		strvec_ತಳ್ಳು(&args, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(args_copy, args.nr + 1);
	COPY_ARRAY(args_copy, args.v, args.nr);

	ret = cmd_blame(args.nr, args_copy, prefix, repo);

	strvec_clear(&args);
	ಮುಕ್ತ(args_copy);
	ಪ್ರತ್ಯಾಗಮ ret;
}
