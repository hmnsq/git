/*
 * Builtin "git clone"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *		 2008 Daniel Barkalow <barkalow@iabervon.org>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 *
 * Clone a repository into a different directory that does not yet exist.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "copy.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpack-trees.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "transport.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir-iterator.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "iterator.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sigchain.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "connected.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pkt-line.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list-objects-filter-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hook.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bundle.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

ਬਣਤਰ clone_opts {
	ਪੂਰਨ ਚਾਹੁੰਦਾ_ਸਿਰ;
	ਪੂਰਨ detach;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CLONE_OPTS_INIT { \
	.ਚਾਹੁੰਦਾ_ਸਿਰ = 1 /* default enabled */ \
}

ਸਥਿਰ ਪੂਰਨ option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
ਸਥਿਰ ਪੂਰਨ option_local = -1, option_no_hardlinks, option_shared;
ਸਥਿਰ ਪੂਰਨ option_ਟੈਗ_ਸੂਚੀ = 1; /* default enabled */
ਸਥਿਰ ਪੂਰਨ option_shallow_submodules;
ਸਥਿਰ ਪੂਰਨ config_reject_shallow = -1;    /* unspecified */
ਸਥਿਰ ਅੱਖਰ *remote_name = ਨਿਰਾ;
ਸਥਿਰ ਅੱਖਰ *option_branch = ਨਿਰਾ;
ਸਥਿਰ ਪੂਰਨ option_verbosity;
ਸਥਿਰ ਬਣਤਰ string_list option_required_reference = STRING_LIST_INIT_NODUP;
ਸਥਿਰ ਬਣਤਰ string_list option_optional_reference = STRING_LIST_INIT_NODUP;
ਸਥਿਰ ਪੂਰਨ max_jobs = -1;
ਸਥਿਰ ਬਣਤਰ string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
ਸਥਿਰ ਪੂਰਨ config_filter_submodules = -1;    /* unspecified */
ਸਥਿਰ ਪੂਰਨ option_remote_submodules;

ਸਥਿਰ ਪੂਰਨ recurse_submodules_cb(ਅਚਲ ਬਣਤਰ option *opt,
				 ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਜੇ (unset)
		string_list_clear((ਬਣਤਰ string_list *)opt->ਮੁੱਲ, 0);
	ਨਹੀਂਤਾਂ ਜੇ (arg)
		string_list_append((ਬਣਤਰ string_list *)opt->ਮੁੱਲ, arg);
	ਨਹੀਂਤਾਂ
		string_list_append((ਬਣਤਰ string_list *)opt->ਮੁੱਲ,
				   (ਅਚਲ ਅੱਖਰ *)opt->defval);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_repo_ਰਸਤਾ_1(ਬਣਤਰ strbuf *ਰਸਤਾ, ਪੂਰਨ *is_bundle)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *suffix[] = { "/.git", "", ".git/.git", ".git" };
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *bundle_suffix[] = { ".bundle", "" };
	ਮਾਪ_t baselen = ਰਸਤਾ->len;
	ਬਣਤਰ stat st;
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < ARRAY_ਮਾਪ(suffix); i++) {
		strbuf_setlen(ਰਸਤਾ, baselen);
		strbuf_addstr(ਰਸਤਾ, suffix[i]);
		ਜੇ (stat(ਰਸਤਾ->buf, &st))
			ਜਾਰੀ;
		ਜੇ (S_ISDIR(st.st_ਬਹੁਲਕ) && is_git_directory(ਰਸਤਾ->buf)) {
			*is_bundle = 0;
			ਵਾਪਸ ਰਸਤਾ->buf;
		} ਨਹੀਂਤਾਂ ਜੇ (S_ISREG(st.st_ਬਹੁਲਕ) && st.st_ਮਾਪ > 8) {
			/* Is it a "gitfile"? */
			ਅੱਖਰ signature[8];
			ਅਚਲ ਅੱਖਰ *dst;
			ਪੂਰਨ len, fd = open(ਰਸਤਾ->buf, O_RDONLY);
			ਜੇ (fd < 0)
				ਜਾਰੀ;
			len = read_ਵਿੱਚ_ਭਰਿਆ(fd, signature, 8);
			close(fd);
			ਜੇ (len != 8 || ਲੜੀ_ਤੁਲਨਾ(signature, "gitdir: ", 8))
				ਜਾਰੀ;
			dst = read_gitfile(ਰਸਤਾ->buf);
			ਜੇ (dst) {
				*is_bundle = 0;
				ਵਾਪਸ dst;
			}
		}
	}

	ਲਈ (i = 0; i < ARRAY_ਮਾਪ(bundle_suffix); i++) {
		strbuf_setlen(ਰਸਤਾ, baselen);
		strbuf_addstr(ਰਸਤਾ, bundle_suffix[i]);
		ਜੇ (!stat(ਰਸਤਾ->buf, &st) && S_ISREG(st.st_ਬਹੁਲਕ)) {
			*is_bundle = 1;
			ਵਾਪਸ ਰਸਤਾ->buf;
		}
	}

	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਅੱਖਰ *get_repo_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *repo, ਪੂਰਨ *is_bundle)
{
	ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *raw;
	ਅੱਖਰ *canon;

	strbuf_addstr(&ਰਸਤਾ, repo);
	raw = get_repo_ਰਸਤਾ_1(&ਰਸਤਾ, is_bundle);
	canon = raw ? absolute_pathdup(raw) : ਨਿਰਾ;
	strbuf_release(&ਰਸਤਾ);
	ਵਾਪਸ canon;
}

ਬਣਤਰ collect_alternates_ਅੰਕੜੇ {
	ਬਣਤਰ strvec *alternates;
	bool required;
};

