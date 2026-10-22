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

#অন্তর্ভাৱ "builtin.h"

#অন্তর্ভাৱ "abspath.h"
#অন্তর্ভাৱ "advice.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "copy.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "lockfile.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "refspec.h"
#অন্তর্ভাৱ "object-file.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "tree.h"
#অন্তর্ভাৱ "tree-walk.h"
#অন্তর্ভাৱ "unpack-trees.h"
#অন্তর্ভাৱ "transport.h"
#অন্তর্ভাৱ "strbuf.h"
#অন্তর্ভাৱ "dir.h"
#অন্তর্ভাৱ "dir-iterator.h"
#অন্তর্ভাৱ "iterator.h"
#অন্তর্ভাৱ "sigchain.h"
#অন্তর্ভাৱ "branch.h"
#অন্তর্ভাৱ "remote.h"
#অন্তর্ভাৱ "run-command.h"
#অন্তর্ভাৱ "setup.h"
#অন্তর্ভাৱ "connected.h"
#অন্তর্ভাৱ "packfile.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "pkt-line.h"
#অন্তর্ভাৱ "list-objects-filter-options.h"
#অন্তর্ভাৱ "hook.h"
#অন্তর্ভাৱ "bundle.h"
#অন্তর্ভাৱ "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

সংরচনা clone_opts {
	পূর্ণাঙ্ক wants_head;
	পূর্ণাঙ্ক detach;
};
#define CLONE_OPTS_INIT { \
	.wants_head = 1 /* default enabled */ \
}

স্থির পূর্ণাঙ্ক option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
স্থির পূর্ণাঙ্ক option_local = -1, option_no_hardlinks, option_shared;
স্থির পূর্ণাঙ্ক option_tags = 1; /* default enabled */
স্থির পূর্ণাঙ্ক option_shallow_submodules;
স্থির পূর্ণাঙ্ক config_reject_shallow = -1;    /* unspecified */
স্থির ৱর্ণ *remote_name = রিক্ত;
স্থির ৱর্ণ *option_branch = রিক্ত;
স্থির পূর্ণাঙ্ক option_verbosity;
স্থির সংরচনা string_list option_required_reference = STRING_LIST_INIT_NODUP;
স্থির সংরচনা string_list option_optional_reference = STRING_LIST_INIT_NODUP;
স্থির পূর্ণাঙ্ক max_jobs = -1;
স্থির সংরচনা string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
স্থির পূর্ণাঙ্ক config_filter_submodules = -1;    /* unspecified */
স্থির পূর্ণাঙ্ক option_remote_submodules;

স্থির পূর্ণাঙ্ক recurse_submodules_cb(ধ্রুৱ সংরচনা option *opt,
				 ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	যদি (unset)
		string_list_clear((সংরচনা string_list *)opt->মান, 0);
	অন্যথা যদি (arg)
		string_list_append((সংরচনা string_list *)opt->মান, arg);
	অন্যথা
		string_list_append((সংরচনা string_list *)opt->মান,
				   (ধ্রুৱ ৱর্ণ *)opt->defval);

	ঘূৰাই 0;
}

স্থির ধ্রুৱ ৱর্ণ *get_repo_path_1(সংরচনা strbuf *path, পূর্ণাঙ্ক *is_bundle)
{
	স্থির ধ্রুৱ ৱর্ণ *suffix[] = { "/.git", "", ".git/.git", ".git" };
	স্থির ধ্রুৱ ৱর্ণ *bundle_suffix[] = { ".bundle", "" };
	size_t baselen = path->len;
	সংরচনা stat st;
	পূর্ণাঙ্ক i;

	বাবে (i = 0; i < ARRAY_SIZE(suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, suffix[i]);
		যদি (stat(path->buf, &st))
			অনুৱর্ত;
		যদি (S_ISDIR(st.st_mode) && is_git_directory(path->buf)) {
			*is_bundle = 0;
			ঘূৰাই path->buf;
		} অন্যথা যদি (S_ISREG(st.st_mode) && st.st_size > 8) {
			/* Is it a "gitfile"? */
			ৱর্ণ signature[8];
			ধ্রুৱ ৱর্ণ *dst;
			পূর্ণাঙ্ক len, fd = open(path->buf, O_RDONLY);
			যদি (fd < 0)
				অনুৱর্ত;
			len = read_in_full(fd, signature, 8);
			close(fd);
			যদি (len != 8 || strncmp(signature, "gitdir: ", 8))
				অনুৱর্ত;
			dst = read_gitfile(path->buf);
			যদি (dst) {
				*is_bundle = 0;
				ঘূৰাই dst;
			}
		}
	}

	বাবে (i = 0; i < ARRAY_SIZE(bundle_suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, bundle_suffix[i]);
		যদি (!stat(path->buf, &st) && S_ISREG(st.st_mode)) {
			*is_bundle = 1;
			ঘূৰাই path->buf;
		}
	}

	ঘূৰাই রিক্ত;
}

