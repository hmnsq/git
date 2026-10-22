#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rerere.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdiff/xdiff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdiff-interface.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ rerere_usage[] = {
	N_("git rerere [clear | forget <pathspec>... | diff | status | remaining | gc]"),
	ਨਿਰਾ,
};

ਸਥਿਰ ਪੂਰਨ outf(ਖਾਲੀ *dummy ਅਣਵਰਤਿਆ, mmfile_t *ptr, ਪੂਰਨ nbuf)
{
	ਪੂਰਨ i;
	ਲਈ (i = 0; i < nbuf; i++)
		ਜੇ (write_ਵਿੱਚ_ਭਰਿਆ(1, ptr[i].ptr, ptr[i].ਮਾਪ) < 0)
			ਵਾਪਸ -1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ diff_two(ਅਚਲ ਅੱਖਰ *file1, ਅਚਲ ਅੱਖਰ *label1,
		ਅਚਲ ਅੱਖਰ *file2, ਅਚਲ ਅੱਖਰ *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .ਬਾਹਰਲਾ_ਸਤਰ = outf };
	mmfile_t minus, plus;
	ਪੂਰਨ ret;

	ਜੇ (read_mmfile(&minus, file1))
		ਵਾਪਸ -1;
	ਜੇ (read_mmfile(&plus, file2)) {
		ਮੁਕਤ(minus.ptr);
		ਵਾਪਸ -1;
	}

	ਲਿਖੋ("--- a/%s\n+++ b/%s\n", label1, label2);
	fflush(stdout);
	memset(&xpp, 0, ਆਕਾਰ(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, ਆਕਾਰ(xecfg));
	xecfg.ctxlen = 3;
	ret = xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	ਮੁਕਤ(minus.ptr);
	ਮੁਕਤ(plus.ptr);
	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_rerere(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ string_list ਮਿਲਾਓ_rr = STRING_LIST_INIT_DUP;
	ਪੂਰਨ autoupdate = -1, flags = 0;

	ਬਣਤਰ option options[] = {
		OPT_ਸੈੱਟ_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_ਸਿਰਾ(),
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, ਨਿਰਾ);

	ਜੇ (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	ਜੇ (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	ਜੇ (argc < 1)
		ਵਾਪਸ repo_rerere(the_repository, flags);

	ਜੇ (!strcmp(argv[0], "forget")) {
		ਬਣਤਰ pathspec pathspec;
		ਪੂਰਨ ret;

		ਜੇ (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       ਅਗੇਤਰ, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		ਵਾਪਸ ret;
	}

	ਜੇ (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &ਮਿਲਾਓ_rr);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &ਮਿਲਾਓ_rr);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(argv[0], "status")) {
		ਜੇ (ਤਿਆਰੀ_rerere(the_repository, &ਮਿਲਾਓ_rr,
				 flags | RERERE_READONLY) < 0)
			ਵਾਪਸ 0;
		ਲਈ (ਮਾਪ_t i = 0; i < ਮਿਲਾਓ_rr.nr; i++)
			ਲਿਖੋ("%s\n", ਮਿਲਾਓ_rr.ਚੀਜ਼ਾਂ[i].string);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(argv[0], "remaining")) {
		rerere_ਬਚਿਆ(the_repository, &ਮਿਲਾਓ_rr);
		ਲਈ (ਮਾਪ_t i = 0; i < ਮਿਲਾਓ_rr.nr; i++) {
			ਜੇ (ਮਿਲਾਓ_rr.ਚੀਜ਼ਾਂ[i].util != RERERE_RESOLVED)
				ਲਿਖੋ("%s\n", ਮਿਲਾਓ_rr.ਚੀਜ਼ਾਂ[i].string);
			ਨਹੀਂਤਾਂ
				/* prepare for later call to
				 * string_list_clear() */
				ਮਿਲਾਓ_rr.ਚੀਜ਼ਾਂ[i].util = ਨਿਰਾ;
		}
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(argv[0], "diff")) {
		ਬਣਤਰ strbuf buf = STRBUF_INIT;
		ਜੇ (ਤਿਆਰੀ_rerere(the_repository, &ਮਿਲਾਓ_rr,
				 flags | RERERE_READONLY) < 0)
			ਵਾਪਸ 0;
		ਲਈ (ਮਾਪ_t i = 0; i < ਮਿਲਾਓ_rr.nr; i++) {
			ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = ਮਿਲਾਓ_rr.ਚੀਜ਼ਾਂ[i].string;
			ਅਚਲ ਬਣਤਰ rerere_id *id = ਮਿਲਾਓ_rr.ਚੀਜ਼ਾਂ[i].util;
			ਜੇ (diff_two(rerere_ਰਸਤਾ(&buf, id, "preimage"), ਰਸਤਾ, ਰਸਤਾ, ਰਸਤਾ))
				die(_("unable to generate diff for '%s'"), rerere_ਰਸਤਾ(&buf, id, ਨਿਰਾ));
		}

		strbuf_release(&buf);
	} ਨਹੀਂਤਾਂ
		usage_with_options(rerere_usage, options);

	string_list_clear(&ਮਿਲਾਓ_rr, 1);
	ਵਾਪਸ 0;
}
