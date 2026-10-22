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

স্থির পূর্ণসংখ্যা show_merge_base(কাঠামো commit **rev, আকার_t rev_nr, পূর্ণসংখ্যা show_all)
{
	গণনা merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	কাঠামো commit_list *ফলাফল = ফাঁকা, *r;

	যদি (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &ফলাফল) < 0) {
		commit_list_free(ফলাফল);
		ফেরত -1;
	}

	যদি (!ফলাফল)
		ফেরত 1;

	জন্য (r = ফলাফল; r; r = r->পরবর্তী) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		যদি (!show_all)
			বিরতি;
	}

	commit_list_free(ফলাফল);
	ফেরত 0;
}

স্থির ধ্রুবক অক্ষর * ধ্রুবক merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	ফাঁকা
};

স্থির কাঠামো commit *get_commit_reference(ধ্রুবক অক্ষর *arg)
{
	কাঠামো object_id revkey;
	কাঠামো commit *r;

	যদি (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	যদি (!r)
		die("Not a valid commit name %s", arg);

	ফেরত r;
}

স্থির পূর্ণসংখ্যা handle_independent(পূর্ণসংখ্যা গুনতি, ধ্রুবক অক্ষর **args)
{
	কাঠামো commit_list *revs = ফাঁকা, *rev;
	পূর্ণসংখ্যা i;

	জন্য (i = গুনতি - 1; i >= 0; i--)
		commit_list_ঢোকাও(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	যদি (!revs)
		ফেরত 1;

	জন্য (rev = revs; rev; rev = rev->পরবর্তী)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা handle_octopus(পূর্ণসংখ্যা গুনতি, ধ্রুবক অক্ষর **args, পূর্ণসংখ্যা show_all)
{
	কাঠামো commit_list *revs = ফাঁকা;
	কাঠামো commit_list *ফলাফল = ফাঁকা, *rev;
	পূর্ণসংখ্যা i;

	জন্য (i = গুনতি - 1; i >= 0; i--)
		commit_list_ঢোকাও(get_commit_reference(args[i]), &revs);

	যদি (get_octopus_merge_bases(revs, &ফলাফল) < 0) {
		commit_list_free(revs);
		commit_list_free(ফলাফল);
		ফেরত 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&ফলাফল);

	যদি (!ফলাফল)
		ফেরত 1;

	জন্য (rev = ফলাফল; rev; rev = rev->পরবর্তী) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		যদি (!show_all)
			বিরতি;
	}

	commit_list_free(ফলাফল);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা handle_is_ancestor(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	কাঠামো commit *one, *two;
	পূর্ণসংখ্যা ret;

	যদি (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	যদি (ret < 0)
		exit(128);
	যদি (ret)
		ফেরত 0;
	অন্যথা
		ফেরত 1;
}

স্থির পূর্ণসংখ্যা handle_fork_point(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	কাঠামো object_id oid;
	কাঠামো commit *derived, *fork_point;
	ধ্রুবক অক্ষর *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	যদি (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	যদি (!fork_point)
		ফেরত 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	ফেরত 0;
}

পূর্ণসংখ্যা cmd_merge_base(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *prefix,
		   কাঠামো repository *repo UNUSED)
{
	কাঠামো commit **rev;
	আকার_t rev_nr = 0;
	পূর্ণসংখ্যা show_all = 0;
	পূর্ণসংখ্যা cmdmode = 0;
	পূর্ণসংখ্যা ret;

	কাঠামো option options[] = {
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

	repo_config(the_repository, git_default_config, ফাঁকা);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	যদি (cmdmode == 'a') {
		যদি (argc < 2)
			usage_with_options(merge_base_usage, options);
		যদি (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		ফেরত handle_is_ancestor(argc, argv);
	}

	যদি (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	যদি (cmdmode == 'o')
		ফেরত handle_octopus(argc, argv, show_all);

	যদি (cmdmode == 'r')
		ফেরত handle_independent(argc, argv);

	যদি (cmdmode == 'f') {
		যদি (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		ফেরত handle_fork_point(argc, argv);
	}

	যদি (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	যখন (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	মুক্ত(rev);
	ফেরত ret;
}
