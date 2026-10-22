/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#include "git-compat-util.h"
#include "builtin.h"
#include "strvec.h"

ཧྲིལ cmd_annotate(ཧྲིལ argc,
		 const var_na **argv,
		 const var_na *prefix,
		 sa_nracanaa repository *repo)
{
	sa_nracanaa strvec args = STRVEC_INIT;
	const var_na **args_copy;
	ཧྲིལ ret;

	strvec_pushl(&args, "annotate", "-c", NULL);
	དོན་ལུ (ཧྲིལ i = 1; i < argc; i++)
		strvec_push(&args, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(args_copy, args.nr + 1);
	COPY_ARRAY(args_copy, args.v, args.nr);

	ret = cmd_blame(args.nr, args_copy, prefix, repo);

	strvec_clear(&args);
	free(args_copy);
	ལོག ret;
}