ਸਥਿਰ ਪੂਰਨ collect_one_alternate(ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ collect_alternates_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਅੱਖਰ *ref_git = compute_alternate_ਰਸਤਾ(ਵਸਤੂ->string, &err);

	ਜੇ (!ref_git) {
		ਜੇ (ਅੰਕੜੇ->required)
			die("%s", err.buf);
		ਨਹੀਂਤਾਂ
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				ਵਸਤੂ->string, err.buf);
	} ਨਹੀਂਤਾਂ {
		strvec_pushf(ਅੰਕੜੇ->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	ਮੁਕਤ(ref_git);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ read_alternates(ਬਣਤਰ strvec *alternates, ਅਚਲ ਅੱਖਰ *src_repo)
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
	FILE *ਵਿੱਚ;
	ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;
	ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;

	strbuf_addf(&ਰਸਤਾ, "%s/objects/info/alternates", src_repo);

	ਵਿੱਚ = fopen(ਰਸਤਾ.buf, "r");
	ਜੇ (!ਵਿੱਚ) {
		ਜੇ (errno == ENOENT)
			ਜਾਓ ਬਾਹਰਲਾ;
		die_errno("could not read alternates file '%s'", ਰਸਤਾ.buf);
	}

	ਜਦੋਂ (strbuf_getline(&ਸਤਰ, ਵਿੱਚ) != EOF) {
		ਅੱਖਰ *abs_ਰਸਤਾ;
		ਜੇ (!ਸਤਰ.len || ਸਤਰ.buf[0] == '#')
			ਜਾਰੀ;
		ਜੇ (is_absolute_ਰਸਤਾ(ਸਤਰ.buf)) {
			strvec_ਧੱਕੋ(alternates, ਸਤਰ.buf);
			ਜਾਰੀ;
		}
		abs_ਰਸਤਾ = mkpathdup("%s/objects/%s", src_repo, ਸਤਰ.buf);
		ਜੇ (!normalize_ਰਸਤਾ_copy(abs_ਰਸਤਾ, abs_ਰਸਤਾ))
			strvec_ਧੱਕੋ(alternates, abs_ਰਸਤਾ);
		ਨਹੀਂਤਾਂ
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, ਸਤਰ.buf);
		ਮੁਕਤ(abs_ਰਸਤਾ);
	}

ਬਾਹਰਲਾ:
	strbuf_release(&ਰਸਤਾ);
	strbuf_release(&ਸਤਰ);
	ਜੇ (ਵਿੱਚ)
		fclose(ਵਿੱਚ);
}

