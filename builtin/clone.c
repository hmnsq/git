/*
 * Builtin "git clone"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *		 2008 Daniel Barkalow <barkalow@iabervon.org>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 *
 * Clone a repository into a different directory that does not yet exist.
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"

#समावेश "abspath.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "copy.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "lockfile.h"
#समावेश "parse-options.h"
#समावेश "refs.h"
#समावेश "refspec.h"
#समावेश "object-file.h"
#समावेश "odb.h"
#समावेश "tree.h"
#समावेश "tree-walk.h"
#समावेश "unpack-trees.h"
#समावेश "transport.h"
#समावेश "strbuf.h"
#समावेश "dir.h"
#समावेश "dir-iterator.h"
#समावेश "iterator.h"
#समावेश "sigchain.h"
#समावेश "branch.h"
#समावेश "remote.h"
#समावेश "run-command.h"
#समावेश "setup.h"
#समावेश "connected.h"
#समावेश "packfile.h"
#समावेश "path.h"
#समावेश "pkt-line.h"
#समावेश "list-objects-filter-options.h"
#समावेश "hook.h"
#समावेश "bundle.h"
#समावेश "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

काष्ठा clone_opts {
	पूर्णांक wants_शीर्ष;
	पूर्णांक detach;
};
#घोषणा CLONE_OPTS_INIT { \
	.wants_शीर्ष = 1 /* default enabled */ \
}

अटल पूर्णांक option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
अटल पूर्णांक option_local = -1, option_no_hardlinks, option_shared;
अटल पूर्णांक option_tags = 1; /* default enabled */
अटल पूर्णांक option_shallow_submodules;
अटल पूर्णांक config_reject_shallow = -1;    /* unspecified */
अटल अक्षर *remote_name = शून्य;
अटल अक्षर *option_branch = शून्य;
अटल पूर्णांक option_verbosity;
अटल काष्ठा string_list option_required_reference = STRING_LIST_INIT_NODUP;
अटल काष्ठा string_list option_optional_reference = STRING_LIST_INIT_NODUP;
अटल पूर्णांक max_jobs = -1;
अटल काष्ठा string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
अटल पूर्णांक config_filter_submodules = -1;    /* unspecified */
अटल पूर्णांक option_remote_submodules;

अटल पूर्णांक recurse_submodules_cb(स्थिर काष्ठा option *opt,
				 स्थिर अक्षर *arg, पूर्णांक unset)
{
	अगर (unset)
		string_list_clear((काष्ठा string_list *)opt->मूल्य, 0);
	अन्यथा अगर (arg)
		string_list_append((काष्ठा string_list *)opt->मूल्य, arg);
	अन्यथा
		string_list_append((काष्ठा string_list *)opt->मूल्य,
				   (स्थिर अक्षर *)opt->defval);

	वापस 0;
}

अटल स्थिर अक्षर *get_repo_path_1(काष्ठा strbuf *path, पूर्णांक *is_bundle)
{
	अटल स्थिर अक्षर *suffix[] = { "/.git", "", ".git/.git", ".git" };
	अटल स्थिर अक्षर *bundle_suffix[] = { ".bundle", "" };
	माप_प्रकार baselen = path->len;
	काष्ठा stat st;
	पूर्णांक i;

	क्रम (i = 0; i < ARRAY_आकार(suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, suffix[i]);
		अगर (stat(path->buf, &st))
			जारी;
		अगर (S_ISDIR(st.st_mode) && is_git_directory(path->buf)) {
			*is_bundle = 0;
			वापस path->buf;
		} अन्यथा अगर (S_ISREG(st.st_mode) && st.st_आकार > 8) {
			/* Is it a "gitfile"? */
			अक्षर signature[8];
			स्थिर अक्षर *dst;
			पूर्णांक len, fd = खोलो(path->buf, O_RDONLY);
			अगर (fd < 0)
				जारी;
			len = read_in_full(fd, signature, 8);
			बंद(fd);
			अगर (len != 8 || म_भेदन(signature, "gitdir: ", 8))
				जारी;
			dst = read_gitfile(path->buf);
			अगर (dst) {
				*is_bundle = 0;
				वापस dst;
			}
		}
	}

	क्रम (i = 0; i < ARRAY_आकार(bundle_suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, bundle_suffix[i]);
		अगर (!stat(path->buf, &st) && S_ISREG(st.st_mode)) {
			*is_bundle = 1;
			वापस path->buf;
		}
	}

	वापस शून्य;
}

