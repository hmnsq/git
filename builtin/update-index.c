/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "lockfile.h"
#شامل "quote.h"
#شامل "cache-tree.h"
#شامل "tree-walk.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "odb/transaction.h"
#شامل "refs.h"
#شامل "resolve-undo.h"
#شامل "parse-options.h"
#شامل "pathspec.h"
#شامل "dir.h"
#شامل "read-cache.h"
#شامل "setup.h"
#شامل "sparse-index.h"
#شامل "split-index.h"
#شامل "symlinks.h"
#شامل "fsmonitor.h"
#شامل "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
ساکن صحیح allow_جوڑو;
ساکن صحیح allow_remove;
ساکن صحیح allow_replace;
ساکن صحیح info_only;
ساکن صحیح force_remove;
ساکن صحیح verbose;
ساکن صحیح mark_valid_only;
ساکن صحیح mark_skip_worktree_only;
ساکن صحیح mark_fsmonitor_only;
ساکن صحیح ignore_skip_worktree_entries;
#تعریف_کرو MARK_FLAG 1
#تعریف_کرو UNMARK_FLAG 2
ساکن ساخت strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
شمار uc_منوال {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (لکھو, 1, 2)))
ساکن خالی report(ثابت حرف *fmt, ...)
{
	va_list vp;

	اگر (!verbose)
		واپس;

	va_آغاز(vp, fmt);
	vprintf(fmt, vp);
	حرف_لکھو('\n');
	va_انتہا(vp);
}

