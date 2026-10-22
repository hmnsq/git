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

स्थिर पूर्णाङ्क show_merge_base(संरचना commit **rev, माप_प्रकार rev_nr, पूर्णाङ्क show_all)
{
	गणना merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	संरचना commit_list *result = रिक्त, *r;

	यदि (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		प्रत्यागम -1;
	}

	यदि (!result)
		प्रत्यागम 1;

	प्रति (r = result; r; r = r->next) {
		म_लिखो("%s\n", oid_to_hex(&r->item->object.oid));
		यदि (!show_all)
			भङ्ग;
	}

	commit_list_free(result);
	प्रत्यागम 0;
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

	यदि (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	यदि (!r)
		die("Not a valid commit name %s", arg);

	प्रत्यागम r;
}

स्थिर पूर्णाङ्क handle_independent(पूर्णाङ्क गिनती, ध्रुव वर्ण **args)
{
	संरचना commit_list *revs = रिक्त, *rev;
	पूर्णाङ्क i;

	प्रति (i = गिनती - 1; i >= 0; i--)
		commit_list_जोड़ो(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	यदि (!revs)
		प्रत्यागम 1;

	प्रति (rev = revs; rev; rev = rev->next)
		म_लिखो("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क handle_octopus(पूर्णाङ्क गिनती, ध्रुव वर्ण **args, पूर्णाङ्क show_all)
{
	संरचना commit_list *revs = रिक्त;
	संरचना commit_list *result = रिक्त, *rev;
	पूर्णाङ्क i;

	प्रति (i = गिनती - 1; i >= 0; i--)
		commit_list_जोड़ो(get_commit_reference(args[i]), &revs);

	यदि (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		प्रत्यागम 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	यदि (!result)
		प्रत्यागम 1;

	प्रति (rev = result; rev; rev = rev->next) {
		म_लिखो("%s\n", oid_to_hex(&rev->item->object.oid));
		यदि (!show_all)
			भङ्ग;
	}

	commit_list_free(result);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क handle_is_ancestor(पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	संरचना commit *one, *two;
	पूर्णाङ्क ret;

	यदि (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	यदि (ret < 0)
		निकास(128);
	यदि (ret)
		प्रत्यागम 0;
	अन्यथा
		प्रत्यागम 1;
}

स्थिर पूर्णाङ्क handle_fork_point(पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	संरचना object_id oid;
	संरचना commit *derived, *fork_point;
	ध्रुव वर्ण *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	यदि (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	यदि (!fork_point)
		प्रत्यागम 1;

	म_लिखो("%s\n", oid_to_hex(&fork_point->object.oid));
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_merge_base(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *prefix,
		   संरचना repository *repo UNUSED)
{
	संरचना commit **rev;
	माप_प्रकार rev_nr = 0;
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

	यदि (cmdmode == 'a') {
		यदि (argc < 2)
			usage_with_options(merge_base_usage, options);
		यदि (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		प्रत्यागम handle_is_ancestor(argc, argv);
	}

	यदि (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	यदि (cmdmode == 'o')
		प्रत्यागम handle_octopus(argc, argv, show_all);

	यदि (cmdmode == 'r')
		प्रत्यागम handle_independent(argc, argv);

	यदि (cmdmode == 'f') {
		यदि (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		प्रत्यागम handle_fork_point(argc, argv);
	}

	यदि (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	यावत् (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	मुक्त(rev);
	प्रत्यागम ret;
}
