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

static 整数 show_merge_base(struct commit **rev, size_t rev_nr, 整数 show_all)
{
	enum merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	struct commit_list *result = NULL, *r;

	如果 (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		返回 -1;
	}

	如果 (!result)
		返回 1;

	为 (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		如果 (!show_all)
			break;
	}

	commit_list_free(result);
	返回 0;
}

static const char * const merge_base_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	NULL
};

static struct commit *get_commit_reference(const char *arg)
{
	struct object_id revkey;
	struct commit *r;

	如果 (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	如果 (!r)
		die("Not a valid commit name %s", arg);

	返回 r;
}

static 整数 handle_independent(整数 count, const char **args)
{
	struct commit_list *revs = NULL, *rev;
	整数 i;

	为 (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	如果 (!revs)
		返回 1;

	为 (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	返回 0;
}

static 整数 handle_octopus(整数 count, const char **args, 整数 show_all)
{
	struct commit_list *revs = NULL;
	struct commit_list *result = NULL, *rev;
	整数 i;

	为 (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	如果 (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		返回 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	如果 (!result)
		返回 1;

	为 (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		如果 (!show_all)
			break;
	}

	commit_list_free(result);
	返回 0;
}

static 整数 handle_is_ancestor(整数 argc, const char **argv)
{
	struct commit *one, *two;
	整数 ret;

	如果 (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	如果 (ret < 0)
		exit(128);
	如果 (ret)
		返回 0;
	否则
		返回 1;
}

static 整数 handle_fork_point(整数 argc, const char **argv)
{
	struct object_id oid;
	struct commit *derived, *fork_point;
	const char *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	如果 (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	如果 (!fork_point)
		返回 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	返回 0;
}

整数 cmd_merge_base(整数 argc,
		   const char **argv,
		   const char *prefix,
		   struct repository *repo UNUSED)
{
	struct commit **rev;
	size_t rev_nr = 0;
	整数 show_all = 0;
	整数 cmdmode = 0;
	整数 ret;

	struct option options[] = {
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

	如果 (cmdmode == 'a') {
		如果 (argc < 2)
			usage_with_options(merge_base_usage, options);
		如果 (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		返回 handle_is_ancestor(argc, argv);
	}

	如果 (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	如果 (cmdmode == 'o')
		返回 handle_octopus(argc, argv, show_all);

	如果 (cmdmode == 'r')
		返回 handle_independent(argc, argv);

	如果 (cmdmode == 'f') {
		如果 (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		返回 handle_fork_point(argc, argv);
	}

	如果 (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	当 (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	free(rev);
	返回 ret;
}
