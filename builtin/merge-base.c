#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "commit-reach.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ show_merge_base(ഘടന commit **rev, size_t rev_nr, പൂർണ്ണസംഖ്യ show_all)
{
	എണ്ണൽ merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	ഘടന commit_list *result = ശൂന്യ, *r;

	എങ്കിൽ (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		തിരികെ -1;
	}

	എങ്കിൽ (!result)
		തിരികെ 1;

	വേണ്ടി (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		എങ്കിൽ (!show_all)
			നിർത്തുക;
	}

	commit_list_free(result);
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	ശൂന്യ
};

സ്ഥിതം ഘടന commit *get_commit_reference(സ്ഥിരം അക്ഷരം *arg)
{
	ഘടന object_id revkey;
	ഘടന commit *r;

	എങ്കിൽ (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	എങ്കിൽ (!r)
		die("Not a valid commit name %s", arg);

	തിരികെ r;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ handle_independent(പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം അക്ഷരം **args)
{
	ഘടന commit_list *revs = ശൂന്യ, *rev;
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = എണ്ണം - 1; i >= 0; i--)
		commit_list_തിരുകുക(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	എങ്കിൽ (!revs)
		തിരികെ 1;

	വേണ്ടി (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ handle_octopus(പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം അക്ഷരം **args, പൂർണ്ണസംഖ്യ show_all)
{
	ഘടന commit_list *revs = ശൂന്യ;
	ഘടന commit_list *result = ശൂന്യ, *rev;
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = എണ്ണം - 1; i >= 0; i--)
		commit_list_തിരുകുക(get_commit_reference(args[i]), &revs);

	എങ്കിൽ (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		തിരികെ 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	എങ്കിൽ (!result)
		തിരികെ 1;

	വേണ്ടി (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		എങ്കിൽ (!show_all)
			നിർത്തുക;
	}

	commit_list_free(result);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ handle_is_ancestor(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	ഘടന commit *one, *two;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	എങ്കിൽ (ret < 0)
		exit(128);
	എങ്കിൽ (ret)
		തിരികെ 0;
	അല്ലെങ്കിൽ
		തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ handle_fork_point(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	ഘടന object_id oid;
	ഘടന commit *derived, *fork_point;
	സ്ഥിരം അക്ഷരം *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	എങ്കിൽ (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	എങ്കിൽ (!fork_point)
		തിരികെ 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_merge_base(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	ഘടന commit **rev;
	size_t rev_nr = 0;
	പൂർണ്ണസംഖ്യ show_all = 0;
	പൂർണ്ണസംഖ്യ cmdmode = 0;
	പൂർണ്ണസംഖ്യ ret;

	ഘടന option options[] = {
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

	repo_config(the_repository, git_default_config, ശൂന്യ);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	എങ്കിൽ (cmdmode == 'a') {
		എങ്കിൽ (argc < 2)
			usage_with_options(merge_base_usage, options);
		എങ്കിൽ (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		തിരികെ handle_is_ancestor(argc, argv);
	}

	എങ്കിൽ (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	എങ്കിൽ (cmdmode == 'o')
		തിരികെ handle_octopus(argc, argv, show_all);

	എങ്കിൽ (cmdmode == 'r')
		തിരികെ handle_independent(argc, argv);

	എങ്കിൽ (cmdmode == 'f') {
		എങ്കിൽ (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		തിരികെ handle_fork_point(argc, argv);
	}

	എങ്കിൽ (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	ആകുമ്പോൾ (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	സ്വതന്ത്രം(rev);
	തിരികെ ret;
}
