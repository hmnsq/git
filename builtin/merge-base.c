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

static ចំនួនគត់ show_merge_base(sa_nracanaa commit **rev, size_t rev_nr, ចំនួនគត់ show_all)
{
	ga_nanaa merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	sa_nracanaa commit_list *result = NULL, *r;

	ប្រសិនបើ (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		ត្រឡប់ -1;
	}

	ប្រសិនបើ (!result)
		ត្រឡប់ 1;

	សម្រាប់ (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		ប្រសិនបើ (!show_all)
			bha_nga;
	}

	commit_list_free(result);
	ត្រឡប់ 0;
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

	ប្រសិនបើ (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	ប្រសិនបើ (!r)
		die("Not a valid commit name %s", arg);

	ត្រឡប់ r;
}

static ចំនួនគត់ handle_independent(ចំនួនគត់ count, const var_na **args)
{
	sa_nracanaa commit_list *revs = NULL, *rev;
	ចំនួនគត់ i;

	សម្រាប់ (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	ប្រសិនបើ (!revs)
		ត្រឡប់ 1;

	សម្រាប់ (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	ត្រឡប់ 0;
}

static ចំនួនគត់ handle_octopus(ចំនួនគត់ count, const var_na **args, ចំនួនគត់ show_all)
{
	sa_nracanaa commit_list *revs = NULL;
	sa_nracanaa commit_list *result = NULL, *rev;
	ចំនួនគត់ i;

	សម្រាប់ (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	ប្រសិនបើ (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		ត្រឡប់ 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	ប្រសិនបើ (!result)
		ត្រឡប់ 1;

	សម្រាប់ (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		ប្រសិនបើ (!show_all)
			bha_nga;
	}

	commit_list_free(result);
	ត្រឡប់ 0;
}

static ចំនួនគត់ handle_is_ancestor(ចំនួនគត់ argc, const var_na **argv)
{
	sa_nracanaa commit *one, *two;
	ចំនួនគត់ ret;

	ប្រសិនបើ (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	ប្រសិនបើ (ret < 0)
		exit(128);
	ប្រសិនបើ (ret)
		ត្រឡប់ 0;
	បើមិនដូច្នេះ
		ត្រឡប់ 1;
}

static ចំនួនគត់ handle_fork_point(ចំនួនគត់ argc, const var_na **argv)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit *derived, *fork_point;
	const var_na *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	ប្រសិនបើ (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	ប្រសិនបើ (!fork_point)
		ត្រឡប់ 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	ត្រឡប់ 0;
}

ចំនួនគត់ cmd_merge_base(ចំនួនគត់ argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa commit **rev;
	size_t rev_nr = 0;
	ចំនួនគត់ show_all = 0;
	ចំនួនគត់ cmdmode = 0;
	ចំនួនគត់ ret;

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

	ប្រសិនបើ (cmdmode == 'a') {
		ប្រសិនបើ (argc < 2)
			usage_with_options(merge_base_usage, options);
		ប្រសិនបើ (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		ត្រឡប់ handle_is_ancestor(argc, argv);
	}

	ប្រសិនបើ (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	ប្រសិនបើ (cmdmode == 'o')
		ត្រឡប់ handle_octopus(argc, argv, show_all);

	ប្រសិនបើ (cmdmode == 'r')
		ត្រឡប់ handle_independent(argc, argv);

	ប្រសិនបើ (cmdmode == 'f') {
		ប្រសិនបើ (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		ត្រឡប់ handle_fork_point(argc, argv);
	}

	ប្រសិនបើ (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	ខណៈពេល (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	free(rev);
	ត្រឡប់ ret;
}
