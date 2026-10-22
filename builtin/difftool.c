/*
 * "git difftool" builtin command
 *
 * This is a wrapper around the GIT_EXTERNAL_DIFF-compatible
 * git-difftool--helper script.
 *
 * This script exports GIT_EXTERNAL_DIFF and GIT_PAGER for use by git.
 * The GIT_DIFF* variables are exported for use by git-difftool--helper.
 *
 * Any arguments that are unknown to this script are forwarded to 'git diff'.
 *
 * Copyright (C) 2016 Johannes Schindelin
 */

#অন্তর্ভাব "builtin.h"

#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "copy.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "repository.h"
#অন্তর্ভাব "sparse-index.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "entry.h"
#অন্তর্ভাব "setup.h"

স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	ফাঁকা
};

কাঠামো difftool_options {
	পূর্ণসংখ্যা has_symlinks;
	পূর্ণসংখ্যা symlinks;
	পূর্ণসংখ্যা trust_exit_code;
};

স্থির পূর্ণসংখ্যা difftool_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			   ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	কাঠামো difftool_options *dt_options = (কাঠামো difftool_options *)cb;
	যদি (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_code = git_config_bool(var, মান);
		ফেরত 0;
	}
	যদি (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, মান);
		ফেরত 0;
	}

	ফেরত git_default_config(var, মান, ctx, cb);
}

স্থির পূর্ণসংখ্যা print_tool_help(শূন্য)
{
	কাঠামো child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "mergetool", "--tool-help=diff", ফাঁকা);
	ফেরত run_command(&cmd);
}

