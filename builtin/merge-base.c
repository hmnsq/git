#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "commit.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "commit-reach.h"

अटल पूर्णांक show_merge_base(काष्ठा commit **rev, माप_प्रकार rev_nr, पूर्णांक show_all)
{
	क्रमागत merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	काष्ठा commit_list *परिणाम = शून्य, *r;

	अगर (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &परिणाम) < 0) {
		commit_list_free(परिणाम);
		वापस -1;
	}

	अगर (!परिणाम)
		वापस 1;

	क्रम (r = परिणाम; r; r = r->अग्र) {
		म_लिखो("%s\n", oid_to_hex(&r->item->object.oid));
		अगर (!show_all)
			अवरोध;
	}

	commit_list_free(परिणाम);
	वापस 0;
}

अटल स्थिर अक्षर * स्थिर merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	शून्य
};

अटल काष्ठा commit *get_commit_reference(स्थिर अक्षर *arg)
{
	काष्ठा object_id revkey;
	काष्ठा commit *r;

	अगर (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	अगर (!r)
		die("Not a valid commit name %s", arg);

	वापस r;
}

अटल पूर्णांक handle_independent(पूर्णांक गिनती, स्थिर अक्षर **args)
{
	काष्ठा commit_list *revs = शून्य, *rev;
	पूर्णांक i;

	क्रम (i = गिनती - 1; i >= 0; i--)
		commit_list_जोड़ो(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	अगर (!revs)
		वापस 1;

	क्रम (rev = revs; rev; rev = rev->अग्र)
		म_लिखो("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	वापस 0;
}

अटल पूर्णांक handle_octopus(पूर्णांक गिनती, स्थिर अक्षर **args, पूर्णांक show_all)
{
	काष्ठा commit_list *revs = शून्य;
	काष्ठा commit_list *परिणाम = शून्य, *rev;
	पूर्णांक i;

	क्रम (i = गिनती - 1; i >= 0; i--)
		commit_list_जोड़ो(get_commit_reference(args[i]), &revs);

	अगर (get_octopus_merge_bases(revs, &परिणाम) < 0) {
		commit_list_free(revs);
		commit_list_free(परिणाम);
		वापस 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&परिणाम);

	अगर (!परिणाम)
		वापस 1;

	क्रम (rev = परिणाम; rev; rev = rev->अग्र) {
		म_लिखो("%s\n", oid_to_hex(&rev->item->object.oid));
		अगर (!show_all)
			अवरोध;
	}

	commit_list_free(परिणाम);
	वापस 0;
}

अटल पूर्णांक handle_is_ancestor(पूर्णांक argc, स्थिर अक्षर **argv)
{
	काष्ठा commit *one, *two;
	पूर्णांक ret;

	अगर (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	अगर (ret < 0)
		निकास(128);
	अगर (ret)
		वापस 0;
	अन्यथा
		वापस 1;
}

अटल पूर्णांक handle_fork_point(पूर्णांक argc, स्थिर अक्षर **argv)
{
	काष्ठा object_id oid;
	काष्ठा commit *derived, *fork_point;
	स्थिर अक्षर *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	अगर (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	अगर (!fork_point)
		वापस 1;

	म_लिखो("%s\n", oid_to_hex(&fork_point->object.oid));
	वापस 0;
}

पूर्णांक cmd_merge_base(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *prefix,
		   काष्ठा repository *repo UNUSED)
{
	काष्ठा commit **rev;
	माप_प्रकार rev_nr = 0;
	पूर्णांक show_all = 0;
	पूर्णांक cmdmode = 0;
	पूर्णांक ret;

	काष्ठा option options[] = {
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

	repo_config(the_repository, git_default_config, शून्य);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	अगर (cmdmode == 'a') {
		अगर (argc < 2)
			usage_with_options(merge_base_usage, options);
		अगर (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		वापस handle_is_ancestor(argc, argv);
	}

	अगर (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	अगर (cmdmode == 'o')
		वापस handle_octopus(argc, argv, show_all);

	अगर (cmdmode == 'r')
		वापस handle_independent(argc, argv);

	अगर (cmdmode == 'f') {
		अगर (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		वापस handle_fork_point(argc, argv);
	}

	अगर (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	जबतक (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	मुक्त(rev);
	वापस ret;
}
