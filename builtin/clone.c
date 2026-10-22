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

#ଅନ୍ତର୍ଭାଵ "builtin.h"

#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "copy.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "refspec.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "tree-walk.h"
#ଅନ୍ତର୍ଭାଵ "unpack-trees.h"
#ଅନ୍ତର୍ଭାଵ "transport.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "dir-iterator.h"
#ଅନ୍ତର୍ଭାଵ "iterator.h"
#ଅନ୍ତର୍ଭାଵ "sigchain.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "connected.h"
#ଅନ୍ତର୍ଭାଵ "packfile.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "pkt-line.h"
#ଅନ୍ତର୍ଭାଵ "list-objects-filter-options.h"
#ଅନ୍ତର୍ଭାଵ "hook.h"
#ଅନ୍ତର୍ଭାଵ "bundle.h"
#ଅନ୍ତର୍ଭାଵ "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

ସଂରଚନା clone_opts {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା wants_head;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach;
};
#define CLONE_OPTS_INIT { \
	.wants_head = 1 /* default enabled */ \
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_local = -1, option_no_hardlinks, option_shared;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_tags = 1; /* default enabled */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_shallow_submodules;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା config_reject_shallow = -1;    /* unspecified */
ସ୍ଥିର ଅକ୍ଷର *remote_name = ଖାଲି;
ସ୍ଥିର ଅକ୍ଷର *option_branch = ଖାଲି;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_verbosity;
ସ୍ଥିର ସଂରଚନା string_list option_required_reference = STRING_LIST_INIT_NODUP;
ସ୍ଥିର ସଂରଚନା string_list option_optional_reference = STRING_LIST_INIT_NODUP;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_jobs = -1;
ସ୍ଥିର ସଂରଚନା string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା config_filter_submodules = -1;    /* unspecified */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_remote_submodules;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse_submodules_cb(ଅଚଳ ସଂରଚନା option *opt,
				 ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଯଦି (unset)
		string_list_clear((ସଂରଚନା string_list *)opt->ମୂଲ୍ୟ, 0);
	ଅନ୍ୟଥା ଯଦି (arg)
		string_list_append((ସଂରଚନା string_list *)opt->ମୂଲ୍ୟ, arg);
	ଅନ୍ୟଥା
		string_list_append((ସଂରଚନା string_list *)opt->ମୂଲ୍ୟ,
				   (ଅଚଳ ଅକ୍ଷର *)opt->defval);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_repo_path_1(ସଂରଚନା strbuf *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *is_bundle)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *suffix[] = { "/.git", "", ".git/.git", ".git" };
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *bundle_suffix[] = { ".bundle", "" };
	size_t baselen = path->len;
	ସଂରଚନା stat st;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < ARRAY_SIZE(suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, suffix[i]);
		ଯଦି (stat(path->buf, &st))
			ଜାରି;
		ଯଦି (S_ISDIR(st.st_mode) && is_git_directory(path->buf)) {
			*is_bundle = 0;
			ଫେରନ୍ତୁ path->buf;
		} ଅନ୍ୟଥା ଯଦି (S_ISREG(st.st_mode) && st.st_size > 8) {
			/* Is it a "gitfile"? */
			ଅକ୍ଷର signature[8];
			ଅଚଳ ଅକ୍ଷର *dst;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, fd = open(path->buf, O_RDONLY);
			ଯଦି (fd < 0)
				ଜାରି;
			len = read_in_full(fd, signature, 8);
			close(fd);
			ଯଦି (len != 8 || strncmp(signature, "gitdir: ", 8))
				ଜାରି;
			dst = read_gitfile(path->buf);
			ଯଦି (dst) {
				*is_bundle = 0;
				ଫେରନ୍ତୁ dst;
			}
		}
	}

	ପାଇଁ (i = 0; i < ARRAY_SIZE(bundle_suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, bundle_suffix[i]);
		ଯଦି (!stat(path->buf, &st) && S_ISREG(st.st_mode)) {
			*is_bundle = 1;
			ଫେରନ୍ତୁ path->buf;
		}
	}

	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଅକ୍ଷର *get_repo_path(ଅଚଳ ଅକ୍ଷର *repo, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *is_bundle)
{
	ସଂରଚନା strbuf path = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *raw;
	ଅକ୍ଷର *canon;

	strbuf_addstr(&path, repo);
	raw = get_repo_path_1(&path, is_bundle);
	canon = raw ? absolute_pathdup(raw) : ଖାଲି;
	strbuf_release(&path);
	ଫେରନ୍ତୁ canon;
}

