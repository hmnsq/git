#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "commit.h"
#include "diff.h"
#include "dir.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "revision.h"
#include "reachable.h"
#include "parse-options.h"
#include "path.h"
#include "progress.h"
#include "prune-packed.h"
#include "replace-object.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"
#include "shallow.h"

static const char * const prune_usage[] = {
	N_("git prune [-n] [-v] [--progress] [--expire <time>] [--] [<head>...]"),
	NULL
};
static целое show_only;
static целое verbose;
static timestamp_t expire;
static целое show_progress = -1;

static целое prune_tmp_file(const char *fullpath)
{
	struct stat st;
	если (lstat(fullpath, &st))
		вернуть error("Could not stat '%s'", fullpath);
	если (st.st_mtime > expire)
		вернуть 0;
	если (S_ISDIR(st.st_mode)) {
		если (show_only || verbose)
			printf("Removing stale temporary directory %s\n", fullpath);
		если (!show_only) {
			struct strbuf remove_dir_buf = STRBUF_INIT;

			strbuf_addstr(&remove_dir_buf, fullpath);
			remove_dir_recursively(&remove_dir_buf, 0);
			strbuf_release(&remove_dir_buf);
		}
	} иначе {
		если (show_only || verbose)
			printf("Removing stale temporary file %s\n", fullpath);
		если (!show_only)
			unlink_or_warn(fullpath);
	}
	вернуть 0;
}

static void perform_reachability_traversal(struct rev_info *revs)
{
	static целое initialized;
	struct progress *progress = NULL;

	если (initialized)
		вернуть;

	если (show_progress)
		progress = start_delayed_progress(revs->repo,
						  _("Checking connectivity"), 0);
	mark_reachable_objects(revs, 1, expire, progress);
	stop_progress(&progress);
	initialized = 1;
}

static целое is_object_reachable(const struct object_id *oid,
			       struct rev_info *revs)
{
	struct object *obj;

	perform_reachability_traversal(revs);

	obj = lookup_object(revs->repo, oid);
	вернуть obj && (obj->flags & SEEN);
}

static целое prune_object(const struct object_id *oid, const char *fullpath,
			void *data)
{
	struct rev_info *revs = data;
	struct stat st;

	если (is_object_reachable(oid, revs))
		вернуть 0;

	если (lstat(fullpath, &st)) {
		/* report errors, but do not stop pruning */
		error("Could not stat '%s'", fullpath);
		вернуть 0;
	}
	если (st.st_mtime > expire)
		вернуть 0;
	если (show_only || verbose) {
		enum object_type type =
			odb_read_object_info(revs->repo->objects, oid, NULL);
		printf("%s %s\n", oid_to_hex(oid),
		       (type > 0) ? type_name(type) : "unknown");
	}
	если (!show_only)
		unlink_or_warn(fullpath);
	вернуть 0;
}

static целое prune_cruft(const char *basename, const char *path,
		       void *data UNUSED)
{
	если (starts_with(basename, "tmp_obj_"))
		prune_tmp_file(path);
	иначе
		fprintf(stderr, "bad sha1 file: %s\n", path);
	вернуть 0;
}

static целое prune_subdir(unsigned целое nr UNUSED, const char *path,
			void *data UNUSED)
{
	если (!show_only)
		rmdir(path);
	вернуть 0;
}

/*
 * Write errors (particularly out of space) can result in
 * failed temporary packs (and more rarely indexes and other
 * files beginning with "tmp_") accumulating in the object
 * and the pack directories.
 */
static void remove_temporary_files(const char *path)
{
	DIR *dir;
	struct dirent *de;

	dir = opendir(path);
	если (!dir) {
		если (errno != ENOENT)
			fprintf(stderr, "Unable to open directory %s: %s\n",
				path, strerror(errno));
		вернуть;
	}
	пока ((de = readdir(dir)) != NULL)
		если (starts_with(de->d_name, "tmp_"))
			prune_tmp_file(mkpath("%s/%s", path, de->d_name));
	closedir(dir);
}

целое cmd_prune(целое argc,
	      const char **argv,
	      const char *prefix,
	      struct repository *repo)
{
	struct rev_info revs;
	целое exclude_promisor_objects = 0;
	const struct option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned objects")),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("expire objects older than <time>")),
		OPT_BOOL(0, "exclude-promisor-objects", &exclude_promisor_objects,
			 N_("limit traversal to objects outside promisor packfiles")),
		OPT_END()
	};
	char *s;

	expire = TIME_MAX;
	save_commit_buffer = 0;
	disable_replace_refs();

	argc = parse_options(argc, argv, prefix, options, prune_usage, 0);

	repo_init_revisions(repo, &revs, prefix);
	если (repo->repository_format_precious_objects)
		die(_("cannot prune in a precious-objects repo"));

	пока (argc--) {
		struct object_id oid;
		const char *name = *argv++;

		если (!repo_get_oid(repo, name, &oid)) {
			struct object *object = parse_object_or_die(repo, &oid, name);
			add_pending_object(&revs, object, "");
		}
		иначе
			die("unrecognized argument: %s", name);
	}

	если (show_progress == -1)
		show_progress = isatty(2);
	если (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;
		revs.exclude_promisor_objects = 1;
	}

	for_each_loose_file_in_source(repo->objects->source,
				      prune_object, prune_cruft, prune_subdir, &revs);

	prune_packed_objects(show_only ? PRUNE_PACKED_DRY_RUN : 0);
	remove_temporary_files(repo_get_object_directory(repo));
	s = mkpathdup("%s/pack", repo_get_object_directory(repo));
	remove_temporary_files(s);
	free(s);

	если (is_repository_shallow(repo)) {
		perform_reachability_traversal(&revs);
		prune_shallow(show_only ? PRUNE_SHOW_ONLY : 0);
	}

	release_revisions(&revs);
	вернуть 0;
}
