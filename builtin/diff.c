/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کرو DIFF_NO_اشاریہ_EXPLICIT 1
#تعریف_کرو DIFF_NO_اشاریہ_IMPLICIT 2

ساکن ثابت حرف builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ساکن ثابت حرف *blob_راستہ(ساخت object_array_اندراج *اندراج_2)
{
	واپس اندراج_2->راستہ ? اندراج_2->راستہ : اندراج_2->name;
}

ساکن خالی stuff_change(ساخت diff_options *opt,
			 غیرعلامتی old_منوال, غیرعلامتی new_منوال,
			 ثابت ساخت object_id *old_oid,
			 ثابت ساخت object_id *new_oid,
			 صحیح old_oid_valid,
			 صحیح new_oid_valid,
			 ثابت حرف *old_راستہ,
			 ثابت حرف *new_راستہ)
{
	ساخت diff_filespec *one, *two;

	اگر (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_منوال == new_منوال))
		واپس;

	اگر (opt->flags.الٹ_diff) {
		تبادلہ(old_منوال, new_منوال);
		تبادلہ(old_oid, new_oid);
		تبادلہ(old_راستہ, new_راستہ);
	}

	اگر (opt->سابقہ &&
	    (لڑی_موازنہ(old_راستہ, opt->سابقہ, opt->سابقہ_لمبائی) ||
	     لڑی_موازنہ(new_راستہ, opt->سابقہ, opt->سابقہ_لمبائی)))
		واپس;

	one = alloc_filespec(old_راستہ);
	two = alloc_filespec(new_راستہ);
	fill_filespec(one, old_oid, old_oid_valid, old_منوال);
	fill_filespec(two, new_oid, new_oid_valid, new_منوال);

	diff_قطار(&diff_queued_diff, one, two);
}

