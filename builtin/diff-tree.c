#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "log-tree.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "tmp-objdir.h"
#அந்தர்பாவ "tree.h"

நிலையான கட்டமைப்பு rev_info log_மரம்_opt;

நிலையான முழு diff_மரம்_commit_oid(மாறிலி கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு commit *commit = lookup_commit_reference(the_repository, oid);
	எனில் (!commit)
		திரும்பு -1;
	திரும்பு log_மரம்_commit(&log_மரம்_opt, commit);
}

/* Diff one or more commits. */
நிலையான முழு stdin_diff_commit(கட்டமைப்பு commit *commit, மாறிலி எழுத்து *p)
{
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு commit_list **pptr = வெற்று;

	/* Graft the fake parents locally to the commit */
	போது (isspace(*p++) && !parse_oid_hex(p, &oid, &p)) {
		கட்டமைப்பு commit *parent = lookup_commit(the_repository, &oid);
		எனில் (!pptr) {
			/* Free the real parent list */
			commit_list_free(commit->parents);
			commit->parents = வெற்று;
			pptr = &(commit->parents);
		}
		எனில் (parent) {
			pptr = &commit_list_செருகு(parent, pptr)->next;
		}
	}
	திரும்பு log_மரம்_commit(&log_மரம்_opt, commit);
}

/* Diff two trees. */
நிலையான முழு stdin_diff_trees(கட்டமைப்பு மரம் *tree1, மாறிலி எழுத்து *p)
{
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு மரம் *tree2;
	எனில் (!isspace(*p++) || parse_oid_hex(p, &oid, &p) || *p)
		திரும்பு error("Need exactly two trees, separated by a space");
	tree2 = lookup_மரம்(the_repository, &oid);
	எனில் (!tree2 || repo_parse_மரம்(the_repository, tree2))
		திரும்பு -1;
	printf("%s %s\n", oid_to_hex(&tree1->object.oid),
			  oid_to_hex(&tree2->object.oid));
	diff_மரம்_oid(&tree1->object.oid, &tree2->object.oid,
		      "", &log_மரம்_opt.diffopt);
	log_மரம்_diff_flush(&log_மரம்_opt);
	திரும்பு 0;
}

நிலையான முழு diff_மரம்_stdin(எழுத்து *வரி)
{
	முழு len = strlen(வரி);
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு object *obj;
	மாறிலி எழுத்து *p;

	எனில் (!len || வரி[len-1] != '\n')
		திரும்பு -1;
	வரி[len-1] = 0;
	எனில் (parse_oid_hex(வரி, &oid, &p))
		திரும்பு -1;
	obj = parse_object(the_repository, &oid);
	எனில் (!obj)
		திரும்பு -1;
	எனில் (obj->type == OBJ_COMMIT)
		திரும்பு stdin_diff_commit((கட்டமைப்பு commit *)obj, p);
	எனில் (obj->type == OBJ_மரம்)
		திரும்பு stdin_diff_trees((கட்டமைப்பு மரம் *)obj, p);
	error("Object %s is a %s, not a commit or tree",
	      oid_to_hex(&oid), type_name(obj->type));
	திரும்பு -1;
}

நிலையான மாறிலி எழுத்து diff_மரம்_usage[] =
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

நிலையான வெறுமை diff_மரம்_tweak_rev(கட்டமைப்பு rev_info *rev)
{
	எனில் (!rev->diffopt.output_format) {
		எனில் (rev->dense_combined_merges)
			rev->diffopt.output_format = DIFF_FORMAT_PATCH;
		இல்லையெனில்
			rev->diffopt.output_format = DIFF_FORMAT_RAW;
	}
}

