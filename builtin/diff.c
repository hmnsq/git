/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

#ವ್ಯಾಖ್ಯಾನಿಸು DIFF_NO_ಸೂಚ್ಯಂಕ_EXPLICIT 1
#ವ್ಯಾಖ್ಯಾನಿಸು DIFF_NO_ಸೂಚ್ಯಂಕ_IMPLICIT 2

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *blob_ಮಾರ್ಗ(ರಚನೆ object_array_ನಮೂದು *ನಮೂದು)
{
	ಹಿಂತಿರುಗಿ ನಮೂದು->ಮಾರ್ಗ ? ನಮೂದು->ಮಾರ್ಗ : ನಮೂದು->name;
}

ಸ್ಥಿರ ಶೂನ್ಯ stuff_change(ರಚನೆ diff_options *opt,
			 ಚಿಹ್ನೆರಹಿತ old_ಬಹುಲಕ, ಚಿಹ್ನೆರಹಿತ new_ಬಹುಲಕ,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_oid,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *new_oid,
			 ಪೂರ್ಣಾಂಕ old_oid_valid,
			 ಪೂರ್ಣಾಂಕ new_oid_valid,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_ಮಾರ್ಗ,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_ಮಾರ್ಗ)
{
	ರಚನೆ diff_filespec *one, *two;

	ಆದರೆ (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_ಬಹುಲಕ == new_ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (opt->flags.ಹಿಮ್ಮುಖ_diff) {
		ಅದಲುಬದಲು(old_ಬಹುಲಕ, new_ಬಹುಲಕ);
		ಅದಲುಬದಲು(old_oid, new_oid);
		ಅದಲುಬದಲು(old_ಮಾರ್ಗ, new_ಮಾರ್ಗ);
	}

	ಆದರೆ (opt->ಪೂರ್ವಪ್ರತ್ಯಯ &&
	    (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(old_ಮಾರ್ಗ, opt->ಪೂರ್ವಪ್ರತ್ಯಯ, opt->ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ) ||
	     ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(new_ಮಾರ್ಗ, opt->ಪೂರ್ವಪ್ರತ್ಯಯ, opt->ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ)))
		ಹಿಂತಿರುಗಿ;

	one = alloc_filespec(old_ಮಾರ್ಗ);
	two = alloc_filespec(new_ಮಾರ್ಗ);
	fill_filespec(one, old_oid, old_oid_valid, old_ಬಹುಲಕ);
	fill_filespec(two, new_oid, new_oid_valid, new_ಬಹುಲಕ);

	diff_ಸರತಿ(&diff_queued_diff, one, two);
}

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_b_f(ರಚನೆ rev_info *revs,
			     ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv ಬಳಸದ,
			     ರಚನೆ object_array_ನಮೂದು **blob)
{
	/* Blob vs file in the working tree*/
	ರಚನೆ stat st;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;

	ಆದರೆ (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_ದತ್ತಾಂಶ, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	ಮಾರ್ಗ = revs->prune_ದತ್ತಾಂಶ.ವಸ್ತುಗಳು[0].ಹೊಂದಿಕೆ;

	ಆದರೆ (lstat(ಮಾರ್ಗ, &st))
		die_errno(_("failed to stat '%s'"), ಮಾರ್ಗ);
	ಆದರೆ (!(S_ISREG(st.st_ಬಹುಲಕ) || S_ISLNK(st.st_ಬಹುಲಕ)))
		die(_("'%s': not a regular file or symlink"), ಮಾರ್ಗ);

	diff_ಗಣ_mnemonic_ಪೂರ್ವಪ್ರತ್ಯಯ(&revs->diffopt, "o/", "w/");

	ಆದರೆ (blob[0]->ಬಹುಲಕ == S_IFINVALID)
		blob[0]->ಬಹುಲಕ = canon_ಬಹುಲಕ(st.st_ಬಹುಲಕ);

	stuff_change(&revs->diffopt,
		     blob[0]->ಬಹುಲಕ, canon_ಬಹುಲಕ(st.st_ಬಹುಲಕ),
		     &blob[0]->ಅಂಶ->oid, null_oid(the_hash_algo),
		     1, 0,
		     blob[0]->ಮಾರ್ಗ ? blob[0]->ಮಾರ್ಗ : ಮಾರ್ಗ,
		     ಮಾರ್ಗ);
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_blobs(ರಚನೆ rev_info *revs,
			       ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv ಬಳಸದ,
			       ರಚನೆ object_array_ನಮೂದು **blob)
{
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ = canon_ಬಹುಲಕ(S_IFREG | 0644);

	ಆದರೆ (argc > 1)
		usage(builtin_diff_usage);

	ಆದರೆ (blob[0]->ಬಹುಲಕ == S_IFINVALID)
		blob[0]->ಬಹುಲಕ = ಬಹುಲಕ;

	ಆದರೆ (blob[1]->ಬಹುಲಕ == S_IFINVALID)
		blob[1]->ಬಹುಲಕ = ಬಹುಲಕ;

	stuff_change(&revs->diffopt,
		     blob[0]->ಬಹುಲಕ, blob[1]->ಬಹುಲಕ,
		     &blob[0]->ಅಂಶ->oid, &blob[1]->ಅಂಶ->oid,
		     1, 1,
		     blob_ಮಾರ್ಗ(blob[0]), blob_ಮಾರ್ಗ(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_ಸೂಚ್ಯಂಕ(ರಚನೆ rev_info *revs,
			       ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ option = 0;
	ಆಗ (1 < argc) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[1];
		ಆದರೆ (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_ಸೂಚ್ಯಂಕ_CACHED;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--merge-base"))
			option |= DIFF_ಸೂಚ್ಯಂಕ_ವಿಲೀನ_ತಳ;
		ಇಲ್ಲದಿದ್ದರೆ
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	ಆದರೆ (revs->pending.nr != 1 ||
	    revs->max_ಎಣಿಕೆ != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	ಆದರೆ (!(option & DIFF_ಸೂಚ್ಯಂಕ_CACHED)) {
		ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
		ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	ಸತತ_diff_ಸೂಚ್ಯಂಕ(revs, option);
}

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_ವೃಕ್ಷ(ರಚನೆ rev_info *revs,
			      ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			      ರಚನೆ object_array_ನಮೂದು *ent0,
			      ರಚನೆ object_array_ನಮೂದು *ent1)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *(oid[2]);
	ರಚನೆ object_id mb_oid;
	ಪೂರ್ಣಾಂಕ ವಿಲೀನ_ತಳ = 0;

	ಆಗ (1 < argc) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[1];
		ಆದರೆ (!strcmp(arg, "--merge-base"))
			ವಿಲೀನ_ತಳ = 1;
		ಇಲ್ಲದಿದ್ದರೆ
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	ಆದರೆ (ವಿಲೀನ_ತಳ) {
		diff_get_ವಿಲೀನ_ತಳ(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].ಅಂಶ->oid;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಪೂರ್ಣಾಂಕ ಅದಲುಬದಲು_2 = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		ಆದರೆ (ent1->ಅಂಶ->flags & UNINTERESTING)
			ಅದಲುಬದಲು_2 = 1;
		oid[ಅದಲುಬದಲು_2] = &ent0->ಅಂಶ->oid;
		oid[1 - ಅದಲುಬದಲು_2] = &ent1->ಅಂಶ->oid;
	}
	diff_ವೃಕ್ಷ_oid(oid[0], oid[1], "", &revs->diffopt);
	log_ವೃಕ್ಷ_diff_flush(revs);
}

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_combined(ರಚನೆ rev_info *revs,
				  ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv ಬಳಸದ,
				  ರಚನೆ object_array_ನಮೂದು *ent,
				  ಪೂರ್ಣಾಂಕ ents, ಪೂರ್ಣಾಂಕ ಮೊದಲ_non_ಪೋಷಕ)
{
	ರಚನೆ oid_array parents = OID_ARRAY_INIT;
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (argc > 1)
		usage(builtin_diff_usage);

	ಆದರೆ (ಮೊದಲ_non_ಪೋಷಕ < 0)
		die(_("no merge given, only parents."));
	ಆದರೆ (ಮೊದಲ_non_ಪೋಷಕ >= ents)
		BUG("first_non_parent out of range: %d", ಮೊದಲ_non_ಪೋಷಕ);

	diff_merges_ಗಣ_dense_combined_if_unset(revs);

	ಗಾಗಿ (i = 0; i < ents; i++) {
		ಆದರೆ (i != ಮೊದಲ_non_ಪೋಷಕ)
			oid_array_append(&parents, &ent[i].ಅಂಶ->oid);
	}
	diff_ವೃಕ್ಷ_combined(&ent[ಮೊದಲ_non_ಪೋಷಕ].ಅಂಶ->oid, &parents, revs);
	oid_array_clear(&parents);
}

ಸ್ಥಿರ ಶೂನ್ಯ refresh_ಸೂಚ್ಯಂಕ_quietly(ಶೂನ್ಯ)
{
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ಪೂರ್ಣಾಂಕ fd;

	fd = repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, 0);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ;
	discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	repo_read_ಸೂಚ್ಯಂಕ(the_repository);
	refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET|REFRESH_UNMERGED, ಖಾಲಿ, ಖಾಲಿ,
		      ಖಾಲಿ);
	repo_update_ಸೂಚ್ಯಂಕ_if_able(the_repository, &ಬೀಗ_file);
}

ಸ್ಥಿರ ಶೂನ್ಯ builtin_diff_files(ರಚನೆ rev_info *revs, ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ options = 0;

	ಆಗ (1 < argc && argv[1][0] == '-') {
		ಆದರೆ (!strcmp(argv[1], "--base"))
			revs->max_ಎಣಿಕೆ = 1;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(argv[1], "--ours"))
			revs->max_ಎಣಿಕೆ = 2;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(argv[1], "--theirs"))
			revs->max_ಎಣಿಕೆ = 3;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		ಇಲ್ಲದಿದ್ದರೆ {
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
	ಆದರೆ (revs->max_ಎಣಿಕೆ == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_ಗಣ_dense_combined_if_unset(revs);

	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	ಸತತ_diff_files(revs, options);
}

ರಚನೆ symdiff {
	ರಚನೆ bitmap *skip;
	ಪೂರ್ಣಾಂಕ warn;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ, *ಎಡ, *ಬಲ;
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
ಸ್ಥಿರ ಶೂನ್ಯ symdiff_prepare(ರಚನೆ rev_info *rev, ರಚನೆ symdiff *sym)
{
	ಪೂರ್ಣಾಂಕ i, is_symdiff = 0, basecount = 0, othercount = 0;
	ಪೂರ್ಣಾಂಕ lpos = -1, rpos = -1, basepos = -1;
	ರಚನೆ bitmap *map = ಖಾಲಿ;

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
	ಗಾಗಿ (i = 0; i < rev->cmdline.nr; i++) {
		ರಚನೆ object *obj = rev->pending.objects[i].ಅಂಶ;
		ಆಯ್ಕೆ (rev->cmdline.rev[i].whence) {
		ಸ್ಥಿತಿ REV_CMD_ವಿಲೀನ_ತಳ:
			ಆದರೆ (basepos < 0)
				basepos = i;
			basecount++;
			ನಿಲ್ಲಿಸು;		/* do mark all bases */
		ಸ್ಥಿತಿ REV_CMD_ಎಡ:
			ಆದರೆ (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			ಆದರೆ (obj->flags & ಸಮಮಿತ_ಎಡ) {
				is_symdiff = 1;
				ನಿಲ್ಲಿಸು;	/* do mark A */
			}
			ಮುಂದುವರಿ;
		ಸ್ಥಿತಿ REV_CMD_ಬಲ:
			ಆದರೆ (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			ಮುಂದುವರಿ;	/* don't mark B */
		ಸ್ಥಿತಿ REV_CMD_PARENTS_ONLY:
		ಸ್ಥಿತಿ REV_CMD_REF:
		ಸ್ಥಿತಿ REV_CMD_REV:
			othercount++;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (!map)
			map = bitmap_new();
		bitmap_ಗಣ(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	ಆದರೆ (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	ಆದರೆ (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = ಖಾಲಿ;
		ಹಿಂತಿರುಗಿ;
	}

	sym->ಎಡ = rev->pending.objects[lpos].name;
	sym->ಬಲ = rev->pending.objects[rpos].name;
	ಆದರೆ (basecount == 0)
		die(_("%s...%s: no merge base"), sym->ಎಡ, sym->ಬಲ);
	sym->ತಳ = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

ಸ್ಥಿರ ಶೂನ್ಯ symdiff_release(ರಚನೆ symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

ಪೂರ್ಣಾಂಕ cmd_diff(ಪೂರ್ಣಾಂಕ argc,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ rev_info rev;
	ರಚನೆ object_array ent = OBJECT_ARRAY_INIT;
	ಪೂರ್ಣಾಂಕ ಮೊದಲ_non_ಪೋಷಕ = -1;
	ಪೂರ್ಣಾಂಕ blobs = 0, paths = 0;
	ರಚನೆ object_array_ನಮೂದು *blob[2];
	ಪೂರ್ಣಾಂಕ nongit = 0, no_ಸೂಚ್ಯಂಕ = 0;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
	ರಚನೆ symdiff sdiff;

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
	ಗಾಗಿ (i = 1; i < argc; i++) {
		ಆದರೆ (!strcmp(argv[i], "--")) {
			i++;
			ನಿಲ್ಲಿಸು;
		}
		ಆದರೆ (!strcmp(argv[i], "--no-index"))
			no_ಸೂಚ್ಯಂಕ = DIFF_NO_ಸೂಚ್ಯಂಕ_EXPLICIT;
		ಆದರೆ (argv[i][0] != '-')
			ನಿಲ್ಲಿಸು;
	}

	ಪೂರ್ವಪ್ರತ್ಯಯ = ಸಿದ್ಧತೆ_git_directory_gently(the_repository, &nongit);

	ಆದರೆ (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;
	}

	ಆದರೆ (!no_ಸೂಚ್ಯಂಕ) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		ಆದರೆ (nongit || ((argc == i + 2) &&
			       (!ಮಾರ್ಗ_inside_repo(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, argv[i]) ||
				!ಮಾರ್ಗ_inside_repo(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, argv[i + 1]))))
			no_ಸೂಚ್ಯಂಕ = DIFF_NO_ಸೂಚ್ಯಂಕ_IMPLICIT;
	}

	/*
	 * When operating outside of a Git repository we need to have a hash
	 * algorithm at hand so that we can generate the blob hashes. We
	 * default to SHA1 here, but may eventually want to change this to be
	 * configurable via a command line option.
	 */
	ಆದರೆ (nongit)
		repo_ಗಣ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ಖಾಲಿ);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	ಆದರೆ (no_ಸೂಚ್ಯಂಕ && ಪೂರ್ವಪ್ರತ್ಯಯ) {
		ಆದರೆ (chdir(ಪೂರ್ವಪ್ರತ್ಯಯ))
			die(_("cannot come back to cwd"));
		ಪೂರ್ವಪ್ರತ್ಯಯ = ಖಾಲಿ;
	}

	ಪೂರ್ವಪ್ರತ್ಯಯ = precompose_argv_ಪೂರ್ವಪ್ರತ್ಯಯ(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ);

	repo_init_revisions(the_repository, &rev, ಪೂರ್ವಪ್ರತ್ಯಯ);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	ಆದರೆ (no_ಸೂಚ್ಯಂಕ)
		ನಿರ್ಗಮಿಸು(diff_no_ಸೂಚ್ಯಂಕ(&rev, the_repository->hash_algo,
				   no_ಸೂಚ್ಯಂಕ == DIFF_NO_ಸೂಚ್ಯಂಕ_IMPLICIT,
				   argc, argv));


	/*
	 * Otherwise, we are doing the usual "git" diff; set up any
	 * further defaults that apply to regular diffs.
	 */
	rev.diffopt.skip_stat_unmatch = !!diff_auto_refresh_ಸೂಚ್ಯಂಕ;

	/*
	 * Default to intent-to-add entries invisible in the
	 * index. This makes them show up as new files in diff-files
	 * and not at all in diff-cached.
	 */
	rev.diffopt.ita_invisible_ಒಳ_ಸೂಚ್ಯಂಕ = 1;

	ಆದರೆ (nongit)
		die(_("Not a git repository"));
	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, &rev, ಖಾಲಿ);
	ಆದರೆ (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	rev.diffopt.rotate_ಗಮ್ಯ_strict = 1;

	ಸಿದ್ಧತೆ_diff_pager(&rev.diffopt);

	/*
	 * Do we have --cached and not have a pending object, then
	 * default to HEAD by hand.  Eek.
	 */
	ಆದರೆ (!rev.pending.nr) {
		ಪೂರ್ಣಾಂಕ i;
		ಗಾಗಿ (i = 1; i < argc; i++) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];
			ಆದರೆ (!strcmp(arg, "--"))
				ನಿಲ್ಲಿಸು;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				ಕೂಡಿಸು_ತಲೆ_ಗಮ್ಯ_pending(&rev);
				ಆದರೆ (!rev.pending.nr) {
					ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;
					ವೃಕ್ಷ = lookup_ವೃಕ್ಷ(the_repository,
							   the_repository->hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ);
					ಕೂಡಿಸು_pending_object(&rev, &ವೃಕ್ಷ->object, "HEAD");
				}
				ನಿಲ್ಲಿಸು;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	ಗಾಗಿ (i = 0; i < rev.pending.nr; i++) {
		ರಚನೆ object_array_ನಮೂದು *ನಮೂದು = &rev.pending.objects[i];
		ರಚನೆ object *obj = ನಮೂದು->ಅಂಶ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = ನಮೂದು->name;
		ಪೂರ್ಣಾಂಕ flags = (obj->flags & UNINTERESTING);
		ಆದರೆ (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_ಟ್ಯಾಗ್(the_repository, obj, ಖಾಲಿ, 0);
		ಆದರೆ (!obj)
			die(_("invalid object '%s' given."), name);
		ಆದರೆ (obj->type == OBJ_COMMIT) {
			ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(
				the_repository, (ರಚನೆ commit *)obj);
			ಆದರೆ (!ವೃಕ್ಷ)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &ವೃಕ್ಷ->object;
		}

		ಆದರೆ (obj->type == OBJ_ವೃಕ್ಷ) {
			ಆದರೆ (sdiff.skip && bitmap_get(sdiff.skip, i))
				ಮುಂದುವರಿ;
			obj->flags |= flags;
			ಕೂಡಿಸು_object_array(obj, name, &ent);
			ಆದರೆ (ಮೊದಲ_non_ಪೋಷಕ < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				ಮೊದಲ_non_ಪೋಷಕ = ent.nr - 1;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (obj->type == OBJ_BLOB) {
			ಆದರೆ (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = ನಮೂದು;
			blobs++;

		} ಇಲ್ಲದಿದ್ದರೆ {
			die(_("unhandled object '%s' given."), name);
		}
	}
	ಆದರೆ (rev.prune_ದತ್ತಾಂಶ.nr)
		paths += rev.prune_ದತ್ತಾಂಶ.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	ಆದರೆ (!ent.nr) {
		ಆಯ್ಕೆ (blobs) {
		ಸ್ಥಿತಿ 0:
			builtin_diff_files(&rev, argc, argv);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 1:
			ಆದರೆ (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 2:
			ಆದರೆ (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			usage(builtin_diff_usage);
		}
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (blobs)
		usage(builtin_diff_usage);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ent.nr == 1)
		builtin_diff_ಸೂಚ್ಯಂಕ(&rev, argc, argv);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ent.nr == 2) {
		ಆದರೆ (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.ಎಡ, sdiff.ಬಲ, sdiff.ತಳ);
		builtin_diff_ವೃಕ್ಷ(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} ಇಲ್ಲದಿದ್ದರೆ
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      ಮೊದಲ_non_ಪೋಷಕ);
	ಫಲಿತಾಂಶ = diff_ಫಲಿತಾಂಶ_ಸಂಕೇತ(&rev);
	ಆದರೆ (1 < rev.diffopt.skip_stat_unmatch)
		refresh_ಸೂಚ್ಯಂಕ_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
