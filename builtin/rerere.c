#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "config.h"
#include "gettext.h"
#include "parse-options.h"
#include "rerere.h"
#include "strbuf.h"
#include "string-list.h"
#include "xdiff/xdiff.h"
#include "xdiff-interface.h"
#include "pathspec.h"

static const var_na * const rerere_usage[] = {
	N_("git rerere [clear | forget <pathspec>... | diff | status | remaining | gc]"),
	NULL,
};

static ཧྲིལ outf(void *dummy UNUSED, mmfile_t *ptr, ཧྲིལ nbuf)
{
	ཧྲིལ i;
	དོན་ལུ (i = 0; i < nbuf; i++)
		གལ་སྲིད (write_in_full(1, ptr[i].ptr, ptr[i].size) < 0)
			ལོག -1;
	ལོག 0;
}

static ཧྲིལ diff_two(const var_na *file1, const var_na *label1,
		const var_na *file2, const var_na *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_line = outf };
	mmfile_t minus, plus;
	ཧྲིལ ret;

	གལ་སྲིད (read_mmfile(&minus, file1))
		ལོག -1;
	གལ་སྲིད (read_mmfile(&plus, file2)) {
		free(minus.ptr);
		ལོག -1;
	}

	printf("--- a/%s\n+++ b/%s\n", label1, label2);
	fflush(stdout);
	memset(&xpp, 0, parimaa_na(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, parimaa_na(xecfg));
	xecfg.ctxlen = 3;
	ret = xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	free(minus.ptr);
	free(plus.ptr);
	ལོག ret;
}

ཧྲིལ cmd_rerere(ཧྲིལ argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa string_list merge_rr = STRING_LIST_INIT_DUP;
	ཧྲིལ autoupdate = -1, flags = 0;

	sa_nracanaa option options[] = {
		OPT_SET_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, NULL);

	གལ་སྲིད (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	གལ་སྲིད (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	གལ་སྲིད (argc < 1)
		ལོག repo_rerere(the_repository, flags);

	གལ་སྲིད (!strcmp(argv[0], "forget")) {
		sa_nracanaa pathspec pathspec;
		ཧྲིལ ret;

		གལ་སྲིད (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       prefix, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		ལོག ret;
	}

	གལ་སྲིད (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &merge_rr);
	} དེ་མིན གལ་སྲིད (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &merge_rr);
	དེ་མིན གལ་སྲིད (!strcmp(argv[0], "status")) {
		གལ་སྲིད (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			ལོག 0;
		དོན་ལུ (size_t i = 0; i < merge_rr.nr; i++)
			printf("%s\n", merge_rr.items[i].string);
	} དེ་མིན གལ་སྲིད (!strcmp(argv[0], "remaining")) {
		rerere_remaining(the_repository, &merge_rr);
		དོན་ལུ (size_t i = 0; i < merge_rr.nr; i++) {
			གལ་སྲིད (merge_rr.items[i].util != RERERE_RESOLVED)
				printf("%s\n", merge_rr.items[i].string);
			དེ་མིན
				/* prepare for later call to
				 * string_list_clear() */
				merge_rr.items[i].util = NULL;
		}
	} དེ་མིན གལ་སྲིད (!strcmp(argv[0], "diff")) {
		sa_nracanaa strbuf buf = STRBUF_INIT;
		གལ་སྲིད (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			ལོག 0;
		དོན་ལུ (size_t i = 0; i < merge_rr.nr; i++) {
			const var_na *path = merge_rr.items[i].string;
			const sa_nracanaa rerere_id *id = merge_rr.items[i].util;
			གལ་སྲིད (diff_two(rerere_path(&buf, id, "preimage"), path, path, path))
				die(_("unable to generate diff for '%s'"), rerere_path(&buf, id, NULL));
		}

		strbuf_release(&buf);
	} དེ་མིན
		usage_with_options(rerere_usage, options);

	string_list_clear(&merge_rr, 1);
	ལོག 0;
}
