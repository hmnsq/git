/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "ewah/ewok.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "color.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "diff-merges.h"
#অন্তর্ভাব "diffcore.h"
#অন্তর্ভাব "preload-index.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "log-tree.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "tree.h"

#define DIFF_NO_সূচক_EXPLICIT 1
#define DIFF_NO_সূচক_IMPLICIT 2

স্থির ধ্রুবক অক্ষর builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

স্থির ধ্রুবক অক্ষর *blob_path(কাঠামো object_array_entry *entry)
{
	ফেরত entry->path ? entry->path : entry->name;
}

স্থির শূন্য stuff_change(কাঠামো diff_options *opt,
			 unsigned old_mode, unsigned new_mode,
			 ধ্রুবক কাঠামো object_id *old_oid,
			 ধ্রুবক কাঠামো object_id *new_oid,
			 পূর্ণসংখ্যা old_oid_valid,
			 পূর্ণসংখ্যা new_oid_valid,
			 ধ্রুবক অক্ষর *old_path,
			 ধ্রুবক অক্ষর *new_path)
{
	কাঠামো diff_filespec *one, *two;

	যদি (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		ফেরত;

	যদি (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	যদি (opt->prefix &&
	    (strncmp(old_path, opt->prefix, opt->prefix_দৈর্ঘ্য) ||
	     strncmp(new_path, opt->prefix, opt->prefix_দৈর্ঘ্য)))
		ফেরত;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

স্থির শূন্য builtin_diff_b_f(কাঠামো rev_info *revs,
			     পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv UNUSED,
			     কাঠামো object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	কাঠামো stat st;
	ধ্রুবক অক্ষর *path;

	যদি (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_তথ্য, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_তথ্য.বস্তুসমূহ[0].match;

	যদি (lstat(path, &st))
		die_errno(_("failed to stat '%s'"), path);
	যদি (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
		die(_("'%s': not a regular file or symlink"), path);

	diff_set_mnemonic_prefix(&revs->diffopt, "o/", "w/");

	যদি (blob[0]->mode == S_IFINVALID)
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

স্থির শূন্য builtin_diff_blobs(কাঠামো rev_info *revs,
			       পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv UNUSED,
			       কাঠামো object_array_entry **blob)
{
	ধ্রুবক unsigned mode = canon_mode(S_IFREG | 0644);

	যদি (argc > 1)
		usage(builtin_diff_usage);

	যদি (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = mode;

	যদি (blob[1]->mode == S_IFINVALID)
		blob[1]->mode = mode;

	stuff_change(&revs->diffopt,
		     blob[0]->mode, blob[1]->mode,
		     &blob[0]->item->oid, &blob[1]->item->oid,
		     1, 1,
		     blob_path(blob[0]), blob_path(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

স্থির শূন্য builtin_diff_সূচক(কাঠামো rev_info *revs,
			       পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	unsigned পূর্ণসংখ্যা option = 0;
	যখন (1 < argc) {
		ধ্রুবক অক্ষর *arg = argv[1];
		যদি (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_সূচক_CACHED;
		অন্যথা যদি (!strcmp(arg, "--merge-base"))
			option |= DIFF_সূচক_MERGE_BASE;
		অন্যথা
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	যদি (revs->pending.nr != 1 ||
	    revs->max_গুনতি != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	যদি (!(option & DIFF_সূচক_CACHED)) {
		setup_work_বৃক্ষ(the_repository);
		যদি (repo_read_সূচক_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} অন্যথা যদি (repo_read_সূচক(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_সূচক(revs, option);
}

স্থির শূন্য builtin_diff_বৃক্ষ(কাঠামো rev_info *revs,
			      পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv,
			      কাঠামো object_array_entry *ent0,
			      কাঠামো object_array_entry *ent1)
{
	ধ্রুবক কাঠামো object_id *(oid[2]);
	কাঠামো object_id mb_oid;
	পূর্ণসংখ্যা merge_base = 0;

	যখন (1 < argc) {
		ধ্রুবক অক্ষর *arg = argv[1];
		যদি (!strcmp(arg, "--merge-base"))
			merge_base = 1;
		অন্যথা
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	যদি (merge_base) {
		diff_get_merge_base(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].item->oid;
	} অন্যথা {
		পূর্ণসংখ্যা swap = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		যদি (ent1->item->flags & UNINTERESTING)
			swap = 1;
		oid[swap] = &ent0->item->oid;
		oid[1 - swap] = &ent1->item->oid;
	}
	diff_বৃক্ষ_oid(oid[0], oid[1], "", &revs->diffopt);
	log_বৃক্ষ_diff_flush(revs);
}

স্থির শূন্য builtin_diff_combined(কাঠামো rev_info *revs,
				  পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv UNUSED,
				  কাঠামো object_array_entry *ent,
				  পূর্ণসংখ্যা ents, পূর্ণসংখ্যা first_non_parent)
{
	কাঠামো oid_array parents = OID_ARRAY_INIT;
	পূর্ণসংখ্যা i;

	যদি (argc > 1)
		usage(builtin_diff_usage);

	যদি (first_non_parent < 0)
		die(_("no merge given, only parents."));
	যদি (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	জন্য (i = 0; i < ents; i++) {
		যদি (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_বৃক্ষ_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

স্থির শূন্য refresh_সূচক_quietly(শূন্য)
{
	কাঠামো lock_file lock_file = LOCK_INIT;
	পূর্ণসংখ্যা fd;

	fd = repo_hold_locked_সূচক(the_repository, &lock_file, 0);
	যদি (fd < 0)
		ফেরত;
	discard_সূচক(the_repository->সূচক);
	repo_read_সূচক(the_repository);
	refresh_সূচক(the_repository->সূচক, REFRESH_QUIET|REFRESH_UNMERGED, ফাঁকা, ফাঁকা,
		      ফাঁকা);
	repo_update_সূচক_if_able(the_repository, &lock_file);
}

স্থির শূন্য builtin_diff_files(কাঠামো rev_info *revs, পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	unsigned পূর্ণসংখ্যা options = 0;

	যখন (1 < argc && argv[1][0] == '-') {
		যদি (!strcmp(argv[1], "--base"))
			revs->max_গুনতি = 1;
		অন্যথা যদি (!strcmp(argv[1], "--ours"))
			revs->max_গুনতি = 2;
		অন্যথা যদি (!strcmp(argv[1], "--theirs"))
			revs->max_গুনতি = 3;
		অন্যথা যদি (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		অন্যথা যদি (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		অন্যথা {
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
	যদি (revs->max_গুনতি == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(revs);

	setup_work_বৃক্ষ(the_repository);
	যদি (repo_read_সূচক_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

কাঠামো symdiff {
	কাঠামো bitmap *skip;
	পূর্ণসংখ্যা warn;
	ধ্রুবক অক্ষর *base, *বাম, *ডান;
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
স্থির শূন্য symdiff_prepare(কাঠামো rev_info *rev, কাঠামো symdiff *sym)
{
	পূর্ণসংখ্যা i, is_symdiff = 0, basecount = 0, othercount = 0;
	পূর্ণসংখ্যা lpos = -1, rpos = -1, basepos = -1;
	কাঠামো bitmap *map = ফাঁকা;

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
	জন্য (i = 0; i < rev->cmdline.nr; i++) {
		কাঠামো object *obj = rev->pending.objects[i].item;
		নির্বাচন (rev->cmdline.rev[i].whence) {
		ক্ষেত্র REV_CMD_MERGE_BASE:
			যদি (basepos < 0)
				basepos = i;
			basecount++;
			বিরতি;		/* do mark all bases */
		ক্ষেত্র REV_CMD_বাম:
			যদি (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			যদি (obj->flags & SYMMETRIC_বাম) {
				is_symdiff = 1;
				বিরতি;	/* do mark A */
			}
			চালু;
		ক্ষেত্র REV_CMD_ডান:
			যদি (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			চালু;	/* don't mark B */
		ক্ষেত্র REV_CMD_PARENTS_ONLY:
		ক্ষেত্র REV_CMD_REF:
		ক্ষেত্র REV_CMD_REV:
			othercount++;
			চালু;
		}
		যদি (!map)
			map = bitmap_new();
		bitmap_set(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	যদি (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	যদি (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = ফাঁকা;
		ফেরত;
	}

	sym->বাম = rev->pending.objects[lpos].name;
	sym->ডান = rev->pending.objects[rpos].name;
	যদি (basecount == 0)
		die(_("%s...%s: no merge base"), sym->বাম, sym->ডান);
	sym->base = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

স্থির শূন্য symdiff_release(কাঠামো symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

পূর্ণসংখ্যা cmd_diff(পূর্ণসংখ্যা argc,
	     ধ্রুবক অক্ষর **argv,
	     ধ্রুবক অক্ষর *prefix,
	     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা i;
	কাঠামো rev_info rev;
	কাঠামো object_array ent = OBJECT_ARRAY_INIT;
	পূর্ণসংখ্যা first_non_parent = -1;
	পূর্ণসংখ্যা blobs = 0, paths = 0;
	কাঠামো object_array_entry *blob[2];
	পূর্ণসংখ্যা nongit = 0, no_সূচক = 0;
	পূর্ণসংখ্যা ফলাফল;
	কাঠামো symdiff sdiff;

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
	জন্য (i = 1; i < argc; i++) {
		যদি (!strcmp(argv[i], "--")) {
			i++;
			বিরতি;
		}
		যদি (!strcmp(argv[i], "--no-index"))
			no_সূচক = DIFF_NO_সূচক_EXPLICIT;
		যদি (argv[i][0] != '-')
			বিরতি;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	যদি (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_সূচক = 0;
	}

	যদি (!no_সূচক) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		যদি (nongit || ((argc == i + 2) &&
			       (!path_inside_repo(the_repository, prefix, argv[i]) ||
				!path_inside_repo(the_repository, prefix, argv[i + 1]))))
			no_সূচক = DIFF_NO_সূচক_IMPLICIT;
	}

	/*
	 * When operating outside of a Git repository we need to have a hash
	 * algorithm at hand so that we can generate the blob hashes. We
	 * default to SHA1 here, but may eventually want to change this to be
	 * configurable via a command line option.
	 */
	যদি (nongit)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ফাঁকা);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	যদি (no_সূচক && prefix) {
		যদি (chdir(prefix))
			die(_("cannot come back to cwd"));
		prefix = ফাঁকা;
	}

	prefix = precompose_argv_prefix(argc, argv, prefix);

	repo_init_revisions(the_repository, &rev, prefix);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	যদি (no_সূচক)
		exit(diff_no_সূচক(&rev, the_repository->hash_algo,
				   no_সূচক == DIFF_NO_সূচক_IMPLICIT,
				   argc, argv));


	/*
	 * Otherwise, we are doing the usual "git" diff; set up any
	 * further defaults that apply to regular diffs.
	 */
	rev.diffopt.skip_stat_unmatch = !!diff_auto_refresh_সূচক;

	/*
	 * Default to intent-to-add entries invisible in the
	 * index. This makes them show up as new files in diff-files
	 * and not at all in diff-cached.
	 */
	rev.diffopt.ita_invisible_in_সূচক = 1;

	যদি (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, ফাঁকা);
	যদি (!rev.diffopt.output_format) {
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
	যদি (!rev.pending.nr) {
		পূর্ণসংখ্যা i;
		জন্য (i = 1; i < argc; i++) {
			ধ্রুবক অক্ষর *arg = argv[i];
			যদি (!strcmp(arg, "--"))
				বিরতি;
			অন্যথা যদি (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				add_শীর্ষ_to_pending(&rev);
				যদি (!rev.pending.nr) {
					কাঠামো বৃক্ষ *বৃক্ষ;
					বৃক্ষ = lookup_বৃক্ষ(the_repository,
							   the_repository->hash_algo->empty_বৃক্ষ);
					add_pending_object(&rev, &বৃক্ষ->object, "HEAD");
				}
				বিরতি;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	জন্য (i = 0; i < rev.pending.nr; i++) {
		কাঠামো object_array_entry *entry = &rev.pending.objects[i];
		কাঠামো object *obj = entry->item;
		ধ্রুবক অক্ষর *name = entry->name;
		পূর্ণসংখ্যা flags = (obj->flags & UNINTERESTING);
		যদি (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, ফাঁকা, 0);
		যদি (!obj)
			die(_("invalid object '%s' given."), name);
		যদি (obj->type == OBJ_COMMIT) {
			কাঠামো বৃক্ষ *বৃক্ষ = repo_get_commit_বৃক্ষ(
				the_repository, (কাঠামো commit *)obj);
			যদি (!বৃক্ষ)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &বৃক্ষ->object;
		}

		যদি (obj->type == OBJ_বৃক্ষ) {
			যদি (sdiff.skip && bitmap_get(sdiff.skip, i))
				চালু;
			obj->flags |= flags;
			add_object_array(obj, name, &ent);
			যদি (first_non_parent < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				first_non_parent = ent.nr - 1;
		} অন্যথা যদি (obj->type == OBJ_BLOB) {
			যদি (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = entry;
			blobs++;

		} অন্যথা {
			die(_("unhandled object '%s' given."), name);
		}
	}
	যদি (rev.prune_তথ্য.nr)
		paths += rev.prune_তথ্য.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	যদি (!ent.nr) {
		নির্বাচন (blobs) {
		ক্ষেত্র 0:
			builtin_diff_files(&rev, argc, argv);
			বিরতি;
		ক্ষেত্র 1:
			যদি (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			বিরতি;
		ক্ষেত্র 2:
			যদি (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			বিরতি;
		পূর্বনির্ধারিত:
			usage(builtin_diff_usage);
		}
	}
	অন্যথা যদি (blobs)
		usage(builtin_diff_usage);
	অন্যথা যদি (ent.nr == 1)
		builtin_diff_সূচক(&rev, argc, argv);
	অন্যথা যদি (ent.nr == 2) {
		যদি (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.বাম, sdiff.ডান, sdiff.base);
		builtin_diff_বৃক্ষ(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} অন্যথা
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      first_non_parent);
	ফলাফল = diff_ফলাফল_code(&rev);
	যদি (1 < rev.diffopt.skip_stat_unmatch)
		refresh_সূচক_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	ফেরত ফলাফল;
}
