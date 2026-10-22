/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "name-hash.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "pathspec.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "symlinks.h"

#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "entry.h"

நிலையான மாறிலி எழுத்து * மாறிலி builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	வெற்று
};

எண்ணிக்கை update_mode {
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

நிலையான முழு needs_worktree_rename(எண்ணிக்கை update_mode mode, எண்ணிக்கை update_mode dst_mode)
{
	திரும்பு !(mode & (INDEX | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#define DUP_BASENAME 1
#define KEEP_TRAILING_SLASH 2

நிலையான வெறுமை internal_prefix_pathspec(கட்டமைப்பு strvec *out,
				     மாறிலி எழுத்து *prefix,
				     மாறிலி எழுத்து **pathspec,
				     முழு மொத்தம், unsigned flags)
{
	முழு prefixlen = prefix ? strlen(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	க்கு (முழு i = 0; i < மொத்தம்; i++) {
		size_t length = strlen(pathspec[i]);
		size_t to_copy = length;
		மாறிலி எழுத்து *maybe_basename;
		எழுத்து *trimmed, *prefixed_path;

		போது (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_தள்ளு(out, prefixed_path);

		விடுவி(prefixed_path);
		விடுவி(trimmed);
	}
}

நிலையான எழுத்து *add_slash(மாறிலி எழுத்து *path)
{
	size_t len = strlen(path);
	எனில் (len && path[len - 1] != '/') {
		எழுத்து *with_slash = xmalloc(st_add(len, 2));
		memcpy(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		திரும்பு with_slash;
	}
	திரும்பு xstrdup(path);
}

#define SUBMODULE_WITH_GITDIR ((மாறிலி எழுத்து *)1)

நிலையான மாறிலி எழுத்து *submodule_gitfile_path(மாறிலி எழுத்து *src, முழு first)
{
	கட்டமைப்பு strbuf submodule_dotgit = STRBUF_INIT;
	மாறிலி எழுத்து *path;

	எனில் (!S_ISGITLINK(the_repository->index->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	எனில் (!is_staging_gitmodules_ok(the_repository->index))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	எனில் (path)
		திரும்பு path;
	திரும்பு SUBMODULE_WITH_GITDIR;
}

நிலையான முழு index_range_of_same_dir(மாறிலி எழுத்து *src, முழு length,
				   முழு *first_p, முழு *last_p)
{
	எழுத்து *src_w_slash = add_slash(src);
	முழு first, last, len_w_slash = length + 1;

	first = index_name_pos(the_repository->index, src_w_slash, len_w_slash);
	எனில் (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	க்கு (last = first; last < the_repository->index->cache_nr; last++) {
		மாறிலி எழுத்து *path = the_repository->index->cache[last]->name;
		எனில் (strncmp(path, src_w_slash, len_w_slash))
			நிறுத்து;
	}

	விடுவி(src_w_slash);
	*first_p = first;
	*last_p = last;
	திரும்பு last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
நிலையான முழு empty_dir_has_sparse_contents(மாறிலி எழுத்து *name)
{
	முழு ret = 0;
	எழுத்து *with_slash = add_slash(name);
	முழு length = strlen(with_slash);

	முழு pos = index_name_pos(the_repository->index, with_slash, length);
	மாறிலி கட்டமைப்பு cache_entry *ce;

	எனில் (pos < 0) {
		pos = -pos - 1;
		எனில் (pos >= the_repository->index->cache_nr)
			செல் free_return;
		ce = the_repository->index->cache[pos];
		எனில் (strncmp(with_slash, ce->name, length))
			செல் free_return;
		எனில் (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	விடுவி(with_slash);
	திரும்பு ret;
}

நிலையான வெறுமை remove_empty_src_dirs(மாறிலி எழுத்து **src_dir, size_t src_dir_nr)
{
	size_t i;
	கட்டமைப்பு strbuf a_src_dir = STRBUF_INIT;

	க்கு (i = 0; i < src_dir_nr; i++) {
		முழு dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		எனில் (index_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

கட்டமைப்பு pathmap_entry {
	கட்டமைப்பு hashmap_entry ent;
	மாறிலி எழுத்து *path;
};

நிலையான முழு pathmap_cmp(மாறிலி வெறுமை *cmp_data UNUSED,
		       மாறிலி கட்டமைப்பு hashmap_entry *a,
		       மாறிலி கட்டமைப்பு hashmap_entry *b,
		       மாறிலி வெறுமை *சாவி UNUSED)
{
	மாறிலி கட்டமைப்பு pathmap_entry *e1 = container_of(a, கட்டமைப்பு pathmap_entry, ent);
	மாறிலி கட்டமைப்பு pathmap_entry *e2 = container_of(b, கட்டமைப்பு pathmap_entry, ent);
	திரும்பு fspathcmp(e1->path, e2->path);
}

முழு cmd_mv(முழு argc,
	   மாறிலி எழுத்து **argv,
	   மாறிலி எழுத்து *prefix,
	   கட்டமைப்பு repository *repo UNUSED)
{
	முழு i, flags, gitmodules_modified = 0;
	முழு verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	கட்டமைப்பு option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT__DRY_RUN(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', வெற்று, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_END(),
	};
	கட்டமைப்பு strvec sources = STRVEC_INIT;
	கட்டமைப்பு strvec dest_paths = STRVEC_INIT;
	கட்டமைப்பு strvec destinations = STRVEC_INIT;
	கட்டமைப்பு strvec submodule_gitfiles_to_free = STRVEC_INIT;
	மாறிலி எழுத்து **submodule_gitfiles;
	எழுத்து *dst_w_slash = வெற்று;
	கட்டமைப்பு strvec src_dir = STRVEC_INIT;
	எண்ணிக்கை update_mode *modes, dst_mode = 0;
	கட்டமைப்பு stat st, dest_st;
	கட்டமைப்பு string_list src_for_dst = STRING_LIST_INIT_DUP;
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	கட்டமைப்பு cache_entry *ce;
	கட்டமைப்பு string_list only_match_skip_worktree = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list dirty_paths = STRING_LIST_INIT_DUP;
	கட்டமைப்பு hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, வெற்று);
	கட்டமைப்பு strbuf pathbuf = STRBUF_INIT;
	முழு ret;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	repo_config(the_repository, git_default_config, வெற்று);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	எனில் (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	எனில் (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	எனில் (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, அளவு(எழுத்து *));

	எனில் (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	இல்லையெனில் எனில் (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} இல்லையெனில் எனில் (!path_in_sparse_checkout(dst_w_slash, the_repository->index) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} இல்லையெனில் எனில் (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} இல்லையெனில் {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		எனில் (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->index))
			dst_mode = SPARSE;
	}

	/* Checking */
	க்கு (i = 0; i < argc; i++) {
		மாறிலி எழுத்து *src = sources.v[i], *dst = destinations.v[i];
		முழு length;
		மாறிலி எழுத்து *bad = வெற்று;
		முழு skip_sparse = 0;

		எனில் (show_only)
			printf(_("Checking rename of '%s' to '%s'\n"), src, dst);

		length = strlen(src);
		எனில் (lstat(src, &st) < 0) {
			முழு pos;
			மாறிலி கட்டமைப்பு cache_entry *ce;

			pos = index_name_pos(the_repository->index, src, length);
			எனில் (pos < 0) {
				எழுத்து *src_w_slash = add_slash(src);
				எனில் (!path_in_sparse_checkout(src_w_slash, the_repository->index) &&
				    empty_dir_has_sparse_contents(src)) {
					விடுவி(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					செல் dir_check;
				}
				விடுவி(src_w_slash);
				/* only error if existence is expected. */
				எனில் (!(modes[i] & SPARSE))
					bad = _("bad source");
				செல் act_on_entry;
			}
			ce = the_repository->index->cache[pos];
			எனில் (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				செல் act_on_entry;
			}
			எனில் (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				செல் act_on_entry;
			}
			/* Check if dst exists in index */
			எனில் (index_name_pos(the_repository->index, dst, strlen(dst)) < 0) {
				modes[i] |= SPARSE;
				செல் act_on_entry;
			}
			எனில் (!force) {
				bad = _("destination exists");
				செல் act_on_entry;
			}
			modes[i] |= SPARSE;
			செல் act_on_entry;
		}
		எனில் (!strncmp(src, dst, length) &&
		    (dst[length] == 0 || dst[length] == '/')) {
			bad = _("can not move directory into itself");
			செல் act_on_entry;
		}
		எனில் (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			செல் act_on_entry;
		}

dir_check:
		எனில் (S_ISDIR(st.st_mode)) {
			கட்டமைப்பு pathmap_entry *entry;
			எழுத்து *dst_with_slash;
			size_t dst_with_slash_len;
			முழு j, n;
			முழு first = index_name_pos(the_repository->index, src, length), last;

			எனில் (first >= 0) {
				மாறிலி எழுத்து *path = submodule_gitfile_path(src, first);
				எனில் (path != SUBMODULE_WITH_GITDIR)
					path = strvec_தள்ளு(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				செல் act_on_entry;
			} இல்லையெனில் எனில் (index_range_of_same_dir(src, length,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				செல் act_on_entry;
			}

			entry = xmalloc(அளவு(*entry));
			entry->path = src;
			hashmap_entry_init(&entry->ent, fspathhash(src));
			hashmap_add(&moved_dirs, &entry->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_தள்ளு(&src_dir, src);

			n = argc + last - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = add_slash(dst);
			dst_with_slash_len = strlen(dst_with_slash);

			க்கு (j = 0; j < last - first; j++) {
				மாறிலி கட்டமைப்பு cache_entry *ce = the_repository->index->cache[first + j];
				மாறிலி எழுத்து *path = ce->name;
				எழுத்து *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + length + 1);

				strvec_தள்ளு(&sources, path);
				strvec_தள்ளு(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : INDEX);
				submodule_gitfiles[argc + j] = வெற்று;

				விடுவி(prefixed_path);
			}

			விடுவி(dst_with_slash);
			argc += last - first;
			செல் act_on_entry;
		}
		எனில் (!(ce = index_file_exists(the_repository->index, src, length, 0))) {
			bad = _("not under version control");
			செல் act_on_entry;
		}
		எனில் (ce_stage(ce)) {
			bad = _("conflicted");
			செல் act_on_entry;
		}
		எனில் (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			எனில் (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				எனில் (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					எனில் (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = வெற்று;
				} இல்லையெனில்
					bad = _("Cannot overwrite");
			}
			செல் act_on_entry;
		}
		எனில் (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			செல் act_on_entry;
		}
		எனில் (is_dir_sep(dst[strlen(dst) - 1])) {
			bad = _("destination directory does not exist");
			செல் act_on_entry;
		}
		எனில் (has_symlink_leading_path(dst, strlen(dst))) {
			bad = _("destination is beyond a symbolic link");
			செல் act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		எனில் (needs_worktree_rename(modes[i], dst_mode)) {
			மாறிலி எழுத்து *slash_ = strrchr(dst, '/');

			எனில் (slash_) {
				கட்டமைப்பு stat dir_st;
				எழுத்து *dst_dir = xstrdup(dst);
				எழுத்து *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				எனில் (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					எனில் (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} இல்லையெனில் எனில் (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				விடுவி(dst_dir);
			}

			எனில் (bad)
				செல் act_on_entry;
		}

		எனில் (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    index_entry_exists(the_repository->index, dst, strlen(dst))) {
			bad = _("destination exists in the index");
			எனில் (force) {
				எனில் (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = வெற்று;
			} இல்லையெனில் {
				செல் act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		எனில் (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		எனில் (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		எனில் (skip_sparse)
			செல் remove_entry;

		string_list_செருகு(&src_for_dst, dst);

act_on_entry:
		எனில் (!bad)
			தொடர்;
		எனில் (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		எனில் (--argc > 0) {
			முழு n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	க்கு (i = 0; i < argc; i++) {
		மாறிலி எழுத்து *slash_pos;

		எனில் (modes[i] & MOVE_VIA_PARENT_DIR)
			தொடர்;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		போது (slash_pos > pathbuf.buf) {
			கட்டமைப்பு pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			எனில் (hashmap_get_entry(&moved_dirs, &needle, ent, வெற்று))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	எனில் (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		எனில் (!ignore_errors) {
			ret = 1;
			செல் out;
		}
	}

	க்கு (i = 0; i < argc; i++) {
		மாறிலி எழுத்து *src = sources.v[i], *dst = destinations.v[i];
		எண்ணிக்கை update_mode mode = modes[i];
		முழு pos;
		முழு sparse_and_dirty = 0;
		கட்டமைப்பு checkout state = CHECKOUT_INIT;
		state.istate = the_repository->index;

		எனில் (force)
			state.force = 1;
		எனில் (show_only || verbose)
			printf(_("Renaming %s to %s\n"), src, dst);
		எனில் (show_only)
			தொடர்;
		எனில் (needs_worktree_rename(mode, dst_mode) &&
		    rename(src, dst) < 0) {
			எனில் (ignore_errors)
				தொடர்;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		எனில் (submodule_gitfiles[i]) {
			எனில் (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			எனில் (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_மரம்_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		எனில் (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			தொடர்;

		pos = index_name_pos(the_repository->index, src, strlen(src));
		எனில் (pos < 0)
			BUG("could not find source in index: '%s'", src);
		எனில் (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->index,
						       the_repository->index->cache[pos],
						       &st,
						       0);
		rename_index_entry_at(the_repository->index, pos, dst);

		எனில் (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			எனில் ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->index)) {
				/* from out-of-cone to in-cone */
				முழு dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				கட்டமைப்பு cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				எனில் (checkout_entry(dst_ce, &state, வெற்று, வெற்று))
					die(_("cannot checkout %s"), dst_ce->name);
			} இல்லையெனில் எனில் ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->index)) {
				/* from in-cone to out-of-cone */
				முழு dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				கட்டமைப்பு cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				எனில் (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} இல்லையெனில் {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					எழுத்து *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					safe_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_empty_src_dirs(src_dir.v, src_dir.nr);

	எனில் (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	எனில் (gitmodules_modified)
		stage_updated_gitmodules(the_repository->index);

	எனில் (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

out:
	strvec_clear(&src_dir);
	விடுவி(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_match_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_to_free);
	hashmap_clear_and_free(&moved_dirs, கட்டமைப்பு pathmap_entry, ent);
	strbuf_release(&pathbuf);
	விடுவி(submodule_gitfiles);
	விடுவி(modes);
	திரும்பு ret;
}
