/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "lockfile.h"
#ضمن "quote.h"
#ضمن "cache-tree.h"
#ضمن "tree-walk.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "odb/transaction.h"
#ضمن "refs.h"
#ضمن "resolve-undo.h"
#ضمن "parse-options.h"
#ضمن "pathspec.h"
#ضمن "dir.h"
#ضمن "read-cache.h"
#ضمن "setup.h"
#ضمن "sparse-index.h"
#ضمن "split-index.h"
#ضمن "symlinks.h"
#ضمن "fsmonitor.h"
#ضمن "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
ساكن صحيح allow_جوڑو;
ساكن صحيح allow_remove;
ساكن صحيح allow_replace;
ساكن صحيح info_only;
ساكن صحيح force_remove;
ساكن صحيح verbose;
ساكن صحيح mark_valid_only;
ساكن صحيح mark_skip_worktree_only;
ساكن صحيح mark_fsmonitor_only;
ساكن صحيح ignore_skip_worktree_entries;
#تعریف_کرو MARK_FLAG 1
#تعریف_کرو UNMARK_FLAG 2
ساكن بنية strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
تعداد uc_منوال {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (لکھو, 1, 2)))
ساكن فراغ report(ثابت حرف *fmt, ...)
{
	va_list vp;

	إذا (!verbose)
		أرجع;

	va_شروع(vp, fmt);
	vprintf(fmt, vp);
	حرف_لکھو('\n');
	va_سرا(vp);
}

