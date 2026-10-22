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

स्थिर पूर्णाङ्क recurse_submodules_cb(अचर संरचना option *opt,
				 अचर वर्ण *arg, पूर्णाङ्क unset)
{
	यदि (unset)
		string_list_clear((संरचना string_list *)opt->मान, 0);
	अन्यथा यदि (arg)
		string_list_append((संरचना string_list *)opt->मान, arg);
	अन्यथा
		string_list_append((संरचना string_list *)opt->मान,
				   (अचर वर्ण *)opt->defval);

	फर्काऊ 0;
}

स्थिर अचर वर्ण *get_repo_path_1(संरचना strbuf *path, पूर्णाङ्क *is_bundle)
{
	स्थिर अचर वर्ण *suffix[] = { "/.git", "", ".git/.git", ".git" };
	स्थिर अचर वर्ण *bundle_suffix[] = { ".bundle", "" };
	माप_प्रकार baselen = path->len;
	संरचना stat st;
	पूर्णाङ्क i;

	लागि (i = 0; i < ARRAY_SIZE(suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, suffix[i]);
		यदि (stat(path->buf, &st))
			जारी;
		यदि (S_ISDIR(st.st_mode) && is_git_directory(path->buf)) {
			*is_bundle = 0;
			फर्काऊ path->buf;
		} अन्यथा यदि (S_ISREG(st.st_mode) && st.st_size > 8) {
			/* Is it a "gitfile"? */
			वर्ण signature[8];
			अचर वर्ण *dst;
			पूर्णाङ्क len, fd = खोलो(path->buf, O_RDONLY);
			यदि (fd < 0)
				जारी;
			len = read_in_full(fd, signature, 8);
			बंद(fd);
			यदि (len != 8 || म_भेदन(signature, "gitdir: ", 8))
				जारी;
			dst = read_gitfile(path->buf);
			यदि (dst) {
				*is_bundle = 0;
				फर्काऊ dst;
			}
		}
	}

	लागि (i = 0; i < ARRAY_SIZE(bundle_suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, bundle_suffix[i]);
		यदि (!stat(path->buf, &st) && S_ISREG(st.st_mode)) {
			*is_bundle = 1;
			फर्काऊ path->buf;
		}
	}

	फर्काऊ रिक्त;
}

स्थिर वर्ण *get_repo_path(अचर वर्ण *repo, पूर्णाङ्क *is_bundle)
{
	संरचना strbuf path = STRBUF_INIT;
	अचर वर्ण *raw;
	वर्ण *canon;

	strbuf_addstr(&path, repo);
	raw = get_repo_path_1(&path, is_bundle);
	canon = raw ? absolute_pathdup(raw) : रिक्त;
	strbuf_release(&path);
	फर्काऊ canon;
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

	यदि (!ref_git) {
		यदि (data->required)
			die("%s", err.buf);
		अन्यथा
			ख_लिखो(मानक_त्रुटि,
				_("info: Could not add alternate for '%s': %s\n"),
				item->string, err.buf);
	} अन्यथा {
		strvec_pushf(data->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	मुक्त(ref_git);
	फर्काऊ 0;
}

स्थिर शून्य read_alternates(संरचना strvec *alternates, अचर वर्ण *src_repo)
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
	खाता *in;
	संरचना strbuf path = STRBUF_INIT;
	संरचना strbuf पङ्क्ति = STRBUF_INIT;

	strbuf_addf(&path, "%s/objects/info/alternates", src_repo);

	in = ख_खोलो(path.buf, "r");
	यदि (!in) {
		यदि (त्रुटि_सं == ENOENT)
			गच्छ out;
		die_errno("could not read alternates file '%s'", path.buf);
	}

	जबसम्म (strbuf_getline(&पङ्क्ति, in) != खातापूर्ण) {
		वर्ण *abs_path;
		यदि (!पङ्क्ति.len || पङ्क्ति.buf[0] == '#')
			जारी;
		यदि (is_absolute_path(पङ्क्ति.buf)) {
			strvec_थप(alternates, पङ्क्ति.buf);
			जारी;
		}
		abs_path = mkpathdup("%s/objects/%s", src_repo, पङ्क्ति.buf);
		यदि (!normalize_path_copy(abs_path, abs_path))
			strvec_थप(alternates, abs_path);
		अन्यथा
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, पङ्क्ति.buf);
		मुक्त(abs_path);
	}

out:
	strbuf_release(&path);
	strbuf_release(&पङ्क्ति);
	यदि (in)
		ख_बंद(in);
}

