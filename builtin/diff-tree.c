#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "log-tree.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "tmp-objdir.h"
#અન્તર્ભાવ "tree.h"

સ્થિર સંરચના rev_info log_વૃક્ષ_opt;

સ્થિર પૂર્ણાંક diff_વૃક્ષ_commit_oid(અચલ સંરચના object_id *oid)
{
	સંરચના commit *commit = lookup_commit_reference(the_repository, oid);
	જો (!commit)
		પાછા -1;
	પાછા log_વૃક્ષ_commit(&log_વૃક્ષ_opt, commit);
}

/* Diff one or more commits. */
સ્થિર પૂર્ણાંક stdin_diff_commit(સંરચના commit *commit, અચલ અક્ષર *p)
{
	સંરચના object_id oid;
	સંરચના commit_list **pptr = ખાલી;

	/* Graft the fake parents locally to the commit */
	જ્યારે (isspace(*p++) && !parse_oid_hex(p, &oid, &p)) {
		સંરચના commit *parent = lookup_commit(the_repository, &oid);
		જો (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = ખાલી;
			pptr = &(commit->parents);
		}
		જો (parent) {
			pptr = &commit_list_ઉમેરો(parent, pptr)->next;
		}
	}
	પાછા log_વૃક્ષ_commit(&log_વૃક્ષ_opt, commit);
}

/* Diff two trees. */
સ્થિર પૂર્ણાંક stdin_diff_trees(સંરચના વૃક્ષ *tree1, અચલ અક્ષર *p)
{
	સંરચના object_id oid;
	સંરચના વૃક્ષ *tree2;
	જો (!isspace(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		પાછા error("Need exactly two trees, separated by a space");
	tree2 = lookup_વૃક્ષ(the_repository, &oid);
	જો (!tree2 || repo_parse_વૃક્ષ(the_repository, tree2))
		પાછા -1;
	printf("%s %s\n", oid_to_hex(&tree1->object.oid),
			  oid_to_hex(&tree2->object.oid));
	diff_વૃક્ષ_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_વૃક્ષ_opt.diffopt);
	log_વૃક્ષ_diff_flush(&log_વૃક્ષ_opt);
	પાછા 0;
}

સ્થિર પૂર્ણાંક diff_વૃક્ષ_stdin(અક્ષર *લીટી)
{
	પૂર્ણાંક len = strlen(લીટી);
	સંરચના object_id oid;
	સંરચના object *obj;
	અચલ અક્ષર *p;

	જો (!len || લીટી[len-1] != '\n')
		પાછા -1;
	લીટી[len-1] = 0;
	જો (parse_oid_hex(લીટી, &oid, &p))
		પાછા -1;
	obj = parse_object(the_repository, &oid);
	જો (!obj)
		પાછા -1;
	જો (obj->type == OBJ_COMMIT)
		પાછા stdin_diff_commit((સંરચના commit *)obj, p);
	જો (obj->type == OBJ_વૃક્ષ)
		પાછા stdin_diff_trees((સંરચના વૃક્ષ *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_to_hex(&oid), type_name(obj->type));
	પાછા -1;
}

સ્થિર અચલ અક્ષર diff_વૃક્ષ_usage[] =
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

સ્થિર શૂન્ય diff_વૃક્ષ_tweak_rev(સંરચના rev_info *rev)
{
	જો (!rev->diffopt.output_format) {
		જો (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		નહીંતર
			rev->diffopt.output_format = DIFF_FORMAT_RAW;
	}
}

પૂર્ણાંક cmd_diff_વૃક્ષ(પૂર્ણાંક argc,
		  અચલ અક્ષર **argv,
		  અચલ અક્ષર *prefix,
		  સંરચના repository *repo UNUSED)
{
	અક્ષર લીટી[1000];
	સંરચના object *tree1, *tree2;
	સ્થિર સંરચના rev_info *opt = &log_વૃક્ષ_opt;
	સંરચના setup_revision_opt s_r_opt;
	સંરચના userformat_want w;
	પૂર્ણાંક read_stdin = 0;
	પૂર્ણાંક merge_base = 0;

	show_usage_if_asked(argc, argv, diff_વૃક્ષ_usage);

	repo_config(the_repository, git_diff_basic_config, ખાલી); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_init_revisions(the_repository, opt, prefix);
	જો (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));
	opt->abbrev = 0;
	opt->diff = 1;
	opt->disable_stdin = 1;
	memset(&s_r_opt, 0, કદ(s_r_opt));
	s_r_opt.tweak = diff_વૃક્ષ_tweak_rev;

	prefix = precompose_argv_prefix(argc, argv, prefix);
	argc = setup_revisions(argc, argv, opt, &s_r_opt);

	memset(&w, 0, કદ(w));
	userformat_find_requirements(ખાલી, &w);

	જો (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	જો (opt->show_notes)
		load_display_notes(&opt->notes_opt);

	જ્યારે (--argc > 0) {
		અચલ અક્ષર *arg = *++argv;

		જો (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			ચાલુ;
		}
		જો (!strcmp(arg, "--merge-base")) {
			merge_base = 1;
			ચાલુ;
		}
		usage(diff_વૃક્ષ_usage);
	}

	જો (read_stdin && merge_base)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	જો (merge_base && opt->pending.nr != 2)
		die(_("--merge-base only works with two commits"));

	opt->diffopt.rotate_to_strict = 1;

	/*
	 * NOTE!  We expect "a..b" to expand to "^a b" but it is
	 * perfectly valid for revision range parser to yield "b ^a",
	 * which means the same thing. If we get the latter, i.e. the
	 * second one is marked UNINTERESTING, we recover the original
	 * order the user gave, i.e. "a..b", by swapping the trees.
	 */
	પસંદગી (opt->pending.nr) {
	સ્થિતિ 0:
		જો (!read_stdin)
			usage(diff_વૃક્ષ_usage);
		તોડો;
	સ્થિતિ 1:
		tree1 = opt->pending.objects[0].item;
		diff_વૃક્ષ_commit_oid(&tree1->oid);
		તોડો;
	સ્થિતિ 2:
		tree1 = opt->pending.objects[0].item;
		tree2 = opt->pending.objects[1].item;
		જો (merge_base) {
			સંરચના object_id oid;

			diff_get_merge_base(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} નહીંતર જો (tree2->flags & UNINTERESTING) {
			SWAP(tree2, tree1);
		}
		diff_વૃક્ષ_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_વૃક્ષ_diff_flush(opt);
		તોડો;
	}

	જો (read_stdin) {
		પૂર્ણાંક saved_nrl = 0;
		પૂર્ણાંક saved_dcctc = 0;

		opt->diffopt.rotate_to_strict = 0;
		opt->diffopt.no_free = 1;
		જો (opt->diffopt.detect_rename) {
			જો (the_repository->index->cache)
				repo_read_index(the_repository);
			opt->diffopt.setup |= DIFF_SETUP_USE_SIZE_CACHE;
		}
		જ્યારે (fgets(લીટી, કદ(લીટી), stdin)) {
			સંરચના object_id oid;

			જો (get_oid_hex(લીટી, &oid)) {
				fputs(લીટી, stdout);
				fflush(stdout);
			}
			નહીંતર {
				diff_વૃક્ષ_stdin(લીટી);
				જો (saved_nrl < opt->diffopt.needed_rename_limit)
					saved_nrl = opt->diffopt.needed_rename_limit;
				જો (opt->diffopt.degraded_cc_to_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_to_c = saved_dcctc;
		opt->diffopt.needed_rename_limit = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	પાછા diff_result_code(opt);
}
