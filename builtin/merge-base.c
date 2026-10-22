#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "commit-reach.h"

స్థిర పూర్ణాంకం show_విలీనం_పునాది(నిర్మాణం commit **rev, కొలత_t rev_nr, పూర్ణాంకం show_all)
{
	గణన విలీనం_పునాది_flags flags = show_all ? విలీనం_పునాది_కనుగొను_ALL : 0;
	నిర్మాణం commit_list *ఫలితం = ఖాళీ, *r;

	ఒకవేళ (repo_get_విలీనం_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &ఫలితం) < 0) {
		commit_list_free(ఫలితం);
		తిరిగి -1;
	}

	ఒకవేళ (!ఫలితం)
		తిరిగి 1;

	కొరకు (r = ఫలితం; r; r = r->తదుపరి) {
		రాయి("%s\n", oid_గమ్యం_hex(&r->అంశం->object.oid));
		ఒకవేళ (!show_all)
			ఆపు;
	}

	commit_list_free(ఫలితం);
	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి విలీనం_పునాది_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	ఖాళీ
};

స్థిర నిర్మాణం commit *get_commit_reference(స్థిరరాశి అక్షరం *arg)
{
	నిర్మాణం object_id revkey;
	నిర్మాణం commit *r;

	ఒకవేళ (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	ఒకవేళ (!r)
		die("Not a valid commit name %s", arg);

	తిరిగి r;
}

స్థిర పూర్ణాంకం handle_independent(పూర్ణాంకం లెక్క, స్థిరరాశి అక్షరం **వాదనలు)
{
	నిర్మాణం commit_list *revs = ఖాళీ, *rev;
	పూర్ణాంకం i;

	కొరకు (i = లెక్క - 1; i >= 0; i--)
		commit_list_చొప్పించు(get_commit_reference(వాదనలు[i]), &revs);

	reduce_heads_replace(&revs);

	ఒకవేళ (!revs)
		తిరిగి 1;

	కొరకు (rev = revs; rev; rev = rev->తదుపరి)
		రాయి("%s\n", oid_గమ్యం_hex(&rev->అంశం->object.oid));

	commit_list_free(revs);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం handle_octopus(పూర్ణాంకం లెక్క, స్థిరరాశి అక్షరం **వాదనలు, పూర్ణాంకం show_all)
{
	నిర్మాణం commit_list *revs = ఖాళీ;
	నిర్మాణం commit_list *ఫలితం = ఖాళీ, *rev;
	పూర్ణాంకం i;

	కొరకు (i = లెక్క - 1; i >= 0; i--)
		commit_list_చొప్పించు(get_commit_reference(వాదనలు[i]), &revs);

	ఒకవేళ (get_octopus_విలీనం_bases(revs, &ఫలితం) < 0) {
		commit_list_free(revs);
		commit_list_free(ఫలితం);
		తిరిగి 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&ఫలితం);

	ఒకవేళ (!ఫలితం)
		తిరిగి 1;

	కొరకు (rev = ఫలితం; rev; rev = rev->తదుపరి) {
		రాయి("%s\n", oid_గమ్యం_hex(&rev->అంశం->object.oid));
		ఒకవేళ (!show_all)
			ఆపు;
	}

	commit_list_free(ఫలితం);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం handle_is_ancestor(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	నిర్మాణం commit *one, *two;
	పూర్ణాంకం ret;

	ఒకవేళ (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_లోపల_విలీనం_bases(the_repository, one, two);
	ఒకవేళ (ret < 0)
		నిష్క్రమించు(128);
	ఒకవేళ (ret)
		తిరిగి 0;
	లేకపోతే
		తిరిగి 1;
}

స్థిర పూర్ణాంకం handle_fork_బిందువు(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	నిర్మాణం object_id oid;
	నిర్మాణం commit *derived, *fork_బిందువు;
	స్థిరరాశి అక్షరం *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	ఒకవేళ (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_బిందువు = get_fork_బిందువు(argv[0], derived);

	ఒకవేళ (!fork_బిందువు)
		తిరిగి 1;

	రాయి("%s\n", oid_గమ్యం_hex(&fork_బిందువు->object.oid));
	తిరిగి 0;
}

పూర్ణాంకం cmd_విలీనం_పునాది(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం commit **rev;
	కొలత_t rev_nr = 0;
	పూర్ణాంకం show_all = 0;
	పూర్ణాంకం cmdmode = 0;
	పూర్ణాంకం ret;

	నిర్మాణం option options[] = {
		OPT_BOOL('a', "all", &show_all, N_("output all common ancestors")),
		OPT_CMDMODE(0, "octopus", &cmdmode,
			    N_("find ancestors for a single n-way merge"), 'o'),
		OPT_CMDMODE(0, "independent", &cmdmode,
			    N_("list revs not reachable from others"), 'r'),
		OPT_CMDMODE(0, "is-ancestor", &cmdmode,
			    N_("is the first one ancestor of the other?"), 'a'),
		OPT_CMDMODE(0, "fork-point", &cmdmode,
			    N_("find where <commit> forked from reflog of <ref>"), 'f'),
		OPT_చివర()
	};

	repo_config(the_repository, git_default_config, ఖాళీ);
	argc = parse_options(argc, argv, ఉపసర్గ, options, విలీనం_పునాది_usage, 0);

	ఒకవేళ (cmdmode == 'a') {
		ఒకవేళ (argc < 2)
			usage_with_options(విలీనం_పునాది_usage, options);
		ఒకవేళ (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		తిరిగి handle_is_ancestor(argc, argv);
	}

	ఒకవేళ (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	ఒకవేళ (cmdmode == 'o')
		తిరిగి handle_octopus(argc, argv, show_all);

	ఒకవేళ (cmdmode == 'r')
		తిరిగి handle_independent(argc, argv);

	ఒకవేళ (cmdmode == 'f') {
		ఒకవేళ (argc < 1 || 2 < argc)
			usage_with_options(విలీనం_పునాది_usage, options);
		తిరిగి handle_fork_బిందువు(argc, argv);
	}

	ఒకవేళ (argc < 2)
		usage_with_options(విలీనం_పునాది_usage, options);

	ALLOC_ARRAY(rev, argc);
	అయితే (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_విలీనం_పునాది(rev, rev_nr, show_all);
	విడుదల(rev);
	తిరిగి ret;
}
