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

#ضمن "builtin.h"

#ضمن "abspath.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "copy.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "lockfile.h"
#ضمن "parse-options.h"
#ضمن "refs.h"
#ضمن "refspec.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "tree.h"
#ضمن "tree-walk.h"
#ضمن "unpack-trees.h"
#ضمن "transport.h"
#ضمن "strbuf.h"
#ضمن "dir.h"
#ضمن "dir-iterator.h"
#ضمن "iterator.h"
#ضمن "sigchain.h"
#ضمن "branch.h"
#ضمن "remote.h"
#ضمن "run-command.h"
#ضمن "setup.h"
#ضمن "connected.h"
#ضمن "packfile.h"
#ضمن "path.h"
#ضمن "pkt-line.h"
#ضمن "list-objects-filter-options.h"
#ضمن "hook.h"
#ضمن "bundle.h"
#ضمن "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

بنية clone_opts {
	صحيح wants_head;
	صحيح detach;
};
#define CLONE_OPTS_INIT { \
	.wants_head = 1 /* default enabled */ \
}

ساكن صحيح option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
ساكن صحيح option_local = -1, option_no_hardlinks, option_shared;
ساكن صحيح option_tags = 1; /* default enabled */
ساكن صحيح option_shallow_submodules;
ساكن صحيح config_reject_shallow = -1;    /* unspecified */
ساكن حرف *remote_name = عدم;
ساكن حرف *option_branch = عدم;
ساكن صحيح option_verbosity;
ساكن بنية string_list option_required_reference = STRING_LIST_INIT_NODUP;
ساكن بنية string_list option_optional_reference = STRING_LIST_INIT_NODUP;
ساكن صحيح max_jobs = -1;
ساكن بنية string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
ساكن صحيح config_filter_submodules = -1;    /* unspecified */
ساكن صحيح option_remote_submodules;

ساكن صحيح recurse_submodules_cb(ثابت بنية option *opt,
				 ثابت حرف *arg, صحيح unset)
{
	إذا (unset)
		string_list_clear((بنية string_list *)opt->قىممەت, 0);
	وإلا إذا (arg)
		string_list_append((بنية string_list *)opt->قىممەت, arg);
	وإلا
		string_list_append((بنية string_list *)opt->قىممەت,
				   (ثابت حرف *)opt->defval);

	أرجع 0;
}

ساكن ثابت حرف *get_repo_path_1(بنية strbuf *path, صحيح *is_bundle)
{
	ساكن ثابت حرف *suffix[] = { "/.git", "", ".git/.git", ".git" };
	ساكن ثابت حرف *bundle_suffix[] = { ".bundle", "" };
	size_t baselen = path->len;
	بنية stat st;
	صحيح i;

	لكل (i = 0; i < ARRAY_SIZE(suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, suffix[i]);
		إذا (stat(path->buf, &st))
			استمر;
		إذا (S_ISDIR(st.st_mode) && is_git_directory(path->buf)) {
			*is_bundle = 0;
			أرجع path->buf;
		} وإلا إذا (S_ISREG(st.st_mode) && st.st_size > 8) {
			/* Is it a "gitfile"? */
			حرف signature[8];
			ثابت حرف *dst;
			صحيح len, fd = open(path->buf, O_RDONLY);
			إذا (fd < 0)
				استمر;
			len = read_in_full(fd, signature, 8);
			close(fd);
			إذا (len != 8 || strncmp(signature, "gitdir: ", 8))
				استمر;
			dst = read_gitfile(path->buf);
			إذا (dst) {
				*is_bundle = 0;
				أرجع dst;
			}
		}
	}

	لكل (i = 0; i < ARRAY_SIZE(bundle_suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, bundle_suffix[i]);
		إذا (!stat(path->buf, &st) && S_ISREG(st.st_mode)) {
			*is_bundle = 1;
			أرجع path->buf;
		}
	}

	أرجع عدم;
}

ساكن حرف *get_repo_path(ثابت حرف *repo, صحيح *is_bundle)
{
	بنية strbuf path = STRBUF_INIT;
	ثابت حرف *raw;
	حرف *canon;

	strbuf_addstr(&path, repo);
	raw = get_repo_path_1(&path, is_bundle);
	canon = raw ? absolute_pathdup(raw) : عدم;
	strbuf_release(&path);
	أرجع canon;
}

