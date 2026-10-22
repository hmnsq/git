#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "rerere.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "xdiff/xdiff.h"
#અન્તર્ભાવ "xdiff-interface.h"
#અન્તર્ભાવ "pathspec.h"

સ્થિર અચલ અક્ષર * અચલ rerere_usage[] = {
	N_("git rerere [clear | forget <pathspec>... | diff | status | remaining | gc]"),
	ખાલી,
};

સ્થિર પૂર્ણાંક outf(શૂન્ય *dummy UNUSED, mmfile_t *ptr, પૂર્ણાંક nbuf)
{
	પૂર્ણાંક i;
	માટે (i = 0; i < nbuf; i++)
		જો (write_in_full(1, ptr[i].ptr, ptr[i].size) < 0)
			પાછા -1;
	પાછા 0;
}

સ્થિર પૂર્ણાંક diff_two(અચલ અક્ષર *file1, અચલ અક્ષર *label1,
		અચલ અક્ષર *file2, અચલ અક્ષર *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_લીટી = outf };
	mmfile_t minus, plus;
	પૂર્ણાંક ret;

	જો (read_mmfile(&minus, file1))
		પાછા -1;
	જો (read_mmfile(&plus, file2)) {
		મુક્ત(minus.ptr);
		પાછા -1;
	}

	printf("--- a/%s\n+++ b/%s\n", label1, label2);
	fflush(stdout);
	memset(&xpp, 0, કદ(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, કદ(xecfg));
	xecfg.ctxlen = 3;
	ret = xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	મુક્ત(minus.ptr);
	મુક્ત(plus.ptr);
	પાછા ret;
}

પૂર્ણાંક cmd_rerere(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo UNUSED)
{
	સંરચના string_list merge_rr = STRING_LIST_INIT_DUP;
	પૂર્ણાંક autoupdate = -1, flags = 0;

	સંરચના option options[] = {
		OPT_SET_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, ખાલી);

	જો (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	જો (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	જો (argc < 1)
		પાછા repo_rerere(the_repository, flags);

	જો (!strcmp(argv[0], "forget")) {
		સંરચના pathspec pathspec;
		પૂર્ણાંક ret;

		જો (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       prefix, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		પાછા ret;
	}

	જો (!strcmp(argv[0], "clear")) {
		rerere_clear(the_repository, &merge_rr);
	} નહીંતર જો (!strcmp(argv[0], "gc"))
		rerere_gc(the_repository, &merge_rr);
	નહીંતર જો (!strcmp(argv[0], "status")) {
		જો (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			પાછા 0;
		માટે (size_t i = 0; i < merge_rr.nr; i++)
			printf("%s\n", merge_rr.વસ્તુઓ[i].string);
	} નહીંતર જો (!strcmp(argv[0], "remaining")) {
		rerere_remaining(the_repository, &merge_rr);
		માટે (size_t i = 0; i < merge_rr.nr; i++) {
			જો (merge_rr.વસ્તુઓ[i].util != RERERE_RESOLVED)
				printf("%s\n", merge_rr.વસ્તુઓ[i].string);
			નહીંતર
				/* prepare for later call to
				 * string_list_clear() */
				merge_rr.વસ્તુઓ[i].util = ખાલી;
		}
	} નહીંતર જો (!strcmp(argv[0], "diff")) {
		સંરચના strbuf buf = STRBUF_INIT;
		જો (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			પાછા 0;
		માટે (size_t i = 0; i < merge_rr.nr; i++) {
			અચલ અક્ષર *path = merge_rr.વસ્તુઓ[i].string;
			અચલ સંરચના rerere_id *id = merge_rr.વસ્તુઓ[i].util;
			જો (diff_two(rerere_path(&buf, id, "preimage"), path, path, path))
				die(_("unable to generate diff for '%s'"), rerere_path(&buf, id, ખાલી));
		}

		strbuf_release(&buf);
	} નહીંતર
		usage_with_options(rerere_usage, options);

	string_list_clear(&merge_rr, 1);
	પાછા 0;
}
