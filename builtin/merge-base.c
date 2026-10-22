#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "commit-reach.h"

நிலையான முழு show_merge_base(கட்டமைப்பு commit **rev, size_t rev_nr, முழு show_all)
{
	எண்ணிக்கை merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	கட்டமைப்பு commit_list *result = வெற்று, *r;

	எனில் (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		திரும்பு -1;
	}

	எனில் (!result)
		திரும்பு 1;

	க்கு (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		எனில் (!show_all)
			நிறுத்து;
	}

	commit_list_free(result);
	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து * மாறிலி merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	வெற்று
};

நிலையான கட்டமைப்பு commit *get_commit_reference(மாறிலி எழுத்து *arg)
{
	கட்டமைப்பு object_id revkey;
	கட்டமைப்பு commit *r;

	எனில் (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	எனில் (!r)
		die("Not a valid commit name %s", arg);

	திரும்பு r;
}

நிலையான முழு handle_independent(முழு மொத்தம், மாறிலி எழுத்து **args)
{
	கட்டமைப்பு commit_list *revs = வெற்று, *rev;
	முழு i;

	க்கு (i = மொத்தம் - 1; i >= 0; i--)
		commit_list_செருகு(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	எனில் (!revs)
		திரும்பு 1;

	க்கு (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	திரும்பு 0;
}

நிலையான முழு handle_octopus(முழு மொத்தம், மாறிலி எழுத்து **args, முழு show_all)
{
	கட்டமைப்பு commit_list *revs = வெற்று;
	கட்டமைப்பு commit_list *result = வெற்று, *rev;
	முழு i;

	க்கு (i = மொத்தம் - 1; i >= 0; i--)
		commit_list_செருகு(get_commit_reference(args[i]), &revs);

	எனில் (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		திரும்பு 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	எனில் (!result)
		திரும்பு 1;

	க்கு (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		எனில் (!show_all)
			நிறுத்து;
	}

	commit_list_free(result);
	திரும்பு 0;
}

நிலையான முழு handle_is_ancestor(முழு argc, மாறிலி எழுத்து **argv)
{
	கட்டமைப்பு commit *one, *two;
	முழு ret;

	எனில் (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	எனில் (ret < 0)
		exit(128);
	எனில் (ret)
		திரும்பு 0;
	இல்லையெனில்
		திரும்பு 1;
}

நிலையான முழு handle_fork_point(முழு argc, மாறிலி எழுத்து **argv)
{
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு commit *derived, *fork_point;
	மாறிலி எழுத்து *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	எனில் (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	எனில் (!fork_point)
		திரும்பு 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	திரும்பு 0;
}

முழு cmd_merge_base(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு commit **rev;
	size_t rev_nr = 0;
	முழு show_all = 0;
	முழு cmdmode = 0;
	முழு ret;

	கட்டமைப்பு option options[] = {
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

	repo_config(the_repository, git_default_config, வெற்று);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	எனில் (cmdmode == 'a') {
		எனில் (argc < 2)
			usage_with_options(merge_base_usage, options);
		எனில் (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		திரும்பு handle_is_ancestor(argc, argv);
	}

	எனில் (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	எனில் (cmdmode == 'o')
		திரும்பு handle_octopus(argc, argv, show_all);

	எனில் (cmdmode == 'r')
		திரும்பு handle_independent(argc, argv);

	எனில் (cmdmode == 'f') {
		எனில் (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		திரும்பு handle_fork_point(argc, argv);
	}

	எனில் (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	போது (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	விடுவி(rev);
	திரும்பு ret;
}
