/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "ewah/ewok.h"
#include "lockfile.h"
#include "color.h"
#include "commit.h"
#include "gettext.h"
#include "tag.h"
#include "diff.h"
#include "diff-merges.h"
#include "diffcore.h"
#include "preload-index.h"
#include "read-cache-ll.h"
#include "revision.h"
#include "log-tree.h"
#include "setup.h"
#include "oid-array.h"
#include "tree.h"

#define DIFF_NO_INDEX_EXPLICIT 1
#define DIFF_NO_INDEX_IMPLICIT 2

static const var_na builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

static const var_na *blob_path(sa_nracanaa object_array_entry *entry)
{
	ສົ່ງຄືນ entry->path ? entry->path : entry->name;
}

static void stuff_change(sa_nracanaa diff_options *opt,
			 unsigned old_mode, unsigned new_mode,
			 const sa_nracanaa object_id *old_oid,
			 const sa_nracanaa object_id *new_oid,
			 ຈຳນວນເຕັມ old_oid_valid,
			 ຈຳນວນເຕັມ new_oid_valid,
			 const var_na *old_path,
			 const var_na *new_path)
{
	sa_nracanaa diff_filespec *one, *two;

	ຖ້າ (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		ສົ່ງຄືນ;

	ຖ້າ (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	ຖ້າ (opt->prefix &&
	    (strncmp(old_path, opt->prefix, opt->prefix_length) ||
	     strncmp(new_path, opt->prefix, opt->prefix_length)))
		ສົ່ງຄືນ;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

static void builtin_diff_b_f(sa_nracanaa rev_info *revs,
			     ຈຳນວນເຕັມ argc, const var_na **argv UNUSED,
			     sa_nracanaa object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	sa_nracanaa stat st;
	const var_na *path;

	ຖ້າ (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_data, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_data.items[0].match;

	ຖ້າ (lstat(path, &st))
		die_errno(_("failed to stat '%s'"), path);
	ຖ້າ (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
		die(_("'%s': not a regular file or symlink"), path);

	diff_set_mnemonic_prefix(&revs->diffopt, "o/", "w/");

	ຖ້າ (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = canon_mode(st.st_mode);

	stuff_change(&revs->diffopt,
		     blob[0]->mode, canon_mode(st.st_mode),
		     &blob[0]->item->oid, null_oid(the_hash_algo),
		     1, 0,
		     blob[0]->path ? blob[0]->path : path,
		     path);
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

static void builtin_diff_blobs(sa_nracanaa rev_info *revs,
			       ຈຳນວນເຕັມ argc, const var_na **argv UNUSED,
			       sa_nracanaa object_array_entry **blob)
{
	const unsigned mode = canon_mode(S_IFREG | 0644);

	ຖ້າ (argc > 1)
		usage(builtin_diff_usage);

	ຖ້າ (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = mode;

	ຖ້າ (blob[1]->mode == S_IFINVALID)
		blob[1]->mode = mode;

	stuff_change(&revs->diffopt,
		     blob[0]->mode, blob[1]->mode,
		     &blob[0]->item->oid, &blob[1]->item->oid,
		     1, 1,
		     blob_path(blob[0]), blob_path(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

static void builtin_diff_index(sa_nracanaa rev_info *revs,
			       ຈຳນວນເຕັມ argc, const var_na **argv)
{
	unsigned ຈຳນວນເຕັມ option = 0;
	ໃນຂະນະທີ່ (1 < argc) {
		const var_na *arg = argv[1];
		ຖ້າ (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_INDEX_CACHED;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		ບໍ່ດັ່ງນັ້ນ
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	ຖ້າ (revs->pending.nr != 1 ||
	    revs->max_count != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	ຖ້າ (!(option & DIFF_INDEX_CACHED)) {
		setup_work_tree(the_repository);
		ຖ້າ (repo_read_index_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (repo_read_index(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_index(revs, option);
}

static void builtin_diff_tree(sa_nracanaa rev_info *revs,
			      ຈຳນວນເຕັມ argc, const var_na **argv,
			      sa_nracanaa object_array_entry *ent0,
			      sa_nracanaa object_array_entry *ent1)
{
	const sa_nracanaa object_id *(oid[2]);
	sa_nracanaa object_id mb_oid;
	ຈຳນວນເຕັມ merge_base = 0;

	ໃນຂະນະທີ່ (1 < argc) {
		const var_na *arg = argv[1];
		ຖ້າ (!strcmp(arg, "--merge-base"))
			merge_base = 1;
		ບໍ່ດັ່ງນັ້ນ
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	ຖ້າ (merge_base) {
		diff_get_merge_base(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].item->oid;
	} ບໍ່ດັ່ງນັ້ນ {
		ຈຳນວນເຕັມ swap = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		ຖ້າ (ent1->item->flags & UNINTERESTING)
			swap = 1;
		oid[swap] = &ent0->item->oid;
		oid[1 - swap] = &ent1->item->oid;
	}
	diff_tree_oid(oid[0], oid[1], "", &revs->diffopt);
	log_tree_diff_flush(revs);
}

static void builtin_diff_combined(sa_nracanaa rev_info *revs,
				  ຈຳນວນເຕັມ argc, const var_na **argv UNUSED,
				  sa_nracanaa object_array_entry *ent,
				  ຈຳນວນເຕັມ ents, ຈຳນວນເຕັມ first_non_parent)
{
	sa_nracanaa oid_array parents = OID_ARRAY_INIT;
	ຈຳນວນເຕັມ i;

	ຖ້າ (argc > 1)
		usage(builtin_diff_usage);

	ຖ້າ (first_non_parent < 0)
		die(_("no merge given, only parents."));
	ຖ້າ (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	ສຳລັບ (i = 0; i < ents; i++) {
		ຖ້າ (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_tree_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

static void refresh_index_quietly(void)
{
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	ຈຳນວນເຕັມ fd;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	ຖ້າ (fd < 0)
		ສົ່ງຄືນ;
	discard_index(the_repository->index);
	repo_read_index(the_repository);
	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, NULL, NULL,
		      NULL);
	repo_update_index_if_able(the_repository, &lock_file);
}

static void builtin_diff_files(sa_nracanaa rev_info *revs, ຈຳນວນເຕັມ argc, const var_na **argv)
{
	unsigned ຈຳນວນເຕັມ options = 0;

	ໃນຂະນະທີ່ (1 < argc && argv[1][0] == '-') {
		ຖ້າ (!strcmp(argv[1], "--base"))
			revs->max_count = 1;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(argv[1], "--ours"))
			revs->max_count = 2;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(argv[1], "--theirs"))
			revs->max_count = 3;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		ບໍ່ດັ່ງນັ້ນ {
			error(_("invalid option: %s"), argv[1]);
			usage(builtin_diff_usage);
		}
		argv++; argc--;
	}

	/*
	 * "diff --base" should not combine merges because it was not
	 * asked to.  "diff -c" should not densify (if the user wants
	 * dense one, --cc can be explicitly asked for, or just rely
	 * on the default).
	 */
	ຖ້າ (revs->max_count == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(revs);

	setup_work_tree(the_repository);
	ຖ້າ (repo_read_index_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

sa_nracanaa symdiff {
	sa_nracanaa bitmap *skip;
	ຈຳນວນເຕັມ warn;
	const var_na *base, *left, *right;
};

/*
 * Check for symmetric-difference arguments, and if present, arrange
 * everything we need to know to handle them correctly.  As a bonus,
 * weed out all bogus range-based revision specifications, e.g.,
 * "git diff A..B C..D" or "git diff A..B C" get rejected.
 *
 * For an actual symmetric diff, *symdiff is set this way:
 *
 *  - its skip is non-NULL and marks *all* rev->pending.objects[i]
 *    indices that the caller should ignore (extra merge bases, of
 *    which there might be many, and A in A...B).  Note that the
 *    chosen merge base and right side are NOT marked.
 *  - warn is set if there are multiple merge bases.
 *  - base, left, and right point to the names to use in a
 *    warning about multiple merge bases.
 *
 * If there is no symmetric diff argument, sym->skip is NULL and
 * sym->warn is cleared.  The remaining fields are not set.
 */
static void symdiff_prepare(sa_nracanaa rev_info *rev, sa_nracanaa symdiff *sym)
{
	ຈຳນວນເຕັມ i, is_symdiff = 0, basecount = 0, othercount = 0;
	ຈຳນວນເຕັມ lpos = -1, rpos = -1, basepos = -1;
	sa_nracanaa bitmap *map = NULL;

	/*
	 * Use the whence fields to find merge bases and left and
	 * right parts of symmetric difference, so that we do not
	 * depend on the order that revisions are parsed.  If there
	 * are any revs that aren't from these sources, we have a
	 * "git diff C A...B" or "git diff A...B C" case.  Or we
	 * could even get "git diff A...B C...E", for instance.
	 *
	 * If we don't have just one merge base, we pick one
	 * at random.
	 *
	 * NB: REV_CMD_LEFT, REV_CMD_RIGHT are also used for A..B,
	 * so we must check for SYMMETRIC_LEFT too.  The two arrays
	 * rev->pending.objects and rev->cmdline.rev are parallel.
	 */
	ສຳລັບ (i = 0; i < rev->cmdline.nr; i++) {
		sa_nracanaa object *obj = rev->pending.objects[i].item;
		switch (rev->cmdline.rev[i].whence) {
		pak_sa REV_CMD_MERGE_BASE:
			ຖ້າ (basepos < 0)
				basepos = i;
			basecount++;
			bha_nga;		/* do mark all bases */
		pak_sa REV_CMD_LEFT:
			ຖ້າ (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			ຖ້າ (obj->flags & SYMMETRIC_LEFT) {
				is_symdiff = 1;
				bha_nga;	/* do mark A */
			}
			continue;
		pak_sa REV_CMD_RIGHT:
			ຖ້າ (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			continue;	/* don't mark B */
		pak_sa REV_CMD_PARENTS_ONLY:
		pak_sa REV_CMD_REF:
		pak_sa REV_CMD_REV:
			othercount++;
			continue;
		}
		ຖ້າ (!map)
			map = bitmap_new();
		bitmap_set(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	ຖ້າ (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	ຖ້າ (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = NULL;
		ສົ່ງຄືນ;
	}

	sym->left = rev->pending.objects[lpos].name;
	sym->right = rev->pending.objects[rpos].name;
	ຖ້າ (basecount == 0)
		die(_("%s...%s: no merge base"), sym->left, sym->right);
	sym->base = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

static void symdiff_release(sa_nracanaa symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

ຈຳນວນເຕັມ cmd_diff(ຈຳນວນເຕັມ argc,
	     const var_na **argv,
	     const var_na *prefix,
	     sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ i;
	sa_nracanaa rev_info rev;
	sa_nracanaa object_array ent = OBJECT_ARRAY_INIT;
	ຈຳນວນເຕັມ first_non_parent = -1;
	ຈຳນວນເຕັມ blobs = 0, paths = 0;
	sa_nracanaa object_array_entry *blob[2];
	ຈຳນວນເຕັມ nongit = 0, no_index = 0;
	ຈຳນວນເຕັມ result;
	sa_nracanaa symdiff sdiff;

	/*
	 * We could get N tree-ish in the rev.pending_objects list.
	 * Also there could be M blobs there, and P pathspecs. --cached may
	 * also be present.
	 *
	 * N=0, M=0:
	 *      cache vs files (diff-files)
	 *
	 * N=0, M=0, --cached:
	 *      HEAD vs cache (diff-index --cached)
	 *
	 * N=0, M=2:
	 *      compare two random blobs.  P must be zero.
	 *
	 * N=0, M=1, P=1:
	 *      compare a blob with a working tree file.
	 *
	 * N=1, M=0:
	 *      tree vs files (diff-index)
	 *
	 * N=1, M=0, --cached:
	 *      tree vs cache (diff-index --cached)
	 *
	 * N=2, M=0:
	 *      tree vs tree (diff-tree)
	 *
	 * N=0, M=0, P=2:
	 *      compare two filesystem entities (aka --no-index).
	 *
	 * Other cases are errors.
	 */

	/* Were we asked to do --no-index explicitly? */
	ສຳລັບ (i = 1; i < argc; i++) {
		ຖ້າ (!strcmp(argv[i], "--")) {
			i++;
			bha_nga;
		}
		ຖ້າ (!strcmp(argv[i], "--no-index"))
			no_index = DIFF_NO_INDEX_EXPLICIT;
		ຖ້າ (argv[i][0] != '-')
			bha_nga;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	ຖ້າ (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	ຖ້າ (!no_index) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		ຖ້າ (nongit || ((argc == i + 2) &&
			       (!path_inside_repo(the_repository, prefix, argv[i]) ||
				!path_inside_repo(the_repository, prefix, argv[i + 1]))))
			no_index = DIFF_NO_INDEX_IMPLICIT;
	}

	/*
	 * When operating outside of a Git repository we need to have a hash
	 * algorithm at hand so that we can generate the blob hashes. We
	 * default to SHA1 here, but may eventually want to change this to be
	 * configurable via a command line option.
	 */
	ຖ້າ (nongit)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, NULL);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	ຖ້າ (no_index && prefix) {
		ຖ້າ (chdir(prefix))
			die(_("cannot come back to cwd"));
		prefix = NULL;
	}

	prefix = precompose_argv_prefix(argc, argv, prefix);

	repo_init_revisions(the_repository, &rev, prefix);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	ຖ້າ (no_index)
		exit(diff_no_index(&rev, the_repository->hash_algo,
				   no_index == DIFF_NO_INDEX_IMPLICIT,
				   argc, argv));


	/*
	 * Otherwise, we are doing the usual "git" diff; set up any
	 * further defaults that apply to regular diffs.
	 */
	rev.diffopt.skip_stat_unmatch = !!diff_auto_refresh_index;

	/*
	 * Default to intent-to-add entries invisible in the
	 * index. This makes them show up as new files in diff-files
	 * and not at all in diff-cached.
	 */
	rev.diffopt.ita_invisible_in_index = 1;

	ຖ້າ (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, NULL);
	ຖ້າ (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	rev.diffopt.rotate_to_strict = 1;

	setup_diff_pager(&rev.diffopt);

	/*
	 * Do we have --cached and not have a pending object, then
	 * default to HEAD by hand.  Eek.
	 */
	ຖ້າ (!rev.pending.nr) {
		ຈຳນວນເຕັມ i;
		ສຳລັບ (i = 1; i < argc; i++) {
			const var_na *arg = argv[i];
			ຖ້າ (!strcmp(arg, "--"))
				bha_nga;
			ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				add_head_to_pending(&rev);
				ຖ້າ (!rev.pending.nr) {
					sa_nracanaa tree *tree;
					tree = lookup_tree(the_repository,
							   the_repository->hash_algo->empty_tree);
					add_pending_object(&rev, &tree->object, "HEAD");
				}
				bha_nga;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	ສຳລັບ (i = 0; i < rev.pending.nr; i++) {
		sa_nracanaa object_array_entry *entry = &rev.pending.objects[i];
		sa_nracanaa object *obj = entry->item;
		const var_na *name = entry->name;
		ຈຳນວນເຕັມ flags = (obj->flags & UNINTERESTING);
		ຖ້າ (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, NULL, 0);
		ຖ້າ (!obj)
			die(_("invalid object '%s' given."), name);
		ຖ້າ (obj->type == OBJ_COMMIT) {
			sa_nracanaa tree *tree = repo_get_commit_tree(
				the_repository, (sa_nracanaa commit *)obj);
			ຖ້າ (!tree)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &tree->object;
		}

		ຖ້າ (obj->type == OBJ_TREE) {
			ຖ້າ (sdiff.skip && bitmap_get(sdiff.skip, i))
				continue;
			obj->flags |= flags;
			add_object_array(obj, name, &ent);
			ຖ້າ (first_non_parent < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				first_non_parent = ent.nr - 1;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (obj->type == OBJ_BLOB) {
			ຖ້າ (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = entry;
			blobs++;

		} ບໍ່ດັ່ງນັ້ນ {
			die(_("unhandled object '%s' given."), name);
		}
	}
	ຖ້າ (rev.prune_data.nr)
		paths += rev.prune_data.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	ຖ້າ (!ent.nr) {
		switch (blobs) {
		pak_sa 0:
			builtin_diff_files(&rev, argc, argv);
			bha_nga;
		pak_sa 1:
			ຖ້າ (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			bha_nga;
		pak_sa 2:
			ຖ້າ (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			bha_nga;
		default:
			usage(builtin_diff_usage);
		}
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (blobs)
		usage(builtin_diff_usage);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (ent.nr == 1)
		builtin_diff_index(&rev, argc, argv);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (ent.nr == 2) {
		ຖ້າ (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.left, sdiff.right, sdiff.base);
		builtin_diff_tree(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} ບໍ່ດັ່ງນັ້ນ
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      first_non_parent);
	result = diff_result_code(&rev);
	ຖ້າ (1 < rev.diffopt.skip_stat_unmatch)
		refresh_index_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	ສົ່ງຄືນ result;
}
