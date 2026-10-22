/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "ewah/ewok.h"
#شامل "lockfile.h"
#شامل "color.h"
#شامل "commit.h"
#شامل "gettext.h"
#شامل "tag.h"
#شامل "diff.h"
#شامل "diff-merges.h"
#شامل "diffcore.h"
#شامل "preload-index.h"
#شامل "read-cache-ll.h"
#شامل "revision.h"
#شامل "log-tree.h"
#شامل "setup.h"
#شامل "oid-array.h"
#شامل "tree.h"

#define DIFF_NO_INDEX_EXPLICIT 1
#define DIFF_NO_INDEX_IMPLICIT 2

ایستا ثابت نویسه builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ایستا ثابت نویسه *blob_path(ساختار object_array_entry *entry)
{
	بازگشت entry->path ? entry->path : entry->name;
}

ایستا تهی stuff_change(ساختار diff_options *opt,
			 unsigned old_mode, unsigned new_mode,
			 ثابت ساختار object_id *old_oid,
			 ثابت ساختار object_id *new_oid,
			 صحیح old_oid_valid,
			 صحیح new_oid_valid,
			 ثابت نویسه *old_path,
			 ثابت نویسه *new_path)
{
	ساختار diff_filespec *one, *two;

	اگر (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		بازگشت;

	اگر (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	اگر (opt->prefix &&
	    (strncmp(old_path, opt->prefix, opt->prefix_length) ||
	     strncmp(new_path, opt->prefix, opt->prefix_length)))
		بازگشت;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

ایستا تهی builtin_diff_b_f(ساختار rev_info *revs,
			     صحیح argc, ثابت نویسه **argv UNUSED,
			     ساختار object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	ساختار stat st;
	ثابت نویسه *path;

	اگر (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_data, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_data.اقلام[0].match;

	اگر (lstat(path, &st))
		die_errno(_("failed to stat '%s'"), path);
	اگر (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
		die(_("'%s': not a regular file or symlink"), path);

	diff_set_mnemonic_prefix(&revs->diffopt, "o/", "w/");

	اگر (blob[0]->mode == S_IFINVALID)
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

ایستا تهی builtin_diff_blobs(ساختار rev_info *revs,
			       صحیح argc, ثابت نویسه **argv UNUSED,
			       ساختار object_array_entry **blob)
{
	ثابت unsigned mode = canon_mode(S_IFREG | 0644);

	اگر (argc > 1)
		usage(builtin_diff_usage);

	اگر (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = mode;

	اگر (blob[1]->mode == S_IFINVALID)
		blob[1]->mode = mode;

	stuff_change(&revs->diffopt,
		     blob[0]->mode, blob[1]->mode,
		     &blob[0]->item->oid, &blob[1]->item->oid,
		     1, 1,
		     blob_path(blob[0]), blob_path(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ایستا تهی builtin_diff_index(ساختار rev_info *revs,
			       صحیح argc, ثابت نویسه **argv)
{
	unsigned صحیح option = 0;
	وقتی (1 < argc) {
		ثابت نویسه *arg = argv[1];
		اگر (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_INDEX_CACHED;
		وگرنه اگر (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		وگرنه
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	اگر (revs->pending.nr != 1 ||
	    revs->max_شمار != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	اگر (!(option & DIFF_INDEX_CACHED)) {
		setup_work_درخت(the_repository);
		اگر (repo_read_index_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} وگرنه اگر (repo_read_index(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_index(revs, option);
}

ایستا تهی builtin_diff_درخت(ساختار rev_info *revs,
			      صحیح argc, ثابت نویسه **argv,
			      ساختار object_array_entry *ent0,
			      ساختار object_array_entry *ent1)
{
	ثابت ساختار object_id *(oid[2]);
	ساختار object_id mb_oid;
	صحیح merge_base = 0;

	وقتی (1 < argc) {
		ثابت نویسه *arg = argv[1];
		اگر (!strcmp(arg, "--merge-base"))
			merge_base = 1;
		وگرنه
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	اگر (merge_base) {
		diff_get_merge_base(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].item->oid;
	} وگرنه {
		صحیح swap = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		اگر (ent1->item->flags & UNINTERESTING)
			swap = 1;
		oid[swap] = &ent0->item->oid;
		oid[1 - swap] = &ent1->item->oid;
	}
	diff_درخت_oid(oid[0], oid[1], "", &revs->diffopt);
	log_درخت_diff_flush(revs);
}

ایستا تهی builtin_diff_combined(ساختار rev_info *revs,
				  صحیح argc, ثابت نویسه **argv UNUSED,
				  ساختار object_array_entry *ent,
				  صحیح ents, صحیح first_non_parent)
{
	ساختار oid_array parents = OID_ARRAY_INIT;
	صحیح i;

	اگر (argc > 1)
		usage(builtin_diff_usage);

	اگر (first_non_parent < 0)
		die(_("no merge given, only parents."));
	اگر (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	برای (i = 0; i < ents; i++) {
		اگر (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_درخت_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

ایستا تهی refresh_index_quietly(تهی)
{
	ساختار lock_file lock_file = LOCK_INIT;
	صحیح fd;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	اگر (fd < 0)
		بازگشت;
	discard_index(the_repository->index);
	repo_read_index(the_repository);
	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, پوچ, پوچ,
		      پوچ);
	repo_update_index_if_able(the_repository, &lock_file);
}

ایستا تهی builtin_diff_files(ساختار rev_info *revs, صحیح argc, ثابت نویسه **argv)
{
	unsigned صحیح options = 0;

	وقتی (1 < argc && argv[1][0] == '-') {
		اگر (!strcmp(argv[1], "--base"))
			revs->max_شمار = 1;
		وگرنه اگر (!strcmp(argv[1], "--ours"))
			revs->max_شمار = 2;
		وگرنه اگر (!strcmp(argv[1], "--theirs"))
			revs->max_شمار = 3;
		وگرنه اگر (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		وگرنه اگر (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		وگرنه {
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
	اگر (revs->max_شمار == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(revs);

	setup_work_درخت(the_repository);
	اگر (repo_read_index_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

ساختار symdiff {
	ساختار bitmap *skip;
	صحیح warn;
	ثابت نویسه *base, *چپ, *راست;
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
ایستا تهی symdiff_prepare(ساختار rev_info *rev, ساختار symdiff *sym)
{
	صحیح i, is_symdiff = 0, basecount = 0, othercount = 0;
	صحیح lpos = -1, rpos = -1, basepos = -1;
	ساختار bitmap *map = پوچ;

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
	برای (i = 0; i < rev->cmdline.nr; i++) {
		ساختار object *obj = rev->pending.objects[i].item;
		انتخاب (rev->cmdline.rev[i].whence) {
		حالت REV_CMD_MERGE_BASE:
			اگر (basepos < 0)
				basepos = i;
			basecount++;
			بشکن;		/* do mark all bases */
		حالت REV_CMD_چپ:
			اگر (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			اگر (obj->flags & SYMMETRIC_چپ) {
				is_symdiff = 1;
				بشکن;	/* do mark A */
			}
			ادامه;
		حالت REV_CMD_راست:
			اگر (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			ادامه;	/* don't mark B */
		حالت REV_CMD_PARENTS_ONLY:
		حالت REV_CMD_REF:
		حالت REV_CMD_REV:
			othercount++;
			ادامه;
		}
		اگر (!map)
			map = bitmap_new();
		bitmap_set(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	اگر (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	اگر (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = پوچ;
		بازگشت;
	}

	sym->چپ = rev->pending.objects[lpos].name;
	sym->راست = rev->pending.objects[rpos].name;
	اگر (basecount == 0)
		die(_("%s...%s: no merge base"), sym->چپ, sym->راست);
	sym->base = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

ایستا تهی symdiff_release(ساختار symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

صحیح cmd_diff(صحیح argc,
	     ثابت نویسه **argv,
	     ثابت نویسه *prefix,
	     ساختار repository *repo UNUSED)
{
	صحیح i;
	ساختار rev_info rev;
	ساختار object_array ent = OBJECT_ARRAY_INIT;
	صحیح first_non_parent = -1;
	صحیح blobs = 0, paths = 0;
	ساختار object_array_entry *blob[2];
	صحیح nongit = 0, no_index = 0;
	صحیح result;
	ساختار symdiff sdiff;

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
	برای (i = 1; i < argc; i++) {
		اگر (!strcmp(argv[i], "--")) {
			i++;
			بشکن;
		}
		اگر (!strcmp(argv[i], "--no-index"))
			no_index = DIFF_NO_INDEX_EXPLICIT;
		اگر (argv[i][0] != '-')
			بشکن;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	اگر (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	اگر (!no_index) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		اگر (nongit || ((argc == i + 2) &&
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
	اگر (nongit)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, پوچ);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	اگر (no_index && prefix) {
		اگر (chdir(prefix))
			die(_("cannot come back to cwd"));
		prefix = پوچ;
	}

	prefix = precompose_argv_prefix(argc, argv, prefix);

	repo_init_revisions(the_repository, &rev, prefix);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	اگر (no_index)
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

	اگر (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, پوچ);
	اگر (!rev.diffopt.output_format) {
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
	اگر (!rev.pending.nr) {
		صحیح i;
		برای (i = 1; i < argc; i++) {
			ثابت نویسه *arg = argv[i];
			اگر (!strcmp(arg, "--"))
				بشکن;
			وگرنه اگر (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				add_head_to_pending(&rev);
				اگر (!rev.pending.nr) {
					ساختار درخت *درخت;
					درخت = lookup_درخت(the_repository,
							   the_repository->hash_algo->empty_درخت);
					add_pending_object(&rev, &درخت->object, "HEAD");
				}
				بشکن;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	برای (i = 0; i < rev.pending.nr; i++) {
		ساختار object_array_entry *entry = &rev.pending.objects[i];
		ساختار object *obj = entry->item;
		ثابت نویسه *name = entry->name;
		صحیح flags = (obj->flags & UNINTERESTING);
		اگر (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, پوچ, 0);
		اگر (!obj)
			die(_("invalid object '%s' given."), name);
		اگر (obj->type == OBJ_COMMIT) {
			ساختار درخت *درخت = repo_get_commit_درخت(
				the_repository, (ساختار commit *)obj);
			اگر (!درخت)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &درخت->object;
		}

		اگر (obj->type == OBJ_درخت) {
			اگر (sdiff.skip && bitmap_get(sdiff.skip, i))
				ادامه;
			obj->flags |= flags;
			add_object_array(obj, name, &ent);
			اگر (first_non_parent < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				first_non_parent = ent.nr - 1;
		} وگرنه اگر (obj->type == OBJ_BLOB) {
			اگر (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = entry;
			blobs++;

		} وگرنه {
			die(_("unhandled object '%s' given."), name);
		}
	}
	اگر (rev.prune_data.nr)
		paths += rev.prune_data.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	اگر (!ent.nr) {
		انتخاب (blobs) {
		حالت 0:
			builtin_diff_files(&rev, argc, argv);
			بشکن;
		حالت 1:
			اگر (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			بشکن;
		حالت 2:
			اگر (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			بشکن;
		پیشفرض:
			usage(builtin_diff_usage);
		}
	}
	وگرنه اگر (blobs)
		usage(builtin_diff_usage);
	وگرنه اگر (ent.nr == 1)
		builtin_diff_index(&rev, argc, argv);
	وگرنه اگر (ent.nr == 2) {
		اگر (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.چپ, sdiff.راست, sdiff.base);
		builtin_diff_درخت(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} وگرنه
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      first_non_parent);
	result = diff_result_code(&rev);
	اگر (1 < rev.diffopt.skip_stat_unmatch)
		refresh_index_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	بازگشت result;
}
