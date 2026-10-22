#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "hex.h"
#include "read-cache-ll.h"
#include "run-command.h"
#include "sparse-index.h"

static const char *pgm;
static 정수 one_shot, quiet;
static 정수 err;

static 정수 merge_entry(정수 pos, const char *path)
{
	정수 found;
	const char *arguments[] = { pgm, "", "", "", path, "", "", "", NULL };
	char hexbuf[4][GIT_MAX_HEXSZ + 1];
	char ownbuf[4][60];
	struct child_process cmd = CHILD_PROCESS_INIT;

	만약 (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	do {
		const struct cache_entry *ce = the_repository->index->cache[pos];
		정수 stage = ce_stage(ce);

		만약 (strcmp(ce->name, path))
			break;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], sizeof(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} 동안 (++pos < the_repository->index->cache_nr);
	만약 (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	만약 (run_command(&cmd)) {
		만약 (one_shot)
			err++;
		아니면 {
			만약 (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	반환 found;
}

static void merge_one_path(const char *path)
{
	정수 pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	만약 (pos < 0)
		merge_entry(-pos-1, path);
}

static void merge_all(void)
{
	정수 i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	위해 (i = 0; i < the_repository->index->cache_nr; i++) {
		const struct cache_entry *ce = the_repository->index->cache[i];
		만약 (!ce_stage(ce))
			continue;
		i += merge_entry(i, ce->name)-1;
	}
}

static const char usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

정수 cmd_merge_index(정수 argc,
		    const char **argv,
		    const char *prefix UNUSED,
		    struct repository *repo UNUSED)
{
	정수 i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	signal(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	만약 (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	만약 (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	만약 (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	위해 (; i < argc; i++) {
		const char *arg = argv[i];
		만약 (!force_file && *arg == '-') {
			만약 (!strcmp(arg, "--")) {
				force_file = 1;
				continue;
			}
			만약 (!strcmp(arg, "-a")) {
				merge_all();
				continue;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	만약 (err && !quiet)
		die("merge program failed");
	반환 err;
}
