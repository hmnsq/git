#అన్తర్భావ "builtin.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "pack-refs.h"

పూర్ణాంకం cmd_pack_refs(పూర్ణాంకం argc,
		  స్థిరరాశి అక్షరం **argv,
		  స్థిరరాశి అక్షరం *ఉపసర్గ,
		  నిర్మాణం repository *repo)
{
	స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		ఖాళీ
	};

	తిరిగి pack_refs_core(argc, argv, ఉపసర్గ, repo, pack_refs_usage);
}
