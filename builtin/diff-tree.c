#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "log-tree.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "tmp-objdir.h"
#ಅನ್ತರ್ಭಾವ "tree.h"

ಸ್ಥಿರ ರಚನೆ rev_info log_ವೃಕ್ಷ_opt;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ diff_ವೃಕ್ಷ_commit_oid(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ರಚನೆ commit *commit = lookup_commit_reference(the_repository, oid);
	ಆದರೆ (!commit)
		ಹಿಂತಿರುಗಿ -1;
	ಹಿಂತಿರುಗಿ log_ವೃಕ್ಷ_commit(&log_ವೃಕ್ಷ_opt, commit);
}

/* Diff one or more commits. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stdin_diff_commit(ರಚನೆ commit *commit, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p)
{
	ರಚನೆ object_id oid;
	ರಚನೆ commit_list **pptr = ಖಾಲಿ;

	/* Graft the fake parents locally to the commit */
	ಆಗ (ಖಾಲಿಯೇ(*p++) && !parse_oid_hex(p, &oid, &p)) {
		ರಚನೆ commit *ಪೋಷಕ = lookup_commit(the_repository, &oid);
		ಆದರೆ (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = ಖಾಲಿ;
			pptr = &(commit->parents);
		}
		ಆದರೆ (ಪೋಷಕ) {
			pptr = &commit_list_ಸೇರಿಸು(ಪೋಷಕ, pptr)->ಮುಂದಿನ;
		}
	}
	ಹಿಂತಿರುಗಿ log_ವೃಕ್ಷ_commit(&log_ವೃಕ್ಷ_opt, commit);
}

