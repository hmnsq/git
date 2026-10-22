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

static 정수 outf(void *dummy UNUSED, mmfile_t *ptr, 정수 nbuf)
{
	정수 i;
	위해 (i = 0; i < nbuf; i++)
		만약 (write_in_full(1, ptr[i].ptr, ptr[i].size) < 0)
			반환 -1;
	반환 0;
}

static 정수 diff_two(const char *file1, const char *label1,
		const char *file2, const char *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_line = outf };
	mmfile_t minus, plus;
	정수 ret;

	만약 (read_mmfile(&minus, file1))
		반환 -1;
	만약 (read_mmfile(&plus, file2)) {
		free(minus.ptr);
		반환 -1;
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
	반환 ret;
}

정수 cmd_rerere(정수 argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo UNUSED)
{
	struct string_list merge_rr = STRING_LIST_INIT_DUP;
	정수 autoupdate = -1, flags = 0;

	struct option options[] = {
		OPT_SET_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, NULL);

	만약 (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	만약 (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	만약 (argc < 1)
		반환 repo_rerere(the_repository, flags);

	만약 (!strcmp(argv[0], "forget")) {
		struct pathspec pathspec;
		정수 ret;

		만약 (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       prefix, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		반환 ret;
	}

	만약 (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &merge_rr);
	} 아니면 만약 (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &merge_rr);
	아니면 만약 (!strcmp(argv[0], "status")) {
		만약 (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			반환 0;
		위해 (size_t i = 0; i < merge_rr.nr; i++)
			printf("%s\n", merge_rr.items[i].string);
	} 아니면 만약 (!strcmp(argv[0], "remaining")) {
		rerere_remaining(the_repository, &merge_rr);
		위해 (size_t i = 0; i < merge_rr.nr; i++) {
			만약 (merge_rr.items[i].util != RERERE_RESOLVED)
				printf("%s\n", merge_rr.items[i].string);
			아니면
				/* prepare for later call to
				 * string_list_clear() */
				merge_rr.items[i].util = NULL;
		}
	} 아니면 만약 (!strcmp(argv[0], "diff")) {
		struct strbuf buf = STRBUF_INIT;
		만약 (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			반환 0;
		위해 (size_t i = 0; i < merge_rr.nr; i++) {
			const char *path = merge_rr.items[i].string;
			const struct rerere_id *id = merge_rr.items[i].util;
			만약 (diff_two(rerere_path(&buf, id, "preimage"), path, path, path))
				die(_("unable to generate diff for '%s'"), rerere_path(&buf, id, NULL));
		}

		strbuf_release(&buf);
	} 아니면
		usage_with_options(rerere_usage, options);

	string_list_clear(&merge_rr, 1);
	반환 0;
}
