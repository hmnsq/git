#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "xdiff/xdiff.h"
#ಅನ್ತರ್ಭಾವ "xdiff-interface.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ ವಿಲೀನ_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ label_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ label_ಎಣಿಕೆ = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಹೆಸರುಗಳು = (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **)opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (label_ಎಣಿಕೆ >= 3)
		ಹಿಂತಿರುಗಿ error("too many labels on the command line");
	ಹೆಸರುಗಳು[label_ಎಣಿಕೆ++] = arg;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಣ_diff_algorithm(xpparam_t *xpp,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *alg)
{
	ದೀರ್ಘ diff_algorithm = parse_algorithm_ಮೌಲ್ಯ(alg);
	ಆದರೆ (diff_algorithm < 0)
		ಹಿಂತಿರುಗಿ -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_ಮುಖವಾಡ) | diff_algorithm;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ diff_algorithm_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	xpparam_t *xpp = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (ಗಣ_diff_algorithm(xpp, arg))
		ಹಿಂತಿರುಗಿ error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_ವಿಲೀನ_file(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಹೆಸರುಗಳು[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t ಫಲಿತಾಂಶ = { 0 };
	xmparam_t xmp = { 0 };
	ಪೂರ್ಣಾಂಕ ret = 0, i = 0, ಗಮ್ಯ_stdout = 0, object_id = 0;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ರಚನೆ option options[] = {
		OPT_BOOL('p', "stdout", &ಗಮ್ಯ_stdout, N_("send results to standard output")),
		OPT_BOOL(0,   "object-id", &object_id, N_("use object IDs instead of filenames")),
		OPT_ಗಣ_INT(0, "diff3", &xmp.style, N_("use a diff3 based merge"), XDL_ವಿಲೀನ_DIFF3),
		OPT_ಗಣ_INT(0, "zdiff3", &xmp.style, N_("use a zealous diff3 based merge"),
				XDL_ವಿಲೀನ_ZEALOUS_DIFF3),
		OPT_ಗಣ_INT(0, "ours", &xmp.favor, N_("for conflicts, use our version"),
			    XDL_ವಿಲೀನ_FAVOR_OURS),
		OPT_ಗಣ_INT(0, "theirs", &xmp.favor, N_("for conflicts, use their version"),
			    XDL_ವಿಲೀನ_FAVOR_THEIRS),
		OPT_ಗಣ_INT(0, "union", &xmp.favor, N_("for conflicts, use a union version"),
			    XDL_ವಿಲೀನ_FAVOR_UNION),
		OPT_CALLBACK_F(0, "diff-algorithm", &xmp.xpp, N_("<algorithm>"),
			     N_("choose a diff algorithm"),
			     PARSE_OPT_NONEG, diff_algorithm_cb),
		OPT_INTEGER(0, "marker-size", &xmp.marker_ಗಾತ್ರ,
			    N_("for conflicts, use this marker size")),
		OPT__QUIET(&quiet, N_("do not warn about conflicts")),
		OPT_CALLBACK('L', ಖಾಲಿ, ಹೆಸರುಗಳು, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_ಕೊನೆ(),
	};

	xmp.level = XDL_ವಿಲೀನ_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, ಖಾಲಿ);
	ಆದರೆ (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, ವಿಲೀನ_file_usage, 0);
	ಆದರೆ (argc != 3)
		usage_with_options(ವಿಲೀನ_file_usage, options);
	ಆದರೆ (quiet) {
		ಆದರೆ (!freopen("/dev/null", "w", stderr))
			ಹಿಂತಿರುಗಿ error_errno("failed to redirect stderr to /dev/null");
	}

	ಆದರೆ (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		ಸಿದ್ಧತೆ_git_directory(the_repository);

	ಗಾಗಿ (i = 0; i < 3; i++) {
		ಅಕ್ಷರ *fname;
		ರಚನೆ object_id oid;
		mmfile_t *mmf = mmfs + i;

		ಆದರೆ (!ಹೆಸರುಗಳು[i])
			ಹೆಸರುಗಳು[i] = argv[i];

		fname = ಪೂರ್ವಪ್ರತ್ಯಯ_filename(ಪೂರ್ವಪ್ರತ್ಯಯ, argv[i]);

		ಆದರೆ (object_id) {
			ಆದರೆ (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!oideq(&oid, the_hash_algo->ಖಾಲಿಗಳು_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			ಇಲ್ಲದಿದ್ದರೆ
				read_mmfile(mmf, "/dev/null");
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		ಆದರೆ (ret != -1 && (mmf->ಗಾತ್ರ_2 > MAX_XDIFF_ಗಾತ್ರ ||
		    ಬಫರ್_is_ದ್ವಿಮಾನ(mmf->ptr, mmf->ಗಾತ್ರ_2))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		ಬಿಡುಗಡೆ(fname);
		ಆದರೆ (ret)
			ಹೋಗು cleanup;

	}

	xmp.ancestor = ಹೆಸರುಗಳು[1];
	xmp.file1 = ಹೆಸರುಗಳು[0];
	xmp.file2 = ಹೆಸರುಗಳು[2];
	ret = xdl_ವಿಲೀನ(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &ಫಲಿತಾಂಶ);

	ಆದರೆ (ret >= 0) {
		ಆದರೆ (object_id && !ಗಮ್ಯ_stdout) {
			ರಚನೆ object_id oid;
			ಆದರೆ (ಫಲಿತಾಂಶ.ಗಾತ್ರ_2) {
				ಆದರೆ (odb_write_object(the_repository->objects, ಫಲಿತಾಂಶ.ptr,
						     ಫಲಿತಾಂಶ.ಗಾತ್ರ_2, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} ಇಲ್ಲದಿದ್ದರೆ {
				oidcpy(&oid, the_hash_algo->ಖಾಲಿಗಳು_blob);
			}
			ಆದರೆ (ret >= 0)
				ಬರೆ("%s\n", oid_ಗಮ್ಯ_hex(&oid));
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename = argv[0];
			ಅಕ್ಷರ *fpath = ಪೂರ್ವಪ್ರತ್ಯಯ_filename(ಪೂರ್ವಪ್ರತ್ಯಯ, argv[0]);
			FILE *f = ಗಮ್ಯ_stdout ? stdout : fopen(fpath, "wb");

			ಆದರೆ (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಫಲಿತಾಂಶ.ಗಾತ್ರ_2 &&
				 fwrite(ಫಲಿತಾಂಶ.ptr, ಫಲಿತಾಂಶ.ಗಾತ್ರ_2, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose(f))
				ret = error_errno("Could not close %s", filename);
			ಬಿಡುಗಡೆ(fpath);
		}
		ಬಿಡುಗಡೆ(ಫಲಿತಾಂಶ.ptr);
	}

	ಆದರೆ (ret > 127)
		ret = 127;

cleanup:
	ಗಾಗಿ (i = 0; i < 3; i++)
		ಬಿಡುಗಡೆ(mmfs[i].ptr);

	ಹಿಂತಿರುಗಿ ret;
}
