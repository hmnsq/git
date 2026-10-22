#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"

ਸਥਿਰ ਪੂਰਨ show_ਮਿਲਾਓ_ਅਧਾਰ(ਬਣਤਰ commit **rev, ਮਾਪ_t rev_nr, ਪੂਰਨ show_all)
{
	ਗਿਣਤੀ ਮਿਲਾਓ_ਅਧਾਰ_flags flags = show_all ? ਮਿਲਾਓ_ਅਧਾਰ_ਲੱਭੋ_ALL : 0;
	ਬਣਤਰ commit_list *ਨਤੀਜਾ = ਨਿਰਾ, *r;

	ਜੇ (repo_get_ਮਿਲਾਓ_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &ਨਤੀਜਾ) < 0) {
		commit_list_free(ਨਤੀਜਾ);
		ਵਾਪਸ -1;
	}

	ਜੇ (!ਨਤੀਜਾ)
		ਵਾਪਸ 1;

	ਲਈ (r = ਨਤੀਜਾ; r; r = r->ਅਗਲਾ) {
		ਲਿਖੋ("%s\n", oid_ਮੰਜ਼ਿਲ_hex(&r->ਵਸਤੂ->object.oid));
		ਜੇ (!show_all)
			ਰੋਕੋ;
	}

	commit_list_free(ਨਤੀਜਾ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ ਮਿਲਾਓ_ਅਧਾਰ_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਬਣਤਰ commit *get_commit_reference(ਅਚਲ ਅੱਖਰ *arg)
{
	ਬਣਤਰ object_id revkey;
	ਬਣਤਰ commit *r;

	ਜੇ (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	ਜੇ (!r)
		die("Not a valid commit name %s", arg);

	ਵਾਪਸ r;
}

ਸਥਿਰ ਪੂਰਨ handle_independent(ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਅੱਖਰ **ਦਲੀਲਾਂ)
{
	ਬਣਤਰ commit_list *revs = ਨਿਰਾ, *rev;
	ਪੂਰਨ i;

	ਲਈ (i = ਗਿਣਤੀ_2 - 1; i >= 0; i--)
		commit_list_ਪਾਓ(get_commit_reference(ਦਲੀਲਾਂ[i]), &revs);

	reduce_heads_replace(&revs);

	ਜੇ (!revs)
		ਵਾਪਸ 1;

	ਲਈ (rev = revs; rev; rev = rev->ਅਗਲਾ)
		ਲਿਖੋ("%s\n", oid_ਮੰਜ਼ਿਲ_hex(&rev->ਵਸਤੂ->object.oid));

	commit_list_free(revs);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ handle_octopus(ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਅੱਖਰ **ਦਲੀਲਾਂ, ਪੂਰਨ show_all)
{
	ਬਣਤਰ commit_list *revs = ਨਿਰਾ;
	ਬਣਤਰ commit_list *ਨਤੀਜਾ = ਨਿਰਾ, *rev;
	ਪੂਰਨ i;

	ਲਈ (i = ਗਿਣਤੀ_2 - 1; i >= 0; i--)
		commit_list_ਪਾਓ(get_commit_reference(ਦਲੀਲਾਂ[i]), &revs);

	ਜੇ (get_octopus_ਮਿਲਾਓ_bases(revs, &ਨਤੀਜਾ) < 0) {
		commit_list_free(revs);
		commit_list_free(ਨਤੀਜਾ);
		ਵਾਪਸ 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&ਨਤੀਜਾ);

	ਜੇ (!ਨਤੀਜਾ)
		ਵਾਪਸ 1;

	ਲਈ (rev = ਨਤੀਜਾ; rev; rev = rev->ਅਗਲਾ) {
		ਲਿਖੋ("%s\n", oid_ਮੰਜ਼ਿਲ_hex(&rev->ਵਸਤੂ->object.oid));
		ਜੇ (!show_all)
			ਰੋਕੋ;
	}

	commit_list_free(ਨਤੀਜਾ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ handle_is_ancestor(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਣਤਰ commit *one, *two;
	ਪੂਰਨ ret;

	ਜੇ (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_ਵਿੱਚ_ਮਿਲਾਓ_bases(the_repository, one, two);
	ਜੇ (ret < 0)
		ਬਾਹਰ_ਨਿਕਲੋ(128);
	ਜੇ (ret)
		ਵਾਪਸ 0;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ handle_fork_ਬਿੰਦੂ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ commit *derived, *fork_ਬਿੰਦੂ;
	ਅਚਲ ਅੱਖਰ *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	ਜੇ (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_ਬਿੰਦੂ = get_fork_ਬਿੰਦੂ(argv[0], derived);

	ਜੇ (!fork_ਬਿੰਦੂ)
		ਵਾਪਸ 1;

	ਲਿਖੋ("%s\n", oid_ਮੰਜ਼ਿਲ_hex(&fork_ਬਿੰਦੂ->object.oid));
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_ਮਿਲਾਓ_ਅਧਾਰ(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ commit **rev;
	ਮਾਪ_t rev_nr = 0;
	ਪੂਰਨ show_all = 0;
	ਪੂਰਨ cmdmode = 0;
	ਪੂਰਨ ret;

	ਬਣਤਰ option options[] = {
		OPT_BOOL('a', "all", &show_all, N_("output all common ancestors")),
		OPT_CMDMODE(0, "octopus", &cmdmode,
			    N_("find ancestors for a single n-way merge"), 'o'),
		OPT_CMDMODE(0, "independent", &cmdmode,
			    N_("list revs not reachable from others"), 'r'),
		OPT_CMDMODE(0, "is-ancestor", &cmdmode,
			    N_("is the first one ancestor of the other?"), 'a'),
		OPT_CMDMODE(0, "fork-point", &cmdmode,
			    N_("find where <commit> forked from reflog of <ref>"), 'f'),
		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, ਮਿਲਾਓ_ਅਧਾਰ_usage, 0);

	ਜੇ (cmdmode == 'a') {
		ਜੇ (argc < 2)
			usage_with_options(ਮਿਲਾਓ_ਅਧਾਰ_usage, options);
		ਜੇ (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		ਵਾਪਸ handle_is_ancestor(argc, argv);
	}

	ਜੇ (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	ਜੇ (cmdmode == 'o')
		ਵਾਪਸ handle_octopus(argc, argv, show_all);

	ਜੇ (cmdmode == 'r')
		ਵਾਪਸ handle_independent(argc, argv);

	ਜੇ (cmdmode == 'f') {
		ਜੇ (argc < 1 || 2 < argc)
			usage_with_options(ਮਿਲਾਓ_ਅਧਾਰ_usage, options);
		ਵਾਪਸ handle_fork_ਬਿੰਦੂ(argc, argv);
	}

	ਜੇ (argc < 2)
		usage_with_options(ਮਿਲਾਓ_ਅਧਾਰ_usage, options);

	ALLOC_ARRAY(rev, argc);
	ਜਦੋਂ (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_ਮਿਲਾਓ_ਅਧਾਰ(rev, rev_nr, show_all);
	ਮੁਕਤ(rev);
	ਵਾਪਸ ret;
}
