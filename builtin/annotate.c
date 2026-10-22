/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#अन्तर्भाव "git-compat-util.h"
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "strvec.h"

पूर्णाङ्क cmd_annotate(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *prefix,
		 संरचना repository *repo)
{
	संरचना strvec args = STRVEC_INIT;
	ध्रुव वर्ण **args_copy;
	पूर्णाङ्क ret;

	strvec_pushl(&args, "annotate", "-c", रिक्त);
	प्रति (पूर्णाङ्क i = 1; i < argc; i++)
		strvec_ढकला(&args, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(args_copy, args.nr + 1);
	COPY_ARRAY(args_copy, args.v, args.nr);

	ret = cmd_blame(args.nr, args_copy, prefix, repo);

	strvec_clear(&args);
	मुक्त(args_copy);
	प्रत्यागम ret;
}
