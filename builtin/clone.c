/*
 * Builtin "git clone"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *		 2008 Daniel Barkalow <barkalow@iabervon.org>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 *
 * Clone a repository into a different directory that does not yet exist.
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"

#అన్తర్భావ "abspath.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "copy.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "refspec.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "unpack-trees.h"
#అన్తర్భావ "transport.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "dir-iterator.h"
#అన్తర్భావ "iterator.h"
#అన్తర్భావ "sigchain.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "connected.h"
#అన్తర్భావ "packfile.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "pkt-line.h"
#అన్తర్భావ "list-objects-filter-options.h"
#అన్తర్భావ "hook.h"
#అన్తర్భావ "bundle.h"
#అన్తర్భావ "bundle-uri.h"

/*
 * Overall FIXMEs:
 *  - respect DB_ENVIRONMENT for .git/objects.
 *
 * Implementation notes:
 *  - dropping use-separate-remote and no-separate-remote compatibility
 *
 */

నిర్మాణం clone_opts {
	పూర్ణాంకం కోరుతుంది_తల;
	పూర్ణాంకం detach;
};
#నిర్వచించు CLONE_OPTS_INIT { \
	.కోరుతుంది_తల = 1 /* default enabled */ \
}

స్థిర పూర్ణాంకం option_no_checkout, option_bare, option_mirror, option_single_branch = -1;
స్థిర పూర్ణాంకం option_local = -1, option_no_hardlinks, option_shared;
స్థిర పూర్ణాంకం option_ట్యాగులు = 1; /* default enabled */
స్థిర పూర్ణాంకం option_shallow_submodules;
స్థిర పూర్ణాంకం config_reject_shallow = -1;    /* unspecified */
స్థిర అక్షరం *remote_name = ఖాళీ;
స్థిర అక్షరం *option_branch = ఖాళీ;
స్థిర పూర్ణాంకం option_verbosity;
స్థిర నిర్మాణం string_list option_required_reference = STRING_LIST_INIT_NODUP;
స్థిర నిర్మాణం string_list option_optional_reference = STRING_LIST_INIT_NODUP;
స్థిర పూర్ణాంకం max_jobs = -1;
స్థిర నిర్మాణం string_list option_recurse_submodules = STRING_LIST_INIT_NODUP;
స్థిర పూర్ణాంకం config_filter_submodules = -1;    /* unspecified */
స్థిర పూర్ణాంకం option_remote_submodules;

స్థిర పూర్ణాంకం recurse_submodules_cb(స్థిరరాశి నిర్మాణం option *opt,
				 స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	ఒకవేళ (unset)
		string_list_clear((నిర్మాణం string_list *)opt->విలువ, 0);
	లేకపోతే ఒకవేళ (arg)
		string_list_append((నిర్మాణం string_list *)opt->విలువ, arg);
	లేకపోతే
		string_list_append((నిర్మాణం string_list *)opt->విలువ,
				   (స్థిరరాశి అక్షరం *)opt->defval);

	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *get_repo_మార్గం_1(నిర్మాణం strbuf *మార్గం, పూర్ణాంకం *is_bundle)
{
	స్థిర స్థిరరాశి అక్షరం *suffix[] = { "/.git", "", ".git/.git", ".git" };
	స్థిర స్థిరరాశి అక్షరం *bundle_suffix[] = { ".bundle", "" };
	కొలత_t baselen = మార్గం->len;
	నిర్మాణం stat st;
	పూర్ణాంకం i;

	కొరకు (i = 0; i < ARRAY_కొలత(suffix); i++) {
		strbuf_setlen(మార్గం, baselen);
		strbuf_addstr(మార్గం, suffix[i]);
		ఒకవేళ (stat(మార్గం->buf, &st))
			కొనసాగు;
		ఒకవేళ (S_ISDIR(st.st_బహుళకం) && is_git_directory(మార్గం->buf)) {
			*is_bundle = 0;
			తిరిగి మార్గం->buf;
		} లేకపోతే ఒకవేళ (S_ISREG(st.st_బహుళకం) && st.st_కొలత > 8) {
			/* Is it a "gitfile"? */
			అక్షరం signature[8];
			స్థిరరాశి అక్షరం *dst;
			పూర్ణాంకం len, fd = open(మార్గం->buf, O_RDONLY);
			ఒకవేళ (fd < 0)
				కొనసాగు;
			len = read_లోపల_నిండిన(fd, signature, 8);
			close(fd);
			ఒకవేళ (len != 8 || అక్షరశ్రేణి_పోలిక(signature, "gitdir: ", 8))
				కొనసాగు;
			dst = read_gitfile(మార్గం->buf);
			ఒకవేళ (dst) {
				*is_bundle = 0;
				తిరిగి dst;
			}
		}
	}

	కొరకు (i = 0; i < ARRAY_కొలత(bundle_suffix); i++) {
		strbuf_setlen(మార్గం, baselen);
		strbuf_addstr(మార్గం, bundle_suffix[i]);
		ఒకవేళ (!stat(మార్గం->buf, &st) && S_ISREG(st.st_బహుళకం)) {
			*is_bundle = 1;
			తిరిగి మార్గం->buf;
		}
	}

	తిరిగి ఖాళీ;
}

స్థిర అక్షరం *get_repo_మార్గం(స్థిరరాశి అక్షరం *repo, పూర్ణాంకం *is_bundle)
{
	నిర్మాణం strbuf మార్గం = STRBUF_INIT;
	స్థిరరాశి అక్షరం *raw;
	అక్షరం *canon;

	strbuf_addstr(&మార్గం, repo);
	raw = get_repo_మార్గం_1(&మార్గం, is_bundle);
	canon = raw ? absolute_pathdup(raw) : ఖాళీ;
	strbuf_release(&మార్గం);
	తిరిగి canon;
}

నిర్మాణం collect_alternates_దత్తాంశం {
	నిర్మాణం strvec *alternates;
	bool required;
};

స్థిర పూర్ణాంకం collect_one_alternate(నిర్మాణం string_list_అంశం *అంశం, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం collect_alternates_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;
	నిర్మాణం strbuf err = STRBUF_INIT;
	అక్షరం *ref_git = compute_alternate_మార్గం(అంశం->string, &err);

	ఒకవేళ (!ref_git) {
		ఒకవేళ (దత్తాంశం->required)
			die("%s", err.buf);
		లేకపోతే
			fprintf(stderr,
				_("info: Could not add alternate for '%s': %s\n"),
				అంశం->string, err.buf);
	} లేకపోతే {
		strvec_pushf(దత్తాంశం->alternates, "%s/objects", ref_git);
	}

	strbuf_release(&err);
	విడుదల(ref_git);
	తిరిగి 0;
}

స్థిర శూన్య read_alternates(నిర్మాణం strvec *alternates, స్థిరరాశి అక్షరం *src_repo)
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
	FILE *లోపల;
	నిర్మాణం strbuf మార్గం = STRBUF_INIT;
	నిర్మాణం strbuf పంక్తి = STRBUF_INIT;

	strbuf_addf(&మార్గం, "%s/objects/info/alternates", src_repo);

	లోపల = fopen(మార్గం.buf, "r");
	ఒకవేళ (!లోపల) {
		ఒకవేళ (errno == ENOENT)
			వెళ్ళు బహిర్గతం;
		die_errno("could not read alternates file '%s'", మార్గం.buf);
	}

	అయితే (strbuf_getline(&పంక్తి, లోపల) != EOF) {
		అక్షరం *abs_మార్గం;
		ఒకవేళ (!పంక్తి.len || పంక్తి.buf[0] == '#')
			కొనసాగు;
		ఒకవేళ (is_absolute_మార్గం(పంక్తి.buf)) {
			strvec_నెట్టు(alternates, పంక్తి.buf);
			కొనసాగు;
		}
		abs_మార్గం = mkpathdup("%s/objects/%s", src_repo, పంక్తి.buf);
		ఒకవేళ (!normalize_మార్గం_copy(abs_మార్గం, abs_మార్గం))
			strvec_నెట్టు(alternates, abs_మార్గం);
		లేకపోతే
			warning("skipping invalid relative alternate: %s/%s",
				src_repo, పంక్తి.buf);
		విడుదల(abs_మార్గం);
	}

బహిర్గతం:
	strbuf_release(&మార్గం);
	strbuf_release(&పంక్తి);
	ఒకవేళ (లోపల)
		fclose(లోపల);
}

