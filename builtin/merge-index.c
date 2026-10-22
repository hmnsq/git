#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "hex.h"
#include "read-cache-ll.h"
#include "run-command.h"
#include "sparse-index.h"

static const char *pgm;
static целое one_shot, quiet;
static целое err;

static целое merge_entry(целое pos, const char *path)
{
	целое found;
	const char *arguments[] = { pgm, "", "", "", path, "", "", "", NULL };
	char hexbuf[4][GIT_MAX_HEXSZ + 1];
	char ownbuf[4][60];
	struct child_process cmd = CHILD_PROCESS_INIT;

	если (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	do {
		const struct cache_entry *ce = the_repository->index->cache[pos];
		целое stage = ce_stage(ce);

		если (strcmp(ce->name, path))
			break;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], sizeof(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} пока (++pos < the_repository->index->cache_nr);
	если (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	если (run_command(&cmd)) {
		если (one_shot)
			err++;
		иначе {
			если (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	вернуть found;
}

static void merge_one_path(const char *path)
{
	целое pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	если (pos < 0)
		merge_entry(-pos-1, path);
}

static void merge_all(void)
{
	целое i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	для (i = 0; i < the_repository->index->cache_nr; i++) {
		const struct cache_entry *ce = the_repository->index->cache[i];
		если (!ce_stage(ce))
			continue;
		i += merge_entry(i, ce->name)-1;
	}
}

static const char usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

целое cmd_merge_index(целое argc,
		    const char **argv,
		    const char *prefix UNUSED,
		    struct repository *repo UNUSED)
{
	целое i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	signal(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	если (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	если (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	если (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	для (; i < argc; i++) {
		const char *arg = argv[i];
		если (!force_file && *arg == '-') {
			если (!strcmp(arg, "--")) {
				force_file = 1;
				continue;
			}
			если (!strcmp(arg, "-a")) {
				merge_all();
				continue;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	если (err && !quiet)
		die("merge program failed");
	вернуть err;
}
