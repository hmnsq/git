#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

ساکن ساخت rev_info log_درخت_opt;

ساکن صحیح diff_درخت_commit_oid(ثابت ساخت object_id *oid)
{
	ساخت commit *commit = lookup_commit_reference(the_repository, oid);
	اگر (!commit)
		واپس -1;
	واپس log_درخت_commit(&log_درخت_opt, commit);
}

/* Diff one or more commits. */
ساکن صحیح stdin_diff_commit(ساخت commit *commit, ثابت حرف *p)
{
	ساخت object_id oid;
	ساخت commit_list **pptr = تہی;

	/* Graft the fake parents locally to the commit */
	جب (خلا_ہے(*p++) && !parse_oid_hex(p, &oid, &p)) {
		ساخت commit *والد = lookup_commit(the_repository, &oid);
		اگر (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = تہی;
			pptr = &(commit->parents);
		}
		اگر (والد) {
			pptr = &commit_list_داخل_کرو(والد, pptr)->اگلا;
		}
	}
	واپس log_درخت_commit(&log_درخت_opt, commit);
}

/* Diff two trees. */
ساکن صحیح stdin_diff_trees(ساخت درخت *tree1, ثابت حرف *p)
{
	ساخت object_id oid;
	ساخت درخت *tree2;
	اگر (!خلا_ہے(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		واپس error("Need exactly two trees, separated by a space");
	tree2 = lookup_درخت(the_repository, &oid);
	اگر (!tree2 || repo_parse_درخت(the_repository, tree2))
		واپس -1;
	لکھو("%s %s\n", oid_مقصد_hex(&tree1->object.oid),
			  oid_مقصد_hex(&tree2->object.oid));
	diff_درخت_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_درخت_opt.diffopt);
	log_درخت_diff_flush(&log_درخت_opt);
	واپس 0;
}

ساکن صحیح diff_درخت_stdin(حرف *سطر)
{
	صحیح len = لڑی_لمبائی(سطر);
	ساخت object_id oid;
	ساخت object *obj;
	ثابت حرف *p;

	اگر (!len || سطر[len-1] != '\n')
		واپس -1;
	سطر[len-1] = 0;
	اگر (parse_oid_hex(سطر, &oid, &p))
		واپس -1;
	obj = parse_object(the_repository, &oid);
	اگر (!obj)
		واپس -1;
	اگر (obj->type == OBJ_COMMIT)
		واپس stdin_diff_commit((ساخت commit *)obj, p);
	اگر (obj->type == OBJ_درخت)
		واپس stdin_diff_trees((ساخت درخت *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_مقصد_hex(&oid), type_name(obj->type));
	واپس -1;
}

ساکن ثابت حرف diff_درخت_usage[] =
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

ساکن خالی diff_درخت_tweak_rev(ساخت rev_info *rev)
{
	اگر (!rev->diffopt.output_format) {
		اگر (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		ورنہ
			rev->diffopt.output_format = DIFF_FORMAT_RAW;
	}
}

صحیح cmd_diff_درخت(صحیح argc,
		  ثابت حرف **argv,
		  ثابت حرف *سابقہ,
		  ساخت repository *repo غیر_مستعمل)
{
	حرف سطر[1000];
	ساخت object *tree1, *tree2;
	ساکن ساخت rev_info *opt = &log_درخت_opt;
	ساخت تیاری_revision_opt s_r_opt;
	ساخت userformat_want w;
	صحیح read_stdin = 0;
	صحیح ملاؤ_بنیاد = 0;

	show_usage_if_asked(argc, argv, diff_درخت_usage);

	repo_config(the_repository, git_diff_basic_config, تہی); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	repo_init_revisions(the_repository, opt, سابقہ);
	اگر (repo_read_اشاریہ(the_repository) < 0)
		die(_("index file corrupt"));
	opt->abbrev = 0;
	opt->diff = 1;
	opt->disable_stdin = 1;
	memset(&s_r_opt, 0, حجم(s_r_opt));
	s_r_opt.tweak = diff_درخت_tweak_rev;

	سابقہ = precompose_argv_سابقہ(argc, argv, سابقہ);
	argc = تیاری_revisions(argc, argv, opt, &s_r_opt);

	memset(&w, 0, حجم(w));
	userformat_ڈھونڈو_requirements(تہی, &w);

	اگر (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	اگر (opt->show_notes)
		بھرو_display_notes(&opt->notes_opt);

	جب (--argc > 0) {
		ثابت حرف *arg = *++argv;

		اگر (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			جاری;
		}
		اگر (!strcmp(arg, "--merge-base")) {
			ملاؤ_بنیاد = 1;
			جاری;
		}
		usage(diff_درخت_usage);
	}

	اگر (read_stdin && ملاؤ_بنیاد)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	اگر (ملاؤ_بنیاد && opt->pending.nr != 2)
		die(_("--merge-base only works with two commits"));

	opt->diffopt.rotate_مقصد_strict = 1;

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
		توڑو;
	حالت 1:
		tree1 = opt->pending.objects[0].شے;
		diff_درخت_commit_oid(&tree1->oid);
		توڑو;
	حالت 2:
		tree1 = opt->pending.objects[0].شے;
		tree2 = opt->pending.objects[1].شے;
		اگر (ملاؤ_بنیاد) {
			ساخت object_id oid;

			diff_get_ملاؤ_بنیاد(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} ورنہ اگر (tree2->flags & UNINTERESTING) {
			تبادلہ(tree2, tree1);
		}
		diff_درخت_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_درخت_diff_flush(opt);
		توڑو;
	}

	اگر (read_stdin) {
		صحیح saved_nrl = 0;
		صحیح saved_dcctc = 0;

		opt->diffopt.rotate_مقصد_strict = 0;
		opt->diffopt.no_free = 1;
		اگر (opt->diffopt.detect_rename) {
			اگر (the_repository->اشاریہ->cache)
				repo_read_اشاریہ(the_repository);
			opt->diffopt.تیاری |= DIFF_تیاری_USE_سائز_CACHE;
		}
		جب (fgets(سطر, حجم(سطر), stdin)) {
			ساخت object_id oid;

			اگر (get_oid_hex(سطر, &oid)) {
				fputs(سطر, stdout);
				fflush(stdout);
			}
			ورنہ {
				diff_درخت_stdin(سطر);
				اگر (saved_nrl < opt->diffopt.needed_rename_حد_بندی)
					saved_nrl = opt->diffopt.needed_rename_حد_بندی;
				اگر (opt->diffopt.degraded_cc_مقصد_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_مقصد_c = saved_dcctc;
		opt->diffopt.needed_rename_حد_بندی = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	واپس diff_نتیجہ_رمز(opt);
}
