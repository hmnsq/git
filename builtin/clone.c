/*
 * Builtin "git clone"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *		 2008 Daniel Barkalow <barkalow@iabervon.org>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 *
 * Clone a repository into a different directory that does not yet exist.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

ರಚನೆ clone_opts {
	ಪೂರ್ಣಾಂಕ ಬಯಸುತ್ತದೆ_ತಲೆ;
	ಪೂರ್ಣಾಂಕ detach;
};
#ವ್ಯಾಖ್ಯಾನಿಸು CLONE_OPTS_INIT { \
	.ಬಯಸುತ್ತದೆ_ತಲೆ = 1 /* default enabled */ \
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_local = -1, option_no_hardlinks, option_shared;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_ಟ್ಯಾಗುಗಳು = 1; /* default enabled */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_shallow_submodules;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ config_reject_shallow = -1;    /* unspecified */
ಸ್ಥಿರ ಅಕ್ಷರ *remote_name = ಖಾಲಿ;
ಸ್ಥಿರ ಅಕ್ಷರ *option_branch = ಖಾಲಿ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_verbosity;
ಸ್ಥಿರ ರಚನೆ string_list option_required_reference = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ರಚನೆ string_list option_optional_reference = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ max_jobs = -1;
ಸ್ಥಿರ ರಚನೆ string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ config_filter_submodules = -1;    /* unspecified */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_remote_submodules;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ recurse_submodules_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಆದರೆ (unset)
		string_list_clear((ರಚನೆ string_list *)opt->ಮೌಲ್ಯ, 0);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (arg)
		string_list_append((ರಚನೆ string_list *)opt->ಮೌಲ್ಯ, arg);
	ಇಲ್ಲದಿದ್ದರೆ
		string_list_append((ರಚನೆ string_list *)opt->ಮೌಲ್ಯ,
				   (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)opt->defval);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_repo_ಮಾರ್ಗ_1(ರಚನೆ strbuf *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ *is_bundle)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix[] = { "/.git", "", ".git/.git", ".git" };
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle_suffix[] = { ".bundle", "" };
	ಗಾತ್ರ_t baselen = ಮಾರ್ಗ->len;
	ರಚನೆ stat st;
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < ARRAY_ಗಾತ್ರ(suffix); i++) {
		strbuf_setlen(ಮಾರ್ಗ, baselen);
		strbuf_addstr(ಮಾರ್ಗ, suffix[i]);
		ಆದರೆ (stat(ಮಾರ್ಗ->buf, &st))
			ಮುಂದುವರಿ;
		ಆದರೆ (S_ISDIR(st.st_ಬಹುಲಕ) && is_git_directory(ಮಾರ್ಗ->buf)) {
			*is_bundle = 0;
			ಹಿಂತಿರುಗಿ ಮಾರ್ಗ->buf;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISREG(st.st_ಬಹುಲಕ) && st.st_ಗಾತ್ರ > 8) {
			/* Is it a "gitfile"? */
			ಅಕ್ಷರ signature[8];
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dst;
			ಪೂರ್ಣಾಂಕ len, fd = open(ಮಾರ್ಗ->buf, O_RDONLY);
			ಆದರೆ (fd < 0)
				ಮುಂದುವರಿ;
			len = read_ಒಳ_ತುಂಬಿದ(fd, signature, 8);
			close(fd);
			ಆದರೆ (len != 8 || ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(signature, "gitdir: ", 8))
				ಮುಂದುವರಿ;
			dst = read_gitfile(ಮಾರ್ಗ->buf);
			ಆದರೆ (dst) {
				*is_bundle = 0;
				ಹಿಂತಿರುಗಿ dst;
			}
		}
	}

	ಗಾಗಿ (i = 0; i < ARRAY_ಗಾತ್ರ(bundle_suffix); i++) {
		strbuf_setlen(ಮಾರ್ಗ, baselen);
		strbuf_addstr(ಮಾರ್ಗ, bundle_suffix[i]);
		ಆದರೆ (!stat(ಮಾರ್ಗ->buf, &st) && S_ISREG(st.st_ಬಹುಲಕ)) {
			*is_bundle = 1;
			ಹಿಂತಿರುಗಿ ಮಾರ್ಗ->buf;
		}
	}

	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಅಕ್ಷರ *get_repo_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *repo, ಪೂರ್ಣಾಂಕ *is_bundle)
{
	ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *raw;
	ಅಕ್ಷರ *canon;

	strbuf_addstr(&ಮಾರ್ಗ, repo);
	raw = get_repo_ಮಾರ್ಗ_1(&ಮಾರ್ಗ, is_bundle);
	canon = raw ? absolute_pathdup(raw) : ಖಾಲಿ;
	strbuf_release(&ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ canon;
}

ರಚನೆ collect_alternates_ದತ್ತಾಂಶ {
	ರಚನೆ strvec *alternates;
	bool required;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ collect_one_alternate(ರಚನೆ string_list_ಅಂಶ *ಅಂಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ collect_alternates_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಅಕ್ಷರ *ref_git = compute_alternate_ಮಾರ್ಗ(ಅಂಶ->string, &err);

	ಆದರೆ (!ref_git) {
		ಆದರೆ (ದತ್ತಾಂಶ->required)
			die("%s", err.buf);
		ಇಲ್ಲದಿದ್ದರೆ
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				ಅಂಶ->string, err.buf);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strvec_pushf(ದತ್ತಾಂಶ->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	ಬಿಡುಗಡೆ(ref_git);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_alternates(ರಚನೆ strvec *alternates, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src_repo)
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
	FILE *ಒಳ;
	ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;
	ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;

	strbuf_addf(&ಮಾರ್ಗ, "%s/objects/info/alternates", src_repo);

	ಒಳ = fopen(ಮಾರ್ಗ.buf, "r");
	ಆದರೆ (!ಒಳ) {
		ಆದರೆ (errno == ENOENT)
			ಹೋಗು ಹೊರಹೋಗು;
		die_errno("could not read alternates file '%s'", ಮಾರ್ಗ.buf);
	}

	ಆಗ (strbuf_getline(&ಸಾಲು, ಒಳ) != EOF) {
		ಅಕ್ಷರ *abs_ಮಾರ್ಗ;
		ಆದರೆ (!ಸಾಲು.len || ಸಾಲು.buf[0] == '#')
			ಮುಂದುವರಿ;
		ಆದರೆ (is_absolute_ಮಾರ್ಗ(ಸಾಲು.buf)) {
			strvec_ತಳ್ಳು(alternates, ಸಾಲು.buf);
			ಮುಂದುವರಿ;
		}
		abs_ಮಾರ್ಗ = mkpathdup("%s/objects/%s", src_repo, ಸಾಲು.buf);
		ಆದರೆ (!normalize_ಮಾರ್ಗ_copy(abs_ಮಾರ್ಗ, abs_ಮಾರ್ಗ))
			strvec_ತಳ್ಳು(alternates, abs_ಮಾರ್ಗ);
		ಇಲ್ಲದಿದ್ದರೆ
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, ಸಾಲು.buf);
		ಬಿಡುಗಡೆ(abs_ಮಾರ್ಗ);
	}

ಹೊರಹೋಗು:
	strbuf_release(&ಮಾರ್ಗ);
	strbuf_release(&ಸಾಲು);
	ಆದರೆ (ಒಳ)
		fclose(ಒಳ);
}

ಸ್ಥಿರ ಶೂನ್ಯ collect_alternates(ರಚನೆ strvec *alternates,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *local_ಮೂಲ_repo)
{
	ಆದರೆ (option_required_reference.nr || option_optional_reference.nr) {
		ರಚನೆ collect_alternates_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &ದತ್ತಾಂಶ);
		ದತ್ತಾಂಶ.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &ದತ್ತಾಂಶ);
	}

	ಆದರೆ (local_ಮೂಲ_repo) {
		ರಚನೆ strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_ಮೂಲ_repo);
		ಆದರೆ (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		ಇಲ್ಲದಿದ್ದರೆ
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ mkdir_if_missing(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಬಹುಲಕ_t ಬಹುಲಕ)
{
	ರಚನೆ stat st;

	ಆದರೆ (!mkdir(pathname, ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!S_ISDIR(st.st_ಬಹುಲಕ))
		die(_("%s exists and is not a directory"), pathname);
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_or_link_directory(ರಚನೆ strbuf *src, ರಚನೆ strbuf *dest,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src_repo)
{
	ಪೂರ್ಣಾಂಕ src_len, dest_len;
	ರಚನೆ dir_iterator *iter;
	ಪೂರ್ಣಾಂಕ iter_ಸ್ಥಿತಿ;

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
	die_upon_dubious_ownership(ಖಾಲಿ, ಖಾಲಿ, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	ಆದರೆ (!iter) {
		ಆದರೆ (errno == ENOTDIR) {
			ಪೂರ್ಣಾಂಕ saved_errno = errno;
			ರಚನೆ stat st;

			ಆದರೆ (!lstat(src->buf, &st) && S_ISLNK(st.st_ಬಹುಲಕ))
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

	ಆಗ ((iter_ಸ್ಥಿತಿ = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_ಮಾರ್ಗ);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_ಮಾರ್ಗ);

		ಆದರೆ (S_ISLNK(iter->st.st_ಬಹುಲಕ))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_ಮಾರ್ಗ);

		ಆದರೆ (S_ISDIR(iter->st.st_ಬಹುಲಕ)) {
			mkdir_if_missing(dest->buf, 0777);
			ಮುಂದುವರಿ;
		}

		/* Alternates were already handled earlier. */
		ಆದರೆ (!fspathcmp(iter->relative_ಮಾರ್ಗ, "info/alternates"))
			ಮುಂದುವರಿ;

		ಆದರೆ (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		ಆದರೆ (!option_no_hardlinks) {
			ಆದರೆ (!link(src->buf, dest->buf)) {
				ರಚನೆ stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				ಆದರೆ (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				ಆದರೆ (st.st_ಬಹುಲಕ != iter->st.st_ಬಹುಲಕ ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_ಗಾತ್ರ != iter->st.st_ಗಾತ್ರ ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				ಮುಂದುವರಿ;
			}
			ಆದರೆ (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		ಆದರೆ (copy_file_with_ಸಮಯ(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	ಆದರೆ (iter_ಸ್ಥಿತಿ != ITER_ಮುಗಿದ) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

ಸ್ಥಿರ ಶೂನ್ಯ clone_local(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src_repo, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dest_repo)
{
	ಆದರೆ (!option_shared) {
		ರಚನೆ strbuf src = STRBUF_INIT;
		ರಚನೆ strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	ಆದರೆ (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *junk_work_ವೃಕ್ಷ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ junk_work_ವೃಕ್ಷ_flags;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *junk_git_dir;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ junk_git_dir_flags;
ಸ್ಥಿರ ಎಣಿಕೆ {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_ಬಹುಲಕ = JUNK_LEAVE_NONE;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ junk_leave_repo_ಸಂದೇಶ[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

ಸ್ಥಿರ ಶೂನ್ಯ remove_junk(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಆಯ್ಕೆ (junk_ಬಹುಲಕ) {
	ಸ್ಥಿತಿ JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_ಸಂದೇಶ));
		/* fall-through */
	ಸ್ಥಿತಿ JUNK_LEAVE_ALL:
		ಹಿಂತಿರುಗಿ;
	ಪೂರ್ವನಿಯೋಜಿತ:
		/* proceed to removal */
		ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	ಆದರೆ (junk_work_ವೃಕ್ಷ) {
		strbuf_addstr(&sb, junk_work_ವೃಕ್ಷ);
		remove_dir_recursively(&sb, junk_work_ವೃಕ್ಷ_flags);
	}
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_junk_on_signal(ಪೂರ್ಣಾಂಕ signo)
{
	remove_junk();
	sigchain_ತೆಗೆ(signo);
	ಎತ್ತು(signo);
}

ಸ್ಥಿರ ರಚನೆ ref *ಕಂಡುಹಿಡಿ_remote_branch(ಸ್ಥಿರಾಂಕ ರಚನೆ ref *refs, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch)
{
	ರಚನೆ ref *ref;
	ರಚನೆ strbuf ತಲೆ = STRBUF_INIT;
	strbuf_addstr(&ತಲೆ, "refs/heads/");
	strbuf_addstr(&ತಲೆ, branch);
	ref = ಕಂಡುಹಿಡಿ_ref_by_name(refs, ತಲೆ.buf);
	strbuf_release(&ತಲೆ);

	ಆದರೆ (ref)
		ಹಿಂತಿರುಗಿ ref;

	strbuf_addstr(&ತಲೆ, "refs/tags/");
	strbuf_addstr(&ತಲೆ, branch);
	ref = ಕಂಡುಹಿಡಿ_ref_by_name(refs, ತಲೆ.buf);
	strbuf_release(&ತಲೆ);

	ಹಿಂತಿರುಗಿ ref;
}

ಸ್ಥಿರ ರಚನೆ ref *wanted_peer_refs(ರಚನೆ clone_opts *opts,
				    ಸ್ಥಿರಾಂಕ ರಚನೆ ref *refs,
				    ರಚನೆ refspec *refspec)
{
	ರಚನೆ ref *local_refs = ಖಾಲಿ;
	ರಚನೆ ref **ಬಾಲ = &local_refs;
	ರಚನೆ ref *ಗಮ್ಯ_free = ಖಾಲಿ;

	ಆದರೆ (opts->ಬಯಸುತ್ತದೆ_ತಲೆ) {
		ರಚನೆ ref *ತಲೆ = copy_ref(ಕಂಡುಹಿಡಿ_ref_by_name(refs, "HEAD"));
		ಆದರೆ (ತಲೆ)
			ಬಾಲ_link_ref(ತಲೆ, &ಬಾಲ);
		ಆದರೆ (option_single_branch)
			refs = ಗಮ್ಯ_free =
				guess_remote_ತಲೆ(ತಲೆ, refs,
						  REMOTE_GUESS_ತಲೆ_QUIET);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (option_single_branch) {
		local_refs = ಖಾಲಿ;
		ಬಾಲ = &local_refs;
		refs = ಗಮ್ಯ_free = copy_ref(ಕಂಡುಹಿಡಿ_remote_branch(refs, option_branch));
	}

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->ವಸ್ತುಗಳು[i], &ಬಾಲ, 0);

	free_one_ref(ಗಮ್ಯ_free);

	ಹಿಂತಿರುಗಿ local_refs;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_remote_refs(ಸ್ಥಿರಾಂಕ ರಚನೆ ref *local_refs)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref *r;

	ರಚನೆ ref_transaction *t;
	ರಚನೆ strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	ಆದರೆ (!t)
		die("%s", err.buf);

	ಗಾಗಿ (r = local_refs; r; r = r->ಮುಂದಿನ) {
		ಆದರೆ (!r->peer_ref)
			ಮುಂದುವರಿ;
		ಆದರೆ (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   ಖಾಲಿ, 0, ಖಾಲಿ, &err))
			die("%s", err.buf);
	}

	ಆದರೆ (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_followtags(ಸ್ಥಿರಾಂಕ ರಚನೆ ref *refs, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref *ref;
	ಗಾಗಿ (ref = refs; ref; ref = ref->ಮುಂದಿನ) {
		ಆದರೆ (!starts_with(ref->name, "refs/tags/"))
			ಮುಂದುವರಿ;
		ಆದರೆ (ತುದಿಗಳು_with(ref->name, "^{}"))
			ಮುಂದುವರಿ;
		ಆದರೆ (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			ಮುಂದುವರಿ;
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ,
				ref->name, &ref->old_oid, ಖಾಲಿ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *iterate_ref_map(ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ ref **rm = cb_ದತ್ತಾಂಶ;
	ರಚನೆ ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	ಆಗ (ref && !ref->peer_ref)
		ref = ref->ಮುಂದಿನ;
	ಆದರೆ (!ref)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	*rm = ref->ಮುಂದಿನ;
	ಹಿಂತಿರುಗಿ &ref->old_oid;
}

ಸ್ಥಿರ ಶೂನ್ಯ update_remote_refs(ಸ್ಥಿರಾಂಕ ರಚನೆ ref *refs,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ ref *mapped_refs,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote_ತಲೆ_points_at,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_ತುದಿ,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ,
			       ರಚನೆ transport *transport,
			       ಪೂರ್ಣಾಂಕ check_connectivity)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref *rm = mapped_refs;

	ಆದರೆ (check_connectivity) {
		ರಚನೆ check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.ಪ್ರಗತಿ = transport->ಪ್ರಗತಿ;

		ಆದರೆ (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	ಆದರೆ (refs) {
		write_remote_refs(mapped_refs);
		ಆದರೆ (option_single_branch && option_ಟ್ಯಾಗುಗಳು)
			write_followtags(refs, ಸಂದೇಶ);
	}

	ಆದರೆ (remote_ತಲೆ_points_at && remote_ತಲೆ_points_at->peer_ref && !option_bare) {
		ರಚನೆ strbuf ತಲೆ_ref = STRBUF_INIT;
		strbuf_addstr(&ತಲೆ_ref, branch_ತುದಿ);
		strbuf_addstr(&ತಲೆ_ref, "HEAD");
		ಆದರೆ (refs_update_symref(get_main_ref_store(the_repository), ತಲೆ_ref.buf,
				       remote_ತಲೆ_points_at->peer_ref->name,
				       ಸಂದೇಶ) < 0)
			die(_("unable to update %s"), ತಲೆ_ref.buf);
		strbuf_release(&ತಲೆ_ref);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ update_ತಲೆ(ರಚನೆ clone_opts *opts, ಸ್ಥಿರಾಂಕ ರಚನೆ ref *our, ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *unborn, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ;
	ಆದರೆ (our && !opts->detach && skip_ಪೂರ್ವಪ್ರತ್ಯಯ(our->name, "refs/heads/", &ತಲೆ)) {
		/* Local default branch link */
		ಆದರೆ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, ಖಾಲಿ) < 0)
			die(_("unable to update HEAD"));
		ಆದರೆ (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					ಸಂದೇಶ, "HEAD", &our->old_oid, ಖಾಲಿ, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, ತಲೆ, remote_name, our->name);
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (our) {
		ರಚನೆ commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ,
				"HEAD", &c->object.oid, ಖಾಲಿ, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ,
				"HEAD", &remote->old_oid, ಖಾಲಿ, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unborn && skip_ಪೂರ್ವಪ್ರತ್ಯಯ(unborn, "refs/heads/", &ತಲೆ)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		ಆದರೆ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, ಖಾಲಿ) < 0)
			die(_("unable to update HEAD"));
		ಆದರೆ (!option_bare)
			install_branch_config(0, ತಲೆ, remote_name, unborn);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_sparse_checkout_init(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *repo)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	strvec_pushl(&cmd.ವಾದಗಳು, "-C", repo, "sparse-checkout", "set", ಖಾಲಿ);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	ಆದರೆ (ಸತತ_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		ಫಲಿತಾಂಶ = 1;
	}

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout(ಪೂರ್ಣಾಂಕ submodule_ಪ್ರಗತಿ,
		    ರಚನೆ list_objects_filter_options *filter_options,
		    ಪೂರ್ಣಾಂಕ filter_submodules,
		    ಎಣಿಕೆ ref_storage_format ref_storage_format)
{
	ರಚನೆ object_id oid;
	ಅಕ್ಷರ *ತಲೆ;
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ರಚನೆ unpack_trees_options opts;
	ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ರಚನೆ ವೃಕ್ಷ_desc t;
	ಪೂರ್ಣಾಂಕ err = 0;
	ರಚನೆ ಸತತ_hooks_opt hook_opt = ಸತತ_HOOKS_OPT_INIT_FORCE_SERIAL;

	ಆದರೆ (option_no_checkout)
		ಹಿಂತಿರುಗಿ 0;

	ತಲೆ = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, ಖಾಲಿ);
	ಆದರೆ (!ತಲೆ) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(ತಲೆ, "HEAD")) {
		ಆದರೆ (advice_enabled(ADVICE_DETACHED_ತಲೆ))
			detach_advice(oid_ಗಮ್ಯ_hex(&oid));
		FREE_AND_NULL(ತಲೆ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (!starts_with(ತಲೆ, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);

	repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, ಬೀಗ_DIE_ON_ERROR);

	memset(&opts, 0, ಗಾತ್ರ opts);
	opts.update = 1;
	opts.ವಿಲೀನ = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_ವಿಲೀನ;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.dst_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	init_checkout_metadata(&opts.meta, ತಲೆ, &oid, ಖಾಲಿ);

	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, &oid);
	ಆದರೆ (!ವೃಕ್ಷ)
		die(_("unable to parse commit %s"), oid_ಗಮ್ಯ_hex(&oid));
	ಆದರೆ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ) < 0)
		ನಿರ್ಗಮಿಸು(128);
	init_ವೃಕ್ಷ_desc(&t, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->ಬಫರ್, ವೃಕ್ಷ->ಗಾತ್ರ_2);
	ಆದರೆ (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	ಬಿಡುಗಡೆ(ತಲೆ);

	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file, COMMIT_ಬೀಗ))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.ವಾದಗಳು, oid_ಗಮ್ಯ_hex(null_oid(the_hash_algo)),
		     oid_ಗಮ್ಯ_hex(&oid), "1", ಖಾಲಿ);
	err |= ಸತತ_hooks_opt(the_repository, "post-checkout", &hook_opt);

	ಆದರೆ (!err && (option_recurse_submodules.nr > 0)) {
		ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;
		strvec_pushl(&cmd.ವಾದಗಳು, "submodule", "update", "--require-init",
			     "--recursive", ಖಾಲಿ);

		ಆದರೆ (option_shallow_submodules == 1)
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--depth=1");

		ಆದರೆ (max_jobs != -1)
			strvec_pushf(&cmd.ವಾದಗಳು, "--jobs=%d", max_jobs);

		ಆದರೆ (submodule_ಪ್ರಗತಿ)
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--progress");

		ಆದರೆ (option_verbosity < 0)
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--quiet");

		ಆದರೆ (option_remote_submodules) {
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--remote");
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--no-fetch");
		}

		ಆದರೆ (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.ವಾದಗಳು, "--ref-storage-format=%s",
				     ref_storage_format_ಗಮ್ಯ_name(ref_storage_format));

		ಆದರೆ (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.ವಾದಗಳು, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		ಆದರೆ (option_single_branch >= 0)
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = ಸತತ_command(&cmd);
	}

	ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_clone_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *k, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(k, "clone.defaultremotename")) {
		ಆದರೆ (!v)
			ಹಿಂತಿರುಗಿ config_error_nonbool(k);
		ಬಿಡುಗಡೆ(remote_name);
		remote_name = xstrdup(v);
	}
	ಆದರೆ (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	ಆದರೆ (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	ಹಿಂತಿರುಗಿ git_default_config(k, v, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_one_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx,
			    ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	ಪೂರ್ಣಾಂಕ apply_failed = git_clone_config(ಕೀಲಿ, ಮೌಲ್ಯ, ctx, ದತ್ತಾಂಶ);
	ಆದರೆ (apply_failed)
		ಹಿಂತಿರುಗಿ apply_failed;

	ಹಿಂತಿರುಗಿ repo_config_ಗಣ_multivar_gently(the_repository, ಕೀಲಿ,
					       ಮೌಲ್ಯ ? ಮೌಲ್ಯ : "true",
					       CONFIG_REGEX_NONE, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_config(ರಚನೆ string_list *config)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < config->nr; i++) {
		ಆದರೆ (git_config_parse_parameter(config->ವಸ್ತುಗಳು[i].string,
					       write_one_config, ಖಾಲಿ) < 0)
			die(_("unable to write parameters to config file"));
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ write_refspec_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src_ref_ಪೂರ್ವಪ್ರತ್ಯಯ,
		ಸ್ಥಿರಾಂಕ ರಚನೆ ref *our_ತಲೆ_points_at,
		ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote_ತಲೆ_points_at,
		ರಚನೆ strbuf *branch_ತುದಿ)
{
	ರಚನೆ strbuf ಕೀಲಿ = STRBUF_INIT;
	ರಚನೆ strbuf ಮೌಲ್ಯ = STRBUF_INIT;

	ಆದರೆ (option_mirror || !option_bare) {
		ಆದರೆ (option_single_branch && !option_mirror) {
			ಆದರೆ (option_branch) {
				ಆದರೆ (starts_with(our_ತಲೆ_points_at->name, "refs/tags/"))
					strbuf_addf(&ಮೌಲ್ಯ, "+%s:%s", our_ತಲೆ_points_at->name,
						our_ತಲೆ_points_at->name);
				ಇಲ್ಲದಿದ್ದರೆ
					strbuf_addf(&ಮೌಲ್ಯ, "+%s:%s%s", our_ತಲೆ_points_at->name,
						branch_ತುದಿ->buf, option_branch);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (remote_ತಲೆ_points_at) {
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ = remote_ತಲೆ_points_at->name;
				ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ತಲೆ, "refs/heads/", &ತಲೆ))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&ಮೌಲ್ಯ, "+%s:%s%s", remote_ತಲೆ_points_at->name,
						branch_ತುದಿ->buf, ತಲೆ);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} ಇಲ್ಲದಿದ್ದರೆ {
			strbuf_addf(&ಮೌಲ್ಯ, "+%s*:%s*", src_ref_ಪೂರ್ವಪ್ರತ್ಯಯ, branch_ತುದಿ->buf);
		}
		/* Configure the remote */
		ಆದರೆ (ಮೌಲ್ಯ.len) {
			strbuf_addf(&ಕೀಲಿ, "remote.%s.fetch", remote_name);
			repo_config_ಗಣ_multivar(the_repository, ಕೀಲಿ.buf, ಮೌಲ್ಯ.buf, "^$", 0);
			strbuf_reset(&ಕೀಲಿ);

			ಆದರೆ (option_mirror) {
				strbuf_addf(&ಕೀಲಿ, "remote.%s.mirror", remote_name);
				repo_config_ಗಣ(the_repository, ಕೀಲಿ.buf, "true");
				strbuf_reset(&ಕೀಲಿ);
			}
		}
	}

	strbuf_release(&ಕೀಲಿ);
	strbuf_release(&ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ dissociate_from_references(ಶೂನ್ಯ)
{
	ಅಕ್ಷರ *alternates = repo_git_ಮಾರ್ಗ(the_repository, "objects/info/alternates");

	ಆದರೆ (!access(alternates, F_OK)) {
		ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.ವಾದಗಳು, "repack", "-a", "-d", ಖಾಲಿ);
		ಆದರೆ (ಸತತ_command(&cmd))
			die(_("cannot repack to clean up"));
		ಆದರೆ (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	ಬಿಡುಗಡೆ(alternates);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮಾರ್ಗ_exists(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ರಚನೆ stat sb;
	ಹಿಂತಿರುಗಿ !stat(ಮಾರ್ಗ, &sb);
}

ಪೂರ್ಣಾಂಕ cmd_clone(ಪೂರ್ಣಾಂಕ argc,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repository ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ is_bundle = 0, is_local;
	ಪೂರ್ಣಾಂಕ reject_shallow = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *repo_name, *repo, *work_ವೃಕ್ಷ, *git_dir;
	ಅಕ್ಷರ *repo_ಗಮ್ಯ_free = ಖಾಲಿ;
	ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ, *dir, *display_repo = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ dest_exists, real_dest_exists = 0;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref *refs, *remote_ತಲೆ;
	ರಚನೆ ref *remote_ತಲೆ_points_at = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref *our_ತಲೆ_points_at;
	ಅಕ್ಷರ *unborn_ತಲೆ = ಖಾಲಿ;
	ರಚನೆ ref *mapped_refs = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref *ref;
	ರಚನೆ strbuf ಕೀಲಿ = STRBUF_INIT;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ strbuf branch_ತುದಿ = STRBUF_INIT, reflog_ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ transport *transport = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src_ref_ಪೂರ್ವಪ್ರತ್ಯಯ = "refs/heads/";
	ರಚನೆ remote *remote;
	ಪೂರ್ಣಾಂಕ err = 0, complete_refs_before_fetch = 1;
	ಪೂರ್ಣಾಂಕ submodule_ಪ್ರಗತಿ;
	ಪೂರ್ಣಾಂಕ filter_submodules = 0;
	ಪೂರ್ಣಾಂಕ hash_algo;
	ಎಣಿಕೆ ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ do_not_override_repo_unix_permissions = -1;
	ಪೂರ್ಣಾಂಕ option_reject_shallow = -1; /* unspecified */
	ಪೂರ್ಣಾಂಕ deepen = 0;
	ಅಕ್ಷರ *option_template = ಖಾಲಿ, *option_ಆಳ = ಖಾಲಿ, *option_since = ಖಾಲಿ;
	ಅಕ್ಷರ *option_origin = ಖಾಲಿ;
	ರಚನೆ string_list option_not = STRING_LIST_INIT_NODUP;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *real_git_dir = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref_storage_format_uri = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option_upload_pack = "git-upload-pack";
	ಪೂರ್ಣಾಂಕ option_ಪ್ರಗತಿ = -1;
	ಪೂರ್ಣಾಂಕ option_sparse_checkout = 0;
	ಎಣಿಕೆ transport_family family = TRANSPORT_FAMILY_ALL;
	ರಚನೆ string_list option_config = STRING_LIST_INIT_DUP;
	ಪೂರ್ಣಾಂಕ option_dissociate = 0;
	ರಚನೆ list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	ಪೂರ್ಣಾಂಕ option_filter_submodules = -1; /* unspecified */
	ರಚನೆ string_list server_options = STRING_LIST_INIT_NODUP;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle_uri = ಖಾಲಿ;
	ಅಕ್ಷರ *option_rev = ಖಾಲಿ;

	ರಚನೆ clone_opts opts = CLONE_OPTS_INIT;

	ರಚನೆ transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	ರಚನೆ option builtin_clone_options[] = {
		OPT__VERBOSITY(&option_verbosity),
		OPT_BOOL(0, "progress", &option_ಪ್ರಗತಿ,
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
		OPT_STRING(0, "depth", &option_ಆಳ, N_("depth"),
			   N_("create a shallow clone of that depth")),
		OPT_STRING(0, "shallow-since", &option_since, N_("time"),
			   N_("create a shallow clone since a specific time")),
		OPT_STRING_LIST(0, "shallow-exclude", &option_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_BOOL(0, "single-branch", &option_single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_BOOL(0, "tags", &option_ಟ್ಯಾಗುಗಳು,
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
		OPT_ಕೊನೆ()
	};
	ರಚನೆ strvec alternates = STRVEC_INIT;

	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		ಖಾಲಿ
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_ತಟಸ್ಥ("clone");

	repo_config(the_repository, git_clone_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_clone_options,
			     builtin_clone_usage, 0);

	ಆದರೆ (argc > 2)
		usage_ಸಂದೇಶ_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	ಆದರೆ (argc == 0)
		usage_ಸಂದೇಶ_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	ಆದರೆ (option_ಆಳ || option_since || option_not.nr)
		deepen = 1;
	ಆದರೆ (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	ಆದರೆ (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, ಖಾಲಿ);
		ಆದರೆ (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	ಆದರೆ (option_mirror) {
		option_bare = 1;
		option_ಟ್ಯಾಗುಗಳು = 0;
	}

	ಆದರೆ (option_bare) {
		ಆದರೆ (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	ಆದರೆ (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	ಮಾರ್ಗ = get_repo_ಮಾರ್ಗ(repo_name, &is_bundle);
	ಆದರೆ (ಮಾರ್ಗ) {
		FREE_AND_NULL(ಮಾರ್ಗ);
		repo = repo_ಗಮ್ಯ_free = absolute_pathdup(repo_name);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} ಇಲ್ಲದಿದ್ದರೆ
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	ಆದರೆ (option_ಆಳ && atoi(option_ಆಳ) < 1)
		die(_("depth %s is not a positive number"), option_ಆಳ);

	ಆದರೆ (argc == 2)
		dir = xstrdup(argv[1]);
	ಇಲ್ಲದಿದ್ದರೆ
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = ಮಾರ್ಗ_exists(dir);
	ಆದರೆ (dest_exists && !is_ಖಾಲಿಗಳು_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	ಆದರೆ (real_git_dir) {
		real_dest_exists = ಮಾರ್ಗ_exists(real_git_dir);
		ಆದರೆ (real_dest_exists && !is_ಖಾಲಿಗಳು_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_ಸಂದೇಶ, "clone: from %s",
		    display_repo ? display_repo : repo);
	ಬಿಡುಗಡೆ(display_repo);

	ಆದರೆ (option_bare)
		work_ವೃಕ್ಷ = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ {
		work_ವೃಕ್ಷ = getenv("GIT_WORK_TREE");
		ಆದರೆ (work_ವೃಕ್ಷ && ಮಾರ್ಗ_exists(work_ವೃಕ್ಷ))
			die(_("working tree '%s' already exists."), work_ವೃಕ್ಷ);
	}

	ಆದರೆ (option_bare || work_ವೃಕ್ಷ)
		git_dir = xstrdup(dir);
	ಇಲ್ಲದಿದ್ದರೆ {
		work_ವೃಕ್ಷ = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_ತಳ್ಳು_common(remove_junk_on_signal);

	ಆದರೆ (!option_bare) {
		ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories_const(the_repository, work_ವೃಕ್ಷ) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_ವೃಕ್ಷ);
		ಆದರೆ (dest_exists)
			junk_work_ವೃಕ್ಷ_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mkdir(work_ವೃಕ್ಷ, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_ವೃಕ್ಷ);
		junk_work_ವೃಕ್ಷ = work_ವೃಕ್ಷ;
	}

	ಆದರೆ (real_git_dir) {
		ಆದರೆ (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	ಆದರೆ (0 <= option_verbosity) {
		ಆದರೆ (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		ಇಲ್ಲದಿದ್ದರೆ
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	ಆದರೆ (option_recurse_submodules.nr > 0) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
		ರಚನೆ strbuf sb = STRBUF_INIT;
		ಪೂರ್ಣಾಂಕ val;

		/* remove duplicates */
		string_list_ವಿಂಗಡಿಸು_u(&option_recurse_submodules, 0);

		/*
		 * NEEDSWORK: In a multi-working-tree world, this needs to be
		 * set in the per-worktree config.
		 */
		for_each_string_list_ಅಂಶ(ಅಂಶ, &option_recurse_submodules) {
			strbuf_addf(&sb, "submodule.active=%s",
				    ಅಂಶ->string);
			string_list_append(&option_config, sb.buf);
			strbuf_reset(&sb);
		}

		ಆದರೆ (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		ಆದರೆ (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (option_optional_reference.nr) {
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
			  do_not_override_repo_unix_permissions, ಖಾಲಿ);

	ಆದರೆ (real_git_dir) {
		ಬಿಡುಗಡೆ((ಅಕ್ಷರ *)git_dir);
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
	initialize_repository_ಆವೃತ್ತಿ(the_repository, GIT_HASH_UNKNOWN,
				      the_repository->ref_storage_format, 1);

	refs_create_refdir_stubs(the_repository, git_dir, ಖಾಲಿ);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, ಖಾಲಿ);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	ಆದರೆ (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	ಆದರೆ (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	ಆದರೆ (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	ಆದರೆ (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	ಆದರೆ (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	ಆದರೆ (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	ಆದರೆ (option_origin) {
		ಬಿಡುಗಡೆ(remote_name);
		remote_name = xstrdup(option_origin);
	}

	ಆದರೆ (!remote_name)
		remote_name = xstrdup("origin");

	ಆದರೆ (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	ಆದರೆ (option_bare) {
		ಆದರೆ (option_mirror)
			src_ref_ಪೂರ್ವಪ್ರತ್ಯಯ = "refs/";
		strbuf_addstr(&branch_ತುದಿ, src_ref_ಪೂರ್ವಪ್ರತ್ಯಯ);

		repo_config_ಗಣ(the_repository, "core.bare", "true");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!option_rev) {
		strbuf_addf(&branch_ತುದಿ, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&ಕೀಲಿ, "remote.%s.url", remote_name);
	repo_config_ಗಣ(the_repository, ಕೀಲಿ.buf, repo);
	strbuf_reset(&ಕೀಲಿ);

	ಆದರೆ (!option_ಟ್ಯಾಗುಗಳು) {
		strbuf_addf(&ಕೀಲಿ, "remote.%s.tagOpt", remote_name);
		repo_config_ಗಣ(the_repository, ಕೀಲಿ.buf, "--no-tags");
		strbuf_reset(&ಕೀಲಿ);
	}

	remote = remote_get_early(remote_name);

	ಆದರೆ (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_ಪೂರ್ವಪ್ರತ್ಯಯ,
				branch_ತುದಿ.buf);

	ಮಾರ್ಗ = get_repo_ಮಾರ್ಗ(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && ಮಾರ್ಗ && !is_bundle;
	ಆದರೆ (is_local) {
		ಆದರೆ (option_ಆಳ)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		ಆದರೆ (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		ಆದರೆ (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		ಆದರೆ (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		ಆದರೆ (!access(mkpath("%s/shallow", ಮಾರ್ಗ), F_OK)) {
			ಆದರೆ (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			ಆದರೆ (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	ಆದರೆ (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? ಮಾರ್ಗ : ಖಾಲಿ);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, ಮಾರ್ಗ ? ಮಾರ್ಗ : remote->url.v[0]);
	transport_ಗಣ_verbosity(transport, option_verbosity, option_ಪ್ರಗತಿ);
	transport->family = family;
	transport->cloning = 1;

	ಆದರೆ (is_bundle) {
		ರಚನೆ bundle_ಶೀರ್ಷಿಕೆ ಶೀರ್ಷಿಕೆ = BUNDLE_ಶೀರ್ಷಿಕೆ_INIT;
		ಪೂರ್ಣಾಂಕ fd = read_bundle_ಶೀರ್ಷಿಕೆ(ಮಾರ್ಗ, &ಶೀರ್ಷಿಕೆ);
		ಪೂರ್ಣಾಂಕ has_filter = ಶೀರ್ಷಿಕೆ.filter.choice != LOFC_DISABLED;

		ಆದರೆ (fd > 0)
			close(fd);
		bundle_ಶೀರ್ಷಿಕೆ_release(&ಶೀರ್ಷಿಕೆ);
		ಆದರೆ (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_ಗಣ_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	ಆದರೆ (reject_shallow)
		transport_ಗಣ_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	ಆದರೆ (option_ಆಳ)
		transport_ಗಣ_option(transport, TRANS_OPT_ಆಳ,
				     option_ಆಳ);
	ಆದರೆ (option_since)
		transport_ಗಣ_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	ಆದರೆ (option_not.nr)
		transport_ಗಣ_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)&option_not);
	ಆದರೆ (option_single_branch) {
		transport_ಗಣ_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		ಆದರೆ (option_branch)
			opts.ಬಯಸುತ್ತದೆ_ತಲೆ = 0;
	}

	ಆದರೆ (option_upload_pack)
		transport_ಗಣ_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	ಆದರೆ (server_options.nr)
		transport->server_options = &server_options;

	ಆದರೆ (filter_options.choice) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_ಗಣ_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_ಗಣ_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	ಆದರೆ (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	ಆದರೆ (option_rev) {
		option_ಟ್ಯಾಗುಗಳು = 0;
		option_single_branch = 0;
		opts.ಬಯಸುತ್ತದೆ_ತಲೆ = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	ಆದರೆ (option_ಟ್ಯಾಗುಗಳು || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, ಟ್ಯಾಗ್_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	ಆದರೆ (option_branch)
		expand_ref_ಪೂರ್ವಪ್ರತ್ಯಯ(&transport_ls_refs_options.ref_prefixes,
				  option_branch);

	/*
	 * As part of transport_get_remote_refs() the server tells us the hash
	 * algorithm, which we require to initialize the repo. But calling that
	 * function without any ref prefix, will cause the server to announce
	 * all known refs. If the argument passed to --revision was a hex oid,
	 * ref_prefixes will be empty so we fall back to asking about HEAD to
	 * reduce traffic from the server.
	 */
	ಆದರೆ (opts.ಬಯಸುತ್ತದೆ_ತಲೆ || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_ತಳ್ಳು(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_ಆವೃತ್ತಿ(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_ಗಣ_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, ಖಾಲಿ, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	ಆದರೆ (bundle_uri) {
		ರಚನೆ remote_state *state;
		ಪೂರ್ಣಾಂಕ has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = ಖಾಲಿ;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		ಆದರೆ (repo_init(the_repository, git_dir, work_ವೃಕ್ಷ))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_heuristic)
			repo_config_ಗಣ_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		ಬಿಡುಗಡೆ(the_repository->remote_state);
		the_repository->remote_state = state;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		ಆದರೆ (transport->bundles &&
		    hashmap_get_ಗಾತ್ರ(&transport->bundles->bundles)) {
			ರಚನೆ remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = ಖಾಲಿ;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			ಆದರೆ (repo_init(the_repository, git_dir, work_ವೃಕ್ಷ))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			ಬಿಡುಗಡೆ(the_repository->remote_state);
			the_repository->remote_state = state;
		} ಇಲ್ಲದಿದ್ದರೆ {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	ಆದರೆ (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	ಆದರೆ (mapped_refs) {
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
		ಗಾಗಿ (ref = refs; ref; ref = ref->ಮುಂದಿನ)
			ಆದರೆ (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				ನಿಲ್ಲಿಸು;
			}

		ಆದರೆ (!is_local && !complete_refs_before_fetch) {
			ಆದರೆ (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_ತಲೆ = ಕಂಡುಹಿಡಿ_ref_by_name(refs, "HEAD");
	remote_ತಲೆ_points_at = guess_remote_ತಲೆ(remote_ತಲೆ, mapped_refs,
						  REMOTE_GUESS_ತಲೆ_QUIET);

	ಆದರೆ (option_branch) {
		our_ತಲೆ_points_at = ಕಂಡುಹಿಡಿ_remote_branch(mapped_refs, option_branch);
		ಆದರೆ (!our_ತಲೆ_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (option_rev) {
		our_ತಲೆ_points_at = mapped_refs;
		ಆದರೆ (!our_ತಲೆ_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (remote_ತಲೆ_points_at) {
		our_ತಲೆ_points_at = remote_ತಲೆ_points_at;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (remote_ತಲೆ) {
		our_ತಲೆ_points_at = ಖಾಲಿ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಅಕ್ಷರ *ಗಮ್ಯ_free = ಖಾಲಿ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch;

		ಆದರೆ (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		ಆದರೆ (transport_ls_refs_options.unborn_ತಲೆ_ಗುರಿ &&
		    skip_ಪೂರ್ವಪ್ರತ್ಯಯ(transport_ls_refs_options.unborn_ತಲೆ_ಗುರಿ,
				"refs/heads/", &branch)) {
			unborn_ತಲೆ  = xstrdup(transport_ls_refs_options.unborn_ತಲೆ_ಗುರಿ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			branch = ಗಮ್ಯ_free = repo_default_branch_name(the_repository, 0);
			unborn_ತಲೆ = xstrfmt("refs/heads/%s", branch);
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
		our_ತಲೆ_points_at = ಕಂಡುಹಿಡಿ_remote_branch(mapped_refs, branch);

		ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
	}

	ಆದರೆ (!option_rev)
		write_refspec_config(src_ref_ಪೂರ್ವಪ್ರತ್ಯಯ, our_ತಲೆ_points_at,
				     remote_ತಲೆ_points_at, &branch_ತುದಿ);

	ಆದರೆ (filter_options.choice)
		partial_clone_ರಿಜಿಸ್ಟರ್(remote_name, &filter_options);

	ಆದರೆ (is_local)
		clone_local(ಮಾರ್ಗ, git_dir);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mapped_refs && complete_refs_before_fetch) {
		ಆದರೆ (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_ತಲೆ_points_at,
			   branch_ತುದಿ.buf, reflog_ಸಂದೇಶ.buf, transport,
			   !is_local);

	update_ತಲೆ(&opts, our_ತಲೆ_points_at, remote_ತಲೆ, unborn_ತಲೆ, reflog_ಸಂದೇಶ.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_ಪ್ರಗತಿ = transport->ಪ್ರಗತಿ;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	ಆದರೆ (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	ಆದರೆ (option_sparse_checkout && git_sparse_checkout_init(dir))
		ಹಿಂತಿರುಗಿ 1;

	junk_ಬಹುಲಕ = JUNK_LEAVE_REPO;
	err = checkout(submodule_ಪ್ರಗತಿ,
		       &filter_options,
		       filter_submodules,
		       ref_storage_format);

	list_objects_filter_release(&filter_options);

	string_list_clear(&option_not, 0);
	string_list_clear(&option_config, 0);
	string_list_clear(&server_options, 0);
	strvec_clear(&alternates);

	ಬಿಡುಗಡೆ(remote_name);
	strbuf_release(&reflog_ಸಂದೇಶ);
	strbuf_release(&branch_ತುದಿ);
	strbuf_release(&buf);
	strbuf_release(&ಕೀಲಿ);
	free_refs(mapped_refs);
	free_refs(remote_ತಲೆ_points_at);
	ಬಿಡುಗಡೆ(unborn_ತಲೆ);
	ಬಿಡುಗಡೆ(dir);
	ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	ಬಿಡುಗಡೆ(repo_ಗಮ್ಯ_free);
	junk_ಬಹುಲಕ = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	ಹಿಂತಿರುಗಿ err;
}
