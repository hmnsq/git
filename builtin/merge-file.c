#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "diff.h"
#समावेश "hex.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "config.h"
#समावेश "gettext.h"
#समावेश "setup.h"
#समावेश "xdiff/xdiff.h"
#समावेश "xdiff-interface.h"
#समावेश "parse-options.h"

अटल स्थिर अक्षर *स्थिर merge_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	शून्य
};

अटल पूर्णांक label_cb(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	अटल पूर्णांक label_गिनती = 0;
	स्थिर अक्षर **names = (स्थिर अक्षर **)opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (label_गिनती >= 3)
		वापस error("too many labels on the command line");
	names[label_गिनती++] = arg;
	वापस 0;
}

अटल पूर्णांक set_diff_algorithm(xpparam_t *xpp,
			      स्थिर अक्षर *alg)
{
	दीर्घ diff_algorithm = parse_algorithm_मूल्य(alg);
	अगर (diff_algorithm < 0)
		वापस -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_MASK) | diff_algorithm;
	वापस 0;
}

अटल पूर्णांक diff_algorithm_cb(स्थिर काष्ठा option *opt,
				स्थिर अक्षर *arg, पूर्णांक unset)
{
	xpparam_t *xpp = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (set_diff_algorithm(xpp, arg))
		वापस error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	वापस 0;
}

पूर्णांक cmd_merge_file(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *prefix,
		   काष्ठा repository *repo)
{
	स्थिर अक्षर *names[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t परिणाम = { 0 };
	xmparam_t xmp = { 0 };
	पूर्णांक ret = 0, i = 0, to_stdout = 0, object_id = 0;
	पूर्णांक quiet = 0;
	काष्ठा option options[] = {
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
		OPT_INTEGER(0, "marker-size", &xmp.marker_आकार,
			    N_("for conflicts, use this marker size")),
		OPT__QUIET(&quiet, N_("do not warn about conflicts")),
		OPT_CALLBACK('L', शून्य, names, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_END(),
	};

	xmp.level = XDL_MERGE_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, शून्य);
	अगर (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, prefix, options, merge_file_usage, 0);
	अगर (argc != 3)
		usage_with_options(merge_file_usage, options);
	अगर (quiet) {
		अगर (!ख_व_खोलो("/dev/null", "w", मानक_त्रुटि))
			वापस error_errno("failed to redirect stderr to /dev/null");
	}

	अगर (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		setup_git_directory(the_repository);

	क्रम (i = 0; i < 3; i++) {
		अक्षर *fname;
		काष्ठा object_id oid;
		mmfile_t *mmf = mmfs + i;

		अगर (!names[i])
			names[i] = argv[i];

		fname = prefix_filename(prefix, argv[i]);

		अगर (object_id) {
			अगर (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			अन्यथा अगर (!oideq(&oid, the_hash_algo->empty_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			अन्यथा
				read_mmfile(mmf, "/dev/null");
		} अन्यथा अगर (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		अगर (ret != -1 && (mmf->आकार > MAX_XDIFF_आकार ||
		    बफर_is_द्विआधारी(mmf->ptr, mmf->आकार))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		मुक्त(fname);
		अगर (ret)
			जाओ cleanup;

	}

	xmp.ancestor = names[1];
	xmp.file1 = names[0];
	xmp.file2 = names[2];
	ret = xdl_merge(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &परिणाम);

	अगर (ret >= 0) {
		अगर (object_id && !to_stdout) {
			काष्ठा object_id oid;
			अगर (परिणाम.आकार) {
				अगर (odb_write_object(the_repository->objects, परिणाम.ptr,
						     परिणाम.आकार, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} अन्यथा {
				oidcpy(&oid, the_hash_algo->empty_blob);
			}
			अगर (ret >= 0)
				म_लिखो("%s\n", oid_to_hex(&oid));
		} अन्यथा {
			स्थिर अक्षर *filename = argv[0];
			अक्षर *fpath = prefix_filename(prefix, argv[0]);
			खाता *f = to_stdout ? मानक_निकास : ख_खोलो(fpath, "wb");

			अगर (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			अन्यथा अगर (परिणाम.आकार &&
				 ख_डालो(परिणाम.ptr, परिणाम.आकार, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			अन्यथा अगर (ख_बंद(f))
				ret = error_errno("Could not close %s", filename);
			मुक्त(fpath);
		}
		मुक्त(परिणाम.ptr);
	}

	अगर (ret > 127)
		ret = 127;

cleanup:
	क्रम (i = 0; i < 3; i++)
		मुक्त(mmfs[i].ptr);

	वापस ret;
}
