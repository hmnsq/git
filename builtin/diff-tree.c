#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "log-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tmp-objdir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"

ਸਥਿਰ ਬਣਤਰ rev_info log_ਰੁੱਖ_opt;

ਸਥਿਰ ਪੂਰਨ diff_ਰੁੱਖ_commit_oid(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ commit *commit = lookup_commit_reference(the_repository, oid);
	ਜੇ (!commit)
		ਵਾਪਸ -1;
	ਵਾਪਸ log_ਰੁੱਖ_commit(&log_ਰੁੱਖ_opt, commit);
}

/* Diff one or more commits. */
ਸਥਿਰ ਪੂਰਨ stdin_diff_commit(ਬਣਤਰ commit *commit, ਅਚਲ ਅੱਖਰ *p)
{
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ commit_list **pptr = ਨਿਰਾ;

	/* Graft the fake parents locally to the commit */
	ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ(*p++) && !parse_oid_hex(p, &oid, &p)) {
		ਬਣਤਰ commit *ਮਾਪਾ = lookup_commit(the_repository, &oid);
		ਜੇ (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = ਨਿਰਾ;
			pptr = &(commit->parents);
		}
		ਜੇ (ਮਾਪਾ) {
			pptr = &commit_list_ਪਾਓ(ਮਾਪਾ, pptr)->ਅਗਲਾ;
		}
	}
	ਵਾਪਸ log_ਰੁੱਖ_commit(&log_ਰੁੱਖ_opt, commit);
}

