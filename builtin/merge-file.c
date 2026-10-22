#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "xdiff/xdiff.h"
#அந்தர்பாவ "xdiff-interface.h"
#அந்தர்பாவ "parse-options.h"

நிலையான மாறிலி எழுத்து *மாறிலி merge_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	வெற்று
};

நிலையான முழு label_cb(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	நிலையான முழு label_மொத்தம் = 0;
	மாறிலி எழுத்து **names = (மாறிலி எழுத்து **)opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (label_மொத்தம் >= 3)
		திரும்பு error("too many labels on the command line");
	names[label_மொத்தம்++] = arg;
	திரும்பு 0;
}

நிலையான முழு set_diff_algorithm(xpparam_t *xpp,
			      மாறிலி எழுத்து *alg)
{
	long diff_algorithm = parse_algorithm_மதிப்பு(alg);
	எனில் (diff_algorithm < 0)
		திரும்பு -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_MASK) | diff_algorithm;
	திரும்பு 0;
}

நிலையான முழு diff_algorithm_cb(மாறிலி கட்டமைப்பு option *opt,
				மாறிலி எழுத்து *arg, முழு unset)
{
	xpparam_t *xpp = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (set_diff_algorithm(xpp, arg))
		திரும்பு error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	திரும்பு 0;
}

முழு cmd_merge_file(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo)
{
	மாறிலி எழுத்து *names[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t result = { 0 };
	xmparam_t xmp = { 0 };
	முழு ret = 0, i = 0, to_stdout = 0, object_id = 0;
	முழு quiet = 0;
	கட்டமைப்பு option options[] = {
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
		OPT_CALLBACK('L', வெற்று, names, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_END(),
	};

	xmp.level = XDL_MERGE_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, வெற்று);
	எனில் (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, prefix, options, merge_file_usage, 0);
	எனில் (argc != 3)
		usage_with_options(merge_file_usage, options);
	எனில் (quiet) {
		எனில் (!freopen("/dev/null", "w", stderr))
			திரும்பு error_errno("failed to redirect stderr to /dev/null");
	}

	எனில் (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		setup_git_directory(the_repository);

	க்கு (i = 0; i < 3; i++) {
		எழுத்து *fname;
		கட்டமைப்பு object_id oid;
		mmfile_t *mmf = mmfs + i;

		எனில் (!names[i])
			names[i] = argv[i];

		fname = prefix_filename(prefix, argv[i]);

		எனில் (object_id) {
			எனில் (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			இல்லையெனில் எனில் (!oideq(&oid, the_hash_algo->empty_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			இல்லையெனில்
				read_mmfile(mmf, "/dev/null");
		} இல்லையெனில் எனில் (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		எனில் (ret != -1 && (mmf->size > MAX_XDIFF_SIZE ||
		    buffer_is_இருமம்(mmf->ptr, mmf->size))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		விடுவி(fname);
		எனில் (ret)
			செல் cleanup;

	}

	xmp.ancestor = names[1];
	xmp.file1 = names[0];
	xmp.file2 = names[2];
	ret = xdl_merge(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &result);

	எனில் (ret >= 0) {
		எனில் (object_id && !to_stdout) {
			கட்டமைப்பு object_id oid;
			எனில் (result.size) {
				எனில் (odb_write_object(the_repository->objects, result.ptr,
						     result.size, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} இல்லையெனில் {
				oidcpy(&oid, the_hash_algo->empty_blob);
			}
			எனில் (ret >= 0)
				printf("%s\n", oid_to_hex(&oid));
		} இல்லையெனில் {
			மாறிலி எழுத்து *filename = argv[0];
			எழுத்து *fpath = prefix_filename(prefix, argv[0]);
			FILE *f = to_stdout ? stdout : fopen(fpath, "wb");

			எனில் (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			இல்லையெனில் எனில் (result.size &&
				 fwrite(result.ptr, result.size, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			இல்லையெனில் எனில் (fclose(f))
				ret = error_errno("Could not close %s", filename);
			விடுவி(fpath);
		}
		விடுவி(result.ptr);
	}

	எனில் (ret > 127)
		ret = 127;

cleanup:
	க்கு (i = 0; i < 3; i++)
		விடுவி(mmfs[i].ptr);

	திரும்பு ret;
}
