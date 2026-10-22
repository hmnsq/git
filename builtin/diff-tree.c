#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "log-tree.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "tmp-objdir.h"
#అన్తర్భావ "tree.h"

స్థిర నిర్మాణం rev_info log_వృక్షం_opt;

స్థిర పూర్ణాంకం diff_వృక్షం_commit_oid(స్థిరరాశి నిర్మాణం object_id *oid)
{
	నిర్మాణం commit *commit = lookup_commit_reference(the_repository, oid);
	ఒకవేళ (!commit)
		తిరిగి -1;
	తిరిగి log_వృక్షం_commit(&log_వృక్షం_opt, commit);
}

/* Diff one or more commits. */
స్థిర పూర్ణాంకం stdin_diff_commit(నిర్మాణం commit *commit, స్థిరరాశి అక్షరం *p)
{
	నిర్మాణం object_id oid;
	నిర్మాణం commit_list **pptr = ఖాళీ;

	/* Graft the fake parents locally to the commit */
	అయితే (ఖాళీనా(*p++) && !parse_oid_hex(p, &oid, &p)) {
		నిర్మాణం commit *మాతృ = lookup_commit(the_repository, &oid);
		ఒకవేళ (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = ఖాళీ;
			pptr = &(commit->parents);
		}
		ఒకవేళ (మాతృ) {
			pptr = &commit_list_చొప్పించు(మాతృ, pptr)->తదుపరి;
		}
	}
	తిరిగి log_వృక్షం_commit(&log_వృక్షం_opt, commit);
}

