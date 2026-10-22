#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "commit-reach.h"

સ્થિર પૂર્ણાંક show_merge_base(સંરચના commit **rev, size_t rev_nr, પૂર્ણાંક show_all)
{
	ગણના merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	સંરચના commit_list *result = ખાલી, *r;

	જો (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		પાછા -1;
	}

	જો (!result)
		પાછા 1;

	માટે (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		જો (!show_all)
			તોડો;
	}

	commit_list_free(result);
	પાછા 0;
}

સ્થિર અચલ અક્ષર * અચલ merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	ખાલી
};

સ્થિર સંરચના commit *get_commit_reference(અચલ અક્ષર *arg)
{
	સંરચના object_id revkey;
	સંરચના commit *r;

	જો (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	જો (!r)
		die("Not a valid commit name %s", arg);

	પાછા r;
}

સ્થિર પૂર્ણાંક handle_independent(પૂર્ણાંક ગણતરી, અચલ અક્ષર **args)
{
	સંરચના commit_list *revs = ખાલી, *rev;
	પૂર્ણાંક i;

	માટે (i = ગણતરી - 1; i >= 0; i--)
		commit_list_ઉમેરો(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	જો (!revs)
		પાછા 1;

	માટે (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	પાછા 0;
}

સ્થિર પૂર્ણાંક handle_octopus(પૂર્ણાંક ગણતરી, અચલ અક્ષર **args, પૂર્ણાંક show_all)
{
	સંરચના commit_list *revs = ખાલી;
	સંરચના commit_list *result = ખાલી, *rev;
	પૂર્ણાંક i;

	માટે (i = ગણતરી - 1; i >= 0; i--)
		commit_list_ઉમેરો(get_commit_reference(args[i]), &revs);

	જો (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		પાછા 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	જો (!result)
		પાછા 1;

	માટે (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		જો (!show_all)
			તોડો;
	}

	commit_list_free(result);
	પાછા 0;
}

સ્થિર પૂર્ણાંક handle_is_ancestor(પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	સંરચના commit *one, *two;
	પૂર્ણાંક ret;

	જો (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	જો (ret < 0)
		exit(128);
	જો (ret)
		પાછા 0;
	નહીંતર
		પાછા 1;
}

સ્થિર પૂર્ણાંક handle_fork_point(પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	સંરચના object_id oid;
	સંરચના commit *derived, *fork_point;
	અચલ અક્ષર *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	જો (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	જો (!fork_point)
		પાછા 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	પાછા 0;
}

પૂર્ણાંક cmd_merge_base(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix,
		   સંરચના repository *repo UNUSED)
{
	સંરચના commit **rev;
	size_t rev_nr = 0;
	પૂર્ણાંક show_all = 0;
	પૂર્ણાંક cmdmode = 0;
	પૂર્ણાંક ret;

	સંરચના option options[] = {
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

	repo_config(the_repository, git_default_config, ખાલી);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	જો (cmdmode == 'a') {
		જો (argc < 2)
			usage_with_options(merge_base_usage, options);
		જો (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		પાછા handle_is_ancestor(argc, argv);
	}

	જો (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	જો (cmdmode == 'o')
		પાછા handle_octopus(argc, argv, show_all);

	જો (cmdmode == 'r')
		પાછા handle_independent(argc, argv);

	જો (cmdmode == 'f') {
		જો (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		પાછા handle_fork_point(argc, argv);
	}

	જો (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	જ્યારે (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	મુક્ત(rev);
	પાછા ret;
}
