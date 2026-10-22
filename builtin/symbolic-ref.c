#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "strbuf.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	ఖాళీ
};

స్థిర పూర్ణాంకం check_symref(స్థిరరాశి అక్షరం *తల_2, పూర్ణాంకం quiet, పూర్ణాంకం shorten, పూర్ణాంకం recurse, పూర్ణాంకం print)
{
	పూర్ణాంకం resolve_flags, flag;
	స్థిరరాశి అక్షరం *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  తల_2, resolve_flags, ఖాళీ, &flag);

	ఒకవేళ (!refname)
		die("No such ref: %s", తల_2);
	లేకపోతే ఒకవేళ (!(flag & REF_ISSYMREF)) {
		ఒకవేళ (!quiet)
			die("ref %s is not a symbolic ref", తల_2);
		లేకపోతే
			తిరిగి 1;
	}
	ఒకవేళ (print) {
		అక్షరం *గమ్యం_free = ఖాళీ;
		ఒకవేళ (shorten)
			refname = గమ్యం_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		విడుదల(గమ్యం_free);
	}
	తిరిగి 0;
}

పూర్ణాంకం cmd_symbolic_ref(పూర్ణాంకం argc,
		     స్థిరరాశి అక్షరం **argv,
		     స్థిరరాశి అక్షరం *ఉపసర్గ,
		     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	స్థిరరాశి అక్షరం *సందేశం = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', ఖాళీ, &సందేశం, N_("reason"), N_("reason of the update")),
		OPT_చివర(),
	};

	repo_config(the_repository, git_default_config, ఖాళీ);
	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_symbolic_ref_usage, 0);
	ఒకవేళ (సందేశం && !*సందేశం)
		die("Refusing to perform update with empty message");

	ఒకవేళ (delete) {
		ఒకవేళ (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		ఒకవేళ (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		ఒకవేళ (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		తిరిగి refs_delete_ref(get_main_ref_store(the_repository),
				       ఖాళీ, argv[0], ఖాళీ, REF_NO_DEREF);
	}

	ఎంపిక (argc) {
	స్థితి 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		ఆపు;
	స్థితి 2:
		ఒకవేళ (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		ఒకవేళ (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], సందేశం);
		ఆపు;
	అప్రమేయ:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	తిరిగి ret;
}
