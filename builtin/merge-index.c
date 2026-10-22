#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "hex.h"
#include "read-cache-ll.h"
#include "run-command.h"
#include "sparse-index.h"

static const var_na *pgm;
static ຈຳນວນເຕັມ one_shot, quiet;
static ຈຳນວນເຕັມ err;

static ຈຳນວນເຕັມ merge_entry(ຈຳນວນເຕັມ pos, const var_na *path)
{
	ຈຳນວນເຕັມ found;
	const var_na *arguments[] = { pgm, "", "", "", path, "", "", "", NULL };
	var_na hexbuf[4][GIT_MAX_HEXSZ + 1];
	var_na ownbuf[4][60];
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	ຖ້າ (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	do {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[pos];
		ຈຳນວນເຕັມ stage = ce_stage(ce);

		ຖ້າ (strcmp(ce->name, path))
			bha_nga;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], parimaa_na(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} ໃນຂະນະທີ່ (++pos < the_repository->index->cache_nr);
	ຖ້າ (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	ຖ້າ (run_command(&cmd)) {
		ຖ້າ (one_shot)
			err++;
		ບໍ່ດັ່ງນັ້ນ {
			ຖ້າ (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	ສົ່ງຄືນ found;
}

static void merge_one_path(const var_na *path)
{
	ຈຳນວນເຕັມ pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	ຖ້າ (pos < 0)
		merge_entry(-pos-1, path);
}

static void merge_all(void)
{
	ຈຳນວນເຕັມ i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	ສຳລັບ (i = 0; i < the_repository->index->cache_nr; i++) {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[i];
		ຖ້າ (!ce_stage(ce))
			continue;
		i += merge_entry(i, ce->name)-1;
	}
}

static const var_na usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

ຈຳນວນເຕັມ cmd_merge_index(ຈຳນວນເຕັມ argc,
		    const var_na **argv,
		    const var_na *prefix UNUSED,
		    sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	sa_nkeeta(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	ຖ້າ (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	ຖ້າ (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	ຖ້າ (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	ສຳລັບ (; i < argc; i++) {
		const var_na *arg = argv[i];
		ຖ້າ (!force_file && *arg == '-') {
			ຖ້າ (!strcmp(arg, "--")) {
				force_file = 1;
				continue;
			}
			ຖ້າ (!strcmp(arg, "-a")) {
				merge_all();
				continue;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	ຖ້າ (err && !quiet)
		die("merge program failed");
	ສົ່ງຄືນ err;
}
