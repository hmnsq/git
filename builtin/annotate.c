/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#અન્તર્ભાવ "git-compat-util.h"
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "strvec.h"

પૂર્ણાંક cmd_annotate(પૂર્ણાંક argc,
		 અચલ અક્ષર **argv,
		 અચલ અક્ષર *prefix,
		 સંરચના repository *repo)
{
	સંરચના strvec args = STRVEC_INIT;
	અચલ અક્ષર **args_copy;
	પૂર્ણાંક ret;

	strvec_pushl(&args, "annotate", "-c", ખાલી);
	માટે (પૂર્ણાંક i = 1; i < argc; i++)
		strvec_ધકેલો(&args, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(args_copy, args.nr + 1);
	COPY_ARRAY(args_copy, args.v, args.nr);

	ret = cmd_blame(args.nr, args_copy, prefix, repo);

	strvec_clear(&args);
	મુક્ત(args_copy);
	પાછા ret;
}
