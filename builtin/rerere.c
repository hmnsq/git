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

static const char * const rerere_usage[] = {
	N_("git rerere [clear | forget <pathspec>... | diff | status | remaining | gc]"),
	NULL,
};

static 整数 outf(void *dummy UNUSED, mmfile_t *ptr, 整数 nbuf)
{
	整数 i;
	为 (i = 0; i < nbuf; i++)
		如果 (write_in_full(1, ptr[i].ptr, ptr[i].size) < 0)
			返回 -1;
	返回 0;
}

static 整数 diff_two(const char *file1, const char *label1,
		const char *file2, const char *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_line = outf };
	mmfile_t minus, plus;
	整数 ret;

	如果 (read_mmfile(&minus, file1))
		返回 -1;
	如果 (read_mmfile(&plus, file2)) {
		free(minus.ptr);
		返回 -1;
	}

	printf("--- a/%s\n+++ b/%s\n", label1, label2);
	fflush(stdout);
	memset(&xpp, 0, sizeof(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, sizeof(xecfg));
	xecfg.ctxlen = 3;
	ret = xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	free(minus.ptr);
	free(plus.ptr);
	返回 ret;
}

整数 cmd_rerere(整数 argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo UNUSED)
{
	struct string_list merge_rr = STRING_LIST_INIT_DUP;
	整数 autoupdate = -1, flags = 0;

	struct option options[] = {
		OPT_SET_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, NULL);

	如果 (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	如果 (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	如果 (argc < 1)
		返回 repo_rerere(the_repository, flags);

	如果 (!strcmp(argv[0], "forget")) {
		struct pathspec pathspec;
		整数 ret;

		如果 (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       prefix, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		返回 ret;
	}

	如果 (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &merge_rr);
	} 否则 如果 (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &merge_rr);
	否则 如果 (!strcmp(argv[0], "status")) {
		如果 (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			返回 0;
		为 (size_t i = 0; i < merge_rr.nr; i++)
			printf("%s\n", merge_rr.items[i].string);
	} 否则 如果 (!strcmp(argv[0], "remaining")) {
		rerere_remaining(the_repository, &merge_rr);
		为 (size_t i = 0; i < merge_rr.nr; i++) {
			如果 (merge_rr.items[i].util != RERERE_RESOLVED)
				printf("%s\n", merge_rr.items[i].string);
			否则
				/* prepare for later call to
				 * string_list_clear() */
				merge_rr.items[i].util = NULL;
		}
	} 否则 如果 (!strcmp(argv[0], "diff")) {
		struct strbuf buf = STRBUF_INIT;
		如果 (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			返回 0;
		为 (size_t i = 0; i < merge_rr.nr; i++) {
			const char *path = merge_rr.items[i].string;
			const struct rerere_id *id = merge_rr.items[i].util;
			如果 (diff_two(rerere_path(&buf, id, "preimage"), path, path, path))
				die(_("unable to generate diff for '%s'"), rerere_path(&buf, id, NULL));
		}

		strbuf_release(&buf);
	} 否则
		usage_with_options(rerere_usage, options);

	string_list_clear(&merge_rr, 1);
	返回 0;
}
