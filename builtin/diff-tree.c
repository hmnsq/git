#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "config.h"
#include "diff.h"
#include "commit.h"
#include "gettext.h"
#include "hex.h"
#include "log-tree.h"
#include "read-cache-ll.h"
#include "revision.h"
#include "tmp-objdir.h"
#include "tree.h"

static struct rev_info log_tree_opt;

static цяло diff_tree_commit_oid(const struct object_id *oid)
{
	struct commit *commit = lookup_commit_reference(the_repository, oid);
	ако (!commit)
		върни -1;
	върни log_tree_commit(&log_tree_opt, commit);
}

/* Diff one or more commits. */
static цяло stdin_diff_commit(struct commit *commit, const char *p)
{
	struct object_id oid;
	struct commit_list **pptr = NULL;

	/* Graft the fake parents locally to the commit */
	докато (isspace(*p++) && !parse_oid_hex(p, &oid, &p)) {
		struct commit *parent = lookup_commit(the_repository, &oid);
		ако (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = NULL;
			pptr = &(commit->parents);
		}
		ако (parent) {
			pptr = &commit_list_insert(parent, pptr)->next;
		}
	}
	върни log_tree_commit(&log_tree_opt, commit);
}

/* Diff two trees. */
static цяло stdin_diff_trees(struct tree *tree1, const char *p)
{
	struct object_id oid;
	struct tree *tree2;
	ако (!isspace(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		върни error("Need exactly two trees, separated by a space");
	tree2 = lookup_tree(the_repository, &oid);
	ако (!tree2 || repo_parse_tree(the_repository, tree2))
		върни -1;
	printf("%s %s\n", oid_to_hex(&tree1->object.oid),
			  oid_to_hex(&tree2->object.oid));
	diff_tree_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_tree_opt.diffopt);
	log_tree_diff_flush(&log_tree_opt);
	върни 0;
}

static цяло diff_tree_stdin(char *line)
{
	цяло len = strlen(line);
	struct object_id oid;
	struct object *obj;
	const char *p;

	ако (!len || line[len-1] != '\n')
		върни -1;
	line[len-1] = 0;
	ако (parse_oid_hex(line, &oid, &p))
		върни -1;
	obj = parse_object(the_repository, &oid);
	ако (!obj)
		върни -1;
	ако (obj->type == OBJ_COMMIT)
		върни stdin_diff_commit((struct commit *)obj, p);
	ако (obj->type == OBJ_TREE)
		върни stdin_diff_trees((struct tree *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_to_hex(&oid), type_name(obj->type));
	върни -1;
}

static const char diff_tree_usage[] =
"git diff-tree [--stdin] [-m] [-s] [-v] [--no-commit-id] [--pretty]\n"
"              [-t] [-r] [-c | --cc] [--combined-all-paths] [--root] [--merge-base]\n"
"              [<common-diff-options>] <tree-ish> [<tree-ish>] [<path>...]\n"
"\n"
"  -r            diff recursively\n"
"  -c            show combined diff for merge commits\n"
"  --cc          show combined diff for merge commits removing uninteresting hunks\n"
"  --combined-all-paths\n"
"                show name of file in all parents for combined diffs\n"
"  --root        include the initial commit as diff against /dev/null\n"
COMMON_DIFF_OPTIONS_HELP;

static void diff_tree_tweak_rev(struct rev_info *rev)
{
	ако (!rev->diffopt.output_format) {
		ако (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		иначе
			rev->diffopt.output_format = DIFF_FORMAT_RAW;
	}
}

цяло cmd_diff_tree(цяло argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	char line[1000];
	struct object *tree1, *tree2;
	static struct rev_info *opt = &log_tree_opt;
	struct setup_revision_opt s_r_opt;
	struct userformat_want w;
	цяло read_stdin = 0;
	цяло merge_base = 0;

	show_usage_if_asked(argc, argv, diff_tree_usage);

	repo_config(the_repository, git_diff_basic_config, NULL); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_init_revisions(the_repository, opt, prefix);
	ако (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));
	opt->abbrev = 0;
	opt->diff = 1;
	opt->disable_stdin = 1;
	memset(&s_r_opt, 0, sizeof(s_r_opt));
	s_r_opt.tweak = diff_tree_tweak_rev;

	prefix = precompose_argv_prefix(argc, argv, prefix);
	argc = setup_revisions(argc, argv, opt, &s_r_opt);

	memset(&w, 0, sizeof(w));
	userformat_find_requirements(NULL, &w);

	ако (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	ако (opt->show_notes)
		load_display_notes(&opt->notes_opt);

	докато (--argc > 0) {
		const char *arg = *++argv;

		ако (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			continue;
		}
		ако (!strcmp(arg, "--merge-base")) {
			merge_base = 1;
			continue;
		}
		usage(diff_tree_usage);
	}

	ако (read_stdin && merge_base)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	ако (merge_base && opt->pending.nr != 2)
		die(_("--merge-base only works with two commits"));

	opt->diffopt.rotate_to_strict = 1;

	/*
	 * NOTE!  We expect "a..b" to expand to "^a b" but it is
	 * perfectly valid for revision range parser to yield "b ^a",
	 * which means the same thing. If we get the latter, i.e. the
	 * second one is marked UNINTERESTING, we recover the original
	 * order the user gave, i.e. "a..b", by swapping the trees.
	 */
	switch (opt->pending.nr) {
	case 0:
		ако (!read_stdin)
			usage(diff_tree_usage);
		break;
	case 1:
		tree1 = opt->pending.objects[0].item;
		diff_tree_commit_oid(&tree1->oid);
		break;
	case 2:
		tree1 = opt->pending.objects[0].item;
		tree2 = opt->pending.objects[1].item;
		ако (merge_base) {
			struct object_id oid;

			diff_get_merge_base(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} иначе ако (tree2->flags & UNINTERESTING) {
			SWAP(tree2, tree1);
		}
		diff_tree_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_tree_diff_flush(opt);
		break;
	}

	ако (read_stdin) {
		цяло saved_nrl = 0;
		цяло saved_dcctc = 0;

		opt->diffopt.rotate_to_strict = 0;
		opt->diffopt.no_free = 1;
		ако (opt->diffopt.detect_rename) {
			ако (the_repository->index->cache)
				repo_read_index(the_repository);
			opt->diffopt.setup |= DIFF_SETUP_USE_SIZE_CACHE;
		}
		докато (fgets(line, sizeof(line), stdin)) {
			struct object_id oid;

			ако (get_oid_hex(line, &oid)) {
				fputs(line, stdout);
				fflush(stdout);
			}
			иначе {
				diff_tree_stdin(line);
				ако (saved_nrl < opt->diffopt.needed_rename_limit)
					saved_nrl = opt->diffopt.needed_rename_limit;
				ако (opt->diffopt.degraded_cc_to_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_to_c = saved_dcctc;
		opt->diffopt.needed_rename_limit = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	върни diff_result_code(opt);
}