ਸਥਿਰ ਖਾਲੀ collect_alternates(ਬਣਤਰ strvec *alternates,
			       ਅਚਲ ਅੱਖਰ *local_ਸਰੋਤ_repo)
{
	ਜੇ (option_required_reference.nr || option_optional_reference.nr) {
		ਬਣਤਰ collect_alternates_ਅੰਕੜੇ ਅੰਕੜੇ = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &ਅੰਕੜੇ);
		ਅੰਕੜੇ.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &ਅੰਕੜੇ);
	}

	ਜੇ (local_ਸਰੋਤ_repo) {
		ਬਣਤਰ strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_ਸਰੋਤ_repo);
		ਜੇ (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		ਨਹੀਂਤਾਂ
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

ਸਥਿਰ ਖਾਲੀ mkdir_if_missing(ਅਚਲ ਅੱਖਰ *pathname, ਬਹੁਲਕ_t ਬਹੁਲਕ)
{
	ਬਣਤਰ stat st;

	ਜੇ (!mkdir(pathname, ਬਹੁਲਕ))
		ਵਾਪਸ;

	ਜੇ (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	ਨਹੀਂਤਾਂ ਜੇ (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	ਨਹੀਂਤਾਂ ਜੇ (!S_ISDIR(st.st_ਬਹੁਲਕ))
		die(_("%s exists and is not a directory"), pathname);
}

ਸਥਿਰ ਖਾਲੀ copy_or_link_directory(ਬਣਤਰ strbuf *src, ਬਣਤਰ strbuf *dest,
				   ਅਚਲ ਅੱਖਰ *src_repo)
{
	ਪੂਰਨ src_len, dest_len;
	ਬਣਤਰ dir_iterator *iter;
	ਪੂਰਨ iter_ਸਥਿਤੀ;

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
	die_upon_dubious_ownership(ਨਿਰਾ, ਨਿਰਾ, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	ਜੇ (!iter) {
		ਜੇ (errno == ENOTDIR) {
			ਪੂਰਨ saved_errno = errno;
			ਬਣਤਰ stat st;

			ਜੇ (!lstat(src->buf, &st) && S_ISLNK(st.st_ਬਹੁਲਕ))
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

	ਜਦੋਂ ((iter_ਸਥਿਤੀ = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_ਰਸਤਾ);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_ਰਸਤਾ);

		ਜੇ (S_ISLNK(iter->st.st_ਬਹੁਲਕ))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_ਰਸਤਾ);

		ਜੇ (S_ISDIR(iter->st.st_ਬਹੁਲਕ)) {
			mkdir_if_missing(dest->buf, 0777);
			ਜਾਰੀ;
		}

		/* Alternates were already handled earlier. */
		ਜੇ (!fspathcmp(iter->relative_ਰਸਤਾ, "info/alternates"))
			ਜਾਰੀ;

		ਜੇ (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		ਜੇ (!option_no_hardlinks) {
			ਜੇ (!link(src->buf, dest->buf)) {
				ਬਣਤਰ stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				ਜੇ (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				ਜੇ (st.st_ਬਹੁਲਕ != iter->st.st_ਬਹੁਲਕ ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_ਮਾਪ != iter->st.st_ਮਾਪ ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				ਜਾਰੀ;
			}
			ਜੇ (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		ਜੇ (copy_file_with_ਸਮਾਂ(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	ਜੇ (iter_ਸਥਿਤੀ != ITER_ਪੂਰਾ) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

ਸਥਿਰ ਖਾਲੀ clone_local(ਅਚਲ ਅੱਖਰ *src_repo, ਅਚਲ ਅੱਖਰ *dest_repo)
{
	ਜੇ (!option_shared) {
		ਬਣਤਰ strbuf src = STRBUF_INIT;
		ਬਣਤਰ strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	ਜੇ (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *junk_work_ਰੁੱਖ;
ਸਥਿਰ ਪੂਰਨ junk_work_ਰੁੱਖ_flags;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *junk_git_dir;
ਸਥਿਰ ਪੂਰਨ junk_git_dir_flags;
ਸਥਿਰ ਗਿਣਤੀ {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_ਬਹੁਲਕ = JUNK_LEAVE_NONE;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ junk_leave_repo_ਸੁਨੇਹਾ[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

ਸਥਿਰ ਖਾਲੀ remove_junk(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਚੋਣ (junk_ਬਹੁਲਕ) {
	ਹਾਲਤ JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_ਸੁਨੇਹਾ));
		/* fall-through */
	ਹਾਲਤ JUNK_LEAVE_ALL:
		ਵਾਪਸ;
	ਮੂਲ:
		/* proceed to removal */
		ਰੋਕੋ;
	}

	ਜੇ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	ਜੇ (junk_work_ਰੁੱਖ) {
		strbuf_addstr(&sb, junk_work_ਰੁੱਖ);
		remove_dir_recursively(&sb, junk_work_ਰੁੱਖ_flags);
	}
	strbuf_release(&sb);
}

ਸਥਿਰ ਖਾਲੀ remove_junk_on_signal(ਪੂਰਨ signo)
{
	remove_junk();
	sigchain_ਕੱਢੋ(signo);
	ਉਠਾਓ(signo);
}

ਸਥਿਰ ਬਣਤਰ ref *ਲੱਭੋ_remote_branch(ਅਚਲ ਬਣਤਰ ref *refs, ਅਚਲ ਅੱਖਰ *branch)
{
	ਬਣਤਰ ref *ref;
	ਬਣਤਰ strbuf ਸਿਰ = STRBUF_INIT;
	strbuf_addstr(&ਸਿਰ, "refs/heads/");
	strbuf_addstr(&ਸਿਰ, branch);
	ref = ਲੱਭੋ_ref_by_name(refs, ਸਿਰ.buf);
	strbuf_release(&ਸਿਰ);

	ਜੇ (ref)
		ਵਾਪਸ ref;

	strbuf_addstr(&ਸਿਰ, "refs/tags/");
	strbuf_addstr(&ਸਿਰ, branch);
	ref = ਲੱਭੋ_ref_by_name(refs, ਸਿਰ.buf);
	strbuf_release(&ਸਿਰ);

	ਵਾਪਸ ref;
}

ਸਥਿਰ ਬਣਤਰ ref *wanted_peer_refs(ਬਣਤਰ clone_opts *opts,
				    ਅਚਲ ਬਣਤਰ ref *refs,
				    ਬਣਤਰ refspec *refspec)
{
	ਬਣਤਰ ref *local_refs = ਨਿਰਾ;
	ਬਣਤਰ ref **ਪੂਛ = &local_refs;
	ਬਣਤਰ ref *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;

	ਜੇ (opts->ਚਾਹੁੰਦਾ_ਸਿਰ) {
		ਬਣਤਰ ref *ਸਿਰ = copy_ref(ਲੱਭੋ_ref_by_name(refs, "HEAD"));
		ਜੇ (ਸਿਰ)
			ਪੂਛ_link_ref(ਸਿਰ, &ਪੂਛ);
		ਜੇ (option_single_branch)
			refs = ਮੰਜ਼ਿਲ_free =
				guess_remote_ਸਿਰ(ਸਿਰ, refs,
						  REMOTE_GUESS_ਸਿਰ_QUIET);
	} ਨਹੀਂਤਾਂ ਜੇ (option_single_branch) {
		local_refs = ਨਿਰਾ;
		ਪੂਛ = &local_refs;
		refs = ਮੰਜ਼ਿਲ_free = copy_ref(ਲੱਭੋ_remote_branch(refs, option_branch));
	}

	ਲਈ (ਮਾਪ_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->ਚੀਜ਼ਾਂ[i], &ਪੂਛ, 0);

	free_one_ref(ਮੰਜ਼ਿਲ_free);

	ਵਾਪਸ local_refs;
}

ਸਥਿਰ ਖਾਲੀ write_remote_refs(ਅਚਲ ਬਣਤਰ ref *local_refs)
{
	ਅਚਲ ਬਣਤਰ ref *r;

	ਬਣਤਰ ref_transaction *t;
	ਬਣਤਰ strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	ਜੇ (!t)
		die("%s", err.buf);

	ਲਈ (r = local_refs; r; r = r->ਅਗਲਾ) {
		ਜੇ (!r->peer_ref)
			ਜਾਰੀ;
		ਜੇ (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   ਨਿਰਾ, 0, ਨਿਰਾ, &err))
			die("%s", err.buf);
	}

	ਜੇ (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

ਸਥਿਰ ਖਾਲੀ write_followtags(ਅਚਲ ਬਣਤਰ ref *refs, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ)
{
	ਅਚਲ ਬਣਤਰ ref *ref;
	ਲਈ (ref = refs; ref; ref = ref->ਅਗਲਾ) {
		ਜੇ (!starts_with(ref->name, "refs/tags/"))
			ਜਾਰੀ;
		ਜੇ (ਸਿਰੇ_with(ref->name, "^{}"))
			ਜਾਰੀ;
		ਜੇ (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			ਜਾਰੀ;
		refs_update_ref(get_main_ref_store(the_repository), ਸੁਨੇਹਾ,
				ref->name, &ref->old_oid, ਨਿਰਾ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ object_id *iterate_ref_map(ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ ref **rm = cb_ਅੰਕੜੇ;
	ਬਣਤਰ ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	ਜਦੋਂ (ref && !ref->peer_ref)
		ref = ref->ਅਗਲਾ;
	ਜੇ (!ref)
		ਵਾਪਸ ਨਿਰਾ;

	*rm = ref->ਅਗਲਾ;
	ਵਾਪਸ &ref->old_oid;
}

ਸਥਿਰ ਖਾਲੀ update_remote_refs(ਅਚਲ ਬਣਤਰ ref *refs,
			       ਅਚਲ ਬਣਤਰ ref *mapped_refs,
			       ਅਚਲ ਬਣਤਰ ref *remote_ਸਿਰ_points_at,
			       ਅਚਲ ਅੱਖਰ *branch_ਸਿਖਰ,
			       ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ,
			       ਬਣਤਰ transport *transport,
			       ਪੂਰਨ check_connectivity)
{
	ਅਚਲ ਬਣਤਰ ref *rm = mapped_refs;

	ਜੇ (check_connectivity) {
		ਬਣਤਰ check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.ਤਰੱਕੀ = transport->ਤਰੱਕੀ;

		ਜੇ (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	ਜੇ (refs) {
		write_remote_refs(mapped_refs);
		ਜੇ (option_single_branch && option_ਟੈਗ_ਸੂਚੀ)
			write_followtags(refs, ਸੁਨੇਹਾ);
	}

	ਜੇ (remote_ਸਿਰ_points_at && remote_ਸਿਰ_points_at->peer_ref && !option_bare) {
		ਬਣਤਰ strbuf ਸਿਰ_ref = STRBUF_INIT;
		strbuf_addstr(&ਸਿਰ_ref, branch_ਸਿਖਰ);
		strbuf_addstr(&ਸਿਰ_ref, "HEAD");
		ਜੇ (refs_update_symref(get_main_ref_store(the_repository), ਸਿਰ_ref.buf,
				       remote_ਸਿਰ_points_at->peer_ref->name,
				       ਸੁਨੇਹਾ) < 0)
			die(_("unable to update %s"), ਸਿਰ_ref.buf);
		strbuf_release(&ਸਿਰ_ref);
	}
}

ਸਥਿਰ ਖਾਲੀ update_ਸਿਰ(ਬਣਤਰ clone_opts *opts, ਅਚਲ ਬਣਤਰ ref *our, ਅਚਲ ਬਣਤਰ ref *remote,
			ਅਚਲ ਅੱਖਰ *unborn, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ)
{
	ਅਚਲ ਅੱਖਰ *ਸਿਰ;
	ਜੇ (our && !opts->detach && skip_ਅਗੇਤਰ(our->name, "refs/heads/", &ਸਿਰ)) {
		/* Local default branch link */
		ਜੇ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, ਨਿਰਾ) < 0)
			die(_("unable to update HEAD"));
		ਜੇ (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					ਸੁਨੇਹਾ, "HEAD", &our->old_oid, ਨਿਰਾ, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, ਸਿਰ, remote_name, our->name);
		}
	} ਨਹੀਂਤਾਂ ਜੇ (our) {
		ਬਣਤਰ commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), ਸੁਨੇਹਾ,
				"HEAD", &c->object.oid, ਨਿਰਾ, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ਨਹੀਂਤਾਂ ਜੇ (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), ਸੁਨੇਹਾ,
				"HEAD", &remote->old_oid, ਨਿਰਾ, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ਨਹੀਂਤਾਂ ਜੇ (unborn && skip_ਅਗੇਤਰ(unborn, "refs/heads/", &ਸਿਰ)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		ਜੇ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, ਨਿਰਾ) < 0)
			die(_("unable to update HEAD"));
		ਜੇ (!option_bare)
			install_branch_config(0, ਸਿਰ, remote_name, unborn);
	}
}

ਸਥਿਰ ਪੂਰਨ git_sparse_checkout_init(ਅਚਲ ਅੱਖਰ *repo)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	strvec_pushl(&cmd.ਦਲੀਲਾਂ, "-C", repo, "sparse-checkout", "set", ਨਿਰਾ);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	ਜੇ (ਲੜੀਵਾਰ_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		ਨਤੀਜਾ = 1;
	}

	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ checkout(ਪੂਰਨ submodule_ਤਰੱਕੀ,
		    ਬਣਤਰ list_objects_filter_options *filter_options,
		    ਪੂਰਨ filter_submodules,
		    ਗਿਣਤੀ ref_storage_format ref_storage_format)
{
	ਬਣਤਰ object_id oid;
	ਅੱਖਰ *ਸਿਰ;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਬਣਤਰ unpack_trees_options opts;
	ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;
	ਬਣਤਰ ਰੁੱਖ_desc t;
	ਪੂਰਨ err = 0;
	ਬਣਤਰ ਲੜੀਵਾਰ_hooks_opt hook_opt = ਲੜੀਵਾਰ_HOOKS_OPT_INIT_FORCE_SERIAL;

	ਜੇ (option_no_checkout)
		ਵਾਪਸ 0;

	ਸਿਰ = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, ਨਿਰਾ);
	ਜੇ (!ਸਿਰ) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(ਸਿਰ, "HEAD")) {
		ਜੇ (advice_enabled(ADVICE_DETACHED_ਸਿਰ))
			detach_advice(oid_ਮੰਜ਼ਿਲ_hex(&oid));
		FREE_AND_NULL(ਸਿਰ);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (!starts_with(ਸਿਰ, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);

	repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, ਤਾਲਾ_DIE_ON_ERROR);

	memset(&opts, 0, ਆਕਾਰ opts);
	opts.update = 1;
	opts.ਮਿਲਾਓ = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_ਮਿਲਾਓ;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.dst_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	init_checkout_metadata(&opts.meta, ਸਿਰ, &oid, ਨਿਰਾ);

	ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, &oid);
	ਜੇ (!ਰੁੱਖ)
		die(_("unable to parse commit %s"), oid_ਮੰਜ਼ਿਲ_hex(&oid));
	ਜੇ (repo_parse_ਰੁੱਖ(the_repository, ਰੁੱਖ) < 0)
		ਬਾਹਰ_ਨਿਕਲੋ(128);
	init_ਰੁੱਖ_desc(&t, &ਰੁੱਖ->object.oid, ਰੁੱਖ->ਬਫਰ, ਰੁੱਖ->ਮਾਪ);
	ਜੇ (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	ਮੁਕਤ(ਸਿਰ);

	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file, COMMIT_ਤਾਲਾ))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.ਦਲੀਲਾਂ, oid_ਮੰਜ਼ਿਲ_hex(null_oid(the_hash_algo)),
		     oid_ਮੰਜ਼ਿਲ_hex(&oid), "1", ਨਿਰਾ);
	err |= ਲੜੀਵਾਰ_hooks_opt(the_repository, "post-checkout", &hook_opt);

	ਜੇ (!err && (option_recurse_submodules.nr > 0)) {
		ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;
		strvec_pushl(&cmd.ਦਲੀਲਾਂ, "submodule", "update", "--require-init",
			     "--recursive", ਨਿਰਾ);

		ਜੇ (option_shallow_submodules == 1)
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--depth=1");

		ਜੇ (max_jobs != -1)
			strvec_pushf(&cmd.ਦਲੀਲਾਂ, "--jobs=%d", max_jobs);

		ਜੇ (submodule_ਤਰੱਕੀ)
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--progress");

		ਜੇ (option_verbosity < 0)
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--quiet");

		ਜੇ (option_remote_submodules) {
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--remote");
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--no-fetch");
		}

		ਜੇ (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.ਦਲੀਲਾਂ, "--ref-storage-format=%s",
				     ref_storage_format_ਮੰਜ਼ਿਲ_name(ref_storage_format));

		ਜੇ (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.ਦਲੀਲਾਂ, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		ਜੇ (option_single_branch >= 0)
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = ਲੜੀਵਾਰ_command(&cmd);
	}

	ਵਾਪਸ err;
}

ਸਥਿਰ ਪੂਰਨ git_clone_config(ਅਚਲ ਅੱਖਰ *k, ਅਚਲ ਅੱਖਰ *v,
			    ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(k, "clone.defaultremotename")) {
		ਜੇ (!v)
			ਵਾਪਸ config_error_nonbool(k);
		ਮੁਕਤ(remote_name);
		remote_name = xstrdup(v);
	}
	ਜੇ (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	ਜੇ (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	ਵਾਪਸ git_default_config(k, v, ctx, cb);
}

ਸਥਿਰ ਪੂਰਨ write_one_config(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			    ਅਚਲ ਬਣਤਰ config_context *ctx,
			    ਖਾਲੀ *ਅੰਕੜੇ)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	ਪੂਰਨ apply_failed = git_clone_config(ਕੁੰਜੀ, ਮੁੱਲ, ctx, ਅੰਕੜੇ);
	ਜੇ (apply_failed)
		ਵਾਪਸ apply_failed;

	ਵਾਪਸ repo_config_ਸੈੱਟ_multivar_gently(the_repository, ਕੁੰਜੀ,
					       ਮੁੱਲ ? ਮੁੱਲ : "true",
					       CONFIG_REGEX_NONE, 0);
}

ਸਥਿਰ ਖਾਲੀ write_config(ਬਣਤਰ string_list *config)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < config->nr; i++) {
		ਜੇ (git_config_parse_parameter(config->ਚੀਜ਼ਾਂ[i].string,
					       write_one_config, ਨਿਰਾ) < 0)
			die(_("unable to write parameters to config file"));
	}
}

ਸਥਿਰ ਖਾਲੀ write_refspec_config(ਅਚਲ ਅੱਖਰ *src_ref_ਅਗੇਤਰ,
		ਅਚਲ ਬਣਤਰ ref *our_ਸਿਰ_points_at,
		ਅਚਲ ਬਣਤਰ ref *remote_ਸਿਰ_points_at,
		ਬਣਤਰ strbuf *branch_ਸਿਖਰ)
{
	ਬਣਤਰ strbuf ਕੁੰਜੀ = STRBUF_INIT;
	ਬਣਤਰ strbuf ਮੁੱਲ = STRBUF_INIT;

	ਜੇ (option_mirror || !option_bare) {
		ਜੇ (option_single_branch && !option_mirror) {
			ਜੇ (option_branch) {
				ਜੇ (starts_with(our_ਸਿਰ_points_at->name, "refs/tags/"))
					strbuf_addf(&ਮੁੱਲ, "+%s:%s", our_ਸਿਰ_points_at->name,
						our_ਸਿਰ_points_at->name);
				ਨਹੀਂਤਾਂ
					strbuf_addf(&ਮੁੱਲ, "+%s:%s%s", our_ਸਿਰ_points_at->name,
						branch_ਸਿਖਰ->buf, option_branch);
			} ਨਹੀਂਤਾਂ ਜੇ (remote_ਸਿਰ_points_at) {
				ਅਚਲ ਅੱਖਰ *ਸਿਰ = remote_ਸਿਰ_points_at->name;
				ਜੇ (!skip_ਅਗੇਤਰ(ਸਿਰ, "refs/heads/", &ਸਿਰ))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&ਮੁੱਲ, "+%s:%s%s", remote_ਸਿਰ_points_at->name,
						branch_ਸਿਖਰ->buf, ਸਿਰ);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} ਨਹੀਂਤਾਂ {
			strbuf_addf(&ਮੁੱਲ, "+%s*:%s*", src_ref_ਅਗੇਤਰ, branch_ਸਿਖਰ->buf);
		}
		/* Configure the remote */
		ਜੇ (ਮੁੱਲ.len) {
			strbuf_addf(&ਕੁੰਜੀ, "remote.%s.fetch", remote_name);
			repo_config_ਸੈੱਟ_multivar(the_repository, ਕੁੰਜੀ.buf, ਮੁੱਲ.buf, "^$", 0);
			strbuf_reset(&ਕੁੰਜੀ);

			ਜੇ (option_mirror) {
				strbuf_addf(&ਕੁੰਜੀ, "remote.%s.mirror", remote_name);
				repo_config_ਸੈੱਟ(the_repository, ਕੁੰਜੀ.buf, "true");
				strbuf_reset(&ਕੁੰਜੀ);
			}
		}
	}

	strbuf_release(&ਕੁੰਜੀ);
	strbuf_release(&ਮੁੱਲ);
}

ਸਥਿਰ ਖਾਲੀ dissociate_from_references(ਖਾਲੀ)
{
	ਅੱਖਰ *alternates = repo_git_ਰਸਤਾ(the_repository, "objects/info/alternates");

	ਜੇ (!access(alternates, F_OK)) {
		ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.ਦਲੀਲਾਂ, "repack", "-a", "-d", ਨਿਰਾ);
		ਜੇ (ਲੜੀਵਾਰ_command(&cmd))
			die(_("cannot repack to clean up"));
		ਜੇ (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	ਮੁਕਤ(alternates);
}

ਸਥਿਰ ਪੂਰਨ ਰਸਤਾ_exists(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ stat sb;
	ਵਾਪਸ !stat(ਰਸਤਾ, &sb);
}

ਪੂਰਨ cmd_clone(ਪੂਰਨ argc,
	      ਅਚਲ ਅੱਖਰ **argv,
	      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repository ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ is_bundle = 0, is_local;
	ਪੂਰਨ reject_shallow = 0;
	ਅਚਲ ਅੱਖਰ *repo_name, *repo, *work_ਰੁੱਖ, *git_dir;
	ਅੱਖਰ *repo_ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ, *dir, *display_repo = ਨਿਰਾ;
	ਪੂਰਨ dest_exists, real_dest_exists = 0;
	ਅਚਲ ਬਣਤਰ ref *refs, *remote_ਸਿਰ;
	ਬਣਤਰ ref *remote_ਸਿਰ_points_at = ਨਿਰਾ;
	ਅਚਲ ਬਣਤਰ ref *our_ਸਿਰ_points_at;
	ਅੱਖਰ *unborn_ਸਿਰ = ਨਿਰਾ;
	ਬਣਤਰ ref *mapped_refs = ਨਿਰਾ;
	ਅਚਲ ਬਣਤਰ ref *ref;
	ਬਣਤਰ strbuf ਕੁੰਜੀ = STRBUF_INIT;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ strbuf branch_ਸਿਖਰ = STRBUF_INIT, reflog_ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ transport *transport = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *src_ref_ਅਗੇਤਰ = "refs/heads/";
	ਬਣਤਰ remote *remote;
	ਪੂਰਨ err = 0, complete_refs_before_fetch = 1;
	ਪੂਰਨ submodule_ਤਰੱਕੀ;
	ਪੂਰਨ filter_submodules = 0;
	ਪੂਰਨ hash_algo;
	ਗਿਣਤੀ ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	ਅਚਲ ਪੂਰਨ do_not_override_repo_unix_permissions = -1;
	ਪੂਰਨ option_reject_shallow = -1; /* unspecified */
	ਪੂਰਨ deepen = 0;
	ਅੱਖਰ *option_template = ਨਿਰਾ, *option_ਡੂੰਘਾਈ = ਨਿਰਾ, *option_since = ਨਿਰਾ;
	ਅੱਖਰ *option_origin = ਨਿਰਾ;
	ਬਣਤਰ string_list option_not = STRING_LIST_INIT_NODUP;
	ਅਚਲ ਅੱਖਰ *real_git_dir = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ref_storage_format_uri = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *option_upload_pack = "git-upload-pack";
	ਪੂਰਨ option_ਤਰੱਕੀ = -1;
	ਪੂਰਨ option_sparse_checkout = 0;
	ਗਿਣਤੀ transport_family family = TRANSPORT_FAMILY_ALL;
	ਬਣਤਰ string_list option_config = STRING_LIST_INIT_DUP;
	ਪੂਰਨ option_dissociate = 0;
	ਬਣਤਰ list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	ਪੂਰਨ option_filter_submodules = -1; /* unspecified */
	ਬਣਤਰ string_list server_options = STRING_LIST_INIT_NODUP;
	ਅਚਲ ਅੱਖਰ *bundle_uri = ਨਿਰਾ;
	ਅੱਖਰ *option_rev = ਨਿਰਾ;

	ਬਣਤਰ clone_opts opts = CLONE_OPTS_INIT;

	ਬਣਤਰ transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	ਬਣਤਰ option builtin_clone_options[] = {
		OPT__VERBOSITY(&option_verbosity),
		OPT_BOOL(0, "progress", &option_ਤਰੱਕੀ,
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
			.ਮੁੱਲ = &option_recurse_submodules,
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
		OPT_STRING(0, "depth", &option_ਡੂੰਘਾਈ, N_("depth"),
			   N_("create a shallow clone of that depth")),
		OPT_STRING(0, "shallow-since", &option_since, N_("time"),
			   N_("create a shallow clone since a specific time")),
		OPT_STRING_LIST(0, "shallow-exclude", &option_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_BOOL(0, "single-branch", &option_single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_BOOL(0, "tags", &option_ਟੈਗ_ਸੂਚੀ,
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
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ strvec alternates = STRVEC_INIT;

	ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		ਨਿਰਾ
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_ਪਛਾਣ_ਤੱਤ("clone");

	repo_config(the_repository, git_clone_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_clone_options,
			     builtin_clone_usage, 0);

	ਜੇ (argc > 2)
		usage_ਸੁਨੇਹਾ_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	ਜੇ (argc == 0)
		usage_ਸੁਨੇਹਾ_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	ਜੇ (option_ਡੂੰਘਾਈ || option_since || option_not.nr)
		deepen = 1;
	ਜੇ (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	ਜੇ (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, ਨਿਰਾ);
		ਜੇ (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	ਜੇ (option_mirror) {
		option_bare = 1;
		option_ਟੈਗ_ਸੂਚੀ = 0;
	}

	ਜੇ (option_bare) {
		ਜੇ (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	ਜੇ (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	ਰਸਤਾ = get_repo_ਰਸਤਾ(repo_name, &is_bundle);
	ਜੇ (ਰਸਤਾ) {
		FREE_AND_NULL(ਰਸਤਾ);
		repo = repo_ਮੰਜ਼ਿਲ_free = absolute_pathdup(repo_name);
	} ਨਹੀਂਤਾਂ ਜੇ (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} ਨਹੀਂਤਾਂ
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	ਜੇ (option_ਡੂੰਘਾਈ && atoi(option_ਡੂੰਘਾਈ) < 1)
		die(_("depth %s is not a positive number"), option_ਡੂੰਘਾਈ);

	ਜੇ (argc == 2)
		dir = xstrdup(argv[1]);
	ਨਹੀਂਤਾਂ
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = ਰਸਤਾ_exists(dir);
	ਜੇ (dest_exists && !is_ਖਾਲੀ_ਥਾਵਾਂ_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	ਜੇ (real_git_dir) {
		real_dest_exists = ਰਸਤਾ_exists(real_git_dir);
		ਜੇ (real_dest_exists && !is_ਖਾਲੀ_ਥਾਵਾਂ_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_ਸੁਨੇਹਾ, "clone: from %s",
		    display_repo ? display_repo : repo);
	ਮੁਕਤ(display_repo);

	ਜੇ (option_bare)
		work_ਰੁੱਖ = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ {
		work_ਰੁੱਖ = getenv("GIT_WORK_TREE");
		ਜੇ (work_ਰੁੱਖ && ਰਸਤਾ_exists(work_ਰੁੱਖ))
			die(_("working tree '%s' already exists."), work_ਰੁੱਖ);
	}

	ਜੇ (option_bare || work_ਰੁੱਖ)
		git_dir = xstrdup(dir);
	ਨਹੀਂਤਾਂ {
		work_ਰੁੱਖ = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_ਧੱਕੋ_common(remove_junk_on_signal);

	ਜੇ (!option_bare) {
		ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories_const(the_repository, work_ਰੁੱਖ) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_ਰੁੱਖ);
		ਜੇ (dest_exists)
			junk_work_ਰੁੱਖ_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		ਨਹੀਂਤਾਂ ਜੇ (mkdir(work_ਰੁੱਖ, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_ਰੁੱਖ);
		junk_work_ਰੁੱਖ = work_ਰੁੱਖ;
	}

	ਜੇ (real_git_dir) {
		ਜੇ (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	ਜੇ (0 <= option_verbosity) {
		ਜੇ (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		ਨਹੀਂਤਾਂ
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	ਜੇ (option_recurse_submodules.nr > 0) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
		ਬਣਤਰ strbuf sb = STRBUF_INIT;
		ਪੂਰਨ val;

		/* remove duplicates */
		string_list_ਛਾਂਟੋ_u(&option_recurse_submodules, 0);

		/*
		 * NEEDSWORK: In a multi-working-tree world, this needs to be
		 * set in the per-worktree config.
		 */
		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &option_recurse_submodules) {
			strbuf_addf(&sb, "submodule.active=%s",
				    ਵਸਤੂ->string);
			string_list_append(&option_config, sb.buf);
			strbuf_reset(&sb);
		}

		ਜੇ (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		ਜੇ (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		ਨਹੀਂਤਾਂ ਜੇ (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} ਨਹੀਂਤਾਂ ਜੇ (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_ਰੁੱਖ,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, ਨਿਰਾ);

	ਜੇ (real_git_dir) {
		ਮੁਕਤ((ਅੱਖਰ *)git_dir);
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
	initialize_repository_ਸੰਸਕਰਣ(the_repository, GIT_HASH_UNKNOWN,
				      the_repository->ref_storage_format, 1);

	refs_create_refdir_stubs(the_repository, git_dir, ਨਿਰਾ);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, ਨਿਰਾ);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	ਜੇ (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	ਜੇ (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	ਜੇ (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	ਜੇ (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	ਜੇ (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	ਜੇ (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	ਜੇ (option_origin) {
		ਮੁਕਤ(remote_name);
		remote_name = xstrdup(option_origin);
	}

	ਜੇ (!remote_name)
		remote_name = xstrdup("origin");

	ਜੇ (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	ਜੇ (option_bare) {
		ਜੇ (option_mirror)
			src_ref_ਅਗੇਤਰ = "refs/";
		strbuf_addstr(&branch_ਸਿਖਰ, src_ref_ਅਗੇਤਰ);

		repo_config_ਸੈੱਟ(the_repository, "core.bare", "true");
	} ਨਹੀਂਤਾਂ ਜੇ (!option_rev) {
		strbuf_addf(&branch_ਸਿਖਰ, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&ਕੁੰਜੀ, "remote.%s.url", remote_name);
	repo_config_ਸੈੱਟ(the_repository, ਕੁੰਜੀ.buf, repo);
	strbuf_reset(&ਕੁੰਜੀ);

	ਜੇ (!option_ਟੈਗ_ਸੂਚੀ) {
		strbuf_addf(&ਕੁੰਜੀ, "remote.%s.tagOpt", remote_name);
		repo_config_ਸੈੱਟ(the_repository, ਕੁੰਜੀ.buf, "--no-tags");
		strbuf_reset(&ਕੁੰਜੀ);
	}

	remote = remote_get_early(remote_name);

	ਜੇ (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_ਅਗੇਤਰ,
				branch_ਸਿਖਰ.buf);

	ਰਸਤਾ = get_repo_ਰਸਤਾ(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && ਰਸਤਾ && !is_bundle;
	ਜੇ (is_local) {
		ਜੇ (option_ਡੂੰਘਾਈ)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		ਜੇ (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		ਜੇ (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		ਜੇ (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		ਜੇ (!access(mkpath("%s/shallow", ਰਸਤਾ), F_OK)) {
			ਜੇ (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			ਜੇ (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	ਜੇ (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? ਰਸਤਾ : ਨਿਰਾ);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, ਰਸਤਾ ? ਰਸਤਾ : remote->url.v[0]);
	transport_ਸੈੱਟ_verbosity(transport, option_verbosity, option_ਤਰੱਕੀ);
	transport->family = family;
	transport->cloning = 1;

	ਜੇ (is_bundle) {
		ਬਣਤਰ bundle_ਸਿਰਲੇਖ ਸਿਰਲੇਖ = BUNDLE_ਸਿਰਲੇਖ_INIT;
		ਪੂਰਨ fd = read_bundle_ਸਿਰਲੇਖ(ਰਸਤਾ, &ਸਿਰਲੇਖ);
		ਪੂਰਨ has_filter = ਸਿਰਲੇਖ.filter.choice != LOFC_DISABLED;

		ਜੇ (fd > 0)
			close(fd);
		bundle_ਸਿਰਲੇਖ_release(&ਸਿਰਲੇਖ);
		ਜੇ (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_ਸੈੱਟ_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	ਜੇ (reject_shallow)
		transport_ਸੈੱਟ_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	ਜੇ (option_ਡੂੰਘਾਈ)
		transport_ਸੈੱਟ_option(transport, TRANS_OPT_ਡੂੰਘਾਈ,
				     option_ਡੂੰਘਾਈ);
	ਜੇ (option_since)
		transport_ਸੈੱਟ_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	ਜੇ (option_not.nr)
		transport_ਸੈੱਟ_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (ਅਚਲ ਅੱਖਰ *)&option_not);
	ਜੇ (option_single_branch) {
		transport_ਸੈੱਟ_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		ਜੇ (option_branch)
			opts.ਚਾਹੁੰਦਾ_ਸਿਰ = 0;
	}

	ਜੇ (option_upload_pack)
		transport_ਸੈੱਟ_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	ਜੇ (server_options.nr)
		transport->server_options = &server_options;

	ਜੇ (filter_options.choice) {
		ਅਚਲ ਅੱਖਰ *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_ਸੈੱਟ_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_ਸੈੱਟ_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	ਜੇ (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	ਜੇ (option_rev) {
		option_ਟੈਗ_ਸੂਚੀ = 0;
		option_single_branch = 0;
		opts.ਚਾਹੁੰਦਾ_ਸਿਰ = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	ਜੇ (option_ਟੈਗ_ਸੂਚੀ || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, ਟੈਗ_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	ਜੇ (option_branch)
		expand_ref_ਅਗੇਤਰ(&transport_ls_refs_options.ref_prefixes,
				  option_branch);

	/*
	 * As part of transport_get_remote_refs() the server tells us the hash
	 * algorithm, which we require to initialize the repo. But calling that
	 * function without any ref prefix, will cause the server to announce
	 * all known refs. If the argument passed to --revision was a hex oid,
	 * ref_prefixes will be empty so we fall back to asking about HEAD to
	 * reduce traffic from the server.
	 */
	ਜੇ (opts.ਚਾਹੁੰਦਾ_ਸਿਰ || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_ਧੱਕੋ(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_ਸੰਸਕਰਣ(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_ਸੈੱਟ_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, ਨਿਰਾ, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	ਜੇ (bundle_uri) {
		ਬਣਤਰ remote_state *state;
		ਪੂਰਨ has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = ਨਿਰਾ;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		ਜੇ (repo_init(the_repository, git_dir, work_ਰੁੱਖ))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		ਨਹੀਂਤਾਂ ਜੇ (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		ਨਹੀਂਤਾਂ ਜੇ (has_heuristic)
			repo_config_ਸੈੱਟ_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		ਮੁਕਤ(the_repository->remote_state);
		the_repository->remote_state = state;
	} ਨਹੀਂਤਾਂ {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		ਜੇ (transport->bundles &&
		    hashmap_get_ਮਾਪ(&transport->bundles->bundles)) {
			ਬਣਤਰ remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = ਨਿਰਾ;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			ਜੇ (repo_init(the_repository, git_dir, work_ਰੁੱਖ))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			ਨਹੀਂਤਾਂ ਜੇ (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			ਮੁਕਤ(the_repository->remote_state);
			the_repository->remote_state = state;
		} ਨਹੀਂਤਾਂ {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	ਜੇ (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	ਜੇ (mapped_refs) {
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
		ਲਈ (ref = refs; ref; ref = ref->ਅਗਲਾ)
			ਜੇ (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				ਰੋਕੋ;
			}

		ਜੇ (!is_local && !complete_refs_before_fetch) {
			ਜੇ (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_ਸਿਰ = ਲੱਭੋ_ref_by_name(refs, "HEAD");
	remote_ਸਿਰ_points_at = guess_remote_ਸਿਰ(remote_ਸਿਰ, mapped_refs,
						  REMOTE_GUESS_ਸਿਰ_QUIET);

	ਜੇ (option_branch) {
		our_ਸਿਰ_points_at = ਲੱਭੋ_remote_branch(mapped_refs, option_branch);
		ਜੇ (!our_ਸਿਰ_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} ਨਹੀਂਤਾਂ ਜੇ (option_rev) {
		our_ਸਿਰ_points_at = mapped_refs;
		ਜੇ (!our_ਸਿਰ_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} ਨਹੀਂਤਾਂ ਜੇ (remote_ਸਿਰ_points_at) {
		our_ਸਿਰ_points_at = remote_ਸਿਰ_points_at;
	} ਨਹੀਂਤਾਂ ਜੇ (remote_ਸਿਰ) {
		our_ਸਿਰ_points_at = ਨਿਰਾ;
	} ਨਹੀਂਤਾਂ {
		ਅੱਖਰ *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
		ਅਚਲ ਅੱਖਰ *branch;

		ਜੇ (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		ਜੇ (transport_ls_refs_options.unborn_ਸਿਰ_ਨਿਸ਼ਾਨਾ &&
		    skip_ਅਗੇਤਰ(transport_ls_refs_options.unborn_ਸਿਰ_ਨਿਸ਼ਾਨਾ,
				"refs/heads/", &branch)) {
			unborn_ਸਿਰ  = xstrdup(transport_ls_refs_options.unborn_ਸਿਰ_ਨਿਸ਼ਾਨਾ);
		} ਨਹੀਂਤਾਂ {
			branch = ਮੰਜ਼ਿਲ_free = repo_default_branch_name(the_repository, 0);
			unborn_ਸਿਰ = xstrfmt("refs/heads/%s", branch);
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
		our_ਸਿਰ_points_at = ਲੱਭੋ_remote_branch(mapped_refs, branch);

		ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
	}

	ਜੇ (!option_rev)
		write_refspec_config(src_ref_ਅਗੇਤਰ, our_ਸਿਰ_points_at,
				     remote_ਸਿਰ_points_at, &branch_ਸਿਖਰ);

	ਜੇ (filter_options.choice)
		partial_clone_ਰਜਿਸਟਰ(remote_name, &filter_options);

	ਜੇ (is_local)
		clone_local(ਰਸਤਾ, git_dir);
	ਨਹੀਂਤਾਂ ਜੇ (mapped_refs && complete_refs_before_fetch) {
		ਜੇ (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_ਸਿਰ_points_at,
			   branch_ਸਿਖਰ.buf, reflog_ਸੁਨੇਹਾ.buf, transport,
			   !is_local);

	update_ਸਿਰ(&opts, our_ਸਿਰ_points_at, remote_ਸਿਰ, unborn_ਸਿਰ, reflog_ਸੁਨੇਹਾ.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_ਤਰੱਕੀ = transport->ਤਰੱਕੀ;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	ਜੇ (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	ਜੇ (option_sparse_checkout && git_sparse_checkout_init(dir))
		ਵਾਪਸ 1;

	junk_ਬਹੁਲਕ = JUNK_LEAVE_REPO;
	err = checkout(submodule_ਤਰੱਕੀ,
		       &filter_options,
		       filter_submodules,
		       ref_storage_format);

	list_objects_filter_release(&filter_options);

	string_list_clear(&option_not, 0);
	string_list_clear(&option_config, 0);
	string_list_clear(&server_options, 0);
	strvec_clear(&alternates);

	ਮੁਕਤ(remote_name);
	strbuf_release(&reflog_ਸੁਨੇਹਾ);
	strbuf_release(&branch_ਸਿਖਰ);
	strbuf_release(&buf);
	strbuf_release(&ਕੁੰਜੀ);
	free_refs(mapped_refs);
	free_refs(remote_ਸਿਰ_points_at);
	ਮੁਕਤ(unborn_ਸਿਰ);
	ਮੁਕਤ(dir);
	ਮੁਕਤ(ਰਸਤਾ);
	ਮੁਕਤ(repo_ਮੰਜ਼ਿਲ_free);
	junk_ਬਹੁਲਕ = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	ਵਾਪਸ err;
}
