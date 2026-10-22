#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن ثابت حرف *ثابت ادغام_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	عدم
};

ساكن صحيح label_cb(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	ساكن صحيح label_شمار = 0;
	ثابت حرف **نامها = (ثابت حرف **)opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	إذا (label_شمار >= 3)
		أرجع error("too many labels on the command line");
	نامها[label_شمار++] = arg;
	أرجع 0;
}

ساكن صحيح مجموعه_diff_algorithm(xpparam_t *xpp,
			      ثابت حرف *alg)
{
	دراز diff_algorithm = parse_algorithm_مقدار(alg);
	إذا (diff_algorithm < 0)
		أرجع -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_ماسک) | diff_algorithm;
	أرجع 0;
}

ساكن صحيح diff_algorithm_cb(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	xpparam_t *xpp = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	إذا (مجموعه_diff_algorithm(xpp, arg))
		أرجع error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	أرجع 0;
}

صحيح cmd_ادغام_file(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *پیشوند,
		   بنية repository *repo)
{
	ثابت حرف *نامها[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t نتیجه = { 0 };
	xmparam_t xmp = { 0 };
	صحيح ret = 0, i = 0, مقصد_stdout = 0, object_id = 0;
	صحيح quiet = 0;
	بنية option options[] = {
		OPT_BOOL('p', "stdout", &مقصد_stdout, N_("send results to standard output")),
		OPT_BOOL(0,   "object-id", &object_id, N_("use object IDs instead of filenames")),
		OPT_مجموعه_INT(0, "diff3", &xmp.style, N_("use a diff3 based merge"), XDL_ادغام_DIFF3),
		OPT_مجموعه_INT(0, "zdiff3", &xmp.style, N_("use a zealous diff3 based merge"),
				XDL_ادغام_ZEALOUS_DIFF3),
		OPT_مجموعه_INT(0, "ours", &xmp.favor, N_("for conflicts, use our version"),
			    XDL_ادغام_FAVOR_OURS),
		OPT_مجموعه_INT(0, "theirs", &xmp.favor, N_("for conflicts, use their version"),
			    XDL_ادغام_FAVOR_THEIRS),
		OPT_مجموعه_INT(0, "union", &xmp.favor, N_("for conflicts, use a union version"),
			    XDL_ادغام_FAVOR_UNION),
		OPT_CALLBACK_F(0, "diff-algorithm", &xmp.xpp, N_("<algorithm>"),
			     N_("choose a diff algorithm"),
			     PARSE_OPT_NONEG, diff_algorithm_cb),
		OPT_INTEGER(0, "marker-size", &xmp.marker_اندازه,
			    N_("for conflicts, use this marker size")),
		OPT__QUIET(&quiet, N_("do not warn about conflicts")),
		OPT_CALLBACK('L', عدم, نامها, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_انتها(),
	};

	xmp.level = XDL_ادغام_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, عدم);
	إذا (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, پیشوند, options, ادغام_file_usage, 0);
	إذا (argc != 3)
		usage_with_options(ادغام_file_usage, options);
	إذا (quiet) {
		إذا (!freopen("/dev/null", "w", stderr))
			أرجع error_errno("failed to redirect stderr to /dev/null");
	}

	إذا (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		راهاندازی_git_directory(the_repository);

	لكل (i = 0; i < 3; i++) {
		حرف *fname;
		بنية object_id oid;
		mmfile_t *mmf = mmfs + i;

		إذا (!نامها[i])
			نامها[i] = argv[i];

		fname = پیشوند_filename(پیشوند, argv[i]);

		إذا (object_id) {
			إذا (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			وإلا إذا (!oideq(&oid, the_hash_algo->خالی_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			وإلا
				read_mmfile(mmf, "/dev/null");
		} وإلا إذا (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		إذا (ret != -1 && (mmf->اندازه > MAX_XDIFF_اندازه ||
		    میانگیر_is_دودویی(mmf->ptr, mmf->اندازه))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		حرر(fname);
		إذا (ret)
			اذهب cleanup;

	}

	xmp.ancestor = نامها[1];
	xmp.file1 = نامها[0];
	xmp.file2 = نامها[2];
	ret = xdl_ادغام(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &نتیجه);

	إذا (ret >= 0) {
		إذا (object_id && !مقصد_stdout) {
			بنية object_id oid;
			إذا (نتیجه.اندازه) {
				إذا (odb_write_object(the_repository->objects, نتیجه.ptr,
						     نتیجه.اندازه, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} وإلا {
				oidcpy(&oid, the_hash_algo->خالی_blob);
			}
			إذا (ret >= 0)
				چاپ_کن("%s\n", oid_مقصد_hex(&oid));
		} وإلا {
			ثابت حرف *filename = argv[0];
			حرف *fpath = پیشوند_filename(پیشوند, argv[0]);
			FILE *f = مقصد_stdout ? stdout : fopen(fpath, "wb");

			إذا (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			وإلا إذا (نتیجه.اندازه &&
				 fwrite(نتیجه.ptr, نتیجه.اندازه, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			وإلا إذا (fclose(f))
				ret = error_errno("Could not close %s", filename);
			حرر(fpath);
		}
		حرر(نتیجه.ptr);
	}

	إذا (ret > 127)
		ret = 127;

cleanup:
	لكل (i = 0; i < 3; i++)
		حرر(mmfs[i].ptr);

	أرجع ret;
}
