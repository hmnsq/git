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

#અન્તર્ભાવ "builtin.h"

#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "copy.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "refspec.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "tree-walk.h"
#અન્તર્ભાવ "unpack-trees.h"
#અન્તર્ભાવ "transport.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "dir-iterator.h"
#અન્તર્ભાવ "iterator.h"
#અન્તર્ભાવ "sigchain.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "connected.h"
#અન્તર્ભાવ "packfile.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "pkt-line.h"
#અન્તર્ભાવ "list-objects-filter-options.h"
#અન્તર્ભાવ "hook.h"
#અન્તર્ભાવ "bundle.h"
#અન્તર્ભાવ "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

સંરચના clone_opts {
	પૂર્ણાંક wants_head;
	પૂર્ણાંક detach;
};
#define CLONE_OPTS_INIT { \
	.wants_head = 1 /* default enabled */ \
}

સ્થિર પૂર્ણાંક option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
સ્થિર પૂર્ણાંક option_local = -1, option_no_hardlinks, option_shared;
સ્થિર પૂર્ણાંક option_tags = 1; /* default enabled */
સ્થિર પૂર્ણાંક option_shallow_submodules;
સ્થિર પૂર્ણાંક config_reject_shallow = -1;    /* unspecified */
સ્થિર અક્ષર *remote_name = ખાલી;
સ્થિર અક્ષર *option_branch = ખાલી;
સ્થિર પૂર્ણાંક option_verbosity;
સ્થિર સંરચના string_list option_required_reference = STRING_LIST_INIT_NODUP;
સ્થિર સંરચના string_list option_optional_reference = STRING_LIST_INIT_NODUP;
સ્થિર પૂર્ણાંક max_jobs = -1;
સ્થિર સંરચના string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
સ્થિર પૂર્ણાંક config_filter_submodules = -1;    /* unspecified */
સ્થિર પૂર્ણાંક option_remote_submodules;

સ્થિર પૂર્ણાંક recurse_submodules_cb(અચલ સંરચના option *opt,
				 અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	જો (unset)
		string_list_clear((સંરચના string_list *)opt->કિંમત, 0);
	નહીંતર જો (arg)
		string_list_append((સંરચના string_list *)opt->કિંમત, arg);
	નહીંતર
		string_list_append((સંરચના string_list *)opt->કિંમત,
				   (અચલ અક્ષર *)opt->defval);

	પાછા 0;
}

સ્થિર અચલ અક્ષર *get_repo_path_1(સંરચના strbuf *path, પૂર્ણાંક *is_bundle)
{
	સ્થિર અચલ અક્ષર *suffix[] = { "/.git", "", ".git/.git", ".git" };
	સ્થિર અચલ અક્ષર *bundle_suffix[] = { ".bundle", "" };
	size_t baselen = path->len;
	સંરચના stat st;
	પૂર્ણાંક i;

	માટે (i = 0; i < ARRAY_SIZE(suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, suffix[i]);
		જો (stat(path->buf, &st))
			ચાલુ;
		જો (S_ISDIR(st.st_mode) && is_git_directory(path->buf)) {
			*is_bundle = 0;
			પાછા path->buf;
		} નહીંતર જો (S_ISREG(st.st_mode) && st.st_size > 8) {
			/* Is it a "gitfile"? */
			અક્ષર signature[8];
			અચલ અક્ષર *dst;
			પૂર્ણાંક len, fd = open(path->buf, O_RDONLY);
			જો (fd < 0)
				ચાલુ;
			len = read_in_full(fd, signature, 8);
			close(fd);
			જો (len != 8 || strncmp(signature, "gitdir: ", 8))
				ચાલુ;
			dst = read_gitfile(path->buf);
			જો (dst) {
				*is_bundle = 0;
				પાછા dst;
			}
		}
	}

	માટે (i = 0; i < ARRAY_SIZE(bundle_suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, bundle_suffix[i]);
		જો (!stat(path->buf, &st) && S_ISREG(st.st_mode)) {
			*is_bundle = 1;
			પાછા path->buf;
		}
	}

	પાછા ખાલી;
}

સ્થિર અક્ષર *get_repo_path(અચલ અક્ષર *repo, પૂર્ણાંક *is_bundle)
{
	સંરચના strbuf path = STRBUF_INIT;
	અચલ અક્ષર *raw;
	અક્ષર *canon;

	strbuf_addstr(&path, repo);
	raw = get_repo_path_1(&path, is_bundle);
	canon = raw ? absolute_pathdup(raw) : ખાલી;
	strbuf_release(&path);
	પાછા canon;
}

