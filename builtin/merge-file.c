#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

ایستا ثابت نویسه *ثابت merge_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	پوچ
};

ایستا صحیح label_cb(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ایستا صحیح label_شمار = 0;
	ثابت نویسه **names = (ثابت نویسه **)opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (label_شمار >= 3)
		بازگشت error("too many labels on the command line");
	names[label_شمار++] = arg;
	بازگشت 0;
}

ایستا صحیح set_diff_algorithm(xpparam_t *xpp,
			      ثابت نویسه *alg)
{
	long diff_algorithm = parse_algorithm_مقدار(alg);
	اگر (diff_algorithm < 0)
		بازگشت -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_MASK) | diff_algorithm;
	بازگشت 0;
}

ایستا صحیح diff_algorithm_cb(ثابت ساختار option *opt,
				ثابت نویسه *arg, صحیح unset)
{
	xpparam_t *xpp = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (set_diff_algorithm(xpp, arg))
		بازگشت error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	بازگشت 0;
}

صحیح cmd_merge_file(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix,
		   ساختار repository *repo)
{
	ثابت نویسه *names[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t result = { 0 };
	xmparam_t xmp = { 0 };
	صحیح ret = 0, i = 0, to_stdout = 0, object_id = 0;
	صحیح quiet = 0;
	ساختار option options[] = {
		OPT_BOOL('p', "stdout", &to_stdout, N_("send results to standard output")),
		OPT_BOOL(0,   "object-id", &object_id, N_("use object IDs instead of filenames")),
		OPT_SET_INT(0, "diff3", &xmp.style, N_("use a diff3 based merge"), XDL_MERGE_DIFF3),
		OPT_SET_INT(0, "zdiff3", &xmp.style, N_("use a zealous diff3 based merge"),
				XDL_MERGE_ZEALOUS_DIFF3),
		OPT_SET_INT(0, "ours", &xmp.favor, N_("for conflicts, use our version"),
			    XDL_MERGE_FAVOR_OURS),
		OPT_SET_INT(0, "theirs", &xmp.favor, N_("for conflicts, use their version"),
			    XDL_MERGE_FAVOR_THEIRS),
		OPT_SET_INT(0, "union", &xmp.favor, N_("for conflicts, use a union version"),
			    XDL_MERGE_FAVOR_UNION),
		OPT_CALLBACK_F(0, "diff-algorithm", &xmp.xpp, N_("<algorithm>"),
			     N_("choose a diff algorithm"),
			     PARSE_OPT_NONEG, diff_algorithm_cb),
		OPT_INTEGER(0, "marker-size", &xmp.marker_size,
			    N_("for conflicts, use this marker size")),
		OPT__QUIET(&quiet, N_("do not warn about conflicts")),
		OPT_CALLBACK('L', پوچ, names, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_END(),
	};

	xmp.level = XDL_MERGE_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, پوچ);
	اگر (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, prefix, options, merge_file_usage, 0);
	اگر (argc != 3)
		usage_with_options(merge_file_usage, options);
	اگر (quiet) {
		اگر (!freopen("/dev/null", "w", stderr))
			بازگشت error_errno("failed to redirect stderr to /dev/null");
	}

	اگر (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		setup_git_directory(the_repository);

	برای (i = 0; i < 3; i++) {
		نویسه *fname;
		ساختار object_id oid;
		mmfile_t *mmf = mmfs + i;

		اگر (!names[i])
			names[i] = argv[i];

		fname = prefix_filename(prefix, argv[i]);

		اگر (object_id) {
			اگر (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			وگرنه اگر (!oideq(&oid, the_hash_algo->empty_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			وگرنه
				read_mmfile(mmf, "/dev/null");
		} وگرنه اگر (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		اگر (ret != -1 && (mmf->size > MAX_XDIFF_SIZE ||
		    buffer_is_دودویی(mmf->ptr, mmf->size))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		آزاد(fname);
		اگر (ret)
			برو cleanup;

	}

	xmp.ancestor = names[1];
	xmp.file1 = names[0];
	xmp.file2 = names[2];
	ret = xdl_merge(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &result);

	اگر (ret >= 0) {
		اگر (object_id && !to_stdout) {
			ساختار object_id oid;
			اگر (result.size) {
				اگر (odb_write_object(the_repository->objects, result.ptr,
						     result.size, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} وگرنه {
				oidcpy(&oid, the_hash_algo->empty_blob);
			}
			اگر (ret >= 0)
				printf("%s\n", oid_to_hex(&oid));
		} وگرنه {
			ثابت نویسه *filename = argv[0];
			نویسه *fpath = prefix_filename(prefix, argv[0]);
			FILE *f = to_stdout ? stdout : fopen(fpath, "wb");

			اگر (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			وگرنه اگر (result.size &&
				 fwrite(result.ptr, result.size, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			وگرنه اگر (fclose(f))
				ret = error_errno("Could not close %s", filename);
			آزاد(fpath);
		}
		آزاد(result.ptr);
	}

	اگر (ret > 127)
		ret = 127;

cleanup:
	برای (i = 0; i < 3; i++)
		آزاد(mmfs[i].ptr);

	بازگشت ret;
}
