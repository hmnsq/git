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

#अन्तर्भाव "builtin.h"

#अन्तर्भाव "abspath.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "copy.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "refspec.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "transport.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "dir-iterator.h"
#अन्तर्भाव "iterator.h"
#अन्तर्भाव "sigchain.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "connected.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "pkt-line.h"
#अन्तर्भाव "list-objects-filter-options.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "bundle.h"
#अन्तर्भाव "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

संरचना clone_opts {
	पूर्णाङ्क wants_head;
	पूर्णाङ्क detach;
};
#define CLONE_OPTS_INIT { \
	.wants_head = 1 /* default enabled */ \
}

स्थिर पूर्णाङ्क option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
स्थिर पूर्णाङ्क option_local = -1, option_no_hardlinks, option_shared;
स्थिर पूर्णाङ्क option_tags = 1; /* default enabled */
स्थिर पूर्णाङ्क option_shallow_submodules;
स्थिर पूर्णाङ्क config_reject_shallow = -1;    /* unspecified */
स्थिर वर्ण *remote_name = रिक्त;
स्थिर वर्ण *option_branch = रिक्त;
स्थिर पूर्णाङ्क option_verbosity;
स्थिर संरचना string_list option_required_reference = STRING_LIST_INIT_NODUP;
स्थिर संरचना string_list option_optional_reference = STRING_LIST_INIT_NODUP;
स्थिर पूर्णाङ्क max_jobs = -1;
स्थिर संरचना string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
स्थिर पूर्णाङ्क config_filter_submodules = -1;    /* unspecified */
स्थिर पूर्णाङ्क option_remote_submodules;

स्थिर पूर्णाङ्क recurse_submodules_cb(ध्रुव संरचना option *opt,
				 ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	सचे (unset)
		string_list_clear((संरचना string_list *)opt->मानम्, 0);
	अञ्ञथा सचे (arg)
		string_list_append((संरचना string_list *)opt->मानम्, arg);
	अञ्ञथा
		string_list_append((संरचना string_list *)opt->मानम्,
				   (ध्रुव वर्ण *)opt->defval);

	पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण *get_repo_path_1(संरचना strbuf *path, पूर्णाङ्क *is_bundle)
{
	स्थिर ध्रुव वर्ण *suffix[] = { "/.git", "", ".git/.git", ".git" };
	स्थिर ध्रुव वर्ण *bundle_suffix[] = { ".bundle", "" };
	size_t baselen = path->len;
	संरचना stat st;
	पूर्णाङ्क i;

	अत्थाय (i = 0; i < ARRAY_SIZE(suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, suffix[i]);
		सचे (stat(path->buf, &st))
			अनुवर्त;
		सचे (S_ISDIR(st.st_mode) && is_git_directory(path->buf)) {
			*is_bundle = 0;
			पटिनिवत्त path->buf;
		} अञ्ञथा सचे (S_ISREG(st.st_mode) && st.st_size > 8) {
			/* Is it a "gitfile"? */
			वर्ण signature[8];
			ध्रुव वर्ण *dst;
			पूर्णाङ्क len, fd = open(path->buf, O_RDONLY);
			सचे (fd < 0)
				अनुवर्त;
			len = read_in_full(fd, signature, 8);
			close(fd);
			सचे (len != 8 || strncmp(signature, "gitdir: ", 8))
				अनुवर्त;
			dst = read_gitfile(path->buf);
			सचे (dst) {
				*is_bundle = 0;
				पटिनिवत्त dst;
			}
		}
	}

	अत्थाय (i = 0; i < ARRAY_SIZE(bundle_suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, bundle_suffix[i]);
		सचे (!stat(path->buf, &st) && S_ISREG(st.st_mode)) {
			*is_bundle = 1;
			पटिनिवत्त path->buf;
		}
	}

	पटिनिवत्त रिक्त;
}

स्थिर वर्ण *get_repo_path(ध्रुव वर्ण *repo, पूर्णाङ्क *is_bundle)
{
	संरचना strbuf path = STRBUF_INIT;
	ध्रुव वर्ण *raw;
	वर्ण *canon;

	strbuf_addstr(&path, repo);
	raw = get_repo_path_1(&path, is_bundle);
	canon = raw ? absolute_pathdup(raw) : रिक्त;
	strbuf_release(&path);
	पटिनिवत्त canon;
}

संरचना collect_alternates_data {
	संरचना strvec *alternates;
	bool required;
};

स्थिर पूर्णाङ्क collect_one_alternate(संरचना string_list_item *item, शून्य *cb_data)
{
	संरचना collect_alternates_data *data = cb_data;
	संरचना strbuf err = STRBUF_INIT;
	वर्ण *ref_git = compute_alternate_path(item->string, &err);

	सचे (!ref_git) {
		सचे (data->required)
			die("%s", err.buf);
		अञ्ञथा
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				item->string, err.buf);
	} अञ्ञथा {
		strvec_pushf(data->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	मुक्त(ref_git);
	पटिनिवत्त 0;
}

स्थिर शून्य read_alternates(संरचना strvec *alternates, ध्रुव वर्ण *src_repo)
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
	संरचना strbuf path = STRBUF_INIT;
	संरचना strbuf पङ्क्तिः = STRBUF_INIT;

	strbuf_addf(&path, "%s/objects/info/alternates", src_repo);

	in = fopen(path.buf, "r");
	सचे (!in) {
		सचे (errno == ENOENT)
			गच्छ out;
		die_errno("could not read alternates file '%s'", path.buf);
	}

	याव (strbuf_getline(&पङ्क्तिः, in) != EOF) {
		वर्ण *abs_path;
		सचे (!पङ्क्तिः.len || पङ्क्तिः.buf[0] == '#')
			अनुवर्त;
		सचे (is_absolute_path(पङ्क्तिः.buf)) {
			strvec_प्रक्षिप(alternates, पङ्क्तिः.buf);
			अनुवर्त;
		}
		abs_path = mkpathdup("%s/objects/%s", src_repo, पङ्क्तिः.buf);
		सचे (!normalize_path_copy(abs_path, abs_path))
			strvec_प्रक्षिप(alternates, abs_path);
		अञ्ञथा
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, पङ्क्तिः.buf);
		मुक्त(abs_path);
	}

out:
	strbuf_release(&path);
	strbuf_release(&पङ्क्तिः);
	सचे (in)
		fclose(in);
}

