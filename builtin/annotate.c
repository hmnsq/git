/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#ಅನ್ತರ್ಭಾವ "git-compat-util.h"
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"

ಪೂರ್ಣಾಂಕ cmd_annotate(ಪೂರ್ಣಾಂಕ argc,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo)
{
	ರಚನೆ strvec ವಾದಗಳು = STRVEC_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ವಾದಗಳು_copy;
	ಪೂರ್ಣಾಂಕ ret;

	strvec_pushl(&ವಾದಗಳು, "annotate", "-c", ಖಾಲಿ);
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < argc; i++)
		strvec_ತಳ್ಳು(&ವಾದಗಳು, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(ವಾದಗಳು_copy, ವಾದಗಳು.nr + 1);
	COPY_ARRAY(ವಾದಗಳು_copy, ವಾದಗಳು.v, ವಾದಗಳು.nr);

	ret = cmd_blame(ವಾದಗಳು.nr, ವಾದಗಳು_copy, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);

	strvec_clear(&ವಾದಗಳು);
	ಬಿಡುಗಡೆ(ವಾದಗಳು_copy);
	ಹಿಂತಿರುಗಿ ret;
}
