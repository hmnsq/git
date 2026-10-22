#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "hex.h"
#include "read-cache-ll.h"
#include "run-command.h"
#include "sparse-index.h"

static const char *pgm;
static ціле one_shot, quiet;
static ціле err;

static ціле merge_entry(ціле pos, const char *path)
{
	ціле found;
	const char *arguments[] = { pgm, "", "", "", path, "", "", "", NULL };
	char hexbuf[4][GIT_MAX_HEXSZ + 1];
	char ownbuf[4][60];
	struct child_process cmd = CHILD_PROCESS_INIT;

	якщо (pos >= the_repository->index->cache_nr)
		die("git merge-index: %s not in the cache", path);
	found = 0;
	do {
		const struct cache_entry *ce = the_repository->index->cache[pos];
		ціле stage = ce_stage(ce);

		якщо (strcmp(ce->name, path))
			break;
		found++;
		oid_to_hex_r(hexbuf[stage], &ce->oid);
		xsnprintf(ownbuf[stage], sizeof(ownbuf[stage]), "%o", ce->ce_mode);
		arguments[stage] = hexbuf[stage];
		arguments[stage + 4] = ownbuf[stage];
	} поки (++pos < the_repository->index->cache_nr);
	якщо (!found)
		die("git merge-index: %s not in the cache", path);

	strvec_pushv(&cmd.args, arguments);
	якщо (run_command(&cmd)) {
		якщо (one_shot)
			err++;
		інакше {
			якщо (!quiet)
				die("merge program failed");
			exit(1);
		}
	}
	повернути found;
}

static void merge_one_path(const char *path)
{
	ціле pos = index_name_pos(the_repository->index, path, strlen(path));

	/*
	 * If it already exists in the cache as stage0, it's
	 * already merged and there is nothing to do.
	 */
	якщо (pos < 0)
		merge_entry(-pos-1, path);
}

static void merge_all(void)
{
	ціле i;
	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	для (i = 0; i < the_repository->index->cache_nr; i++) {
		const struct cache_entry *ce = the_repository->index->cache[i];
		якщо (!ce_stage(ce))
			continue;
		i += merge_entry(i, ce->name)-1;
	}
}

static const char usage_string[] =
"git merge-index [-o] [-q] <merge-program> (-a | [--] [<filename>...])";

ціле cmd_merge_index(ціле argc,
		    const char **argv,
		    const char *prefix UNUSED,
		    struct repository *repo UNUSED)
{
	ціле i, force_file = 0;

	/* Without this we cannot rely on waitpid() to tell
	 * what happened to our children.
	 */
	signal(SIGCHLD, SIG_DFL);

	show_usage_if_asked(argc, argv, usage_string);

	якщо (argc < 3)
		usage(usage_string);

	repo_read_index(the_repository);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);

	i = 1;
	якщо (!strcmp(argv[i], "-o")) {
		one_shot = 1;
		i++;
	}
	якщо (!strcmp(argv[i], "-q")) {
		quiet = 1;
		i++;
	}
	pgm = argv[i++];
	для (; i < argc; i++) {
		const char *arg = argv[i];
		якщо (!force_file && *arg == '-') {
			якщо (!strcmp(arg, "--")) {
				force_file = 1;
				continue;
			}
			якщо (!strcmp(arg, "-a")) {
				merge_all();
				continue;
			}
			die("git merge-index: unknown option %s", arg);
		}
		merge_one_path(arg);
	}
	якщо (err && !quiet)
		die("merge program failed");
	повернути err;
}