સંરચના collect_alternates_data {
	સંરચના strvec *alternates;
	bool required;
};

સ્થિર પૂર્ણાંક collect_one_alternate(સંરચના string_list_item *item, શૂન્ય *cb_data)
{
	સંરચના collect_alternates_data *data = cb_data;
	સંરચના strbuf err = STRBUF_INIT;
	અક્ષર *ref_git = compute_alternate_path(item->string, &err);

	જો (!ref_git) {
		જો (data->required)
			die("%s", err.buf);
		નહીંતર
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				item->string, err.buf);
	} નહીંતર {
		strvec_pushf(data->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	મુક્ત(ref_git);
	પાછા 0;
}

સ્થિર શૂન્ય read_alternates(સંરચના strvec *alternates, અચલ અક્ષર *src_repo)
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
	સંરચના strbuf path = STRBUF_INIT;
	સંરચના strbuf લીટી = STRBUF_INIT;

	strbuf_addf(&path, "%s/objects/info/alternates", src_repo);

	in = fopen(path.buf, "r");
	જો (!in) {
		જો (errno == ENOENT)
			જાઓ out;
		die_errno("could not read alternates file '%s'", path.buf);
	}

	જ્યારે (strbuf_getline(&લીટી, in) != EOF) {
		અક્ષર *abs_path;
		જો (!લીટી.len || લીટી.buf[0] == '#')
			ચાલુ;
		જો (is_absolute_path(લીટી.buf)) {
			strvec_ધકેલો(alternates, લીટી.buf);
			ચાલુ;
		}
		abs_path = mkpathdup("%s/objects/%s", src_repo, લીટી.buf);
		જો (!normalize_path_copy(abs_path, abs_path))
			strvec_ધકેલો(alternates, abs_path);
		નહીંતર
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, લીટી.buf);
		મુક્ત(abs_path);
	}

out:
	strbuf_release(&path);
	strbuf_release(&લીટી);
	જો (in)
		fclose(in);
}

