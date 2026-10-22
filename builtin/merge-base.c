#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "commit.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "commit-reach.h"

ساكن صحيح show_merge_base(بنية commit **rev, size_t rev_nr, صحيح show_all)
{
	تعداد merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	بنية commit_list *result = عدم, *r;

	אם (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		החזר -1;
	}

	אם (!result)
		החזר 1;

	עבור (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		אם (!show_all)
			اكسر;
	}

	commit_list_free(result);
	החזר 0;
}

ساكن ثابت حرف * ثابت merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	عدم
};

ساكن بنية commit *get_commit_reference(ثابت حرف *arg)
{
	بنية object_id revkey;
	بنية commit *r;

	אם (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	אם (!r)
		die("Not a valid commit name %s", arg);

	החזר r;
}

ساكن صحيح handle_independent(صحيح count, ثابت حرف **args)
{
	بنية commit_list *revs = عدم, *rev;
	صحيح i;

	עבור (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	אם (!revs)
		החזר 1;

	עבור (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	החזר 0;
}

ساكن صحيح handle_octopus(صحيح count, ثابت حرف **args, صحيح show_all)
{
	بنية commit_list *revs = عدم;
	بنية commit_list *result = عدم, *rev;
	صحيح i;

	עבור (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	אם (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		החזר 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	אם (!result)
		החזר 1;

	עבור (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		אם (!show_all)
			اكسر;
	}

	commit_list_free(result);
	החזר 0;
}

ساكن صحيح handle_is_ancestor(صحيح argc, ثابت حرف **argv)
{
	بنية commit *one, *two;
	صحيح ret;

	אם (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	אם (ret < 0)
		exit(128);
	אם (ret)
		החזר 0;
	אחרת
		החזר 1;
}

ساكن صحيح handle_fork_point(صحيح argc, ثابت حرف **argv)
{
	بنية object_id oid;
	بنية commit *derived, *fork_point;
	ثابت حرف *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	אם (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	אם (!fork_point)
		החזר 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	החזר 0;
}

صحيح cmd_merge_base(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *prefix,
		   بنية repository *repo UNUSED)
{
	بنية commit **rev;
	size_t rev_nr = 0;
	صحيح show_all = 0;
	صحيح cmdmode = 0;
	صحيح ret;

	بنية option options[] = {
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

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	אם (cmdmode == 'a') {
		אם (argc < 2)
			usage_with_options(merge_base_usage, options);
		אם (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		החזר handle_is_ancestor(argc, argv);
	}

	אם (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	אם (cmdmode == 'o')
		החזר handle_octopus(argc, argv, show_all);

	אם (cmdmode == 'r')
		החזר handle_independent(argc, argv);

	אם (cmdmode == 'f') {
		אם (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		החזר handle_fork_point(argc, argv);
	}

	אם (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	כלעוד (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	حرر(rev);
	החזר ret;
}
