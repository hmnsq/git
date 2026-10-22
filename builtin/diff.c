/*
 * Builtin "git diff"
 *
 * Copyright (c) 2006 Junio C Hamano
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کرو DIFF_NO_اشاریہ_EXPLICIT 1
#تعریف_کرو DIFF_NO_اشاریہ_IMPLICIT 2

ساكن ثابت حرف builtin_diff_usage[] =
"git diff [<options>] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] --cached [--merge-base] [<commit>] [--] [<path>...]\n"
"   or: git diff [<options>] [--merge-base] <commit> [<commit>...] <commit> [--] [<path>...]\n"
"   or: git diff [<options>] <commit>...<commit> [--] [<path>...]\n"
"   or: git diff [<options>] <blob> <blob>\n"
"   or: git diff [<options>] --no-index [--] <path> <path> [<pathspec>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ساكن ثابت حرف *blob_رستہ(بنية object_array_اندراج *اندراج)
{
	أرجع اندراج->رستہ ? اندراج->رستہ : اندراج->name;
}

ساكن فراغ stuff_change(بنية diff_options *opt,
			 بنانشانی old_منوال, بنانشانی new_منوال,
			 ثابت بنية object_id *old_oid,
			 ثابت بنية object_id *new_oid,
			 صحيح old_oid_valid,
			 صحيح new_oid_valid,
			 ثابت حرف *old_رستہ,
			 ثابت حرف *new_رستہ)
{
	بنية diff_filespec *one, *two;

	إذا (!is_null_oid(old_oid) && !is_null_oid(new_oid) &&
	    oideq(old_oid, new_oid) && (old_منوال == new_منوال))
		أرجع;

	إذا (opt->flags.الٹ_diff) {
		وٹاندرا(old_منوال, new_منوال);
		وٹاندرا(old_oid, new_oid);
		وٹاندرا(old_رستہ, new_رستہ);
	}

	إذا (opt->اگیتر &&
	    (لڑی_مقابلہ(old_رستہ, opt->اگیتر, opt->اگیتر_لمائی) ||
	     لڑی_مقابلہ(new_رستہ, opt->اگیتر, opt->اگیتر_لمائی)))
		أرجع;

	one = alloc_filespec(old_رستہ);
	two = alloc_filespec(new_رستہ);
	fill_filespec(one, old_oid, old_oid_valid, old_منوال);
	fill_filespec(two, new_oid, new_oid_valid, new_منوال);

	diff_قطار(&diff_queued_diff, one, two);
}

ساكن فراغ builtin_diff_b_f(بنية rev_info *revs,
			     صحيح argc, ثابت حرف **argv بن_ورتیا,
			     بنية object_array_اندراج **blob)
{
	/* Blob vs file in the working tree*/
	بنية stat st;
	ثابت حرف *رستہ;

	إذا (argc > 1)
		usage(builtin_diff_usage);

	GUARD_PATHSPEC(&revs->prune_انکڑے, PATHSPEC_FROMTOP | PATHSPEC_LITERAL);
	رستہ = revs->prune_انکڑے.اشیاء[0].میل;

	إذا (lstat(رستہ, &st))
		die_errno(_("failed to stat '%s'"), رستہ);
	إذا (!(S_ISREG(st.st_منوال) || S_ISLNK(st.st_منوال)))
		die(_("'%s': not a regular file or symlink"), رستہ);

	diff_سیٹ_mnemonic_اگیتر(&revs->diffopt, "o/", "w/");

	إذا (blob[0]->منوال == S_IFINVALID)
		blob[0]->منوال = canon_منوال(st.st_منوال);

	stuff_change(&revs->diffopt,
		     blob[0]->منوال, canon_منوال(st.st_منوال),
		     &blob[0]->شے->oid, null_oid(the_hash_algo),
		     1, 0,
		     blob[0]->رستہ ? blob[0]->رستہ : رستہ,
		     رستہ);
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ساكن فراغ builtin_diff_blobs(بنية rev_info *revs,
			       صحيح argc, ثابت حرف **argv بن_ورتیا,
			       بنية object_array_اندراج **blob)
{
	ثابت بنانشانی منوال = canon_منوال(S_IFREG | 0644);

	إذا (argc > 1)
		usage(builtin_diff_usage);

	إذا (blob[0]->منوال == S_IFINVALID)
		blob[0]->منوال = منوال;

	إذا (blob[1]->منوال == S_IFINVALID)
		blob[1]->منوال = منوال;

	stuff_change(&revs->diffopt,
		     blob[0]->منوال, blob[1]->منوال,
		     &blob[0]->شے->oid, &blob[1]->شے->oid,
		     1, 1,
		     blob_رستہ(blob[0]), blob_رستہ(blob[1]));
	diffcore_std(&revs->diffopt);
	diff_flush(&revs->diffopt);
}

