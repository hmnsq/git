/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "ewah/ewok.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "color.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "diff-merges.h"
#ಅನ್ತರ್ಭಾವ "diffcore.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "log-tree.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "tree.h"

#define DIFF_NO_INDEX_EXPLICIT 1
#define DIFF_NO_INDEX_IMPLICIT 2

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *blob_path(ಸಂರಚನಾ object_array_entry *entry)
{
	ಪ್ರತ್ಯಾಗಮ entry->path ? entry->path : entry->name;
}

ಸ್ಥಿರ ಶೂನ್ಯ stuff_change(ಸಂರಚನಾ diff_options *opt,
			 unsigned old_mode, unsigned new_mode,
			 ಧ್ರುವ ಸಂರಚನಾ object_id *old_oid,
			 ಧ್ರುವ ಸಂರಚನಾ object_id *new_oid,
			 ಪೂರ್ಣಾಙ್ಕ old_oid_valid,
			 ಪೂರ್ಣಾಙ್ಕ new_oid_valid,
			 ಧ್ರುವ ವರ್ಣ *old_path,
			 ಧ್ರುವ ವರ್ಣ *new_path)
{
	ಸಂರಚನಾ diff_filespec *one, *two;

	ಯದಿ (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	ಯದಿ (opt->prefix &&
	    (strncmp(old_path, opt->prefix, opt->prefix_length) ||
	     strncmp(new_path, opt->prefix, opt->prefix_length)))
		ಪ್ರತ್ಯಾಗಮ;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_b_f(ಸಂರಚನಾ rev_info *revs,
			     ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv UNUSED,
			     ಸಂರಚನಾ object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	ಸಂರಚನಾ stat st;
	ಧ್ರುವ ವರ್ಣ *path;

	ಯದಿ (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_data, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_data.ವಸ್ತುಗಳು[0].match;

	ಯದಿ (lstat(path, &st))
		die_errno(_("failed to stat '%s'"), path);
	ಯದಿ (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
		die(_("'%s': not a regular file or symlink"), path);

	diff_set_mnemonic_prefix(&revs->diffopt, "o/", "w/");

	ಯದಿ (blob[0]->mode == S_IFINVALID)
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

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_blobs(ಸಂರಚನಾ rev_info *revs,
			       ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv UNUSED,
			       ಸಂರಚನಾ object_array_entry **blob)
{
	ಧ್ರುವ unsigned mode = canon_mode(S_IFREG | 0644);

	ಯದಿ (argc > 1)
		usage(builtin_diff_usage);

	ಯದಿ (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = mode;

	ಯದಿ (blob[1]->mode == S_IFINVALID)
		blob[1]->mode = mode;

	stuff_change(&revs->diffopt,
		     blob[0]->mode, blob[1]->mode,
		     &blob[0]->item->oid, &blob[1]->item->oid,
		     1, 1,
		     blob_path(blob[0]), blob_path(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_index(ಸಂರಚನಾ rev_info *revs,
			       ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ option = 0;
	ಯಾವತ್ (1 < argc) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[1];
		ಯದಿ (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_INDEX_CACHED;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		ಅನ್ಯಥಾ
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	ಯದಿ (revs->pending.nr != 1 ||
	    revs->max_ಎಣಿಕೆ != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	ಯದಿ (!(option & DIFF_INDEX_CACHED)) {
		setup_work_ವೃಕ್ಷ(the_repository);
		ಯದಿ (repo_read_index_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} ಅನ್ಯಥಾ ಯದಿ (repo_read_index(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_index(revs, option);
}

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_ವೃಕ್ಷ(ಸಂರಚನಾ rev_info *revs,
			      ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
			      ಸಂರಚನಾ object_array_entry *ent0,
			      ಸಂರಚನಾ object_array_entry *ent1)
{
	ಧ್ರುವ ಸಂರಚನಾ object_id *(oid[2]);
	ಸಂರಚನಾ object_id mb_oid;
	ಪೂರ್ಣಾಙ್ಕ merge_base = 0;

	ಯಾವತ್ (1 < argc) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[1];
		ಯದಿ (!strcmp(arg, "--merge-base"))
			merge_base = 1;
		ಅನ್ಯಥಾ
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	ಯದಿ (merge_base) {
		diff_get_merge_base(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].item->oid;
	} ಅನ್ಯಥಾ {
		ಪೂರ್ಣಾಙ್ಕ swap = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		ಯದಿ (ent1->item->flags & UNINTERESTING)
			swap = 1;
		oid[swap] = &ent0->item->oid;
		oid[1 - swap] = &ent1->item->oid;
	}
	diff_ವೃಕ್ಷ_oid(oid[0], oid[1], "", &revs->diffopt);
	log_ವೃಕ್ಷ_diff_flush(revs);
}

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_combined(ಸಂರಚನಾ rev_info *revs,
				  ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv UNUSED,
				  ಸಂರಚನಾ object_array_entry *ent,
				  ಪೂರ್ಣಾಙ್ಕ ents, ಪೂರ್ಣಾಙ್ಕ first_non_parent)
{
	ಸಂರಚನಾ oid_array parents = OID_ARRAY_INIT;
	ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (argc > 1)
		usage(builtin_diff_usage);

	ಯದಿ (first_non_parent < 0)
		die(_("no merge given, only parents."));
	ಯದಿ (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	ಪ್ರತಿ (i = 0; i < ents; i++) {
		ಯದಿ (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_ವೃಕ್ಷ_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

ಸ್ಥಿರ ಶೂನ್ಯ refresh_index_quietly(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಪೂರ್ಣಾಙ್ಕ fd;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ;
	discard_index(the_repository->index);
	repo_read_index(the_repository);
	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, ರಿಕ್ತ, ರಿಕ್ತ,
		      ರಿಕ್ತ);
	repo_update_index_if_able(the_repository, &lock_file);
}

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_files(ಸಂರಚನಾ rev_info *revs, ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ options = 0;

	ಯಾವತ್ (1 < argc && argv[1][0] == '-') {
		ಯದಿ (!strcmp(argv[1], "--base"))
			revs->max_ಎಣಿಕೆ = 1;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(argv[1], "--ours"))
			revs->max_ಎಣಿಕೆ = 2;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(argv[1], "--theirs"))
			revs->max_ಎಣಿಕೆ = 3;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		ಅನ್ಯಥಾ {
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
	ಯದಿ (revs->max_ಎಣಿಕೆ == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(revs);

	setup_work_ವೃಕ್ಷ(the_repository);
	ಯದಿ (repo_read_index_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

ಸಂರಚನಾ symdiff {
	ಸಂರಚನಾ bitmap *skip;
	ಪೂರ್ಣಾಙ್ಕ warn;
	ಧ್ರುವ ವರ್ಣ *base, *ಎಡ, *ಬಲ;
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
ಸ್ಥಿರ ಶೂನ್ಯ symdiff_prepare(ಸಂರಚನಾ rev_info *rev, ಸಂರಚನಾ symdiff *sym)
{
	ಪೂರ್ಣಾಙ್ಕ i, is_symdiff = 0, basecount = 0, othercount = 0;
	ಪೂರ್ಣಾಙ್ಕ lpos = -1, rpos = -1, basepos = -1;
	ಸಂರಚನಾ bitmap *map = ರಿಕ್ತ;

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
	ಪ್ರತಿ (i = 0; i < rev->cmdline.nr; i++) {
		ಸಂರಚನಾ object *obj = rev->pending.objects[i].item;
		ವಿಕಲ್ಪ (rev->cmdline.rev[i].whence) {
		ಪಕ್ಷ REV_CMD_MERGE_BASE:
			ಯದಿ (basepos < 0)
				basepos = i;
			basecount++;
			ಭಙ್ಗ;		/* do mark all bases */
		ಪಕ್ಷ REV_CMD_ಎಡ:
			ಯದಿ (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			ಯದಿ (obj->flags & SYMMETRIC_ಎಡ) {
				is_symdiff = 1;
				ಭಙ್ಗ;	/* do mark A */
			}
			ಅನುವರ್ತ;
		ಪಕ್ಷ REV_CMD_ಬಲ:
			ಯದಿ (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			ಅನುವರ್ತ;	/* don't mark B */
		ಪಕ್ಷ REV_CMD_PARENTS_ONLY:
		ಪಕ್ಷ REV_CMD_REF:
		ಪಕ್ಷ REV_CMD_REV:
			othercount++;
			ಅನುವರ್ತ;
		}
		ಯದಿ (!map)
			map = bitmap_new();
		bitmap_set(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	ಯದಿ (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	ಯದಿ (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = ರಿಕ್ತ;
		ಪ್ರತ್ಯಾಗಮ;
	}

	sym->ಎಡ = rev->pending.objects[lpos].name;
	sym->ಬಲ = rev->pending.objects[rpos].name;
	ಯದಿ (basecount == 0)
		die(_("%s...%s: no merge base"), sym->ಎಡ, sym->ಬಲ);
	sym->base = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

ಸ್ಥಿರ ಶೂನ್ಯ symdiff_release(ಸಂರಚನಾ symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

ಪೂರ್ಣಾಙ್ಕ cmd_diff(ಪೂರ್ಣಾಙ್ಕ argc,
	     ಧ್ರುವ ವರ್ಣ **argv,
	     ಧ್ರುವ ವರ್ಣ *prefix,
	     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ rev_info rev;
	ಸಂರಚನಾ object_array ent = OBJECT_ARRAY_INIT;
	ಪೂರ್ಣಾಙ್ಕ first_non_parent = -1;
	ಪೂರ್ಣಾಙ್ಕ blobs = 0, paths = 0;
	ಸಂರಚನಾ object_array_entry *blob[2];
	ಪೂರ್ಣಾಙ್ಕ nongit = 0, no_index = 0;
	ಪೂರ್ಣಾಙ್ಕ result;
	ಸಂರಚನಾ symdiff sdiff;

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
	ಪ್ರತಿ (i = 1; i < argc; i++) {
		ಯದಿ (!strcmp(argv[i], "--")) {
			i++;
			ಭಙ್ಗ;
		}
		ಯದಿ (!strcmp(argv[i], "--no-index"))
			no_index = DIFF_NO_INDEX_EXPLICIT;
		ಯದಿ (argv[i][0] != '-')
			ಭಙ್ಗ;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	ಯದಿ (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	ಯದಿ (!no_index) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		ಯದಿ (nongit || ((argc == i + 2) &&
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
	ಯದಿ (nongit)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ರಿಕ್ತ);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	ಯದಿ (no_index && prefix) {
		ಯದಿ (chdir(prefix))
			die(_("cannot come back to cwd"));
		prefix = ರಿಕ್ತ;
	}

	prefix = precompose_argv_prefix(argc, argv, prefix);

	repo_init_revisions(the_repository, &rev, prefix);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	ಯದಿ (no_index)
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

	ಯದಿ (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, ರಿಕ್ತ);
	ಯದಿ (!rev.diffopt.output_format) {
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
	ಯದಿ (!rev.pending.nr) {
		ಪೂರ್ಣಾಙ್ಕ i;
		ಪ್ರತಿ (i = 1; i < argc; i++) {
			ಧ್ರುವ ವರ್ಣ *arg = argv[i];
			ಯದಿ (!strcmp(arg, "--"))
				ಭಙ್ಗ;
			ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				add_head_to_pending(&rev);
				ಯದಿ (!rev.pending.nr) {
					ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ;
					ವೃಕ್ಷ = lookup_ವೃಕ್ಷ(the_repository,
							   the_repository->hash_algo->empty_ವೃಕ್ಷ);
					add_pending_object(&rev, &ವೃಕ್ಷ->object, "HEAD");
				}
				ಭಙ್ಗ;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	ಪ್ರತಿ (i = 0; i < rev.pending.nr; i++) {
		ಸಂರಚನಾ object_array_entry *entry = &rev.pending.objects[i];
		ಸಂರಚನಾ object *obj = entry->item;
		ಧ್ರುವ ವರ್ಣ *name = entry->name;
		ಪೂರ್ಣಾಙ್ಕ flags = (obj->flags & UNINTERESTING);
		ಯದಿ (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, ರಿಕ್ತ, 0);
		ಯದಿ (!obj)
			die(_("invalid object '%s' given."), name);
		ಯದಿ (obj->type == OBJ_COMMIT) {
			ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(
				the_repository, (ಸಂರಚನಾ commit *)obj);
			ಯದಿ (!ವೃಕ್ಷ)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &ವೃಕ್ಷ->object;
		}

		ಯದಿ (obj->type == OBJ_ವೃಕ್ಷ) {
			ಯದಿ (sdiff.skip && bitmap_get(sdiff.skip, i))
				ಅನುವರ್ತ;
			obj->flags |= flags;
			add_object_array(obj, name, &ent);
			ಯದಿ (first_non_parent < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				first_non_parent = ent.nr - 1;
		} ಅನ್ಯಥಾ ಯದಿ (obj->type == OBJ_BLOB) {
			ಯದಿ (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = entry;
			blobs++;

		} ಅನ್ಯಥಾ {
			die(_("unhandled object '%s' given."), name);
		}
	}
	ಯದಿ (rev.prune_data.nr)
		paths += rev.prune_data.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	ಯದಿ (!ent.nr) {
		ವಿಕಲ್ಪ (blobs) {
		ಪಕ್ಷ 0:
			builtin_diff_files(&rev, argc, argv);
			ಭಙ್ಗ;
		ಪಕ್ಷ 1:
			ಯದಿ (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			ಭಙ್ಗ;
		ಪಕ್ಷ 2:
			ಯದಿ (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			usage(builtin_diff_usage);
		}
	}
	ಅನ್ಯಥಾ ಯದಿ (blobs)
		usage(builtin_diff_usage);
	ಅನ್ಯಥಾ ಯದಿ (ent.nr == 1)
		builtin_diff_index(&rev, argc, argv);
	ಅನ್ಯಥಾ ಯದಿ (ent.nr == 2) {
		ಯದಿ (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.ಎಡ, sdiff.ಬಲ, sdiff.base);
		builtin_diff_ವೃಕ್ಷ(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} ಅನ್ಯಥಾ
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      first_non_parent);
	result = diff_result_code(&rev);
	ಯದಿ (1 < rev.diffopt.skip_stat_unmatch)
		refresh_index_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	ಪ್ರತ್ಯಾಗಮ result;
}
