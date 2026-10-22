/*
 * "git mv" builtin command
 *
 * Copyright (C) 2006 Johannes Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "name-hash.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "pathspec.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "symlinks.h"

#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "entry.h"

સ્થિર અચલ અક્ષર * અચલ builtin_mv_usage[] = {
	N_("git mv [-v] [-f] [-n] [-k] <source> <destination>"),
	N_("git mv [-v] [-f] [-n] [-k] <source>... <destination-directory>"),
	ખાલી
};

ગણના update_mode {
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

સ્થિર પૂર્ણાંક needs_worktree_rename(ગણના update_mode mode, ગણના update_mode dst_mode)
{
	પાછા !(mode & (INDEX | SPARSE | SKIP_WORKTREE_DIR)) &&
	       !(dst_mode & (SKIP_WORKTREE_DIR | SPARSE));
}

#define DUP_BASENAME 1
#define KEEP_TRAILING_SLASH 2

સ્થિર શૂન્ય internal_prefix_pathspec(સંરચના strvec *out,
				     અચલ અક્ષર *prefix,
				     અચલ અક્ષર **pathspec,
				     પૂર્ણાંક ગણતરી, unsigned flags)
{
	પૂર્ણાંક prefixlen = prefix ? strlen(prefix) : 0;

	/* Create an intermediate copy of the pathspec based on the flags */
	માટે (પૂર્ણાંક i = 0; i < ગણતરી; i++) {
		size_t length = strlen(pathspec[i]);
		size_t to_copy = length;
		અચલ અક્ષર *maybe_basename;
		અક્ષર *trimmed, *prefixed_path;

		જ્યારે (!(flags & KEEP_TRAILING_SLASH) &&
		       to_copy > 0 && is_dir_sep(pathspec[i][to_copy - 1]))
			to_copy--;

		trimmed = xmemdupz(pathspec[i], to_copy);
		maybe_basename = (flags & DUP_BASENAME) ? basename(trimmed) : trimmed;
		prefixed_path = prefix_path(the_repository, prefix, prefixlen, maybe_basename);
		strvec_ધકેલો(out, prefixed_path);

		મુક્ત(prefixed_path);
		મુક્ત(trimmed);
	}
}

સ્થિર અક્ષર *add_slash(અચલ અક્ષર *path)
{
	size_t len = strlen(path);
	જો (len && path[len - 1] != '/') {
		અક્ષર *with_slash = xmalloc(st_add(len, 2));
		memcpy(with_slash, path, len);
		with_slash[len++] = '/';
		with_slash[len] = 0;
		પાછા with_slash;
	}
	પાછા xstrdup(path);
}

#define SUBMODULE_WITH_GITDIR ((અચલ અક્ષર *)1)

સ્થિર અચલ અક્ષર *submodule_gitfile_path(અચલ અક્ષર *src, પૂર્ણાંક first)
{
	સંરચના strbuf submodule_dotgit = STRBUF_INIT;
	અચલ અક્ષર *path;

	જો (!S_ISGITLINK(the_repository->index->cache[first]->ce_mode))
		die(_("Directory %s is in index and no submodule?"), src);
	જો (!is_staging_gitmodules_ok(the_repository->index))
		die(_("Please stage your changes to .gitmodules or stash them to proceed"));

	strbuf_addf(&submodule_dotgit, "%s/.git", src);

	path = read_gitfile(submodule_dotgit.buf);
	strbuf_release(&submodule_dotgit);
	જો (path)
		પાછા path;
	પાછા SUBMODULE_WITH_GITDIR;
}

સ્થિર પૂર્ણાંક index_range_of_same_dir(અચલ અક્ષર *src, પૂર્ણાંક length,
				   પૂર્ણાંક *first_p, પૂર્ણાંક *last_p)
{
	અક્ષર *src_w_slash = add_slash(src);
	પૂર્ણાંક first, last, len_w_slash = length + 1;

	first = index_name_pos(the_repository->index, src_w_slash, len_w_slash);
	જો (first >= 0)
		die(_("%.*s is in index"), len_w_slash, src_w_slash);

	first = -1 - first;
	માટે (last = first; last < the_repository->index->cache_nr; last++) {
		અચલ અક્ષર *path = the_repository->index->cache[last]->name;
		જો (strncmp(path, src_w_slash, len_w_slash))
			તોડો;
	}

	મુક્ત(src_w_slash);
	*first_p = first;
	*last_p = last;
	પાછા last - first;
}

