/*
 * Builtin "git clone"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *		 2008 Daniel Barkalow <barkalow@iabervon.org>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 *
 * Clone a repository into a different directory that does not yet exist.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"

#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "copy.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "refspec.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "transport.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "dir-iterator.h"
#ಅನ್ತರ್ಭಾವ "iterator.h"
#ಅನ್ತರ್ಭಾವ "sigchain.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "connected.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "pkt-line.h"
#ಅನ್ತರ್ಭಾವ "list-objects-filter-options.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "bundle.h"
#ಅನ್ತರ್ಭಾವ "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

ಸಂರಚನಾ clone_opts {
	ಪೂರ್ಣಾಙ್ಕ wants_head;
	ಪೂರ್ಣಾಙ್ಕ detach;
};
#define CLONE_OPTS_INIT { \
	.wants_head = 1 /* default enabled */ \
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_local = -1, option_no_hardlinks, option_shared;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_tags = 1; /* default enabled */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_shallow_submodules;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ config_reject_shallow = -1;    /* unspecified */
ಸ್ಥಿರ ವರ್ಣ *remote_name = ರಿಕ್ತ;
ಸ್ಥಿರ ವರ್ಣ *option_branch = ರಿಕ್ತ;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_verbosity;
ಸ್ಥಿರ ಸಂರಚನಾ string_list option_required_reference = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಸಂರಚನಾ string_list option_optional_reference = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ max_jobs = -1;
ಸ್ಥಿರ ಸಂರಚನಾ string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ config_filter_submodules = -1;    /* unspecified */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_remote_submodules;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ recurse_submodules_cb(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				 ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಯದಿ (unset)
		string_list_clear((ಸಂರಚನಾ string_list *)opt->ಮೌಲ್ಯ, 0);
	ಅನ್ಯಥಾ ಯದಿ (arg)
		string_list_append((ಸಂರಚನಾ string_list *)opt->ಮೌಲ್ಯ, arg);
	ಅನ್ಯಥಾ
		string_list_append((ಸಂರಚನಾ string_list *)opt->ಮೌಲ್ಯ,
				   (ಧ್ರುವ ವರ್ಣ *)opt->defval);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_repo_path_1(ಸಂರಚನಾ strbuf *path, ಪೂರ್ಣಾಙ್ಕ *is_bundle)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *suffix[] = { "/.git", "", ".git/.git", ".git" };
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *bundle_suffix[] = { ".bundle", "" };
	size_t baselen = path->len;
	ಸಂರಚನಾ stat st;
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < ARRAY_SIZE(suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, suffix[i]);
		ಯದಿ (stat(path->buf, &st))
			ಅನುವರ್ತ;
		ಯದಿ (S_ISDIR(st.st_mode) && is_git_directory(path->buf)) {
			*is_bundle = 0;
			ಪ್ರತ್ಯಾಗಮ path->buf;
		} ಅನ್ಯಥಾ ಯದಿ (S_ISREG(st.st_mode) && st.st_size > 8) {
			/* Is it a "gitfile"? */
			ವರ್ಣ signature[8];
			ಧ್ರುವ ವರ್ಣ *dst;
			ಪೂರ್ಣಾಙ್ಕ len, fd = open(path->buf, O_RDONLY);
			ಯದಿ (fd < 0)
				ಅನುವರ್ತ;
			len = read_in_full(fd, signature, 8);
			close(fd);
			ಯದಿ (len != 8 || strncmp(signature, "gitdir: ", 8))
				ಅನುವರ್ತ;
			dst = read_gitfile(path->buf);
			ಯದಿ (dst) {
				*is_bundle = 0;
				ಪ್ರತ್ಯಾಗಮ dst;
			}
		}
	}

	ಪ್ರತಿ (i = 0; i < ARRAY_SIZE(bundle_suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, bundle_suffix[i]);
		ಯದಿ (!stat(path->buf, &st) && S_ISREG(st.st_mode)) {
			*is_bundle = 1;
			ಪ್ರತ್ಯಾಗಮ path->buf;
		}
	}

	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ವರ್ಣ *get_repo_path(ಧ್ರುವ ವರ್ಣ *repo, ಪೂರ್ಣಾಙ್ಕ *is_bundle)
{
	ಸಂರಚನಾ strbuf path = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *raw;
	ವರ್ಣ *canon;

	strbuf_addstr(&path, repo);
	raw = get_repo_path_1(&path, is_bundle);
	canon = raw ? absolute_pathdup(raw) : ರಿಕ್ತ;
	strbuf_release(&path);
	ಪ್ರತ್ಯಾಗಮ canon;
}