/* Diff two trees. */
స్థిర పూర్ణాంకం stdin_diff_trees(నిర్మాణం వృక్షం *tree1, స్థిరరాశి అక్షరం *p)
{
	నిర్మాణం object_id oid;
	నిర్మాణం వృక్షం *tree2;
	ఒకవేళ (!ఖాళీనా(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		తిరిగి error("Need exactly two trees, separated by a space");
	tree2 = lookup_వృక్షం(the_repository, &oid);
	ఒకవేళ (!tree2 || repo_parse_వృక్షం(the_repository, tree2))
		తిరిగి -1;
	రాయి("%s %s\n", oid_గమ్యం_hex(&tree1->object.oid),
			  oid_గమ్యం_hex(&tree2->object.oid));
	diff_వృక్షం_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_వృక్షం_opt.diffopt);
	log_వృక్షం_diff_flush(&log_వృక్షం_opt);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం diff_వృక్షం_stdin(అక్షరం *పంక్తి)
{
	పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(పంక్తి);
	నిర్మాణం object_id oid;
	నిర్మాణం object *obj;
	స్థిరరాశి అక్షరం *p;

	ఒకవేళ (!len || పంక్తి[len-1] != '\n')
		తిరిగి -1;
	పంక్తి[len-1] = 0;
	ఒకవేళ (parse_oid_hex(పంక్తి, &oid, &p))
		తిరిగి -1;
	obj = parse_object(the_repository, &oid);
	ఒకవేళ (!obj)
		తిరిగి -1;
	ఒకవేళ (obj->type == OBJ_COMMIT)
		తిరిగి stdin_diff_commit((నిర్మాణం commit *)obj, p);
	ఒకవేళ (obj->type == OBJ_వృక్షం)
		తిరిగి stdin_diff_trees((నిర్మాణం వృక్షం *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_గమ్యం_hex(&oid), type_name(obj->type));
	తిరిగి -1;
}

స్థిర స్థిరరాశి అక్షరం diff_వృక్షం_usage[] =
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

స్థిర శూన్య diff_వృక్షం_tweak_rev(నిర్మాణం rev_info *rev)
{
	ఒకవేళ (!rev->diffopt.output_format) {
		ఒకవేళ (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		లేకపోతే
			rev->diffopt.output_format = DIFF_FORMAT_RAW;
	}
}

పూర్ణాంకం cmd_diff_వృక్షం(పూర్ణాంకం argc,
		  స్థిరరాశి అక్షరం **argv,
		  స్థిరరాశి అక్షరం *ఉపసర్గ,
		  నిర్మాణం repository *repo వాడని)
{
	అక్షరం పంక్తి[1000];
	నిర్మాణం object *tree1, *tree2;
	స్థిర నిర్మాణం rev_info *opt = &log_వృక్షం_opt;
	నిర్మాణం అమరిక_revision_opt s_r_opt;
	నిర్మాణం userformat_want w;
	పూర్ణాంకం read_stdin = 0;
	పూర్ణాంకం విలీనం_పునాది = 0;

	show_usage_if_asked(argc, argv, diff_వృక్షం_usage);

	repo_config(the_repository, git_diff_basic_config, ఖాళీ); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	repo_init_revisions(the_repository, opt, ఉపసర్గ);
	ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
		die(_("index file corrupt"));
	opt->abbrev = 0;
	opt->diff = 1;
	opt->disable_stdin = 1;
	memset(&s_r_opt, 0, పరిమాణం(s_r_opt));
	s_r_opt.tweak = diff_వృక్షం_tweak_rev;

	ఉపసర్గ = precompose_argv_ఉపసర్గ(argc, argv, ఉపసర్గ);
	argc = అమరిక_revisions(argc, argv, opt, &s_r_opt);

	memset(&w, 0, పరిమాణం(w));
	userformat_కనుగొను_requirements(ఖాళీ, &w);

	ఒకవేళ (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	ఒకవేళ (opt->show_notes)
		ఎక్కించు_display_notes(&opt->notes_opt);

	అయితే (--argc > 0) {
		స్థిరరాశి అక్షరం *arg = *++argv;

		ఒకవేళ (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			కొనసాగు;
		}
		ఒకవేళ (!strcmp(arg, "--merge-base")) {
			విలీనం_పునాది = 1;
			కొనసాగు;
		}
		usage(diff_వృక్షం_usage);
	}

	ఒకవేళ (read_stdin && విలీనం_పునాది)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	ఒకవేళ (విలీనం_పునాది && opt->pending.nr != 2)
		die(_("--merge-base only works with two commits"));

	opt->diffopt.rotate_గమ్యం_strict = 1;

	/*
	 * NOTE!  We expect "a..b" to expand to "^a b" but it is
	 * perfectly valid for revision range parser to yield "b ^a",
	 * which means the same thing. If we get the latter, i.e. the
	 * second one is marked UNINTERESTING, we recover the original
	 * order the user gave, i.e. "a..b", by swapping the trees.
	 */
	ఎంపిక (opt->pending.nr) {
	స్థితి 0:
		ఒకవేళ (!read_stdin)
			usage(diff_వృక్షం_usage);
		ఆపు;
	స్థితి 1:
		tree1 = opt->pending.objects[0].అంశం;
		diff_వృక్షం_commit_oid(&tree1->oid);
		ఆపు;
	స్థితి 2:
		tree1 = opt->pending.objects[0].అంశం;
		tree2 = opt->pending.objects[1].అంశం;
		ఒకవేళ (విలీనం_పునాది) {
			నిర్మాణం object_id oid;

			diff_get_విలీనం_పునాది(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} లేకపోతే ఒకవేళ (tree2->flags & UNINTERESTING) {
			మార్పిడి(tree2, tree1);
		}
		diff_వృక్షం_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_వృక్షం_diff_flush(opt);
		ఆపు;
	}

	ఒకవేళ (read_stdin) {
		పూర్ణాంకం saved_nrl = 0;
		పూర్ణాంకం saved_dcctc = 0;

		opt->diffopt.rotate_గమ్యం_strict = 0;
		opt->diffopt.no_free = 1;
		ఒకవేళ (opt->diffopt.detect_rename) {
			ఒకవేళ (the_repository->సూచీ->cache)
				repo_read_సూచీ(the_repository);
			opt->diffopt.అమరిక |= DIFF_అమరిక_USE_కొలత_CACHE;
		}
		అయితే (fgets(పంక్తి, పరిమాణం(పంక్తి), stdin)) {
			నిర్మాణం object_id oid;

			ఒకవేళ (get_oid_hex(పంక్తి, &oid)) {
				fputs(పంక్తి, stdout);
				fflush(stdout);
			}
			లేకపోతే {
				diff_వృక్షం_stdin(పంక్తి);
				ఒకవేళ (saved_nrl < opt->diffopt.needed_rename_హద్దు)
					saved_nrl = opt->diffopt.needed_rename_హద్దు;
				ఒకవేళ (opt->diffopt.degraded_cc_గమ్యం_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_గమ్యం_c = saved_dcctc;
		opt->diffopt.needed_rename_హద్దు = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	తిరిగి diff_ఫలితం_సంకేతం(opt);
}