/*
 * Given the path of a directory that does not exist on-disk, check whether the
 * directory contains any entries in the index with the SKIP_WORKTREE flag
 * enabled.
 * Return 1 if such index entries exist.
 * Return 0 otherwise.
 */
સ્થિર પૂર્ણાંક empty_dir_has_sparse_contents(અચલ અક્ષર *name)
{
	પૂર્ણાંક ret = 0;
	અક્ષર *with_slash = add_slash(name);
	પૂર્ણાંક length = strlen(with_slash);

	પૂર્ણાંક pos = index_name_pos(the_repository->index, with_slash, length);
	અચલ સંરચના cache_entry *ce;

	જો (pos < 0) {
		pos = -pos - 1;
		જો (pos >= the_repository->index->cache_nr)
			જાઓ free_return;
		ce = the_repository->index->cache[pos];
		જો (strncmp(with_slash, ce->name, length))
			જાઓ free_return;
		જો (ce_skip_worktree(ce))
			ret = 1;
	}

free_return:
	મુક્ત(with_slash);
	પાછા ret;
}

સ્થિર શૂન્ય remove_empty_src_dirs(અચલ અક્ષર **src_dir, size_t src_dir_nr)
{
	size_t i;
	સંરચના strbuf a_src_dir = STRBUF_INIT;

	માટે (i = 0; i < src_dir_nr; i++) {
		પૂર્ણાંક dummy;
		strbuf_addstr(&a_src_dir, src_dir[i]);
		/*
		 * if entries under a_src_dir are all moved away,
		 * recursively remove a_src_dir to cleanup
		 */
		જો (index_range_of_same_dir(a_src_dir.buf, a_src_dir.len,
					    &dummy, &dummy) < 1) {
			remove_dir_recursively(&a_src_dir, 0);
		}
		strbuf_reset(&a_src_dir);
	}

	strbuf_release(&a_src_dir);
}

સંરચના pathmap_entry {
	સંરચના hashmap_entry ent;
	અચલ અક્ષર *path;
};

સ્થિર પૂર્ણાંક pathmap_cmp(અચલ શૂન્ય *cmp_data UNUSED,
		       અચલ સંરચના hashmap_entry *a,
		       અચલ સંરચના hashmap_entry *b,
		       અચલ શૂન્ય *ચાવી UNUSED)
{
	અચલ સંરચના pathmap_entry *e1 = container_of(a, સંરચના pathmap_entry, ent);
	અચલ સંરચના pathmap_entry *e2 = container_of(b, સંરચના pathmap_entry, ent);
	પાછા fspathcmp(e1->path, e2->path);
}

