#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_merge_base(ସଂରଚନା commit **rev, size_t rev_nr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_all)
{
	ଗଣନା merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	ସଂରଚନା commit_list *result = ଖାଲି, *r;

	ଯଦି (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (!result)
		ଫେରନ୍ତୁ 1;

	ପାଇଁ (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		ଯଦି (!show_all)
			ଭାଙ୍ଗ;
	}

	commit_list_free(result);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	ଖାଲି
};

ସ୍ଥିର ସଂରଚନା commit *get_commit_reference(ଅଚଳ ଅକ୍ଷର *arg)
{
	ସଂରଚନା object_id revkey;
	ସଂରଚନା commit *r;

	ଯଦି (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	ଯଦି (!r)
		die("Not a valid commit name %s", arg);

	ଫେରନ୍ତୁ r;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା handle_independent(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଅକ୍ଷର **args)
{
	ସଂରଚନା commit_list *revs = ଖାଲି, *rev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = ଗଣନା_2 - 1; i >= 0; i--)
		commit_list_ଭର୍ତ୍ତି(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	ଯଦି (!revs)
		ଫେରନ୍ତୁ 1;

	ପାଇଁ (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା handle_octopus(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଅକ୍ଷର **args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_all)
{
	ସଂରଚନା commit_list *revs = ଖାଲି;
	ସଂରଚନା commit_list *result = ଖାଲି, *rev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = ଗଣନା_2 - 1; i >= 0; i--)
		commit_list_ଭର୍ତ୍ତି(get_commit_reference(args[i]), &revs);

	ଯଦି (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		ଫେରନ୍ତୁ 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	ଯଦି (!result)
		ଫେରନ୍ତୁ 1;

	ପାଇଁ (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		ଯଦି (!show_all)
			ଭାଙ୍ଗ;
	}

	commit_list_free(result);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା handle_is_ancestor(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	ସଂରଚନା commit *one, *two;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	ଯଦି (ret < 0)
		exit(128);
	ଯଦି (ret)
		ଫେରନ୍ତୁ 0;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା handle_fork_point(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	ସଂରଚନା object_id oid;
	ସଂରଚନା commit *derived, *fork_point;
	ଅଚଳ ଅକ୍ଷର *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	ଯଦି (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	ଯଦି (!fork_point)
		ଫେରନ୍ତୁ 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_merge_base(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		   ଅଚଳ ଅକ୍ଷର **argv,
		   ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା commit **rev;
	size_t rev_nr = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_all = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmdmode = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ସଂରଚନା option options[] = {
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

	repo_config(the_repository, git_default_config, ଖାଲି);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	ଯଦି (cmdmode == 'a') {
		ଯଦି (argc < 2)
			usage_with_options(merge_base_usage, options);
		ଯଦି (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		ଫେରନ୍ତୁ handle_is_ancestor(argc, argv);
	}

	ଯଦି (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	ଯଦି (cmdmode == 'o')
		ଫେରନ୍ତୁ handle_octopus(argc, argv, show_all);

	ଯଦି (cmdmode == 'r')
		ଫେରନ୍ତୁ handle_independent(argc, argv);

	ଯଦି (cmdmode == 'f') {
		ଯଦି (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		ଫେରନ୍ତୁ handle_fork_point(argc, argv);
	}

	ଯଦି (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	ଯେତେବେଳେ (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	ମୁକ୍ତ(rev);
	ଫେରନ୍ତୁ ret;
}
