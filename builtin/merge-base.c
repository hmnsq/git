#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "commit-reach.h"

स्थिर पूर्णांक show_merge_base(संरचना commit **rev, माप_प्रकार rev_nr, पूर्णांक show_all)
{
	गणना merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	संरचना commit_list *result = रिक्त, *r;

	जर (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		परत -1;
	}

	जर (!result)
		परत 1;

	साठी (r = result; r; r = r->next) {
		म_लिखो("%s\n", oid_to_hex(&r->item->object.oid));
		जर (!show_all)
			थांबा;
	}

	commit_list_free(result);
	परत 0;
}

स्थिर स्थिरांक अक्षर * स्थिरांक merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	रिक्त
};

स्थिर संरचना commit *get_commit_reference(स्थिरांक अक्षर *arg)
{
	संरचना object_id revkey;
	संरचना commit *r;

	जर (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	जर (!r)
		die("Not a valid commit name %s", arg);

	परत r;
}

स्थिर पूर्णांक handle_independent(पूर्णांक मोजणी, स्थिरांक अक्षर **args)
{
	संरचना commit_list *revs = रिक्त, *rev;
	पूर्णांक i;

	साठी (i = मोजणी - 1; i >= 0; i--)
		commit_list_घाला(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	जर (!revs)
		परत 1;

	साठी (rev = revs; rev; rev = rev->next)
		म_लिखो("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	परत 0;
}

स्थिर पूर्णांक handle_octopus(पूर्णांक मोजणी, स्थिरांक अक्षर **args, पूर्णांक show_all)
{
	संरचना commit_list *revs = रिक्त;
	संरचना commit_list *result = रिक्त, *rev;
	पूर्णांक i;

	साठी (i = मोजणी - 1; i >= 0; i--)
		commit_list_घाला(get_commit_reference(args[i]), &revs);

	जर (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		परत 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	जर (!result)
		परत 1;

	साठी (rev = result; rev; rev = rev->next) {
		म_लिखो("%s\n", oid_to_hex(&rev->item->object.oid));
		जर (!show_all)
			थांबा;
	}

	commit_list_free(result);
	परत 0;
}

स्थिर पूर्णांक handle_is_ancestor(पूर्णांक argc, स्थिरांक अक्षर **argv)
{
	संरचना commit *one, *two;
	पूर्णांक ret;

	जर (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	जर (ret < 0)
		निकास(128);
	जर (ret)
		परत 0;
	अन्यथा
		परत 1;
}

स्थिर पूर्णांक handle_fork_point(पूर्णांक argc, स्थिरांक अक्षर **argv)
{
	संरचना object_id oid;
	संरचना commit *derived, *fork_point;
	स्थिरांक अक्षर *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	जर (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	जर (!fork_point)
		परत 1;

	म_लिखो("%s\n", oid_to_hex(&fork_point->object.oid));
	परत 0;
}

पूर्णांक cmd_merge_base(पूर्णांक argc,
		   स्थिरांक अक्षर **argv,
		   स्थिरांक अक्षर *prefix,
		   संरचना repository *repo UNUSED)
{
	संरचना commit **rev;
	माप_प्रकार rev_nr = 0;
	पूर्णांक show_all = 0;
	पूर्णांक cmdmode = 0;
	पूर्णांक ret;

	संरचना option options[] = {
		OPT_BOOL('a', "all", &show_all, N_("output all common ancestors")),
		OPT_CMDMODE(0, "octopus", &cmdmode,
			    N_("find ancestors for a single n-way merge"), 'o'),
		OPT_CMDMODE(0, "independent", &cmdmode,
			    N_("list revs not reachable from others"), 'r'),
		OPT_CMDMODE(0, "is-ancestor", &cmdmode,
			    N_("is the first one ancestor of the other?"), 'a'),
		OPT_CMDMODE(0, "fork-point", &cmdmode,
			    N_("find where <commit> forked from reflog of <ref>"), 'f'),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	जर (cmdmode == 'a') {
		जर (argc < 2)
			usage_with_options(merge_base_usage, options);
		जर (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		परत handle_is_ancestor(argc, argv);
	}

	जर (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	जर (cmdmode == 'o')
		परत handle_octopus(argc, argv, show_all);

	जर (cmdmode == 'r')
		परत handle_independent(argc, argv);

	जर (cmdmode == 'f') {
		जर (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		परत handle_fork_point(argc, argv);
	}

	जर (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	पर्यंत (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	मुक्त(rev);
	परत ret;
}
