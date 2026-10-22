/*
 * Builtin "git clone"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *		 2008 Daniel Barkalow <barkalow@iabervon.org>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 *
 * Clone a repository into a different directory that does not yet exist.
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"

#شامل "abspath.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "copy.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "lockfile.h"
#شامل "parse-options.h"
#شامل "refs.h"
#شامل "refspec.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "tree.h"
#شامل "tree-walk.h"
#شامل "unpack-trees.h"
#شامل "transport.h"
#شامل "strbuf.h"
#شامل "dir.h"
#شامل "dir-iterator.h"
#شامل "iterator.h"
#شامل "sigchain.h"
#شامل "branch.h"
#شامل "remote.h"
#شامل "run-command.h"
#شامل "setup.h"
#شامل "connected.h"
#شامل "packfile.h"
#شامل "path.h"
#شامل "pkt-line.h"
#شامل "list-objects-filter-options.h"
#شامل "hook.h"
#شامل "bundle.h"
#شامل "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

ساخت clone_opts {
	صحیح چاہتا_سر;
	صحیح detach;
};
#تعریف_کرو CLONE_OPTS_INIT { \
	.چاہتا_سر = 1 /* default enabled */ \
}

ساکن صحیح option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
ساکن صحیح option_local = -1, option_no_hardlinks, option_shared;
ساکن صحیح option_لیبل_فہرست = 1; /* default enabled */
ساکن صحیح option_shallow_submodules;
ساکن صحیح config_reject_shallow = -1;    /* unspecified */
ساکن حرف *remote_name = تہی;
ساکن حرف *option_branch = تہی;
ساکن صحیح option_verbosity;
ساکن ساخت string_list option_required_reference = STRING_LIST_INIT_NODUP;
ساکن ساخت string_list option_optional_reference = STRING_LIST_INIT_NODUP;
ساکن صحیح max_jobs = -1;
ساکن ساخت string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
ساکن صحیح config_filter_submodules = -1;    /* unspecified */
ساکن صحیح option_remote_submodules;

ساکن صحیح recurse_submodules_cb(ثابت ساخت option *opt,
				 ثابت حرف *arg, صحیح unset)
{
	اگر (unset)
		string_list_clear((ساخت string_list *)opt->قدر, 0);
	ورنہ اگر (arg)
		string_list_append((ساخت string_list *)opt->قدر, arg);
	ورنہ
		string_list_append((ساخت string_list *)opt->قدر,
				   (ثابت حرف *)opt->defval);

	واپس 0;
}

ساکن ثابت حرف *get_repo_راستہ_1(ساخت strbuf *راستہ, صحیح *is_bundle)
{
	ساکن ثابت حرف *suffix[] = { "/.git", "", ".git/.git", ".git" };
	ساکن ثابت حرف *bundle_suffix[] = { ".bundle", "" };
	سائز_t baselen = راستہ->len;
	ساخت stat st;
	صحیح i;

	برائے (i = 0; i < ARRAY_سائز(suffix); i++) {
		strbuf_setlen(راستہ, baselen);
		strbuf_addstr(راستہ, suffix[i]);
		اگر (stat(راستہ->buf, &st))
			جاری;
		اگر (S_ISDIR(st.st_منوال) && is_git_directory(راستہ->buf)) {
			*is_bundle = 0;
			واپس راستہ->buf;
		} ورنہ اگر (S_ISREG(st.st_منوال) && st.st_سائز > 8) {
			/* Is it a "gitfile"? */
			حرف signature[8];
			ثابت حرف *dst;
			صحیح len, fd = open(راستہ->buf, O_RDONLY);
			اگر (fd < 0)
				جاری;
			len = read_اندر_بھرا(fd, signature, 8);
			close(fd);
			اگر (len != 8 || لڑی_موازنہ(signature, "gitdir: ", 8))
				جاری;
			dst = read_gitfile(راستہ->buf);
			اگر (dst) {
				*is_bundle = 0;
				واپس dst;
			}
		}
	}

	برائے (i = 0; i < ARRAY_سائز(bundle_suffix); i++) {
		strbuf_setlen(راستہ, baselen);
		strbuf_addstr(راستہ, bundle_suffix[i]);
		اگر (!stat(راستہ->buf, &st) && S_ISREG(st.st_منوال)) {
			*is_bundle = 1;
			واپس راستہ->buf;
		}
	}

	واپس تہی;
}

ساکن حرف *get_repo_راستہ(ثابت حرف *repo, صحیح *is_bundle)
{
	ساخت strbuf راستہ = STRBUF_INIT;
	ثابت حرف *raw;
	حرف *canon;

	strbuf_addstr(&راستہ, repo);
	raw = get_repo_راستہ_1(&راستہ, is_bundle);
	canon = raw ? absolute_pathdup(raw) : تہی;
	strbuf_release(&راستہ);
	واپس canon;
}

