/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#அந்தர்பாவ "git-compat-util.h"
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "strvec.h"

முழு cmd_annotate(முழு argc,
		 மாறிலி எழுத்து **argv,
		 மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு strvec args = STRVEC_INIT;
	மாறிலி எழுத்து **args_copy;
	முழு ret;

	strvec_pushl(&args, "annotate", "-c", வெற்று);
	க்கு (முழு i = 1; i < argc; i++)
		strvec_தள்ளு(&args, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(args_copy, args.nr + 1);
	COPY_ARRAY(args_copy, args.v, args.nr);

	ret = cmd_blame(args.nr, args_copy, prefix, repo);

	strvec_clear(&args);
	விடுவி(args_copy);
	திரும்பு ret;
}
