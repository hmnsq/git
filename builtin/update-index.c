/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hash.h"
#समावेश "hex.h"
#समावेश "lockfile.h"
#समावेश "quote.h"
#समावेश "cache-tree.h"
#समावेश "tree-walk.h"
#समावेश "object-file.h"
#समावेश "odb.h"
#समावेश "odb/transaction.h"
#समावेश "refs.h"
#समावेश "resolve-undo.h"
#समावेश "parse-options.h"
#समावेश "pathspec.h"
#समावेश "dir.h"
#समावेश "read-cache.h"
#समावेश "setup.h"
#समावेश "sparse-index.h"
#समावेश "split-index.h"
#समावेश "symlinks.h"
#समावेश "fsmonitor.h"
#समावेश "write-or-die.h"

/*
 * Default to not allowing changes to the list of files. The
 * tool doesn't actually care, but this makes it harder to add
 * files to the revision control by mistake by doing something
 * like "git update-index *" and suddenly having all the object
 * files be revision controlled.
 */
अटल पूर्णांक allow_add;
अटल पूर्णांक allow_remove;
अटल पूर्णांक allow_replace;
अटल पूर्णांक info_only;
अटल पूर्णांक force_remove;
अटल पूर्णांक verbose;
अटल पूर्णांक mark_valid_only;
अटल पूर्णांक mark_skip_worktree_only;
अटल पूर्णांक mark_fsmonitor_only;
अटल पूर्णांक ignore_skip_worktree_entries;
#घोषणा MARK_FLAG 1
#घोषणा UNMARK_FLAG 2
अटल काष्ठा strbuf mtime_dir = STRBUF_INIT;

/* Untracked cache mode */
क्रमागत uc_mode {
	UC_UNSPECIFIED = -1,
	UC_DISABLE = 0,
	UC_ENABLE,
	UC_TEST,
	UC_FORCE
};

__attribute__((format (म_लिखो, 1, 2)))
अटल व्योम report(स्थिर अक्षर *fmt, ...)
{
	बहु_सूची vp;

	अगर (!verbose)
		वापस;

	बहु_शुरू(vp, fmt);
	भ_लिखो(fmt, vp);
	अक्षर_दो('\n');
	बहु_पूर्ण(vp);
}

अटल व्योम remove_test_directory(व्योम)
{
	अगर (mtime_dir.len)
		remove_dir_recursively(&mtime_dir, 0);
}

अटल स्थिर अक्षर *get_mtime_path(स्थिर अक्षर *path)
{
	अटल काष्ठा strbuf sb = STRBUF_INIT;
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/%s", mtime_dir.buf, path);
	वापस sb.buf;
}

अटल व्योम xmkdir(स्थिर अक्षर *path)
{
	path = get_mtime_path(path);
	अगर (सूची_गढ़ो(path, 0700))
		die_errno(_("failed to create directory %s"), path);
}

अटल पूर्णांक xstat_mtime_dir(काष्ठा stat *st)
{
	अगर (stat(mtime_dir.buf, st))
		die_errno(_("failed to stat %s"), mtime_dir.buf);
	वापस 0;
}

अटल पूर्णांक create_file(स्थिर अक्षर *path)
{
	पूर्णांक fd;
	path = get_mtime_path(path);
	fd = xopen(path, O_CREAT | O_RDWR, 0644);
	वापस fd;
}

अटल व्योम xunlink(स्थिर अक्षर *path)
{
	path = get_mtime_path(path);
	अगर (unlink(path))
		die_errno(_("failed to delete file %s"), path);
}

अटल व्योम xrmdir(स्थिर अक्षर *path)
{
	path = get_mtime_path(path);
	अगर (सूची_हटाओ(path))
		die_errno(_("failed to delete directory %s"), path);
}

अटल व्योम avoid_racy(व्योम)
{
	/*
	 * not use if we could usleep(10) if core.useNanosec is enabled. The
	 * field nsec could be there, but the OS could choose to
	 * ignore it?
	 */
	sleep(1);
}