ساخت collect_alternates_ڈیٹا {
	ساخت strvec *alternates;
	bool required;
};

ساکن صحیح collect_one_alternate(ساخت string_list_شے *شے, خالی *cb_ڈیٹا)
{
	ساخت collect_alternates_ڈیٹا *ڈیٹا = cb_ڈیٹا;
	ساخت strbuf err = STRBUF_INIT;
	حرف *ref_git = compute_alternate_راستہ(شے->string, &err);

	اگر (!ref_git) {
		اگر (ڈیٹا->required)
			die("%s", err.buf);
		ورنہ
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				شے->string, err.buf);
	} ورنہ {
		strvec_pushf(ڈیٹا->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	آزاد(ref_git);
	واپس 0;
}

ساکن خالی read_alternates(ساخت strvec *alternates, ثابت حرف *src_repo)
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
	FILE *اندر;
	ساخت strbuf راستہ = STRBUF_INIT;
	ساخت strbuf سطر = STRBUF_INIT;

	strbuf_addf(&راستہ, "%s/objects/info/alternates", src_repo);

	اندر = fopen(راستہ.buf, "r");
	اگر (!اندر) {
		اگر (errno == ENOENT)
			جاؤ برآمد;
		die_errno("could not read alternates file '%s'", راستہ.buf);
	}

	جب (strbuf_getline(&سطر, اندر) != EOF) {
		حرف *abs_راستہ;
		اگر (!سطر.len || سطر.buf[0] == '#')
			جاری;
		اگر (is_absolute_راستہ(سطر.buf)) {
			strvec_ڈالو(alternates, سطر.buf);
			جاری;
		}
		abs_راستہ = mkpathdup("%s/objects/%s", src_repo, سطر.buf);
		اگر (!normalize_راستہ_copy(abs_راستہ, abs_راستہ))
			strvec_ڈالو(alternates, abs_راستہ);
		ورنہ
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, سطر.buf);
		آزاد(abs_راستہ);
	}

برآمد:
	strbuf_release(&راستہ);
	strbuf_release(&سطر);
	اگر (اندر)
		fclose(اندر);
}