స్థిర శూన్య collect_alternates(నిర్మాణం strvec *alternates,
			       స్థిరరాశి అక్షరం *local_ఆధారం_repo)
{
	ఒకవేళ (option_required_reference.nr || option_optional_reference.nr) {
		నిర్మాణం collect_alternates_దత్తాంశం దత్తాంశం = {
			.alternates = alternates,
			.required = true,
		};

		for_each_string_list(&option_required_reference,
				     collect_one_alternate, &దత్తాంశం);
		దత్తాంశం.required = false;
		for_each_string_list(&option_optional_reference,
				     collect_one_alternate, &దత్తాంశం);
	}

	ఒకవేళ (local_ఆధారం_repo) {
		నిర్మాణం strbuf commondir = STRBUF_INIT;

		get_common_dir(&commondir, local_ఆధారం_repo);
		ఒకవేళ (option_shared)
			strvec_pushf(alternates, "%s/objects", commondir.buf);
		లేకపోతే
			read_alternates(alternates, commondir.buf);

		strbuf_release(&commondir);
	}
}

స్థిర శూన్య mkdir_if_missing(స్థిరరాశి అక్షరం *pathname, బహుళకం_t బహుళకం)
{
	నిర్మాణం stat st;

	ఒకవేళ (!mkdir(pathname, బహుళకం))
		తిరిగి;

	ఒకవేళ (errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), pathname);
	లేకపోతే ఒకవేళ (stat(pathname, &st))
		die_errno(_("failed to stat '%s'"), pathname);
	లేకపోతే ఒకవేళ (!S_ISDIR(st.st_బహుళకం))
		die(_("%s exists and is not a directory"), pathname);
}

స్థిర శూన్య copy_or_link_directory(నిర్మాణం strbuf *src, నిర్మాణం strbuf *dest,
				   స్థిరరాశి అక్షరం *src_repo)
{
	పూర్ణాంకం src_len, dest_len;
	నిర్మాణం dir_iterator *iter;
	పూర్ణాంకం iter_స్థితి;

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
	die_upon_dubious_ownership(ఖాళీ, ఖాళీ, src_repo);

	mkdir_if_missing(dest->buf, 0777);

	iter = dir_iterator_begin(src->buf, DIR_ITERATOR_PEDANTIC);

	ఒకవేళ (!iter) {
		ఒకవేళ (errno == ENOTDIR) {
			పూర్ణాంకం saved_errno = errno;
			నిర్మాణం stat st;

			ఒకవేళ (!lstat(src->buf, &st) && S_ISLNK(st.st_బహుళకం))
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

	అయితే ((iter_స్థితి = dir_iterator_advance(iter)) == ITER_OK) {
		strbuf_setlen(src, src_len);
		strbuf_addstr(src, iter->relative_మార్గం);
		strbuf_setlen(dest, dest_len);
		strbuf_addstr(dest, iter->relative_మార్గం);

		ఒకవేళ (S_ISLNK(iter->st.st_బహుళకం))
			die(_("symlink '%s' exists, refusing to clone with --local"),
			    iter->relative_మార్గం);

		ఒకవేళ (S_ISDIR(iter->st.st_బహుళకం)) {
			mkdir_if_missing(dest->buf, 0777);
			కొనసాగు;
		}

		/* Alternates were already handled earlier. */
		ఒకవేళ (!fspathcmp(iter->relative_మార్గం, "info/alternates"))
			కొనసాగు;

		ఒకవేళ (unlink(dest->buf) && errno != ENOENT)
			die_errno(_("failed to unlink '%s'"), dest->buf);
		ఒకవేళ (!option_no_hardlinks) {
			ఒకవేళ (!link(src->buf, dest->buf)) {
				నిర్మాణం stat st;

				/*
				 * Sanity-check whether the created hardlink
				 * actually links to the expected file now. This
				 * catches time-of-check-time-of-use bugs in
				 * case the source file was meanwhile swapped.
				 */
				ఒకవేళ (lstat(dest->buf, &st))
					die(_("hardlink cannot be checked at '%s'"), dest->buf);
				ఒకవేళ (st.st_బహుళకం != iter->st.st_బహుళకం ||
				    st.st_ino != iter->st.st_ino ||
				    st.st_dev != iter->st.st_dev ||
				    st.st_కొలత != iter->st.st_కొలత ||
				    st.st_uid != iter->st.st_uid ||
				    st.st_gid != iter->st.st_gid)
					die(_("hardlink different from source at '%s'"), dest->buf);

				కొనసాగు;
			}
			ఒకవేళ (option_local > 0)
				die_errno(_("failed to create link '%s'"), dest->buf);
			option_no_hardlinks = 1;
		}
		ఒకవేళ (copy_file_with_సమయం(the_repository, dest->buf, src->buf, 0666))
			die_errno(_("failed to copy file to '%s'"), dest->buf);
	}

	ఒకవేళ (iter_స్థితి != ITER_పూర్తి) {
		strbuf_setlen(src, src_len);
		die(_("failed to iterate over '%s'"), src->buf);
	}

	dir_iterator_free(iter);
}

స్థిర శూన్య clone_local(స్థిరరాశి అక్షరం *src_repo, స్థిరరాశి అక్షరం *dest_repo)
{
	ఒకవేళ (!option_shared) {
		నిర్మాణం strbuf src = STRBUF_INIT;
		నిర్మాణం strbuf dest = STRBUF_INIT;
		get_common_dir(&src, src_repo);
		get_common_dir(&dest, dest_repo);
		strbuf_addstr(&src, "/objects");
		strbuf_addstr(&dest, "/objects");
		copy_or_link_directory(&src, &dest, src_repo);
		strbuf_release(&src);
		strbuf_release(&dest);
	}

	ఒకవేళ (0 <= option_verbosity)
		fprintf(stderr, _("done.\n"));
}

స్థిర స్థిరరాశి అక్షరం *junk_work_వృక్షం;
స్థిర పూర్ణాంకం junk_work_వృక్షం_flags;
స్థిర స్థిరరాశి అక్షరం *junk_git_dir;
స్థిర పూర్ణాంకం junk_git_dir_flags;
స్థిర గణన {
	JUNK_LEAVE_NONE,
	JUNK_LEAVE_REPO,
	JUNK_LEAVE_ALL
} junk_బహుళకం = JUNK_LEAVE_NONE;

స్థిర స్థిరరాశి అక్షరం junk_leave_repo_సందేశం[] =
N_("Clone succeeded, but checkout failed.\n"
   "You can inspect what was checked out with 'git status'\n"
   "and retry with 'git restore --source=HEAD :/'\n");

స్థిర శూన్య remove_junk(శూన్య)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;

	ఎంపిక (junk_బహుళకం) {
	స్థితి JUNK_LEAVE_REPO:
		warning("%s", _(junk_leave_repo_సందేశం));
		/* fall-through */
	స్థితి JUNK_LEAVE_ALL:
		తిరిగి;
	అప్రమేయ:
		/* proceed to removal */
		ఆపు;
	}

	ఒకవేళ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, junk_git_dir_flags);
		strbuf_reset(&sb);
	}
	ఒకవేళ (junk_work_వృక్షం) {
		strbuf_addstr(&sb, junk_work_వృక్షం);
		remove_dir_recursively(&sb, junk_work_వృక్షం_flags);
	}
	strbuf_release(&sb);
}