अटल पूर्णांक test_if_untracked_cache_is_supported(व्योम)
{
	काष्ठा stat st;
	काष्ठा stat_तथ्य base;
	पूर्णांक fd, ret = 0;
	अक्षर *cwd;

	strbuf_addstr(&mtime_dir, "mtime-test-XXXXXX");
	अगर (!mkdtemp(mtime_dir.buf))
		die_errno("Could not make temporary directory");

	cwd = xgetcwd();
	ख_लिखो(मानक_त्रुटि, _("Testing mtime in '%s' "), cwd);
	मुक्त(cwd);

	निकास_पर(remove_test_directory);
	xstat_mtime_dir(&st);
	fill_stat_तथ्य(&base, &st);
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	fd = create_file("newfile");
	xstat_mtime_dir(&st);
	अगर (!match_stat_तथ्य(&base, &st)) {
		बंद(fd);
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि,_("directory stat info does not "
				    "change after adding a new file"));
		जाओ done;
	}
	fill_stat_तथ्य(&base, &st);
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	xmkdir("new-dir");
	xstat_mtime_dir(&st);
	अगर (!match_stat_तथ्य(&base, &st)) {
		बंद(fd);
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि, _("directory stat info does not change "
				     "after adding a new directory"));
		जाओ done;
	}
	fill_stat_तथ्य(&base, &st);
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	write_or_die(fd, "data", 4);
	बंद(fd);
	xstat_mtime_dir(&st);
	अगर (match_stat_तथ्य(&base, &st)) {
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि, _("directory stat info changes "
				     "after updating a file"));
		जाओ done;
	}
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	बंद(create_file("new-dir/new"));
	xstat_mtime_dir(&st);
	अगर (match_stat_तथ्य(&base, &st)) {
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि, _("directory stat info changes after "
				     "adding a file inside subdirectory"));
		जाओ done;
	}
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	xunlink("newfile");
	xstat_mtime_dir(&st);
	अगर (!match_stat_तथ्य(&base, &st)) {
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि, _("directory stat info does not "
				     "change after deleting a file"));
		जाओ done;
	}
	fill_stat_तथ्य(&base, &st);
	ख_अक्षर_दो('.', मानक_त्रुटि);

	avoid_racy();
	xunlink("new-dir/new");
	xrmdir("new-dir");
	xstat_mtime_dir(&st);
	अगर (!match_stat_तथ्य(&base, &st)) {
		ख_अक्षर_दो('\n', मानक_त्रुटि);
		fprintf_ln(मानक_त्रुटि, _("directory stat info does not "
				     "change after deleting a directory"));
		जाओ done;
	}

	अगर (सूची_हटाओ(mtime_dir.buf))
		die_errno(_("failed to delete directory %s"), mtime_dir.buf);
	fprintf_ln(मानक_त्रुटि, _(" OK"));
	ret = 1;

done:
	strbuf_release(&mtime_dir);
	वापस ret;
}

अटल पूर्णांक mark_ce_flags(स्थिर अक्षर *path, पूर्णांक flag, पूर्णांक mark)
{
	पूर्णांक namelen = म_माप(path);
	पूर्णांक pos = सूचकांक_name_pos(the_repository->सूचकांक, path, namelen);
	अगर (0 <= pos) {
		mark_fsmonitor_invalid(the_repository->सूचकांक, the_repository->सूचकांक->cache[pos]);
		अगर (mark)
			the_repository->सूचकांक->cache[pos]->ce_flags |= flag;
		अन्यथा
			the_repository->सूचकांक->cache[pos]->ce_flags &= ~flag;
		the_repository->सूचकांक->cache[pos]->ce_flags |= CE_UPDATE_IN_BASE;
		cache_वृक्ष_invalidate_path(the_repository->सूचकांक, path);
		the_repository->सूचकांक->cache_changed |= CE_ENTRY_CHANGED;
		वापस 0;
	}
	वापस -1;
}

अटल पूर्णांक remove_one_path(स्थिर अक्षर *path)
{
	अगर (!allow_remove)
		वापस error("%s: does not exist and --remove not passed", path);
	अगर (remove_file_from_सूचकांक(the_repository->सूचकांक, path))
		वापस error("%s: cannot remove from the index", path);
	वापस 0;
}

/*
 * Handle a path that couldn't be lstat'ed. It's either:
 *  - missing file (ENOENT or ENOTDIR). That's ok if we're
 *    supposed to be removing it and the removal actually
 *    succeeds.
 *  - permission error. That's never ok.
 */
अटल पूर्णांक process_lstat_error(स्थिर अक्षर *path, पूर्णांक err)
{
	अगर (is_missing_file_error(err))
		वापस remove_one_path(path);
	वापस error("lstat(\"%s\"): %s", path, म_त्रुटि(err));
}

अटल पूर्णांक add_one_path(स्थिर काष्ठा cache_entry *old, स्थिर अक्षर *path, पूर्णांक len, काष्ठा stat *st)
{
	पूर्णांक option;
	काष्ठा cache_entry *ce;

	/* Was the old index entry already up-to-date? */
	अगर (old && !ce_stage(old) && !ie_match_stat(the_repository->सूचकांक, old, st, 0))
		वापस 0;

	ce = make_empty_cache_entry(the_repository->सूचकांक, len);
	स_नकल(ce->name, path, len);
	ce->ce_flags = create_ce_flags(0);
	ce->ce_namelen = len;
	fill_stat_cache_info(the_repository->सूचकांक, ce, st);
	ce->ce_mode = ce_mode_from_stat(the_repository, old, st->st_mode);

	अगर (सूचकांक_path(the_repository->सूचकांक, &ce->oid, path, st,
		       info_only ? 0 : सूचकांक_WRITE_OBJECT)) {
		discard_cache_entry(ce);
		वापस -1;
	}
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	अगर (add_सूचकांक_entry(the_repository->सूचकांक, ce, option)) {
		discard_cache_entry(ce);
		वापस error("%s: cannot add to the index - missing --add option?", path);
	}
	वापस 0;
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
अटल पूर्णांक process_directory(स्थिर अक्षर *path, पूर्णांक len, काष्ठा stat *st)
{
	काष्ठा object_id oid;
	पूर्णांक pos = सूचकांक_name_pos(the_repository->सूचकांक, path, len);

	/* Exact match: file or existing gitlink */
	अगर (pos >= 0) {
		स्थिर काष्ठा cache_entry *ce = the_repository->सूचकांक->cache[pos];
		अगर (S_ISGITLINK(ce->ce_mode)) {

			/* Do nothing to the index if there is no HEAD! */
			अगर (repo_resolve_gitlink_ref(the_repository, path,
						     "HEAD", &oid) < 0)
				वापस 0;

			वापस add_one_path(ce, path, len, st);
		}
		/* Should this be an unconditional error? */
		वापस remove_one_path(path);
	}

	/* Inexact match: is there perhaps a subdirectory match? */
	pos = -pos-1;
	जबतक (pos < the_repository->सूचकांक->cache_nr) {
		स्थिर काष्ठा cache_entry *ce = the_repository->सूचकांक->cache[pos++];

		अगर (म_भेदन(ce->name, path, len))
			अवरोध;
		अगर (ce->name[len] > '/')
			अवरोध;
		अगर (ce->name[len] < '/')
			जारी;

		/* Subdirectory match - error out */
		वापस error("%s: is a directory - add individual files instead", path);
	}

	/* No match - should we add it as a gitlink? */
	अगर (!repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid))
		वापस add_one_path(शून्य, path, len, st);

	/* Error out. */
	वापस error("%s: is a directory - add files inside instead", path);
}

