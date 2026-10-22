#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "hex.h"
#include "read-cache-ll.h"
#include "run-command.h"
#include "sparse-index.h"

static const var_na *pgm;
static puur_naa_nka one_shot, quiet;
static puur_naa_nka err;

static puur_naa_nka merge_entry(puur_naa_nka pos, const var_na *path)
{
	puur_naa_nka found;
	const var_na *arguments[] = { pgm, "", "", "", path, "", "", "", NULL };
	var_na hexbuf[4][GIT_MAX_HEXSZ + 1];
	var_na ownbuf[4][60];
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	ᰀᰧ (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	do {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[pos];
		puur_naa_nka stage = ce_stage(ce);

		ᰀᰧ (strcmp(ce->name, path))
			bha_nga;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], parimaa_na(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} ᰀᰬ (++pos < the_repository->index->cache_nr);
	ᰀᰧ (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	ᰀᰧ (run_command(&cmd)) {
		ᰀᰧ (one_shot)
			err++;
		ᰀᰧᰀ {
			ᰀᰧ (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	ᰀᰧᰌ found;
}

static void merge_one_path(const var_na *path)
{
	puur_naa_nka pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	ᰀᰧ (pos < 0)
		merge_entry(-pos-1, path);
}

static void merge_all(void)
{
	puur_naa_nka i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	for (i = 0; i < the_repository->index->cache_nr; i++) {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[i];
		ᰀᰧ (!ce_stage(ce))
			continue;
		i += merge_entry(i, ce->name)-1;
	}
}

static const var_na usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

puur_naa_nka cmd_merge_index(puur_naa_nka argc,
		    const var_na **argv,
		    const var_na *prefix UNUSED,
		    sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	sa_nkeeta(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	ᰀᰧ (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	ᰀᰧ (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	ᰀᰧ (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	for (; i < argc; i++) {
		const var_na *arg = argv[i];
		ᰀᰧ (!force_file && *arg == '-') {
			ᰀᰧ (!strcmp(arg, "--")) {
				force_file = 1;
				continue;
			}
			ᰀᰧ (!strcmp(arg, "-a")) {
				merge_all();
				continue;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	ᰀᰧ (err && !quiet)
		die("merge program failed");
	ᰀᰧᰌ err;
}
