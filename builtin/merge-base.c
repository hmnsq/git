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

static ཧྲིལ show_merge_base(sa_nracanaa commit **rev, size_t rev_nr, ཧྲིལ show_all)
{
	ga_nanaa merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	sa_nracanaa commit_list *result = NULL, *r;

	གལ་སྲིད (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		ལོག -1;
	}

	གལ་སྲིད (!result)
		ལོག 1;

	དོན་ལུ (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		གལ་སྲིད (!show_all)
			bha_nga;
	}

	commit_list_free(result);
	ལོག 0;
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

	གལ་སྲིད (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	གལ་སྲིད (!r)
		die("Not a valid commit name %s", arg);

	ལོག r;
}

static ཧྲིལ handle_independent(ཧྲིལ count, const var_na **args)
{
	sa_nracanaa commit_list *revs = NULL, *rev;
	ཧྲིལ i;

	དོན་ལུ (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	གལ་སྲིད (!revs)
		ལོག 1;

	དོན་ལུ (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	ལོག 0;
}

static ཧྲིལ handle_octopus(ཧྲིལ count, const var_na **args, ཧྲིལ show_all)
{
	sa_nracanaa commit_list *revs = NULL;
	sa_nracanaa commit_list *result = NULL, *rev;
	ཧྲིལ i;

	དོན་ལུ (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	གལ་སྲིད (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		ལོག 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	གལ་སྲིད (!result)
		ལོག 1;

	དོན་ལུ (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		གལ་སྲིད (!show_all)
			bha_nga;
	}

	commit_list_free(result);
	ལོག 0;
}

static ཧྲིལ handle_is_ancestor(ཧྲིལ argc, const var_na **argv)
{
	sa_nracanaa commit *one, *two;
	ཧྲིལ ret;

	གལ་སྲིད (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	གལ་སྲིད (ret < 0)
		exit(128);
	གལ་སྲིད (ret)
		ལོག 0;
	དེ་མིན
		ལོག 1;
}

static ཧྲིལ handle_fork_point(ཧྲིལ argc, const var_na **argv)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit *derived, *fork_point;
	const var_na *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	གལ་སྲིད (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	གལ་སྲིད (!fork_point)
		ལོག 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	ལོག 0;
}

ཧྲིལ cmd_merge_base(ཧྲིལ argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa commit **rev;
	size_t rev_nr = 0;
	ཧྲིལ show_all = 0;
	ཧྲིལ cmdmode = 0;
	ཧྲིལ ret;

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

	གལ་སྲིད (cmdmode == 'a') {
		གལ་སྲིད (argc < 2)
			usage_with_options(merge_base_usage, options);
		གལ་སྲིད (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		ལོག handle_is_ancestor(argc, argv);
	}

	གལ་སྲིད (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	གལ་སྲིད (cmdmode == 'o')
		ལོག handle_octopus(argc, argv, show_all);

	གལ་སྲིད (cmdmode == 'r')
		ལོག handle_independent(argc, argv);

	གལ་སྲིད (cmdmode == 'f') {
		གལ་སྲིད (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		ལོག handle_fork_point(argc, argv);
	}

	གལ་སྲིད (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	སྐབས (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	free(rev);
	ལོག ret;
}