अटल पूर्णांक process_path(स्थिर अक्षर *path, काष्ठा stat *st, पूर्णांक stat_errno)
{
	पूर्णांक pos, len;
	स्थिर काष्ठा cache_entry *ce;

	len = म_माप(path);
	अगर (has_symlink_leading_path(path, len))
		वापस error("'%s' is beyond a symbolic link", path);

	pos = सूचकांक_name_pos(the_repository->सूचकांक, path, len);
	ce = pos < 0 ? शून्य : the_repository->सूचकांक->cache[pos];
	अगर (ce && ce_skip_worktree(ce)) {
		/*
		 * working directory version is assumed "good"
		 * so updating it does not make sense.
		 * On the other hand, removing it from index should work
		 */
		अगर (!ignore_skip_worktree_entries && allow_remove &&
		    remove_file_from_सूचकांक(the_repository->सूचकांक, path))
			वापस error("%s: cannot remove from the index", path);
		वापस 0;
	}

	/*
	 * First things first: get the stat information, to decide
	 * what to do about the pathname!
	 */
	अगर (stat_errno)
		वापस process_lstat_error(path, stat_errno);

	अगर (S_ISDIR(st->st_mode))
		वापस process_directory(path, len, st);

	वापस add_one_path(ce, path, len, st);
}

अटल पूर्णांक add_cacheinfo(अचिन्हित पूर्णांक mode, स्थिर काष्ठा object_id *oid,
			 स्थिर अक्षर *path, पूर्णांक stage)
{
	पूर्णांक len, option;
	काष्ठा cache_entry *ce;

	अगर (!verify_path(path, mode))
		वापस error("Invalid path '%s'", path);

	len = म_माप(path);
	ce = make_empty_cache_entry(the_repository->सूचकांक, len);

	oidcpy(&ce->oid, oid);
	स_नकल(ce->name, path, len);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = len;
	ce->ce_mode = create_ce_mode(mode);
	अगर (assume_unchanged)
		ce->ce_flags |= CE_VALID;
	option = allow_add ? ADD_CACHE_OK_TO_ADD : 0;
	option |= allow_replace ? ADD_CACHE_OK_TO_REPLACE : 0;
	अगर (add_सूचकांक_entry(the_repository->सूचकांक, ce, option))
		वापस error("%s: cannot add to the index - missing --add option?",
			     path);
	report("add '%s'", path);
	वापस 0;
}

अटल व्योम chmod_path(अक्षर flip, स्थिर अक्षर *path)
{
	पूर्णांक pos;
	काष्ठा cache_entry *ce;

	pos = सूचकांक_name_pos(the_repository->सूचकांक, path, म_माप(path));
	अगर (pos < 0)
		जाओ fail;
	ce = the_repository->सूचकांक->cache[pos];
	अगर (chmod_सूचकांक_entry(the_repository->सूचकांक, ce, flip) < 0)
		जाओ fail;

	report("chmod %cx '%s'", flip, path);
	वापस;
 fail:
	die("git update-index: cannot chmod %cx '%s'", flip, path);
}

