#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "rerere.h"
#ضمن "strbuf.h"
#ضمن "string-list.h"
#ضمن "xdiff/xdiff.h"
#ضمن "xdiff-interface.h"
#ضمن "pathspec.h"

ساكن ثابت حرف * ثابت rerere_usage[] = {
	N_("git rerere [clear | forget <pathspec>... | diff | status | remaining | gc]"),
	عدم,
};

ساكن صحيح outf(فراغ *dummy UNUSED, mmfile_t *ptr, صحيح nbuf)
{
	صحيح i;
	עבור (i = 0; i < nbuf; i++)
		אם (write_in_full(1, ptr[i].ptr, ptr[i].size) < 0)
			החזר -1;
	החזר 0;
}

ساكن صحيح diff_two(ثابت حرف *file1, ثابت حرف *label1,
		ثابت حرف *file2, ثابت حرف *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_line = outf };
	mmfile_t minus, plus;
	صحيح ret;

	אם (read_mmfile(&minus, file1))
		החזר -1;
	אם (read_mmfile(&plus, file2)) {
		حرر(minus.ptr);
		החזר -1;
	}

	printf("--- a/%s\n+++ b/%s\n", label1, label2);
	fflush(stdout);
	memset(&xpp, 0, حجم(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, حجم(xecfg));
	xecfg.ctxlen = 3;
	ret = xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	حرر(minus.ptr);
	حرر(plus.ptr);
	החזר ret;
}

صحيح cmd_rerere(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *prefix,
	       بنية repository *repo UNUSED)
{
	بنية string_list merge_rr = STRING_LIST_INIT_DUP;
	صحيح autoupdate = -1, flags = 0;

	بنية option options[] = {
		OPT_SET_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, عدم);

	אם (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	אם (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	אם (argc < 1)
		החזר repo_rerere(the_repository, flags);

	אם (!strcmp(argv[0], "forget")) {
		بنية pathspec pathspec;
		صحيح ret;

		אם (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       prefix, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		החזר ret;
	}

	אם (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &merge_rr);
	} אחרת אם (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &merge_rr);
	אחרת אם (!strcmp(argv[0], "status")) {
		אם (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			החזר 0;
		עבור (size_t i = 0; i < merge_rr.nr; i++)
			printf("%s\n", merge_rr.items[i].string);
	} אחרת אם (!strcmp(argv[0], "remaining")) {
		rerere_remaining(the_repository, &merge_rr);
		עבור (size_t i = 0; i < merge_rr.nr; i++) {
			אם (merge_rr.items[i].util != RERERE_RESOLVED)
				printf("%s\n", merge_rr.items[i].string);
			אחרת
				/* prepare for later call to
				 * string_list_clear() */
				merge_rr.items[i].util = عدم;
		}
	} אחרת אם (!strcmp(argv[0], "diff")) {
		بنية strbuf buf = STRBUF_INIT;
		אם (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			החזר 0;
		עבור (size_t i = 0; i < merge_rr.nr; i++) {
			ثابت حرف *path = merge_rr.items[i].string;
			ثابت بنية rerere_id *id = merge_rr.items[i].util;
			אם (diff_two(rerere_path(&buf, id, "preimage"), path, path, path))
				die(_("unable to generate diff for '%s'"), rerere_path(&buf, id, عدم));
		}

		strbuf_release(&buf);
	} אחרת
		usage_with_options(rerere_usage, options);

	string_list_clear(&merge_rr, 1);
	החזר 0;
}