ساكن فراغ remove_test_directory(فراغ)
{
	إذا (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

ساكن ثابت حرف *get_mtime_رستہ(ثابت حرف *رستہ)
{
	ساكن بنية strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, رستہ);
	أرجع sb.buf;
}

ساكن فراغ xmkdir(ثابت حرف *رستہ)
{
	رستہ = get_mtime_رستہ(رستہ);
	إذا (mkdir(رستہ, 0700))
		die_errno(_("failed to create directory %s"), رستہ);
}

ساكن صحيح xstat_mtime_dir(بنية stat *st)
{
	إذا (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	أرجع 0;
}

ساكن صحيح create_file(ثابت حرف *رستہ)
{
	صحيح fd;
	رستہ = get_mtime_رستہ(رستہ);
	fd = xopen(رستہ, O_CREAT | O_RDWR, 0644);
	أرجع fd;
}

ساكن فراغ xunlink(ثابت حرف *رستہ)
{
	رستہ = get_mtime_رستہ(رستہ);
	إذا (unlink(رستہ))
		die_errno(_("failed to delete file %s"), رستہ);
}

ساكن فراغ xrmdir(ثابت حرف *رستہ)
{
	رستہ = get_mtime_رستہ(رستہ);
	إذا (rmdir(رستہ))
		die_errno(_("failed to delete directory %s"), رستہ);
}

ساكن فراغ avoid_racy(فراغ)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

ساكن صحيح test_if_untracked_cache_is_supported(فراغ)
{
	بنية stat st;
	بنية stat_انکڑے ادھار;
	صحيح fd, ret = 0;
	حرف *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	إذا (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	حرر(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_انکڑے(&ادھار, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	إذا (!میل_stat_انکڑے(&ادھار, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		اذهب پورا;
	}
	fill_stat_انکڑے(&ادھار, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	إذا (!میل_stat_انکڑے(&ادھار, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		اذهب پورا;
	}
	fill_stat_انکڑے(&ادھار, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	إذا (میل_stat_انکڑے(&ادھار, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		اذهب پورا;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	إذا (میل_stat_انکڑے(&ادھار, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		اذهب پورا;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	إذا (!میل_stat_انکڑے(&ادھار, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		اذهب پورا;
	}
	fill_stat_انکڑے(&ادھار, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	إذا (!میل_stat_انکڑے(&ادھار, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		اذهب پورا;
	}

	إذا (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

پورا:
	strbuf_release(&mtime_dir);
	أرجع ret;
}

ساكن صحيح mark_ce_flags(ثابت حرف *رستہ, صحيح flag, صحيح mark)
{
	صحيح namelen = لڑی_لمبائی(رستہ);
	صحيح pos = اشاریہ_name_pos(the_repository->اشاریہ, رستہ, namelen);
	إذا (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->اشاریہ, the_repository->اشاریہ->cache[pos]);
		إذا (mark)
			the_repository->اشاریہ->cache[pos]->ce_flags |= flag;
		وإلا
			the_repository->اشاریہ->cache[pos]->ce_flags &= ~flag;
		the_repository->اشاریہ->cache[pos]->ce_flags |= CE_UPDATE_وچ_ادھار;
		cache_درخت_invalidate_رستہ(the_repository->اشاریہ, رستہ);
		the_repository->اشاریہ->cache_بدلیا |= CE_اندراج_بدلیا;
		أرجع 0;
	}
	أرجع -1;
}

ساكن صحيح remove_one_رستہ(ثابت حرف *رستہ)
{
	إذا (!allow_remove)
		أرجع error("%s: does not exist and --remove not passed", رستہ);
	إذا (remove_file_from_اشاریہ(the_repository->اشاریہ, رستہ))
		أرجع error("%s: cannot remove from the index", رستہ);
	أرجع 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
ساكن صحيح process_lstat_error(ثابت حرف *رستہ, صحيح err)
{
	إذا (is_missing_file_error(err))
		أرجع remove_one_رستہ(رستہ);
	أرجع error("lstat(\"%s\"): %s", رستہ, strerror(err));
}

ساكن صحيح جوڑو_one_رستہ(ثابت بنية cache_اندراج *old, ثابت حرف *رستہ, صحيح len, بنية stat *st)
{
	صحيح option;
	بنية cache_اندراج *ce;

	/* Was the old index entry already up-to-date? */
	إذا (old && !ce_stage(old) && !ie_میل_stat(the_repository->اشاریہ, old, st, 0))
		أرجع 0;

	ce = make_خالی_تھاواں_cache_اندراج(the_repository->اشاریہ, len);
	حافظہ_نقل(ce->name, رستہ, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->اشاریہ, ce, st);
	ce->ce_منوال = ce_منوال_from_stat(the_repository, old, st->st_منوال);

	إذا (اشاریہ_رستہ(the_repository->اشاریہ, &ce->oid, رستہ, st,
		       info_only ? 0 : اشاریہ_WRITE_OBJECT)) {
		discard_cache_اندراج(ce);
		أرجع -1;
	}
	option = allow_جوڑو ? جوڑو_CACHE_OK_منزل_جوڑو : 0;
	option |= allow_replace ? جوڑو_CACHE_OK_منزل_REPLACE : 0;
	إذا (جوڑو_اشاریہ_اندراج(the_repository->اشاریہ, ce, option)) {
		discard_cache_اندراج(ce);
		أرجع error("%s: cannot add to the index - missing --add option?", رستہ);
	}
	أرجع 0;
}

/*
 * Handle a path that was a directory. Four cases:
 *
 *  - it's already a gitlink in the index, and we keep it that
 *    way, and update it if we can (if we cannot find the HEAD,
 *    we're going to keep it unchanged in the index!)
 *
 *  - it's a *file* in the index, in which case it should be
 *    removed as a file if removal is allowed, since it doesn't
 *    exist as such any more. If removal isn't allowed, it's
 *    an error.
 *
 *    (NOTE! This is old and arguably fairly strange behaviour.
 *    We might want to make this an error unconditionally, and
 *    use "--force-remove" if you actually want to force removal).
 *
 *  - it used to exist as a subdirectory (ie multiple files with
 *    this particular prefix) in the index, in which case it's wrong
 *    to try to update it as a directory.
 *
 *  - it doesn't exist at all in the index, but it is a valid
 *    git directory, and it should be *added* as a gitlink.
 */
ساكن صحيح process_directory(ثابت حرف *رستہ, صحيح len, بنية stat *st)
{
	بنية object_id oid;
	صحيح pos = اشاریہ_name_pos(the_repository->اشاریہ, رستہ, len);

	/* Exact match: file or existing gitlink */
	إذا (pos >= 0) {
		ثابت بنية cache_اندراج *ce = the_repository->اشاریہ->cache[pos];
		إذا (S_ISGITLINK(ce->ce_منوال)) {

			/* Do nothing to the index if there is no HEAD! */
			إذا (repo_resolve_gitlink_ref(the_repository, رستہ,
						     "HEAD", &oid) < 0)
				أرجع 0;

			أرجع جوڑو_one_رستہ(ce, رستہ, len, st);
		}
		/* Should this be an unconditional error? */
		أرجع remove_one_رستہ(رستہ);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	طالما (pos < the_repository->اشاریہ->cache_nr) {
		ثابت بنية cache_اندراج *ce = the_repository->اشاریہ->cache[pos++];

		إذا (لڑی_مقابلہ(ce->name, رستہ, len))
			اكسر;
		إذا (ce->name[len] > '/')
			اكسر;
		إذا (ce->name[len] < '/')
			استمر;

		/* Subdirectory match - error out */
		أرجع error("%s: is a directory - add individual files instead", رستہ);
	}

	/* No match - should we add it as a gitlink? */
	إذا (!repo_resolve_gitlink_ref(the_repository, رستہ, "HEAD", &oid))
		أرجع جوڑو_one_رستہ(عدم, رستہ, len, st);

	/* Error out. */
	أرجع error("%s: is a directory - add files inside instead", رستہ);
}

ساكن صحيح process_رستہ(ثابت حرف *رستہ, بنية stat *st, صحيح stat_errno)
{
	صحيح pos, len;
	ثابت بنية cache_اندراج *ce;

	len = لڑی_لمبائی(رستہ);
	إذا (has_symlink_leading_رستہ(رستہ, len))
		أرجع error("'%s' is beyond a symbolic link", رستہ);

	pos = اشاریہ_name_pos(the_repository->اشاریہ, رستہ, len);
	ce = pos < 0 ? عدم : the_repository->اشاریہ->cache[pos];
	إذا (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		إذا (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_اشاریہ(the_repository->اشاریہ, رستہ))
			أرجع error("%s: cannot remove from the index", رستہ);
		أرجع 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	إذا (stat_errno)
		أرجع process_lstat_error(رستہ, stat_errno);

	إذا (S_ISDIR(st->st_منوال))
		أرجع process_directory(رستہ, len, st);

	أرجع جوڑو_one_رستہ(ce, رستہ, len, st);
}

ساكن صحيح جوڑو_cacheinfo(بنانشانی صحيح منوال, ثابت بنية object_id *oid,
			 ثابت حرف *رستہ, صحيح stage)
{
	صحيح len, option;
	بنية cache_اندراج *ce;

	إذا (!verify_رستہ(رستہ, منوال))
		أرجع error("Invalid path '%s'", رستہ);

	len = لڑی_لمبائی(رستہ);
	ce = make_خالی_تھاواں_cache_اندراج(the_repository->اشاریہ, len);

	oidcpy(&ce->oid, oid);
	حافظہ_نقل(ce->name, رستہ, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_منوال = create_ce_منوال(منوال);
	إذا (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_جوڑو ? جوڑو_CACHE_OK_منزل_جوڑو : 0;
	option |= allow_replace ? جوڑو_CACHE_OK_منزل_REPLACE : 0;
	إذا (جوڑو_اشاریہ_اندراج(the_repository->اشاریہ, ce, option))
		أرجع error("%s: cannot add to the index - missing --add option?",
			     رستہ);
	report("add '%s'", رستہ);
	أرجع 0;
}

ساكن فراغ chmod_رستہ(حرف flip, ثابت حرف *رستہ)
{
	صحيح pos;
	بنية cache_اندراج *ce;

	pos = اشاریہ_name_pos(the_repository->اشاریہ, رستہ, لڑی_لمبائی(رستہ));
	إذا (pos < 0)
		اذهب fail;
	ce = the_repository->اشاریہ->cache[pos];
	إذا (chmod_اشاریہ_اندراج(the_repository->اشاریہ, ce, flip) < 0)
		اذهب fail;

	report("chmod %cx '%s'", flip, رستہ);
	أرجع;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, رستہ);
}

ساكن فراغ update_one(ثابت حرف *رستہ)
{
	صحيح stat_errno = 0;
	بنية stat st;

	إذا (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_منوال = 0;
	وإلا إذا (lstat(رستہ, &st) < 0) {
		st.st_منوال = 0;
		stat_errno = errno;
	} /* else stat is valid */

	إذا (!verify_رستہ(رستہ, st.st_منوال)) {
		fprintf(stderr, "Ignoring path %s\n", رستہ);
		أرجع;
	}
	إذا (mark_valid_only) {
		إذا (mark_ce_flags(رستہ, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", رستہ);
		أرجع;
	}
	إذا (mark_skip_worktree_only) {
		إذا (mark_ce_flags(رستہ, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", رستہ);
		أرجع;
	}
	إذا (mark_fsmonitor_only) {
		إذا (mark_ce_flags(رستہ, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", رستہ);
		أرجع;
	}

	إذا (force_remove) {
		إذا (remove_file_from_اشاریہ(the_repository->اشاریہ, رستہ))
			die("git update-index: unable to remove %s", رستہ);
		report("remove '%s'", رستہ);
		أرجع;
	}
	إذا (process_رستہ(رستہ, &st, stat_errno))
		die("Unable to process path %s", رستہ);
	report("add '%s'", رستہ);
}

ساكن فراغ read_اشاریہ_info(صحيح nul_رکن_سطر)
{
	ثابت صحيح hexsz = the_hash_algo->hexsz;
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_رکن_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	طالما (getline_fn(&buf, stdin) != EOF) {
		حرف *ptr, *tab;
		حرف *رستہ_name;
		بنية object_id oid;
		بنانشانی صحيح منوال;
		بنانشانی لما ul;
		صحيح stage;

		/* This reads lines formatted in one of three formats:
		 *
		 * (1) mode         SP sha1          TAB path
		 * The first format is what "git apply --index-info"
		 * reports, and used to reconstruct a partial tree
		 * that is used for phony merge base tree when falling
		 * back on 3-way merge.
		 *
		 * (2) mode SP type SP sha1          TAB path
		 * The second format is to stuff "git ls-tree" output
		 * into the index file.
		 *
		 * (3) mode         SP sha1 SP stage TAB path
		 * This format is to put higher order stages into the
		 * index file and matches "git ls-files --stage" output.
		 */
		errno = 0;
		ul = strtoul(buf.buf, &ptr, 8);
		إذا (ptr == buf.buf || *ptr != ' '
		    || errno || (بنانشانی صحيح) ul != ul)
			اذهب bad_سطر;
		منوال = ul;

		tab = strchr(ptr, '\t');
		إذا (!tab || tab - ptr < hexsz + 1)
			اذهب bad_سطر;

		إذا (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		وإلا {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		إذا (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			اذهب bad_سطر;

		رستہ_name = ptr;
		إذا (!nul_رکن_سطر && رستہ_name[0] == '"') {
			strbuf_reset(&uq);
			إذا (unquote_c_style(&uq, رستہ_name, عدم)) {
				die("git update-index: bad quoting of path name");
			}
			رستہ_name = uq.buf;
		}

		إذا (!verify_رستہ(رستہ_name, منوال)) {
			fprintf(stderr, "Ignoring path %s\n", رستہ_name);
			استمر;
		}

		إذا (!منوال) {
			/* mode == 0 means there is no such path -- remove */
			إذا (remove_file_from_اشاریہ(the_repository->اشاریہ, رستہ_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		وإلا {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			إذا (جوڑو_cacheinfo(منوال, &oid, رستہ_name, stage))
				die("git update-index: unable to update %s",
				    رستہ_name);
		}
		استمر;

	bad_سطر:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

ساكن ثابت حرف * ثابت update_اشاریہ_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	عدم
};

ساكن بنية cache_اندراج *read_one_ent(ثابت حرف *which,
					بنية object_id *ent, ثابت حرف *رستہ,
					صحيح namelen, صحيح stage)
{
	بنانشانی چھوٹا منوال;
	بنية object_id oid;
	بنية cache_اندراج *ce;

	إذا (get_درخت_اندراج(the_repository, ent, رستہ, &oid, &منوال)) {
		إذا (which)
			error("%s: not in %s branch.", رستہ, which);
		أرجع عدم;
	}
	إذا (!the_repository->اشاریہ->sparse_اشاریہ && منوال == S_IFDIR) {
		إذا (which)
			error("%s: not a blob in %s branch.", رستہ, which);
		أرجع عدم;
	}
	ce = make_خالی_تھاواں_cache_اندراج(the_repository->اشاریہ, namelen);

	oidcpy(&ce->oid, &oid);
	حافظہ_نقل(ce->name, رستہ, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_منوال = create_ce_منوال(منوال);
	أرجع ce;
}

ساكن صحيح unresolve_one(ثابت حرف *رستہ)
{
	بنية string_list_شے *شے;
	صحيح res = 0;

	إذا (!the_repository->اشاریہ->resolve_undo)
		أرجع res;
	شے = string_list_lookup(the_repository->اشاریہ->resolve_undo, رستہ);
	إذا (!شے)
		أرجع res; /* no resolve-undo record for the path */
	res = unmerge_اشاریہ_اندراج(the_repository->اشاریہ, رستہ, شے->util, 0);
	FREE_AND_NULL(شے->util);
	أرجع res;
}

ساكن صحيح do_unresolve(صحيح ac, ثابت حرف **av,
			ثابت حرف *اگیتر, صحيح اگیتر_لمائی)
{
	صحيح i;
	صحيح err = 0;

	لكل (i = 1; i < ac; i++) {
		ثابت حرف *arg = av[i];
		حرف *p = اگیتر_رستہ(the_repository, اگیتر, اگیتر_لمائی, arg);
		err |= unresolve_one(p);
		حرر(p);
	}
	أرجع err;
}

ساكن صحيح do_reupdate(ثابت حرف **paths,
		       ثابت حرف *اگیتر)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	صحيح pos;
	صحيح has_سر = 1;
	بنية pathspec pathspec;
	بنية object_id سر_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       اگیتر, paths);

	إذا (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &سر_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_سر = 0;
 redo:
	لكل (pos = 0; pos < the_repository->اشاریہ->cache_nr; pos++) {
		ثابت بنية cache_اندراج *ce = the_repository->اشاریہ->cache[pos];
		بنية cache_اندراج *old = عدم;
		صحيح save_nr;
		حرف *رستہ;

		إذا (ce_stage(ce) || !ce_رستہ_میل(the_repository->اشاریہ, ce, &pathspec, عدم))
			استمر;
		إذا (has_سر)
			old = read_one_ent(عدم, &سر_oid,
					   ce->name, ce_namelen(ce), 0);
		إذا (old && ce->ce_منوال == old->ce_منوال &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_اندراج(old);
			استمر; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		إذا (S_ISSPARSEDIR(ce->ce_منوال)) {
			ensure_بھریا_اشاریہ(the_repository->اشاریہ);
			اذهب redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->اشاریہ->cache_nr;
		رستہ = xstrdup(ce->name);
		update_one(رستہ);
		حرر(رستہ);
		discard_cache_اندراج(old);
		إذا (save_nr != the_repository->اشاریہ->cache_nr)
			اذهب redo;
	}
	clear_pathspec(&pathspec);
	أرجع 0;
}

بنية refresh_params {
	بنانشانی صحيح flags;
	صحيح *has_errors;
};

ساكن صحيح refresh(بنية refresh_params *o, بنانشانی صحيح flag)
{
	تیاری_work_درخت(the_repository);
	repo_read_اشاریہ(the_repository);
	*o->has_errors |= refresh_اشاریہ(the_repository->اشاریہ, o->flags | flag, عدم,
					عدم, عدم);
	إذا (has_racy_timestamp(the_repository->اشاریہ)) {
		/*
		 * Even if nothing else has changed, updating the file
		 * increases the chance that racy timestamps become
		 * non-racy, helping future run-time performance.
		 * We do that even in case of "errors" returned by
		 * refresh_index() as these are no actual errors.
		 * cmd_status() does the same.
		 */
		the_repository->اشاریہ->cache_بدلیا |= SOMETHING_بدلیا;
	}
	أرجع 0;
}

ساكن صحيح refresh_callback(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	أرجع refresh(opt->قدر, 0);
}

ساكن صحيح really_refresh_callback(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	أرجع refresh(opt->قدر, REFRESH_REALLY);
}

ساكن صحيح chmod_callback(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	حرف *flip = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	إذا ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		أرجع error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	أرجع 0;
}

ساكن صحيح resolve_undo_clear_callback(ثابت بنية option *opt بن_ورتیا,
				ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_اشاریہ(the_repository->اشاریہ);
	أرجع 0;
}

ساكن صحيح parse_new_style_cacheinfo(ثابت حرف *arg,
				     بنانشانی صحيح *منوال,
				     بنية object_id *oid,
				     ثابت حرف **رستہ)
{
	بنانشانی لما ul;
	حرف *endp;
	ثابت حرف *p;

	إذا (!arg)
		أرجع -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	إذا (errno || endp == arg || *endp != ',' || (بنانشانی صحيح) ul != ul)
		أرجع -1; /* not a new-style cacheinfo */
	*منوال = ul;
	endp++;
	إذا (parse_oid_hex(endp, oid, &p) || *p != ',')
		أرجع -1;
	*رستہ = p + 1;
	أرجع 0;
}

ساكن تعداد parse_opt_نتیجہ cacheinfo_callback(
	بنية parse_opt_ctx_t *ctx, ثابت بنية option *opt بن_ورتیا,
	ثابت حرف *arg, صحيح unset)
{
	بنية object_id oid;
	بنانشانی صحيح منوال;
	ثابت حرف *رستہ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	إذا (!parse_new_style_cacheinfo(ctx->argv[1], &منوال, &oid, &رستہ)) {
		إذا (جوڑو_cacheinfo(منوال, &oid, رستہ, 0))
			die("git update-index: --cacheinfo cannot add %s", رستہ);
		ctx->argv++;
		ctx->argc--;
		أرجع 0;
	}
	إذا (ctx->argc <= 3)
		أرجع error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	إذا (strtoul_ui(*++ctx->argv, 8, &منوال) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    جوڑو_cacheinfo(منوال, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	أرجع 0;
}

ساكن تعداد parse_opt_نتیجہ stdin_cacheinfo_callback(
	بنية parse_opt_ctx_t *ctx, ثابت بنية option *opt,
	ثابت حرف *arg, صحيح unset)
{
	صحيح *nul_رکن_سطر = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	إذا (ctx->argc != 1)
		أرجع error("option '%s' must be the last argument", opt->long_name);
	allow_جوڑو = allow_replace = allow_remove = 1;
	read_اشاریہ_info(*nul_رکن_سطر);
	أرجع 0;
}

ساكن تعداد parse_opt_نتیجہ stdin_callback(
	بنية parse_opt_ctx_t *ctx, ثابت بنية option *opt,
	ثابت حرف *arg, صحيح unset)
{
	صحيح *read_from_stdin = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	إذا (ctx->argc != 1)
		أرجع error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	أرجع 0;
}

ساكن تعداد parse_opt_نتیجہ unresolve_callback(
	بنية parse_opt_ctx_t *ctx, ثابت بنية option *opt,
	ثابت حرف *arg, صحيح unset)
{
	صحيح *has_errors = opt->قدر;
	ثابت حرف *اگیتر = the_repository->اگیتر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				اگیتر, اگیتر ? لڑی_لمبائی(اگیتر) : 0);
	إذا (*has_errors)
		the_repository->اشاریہ->cache_بدلیا = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	أرجع 0;
}

ساكن تعداد parse_opt_نتیجہ reupdate_callback(
	بنية parse_opt_ctx_t *ctx, ثابت بنية option *opt,
	ثابت حرف *arg, صحيح unset)
{
	صحيح *has_errors = opt->قدر;
	ثابت حرف *اگیتر = the_repository->اگیتر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	تیاری_work_درخت(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, اگیتر);
	إذا (*has_errors)
		the_repository->اشاریہ->cache_بدلیا = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	أرجع 0;
}

صحيح cmd_update_اشاریہ(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *اگیتر,
		     بنية repository *repo بن_ورتیا)
{
	صحيح newfd, entries, has_errors = 0, nul_رکن_سطر = 0;
	تعداد uc_منوال untracked_cache = UC_UNSPECIFIED;
	صحيح read_from_stdin = 0;
	صحيح اگیتر_لمائی = اگیتر ? لڑی_لمبائی(اگیتر) : 0;
	صحيح preferred_اشاریہ_format = 0;
	حرف سیٹ_executable_ثنائی_ہندسہ = 0;
	بنية refresh_params refresh_دلیلاں = {0, &has_errors};
	صحيح جندرا_error = 0;
	صحيح ونڈو_اشاریہ = -1;
	صحيح force_write = 0;
	صحيح fsmonitor = -1;
	بنية جندرا_file جندرا_file = جندرا_INIT;
	بنية parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	صحيح parseopt_state = PARSE_OPT_UNKNOWN;
	بنية repository *r = the_repository;
	بنية odb_transaction *transaction;
	بنية option options[] = {
		OPT_ثنائی_ہندسہ('q', عدم, &refresh_دلیلاں.flags,
			N_("continue refresh even when index needs update"),
			REFRESH_QUIET),
		OPT_ثنائی_ہندسہ(0, "ignore-submodules", &refresh_دلیلاں.flags,
			N_("refresh: ignore submodules"),
			REFRESH_IGNORE_SUBMODULES),
		OPT_سیٹ_INT(0, "add", &allow_جوڑو,
			N_("do not ignore new files"), 1),
		OPT_سیٹ_INT(0, "replace", &allow_replace,
			N_("let files replace directories and vice-versa"), 1),
		OPT_سیٹ_INT(0, "remove", &allow_remove,
			N_("notice files missing from worktree"), 1),
		OPT_ثنائی_ہندسہ(0, "unmerged", &refresh_دلیلاں.flags,
			N_("refresh even if index contains unmerged entries"),
			REFRESH_UNMERGED),
		OPT_CALLBACK_F(0, "refresh", &refresh_دلیلاں, عدم,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_دلیلاں, عدم,
			N_("like --refresh, but ignore assume-unchanged setting"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			really_refresh_callback),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "cacheinfo",
			.argh = N_("<mode>,<object>,<path>"),
			.help = N_("add the specified entry to the index"),
			.flags = PARSE_OPT_NOARG | /* disallow --cacheinfo=<mode> form */
				 PARSE_OPT_NONEG | PARSE_OPT_LITERAL_ARGHELP,
			.ll_callback = cacheinfo_callback,
		},
		OPT_CALLBACK_F(0, "chmod", &سیٹ_executable_ثنائی_ہندسہ, "(+|-)x",
			N_("override the executable bit of the listed files"),
			PARSE_OPT_NONEG,
			chmod_callback),
		{
			.type = OPTION_سیٹ_INT,
			.long_name = "assume-unchanged",
			.قدر = &mark_valid_only,
			.precision = حجم(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_سیٹ_INT,
			.long_name = "no-assume-unchanged",
			.قدر = &mark_valid_only,
			.precision = حجم(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_سیٹ_INT,
			.long_name = "skip-worktree",
			.قدر = &mark_skip_worktree_only,
			.precision = حجم(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_سیٹ_INT,
			.long_name = "no-skip-worktree",
			.قدر = &mark_skip_worktree_only,
			.precision = حجم(mark_skip_worktree_only),
			.help = N_("clear skip-worktree bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_BOOL(0, "ignore-skip-worktree-entries", &ignore_skip_worktree_entries,
			 N_("do not touch index-only entries")),
		OPT_سیٹ_INT(0, "info-only", &info_only,
			N_("add to index only; do not add content to object database"), 1),
		OPT_سیٹ_INT(0, "force-remove", &force_remove,
			N_("remove named paths even if present in worktree"), 1),
		OPT_BOOL('z', عدم, &nul_رکن_سطر,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.قدر = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.قدر = &nul_رکن_سطر,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.قدر = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.قدر = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_ثنائی_ہندسہ(0, "ignore-missing", &refresh_دلیلاں.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_سیٹ_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", عدم, عدم,
			N_("(for porcelains) forget saved unresolved conflicts"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			resolve_undo_clear_callback),
		OPT_INTEGER(0, "index-version", &preferred_اشاریہ_format,
			N_("write index in this format")),
		OPT_سیٹ_INT(0, "show-index-version", &preferred_اشاریہ_format,
			    N_("report on-disk index format version"), -1),
		OPT_BOOL(0, "split-index", &ونڈو_اشاریہ,
			N_("enable or disable split index")),
		OPT_BOOL(0, "untracked-cache", &untracked_cache,
			N_("enable/disable untracked cache")),
		OPT_سیٹ_INT(0, "test-untracked-cache", &untracked_cache,
			    N_("test if the filesystem supports untracked cache"), UC_TEST),
		OPT_سیٹ_INT(0, "force-untracked-cache", &untracked_cache,
			    N_("enable untracked cache without testing the filesystem"), UC_FORCE),
		OPT_سیٹ_INT(0, "force-write-index", &force_write,
			N_("write out the index even if is not flagged as changed"), 1),
		OPT_BOOL(0, "fsmonitor", &fsmonitor,
			N_("enable or disable file system monitor")),
		{
			.type = OPTION_سیٹ_INT,
			.long_name = "fsmonitor-valid",
			.قدر = &mark_fsmonitor_only,
			.precision = حجم(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_سیٹ_INT,
			.long_name = "no-fsmonitor-valid",
			.قدر = &mark_fsmonitor_only,
			.precision = حجم(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_سرا()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_اشاریہ_usage, options);

	repo_config(the_repository, git_default_config, عدم);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_بھریا_اشاریہ = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_اشاریہ(the_repository, &جندرا_file, 0);
	إذا (newfd < 0)
		جندرا_error = errno;

	entries = repo_read_اشاریہ(the_repository);
	إذا (entries < 0)
		die("cache corrupted");

	the_repository->اشاریہ->updated_skipworktree = 1;

	/*
	 * Custom copy of parse_options() because we want to handle
	 * filename arguments as they come.
	 */
	parse_options_شروع(&ctx, argc, argv, اگیتر,
			    options, PARSE_OPT_STOP_AT_NON_OPTION);

	/*
	 * Allow the object layer to optimize adding multiple objects in
	 * a batch.
	 */
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	طالما (ctx.argc) {
		إذا (parseopt_state != PARSE_OPT_پورا)
			parseopt_state = parse_options_قدم(&ctx, options,
							    update_اشاریہ_usage);
		إذا (!ctx.argc)
			اكسر;
		بدل (parseopt_state) {
		حالة PARSE_OPT_HELP:
			باہر_نکلو(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
			باہر_نکلو(129);
		حالة PARSE_OPT_COMPLETE:
			باہر_نکلو(0);
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_پورا:
		{
			ثابت حرف *رستہ = ctx.argv[0];
			حرف *p;

			/*
			 * It is possible, though unlikely, that a caller could
			 * use the verbose output to synchronize with addition
			 * of objects to the object database. The current
			 * implementation of ODB transactions leaves objects
			 * invisible while a transaction is active, so end the
			 * transaction here early before processing the next
			 * update. All further updates are performed outside of
			 * a transaction.
			 */
			إذا (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = عدم;
			}

			تیاری_work_درخت(the_repository);
			p = اگیتر_رستہ(the_repository, اگیتر, اگیتر_لمائی, رستہ);
			update_one(p);
			إذا (سیٹ_executable_ثنائی_ہندسہ)
				chmod_رستہ(سیٹ_executable_ثنائی_ہندسہ, p);
			حرر(p);
			ctx.argc--;
			ctx.argv++;
			اكسر;
		}
		حالة PARSE_OPT_UNKNOWN:
			إذا (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			وإلا
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_اشاریہ_usage, options);
		}
	}
	argc = parse_options_سرا(&ctx);

	getline_fn = nul_رکن_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	إذا (preferred_اشاریہ_format) {
		إذا (preferred_اشاریہ_format < 0) {
			لکھو(_("%d\n"), the_repository->اشاریہ->نسخہ);
		} وإلا إذا (preferred_اشاریہ_format < اشاریہ_FORMAT_LB ||
			   اشاریہ_FORMAT_UB < preferred_اشاریہ_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_اشاریہ_format,
			    اشاریہ_FORMAT_LB, اشاریہ_FORMAT_UB);
		} وإلا {
			إذا (the_repository->اشاریہ->نسخہ != preferred_اشاریہ_format)
				the_repository->اشاریہ->cache_بدلیا |= SOMETHING_بدلیا;
			report(_("index-version: was %d, set to %d"),
			       the_repository->اشاریہ->نسخہ, preferred_اشاریہ_format);
			the_repository->اشاریہ->نسخہ = preferred_اشاریہ_format;
		}
	}

	إذا (read_from_stdin) {
		بنية strbuf buf = STRBUF_INIT;
		بنية strbuf unquoted = STRBUF_INIT;

		تیاری_work_درخت(the_repository);
		طالما (getline_fn(&buf, stdin) != EOF) {
			حرف *p;
			إذا (!nul_رکن_سطر && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				إذا (unquote_c_style(&unquoted, buf.buf, عدم))
					die("line is badly quoted");
				strbuf_وٹاندرا(&buf, &unquoted);
			}
			p = اگیتر_رستہ(the_repository, اگیتر, اگیتر_لمائی, buf.buf);
			update_one(p);
			إذا (سیٹ_executable_ثنائی_ہندسہ)
				chmod_رستہ(سیٹ_executable_ثنائی_ہندسہ, p);
			حرر(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	إذا (ونڈو_اشاریہ > 0) {
		إذا (repo_config_get_ونڈو_اشاریہ(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		إذا (the_repository->اشاریہ->ونڈو_اشاریہ)
			the_repository->اشاریہ->cache_بدلیا |= ونڈو_اشاریہ_ORDERED;
		وإلا
			جوڑو_ونڈو_اشاریہ(the_repository->اشاریہ);
	} وإلا إذا (!ونڈو_اشاریہ) {
		إذا (repo_config_get_ونڈو_اشاریہ(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_ونڈو_اشاریہ(the_repository->اشاریہ);
	}

	prepare_repo_settings(r);
	بدل (untracked_cache) {
	حالة UC_UNSPECIFIED:
		اكسر;
	حالة UC_DISABLE:
		إذا (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->اشاریہ);
		report(_("Untracked cache disabled"));
		اكسر;
	حالة UC_TEST:
		تیاری_work_درخت(the_repository);
		أرجع !test_if_untracked_cache_is_supported();
	حالة UC_ENABLE:
	حالة UC_FORCE:
		إذا (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		جوڑو_untracked_cache(the_repository->اشاریہ);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_درخت(the_repository));
		اكسر;
	افتراضي:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	إذا (fsmonitor > 0) {
		تعداد fsmonitor_منوال fsm_منوال = fsm_settings_get_منوال(r);
		تعداد fsmonitor_reason reason = fsm_settings__get_reason(r);

		/*
		 * The user wants to turn on FSMonitor using the command
		 * line argument.  (We don't know (or care) whether that
		 * is the IPC or HOOK version.)
		 *
		 * Use one of the __get routines to force load the FSMonitor
		 * config settings into the repo-settings.  That will detect
		 * whether the file system is compatible so that we can stop
		 * here with a nice error message.
		 */
		إذا (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_پیغام(r, reason));

		إذا (fsm_منوال == FSMONITOR_منوال_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		جوڑو_fsmonitor(the_repository->اشاریہ);
		report(_("fsmonitor enabled"));
	} وإلا إذا (!fsmonitor) {
		تعداد fsmonitor_منوال fsm_منوال = fsm_settings_get_منوال(r);
		إذا (fsm_منوال > FSMONITOR_منوال_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->اشاریہ);
		report(_("fsmonitor disabled"));
	}

	إذا (the_repository->اشاریہ->cache_بدلیا || force_write) {
		إذا (newfd < 0) {
			إذا (refresh_دلیلاں.flags & REFRESH_QUIET)
				باہر_نکلو(128);
			unable_منزل_جندرا_die(repo_get_اشاریہ_file(the_repository), جندرا_error);
		}
		إذا (write_locked_اشاریہ(the_repository->اشاریہ, &جندرا_file, COMMIT_جندرا))
			die("Unable to write new index file");
	}

	rollback_جندرا_file(&جندرا_file);

	أرجع has_errors ? 1 : 0;
}
