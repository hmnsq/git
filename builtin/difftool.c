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

#شامل "builtin.h"

#شامل "abspath.h"
#شامل "config.h"
#شامل "copy.h"
#شامل "run-command.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "read-cache-ll.h"
#شامل "repository.h"
#شامل "sparse-index.h"
#شامل "strvec.h"
#شامل "strbuf.h"
#شامل "lockfile.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "dir.h"
#شامل "entry.h"
#شامل "setup.h"

ایستا ثابت نویسه *ثابت builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	پوچ
};

ساختار difftool_options {
	صحیح has_symlinks;
	صحیح symlinks;
	صحیح trust_exit_code;
};

ایستا صحیح difftool_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			   ثابت ساختار config_context *ctx, تهی *cb)
{
	ساختار difftool_options *dt_options = (ساختار difftool_options *)cb;
	اگر (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_code = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

ایستا صحیح print_tool_help(تهی)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "mergetool", "--tool-help=diff", پوچ);
	بازگشت run_command(&cmd);
}

ایستا صحیح parse_index_info(ساختار repository *repo,
			    نویسه *p, صحیح *mode1, صحیح *mode2,
			    ساختار object_id *oid1, ساختار object_id *oid2,
			    نویسه *status)
{
	اگر (*p != ':')
		بازگشت error("expected ':', got '%c'", *p);
	*mode1 = (صحیح)strtol(p + 1, &p, 8);
	اگر (*p != ' ')
		بازگشت error("expected ' ', got '%c'", *p);
	*mode2 = (صحیح)strtol(p + 1, &p, 8);
	اگر (*p != ' ')
		بازگشت error("expected ' ', got '%c'", *p);
	اگر (parse_oid_hex_algop(++p, oid1, (ثابت نویسه **)&p, repo->hash_algo))
		بازگشت error("expected object ID, got '%s'", p);
	اگر (*p != ' ')
		بازگشت error("expected ' ', got '%c'", *p);
	اگر (parse_oid_hex_algop(++p, oid2, (ثابت نویسه **)&p, repo->hash_algo))
		بازگشت error("expected object ID, got '%s'", p);
	اگر (*p != ' ')
		بازگشت error("expected ' ', got '%c'", *p);
	*status = *++p;
	اگر (!*status)
		بازگشت error("missing status");
	اگر (p[1] && !isdigit(p[1]))
		بازگشت error("unexpected trailer: '%s'", p + 1);
	بازگشت 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
ایستا تهی add_path(ساختار strbuf *buf, size_t base_len, ثابت نویسه *path)
{
	strbuf_setlen(buf, base_len);
	اگر (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, path);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
ایستا صحیح use_wt_file(ساختار repository *repo,
		       ثابت نویسه *workdir, ثابت نویسه *name,
		       ساختار object_id *oid)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار stat st;
	صحیح use = 0;

	strbuf_addstr(&buf, workdir);
	add_path(&buf, buf.len, name);

	اگر (!lstat(buf.buf, &st) && !S_ISLNK(st.st_mode)) {
		ساختار object_id wt_oid;
		صحیح fd = open(buf.buf, O_RDONLY);

		اگر (fd >= 0 &&
		    !index_fd(repo->index, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			اگر (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} وگرنه اگر (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	بازگشت use;
}

ساختار working_درخت_entry {
	ساختار hashmap_entry entry;
	نویسه path[FLEX_ARRAY];
};

ایستا صحیح working_درخت_entry_cmp(ثابت تهی *cmp_data UNUSED,
				  ثابت ساختار hashmap_entry *eptr,
				  ثابت ساختار hashmap_entry *entry_or_کلید,
				  ثابت تهی *keydata UNUSED)
{
	ثابت ساختار working_درخت_entry *a, *b;

	a = container_of(eptr, ثابت ساختار working_درخت_entry, entry);
	b = container_of(entry_or_کلید, ثابت ساختار working_درخت_entry, entry);

	بازگشت strcmp(a->path, b->path);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
ساختار pair_entry {
	ساختار hashmap_entry entry;
	نویسه چپ[PATH_MAX], راست[PATH_MAX];
	ثابت نویسه path[FLEX_ARRAY];
};

ایستا صحیح pair_cmp(ثابت تهی *cmp_data UNUSED,
		    ثابت ساختار hashmap_entry *eptr,
		    ثابت ساختار hashmap_entry *entry_or_کلید,
		    ثابت تهی *keydata UNUSED)
{
	ثابت ساختار pair_entry *a, *b;

	a = container_of(eptr, ثابت ساختار pair_entry, entry);
	b = container_of(entry_or_کلید, ثابت ساختار pair_entry, entry);

	بازگشت strcmp(a->path, b->path);
}

ایستا تهی add_چپ_or_راست(ساختار hashmap *map, ثابت نویسه *path,
			      ثابت نویسه *content, صحیح is_راست)
{
	ساختار pair_entry *e, *existing;

	FLEX_ALLOC_STR(e, path, path);
	hashmap_entry_init(&e->entry, strhash(path));
	existing = hashmap_get_entry(map, e, entry, پوچ);
	اگر (existing) {
		آزاد(e);
		e = existing;
	} وگرنه {
		e->چپ[0] = e->راست[0] = '\0';
		hashmap_add(map, &e->entry);
	}
	strlcpy(is_راست ? e->راست : e->چپ, content, PATH_MAX);
}

ساختار path_entry {
	ساختار hashmap_entry entry;
	نویسه path[FLEX_ARRAY];
};

ایستا صحیح path_entry_cmp(ثابت تهی *cmp_data UNUSED,
			  ثابت ساختار hashmap_entry *eptr,
			  ثابت ساختار hashmap_entry *entry_or_کلید,
			  ثابت تهی *کلید)
{
	ثابت ساختار path_entry *a, *b;

	a = container_of(eptr, ثابت ساختار path_entry, entry);
	b = container_of(entry_or_کلید, ثابت ساختار path_entry, entry);

	بازگشت strcmp(a->path, کلید ? کلید : b->path);
}

ایستا تهی changed_files(ساختار repository *repo,
			  ساختار hashmap *result, ثابت نویسه *index_path,
			  ثابت نویسه *workdir)
{
	ساختار child_process update_index = CHILD_PROCESS_INIT;
	ساختار child_process diff_files = CHILD_PROCESS_INIT;
	ساختار strbuf buf = STRBUF_INIT;
	ثابت نویسه *git_dir = absolute_path(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_index.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", پوچ);
	update_index.no_stdin = 1;
	update_index.no_stdout = 1;
	update_index.no_stderr = 1;
	update_index.git_cmd = 1;
	update_index.use_shell = 0;
	update_index.clean_on_exit = 1;
	update_index.dir = workdir;
	strvec_pushf(&update_index.env, "GIT_INDEX_FILE=%s", index_path);
	/* Ignore any errors of update-index */
	run_command(&update_index);

	strvec_pushl(&diff_files.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "diff-files", "--name-only", "-z", پوچ);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.out = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", index_path);
	اگر (start_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.out, "r");
	وقتی (!strbuf_getline_nul(&buf, fp)) {
		ساختار path_entry *entry;
		FLEX_ALLOC_STR(entry, path, buf.buf);
		hashmap_entry_init(&entry->entry, strhash(buf.buf));
		hashmap_add(result, &entry->entry);
	}
	fclose(fp);
	اگر (finish_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

ایستا صحیح ensure_leading_directories(ساختار repository *repo, نویسه *path)
{
	انتخاب (safe_create_leading_directories(repo, path)) {
		حالت SCLD_OK:
		حالت SCLD_EXISTS:
			بازگشت 0;
		پیشفرض:
			بازگشت error(_("could not create leading directories "
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
ایستا نویسه *get_symlink(ساختار repository *repo,
			 ساختار difftool_options *dt_options,
			 ثابت ساختار object_id *oid, ثابت نویسه *path)
{
	نویسه *data;
	اگر (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		ساختار strbuf link = STRBUF_INIT;
		اگر (dt_options->has_symlinks) {
			اگر (strbuf_readlink(&link, path, strlen(path)))
				die(_("could not read symlink %s"), path);
		} وگرنه اگر (strbuf_read_file(&link, path, 128))
			die(_("could not read symlink file %s"), path);

		data = strbuf_detach(&link, پوچ);
	} وگرنه {
		شمارش object_type type;
		size_t size;
		data = odb_read_object(repo->objects, oid, &type, &size);
		اگر (!data)
			die(_("could not read object %s for symlink %s"),
				oid_to_hex(oid), path);
	}

	بازگشت data;
}

ایستا صحیح checkout_path(unsigned mode, ساختار object_id *oid,
			 ثابت نویسه *path, ثابت ساختار checkout *state)
{
	ساختار cache_entry *ce;
	صحیح ret;

	ce = make_transient_cache_entry(mode, oid, path, 0, پوچ);
	ret = checkout_entry(ce, state, پوچ, پوچ);

	discard_cache_entry(ce);
	بازگشت ret;
}

ایستا تهی write_file_in_directory(ساختار repository *repo,
				    ساختار strbuf *dir, size_t dir_len,
				    ثابت نویسه *path, ثابت نویسه *content)
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
ایستا تهی write_standin_files(ساختار repository *repo,
				ساختار pair_entry *entry,
				ساختار strbuf *ldir, size_t ldir_len,
				ساختار strbuf *rdir, size_t rdir_len)
{
	اگر (*entry->چپ)
		write_file_in_directory(repo, ldir, ldir_len, entry->path, entry->چپ);
	اگر (*entry->راست)
		write_file_in_directory(repo, rdir, rdir_len, entry->path, entry->راست);
}

ایستا صحیح run_dir_diff(ساختار repository *repo,
			ساختار difftool_options *dt_options,
			ثابت نویسه *extcmd, ثابت نویسه *prefix,
			ساختار child_process *child)
{
	ساختار strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	ساختار strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	ساختار strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	ساختار strbuf wtdir = STRBUF_INIT;
	ساختار strbuf tmpdir = STRBUF_INIT;
	نویسه *lbase_dir = پوچ, *rbase_dir = پوچ;
	size_t ldir_len, rdir_len, wtdir_len;
	ثابت نویسه *workdir, *tmp;
	صحیح ret = 0;
	size_t i;
	FILE *fp = پوچ;
	ساختار hashmap working_درخت_dups = HASHMAP_INIT(working_درخت_entry_cmp,
							پوچ);
	ساختار hashmap submodules = HASHMAP_INIT(pair_cmp, پوچ);
	ساختار hashmap symlinks2 = HASHMAP_INIT(pair_cmp, پوچ);
	ساختار hashmap_iter iter;
	ساختار pair_entry *entry;
	ساختار index_state wtindex = INDEX_STATE_INIT(repo);
	ساختار checkout lstate, rstate;
	صحیح err = 0;
	ساختار child_process cmd = CHILD_PROCESS_INIT;
	ساختار hashmap wt_modified = HASHMAP_INIT(path_entry_cmp, پوچ);
	ساختار hashmap tmp_modified = HASHMAP_INIT(path_entry_cmp, پوچ);
	صحیح indices_loaded = 0;

	workdir = repo_get_work_درخت(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_add_absolute_path(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	اگر (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		برو finish;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	اگر (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, اندازه(lstate));
	lstate.base_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.base_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, اندازه(rstate));
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
	اگر (start_command(child))
		die("could not obtain raw diff");
	fp = xfdopen(child->out, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	وقتی (!strbuf_getline_nul(&info, fp)) {
		صحیح lmode, rmode;
		ساختار object_id loid, roid;
		نویسه status;
		ثابت نویسه *src_path, *dst_path;

		اگر (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		اگر (parse_index_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &status))
			بشکن;
		اگر (strbuf_getline_nul(&lpath, fp))
			بشکن;
		src_path = lpath.buf;

		i++;
		اگر (status != 'C' && status != 'R') {
			dst_path = src_path;
		} وگرنه {
			اگر (strbuf_getline_nul(&rpath, fp))
				بشکن;
			dst_path = rpath.buf;
		}

		اگر (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&loid));
			add_چپ_or_راست(&submodules, src_path, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&roid));
			اگر (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			add_چپ_or_راست(&submodules, dst_path, buf.buf, 1);
			ادامه;
		}

		اگر (S_ISLNK(lmode)) {
			نویسه *content = get_symlink(repo, dt_options, &loid, src_path);
			add_چپ_or_راست(&symlinks2, src_path, content, 0);
			آزاد(content);
		}

		اگر (S_ISLNK(rmode)) {
			نویسه *content = get_symlink(repo, dt_options, &roid, dst_path);
			add_چپ_or_راست(&symlinks2, dst_path, content, 1);
			آزاد(content);
		}

		اگر (lmode && status != 'C') {
			اگر (checkout_path(lmode, &loid, src_path, &lstate)) {
				ret = error("could not write '%s'", src_path);
				برو finish;
			}
		}

		اگر (rmode && !S_ISLNK(rmode)) {
			ساختار working_درخت_entry *entry;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(entry, path, dst_path);
			hashmap_entry_init(&entry->entry, strhash(dst_path));
			اگر (hashmap_get(&working_درخت_dups, &entry->entry,
					پوچ)) {
				آزاد(entry);
				ادامه;
			}
			hashmap_add(&working_درخت_dups, &entry->entry);

			اگر (!use_wt_file(repo, workdir, dst_path, &roid)) {
				اگر (checkout_path(rmode, &roid, dst_path,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_path);
					برو finish;
				}
			} وگرنه اگر (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				ساختار cache_entry *ce2 =
					make_cache_entry(&wtindex, rmode, &roid,
							 dst_path, 0, 0);

				add_index_entry(&wtindex, ce2,
						ADD_CACHE_JUST_APPEND);

				add_path(&rdir, rdir_len, dst_path);
				اگر (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_path);
					برو finish;
				}
				add_path(&wtdir, wtdir_len, dst_path);
				اگر (dt_options->symlinks) {
					اگر (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						برو finish;
					}
				} وگرنه {
					ساختار stat st;
					اگر (stat(wtdir.buf, &st))
						st.st_mode = 0644;
					اگر (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_mode)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						برو finish;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = پوچ;
	اگر (finish_command(child)) {
		ret = error("error occurred running diff --raw");
		برو finish;
	}

	اگر (!i)
		برو finish;

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

	اگر (extcmd) {
		strvec_افزودن(&cmd.args, extcmd);
	} وگرنه {
		strvec_افزودن(&cmd.args, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.args, ldir.buf, rdir.buf, پوچ);
	ret = run_command(&cmd);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(&wtindex);

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
	برای (i = 0; i < wtindex.cache_nr; i++) {
		ساختار hashmap_entry dummy;
		ثابت نویسه *name = wtindex.cache[i]->name;
		ساختار stat st;

		add_path(&rdir, rdir_len, name);
		اگر (lstat(rdir.buf, &st))
			ادامه;

		اگر ((dt_options->symlinks && S_ISLNK(st.st_mode)) || !S_ISREG(st.st_mode))
			ادامه;

		اگر (!indices_loaded) {
			ساختار lock_file lock = LOCK_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			اگر (repo_hold_lock_file_for_update(repo, &lock, buf.buf, 0) < 0 ||
			    write_locked_index(&wtindex, &lock, COMMIT_LOCK)) {
				ret = error("could not write %s", buf.buf);
				برو finish;
			}
			changed_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			changed_files(repo, &tmp_modified, buf.buf, rdir.buf);
			add_path(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_entry_init(&dummy, strhash(name));
		اگر (hashmap_get(&tmp_modified, &dummy, name)) {
			add_path(&wtdir, wtdir_len, name);
			اگر (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} وگرنه اگر (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_mode))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	اگر (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} وگرنه {
		remove_dir_recursively(&tmpdir, 0);
		اگر (ret)
			warning(_("failed: %d"), ret);
	}

finish:
	اگر (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_درخت_dups, ساختار working_درخت_entry, entry);
	hashmap_clear_and_free(&wt_modified, ساختار path_entry, entry);
	hashmap_clear_and_free(&tmp_modified, ساختار path_entry, entry);
	hashmap_clear_and_free(&submodules, ساختار pair_entry, entry);
	hashmap_clear_and_free(&symlinks2, ساختار pair_entry, entry);
	release_index(&wtindex);
	آزاد(lbase_dir);
	آزاد(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	بازگشت (ret < 0) ? 1 : ret;
}

ایستا صحیح run_file_diff(صحیح prompt, ثابت نویسه *prefix,
			 ساختار child_process *child)
{
	strvec_افزودن(&child->env, "GIT_PAGER=");
	strvec_افزودن(&child->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	اگر (prompt > 0)
		strvec_افزودن(&child->env, "GIT_DIFFTOOL_PROMPT=true");
	وگرنه اگر (!prompt)
		strvec_افزودن(&child->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	child->git_cmd = 1;
	child->dir = prefix;

	بازگشت run_command(child);
}

صحیح cmd_difftool(صحیح argc,
		 ثابت نویسه **argv,
		 ثابت نویسه *prefix,
		 ساختار repository *repo)
{
	صحیح use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_index = 0;
	ایستا نویسه *difftool_cmd = پوچ, *extcmd = پوچ;
	ساختار difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_code = 0
	};
	ساختار option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_SET_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "prompt", &prompt, پوچ,
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
		OPT_BOOL(0, "no-index", &no_index, N_("passed to `diff`")),
		OPT_END()
	};
	ساختار child_process child = CHILD_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, prefix, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	اگر (tool_help)
		بازگشت print_tool_help();

	اگر (!no_index && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	اگر (!no_index){
		setup_work_درخت(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_path(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_درخت_ENVIRONMENT, absolute_path(repo_get_work_درخت(repo)), 1);
	} وگرنه اگر (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	اگر (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	وگرنه اگر (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	اگر (difftool_cmd) {
		اگر (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		وگرنه
			die(_("no <tool> given for --tool=<tool>"));
	}

	اگر (extcmd) {
		اگر (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		وگرنه
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
	strvec_افزودن(&child.args, "diff");
	اگر (no_index)
		strvec_افزودن(&child.args, "--no-index");
	اگر (dir_diff)
		strvec_pushl(&child.args, "--raw", "--no-abbrev", "-z", پوچ);
	strvec_pushv(&child.args, argv);

	اگر (dir_diff)
		بازگشت run_dir_diff(repo, &dt_options, extcmd, prefix, &child);
	بازگشت run_file_diff(prompt, prefix, &child);
}