স্থির ৱর্ণ *get_repo_path(ধ্রুৱ ৱর্ণ *repo, পূর্ণাঙ্ক *is_bundle)
{
	সংরচনা strbuf path = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *raw;
	ৱর্ণ *canon;

	strbuf_addstr(&path, repo);
	raw = get_repo_path_1(&path, is_bundle);
	canon = raw ? absolute_pathdup(raw) : রিক্ত;
	strbuf_release(&path);
	ঘূৰাই canon;
}

সংরচনা collect_alternates_data {
	সংরচনা strvec *alternates;
	bool required;
};

স্থির পূর্ণাঙ্ক collect_one_alternate(সংরচনা string_list_item *item, শূন্য *cb_data)
{
	সংরচনা collect_alternates_data *data = cb_data;
	সংরচনা strbuf err = STRBUF_INIT;
	ৱর্ণ *ref_git = compute_alternate_path(item->string, &err);

	যদি (!ref_git) {
		যদি (data->required)
			die("%s", err.buf);
		অন্যথা
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				item->string, err.buf);
	} অন্যথা {
		strvec_pushf(data->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	মুক্ত(ref_git);
	ঘূৰাই 0;
}

স্থির শূন্য read_alternates(সংরচনা strvec *alternates, ধ্রুৱ ৱর্ণ *src_repo)
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
	সংরচনা strbuf path = STRBUF_INIT;
	সংরচনা strbuf সাৰি = STRBUF_INIT;

	strbuf_addf(&path, "%s/objects/info/alternates", src_repo);

	in = fopen(path.buf, "r");
	যদি (!in) {
		যদি (errno == ENOENT)
			গচ্ছ out;
		die_errno("could not read alternates file '%s'", path.buf);
	}

	যেতিয়ালৈ (strbuf_getline(&সাৰি, in) != EOF) {
		ৱর্ণ *abs_path;
		যদি (!সাৰি.len || সাৰি.buf[0] == '#')
			অনুৱর্ত;
		যদি (is_absolute_path(সাৰি.buf)) {
			strvec_ঠেলো(alternates, সাৰি.buf);
			অনুৱর্ত;
		}
		abs_path = mkpathdup("%s/objects/%s", src_repo, সাৰি.buf);
		যদি (!normalize_path_copy(abs_path, abs_path))
			strvec_ঠেলো(alternates, abs_path);
		অন্যথা
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, সাৰি.buf);
		মুক্ত(abs_path);
	}

out:
	strbuf_release(&path);
	strbuf_release(&সাৰি);
	যদি (in)
		fclose(in);
}

