/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#అన్తర్భావ "git-compat-util.h"
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "strvec.h"

పూర్ణాంకం cmd_annotate(పూర్ణాంకం argc,
		 స్థిరరాశి అక్షరం **argv,
		 స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo)
{
	నిర్మాణం strvec వాదనలు = STRVEC_INIT;
	స్థిరరాశి అక్షరం **వాదనలు_copy;
	పూర్ణాంకం ret;

	strvec_pushl(&వాదనలు, "annotate", "-c", ఖాళీ);
	కొరకు (పూర్ణాంకం i = 1; i < argc; i++)
		strvec_నెట్టు(&వాదనలు, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(వాదనలు_copy, వాదనలు.nr + 1);
	COPY_ARRAY(వాదనలు_copy, వాదనలు.v, వాదనలు.nr);

	ret = cmd_blame(వాదనలు.nr, వాదనలు_copy, ఉపసర్గ, repo);

	strvec_clear(&వాదనలు);
	విడుదల(వాదనలు_copy);
	తిరిగి ret;
}
