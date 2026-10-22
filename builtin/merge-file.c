#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "xdiff/xdiff.h"
#अन्तर्भाव "xdiff-interface.h"
#अन्तर्भाव "parse-options.h"

स्थिर ध्रुव वर्ण *ध्रुव merge_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	रिक्त
};

स्थिर पूर्णाङ्क label_cb(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	स्थिर पूर्णाङ्क label_गणनम् = 0;
	ध्रुव वर्ण **नामानि = (ध्रुव वर्ण **)opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	यदि (label_गणनम् >= 3)
		प्रत्यागम error("too many labels on the command line");
	नामानि[label_गणनम्++] = arg;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क समुच्चयः_diff_algorithm(xpparam_t *xpp,
			      ध्रुव वर्ण *alg)
{
	दीर्घ diff_algorithm = parse_algorithm_मानम्(alg);
	यदि (diff_algorithm < 0)
		प्रत्यागम -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_आवरणम्) | diff_algorithm;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क diff_algorithm_cb(ध्रुव संरचना option *opt,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	xpparam_t *xpp = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	यदि (समुच्चयः_diff_algorithm(xpp, arg))
		प्रत्यागम error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_merge_file(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *उपसर्गः,
		   संरचना repository *repo)
{
	ध्रुव वर्ण *नामानि[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t परिणाम = { 0 };
	xmparam_t xmp = { 0 };
	पूर्णाङ्क ret = 0, i = 0, गन्तव्यम्_stdout = 0, object_id = 0;
	पूर्णाङ्क quiet = 0;
	संरचना option options[] = {
		OPT_BOOL('p', "stdout", &गन्तव्यम्_stdout, N_("send results to standard output")),
		OPT_BOOL(0,   "object-id", &object_id, N_("use object IDs instead of filenames")),
		OPT_समुच्चयः_INT(0, "diff3", &xmp.style, N_("use a diff3 based merge"), XDL_MERGE_DIFF3),
		OPT_समुच्चयः_INT(0, "zdiff3", &xmp.style, N_("use a zealous diff3 based merge"),
				XDL_MERGE_ZEALOUS_DIFF3),
		OPT_समुच्चयः_INT(0, "ours", &xmp.favor, N_("for conflicts, use our version"),
			    XDL_MERGE_FAVOR_OURS),
		OPT_समुच्चयः_INT(0, "theirs", &xmp.favor, N_("for conflicts, use their version"),
			    XDL_MERGE_FAVOR_THEIRS),
		OPT_समुच्चयः_INT(0, "union", &xmp.favor, N_("for conflicts, use a union version"),
			    XDL_MERGE_FAVOR_UNION),
		OPT_CALLBACK_F(0, "diff-algorithm", &xmp.xpp, N_("<algorithm>"),
			     N_("choose a diff algorithm"),
			     PARSE_OPT_NONEG, diff_algorithm_cb),
		OPT_INTEGER(0, "marker-size", &xmp.marker_आकार,
			    N_("for conflicts, use this marker size")),
		OPT__QUIET(&quiet, N_("do not warn about conflicts")),
		OPT_CALLBACK('L', रिक्त, नामानि, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_अन्तः(),
	};

	xmp.level = XDL_MERGE_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, रिक्त);
	यदि (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, उपसर्गः, options, merge_file_usage, 0);
	यदि (argc != 3)
		usage_with_options(merge_file_usage, options);
	यदि (quiet) {
		यदि (!freopen("/dev/null", "w", stderr))
			प्रत्यागम error_errno("failed to redirect stderr to /dev/null");
	}

	यदि (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		सज्जता_git_directory(the_repository);

	प्रति (i = 0; i < 3; i++) {
		वर्ण *fname;
		संरचना object_id oid;
		mmfile_t *mmf = mmfs + i;

		यदि (!नामानि[i])
			नामानि[i] = argv[i];

		fname = उपसर्गः_filename(उपसर्गः, argv[i]);

		यदि (object_id) {
			यदि (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			अन्यथा यदि (!oideq(&oid, the_hash_algo->रिक्तानि_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			अन्यथा
				read_mmfile(mmf, "/dev/null");
		} अन्यथा यदि (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		यदि (ret != -1 && (mmf->आकार > MAX_XDIFF_आकार ||
		    बफर_is_द्विमान(mmf->ptr, mmf->आकार))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		मुक्त(fname);
		यदि (ret)
			गच्छ cleanup;

	}

	xmp.ancestor = नामानि[1];
	xmp.file1 = नामानि[0];
	xmp.file2 = नामानि[2];
	ret = xdl_merge(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &परिणाम);

	यदि (ret >= 0) {
		यदि (object_id && !गन्तव्यम्_stdout) {
			संरचना object_id oid;
			यदि (परिणाम.आकार) {
				यदि (odb_write_object(the_repository->objects, परिणाम.ptr,
						     परिणाम.आकार, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} अन्यथा {
				oidcpy(&oid, the_hash_algo->रिक्तानि_blob);
			}
			यदि (ret >= 0)
				प्रारूप_लिख("%s\n", oid_गन्तव्यम्_hex(&oid));
		} अन्यथा {
			ध्रुव वर्ण *filename = argv[0];
			वर्ण *fpath = उपसर्गः_filename(उपसर्गः, argv[0]);
			FILE *f = गन्तव्यम्_stdout ? stdout : fopen(fpath, "wb");

			यदि (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			अन्यथा यदि (परिणाम.आकार &&
				 fwrite(परिणाम.ptr, परिणाम.आकार, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			अन्यथा यदि (fclose(f))
				ret = error_errno("Could not close %s", filename);
			मुक्त(fpath);
		}
		मुक्त(परिणाम.ptr);
	}

	यदि (ret > 127)
		ret = 127;

cleanup:
	प्रति (i = 0; i < 3; i++)
		मुक्त(mmfs[i].ptr);

	प्रत्यागम ret;
}