স্থির শূন্য collect_alternates(সংরচনা strvec *alternates,
			       ধ্রুৱ ৱর্ণ *local_source_repo)
{
	যদি (option_required_reference.nr || option_optional_reference.nr) {
		সংরচনা collect_alternates_data data = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &data);
		data.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &data);
	}

	যদি (local_source_repo) {
		সংরচনা strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_source_repo);
		যদি (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		অন্যথা
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

স্থির শূন্য mkdir_if_missing(ধ্রুৱ ৱর্ণ *pathname, mode_t mode)
{
	সংরচনা stat st;

	যদি (!mkdir(pathname, mode))
		ঘূৰাই;

	যদি (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	অন্যথা যদি (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	অন্যথা যদি (!S_ISDIR(st.st_mode))
		die(_("%s exists and is not a directory"), pathname);
}

স্থির শূন্য copy_or_link_directory(সংরচনা strbuf *src, সংরচনা strbuf *dest,
				   ধ্রুৱ ৱর্ণ *src_repo)
{
	পূর্ণাঙ্ক src_len, dest_len;
	সংরচনা dir_iterator *iter;
	পূর্ণাঙ্ক iter_status;

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
	die_upon_dubious_ownership(রিক্ত, রিক্ত, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	যদি (!iter) {
		যদি (errno == ENOTDIR) {
			পূর্ণাঙ্ক saved_errno = errno;
			সংরচনা stat st;

			যদি (!lstat(src->buf, &st) && S_ISLNK(st.st_mode))
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

	যেতিয়ালৈ ((iter_status = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_path);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_path);

		যদি (S_ISLNK(iter->st.st_mode))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_path);

		যদি (S_ISDIR(iter->st.st_mode)) {
			mkdir_if_missing(dest->buf, 0777);
			অনুৱর্ত;
		}

		/* Alternates were already handled earlier. */
		যদি (!fspathcmp(iter->relative_path, "info/alternates"))
			অনুৱর্ত;

		যদি (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		যদি (!option_no_hardlinks) {
			যদি (!link(src->buf, dest->buf)) {
				সংরচনা stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				যদি (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				যদি (st.st_mode != iter->st.st_mode ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_size != iter->st.st_size ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				অনুৱর্ত;
			}
			যদি (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		যদি (copy_file_with_time(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	যদি (iter_status != ITER_DONE) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

স্থির শূন্য clone_local(ধ্রুৱ ৱর্ণ *src_repo, ধ্রুৱ ৱর্ণ *dest_repo)
{
	যদি (!option_shared) {
		সংরচনা strbuf src = STRBUF_INIT;
		সংরচনা strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	যদি (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

স্থির ধ্রুৱ ৱর্ণ *junk_work_বৃক্ষ;
স্থির পূর্ণাঙ্ক junk_work_বৃক্ষ_flags;
স্থির ধ্রুৱ ৱর্ণ *junk_git_dir;
স্থির পূর্ণাঙ্ক junk_git_dir_flags;
স্থির গণনা {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_mode = JUNK_LEAVE_NONE;

স্থির ধ্রুৱ ৱর্ণ junk_leave_repo_বাৰ্তা[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

স্থির শূন্য remove_junk(শূন্য)
{
	সংরচনা strbuf sb = STRBUF_INIT;

	ৱিকল্প (junk_mode) {
	পক্ষ JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_বাৰ্তা));
		/* fall-through */
	পক্ষ JUNK_LEAVE_ALL:
		ঘূৰাই;
	স্ৱভাৱ:
		/* proceed to removal */
		ভঙ্গ;
	}

	যদি (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	যদি (junk_work_বৃক্ষ) {
		strbuf_addstr(&sb, junk_work_বৃক্ষ);
		remove_dir_recursively(&sb, junk_work_বৃক্ষ_flags);
	}
	strbuf_release(&sb);
}

স্থির শূন্য remove_junk_on_signal(পূর্ণাঙ্ক signo)
{
	remove_junk();
	sigchain_বেৰ_কৰো(signo);
	উত্থাপয(signo);
}

স্থির সংরচনা ref *find_remote_branch(ধ্রুৱ সংরচনা ref *refs, ধ্রুৱ ৱর্ণ *branch)
{
	সংরচনা ref *ref;
	সংরচনা strbuf head = STRBUF_INIT;
	strbuf_addstr(&head, "refs/heads/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	যদি (ref)
		ঘূৰাই ref;

	strbuf_addstr(&head, "refs/tags/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	ঘূৰাই ref;
}

স্থির সংরচনা ref *wanted_peer_refs(সংরচনা clone_opts *opts,
				    ধ্রুৱ সংরচনা ref *refs,
				    সংরচনা refspec *refspec)
{
	সংরচনা ref *local_refs = রিক্ত;
	সংরচনা ref **tail = &local_refs;
	সংরচনা ref *to_free = রিক্ত;

	যদি (opts->wants_head) {
		সংরচনা ref *head = copy_ref(find_ref_by_name(refs, "HEAD"));
		যদি (head)
			tail_link_ref(head, &tail);
		যদি (option_single_branch)
			refs = to_free =
				guess_remote_head(head, refs,
						  REMOTE_GUESS_HEAD_QUIET);
	} অন্যথা যদি (option_single_branch) {
		local_refs = রিক্ত;
		tail = &local_refs;
		refs = to_free = copy_ref(find_remote_branch(refs, option_branch));
	}

	বাবে (size_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->বস্তুসমূহ[i], &tail, 0);

	free_one_ref(to_free);

	ঘূৰাই local_refs;
}

স্থির শূন্য write_remote_refs(ধ্রুৱ সংরচনা ref *local_refs)
{
	ধ্রুৱ সংরচনা ref *r;

	সংরচনা ref_transaction *t;
	সংরচনা strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	যদি (!t)
		die("%s", err.buf);

	বাবে (r = local_refs; r; r = r->next) {
		যদি (!r->peer_ref)
			অনুৱর্ত;
		যদি (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   রিক্ত, 0, রিক্ত, &err))
			die("%s", err.buf);
	}

	যদি (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

স্থির শূন্য write_followtags(ধ্রুৱ সংরচনা ref *refs, ধ্রুৱ ৱর্ণ *বাৰ্তা)
{
	ধ্রুৱ সংরচনা ref *ref;
	বাবে (ref = refs; ref; ref = ref->next) {
		যদি (!starts_with(ref->name, "refs/tags/"))
			অনুৱর্ত;
		যদি (ends_with(ref->name, "^{}"))
			অনুৱর্ত;
		যদি (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			অনুৱর্ত;
		refs_update_ref(get_main_ref_store(the_repository), বাৰ্তা,
				ref->name, &ref->old_oid, রিক্ত, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

স্থির ধ্রুৱ সংরচনা object_id *iterate_ref_map(শূন্য *cb_data)
{
	সংরচনা ref **rm = cb_data;
	সংরচনা ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	যেতিয়ালৈ (ref && !ref->peer_ref)
		ref = ref->next;
	যদি (!ref)
		ঘূৰাই রিক্ত;

	*rm = ref->next;
	ঘূৰাই &ref->old_oid;
}

স্থির শূন্য update_remote_refs(ধ্রুৱ সংরচনা ref *refs,
			       ধ্রুৱ সংরচনা ref *mapped_refs,
			       ধ্রুৱ সংরচনা ref *remote_head_points_at,
			       ধ্রুৱ ৱর্ণ *branch_শীৰ্ষ,
			       ধ্রুৱ ৱর্ণ *বাৰ্তা,
			       সংরচনা transport *transport,
			       পূর্ণাঙ্ক check_connectivity)
{
	ধ্রুৱ সংরচনা ref *rm = mapped_refs;

	যদি (check_connectivity) {
		সংরচনা check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.progress = transport->progress;

		যদি (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	যদি (refs) {
		write_remote_refs(mapped_refs);
		যদি (option_single_branch && option_tags)
			write_followtags(refs, বাৰ্তা);
	}

	যদি (remote_head_points_at && remote_head_points_at->peer_ref && !option_bare) {
		সংরচনা strbuf head_ref = STRBUF_INIT;
		strbuf_addstr(&head_ref, branch_শীৰ্ষ);
		strbuf_addstr(&head_ref, "HEAD");
		যদি (refs_update_symref(get_main_ref_store(the_repository), head_ref.buf,
				       remote_head_points_at->peer_ref->name,
				       বাৰ্তা) < 0)
			die(_("unable to update %s"), head_ref.buf);
		strbuf_release(&head_ref);
	}
}

স্থির শূন্য update_head(সংরচনা clone_opts *opts, ধ্রুৱ সংরচনা ref *our, ধ্রুৱ সংরচনা ref *remote,
			ধ্রুৱ ৱর্ণ *unborn, ধ্রুৱ ৱর্ণ *বাৰ্তা)
{
	ধ্রুৱ ৱর্ণ *head;
	যদি (our && !opts->detach && skip_prefix(our->name, "refs/heads/", &head)) {
		/* Local default branch link */
		যদি (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, রিক্ত) < 0)
			die(_("unable to update HEAD"));
		যদি (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					বাৰ্তা, "HEAD", &our->old_oid, রিক্ত, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, head, remote_name, our->name);
		}
	} অন্যথা যদি (our) {
		সংরচনা commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), বাৰ্তা,
				"HEAD", &c->object.oid, রিক্ত, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} অন্যথা যদি (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), বাৰ্তা,
				"HEAD", &remote->old_oid, রিক্ত, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} অন্যথা যদি (unborn && skip_prefix(unborn, "refs/heads/", &head)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		যদি (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, রিক্ত) < 0)
			die(_("unable to update HEAD"));
		যদি (!option_bare)
			install_branch_config(0, head, remote_name, unborn);
	}
}

স্থির পূর্ণাঙ্ক git_sparse_checkout_init(ধ্রুৱ ৱর্ণ *repo)
{
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;
	পূর্ণাঙ্ক result = 0;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	strvec_pushl(&cmd.args, "-C", repo, "sparse-checkout", "set", রিক্ত);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	যদি (run_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		result = 1;
	}

	ঘূৰাই result;
}

স্থির পূর্ণাঙ্ক checkout(পূর্ণাঙ্ক submodule_progress,
		    সংরচনা list_objects_filter_options *filter_options,
		    পূর্ণাঙ্ক filter_submodules,
		    গণনা ref_storage_format ref_storage_format)
{
	সংরচনা object_id oid;
	ৱর্ণ *head;
	সংরচনা lock_file lock_file = LOCK_INIT;
	সংরচনা unpack_trees_options opts;
	সংরচনা বৃক্ষ *বৃক্ষ;
	সংরচনা বৃক্ষ_desc t;
	পূর্ণাঙ্ক err = 0;
	সংরচনা run_hooks_opt hook_opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	যদি (option_no_checkout)
		ঘূৰাই 0;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, রিক্ত);
	যদি (!head) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		ঘূৰাই 0;
	}
	যদি (!strcmp(head, "HEAD")) {
		যদি (advice_enabled(ADVICE_DETACHED_HEAD))
			detach_advice(oid_to_hex(&oid));
		FREE_AND_NULL(head);
	} অন্যথা {
		যদি (!starts_with(head, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	setup_work_বৃক্ষ(the_repository);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, পরিমাণ opts);
	opts.update = 1;
	opts.merge = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_merge;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	init_checkout_metadata(&opts.meta, head, &oid, রিক্ত);

	বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, &oid);
	যদি (!বৃক্ষ)
		die(_("unable to parse commit %s"), oid_to_hex(&oid));
	যদি (repo_parse_বৃক্ষ(the_repository, বৃক্ষ) < 0)
		exit(128);
	init_বৃক্ষ_desc(&t, &বৃক্ষ->object.oid, বৃক্ষ->buffer, বৃক্ষ->size);
	যদি (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	মুক্ত(head);

	যদি (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.args, oid_to_hex(null_oid(the_hash_algo)),
		     oid_to_hex(&oid), "1", রিক্ত);
	err |= run_hooks_opt(the_repository, "post-checkout", &hook_opt);

	যদি (!err && (option_recurse_submodules.nr > 0)) {
		সংরচনা child_process cmd = CHILD_PROCESS_INIT;
		strvec_pushl(&cmd.args, "submodule", "update", "--require-init",
			     "--recursive", রিক্ত);

		যদি (option_shallow_submodules == 1)
			strvec_ঠেলো(&cmd.args, "--depth=1");

		যদি (max_jobs != -1)
			strvec_pushf(&cmd.args, "--jobs=%d", max_jobs);

		যদি (submodule_progress)
			strvec_ঠেলো(&cmd.args, "--progress");

		যদি (option_verbosity < 0)
			strvec_ঠেলো(&cmd.args, "--quiet");

		যদি (option_remote_submodules) {
			strvec_ঠেলো(&cmd.args, "--remote");
			strvec_ঠেলো(&cmd.args, "--no-fetch");
		}

		যদি (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(ref_storage_format));

		যদি (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.args, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		যদি (option_single_branch >= 0)
			strvec_ঠেলো(&cmd.args, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = run_command(&cmd);
	}

	ঘূৰাই err;
}

স্থির পূর্ণাঙ্ক git_clone_config(ধ্রুৱ ৱর্ণ *k, ধ্রুৱ ৱর্ণ *v,
			    ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(k, "clone.defaultremotename")) {
		যদি (!v)
			ঘূৰাই config_error_nonbool(k);
		মুক্ত(remote_name);
		remote_name = xstrdup(v);
	}
	যদি (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	যদি (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	ঘূৰাই git_default_config(k, v, ctx, cb);
}

স্থির পূর্ণাঙ্ক write_one_config(ধ্রুৱ ৱর্ণ *চাবি, ধ্রুৱ ৱর্ণ *মান,
			    ধ্রুৱ সংরচনা config_context *ctx,
			    শূন্য *data)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	পূর্ণাঙ্ক apply_failed = git_clone_config(চাবি, মান, ctx, data);
	যদি (apply_failed)
		ঘূৰাই apply_failed;

	ঘূৰাই repo_config_set_multivar_gently(the_repository, চাবি,
					       মান ? মান : "true",
					       CONFIG_REGEX_NONE, 0);
}

স্থির শূন্য write_config(সংরচনা string_list *config)
{
	পূর্ণাঙ্ক i;

	বাবে (i = 0; i < config->nr; i++) {
		যদি (git_config_parse_parameter(config->বস্তুসমূহ[i].string,
					       write_one_config, রিক্ত) < 0)
			die(_("unable to write parameters to config file"));
	}
}

স্থির শূন্য write_refspec_config(ধ্রুৱ ৱর্ণ *src_ref_prefix,
		ধ্রুৱ সংরচনা ref *our_head_points_at,
		ধ্রুৱ সংরচনা ref *remote_head_points_at,
		সংরচনা strbuf *branch_শীৰ্ষ)
{
	সংরচনা strbuf চাবি = STRBUF_INIT;
	সংরচনা strbuf মান = STRBUF_INIT;

	যদি (option_mirror || !option_bare) {
		যদি (option_single_branch && !option_mirror) {
			যদি (option_branch) {
				যদি (starts_with(our_head_points_at->name, "refs/tags/"))
					strbuf_addf(&মান, "+%s:%s", our_head_points_at->name,
						our_head_points_at->name);
				অন্যথা
					strbuf_addf(&মান, "+%s:%s%s", our_head_points_at->name,
						branch_শীৰ্ষ->buf, option_branch);
			} অন্যথা যদি (remote_head_points_at) {
				ধ্রুৱ ৱর্ণ *head = remote_head_points_at->name;
				যদি (!skip_prefix(head, "refs/heads/", &head))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&মান, "+%s:%s%s", remote_head_points_at->name,
						branch_শীৰ্ষ->buf, head);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} অন্যথা {
			strbuf_addf(&মান, "+%s*:%s*", src_ref_prefix, branch_শীৰ্ষ->buf);
		}
		/* Configure the remote */
		যদি (মান.len) {
			strbuf_addf(&চাবি, "remote.%s.fetch", remote_name);
			repo_config_set_multivar(the_repository, চাবি.buf, মান.buf, "^$", 0);
			strbuf_reset(&চাবি);

			যদি (option_mirror) {
				strbuf_addf(&চাবি, "remote.%s.mirror", remote_name);
				repo_config_set(the_repository, চাবি.buf, "true");
				strbuf_reset(&চাবি);
			}
		}
	}

	strbuf_release(&চাবি);
	strbuf_release(&মান);
}

স্থির শূন্য dissociate_from_references(শূন্য)
{
	ৱর্ণ *alternates = repo_git_path(the_repository, "objects/info/alternates");

	যদি (!access(alternates, F_OK)) {
		সংরচনা child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.args, "repack", "-a", "-d", রিক্ত);
		যদি (run_command(&cmd))
			die(_("cannot repack to clean up"));
		যদি (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	মুক্ত(alternates);
}

স্থির পূর্ণাঙ্ক path_exists(ধ্রুৱ ৱর্ণ *path)
{
	সংরচনা stat sb;
	ঘূৰাই !stat(path, &sb);
}

পূর্ণাঙ্ক cmd_clone(পূর্ণাঙ্ক argc,
	      ধ্রুৱ ৱর্ণ **argv,
	      ধ্রুৱ ৱর্ণ *prefix,
	      সংরচনা repository *repository UNUSED)
{
	পূর্ণাঙ্ক is_bundle = 0, is_local;
	পূর্ণাঙ্ক reject_shallow = 0;
	ধ্রুৱ ৱর্ণ *repo_name, *repo, *work_বৃক্ষ, *git_dir;
	ৱর্ণ *repo_to_free = রিক্ত;
	ৱর্ণ *path = রিক্ত, *dir, *display_repo = রিক্ত;
	পূর্ণাঙ্ক dest_exists, real_dest_exists = 0;
	ধ্রুৱ সংরচনা ref *refs, *remote_head;
	সংরচনা ref *remote_head_points_at = রিক্ত;
	ধ্রুৱ সংরচনা ref *our_head_points_at;
	ৱর্ণ *unborn_head = রিক্ত;
	সংরচনা ref *mapped_refs = রিক্ত;
	ধ্রুৱ সংরচনা ref *ref;
	সংরচনা strbuf চাবি = STRBUF_INIT;
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা strbuf branch_শীৰ্ষ = STRBUF_INIT, reflog_বাৰ্তা = STRBUF_INIT;
	সংরচনা transport *transport = রিক্ত;
	ধ্রুৱ ৱর্ণ *src_ref_prefix = "refs/heads/";
	সংরচনা remote *remote;
	পূর্ণাঙ্ক err = 0, complete_refs_before_fetch = 1;
	পূর্ণাঙ্ক submodule_progress;
	পূর্ণাঙ্ক filter_submodules = 0;
	পূর্ণাঙ্ক hash_algo;
	গণনা ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	ধ্রুৱ পূর্ণাঙ্ক do_not_override_repo_unix_permissions = -1;
	পূর্ণাঙ্ক option_reject_shallow = -1; /* unspecified */
	পূর্ণাঙ্ক deepen = 0;
	ৱর্ণ *option_template = রিক্ত, *option_depth = রিক্ত, *option_since = রিক্ত;
	ৱর্ণ *option_origin = রিক্ত;
	সংরচনা string_list option_not = STRING_LIST_INIT_NODUP;
	ধ্রুৱ ৱর্ণ *real_git_dir = রিক্ত;
	ধ্রুৱ ৱর্ণ *ref_storage_format_uri = রিক্ত;
	ধ্রুৱ ৱর্ণ *option_upload_pack = "git-upload-pack";
	পূর্ণাঙ্ক option_progress = -1;
	পূর্ণাঙ্ক option_sparse_checkout = 0;
	গণনা transport_family family = TRANSPORT_FAMILY_ALL;
	সংরচনা string_list option_config = STRING_LIST_INIT_DUP;
	পূর্ণাঙ্ক option_dissociate = 0;
	সংরচনা list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	পূর্ণাঙ্ক option_filter_submodules = -1; /* unspecified */
	সংরচনা string_list server_options = STRING_LIST_INIT_NODUP;
	ধ্রুৱ ৱর্ণ *bundle_uri = রিক্ত;
	ৱর্ণ *option_rev = রিক্ত;

	সংরচনা clone_opts opts = CLONE_OPTS_INIT;

	সংরচনা transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	সংরচনা option builtin_clone_options[] = {
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
			.মান = &option_recurse_submodules,
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
	সংরচনা strvec alternates = STRVEC_INIT;

	ধ্রুৱ ৱর্ণ * ধ্রুৱ builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		রিক্ত
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_identity("clone");

	repo_config(the_repository, git_clone_config, রিক্ত);

	argc = parse_options(argc, argv, prefix, builtin_clone_options,
			     builtin_clone_usage, 0);

	যদি (argc > 2)
		usage_বাৰ্তা_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	যদি (argc == 0)
		usage_বাৰ্তা_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	যদি (option_depth || option_since || option_not.nr)
		deepen = 1;
	যদি (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	যদি (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, রিক্ত);
		যদি (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	যদি (option_mirror) {
		option_bare = 1;
		option_tags = 0;
	}

	যদি (option_bare) {
		যদি (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	যদি (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	path = get_repo_path(repo_name, &is_bundle);
	যদি (path) {
		FREE_AND_NULL(path);
		repo = repo_to_free = absolute_pathdup(repo_name);
	} অন্যথা যদি (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} অন্যথা
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	যদি (option_depth && atoi(option_depth) < 1)
		die(_("depth %s is not a positive number"), option_depth);

	যদি (argc == 2)
		dir = xstrdup(argv[1]);
	অন্যথা
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = path_exists(dir);
	যদি (dest_exists && !is_empty_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	যদি (real_git_dir) {
		real_dest_exists = path_exists(real_git_dir);
		যদি (real_dest_exists && !is_empty_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_বাৰ্তা, "clone: from %s",
		    display_repo ? display_repo : repo);
	মুক্ত(display_repo);

	যদি (option_bare)
		work_বৃক্ষ = রিক্ত;
	অন্যথা {
		work_বৃক্ষ = getenv("GIT_WORK_TREE");
		যদি (work_বৃক্ষ && path_exists(work_বৃক্ষ))
			die(_("working tree '%s' already exists."), work_বৃক্ষ);
	}

	যদি (option_bare || work_বৃক্ষ)
		git_dir = xstrdup(dir);
	অন্যথা {
		work_বৃক্ষ = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_ঠেলো_common(remove_junk_on_signal);

	যদি (!option_bare) {
		যদি (safe_create_leading_directories_const(the_repository, work_বৃক্ষ) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_বৃক্ষ);
		যদি (dest_exists)
			junk_work_বৃক্ষ_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		অন্যথা যদি (mkdir(work_বৃক্ষ, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_বৃক্ষ);
		junk_work_বৃক্ষ = work_বৃক্ষ;
	}

	যদি (real_git_dir) {
		যদি (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} অন্যথা {
		যদি (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	যদি (safe_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	যদি (0 <= option_verbosity) {
		যদি (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		অন্যথা
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	যদি (option_recurse_submodules.nr > 0) {
		সংরচনা string_list_item *item;
		সংরচনা strbuf sb = STRBUF_INIT;
		পূর্ণাঙ্ক val;

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

		যদি (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		যদি (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		অন্যথা যদি (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} অন্যথা যদি (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_বৃক্ষ,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, রিক্ত);

	যদি (real_git_dir) {
		মুক্ত((ৱর্ণ *)git_dir);
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

	refs_create_refdir_stubs(the_repository, git_dir, রিক্ত);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, রিক্ত);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	যদি (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	যদি (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	যদি (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	যদি (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	যদি (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	যদি (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	যদি (option_origin) {
		মুক্ত(remote_name);
		remote_name = xstrdup(option_origin);
	}

	যদি (!remote_name)
		remote_name = xstrdup("origin");

	যদি (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	যদি (option_bare) {
		যদি (option_mirror)
			src_ref_prefix = "refs/";
		strbuf_addstr(&branch_শীৰ্ষ, src_ref_prefix);

		repo_config_set(the_repository, "core.bare", "true");
	} অন্যথা যদি (!option_rev) {
		strbuf_addf(&branch_শীৰ্ষ, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&চাবি, "remote.%s.url", remote_name);
	repo_config_set(the_repository, চাবি.buf, repo);
	strbuf_reset(&চাবি);

	যদি (!option_tags) {
		strbuf_addf(&চাবি, "remote.%s.tagOpt", remote_name);
		repo_config_set(the_repository, চাবি.buf, "--no-tags");
		strbuf_reset(&চাবি);
	}

	remote = remote_get_early(remote_name);

	যদি (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_prefix,
				branch_শীৰ্ষ.buf);

	path = get_repo_path(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && path && !is_bundle;
	যদি (is_local) {
		যদি (option_depth)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		যদি (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		যদি (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		যদি (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		যদি (!access(mkpath("%s/shallow", path), F_OK)) {
			যদি (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			যদি (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	যদি (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? path : রিক্ত);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, path ? path : remote->url.v[0]);
	transport_set_verbosity(transport, option_verbosity, option_progress);
	transport->family = family;
	transport->cloning = 1;

	যদি (is_bundle) {
		সংরচনা bundle_header header = BUNDLE_HEADER_INIT;
		পূর্ণাঙ্ক fd = read_bundle_header(path, &header);
		পূর্ণাঙ্ক has_filter = header.filter.choice != LOFC_DISABLED;

		যদি (fd > 0)
			close(fd);
		bundle_header_release(&header);
		যদি (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_set_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	যদি (reject_shallow)
		transport_set_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	যদি (option_depth)
		transport_set_option(transport, TRANS_OPT_DEPTH,
				     option_depth);
	যদি (option_since)
		transport_set_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	যদি (option_not.nr)
		transport_set_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (ধ্রুৱ ৱর্ণ *)&option_not);
	যদি (option_single_branch) {
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		যদি (option_branch)
			opts.wants_head = 0;
	}

	যদি (option_upload_pack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	যদি (server_options.nr)
		transport->server_options = &server_options;

	যদি (filter_options.choice) {
		ধ্রুৱ ৱর্ণ *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	যদি (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	যদি (option_rev) {
		option_tags = 0;
		option_single_branch = 0;
		opts.wants_head = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	যদি (option_tags || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, TAG_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	যদি (option_branch)
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
	যদি (opts.wants_head || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_ঠেলো(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_version(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_set_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, রিক্ত, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	যদি (bundle_uri) {
		সংরচনা remote_state *state;
		পূর্ণাঙ্ক has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = রিক্ত;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		যদি (repo_init(the_repository, git_dir, work_বৃক্ষ))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		অন্যথা যদি (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		অন্যথা যদি (has_heuristic)
			repo_config_set_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		মুক্ত(the_repository->remote_state);
		the_repository->remote_state = state;
	} অন্যথা {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		যদি (transport->bundles &&
		    hashmap_get_size(&transport->bundles->bundles)) {
			সংরচনা remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = রিক্ত;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			যদি (repo_init(the_repository, git_dir, work_বৃক্ষ))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			অন্যথা যদি (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			মুক্ত(the_repository->remote_state);
			the_repository->remote_state = state;
		} অন্যথা {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	যদি (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	যদি (mapped_refs) {
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
		বাবে (ref = refs; ref; ref = ref->next)
			যদি (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				ভঙ্গ;
			}

		যদি (!is_local && !complete_refs_before_fetch) {
			যদি (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_head = find_ref_by_name(refs, "HEAD");
	remote_head_points_at = guess_remote_head(remote_head, mapped_refs,
						  REMOTE_GUESS_HEAD_QUIET);

	যদি (option_branch) {
		our_head_points_at = find_remote_branch(mapped_refs, option_branch);
		যদি (!our_head_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} অন্যথা যদি (option_rev) {
		our_head_points_at = mapped_refs;
		যদি (!our_head_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} অন্যথা যদি (remote_head_points_at) {
		our_head_points_at = remote_head_points_at;
	} অন্যথা যদি (remote_head) {
		our_head_points_at = রিক্ত;
	} অন্যথা {
		ৱর্ণ *to_free = রিক্ত;
		ধ্রুৱ ৱর্ণ *branch;

		যদি (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		যদি (transport_ls_refs_options.unborn_head_target &&
		    skip_prefix(transport_ls_refs_options.unborn_head_target,
				"refs/heads/", &branch)) {
			unborn_head  = xstrdup(transport_ls_refs_options.unborn_head_target);
		} অন্যথা {
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

		মুক্ত(to_free);
	}

	যদি (!option_rev)
		write_refspec_config(src_ref_prefix, our_head_points_at,
				     remote_head_points_at, &branch_শীৰ্ষ);

	যদি (filter_options.choice)
		partial_clone_register(remote_name, &filter_options);

	যদি (is_local)
		clone_local(path, git_dir);
	অন্যথা যদি (mapped_refs && complete_refs_before_fetch) {
		যদি (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_head_points_at,
			   branch_শীৰ্ষ.buf, reflog_বাৰ্তা.buf, transport,
			   !is_local);

	update_head(&opts, our_head_points_at, remote_head, unborn_head, reflog_বাৰ্তা.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_progress = transport->progress;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	যদি (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	যদি (option_sparse_checkout && git_sparse_checkout_init(dir))
		ঘূৰাই 1;

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

	মুক্ত(remote_name);
	strbuf_release(&reflog_বাৰ্তা);
	strbuf_release(&branch_শীৰ্ষ);
	strbuf_release(&buf);
	strbuf_release(&চাবি);
	free_refs(mapped_refs);
	free_refs(remote_head_points_at);
	মুক্ত(unborn_head);
	মুক্ত(dir);
	মুক্ত(path);
	মুক্ত(repo_to_free);
	junk_mode = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	ঘূৰাই err;
}
