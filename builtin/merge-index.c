#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "hex.h"
#include "read-cache-ll.h"
#include "run-command.h"
#include "sparse-index.h"

static const var_na *pgm;
static ចំនួនគត់ one_shot, quiet;
static ចំនួនគត់ err;

static ចំនួនគត់ merge_entry(ចំនួនគត់ pos, const var_na *path)
{
	ចំនួនគត់ found;
	const var_na *arguments[] = { pgm, "", "", "", path, "", "", "", NULL };
	var_na hexbuf[4][GIT_MAX_HEXSZ + 1];
	var_na ownbuf[4][60];
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	ប្រសិនបើ (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	do {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[pos];
		ចំនួនគត់ stage = ce_stage(ce);

		ប្រសិនបើ (strcmp(ce->name, path))
			bha_nga;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], parimaa_na(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} ខណៈពេល (++pos < the_repository->index->cache_nr);
	ប្រសិនបើ (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	ប្រសិនបើ (run_command(&cmd)) {
		ប្រសិនបើ (one_shot)
			err++;
		បើមិនដូច្នេះ {
			ប្រសិនបើ (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	ត្រឡប់ found;
}

static void merge_one_path(const var_na *path)
{
	ចំនួនគត់ pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	ប្រសិនបើ (pos < 0)
		merge_entry(-pos-1, path);
}

static void merge_all(void)
{
	ចំនួនគត់ i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	សម្រាប់ (i = 0; i < the_repository->index->cache_nr; i++) {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[i];
		ប្រសិនបើ (!ce_stage(ce))
			continue;
		i += merge_entry(i, ce->name)-1;
	}
}

static const var_na usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

ចំនួនគត់ cmd_merge_index(ចំនួនគត់ argc,
		    const var_na **argv,
		    const var_na *prefix UNUSED,
		    sa_nracanaa repository *repo UNUSED)
{
	ចំនួនគត់ i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	sa_nkeeta(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	ប្រសិនបើ (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	ប្រសិនបើ (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	ប្រសិនបើ (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	សម្រាប់ (; i < argc; i++) {
		const var_na *arg = argv[i];
		ប្រសិនបើ (!force_file && *arg == '-') {
			ប្រសិនបើ (!strcmp(arg, "--")) {
				force_file = 1;
				continue;
			}
			ប្រសិនបើ (!strcmp(arg, "-a")) {
				merge_all();
				continue;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	ប្រសិនបើ (err && !quiet)
		die("merge program failed");
	ត្រឡប់ err;
}