بنية collect_alternates_data {
	بنية strvec *alternates;
	bool required;
};

ساكن صحيح collect_one_alternate(بنية string_list_item *item, فراغ *cb_data)
{
	بنية collect_alternates_data *data = cb_data;
	بنية strbuf err = STRBUF_INIT;
	حرف *ref_git = compute_alternate_path(item->string, &err);

	إذا (!ref_git) {
		إذا (data->required)
			die("%s", err.buf);
		وإلا
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				item->string, err.buf);
	} وإلا {
		strvec_pushf(data->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	حرر(ref_git);
	أرجع 0;
}

ساكن فراغ read_alternates(بنية strvec *alternates, ثابت حرف *src_repo)
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
	بنية strbuf path = STRBUF_INIT;
	بنية strbuf قۇر = STRBUF_INIT;

	strbuf_addf(&path, "%s/objects/info/alternates", src_repo);

	in = fopen(path.buf, "r");
	إذا (!in) {
		إذا (errno == ENOENT)
			اذهب out;
		die_errno("could not read alternates file '%s'", path.buf);
	}

	طالما (strbuf_getline(&قۇر, in) != EOF) {
		حرف *abs_path;
		إذا (!قۇر.len || قۇر.buf[0] == '#')
			استمر;
		إذا (is_absolute_path(قۇر.buf)) {
			strvec_ئىتتىرىش(alternates, قۇر.buf);
			استمر;
		}
		abs_path = mkpathdup("%s/objects/%s", src_repo, قۇر.buf);
		إذا (!normalize_path_copy(abs_path, abs_path))
			strvec_ئىتتىرىش(alternates, abs_path);
		وإلا
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, قۇر.buf);
		حرر(abs_path);
	}

out:
	strbuf_release(&path);
	strbuf_release(&قۇر);
	إذا (in)
		fclose(in);
}

