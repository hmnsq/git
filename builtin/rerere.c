#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "rerere.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "xdiff/xdiff.h"
#అన్తర్భావ "xdiff-interface.h"
#అన్తర్భావ "pathspec.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి rerere_usage[] = {
	N_("git rerere [clear | forget <pathspec>... | diff | status | remaining | gc]"),
	ఖాళీ,
};

స్థిర పూర్ణాంకం outf(శూన్య *dummy వాడని, mmfile_t *ptr, పూర్ణాంకం nbuf)
{
	పూర్ణాంకం i;
	కొరకు (i = 0; i < nbuf; i++)
		ఒకవేళ (write_లోపల_నిండిన(1, ptr[i].ptr, ptr[i].కొలత) < 0)
			తిరిగి -1;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం diff_two(స్థిరరాశి అక్షరం *file1, స్థిరరాశి అక్షరం *label1,
		స్థిరరాశి అక్షరం *file2, స్థిరరాశి అక్షరం *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .బహిర్గతం_పంక్తి = outf };
	mmfile_t minus, plus;
	పూర్ణాంకం ret;

	ఒకవేళ (read_mmfile(&minus, file1))
		తిరిగి -1;
	ఒకవేళ (read_mmfile(&plus, file2)) {
		విడుదల(minus.ptr);
		తిరిగి -1;
	}

	రాయి("--- a/%s\n+++ b/%s\n", label1, label2);
	fflush(stdout);
	memset(&xpp, 0, పరిమాణం(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, పరిమాణం(xecfg));
	xecfg.ctxlen = 3;
	ret = xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	విడుదల(minus.ptr);
	విడుదల(plus.ptr);
	తిరిగి ret;
}

పూర్ణాంకం cmd_rerere(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం string_list విలీనం_rr = STRING_LIST_INIT_DUP;
	పూర్ణాంకం autoupdate = -1, flags = 0;

	నిర్మాణం option options[] = {
		OPT_సమితి_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_చివర(),
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, ఖాళీ);

	ఒకవేళ (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	ఒకవేళ (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	ఒకవేళ (argc < 1)
		తిరిగి repo_rerere(the_repository, flags);

	ఒకవేళ (!strcmp(argv[0], "forget")) {
		నిర్మాణం pathspec pathspec;
		పూర్ణాంకం ret;

		ఒకవేళ (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       ఉపసర్గ, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		తిరిగి ret;
	}

	ఒకవేళ (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &విలీనం_rr);
	} లేకపోతే ఒకవేళ (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &విలీనం_rr);
	లేకపోతే ఒకవేళ (!strcmp(argv[0], "status")) {
		ఒకవేళ (అమరిక_rerere(the_repository, &విలీనం_rr,
				 flags | RERERE_READONLY) < 0)
			తిరిగి 0;
		కొరకు (కొలత_t i = 0; i < విలీనం_rr.nr; i++)
			రాయి("%s\n", విలీనం_rr.వస్తువులు[i].string);
	} లేకపోతే ఒకవేళ (!strcmp(argv[0], "remaining")) {
		rerere_మిగిలిన(the_repository, &విలీనం_rr);
		కొరకు (కొలత_t i = 0; i < విలీనం_rr.nr; i++) {
			ఒకవేళ (విలీనం_rr.వస్తువులు[i].util != RERERE_RESOLVED)
				రాయి("%s\n", విలీనం_rr.వస్తువులు[i].string);
			లేకపోతే
				/* prepare for later call to
				 * string_list_clear() */
				విలీనం_rr.వస్తువులు[i].util = ఖాళీ;
		}
	} లేకపోతే ఒకవేళ (!strcmp(argv[0], "diff")) {
		నిర్మాణం strbuf buf = STRBUF_INIT;
		ఒకవేళ (అమరిక_rerere(the_repository, &విలీనం_rr,
				 flags | RERERE_READONLY) < 0)
			తిరిగి 0;
		కొరకు (కొలత_t i = 0; i < విలీనం_rr.nr; i++) {
			స్థిరరాశి అక్షరం *మార్గం = విలీనం_rr.వస్తువులు[i].string;
			స్థిరరాశి నిర్మాణం rerere_id *id = విలీనం_rr.వస్తువులు[i].util;
			ఒకవేళ (diff_two(rerere_మార్గం(&buf, id, "preimage"), మార్గం, మార్గం, మార్గం))
				die(_("unable to generate diff for '%s'"), rerere_మార్గం(&buf, id, ఖాళీ));
		}

		strbuf_release(&buf);
	} లేకపోతే
		usage_with_options(rerere_usage, options);

	string_list_clear(&విలీనం_rr, 1);
	తిరిగి 0;
}
