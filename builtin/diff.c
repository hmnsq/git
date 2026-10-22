/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ewah/ewok.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "color.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff-merges.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diffcore.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "preload-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "log-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIFF_NO_ਸੂਚਕਾਂਕ_EXPLICIT 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIFF_NO_ਸੂਚਕਾਂਕ_IMPLICIT 2

ਸਥਿਰ ਅਚਲ ਅੱਖਰ builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *blob_ਰਸਤਾ(ਬਣਤਰ object_array_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
	ਵਾਪਸ ਇੰਦਰਾਜ->ਰਸਤਾ ? ਇੰਦਰਾਜ->ਰਸਤਾ : ਇੰਦਰਾਜ->name;
}

ਸਥਿਰ ਖਾਲੀ stuff_change(ਬਣਤਰ diff_options *opt,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਬਹੁਲਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਬਹੁਲਕ,
			 ਅਚਲ ਬਣਤਰ object_id *old_oid,
			 ਅਚਲ ਬਣਤਰ object_id *new_oid,
			 ਪੂਰਨ old_oid_valid,
			 ਪੂਰਨ new_oid_valid,
			 ਅਚਲ ਅੱਖਰ *old_ਰਸਤਾ,
			 ਅਚਲ ਅੱਖਰ *new_ਰਸਤਾ)
{
	ਬਣਤਰ diff_filespec *one, *two;

	ਜੇ (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_ਬਹੁਲਕ == new_ਬਹੁਲਕ))
		ਵਾਪਸ;

	ਜੇ (opt->flags.ਉਲਟ_diff) {
		ਅਦਲਾ_ਬਦਲੀ(old_ਬਹੁਲਕ, new_ਬਹੁਲਕ);
		ਅਦਲਾ_ਬਦਲੀ(old_oid, new_oid);
		ਅਦਲਾ_ਬਦਲੀ(old_ਰਸਤਾ, new_ਰਸਤਾ);
	}

	ਜੇ (opt->ਅਗੇਤਰ &&
	    (ਲੜੀ_ਤੁਲਨਾ(old_ਰਸਤਾ, opt->ਅਗੇਤਰ, opt->ਅਗੇਤਰ_ਲੰਮਾਈ) ||
	     ਲੜੀ_ਤੁਲਨਾ(new_ਰਸਤਾ, opt->ਅਗੇਤਰ, opt->ਅਗੇਤਰ_ਲੰਮਾਈ)))
		ਵਾਪਸ;

	one = alloc_filespec(old_ਰਸਤਾ);
	two = alloc_filespec(new_ਰਸਤਾ);
	fill_filespec(one, old_oid, old_oid_valid, old_ਬਹੁਲਕ);
	fill_filespec(two, new_oid, new_oid_valid, new_ਬਹੁਲਕ);

	diff_ਕਤਾਰ(&diff_queued_diff, one, two);
}

