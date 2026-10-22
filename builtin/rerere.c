#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

ساکن ثابت حرف * ثابت rerere_usage[] = {
	N_("git rerere [clear | forget <pathspec>... | diff | status | remaining | gc]"),
	تہی,
};

ساکن صحیح outf(خالی *dummy غیر_مستعمل, mmfile_t *ptr, صحیح nbuf)
{
	صحیح i;
	برائے (i = 0; i < nbuf; i++)
		اگر (write_اندر_بھرا(1, ptr[i].ptr, ptr[i].سائز) < 0)
			واپس -1;
	واپس 0;
}

ساکن صحیح diff_two(ثابت حرف *file1, ثابت حرف *label1,
		ثابت حرف *file2, ثابت حرف *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .برآمد_سطر = outf };
	mmfile_t minus, plus;
	صحیح ret;

	اگر (read_mmfile(&minus, file1))
		واپس -1;
	اگر (read_mmfile(&plus, file2)) {
		آزاد(minus.ptr);
		واپس -1;
	}

	لکھو("--- a/%s\n+++ b/%s\n", label1, label2);
	fflush(stdout);
	memset(&xpp, 0, حجم(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, حجم(xecfg));
	xecfg.ctxlen = 3;
	ret = xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	آزاد(minus.ptr);
	آزاد(plus.ptr);
	واپس ret;
}

صحیح cmd_rerere(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo غیر_مستعمل)
{
	ساخت string_list ملاؤ_rr = STRING_LIST_INIT_DUP;
	صحیح autoupdate = -1, flags = 0;

	ساخت option options[] = {
		OPT_سیٹ_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_انتہا(),
	};

	argc = parse_options(argc, argv, سابقہ, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, تہی);

	اگر (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	اگر (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	اگر (argc < 1)
		واپس repo_rerere(the_repository, flags);

	اگر (!strcmp(argv[0], "forget")) {
		ساخت pathspec pathspec;
		صحیح ret;

		اگر (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       سابقہ, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		واپس ret;
	}

	اگر (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &ملاؤ_rr);
	} ورنہ اگر (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &ملاؤ_rr);
	ورنہ اگر (!strcmp(argv[0], "status")) {
		اگر (تیاری_rerere(the_repository, &ملاؤ_rr,
				 flags | RERERE_READONLY) < 0)
			واپس 0;
		برائے (سائز_t i = 0; i < ملاؤ_rr.nr; i++)
			لکھو("%s\n", ملاؤ_rr.اشیاء[i].string);
	} ورنہ اگر (!strcmp(argv[0], "remaining")) {
		rerere_بقایا(the_repository, &ملاؤ_rr);
		برائے (سائز_t i = 0; i < ملاؤ_rr.nr; i++) {
			اگر (ملاؤ_rr.اشیاء[i].util != RERERE_RESOLVED)
				لکھو("%s\n", ملاؤ_rr.اشیاء[i].string);
			ورنہ
				/* prepare for later call to
				 * string_list_clear() */
				ملاؤ_rr.اشیاء[i].util = تہی;
		}
	} ورنہ اگر (!strcmp(argv[0], "diff")) {
		ساخت strbuf buf = STRBUF_INIT;
		اگر (تیاری_rerere(the_repository, &ملاؤ_rr,
				 flags | RERERE_READONLY) < 0)
			واپس 0;
		برائے (سائز_t i = 0; i < ملاؤ_rr.nr; i++) {
			ثابت حرف *راستہ = ملاؤ_rr.اشیاء[i].string;
			ثابت ساخت rerere_id *id = ملاؤ_rr.اشیاء[i].util;
			اگر (diff_two(rerere_راستہ(&buf, id, "preimage"), راستہ, راستہ, راستہ))
				die(_("unable to generate diff for '%s'"), rerere_راستہ(&buf, id, تہی));
		}

		strbuf_release(&buf);
	} ورنہ
		usage_with_options(rerere_usage, options);

	string_list_clear(&ملاؤ_rr, 1);
	واپس 0;
}
