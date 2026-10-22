#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "pack-refs.h"

പൂർണ്ണസംഖ്യ cmd_pack_refs(പൂർണ്ണസംഖ്യ argc,
		  സ്ഥിരം അക്ഷരം **argv,
		  സ്ഥിരം അക്ഷരം *prefix,
		  ഘടന repository *repo)
{
	സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		ശൂന്യ
	};

	തിരികെ pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
