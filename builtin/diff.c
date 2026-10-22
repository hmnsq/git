/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "ewah/ewok.h"
#ضمن "lockfile.h"
#ضمن "color.h"
#ضمن "commit.h"
#ضمن "gettext.h"
#ضمن "tag.h"
#ضمن "diff.h"
#ضمن "diff-merges.h"
#ضمن "diffcore.h"
#ضمن "preload-index.h"
#ضمن "read-cache-ll.h"
#ضمن "revision.h"
#ضمن "log-tree.h"
#ضمن "setup.h"
#ضمن "oid-array.h"
#ضمن "tree.h"

#define DIFF_NO_INDEX_EXPLICIT 1
#define DIFF_NO_INDEX_IMPLICIT 2

ساكن ثابت حرف builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ساكن ثابت حرف *blob_path(بنية object_array_entry *entry)
{
	أرجع entry->path ? entry->path : entry->name;
}

ساكن فراغ stuff_change(بنية diff_options *opt,
			 unsigned old_mode, unsigned new_mode,
			 ثابت بنية object_id *old_oid,
			 ثابت بنية object_id *new_oid,
			 صحيح old_oid_valid,
			 صحيح new_oid_valid,
			 ثابت حرف *old_path,
			 ثابت حرف *new_path)
{
	بنية diff_filespec *one, *two;

	إذا (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		أرجع;

	إذا (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	إذا (opt->prefix &&
	    (strncmp(old_path, opt->prefix, opt->prefix_length) ||
	     strncmp(new_path, opt->prefix, opt->prefix_length)))
		أرجع;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

ساكن فراغ builtin_diff_b_f(بنية rev_info *revs,
			     صحيح argc, ثابت حرف **argv UNUSED,
			     بنية object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	بنية stat st;
	ثابت حرف *path;

	إذا (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_data, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_data.کاڵاکان[0].match;

	إذا (lstat(path, &st))
		die_errno(_("failed to stat '%s'"), path);
	إذا (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
		die(_("'%s': not a regular file or symlink"), path);

	diff_set_mnemonic_prefix(&revs->diffopt, "o/", "w/");

	إذا (blob[0]->mode == S_IFINVALID)
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

ساكن فراغ builtin_diff_blobs(بنية rev_info *revs,
			       صحيح argc, ثابت حرف **argv UNUSED,
			       بنية object_array_entry **blob)
{
	ثابت unsigned mode = canon_mode(S_IFREG | 0644);

	إذا (argc > 1)
		usage(builtin_diff_usage);

	إذا (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = mode;

	إذا (blob[1]->mode == S_IFINVALID)
		blob[1]->mode = mode;

	stuff_change(&revs->diffopt,
		     blob[0]->mode, blob[1]->mode,
		     &blob[0]->item->oid, &blob[1]->item->oid,
		     1, 1,
		     blob_path(blob[0]), blob_path(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ساكن فراغ builtin_diff_index(بنية rev_info *revs,
			       صحيح argc, ثابت حرف **argv)
{
	unsigned صحيح option = 0;
	طالما (1 < argc) {
		ثابت حرف *arg = argv[1];
		إذا (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_INDEX_CACHED;
		وإلا إذا (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		وإلا
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	إذا (revs->pending.nr != 1 ||
	    revs->max_ژماردن != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	إذا (!(option & DIFF_INDEX_CACHED)) {
		setup_work_درەخت(the_repository);
		إذا (repo_read_index_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} وإلا إذا (repo_read_index(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_index(revs, option);
}

ساكن فراغ builtin_diff_درەخت(بنية rev_info *revs,
			      صحيح argc, ثابت حرف **argv,
			      بنية object_array_entry *ent0,
			      بنية object_array_entry *ent1)
{
	ثابت بنية object_id *(oid[2]);
	بنية object_id mb_oid;
	صحيح merge_base = 0;

	طالما (1 < argc) {
		ثابت حرف *arg = argv[1];
		إذا (!strcmp(arg, "--merge-base"))
			merge_base = 1;
		وإلا
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	إذا (merge_base) {
		diff_get_merge_base(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].item->oid;
	} وإلا {
		صحيح swap = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		إذا (ent1->item->flags & UNINTERESTING)
			swap = 1;
		oid[swap] = &ent0->item->oid;
		oid[1 - swap] = &ent1->item->oid;
	}
	diff_درەخت_oid(oid[0], oid[1], "", &revs->diffopt);
	log_درەخت_diff_flush(revs);
}

ساكن فراغ builtin_diff_combined(بنية rev_info *revs,
				  صحيح argc, ثابت حرف **argv UNUSED,
				  بنية object_array_entry *ent,
				  صحيح ents, صحيح first_non_parent)
{
	بنية oid_array parents = OID_ARRAY_INIT;
	صحيح i;

	إذا (argc > 1)
		usage(builtin_diff_usage);

	إذا (first_non_parent < 0)
		die(_("no merge given, only parents."));
	إذا (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	لكل (i = 0; i < ents; i++) {
		إذا (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_درەخت_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

ساكن فراغ refresh_index_quietly(فراغ)
{
	بنية lock_file lock_file = LOCK_INIT;
	صحيح fd;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	إذا (fd < 0)
		أرجع;
	discard_index(the_repository->index);
	repo_read_index(the_repository);
	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, عدم, عدم,
		      عدم);
	repo_update_index_if_able(the_repository, &lock_file);
}

ساكن فراغ builtin_diff_files(بنية rev_info *revs, صحيح argc, ثابت حرف **argv)
{
	unsigned صحيح options = 0;

	طالما (1 < argc && argv[1][0] == '-') {
		إذا (!strcmp(argv[1], "--base"))
			revs->max_ژماردن = 1;
		وإلا إذا (!strcmp(argv[1], "--ours"))
			revs->max_ژماردن = 2;
		وإلا إذا (!strcmp(argv[1], "--theirs"))
			revs->max_ژماردن = 3;
		وإلا إذا (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		وإلا إذا (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		وإلا {
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
	إذا (revs->max_ژماردن == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(revs);

	setup_work_درەخت(the_repository);
	إذا (repo_read_index_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

بنية symdiff {
	بنية bitmap *skip;
	صحيح warn;
	ثابت حرف *base, *چەپ, *ڕاست;
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
ساكن فراغ symdiff_prepare(بنية rev_info *rev, بنية symdiff *sym)
{
	صحيح i, is_symdiff = 0, basecount = 0, othercount = 0;
	صحيح lpos = -1, rpos = -1, basepos = -1;
	بنية bitmap *map = عدم;

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
	لكل (i = 0; i < rev->cmdline.nr; i++) {
		بنية object *obj = rev->pending.objects[i].item;
		بدل (rev->cmdline.rev[i].whence) {
		حالة REV_CMD_MERGE_BASE:
			إذا (basepos < 0)
				basepos = i;
			basecount++;
			اكسر;		/* do mark all bases */
		حالة REV_CMD_چەپ:
			إذا (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			إذا (obj->flags & SYMMETRIC_چەپ) {
				is_symdiff = 1;
				اكسر;	/* do mark A */
			}
			استمر;
		حالة REV_CMD_ڕاست:
			إذا (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			استمر;	/* don't mark B */
		حالة REV_CMD_PARENTS_ONLY:
		حالة REV_CMD_REF:
		حالة REV_CMD_REV:
			othercount++;
			استمر;
		}
		إذا (!map)
			map = bitmap_new();
		bitmap_set(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	إذا (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	إذا (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = عدم;
		أرجع;
	}

	sym->چەپ = rev->pending.objects[lpos].name;
	sym->ڕاست = rev->pending.objects[rpos].name;
	إذا (basecount == 0)
		die(_("%s...%s: no merge base"), sym->چەپ, sym->ڕاست);
	sym->base = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

ساكن فراغ symdiff_release(بنية symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

صحيح cmd_diff(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *prefix,
	     بنية repository *repo UNUSED)
{
	صحيح i;
	بنية rev_info rev;
	بنية object_array ent = OBJECT_ARRAY_INIT;
	صحيح first_non_parent = -1;
	صحيح blobs = 0, paths = 0;
	بنية object_array_entry *blob[2];
	صحيح nongit = 0, no_index = 0;
	صحيح result;
	بنية symdiff sdiff;

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
	لكل (i = 1; i < argc; i++) {
		إذا (!strcmp(argv[i], "--")) {
			i++;
			اكسر;
		}
		إذا (!strcmp(argv[i], "--no-index"))
			no_index = DIFF_NO_INDEX_EXPLICIT;
		إذا (argv[i][0] != '-')
			اكسر;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	إذا (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	إذا (!no_index) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		إذا (nongit || ((argc == i + 2) &&
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
	إذا (nongit)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, عدم);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	إذا (no_index && prefix) {
		إذا (chdir(prefix))
			die(_("cannot come back to cwd"));
		prefix = عدم;
	}

	prefix = precompose_argv_prefix(argc, argv, prefix);

	repo_init_revisions(the_repository, &rev, prefix);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	إذا (no_index)
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

	إذا (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, عدم);
	إذا (!rev.diffopt.output_format) {
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
	إذا (!rev.pending.nr) {
		صحيح i;
		لكل (i = 1; i < argc; i++) {
			ثابت حرف *arg = argv[i];
			إذا (!strcmp(arg, "--"))
				اكسر;
			وإلا إذا (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				add_head_to_pending(&rev);
				إذا (!rev.pending.nr) {
					بنية درەخت *درەخت;
					درەخت = lookup_درەخت(the_repository,
							   the_repository->hash_algo->empty_درەخت);
					add_pending_object(&rev, &درەخت->object, "HEAD");
				}
				اكسر;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	لكل (i = 0; i < rev.pending.nr; i++) {
		بنية object_array_entry *entry = &rev.pending.objects[i];
		بنية object *obj = entry->item;
		ثابت حرف *name = entry->name;
		صحيح flags = (obj->flags & UNINTERESTING);
		إذا (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, عدم, 0);
		إذا (!obj)
			die(_("invalid object '%s' given."), name);
		إذا (obj->type == OBJ_COMMIT) {
			بنية درەخت *درەخت = repo_get_commit_درەخت(
				the_repository, (بنية commit *)obj);
			إذا (!درەخت)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &درەخت->object;
		}

		إذا (obj->type == OBJ_درەخت) {
			إذا (sdiff.skip && bitmap_get(sdiff.skip, i))
				استمر;
			obj->flags |= flags;
			add_object_array(obj, name, &ent);
			إذا (first_non_parent < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				first_non_parent = ent.nr - 1;
		} وإلا إذا (obj->type == OBJ_BLOB) {
			إذا (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = entry;
			blobs++;

		} وإلا {
			die(_("unhandled object '%s' given."), name);
		}
	}
	إذا (rev.prune_data.nr)
		paths += rev.prune_data.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	إذا (!ent.nr) {
		بدل (blobs) {
		حالة 0:
			builtin_diff_files(&rev, argc, argv);
			اكسر;
		حالة 1:
			إذا (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			اكسر;
		حالة 2:
			إذا (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			اكسر;
		افتراضي:
			usage(builtin_diff_usage);
		}
	}
	وإلا إذا (blobs)
		usage(builtin_diff_usage);
	وإلا إذا (ent.nr == 1)
		builtin_diff_index(&rev, argc, argv);
	وإلا إذا (ent.nr == 2) {
		إذا (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.چەپ, sdiff.ڕاست, sdiff.base);
		builtin_diff_درەخت(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} وإلا
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      first_non_parent);
	result = diff_result_code(&rev);
	إذا (1 < rev.diffopt.skip_stat_unmatch)
		refresh_index_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	أرجع result;
}
