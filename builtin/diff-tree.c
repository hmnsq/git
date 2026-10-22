#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

ساكن بنية rev_info log_درخت_opt;

ساكن صحيح diff_درخت_commit_oid(ثابت بنية object_id *oid)
{
	بنية commit *commit = lookup_commit_reference(the_repository, oid);
	إذا (!commit)
		أرجع -1;
	أرجع log_درخت_commit(&log_درخت_opt, commit);
}

/* Diff one or more commits. */
ساكن صحيح stdin_diff_commit(بنية commit *commit, ثابت حرف *p)
{
	بنية object_id oid;
	بنية commit_list **pptr = عدم;

	/* Graft the fake parents locally to the commit */
	طالما (خلا_اے(*p++) && !parse_oid_hex(p, &oid, &p)) {
		بنية commit *ماپا = lookup_commit(the_repository, &oid);
		إذا (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = عدم;
			pptr = &(commit->parents);
		}
		إذا (ماپا) {
			pptr = &commit_list_داخل_کرو(ماپا, pptr)->اگلا;
		}
	}
	أرجع log_درخت_commit(&log_درخت_opt, commit);
}

/* Diff two trees. */
ساكن صحيح stdin_diff_trees(بنية درخت *tree1, ثابت حرف *p)
{
	بنية object_id oid;
	بنية درخت *tree2;
	إذا (!خلا_اے(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		أرجع error("Need exactly two trees, separated by a space");
	tree2 = lookup_درخت(the_repository, &oid);
	إذا (!tree2 || repo_parse_درخت(the_repository, tree2))
		أرجع -1;
	لکھو("%s %s\n", oid_منزل_hex(&tree1->object.oid),
			  oid_منزل_hex(&tree2->object.oid));
	diff_درخت_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_درخت_opt.diffopt);
	log_درخت_diff_flush(&log_درخت_opt);
	أرجع 0;
}

ساكن صحيح diff_درخت_stdin(حرف *سطر)
{
	صحيح len = لڑی_لمبائی(سطر);
	بنية object_id oid;
	بنية object *obj;
	ثابت حرف *p;

	إذا (!len || سطر[len-1] != '\n')
		أرجع -1;
	سطر[len-1] = 0;
	إذا (parse_oid_hex(سطر, &oid, &p))
		أرجع -1;
	obj = parse_object(the_repository, &oid);
	إذا (!obj)
		أرجع -1;
	إذا (obj->type == OBJ_COMMIT)
		أرجع stdin_diff_commit((بنية commit *)obj, p);
	إذا (obj->type == OBJ_درخت)
		أرجع stdin_diff_trees((بنية درخت *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_منزل_hex(&oid), type_name(obj->type));
	أرجع -1;
}

ساكن ثابت حرف diff_درخت_usage[] =
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

ساكن فراغ diff_درخت_tweak_rev(بنية rev_info *rev)
{
	إذا (!rev->diffopt.output_format) {
		إذا (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		وإلا
			rev->diffopt.output_format = DIFF_FORMAT_RAW;
	}
}

صحيح cmd_diff_درخت(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *اگیتر,
		  بنية repository *repo بن_ورتیا)
{
	حرف سطر[1000];
	بنية object *tree1, *tree2;
	ساكن بنية rev_info *opt = &log_درخت_opt;
	بنية تیاری_revision_opt s_r_opt;
	بنية userformat_want w;
	صحيح read_stdin = 0;
	صحيح ملاؤ_ادھار = 0;

	show_usage_if_asked(argc, argv, diff_درخت_usage);

	repo_config(the_repository, git_diff_basic_config, عدم); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھریا_اشاریہ = 0;

	repo_init_revisions(the_repository, opt, اگیتر);
	إذا (repo_read_اشاریہ(the_repository) < 0)
		die(_("index file corrupt"));
	opt->abbrev = 0;
	opt->diff = 1;
	opt->disable_stdin = 1;
	memset(&s_r_opt, 0, حجم(s_r_opt));
	s_r_opt.tweak = diff_درخت_tweak_rev;

	اگیتر = precompose_argv_اگیتر(argc, argv, اگیتر);
	argc = تیاری_revisions(argc, argv, opt, &s_r_opt);

	memset(&w, 0, حجم(w));
	userformat_لبھو_requirements(عدم, &w);

	إذا (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	إذا (opt->show_notes)
		لدو_display_notes(&opt->notes_opt);

	طالما (--argc > 0) {
		ثابت حرف *arg = *++argv;

		إذا (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			استمر;
		}
		إذا (!strcmp(arg, "--merge-base")) {
			ملاؤ_ادھار = 1;
			استمر;
		}
		usage(diff_درخت_usage);
	}

	إذا (read_stdin && ملاؤ_ادھار)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	إذا (ملاؤ_ادھار && opt->pending.nr != 2)
		die(_("--merge-base only works with two commits"));

	opt->diffopt.rotate_منزل_strict = 1;

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
			usage(diff_درخت_usage);
		اكسر;
	حالة 1:
		tree1 = opt->pending.objects[0].شے;
		diff_درخت_commit_oid(&tree1->oid);
		اكسر;
	حالة 2:
		tree1 = opt->pending.objects[0].شے;
		tree2 = opt->pending.objects[1].شے;
		إذا (ملاؤ_ادھار) {
			بنية object_id oid;

			diff_get_ملاؤ_ادھار(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} وإلا إذا (tree2->flags & UNINTERESTING) {
			وٹاندرا(tree2, tree1);
		}
		diff_درخت_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_درخت_diff_flush(opt);
		اكسر;
	}

	إذا (read_stdin) {
		صحيح saved_nrl = 0;
		صحيح saved_dcctc = 0;

		opt->diffopt.rotate_منزل_strict = 0;
		opt->diffopt.no_free = 1;
		إذا (opt->diffopt.detect_rename) {
			إذا (the_repository->اشاریہ->cache)
				repo_read_اشاریہ(the_repository);
			opt->diffopt.تیاری |= DIFF_تیاری_USE_ناپ_CACHE;
		}
		طالما (fgets(سطر, حجم(سطر), stdin)) {
			بنية object_id oid;

			إذا (get_oid_hex(سطر, &oid)) {
				fputs(سطر, stdout);
				fflush(stdout);
			}
			وإلا {
				diff_درخت_stdin(سطر);
				إذا (saved_nrl < opt->diffopt.needed_rename_حد_بندی)
					saved_nrl = opt->diffopt.needed_rename_حد_بندی;
				إذا (opt->diffopt.degraded_cc_منزل_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_منزل_c = saved_dcctc;
		opt->diffopt.needed_rename_حد_بندی = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	أرجع diff_نتیجہ_کوڈ(opt);
}
