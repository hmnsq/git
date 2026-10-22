/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "ewah/ewok.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "color.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "diff-merges.h"
#അന്തര്ഭാവ "diffcore.h"
#അന്തര്ഭാവ "preload-index.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "log-tree.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "tree.h"

#define DIFF_NO_INDEX_EXPLICIT 1
#define DIFF_NO_INDEX_IMPLICIT 2

സ്ഥിതം സ്ഥിരം അക്ഷരം builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

സ്ഥിതം സ്ഥിരം അക്ഷരം *blob_path(ഘടന object_array_entry *entry)
{
	തിരികെ entry->path ? entry->path : entry->name;
}

സ്ഥിതം ശൂന്യം stuff_change(ഘടന diff_options *opt,
			 unsigned old_mode, unsigned new_mode,
			 സ്ഥിരം ഘടന object_id *old_oid,
			 സ്ഥിരം ഘടന object_id *new_oid,
			 പൂർണ്ണസംഖ്യ old_oid_valid,
			 പൂർണ്ണസംഖ്യ new_oid_valid,
			 സ്ഥിരം അക്ഷരം *old_path,
			 സ്ഥിരം അക്ഷരം *new_path)
{
	ഘടന diff_filespec *one, *two;

	എങ്കിൽ (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		തിരികെ;

	എങ്കിൽ (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	എങ്കിൽ (opt->prefix &&
	    (strncmp(old_path, opt->prefix, opt->prefix_length) ||
	     strncmp(new_path, opt->prefix, opt->prefix_length)))
		തിരികെ;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

സ്ഥിതം ശൂന്യം builtin_diff_b_f(ഘടന rev_info *revs,
			     പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv UNUSED,
			     ഘടന object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	ഘടന stat st;
	സ്ഥിരം അക്ഷരം *path;

	എങ്കിൽ (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_data, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_data.ഇനങ്ങൾ[0].match;

	എങ്കിൽ (lstat(path, &st))
		die_errno(_("failed to stat '%s'"), path);
	എങ്കിൽ (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
		die(_("'%s': not a regular file or symlink"), path);

	diff_set_mnemonic_prefix(&revs->diffopt, "o/", "w/");

	എങ്കിൽ (blob[0]->mode == S_IFINVALID)
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

സ്ഥിതം ശൂന്യം builtin_diff_blobs(ഘടന rev_info *revs,
			       പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv UNUSED,
			       ഘടന object_array_entry **blob)
{
	സ്ഥിരം unsigned mode = canon_mode(S_IFREG | 0644);

	എങ്കിൽ (argc > 1)
		usage(builtin_diff_usage);

	എങ്കിൽ (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = mode;

	എങ്കിൽ (blob[1]->mode == S_IFINVALID)
		blob[1]->mode = mode;

	stuff_change(&revs->diffopt,
		     blob[0]->mode, blob[1]->mode,
		     &blob[0]->item->oid, &blob[1]->item->oid,
		     1, 1,
		     blob_path(blob[0]), blob_path(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

സ്ഥിതം ശൂന്യം builtin_diff_index(ഘടന rev_info *revs,
			       പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	unsigned പൂർണ്ണസംഖ്യ option = 0;
	ആകുമ്പോൾ (1 < argc) {
		സ്ഥിരം അക്ഷരം *arg = argv[1];
		എങ്കിൽ (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_INDEX_CACHED;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		അല്ലെങ്കിൽ
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	എങ്കിൽ (revs->pending.nr != 1 ||
	    revs->max_എണ്ണം != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	എങ്കിൽ (!(option & DIFF_INDEX_CACHED)) {
		setup_work_വൃക്ഷം(the_repository);
		എങ്കിൽ (repo_read_index_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (repo_read_index(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_index(revs, option);
}

സ്ഥിതം ശൂന്യം builtin_diff_വൃക്ഷം(ഘടന rev_info *revs,
			      പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
			      ഘടന object_array_entry *ent0,
			      ഘടന object_array_entry *ent1)
{
	സ്ഥിരം ഘടന object_id *(oid[2]);
	ഘടന object_id mb_oid;
	പൂർണ്ണസംഖ്യ merge_base = 0;

	ആകുമ്പോൾ (1 < argc) {
		സ്ഥിരം അക്ഷരം *arg = argv[1];
		എങ്കിൽ (!strcmp(arg, "--merge-base"))
			merge_base = 1;
		അല്ലെങ്കിൽ
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	എങ്കിൽ (merge_base) {
		diff_get_merge_base(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].item->oid;
	} അല്ലെങ്കിൽ {
		പൂർണ്ണസംഖ്യ swap = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		എങ്കിൽ (ent1->item->flags & UNINTERESTING)
			swap = 1;
		oid[swap] = &ent0->item->oid;
		oid[1 - swap] = &ent1->item->oid;
	}
	diff_വൃക്ഷം_oid(oid[0], oid[1], "", &revs->diffopt);
	log_വൃക്ഷം_diff_flush(revs);
}

സ്ഥിതം ശൂന്യം builtin_diff_combined(ഘടന rev_info *revs,
				  പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv UNUSED,
				  ഘടന object_array_entry *ent,
				  പൂർണ്ണസംഖ്യ ents, പൂർണ്ണസംഖ്യ first_non_parent)
{
	ഘടന oid_array parents = OID_ARRAY_INIT;
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (argc > 1)
		usage(builtin_diff_usage);

	എങ്കിൽ (first_non_parent < 0)
		die(_("no merge given, only parents."));
	എങ്കിൽ (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	വേണ്ടി (i = 0; i < ents; i++) {
		എങ്കിൽ (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_വൃക്ഷം_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

സ്ഥിതം ശൂന്യം refresh_index_quietly(ശൂന്യം)
{
	ഘടന lock_file lock_file = LOCK_INIT;
	പൂർണ്ണസംഖ്യ fd;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	എങ്കിൽ (fd < 0)
		തിരികെ;
	discard_index(the_repository->index);
	repo_read_index(the_repository);
	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, ശൂന്യ, ശൂന്യ,
		      ശൂന്യ);
	repo_update_index_if_able(the_repository, &lock_file);
}

സ്ഥിതം ശൂന്യം builtin_diff_files(ഘടന rev_info *revs, പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	unsigned പൂർണ്ണസംഖ്യ options = 0;

	ആകുമ്പോൾ (1 < argc && argv[1][0] == '-') {
		എങ്കിൽ (!strcmp(argv[1], "--base"))
			revs->max_എണ്ണം = 1;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(argv[1], "--ours"))
			revs->max_എണ്ണം = 2;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(argv[1], "--theirs"))
			revs->max_എണ്ണം = 3;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		അല്ലെങ്കിൽ {
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
	എങ്കിൽ (revs->max_എണ്ണം == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(revs);

	setup_work_വൃക്ഷം(the_repository);
	എങ്കിൽ (repo_read_index_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

ഘടന symdiff {
	ഘടന bitmap *skip;
	പൂർണ്ണസംഖ്യ warn;
	സ്ഥിരം അക്ഷരം *base, *ഇടത്, *വലത്;
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
സ്ഥിതം ശൂന്യം symdiff_prepare(ഘടന rev_info *rev, ഘടന symdiff *sym)
{
	പൂർണ്ണസംഖ്യ i, is_symdiff = 0, basecount = 0, othercount = 0;
	പൂർണ്ണസംഖ്യ lpos = -1, rpos = -1, basepos = -1;
	ഘടന bitmap *map = ശൂന്യ;

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
	വേണ്ടി (i = 0; i < rev->cmdline.nr; i++) {
		ഘടന object *obj = rev->pending.objects[i].item;
		തിരഞ്ഞെടുപ്പ് (rev->cmdline.rev[i].whence) {
		അവസ്ഥ REV_CMD_MERGE_BASE:
			എങ്കിൽ (basepos < 0)
				basepos = i;
			basecount++;
			നിർത്തുക;		/* do mark all bases */
		അവസ്ഥ REV_CMD_ഇടത്:
			എങ്കിൽ (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			എങ്കിൽ (obj->flags & SYMMETRIC_ഇടത്) {
				is_symdiff = 1;
				നിർത്തുക;	/* do mark A */
			}
			തുടരുക;
		അവസ്ഥ REV_CMD_വലത്:
			എങ്കിൽ (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			തുടരുക;	/* don't mark B */
		അവസ്ഥ REV_CMD_PARENTS_ONLY:
		അവസ്ഥ REV_CMD_REF:
		അവസ്ഥ REV_CMD_REV:
			othercount++;
			തുടരുക;
		}
		എങ്കിൽ (!map)
			map = bitmap_new();
		bitmap_set(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	എങ്കിൽ (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	എങ്കിൽ (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = ശൂന്യ;
		തിരികെ;
	}

	sym->ഇടത് = rev->pending.objects[lpos].name;
	sym->വലത് = rev->pending.objects[rpos].name;
	എങ്കിൽ (basecount == 0)
		die(_("%s...%s: no merge base"), sym->ഇടത്, sym->വലത്);
	sym->base = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

സ്ഥിതം ശൂന്യം symdiff_release(ഘടന symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

പൂർണ്ണസംഖ്യ cmd_diff(പൂർണ്ണസംഖ്യ argc,
	     സ്ഥിരം അക്ഷരം **argv,
	     സ്ഥിരം അക്ഷരം *prefix,
	     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന rev_info rev;
	ഘടന object_array ent = OBJECT_ARRAY_INIT;
	പൂർണ്ണസംഖ്യ first_non_parent = -1;
	പൂർണ്ണസംഖ്യ blobs = 0, paths = 0;
	ഘടന object_array_entry *blob[2];
	പൂർണ്ണസംഖ്യ nongit = 0, no_index = 0;
	പൂർണ്ണസംഖ്യ result;
	ഘടന symdiff sdiff;

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
	വേണ്ടി (i = 1; i < argc; i++) {
		എങ്കിൽ (!strcmp(argv[i], "--")) {
			i++;
			നിർത്തുക;
		}
		എങ്കിൽ (!strcmp(argv[i], "--no-index"))
			no_index = DIFF_NO_INDEX_EXPLICIT;
		എങ്കിൽ (argv[i][0] != '-')
			നിർത്തുക;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	എങ്കിൽ (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	എങ്കിൽ (!no_index) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		എങ്കിൽ (nongit || ((argc == i + 2) &&
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
	എങ്കിൽ (nongit)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ശൂന്യ);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	എങ്കിൽ (no_index && prefix) {
		എങ്കിൽ (chdir(prefix))
			die(_("cannot come back to cwd"));
		prefix = ശൂന്യ;
	}

	prefix = precompose_argv_prefix(argc, argv, prefix);

	repo_init_revisions(the_repository, &rev, prefix);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	എങ്കിൽ (no_index)
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

	എങ്കിൽ (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, ശൂന്യ);
	എങ്കിൽ (!rev.diffopt.output_format) {
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
	എങ്കിൽ (!rev.pending.nr) {
		പൂർണ്ണസംഖ്യ i;
		വേണ്ടി (i = 1; i < argc; i++) {
			സ്ഥിരം അക്ഷരം *arg = argv[i];
			എങ്കിൽ (!strcmp(arg, "--"))
				നിർത്തുക;
			അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				add_head_to_pending(&rev);
				എങ്കിൽ (!rev.pending.nr) {
					ഘടന വൃക്ഷം *വൃക്ഷം;
					വൃക്ഷം = lookup_വൃക്ഷം(the_repository,
							   the_repository->hash_algo->empty_വൃക്ഷം);
					add_pending_object(&rev, &വൃക്ഷം->object, "HEAD");
				}
				നിർത്തുക;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	വേണ്ടി (i = 0; i < rev.pending.nr; i++) {
		ഘടന object_array_entry *entry = &rev.pending.objects[i];
		ഘടന object *obj = entry->item;
		സ്ഥിരം അക്ഷരം *name = entry->name;
		പൂർണ്ണസംഖ്യ flags = (obj->flags & UNINTERESTING);
		എങ്കിൽ (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, ശൂന്യ, 0);
		എങ്കിൽ (!obj)
			die(_("invalid object '%s' given."), name);
		എങ്കിൽ (obj->type == OBJ_COMMIT) {
			ഘടന വൃക്ഷം *വൃക്ഷം = repo_get_commit_വൃക്ഷം(
				the_repository, (ഘടന commit *)obj);
			എങ്കിൽ (!വൃക്ഷം)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &വൃക്ഷം->object;
		}

		എങ്കിൽ (obj->type == OBJ_വൃക്ഷം) {
			എങ്കിൽ (sdiff.skip && bitmap_get(sdiff.skip, i))
				തുടരുക;
			obj->flags |= flags;
			add_object_array(obj, name, &ent);
			എങ്കിൽ (first_non_parent < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				first_non_parent = ent.nr - 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (obj->type == OBJ_BLOB) {
			എങ്കിൽ (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = entry;
			blobs++;

		} അല്ലെങ്കിൽ {
			die(_("unhandled object '%s' given."), name);
		}
	}
	എങ്കിൽ (rev.prune_data.nr)
		paths += rev.prune_data.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	എങ്കിൽ (!ent.nr) {
		തിരഞ്ഞെടുപ്പ് (blobs) {
		അവസ്ഥ 0:
			builtin_diff_files(&rev, argc, argv);
			നിർത്തുക;
		അവസ്ഥ 1:
			എങ്കിൽ (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			നിർത്തുക;
		അവസ്ഥ 2:
			എങ്കിൽ (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			usage(builtin_diff_usage);
		}
	}
	അല്ലെങ്കിൽ എങ്കിൽ (blobs)
		usage(builtin_diff_usage);
	അല്ലെങ്കിൽ എങ്കിൽ (ent.nr == 1)
		builtin_diff_index(&rev, argc, argv);
	അല്ലെങ്കിൽ എങ്കിൽ (ent.nr == 2) {
		എങ്കിൽ (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.ഇടത്, sdiff.വലത്, sdiff.base);
		builtin_diff_വൃക്ഷം(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} അല്ലെങ്കിൽ
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      first_non_parent);
	result = diff_result_code(&rev);
	എങ്കിൽ (1 < rev.diffopt.skip_stat_unmatch)
		refresh_index_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	തിരികെ result;
}
