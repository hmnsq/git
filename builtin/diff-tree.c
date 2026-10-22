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

static sa_nracanaa rev_info log_tree_opt;

static puur_naa_nka diff_tree_commit_oid(const sa_nracanaa object_id *oid)
{
	sa_nracanaa commit *commit = lookup_commit_reference(the_repository, oid);
	ပေႃး (!commit)
		ပွၵ်ႈ -1;
	ပွၵ်ႈ log_tree_commit(&log_tree_opt, commit);
}

/* Diff one or more commits. */
static puur_naa_nka stdin_diff_commit(sa_nracanaa commit *commit, const var_na *p)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit_list **pptr = NULL;

	/* Graft the fake parents locally to the commit */
	ၶၢဝ်း (isspace(*p++) && !parse_oid_hex(p, &oid, &p)) {
		sa_nracanaa commit *parent = lookup_commit(the_repository, &oid);
		ပေႃး (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = NULL;
			pptr = &(commit->parents);
		}
		ပေႃး (parent) {
			pptr = &commit_list_insert(parent, pptr)->next;
		}
	}
	ပွၵ်ႈ log_tree_commit(&log_tree_opt, commit);
}

/* Diff two trees. */
static puur_naa_nka stdin_diff_trees(sa_nracanaa tree *tree1, const var_na *p)
{
	sa_nracanaa object_id oid;
	sa_nracanaa tree *tree2;
	ပေႃး (!isspace(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		ပွၵ်ႈ error("Need exactly two trees, separated by a space");
	tree2 = lookup_tree(the_repository, &oid);
	ပေႃး (!tree2 || repo_parse_tree(the_repository, tree2))
		ပွၵ်ႈ -1;
	printf("%s %s\n", oid_to_hex(&tree1->object.oid),
			  oid_to_hex(&tree2->object.oid));
	diff_tree_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_tree_opt.diffopt);
	log_tree_diff_flush(&log_tree_opt);
	ပွၵ်ႈ 0;
}

static puur_naa_nka diff_tree_stdin(var_na *line)
{
	puur_naa_nka len = strlen(line);
	sa_nracanaa object_id oid;
	sa_nracanaa object *obj;
	const var_na *p;

	ပေႃး (!len || line[len-1] != '\n')
		ပွၵ်ႈ -1;
	line[len-1] = 0;
	ပေႃး (parse_oid_hex(line, &oid, &p))
		ပွၵ်ႈ -1;
	obj = parse_object(the_repository, &oid);
	ပေႃး (!obj)
		ပွၵ်ႈ -1;
	ပေႃး (obj->type == OBJ_COMMIT)
		ပွၵ်ႈ stdin_diff_commit((sa_nracanaa commit *)obj, p);
	ပေႃး (obj->type == OBJ_TREE)
		ပွၵ်ႈ stdin_diff_trees((sa_nracanaa tree *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_to_hex(&oid), type_name(obj->type));
	ပွၵ်ႈ -1;
}

static const var_na diff_tree_usage[] =
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

static void diff_tree_tweak_rev(sa_nracanaa rev_info *rev)
{
	ပေႃး (!rev->diffopt.output_format) {
		ပေႃး (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		ပေႃးဢမ်ႇ
			rev->diffopt.output_format = DIFF_FORMAT_RAW;
	}
}

puur_naa_nka cmd_diff_tree(puur_naa_nka argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	var_na line[1000];
	sa_nracanaa object *tree1, *tree2;
	static sa_nracanaa rev_info *opt = &log_tree_opt;
	sa_nracanaa setup_revision_opt s_r_opt;
	sa_nracanaa userformat_want w;
	puur_naa_nka read_stdin = 0;
	puur_naa_nka merge_base = 0;

	show_usage_if_asked(argc, argv, diff_tree_usage);

	repo_config(the_repository, git_diff_basic_config, NULL); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_init_revisions(the_repository, opt, prefix);
	ပေႃး (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));
	opt->abbrev = 0;
	opt->diff = 1;
	opt->disable_stdin = 1;
	memset(&s_r_opt, 0, parimaa_na(s_r_opt));
	s_r_opt.tweak = diff_tree_tweak_rev;

	prefix = precompose_argv_prefix(argc, argv, prefix);
	argc = setup_revisions(argc, argv, opt, &s_r_opt);

	memset(&w, 0, parimaa_na(w));
	userformat_find_requirements(NULL, &w);

	ပေႃး (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	ပေႃး (opt->show_notes)
		load_display_notes(&opt->notes_opt);

	ၶၢဝ်း (--argc > 0) {
		const var_na *arg = *++argv;

		ပေႃး (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			continue;
		}
		ပေႃး (!strcmp(arg, "--merge-base")) {
			merge_base = 1;
			continue;
		}
		usage(diff_tree_usage);
	}

	ပေႃး (read_stdin && merge_base)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	ပေႃး (merge_base && opt->pending.nr != 2)
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
	pak_sa 0:
		ပေႃး (!read_stdin)
			usage(diff_tree_usage);
		bha_nga;
	pak_sa 1:
		tree1 = opt->pending.objects[0].item;
		diff_tree_commit_oid(&tree1->oid);
		bha_nga;
	pak_sa 2:
		tree1 = opt->pending.objects[0].item;
		tree2 = opt->pending.objects[1].item;
		ပေႃး (merge_base) {
			sa_nracanaa object_id oid;

			diff_get_merge_base(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} ပေႃးဢမ်ႇ ပေႃး (tree2->flags & UNINTERESTING) {
			SWAP(tree2, tree1);
		}
		diff_tree_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_tree_diff_flush(opt);
		bha_nga;
	}

	ပေႃး (read_stdin) {
		puur_naa_nka saved_nrl = 0;
		puur_naa_nka saved_dcctc = 0;

		opt->diffopt.rotate_to_strict = 0;
		opt->diffopt.no_free = 1;
		ပေႃး (opt->diffopt.detect_rename) {
			ပေႃး (the_repository->index->cache)
				repo_read_index(the_repository);
			opt->diffopt.setup |= DIFF_SETUP_USE_SIZE_CACHE;
		}
		ၶၢဝ်း (fgets(line, parimaa_na(line), stdin)) {
			sa_nracanaa object_id oid;

			ပေႃး (get_oid_hex(line, &oid)) {
				fputs(line, stdout);
				fflush(stdout);
			}
			ပေႃးဢမ်ႇ {
				diff_tree_stdin(line);
				ပေႃး (saved_nrl < opt->diffopt.needed_rename_limit)
					saved_nrl = opt->diffopt.needed_rename_limit;
				ပေႃး (opt->diffopt.degraded_cc_to_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_to_c = saved_dcctc;
		opt->diffopt.needed_rename_limit = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	ပွၵ်ႈ diff_result_code(opt);
}