अटल अक्षर *get_repo_path(स्थिर अक्षर *repo, पूर्णांक *is_bundle)
{
	काष्ठा strbuf path = STRBUF_INIT;
	स्थिर अक्षर *raw;
	अक्षर *canon;

	strbuf_addstr(&path, repo);
	raw = get_repo_path_1(&path, is_bundle);
	canon = raw ? absolute_pathdup(raw) : शून्य;
	strbuf_release(&path);
	वापस canon;
}

काष्ठा collect_alternates_तथ्य {
	काष्ठा strvec *alternates;
	bool required;
};

अटल पूर्णांक collect_one_alternate(काष्ठा string_list_item *item, व्योम *cb_तथ्य)
{
	काष्ठा collect_alternates_तथ्य *तथ्य = cb_तथ्य;
	काष्ठा strbuf err = STRBUF_INIT;
	अक्षर *ref_git = compute_alternate_path(item->string, &err);

	अगर (!ref_git) {
		अगर (तथ्य->required)
			die("%s", err.buf);
		अन्यथा
			ख_लिखो(मानक_त्रुटि,
				_("info: Could not add alternate for '%s': %s\n"),
				item->string, err.buf);
	} अन्यथा {
		strvec_pushf(तथ्य->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	मुक्त(ref_git);
	वापस 0;
}

अटल व्योम read_alternates(काष्ठा strvec *alternates, स्थिर अक्षर *src_repo)
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
	काष्ठा strbuf path = STRBUF_INIT;
	काष्ठा strbuf पंक्ति = STRBUF_INIT;

	strbuf_addf(&path, "%s/objects/info/alternates", src_repo);

	in = ख_खोलो(path.buf, "r");
	अगर (!in) {
		अगर (त्रुटि_सं == ENOENT)
			जाओ out;
		die_errno("could not read alternates file '%s'", path.buf);
	}

	जबतक (strbuf_getline(&पंक्ति, in) != खातापूर्ण) {
		अक्षर *abs_path;
		अगर (!पंक्ति.len || पंक्ति.buf[0] == '#')
			जारी;
		अगर (is_absolute_path(पंक्ति.buf)) {
			strvec_धकेलो(alternates, पंक्ति.buf);
			जारी;
		}
		abs_path = mkpathdup("%s/objects/%s", src_repo, पंक्ति.buf);
		अगर (!normalize_path_copy(abs_path, abs_path))
			strvec_धकेलो(alternates, abs_path);
		अन्यथा
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, पंक्ति.buf);
		मुक्त(abs_path);
	}

out:
	strbuf_release(&path);
	strbuf_release(&पंक्ति);
	अगर (in)
		ख_बंद(in);
}

