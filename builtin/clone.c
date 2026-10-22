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

#include "builtin.h"

#include "abspath.h"
#include "advice.h"
#include "config.h"
#include "copy.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "lockfile.h"
#include "parse-options.h"
#include "refs.h"
#include "refspec.h"
#include "object-file.h"
#include "odb.h"
#include "tree.h"
#include "tree-walk.h"
#include "unpack-trees.h"
#include "transport.h"
#include "strbuf.h"
#include "dir.h"
#include "dir-iterator.h"
#include "iterator.h"
#include "sigchain.h"
#include "branch.h"
#include "remote.h"
#include "run-command.h"
#include "setup.h"
#include "connected.h"
#include "packfile.h"
#include "path.h"
#include "pkt-line.h"
#include "list-objects-filter-options.h"
#include "hook.h"
#include "bundle.h"
#include "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

sa_nracanaa clone_opts {
	puur_naa_nka wants_head;
	puur_naa_nka detach;
};
#define CLONE_OPTS_INIT { \
	.wants_head = 1 /* default enabled */ \
}

static puur_naa_nka option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
static puur_naa_nka option_local = -1, option_no_hardlinks, option_shared;
static puur_naa_nka option_tags = 1; /* default enabled */
static puur_naa_nka option_shallow_submodules;
static puur_naa_nka config_reject_shallow = -1;    /* unspecified */
static var_na *remote_name = NULL;
static var_na *option_branch = NULL;
static puur_naa_nka option_verbosity;
static sa_nracanaa string_list option_required_reference = STRING_LIST_INIT_NODUP;
static sa_nracanaa string_list option_optional_reference = STRING_LIST_INIT_NODUP;
static puur_naa_nka max_jobs = -1;
static sa_nracanaa string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
static puur_naa_nka config_filter_submodules = -1;    /* unspecified */
static puur_naa_nka option_remote_submodules;

static puur_naa_nka recurse_submodules_cb(const sa_nracanaa option *opt,
				 const var_na *arg, puur_naa_nka unset)
{
	ᨀᨘ (unset)
		string_list_clear((sa_nracanaa string_list *)opt->value, 0);
	ᨀᨘᨁ ᨀᨘ (arg)
		string_list_append((sa_nracanaa string_list *)opt->value, arg);
	ᨀᨘᨁ
		string_list_append((sa_nracanaa string_list *)opt->value,
				   (const var_na *)opt->defval);

	ᨀᨘᨒ 0;
}

static const var_na *get_repo_path_1(sa_nracanaa strbuf *path, puur_naa_nka *is_bundle)
{
	static const var_na *suffix[] = { "/.git", "", ".git/.git", ".git" };
	static const var_na *bundle_suffix[] = { ".bundle", "" };
	size_t baselen = path->len;
	sa_nracanaa stat st;
	puur_naa_nka i;

	for (i = 0; i < ARRAY_SIZE(suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, suffix[i]);
		ᨀᨘ (stat(path->buf, &st))
			continue;
		ᨀᨘ (S_ISDIR(st.st_mode) && is_git_directory(path->buf)) {
			*is_bundle = 0;
			ᨀᨘᨒ path->buf;
		} ᨀᨘᨁ ᨀᨘ (S_ISREG(st.st_mode) && st.st_size > 8) {
			/* Is it a "gitfile"? */
			var_na signature[8];
			const var_na *dst;
			puur_naa_nka len, fd = open(path->buf, O_RDONLY);
			ᨀᨘ (fd < 0)
				continue;
			len = read_in_full(fd, signature, 8);
			close(fd);
			ᨀᨘ (len != 8 || strncmp(signature, "gitdir: ", 8))
				continue;
			dst = read_gitfile(path->buf);
			ᨀᨘ (dst) {
				*is_bundle = 0;
				ᨀᨘᨒ dst;
			}
		}
	}

	for (i = 0; i < ARRAY_SIZE(bundle_suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, bundle_suffix[i]);
		ᨀᨘ (!stat(path->buf, &st) && S_ISREG(st.st_mode)) {
			*is_bundle = 1;
			ᨀᨘᨒ path->buf;
		}
	}

	ᨀᨘᨒ NULL;
}

static var_na *get_repo_path(const var_na *repo, puur_naa_nka *is_bundle)
{
	sa_nracanaa strbuf path = STRBUF_INIT;
	const var_na *raw;
	var_na *canon;

	strbuf_addstr(&path, repo);
	raw = get_repo_path_1(&path, is_bundle);
	canon = raw ? absolute_pathdup(raw) : NULL;
	strbuf_release(&path);
	ᨀᨘᨒ canon;
}