ساكن فراغ builtin_diff_اشاریہ(بنية rev_info *revs,
			       صحيح argc, ثابت حرف **argv)
{
	بنانشانی صحيح option = 0;
	طالما (1 < argc) {
		ثابت حرف *arg = argv[1];
		إذا (!strcmp(arg, "--cached") || !strcmp(arg, "--staged"))
			option |= DIFF_اشاریہ_CACHED;
		وإلا إذا (!strcmp(arg, "--merge-base"))
			option |= DIFF_اشاریہ_ملاؤ_ادھار;
		وإلا
			usage(builtin_diff_usage);
		argv++; argc--;
	}
	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	إذا (revs->pending.nr != 1 ||
	    revs->max_گنتی != -1 || revs->min_age != -1 ||
	    revs->max_age != -1)
		usage(builtin_diff_usage);
	إذا (!(option & DIFF_اشاریہ_CACHED)) {
		تیاری_work_درخت(the_repository);
		إذا (repo_read_اشاریہ_preload(the_repository,
					    &revs->diffopt.pathspec, 0) < 0) {
			die_errno("repo_read_index_preload");
		}
	} وإلا إذا (repo_read_اشاریہ(the_repository) < 0) {
		die_errno("repo_read_cache");
	}
	لگاتار_diff_اشاریہ(revs, option);
}

ساكن فراغ builtin_diff_درخت(بنية rev_info *revs,
			      صحيح argc, ثابت حرف **argv,
			      بنية object_array_اندراج *ent0,
			      بنية object_array_اندراج *ent1)
{
	ثابت بنية object_id *(oid[2]);
	بنية object_id mb_oid;
	صحيح ملاؤ_ادھار = 0;

	طالما (1 < argc) {
		ثابت حرف *arg = argv[1];
		إذا (!strcmp(arg, "--merge-base"))
			ملاؤ_ادھار = 1;
		وإلا
			usage(builtin_diff_usage);
		argv++; argc--;
	}

	إذا (ملاؤ_ادھار) {
		diff_get_ملاؤ_ادھار(revs, &mb_oid);
		oid[0] = &mb_oid;
		oid[1] = &revs->pending.objects[1].شے->oid;
	} وإلا {
		صحيح وٹاندرا_2 = 0;

		/*
		 * We saw two trees, ent0 and ent1.  If ent1 is uninteresting,
		 * swap them.
		 */
		إذا (ent1->شے->flags & UNINTERESTING)
			وٹاندرا_2 = 1;
		oid[وٹاندرا_2] = &ent0->شے->oid;
		oid[1 - وٹاندرا_2] = &ent1->شے->oid;
	}
	diff_درخت_oid(oid[0], oid[1], "", &revs->diffopt);
	log_درخت_diff_flush(revs);
}

ساكن فراغ builtin_diff_combined(بنية rev_info *revs,
				  صحيح argc, ثابت حرف **argv بن_ورتیا,
				  بنية object_array_اندراج *ent,
				  صحيح ents, صحيح پہلا_non_ماپا)
{
	بنية oid_array parents = OID_ARRAY_INIT;
	صحيح i;

	إذا (argc > 1)
		usage(builtin_diff_usage);

	إذا (پہلا_non_ماپا < 0)
		die(_("no merge given, only parents."));
	إذا (پہلا_non_ماپا >= ents)
		BUG("first_non_parent out of range: %d", پہلا_non_ماپا);

	diff_merges_سیٹ_dense_combined_if_unset(revs);

	لكل (i = 0; i < ents; i++) {
		إذا (i != پہلا_non_ماپا)
			oid_array_append(&parents, &ent[i].شے->oid);
	}
	diff_درخت_combined(&ent[پہلا_non_ماپا].شے->oid, &parents, revs);
	oid_array_clear(&parents);
}

