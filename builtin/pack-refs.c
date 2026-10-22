#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "pack-refs.h"

পূর্ণসংখ্যা cmd_pack_refs(পূর্ণসংখ্যা argc,
		  ধ্রুবক অক্ষর **argv,
		  ধ্রুবক অক্ষর *prefix,
		  কাঠামো repository *repo)
{
	স্থির অক্ষর ধ্রুবক * ধ্রুবক pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		ফাঁকা
	};

	ফেরত pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
