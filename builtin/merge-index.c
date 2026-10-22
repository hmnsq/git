#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "hex.h"
#include "read-cache-ll.h"
#include "run-command.h"
#include "sparse-index.h"

static const char *pgm;
static số_nguyên one_shot, quiet;
static số_nguyên err;

static số_nguyên merge_entry(số_nguyên pos, const char *path)
{
	số_nguyên found;
	const char *arguments[] = { pgm, "", "", "", path, "", "", "", NULL };
	char hexbuf[4][GIT_MAX_HEXSZ + 1];
	char ownbuf[4][60];
	struct child_process cmd = CHILD_PROCESS_INIT;

	nếu (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	do {
		const struct cache_entry *ce = the_repository->index->cache[pos];
		số_nguyên stage = ce_stage(ce);

		nếu (strcmp(ce->name, path))
			break;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], sizeof(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} trong_khi (++pos < the_repository->index->cache_nr);
	nếu (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	nếu (run_command(&cmd)) {
		nếu (one_shot)
			err++;
		không_thì {
			nếu (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	trả_về found;
}

static void merge_one_path(const char *path)
{
	số_nguyên pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	nếu (pos < 0)
		merge_entry(-pos-1, path);
}

static void merge_all(void)
{
	số_nguyên i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	cho (i = 0; i < the_repository->index->cache_nr; i++) {
		const struct cache_entry *ce = the_repository->index->cache[i];
		nếu (!ce_stage(ce))
			continue;
		i += merge_entry(i, ce->name)-1;
	}
}

static const char usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

số_nguyên cmd_merge_index(số_nguyên argc,
		    const char **argv,
		    const char *prefix UNUSED,
		    struct repository *repo UNUSED)
{
	số_nguyên i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	signal(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	nếu (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	nếu (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	nếu (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	cho (; i < argc; i++) {
		const char *arg = argv[i];
		nếu (!force_file && *arg == '-') {
			nếu (!strcmp(arg, "--")) {
				force_file = 1;
				continue;
			}
			nếu (!strcmp(arg, "-a")) {
				merge_all();
				continue;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	nếu (err && !quiet)
		die("merge program failed");
	trả_về err;
}
