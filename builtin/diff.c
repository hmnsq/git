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
	ပြန်ပေး entry->path ? entry->path : entry->name;
}

static void stuff_change(sa_nracanaa diff_options *opt,
			 unsigned old_mode, unsigned new_mode,
			 const sa_nracanaa object_id *old_oid,
			 const sa_nracanaa object_id *new_oid,
			 ကိန်းပြည့် old_oid_valid,
			 ကိန်းပြည့် new_oid_valid,
			 const var_na *old_path,
			 const var_na *new_path)
{
	sa_nracanaa diff_filespec *one, *two;

	အကယ်၍ (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		ပြန်ပေး;

	အကယ်၍ (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	အကယ်၍ (opt->prefix &&
	    (strncmp(old_path, opt->prefix, opt->prefix_length) ||
	     strncmp(new_path, opt->prefix, opt->prefix_length)))
		ပြန်ပေး;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

static void builtin_diff_b_f(sa_nracanaa rev_info *revs,
			     ကိန်းပြည့် argc, const var_na **argv UNUSED,
			     sa_nracanaa object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	sa_nracanaa stat st;
	const var_na *path;

	အကယ်၍ (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_data, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_data.items[0].match;

	အကယ်၍ (lstat(path, &st))
		die_errno(_("failed to stat '%s'"), path);
	အကယ်၍ (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
		die(_("'%s': not a regular file or symlink"), path);

	diff_set_mnemonic_prefix(&revs->diffopt, "o/", "w/");

	အကယ်၍ (blob[0]->mode == S_IFINVALID)
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
			       ကိန်းပြည့် argc, const var_na **argv UNUSED,
			       sa_nracanaa object_array_entry **blob)
{
	const unsigned mode = canon_mode(S_IFREG | 0644);

	အကယ်၍ (argc > 1)
		usage(builtin_diff_usage);

	အကယ်၍ (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = mode;

	အကယ်၍ (blob[1]->mode == S_IFINVALID)
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
			       ကိန်းပြည့် argc, const var_na **argv)
{
	unsigned ကိန်းပြည့် option = 0;
	နေစဉ် (1 < argc) {
		const var_na *arg = argv[1];
		အကယ်၍ (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_INDEX_CACHED;
		မဟုတ်ပါက အကယ်၍ (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		မဟုတ်ပါက
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	အကယ်၍ (revs->pending.nr != 1 ||
	    revs->max_count != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	အကယ်၍ (!(option & DIFF_INDEX_CACHED)) {
		setup_work_tree(the_repository);
		အကယ်၍ (repo_read_index_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} မဟုတ်ပါက အကယ်၍ (repo_read_index(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_index(revs, option);
}

static void builtin_diff_tree(sa_nracanaa rev_info *revs,
			      ကိန်းပြည့် argc, const var_na **argv,
			      sa_nracanaa object_array_entry *ent0,
			      sa_nracanaa object_array_entry *ent1)
{
	const sa_nracanaa object_id *(oid[2]);
	sa_nracanaa object_id mb_oid;
	ကိန်းပြည့် merge_base = 0;

	နေစဉ် (1 < argc) {
		const var_na *arg = argv[1];
		အကယ်၍ (!strcmp(arg, "--merge-base"))
			merge_base = 1;
		မဟုတ်ပါက
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	အကယ်၍ (merge_base) {
		diff_get_merge_base(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].item->oid;
	} မဟုတ်ပါက {
		ကိန်းပြည့် swap = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		အကယ်၍ (ent1->item->flags & UNINTERESTING)
			swap = 1;
		oid[swap] = &ent0->item->oid;
		oid[1 - swap] = &ent1->item->oid;
	}
	diff_tree_oid(oid[0], oid[1], "", &revs->diffopt);
	log_tree_diff_flush(revs);
}

static void builtin_diff_combined(sa_nracanaa rev_info *revs,
				  ကိန်းပြည့် argc, const var_na **argv UNUSED,
				  sa_nracanaa object_array_entry *ent,
				  ကိန်းပြည့် ents, ကိန်းပြည့် first_non_parent)
{
	sa_nracanaa oid_array parents = OID_ARRAY_INIT;
	ကိန်းပြည့် i;

	အကယ်၍ (argc > 1)
		usage(builtin_diff_usage);

	အကယ်၍ (first_non_parent < 0)
		die(_("no merge given, only parents."));
	အကယ်၍ (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	အတွက် (i = 0; i < ents; i++) {
		အကယ်၍ (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_tree_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

static void refresh_index_quietly(void)
{
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	ကိန်းပြည့် fd;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	အကယ်၍ (fd < 0)
		ပြန်ပေး;
	discard_index(the_repository->index);
	repo_read_index(the_repository);
	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, NULL, NULL,
		      NULL);
	repo_update_index_if_able(the_repository, &lock_file);
}

static void builtin_diff_files(sa_nracanaa rev_info *revs, ကိန်းပြည့် argc, const var_na **argv)
{
	unsigned ကိန်းပြည့် options = 0;

	နေစဉ် (1 < argc && argv[1][0] == '-') {
		အကယ်၍ (!strcmp(argv[1], "--base"))
			revs->max_count = 1;
		မဟုတ်ပါက အကယ်၍ (!strcmp(argv[1], "--ours"))
			revs->max_count = 2;
		မဟုတ်ပါက အကယ်၍ (!strcmp(argv[1], "--theirs"))
			revs->max_count = 3;
		မဟုတ်ပါက အကယ်၍ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		မဟုတ်ပါက အကယ်၍ (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		မဟုတ်ပါက {
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
	အကယ်၍ (revs->max_count == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(revs);

	setup_work_tree(the_repository);
	အကယ်၍ (repo_read_index_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

sa_nracanaa symdiff {
	sa_nracanaa bitmap *skip;
	ကိန်းပြည့် warn;
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
	ကိန်းပြည့် i, is_symdiff = 0, basecount = 0, othercount = 0;
	ကိန်းပြည့် lpos = -1, rpos = -1, basepos = -1;
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
	အတွက် (i = 0; i < rev->cmdline.nr; i++) {
		sa_nracanaa object *obj = rev->pending.objects[i].item;
		switch (rev->cmdline.rev[i].whence) {
		pak_sa REV_CMD_MERGE_BASE:
			အကယ်၍ (basepos < 0)
				basepos = i;
			basecount++;
			bha_nga;		/* do mark all bases */
		pak_sa REV_CMD_LEFT:
			အကယ်၍ (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			အကယ်၍ (obj->flags & SYMMETRIC_LEFT) {
				is_symdiff = 1;
				bha_nga;	/* do mark A */
			}
			continue;
		pak_sa REV_CMD_RIGHT:
			အကယ်၍ (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			continue;	/* don't mark B */
		pak_sa REV_CMD_PARENTS_ONLY:
		pak_sa REV_CMD_REF:
		pak_sa REV_CMD_REV:
			othercount++;
			continue;
		}
		အကယ်၍ (!map)
			map = bitmap_new();
		bitmap_set(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	အကယ်၍ (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	အကယ်၍ (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = NULL;
		ပြန်ပေး;
	}

	sym->left = rev->pending.objects[lpos].name;
	sym->right = rev->pending.objects[rpos].name;
	အကယ်၍ (basecount == 0)
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

ကိန်းပြည့် cmd_diff(ကိန်းပြည့် argc,
	     const var_na **argv,
	     const var_na *prefix,
	     sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် i;
	sa_nracanaa rev_info rev;
	sa_nracanaa object_array ent = OBJECT_ARRAY_INIT;
	ကိန်းပြည့် first_non_parent = -1;
	ကိန်းပြည့် blobs = 0, paths = 0;
	sa_nracanaa object_array_entry *blob[2];
	ကိန်းပြည့် nongit = 0, no_index = 0;
	ကိန်းပြည့် result;
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
	အတွက် (i = 1; i < argc; i++) {
		အကယ်၍ (!strcmp(argv[i], "--")) {
			i++;
			bha_nga;
		}
		အကယ်၍ (!strcmp(argv[i], "--no-index"))
			no_index = DIFF_NO_INDEX_EXPLICIT;
		အကယ်၍ (argv[i][0] != '-')
			bha_nga;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	အကယ်၍ (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	အကယ်၍ (!no_index) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		အကယ်၍ (nongit || ((argc == i + 2) &&
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
	အကယ်၍ (nongit)
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
	အကယ်၍ (no_index && prefix) {
		အကယ်၍ (chdir(prefix))
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
	အကယ်၍ (no_index)
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

	အကယ်၍ (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, NULL);
	အကယ်၍ (!rev.diffopt.output_format) {
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
	အကယ်၍ (!rev.pending.nr) {
		ကိန်းပြည့် i;
		အတွက် (i = 1; i < argc; i++) {
			const var_na *arg = argv[i];
			အကယ်၍ (!strcmp(arg, "--"))
				bha_nga;
			မဟုတ်ပါက အကယ်၍ (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				add_head_to_pending(&rev);
				အကယ်၍ (!rev.pending.nr) {
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
	အတွက် (i = 0; i < rev.pending.nr; i++) {
		sa_nracanaa object_array_entry *entry = &rev.pending.objects[i];
		sa_nracanaa object *obj = entry->item;
		const var_na *name = entry->name;
		ကိန်းပြည့် flags = (obj->flags & UNINTERESTING);
		အကယ်၍ (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, NULL, 0);
		အကယ်၍ (!obj)
			die(_("invalid object '%s' given."), name);
		အကယ်၍ (obj->type == OBJ_COMMIT) {
			sa_nracanaa tree *tree = repo_get_commit_tree(
				the_repository, (sa_nracanaa commit *)obj);
			အကယ်၍ (!tree)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &tree->object;
		}

		အကယ်၍ (obj->type == OBJ_TREE) {
			အကယ်၍ (sdiff.skip && bitmap_get(sdiff.skip, i))
				continue;
			obj->flags |= flags;
			add_object_array(obj, name, &ent);
			အကယ်၍ (first_non_parent < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				first_non_parent = ent.nr - 1;
		} မဟုတ်ပါက အကယ်၍ (obj->type == OBJ_BLOB) {
			အကယ်၍ (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = entry;
			blobs++;

		} မဟုတ်ပါက {
			die(_("unhandled object '%s' given."), name);
		}
	}
	အကယ်၍ (rev.prune_data.nr)
		paths += rev.prune_data.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	အကယ်၍ (!ent.nr) {
		switch (blobs) {
		pak_sa 0:
			builtin_diff_files(&rev, argc, argv);
			bha_nga;
		pak_sa 1:
			အကယ်၍ (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			bha_nga;
		pak_sa 2:
			အကယ်၍ (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			bha_nga;
		default:
			usage(builtin_diff_usage);
		}
	}
	မဟုတ်ပါက အကယ်၍ (blobs)
		usage(builtin_diff_usage);
	မဟုတ်ပါက အကယ်၍ (ent.nr == 1)
		builtin_diff_index(&rev, argc, argv);
	မဟုတ်ပါက အကယ်၍ (ent.nr == 2) {
		အကယ်၍ (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.left, sdiff.right, sdiff.base);
		builtin_diff_tree(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} မဟုတ်ပါက
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      first_non_parent);
	result = diff_result_code(&rev);
	အကယ်၍ (1 < rev.diffopt.skip_stat_unmatch)
		refresh_index_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	ပြန်ပေး result;
}
