/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "ewah/ewok.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "color.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "diff-merges.h"
#અન્તર્ભાવ "diffcore.h"
#અન્તર્ભાવ "preload-index.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "log-tree.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "tree.h"

#define DIFF_NO_INDEX_EXPLICIT 1
#define DIFF_NO_INDEX_IMPLICIT 2

સ્થિર અચલ અક્ષર builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

સ્થિર અચલ અક્ષર *blob_path(સંરચના object_array_entry *entry)
{
	પાછા entry->path ? entry->path : entry->name;
}

સ્થિર શૂન્ય stuff_change(સંરચના diff_options *opt,
			 unsigned old_mode, unsigned new_mode,
			 અચલ સંરચના object_id *old_oid,
			 અચલ સંરચના object_id *new_oid,
			 પૂર્ણાંક old_oid_valid,
			 પૂર્ણાંક new_oid_valid,
			 અચલ અક્ષર *old_path,
			 અચલ અક્ષર *new_path)
{
	સંરચના diff_filespec *one, *two;

	જો (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		પાછા;

	જો (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	જો (opt->prefix &&
	    (strncmp(old_path, opt->prefix, opt->prefix_length) ||
	     strncmp(new_path, opt->prefix, opt->prefix_length)))
		પાછા;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

સ્થિર શૂન્ય builtin_diff_b_f(સંરચના rev_info *revs,
			     પૂર્ણાંક argc, અચલ અક્ષર **argv UNUSED,
			     સંરચના object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	સંરચના stat st;
	અચલ અક્ષર *path;

	જો (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_data, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_data.વસ્તુઓ[0].match;

	જો (lstat(path, &st))
		die_errno(_("failed to stat '%s'"), path);
	જો (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
		die(_("'%s': not a regular file or symlink"), path);

	diff_set_mnemonic_prefix(&revs->diffopt, "o/", "w/");

	જો (blob[0]->mode == S_IFINVALID)
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

સ્થિર શૂન્ય builtin_diff_blobs(સંરચના rev_info *revs,
			       પૂર્ણાંક argc, અચલ અક્ષર **argv UNUSED,
			       સંરચના object_array_entry **blob)
{
	અચલ unsigned mode = canon_mode(S_IFREG | 0644);

	જો (argc > 1)
		usage(builtin_diff_usage);

	જો (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = mode;

	જો (blob[1]->mode == S_IFINVALID)
		blob[1]->mode = mode;

	stuff_change(&revs->diffopt,
		     blob[0]->mode, blob[1]->mode,
		     &blob[0]->item->oid, &blob[1]->item->oid,
		     1, 1,
		     blob_path(blob[0]), blob_path(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

સ્થિર શૂન્ય builtin_diff_index(સંરચના rev_info *revs,
			       પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	unsigned પૂર્ણાંક option = 0;
	જ્યારે (1 < argc) {
		અચલ અક્ષર *arg = argv[1];
		જો (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_INDEX_CACHED;
		નહીંતર જો (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		નહીંતર
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	જો (revs->pending.nr != 1 ||
	    revs->max_ગણતરી != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	જો (!(option & DIFF_INDEX_CACHED)) {
		setup_work_વૃક્ષ(the_repository);
		જો (repo_read_index_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} નહીંતર જો (repo_read_index(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_index(revs, option);
}

સ્થિર શૂન્ય builtin_diff_વૃક્ષ(સંરચના rev_info *revs,
			      પૂર્ણાંક argc, અચલ અક્ષર **argv,
			      સંરચના object_array_entry *ent0,
			      સંરચના object_array_entry *ent1)
{
	અચલ સંરચના object_id *(oid[2]);
	સંરચના object_id mb_oid;
	પૂર્ણાંક merge_base = 0;

	જ્યારે (1 < argc) {
		અચલ અક્ષર *arg = argv[1];
		જો (!strcmp(arg, "--merge-base"))
			merge_base = 1;
		નહીંતર
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	જો (merge_base) {
		diff_get_merge_base(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].item->oid;
	} નહીંતર {
		પૂર્ણાંક swap = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		જો (ent1->item->flags & UNINTERESTING)
			swap = 1;
		oid[swap] = &ent0->item->oid;
		oid[1 - swap] = &ent1->item->oid;
	}
	diff_વૃક્ષ_oid(oid[0], oid[1], "", &revs->diffopt);
	log_વૃક્ષ_diff_flush(revs);
}

સ્થિર શૂન્ય builtin_diff_combined(સંરચના rev_info *revs,
				  પૂર્ણાંક argc, અચલ અક્ષર **argv UNUSED,
				  સંરચના object_array_entry *ent,
				  પૂર્ણાંક ents, પૂર્ણાંક first_non_parent)
{
	સંરચના oid_array parents = OID_ARRAY_INIT;
	પૂર્ણાંક i;

	જો (argc > 1)
		usage(builtin_diff_usage);

	જો (first_non_parent < 0)
		die(_("no merge given, only parents."));
	જો (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	માટે (i = 0; i < ents; i++) {
		જો (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_વૃક્ષ_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

સ્થિર શૂન્ય refresh_index_quietly(શૂન્ય)
{
	સંરચના lock_file lock_file = LOCK_INIT;
	પૂર્ણાંક fd;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	જો (fd < 0)
		પાછા;
	discard_index(the_repository->index);
	repo_read_index(the_repository);
	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, ખાલી, ખાલી,
		      ખાલી);
	repo_update_index_if_able(the_repository, &lock_file);
}

સ્થિર શૂન્ય builtin_diff_files(સંરચના rev_info *revs, પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	unsigned પૂર્ણાંક options = 0;

	જ્યારે (1 < argc && argv[1][0] == '-') {
		જો (!strcmp(argv[1], "--base"))
			revs->max_ગણતરી = 1;
		નહીંતર જો (!strcmp(argv[1], "--ours"))
			revs->max_ગણતરી = 2;
		નહીંતર જો (!strcmp(argv[1], "--theirs"))
			revs->max_ગણતરી = 3;
		નહીંતર જો (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		નહીંતર જો (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		નહીંતર {
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
	જો (revs->max_ગણતરી == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(revs);

	setup_work_વૃક્ષ(the_repository);
	જો (repo_read_index_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

સંરચના symdiff {
	સંરચના bitmap *skip;
	પૂર્ણાંક warn;
	અચલ અક્ષર *base, *ડાબું, *જમણું;
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
સ્થિર શૂન્ય symdiff_prepare(સંરચના rev_info *rev, સંરચના symdiff *sym)
{
	પૂર્ણાંક i, is_symdiff = 0, basecount = 0, othercount = 0;
	પૂર્ણાંક lpos = -1, rpos = -1, basepos = -1;
	સંરચના bitmap *map = ખાલી;

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
	માટે (i = 0; i < rev->cmdline.nr; i++) {
		સંરચના object *obj = rev->pending.objects[i].item;
		પસંદગી (rev->cmdline.rev[i].whence) {
		સ્થિતિ REV_CMD_MERGE_BASE:
			જો (basepos < 0)
				basepos = i;
			basecount++;
			તોડો;		/* do mark all bases */
		સ્થિતિ REV_CMD_ડાબું:
			જો (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			જો (obj->flags & SYMMETRIC_ડાબું) {
				is_symdiff = 1;
				તોડો;	/* do mark A */
			}
			ચાલુ;
		સ્થિતિ REV_CMD_જમણું:
			જો (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			ચાલુ;	/* don't mark B */
		સ્થિતિ REV_CMD_PARENTS_ONLY:
		સ્થિતિ REV_CMD_REF:
		સ્થિતિ REV_CMD_REV:
			othercount++;
			ચાલુ;
		}
		જો (!map)
			map = bitmap_new();
		bitmap_set(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	જો (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	જો (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = ખાલી;
		પાછા;
	}

	sym->ડાબું = rev->pending.objects[lpos].name;
	sym->જમણું = rev->pending.objects[rpos].name;
	જો (basecount == 0)
		die(_("%s...%s: no merge base"), sym->ડાબું, sym->જમણું);
	sym->base = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

સ્થિર શૂન્ય symdiff_release(સંરચના symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

પૂર્ણાંક cmd_diff(પૂર્ણાંક argc,
	     અચલ અક્ષર **argv,
	     અચલ અક્ષર *prefix,
	     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i;
	સંરચના rev_info rev;
	સંરચના object_array ent = OBJECT_ARRAY_INIT;
	પૂર્ણાંક first_non_parent = -1;
	પૂર્ણાંક blobs = 0, paths = 0;
	સંરચના object_array_entry *blob[2];
	પૂર્ણાંક nongit = 0, no_index = 0;
	પૂર્ણાંક result;
	સંરચના symdiff sdiff;

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
	માટે (i = 1; i < argc; i++) {
		જો (!strcmp(argv[i], "--")) {
			i++;
			તોડો;
		}
		જો (!strcmp(argv[i], "--no-index"))
			no_index = DIFF_NO_INDEX_EXPLICIT;
		જો (argv[i][0] != '-')
			તોડો;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	જો (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	જો (!no_index) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		જો (nongit || ((argc == i + 2) &&
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
	જો (nongit)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ખાલી);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	જો (no_index && prefix) {
		જો (chdir(prefix))
			die(_("cannot come back to cwd"));
		prefix = ખાલી;
	}

	prefix = precompose_argv_prefix(argc, argv, prefix);

	repo_init_revisions(the_repository, &rev, prefix);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	જો (no_index)
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

	જો (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, ખાલી);
	જો (!rev.diffopt.output_format) {
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
	જો (!rev.pending.nr) {
		પૂર્ણાંક i;
		માટે (i = 1; i < argc; i++) {
			અચલ અક્ષર *arg = argv[i];
			જો (!strcmp(arg, "--"))
				તોડો;
			નહીંતર જો (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				add_head_to_pending(&rev);
				જો (!rev.pending.nr) {
					સંરચના વૃક્ષ *વૃક્ષ;
					વૃક્ષ = lookup_વૃક્ષ(the_repository,
							   the_repository->hash_algo->empty_વૃક્ષ);
					add_pending_object(&rev, &વૃક્ષ->object, "HEAD");
				}
				તોડો;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	માટે (i = 0; i < rev.pending.nr; i++) {
		સંરચના object_array_entry *entry = &rev.pending.objects[i];
		સંરચના object *obj = entry->item;
		અચલ અક્ષર *name = entry->name;
		પૂર્ણાંક flags = (obj->flags & UNINTERESTING);
		જો (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, ખાલી, 0);
		જો (!obj)
			die(_("invalid object '%s' given."), name);
		જો (obj->type == OBJ_COMMIT) {
			સંરચના વૃક્ષ *વૃક્ષ = repo_get_commit_વૃક્ષ(
				the_repository, (સંરચના commit *)obj);
			જો (!વૃક્ષ)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &વૃક્ષ->object;
		}

		જો (obj->type == OBJ_વૃક્ષ) {
			જો (sdiff.skip && bitmap_get(sdiff.skip, i))
				ચાલુ;
			obj->flags |= flags;
			add_object_array(obj, name, &ent);
			જો (first_non_parent < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				first_non_parent = ent.nr - 1;
		} નહીંતર જો (obj->type == OBJ_BLOB) {
			જો (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = entry;
			blobs++;

		} નહીંતર {
			die(_("unhandled object '%s' given."), name);
		}
	}
	જો (rev.prune_data.nr)
		paths += rev.prune_data.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	જો (!ent.nr) {
		પસંદગી (blobs) {
		સ્થિતિ 0:
			builtin_diff_files(&rev, argc, argv);
			તોડો;
		સ્થિતિ 1:
			જો (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			તોડો;
		સ્થિતિ 2:
			જો (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			તોડો;
		મૂળભૂત:
			usage(builtin_diff_usage);
		}
	}
	નહીંતર જો (blobs)
		usage(builtin_diff_usage);
	નહીંતર જો (ent.nr == 1)
		builtin_diff_index(&rev, argc, argv);
	નહીંતર જો (ent.nr == 2) {
		જો (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.ડાબું, sdiff.જમણું, sdiff.base);
		builtin_diff_વૃક્ષ(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} નહીંતર
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      first_non_parent);
	result = diff_result_code(&rev);
	જો (1 < rev.diffopt.skip_stat_unmatch)
		refresh_index_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	પાછા result;
}
