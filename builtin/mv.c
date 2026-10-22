/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "name-hash.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "pathspec.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "symlinks.h"

#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "entry.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	ଖାଲି
};

ଗଣନା update_mode {
	WORKING_DIRECTORY = (1 << 1),
	INDEX = (1 << 2),
	SPARSE = (1 << 3),
	SKIP_WORKTREE_DIR = (1 << 4),
	/*
	 * A file gets moved implicitly via a move of one of its parent
	 * directories. This flag causes us to skip the check that we don't try
	 * to move a file and any of its parent directories at the same point
	 * in time.
	 */
	MOVE_VIA_PARENT_DIR = (1 << 5),
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା needs_worktree_rename(ଗଣନା update_mode mode, ଗଣନା update_mode dst_mode)
{
	ଫେରନ୍ତୁ !(mode & (INDEX | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#define DUP_BASENAME 1
#define KEEP_TRAILING_SLASH 2

ସ୍ଥିର ଶୂନ୍ୟ internal_prefix_pathspec(ସଂରଚନା strvec *out,
				     ଅଚଳ ଅକ୍ଷର *prefix,
				     ଅଚଳ ଅକ୍ଷର **pathspec,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned flags)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefixlen = prefix ? strlen(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++) {
		size_t length = strlen(pathspec[i]);
		size_t to_copy = length;
		ଅଚଳ ଅକ୍ଷର *maybe_basename;
		ଅକ୍ଷର *trimmed, *prefixed_path;

		ଯେତେବେଳେ (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_ଠେଲ(out, prefixed_path);

		ମୁକ୍ତ(prefixed_path);
		ମୁକ୍ତ(trimmed);
	}
}

ସ୍ଥିର ଅକ୍ଷର *add_slash(ଅଚଳ ଅକ୍ଷର *path)
{
	size_t len = strlen(path);
	ଯଦି (len && path[len - 1] != '/') {
		ଅକ୍ଷର *with_slash = xmalloc(st_add(len, 2));
		memcpy(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		ଫେରନ୍ତୁ with_slash;
	}
	ଫେରନ୍ତୁ xstrdup(path);
}

#define SUBMODULE_WITH_GITDIR ((ଅଚଳ ଅକ୍ଷର *)1)

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *submodule_gitfile_path(ଅଚଳ ଅକ୍ଷର *src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା first)
{
	ସଂରଚନା strbuf submodule_dotgit = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *path;

	ଯଦି (!S_ISGITLINK(the_repository->index->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	ଯଦି (!is_staging_gitmodules_ok(the_repository->index))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	ଯଦି (path)
		ଫେରନ୍ତୁ path;
	ଫେରନ୍ତୁ SUBMODULE_WITH_GITDIR;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା index_range_of_same_dir(ଅଚଳ ଅକ୍ଷର *src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା length,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା *first_p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *last_p)
{
	ଅକ୍ଷର *src_w_slash = add_slash(src);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା first, last, len_w_slash = length + 1;

	first = index_name_pos(the_repository->index, src_w_slash, len_w_slash);
	ଯଦି (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	ପାଇଁ (last = first; last < the_repository->index->cache_nr; last++) {
		ଅଚଳ ଅକ୍ଷର *path = the_repository->index->cache[last]->name;
		ଯଦି (strncmp(path, src_w_slash, len_w_slash))
			ଭାଙ୍ଗ;
	}

	ମୁକ୍ତ(src_w_slash);
	*first_p = first;
	*last_p = last;
	ଫେରନ୍ତୁ last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା empty_dir_has_sparse_contents(ଅଚଳ ଅକ୍ଷର *name)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ଅକ୍ଷର *with_slash = add_slash(name);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = strlen(with_slash);

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = index_name_pos(the_repository->index, with_slash, length);
	ଅଚଳ ସଂରଚନା cache_entry *ce;

	ଯଦି (pos < 0) {
		pos = -pos - 1;
		ଯଦି (pos >= the_repository->index->cache_nr)
			ଯାଅ free_return;
		ce = the_repository->index->cache[pos];
		ଯଦି (strncmp(with_slash, ce->name, length))
			ଯାଅ free_return;
		ଯଦି (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	ମୁକ୍ତ(with_slash);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ remove_empty_src_dirs(ଅଚଳ ଅକ୍ଷର **src_dir, size_t src_dir_nr)
{
	size_t i;
	ସଂରଚନା strbuf a_src_dir = STRBUF_INIT;

	ପାଇଁ (i = 0; i < src_dir_nr; i++) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		ଯଦି (index_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

ସଂରଚନା pathmap_entry {
	ସଂରଚନା hashmap_entry ent;
	ଅଚଳ ଅକ୍ଷର *path;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pathmap_cmp(ଅଚଳ ଶୂନ୍ୟ *cmp_data UNUSED,
		       ଅଚଳ ସଂରଚନା hashmap_entry *a,
		       ଅଚଳ ସଂରଚନା hashmap_entry *b,
		       ଅଚଳ ଶୂନ୍ୟ *ଚାବି UNUSED)
{
	ଅଚଳ ସଂରଚନା pathmap_entry *e1 = container_of(a, ସଂରଚନା pathmap_entry, ent);
	ଅଚଳ ସଂରଚନା pathmap_entry *e2 = container_of(b, ସଂରଚନା pathmap_entry, ent);
	ଫେରନ୍ତୁ fspathcmp(e1->path, e2->path);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_mv(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	   ଅଚଳ ଅକ୍ଷର **argv,
	   ଅଚଳ ଅକ୍ଷର *prefix,
	   ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, flags, gitmodules_modified = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	ସଂରଚନା option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT__DRY_RUN(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', ଖାଲି, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_END(),
	};
	ସଂରଚନା strvec sources = STRVEC_INIT;
	ସଂରଚନା strvec dest_paths = STRVEC_INIT;
	ସଂରଚନା strvec destinations = STRVEC_INIT;
	ସଂରଚନା strvec submodule_gitfiles_to_free = STRVEC_INIT;
	ଅଚଳ ଅକ୍ଷର **submodule_gitfiles;
	ଅକ୍ଷର *dst_w_slash = ଖାଲି;
	ସଂରଚନା strvec src_dir = STRVEC_INIT;
	ଗଣନା update_mode *modes, dst_mode = 0;
	ସଂରଚନା stat st, dest_st;
	ସଂରଚନା string_list src_for_dst = STRING_LIST_INIT_DUP;
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ସଂରଚନା cache_entry *ce;
	ସଂରଚନା string_list only_match_skip_worktree = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list dirty_paths = STRING_LIST_INIT_DUP;
	ସଂରଚନା hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, ଖାଲି);
	ସଂରଚନା strbuf pathbuf = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	repo_config(the_repository, git_default_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	ଯଦି (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	ଯଦି (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	ଯଦି (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, ଆକାର(ଅକ୍ଷର *));

	ଯଦି (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	ଅନ୍ୟଥା ଯଦି (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} ଅନ୍ୟଥା ଯଦି (!path_in_sparse_checkout(dst_w_slash, the_repository->index) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} ଅନ୍ୟଥା ଯଦି (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} ଅନ୍ୟଥା {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		ଯଦି (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->index))
			dst_mode = SPARSE;
	}

	/* Checking */
	ପାଇଁ (i = 0; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *src = sources.v[i], *dst = destinations.v[i];
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା length;
		ଅଚଳ ଅକ୍ଷର *bad = ଖାଲି;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip_sparse = 0;

		ଯଦି (show_only)
			printf(_("Checking rename of '%s' to '%s'\n"), src, dst);

		length = strlen(src);
		ଯଦି (lstat(src, &st) < 0) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
			ଅଚଳ ସଂରଚନା cache_entry *ce;

			pos = index_name_pos(the_repository->index, src, length);
			ଯଦି (pos < 0) {
				ଅକ୍ଷର *src_w_slash = add_slash(src);
				ଯଦି (!path_in_sparse_checkout(src_w_slash, the_repository->index) &&
				    empty_dir_has_sparse_contents(src)) {
					ମୁକ୍ତ(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					ଯାଅ dir_check;
				}
				ମୁକ୍ତ(src_w_slash);
				/* only error if existence is expected. */
				ଯଦି (!(modes[i] & SPARSE))
					bad = _("bad source");
				ଯାଅ act_on_entry;
			}
			ce = the_repository->index->cache[pos];
			ଯଦି (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				ଯାଅ act_on_entry;
			}
			ଯଦି (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				ଯାଅ act_on_entry;
			}
			/* Check if dst exists in index */
			ଯଦି (index_name_pos(the_repository->index, dst, strlen(dst)) < 0) {
				modes[i] |= SPARSE;
				ଯାଅ act_on_entry;
			}
			ଯଦି (!force) {
				bad = _("destination exists");
				ଯାଅ act_on_entry;
			}
			modes[i] |= SPARSE;
			ଯାଅ act_on_entry;
		}
		ଯଦି (!strncmp(src, dst, length) &&
		    (dst[length] == 0 || dst[length] == '/')) {
			bad = _("can not move directory into itself");
			ଯାଅ act_on_entry;
		}
		ଯଦି (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			ଯାଅ act_on_entry;
		}

dir_check:
		ଯଦି (S_ISDIR(st.st_mode)) {
			ସଂରଚନା pathmap_entry *entry;
			ଅକ୍ଷର *dst_with_slash;
			size_t dst_with_slash_len;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, n;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା first = index_name_pos(the_repository->index, src, length), last;

			ଯଦି (first >= 0) {
				ଅଚଳ ଅକ୍ଷର *path = submodule_gitfile_path(src, first);
				ଯଦି (path != SUBMODULE_WITH_GITDIR)
					path = strvec_ଠେଲ(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				ଯାଅ act_on_entry;
			} ଅନ୍ୟଥା ଯଦି (index_range_of_same_dir(src, length,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				ଯାଅ act_on_entry;
			}

			entry = xmalloc(ଆକାର(*entry));
			entry->path = src;
			hashmap_entry_init(&entry->ent, fspathhash(src));
			hashmap_add(&moved_dirs, &entry->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_ଠେଲ(&src_dir, src);

			n = argc + last - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = add_slash(dst);
			dst_with_slash_len = strlen(dst_with_slash);

			ପାଇଁ (j = 0; j < last - first; j++) {
				ଅଚଳ ସଂରଚନା cache_entry *ce = the_repository->index->cache[first + j];
				ଅଚଳ ଅକ୍ଷର *path = ce->name;
				ଅକ୍ଷର *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + length + 1);

				strvec_ଠେଲ(&sources, path);
				strvec_ଠେଲ(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : INDEX);
				submodule_gitfiles[argc + j] = ଖାଲି;

				ମୁକ୍ତ(prefixed_path);
			}

			ମୁକ୍ତ(dst_with_slash);
			argc += last - first;
			ଯାଅ act_on_entry;
		}
		ଯଦି (!(ce = index_file_exists(the_repository->index, src, length, 0))) {
			bad = _("not under version control");
			ଯାଅ act_on_entry;
		}
		ଯଦି (ce_stage(ce)) {
			bad = _("conflicted");
			ଯାଅ act_on_entry;
		}
		ଯଦି (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			ଯଦି (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				ଯଦି (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					ଯଦି (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = ଖାଲି;
				} ଅନ୍ୟଥା
					bad = _("Cannot overwrite");
			}
			ଯାଅ act_on_entry;
		}
		ଯଦି (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			ଯାଅ act_on_entry;
		}
		ଯଦି (is_dir_sep(dst[strlen(dst) - 1])) {
			bad = _("destination directory does not exist");
			ଯାଅ act_on_entry;
		}
		ଯଦି (has_symlink_leading_path(dst, strlen(dst))) {
			bad = _("destination is beyond a symbolic link");
			ଯାଅ act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		ଯଦି (needs_worktree_rename(modes[i], dst_mode)) {
			ଅଚଳ ଅକ୍ଷର *slash_ = strrchr(dst, '/');

			ଯଦି (slash_) {
				ସଂରଚନା stat dir_st;
				ଅକ୍ଷର *dst_dir = xstrdup(dst);
				ଅକ୍ଷର *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				ଯଦି (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					ଯଦି (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} ଅନ୍ୟଥା ଯଦି (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				ମୁକ୍ତ(dst_dir);
			}

			ଯଦି (bad)
				ଯାଅ act_on_entry;
		}

		ଯଦି (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    index_entry_exists(the_repository->index, dst, strlen(dst))) {
			bad = _("destination exists in the index");
			ଯଦି (force) {
				ଯଦି (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = ଖାଲି;
			} ଅନ୍ୟଥା {
				ଯାଅ act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		ଯଦି (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		ଯଦି (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		ଯଦି (skip_sparse)
			ଯାଅ remove_entry;

		string_list_ଭର୍ତ୍ତି(&src_for_dst, dst);

act_on_entry:
		ଯଦି (!bad)
			ଜାରି;
		ଯଦି (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		ଯଦି (--argc > 0) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	ପାଇଁ (i = 0; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *slash_pos;

		ଯଦି (modes[i] & MOVE_VIA_PARENT_DIR)
			ଜାରି;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		ଯେତେବେଳେ (slash_pos > pathbuf.buf) {
			ସଂରଚନା pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			ଯଦି (hashmap_get_entry(&moved_dirs, &needle, ent, ଖାଲି))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	ଯଦି (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ଯଦି (!ignore_errors) {
			ret = 1;
			ଯାଅ out;
		}
	}

	ପାଇଁ (i = 0; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *src = sources.v[i], *dst = destinations.v[i];
		ଗଣନା update_mode mode = modes[i];
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_and_dirty = 0;
		ସଂରଚନା checkout state = CHECKOUT_INIT;
		state.istate = the_repository->index;

		ଯଦି (force)
			state.force = 1;
		ଯଦି (show_only || verbose)
			printf(_("Renaming %s to %s\n"), src, dst);
		ଯଦି (show_only)
			ଜାରି;
		ଯଦି (needs_worktree_rename(mode, dst_mode) &&
		    rename(src, dst) < 0) {
			ଯଦି (ignore_errors)
				ଜାରି;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		ଯଦି (submodule_gitfiles[i]) {
			ଯଦି (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			ଯଦି (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_ବୃକ୍ଷ_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		ଯଦି (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			ଜାରି;

		pos = index_name_pos(the_repository->index, src, strlen(src));
		ଯଦି (pos < 0)
			BUG("could not find source in index: '%s'", src);
		ଯଦି (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->index,
						       the_repository->index->cache[pos],
						       &st,
						       0);
		rename_index_entry_at(the_repository->index, pos, dst);

		ଯଦି (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			ଯଦି ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->index)) {
				/* from out-of-cone to in-cone */
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				ସଂରଚନା cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				ଯଦି (checkout_entry(dst_ce, &state, ଖାଲି, ଖାଲି))
					die(_("cannot checkout %s"), dst_ce->name);
			} ଅନ୍ୟଥା ଯଦି ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->index)) {
				/* from in-cone to out-of-cone */
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				ସଂରଚନା cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				ଯଦି (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} ଅନ୍ୟଥା {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					ଅକ୍ଷର *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					safe_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_empty_src_dirs(src_dir.v, src_dir.nr);

	ଯଦି (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	ଯଦି (gitmodules_modified)
		stage_updated_gitmodules(the_repository->index);

	ଯଦି (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

out:
	strvec_clear(&src_dir);
	ମୁକ୍ତ(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_match_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_to_free);
	hashmap_clear_and_free(&moved_dirs, ସଂରଚନା pathmap_entry, ent);
	strbuf_release(&pathbuf);
	ମୁକ୍ତ(submodule_gitfiles);
	ମୁକ୍ତ(modes);
	ଫେରନ୍ତୁ ret;
}
