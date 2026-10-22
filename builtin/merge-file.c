#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "abspath.h"
#include "diff.h"
#include "hex.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"
#include "config.h"
#include "gettext.h"
#include "setup.h"
#include "xdiff/xdiff.h"
#include "xdiff-interface.h"
#include "parse-options.h"

static const var_na *const merge_file_usage[] = {
	N_("git merge-file [<options>] [-L <name1> [-L <orig> [-L <name2>]]] <file1> <orig-file> <file2>"),
	NULL
};

static puur_naa_nka label_cb(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	static puur_naa_nka label_count = 0;
	const var_na **names = (const var_na **)opt->value;

	BUG_ON_OPT_NEG(unset);

	ᤀᤡ᤺ (label_count >= 3)
		ᤀᤡᤴ error("too many labels on the command line");
	names[label_count++] = arg;
	ᤀᤡᤴ 0;
}

static puur_naa_nka set_diff_algorithm(xpparam_t *xpp,
			      const var_na *alg)
{
	long diff_algorithm = parse_algorithm_value(alg);
	ᤀᤡ᤺ (diff_algorithm < 0)
		ᤀᤡᤴ -1;
	xpp->flags = (xpp->flags & ~XDF_DIFF_ALGORITHM_MASK) | diff_algorithm;
	ᤀᤡᤴ 0;
}

static puur_naa_nka diff_algorithm_cb(const sa_nracanaa option *opt,
				const var_na *arg, puur_naa_nka unset)
{
	xpparam_t *xpp = opt->value;

	BUG_ON_OPT_NEG(unset);

	ᤀᤡ᤺ (set_diff_algorithm(xpp, arg))
		ᤀᤡᤴ error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	ᤀᤡᤴ 0;
}

puur_naa_nka cmd_merge_file(puur_naa_nka argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo)
{
	const var_na *names[3] = { 0 };
	mmfile_t mmfs[3] = { 0 };
	mmfile_t result = { 0 };
	xmparam_t xmp = { 0 };
	puur_naa_nka ret = 0, i = 0, to_stdout = 0, object_id = 0;
	puur_naa_nka quiet = 0;
	sa_nracanaa option options[] = {
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
		OPT_CALLBACK('L', NULL, names, N_("name"),
			     N_("set labels for file1/orig-file/file2"), &label_cb),
		OPT_END(),
	};

	xmp.level = XDL_MERGE_ZEALOUS_ALNUM;
	xmp.style = 0;
	xmp.favor = 0;

	/* Read the configuration file */
	repo_config(repo, git_xmerge_config, NULL);
	ᤀᤡ᤺ (0 <= git_xmerge_style)
		xmp.style = git_xmerge_style;

	argc = parse_options(argc, argv, prefix, options, merge_file_usage, 0);
	ᤀᤡ᤺ (argc != 3)
		usage_with_options(merge_file_usage, options);
	ᤀᤡ᤺ (quiet) {
		ᤀᤡ᤺ (!freopen("/dev/null", "w", stderr))
			ᤀᤡᤴ error_errno("failed to redirect stderr to /dev/null");
	}

	ᤀᤡ᤺ (!repo && object_id)
		/* emit the correct "not a git repo" error in this case */
		setup_git_directory(the_repository);

	for (i = 0; i < 3; i++) {
		var_na *fname;
		sa_nracanaa object_id oid;
		mmfile_t *mmf = mmfs + i;

		ᤀᤡ᤺ (!names[i])
			names[i] = argv[i];

		fname = prefix_filename(prefix, argv[i]);

		ᤀᤡ᤺ (object_id) {
			ᤀᤡ᤺ (repo_get_oid(the_repository, argv[i], &oid))
				ret = error(_("object '%s' does not exist"),
					      argv[i]);
			ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!oideq(&oid, the_hash_algo->empty_blob))
				read_mmblob(mmf, the_repository->objects, &oid);
			ᤀᤡ᤺ᤆᤡ
				read_mmfile(mmf, "/dev/null");
		} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (read_mmfile(mmf, fname)) {
			ret = -1;
		}
		ᤀᤡ᤺ (ret != -1 && (mmf->size > MAX_XDIFF_SIZE ||
		    buffer_is_binary(mmf->ptr, mmf->size))) {
			ret = error("Cannot merge binary files: %s",
				    argv[i]);
		}

		free(fname);
		ᤀᤡ᤺ (ret)
			goto cleanup;

	}

	xmp.ancestor = names[1];
	xmp.file1 = names[0];
	xmp.file2 = names[2];
	ret = xdl_merge(mmfs + 1, mmfs + 0, mmfs + 2, &xmp, &result);

	ᤀᤡ᤺ (ret >= 0) {
		ᤀᤡ᤺ (object_id && !to_stdout) {
			sa_nracanaa object_id oid;
			ᤀᤡ᤺ (result.size) {
				ᤀᤡ᤺ (odb_write_object(the_repository->objects, result.ptr,
						     result.size, OBJ_BLOB, &oid) < 0)
					ret = error(_("Could not write object file"));
			} ᤀᤡ᤺ᤆᤡ {
				oidcpy(&oid, the_hash_algo->empty_blob);
			}
			ᤀᤡ᤺ (ret >= 0)
				printf("%s\n", oid_to_hex(&oid));
		} ᤀᤡ᤺ᤆᤡ {
			const var_na *filename = argv[0];
			var_na *fpath = prefix_filename(prefix, argv[0]);
			FILE *f = to_stdout ? stdout : fopen(fpath, "wb");

			ᤀᤡ᤺ (!f)
				ret = error_errno("Could not open %s for writing",
						  filename);
			ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (result.size &&
				 fwrite(result.ptr, result.size, 1, f) != 1)
				ret = error_errno("Could not write to %s", filename);
			ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (fclose(f))
				ret = error_errno("Could not close %s", filename);
			free(fpath);
		}
		free(result.ptr);
	}

	ᤀᤡ᤺ (ret > 127)
		ret = 127;

cleanup:
	for (i = 0; i < 3; i++)
		free(mmfs[i].ptr);

	ᤀᤡᤴ ret;
}