முழு cmd_diff_மரம்(முழு argc,
		  மாறிலி எழுத்து **argv,
		  மாறிலி எழுத்து *prefix,
		  கட்டமைப்பு repository *repo UNUSED)
{
	எழுத்து வரி[1000];
	கட்டமைப்பு object *tree1, *tree2;
	நிலையான கட்டமைப்பு rev_info *opt = &log_மரம்_opt;
	கட்டமைப்பு setup_revision_opt s_r_opt;
	கட்டமைப்பு userformat_want w;
	முழு read_stdin = 0;
	முழு merge_base = 0;

	show_usage_if_asked(argc, argv, diff_மரம்_usage);

	repo_config(the_repository, git_diff_basic_config, வெற்று); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_init_revisions(the_repository, opt, prefix);
	எனில் (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));
	opt->abbrev = 0;
	opt->diff = 1;
	opt->disable_stdin = 1;
	memset(&s_r_opt, 0, அளவு(s_r_opt));
	s_r_opt.tweak = diff_மரம்_tweak_rev;

	prefix = precompose_argv_prefix(argc, argv, prefix);
	argc = setup_revisions(argc, argv, opt, &s_r_opt);

	memset(&w, 0, அளவு(w));
	userformat_find_requirements(வெற்று, &w);

	எனில் (!opt->show_notes_given && w.notes)
		opt->show_notes = 1;
	எனில் (opt->show_notes)
		load_display_notes(&opt->notes_opt);

	போது (--argc > 0) {
		மாறிலி எழுத்து *arg = *++argv;

		எனில் (!strcmp(arg, "--stdin")) {
			read_stdin = 1;
			தொடர்;
		}
		எனில் (!strcmp(arg, "--merge-base")) {
			merge_base = 1;
			தொடர்;
		}
		usage(diff_மரம்_usage);
	}

	எனில் (read_stdin && merge_base)
		die(_("options '%s' and '%s' cannot be used together"), "--stdin", "--merge-base");
	எனில் (merge_base && opt->pending.nr != 2)
		die(_("--merge-base only works with two commits"));

	opt->diffopt.rotate_to_strict = 1;

	/*
	 * NOTE!  We expect "a..b" to expand to "^a b" but it is
	 * perfectly valid for revision range parser to yield "b ^a",
	 * which means the same thing. If we get the latter, i.e. the
	 * second one is marked UNINTERESTING, we recover the original
	 * order the user gave, i.e. "a..b", by swapping the trees.
	 */
	தேர்வு (opt->pending.nr) {
	நிலை 0:
		எனில் (!read_stdin)
			usage(diff_மரம்_usage);
		நிறுத்து;
	நிலை 1:
		tree1 = opt->pending.objects[0].item;
		diff_மரம்_commit_oid(&tree1->oid);
		நிறுத்து;
	நிலை 2:
		tree1 = opt->pending.objects[0].item;
		tree2 = opt->pending.objects[1].item;
		எனில் (merge_base) {
			கட்டமைப்பு object_id oid;

			diff_get_merge_base(opt, &oid);
			tree1 = lookup_object(the_repository, &oid);
		} இல்லையெனில் எனில் (tree2->flags & UNINTERESTING) {
			SWAP(tree2, tree1);
		}
		diff_மரம்_oid(&tree1->oid, &tree2->oid, "", &opt->diffopt);
		log_மரம்_diff_flush(opt);
		நிறுத்து;
	}

	எனில் (read_stdin) {
		முழு saved_nrl = 0;
		முழு saved_dcctc = 0;

		opt->diffopt.rotate_to_strict = 0;
		opt->diffopt.no_free = 1;
		எனில் (opt->diffopt.detect_rename) {
			எனில் (the_repository->index->cache)
				repo_read_index(the_repository);
			opt->diffopt.setup |= DIFF_SETUP_USE_SIZE_CACHE;
		}
		போது (fgets(வரி, அளவு(வரி), stdin)) {
			கட்டமைப்பு object_id oid;

			எனில் (get_oid_hex(வரி, &oid)) {
				fputs(வரி, stdout);
				fflush(stdout);
			}
			இல்லையெனில் {
				diff_மரம்_stdin(வரி);
				எனில் (saved_nrl < opt->diffopt.needed_rename_limit)
					saved_nrl = opt->diffopt.needed_rename_limit;
				எனில் (opt->diffopt.degraded_cc_to_c)
					saved_dcctc = 1;
			}
		}
		opt->diffopt.degraded_cc_to_c = saved_dcctc;
		opt->diffopt.needed_rename_limit = saved_nrl;
		opt->diffopt.no_free = 0;
		diff_free(&opt->diffopt);
	}

	திரும்பு diff_result_code(opt);
}