ساكن فراغ collect_alternates(بنية strvec *alternates,
			       ثابت حرف *local_source_repo)
{
	إذا (option_required_reference.nr || option_optional_reference.nr) {
		بنية collect_alternates_data data = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &data);
		data.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &data);
	}

	إذا (local_source_repo) {
		بنية strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_source_repo);
		إذا (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		وإلا
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

ساكن فراغ mkdir_if_missing(ثابت حرف *pathname, mode_t mode)
{
	بنية stat st;

	إذا (!mkdir(pathname, mode))
		أرجع;

	إذا (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	وإلا إذا (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	وإلا إذا (!S_ISDIR(st.st_mode))
		die(_("%s exists and is not a directory"), pathname);
}

ساكن فراغ copy_or_link_directory(بنية strbuf *src, بنية strbuf *dest,
				   ثابت حرف *src_repo)
{
	صحيح src_len, dest_len;
	بنية dir_iterator *iter;
	صحيح iter_status;

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
	die_upon_dubious_ownership(عدم, عدم, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	إذا (!iter) {
		إذا (errno == ENOTDIR) {
			صحيح saved_errno = errno;
			بنية stat st;

			إذا (!lstat(src->buf, &st) && S_ISLNK(st.st_mode))
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

	طالما ((iter_status = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_path);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_path);

		إذا (S_ISLNK(iter->st.st_mode))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_path);

		إذا (S_ISDIR(iter->st.st_mode)) {
			mkdir_if_missing(dest->buf, 0777);
			استمر;
		}

		/* Alternates were already handled earlier. */
		إذا (!fspathcmp(iter->relative_path, "info/alternates"))
			استمر;

		إذا (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		إذا (!option_no_hardlinks) {
			إذا (!link(src->buf, dest->buf)) {
				بنية stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				إذا (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				إذا (st.st_mode != iter->st.st_mode ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_size != iter->st.st_size ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				استمر;
			}
			إذا (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		إذا (copy_file_with_time(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	إذا (iter_status != ITER_DONE) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

ساكن فراغ clone_local(ثابت حرف *src_repo, ثابت حرف *dest_repo)
{
	إذا (!option_shared) {
		بنية strbuf src = STRBUF_INIT;
		بنية strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	إذا (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

ساكن ثابت حرف *junk_work_دەرەخ;
ساكن صحيح junk_work_دەرەخ_flags;
ساكن ثابت حرف *junk_git_dir;
ساكن صحيح junk_git_dir_flags;
ساكن تعداد {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_mode = JUNK_LEAVE_NONE;

ساكن ثابت حرف junk_leave_repo_ئۇچۇر[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

ساكن فراغ remove_junk(فراغ)
{
	بنية strbuf sb = STRBUF_INIT;

	بدل (junk_mode) {
	حالة JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_ئۇچۇر));
		/* fall-through */
	حالة JUNK_LEAVE_ALL:
		أرجع;
	افتراضي:
		/* proceed to removal */
		اكسر;
	}

	إذا (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	إذا (junk_work_دەرەخ) {
		strbuf_addstr(&sb, junk_work_دەرەخ);
		remove_dir_recursively(&sb, junk_work_دەرەخ_flags);
	}
	strbuf_release(&sb);
}

ساكن فراغ remove_junk_on_signal(صحيح signo)
{
	remove_junk();
	sigchain_چىقىرىش(signo);
	ارفع(signo);
}

ساكن بنية ref *find_remote_branch(ثابت بنية ref *refs, ثابت حرف *branch)
{
	بنية ref *ref;
	بنية strbuf head = STRBUF_INIT;
	strbuf_addstr(&head, "refs/heads/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	إذا (ref)
		أرجع ref;

	strbuf_addstr(&head, "refs/tags/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	أرجع ref;
}

ساكن بنية ref *wanted_peer_refs(بنية clone_opts *opts,
				    ثابت بنية ref *refs,
				    بنية refspec *refspec)
{
	بنية ref *local_refs = عدم;
	بنية ref **tail = &local_refs;
	بنية ref *to_free = عدم;

	إذا (opts->wants_head) {
		بنية ref *head = copy_ref(find_ref_by_name(refs, "HEAD"));
		إذا (head)
			tail_link_ref(head, &tail);
		إذا (option_single_branch)
			refs = to_free =
				guess_remote_head(head, refs,
						  REMOTE_GUESS_HEAD_QUIET);
	} وإلا إذا (option_single_branch) {
		local_refs = عدم;
		tail = &local_refs;
		refs = to_free = copy_ref(find_remote_branch(refs, option_branch));
	}

	لكل (size_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->بۇيۇملار[i], &tail, 0);

	free_one_ref(to_free);

	أرجع local_refs;
}

ساكن فراغ write_remote_refs(ثابت بنية ref *local_refs)
{
	ثابت بنية ref *r;

	بنية ref_transaction *t;
	بنية strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	إذا (!t)
		die("%s", err.buf);

	لكل (r = local_refs; r; r = r->next) {
		إذا (!r->peer_ref)
			استمر;
		إذا (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   عدم, 0, عدم, &err))
			die("%s", err.buf);
	}

	إذا (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

ساكن فراغ write_followtags(ثابت بنية ref *refs, ثابت حرف *ئۇچۇر)
{
	ثابت بنية ref *ref;
	لكل (ref = refs; ref; ref = ref->next) {
		إذا (!starts_with(ref->name, "refs/tags/"))
			استمر;
		إذا (ends_with(ref->name, "^{}"))
			استمر;
		إذا (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			استمر;
		refs_update_ref(get_main_ref_store(the_repository), ئۇچۇر,
				ref->name, &ref->old_oid, عدم, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

ساكن ثابت بنية object_id *iterate_ref_map(فراغ *cb_data)
{
	بنية ref **rm = cb_data;
	بنية ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	طالما (ref && !ref->peer_ref)
		ref = ref->next;
	إذا (!ref)
		أرجع عدم;

	*rm = ref->next;
	أرجع &ref->old_oid;
}

ساكن فراغ update_remote_refs(ثابت بنية ref *refs,
			       ثابت بنية ref *mapped_refs,
			       ثابت بنية ref *remote_head_points_at,
			       ثابت حرف *branch_چوققا,
			       ثابت حرف *ئۇچۇر,
			       بنية transport *transport,
			       صحيح check_connectivity)
{
	ثابت بنية ref *rm = mapped_refs;

	إذا (check_connectivity) {
		بنية check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.progress = transport->progress;

		إذا (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	إذا (refs) {
		write_remote_refs(mapped_refs);
		إذا (option_single_branch && option_tags)
			write_followtags(refs, ئۇچۇر);
	}

	إذا (remote_head_points_at && remote_head_points_at->peer_ref && !option_bare) {
		بنية strbuf head_ref = STRBUF_INIT;
		strbuf_addstr(&head_ref, branch_چوققا);
		strbuf_addstr(&head_ref, "HEAD");
		إذا (refs_update_symref(get_main_ref_store(the_repository), head_ref.buf,
				       remote_head_points_at->peer_ref->name,
				       ئۇچۇر) < 0)
			die(_("unable to update %s"), head_ref.buf);
		strbuf_release(&head_ref);
	}
}

ساكن فراغ update_head(بنية clone_opts *opts, ثابت بنية ref *our, ثابت بنية ref *remote,
			ثابت حرف *unborn, ثابت حرف *ئۇچۇر)
{
	ثابت حرف *head;
	إذا (our && !opts->detach && skip_prefix(our->name, "refs/heads/", &head)) {
		/* Local default branch link */
		إذا (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, عدم) < 0)
			die(_("unable to update HEAD"));
		إذا (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					ئۇچۇر, "HEAD", &our->old_oid, عدم, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, head, remote_name, our->name);
		}
	} وإلا إذا (our) {
		بنية commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), ئۇچۇر,
				"HEAD", &c->object.oid, عدم, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} وإلا إذا (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), ئۇچۇر,
				"HEAD", &remote->old_oid, عدم, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} وإلا إذا (unborn && skip_prefix(unborn, "refs/heads/", &head)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		إذا (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, عدم) < 0)
			die(_("unable to update HEAD"));
		إذا (!option_bare)
			install_branch_config(0, head, remote_name, unborn);
	}
}

ساكن صحيح git_sparse_checkout_init(ثابت حرف *repo)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;
	صحيح result = 0;
	بنية repo_config_قىممەتلەر *cfg = repo_config_قىممەتلەر(the_repository);

	strvec_pushl(&cmd.args, "-C", repo, "sparse-checkout", "set", عدم);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	إذا (run_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		result = 1;
	}

	أرجع result;
}

ساكن صحيح checkout(صحيح submodule_progress,
		    بنية list_objects_filter_options *filter_options,
		    صحيح filter_submodules,
		    تعداد ref_storage_format ref_storage_format)
{
	بنية object_id oid;
	حرف *head;
	بنية lock_file lock_file = LOCK_INIT;
	بنية unpack_trees_options opts;
	بنية دەرەخ *دەرەخ;
	بنية دەرەخ_desc t;
	صحيح err = 0;
	بنية run_hooks_opt hook_opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	إذا (option_no_checkout)
		أرجع 0;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, عدم);
	إذا (!head) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		أرجع 0;
	}
	إذا (!strcmp(head, "HEAD")) {
		إذا (advice_enabled(ADVICE_DETACHED_HEAD))
			detach_advice(oid_to_hex(&oid));
		FREE_AND_NULL(head);
	} وإلا {
		إذا (!starts_with(head, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	setup_work_دەرەخ(the_repository);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, حجم opts);
	opts.update = 1;
	opts.merge = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_merge;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	init_checkout_metadata(&opts.meta, head, &oid, عدم);

	دەرەخ = repo_parse_دەرەخ_indirect(the_repository, &oid);
	إذا (!دەرەخ)
		die(_("unable to parse commit %s"), oid_to_hex(&oid));
	إذا (repo_parse_دەرەخ(the_repository, دەرەخ) < 0)
		exit(128);
	init_دەرەخ_desc(&t, &دەرەخ->object.oid, دەرەخ->buffer, دەرەخ->size);
	إذا (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	حرر(head);

	إذا (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.args, oid_to_hex(null_oid(the_hash_algo)),
		     oid_to_hex(&oid), "1", عدم);
	err |= run_hooks_opt(the_repository, "post-checkout", &hook_opt);

	إذا (!err && (option_recurse_submodules.nr > 0)) {
		بنية child_process cmd = CHILD_PROCESS_INIT;
		strvec_pushl(&cmd.args, "submodule", "update", "--require-init",
			     "--recursive", عدم);

		إذا (option_shallow_submodules == 1)
			strvec_ئىتتىرىش(&cmd.args, "--depth=1");

		إذا (max_jobs != -1)
			strvec_pushf(&cmd.args, "--jobs=%d", max_jobs);

		إذا (submodule_progress)
			strvec_ئىتتىرىش(&cmd.args, "--progress");

		إذا (option_verbosity < 0)
			strvec_ئىتتىرىش(&cmd.args, "--quiet");

		إذا (option_remote_submodules) {
			strvec_ئىتتىرىش(&cmd.args, "--remote");
			strvec_ئىتتىرىش(&cmd.args, "--no-fetch");
		}

		إذا (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(ref_storage_format));

		إذا (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.args, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		إذا (option_single_branch >= 0)
			strvec_ئىتتىرىش(&cmd.args, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = run_command(&cmd);
	}

	أرجع err;
}

ساكن صحيح git_clone_config(ثابت حرف *k, ثابت حرف *v,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(k, "clone.defaultremotename")) {
		إذا (!v)
			أرجع config_error_nonbool(k);
		حرر(remote_name);
		remote_name = xstrdup(v);
	}
	إذا (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	إذا (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	أرجع git_default_config(k, v, ctx, cb);
}

ساكن صحيح write_one_config(ثابت حرف *ئاچقۇچ, ثابت حرف *قىممەت,
			    ثابت بنية config_context *ctx,
			    فراغ *data)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	صحيح apply_failed = git_clone_config(ئاچقۇچ, قىممەت, ctx, data);
	إذا (apply_failed)
		أرجع apply_failed;

	أرجع repo_config_set_multivar_gently(the_repository, ئاچقۇچ,
					       قىممەت ? قىممەت : "true",
					       CONFIG_REGEX_NONE, 0);
}

ساكن فراغ write_config(بنية string_list *config)
{
	صحيح i;

	لكل (i = 0; i < config->nr; i++) {
		إذا (git_config_parse_parameter(config->بۇيۇملار[i].string,
					       write_one_config, عدم) < 0)
			die(_("unable to write parameters to config file"));
	}
}

ساكن فراغ write_refspec_config(ثابت حرف *src_ref_prefix,
		ثابت بنية ref *our_head_points_at,
		ثابت بنية ref *remote_head_points_at,
		بنية strbuf *branch_چوققا)
{
	بنية strbuf ئاچقۇچ = STRBUF_INIT;
	بنية strbuf قىممەت = STRBUF_INIT;

	إذا (option_mirror || !option_bare) {
		إذا (option_single_branch && !option_mirror) {
			إذا (option_branch) {
				إذا (starts_with(our_head_points_at->name, "refs/tags/"))
					strbuf_addf(&قىممەت, "+%s:%s", our_head_points_at->name,
						our_head_points_at->name);
				وإلا
					strbuf_addf(&قىممەت, "+%s:%s%s", our_head_points_at->name,
						branch_چوققا->buf, option_branch);
			} وإلا إذا (remote_head_points_at) {
				ثابت حرف *head = remote_head_points_at->name;
				إذا (!skip_prefix(head, "refs/heads/", &head))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&قىممەت, "+%s:%s%s", remote_head_points_at->name,
						branch_چوققا->buf, head);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} وإلا {
			strbuf_addf(&قىممەت, "+%s*:%s*", src_ref_prefix, branch_چوققا->buf);
		}
		/* Configure the remote */
		إذا (قىممەت.len) {
			strbuf_addf(&ئاچقۇچ, "remote.%s.fetch", remote_name);
			repo_config_set_multivar(the_repository, ئاچقۇچ.buf, قىممەت.buf, "^$", 0);
			strbuf_reset(&ئاچقۇچ);

			إذا (option_mirror) {
				strbuf_addf(&ئاچقۇچ, "remote.%s.mirror", remote_name);
				repo_config_set(the_repository, ئاچقۇچ.buf, "true");
				strbuf_reset(&ئاچقۇچ);
			}
		}
	}

	strbuf_release(&ئاچقۇچ);
	strbuf_release(&قىممەت);
}

ساكن فراغ dissociate_from_references(فراغ)
{
	حرف *alternates = repo_git_path(the_repository, "objects/info/alternates");

	إذا (!access(alternates, F_OK)) {
		بنية child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.args, "repack", "-a", "-d", عدم);
		إذا (run_command(&cmd))
			die(_("cannot repack to clean up"));
		إذا (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	حرر(alternates);
}

ساكن صحيح path_exists(ثابت حرف *path)
{
	بنية stat sb;
	أرجع !stat(path, &sb);
}

صحيح cmd_clone(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *prefix,
	      بنية repository *repository UNUSED)
{
	صحيح is_bundle = 0, is_local;
	صحيح reject_shallow = 0;
	ثابت حرف *repo_name, *repo, *work_دەرەخ, *git_dir;
	حرف *repo_to_free = عدم;
	حرف *path = عدم, *dir, *display_repo = عدم;
	صحيح dest_exists, real_dest_exists = 0;
	ثابت بنية ref *refs, *remote_head;
	بنية ref *remote_head_points_at = عدم;
	ثابت بنية ref *our_head_points_at;
	حرف *unborn_head = عدم;
	بنية ref *mapped_refs = عدم;
	ثابت بنية ref *ref;
	بنية strbuf ئاچقۇچ = STRBUF_INIT;
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf branch_چوققا = STRBUF_INIT, reflog_ئۇچۇر = STRBUF_INIT;
	بنية transport *transport = عدم;
	ثابت حرف *src_ref_prefix = "refs/heads/";
	بنية remote *remote;
	صحيح err = 0, complete_refs_before_fetch = 1;
	صحيح submodule_progress;
	صحيح filter_submodules = 0;
	صحيح hash_algo;
	تعداد ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	ثابت صحيح do_not_override_repo_unix_permissions = -1;
	صحيح option_reject_shallow = -1; /* unspecified */
	صحيح deepen = 0;
	حرف *option_template = عدم, *option_depth = عدم, *option_since = عدم;
	حرف *option_origin = عدم;
	بنية string_list option_not = STRING_LIST_INIT_NODUP;
	ثابت حرف *real_git_dir = عدم;
	ثابت حرف *ref_storage_format_uri = عدم;
	ثابت حرف *option_upload_pack = "git-upload-pack";
	صحيح option_progress = -1;
	صحيح option_sparse_checkout = 0;
	تعداد transport_family family = TRANSPORT_FAMILY_ALL;
	بنية string_list option_config = STRING_LIST_INIT_DUP;
	صحيح option_dissociate = 0;
	بنية list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	صحيح option_filter_submodules = -1; /* unspecified */
	بنية string_list server_options = STRING_LIST_INIT_NODUP;
	ثابت حرف *bundle_uri = عدم;
	حرف *option_rev = عدم;

	بنية clone_opts opts = CLONE_OPTS_INIT;

	بنية transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	بنية option builtin_clone_options[] = {
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
			.قىممەت = &option_recurse_submodules,
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
	بنية strvec alternates = STRVEC_INIT;

	ثابت حرف * ثابت builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		عدم
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_identity("clone");

	repo_config(the_repository, git_clone_config, عدم);

	argc = parse_options(argc, argv, prefix, builtin_clone_options,
			     builtin_clone_usage, 0);

	إذا (argc > 2)
		usage_ئۇچۇر_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	إذا (argc == 0)
		usage_ئۇچۇر_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	إذا (option_depth || option_since || option_not.nr)
		deepen = 1;
	إذا (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	إذا (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, عدم);
		إذا (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	إذا (option_mirror) {
		option_bare = 1;
		option_tags = 0;
	}

	إذا (option_bare) {
		إذا (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	إذا (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	path = get_repo_path(repo_name, &is_bundle);
	إذا (path) {
		FREE_AND_NULL(path);
		repo = repo_to_free = absolute_pathdup(repo_name);
	} وإلا إذا (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} وإلا
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	إذا (option_depth && atoi(option_depth) < 1)
		die(_("depth %s is not a positive number"), option_depth);

	إذا (argc == 2)
		dir = xstrdup(argv[1]);
	وإلا
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = path_exists(dir);
	إذا (dest_exists && !is_empty_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	إذا (real_git_dir) {
		real_dest_exists = path_exists(real_git_dir);
		إذا (real_dest_exists && !is_empty_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_ئۇچۇر, "clone: from %s",
		    display_repo ? display_repo : repo);
	حرر(display_repo);

	إذا (option_bare)
		work_دەرەخ = عدم;
	وإلا {
		work_دەرەخ = getenv("GIT_WORK_TREE");
		إذا (work_دەرەخ && path_exists(work_دەرەخ))
			die(_("working tree '%s' already exists."), work_دەرەخ);
	}

	إذا (option_bare || work_دەرەخ)
		git_dir = xstrdup(dir);
	وإلا {
		work_دەرەخ = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_ئىتتىرىش_common(remove_junk_on_signal);

	إذا (!option_bare) {
		إذا (safe_create_leading_directories_const(the_repository, work_دەرەخ) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_دەرەخ);
		إذا (dest_exists)
			junk_work_دەرەخ_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		وإلا إذا (mkdir(work_دەرەخ, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_دەرەخ);
		junk_work_دەرەخ = work_دەرەخ;
	}

	إذا (real_git_dir) {
		إذا (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} وإلا {
		إذا (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	إذا (safe_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	إذا (0 <= option_verbosity) {
		إذا (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		وإلا
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	إذا (option_recurse_submodules.nr > 0) {
		بنية string_list_item *item;
		بنية strbuf sb = STRBUF_INIT;
		صحيح val;

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

		إذا (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		إذا (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		وإلا إذا (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} وإلا إذا (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_دەرەخ,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, عدم);

	إذا (real_git_dir) {
		حرر((حرف *)git_dir);
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

	refs_create_refdir_stubs(the_repository, git_dir, عدم);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, عدم);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	إذا (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	إذا (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	إذا (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	إذا (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	إذا (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	إذا (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	إذا (option_origin) {
		حرر(remote_name);
		remote_name = xstrdup(option_origin);
	}

	إذا (!remote_name)
		remote_name = xstrdup("origin");

	إذا (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	إذا (option_bare) {
		إذا (option_mirror)
			src_ref_prefix = "refs/";
		strbuf_addstr(&branch_چوققا, src_ref_prefix);

		repo_config_set(the_repository, "core.bare", "true");
	} وإلا إذا (!option_rev) {
		strbuf_addf(&branch_چوققا, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&ئاچقۇچ, "remote.%s.url", remote_name);
	repo_config_set(the_repository, ئاچقۇچ.buf, repo);
	strbuf_reset(&ئاچقۇچ);

	إذا (!option_tags) {
		strbuf_addf(&ئاچقۇچ, "remote.%s.tagOpt", remote_name);
		repo_config_set(the_repository, ئاچقۇچ.buf, "--no-tags");
		strbuf_reset(&ئاچقۇچ);
	}

	remote = remote_get_early(remote_name);

	إذا (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_prefix,
				branch_چوققا.buf);

	path = get_repo_path(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && path && !is_bundle;
	إذا (is_local) {
		إذا (option_depth)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		إذا (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		إذا (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		إذا (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		إذا (!access(mkpath("%s/shallow", path), F_OK)) {
			إذا (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			إذا (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	إذا (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? path : عدم);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, path ? path : remote->url.v[0]);
	transport_set_verbosity(transport, option_verbosity, option_progress);
	transport->family = family;
	transport->cloning = 1;

	إذا (is_bundle) {
		بنية bundle_header header = BUNDLE_HEADER_INIT;
		صحيح fd = read_bundle_header(path, &header);
		صحيح has_filter = header.filter.choice != LOFC_DISABLED;

		إذا (fd > 0)
			close(fd);
		bundle_header_release(&header);
		إذا (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_set_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	إذا (reject_shallow)
		transport_set_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	إذا (option_depth)
		transport_set_option(transport, TRANS_OPT_DEPTH,
				     option_depth);
	إذا (option_since)
		transport_set_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	إذا (option_not.nr)
		transport_set_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (ثابت حرف *)&option_not);
	إذا (option_single_branch) {
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		إذا (option_branch)
			opts.wants_head = 0;
	}

	إذا (option_upload_pack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	إذا (server_options.nr)
		transport->server_options = &server_options;

	إذا (filter_options.choice) {
		ثابت حرف *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	إذا (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	إذا (option_rev) {
		option_tags = 0;
		option_single_branch = 0;
		opts.wants_head = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	إذا (option_tags || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, TAG_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	إذا (option_branch)
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
	إذا (opts.wants_head || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_ئىتتىرىش(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_version(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_set_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, عدم, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	إذا (bundle_uri) {
		بنية remote_state *state;
		صحيح has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = عدم;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		إذا (repo_init(the_repository, git_dir, work_دەرەخ))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		وإلا إذا (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		وإلا إذا (has_heuristic)
			repo_config_set_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		حرر(the_repository->remote_state);
		the_repository->remote_state = state;
	} وإلا {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		إذا (transport->bundles &&
		    hashmap_get_size(&transport->bundles->bundles)) {
			بنية remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = عدم;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			إذا (repo_init(the_repository, git_dir, work_دەرەخ))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			وإلا إذا (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			حرر(the_repository->remote_state);
			the_repository->remote_state = state;
		} وإلا {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	إذا (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	إذا (mapped_refs) {
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
		لكل (ref = refs; ref; ref = ref->next)
			إذا (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				اكسر;
			}

		إذا (!is_local && !complete_refs_before_fetch) {
			إذا (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_head = find_ref_by_name(refs, "HEAD");
	remote_head_points_at = guess_remote_head(remote_head, mapped_refs,
						  REMOTE_GUESS_HEAD_QUIET);

	إذا (option_branch) {
		our_head_points_at = find_remote_branch(mapped_refs, option_branch);
		إذا (!our_head_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} وإلا إذا (option_rev) {
		our_head_points_at = mapped_refs;
		إذا (!our_head_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} وإلا إذا (remote_head_points_at) {
		our_head_points_at = remote_head_points_at;
	} وإلا إذا (remote_head) {
		our_head_points_at = عدم;
	} وإلا {
		حرف *to_free = عدم;
		ثابت حرف *branch;

		إذا (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		إذا (transport_ls_refs_options.unborn_head_target &&
		    skip_prefix(transport_ls_refs_options.unborn_head_target,
				"refs/heads/", &branch)) {
			unborn_head  = xstrdup(transport_ls_refs_options.unborn_head_target);
		} وإلا {
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

		حرر(to_free);
	}

	إذا (!option_rev)
		write_refspec_config(src_ref_prefix, our_head_points_at,
				     remote_head_points_at, &branch_چوققا);

	إذا (filter_options.choice)
		partial_clone_register(remote_name, &filter_options);

	إذا (is_local)
		clone_local(path, git_dir);
	وإلا إذا (mapped_refs && complete_refs_before_fetch) {
		إذا (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_head_points_at,
			   branch_چوققا.buf, reflog_ئۇچۇر.buf, transport,
			   !is_local);

	update_head(&opts, our_head_points_at, remote_head, unborn_head, reflog_ئۇچۇر.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_progress = transport->progress;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	إذا (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	إذا (option_sparse_checkout && git_sparse_checkout_init(dir))
		أرجع 1;

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

	حرر(remote_name);
	strbuf_release(&reflog_ئۇچۇر);
	strbuf_release(&branch_چوققا);
	strbuf_release(&buf);
	strbuf_release(&ئاچقۇچ);
	free_refs(mapped_refs);
	free_refs(remote_head_points_at);
	حرر(unborn_head);
	حرر(dir);
	حرر(path);
	حرر(repo_to_free);
	junk_mode = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	أرجع err;
}
