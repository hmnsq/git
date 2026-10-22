/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "ewah/ewok.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "color.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "diff-merges.h"
#ଅନ୍ତର୍ଭାଵ "diffcore.h"
#ଅନ୍ତର୍ଭାଵ "preload-index.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "log-tree.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"

#define DIFF_NO_INDEX_EXPLICIT 1
#define DIFF_NO_INDEX_IMPLICIT 2

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *blob_path(ସଂରଚନା object_array_entry *entry)
{
	ଫେରନ୍ତୁ entry->path ? entry->path : entry->name;
}

ସ୍ଥିର ଶୂନ୍ୟ stuff_change(ସଂରଚନା diff_options *opt,
			 unsigned old_mode, unsigned new_mode,
			 ଅଚଳ ସଂରଚନା object_id *old_oid,
			 ଅଚଳ ସଂରଚନା object_id *new_oid,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_oid_valid,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_oid_valid,
			 ଅଚଳ ଅକ୍ଷର *old_path,
			 ଅଚଳ ଅକ୍ଷର *new_path)
{
	ସଂରଚନା diff_filespec *one, *two;

	ଯଦି (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		ଫେରନ୍ତୁ;

	ଯଦି (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	ଯଦି (opt->prefix &&
	    (strncmp(old_path, opt->prefix, opt->prefix_length) ||
	     strncmp(new_path, opt->prefix, opt->prefix_length)))
		ଫେରନ୍ତୁ;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

ସ୍ଥିର ଶୂନ୍ୟ builtin_diff_b_f(ସଂରଚନା rev_info *revs,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv UNUSED,
			     ସଂରଚନା object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	ସଂରଚନା stat st;
	ଅଚଳ ଅକ୍ଷର *path;

	ଯଦି (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_data, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_data.ବସ୍ତୁଗୁଡ଼ିକ[0].match;

	ଯଦି (lstat(path, &st))
		die_errno(_("failed to stat '%s'"), path);
	ଯଦି (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
		die(_("'%s': not a regular file or symlink"), path);

	diff_set_mnemonic_prefix(&revs->diffopt, "o/", "w/");

	ଯଦି (blob[0]->mode == S_IFINVALID)
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

ସ୍ଥିର ଶୂନ୍ୟ builtin_diff_blobs(ସଂରଚନା rev_info *revs,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv UNUSED,
			       ସଂରଚନା object_array_entry **blob)
{
	ଅଚଳ unsigned mode = canon_mode(S_IFREG | 0644);

	ଯଦି (argc > 1)
		usage(builtin_diff_usage);

	ଯଦି (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = mode;

	ଯଦି (blob[1]->mode == S_IFINVALID)
		blob[1]->mode = mode;

	stuff_change(&revs->diffopt,
		     blob[0]->mode, blob[1]->mode,
		     &blob[0]->item->oid, &blob[1]->item->oid,
		     1, 1,
		     blob_path(blob[0]), blob_path(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ସ୍ଥିର ଶୂନ୍ୟ builtin_diff_index(ସଂରଚନା rev_info *revs,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା option = 0;
	ଯେତେବେଳେ (1 < argc) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[1];
		ଯଦି (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_INDEX_CACHED;
		ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		ଅନ୍ୟଥା
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	ଯଦି (revs->pending.nr != 1 ||
	    revs->max_ଗଣନା != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	ଯଦି (!(option & DIFF_INDEX_CACHED)) {
		setup_work_ବୃକ୍ଷ(the_repository);
		ଯଦି (repo_read_index_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} ଅନ୍ୟଥା ଯଦି (repo_read_index(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_index(revs, option);
}

ସ୍ଥିର ଶୂନ୍ୟ builtin_diff_ବୃକ୍ଷ(ସଂରଚନା rev_info *revs,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
			      ସଂରଚନା object_array_entry *ent0,
			      ସଂରଚନା object_array_entry *ent1)
{
	ଅଚଳ ସଂରଚନା object_id *(oid[2]);
	ସଂରଚନା object_id mb_oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge_base = 0;

	ଯେତେବେଳେ (1 < argc) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[1];
		ଯଦି (!strcmp(arg, "--merge-base"))
			merge_base = 1;
		ଅନ୍ୟଥା
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	ଯଦି (merge_base) {
		diff_get_merge_base(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].item->oid;
	} ଅନ୍ୟଥା {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା swap = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		ଯଦି (ent1->item->flags & UNINTERESTING)
			swap = 1;
		oid[swap] = &ent0->item->oid;
		oid[1 - swap] = &ent1->item->oid;
	}
	diff_ବୃକ୍ଷ_oid(oid[0], oid[1], "", &revs->diffopt);
	log_ବୃକ୍ଷ_diff_flush(revs);
}

ସ୍ଥିର ଶୂନ୍ୟ builtin_diff_combined(ସଂରଚନା rev_info *revs,
				  ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv UNUSED,
				  ସଂରଚନା object_array_entry *ent,
				  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ents, ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_non_parent)
{
	ସଂରଚନା oid_array parents = OID_ARRAY_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (argc > 1)
		usage(builtin_diff_usage);

	ଯଦି (first_non_parent < 0)
		die(_("no merge given, only parents."));
	ଯଦି (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	ପାଇଁ (i = 0; i < ents; i++) {
		ଯଦି (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_ବୃକ୍ଷ_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

ସ୍ଥିର ଶୂନ୍ୟ refresh_index_quietly(ଶୂନ୍ୟ)
{
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ;
	discard_index(the_repository->index);
	repo_read_index(the_repository);
	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, ଖାଲି, ଖାଲି,
		      ଖାଲି);
	repo_update_index_if_able(the_repository, &lock_file);
}

ସ୍ଥିର ଶୂନ୍ୟ builtin_diff_files(ସଂରଚନା rev_info *revs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା options = 0;

	ଯେତେବେଳେ (1 < argc && argv[1][0] == '-') {
		ଯଦି (!strcmp(argv[1], "--base"))
			revs->max_ଗଣନା = 1;
		ଅନ୍ୟଥା ଯଦି (!strcmp(argv[1], "--ours"))
			revs->max_ଗଣନା = 2;
		ଅନ୍ୟଥା ଯଦି (!strcmp(argv[1], "--theirs"))
			revs->max_ଗଣନା = 3;
		ଅନ୍ୟଥା ଯଦି (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		ଅନ୍ୟଥା ଯଦି (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		ଅନ୍ୟଥା {
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
	ଯଦି (revs->max_ଗଣନା == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(revs);

	setup_work_ବୃକ୍ଷ(the_repository);
	ଯଦି (repo_read_index_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

ସଂରଚନା symdiff {
	ସଂରଚନା bitmap *skip;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା warn;
	ଅଚଳ ଅକ୍ଷର *base, *ବାମ, *ଡାହାଣ;
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
ସ୍ଥିର ଶୂନ୍ୟ symdiff_prepare(ସଂରଚନା rev_info *rev, ସଂରଚନା symdiff *sym)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, is_symdiff = 0, basecount = 0, othercount = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା lpos = -1, rpos = -1, basepos = -1;
	ସଂରଚନା bitmap *map = ଖାଲି;

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
	ପାଇଁ (i = 0; i < rev->cmdline.nr; i++) {
		ସଂରଚନା object *obj = rev->pending.objects[i].item;
		ବାଛ (rev->cmdline.rev[i].whence) {
		ସ୍ଥିତି REV_CMD_MERGE_BASE:
			ଯଦି (basepos < 0)
				basepos = i;
			basecount++;
			ଭାଙ୍ଗ;		/* do mark all bases */
		ସ୍ଥିତି REV_CMD_ବାମ:
			ଯଦି (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			ଯଦି (obj->flags & SYMMETRIC_ବାମ) {
				is_symdiff = 1;
				ଭାଙ୍ଗ;	/* do mark A */
			}
			ଜାରି;
		ସ୍ଥିତି REV_CMD_ଡାହାଣ:
			ଯଦି (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			ଜାରି;	/* don't mark B */
		ସ୍ଥିତି REV_CMD_PARENTS_ONLY:
		ସ୍ଥିତି REV_CMD_REF:
		ସ୍ଥିତି REV_CMD_REV:
			othercount++;
			ଜାରି;
		}
		ଯଦି (!map)
			map = bitmap_new();
		bitmap_set(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	ଯଦି (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	ଯଦି (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = ଖାଲି;
		ଫେରନ୍ତୁ;
	}

	sym->ବାମ = rev->pending.objects[lpos].name;
	sym->ଡାହାଣ = rev->pending.objects[rpos].name;
	ଯଦି (basecount == 0)
		die(_("%s...%s: no merge base"), sym->ବାମ, sym->ଡାହାଣ);
	sym->base = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

ସ୍ଥିର ଶୂନ୍ୟ symdiff_release(ସଂରଚନା symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_diff(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	     ଅଚଳ ଅକ୍ଷର **argv,
	     ଅଚଳ ଅକ୍ଷର *prefix,
	     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା rev_info rev;
	ସଂରଚନା object_array ent = OBJECT_ARRAY_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_non_parent = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା blobs = 0, paths = 0;
	ସଂରଚନା object_array_entry *blob[2];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nongit = 0, no_index = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;
	ସଂରଚନା symdiff sdiff;

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
	ପାଇଁ (i = 1; i < argc; i++) {
		ଯଦି (!strcmp(argv[i], "--")) {
			i++;
			ଭାଙ୍ଗ;
		}
		ଯଦି (!strcmp(argv[i], "--no-index"))
			no_index = DIFF_NO_INDEX_EXPLICIT;
		ଯଦି (argv[i][0] != '-')
			ଭାଙ୍ଗ;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	ଯଦି (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	ଯଦି (!no_index) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		ଯଦି (nongit || ((argc == i + 2) &&
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
	ଯଦି (nongit)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ଖାଲି);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	ଯଦି (no_index && prefix) {
		ଯଦି (chdir(prefix))
			die(_("cannot come back to cwd"));
		prefix = ଖାଲି;
	}

	prefix = precompose_argv_prefix(argc, argv, prefix);

	repo_init_revisions(the_repository, &rev, prefix);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	ଯଦି (no_index)
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

	ଯଦି (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, ଖାଲି);
	ଯଦି (!rev.diffopt.output_format) {
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
	ଯଦି (!rev.pending.nr) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
		ପାଇଁ (i = 1; i < argc; i++) {
			ଅଚଳ ଅକ୍ଷର *arg = argv[i];
			ଯଦି (!strcmp(arg, "--"))
				ଭାଙ୍ଗ;
			ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				add_head_to_pending(&rev);
				ଯଦି (!rev.pending.nr) {
					ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;
					ବୃକ୍ଷ = lookup_ବୃକ୍ଷ(the_repository,
							   the_repository->hash_algo->empty_ବୃକ୍ଷ);
					add_pending_object(&rev, &ବୃକ୍ଷ->object, "HEAD");
				}
				ଭାଙ୍ଗ;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	ପାଇଁ (i = 0; i < rev.pending.nr; i++) {
		ସଂରଚନା object_array_entry *entry = &rev.pending.objects[i];
		ସଂରଚନା object *obj = entry->item;
		ଅଚଳ ଅକ୍ଷର *name = entry->name;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = (obj->flags & UNINTERESTING);
		ଯଦି (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, ଖାଲି, 0);
		ଯଦି (!obj)
			die(_("invalid object '%s' given."), name);
		ଯଦି (obj->type == OBJ_COMMIT) {
			ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ = repo_get_commit_ବୃକ୍ଷ(
				the_repository, (ସଂରଚନା commit *)obj);
			ଯଦି (!ବୃକ୍ଷ)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &ବୃକ୍ଷ->object;
		}

		ଯଦି (obj->type == OBJ_ବୃକ୍ଷ) {
			ଯଦି (sdiff.skip && bitmap_get(sdiff.skip, i))
				ଜାରି;
			obj->flags |= flags;
			add_object_array(obj, name, &ent);
			ଯଦି (first_non_parent < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				first_non_parent = ent.nr - 1;
		} ଅନ୍ୟଥା ଯଦି (obj->type == OBJ_BLOB) {
			ଯଦି (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = entry;
			blobs++;

		} ଅନ୍ୟଥା {
			die(_("unhandled object '%s' given."), name);
		}
	}
	ଯଦି (rev.prune_data.nr)
		paths += rev.prune_data.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	ଯଦି (!ent.nr) {
		ବାଛ (blobs) {
		ସ୍ଥିତି 0:
			builtin_diff_files(&rev, argc, argv);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 1:
			ଯଦି (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 2:
			ଯଦି (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			usage(builtin_diff_usage);
		}
	}
	ଅନ୍ୟଥା ଯଦି (blobs)
		usage(builtin_diff_usage);
	ଅନ୍ୟଥା ଯଦି (ent.nr == 1)
		builtin_diff_index(&rev, argc, argv);
	ଅନ୍ୟଥା ଯଦି (ent.nr == 2) {
		ଯଦି (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.ବାମ, sdiff.ଡାହାଣ, sdiff.base);
		builtin_diff_ବୃକ୍ଷ(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} ଅନ୍ୟଥା
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      first_non_parent);
	result = diff_result_code(&rev);
	ଯଦି (1 < rev.diffopt.skip_stat_unmatch)
		refresh_index_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	ଫେରନ୍ତୁ result;
}
