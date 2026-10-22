#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "pack-refs.h"

પૂર્ણાંક cmd_pack_refs(પૂર્ણાંક argc,
		  અચલ અક્ષર **argv,
		  અચલ અક્ષર *prefix,
		  સંરચના repository *repo)
{
	સ્થિર અક્ષર અચલ * અચલ pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		ખાલી
	};

	પાછા pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
