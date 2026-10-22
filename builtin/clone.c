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

#അന്തര്ഭാവ "builtin.h"

#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "copy.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "refspec.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "tree-walk.h"
#അന്തര്ഭാവ "unpack-trees.h"
#അന്തര്ഭാവ "transport.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "dir-iterator.h"
#അന്തര്ഭാവ "iterator.h"
#അന്തര്ഭാവ "sigchain.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "connected.h"
#അന്തര്ഭാവ "packfile.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "pkt-line.h"
#അന്തര്ഭാവ "list-objects-filter-options.h"
#അന്തര്ഭാവ "hook.h"
#അന്തര്ഭാവ "bundle.h"
#അന്തര്ഭാവ "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

ഘടന clone_opts {
	പൂർണ്ണസംഖ്യ wants_head;
	പൂർണ്ണസംഖ്യ detach;
};
#define CLONE_OPTS_INIT { \
	.wants_head = 1 /* default enabled */ \
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ option_local = -1, option_no_hardlinks, option_shared;
സ്ഥിതം പൂർണ്ണസംഖ്യ option_tags = 1; /* default enabled */
സ്ഥിതം പൂർണ്ണസംഖ്യ option_shallow_submodules;
സ്ഥിതം പൂർണ്ണസംഖ്യ config_reject_shallow = -1;    /* unspecified */
സ്ഥിതം അക്ഷരം *remote_name = ശൂന്യ;
സ്ഥിതം അക്ഷരം *option_branch = ശൂന്യ;
സ്ഥിതം പൂർണ്ണസംഖ്യ option_verbosity;
സ്ഥിതം ഘടന string_list option_required_reference = STRING_LIST_INIT_NODUP;
സ്ഥിതം ഘടന string_list option_optional_reference = STRING_LIST_INIT_NODUP;
സ്ഥിതം പൂർണ്ണസംഖ്യ max_jobs = -1;
സ്ഥിതം ഘടന string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
സ്ഥിതം പൂർണ്ണസംഖ്യ config_filter_submodules = -1;    /* unspecified */
സ്ഥിതം പൂർണ്ണസംഖ്യ option_remote_submodules;

സ്ഥിതം പൂർണ്ണസംഖ്യ recurse_submodules_cb(സ്ഥിരം ഘടന option *opt,
				 സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	എങ്കിൽ (unset)
		string_list_clear((ഘടന string_list *)opt->മൂല്യം, 0);
	അല്ലെങ്കിൽ എങ്കിൽ (arg)
		string_list_append((ഘടന string_list *)opt->മൂല്യം, arg);
	അല്ലെങ്കിൽ
		string_list_append((ഘടന string_list *)opt->മൂല്യം,
				   (സ്ഥിരം അക്ഷരം *)opt->defval);

	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_repo_path_1(ഘടന strbuf *path, പൂർണ്ണസംഖ്യ *is_bundle)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം *suffix[] = { "/.git", "", ".git/.git", ".git" };
	സ്ഥിതം സ്ഥിരം അക്ഷരം *bundle_suffix[] = { ".bundle", "" };
	size_t baselen = path->len;
	ഘടന stat st;
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < ARRAY_SIZE(suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, suffix[i]);
		എങ്കിൽ (stat(path->buf, &st))
			തുടരുക;
		എങ്കിൽ (S_ISDIR(st.st_mode) && is_git_directory(path->buf)) {
			*is_bundle = 0;
			തിരികെ path->buf;
		} അല്ലെങ്കിൽ എങ്കിൽ (S_ISREG(st.st_mode) && st.st_size > 8) {
			/* Is it a "gitfile"? */
			അക്ഷരം signature[8];
			സ്ഥിരം അക്ഷരം *dst;
			പൂർണ്ണസംഖ്യ len, fd = open(path->buf, O_RDONLY);
			എങ്കിൽ (fd < 0)
				തുടരുക;
			len = read_in_full(fd, signature, 8);
			close(fd);
			എങ്കിൽ (len != 8 || strncmp(signature, "gitdir: ", 8))
				തുടരുക;
			dst = read_gitfile(path->buf);
			എങ്കിൽ (dst) {
				*is_bundle = 0;
				തിരികെ dst;
			}
		}
	}

	വേണ്ടി (i = 0; i < ARRAY_SIZE(bundle_suffix); i++) {
		strbuf_setlen(path, baselen);
		strbuf_addstr(path, bundle_suffix[i]);
		എങ്കിൽ (!stat(path->buf, &st) && S_ISREG(st.st_mode)) {
			*is_bundle = 1;
			തിരികെ path->buf;
		}
	}

	തിരികെ ശൂന്യ;
}

സ്ഥിതം അക്ഷരം *get_repo_path(സ്ഥിരം അക്ഷരം *repo, പൂർണ്ണസംഖ്യ *is_bundle)
{
	ഘടന strbuf path = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *raw;
	അക്ഷരം *canon;

	strbuf_addstr(&path, repo);
	raw = get_repo_path_1(&path, is_bundle);
	canon = raw ? absolute_pathdup(raw) : ശൂന്യ;
	strbuf_release(&path);
	തിരികെ canon;
}

