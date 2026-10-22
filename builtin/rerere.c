#تعریف_کن USE_THE_REPOSITORY_VARIABLE

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

ساكن صحيح outf(فراغ *dummy استفادهنشده, mmfile_t *ptr, صحيح nbuf)
{
	صحيح i;
	لكل (i = 0; i < nbuf; i++)
		إذا (write_درون_پر(1, ptr[i].ptr, ptr[i].اندازه) < 0)
			أرجع -1;
	أرجع 0;
}

ساكن صحيح diff_two(ثابت حرف *file1, ثابت حرف *label1,
		ثابت حرف *file2, ثابت حرف *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .بیرون_سطر = outf };
	mmfile_t minus, plus;
	صحيح ret;

	إذا (read_mmfile(&minus, file1))
		أرجع -1;
	إذا (read_mmfile(&plus, file2)) {
		حرر(minus.ptr);
		أرجع -1;
	}

	چاپ_کن("--- a/%s\n+++ b/%s\n", label1, label2);
	fflush(stdout);
	memset(&xpp, 0, حجم(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, حجم(xecfg));
	xecfg.ctxlen = 3;
	ret = xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	حرر(minus.ptr);
	حرر(plus.ptr);
	أرجع ret;
}

صحيح cmd_rerere(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *پیشوند,
	       بنية repository *repo استفادهنشده)
{
	بنية string_list ادغام_rr = STRING_LIST_INIT_DUP;
	صحيح autoupdate = -1, flags = 0;

	بنية option options[] = {
		OPT_مجموعه_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_انتها(),
	};

	argc = parse_options(argc, argv, پیشوند, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, عدم);

	إذا (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	إذا (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	إذا (argc < 1)
		أرجع repo_rerere(the_repository, flags);

	إذا (!strcmp(argv[0], "forget")) {
		بنية pathspec pathspec;
		صحيح ret;

		إذا (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       پیشوند, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		أرجع ret;
	}

	إذا (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &ادغام_rr);
	} وإلا إذا (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &ادغام_rr);
	وإلا إذا (!strcmp(argv[0], "status")) {
		إذا (راهاندازی_rerere(the_repository, &ادغام_rr,
				 flags | RERERE_READONLY) < 0)
			أرجع 0;
		لكل (اندازه_t i = 0; i < ادغام_rr.nr; i++)
			چاپ_کن("%s\n", ادغام_rr.اقلام[i].string);
	} وإلا إذا (!strcmp(argv[0], "remaining")) {
		rerere_مانده(the_repository, &ادغام_rr);
		لكل (اندازه_t i = 0; i < ادغام_rr.nr; i++) {
			إذا (ادغام_rr.اقلام[i].util != RERERE_RESOLVED)
				چاپ_کن("%s\n", ادغام_rr.اقلام[i].string);
			وإلا
				/* prepare for later call to
				 * string_list_clear() */
				ادغام_rr.اقلام[i].util = عدم;
		}
	} وإلا إذا (!strcmp(argv[0], "diff")) {
		بنية strbuf buf = STRBUF_INIT;
		إذا (راهاندازی_rerere(the_repository, &ادغام_rr,
				 flags | RERERE_READONLY) < 0)
			أرجع 0;
		لكل (اندازه_t i = 0; i < ادغام_rr.nr; i++) {
			ثابت حرف *مسیر = ادغام_rr.اقلام[i].string;
			ثابت بنية rerere_id *id = ادغام_rr.اقلام[i].util;
			إذا (diff_two(rerere_مسیر(&buf, id, "preimage"), مسیر, مسیر, مسیر))
				die(_("unable to generate diff for '%s'"), rerere_مسیر(&buf, id, عدم));
		}

		strbuf_release(&buf);
	} وإلا
		usage_with_options(rerere_usage, options);

	string_list_clear(&ادغام_rr, 1);
	أرجع 0;
}
