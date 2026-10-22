#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "xdiff/xdiff.h"
#অন্তর্ভাব "xdiff-interface.h"
#অন্তর্ভাব "parse-options.h"

স্থির ধ্রুবক অক্ষর *ধ্রুবক merge_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	ফাঁকা
};

স্থির পূর্ণসংখ্যা label_cb(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	স্থির পূর্ণসংখ্যা label_গুনতি = 0;
	ধ্রুবক অক্ষর **names = (ধ্রুবক অক্ষর **)opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (label_গুনতি >= 3)
		ফেরত error("too many labels on the command line");
	names[label_গুনতি++] = arg;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা set_diff_algorithm(xpparam_t *xpp,
			      ধ্রুবক অক্ষর *alg)
{
	long diff_algorithm = parse_algorithm_মান(alg);
	যদি (diff_algorithm < 0)
		ফেরত -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_MASK) | diff_algorithm;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা diff_algorithm_cb(ধ্রুবক কাঠামো option *opt,
				ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	xpparam_t *xpp = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (set_diff_algorithm(xpp, arg))
		ফেরত error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	ফেরত 0;
}

পূর্ণসংখ্যা cmd_merge_file(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *prefix,
		   কাঠামো repository *repo)
{
	ধ্রুবক অক্ষর *names[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t ফলাফল = { 0 };
	xmparam_t xmp = { 0 };
	পূর্ণসংখ্যা ret = 0, i = 0, to_stdout = 0, object_id = 0;
	পূর্ণসংখ্যা quiet = 0;
	কাঠামো option options[] = {
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
		OPT_INTEGER(0, "marker-size", &xmp.marker_আকার,
			    N_("for conflicts, use this marker size")),
		OPT__QUIET(&quiet, N_("do not warn about conflicts")),
		OPT_CALLBACK('L', ফাঁকা, names, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_END(),
	};

	xmp.level = XDL_MERGE_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, ফাঁকা);
	যদি (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, prefix, options, merge_file_usage, 0);
	যদি (argc != 3)
		usage_with_options(merge_file_usage, options);
	যদি (quiet) {
		যদি (!freopen("/dev/null", "w", stderr))
			ফেরত error_errno("failed to redirect stderr to /dev/null");
	}

	যদি (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		setup_git_directory(the_repository);

	জন্য (i = 0; i < 3; i++) {
		অক্ষর *fname;
		কাঠামো object_id oid;
		mmfile_t *mmf = mmfs + i;

		যদি (!names[i])
			names[i] = argv[i];

		fname = prefix_filename(prefix, argv[i]);

		যদি (object_id) {
			যদি (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			অন্যথা যদি (!oideq(&oid, the_hash_algo->empty_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			অন্যথা
				read_mmfile(mmf, "/dev/null");
		} অন্যথা যদি (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		যদি (ret != -1 && (mmf->আকার_2 > MAX_XDIFF_আকার ||
		    বাফার_is_দ্বিমিক(mmf->ptr, mmf->আকার_2))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		মুক্ত(fname);
		যদি (ret)
			যাও cleanup;

	}

	xmp.ancestor = names[1];
	xmp.file1 = names[0];
	xmp.file2 = names[2];
	ret = xdl_merge(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &ফলাফল);

	যদি (ret >= 0) {
		যদি (object_id && !to_stdout) {
			কাঠামো object_id oid;
			যদি (ফলাফল.আকার_2) {
				যদি (odb_write_object(the_repository->objects, ফলাফল.ptr,
						     ফলাফল.আকার_2, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} অন্যথা {
				oidcpy(&oid, the_hash_algo->empty_blob);
			}
			যদি (ret >= 0)
				printf("%s\n", oid_to_hex(&oid));
		} অন্যথা {
			ধ্রুবক অক্ষর *filename = argv[0];
			অক্ষর *fpath = prefix_filename(prefix, argv[0]);
			FILE *f = to_stdout ? stdout : fopen(fpath, "wb");

			যদি (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			অন্যথা যদি (ফলাফল.আকার_2 &&
				 fwrite(ফলাফল.ptr, ফলাফল.আকার_2, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			অন্যথা যদি (fclose(f))
				ret = error_errno("Could not close %s", filename);
			মুক্ত(fpath);
		}
		মুক্ত(ফলাফল.ptr);
	}

	যদি (ret > 127)
		ret = 127;

cleanup:
	জন্য (i = 0; i < 3; i++)
		মুক্ত(mmfs[i].ptr);

	ফেরত ret;
}
