#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_merge_base(ಸಂರಚನಾ commit **rev, size_t rev_nr, ಪೂರ್ಣಾಙ್ಕ show_all)
{
	ಗಣನಾ merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	ಸಂರಚನಾ commit_list *result = ರಿಕ್ತ, *r;

	ಯದಿ (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (!result)
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತಿ (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		ಯದಿ (!show_all)
			ಭಙ್ಗ;
	}

	commit_list_free(result);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಸಂರಚನಾ commit *get_commit_reference(ಧ್ರುವ ವರ್ಣ *arg)
{
	ಸಂರಚನಾ object_id revkey;
	ಸಂರಚನಾ commit *r;

	ಯದಿ (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	ಯದಿ (!r)
		die("Not a valid commit name %s", arg);

	ಪ್ರತ್ಯಾಗಮ r;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ handle_independent(ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ವರ್ಣ **args)
{
	ಸಂರಚನಾ commit_list *revs = ರಿಕ್ತ, *rev;
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = ಎಣಿಕೆ - 1; i >= 0; i--)
		commit_list_ಸೇರಿಸು(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	ಯದಿ (!revs)
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತಿ (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ handle_octopus(ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ವರ್ಣ **args, ಪೂರ್ಣಾಙ್ಕ show_all)
{
	ಸಂರಚನಾ commit_list *revs = ರಿಕ್ತ;
	ಸಂರಚನಾ commit_list *result = ರಿಕ್ತ, *rev;
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = ಎಣಿಕೆ - 1; i >= 0; i--)
		commit_list_ಸೇರಿಸು(get_commit_reference(args[i]), &revs);

	ಯದಿ (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		ಪ್ರತ್ಯಾಗಮ 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	ಯದಿ (!result)
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತಿ (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		ಯದಿ (!show_all)
			ಭಙ್ಗ;
	}

	commit_list_free(result);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ handle_is_ancestor(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	ಸಂರಚನಾ commit *one, *two;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	ಯದಿ (ret < 0)
		exit(128);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ handle_fork_point(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ commit *derived, *fork_point;
	ಧ್ರುವ ವರ್ಣ *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	ಯದಿ (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	ಯದಿ (!fork_point)
		ಪ್ರತ್ಯಾಗಮ 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_merge_base(ಪೂರ್ಣಾಙ್ಕ argc,
		   ಧ್ರುವ ವರ್ಣ **argv,
		   ಧ್ರುವ ವರ್ಣ *prefix,
		   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ commit **rev;
	size_t rev_nr = 0;
	ಪೂರ್ಣಾಙ್ಕ show_all = 0;
	ಪೂರ್ಣಾಙ್ಕ cmdmode = 0;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಸಂರಚನಾ option options[] = {
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

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	ಯದಿ (cmdmode == 'a') {
		ಯದಿ (argc < 2)
			usage_with_options(merge_base_usage, options);
		ಯದಿ (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		ಪ್ರತ್ಯಾಗಮ handle_is_ancestor(argc, argv);
	}

	ಯದಿ (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	ಯದಿ (cmdmode == 'o')
		ಪ್ರತ್ಯಾಗಮ handle_octopus(argc, argv, show_all);

	ಯದಿ (cmdmode == 'r')
		ಪ್ರತ್ಯಾಗಮ handle_independent(argc, argv);

	ಯದಿ (cmdmode == 'f') {
		ಯದಿ (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		ಪ್ರತ್ಯಾಗಮ handle_fork_point(argc, argv);
	}

	ಯದಿ (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	ಯಾವತ್ (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	ಮುಕ್ತ(rev);
	ಪ್ರತ್ಯಾಗಮ ret;
}
