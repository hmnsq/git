#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "diff.h"
#ضمن "commit.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "log-tree.h"
#ضمن "read-cache-ll.h"
#ضمن "revision.h"
#ضمن "tmp-objdir.h"
#ضمن "tree.h"

ساكن بنية rev_info log_درەخت_opt;

ساكن صحيح diff_درەخت_commit_oid(ثابت بنية object_id *oid)
{
	بنية commit *commit = lookup_commit_reference(the_repository, oid);
	إذا (!commit)
		أرجع -1;
	أرجع log_درەخت_commit(&log_درەخت_opt, commit);
}

/* Diff one or more commits. */
ساكن صحيح stdin_diff_commit(بنية commit *commit, ثابت حرف *p)
{
	بنية object_id oid;
	بنية commit_list **pptr = عدم;

	/* Graft the fake parents locally to the commit */
	طالما (isspace(*p++) && !parse_oid_hex(p, &oid, &p)) {
		بنية commit *parent = lookup_commit(the_repository, &oid);
		إذا (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = عدم;
			pptr = &(commit->parents);
		}
		إذا (parent) {
			pptr = &commit_list_زیادکردن(parent, pptr)->next;
		}
	}
	أرجع log_درەخت_commit(&log_درەخت_opt, commit);
}

/* Diff two trees. */
ساكن صحيح stdin_diff_trees(بنية درەخت *tree1, ثابت حرف *p)
{
	بنية object_id oid;
	بنية درەخت *tree2;
	إذا (!isspace(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		أرجع error("Need exactly two trees, separated by a space");
	tree2 = lookup_درەخت(the_repository, &oid);
	إذا (!tree2 || repo_parse_درەخت(the_repository, tree2))
		أرجع -1;
	printf("%s %s\n", oid_to_hex(&tree1->object.oid),
			  oid_to_hex(&tree2->object.oid));
	diff_درەخت_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_درەخت_opt.diffopt);
	log_درەخت_diff_flush(&log_درەخت_opt);
	أرجع 0;
}

ساكن صحيح diff_درەخت_stdin(حرف *هێڵ)
{
	صحيح len = strlen(هێڵ);
	بنية object_id oid;
	بنية object *obj;
	ثابت حرف *p;

	إذا (!len || هێڵ[len-1] != '\n')
		أرجع -1;
	هێڵ[len-1] = 0;
	إذا (parse_oid_hex(هێڵ, &oid, &p))
		أرجع -1;
	obj = parse_object(the_repository, &oid);
	إذا (!obj)
		أرجع -1;
	إذا (obj->type == OBJ_COMMIT)
		أرجع stdin_diff_commit((بنية commit *)obj, p);
	إذا (obj->type == OBJ_درەخت)
		أرجع stdin_diff_trees((بنية درەخت *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_to_hex(&oid), type_name(obj->type));
	أرجع -1;
}

ساكن ثابت حرف diff_درەخت_usage[] =
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

ساكن فراغ diff_درەخت_tweak_rev(بنية rev_info *rev)
{
	إذا (!rev->diffopt.output_format) {
		إذا (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		وإلا
			rev->diffopt.output_format = DIFF_FORMAT_RAW;
	}
}

صحيح cmd_diff_درەخت(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *prefix,
		  بنية repository *repo UNUSED)
{
	حرف هێڵ[1000];
	بنية object *tree1, *tree2;
	ساكن بنية rev_info *opt = &log_درەخت_opt;
	بنية setup_revision_opt s_r_opt;
	بنية userformat_want w;
	صحيح read_stdin = 0;
	صحيح merge_base = 0;

	show_usage_if_asked(argc, argv, diff_درەخت_usage);

	repo_config(the_repository, git_diff_basic_config, عدم); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_init_revisions(the_repository, opt, prefix);
	إذا (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));
	opt->abbrev = 0;
	opt->diff = 1;
	opt->disable_stdin = 1;
	memset(&s_r_opt, 0, حجم(s_r_opt));
	s_r_opt.tweak = diff_درەخت_tweak_rev;

	prefix = precompose_argv_prefix(argc, argv, prefix);
	argc = setup_revisions(argc, argv, opt, &s_r_opt);

	memset(&w, 0, حجم(w));
	userformat_find_requirements(عدم, &w);

	إذا (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	إذا (opt->show_notes)
		load_display_notes(&opt->notes_opt);

	طالما (--argc > 0) {
		ثابت حرف *arg = *++argv;

		إذا (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--merge-base")) {
			merge_base = 1;
			استمر;
		}
		usage(diff_درەخت_usage);
	}

	إذا (read_stdin && merge_base)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	إذا (merge_base && opt->pending.nr != 2)
		die(_("--merge-base only works with two commits"));

	opt->diffopt.rotate_to_strict = 1;

	/*
	 * NOTE!  We expect "a..b" to expand to "^a b" but it is
	 * perfectly valid for revision range parser to yield "b ^a",
	 * which means the same thing. If we get the latter, i.e. the
	 * second one is marked UNINTERESTING, we recover the original
	 * order the user gave, i.e. "a..b", by swapping the trees.
	 */
	بدل (opt->pending.nr) {
	حالة 0:
		إذا (!read_stdin)
			usage(diff_درەخت_usage);
		اكسر;
	حالة 1:
		tree1 = opt->pending.objects[0].item;
		diff_درەخت_commit_oid(&tree1->oid);
		اكسر;
	حالة 2:
		tree1 = opt->pending.objects[0].item;
		tree2 = opt->pending.objects[1].item;
		إذا (merge_base) {
			بنية object_id oid;

			diff_get_merge_base(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} وإلا إذا (tree2->flags & UNINTERESTING) {
			SWAP(tree2, tree1);
		}
		diff_درەخت_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_درەخت_diff_flush(opt);
		اكسر;
	}

	إذا (read_stdin) {
		صحيح saved_nrl = 0;
		صحيح saved_dcctc = 0;

		opt->diffopt.rotate_to_strict = 0;
		opt->diffopt.no_free = 1;
		إذا (opt->diffopt.detect_rename) {
			إذا (the_repository->index->cache)
				repo_read_index(the_repository);
			opt->diffopt.setup |= DIFF_SETUP_USE_SIZE_CACHE;
		}
		طالما (fgets(هێڵ, حجم(هێڵ), stdin)) {
			بنية object_id oid;

			إذا (get_oid_hex(هێڵ, &oid)) {
				fputs(هێڵ, stdout);
				fflush(stdout);
			}
			وإلا {
				diff_درەخت_stdin(هێڵ);
				إذا (saved_nrl < opt->diffopt.needed_rename_limit)
					saved_nrl = opt->diffopt.needed_rename_limit;
				إذا (opt->diffopt.degraded_cc_to_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_to_c = saved_dcctc;
		opt->diffopt.needed_rename_limit = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	أرجع diff_result_code(opt);
}