ساکن خالی remove_test_directory(خالی)
{
	اگر (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

ساکن ثابت حرف *get_mtime_راستہ(ثابت حرف *راستہ)
{
	ساکن ساخت strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, راستہ);
	واپس sb.buf;
}

ساکن خالی xmkdir(ثابت حرف *راستہ)
{
	راستہ = get_mtime_راستہ(راستہ);
	اگر (mkdir(راستہ, 0700))
		die_errno(_("failed to create directory %s"), راستہ);
}

ساکن صحیح xstat_mtime_dir(ساخت stat *st)
{
	اگر (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	واپس 0;
}

ساکن صحیح create_file(ثابت حرف *راستہ)
{
	صحیح fd;
	راستہ = get_mtime_راستہ(راستہ);
	fd = xopen(راستہ, O_CREAT | O_RDWR, 0644);
	واپس fd;
}

ساکن خالی xunlink(ثابت حرف *راستہ)
{
	راستہ = get_mtime_راستہ(راستہ);
	اگر (unlink(راستہ))
		die_errno(_("failed to delete file %s"), راستہ);
}

ساکن خالی xrmdir(ثابت حرف *راستہ)
{
	راستہ = get_mtime_راستہ(راستہ);
	اگر (rmdir(راستہ))
		die_errno(_("failed to delete directory %s"), راستہ);
}

ساکن خالی avoid_racy(خالی)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

ساکن صحیح test_if_untracked_cache_is_supported(خالی)
{
	ساخت stat st;
	ساخت stat_ڈیٹا بنیاد;
	صحیح fd, ret = 0;
	حرف *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	اگر (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	fprintf(stderr, _("Testing mtime in '%s' "), cwd);
	آزاد(cwd);

	atexit(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_ڈیٹا(&بنیاد, &st);
	fputc('.', stderr);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	اگر (!مطابقت_stat_ڈیٹا(&بنیاد, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr,_("directory stat info does not "
				    "change after adding a new file"));
		جاؤ مکمل;
	}
	fill_stat_ڈیٹا(&بنیاد, &st);
	fputc('.', stderr);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	اگر (!مطابقت_stat_ڈیٹا(&بنیاد, &st)) {
		close(fd);
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not change "
				     "after adding a new directory"));
		جاؤ مکمل;
	}
	fill_stat_ڈیٹا(&بنیاد, &st);
	fputc('.', stderr);

	avoid_racy();
	write_or_die(fd, "data", 4);
	close(fd);
	xstat_mtime_dir(&st);
	اگر (مطابقت_stat_ڈیٹا(&بنیاد, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes "
				     "after updating a file"));
		جاؤ مکمل;
	}
	fputc('.', stderr);

	avoid_racy();
	close(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	اگر (مطابقت_stat_ڈیٹا(&بنیاد, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		جاؤ مکمل;
	}
	fputc('.', stderr);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	اگر (!مطابقت_stat_ڈیٹا(&بنیاد, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a file"));
		جاؤ مکمل;
	}
	fill_stat_ڈیٹا(&بنیاد, &st);
	fputc('.', stderr);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	اگر (!مطابقت_stat_ڈیٹا(&بنیاد, &st)) {
		fputc('\n', stderr);
		fprintf_ln(stderr, _("directory stat info does not "
				     "change after deleting a directory"));
		جاؤ مکمل;
	}

	اگر (rmdir(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(stderr, _(" OK"));
	ret = 1;

مکمل:
	strbuf_release(&mtime_dir);
	واپس ret;
}

ساکن صحیح mark_ce_flags(ثابت حرف *راستہ, صحیح flag, صحیح mark)
{
	صحیح namelen = لڑی_لمبائی(راستہ);
	صحیح pos = اشاریہ_name_pos(the_repository->اشاریہ, راستہ, namelen);
	اگر (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->اشاریہ, the_repository->اشاریہ->cache[pos]);
		اگر (mark)
			the_repository->اشاریہ->cache[pos]->ce_flags |= flag;
		ورنہ
			the_repository->اشاریہ->cache[pos]->ce_flags &= ~flag;
		the_repository->اشاریہ->cache[pos]->ce_flags |= CE_UPDATE_اندر_بنیاد;
		cache_درخت_invalidate_راستہ(the_repository->اشاریہ, راستہ);
		the_repository->اشاریہ->cache_بدلا |= CE_اندراج_بدلا;
		واپس 0;
	}
	واپس -1;
}

ساکن صحیح remove_one_راستہ(ثابت حرف *راستہ)
{
	اگر (!allow_remove)
		واپس error("%s: does not exist and --remove not passed", راستہ);
	اگر (remove_file_from_اشاریہ(the_repository->اشاریہ, راستہ))
		واپس error("%s: cannot remove from the index", راستہ);
	واپس 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
ساکن صحیح process_lstat_error(ثابت حرف *راستہ, صحیح err)
{
	اگر (is_missing_file_error(err))
		واپس remove_one_راستہ(راستہ);
	واپس error("lstat(\"%s\"): %s", راستہ, strerror(err));
}

ساکن صحیح جوڑو_one_راستہ(ثابت ساخت cache_اندراج *old, ثابت حرف *راستہ, صحیح len, ساخت stat *st)
{
	صحیح option;
	ساخت cache_اندراج *ce;

	/* Was the old index entry already up-to-date? */
	اگر (old && !ce_stage(old) && !ie_مطابقت_stat(the_repository->اشاریہ, old, st, 0))
		واپس 0;

	ce = make_خالی_خانے_cache_اندراج(the_repository->اشاریہ, len);
	حافظہ_نقل(ce->name, راستہ, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->اشاریہ, ce, st);
	ce->ce_منوال = ce_منوال_from_stat(the_repository, old, st->st_منوال);

	اگر (اشاریہ_راستہ(the_repository->اشاریہ, &ce->oid, راستہ, st,
		       info_only ? 0 : اشاریہ_WRITE_OBJECT)) {
		discard_cache_اندراج(ce);
		واپس -1;
	}
	option = allow_جوڑو ? جوڑو_CACHE_OK_مقصد_جوڑو : 0;
	option |= allow_replace ? جوڑو_CACHE_OK_مقصد_REPLACE : 0;
	اگر (جوڑو_اشاریہ_اندراج(the_repository->اشاریہ, ce, option)) {
		discard_cache_اندراج(ce);
		واپس error("%s: cannot add to the index - missing --add option?", راستہ);
	}
	واپس 0;
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
ساکن صحیح process_directory(ثابت حرف *راستہ, صحیح len, ساخت stat *st)
{
	ساخت object_id oid;
	صحیح pos = اشاریہ_name_pos(the_repository->اشاریہ, راستہ, len);

	/* Exact match: file or existing gitlink */
	اگر (pos >= 0) {
		ثابت ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[pos];
		اگر (S_ISGITLINK(ce->ce_منوال)) {

			/* Do nothing to the index if there is no HEAD! */
			اگر (repo_resolve_gitlink_ref(the_repository, راستہ,
						     "HEAD", &oid) < 0)
				واپس 0;

			واپس جوڑو_one_راستہ(ce, راستہ, len, st);
		}
		/* Should this be an unconditional error? */
		واپس remove_one_راستہ(راستہ);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	جب (pos < the_repository->اشاریہ->cache_nr) {
		ثابت ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[pos++];

		اگر (لڑی_موازنہ(ce->name, راستہ, len))
			توڑو;
		اگر (ce->name[len] > '/')
			توڑو;
		اگر (ce->name[len] < '/')
			جاری;

		/* Subdirectory match - error out */
		واپس error("%s: is a directory - add individual files instead", راستہ);
	}

	/* No match - should we add it as a gitlink? */
	اگر (!repo_resolve_gitlink_ref(the_repository, راستہ, "HEAD", &oid))
		واپس جوڑو_one_راستہ(تہی, راستہ, len, st);

	/* Error out. */
	واپس error("%s: is a directory - add files inside instead", راستہ);
}

ساکن صحیح process_راستہ(ثابت حرف *راستہ, ساخت stat *st, صحیح stat_errno)
{
	صحیح pos, len;
	ثابت ساخت cache_اندراج *ce;

	len = لڑی_لمبائی(راستہ);
	اگر (has_symlink_leading_راستہ(راستہ, len))
		واپس error("'%s' is beyond a symbolic link", راستہ);

	pos = اشاریہ_name_pos(the_repository->اشاریہ, راستہ, len);
	ce = pos < 0 ? تہی : the_repository->اشاریہ->cache[pos];
	اگر (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		اگر (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_اشاریہ(the_repository->اشاریہ, راستہ))
			واپس error("%s: cannot remove from the index", راستہ);
		واپس 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	اگر (stat_errno)
		واپس process_lstat_error(راستہ, stat_errno);

	اگر (S_ISDIR(st->st_منوال))
		واپس process_directory(راستہ, len, st);

	واپس جوڑو_one_راستہ(ce, راستہ, len, st);
}

ساکن صحیح جوڑو_cacheinfo(غیرعلامتی صحیح منوال, ثابت ساخت object_id *oid,
			 ثابت حرف *راستہ, صحیح stage)
{
	صحیح len, option;
	ساخت cache_اندراج *ce;

	اگر (!verify_راستہ(راستہ, منوال))
		واپس error("Invalid path '%s'", راستہ);

	len = لڑی_لمبائی(راستہ);
	ce = make_خالی_خانے_cache_اندراج(the_repository->اشاریہ, len);

	oidcpy(&ce->oid, oid);
	حافظہ_نقل(ce->name, راستہ, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_منوال = create_ce_منوال(منوال);
	اگر (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_جوڑو ? جوڑو_CACHE_OK_مقصد_جوڑو : 0;
	option |= allow_replace ? جوڑو_CACHE_OK_مقصد_REPLACE : 0;
	اگر (جوڑو_اشاریہ_اندراج(the_repository->اشاریہ, ce, option))
		واپس error("%s: cannot add to the index - missing --add option?",
			     راستہ);
	report("add '%s'", راستہ);
	واپس 0;
}

ساکن خالی chmod_راستہ(حرف flip, ثابت حرف *راستہ)
{
	صحیح pos;
	ساخت cache_اندراج *ce;

	pos = اشاریہ_name_pos(the_repository->اشاریہ, راستہ, لڑی_لمبائی(راستہ));
	اگر (pos < 0)
		جاؤ fail;
	ce = the_repository->اشاریہ->cache[pos];
	اگر (chmod_اشاریہ_اندراج(the_repository->اشاریہ, ce, flip) < 0)
		جاؤ fail;

	report("chmod %cx '%s'", flip, راستہ);
	واپس;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, راستہ);
}

ساکن خالی update_one(ثابت حرف *راستہ)
{
	صحیح stat_errno = 0;
	ساخت stat st;

	اگر (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_منوال = 0;
	ورنہ اگر (lstat(راستہ, &st) < 0) {
		st.st_منوال = 0;
		stat_errno = errno;
	} /* else stat is valid */

	اگر (!verify_راستہ(راستہ, st.st_منوال)) {
		fprintf(stderr, "Ignoring path %s\n", راستہ);
		واپس;
	}
	اگر (mark_valid_only) {
		اگر (mark_ce_flags(راستہ, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", راستہ);
		واپس;
	}
	اگر (mark_skip_worktree_only) {
		اگر (mark_ce_flags(راستہ, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", راستہ);
		واپس;
	}
	اگر (mark_fsmonitor_only) {
		اگر (mark_ce_flags(راستہ, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", راستہ);
		واپس;
	}

	اگر (force_remove) {
		اگر (remove_file_from_اشاریہ(the_repository->اشاریہ, راستہ))
			die("git update-index: unable to remove %s", راستہ);
		report("remove '%s'", راستہ);
		واپس;
	}
	اگر (process_راستہ(راستہ, &st, stat_errno))
		die("Unable to process path %s", راستہ);
	report("add '%s'", راستہ);
}

ساکن خالی read_اشاریہ_info(صحیح nul_رکن_سطر)
{
	ثابت صحیح hexsz = the_hash_algo->hexsz;
	ساخت strbuf buf = STRBUF_INIT;
	ساخت strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_رکن_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	جب (getline_fn(&buf, stdin) != EOF) {
		حرف *ptr, *tab;
		حرف *راستہ_name;
		ساخت object_id oid;
		غیرعلامتی صحیح منوال;
		غیرعلامتی طویل ul;
		صحیح stage;

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
		اگر (ptr == buf.buf || *ptr != ' '
		    || errno || (غیرعلامتی صحیح) ul != ul)
			جاؤ bad_سطر;
		منوال = ul;

		tab = strchr(ptr, '\t');
		اگر (!tab || tab - ptr < hexsz + 1)
			جاؤ bad_سطر;

		اگر (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		ورنہ {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		اگر (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			جاؤ bad_سطر;

		راستہ_name = ptr;
		اگر (!nul_رکن_سطر && راستہ_name[0] == '"') {
			strbuf_reset(&uq);
			اگر (unquote_c_style(&uq, راستہ_name, تہی)) {
				die("git update-index: bad quoting of path name");
			}
			راستہ_name = uq.buf;
		}

		اگر (!verify_راستہ(راستہ_name, منوال)) {
			fprintf(stderr, "Ignoring path %s\n", راستہ_name);
			جاری;
		}

		اگر (!منوال) {
			/* mode == 0 means there is no such path -- remove */
			اگر (remove_file_from_اشاریہ(the_repository->اشاریہ, راستہ_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		ورنہ {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			اگر (جوڑو_cacheinfo(منوال, &oid, راستہ_name, stage))
				die("git update-index: unable to update %s",
				    راستہ_name);
		}
		جاری;

	bad_سطر:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

ساکن ثابت حرف * ثابت update_اشاریہ_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	تہی
};

ساکن ساخت cache_اندراج *read_one_ent(ثابت حرف *which,
					ساخت object_id *ent, ثابت حرف *راستہ,
					صحیح namelen, صحیح stage)
{
	غیرعلامتی مختصر منوال;
	ساخت object_id oid;
	ساخت cache_اندراج *ce;

	اگر (get_درخت_اندراج(the_repository, ent, راستہ, &oid, &منوال)) {
		اگر (which)
			error("%s: not in %s branch.", راستہ, which);
		واپس تہی;
	}
	اگر (!the_repository->اشاریہ->sparse_اشاریہ && منوال == S_IFDIR) {
		اگر (which)
			error("%s: not a blob in %s branch.", راستہ, which);
		واپس تہی;
	}
	ce = make_خالی_خانے_cache_اندراج(the_repository->اشاریہ, namelen);

	oidcpy(&ce->oid, &oid);
	حافظہ_نقل(ce->name, راستہ, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_منوال = create_ce_منوال(منوال);
	واپس ce;
}

ساکن صحیح unresolve_one(ثابت حرف *راستہ)
{
	ساخت string_list_شے *شے;
	صحیح res = 0;

	اگر (!the_repository->اشاریہ->resolve_undo)
		واپس res;
	شے = string_list_lookup(the_repository->اشاریہ->resolve_undo, راستہ);
	اگر (!شے)
		واپس res; /* no resolve-undo record for the path */
	res = unmerge_اشاریہ_اندراج(the_repository->اشاریہ, راستہ, شے->util, 0);
	FREE_AND_NULL(شے->util);
	واپس res;
}

ساکن صحیح do_unresolve(صحیح ac, ثابت حرف **av,
			ثابت حرف *سابقہ, صحیح سابقہ_لمبائی)
{
	صحیح i;
	صحیح err = 0;

	برائے (i = 1; i < ac; i++) {
		ثابت حرف *arg = av[i];
		حرف *p = سابقہ_راستہ(the_repository, سابقہ, سابقہ_لمبائی, arg);
		err |= unresolve_one(p);
		آزاد(p);
	}
	واپس err;
}

ساکن صحیح do_reupdate(ثابت حرف **paths,
		       ثابت حرف *سابقہ)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	صحیح pos;
	صحیح has_سر = 1;
	ساخت pathspec pathspec;
	ساخت object_id سر_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       سابقہ, paths);

	اگر (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &سر_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_سر = 0;
 redo:
	برائے (pos = 0; pos < the_repository->اشاریہ->cache_nr; pos++) {
		ثابت ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[pos];
		ساخت cache_اندراج *old = تہی;
		صحیح save_nr;
		حرف *راستہ;

		اگر (ce_stage(ce) || !ce_راستہ_مطابقت(the_repository->اشاریہ, ce, &pathspec, تہی))
			جاری;
		اگر (has_سر)
			old = read_one_ent(تہی, &سر_oid,
					   ce->name, ce_namelen(ce), 0);
		اگر (old && ce->ce_منوال == old->ce_منوال &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_اندراج(old);
			جاری; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		اگر (S_ISSPARSEDIR(ce->ce_منوال)) {
			ensure_بھرا_اشاریہ(the_repository->اشاریہ);
			جاؤ redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->اشاریہ->cache_nr;
		راستہ = xstrdup(ce->name);
		update_one(راستہ);
		آزاد(راستہ);
		discard_cache_اندراج(old);
		اگر (save_nr != the_repository->اشاریہ->cache_nr)
			جاؤ redo;
	}
	clear_pathspec(&pathspec);
	واپس 0;
}

ساخت refresh_params {
	غیرعلامتی صحیح flags;
	صحیح *has_errors;
};

ساکن صحیح refresh(ساخت refresh_params *o, غیرعلامتی صحیح flag)
{
	تیاری_work_درخت(the_repository);
	repo_read_اشاریہ(the_repository);
	*o->has_errors |= refresh_اشاریہ(the_repository->اشاریہ, o->flags | flag, تہی,
					تہی, تہی);
	اگر (has_racy_timestamp(the_repository->اشاریہ)) {
		/*
		 * Even if nothing else has changed, updating the file
		 * increases the chance that racy timestamps become
		 * non-racy, helping future run-time performance.
		 * We do that even in case of "errors" returned by
		 * refresh_index() as these are no actual errors.
		 * cmd_status() does the same.
		 */
		the_repository->اشاریہ->cache_بدلا |= SOMETHING_بدلا;
	}
	واپس 0;
}

ساکن صحیح refresh_callback(ثابت ساخت option *opt,
				ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	واپس refresh(opt->قدر, 0);
}

ساکن صحیح really_refresh_callback(ثابت ساخت option *opt,
				ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	واپس refresh(opt->قدر, REFRESH_REALLY);
}

ساکن صحیح chmod_callback(ثابت ساخت option *opt,
				ثابت حرف *arg, صحیح unset)
{
	حرف *flip = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	اگر ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		واپس error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	واپس 0;
}

ساکن صحیح resolve_undo_clear_callback(ثابت ساخت option *opt غیر_مستعمل,
				ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_اشاریہ(the_repository->اشاریہ);
	واپس 0;
}

ساکن صحیح parse_new_style_cacheinfo(ثابت حرف *arg,
				     غیرعلامتی صحیح *منوال,
				     ساخت object_id *oid,
				     ثابت حرف **راستہ)
{
	غیرعلامتی طویل ul;
	حرف *endp;
	ثابت حرف *p;

	اگر (!arg)
		واپس -1;

	errno = 0;
	ul = strtoul(arg, &endp, 8);
	اگر (errno || endp == arg || *endp != ',' || (غیرعلامتی صحیح) ul != ul)
		واپس -1; /* not a new-style cacheinfo */
	*منوال = ul;
	endp++;
	اگر (parse_oid_hex(endp, oid, &p) || *p != ',')
		واپس -1;
	*راستہ = p + 1;
	واپس 0;
}

ساکن شمار parse_opt_نتیجہ cacheinfo_callback(
	ساخت parse_opt_ctx_t *ctx, ثابت ساخت option *opt غیر_مستعمل,
	ثابت حرف *arg, صحیح unset)
{
	ساخت object_id oid;
	غیرعلامتی صحیح منوال;
	ثابت حرف *راستہ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (!parse_new_style_cacheinfo(ctx->argv[1], &منوال, &oid, &راستہ)) {
		اگر (جوڑو_cacheinfo(منوال, &oid, راستہ, 0))
			die("git update-index: --cacheinfo cannot add %s", راستہ);
		ctx->argv++;
		ctx->argc--;
		واپس 0;
	}
	اگر (ctx->argc <= 3)
		واپس error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	اگر (strtoul_ui(*++ctx->argv, 8, &منوال) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    جوڑو_cacheinfo(منوال, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	واپس 0;
}

ساکن شمار parse_opt_نتیجہ stdin_cacheinfo_callback(
	ساخت parse_opt_ctx_t *ctx, ثابت ساخت option *opt,
	ثابت حرف *arg, صحیح unset)
{
	صحیح *nul_رکن_سطر = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (ctx->argc != 1)
		واپس error("option '%s' must be the last argument", opt->long_name);
	allow_جوڑو = allow_replace = allow_remove = 1;
	read_اشاریہ_info(*nul_رکن_سطر);
	واپس 0;
}

ساکن شمار parse_opt_نتیجہ stdin_callback(
	ساخت parse_opt_ctx_t *ctx, ثابت ساخت option *opt,
	ثابت حرف *arg, صحیح unset)
{
	صحیح *read_from_stdin = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (ctx->argc != 1)
		واپس error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	واپس 0;
}

ساکن شمار parse_opt_نتیجہ unresolve_callback(
	ساخت parse_opt_ctx_t *ctx, ثابت ساخت option *opt,
	ثابت حرف *arg, صحیح unset)
{
	صحیح *has_errors = opt->قدر;
	ثابت حرف *سابقہ = the_repository->سابقہ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				سابقہ, سابقہ ? لڑی_لمبائی(سابقہ) : 0);
	اگر (*has_errors)
		the_repository->اشاریہ->cache_بدلا = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	واپس 0;
}

ساکن شمار parse_opt_نتیجہ reupdate_callback(
	ساخت parse_opt_ctx_t *ctx, ثابت ساخت option *opt,
	ثابت حرف *arg, صحیح unset)
{
	صحیح *has_errors = opt->قدر;
	ثابت حرف *سابقہ = the_repository->سابقہ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	تیاری_work_درخت(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, سابقہ);
	اگر (*has_errors)
		the_repository->اشاریہ->cache_بدلا = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	واپس 0;
}

صحیح cmd_update_اشاریہ(صحیح argc,
		     ثابت حرف **argv,
		     ثابت حرف *سابقہ,
		     ساخت repository *repo غیر_مستعمل)
{
	صحیح newfd, entries, has_errors = 0, nul_رکن_سطر = 0;
	شمار uc_منوال untracked_cache = UC_UNSPECIFIED;
	صحیح read_from_stdin = 0;
	صحیح سابقہ_لمبائی = سابقہ ? لڑی_لمبائی(سابقہ) : 0;
	صحیح preferred_اشاریہ_format = 0;
	حرف سیٹ_executable_ثنائی_ہندسہ = 0;
	ساخت refresh_params refresh_دلائل = {0, &has_errors};
	صحیح تالا_error = 0;
	صحیح بانٹو_اشاریہ = -1;
	صحیح force_write = 0;
	صحیح fsmonitor = -1;
	ساخت تالا_file تالا_file = تالا_INIT;
	ساخت parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	صحیح parseopt_state = PARSE_OPT_UNKNOWN;
	ساخت repository *r = the_repository;
	ساخت odb_transaction *transaction;
	ساخت option options[] = {
		OPT_ثنائی_ہندسہ('q', تہی, &refresh_دلائل.flags,
			N_("continue refresh even when index needs update"),
			REFRESH_QUIET),
		OPT_ثنائی_ہندسہ(0, "ignore-submodules", &refresh_دلائل.flags,
			N_("refresh: ignore submodules"),
			REFRESH_IGNORE_SUBMODULES),
		OPT_سیٹ_INT(0, "add", &allow_جوڑو,
			N_("do not ignore new files"), 1),
		OPT_سیٹ_INT(0, "replace", &allow_replace,
			N_("let files replace directories and vice-versa"), 1),
		OPT_سیٹ_INT(0, "remove", &allow_remove,
			N_("notice files missing from worktree"), 1),
		OPT_ثنائی_ہندسہ(0, "unmerged", &refresh_دلائل.flags,
			N_("refresh even if index contains unmerged entries"),
			REFRESH_UNMERGED),
		OPT_CALLBACK_F(0, "refresh", &refresh_دلائل, تہی,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_دلائل, تہی,
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
		OPT_BOOL('z', تہی, &nul_رکن_سطر,
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
		OPT_ثنائی_ہندسہ(0, "ignore-missing", &refresh_دلائل.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_سیٹ_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", تہی, تہی,
			N_("(for porcelains) forget saved unresolved conflicts"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			resolve_undo_clear_callback),
		OPT_INTEGER(0, "index-version", &preferred_اشاریہ_format,
			N_("write index in this format")),
		OPT_سیٹ_INT(0, "show-index-version", &preferred_اشاریہ_format,
			    N_("report on-disk index format version"), -1),
		OPT_BOOL(0, "split-index", &بانٹو_اشاریہ,
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
		OPT_انتہا()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_اشاریہ_usage, options);

	repo_config(the_repository, git_default_config, تہی);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_اشاریہ(the_repository, &تالا_file, 0);
	اگر (newfd < 0)
		تالا_error = errno;

	entries = repo_read_اشاریہ(the_repository);
	اگر (entries < 0)
		die("cache corrupted");

	the_repository->اشاریہ->updated_skipworktree = 1;

	/*
	 * Custom copy of parse_options() because we want to handle
	 * filename arguments as they come.
	 */
	parse_options_آغاز(&ctx, argc, argv, سابقہ,
			    options, PARSE_OPT_STOP_AT_NON_OPTION);

	/*
	 * Allow the object layer to optimize adding multiple objects in
	 * a batch.
	 */
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	جب (ctx.argc) {
		اگر (parseopt_state != PARSE_OPT_مکمل)
			parseopt_state = parse_options_قدم(&ctx, options,
							    update_اشاریہ_usage);
		اگر (!ctx.argc)
			توڑو;
		انتخاب (parseopt_state) {
		حالت PARSE_OPT_HELP:
			باہر_نکلو(0);
		حالت PARSE_OPT_HELP_ERROR:
		حالت PARSE_OPT_ERROR:
			باہر_نکلو(129);
		حالت PARSE_OPT_COMPLETE:
			باہر_نکلو(0);
		حالت PARSE_OPT_NON_OPTION:
		حالت PARSE_OPT_مکمل:
		{
			ثابت حرف *راستہ = ctx.argv[0];
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
			اگر (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = تہی;
			}

			تیاری_work_درخت(the_repository);
			p = سابقہ_راستہ(the_repository, سابقہ, سابقہ_لمبائی, راستہ);
			update_one(p);
			اگر (سیٹ_executable_ثنائی_ہندسہ)
				chmod_راستہ(سیٹ_executable_ثنائی_ہندسہ, p);
			آزاد(p);
			ctx.argc--;
			ctx.argv++;
			توڑو;
		}
		حالت PARSE_OPT_UNKNOWN:
			اگر (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			ورنہ
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_اشاریہ_usage, options);
		}
	}
	argc = parse_options_انتہا(&ctx);

	getline_fn = nul_رکن_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	اگر (preferred_اشاریہ_format) {
		اگر (preferred_اشاریہ_format < 0) {
			لکھو(_("%d\n"), the_repository->اشاریہ->نسخہ);
		} ورنہ اگر (preferred_اشاریہ_format < اشاریہ_FORMAT_LB ||
			   اشاریہ_FORMAT_UB < preferred_اشاریہ_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_اشاریہ_format,
			    اشاریہ_FORMAT_LB, اشاریہ_FORMAT_UB);
		} ورنہ {
			اگر (the_repository->اشاریہ->نسخہ != preferred_اشاریہ_format)
				the_repository->اشاریہ->cache_بدلا |= SOMETHING_بدلا;
			report(_("index-version: was %d, set to %d"),
			       the_repository->اشاریہ->نسخہ, preferred_اشاریہ_format);
			the_repository->اشاریہ->نسخہ = preferred_اشاریہ_format;
		}
	}

	اگر (read_from_stdin) {
		ساخت strbuf buf = STRBUF_INIT;
		ساخت strbuf unquoted = STRBUF_INIT;

		تیاری_work_درخت(the_repository);
		جب (getline_fn(&buf, stdin) != EOF) {
			حرف *p;
			اگر (!nul_رکن_سطر && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				اگر (unquote_c_style(&unquoted, buf.buf, تہی))
					die("line is badly quoted");
				strbuf_تبادلہ(&buf, &unquoted);
			}
			p = سابقہ_راستہ(the_repository, سابقہ, سابقہ_لمبائی, buf.buf);
			update_one(p);
			اگر (سیٹ_executable_ثنائی_ہندسہ)
				chmod_راستہ(سیٹ_executable_ثنائی_ہندسہ, p);
			آزاد(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	اگر (بانٹو_اشاریہ > 0) {
		اگر (repo_config_get_بانٹو_اشاریہ(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		اگر (the_repository->اشاریہ->بانٹو_اشاریہ)
			the_repository->اشاریہ->cache_بدلا |= بانٹو_اشاریہ_ORDERED;
		ورنہ
			جوڑو_بانٹو_اشاریہ(the_repository->اشاریہ);
	} ورنہ اگر (!بانٹو_اشاریہ) {
		اگر (repo_config_get_بانٹو_اشاریہ(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_بانٹو_اشاریہ(the_repository->اشاریہ);
	}

	prepare_repo_settings(r);
	انتخاب (untracked_cache) {
	حالت UC_UNSPECIFIED:
		توڑو;
	حالت UC_DISABLE:
		اگر (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->اشاریہ);
		report(_("Untracked cache disabled"));
		توڑو;
	حالت UC_TEST:
		تیاری_work_درخت(the_repository);
		واپس !test_if_untracked_cache_is_supported();
	حالت UC_ENABLE:
	حالت UC_FORCE:
		اگر (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		جوڑو_untracked_cache(the_repository->اشاریہ);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_درخت(the_repository));
		توڑو;
	طےشدہ:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	اگر (fsmonitor > 0) {
		شمار fsmonitor_منوال fsm_منوال = fsm_settings_get_منوال(r);
		شمار fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		اگر (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_پیغام(r, reason));

		اگر (fsm_منوال == FSMONITOR_منوال_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		جوڑو_fsmonitor(the_repository->اشاریہ);
		report(_("fsmonitor enabled"));
	} ورنہ اگر (!fsmonitor) {
		شمار fsmonitor_منوال fsm_منوال = fsm_settings_get_منوال(r);
		اگر (fsm_منوال > FSMONITOR_منوال_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->اشاریہ);
		report(_("fsmonitor disabled"));
	}

	اگر (the_repository->اشاریہ->cache_بدلا || force_write) {
		اگر (newfd < 0) {
			اگر (refresh_دلائل.flags & REFRESH_QUIET)
				باہر_نکلو(128);
			unable_مقصد_تالا_die(repo_get_اشاریہ_file(the_repository), تالا_error);
		}
		اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا_file, COMMIT_تالا))
			die("Unable to write new index file");
	}

	rollback_تالا_file(&تالا_file);

	واپس has_errors ? 1 : 0;
}
