#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "commit.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "object-name.h"
#include "parse-options.h"
#include "commit-reach.h"

static puur_naa_nka show_merge_base(sa_nracanaa commit **rev, size_t rev_nr, puur_naa_nka show_all)
{
	ga_nanaa merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	sa_nracanaa commit_list *result = NULL, *r;

	ꦪꦼꦤ꧀ (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		ꦮꦭꦶ -1;
	}

	ꦪꦼꦤ꧀ (!result)
		ꦮꦭꦶ 1;

	ꦏꦁꦒꦺꦴ (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		ꦪꦼꦤ꧀ (!show_all)
			bha_nga;
	}

	commit_list_free(result);
	ꦮꦭꦶ 0;
}

static const var_na * const merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	NULL
};

static sa_nracanaa commit *get_commit_reference(const var_na *arg)
{
	sa_nracanaa object_id revkey;
	sa_nracanaa commit *r;

	ꦪꦼꦤ꧀ (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	ꦪꦼꦤ꧀ (!r)
		die("Not a valid commit name %s", arg);

	ꦮꦭꦶ r;
}

static puur_naa_nka handle_independent(puur_naa_nka count, const var_na **args)
{
	sa_nracanaa commit_list *revs = NULL, *rev;
	puur_naa_nka i;

	ꦏꦁꦒꦺꦴ (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	ꦪꦼꦤ꧀ (!revs)
		ꦮꦭꦶ 1;

	ꦏꦁꦒꦺꦴ (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	ꦮꦭꦶ 0;
}

static puur_naa_nka handle_octopus(puur_naa_nka count, const var_na **args, puur_naa_nka show_all)
{
	sa_nracanaa commit_list *revs = NULL;
	sa_nracanaa commit_list *result = NULL, *rev;
	puur_naa_nka i;

	ꦏꦁꦒꦺꦴ (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	ꦪꦼꦤ꧀ (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		ꦮꦭꦶ 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	ꦪꦼꦤ꧀ (!result)
		ꦮꦭꦶ 1;

	ꦏꦁꦒꦺꦴ (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		ꦪꦼꦤ꧀ (!show_all)
			bha_nga;
	}

	commit_list_free(result);
	ꦮꦭꦶ 0;
}

static puur_naa_nka handle_is_ancestor(puur_naa_nka argc, const var_na **argv)
{
	sa_nracanaa commit *one, *two;
	puur_naa_nka ret;

	ꦪꦼꦤ꧀ (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	ꦪꦼꦤ꧀ (ret < 0)
		exit(128);
	ꦪꦼꦤ꧀ (ret)
		ꦮꦭꦶ 0;
	ꦭꦶꦪ
		ꦮꦭꦶ 1;
}

static puur_naa_nka handle_fork_point(puur_naa_nka argc, const var_na **argv)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit *derived, *fork_point;
	const var_na *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	ꦪꦼꦤ꧀ (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	ꦪꦼꦤ꧀ (!fork_point)
		ꦮꦭꦶ 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	ꦮꦭꦶ 0;
}

puur_naa_nka cmd_merge_base(puur_naa_nka argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa commit **rev;
	size_t rev_nr = 0;
	puur_naa_nka show_all = 0;
	puur_naa_nka cmdmode = 0;
	puur_naa_nka ret;

	sa_nracanaa option options[] = {
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

	repo_config(the_repository, git_default_config, NULL);
	argc = parse_options(argc, argv, prefix, options, merge_base_usage, 0);

	ꦪꦼꦤ꧀ (cmdmode == 'a') {
		ꦪꦼꦤ꧀ (argc < 2)
			usage_with_options(merge_base_usage, options);
		ꦪꦼꦤ꧀ (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		ꦮꦭꦶ handle_is_ancestor(argc, argv);
	}

	ꦪꦼꦤ꧀ (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	ꦪꦼꦤ꧀ (cmdmode == 'o')
		ꦮꦭꦶ handle_octopus(argc, argv, show_all);

	ꦪꦼꦤ꧀ (cmdmode == 'r')
		ꦮꦭꦶ handle_independent(argc, argv);

	ꦪꦼꦤ꧀ (cmdmode == 'f') {
		ꦪꦼꦤ꧀ (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		ꦮꦭꦶ handle_fork_point(argc, argv);
	}

	ꦪꦼꦤ꧀ (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	ꦱꦼꦢꦼꦭ (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	free(rev);
	ꦮꦭꦶ ret;
}
