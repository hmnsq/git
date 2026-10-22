#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "xdiff/xdiff.h"
#అన్తర్భావ "xdiff-interface.h"
#అన్తర్భావ "parse-options.h"

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి విలీనం_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	ఖాళీ
};

స్థిర పూర్ణాంకం label_cb(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	స్థిర పూర్ణాంకం label_లెక్క = 0;
	స్థిరరాశి అక్షరం **పేర్లు = (స్థిరరాశి అక్షరం **)opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (label_లెక్క >= 3)
		తిరిగి error("too many labels on the command line");
	పేర్లు[label_లెక్క++] = arg;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం సమితి_diff_algorithm(xpparam_t *xpp,
			      స్థిరరాశి అక్షరం *alg)
{
	దీర్ఘ diff_algorithm = parse_algorithm_విలువ(alg);
	ఒకవేళ (diff_algorithm < 0)
		తిరిగి -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_ముసుగు) | diff_algorithm;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం diff_algorithm_cb(స్థిరరాశి నిర్మాణం option *opt,
				స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	xpparam_t *xpp = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (సమితి_diff_algorithm(xpp, arg))
		తిరిగి error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	తిరిగి 0;
}

పూర్ణాంకం cmd_విలీనం_file(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo)
{
	స్థిరరాశి అక్షరం *పేర్లు[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t ఫలితం = { 0 };
	xmparam_t xmp = { 0 };
	పూర్ణాంకం ret = 0, i = 0, గమ్యం_stdout = 0, object_id = 0;
	పూర్ణాంకం quiet = 0;
	నిర్మాణం option options[] = {
		OPT_BOOL('p', "stdout", &గమ్యం_stdout, N_("send results to standard output")),
		OPT_BOOL(0,   "object-id", &object_id, N_("use object IDs instead of filenames")),
		OPT_సమితి_INT(0, "diff3", &xmp.style, N_("use a diff3 based merge"), XDL_విలీనం_DIFF3),
		OPT_సమితి_INT(0, "zdiff3", &xmp.style, N_("use a zealous diff3 based merge"),
				XDL_విలీనం_ZEALOUS_DIFF3),
		OPT_సమితి_INT(0, "ours", &xmp.favor, N_("for conflicts, use our version"),
			    XDL_విలీనం_FAVOR_OURS),
		OPT_సమితి_INT(0, "theirs", &xmp.favor, N_("for conflicts, use their version"),
			    XDL_విలీనం_FAVOR_THEIRS),
		OPT_సమితి_INT(0, "union", &xmp.favor, N_("for conflicts, use a union version"),
			    XDL_విలీనం_FAVOR_UNION),
		OPT_CALLBACK_F(0, "diff-algorithm", &xmp.xpp, N_("<algorithm>"),
			     N_("choose a diff algorithm"),
			     PARSE_OPT_NONEG, diff_algorithm_cb),
		OPT_INTEGER(0, "marker-size", &xmp.marker_కొలత,
			    N_("for conflicts, use this marker size")),
		OPT__QUIET(&quiet, N_("do not warn about conflicts")),
		OPT_CALLBACK('L', ఖాళీ, పేర్లు, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_చివర(),
	};

	xmp.level = XDL_విలీనం_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, ఖాళీ);
	ఒకవేళ (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, ఉపసర్గ, options, విలీనం_file_usage, 0);
	ఒకవేళ (argc != 3)
		usage_with_options(విలీనం_file_usage, options);
	ఒకవేళ (quiet) {
		ఒకవేళ (!freopen("/dev/null", "w", stderr))
			తిరిగి error_errno("failed to redirect stderr to /dev/null");
	}

	ఒకవేళ (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		అమరిక_git_directory(the_repository);

	కొరకు (i = 0; i < 3; i++) {
		అక్షరం *fname;
		నిర్మాణం object_id oid;
		mmfile_t *mmf = mmfs + i;

		ఒకవేళ (!పేర్లు[i])
			పేర్లు[i] = argv[i];

		fname = ఉపసర్గ_filename(ఉపసర్గ, argv[i]);

		ఒకవేళ (object_id) {
			ఒకవేళ (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			లేకపోతే ఒకవేళ (!oideq(&oid, the_hash_algo->ఖాళీలు_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			లేకపోతే
				read_mmfile(mmf, "/dev/null");
		} లేకపోతే ఒకవేళ (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		ఒకవేళ (ret != -1 && (mmf->కొలత > MAX_XDIFF_కొలత ||
		    బఫరు_is_ద్విమాన(mmf->ptr, mmf->కొలత))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		విడుదల(fname);
		ఒకవేళ (ret)
			వెళ్ళు cleanup;

	}

	xmp.ancestor = పేర్లు[1];
	xmp.file1 = పేర్లు[0];
	xmp.file2 = పేర్లు[2];
	ret = xdl_విలీనం(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &ఫలితం);

	ఒకవేళ (ret >= 0) {
		ఒకవేళ (object_id && !గమ్యం_stdout) {
			నిర్మాణం object_id oid;
			ఒకవేళ (ఫలితం.కొలత) {
				ఒకవేళ (odb_write_object(the_repository->objects, ఫలితం.ptr,
						     ఫలితం.కొలత, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} లేకపోతే {
				oidcpy(&oid, the_hash_algo->ఖాళీలు_blob);
			}
			ఒకవేళ (ret >= 0)
				రాయి("%s\n", oid_గమ్యం_hex(&oid));
		} లేకపోతే {
			స్థిరరాశి అక్షరం *filename = argv[0];
			అక్షరం *fpath = ఉపసర్గ_filename(ఉపసర్గ, argv[0]);
			FILE *f = గమ్యం_stdout ? stdout : fopen(fpath, "wb");

			ఒకవేళ (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			లేకపోతే ఒకవేళ (ఫలితం.కొలత &&
				 fwrite(ఫలితం.ptr, ఫలితం.కొలత, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			లేకపోతే ఒకవేళ (fclose(f))
				ret = error_errno("Could not close %s", filename);
			విడుదల(fpath);
		}
		విడుదల(ఫలితం.ptr);
	}

	ఒకవేళ (ret > 127)
		ret = 127;

cleanup:
	కొరకు (i = 0; i < 3; i++)
		విడుదల(mmfs[i].ptr);

	తిరిగి ret;
}
