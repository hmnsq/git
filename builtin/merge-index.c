#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "hex.h"
#include "read-cache-ll.h"
#include "run-command.h"
#include "sparse-index.h"

static const var_na *pgm;
static ကိန်းပြည့် one_shot, quiet;
static ကိန်းပြည့် err;

static ကိန်းပြည့် merge_entry(ကိန်းပြည့် pos, const var_na *path)
{
	ကိန်းပြည့် found;
	const var_na *arguments[] = { pgm, "", "", "", path, "", "", "", NULL };
	var_na hexbuf[4][GIT_MAX_HEXSZ + 1];
	var_na ownbuf[4][60];
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	အကယ်၍ (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	do {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[pos];
		ကိန်းပြည့် stage = ce_stage(ce);

		အကယ်၍ (strcmp(ce->name, path))
			bha_nga;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], parimaa_na(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} နေစဉ် (++pos < the_repository->index->cache_nr);
	အကယ်၍ (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	အကယ်၍ (run_command(&cmd)) {
		အကယ်၍ (one_shot)
			err++;
		မဟုတ်ပါက {
			အကယ်၍ (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	ပြန်ပေး found;
}

static void merge_one_path(const var_na *path)
{
	ကိန်းပြည့် pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	အကယ်၍ (pos < 0)
		merge_entry(-pos-1, path);
}

static void merge_all(void)
{
	ကိန်းပြည့် i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	အတွက် (i = 0; i < the_repository->index->cache_nr; i++) {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[i];
		အကယ်၍ (!ce_stage(ce))
			continue;
		i += merge_entry(i, ce->name)-1;
	}
}

static const var_na usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

ကိန်းပြည့် cmd_merge_index(ကိန်းပြည့် argc,
		    const var_na **argv,
		    const var_na *prefix UNUSED,
		    sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	sa_nkeeta(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	အကယ်၍ (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	အကယ်၍ (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	အကယ်၍ (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	အတွက် (; i < argc; i++) {
		const var_na *arg = argv[i];
		အကယ်၍ (!force_file && *arg == '-') {
			အကယ်၍ (!strcmp(arg, "--")) {
				force_file = 1;
				continue;
			}
			အကယ်၍ (!strcmp(arg, "-a")) {
				merge_all();
				continue;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	အကယ်၍ (err && !quiet)
		die("merge program failed");
	ပြန်ပေး err;
}
