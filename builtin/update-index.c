/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/transaction.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "resolve-undo.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "read-cache.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "split-index.h"
#अन्तर्भाव "symlinks.h"
#अन्तर्भाव "fsmonitor.h"
#अन्तर्भाव "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
स्थिर पूर्णाङ्क allow_add;
स्थिर पूर्णाङ्क allow_remove;
स्थिर पूर्णाङ्क allow_replace;
स्थिर पूर्णाङ्क info_only;
स्थिर पूर्णाङ्क force_remove;
स्थिर पूर्णाङ्क verbose;
स्थिर पूर्णाङ्क mark_valid_only;
स्थिर पूर्णाङ्क mark_skip_worktree_only;
स्थिर पूर्णाङ्क mark_fsmonitor_only;
स्थिर पूर्णाङ्क ignore_skip_worktree_entries;
#define MARK_FLAG 1
#define UNMARK_FLAG 2
स्थिर संरचना strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
गणना uc_mode {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (म_लिखो, 1, 2)))
स्थिर शून्य report(अचर वर्ण *fmt, ...)
{
	बहु_सूची vp;

	यदि (!verbose)
		फर्काऊ;

	बहु_शुरू(vp, fmt);
	भ_लिखो(fmt, vp);
	अक्षर_दो('\n');
	बहु_पूर्ण(vp);
}