స్థిర శూన్య remove_junk_on_signal(పూర్ణాంకం signo)
{
	remove_junk();
	sigchain_తీయి(signo);
	లేపు(signo);
}

స్థిర నిర్మాణం ref *కనుగొను_remote_branch(స్థిరరాశి నిర్మాణం ref *refs, స్థిరరాశి అక్షరం *branch)
{
	నిర్మాణం ref *ref;
	నిర్మాణం strbuf తల = STRBUF_INIT;
	strbuf_addstr(&తల, "refs/heads/");
	strbuf_addstr(&తల, branch);
	ref = కనుగొను_ref_by_name(refs, తల.buf);
	strbuf_release(&తల);

	ఒకవేళ (ref)
		తిరిగి ref;

	strbuf_addstr(&తల, "refs/tags/");
	strbuf_addstr(&తల, branch);
	ref = కనుగొను_ref_by_name(refs, తల.buf);
	strbuf_release(&తల);

	తిరిగి ref;
}

స్థిర నిర్మాణం ref *wanted_peer_refs(నిర్మాణం clone_opts *opts,
				    స్థిరరాశి నిర్మాణం ref *refs,
				    నిర్మాణం refspec *refspec)
{
	నిర్మాణం ref *local_refs = ఖాళీ;
	నిర్మాణం ref **తోక = &local_refs;
	నిర్మాణం ref *గమ్యం_free = ఖాళీ;

	ఒకవేళ (opts->కోరుతుంది_తల) {
		నిర్మాణం ref *తల = copy_ref(కనుగొను_ref_by_name(refs, "HEAD"));
		ఒకవేళ (తల)
			తోక_link_ref(తల, &తోక);
		ఒకవేళ (option_single_branch)
			refs = గమ్యం_free =
				guess_remote_తల(తల, refs,
						  REMOTE_GUESS_తల_QUIET);
	} లేకపోతే ఒకవేళ (option_single_branch) {
		local_refs = ఖాళీ;
		తోక = &local_refs;
		refs = గమ్యం_free = copy_ref(కనుగొను_remote_branch(refs, option_branch));
	}

	కొరకు (కొలత_t i = 0; i < refspec->nr; i++)
		get_fetch_map(refs, &refspec->వస్తువులు[i], &తోక, 0);

	free_one_ref(గమ్యం_free);

	తిరిగి local_refs;
}

స్థిర శూన్య write_remote_refs(స్థిరరాశి నిర్మాణం ref *local_refs)
{
	స్థిరరాశి నిర్మాణం ref *r;

	నిర్మాణం ref_transaction *t;
	నిర్మాణం strbuf err = STRBUF_INIT;

	t = ref_store_transaction_begin(get_main_ref_store(the_repository),
					REF_TRANSACTION_FLAG_INITIAL, &err);
	ఒకవేళ (!t)
		die("%s", err.buf);

	కొరకు (r = local_refs; r; r = r->తదుపరి) {
		ఒకవేళ (!r->peer_ref)
			కొనసాగు;
		ఒకవేళ (ref_transaction_create(t, r->peer_ref->name, &r->old_oid,
					   ఖాళీ, 0, ఖాళీ, &err))
			die("%s", err.buf);
	}

	ఒకవేళ (ref_transaction_commit(t, &err))
		die("%s", err.buf);

	strbuf_release(&err);
	ref_transaction_free(t);
}

