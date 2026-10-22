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

#அந்தர்பாவ "builtin.h"

#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "copy.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "refspec.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "unpack-trees.h"
#அந்தர்பாவ "transport.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "dir-iterator.h"
#அந்தர்பாவ "iterator.h"
#அந்தர்பாவ "sigchain.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "connected.h"
#அந்தர்பாவ "packfile.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "pkt-line.h"
#அந்தர்பாவ "list-objects-filter-options.h"
#அந்தர்பாவ "hook.h"
#அந்தர்பாவ "bundle.h"
#அந்தர்பாவ "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

கட்டமைப்பு clone_opts {
	முழு wants_head;
	முழு detach;
};
#define CLONE_OPTS_INIT { \
	.wants_head = 1 /* default enabled */ \
}

நிலையான முழு option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
நிலையான முழு option_local = -1, option_no_hardlinks, option_shared;
நிலையான முழு option_tags = 1; /* default enabled */
நிலையான முழு option_shallow_submodules;
நிலையான முழு config_reject_shallow = -1;    /* unspecified */
நிலையான எழுத்து *remote_name = வெற்று;
நிலையான எழுத்து *option_branch = வெற்று;
நிலையான முழு option_verbosity;
நிலையான கட்டமைப்பு string_list option_required_reference = STRING_LIST_INIT_NODUP;
நிலையான கட்டமைப்பு string_list option_optional_reference = STRING_LIST_INIT_NODUP;
நிலையான முழு max_jobs = -1;
நிலையான கட்டமைப்பு string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
நிலையான முழு config_filter_submodules = -1;    /* unspecified */
நிலையான முழு option_remote_submodules;

நிலையான முழு recurse_submodules_cb(மாறிலி கட்டமைப்பு option *opt,
				 மாறிலி எழுத்து *arg, முழு unset)
{
	எனில் (unset)
		string_list_clear((கட்டமைப்பு string_list *)opt->மதிப்பு, 0);
	இல்லையெனில் எனில் (arg)
		string_list_append((கட்டமைப்பு string_list *)opt->மதிப்பு, arg);
	இல்லையெனில்
		string_list_append((கட்டமைப்பு string_list *)opt->மதிப்பு,
				   (மாறிலி எழுத்து *)opt->defval);

	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *get_repo_path_1(கட்டமைப்பு strbuf *path, முழு *is_bundle)
{
	நிலையான மாறிலி எழுத்து *suffix[] = { "/.git", "", ".git/.git", ".git" };
	நிலையான மாறிலி எழுத்து *bundle_suffix[] = { ".bundle", "" };
	size_t baselen = path->len;
	கட்டமைப்பு stat st;
	முழு i;

	க்கு (i = 0; i < ARRAY_SIZE(suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, suffix[i]);
		எனில் (stat(path->buf, &st))
			தொடர்;
		எனில் (S_ISDIR(st.st_mode) && is_git_directory(path->buf)) {
			*is_bundle = 0;
			திரும்பு path->buf;
		} இல்லையெனில் எனில் (S_ISREG(st.st_mode) && st.st_size > 8) {
			/* Is it a "gitfile"? */
			எழுத்து signature[8];
			மாறிலி எழுத்து *dst;
			முழு len, fd = open(path->buf, O_RDONLY);
			எனில் (fd < 0)
				தொடர்;
			len = read_in_full(fd, signature, 8);
			close(fd);
			எனில் (len != 8 || strncmp(signature, "gitdir: ", 8))
				தொடர்;
			dst = read_gitfile(path->buf);
			எனில் (dst) {
				*is_bundle = 0;
				திரும்பு dst;
			}
		}
	}

	க்கு (i = 0; i < ARRAY_SIZE(bundle_suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, bundle_suffix[i]);
		எனில் (!stat(path->buf, &st) && S_ISREG(st.st_mode)) {
			*is_bundle = 1;
			திரும்பு path->buf;
		}
	}

	திரும்பு வெற்று;
}

நிலையான எழுத்து *get_repo_path(மாறிலி எழுத்து *repo, முழு *is_bundle)
{
	கட்டமைப்பு strbuf path = STRBUF_INIT;
	மாறிலி எழுத்து *raw;
	எழுத்து *canon;

	strbuf_addstr(&path, repo);
	raw = get_repo_path_1(&path, is_bundle);
	canon = raw ? absolute_pathdup(raw) : வெற்று;
	strbuf_release(&path);
	திரும்பு canon;
}

கட்டமைப்பு collect_alternates_data {
	கட்டமைப்பு strvec *alternates;
	bool required;
};

நிலையான முழு collect_one_alternate(கட்டமைப்பு string_list_item *item, வெறுமை *cb_data)
{
	கட்டமைப்பு collect_alternates_data *data = cb_data;
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	எழுத்து *ref_git = compute_alternate_path(item->string, &err);

	எனில் (!ref_git) {
		எனில் (data->required)
			die("%s", err.buf);
		இல்லையெனில்
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				item->string, err.buf);
	} இல்லையெனில் {
		strvec_pushf(data->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	விடுவி(ref_git);
	திரும்பு 0;
}

நிலையான வெறுமை read_alternates(கட்டமைப்பு strvec *alternates, மாறிலி எழுத்து *src_repo)
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
	கட்டமைப்பு strbuf path = STRBUF_INIT;
	கட்டமைப்பு strbuf வரி = STRBUF_INIT;

	strbuf_addf(&path, "%s/objects/info/alternates", src_repo);

	in = fopen(path.buf, "r");
	எனில் (!in) {
		எனில் (errno == ENOENT)
			செல் out;
		die_errno("could not read alternates file '%s'", path.buf);
	}

	போது (strbuf_getline(&வரி, in) != EOF) {
		எழுத்து *abs_path;
		எனில் (!வரி.len || வரி.buf[0] == '#')
			தொடர்;
		எனில் (is_absolute_path(வரி.buf)) {
			strvec_தள்ளு(alternates, வரி.buf);
			தொடர்;
		}
		abs_path = mkpathdup("%s/objects/%s", src_repo, வரி.buf);
		எனில் (!normalize_path_copy(abs_path, abs_path))
			strvec_தள்ளு(alternates, abs_path);
		இல்லையெனில்
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, வரி.buf);
		விடுவி(abs_path);
	}

out:
	strbuf_release(&path);
	strbuf_release(&வரி);
	எனில் (in)
		fclose(in);
}

