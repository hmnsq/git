#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "xdiff/xdiff.h"
#અન્તર્ભાવ "xdiff-interface.h"
#અન્તર્ભાવ "parse-options.h"

સ્થિર અચલ અક્ષર *અચલ merge_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	ખાલી
};

સ્થિર પૂર્ણાંક label_cb(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સ્થિર પૂર્ણાંક label_ગણતરી = 0;
	અચલ અક્ષર **names = (અચલ અક્ષર **)opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (label_ગણતરી >= 3)
		પાછા error("too many labels on the command line");
	names[label_ગણતરી++] = arg;
	પાછા 0;
}

સ્થિર પૂર્ણાંક set_diff_algorithm(xpparam_t *xpp,
			      અચલ અક્ષર *alg)
{
	long diff_algorithm = parse_algorithm_કિંમત(alg);
	જો (diff_algorithm < 0)
		પાછા -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_MASK) | diff_algorithm;
	પાછા 0;
}

સ્થિર પૂર્ણાંક diff_algorithm_cb(અચલ સંરચના option *opt,
				અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	xpparam_t *xpp = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (set_diff_algorithm(xpp, arg))
		પાછા error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	પાછા 0;
}

પૂર્ણાંક cmd_merge_file(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix,
		   સંરચના repository *repo)
{
	અચલ અક્ષર *names[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t result = { 0 };
	xmparam_t xmp = { 0 };
	પૂર્ણાંક ret = 0, i = 0, to_stdout = 0, object_id = 0;
	પૂર્ણાંક quiet = 0;
	સંરચના option options[] = {
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
		OPT_CALLBACK('L', ખાલી, names, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_END(),
	};

	xmp.level = XDL_MERGE_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, ખાલી);
	જો (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, prefix, options, merge_file_usage, 0);
	જો (argc != 3)
		usage_with_options(merge_file_usage, options);
	જો (quiet) {
		જો (!freopen("/dev/null", "w", stderr))
			પાછા error_errno("failed to redirect stderr to /dev/null");
	}

	જો (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		setup_git_directory(the_repository);

	માટે (i = 0; i < 3; i++) {
		અક્ષર *fname;
		સંરચના object_id oid;
		mmfile_t *mmf = mmfs + i;

		જો (!names[i])
			names[i] = argv[i];

		fname = prefix_filename(prefix, argv[i]);

		જો (object_id) {
			જો (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			નહીંતર જો (!oideq(&oid, the_hash_algo->empty_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			નહીંતર
				read_mmfile(mmf, "/dev/null");
		} નહીંતર જો (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		જો (ret != -1 && (mmf->size > MAX_XDIFF_SIZE ||
		    buffer_is_દ્વિઅંકી(mmf->ptr, mmf->size))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		મુક્ત(fname);
		જો (ret)
			જાઓ cleanup;

	}

	xmp.ancestor = names[1];
	xmp.file1 = names[0];
	xmp.file2 = names[2];
	ret = xdl_merge(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &result);

	જો (ret >= 0) {
		જો (object_id && !to_stdout) {
			સંરચના object_id oid;
			જો (result.size) {
				જો (odb_write_object(the_repository->objects, result.ptr,
						     result.size, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} નહીંતર {
				oidcpy(&oid, the_hash_algo->empty_blob);
			}
			જો (ret >= 0)
				printf("%s\n", oid_to_hex(&oid));
		} નહીંતર {
			અચલ અક્ષર *filename = argv[0];
			અક્ષર *fpath = prefix_filename(prefix, argv[0]);
			FILE *f = to_stdout ? stdout : fopen(fpath, "wb");

			જો (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			નહીંતર જો (result.size &&
				 fwrite(result.ptr, result.size, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			નહીંતર જો (fclose(f))
				ret = error_errno("Could not close %s", filename);
			મુક્ત(fpath);
		}
		મુક્ત(result.ptr);
	}

	જો (ret > 127)
		ret = 127;

cleanup:
	માટે (i = 0; i < 3; i++)
		મુક્ત(mmfs[i].ptr);

	પાછા ret;
}