ഘടന collect_alternates_data {
	ഘടന strvec *alternates;
	bool required;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ collect_one_alternate(ഘടന string_list_item *item, ശൂന്യം *cb_data)
{
	ഘടന collect_alternates_data *data = cb_data;
	ഘടന strbuf err = STRBUF_INIT;
	അക്ഷരം *ref_git = compute_alternate_path(item->string, &err);

	എങ്കിൽ (!ref_git) {
		എങ്കിൽ (data->required)
			die("%s", err.buf);
		അല്ലെങ്കിൽ
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				item->string, err.buf);
	} അല്ലെങ്കിൽ {
		strvec_pushf(data->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	സ്വതന്ത്രം(ref_git);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം read_alternates(ഘടന strvec *alternates, സ്ഥിരം അക്ഷരം *src_repo)
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
	ഘടന strbuf path = STRBUF_INIT;
	ഘടന strbuf വരി = STRBUF_INIT;

	strbuf_addf(&path, "%s/objects/info/alternates", src_repo);

	in = fopen(path.buf, "r");
	എങ്കിൽ (!in) {
		എങ്കിൽ (errno == ENOENT)
			പോകുക out;
		die_errno("could not read alternates file '%s'", path.buf);
	}

	ആകുമ്പോൾ (strbuf_getline(&വരി, in) != EOF) {
		അക്ഷരം *abs_path;
		എങ്കിൽ (!വരി.len || വരി.buf[0] == '#')
			തുടരുക;
		എങ്കിൽ (is_absolute_path(വരി.buf)) {
			strvec_തള്ളുക(alternates, വരി.buf);
			തുടരുക;
		}
		abs_path = mkpathdup("%s/objects/%s", src_repo, വരി.buf);
		എങ്കിൽ (!normalize_path_copy(abs_path, abs_path))
			strvec_തള്ളുക(alternates, abs_path);
		അല്ലെങ്കിൽ
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, വരി.buf);
		സ്വതന്ത്രം(abs_path);
	}

out:
	strbuf_release(&path);
	strbuf_release(&വരി);
	എങ്കിൽ (in)
		fclose(in);
}

സ്ഥിതം ശൂന്യം collect_alternates(ഘടന strvec *alternates,
			       സ്ഥിരം അക്ഷരം *local_source_repo)
{
	എങ്കിൽ (option_required_reference.nr || option_optional_reference.nr) {
		ഘടന collect_alternates_data data = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &data);
		data.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &data);
	}

	എങ്കിൽ (local_source_repo) {
		ഘടന strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_source_repo);
		എങ്കിൽ (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		അല്ലെങ്കിൽ
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

സ്ഥിതം ശൂന്യം mkdir_if_missing(സ്ഥിരം അക്ഷരം *pathname, mode_t mode)
{
	ഘടന stat st;

	എങ്കിൽ (!mkdir(pathname, mode))
		തിരികെ;

	എങ്കിൽ (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	അല്ലെങ്കിൽ എങ്കിൽ (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	അല്ലെങ്കിൽ എങ്കിൽ (!S_ISDIR(st.st_mode))
		die(_("%s exists and is not a directory"), pathname);
}

സ്ഥിതം ശൂന്യം copy_or_link_directory(ഘടന strbuf *src, ഘടന strbuf *dest,
				   സ്ഥിരം അക്ഷരം *src_repo)
{
	പൂർണ്ണസംഖ്യ src_len, dest_len;
	ഘടന dir_iterator *iter;
	പൂർണ്ണസംഖ്യ iter_status;

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
	die_upon_dubious_ownership(ശൂന്യ, ശൂന്യ, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	എങ്കിൽ (!iter) {
		എങ്കിൽ (errno == ENOTDIR) {
			പൂർണ്ണസംഖ്യ saved_errno = errno;
			ഘടന stat st;

			എങ്കിൽ (!lstat(src->buf, &st) && S_ISLNK(st.st_mode))
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

	ആകുമ്പോൾ ((iter_status = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_path);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_path);

		എങ്കിൽ (S_ISLNK(iter->st.st_mode))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_path);

		എങ്കിൽ (S_ISDIR(iter->st.st_mode)) {
			mkdir_if_missing(dest->buf, 0777);
			തുടരുക;
		}

		/* Alternates were already handled earlier. */
		എങ്കിൽ (!fspathcmp(iter->relative_path, "info/alternates"))
			തുടരുക;

		എങ്കിൽ (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		എങ്കിൽ (!option_no_hardlinks) {
			എങ്കിൽ (!link(src->buf, dest->buf)) {
				ഘടന stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				എങ്കിൽ (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				എങ്കിൽ (st.st_mode != iter->st.st_mode ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_size != iter->st.st_size ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				തുടരുക;
			}
			എങ്കിൽ (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		എങ്കിൽ (copy_file_with_time(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	എങ്കിൽ (iter_status != ITER_DONE) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

സ്ഥിതം ശൂന്യം clone_local(സ്ഥിരം അക്ഷരം *src_repo, സ്ഥിരം അക്ഷരം *dest_repo)
{
	എങ്കിൽ (!option_shared) {
		ഘടന strbuf src = STRBUF_INIT;
		ഘടന strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	എങ്കിൽ (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *junk_work_വൃക്ഷം;
സ്ഥിതം പൂർണ്ണസംഖ്യ junk_work_വൃക്ഷം_flags;
സ്ഥിതം സ്ഥിരം അക്ഷരം *junk_git_dir;
സ്ഥിതം പൂർണ്ണസംഖ്യ junk_git_dir_flags;
സ്ഥിതം എണ്ണൽ {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_mode = JUNK_LEAVE_NONE;

സ്ഥിതം സ്ഥിരം അക്ഷരം junk_leave_repo_സന്ദേശം[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

സ്ഥിതം ശൂന്യം remove_junk(ശൂന്യം)
{
	ഘടന strbuf sb = STRBUF_INIT;

	തിരഞ്ഞെടുപ്പ് (junk_mode) {
	അവസ്ഥ JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_സന്ദേശം));
		/* fall-through */
	അവസ്ഥ JUNK_LEAVE_ALL:
		തിരികെ;
	സ്ഥിരസ്ഥിതി:
		/* proceed to removal */
		നിർത്തുക;
	}

	എങ്കിൽ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	എങ്കിൽ (junk_work_വൃക്ഷം) {
		strbuf_addstr(&sb, junk_work_വൃക്ഷം);
		remove_dir_recursively(&sb, junk_work_വൃക്ഷം_flags);
	}
	strbuf_release(&sb);
}

സ്ഥിതം ശൂന്യം remove_junk_on_signal(പൂർണ്ണസംഖ്യ signo)
{
	remove_junk();
	sigchain_എടുക്കുക(signo);
	ഉയർത്തുക(signo);
}

സ്ഥിതം ഘടന ref *find_remote_branch(സ്ഥിരം ഘടന ref *refs, സ്ഥിരം അക്ഷരം *branch)
{
	ഘടന ref *ref;
	ഘടന strbuf head = STRBUF_INIT;
	strbuf_addstr(&head, "refs/heads/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	എങ്കിൽ (ref)
		തിരികെ ref;

	strbuf_addstr(&head, "refs/tags/");
	strbuf_addstr(&head, branch);
	ref = find_ref_by_name(refs, head.buf);
	strbuf_release(&head);

	തിരികെ ref;
}

സ്ഥിതം ഘടന ref *wanted_peer_refs(ഘടന clone_opts *opts,
				    സ്ഥിരം ഘടന ref *refs,
				    ഘടന refspec *refspec)
{
	ഘടന ref *local_refs = ശൂന്യ;
	ഘടന ref **tail = &local_refs;
	ഘടന ref *to_free = ശൂന്യ;

	എങ്കിൽ (opts->wants_head) {
		ഘടന ref *head = copy_ref(find_ref_by_name(refs, "HEAD"));
		എങ്കിൽ (head)
			tail_link_ref(head, &tail);
		എങ്കിൽ (option_single_branch)
			refs = to_free =
				guess_remote_head(head, refs,
						  REMOTE_GUESS_HEAD_QUIET);
	} അല്ലെങ്കിൽ എങ്കിൽ (option_single_branch) {
		local_refs = ശൂന്യ;
		tail = &local_refs;
		refs = to_free = copy_ref(find_remote_branch(refs, option_branch));
	}

	വേണ്ടി (size_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->ഇനങ്ങൾ[i], &tail, 0);

	free_one_ref(to_free);

	തിരികെ local_refs;
}

സ്ഥിതം ശൂന്യം write_remote_refs(സ്ഥിരം ഘടന ref *local_refs)
{
	സ്ഥിരം ഘടന ref *r;

	ഘടന ref_transaction *t;
	ഘടന strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	എങ്കിൽ (!t)
		die("%s", err.buf);

	വേണ്ടി (r = local_refs; r; r = r->next) {
		എങ്കിൽ (!r->peer_ref)
			തുടരുക;
		എങ്കിൽ (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   ശൂന്യ, 0, ശൂന്യ, &err))
			die("%s", err.buf);
	}

	എങ്കിൽ (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

സ്ഥിതം ശൂന്യം write_followtags(സ്ഥിരം ഘടന ref *refs, സ്ഥിരം അക്ഷരം *സന്ദേശം)
{
	സ്ഥിരം ഘടന ref *ref;
	വേണ്ടി (ref = refs; ref; ref = ref->next) {
		എങ്കിൽ (!starts_with(ref->name, "refs/tags/"))
			തുടരുക;
		എങ്കിൽ (ends_with(ref->name, "^{}"))
			തുടരുക;
		എങ്കിൽ (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			തുടരുക;
		refs_update_ref(get_main_ref_store(the_repository), സന്ദേശം,
				ref->name, &ref->old_oid, ശൂന്യ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

സ്ഥിതം സ്ഥിരം ഘടന object_id *iterate_ref_map(ശൂന്യം *cb_data)
{
	ഘടന ref **rm = cb_data;
	ഘടന ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	ആകുമ്പോൾ (ref && !ref->peer_ref)
		ref = ref->next;
	എങ്കിൽ (!ref)
		തിരികെ ശൂന്യ;

	*rm = ref->next;
	തിരികെ &ref->old_oid;
}

സ്ഥിതം ശൂന്യം update_remote_refs(സ്ഥിരം ഘടന ref *refs,
			       സ്ഥിരം ഘടന ref *mapped_refs,
			       സ്ഥിരം ഘടന ref *remote_head_points_at,
			       സ്ഥിരം അക്ഷരം *branch_മുകൾ,
			       സ്ഥിരം അക്ഷരം *സന്ദേശം,
			       ഘടന transport *transport,
			       പൂർണ്ണസംഖ്യ check_connectivity)
{
	സ്ഥിരം ഘടന ref *rm = mapped_refs;

	എങ്കിൽ (check_connectivity) {
		ഘടന check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.progress = transport->progress;

		എങ്കിൽ (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	എങ്കിൽ (refs) {
		write_remote_refs(mapped_refs);
		എങ്കിൽ (option_single_branch && option_tags)
			write_followtags(refs, സന്ദേശം);
	}

	എങ്കിൽ (remote_head_points_at && remote_head_points_at->peer_ref && !option_bare) {
		ഘടന strbuf head_ref = STRBUF_INIT;
		strbuf_addstr(&head_ref, branch_മുകൾ);
		strbuf_addstr(&head_ref, "HEAD");
		എങ്കിൽ (refs_update_symref(get_main_ref_store(the_repository), head_ref.buf,
				       remote_head_points_at->peer_ref->name,
				       സന്ദേശം) < 0)
			die(_("unable to update %s"), head_ref.buf);
		strbuf_release(&head_ref);
	}
}

സ്ഥിതം ശൂന്യം update_head(ഘടന clone_opts *opts, സ്ഥിരം ഘടന ref *our, സ്ഥിരം ഘടന ref *remote,
			സ്ഥിരം അക്ഷരം *unborn, സ്ഥിരം അക്ഷരം *സന്ദേശം)
{
	സ്ഥിരം അക്ഷരം *head;
	എങ്കിൽ (our && !opts->detach && skip_prefix(our->name, "refs/heads/", &head)) {
		/* Local default branch link */
		എങ്കിൽ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, ശൂന്യ) < 0)
			die(_("unable to update HEAD"));
		എങ്കിൽ (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					സന്ദേശം, "HEAD", &our->old_oid, ശൂന്യ, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, head, remote_name, our->name);
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (our) {
		ഘടന commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), സന്ദേശം,
				"HEAD", &c->object.oid, ശൂന്യ, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} അല്ലെങ്കിൽ എങ്കിൽ (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), സന്ദേശം,
				"HEAD", &remote->old_oid, ശൂന്യ, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} അല്ലെങ്കിൽ എങ്കിൽ (unborn && skip_prefix(unborn, "refs/heads/", &head)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		എങ്കിൽ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, ശൂന്യ) < 0)
			die(_("unable to update HEAD"));
		എങ്കിൽ (!option_bare)
			install_branch_config(0, head, remote_name, unborn);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_sparse_checkout_init(സ്ഥിരം അക്ഷരം *repo)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;
	പൂർണ്ണസംഖ്യ result = 0;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	strvec_pushl(&cmd.args, "-C", repo, "sparse-checkout", "set", ശൂന്യ);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	എങ്കിൽ (run_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		result = 1;
	}

	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout(പൂർണ്ണസംഖ്യ submodule_progress,
		    ഘടന list_objects_filter_options *filter_options,
		    പൂർണ്ണസംഖ്യ filter_submodules,
		    എണ്ണൽ ref_storage_format ref_storage_format)
{
	ഘടന object_id oid;
	അക്ഷരം *head;
	ഘടന lock_file lock_file = LOCK_INIT;
	ഘടന unpack_trees_options opts;
	ഘടന വൃക്ഷം *വൃക്ഷം;
	ഘടന വൃക്ഷം_desc t;
	പൂർണ്ണസംഖ്യ err = 0;
	ഘടന run_hooks_opt hook_opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	എങ്കിൽ (option_no_checkout)
		തിരികെ 0;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, ശൂന്യ);
	എങ്കിൽ (!head) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(head, "HEAD")) {
		എങ്കിൽ (advice_enabled(ADVICE_DETACHED_HEAD))
			detach_advice(oid_to_hex(&oid));
		FREE_AND_NULL(head);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (!starts_with(head, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	setup_work_വൃക്ഷം(the_repository);

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, വലുപ്പം opts);
	opts.update = 1;
	opts.merge = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_merge;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	init_checkout_metadata(&opts.meta, head, &oid, ശൂന്യ);

	വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository, &oid);
	എങ്കിൽ (!വൃക്ഷം)
		die(_("unable to parse commit %s"), oid_to_hex(&oid));
	എങ്കിൽ (repo_parse_വൃക്ഷം(the_repository, വൃക്ഷം) < 0)
		exit(128);
	init_വൃക്ഷം_desc(&t, &വൃക്ഷം->object.oid, വൃക്ഷം->buffer, വൃക്ഷം->size);
	എങ്കിൽ (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	സ്വതന്ത്രം(head);

	എങ്കിൽ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.args, oid_to_hex(null_oid(the_hash_algo)),
		     oid_to_hex(&oid), "1", ശൂന്യ);
	err |= run_hooks_opt(the_repository, "post-checkout", &hook_opt);

	എങ്കിൽ (!err && (option_recurse_submodules.nr > 0)) {
		ഘടന child_process cmd = CHILD_PROCESS_INIT;
		strvec_pushl(&cmd.args, "submodule", "update", "--require-init",
			     "--recursive", ശൂന്യ);

		എങ്കിൽ (option_shallow_submodules == 1)
			strvec_തള്ളുക(&cmd.args, "--depth=1");

		എങ്കിൽ (max_jobs != -1)
			strvec_pushf(&cmd.args, "--jobs=%d", max_jobs);

		എങ്കിൽ (submodule_progress)
			strvec_തള്ളുക(&cmd.args, "--progress");

		എങ്കിൽ (option_verbosity < 0)
			strvec_തള്ളുക(&cmd.args, "--quiet");

		എങ്കിൽ (option_remote_submodules) {
			strvec_തള്ളുക(&cmd.args, "--remote");
			strvec_തള്ളുക(&cmd.args, "--no-fetch");
		}

		എങ്കിൽ (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(ref_storage_format));

		എങ്കിൽ (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.args, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		എങ്കിൽ (option_single_branch >= 0)
			strvec_തള്ളുക(&cmd.args, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = run_command(&cmd);
	}

	തിരികെ err;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_clone_config(സ്ഥിരം അക്ഷരം *k, സ്ഥിരം അക്ഷരം *v,
			    സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(k, "clone.defaultremotename")) {
		എങ്കിൽ (!v)
			തിരികെ config_error_nonbool(k);
		സ്വതന്ത്രം(remote_name);
		remote_name = xstrdup(v);
	}
	എങ്കിൽ (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	എങ്കിൽ (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	തിരികെ git_default_config(k, v, ctx, cb);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ write_one_config(സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം അക്ഷരം *മൂല്യം,
			    സ്ഥിരം ഘടന config_context *ctx,
			    ശൂന്യം *data)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	പൂർണ്ണസംഖ്യ apply_failed = git_clone_config(താക്കോൽ, മൂല്യം, ctx, data);
	എങ്കിൽ (apply_failed)
		തിരികെ apply_failed;

	തിരികെ repo_config_set_multivar_gently(the_repository, താക്കോൽ,
					       മൂല്യം ? മൂല്യം : "true",
					       CONFIG_REGEX_NONE, 0);
}

സ്ഥിതം ശൂന്യം write_config(ഘടന string_list *config)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < config->nr; i++) {
		എങ്കിൽ (git_config_parse_parameter(config->ഇനങ്ങൾ[i].string,
					       write_one_config, ശൂന്യ) < 0)
			die(_("unable to write parameters to config file"));
	}
}

സ്ഥിതം ശൂന്യം write_refspec_config(സ്ഥിരം അക്ഷരം *src_ref_prefix,
		സ്ഥിരം ഘടന ref *our_head_points_at,
		സ്ഥിരം ഘടന ref *remote_head_points_at,
		ഘടന strbuf *branch_മുകൾ)
{
	ഘടന strbuf താക്കോൽ = STRBUF_INIT;
	ഘടന strbuf മൂല്യം = STRBUF_INIT;

	എങ്കിൽ (option_mirror || !option_bare) {
		എങ്കിൽ (option_single_branch && !option_mirror) {
			എങ്കിൽ (option_branch) {
				എങ്കിൽ (starts_with(our_head_points_at->name, "refs/tags/"))
					strbuf_addf(&മൂല്യം, "+%s:%s", our_head_points_at->name,
						our_head_points_at->name);
				അല്ലെങ്കിൽ
					strbuf_addf(&മൂല്യം, "+%s:%s%s", our_head_points_at->name,
						branch_മുകൾ->buf, option_branch);
			} അല്ലെങ്കിൽ എങ്കിൽ (remote_head_points_at) {
				സ്ഥിരം അക്ഷരം *head = remote_head_points_at->name;
				എങ്കിൽ (!skip_prefix(head, "refs/heads/", &head))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&മൂല്യം, "+%s:%s%s", remote_head_points_at->name,
						branch_മുകൾ->buf, head);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} അല്ലെങ്കിൽ {
			strbuf_addf(&മൂല്യം, "+%s*:%s*", src_ref_prefix, branch_മുകൾ->buf);
		}
		/* Configure the remote */
		എങ്കിൽ (മൂല്യം.len) {
			strbuf_addf(&താക്കോൽ, "remote.%s.fetch", remote_name);
			repo_config_set_multivar(the_repository, താക്കോൽ.buf, മൂല്യം.buf, "^$", 0);
			strbuf_reset(&താക്കോൽ);

			എങ്കിൽ (option_mirror) {
				strbuf_addf(&താക്കോൽ, "remote.%s.mirror", remote_name);
				repo_config_set(the_repository, താക്കോൽ.buf, "true");
				strbuf_reset(&താക്കോൽ);
			}
		}
	}

	strbuf_release(&താക്കോൽ);
	strbuf_release(&മൂല്യം);
}

സ്ഥിതം ശൂന്യം dissociate_from_references(ശൂന്യം)
{
	അക്ഷരം *alternates = repo_git_path(the_repository, "objects/info/alternates");

	എങ്കിൽ (!access(alternates, F_OK)) {
		ഘടന child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.args, "repack", "-a", "-d", ശൂന്യ);
		എങ്കിൽ (run_command(&cmd))
			die(_("cannot repack to clean up"));
		എങ്കിൽ (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	സ്വതന്ത്രം(alternates);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ path_exists(സ്ഥിരം അക്ഷരം *path)
{
	ഘടന stat sb;
	തിരികെ !stat(path, &sb);
}

പൂർണ്ണസംഖ്യ cmd_clone(പൂർണ്ണസംഖ്യ argc,
	      സ്ഥിരം അക്ഷരം **argv,
	      സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repository UNUSED)
{
	പൂർണ്ണസംഖ്യ is_bundle = 0, is_local;
	പൂർണ്ണസംഖ്യ reject_shallow = 0;
	സ്ഥിരം അക്ഷരം *repo_name, *repo, *work_വൃക്ഷം, *git_dir;
	അക്ഷരം *repo_to_free = ശൂന്യ;
	അക്ഷരം *path = ശൂന്യ, *dir, *display_repo = ശൂന്യ;
	പൂർണ്ണസംഖ്യ dest_exists, real_dest_exists = 0;
	സ്ഥിരം ഘടന ref *refs, *remote_head;
	ഘടന ref *remote_head_points_at = ശൂന്യ;
	സ്ഥിരം ഘടന ref *our_head_points_at;
	അക്ഷരം *unborn_head = ശൂന്യ;
	ഘടന ref *mapped_refs = ശൂന്യ;
	സ്ഥിരം ഘടന ref *ref;
	ഘടന strbuf താക്കോൽ = STRBUF_INIT;
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന strbuf branch_മുകൾ = STRBUF_INIT, reflog_സന്ദേശം = STRBUF_INIT;
	ഘടന transport *transport = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *src_ref_prefix = "refs/heads/";
	ഘടന remote *remote;
	പൂർണ്ണസംഖ്യ err = 0, complete_refs_before_fetch = 1;
	പൂർണ്ണസംഖ്യ submodule_progress;
	പൂർണ്ണസംഖ്യ filter_submodules = 0;
	പൂർണ്ണസംഖ്യ hash_algo;
	എണ്ണൽ ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	സ്ഥിരം പൂർണ്ണസംഖ്യ do_not_override_repo_unix_permissions = -1;
	പൂർണ്ണസംഖ്യ option_reject_shallow = -1; /* unspecified */
	പൂർണ്ണസംഖ്യ deepen = 0;
	അക്ഷരം *option_template = ശൂന്യ, *option_depth = ശൂന്യ, *option_since = ശൂന്യ;
	അക്ഷരം *option_origin = ശൂന്യ;
	ഘടന string_list option_not = STRING_LIST_INIT_NODUP;
	സ്ഥിരം അക്ഷരം *real_git_dir = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *ref_storage_format_uri = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *option_upload_pack = "git-upload-pack";
	പൂർണ്ണസംഖ്യ option_progress = -1;
	പൂർണ്ണസംഖ്യ option_sparse_checkout = 0;
	എണ്ണൽ transport_family family = TRANSPORT_FAMILY_ALL;
	ഘടന string_list option_config = STRING_LIST_INIT_DUP;
	പൂർണ്ണസംഖ്യ option_dissociate = 0;
	ഘടന list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	പൂർണ്ണസംഖ്യ option_filter_submodules = -1; /* unspecified */
	ഘടന string_list server_options = STRING_LIST_INIT_NODUP;
	സ്ഥിരം അക്ഷരം *bundle_uri = ശൂന്യ;
	അക്ഷരം *option_rev = ശൂന്യ;

	ഘടന clone_opts opts = CLONE_OPTS_INIT;

	ഘടന transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	ഘടന option builtin_clone_options[] = {
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
			.മൂല്യം = &option_recurse_submodules,
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
	ഘടന strvec alternates = STRVEC_INIT;

	സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		ശൂന്യ
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_identity("clone");

	repo_config(the_repository, git_clone_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, builtin_clone_options,
			     builtin_clone_usage, 0);

	എങ്കിൽ (argc > 2)
		usage_സന്ദേശം_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	എങ്കിൽ (argc == 0)
		usage_സന്ദേശം_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	എങ്കിൽ (option_depth || option_since || option_not.nr)
		deepen = 1;
	എങ്കിൽ (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	എങ്കിൽ (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, ശൂന്യ);
		എങ്കിൽ (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	എങ്കിൽ (option_mirror) {
		option_bare = 1;
		option_tags = 0;
	}

	എങ്കിൽ (option_bare) {
		എങ്കിൽ (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	എങ്കിൽ (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	path = get_repo_path(repo_name, &is_bundle);
	എങ്കിൽ (path) {
		FREE_AND_NULL(path);
		repo = repo_to_free = absolute_pathdup(repo_name);
	} അല്ലെങ്കിൽ എങ്കിൽ (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} അല്ലെങ്കിൽ
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	എങ്കിൽ (option_depth && atoi(option_depth) < 1)
		die(_("depth %s is not a positive number"), option_depth);

	എങ്കിൽ (argc == 2)
		dir = xstrdup(argv[1]);
	അല്ലെങ്കിൽ
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = path_exists(dir);
	എങ്കിൽ (dest_exists && !is_empty_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	എങ്കിൽ (real_git_dir) {
		real_dest_exists = path_exists(real_git_dir);
		എങ്കിൽ (real_dest_exists && !is_empty_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_സന്ദേശം, "clone: from %s",
		    display_repo ? display_repo : repo);
	സ്വതന്ത്രം(display_repo);

	എങ്കിൽ (option_bare)
		work_വൃക്ഷം = ശൂന്യ;
	അല്ലെങ്കിൽ {
		work_വൃക്ഷം = getenv("GIT_WORK_TREE");
		എങ്കിൽ (work_വൃക്ഷം && path_exists(work_വൃക്ഷം))
			die(_("working tree '%s' already exists."), work_വൃക്ഷം);
	}

	എങ്കിൽ (option_bare || work_വൃക്ഷം)
		git_dir = xstrdup(dir);
	അല്ലെങ്കിൽ {
		work_വൃക്ഷം = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_തള്ളുക_common(remove_junk_on_signal);

	എങ്കിൽ (!option_bare) {
		എങ്കിൽ (safe_create_leading_directories_const(the_repository, work_വൃക്ഷം) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_വൃക്ഷം);
		എങ്കിൽ (dest_exists)
			junk_work_വൃക്ഷം_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		അല്ലെങ്കിൽ എങ്കിൽ (mkdir(work_വൃക്ഷം, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_വൃക്ഷം);
		junk_work_വൃക്ഷം = work_വൃക്ഷം;
	}

	എങ്കിൽ (real_git_dir) {
		എങ്കിൽ (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	എങ്കിൽ (safe_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	എങ്കിൽ (0 <= option_verbosity) {
		എങ്കിൽ (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		അല്ലെങ്കിൽ
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	എങ്കിൽ (option_recurse_submodules.nr > 0) {
		ഘടന string_list_item *item;
		ഘടന strbuf sb = STRBUF_INIT;
		പൂർണ്ണസംഖ്യ val;

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

		എങ്കിൽ (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		എങ്കിൽ (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		അല്ലെങ്കിൽ എങ്കിൽ (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} അല്ലെങ്കിൽ എങ്കിൽ (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_വൃക്ഷം,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, ശൂന്യ);

	എങ്കിൽ (real_git_dir) {
		സ്വതന്ത്രം((അക്ഷരം *)git_dir);
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

	refs_create_refdir_stubs(the_repository, git_dir, ശൂന്യ);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, ശൂന്യ);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	എങ്കിൽ (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	എങ്കിൽ (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	എങ്കിൽ (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	എങ്കിൽ (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	എങ്കിൽ (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	എങ്കിൽ (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	എങ്കിൽ (option_origin) {
		സ്വതന്ത്രം(remote_name);
		remote_name = xstrdup(option_origin);
	}

	എങ്കിൽ (!remote_name)
		remote_name = xstrdup("origin");

	എങ്കിൽ (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	എങ്കിൽ (option_bare) {
		എങ്കിൽ (option_mirror)
			src_ref_prefix = "refs/";
		strbuf_addstr(&branch_മുകൾ, src_ref_prefix);

		repo_config_set(the_repository, "core.bare", "true");
	} അല്ലെങ്കിൽ എങ്കിൽ (!option_rev) {
		strbuf_addf(&branch_മുകൾ, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&താക്കോൽ, "remote.%s.url", remote_name);
	repo_config_set(the_repository, താക്കോൽ.buf, repo);
	strbuf_reset(&താക്കോൽ);

	എങ്കിൽ (!option_tags) {
		strbuf_addf(&താക്കോൽ, "remote.%s.tagOpt", remote_name);
		repo_config_set(the_repository, താക്കോൽ.buf, "--no-tags");
		strbuf_reset(&താക്കോൽ);
	}

	remote = remote_get_early(remote_name);

	എങ്കിൽ (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_prefix,
				branch_മുകൾ.buf);

	path = get_repo_path(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && path && !is_bundle;
	എങ്കിൽ (is_local) {
		എങ്കിൽ (option_depth)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		എങ്കിൽ (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		എങ്കിൽ (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		എങ്കിൽ (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		എങ്കിൽ (!access(mkpath("%s/shallow", path), F_OK)) {
			എങ്കിൽ (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			എങ്കിൽ (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	എങ്കിൽ (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? path : ശൂന്യ);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, path ? path : remote->url.v[0]);
	transport_set_verbosity(transport, option_verbosity, option_progress);
	transport->family = family;
	transport->cloning = 1;

	എങ്കിൽ (is_bundle) {
		ഘടന bundle_header header = BUNDLE_HEADER_INIT;
		പൂർണ്ണസംഖ്യ fd = read_bundle_header(path, &header);
		പൂർണ്ണസംഖ്യ has_filter = header.filter.choice != LOFC_DISABLED;

		എങ്കിൽ (fd > 0)
			close(fd);
		bundle_header_release(&header);
		എങ്കിൽ (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_set_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	എങ്കിൽ (reject_shallow)
		transport_set_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	എങ്കിൽ (option_depth)
		transport_set_option(transport, TRANS_OPT_DEPTH,
				     option_depth);
	എങ്കിൽ (option_since)
		transport_set_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	എങ്കിൽ (option_not.nr)
		transport_set_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (സ്ഥിരം അക്ഷരം *)&option_not);
	എങ്കിൽ (option_single_branch) {
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		എങ്കിൽ (option_branch)
			opts.wants_head = 0;
	}

	എങ്കിൽ (option_upload_pack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	എങ്കിൽ (server_options.nr)
		transport->server_options = &server_options;

	എങ്കിൽ (filter_options.choice) {
		സ്ഥിരം അക്ഷരം *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	എങ്കിൽ (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	എങ്കിൽ (option_rev) {
		option_tags = 0;
		option_single_branch = 0;
		opts.wants_head = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	എങ്കിൽ (option_tags || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, TAG_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	എങ്കിൽ (option_branch)
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
	എങ്കിൽ (opts.wants_head || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_തള്ളുക(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_version(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_set_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, ശൂന്യ, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	എങ്കിൽ (bundle_uri) {
		ഘടന remote_state *state;
		പൂർണ്ണസംഖ്യ has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = ശൂന്യ;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		എങ്കിൽ (repo_init(the_repository, git_dir, work_വൃക്ഷം))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		അല്ലെങ്കിൽ എങ്കിൽ (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		അല്ലെങ്കിൽ എങ്കിൽ (has_heuristic)
			repo_config_set_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		സ്വതന്ത്രം(the_repository->remote_state);
		the_repository->remote_state = state;
	} അല്ലെങ്കിൽ {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		എങ്കിൽ (transport->bundles &&
		    hashmap_get_size(&transport->bundles->bundles)) {
			ഘടന remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = ശൂന്യ;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			എങ്കിൽ (repo_init(the_repository, git_dir, work_വൃക്ഷം))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			അല്ലെങ്കിൽ എങ്കിൽ (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			സ്വതന്ത്രം(the_repository->remote_state);
			the_repository->remote_state = state;
		} അല്ലെങ്കിൽ {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	എങ്കിൽ (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	എങ്കിൽ (mapped_refs) {
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
		വേണ്ടി (ref = refs; ref; ref = ref->next)
			എങ്കിൽ (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				നിർത്തുക;
			}

		എങ്കിൽ (!is_local && !complete_refs_before_fetch) {
			എങ്കിൽ (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_head = find_ref_by_name(refs, "HEAD");
	remote_head_points_at = guess_remote_head(remote_head, mapped_refs,
						  REMOTE_GUESS_HEAD_QUIET);

	എങ്കിൽ (option_branch) {
		our_head_points_at = find_remote_branch(mapped_refs, option_branch);
		എങ്കിൽ (!our_head_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} അല്ലെങ്കിൽ എങ്കിൽ (option_rev) {
		our_head_points_at = mapped_refs;
		എങ്കിൽ (!our_head_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} അല്ലെങ്കിൽ എങ്കിൽ (remote_head_points_at) {
		our_head_points_at = remote_head_points_at;
	} അല്ലെങ്കിൽ എങ്കിൽ (remote_head) {
		our_head_points_at = ശൂന്യ;
	} അല്ലെങ്കിൽ {
		അക്ഷരം *to_free = ശൂന്യ;
		സ്ഥിരം അക്ഷരം *branch;

		എങ്കിൽ (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		എങ്കിൽ (transport_ls_refs_options.unborn_head_target &&
		    skip_prefix(transport_ls_refs_options.unborn_head_target,
				"refs/heads/", &branch)) {
			unborn_head  = xstrdup(transport_ls_refs_options.unborn_head_target);
		} അല്ലെങ്കിൽ {
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

		സ്വതന്ത്രം(to_free);
	}

	എങ്കിൽ (!option_rev)
		write_refspec_config(src_ref_prefix, our_head_points_at,
				     remote_head_points_at, &branch_മുകൾ);

	എങ്കിൽ (filter_options.choice)
		partial_clone_register(remote_name, &filter_options);

	എങ്കിൽ (is_local)
		clone_local(path, git_dir);
	അല്ലെങ്കിൽ എങ്കിൽ (mapped_refs && complete_refs_before_fetch) {
		എങ്കിൽ (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_head_points_at,
			   branch_മുകൾ.buf, reflog_സന്ദേശം.buf, transport,
			   !is_local);

	update_head(&opts, our_head_points_at, remote_head, unborn_head, reflog_സന്ദേശം.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_progress = transport->progress;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	എങ്കിൽ (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	എങ്കിൽ (option_sparse_checkout && git_sparse_checkout_init(dir))
		തിരികെ 1;

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

	സ്വതന്ത്രം(remote_name);
	strbuf_release(&reflog_സന്ദേശം);
	strbuf_release(&branch_മുകൾ);
	strbuf_release(&buf);
	strbuf_release(&താക്കോൽ);
	free_refs(mapped_refs);
	free_refs(remote_head_points_at);
	സ്വതന്ത്രം(unborn_head);
	സ്വതന്ത്രം(dir);
	സ്വതന്ത്രം(path);
	സ്വതന്ത്രം(repo_to_free);
	junk_mode = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	തിരികെ err;
}
