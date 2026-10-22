/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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
ساكن صحيح allow_جمع_کن;
ساكن صحيح allow_remove;
ساكن صحيح allow_replace;
ساكن صحيح info_only;
ساكن صحيح force_remove;
ساكن صحيح verbose;
ساكن صحيح mark_valid_only;
ساكن صحيح mark_skip_worktree_only;
ساكن صحيح mark_fsmonitor_only;
ساكن صحيح ignore_skip_worktree_entries;
#تعریف_کن MARK_FLAG 1
#تعریف_کن UNMARK_FLAG 2
ساكن بنية strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
تعداد uc_نما {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (چاپ_کن, 1, 2)))
ساكن فراغ report(ثابت حرف *fmt, ...)
{
	va_list vp;

	إذا (!verbose)
		أرجع;

	va_شروع(vp, fmt);
	vprintf(fmt, vp);
	حرف_چاپ_کن('\n');
	va_انتها(vp);
}

ساكن فراغ remove_test_directory(فراغ)
{
	إذا (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

ساكن ثابت حرف *get_mtime_مسیر(ثابت حرف *مسیر)
{
	ساكن بنية strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, مسیر);
	أرجع sb.buf;
}

ساكن فراغ xmkdir(ثابت حرف *مسیر)
{
	مسیر = get_mtime_مسیر(مسیر);
	إذا (mkdir(مسیر, 0700))
		die_errno(_("failed to create directory %s"), مسیر);
}

ساكن صحيح xstat_mtime_dir(بنية stat *st)
{
	إذا (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	أرجع 0;
}

ساكن صحيح create_file(ثابت حرف *مسیر)
{
	صحيح fd;
	مسیر = get_mtime_مسیر(مسیر);
	fd = xopen(مسیر, O_CREAT | O_RDWR, 0644);
	أرجع fd;
}

ساكن فراغ xunlink(ثابت حرف *مسیر)
{
	مسیر = get_mtime_مسیر(مسیر);
	إذا (unlink(مسیر))
		die_errno(_("failed to delete file %s"), مسیر);
}

ساكن فراغ xrmdir(ثابت حرف *مسیر)
{
	مسیر = get_mtime_مسیر(مسیر);
	إذا (rmdir(مسیر))
		die_errno(_("failed to delete directory %s"), مسیر);
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
	بنية stat_داده پایه;
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
	fill_stat_داده(&پایه, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	إذا (!تطابق_stat_داده(&پایه, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		اذهب انجامشده;
	}
	fill_stat_داده(&پایه, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	إذا (!تطابق_stat_داده(&پایه, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		اذهب انجامشده;
	}
	fill_stat_داده(&پایه, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	إذا (تطابق_stat_داده(&پایه, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		اذهب انجامشده;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	إذا (تطابق_stat_داده(&پایه, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		اذهب انجامشده;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	إذا (!تطابق_stat_داده(&پایه, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		اذهب انجامشده;
	}
	fill_stat_داده(&پایه, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	إذا (!تطابق_stat_داده(&پایه, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		اذهب انجامشده;
	}

	إذا (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

انجامشده:
	strbuf_release(&mtime_dir);
	أرجع ret;
}

ساكن صحيح mark_ce_flags(ثابت حرف *مسیر, صحيح flag, صحيح mark)
{
	صحيح namelen = طول_رشته(مسیر);
	صحيح pos = شاخص_name_pos(the_repository->شاخص, مسیر, namelen);
	إذا (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->شاخص, the_repository->شاخص->cache[pos]);
		إذا (mark)
			the_repository->شاخص->cache[pos]->ce_flags |= flag;
		وإلا
			the_repository->شاخص->cache[pos]->ce_flags &= ~flag;
		the_repository->شاخص->cache[pos]->ce_flags |= CE_UPDATE_درون_پایه;
		cache_درخت_invalidate_مسیر(the_repository->شاخص, مسیر);
		the_repository->شاخص->cache_تغییریافته |= CE_مدخل_تغییریافته;
		أرجع 0;
	}
	أرجع -1;
}

ساكن صحيح remove_one_مسیر(ثابت حرف *مسیر)
{
	إذا (!allow_remove)
		أرجع error("%s: does not exist and --remove not passed", مسیر);
	إذا (remove_file_from_شاخص(the_repository->شاخص, مسیر))
		أرجع error("%s: cannot remove from the index", مسیر);
	أرجع 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
ساكن صحيح process_lstat_error(ثابت حرف *مسیر, صحيح err)
{
	إذا (is_missing_file_error(err))
		أرجع remove_one_مسیر(مسیر);
	أرجع error("lstat(\"%s\"): %s", مسیر, strerror(err));
}

ساكن صحيح جمع_کن_one_مسیر(ثابت بنية cache_مدخل *old, ثابت حرف *مسیر, صحيح len, بنية stat *st)
{
	صحيح option;
	بنية cache_مدخل *ce;

	/* Was the old index entry already up-to-date? */
	إذا (old && !ce_stage(old) && !ie_تطابق_stat(the_repository->شاخص, old, st, 0))
		أرجع 0;

	ce = make_خالی_cache_مدخل(the_repository->شاخص, len);
	کپی_حافظه(ce->name, مسیر, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->شاخص, ce, st);
	ce->ce_نما = ce_نما_from_stat(the_repository, old, st->st_نما);

	إذا (شاخص_مسیر(the_repository->شاخص, &ce->oid, مسیر, st,
		       info_only ? 0 : شاخص_WRITE_OBJECT)) {
		discard_cache_مدخل(ce);
		أرجع -1;
	}
	option = allow_جمع_کن ? جمع_کن_CACHE_OK_مقصد_جمع_کن : 0;
	option |= allow_replace ? جمع_کن_CACHE_OK_مقصد_REPLACE : 0;
	إذا (جمع_کن_شاخص_مدخل(the_repository->شاخص, ce, option)) {
		discard_cache_مدخل(ce);
		أرجع error("%s: cannot add to the index - missing --add option?", مسیر);
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
ساكن صحيح process_directory(ثابت حرف *مسیر, صحيح len, بنية stat *st)
{
	بنية object_id oid;
	صحيح pos = شاخص_name_pos(the_repository->شاخص, مسیر, len);

	/* Exact match: file or existing gitlink */
	إذا (pos >= 0) {
		ثابت بنية cache_مدخل *ce = the_repository->شاخص->cache[pos];
		إذا (S_ISGITLINK(ce->ce_نما)) {

			/* Do nothing to the index if there is no HEAD! */
			إذا (repo_resolve_gitlink_ref(the_repository, مسیر,
						     "HEAD", &oid) < 0)
				أرجع 0;

			أرجع جمع_کن_one_مسیر(ce, مسیر, len, st);
		}
		/* Should this be an unconditional error? */
		أرجع remove_one_مسیر(مسیر);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	طالما (pos < the_repository->شاخص->cache_nr) {
		ثابت بنية cache_مدخل *ce = the_repository->شاخص->cache[pos++];

		إذا (مقایسه_رشته(ce->name, مسیر, len))
			اكسر;
		إذا (ce->name[len] > '/')
			اكسر;
		إذا (ce->name[len] < '/')
			استمر;

		/* Subdirectory match - error out */
		أرجع error("%s: is a directory - add individual files instead", مسیر);
	}

	/* No match - should we add it as a gitlink? */
	إذا (!repo_resolve_gitlink_ref(the_repository, مسیر, "HEAD", &oid))
		أرجع جمع_کن_one_مسیر(عدم, مسیر, len, st);

	/* Error out. */
	أرجع error("%s: is a directory - add files inside instead", مسیر);
}

ساكن صحيح process_مسیر(ثابت حرف *مسیر, بنية stat *st, صحيح stat_errno)
{
	صحيح pos, len;
	ثابت بنية cache_مدخل *ce;

	len = طول_رشته(مسیر);
	إذا (has_symlink_leading_مسیر(مسیر, len))
		أرجع error("'%s' is beyond a symbolic link", مسیر);

	pos = شاخص_name_pos(the_repository->شاخص, مسیر, len);
	ce = pos < 0 ? عدم : the_repository->شاخص->cache[pos];
	إذا (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		إذا (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_شاخص(the_repository->شاخص, مسیر))
			أرجع error("%s: cannot remove from the index", مسیر);
		أرجع 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	إذا (stat_errno)
		أرجع process_lstat_error(مسیر, stat_errno);

	إذا (S_ISDIR(st->st_نما))
		أرجع process_directory(مسیر, len, st);

	أرجع جمع_کن_one_مسیر(ce, مسیر, len, st);
}

ساكن صحيح جمع_کن_cacheinfo(بیعلامت صحيح نما, ثابت بنية object_id *oid,
			 ثابت حرف *مسیر, صحيح stage)
{
	صحيح len, option;
	بنية cache_مدخل *ce;

	إذا (!verify_مسیر(مسیر, نما))
		أرجع error("Invalid path '%s'", مسیر);

	len = طول_رشته(مسیر);
	ce = make_خالی_cache_مدخل(the_repository->شاخص, len);

	oidcpy(&ce->oid, oid);
	کپی_حافظه(ce->name, مسیر, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_نما = create_ce_نما(نما);
	إذا (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_جمع_کن ? جمع_کن_CACHE_OK_مقصد_جمع_کن : 0;
	option |= allow_replace ? جمع_کن_CACHE_OK_مقصد_REPLACE : 0;
	إذا (جمع_کن_شاخص_مدخل(the_repository->شاخص, ce, option))
		أرجع error("%s: cannot add to the index - missing --add option?",
			     مسیر);
	report("add '%s'", مسیر);
	أرجع 0;
}

ساكن فراغ chmod_مسیر(حرف flip, ثابت حرف *مسیر)
{
	صحيح pos;
	بنية cache_مدخل *ce;

	pos = شاخص_name_pos(the_repository->شاخص, مسیر, طول_رشته(مسیر));
	إذا (pos < 0)
		اذهب fail;
	ce = the_repository->شاخص->cache[pos];
	إذا (chmod_شاخص_مدخل(the_repository->شاخص, ce, flip) < 0)
		اذهب fail;

	report("chmod %cx '%s'", flip, مسیر);
	أرجع;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, مسیر);
}

ساكن فراغ update_one(ثابت حرف *مسیر)
{
	صحيح stat_errno = 0;
	بنية stat st;

	إذا (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_نما = 0;
	وإلا إذا (lstat(مسیر, &st) < 0) {
		st.st_نما = 0;
		stat_errno = errno;
	} /* else stat is valid */

	إذا (!verify_مسیر(مسیر, st.st_نما)) {
		fprintf(stderr, "Ignoring path %s\n", مسیر);
		أرجع;
	}
	إذا (mark_valid_only) {
		إذا (mark_ce_flags(مسیر, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", مسیر);
		أرجع;
	}
	إذا (mark_skip_worktree_only) {
		إذا (mark_ce_flags(مسیر, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", مسیر);
		أرجع;
	}
	إذا (mark_fsmonitor_only) {
		إذا (mark_ce_flags(مسیر, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", مسیر);
		أرجع;
	}

	إذا (force_remove) {
		إذا (remove_file_from_شاخص(the_repository->شاخص, مسیر))
			die("git update-index: unable to remove %s", مسیر);
		report("remove '%s'", مسیر);
		أرجع;
	}
	إذا (process_مسیر(مسیر, &st, stat_errno))
		die("Unable to process path %s", مسیر);
	report("add '%s'", مسیر);
}

ساكن فراغ read_شاخص_info(صحيح nul_جمله_سطر)
{
	ثابت صحيح hexsz = the_hash_algo->hexsz;
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_جمله_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	طالما (getline_fn(&buf, stdin) != EOF) {
		حرف *ptr, *tab;
		حرف *مسیر_name;
		بنية object_id oid;
		بیعلامت صحيح نما;
		بیعلامت دراز ul;
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
		    || errno || (بیعلامت صحيح) ul != ul)
			اذهب bad_سطر;
		نما = ul;

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

		مسیر_name = ptr;
		إذا (!nul_جمله_سطر && مسیر_name[0] == '"') {
			strbuf_reset(&uq);
			إذا (unquote_c_style(&uq, مسیر_name, عدم)) {
				die("git update-index: bad quoting of path name");
			}
			مسیر_name = uq.buf;
		}

		إذا (!verify_مسیر(مسیر_name, نما)) {
			fprintf(stderr, "Ignoring path %s\n", مسیر_name);
			استمر;
		}

		إذا (!نما) {
			/* mode == 0 means there is no such path -- remove */
			إذا (remove_file_from_شاخص(the_repository->شاخص, مسیر_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		وإلا {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			إذا (جمع_کن_cacheinfo(نما, &oid, مسیر_name, stage))
				die("git update-index: unable to update %s",
				    مسیر_name);
		}
		استمر;

	bad_سطر:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

ساكن ثابت حرف * ثابت update_شاخص_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	عدم
};

ساكن بنية cache_مدخل *read_one_ent(ثابت حرف *which,
					بنية object_id *ent, ثابت حرف *مسیر,
					صحيح namelen, صحيح stage)
{
	بیعلامت کوتاه نما;
	بنية object_id oid;
	بنية cache_مدخل *ce;

	إذا (get_درخت_مدخل(the_repository, ent, مسیر, &oid, &نما)) {
		إذا (which)
			error("%s: not in %s branch.", مسیر, which);
		أرجع عدم;
	}
	إذا (!the_repository->شاخص->sparse_شاخص && نما == S_IFDIR) {
		إذا (which)
			error("%s: not a blob in %s branch.", مسیر, which);
		أرجع عدم;
	}
	ce = make_خالی_cache_مدخل(the_repository->شاخص, namelen);

	oidcpy(&ce->oid, &oid);
	کپی_حافظه(ce->name, مسیر, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_نما = create_ce_نما(نما);
	أرجع ce;
}

ساكن صحيح unresolve_one(ثابت حرف *مسیر)
{
	بنية string_list_قلم *قلم;
	صحيح res = 0;

	إذا (!the_repository->شاخص->resolve_undo)
		أرجع res;
	قلم = string_list_lookup(the_repository->شاخص->resolve_undo, مسیر);
	إذا (!قلم)
		أرجع res; /* no resolve-undo record for the path */
	res = unmerge_شاخص_مدخل(the_repository->شاخص, مسیر, قلم->util, 0);
	FREE_AND_NULL(قلم->util);
	أرجع res;
}

ساكن صحيح do_unresolve(صحيح ac, ثابت حرف **av,
			ثابت حرف *پیشوند, صحيح پیشوند_درازی)
{
	صحيح i;
	صحيح err = 0;

	لكل (i = 1; i < ac; i++) {
		ثابت حرف *arg = av[i];
		حرف *p = پیشوند_مسیر(the_repository, پیشوند, پیشوند_درازی, arg);
		err |= unresolve_one(p);
		حرر(p);
	}
	أرجع err;
}

ساكن صحيح do_reupdate(ثابت حرف **paths,
		       ثابت حرف *پیشوند)
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
		       پیشوند, paths);

	إذا (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &سر_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_سر = 0;
 redo:
	لكل (pos = 0; pos < the_repository->شاخص->cache_nr; pos++) {
		ثابت بنية cache_مدخل *ce = the_repository->شاخص->cache[pos];
		بنية cache_مدخل *old = عدم;
		صحيح save_nr;
		حرف *مسیر;

		إذا (ce_stage(ce) || !ce_مسیر_تطابق(the_repository->شاخص, ce, &pathspec, عدم))
			استمر;
		إذا (has_سر)
			old = read_one_ent(عدم, &سر_oid,
					   ce->name, ce_namelen(ce), 0);
		إذا (old && ce->ce_نما == old->ce_نما &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_مدخل(old);
			استمر; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		إذا (S_ISSPARSEDIR(ce->ce_نما)) {
			ensure_پر_شاخص(the_repository->شاخص);
			اذهب redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->شاخص->cache_nr;
		مسیر = xstrdup(ce->name);
		update_one(مسیر);
		حرر(مسیر);
		discard_cache_مدخل(old);
		إذا (save_nr != the_repository->شاخص->cache_nr)
			اذهب redo;
	}
	clear_pathspec(&pathspec);
	أرجع 0;
}

بنية refresh_params {
	بیعلامت صحيح flags;
	صحيح *has_errors;
};

ساكن صحيح refresh(بنية refresh_params *o, بیعلامت صحيح flag)
{
	راهاندازی_work_درخت(the_repository);
	repo_read_شاخص(the_repository);
	*o->has_errors |= refresh_شاخص(the_repository->شاخص, o->flags | flag, عدم,
					عدم, عدم);
	إذا (has_racy_timestamp(the_repository->شاخص)) {
		/*
		 * Even if nothing else has changed, updating the file
		 * increases the chance that racy timestamps become
		 * non-racy, helping future run-time performance.
		 * We do that even in case of "errors" returned by
		 * refresh_index() as these are no actual errors.
		 * cmd_status() does the same.
		 */
		the_repository->شاخص->cache_تغییریافته |= SOMETHING_تغییریافته;
	}
	أرجع 0;
}

ساكن صحيح refresh_callback(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	أرجع refresh(opt->مقدار, 0);
}

ساكن صحيح really_refresh_callback(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	أرجع refresh(opt->مقدار, REFRESH_REALLY);
}

ساكن صحيح chmod_callback(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	حرف *flip = opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	إذا ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		أرجع error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	أرجع 0;
}

ساكن صحيح resolve_undo_clear_callback(ثابت بنية option *opt استفادهنشده,
				ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_شاخص(the_repository->شاخص);
	أرجع 0;
}

ساكن صحيح parse_new_style_cacheinfo(ثابت حرف *arg,
				     بیعلامت صحيح *نما,
				     بنية object_id *oid,
				     ثابت حرف **مسیر)
{
	بیعلامت دراز ul;
	حرف *endp;
	ثابت حرف *p;

	إذا (!arg)
		أرجع -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	إذا (errno || endp == arg || *endp != ',' || (بیعلامت صحيح) ul != ul)
		أرجع -1; /* not a new-style cacheinfo */
	*نما = ul;
	endp++;
	إذا (parse_oid_hex(endp, oid, &p) || *p != ',')
		أرجع -1;
	*مسیر = p + 1;
	أرجع 0;
}

ساكن تعداد parse_opt_نتیجه cacheinfo_callback(
	بنية parse_opt_ctx_t *ctx, ثابت بنية option *opt استفادهنشده,
	ثابت حرف *arg, صحيح unset)
{
	بنية object_id oid;
	بیعلامت صحيح نما;
	ثابت حرف *مسیر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	إذا (!parse_new_style_cacheinfo(ctx->argv[1], &نما, &oid, &مسیر)) {
		إذا (جمع_کن_cacheinfo(نما, &oid, مسیر, 0))
			die("git update-index: --cacheinfo cannot add %s", مسیر);
		ctx->argv++;
		ctx->argc--;
		أرجع 0;
	}
	إذا (ctx->argc <= 3)
		أرجع error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	إذا (strtoul_ui(*++ctx->argv, 8, &نما) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    جمع_کن_cacheinfo(نما, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	أرجع 0;
}

ساكن تعداد parse_opt_نتیجه stdin_cacheinfo_callback(
	بنية parse_opt_ctx_t *ctx, ثابت بنية option *opt,
	ثابت حرف *arg, صحيح unset)
{
	صحيح *nul_جمله_سطر = opt->مقدار;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	إذا (ctx->argc != 1)
		أرجع error("option '%s' must be the last argument", opt->long_name);
	allow_جمع_کن = allow_replace = allow_remove = 1;
	read_شاخص_info(*nul_جمله_سطر);
	أرجع 0;
}

ساكن تعداد parse_opt_نتیجه stdin_callback(
	بنية parse_opt_ctx_t *ctx, ثابت بنية option *opt,
	ثابت حرف *arg, صحيح unset)
{
	صحيح *read_from_stdin = opt->مقدار;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	إذا (ctx->argc != 1)
		أرجع error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	أرجع 0;
}

ساكن تعداد parse_opt_نتیجه unresolve_callback(
	بنية parse_opt_ctx_t *ctx, ثابت بنية option *opt,
	ثابت حرف *arg, صحيح unset)
{
	صحيح *has_errors = opt->مقدار;
	ثابت حرف *پیشوند = the_repository->پیشوند;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				پیشوند, پیشوند ? طول_رشته(پیشوند) : 0);
	إذا (*has_errors)
		the_repository->شاخص->cache_تغییریافته = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	أرجع 0;
}

ساكن تعداد parse_opt_نتیجه reupdate_callback(
	بنية parse_opt_ctx_t *ctx, ثابت بنية option *opt,
	ثابت حرف *arg, صحيح unset)
{
	صحيح *has_errors = opt->مقدار;
	ثابت حرف *پیشوند = the_repository->پیشوند;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	راهاندازی_work_درخت(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, پیشوند);
	إذا (*has_errors)
		the_repository->شاخص->cache_تغییریافته = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	أرجع 0;
}

صحيح cmd_update_شاخص(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *پیشوند,
		     بنية repository *repo استفادهنشده)
{
	صحيح newfd, entries, has_errors = 0, nul_جمله_سطر = 0;
	تعداد uc_نما untracked_cache = UC_UNSPECIFIED;
	صحيح read_from_stdin = 0;
	صحيح پیشوند_درازی = پیشوند ? طول_رشته(پیشوند) : 0;
	صحيح preferred_شاخص_format = 0;
	حرف مجموعه_executable_بیت = 0;
	بنية refresh_params refresh_آرگومانها = {0, &has_errors};
	صحيح قفل_error = 0;
	صحيح تقسیم_شاخص = -1;
	صحيح force_write = 0;
	صحيح fsmonitor = -1;
	بنية قفل_file قفل_file = قفل_INIT;
	بنية parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	صحيح parseopt_state = PARSE_OPT_UNKNOWN;
	بنية repository *r = the_repository;
	بنية odb_transaction *transaction;
	بنية option options[] = {
		OPT_بیت('q', عدم, &refresh_آرگومانها.flags,
			N_("continue refresh even when index needs update"),
			REFRESH_QUIET),
		OPT_بیت(0, "ignore-submodules", &refresh_آرگومانها.flags,
			N_("refresh: ignore submodules"),
			REFRESH_IGNORE_SUBMODULES),
		OPT_مجموعه_INT(0, "add", &allow_جمع_کن,
			N_("do not ignore new files"), 1),
		OPT_مجموعه_INT(0, "replace", &allow_replace,
			N_("let files replace directories and vice-versa"), 1),
		OPT_مجموعه_INT(0, "remove", &allow_remove,
			N_("notice files missing from worktree"), 1),
		OPT_بیت(0, "unmerged", &refresh_آرگومانها.flags,
			N_("refresh even if index contains unmerged entries"),
			REFRESH_UNMERGED),
		OPT_CALLBACK_F(0, "refresh", &refresh_آرگومانها, عدم,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_آرگومانها, عدم,
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
		OPT_CALLBACK_F(0, "chmod", &مجموعه_executable_بیت, "(+|-)x",
			N_("override the executable bit of the listed files"),
			PARSE_OPT_NONEG,
			chmod_callback),
		{
			.type = OPTION_مجموعه_INT,
			.long_name = "assume-unchanged",
			.مقدار = &mark_valid_only,
			.precision = حجم(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_مجموعه_INT,
			.long_name = "no-assume-unchanged",
			.مقدار = &mark_valid_only,
			.precision = حجم(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_مجموعه_INT,
			.long_name = "skip-worktree",
			.مقدار = &mark_skip_worktree_only,
			.precision = حجم(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_مجموعه_INT,
			.long_name = "no-skip-worktree",
			.مقدار = &mark_skip_worktree_only,
			.precision = حجم(mark_skip_worktree_only),
			.help = N_("clear skip-worktree bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_BOOL(0, "ignore-skip-worktree-entries", &ignore_skip_worktree_entries,
			 N_("do not touch index-only entries")),
		OPT_مجموعه_INT(0, "info-only", &info_only,
			N_("add to index only; do not add content to object database"), 1),
		OPT_مجموعه_INT(0, "force-remove", &force_remove,
			N_("remove named paths even if present in worktree"), 1),
		OPT_BOOL('z', عدم, &nul_جمله_سطر,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.مقدار = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.مقدار = &nul_جمله_سطر,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.مقدار = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.مقدار = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_بیت(0, "ignore-missing", &refresh_آرگومانها.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_مجموعه_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", عدم, عدم,
			N_("(for porcelains) forget saved unresolved conflicts"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			resolve_undo_clear_callback),
		OPT_INTEGER(0, "index-version", &preferred_شاخص_format,
			N_("write index in this format")),
		OPT_مجموعه_INT(0, "show-index-version", &preferred_شاخص_format,
			    N_("report on-disk index format version"), -1),
		OPT_BOOL(0, "split-index", &تقسیم_شاخص,
			N_("enable or disable split index")),
		OPT_BOOL(0, "untracked-cache", &untracked_cache,
			N_("enable/disable untracked cache")),
		OPT_مجموعه_INT(0, "test-untracked-cache", &untracked_cache,
			    N_("test if the filesystem supports untracked cache"), UC_TEST),
		OPT_مجموعه_INT(0, "force-untracked-cache", &untracked_cache,
			    N_("enable untracked cache without testing the filesystem"), UC_FORCE),
		OPT_مجموعه_INT(0, "force-write-index", &force_write,
			N_("write out the index even if is not flagged as changed"), 1),
		OPT_BOOL(0, "fsmonitor", &fsmonitor,
			N_("enable or disable file system monitor")),
		{
			.type = OPTION_مجموعه_INT,
			.long_name = "fsmonitor-valid",
			.مقدار = &mark_fsmonitor_only,
			.precision = حجم(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_مجموعه_INT,
			.long_name = "no-fsmonitor-valid",
			.مقدار = &mark_fsmonitor_only,
			.precision = حجم(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_انتها()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_شاخص_usage, options);

	repo_config(the_repository, git_default_config, عدم);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_پر_شاخص = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_شاخص(the_repository, &قفل_file, 0);
	إذا (newfd < 0)
		قفل_error = errno;

	entries = repo_read_شاخص(the_repository);
	إذا (entries < 0)
		die("cache corrupted");

	the_repository->شاخص->updated_skipworktree = 1;

	/*
	 * Custom copy of parse_options() because we want to handle
	 * filename arguments as they come.
	 */
	parse_options_شروع(&ctx, argc, argv, پیشوند,
			    options, PARSE_OPT_STOP_AT_NON_OPTION);

	/*
	 * Allow the object layer to optimize adding multiple objects in
	 * a batch.
	 */
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	طالما (ctx.argc) {
		إذا (parseopt_state != PARSE_OPT_انجامشده)
			parseopt_state = parse_options_گام(&ctx, options,
							    update_شاخص_usage);
		إذا (!ctx.argc)
			اكسر;
		بدل (parseopt_state) {
		حالة PARSE_OPT_HELP:
			خروج(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
			خروج(129);
		حالة PARSE_OPT_COMPLETE:
			خروج(0);
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_انجامشده:
		{
			ثابت حرف *مسیر = ctx.argv[0];
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

			راهاندازی_work_درخت(the_repository);
			p = پیشوند_مسیر(the_repository, پیشوند, پیشوند_درازی, مسیر);
			update_one(p);
			إذا (مجموعه_executable_بیت)
				chmod_مسیر(مجموعه_executable_بیت, p);
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
			usage_with_options(update_شاخص_usage, options);
		}
	}
	argc = parse_options_انتها(&ctx);

	getline_fn = nul_جمله_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	إذا (preferred_شاخص_format) {
		إذا (preferred_شاخص_format < 0) {
			چاپ_کن(_("%d\n"), the_repository->شاخص->نسخه);
		} وإلا إذا (preferred_شاخص_format < شاخص_FORMAT_LB ||
			   شاخص_FORMAT_UB < preferred_شاخص_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_شاخص_format,
			    شاخص_FORMAT_LB, شاخص_FORMAT_UB);
		} وإلا {
			إذا (the_repository->شاخص->نسخه != preferred_شاخص_format)
				the_repository->شاخص->cache_تغییریافته |= SOMETHING_تغییریافته;
			report(_("index-version: was %d, set to %d"),
			       the_repository->شاخص->نسخه, preferred_شاخص_format);
			the_repository->شاخص->نسخه = preferred_شاخص_format;
		}
	}

	إذا (read_from_stdin) {
		بنية strbuf buf = STRBUF_INIT;
		بنية strbuf unquoted = STRBUF_INIT;

		راهاندازی_work_درخت(the_repository);
		طالما (getline_fn(&buf, stdin) != EOF) {
			حرف *p;
			إذا (!nul_جمله_سطر && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				إذا (unquote_c_style(&unquoted, buf.buf, عدم))
					die("line is badly quoted");
				strbuf_جابجا(&buf, &unquoted);
			}
			p = پیشوند_مسیر(the_repository, پیشوند, پیشوند_درازی, buf.buf);
			update_one(p);
			إذا (مجموعه_executable_بیت)
				chmod_مسیر(مجموعه_executable_بیت, p);
			حرر(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	إذا (تقسیم_شاخص > 0) {
		إذا (repo_config_get_تقسیم_شاخص(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		إذا (the_repository->شاخص->تقسیم_شاخص)
			the_repository->شاخص->cache_تغییریافته |= تقسیم_شاخص_ORDERED;
		وإلا
			جمع_کن_تقسیم_شاخص(the_repository->شاخص);
	} وإلا إذا (!تقسیم_شاخص) {
		إذا (repo_config_get_تقسیم_شاخص(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_تقسیم_شاخص(the_repository->شاخص);
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
		remove_untracked_cache(the_repository->شاخص);
		report(_("Untracked cache disabled"));
		اكسر;
	حالة UC_TEST:
		راهاندازی_work_درخت(the_repository);
		أرجع !test_if_untracked_cache_is_supported();
	حالة UC_ENABLE:
	حالة UC_FORCE:
		إذا (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		جمع_کن_untracked_cache(the_repository->شاخص);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_درخت(the_repository));
		اكسر;
	افتراضي:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	إذا (fsmonitor > 0) {
		تعداد fsmonitor_نما fsm_نما = fsm_settings_get_نما(r);
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
			    fsm_settings_get_incompatible_پیام(r, reason));

		إذا (fsm_نما == FSMONITOR_نما_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		جمع_کن_fsmonitor(the_repository->شاخص);
		report(_("fsmonitor enabled"));
	} وإلا إذا (!fsmonitor) {
		تعداد fsmonitor_نما fsm_نما = fsm_settings_get_نما(r);
		إذا (fsm_نما > FSMONITOR_نما_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->شاخص);
		report(_("fsmonitor disabled"));
	}

	إذا (the_repository->شاخص->cache_تغییریافته || force_write) {
		إذا (newfd < 0) {
			إذا (refresh_آرگومانها.flags & REFRESH_QUIET)
				خروج(128);
			unable_مقصد_قفل_die(repo_get_شاخص_file(the_repository), قفل_error);
		}
		إذا (write_locked_شاخص(the_repository->شاخص, &قفل_file, COMMIT_قفل))
			die("Unable to write new index file");
	}

	rollback_قفل_file(&قفل_file);

	أرجع has_errors ? 1 : 0;
}