/* Diff two trees. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stdin_diff_trees(ರಚನೆ ವೃಕ್ಷ *tree1, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p)
{
	ರಚನೆ object_id oid;
	ರಚನೆ ವೃಕ್ಷ *tree2;
	ಆದರೆ (!ಖಾಲಿಯೇ(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		ಹಿಂತಿರುಗಿ error("Need exactly two trees, separated by a space");
	tree2 = lookup_ವೃಕ್ಷ(the_repository, &oid);
	ಆದರೆ (!tree2 || repo_parse_ವೃಕ್ಷ(the_repository, tree2))
		ಹಿಂತಿರುಗಿ -1;
	ಬರೆ("%s %s\n", oid_ಗಮ್ಯ_hex(&tree1->object.oid),
			  oid_ಗಮ್ಯ_hex(&tree2->object.oid));
	diff_ವೃಕ್ಷ_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_ವೃಕ್ಷ_opt.diffopt);
	log_ವೃಕ್ಷ_diff_flush(&log_ವೃಕ್ಷ_opt);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ diff_ವೃಕ್ಷ_stdin(ಅಕ್ಷರ *ಸಾಲು)
{
	ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಸಾಲು);
	ರಚನೆ object_id oid;
	ರಚನೆ object *obj;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;

	ಆದರೆ (!len || ಸಾಲು[len-1] != '\n')
		ಹಿಂತಿರುಗಿ -1;
	ಸಾಲು[len-1] = 0;
	ಆದರೆ (parse_oid_hex(ಸಾಲು, &oid, &p))
		ಹಿಂತಿರುಗಿ -1;
	obj = parse_object(the_repository, &oid);
	ಆದರೆ (!obj)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (obj->type == OBJ_COMMIT)
		ಹಿಂತಿರುಗಿ stdin_diff_commit((ರಚನೆ commit *)obj, p);
	ಆದರೆ (obj->type == OBJ_ವೃಕ್ಷ)
		ಹಿಂತಿರುಗಿ stdin_diff_trees((ರಚನೆ ವೃಕ್ಷ *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_ಗಮ್ಯ_hex(&oid), type_name(obj->type));
	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ diff_ವೃಕ್ಷ_usage[] =
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

ಸ್ಥಿರ ಶೂನ್ಯ diff_ವೃಕ್ಷ_tweak_rev(ರಚನೆ rev_info *rev)
{
	ಆದರೆ (!rev->diffopt.output_format) {
		ಆದರೆ (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		ಇಲ್ಲದಿದ್ದರೆ
			rev->diffopt.output_format = DIFF_FORMAT_RAW;
	}
}

ಪೂರ್ಣಾಂಕ cmd_diff_ವೃಕ್ಷ(ಪೂರ್ಣಾಂಕ argc,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		  ರಚನೆ repository *repo ಬಳಸದ)
{
	ಅಕ್ಷರ ಸಾಲು[1000];
	ರಚನೆ object *tree1, *tree2;
	ಸ್ಥಿರ ರಚನೆ rev_info *opt = &log_ವೃಕ್ಷ_opt;
	ರಚನೆ ಸಿದ್ಧತೆ_revision_opt s_r_opt;
	ರಚನೆ userformat_want w;
	ಪೂರ್ಣಾಂಕ read_stdin = 0;
	ಪೂರ್ಣಾಂಕ ವಿಲೀನ_ತಳ = 0;

	show_usage_if_asked(argc, argv, diff_ವೃಕ್ಷ_usage);

	repo_config(the_repository, git_diff_basic_config, ಖಾಲಿ); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	repo_init_revisions(the_repository, opt, ಪೂರ್ವಪ್ರತ್ಯಯ);
	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
		die(_("index file corrupt"));
	opt->abbrev = 0;
	opt->diff = 1;
	opt->disable_stdin = 1;
	memset(&s_r_opt, 0, ಗಾತ್ರ(s_r_opt));
	s_r_opt.tweak = diff_ವೃಕ್ಷ_tweak_rev;

	ಪೂರ್ವಪ್ರತ್ಯಯ = precompose_argv_ಪೂರ್ವಪ್ರತ್ಯಯ(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ);
	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, opt, &s_r_opt);

	memset(&w, 0, ಗಾತ್ರ(w));
	userformat_ಕಂಡುಹಿಡಿ_requirements(ಖಾಲಿ, &w);

	ಆದರೆ (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	ಆದರೆ (opt->show_notes)
		ತುಂಬು_display_notes(&opt->notes_opt);

	ಆಗ (--argc > 0) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = *++argv;

		ಆದರೆ (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!strcmp(arg, "--merge-base")) {
			ವಿಲೀನ_ತಳ = 1;
			ಮುಂದುವರಿ;
		}
		usage(diff_ವೃಕ್ಷ_usage);
	}

	ಆದರೆ (read_stdin && ವಿಲೀನ_ತಳ)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	ಆದರೆ (ವಿಲೀನ_ತಳ && opt->pending.nr != 2)
		die(_("--merge-base only works with two commits"));

	opt->diffopt.rotate_ಗಮ್ಯ_strict = 1;

	/*
	 * NOTE!  We expect "a..b" to expand to "^a b" but it is
	 * perfectly valid for revision range parser to yield "b ^a",
	 * which means the same thing. If we get the latter, i.e. the
	 * second one is marked UNINTERESTING, we recover the original
	 * order the user gave, i.e. "a..b", by swapping the trees.
	 */
	ಆಯ್ಕೆ (opt->pending.nr) {
	ಸ್ಥಿತಿ 0:
		ಆದರೆ (!read_stdin)
			usage(diff_ವೃಕ್ಷ_usage);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ 1:
		tree1 = opt->pending.objects[0].ಅಂಶ;
		diff_ವೃಕ್ಷ_commit_oid(&tree1->oid);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ 2:
		tree1 = opt->pending.objects[0].ಅಂಶ;
		tree2 = opt->pending.objects[1].ಅಂಶ;
		ಆದರೆ (ವಿಲೀನ_ತಳ) {
			ರಚನೆ object_id oid;

			diff_get_ವಿಲೀನ_ತಳ(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (tree2->flags & UNINTERESTING) {
			ಅದಲುಬದಲು(tree2, tree1);
		}
		diff_ವೃಕ್ಷ_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_ವೃಕ್ಷ_diff_flush(opt);
		ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (read_stdin) {
		ಪೂರ್ಣಾಂಕ saved_nrl = 0;
		ಪೂರ್ಣಾಂಕ saved_dcctc = 0;

		opt->diffopt.rotate_ಗಮ್ಯ_strict = 0;
		opt->diffopt.no_free = 1;
		ಆದರೆ (opt->diffopt.detect_rename) {
			ಆದರೆ (the_repository->ಸೂಚ್ಯಂಕ->cache)
				repo_read_ಸೂಚ್ಯಂಕ(the_repository);
			opt->diffopt.ಸಿದ್ಧತೆ |= DIFF_ಸಿದ್ಧತೆ_USE_ಗಾತ್ರ_CACHE;
		}
		ಆಗ (fgets(ಸಾಲು, ಗಾತ್ರ(ಸಾಲು), stdin)) {
			ರಚನೆ object_id oid;

			ಆದರೆ (get_oid_hex(ಸಾಲು, &oid)) {
				fputs(ಸಾಲು, stdout);
				fflush(stdout);
			}
			ಇಲ್ಲದಿದ್ದರೆ {
				diff_ವೃಕ್ಷ_stdin(ಸಾಲು);
				ಆದರೆ (saved_nrl < opt->diffopt.needed_rename_ಎಲ್ಲೆ)
					saved_nrl = opt->diffopt.needed_rename_ಎಲ್ಲೆ;
				ಆದರೆ (opt->diffopt.degraded_cc_ಗಮ್ಯ_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_ಗಮ್ಯ_c = saved_dcctc;
		opt->diffopt.needed_rename_ಎಲ್ಲೆ = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	ಹಿಂತಿರುಗಿ diff_ಫಲಿತಾಂಶ_ಸಂಕೇತ(opt);
}
