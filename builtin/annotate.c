/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#അന്തര്ഭാവ "git-compat-util.h"
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "strvec.h"

പൂർണ്ണസംഖ്യ cmd_annotate(പൂർണ്ണസംഖ്യ argc,
		 സ്ഥിരം അക്ഷരം **argv,
		 സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo)
{
	ഘടന strvec args = STRVEC_INIT;
	സ്ഥിരം അക്ഷരം **args_copy;
	പൂർണ്ണസംഖ്യ ret;

	strvec_pushl(&args, "annotate", "-c", ശൂന്യ);
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < argc; i++)
		strvec_തള്ളുക(&args, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(args_copy, args.nr + 1);
	COPY_ARRAY(args_copy, args.v, args.nr);

	ret = cmd_blame(args.nr, args_copy, prefix, repo);

	strvec_clear(&args);
	സ്വതന്ത്രം(args_copy);
	തിരികെ ret;
}