sa_nracanaa collect_alternates_data {
	sa_nracanaa strvec *alternates;
	bool required;
};

static puur_naa_nka collect_one_alternate(sa_nracanaa string_list_item *item, void *cb_data)
{
	sa_nracanaa collect_alternates_data *data = cb_data;
	sa_nracanaa strbuf err = STRBUF_INIT;
	var_na *ref_git = compute_alternate_path(item->string, &err);

	ᨀᨘ (!ref_git) {
		ᨀᨘ (data->required)
			die("%s", err.buf);
		ᨀᨘᨁ
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				item->string, err.buf);
	} ᨀᨘᨁ {
		strvec_pushf(data->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	free(ref_git);
	ᨀᨘᨒ 0;
}

static void read_alternates(sa_nracanaa strvec *alternates, const var_na *src_repo)
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
	sa_nracanaa strbuf path = STRBUF_INIT;
	sa_nracanaa strbuf line = STRBUF_INIT;

	strbuf_addf(&path, "%s/objects/info/alternates", src_repo);

	in = fopen(path.buf, "r");
	ᨀᨘ (!in) {
		ᨀᨘ (errno == ENOENT)
			goto out;
		die_errno("could not read alternates file '%s'", path.buf);
	}

	ᨀᨛ (strbuf_getline(&line, in) != EOF) {
		var_na *abs_path;
		ᨀᨘ (!line.len || line.buf[0] == '#')
			continue;
		ᨀᨘ (is_absolute_path(line.buf)) {
			strvec_push(alternates, line.buf);
			continue;
		}
		abs_path = mkpathdup("%s/objects/%s", src_repo, line.buf);
		ᨀᨘ (!normalize_path_copy(abs_path, abs_path))
			strvec_push(alternates, abs_path);
		ᨀᨘᨁ
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, line.buf);
		free(abs_path);
	}

out:
	strbuf_release(&path);
	strbuf_release(&line);
	ᨀᨘ (in)
		fclose(in);
}

