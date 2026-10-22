#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "diff.h"
#ضمن "hex.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "config.h"
#ضمن "gettext.h"
#ضمن "setup.h"
#ضمن "xdiff/xdiff.h"
#ضمن "xdiff-interface.h"
#ضمن "parse-options.h"

ساكن ثابت حرف *ثابت ملايو_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	عدم
};

ساكن صحيح label_cb(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	ساكن صحيح label_ڳڻپ = 0;
	ثابت حرف **نالا = (ثابت حرف **)opt->قدر;

	BUG_ON_OPT_NEG(unset);

	إذا (label_ڳڻپ >= 3)
		أرجع error("too many labels on the command line");
	نالا[label_ڳڻپ++] = arg;
	أرجع 0;
}

ساكن صحيح سيٽ_diff_algorithm(xpparam_t *xpp,
			      ثابت حرف *alg)
{
	ڊگهو diff_algorithm = parse_algorithm_قدر(alg);
	إذا (diff_algorithm < 0)
		أرجع -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_نقاب) | diff_algorithm;
	أرجع 0;
}

ساكن صحيح diff_algorithm_cb(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	xpparam_t *xpp = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	إذا (سيٽ_diff_algorithm(xpp, arg))
		أرجع error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	أرجع 0;
}

صحيح cmd_ملايو_file(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *اڳياڙي,
		   بنية repository *repo)
{
	ثابت حرف *نالا[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t نتيجو = { 0 };
	xmparam_t xmp = { 0 };
	صحيح ret = 0, i = 0, منزل_stdout = 0, object_id = 0;
	صحيح quiet = 0;
	بنية option options[] = {
		OPT_BOOL('p', "stdout", &منزل_stdout, N_("send results to standard output")),
		OPT_BOOL(0,   "object-id", &object_id, N_("use object IDs instead of filenames")),
		OPT_سيٽ_INT(0, "diff3", &xmp.style, N_("use a diff3 based merge"), XDL_ملايو_DIFF3),
		OPT_سيٽ_INT(0, "zdiff3", &xmp.style, N_("use a zealous diff3 based merge"),
				XDL_ملايو_ZEALOUS_DIFF3),
		OPT_سيٽ_INT(0, "ours", &xmp.favor, N_("for conflicts, use our version"),
			    XDL_ملايو_FAVOR_OURS),
		OPT_سيٽ_INT(0, "theirs", &xmp.favor, N_("for conflicts, use their version"),
			    XDL_ملايو_FAVOR_THEIRS),
		OPT_سيٽ_INT(0, "union", &xmp.favor, N_("for conflicts, use a union version"),
			    XDL_ملايو_FAVOR_UNION),
		OPT_CALLBACK_F(0, "diff-algorithm", &xmp.xpp, N_("<algorithm>"),
			     N_("choose a diff algorithm"),
			     PARSE_OPT_NONEG, diff_algorithm_cb),
		OPT_INTEGER(0, "marker-size", &xmp.marker_سائيز,
			    N_("for conflicts, use this marker size")),
		OPT__QUIET(&quiet, N_("do not warn about conflicts")),
		OPT_CALLBACK('L', عدم, نالا, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_پڇاڙي(),
	};

	xmp.level = XDL_ملايو_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, عدم);
	إذا (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, اڳياڙي, options, ملايو_file_usage, 0);
	إذا (argc != 3)
		usage_with_options(ملايو_file_usage, options);
	إذا (quiet) {
		إذا (!freopen("/dev/null", "w", stderr))
			أرجع error_errno("failed to redirect stderr to /dev/null");
	}

	إذا (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		تياري_git_directory(the_repository);

	لكل (i = 0; i < 3; i++) {
		حرف *fname;
		بنية object_id oid;
		mmfile_t *mmf = mmfs + i;

		إذا (!نالا[i])
			نالا[i] = argv[i];

		fname = اڳياڙي_filename(اڳياڙي, argv[i]);

		إذا (object_id) {
			إذا (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			وإلا إذا (!oideq(&oid, the_hash_algo->خالي_جايون_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			وإلا
				read_mmfile(mmf, "/dev/null");
		} وإلا إذا (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		إذا (ret != -1 && (mmf->سائيز > MAX_XDIFF_سائيز ||
		    بفر_is_ٻٽو(mmf->ptr, mmf->سائيز))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		حرر(fname);
		إذا (ret)
			اذهب cleanup;

	}

	xmp.ancestor = نالا[1];
	xmp.file1 = نالا[0];
	xmp.file2 = نالا[2];
	ret = xdl_ملايو(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &نتيجو);

	إذا (ret >= 0) {
		إذا (object_id && !منزل_stdout) {
			بنية object_id oid;
			إذا (نتيجو.سائيز) {
				إذا (odb_write_object(the_repository->objects, نتيجو.ptr,
						     نتيجو.سائيز, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} وإلا {
				oidcpy(&oid, the_hash_algo->خالي_جايون_blob);
			}
			إذا (ret >= 0)
				لکو("%s\n", oid_منزل_hex(&oid));
		} وإلا {
			ثابت حرف *filename = argv[0];
			حرف *fpath = اڳياڙي_filename(اڳياڙي, argv[0]);
			FILE *f = منزل_stdout ? stdout : fopen(fpath, "wb");

			إذا (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			وإلا إذا (نتيجو.سائيز &&
				 fwrite(نتيجو.ptr, نتيجو.سائيز, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			وإلا إذا (fclose(f))
				ret = error_errno("Could not close %s", filename);
			حرر(fpath);
		}
		حرر(نتيجو.ptr);
	}

	إذا (ret > 127)
		ret = 127;

cleanup:
	لكل (i = 0; i < 3; i++)
		حرر(mmfs[i].ptr);

	أرجع ret;
}