ساکن خالی collect_alternates(ساخت strvec *alternates,
			       ثابت حرف *local_ماخذ_repo)
{
	اگر (option_required_reference.nr || option_optional_reference.nr) {
		ساخت collect_alternates_ڈیٹا ڈیٹا = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &ڈیٹا);
		ڈیٹا.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &ڈیٹا);
	}

	اگر (local_ماخذ_repo) {
		ساخت strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_ماخذ_repo);
		اگر (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		ورنہ
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

ساکن خالی mkdir_if_missing(ثابت حرف *pathname, منوال_t منوال)
{
	ساخت stat st;

	اگر (!mkdir(pathname, منوال))
		واپس;

	اگر (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	ورنہ اگر (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	ورنہ اگر (!S_ISDIR(st.st_منوال))
		die(_("%s exists and is not a directory"), pathname);
}

ساکن خالی copy_or_link_directory(ساخت strbuf *src, ساخت strbuf *dest,
				   ثابت حرف *src_repo)
{
	صحیح src_len, dest_len;
	ساخت dir_iterator *iter;
	صحیح iter_حالت;

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
	die_upon_dubious_ownership(تہی, تہی, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	اگر (!iter) {
		اگر (errno == ENOTDIR) {
			صحیح saved_errno = errno;
			ساخت stat st;

			اگر (!lstat(src->buf, &st) && S_ISLNK(st.st_منوال))
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

	جب ((iter_حالت = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_راستہ);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_راستہ);

		اگر (S_ISLNK(iter->st.st_منوال))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_راستہ);

		اگر (S_ISDIR(iter->st.st_منوال)) {
			mkdir_if_missing(dest->buf, 0777);
			جاری;
		}

		/* Alternates were already handled earlier. */
		اگر (!fspathcmp(iter->relative_راستہ, "info/alternates"))
			جاری;

		اگر (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		اگر (!option_no_hardlinks) {
			اگر (!link(src->buf, dest->buf)) {
				ساخت stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				اگر (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				اگر (st.st_منوال != iter->st.st_منوال ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_سائز != iter->st.st_سائز ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				جاری;
			}
			اگر (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		اگر (copy_file_with_وقت(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	اگر (iter_حالت != ITER_مکمل) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

ساکن خالی clone_local(ثابت حرف *src_repo, ثابت حرف *dest_repo)
{
	اگر (!option_shared) {
		ساخت strbuf src = STRBUF_INIT;
		ساخت strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	اگر (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

ساکن ثابت حرف *junk_work_درخت;
ساکن صحیح junk_work_درخت_flags;
ساکن ثابت حرف *junk_git_dir;
ساکن صحیح junk_git_dir_flags;
ساکن شمار {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_منوال = JUNK_LEAVE_NONE;

ساکن ثابت حرف junk_leave_repo_پیغام[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

ساکن خالی remove_junk(خالی)
{
	ساخت strbuf sb = STRBUF_INIT;

	انتخاب (junk_منوال) {
	حالت JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_پیغام));
		/* fall-through */
	حالت JUNK_LEAVE_ALL:
		واپس;
	طےشدہ:
		/* proceed to removal */
		توڑو;
	}

	اگر (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	اگر (junk_work_درخت) {
		strbuf_addstr(&sb, junk_work_درخت);
		remove_dir_recursively(&sb, junk_work_درخت_flags);
	}
	strbuf_release(&sb);
}

ساکن خالی remove_junk_on_signal(صحیح signo)
{
	remove_junk();
	sigchain_نکالو(signo);
	اٹھاؤ(signo);
}

ساکن ساخت ref *ڈھونڈو_remote_branch(ثابت ساخت ref *refs, ثابت حرف *branch)
{
	ساخت ref *ref;
	ساخت strbuf سر = STRBUF_INIT;
	strbuf_addstr(&سر, "refs/heads/");
	strbuf_addstr(&سر, branch);
	ref = ڈھونڈو_ref_by_name(refs, سر.buf);
	strbuf_release(&سر);

	اگر (ref)
		واپس ref;

	strbuf_addstr(&سر, "refs/tags/");
	strbuf_addstr(&سر, branch);
	ref = ڈھونڈو_ref_by_name(refs, سر.buf);
	strbuf_release(&سر);

	واپس ref;
}

ساکن ساخت ref *wanted_peer_refs(ساخت clone_opts *opts,
				    ثابت ساخت ref *refs,
				    ساخت refspec *refspec)
{
	ساخت ref *local_refs = تہی;
	ساخت ref **دم = &local_refs;
	ساخت ref *مقصد_free = تہی;

	اگر (opts->چاہتا_سر) {
		ساخت ref *سر = copy_ref(ڈھونڈو_ref_by_name(refs, "HEAD"));
		اگر (سر)
			دم_link_ref(سر, &دم);
		اگر (option_single_branch)
			refs = مقصد_free =
				guess_remote_سر(سر, refs,
						  REMOTE_GUESS_سر_QUIET);
	} ورنہ اگر (option_single_branch) {
		local_refs = تہی;
		دم = &local_refs;
		refs = مقصد_free = copy_ref(ڈھونڈو_remote_branch(refs, option_branch));
	}

	برائے (سائز_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->اشیاء[i], &دم, 0);

	free_one_ref(مقصد_free);

	واپس local_refs;
}

ساکن خالی write_remote_refs(ثابت ساخت ref *local_refs)
{
	ثابت ساخت ref *r;

	ساخت ref_transaction *t;
	ساخت strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	اگر (!t)
		die("%s", err.buf);

	برائے (r = local_refs; r; r = r->اگلا) {
		اگر (!r->peer_ref)
			جاری;
		اگر (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   تہی, 0, تہی, &err))
			die("%s", err.buf);
	}

	اگر (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

ساکن خالی write_followtags(ثابت ساخت ref *refs, ثابت حرف *پیغام)
{
	ثابت ساخت ref *ref;
	برائے (ref = refs; ref; ref = ref->اگلا) {
		اگر (!starts_with(ref->name, "refs/tags/"))
			جاری;
		اگر (سرے_with(ref->name, "^{}"))
			جاری;
		اگر (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			جاری;
		refs_update_ref(get_main_ref_store(the_repository), پیغام,
				ref->name, &ref->old_oid, تہی, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

ساکن ثابت ساخت object_id *iterate_ref_map(خالی *cb_ڈیٹا)
{
	ساخت ref **rm = cb_ڈیٹا;
	ساخت ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	جب (ref && !ref->peer_ref)
		ref = ref->اگلا;
	اگر (!ref)
		واپس تہی;

	*rm = ref->اگلا;
	واپس &ref->old_oid;
}

ساکن خالی update_remote_refs(ثابت ساخت ref *refs,
			       ثابت ساخت ref *mapped_refs,
			       ثابت ساخت ref *remote_سر_points_at,
			       ثابت حرف *branch_چوٹی,
			       ثابت حرف *پیغام,
			       ساخت transport *transport,
			       صحیح check_connectivity)
{
	ثابت ساخت ref *rm = mapped_refs;

	اگر (check_connectivity) {
		ساخت check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.پیش_رفت = transport->پیش_رفت;

		اگر (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	اگر (refs) {
		write_remote_refs(mapped_refs);
		اگر (option_single_branch && option_لیبل_فہرست)
			write_followtags(refs, پیغام);
	}

	اگر (remote_سر_points_at && remote_سر_points_at->peer_ref && !option_bare) {
		ساخت strbuf سر_ref = STRBUF_INIT;
		strbuf_addstr(&سر_ref, branch_چوٹی);
		strbuf_addstr(&سر_ref, "HEAD");
		اگر (refs_update_symref(get_main_ref_store(the_repository), سر_ref.buf,
				       remote_سر_points_at->peer_ref->name,
				       پیغام) < 0)
			die(_("unable to update %s"), سر_ref.buf);
		strbuf_release(&سر_ref);
	}
}

ساکن خالی update_سر(ساخت clone_opts *opts, ثابت ساخت ref *our, ثابت ساخت ref *remote,
			ثابت حرف *unborn, ثابت حرف *پیغام)
{
	ثابت حرف *سر;
	اگر (our && !opts->detach && skip_سابقہ(our->name, "refs/heads/", &سر)) {
		/* Local default branch link */
		اگر (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, تہی) < 0)
			die(_("unable to update HEAD"));
		اگر (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					پیغام, "HEAD", &our->old_oid, تہی, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, سر, remote_name, our->name);
		}
	} ورنہ اگر (our) {
		ساخت commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), پیغام,
				"HEAD", &c->object.oid, تہی, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ورنہ اگر (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), پیغام,
				"HEAD", &remote->old_oid, تہی, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} ورنہ اگر (unborn && skip_سابقہ(unborn, "refs/heads/", &سر)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		اگر (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, تہی) < 0)
			die(_("unable to update HEAD"));
		اگر (!option_bare)
			install_branch_config(0, سر, remote_name, unborn);
	}
}

ساکن صحیح git_sparse_checkout_init(ثابت حرف *repo)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;
	صحیح نتیجہ = 0;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	strvec_pushl(&cmd.دلائل, "-C", repo, "sparse-checkout", "set", تہی);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	اگر (تسلسل_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		نتیجہ = 1;
	}

	واپس نتیجہ;
}

ساکن صحیح checkout(صحیح submodule_پیش_رفت,
		    ساخت list_objects_filter_options *filter_options,
		    صحیح filter_submodules,
		    شمار ref_storage_format ref_storage_format)
{
	ساخت object_id oid;
	حرف *سر;
	ساخت تالا_file تالا_file = تالا_INIT;
	ساخت unpack_trees_options opts;
	ساخت درخت *درخت;
	ساخت درخت_desc t;
	صحیح err = 0;
	ساخت تسلسل_hooks_opt hook_opt = تسلسل_HOOKS_OPT_INIT_FORCE_SERIAL;

	اگر (option_no_checkout)
		واپس 0;

	سر = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, تہی);
	اگر (!سر) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		واپس 0;
	}
	اگر (!strcmp(سر, "HEAD")) {
		اگر (advice_enabled(ADVICE_DETACHED_سر))
			detach_advice(oid_مقصد_hex(&oid));
		FREE_AND_NULL(سر);
	} ورنہ {
		اگر (!starts_with(سر, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	تیاری_work_درخت(the_repository);

	repo_hold_locked_اشاریہ(the_repository, &تالا_file, تالا_DIE_ON_ERROR);

	memset(&opts, 0, حجم opts);
	opts.update = 1;
	opts.ملاؤ = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_ملاؤ;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	init_checkout_metadata(&opts.meta, سر, &oid, تہی);

	درخت = repo_parse_درخت_بالواسطہ(the_repository, &oid);
	اگر (!درخت)
		die(_("unable to parse commit %s"), oid_مقصد_hex(&oid));
	اگر (repo_parse_درخت(the_repository, درخت) < 0)
		باہر_نکلو(128);
	init_درخت_desc(&t, &درخت->object.oid, درخت->بفر, درخت->سائز);
	اگر (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	آزاد(سر);

	اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا_file, COMMIT_تالا))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.دلائل, oid_مقصد_hex(null_oid(the_hash_algo)),
		     oid_مقصد_hex(&oid), "1", تہی);
	err |= تسلسل_hooks_opt(the_repository, "post-checkout", &hook_opt);

	اگر (!err && (option_recurse_submodules.nr > 0)) {
		ساخت بچہ_process cmd = بچہ_PROCESS_INIT;
		strvec_pushl(&cmd.دلائل, "submodule", "update", "--require-init",
			     "--recursive", تہی);

		اگر (option_shallow_submodules == 1)
			strvec_ڈالو(&cmd.دلائل, "--depth=1");

		اگر (max_jobs != -1)
			strvec_pushf(&cmd.دلائل, "--jobs=%d", max_jobs);

		اگر (submodule_پیش_رفت)
			strvec_ڈالو(&cmd.دلائل, "--progress");

		اگر (option_verbosity < 0)
			strvec_ڈالو(&cmd.دلائل, "--quiet");

		اگر (option_remote_submodules) {
			strvec_ڈالو(&cmd.دلائل, "--remote");
			strvec_ڈالو(&cmd.دلائل, "--no-fetch");
		}

		اگر (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.دلائل, "--ref-storage-format=%s",
				     ref_storage_format_مقصد_name(ref_storage_format));

		اگر (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.دلائل, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		اگر (option_single_branch >= 0)
			strvec_ڈالو(&cmd.دلائل, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = تسلسل_command(&cmd);
	}

	واپس err;
}

ساکن صحیح git_clone_config(ثابت حرف *k, ثابت حرف *v,
			    ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(k, "clone.defaultremotename")) {
		اگر (!v)
			واپس config_error_nonbool(k);
		آزاد(remote_name);
		remote_name = xstrdup(v);
	}
	اگر (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	اگر (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	واپس git_default_config(k, v, ctx, cb);
}

ساکن صحیح write_one_config(ثابت حرف *کلید, ثابت حرف *قدر,
			    ثابت ساخت config_context *ctx,
			    خالی *ڈیٹا)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	صحیح apply_failed = git_clone_config(کلید, قدر, ctx, ڈیٹا);
	اگر (apply_failed)
		واپس apply_failed;

	واپس repo_config_سیٹ_multivar_gently(the_repository, کلید,
					       قدر ? قدر : "true",
					       CONFIG_REGEX_NONE, 0);
}

ساکن خالی write_config(ساخت string_list *config)
{
	صحیح i;

	برائے (i = 0; i < config->nr; i++) {
		اگر (git_config_parse_parameter(config->اشیاء[i].string,
					       write_one_config, تہی) < 0)
			die(_("unable to write parameters to config file"));
	}
}

ساکن خالی write_refspec_config(ثابت حرف *src_ref_سابقہ,
		ثابت ساخت ref *our_سر_points_at,
		ثابت ساخت ref *remote_سر_points_at,
		ساخت strbuf *branch_چوٹی)
{
	ساخت strbuf کلید = STRBUF_INIT;
	ساخت strbuf قدر = STRBUF_INIT;

	اگر (option_mirror || !option_bare) {
		اگر (option_single_branch && !option_mirror) {
			اگر (option_branch) {
				اگر (starts_with(our_سر_points_at->name, "refs/tags/"))
					strbuf_addf(&قدر, "+%s:%s", our_سر_points_at->name,
						our_سر_points_at->name);
				ورنہ
					strbuf_addf(&قدر, "+%s:%s%s", our_سر_points_at->name,
						branch_چوٹی->buf, option_branch);
			} ورنہ اگر (remote_سر_points_at) {
				ثابت حرف *سر = remote_سر_points_at->name;
				اگر (!skip_سابقہ(سر, "refs/heads/", &سر))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&قدر, "+%s:%s%s", remote_سر_points_at->name,
						branch_چوٹی->buf, سر);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} ورنہ {
			strbuf_addf(&قدر, "+%s*:%s*", src_ref_سابقہ, branch_چوٹی->buf);
		}
		/* Configure the remote */
		اگر (قدر.len) {
			strbuf_addf(&کلید, "remote.%s.fetch", remote_name);
			repo_config_سیٹ_multivar(the_repository, کلید.buf, قدر.buf, "^$", 0);
			strbuf_reset(&کلید);

			اگر (option_mirror) {
				strbuf_addf(&کلید, "remote.%s.mirror", remote_name);
				repo_config_سیٹ(the_repository, کلید.buf, "true");
				strbuf_reset(&کلید);
			}
		}
	}

	strbuf_release(&کلید);
	strbuf_release(&قدر);
}

ساکن خالی dissociate_from_references(خالی)
{
	حرف *alternates = repo_git_راستہ(the_repository, "objects/info/alternates");

	اگر (!access(alternates, F_OK)) {
		ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.دلائل, "repack", "-a", "-d", تہی);
		اگر (تسلسل_command(&cmd))
			die(_("cannot repack to clean up"));
		اگر (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	آزاد(alternates);
}

ساکن صحیح راستہ_exists(ثابت حرف *راستہ)
{
	ساخت stat sb;
	واپس !stat(راستہ, &sb);
}

صحیح cmd_clone(صحیح argc,
	      ثابت حرف **argv,
	      ثابت حرف *سابقہ,
	      ساخت repository *repository غیر_مستعمل)
{
	صحیح is_bundle = 0, is_local;
	صحیح reject_shallow = 0;
	ثابت حرف *repo_name, *repo, *work_درخت, *git_dir;
	حرف *repo_مقصد_free = تہی;
	حرف *راستہ = تہی, *dir, *display_repo = تہی;
	صحیح dest_exists, real_dest_exists = 0;
	ثابت ساخت ref *refs, *remote_سر;
	ساخت ref *remote_سر_points_at = تہی;
	ثابت ساخت ref *our_سر_points_at;
	حرف *unborn_سر = تہی;
	ساخت ref *mapped_refs = تہی;
	ثابت ساخت ref *ref;
	ساخت strbuf کلید = STRBUF_INIT;
	ساخت strbuf buf = STRBUF_INIT;
	ساخت strbuf branch_چوٹی = STRBUF_INIT, reflog_پیغام = STRBUF_INIT;
	ساخت transport *transport = تہی;
	ثابت حرف *src_ref_سابقہ = "refs/heads/";
	ساخت remote *remote;
	صحیح err = 0, complete_refs_before_fetch = 1;
	صحیح submodule_پیش_رفت;
	صحیح filter_submodules = 0;
	صحیح hash_algo;
	شمار ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	ثابت صحیح do_not_override_repo_unix_permissions = -1;
	صحیح option_reject_shallow = -1; /* unspecified */
	صحیح deepen = 0;
	حرف *option_template = تہی, *option_گہرائی = تہی, *option_since = تہی;
	حرف *option_origin = تہی;
	ساخت string_list option_not = STRING_LIST_INIT_NODUP;
	ثابت حرف *real_git_dir = تہی;
	ثابت حرف *ref_storage_format_uri = تہی;
	ثابت حرف *option_upload_pack = "git-upload-pack";
	صحیح option_پیش_رفت = -1;
	صحیح option_sparse_checkout = 0;
	شمار transport_family family = TRANSPORT_FAMILY_ALL;
	ساخت string_list option_config = STRING_LIST_INIT_DUP;
	صحیح option_dissociate = 0;
	ساخت list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	صحیح option_filter_submodules = -1; /* unspecified */
	ساخت string_list server_options = STRING_LIST_INIT_NODUP;
	ثابت حرف *bundle_uri = تہی;
	حرف *option_rev = تہی;

	ساخت clone_opts opts = CLONE_OPTS_INIT;

	ساخت transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	ساخت option builtin_clone_options[] = {
		OPT__VERBOSITY(&option_verbosity),
		OPT_BOOL(0, "progress", &option_پیش_رفت,
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
			.قدر = &option_recurse_submodules,
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
		OPT_STRING(0, "depth", &option_گہرائی, N_("depth"),
			   N_("create a shallow clone of that depth")),
		OPT_STRING(0, "shallow-since", &option_since, N_("time"),
			   N_("create a shallow clone since a specific time")),
		OPT_STRING_LIST(0, "shallow-exclude", &option_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_BOOL(0, "single-branch", &option_single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_BOOL(0, "tags", &option_لیبل_فہرست,
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
		OPT_انتہا()
	};
	ساخت strvec alternates = STRVEC_INIT;

	ثابت حرف * ثابت builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		تہی
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_شناختی_عنصر("clone");

	repo_config(the_repository, git_clone_config, تہی);

	argc = parse_options(argc, argv, سابقہ, builtin_clone_options,
			     builtin_clone_usage, 0);

	اگر (argc > 2)
		usage_پیغام_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	اگر (argc == 0)
		usage_پیغام_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	اگر (option_گہرائی || option_since || option_not.nr)
		deepen = 1;
	اگر (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	اگر (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, تہی);
		اگر (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	اگر (option_mirror) {
		option_bare = 1;
		option_لیبل_فہرست = 0;
	}

	اگر (option_bare) {
		اگر (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	اگر (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	راستہ = get_repo_راستہ(repo_name, &is_bundle);
	اگر (راستہ) {
		FREE_AND_NULL(راستہ);
		repo = repo_مقصد_free = absolute_pathdup(repo_name);
	} ورنہ اگر (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} ورنہ
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	اگر (option_گہرائی && atoi(option_گہرائی) < 1)
		die(_("depth %s is not a positive number"), option_گہرائی);

	اگر (argc == 2)
		dir = xstrdup(argv[1]);
	ورنہ
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = راستہ_exists(dir);
	اگر (dest_exists && !is_خالی_خانے_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	اگر (real_git_dir) {
		real_dest_exists = راستہ_exists(real_git_dir);
		اگر (real_dest_exists && !is_خالی_خانے_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_پیغام, "clone: from %s",
		    display_repo ? display_repo : repo);
	آزاد(display_repo);

	اگر (option_bare)
		work_درخت = تہی;
	ورنہ {
		work_درخت = getenv("GIT_WORK_TREE");
		اگر (work_درخت && راستہ_exists(work_درخت))
			die(_("working tree '%s' already exists."), work_درخت);
	}

	اگر (option_bare || work_درخت)
		git_dir = xstrdup(dir);
	ورنہ {
		work_درخت = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_ڈالو_common(remove_junk_on_signal);

	اگر (!option_bare) {
		اگر (محفوظ_create_leading_directories_const(the_repository, work_درخت) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_درخت);
		اگر (dest_exists)
			junk_work_درخت_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		ورنہ اگر (mkdir(work_درخت, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_درخت);
		junk_work_درخت = work_درخت;
	}

	اگر (real_git_dir) {
		اگر (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} ورنہ {
		اگر (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	اگر (محفوظ_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	اگر (0 <= option_verbosity) {
		اگر (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		ورنہ
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	اگر (option_recurse_submodules.nr > 0) {
		ساخت string_list_شے *شے;
		ساخت strbuf sb = STRBUF_INIT;
		صحیح val;

		/* remove duplicates */
		string_list_ترتیب_دو_u(&option_recurse_submodules, 0);

		/*
		 * NEEDSWORK: In a multi-working-tree world, this needs to be
		 * set in the per-worktree config.
		 */
		for_each_string_list_شے(شے, &option_recurse_submodules) {
			strbuf_addf(&sb, "submodule.active=%s",
				    شے->string);
			string_list_append(&option_config, sb.buf);
			strbuf_reset(&sb);
		}

		اگر (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		اگر (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		ورنہ اگر (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} ورنہ اگر (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_درخت,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, تہی);

	اگر (real_git_dir) {
		آزاد((حرف *)git_dir);
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
	initialize_repository_نسخہ(the_repository, GIT_HASH_UNKNOWN,
				      the_repository->ref_storage_format, 1);

	refs_create_refdir_stubs(the_repository, git_dir, تہی);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, تہی);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	اگر (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	اگر (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	اگر (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	اگر (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	اگر (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	اگر (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	اگر (option_origin) {
		آزاد(remote_name);
		remote_name = xstrdup(option_origin);
	}

	اگر (!remote_name)
		remote_name = xstrdup("origin");

	اگر (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	اگر (option_bare) {
		اگر (option_mirror)
			src_ref_سابقہ = "refs/";
		strbuf_addstr(&branch_چوٹی, src_ref_سابقہ);

		repo_config_سیٹ(the_repository, "core.bare", "true");
	} ورنہ اگر (!option_rev) {
		strbuf_addf(&branch_چوٹی, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&کلید, "remote.%s.url", remote_name);
	repo_config_سیٹ(the_repository, کلید.buf, repo);
	strbuf_reset(&کلید);

	اگر (!option_لیبل_فہرست) {
		strbuf_addf(&کلید, "remote.%s.tagOpt", remote_name);
		repo_config_سیٹ(the_repository, کلید.buf, "--no-tags");
		strbuf_reset(&کلید);
	}

	remote = remote_get_early(remote_name);

	اگر (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_سابقہ,
				branch_چوٹی.buf);

	راستہ = get_repo_راستہ(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && راستہ && !is_bundle;
	اگر (is_local) {
		اگر (option_گہرائی)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		اگر (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		اگر (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		اگر (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		اگر (!access(mkpath("%s/shallow", راستہ), F_OK)) {
			اگر (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			اگر (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	اگر (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? راستہ : تہی);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, راستہ ? راستہ : remote->url.v[0]);
	transport_سیٹ_verbosity(transport, option_verbosity, option_پیش_رفت);
	transport->family = family;
	transport->cloning = 1;

	اگر (is_bundle) {
		ساخت bundle_سرخی سرخی = BUNDLE_سرخی_INIT;
		صحیح fd = read_bundle_سرخی(راستہ, &سرخی);
		صحیح has_filter = سرخی.filter.choice != LOFC_DISABLED;

		اگر (fd > 0)
			close(fd);
		bundle_سرخی_release(&سرخی);
		اگر (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_سیٹ_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	اگر (reject_shallow)
		transport_سیٹ_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	اگر (option_گہرائی)
		transport_سیٹ_option(transport, TRANS_OPT_گہرائی,
				     option_گہرائی);
	اگر (option_since)
		transport_سیٹ_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	اگر (option_not.nr)
		transport_سیٹ_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (ثابت حرف *)&option_not);
	اگر (option_single_branch) {
		transport_سیٹ_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		اگر (option_branch)
			opts.چاہتا_سر = 0;
	}

	اگر (option_upload_pack)
		transport_سیٹ_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	اگر (server_options.nr)
		transport->server_options = &server_options;

	اگر (filter_options.choice) {
		ثابت حرف *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_سیٹ_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_سیٹ_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	اگر (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	اگر (option_rev) {
		option_لیبل_فہرست = 0;
		option_single_branch = 0;
		opts.چاہتا_سر = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	اگر (option_لیبل_فہرست || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, لیبل_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	اگر (option_branch)
		expand_ref_سابقہ(&transport_ls_refs_options.ref_prefixes,
				  option_branch);

	/*
	 * As part of transport_get_remote_refs() the server tells us the hash
	 * algorithm, which we require to initialize the repo. But calling that
	 * function without any ref prefix, will cause the server to announce
	 * all known refs. If the argument passed to --revision was a hex oid,
	 * ref_prefixes will be empty so we fall back to asking about HEAD to
	 * reduce traffic from the server.
	 */
	اگر (opts.چاہتا_سر || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_ڈالو(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_نسخہ(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_سیٹ_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, تہی, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	اگر (bundle_uri) {
		ساخت remote_state *state;
		صحیح has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = تہی;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		اگر (repo_init(the_repository, git_dir, work_درخت))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		ورنہ اگر (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		ورنہ اگر (has_heuristic)
			repo_config_سیٹ_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		آزاد(the_repository->remote_state);
		the_repository->remote_state = state;
	} ورنہ {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		اگر (transport->bundles &&
		    hashmap_get_سائز(&transport->bundles->bundles)) {
			ساخت remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = تہی;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			اگر (repo_init(the_repository, git_dir, work_درخت))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			ورنہ اگر (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			آزاد(the_repository->remote_state);
			the_repository->remote_state = state;
		} ورنہ {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	اگر (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	اگر (mapped_refs) {
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
		برائے (ref = refs; ref; ref = ref->اگلا)
			اگر (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				توڑو;
			}

		اگر (!is_local && !complete_refs_before_fetch) {
			اگر (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_سر = ڈھونڈو_ref_by_name(refs, "HEAD");
	remote_سر_points_at = guess_remote_سر(remote_سر, mapped_refs,
						  REMOTE_GUESS_سر_QUIET);

	اگر (option_branch) {
		our_سر_points_at = ڈھونڈو_remote_branch(mapped_refs, option_branch);
		اگر (!our_سر_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} ورنہ اگر (option_rev) {
		our_سر_points_at = mapped_refs;
		اگر (!our_سر_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} ورنہ اگر (remote_سر_points_at) {
		our_سر_points_at = remote_سر_points_at;
	} ورنہ اگر (remote_سر) {
		our_سر_points_at = تہی;
	} ورنہ {
		حرف *مقصد_free = تہی;
		ثابت حرف *branch;

		اگر (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		اگر (transport_ls_refs_options.unborn_سر_ہدف &&
		    skip_سابقہ(transport_ls_refs_options.unborn_سر_ہدف,
				"refs/heads/", &branch)) {
			unborn_سر  = xstrdup(transport_ls_refs_options.unborn_سر_ہدف);
		} ورنہ {
			branch = مقصد_free = repo_default_branch_name(the_repository, 0);
			unborn_سر = xstrfmt("refs/heads/%s", branch);
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
		our_سر_points_at = ڈھونڈو_remote_branch(mapped_refs, branch);

		آزاد(مقصد_free);
	}

	اگر (!option_rev)
		write_refspec_config(src_ref_سابقہ, our_سر_points_at,
				     remote_سر_points_at, &branch_چوٹی);

	اگر (filter_options.choice)
		partial_clone_رجسٹر(remote_name, &filter_options);

	اگر (is_local)
		clone_local(راستہ, git_dir);
	ورنہ اگر (mapped_refs && complete_refs_before_fetch) {
		اگر (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_سر_points_at,
			   branch_چوٹی.buf, reflog_پیغام.buf, transport,
			   !is_local);

	update_سر(&opts, our_سر_points_at, remote_سر, unborn_سر, reflog_پیغام.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_پیش_رفت = transport->پیش_رفت;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	اگر (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	اگر (option_sparse_checkout && git_sparse_checkout_init(dir))
		واپس 1;

	junk_منوال = JUNK_LEAVE_REPO;
	err = checkout(submodule_پیش_رفت,
		       &filter_options,
		       filter_submodules,
		       ref_storage_format);

	list_objects_filter_release(&filter_options);

	string_list_clear(&option_not, 0);
	string_list_clear(&option_config, 0);
	string_list_clear(&server_options, 0);
	strvec_clear(&alternates);

	آزاد(remote_name);
	strbuf_release(&reflog_پیغام);
	strbuf_release(&branch_چوٹی);
	strbuf_release(&buf);
	strbuf_release(&کلید);
	free_refs(mapped_refs);
	free_refs(remote_سر_points_at);
	آزاد(unborn_سر);
	آزاد(dir);
	آزاد(راستہ);
	آزاد(repo_مقصد_free);
	junk_منوال = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	واپس err;
}