/* Diff two trees. */
ਸਥਿਰ ਪੂਰਨ stdin_diff_trees(ਬਣਤਰ ਰੁੱਖ *tree1, ਅਚਲ ਅੱਖਰ *p)
{
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ ਰੁੱਖ *tree2;
	ਜੇ (!ਖਾਲੀ_ਥਾਂ_ਹੈ(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		ਵਾਪਸ error("Need exactly two trees, separated by a space");
	tree2 = lookup_ਰੁੱਖ(the_repository, &oid);
	ਜੇ (!tree2 || repo_parse_ਰੁੱਖ(the_repository, tree2))
		ਵਾਪਸ -1;
	ਲਿਖੋ("%s %s\n", oid_ਮੰਜ਼ਿਲ_hex(&tree1->object.oid),
			  oid_ਮੰਜ਼ਿਲ_hex(&tree2->object.oid));
	diff_ਰੁੱਖ_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_ਰੁੱਖ_opt.diffopt);
	log_ਰੁੱਖ_diff_flush(&log_ਰੁੱਖ_opt);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ diff_ਰੁੱਖ_stdin(ਅੱਖਰ *ਸਤਰ)
{
	ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(ਸਤਰ);
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ object *obj;
	ਅਚਲ ਅੱਖਰ *p;

	ਜੇ (!len || ਸਤਰ[len-1] != '\n')
		ਵਾਪਸ -1;
	ਸਤਰ[len-1] = 0;
	ਜੇ (parse_oid_hex(ਸਤਰ, &oid, &p))
		ਵਾਪਸ -1;
	obj = parse_object(the_repository, &oid);
	ਜੇ (!obj)
		ਵਾਪਸ -1;
	ਜੇ (obj->type == OBJ_COMMIT)
		ਵਾਪਸ stdin_diff_commit((ਬਣਤਰ commit *)obj, p);
	ਜੇ (obj->type == OBJ_ਰੁੱਖ)
		ਵਾਪਸ stdin_diff_trees((ਬਣਤਰ ਰੁੱਖ *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_ਮੰਜ਼ਿਲ_hex(&oid), type_name(obj->type));
	ਵਾਪਸ -1;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ diff_ਰੁੱਖ_usage[] =
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

ਸਥਿਰ ਖਾਲੀ diff_ਰੁੱਖ_tweak_rev(ਬਣਤਰ rev_info *rev)
{
	ਜੇ (!rev->diffopt.output_format) {
		ਜੇ (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		ਨਹੀਂਤਾਂ
			rev->diffopt.output_format = DIFF_FORMAT_RAW;
	}
}

ਪੂਰਨ cmd_diff_ਰੁੱਖ(ਪੂਰਨ argc,
		  ਅਚਲ ਅੱਖਰ **argv,
		  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅੱਖਰ ਸਤਰ[1000];
	ਬਣਤਰ object *tree1, *tree2;
	ਸਥਿਰ ਬਣਤਰ rev_info *opt = &log_ਰੁੱਖ_opt;
	ਬਣਤਰ ਤਿਆਰੀ_revision_opt s_r_opt;
	ਬਣਤਰ userformat_want w;
	ਪੂਰਨ read_stdin = 0;
	ਪੂਰਨ ਮਿਲਾਓ_ਅਧਾਰ = 0;

	show_usage_if_asked(argc, argv, diff_ਰੁੱਖ_usage);

	repo_config(the_repository, git_diff_basic_config, ਨਿਰਾ); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	repo_init_revisions(the_repository, opt, ਅਗੇਤਰ);
	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
		die(_("index file corrupt"));
	opt->abbrev = 0;
	opt->diff = 1;
	opt->disable_stdin = 1;
	memset(&s_r_opt, 0, ਆਕਾਰ(s_r_opt));
	s_r_opt.tweak = diff_ਰੁੱਖ_tweak_rev;

	ਅਗੇਤਰ = precompose_argv_ਅਗੇਤਰ(argc, argv, ਅਗੇਤਰ);
	argc = ਤਿਆਰੀ_revisions(argc, argv, opt, &s_r_opt);

	memset(&w, 0, ਆਕਾਰ(w));
	userformat_ਲੱਭੋ_requirements(ਨਿਰਾ, &w);

	ਜੇ (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	ਜੇ (opt->show_notes)
		ਲੱਦੋ_display_notes(&opt->notes_opt);

	ਜਦੋਂ (--argc > 0) {
		ਅਚਲ ਅੱਖਰ *arg = *++argv;

		ਜੇ (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--merge-base")) {
			ਮਿਲਾਓ_ਅਧਾਰ = 1;
			ਜਾਰੀ;
		}
		usage(diff_ਰੁੱਖ_usage);
	}

	ਜੇ (read_stdin && ਮਿਲਾਓ_ਅਧਾਰ)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	ਜੇ (ਮਿਲਾਓ_ਅਧਾਰ && opt->pending.nr != 2)
		die(_("--merge-base only works with two commits"));

	opt->diffopt.rotate_ਮੰਜ਼ਿਲ_strict = 1;

	/*
	 * NOTE!  We expect "a..b" to expand to "^a b" but it is
	 * perfectly valid for revision range parser to yield "b ^a",
	 * which means the same thing. If we get the latter, i.e. the
	 * second one is marked UNINTERESTING, we recover the original
	 * order the user gave, i.e. "a..b", by swapping the trees.
	 */
	ਚੋਣ (opt->pending.nr) {
	ਹਾਲਤ 0:
		ਜੇ (!read_stdin)
			usage(diff_ਰੁੱਖ_usage);
		ਰੋਕੋ;
	ਹਾਲਤ 1:
		tree1 = opt->pending.objects[0].ਵਸਤੂ;
		diff_ਰੁੱਖ_commit_oid(&tree1->oid);
		ਰੋਕੋ;
	ਹਾਲਤ 2:
		tree1 = opt->pending.objects[0].ਵਸਤੂ;
		tree2 = opt->pending.objects[1].ਵਸਤੂ;
		ਜੇ (ਮਿਲਾਓ_ਅਧਾਰ) {
			ਬਣਤਰ object_id oid;

			diff_get_ਮਿਲਾਓ_ਅਧਾਰ(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} ਨਹੀਂਤਾਂ ਜੇ (tree2->flags & UNINTERESTING) {
			ਅਦਲਾ_ਬਦਲੀ(tree2, tree1);
		}
		diff_ਰੁੱਖ_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_ਰੁੱਖ_diff_flush(opt);
		ਰੋਕੋ;
	}

	ਜੇ (read_stdin) {
		ਪੂਰਨ saved_nrl = 0;
		ਪੂਰਨ saved_dcctc = 0;

		opt->diffopt.rotate_ਮੰਜ਼ਿਲ_strict = 0;
		opt->diffopt.no_free = 1;
		ਜੇ (opt->diffopt.detect_rename) {
			ਜੇ (the_repository->ਸੂਚਕਾਂਕ->cache)
				repo_read_ਸੂਚਕਾਂਕ(the_repository);
			opt->diffopt.ਤਿਆਰੀ |= DIFF_ਤਿਆਰੀ_USE_ਮਾਪ_CACHE;
		}
		ਜਦੋਂ (fgets(ਸਤਰ, ਆਕਾਰ(ਸਤਰ), stdin)) {
			ਬਣਤਰ object_id oid;

			ਜੇ (get_oid_hex(ਸਤਰ, &oid)) {
				fputs(ਸਤਰ, stdout);
				fflush(stdout);
			}
			ਨਹੀਂਤਾਂ {
				diff_ਰੁੱਖ_stdin(ਸਤਰ);
				ਜੇ (saved_nrl < opt->diffopt.needed_rename_ਹੱਦਬੰਦੀ)
					saved_nrl = opt->diffopt.needed_rename_ਹੱਦਬੰਦੀ;
				ਜੇ (opt->diffopt.degraded_cc_ਮੰਜ਼ਿਲ_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_ਮੰਜ਼ਿਲ_c = saved_dcctc;
		opt->diffopt.needed_rename_ਹੱਦਬੰਦੀ = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	ਵਾਪਸ diff_ਨਤੀਜਾ_ਕੋਡ(opt);
}
