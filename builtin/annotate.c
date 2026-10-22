/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#ଅନ୍ତର୍ଭାଵ "git-compat-util.h"
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_annotate(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		 ଅଚଳ ଅକ୍ଷର **argv,
		 ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo)
{
	ସଂରଚନା strvec args = STRVEC_INIT;
	ଅଚଳ ଅକ୍ଷର **args_copy;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	strvec_pushl(&args, "annotate", "-c", ଖାଲି);
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < argc; i++)
		strvec_ଠେଲ(&args, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(args_copy, args.nr + 1);
	COPY_ARRAY(args_copy, args.v, args.nr);

	ret = cmd_blame(args.nr, args_copy, prefix, repo);

	strvec_clear(&args);
	ମୁକ୍ତ(args_copy);
	ଫେରନ୍ତୁ ret;
}