ਸਥਿਰ ਖਾਲੀ builtin_diff_b_f(ਬਣਤਰ rev_info *revs,
			     ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv ਅਣਵਰਤਿਆ,
			     ਬਣਤਰ object_array_ਇੰਦਰਾਜ **blob)
{
	/* Blob vs file in the working tree*/
	ਬਣਤਰ stat st;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;

	ਜੇ (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_ਅੰਕੜੇ, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	ਰਸਤਾ = revs->prune_ਅੰਕੜੇ.ਚੀਜ਼ਾਂ[0].ਮੇਲ;

	ਜੇ (lstat(ਰਸਤਾ, &st))
		die_errno(_("failed to stat '%s'"), ਰਸਤਾ);
	ਜੇ (!(S_ISREG(st.st_ਬਹੁਲਕ) || S_ISLNK(st.st_ਬਹੁਲਕ)))
		die(_("'%s': not a regular file or symlink"), ਰਸਤਾ);

	diff_ਸੈੱਟ_mnemonic_ਅਗੇਤਰ(&revs->diffopt, "o/", "w/");

	ਜੇ (blob[0]->ਬਹੁਲਕ == S_IFINVALID)
		blob[0]->ਬਹੁਲਕ = canon_ਬਹੁਲਕ(st.st_ਬਹੁਲਕ);

	stuff_change(&revs->diffopt,
		     blob[0]->ਬਹੁਲਕ, canon_ਬਹੁਲਕ(st.st_ਬਹੁਲਕ),
		     &blob[0]->ਵਸਤੂ->oid, null_oid(the_hash_algo),
		     1, 0,
		     blob[0]->ਰਸਤਾ ? blob[0]->ਰਸਤਾ : ਰਸਤਾ,
		     ਰਸਤਾ);
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ਸਥਿਰ ਖਾਲੀ builtin_diff_blobs(ਬਣਤਰ rev_info *revs,
			       ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv ਅਣਵਰਤਿਆ,
			       ਬਣਤਰ object_array_ਇੰਦਰਾਜ **blob)
{
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ = canon_ਬਹੁਲਕ(S_IFREG | 0644);

	ਜੇ (argc > 1)
		usage(builtin_diff_usage);

	ਜੇ (blob[0]->ਬਹੁਲਕ == S_IFINVALID)
		blob[0]->ਬਹੁਲਕ = ਬਹੁਲਕ;

	ਜੇ (blob[1]->ਬਹੁਲਕ == S_IFINVALID)
		blob[1]->ਬਹੁਲਕ = ਬਹੁਲਕ;

	stuff_change(&revs->diffopt,
		     blob[0]->ਬਹੁਲਕ, blob[1]->ਬਹੁਲਕ,
		     &blob[0]->ਵਸਤੂ->oid, &blob[1]->ਵਸਤੂ->oid,
		     1, 1,
		     blob_ਰਸਤਾ(blob[0]), blob_ਰਸਤਾ(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ਸਥਿਰ ਖਾਲੀ builtin_diff_ਸੂਚਕਾਂਕ(ਬਣਤਰ rev_info *revs,
			       ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ option = 0;
	ਜਦੋਂ (1 < argc) {
		ਅਚਲ ਅੱਖਰ *arg = argv[1];
		ਜੇ (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_ਸੂਚਕਾਂਕ_CACHED;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--merge-base"))
			option |= DIFF_ਸੂਚਕਾਂਕ_ਮਿਲਾਓ_ਅਧਾਰ;
		ਨਹੀਂਤਾਂ
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	ਜੇ (revs->pending.nr != 1 ||
	    revs->max_ਗਿਣਤੀ != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	ਜੇ (!(option & DIFF_ਸੂਚਕਾਂਕ_CACHED)) {
		ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
		ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} ਨਹੀਂਤਾਂ ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	ਲੜੀਵਾਰ_diff_ਸੂਚਕਾਂਕ(revs, option);
}

ਸਥਿਰ ਖਾਲੀ builtin_diff_ਰੁੱਖ(ਬਣਤਰ rev_info *revs,
			      ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
			      ਬਣਤਰ object_array_ਇੰਦਰਾਜ *ent0,
			      ਬਣਤਰ object_array_ਇੰਦਰਾਜ *ent1)
{
	ਅਚਲ ਬਣਤਰ object_id *(oid[2]);
	ਬਣਤਰ object_id mb_oid;
	ਪੂਰਨ ਮਿਲਾਓ_ਅਧਾਰ = 0;

	ਜਦੋਂ (1 < argc) {
		ਅਚਲ ਅੱਖਰ *arg = argv[1];
		ਜੇ (!strcmp(arg, "--merge-base"))
			ਮਿਲਾਓ_ਅਧਾਰ = 1;
		ਨਹੀਂਤਾਂ
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	ਜੇ (ਮਿਲਾਓ_ਅਧਾਰ) {
		diff_get_ਮਿਲਾਓ_ਅਧਾਰ(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].ਵਸਤੂ->oid;
	} ਨਹੀਂਤਾਂ {
		ਪੂਰਨ ਅਦਲਾ_ਬਦਲੀ_2 = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		ਜੇ (ent1->ਵਸਤੂ->flags & UNINTERESTING)
			ਅਦਲਾ_ਬਦਲੀ_2 = 1;
		oid[ਅਦਲਾ_ਬਦਲੀ_2] = &ent0->ਵਸਤੂ->oid;
		oid[1 - ਅਦਲਾ_ਬਦਲੀ_2] = &ent1->ਵਸਤੂ->oid;
	}
	diff_ਰੁੱਖ_oid(oid[0], oid[1], "", &revs->diffopt);
	log_ਰੁੱਖ_diff_flush(revs);
}

ਸਥਿਰ ਖਾਲੀ builtin_diff_combined(ਬਣਤਰ rev_info *revs,
				  ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv ਅਣਵਰਤਿਆ,
				  ਬਣਤਰ object_array_ਇੰਦਰਾਜ *ent,
				  ਪੂਰਨ ents, ਪੂਰਨ ਪਹਿਲਾ_non_ਮਾਪਾ)
{
	ਬਣਤਰ oid_array parents = OID_ARRAY_INIT;
	ਪੂਰਨ i;

	ਜੇ (argc > 1)
		usage(builtin_diff_usage);

	ਜੇ (ਪਹਿਲਾ_non_ਮਾਪਾ < 0)
		die(_("no merge given, only parents."));
	ਜੇ (ਪਹਿਲਾ_non_ਮਾਪਾ >= ents)
		BUG("first_non_parent out of range: %d", ਪਹਿਲਾ_non_ਮਾਪਾ);

	diff_merges_ਸੈੱਟ_dense_combined_if_unset(revs);

	ਲਈ (i = 0; i < ents; i++) {
		ਜੇ (i != ਪਹਿਲਾ_non_ਮਾਪਾ)
			oid_array_append(&parents, &ent[i].ਵਸਤੂ->oid);
	}
	diff_ਰੁੱਖ_combined(&ent[ਪਹਿਲਾ_non_ਮਾਪਾ].ਵਸਤੂ->oid, &parents, revs);
	oid_array_clear(&parents);
}

ਸਥਿਰ ਖਾਲੀ refresh_ਸੂਚਕਾਂਕ_quietly(ਖਾਲੀ)
{
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਪੂਰਨ fd;

	fd = repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, 0);
	ਜੇ (fd < 0)
		ਵਾਪਸ;
	discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	repo_read_ਸੂਚਕਾਂਕ(the_repository);
	refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET|REFRESH_UNMERGED, ਨਿਰਾ, ਨਿਰਾ,
		      ਨਿਰਾ);
	repo_update_ਸੂਚਕਾਂਕ_if_able(the_repository, &ਤਾਲਾ_file);
}

ਸਥਿਰ ਖਾਲੀ builtin_diff_files(ਬਣਤਰ rev_info *revs, ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ options = 0;

	ਜਦੋਂ (1 < argc && argv[1][0] == '-') {
		ਜੇ (!strcmp(argv[1], "--base"))
			revs->max_ਗਿਣਤੀ = 1;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(argv[1], "--ours"))
			revs->max_ਗਿਣਤੀ = 2;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(argv[1], "--theirs"))
			revs->max_ਗਿਣਤੀ = 3;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		ਨਹੀਂਤਾਂ {
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
	ਜੇ (revs->max_ਗਿਣਤੀ == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_ਸੈੱਟ_dense_combined_if_unset(revs);

	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	ਲੜੀਵਾਰ_diff_files(revs, options);
}

ਬਣਤਰ symdiff {
	ਬਣਤਰ bitmap *skip;
	ਪੂਰਨ warn;
	ਅਚਲ ਅੱਖਰ *ਅਧਾਰ, *ਖੱਬਾ, *ਸੱਜਾ;
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
ਸਥਿਰ ਖਾਲੀ symdiff_prepare(ਬਣਤਰ rev_info *rev, ਬਣਤਰ symdiff *sym)
{
	ਪੂਰਨ i, is_symdiff = 0, basecount = 0, othercount = 0;
	ਪੂਰਨ lpos = -1, rpos = -1, basepos = -1;
	ਬਣਤਰ bitmap *map = ਨਿਰਾ;

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
	ਲਈ (i = 0; i < rev->cmdline.nr; i++) {
		ਬਣਤਰ object *obj = rev->pending.objects[i].ਵਸਤੂ;
		ਚੋਣ (rev->cmdline.rev[i].whence) {
		ਹਾਲਤ REV_CMD_ਮਿਲਾਓ_ਅਧਾਰ:
			ਜੇ (basepos < 0)
				basepos = i;
			basecount++;
			ਰੋਕੋ;		/* do mark all bases */
		ਹਾਲਤ REV_CMD_ਖੱਬਾ:
			ਜੇ (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			ਜੇ (obj->flags & ਸਮਮਿਤ_ਖੱਬਾ) {
				is_symdiff = 1;
				ਰੋਕੋ;	/* do mark A */
			}
			ਜਾਰੀ;
		ਹਾਲਤ REV_CMD_ਸੱਜਾ:
			ਜੇ (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			ਜਾਰੀ;	/* don't mark B */
		ਹਾਲਤ REV_CMD_PARENTS_ONLY:
		ਹਾਲਤ REV_CMD_REF:
		ਹਾਲਤ REV_CMD_REV:
			othercount++;
			ਜਾਰੀ;
		}
		ਜੇ (!map)
			map = bitmap_new();
		bitmap_ਸੈੱਟ(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	ਜੇ (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	ਜੇ (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = ਨਿਰਾ;
		ਵਾਪਸ;
	}

	sym->ਖੱਬਾ = rev->pending.objects[lpos].name;
	sym->ਸੱਜਾ = rev->pending.objects[rpos].name;
	ਜੇ (basecount == 0)
		die(_("%s...%s: no merge base"), sym->ਖੱਬਾ, sym->ਸੱਜਾ);
	sym->ਅਧਾਰ = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

ਸਥਿਰ ਖਾਲੀ symdiff_release(ਬਣਤਰ symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

ਪੂਰਨ cmd_diff(ਪੂਰਨ argc,
	     ਅਚਲ ਅੱਖਰ **argv,
	     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i;
	ਬਣਤਰ rev_info rev;
	ਬਣਤਰ object_array ent = OBJECT_ARRAY_INIT;
	ਪੂਰਨ ਪਹਿਲਾ_non_ਮਾਪਾ = -1;
	ਪੂਰਨ blobs = 0, paths = 0;
	ਬਣਤਰ object_array_ਇੰਦਰਾਜ *blob[2];
	ਪੂਰਨ nongit = 0, no_ਸੂਚਕਾਂਕ = 0;
	ਪੂਰਨ ਨਤੀਜਾ;
	ਬਣਤਰ symdiff sdiff;

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
	ਲਈ (i = 1; i < argc; i++) {
		ਜੇ (!strcmp(argv[i], "--")) {
			i++;
			ਰੋਕੋ;
		}
		ਜੇ (!strcmp(argv[i], "--no-index"))
			no_ਸੂਚਕਾਂਕ = DIFF_NO_ਸੂਚਕਾਂਕ_EXPLICIT;
		ਜੇ (argv[i][0] != '-')
			ਰੋਕੋ;
	}

	ਅਗੇਤਰ = ਤਿਆਰੀ_git_directory_gently(the_repository, &nongit);

	ਜੇ (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;
	}

	ਜੇ (!no_ਸੂਚਕਾਂਕ) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		ਜੇ (nongit || ((argc == i + 2) &&
			       (!ਰਸਤਾ_inside_repo(the_repository, ਅਗੇਤਰ, argv[i]) ||
				!ਰਸਤਾ_inside_repo(the_repository, ਅਗੇਤਰ, argv[i + 1]))))
			no_ਸੂਚਕਾਂਕ = DIFF_NO_ਸੂਚਕਾਂਕ_IMPLICIT;
	}

	/*
	 * When operating outside of a Git repository we need to have a hash
	 * algorithm at hand so that we can generate the blob hashes. We
	 * default to SHA1 here, but may eventually want to change this to be
	 * configurable via a command line option.
	 */
	ਜੇ (nongit)
		repo_ਸੈੱਟ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ਨਿਰਾ);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	ਜੇ (no_ਸੂਚਕਾਂਕ && ਅਗੇਤਰ) {
		ਜੇ (chdir(ਅਗੇਤਰ))
			die(_("cannot come back to cwd"));
		ਅਗੇਤਰ = ਨਿਰਾ;
	}

	ਅਗੇਤਰ = precompose_argv_ਅਗੇਤਰ(argc, argv, ਅਗੇਤਰ);

	repo_init_revisions(the_repository, &rev, ਅਗੇਤਰ);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	ਜੇ (no_ਸੂਚਕਾਂਕ)
		ਬਾਹਰ_ਨਿਕਲੋ(diff_no_ਸੂਚਕਾਂਕ(&rev, the_repository->hash_algo,
				   no_ਸੂਚਕਾਂਕ == DIFF_NO_ਸੂਚਕਾਂਕ_IMPLICIT,
				   argc, argv));


	/*
	 * Otherwise, we are doing the usual "git" diff; set up any
	 * further defaults that apply to regular diffs.
	 */
	rev.diffopt.skip_stat_unmatch = !!diff_auto_refresh_ਸੂਚਕਾਂਕ;

	/*
	 * Default to intent-to-add entries invisible in the
	 * index. This makes them show up as new files in diff-files
	 * and not at all in diff-cached.
	 */
	rev.diffopt.ita_invisible_ਵਿੱਚ_ਸੂਚਕਾਂਕ = 1;

	ਜੇ (nongit)
		die(_("Not a git repository"));
	argc = ਤਿਆਰੀ_revisions(argc, argv, &rev, ਨਿਰਾ);
	ਜੇ (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_ਤਿਆਰੀ_ਪੂਰਾ(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	rev.diffopt.rotate_ਮੰਜ਼ਿਲ_strict = 1;

	ਤਿਆਰੀ_diff_pager(&rev.diffopt);

	/*
	 * Do we have --cached and not have a pending object, then
	 * default to HEAD by hand.  Eek.
	 */
	ਜੇ (!rev.pending.nr) {
		ਪੂਰਨ i;
		ਲਈ (i = 1; i < argc; i++) {
			ਅਚਲ ਅੱਖਰ *arg = argv[i];
			ਜੇ (!strcmp(arg, "--"))
				ਰੋਕੋ;
			ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				ਜੋੜੋ_ਸਿਰ_ਮੰਜ਼ਿਲ_pending(&rev);
				ਜੇ (!rev.pending.nr) {
					ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;
					ਰੁੱਖ = lookup_ਰੁੱਖ(the_repository,
							   the_repository->hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ);
					ਜੋੜੋ_pending_object(&rev, &ਰੁੱਖ->object, "HEAD");
				}
				ਰੋਕੋ;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	ਲਈ (i = 0; i < rev.pending.nr; i++) {
		ਬਣਤਰ object_array_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = &rev.pending.objects[i];
		ਬਣਤਰ object *obj = ਇੰਦਰਾਜ->ਵਸਤੂ;
		ਅਚਲ ਅੱਖਰ *name = ਇੰਦਰਾਜ->name;
		ਪੂਰਨ flags = (obj->flags & UNINTERESTING);
		ਜੇ (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_ਟੈਗ(the_repository, obj, ਨਿਰਾ, 0);
		ਜੇ (!obj)
			die(_("invalid object '%s' given."), name);
		ਜੇ (obj->type == OBJ_COMMIT) {
			ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ = repo_get_commit_ਰੁੱਖ(
				the_repository, (ਬਣਤਰ commit *)obj);
			ਜੇ (!ਰੁੱਖ)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &ਰੁੱਖ->object;
		}

		ਜੇ (obj->type == OBJ_ਰੁੱਖ) {
			ਜੇ (sdiff.skip && bitmap_get(sdiff.skip, i))
				ਜਾਰੀ;
			obj->flags |= flags;
			ਜੋੜੋ_object_array(obj, name, &ent);
			ਜੇ (ਪਹਿਲਾ_non_ਮਾਪਾ < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				ਪਹਿਲਾ_non_ਮਾਪਾ = ent.nr - 1;
		} ਨਹੀਂਤਾਂ ਜੇ (obj->type == OBJ_BLOB) {
			ਜੇ (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = ਇੰਦਰਾਜ;
			blobs++;

		} ਨਹੀਂਤਾਂ {
			die(_("unhandled object '%s' given."), name);
		}
	}
	ਜੇ (rev.prune_ਅੰਕੜੇ.nr)
		paths += rev.prune_ਅੰਕੜੇ.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	ਜੇ (!ent.nr) {
		ਚੋਣ (blobs) {
		ਹਾਲਤ 0:
			builtin_diff_files(&rev, argc, argv);
			ਰੋਕੋ;
		ਹਾਲਤ 1:
			ਜੇ (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			ਰੋਕੋ;
		ਹਾਲਤ 2:
			ਜੇ (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			ਰੋਕੋ;
		ਮੂਲ:
			usage(builtin_diff_usage);
		}
	}
	ਨਹੀਂਤਾਂ ਜੇ (blobs)
		usage(builtin_diff_usage);
	ਨਹੀਂਤਾਂ ਜੇ (ent.nr == 1)
		builtin_diff_ਸੂਚਕਾਂਕ(&rev, argc, argv);
	ਨਹੀਂਤਾਂ ਜੇ (ent.nr == 2) {
		ਜੇ (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.ਖੱਬਾ, sdiff.ਸੱਜਾ, sdiff.ਅਧਾਰ);
		builtin_diff_ਰੁੱਖ(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} ਨਹੀਂਤਾਂ
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      ਪਹਿਲਾ_non_ਮਾਪਾ);
	ਨਤੀਜਾ = diff_ਨਤੀਜਾ_ਕੋਡ(&rev);
	ਜੇ (1 < rev.diffopt.skip_stat_unmatch)
		refresh_ਸੂਚਕਾਂਕ_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	ਵਾਪਸ ਨਤੀਜਾ;
}