స్థిర శూన్య write_followtags(స్థిరరాశి నిర్మాణం ref *refs, స్థిరరాశి అక్షరం *సందేశం)
{
	స్థిరరాశి నిర్మాణం ref *ref;
	కొరకు (ref = refs; ref; ref = ref->తదుపరి) {
		ఒకవేళ (!starts_with(ref->name, "refs/tags/"))
			కొనసాగు;
		ఒకవేళ (చివర్లు_with(ref->name, "^{}"))
			కొనసాగు;
		ఒకవేళ (!odb_has_object(the_repository->objects, &ref->old_oid, 0))
			కొనసాగు;
		refs_update_ref(get_main_ref_store(the_repository), సందేశం,
				ref->name, &ref->old_oid, ఖాళీ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	}
}

స్థిర స్థిరరాశి నిర్మాణం object_id *iterate_ref_map(శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం ref **rm = cb_దత్తాంశం;
	నిర్మాణం ref *ref = *rm;

	/*
	 * Skip anything missing a peer_ref, which we are not
	 * actually going to write a ref for.
	 */
	అయితే (ref && !ref->peer_ref)
		ref = ref->తదుపరి;
	ఒకవేళ (!ref)
		తిరిగి ఖాళీ;

	*rm = ref->తదుపరి;
	తిరిగి &ref->old_oid;
}

స్థిర శూన్య update_remote_refs(స్థిరరాశి నిర్మాణం ref *refs,
			       స్థిరరాశి నిర్మాణం ref *mapped_refs,
			       స్థిరరాశి నిర్మాణం ref *remote_తల_points_at,
			       స్థిరరాశి అక్షరం *branch_శిఖరం,
			       స్థిరరాశి అక్షరం *సందేశం,
			       నిర్మాణం transport *transport,
			       పూర్ణాంకం check_connectivity)
{
	స్థిరరాశి నిర్మాణం ref *rm = mapped_refs;

	ఒకవేళ (check_connectivity) {
		నిర్మాణం check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.transport = transport;
		opt.పురోగతి = transport->పురోగతి;

		ఒకవేళ (check_connected(iterate_ref_map, &rm, &opt))
			die(_("remote did not send all necessary objects"));
	}

	ఒకవేళ (refs) {
		write_remote_refs(mapped_refs);
		ఒకవేళ (option_single_branch && option_ట్యాగులు)
			write_followtags(refs, సందేశం);
	}

	ఒకవేళ (remote_తల_points_at && remote_తల_points_at->peer_ref && !option_bare) {
		నిర్మాణం strbuf తల_ref = STRBUF_INIT;
		strbuf_addstr(&తల_ref, branch_శిఖరం);
		strbuf_addstr(&తల_ref, "HEAD");
		ఒకవేళ (refs_update_symref(get_main_ref_store(the_repository), తల_ref.buf,
				       remote_తల_points_at->peer_ref->name,
				       సందేశం) < 0)
			die(_("unable to update %s"), తల_ref.buf);
		strbuf_release(&తల_ref);
	}
}

స్థిర శూన్య update_తల(నిర్మాణం clone_opts *opts, స్థిరరాశి నిర్మాణం ref *our, స్థిరరాశి నిర్మాణం ref *remote,
			స్థిరరాశి అక్షరం *unborn, స్థిరరాశి అక్షరం *సందేశం)
{
	స్థిరరాశి అక్షరం *తల;
	ఒకవేళ (our && !opts->detach && skip_ఉపసర్గ(our->name, "refs/heads/", &తల)) {
		/* Local default branch link */
		ఒకవేళ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", our->name, ఖాళీ) < 0)
			die(_("unable to update HEAD"));
		ఒకవేళ (!option_bare) {
			refs_update_ref(get_main_ref_store(the_repository),
					సందేశం, "HEAD", &our->old_oid, ఖాళీ, 0,
					UPDATE_REFS_DIE_ON_ERR);
			install_branch_config(0, తల, remote_name, our->name);
		}
	} లేకపోతే ఒకవేళ (our) {
		నిర్మాణం commit *c = lookup_commit_or_die(&our->old_oid,
							our->name);

		/* --branch specifies a non-branch (i.e. tags), detach HEAD */
		refs_update_ref(get_main_ref_store(the_repository), సందేశం,
				"HEAD", &c->object.oid, ఖాళీ, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} లేకపోతే ఒకవేళ (remote) {
		/*
		 * We know remote HEAD points to a non-branch, or
		 * HEAD points to a branch but we don't know which one.
		 * Detach HEAD in all these cases.
		 */
		refs_update_ref(get_main_ref_store(the_repository), సందేశం,
				"HEAD", &remote->old_oid, ఖాళీ, REF_NO_DEREF,
				UPDATE_REFS_DIE_ON_ERR);
	} లేకపోతే ఒకవేళ (unborn && skip_ఉపసర్గ(unborn, "refs/heads/", &తల)) {
		/*
		 * Unborn head from remote; same as "our" case above except
		 * that we have no ref to update.
		 */
		ఒకవేళ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", unborn, ఖాళీ) < 0)
			die(_("unable to update HEAD"));
		ఒకవేళ (!option_bare)
			install_branch_config(0, తల, remote_name, unborn);
	}
}

స్థిర పూర్ణాంకం git_sparse_checkout_init(స్థిరరాశి అక్షరం *repo)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;
	పూర్ణాంకం ఫలితం = 0;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	strvec_pushl(&cmd.వాదనలు, "-C", repo, "sparse-checkout", "set", ఖాళీ);

	/*
	 * We must apply the setting in the current process
	 * for the later checkout to use the sparse-checkout file.
	 */
	cfg->apply_sparse_checkout = 1;

	cmd.git_cmd = 1;
	ఒకవేళ (నిరంతరం_command(&cmd)) {
		error(_("failed to initialize sparse-checkout"));
		ఫలితం = 1;
	}

	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం checkout(పూర్ణాంకం submodule_పురోగతి,
		    నిర్మాణం list_objects_filter_options *filter_options,
		    పూర్ణాంకం filter_submodules,
		    గణన ref_storage_format ref_storage_format)
{
	నిర్మాణం object_id oid;
	అక్షరం *తల;
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	నిర్మాణం unpack_trees_options opts;
	నిర్మాణం వృక్షం *వృక్షం;
	నిర్మాణం వృక్షం_desc t;
	పూర్ణాంకం err = 0;
	నిర్మాణం నిరంతరం_hooks_opt hook_opt = నిరంతరం_HOOKS_OPT_INIT_FORCE_SERIAL;

	ఒకవేళ (option_no_checkout)
		తిరిగి 0;

	తల = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING, &oid, ఖాళీ);
	ఒకవేళ (!తల) {
		warning(_("remote HEAD refers to nonexistent ref, "
			  "unable to checkout"));
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(తల, "HEAD")) {
		ఒకవేళ (advice_enabled(ADVICE_DETACHED_తల))
			detach_advice(oid_గమ్యం_hex(&oid));
		FREE_AND_NULL(తల);
	} లేకపోతే {
		ఒకవేళ (!starts_with(తల, "refs/heads/"))
			die(_("HEAD not found below refs/heads!"));
	}

	/* We need to be in the new work tree for the checkout */
	అమరిక_work_వృక్షం(the_repository);

	repo_hold_locked_సూచీ(the_repository, &తాళం_file, తాళం_DIE_ON_ERROR);

	memset(&opts, 0, పరిమాణం opts);
	opts.update = 1;
	opts.విలీనం = 1;
	opts.clone = 1;
	opts.preserve_ignored = 0;
	opts.fn = oneway_విలీనం;
	opts.verbose_update = (option_verbosity >= 0);
	opts.src_సూచీ = the_repository->సూచీ;
	opts.dst_సూచీ = the_repository->సూచీ;
	init_checkout_metadata(&opts.meta, తల, &oid, ఖాళీ);

	వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository, &oid);
	ఒకవేళ (!వృక్షం)
		die(_("unable to parse commit %s"), oid_గమ్యం_hex(&oid));
	ఒకవేళ (repo_parse_వృక్షం(the_repository, వృక్షం) < 0)
		నిష్క్రమించు(128);
	init_వృక్షం_desc(&t, &వృక్షం->object.oid, వృక్షం->బఫరు, వృక్షం->కొలత);
	ఒకవేళ (unpack_trees(1, &t, &opts) < 0)
		die(_("unable to checkout working tree"));

	విడుదల(తల);

	ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం_file, COMMIT_తాళం))
		die(_("unable to write new index file"));

	strvec_pushl(&hook_opt.వాదనలు, oid_గమ్యం_hex(null_oid(the_hash_algo)),
		     oid_గమ్యం_hex(&oid), "1", ఖాళీ);
	err |= నిరంతరం_hooks_opt(the_repository, "post-checkout", &hook_opt);

	ఒకవేళ (!err && (option_recurse_submodules.nr > 0)) {
		నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;
		strvec_pushl(&cmd.వాదనలు, "submodule", "update", "--require-init",
			     "--recursive", ఖాళీ);

		ఒకవేళ (option_shallow_submodules == 1)
			strvec_నెట్టు(&cmd.వాదనలు, "--depth=1");

		ఒకవేళ (max_jobs != -1)
			strvec_pushf(&cmd.వాదనలు, "--jobs=%d", max_jobs);

		ఒకవేళ (submodule_పురోగతి)
			strvec_నెట్టు(&cmd.వాదనలు, "--progress");

		ఒకవేళ (option_verbosity < 0)
			strvec_నెట్టు(&cmd.వాదనలు, "--quiet");

		ఒకవేళ (option_remote_submodules) {
			strvec_నెట్టు(&cmd.వాదనలు, "--remote");
			strvec_నెట్టు(&cmd.వాదనలు, "--no-fetch");
		}

		ఒకవేళ (ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cmd.వాదనలు, "--ref-storage-format=%s",
				     ref_storage_format_గమ్యం_name(ref_storage_format));

		ఒకవేళ (filter_submodules && filter_options->choice)
			strvec_pushf(&cmd.వాదనలు, "--filter=%s",
				     expand_list_objects_filter_spec(filter_options));

		ఒకవేళ (option_single_branch >= 0)
			strvec_నెట్టు(&cmd.వాదనలు, option_single_branch ?
					       "--single-branch" :
					       "--no-single-branch");

		cmd.git_cmd = 1;
		err = నిరంతరం_command(&cmd);
	}

	తిరిగి err;
}

స్థిర పూర్ణాంకం git_clone_config(స్థిరరాశి అక్షరం *k, స్థిరరాశి అక్షరం *v,
			    స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(k, "clone.defaultremotename")) {
		ఒకవేళ (!v)
			తిరిగి config_error_nonbool(k);
		విడుదల(remote_name);
		remote_name = xstrdup(v);
	}
	ఒకవేళ (!strcmp(k, "clone.rejectshallow"))
		config_reject_shallow = git_config_bool(k, v);
	ఒకవేళ (!strcmp(k, "clone.filtersubmodules"))
		config_filter_submodules = git_config_bool(k, v);

	తిరిగి git_default_config(k, v, ctx, cb);
}

స్థిర పూర్ణాంకం write_one_config(స్థిరరాశి అక్షరం *కీలకం, స్థిరరాశి అక్షరం *విలువ,
			    స్థిరరాశి నిర్మాణం config_context *ctx,
			    శూన్య *దత్తాంశం)
{
	/*
	 * give git_clone_config a chance to write config values back to the
	 * environment, since repo_config_set_multivar_gently only deals with
	 * config-file writes
	 */
	పూర్ణాంకం apply_failed = git_clone_config(కీలకం, విలువ, ctx, దత్తాంశం);
	ఒకవేళ (apply_failed)
		తిరిగి apply_failed;

	తిరిగి repo_config_సమితి_multivar_gently(the_repository, కీలకం,
					       విలువ ? విలువ : "true",
					       CONFIG_REGEX_NONE, 0);
}

