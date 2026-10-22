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

static puur_naa_nka outf(void *dummy UNUSED, mmfile_t *ptr, puur_naa_nka nbuf)
{
	puur_naa_nka i;
	for (i = 0; i < nbuf; i++)
		ပေႃး (write_in_full(1, ptr[i].ptr, ptr[i].size) < 0)
			ပွၵ်ႈ -1;
	ပွၵ်ႈ 0;
}

static puur_naa_nka diff_two(const var_na *file1, const var_na *label1,
		const var_na *file2, const var_na *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_line = outf };
	mmfile_t minus, plus;
	puur_naa_nka ret;

	ပေႃး (read_mmfile(&minus, file1))
		ပွၵ်ႈ -1;
	ပေႃး (read_mmfile(&plus, file2)) {
		free(minus.ptr);
		ပွၵ်ႈ -1;
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
	ပွၵ်ႈ ret;
}

puur_naa_nka cmd_rerere(puur_naa_nka argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa string_list merge_rr = STRING_LIST_INIT_DUP;
	puur_naa_nka autoupdate = -1, flags = 0;

	sa_nracanaa option options[] = {
		OPT_SET_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, NULL);

	ပေႃး (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	ပေႃး (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	ပေႃး (argc < 1)
		ပွၵ်ႈ repo_rerere(the_repository, flags);

	ပေႃး (!strcmp(argv[0], "forget")) {
		sa_nracanaa pathspec pathspec;
		puur_naa_nka ret;

		ပေႃး (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       prefix, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		ပွၵ်ႈ ret;
	}

	ပေႃး (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &merge_rr);
	} ပေႃးဢမ်ႇ ပေႃး (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &merge_rr);
	ပေႃးဢမ်ႇ ပေႃး (!strcmp(argv[0], "status")) {
		ပေႃး (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			ပွၵ်ႈ 0;
		for (size_t i = 0; i < merge_rr.nr; i++)
			printf("%s\n", merge_rr.items[i].string);
	} ပေႃးဢမ်ႇ ပေႃး (!strcmp(argv[0], "remaining")) {
		rerere_remaining(the_repository, &merge_rr);
		for (size_t i = 0; i < merge_rr.nr; i++) {
			ပေႃး (merge_rr.items[i].util != RERERE_RESOLVED)
				printf("%s\n", merge_rr.items[i].string);
			ပေႃးဢမ်ႇ
				/* prepare for later call to
				 * string_list_clear() */
				merge_rr.items[i].util = NULL;
		}
	} ပေႃးဢမ်ႇ ပေႃး (!strcmp(argv[0], "diff")) {
		sa_nracanaa strbuf buf = STRBUF_INIT;
		ပေႃး (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			ပွၵ်ႈ 0;
		for (size_t i = 0; i < merge_rr.nr; i++) {
			const var_na *path = merge_rr.items[i].string;
			const sa_nracanaa rerere_id *id = merge_rr.items[i].util;
			ပေႃး (diff_two(rerere_path(&buf, id, "preimage"), path, path, path))
				die(_("unable to generate diff for '%s'"), rerere_path(&buf, id, NULL));
		}

		strbuf_release(&buf);
	} ပေႃးဢမ်ႇ
		usage_with_options(rerere_usage, options);

	string_list_clear(&merge_rr, 1);
	ပွၵ်ႈ 0;
}
