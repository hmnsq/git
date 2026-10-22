/*
 * "git annotate" builtin alias
 *
 * Copyright (C) 2006 Ryan Anderson
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "git-compat-util.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"

ਪੂਰਨ cmd_annotate(ਪੂਰਨ argc,
		 ਅਚਲ ਅੱਖਰ **argv,
		 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ strvec ਦਲੀਲਾਂ = STRVEC_INIT;
	ਅਚਲ ਅੱਖਰ **ਦਲੀਲਾਂ_copy;
	ਪੂਰਨ ret;

	strvec_pushl(&ਦਲੀਲਾਂ, "annotate", "-c", ਨਿਰਾ);
	ਲਈ (ਪੂਰਨ i = 1; i < argc; i++)
		strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, argv[i]);

	/*
	 * `cmd_blame()` ends up modifying the array, which causes memory leaks
	 * if we didn't copy the array here.
	 */
	CALLOC_ARRAY(ਦਲੀਲਾਂ_copy, ਦਲੀਲਾਂ.nr + 1);
	COPY_ARRAY(ਦਲੀਲਾਂ_copy, ਦਲੀਲਾਂ.v, ਦਲੀਲਾਂ.nr);

	ret = cmd_blame(ਦਲੀਲਾਂ.nr, ਦਲੀਲਾਂ_copy, ਅਗੇਤਰ, repo);

	strvec_clear(&ਦਲੀਲਾਂ);
	ਮੁਕਤ(ਦਲੀਲਾਂ_copy);
	ਵਾਪਸ ret;
}
