#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "commit-reach.h"

স্থির পূর্ণাঙ্ক show_merge_base(সংরচনা commit **rev, size_t rev_nr, পূর্ণাঙ্ক show_all)
{
	গণনা merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	সংরচনা commit_list *result = রিক্ত, *r;

	যদি (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		প্রত্যাগম -1;
	}

	যদি (!result)
		প্রত্যাগম 1;

	প্রতি (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		যদি (!show_all)
			ভঙ্গ;
	}

	commit_list_free(result);
	প্রত্যাগম 0;
}

স্থির ধ্রুব বর্ণ * ধ্রুব merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	রিক্ত
};

স্থির সংরচনা commit *get_commit_reference(ধ্রুব বর্ণ *arg)
{
	সংরচনা object_id revkey;
	সংরচনা commit *r;

	যদি (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	যদি (!r)
		die("Not a valid commit name %s", arg);

	প্রত্যাগম r;
}

স্থির পূর্ণাঙ্ক handle_independent(পূর্ণাঙ্ক গুনতি, ধ্রুব বর্ণ **args)
{
	সংরচনা commit_list *revs = রিক্ত, *rev;
	পূর্ণাঙ্ক i;

	প্রতি (i = গুনতি - 1; i >= 0; i--)
		commit_list_ঢোকাও(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	যদি (!revs)
		প্রত্যাগম 1;

	প্রতি (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক handle_octopus(পূর্ণাঙ্ক গুনতি, ধ্রুব বর্ণ **args, পূর্ণাঙ্ক show_all)
{
	সংরচনা commit_list *revs = রিক্ত;
	সংরচনা commit_list *result = রিক্ত, *rev;
	পূর্ণাঙ্ক i;

	প্রতি (i = গুনতি - 1; i >= 0; i--)
		commit_list_ঢোকাও(get_commit_reference(args[i]), &revs);

	যদি (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		প্রত্যাগম 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	যদি (!result)
		প্রত্যাগম 1;

	প্রতি (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		যদি (!show_all)
			ভঙ্গ;
	}

	commit_list_free(result);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক handle_is_ancestor(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv)
{
	সংরচনা commit *one, *two;
	পূর্ণাঙ্ক ret;

	যদি (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	যদি (ret < 0)
		exit(128);
	যদি (ret)
		প্রত্যাগম 0;
	অন্যথা
		প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক handle_fork_point(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv)
{
	সংরচনা object_id oid;
	সংরচনা commit *derived, *fork_point;
	ধ্রুব বর্ণ *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	যদি (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	যদি (!fork_point)
		প্রত্যাগম 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	প্রত্যাগম 0;
}

পূর্ণাঙ্ক cmd_merge_base(পূর্ণাঙ্ক argc,
		   ধ্রুব বর্ণ **argv,
		   ধ্রুব বর্ণ *prefix,
		   সংরচনা repository *repo UNUSED)
{
	সংরচনা commit **rev;
	size_t rev_nr = 0;
	পূর্ণাঙ্ক show_all = 0;
	পূর্ণাঙ্ক cmdmode = 0;
	পূর্ণাঙ্ক ret;

	সংরচনা option options[] = {
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

	repo_config(the_repository, git_default_config, রিক্ত);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	যদি (cmdmode == 'a') {
		যদি (argc < 2)
			usage_with_options(merge_base_usage, options);
		যদি (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		প্রত্যাগম handle_is_ancestor(argc, argv);
	}

	যদি (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	যদি (cmdmode == 'o')
		প্রত্যাগম handle_octopus(argc, argv, show_all);

	যদি (cmdmode == 'r')
		প্রত্যাগম handle_independent(argc, argv);

	যদি (cmdmode == 'f') {
		যদি (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		প্রত্যাগম handle_fork_point(argc, argv);
	}

	যদি (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	যাবত্ (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	মুক্ত(rev);
	প্রত্যাগম ret;
}