स्थिर शून्य collect_alternates(संरचना strvec *alternates,
			       अचर वर्ण *local_source_repo)
{
	यदि (option_required_reference.nr || option_optional_reference.nr) {
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

	यदि (local_source_repo) {
		संरचना strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_source_repo);
		यदि (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		अन्यथा
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

स्थिर शून्य mkdir_if_missing(अचर वर्ण *pathname, mode_t mode)
{
	संरचना stat st;

	यदि (!सूची_गढ़ो(pathname, mode))
		फर्काऊ;

	यदि (त्रुटि_सं != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	अन्यथा यदि (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	अन्यथा यदि (!S_ISDIR(st.st_mode))
		die(_("%s exists and is not a directory"), pathname);
}

स्थिर शून्य copy_or_link_directory(संरचना strbuf *src, संरचना strbuf *dest,
				   अचर वर्ण *src_repo)
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

	यदि (!iter) {
		यदि (त्रुटि_सं == ENOTDIR) {
			पूर्णाङ्क saved_errno = त्रुटि_सं;
			संरचना stat st;

			यदि (!lstat(src->buf, &st) && S_ISLNK(st.st_mode))
				die(_("'%s' is a symlink, refusing to clone with --local"),
				    src->buf);
			त्रुटि_सं = saved_errno;
		}
		die_errno(_("failed to start iterator over '%s'"), src->buf);
	}

	strbuf_addch(src, '/');
	src_len = src->len;
	strbuf_addch(dest, '/');
	dest_len = dest->len;

	जबसम्म ((iter_status = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_path);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_path);

		यदि (S_ISLNK(iter->st.st_mode))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_path);

		यदि (S_ISDIR(iter->st.st_mode)) {
			mkdir_if_missing(dest->buf, 0777);
			जारी;
		}

		/* Alternates were already handled earlier. */
		यदि (!fspathcmp(iter->relative_path, "info/alternates"))
			जारी;

		यदि (unlink(dest->buf) && त्रुटि_सं != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		यदि (!option_no_hardlinks) {
			यदि (!link(src->buf, dest->buf)) {
				संरचना stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				यदि (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				यदि (st.st_mode != iter->st.st_mode ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_size != iter->st.st_size ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				जारी;
			}
			यदि (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		यदि (copy_file_with_time(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	यदि (iter_status != ITER_DONE) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

स्थिर शून्य clone_local(अचर वर्ण *src_repo, अचर वर्ण *dest_repo)
{
	यदि (!option_shared) {
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

	यदि (0 <= option_verbosity)
		ख_लिखो(मानक_त्रुटि, _("done.\n"));
}

स्थिर अचर वर्ण *junk_work_रूख;
स्थिर पूर्णाङ्क junk_work_रूख_flags;
स्थिर अचर वर्ण *junk_git_dir;
स्थिर पूर्णाङ्क junk_git_dir_flags;
स्थिर गणना {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_mode = JUNK_LEAVE_NONE;

स्थिर अचर वर्ण junk_leave_repo_सन्देश[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

स्थिर शून्य remove_junk(शून्य)
{
	संरचना strbuf sb = STRBUF_INIT;

	विकल्प (junk_mode) {
	पक्ष JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_सन्देश));
		/* fall-through */
	पक्ष JUNK_LEAVE_ALL:
		फर्काऊ;
	स्वभाव:
		/* proceed to removal */
		रोक;
	}

	यदि (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	यदि (junk_work_रूख) {
		strbuf_addstr(&sb, junk_work_रूख);
		remove_dir_recursively(&sb, junk_work_रूख_flags);
	}
	strbuf_release(&sb);
}

स्थिर शून्य remove_junk_on_signal(पूर्णाङ्क signo)
{
	remove_junk();
	sigchain_झिक(signo);
	उत्थापय(signo);
}

स्थिर संरचना ref *find_remote_branch(अचर संरचना ref *refs, अचर वर्ण *branch)
{
	संरचना ref *ref;
	संरचना strbuf head = STRBUF_INIT;
	strbuf_addstr(&head, "refs/heads/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	यदि (ref)
		फर्काऊ ref;

	strbuf_addstr(&head, "refs/tags/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	फर्काऊ ref;
}

स्थिर संरचना ref *wanted_peer_refs(संरचना clone_opts *opts,
				    अचर संरचना ref *refs,
				    संरचना refspec *refspec)
{
	संरचना ref *local_refs = रिक्त;
	संरचना ref **tail = &local_refs;
	संरचना ref *to_free = रिक्त;

	यदि (opts->wants_head) {
		संरचना ref *head = copy_ref(find_ref_by_name(refs, "HEAD"));
		यदि (head)
			tail_link_ref(head, &tail);
		यदि (option_single_branch)
			refs = to_free =
				guess_remote_head(head, refs,
						  REMOTE_GUESS_HEAD_QUIET);
	} अन्यथा यदि (option_single_branch) {
		local_refs = रिक्त;
		tail = &local_refs;
		refs = to_free = copy_ref(find_remote_branch(refs, option_branch));
	}

	लागि (माप_प्रकार i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->वस्तुहरू[i], &tail, 0);

	free_one_ref(to_free);

	फर्काऊ local_refs;
}

स्थिर शून्य write_remote_refs(अचर संरचना ref *local_refs)
{
	अचर संरचना ref *r;

	संरचना ref_transaction *t;
	संरचना strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	यदि (!t)
		die("%s", err.buf);

	लागि (r = local_refs; r; r = r->next) {
		यदि (!r->peer_ref)
			जारी;
		यदि (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   रिक्त, 0, रिक्त, &err))
			die("%s", err.buf);
	}

	यदि (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

स्थिर शून्य write_followtags(अचर संरचना ref *refs, अचर वर्ण *सन्देश)
{
	अचर संरचना ref *ref;
	लागि (ref = refs; ref; ref = ref->next) {
		यदि (!starts_with(ref->name, "refs/tags/"))
			जारी;
		यदि (ends_with(ref->name, "^{}"))
			जारी;
		यदि (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			जारी;
		refs_update_ref(get_main_ref_store(the_repository), सन्देश,
				ref->name, &ref->old_oid, रिक्त, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

स्थिर अचर संरचना object_id *iterate_ref_map(शून्य *cb_data)
{
	संरचना ref **rm = cb_data;
	संरचना ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	जबसम्म (ref && !ref->peer_ref)
		ref = ref->next;
	यदि (!ref)
		फर्काऊ रिक्त;

	*rm = ref->next;
	फर्काऊ &ref->old_oid;
}

स्थिर शून्य update_remote_refs(अचर संरचना ref *refs,
			       अचर संरचना ref *mapped_refs,
			       अचर संरचना ref *remote_head_points_at,
			       अचर वर्ण *branch_शीर्ष,
			       अचर वर्ण *सन्देश,
			       संरचना transport *transport,
			       पूर्णाङ्क check_connectivity)
{
	अचर संरचना ref *rm = mapped_refs;

	यदि (check_connectivity) {
		संरचना check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.progress = transport->progress;

		यदि (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	यदि (refs) {
		write_remote_refs(mapped_refs);
		यदि (option_single_branch && option_tags)
			write_followtags(refs, सन्देश);
	}

	यदि (remote_head_points_at && remote_head_points_at->peer_ref && !option_bare) {
		संरचना strbuf head_ref = STRBUF_INIT;
		strbuf_addstr(&head_ref, branch_शीर्ष);
		strbuf_addstr(&head_ref, "HEAD");
		यदि (refs_update_symref(get_main_ref_store(the_repository), head_ref.buf,
				       remote_head_points_at->peer_ref->name,
				       सन्देश) < 0)
			die(_("unable to update %s"), head_ref.buf);
		strbuf_release(&head_ref);
	}
}

स्थिर शून्य update_head(संरचना clone_opts *opts, अचर संरचना ref *our, अचर संरचना ref *remote,
			अचर वर्ण *unborn, अचर वर्ण *सन्देश)
{
	अचर वर्ण *head;
	यदि (our && !opts->detach && skip_prefix(our->name, "refs/heads/", &head)) {
		/* Local default branch link */
		यदि (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, रिक्त) < 0)
			die(_("unable to update HEAD"));
		यदि (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					सन्देश, "HEAD", &our->old_oid, रिक्त, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, head, remote_name, our->name);
		}
	} अन्यथा यदि (our) {
		संरचना commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), सन्देश,
				"HEAD", &c->object.oid, रिक्त, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} अन्यथा यदि (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), सन्देश,
				"HEAD", &remote->old_oid, रिक्त, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} अन्यथा यदि (unborn && skip_prefix(unborn, "refs/heads/", &head)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		यदि (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, रिक्त) < 0)
			die(_("unable to update HEAD"));
		यदि (!option_bare)
			install_branch_config(0, head, remote_name, unborn);
	}
}

स्थिर पूर्णाङ्क git_sparse_checkout_init(अचर वर्ण *repo)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	पूर्णाङ्क result = 0;
	संरचना repo_config_मानहरू *cfg = repo_config_मानहरू(the_repository);

	strvec_pushl(&cmd.args, "-C", repo, "sparse-checkout", "set", रिक्त);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	यदि (run_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		result = 1;
	}

	फर्काऊ result;
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
	संरचना रूख *रूख;
	संरचना रूख_desc t;
	पूर्णाङ्क err = 0;
	संरचना run_hooks_opt hook_opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	यदि (option_no_checkout)
		फर्काऊ 0;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, रिक्त);
	यदि (!head) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		फर्काऊ 0;
	}
	यदि (!म_भेद(head, "HEAD")) {
		यदि (advice_enabled(ADVICE_DETACHED_HEAD))
			detach_advice(oid_to_hex(&oid));
		FREE_AND_NULL(head);
	} अन्यथा {
		यदि (!starts_with(head, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	setup_work_रूख(the_repository);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	स_रखो(&opts, 0, परिमाण opts);
	opts.update = 1;
	opts.merge = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_merge;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	init_checkout_metadata(&opts.meta, head, &oid, रिक्त);

	रूख = repo_parse_रूख_indirect(the_repository, &oid);
	यदि (!रूख)
		die(_("unable to parse commit %s"), oid_to_hex(&oid));
	यदि (repo_parse_रूख(the_repository, रूख) < 0)
		निकास(128);
	init_रूख_desc(&t, &रूख->object.oid, रूख->buffer, रूख->size);
	यदि (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	मुक्त(head);

	यदि (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.args, oid_to_hex(null_oid(the_hash_algo)),
		     oid_to_hex(&oid), "1", रिक्त);
	err |= run_hooks_opt(the_repository, "post-checkout", &hook_opt);

	यदि (!err && (option_recurse_submodules.nr > 0)) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;
		strvec_pushl(&cmd.args, "submodule", "update", "--require-init",
			     "--recursive", रिक्त);

		यदि (option_shallow_submodules == 1)
			strvec_थप(&cmd.args, "--depth=1");

		यदि (max_jobs != -1)
			strvec_pushf(&cmd.args, "--jobs=%d", max_jobs);

		यदि (submodule_progress)
			strvec_थप(&cmd.args, "--progress");

		यदि (option_verbosity < 0)
			strvec_थप(&cmd.args, "--quiet");

		यदि (option_remote_submodules) {
			strvec_थप(&cmd.args, "--remote");
			strvec_थप(&cmd.args, "--no-fetch");
		}

		यदि (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(ref_storage_format));

		यदि (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.args, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		यदि (option_single_branch >= 0)
			strvec_थप(&cmd.args, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = run_command(&cmd);
	}

	फर्काऊ err;
}

स्थिर पूर्णाङ्क git_clone_config(अचर वर्ण *k, अचर वर्ण *v,
			    अचर संरचना config_context *ctx, शून्य *cb)
{
	यदि (!म_भेद(k, "clone.defaultremotename")) {
		यदि (!v)
			फर्काऊ config_error_nonbool(k);
		मुक्त(remote_name);
		remote_name = xstrdup(v);
	}
	यदि (!म_भेद(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	यदि (!म_भेद(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	फर्काऊ git_default_config(k, v, ctx, cb);
}

स्थिर पूर्णाङ्क write_one_config(अचर वर्ण *साँचो, अचर वर्ण *मान,
			    अचर संरचना config_context *ctx,
			    शून्य *data)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	पूर्णाङ्क apply_failed = git_clone_config(साँचो, मान, ctx, data);
	यदि (apply_failed)
		फर्काऊ apply_failed;

	फर्काऊ repo_config_set_multivar_gently(the_repository, साँचो,
					       मान ? मान : "true",
					       CONFIG_REGEX_NONE, 0);
}

स्थिर शून्य write_config(संरचना string_list *config)
{
	पूर्णाङ्क i;

	लागि (i = 0; i < config->nr; i++) {
		यदि (git_config_parse_parameter(config->वस्तुहरू[i].string,
					       write_one_config, रिक्त) < 0)
			die(_("unable to write parameters to config file"));
	}
}

स्थिर शून्य write_refspec_config(अचर वर्ण *src_ref_prefix,
		अचर संरचना ref *our_head_points_at,
		अचर संरचना ref *remote_head_points_at,
		संरचना strbuf *branch_शीर्ष)
{
	संरचना strbuf साँचो = STRBUF_INIT;
	संरचना strbuf मान = STRBUF_INIT;

	यदि (option_mirror || !option_bare) {
		यदि (option_single_branch && !option_mirror) {
			यदि (option_branch) {
				यदि (starts_with(our_head_points_at->name, "refs/tags/"))
					strbuf_addf(&मान, "+%s:%s", our_head_points_at->name,
						our_head_points_at->name);
				अन्यथा
					strbuf_addf(&मान, "+%s:%s%s", our_head_points_at->name,
						branch_शीर्ष->buf, option_branch);
			} अन्यथा यदि (remote_head_points_at) {
				अचर वर्ण *head = remote_head_points_at->name;
				यदि (!skip_prefix(head, "refs/heads/", &head))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&मान, "+%s:%s%s", remote_head_points_at->name,
						branch_शीर्ष->buf, head);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} अन्यथा {
			strbuf_addf(&मान, "+%s*:%s*", src_ref_prefix, branch_शीर्ष->buf);
		}
		/* Configure the remote */
		यदि (मान.len) {
			strbuf_addf(&साँचो, "remote.%s.fetch", remote_name);
			repo_config_set_multivar(the_repository, साँचो.buf, मान.buf, "^$", 0);
			strbuf_reset(&साँचो);

			यदि (option_mirror) {
				strbuf_addf(&साँचो, "remote.%s.mirror", remote_name);
				repo_config_set(the_repository, साँचो.buf, "true");
				strbuf_reset(&साँचो);
			}
		}
	}

	strbuf_release(&साँचो);
	strbuf_release(&मान);
}

स्थिर शून्य dissociate_from_references(शून्य)
{
	वर्ण *alternates = repo_git_path(the_repository, "objects/info/alternates");

	यदि (!access(alternates, F_OK)) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.args, "repack", "-a", "-d", रिक्त);
		यदि (run_command(&cmd))
			die(_("cannot repack to clean up"));
		यदि (unlink(alternates) && त्रुटि_सं != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	मुक्त(alternates);
}

स्थिर पूर्णाङ्क path_exists(अचर वर्ण *path)
{
	संरचना stat sb;
	फर्काऊ !stat(path, &sb);
}

पूर्णाङ्क cmd_clone(पूर्णाङ्क argc,
	      अचर वर्ण **argv,
	      अचर वर्ण *prefix,
	      संरचना repository *repository UNUSED)
{
	पूर्णाङ्क is_bundle = 0, is_local;
	पूर्णाङ्क reject_shallow = 0;
	अचर वर्ण *repo_name, *repo, *work_रूख, *git_dir;
	वर्ण *repo_to_free = रिक्त;
	वर्ण *path = रिक्त, *dir, *display_repo = रिक्त;
	पूर्णाङ्क dest_exists, real_dest_exists = 0;
	अचर संरचना ref *refs, *remote_head;
	संरचना ref *remote_head_points_at = रिक्त;
	अचर संरचना ref *our_head_points_at;
	वर्ण *unborn_head = रिक्त;
	संरचना ref *mapped_refs = रिक्त;
	अचर संरचना ref *ref;
	संरचना strbuf साँचो = STRBUF_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf branch_शीर्ष = STRBUF_INIT, reflog_सन्देश = STRBUF_INIT;
	संरचना transport *transport = रिक्त;
	अचर वर्ण *src_ref_prefix = "refs/heads/";
	संरचना remote *remote;
	पूर्णाङ्क err = 0, complete_refs_before_fetch = 1;
	पूर्णाङ्क submodule_progress;
	पूर्णाङ्क filter_submodules = 0;
	पूर्णाङ्क hash_algo;
	गणना ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	अचर पूर्णाङ्क do_not_override_repo_unix_permissions = -1;
	पूर्णाङ्क option_reject_shallow = -1; /* unspecified */
	पूर्णाङ्क deepen = 0;
	वर्ण *option_template = रिक्त, *option_depth = रिक्त, *option_since = रिक्त;
	वर्ण *option_origin = रिक्त;
	संरचना string_list option_not = STRING_LIST_INIT_NODUP;
	अचर वर्ण *real_git_dir = रिक्त;
	अचर वर्ण *ref_storage_format_uri = रिक्त;
	अचर वर्ण *option_upload_pack = "git-upload-pack";
	पूर्णाङ्क option_progress = -1;
	पूर्णाङ्क option_sparse_checkout = 0;
	गणना transport_family family = TRANSPORT_FAMILY_ALL;
	संरचना string_list option_config = STRING_LIST_INIT_DUP;
	पूर्णाङ्क option_dissociate = 0;
	संरचना list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	पूर्णाङ्क option_filter_submodules = -1; /* unspecified */
	संरचना string_list server_options = STRING_LIST_INIT_NODUP;
	अचर वर्ण *bundle_uri = रिक्त;
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
			.मान = &option_recurse_submodules,
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

	अचर वर्ण * अचर builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		रिक्त
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_identity("clone");

	repo_config(the_repository, git_clone_config, रिक्त);

	argc = parse_options(argc, argv, prefix, builtin_clone_options,
			     builtin_clone_usage, 0);

	यदि (argc > 2)
		usage_सन्देश_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	यदि (argc == 0)
		usage_सन्देश_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	यदि (option_depth || option_since || option_not.nr)
		deepen = 1;
	यदि (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	यदि (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, रिक्त);
		यदि (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	यदि (option_mirror) {
		option_bare = 1;
		option_tags = 0;
	}

	यदि (option_bare) {
		यदि (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	यदि (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	path = get_repo_path(repo_name, &is_bundle);
	यदि (path) {
		FREE_AND_NULL(path);
		repo = repo_to_free = absolute_pathdup(repo_name);
	} अन्यथा यदि (म_अक्षर(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} अन्यथा
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	यदि (option_depth && म_से_प(option_depth) < 1)
		die(_("depth %s is not a positive number"), option_depth);

	यदि (argc == 2)
		dir = xstrdup(argv[1]);
	अन्यथा
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = path_exists(dir);
	यदि (dest_exists && !is_empty_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	यदि (real_git_dir) {
		real_dest_exists = path_exists(real_git_dir);
		यदि (real_dest_exists && !is_empty_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_सन्देश, "clone: from %s",
		    display_repo ? display_repo : repo);
	मुक्त(display_repo);

	यदि (option_bare)
		work_रूख = रिक्त;
	अन्यथा {
		work_रूख = दो_पर्या("GIT_WORK_TREE");
		यदि (work_रूख && path_exists(work_रूख))
			die(_("working tree '%s' already exists."), work_रूख);
	}

	यदि (option_bare || work_रूख)
		git_dir = xstrdup(dir);
	अन्यथा {
		work_रूख = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	निकास_पर(remove_junk);
	sigchain_थप_common(remove_junk_on_signal);

	यदि (!option_bare) {
		यदि (safe_create_leading_directories_const(the_repository, work_रूख) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_रूख);
		यदि (dest_exists)
			junk_work_रूख_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		अन्यथा यदि (सूची_गढ़ो(work_रूख, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_रूख);
		junk_work_रूख = work_रूख;
	}

	यदि (real_git_dir) {
		यदि (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} अन्यथा {
		यदि (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	यदि (safe_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	यदि (0 <= option_verbosity) {
		यदि (option_bare)
			ख_लिखो(मानक_त्रुटि, _("Cloning into bare repository '%s'...\n"), dir);
		अन्यथा
			ख_लिखो(मानक_त्रुटि, _("Cloning into '%s'...\n"), dir);
	}

	यदि (option_recurse_submodules.nr > 0) {
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

		यदि (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		यदि (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		अन्यथा यदि (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} अन्यथा यदि (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_रूख,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, रिक्त);

	यदि (real_git_dir) {
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
	यदि (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	यदि (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	यदि (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	यदि (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	यदि (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	यदि (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	यदि (option_origin) {
		मुक्त(remote_name);
		remote_name = xstrdup(option_origin);
	}

	यदि (!remote_name)
		remote_name = xstrdup("origin");

	यदि (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	यदि (option_bare) {
		यदि (option_mirror)
			src_ref_prefix = "refs/";
		strbuf_addstr(&branch_शीर्ष, src_ref_prefix);

		repo_config_set(the_repository, "core.bare", "true");
	} अन्यथा यदि (!option_rev) {
		strbuf_addf(&branch_शीर्ष, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&साँचो, "remote.%s.url", remote_name);
	repo_config_set(the_repository, साँचो.buf, repo);
	strbuf_reset(&साँचो);

	यदि (!option_tags) {
		strbuf_addf(&साँचो, "remote.%s.tagOpt", remote_name);
		repo_config_set(the_repository, साँचो.buf, "--no-tags");
		strbuf_reset(&साँचो);
	}

	remote = remote_get_early(remote_name);

	यदि (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_prefix,
				branch_शीर्ष.buf);

	path = get_repo_path(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && path && !is_bundle;
	यदि (is_local) {
		यदि (option_depth)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		यदि (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		यदि (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		यदि (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		यदि (!access(mkpath("%s/shallow", path), F_OK)) {
			यदि (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			यदि (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	यदि (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? path : रिक्त);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, path ? path : remote->url.v[0]);
	transport_set_verbosity(transport, option_verbosity, option_progress);
	transport->family = family;
	transport->cloning = 1;

	यदि (is_bundle) {
		संरचना bundle_header header = BUNDLE_HEADER_INIT;
		पूर्णाङ्क fd = read_bundle_header(path, &header);
		पूर्णाङ्क has_filter = header.filter.choice != LOFC_DISABLED;

		यदि (fd > 0)
			बंद(fd);
		bundle_header_release(&header);
		यदि (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_set_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	यदि (reject_shallow)
		transport_set_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	यदि (option_depth)
		transport_set_option(transport, TRANS_OPT_DEPTH,
				     option_depth);
	यदि (option_since)
		transport_set_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	यदि (option_not.nr)
		transport_set_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (अचर वर्ण *)&option_not);
	यदि (option_single_branch) {
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		यदि (option_branch)
			opts.wants_head = 0;
	}

	यदि (option_upload_pack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	यदि (server_options.nr)
		transport->server_options = &server_options;

	यदि (filter_options.choice) {
		अचर वर्ण *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	यदि (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	यदि (option_rev) {
		option_tags = 0;
		option_single_branch = 0;
		opts.wants_head = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	यदि (option_tags || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, TAG_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	यदि (option_branch)
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
	यदि (opts.wants_head || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_थप(&transport_ls_refs_options.ref_prefixes, "HEAD");

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
	यदि (bundle_uri) {
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
		यदि (repo_init(the_repository, git_dir, work_रूख))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		अन्यथा यदि (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		अन्यथा यदि (has_heuristic)
			repo_config_set_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		मुक्त(the_repository->remote_state);
		the_repository->remote_state = state;
	} अन्यथा {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		यदि (transport->bundles &&
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
			यदि (repo_init(the_repository, git_dir, work_रूख))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			अन्यथा यदि (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			मुक्त(the_repository->remote_state);
			the_repository->remote_state = state;
		} अन्यथा {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	यदि (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	यदि (mapped_refs) {
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
		लागि (ref = refs; ref; ref = ref->next)
			यदि (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				रोक;
			}

		यदि (!is_local && !complete_refs_before_fetch) {
			यदि (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_head = find_ref_by_name(refs, "HEAD");
	remote_head_points_at = guess_remote_head(remote_head, mapped_refs,
						  REMOTE_GUESS_HEAD_QUIET);

	यदि (option_branch) {
		our_head_points_at = find_remote_branch(mapped_refs, option_branch);
		यदि (!our_head_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} अन्यथा यदि (option_rev) {
		our_head_points_at = mapped_refs;
		यदि (!our_head_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} अन्यथा यदि (remote_head_points_at) {
		our_head_points_at = remote_head_points_at;
	} अन्यथा यदि (remote_head) {
		our_head_points_at = रिक्त;
	} अन्यथा {
		वर्ण *to_free = रिक्त;
		अचर वर्ण *branch;

		यदि (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		यदि (transport_ls_refs_options.unborn_head_target &&
		    skip_prefix(transport_ls_refs_options.unborn_head_target,
				"refs/heads/", &branch)) {
			unborn_head  = xstrdup(transport_ls_refs_options.unborn_head_target);
		} अन्यथा {
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

	यदि (!option_rev)
		write_refspec_config(src_ref_prefix, our_head_points_at,
				     remote_head_points_at, &branch_शीर्ष);

	यदि (filter_options.choice)
		partial_clone_register(remote_name, &filter_options);

	यदि (is_local)
		clone_local(path, git_dir);
	अन्यथा यदि (mapped_refs && complete_refs_before_fetch) {
		यदि (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_head_points_at,
			   branch_शीर्ष.buf, reflog_सन्देश.buf, transport,
			   !is_local);

	update_head(&opts, our_head_points_at, remote_head, unborn_head, reflog_सन्देश.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_progress = transport->progress;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	यदि (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	यदि (option_sparse_checkout && git_sparse_checkout_init(dir))
		फर्काऊ 1;

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
	strbuf_release(&reflog_सन्देश);
	strbuf_release(&branch_शीर्ष);
	strbuf_release(&buf);
	strbuf_release(&साँचो);
	free_refs(mapped_refs);
	free_refs(remote_head_points_at);
	मुक्त(unborn_head);
	मुक्त(dir);
	मुक्त(path);
	मुक्त(repo_to_free);
	junk_mode = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	फर्काऊ err;
}