স্থির পূর্ণসংখ্যা parse_সূচক_info(কাঠামো repository *repo,
			    অক্ষর *p, পূর্ণসংখ্যা *mode1, পূর্ণসংখ্যা *mode2,
			    কাঠামো object_id *oid1, কাঠামো object_id *oid2,
			    অক্ষর *status)
{
	যদি (*p != ':')
		ফেরত error("expected ':', got '%c'", *p);
	*mode1 = (পূর্ণসংখ্যা)strtol(p + 1, &p, 8);
	যদি (*p != ' ')
		ফেরত error("expected ' ', got '%c'", *p);
	*mode2 = (পূর্ণসংখ্যা)strtol(p + 1, &p, 8);
	যদি (*p != ' ')
		ফেরত error("expected ' ', got '%c'", *p);
	যদি (parse_oid_hex_algop(++p, oid1, (ধ্রুবক অক্ষর **)&p, repo->hash_algo))
		ফেরত error("expected object ID, got '%s'", p);
	যদি (*p != ' ')
		ফেরত error("expected ' ', got '%c'", *p);
	যদি (parse_oid_hex_algop(++p, oid2, (ধ্রুবক অক্ষর **)&p, repo->hash_algo))
		ফেরত error("expected object ID, got '%s'", p);
	যদি (*p != ' ')
		ফেরত error("expected ' ', got '%c'", *p);
	*status = *++p;
	যদি (!*status)
		ফেরত error("missing status");
	যদি (p[1] && !isdigit(p[1]))
		ফেরত error("unexpected trailer: '%s'", p + 1);
	ফেরত 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
স্থির শূন্য add_path(কাঠামো strbuf *buf, আকার_t base_len, ধ্রুবক অক্ষর *path)
{
	strbuf_setlen(buf, base_len);
	যদি (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, path);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
স্থির পূর্ণসংখ্যা use_wt_file(কাঠামো repository *repo,
		       ধ্রুবক অক্ষর *workdir, ধ্রুবক অক্ষর *name,
		       কাঠামো object_id *oid)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো stat st;
	পূর্ণসংখ্যা use = 0;

	strbuf_addstr(&buf, workdir);
	add_path(&buf, buf.len, name);

	যদি (!lstat(buf.buf, &st) && !S_ISLNK(st.st_mode)) {
		কাঠামো object_id wt_oid;
		পূর্ণসংখ্যা fd = open(buf.buf, O_RDONLY);

		যদি (fd >= 0 &&
		    !সূচক_fd(repo->সূচক, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			যদি (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} অন্যথা যদি (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	ফেরত use;
}

কাঠামো working_বৃক্ষ_entry {
	কাঠামো hashmap_entry entry;
	অক্ষর path[FLEX_ARRAY];
};

স্থির পূর্ণসংখ্যা working_বৃক্ষ_entry_cmp(ধ্রুবক শূন্য *cmp_তথ্য UNUSED,
				  ধ্রুবক কাঠামো hashmap_entry *eptr,
				  ধ্রুবক কাঠামো hashmap_entry *entry_or_চাবি,
				  ধ্রুবক শূন্য *keydata UNUSED)
{
	ধ্রুবক কাঠামো working_বৃক্ষ_entry *a, *b;

	a = container_of(eptr, ধ্রুবক কাঠামো working_বৃক্ষ_entry, entry);
	b = container_of(entry_or_চাবি, ধ্রুবক কাঠামো working_বৃক্ষ_entry, entry);

	ফেরত strcmp(a->path, b->path);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
কাঠামো pair_entry {
	কাঠামো hashmap_entry entry;
	অক্ষর বাম[PATH_MAX], ডান[PATH_MAX];
	ধ্রুবক অক্ষর path[FLEX_ARRAY];
};

স্থির পূর্ণসংখ্যা pair_cmp(ধ্রুবক শূন্য *cmp_তথ্য UNUSED,
		    ধ্রুবক কাঠামো hashmap_entry *eptr,
		    ধ্রুবক কাঠামো hashmap_entry *entry_or_চাবি,
		    ধ্রুবক শূন্য *keydata UNUSED)
{
	ধ্রুবক কাঠামো pair_entry *a, *b;

	a = container_of(eptr, ধ্রুবক কাঠামো pair_entry, entry);
	b = container_of(entry_or_চাবি, ধ্রুবক কাঠামো pair_entry, entry);

	ফেরত strcmp(a->path, b->path);
}

স্থির শূন্য add_বাম_or_ডান(কাঠামো hashmap *map, ধ্রুবক অক্ষর *path,
			      ধ্রুবক অক্ষর *content, পূর্ণসংখ্যা is_ডান)
{
	কাঠামো pair_entry *e, *existing;

	FLEX_ALLOC_STR(e, path, path);
	hashmap_entry_init(&e->entry, strhash(path));
	existing = hashmap_get_entry(map, e, entry, ফাঁকা);
	যদি (existing) {
		মুক্ত(e);
		e = existing;
	} অন্যথা {
		e->বাম[0] = e->ডান[0] = '\0';
		hashmap_add(map, &e->entry);
	}
	strlcpy(is_ডান ? e->ডান : e->বাম, content, PATH_MAX);
}

কাঠামো path_entry {
	কাঠামো hashmap_entry entry;
	অক্ষর path[FLEX_ARRAY];
};

স্থির পূর্ণসংখ্যা path_entry_cmp(ধ্রুবক শূন্য *cmp_তথ্য UNUSED,
			  ধ্রুবক কাঠামো hashmap_entry *eptr,
			  ধ্রুবক কাঠামো hashmap_entry *entry_or_চাবি,
			  ধ্রুবক শূন্য *চাবি)
{
	ধ্রুবক কাঠামো path_entry *a, *b;

	a = container_of(eptr, ধ্রুবক কাঠামো path_entry, entry);
	b = container_of(entry_or_চাবি, ধ্রুবক কাঠামো path_entry, entry);

	ফেরত strcmp(a->path, চাবি ? চাবি : b->path);
}

স্থির শূন্য changed_files(কাঠামো repository *repo,
			  কাঠামো hashmap *ফলাফল, ধ্রুবক অক্ষর *সূচক_path,
			  ধ্রুবক অক্ষর *workdir)
{
	কাঠামো child_process update_সূচক = CHILD_PROCESS_INIT;
	কাঠামো child_process diff_files = CHILD_PROCESS_INIT;
	কাঠামো strbuf buf = STRBUF_INIT;
	ধ্রুবক অক্ষর *git_dir = absolute_path(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_সূচক.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", ফাঁকা);
	update_সূচক.no_stdin = 1;
	update_সূচক.no_stdout = 1;
	update_সূচক.no_stderr = 1;
	update_সূচক.git_cmd = 1;
	update_সূচক.use_shell = 0;
	update_সূচক.clean_on_exit = 1;
	update_সূচক.dir = workdir;
	strvec_pushf(&update_সূচক.env, "GIT_INDEX_FILE=%s", সূচক_path);
	/* Ignore any errors of update-index */
	run_command(&update_সূচক);

	strvec_pushl(&diff_files.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "diff-files", "--name-only", "-z", ফাঁকা);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.out = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", সূচক_path);
	যদি (start_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.out, "r");
	যখন (!strbuf_getline_nul(&buf, fp)) {
		কাঠামো path_entry *entry;
		FLEX_ALLOC_STR(entry, path, buf.buf);
		hashmap_entry_init(&entry->entry, strhash(buf.buf));
		hashmap_add(ফলাফল, &entry->entry);
	}
	fclose(fp);
	যদি (finish_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

স্থির পূর্ণসংখ্যা ensure_leading_directories(কাঠামো repository *repo, অক্ষর *path)
{
	নির্বাচন (safe_create_leading_directories(repo, path)) {
		ক্ষেত্র SCLD_OK:
		ক্ষেত্র SCLD_EXISTS:
			ফেরত 0;
		পূর্বনির্ধারিত:
			ফেরত error(_("could not create leading directories "
				       "of '%s'"), path);
	}
}

/*
 * Unconditional writing of a plain regular file is what
 * "git difftool --dir-diff" wants to do for symlinks.  We are preparing two
 * temporary directories to be fed to a Git-unaware tool that knows how to
 * show a diff of two directories (e.g. "diff -r A B").
 *
 * Because the tool is Git-unaware, if a symbolic link appears in either of
 * these temporary directories, it will try to dereference and show the
 * difference of the target of the symbolic link, which is not what we want,
 * as the goal of the dir-diff mode is to produce an output that is logically
 * equivalent to what "git diff" produces.
 *
 * Most importantly, we want to get textual comparison of the result of the
 * readlink(2).  get_symlink() provides that---it returns the contents of
 * the symlink that gets written to a regular file to force the external tool
 * to compare the readlink(2) result as text, even on a filesystem that is
 * capable of doing a symbolic link.
 */
স্থির অক্ষর *get_symlink(কাঠামো repository *repo,
			 কাঠামো difftool_options *dt_options,
			 ধ্রুবক কাঠামো object_id *oid, ধ্রুবক অক্ষর *path)
{
	অক্ষর *তথ্য;
	যদি (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		কাঠামো strbuf link = STRBUF_INIT;
		যদি (dt_options->has_symlinks) {
			যদি (strbuf_readlink(&link, path, strlen(path)))
				die(_("could not read symlink %s"), path);
		} অন্যথা যদি (strbuf_read_file(&link, path, 128))
			die(_("could not read symlink file %s"), path);

		তথ্য = strbuf_detach(&link, ফাঁকা);
	} অন্যথা {
		গণনা object_type type;
		আকার_t আকার_2;
		তথ্য = odb_read_object(repo->objects, oid, &type, &আকার_2);
		যদি (!তথ্য)
			die(_("could not read object %s for symlink %s"),
				oid_to_hex(oid), path);
	}

	ফেরত তথ্য;
}

স্থির পূর্ণসংখ্যা checkout_path(unsigned mode, কাঠামো object_id *oid,
			 ধ্রুবক অক্ষর *path, ধ্রুবক কাঠামো checkout *state)
{
	কাঠামো cache_entry *ce;
	পূর্ণসংখ্যা ret;

	ce = make_transient_cache_entry(mode, oid, path, 0, ফাঁকা);
	ret = checkout_entry(ce, state, ফাঁকা, ফাঁকা);

	discard_cache_entry(ce);
	ফেরত ret;
}

স্থির শূন্য write_file_in_directory(কাঠামো repository *repo,
				    কাঠামো strbuf *dir, আকার_t dir_len,
				    ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *content)
{
	add_path(dir, dir_len, path);
	ensure_leading_directories(repo, dir->buf);
	unlink(dir->buf);
	write_file(dir->buf, "%s", content);
}

/* Write the file contents for the left and right sides of the difftool
 * dir-diff representation for submodules and symlinks. Symlinks and submodules
 * are written as regular text files so that external diff tools can diff them
 * as text files, resulting in behavior that is analogous to what "git diff"
 * displays for symlink and submodule diffs.
 */
স্থির শূন্য write_standin_files(কাঠামো repository *repo,
				কাঠামো pair_entry *entry,
				কাঠামো strbuf *ldir, আকার_t ldir_len,
				কাঠামো strbuf *rdir, আকার_t rdir_len)
{
	যদি (*entry->বাম)
		write_file_in_directory(repo, ldir, ldir_len, entry->path, entry->বাম);
	যদি (*entry->ডান)
		write_file_in_directory(repo, rdir, rdir_len, entry->path, entry->ডান);
}

স্থির পূর্ণসংখ্যা run_dir_diff(কাঠামো repository *repo,
			কাঠামো difftool_options *dt_options,
			ধ্রুবক অক্ষর *extcmd, ধ্রুবক অক্ষর *prefix,
			কাঠামো child_process *child)
{
	কাঠামো strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	কাঠামো strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	কাঠামো strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	কাঠামো strbuf wtdir = STRBUF_INIT;
	কাঠামো strbuf tmpdir = STRBUF_INIT;
	অক্ষর *lbase_dir = ফাঁকা, *rbase_dir = ফাঁকা;
	আকার_t ldir_len, rdir_len, wtdir_len;
	ধ্রুবক অক্ষর *workdir, *tmp;
	পূর্ণসংখ্যা ret = 0;
	আকার_t i;
	FILE *fp = ফাঁকা;
	কাঠামো hashmap working_বৃক্ষ_dups = HASHMAP_INIT(working_বৃক্ষ_entry_cmp,
							ফাঁকা);
	কাঠামো hashmap submodules = HASHMAP_INIT(pair_cmp, ফাঁকা);
	কাঠামো hashmap symlinks2 = HASHMAP_INIT(pair_cmp, ফাঁকা);
	কাঠামো hashmap_iter iter;
	কাঠামো pair_entry *entry;
	কাঠামো সূচক_state wtindex = সূচক_STATE_INIT(repo);
	কাঠামো checkout lstate, rstate;
	পূর্ণসংখ্যা err = 0;
	কাঠামো child_process cmd = CHILD_PROCESS_INIT;
	কাঠামো hashmap wt_modified = HASHMAP_INIT(path_entry_cmp, ফাঁকা);
	কাঠামো hashmap tmp_modified = HASHMAP_INIT(path_entry_cmp, ফাঁকা);
	পূর্ণসংখ্যা indices_loaded = 0;

	workdir = repo_get_work_বৃক্ষ(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_add_absolute_path(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	যদি (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		যাও finish;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	যদি (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, আকার(lstate));
	lstate.base_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.base_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, আকার(rstate));
	rstate.base_dir = rbase_dir = xstrdup(rdir.buf);
	rstate.base_dir_len = rdir.len;
	rstate.force = 1;

	ldir_len = ldir.len;
	rdir_len = rdir.len;
	wtdir_len = wtdir.len;

	child->no_stdin = 1;
	child->git_cmd = 1;
	child->use_shell = 0;
	child->clean_on_exit = 1;
	child->dir = prefix;
	child->out = -1;
	যদি (start_command(child))
		die("could not obtain raw diff");
	fp = xfdopen(child->out, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	যখন (!strbuf_getline_nul(&info, fp)) {
		পূর্ণসংখ্যা lmode, rmode;
		কাঠামো object_id loid, roid;
		অক্ষর status;
		ধ্রুবক অক্ষর *src_path, *dst_path;

		যদি (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		যদি (parse_সূচক_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &status))
			বিরতি;
		যদি (strbuf_getline_nul(&lpath, fp))
			বিরতি;
		src_path = lpath.buf;

		i++;
		যদি (status != 'C' && status != 'R') {
			dst_path = src_path;
		} অন্যথা {
			যদি (strbuf_getline_nul(&rpath, fp))
				বিরতি;
			dst_path = rpath.buf;
		}

		যদি (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&loid));
			add_বাম_or_ডান(&submodules, src_path, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&roid));
			যদি (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			add_বাম_or_ডান(&submodules, dst_path, buf.buf, 1);
			চালু;
		}

		যদি (S_ISLNK(lmode)) {
			অক্ষর *content = get_symlink(repo, dt_options, &loid, src_path);
			add_বাম_or_ডান(&symlinks2, src_path, content, 0);
			মুক্ত(content);
		}

		যদি (S_ISLNK(rmode)) {
			অক্ষর *content = get_symlink(repo, dt_options, &roid, dst_path);
			add_বাম_or_ডান(&symlinks2, dst_path, content, 1);
			মুক্ত(content);
		}

		যদি (lmode && status != 'C') {
			যদি (checkout_path(lmode, &loid, src_path, &lstate)) {
				ret = error("could not write '%s'", src_path);
				যাও finish;
			}
		}

		যদি (rmode && !S_ISLNK(rmode)) {
			কাঠামো working_বৃক্ষ_entry *entry;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(entry, path, dst_path);
			hashmap_entry_init(&entry->entry, strhash(dst_path));
			যদি (hashmap_get(&working_বৃক্ষ_dups, &entry->entry,
					ফাঁকা)) {
				মুক্ত(entry);
				চালু;
			}
			hashmap_add(&working_বৃক্ষ_dups, &entry->entry);

			যদি (!use_wt_file(repo, workdir, dst_path, &roid)) {
				যদি (checkout_path(rmode, &roid, dst_path,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_path);
					যাও finish;
				}
			} অন্যথা যদি (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				কাঠামো cache_entry *ce2 =
					make_cache_entry(&wtindex, rmode, &roid,
							 dst_path, 0, 0);

				add_সূচক_entry(&wtindex, ce2,
						ADD_CACHE_JUST_APPEND);

				add_path(&rdir, rdir_len, dst_path);
				যদি (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_path);
					যাও finish;
				}
				add_path(&wtdir, wtdir_len, dst_path);
				যদি (dt_options->symlinks) {
					যদি (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						যাও finish;
					}
				} অন্যথা {
					কাঠামো stat st;
					যদি (stat(wtdir.buf, &st))
						st.st_mode = 0644;
					যদি (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_mode)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						যাও finish;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = ফাঁকা;
	যদি (finish_command(child)) {
		ret = error("error occurred running diff --raw");
		যাও finish;
	}

	যদি (!i)
		যাও finish;

	/*
	 * Changes to submodules require special treatment.This loop writes a
	 * temporary file to both the left and right directories to show the
	 * change in the recorded SHA1 for the submodule.
	 */
	hashmap_for_each_entry(&submodules, &iter, entry,
				entry /* member name */) {
		write_standin_files(repo, entry, &ldir, ldir_len, &rdir, rdir_len);
	}

	/*
	 * Symbolic links require special treatment. The standard "git diff"
	 * shows only the link itself, not the contents of the link target.
	 * This loop replicates that behavior.
	 */
	hashmap_for_each_entry(&symlinks2, &iter, entry,
				entry /* member name */) {

		write_standin_files(repo, entry, &ldir, ldir_len, &rdir, rdir_len);
	}

	strbuf_setlen(&ldir, ldir_len);
	strbuf_setlen(&rdir, rdir_len);

	যদি (extcmd) {
		strvec_ঠেলো(&cmd.args, extcmd);
	} অন্যথা {
		strvec_ঠেলো(&cmd.args, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.args, ldir.buf, rdir.buf, ফাঁকা);
	ret = run_command(&cmd);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_সূচক(&wtindex);

	/*
	 * If the diff includes working copy files and those
	 * files were modified during the diff, then the changes
	 * should be copied back to the working tree.
	 * Do not copy back files when symlinks are used and the
	 * external tool did not replace the original link with a file.
	 *
	 * These hashes are loaded lazily since they aren't needed
	 * in the common case of --symlinks and the difftool updating
	 * files through the symlink.
	 */
	জন্য (i = 0; i < wtindex.cache_nr; i++) {
		কাঠামো hashmap_entry dummy;
		ধ্রুবক অক্ষর *name = wtindex.cache[i]->name;
		কাঠামো stat st;

		add_path(&rdir, rdir_len, name);
		যদি (lstat(rdir.buf, &st))
			চালু;

		যদি ((dt_options->symlinks && S_ISLNK(st.st_mode)) || !S_ISREG(st.st_mode))
			চালু;

		যদি (!indices_loaded) {
			কাঠামো lock_file lock = LOCK_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			যদি (repo_hold_lock_file_for_update(repo, &lock, buf.buf, 0) < 0 ||
			    write_locked_সূচক(&wtindex, &lock, COMMIT_LOCK)) {
				ret = error("could not write %s", buf.buf);
				যাও finish;
			}
			changed_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			changed_files(repo, &tmp_modified, buf.buf, rdir.buf);
			add_path(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_entry_init(&dummy, strhash(name));
		যদি (hashmap_get(&tmp_modified, &dummy, name)) {
			add_path(&wtdir, wtdir_len, name);
			যদি (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} অন্যথা যদি (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_mode))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	যদি (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} অন্যথা {
		remove_dir_recursively(&tmpdir, 0);
		যদি (ret)
			warning(_("failed: %d"), ret);
	}

finish:
	যদি (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_বৃক্ষ_dups, কাঠামো working_বৃক্ষ_entry, entry);
	hashmap_clear_and_free(&wt_modified, কাঠামো path_entry, entry);
	hashmap_clear_and_free(&tmp_modified, কাঠামো path_entry, entry);
	hashmap_clear_and_free(&submodules, কাঠামো pair_entry, entry);
	hashmap_clear_and_free(&symlinks2, কাঠামো pair_entry, entry);
	release_সূচক(&wtindex);
	মুক্ত(lbase_dir);
	মুক্ত(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	ফেরত (ret < 0) ? 1 : ret;
}

স্থির পূর্ণসংখ্যা run_file_diff(পূর্ণসংখ্যা prompt, ধ্রুবক অক্ষর *prefix,
			 কাঠামো child_process *child)
{
	strvec_ঠেলো(&child->env, "GIT_PAGER=");
	strvec_ঠেলো(&child->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	যদি (prompt > 0)
		strvec_ঠেলো(&child->env, "GIT_DIFFTOOL_PROMPT=true");
	অন্যথা যদি (!prompt)
		strvec_ঠেলো(&child->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	child->git_cmd = 1;
	child->dir = prefix;

	ফেরত run_command(child);
}

পূর্ণসংখ্যা cmd_difftool(পূর্ণসংখ্যা argc,
		 ধ্রুবক অক্ষর **argv,
		 ধ্রুবক অক্ষর *prefix,
		 কাঠামো repository *repo)
{
	পূর্ণসংখ্যা use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_সূচক = 0;
	স্থির অক্ষর *difftool_cmd = ফাঁকা, *extcmd = ফাঁকা;
	কাঠামো difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_code = 0
	};
	কাঠামো option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_SET_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "prompt", &prompt, ফাঁকা,
			1, PARSE_OPT_NONEG | PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "symlinks", &dt_options.symlinks,
			 N_("use symlinks in dir-diff mode")),
		OPT_STRING('t', "tool", &difftool_cmd, N_("tool"),
			   N_("use the specified diff tool")),
		OPT_BOOL(0, "tool-help", &tool_help,
			 N_("print a list of diff tools that may be used with "
			    "`--tool`")),
		OPT_BOOL(0, "trust-exit-code", &dt_options.trust_exit_code,
			 N_("make 'git-difftool' exit when an invoked diff "
			    "tool returns a non-zero exit code")),
		OPT_STRING('x', "extcmd", &extcmd, N_("command"),
			   N_("specify a custom command for viewing diffs")),
		OPT_BOOL(0, "no-index", &no_সূচক, N_("passed to `diff`")),
		OPT_END()
	};
	কাঠামো child_process child = CHILD_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, prefix, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	যদি (tool_help)
		ফেরত print_tool_help();

	যদি (!no_সূচক && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	যদি (!no_সূচক){
		setup_work_বৃক্ষ(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_path(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_বৃক্ষ_ENVIRONMENT, absolute_path(repo_get_work_বৃক্ষ(repo)), 1);
	} অন্যথা যদি (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	যদি (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	অন্যথা যদি (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	যদি (difftool_cmd) {
		যদি (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		অন্যথা
			die(_("no <tool> given for --tool=<tool>"));
	}

	যদি (extcmd) {
		যদি (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		অন্যথা
			die(_("no <cmd> given for --extcmd=<cmd>"));
	}

	setenv("GIT_DIFFTOOL_TRUST_EXIT_CODE",
	       dt_options.trust_exit_code ? "true" : "false", 1);

	/*
	 * In directory diff mode, 'git-difftool--helper' is called once
	 * to compare the a / b directories. In file diff mode, 'git diff'
	 * will invoke a separate instance of 'git-difftool--helper' for
	 * each file that changed.
	 */
	strvec_ঠেলো(&child.args, "diff");
	যদি (no_সূচক)
		strvec_ঠেলো(&child.args, "--no-index");
	যদি (dir_diff)
		strvec_pushl(&child.args, "--raw", "--no-abbrev", "-z", ফাঁকা);
	strvec_pushv(&child.args, argv);

	যদি (dir_diff)
		ফেরত run_dir_diff(repo, &dt_options, extcmd, prefix, &child);
	ফেরত run_file_diff(prompt, prefix, &child);
}
