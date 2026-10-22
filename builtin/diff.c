/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "ewah/ewok.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "color.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "diff-merges.h"
#అన్తర్భావ "diffcore.h"
#అన్తర్భావ "preload-index.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "log-tree.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "tree.h"

#నిర్వచించు DIFF_NO_సూచీ_EXPLICIT 1
#నిర్వచించు DIFF_NO_సూచీ_IMPLICIT 2

స్థిర స్థిరరాశి అక్షరం builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

స్థిర స్థిరరాశి అక్షరం *blob_మార్గం(నిర్మాణం object_array_నమోదు *నమోదు_2)
{
	తిరిగి నమోదు_2->మార్గం ? నమోదు_2->మార్గం : నమోదు_2->name;
}

స్థిర శూన్య stuff_change(నిర్మాణం diff_options *opt,
			 గుర్తులేని old_బహుళకం, గుర్తులేని new_బహుళకం,
			 స్థిరరాశి నిర్మాణం object_id *old_oid,
			 స్థిరరాశి నిర్మాణం object_id *new_oid,
			 పూర్ణాంకం old_oid_valid,
			 పూర్ణాంకం new_oid_valid,
			 స్థిరరాశి అక్షరం *old_మార్గం,
			 స్థిరరాశి అక్షరం *new_మార్గం)
{
	నిర్మాణం diff_filespec *one, *two;

	ఒకవేళ (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_బహుళకం == new_బహుళకం))
		తిరిగి;

	ఒకవేళ (opt->flags.తిరగేయి_diff) {
		మార్పిడి(old_బహుళకం, new_బహుళకం);
		మార్పిడి(old_oid, new_oid);
		మార్పిడి(old_మార్గం, new_మార్గం);
	}

	ఒకవేళ (opt->ఉపసర్గ &&
	    (అక్షరశ్రేణి_పోలిక(old_మార్గం, opt->ఉపసర్గ, opt->ఉపసర్గ_నిడివి) ||
	     అక్షరశ్రేణి_పోలిక(new_మార్గం, opt->ఉపసర్గ, opt->ఉపసర్గ_నిడివి)))
		తిరిగి;

	one = alloc_filespec(old_మార్గం);
	two = alloc_filespec(new_మార్గం);
	fill_filespec(one, old_oid, old_oid_valid, old_బహుళకం);
	fill_filespec(two, new_oid, new_oid_valid, new_బహుళకం);

	diff_వరుస(&diff_queued_diff, one, two);
}