સ્થિર શૂન્ય collect_alternates(સંરચના strvec *alternates,
			       અચલ અક્ષર *local_source_repo)
{
	જો (option_required_reference.nr || option_optional_reference.nr) {
		સંરચના collect_alternates_data data = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &data);
		data.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &data);
	}

	જો (local_source_repo) {
		સંરચના strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_source_repo);
		જો (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		નહીંતર
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

સ્થિર શૂન્ય mkdir_if_missing(અચલ અક્ષર *pathname, mode_t mode)
{
	સંરચના stat st;

	જો (!mkdir(pathname, mode))
		પાછા;

	જો (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	નહીંતર જો (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	નહીંતર જો (!S_ISDIR(st.st_mode))
		die(_("%s exists and is not a directory"), pathname);
}

સ્થિર શૂન્ય copy_or_link_directory(સંરચના strbuf *src, સંરચના strbuf *dest,
				   અચલ અક્ષર *src_repo)
{
	પૂર્ણાંક src_len, dest_len;
	સંરચના dir_iterator *iter;
	પૂર્ણાંક iter_status;

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
	die_upon_dubious_ownership(ખાલી, ખાલી, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	જો (!iter) {
		જો (errno == ENOTDIR) {
			પૂર્ણાંક saved_errno = errno;
			સંરચના stat st;

			જો (!lstat(src->buf, &st) && S_ISLNK(st.st_mode))
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

	જ્યારે ((iter_status = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_path);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_path);

		જો (S_ISLNK(iter->st.st_mode))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_path);

		જો (S_ISDIR(iter->st.st_mode)) {
			mkdir_if_missing(dest->buf, 0777);
			ચાલુ;
		}

		/* Alternates were already handled earlier. */
		જો (!fspathcmp(iter->relative_path, "info/alternates"))
			ચાલુ;

		જો (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		જો (!option_no_hardlinks) {
			જો (!link(src->buf, dest->buf)) {
				સંરચના stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				જો (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				જો (st.st_mode != iter->st.st_mode ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_size != iter->st.st_size ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				ચાલુ;
			}
			જો (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		જો (copy_file_with_time(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	જો (iter_status != ITER_DONE) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

સ્થિર શૂન્ય clone_local(અચલ અક્ષર *src_repo, અચલ અક્ષર *dest_repo)
{
	જો (!option_shared) {
		સંરચના strbuf src = STRBUF_INIT;
		સંરચના strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	જો (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

સ્થિર અચલ અક્ષર *junk_work_વૃક્ષ;
સ્થિર પૂર્ણાંક junk_work_વૃક્ષ_flags;
સ્થિર અચલ અક્ષર *junk_git_dir;
સ્થિર પૂર્ણાંક junk_git_dir_flags;
સ્થિર ગણના {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_mode = JUNK_LEAVE_NONE;

સ્થિર અચલ અક્ષર junk_leave_repo_સંદેશ[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

સ્થિર શૂન્ય remove_junk(શૂન્ય)
{
	સંરચના strbuf sb = STRBUF_INIT;

	પસંદગી (junk_mode) {
	સ્થિતિ JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_સંદેશ));
		/* fall-through */
	સ્થિતિ JUNK_LEAVE_ALL:
		પાછા;
	મૂળભૂત:
		/* proceed to removal */
		તોડો;
	}

	જો (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	જો (junk_work_વૃક્ષ) {
		strbuf_addstr(&sb, junk_work_વૃક્ષ);
		remove_dir_recursively(&sb, junk_work_વૃક્ષ_flags);
	}
	strbuf_release(&sb);
}

સ્થિર શૂન્ય remove_junk_on_signal(પૂર્ણાંક signo)
{
	remove_junk();
	sigchain_કાઢો(signo);
	ઉઠાવો(signo);
}

સ્થિર સંરચના ref *find_remote_branch(અચલ સંરચના ref *refs, અચલ અક્ષર *branch)
{
	સંરચના ref *ref;
	સંરચના strbuf head = STRBUF_INIT;
	strbuf_addstr(&head, "refs/heads/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	જો (ref)
		પાછા ref;

	strbuf_addstr(&head, "refs/tags/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	પાછા ref;
}

સ્થિર સંરચના ref *wanted_peer_refs(સંરચના clone_opts *opts,
				    અચલ સંરચના ref *refs,
				    સંરચના refspec *refspec)
{
	સંરચના ref *local_refs = ખાલી;
	સંરચના ref **tail = &local_refs;
	સંરચના ref *to_free = ખાલી;

	જો (opts->wants_head) {
		સંરચના ref *head = copy_ref(find_ref_by_name(refs, "HEAD"));
		જો (head)
			tail_link_ref(head, &tail);
		જો (option_single_branch)
			refs = to_free =
				guess_remote_head(head, refs,
						  REMOTE_GUESS_HEAD_QUIET);
	} નહીંતર જો (option_single_branch) {
		local_refs = ખાલી;
		tail = &local_refs;
		refs = to_free = copy_ref(find_remote_branch(refs, option_branch));
	}

	માટે (size_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->વસ્તુઓ[i], &tail, 0);

	free_one_ref(to_free);

	પાછા local_refs;
}

સ્થિર શૂન્ય write_remote_refs(અચલ સંરચના ref *local_refs)
{
	અચલ સંરચના ref *r;

	સંરચના ref_transaction *t;
	સંરચના strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	જો (!t)
		die("%s", err.buf);

	માટે (r = local_refs; r; r = r->next) {
		જો (!r->peer_ref)
			ચાલુ;
		જો (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   ખાલી, 0, ખાલી, &err))
			die("%s", err.buf);
	}

	જો (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

સ્થિર શૂન્ય write_followtags(અચલ સંરચના ref *refs, અચલ અક્ષર *સંદેશ)
{
	અચલ સંરચના ref *ref;
	માટે (ref = refs; ref; ref = ref->next) {
		જો (!starts_with(ref->name, "refs/tags/"))
			ચાલુ;
		જો (ends_with(ref->name, "^{}"))
			ચાલુ;
		જો (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			ચાલુ;
		refs_update_ref(get_main_ref_store(the_repository), સંદેશ,
				ref->name, &ref->old_oid, ખાલી, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

સ્થિર અચલ સંરચના object_id *iterate_ref_map(શૂન્ય *cb_data)
{
	સંરચના ref **rm = cb_data;
	સંરચના ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	જ્યારે (ref && !ref->peer_ref)
		ref = ref->next;
	જો (!ref)
		પાછા ખાલી;

	*rm = ref->next;
	પાછા &ref->old_oid;
}

સ્થિર શૂન્ય update_remote_refs(અચલ સંરચના ref *refs,
			       અચલ સંરચના ref *mapped_refs,
			       અચલ સંરચના ref *remote_head_points_at,
			       અચલ અક્ષર *branch_ટોચ,
			       અચલ અક્ષર *સંદેશ,
			       સંરચના transport *transport,
			       પૂર્ણાંક check_connectivity)
{
	અચલ સંરચના ref *rm = mapped_refs;

	જો (check_connectivity) {
		સંરચના check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.progress = transport->progress;

		જો (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	જો (refs) {
		write_remote_refs(mapped_refs);
		જો (option_single_branch && option_tags)
			write_followtags(refs, સંદેશ);
	}

	જો (remote_head_points_at && remote_head_points_at->peer_ref && !option_bare) {
		સંરચના strbuf head_ref = STRBUF_INIT;
		strbuf_addstr(&head_ref, branch_ટોચ);
		strbuf_addstr(&head_ref, "HEAD");
		જો (refs_update_symref(get_main_ref_store(the_repository), head_ref.buf,
				       remote_head_points_at->peer_ref->name,
				       સંદેશ) < 0)
			die(_("unable to update %s"), head_ref.buf);
		strbuf_release(&head_ref);
	}
}

સ્થિર શૂન્ય update_head(સંરચના clone_opts *opts, અચલ સંરચના ref *our, અચલ સંરચના ref *remote,
			અચલ અક્ષર *unborn, અચલ અક્ષર *સંદેશ)
{
	અચલ અક્ષર *head;
	જો (our && !opts->detach && skip_prefix(our->name, "refs/heads/", &head)) {
		/* Local default branch link */
		જો (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, ખાલી) < 0)
			die(_("unable to update HEAD"));
		જો (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					સંદેશ, "HEAD", &our->old_oid, ખાલી, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, head, remote_name, our->name);
		}
	} નહીંતર જો (our) {
		સંરચના commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), સંદેશ,
				"HEAD", &c->object.oid, ખાલી, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} નહીંતર જો (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), સંદેશ,
				"HEAD", &remote->old_oid, ખાલી, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} નહીંતર જો (unborn && skip_prefix(unborn, "refs/heads/", &head)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		જો (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, ખાલી) < 0)
			die(_("unable to update HEAD"));
		જો (!option_bare)
			install_branch_config(0, head, remote_name, unborn);
	}
}

સ્થિર પૂર્ણાંક git_sparse_checkout_init(અચલ અક્ષર *repo)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;
	પૂર્ણાંક result = 0;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	strvec_pushl(&cmd.args, "-C", repo, "sparse-checkout", "set", ખાલી);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	જો (run_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		result = 1;
	}

	પાછા result;
}

સ્થિર પૂર્ણાંક checkout(પૂર્ણાંક submodule_progress,
		    સંરચના list_objects_filter_options *filter_options,
		    પૂર્ણાંક filter_submodules,
		    ગણના ref_storage_format ref_storage_format)
{
	સંરચના object_id oid;
	અક્ષર *head;
	સંરચના lock_file lock_file = LOCK_INIT;
	સંરચના unpack_trees_options opts;
	સંરચના વૃક્ષ *વૃક્ષ;
	સંરચના વૃક્ષ_desc t;
	પૂર્ણાંક err = 0;
	સંરચના run_hooks_opt hook_opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	જો (option_no_checkout)
		પાછા 0;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, ખાલી);
	જો (!head) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		પાછા 0;
	}
	જો (!strcmp(head, "HEAD")) {
		જો (advice_enabled(ADVICE_DETACHED_HEAD))
			detach_advice(oid_to_hex(&oid));
		FREE_AND_NULL(head);
	} નહીંતર {
		જો (!starts_with(head, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	setup_work_વૃક્ષ(the_repository);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, કદ opts);
	opts.update = 1;
	opts.merge = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_merge;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	init_checkout_metadata(&opts.meta, head, &oid, ખાલી);

	વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository, &oid);
	જો (!વૃક્ષ)
		die(_("unable to parse commit %s"), oid_to_hex(&oid));
	જો (repo_parse_વૃક્ષ(the_repository, વૃક્ષ) < 0)
		exit(128);
	init_વૃક્ષ_desc(&t, &વૃક્ષ->object.oid, વૃક્ષ->buffer, વૃક્ષ->size);
	જો (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	મુક્ત(head);

	જો (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.args, oid_to_hex(null_oid(the_hash_algo)),
		     oid_to_hex(&oid), "1", ખાલી);
	err |= run_hooks_opt(the_repository, "post-checkout", &hook_opt);

	જો (!err && (option_recurse_submodules.nr > 0)) {
		સંરચના child_process cmd = CHILD_PROCESS_INIT;
		strvec_pushl(&cmd.args, "submodule", "update", "--require-init",
			     "--recursive", ખાલી);

		જો (option_shallow_submodules == 1)
			strvec_ધકેલો(&cmd.args, "--depth=1");

		જો (max_jobs != -1)
			strvec_pushf(&cmd.args, "--jobs=%d", max_jobs);

		જો (submodule_progress)
			strvec_ધકેલો(&cmd.args, "--progress");

		જો (option_verbosity < 0)
			strvec_ધકેલો(&cmd.args, "--quiet");

		જો (option_remote_submodules) {
			strvec_ધકેલો(&cmd.args, "--remote");
			strvec_ધકેલો(&cmd.args, "--no-fetch");
		}

		જો (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(ref_storage_format));

		જો (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.args, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		જો (option_single_branch >= 0)
			strvec_ધકેલો(&cmd.args, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = run_command(&cmd);
	}

	પાછા err;
}

સ્થિર પૂર્ણાંક git_clone_config(અચલ અક્ષર *k, અચલ અક્ષર *v,
			    અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(k, "clone.defaultremotename")) {
		જો (!v)
			પાછા config_error_nonbool(k);
		મુક્ત(remote_name);
		remote_name = xstrdup(v);
	}
	જો (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	જો (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	પાછા git_default_config(k, v, ctx, cb);
}

સ્થિર પૂર્ણાંક write_one_config(અચલ અક્ષર *ચાવી, અચલ અક્ષર *કિંમત,
			    અચલ સંરચના config_context *ctx,
			    શૂન્ય *data)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	પૂર્ણાંક apply_failed = git_clone_config(ચાવી, કિંમત, ctx, data);
	જો (apply_failed)
		પાછા apply_failed;

	પાછા repo_config_set_multivar_gently(the_repository, ચાવી,
					       કિંમત ? કિંમત : "true",
					       CONFIG_REGEX_NONE, 0);
}

સ્થિર શૂન્ય write_config(સંરચના string_list *config)
{
	પૂર્ણાંક i;

	માટે (i = 0; i < config->nr; i++) {
		જો (git_config_parse_parameter(config->વસ્તુઓ[i].string,
					       write_one_config, ખાલી) < 0)
			die(_("unable to write parameters to config file"));
	}
}

સ્થિર શૂન્ય write_refspec_config(અચલ અક્ષર *src_ref_prefix,
		અચલ સંરચના ref *our_head_points_at,
		અચલ સંરચના ref *remote_head_points_at,
		સંરચના strbuf *branch_ટોચ)
{
	સંરચના strbuf ચાવી = STRBUF_INIT;
	સંરચના strbuf કિંમત = STRBUF_INIT;

	જો (option_mirror || !option_bare) {
		જો (option_single_branch && !option_mirror) {
			જો (option_branch) {
				જો (starts_with(our_head_points_at->name, "refs/tags/"))
					strbuf_addf(&કિંમત, "+%s:%s", our_head_points_at->name,
						our_head_points_at->name);
				નહીંતર
					strbuf_addf(&કિંમત, "+%s:%s%s", our_head_points_at->name,
						branch_ટોચ->buf, option_branch);
			} નહીંતર જો (remote_head_points_at) {
				અચલ અક્ષર *head = remote_head_points_at->name;
				જો (!skip_prefix(head, "refs/heads/", &head))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&કિંમત, "+%s:%s%s", remote_head_points_at->name,
						branch_ટોચ->buf, head);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} નહીંતર {
			strbuf_addf(&કિંમત, "+%s*:%s*", src_ref_prefix, branch_ટોચ->buf);
		}
		/* Configure the remote */
		જો (કિંમત.len) {
			strbuf_addf(&ચાવી, "remote.%s.fetch", remote_name);
			repo_config_set_multivar(the_repository, ચાવી.buf, કિંમત.buf, "^$", 0);
			strbuf_reset(&ચાવી);

			જો (option_mirror) {
				strbuf_addf(&ચાવી, "remote.%s.mirror", remote_name);
				repo_config_set(the_repository, ચાવી.buf, "true");
				strbuf_reset(&ચાવી);
			}
		}
	}

	strbuf_release(&ચાવી);
	strbuf_release(&કિંમત);
}

સ્થિર શૂન્ય dissociate_from_references(શૂન્ય)
{
	અક્ષર *alternates = repo_git_path(the_repository, "objects/info/alternates");

	જો (!access(alternates, F_OK)) {
		સંરચના child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.args, "repack", "-a", "-d", ખાલી);
		જો (run_command(&cmd))
			die(_("cannot repack to clean up"));
		જો (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	મુક્ત(alternates);
}

સ્થિર પૂર્ણાંક path_exists(અચલ અક્ષર *path)
{
	સંરચના stat sb;
	પાછા !stat(path, &sb);
}

પૂર્ણાંક cmd_clone(પૂર્ણાંક argc,
	      અચલ અક્ષર **argv,
	      અચલ અક્ષર *prefix,
	      સંરચના repository *repository UNUSED)
{
	પૂર્ણાંક is_bundle = 0, is_local;
	પૂર્ણાંક reject_shallow = 0;
	અચલ અક્ષર *repo_name, *repo, *work_વૃક્ષ, *git_dir;
	અક્ષર *repo_to_free = ખાલી;
	અક્ષર *path = ખાલી, *dir, *display_repo = ખાલી;
	પૂર્ણાંક dest_exists, real_dest_exists = 0;
	અચલ સંરચના ref *refs, *remote_head;
	સંરચના ref *remote_head_points_at = ખાલી;
	અચલ સંરચના ref *our_head_points_at;
	અક્ષર *unborn_head = ખાલી;
	સંરચના ref *mapped_refs = ખાલી;
	અચલ સંરચના ref *ref;
	સંરચના strbuf ચાવી = STRBUF_INIT;
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના strbuf branch_ટોચ = STRBUF_INIT, reflog_સંદેશ = STRBUF_INIT;
	સંરચના transport *transport = ખાલી;
	અચલ અક્ષર *src_ref_prefix = "refs/heads/";
	સંરચના remote *remote;
	પૂર્ણાંક err = 0, complete_refs_before_fetch = 1;
	પૂર્ણાંક submodule_progress;
	પૂર્ણાંક filter_submodules = 0;
	પૂર્ણાંક hash_algo;
	ગણના ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	અચલ પૂર્ણાંક do_not_override_repo_unix_permissions = -1;
	પૂર્ણાંક option_reject_shallow = -1; /* unspecified */
	પૂર્ણાંક deepen = 0;
	અક્ષર *option_template = ખાલી, *option_depth = ખાલી, *option_since = ખાલી;
	અક્ષર *option_origin = ખાલી;
	સંરચના string_list option_not = STRING_LIST_INIT_NODUP;
	અચલ અક્ષર *real_git_dir = ખાલી;
	અચલ અક્ષર *ref_storage_format_uri = ખાલી;
	અચલ અક્ષર *option_upload_pack = "git-upload-pack";
	પૂર્ણાંક option_progress = -1;
	પૂર્ણાંક option_sparse_checkout = 0;
	ગણના transport_family family = TRANSPORT_FAMILY_ALL;
	સંરચના string_list option_config = STRING_LIST_INIT_DUP;
	પૂર્ણાંક option_dissociate = 0;
	સંરચના list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	પૂર્ણાંક option_filter_submodules = -1; /* unspecified */
	સંરચના string_list server_options = STRING_LIST_INIT_NODUP;
	અચલ અક્ષર *bundle_uri = ખાલી;
	અક્ષર *option_rev = ખાલી;

	સંરચના clone_opts opts = CLONE_OPTS_INIT;

	સંરચના transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	સંરચના option builtin_clone_options[] = {
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
			.કિંમત = &option_recurse_submodules,
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
	સંરચના strvec alternates = STRVEC_INIT;

	અચલ અક્ષર * અચલ builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		ખાલી
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_identity("clone");

	repo_config(the_repository, git_clone_config, ખાલી);

	argc = parse_options(argc, argv, prefix, builtin_clone_options,
			     builtin_clone_usage, 0);

	જો (argc > 2)
		usage_સંદેશ_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	જો (argc == 0)
		usage_સંદેશ_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	જો (option_depth || option_since || option_not.nr)
		deepen = 1;
	જો (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	જો (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, ખાલી);
		જો (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	જો (option_mirror) {
		option_bare = 1;
		option_tags = 0;
	}

	જો (option_bare) {
		જો (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	જો (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	path = get_repo_path(repo_name, &is_bundle);
	જો (path) {
		FREE_AND_NULL(path);
		repo = repo_to_free = absolute_pathdup(repo_name);
	} નહીંતર જો (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} નહીંતર
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	જો (option_depth && atoi(option_depth) < 1)
		die(_("depth %s is not a positive number"), option_depth);

	જો (argc == 2)
		dir = xstrdup(argv[1]);
	નહીંતર
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = path_exists(dir);
	જો (dest_exists && !is_empty_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	જો (real_git_dir) {
		real_dest_exists = path_exists(real_git_dir);
		જો (real_dest_exists && !is_empty_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_સંદેશ, "clone: from %s",
		    display_repo ? display_repo : repo);
	મુક્ત(display_repo);

	જો (option_bare)
		work_વૃક્ષ = ખાલી;
	નહીંતર {
		work_વૃક્ષ = getenv("GIT_WORK_TREE");
		જો (work_વૃક્ષ && path_exists(work_વૃક્ષ))
			die(_("working tree '%s' already exists."), work_વૃક્ષ);
	}

	જો (option_bare || work_વૃક્ષ)
		git_dir = xstrdup(dir);
	નહીંતર {
		work_વૃક્ષ = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_ધકેલો_common(remove_junk_on_signal);

	જો (!option_bare) {
		જો (safe_create_leading_directories_const(the_repository, work_વૃક્ષ) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_વૃક્ષ);
		જો (dest_exists)
			junk_work_વૃક્ષ_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		નહીંતર જો (mkdir(work_વૃક્ષ, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_વૃક્ષ);
		junk_work_વૃક્ષ = work_વૃક્ષ;
	}

	જો (real_git_dir) {
		જો (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} નહીંતર {
		જો (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	જો (safe_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	જો (0 <= option_verbosity) {
		જો (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		નહીંતર
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	જો (option_recurse_submodules.nr > 0) {
		સંરચના string_list_item *item;
		સંરચના strbuf sb = STRBUF_INIT;
		પૂર્ણાંક val;

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

		જો (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		જો (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		નહીંતર જો (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} નહીંતર જો (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_વૃક્ષ,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, ખાલી);

	જો (real_git_dir) {
		મુક્ત((અક્ષર *)git_dir);
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

	refs_create_refdir_stubs(the_repository, git_dir, ખાલી);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, ખાલી);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	જો (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	જો (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	જો (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	જો (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	જો (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	જો (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	જો (option_origin) {
		મુક્ત(remote_name);
		remote_name = xstrdup(option_origin);
	}

	જો (!remote_name)
		remote_name = xstrdup("origin");

	જો (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	જો (option_bare) {
		જો (option_mirror)
			src_ref_prefix = "refs/";
		strbuf_addstr(&branch_ટોચ, src_ref_prefix);

		repo_config_set(the_repository, "core.bare", "true");
	} નહીંતર જો (!option_rev) {
		strbuf_addf(&branch_ટોચ, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&ચાવી, "remote.%s.url", remote_name);
	repo_config_set(the_repository, ચાવી.buf, repo);
	strbuf_reset(&ચાવી);

	જો (!option_tags) {
		strbuf_addf(&ચાવી, "remote.%s.tagOpt", remote_name);
		repo_config_set(the_repository, ચાવી.buf, "--no-tags");
		strbuf_reset(&ચાવી);
	}

	remote = remote_get_early(remote_name);

	જો (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_prefix,
				branch_ટોચ.buf);

	path = get_repo_path(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && path && !is_bundle;
	જો (is_local) {
		જો (option_depth)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		જો (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		જો (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		જો (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		જો (!access(mkpath("%s/shallow", path), F_OK)) {
			જો (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			જો (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	જો (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? path : ખાલી);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, path ? path : remote->url.v[0]);
	transport_set_verbosity(transport, option_verbosity, option_progress);
	transport->family = family;
	transport->cloning = 1;

	જો (is_bundle) {
		સંરચના bundle_header header = BUNDLE_HEADER_INIT;
		પૂર્ણાંક fd = read_bundle_header(path, &header);
		પૂર્ણાંક has_filter = header.filter.choice != LOFC_DISABLED;

		જો (fd > 0)
			close(fd);
		bundle_header_release(&header);
		જો (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_set_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	જો (reject_shallow)
		transport_set_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	જો (option_depth)
		transport_set_option(transport, TRANS_OPT_DEPTH,
				     option_depth);
	જો (option_since)
		transport_set_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	જો (option_not.nr)
		transport_set_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (અચલ અક્ષર *)&option_not);
	જો (option_single_branch) {
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		જો (option_branch)
			opts.wants_head = 0;
	}

	જો (option_upload_pack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	જો (server_options.nr)
		transport->server_options = &server_options;

	જો (filter_options.choice) {
		અચલ અક્ષર *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	જો (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	જો (option_rev) {
		option_tags = 0;
		option_single_branch = 0;
		opts.wants_head = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	જો (option_tags || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, TAG_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	જો (option_branch)
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
	જો (opts.wants_head || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_ધકેલો(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_version(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_set_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, ખાલી, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	જો (bundle_uri) {
		સંરચના remote_state *state;
		પૂર્ણાંક has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = ખાલી;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		જો (repo_init(the_repository, git_dir, work_વૃક્ષ))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		નહીંતર જો (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		નહીંતર જો (has_heuristic)
			repo_config_set_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		મુક્ત(the_repository->remote_state);
		the_repository->remote_state = state;
	} નહીંતર {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		જો (transport->bundles &&
		    hashmap_get_size(&transport->bundles->bundles)) {
			સંરચના remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = ખાલી;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			જો (repo_init(the_repository, git_dir, work_વૃક્ષ))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			નહીંતર જો (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			મુક્ત(the_repository->remote_state);
			the_repository->remote_state = state;
		} નહીંતર {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	જો (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	જો (mapped_refs) {
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
		માટે (ref = refs; ref; ref = ref->next)
			જો (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				તોડો;
			}

		જો (!is_local && !complete_refs_before_fetch) {
			જો (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_head = find_ref_by_name(refs, "HEAD");
	remote_head_points_at = guess_remote_head(remote_head, mapped_refs,
						  REMOTE_GUESS_HEAD_QUIET);

	જો (option_branch) {
		our_head_points_at = find_remote_branch(mapped_refs, option_branch);
		જો (!our_head_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} નહીંતર જો (option_rev) {
		our_head_points_at = mapped_refs;
		જો (!our_head_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} નહીંતર જો (remote_head_points_at) {
		our_head_points_at = remote_head_points_at;
	} નહીંતર જો (remote_head) {
		our_head_points_at = ખાલી;
	} નહીંતર {
		અક્ષર *to_free = ખાલી;
		અચલ અક્ષર *branch;

		જો (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		જો (transport_ls_refs_options.unborn_head_target &&
		    skip_prefix(transport_ls_refs_options.unborn_head_target,
				"refs/heads/", &branch)) {
			unborn_head  = xstrdup(transport_ls_refs_options.unborn_head_target);
		} નહીંતર {
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

		મુક્ત(to_free);
	}

	જો (!option_rev)
		write_refspec_config(src_ref_prefix, our_head_points_at,
				     remote_head_points_at, &branch_ટોચ);

	જો (filter_options.choice)
		partial_clone_register(remote_name, &filter_options);

	જો (is_local)
		clone_local(path, git_dir);
	નહીંતર જો (mapped_refs && complete_refs_before_fetch) {
		જો (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_head_points_at,
			   branch_ટોચ.buf, reflog_સંદેશ.buf, transport,
			   !is_local);

	update_head(&opts, our_head_points_at, remote_head, unborn_head, reflog_સંદેશ.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_progress = transport->progress;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	જો (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	જો (option_sparse_checkout && git_sparse_checkout_init(dir))
		પાછા 1;

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

	મુક્ત(remote_name);
	strbuf_release(&reflog_સંદેશ);
	strbuf_release(&branch_ટોચ);
	strbuf_release(&buf);
	strbuf_release(&ચાવી);
	free_refs(mapped_refs);
	free_refs(remote_head_points_at);
	મુક્ત(unborn_head);
	મુક્ત(dir);
	મુક્ત(path);
	મુક્ત(repo_to_free);
	junk_mode = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	પાછા err;
}
