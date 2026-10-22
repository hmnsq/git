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

स्थिर पूर्णाङ्क show_merge_base(संरचना commit **rev, size_t rev_nr, पूर्णाङ्क show_all)
{
	गणना merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	संरचना commit_list *result = रिक्त, *r;

	सचे (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		पटिनिवत्त -1;
	}

	सचे (!result)
		पटिनिवत्त 1;

	अत्थाय (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		सचे (!show_all)
			भङ्ग;
	}

	commit_list_free(result);
	पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण * ध्रुव merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	रिक्त
};

स्थिर संरचना commit *get_commit_reference(ध्रुव वर्ण *arg)
{
	संरचना object_id revkey;
	संरचना commit *r;

	सचे (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	सचे (!r)
		die("Not a valid commit name %s", arg);

	पटिनिवत्त r;
}

स्थिर पूर्णाङ्क handle_independent(पूर्णाङ्क गणनम्, ध्रुव वर्ण **args)
{
	संरचना commit_list *revs = रिक्त, *rev;
	पूर्णाङ्क i;

	अत्थाय (i = गणनम् - 1; i >= 0; i--)
		commit_list_निवेशय(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	सचे (!revs)
		पटिनिवत्त 1;

	अत्थाय (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क handle_octopus(पूर्णाङ्क गणनम्, ध्रुव वर्ण **args, पूर्णाङ्क show_all)
{
	संरचना commit_list *revs = रिक्त;
	संरचना commit_list *result = रिक्त, *rev;
	पूर्णाङ्क i;

	अत्थाय (i = गणनम् - 1; i >= 0; i--)
		commit_list_निवेशय(get_commit_reference(args[i]), &revs);

	सचे (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		पटिनिवत्त 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	सचे (!result)
		पटिनिवत्त 1;

	अत्थाय (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		सचे (!show_all)
			भङ्ग;
	}

	commit_list_free(result);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क handle_is_ancestor(पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	संरचना commit *one, *two;
	पूर्णाङ्क ret;

	सचे (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	सचे (ret < 0)
		exit(128);
	सचे (ret)
		पटिनिवत्त 0;
	अञ्ञथा
		पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क handle_fork_point(पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	संरचना object_id oid;
	संरचना commit *derived, *fork_point;
	ध्रुव वर्ण *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	सचे (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	सचे (!fork_point)
		पटिनिवत्त 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	पटिनिवत्त 0;
}

पूर्णाङ्क cmd_merge_base(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *prefix,
		   संरचना repository *repo UNUSED)
{
	संरचना commit **rev;
	size_t rev_nr = 0;
	पूर्णाङ्क show_all = 0;
	पूर्णाङ्क cmdmode = 0;
	पूर्णाङ्क ret;

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

	सचे (cmdmode == 'a') {
		सचे (argc < 2)
			usage_with_options(merge_base_usage, options);
		सचे (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		पटिनिवत्त handle_is_ancestor(argc, argv);
	}

	सचे (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	सचे (cmdmode == 'o')
		पटिनिवत्त handle_octopus(argc, argv, show_all);

	सचे (cmdmode == 'r')
		पटिनिवत्त handle_independent(argc, argv);

	सचे (cmdmode == 'f') {
		सचे (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		पटिनिवत्त handle_fork_point(argc, argv);
	}

	सचे (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	याव (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	मुक्त(rev);
	पटिनिवत्त ret;
}
