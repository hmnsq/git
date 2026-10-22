#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "config.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "rerere.h"
#شامل "strbuf.h"
#شامل "string-list.h"
#شامل "xdiff/xdiff.h"
#شامل "xdiff-interface.h"
#شامل "pathspec.h"

ایستا ثابت نویسه * ثابت rerere_usage[] = {
	N_("git rerere [clear | forget <pathspec>... | diff | status | remaining | gc]"),
	پوچ,
};

ایستا صحیح outf(تهی *dummy UNUSED, mmfile_t *ptr, صحیح nbuf)
{
	صحیح i;
	برای (i = 0; i < nbuf; i++)
		اگر (write_in_full(1, ptr[i].ptr, ptr[i].size) < 0)
			بازگشت -1;
	بازگشت 0;
}

ایستا صحیح diff_two(ثابت نویسه *file1, ثابت نویسه *label1,
		ثابت نویسه *file2, ثابت نویسه *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_سطر = outf };
	mmfile_t minus, plus;
	صحیح ret;

	اگر (read_mmfile(&minus, file1))
		بازگشت -1;
	اگر (read_mmfile(&plus, file2)) {
		آزاد(minus.ptr);
		بازگشت -1;
	}

	printf("--- a/%s\n+++ b/%s\n", label1, label2);
	fflush(stdout);
	memset(&xpp, 0, اندازه(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, اندازه(xecfg));
	xecfg.ctxlen = 3;
	ret = xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	آزاد(minus.ptr);
	آزاد(plus.ptr);
	بازگشت ret;
}

صحیح cmd_rerere(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo UNUSED)
{
	ساختار string_list merge_rr = STRING_LIST_INIT_DUP;
	صحیح autoupdate = -1, flags = 0;

	ساختار option options[] = {
		OPT_SET_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, پوچ);

	اگر (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	اگر (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	اگر (argc < 1)
		بازگشت repo_rerere(the_repository, flags);

	اگر (!strcmp(argv[0], "forget")) {
		ساختار pathspec pathspec;
		صحیح ret;

		اگر (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       prefix, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		بازگشت ret;
	}

	اگر (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &merge_rr);
	} وگرنه اگر (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &merge_rr);
	وگرنه اگر (!strcmp(argv[0], "status")) {
		اگر (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			بازگشت 0;
		برای (size_t i = 0; i < merge_rr.nr; i++)
			printf("%s\n", merge_rr.اقلام[i].string);
	} وگرنه اگر (!strcmp(argv[0], "remaining")) {
		rerere_remaining(the_repository, &merge_rr);
		برای (size_t i = 0; i < merge_rr.nr; i++) {
			اگر (merge_rr.اقلام[i].util != RERERE_RESOLVED)
				printf("%s\n", merge_rr.اقلام[i].string);
			وگرنه
				/* prepare for later call to
				 * string_list_clear() */
				merge_rr.اقلام[i].util = پوچ;
		}
	} وگرنه اگر (!strcmp(argv[0], "diff")) {
		ساختار strbuf buf = STRBUF_INIT;
		اگر (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			بازگشت 0;
		برای (size_t i = 0; i < merge_rr.nr; i++) {
			ثابت نویسه *path = merge_rr.اقلام[i].string;
			ثابت ساختار rerere_id *id = merge_rr.اقلام[i].util;
			اگر (diff_two(rerere_path(&buf, id, "preimage"), path, path, path))
				die(_("unable to generate diff for '%s'"), rerere_path(&buf, id, پوچ));
		}

		strbuf_release(&buf);
	} وگرنه
		usage_with_options(rerere_usage, options);

	string_list_clear(&merge_rr, 1);
	بازگشت 0;
}