ಸಂರಚನಾ collect_alternates_data {
	ಸಂರಚನಾ strvec *alternates;
	bool required;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ collect_one_alternate(ಸಂರಚನಾ string_list_item *item, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ collect_alternates_data *data = cb_data;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ವರ್ಣ *ref_git = compute_alternate_path(item->string, &err);

	ಯದಿ (!ref_git) {
		ಯದಿ (data->required)
			die("%s", err.buf);
		ಅನ್ಯಥಾ
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				item->string, err.buf);
	} ಅನ್ಯಥಾ {
		strvec_pushf(data->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	ಮುಕ್ತ(ref_git);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_alternates(ಸಂರಚನಾ strvec *alternates, ಧ್ರುವ ವರ್ಣ *src_repo)
{
	/*
	 * Read from the source objects/info/alternates file
	 * and copy the entries to corresponding file in the
	 * destination repository with add_to_alternates_file().
	 * Both src and dst have "$path/objects/info/alternates".
	 *
	 * Instead of copying bit-for-bit from the original,
	 * we need to append to existing one so that the already
	 * created entry via "clone -s" is not lost, and also
	 * to turn entries with paths relative to the original
	 * absolute, so that they can be used in the new repository.
	 */
	FILE *in;
	ಸಂರಚನಾ strbuf path = STRBUF_INIT;
	ಸಂರಚನಾ strbuf ಸಾಲು = STRBUF_INIT;

	strbuf_addf(&path, "%s/objects/info/alternates", src_repo);

	in = fopen(path.buf, "r");
	ಯದಿ (!in) {
		ಯದಿ (errno == ENOENT)
			ಗಚ್ಛ out;
		die_errno("could not read alternates file '%s'", path.buf);
	}

	ಯಾವತ್ (strbuf_getline(&ಸಾಲು, in) != EOF) {
		ವರ್ಣ *abs_path;
		ಯದಿ (!ಸಾಲು.len || ಸಾಲು.buf[0] == '#')
			ಅನುವರ್ತ;
		ಯದಿ (is_absolute_path(ಸಾಲು.buf)) {
			strvec_ತಳ್ಳು(alternates, ಸಾಲು.buf);
			ಅನುವರ್ತ;
		}
		abs_path = mkpathdup("%s/objects/%s", src_repo, ಸಾಲು.buf);
		ಯದಿ (!normalize_path_copy(abs_path, abs_path))
			strvec_ತಳ್ಳು(alternates, abs_path);
		ಅನ್ಯಥಾ
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, ಸಾಲು.buf);
		ಮುಕ್ತ(abs_path);
	}

out:
	strbuf_release(&path);
	strbuf_release(&ಸಾಲು);
	ಯದಿ (in)
		fclose(in);
}

ಸ್ಥಿರ ಶೂನ್ಯ collect_alternates(ಸಂರಚನಾ strvec *alternates,
			       ಧ್ರುವ ವರ್ಣ *local_source_repo)
{
	ಯದಿ (option_required_reference.nr || option_optional_reference.nr) {
		ಸಂರಚನಾ collect_alternates_data data = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &data);
		data.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &data);
	}

	ಯದಿ (local_source_repo) {
		ಸಂರಚನಾ strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_source_repo);
		ಯದಿ (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		ಅನ್ಯಥಾ
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ mkdir_if_missing(ಧ್ರುವ ವರ್ಣ *pathname, mode_t mode)
{
	ಸಂರಚನಾ stat st;

	ಯದಿ (!mkdir(pathname, mode))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	ಅನ್ಯಥಾ ಯದಿ (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	ಅನ್ಯಥಾ ಯದಿ (!S_ISDIR(st.st_mode))
		die(_("%s exists and is not a directory"), pathname);
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_or_link_directory(ಸಂರಚನಾ strbuf *src, ಸಂರಚನಾ strbuf *dest,
				   ಧ್ರುವ ವರ್ಣ *src_repo)
{
	ಪೂರ್ಣಾಙ್ಕ src_len, dest_len;
	ಸಂರಚನಾ dir_iterator *iter;
	ಪೂರ್ಣಾಙ್ಕ iter_status;

	/*
	 * Refuse copying directories by default which aren't owned by us. The
	 * code that performs either the copying or hardlinking is not prepared
	 * to handle various edge cases where an adversary may for example
	 * racily swap out files for symlinks. This can cause us to
	 * inadvertently use the wrong source file.
	 *
	 * Furthermore, even if we were prepared to handle such races safely,
	 * creating hardlinks across user boundaries is an inherently unsafe
	 * operation as the hardlinked files can be rewritten at will by the
	 * potentially-untrusted user. We thus refuse to do so by default.
	 */
	die_upon_dubious_ownership(ರಿಕ್ತ, ರಿಕ್ತ, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	ಯದಿ (!iter) {
		ಯದಿ (errno == ENOTDIR) {
			ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
			ಸಂರಚನಾ stat st;

			ಯದಿ (!lstat(src->buf, &st) && S_ISLNK(st.st_mode))
				die(_("'%s' is a symlink, refusing to clone with --local"),
				    src->buf);
			errno = saved_errno;
		}
		die_errno(_("failed to start iterator over '%s'"), src->buf);
	}

	strbuf_addch(src, '/');
	src_len = src->len;
	strbuf_addch(dest, '/');
	dest_len = dest->len;

	ಯಾವತ್ ((iter_status = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_path);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_path);

		ಯದಿ (S_ISLNK(iter->st.st_mode))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_path);

		ಯದಿ (S_ISDIR(iter->st.st_mode)) {
			mkdir_if_missing(dest->buf, 0777);
			ಅನುವರ್ತ;
		}

		/* Alternates were already handled earlier. */
		ಯದಿ (!fspathcmp(iter->relative_path, "info/alternates"))
			ಅನುವರ್ತ;

		ಯದಿ (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		ಯದಿ (!option_no_hardlinks) {
			ಯದಿ (!link(src->buf, dest->buf)) {
				ಸಂರಚನಾ stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				ಯದಿ (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				ಯದಿ (st.st_mode != iter->st.st_mode ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_size != iter->st.st_size ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				ಅನುವರ್ತ;
			}
			ಯದಿ (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		ಯದಿ (copy_file_with_time(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	ಯದಿ (iter_status != ITER_DONE) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

ಸ್ಥಿರ ಶೂನ್ಯ clone_local(ಧ್ರುವ ವರ್ಣ *src_repo, ಧ್ರುವ ವರ್ಣ *dest_repo)
{
	ಯದಿ (!option_shared) {
		ಸಂರಚನಾ strbuf src = STRBUF_INIT;
		ಸಂರಚನಾ strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	ಯದಿ (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *junk_work_ವೃಕ್ಷ;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ junk_work_ವೃಕ್ಷ_flags;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *junk_git_dir;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ junk_git_dir_flags;
ಸ್ಥಿರ ಗಣನಾ {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_mode = JUNK_LEAVE_NONE;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ junk_leave_repo_ಸಂದೇಶ[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

ಸ್ಥಿರ ಶೂನ್ಯ remove_junk(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	ವಿಕಲ್ಪ (junk_mode) {
	ಪಕ್ಷ JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_ಸಂದೇಶ));
		/* fall-through */
	ಪಕ್ಷ JUNK_LEAVE_ALL:
		ಪ್ರತ್ಯಾಗಮ;
	ಸ್ವಭಾವ:
		/* proceed to removal */
		ಭಙ್ಗ;
	}

	ಯದಿ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	ಯದಿ (junk_work_ವೃಕ್ಷ) {
		strbuf_addstr(&sb, junk_work_ವೃಕ್ಷ);
		remove_dir_recursively(&sb, junk_work_ವೃಕ್ಷ_flags);
	}
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_junk_on_signal(ಪೂರ್ಣಾಙ್ಕ signo)
{
	remove_junk();
	sigchain_ತೆಗೆ(signo);
	ಉತ್ಥಾಪಯ(signo);
}

ಸ್ಥಿರ ಸಂರಚನಾ ref *find_remote_branch(ಧ್ರುವ ಸಂರಚನಾ ref *refs, ಧ್ರುವ ವರ್ಣ *branch)
{
	ಸಂರಚನಾ ref *ref;
	ಸಂರಚನಾ strbuf head = STRBUF_INIT;
	strbuf_addstr(&head, "refs/heads/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	ಯದಿ (ref)
		ಪ್ರತ್ಯಾಗಮ ref;

	strbuf_addstr(&head, "refs/tags/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	ಪ್ರತ್ಯಾಗಮ ref;
}

ಸ್ಥಿರ ಸಂರಚನಾ ref *wanted_peer_refs(ಸಂರಚನಾ clone_opts *opts,
				    ಧ್ರುವ ಸಂರಚನಾ ref *refs,
				    ಸಂರಚನಾ refspec *refspec)
{
	ಸಂರಚನಾ ref *local_refs = ರಿಕ್ತ;
	ಸಂರಚನಾ ref **tail = &local_refs;
	ಸಂರಚನಾ ref *to_free = ರಿಕ್ತ;

	ಯದಿ (opts->wants_head) {
		ಸಂರಚನಾ ref *head = copy_ref(find_ref_by_name(refs, "HEAD"));
		ಯದಿ (head)
			tail_link_ref(head, &tail);
		ಯದಿ (option_single_branch)
			refs = to_free =
				guess_remote_head(head, refs,
						  REMOTE_GUESS_HEAD_QUIET);
	} ಅನ್ಯಥಾ ಯದಿ (option_single_branch) {
		local_refs = ರಿಕ್ತ;
		tail = &local_refs;
		refs = to_free = copy_ref(find_remote_branch(refs, option_branch));
	}

	ಪ್ರತಿ (size_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->ವಸ್ತುಗಳು[i], &tail, 0);

	free_one_ref(to_free);

	ಪ್ರತ್ಯಾಗಮ local_refs;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_remote_refs(ಧ್ರುವ ಸಂರಚನಾ ref *local_refs)
{
	ಧ್ರುವ ಸಂರಚನಾ ref *r;

	ಸಂರಚನಾ ref_transaction *t;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	ಯದಿ (!t)
		die("%s", err.buf);

	ಪ್ರತಿ (r = local_refs; r; r = r->next) {
		ಯದಿ (!r->peer_ref)
			ಅನುವರ್ತ;
		ಯದಿ (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   ರಿಕ್ತ, 0, ರಿಕ್ತ, &err))
			die("%s", err.buf);
	}

	ಯದಿ (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_followtags(ಧ್ರುವ ಸಂರಚನಾ ref *refs, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ)
{
	ಧ್ರುವ ಸಂರಚನಾ ref *ref;
	ಪ್ರತಿ (ref = refs; ref; ref = ref->next) {
		ಯದಿ (!starts_with(ref->name, "refs/tags/"))
			ಅನುವರ್ತ;
		ಯದಿ (ends_with(ref->name, "^{}"))
			ಅನುವರ್ತ;
		ಯದಿ (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			ಅನುವರ್ತ;
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ,
				ref->name, &ref->old_oid, ರಿಕ್ತ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ object_id *iterate_ref_map(ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ ref **rm = cb_data;
	ಸಂರಚನಾ ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	ಯಾವತ್ (ref && !ref->peer_ref)
		ref = ref->next;
	ಯದಿ (!ref)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	*rm = ref->next;
	ಪ್ರತ್ಯಾಗಮ &ref->old_oid;
}

ಸ್ಥಿರ ಶೂನ್ಯ update_remote_refs(ಧ್ರುವ ಸಂರಚನಾ ref *refs,
			       ಧ್ರುವ ಸಂರಚನಾ ref *mapped_refs,
			       ಧ್ರುವ ಸಂರಚನಾ ref *remote_head_points_at,
			       ಧ್ರುವ ವರ್ಣ *branch_ತುದಿ,
			       ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ,
			       ಸಂರಚನಾ transport *transport,
			       ಪೂರ್ಣಾಙ್ಕ check_connectivity)
{
	ಧ್ರುವ ಸಂರಚನಾ ref *rm = mapped_refs;

	ಯದಿ (check_connectivity) {
		ಸಂರಚನಾ check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.progress = transport->progress;

		ಯದಿ (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	ಯದಿ (refs) {
		write_remote_refs(mapped_refs);
		ಯದಿ (option_single_branch && option_tags)
			write_followtags(refs, ಸಂದೇಶ);
	}

	ಯದಿ (remote_head_points_at && remote_head_points_at->peer_ref && !option_bare) {
		ಸಂರಚನಾ strbuf head_ref = STRBUF_INIT;
		strbuf_addstr(&head_ref, branch_ತುದಿ);
		strbuf_addstr(&head_ref, "HEAD");
		ಯದಿ (refs_update_symref(get_main_ref_store(the_repository), head_ref.buf,
				       remote_head_points_at->peer_ref->name,
				       ಸಂದೇಶ) < 0)
			die(_("unable to update %s"), head_ref.buf);
		strbuf_release(&head_ref);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ update_head(ಸಂರಚನಾ clone_opts *opts, ಧ್ರುವ ಸಂರಚನಾ ref *our, ಧ್ರುವ ಸಂರಚನಾ ref *remote,
			ಧ್ರುವ ವರ್ಣ *unborn, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ)
{
	ಧ್ರುವ ವರ್ಣ *head;
	ಯದಿ (our && !opts->detach && skip_prefix(our->name, "refs/heads/", &head)) {
		/* Local default branch link */
		ಯದಿ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, ರಿಕ್ತ) < 0)
			die(_("unable to update HEAD"));
		ಯದಿ (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					ಸಂದೇಶ, "HEAD", &our->old_oid, ರಿಕ್ತ, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, head, remote_name, our->name);
		}
	} ಅನ್ಯಥಾ ಯದಿ (our) {
		ಸಂರಚನಾ commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ,
				"HEAD", &c->object.oid, ರಿಕ್ತ, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ಅನ್ಯಥಾ ಯದಿ (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ,
				"HEAD", &remote->old_oid, ರಿಕ್ತ, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ಅನ್ಯಥಾ ಯದಿ (unborn && skip_prefix(unborn, "refs/heads/", &head)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		ಯದಿ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, ರಿಕ್ತ) < 0)
			die(_("unable to update HEAD"));
		ಯದಿ (!option_bare)
			install_branch_config(0, head, remote_name, unborn);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_sparse_checkout_init(ಧ್ರುವ ವರ್ಣ *repo)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;
	ಪೂರ್ಣಾಙ್ಕ result = 0;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	strvec_pushl(&cmd.args, "-C", repo, "sparse-checkout", "set", ರಿಕ್ತ);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	ಯದಿ (run_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		result = 1;
	}

	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ checkout(ಪೂರ್ಣಾಙ್ಕ submodule_progress,
		    ಸಂರಚನಾ list_objects_filter_options *filter_options,
		    ಪೂರ್ಣಾಙ್ಕ filter_submodules,
		    ಗಣನಾ ref_storage_format ref_storage_format)
{
	ಸಂರಚನಾ object_id oid;
	ವರ್ಣ *head;
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಸಂರಚನಾ unpack_trees_options opts;
	ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc t;
	ಪೂರ್ಣಾಙ್ಕ err = 0;
	ಸಂರಚನಾ run_hooks_opt hook_opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	ಯದಿ (option_no_checkout)
		ಪ್ರತ್ಯಾಗಮ 0;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, ರಿಕ್ತ);
	ಯದಿ (!head) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(head, "HEAD")) {
		ಯದಿ (advice_enabled(ADVICE_DETACHED_HEAD))
			detach_advice(oid_to_hex(&oid));
		FREE_AND_NULL(head);
	} ಅನ್ಯಥಾ {
		ಯದಿ (!starts_with(head, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	setup_work_ವೃಕ್ಷ(the_repository);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, ಪರಿಮಾಣ opts);
	opts.update = 1;
	opts.merge = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_merge;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	init_checkout_metadata(&opts.meta, head, &oid, ರಿಕ್ತ);

	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository, &oid);
	ಯದಿ (!ವೃಕ್ಷ)
		die(_("unable to parse commit %s"), oid_to_hex(&oid));
	ಯದಿ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ) < 0)
		exit(128);
	init_ವೃಕ್ಷ_desc(&t, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->buffer, ವೃಕ್ಷ->size);
	ಯದಿ (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	ಮುಕ್ತ(head);

	ಯದಿ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.args, oid_to_hex(null_oid(the_hash_algo)),
		     oid_to_hex(&oid), "1", ರಿಕ್ತ);
	err |= run_hooks_opt(the_repository, "post-checkout", &hook_opt);

	ಯದಿ (!err && (option_recurse_submodules.nr > 0)) {
		ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;
		strvec_pushl(&cmd.args, "submodule", "update", "--require-init",
			     "--recursive", ರಿಕ್ತ);

		ಯದಿ (option_shallow_submodules == 1)
			strvec_ತಳ್ಳು(&cmd.args, "--depth=1");

		ಯದಿ (max_jobs != -1)
			strvec_pushf(&cmd.args, "--jobs=%d", max_jobs);

		ಯದಿ (submodule_progress)
			strvec_ತಳ್ಳು(&cmd.args, "--progress");

		ಯದಿ (option_verbosity < 0)
			strvec_ತಳ್ಳು(&cmd.args, "--quiet");

		ಯದಿ (option_remote_submodules) {
			strvec_ತಳ್ಳು(&cmd.args, "--remote");
			strvec_ತಳ್ಳು(&cmd.args, "--no-fetch");
		}

		ಯದಿ (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(ref_storage_format));

		ಯದಿ (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.args, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		ಯದಿ (option_single_branch >= 0)
			strvec_ತಳ್ಳು(&cmd.args, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = run_command(&cmd);
	}

	ಪ್ರತ್ಯಾಗಮ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_clone_config(ಧ್ರುವ ವರ್ಣ *k, ಧ್ರುವ ವರ್ಣ *v,
			    ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(k, "clone.defaultremotename")) {
		ಯದಿ (!v)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(k);
		ಮುಕ್ತ(remote_name);
		remote_name = xstrdup(v);
	}
	ಯದಿ (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	ಯದಿ (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	ಪ್ರತ್ಯಾಗಮ git_default_config(k, v, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_one_config(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			    ಧ್ರುವ ಸಂರಚನಾ config_context *ctx,
			    ಶೂನ್ಯ *data)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	ಪೂರ್ಣಾಙ್ಕ apply_failed = git_clone_config(ಕೀಲಿ, ಮೌಲ್ಯ, ctx, data);
	ಯದಿ (apply_failed)
		ಪ್ರತ್ಯಾಗಮ apply_failed;

	ಪ್ರತ್ಯಾಗಮ repo_config_set_multivar_gently(the_repository, ಕೀಲಿ,
					       ಮೌಲ್ಯ ? ಮೌಲ್ಯ : "true",
					       CONFIG_REGEX_NONE, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_config(ಸಂರಚನಾ string_list *config)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < config->nr; i++) {
		ಯದಿ (git_config_parse_parameter(config->ವಸ್ತುಗಳು[i].string,
					       write_one_config, ರಿಕ್ತ) < 0)
			die(_("unable to write parameters to config file"));
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ write_refspec_config(ಧ್ರುವ ವರ್ಣ *src_ref_prefix,
		ಧ್ರುವ ಸಂರಚನಾ ref *our_head_points_at,
		ಧ್ರುವ ಸಂರಚನಾ ref *remote_head_points_at,
		ಸಂರಚನಾ strbuf *branch_ತುದಿ)
{
	ಸಂರಚನಾ strbuf ಕೀಲಿ = STRBUF_INIT;
	ಸಂರಚನಾ strbuf ಮೌಲ್ಯ = STRBUF_INIT;

	ಯದಿ (option_mirror || !option_bare) {
		ಯದಿ (option_single_branch && !option_mirror) {
			ಯದಿ (option_branch) {
				ಯದಿ (starts_with(our_head_points_at->name, "refs/tags/"))
					strbuf_addf(&ಮೌಲ್ಯ, "+%s:%s", our_head_points_at->name,
						our_head_points_at->name);
				ಅನ್ಯಥಾ
					strbuf_addf(&ಮೌಲ್ಯ, "+%s:%s%s", our_head_points_at->name,
						branch_ತುದಿ->buf, option_branch);
			} ಅನ್ಯಥಾ ಯದಿ (remote_head_points_at) {
				ಧ್ರುವ ವರ್ಣ *head = remote_head_points_at->name;
				ಯದಿ (!skip_prefix(head, "refs/heads/", &head))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&ಮೌಲ್ಯ, "+%s:%s%s", remote_head_points_at->name,
						branch_ತುದಿ->buf, head);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} ಅನ್ಯಥಾ {
			strbuf_addf(&ಮೌಲ್ಯ, "+%s*:%s*", src_ref_prefix, branch_ತುದಿ->buf);
		}
		/* Configure the remote */
		ಯದಿ (ಮೌಲ್ಯ.len) {
			strbuf_addf(&ಕೀಲಿ, "remote.%s.fetch", remote_name);
			repo_config_set_multivar(the_repository, ಕೀಲಿ.buf, ಮೌಲ್ಯ.buf, "^$", 0);
			strbuf_reset(&ಕೀಲಿ);

			ಯದಿ (option_mirror) {
				strbuf_addf(&ಕೀಲಿ, "remote.%s.mirror", remote_name);
				repo_config_set(the_repository, ಕೀಲಿ.buf, "true");
				strbuf_reset(&ಕೀಲಿ);
			}
		}
	}

	strbuf_release(&ಕೀಲಿ);
	strbuf_release(&ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ dissociate_from_references(ಶೂನ್ಯ)
{
	ವರ್ಣ *alternates = repo_git_path(the_repository, "objects/info/alternates");

	ಯದಿ (!access(alternates, F_OK)) {
		ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.args, "repack", "-a", "-d", ರಿಕ್ತ);
		ಯದಿ (run_command(&cmd))
			die(_("cannot repack to clean up"));
		ಯದಿ (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	ಮುಕ್ತ(alternates);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ path_exists(ಧ್ರುವ ವರ್ಣ *path)
{
	ಸಂರಚನಾ stat sb;
	ಪ್ರತ್ಯಾಗಮ !stat(path, &sb);
}

ಪೂರ್ಣಾಙ್ಕ cmd_clone(ಪೂರ್ಣಾಙ್ಕ argc,
	      ಧ್ರುವ ವರ್ಣ **argv,
	      ಧ್ರುವ ವರ್ಣ *prefix,
	      ಸಂರಚನಾ repository *repository UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ is_bundle = 0, is_local;
	ಪೂರ್ಣಾಙ್ಕ reject_shallow = 0;
	ಧ್ರುವ ವರ್ಣ *repo_name, *repo, *work_ವೃಕ್ಷ, *git_dir;
	ವರ್ಣ *repo_to_free = ರಿಕ್ತ;
	ವರ್ಣ *path = ರಿಕ್ತ, *dir, *display_repo = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ dest_exists, real_dest_exists = 0;
	ಧ್ರುವ ಸಂರಚನಾ ref *refs, *remote_head;
	ಸಂರಚನಾ ref *remote_head_points_at = ರಿಕ್ತ;
	ಧ್ರುವ ಸಂರಚನಾ ref *our_head_points_at;
	ವರ್ಣ *unborn_head = ರಿಕ್ತ;
	ಸಂರಚನಾ ref *mapped_refs = ರಿಕ್ತ;
	ಧ್ರುವ ಸಂರಚನಾ ref *ref;
	ಸಂರಚನಾ strbuf ಕೀಲಿ = STRBUF_INIT;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ strbuf branch_ತುದಿ = STRBUF_INIT, reflog_ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ transport *transport = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *src_ref_prefix = "refs/heads/";
	ಸಂರಚನಾ remote *remote;
	ಪೂರ್ಣಾಙ್ಕ err = 0, complete_refs_before_fetch = 1;
	ಪೂರ್ಣಾಙ್ಕ submodule_progress;
	ಪೂರ್ಣಾಙ್ಕ filter_submodules = 0;
	ಪೂರ್ಣಾಙ್ಕ hash_algo;
	ಗಣನಾ ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ do_not_override_repo_unix_permissions = -1;
	ಪೂರ್ಣಾಙ್ಕ option_reject_shallow = -1; /* unspecified */
	ಪೂರ್ಣಾಙ್ಕ deepen = 0;
	ವರ್ಣ *option_template = ರಿಕ್ತ, *option_depth = ರಿಕ್ತ, *option_since = ರಿಕ್ತ;
	ವರ್ಣ *option_origin = ರಿಕ್ತ;
	ಸಂರಚನಾ string_list option_not = STRING_LIST_INIT_NODUP;
	ಧ್ರುವ ವರ್ಣ *real_git_dir = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *ref_storage_format_uri = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *option_upload_pack = "git-upload-pack";
	ಪೂರ್ಣಾಙ್ಕ option_progress = -1;
	ಪೂರ್ಣಾಙ್ಕ option_sparse_checkout = 0;
	ಗಣನಾ transport_family family = TRANSPORT_FAMILY_ALL;
	ಸಂರಚನಾ string_list option_config = STRING_LIST_INIT_DUP;
	ಪೂರ್ಣಾಙ್ಕ option_dissociate = 0;
	ಸಂರಚನಾ list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	ಪೂರ್ಣಾಙ್ಕ option_filter_submodules = -1; /* unspecified */
	ಸಂರಚನಾ string_list server_options = STRING_LIST_INIT_NODUP;
	ಧ್ರುವ ವರ್ಣ *bundle_uri = ರಿಕ್ತ;
	ವರ್ಣ *option_rev = ರಿಕ್ತ;

	ಸಂರಚನಾ clone_opts opts = CLONE_OPTS_INIT;

	ಸಂರಚನಾ transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	ಸಂರಚನಾ option builtin_clone_options[] = {
		OPT__VERBOSITY(&option_verbosity),
		OPT_BOOL(0, "progress", &option_progress,
			 N_("force progress reporting")),
		OPT_BOOL(0, "reject-shallow", &option_reject_shallow,
			 N_("don't clone shallow repository")),
		OPT_BOOL('n', "no-checkout", &option_no_checkout,
			 N_("don't create a checkout")),
		OPT_BOOL(0, "bare", &option_bare, N_("create a bare repository")),
		OPT_HIDDEN_BOOL(0, "naked", &option_bare,
				N_("create a bare repository")),
		OPT_BOOL(0, "mirror", &option_mirror,
			 N_("create a mirror repository (implies --bare)")),
		OPT_BOOL('l', "local", &option_local,
			 N_("to clone from a local repository")),
		OPT_BOOL(0, "no-hardlinks", &option_no_hardlinks,
			 N_("don't use local hardlinks, always copy")),
		OPT_BOOL('s', "shared", &option_shared,
			 N_("setup as shared repository")),
		{
			.type = OPTION_CALLBACK,
			.long_name = "recurse-submodules",
			.ಮೌಲ್ಯ = &option_recurse_submodules,
			.argh = N_("pathspec"),
			.help = N_("initialize submodules in the clone"),
			.flags = PARSE_OPT_OPTARG,
			.callback = recurse_submodules_cb,
			.defval = (intptr_t)".",
		},
		OPT_ALIAS(0, "recursive", "recurse-submodules"),
		OPT_INTEGER('j', "jobs", &max_jobs,
			    N_("number of submodules cloned in parallel")),
		OPT_STRING(0, "template", &option_template, N_("template-directory"),
			   N_("directory from which templates will be used")),
		OPT_STRING_LIST(0, "reference", &option_required_reference, N_("repo"),
				N_("reference repository")),
		OPT_STRING_LIST(0, "reference-if-able", &option_optional_reference,
				N_("repo"), N_("reference repository")),
		OPT_BOOL(0, "dissociate", &option_dissociate,
			 N_("use --reference only while cloning")),
		OPT_STRING('o', "origin", &option_origin, N_("name"),
			   N_("use <name> instead of 'origin' to track upstream")),
		OPT_STRING('b', "branch", &option_branch, N_("branch"),
			   N_("checkout <branch> instead of the remote's HEAD")),
		OPT_STRING(0, "revision", &option_rev, N_("rev"),
			   N_("clone single revision <rev> and check out")),
		OPT_STRING('u', "upload-pack", &option_upload_pack, N_("path"),
			   N_("path to git-upload-pack on the remote")),
		OPT_STRING(0, "depth", &option_depth, N_("depth"),
			   N_("create a shallow clone of that depth")),
		OPT_STRING(0, "shallow-since", &option_since, N_("time"),
			   N_("create a shallow clone since a specific time")),
		OPT_STRING_LIST(0, "shallow-exclude", &option_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_BOOL(0, "single-branch", &option_single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_BOOL(0, "tags", &option_tags,
			 N_("clone tags, and make later fetches not to follow them")),
		OPT_BOOL(0, "shallow-submodules", &option_shallow_submodules,
			 N_("any cloned submodules will be shallow")),
		OPT_STRING(0, "separate-git-dir", &real_git_dir, N_("gitdir"),
			   N_("separate git dir from working tree")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format_uri, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_STRING_LIST('c', "config", &option_config, N_("key=value"),
				N_("set config inside the new repository")),
		OPT_STRING_LIST(0, "server-option", &server_options,
				N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_BOOL(0, "also-filter-submodules", &option_filter_submodules,
			 N_("apply partial clone filters to submodules")),
		OPT_BOOL(0, "remote-submodules", &option_remote_submodules,
			 N_("any cloned submodules will use their remote-tracking branch")),
		OPT_BOOL(0, "sparse", &option_sparse_checkout,
			 N_("initialize sparse-checkout file to include only files at root")),
		OPT_STRING(0, "bundle-uri", &bundle_uri,
			   N_("uri"), N_("a URI for downloading bundles before fetching from origin remote")),
		OPT_END()
	};
	ಸಂರಚನಾ strvec alternates = STRVEC_INIT;

	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		ರಿಕ್ತ
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_identity("clone");

	repo_config(the_repository, git_clone_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, builtin_clone_options,
			     builtin_clone_usage, 0);

	ಯದಿ (argc > 2)
		usage_ಸಂದೇಶ_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	ಯದಿ (argc == 0)
		usage_ಸಂದೇಶ_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	ಯದಿ (option_depth || option_since || option_not.nr)
		deepen = 1;
	ಯದಿ (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	ಯದಿ (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, ರಿಕ್ತ);
		ಯದಿ (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	ಯದಿ (option_mirror) {
		option_bare = 1;
		option_tags = 0;
	}

	ಯದಿ (option_bare) {
		ಯದಿ (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	ಯದಿ (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	path = get_repo_path(repo_name, &is_bundle);
	ಯದಿ (path) {
		FREE_AND_NULL(path);
		repo = repo_to_free = absolute_pathdup(repo_name);
	} ಅನ್ಯಥಾ ಯದಿ (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} ಅನ್ಯಥಾ
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	ಯದಿ (option_depth && atoi(option_depth) < 1)
		die(_("depth %s is not a positive number"), option_depth);

	ಯದಿ (argc == 2)
		dir = xstrdup(argv[1]);
	ಅನ್ಯಥಾ
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = path_exists(dir);
	ಯದಿ (dest_exists && !is_empty_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	ಯದಿ (real_git_dir) {
		real_dest_exists = path_exists(real_git_dir);
		ಯದಿ (real_dest_exists && !is_empty_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_ಸಂದೇಶ, "clone: from %s",
		    display_repo ? display_repo : repo);
	ಮುಕ್ತ(display_repo);

	ಯದಿ (option_bare)
		work_ವೃಕ್ಷ = ರಿಕ್ತ;
	ಅನ್ಯಥಾ {
		work_ವೃಕ್ಷ = getenv("GIT_WORK_TREE");
		ಯದಿ (work_ವೃಕ್ಷ && path_exists(work_ವೃಕ್ಷ))
			die(_("working tree '%s' already exists."), work_ವೃಕ್ಷ);
	}

	ಯದಿ (option_bare || work_ವೃಕ್ಷ)
		git_dir = xstrdup(dir);
	ಅನ್ಯಥಾ {
		work_ವೃಕ್ಷ = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_ತಳ್ಳು_common(remove_junk_on_signal);

	ಯದಿ (!option_bare) {
		ಯದಿ (safe_create_leading_directories_const(the_repository, work_ವೃಕ್ಷ) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_ವೃಕ್ಷ);
		ಯದಿ (dest_exists)
			junk_work_ವೃಕ್ಷ_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		ಅನ್ಯಥಾ ಯದಿ (mkdir(work_ವೃಕ್ಷ, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_ವೃಕ್ಷ);
		junk_work_ವೃಕ್ಷ = work_ವೃಕ್ಷ;
	}

	ಯದಿ (real_git_dir) {
		ಯದಿ (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} ಅನ್ಯಥಾ {
		ಯದಿ (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	ಯದಿ (safe_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	ಯದಿ (0 <= option_verbosity) {
		ಯದಿ (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		ಅನ್ಯಥಾ
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	ಯದಿ (option_recurse_submodules.nr > 0) {
		ಸಂರಚನಾ string_list_item *item;
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
		ಪೂರ್ಣಾಙ್ಕ val;

		/* remove duplicates */
		string_list_sort_u(&option_recurse_submodules, 0);

		/*
		 * NEEDSWORK: In a multi-working-tree world, this needs to be
		 * set in the per-worktree config.
		 */
		for_each_string_list_item(item, &option_recurse_submodules) {
			strbuf_addf(&sb, "submodule.active=%s",
				    item->string);
			string_list_append(&option_config, sb.buf);
			strbuf_reset(&sb);
		}

		ಯದಿ (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		ಯದಿ (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		ಅನ್ಯಥಾ ಯದಿ (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} ಅನ್ಯಥಾ ಯದಿ (option_optional_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=info");
		}

		strbuf_release(&sb);
	}

	/*
	 * Initialize the repository, but skip initializing the reference
	 * database. We do not yet know about the object format of the
	 * repository, and reference backends may persist that information into
	 * their on-disk data structures.
	 */
	create_repository(the_repository, git_dir, real_git_dir, work_ವೃಕ್ಷ,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, ರಿಕ್ತ);

	ಯದಿ (real_git_dir) {
		ಮುಕ್ತ((ವರ್ಣ *)git_dir);
		git_dir = real_git_dir;
	}

	/*
	 * We have a chicken-and-egg situation between initializing the refdb
	 * and spawning transport helpers:
	 *
	 *   - Initializing the refdb requires us to know about the object
	 *     format. We thus have to spawn the transport helper to learn
	 *     about it.
	 *
	 *   - The transport helper may want to access the Git repository. But
	 *     because the refdb has not been initialized, we don't have "HEAD"
	 *     or "refs/". Thus, the helper cannot find the Git repository.
	 *
	 * Ideally, we would have structured the helper protocol such that it's
	 * mandatory for the helper to first announce its capabilities without
	 * yet assuming a fully initialized repository. Like that, we could
	 * have added a "lazy-refdb-init" capability that announces whether the
	 * helper is ready to handle not-yet-initialized refdbs. If any helper
	 * didn't support them, we would have fully initialized the refdb with
	 * the SHA1 object format, but later on bailed out if we found out that
	 * the remote repository used a different object format.
	 *
	 * But we didn't, and thus we use the following workaround to partially
	 * initialize the repository's refdb such that it can be discovered by
	 * Git commands. To do so, we:
	 *
	 *   - Create an invalid HEAD ref pointing at "refs/heads/.invalid".
	 *
	 *   - Create the "refs/" directory.
	 *
	 *   - Set up the ref storage format and repository version as
	 *     required.
	 *
	 * This is sufficient for Git commands to discover the Git directory.
	 */
	initialize_repository_version(the_repository, GIT_HASH_UNKNOWN,
				      the_repository->ref_storage_format, 1);

	refs_create_refdir_stubs(the_repository, git_dir, ರಿಕ್ತ);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, ರಿಕ್ತ);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	ಯದಿ (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	ಯದಿ (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	ಯದಿ (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	ಯದಿ (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	ಯದಿ (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	ಯದಿ (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	ಯದಿ (option_origin) {
		ಮುಕ್ತ(remote_name);
		remote_name = xstrdup(option_origin);
	}

	ಯದಿ (!remote_name)
		remote_name = xstrdup("origin");

	ಯದಿ (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	ಯದಿ (option_bare) {
		ಯದಿ (option_mirror)
			src_ref_prefix = "refs/";
		strbuf_addstr(&branch_ತುದಿ, src_ref_prefix);

		repo_config_set(the_repository, "core.bare", "true");
	} ಅನ್ಯಥಾ ಯದಿ (!option_rev) {
		strbuf_addf(&branch_ತುದಿ, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&ಕೀಲಿ, "remote.%s.url", remote_name);
	repo_config_set(the_repository, ಕೀಲಿ.buf, repo);
	strbuf_reset(&ಕೀಲಿ);

	ಯದಿ (!option_tags) {
		strbuf_addf(&ಕೀಲಿ, "remote.%s.tagOpt", remote_name);
		repo_config_set(the_repository, ಕೀಲಿ.buf, "--no-tags");
		strbuf_reset(&ಕೀಲಿ);
	}

	remote = remote_get_early(remote_name);

	ಯದಿ (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_prefix,
				branch_ತುದಿ.buf);

	path = get_repo_path(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && path && !is_bundle;
	ಯದಿ (is_local) {
		ಯದಿ (option_depth)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		ಯದಿ (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		ಯದಿ (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		ಯದಿ (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		ಯದಿ (!access(mkpath("%s/shallow", path), F_OK)) {
			ಯದಿ (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			ಯದಿ (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	ಯದಿ (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? path : ರಿಕ್ತ);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, path ? path : remote->url.v[0]);
	transport_set_verbosity(transport, option_verbosity, option_progress);
	transport->family = family;
	transport->cloning = 1;

	ಯದಿ (is_bundle) {
		ಸಂರಚನಾ bundle_header header = BUNDLE_HEADER_INIT;
		ಪೂರ್ಣಾಙ್ಕ fd = read_bundle_header(path, &header);
		ಪೂರ್ಣಾಙ್ಕ has_filter = header.filter.choice != LOFC_DISABLED;

		ಯದಿ (fd > 0)
			close(fd);
		bundle_header_release(&header);
		ಯದಿ (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_set_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	ಯದಿ (reject_shallow)
		transport_set_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	ಯದಿ (option_depth)
		transport_set_option(transport, TRANS_OPT_DEPTH,
				     option_depth);
	ಯದಿ (option_since)
		transport_set_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	ಯದಿ (option_not.nr)
		transport_set_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (ಧ್ರುವ ವರ್ಣ *)&option_not);
	ಯದಿ (option_single_branch) {
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		ಯದಿ (option_branch)
			opts.wants_head = 0;
	}

	ಯದಿ (option_upload_pack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	ಯದಿ (server_options.nr)
		transport->server_options = &server_options;

	ಯದಿ (filter_options.choice) {
		ಧ್ರುವ ವರ್ಣ *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	ಯದಿ (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	ಯದಿ (option_rev) {
		option_tags = 0;
		option_single_branch = 0;
		opts.wants_head = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	ಯದಿ (option_tags || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, TAG_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	ಯದಿ (option_branch)
		expand_ref_prefix(&transport_ls_refs_options.ref_prefixes,
				  option_branch);

	/*
	 * As part of transport_get_remote_refs() the server tells us the hash
	 * algorithm, which we require to initialize the repo. But calling that
	 * function without any ref prefix, will cause the server to announce
	 * all known refs. If the argument passed to --revision was a hex oid,
	 * ref_prefixes will be empty so we fall back to asking about HEAD to
	 * reduce traffic from the server.
	 */
	ಯದಿ (opts.wants_head || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_ತಳ್ಳು(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_version(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_set_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, ರಿಕ್ತ, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	ಯದಿ (bundle_uri) {
		ಸಂರಚನಾ remote_state *state;
		ಪೂರ್ಣಾಙ್ಕ has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = ರಿಕ್ತ;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		ಯದಿ (repo_init(the_repository, git_dir, work_ವೃಕ್ಷ))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		ಅನ್ಯಥಾ ಯದಿ (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		ಅನ್ಯಥಾ ಯದಿ (has_heuristic)
			repo_config_set_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		ಮುಕ್ತ(the_repository->remote_state);
		the_repository->remote_state = state;
	} ಅನ್ಯಥಾ {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		ಯದಿ (transport->bundles &&
		    hashmap_get_size(&transport->bundles->bundles)) {
			ಸಂರಚನಾ remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = ರಿಕ್ತ;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			ಯದಿ (repo_init(the_repository, git_dir, work_ವೃಕ್ಷ))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			ಅನ್ಯಥಾ ಯದಿ (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			ಮುಕ್ತ(the_repository->remote_state);
			the_repository->remote_state = state;
		} ಅನ್ಯಥಾ {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	ಯದಿ (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	ಯದಿ (mapped_refs) {
		/*
		 * transport_get_remote_refs() may return refs with null sha-1
		 * in mapped_refs (see struct transport->get_refs_list
		 * comment). In that case we need fetch it early because
		 * remote_head code below relies on it.
		 *
		 * for normal clones, transport_get_remote_refs() should
		 * return reliable ref set, we can delay cloning until after
		 * remote HEAD check.
		 */
		ಪ್ರತಿ (ref = refs; ref; ref = ref->next)
			ಯದಿ (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				ಭಙ್ಗ;
			}

		ಯದಿ (!is_local && !complete_refs_before_fetch) {
			ಯದಿ (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_head = find_ref_by_name(refs, "HEAD");
	remote_head_points_at = guess_remote_head(remote_head, mapped_refs,
						  REMOTE_GUESS_HEAD_QUIET);

	ಯದಿ (option_branch) {
		our_head_points_at = find_remote_branch(mapped_refs, option_branch);
		ಯದಿ (!our_head_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} ಅನ್ಯಥಾ ಯದಿ (option_rev) {
		our_head_points_at = mapped_refs;
		ಯದಿ (!our_head_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} ಅನ್ಯಥಾ ಯದಿ (remote_head_points_at) {
		our_head_points_at = remote_head_points_at;
	} ಅನ್ಯಥಾ ಯದಿ (remote_head) {
		our_head_points_at = ರಿಕ್ತ;
	} ಅನ್ಯಥಾ {
		ವರ್ಣ *to_free = ರಿಕ್ತ;
		ಧ್ರುವ ವರ್ಣ *branch;

		ಯದಿ (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		ಯದಿ (transport_ls_refs_options.unborn_head_target &&
		    skip_prefix(transport_ls_refs_options.unborn_head_target,
				"refs/heads/", &branch)) {
			unborn_head  = xstrdup(transport_ls_refs_options.unborn_head_target);
		} ಅನ್ಯಥಾ {
			branch = to_free = repo_default_branch_name(the_repository, 0);
			unborn_head = xstrfmt("refs/heads/%s", branch);
		}

		/*
		 * We may have selected a local default branch name "foo",
		 * and even though the remote's HEAD does not point there,
		 * it may still have a "foo" branch. If so, set it up so
		 * that we can follow the usual checkout code later.
		 *
		 * Note that for an empty repo we'll already have set
		 * option_no_checkout above, which would work against us here.
		 * But for an empty repo, find_remote_branch() can never find
		 * a match.
		 */
		our_head_points_at = find_remote_branch(mapped_refs, branch);

		ಮುಕ್ತ(to_free);
	}

	ಯದಿ (!option_rev)
		write_refspec_config(src_ref_prefix, our_head_points_at,
				     remote_head_points_at, &branch_ತುದಿ);

	ಯದಿ (filter_options.choice)
		partial_clone_register(remote_name, &filter_options);

	ಯದಿ (is_local)
		clone_local(path, git_dir);
	ಅನ್ಯಥಾ ಯದಿ (mapped_refs && complete_refs_before_fetch) {
		ಯದಿ (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_head_points_at,
			   branch_ತುದಿ.buf, reflog_ಸಂದೇಶ.buf, transport,
			   !is_local);

	update_head(&opts, our_head_points_at, remote_head, unborn_head, reflog_ಸಂದೇಶ.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_progress = transport->progress;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	ಯದಿ (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	ಯದಿ (option_sparse_checkout && git_sparse_checkout_init(dir))
		ಪ್ರತ್ಯಾಗಮ 1;

	junk_mode = JUNK_LEAVE_REPO;
	err = checkout(submodule_progress,
		       &filter_options,
		       filter_submodules,
		       ref_storage_format);

	list_objects_filter_release(&filter_options);

	string_list_clear(&option_not, 0);
	string_list_clear(&option_config, 0);
	string_list_clear(&server_options, 0);
	strvec_clear(&alternates);

	ಮುಕ್ತ(remote_name);
	strbuf_release(&reflog_ಸಂದೇಶ);
	strbuf_release(&branch_ತುದಿ);
	strbuf_release(&buf);
	strbuf_release(&ಕೀಲಿ);
	free_refs(mapped_refs);
	free_refs(remote_head_points_at);
	ಮುಕ್ತ(unborn_head);
	ಮುಕ್ತ(dir);
	ಮುಕ್ತ(path);
	ಮುಕ್ತ(repo_to_free);
	junk_mode = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	ಪ್ರತ್ಯಾಗಮ err;
}