ସଂରଚନା collect_alternates_data {
	ସଂରଚନା strvec *alternates;
	bool required;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା collect_one_alternate(ସଂରଚନା string_list_item *item, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା collect_alternates_data *data = cb_data;
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ଅକ୍ଷର *ref_git = compute_alternate_path(item->string, &err);

	ଯଦି (!ref_git) {
		ଯଦି (data->required)
			die("%s", err.buf);
		ଅନ୍ୟଥା
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				item->string, err.buf);
	} ଅନ୍ୟଥା {
		strvec_pushf(data->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	ମୁକ୍ତ(ref_git);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ read_alternates(ସଂରଚନା strvec *alternates, ଅଚଳ ଅକ୍ଷର *src_repo)
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
	ସଂରଚନା strbuf path = STRBUF_INIT;
	ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;

	strbuf_addf(&path, "%s/objects/info/alternates", src_repo);

	in = fopen(path.buf, "r");
	ଯଦି (!in) {
		ଯଦି (errno == ENOENT)
			ଯାଅ out;
		die_errno("could not read alternates file '%s'", path.buf);
	}

	ଯେତେବେଳେ (strbuf_getline(&ଧାଡ଼ି, in) != EOF) {
		ଅକ୍ଷର *abs_path;
		ଯଦି (!ଧାଡ଼ି.len || ଧାଡ଼ି.buf[0] == '#')
			ଜାରି;
		ଯଦି (is_absolute_path(ଧାଡ଼ି.buf)) {
			strvec_ଠେଲ(alternates, ଧାଡ଼ି.buf);
			ଜାରି;
		}
		abs_path = mkpathdup("%s/objects/%s", src_repo, ଧାଡ଼ି.buf);
		ଯଦି (!normalize_path_copy(abs_path, abs_path))
			strvec_ଠେଲ(alternates, abs_path);
		ଅନ୍ୟଥା
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, ଧାଡ଼ି.buf);
		ମୁକ୍ତ(abs_path);
	}

out:
	strbuf_release(&path);
	strbuf_release(&ଧାଡ଼ି);
	ଯଦି (in)
		fclose(in);
}

ସ୍ଥିର ଶୂନ୍ୟ collect_alternates(ସଂରଚନା strvec *alternates,
			       ଅଚଳ ଅକ୍ଷର *local_source_repo)
{
	ଯଦି (option_required_reference.nr || option_optional_reference.nr) {
		ସଂରଚନା collect_alternates_data data = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &data);
		data.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &data);
	}

	ଯଦି (local_source_repo) {
		ସଂରଚନା strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_source_repo);
		ଯଦି (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		ଅନ୍ୟଥା
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ mkdir_if_missing(ଅଚଳ ଅକ୍ଷର *pathname, mode_t mode)
{
	ସଂରଚନା stat st;

	ଯଦି (!mkdir(pathname, mode))
		ଫେରନ୍ତୁ;

	ଯଦି (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	ଅନ୍ୟଥା ଯଦି (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	ଅନ୍ୟଥା ଯଦି (!S_ISDIR(st.st_mode))
		die(_("%s exists and is not a directory"), pathname);
}

ସ୍ଥିର ଶୂନ୍ୟ copy_or_link_directory(ସଂରଚନା strbuf *src, ସଂରଚନା strbuf *dest,
				   ଅଚଳ ଅକ୍ଷର *src_repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_len, dest_len;
	ସଂରଚନା dir_iterator *iter;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା iter_status;

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
	die_upon_dubious_ownership(ଖାଲି, ଖାଲି, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	ଯଦି (!iter) {
		ଯଦି (errno == ENOTDIR) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
			ସଂରଚନା stat st;

			ଯଦି (!lstat(src->buf, &st) && S_ISLNK(st.st_mode))
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

	ଯେତେବେଳେ ((iter_status = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_path);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_path);

		ଯଦି (S_ISLNK(iter->st.st_mode))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_path);

		ଯଦି (S_ISDIR(iter->st.st_mode)) {
			mkdir_if_missing(dest->buf, 0777);
			ଜାରି;
		}

		/* Alternates were already handled earlier. */
		ଯଦି (!fspathcmp(iter->relative_path, "info/alternates"))
			ଜାରି;

		ଯଦି (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		ଯଦି (!option_no_hardlinks) {
			ଯଦି (!link(src->buf, dest->buf)) {
				ସଂରଚନା stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				ଯଦି (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				ଯଦି (st.st_mode != iter->st.st_mode ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_size != iter->st.st_size ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				ଜାରି;
			}
			ଯଦି (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		ଯଦି (copy_file_with_time(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	ଯଦି (iter_status != ITER_DONE) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

ସ୍ଥିର ଶୂନ୍ୟ clone_local(ଅଚଳ ଅକ୍ଷର *src_repo, ଅଚଳ ଅକ୍ଷର *dest_repo)
{
	ଯଦି (!option_shared) {
		ସଂରଚନା strbuf src = STRBUF_INIT;
		ସଂରଚନା strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	ଯଦି (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *junk_work_ବୃକ୍ଷ;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା junk_work_ବୃକ୍ଷ_flags;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *junk_git_dir;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା junk_git_dir_flags;
ସ୍ଥିର ଗଣନା {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_mode = JUNK_LEAVE_NONE;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର junk_leave_repo_ସନ୍ଦେଶ[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

ସ୍ଥିର ଶୂନ୍ୟ remove_junk(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ବାଛ (junk_mode) {
	ସ୍ଥିତି JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_ସନ୍ଦେଶ));
		/* fall-through */
	ସ୍ଥିତି JUNK_LEAVE_ALL:
		ଫେରନ୍ତୁ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		/* proceed to removal */
		ଭାଙ୍ଗ;
	}

	ଯଦି (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	ଯଦି (junk_work_ବୃକ୍ଷ) {
		strbuf_addstr(&sb, junk_work_ବୃକ୍ଷ);
		remove_dir_recursively(&sb, junk_work_ବୃକ୍ଷ_flags);
	}
	strbuf_release(&sb);
}

ସ୍ଥିର ଶୂନ୍ୟ remove_junk_on_signal(ପୂର୍ଣ୍ଣସଂଖ୍ୟା signo)
{
	remove_junk();
	sigchain_କାଢ଼(signo);
	ଉଠାଅ(signo);
}

ସ୍ଥିର ସଂରଚନା ref *find_remote_branch(ଅଚଳ ସଂରଚନା ref *refs, ଅଚଳ ଅକ୍ଷର *branch)
{
	ସଂରଚନା ref *ref;
	ସଂରଚନା strbuf head = STRBUF_INIT;
	strbuf_addstr(&head, "refs/heads/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	ଯଦି (ref)
		ଫେରନ୍ତୁ ref;

	strbuf_addstr(&head, "refs/tags/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	ଫେରନ୍ତୁ ref;
}

ସ୍ଥିର ସଂରଚନା ref *wanted_peer_refs(ସଂରଚନା clone_opts *opts,
				    ଅଚଳ ସଂରଚନା ref *refs,
				    ସଂରଚନା refspec *refspec)
{
	ସଂରଚନା ref *local_refs = ଖାଲି;
	ସଂରଚନା ref **tail = &local_refs;
	ସଂରଚନା ref *to_free = ଖାଲି;

	ଯଦି (opts->wants_head) {
		ସଂରଚନା ref *head = copy_ref(find_ref_by_name(refs, "HEAD"));
		ଯଦି (head)
			tail_link_ref(head, &tail);
		ଯଦି (option_single_branch)
			refs = to_free =
				guess_remote_head(head, refs,
						  REMOTE_GUESS_HEAD_QUIET);
	} ଅନ୍ୟଥା ଯଦି (option_single_branch) {
		local_refs = ଖାଲି;
		tail = &local_refs;
		refs = to_free = copy_ref(find_remote_branch(refs, option_branch));
	}

	ପାଇଁ (size_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->ବସ୍ତୁଗୁଡ଼ିକ[i], &tail, 0);

	free_one_ref(to_free);

	ଫେରନ୍ତୁ local_refs;
}

ସ୍ଥିର ଶୂନ୍ୟ write_remote_refs(ଅଚଳ ସଂରଚନା ref *local_refs)
{
	ଅଚଳ ସଂରଚନା ref *r;

	ସଂରଚନା ref_transaction *t;
	ସଂରଚନା strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	ଯଦି (!t)
		die("%s", err.buf);

	ପାଇଁ (r = local_refs; r; r = r->next) {
		ଯଦି (!r->peer_ref)
			ଜାରି;
		ଯଦି (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   ଖାଲି, 0, ଖାଲି, &err))
			die("%s", err.buf);
	}

	ଯଦି (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

ସ୍ଥିର ଶୂନ୍ୟ write_followtags(ଅଚଳ ସଂରଚନା ref *refs, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ)
{
	ଅଚଳ ସଂରଚନା ref *ref;
	ପାଇଁ (ref = refs; ref; ref = ref->next) {
		ଯଦି (!starts_with(ref->name, "refs/tags/"))
			ଜାରି;
		ଯଦି (ends_with(ref->name, "^{}"))
			ଜାରି;
		ଯଦି (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			ଜାରି;
		refs_update_ref(get_main_ref_store(the_repository), ସନ୍ଦେଶ,
				ref->name, &ref->old_oid, ଖାଲି, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା object_id *iterate_ref_map(ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା ref **rm = cb_data;
	ସଂରଚନା ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	ଯେତେବେଳେ (ref && !ref->peer_ref)
		ref = ref->next;
	ଯଦି (!ref)
		ଫେରନ୍ତୁ ଖାଲି;

	*rm = ref->next;
	ଫେରନ୍ତୁ &ref->old_oid;
}

ସ୍ଥିର ଶୂନ୍ୟ update_remote_refs(ଅଚଳ ସଂରଚନା ref *refs,
			       ଅଚଳ ସଂରଚନା ref *mapped_refs,
			       ଅଚଳ ସଂରଚନା ref *remote_head_points_at,
			       ଅଚଳ ଅକ୍ଷର *branch_ଶୀର୍ଷ,
			       ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ,
			       ସଂରଚନା transport *transport,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_connectivity)
{
	ଅଚଳ ସଂରଚନା ref *rm = mapped_refs;

	ଯଦି (check_connectivity) {
		ସଂରଚନା check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.progress = transport->progress;

		ଯଦି (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	ଯଦି (refs) {
		write_remote_refs(mapped_refs);
		ଯଦି (option_single_branch && option_tags)
			write_followtags(refs, ସନ୍ଦେଶ);
	}

	ଯଦି (remote_head_points_at && remote_head_points_at->peer_ref && !option_bare) {
		ସଂରଚନା strbuf head_ref = STRBUF_INIT;
		strbuf_addstr(&head_ref, branch_ଶୀର୍ଷ);
		strbuf_addstr(&head_ref, "HEAD");
		ଯଦି (refs_update_symref(get_main_ref_store(the_repository), head_ref.buf,
				       remote_head_points_at->peer_ref->name,
				       ସନ୍ଦେଶ) < 0)
			die(_("unable to update %s"), head_ref.buf);
		strbuf_release(&head_ref);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ update_head(ସଂରଚନା clone_opts *opts, ଅଚଳ ସଂରଚନା ref *our, ଅଚଳ ସଂରଚନା ref *remote,
			ଅଚଳ ଅକ୍ଷର *unborn, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ)
{
	ଅଚଳ ଅକ୍ଷର *head;
	ଯଦି (our && !opts->detach && skip_prefix(our->name, "refs/heads/", &head)) {
		/* Local default branch link */
		ଯଦି (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, ଖାଲି) < 0)
			die(_("unable to update HEAD"));
		ଯଦି (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					ସନ୍ଦେଶ, "HEAD", &our->old_oid, ଖାଲି, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, head, remote_name, our->name);
		}
	} ଅନ୍ୟଥା ଯଦି (our) {
		ସଂରଚନା commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), ସନ୍ଦେଶ,
				"HEAD", &c->object.oid, ଖାଲି, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ଅନ୍ୟଥା ଯଦି (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), ସନ୍ଦେଶ,
				"HEAD", &remote->old_oid, ଖାଲି, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ଅନ୍ୟଥା ଯଦି (unborn && skip_prefix(unborn, "refs/heads/", &head)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		ଯଦି (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, ଖାଲି) < 0)
			die(_("unable to update HEAD"));
		ଯଦି (!option_bare)
			install_branch_config(0, head, remote_name, unborn);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_sparse_checkout_init(ଅଚଳ ଅକ୍ଷର *repo)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	strvec_pushl(&cmd.args, "-C", repo, "sparse-checkout", "set", ଖାଲି);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	ଯଦି (run_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		result = 1;
	}

	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout(ପୂର୍ଣ୍ଣସଂଖ୍ୟା submodule_progress,
		    ସଂରଚନା list_objects_filter_options *filter_options,
		    ପୂର୍ଣ୍ଣସଂଖ୍ୟା filter_submodules,
		    ଗଣନା ref_storage_format ref_storage_format)
{
	ସଂରଚନା object_id oid;
	ଅକ୍ଷର *head;
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ସଂରଚନା unpack_trees_options opts;
	ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;
	ସଂରଚନା ବୃକ୍ଷ_desc t;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;
	ସଂରଚନା run_hooks_opt hook_opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	ଯଦି (option_no_checkout)
		ଫେରନ୍ତୁ 0;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, ଖାଲି);
	ଯଦି (!head) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(head, "HEAD")) {
		ଯଦି (advice_enabled(ADVICE_DETACHED_HEAD))
			detach_advice(oid_to_hex(&oid));
		FREE_AND_NULL(head);
	} ଅନ୍ୟଥା {
		ଯଦି (!starts_with(head, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	setup_work_ବୃକ୍ଷ(the_repository);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, ଆକାର opts);
	opts.update = 1;
	opts.merge = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_merge;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	init_checkout_metadata(&opts.meta, head, &oid, ଖାଲି);

	ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository, &oid);
	ଯଦି (!ବୃକ୍ଷ)
		die(_("unable to parse commit %s"), oid_to_hex(&oid));
	ଯଦି (repo_parse_ବୃକ୍ଷ(the_repository, ବୃକ୍ଷ) < 0)
		exit(128);
	init_ବୃକ୍ଷ_desc(&t, &ବୃକ୍ଷ->object.oid, ବୃକ୍ଷ->buffer, ବୃକ୍ଷ->size);
	ଯଦି (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	ମୁକ୍ତ(head);

	ଯଦି (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.args, oid_to_hex(null_oid(the_hash_algo)),
		     oid_to_hex(&oid), "1", ଖାଲି);
	err |= run_hooks_opt(the_repository, "post-checkout", &hook_opt);

	ଯଦି (!err && (option_recurse_submodules.nr > 0)) {
		ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;
		strvec_pushl(&cmd.args, "submodule", "update", "--require-init",
			     "--recursive", ଖାଲି);

		ଯଦି (option_shallow_submodules == 1)
			strvec_ଠେଲ(&cmd.args, "--depth=1");

		ଯଦି (max_jobs != -1)
			strvec_pushf(&cmd.args, "--jobs=%d", max_jobs);

		ଯଦି (submodule_progress)
			strvec_ଠେଲ(&cmd.args, "--progress");

		ଯଦି (option_verbosity < 0)
			strvec_ଠେଲ(&cmd.args, "--quiet");

		ଯଦି (option_remote_submodules) {
			strvec_ଠେଲ(&cmd.args, "--remote");
			strvec_ଠେଲ(&cmd.args, "--no-fetch");
		}

		ଯଦି (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(ref_storage_format));

		ଯଦି (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.args, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		ଯଦି (option_single_branch >= 0)
			strvec_ଠେଲ(&cmd.args, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = run_command(&cmd);
	}

	ଫେରନ୍ତୁ err;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_clone_config(ଅଚଳ ଅକ୍ଷର *k, ଅଚଳ ଅକ୍ଷର *v,
			    ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(k, "clone.defaultremotename")) {
		ଯଦି (!v)
			ଫେରନ୍ତୁ config_error_nonbool(k);
		ମୁକ୍ତ(remote_name);
		remote_name = xstrdup(v);
	}
	ଯଦି (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	ଯଦି (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	ଫେରନ୍ତୁ git_default_config(k, v, ctx, cb);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_one_config(ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			    ଅଚଳ ସଂରଚନା config_context *ctx,
			    ଶୂନ୍ୟ *data)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା apply_failed = git_clone_config(ଚାବି, ମୂଲ୍ୟ, ctx, data);
	ଯଦି (apply_failed)
		ଫେରନ୍ତୁ apply_failed;

	ଫେରନ୍ତୁ repo_config_set_multivar_gently(the_repository, ଚାବି,
					       ମୂଲ୍ୟ ? ମୂଲ୍ୟ : "true",
					       CONFIG_REGEX_NONE, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ write_config(ସଂରଚନା string_list *config)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < config->nr; i++) {
		ଯଦି (git_config_parse_parameter(config->ବସ୍ତୁଗୁଡ଼ିକ[i].string,
					       write_one_config, ଖାଲି) < 0)
			die(_("unable to write parameters to config file"));
	}
}

ସ୍ଥିର ଶୂନ୍ୟ write_refspec_config(ଅଚଳ ଅକ୍ଷର *src_ref_prefix,
		ଅଚଳ ସଂରଚନା ref *our_head_points_at,
		ଅଚଳ ସଂରଚନା ref *remote_head_points_at,
		ସଂରଚନା strbuf *branch_ଶୀର୍ଷ)
{
	ସଂରଚନା strbuf ଚାବି = STRBUF_INIT;
	ସଂରଚନା strbuf ମୂଲ୍ୟ = STRBUF_INIT;

	ଯଦି (option_mirror || !option_bare) {
		ଯଦି (option_single_branch && !option_mirror) {
			ଯଦି (option_branch) {
				ଯଦି (starts_with(our_head_points_at->name, "refs/tags/"))
					strbuf_addf(&ମୂଲ୍ୟ, "+%s:%s", our_head_points_at->name,
						our_head_points_at->name);
				ଅନ୍ୟଥା
					strbuf_addf(&ମୂଲ୍ୟ, "+%s:%s%s", our_head_points_at->name,
						branch_ଶୀର୍ଷ->buf, option_branch);
			} ଅନ୍ୟଥା ଯଦି (remote_head_points_at) {
				ଅଚଳ ଅକ୍ଷର *head = remote_head_points_at->name;
				ଯଦି (!skip_prefix(head, "refs/heads/", &head))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&ମୂଲ୍ୟ, "+%s:%s%s", remote_head_points_at->name,
						branch_ଶୀର୍ଷ->buf, head);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} ଅନ୍ୟଥା {
			strbuf_addf(&ମୂଲ୍ୟ, "+%s*:%s*", src_ref_prefix, branch_ଶୀର୍ଷ->buf);
		}
		/* Configure the remote */
		ଯଦି (ମୂଲ୍ୟ.len) {
			strbuf_addf(&ଚାବି, "remote.%s.fetch", remote_name);
			repo_config_set_multivar(the_repository, ଚାବି.buf, ମୂଲ୍ୟ.buf, "^$", 0);
			strbuf_reset(&ଚାବି);

			ଯଦି (option_mirror) {
				strbuf_addf(&ଚାବି, "remote.%s.mirror", remote_name);
				repo_config_set(the_repository, ଚାବି.buf, "true");
				strbuf_reset(&ଚାବି);
			}
		}
	}

	strbuf_release(&ଚାବି);
	strbuf_release(&ମୂଲ୍ୟ);
}

ସ୍ଥିର ଶୂନ୍ୟ dissociate_from_references(ଶୂନ୍ୟ)
{
	ଅକ୍ଷର *alternates = repo_git_path(the_repository, "objects/info/alternates");

	ଯଦି (!access(alternates, F_OK)) {
		ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.args, "repack", "-a", "-d", ଖାଲି);
		ଯଦି (run_command(&cmd))
			die(_("cannot repack to clean up"));
		ଯଦି (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	ମୁକ୍ତ(alternates);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା path_exists(ଅଚଳ ଅକ୍ଷର *path)
{
	ସଂରଚନା stat sb;
	ଫେରନ୍ତୁ !stat(path, &sb);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_clone(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	      ଅଚଳ ଅକ୍ଷର **argv,
	      ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repository UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_bundle = 0, is_local;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା reject_shallow = 0;
	ଅଚଳ ଅକ୍ଷର *repo_name, *repo, *work_ବୃକ୍ଷ, *git_dir;
	ଅକ୍ଷର *repo_to_free = ଖାଲି;
	ଅକ୍ଷର *path = ଖାଲି, *dir, *display_repo = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_exists, real_dest_exists = 0;
	ଅଚଳ ସଂରଚନା ref *refs, *remote_head;
	ସଂରଚନା ref *remote_head_points_at = ଖାଲି;
	ଅଚଳ ସଂରଚନା ref *our_head_points_at;
	ଅକ୍ଷର *unborn_head = ଖାଲି;
	ସଂରଚନା ref *mapped_refs = ଖାଲି;
	ଅଚଳ ସଂରଚନା ref *ref;
	ସଂରଚନା strbuf ଚାବି = STRBUF_INIT;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା strbuf branch_ଶୀର୍ଷ = STRBUF_INIT, reflog_ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା transport *transport = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *src_ref_prefix = "refs/heads/";
	ସଂରଚନା remote *remote;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0, complete_refs_before_fetch = 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା submodule_progress;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା filter_submodules = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash_algo;
	ଗଣନା ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_not_override_repo_unix_permissions = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_reject_shallow = -1; /* unspecified */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା deepen = 0;
	ଅକ୍ଷର *option_template = ଖାଲି, *option_depth = ଖାଲି, *option_since = ଖାଲି;
	ଅକ୍ଷର *option_origin = ଖାଲି;
	ସଂରଚନା string_list option_not = STRING_LIST_INIT_NODUP;
	ଅଚଳ ଅକ୍ଷର *real_git_dir = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *ref_storage_format_uri = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *option_upload_pack = "git-upload-pack";
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_progress = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_sparse_checkout = 0;
	ଗଣନା transport_family family = TRANSPORT_FAMILY_ALL;
	ସଂରଚନା string_list option_config = STRING_LIST_INIT_DUP;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_dissociate = 0;
	ସଂରଚନା list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_filter_submodules = -1; /* unspecified */
	ସଂରଚନା string_list server_options = STRING_LIST_INIT_NODUP;
	ଅଚଳ ଅକ୍ଷର *bundle_uri = ଖାଲି;
	ଅକ୍ଷର *option_rev = ଖାଲି;

	ସଂରଚନା clone_opts opts = CLONE_OPTS_INIT;

	ସଂରଚନା transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	ସଂରଚନା option builtin_clone_options[] = {
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
			.ମୂଲ୍ୟ = &option_recurse_submodules,
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
	ସଂରଚନା strvec alternates = STRVEC_INIT;

	ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		ଖାଲି
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_identity("clone");

	repo_config(the_repository, git_clone_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, builtin_clone_options,
			     builtin_clone_usage, 0);

	ଯଦି (argc > 2)
		usage_ସନ୍ଦେଶ_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	ଯଦି (argc == 0)
		usage_ସନ୍ଦେଶ_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	ଯଦି (option_depth || option_since || option_not.nr)
		deepen = 1;
	ଯଦି (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	ଯଦି (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, ଖାଲି);
		ଯଦି (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	ଯଦି (option_mirror) {
		option_bare = 1;
		option_tags = 0;
	}

	ଯଦି (option_bare) {
		ଯଦି (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	ଯଦି (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	path = get_repo_path(repo_name, &is_bundle);
	ଯଦି (path) {
		FREE_AND_NULL(path);
		repo = repo_to_free = absolute_pathdup(repo_name);
	} ଅନ୍ୟଥା ଯଦି (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} ଅନ୍ୟଥା
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	ଯଦି (option_depth && atoi(option_depth) < 1)
		die(_("depth %s is not a positive number"), option_depth);

	ଯଦି (argc == 2)
		dir = xstrdup(argv[1]);
	ଅନ୍ୟଥା
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = path_exists(dir);
	ଯଦି (dest_exists && !is_empty_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	ଯଦି (real_git_dir) {
		real_dest_exists = path_exists(real_git_dir);
		ଯଦି (real_dest_exists && !is_empty_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_ସନ୍ଦେଶ, "clone: from %s",
		    display_repo ? display_repo : repo);
	ମୁକ୍ତ(display_repo);

	ଯଦି (option_bare)
		work_ବୃକ୍ଷ = ଖାଲି;
	ଅନ୍ୟଥା {
		work_ବୃକ୍ଷ = getenv("GIT_WORK_TREE");
		ଯଦି (work_ବୃକ୍ଷ && path_exists(work_ବୃକ୍ଷ))
			die(_("working tree '%s' already exists."), work_ବୃକ୍ଷ);
	}

	ଯଦି (option_bare || work_ବୃକ୍ଷ)
		git_dir = xstrdup(dir);
	ଅନ୍ୟଥା {
		work_ବୃକ୍ଷ = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_ଠେଲ_common(remove_junk_on_signal);

	ଯଦି (!option_bare) {
		ଯଦି (safe_create_leading_directories_const(the_repository, work_ବୃକ୍ଷ) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_ବୃକ୍ଷ);
		ଯଦି (dest_exists)
			junk_work_ବୃକ୍ଷ_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		ଅନ୍ୟଥା ଯଦି (mkdir(work_ବୃକ୍ଷ, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_ବୃକ୍ଷ);
		junk_work_ବୃକ୍ଷ = work_ବୃକ୍ଷ;
	}

	ଯଦି (real_git_dir) {
		ଯଦି (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} ଅନ୍ୟଥା {
		ଯଦି (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	ଯଦି (safe_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	ଯଦି (0 <= option_verbosity) {
		ଯଦି (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		ଅନ୍ୟଥା
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	ଯଦି (option_recurse_submodules.nr > 0) {
		ସଂରଚନା string_list_item *item;
		ସଂରଚନା strbuf sb = STRBUF_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା val;

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

		ଯଦି (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		ଯଦି (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		ଅନ୍ୟଥା ଯଦି (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} ଅନ୍ୟଥା ଯଦି (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_ବୃକ୍ଷ,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, ଖାଲି);

	ଯଦି (real_git_dir) {
		ମୁକ୍ତ((ଅକ୍ଷର *)git_dir);
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

	refs_create_refdir_stubs(the_repository, git_dir, ଖାଲି);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, ଖାଲି);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	ଯଦି (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	ଯଦି (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	ଯଦି (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	ଯଦି (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	ଯଦି (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	ଯଦି (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	ଯଦି (option_origin) {
		ମୁକ୍ତ(remote_name);
		remote_name = xstrdup(option_origin);
	}

	ଯଦି (!remote_name)
		remote_name = xstrdup("origin");

	ଯଦି (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	ଯଦି (option_bare) {
		ଯଦି (option_mirror)
			src_ref_prefix = "refs/";
		strbuf_addstr(&branch_ଶୀର୍ଷ, src_ref_prefix);

		repo_config_set(the_repository, "core.bare", "true");
	} ଅନ୍ୟଥା ଯଦି (!option_rev) {
		strbuf_addf(&branch_ଶୀର୍ଷ, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&ଚାବି, "remote.%s.url", remote_name);
	repo_config_set(the_repository, ଚାବି.buf, repo);
	strbuf_reset(&ଚାବି);

	ଯଦି (!option_tags) {
		strbuf_addf(&ଚାବି, "remote.%s.tagOpt", remote_name);
		repo_config_set(the_repository, ଚାବି.buf, "--no-tags");
		strbuf_reset(&ଚାବି);
	}

	remote = remote_get_early(remote_name);

	ଯଦି (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_prefix,
				branch_ଶୀର୍ଷ.buf);

	path = get_repo_path(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && path && !is_bundle;
	ଯଦି (is_local) {
		ଯଦି (option_depth)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		ଯଦି (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		ଯଦି (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		ଯଦି (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		ଯଦି (!access(mkpath("%s/shallow", path), F_OK)) {
			ଯଦି (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			ଯଦି (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	ଯଦି (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? path : ଖାଲି);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, path ? path : remote->url.v[0]);
	transport_set_verbosity(transport, option_verbosity, option_progress);
	transport->family = family;
	transport->cloning = 1;

	ଯଦି (is_bundle) {
		ସଂରଚନା bundle_header header = BUNDLE_HEADER_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = read_bundle_header(path, &header);
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_filter = header.filter.choice != LOFC_DISABLED;

		ଯଦି (fd > 0)
			close(fd);
		bundle_header_release(&header);
		ଯଦି (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_set_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	ଯଦି (reject_shallow)
		transport_set_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	ଯଦି (option_depth)
		transport_set_option(transport, TRANS_OPT_DEPTH,
				     option_depth);
	ଯଦି (option_since)
		transport_set_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	ଯଦି (option_not.nr)
		transport_set_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (ଅଚଳ ଅକ୍ଷର *)&option_not);
	ଯଦି (option_single_branch) {
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		ଯଦି (option_branch)
			opts.wants_head = 0;
	}

	ଯଦି (option_upload_pack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	ଯଦି (server_options.nr)
		transport->server_options = &server_options;

	ଯଦି (filter_options.choice) {
		ଅଚଳ ଅକ୍ଷର *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	ଯଦି (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	ଯଦି (option_rev) {
		option_tags = 0;
		option_single_branch = 0;
		opts.wants_head = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	ଯଦି (option_tags || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, TAG_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	ଯଦି (option_branch)
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
	ଯଦି (opts.wants_head || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_ଠେଲ(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_version(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_set_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, ଖାଲି, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	ଯଦି (bundle_uri) {
		ସଂରଚନା remote_state *state;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = ଖାଲି;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		ଯଦି (repo_init(the_repository, git_dir, work_ବୃକ୍ଷ))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		ଅନ୍ୟଥା ଯଦି (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		ଅନ୍ୟଥା ଯଦି (has_heuristic)
			repo_config_set_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		ମୁକ୍ତ(the_repository->remote_state);
		the_repository->remote_state = state;
	} ଅନ୍ୟଥା {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		ଯଦି (transport->bundles &&
		    hashmap_get_size(&transport->bundles->bundles)) {
			ସଂରଚନା remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = ଖାଲି;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			ଯଦି (repo_init(the_repository, git_dir, work_ବୃକ୍ଷ))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			ଅନ୍ୟଥା ଯଦି (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			ମୁକ୍ତ(the_repository->remote_state);
			the_repository->remote_state = state;
		} ଅନ୍ୟଥା {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	ଯଦି (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	ଯଦି (mapped_refs) {
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
		ପାଇଁ (ref = refs; ref; ref = ref->next)
			ଯଦି (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				ଭାଙ୍ଗ;
			}

		ଯଦି (!is_local && !complete_refs_before_fetch) {
			ଯଦି (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_head = find_ref_by_name(refs, "HEAD");
	remote_head_points_at = guess_remote_head(remote_head, mapped_refs,
						  REMOTE_GUESS_HEAD_QUIET);

	ଯଦି (option_branch) {
		our_head_points_at = find_remote_branch(mapped_refs, option_branch);
		ଯଦି (!our_head_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} ଅନ୍ୟଥା ଯଦି (option_rev) {
		our_head_points_at = mapped_refs;
		ଯଦି (!our_head_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} ଅନ୍ୟଥା ଯଦି (remote_head_points_at) {
		our_head_points_at = remote_head_points_at;
	} ଅନ୍ୟଥା ଯଦି (remote_head) {
		our_head_points_at = ଖାଲି;
	} ଅନ୍ୟଥା {
		ଅକ୍ଷର *to_free = ଖାଲି;
		ଅଚଳ ଅକ୍ଷର *branch;

		ଯଦି (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		ଯଦି (transport_ls_refs_options.unborn_head_target &&
		    skip_prefix(transport_ls_refs_options.unborn_head_target,
				"refs/heads/", &branch)) {
			unborn_head  = xstrdup(transport_ls_refs_options.unborn_head_target);
		} ଅନ୍ୟଥା {
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

		ମୁକ୍ତ(to_free);
	}

	ଯଦି (!option_rev)
		write_refspec_config(src_ref_prefix, our_head_points_at,
				     remote_head_points_at, &branch_ଶୀର୍ଷ);

	ଯଦି (filter_options.choice)
		partial_clone_register(remote_name, &filter_options);

	ଯଦି (is_local)
		clone_local(path, git_dir);
	ଅନ୍ୟଥା ଯଦି (mapped_refs && complete_refs_before_fetch) {
		ଯଦି (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_head_points_at,
			   branch_ଶୀର୍ଷ.buf, reflog_ସନ୍ଦେଶ.buf, transport,
			   !is_local);

	update_head(&opts, our_head_points_at, remote_head, unborn_head, reflog_ସନ୍ଦେଶ.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_progress = transport->progress;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	ଯଦି (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	ଯଦି (option_sparse_checkout && git_sparse_checkout_init(dir))
		ଫେରନ୍ତୁ 1;

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

	ମୁକ୍ତ(remote_name);
	strbuf_release(&reflog_ସନ୍ଦେଶ);
	strbuf_release(&branch_ଶୀର୍ଷ);
	strbuf_release(&buf);
	strbuf_release(&ଚାବି);
	free_refs(mapped_refs);
	free_refs(remote_head_points_at);
	ମୁକ୍ତ(unborn_head);
	ମୁକ୍ତ(dir);
	ମୁକ୍ତ(path);
	ମୁକ୍ତ(repo_to_free);
	junk_mode = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	ଫେରନ୍ତୁ err;
}