ساكن فراغ refresh_اشاریہ_quietly(فراغ)
{
	بنية جندرا_file جندرا_file = جندرا_INIT;
	صحيح fd;

	fd = repo_hold_locked_اشاریہ(the_repository, &جندرا_file, 0);
	إذا (fd < 0)
		أرجع;
	discard_اشاریہ(the_repository->اشاریہ);
	repo_read_اشاریہ(the_repository);
	refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET|REFRESH_UNMERGED, عدم, عدم,
		      عدم);
	repo_update_اشاریہ_if_able(the_repository, &جندرا_file);
}

ساكن فراغ builtin_diff_files(بنية rev_info *revs, صحيح argc, ثابت حرف **argv)
{
	بنانشانی صحيح options = 0;

	طالما (1 < argc && argv[1][0] == '-') {
		إذا (!strcmp(argv[1], "--base"))
			revs->max_گنتی = 1;
		وإلا إذا (!strcmp(argv[1], "--ours"))
			revs->max_گنتی = 2;
		وإلا إذا (!strcmp(argv[1], "--theirs"))
			revs->max_گنتی = 3;
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
	إذا (revs->max_گنتی == -1 &&
	    (revs->diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_سیٹ_dense_combined_if_unset(revs);

	تیاری_work_درخت(the_repository);
	إذا (repo_read_اشاریہ_preload(the_repository, &revs->diffopt.pathspec,
				    0) < 0) {
		die_errno("repo_read_index_preload");
	}
	لگاتار_diff_files(revs, options);
}

بنية symdiff {
	بنية bitmap *skip;
	صحيح warn;
	ثابت حرف *ادھار, *بایاں, *دایاں;
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
		بنية object *obj = rev->pending.objects[i].شے;
		بدل (rev->cmdline.rev[i].whence) {
		حالة REV_CMD_ملاؤ_ادھار:
			إذا (basepos < 0)
				basepos = i;
			basecount++;
			اكسر;		/* do mark all bases */
		حالة REV_CMD_بایاں:
			إذا (lpos >= 0)
				usage(builtin_diff_usage);
			lpos = i;
			إذا (obj->flags & متشاکل_بایاں) {
				is_symdiff = 1;
				اكسر;	/* do mark A */
			}
			استمر;
		حالة REV_CMD_دایاں:
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
		bitmap_سیٹ(map, i);
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

	sym->بایاں = rev->pending.objects[lpos].name;
	sym->دایاں = rev->pending.objects[rpos].name;
	إذا (basecount == 0)
		die(_("%s...%s: no merge base"), sym->بایاں, sym->دایاں);
	sym->ادھار = rev->pending.objects[basepos].name;
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
	     ثابت حرف *اگیتر,
	     بنية repository *repo بن_ورتیا)
{
	صحيح i;
	بنية rev_info rev;
	بنية object_array ent = OBJECT_ARRAY_INIT;
	صحيح پہلا_non_ماپا = -1;
	صحيح blobs = 0, paths = 0;
	بنية object_array_اندراج *blob[2];
	صحيح nongit = 0, no_اشاریہ = 0;
	صحيح نتیجہ;
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
			no_اشاریہ = DIFF_NO_اشاریہ_EXPLICIT;
		إذا (argv[i][0] != '-')
			اكسر;
	}

	اگیتر = تیاری_git_directory_gently(the_repository, &nongit);

	إذا (!nongit) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_بھریا_اشاریہ = 0;
	}

	إذا (!no_اشاریہ) {
		/*
		 * Treat git diff with at least one path outside of the
		 * repo the same as if the command would have been executed
		 * outside of a git repository.  In this case it behaves
		 * the same way as "git diff --no-index <a> <b>", which acts
		 * as a colourful "diff" replacement.
		 */
		إذا (nongit || ((argc == i + 2) &&
			       (!رستہ_inside_repo(the_repository, اگیتر, argv[i]) ||
				!رستہ_inside_repo(the_repository, اگیتر, argv[i + 1]))))
			no_اشاریہ = DIFF_NO_اشاریہ_IMPLICIT;
	}

	/*
	 * When operating outside of a Git repository we need to have a hash
	 * algorithm at hand so that we can generate the blob hashes. We
	 * default to SHA1 here, but may eventually want to change this to be
	 * configurable via a command line option.
	 */
	إذا (nongit)
		repo_سیٹ_hash_algo(the_repository, GIT_HASH_DEFAULT);

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
	إذا (no_اشاریہ && اگیتر) {
		إذا (chdir(اگیتر))
			die(_("cannot come back to cwd"));
		اگیتر = عدم;
	}

	اگیتر = precompose_argv_اگیتر(argc, argv, اگیتر);

	repo_init_revisions(the_repository, &rev, اگیتر);

	/* Set up defaults that will apply to both no-index and regular diffs. */
	init_diffstat_widths(&rev.diffopt);
	rev.diffopt.flags.allow_external = 1;
	rev.diffopt.flags.allow_textconv = 1;

	/* If this is a no-index diff, just run it and exit there. */
	إذا (no_اشاریہ)
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
	rev.diffopt.ita_invisible_وچ_اشاریہ = 1;

	إذا (nongit)
		die(_("Not a git repository"));
	argc = تیاری_revisions(argc, argv, &rev, عدم);
	إذا (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_تیاری_پورا(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	rev.diffopt.rotate_منزل_strict = 1;

	تیاری_diff_pager(&rev.diffopt);

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
				جوڑو_سر_منزل_pending(&rev);
				إذا (!rev.pending.nr) {
					بنية درخت *درخت;
					درخت = lookup_درخت(the_repository,
							   the_repository->hash_algo->خالی_تھاواں_درخت);
					جوڑو_pending_object(&rev, &درخت->object, "HEAD");
				}
				اكسر;
			}
		}
	}

	symdiff_prepare(&rev, &sdiff);
	لكل (i = 0; i < rev.pending.nr; i++) {
		بنية object_array_اندراج *اندراج = &rev.pending.objects[i];
		بنية object *obj = اندراج->شے;
		ثابت حرف *name = اندراج->name;
		صحيح flags = (obj->flags & UNINTERESTING);
		إذا (!obj->parsed)
			obj = parse_object(the_repository, &obj->oid);
		obj = deref_ٹیگ(the_repository, obj, عدم, 0);
		إذا (!obj)
			die(_("invalid object '%s' given."), name);
		إذا (obj->type == OBJ_COMMIT) {
			بنية درخت *درخت = repo_get_commit_درخت(
				the_repository, (بنية commit *)obj);
			إذا (!درخت)
				die(_("unable to read tree object for commit '%s'"), name);
			obj = &درخت->object;
		}

		إذا (obj->type == OBJ_درخت) {
			إذا (sdiff.skip && bitmap_get(sdiff.skip, i))
				استمر;
			obj->flags |= flags;
			جوڑو_object_array(obj, name, &ent);
			إذا (پہلا_non_ماپا < 0 &&
			    (i >= rev.cmdline.nr || /* HEAD by hand. */
			     rev.cmdline.rev[i].whence != REV_CMD_PARENTS_ONLY))
				پہلا_non_ماپا = ent.nr - 1;
		} وإلا إذا (obj->type == OBJ_BLOB) {
			إذا (2 <= blobs)
				die(_("more than two blobs given: '%s'"), name);
			blob[blobs] = اندراج;
			blobs++;

		} وإلا {
			die(_("unhandled object '%s' given."), name);
		}
	}
	إذا (rev.prune_انکڑے.nr)
		paths += rev.prune_انکڑے.nr;

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
		builtin_diff_اشاریہ(&rev, argc, argv);
	وإلا إذا (ent.nr == 2) {
		إذا (sdiff.warn)
			warning(_("%s...%s: multiple merge bases, using %s"),
				sdiff.بایاں, sdiff.دایاں, sdiff.ادھار);
		builtin_diff_درخت(&rev, argc, argv,
				  &ent.objects[0], &ent.objects[1]);
	} وإلا
		builtin_diff_combined(&rev, argc, argv,
				      ent.objects, ent.nr,
				      پہلا_non_ماپا);
	نتیجہ = diff_نتیجہ_کوڈ(&rev);
	إذا (1 < rev.diffopt.skip_stat_unmatch)
		refresh_اشاریہ_quietly();
	release_revisions(&rev);
	object_array_clear(&ent);
	symdiff_release(&sdiff);
	أرجع نتیجہ;
}
