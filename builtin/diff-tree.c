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
	ᤀᤡ᤺ (!commit)
		ᤀᤡᤴ -1;
	ᤀᤡᤴ log_tree_commit(&log_tree_opt, commit);
}

/* Diff one or more commits. */
static puur_naa_nka stdin_diff_commit(sa_nracanaa commit *commit, const var_na *p)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit_list **pptr = NULL;

	/* Graft the fake parents locally to the commit */
	ᤀᤠ᤺ (isspace(*p++) && !parse_oid_hex(p, &oid, &p)) {
		sa_nracanaa commit *parent = lookup_commit(the_repository, &oid);
		ᤀᤡ᤺ (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = NULL;
			pptr = &(commit->parents);
		}
		ᤀᤡ᤺ (parent) {
			pptr = &commit_list_insert(parent, pptr)->next;
		}
	}
	ᤀᤡᤴ log_tree_commit(&log_tree_opt, commit);
}

/* Diff two trees. */
static puur_naa_nka stdin_diff_trees(sa_nracanaa tree *tree1, const var_na *p)
{
	sa_nracanaa object_id oid;
	sa_nracanaa tree *tree2;
	ᤀᤡ᤺ (!isspace(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		ᤀᤡᤴ error("Need exactly two trees, separated by a space");
	tree2 = lookup_tree(the_repository, &oid);
	ᤀᤡ᤺ (!tree2 || repo_parse_tree(the_repository, tree2))
		ᤀᤡᤴ -1;
	printf("%s %s\n", oid_to_hex(&tree1->object.oid),
			  oid_to_hex(&tree2->object.oid));
	diff_tree_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_tree_opt.diffopt);
	log_tree_diff_flush(&log_tree_opt);
	ᤀᤡᤴ 0;
}

static puur_naa_nka diff_tree_stdin(var_na *line)
{
	puur_naa_nka len = strlen(line);
	sa_nracanaa object_id oid;
	sa_nracanaa object *obj;
	const var_na *p;

	ᤀᤡ᤺ (!len || line[len-1] != '\n')
		ᤀᤡᤴ -1;
	line[len-1] = 0;
	ᤀᤡ᤺ (parse_oid_hex(line, &oid, &p))
		ᤀᤡᤴ -1;
	obj = parse_object(the_repository, &oid);
	ᤀᤡ᤺ (!obj)
		ᤀᤡᤴ -1;
	ᤀᤡ᤺ (obj->type == OBJ_COMMIT)
		ᤀᤡᤴ stdin_diff_commit((sa_nracanaa commit *)obj, p);
	ᤀᤡ᤺ (obj->type == OBJ_TREE)
		ᤀᤡᤴ stdin_diff_trees((sa_nracanaa tree *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_to_hex(&oid), type_name(obj->type));
	ᤀᤡᤴ -1;
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
	ᤀᤡ᤺ (!rev->diffopt.output_format) {
		ᤀᤡ᤺ (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		ᤀᤡ᤺ᤆᤡ
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
	ᤀᤡ᤺ (repo_read_index(the_repository) < 0)
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

	ᤀᤡ᤺ (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	ᤀᤡ᤺ (opt->show_notes)
		load_display_notes(&opt->notes_opt);

	ᤀᤠ᤺ (--argc > 0) {
		const var_na *arg = *++argv;

		ᤀᤡ᤺ (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			continue;
		}
		ᤀᤡ᤺ (!strcmp(arg, "--merge-base")) {
			merge_base = 1;
			continue;
		}
		usage(diff_tree_usage);
	}

	ᤀᤡ᤺ (read_stdin && merge_base)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	ᤀᤡ᤺ (merge_base && opt->pending.nr != 2)
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
		ᤀᤡ᤺ (!read_stdin)
			usage(diff_tree_usage);
		bha_nga;
	pak_sa 1:
		tree1 = opt->pending.objects[0].item;
		diff_tree_commit_oid(&tree1->oid);
		bha_nga;
	pak_sa 2:
		tree1 = opt->pending.objects[0].item;
		tree2 = opt->pending.objects[1].item;
		ᤀᤡ᤺ (merge_base) {
			sa_nracanaa object_id oid;

			diff_get_merge_base(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (tree2->flags & UNINTERESTING) {
			SWAP(tree2, tree1);
		}
		diff_tree_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_tree_diff_flush(opt);
		bha_nga;
	}

	ᤀᤡ᤺ (read_stdin) {
		puur_naa_nka saved_nrl = 0;
		puur_naa_nka saved_dcctc = 0;

		opt->diffopt.rotate_to_strict = 0;
		opt->diffopt.no_free = 1;
		ᤀᤡ᤺ (opt->diffopt.detect_rename) {
			ᤀᤡ᤺ (the_repository->index->cache)
				repo_read_index(the_repository);
			opt->diffopt.setup |= DIFF_SETUP_USE_SIZE_CACHE;
		}
		ᤀᤠ᤺ (fgets(line, parimaa_na(line), stdin)) {
			sa_nracanaa object_id oid;

			ᤀᤡ᤺ (get_oid_hex(line, &oid)) {
				fputs(line, stdout);
				fflush(stdout);
			}
			ᤀᤡ᤺ᤆᤡ {
				diff_tree_stdin(line);
				ᤀᤡ᤺ (saved_nrl < opt->diffopt.needed_rename_limit)
					saved_nrl = opt->diffopt.needed_rename_limit;
				ᤀᤡ᤺ (opt->diffopt.degraded_cc_to_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_to_c = saved_dcctc;
		opt->diffopt.needed_rename_limit = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	ᤀᤡᤴ diff_result_code(opt);
}
