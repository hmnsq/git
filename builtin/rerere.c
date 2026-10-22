#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

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

ساكن صحيح outf(فراغ *dummy اڻ_استعمال_ٿيل, mmfile_t *ptr, صحيح nbuf)
{
	صحيح i;
	لكل (i = 0; i < nbuf; i++)
		إذا (write_اندريون_ڀريل(1, ptr[i].ptr, ptr[i].سائيز) < 0)
			أرجع -1;
	أرجع 0;
}

ساكن صحيح diff_two(ثابت حرف *file1, ثابت حرف *label1,
		ثابت حرف *file2, ثابت حرف *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .ٻاهريون_سٽ = outf };
	mmfile_t minus, plus;
	صحيح ret;

	إذا (read_mmfile(&minus, file1))
		أرجع -1;
	إذا (read_mmfile(&plus, file2)) {
		حرر(minus.ptr);
		أرجع -1;
	}

	لکو("--- a/%s\n+++ b/%s\n", label1, label2);
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
	       ثابت حرف *اڳياڙي,
	       بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية string_list ملايو_rr = STRING_LIST_INIT_DUP;
	صحيح autoupdate = -1, flags = 0;

	بنية option options[] = {
		OPT_سيٽ_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_پڇاڙي(),
	};

	argc = parse_options(argc, argv, اڳياڙي, options, rerere_usage, 0);

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
			       اڳياڙي, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		أرجع ret;
	}

	إذا (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &ملايو_rr);
	} وإلا إذا (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &ملايو_rr);
	وإلا إذا (!strcmp(argv[0], "status")) {
		إذا (تياري_rerere(the_repository, &ملايو_rr,
				 flags | RERERE_READONLY) < 0)
			أرجع 0;
		لكل (سائيز_t i = 0; i < ملايو_rr.nr; i++)
			لکو("%s\n", ملايو_rr.شيون[i].string);
	} وإلا إذا (!strcmp(argv[0], "remaining")) {
		rerere_بچيل(the_repository, &ملايو_rr);
		لكل (سائيز_t i = 0; i < ملايو_rr.nr; i++) {
			إذا (ملايو_rr.شيون[i].util != RERERE_RESOLVED)
				لکو("%s\n", ملايو_rr.شيون[i].string);
			وإلا
				/* prepare for later call to
				 * string_list_clear() */
				ملايو_rr.شيون[i].util = عدم;
		}
	} وإلا إذا (!strcmp(argv[0], "diff")) {
		بنية strbuf buf = STRBUF_INIT;
		إذا (تياري_rerere(the_repository, &ملايو_rr,
				 flags | RERERE_READONLY) < 0)
			أرجع 0;
		لكل (سائيز_t i = 0; i < ملايو_rr.nr; i++) {
			ثابت حرف *رستو = ملايو_rr.شيون[i].string;
			ثابت بنية rerere_id *id = ملايو_rr.شيون[i].util;
			إذا (diff_two(rerere_رستو(&buf, id, "preimage"), رستو, رستو, رستو))
				die(_("unable to generate diff for '%s'"), rerere_رستو(&buf, id, عدم));
		}

		strbuf_release(&buf);
	} وإلا
		usage_with_options(rerere_usage, options);

	string_list_clear(&ملايو_rr, 1);
	أرجع 0;
}