స్థిర శూన్య write_config(నిర్మాణం string_list *config)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < config->nr; i++) {
		ఒకవేళ (git_config_parse_parameter(config->వస్తువులు[i].string,
					       write_one_config, ఖాళీ) < 0)
			die(_("unable to write parameters to config file"));
	}
}

స్థిర శూన్య write_refspec_config(స్థిరరాశి అక్షరం *src_ref_ఉపసర్గ,
		స్థిరరాశి నిర్మాణం ref *our_తల_points_at,
		స్థిరరాశి నిర్మాణం ref *remote_తల_points_at,
		నిర్మాణం strbuf *branch_శిఖరం)
{
	నిర్మాణం strbuf కీలకం = STRBUF_INIT;
	నిర్మాణం strbuf విలువ = STRBUF_INIT;

	ఒకవేళ (option_mirror || !option_bare) {
		ఒకవేళ (option_single_branch && !option_mirror) {
			ఒకవేళ (option_branch) {
				ఒకవేళ (starts_with(our_తల_points_at->name, "refs/tags/"))
					strbuf_addf(&విలువ, "+%s:%s", our_తల_points_at->name,
						our_తల_points_at->name);
				లేకపోతే
					strbuf_addf(&విలువ, "+%s:%s%s", our_తల_points_at->name,
						branch_శిఖరం->buf, option_branch);
			} లేకపోతే ఒకవేళ (remote_తల_points_at) {
				స్థిరరాశి అక్షరం *తల = remote_తల_points_at->name;
				ఒకవేళ (!skip_ఉపసర్గ(తల, "refs/heads/", &తల))
					BUG("remote HEAD points at non-head?");

				strbuf_addf(&విలువ, "+%s:%s%s", remote_తల_points_at->name,
						branch_శిఖరం->buf, తల);
			}
			/*
			 * otherwise, the next "git fetch" will
			 * simply fetch from HEAD without updating
			 * any remote-tracking branch, which is what
			 * we want.
			 */
		} లేకపోతే {
			strbuf_addf(&విలువ, "+%s*:%s*", src_ref_ఉపసర్గ, branch_శిఖరం->buf);
		}
		/* Configure the remote */
		ఒకవేళ (విలువ.len) {
			strbuf_addf(&కీలకం, "remote.%s.fetch", remote_name);
			repo_config_సమితి_multivar(the_repository, కీలకం.buf, విలువ.buf, "^$", 0);
			strbuf_reset(&కీలకం);

			ఒకవేళ (option_mirror) {
				strbuf_addf(&కీలకం, "remote.%s.mirror", remote_name);
				repo_config_సమితి(the_repository, కీలకం.buf, "true");
				strbuf_reset(&కీలకం);
			}
		}
	}

	strbuf_release(&కీలకం);
	strbuf_release(&విలువ);
}

స్థిర శూన్య dissociate_from_references(శూన్య)
{
	అక్షరం *alternates = repo_git_మార్గం(the_repository, "objects/info/alternates");

	ఒకవేళ (!access(alternates, F_OK)) {
		నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

		cmd.git_cmd = 1;
		cmd.no_stdin = 1;
		strvec_pushl(&cmd.వాదనలు, "repack", "-a", "-d", ఖాళీ);
		ఒకవేళ (నిరంతరం_command(&cmd))
			die(_("cannot repack to clean up"));
		ఒకవేళ (unlink(alternates) && errno != ENOENT)
			die_errno(_("cannot unlink temporary alternates file"));
	}
	విడుదల(alternates);
}

స్థిర పూర్ణాంకం మార్గం_exists(స్థిరరాశి అక్షరం *మార్గం)
{
	నిర్మాణం stat sb;
	తిరిగి !stat(మార్గం, &sb);
}