स्थिर शून्य collect_alternates(संरचना strvec *alternates,
			       ध्रुव वर्ण *local_source_repo)
{
	सचे (option_required_reference.nr || option_optional_reference.nr) {
		संरचना collect_alternates_data data = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &data);
		data.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &data);
	}

	सचे (local_source_repo) {
		संरचना strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_source_repo);
		सचे (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		अञ्ञथा
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

स्थिर शून्य mkdir_if_missing(ध्रुव वर्ण *pathname, mode_t mode)
{
	संरचना stat st;

	सचे (!mkdir(pathname, mode))
		पटिनिवत्त;

	सचे (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	अञ्ञथा सचे (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	अञ्ञथा सचे (!S_ISDIR(st.st_mode))
		die(_("%s exists and is not a directory"), pathname);
}

स्थिर शून्य copy_or_link_directory(संरचना strbuf *src, संरचना strbuf *dest,
				   ध्रुव वर्ण *src_repo)
{
	पूर्णाङ्क src_len, dest_len;
	संरचना dir_iterator *iter;
	पूर्णाङ्क iter_status;

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
	die_upon_dubious_ownership(रिक्त, रिक्त, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	सचे (!iter) {
		सचे (errno == ENOTDIR) {
			पूर्णाङ्क saved_errno = errno;
			संरचना stat st;

			सचे (!lstat(src->buf, &st) && S_ISLNK(st.st_mode))
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

	याव ((iter_status = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_path);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_path);

		सचे (S_ISLNK(iter->st.st_mode))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_path);

		सचे (S_ISDIR(iter->st.st_mode)) {
			mkdir_if_missing(dest->buf, 0777);
			अनुवर्त;
		}

		/* Alternates were already handled earlier. */
		सचे (!fspathcmp(iter->relative_path, "info/alternates"))
			अनुवर्त;

		सचे (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		सचे (!option_no_hardlinks) {
			सचे (!link(src->buf, dest->buf)) {
				संरचना stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				सचे (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				सचे (st.st_mode != iter->st.st_mode ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_size != iter->st.st_size ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				अनुवर्त;
			}
			सचे (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		सचे (copy_file_with_time(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	सचे (iter_status != ITER_DONE) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

स्थिर शून्य clone_local(ध्रुव वर्ण *src_repo, ध्रुव वर्ण *dest_repo)
{
	सचे (!option_shared) {
		संरचना strbuf src = STRBUF_INIT;
		संरचना strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	सचे (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

स्थिर ध्रुव वर्ण *junk_work_वृक्षः;
स्थिर पूर्णाङ्क junk_work_वृक्षः_flags;
स्थिर ध्रुव वर्ण *junk_git_dir;
स्थिर पूर्णाङ्क junk_git_dir_flags;
स्थिर गणना {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_mode = JUNK_LEAVE_NONE;

स्थिर ध्रुव वर्ण junk_leave_repo_सन्देशः[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

स्थिर शून्य remove_junk(शून्य)
{
	संरचना strbuf sb = STRBUF_INIT;

	विकल्प (junk_mode) {
	पक्ष JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_सन्देशः));
		/* fall-through */
	पक्ष JUNK_LEAVE_ALL:
		पटिनिवत्त;
	स्वभाव:
		/* proceed to removal */
		भङ्ग;
	}

	सचे (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	सचे (junk_work_वृक्षः) {
		strbuf_addstr(&sb, junk_work_वृक्षः);
		remove_dir_recursively(&sb, junk_work_वृक्षः_flags);
	}
	strbuf_release(&sb);
}

स्थिर शून्य remove_junk_on_signal(पूर्णाङ्क signo)
{
	remove_junk();
	sigchain_निष्कासय(signo);
	उत्थापय(signo);
}

स्थिर संरचना ref *find_remote_branch(ध्रुव संरचना ref *refs, ध्रुव वर्ण *branch)
{
	संरचना ref *ref;
	संरचना strbuf head = STRBUF_INIT;
	strbuf_addstr(&head, "refs/heads/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	सचे (ref)
		पटिनिवत्त ref;

	strbuf_addstr(&head, "refs/tags/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	पटिनिवत्त ref;
}

स्थिर संरचना ref *wanted_peer_refs(संरचना clone_opts *opts,
				    ध्रुव संरचना ref *refs,
				    संरचना refspec *refspec)
{
	संरचना ref *local_refs = रिक्त;
	संरचना ref **tail = &local_refs;
	संरचना ref *to_free = रिक्त;

	सचे (opts->wants_head) {
		संरचना ref *head = copy_ref(find_ref_by_name(refs, "HEAD"));
		सचे (head)
			tail_link_ref(head, &tail);
		सचे (option_single_branch)
			refs = to_free =
				guess_remote_head(head, refs,
						  REMOTE_GUESS_HEAD_QUIET);
	} अञ्ञथा सचे (option_single_branch) {
		local_refs = रिक्त;
		tail = &local_refs;
		refs = to_free = copy_ref(find_remote_branch(refs, option_branch));
	}

	अत्थाय (size_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->वस्तूनि[i], &tail, 0);

	free_one_ref(to_free);

	पटिनिवत्त local_refs;
}

स्थिर शून्य write_remote_refs(ध्रुव संरचना ref *local_refs)
{
	ध्रुव संरचना ref *r;

	संरचना ref_transaction *t;
	संरचना strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	सचे (!t)
		die("%s", err.buf);

	अत्थाय (r = local_refs; r; r = r->next) {
		सचे (!r->peer_ref)
			अनुवर्त;
		सचे (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   रिक्त, 0, रिक्त, &err))
			die("%s", err.buf);
	}

	सचे (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

स्थिर शून्य write_followtags(ध्रुव संरचना ref *refs, ध्रुव वर्ण *सन्देशः)
{
	ध्रुव संरचना ref *ref;
	अत्थाय (ref = refs; ref; ref = ref->next) {
		सचे (!starts_with(ref->name, "refs/tags/"))
			अनुवर्त;
		सचे (ends_with(ref->name, "^{}"))
			अनुवर्त;
		सचे (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			अनुवर्त;
		refs_update_ref(get_main_ref_store(the_repository), सन्देशः,
				ref->name, &ref->old_oid, रिक्त, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

स्थिर ध्रुव संरचना object_id *iterate_ref_map(शून्य *cb_data)
{
	संरचना ref **rm = cb_data;
	संरचना ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	याव (ref && !ref->peer_ref)
		ref = ref->next;
	सचे (!ref)
		पटिनिवत्त रिक्त;

	*rm = ref->next;
	पटिनिवत्त &ref->old_oid;
}

स्थिर शून्य update_remote_refs(ध्रुव संरचना ref *refs,
			       ध्रुव संरचना ref *mapped_refs,
			       ध्रुव संरचना ref *remote_head_points_at,
			       ध्रुव वर्ण *branch_शिखरम्,
			       ध्रुव वर्ण *सन्देशः,
			       संरचना transport *transport,
			       पूर्णाङ्क check_connectivity)
{
	ध्रुव संरचना ref *rm = mapped_refs;

	सचे (check_connectivity) {
		संरचना check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.progress = transport->progress;

		सचे (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	सचे (refs) {
		write_remote_refs(mapped_refs);
		सचे (option_single_branch && option_tags)
			write_followtags(refs, सन्देशः);
	}

	सचे (remote_head_points_at && remote_head_points_at->peer_ref && !option_bare) {
		संरचना strbuf head_ref = STRBUF_INIT;
		strbuf_addstr(&head_ref, branch_शिखरम्);
		strbuf_addstr(&head_ref, "HEAD");
		सचे (refs_update_symref(get_main_ref_store(the_repository), head_ref.buf,
				       remote_head_points_at->peer_ref->name,
				       सन्देशः) < 0)
			die(_("unable to update %s"), head_ref.buf);
		strbuf_release(&head_ref);
	}
}

स्थिर शून्य update_head(संरचना clone_opts *opts, ध्रुव संरचना ref *our, ध्रुव संरचना ref *remote,
			ध्रुव वर्ण *unborn, ध्रुव वर्ण *सन्देशः)
{
	ध्रुव वर्ण *head;
	सचे (our && !opts->detach && skip_prefix(our->name, "refs/heads/", &head)) {
		/* Local default branch link */
		सचे (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, रिक्त) < 0)
			die(_("unable to update HEAD"));
		सचे (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					सन्देशः, "HEAD", &our->old_oid, रिक्त, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, head, remote_name, our->name);
		}
	} अञ्ञथा सचे (our) {
		संरचना commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), सन्देशः,
				"HEAD", &c->object.oid, रिक्त, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} अञ्ञथा सचे (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), सन्देशः,
				"HEAD", &remote->old_oid, रिक्त, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} अञ्ञथा सचे (unborn && skip_prefix(unborn, "refs/heads/", &head)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		सचे (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, रिक्त) < 0)
			die(_("unable to update HEAD"));
		सचे (!option_bare)
			install_branch_config(0, head, remote_name, unborn);
	}
}

स्थिर पूर्णाङ्क git_sparse_checkout_init(ध्रुव वर्ण *repo)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	पूर्णाङ्क result = 0;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	strvec_pushl(&cmd.args, "-C", repo, "sparse-checkout", "set", रिक्त);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	सचे (run_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		result = 1;
	}

	पटिनिवत्त result;
}

स्थिर पूर्णाङ्क checkout(पूर्णाङ्क submodule_progress,
		    संरचना list_objects_filter_options *filter_options,
		    पूर्णाङ्क filter_submodules,
		    गणना ref_storage_format ref_storage_format)
{
	संरचना object_id oid;
	वर्ण *head;
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना unpack_trees_options opts;
	संरचना वृक्षः *वृक्षः;
	संरचना वृक्षः_desc t;
	पूर्णाङ्क err = 0;
	संरचना run_hooks_opt hook_opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	सचे (option_no_checkout)
		पटिनिवत्त 0;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, रिक्त);
	सचे (!head) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		पटिनिवत्त 0;
	}
	सचे (!strcmp(head, "HEAD")) {
		सचे (advice_enabled(ADVICE_DETACHED_HEAD))
			detach_advice(oid_to_hex(&oid));
		FREE_AND_NULL(head);
	} अञ्ञथा {
		सचे (!starts_with(head, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	setup_work_वृक्षः(the_repository);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, परिमाण opts);
	opts.update = 1;
	opts.merge = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_merge;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	init_checkout_metadata(&opts.meta, head, &oid, रिक्त);

	वृक्षः = repo_parse_वृक्षः_indirect(the_repository, &oid);
	सचे (!वृक्षः)
		die(_("unable to parse commit %s"), oid_to_hex(&oid));
	सचे (repo_parse_वृक्षः(the_repository, वृक्षः) < 0)
		exit(128);
	init_वृक्षः_desc(&t, &वृक्षः->object.oid, वृक्षः->buffer, वृक्षः->size);
	सचे (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	मुक्त(head);

	सचे (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.args, oid_to_hex(null_oid(the_hash_algo)),
		     oid_to_hex(&oid), "1", रिक्त);
	err |= run_hooks_opt(the_repository, "post-checkout", &hook_opt);

	सचे (!err && (option_recurse_submodules.nr > 0)) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;
		strvec_pushl(&cmd.args, "submodule", "update", "--require-init",
			     "--recursive", रिक्त);

		सचे (option_shallow_submodules == 1)
			strvec_प्रक्षिप(&cmd.args, "--depth=1");

		सचे (max_jobs != -1)
			strvec_pushf(&cmd.args, "--jobs=%d", max_jobs);

		सचे (submodule_progress)
			strvec_प्रक्षिप(&cmd.args, "--progress");

		सचे (option_verbosity < 0)
			strvec_प्रक्षिप(&cmd.args, "--quiet");

		सचे (option_remote_submodules) {
			strvec_प्रक्षिप(&cmd.args, "--remote");
			strvec_प्रक्षिप(&cmd.args, "--no-fetch");
		}

		सचे (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(ref_storage_format));

		सचे (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.args, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		सचे (option_single_branch >= 0)
			strvec_प्रक्षिप(&cmd.args, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = run_command(&cmd);
	}

	पटिनिवत्त err;
}

स्थिर पूर्णाङ्क git_clone_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
			    ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	सचे (!strcmp(k, "clone.defaultremotename")) {
		सचे (!v)
			पटिनिवत्त config_error_nonbool(k);
		मुक्त(remote_name);
		remote_name = xstrdup(v);
	}
	सचे (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	सचे (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	पटिनिवत्त git_default_config(k, v, ctx, cb);
}

स्थिर पूर्णाङ्क write_one_config(ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *मानम्,
			    ध्रुव संरचना config_context *ctx,
			    शून्य *data)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	पूर्णाङ्क apply_failed = git_clone_config(कुञ्चिका, मानम्, ctx, data);
	सचे (apply_failed)
		पटिनिवत्त apply_failed;

	पटिनिवत्त repo_config_set_multivar_gently(the_repository, कुञ्चिका,
					       मानम् ? मानम् : "true",
					       CONFIG_REGEX_NONE, 0);
}

स्थिर शून्य write_config(संरचना string_list *config)
{
	पूर्णाङ्क i;

	अत्थाय (i = 0; i < config->nr; i++) {
		सचे (git_config_parse_parameter(config->वस्तूनि[i].string,
					       write_one_config, रिक्त) < 0)
			die(_("unable to write parameters to config file"));
	}
}

स्थिर शून्य write_refspec_config(ध्रुव वर्ण *src_ref_prefix,
		ध्रुव संरचना ref *our_head_points_at,
		ध्रुव संरचना ref *remote_head_points_at,
		संरचना strbuf *branch_शिखरम्)
{
	संरचना strbuf कुञ्चिका = STRBUF_INIT;
	संरचना strbuf मानम् = STRBUF_INIT;

	सचे (option_mirror || !option_bare) {
		सचे (option_single_branch && !option_mirror) {
			सचे (option_branch) {
				सचे (starts_with(our_head_points_at->name, "refs/tags/"))
					strbuf_addf(&मानम्, "+%s:%s", our_head_points_at->name,
						our_head_points_at->name);
				अञ्ञथा
					strbuf_addf(&मानम्, "+%s:%s%s", our_head_points_at->name,
						branch_शिखरम्->buf, option_branch);
			} अञ्ञथा सचे (remote_head_points_at) {
				ध्रुव वर्ण *head = remote_head_points_at->name;
				सचे (!skip_prefix(head, "refs/heads/", &head))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&मानम्, "+%s:%s%s", remote_head_points_at->name,
						branch_शिखरम्->buf, head);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} अञ्ञथा {
			strbuf_addf(&मानम्, "+%s*:%s*", src_ref_prefix, branch_शिखरम्->buf);
		}
		/* Configure the remote */
		सचे (मानम्.len) {
			strbuf_addf(&कुञ्चिका, "remote.%s.fetch", remote_name);
			repo_config_set_multivar(the_repository, कुञ्चिका.buf, मानम्.buf, "^$", 0);
			strbuf_reset(&कुञ्चिका);

			सचे (option_mirror) {
				strbuf_addf(&कुञ्चिका, "remote.%s.mirror", remote_name);
				repo_config_set(the_repository, कुञ्चिका.buf, "true");
				strbuf_reset(&कुञ्चिका);
			}
		}
	}

	strbuf_release(&कुञ्चिका);
	strbuf_release(&मानम्);
}

स्थिर शून्य dissociate_from_references(शून्य)
{
	वर्ण *alternates = repo_git_path(the_repository, "objects/info/alternates");

	सचे (!access(alternates, F_OK)) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.args, "repack", "-a", "-d", रिक्त);
		सचे (run_command(&cmd))
			die(_("cannot repack to clean up"));
		सचे (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	मुक्त(alternates);
}

स्थिर पूर्णाङ्क path_exists(ध्रुव वर्ण *path)
{
	संरचना stat sb;
	पटिनिवत्त !stat(path, &sb);
}

पूर्णाङ्क cmd_clone(पूर्णाङ्क argc,
	      ध्रुव वर्ण **argv,
	      ध्रुव वर्ण *prefix,
	      संरचना repository *repository UNUSED)
{
	पूर्णाङ्क is_bundle = 0, is_local;
	पूर्णाङ्क reject_shallow = 0;
	ध्रुव वर्ण *repo_name, *repo, *work_वृक्षः, *git_dir;
	वर्ण *repo_to_free = रिक्त;
	वर्ण *path = रिक्त, *dir, *display_repo = रिक्त;
	पूर्णाङ्क dest_exists, real_dest_exists = 0;
	ध्रुव संरचना ref *refs, *remote_head;
	संरचना ref *remote_head_points_at = रिक्त;
	ध्रुव संरचना ref *our_head_points_at;
	वर्ण *unborn_head = रिक्त;
	संरचना ref *mapped_refs = रिक्त;
	ध्रुव संरचना ref *ref;
	संरचना strbuf कुञ्चिका = STRBUF_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf branch_शिखरम् = STRBUF_INIT, reflog_सन्देशः = STRBUF_INIT;
	संरचना transport *transport = रिक्त;
	ध्रुव वर्ण *src_ref_prefix = "refs/heads/";
	संरचना remote *remote;
	पूर्णाङ्क err = 0, complete_refs_before_fetch = 1;
	पूर्णाङ्क submodule_progress;
	पूर्णाङ्क filter_submodules = 0;
	पूर्णाङ्क hash_algo;
	गणना ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	ध्रुव पूर्णाङ्क do_not_override_repo_unix_permissions = -1;
	पूर्णाङ्क option_reject_shallow = -1; /* unspecified */
	पूर्णाङ्क deepen = 0;
	वर्ण *option_template = रिक्त, *option_depth = रिक्त, *option_since = रिक्त;
	वर्ण *option_origin = रिक्त;
	संरचना string_list option_not = STRING_LIST_INIT_NODUP;
	ध्रुव वर्ण *real_git_dir = रिक्त;
	ध्रुव वर्ण *ref_storage_format_uri = रिक्त;
	ध्रुव वर्ण *option_upload_pack = "git-upload-pack";
	पूर्णाङ्क option_progress = -1;
	पूर्णाङ्क option_sparse_checkout = 0;
	गणना transport_family family = TRANSPORT_FAMILY_ALL;
	संरचना string_list option_config = STRING_LIST_INIT_DUP;
	पूर्णाङ्क option_dissociate = 0;
	संरचना list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	पूर्णाङ्क option_filter_submodules = -1; /* unspecified */
	संरचना string_list server_options = STRING_LIST_INIT_NODUP;
	ध्रुव वर्ण *bundle_uri = रिक्त;
	वर्ण *option_rev = रिक्त;

	संरचना clone_opts opts = CLONE_OPTS_INIT;

	संरचना transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	संरचना option builtin_clone_options[] = {
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
			.मानम् = &option_recurse_submodules,
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
	संरचना strvec alternates = STRVEC_INIT;

	ध्रुव वर्ण * ध्रुव builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		रिक्त
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_identity("clone");

	repo_config(the_repository, git_clone_config, रिक्त);

	argc = parse_options(argc, argv, prefix, builtin_clone_options,
			     builtin_clone_usage, 0);

	सचे (argc > 2)
		usage_सन्देशः_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	सचे (argc == 0)
		usage_सन्देशः_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	सचे (option_depth || option_since || option_not.nr)
		deepen = 1;
	सचे (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	सचे (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, रिक्त);
		सचे (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	सचे (option_mirror) {
		option_bare = 1;
		option_tags = 0;
	}

	सचे (option_bare) {
		सचे (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	सचे (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	path = get_repo_path(repo_name, &is_bundle);
	सचे (path) {
		FREE_AND_NULL(path);
		repo = repo_to_free = absolute_pathdup(repo_name);
	} अञ्ञथा सचे (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} अञ्ञथा
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	सचे (option_depth && atoi(option_depth) < 1)
		die(_("depth %s is not a positive number"), option_depth);

	सचे (argc == 2)
		dir = xstrdup(argv[1]);
	अञ्ञथा
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = path_exists(dir);
	सचे (dest_exists && !is_empty_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	सचे (real_git_dir) {
		real_dest_exists = path_exists(real_git_dir);
		सचे (real_dest_exists && !is_empty_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_सन्देशः, "clone: from %s",
		    display_repo ? display_repo : repo);
	मुक्त(display_repo);

	सचे (option_bare)
		work_वृक्षः = रिक्त;
	अञ्ञथा {
		work_वृक्षः = getenv("GIT_WORK_TREE");
		सचे (work_वृक्षः && path_exists(work_वृक्षः))
			die(_("working tree '%s' already exists."), work_वृक्षः);
	}

	सचे (option_bare || work_वृक्षः)
		git_dir = xstrdup(dir);
	अञ्ञथा {
		work_वृक्षः = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_प्रक्षिप_common(remove_junk_on_signal);

	सचे (!option_bare) {
		सचे (safe_create_leading_directories_const(the_repository, work_वृक्षः) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_वृक्षः);
		सचे (dest_exists)
			junk_work_वृक्षः_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		अञ्ञथा सचे (mkdir(work_वृक्षः, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_वृक्षः);
		junk_work_वृक्षः = work_वृक्षः;
	}

	सचे (real_git_dir) {
		सचे (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} अञ्ञथा {
		सचे (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	सचे (safe_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	सचे (0 <= option_verbosity) {
		सचे (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		अञ्ञथा
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	सचे (option_recurse_submodules.nr > 0) {
		संरचना string_list_item *item;
		संरचना strbuf sb = STRBUF_INIT;
		पूर्णाङ्क val;

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

		सचे (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		सचे (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		अञ्ञथा सचे (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} अञ्ञथा सचे (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_वृक्षः,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, रिक्त);

	सचे (real_git_dir) {
		मुक्त((वर्ण *)git_dir);
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

	refs_create_refdir_stubs(the_repository, git_dir, रिक्त);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, रिक्त);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	सचे (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	सचे (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	सचे (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	सचे (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	सचे (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	सचे (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	सचे (option_origin) {
		मुक्त(remote_name);
		remote_name = xstrdup(option_origin);
	}

	सचे (!remote_name)
		remote_name = xstrdup("origin");

	सचे (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	सचे (option_bare) {
		सचे (option_mirror)
			src_ref_prefix = "refs/";
		strbuf_addstr(&branch_शिखरम्, src_ref_prefix);

		repo_config_set(the_repository, "core.bare", "true");
	} अञ्ञथा सचे (!option_rev) {
		strbuf_addf(&branch_शिखरम्, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&कुञ्चिका, "remote.%s.url", remote_name);
	repo_config_set(the_repository, कुञ्चिका.buf, repo);
	strbuf_reset(&कुञ्चिका);

	सचे (!option_tags) {
		strbuf_addf(&कुञ्चिका, "remote.%s.tagOpt", remote_name);
		repo_config_set(the_repository, कुञ्चिका.buf, "--no-tags");
		strbuf_reset(&कुञ्चिका);
	}

	remote = remote_get_early(remote_name);

	सचे (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_prefix,
				branch_शिखरम्.buf);

	path = get_repo_path(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && path && !is_bundle;
	सचे (is_local) {
		सचे (option_depth)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		सचे (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		सचे (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		सचे (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		सचे (!access(mkpath("%s/shallow", path), F_OK)) {
			सचे (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			सचे (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	सचे (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? path : रिक्त);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, path ? path : remote->url.v[0]);
	transport_set_verbosity(transport, option_verbosity, option_progress);
	transport->family = family;
	transport->cloning = 1;

	सचे (is_bundle) {
		संरचना bundle_header header = BUNDLE_HEADER_INIT;
		पूर्णाङ्क fd = read_bundle_header(path, &header);
		पूर्णाङ्क has_filter = header.filter.choice != LOFC_DISABLED;

		सचे (fd > 0)
			close(fd);
		bundle_header_release(&header);
		सचे (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_set_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	सचे (reject_shallow)
		transport_set_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	सचे (option_depth)
		transport_set_option(transport, TRANS_OPT_DEPTH,
				     option_depth);
	सचे (option_since)
		transport_set_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	सचे (option_not.nr)
		transport_set_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (ध्रुव वर्ण *)&option_not);
	सचे (option_single_branch) {
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		सचे (option_branch)
			opts.wants_head = 0;
	}

	सचे (option_upload_pack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	सचे (server_options.nr)
		transport->server_options = &server_options;

	सचे (filter_options.choice) {
		ध्रुव वर्ण *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	सचे (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	सचे (option_rev) {
		option_tags = 0;
		option_single_branch = 0;
		opts.wants_head = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	सचे (option_tags || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, TAG_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	सचे (option_branch)
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
	सचे (opts.wants_head || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_प्रक्षिप(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_version(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_set_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, रिक्त, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	सचे (bundle_uri) {
		संरचना remote_state *state;
		पूर्णाङ्क has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = रिक्त;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		सचे (repo_init(the_repository, git_dir, work_वृक्षः))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		अञ्ञथा सचे (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		अञ्ञथा सचे (has_heuristic)
			repo_config_set_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		मुक्त(the_repository->remote_state);
		the_repository->remote_state = state;
	} अञ्ञथा {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		सचे (transport->bundles &&
		    hashmap_get_size(&transport->bundles->bundles)) {
			संरचना remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = रिक्त;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			सचे (repo_init(the_repository, git_dir, work_वृक्षः))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			अञ्ञथा सचे (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			मुक्त(the_repository->remote_state);
			the_repository->remote_state = state;
		} अञ्ञथा {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	सचे (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	सचे (mapped_refs) {
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
		अत्थाय (ref = refs; ref; ref = ref->next)
			सचे (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				भङ्ग;
			}

		सचे (!is_local && !complete_refs_before_fetch) {
			सचे (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_head = find_ref_by_name(refs, "HEAD");
	remote_head_points_at = guess_remote_head(remote_head, mapped_refs,
						  REMOTE_GUESS_HEAD_QUIET);

	सचे (option_branch) {
		our_head_points_at = find_remote_branch(mapped_refs, option_branch);
		सचे (!our_head_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} अञ्ञथा सचे (option_rev) {
		our_head_points_at = mapped_refs;
		सचे (!our_head_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} अञ्ञथा सचे (remote_head_points_at) {
		our_head_points_at = remote_head_points_at;
	} अञ्ञथा सचे (remote_head) {
		our_head_points_at = रिक्त;
	} अञ्ञथा {
		वर्ण *to_free = रिक्त;
		ध्रुव वर्ण *branch;

		सचे (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		सचे (transport_ls_refs_options.unborn_head_target &&
		    skip_prefix(transport_ls_refs_options.unborn_head_target,
				"refs/heads/", &branch)) {
			unborn_head  = xstrdup(transport_ls_refs_options.unborn_head_target);
		} अञ्ञथा {
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

		मुक्त(to_free);
	}

	सचे (!option_rev)
		write_refspec_config(src_ref_prefix, our_head_points_at,
				     remote_head_points_at, &branch_शिखरम्);

	सचे (filter_options.choice)
		partial_clone_register(remote_name, &filter_options);

	सचे (is_local)
		clone_local(path, git_dir);
	अञ्ञथा सचे (mapped_refs && complete_refs_before_fetch) {
		सचे (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_head_points_at,
			   branch_शिखरम्.buf, reflog_सन्देशः.buf, transport,
			   !is_local);

	update_head(&opts, our_head_points_at, remote_head, unborn_head, reflog_सन्देशः.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_progress = transport->progress;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	सचे (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	सचे (option_sparse_checkout && git_sparse_checkout_init(dir))
		पटिनिवत्त 1;

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

	मुक्त(remote_name);
	strbuf_release(&reflog_सन्देशः);
	strbuf_release(&branch_शिखरम्);
	strbuf_release(&buf);
	strbuf_release(&कुञ्चिका);
	free_refs(mapped_refs);
	free_refs(remote_head_points_at);
	मुक्त(unborn_head);
	मुक्त(dir);
	मुक्त(path);
	मुक्त(repo_to_free);
	junk_mode = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	पटिनिवत्त err;
}