స్థిర శూన్య builtin_diff_b_f(నిర్మాణం rev_info *revs,
			     పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv వాడని,
			     నిర్మాణం object_array_నమోదు **blob)
{
	/* Blob vs file in the working tree*/
	నిర్మాణం stat st;
	స్థిరరాశి అక్షరం *మార్గం;

	ఒకవేళ (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_దత్తాంశం, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	మార్గం = revs->prune_దత్తాంశం.వస్తువులు[0].సరిపోలిక;

	ఒకవేళ (lstat(మార్గం, &st))
		die_errno(_("failed to stat '%s'"), మార్గం);
	ఒకవేళ (!(S_ISREG(st.st_బహుళకం) || S_ISLNK(st.st_బహుళకం)))
		die(_("'%s': not a regular file or symlink"), మార్గం);

	diff_సమితి_mnemonic_ఉపసర్గ(&revs->diffopt, "o/", "w/");

	ఒకవేళ (blob[0]->బహుళకం == S_IFINVALID)
		blob[0]->బహుళకం = canon_బహుళకం(st.st_బహుళకం);

	stuff_change(&revs->diffopt,
		     blob[0]->బహుళకం, canon_బహుళకం(st.st_బహుళకం),
		     &blob[0]->అంశం->oid, null_oid(the_hash_algo),
		     1, 0,
		     blob[0]->మార్గం ? blob[0]->మార్గం : మార్గం,
		     మార్గం);
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

స్థిర శూన్య builtin_diff_blobs(నిర్మాణం rev_info *revs,
			       పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv వాడని,
			       నిర్మాణం object_array_నమోదు **blob)
{
	స్థిరరాశి గుర్తులేని బహుళకం = canon_బహుళకం(S_IFREG | 0644);

	ఒకవేళ (argc > 1)
		usage(builtin_diff_usage);

	ఒకవేళ (blob[0]->బహుళకం == S_IFINVALID)
		blob[0]->బహుళకం = బహుళకం;

	ఒకవేళ (blob[1]->బహుళకం == S_IFINVALID)
		blob[1]->బహుళకం = బహుళకం;

	stuff_change(&revs->diffopt,
		     blob[0]->బహుళకం, blob[1]->బహుళకం,
		     &blob[0]->అంశం->oid, &blob[1]->అంశం->oid,
		     1, 1,
		     blob_మార్గం(blob[0]), blob_మార్గం(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

స్థిర శూన్య builtin_diff_సూచీ(నిర్మాణం rev_info *revs,
			       పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	గుర్తులేని పూర్ణాంకం option = 0;
	అయితే (1 < argc) {
		స్థిరరాశి అక్షరం *arg = argv[1];
		ఒకవేళ (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_సూచీ_CACHED;
		లేకపోతే ఒకవేళ (!strcmp(arg, "--merge-base"))
			option |= DIFF_సూచీ_విలీనం_పునాది;
		లేకపోతే
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	ఒకవేళ (revs->pending.nr != 1 ||
	    revs->max_లెక్క != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	ఒకవేళ (!(option & DIFF_సూచీ_CACHED)) {
		అమరిక_work_వృక్షం(the_repository);
		ఒకవేళ (repo_read_సూచీ_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} లేకపోతే ఒకవేళ (repo_read_సూచీ(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	నిరంతరం_diff_సూచీ(revs, option);
}

స్థిర శూన్య builtin_diff_వృక్షం(నిర్మాణం rev_info *revs,
			      పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
			      నిర్మాణం object_array_నమోదు *ent0,
			      నిర్మాణం object_array_నమోదు *ent1)
{
	స్థిరరాశి నిర్మాణం object_id *(oid[2]);
	నిర్మాణం object_id mb_oid;
	పూర్ణాంకం విలీనం_పునాది = 0;

	అయితే (1 < argc) {
		స్థిరరాశి అక్షరం *arg = argv[1];
		ఒకవేళ (!strcmp(arg, "--merge-base"))
			విలీనం_పునాది = 1;
		లేకపోతే
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	ఒకవేళ (విలీనం_పునాది) {
		diff_get_విలీనం_పునాది(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].అంశం->oid;
	} లేకపోతే {
		పూర్ణాంకం మార్పిడి_2 = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		ఒకవేళ (ent1->అంశం->flags & UNINTERESTING)
			మార్పిడి_2 = 1;
		oid[మార్పిడి_2] = &ent0->అంశం->oid;
		oid[1 - మార్పిడి_2] = &ent1->అంశం->oid;
	}
	diff_వృక్షం_oid(oid[0], oid[1], "", &revs->diffopt);
	log_వృక్షం_diff_flush(revs);
}

స్థిర శూన్య builtin_diff_combined(నిర్మాణం rev_info *revs,
				  పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv వాడని,
				  నిర్మాణం object_array_నమోదు *ent,
				  పూర్ణాంకం ents, పూర్ణాంకం మొదటి_non_మాతృ)
{
	నిర్మాణం oid_array parents = OID_ARRAY_INIT;
	పూర్ణాంకం i;

	ఒకవేళ (argc > 1)
		usage(builtin_diff_usage);

	ఒకవేళ (మొదటి_non_మాతృ < 0)
		die(_("no merge given, only parents."));
	ఒకవేళ (మొదటి_non_మాతృ >= ents)
		BUG("first_non_parent out of range: %d", మొదటి_non_మాతృ);

	diff_merges_సమితి_dense_combined_if_unset(revs);

	కొరకు (i = 0; i < ents; i++) {
		ఒకవేళ (i != మొదటి_non_మాతృ)
			oid_array_append(&parents, &ent[i].అంశం->oid);
	}
	diff_వృక్షం_combined(&ent[మొదటి_non_మాతృ].అంశం->oid, &parents, revs);
	oid_array_clear(&parents);
}

స్థిర శూన్య refresh_సూచీ_quietly(శూన్య)
{
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	పూర్ణాంకం fd;

	fd = repo_hold_locked_సూచీ(the_repository, &తాళం_file, 0);
	ఒకవేళ (fd < 0)
		తిరిగి;
	discard_సూచీ(the_repository->సూచీ);
	repo_read_సూచీ(the_repository);
	refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET|REFRESH_UNMERGED, ఖాళీ, ఖాళీ,
		      ఖాళీ);
	repo_update_సూచీ_if_able(the_repository, &తాళం_file);
}

స్థిర శూన్య builtin_diff_files(నిర్మాణం rev_info *revs, పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	గుర్తులేని పూర్ణాంకం options = 0;

	అయితే (1 < argc && argv[1][0] == '-') {
		ఒకవేళ (!strcmp(argv[1], "--base"))
			revs->max_లెక్క = 1;
		లేకపోతే ఒకవేళ (!strcmp(argv[1], "--ours"))
			revs->max_లెక్క = 2;
		లేకపోతే ఒకవేళ (!strcmp(argv[1], "--theirs"))
			revs->max_లెక్క = 3;
		లేకపోతే ఒకవేళ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		లేకపోతే ఒకవేళ (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		లేకపోతే {
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
	ఒకవేళ (revs->max_లెక్క == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_సమితి_dense_combined_if_unset(revs);

	అమరిక_work_వృక్షం(the_repository);
	ఒకవేళ (repo_read_సూచీ_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	నిరంతరం_diff_files(revs, options);
}

నిర్మాణం symdiff {
	నిర్మాణం bitmap *skip;
	పూర్ణాంకం warn;
	స్థిరరాశి అక్షరం *పునాది, *ఎడమ, *కుడి;
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
స్థిర శూన్య symdiff_prepare(నిర్మాణం rev_info *rev, నిర్మాణం symdiff *sym)
{
	పూర్ణాంకం i, is_symdiff = 0, basecount = 0, othercount = 0;
	పూర్ణాంకం lpos = -1, rpos = -1, basepos = -1;
	నిర్మాణం bitmap *map = ఖాళీ;

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
	కొరకు (i = 0; i < rev->cmdline.nr; i++) {
		నిర్మాణం object *obj = rev->pending.objects[i].అంశం;
		ఎంపిక (rev->cmdline.rev[i].whence) {
		స్థితి REV_CMD_విలీనం_పునాది:
			ఒకవేళ (basepos < 0)
				basepos = i;
			basecount++;
			ఆపు;		/* do mark all bases */
		స్థితి REV_CMD_ఎడమ:
			ఒకవేళ (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			ఒకవేళ (obj->flags & సౌష్ఠవ_ఎడమ) {
				is_symdiff = 1;
				ఆపు;	/* do mark A */
			}
			కొనసాగు;
		స్థితి REV_CMD_కుడి:
			ఒకవేళ (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			కొనసాగు;	/* don't mark B */
		స్థితి REV_CMD_PARENTS_ONLY:
		స్థితి REV_CMD_REF:
		స్థితి REV_CMD_REV:
			othercount++;
			కొనసాగు;
		}
		ఒకవేళ (!map)
			map = bitmap_new();
		bitmap_సమితి(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	ఒకవేళ (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	ఒకవేళ (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = ఖాళీ;
		తిరిగి;
	}

	sym->ఎడమ = rev->pending.objects[lpos].name;
	sym->కుడి = rev->pending.objects[rpos].name;
	ఒకవేళ (basecount == 0)
		die(_("%s...%s: no merge base"), sym->ఎడమ, sym->కుడి);
	sym->పునాది = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

స్థిర శూన్య symdiff_release(నిర్మాణం symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

పూర్ణాంకం cmd_diff(పూర్ణాంకం argc,
	     స్థిరరాశి అక్షరం **argv,
	     స్థిరరాశి అక్షరం *ఉపసర్గ,
	     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i;
	నిర్మాణం rev_info rev;
	నిర్మాణం object_array ent = OBJECT_ARRAY_INIT;
	పూర్ణాంకం మొదటి_non_మాతృ = -1;
	పూర్ణాంకం blobs = 0, paths = 0;
	నిర్మాణం object_array_నమోదు *blob[2];
	పూర్ణాంకం nongit = 0, no_సూచీ = 0;
	పూర్ణాంకం ఫలితం;
	నిర్మాణం symdiff sdiff;

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
	కొరకు (i = 1; i < argc; i++) {
		ఒకవేళ (!strcmp(argv[i], "--")) {
			i++;
			ఆపు;
		}
		ఒకవేళ (!strcmp(argv[i], "--no-index"))
			no_సూచీ = DIFF_NO_సూచీ_EXPLICIT;
		ఒకవేళ (argv[i][0] != '-')
			ఆపు;
	}

	ఉపసర్గ = అమరిక_git_directory_gently(the_repository, &nongit);

	ఒకవేళ (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_నిండిన_సూచీ = 0;
	}

	ఒకవేళ (!no_సూచీ) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		ఒకవేళ (nongit || ((argc == i + 2) &&
			       (!మార్గం_inside_repo(the_repository, ఉపసర్గ, argv[i]) ||
				!మార్గం_inside_repo(the_repository, ఉపసర్గ, argv[i + 1]))))
			no_సూచీ = DIFF_NO_సూచీ_IMPLICIT;
	}

	/*
	 * When operating outside of a Git repository we need to have a hash
	 * algorithm at hand so that we can generate the blob hashes. We
	 * default to SHA1 here, but may eventually want to change this to be
	 * configurable via a command line option.
	 */
	ఒకవేళ (nongit)
		repo_సమితి_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ఖాళీ);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	ఒకవేళ (no_సూచీ && ఉపసర్గ) {
		ఒకవేళ (chdir(ఉపసర్గ))
			die(_("cannot come back to cwd"));
		ఉపసర్గ = ఖాళీ;
	}

	ఉపసర్గ = precompose_argv_ఉపసర్గ(argc, argv, ఉపసర్గ);

	repo_init_revisions(the_repository, &rev, ఉపసర్గ);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	ఒకవేళ (no_సూచీ)
		నిష్క్రమించు(diff_no_సూచీ(&rev, the_repository->hash_algo,
				   no_సూచీ == DIFF_NO_సూచీ_IMPLICIT,
				   argc, argv));


	/*
	 * Otherwise, we are doing the usual "git" diff; set up any
	 * further defaults that apply to regular diffs.
	 */
	rev.diffopt.skip_stat_unmatch = !!diff_auto_refresh_సూచీ;

	/*
	 * Default to intent-to-add entries invisible in the
	 * index. This makes them show up as new files in diff-files
	 * and not at all in diff-cached.
	 */
	rev.diffopt.ita_invisible_లోపల_సూచీ = 1;

	ఒకవేళ (nongit)
		die(_("Not a git repository"));
	argc = అమరిక_revisions(argc, argv, &rev, ఖాళీ);
	ఒకవేళ (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_అమరిక_పూర్తి(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	rev.diffopt.rotate_గమ్యం_strict = 1;

	అమరిక_diff_pager(&rev.diffopt);

	/*
	 * Do we have --cached and not have a pending object, then
	 * default to HEAD by hand.  Eek.
	 */
	ఒకవేళ (!rev.pending.nr) {
		పూర్ణాంకం i;
		కొరకు (i = 1; i < argc; i++) {
			స్థిరరాశి అక్షరం *arg = argv[i];
			ఒకవేళ (!strcmp(arg, "--"))
				ఆపు;
			లేకపోతే ఒకవేళ (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				కూడు_తల_గమ్యం_pending(&rev);
				ఒకవేళ (!rev.pending.nr) {
					నిర్మాణం వృక్షం *వృక్షం;
					వృక్షం = lookup_వృక్షం(the_repository,
							   the_repository->hash_algo->ఖాళీలు_వృక్షం);
					కూడు_pending_object(&rev, &వృక్షం->object, "HEAD");
				}
				ఆపు;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	కొరకు (i = 0; i < rev.pending.nr; i++) {
		నిర్మాణం object_array_నమోదు *నమోదు_2 = &rev.pending.objects[i];
		నిర్మాణం object *obj = నమోదు_2->అంశం;
		స్థిరరాశి అక్షరం *name = నమోదు_2->name;
		పూర్ణాంకం flags = (obj->flags & UNINTERESTING);
		ఒకవేళ (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_ట్యాగ్(the_repository, obj, ఖాళీ, 0);
		ఒకవేళ (!obj)
			die(_("invalid object '%s' given."), name);
		ఒకవేళ (obj->type == OBJ_COMMIT) {
			నిర్మాణం వృక్షం *వృక్షం = repo_get_commit_వృక్షం(
				the_repository, (నిర్మాణం commit *)obj);
			ఒకవేళ (!వృక్షం)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &వృక్షం->object;
		}

		ఒకవేళ (obj->type == OBJ_వృక్షం) {
			ఒకవేళ (sdiff.skip && bitmap_get(sdiff.skip, i))
				కొనసాగు;
			obj->flags |= flags;
			కూడు_object_array(obj, name, &ent);
			ఒకవేళ (మొదటి_non_మాతృ < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				మొదటి_non_మాతృ = ent.nr - 1;
		} లేకపోతే ఒకవేళ (obj->type == OBJ_BLOB) {
			ఒకవేళ (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = నమోదు_2;
			blobs++;

		} లేకపోతే {
			die(_("unhandled object '%s' given."), name);
		}
	}
	ఒకవేళ (rev.prune_దత్తాంశం.nr)
		paths += rev.prune_దత్తాంశం.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	ఒకవేళ (!ent.nr) {
		ఎంపిక (blobs) {
		స్థితి 0:
			builtin_diff_files(&rev, argc, argv);
			ఆపు;
		స్థితి 1:
			ఒకవేళ (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			ఆపు;
		స్థితి 2:
			ఒకవేళ (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			ఆపు;
		అప్రమేయ:
			usage(builtin_diff_usage);
		}
	}
	లేకపోతే ఒకవేళ (blobs)
		usage(builtin_diff_usage);
	లేకపోతే ఒకవేళ (ent.nr == 1)
		builtin_diff_సూచీ(&rev, argc, argv);
	లేకపోతే ఒకవేళ (ent.nr == 2) {
		ఒకవేళ (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.ఎడమ, sdiff.కుడి, sdiff.పునాది);
		builtin_diff_వృక్షం(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} లేకపోతే
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      మొదటి_non_మాతృ);
	ఫలితం = diff_ఫలితం_సంకేతం(&rev);
	ఒకవేళ (1 < rev.diffopt.skip_stat_unmatch)
		refresh_సూచీ_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	తిరిగి ఫలితం;
}
