#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdiff/xdiff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdiff-interface.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ ਮਿਲਾਓ_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ label_cb(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਸਥਿਰ ਪੂਰਨ label_ਗਿਣਤੀ = 0;
	ਅਚਲ ਅੱਖਰ **ਨਾਂ = (ਅਚਲ ਅੱਖਰ **)opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (label_ਗਿਣਤੀ >= 3)
		ਵਾਪਸ error("too many labels on the command line");
	ਨਾਂ[label_ਗਿਣਤੀ++] = arg;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਸੈੱਟ_diff_algorithm(xpparam_t *xpp,
			      ਅਚਲ ਅੱਖਰ *alg)
{
	ਲੰਮਾ diff_algorithm = parse_algorithm_ਮੁੱਲ(alg);
	ਜੇ (diff_algorithm < 0)
		ਵਾਪਸ -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_ਨਕਾਬ) | diff_algorithm;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ diff_algorithm_cb(ਅਚਲ ਬਣਤਰ option *opt,
				ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	xpparam_t *xpp = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (ਸੈੱਟ_diff_algorithm(xpp, arg))
		ਵਾਪਸ error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_ਮਿਲਾਓ_file(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਅੱਖਰ *ਨਾਂ[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t ਨਤੀਜਾ = { 0 };
	xmparam_t xmp = { 0 };
	ਪੂਰਨ ret = 0, i = 0, ਮੰਜ਼ਿਲ_stdout = 0, object_id = 0;
	ਪੂਰਨ quiet = 0;
	ਬਣਤਰ option options[] = {
		OPT_BOOL('p', "stdout", &ਮੰਜ਼ਿਲ_stdout, N_("send results to standard output")),
		OPT_BOOL(0,   "object-id", &object_id, N_("use object IDs instead of filenames")),
		OPT_ਸੈੱਟ_INT(0, "diff3", &xmp.style, N_("use a diff3 based merge"), XDL_ਮਿਲਾਓ_DIFF3),
		OPT_ਸੈੱਟ_INT(0, "zdiff3", &xmp.style, N_("use a zealous diff3 based merge"),
				XDL_ਮਿਲਾਓ_ZEALOUS_DIFF3),
		OPT_ਸੈੱਟ_INT(0, "ours", &xmp.favor, N_("for conflicts, use our version"),
			    XDL_ਮਿਲਾਓ_FAVOR_OURS),
		OPT_ਸੈੱਟ_INT(0, "theirs", &xmp.favor, N_("for conflicts, use their version"),
			    XDL_ਮਿਲਾਓ_FAVOR_THEIRS),
		OPT_ਸੈੱਟ_INT(0, "union", &xmp.favor, N_("for conflicts, use a union version"),
			    XDL_ਮਿਲਾਓ_FAVOR_UNION),
		OPT_CALLBACK_F(0, "diff-algorithm", &xmp.xpp, N_("<algorithm>"),
			     N_("choose a diff algorithm"),
			     PARSE_OPT_NONEG, diff_algorithm_cb),
		OPT_INTEGER(0, "marker-size", &xmp.marker_ਮਾਪ,
			    N_("for conflicts, use this marker size")),
		OPT__QUIET(&quiet, N_("do not warn about conflicts")),
		OPT_CALLBACK('L', ਨਿਰਾ, ਨਾਂ, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_ਸਿਰਾ(),
	};

	xmp.level = XDL_ਮਿਲਾਓ_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, ਨਿਰਾ);
	ਜੇ (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, ਮਿਲਾਓ_file_usage, 0);
	ਜੇ (argc != 3)
		usage_with_options(ਮਿਲਾਓ_file_usage, options);
	ਜੇ (quiet) {
		ਜੇ (!freopen("/dev/null", "w", stderr))
			ਵਾਪਸ error_errno("failed to redirect stderr to /dev/null");
	}

	ਜੇ (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		ਤਿਆਰੀ_git_directory(the_repository);

	ਲਈ (i = 0; i < 3; i++) {
		ਅੱਖਰ *fname;
		ਬਣਤਰ object_id oid;
		mmfile_t *mmf = mmfs + i;

		ਜੇ (!ਨਾਂ[i])
			ਨਾਂ[i] = argv[i];

		fname = ਅਗੇਤਰ_filename(ਅਗੇਤਰ, argv[i]);

		ਜੇ (object_id) {
			ਜੇ (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			ਨਹੀਂਤਾਂ ਜੇ (!oideq(&oid, the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			ਨਹੀਂਤਾਂ
				read_mmfile(mmf, "/dev/null");
		} ਨਹੀਂਤਾਂ ਜੇ (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		ਜੇ (ret != -1 && (mmf->ਮਾਪ > MAX_XDIFF_ਮਾਪ ||
		    ਬਫਰ_is_ਦੁਆਧਾਰੀ(mmf->ptr, mmf->ਮਾਪ))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		ਮੁਕਤ(fname);
		ਜੇ (ret)
			ਜਾਓ cleanup;

	}

	xmp.ancestor = ਨਾਂ[1];
	xmp.file1 = ਨਾਂ[0];
	xmp.file2 = ਨਾਂ[2];
	ret = xdl_ਮਿਲਾਓ(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &ਨਤੀਜਾ);

	ਜੇ (ret >= 0) {
		ਜੇ (object_id && !ਮੰਜ਼ਿਲ_stdout) {
			ਬਣਤਰ object_id oid;
			ਜੇ (ਨਤੀਜਾ.ਮਾਪ) {
				ਜੇ (odb_write_object(the_repository->objects, ਨਤੀਜਾ.ptr,
						     ਨਤੀਜਾ.ਮਾਪ, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} ਨਹੀਂਤਾਂ {
				oidcpy(&oid, the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_blob);
			}
			ਜੇ (ret >= 0)
				ਲਿਖੋ("%s\n", oid_ਮੰਜ਼ਿਲ_hex(&oid));
		} ਨਹੀਂਤਾਂ {
			ਅਚਲ ਅੱਖਰ *filename = argv[0];
			ਅੱਖਰ *fpath = ਅਗੇਤਰ_filename(ਅਗੇਤਰ, argv[0]);
			FILE *f = ਮੰਜ਼ਿਲ_stdout ? stdout : fopen(fpath, "wb");

			ਜੇ (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			ਨਹੀਂਤਾਂ ਜੇ (ਨਤੀਜਾ.ਮਾਪ &&
				 fwrite(ਨਤੀਜਾ.ptr, ਨਤੀਜਾ.ਮਾਪ, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			ਨਹੀਂਤਾਂ ਜੇ (fclose(f))
				ret = error_errno("Could not close %s", filename);
			ਮੁਕਤ(fpath);
		}
		ਮੁਕਤ(ਨਤੀਜਾ.ptr);
	}

	ਜੇ (ret > 127)
		ret = 127;

cleanup:
	ਲਈ (i = 0; i < 3; i++)
		ਮੁਕਤ(mmfs[i].ptr);

	ਵਾਪਸ ret;
}
