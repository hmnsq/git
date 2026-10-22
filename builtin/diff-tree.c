#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "config.h"
#شامل "diff.h"
#شامل "commit.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "log-tree.h"
#شامل "read-cache-ll.h"
#شامل "revision.h"
#شامل "tmp-objdir.h"
#شامل "tree.h"

ایستا ساختار rev_info log_درخت_opt;

ایستا صحیح diff_درخت_commit_oid(ثابت ساختار object_id *oid)
{
	ساختار commit *commit = lookup_commit_reference(the_repository, oid);
	اگر (!commit)
		بازگشت -1;
	بازگشت log_درخت_commit(&log_درخت_opt, commit);
}

/* Diff one or more commits. */
ایستا صحیح stdin_diff_commit(ساختار commit *commit, ثابت نویسه *p)
{
	ساختار object_id oid;
	ساختار commit_list **pptr = پوچ;

	/* Graft the fake parents locally to the commit */
	وقتی (isspace(*p++) && !parse_oid_hex(p, &oid, &p)) {
		ساختار commit *parent = lookup_commit(the_repository, &oid);
		اگر (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = پوچ;
			pptr = &(commit->parents);
		}
		اگر (parent) {
			pptr = &commit_list_درج(parent, pptr)->next;
		}
	}
	بازگشت log_درخت_commit(&log_درخت_opt, commit);
}

/* Diff two trees. */
ایستا صحیح stdin_diff_trees(ساختار درخت *tree1, ثابت نویسه *p)
{
	ساختار object_id oid;
	ساختار درخت *tree2;
	اگر (!isspace(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		بازگشت error("Need exactly two trees, separated by a space");
	tree2 = lookup_درخت(the_repository, &oid);
	اگر (!tree2 || repo_parse_درخت(the_repository, tree2))
		بازگشت -1;
	printf("%s %s\n", oid_to_hex(&tree1->object.oid),
			  oid_to_hex(&tree2->object.oid));
	diff_درخت_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_درخت_opt.diffopt);
	log_درخت_diff_flush(&log_درخت_opt);
	بازگشت 0;
}

ایستا صحیح diff_درخت_stdin(نویسه *سطر)
{
	صحیح len = strlen(سطر);
	ساختار object_id oid;
	ساختار object *obj;
	ثابت نویسه *p;

	اگر (!len || سطر[len-1] != '\n')
		بازگشت -1;
	سطر[len-1] = 0;
	اگر (parse_oid_hex(سطر, &oid, &p))
		بازگشت -1;
	obj = parse_object(the_repository, &oid);
	اگر (!obj)
		بازگشت -1;
	اگر (obj->type == OBJ_COMMIT)
		بازگشت stdin_diff_commit((ساختار commit *)obj, p);
	اگر (obj->type == OBJ_درخت)
		بازگشت stdin_diff_trees((ساختار درخت *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_to_hex(&oid), type_name(obj->type));
	بازگشت -1;
}

ایستا ثابت نویسه diff_درخت_usage[] =
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

ایستا تهی diff_درخت_tweak_rev(ساختار rev_info *rev)
{
	اگر (!rev->diffopt.output_format) {
		اگر (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		وگرنه
			rev->diffopt.output_format = DIFF_FORMAT_RAW;
	}
}

صحیح cmd_diff_درخت(صحیح argc,
		  ثابت نویسه **argv,
		  ثابت نویسه *prefix,
		  ساختار repository *repo UNUSED)
{
	نویسه سطر[1000];
	ساختار object *tree1, *tree2;
	ایستا ساختار rev_info *opt = &log_درخت_opt;
	ساختار setup_revision_opt s_r_opt;
	ساختار userformat_want w;
	صحیح read_stdin = 0;
	صحیح merge_base = 0;

	show_usage_if_asked(argc, argv, diff_درخت_usage);

	repo_config(the_repository, git_diff_basic_config, پوچ); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_init_revisions(the_repository, opt, prefix);
	اگر (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));
	opt->abbrev = 0;
	opt->diff = 1;
	opt->disable_stdin = 1;
	memset(&s_r_opt, 0, اندازه(s_r_opt));
	s_r_opt.tweak = diff_درخت_tweak_rev;

	prefix = precompose_argv_prefix(argc, argv, prefix);
	argc = setup_revisions(argc, argv, opt, &s_r_opt);

	memset(&w, 0, اندازه(w));
	userformat_find_requirements(پوچ, &w);

	اگر (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	اگر (opt->show_notes)
		load_display_notes(&opt->notes_opt);

	وقتی (--argc > 0) {
		ثابت نویسه *arg = *++argv;

		اگر (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			ادامه;
		}
		اگر (!strcmp(arg, "--merge-base")) {
			merge_base = 1;
			ادامه;
		}
		usage(diff_درخت_usage);
	}

	اگر (read_stdin && merge_base)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	اگر (merge_base && opt->pending.nr != 2)
		die(_("--merge-base only works with two commits"));

	opt->diffopt.rotate_to_strict = 1;

	/*
	 * NOTE!  We expect "a..b" to expand to "^a b" but it is
	 * perfectly valid for revision range parser to yield "b ^a",
	 * which means the same thing. If we get the latter, i.e. the
	 * second one is marked UNINTERESTING, we recover the original
	 * order the user gave, i.e. "a..b", by swapping the trees.
	 */
	انتخاب (opt->pending.nr) {
	حالت 0:
		اگر (!read_stdin)
			usage(diff_درخت_usage);
		بشکن;
	حالت 1:
		tree1 = opt->pending.objects[0].item;
		diff_درخت_commit_oid(&tree1->oid);
		بشکن;
	حالت 2:
		tree1 = opt->pending.objects[0].item;
		tree2 = opt->pending.objects[1].item;
		اگر (merge_base) {
			ساختار object_id oid;

			diff_get_merge_base(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} وگرنه اگر (tree2->flags & UNINTERESTING) {
			SWAP(tree2, tree1);
		}
		diff_درخت_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_درخت_diff_flush(opt);
		بشکن;
	}

	اگر (read_stdin) {
		صحیح saved_nrl = 0;
		صحیح saved_dcctc = 0;

		opt->diffopt.rotate_to_strict = 0;
		opt->diffopt.no_free = 1;
		اگر (opt->diffopt.detect_rename) {
			اگر (the_repository->index->cache)
				repo_read_index(the_repository);
			opt->diffopt.setup |= DIFF_SETUP_USE_SIZE_CACHE;
		}
		وقتی (fgets(سطر, اندازه(سطر), stdin)) {
			ساختار object_id oid;

			اگر (get_oid_hex(سطر, &oid)) {
				fputs(سطر, stdout);
				fflush(stdout);
			}
			وگرنه {
				diff_درخت_stdin(سطر);
				اگر (saved_nrl < opt->diffopt.needed_rename_limit)
					saved_nrl = opt->diffopt.needed_rename_limit;
				اگر (opt->diffopt.degraded_cc_to_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_to_c = saved_dcctc;
		opt->diffopt.needed_rename_limit = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	بازگشت diff_result_code(opt);
}