static void collect_alternates(sa_nracanaa strvec *alternates,
			       const var_na *local_source_repo)
{
	ᨀᨘ (option_required_reference.nr || option_optional_reference.nr) {
		sa_nracanaa collect_alternates_data data = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &data);
		data.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &data);
	}

	ᨀᨘ (local_source_repo) {
		sa_nracanaa strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_source_repo);
		ᨀᨘ (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		ᨀᨘᨁ
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

static void mkdir_if_missing(const var_na *pathname, mode_t mode)
{
	sa_nracanaa stat st;

	ᨀᨘ (!mkdir(pathname, mode))
		ᨀᨘᨒ;

	ᨀᨘ (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	ᨀᨘᨁ ᨀᨘ (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	ᨀᨘᨁ ᨀᨘ (!S_ISDIR(st.st_mode))
		die(_("%s exists and is not a directory"), pathname);
}

static void copy_or_link_directory(sa_nracanaa strbuf *src, sa_nracanaa strbuf *dest,
				   const var_na *src_repo)
{
	puur_naa_nka src_len, dest_len;
	sa_nracanaa dir_iterator *iter;
	puur_naa_nka iter_status;

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
	die_upon_dubious_ownership(NULL, NULL, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	ᨀᨘ (!iter) {
		ᨀᨘ (errno == ENOTDIR) {
			puur_naa_nka saved_errno = errno;
			sa_nracanaa stat st;

			ᨀᨘ (!lstat(src->buf, &st) && S_ISLNK(st.st_mode))
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

	ᨀᨛ ((iter_status = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_path);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_path);

		ᨀᨘ (S_ISLNK(iter->st.st_mode))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_path);

		ᨀᨘ (S_ISDIR(iter->st.st_mode)) {
			mkdir_if_missing(dest->buf, 0777);
			continue;
		}

		/* Alternates were already handled earlier. */
		ᨀᨘ (!fspathcmp(iter->relative_path, "info/alternates"))
			continue;

		ᨀᨘ (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		ᨀᨘ (!option_no_hardlinks) {
			ᨀᨘ (!link(src->buf, dest->buf)) {
				sa_nracanaa stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				ᨀᨘ (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				ᨀᨘ (st.st_mode != iter->st.st_mode ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_size != iter->st.st_size ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				continue;
			}
			ᨀᨘ (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		ᨀᨘ (copy_file_with_time(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	ᨀᨘ (iter_status != ITER_DONE) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

static void clone_local(const var_na *src_repo, const var_na *dest_repo)
{
	ᨀᨘ (!option_shared) {
		sa_nracanaa strbuf src = STRBUF_INIT;
		sa_nracanaa strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	ᨀᨘ (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

static const var_na *junk_work_tree;
static puur_naa_nka junk_work_tree_flags;
static const var_na *junk_git_dir;
static puur_naa_nka junk_git_dir_flags;
static ga_nanaa {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_mode = JUNK_LEAVE_NONE;

static const var_na junk_leave_repo_msg[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

static void remove_junk(void)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;

	switch (junk_mode) {
	pak_sa JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_msg));
		/* fall-through */
	pak_sa JUNK_LEAVE_ALL:
		ᨀᨘᨒ;
	default:
		/* proceed to removal */
		bha_nga;
	}

	ᨀᨘ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	ᨀᨘ (junk_work_tree) {
		strbuf_addstr(&sb, junk_work_tree);
		remove_dir_recursively(&sb, junk_work_tree_flags);
	}
	strbuf_release(&sb);
}

static void remove_junk_on_signal(puur_naa_nka signo)
{
	remove_junk();
	sigchain_pop(signo);
	raise(signo);
}

static sa_nracanaa ref *find_remote_branch(const sa_nracanaa ref *refs, const var_na *branch)
{
	sa_nracanaa ref *ref;
	sa_nracanaa strbuf head = STRBUF_INIT;
	strbuf_addstr(&head, "refs/heads/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	ᨀᨘ (ref)
		ᨀᨘᨒ ref;

	strbuf_addstr(&head, "refs/tags/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	ᨀᨘᨒ ref;
}

static sa_nracanaa ref *wanted_peer_refs(sa_nracanaa clone_opts *opts,
				    const sa_nracanaa ref *refs,
				    sa_nracanaa refspec *refspec)
{
	sa_nracanaa ref *local_refs = NULL;
	sa_nracanaa ref **tail = &local_refs;
	sa_nracanaa ref *to_free = NULL;

	ᨀᨘ (opts->wants_head) {
		sa_nracanaa ref *head = copy_ref(find_ref_by_name(refs, "HEAD"));
		ᨀᨘ (head)
			tail_link_ref(head, &tail);
		ᨀᨘ (option_single_branch)
			refs = to_free =
				guess_remote_head(head, refs,
						  REMOTE_GUESS_HEAD_QUIET);
	} ᨀᨘᨁ ᨀᨘ (option_single_branch) {
		local_refs = NULL;
		tail = &local_refs;
		refs = to_free = copy_ref(find_remote_branch(refs, option_branch));
	}

	for (size_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->items[i], &tail, 0);

	free_one_ref(to_free);

	ᨀᨘᨒ local_refs;
}

static void write_remote_refs(const sa_nracanaa ref *local_refs)
{
	const sa_nracanaa ref *r;

	sa_nracanaa ref_transaction *t;
	sa_nracanaa strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	ᨀᨘ (!t)
		die("%s", err.buf);

	for (r = local_refs; r; r = r->next) {
		ᨀᨘ (!r->peer_ref)
			continue;
		ᨀᨘ (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   NULL, 0, NULL, &err))
			die("%s", err.buf);
	}

	ᨀᨘ (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

static void write_followtags(const sa_nracanaa ref *refs, const var_na *msg)
{
	const sa_nracanaa ref *ref;
	for (ref = refs; ref; ref = ref->next) {
		ᨀᨘ (!starts_with(ref->name, "refs/tags/"))
			continue;
		ᨀᨘ (ends_with(ref->name, "^{}"))
			continue;
		ᨀᨘ (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			continue;
		refs_update_ref(get_main_ref_store(the_repository), msg,
				ref->name, &ref->old_oid, NULL, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

static const sa_nracanaa object_id *iterate_ref_map(void *cb_data)
{
	sa_nracanaa ref **rm = cb_data;
	sa_nracanaa ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	ᨀᨛ (ref && !ref->peer_ref)
		ref = ref->next;
	ᨀᨘ (!ref)
		ᨀᨘᨒ NULL;

	*rm = ref->next;
	ᨀᨘᨒ &ref->old_oid;
}

static void update_remote_refs(const sa_nracanaa ref *refs,
			       const sa_nracanaa ref *mapped_refs,
			       const sa_nracanaa ref *remote_head_points_at,
			       const var_na *branch_top,
			       const var_na *msg,
			       sa_nracanaa transport *transport,
			       puur_naa_nka check_connectivity)
{
	const sa_nracanaa ref *rm = mapped_refs;

	ᨀᨘ (check_connectivity) {
		sa_nracanaa check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.progress = transport->progress;

		ᨀᨘ (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	ᨀᨘ (refs) {
		write_remote_refs(mapped_refs);
		ᨀᨘ (option_single_branch && option_tags)
			write_followtags(refs, msg);
	}

	ᨀᨘ (remote_head_points_at && remote_head_points_at->peer_ref && !option_bare) {
		sa_nracanaa strbuf head_ref = STRBUF_INIT;
		strbuf_addstr(&head_ref, branch_top);
		strbuf_addstr(&head_ref, "HEAD");
		ᨀᨘ (refs_update_symref(get_main_ref_store(the_repository), head_ref.buf,
				       remote_head_points_at->peer_ref->name,
				       msg) < 0)
			die(_("unable to update %s"), head_ref.buf);
		strbuf_release(&head_ref);
	}
}

static void update_head(sa_nracanaa clone_opts *opts, const sa_nracanaa ref *our, const sa_nracanaa ref *remote,
			const var_na *unborn, const var_na *msg)
{
	const var_na *head;
	ᨀᨘ (our && !opts->detach && skip_prefix(our->name, "refs/heads/", &head)) {
		/* Local default branch link */
		ᨀᨘ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, NULL) < 0)
			die(_("unable to update HEAD"));
		ᨀᨘ (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					msg, "HEAD", &our->old_oid, NULL, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, head, remote_name, our->name);
		}
	} ᨀᨘᨁ ᨀᨘ (our) {
		sa_nracanaa commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), msg,
				"HEAD", &c->object.oid, NULL, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ᨀᨘᨁ ᨀᨘ (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), msg,
				"HEAD", &remote->old_oid, NULL, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ᨀᨘᨁ ᨀᨘ (unborn && skip_prefix(unborn, "refs/heads/", &head)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		ᨀᨘ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, NULL) < 0)
			die(_("unable to update HEAD"));
		ᨀᨘ (!option_bare)
			install_branch_config(0, head, remote_name, unborn);
	}
}

static puur_naa_nka git_sparse_checkout_init(const var_na *repo)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	puur_naa_nka result = 0;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	strvec_pushl(&cmd.args, "-C", repo, "sparse-checkout", "set", NULL);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	ᨀᨘ (run_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		result = 1;
	}

	ᨀᨘᨒ result;
}

static puur_naa_nka checkout(puur_naa_nka submodule_progress,
		    sa_nracanaa list_objects_filter_options *filter_options,
		    puur_naa_nka filter_submodules,
		    ga_nanaa ref_storage_format ref_storage_format)
{
	sa_nracanaa object_id oid;
	var_na *head;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	sa_nracanaa unpack_trees_options opts;
	sa_nracanaa tree *tree;
	sa_nracanaa tree_desc t;
	puur_naa_nka err = 0;
	sa_nracanaa run_hooks_opt hook_opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	ᨀᨘ (option_no_checkout)
		ᨀᨘᨒ 0;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, NULL);
	ᨀᨘ (!head) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		ᨀᨘᨒ 0;
	}
	ᨀᨘ (!strcmp(head, "HEAD")) {
		ᨀᨘ (advice_enabled(ADVICE_DETACHED_HEAD))
			detach_advice(oid_to_hex(&oid));
		FREE_AND_NULL(head);
	} ᨀᨘᨁ {
		ᨀᨘ (!starts_with(head, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	setup_work_tree(the_repository);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, parimaa_na opts);
	opts.update = 1;
	opts.merge = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_merge;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	init_checkout_metadata(&opts.meta, head, &oid, NULL);

	tree = repo_parse_tree_indirect(the_repository, &oid);
	ᨀᨘ (!tree)
		die(_("unable to parse commit %s"), oid_to_hex(&oid));
	ᨀᨘ (repo_parse_tree(the_repository, tree) < 0)
		exit(128);
	init_tree_desc(&t, &tree->object.oid, tree->buffer, tree->size);
	ᨀᨘ (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	free(head);

	ᨀᨘ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.args, oid_to_hex(null_oid(the_hash_algo)),
		     oid_to_hex(&oid), "1", NULL);
	err |= run_hooks_opt(the_repository, "post-checkout", &hook_opt);

	ᨀᨘ (!err && (option_recurse_submodules.nr > 0)) {
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
		strvec_pushl(&cmd.args, "submodule", "update", "--require-init",
			     "--recursive", NULL);

		ᨀᨘ (option_shallow_submodules == 1)
			strvec_push(&cmd.args, "--depth=1");

		ᨀᨘ (max_jobs != -1)
			strvec_pushf(&cmd.args, "--jobs=%d", max_jobs);

		ᨀᨘ (submodule_progress)
			strvec_push(&cmd.args, "--progress");

		ᨀᨘ (option_verbosity < 0)
			strvec_push(&cmd.args, "--quiet");

		ᨀᨘ (option_remote_submodules) {
			strvec_push(&cmd.args, "--remote");
			strvec_push(&cmd.args, "--no-fetch");
		}

		ᨀᨘ (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(ref_storage_format));

		ᨀᨘ (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.args, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		ᨀᨘ (option_single_branch >= 0)
			strvec_push(&cmd.args, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = run_command(&cmd);
	}

	ᨀᨘᨒ err;
}

static puur_naa_nka git_clone_config(const var_na *k, const var_na *v,
			    const sa_nracanaa config_context *ctx, void *cb)
{
	ᨀᨘ (!strcmp(k, "clone.defaultremotename")) {
		ᨀᨘ (!v)
			ᨀᨘᨒ config_error_nonbool(k);
		free(remote_name);
		remote_name = xstrdup(v);
	}
	ᨀᨘ (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	ᨀᨘ (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	ᨀᨘᨒ git_default_config(k, v, ctx, cb);
}

static puur_naa_nka write_one_config(const var_na *key, const var_na *value,
			    const sa_nracanaa config_context *ctx,
			    void *data)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	puur_naa_nka apply_failed = git_clone_config(key, value, ctx, data);
	ᨀᨘ (apply_failed)
		ᨀᨘᨒ apply_failed;

	ᨀᨘᨒ repo_config_set_multivar_gently(the_repository, key,
					       value ? value : "true",
					       CONFIG_REGEX_NONE, 0);
}

static void write_config(sa_nracanaa string_list *config)
{
	puur_naa_nka i;

	for (i = 0; i < config->nr; i++) {
		ᨀᨘ (git_config_parse_parameter(config->items[i].string,
					       write_one_config, NULL) < 0)
			die(_("unable to write parameters to config file"));
	}
}

static void write_refspec_config(const var_na *src_ref_prefix,
		const sa_nracanaa ref *our_head_points_at,
		const sa_nracanaa ref *remote_head_points_at,
		sa_nracanaa strbuf *branch_top)
{
	sa_nracanaa strbuf key = STRBUF_INIT;
	sa_nracanaa strbuf value = STRBUF_INIT;

	ᨀᨘ (option_mirror || !option_bare) {
		ᨀᨘ (option_single_branch && !option_mirror) {
			ᨀᨘ (option_branch) {
				ᨀᨘ (starts_with(our_head_points_at->name, "refs/tags/"))
					strbuf_addf(&value, "+%s:%s", our_head_points_at->name,
						our_head_points_at->name);
				ᨀᨘᨁ
					strbuf_addf(&value, "+%s:%s%s", our_head_points_at->name,
						branch_top->buf, option_branch);
			} ᨀᨘᨁ ᨀᨘ (remote_head_points_at) {
				const var_na *head = remote_head_points_at->name;
				ᨀᨘ (!skip_prefix(head, "refs/heads/", &head))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&value, "+%s:%s%s", remote_head_points_at->name,
						branch_top->buf, head);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} ᨀᨘᨁ {
			strbuf_addf(&value, "+%s*:%s*", src_ref_prefix, branch_top->buf);
		}
		/* Configure the remote */
		ᨀᨘ (value.len) {
			strbuf_addf(&key, "remote.%s.fetch", remote_name);
			repo_config_set_multivar(the_repository, key.buf, value.buf, "^$", 0);
			strbuf_reset(&key);

			ᨀᨘ (option_mirror) {
				strbuf_addf(&key, "remote.%s.mirror", remote_name);
				repo_config_set(the_repository, key.buf, "true");
				strbuf_reset(&key);
			}
		}
	}

	strbuf_release(&key);
	strbuf_release(&value);
}

static void dissociate_from_references(void)
{
	var_na *alternates = repo_git_path(the_repository, "objects/info/alternates");

	ᨀᨘ (!access(alternates, F_OK)) {
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.args, "repack", "-a", "-d", NULL);
		ᨀᨘ (run_command(&cmd))
			die(_("cannot repack to clean up"));
		ᨀᨘ (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	free(alternates);
}

static puur_naa_nka path_exists(const var_na *path)
{
	sa_nracanaa stat sb;
	ᨀᨘᨒ !stat(path, &sb);
}

puur_naa_nka cmd_clone(puur_naa_nka argc,
	      const var_na **argv,
	      const var_na *prefix,
	      sa_nracanaa repository *repository UNUSED)
{
	puur_naa_nka is_bundle = 0, is_local;
	puur_naa_nka reject_shallow = 0;
	const var_na *repo_name, *repo, *work_tree, *git_dir;
	var_na *repo_to_free = NULL;
	var_na *path = NULL, *dir, *display_repo = NULL;
	puur_naa_nka dest_exists, real_dest_exists = 0;
	const sa_nracanaa ref *refs, *remote_head;
	sa_nracanaa ref *remote_head_points_at = NULL;
	const sa_nracanaa ref *our_head_points_at;
	var_na *unborn_head = NULL;
	sa_nracanaa ref *mapped_refs = NULL;
	const sa_nracanaa ref *ref;
	sa_nracanaa strbuf key = STRBUF_INIT;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa strbuf branch_top = STRBUF_INIT, reflog_msg = STRBUF_INIT;
	sa_nracanaa transport *transport = NULL;
	const var_na *src_ref_prefix = "refs/heads/";
	sa_nracanaa remote *remote;
	puur_naa_nka err = 0, complete_refs_before_fetch = 1;
	puur_naa_nka submodule_progress;
	puur_naa_nka filter_submodules = 0;
	puur_naa_nka hash_algo;
	ga_nanaa ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	const puur_naa_nka do_not_override_repo_unix_permissions = -1;
	puur_naa_nka option_reject_shallow = -1; /* unspecified */
	puur_naa_nka deepen = 0;
	var_na *option_template = NULL, *option_depth = NULL, *option_since = NULL;
	var_na *option_origin = NULL;
	sa_nracanaa string_list option_not = STRING_LIST_INIT_NODUP;
	const var_na *real_git_dir = NULL;
	const var_na *ref_storage_format_uri = NULL;
	const var_na *option_upload_pack = "git-upload-pack";
	puur_naa_nka option_progress = -1;
	puur_naa_nka option_sparse_checkout = 0;
	ga_nanaa transport_family family = TRANSPORT_FAMILY_ALL;
	sa_nracanaa string_list option_config = STRING_LIST_INIT_DUP;
	puur_naa_nka option_dissociate = 0;
	sa_nracanaa list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	puur_naa_nka option_filter_submodules = -1; /* unspecified */
	sa_nracanaa string_list server_options = STRING_LIST_INIT_NODUP;
	const var_na *bundle_uri = NULL;
	var_na *option_rev = NULL;

	sa_nracanaa clone_opts opts = CLONE_OPTS_INIT;

	sa_nracanaa transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	sa_nracanaa option builtin_clone_options[] = {
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
			.value = &option_recurse_submodules,
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
	sa_nracanaa strvec alternates = STRVEC_INIT;

	const var_na * const builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		NULL
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_identity("clone");

	repo_config(the_repository, git_clone_config, NULL);

	argc = parse_options(argc, argv, prefix, builtin_clone_options,
			     builtin_clone_usage, 0);

	ᨀᨘ (argc > 2)
		usage_msg_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	ᨀᨘ (argc == 0)
		usage_msg_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	ᨀᨘ (option_depth || option_since || option_not.nr)
		deepen = 1;
	ᨀᨘ (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	ᨀᨘ (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, NULL);
		ᨀᨘ (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	ᨀᨘ (option_mirror) {
		option_bare = 1;
		option_tags = 0;
	}

	ᨀᨘ (option_bare) {
		ᨀᨘ (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	ᨀᨘ (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	path = get_repo_path(repo_name, &is_bundle);
	ᨀᨘ (path) {
		FREE_AND_NULL(path);
		repo = repo_to_free = absolute_pathdup(repo_name);
	} ᨀᨘᨁ ᨀᨘ (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} ᨀᨘᨁ
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	ᨀᨘ (option_depth && atoi(option_depth) < 1)
		die(_("depth %s is not a positive number"), option_depth);

	ᨀᨘ (argc == 2)
		dir = xstrdup(argv[1]);
	ᨀᨘᨁ
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = path_exists(dir);
	ᨀᨘ (dest_exists && !is_empty_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	ᨀᨘ (real_git_dir) {
		real_dest_exists = path_exists(real_git_dir);
		ᨀᨘ (real_dest_exists && !is_empty_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_msg, "clone: from %s",
		    display_repo ? display_repo : repo);
	free(display_repo);

	ᨀᨘ (option_bare)
		work_tree = NULL;
	ᨀᨘᨁ {
		work_tree = getenv("GIT_WORK_TREE");
		ᨀᨘ (work_tree && path_exists(work_tree))
			die(_("working tree '%s' already exists."), work_tree);
	}

	ᨀᨘ (option_bare || work_tree)
		git_dir = xstrdup(dir);
	ᨀᨘᨁ {
		work_tree = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_push_common(remove_junk_on_signal);

	ᨀᨘ (!option_bare) {
		ᨀᨘ (safe_create_leading_directories_const(the_repository, work_tree) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_tree);
		ᨀᨘ (dest_exists)
			junk_work_tree_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		ᨀᨘᨁ ᨀᨘ (mkdir(work_tree, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_tree);
		junk_work_tree = work_tree;
	}

	ᨀᨘ (real_git_dir) {
		ᨀᨘ (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} ᨀᨘᨁ {
		ᨀᨘ (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	ᨀᨘ (safe_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	ᨀᨘ (0 <= option_verbosity) {
		ᨀᨘ (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		ᨀᨘᨁ
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	ᨀᨘ (option_recurse_submodules.nr > 0) {
		sa_nracanaa string_list_item *item;
		sa_nracanaa strbuf sb = STRBUF_INIT;
		puur_naa_nka val;

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

		ᨀᨘ (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		ᨀᨘ (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		ᨀᨘᨁ ᨀᨘ (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} ᨀᨘᨁ ᨀᨘ (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_tree,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, NULL);

	ᨀᨘ (real_git_dir) {
		free((var_na *)git_dir);
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

	refs_create_refdir_stubs(the_repository, git_dir, NULL);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, NULL);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	ᨀᨘ (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	ᨀᨘ (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	ᨀᨘ (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	ᨀᨘ (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	ᨀᨘ (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	ᨀᨘ (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	ᨀᨘ (option_origin) {
		free(remote_name);
		remote_name = xstrdup(option_origin);
	}

	ᨀᨘ (!remote_name)
		remote_name = xstrdup("origin");

	ᨀᨘ (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	ᨀᨘ (option_bare) {
		ᨀᨘ (option_mirror)
			src_ref_prefix = "refs/";
		strbuf_addstr(&branch_top, src_ref_prefix);

		repo_config_set(the_repository, "core.bare", "true");
	} ᨀᨘᨁ ᨀᨘ (!option_rev) {
		strbuf_addf(&branch_top, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&key, "remote.%s.url", remote_name);
	repo_config_set(the_repository, key.buf, repo);
	strbuf_reset(&key);

	ᨀᨘ (!option_tags) {
		strbuf_addf(&key, "remote.%s.tagOpt", remote_name);
		repo_config_set(the_repository, key.buf, "--no-tags");
		strbuf_reset(&key);
	}

	remote = remote_get_early(remote_name);

	ᨀᨘ (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_prefix,
				branch_top.buf);

	path = get_repo_path(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && path && !is_bundle;
	ᨀᨘ (is_local) {
		ᨀᨘ (option_depth)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		ᨀᨘ (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		ᨀᨘ (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		ᨀᨘ (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		ᨀᨘ (!access(mkpath("%s/shallow", path), F_OK)) {
			ᨀᨘ (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			ᨀᨘ (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	ᨀᨘ (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? path : NULL);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, path ? path : remote->url.v[0]);
	transport_set_verbosity(transport, option_verbosity, option_progress);
	transport->family = family;
	transport->cloning = 1;

	ᨀᨘ (is_bundle) {
		sa_nracanaa bundle_header header = BUNDLE_HEADER_INIT;
		puur_naa_nka fd = read_bundle_header(path, &header);
		puur_naa_nka has_filter = header.filter.choice != LOFC_DISABLED;

		ᨀᨘ (fd > 0)
			close(fd);
		bundle_header_release(&header);
		ᨀᨘ (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_set_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	ᨀᨘ (reject_shallow)
		transport_set_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	ᨀᨘ (option_depth)
		transport_set_option(transport, TRANS_OPT_DEPTH,
				     option_depth);
	ᨀᨘ (option_since)
		transport_set_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	ᨀᨘ (option_not.nr)
		transport_set_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (const var_na *)&option_not);
	ᨀᨘ (option_single_branch) {
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		ᨀᨘ (option_branch)
			opts.wants_head = 0;
	}

	ᨀᨘ (option_upload_pack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	ᨀᨘ (server_options.nr)
		transport->server_options = &server_options;

	ᨀᨘ (filter_options.choice) {
		const var_na *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	ᨀᨘ (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	ᨀᨘ (option_rev) {
		option_tags = 0;
		option_single_branch = 0;
		opts.wants_head = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	ᨀᨘ (option_tags || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, TAG_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	ᨀᨘ (option_branch)
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
	ᨀᨘ (opts.wants_head || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_push(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_version(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_set_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, NULL, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	ᨀᨘ (bundle_uri) {
		sa_nracanaa remote_state *state;
		puur_naa_nka has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = NULL;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		ᨀᨘ (repo_init(the_repository, git_dir, work_tree))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		ᨀᨘᨁ ᨀᨘ (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		ᨀᨘᨁ ᨀᨘ (has_heuristic)
			repo_config_set_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		free(the_repository->remote_state);
		the_repository->remote_state = state;
	} ᨀᨘᨁ {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		ᨀᨘ (transport->bundles &&
		    hashmap_get_size(&transport->bundles->bundles)) {
			sa_nracanaa remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = NULL;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			ᨀᨘ (repo_init(the_repository, git_dir, work_tree))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			ᨀᨘᨁ ᨀᨘ (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			free(the_repository->remote_state);
			the_repository->remote_state = state;
		} ᨀᨘᨁ {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	ᨀᨘ (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	ᨀᨘ (mapped_refs) {
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
		for (ref = refs; ref; ref = ref->next)
			ᨀᨘ (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				bha_nga;
			}

		ᨀᨘ (!is_local && !complete_refs_before_fetch) {
			ᨀᨘ (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_head = find_ref_by_name(refs, "HEAD");
	remote_head_points_at = guess_remote_head(remote_head, mapped_refs,
						  REMOTE_GUESS_HEAD_QUIET);

	ᨀᨘ (option_branch) {
		our_head_points_at = find_remote_branch(mapped_refs, option_branch);
		ᨀᨘ (!our_head_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} ᨀᨘᨁ ᨀᨘ (option_rev) {
		our_head_points_at = mapped_refs;
		ᨀᨘ (!our_head_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} ᨀᨘᨁ ᨀᨘ (remote_head_points_at) {
		our_head_points_at = remote_head_points_at;
	} ᨀᨘᨁ ᨀᨘ (remote_head) {
		our_head_points_at = NULL;
	} ᨀᨘᨁ {
		var_na *to_free = NULL;
		const var_na *branch;

		ᨀᨘ (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		ᨀᨘ (transport_ls_refs_options.unborn_head_target &&
		    skip_prefix(transport_ls_refs_options.unborn_head_target,
				"refs/heads/", &branch)) {
			unborn_head  = xstrdup(transport_ls_refs_options.unborn_head_target);
		} ᨀᨘᨁ {
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

		free(to_free);
	}

	ᨀᨘ (!option_rev)
		write_refspec_config(src_ref_prefix, our_head_points_at,
				     remote_head_points_at, &branch_top);

	ᨀᨘ (filter_options.choice)
		partial_clone_register(remote_name, &filter_options);

	ᨀᨘ (is_local)
		clone_local(path, git_dir);
	ᨀᨘᨁ ᨀᨘ (mapped_refs && complete_refs_before_fetch) {
		ᨀᨘ (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_head_points_at,
			   branch_top.buf, reflog_msg.buf, transport,
			   !is_local);

	update_head(&opts, our_head_points_at, remote_head, unborn_head, reflog_msg.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_progress = transport->progress;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	ᨀᨘ (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	ᨀᨘ (option_sparse_checkout && git_sparse_checkout_init(dir))
		ᨀᨘᨒ 1;

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

	free(remote_name);
	strbuf_release(&reflog_msg);
	strbuf_release(&branch_top);
	strbuf_release(&buf);
	strbuf_release(&key);
	free_refs(mapped_refs);
	free_refs(remote_head_points_at);
	free(unborn_head);
	free(dir);
	free(path);
	free(repo_to_free);
	junk_mode = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	ᨀᨘᨒ err;
}
