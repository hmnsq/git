#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "hex.h"
#include "read-cache-ll.h"
#include "run-command.h"
#include "sparse-index.h"

static const char *pgm;
static цео one_shot, quiet;
static цео err;

static цео merge_entry(цео pos, const char *path)
{
	цео found;
	const char *arguments[] = { pgm, "", "", "", path, "", "", "", NULL };
	char hexbuf[4][GIT_MAX_HEXSZ + 1];
	char ownbuf[4][60];
	struct child_process cmd = CHILD_PROCESS_INIT;

	ако (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	do {
		const struct cache_entry *ce = the_repository->index->cache[pos];
		цео stage = ce_stage(ce);

		ако (strcmp(ce->name, path))
			break;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], sizeof(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} док (++pos < the_repository->index->cache_nr);
	ако (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	ако (run_command(&cmd)) {
		ако (one_shot)
			err++;
		инако {
			ако (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	врати found;
}

static void merge_one_path(const char *path)
{
	цео pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	ако (pos < 0)
		merge_entry(-pos-1, path);
}

static void merge_all(void)
{
	цео i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	за (i = 0; i < the_repository->index->cache_nr; i++) {
		const struct cache_entry *ce = the_repository->index->cache[i];
		ако (!ce_stage(ce))
			continue;
		i += merge_entry(i, ce->name)-1;
	}
}

static const char usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

цео cmd_merge_index(цео argc,
		    const char **argv,
		    const char *prefix UNUSED,
		    struct repository *repo UNUSED)
{
	цео i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	signal(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	ако (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	ако (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	ако (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	за (; i < argc; i++) {
		const char *arg = argv[i];
		ако (!force_file && *arg == '-') {
			ако (!strcmp(arg, "--")) {
				force_file = 1;
				continue;
			}
			ако (!strcmp(arg, "-a")) {
				merge_all();
				continue;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	ако (err && !quiet)
		die("merge program failed");
	врати err;
}