પૂર્ણાંક cmd_mv(પૂર્ણાંક argc,
	   અચલ અક્ષર **argv,
	   અચલ અક્ષર *prefix,
	   સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i, flags, gitmodules_modified = 0;
	પૂર્ણાંક verbose = 0, show_only = 0, force = 0, ignore_errors = 0, ignore_sparse = 0;
	સંરચના option builtin_mv_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT__DRY_RUN(&show_only, N_("dry run")),
		OPT__FORCE(&force, N_("force move/rename even if target exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('k', ખાલી, &ignore_errors, N_("skip move/rename errors")),
		OPT_BOOL(0, "sparse", &ignore_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
		OPT_END(),
	};
	સંરચના strvec sources = STRVEC_INIT;
	સંરચના strvec dest_paths = STRVEC_INIT;
	સંરચના strvec destinations = STRVEC_INIT;
	સંરચના strvec submodule_gitfiles_to_free = STRVEC_INIT;
	અચલ અક્ષર **submodule_gitfiles;
	અક્ષર *dst_w_slash = ખાલી;
	સંરચના strvec src_dir = STRVEC_INIT;
	ગણના update_mode *modes, dst_mode = 0;
	સંરચના stat st, dest_st;
	સંરચના string_list src_for_dst = STRING_LIST_INIT_DUP;
	સંરચના lock_file lock_file = LOCK_INIT;
	સંરચના cache_entry *ce;
	સંરચના string_list only_match_skip_worktree = STRING_LIST_INIT_DUP;
	સંરચના string_list dirty_paths = STRING_LIST_INIT_DUP;
	સંરચના hashmap moved_dirs = HASHMAP_INIT(pathmap_cmp, ખાલી);
	સંરચના strbuf pathbuf = STRBUF_INIT;
	પૂર્ણાંક ret;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	repo_config(the_repository, git_default_config, ખાલી);

	argc = parse_options(argc, argv, prefix, builtin_mv_options,
			     builtin_mv_usage, 0);
	જો (--argc < 1)
		usage_with_options(builtin_mv_usage, builtin_mv_options);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	જો (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	internal_prefix_pathspec(&sources, prefix, argv, argc, 0);
	CALLOC_ARRAY(modes, argc);

	/*
	 * Keep trailing slash, needed to let
	 * "git mv file no-such-dir/" error out, except in the case
	 * "git mv directory no-such-dir/".
	 */
	flags = KEEP_TRAILING_SLASH;
	જો (argc == 1 && is_directory(argv[0]) && !is_directory(argv[1]))
		flags = 0;
	internal_prefix_pathspec(&dest_paths, prefix, argv + argc, 1, flags);
	dst_w_slash = add_slash(dest_paths.v[0]);
	submodule_gitfiles = xcalloc(argc, કદ(અક્ષર *));

	જો (dest_paths.v[0][0] == '\0')
		/* special case: "." was normalized to "" */
		internal_prefix_pathspec(&destinations, dest_paths.v[0], argv, argc, DUP_BASENAME);
	નહીંતર જો (!lstat(dest_paths.v[0], &st) && S_ISDIR(st.st_mode)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
	} નહીંતર જો (!path_in_sparse_checkout(dst_w_slash, the_repository->index) &&
		   empty_dir_has_sparse_contents(dst_w_slash)) {
		internal_prefix_pathspec(&destinations, dst_w_slash, argv, argc, DUP_BASENAME);
		dst_mode = SKIP_WORKTREE_DIR;
	} નહીંતર જો (argc != 1) {
		die(_("destination '%s' is not a directory"), dest_paths.v[0]);
	} નહીંતર {
		strvec_pushv(&destinations, dest_paths.v);

		/*
		 * <destination> is a file outside of sparse-checkout
		 * cone. Insist on cone mode here for backward
		 * compatibility. We don't want dst_mode to be assigned
		 * for a file when the repo is using no-cone mode (which
		 * is deprecated at this point) sparse-checkout. As
		 * SPARSE here is only considering cone-mode situation.
		 */
		જો (!path_in_cone_mode_sparse_checkout(destinations.v[0], the_repository->index))
			dst_mode = SPARSE;
	}

	/* Checking */
	માટે (i = 0; i < argc; i++) {
		અચલ અક્ષર *src = sources.v[i], *dst = destinations.v[i];
		પૂર્ણાંક length;
		અચલ અક્ષર *bad = ખાલી;
		પૂર્ણાંક skip_sparse = 0;

		જો (show_only)
			printf(_("Checking rename of '%s' to '%s'\n"), src, dst);

		length = strlen(src);
		જો (lstat(src, &st) < 0) {
			પૂર્ણાંક pos;
			અચલ સંરચના cache_entry *ce;

			pos = index_name_pos(the_repository->index, src, length);
			જો (pos < 0) {
				અક્ષર *src_w_slash = add_slash(src);
				જો (!path_in_sparse_checkout(src_w_slash, the_repository->index) &&
				    empty_dir_has_sparse_contents(src)) {
					મુક્ત(src_w_slash);
					modes[i] |= SKIP_WORKTREE_DIR;
					જાઓ dir_check;
				}
				મુક્ત(src_w_slash);
				/* only error if existence is expected. */
				જો (!(modes[i] & SPARSE))
					bad = _("bad source");
				જાઓ act_on_entry;
			}
			ce = the_repository->index->cache[pos];
			જો (!ce_skip_worktree(ce)) {
				bad = _("bad source");
				જાઓ act_on_entry;
			}
			જો (!ignore_sparse) {
				string_list_append(&only_match_skip_worktree, src);
				જાઓ act_on_entry;
			}
			/* Check if dst exists in index */
			જો (index_name_pos(the_repository->index, dst, strlen(dst)) < 0) {
				modes[i] |= SPARSE;
				જાઓ act_on_entry;
			}
			જો (!force) {
				bad = _("destination exists");
				જાઓ act_on_entry;
			}
			modes[i] |= SPARSE;
			જાઓ act_on_entry;
		}
		જો (!strncmp(src, dst, length) &&
		    (dst[length] == 0 || dst[length] == '/')) {
			bad = _("can not move directory into itself");
			જાઓ act_on_entry;
		}
		જો (S_ISDIR(st.st_mode)
		    && lstat(dst, &dest_st) == 0) {
			bad = _("destination already exists");
			જાઓ act_on_entry;
		}

dir_check:
		જો (S_ISDIR(st.st_mode)) {
			સંરચના pathmap_entry *entry;
			અક્ષર *dst_with_slash;
			size_t dst_with_slash_len;
			પૂર્ણાંક j, n;
			પૂર્ણાંક first = index_name_pos(the_repository->index, src, length), last;

			જો (first >= 0) {
				અચલ અક્ષર *path = submodule_gitfile_path(src, first);
				જો (path != SUBMODULE_WITH_GITDIR)
					path = strvec_ધકેલો(&submodule_gitfiles_to_free, path);
				submodule_gitfiles[i] = path;
				જાઓ act_on_entry;
			} નહીંતર જો (index_range_of_same_dir(src, length,
							   &first, &last) < 1) {
				bad = _("source directory is empty");
				જાઓ act_on_entry;
			}

			entry = xmalloc(કદ(*entry));
			entry->path = src;
			hashmap_entry_init(&entry->ent, fspathhash(src));
			hashmap_add(&moved_dirs, &entry->ent);

			/* last - first >= 1 */
			modes[i] |= WORKING_DIRECTORY;

			strvec_ધકેલો(&src_dir, src);

			n = argc + last - first;
			REALLOC_ARRAY(modes, n);
			REALLOC_ARRAY(submodule_gitfiles, n);

			dst_with_slash = add_slash(dst);
			dst_with_slash_len = strlen(dst_with_slash);

			માટે (j = 0; j < last - first; j++) {
				અચલ સંરચના cache_entry *ce = the_repository->index->cache[first + j];
				અચલ અક્ષર *path = ce->name;
				અક્ષર *prefixed_path = prefix_path(the_repository, dst_with_slash,
								  dst_with_slash_len, path + length + 1);

				strvec_ધકેલો(&sources, path);
				strvec_ધકેલો(&destinations, prefixed_path);

				modes[argc + j] = MOVE_VIA_PARENT_DIR | (ce_skip_worktree(ce) ? SPARSE : INDEX);
				submodule_gitfiles[argc + j] = ખાલી;

				મુક્ત(prefixed_path);
			}

			મુક્ત(dst_with_slash);
			argc += last - first;
			જાઓ act_on_entry;
		}
		જો (!(ce = index_file_exists(the_repository->index, src, length, 0))) {
			bad = _("not under version control");
			જાઓ act_on_entry;
		}
		જો (ce_stage(ce)) {
			bad = _("conflicted");
			જાઓ act_on_entry;
		}
		જો (lstat(dst, &st) == 0 &&
		    (!repo_ignore_case(the_repository) || strcasecmp(src, dst))) {
			bad = _("destination exists");
			જો (force) {
				/*
				 * only files can overwrite each other:
				 * check both source and destination
				 */
				જો (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
					જો (verbose)
						warning(_("overwriting '%s'"), dst);
					bad = ખાલી;
				} નહીંતર
					bad = _("Cannot overwrite");
			}
			જાઓ act_on_entry;
		}
		જો (string_list_has_string(&src_for_dst, dst)) {
			bad = _("multiple sources for the same target");
			જાઓ act_on_entry;
		}
		જો (is_dir_sep(dst[strlen(dst) - 1])) {
			bad = _("destination directory does not exist");
			જાઓ act_on_entry;
		}
		જો (has_symlink_leading_path(dst, strlen(dst))) {
			bad = _("destination is beyond a symbolic link");
			જાઓ act_on_entry;
		}

		/*
		 * If we are going to move SRC to DST on disk, DST's leading
		 * directories must already exist.
		 */
		જો (needs_worktree_rename(modes[i], dst_mode)) {
			અચલ અક્ષર *slash_ = strrchr(dst, '/');

			જો (slash_) {
				સંરચના stat dir_st;
				અક્ષર *dst_dir = xstrdup(dst);
				અક્ષર *slash = &dst_dir[slash_ - dst];

				*slash = '\0';
				જો (lstat(dst_dir, &dir_st) < 0) {
					/*
					 * other errors fall through to rename(),
					 * which reports them
					 */
					જો (errno == ENOENT || errno == ENOTDIR)
						bad = _("destination directory does not exist");
				} નહીંતર જો (!S_ISDIR(dir_st.st_mode)) {
					bad = _("destination is not a directory");
				}

				મુક્ત(dst_dir);
			}

			જો (bad)
				જાઓ act_on_entry;
		}

		જો (ignore_sparse &&
		    (dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
		    index_entry_exists(the_repository->index, dst, strlen(dst))) {
			bad = _("destination exists in the index");
			જો (force) {
				જો (verbose)
					warning(_("overwriting '%s'"), dst);
				bad = ખાલી;
			} નહીંતર {
				જાઓ act_on_entry;
			}
		}
		/*
		 * We check if the paths are in the sparse-checkout
		 * definition as a very final check, since that
		 * allows us to point the user to the --sparse
		 * option as a way to have a successful run.
		 */
		જો (!ignore_sparse &&
		    !path_in_sparse_checkout(src, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, src);
			skip_sparse = 1;
		}
		જો (!ignore_sparse &&
		    !path_in_sparse_checkout(dst, the_repository->index)) {
			string_list_append(&only_match_skip_worktree, dst);
			skip_sparse = 1;
		}

		જો (skip_sparse)
			જાઓ remove_entry;

		string_list_ઉમેરો(&src_for_dst, dst);

act_on_entry:
		જો (!bad)
			ચાલુ;
		જો (!ignore_errors)
			die(_("%s, source=%s, destination=%s"),
			     bad, src, dst);
remove_entry:
		જો (--argc > 0) {
			પૂર્ણાંક n = argc - i;
			strvec_remove(&sources, i);
			strvec_remove(&destinations, i);
			MOVE_ARRAY(modes + i, modes + i + 1, n);
			MOVE_ARRAY(submodule_gitfiles + i,
				   submodule_gitfiles + i + 1, n);
			i--;
		}
	}

	માટે (i = 0; i < argc; i++) {
		અચલ અક્ષર *slash_pos;

		જો (modes[i] & MOVE_VIA_PARENT_DIR)
			ચાલુ;

		strbuf_reset(&pathbuf);
		strbuf_addstr(&pathbuf, sources.v[i]);

		slash_pos = strrchr(pathbuf.buf, '/');
		જ્યારે (slash_pos > pathbuf.buf) {
			સંરચના pathmap_entry needle;

			strbuf_setlen(&pathbuf, slash_pos - pathbuf.buf);

			needle.path = pathbuf.buf;
			hashmap_entry_init(&needle.ent, fspathhash(pathbuf.buf));

			જો (hashmap_get_entry(&moved_dirs, &needle, ent, ખાલી))
				die(_("cannot move both '%s' and its parent directory '%s'"),
				    sources.v[i], pathbuf.buf);

			slash_pos = strrchr(pathbuf.buf, '/');
		}
	}

	જો (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		જો (!ignore_errors) {
			ret = 1;
			જાઓ out;
		}
	}

	માટે (i = 0; i < argc; i++) {
		અચલ અક્ષર *src = sources.v[i], *dst = destinations.v[i];
		ગણના update_mode mode = modes[i];
		પૂર્ણાંક pos;
		પૂર્ણાંક sparse_and_dirty = 0;
		સંરચના checkout state = CHECKOUT_INIT;
		state.istate = the_repository->index;

		જો (force)
			state.force = 1;
		જો (show_only || verbose)
			printf(_("Renaming %s to %s\n"), src, dst);
		જો (show_only)
			ચાલુ;
		જો (needs_worktree_rename(mode, dst_mode) &&
		    rename(src, dst) < 0) {
			જો (ignore_errors)
				ચાલુ;
			die_errno(_("renaming '%s' to '%s' failed"), src, dst);
		}
		જો (submodule_gitfiles[i]) {
			જો (!update_path_in_gitmodules(src, dst))
				gitmodules_modified = 1;
			જો (submodule_gitfiles[i] != SUBMODULE_WITH_GITDIR)
				connect_work_વૃક્ષ_and_git_dir(dst,
							      submodule_gitfiles[i],
							      1);
		}

		જો (mode & (WORKING_DIRECTORY | SKIP_WORKTREE_DIR))
			ચાલુ;

		pos = index_name_pos(the_repository->index, src, strlen(src));
		જો (pos < 0)
			BUG("could not find source in index: '%s'", src);
		જો (!(mode & SPARSE) && !lstat(src, &st))
			sparse_and_dirty = ie_modified(the_repository->index,
						       the_repository->index->cache[pos],
						       &st,
						       0);
		rename_index_entry_at(the_repository->index, pos, dst);

		જો (ignore_sparse &&
		    cfg->apply_sparse_checkout &&
		    cfg->core_sparse_checkout_cone) {
			/*
			 * NEEDSWORK: we are *not* paying attention to
			 * "out-to-out" move (<source> is out-of-cone and
			 * <destination> is out-of-cone) at this point. It
			 * should be added in a future patch.
			 */
			જો ((mode & SPARSE) &&
			    path_in_sparse_checkout(dst, the_repository->index)) {
				/* from out-of-cone to in-cone */
				પૂર્ણાંક dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				સંરચના cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				dst_ce->ce_flags &= ~CE_SKIP_WORKTREE;

				જો (checkout_entry(dst_ce, &state, ખાલી, ખાલી))
					die(_("cannot checkout %s"), dst_ce->name);
			} નહીંતર જો ((dst_mode & (SKIP_WORKTREE_DIR | SPARSE)) &&
				   !(mode & SPARSE) &&
				   !path_in_sparse_checkout(dst, the_repository->index)) {
				/* from in-cone to out-of-cone */
				પૂર્ણાંક dst_pos = index_name_pos(the_repository->index, dst,
							     strlen(dst));
				સંરચના cache_entry *dst_ce = the_repository->index->cache[dst_pos];

				/*
				 * if src is clean, it will suffice to remove it
				 */
				જો (!sparse_and_dirty) {
					dst_ce->ce_flags |= CE_SKIP_WORKTREE;
					unlink_or_warn(src);
				} નહીંતર {
					/*
					 * if src is dirty, move it to the
					 * destination and create leading
					 * dirs if necessary
					 */
					અક્ષર *dst_dup = xstrdup(dst);
					string_list_append(&dirty_paths, dst);
					safe_create_leading_directories(the_repository, dst_dup);
					FREE_AND_NULL(dst_dup);
					rename(src, dst);
				}
			}
		}
	}

	remove_empty_src_dirs(src_dir.v, src_dir.nr);

	જો (dirty_paths.nr)
		advise_on_moving_dirty_path(&dirty_paths);

	જો (gitmodules_modified)
		stage_updated_gitmodules(the_repository->index);

	જો (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ret = 0;

out:
	strvec_clear(&src_dir);
	મુક્ત(dst_w_slash);
	string_list_clear(&src_for_dst, 0);
	string_list_clear(&dirty_paths, 0);
	string_list_clear(&only_match_skip_worktree, 0);
	strvec_clear(&sources);
	strvec_clear(&dest_paths);
	strvec_clear(&destinations);
	strvec_clear(&submodule_gitfiles_to_free);
	hashmap_clear_and_free(&moved_dirs, સંરચના pathmap_entry, ent);
	strbuf_release(&pathbuf);
	મુક્ત(submodule_gitfiles);
	મુક્ત(modes);
	પાછા ret;
}
