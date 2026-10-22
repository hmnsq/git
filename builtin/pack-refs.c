#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "pack-refs.h"

পূর্ণাঙ্ক cmd_pack_refs(পূর্ণাঙ্ক argc,
		  ধ্রুৱ ৱর্ণ **argv,
		  ধ্রুৱ ৱর্ণ *prefix,
		  সংরচনা repository *repo)
{
	স্থির ৱর্ণ ধ্রুৱ * ধ্রুৱ pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		রিক্ত
	};

	ঘূৰাই pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
