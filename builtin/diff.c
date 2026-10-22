/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "ewah/ewok.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "color.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "diff-merges.h"
#अन्तर्भाव "diffcore.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "log-tree.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "tree.h"

#define DIFF_NO_INDEX_EXPLICIT 1
#define DIFF_NO_INDEX_IMPLICIT 2

स्थिर ध्रुव वर्ण builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

स्थिर ध्रुव वर्ण *blob_path(संरचना object_array_entry *entry)
{
	प्रत्यागम entry->path ? entry->path : entry->name;
}

स्थिर शून्य stuff_change(संरचना diff_options *opt,
			 unsigned old_mode, unsigned new_mode,
			 ध्रुव संरचना object_id *old_oid,
			 ध्रुव संरचना object_id *new_oid,
			 पूर्णाङ्क old_oid_valid,
			 पूर्णाङ्क new_oid_valid,
			 ध्रुव वर्ण *old_path,
			 ध्रुव वर्ण *new_path)
{
	संरचना diff_filespec *one, *two;

	जइ (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		प्रत्यागम;

	जइ (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	जइ (opt->prefix &&
	    (strncmp(old_path, opt->prefix, opt->prefix_length) ||
	     strncmp(new_path, opt->prefix, opt->prefix_length)))
		प्रत्यागम;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

स्थिर शून्य builtin_diff_b_f(संरचना rev_info *revs,
			     पूर्णाङ्क argc, ध्रुव वर्ण **argv UNUSED,
			     संरचना object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	संरचना stat st;
	ध्रुव वर्ण *path;

	जइ (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_data, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_data.वस्तूनि[0].match;

	जइ (lstat(path, &st))
		die_errno(_("failed to stat '%s'"), path);
	जइ (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
		die(_("'%s': not a regular file or symlink"), path);

	diff_set_mnemonic_prefix(&revs->diffopt, "o/", "w/");

	जइ (blob[0]->mode == S_IFINVALID)
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

स्थिर शून्य builtin_diff_blobs(संरचना rev_info *revs,
			       पूर्णाङ्क argc, ध्रुव वर्ण **argv UNUSED,
			       संरचना object_array_entry **blob)
{
	ध्रुव unsigned mode = canon_mode(S_IFREG | 0644);

	जइ (argc > 1)
		usage(builtin_diff_usage);

	जइ (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = mode;

	जइ (blob[1]->mode == S_IFINVALID)
		blob[1]->mode = mode;

	stuff_change(&revs->diffopt,
		     blob[0]->mode, blob[1]->mode,
		     &blob[0]->item->oid, &blob[1]->item->oid,
		     1, 1,
		     blob_path(blob[0]), blob_path(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

स्थिर शून्य builtin_diff_index(संरचना rev_info *revs,
			       पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	unsigned पूर्णाङ्क option = 0;
	जाव (1 < argc) {
		ध्रुव वर्ण *arg = argv[1];
		जइ (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_INDEX_CACHED;
		अण्णहा जइ (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		अण्णहा
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	जइ (revs->pending.nr != 1 ||
	    revs->max_गणनम् != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	जइ (!(option & DIFF_INDEX_CACHED)) {
		setup_work_वृक्षः(the_repository);
		जइ (repo_read_index_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} अण्णहा जइ (repo_read_index(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_index(revs, option);
}

स्थिर शून्य builtin_diff_वृक्षः(संरचना rev_info *revs,
			      पूर्णाङ्क argc, ध्रुव वर्ण **argv,
			      संरचना object_array_entry *ent0,
			      संरचना object_array_entry *ent1)
{
	ध्रुव संरचना object_id *(oid[2]);
	संरचना object_id mb_oid;
	पूर्णाङ्क merge_base = 0;

	जाव (1 < argc) {
		ध्रुव वर्ण *arg = argv[1];
		जइ (!strcmp(arg, "--merge-base"))
			merge_base = 1;
		अण्णहा
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	जइ (merge_base) {
		diff_get_merge_base(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].item->oid;
	} अण्णहा {
		पूर्णाङ्क swap = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		जइ (ent1->item->flags & UNINTERESTING)
			swap = 1;
		oid[swap] = &ent0->item->oid;
		oid[1 - swap] = &ent1->item->oid;
	}
	diff_वृक्षः_oid(oid[0], oid[1], "", &revs->diffopt);
	log_वृक्षः_diff_flush(revs);
}

स्थिर शून्य builtin_diff_combined(संरचना rev_info *revs,
				  पूर्णाङ्क argc, ध्रुव वर्ण **argv UNUSED,
				  संरचना object_array_entry *ent,
				  पूर्णाङ्क ents, पूर्णाङ्क first_non_parent)
{
	संरचना oid_array parents = OID_ARRAY_INIT;
	पूर्णाङ्क i;

	जइ (argc > 1)
		usage(builtin_diff_usage);

	जइ (first_non_parent < 0)
		die(_("no merge given, only parents."));
	जइ (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	प्रति (i = 0; i < ents; i++) {
		जइ (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_वृक्षः_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

स्थिर शून्य refresh_index_quietly(शून्य)
{
	संरचना lock_file lock_file = LOCK_INIT;
	पूर्णाङ्क fd;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	जइ (fd < 0)
		प्रत्यागम;
	discard_index(the_repository->index);
	repo_read_index(the_repository);
	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, रिक्त, रिक्त,
		      रिक्त);
	repo_update_index_if_able(the_repository, &lock_file);
}

स्थिर शून्य builtin_diff_files(संरचना rev_info *revs, पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	unsigned पूर्णाङ्क options = 0;

	जाव (1 < argc && argv[1][0] == '-') {
		जइ (!strcmp(argv[1], "--base"))
			revs->max_गणनम् = 1;
		अण्णहा जइ (!strcmp(argv[1], "--ours"))
			revs->max_गणनम् = 2;
		अण्णहा जइ (!strcmp(argv[1], "--theirs"))
			revs->max_गणनम् = 3;
		अण्णहा जइ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		अण्णहा जइ (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		अण्णहा {
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
	जइ (revs->max_गणनम् == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(revs);

	setup_work_वृक्षः(the_repository);
	जइ (repo_read_index_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

संरचना symdiff {
	संरचना bitmap *skip;
	पूर्णाङ्क warn;
	ध्रुव वर्ण *base, *वामः, *दक्षिणः;
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
स्थिर शून्य symdiff_prepare(संरचना rev_info *rev, संरचना symdiff *sym)
{
	पूर्णाङ्क i, is_symdiff = 0, basecount = 0, othercount = 0;
	पूर्णाङ्क lpos = -1, rpos = -1, basepos = -1;
	संरचना bitmap *map = रिक्त;

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
	प्रति (i = 0; i < rev->cmdline.nr; i++) {
		संरचना object *obj = rev->pending.objects[i].item;
		विकल्प (rev->cmdline.rev[i].whence) {
		पक्ष REV_CMD_MERGE_BASE:
			जइ (basepos < 0)
				basepos = i;
			basecount++;
			भङ्ग;		/* do mark all bases */
		पक्ष REV_CMD_वामः:
			जइ (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			जइ (obj->flags & SYMMETRIC_वामः) {
				is_symdiff = 1;
				भङ्ग;	/* do mark A */
			}
			अनुवर्त;
		पक्ष REV_CMD_दक्षिणः:
			जइ (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			अनुवर्त;	/* don't mark B */
		पक्ष REV_CMD_PARENTS_ONLY:
		पक्ष REV_CMD_REF:
		पक्ष REV_CMD_REV:
			othercount++;
			अनुवर्त;
		}
		जइ (!map)
			map = bitmap_new();
		bitmap_set(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	जइ (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	जइ (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = रिक्त;
		प्रत्यागम;
	}

	sym->वामः = rev->pending.objects[lpos].name;
	sym->दक्षिणः = rev->pending.objects[rpos].name;
	जइ (basecount == 0)
		die(_("%s...%s: no merge base"), sym->वामः, sym->दक्षिणः);
	sym->base = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

स्थिर शून्य symdiff_release(संरचना symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

पूर्णाङ्क cmd_diff(पूर्णाङ्क argc,
	     ध्रुव वर्ण **argv,
	     ध्रुव वर्ण *prefix,
	     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क i;
	संरचना rev_info rev;
	संरचना object_array ent = OBJECT_ARRAY_INIT;
	पूर्णाङ्क first_non_parent = -1;
	पूर्णाङ्क blobs = 0, paths = 0;
	संरचना object_array_entry *blob[2];
	पूर्णाङ्क nongit = 0, no_index = 0;
	पूर्णाङ्क result;
	संरचना symdiff sdiff;

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
	प्रति (i = 1; i < argc; i++) {
		जइ (!strcmp(argv[i], "--")) {
			i++;
			भङ्ग;
		}
		जइ (!strcmp(argv[i], "--no-index"))
			no_index = DIFF_NO_INDEX_EXPLICIT;
		जइ (argv[i][0] != '-')
			भङ्ग;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	जइ (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	जइ (!no_index) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		जइ (nongit || ((argc == i + 2) &&
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
	जइ (nongit)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, रिक्त);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	जइ (no_index && prefix) {
		जइ (chdir(prefix))
			die(_("cannot come back to cwd"));
		prefix = रिक्त;
	}

	prefix = precompose_argv_prefix(argc, argv, prefix);

	repo_init_revisions(the_repository, &rev, prefix);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	जइ (no_index)
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

	जइ (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, रिक्त);
	जइ (!rev.diffopt.output_format) {
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
	जइ (!rev.pending.nr) {
		पूर्णाङ्क i;
		प्रति (i = 1; i < argc; i++) {
			ध्रुव वर्ण *arg = argv[i];
			जइ (!strcmp(arg, "--"))
				भङ्ग;
			अण्णहा जइ (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				add_head_to_pending(&rev);
				जइ (!rev.pending.nr) {
					संरचना वृक्षः *वृक्षः;
					वृक्षः = lookup_वृक्षः(the_repository,
							   the_repository->hash_algo->empty_वृक्षः);
					add_pending_object(&rev, &वृक्षः->object, "HEAD");
				}
				भङ्ग;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	प्रति (i = 0; i < rev.pending.nr; i++) {
		संरचना object_array_entry *entry = &rev.pending.objects[i];
		संरचना object *obj = entry->item;
		ध्रुव वर्ण *name = entry->name;
		पूर्णाङ्क flags = (obj->flags & UNINTERESTING);
		जइ (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, रिक्त, 0);
		जइ (!obj)
			die(_("invalid object '%s' given."), name);
		जइ (obj->type == OBJ_COMMIT) {
			संरचना वृक्षः *वृक्षः = repo_get_commit_वृक्षः(
				the_repository, (संरचना commit *)obj);
			जइ (!वृक्षः)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &वृक्षः->object;
		}

		जइ (obj->type == OBJ_वृक्षः) {
			जइ (sdiff.skip && bitmap_get(sdiff.skip, i))
				अनुवर्त;
			obj->flags |= flags;
			add_object_array(obj, name, &ent);
			जइ (first_non_parent < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				first_non_parent = ent.nr - 1;
		} अण्णहा जइ (obj->type == OBJ_BLOB) {
			जइ (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = entry;
			blobs++;

		} अण्णहा {
			die(_("unhandled object '%s' given."), name);
		}
	}
	जइ (rev.prune_data.nr)
		paths += rev.prune_data.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	जइ (!ent.nr) {
		विकल्प (blobs) {
		पक्ष 0:
			builtin_diff_files(&rev, argc, argv);
			भङ्ग;
		पक्ष 1:
			जइ (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			भङ्ग;
		पक्ष 2:
			जइ (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			भङ्ग;
		स्वभाव:
			usage(builtin_diff_usage);
		}
	}
	अण्णहा जइ (blobs)
		usage(builtin_diff_usage);
	अण्णहा जइ (ent.nr == 1)
		builtin_diff_index(&rev, argc, argv);
	अण्णहा जइ (ent.nr == 2) {
		जइ (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.वामः, sdiff.दक्षिणः, sdiff.base);
		builtin_diff_वृक्षः(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} अण्णहा
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      first_non_parent);
	result = diff_result_code(&rev);
	जइ (1 < rev.diffopt.skip_stat_unmatch)
		refresh_index_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	प्रत्यागम result;
}
