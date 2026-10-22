/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "ewah/ewok.h"
#समावेश "lockfile.h"
#समावेश "color.h"
#समावेश "commit.h"
#समावेश "gettext.h"
#समावेश "tag.h"
#समावेश "diff.h"
#समावेश "diff-merges.h"
#समावेश "diffcore.h"
#समावेश "preload-index.h"
#समावेश "read-cache-ll.h"
#समावेश "revision.h"
#समावेश "log-tree.h"
#समावेश "setup.h"
#समावेश "oid-array.h"
#समावेश "tree.h"

#घोषणा DIFF_NO_सूचकांक_EXPLICIT 1
#घोषणा DIFF_NO_सूचकांक_IMPLICIT 2

अटल स्थिर अक्षर builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

अटल स्थिर अक्षर *blob_path(काष्ठा object_array_entry *entry)
{
	वापस entry->path ? entry->path : entry->name;
}

अटल व्योम stuff_change(काष्ठा diff_options *opt,
			 अचिन्हित old_mode, अचिन्हित new_mode,
			 स्थिर काष्ठा object_id *old_oid,
			 स्थिर काष्ठा object_id *new_oid,
			 पूर्णांक old_oid_valid,
			 पूर्णांक new_oid_valid,
			 स्थिर अक्षर *old_path,
			 स्थिर अक्षर *new_path)
{
	काष्ठा diff_filespec *one, *two;

	अगर (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_mode == new_mode))
		वापस;

	अगर (opt->flags.reverse_diff) {
		SWAP(old_mode, new_mode);
		SWAP(old_oid, new_oid);
		SWAP(old_path, new_path);
	}

	अगर (opt->prefix &&
	    (म_भेदन(old_path, opt->prefix, opt->prefix_लंबाई) ||
	     म_भेदन(new_path, opt->prefix, opt->prefix_लंबाई)))
		वापस;

	one = alloc_filespec(old_path);
	two = alloc_filespec(new_path);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);

	diff_queue(&diff_queued_diff, one, two);
}

