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

#ضمن "builtin.h"

#ضمن "abspath.h"
#ضمن "config.h"
#ضمن "copy.h"
#ضمن "run-command.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "read-cache-ll.h"
#ضمن "repository.h"
#ضمن "sparse-index.h"
#ضمن "strvec.h"
#ضمن "strbuf.h"
#ضمن "lockfile.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "dir.h"
#ضمن "entry.h"
#ضمن "setup.h"

ساكن ثابت حرف *ثابت builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	عدم
};

بنية difftool_options {
	صحيح has_symlinks;
	صحيح symlinks;
	صحيح trust_exit_code;
};

ساكن صحيح difftool_config(ثابت حرف *var, ثابت حرف *بەها,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية difftool_options *dt_options = (بنية difftool_options *)cb;
	إذا (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_code = git_config_bool(var, بەها);
		أرجع 0;
	}
	إذا (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, بەها);
		أرجع 0;
	}

	أرجع git_default_config(var, بەها, ctx, cb);
}

ساكن صحيح print_tool_help(فراغ)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "mergetool", "--tool-help=diff", عدم);
	أرجع run_command(&cmd);
}

ساكن صحيح parse_index_info(بنية repository *repo,
			    حرف *p, صحيح *mode1, صحيح *mode2,
			    بنية object_id *oid1, بنية object_id *oid2,
			    حرف *status)
{
	إذا (*p != ':')
		أرجع error("expected ':', got '%c'", *p);
	*mode1 = (صحيح)strtol(p + 1, &p, 8);
	إذا (*p != ' ')
		أرجع error("expected ' ', got '%c'", *p);
	*mode2 = (صحيح)strtol(p + 1, &p, 8);
	إذا (*p != ' ')
		أرجع error("expected ' ', got '%c'", *p);
	إذا (parse_oid_hex_algop(++p, oid1, (ثابت حرف **)&p, repo->hash_algo))
		أرجع error("expected object ID, got '%s'", p);
	إذا (*p != ' ')
		أرجع error("expected ' ', got '%c'", *p);
	إذا (parse_oid_hex_algop(++p, oid2, (ثابت حرف **)&p, repo->hash_algo))
		أرجع error("expected object ID, got '%s'", p);
	إذا (*p != ' ')
		أرجع error("expected ' ', got '%c'", *p);
	*status = *++p;
	إذا (!*status)
		أرجع error("missing status");
	إذا (p[1] && !isdigit(p[1]))
		أرجع error("unexpected trailer: '%s'", p + 1);
	أرجع 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
ساكن فراغ add_path(بنية strbuf *buf, size_t base_len, ثابت حرف *path)
{
	strbuf_setlen(buf, base_len);
	إذا (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, path);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
ساكن صحيح use_wt_file(بنية repository *repo,
		       ثابت حرف *workdir, ثابت حرف *name,
		       بنية object_id *oid)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية stat st;
	صحيح use = 0;

	strbuf_addstr(&buf, workdir);
	add_path(&buf, buf.len, name);

	إذا (!lstat(buf.buf, &st) && !S_ISLNK(st.st_mode)) {
		بنية object_id wt_oid;
		صحيح fd = open(buf.buf, O_RDONLY);

		إذا (fd >= 0 &&
		    !index_fd(repo->index, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			إذا (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} وإلا إذا (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	أرجع use;
}

بنية working_درەخت_entry {
	بنية hashmap_entry entry;
	حرف path[FLEX_ARRAY];
};

ساكن صحيح working_درەخت_entry_cmp(ثابت فراغ *cmp_data UNUSED,
				  ثابت بنية hashmap_entry *eptr,
				  ثابت بنية hashmap_entry *entry_or_کلیل,
				  ثابت فراغ *keydata UNUSED)
{
	ثابت بنية working_درەخت_entry *a, *b;

	a = container_of(eptr, ثابت بنية working_درەخت_entry, entry);
	b = container_of(entry_or_کلیل, ثابت بنية working_درەخت_entry, entry);

	أرجع strcmp(a->path, b->path);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
بنية pair_entry {
	بنية hashmap_entry entry;
	حرف چەپ[PATH_MAX], ڕاست[PATH_MAX];
	ثابت حرف path[FLEX_ARRAY];
};

ساكن صحيح pair_cmp(ثابت فراغ *cmp_data UNUSED,
		    ثابت بنية hashmap_entry *eptr,
		    ثابت بنية hashmap_entry *entry_or_کلیل,
		    ثابت فراغ *keydata UNUSED)
{
	ثابت بنية pair_entry *a, *b;

	a = container_of(eptr, ثابت بنية pair_entry, entry);
	b = container_of(entry_or_کلیل, ثابت بنية pair_entry, entry);

	أرجع strcmp(a->path, b->path);
}

ساكن فراغ add_چەپ_or_ڕاست(بنية hashmap *map, ثابت حرف *path,
			      ثابت حرف *content, صحيح is_ڕاست)
{
	بنية pair_entry *e, *existing;

	FLEX_ALLOC_STR(e, path, path);
	hashmap_entry_init(&e->entry, strhash(path));
	existing = hashmap_get_entry(map, e, entry, عدم);
	إذا (existing) {
		حرر(e);
		e = existing;
	} وإلا {
		e->چەپ[0] = e->ڕاست[0] = '\0';
		hashmap_add(map, &e->entry);
	}
	strlcpy(is_ڕاست ? e->ڕاست : e->چەپ, content, PATH_MAX);
}

بنية path_entry {
	بنية hashmap_entry entry;
	حرف path[FLEX_ARRAY];
};

ساكن صحيح path_entry_cmp(ثابت فراغ *cmp_data UNUSED,
			  ثابت بنية hashmap_entry *eptr,
			  ثابت بنية hashmap_entry *entry_or_کلیل,
			  ثابت فراغ *کلیل)
{
	ثابت بنية path_entry *a, *b;

	a = container_of(eptr, ثابت بنية path_entry, entry);
	b = container_of(entry_or_کلیل, ثابت بنية path_entry, entry);

	أرجع strcmp(a->path, کلیل ? کلیل : b->path);
}

ساكن فراغ changed_files(بنية repository *repo,
			  بنية hashmap *result, ثابت حرف *index_path,
			  ثابت حرف *workdir)
{
	بنية child_process update_index = CHILD_PROCESS_INIT;
	بنية child_process diff_files = CHILD_PROCESS_INIT;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *git_dir = absolute_path(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_index.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", عدم);
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
		     "diff-files", "--name-only", "-z", عدم);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.out = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", index_path);
	إذا (start_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.out, "r");
	طالما (!strbuf_getline_nul(&buf, fp)) {
		بنية path_entry *entry;
		FLEX_ALLOC_STR(entry, path, buf.buf);
		hashmap_entry_init(&entry->entry, strhash(buf.buf));
		hashmap_add(result, &entry->entry);
	}
	fclose(fp);
	إذا (finish_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

ساكن صحيح ensure_leading_directories(بنية repository *repo, حرف *path)
{
	بدل (safe_create_leading_directories(repo, path)) {
		حالة SCLD_OK:
		حالة SCLD_EXISTS:
			أرجع 0;
		افتراضي:
			أرجع error(_("could not create leading directories "
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
ساكن حرف *get_symlink(بنية repository *repo,
			 بنية difftool_options *dt_options,
			 ثابت بنية object_id *oid, ثابت حرف *path)
{
	حرف *data;
	إذا (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		بنية strbuf link = STRBUF_INIT;
		إذا (dt_options->has_symlinks) {
			إذا (strbuf_readlink(&link, path, strlen(path)))
				die(_("could not read symlink %s"), path);
		} وإلا إذا (strbuf_read_file(&link, path, 128))
			die(_("could not read symlink file %s"), path);

		data = strbuf_detach(&link, عدم);
	} وإلا {
		تعداد object_type type;
		size_t size;
		data = odb_read_object(repo->objects, oid, &type, &size);
		إذا (!data)
			die(_("could not read object %s for symlink %s"),
				oid_to_hex(oid), path);
	}

	أرجع data;
}

ساكن صحيح checkout_path(unsigned mode, بنية object_id *oid,
			 ثابت حرف *path, ثابت بنية checkout *state)
{
	بنية cache_entry *ce;
	صحيح ret;

	ce = make_transient_cache_entry(mode, oid, path, 0, عدم);
	ret = checkout_entry(ce, state, عدم, عدم);

	discard_cache_entry(ce);
	أرجع ret;
}

ساكن فراغ write_file_in_directory(بنية repository *repo,
				    بنية strbuf *dir, size_t dir_len,
				    ثابت حرف *path, ثابت حرف *content)
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
ساكن فراغ write_standin_files(بنية repository *repo,
				بنية pair_entry *entry,
				بنية strbuf *ldir, size_t ldir_len,
				بنية strbuf *rdir, size_t rdir_len)
{
	إذا (*entry->چەپ)
		write_file_in_directory(repo, ldir, ldir_len, entry->path, entry->چەپ);
	إذا (*entry->ڕاست)
		write_file_in_directory(repo, rdir, rdir_len, entry->path, entry->ڕاست);
}

ساكن صحيح run_dir_diff(بنية repository *repo,
			بنية difftool_options *dt_options,
			ثابت حرف *extcmd, ثابت حرف *prefix,
			بنية child_process *child)
{
	بنية strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	بنية strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	بنية strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	بنية strbuf wtdir = STRBUF_INIT;
	بنية strbuf tmpdir = STRBUF_INIT;
	حرف *lbase_dir = عدم, *rbase_dir = عدم;
	size_t ldir_len, rdir_len, wtdir_len;
	ثابت حرف *workdir, *tmp;
	صحيح ret = 0;
	size_t i;
	FILE *fp = عدم;
	بنية hashmap working_درەخت_dups = HASHMAP_INIT(working_درەخت_entry_cmp,
							عدم);
	بنية hashmap submodules = HASHMAP_INIT(pair_cmp, عدم);
	بنية hashmap symlinks2 = HASHMAP_INIT(pair_cmp, عدم);
	بنية hashmap_iter iter;
	بنية pair_entry *entry;
	بنية index_state wtindex = INDEX_STATE_INIT(repo);
	بنية checkout lstate, rstate;
	صحيح err = 0;
	بنية child_process cmd = CHILD_PROCESS_INIT;
	بنية hashmap wt_modified = HASHMAP_INIT(path_entry_cmp, عدم);
	بنية hashmap tmp_modified = HASHMAP_INIT(path_entry_cmp, عدم);
	صحيح indices_loaded = 0;

	workdir = repo_get_work_درەخت(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_add_absolute_path(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	إذا (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		اذهب finish;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	إذا (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, حجم(lstate));
	lstate.base_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.base_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, حجم(rstate));
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
	إذا (start_command(child))
		die("could not obtain raw diff");
	fp = xfdopen(child->out, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	طالما (!strbuf_getline_nul(&info, fp)) {
		صحيح lmode, rmode;
		بنية object_id loid, roid;
		حرف status;
		ثابت حرف *src_path, *dst_path;

		إذا (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		إذا (parse_index_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &status))
			اكسر;
		إذا (strbuf_getline_nul(&lpath, fp))
			اكسر;
		src_path = lpath.buf;

		i++;
		إذا (status != 'C' && status != 'R') {
			dst_path = src_path;
		} وإلا {
			إذا (strbuf_getline_nul(&rpath, fp))
				اكسر;
			dst_path = rpath.buf;
		}

		إذا (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&loid));
			add_چەپ_or_ڕاست(&submodules, src_path, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&roid));
			إذا (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			add_چەپ_or_ڕاست(&submodules, dst_path, buf.buf, 1);
			استمر;
		}

		إذا (S_ISLNK(lmode)) {
			حرف *content = get_symlink(repo, dt_options, &loid, src_path);
			add_چەپ_or_ڕاست(&symlinks2, src_path, content, 0);
			حرر(content);
		}

		إذا (S_ISLNK(rmode)) {
			حرف *content = get_symlink(repo, dt_options, &roid, dst_path);
			add_چەپ_or_ڕاست(&symlinks2, dst_path, content, 1);
			حرر(content);
		}

		إذا (lmode && status != 'C') {
			إذا (checkout_path(lmode, &loid, src_path, &lstate)) {
				ret = error("could not write '%s'", src_path);
				اذهب finish;
			}
		}

		إذا (rmode && !S_ISLNK(rmode)) {
			بنية working_درەخت_entry *entry;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(entry, path, dst_path);
			hashmap_entry_init(&entry->entry, strhash(dst_path));
			إذا (hashmap_get(&working_درەخت_dups, &entry->entry,
					عدم)) {
				حرر(entry);
				استمر;
			}
			hashmap_add(&working_درەخت_dups, &entry->entry);

			إذا (!use_wt_file(repo, workdir, dst_path, &roid)) {
				إذا (checkout_path(rmode, &roid, dst_path,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_path);
					اذهب finish;
				}
			} وإلا إذا (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				بنية cache_entry *ce2 =
					make_cache_entry(&wtindex, rmode, &roid,
							 dst_path, 0, 0);

				add_index_entry(&wtindex, ce2,
						ADD_CACHE_JUST_APPEND);

				add_path(&rdir, rdir_len, dst_path);
				إذا (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_path);
					اذهب finish;
				}
				add_path(&wtdir, wtdir_len, dst_path);
				إذا (dt_options->symlinks) {
					إذا (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						اذهب finish;
					}
				} وإلا {
					بنية stat st;
					إذا (stat(wtdir.buf, &st))
						st.st_mode = 0644;
					إذا (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_mode)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						اذهب finish;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = عدم;
	إذا (finish_command(child)) {
		ret = error("error occurred running diff --raw");
		اذهب finish;
	}

	إذا (!i)
		اذهب finish;

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

	إذا (extcmd) {
		strvec_پاڵنان(&cmd.args, extcmd);
	} وإلا {
		strvec_پاڵنان(&cmd.args, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.args, ldir.buf, rdir.buf, عدم);
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
	لكل (i = 0; i < wtindex.cache_nr; i++) {
		بنية hashmap_entry dummy;
		ثابت حرف *name = wtindex.cache[i]->name;
		بنية stat st;

		add_path(&rdir, rdir_len, name);
		إذا (lstat(rdir.buf, &st))
			استمر;

		إذا ((dt_options->symlinks && S_ISLNK(st.st_mode)) || !S_ISREG(st.st_mode))
			استمر;

		إذا (!indices_loaded) {
			بنية lock_file lock = LOCK_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			إذا (repo_hold_lock_file_for_update(repo, &lock, buf.buf, 0) < 0 ||
			    write_locked_index(&wtindex, &lock, COMMIT_LOCK)) {
				ret = error("could not write %s", buf.buf);
				اذهب finish;
			}
			changed_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			changed_files(repo, &tmp_modified, buf.buf, rdir.buf);
			add_path(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_entry_init(&dummy, strhash(name));
		إذا (hashmap_get(&tmp_modified, &dummy, name)) {
			add_path(&wtdir, wtdir_len, name);
			إذا (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} وإلا إذا (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_mode))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	إذا (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} وإلا {
		remove_dir_recursively(&tmpdir, 0);
		إذا (ret)
			warning(_("failed: %d"), ret);
	}

finish:
	إذا (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_درەخت_dups, بنية working_درەخت_entry, entry);
	hashmap_clear_and_free(&wt_modified, بنية path_entry, entry);
	hashmap_clear_and_free(&tmp_modified, بنية path_entry, entry);
	hashmap_clear_and_free(&submodules, بنية pair_entry, entry);
	hashmap_clear_and_free(&symlinks2, بنية pair_entry, entry);
	release_index(&wtindex);
	حرر(lbase_dir);
	حرر(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	أرجع (ret < 0) ? 1 : ret;
}

ساكن صحيح run_file_diff(صحيح prompt, ثابت حرف *prefix,
			 بنية child_process *child)
{
	strvec_پاڵنان(&child->env, "GIT_PAGER=");
	strvec_پاڵنان(&child->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	إذا (prompt > 0)
		strvec_پاڵنان(&child->env, "GIT_DIFFTOOL_PROMPT=true");
	وإلا إذا (!prompt)
		strvec_پاڵنان(&child->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	child->git_cmd = 1;
	child->dir = prefix;

	أرجع run_command(child);
}

صحيح cmd_difftool(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *prefix,
		 بنية repository *repo)
{
	صحيح use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_index = 0;
	ساكن حرف *difftool_cmd = عدم, *extcmd = عدم;
	بنية difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_code = 0
	};
	بنية option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_SET_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "prompt", &prompt, عدم,
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
	بنية child_process child = CHILD_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, prefix, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (tool_help)
		أرجع print_tool_help();

	إذا (!no_index && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	إذا (!no_index){
		setup_work_درەخت(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_path(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_درەخت_ENVIRONMENT, absolute_path(repo_get_work_درەخت(repo)), 1);
	} وإلا إذا (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	إذا (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	وإلا إذا (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	إذا (difftool_cmd) {
		إذا (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		وإلا
			die(_("no <tool> given for --tool=<tool>"));
	}

	إذا (extcmd) {
		إذا (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		وإلا
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
	strvec_پاڵنان(&child.args, "diff");
	إذا (no_index)
		strvec_پاڵنان(&child.args, "--no-index");
	إذا (dir_diff)
		strvec_pushl(&child.args, "--raw", "--no-abbrev", "-z", عدم);
	strvec_pushv(&child.args, argv);

	إذا (dir_diff)
		أرجع run_dir_diff(repo, &dt_options, extcmd, prefix, &child);
	أرجع run_file_diff(prompt, prefix, &child);
}
