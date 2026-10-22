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
	صحيح چاہندا_سر;
	صحيح detach;
};
#تعریف_کرو CLONE_OPTS_INIT { \
	.چاہندا_سر = 1 /* default enabled */ \
}

ساكن صحيح option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
ساكن صحيح option_local = -1, option_no_hardlinks, option_shared;
ساكن صحيح option_ٹیگ_سوچی = 1; /* default enabled */
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
		string_list_clear((بنية string_list *)opt->قدر, 0);
	وإلا إذا (arg)
		string_list_append((بنية string_list *)opt->قدر, arg);
	وإلا
		string_list_append((بنية string_list *)opt->قدر,
				   (ثابت حرف *)opt->defval);

	أرجع 0;
}

ساكن ثابت حرف *get_repo_رستہ_1(بنية strbuf *رستہ, صحيح *is_bundle)
{
	ساكن ثابت حرف *suffix[] = { "/.git", "", ".git/.git", ".git" };
	ساكن ثابت حرف *bundle_suffix[] = { ".bundle", "" };
	ناپ_t baselen = رستہ->len;
	بنية stat st;
	صحيح i;

	لكل (i = 0; i < ARRAY_ناپ(suffix); i++) {
		strbuf_setlen(رستہ, baselen);
		strbuf_addstr(رستہ, suffix[i]);
		إذا (stat(رستہ->buf, &st))
			استمر;
		إذا (S_ISDIR(st.st_منوال) && is_git_directory(رستہ->buf)) {
			*is_bundle = 0;
			أرجع رستہ->buf;
		} وإلا إذا (S_ISREG(st.st_منوال) && st.st_ناپ > 8) {
			/* Is it a "gitfile"? */
			حرف signature[8];
			ثابت حرف *dst;
			صحيح len, fd = open(رستہ->buf, O_RDONLY);
			إذا (fd < 0)
				استمر;
			len = read_وچ_بھریا(fd, signature, 8);
			close(fd);
			إذا (len != 8 || لڑی_مقابلہ(signature, "gitdir: ", 8))
				استمر;
			dst = read_gitfile(رستہ->buf);
			إذا (dst) {
				*is_bundle = 0;
				أرجع dst;
			}
		}
	}

	لكل (i = 0; i < ARRAY_ناپ(bundle_suffix); i++) {
		strbuf_setlen(رستہ, baselen);
		strbuf_addstr(رستہ, bundle_suffix[i]);
		إذا (!stat(رستہ->buf, &st) && S_ISREG(st.st_منوال)) {
			*is_bundle = 1;
			أرجع رستہ->buf;
		}
	}

	أرجع عدم;
}

ساكن حرف *get_repo_رستہ(ثابت حرف *repo, صحيح *is_bundle)
{
	بنية strbuf رستہ = STRBUF_INIT;
	ثابت حرف *raw;
	حرف *canon;

	strbuf_addstr(&رستہ, repo);
	raw = get_repo_رستہ_1(&رستہ, is_bundle);
	canon = raw ? absolute_pathdup(raw) : عدم;
	strbuf_release(&رستہ);
	أرجع canon;
}

بنية collect_alternates_انکڑے {
	بنية strvec *alternates;
	bool required;
};

