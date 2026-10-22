/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "ewah/ewok.h"
#অন্তর্ভাৱ "lockfile.h"
#অন্তর্ভাৱ "color.h"
#অন্তর্ভাৱ "commit.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "tag.h"
#অন্তর্ভাৱ "diff.h"
#অন্তর্ভাৱ "diff-merges.h"
#অন্তর্ভাৱ "diffcore.h"
#অন্তর্ভাৱ "preload-index.h"
#অন্তর্ভাৱ "read-cache-ll.h"
#অন্তর্ভাৱ "revision.h"
#অন্তর্ভাৱ "log-tree.h"
#অন্তর্ভাৱ "setup.h"
#অন্তর্ভাৱ "oid-array.h"
#অন্তর্ভাৱ "tree.h"

#define DIFF_NO_INDEX_EXPLICIT 1
#define DIFF_NO_INDEX_IMPLICIT 2

স্থির ধ্রুৱ ৱর্ণ builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

স্থির ধ্রুৱ ৱর্ণ *blob_path(সংরচনা object_array_entry *entry)
{
	ঘূৰাই entry->path ? entry->path : entry->name;
}

স্থির শূন্য stuff_change(সংরচনা diff_options *opt,
			 unsigned old_mode, unsigned new_mode,
			 ধ্রুৱ সংরচনা object_id *old_oid,
			 ধ্রুৱ সংরচনা object_id *new_oid,
			 পূর্ণাঙ্ক old_oid_valid,
			 পূর্ণাঙ্ক new_oid_valid,
			 ধ্রুৱ ৱর্ণ *old_path,
			 ধ্রুৱ ৱর্ণ *new_path)
{
	সংরচনা diff_filespec *one, *two;

	যদি (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		ঘূৰাই;

	যদি (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	যদি (opt->prefix &&
	    (strncmp(old_path, opt->prefix, opt->prefix_length) ||
	     strncmp(new_path, opt->prefix, opt->prefix_length)))
		ঘূৰাই;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

স্থির শূন্য builtin_diff_b_f(সংরচনা rev_info *revs,
			     পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv UNUSED,
			     সংরচনা object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	সংরচনা stat st;
	ধ্রুৱ ৱর্ণ *path;

	যদি (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_data, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_data.বস্তুসমূহ[0].match;

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

স্থির শূন্য builtin_diff_blobs(সংরচনা rev_info *revs,
			       পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv UNUSED,
			       সংরচনা object_array_entry **blob)
{
	ধ্রুৱ unsigned mode = canon_mode(S_IFREG | 0644);

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

স্থির শূন্য builtin_diff_index(সংরচনা rev_info *revs,
			       পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv)
{
	unsigned পূর্ণাঙ্ক option = 0;
	যেতিয়ালৈ (1 < argc) {
		ধ্রুৱ ৱর্ণ *arg = argv[1];
		যদি (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_INDEX_CACHED;
		অন্যথা যদি (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
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
	যদি (!(option & DIFF_INDEX_CACHED)) {
		setup_work_বৃক্ষ(the_repository);
		যদি (repo_read_index_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} অন্যথা যদি (repo_read_index(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_index(revs, option);
}

স্থির শূন্য builtin_diff_বৃক্ষ(সংরচনা rev_info *revs,
			      পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv,
			      সংরচনা object_array_entry *ent0,
			      সংরচনা object_array_entry *ent1)
{
	ধ্রুৱ সংরচনা object_id *(oid[2]);
	সংরচনা object_id mb_oid;
	পূর্ণাঙ্ক merge_base = 0;

	যেতিয়ালৈ (1 < argc) {
		ধ্রুৱ ৱর্ণ *arg = argv[1];
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
		পূর্ণাঙ্ক swap = 0;

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

স্থির শূন্য builtin_diff_combined(সংরচনা rev_info *revs,
				  পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv UNUSED,
				  সংরচনা object_array_entry *ent,
				  পূর্ণাঙ্ক ents, পূর্ণাঙ্ক first_non_parent)
{
	সংরচনা oid_array parents = OID_ARRAY_INIT;
	পূর্ণাঙ্ক i;

	যদি (argc > 1)
		usage(builtin_diff_usage);

	যদি (first_non_parent < 0)
		die(_("no merge given, only parents."));
	যদি (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	বাবে (i = 0; i < ents; i++) {
		যদি (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_বৃক্ষ_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

স্থির শূন্য refresh_index_quietly(শূন্য)
{
	সংরচনা lock_file lock_file = LOCK_INIT;
	পূর্ণাঙ্ক fd;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	যদি (fd < 0)
		ঘূৰাই;
	discard_index(the_repository->index);
	repo_read_index(the_repository);
	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, রিক্ত, রিক্ত,
		      রিক্ত);
	repo_update_index_if_able(the_repository, &lock_file);
}

স্থির শূন্য builtin_diff_files(সংরচনা rev_info *revs, পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv)
{
	unsigned পূর্ণাঙ্ক options = 0;

	যেতিয়ালৈ (1 < argc && argv[1][0] == '-') {
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
	যদি (repo_read_index_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

সংরচনা symdiff {
	সংরচনা bitmap *skip;
	পূর্ণাঙ্ক warn;
	ধ্রুৱ ৱর্ণ *base, *বাম, *ডান;
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
স্থির শূন্য symdiff_prepare(সংরচনা rev_info *rev, সংরচনা symdiff *sym)
{
	পূর্ণাঙ্ক i, is_symdiff = 0, basecount = 0, othercount = 0;
	পূর্ণাঙ্ক lpos = -1, rpos = -1, basepos = -1;
	সংরচনা bitmap *map = রিক্ত;

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
	বাবে (i = 0; i < rev->cmdline.nr; i++) {
		সংরচনা object *obj = rev->pending.objects[i].item;
		ৱিকল্প (rev->cmdline.rev[i].whence) {
		পক্ষ REV_CMD_MERGE_BASE:
			যদি (basepos < 0)
				basepos = i;
			basecount++;
			ভঙ্গ;		/* do mark all bases */
		পক্ষ REV_CMD_বাম:
			যদি (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			যদি (obj->flags & SYMMETRIC_বাম) {
				is_symdiff = 1;
				ভঙ্গ;	/* do mark A */
			}
			অনুৱর্ত;
		পক্ষ REV_CMD_ডান:
			যদি (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			অনুৱর্ত;	/* don't mark B */
		পক্ষ REV_CMD_PARENTS_ONLY:
		পক্ষ REV_CMD_REF:
		পক্ষ REV_CMD_REV:
			othercount++;
			অনুৱর্ত;
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
		sym->skip = রিক্ত;
		ঘূৰাই;
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

স্থির শূন্য symdiff_release(সংরচনা symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

পূর্ণাঙ্ক cmd_diff(পূর্ণাঙ্ক argc,
	     ধ্রুৱ ৱর্ণ **argv,
	     ধ্রুৱ ৱর্ণ *prefix,
	     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক i;
	সংরচনা rev_info rev;
	সংরচনা object_array ent = OBJECT_ARRAY_INIT;
	পূর্ণাঙ্ক first_non_parent = -1;
	পূর্ণাঙ্ক blobs = 0, paths = 0;
	সংরচনা object_array_entry *blob[2];
	পূর্ণাঙ্ক nongit = 0, no_index = 0;
	পূর্ণাঙ্ক result;
	সংরচনা symdiff sdiff;

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
	বাবে (i = 1; i < argc; i++) {
		যদি (!strcmp(argv[i], "--")) {
			i++;
			ভঙ্গ;
		}
		যদি (!strcmp(argv[i], "--no-index"))
			no_index = DIFF_NO_INDEX_EXPLICIT;
		যদি (argv[i][0] != '-')
			ভঙ্গ;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	যদি (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	যদি (!no_index) {
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
			no_index = DIFF_NO_INDEX_IMPLICIT;
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
	repo_config(the_repository, git_diff_ui_config, রিক্ত);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	যদি (no_index && prefix) {
		যদি (chdir(prefix))
			die(_("cannot come back to cwd"));
		prefix = রিক্ত;
	}

	prefix = precompose_argv_prefix(argc, argv, prefix);

	repo_init_revisions(the_repository, &rev, prefix);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	যদি (no_index)
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

	যদি (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, রিক্ত);
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
		পূর্ণাঙ্ক i;
		বাবে (i = 1; i < argc; i++) {
			ধ্রুৱ ৱর্ণ *arg = argv[i];
			যদি (!strcmp(arg, "--"))
				ভঙ্গ;
			অন্যথা যদি (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				add_head_to_pending(&rev);
				যদি (!rev.pending.nr) {
					সংরচনা বৃক্ষ *বৃক্ষ;
					বৃক্ষ = lookup_বৃক্ষ(the_repository,
							   the_repository->hash_algo->empty_বৃক্ষ);
					add_pending_object(&rev, &বৃক্ষ->object, "HEAD");
				}
				ভঙ্গ;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	বাবে (i = 0; i < rev.pending.nr; i++) {
		সংরচনা object_array_entry *entry = &rev.pending.objects[i];
		সংরচনা object *obj = entry->item;
		ধ্রুৱ ৱর্ণ *name = entry->name;
		পূর্ণাঙ্ক flags = (obj->flags & UNINTERESTING);
		যদি (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, রিক্ত, 0);
		যদি (!obj)
			die(_("invalid object '%s' given."), name);
		যদি (obj->type == OBJ_COMMIT) {
			সংরচনা বৃক্ষ *বৃক্ষ = repo_get_commit_বৃক্ষ(
				the_repository, (সংরচনা commit *)obj);
			যদি (!বৃক্ষ)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &বৃক্ষ->object;
		}

		যদি (obj->type == OBJ_বৃক্ষ) {
			যদি (sdiff.skip && bitmap_get(sdiff.skip, i))
				অনুৱর্ত;
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
	যদি (rev.prune_data.nr)
		paths += rev.prune_data.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	যদি (!ent.nr) {
		ৱিকল্প (blobs) {
		পক্ষ 0:
			builtin_diff_files(&rev, argc, argv);
			ভঙ্গ;
		পক্ষ 1:
			যদি (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			ভঙ্গ;
		পক্ষ 2:
			যদি (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			ভঙ্গ;
		স্ৱভাৱ:
			usage(builtin_diff_usage);
		}
	}
	অন্যথা যদি (blobs)
		usage(builtin_diff_usage);
	অন্যথা যদি (ent.nr == 1)
		builtin_diff_index(&rev, argc, argv);
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
	result = diff_result_code(&rev);
	যদি (1 < rev.diffopt.skip_stat_unmatch)
		refresh_index_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	ঘূৰাই result;
}
