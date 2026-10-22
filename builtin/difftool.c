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

#ಅನ್ತರ್ಭಾವ "builtin.h"

#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "copy.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "repository.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "entry.h"
#ಅನ್ತರ್ಭಾವ "setup.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	ರಿಕ್ತ
};

ಸಂರಚನಾ difftool_options {
	ಪೂರ್ಣಾಙ್ಕ has_symlinks;
	ಪೂರ್ಣಾಙ್ಕ symlinks;
	ಪೂರ್ಣಾಙ್ಕ trust_exit_code;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ difftool_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			   ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ difftool_options *dt_options = (ಸಂರಚನಾ difftool_options *)cb;
	ಯದಿ (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_code = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_tool_help(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "mergetool", "--tool-help=diff", ರಿಕ್ತ);
	ಪ್ರತ್ಯಾಗಮ run_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_index_info(ಸಂರಚನಾ repository *repo,
			    ವರ್ಣ *p, ಪೂರ್ಣಾಙ್ಕ *mode1, ಪೂರ್ಣಾಙ್ಕ *mode2,
			    ಸಂರಚನಾ object_id *oid1, ಸಂರಚನಾ object_id *oid2,
			    ವರ್ಣ *status)
{
	ಯದಿ (*p != ':')
		ಪ್ರತ್ಯಾಗಮ error("expected ':', got '%c'", *p);
	*mode1 = (ಪೂರ್ಣಾಙ್ಕ)strtol(p + 1, &p, 8);
	ಯದಿ (*p != ' ')
		ಪ್ರತ್ಯಾಗಮ error("expected ' ', got '%c'", *p);
	*mode2 = (ಪೂರ್ಣಾಙ್ಕ)strtol(p + 1, &p, 8);
	ಯದಿ (*p != ' ')
		ಪ್ರತ್ಯಾಗಮ error("expected ' ', got '%c'", *p);
	ಯದಿ (parse_oid_hex_algop(++p, oid1, (ಧ್ರುವ ವರ್ಣ **)&p, repo->hash_algo))
		ಪ್ರತ್ಯಾಗಮ error("expected object ID, got '%s'", p);
	ಯದಿ (*p != ' ')
		ಪ್ರತ್ಯಾಗಮ error("expected ' ', got '%c'", *p);
	ಯದಿ (parse_oid_hex_algop(++p, oid2, (ಧ್ರುವ ವರ್ಣ **)&p, repo->hash_algo))
		ಪ್ರತ್ಯಾಗಮ error("expected object ID, got '%s'", p);
	ಯದಿ (*p != ' ')
		ಪ್ರತ್ಯಾಗಮ error("expected ' ', got '%c'", *p);
	*status = *++p;
	ಯದಿ (!*status)
		ಪ್ರತ್ಯಾಗಮ error("missing status");
	ಯದಿ (p[1] && !isdigit(p[1]))
		ಪ್ರತ್ಯಾಗಮ error("unexpected trailer: '%s'", p + 1);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
ಸ್ಥಿರ ಶೂನ್ಯ add_path(ಸಂರಚನಾ strbuf *buf, size_t base_len, ಧ್ರುವ ವರ್ಣ *path)
{
	strbuf_setlen(buf, base_len);
	ಯದಿ (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, path);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_wt_file(ಸಂರಚನಾ repository *repo,
		       ಧ್ರುವ ವರ್ಣ *workdir, ಧ್ರುವ ವರ್ಣ *name,
		       ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ stat st;
	ಪೂರ್ಣಾಙ್ಕ use = 0;

	strbuf_addstr(&buf, workdir);
	add_path(&buf, buf.len, name);

	ಯದಿ (!lstat(buf.buf, &st) && !S_ISLNK(st.st_mode)) {
		ಸಂರಚನಾ object_id wt_oid;
		ಪೂರ್ಣಾಙ್ಕ fd = open(buf.buf, O_RDONLY);

		ಯದಿ (fd >= 0 &&
		    !index_fd(repo->index, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			ಯದಿ (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} ಅನ್ಯಥಾ ಯದಿ (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	ಪ್ರತ್ಯಾಗಮ use;
}

ಸಂರಚನಾ working_ವೃಕ್ಷ_entry {
	ಸಂರಚನಾ hashmap_entry entry;
	ವರ್ಣ path[FLEX_ARRAY];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ working_ವೃಕ್ಷ_entry_cmp(ಧ್ರುವ ಶೂನ್ಯ *cmp_data UNUSED,
				  ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *eptr,
				  ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *entry_or_ಕೀಲಿ,
				  ಧ್ರುವ ಶೂನ್ಯ *keydata UNUSED)
{
	ಧ್ರುವ ಸಂರಚನಾ working_ವೃಕ್ಷ_entry *a, *b;

	a = container_of(eptr, ಧ್ರುವ ಸಂರಚನಾ working_ವೃಕ್ಷ_entry, entry);
	b = container_of(entry_or_ಕೀಲಿ, ಧ್ರುವ ಸಂರಚನಾ working_ವೃಕ್ಷ_entry, entry);

	ಪ್ರತ್ಯಾಗಮ strcmp(a->path, b->path);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
ಸಂರಚನಾ pair_entry {
	ಸಂರಚನಾ hashmap_entry entry;
	ವರ್ಣ ಎಡ[PATH_MAX], ಬಲ[PATH_MAX];
	ಧ್ರುವ ವರ್ಣ path[FLEX_ARRAY];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pair_cmp(ಧ್ರುವ ಶೂನ್ಯ *cmp_data UNUSED,
		    ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *eptr,
		    ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *entry_or_ಕೀಲಿ,
		    ಧ್ರುವ ಶೂನ್ಯ *keydata UNUSED)
{
	ಧ್ರುವ ಸಂರಚನಾ pair_entry *a, *b;

	a = container_of(eptr, ಧ್ರುವ ಸಂರಚನಾ pair_entry, entry);
	b = container_of(entry_or_ಕೀಲಿ, ಧ್ರುವ ಸಂರಚನಾ pair_entry, entry);

	ಪ್ರತ್ಯಾಗಮ strcmp(a->path, b->path);
}

ಸ್ಥಿರ ಶೂನ್ಯ add_ಎಡ_or_ಬಲ(ಸಂರಚನಾ hashmap *map, ಧ್ರುವ ವರ್ಣ *path,
			      ಧ್ರುವ ವರ್ಣ *content, ಪೂರ್ಣಾಙ್ಕ is_ಬಲ)
{
	ಸಂರಚನಾ pair_entry *e, *existing;

	FLEX_ALLOC_STR(e, path, path);
	hashmap_entry_init(&e->entry, strhash(path));
	existing = hashmap_get_entry(map, e, entry, ರಿಕ್ತ);
	ಯದಿ (existing) {
		ಮುಕ್ತ(e);
		e = existing;
	} ಅನ್ಯಥಾ {
		e->ಎಡ[0] = e->ಬಲ[0] = '\0';
		hashmap_add(map, &e->entry);
	}
	strlcpy(is_ಬಲ ? e->ಬಲ : e->ಎಡ, content, PATH_MAX);
}

ಸಂರಚನಾ path_entry {
	ಸಂರಚನಾ hashmap_entry entry;
	ವರ್ಣ path[FLEX_ARRAY];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ path_entry_cmp(ಧ್ರುವ ಶೂನ್ಯ *cmp_data UNUSED,
			  ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *eptr,
			  ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *entry_or_ಕೀಲಿ,
			  ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ)
{
	ಧ್ರುವ ಸಂರಚನಾ path_entry *a, *b;

	a = container_of(eptr, ಧ್ರುವ ಸಂರಚನಾ path_entry, entry);
	b = container_of(entry_or_ಕೀಲಿ, ಧ್ರುವ ಸಂರಚನಾ path_entry, entry);

	ಪ್ರತ್ಯಾಗಮ strcmp(a->path, ಕೀಲಿ ? ಕೀಲಿ : b->path);
}

ಸ್ಥಿರ ಶೂನ್ಯ changed_files(ಸಂರಚನಾ repository *repo,
			  ಸಂರಚನಾ hashmap *result, ಧ್ರುವ ವರ್ಣ *index_path,
			  ಧ್ರುವ ವರ್ಣ *workdir)
{
	ಸಂರಚನಾ child_process update_index = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ child_process diff_files = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *git_dir = absolute_path(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_index.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", ರಿಕ್ತ);
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
		     "diff-files", "--name-only", "-z", ರಿಕ್ತ);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.out = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", index_path);
	ಯದಿ (start_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.out, "r");
	ಯಾವತ್ (!strbuf_getline_nul(&buf, fp)) {
		ಸಂರಚನಾ path_entry *entry;
		FLEX_ALLOC_STR(entry, path, buf.buf);
		hashmap_entry_init(&entry->entry, strhash(buf.buf));
		hashmap_add(result, &entry->entry);
	}
	fclose(fp);
	ಯದಿ (finish_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ensure_leading_directories(ಸಂರಚನಾ repository *repo, ವರ್ಣ *path)
{
	ವಿಕಲ್ಪ (safe_create_leading_directories(repo, path)) {
		ಪಕ್ಷ SCLD_OK:
		ಪಕ್ಷ SCLD_EXISTS:
			ಪ್ರತ್ಯಾಗಮ 0;
		ಸ್ವಭಾವ:
			ಪ್ರತ್ಯಾಗಮ error(_("could not create leading directories "
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
ಸ್ಥಿರ ವರ್ಣ *get_symlink(ಸಂರಚನಾ repository *repo,
			 ಸಂರಚನಾ difftool_options *dt_options,
			 ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಧ್ರುವ ವರ್ಣ *path)
{
	ವರ್ಣ *data;
	ಯದಿ (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		ಸಂರಚನಾ strbuf link = STRBUF_INIT;
		ಯದಿ (dt_options->has_symlinks) {
			ಯದಿ (strbuf_readlink(&link, path, strlen(path)))
				die(_("could not read symlink %s"), path);
		} ಅನ್ಯಥಾ ಯದಿ (strbuf_read_file(&link, path, 128))
			die(_("could not read symlink file %s"), path);

		data = strbuf_detach(&link, ರಿಕ್ತ);
	} ಅನ್ಯಥಾ {
		ಗಣನಾ object_type type;
		size_t size;
		data = odb_read_object(repo->objects, oid, &type, &size);
		ಯದಿ (!data)
			die(_("could not read object %s for symlink %s"),
				oid_to_hex(oid), path);
	}

	ಪ್ರತ್ಯಾಗಮ data;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ checkout_path(unsigned mode, ಸಂರಚನಾ object_id *oid,
			 ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ಸಂರಚನಾ checkout *state)
{
	ಸಂರಚನಾ cache_entry *ce;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ce = make_transient_cache_entry(mode, oid, path, 0, ರಿಕ್ತ);
	ret = checkout_entry(ce, state, ರಿಕ್ತ, ರಿಕ್ತ);

	discard_cache_entry(ce);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_file_in_directory(ಸಂರಚನಾ repository *repo,
				    ಸಂರಚನಾ strbuf *dir, size_t dir_len,
				    ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *content)
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
ಸ್ಥಿರ ಶೂನ್ಯ write_standin_files(ಸಂರಚನಾ repository *repo,
				ಸಂರಚನಾ pair_entry *entry,
				ಸಂರಚನಾ strbuf *ldir, size_t ldir_len,
				ಸಂರಚನಾ strbuf *rdir, size_t rdir_len)
{
	ಯದಿ (*entry->ಎಡ)
		write_file_in_directory(repo, ldir, ldir_len, entry->path, entry->ಎಡ);
	ಯದಿ (*entry->ಬಲ)
		write_file_in_directory(repo, rdir, rdir_len, entry->path, entry->ಬಲ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_dir_diff(ಸಂರಚನಾ repository *repo,
			ಸಂರಚನಾ difftool_options *dt_options,
			ಧ್ರುವ ವರ್ಣ *extcmd, ಧ್ರುವ ವರ್ಣ *prefix,
			ಸಂರಚನಾ child_process *child)
{
	ಸಂರಚನಾ strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	ಸಂರಚನಾ strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	ಸಂರಚನಾ strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	ಸಂರಚನಾ strbuf wtdir = STRBUF_INIT;
	ಸಂರಚನಾ strbuf tmpdir = STRBUF_INIT;
	ವರ್ಣ *lbase_dir = ರಿಕ್ತ, *rbase_dir = ರಿಕ್ತ;
	size_t ldir_len, rdir_len, wtdir_len;
	ಧ್ರುವ ವರ್ಣ *workdir, *tmp;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	size_t i;
	FILE *fp = ರಿಕ್ತ;
	ಸಂರಚನಾ hashmap working_ವೃಕ್ಷ_dups = HASHMAP_INIT(working_ವೃಕ್ಷ_entry_cmp,
							ರಿಕ್ತ);
	ಸಂರಚನಾ hashmap submodules = HASHMAP_INIT(pair_cmp, ರಿಕ್ತ);
	ಸಂರಚನಾ hashmap symlinks2 = HASHMAP_INIT(pair_cmp, ರಿಕ್ತ);
	ಸಂರಚನಾ hashmap_iter iter;
	ಸಂರಚನಾ pair_entry *entry;
	ಸಂರಚನಾ index_state wtindex = INDEX_STATE_INIT(repo);
	ಸಂರಚನಾ checkout lstate, rstate;
	ಪೂರ್ಣಾಙ್ಕ err = 0;
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ hashmap wt_modified = HASHMAP_INIT(path_entry_cmp, ರಿಕ್ತ);
	ಸಂರಚನಾ hashmap tmp_modified = HASHMAP_INIT(path_entry_cmp, ರಿಕ್ತ);
	ಪೂರ್ಣಾಙ್ಕ indices_loaded = 0;

	workdir = repo_get_work_ವೃಕ್ಷ(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_add_absolute_path(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	ಯದಿ (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		ಗಚ್ಛ finish;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	ಯದಿ (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, ಪರಿಮಾಣ(lstate));
	lstate.base_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.base_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, ಪರಿಮಾಣ(rstate));
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
	ಯದಿ (start_command(child))
		die("could not obtain raw diff");
	fp = xfdopen(child->out, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	ಯಾವತ್ (!strbuf_getline_nul(&info, fp)) {
		ಪೂರ್ಣಾಙ್ಕ lmode, rmode;
		ಸಂರಚನಾ object_id loid, roid;
		ವರ್ಣ status;
		ಧ್ರುವ ವರ್ಣ *src_path, *dst_path;

		ಯದಿ (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		ಯದಿ (parse_index_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &status))
			ಭಙ್ಗ;
		ಯದಿ (strbuf_getline_nul(&lpath, fp))
			ಭಙ್ಗ;
		src_path = lpath.buf;

		i++;
		ಯದಿ (status != 'C' && status != 'R') {
			dst_path = src_path;
		} ಅನ್ಯಥಾ {
			ಯದಿ (strbuf_getline_nul(&rpath, fp))
				ಭಙ್ಗ;
			dst_path = rpath.buf;
		}

		ಯದಿ (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&loid));
			add_ಎಡ_or_ಬಲ(&submodules, src_path, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&roid));
			ಯದಿ (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			add_ಎಡ_or_ಬಲ(&submodules, dst_path, buf.buf, 1);
			ಅನುವರ್ತ;
		}

		ಯದಿ (S_ISLNK(lmode)) {
			ವರ್ಣ *content = get_symlink(repo, dt_options, &loid, src_path);
			add_ಎಡ_or_ಬಲ(&symlinks2, src_path, content, 0);
			ಮುಕ್ತ(content);
		}

		ಯದಿ (S_ISLNK(rmode)) {
			ವರ್ಣ *content = get_symlink(repo, dt_options, &roid, dst_path);
			add_ಎಡ_or_ಬಲ(&symlinks2, dst_path, content, 1);
			ಮುಕ್ತ(content);
		}

		ಯದಿ (lmode && status != 'C') {
			ಯದಿ (checkout_path(lmode, &loid, src_path, &lstate)) {
				ret = error("could not write '%s'", src_path);
				ಗಚ್ಛ finish;
			}
		}

		ಯದಿ (rmode && !S_ISLNK(rmode)) {
			ಸಂರಚನಾ working_ವೃಕ್ಷ_entry *entry;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(entry, path, dst_path);
			hashmap_entry_init(&entry->entry, strhash(dst_path));
			ಯದಿ (hashmap_get(&working_ವೃಕ್ಷ_dups, &entry->entry,
					ರಿಕ್ತ)) {
				ಮುಕ್ತ(entry);
				ಅನುವರ್ತ;
			}
			hashmap_add(&working_ವೃಕ್ಷ_dups, &entry->entry);

			ಯದಿ (!use_wt_file(repo, workdir, dst_path, &roid)) {
				ಯದಿ (checkout_path(rmode, &roid, dst_path,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_path);
					ಗಚ್ಛ finish;
				}
			} ಅನ್ಯಥಾ ಯದಿ (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				ಸಂರಚನಾ cache_entry *ce2 =
					make_cache_entry(&wtindex, rmode, &roid,
							 dst_path, 0, 0);

				add_index_entry(&wtindex, ce2,
						ADD_CACHE_JUST_APPEND);

				add_path(&rdir, rdir_len, dst_path);
				ಯದಿ (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_path);
					ಗಚ್ಛ finish;
				}
				add_path(&wtdir, wtdir_len, dst_path);
				ಯದಿ (dt_options->symlinks) {
					ಯದಿ (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						ಗಚ್ಛ finish;
					}
				} ಅನ್ಯಥಾ {
					ಸಂರಚನಾ stat st;
					ಯದಿ (stat(wtdir.buf, &st))
						st.st_mode = 0644;
					ಯದಿ (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_mode)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						ಗಚ್ಛ finish;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = ರಿಕ್ತ;
	ಯದಿ (finish_command(child)) {
		ret = error("error occurred running diff --raw");
		ಗಚ್ಛ finish;
	}

	ಯದಿ (!i)
		ಗಚ್ಛ finish;

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

	ಯದಿ (extcmd) {
		strvec_ತಳ್ಳು(&cmd.args, extcmd);
	} ಅನ್ಯಥಾ {
		strvec_ತಳ್ಳು(&cmd.args, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.args, ldir.buf, rdir.buf, ರಿಕ್ತ);
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
	ಪ್ರತಿ (i = 0; i < wtindex.cache_nr; i++) {
		ಸಂರಚನಾ hashmap_entry dummy;
		ಧ್ರುವ ವರ್ಣ *name = wtindex.cache[i]->name;
		ಸಂರಚನಾ stat st;

		add_path(&rdir, rdir_len, name);
		ಯದಿ (lstat(rdir.buf, &st))
			ಅನುವರ್ತ;

		ಯದಿ ((dt_options->symlinks && S_ISLNK(st.st_mode)) || !S_ISREG(st.st_mode))
			ಅನುವರ್ತ;

		ಯದಿ (!indices_loaded) {
			ಸಂರಚನಾ lock_file lock = LOCK_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			ಯದಿ (repo_hold_lock_file_for_update(repo, &lock, buf.buf, 0) < 0 ||
			    write_locked_index(&wtindex, &lock, COMMIT_LOCK)) {
				ret = error("could not write %s", buf.buf);
				ಗಚ್ಛ finish;
			}
			changed_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			changed_files(repo, &tmp_modified, buf.buf, rdir.buf);
			add_path(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_entry_init(&dummy, strhash(name));
		ಯದಿ (hashmap_get(&tmp_modified, &dummy, name)) {
			add_path(&wtdir, wtdir_len, name);
			ಯದಿ (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} ಅನ್ಯಥಾ ಯದಿ (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_mode))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	ಯದಿ (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} ಅನ್ಯಥಾ {
		remove_dir_recursively(&tmpdir, 0);
		ಯದಿ (ret)
			warning(_("failed: %d"), ret);
	}

finish:
	ಯದಿ (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_ವೃಕ್ಷ_dups, ಸಂರಚನಾ working_ವೃಕ್ಷ_entry, entry);
	hashmap_clear_and_free(&wt_modified, ಸಂರಚನಾ path_entry, entry);
	hashmap_clear_and_free(&tmp_modified, ಸಂರಚನಾ path_entry, entry);
	hashmap_clear_and_free(&submodules, ಸಂರಚನಾ pair_entry, entry);
	hashmap_clear_and_free(&symlinks2, ಸಂರಚನಾ pair_entry, entry);
	release_index(&wtindex);
	ಮುಕ್ತ(lbase_dir);
	ಮುಕ್ತ(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	ಪ್ರತ್ಯಾಗಮ (ret < 0) ? 1 : ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_file_diff(ಪೂರ್ಣಾಙ್ಕ prompt, ಧ್ರುವ ವರ್ಣ *prefix,
			 ಸಂರಚನಾ child_process *child)
{
	strvec_ತಳ್ಳು(&child->env, "GIT_PAGER=");
	strvec_ತಳ್ಳು(&child->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	ಯದಿ (prompt > 0)
		strvec_ತಳ್ಳು(&child->env, "GIT_DIFFTOOL_PROMPT=true");
	ಅನ್ಯಥಾ ಯದಿ (!prompt)
		strvec_ತಳ್ಳು(&child->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	child->git_cmd = 1;
	child->dir = prefix;

	ಪ್ರತ್ಯಾಗಮ run_command(child);
}

ಪೂರ್ಣಾಙ್ಕ cmd_difftool(ಪೂರ್ಣಾಙ್ಕ argc,
		 ಧ್ರುವ ವರ್ಣ **argv,
		 ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_index = 0;
	ಸ್ಥಿರ ವರ್ಣ *difftool_cmd = ರಿಕ್ತ, *extcmd = ರಿಕ್ತ;
	ಸಂರಚನಾ difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_code = 0
	};
	ಸಂರಚನಾ option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_SET_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "prompt", &prompt, ರಿಕ್ತ,
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
	ಸಂರಚನಾ child_process child = CHILD_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, prefix, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	ಯದಿ (tool_help)
		ಪ್ರತ್ಯಾಗಮ print_tool_help();

	ಯದಿ (!no_index && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	ಯದಿ (!no_index){
		setup_work_ವೃಕ್ಷ(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_path(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT, absolute_path(repo_get_work_ವೃಕ್ಷ(repo)), 1);
	} ಅನ್ಯಥಾ ಯದಿ (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	ಯದಿ (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	ಅನ್ಯಥಾ ಯದಿ (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	ಯದಿ (difftool_cmd) {
		ಯದಿ (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		ಅನ್ಯಥಾ
			die(_("no <tool> given for --tool=<tool>"));
	}

	ಯದಿ (extcmd) {
		ಯದಿ (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		ಅನ್ಯಥಾ
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
	strvec_ತಳ್ಳು(&child.args, "diff");
	ಯದಿ (no_index)
		strvec_ತಳ್ಳು(&child.args, "--no-index");
	ಯದಿ (dir_diff)
		strvec_pushl(&child.args, "--raw", "--no-abbrev", "-z", ರಿಕ್ತ);
	strvec_pushv(&child.args, argv);

	ಯದಿ (dir_diff)
		ಪ್ರತ್ಯಾಗಮ run_dir_diff(repo, &dt_options, extcmd, prefix, &child);
	ಪ್ರತ್ಯಾಗಮ run_file_diff(prompt, prefix, &child);
}