ساكن صحيح collect_one_alternate(بنية string_list_شے *شے, فراغ *cb_انکڑے)
{
	بنية collect_alternates_انکڑے *انکڑے = cb_انکڑے;
	بنية strbuf err = STRBUF_INIT;
	حرف *ref_git = compute_alternate_رستہ(شے->string, &err);

	إذا (!ref_git) {
		إذا (انکڑے->required)
			die("%s", err.buf);
		وإلا
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				شے->string, err.buf);
	} وإلا {
		strvec_pushf(انکڑے->alternates, "%s/objects", ref_git);
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
	FILE *وچ;
	بنية strbuf رستہ = STRBUF_INIT;
	بنية strbuf سطر = STRBUF_INIT;

	strbuf_addf(&رستہ, "%s/objects/info/alternates", src_repo);

	وچ = fopen(رستہ.buf, "r");
	إذا (!وچ) {
		إذا (errno == ENOENT)
			اذهب باہرلا;
		die_errno("could not read alternates file '%s'", رستہ.buf);
	}

	طالما (strbuf_getline(&سطر, وچ) != EOF) {
		حرف *abs_رستہ;
		إذا (!سطر.len || سطر.buf[0] == '#')
			استمر;
		إذا (is_absolute_رستہ(سطر.buf)) {
			strvec_ڈالو(alternates, سطر.buf);
			استمر;
		}
		abs_رستہ = mkpathdup("%s/objects/%s", src_repo, سطر.buf);
		إذا (!normalize_رستہ_copy(abs_رستہ, abs_رستہ))
			strvec_ڈالو(alternates, abs_رستہ);
		وإلا
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, سطر.buf);
		حرر(abs_رستہ);
	}

باہرلا:
	strbuf_release(&رستہ);
	strbuf_release(&سطر);
	إذا (وچ)
		fclose(وچ);
}

