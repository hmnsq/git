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

static ціле show_merge_base(struct commit **rev, size_t rev_nr, ціле show_all)
{
	enum merge_base_flags flags = show_all ? MERGE_BASE_FIND_ALL : 0;
	struct commit_list *result = NULL, *r;

	якщо (repo_get_merge_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &result) < 0) {
		commit_list_free(result);
		повернути -1;
	}

	якщо (!result)
		повернути 1;

	для (r = result; r; r = r->next) {
		printf("%s\n", oid_to_hex(&r->item->object.oid));
		якщо (!show_all)
			break;
	}

	commit_list_free(result);
	повернути 0;
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

	якщо (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	якщо (!r)
		die("Not a valid commit name %s", arg);

	повернути r;
}

static ціле handle_independent(ціле count, const char **args)
{
	struct commit_list *revs = NULL, *rev;
	ціле i;

	для (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	reduce_heads_replace(&revs);

	якщо (!revs)
		повернути 1;

	для (rev = revs; rev; rev = rev->next)
		printf("%s\n", oid_to_hex(&rev->item->object.oid));

	commit_list_free(revs);
	повернути 0;
}

static ціле handle_octopus(ціле count, const char **args, ціле show_all)
{
	struct commit_list *revs = NULL;
	struct commit_list *result = NULL, *rev;
	ціле i;

	для (i = count - 1; i >= 0; i--)
		commit_list_insert(get_commit_reference(args[i]), &revs);

	якщо (get_octopus_merge_bases(revs, &result) < 0) {
		commit_list_free(revs);
		commit_list_free(result);
		повернути 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&result);

	якщо (!result)
		повернути 1;

	для (rev = result; rev; rev = rev->next) {
		printf("%s\n", oid_to_hex(&rev->item->object.oid));
		якщо (!show_all)
			break;
	}

	commit_list_free(result);
	повернути 0;
}

static ціле handle_is_ancestor(ціле argc, const char **argv)
{
	struct commit *one, *two;
	ціле ret;

	якщо (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_in_merge_bases(the_repository, one, two);
	якщо (ret < 0)
		exit(128);
	якщо (ret)
		повернути 0;
	інакше
		повернути 1;
}

static ціле handle_fork_point(ціле argc, const char **argv)
{
	struct object_id oid;
	struct commit *derived, *fork_point;
	const char *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	якщо (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_point = get_fork_point(argv[0], derived);

	якщо (!fork_point)
		повернути 1;

	printf("%s\n", oid_to_hex(&fork_point->object.oid));
	повернути 0;
}

ціле cmd_merge_base(ціле argc,
		   const char **argv,
		   const char *prefix,
		   struct repository *repo UNUSED)
{
	struct commit **rev;
	size_t rev_nr = 0;
	ціле show_all = 0;
	ціле cmdmode = 0;
	ціле ret;

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

	якщо (cmdmode == 'a') {
		якщо (argc < 2)
			usage_with_options(merge_base_usage, options);
		якщо (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		повернути handle_is_ancestor(argc, argv);
	}

	якщо (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	якщо (cmdmode == 'o')
		повернути handle_octopus(argc, argv, show_all);

	якщо (cmdmode == 'r')
		повернути handle_independent(argc, argv);

	якщо (cmdmode == 'f') {
		якщо (argc < 1 || 2 < argc)
			usage_with_options(merge_base_usage, options);
		повернути handle_fork_point(argc, argv);
	}

	якщо (argc < 2)
		usage_with_options(merge_base_usage, options);

	ALLOC_ARRAY(rev, argc);
	поки (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_merge_base(rev, rev_nr, show_all);
	free(rev);
	повернути ret;
}