अटल व्योम update_one(स्थिर अक्षर *path)
{
	पूर्णांक stat_errno = 0;
	काष्ठा stat st;

	अगर (mark_valid_only || mark_skip_worktree_only || force_remove ||
	    mark_fsmonitor_only)
		st.st_mode = 0;
	अन्यथा अगर (lstat(path, &st) < 0) {
		st.st_mode = 0;
		stat_errno = त्रुटि_सं;
	} /* else stat is valid */

	अगर (!verify_path(path, st.st_mode)) {
		ख_लिखो(मानक_त्रुटि, "Ignoring path %s\n", path);
		वापस;
	}
	अगर (mark_valid_only) {
		अगर (mark_ce_flags(path, CE_VALID, mark_valid_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		वापस;
	}
	अगर (mark_skip_worktree_only) {
		अगर (mark_ce_flags(path, CE_SKIP_WORKTREE, mark_skip_worktree_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		वापस;
	}
	अगर (mark_fsmonitor_only) {
		अगर (mark_ce_flags(path, CE_FSMONITOR_VALID, mark_fsmonitor_only == MARK_FLAG))
			die("Unable to mark file %s", path);
		वापस;
	}

	अगर (force_remove) {
		अगर (remove_file_from_सूचकांक(the_repository->सूचकांक, path))
			die("git update-index: unable to remove %s", path);
		report("remove '%s'", path);
		वापस;
	}
	अगर (process_path(path, &st, stat_errno))
		die("Unable to process path %s", path);
	report("add '%s'", path);
}

अटल व्योम read_सूचकांक_info(पूर्णांक nul_term_पंक्ति)
{
	स्थिर पूर्णांक hexsz = the_hash_algo->hexsz;
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा strbuf uq = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_पंक्ति ? strbuf_getline_nul : strbuf_getline_lf;
	जबतक (getline_fn(&buf, मानक_निवेश) != खातापूर्ण) {
		अक्षर *ptr, *tab;
		अक्षर *path_name;
		काष्ठा object_id oid;
		अचिन्हित पूर्णांक mode;
		अचिन्हित दीर्घ ul;
		पूर्णांक stage;

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
		अगर (ptr == buf.buf || *ptr != ' '
		    || त्रुटि_सं || (अचिन्हित पूर्णांक) ul != ul)
			जाओ bad_पंक्ति;
		mode = ul;

		tab = म_अक्षर(ptr, '\t');
		अगर (!tab || tab - ptr < hexsz + 1)
			जाओ bad_पंक्ति;

		अगर (tab[-2] == ' ' && '0' <= tab[-1] && tab[-1] <= '3') {
			stage = tab[-1] - '0';
			ptr = tab + 1; /* point at the head of path */
			tab = tab - 2; /* point at tail of sha1 */
		}
		अन्यथा {
			stage = 0;
			ptr = tab + 1; /* point at the head of path */
		}

		अगर (get_oid_hex(tab - hexsz, &oid) ||
			tab[-(hexsz + 1)] != ' ')
			जाओ bad_पंक्ति;

		path_name = ptr;
		अगर (!nul_term_पंक्ति && path_name[0] == '"') {
			strbuf_reset(&uq);
			अगर (unquote_c_style(&uq, path_name, शून्य)) {
				die("git update-index: bad quoting of path name");
			}
			path_name = uq.buf;
		}

		अगर (!verify_path(path_name, mode)) {
			ख_लिखो(मानक_त्रुटि, "Ignoring path %s\n", path_name);
			जारी;
		}

		अगर (!mode) {
			/* mode == 0 means there is no such path -- remove */
			अगर (remove_file_from_सूचकांक(the_repository->सूचकांक, path_name))
				die("git update-index: unable to remove %s",
				    ptr);
		}
		अन्यथा {
			/* mode ' ' sha1 '\t' name
			 * ptr[-1] points at tab,
			 * ptr[-41] is at the beginning of sha1
			 */
			ptr[-(hexsz + 2)] = ptr[-1] = 0;
			अगर (add_cacheinfo(mode, &oid, path_name, stage))
				die("git update-index: unable to update %s",
				    path_name);
		}
		जारी;

	bad_पंक्ति:
		die("malformed index info %s", buf.buf);
	}
	strbuf_release(&buf);
	strbuf_release(&uq);
}

अटल स्थिर अक्षर * स्थिर update_सूचकांक_usage[] = {
	N_("git update-index [<options>] [--] [<file>...]"),
	शून्य
};

अटल काष्ठा cache_entry *read_one_ent(स्थिर अक्षर *which,
					काष्ठा object_id *ent, स्थिर अक्षर *path,
					पूर्णांक namelen, पूर्णांक stage)
{
	अचिन्हित लघु mode;
	काष्ठा object_id oid;
	काष्ठा cache_entry *ce;

	अगर (get_वृक्ष_entry(the_repository, ent, path, &oid, &mode)) {
		अगर (which)
			error("%s: not in %s branch.", path, which);
		वापस शून्य;
	}
	अगर (!the_repository->सूचकांक->sparse_सूचकांक && mode == S_IFDIR) {
		अगर (which)
			error("%s: not a blob in %s branch.", path, which);
		वापस शून्य;
	}
	ce = make_empty_cache_entry(the_repository->सूचकांक, namelen);

	oidcpy(&ce->oid, &oid);
	स_नकल(ce->name, path, namelen);
	ce->ce_flags = create_ce_flags(stage);
	ce->ce_namelen = namelen;
	ce->ce_mode = create_ce_mode(mode);
	वापस ce;
}

अटल पूर्णांक unresolve_one(स्थिर अक्षर *path)
{
	काष्ठा string_list_item *item;
	पूर्णांक res = 0;

	अगर (!the_repository->सूचकांक->resolve_undo)
		वापस res;
	item = string_list_lookup(the_repository->सूचकांक->resolve_undo, path);
	अगर (!item)
		वापस res; /* no resolve-undo record for the path */
	res = unmerge_सूचकांक_entry(the_repository->सूचकांक, path, item->util, 0);
	FREE_AND_NULL(item->util);
	वापस res;
}

अटल पूर्णांक do_unresolve(पूर्णांक ac, स्थिर अक्षर **av,
			स्थिर अक्षर *prefix, पूर्णांक prefix_लंबाई)
{
	पूर्णांक i;
	पूर्णांक err = 0;

	क्रम (i = 1; i < ac; i++) {
		स्थिर अक्षर *arg = av[i];
		अक्षर *p = prefix_path(the_repository, prefix, prefix_लंबाई, arg);
		err |= unresolve_one(p);
		मुक्त(p);
	}
	वापस err;
}

अटल पूर्णांक do_reupdate(स्थिर अक्षर **paths,
		       स्थिर अक्षर *prefix)
{
	/* Read HEAD and run update-index on paths that are
	 * merged and already different between index and HEAD.
	 */
	पूर्णांक pos;
	पूर्णांक has_शीर्ष = 1;
	काष्ठा pathspec pathspec;
	काष्ठा object_id शीर्ष_oid;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, paths);

	अगर (refs_read_ref(get_main_ref_store(the_repository), "HEAD", &शीर्ष_oid))
		/* If there is no HEAD, that means it is an initial
		 * commit.  Update everything in the index.
		 */
		has_शीर्ष = 0;
 redo:
	क्रम (pos = 0; pos < the_repository->सूचकांक->cache_nr; pos++) {
		स्थिर काष्ठा cache_entry *ce = the_repository->सूचकांक->cache[pos];
		काष्ठा cache_entry *old = शून्य;
		पूर्णांक save_nr;
		अक्षर *path;

		अगर (ce_stage(ce) || !ce_path_match(the_repository->सूचकांक, ce, &pathspec, शून्य))
			जारी;
		अगर (has_शीर्ष)
			old = read_one_ent(शून्य, &शीर्ष_oid,
					   ce->name, ce_namelen(ce), 0);
		अगर (old && ce->ce_mode == old->ce_mode &&
		    oideq(&ce->oid, &old->oid)) {
			discard_cache_entry(old);
			जारी; /* unchanged */
		}

		/* At this point, we know the contents of the sparse directory are
		 * modified with respect to HEAD, so we expand the index and restart
		 * to process each path individually
		 */
		अगर (S_ISSPARSEDIR(ce->ce_mode)) {
			ensure_full_सूचकांक(the_repository->सूचकांक);
			जाओ redo;
		}

		/* Be careful.  The working tree may not have the
		 * path anymore, in which case, under 'allow_remove',
		 * or worse yet 'allow_replace', active_nr may decrease.
		 */
		save_nr = the_repository->सूचकांक->cache_nr;
		path = xstrdup(ce->name);
		update_one(path);
		मुक्त(path);
		discard_cache_entry(old);
		अगर (save_nr != the_repository->सूचकांक->cache_nr)
			जाओ redo;
	}
	clear_pathspec(&pathspec);
	वापस 0;
}

काष्ठा refresh_params {
	अचिन्हित पूर्णांक flags;
	पूर्णांक *has_errors;
};

अटल पूर्णांक refresh(काष्ठा refresh_params *o, अचिन्हित पूर्णांक flag)
{
	setup_work_वृक्ष(the_repository);
	repo_read_सूचकांक(the_repository);
	*o->has_errors |= refresh_सूचकांक(the_repository->सूचकांक, o->flags | flag, शून्य,
					शून्य, शून्य);
	अगर (has_racy_timestamp(the_repository->सूचकांक)) {
		/*
		 * Even if nothing else has changed, updating the file
		 * increases the chance that racy timestamps become
		 * non-racy, helping future run-time performance.
		 * We do that even in case of "errors" returned by
		 * refresh_index() as these are no actual errors.
		 * cmd_status() does the same.
		 */
		the_repository->सूचकांक->cache_changed |= SOMETHING_CHANGED;
	}
	वापस 0;
}

अटल पूर्णांक refresh_callback(स्थिर काष्ठा option *opt,
				स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	वापस refresh(opt->मूल्य, 0);
}

अटल पूर्णांक really_refresh_callback(स्थिर काष्ठा option *opt,
				स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	वापस refresh(opt->मूल्य, REFRESH_REALLY);
}

अटल पूर्णांक chmod_callback(स्थिर काष्ठा option *opt,
				स्थिर अक्षर *arg, पूर्णांक unset)
{
	अक्षर *flip = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	अगर ((arg[0] != '-' && arg[0] != '+') || arg[1] != 'x' || arg[2])
		वापस error("option 'chmod' expects \"+x\" or \"-x\"");
	*flip = arg[0];
	वापस 0;
}

अटल पूर्णांक resolve_undo_clear_callback(स्थिर काष्ठा option *opt UNUSED,
				स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	resolve_undo_clear_सूचकांक(the_repository->सूचकांक);
	वापस 0;
}

अटल पूर्णांक parse_new_style_cacheinfo(स्थिर अक्षर *arg,
				     अचिन्हित पूर्णांक *mode,
				     काष्ठा object_id *oid,
				     स्थिर अक्षर **path)
{
	अचिन्हित दीर्घ ul;
	अक्षर *endp;
	स्थिर अक्षर *p;

	अगर (!arg)
		वापस -1;

	त्रुटि_सं = 0;
	ul = म_से_अदीर्घ(arg, &endp, 8);
	अगर (त्रुटि_सं || endp == arg || *endp != ',' || (अचिन्हित पूर्णांक) ul != ul)
		वापस -1; /* not a new-style cacheinfo */
	*mode = ul;
	endp++;
	अगर (parse_oid_hex(endp, oid, &p) || *p != ',')
		वापस -1;
	*path = p + 1;
	वापस 0;
}

अटल क्रमागत parse_opt_परिणाम cacheinfo_callback(
	काष्ठा parse_opt_ctx_t *ctx, स्थिर काष्ठा option *opt UNUSED,
	स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा object_id oid;
	अचिन्हित पूर्णांक mode;
	स्थिर अक्षर *path;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	अगर (!parse_new_style_cacheinfo(ctx->argv[1], &mode, &oid, &path)) {
		अगर (add_cacheinfo(mode, &oid, path, 0))
			die("git update-index: --cacheinfo cannot add %s", path);
		ctx->argv++;
		ctx->argc--;
		वापस 0;
	}
	अगर (ctx->argc <= 3)
		वापस error("option 'cacheinfo' expects <mode>,<sha1>,<path>");
	अगर (strtoul_ui(*++ctx->argv, 8, &mode) ||
	    get_oid_hex(*++ctx->argv, &oid) ||
	    add_cacheinfo(mode, &oid, *++ctx->argv, 0))
		die("git update-index: --cacheinfo cannot add %s", *ctx->argv);
	ctx->argc -= 3;
	वापस 0;
}

अटल क्रमागत parse_opt_परिणाम stdin_cacheinfo_callback(
	काष्ठा parse_opt_ctx_t *ctx, स्थिर काष्ठा option *opt,
	स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *nul_term_पंक्ति = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	अगर (ctx->argc != 1)
		वापस error("option '%s' must be the last argument", opt->long_name);
	allow_add = allow_replace = allow_remove = 1;
	read_सूचकांक_info(*nul_term_पंक्ति);
	वापस 0;
}

अटल क्रमागत parse_opt_परिणाम stdin_callback(
	काष्ठा parse_opt_ctx_t *ctx, स्थिर काष्ठा option *opt,
	स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *read_from_stdin = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	अगर (ctx->argc != 1)
		वापस error("option '%s' must be the last argument", opt->long_name);
	*read_from_stdin = 1;
	वापस 0;
}

अटल क्रमागत parse_opt_परिणाम unresolve_callback(
	काष्ठा parse_opt_ctx_t *ctx, स्थिर काष्ठा option *opt,
	स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *has_errors = opt->मूल्य;
	स्थिर अक्षर *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	*has_errors = do_unresolve(ctx->argc, ctx->argv,
				prefix, prefix ? म_माप(prefix) : 0);
	अगर (*has_errors)
		the_repository->सूचकांक->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	वापस 0;
}

अटल क्रमागत parse_opt_परिणाम reupdate_callback(
	काष्ठा parse_opt_ctx_t *ctx, स्थिर काष्ठा option *opt,
	स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *has_errors = opt->मूल्य;
	स्थिर अक्षर *prefix = the_repository->prefix;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	/* consume remaining arguments. */
	setup_work_वृक्ष(the_repository);
	*has_errors = do_reupdate(ctx->argv + 1, prefix);
	अगर (*has_errors)
		the_repository->सूचकांक->cache_changed = 0;

	ctx->argv += ctx->argc - 1;
	ctx->argc = 1;
	वापस 0;
}

पूर्णांक cmd_update_सूचकांक(पूर्णांक argc,
		     स्थिर अक्षर **argv,
		     स्थिर अक्षर *prefix,
		     काष्ठा repository *repo UNUSED)
{
	पूर्णांक newfd, entries, has_errors = 0, nul_term_पंक्ति = 0;
	क्रमागत uc_mode untracked_cache = UC_UNSPECIFIED;
	पूर्णांक read_from_stdin = 0;
	पूर्णांक prefix_लंबाई = prefix ? म_माप(prefix) : 0;
	पूर्णांक preferred_सूचकांक_format = 0;
	अक्षर set_executable_bit = 0;
	काष्ठा refresh_params refresh_args = {0, &has_errors};
	पूर्णांक lock_error = 0;
	पूर्णांक split_सूचकांक = -1;
	पूर्णांक force_write = 0;
	पूर्णांक fsmonitor = -1;
	काष्ठा lock_file lock_file = LOCK_INIT;
	काष्ठा parse_opt_ctx_t ctx;
	strbuf_getline_fn getline_fn;
	पूर्णांक parseopt_state = PARSE_OPT_UNKNOWN;
	काष्ठा repository *r = the_repository;
	काष्ठा odb_transaction *transaction;
	काष्ठा option options[] = {
		OPT_BIT('q', शून्य, &refresh_args.flags,
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
		OPT_CALLBACK_F(0, "refresh", &refresh_args, शून्य,
			N_("refresh stat information"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			refresh_callback),
		OPT_CALLBACK_F(0, "really-refresh", &refresh_args, शून्य,
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
			.मूल्य = &mark_valid_only,
			.precision = माप(mark_valid_only),
			.help = N_("mark files as \"not changing\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-assume-unchanged",
			.मूल्य = &mark_valid_only,
			.precision = माप(mark_valid_only),
			.help = N_("clear assumed-unchanged bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "skip-worktree",
			.मूल्य = &mark_skip_worktree_only,
			.precision = माप(mark_skip_worktree_only),
			.help = N_("mark files as \"index-only\""),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-skip-worktree",
			.मूल्य = &mark_skip_worktree_only,
			.precision = माप(mark_skip_worktree_only),
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
		OPT_BOOL('z', शून्य, &nul_term_पंक्ति,
			 N_("with --stdin: input lines are terminated by null bytes")),
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "stdin",
			.मूल्य = &read_from_stdin,
			.help = N_("read list of paths to be updated from standard input"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "index-info",
			.मूल्य = &nul_term_पंक्ति,
			.help = N_("add entries from standard input to the index"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = stdin_cacheinfo_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.long_name = "unresolve",
			.मूल्य = &has_errors,
			.help = N_("repopulate stages #2 and #3 for the listed paths"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = unresolve_callback,
		},
		{
			.type = OPTION_LOWLEVEL_CALLBACK,
			.short_name = 'g',
			.long_name = "again",
			.मूल्य = &has_errors,
			.help = N_("only update entries that differ from HEAD"),
			.flags = PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			.ll_callback = reupdate_callback,
		},
		OPT_BIT(0, "ignore-missing", &refresh_args.flags,
			N_("ignore files missing from worktree"),
			REFRESH_IGNORE_MISSING),
		OPT_SET_INT(0, "verbose", &verbose,
			N_("report actions to standard output"), 1),
		OPT_CALLBACK_F(0, "clear-resolve-undo", शून्य, शून्य,
			N_("(for porcelains) forget saved unresolved conflicts"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			resolve_undo_clear_callback),
		OPT_INTEGER(0, "index-version", &preferred_सूचकांक_format,
			N_("write index in this format")),
		OPT_SET_INT(0, "show-index-version", &preferred_सूचकांक_format,
			    N_("report on-disk index format version"), -1),
		OPT_BOOL(0, "split-index", &split_सूचकांक,
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
			.मूल्य = &mark_fsmonitor_only,
			.precision = माप(mark_fsmonitor_only),
			.help = N_("mark files as fsmonitor valid"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = MARK_FLAG,
		},
		{
			.type = OPTION_SET_INT,
			.long_name = "no-fsmonitor-valid",
			.मूल्य = &mark_fsmonitor_only,
			.precision = माप(mark_fsmonitor_only),
			.help = N_("clear fsmonitor valid bit"),
			.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			.defval = UNMARK_FLAG,
		},
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 update_सूचकांक_usage, options);

	repo_config(the_repository, git_default_config, शून्य);

	prepare_repo_settings(r);
	the_repository->settings.command_requires_full_सूचकांक = 0;

	/* we will diagnose later if it turns out that we need to update it */
	newfd = repo_hold_locked_सूचकांक(the_repository, &lock_file, 0);
	अगर (newfd < 0)
		lock_error = त्रुटि_सं;

	entries = repo_read_सूचकांक(the_repository);
	अगर (entries < 0)
		die("cache corrupted");

	the_repository->सूचकांक->updated_skipworktree = 1;

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
	जबतक (ctx.argc) {
		अगर (parseopt_state != PARSE_OPT_DONE)
			parseopt_state = parse_options_step(&ctx, options,
							    update_सूचकांक_usage);
		अगर (!ctx.argc)
			अवरोध;
		चयन (parseopt_state) {
		हाल PARSE_OPT_HELP:
			निकास(0);
		हाल PARSE_OPT_HELP_ERROR:
		हाल PARSE_OPT_ERROR:
			निकास(129);
		हाल PARSE_OPT_COMPLETE:
			निकास(0);
		हाल PARSE_OPT_NON_OPTION:
		हाल PARSE_OPT_DONE:
		{
			स्थिर अक्षर *path = ctx.argv[0];
			अक्षर *p;

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
			अगर (transaction && verbose) {
				odb_transaction_commit_and_finalize_or_die(transaction);
				transaction = शून्य;
			}

			setup_work_वृक्ष(the_repository);
			p = prefix_path(the_repository, prefix, prefix_लंबाई, path);
			update_one(p);
			अगर (set_executable_bit)
				chmod_path(set_executable_bit, p);
			मुक्त(p);
			ctx.argc--;
			ctx.argv++;
			अवरोध;
		}
		हाल PARSE_OPT_UNKNOWN:
			अगर (ctx.argv[0][1] == '-')
				error("unknown option '%s'", ctx.argv[0] + 2);
			अन्यथा
				error("unknown switch '%c'", *ctx.opt);
			usage_with_options(update_सूचकांक_usage, options);
		}
	}
	argc = parse_options_end(&ctx);

	getline_fn = nul_term_पंक्ति ? strbuf_getline_nul : strbuf_getline_lf;
	अगर (preferred_सूचकांक_format) {
		अगर (preferred_सूचकांक_format < 0) {
			म_लिखो(_("%d\n"), the_repository->सूचकांक->version);
		} अन्यथा अगर (preferred_सूचकांक_format < सूचकांक_FORMAT_LB ||
			   सूचकांक_FORMAT_UB < preferred_सूचकांक_format) {
			die("index-version %d not in range: %d..%d",
			    preferred_सूचकांक_format,
			    सूचकांक_FORMAT_LB, सूचकांक_FORMAT_UB);
		} अन्यथा {
			अगर (the_repository->सूचकांक->version != preferred_सूचकांक_format)
				the_repository->सूचकांक->cache_changed |= SOMETHING_CHANGED;
			report(_("index-version: was %d, set to %d"),
			       the_repository->सूचकांक->version, preferred_सूचकांक_format);
			the_repository->सूचकांक->version = preferred_सूचकांक_format;
		}
	}

	अगर (read_from_stdin) {
		काष्ठा strbuf buf = STRBUF_INIT;
		काष्ठा strbuf unquoted = STRBUF_INIT;

		setup_work_वृक्ष(the_repository);
		जबतक (getline_fn(&buf, मानक_निवेश) != खातापूर्ण) {
			अक्षर *p;
			अगर (!nul_term_पंक्ति && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				अगर (unquote_c_style(&unquoted, buf.buf, शून्य))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(the_repository, prefix, prefix_लंबाई, buf.buf);
			update_one(p);
			अगर (set_executable_bit)
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

	अगर (split_सूचकांक > 0) {
		अगर (repo_config_get_split_सूचकांक(the_repository) == 0)
			warning(_("core.splitIndex is set to false; "
				  "remove or change it, if you really want to "
				  "enable split index"));
		अगर (the_repository->सूचकांक->split_सूचकांक)
			the_repository->सूचकांक->cache_changed |= SPLIT_सूचकांक_ORDERED;
		अन्यथा
			add_split_सूचकांक(the_repository->सूचकांक);
	} अन्यथा अगर (!split_सूचकांक) {
		अगर (repo_config_get_split_सूचकांक(the_repository) == 1)
			warning(_("core.splitIndex is set to true; "
				  "remove or change it, if you really want to "
				  "disable split index"));
		remove_split_सूचकांक(the_repository->सूचकांक);
	}

	prepare_repo_settings(r);
	चयन (untracked_cache) {
	हाल UC_UNSPECIFIED:
		अवरोध;
	हाल UC_DISABLE:
		अगर (r->settings.core_untracked_cache == UNTRACKED_CACHE_WRITE)
			warning(_("core.untrackedCache is set to true; "
				  "remove or change it, if you really want to "
				  "disable the untracked cache"));
		remove_untracked_cache(the_repository->सूचकांक);
		report(_("Untracked cache disabled"));
		अवरोध;
	हाल UC_TEST:
		setup_work_वृक्ष(the_repository);
		वापस !test_if_untracked_cache_is_supported();
	हाल UC_ENABLE:
	हाल UC_FORCE:
		अगर (r->settings.core_untracked_cache == UNTRACKED_CACHE_REMOVE)
			warning(_("core.untrackedCache is set to false; "
				  "remove or change it, if you really want to "
				  "enable the untracked cache"));
		add_untracked_cache(the_repository->सूचकांक);
		report(_("Untracked cache enabled for '%s'"), repo_get_work_वृक्ष(the_repository));
		अवरोध;
	शेष:
		BUG("bad untracked_cache value: %d", untracked_cache);
	}

	अगर (fsmonitor > 0) {
		क्रमागत fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		क्रमागत fsmonitor_reason reason = fsm_settings__get_reason(r);

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
		अगर (reason > FSMONITOR_REASON_OK)
			die("%s",
			    fsm_settings_get_incompatible_संदेश(r, reason));

		अगर (fsm_mode == FSMONITOR_MODE_DISABLED) {
			warning(_("core.fsmonitor is unset; "
				"set it if you really want to "
				"enable fsmonitor"));
		}
		add_fsmonitor(the_repository->सूचकांक);
		report(_("fsmonitor enabled"));
	} अन्यथा अगर (!fsmonitor) {
		क्रमागत fsmonitor_mode fsm_mode = fsm_settings__get_mode(r);
		अगर (fsm_mode > FSMONITOR_MODE_DISABLED)
			warning(_("core.fsmonitor is set; "
				"remove it if you really want to "
				"disable fsmonitor"));
		remove_fsmonitor(the_repository->सूचकांक);
		report(_("fsmonitor disabled"));
	}

	अगर (the_repository->सूचकांक->cache_changed || force_write) {
		अगर (newfd < 0) {
			अगर (refresh_args.flags & REFRESH_QUIET)
				निकास(128);
			unable_to_lock_die(repo_get_सूचकांक_file(the_repository), lock_error);
		}
		अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock_file, COMMIT_LOCK))
			die("Unable to write new index file");
	}

	rollback_lock_file(&lock_file);

	वापस has_errors ? 1 : 0;
}
