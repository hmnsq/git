#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "commit.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "commit-reach.h"

ایستا صحیح show_merge_base(ساختار commit **rev, size_t rev_nr, صحیح show_all)
{
	شمارش merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	ساختار commit_list *result = پوچ, *r;

	اگر (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		بازگشت -1;
	}

	اگر (!result)
		بازگشت 1;

	برای (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		اگر (!show_all)
			بشکن;
	}

	commit_list_free(result);
	بازگشت 0;
}

ایستا ثابت نویسه * ثابت merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	پوچ
};

ایستا ساختار commit *get_commit_reference(ثابت نویسه *arg)
{
	ساختار object_id revkey;
	ساختار commit *r;

	اگر (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	اگر (!r)
		die("Not a valid commit name %s", arg);

	بازگشت r;
}

ایستا صحیح handle_independent(صحیح شمار, ثابت نویسه **args)
{
	ساختار commit_list *revs = پوچ, *rev;
	صحیح i;

	برای (i = شمار - 1; i >= 0; i--)
		commit_list_درج(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	اگر (!revs)
		بازگشت 1;

	برای (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	بازگشت 0;
}

ایستا صحیح handle_octopus(صحیح شمار, ثابت نویسه **args, صحیح show_all)
{
	ساختار commit_list *revs = پوچ;
	ساختار commit_list *result = پوچ, *rev;
	صحیح i;

	برای (i = شمار - 1; i >= 0; i--)
		commit_list_درج(get_commit_reference(args[i]), &revs);

	اگر (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		بازگشت 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	اگر (!result)
		بازگشت 1;

	برای (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		اگر (!show_all)
			بشکن;
	}

	commit_list_free(result);
	بازگشت 0;
}

ایستا صحیح handle_is_ancestor(صحیح argc, ثابت نویسه **argv)
{
	ساختار commit *one, *two;
	صحیح ret;

	اگر (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	اگر (ret < 0)
		exit(128);
	اگر (ret)
		بازگشت 0;
	وگرنه
		بازگشت 1;
}

ایستا صحیح handle_fork_point(صحیح argc, ثابت نویسه **argv)
{
	ساختار object_id oid;
	ساختار commit *derived, *fork_point;
	ثابت نویسه *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	اگر (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	اگر (!fork_point)
		بازگشت 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	بازگشت 0;
}

صحیح cmd_merge_base(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix,
		   ساختار repository *repo UNUSED)
{
	ساختار commit **rev;
	size_t rev_nr = 0;
	صحیح show_all = 0;
	صحیح cmdmode = 0;
	صحیح ret;

	ساختار option options[] = {
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

	repo_config(the_repository, git_default_config, پوچ);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	اگر (cmdmode == 'a') {
		اگر (argc < 2)
			usage_with_options(merge_base_usage, options);
		اگر (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		بازگشت handle_is_ancestor(argc, argv);
	}

	اگر (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	اگر (cmdmode == 'o')
		بازگشت handle_octopus(argc, argv, show_all);

	اگر (cmdmode == 'r')
		بازگشت handle_independent(argc, argv);

	اگر (cmdmode == 'f') {
		اگر (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		بازگشت handle_fork_point(argc, argv);
	}

	اگر (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	وقتی (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	آزاد(rev);
	بازگشت ret;
}
