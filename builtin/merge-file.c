#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "diff.h"
#شامل "hex.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "config.h"
#شامل "gettext.h"
#شامل "setup.h"
#شامل "xdiff/xdiff.h"
#شامل "xdiff-interface.h"
#شامل "parse-options.h"

ساکن ثابت حرف *ثابت ملاؤ_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	تہی
};

ساکن صحیح label_cb(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساکن صحیح label_گنتی = 0;
	ثابت حرف **نام = (ثابت حرف **)opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (label_گنتی >= 3)
		واپس error("too many labels on the command line");
	نام[label_گنتی++] = arg;
	واپس 0;
}

ساکن صحیح سیٹ_diff_algorithm(xpparam_t *xpp,
			      ثابت حرف *alg)
{
	طویل diff_algorithm = parse_algorithm_قدر(alg);
	اگر (diff_algorithm < 0)
		واپس -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_نقاب) | diff_algorithm;
	واپس 0;
}

ساکن صحیح diff_algorithm_cb(ثابت ساخت option *opt,
				ثابت حرف *arg, صحیح unset)
{
	xpparam_t *xpp = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (سیٹ_diff_algorithm(xpp, arg))
		واپس error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	واپس 0;
}

صحیح cmd_ملاؤ_file(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ,
		   ساخت repository *repo)
{
	ثابت حرف *نام[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t نتیجہ = { 0 };
	xmparam_t xmp = { 0 };
	صحیح ret = 0, i = 0, مقصد_stdout = 0, object_id = 0;
	صحیح quiet = 0;
	ساخت option options[] = {
		OPT_BOOL('p', "stdout", &مقصد_stdout, N_("send results to standard output")),
		OPT_BOOL(0,   "object-id", &object_id, N_("use object IDs instead of filenames")),
		OPT_سیٹ_INT(0, "diff3", &xmp.style, N_("use a diff3 based merge"), XDL_ملاؤ_DIFF3),
		OPT_سیٹ_INT(0, "zdiff3", &xmp.style, N_("use a zealous diff3 based merge"),
				XDL_ملاؤ_ZEALOUS_DIFF3),
		OPT_سیٹ_INT(0, "ours", &xmp.favor, N_("for conflicts, use our version"),
			    XDL_ملاؤ_FAVOR_OURS),
		OPT_سیٹ_INT(0, "theirs", &xmp.favor, N_("for conflicts, use their version"),
			    XDL_ملاؤ_FAVOR_THEIRS),
		OPT_سیٹ_INT(0, "union", &xmp.favor, N_("for conflicts, use a union version"),
			    XDL_ملاؤ_FAVOR_UNION),
		OPT_CALLBACK_F(0, "diff-algorithm", &xmp.xpp, N_("<algorithm>"),
			     N_("choose a diff algorithm"),
			     PARSE_OPT_NONEG, diff_algorithm_cb),
		OPT_INTEGER(0, "marker-size", &xmp.marker_سائز,
			    N_("for conflicts, use this marker size")),
		OPT__QUIET(&quiet, N_("do not warn about conflicts")),
		OPT_CALLBACK('L', تہی, نام, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_انتہا(),
	};

	xmp.level = XDL_ملاؤ_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, تہی);
	اگر (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, سابقہ, options, ملاؤ_file_usage, 0);
	اگر (argc != 3)
		usage_with_options(ملاؤ_file_usage, options);
	اگر (quiet) {
		اگر (!freopen("/dev/null", "w", stderr))
			واپس error_errno("failed to redirect stderr to /dev/null");
	}

	اگر (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		تیاری_git_directory(the_repository);

	برائے (i = 0; i < 3; i++) {
		حرف *fname;
		ساخت object_id oid;
		mmfile_t *mmf = mmfs + i;

		اگر (!نام[i])
			نام[i] = argv[i];

		fname = سابقہ_filename(سابقہ, argv[i]);

		اگر (object_id) {
			اگر (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			ورنہ اگر (!oideq(&oid, the_hash_algo->خالی_خانے_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			ورنہ
				read_mmfile(mmf, "/dev/null");
		} ورنہ اگر (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		اگر (ret != -1 && (mmf->سائز > MAX_XDIFF_سائز ||
		    بفر_is_ثنائی(mmf->ptr, mmf->سائز))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		آزاد(fname);
		اگر (ret)
			جاؤ cleanup;

	}

	xmp.ancestor = نام[1];
	xmp.file1 = نام[0];
	xmp.file2 = نام[2];
	ret = xdl_ملاؤ(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &نتیجہ);

	اگر (ret >= 0) {
		اگر (object_id && !مقصد_stdout) {
			ساخت object_id oid;
			اگر (نتیجہ.سائز) {
				اگر (odb_write_object(the_repository->objects, نتیجہ.ptr,
						     نتیجہ.سائز, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} ورنہ {
				oidcpy(&oid, the_hash_algo->خالی_خانے_blob);
			}
			اگر (ret >= 0)
				لکھو("%s\n", oid_مقصد_hex(&oid));
		} ورنہ {
			ثابت حرف *filename = argv[0];
			حرف *fpath = سابقہ_filename(سابقہ, argv[0]);
			FILE *f = مقصد_stdout ? stdout : fopen(fpath, "wb");

			اگر (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			ورنہ اگر (نتیجہ.سائز &&
				 fwrite(نتیجہ.ptr, نتیجہ.سائز, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			ورنہ اگر (fclose(f))
				ret = error_errno("Could not close %s", filename);
			آزاد(fpath);
		}
		آزاد(نتیجہ.ptr);
	}

	اگر (ret > 127)
		ret = 127;

cleanup:
	برائے (i = 0; i < 3; i++)
		آزاد(mmfs[i].ptr);

	واپس ret;
}
