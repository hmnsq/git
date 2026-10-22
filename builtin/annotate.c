/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#include "git-compat-util.h"
#include "builtin.h"
#include "strvec.h"

số_nguyên cmd_annotate(số_nguyên argc,
		 const char **argv,
		 const char *prefix,
		 struct repository *repo)
{
	struct strvec args = STRVEC_INIT;
	const char **args_copy;
	số_nguyên ret;

	strvec_pushl(&args, "annotate", "-c", NULL);
	cho (số_nguyên i = 1; i < argc; i++)
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
	trả_về ret;
}