ساكن فراغ collect_alternates(بنية strvec *alternates,
			       ثابت حرف *local_سروت_repo)
{
	إذا (option_required_reference.nr || option_optional_reference.nr) {
		بنية collect_alternates_انکڑے انکڑے = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &انکڑے);
		انکڑے.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &انکڑے);
	}

	إذا (local_سروت_repo) {
		بنية strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_سروت_repo);
		إذا (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		وإلا
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

ساكن فراغ mkdir_if_missing(ثابت حرف *pathname, منوال_t منوال)
{
	بنية stat st;

	إذا (!mkdir(pathname, منوال))
		أرجع;

	إذا (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	وإلا إذا (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	وإلا إذا (!S_ISDIR(st.st_منوال))
		die(_("%s exists and is not a directory"), pathname);
}

ساكن فراغ copy_or_link_directory(بنية strbuf *src, بنية strbuf *dest,
				   ثابت حرف *src_repo)
{
	صحيح src_len, dest_len;
	بنية dir_iterator *iter;
	صحيح iter_حالت;

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

			إذا (!lstat(src->buf, &st) && S_ISLNK(st.st_منوال))
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

	طالما ((iter_حالت = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_رستہ);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_رستہ);

		إذا (S_ISLNK(iter->st.st_منوال))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_رستہ);

		إذا (S_ISDIR(iter->st.st_منوال)) {
			mkdir_if_missing(dest->buf, 0777);
			استمر;
		}

		/* Alternates were already handled earlier. */
		إذا (!fspathcmp(iter->relative_رستہ, "info/alternates"))
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
				إذا (st.st_منوال != iter->st.st_منوال ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_ناپ != iter->st.st_ناپ ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				استمر;
			}
			إذا (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		إذا (copy_file_with_ویلا(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	إذا (iter_حالت != ITER_پورا) {
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

ساكن ثابت حرف *junk_work_درخت;
ساكن صحيح junk_work_درخت_flags;
ساكن ثابت حرف *junk_git_dir;
ساكن صحيح junk_git_dir_flags;
ساكن تعداد {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_منوال = JUNK_LEAVE_NONE;

ساكن ثابت حرف junk_leave_repo_پیغام[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

ساكن فراغ remove_junk(فراغ)
{
	بنية strbuf sb = STRBUF_INIT;

	بدل (junk_منوال) {
	حالة JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_پیغام));
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
	إذا (junk_work_درخت) {
		strbuf_addstr(&sb, junk_work_درخت);
		remove_dir_recursively(&sb, junk_work_درخت_flags);
	}
	strbuf_release(&sb);
}

ساكن فراغ remove_junk_on_signal(صحيح signo)
{
	remove_junk();
	sigchain_نکالو(signo);
	ارفع(signo);
}

ساكن بنية ref *لبھو_remote_branch(ثابت بنية ref *refs, ثابت حرف *branch)
{
	بنية ref *ref;
	بنية strbuf سر = STRBUF_INIT;
	strbuf_addstr(&سر, "refs/heads/");
	strbuf_addstr(&سر, branch);
	ref = لبھو_ref_by_name(refs, سر.buf);
	strbuf_release(&سر);

	إذا (ref)
		أرجع ref;

	strbuf_addstr(&سر, "refs/tags/");
	strbuf_addstr(&سر, branch);
	ref = لبھو_ref_by_name(refs, سر.buf);
	strbuf_release(&سر);

	أرجع ref;
}

ساكن بنية ref *wanted_peer_refs(بنية clone_opts *opts,
				    ثابت بنية ref *refs,
				    بنية refspec *refspec)
{
	بنية ref *local_refs = عدم;
	بنية ref **پوچھ = &local_refs;
	بنية ref *منزل_free = عدم;

	إذا (opts->چاہندا_سر) {
		بنية ref *سر = copy_ref(لبھو_ref_by_name(refs, "HEAD"));
		إذا (سر)
			پوچھ_link_ref(سر, &پوچھ);
		إذا (option_single_branch)
			refs = منزل_free =
				guess_remote_سر(سر, refs,
						  REMOTE_GUESS_سر_QUIET);
	} وإلا إذا (option_single_branch) {
		local_refs = عدم;
		پوچھ = &local_refs;
		refs = منزل_free = copy_ref(لبھو_remote_branch(refs, option_branch));
	}

	لكل (ناپ_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->اشیاء[i], &پوچھ, 0);

	free_one_ref(منزل_free);

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

	لكل (r = local_refs; r; r = r->اگلا) {
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

ساكن فراغ write_followtags(ثابت بنية ref *refs, ثابت حرف *پیغام)
{
	ثابت بنية ref *ref;
	لكل (ref = refs; ref; ref = ref->اگلا) {
		إذا (!starts_with(ref->name, "refs/tags/"))
			استمر;
		إذا (سرے_with(ref->name, "^{}"))
			استمر;
		إذا (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			استمر;
		refs_update_ref(get_main_ref_store(the_repository), پیغام,
				ref->name, &ref->old_oid, عدم, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

ساكن ثابت بنية object_id *iterate_ref_map(فراغ *cb_انکڑے)
{
	بنية ref **rm = cb_انکڑے;
	بنية ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	طالما (ref && !ref->peer_ref)
		ref = ref->اگلا;
	إذا (!ref)
		أرجع عدم;

	*rm = ref->اگلا;
	أرجع &ref->old_oid;
}

ساكن فراغ update_remote_refs(ثابت بنية ref *refs,
			       ثابت بنية ref *mapped_refs,
			       ثابت بنية ref *remote_سر_points_at,
			       ثابت حرف *branch_چوٹی,
			       ثابت حرف *پیغام,
			       بنية transport *transport,
			       صحيح check_connectivity)
{
	ثابت بنية ref *rm = mapped_refs;

	إذا (check_connectivity) {
		بنية check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.ترقی = transport->ترقی;

		إذا (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	إذا (refs) {
		write_remote_refs(mapped_refs);
		إذا (option_single_branch && option_ٹیگ_سوچی)
			write_followtags(refs, پیغام);
	}

	إذا (remote_سر_points_at && remote_سر_points_at->peer_ref && !option_bare) {
		بنية strbuf سر_ref = STRBUF_INIT;
		strbuf_addstr(&سر_ref, branch_چوٹی);
		strbuf_addstr(&سر_ref, "HEAD");
		إذا (refs_update_symref(get_main_ref_store(the_repository), سر_ref.buf,
				       remote_سر_points_at->peer_ref->name,
				       پیغام) < 0)
			die(_("unable to update %s"), سر_ref.buf);
		strbuf_release(&سر_ref);
	}
}

ساكن فراغ update_سر(بنية clone_opts *opts, ثابت بنية ref *our, ثابت بنية ref *remote,
			ثابت حرف *unborn, ثابت حرف *پیغام)
{
	ثابت حرف *سر;
	إذا (our && !opts->detach && skip_اگیتر(our->name, "refs/heads/", &سر)) {
		/* Local default branch link */
		إذا (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, عدم) < 0)
			die(_("unable to update HEAD"));
		إذا (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					پیغام, "HEAD", &our->old_oid, عدم, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, سر, remote_name, our->name);
		}
	} وإلا إذا (our) {
		بنية commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), پیغام,
				"HEAD", &c->object.oid, عدم, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} وإلا إذا (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), پیغام,
				"HEAD", &remote->old_oid, عدم, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} وإلا إذا (unborn && skip_اگیتر(unborn, "refs/heads/", &سر)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		إذا (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, عدم) < 0)
			die(_("unable to update HEAD"));
		إذا (!option_bare)
			install_branch_config(0, سر, remote_name, unborn);
	}
}

ساكن صحيح git_sparse_checkout_init(ثابت حرف *repo)
{
	بنية بچہ_process cmd = بچہ_PROCESS_INIT;
	صحيح نتیجہ = 0;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	strvec_pushl(&cmd.دلیلاں, "-C", repo, "sparse-checkout", "set", عدم);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	إذا (لگاتار_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		نتیجہ = 1;
	}

	أرجع نتیجہ;
}

ساكن صحيح checkout(صحيح submodule_ترقی,
		    بنية list_objects_filter_options *filter_options,
		    صحيح filter_submodules,
		    تعداد ref_storage_format ref_storage_format)
{
	بنية object_id oid;
	حرف *سر;
	بنية جندرا_file جندرا_file = جندرا_INIT;
	بنية unpack_trees_options opts;
	بنية درخت *درخت;
	بنية درخت_desc t;
	صحيح err = 0;
	بنية لگاتار_hooks_opt hook_opt = لگاتار_HOOKS_OPT_INIT_FORCE_SERIAL;

	إذا (option_no_checkout)
		أرجع 0;

	سر = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, عدم);
	إذا (!سر) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		أرجع 0;
	}
	إذا (!strcmp(سر, "HEAD")) {
		إذا (advice_enabled(ADVICE_DETACHED_سر))
			detach_advice(oid_منزل_hex(&oid));
		FREE_AND_NULL(سر);
	} وإلا {
		إذا (!starts_with(سر, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	تیاری_work_درخت(the_repository);

	repo_hold_locked_اشاریہ(the_repository, &جندرا_file, جندرا_DIE_ON_ERROR);

	memset(&opts, 0, حجم opts);
	opts.update = 1;
	opts.ملاؤ = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_ملاؤ;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	init_checkout_metadata(&opts.meta, سر, &oid, عدم);

	درخت = repo_parse_درخت_اسدھا(the_repository, &oid);
	إذا (!درخت)
		die(_("unable to parse commit %s"), oid_منزل_hex(&oid));
	إذا (repo_parse_درخت(the_repository, درخت) < 0)
		باہر_نکلو(128);
	init_درخت_desc(&t, &درخت->object.oid, درخت->بفر, درخت->ناپ);
	إذا (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	حرر(سر);

	إذا (write_locked_اشاریہ(the_repository->اشاریہ, &جندرا_file, COMMIT_جندرا))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.دلیلاں, oid_منزل_hex(null_oid(the_hash_algo)),
		     oid_منزل_hex(&oid), "1", عدم);
	err |= لگاتار_hooks_opt(the_repository, "post-checkout", &hook_opt);

	إذا (!err && (option_recurse_submodules.nr > 0)) {
		بنية بچہ_process cmd = بچہ_PROCESS_INIT;
		strvec_pushl(&cmd.دلیلاں, "submodule", "update", "--require-init",
			     "--recursive", عدم);

		إذا (option_shallow_submodules == 1)
			strvec_ڈالو(&cmd.دلیلاں, "--depth=1");

		إذا (max_jobs != -1)
			strvec_pushf(&cmd.دلیلاں, "--jobs=%d", max_jobs);

		إذا (submodule_ترقی)
			strvec_ڈالو(&cmd.دلیلاں, "--progress");

		إذا (option_verbosity < 0)
			strvec_ڈالو(&cmd.دلیلاں, "--quiet");

		إذا (option_remote_submodules) {
			strvec_ڈالو(&cmd.دلیلاں, "--remote");
			strvec_ڈالو(&cmd.دلیلاں, "--no-fetch");
		}

		إذا (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.دلیلاں, "--ref-storage-format=%s",
				     ref_storage_format_منزل_name(ref_storage_format));

		إذا (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.دلیلاں, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		إذا (option_single_branch >= 0)
			strvec_ڈالو(&cmd.دلیلاں, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = لگاتار_command(&cmd);
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

ساكن صحيح write_one_config(ثابت حرف *کلید, ثابت حرف *قدر,
			    ثابت بنية config_context *ctx,
			    فراغ *انکڑے)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	صحيح apply_failed = git_clone_config(کلید, قدر, ctx, انکڑے);
	إذا (apply_failed)
		أرجع apply_failed;

	أرجع repo_config_سیٹ_multivar_gently(the_repository, کلید,
					       قدر ? قدر : "true",
					       CONFIG_REGEX_NONE, 0);
}

ساكن فراغ write_config(بنية string_list *config)
{
	صحيح i;

	لكل (i = 0; i < config->nr; i++) {
		إذا (git_config_parse_parameter(config->اشیاء[i].string,
					       write_one_config, عدم) < 0)
			die(_("unable to write parameters to config file"));
	}
}

ساكن فراغ write_refspec_config(ثابت حرف *src_ref_اگیتر,
		ثابت بنية ref *our_سر_points_at,
		ثابت بنية ref *remote_سر_points_at,
		بنية strbuf *branch_چوٹی)
{
	بنية strbuf کلید = STRBUF_INIT;
	بنية strbuf قدر = STRBUF_INIT;

	إذا (option_mirror || !option_bare) {
		إذا (option_single_branch && !option_mirror) {
			إذا (option_branch) {
				إذا (starts_with(our_سر_points_at->name, "refs/tags/"))
					strbuf_addf(&قدر, "+%s:%s", our_سر_points_at->name,
						our_سر_points_at->name);
				وإلا
					strbuf_addf(&قدر, "+%s:%s%s", our_سر_points_at->name,
						branch_چوٹی->buf, option_branch);
			} وإلا إذا (remote_سر_points_at) {
				ثابت حرف *سر = remote_سر_points_at->name;
				إذا (!skip_اگیتر(سر, "refs/heads/", &سر))
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
		} وإلا {
			strbuf_addf(&قدر, "+%s*:%s*", src_ref_اگیتر, branch_چوٹی->buf);
		}
		/* Configure the remote */
		إذا (قدر.len) {
			strbuf_addf(&کلید, "remote.%s.fetch", remote_name);
			repo_config_سیٹ_multivar(the_repository, کلید.buf, قدر.buf, "^$", 0);
			strbuf_reset(&کلید);

			إذا (option_mirror) {
				strbuf_addf(&کلید, "remote.%s.mirror", remote_name);
				repo_config_سیٹ(the_repository, کلید.buf, "true");
				strbuf_reset(&کلید);
			}
		}
	}

	strbuf_release(&کلید);
	strbuf_release(&قدر);
}

ساكن فراغ dissociate_from_references(فراغ)
{
	حرف *alternates = repo_git_رستہ(the_repository, "objects/info/alternates");

	إذا (!access(alternates, F_OK)) {
		بنية بچہ_process cmd = بچہ_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.دلیلاں, "repack", "-a", "-d", عدم);
		إذا (لگاتار_command(&cmd))
			die(_("cannot repack to clean up"));
		إذا (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	حرر(alternates);
}

ساكن صحيح رستہ_exists(ثابت حرف *رستہ)
{
	بنية stat sb;
	أرجع !stat(رستہ, &sb);
}

صحيح cmd_clone(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *اگیتر,
	      بنية repository *repository بن_ورتیا)
{
	صحيح is_bundle = 0, is_local;
	صحيح reject_shallow = 0;
	ثابت حرف *repo_name, *repo, *work_درخت, *git_dir;
	حرف *repo_منزل_free = عدم;
	حرف *رستہ = عدم, *dir, *display_repo = عدم;
	صحيح dest_exists, real_dest_exists = 0;
	ثابت بنية ref *refs, *remote_سر;
	بنية ref *remote_سر_points_at = عدم;
	ثابت بنية ref *our_سر_points_at;
	حرف *unborn_سر = عدم;
	بنية ref *mapped_refs = عدم;
	ثابت بنية ref *ref;
	بنية strbuf کلید = STRBUF_INIT;
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf branch_چوٹی = STRBUF_INIT, reflog_پیغام = STRBUF_INIT;
	بنية transport *transport = عدم;
	ثابت حرف *src_ref_اگیتر = "refs/heads/";
	بنية remote *remote;
	صحيح err = 0, complete_refs_before_fetch = 1;
	صحيح submodule_ترقی;
	صحيح filter_submodules = 0;
	صحيح hash_algo;
	تعداد ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	ثابت صحيح do_not_override_repo_unix_permissions = -1;
	صحيح option_reject_shallow = -1; /* unspecified */
	صحيح deepen = 0;
	حرف *option_template = عدم, *option_ڈونگھائی = عدم, *option_since = عدم;
	حرف *option_origin = عدم;
	بنية string_list option_not = STRING_LIST_INIT_NODUP;
	ثابت حرف *real_git_dir = عدم;
	ثابت حرف *ref_storage_format_uri = عدم;
	ثابت حرف *option_upload_pack = "git-upload-pack";
	صحيح option_ترقی = -1;
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
		OPT_BOOL(0, "progress", &option_ترقی,
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
		OPT_STRING(0, "depth", &option_ڈونگھائی, N_("depth"),
			   N_("create a shallow clone of that depth")),
		OPT_STRING(0, "shallow-since", &option_since, N_("time"),
			   N_("create a shallow clone since a specific time")),
		OPT_STRING_LIST(0, "shallow-exclude", &option_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_BOOL(0, "single-branch", &option_single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_BOOL(0, "tags", &option_ٹیگ_سوچی,
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
		OPT_سرا()
	};
	بنية strvec alternates = STRVEC_INIT;

	ثابت حرف * ثابت builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		عدم
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_پچھان_تت("clone");

	repo_config(the_repository, git_clone_config, عدم);

	argc = parse_options(argc, argv, اگیتر, builtin_clone_options,
			     builtin_clone_usage, 0);

	إذا (argc > 2)
		usage_پیغام_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	إذا (argc == 0)
		usage_پیغام_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	إذا (option_ڈونگھائی || option_since || option_not.nr)
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
		option_ٹیگ_سوچی = 0;
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

	رستہ = get_repo_رستہ(repo_name, &is_bundle);
	إذا (رستہ) {
		FREE_AND_NULL(رستہ);
		repo = repo_منزل_free = absolute_pathdup(repo_name);
	} وإلا إذا (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} وإلا
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	إذا (option_ڈونگھائی && atoi(option_ڈونگھائی) < 1)
		die(_("depth %s is not a positive number"), option_ڈونگھائی);

	إذا (argc == 2)
		dir = xstrdup(argv[1]);
	وإلا
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = رستہ_exists(dir);
	إذا (dest_exists && !is_خالی_تھاواں_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	إذا (real_git_dir) {
		real_dest_exists = رستہ_exists(real_git_dir);
		إذا (real_dest_exists && !is_خالی_تھاواں_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_پیغام, "clone: from %s",
		    display_repo ? display_repo : repo);
	حرر(display_repo);

	إذا (option_bare)
		work_درخت = عدم;
	وإلا {
		work_درخت = getenv("GIT_WORK_TREE");
		إذا (work_درخت && رستہ_exists(work_درخت))
			die(_("working tree '%s' already exists."), work_درخت);
	}

	إذا (option_bare || work_درخت)
		git_dir = xstrdup(dir);
	وإلا {
		work_درخت = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_ڈالو_common(remove_junk_on_signal);

	إذا (!option_bare) {
		إذا (محفوظ_create_leading_directories_const(the_repository, work_درخت) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_درخت);
		إذا (dest_exists)
			junk_work_درخت_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		وإلا إذا (mkdir(work_درخت, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_درخت);
		junk_work_درخت = work_درخت;
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
	إذا (محفوظ_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	إذا (0 <= option_verbosity) {
		إذا (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		وإلا
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	إذا (option_recurse_submodules.nr > 0) {
		بنية string_list_شے *شے;
		بنية strbuf sb = STRBUF_INIT;
		صحيح val;

		/* remove duplicates */
		string_list_ترتیب_دیو_u(&option_recurse_submodules, 0);

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
	create_repository(the_repository, git_dir, real_git_dir, work_درخت,
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
	initialize_repository_نسخہ(the_repository, GIT_HASH_UNKNOWN,
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
			src_ref_اگیتر = "refs/";
		strbuf_addstr(&branch_چوٹی, src_ref_اگیتر);

		repo_config_سیٹ(the_repository, "core.bare", "true");
	} وإلا إذا (!option_rev) {
		strbuf_addf(&branch_چوٹی, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&کلید, "remote.%s.url", remote_name);
	repo_config_سیٹ(the_repository, کلید.buf, repo);
	strbuf_reset(&کلید);

	إذا (!option_ٹیگ_سوچی) {
		strbuf_addf(&کلید, "remote.%s.tagOpt", remote_name);
		repo_config_سیٹ(the_repository, کلید.buf, "--no-tags");
		strbuf_reset(&کلید);
	}

	remote = remote_get_early(remote_name);

	إذا (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_اگیتر,
				branch_چوٹی.buf);

	رستہ = get_repo_رستہ(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && رستہ && !is_bundle;
	إذا (is_local) {
		إذا (option_ڈونگھائی)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		إذا (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		إذا (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		إذا (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		إذا (!access(mkpath("%s/shallow", رستہ), F_OK)) {
			إذا (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			إذا (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	إذا (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? رستہ : عدم);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, رستہ ? رستہ : remote->url.v[0]);
	transport_سیٹ_verbosity(transport, option_verbosity, option_ترقی);
	transport->family = family;
	transport->cloning = 1;

	إذا (is_bundle) {
		بنية bundle_سرخی سرخی = BUNDLE_سرخی_INIT;
		صحيح fd = read_bundle_سرخی(رستہ, &سرخی);
		صحيح has_filter = سرخی.filter.choice != LOFC_DISABLED;

		إذا (fd > 0)
			close(fd);
		bundle_سرخی_release(&سرخی);
		إذا (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_سیٹ_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	إذا (reject_shallow)
		transport_سیٹ_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	إذا (option_ڈونگھائی)
		transport_سیٹ_option(transport, TRANS_OPT_ڈونگھائی,
				     option_ڈونگھائی);
	إذا (option_since)
		transport_سیٹ_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	إذا (option_not.nr)
		transport_سیٹ_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (ثابت حرف *)&option_not);
	إذا (option_single_branch) {
		transport_سیٹ_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		إذا (option_branch)
			opts.چاہندا_سر = 0;
	}

	إذا (option_upload_pack)
		transport_سیٹ_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	إذا (server_options.nr)
		transport->server_options = &server_options;

	إذا (filter_options.choice) {
		ثابت حرف *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_سیٹ_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_سیٹ_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	إذا (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	إذا (option_rev) {
		option_ٹیگ_سوچی = 0;
		option_single_branch = 0;
		opts.چاہندا_سر = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	إذا (option_ٹیگ_سوچی || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, ٹیگ_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	إذا (option_branch)
		expand_ref_اگیتر(&transport_ls_refs_options.ref_prefixes,
				  option_branch);

	/*
	 * As part of transport_get_remote_refs() the server tells us the hash
	 * algorithm, which we require to initialize the repo. But calling that
	 * function without any ref prefix, will cause the server to announce
	 * all known refs. If the argument passed to --revision was a hex oid,
	 * ref_prefixes will be empty so we fall back to asking about HEAD to
	 * reduce traffic from the server.
	 */
	إذا (opts.چاہندا_سر || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_ڈالو(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_نسخہ(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_سیٹ_hash_algo(the_repository, hash_algo);
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
		إذا (repo_init(the_repository, git_dir, work_درخت))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		وإلا إذا (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		وإلا إذا (has_heuristic)
			repo_config_سیٹ_gently(the_repository, "fetch.bundleuri", bundle_uri);

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
		    hashmap_get_ناپ(&transport->bundles->bundles)) {
			بنية remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = عدم;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			إذا (repo_init(the_repository, git_dir, work_درخت))
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
		لكل (ref = refs; ref; ref = ref->اگلا)
			إذا (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				اكسر;
			}

		إذا (!is_local && !complete_refs_before_fetch) {
			إذا (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_سر = لبھو_ref_by_name(refs, "HEAD");
	remote_سر_points_at = guess_remote_سر(remote_سر, mapped_refs,
						  REMOTE_GUESS_سر_QUIET);

	إذا (option_branch) {
		our_سر_points_at = لبھو_remote_branch(mapped_refs, option_branch);
		إذا (!our_سر_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} وإلا إذا (option_rev) {
		our_سر_points_at = mapped_refs;
		إذا (!our_سر_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} وإلا إذا (remote_سر_points_at) {
		our_سر_points_at = remote_سر_points_at;
	} وإلا إذا (remote_سر) {
		our_سر_points_at = عدم;
	} وإلا {
		حرف *منزل_free = عدم;
		ثابت حرف *branch;

		إذا (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		إذا (transport_ls_refs_options.unborn_سر_نشانہ &&
		    skip_اگیتر(transport_ls_refs_options.unborn_سر_نشانہ,
				"refs/heads/", &branch)) {
			unborn_سر  = xstrdup(transport_ls_refs_options.unborn_سر_نشانہ);
		} وإلا {
			branch = منزل_free = repo_default_branch_name(the_repository, 0);
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
		our_سر_points_at = لبھو_remote_branch(mapped_refs, branch);

		حرر(منزل_free);
	}

	إذا (!option_rev)
		write_refspec_config(src_ref_اگیتر, our_سر_points_at,
				     remote_سر_points_at, &branch_چوٹی);

	إذا (filter_options.choice)
		partial_clone_رجسٹر(remote_name, &filter_options);

	إذا (is_local)
		clone_local(رستہ, git_dir);
	وإلا إذا (mapped_refs && complete_refs_before_fetch) {
		إذا (transport_fetch_refs(transport, mapped_refs))
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
	submodule_ترقی = transport->ترقی;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	إذا (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	إذا (option_sparse_checkout && git_sparse_checkout_init(dir))
		أرجع 1;

	junk_منوال = JUNK_LEAVE_REPO;
	err = checkout(submodule_ترقی,
		       &filter_options,
		       filter_submodules,
		       ref_storage_format);

	list_objects_filter_release(&filter_options);

	string_list_clear(&option_not, 0);
	string_list_clear(&option_config, 0);
	string_list_clear(&server_options, 0);
	strvec_clear(&alternates);

	حرر(remote_name);
	strbuf_release(&reflog_پیغام);
	strbuf_release(&branch_چوٹی);
	strbuf_release(&buf);
	strbuf_release(&کلید);
	free_refs(mapped_refs);
	free_refs(remote_سر_points_at);
	حرر(unborn_سر);
	حرر(dir);
	حرر(رستہ);
	حرر(repo_منزل_free);
	junk_منوال = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	أرجع err;
}
