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

static ຈຳນວນເຕັມ outf(void *dummy UNUSED, mmfile_t *ptr, ຈຳນວນເຕັມ nbuf)
{
	ຈຳນວນເຕັມ i;
	ສຳລັບ (i = 0; i < nbuf; i++)
		ຖ້າ (write_in_full(1, ptr[i].ptr, ptr[i].size) < 0)
			ສົ່ງຄືນ -1;
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ diff_two(const var_na *file1, const var_na *label1,
		const var_na *file2, const var_na *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_line = outf };
	mmfile_t minus, plus;
	ຈຳນວນເຕັມ ret;

	ຖ້າ (read_mmfile(&minus, file1))
		ສົ່ງຄືນ -1;
	ຖ້າ (read_mmfile(&plus, file2)) {
		free(minus.ptr);
		ສົ່ງຄືນ -1;
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
	ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ cmd_rerere(ຈຳນວນເຕັມ argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa string_list merge_rr = STRING_LIST_INIT_DUP;
	ຈຳນວນເຕັມ autoupdate = -1, flags = 0;

	sa_nracanaa option options[] = {
		OPT_SET_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, NULL);

	ຖ້າ (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	ຖ້າ (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	ຖ້າ (argc < 1)
		ສົ່ງຄືນ repo_rerere(the_repository, flags);

	ຖ້າ (!strcmp(argv[0], "forget")) {
		sa_nracanaa pathspec pathspec;
		ຈຳນວນເຕັມ ret;

		ຖ້າ (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       prefix, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		ສົ່ງຄືນ ret;
	}

	ຖ້າ (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &merge_rr);
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &merge_rr);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(argv[0], "status")) {
		ຖ້າ (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			ສົ່ງຄືນ 0;
		ສຳລັບ (size_t i = 0; i < merge_rr.nr; i++)
			printf("%s\n", merge_rr.items[i].string);
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(argv[0], "remaining")) {
		rerere_remaining(the_repository, &merge_rr);
		ສຳລັບ (size_t i = 0; i < merge_rr.nr; i++) {
			ຖ້າ (merge_rr.items[i].util != RERERE_RESOLVED)
				printf("%s\n", merge_rr.items[i].string);
			ບໍ່ດັ່ງນັ້ນ
				/* prepare for later call to
				 * string_list_clear() */
				merge_rr.items[i].util = NULL;
		}
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(argv[0], "diff")) {
		sa_nracanaa strbuf buf = STRBUF_INIT;
		ຖ້າ (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			ສົ່ງຄືນ 0;
		ສຳລັບ (size_t i = 0; i < merge_rr.nr; i++) {
			const var_na *path = merge_rr.items[i].string;
			const sa_nracanaa rerere_id *id = merge_rr.items[i].util;
			ຖ້າ (diff_two(rerere_path(&buf, id, "preimage"), path, path, path))
				die(_("unable to generate diff for '%s'"), rerere_path(&buf, id, NULL));
		}

		strbuf_release(&buf);
	} ບໍ່ດັ່ງນັ້ນ
		usage_with_options(rerere_usage, options);

	string_list_clear(&merge_rr, 1);
	ສົ່ງຄືນ 0;
}
