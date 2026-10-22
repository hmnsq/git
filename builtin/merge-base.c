#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ವಿಲೀನ_ತಳ(ರಚನೆ commit **rev, ಗಾತ್ರ_t rev_nr, ಪೂರ್ಣಾಂಕ show_all)
{
	ಎಣಿಕೆ ವಿಲೀನ_ತಳ_flags flags = show_all ? ವಿಲೀನ_ತಳ_ಕಂಡುಹಿಡಿ_ALL : 0;
	ರಚನೆ commit_list *ಫಲಿತಾಂಶ = ಖಾಲಿ, *r;

	ಆದರೆ (repo_get_ವಿಲೀನ_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &ಫಲಿತಾಂಶ) < 0) {
		commit_list_free(ಫಲಿತಾಂಶ);
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (!ಫಲಿತಾಂಶ)
		ಹಿಂತಿರುಗಿ 1;

	ಗಾಗಿ (r = ಫಲಿತಾಂಶ; r; r = r->ಮುಂದಿನ) {
		ಬರೆ("%s\n", oid_ಗಮ್ಯ_hex(&r->ಅಂಶ->object.oid));
		ಆದರೆ (!show_all)
			ನಿಲ್ಲಿಸು;
	}

	commit_list_free(ಫಲಿತಾಂಶ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ವಿಲೀನ_ತಳ_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ರಚನೆ commit *get_commit_reference(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ರಚನೆ object_id revkey;
	ರಚನೆ commit *r;

	ಆದರೆ (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	ಆದರೆ (!r)
		die("Not a valid commit name %s", arg);

	ಹಿಂತಿರುಗಿ r;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ handle_independent(ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ವಾದಗಳು)
{
	ರಚನೆ commit_list *revs = ಖಾಲಿ, *rev;
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = ಎಣಿಕೆ_2 - 1; i >= 0; i--)
		commit_list_ಸೇರಿಸು(get_commit_reference(ವಾದಗಳು[i]), &revs);

	reduce_heads_replace(&revs);

	ಆದರೆ (!revs)
		ಹಿಂತಿರುಗಿ 1;

	ಗಾಗಿ (rev = revs; rev; rev = rev->ಮುಂದಿನ)
		ಬರೆ("%s\n", oid_ಗಮ್ಯ_hex(&rev->ಅಂಶ->object.oid));

	commit_list_free(revs);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ handle_octopus(ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ವಾದಗಳು, ಪೂರ್ಣಾಂಕ show_all)
{
	ರಚನೆ commit_list *revs = ಖಾಲಿ;
	ರಚನೆ commit_list *ಫಲಿತಾಂಶ = ಖಾಲಿ, *rev;
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = ಎಣಿಕೆ_2 - 1; i >= 0; i--)
		commit_list_ಸೇರಿಸು(get_commit_reference(ವಾದಗಳು[i]), &revs);

	ಆದರೆ (get_octopus_ವಿಲೀನ_bases(revs, &ಫಲಿತಾಂಶ) < 0) {
		commit_list_free(revs);
		commit_list_free(ಫಲಿತಾಂಶ);
		ಹಿಂತಿರುಗಿ 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&ಫಲಿತಾಂಶ);

	ಆದರೆ (!ಫಲಿತಾಂಶ)
		ಹಿಂತಿರುಗಿ 1;

	ಗಾಗಿ (rev = ಫಲಿತಾಂಶ; rev; rev = rev->ಮುಂದಿನ) {
		ಬರೆ("%s\n", oid_ಗಮ್ಯ_hex(&rev->ಅಂಶ->object.oid));
		ಆದರೆ (!show_all)
			ನಿಲ್ಲಿಸು;
	}

	commit_list_free(ಫಲಿತಾಂಶ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ handle_is_ancestor(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ರಚನೆ commit *one, *two;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_ಒಳ_ವಿಲೀನ_bases(the_repository, one, two);
	ಆದರೆ (ret < 0)
		ನಿರ್ಗಮಿಸು(128);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ 0;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ handle_fork_ಬಿಂದು(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ರಚನೆ object_id oid;
	ರಚನೆ commit *derived, *fork_ಬಿಂದು;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	ಆದರೆ (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_ಬಿಂದು = get_fork_ಬಿಂದು(argv[0], derived);

	ಆದರೆ (!fork_ಬಿಂದು)
		ಹಿಂತಿರುಗಿ 1;

	ಬರೆ("%s\n", oid_ಗಮ್ಯ_hex(&fork_ಬಿಂದು->object.oid));
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_ವಿಲೀನ_ತಳ(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ commit **rev;
	ಗಾತ್ರ_t rev_nr = 0;
	ಪೂರ್ಣಾಂಕ show_all = 0;
	ಪೂರ್ಣಾಂಕ cmdmode = 0;
	ಪೂರ್ಣಾಂಕ ret;

	ರಚನೆ option options[] = {
		OPT_BOOL('a', "all", &show_all, N_("output all common ancestors")),
		OPT_CMDMODE(0, "octopus", &cmdmode,
			    N_("find ancestors for a single n-way merge"), 'o'),
		OPT_CMDMODE(0, "independent", &cmdmode,
			    N_("list revs not reachable from others"), 'r'),
		OPT_CMDMODE(0, "is-ancestor", &cmdmode,
			    N_("is the first one ancestor of the other?"), 'a'),
		OPT_CMDMODE(0, "fork-point", &cmdmode,
			    N_("find where <commit> forked from reflog of <ref>"), 'f'),
		OPT_ಕೊನೆ()
	};

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, ವಿಲೀನ_ತಳ_usage, 0);

	ಆದರೆ (cmdmode == 'a') {
		ಆದರೆ (argc < 2)
			usage_with_options(ವಿಲೀನ_ತಳ_usage, options);
		ಆದರೆ (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		ಹಿಂತಿರುಗಿ handle_is_ancestor(argc, argv);
	}

	ಆದರೆ (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	ಆದರೆ (cmdmode == 'o')
		ಹಿಂತಿರುಗಿ handle_octopus(argc, argv, show_all);

	ಆದರೆ (cmdmode == 'r')
		ಹಿಂತಿರುಗಿ handle_independent(argc, argv);

	ಆದರೆ (cmdmode == 'f') {
		ಆದರೆ (argc < 1 || 2 < argc)
			usage_with_options(ವಿಲೀನ_ತಳ_usage, options);
		ಹಿಂತಿರುಗಿ handle_fork_ಬಿಂದು(argc, argv);
	}

	ಆದರೆ (argc < 2)
		usage_with_options(ವಿಲೀನ_ತಳ_usage, options);

	ALLOC_ARRAY(rev, argc);
	ಆಗ (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_ವಿಲೀನ_ತಳ(rev, rev_nr, show_all);
	ಬಿಡುಗಡೆ(rev);
	ಹಿಂತಿರುಗಿ ret;
}
