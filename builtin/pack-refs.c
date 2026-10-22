#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "pack-refs.h"

পূর্ণাঙ্ক cmd_pack_refs(পূর্ণাঙ্ক argc,
		  ধ্রুব বর্ণ **argv,
		  ধ্রুব বর্ণ *prefix,
		  সংরচনা repository *repo)
{
	স্থির বর্ণ ধ্রুব * ধ্রুব pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		রিক্ত
	};

	প্রত্যাগম pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
