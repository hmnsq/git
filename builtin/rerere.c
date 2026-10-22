#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "rerere.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "xdiff/xdiff.h"
#ಅನ್ತರ್ಭಾವ "xdiff-interface.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ rerere_usage[] = {
	N_("git rerere [clear | forget <pathspec>... | diff | status | remaining | gc]"),
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ outf(ಶೂನ್ಯ *dummy ಬಳಸದ, mmfile_t *ptr, ಪೂರ್ಣಾಂಕ nbuf)
{
	ಪೂರ್ಣಾಂಕ i;
	ಗಾಗಿ (i = 0; i < nbuf; i++)
		ಆದರೆ (write_ಒಳ_ತುಂಬಿದ(1, ptr[i].ptr, ptr[i].ಗಾತ್ರ_2) < 0)
			ಹಿಂತಿರುಗಿ -1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ diff_two(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file1, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label1,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .ಹೊರಹೋಗು_ಸಾಲು = outf };
	mmfile_t minus, plus;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (read_mmfile(&minus, file1))
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (read_mmfile(&plus, file2)) {
		ಬಿಡುಗಡೆ(minus.ptr);
		ಹಿಂತಿರುಗಿ -1;
	}

	ಬರೆ("--- a/%s\n+++ b/%s\n", label1, label2);
	fflush(stdout);
	memset(&xpp, 0, ಗಾತ್ರ(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, ಗಾತ್ರ(xecfg));
	xecfg.ctxlen = 3;
	ret = xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	ಬಿಡುಗಡೆ(minus.ptr);
	ಬಿಡುಗಡೆ(plus.ptr);
	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ cmd_rerere(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ string_list ವಿಲೀನ_rr = STRING_LIST_INIT_DUP;
	ಪೂರ್ಣಾಂಕ autoupdate = -1, flags = 0;

	ರಚನೆ option options[] = {
		OPT_ಗಣ_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_ಕೊನೆ(),
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, ಖಾಲಿ);

	ಆದರೆ (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	ಆದರೆ (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	ಆದರೆ (argc < 1)
		ಹಿಂತಿರುಗಿ repo_rerere(the_repository, flags);

	ಆದರೆ (!strcmp(argv[0], "forget")) {
		ರಚನೆ pathspec pathspec;
		ಪೂರ್ಣಾಂಕ ret;

		ಆದರೆ (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       ಪೂರ್ವಪ್ರತ್ಯಯ, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		ಹಿಂತಿರುಗಿ ret;
	}

	ಆದರೆ (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &ವಿಲೀನ_rr);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &ವಿಲೀನ_rr);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(argv[0], "status")) {
		ಆದರೆ (ಸಿದ್ಧತೆ_rerere(the_repository, &ವಿಲೀನ_rr,
				 flags | RERERE_READONLY) < 0)
			ಹಿಂತಿರುಗಿ 0;
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ವಿಲೀನ_rr.nr; i++)
			ಬರೆ("%s\n", ವಿಲೀನ_rr.ವಸ್ತುಗಳು[i].string);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(argv[0], "remaining")) {
		rerere_ಉಳಿದ(the_repository, &ವಿಲೀನ_rr);
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ವಿಲೀನ_rr.nr; i++) {
			ಆದರೆ (ವಿಲೀನ_rr.ವಸ್ತುಗಳು[i].util != RERERE_RESOLVED)
				ಬರೆ("%s\n", ವಿಲೀನ_rr.ವಸ್ತುಗಳು[i].string);
			ಇಲ್ಲದಿದ್ದರೆ
				/* prepare for later call to
				 * string_list_clear() */
				ವಿಲೀನ_rr.ವಸ್ತುಗಳು[i].util = ಖಾಲಿ;
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(argv[0], "diff")) {
		ರಚನೆ strbuf buf = STRBUF_INIT;
		ಆದರೆ (ಸಿದ್ಧತೆ_rerere(the_repository, &ವಿಲೀನ_rr,
				 flags | RERERE_READONLY) < 0)
			ಹಿಂತಿರುಗಿ 0;
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ವಿಲೀನ_rr.nr; i++) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = ವಿಲೀನ_rr.ವಸ್ತುಗಳು[i].string;
			ಸ್ಥಿರಾಂಕ ರಚನೆ rerere_id *id = ವಿಲೀನ_rr.ವಸ್ತುಗಳು[i].util;
			ಆದರೆ (diff_two(rerere_ಮಾರ್ಗ(&buf, id, "preimage"), ಮಾರ್ಗ, ಮಾರ್ಗ, ಮಾರ್ಗ))
				die(_("unable to generate diff for '%s'"), rerere_ಮಾರ್ಗ(&buf, id, ಖಾಲಿ));
		}

		strbuf_release(&buf);
	} ಇಲ್ಲದಿದ್ದರೆ
		usage_with_options(rerere_usage, options);

	string_list_clear(&ವಿಲೀನ_rr, 1);
	ಹಿಂತಿರುಗಿ 0;
}