स्थिर शून्य remove_test_directory(शून्य)
{
	यदि (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

स्थिर अचर वर्ण *get_mtime_path(अचर वर्ण *path)
{
	स्थिर संरचना strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, path);
	फर्काऊ sb.buf;
}

स्थिर शून्य xmkdir(अचर वर्ण *path)
{
	path = get_mtime_path(path);
	यदि (सूची_गढ़ो(path, 0700))
		die_errno(_("failed to create directory %s"), path);
}

स्थिर पूर्णाङ्क xstat_mtime_dir(संरचना stat *st)
{
	यदि (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क create_file(अचर वर्ण *path)
{
	पूर्णाङ्क fd;
	path = get_mtime_path(path);
	fd = xopen(path, O_CREAT | O_RDWR, 0644);
	फर्काऊ fd;
}

स्थिर शून्य xunlink(अचर वर्ण *path)
{
	path = get_mtime_path(path);
	यदि (unlink(path))
		die_errno(_("failed to delete file %s"), path);
}

स्थिर शून्य xrmdir(अचर वर्ण *path)
{
	path = get_mtime_path(path);
	यदि (सूची_हटाओ(path))
		die_errno(_("failed to delete directory %s"), path);
}

स्थिर शून्य avoid_racy(शून्य)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

स्थिर पूर्णाङ्क test_if_untracked_cache_is_supported(शून्य)
{
	संरचना stat st;
	संरचना stat_data base;
	पूर्णाङ्क fd, ret = 0;
	वर्ण *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	यदि (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	ख_लिखो(मानक_त्रुटि, _("Testing mtime in '%s' "), cwd);
	मुक्त(cwd);

	निकास_पर(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_data(&base, &st);
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	यदि (!match_stat_data(&base, &st)) {
		बंद(fd);
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि,_("directory stat info does not "
				    "change after adding a new file"));
		गच्छ done;
	}
	fill_stat_data(&base, &st);
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	यदि (!match_stat_data(&base, &st)) {
		बंद(fd);
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि, _("directory stat info does not change "
				     "after adding a new directory"));
		गच्छ done;
	}
	fill_stat_data(&base, &st);
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	write_or_die(fd, "data", 4);
	बंद(fd);
	xstat_mtime_dir(&st);
	यदि (match_stat_data(&base, &st)) {
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि, _("directory stat info changes "
				     "after updating a file"));
		गच्छ done;
	}
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	बंद(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	यदि (match_stat_data(&base, &st)) {
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		गच्छ done;
	}
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	यदि (!match_stat_data(&base, &st)) {
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि, _("directory stat info does not "
				     "change after deleting a file"));
		गच्छ done;
	}
	fill_stat_data(&base, &st);
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	यदि (!match_stat_data(&base, &st)) {
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि, _("directory stat info does not "
				     "change after deleting a directory"));
		गच्छ done;
	}

	यदि (सूची_हटाओ(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(मानक_त्रुटि, _(" OK"));
	ret = 1;

done:
	strbuf_release(&mtime_dir);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क mark_ce_flags(अचर वर्ण *path, पूर्णाङ्क flag, पूर्णाङ्क mark)
{
	पूर्णाङ्क namelen = म_माप(path);
	पूर्णाङ्क pos = index_name_pos(the_repository->index, path, namelen);
	यदि (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->index, the_repository->index->cache[pos]);
		यदि (mark)
			the_repository->index->cache[pos]->ce_flags |= flag;
		अन्यथा
			the_repository->index->cache[pos]->ce_flags &= ~flag;
		the_repository->index->cache[pos]->ce_flags |= CE_UPDATE_IN_BASE;
		cache_रूख_invalidate_path(the_repository->index, path);
		the_repository->index->cache_changed |= CE_ENTRY_CHANGED;
		फर्काऊ 0;
	}
	फर्काऊ -1;
}

स्थिर पूर्णाङ्क remove_one_path(अचर वर्ण *path)
{
	यदि (!allow_remove)
		फर्काऊ error("%s: does not exist and --remove not passed", path);
	यदि (remove_file_from_index(the_repository->index, path))
		फर्काऊ error("%s: cannot remove from the index", path);
	फर्काऊ 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
स्थिर पूर्णाङ्क process_lstat_error(अचर वर्ण *path, पूर्णाङ्क err)
{
	यदि (is_missing_file_error(err))
		फर्काऊ remove_one_path(path);
	फर्काऊ error("lstat(\"%s\"): %s", path, म_त्रुटि(err));
}

स्थिर पूर्णाङ्क add_one_path(अचर संरचना cache_entry *old, अचर वर्ण *path, पूर्णाङ्क len, संरचना stat *st)
{
	पूर्णाङ्क option;
	संरचना cache_entry *ce;

	/* Was the old index entry already up-to-date? */
	यदि (old && !ce_stage(old) && !ie_match_stat(the_repository->index, old, st, 0))
		फर्काऊ 0;

	ce = make_empty_cache_entry(the_repository->index, len);
	स_नकल(ce->name, path, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->index, ce, st);
	ce->ce_mode = ce_mode_from_stat(the_repository, old, st->st_mode);

	यदि (index_path(the_repository->index, &ce->oid, path, st,
		       info_only ? 0 : INDEX_WRITE_OBJECT)) {
		discard_cache_entry(ce);
		फर्काऊ -1;
	}
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	यदि (add_index_entry(the_repository->index, ce, option)) {
		discard_cache_entry(ce);
		फर्काऊ error("%s: cannot add to the index - missing --add option?", path);
	}
	फर्काऊ 0;
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
स्थिर पूर्णाङ्क process_directory(अचर वर्ण *path, पूर्णाङ्क len, संरचना stat *st)
{
	संरचना object_id oid;
	पूर्णाङ्क pos = index_name_pos(the_repository->index, path, len);

	/* Exact match: file or existing gitlink */
	यदि (pos >= 0) {
		अचर संरचना cache_entry *ce = the_repository->index->cache[pos];
		यदि (S_ISGITLINK(ce->ce_mode)) {

			/* Do nothing to the index if there is no HEAD! */
			यदि (repo_resolve_gitlink_ref(the_repository, path,
						     "HEAD", &oid) < 0)
				फर्काऊ 0;

			फर्काऊ add_one_path(ce, path, len, st);
		}
		/* Should this be an unconditional error? */
		फर्काऊ remove_one_path(path);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	जबसम्म (pos < the_repository->index->cache_nr) {
		अचर संरचना cache_entry *ce = the_repository->index->cache[pos++];

		यदि (म_भेदन(ce->name, path, len))
			रोक;
		यदि (ce->name[len] > '/')
			रोक;
		यदि (ce->name[len] < '/')
			जारी;

		/* Subdirectory match - error out */
		फर्काऊ error("%s: is a directory - add individual files instead", path);
	}

	/* No match - should we add it as a gitlink? */
	यदि (!repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid))
		फर्काऊ add_one_path(रिक्त, path, len, st);

	/* Error out. */
	फर्काऊ error("%s: is a directory - add files inside instead", path);
}

स्थिर पूर्णाङ्क process_path(अचर वर्ण *path, संरचना stat *st, पूर्णाङ्क stat_errno)
{
	पूर्णाङ्क pos, len;
	अचर संरचना cache_entry *ce;

	len = म_माप(path);
	यदि (has_symlink_leading_path(path, len))
		फर्काऊ error("'%s' is beyond a symbolic link", path);

	pos = index_name_pos(the_repository->index, path, len);
	ce = pos < 0 ? रिक्त : the_repository->index->cache[pos];
	यदि (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		यदि (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_index(the_repository->index, path))
			फर्काऊ error("%s: cannot remove from the index", path);
		फर्काऊ 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	यदि (stat_errno)
		फर्काऊ process_lstat_error(path, stat_errno);

	यदि (S_ISDIR(st->st_mode))
		फर्काऊ process_directory(path, len, st);

	फर्काऊ add_one_path(ce, path, len, st);
}

स्थिर पूर्णाङ्क add_cacheinfo(अचिन्हित पूर्णाङ्क mode, अचर संरचना object_id *oid,
			 अचर वर्ण *path, पूर्णाङ्क stage)
{
	पूर्णाङ्क len, option;
	संरचना cache_entry *ce;

	यदि (!verify_path(path, mode))
		फर्काऊ error("Invalid path '%s'", path);

	len = म_माप(path);
	ce = make_empty_cache_entry(the_repository->index, len);

	oidcpy(&ce->oid, oid);
	स_नकल(ce->name, path, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_mode = create_ce_mode(mode);
	यदि (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	यदि (add_index_entry(the_repository->index, ce, option))
		फर्काऊ error("%s: cannot add to the index - missing --add option?",
			     path);
	report("add '%s'", path);
	फर्काऊ 0;
}

स्थिर शून्य chmod_path(वर्ण flip, अचर वर्ण *path)
{
	पूर्णाङ्क pos;
	संरचना cache_entry *ce;

	pos = index_name_pos(the_repository->index, path, म_माप(path));
	यदि (pos < 0)
		गच्छ fail;
	ce = the_repository->index->cache[pos];
	यदि (chmod_index_entry(the_repository->index, ce, flip) < 0)
		गच्छ fail;

	report("chmod %cx '%s'", flip, path);
	फर्काऊ;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, path);
}

स्थिर शून्य update_one(अचर वर्ण *path)
{
	पूर्णाङ्क stat_errno = 0;
	संरचना stat st;

	यदि (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_mode = 0;
	अन्यथा यदि (lstat(path, &st) < 0) {
		st.st_mode = 0;
		stat_errno = त्रुटि_सं;
	} /* else stat is valid */

	यदि (!verify_path(path, st.st_mode)) {
		ख_लिखो(मानक_त्रुटि, "Ignoring path %s\n", path);
		फर्काऊ;
	}
	यदि (mark_valid_only) {
		यदि (mark_ce_flags(path, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		फर्काऊ;
	}
	यदि (mark_skip_worktree_only) {
		यदि (mark_ce_flags(path, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		फर्काऊ;
	}
	यदि (mark_fsmonitor_only) {
		यदि (mark_ce_flags(path, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		फर्काऊ;
	}

	यदि (force_remove) {
		यदि (remove_file_from_index(the_repository->index, path))
			die("git update-index: unable to remove %s", path);
		report("remove '%s'", path);
		फर्काऊ;
	}
	यदि (process_path(path, &st, stat_errno))
		die("Unable to process path %s", path);
	report("add '%s'", path);
}

स्थिर शून्य read_index_info(पूर्णाङ्क nul_term_पङ्क्ति)
{
	अचर पूर्णाङ्क hexsz = the_hash_algo->hexsz;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_पङ्क्ति ? strbuf_getline_nul : strbuf_getline_lf;
	जबसम्म (getline_fn(&buf, मानक_निवेश) != खातापूर्ण) {
		वर्ण *ptr, *tab;
		वर्ण *path_name;
		संरचना object_id oid;
		अचिन्हित पूर्णाङ्क mode;
		अचिन्हित दीर्घ ul;
		पूर्णाङ्क stage;

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
		त्रुटि_सं = 0;
		ul = म_से_अदीर्घ(buf.buf, &ptr, 8);
		यदि (ptr == buf.buf || *ptr != ' '
		    || त्रुटि_सं || (अचिन्हित पूर्णाङ्क) ul != ul)
			गच्छ bad_पङ्क्ति;
		mode = ul;

		tab = म_अक्षर(ptr, '\t');
		यदि (!tab || tab - ptr < hexsz + 1)
			गच्छ bad_पङ्क्ति;

		यदि (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		अन्यथा {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		यदि (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			गच्छ bad_पङ्क्ति;

		path_name = ptr;
		यदि (!nul_term_पङ्क्ति && path_name[0] == '"') {
			strbuf_reset(&uq);
			यदि (unquote_c_style(&uq, path_name, रिक्त)) {
				die("git update-index: bad quoting of path name");
			}
			path_name = uq.buf;
		}

		यदि (!verify_path(path_name, mode)) {
			ख_लिखो(मानक_त्रुटि, "Ignoring path %s\n", path_name);
			जारी;
		}

		यदि (!mode) {
			/* mode == 0 means there is no such path -- remove */
			यदि (remove_file_from_index(the_repository->index, path_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		अन्यथा {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			यदि (add_cacheinfo(mode, &oid, path_name, stage))
				die("git update-index: unable to update %s",
				    path_name);
		}
		जारी;

	bad_पङ्क्ति:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

स्थिर अचर वर्ण * अचर update_index_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	रिक्त
};

स्थिर संरचना cache_entry *read_one_ent(अचर वर्ण *which,
					संरचना object_id *ent, अचर वर्ण *path,
					पूर्णाङ्क namelen, पूर्णाङ्क stage)
{
	अचिन्हित लघु mode;
	संरचना object_id oid;
	संरचना cache_entry *ce;

	यदि (get_रूख_entry(the_repository, ent, path, &oid, &mode)) {
		यदि (which)
			error("%s: not in %s branch.", path, which);
		फर्काऊ रिक्त;
	}
	यदि (!the_repository->index->sparse_index && mode == S_IFDIR) {
		यदि (which)
			error("%s: not a blob in %s branch.", path, which);
		फर्काऊ रिक्त;
	}
	ce = make_empty_cache_entry(the_repository->index, namelen);

	oidcpy(&ce->oid, &oid);
	स_नकल(ce->name, path, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_mode = create_ce_mode(mode);
	फर्काऊ ce;
}

स्थिर पूर्णाङ्क unresolve_one(अचर वर्ण *path)
{
	संरचना string_list_item *item;
	पूर्णाङ्क res = 0;

	यदि (!the_repository->index->resolve_undo)
		फर्काऊ res;
	item = string_list_lookup(the_repository->index->resolve_undo, path);
	यदि (!item)
		फर्काऊ res; /* no resolve-undo record for the path */
	res = unmerge_index_entry(the_repository->index, path, item->util, 0);
	FREE_AND_NULL(item->util);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क do_unresolve(पूर्णाङ्क ac, अचर वर्ण **av,
			अचर वर्ण *prefix, पूर्णाङ्क prefix_length)
{
	पूर्णाङ्क i;
	पूर्णाङ्क err = 0;

	लागि (i = 1; i < ac; i++) {
		अचर वर्ण *arg = av[i];
		वर्ण *p = prefix_path(the_repository, prefix, prefix_length, arg);
		err |= unresolve_one(p);
		मुक्त(p);
	}
	फर्काऊ err;
}

स्थिर पूर्णाङ्क do_reupdate(अचर वर्ण **paths,
		       अचर वर्ण *prefix)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	पूर्णाङ्क pos;
	पूर्णाङ्क has_head = 1;
	संरचना pathspec pathspec;
	संरचना object_id head_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, paths);

	यदि (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &head_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_head = 0;
 redo:
	लागि (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		अचर संरचना cache_entry *ce = the_repository->index->cache[pos];
		संरचना cache_entry *old = रिक्त;
		पूर्णाङ्क save_nr;
		वर्ण *path;

		यदि (ce_stage(ce) || !ce_path_match(the_repository->index, ce, &pathspec, रिक्त))
			जारी;
		यदि (has_head)
			old = read_one_ent(रिक्त, &head_oid,
					   ce->name, ce_namelen(ce), 0);
		यदि (old && ce->ce_mode == old->ce_mode &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_entry(old);
			जारी; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		यदि (S_ISSPARSEDIR(ce->ce_mode)) {
			ensure_full_index(the_repository->index);
			गच्छ redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->index->cache_nr;
		path = xstrdup(ce->name);
		update_one(path);
		मुक्त(path);
		discard_cache_entry(old);
		यदि (save_nr != the_repository->index->cache_nr)
			गच्छ redo;
	}
	clear_pathspec(&pathspec);
	फर्काऊ 0;
}

संरचना refresh_params {
	अचिन्हित पूर्णाङ्क flags;
	पूर्णाङ्क *has_errors;
};

स्थिर पूर्णाङ्क refresh(संरचना refresh_params *o, अचिन्हित पूर्णाङ्क flag)
{
	setup_work_रूख(the_repository);
	repo_read_index(the_repository);
	*o->has_errors |= refresh_index(the_repository->index, o->flags | flag, रिक्त,
					रिक्त, रिक्त);
	यदि (has_racy_timestamp(the_repository->index)) {
		/*
		 * Even if nothing else has changed, updating the file
		 * increases the chance that racy timestamps become
		 * non-racy, helping future run-time performance.
		 * We do that even in case of "errors" returned by
		 * refresh_index() as these are no actual errors.
		 * cmd_status() does the same.
		 */
		the_repository->index->cache_changed |= SOMETHING_CHANGED;
	}
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क refresh_callback(अचर संरचना option *opt,
				अचर वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	फर्काऊ refresh(opt->मान, 0);
}

स्थिर पूर्णाङ्क really_refresh_callback(अचर संरचना option *opt,
				अचर वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	फर्काऊ refresh(opt->मान, REFRESH_REALLY);
}

स्थिर पूर्णाङ्क chmod_callback(अचर संरचना option *opt,
				अचर वर्ण *arg, पूर्णाङ्क unset)
{
	वर्ण *flip = opt->मान;
	BUG_ON_OPT_NEG(unset);
	यदि ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		फर्काऊ error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क resolve_undo_clear_callback(अचर संरचना option *opt UNUSED,
				अचर वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_index(the_repository->index);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क parse_new_style_cacheinfo(अचर वर्ण *arg,
				     अचिन्हित पूर्णाङ्क *mode,
				     संरचना object_id *oid,
				     अचर वर्ण **path)
{
	अचिन्हित दीर्घ ul;
	वर्ण *endp;
	अचर वर्ण *p;

	यदि (!arg)
		फर्काऊ -1;

	त्रुटि_सं = 0;
	ul = म_से_अदीर्घ(arg, &endp, 8);
	यदि (त्रुटि_सं || endp == arg || *endp != ',' || (अचिन्हित पूर्णाङ्क) ul != ul)
		फर्काऊ -1; /* not a new-style cacheinfo */
	*mode = ul;
	endp++;
	यदि (parse_oid_hex(endp, oid, &p) || *p != ',')
		फर्काऊ -1;
	*path = p + 1;
	फर्काऊ 0;
}

स्थिर गणना parse_opt_result cacheinfo_callback(
	संरचना parse_opt_ctx_t *ctx, अचर संरचना option *opt UNUSED,
	अचर वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना object_id oid;
	अचिन्हित पूर्णाङ्क mode;
	अचर वर्ण *path;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	यदि (!parse_new_style_cacheinfo(ctx->argv[1], &mode, &oid, &path)) {
		यदि (add_cacheinfo(mode, &oid, path, 0))
			die("git update-index: --cacheinfo cannot add %s", path);
		ctx->argv++;
		ctx->argc--;
		फर्काऊ 0;
	}
	यदि (ctx->argc <= 3)
		फर्काऊ error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	यदि (strtoul_ui(*++ctx->argv, 8, &mode) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    add_cacheinfo(mode, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	फर्काऊ 0;
}

स्थिर गणना parse_opt_result stdin_cacheinfo_callback(
	संरचना parse_opt_ctx_t *ctx, अचर संरचना option *opt,
	अचर वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *nul_term_पङ्क्ति = opt->मान;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	यदि (ctx->argc != 1)
		फर्काऊ error("option '%s' must be the last argument", opt->long_name);
	allow_add = allow_replace = allow_remove = 1;
	read_index_info(*nul_term_पङ्क्ति);
	फर्काऊ 0;
}

स्थिर गणना parse_opt_result stdin_callback(
	संरचना parse_opt_ctx_t *ctx, अचर संरचना option *opt,
	अचर वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *read_from_stdin = opt->मान;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	यदि (ctx->argc != 1)
		फर्काऊ error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	फर्काऊ 0;
}

स्थिर गणना parse_opt_result unresolve_callback(
	संरचना parse_opt_ctx_t *ctx, अचर संरचना option *opt,
	अचर वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *has_errors = opt->मान;
	अचर वर्ण *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				prefix, prefix ? म_माप(prefix) : 0);
	यदि (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	फर्काऊ 0;
}

स्थिर गणना parse_opt_result reupdate_callback(
	संरचना parse_opt_ctx_t *ctx, अचर संरचना option *opt,
	अचर वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *has_errors = opt->मान;
	अचर वर्ण *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	setup_work_रूख(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, prefix);
	यदि (*has_errors)
		the_repository->index->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	फर्काऊ 0;
}

पूर्णाङ्क cmd_update_index(पूर्णाङ्क argc,
		     अचर वर्ण **argv,
		     अचर वर्ण *prefix,
		     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क newfd, entries, has_errors = 0, nul_term_पङ्क्ति = 0;
	गणना uc_mode untracked_cache = UC_UNSPECIFIED;
	पूर्णाङ्क read_from_stdin = 0;
	पूर्णाङ्क prefix_length = prefix ? म_माप(prefix) : 0;
	पूर्णाङ्क preferred_index_format = 0;
	वर्ण set_executable_bit = 0;
	संरचना refresh_params refresh_args = {0, &has_errors};
	पूर्णाङ्क lock_error = 0;
	पूर्णाङ्क split_index = -1;
	पूर्णाङ्क force_write = 0;
	पूर्णाङ्क fsmonitor = -1;
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	पूर्णाङ्क parseopt_state = PARSE_OPT_UNKNOWN;
	संरचना repository *r = the_repository;
	संरचना odb_transaction *transaction;
	संरचना option options[] = {
		OPT_BIT('q', रिक्त, &refresh_args.flags,
			N_("continue refresh even when index needs update"),
			REFRESH_QUIET),
		OPT_BIT(0, "ignore-submodules", &refresh_args.flags,
			N_("refresh: ignore submodules"),
			REFRESH_IGNORE_SUBMODULES),
		OPT_SET_INT(0, "add", &allow_add,
			N_("do not ignore new files"), 1),
		OPT_SET_INT(0, "replace", &allow_replace,
			N_("let files replace directories and vice-versa"), 1),
		OPT_SET_INT(0, "remove", &allow_remove,
			N_("notice files missing from worktree"), 1),
		OPT_BIT(0, "unmerged", &refresh_args.flags,
			N_("refresh even if index contains unmerged entries"),
			REFRESH_UNMERGED),
		OPT_CALLBACK_F(0, "refresh", &refresh_args, रिक्त,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_args, रिक्त,
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
		OPT_CALLBACK_F(0, "chmod", &set_executable_bit, "(+|-)x",
			N_("override the executable bit of the listed files"),
			PARSE_OPT_NONEG,
			chmod_callback),
		{
			.type = OPTION_SET_INT,
			.long_name = "assume-unchanged",
			.मान = &mark_valid_only,
			.precision = परिमाण(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-assume-unchanged",
			.मान = &mark_valid_only,
			.precision = परिमाण(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "skip-worktree",
			.मान = &mark_skip_worktree_only,
			.precision = परिमाण(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-skip-worktree",
			.मान = &mark_skip_worktree_only,
			.precision = परिमाण(mark_skip_worktree_only),
			.help = N_("clear skip-worktree bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_BOOL(0, "ignore-skip-worktree-entries", &ignore_skip_worktree_entries,
			 N_("do not touch index-only entries")),
		OPT_SET_INT(0, "info-only", &info_only,
			N_("add to index only; do not add content to object database"), 1),
		OPT_SET_INT(0, "force-remove", &force_remove,
			N_("remove named paths even if present in worktree"), 1),
		OPT_BOOL('z', रिक्त, &nul_term_पङ्क्ति,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.मान = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.मान = &nul_term_पङ्क्ति,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.मान = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.मान = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_BIT(0, "ignore-missing", &refresh_args.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_SET_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", रिक्त, रिक्त,
			N_("(for porcelains) forget saved unresolved conflicts"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			resolve_undo_clear_callback),
		OPT_INTEGER(0, "index-version", &preferred_index_format,
			N_("write index in this format")),
		OPT_SET_INT(0, "show-index-version", &preferred_index_format,
			    N_("report on-disk index format version"), -1),
		OPT_BOOL(0, "split-index", &split_index,
			N_("enable or disable split index")),
		OPT_BOOL(0, "untracked-cache", &untracked_cache,
			N_("enable/disable untracked cache")),
		OPT_SET_INT(0, "test-untracked-cache", &untracked_cache,
			    N_("test if the filesystem supports untracked cache"), UC_TEST),
		OPT_SET_INT(0, "force-untracked-cache", &untracked_cache,
			    N_("enable untracked cache without testing the filesystem"), UC_FORCE),
		OPT_SET_INT(0, "force-write-index", &force_write,
			N_("write out the index even if is not flagged as changed"), 1),
		OPT_BOOL(0, "fsmonitor", &fsmonitor,
			N_("enable or disable file system monitor")),
		{
			.type = OPTION_SET_INT,
			.long_name = "fsmonitor-valid",
			.मान = &mark_fsmonitor_only,
			.precision = परिमाण(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-fsmonitor-valid",
			.मान = &mark_fsmonitor_only,
			.precision = परिमाण(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_index_usage, options);

	repo_config(the_repository, git_default_config, रिक्त);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_full_index = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_index(the_repository, &lock_file, 0);
	यदि (newfd < 0)
		lock_error = त्रुटि_सं;

	entries = repo_read_index(the_repository);
	यदि (entries < 0)
		die("cache corrupted");

	the_repository->index->updated_skipworktree = 1;

	/*
	 * Custom copy of parse_options() because we want to handle
	 * filename arguments as they come.
	 */
	parse_options_start(&ctx, argc, argv, prefix,
			    options, PARSE_OPT_STOP_AT_NON_OPTION);

	/*
	 * Allow the object layer to optimize adding multiple objects in
	 * a batch.
	 */
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	जबसम्म (ctx.argc) {
		यदि (parseopt_state != PARSE_OPT_DONE)
			parseopt_state = parse_options_step(&ctx, options,
							    update_index_usage);
		यदि (!ctx.argc)
			रोक;
		विकल्प (parseopt_state) {
		पक्ष PARSE_OPT_HELP:
			निकास(0);
		पक्ष PARSE_OPT_HELP_ERROR:
		पक्ष PARSE_OPT_ERROR:
			निकास(129);
		पक्ष PARSE_OPT_COMPLETE:
			निकास(0);
		पक्ष PARSE_OPT_NON_OPTION:
		पक्ष PARSE_OPT_DONE:
		{
			अचर वर्ण *path = ctx.argv[0];
			वर्ण *p;

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
			यदि (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = रिक्त;
			}

			setup_work_रूख(the_repository);
			p = prefix_path(the_repository, prefix, prefix_length, path);
			update_one(p);
			यदि (set_executable_bit)
				chmod_path(set_executable_bit, p);
			मुक्त(p);
			ctx.argc--;
			ctx.argv++;
			रोक;
		}
		पक्ष PARSE_OPT_UNKNOWN:
			यदि (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			अन्यथा
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_index_usage, options);
		}
	}
	argc = parse_options_end(&ctx);

	getline_fn = nul_term_पङ्क्ति ? strbuf_getline_nul : strbuf_getline_lf;
	यदि (preferred_index_format) {
		यदि (preferred_index_format < 0) {
			म_लिखो(_("%d\n"), the_repository->index->version);
		} अन्यथा यदि (preferred_index_format < INDEX_FORMAT_LB ||
			   INDEX_FORMAT_UB < preferred_index_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_index_format,
			    INDEX_FORMAT_LB, INDEX_FORMAT_UB);
		} अन्यथा {
			यदि (the_repository->index->version != preferred_index_format)
				the_repository->index->cache_changed |= SOMETHING_CHANGED;
			report(_("index-version: was %d, set to %d"),
			       the_repository->index->version, preferred_index_format);
			the_repository->index->version = preferred_index_format;
		}
	}

	यदि (read_from_stdin) {
		संरचना strbuf buf = STRBUF_INIT;
		संरचना strbuf unquoted = STRBUF_INIT;

		setup_work_रूख(the_repository);
		जबसम्म (getline_fn(&buf, मानक_निवेश) != खातापूर्ण) {
			वर्ण *p;
			यदि (!nul_term_पङ्क्ति && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				यदि (unquote_c_style(&unquoted, buf.buf, रिक्त))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(the_repository, prefix, prefix_length, buf.buf);
			update_one(p);
			यदि (set_executable_bit)
				chmod_path(set_executable_bit, p);
			मुक्त(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	/*
	 * By now we have added all of the new objects
	 */
	odb_transaction_commit_and_finalize_or_die(transaction);

	यदि (split_index > 0) {
		यदि (repo_config_get_split_index(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		यदि (the_repository->index->split_index)
			the_repository->index->cache_changed |= SPLIT_INDEX_ORDERED;
		अन्यथा
			add_split_index(the_repository->index);
	} अन्यथा यदि (!split_index) {
		यदि (repo_config_get_split_index(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_split_index(the_repository->index);
	}

	prepare_repo_settings(r);
	विकल्प (untracked_cache) {
	पक्ष UC_UNSPECIFIED:
		रोक;
	पक्ष UC_DISABLE:
		यदि (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->index);
		report(_("Untracked cache disabled"));
		रोक;
	पक्ष UC_TEST:
		setup_work_रूख(the_repository);
		फर्काऊ !test_if_untracked_cache_is_supported();
	पक्ष UC_ENABLE:
	पक्ष UC_FORCE:
		यदि (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		add_untracked_cache(the_repository->index);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_रूख(the_repository));
		रोक;
	स्वभाव:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	यदि (fsmonitor > 0) {
		गणना fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		गणना fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		यदि (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_सन्देश(r, reason));

		यदि (fsm_mode == FSMONITOR_MODE_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		add_fsmonitor(the_repository->index);
		report(_("fsmonitor enabled"));
	} अन्यथा यदि (!fsmonitor) {
		गणना fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		यदि (fsm_mode > FSMONITOR_MODE_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->index);
		report(_("fsmonitor disabled"));
	}

	यदि (the_repository->index->cache_changed || force_write) {
		यदि (newfd < 0) {
			यदि (refresh_args.flags & REFRESH_QUIET)
				निकास(128);
			unable_to_lock_die(repo_get_index_file(the_repository), lock_error);
		}
		यदि (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die("Unable to write new index file");
	}

	rollback_lock_file(&lock_file);

	फर्काऊ has_errors ? 1 : 0;
}