अटल व्योम builtin_diff_b_f(काष्ठा rev_info *revs,
			     पूर्णांक argc, स्थिर अक्षर **argv UNUSED,
			     काष्ठा object_array_entry **blob)
{
	/* Blob vs file in the working tree*/
	काष्ठा stat st;
	स्थिर अक्षर *path;

	अगर (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_तथ्य, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	path = revs->prune_तथ्य.वस्तुएँ[0].match;

	अगर (lstat(path, &st))
		die_errno(_("failed to stat '%s'"), path);
	अगर (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
		die(_("'%s': not a regular file or symlink"), path);

	diff_set_mnemonic_prefix(&revs->diffopt, "o/", "w/");

	अगर (blob[0]->mode == S_IFINVALID)
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

अटल व्योम builtin_diff_blobs(काष्ठा rev_info *revs,
			       पूर्णांक argc, स्थिर अक्षर **argv UNUSED,
			       काष्ठा object_array_entry **blob)
{
	स्थिर अचिन्हित mode = canon_mode(S_IFREG | 0644);

	अगर (argc > 1)
		usage(builtin_diff_usage);

	अगर (blob[0]->mode == S_IFINVALID)
		blob[0]->mode = mode;

	अगर (blob[1]->mode == S_IFINVALID)
		blob[1]->mode = mode;

	stuff_change(&revs->diffopt,
		     blob[0]->mode, blob[1]->mode,
		     &blob[0]->item->oid, &blob[1]->item->oid,
		     1, 1,
		     blob_path(blob[0]), blob_path(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

अटल व्योम builtin_diff_सूचकांक(काष्ठा rev_info *revs,
			       पूर्णांक argc, स्थिर अक्षर **argv)
{
	अचिन्हित पूर्णांक option = 0;
	जबतक (1 < argc) {
		स्थिर अक्षर *arg = argv[1];
		अगर (!म_भेद(arg, "--cached") || !म_भेद(arg, "--staged"))
			option |= DIFF_सूचकांक_CACHED;
		अन्यथा अगर (!म_भेद(arg, "--merge-base"))
			option |= DIFF_सूचकांक_MERGE_BASE;
		अन्यथा
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	अगर (revs->pending.nr != 1 ||
	    revs->max_गिनती != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	अगर (!(option & DIFF_सूचकांक_CACHED)) {
		setup_work_वृक्ष(the_repository);
		अगर (repo_read_सूचकांक_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} अन्यथा अगर (repo_read_सूचकांक(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	run_diff_सूचकांक(revs, option);
}

अटल व्योम builtin_diff_वृक्ष(काष्ठा rev_info *revs,
			      पूर्णांक argc, स्थिर अक्षर **argv,
			      काष्ठा object_array_entry *ent0,
			      काष्ठा object_array_entry *ent1)
{
	स्थिर काष्ठा object_id *(oid[2]);
	काष्ठा object_id mb_oid;
	पूर्णांक merge_base = 0;

	जबतक (1 < argc) {
		स्थिर अक्षर *arg = argv[1];
		अगर (!म_भेद(arg, "--merge-base"))
			merge_base = 1;
		अन्यथा
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	अगर (merge_base) {
		diff_get_merge_base(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].item->oid;
	} अन्यथा {
		पूर्णांक swap = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		अगर (ent1->item->flags & UNINTERESTING)
			swap = 1;
		oid[swap] = &ent0->item->oid;
		oid[1 - swap] = &ent1->item->oid;
	}
	diff_वृक्ष_oid(oid[0], oid[1], "", &revs->diffopt);
	log_वृक्ष_diff_flush(revs);
}

अटल व्योम builtin_diff_combined(काष्ठा rev_info *revs,
				  पूर्णांक argc, स्थिर अक्षर **argv UNUSED,
				  काष्ठा object_array_entry *ent,
				  पूर्णांक ents, पूर्णांक first_non_parent)
{
	काष्ठा oid_array parents = OID_ARRAY_INIT;
	पूर्णांक i;

	अगर (argc > 1)
		usage(builtin_diff_usage);

	अगर (first_non_parent < 0)
		die(_("no merge given, only parents."));
	अगर (first_non_parent >= ents)
		BUG("first_non_parent out of range: %d", first_non_parent);

	diff_merges_set_dense_combined_if_unset(revs);

	क्रम (i = 0; i < ents; i++) {
		अगर (i != first_non_parent)
			oid_array_append(&parents, &ent[i].item->oid);
	}
	diff_वृक्ष_combined(&ent[first_non_parent].item->oid, &parents, revs);
	oid_array_clear(&parents);
}

अटल व्योम refresh_सूचकांक_quietly(व्योम)
{
	काष्ठा lock_file lock_file = LOCK_INIT;
	पूर्णांक fd;

	fd = repo_hold_locked_सूचकांक(the_repository, &lock_file, 0);
	अगर (fd < 0)
		वापस;
	discard_सूचकांक(the_repository->सूचकांक);
	repo_read_सूचकांक(the_repository);
	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET|REFRESH_UNMERGED, शून्य, शून्य,
		      शून्य);
	repo_update_सूचकांक_if_able(the_repository, &lock_file);
}

अटल व्योम builtin_diff_files(काष्ठा rev_info *revs, पूर्णांक argc, स्थिर अक्षर **argv)
{
	अचिन्हित पूर्णांक options = 0;

	जबतक (1 < argc && argv[1][0] == '-') {
		अगर (!म_भेद(argv[1], "--base"))
			revs->max_गिनती = 1;
		अन्यथा अगर (!म_भेद(argv[1], "--ours"))
			revs->max_गिनती = 2;
		अन्यथा अगर (!म_भेद(argv[1], "--theirs"))
			revs->max_गिनती = 3;
		अन्यथा अगर (!म_भेद(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		अन्यथा अगर (!म_भेद(argv[1], "-h"))
			usage(builtin_diff_usage);
		अन्यथा {
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
	अगर (revs->max_गिनती == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(revs);

	setup_work_वृक्ष(the_repository);
	अगर (repo_read_सूचकांक_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	run_diff_files(revs, options);
}

काष्ठा symdiff {
	काष्ठा bitmap *skip;
	पूर्णांक warn;
	स्थिर अक्षर *base, *बायाँ, *दायाँ;
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
अटल व्योम symdiff_prepare(काष्ठा rev_info *rev, काष्ठा symdiff *sym)
{
	पूर्णांक i, is_symdiff = 0, basecount = 0, othercount = 0;
	पूर्णांक lpos = -1, rpos = -1, basepos = -1;
	काष्ठा bitmap *map = शून्य;

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
	क्रम (i = 0; i < rev->cmdline.nr; i++) {
		काष्ठा object *obj = rev->pending.objects[i].item;
		चयन (rev->cmdline.rev[i].whence) {
		हाल REV_CMD_MERGE_BASE:
			अगर (basepos < 0)
				basepos = i;
			basecount++;
			अवरोध;		/* do mark all bases */
		हाल REV_CMD_बायाँ:
			अगर (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			अगर (obj->flags & SYMMETRIC_बायाँ) {
				is_symdiff = 1;
				अवरोध;	/* do mark A */
			}
			जारी;
		हाल REV_CMD_दायाँ:
			अगर (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			जारी;	/* don't mark B */
		हाल REV_CMD_PARENTS_ONLY:
		हाल REV_CMD_REF:
		हाल REV_CMD_REV:
			othercount++;
			जारी;
		}
		अगर (!map)
			map = bitmap_new();
		bitmap_set(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	अगर (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	अगर (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = शून्य;
		वापस;
	}

	sym->बायाँ = rev->pending.objects[lpos].name;
	sym->दायाँ = rev->pending.objects[rpos].name;
	अगर (basecount == 0)
		die(_("%s...%s: no merge base"), sym->बायाँ, sym->दायाँ);
	sym->base = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

अटल व्योम symdiff_release(काष्ठा symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

पूर्णांक cmd_diff(पूर्णांक argc,
	     स्थिर अक्षर **argv,
	     स्थिर अक्षर *prefix,
	     काष्ठा repository *repo UNUSED)
{
	पूर्णांक i;
	काष्ठा rev_info rev;
	काष्ठा object_array ent = OBJECT_ARRAY_INIT;
	पूर्णांक first_non_parent = -1;
	पूर्णांक blobs = 0, paths = 0;
	काष्ठा object_array_entry *blob[2];
	पूर्णांक nongit = 0, no_सूचकांक = 0;
	पूर्णांक परिणाम;
	काष्ठा symdiff sdiff;

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
	क्रम (i = 1; i < argc; i++) {
		अगर (!म_भेद(argv[i], "--")) {
			i++;
			अवरोध;
		}
		अगर (!म_भेद(argv[i], "--no-index"))
			no_सूचकांक = DIFF_NO_सूचकांक_EXPLICIT;
		अगर (argv[i][0] != '-')
			अवरोध;
	}

	prefix = setup_git_directory_gently(the_repository, &nongit);

	अगर (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_सूचकांक = 0;
	}

	अगर (!no_सूचकांक) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		अगर (nongit || ((argc == i + 2) &&
			       (!path_inside_repo(the_repository, prefix, argv[i]) ||
				!path_inside_repo(the_repository, prefix, argv[i + 1]))))
			no_सूचकांक = DIFF_NO_सूचकांक_IMPLICIT;
	}

	/*
	 * When operating outside of a Git repository we need to have a hash
	 * algorithm at hand so that we can generate the blob hashes. We
	 * default to SHA1 here, but may eventually want to change this to be
	 * configurable via a command line option.
	 */
	अगर (nongit)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, शून्य);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	अगर (no_सूचकांक && prefix) {
		अगर (स_बदलो(prefix))
			die(_("cannot come back to cwd"));
		prefix = शून्य;
	}

	prefix = precompose_argv_prefix(argc, argv, prefix);

	repo_init_revisions(the_repository, &rev, prefix);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	अगर (no_सूचकांक)
		निकास(diff_no_सूचकांक(&rev, the_repository->hash_algo,
				   no_सूचकांक == DIFF_NO_सूचकांक_IMPLICIT,
				   argc, argv));


	/*
	 * Otherwise, we are doing the usual "git" diff; set up any
	 * further defaults that apply to regular diffs.
	 */
	rev.diffopt.skip_stat_unmatch = !!diff_auto_refresh_सूचकांक;

	/*
	 * Default to intent-to-add entries invisible in the
	 * index. This makes them show up as new files in diff-files
	 * and not at all in diff-cached.
	 */
	rev.diffopt.ita_invisible_in_सूचकांक = 1;

	अगर (nongit)
		die(_("Not a git repository"));
	argc = setup_revisions(argc, argv, &rev, शून्य);
	अगर (!rev.diffopt.output_format) {
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
	अगर (!rev.pending.nr) {
		पूर्णांक i;
		क्रम (i = 1; i < argc; i++) {
			स्थिर अक्षर *arg = argv[i];
			अगर (!म_भेद(arg, "--"))
				अवरोध;
			अन्यथा अगर (!म_भेद(arg, "--cached") ||
				 !म_भेद(arg, "--staged")) {
				add_शीर्ष_to_pending(&rev);
				अगर (!rev.pending.nr) {
					काष्ठा वृक्ष *वृक्ष;
					वृक्ष = lookup_वृक्ष(the_repository,
							   the_repository->hash_algo->empty_वृक्ष);
					add_pending_object(&rev, &वृक्ष->object, "HEAD");
				}
				अवरोध;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	क्रम (i = 0; i < rev.pending.nr; i++) {
		काष्ठा object_array_entry *entry = &rev.pending.objects[i];
		काष्ठा object *obj = entry->item;
		स्थिर अक्षर *name = entry->name;
		पूर्णांक flags = (obj->flags & UNINTERESTING);
		अगर (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_tag(the_repository, obj, शून्य, 0);
		अगर (!obj)
			die(_("invalid object '%s' given."), name);
		अगर (obj->type == OBJ_COMMIT) {
			काष्ठा वृक्ष *वृक्ष = repo_get_commit_वृक्ष(
				the_repository, (काष्ठा commit *)obj);
			अगर (!वृक्ष)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &वृक्ष->object;
		}

		अगर (obj->type == OBJ_वृक्ष) {
			अगर (sdiff.skip && bitmap_get(sdiff.skip, i))
				जारी;
			obj->flags |= flags;
			add_object_array(obj, name, &ent);
			अगर (first_non_parent < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				first_non_parent = ent.nr - 1;
		} अन्यथा अगर (obj->type == OBJ_BLOB) {
			अगर (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = entry;
			blobs++;

		} अन्यथा {
			die(_("unhandled object '%s' given."), name);
		}
	}
	अगर (rev.prune_तथ्य.nr)
		paths += rev.prune_तथ्य.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	अगर (!ent.nr) {
		चयन (blobs) {
		हाल 0:
			builtin_diff_files(&rev, argc, argv);
			अवरोध;
		हाल 1:
			अगर (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			अवरोध;
		हाल 2:
			अगर (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			अवरोध;
		शेष:
			usage(builtin_diff_usage);
		}
	}
	अन्यथा अगर (blobs)
		usage(builtin_diff_usage);
	अन्यथा अगर (ent.nr == 1)
		builtin_diff_सूचकांक(&rev, argc, argv);
	अन्यथा अगर (ent.nr == 2) {
		अगर (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.बायाँ, sdiff.दायाँ, sdiff.base);
		builtin_diff_वृक्ष(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} अन्यथा
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      first_non_parent);
	परिणाम = diff_परिणाम_code(&rev);
	अगर (1 < rev.diffopt.skip_stat_unmatch)
		refresh_सूचकांक_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	वापस परिणाम;
}