நிலையான வெறுமை collect_alternates(கட்டமைப்பு strvec *alternates,
			       மாறிலி எழுத்து *local_source_repo)
{
	எனில் (option_required_reference.nr || option_optional_reference.nr) {
		கட்டமைப்பு collect_alternates_data data = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &data);
		data.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &data);
	}

	எனில் (local_source_repo) {
		கட்டமைப்பு strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_source_repo);
		எனில் (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		இல்லையெனில்
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

நிலையான வெறுமை mkdir_if_missing(மாறிலி எழுத்து *pathname, mode_t mode)
{
	கட்டமைப்பு stat st;

	எனில் (!mkdir(pathname, mode))
		திரும்பு;

	எனில் (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	இல்லையெனில் எனில் (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	இல்லையெனில் எனில் (!S_ISDIR(st.st_mode))
		die(_("%s exists and is not a directory"), pathname);
}

நிலையான வெறுமை copy_or_link_directory(கட்டமைப்பு strbuf *src, கட்டமைப்பு strbuf *dest,
				   மாறிலி எழுத்து *src_repo)
{
	முழு src_len, dest_len;
	கட்டமைப்பு dir_iterator *iter;
	முழு iter_status;

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
	die_upon_dubious_ownership(வெற்று, வெற்று, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	எனில் (!iter) {
		எனில் (errno == ENOTDIR) {
			முழு saved_errno = errno;
			கட்டமைப்பு stat st;

			எனில் (!lstat(src->buf, &st) && S_ISLNK(st.st_mode))
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

	போது ((iter_status = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_path);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_path);

		எனில் (S_ISLNK(iter->st.st_mode))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_path);

		எனில் (S_ISDIR(iter->st.st_mode)) {
			mkdir_if_missing(dest->buf, 0777);
			தொடர்;
		}

		/* Alternates were already handled earlier. */
		எனில் (!fspathcmp(iter->relative_path, "info/alternates"))
			தொடர்;

		எனில் (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		எனில் (!option_no_hardlinks) {
			எனில் (!link(src->buf, dest->buf)) {
				கட்டமைப்பு stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				எனில் (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				எனில் (st.st_mode != iter->st.st_mode ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_size != iter->st.st_size ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				தொடர்;
			}
			எனில் (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		எனில் (copy_file_with_time(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	எனில் (iter_status != ITER_DONE) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

நிலையான வெறுமை clone_local(மாறிலி எழுத்து *src_repo, மாறிலி எழுத்து *dest_repo)
{
	எனில் (!option_shared) {
		கட்டமைப்பு strbuf src = STRBUF_INIT;
		கட்டமைப்பு strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	எனில் (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

நிலையான மாறிலி எழுத்து *junk_work_மரம்;
நிலையான முழு junk_work_மரம்_flags;
நிலையான மாறிலி எழுத்து *junk_git_dir;
நிலையான முழு junk_git_dir_flags;
நிலையான எண்ணிக்கை {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_mode = JUNK_LEAVE_NONE;

நிலையான மாறிலி எழுத்து junk_leave_repo_செய்தி[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

நிலையான வெறுமை remove_junk(வெறுமை)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	தேர்வு (junk_mode) {
	நிலை JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_செய்தி));
		/* fall-through */
	நிலை JUNK_LEAVE_ALL:
		திரும்பு;
	இயல்பு:
		/* proceed to removal */
		நிறுத்து;
	}

	எனில் (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	எனில் (junk_work_மரம்) {
		strbuf_addstr(&sb, junk_work_மரம்);
		remove_dir_recursively(&sb, junk_work_மரம்_flags);
	}
	strbuf_release(&sb);
}

நிலையான வெறுமை remove_junk_on_signal(முழு signo)
{
	remove_junk();
	sigchain_எடு(signo);
	எழுப்பு(signo);
}

நிலையான கட்டமைப்பு ref *find_remote_branch(மாறிலி கட்டமைப்பு ref *refs, மாறிலி எழுத்து *branch)
{
	கட்டமைப்பு ref *ref;
	கட்டமைப்பு strbuf head = STRBUF_INIT;
	strbuf_addstr(&head, "refs/heads/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	எனில் (ref)
		திரும்பு ref;

	strbuf_addstr(&head, "refs/tags/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	திரும்பு ref;
}

நிலையான கட்டமைப்பு ref *wanted_peer_refs(கட்டமைப்பு clone_opts *opts,
				    மாறிலி கட்டமைப்பு ref *refs,
				    கட்டமைப்பு refspec *refspec)
{
	கட்டமைப்பு ref *local_refs = வெற்று;
	கட்டமைப்பு ref **tail = &local_refs;
	கட்டமைப்பு ref *to_free = வெற்று;

	எனில் (opts->wants_head) {
		கட்டமைப்பு ref *head = copy_ref(find_ref_by_name(refs, "HEAD"));
		எனில் (head)
			tail_link_ref(head, &tail);
		எனில் (option_single_branch)
			refs = to_free =
				guess_remote_head(head, refs,
						  REMOTE_GUESS_HEAD_QUIET);
	} இல்லையெனில் எனில் (option_single_branch) {
		local_refs = வெற்று;
		tail = &local_refs;
		refs = to_free = copy_ref(find_remote_branch(refs, option_branch));
	}

	க்கு (size_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->பொருட்கள்[i], &tail, 0);

	free_one_ref(to_free);

	திரும்பு local_refs;
}

நிலையான வெறுமை write_remote_refs(மாறிலி கட்டமைப்பு ref *local_refs)
{
	மாறிலி கட்டமைப்பு ref *r;

	கட்டமைப்பு ref_transaction *t;
	கட்டமைப்பு strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	எனில் (!t)
		die("%s", err.buf);

	க்கு (r = local_refs; r; r = r->next) {
		எனில் (!r->peer_ref)
			தொடர்;
		எனில் (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   வெற்று, 0, வெற்று, &err))
			die("%s", err.buf);
	}

	எனில் (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

நிலையான வெறுமை write_followtags(மாறிலி கட்டமைப்பு ref *refs, மாறிலி எழுத்து *செய்தி)
{
	மாறிலி கட்டமைப்பு ref *ref;
	க்கு (ref = refs; ref; ref = ref->next) {
		எனில் (!starts_with(ref->name, "refs/tags/"))
			தொடர்;
		எனில் (ends_with(ref->name, "^{}"))
			தொடர்;
		எனில் (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			தொடர்;
		refs_update_ref(get_main_ref_store(the_repository), செய்தி,
				ref->name, &ref->old_oid, வெற்று, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

நிலையான மாறிலி கட்டமைப்பு object_id *iterate_ref_map(வெறுமை *cb_data)
{
	கட்டமைப்பு ref **rm = cb_data;
	கட்டமைப்பு ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	போது (ref && !ref->peer_ref)
		ref = ref->next;
	எனில் (!ref)
		திரும்பு வெற்று;

	*rm = ref->next;
	திரும்பு &ref->old_oid;
}

நிலையான வெறுமை update_remote_refs(மாறிலி கட்டமைப்பு ref *refs,
			       மாறிலி கட்டமைப்பு ref *mapped_refs,
			       மாறிலி கட்டமைப்பு ref *remote_head_points_at,
			       மாறிலி எழுத்து *branch_உச்சி,
			       மாறிலி எழுத்து *செய்தி,
			       கட்டமைப்பு transport *transport,
			       முழு check_connectivity)
{
	மாறிலி கட்டமைப்பு ref *rm = mapped_refs;

	எனில் (check_connectivity) {
		கட்டமைப்பு check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.progress = transport->progress;

		எனில் (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	எனில் (refs) {
		write_remote_refs(mapped_refs);
		எனில் (option_single_branch && option_tags)
			write_followtags(refs, செய்தி);
	}

	எனில் (remote_head_points_at && remote_head_points_at->peer_ref && !option_bare) {
		கட்டமைப்பு strbuf head_ref = STRBUF_INIT;
		strbuf_addstr(&head_ref, branch_உச்சி);
		strbuf_addstr(&head_ref, "HEAD");
		எனில் (refs_update_symref(get_main_ref_store(the_repository), head_ref.buf,
				       remote_head_points_at->peer_ref->name,
				       செய்தி) < 0)
			die(_("unable to update %s"), head_ref.buf);
		strbuf_release(&head_ref);
	}
}

நிலையான வெறுமை update_head(கட்டமைப்பு clone_opts *opts, மாறிலி கட்டமைப்பு ref *our, மாறிலி கட்டமைப்பு ref *remote,
			மாறிலி எழுத்து *unborn, மாறிலி எழுத்து *செய்தி)
{
	மாறிலி எழுத்து *head;
	எனில் (our && !opts->detach && skip_prefix(our->name, "refs/heads/", &head)) {
		/* Local default branch link */
		எனில் (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, வெற்று) < 0)
			die(_("unable to update HEAD"));
		எனில் (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					செய்தி, "HEAD", &our->old_oid, வெற்று, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, head, remote_name, our->name);
		}
	} இல்லையெனில் எனில் (our) {
		கட்டமைப்பு commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), செய்தி,
				"HEAD", &c->object.oid, வெற்று, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} இல்லையெனில் எனில் (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), செய்தி,
				"HEAD", &remote->old_oid, வெற்று, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} இல்லையெனில் எனில் (unborn && skip_prefix(unborn, "refs/heads/", &head)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		எனில் (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, வெற்று) < 0)
			die(_("unable to update HEAD"));
		எனில் (!option_bare)
			install_branch_config(0, head, remote_name, unborn);
	}
}

நிலையான முழு git_sparse_checkout_init(மாறிலி எழுத்து *repo)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;
	முழு result = 0;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	strvec_pushl(&cmd.args, "-C", repo, "sparse-checkout", "set", வெற்று);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	எனில் (run_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		result = 1;
	}

	திரும்பு result;
}

நிலையான முழு checkout(முழு submodule_progress,
		    கட்டமைப்பு list_objects_filter_options *filter_options,
		    முழு filter_submodules,
		    எண்ணிக்கை ref_storage_format ref_storage_format)
{
	கட்டமைப்பு object_id oid;
	எழுத்து *head;
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	கட்டமைப்பு unpack_trees_options opts;
	கட்டமைப்பு மரம் *மரம்;
	கட்டமைப்பு மரம்_desc t;
	முழு err = 0;
	கட்டமைப்பு run_hooks_opt hook_opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	எனில் (option_no_checkout)
		திரும்பு 0;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, வெற்று);
	எனில் (!head) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		திரும்பு 0;
	}
	எனில் (!strcmp(head, "HEAD")) {
		எனில் (advice_enabled(ADVICE_DETACHED_HEAD))
			detach_advice(oid_to_hex(&oid));
		FREE_AND_NULL(head);
	} இல்லையெனில் {
		எனில் (!starts_with(head, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	setup_work_மரம்(the_repository);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, அளவு opts);
	opts.update = 1;
	opts.merge = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_merge;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	init_checkout_metadata(&opts.meta, head, &oid, வெற்று);

	மரம் = repo_parse_மரம்_indirect(the_repository, &oid);
	எனில் (!மரம்)
		die(_("unable to parse commit %s"), oid_to_hex(&oid));
	எனில் (repo_parse_மரம்(the_repository, மரம்) < 0)
		exit(128);
	init_மரம்_desc(&t, &மரம்->object.oid, மரம்->buffer, மரம்->size);
	எனில் (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	விடுவி(head);

	எனில் (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.args, oid_to_hex(null_oid(the_hash_algo)),
		     oid_to_hex(&oid), "1", வெற்று);
	err |= run_hooks_opt(the_repository, "post-checkout", &hook_opt);

	எனில் (!err && (option_recurse_submodules.nr > 0)) {
		கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;
		strvec_pushl(&cmd.args, "submodule", "update", "--require-init",
			     "--recursive", வெற்று);

		எனில் (option_shallow_submodules == 1)
			strvec_தள்ளு(&cmd.args, "--depth=1");

		எனில் (max_jobs != -1)
			strvec_pushf(&cmd.args, "--jobs=%d", max_jobs);

		எனில் (submodule_progress)
			strvec_தள்ளு(&cmd.args, "--progress");

		எனில் (option_verbosity < 0)
			strvec_தள்ளு(&cmd.args, "--quiet");

		எனில் (option_remote_submodules) {
			strvec_தள்ளு(&cmd.args, "--remote");
			strvec_தள்ளு(&cmd.args, "--no-fetch");
		}

		எனில் (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(ref_storage_format));

		எனில் (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.args, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		எனில் (option_single_branch >= 0)
			strvec_தள்ளு(&cmd.args, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = run_command(&cmd);
	}

	திரும்பு err;
}

நிலையான முழு git_clone_config(மாறிலி எழுத்து *k, மாறிலி எழுத்து *v,
			    மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(k, "clone.defaultremotename")) {
		எனில் (!v)
			திரும்பு config_error_nonbool(k);
		விடுவி(remote_name);
		remote_name = xstrdup(v);
	}
	எனில் (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	எனில் (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	திரும்பு git_default_config(k, v, ctx, cb);
}

நிலையான முழு write_one_config(மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *மதிப்பு,
			    மாறிலி கட்டமைப்பு config_context *ctx,
			    வெறுமை *data)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	முழு apply_failed = git_clone_config(சாவி, மதிப்பு, ctx, data);
	எனில் (apply_failed)
		திரும்பு apply_failed;

	திரும்பு repo_config_set_multivar_gently(the_repository, சாவி,
					       மதிப்பு ? மதிப்பு : "true",
					       CONFIG_REGEX_NONE, 0);
}

நிலையான வெறுமை write_config(கட்டமைப்பு string_list *config)
{
	முழு i;

	க்கு (i = 0; i < config->nr; i++) {
		எனில் (git_config_parse_parameter(config->பொருட்கள்[i].string,
					       write_one_config, வெற்று) < 0)
			die(_("unable to write parameters to config file"));
	}
}

நிலையான வெறுமை write_refspec_config(மாறிலி எழுத்து *src_ref_prefix,
		மாறிலி கட்டமைப்பு ref *our_head_points_at,
		மாறிலி கட்டமைப்பு ref *remote_head_points_at,
		கட்டமைப்பு strbuf *branch_உச்சி)
{
	கட்டமைப்பு strbuf சாவி = STRBUF_INIT;
	கட்டமைப்பு strbuf மதிப்பு = STRBUF_INIT;

	எனில் (option_mirror || !option_bare) {
		எனில் (option_single_branch && !option_mirror) {
			எனில் (option_branch) {
				எனில் (starts_with(our_head_points_at->name, "refs/tags/"))
					strbuf_addf(&மதிப்பு, "+%s:%s", our_head_points_at->name,
						our_head_points_at->name);
				இல்லையெனில்
					strbuf_addf(&மதிப்பு, "+%s:%s%s", our_head_points_at->name,
						branch_உச்சி->buf, option_branch);
			} இல்லையெனில் எனில் (remote_head_points_at) {
				மாறிலி எழுத்து *head = remote_head_points_at->name;
				எனில் (!skip_prefix(head, "refs/heads/", &head))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&மதிப்பு, "+%s:%s%s", remote_head_points_at->name,
						branch_உச்சி->buf, head);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} இல்லையெனில் {
			strbuf_addf(&மதிப்பு, "+%s*:%s*", src_ref_prefix, branch_உச்சி->buf);
		}
		/* Configure the remote */
		எனில் (மதிப்பு.len) {
			strbuf_addf(&சாவி, "remote.%s.fetch", remote_name);
			repo_config_set_multivar(the_repository, சாவி.buf, மதிப்பு.buf, "^$", 0);
			strbuf_reset(&சாவி);

			எனில் (option_mirror) {
				strbuf_addf(&சாவி, "remote.%s.mirror", remote_name);
				repo_config_set(the_repository, சாவி.buf, "true");
				strbuf_reset(&சாவி);
			}
		}
	}

	strbuf_release(&சாவி);
	strbuf_release(&மதிப்பு);
}

நிலையான வெறுமை dissociate_from_references(வெறுமை)
{
	எழுத்து *alternates = repo_git_path(the_repository, "objects/info/alternates");

	எனில் (!access(alternates, F_OK)) {
		கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.args, "repack", "-a", "-d", வெற்று);
		எனில் (run_command(&cmd))
			die(_("cannot repack to clean up"));
		எனில் (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	விடுவி(alternates);
}

நிலையான முழு path_exists(மாறிலி எழுத்து *path)
{
	கட்டமைப்பு stat sb;
	திரும்பு !stat(path, &sb);
}

முழு cmd_clone(முழு argc,
	      மாறிலி எழுத்து **argv,
	      மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repository UNUSED)
{
	முழு is_bundle = 0, is_local;
	முழு reject_shallow = 0;
	மாறிலி எழுத்து *repo_name, *repo, *work_மரம், *git_dir;
	எழுத்து *repo_to_free = வெற்று;
	எழுத்து *path = வெற்று, *dir, *display_repo = வெற்று;
	முழு dest_exists, real_dest_exists = 0;
	மாறிலி கட்டமைப்பு ref *refs, *remote_head;
	கட்டமைப்பு ref *remote_head_points_at = வெற்று;
	மாறிலி கட்டமைப்பு ref *our_head_points_at;
	எழுத்து *unborn_head = வெற்று;
	கட்டமைப்பு ref *mapped_refs = வெற்று;
	மாறிலி கட்டமைப்பு ref *ref;
	கட்டமைப்பு strbuf சாவி = STRBUF_INIT;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு strbuf branch_உச்சி = STRBUF_INIT, reflog_செய்தி = STRBUF_INIT;
	கட்டமைப்பு transport *transport = வெற்று;
	மாறிலி எழுத்து *src_ref_prefix = "refs/heads/";
	கட்டமைப்பு remote *remote;
	முழு err = 0, complete_refs_before_fetch = 1;
	முழு submodule_progress;
	முழு filter_submodules = 0;
	முழு hash_algo;
	எண்ணிக்கை ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	மாறிலி முழு do_not_override_repo_unix_permissions = -1;
	முழு option_reject_shallow = -1; /* unspecified */
	முழு deepen = 0;
	எழுத்து *option_template = வெற்று, *option_depth = வெற்று, *option_since = வெற்று;
	எழுத்து *option_origin = வெற்று;
	கட்டமைப்பு string_list option_not = STRING_LIST_INIT_NODUP;
	மாறிலி எழுத்து *real_git_dir = வெற்று;
	மாறிலி எழுத்து *ref_storage_format_uri = வெற்று;
	மாறிலி எழுத்து *option_upload_pack = "git-upload-pack";
	முழு option_progress = -1;
	முழு option_sparse_checkout = 0;
	எண்ணிக்கை transport_family family = TRANSPORT_FAMILY_ALL;
	கட்டமைப்பு string_list option_config = STRING_LIST_INIT_DUP;
	முழு option_dissociate = 0;
	கட்டமைப்பு list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	முழு option_filter_submodules = -1; /* unspecified */
	கட்டமைப்பு string_list server_options = STRING_LIST_INIT_NODUP;
	மாறிலி எழுத்து *bundle_uri = வெற்று;
	எழுத்து *option_rev = வெற்று;

	கட்டமைப்பு clone_opts opts = CLONE_OPTS_INIT;

	கட்டமைப்பு transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	கட்டமைப்பு option builtin_clone_options[] = {
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
			.மதிப்பு = &option_recurse_submodules,
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
	கட்டமைப்பு strvec alternates = STRVEC_INIT;

	மாறிலி எழுத்து * மாறிலி builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		வெற்று
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_identity("clone");

	repo_config(the_repository, git_clone_config, வெற்று);

	argc = parse_options(argc, argv, prefix, builtin_clone_options,
			     builtin_clone_usage, 0);

	எனில் (argc > 2)
		usage_செய்தி_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	எனில் (argc == 0)
		usage_செய்தி_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	எனில் (option_depth || option_since || option_not.nr)
		deepen = 1;
	எனில் (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	எனில் (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, வெற்று);
		எனில் (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	எனில் (option_mirror) {
		option_bare = 1;
		option_tags = 0;
	}

	எனில் (option_bare) {
		எனில் (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	எனில் (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	path = get_repo_path(repo_name, &is_bundle);
	எனில் (path) {
		FREE_AND_NULL(path);
		repo = repo_to_free = absolute_pathdup(repo_name);
	} இல்லையெனில் எனில் (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} இல்லையெனில்
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	எனில் (option_depth && atoi(option_depth) < 1)
		die(_("depth %s is not a positive number"), option_depth);

	எனில் (argc == 2)
		dir = xstrdup(argv[1]);
	இல்லையெனில்
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = path_exists(dir);
	எனில் (dest_exists && !is_empty_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	எனில் (real_git_dir) {
		real_dest_exists = path_exists(real_git_dir);
		எனில் (real_dest_exists && !is_empty_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_செய்தி, "clone: from %s",
		    display_repo ? display_repo : repo);
	விடுவி(display_repo);

	எனில் (option_bare)
		work_மரம் = வெற்று;
	இல்லையெனில் {
		work_மரம் = getenv("GIT_WORK_TREE");
		எனில் (work_மரம் && path_exists(work_மரம்))
			die(_("working tree '%s' already exists."), work_மரம்);
	}

	எனில் (option_bare || work_மரம்)
		git_dir = xstrdup(dir);
	இல்லையெனில் {
		work_மரம் = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_தள்ளு_common(remove_junk_on_signal);

	எனில் (!option_bare) {
		எனில் (safe_create_leading_directories_const(the_repository, work_மரம்) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_மரம்);
		எனில் (dest_exists)
			junk_work_மரம்_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		இல்லையெனில் எனில் (mkdir(work_மரம், 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_மரம்);
		junk_work_மரம் = work_மரம்;
	}

	எனில் (real_git_dir) {
		எனில் (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} இல்லையெனில் {
		எனில் (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	எனில் (safe_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	எனில் (0 <= option_verbosity) {
		எனில் (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		இல்லையெனில்
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	எனில் (option_recurse_submodules.nr > 0) {
		கட்டமைப்பு string_list_item *item;
		கட்டமைப்பு strbuf sb = STRBUF_INIT;
		முழு val;

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

		எனில் (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		எனில் (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		இல்லையெனில் எனில் (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} இல்லையெனில் எனில் (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_மரம்,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, வெற்று);

	எனில் (real_git_dir) {
		விடுவி((எழுத்து *)git_dir);
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

	refs_create_refdir_stubs(the_repository, git_dir, வெற்று);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, வெற்று);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	எனில் (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	எனில் (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	எனில் (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	எனில் (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	எனில் (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	எனில் (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	எனில் (option_origin) {
		விடுவி(remote_name);
		remote_name = xstrdup(option_origin);
	}

	எனில் (!remote_name)
		remote_name = xstrdup("origin");

	எனில் (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	எனில் (option_bare) {
		எனில் (option_mirror)
			src_ref_prefix = "refs/";
		strbuf_addstr(&branch_உச்சி, src_ref_prefix);

		repo_config_set(the_repository, "core.bare", "true");
	} இல்லையெனில் எனில் (!option_rev) {
		strbuf_addf(&branch_உச்சி, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&சாவி, "remote.%s.url", remote_name);
	repo_config_set(the_repository, சாவி.buf, repo);
	strbuf_reset(&சாவி);

	எனில் (!option_tags) {
		strbuf_addf(&சாவி, "remote.%s.tagOpt", remote_name);
		repo_config_set(the_repository, சாவி.buf, "--no-tags");
		strbuf_reset(&சாவி);
	}

	remote = remote_get_early(remote_name);

	எனில் (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_prefix,
				branch_உச்சி.buf);

	path = get_repo_path(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && path && !is_bundle;
	எனில் (is_local) {
		எனில் (option_depth)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		எனில் (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		எனில் (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		எனில் (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		எனில் (!access(mkpath("%s/shallow", path), F_OK)) {
			எனில் (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			எனில் (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	எனில் (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? path : வெற்று);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, path ? path : remote->url.v[0]);
	transport_set_verbosity(transport, option_verbosity, option_progress);
	transport->family = family;
	transport->cloning = 1;

	எனில் (is_bundle) {
		கட்டமைப்பு bundle_header header = BUNDLE_HEADER_INIT;
		முழு fd = read_bundle_header(path, &header);
		முழு has_filter = header.filter.choice != LOFC_DISABLED;

		எனில் (fd > 0)
			close(fd);
		bundle_header_release(&header);
		எனில் (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_set_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	எனில் (reject_shallow)
		transport_set_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	எனில் (option_depth)
		transport_set_option(transport, TRANS_OPT_DEPTH,
				     option_depth);
	எனில் (option_since)
		transport_set_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	எனில் (option_not.nr)
		transport_set_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (மாறிலி எழுத்து *)&option_not);
	எனில் (option_single_branch) {
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		எனில் (option_branch)
			opts.wants_head = 0;
	}

	எனில் (option_upload_pack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	எனில் (server_options.nr)
		transport->server_options = &server_options;

	எனில் (filter_options.choice) {
		மாறிலி எழுத்து *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	எனில் (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	எனில் (option_rev) {
		option_tags = 0;
		option_single_branch = 0;
		opts.wants_head = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	எனில் (option_tags || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, TAG_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	எனில் (option_branch)
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
	எனில் (opts.wants_head || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_தள்ளு(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_version(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_set_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, வெற்று, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	எனில் (bundle_uri) {
		கட்டமைப்பு remote_state *state;
		முழு has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = வெற்று;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		எனில் (repo_init(the_repository, git_dir, work_மரம்))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		இல்லையெனில் எனில் (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		இல்லையெனில் எனில் (has_heuristic)
			repo_config_set_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		விடுவி(the_repository->remote_state);
		the_repository->remote_state = state;
	} இல்லையெனில் {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		எனில் (transport->bundles &&
		    hashmap_get_size(&transport->bundles->bundles)) {
			கட்டமைப்பு remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = வெற்று;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			எனில் (repo_init(the_repository, git_dir, work_மரம்))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			இல்லையெனில் எனில் (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			விடுவி(the_repository->remote_state);
			the_repository->remote_state = state;
		} இல்லையெனில் {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	எனில் (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	எனில் (mapped_refs) {
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
		க்கு (ref = refs; ref; ref = ref->next)
			எனில் (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				நிறுத்து;
			}

		எனில் (!is_local && !complete_refs_before_fetch) {
			எனில் (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_head = find_ref_by_name(refs, "HEAD");
	remote_head_points_at = guess_remote_head(remote_head, mapped_refs,
						  REMOTE_GUESS_HEAD_QUIET);

	எனில் (option_branch) {
		our_head_points_at = find_remote_branch(mapped_refs, option_branch);
		எனில் (!our_head_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} இல்லையெனில் எனில் (option_rev) {
		our_head_points_at = mapped_refs;
		எனில் (!our_head_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} இல்லையெனில் எனில் (remote_head_points_at) {
		our_head_points_at = remote_head_points_at;
	} இல்லையெனில் எனில் (remote_head) {
		our_head_points_at = வெற்று;
	} இல்லையெனில் {
		எழுத்து *to_free = வெற்று;
		மாறிலி எழுத்து *branch;

		எனில் (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		எனில் (transport_ls_refs_options.unborn_head_target &&
		    skip_prefix(transport_ls_refs_options.unborn_head_target,
				"refs/heads/", &branch)) {
			unborn_head  = xstrdup(transport_ls_refs_options.unborn_head_target);
		} இல்லையெனில் {
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

		விடுவி(to_free);
	}

	எனில் (!option_rev)
		write_refspec_config(src_ref_prefix, our_head_points_at,
				     remote_head_points_at, &branch_உச்சி);

	எனில் (filter_options.choice)
		partial_clone_register(remote_name, &filter_options);

	எனில் (is_local)
		clone_local(path, git_dir);
	இல்லையெனில் எனில் (mapped_refs && complete_refs_before_fetch) {
		எனில் (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_head_points_at,
			   branch_உச்சி.buf, reflog_செய்தி.buf, transport,
			   !is_local);

	update_head(&opts, our_head_points_at, remote_head, unborn_head, reflog_செய்தி.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_progress = transport->progress;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	எனில் (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	எனில் (option_sparse_checkout && git_sparse_checkout_init(dir))
		திரும்பு 1;

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

	விடுவி(remote_name);
	strbuf_release(&reflog_செய்தி);
	strbuf_release(&branch_உச்சி);
	strbuf_release(&buf);
	strbuf_release(&சாவி);
	free_refs(mapped_refs);
	free_refs(remote_head_points_at);
	விடுவி(unborn_head);
	விடுவி(dir);
	விடுவி(path);
	விடுவி(repo_to_free);
	junk_mode = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	திரும்பு err;
}
