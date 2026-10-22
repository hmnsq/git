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

#അന്തര്ഭാവ "builtin.h"

#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "copy.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "repository.h"
#അന്തര്ഭാവ "sparse-index.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "entry.h"
#അന്തര്ഭാവ "setup.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	ശൂന്യ
};

ഘടന difftool_options {
	പൂർണ്ണസംഖ്യ has_symlinks;
	പൂർണ്ണസംഖ്യ symlinks;
	പൂർണ്ണസംഖ്യ trust_exit_code;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ difftool_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			   സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	ഘടന difftool_options *dt_options = (ഘടന difftool_options *)cb;
	എങ്കിൽ (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_code = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ print_tool_help(ശൂന്യം)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "mergetool", "--tool-help=diff", ശൂന്യ);
	തിരികെ run_command(&cmd);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_index_info(ഘടന repository *repo,
			    അക്ഷരം *p, പൂർണ്ണസംഖ്യ *mode1, പൂർണ്ണസംഖ്യ *mode2,
			    ഘടന object_id *oid1, ഘടന object_id *oid2,
			    അക്ഷരം *status)
{
	എങ്കിൽ (*p != ':')
		തിരികെ error("expected ':', got '%c'", *p);
	*mode1 = (പൂർണ്ണസംഖ്യ)strtol(p + 1, &p, 8);
	എങ്കിൽ (*p != ' ')
		തിരികെ error("expected ' ', got '%c'", *p);
	*mode2 = (പൂർണ്ണസംഖ്യ)strtol(p + 1, &p, 8);
	എങ്കിൽ (*p != ' ')
		തിരികെ error("expected ' ', got '%c'", *p);
	എങ്കിൽ (parse_oid_hex_algop(++p, oid1, (സ്ഥിരം അക്ഷരം **)&p, repo->hash_algo))
		തിരികെ error("expected object ID, got '%s'", p);
	എങ്കിൽ (*p != ' ')
		തിരികെ error("expected ' ', got '%c'", *p);
	എങ്കിൽ (parse_oid_hex_algop(++p, oid2, (സ്ഥിരം അക്ഷരം **)&p, repo->hash_algo))
		തിരികെ error("expected object ID, got '%s'", p);
	എങ്കിൽ (*p != ' ')
		തിരികെ error("expected ' ', got '%c'", *p);
	*status = *++p;
	എങ്കിൽ (!*status)
		തിരികെ error("missing status");
	എങ്കിൽ (p[1] && !isdigit(p[1]))
		തിരികെ error("unexpected trailer: '%s'", p + 1);
	തിരികെ 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
സ്ഥിതം ശൂന്യം add_path(ഘടന strbuf *buf, size_t base_len, സ്ഥിരം അക്ഷരം *path)
{
	strbuf_setlen(buf, base_len);
	എങ്കിൽ (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, path);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ use_wt_file(ഘടന repository *repo,
		       സ്ഥിരം അക്ഷരം *workdir, സ്ഥിരം അക്ഷരം *name,
		       ഘടന object_id *oid)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന stat st;
	പൂർണ്ണസംഖ്യ use = 0;

	strbuf_addstr(&buf, workdir);
	add_path(&buf, buf.len, name);

	എങ്കിൽ (!lstat(buf.buf, &st) && !S_ISLNK(st.st_mode)) {
		ഘടന object_id wt_oid;
		പൂർണ്ണസംഖ്യ fd = open(buf.buf, O_RDONLY);

		എങ്കിൽ (fd >= 0 &&
		    !index_fd(repo->index, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			എങ്കിൽ (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	തിരികെ use;
}

ഘടന working_വൃക്ഷം_entry {
	ഘടന hashmap_entry entry;
	അക്ഷരം path[FLEX_ARRAY];
};

സ്ഥിതം പൂർണ്ണസംഖ്യ working_വൃക്ഷം_entry_cmp(സ്ഥിരം ശൂന്യം *cmp_data UNUSED,
				  സ്ഥിരം ഘടന hashmap_entry *eptr,
				  സ്ഥിരം ഘടന hashmap_entry *entry_or_താക്കോൽ,
				  സ്ഥിരം ശൂന്യം *keydata UNUSED)
{
	സ്ഥിരം ഘടന working_വൃക്ഷം_entry *a, *b;

	a = container_of(eptr, സ്ഥിരം ഘടന working_വൃക്ഷം_entry, entry);
	b = container_of(entry_or_താക്കോൽ, സ്ഥിരം ഘടന working_വൃക്ഷം_entry, entry);

	തിരികെ strcmp(a->path, b->path);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
ഘടന pair_entry {
	ഘടന hashmap_entry entry;
	അക്ഷരം ഇടത്[PATH_MAX], വലത്[PATH_MAX];
	സ്ഥിരം അക്ഷരം path[FLEX_ARRAY];
};

സ്ഥിതം പൂർണ്ണസംഖ്യ pair_cmp(സ്ഥിരം ശൂന്യം *cmp_data UNUSED,
		    സ്ഥിരം ഘടന hashmap_entry *eptr,
		    സ്ഥിരം ഘടന hashmap_entry *entry_or_താക്കോൽ,
		    സ്ഥിരം ശൂന്യം *keydata UNUSED)
{
	സ്ഥിരം ഘടന pair_entry *a, *b;

	a = container_of(eptr, സ്ഥിരം ഘടന pair_entry, entry);
	b = container_of(entry_or_താക്കോൽ, സ്ഥിരം ഘടന pair_entry, entry);

	തിരികെ strcmp(a->path, b->path);
}

സ്ഥിതം ശൂന്യം add_ഇടത്_or_വലത്(ഘടന hashmap *map, സ്ഥിരം അക്ഷരം *path,
			      സ്ഥിരം അക്ഷരം *content, പൂർണ്ണസംഖ്യ is_വലത്)
{
	ഘടന pair_entry *e, *existing;

	FLEX_ALLOC_STR(e, path, path);
	hashmap_entry_init(&e->entry, strhash(path));
	existing = hashmap_get_entry(map, e, entry, ശൂന്യ);
	എങ്കിൽ (existing) {
		സ്വതന്ത്രം(e);
		e = existing;
	} അല്ലെങ്കിൽ {
		e->ഇടത്[0] = e->വലത്[0] = '\0';
		hashmap_add(map, &e->entry);
	}
	strlcpy(is_വലത് ? e->വലത് : e->ഇടത്, content, PATH_MAX);
}

ഘടന path_entry {
	ഘടന hashmap_entry entry;
	അക്ഷരം path[FLEX_ARRAY];
};

സ്ഥിതം പൂർണ്ണസംഖ്യ path_entry_cmp(സ്ഥിരം ശൂന്യം *cmp_data UNUSED,
			  സ്ഥിരം ഘടന hashmap_entry *eptr,
			  സ്ഥിരം ഘടന hashmap_entry *entry_or_താക്കോൽ,
			  സ്ഥിരം ശൂന്യം *താക്കോൽ)
{
	സ്ഥിരം ഘടന path_entry *a, *b;

	a = container_of(eptr, സ്ഥിരം ഘടന path_entry, entry);
	b = container_of(entry_or_താക്കോൽ, സ്ഥിരം ഘടന path_entry, entry);

	തിരികെ strcmp(a->path, താക്കോൽ ? താക്കോൽ : b->path);
}

സ്ഥിതം ശൂന്യം changed_files(ഘടന repository *repo,
			  ഘടന hashmap *result, സ്ഥിരം അക്ഷരം *index_path,
			  സ്ഥിരം അക്ഷരം *workdir)
{
	ഘടന child_process update_index = CHILD_PROCESS_INIT;
	ഘടന child_process diff_files = CHILD_PROCESS_INIT;
	ഘടന strbuf buf = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *git_dir = absolute_path(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_index.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", ശൂന്യ);
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
		     "diff-files", "--name-only", "-z", ശൂന്യ);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.out = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", index_path);
	എങ്കിൽ (start_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.out, "r");
	ആകുമ്പോൾ (!strbuf_getline_nul(&buf, fp)) {
		ഘടന path_entry *entry;
		FLEX_ALLOC_STR(entry, path, buf.buf);
		hashmap_entry_init(&entry->entry, strhash(buf.buf));
		hashmap_add(result, &entry->entry);
	}
	fclose(fp);
	എങ്കിൽ (finish_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ ensure_leading_directories(ഘടന repository *repo, അക്ഷരം *path)
{
	തിരഞ്ഞെടുപ്പ് (safe_create_leading_directories(repo, path)) {
		അവസ്ഥ SCLD_OK:
		അവസ്ഥ SCLD_EXISTS:
			തിരികെ 0;
		സ്ഥിരസ്ഥിതി:
			തിരികെ error(_("could not create leading directories "
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
സ്ഥിതം അക്ഷരം *get_symlink(ഘടന repository *repo,
			 ഘടന difftool_options *dt_options,
			 സ്ഥിരം ഘടന object_id *oid, സ്ഥിരം അക്ഷരം *path)
{
	അക്ഷരം *data;
	എങ്കിൽ (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		ഘടന strbuf link = STRBUF_INIT;
		എങ്കിൽ (dt_options->has_symlinks) {
			എങ്കിൽ (strbuf_readlink(&link, path, strlen(path)))
				die(_("could not read symlink %s"), path);
		} അല്ലെങ്കിൽ എങ്കിൽ (strbuf_read_file(&link, path, 128))
			die(_("could not read symlink file %s"), path);

		data = strbuf_detach(&link, ശൂന്യ);
	} അല്ലെങ്കിൽ {
		എണ്ണൽ object_type type;
		size_t size;
		data = odb_read_object(repo->objects, oid, &type, &size);
		എങ്കിൽ (!data)
			die(_("could not read object %s for symlink %s"),
				oid_to_hex(oid), path);
	}

	തിരികെ data;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_path(unsigned mode, ഘടന object_id *oid,
			 സ്ഥിരം അക്ഷരം *path, സ്ഥിരം ഘടന checkout *state)
{
	ഘടന cache_entry *ce;
	പൂർണ്ണസംഖ്യ ret;

	ce = make_transient_cache_entry(mode, oid, path, 0, ശൂന്യ);
	ret = checkout_entry(ce, state, ശൂന്യ, ശൂന്യ);

	discard_cache_entry(ce);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം write_file_in_directory(ഘടന repository *repo,
				    ഘടന strbuf *dir, size_t dir_len,
				    സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *content)
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
സ്ഥിതം ശൂന്യം write_standin_files(ഘടന repository *repo,
				ഘടന pair_entry *entry,
				ഘടന strbuf *ldir, size_t ldir_len,
				ഘടന strbuf *rdir, size_t rdir_len)
{
	എങ്കിൽ (*entry->ഇടത്)
		write_file_in_directory(repo, ldir, ldir_len, entry->path, entry->ഇടത്);
	എങ്കിൽ (*entry->വലത്)
		write_file_in_directory(repo, rdir, rdir_len, entry->path, entry->വലത്);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_dir_diff(ഘടന repository *repo,
			ഘടന difftool_options *dt_options,
			സ്ഥിരം അക്ഷരം *extcmd, സ്ഥിരം അക്ഷരം *prefix,
			ഘടന child_process *child)
{
	ഘടന strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	ഘടന strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	ഘടന strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	ഘടന strbuf wtdir = STRBUF_INIT;
	ഘടന strbuf tmpdir = STRBUF_INIT;
	അക്ഷരം *lbase_dir = ശൂന്യ, *rbase_dir = ശൂന്യ;
	size_t ldir_len, rdir_len, wtdir_len;
	സ്ഥിരം അക്ഷരം *workdir, *tmp;
	പൂർണ്ണസംഖ്യ ret = 0;
	size_t i;
	FILE *fp = ശൂന്യ;
	ഘടന hashmap working_വൃക്ഷം_dups = HASHMAP_INIT(working_വൃക്ഷം_entry_cmp,
							ശൂന്യ);
	ഘടന hashmap submodules = HASHMAP_INIT(pair_cmp, ശൂന്യ);
	ഘടന hashmap symlinks2 = HASHMAP_INIT(pair_cmp, ശൂന്യ);
	ഘടന hashmap_iter iter;
	ഘടന pair_entry *entry;
	ഘടന index_state wtindex = INDEX_STATE_INIT(repo);
	ഘടന checkout lstate, rstate;
	പൂർണ്ണസംഖ്യ err = 0;
	ഘടന child_process cmd = CHILD_PROCESS_INIT;
	ഘടന hashmap wt_modified = HASHMAP_INIT(path_entry_cmp, ശൂന്യ);
	ഘടന hashmap tmp_modified = HASHMAP_INIT(path_entry_cmp, ശൂന്യ);
	പൂർണ്ണസംഖ്യ indices_loaded = 0;

	workdir = repo_get_work_വൃക്ഷം(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_add_absolute_path(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	എങ്കിൽ (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		പോകുക finish;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	എങ്കിൽ (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, വലുപ്പം(lstate));
	lstate.base_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.base_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, വലുപ്പം(rstate));
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
	എങ്കിൽ (start_command(child))
		die("could not obtain raw diff");
	fp = xfdopen(child->out, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	ആകുമ്പോൾ (!strbuf_getline_nul(&info, fp)) {
		പൂർണ്ണസംഖ്യ lmode, rmode;
		ഘടന object_id loid, roid;
		അക്ഷരം status;
		സ്ഥിരം അക്ഷരം *src_path, *dst_path;

		എങ്കിൽ (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		എങ്കിൽ (parse_index_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &status))
			നിർത്തുക;
		എങ്കിൽ (strbuf_getline_nul(&lpath, fp))
			നിർത്തുക;
		src_path = lpath.buf;

		i++;
		എങ്കിൽ (status != 'C' && status != 'R') {
			dst_path = src_path;
		} അല്ലെങ്കിൽ {
			എങ്കിൽ (strbuf_getline_nul(&rpath, fp))
				നിർത്തുക;
			dst_path = rpath.buf;
		}

		എങ്കിൽ (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&loid));
			add_ഇടത്_or_വലത്(&submodules, src_path, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&roid));
			എങ്കിൽ (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			add_ഇടത്_or_വലത്(&submodules, dst_path, buf.buf, 1);
			തുടരുക;
		}

		എങ്കിൽ (S_ISLNK(lmode)) {
			അക്ഷരം *content = get_symlink(repo, dt_options, &loid, src_path);
			add_ഇടത്_or_വലത്(&symlinks2, src_path, content, 0);
			സ്വതന്ത്രം(content);
		}

		എങ്കിൽ (S_ISLNK(rmode)) {
			അക്ഷരം *content = get_symlink(repo, dt_options, &roid, dst_path);
			add_ഇടത്_or_വലത്(&symlinks2, dst_path, content, 1);
			സ്വതന്ത്രം(content);
		}

		എങ്കിൽ (lmode && status != 'C') {
			എങ്കിൽ (checkout_path(lmode, &loid, src_path, &lstate)) {
				ret = error("could not write '%s'", src_path);
				പോകുക finish;
			}
		}

		എങ്കിൽ (rmode && !S_ISLNK(rmode)) {
			ഘടന working_വൃക്ഷം_entry *entry;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(entry, path, dst_path);
			hashmap_entry_init(&entry->entry, strhash(dst_path));
			എങ്കിൽ (hashmap_get(&working_വൃക്ഷം_dups, &entry->entry,
					ശൂന്യ)) {
				സ്വതന്ത്രം(entry);
				തുടരുക;
			}
			hashmap_add(&working_വൃക്ഷം_dups, &entry->entry);

			എങ്കിൽ (!use_wt_file(repo, workdir, dst_path, &roid)) {
				എങ്കിൽ (checkout_path(rmode, &roid, dst_path,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_path);
					പോകുക finish;
				}
			} അല്ലെങ്കിൽ എങ്കിൽ (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				ഘടന cache_entry *ce2 =
					make_cache_entry(&wtindex, rmode, &roid,
							 dst_path, 0, 0);

				add_index_entry(&wtindex, ce2,
						ADD_CACHE_JUST_APPEND);

				add_path(&rdir, rdir_len, dst_path);
				എങ്കിൽ (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_path);
					പോകുക finish;
				}
				add_path(&wtdir, wtdir_len, dst_path);
				എങ്കിൽ (dt_options->symlinks) {
					എങ്കിൽ (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						പോകുക finish;
					}
				} അല്ലെങ്കിൽ {
					ഘടന stat st;
					എങ്കിൽ (stat(wtdir.buf, &st))
						st.st_mode = 0644;
					എങ്കിൽ (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_mode)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						പോകുക finish;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = ശൂന്യ;
	എങ്കിൽ (finish_command(child)) {
		ret = error("error occurred running diff --raw");
		പോകുക finish;
	}

	എങ്കിൽ (!i)
		പോകുക finish;

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

	എങ്കിൽ (extcmd) {
		strvec_തള്ളുക(&cmd.args, extcmd);
	} അല്ലെങ്കിൽ {
		strvec_തള്ളുക(&cmd.args, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.args, ldir.buf, rdir.buf, ശൂന്യ);
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
	വേണ്ടി (i = 0; i < wtindex.cache_nr; i++) {
		ഘടന hashmap_entry dummy;
		സ്ഥിരം അക്ഷരം *name = wtindex.cache[i]->name;
		ഘടന stat st;

		add_path(&rdir, rdir_len, name);
		എങ്കിൽ (lstat(rdir.buf, &st))
			തുടരുക;

		എങ്കിൽ ((dt_options->symlinks && S_ISLNK(st.st_mode)) || !S_ISREG(st.st_mode))
			തുടരുക;

		എങ്കിൽ (!indices_loaded) {
			ഘടന lock_file lock = LOCK_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			എങ്കിൽ (repo_hold_lock_file_for_update(repo, &lock, buf.buf, 0) < 0 ||
			    write_locked_index(&wtindex, &lock, COMMIT_LOCK)) {
				ret = error("could not write %s", buf.buf);
				പോകുക finish;
			}
			changed_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			changed_files(repo, &tmp_modified, buf.buf, rdir.buf);
			add_path(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_entry_init(&dummy, strhash(name));
		എങ്കിൽ (hashmap_get(&tmp_modified, &dummy, name)) {
			add_path(&wtdir, wtdir_len, name);
			എങ്കിൽ (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_mode))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	എങ്കിൽ (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} അല്ലെങ്കിൽ {
		remove_dir_recursively(&tmpdir, 0);
		എങ്കിൽ (ret)
			warning(_("failed: %d"), ret);
	}

finish:
	എങ്കിൽ (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_വൃക്ഷം_dups, ഘടന working_വൃക്ഷം_entry, entry);
	hashmap_clear_and_free(&wt_modified, ഘടന path_entry, entry);
	hashmap_clear_and_free(&tmp_modified, ഘടന path_entry, entry);
	hashmap_clear_and_free(&submodules, ഘടന pair_entry, entry);
	hashmap_clear_and_free(&symlinks2, ഘടന pair_entry, entry);
	release_index(&wtindex);
	സ്വതന്ത്രം(lbase_dir);
	സ്വതന്ത്രം(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	തിരികെ (ret < 0) ? 1 : ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_file_diff(പൂർണ്ണസംഖ്യ prompt, സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന child_process *child)
{
	strvec_തള്ളുക(&child->env, "GIT_PAGER=");
	strvec_തള്ളുക(&child->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	എങ്കിൽ (prompt > 0)
		strvec_തള്ളുക(&child->env, "GIT_DIFFTOOL_PROMPT=true");
	അല്ലെങ്കിൽ എങ്കിൽ (!prompt)
		strvec_തള്ളുക(&child->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	child->git_cmd = 1;
	child->dir = prefix;

	തിരികെ run_command(child);
}

പൂർണ്ണസംഖ്യ cmd_difftool(പൂർണ്ണസംഖ്യ argc,
		 സ്ഥിരം അക്ഷരം **argv,
		 സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_index = 0;
	സ്ഥിതം അക്ഷരം *difftool_cmd = ശൂന്യ, *extcmd = ശൂന്യ;
	ഘടന difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_code = 0
	};
	ഘടന option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_SET_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "prompt", &prompt, ശൂന്യ,
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
	ഘടന child_process child = CHILD_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, prefix, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	എങ്കിൽ (tool_help)
		തിരികെ print_tool_help();

	എങ്കിൽ (!no_index && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	എങ്കിൽ (!no_index){
		setup_work_വൃക്ഷം(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_path(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_വൃക്ഷം_ENVIRONMENT, absolute_path(repo_get_work_വൃക്ഷം(repo)), 1);
	} അല്ലെങ്കിൽ എങ്കിൽ (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	എങ്കിൽ (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	അല്ലെങ്കിൽ എങ്കിൽ (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	എങ്കിൽ (difftool_cmd) {
		എങ്കിൽ (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		അല്ലെങ്കിൽ
			die(_("no <tool> given for --tool=<tool>"));
	}

	എങ്കിൽ (extcmd) {
		എങ്കിൽ (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		അല്ലെങ്കിൽ
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
	strvec_തള്ളുക(&child.args, "diff");
	എങ്കിൽ (no_index)
		strvec_തള്ളുക(&child.args, "--no-index");
	എങ്കിൽ (dir_diff)
		strvec_pushl(&child.args, "--raw", "--no-abbrev", "-z", ശൂന്യ);
	strvec_pushv(&child.args, argv);

	എങ്കിൽ (dir_diff)
		തിരികെ run_dir_diff(repo, &dt_options, extcmd, prefix, &child);
	തിരികെ run_file_diff(prompt, prefix, &child);
}
