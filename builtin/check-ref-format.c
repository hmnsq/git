/*
 * GIT - The information manager from hell
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "strbuf.h"

స్థిర స్థిరరాశి అక్షరం builtin_check_ref_format_usage[] =
"git check-ref-format [--normalize] [<options>] <refname>\n"
"   or: git check-ref-format --branch <branchname-shorthand>";

/*
 * Return a copy of refname but with leading slashes removed and runs
 * of adjacent slashes replaced with single slashes.
 *
 * This function is similar to normalize_path_copy(), but stripped down
 * to meet check_ref_format's simpler needs.
 */
స్థిర అక్షరం *collapse_slashes(స్థిరరాశి అక్షరం *refname)
{
	అక్షరం *ret = xmallocz(అక్షరశ్రేణి_పొడవు(refname));
	అక్షరం ch;
	అక్షరం prev = '/';
	అక్షరం *cp = ret;

	అయితే ((ch = *refname++) != '\0') {
		ఒకవేళ (prev == '/' && ch == prev)
			కొనసాగు;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	తిరిగి ret;
}

స్థిర పూర్ణాంకం check_ref_format_branch(స్థిరరాశి అక్షరం *arg)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం *name;
	పూర్ణాంకం nongit;

	అమరిక_git_directory_gently(the_repository, &nongit);
	ఒకవేళ (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_ఉపసర్గ(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	రాయి("%s\n", name);
	strbuf_release(&sb);
	తిరిగి 0;
}

పూర్ణాంకం cmd_check_ref_format(పూర్ణాంకం argc,
			 స్థిరరాశి అక్షరం **argv,
			 స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i;
	పూర్ణాంకం normalize = 0;
	పూర్ణాంకం flags = 0;
	స్థిరరాశి అక్షరం *refname;
	అక్షరం *గమ్యం_free = ఖాళీ;
	పూర్ణాంకం ret = 1;

	BUG_ON_NON_ఖాళీలు_ఉపసర్గ(ఉపసర్గ);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	ఒకవేళ (argc == 3 && !strcmp(argv[1], "--branch"))
		తిరిగి check_ref_format_branch(argv[2]);

	కొరకు (i = 1; i < argc && argv[i][0] == '-'; i++) {
		ఒకవేళ (!strcmp(argv[i], "--normalize") || !strcmp(argv[i], "--print"))
			normalize = 1;
		లేకపోతే ఒకవేళ (!strcmp(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		లేకపోతే ఒకవేళ (!strcmp(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		లేకపోతే ఒకవేళ (!strcmp(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_నమూనా;
		లేకపోతే
			usage(builtin_check_ref_format_usage);
	}
	ఒకవేళ (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	ఒకవేళ (normalize)
		refname = గమ్యం_free = collapse_slashes(refname);
	ఒకవేళ (check_refname_format(refname, flags))
		వెళ్ళు cleanup;
	ఒకవేళ (normalize)
		రాయి("%s\n", refname);

	ret = 0;
cleanup:
	విడుదల(గమ్యం_free);
	తిరిగి ret;
}
