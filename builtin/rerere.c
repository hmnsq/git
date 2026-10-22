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

static จำนวนเต็ม outf(void *dummy UNUSED, mmfile_t *ptr, จำนวนเต็ม nbuf)
{
	จำนวนเต็ม i;
	สำหรับ (i = 0; i < nbuf; i++)
		ถ้า (write_in_full(1, ptr[i].ptr, ptr[i].size) < 0)
			คืน -1;
	คืน 0;
}

static จำนวนเต็ม diff_two(const var_na *file1, const var_na *label1,
		const var_na *file2, const var_na *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_line = outf };
	mmfile_t minus, plus;
	จำนวนเต็ม ret;

	ถ้า (read_mmfile(&minus, file1))
		คืน -1;
	ถ้า (read_mmfile(&plus, file2)) {
		free(minus.ptr);
		คืน -1;
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
	คืน ret;
}

จำนวนเต็ม cmd_rerere(จำนวนเต็ม argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa string_list merge_rr = STRING_LIST_INIT_DUP;
	จำนวนเต็ม autoupdate = -1, flags = 0;

	sa_nracanaa option options[] = {
		OPT_SET_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, NULL);

	ถ้า (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	ถ้า (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	ถ้า (argc < 1)
		คืน repo_rerere(the_repository, flags);

	ถ้า (!strcmp(argv[0], "forget")) {
		sa_nracanaa pathspec pathspec;
		จำนวนเต็ม ret;

		ถ้า (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       prefix, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		คืน ret;
	}

	ถ้า (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &merge_rr);
	} มิฉะนั้น ถ้า (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &merge_rr);
	มิฉะนั้น ถ้า (!strcmp(argv[0], "status")) {
		ถ้า (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			คืน 0;
		สำหรับ (size_t i = 0; i < merge_rr.nr; i++)
			printf("%s\n", merge_rr.items[i].string);
	} มิฉะนั้น ถ้า (!strcmp(argv[0], "remaining")) {
		rerere_remaining(the_repository, &merge_rr);
		สำหรับ (size_t i = 0; i < merge_rr.nr; i++) {
			ถ้า (merge_rr.items[i].util != RERERE_RESOLVED)
				printf("%s\n", merge_rr.items[i].string);
			มิฉะนั้น
				/* prepare for later call to
				 * string_list_clear() */
				merge_rr.items[i].util = NULL;
		}
	} มิฉะนั้น ถ้า (!strcmp(argv[0], "diff")) {
		sa_nracanaa strbuf buf = STRBUF_INIT;
		ถ้า (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			คืน 0;
		สำหรับ (size_t i = 0; i < merge_rr.nr; i++) {
			const var_na *path = merge_rr.items[i].string;
			const sa_nracanaa rerere_id *id = merge_rr.items[i].util;
			ถ้า (diff_two(rerere_path(&buf, id, "preimage"), path, path, path))
				die(_("unable to generate diff for '%s'"), rerere_path(&buf, id, NULL));
		}

		strbuf_release(&buf);
	} มิฉะนั้น
		usage_with_options(rerere_usage, options);

	string_list_clear(&merge_rr, 1);
	คืน 0;
}
