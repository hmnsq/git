/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#समावेश "git-compat-util.h"
#समावेश "builtin.h"
#समावेश "strvec.h"

पूर्णांक cmd_annotate(पूर्णांक argc,
		 स्थिर अक्षर **argv,
		 स्थिर अक्षर *prefix,
		 काष्ठा repository *repo)
{
	काष्ठा strvec args = STRVEC_INIT;
	स्थिर अक्षर **args_copy;
	पूर्णांक ret;

	strvec_pushl(&args, "annotate", "-c", शून्य);
	क्रम (पूर्णांक i = 1; i < argc; i++)
		strvec_धकेलो(&args, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(args_copy, args.nr + 1);
	COPY_ARRAY(args_copy, args.v, args.nr);

	ret = cmd_blame(args.nr, args_copy, prefix, repo);

	strvec_clear(&args);
	मुक्त(args_copy);
	वापस ret;
}