ساکن خالی builtin_diff_b_f(ساخت rev_info *revs,
			     صحیح argc, ثابت حرف **argv غیر_مستعمل,
			     ساخت object_array_اندراج **blob)
{
	/* Blob vs file in the working tree*/
	ساخت stat st;
	ثابت حرف *راستہ;

	اگر (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_ڈیٹا, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	راستہ = revs->prune_ڈیٹا.اشیاء[0].مطابقت;

	اگر (lstat(راستہ, &st))
		die_errno(_("failed to stat '%s'"), راستہ);
	اگر (!(S_ISREG(st.st_منوال) || S_ISLNK(st.st_منوال)))
		die(_("'%s': not a regular file or symlink"), راستہ);

	diff_سیٹ_mnemonic_سابقہ(&revs->diffopt, "o/", "w/");

	اگر (blob[0]->منوال == S_IFINVALID)
		blob[0]->منوال = canon_منوال(st.st_منوال);

	stuff_change(&revs->diffopt,
		     blob[0]->منوال, canon_منوال(st.st_منوال),
		     &blob[0]->شے->oid, null_oid(the_hash_algo),
		     1, 0,
		     blob[0]->راستہ ? blob[0]->راستہ : راستہ,
		     راستہ);
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ساکن خالی builtin_diff_blobs(ساخت rev_info *revs,
			       صحیح argc, ثابت حرف **argv غیر_مستعمل,
			       ساخت object_array_اندراج **blob)
{
	ثابت غیرعلامتی منوال = canon_منوال(S_IFREG | 0644);

	اگر (argc > 1)
		usage(builtin_diff_usage);

	اگر (blob[0]->منوال == S_IFINVALID)
		blob[0]->منوال = منوال;

	اگر (blob[1]->منوال == S_IFINVALID)
		blob[1]->منوال = منوال;

	stuff_change(&revs->diffopt,
		     blob[0]->منوال, blob[1]->منوال,
		     &blob[0]->شے->oid, &blob[1]->شے->oid,
		     1, 1,
		     blob_راستہ(blob[0]), blob_راستہ(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ساکن خالی builtin_diff_اشاریہ(ساخت rev_info *revs,
			       صحیح argc, ثابت حرف **argv)
{
	غیرعلامتی صحیح option = 0;
	جب (1 < argc) {
		ثابت حرف *arg = argv[1];
		اگر (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_اشاریہ_CACHED;
		ورنہ اگر (!strcmp(arg, "--merge-base"))
			option |= DIFF_اشاریہ_ملاؤ_بنیاد;
		ورنہ
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	اگر (revs->pending.nr != 1 ||
	    revs->max_گنتی != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	اگر (!(option & DIFF_اشاریہ_CACHED)) {
		تیاری_work_درخت(the_repository);
		اگر (repo_read_اشاریہ_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} ورنہ اگر (repo_read_اشاریہ(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	تسلسل_diff_اشاریہ(revs, option);
}

ساکن خالی builtin_diff_درخت(ساخت rev_info *revs,
			      صحیح argc, ثابت حرف **argv,
			      ساخت object_array_اندراج *ent0,
			      ساخت object_array_اندراج *ent1)
{
	ثابت ساخت object_id *(oid[2]);
	ساخت object_id mb_oid;
	صحیح ملاؤ_بنیاد = 0;

	جب (1 < argc) {
		ثابت حرف *arg = argv[1];
		اگر (!strcmp(arg, "--merge-base"))
			ملاؤ_بنیاد = 1;
		ورنہ
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	اگر (ملاؤ_بنیاد) {
		diff_get_ملاؤ_بنیاد(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].شے->oid;
	} ورنہ {
		صحیح تبادلہ_2 = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		اگر (ent1->شے->flags & UNINTERESTING)
			تبادلہ_2 = 1;
		oid[تبادلہ_2] = &ent0->شے->oid;
		oid[1 - تبادلہ_2] = &ent1->شے->oid;
	}
	diff_درخت_oid(oid[0], oid[1], "", &revs->diffopt);
	log_درخت_diff_flush(revs);
}

ساکن خالی builtin_diff_combined(ساخت rev_info *revs,
				  صحیح argc, ثابت حرف **argv غیر_مستعمل,
				  ساخت object_array_اندراج *ent,
				  صحیح ents, صحیح پہلا_non_والد)
{
	ساخت oid_array parents = OID_ARRAY_INIT;
	صحیح i;

	اگر (argc > 1)
		usage(builtin_diff_usage);

	اگر (پہلا_non_والد < 0)
		die(_("no merge given, only parents."));
	اگر (پہلا_non_والد >= ents)
		BUG("first_non_parent out of range: %d", پہلا_non_والد);

	diff_merges_سیٹ_dense_combined_if_unset(revs);

	برائے (i = 0; i < ents; i++) {
		اگر (i != پہلا_non_والد)
			oid_array_append(&parents, &ent[i].شے->oid);
	}
	diff_درخت_combined(&ent[پہلا_non_والد].شے->oid, &parents, revs);
	oid_array_clear(&parents);
}

ساکن خالی refresh_اشاریہ_quietly(خالی)
{
	ساخت تالا_file تالا_file = تالا_INIT;
	صحیح fd;

	fd = repo_hold_locked_اشاریہ(the_repository, &تالا_file, 0);
	اگر (fd < 0)
		واپس;
	discard_اشاریہ(the_repository->اشاریہ);
	repo_read_اشاریہ(the_repository);
	refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET|REFRESH_UNMERGED, تہی, تہی,
		      تہی);
	repo_update_اشاریہ_if_able(the_repository, &تالا_file);
}

ساکن خالی builtin_diff_files(ساخت rev_info *revs, صحیح argc, ثابت حرف **argv)
{
	غیرعلامتی صحیح options = 0;

	جب (1 < argc && argv[1][0] == '-') {
		اگر (!strcmp(argv[1], "--base"))
			revs->max_گنتی = 1;
		ورنہ اگر (!strcmp(argv[1], "--ours"))
			revs->max_گنتی = 2;
		ورنہ اگر (!strcmp(argv[1], "--theirs"))
			revs->max_گنتی = 3;
		ورنہ اگر (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		ورنہ اگر (!strcmp(argv[1], "-h"))
			usage(builtin_diff_usage);
		ورنہ {
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
	اگر (revs->max_گنتی == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_سیٹ_dense_combined_if_unset(revs);

	تیاری_work_درخت(the_repository);
	اگر (repo_read_اشاریہ_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	تسلسل_diff_files(revs, options);
}

ساخت symdiff {
	ساخت bitmap *skip;
	صحیح warn;
	ثابت حرف *بنیاد, *بایاں, *دایاں;
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
ساکن خالی symdiff_prepare(ساخت rev_info *rev, ساخت symdiff *sym)
{
	صحیح i, is_symdiff = 0, basecount = 0, othercount = 0;
	صحیح lpos = -1, rpos = -1, basepos = -1;
	ساخت bitmap *map = تہی;

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
	برائے (i = 0; i < rev->cmdline.nr; i++) {
		ساخت object *obj = rev->pending.objects[i].شے;
		انتخاب (rev->cmdline.rev[i].whence) {
		حالت REV_CMD_ملاؤ_بنیاد:
			اگر (basepos < 0)
				basepos = i;
			basecount++;
			توڑو;		/* do mark all bases */
		حالت REV_CMD_بایاں:
			اگر (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			اگر (obj->flags & متشاکل_بایاں) {
				is_symdiff = 1;
				توڑو;	/* do mark A */
			}
			جاری;
		حالت REV_CMD_دایاں:
			اگر (rpos >= 0)
				usage(builtin_diff_usage);
			rpos = i;
			جاری;	/* don't mark B */
		حالت REV_CMD_PARENTS_ONLY:
		حالت REV_CMD_REF:
		حالت REV_CMD_REV:
			othercount++;
			جاری;
		}
		اگر (!map)
			map = bitmap_new();
		bitmap_سیٹ(map, i);
	}

	/*
	 * Forbid any additional revs for both A...B and A..B.
	 */
	اگر (lpos >= 0 && othercount > 0)
		usage(builtin_diff_usage);

	اگر (!is_symdiff) {
		bitmap_free(map);
		sym->warn = 0;
		sym->skip = تہی;
		واپس;
	}

	sym->بایاں = rev->pending.objects[lpos].name;
	sym->دایاں = rev->pending.objects[rpos].name;
	اگر (basecount == 0)
		die(_("%s...%s: no merge base"), sym->بایاں, sym->دایاں);
	sym->بنیاد = rev->pending.objects[basepos].name;
	bitmap_unset(map, basepos);	/* unmark the base we want */
	sym->warn = basecount > 1;
	sym->skip = map;
}

ساکن خالی symdiff_release(ساخت symdiff *sdiff)
{
	bitmap_free(sdiff->skip);
}

صحیح cmd_diff(صحیح argc,
	     ثابت حرف **argv,
	     ثابت حرف *سابقہ,
	     ساخت repository *repo غیر_مستعمل)
{
	صحیح i;
	ساخت rev_info rev;
	ساخت object_array ent = OBJECT_ARRAY_INIT;
	صحیح پہلا_non_والد = -1;
	صحیح blobs = 0, paths = 0;
	ساخت object_array_اندراج *blob[2];
	صحیح nongit = 0, no_اشاریہ = 0;
	صحیح نتیجہ;
	ساخت symdiff sdiff;

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
	برائے (i = 1; i < argc; i++) {
		اگر (!strcmp(argv[i], "--")) {
			i++;
			توڑو;
		}
		اگر (!strcmp(argv[i], "--no-index"))
			no_اشاریہ = DIFF_NO_اشاریہ_EXPLICIT;
		اگر (argv[i][0] != '-')
			توڑو;
	}

	سابقہ = تیاری_git_directory_gently(the_repository, &nongit);

	اگر (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_بھرا_اشاریہ = 0;
	}

	اگر (!no_اشاریہ) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		اگر (nongit || ((argc == i + 2) &&
			       (!راستہ_inside_repo(the_repository, سابقہ, argv[i]) ||
				!راستہ_inside_repo(the_repository, سابقہ, argv[i + 1]))))
			no_اشاریہ = DIFF_NO_اشاریہ_IMPLICIT;
	}

	/*
	 * When operating outside of a Git repository we need to have a hash
	 * algorithm at hand so that we can generate the blob hashes. We
	 * default to SHA1 here, but may eventually want to change this to be
	 * configurable via a command line option.
	 */
	اگر (nongit)
		repo_سیٹ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, تہی);

	/*
	 * If we are ignoring the fact that our current directory may
	 * be part of a working tree controlled by a Git repository to
	 * pretend to be a "better GNU diff", we should undo the
	 * effect of the setup code that did a chdir() to the top of
	 * the working tree.  Where we came from is recorded in the
	 * prefix.
	 */
	اگر (no_اشاریہ && سابقہ) {
		اگر (chdir(سابقہ))
			die(_("cannot come back to cwd"));
		سابقہ = تہی;
	}

	سابقہ = precompose_argv_سابقہ(argc, argv, سابقہ);

	repo_init_revisions(the_repository, &rev, سابقہ);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	اگر (no_اشاریہ)
		باہر_نکلو(diff_no_اشاریہ(&rev, the_repository->hash_algo,
				   no_اشاریہ == DIFF_NO_اشاریہ_IMPLICIT,
				   argc, argv));


	/*
	 * Otherwise, we are doing the usual "git" diff; set up any
	 * further defaults that apply to regular diffs.
	 */
	rev.diffopt.skip_stat_unmatch = !!diff_auto_refresh_اشاریہ;

	/*
	 * Default to intent-to-add entries invisible in the
	 * index. This makes them show up as new files in diff-files
	 * and not at all in diff-cached.
	 */
	rev.diffopt.ita_invisible_اندر_اشاریہ = 1;

	اگر (nongit)
		die(_("Not a git repository"));
	argc = تیاری_revisions(argc, argv, &rev, تہی);
	اگر (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_تیاری_مکمل(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	rev.diffopt.rotate_مقصد_strict = 1;

	تیاری_diff_pager(&rev.diffopt);

	/*
	 * Do we have --cached and not have a pending object, then
	 * default to HEAD by hand.  Eek.
	 */
	اگر (!rev.pending.nr) {
		صحیح i;
		برائے (i = 1; i < argc; i++) {
			ثابت حرف *arg = argv[i];
			اگر (!strcmp(arg, "--"))
				توڑو;
			ورنہ اگر (!strcmp(arg, "--cached") ||
				 !strcmp(arg, "--staged")) {
				جوڑو_سر_مقصد_pending(&rev);
				اگر (!rev.pending.nr) {
					ساخت درخت *درخت;
					درخت = lookup_درخت(the_repository,
							   the_repository->hash_algo->خالی_خانے_درخت);
					جوڑو_pending_object(&rev, &درخت->object, "HEAD");
				}
				توڑو;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	برائے (i = 0; i < rev.pending.nr; i++) {
		ساخت object_array_اندراج *اندراج_2 = &rev.pending.objects[i];
		ساخت object *obj = اندراج_2->شے;
		ثابت حرف *name = اندراج_2->name;
		صحیح flags = (obj->flags & UNINTERESTING);
		اگر (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_لیبل(the_repository, obj, تہی, 0);
		اگر (!obj)
			die(_("invalid object '%s' given."), name);
		اگر (obj->type == OBJ_COMMIT) {
			ساخت درخت *درخت = repo_get_commit_درخت(
				the_repository, (ساخت commit *)obj);
			اگر (!درخت)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &درخت->object;
		}

		اگر (obj->type == OBJ_درخت) {
			اگر (sdiff.skip && bitmap_get(sdiff.skip, i))
				جاری;
			obj->flags |= flags;
			جوڑو_object_array(obj, name, &ent);
			اگر (پہلا_non_والد < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				پہلا_non_والد = ent.nr - 1;
		} ورنہ اگر (obj->type == OBJ_BLOB) {
			اگر (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = اندراج_2;
			blobs++;

		} ورنہ {
			die(_("unhandled object '%s' given."), name);
		}
	}
	اگر (rev.prune_ڈیٹا.nr)
		paths += rev.prune_ڈیٹا.nr;

	/*
	 * Now, do the arguments look reasonable?
	 */
	اگر (!ent.nr) {
		انتخاب (blobs) {
		حالت 0:
			builtin_diff_files(&rev, argc, argv);
			توڑو;
		حالت 1:
			اگر (paths != 1)
				usage(builtin_diff_usage);
			builtin_diff_b_f(&rev, argc, argv, blob);
			توڑو;
		حالت 2:
			اگر (paths)
				usage(builtin_diff_usage);
			builtin_diff_blobs(&rev, argc, argv, blob);
			توڑو;
		طےشدہ:
			usage(builtin_diff_usage);
		}
	}
	ورنہ اگر (blobs)
		usage(builtin_diff_usage);
	ورنہ اگر (ent.nr == 1)
		builtin_diff_اشاریہ(&rev, argc, argv);
	ورنہ اگر (ent.nr == 2) {
		اگر (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.بایاں, sdiff.دایاں, sdiff.بنیاد);
		builtin_diff_درخت(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} ورنہ
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      پہلا_non_والد);
	نتیجہ = diff_نتیجہ_رمز(&rev);
	اگر (1 < rev.diffopt.skip_stat_unmatch)
		refresh_اشاریہ_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	واپس نتیجہ;
}
