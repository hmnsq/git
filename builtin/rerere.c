#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "gettext.h"
#समावेश "parse-options.h"
#समावेश "rerere.h"
#समावेश "strbuf.h"
#समावेश "string-list.h"
#समावेश "xdiff/xdiff.h"
#समावेश "xdiff-interface.h"
#समावेश "pathspec.h"

अटल स्थिर अक्षर * स्थिर rerere_usage[] = {
	N_("git rerere [clear | forget <pathspec>... | diff | status | remaining | gc]"),
	शून्य,
};

अटल पूर्णांक outf(व्योम *dummy UNUSED, mmfile_t *ptr, पूर्णांक nbuf)
{
	पूर्णांक i;
	क्रम (i = 0; i < nbuf; i++)
		अगर (write_in_full(1, ptr[i].ptr, ptr[i].आकार) < 0)
			वापस -1;
	वापस 0;
}

अटल पूर्णांक diff_two(स्थिर अक्षर *file1, स्थिर अक्षर *label1,
		स्थिर अक्षर *file2, स्थिर अक्षर *label2)
{
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_पंक्ति = outf };
	mmfile_t minus, plus;
	पूर्णांक ret;

	अगर (read_mmfile(&minus, file1))
		वापस -1;
	अगर (read_mmfile(&plus, file2)) {
		मुक्त(minus.ptr);
		वापस -1;
	}

	म_लिखो("--- a/%s\n+++ b/%s\n", label1, label2);
	ख_साफ(मानक_निकास);
	स_रखो(&xpp, 0, माप(xpp));
	xpp.flags = 0;
	स_रखो(&xecfg, 0, माप(xecfg));
	xecfg.ctxlen = 3;
	ret = xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	मुक्त(minus.ptr);
	मुक्त(plus.ptr);
	वापस ret;
}

पूर्णांक cmd_rerere(पूर्णांक argc,
	       स्थिर अक्षर **argv,
	       स्थिर अक्षर *prefix,
	       काष्ठा repository *repo UNUSED)
{
	काष्ठा string_list merge_rr = STRING_LIST_INIT_DUP;
	पूर्णांक autoupdate = -1, flags = 0;

	काष्ठा option options[] = {
		OPT_SET_INT(0, "rerere-autoupdate", &autoupdate,
			N_("register clean resolutions in index"), 1),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, rerere_usage, 0);

	repo_config(the_repository, git_xmerge_config, शून्य);

	अगर (autoupdate == 1)
		flags = RERERE_AUTOUPDATE;
	अगर (autoupdate == 0)
		flags = RERERE_NOAUTOUPDATE;

	अगर (argc < 1)
		वापस repo_rerere(the_repository, flags);

	अगर (!म_भेद(argv[0], "forget")) {
		काष्ठा pathspec pathspec;
		पूर्णांक ret;

		अगर (argc < 2)
			warning(_("'git rerere forget' without paths is deprecated"));
		parse_pathspec(&pathspec, 0, PATHSPEC_PREFER_CWD,
			       prefix, argv + 1);

		ret = rerere_forget(the_repository, &pathspec);

		clear_pathspec(&pathspec);
		वापस ret;
	}

	अगर (!म_भेद(argv[0], "clear")) {
		rerere_clear(the_repository, &merge_rr);
	} अन्यथा अगर (!म_भेद(argv[0], "gc"))
		rerere_gc(the_repository, &merge_rr);
	अन्यथा अगर (!म_भेद(argv[0], "status")) {
		अगर (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			वापस 0;
		क्रम (माप_प्रकार i = 0; i < merge_rr.nr; i++)
			म_लिखो("%s\n", merge_rr.वस्तुएँ[i].string);
	} अन्यथा अगर (!म_भेद(argv[0], "remaining")) {
		rerere_remaining(the_repository, &merge_rr);
		क्रम (माप_प्रकार i = 0; i < merge_rr.nr; i++) {
			अगर (merge_rr.वस्तुएँ[i].util != RERERE_RESOLVED)
				म_लिखो("%s\n", merge_rr.वस्तुएँ[i].string);
			अन्यथा
				/* prepare for later call to
				 * string_list_clear() */
				merge_rr.वस्तुएँ[i].util = शून्य;
		}
	} अन्यथा अगर (!म_भेद(argv[0], "diff")) {
		काष्ठा strbuf buf = STRBUF_INIT;
		अगर (setup_rerere(the_repository, &merge_rr,
				 flags | RERERE_READONLY) < 0)
			वापस 0;
		क्रम (माप_प्रकार i = 0; i < merge_rr.nr; i++) {
			स्थिर अक्षर *path = merge_rr.वस्तुएँ[i].string;
			स्थिर काष्ठा rerere_id *id = merge_rr.वस्तुएँ[i].util;
			अगर (diff_two(rerere_path(&buf, id, "preimage"), path, path, path))
				die(_("unable to generate diff for '%s'"), rerere_path(&buf, id, शून्य));
		}

		strbuf_release(&buf);
	} अन्यथा
		usage_with_options(rerere_usage, options);

	string_list_clear(&merge_rr, 1);
	वापस 0;
}