పూర్ణాంకం cmd_clone(పూర్ణాంకం argc,
	      స్థిరరాశి అక్షరం **argv,
	      స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repository వాడని)
{
	పూర్ణాంకం is_bundle = 0, is_local;
	పూర్ణాంకం reject_shallow = 0;
	స్థిరరాశి అక్షరం *repo_name, *repo, *work_వృక్షం, *git_dir;
	అక్షరం *repo_గమ్యం_free = ఖాళీ;
	అక్షరం *మార్గం = ఖాళీ, *dir, *display_repo = ఖాళీ;
	పూర్ణాంకం dest_exists, real_dest_exists = 0;
	స్థిరరాశి నిర్మాణం ref *refs, *remote_తల;
	నిర్మాణం ref *remote_తల_points_at = ఖాళీ;
	స్థిరరాశి నిర్మాణం ref *our_తల_points_at;
	అక్షరం *unborn_తల = ఖాళీ;
	నిర్మాణం ref *mapped_refs = ఖాళీ;
	స్థిరరాశి నిర్మాణం ref *ref;
	నిర్మాణం strbuf కీలకం = STRBUF_INIT;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం strbuf branch_శిఖరం = STRBUF_INIT, reflog_సందేశం = STRBUF_INIT;
	నిర్మాణం transport *transport = ఖాళీ;
	స్థిరరాశి అక్షరం *src_ref_ఉపసర్గ = "refs/heads/";
	నిర్మాణం remote *remote;
	పూర్ణాంకం err = 0, complete_refs_before_fetch = 1;
	పూర్ణాంకం submodule_పురోగతి;
	పూర్ణాంకం filter_submodules = 0;
	పూర్ణాంకం hash_algo;
	గణన ref_storage_format ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN;
	స్థిరరాశి పూర్ణాంకం do_not_override_repo_unix_permissions = -1;
	పూర్ణాంకం option_reject_shallow = -1; /* unspecified */
	పూర్ణాంకం deepen = 0;
	అక్షరం *option_template = ఖాళీ, *option_లోతు = ఖాళీ, *option_since = ఖాళీ;
	అక్షరం *option_origin = ఖాళీ;
	నిర్మాణం string_list option_not = STRING_LIST_INIT_NODUP;
	స్థిరరాశి అక్షరం *real_git_dir = ఖాళీ;
	స్థిరరాశి అక్షరం *ref_storage_format_uri = ఖాళీ;
	స్థిరరాశి అక్షరం *option_upload_pack = "git-upload-pack";
	పూర్ణాంకం option_పురోగతి = -1;
	పూర్ణాంకం option_sparse_checkout = 0;
	గణన transport_family family = TRANSPORT_FAMILY_ALL;
	నిర్మాణం string_list option_config = STRING_LIST_INIT_DUP;
	పూర్ణాంకం option_dissociate = 0;
	నిర్మాణం list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	పూర్ణాంకం option_filter_submodules = -1; /* unspecified */
	నిర్మాణం string_list server_options = STRING_LIST_INIT_NODUP;
	స్థిరరాశి అక్షరం *bundle_uri = ఖాళీ;
	అక్షరం *option_rev = ఖాళీ;

	నిర్మాణం clone_opts opts = CLONE_OPTS_INIT;

	నిర్మాణం transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;

	నిర్మాణం option builtin_clone_options[] = {
		OPT__VERBOSITY(&option_verbosity),
		OPT_BOOL(0, "progress", &option_పురోగతి,
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
			.విలువ = &option_recurse_submodules,
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
		OPT_STRING(0, "depth", &option_లోతు, N_("depth"),
			   N_("create a shallow clone of that depth")),
		OPT_STRING(0, "shallow-since", &option_since, N_("time"),
			   N_("create a shallow clone since a specific time")),
		OPT_STRING_LIST(0, "shallow-exclude", &option_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_BOOL(0, "single-branch", &option_single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_BOOL(0, "tags", &option_ట్యాగులు,
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
		OPT_చివర()
	};
	నిర్మాణం strvec alternates = STRVEC_INIT;

	స్థిరరాశి అక్షరం * స్థిరరాశి builtin_clone_usage[] = {
		N_("git clone [<options>] [--] <repo> [<dir>]"),
		ఖాళీ
	};

	filter_options.allow_auto_filter = 1;

	packet_trace_తటస్థం("clone");

	repo_config(the_repository, git_clone_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_clone_options,
			     builtin_clone_usage, 0);

	ఒకవేళ (argc > 2)
		usage_సందేశం_opt(_("Too many arguments."),
			builtin_clone_usage, builtin_clone_options);

	ఒకవేళ (argc == 0)
		usage_సందేశం_opt(_("You must specify a repository to clone."),
			builtin_clone_usage, builtin_clone_options);

	ఒకవేళ (option_లోతు || option_since || option_not.nr)
		deepen = 1;
	ఒకవేళ (option_single_branch == -1)
		option_single_branch = deepen ? 1 : 0;

	ఒకవేళ (ref_storage_format_uri) {
		ref_storage_format = ref_storage_format_by_uri(ref_storage_format_uri, ఖాళీ);
		ఒకవేళ (ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format_uri);
	}

	ఒకవేళ (option_mirror) {
		option_bare = 1;
		option_ట్యాగులు = 0;
	}

	ఒకవేళ (option_bare) {
		ఒకవేళ (real_git_dir)
			die(_("options '%s' and '%s' cannot be used together"), "--bare", "--separate-git-dir");
		option_no_checkout = 1;
	}

	ఒకవేళ (bundle_uri && deepen)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--bundle-uri",
		    "--depth/--shallow-since/--shallow-exclude");

	repo_name = argv[0];

	మార్గం = get_repo_మార్గం(repo_name, &is_bundle);
	ఒకవేళ (మార్గం) {
		FREE_AND_NULL(మార్గం);
		repo = repo_గమ్యం_free = absolute_pathdup(repo_name);
	} లేకపోతే ఒకవేళ (strchr(repo_name, ':')) {
		repo = repo_name;
		display_repo = transport_anonymize_url(repo);
	} లేకపోతే
		die(_("repository '%s' does not exist"), repo_name);

	/* no need to be strict, transport_set_option() will validate it again */
	ఒకవేళ (option_లోతు && atoi(option_లోతు) < 1)
		die(_("depth %s is not a positive number"), option_లోతు);

	ఒకవేళ (argc == 2)
		dir = xstrdup(argv[1]);
	లేకపోతే
		dir = git_url_basename(repo_name, is_bundle, option_bare);
	strip_dir_trailing_slashes(dir);

	dest_exists = మార్గం_exists(dir);
	ఒకవేళ (dest_exists && !is_ఖాళీలు_dir(dir))
		die(_("destination path '%s' already exists and is not "
			"an empty directory."), dir);

	ఒకవేళ (real_git_dir) {
		real_dest_exists = మార్గం_exists(real_git_dir);
		ఒకవేళ (real_dest_exists && !is_ఖాళీలు_dir(real_git_dir))
			die(_("repository path '%s' already exists and is not "
				"an empty directory."), real_git_dir);
	}


	strbuf_addf(&reflog_సందేశం, "clone: from %s",
		    display_repo ? display_repo : repo);
	విడుదల(display_repo);

	ఒకవేళ (option_bare)
		work_వృక్షం = ఖాళీ;
	లేకపోతే {
		work_వృక్షం = getenv("GIT_WORK_TREE");
		ఒకవేళ (work_వృక్షం && మార్గం_exists(work_వృక్షం))
			die(_("working tree '%s' already exists."), work_వృక్షం);
	}

	ఒకవేళ (option_bare || work_వృక్షం)
		git_dir = xstrdup(dir);
	లేకపోతే {
		work_వృక్షం = dir;
		git_dir = mkpathdup("%s/.git", dir);
	}

	atexit(remove_junk);
	sigchain_నెట్టు_common(remove_junk_on_signal);

	ఒకవేళ (!option_bare) {
		ఒకవేళ (సురక్షిత_create_leading_directories_const(the_repository, work_వృక్షం) < 0)
			die_errno(_("could not create leading directories of '%s'"),
				  work_వృక్షం);
		ఒకవేళ (dest_exists)
			junk_work_వృక్షం_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		లేకపోతే ఒకవేళ (mkdir(work_వృక్షం, 0777))
			die_errno(_("could not create work tree dir '%s'"),
				  work_వృక్షం);
		junk_work_వృక్షం = work_వృక్షం;
	}

	ఒకవేళ (real_git_dir) {
		ఒకవేళ (real_dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = real_git_dir;
	} లేకపోతే {
		ఒకవేళ (dest_exists)
			junk_git_dir_flags |= REMOVE_DIR_KEEP_TOPLEVEL;
		junk_git_dir = git_dir;
	}
	ఒకవేళ (సురక్షిత_create_leading_directories_const(the_repository, git_dir) < 0)
		die(_("could not create leading directories of '%s'"), git_dir);

	ఒకవేళ (0 <= option_verbosity) {
		ఒకవేళ (option_bare)
			fprintf(stderr, _("Cloning into bare repository '%s'...\n"), dir);
		లేకపోతే
			fprintf(stderr, _("Cloning into '%s'...\n"), dir);
	}

	ఒకవేళ (option_recurse_submodules.nr > 0) {
		నిర్మాణం string_list_అంశం *అంశం;
		నిర్మాణం strbuf sb = STRBUF_INIT;
		పూర్ణాంకం val;

		/* remove duplicates */
		string_list_క్రమీకరించు_u(&option_recurse_submodules, 0);

		/*
		 * NEEDSWORK: In a multi-working-tree world, this needs to be
		 * set in the per-worktree config.
		 */
		for_each_string_list_అంశం(అంశం, &option_recurse_submodules) {
			strbuf_addf(&sb, "submodule.active=%s",
				    అంశం->string);
			string_list_append(&option_config, sb.buf);
			strbuf_reset(&sb);
		}

		ఒకవేళ (!repo_config_get_bool(the_repository, "submodule.stickyRecursiveClone", &val) &&
		    val)
			string_list_append(&option_config, "submodule.recurse=true");

		ఒకవేళ (option_required_reference.nr &&
		    option_optional_reference.nr)
			die(_("clone --recursive is not compatible with "
			      "both --reference and --reference-if-able"));
		లేకపోతే ఒకవేళ (option_required_reference.nr) {
			string_list_append(&option_config,
				"submodule.alternateLocation=superproject");
			string_list_append(&option_config,
				"submodule.alternateErrorStrategy=die");
		} లేకపోతే ఒకవేళ (option_optional_reference.nr) {
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
	create_repository(the_repository, git_dir, real_git_dir, work_వృక్షం,
			  option_template, GIT_HASH_UNKNOWN, ref_storage_format_uri,
			  do_not_override_repo_unix_permissions, ఖాళీ);

	ఒకవేళ (real_git_dir) {
		విడుదల((అక్షరం *)git_dir);
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
	initialize_repository_సంస్కరణ(the_repository, GIT_HASH_UNKNOWN,
				      the_repository->ref_storage_format, 1);

	refs_create_refdir_stubs(the_repository, git_dir, ఖాళీ);

	/*
	 * additional config can be injected with -c, make sure it's included
	 * after init_db, which clears the entire config environment.
	 */
	write_config(&option_config);

	/*
	 * re-read config after init_db and write_config to pick up any config
	 * injected by --template and --config, respectively.
	 */
	repo_config(the_repository, git_clone_config, ఖాళీ);

	/*
	 * If option_reject_shallow is specified from CLI option,
	 * ignore config_reject_shallow from git_clone_config.
	 */
	ఒకవేళ (config_reject_shallow != -1)
		reject_shallow = config_reject_shallow;
	ఒకవేళ (option_reject_shallow != -1)
		reject_shallow = option_reject_shallow;

	/*
	 * If option_filter_submodules is specified from CLI option,
	 * ignore config_filter_submodules from git_clone_config.
	 */
	ఒకవేళ (config_filter_submodules != -1)
		filter_submodules = config_filter_submodules;
	ఒకవేళ (option_filter_submodules != -1)
		filter_submodules = option_filter_submodules;

	/*
	 * Exit if the user seems to be doing something silly with submodule
	 * filter flags (but not with filter configs, as those should be
	 * set-and-forget).
	 */
	ఒకవేళ (option_filter_submodules > 0 && !filter_options.choice)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--filter");
	ఒకవేళ (option_filter_submodules > 0 && !option_recurse_submodules.nr)
		die(_("the option '%s' requires '%s'"),
		    "--also-filter-submodules", "--recurse-submodules");

	/*
	 * apply the remote name provided by --origin only after this second
	 * call to git_config, to ensure it overrides all config-based values.
	 */
	ఒకవేళ (option_origin) {
		విడుదల(remote_name);
		remote_name = xstrdup(option_origin);
	}

	ఒకవేళ (!remote_name)
		remote_name = xstrdup("origin");

	ఒకవేళ (!valid_remote_name(remote_name))
		die(_("'%s' is not a valid remote name"), remote_name);

	ఒకవేళ (option_bare) {
		ఒకవేళ (option_mirror)
			src_ref_ఉపసర్గ = "refs/";
		strbuf_addstr(&branch_శిఖరం, src_ref_ఉపసర్గ);

		repo_config_సమితి(the_repository, "core.bare", "true");
	} లేకపోతే ఒకవేళ (!option_rev) {
		strbuf_addf(&branch_శిఖరం, "refs/remotes/%s/", remote_name);
	}

	strbuf_addf(&కీలకం, "remote.%s.url", remote_name);
	repo_config_సమితి(the_repository, కీలకం.buf, repo);
	strbuf_reset(&కీలకం);

	ఒకవేళ (!option_ట్యాగులు) {
		strbuf_addf(&కీలకం, "remote.%s.tagOpt", remote_name);
		repo_config_సమితి(the_repository, కీలకం.buf, "--no-tags");
		strbuf_reset(&కీలకం);
	}

	remote = remote_get_early(remote_name);

	ఒకవేళ (!option_rev)
		refspec_appendf(&remote->fetch, "+%s*:%s*", src_ref_ఉపసర్గ,
				branch_శిఖరం.buf);

	మార్గం = get_repo_మార్గం(remote->url.v[0], &is_bundle);
	is_local = option_local != 0 && మార్గం && !is_bundle;
	ఒకవేళ (is_local) {
		ఒకవేళ (option_లోతు)
			warning(_("--depth is ignored in local clones; use file:// instead."));
		ఒకవేళ (option_since)
			warning(_("--shallow-since is ignored in local clones; use file:// instead."));
		ఒకవేళ (option_not.nr)
			warning(_("--shallow-exclude is ignored in local clones; use file:// instead."));
		ఒకవేళ (filter_options.choice)
			warning(_("--filter is ignored in local clones; use file:// instead."));
		ఒకవేళ (!access(mkpath("%s/shallow", మార్గం), F_OK)) {
			ఒకవేళ (reject_shallow)
				die(_("source repository is shallow, reject to clone."));
			ఒకవేళ (option_local > 0)
				warning(_("source repository is shallow, ignoring --local"));
			is_local = 0;
		}
	}
	ఒకవేళ (option_local > 0 && !is_local)
		warning(_("--local is ignored"));

	collect_alternates(&alternates, is_local ? మార్గం : ఖాళీ);
	create_object_database(the_repository, &alternates);

	transport = transport_get(remote, మార్గం ? మార్గం : remote->url.v[0]);
	transport_సమితి_verbosity(transport, option_verbosity, option_పురోగతి);
	transport->family = family;
	transport->cloning = 1;

	ఒకవేళ (is_bundle) {
		నిర్మాణం bundle_శీర్షిక శీర్షిక = BUNDLE_శీర్షిక_INIT;
		పూర్ణాంకం fd = read_bundle_శీర్షిక(మార్గం, &శీర్షిక);
		పూర్ణాంకం has_filter = శీర్షిక.filter.choice != LOFC_DISABLED;

		ఒకవేళ (fd > 0)
			close(fd);
		bundle_శీర్షిక_release(&శీర్షిక);
		ఒకవేళ (has_filter)
			die(_("cannot clone from filtered bundle"));
	}

	transport_సమితి_option(transport, TRANS_OPT_KEEP, "yes");

	die_for_incompatible_opt2(!!option_rev, "--revision",
				  !!option_branch, "--branch");
	die_for_incompatible_opt2(!!option_rev, "--revision",
				  option_mirror, "--mirror");

	ఒకవేళ (reject_shallow)
		transport_సమితి_option(transport, TRANS_OPT_REJECT_SHALLOW, "1");
	ఒకవేళ (option_లోతు)
		transport_సమితి_option(transport, TRANS_OPT_లోతు,
				     option_లోతు);
	ఒకవేళ (option_since)
		transport_సమితి_option(transport, TRANS_OPT_DEEPEN_SINCE,
				     option_since);
	ఒకవేళ (option_not.nr)
		transport_సమితి_option(transport, TRANS_OPT_DEEPEN_NOT,
				     (స్థిరరాశి అక్షరం *)&option_not);
	ఒకవేళ (option_single_branch) {
		transport_సమితి_option(transport, TRANS_OPT_FOLLOWTAGS, "1");

		ఒకవేళ (option_branch)
			opts.కోరుతుంది_తల = 0;
	}

	ఒకవేళ (option_upload_pack)
		transport_సమితి_option(transport, TRANS_OPT_UPLOADPACK,
				     option_upload_pack);

	ఒకవేళ (server_options.nr)
		transport->server_options = &server_options;

	ఒకవేళ (filter_options.choice) {
		స్థిరరాశి అక్షరం *spec =
			expand_list_objects_filter_spec(&filter_options);
		transport_సమితి_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER,
				     spec);
		transport_సమితి_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}

	ఒకవేళ (transport->smart_options && !deepen && !filter_options.choice)
		transport->smart_options->check_self_contained_and_connected = 1;

	ఒకవేళ (option_rev) {
		option_ట్యాగులు = 0;
		option_single_branch = 0;
		opts.కోరుతుంది_తల = 0;
		opts.detach = 1;

		refspec_append(&remote->fetch, option_rev);
	}

	ఒకవేళ (option_ట్యాగులు || option_branch)
		/*
		 * Add tags refspec when user asked for tags (implicitly) or
		 * specified --branch, whose argument might be a tag.
		 */
		refspec_append(&remote->fetch, ట్యాగ్_REFSPEC);

	refspec_ref_prefixes(&remote->fetch,
			     &transport_ls_refs_options.ref_prefixes);
	ఒకవేళ (option_branch)
		expand_ref_ఉపసర్గ(&transport_ls_refs_options.ref_prefixes,
				  option_branch);

	/*
	 * As part of transport_get_remote_refs() the server tells us the hash
	 * algorithm, which we require to initialize the repo. But calling that
	 * function without any ref prefix, will cause the server to announce
	 * all known refs. If the argument passed to --revision was a hex oid,
	 * ref_prefixes will be empty so we fall back to asking about HEAD to
	 * reduce traffic from the server.
	 */
	ఒకవేళ (opts.కోరుతుంది_తల || transport_ls_refs_options.ref_prefixes.nr == 0)
		strvec_నెట్టు(&transport_ls_refs_options.ref_prefixes, "HEAD");

	refs = transport_get_remote_refs(transport, &transport_ls_refs_options);

	/*
	 * Now that we know what algorithm the remote side is using, let's set
	 * ours to the same thing.
	 */
	hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
	initialize_repository_సంస్కరణ(the_repository, hash_algo, the_repository->ref_storage_format, 1);
	repo_సమితి_hash_algo(the_repository, hash_algo);
	create_reference_database(the_repository, ఖాళీ, 1);
	startup_info->have_repository = 1;

	/*
	 * Before fetching from the remote, download and install bundle
	 * data from the --bundle-uri option.
	 */
	ఒకవేళ (bundle_uri) {
		నిర్మాణం remote_state *state;
		పూర్ణాంకం has_heuristic = 0;

		/*
		 * We need to save the remote state as our remote's lifetime is
		 * tied to it.
		 */
		state = the_repository->remote_state;
		the_repository->remote_state = ఖాళీ;
		repo_clear(the_repository);

		/* At this point, we need the_repository to match the cloned repo. */
		ఒకవేళ (repo_init(the_repository, git_dir, work_వృక్షం))
			warning(_("failed to initialize the repo, skipping bundle URI"));
		లేకపోతే ఒకవేళ (fetch_bundle_uri(the_repository, bundle_uri, &has_heuristic))
			warning(_("failed to fetch objects from bundle URI '%s'"),
				bundle_uri);
		లేకపోతే ఒకవేళ (has_heuristic)
			repo_config_సమితి_gently(the_repository, "fetch.bundleuri", bundle_uri);

		remote_state_clear(the_repository->remote_state);
		విడుదల(the_repository->remote_state);
		the_repository->remote_state = state;
	} లేకపోతే {
		/*
		* Populate transport->got_remote_bundle_uri and
		* transport->bundle_uri. We might get nothing.
		*/
		transport_get_remote_bundle_uri(transport);

		ఒకవేళ (transport->bundles &&
		    hashmap_get_కొలత(&transport->bundles->bundles)) {
			నిర్మాణం remote_state *state;

			/*
			 * We need to save the remote state as our remote's
			 * lifetime is tied to it.
			 */
			state = the_repository->remote_state;
			the_repository->remote_state = ఖాళీ;
			repo_clear(the_repository);

			/* At this point, we need the_repository to match the cloned repo. */
			ఒకవేళ (repo_init(the_repository, git_dir, work_వృక్షం))
				warning(_("failed to initialize the repo, skipping bundle URI"));
			లేకపోతే ఒకవేళ (fetch_bundle_list(the_repository,
						   transport->bundles))
				warning(_("failed to fetch advertised bundles"));

			remote_state_clear(the_repository->remote_state);
			విడుదల(the_repository->remote_state);
			the_repository->remote_state = state;
		} లేకపోతే {
			clear_bundle_list(transport->bundles);
			FREE_AND_NULL(transport->bundles);
		}
	}

	ఒకవేళ (refs)
		mapped_refs = wanted_peer_refs(&opts, refs, &remote->fetch);

	ఒకవేళ (mapped_refs) {
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
		కొరకు (ref = refs; ref; ref = ref->తదుపరి)
			ఒకవేళ (is_null_oid(&ref->old_oid)) {
				complete_refs_before_fetch = 0;
				ఆపు;
			}

		ఒకవేళ (!is_local && !complete_refs_before_fetch) {
			ఒకవేళ (transport_fetch_refs(transport, mapped_refs))
				die(_("remote transport reported error"));
		}
	}

	remote_తల = కనుగొను_ref_by_name(refs, "HEAD");
	remote_తల_points_at = guess_remote_తల(remote_తల, mapped_refs,
						  REMOTE_GUESS_తల_QUIET);

	ఒకవేళ (option_branch) {
		our_తల_points_at = కనుగొను_remote_branch(mapped_refs, option_branch);
		ఒకవేళ (!our_తల_points_at)
			die(_("Remote branch %s not found in upstream %s"),
			    option_branch, remote_name);
	} లేకపోతే ఒకవేళ (option_rev) {
		our_తల_points_at = mapped_refs;
		ఒకవేళ (!our_తల_points_at)
			die(_("Remote revision %s not found in upstream %s"),
			    option_rev, remote_name);
	} లేకపోతే ఒకవేళ (remote_తల_points_at) {
		our_తల_points_at = remote_తల_points_at;
	} లేకపోతే ఒకవేళ (remote_తల) {
		our_తల_points_at = ఖాళీ;
	} లేకపోతే {
		అక్షరం *గమ్యం_free = ఖాళీ;
		స్థిరరాశి అక్షరం *branch;

		ఒకవేళ (!mapped_refs) {
			warning(_("You appear to have cloned an empty repository."));
			option_no_checkout = 1;
		}

		ఒకవేళ (transport_ls_refs_options.unborn_తల_లక్ష్యం &&
		    skip_ఉపసర్గ(transport_ls_refs_options.unborn_తల_లక్ష్యం,
				"refs/heads/", &branch)) {
			unborn_తల  = xstrdup(transport_ls_refs_options.unborn_తల_లక్ష్యం);
		} లేకపోతే {
			branch = గమ్యం_free = repo_default_branch_name(the_repository, 0);
			unborn_తల = xstrfmt("refs/heads/%s", branch);
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
		our_తల_points_at = కనుగొను_remote_branch(mapped_refs, branch);

		విడుదల(గమ్యం_free);
	}

	ఒకవేళ (!option_rev)
		write_refspec_config(src_ref_ఉపసర్గ, our_తల_points_at,
				     remote_తల_points_at, &branch_శిఖరం);

	ఒకవేళ (filter_options.choice)
		partial_clone_రిజిస్టరు(remote_name, &filter_options);

	ఒకవేళ (is_local)
		clone_local(మార్గం, git_dir);
	లేకపోతే ఒకవేళ (mapped_refs && complete_refs_before_fetch) {
		ఒకవేళ (transport_fetch_refs(transport, mapped_refs))
			die(_("remote transport reported error"));
	}

	update_remote_refs(refs, mapped_refs, remote_తల_points_at,
			   branch_శిఖరం.buf, reflog_సందేశం.buf, transport,
			   !is_local);

	update_తల(&opts, our_తల_points_at, remote_తల, unborn_తల, reflog_సందేశం.buf);

	/*
	 * We want to show progress for recursive submodule clones iff
	 * we did so for the main clone. But only the transport knows
	 * the final decision for this flag, so we need to rescue the value
	 * before we free the transport.
	 */
	submodule_పురోగతి = transport->పురోగతి;

	transport_unlock_pack(transport, 0);
	transport_disconnect(transport);

	ఒకవేళ (option_dissociate) {
		odb_close(the_repository->objects);
		dissociate_from_references();
	}

	ఒకవేళ (option_sparse_checkout && git_sparse_checkout_init(dir))
		తిరిగి 1;

	junk_బహుళకం = JUNK_LEAVE_REPO;
	err = checkout(submodule_పురోగతి,
		       &filter_options,
		       filter_submodules,
		       ref_storage_format);

	list_objects_filter_release(&filter_options);

	string_list_clear(&option_not, 0);
	string_list_clear(&option_config, 0);
	string_list_clear(&server_options, 0);
	strvec_clear(&alternates);

	విడుదల(remote_name);
	strbuf_release(&reflog_సందేశం);
	strbuf_release(&branch_శిఖరం);
	strbuf_release(&buf);
	strbuf_release(&కీలకం);
	free_refs(mapped_refs);
	free_refs(remote_తల_points_at);
	విడుదల(unborn_తల);
	విడుదల(dir);
	విడుదల(మార్గం);
	విడుదల(repo_గమ్యం_free);
	junk_బహుళకం = JUNK_LEAVE_ALL;

	transport_ls_refs_options_release(&transport_ls_refs_options);
	తిరిగి err;
}