अटल व्योम collect_alternates(काष्ठा strvec *alternates,
			       स्थिर अक्षर *local_source_repo)
{
	अगर (option_required_reference.nr || option_optional_reference.nr) {
		काष्ठा collect_alternates_तथ्य तथ्य = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &तथ्य);
		तथ्य.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &तथ्य);
	}

	अगर (local_source_repo) {
		काष्ठा strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_source_repo);
		अगर (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		अन्यथा
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

अटल व्योम mkdir_if_missing(स्थिर अक्षर *pathname, mode_t mode)
{
	काष्ठा stat st;

	अगर (!सूची_गढ़ो(pathname, mode))
		वापस;

	अगर (त्रुटि_सं != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	अन्यथा अगर (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	अन्यथा अगर (!S_ISDIR(st.st_mode))
		die(_("%s exists and is not a directory"), pathname);
}

अटल व्योम copy_or_link_directory(काष्ठा strbuf *src, काष्ठा strbuf *dest,
				   स्थिर अक्षर *src_repo)
{
	पूर्णांक src_len, dest_len;
	काष्ठा dir_iterator *iter;
	पूर्णांक iter_status;

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
	die_upon_dubious_ownership(शून्य, शून्य, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	अगर (!iter) {
		अगर (त्रुटि_सं == ENOTDIR) {
			पूर्णांक saved_errno = त्रुटि_सं;
			काष्ठा stat st;

			अगर (!lstat(src->buf, &st) && S_ISLNK(st.st_mode))
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

	जबतक ((iter_status = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_path);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_path);

		अगर (S_ISLNK(iter->st.st_mode))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_path);

		अगर (S_ISDIR(iter->st.st_mode)) {
			mkdir_if_missing(dest->buf, 0777);
			जारी;
		}

		/* Alternates were already handled earlier. */
		अगर (!fspathcmp(iter->relative_path, "info/alternates"))
			जारी;

		अगर (unlink(dest->buf) && त्रुटि_सं != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		अगर (!option_no_hardlinks) {
			अगर (!link(src->buf, dest->buf)) {
				काष्ठा stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				अगर (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				अगर (st.st_mode != iter->st.st_mode ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_आकार != iter->st.st_आकार ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				जारी;
			}
			अगर (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		अगर (copy_file_with_time(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	अगर (iter_status != ITER_DONE) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

अटल व्योम clone_local(स्थिर अक्षर *src_repo, स्थिर अक्षर *dest_repo)
{
	अगर (!option_shared) {
		काष्ठा strbuf src = STRBUF_INIT;
		काष्ठा strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	अगर (0 <= option_verbosity)
		ख_लिखो(मानक_त्रुटि, _("done.\n"));
}

अटल स्थिर अक्षर *junk_work_वृक्ष;
अटल पूर्णांक junk_work_वृक्ष_flags;
अटल स्थिर अक्षर *junk_git_dir;
अटल पूर्णांक junk_git_dir_flags;
अटल क्रमागत {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_mode = JUNK_LEAVE_NONE;

अटल स्थिर अक्षर junk_leave_repo_संदेश[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

अटल व्योम remove_junk(व्योम)
{
	काष्ठा strbuf sb = STRBUF_INIT;

	चयन (junk_mode) {
	हाल JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_संदेश));
		/* fall-through */
	हाल JUNK_LEAVE_ALL:
		वापस;
	शेष:
		/* proceed to removal */
		अवरोध;
	}

	अगर (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	अगर (junk_work_वृक्ष) {
		strbuf_addstr(&sb, junk_work_वृक्ष);
		remove_dir_recursively(&sb, junk_work_वृक्ष_flags);
	}
	strbuf_release(&sb);
}

अटल व्योम remove_junk_on_signal(पूर्णांक signo)
{
	remove_junk();
	sigchain_निकालो(signo);
	उठाओ(signo);
}

अटल काष्ठा ref *find_remote_branch(स्थिर काष्ठा ref *refs, स्थिर अक्षर *branch)
{
	काष्ठा ref *ref;
	काष्ठा strbuf शीर्ष = STRBUF_INIT;
	strbuf_addstr(&शीर्ष, "refs/heads/");
	strbuf_addstr(&शीर्ष, branch);
	ref = find_ref_by_name(refs, शीर्ष.buf);
	strbuf_release(&शीर्ष);

	अगर (ref)
		वापस ref;

	strbuf_addstr(&शीर्ष, "refs/tags/");
	strbuf_addstr(&शीर्ष, branch);
	ref = find_ref_by_name(refs, शीर्ष.buf);
	strbuf_release(&शीर्ष);

	वापस ref;
}

अटल काष्ठा ref *wanted_peer_refs(काष्ठा clone_opts *opts,
				    स्थिर काष्ठा ref *refs,
				    काष्ठा refspec *refspec)
{
	काष्ठा ref *local_refs = शून्य;
	काष्ठा ref **पुच्छ = &local_refs;
	काष्ठा ref *to_free = शून्य;

	अगर (opts->wants_शीर्ष) {
		काष्ठा ref *शीर्ष = copy_ref(find_ref_by_name(refs, "HEAD"));
		अगर (शीर्ष)
			पुच्छ_link_ref(शीर्ष, &पुच्छ);
		अगर (option_single_branch)
			refs = to_free =
				guess_remote_शीर्ष(शीर्ष, refs,
						  REMOTE_GUESS_शीर्ष_QUIET);
	} अन्यथा अगर (option_single_branch) {
		local_refs = शून्य;
		पुच्छ = &local_refs;
		refs = to_free = copy_ref(find_remote_branch(refs, option_branch));
	}

	क्रम (माप_प्रकार i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->वस्तुएँ[i], &पुच्छ, 0);

	free_one_ref(to_free);

	वापस local_refs;
}

अटल व्योम write_remote_refs(स्थिर काष्ठा ref *local_refs)
{
	स्थिर काष्ठा ref *r;

	काष्ठा ref_transaction *t;
	काष्ठा strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	अगर (!t)
		die("%s", err.buf);

	क्रम (r = local_refs; r; r = r->अग्र) {
		अगर (!r->peer_ref)
			जारी;
		अगर (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   शून्य, 0, शून्य, &err))
			die("%s", err.buf);
	}

	अगर (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

अटल व्योम write_followtags(स्थिर काष्ठा ref *refs, स्थिर अक्षर *संदेश)
{
	स्थिर काष्ठा ref *ref;
	क्रम (ref = refs; ref; ref = ref->अग्र) {
		अगर (!starts_with(ref->name, "refs/tags/"))
			जारी;
		अगर (ends_with(ref->name, "^{}"))
			जारी;
		अगर (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			जारी;
		refs_update_ref(get_main_ref_store(the_repository), संदेश,
				ref->name, &ref->old_oid, शून्य, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

अटल स्थिर काष्ठा object_id *iterate_ref_map(व्योम *cb_तथ्य)
{
	काष्ठा ref **rm = cb_तथ्य;
	काष्ठा ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	जबतक (ref && !ref->peer_ref)
		ref = ref->अग्र;
	अगर (!ref)
		वापस शून्य;

	*rm = ref->अग्र;
	वापस &ref->old_oid;
}

अटल व्योम update_remote_refs(स्थिर काष्ठा ref *refs,
			       स्थिर काष्ठा ref *mapped_refs,
			       स्थिर काष्ठा ref *remote_शीर्ष_points_at,
			       स्थिर अक्षर *branch_शीर्ष,
			       स्थिर अक्षर *संदेश,
			       काष्ठा transport *transport,
			       पूर्णांक check_connectivity)
{
	स्थिर काष्ठा ref *rm = mapped_refs;

	अगर (check_connectivity) {
		काष्ठा check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.progress = transport->progress;

		अगर (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	अगर (refs) {
		write_remote_refs(mapped_refs);
		अगर (option_single_branch && option_tags)
			write_followtags(refs, संदेश);
	}

	अगर (remote_शीर्ष_points_at && remote_शीर्ष_points_at->peer_ref && !option_bare) {
		काष्ठा strbuf शीर्ष_ref = STRBUF_INIT;
		strbuf_addstr(&शीर्ष_ref, branch_शीर्ष);
		strbuf_addstr(&शीर्ष_ref, "HEAD");
		अगर (refs_update_symref(get_main_ref_store(the_repository), शीर्ष_ref.buf,
				       remote_शीर्ष_points_at->peer_ref->name,
				       संदेश) < 0)
			die(_("unable to update %s"), शीर्ष_ref.buf);
		strbuf_release(&शीर्ष_ref);
	}
}

अटल व्योम update_शीर्ष(काष्ठा clone_opts *opts, स्थिर काष्ठा ref *our, स्थिर काष्ठा ref *remote,
			स्थिर अक्षर *unborn, स्थिर अक्षर *संदेश)
{
	स्थिर अक्षर *शीर्ष;
	अगर (our && !opts->detach && skip_prefix(our->name, "refs/heads/", &शीर्ष)) {
		/* Local default branch link */
		अगर (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, शून्य) < 0)
			die(_("unable to update HEAD"));
		अगर (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					संदेश, "HEAD", &our->old_oid, शून्य, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, शीर्ष, remote_name, our->name);
		}
	} अन्यथा अगर (our) {
		काष्ठा commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), संदेश,
				"HEAD", &c->object.oid, शून्य, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} अन्यथा अगर (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), संदेश,
				"HEAD", &remote->old_oid, शून्य, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} अन्यथा अगर (unborn && skip_prefix(unborn, "refs/heads/", &शीर्ष)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		अगर (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, शून्य) < 0)
			die(_("unable to update HEAD"));
		अगर (!option_bare)
			install_branch_config(0, शीर्ष, remote_name, unborn);
	}
}

अटल पूर्णांक git_sparse_checkout_init(स्थिर अक्षर *repo)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;
	पूर्णांक परिणाम = 0;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	strvec_pushl(&cmd.args, "-C", repo, "sparse-checkout", "set", शून्य);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	अगर (run_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		परिणाम = 1;
	}

	वापस परिणाम;
}

अटल पूर्णांक checkout(पूर्णांक submodule_progress,
		    काष्ठा list_objects_filter_options *filter_options,
		    पूर्णांक filter_submodules,
		    क्रमागत ref_storage_format ref_storage_format)
{
	काष्ठा object_id oid;
	अक्षर *शीर्ष;
	काष्ठा lock_file lock_file = LOCK_INIT;
	काष्ठा unpack_trees_options opts;
	काष्ठा वृक्ष *वृक्ष;
	काष्ठा वृक्ष_desc t;
	पूर्णांक err = 0;
	काष्ठा run_hooks_opt hook_opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	अगर (option_no_checkout)
		वापस 0;

	शीर्ष = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, शून्य);
	अगर (!शीर्ष) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		वापस 0;
	}
	अगर (!म_भेद(शीर्ष, "HEAD")) {
		अगर (advice_enabled(ADVICE_DETACHED_शीर्ष))
			detach_advice(oid_to_hex(&oid));
		FREE_AND_NULL(शीर्ष);
	} अन्यथा {
		अगर (!starts_with(शीर्ष, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	setup_work_वृक्ष(the_repository);

	repo_hold_locked_सूचकांक(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	स_रखो(&opts, 0, माप opts);
	opts.update = 1;
	opts.merge = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_merge;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
	init_checkout_metadata(&opts.meta, शीर्ष, &oid, शून्य);

	वृक्ष = repo_parse_वृक्ष_indirect(the_repository, &oid);
	अगर (!वृक्ष)
		die(_("unable to parse commit %s"), oid_to_hex(&oid));
	अगर (repo_parse_वृक्ष(the_repository, वृक्ष) < 0)
		निकास(128);
	init_वृक्ष_desc(&t, &वृक्ष->object.oid, वृक्ष->बफर, वृक्ष->आकार);
	अगर (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	मुक्त(शीर्ष);

	अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.args, oid_to_hex(null_oid(the_hash_algo)),
		     oid_to_hex(&oid), "1", शून्य);
	err |= run_hooks_opt(the_repository, "post-checkout", &hook_opt);

	अगर (!err && (option_recurse_submodules.nr > 0)) {
		काष्ठा child_process cmd = CHILD_PROCESS_INIT;
		strvec_pushl(&cmd.args, "submodule", "update", "--require-init",
			     "--recursive", शून्य);

		अगर (option_shallow_submodules == 1)
			strvec_धकेलो(&cmd.args, "--depth=1");

		अगर (max_jobs != -1)
			strvec_pushf(&cmd.args, "--jobs=%d", max_jobs);

		अगर (submodule_progress)
			strvec_धकेलो(&cmd.args, "--progress");

		अगर (option_verbosity < 0)
			strvec_धकेलो(&cmd.args, "--quiet");

		अगर (option_remote_submodules) {
			strvec_धकेलो(&cmd.args, "--remote");
			strvec_धकेलो(&cmd.args, "--no-fetch");
		}

		अगर (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(ref_storage_format));

		अगर (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.args, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		अगर (option_single_branch >= 0)
			strvec_धकेलो(&cmd.args, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = run_command(&cmd);
	}

	वापस err;
}

अटल पूर्णांक git_clone_config(स्थिर अक्षर *k, स्थिर अक्षर *v,
			    स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(k, "clone.defaultremotename")) {
		अगर (!v)
			वापस config_error_nonbool(k);
		मुक्त(remote_name);
		remote_name = xstrdup(v);
	}
	अगर (!म_भेद(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	अगर (!म_भेद(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	वापस git_default_config(k, v, ctx, cb);
}

अटल पूर्णांक write_one_config(स्थिर अक्षर *कुंजी, स्थिर अक्षर *मूल्य,
			    स्थिर काष्ठा config_context *ctx,
			    व्योम *तथ्य)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	पूर्णांक apply_failed = git_clone_config(कुंजी, मूल्य, ctx, तथ्य);
	अगर (apply_failed)
		वापस apply_failed;

	वापस repo_config_set_multivar_gently(the_repository, कुंजी,
					       मूल्य ? मूल्य : "true",
					       CONFIG_REGEX_NONE, 0);
}

अटल व्योम write_config(काष्ठा string_list *config)
{
	पूर्णांक i;

	क्रम (i = 0; i < config->nr; i++) {
		अगर (git_config_parse_parameter(config->वस्तुएँ[i].string,
					       write_one_config, शून्य) < 0)
			die(_("unable to write parameters to config file"));
	}
}

अटल व्योम write_refspec_config(स्थिर अक्षर *src_ref_prefix,
		स्थिर काष्ठा ref *our_शीर्ष_points_at,
		स्थिर काष्ठा ref *remote_शीर्ष_points_at,
		काष्ठा strbuf *branch_शीर्ष)
{
	काष्ठा strbuf कुंजी = STRBUF_INIT;
	काष्ठा strbuf मूल्य = STRBUF_INIT;

	अगर (option_mirror || !option_bare) {
		अगर (option_single_branch && !option_mirror) {
			अगर (option_branch) {
				अगर (starts_with(our_शीर्ष_points_at->name, "refs/tags/"))
					strbuf_addf(&मूल्य, "+%s:%s", our_शीर्ष_points_at->name,
						our_शीर्ष_points_at->name);
				अन्यथा
					strbuf_addf(&मूल्य, "+%s:%s%s", our_शीर्ष_points_at->name,
						branch_शीर्ष->buf, option_branch);
			} अन्यथा अगर (remote_शीर्ष_points_at) {
				स्थिर अक्षर *शीर्ष = remote_शीर्ष_points_at->name;
				अगर (!skip_prefix(शीर्ष, "refs/heads/", &शीर्ष))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&मूल्य, "+%s:%s%s", remote_शीर्ष_points_at->name,
						branch_शीर्ष->buf, शीर्ष);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} अन्यथा {
			strbuf_addf(&मूल्य, "+%s*:%s*", src_ref_prefix, branch_शीर्ष->buf);
		}
		/* Configure the remote */
		अगर (मूल्य.len) {
			strbuf_addf(&कुंजी, "remote.%s.fetch", remote_name);
			repo_config_set_multivar(the_repository, कुंजी.buf, मूल्य.buf, "^$", 0);
			strbuf_reset(&कुंजी);

			अगर (option_mirror) {
				strbuf_addf(&कुंजी, "remote.%s.mirror", remote_name);
				repo_config_set(the_repository, कुंजी.buf, "true");
				strbuf_reset(&कुंजी);
			}
		}
	}

	strbuf_release(&कुंजी);
	strbuf_release(&मूल्य);
}

अटल व्योम dissociate_from_references(व्योम)
{
	अक्षर *alternates = repo_git_path(the_repository, "objects/info/alternates");

	अगर (!access(alternates, F_OK)) {
		काष्ठा child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.args, "repack", "-a", "-d", शून्य);
		अगर (run_command(&cmd))
			die(_("cannot repack to clean up"));
		अगर (unlink(alternates) && त्रुटि_सं != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	मुक्त(alternates);
}

अटल पूर्णांक path_exists(स्थिर अक्षर *path)
{
	काष्ठा stat sb;
	वापस !stat(path, &sb);
}

पूर्णांक cmd_clone(पूर्णांक argc,
	      स्थिर अक्षर **argv,
	      स्थिर अक्षर *prefix,
	      काष्ठा repository *repository UNUSED)
{
	पूर्णांक is_bundle = 0, is_local;
	पूर्णांक reject_shallow = 0;
	स्थिर अक्षर *repo_name, *repo, *work_वृक्ष, *git_dir;
	अक्षर *repo_to_free = शून्य;
	अक्षर *path = शून्य, *dir, *display_repo = शून्य;
	पूर्णांक dest_exists, real_dest_exists = 0;
	स्थिर काष्ठा ref *refs, *remote_शीर्ष;
	काष्ठा ref *remote_शीर्ष_points_at = शून्य;
	स्थिर काष्ठा ref *our_शीर्ष_points_at;
	अक्षर *unborn_शीर्ष = शून्य;
	काष्ठा ref *mapped_refs = शून्य;
	स्थिर काष्ठा ref *ref;
	काष्ठा strbuf कुंजी = STRBUF_INIT;
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा strbuf branch_शीर्ष = STRBUF_INIT, reflog_संदेश = STRBUF_INIT;
	काष्ठा transport *transport = शून्य;
	स्थिर अक्षर *src_ref_prefix = "refs/heads/";
	काष्ठा remote *remote;
	पूर्णांक err = 0, complete_refs_before_fetch = 1;
	पूर्णांक submodule_progress;
	पूर्णांक filter_submodules = 0;
	पूर्णांक hash_algo;
	क्रमागत ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	स्थिर पूर्णांक do_not_override_repo_unix_permissions = -1;
	पूर्णांक option_reject_shallow = -1; /* unspecified */
	पूर्णांक deepen = 0;
	अक्षर *option_template = शून्य, *option_depth = शून्य, *option_since = शून्य;
	अक्षर *option_origin = शून्य;
	काष्ठा string_list option_not = STRING_LIST_INIT_NODUP;
	स्थिर अक्षर *real_git_dir = शून्य;
	स्थिर अक्षर *ref_storage_format_uri = शून्य;
	स्थिर अक्षर *option_upload_pack = "git-upload-pack";
	पूर्णांक option_progress = -1;
	पूर्णांक option_sparse_checkout = 0;
	क्रमागत transport_family family = TRANSPORT_FAMILY_ALL;
	काष्ठा string_list option_config = STRING_LIST_INIT_DUP;
	पूर्णांक option_dissociate = 0;
	काष्ठा list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	पूर्णांक option_filter_submodules = -1; /* unspecified */
	काष्ठा string_list server_options = STRING_LIST_INIT_NODUP;
	स्थिर अक्षर *bundle_uri = शून्य;
	अक्षर *option_rev = शून्य;

	काष्ठा clone_opts opts = CLONE_OPTS_INIT;

	काष्ठा transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	काष्ठा option builtin_clone_options[] = {
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
			.मूल्य = &option_recurse_submodules,
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
	काष्ठा strvec alternates = STRVEC_INIT;

	स्थिर अक्षर * स्थिर builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		शून्य
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_identity("clone");

	repo_config(the_repository, git_clone_config, शून्य);

	argc = parse_options(argc, argv, prefix, builtin_clone_options,
			     builtin_clone_usage, 0);

	अगर (argc > 2)
		usage_संदेश_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	अगर (argc == 0)
		usage_संदेश_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	अगर (option_depth || option_since || option_not.nr)
		deepen = 1;
	अगर (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	अगर (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, शून्य);
		अगर (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	अगर (option_mirror) {
		option_bare = 1;
		option_tags = 0;
	}

	अगर (option_bare) {
		अगर (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	अगर (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	path = get_repo_path(repo_name, &is_bundle);
	अगर (path) {
		FREE_AND_NULL(path);
		repo = repo_to_free = absolute_pathdup(repo_name);
	} अन्यथा अगर (म_अक्षर(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} अन्यथा
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	अगर (option_depth && म_से_प(option_depth) < 1)
		die(_("depth %s is not a positive number"), option_depth);

	अगर (argc == 2)
		dir = xstrdup(argv[1]);
	अन्यथा
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = path_exists(dir);
	अगर (dest_exists && !is_empty_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	अगर (real_git_dir) {
		real_dest_exists = path_exists(real_git_dir);
		अगर (real_dest_exists && !is_empty_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_संदेश, "clone: from %s",
		    display_repo ? display_repo : repo);
	मुक्त(display_repo);

	अगर (option_bare)
		work_वृक्ष = शून्य;
	अन्यथा {
		work_वृक्ष = दो_पर्या("GIT_WORK_TREE");
		अगर (work_वृक्ष && path_exists(work_वृक्ष))
			die(_("working tree '%s' already exists."), work_वृक्ष);
	}

	अगर (option_bare || work_वृक्ष)
		git_dir = xstrdup(dir);
	अन्यथा {
		work_वृक्ष = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	निकास_पर(remove_junk);
	sigchain_धकेलो_common(remove_junk_on_signal);

	अगर (!option_bare) {
		अगर (safe_create_leading_directories_const(the_repository, work_वृक्ष) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_वृक्ष);
		अगर (dest_exists)
			junk_work_वृक्ष_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		अन्यथा अगर (सूची_गढ़ो(work_वृक्ष, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_वृक्ष);
		junk_work_वृक्ष = work_वृक्ष;
	}

	अगर (real_git_dir) {
		अगर (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} अन्यथा {
		अगर (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	अगर (safe_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	अगर (0 <= option_verbosity) {
		अगर (option_bare)
			ख_लिखो(मानक_त्रुटि, _("Cloning into bare repository '%s'...\n"), dir);
		अन्यथा
			ख_लिखो(मानक_त्रुटि, _("Cloning into '%s'...\n"), dir);
	}

	अगर (option_recurse_submodules.nr > 0) {
		काष्ठा string_list_item *item;
		काष्ठा strbuf sb = STRBUF_INIT;
		पूर्णांक val;

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

		अगर (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		अगर (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		अन्यथा अगर (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} अन्यथा अगर (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_वृक्ष,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, शून्य);

	अगर (real_git_dir) {
		मुक्त((अक्षर *)git_dir);
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

	refs_create_refdir_stubs(the_repository, git_dir, शून्य);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, शून्य);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	अगर (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	अगर (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	अगर (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	अगर (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	अगर (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	अगर (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	अगर (option_origin) {
		मुक्त(remote_name);
		remote_name = xstrdup(option_origin);
	}

	अगर (!remote_name)
		remote_name = xstrdup("origin");

	अगर (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	अगर (option_bare) {
		अगर (option_mirror)
			src_ref_prefix = "refs/";
		strbuf_addstr(&branch_शीर्ष, src_ref_prefix);

		repo_config_set(the_repository, "core.bare", "true");
	} अन्यथा अगर (!option_rev) {
		strbuf_addf(&branch_शीर्ष, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&कुंजी, "remote.%s.url", remote_name);
	repo_config_set(the_repository, कुंजी.buf, repo);
	strbuf_reset(&कुंजी);

	अगर (!option_tags) {
		strbuf_addf(&कुंजी, "remote.%s.tagOpt", remote_name);
		repo_config_set(the_repository, कुंजी.buf, "--no-tags");
		strbuf_reset(&कुंजी);
	}

	remote = remote_get_early(remote_name);

	अगर (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_prefix,
				branch_शीर्ष.buf);

	path = get_repo_path(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && path && !is_bundle;
	अगर (is_local) {
		अगर (option_depth)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		अगर (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		अगर (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		अगर (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		अगर (!access(mkpath("%s/shallow", path), F_OK)) {
			अगर (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			अगर (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	अगर (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? path : शून्य);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, path ? path : remote->url.v[0]);
	transport_set_verbosity(transport, option_verbosity, option_progress);
	transport->family = family;
	transport->cloning = 1;

	अगर (is_bundle) {
		काष्ठा bundle_header header = BUNDLE_HEADER_INIT;
		पूर्णांक fd = read_bundle_header(path, &header);
		पूर्णांक has_filter = header.filter.choice != LOFC_DISABLED;

		अगर (fd > 0)
			बंद(fd);
		bundle_header_release(&header);
		अगर (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_set_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	अगर (reject_shallow)
		transport_set_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	अगर (option_depth)
		transport_set_option(transport, TRANS_OPT_DEPTH,
				     option_depth);
	अगर (option_since)
		transport_set_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	अगर (option_not.nr)
		transport_set_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (स्थिर अक्षर *)&option_not);
	अगर (option_single_branch) {
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		अगर (option_branch)
			opts.wants_शीर्ष = 0;
	}

	अगर (option_upload_pack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	अगर (server_options.nr)
		transport->server_options = &server_options;

	अगर (filter_options.choice) {
		स्थिर अक्षर *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	अगर (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	अगर (option_rev) {
		option_tags = 0;
		option_single_branch = 0;
		opts.wants_शीर्ष = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	अगर (option_tags || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, TAG_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	अगर (option_branch)
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
	अगर (opts.wants_शीर्ष || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_धकेलो(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_version(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_set_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, शून्य, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	अगर (bundle_uri) {
		काष्ठा remote_state *state;
		पूर्णांक has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = शून्य;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		अगर (repo_init(the_repository, git_dir, work_वृक्ष))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		अन्यथा अगर (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		अन्यथा अगर (has_heuristic)
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

		अगर (transport->bundles &&
		    hashmap_get_आकार(&transport->bundles->bundles)) {
			काष्ठा remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = शून्य;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			अगर (repo_init(the_repository, git_dir, work_वृक्ष))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			अन्यथा अगर (fetch_bundle_list(the_repository,
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

	अगर (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	अगर (mapped_refs) {
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
		क्रम (ref = refs; ref; ref = ref->अग्र)
			अगर (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				अवरोध;
			}

		अगर (!is_local && !complete_refs_before_fetch) {
			अगर (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_शीर्ष = find_ref_by_name(refs, "HEAD");
	remote_शीर्ष_points_at = guess_remote_शीर्ष(remote_शीर्ष, mapped_refs,
						  REMOTE_GUESS_शीर्ष_QUIET);

	अगर (option_branch) {
		our_शीर्ष_points_at = find_remote_branch(mapped_refs, option_branch);
		अगर (!our_शीर्ष_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} अन्यथा अगर (option_rev) {
		our_शीर्ष_points_at = mapped_refs;
		अगर (!our_शीर्ष_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} अन्यथा अगर (remote_शीर्ष_points_at) {
		our_शीर्ष_points_at = remote_शीर्ष_points_at;
	} अन्यथा अगर (remote_शीर्ष) {
		our_शीर्ष_points_at = शून्य;
	} अन्यथा {
		अक्षर *to_free = शून्य;
		स्थिर अक्षर *branch;

		अगर (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		अगर (transport_ls_refs_options.unborn_शीर्ष_target &&
		    skip_prefix(transport_ls_refs_options.unborn_शीर्ष_target,
				"refs/heads/", &branch)) {
			unborn_शीर्ष  = xstrdup(transport_ls_refs_options.unborn_शीर्ष_target);
		} अन्यथा {
			branch = to_free = repo_default_branch_name(the_repository, 0);
			unborn_शीर्ष = xstrfmt("refs/heads/%s", branch);
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
		our_शीर्ष_points_at = find_remote_branch(mapped_refs, branch);

		मुक्त(to_free);
	}

	अगर (!option_rev)
		write_refspec_config(src_ref_prefix, our_शीर्ष_points_at,
				     remote_शीर्ष_points_at, &branch_शीर्ष);

	अगर (filter_options.choice)
		partial_clone_register(remote_name, &filter_options);

	अगर (is_local)
		clone_local(path, git_dir);
	अन्यथा अगर (mapped_refs && complete_refs_before_fetch) {
		अगर (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_शीर्ष_points_at,
			   branch_शीर्ष.buf, reflog_संदेश.buf, transport,
			   !is_local);

	update_शीर्ष(&opts, our_शीर्ष_points_at, remote_शीर्ष, unborn_शीर्ष, reflog_संदेश.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_progress = transport->progress;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	अगर (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	अगर (option_sparse_checkout && git_sparse_checkout_init(dir))
		वापस 1;

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
	strbuf_release(&reflog_संदेश);
	strbuf_release(&branch_शीर्ष);
	strbuf_release(&buf);
	strbuf_release(&कुंजी);
	free_refs(mapped_refs);
	free_refs(remote_शीर्ष_points_at);
	मुक्त(unborn_शीर्ष);
	मुक्त(dir);
	मुक्त(path);
	मुक्त(repo_to_free);
	junk_mode = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	वापस err;
}
