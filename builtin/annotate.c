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
		 ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo)
{
	संरचना strvec तर्काः = STRVEC_INIT;
	ध्रुव वर्ण **तर्काः_copy;
	पूर्णाङ्क ret;

	strvec_pushl(&तर्काः, "annotate", "-c", रिक्त);
	प्रति (पूर्णाङ्क i = 1; i < argc; i++)
		strvec_प्रक्षिप(&तर्काः, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(तर्काः_copy, तर्काः.nr + 1);
	COPY_ARRAY(तर्काः_copy, तर्काः.v, तर्काः.nr);

	ret = cmd_blame(तर्काः.nr, तर्काः_copy, उपसर्गः, repo);

	strvec_clear(&तर्काः);
	मुक्त(तर्काः_copy);
	प्रत्यागम ret;
}
