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

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "copy.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repository.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "entry.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	ਨਿਰਾ
};

ਬਣਤਰ difftool_options {
	ਪੂਰਨ has_symlinks;
	ਪੂਰਨ symlinks;
	ਪੂਰਨ trust_exit_ਕੋਡ;
};

ਸਥਿਰ ਪੂਰਨ difftool_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			   ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਬਣਤਰ difftool_options *dt_options = (ਬਣਤਰ difftool_options *)cb;
	ਜੇ (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_ਕੋਡ = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਪੂਰਨ print_tool_help(ਖਾਲੀ)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.ਦਲੀਲਾਂ, "mergetool", "--tool-help=diff", ਨਿਰਾ);
	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cmd);
}

ਸਥਿਰ ਪੂਰਨ parse_ਸੂਚਕਾਂਕ_info(ਬਣਤਰ repository *repo,
			    ਅੱਖਰ *p, ਪੂਰਨ *mode1, ਪੂਰਨ *mode2,
			    ਬਣਤਰ object_id *oid1, ਬਣਤਰ object_id *oid2,
			    ਅੱਖਰ *ਸਥਿਤੀ)
{
	ਜੇ (*p != ':')
		ਵਾਪਸ error("expected ':', got '%c'", *p);
	*mode1 = (ਪੂਰਨ)strtol(p + 1, &p, 8);
	ਜੇ (*p != ' ')
		ਵਾਪਸ error("expected ' ', got '%c'", *p);
	*mode2 = (ਪੂਰਨ)strtol(p + 1, &p, 8);
	ਜੇ (*p != ' ')
		ਵਾਪਸ error("expected ' ', got '%c'", *p);
	ਜੇ (parse_oid_hex_algop(++p, oid1, (ਅਚਲ ਅੱਖਰ **)&p, repo->hash_algo))
		ਵਾਪਸ error("expected object ID, got '%s'", p);
	ਜੇ (*p != ' ')
		ਵਾਪਸ error("expected ' ', got '%c'", *p);
	ਜੇ (parse_oid_hex_algop(++p, oid2, (ਅਚਲ ਅੱਖਰ **)&p, repo->hash_algo))
		ਵਾਪਸ error("expected object ID, got '%s'", p);
	ਜੇ (*p != ' ')
		ਵਾਪਸ error("expected ' ', got '%c'", *p);
	*ਸਥਿਤੀ = *++p;
	ਜੇ (!*ਸਥਿਤੀ)
		ਵਾਪਸ error("missing status");
	ਜੇ (p[1] && !ਅੰਕ_ਹੈ(p[1]))
		ਵਾਪਸ error("unexpected trailer: '%s'", p + 1);
	ਵਾਪਸ 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਰਸਤਾ(ਬਣਤਰ strbuf *buf, ਮਾਪ_t ਅਧਾਰ_len, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	strbuf_setlen(buf, ਅਧਾਰ_len);
	ਜੇ (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, ਰਸਤਾ);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
ਸਥਿਰ ਪੂਰਨ use_wt_file(ਬਣਤਰ repository *repo,
		       ਅਚਲ ਅੱਖਰ *workdir, ਅਚਲ ਅੱਖਰ *name,
		       ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ stat st;
	ਪੂਰਨ use = 0;

	strbuf_addstr(&buf, workdir);
	ਜੋੜੋ_ਰਸਤਾ(&buf, buf.len, name);

	ਜੇ (!lstat(buf.buf, &st) && !S_ISLNK(st.st_ਬਹੁਲਕ)) {
		ਬਣਤਰ object_id wt_oid;
		ਪੂਰਨ fd = open(buf.buf, O_RDONLY);

		ਜੇ (fd >= 0 &&
		    !ਸੂਚਕਾਂਕ_fd(repo->ਸੂਚਕਾਂਕ, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			ਜੇ (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	ਵਾਪਸ use;
}

ਬਣਤਰ working_ਰੁੱਖ_ਇੰਦਰਾਜ {
	ਬਣਤਰ hashmap_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
	ਅੱਖਰ ਰਸਤਾ[FLEX_ARRAY];
};

ਸਥਿਰ ਪੂਰਨ working_ਰੁੱਖ_ਇੰਦਰਾਜ_cmp(ਅਚਲ ਖਾਲੀ *cmp_ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
				  ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *eptr,
				  ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ_or_ਕੁੰਜੀ,
				  ਅਚਲ ਖਾਲੀ *keydata ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਬਣਤਰ working_ਰੁੱਖ_ਇੰਦਰਾਜ *a, *b;

	a = container_ਦਾ(eptr, ਅਚਲ ਬਣਤਰ working_ਰੁੱਖ_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);
	b = container_ਦਾ(ਇੰਦਰਾਜ_or_ਕੁੰਜੀ, ਅਚਲ ਬਣਤਰ working_ਰੁੱਖ_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);

	ਵਾਪਸ strcmp(a->ਰਸਤਾ, b->ਰਸਤਾ);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
ਬਣਤਰ pair_ਇੰਦਰਾਜ {
	ਬਣਤਰ hashmap_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
	ਅੱਖਰ ਖੱਬਾ[ਰਸਤਾ_MAX], ਸੱਜਾ[ਰਸਤਾ_MAX];
	ਅਚਲ ਅੱਖਰ ਰਸਤਾ[FLEX_ARRAY];
};

ਸਥਿਰ ਪੂਰਨ pair_cmp(ਅਚਲ ਖਾਲੀ *cmp_ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
		    ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *eptr,
		    ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ_or_ਕੁੰਜੀ,
		    ਅਚਲ ਖਾਲੀ *keydata ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਬਣਤਰ pair_ਇੰਦਰਾਜ *a, *b;

	a = container_ਦਾ(eptr, ਅਚਲ ਬਣਤਰ pair_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);
	b = container_ਦਾ(ਇੰਦਰਾਜ_or_ਕੁੰਜੀ, ਅਚਲ ਬਣਤਰ pair_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);

	ਵਾਪਸ strcmp(a->ਰਸਤਾ, b->ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਖੱਬਾ_or_ਸੱਜਾ(ਬਣਤਰ hashmap *map, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
			      ਅਚਲ ਅੱਖਰ *content, ਪੂਰਨ is_ਸੱਜਾ)
{
	ਬਣਤਰ pair_ਇੰਦਰਾਜ *e, *existing;

	FLEX_ALLOC_STR(e, ਰਸਤਾ, ਰਸਤਾ);
	hashmap_ਇੰਦਰਾਜ_init(&e->ਇੰਦਰਾਜ, strhash(ਰਸਤਾ));
	existing = hashmap_get_ਇੰਦਰਾਜ(map, e, ਇੰਦਰਾਜ, ਨਿਰਾ);
	ਜੇ (existing) {
		ਮੁਕਤ(e);
		e = existing;
	} ਨਹੀਂਤਾਂ {
		e->ਖੱਬਾ[0] = e->ਸੱਜਾ[0] = '\0';
		hashmap_ਜੋੜੋ(map, &e->ਇੰਦਰਾਜ);
	}
	strlcpy(is_ਸੱਜਾ ? e->ਸੱਜਾ : e->ਖੱਬਾ, content, ਰਸਤਾ_MAX);
}

ਬਣਤਰ ਰਸਤਾ_ਇੰਦਰਾਜ {
	ਬਣਤਰ hashmap_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
	ਅੱਖਰ ਰਸਤਾ[FLEX_ARRAY];
};

ਸਥਿਰ ਪੂਰਨ ਰਸਤਾ_ਇੰਦਰਾਜ_cmp(ਅਚਲ ਖਾਲੀ *cmp_ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
			  ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *eptr,
			  ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ_or_ਕੁੰਜੀ,
			  ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ)
{
	ਅਚਲ ਬਣਤਰ ਰਸਤਾ_ਇੰਦਰਾਜ *a, *b;

	a = container_ਦਾ(eptr, ਅਚਲ ਬਣਤਰ ਰਸਤਾ_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);
	b = container_ਦਾ(ਇੰਦਰਾਜ_or_ਕੁੰਜੀ, ਅਚਲ ਬਣਤਰ ਰਸਤਾ_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);

	ਵਾਪਸ strcmp(a->ਰਸਤਾ, ਕੁੰਜੀ ? ਕੁੰਜੀ : b->ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ ਬਦਲਿਆ_files(ਬਣਤਰ repository *repo,
			  ਬਣਤਰ hashmap *ਨਤੀਜਾ, ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_ਰਸਤਾ,
			  ਅਚਲ ਅੱਖਰ *workdir)
{
	ਬਣਤਰ ਬੱਚਾ_process update_ਸੂਚਕਾਂਕ = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ ਬੱਚਾ_process diff_files = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *git_dir = absolute_ਰਸਤਾ(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_ਸੂਚਕਾਂਕ.ਦਲੀਲਾਂ,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", ਨਿਰਾ);
	update_ਸੂਚਕਾਂਕ.no_stdin = 1;
	update_ਸੂਚਕਾਂਕ.no_stdout = 1;
	update_ਸੂਚਕਾਂਕ.no_stderr = 1;
	update_ਸੂਚਕਾਂਕ.git_cmd = 1;
	update_ਸੂਚਕਾਂਕ.use_shell = 0;
	update_ਸੂਚਕਾਂਕ.clean_on_exit = 1;
	update_ਸੂਚਕਾਂਕ.dir = workdir;
	strvec_pushf(&update_ਸੂਚਕਾਂਕ.env, "GIT_INDEX_FILE=%s", ਸੂਚਕਾਂਕ_ਰਸਤਾ);
	/* Ignore any errors of update-index */
	ਲੜੀਵਾਰ_command(&update_ਸੂਚਕਾਂਕ);

	strvec_pushl(&diff_files.ਦਲੀਲਾਂ,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "diff-files", "--name-only", "-z", ਨਿਰਾ);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.ਬਾਹਰਲਾ = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", ਸੂਚਕਾਂਕ_ਰਸਤਾ);
	ਜੇ (ਸ਼ੁਰੂ_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.ਬਾਹਰਲਾ, "r");
	ਜਦੋਂ (!strbuf_getline_nul(&buf, fp)) {
		ਬਣਤਰ ਰਸਤਾ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
		FLEX_ALLOC_STR(ਇੰਦਰਾਜ, ਰਸਤਾ, buf.buf);
		hashmap_ਇੰਦਰਾਜ_init(&ਇੰਦਰਾਜ->ਇੰਦਰਾਜ, strhash(buf.buf));
		hashmap_ਜੋੜੋ(ਨਤੀਜਾ, &ਇੰਦਰਾਜ->ਇੰਦਰਾਜ);
	}
	fclose(fp);
	ਜੇ (ਅੰਤ_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

ਸਥਿਰ ਪੂਰਨ ensure_leading_directories(ਬਣਤਰ repository *repo, ਅੱਖਰ *ਰਸਤਾ)
{
	ਚੋਣ (ਸੁਰੱਖਿਅਤ_create_leading_directories(repo, ਰਸਤਾ)) {
		ਹਾਲਤ SCLD_OK:
		ਹਾਲਤ SCLD_EXISTS:
			ਵਾਪਸ 0;
		ਮੂਲ:
			ਵਾਪਸ error(_("could not create leading directories "
				       "of '%s'"), ਰਸਤਾ);
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
ਸਥਿਰ ਅੱਖਰ *get_symlink(ਬਣਤਰ repository *repo,
			 ਬਣਤਰ difftool_options *dt_options,
			 ਅਚਲ ਬਣਤਰ object_id *oid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਅੱਖਰ *ਅੰਕੜੇ;
	ਜੇ (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		ਬਣਤਰ strbuf link = STRBUF_INIT;
		ਜੇ (dt_options->has_symlinks) {
			ਜੇ (strbuf_readlink(&link, ਰਸਤਾ, ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ)))
				die(_("could not read symlink %s"), ਰਸਤਾ);
		} ਨਹੀਂਤਾਂ ਜੇ (strbuf_read_file(&link, ਰਸਤਾ, 128))
			die(_("could not read symlink file %s"), ਰਸਤਾ);

		ਅੰਕੜੇ = strbuf_detach(&link, ਨਿਰਾ);
	} ਨਹੀਂਤਾਂ {
		ਗਿਣਤੀ object_type type;
		ਮਾਪ_t ਮਾਪ;
		ਅੰਕੜੇ = odb_read_object(repo->objects, oid, &type, &ਮਾਪ);
		ਜੇ (!ਅੰਕੜੇ)
			die(_("could not read object %s for symlink %s"),
				oid_ਮੰਜ਼ਿਲ_hex(oid), ਰਸਤਾ);
	}

	ਵਾਪਸ ਅੰਕੜੇ;
}

ਸਥਿਰ ਪੂਰਨ checkout_ਰਸਤਾ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਬਣਤਰ object_id *oid,
			 ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਬਣਤਰ checkout *state)
{
	ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;
	ਪੂਰਨ ret;

	ce = make_transient_cache_ਇੰਦਰਾਜ(ਬਹੁਲਕ, oid, ਰਸਤਾ, 0, ਨਿਰਾ);
	ret = checkout_ਇੰਦਰਾਜ(ce, state, ਨਿਰਾ, ਨਿਰਾ);

	discard_cache_ਇੰਦਰਾਜ(ce);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ write_file_ਵਿੱਚ_directory(ਬਣਤਰ repository *repo,
				    ਬਣਤਰ strbuf *dir, ਮਾਪ_t dir_len,
				    ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *content)
{
	ਜੋੜੋ_ਰਸਤਾ(dir, dir_len, ਰਸਤਾ);
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
ਸਥਿਰ ਖਾਲੀ write_standin_files(ਬਣਤਰ repository *repo,
				ਬਣਤਰ pair_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
				ਬਣਤਰ strbuf *ldir, ਮਾਪ_t ldir_len,
				ਬਣਤਰ strbuf *rdir, ਮਾਪ_t rdir_len)
{
	ਜੇ (*ਇੰਦਰਾਜ->ਖੱਬਾ)
		write_file_ਵਿੱਚ_directory(repo, ldir, ldir_len, ਇੰਦਰਾਜ->ਰਸਤਾ, ਇੰਦਰਾਜ->ਖੱਬਾ);
	ਜੇ (*ਇੰਦਰਾਜ->ਸੱਜਾ)
		write_file_ਵਿੱਚ_directory(repo, rdir, rdir_len, ਇੰਦਰਾਜ->ਰਸਤਾ, ਇੰਦਰਾਜ->ਸੱਜਾ);
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_dir_diff(ਬਣਤਰ repository *repo,
			ਬਣਤਰ difftool_options *dt_options,
			ਅਚਲ ਅੱਖਰ *extcmd, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			ਬਣਤਰ ਬੱਚਾ_process *ਬੱਚਾ)
{
	ਬਣਤਰ strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	ਬਣਤਰ strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	ਬਣਤਰ strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	ਬਣਤਰ strbuf wtdir = STRBUF_INIT;
	ਬਣਤਰ strbuf tmpdir = STRBUF_INIT;
	ਅੱਖਰ *lbase_dir = ਨਿਰਾ, *rbase_dir = ਨਿਰਾ;
	ਮਾਪ_t ldir_len, rdir_len, wtdir_len;
	ਅਚਲ ਅੱਖਰ *workdir, *tmp;
	ਪੂਰਨ ret = 0;
	ਮਾਪ_t i;
	FILE *fp = ਨਿਰਾ;
	ਬਣਤਰ hashmap working_ਰੁੱਖ_dups = HASHMAP_INIT(working_ਰੁੱਖ_ਇੰਦਰਾਜ_cmp,
							ਨਿਰਾ);
	ਬਣਤਰ hashmap submodules = HASHMAP_INIT(pair_cmp, ਨਿਰਾ);
	ਬਣਤਰ hashmap symlinks2 = HASHMAP_INIT(pair_cmp, ਨਿਰਾ);
	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ pair_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
	ਬਣਤਰ ਸੂਚਕਾਂਕ_state wtindex = ਸੂਚਕਾਂਕ_STATE_INIT(repo);
	ਬਣਤਰ checkout lstate, rstate;
	ਪੂਰਨ err = 0;
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ hashmap wt_modified = HASHMAP_INIT(ਰਸਤਾ_ਇੰਦਰਾਜ_cmp, ਨਿਰਾ);
	ਬਣਤਰ hashmap tmp_modified = HASHMAP_INIT(ਰਸਤਾ_ਇੰਦਰਾਜ_cmp, ਨਿਰਾ);
	ਪੂਰਨ indices_loaded = 0;

	workdir = repo_get_work_ਰੁੱਖ(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_ਜੋੜੋ_absolute_ਰਸਤਾ(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	ਜੇ (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		ਜਾਓ ਅੰਤ;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	ਜੇ (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, ਆਕਾਰ(lstate));
	lstate.ਅਧਾਰ_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.ਅਧਾਰ_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, ਆਕਾਰ(rstate));
	rstate.ਅਧਾਰ_dir = rbase_dir = xstrdup(rdir.buf);
	rstate.ਅਧਾਰ_dir_len = rdir.len;
	rstate.force = 1;

	ldir_len = ldir.len;
	rdir_len = rdir.len;
	wtdir_len = wtdir.len;

	ਬੱਚਾ->no_stdin = 1;
	ਬੱਚਾ->git_cmd = 1;
	ਬੱਚਾ->use_shell = 0;
	ਬੱਚਾ->clean_on_exit = 1;
	ਬੱਚਾ->dir = ਅਗੇਤਰ;
	ਬੱਚਾ->ਬਾਹਰਲਾ = -1;
	ਜੇ (ਸ਼ੁਰੂ_command(ਬੱਚਾ))
		die("could not obtain raw diff");
	fp = xfdopen(ਬੱਚਾ->ਬਾਹਰਲਾ, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	ਜਦੋਂ (!strbuf_getline_nul(&info, fp)) {
		ਪੂਰਨ lmode, rmode;
		ਬਣਤਰ object_id loid, roid;
		ਅੱਖਰ ਸਥਿਤੀ;
		ਅਚਲ ਅੱਖਰ *src_ਰਸਤਾ, *dst_ਰਸਤਾ;

		ਜੇ (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		ਜੇ (parse_ਸੂਚਕਾਂਕ_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &ਸਥਿਤੀ))
			ਰੋਕੋ;
		ਜੇ (strbuf_getline_nul(&lpath, fp))
			ਰੋਕੋ;
		src_ਰਸਤਾ = lpath.buf;

		i++;
		ਜੇ (ਸਥਿਤੀ != 'C' && ਸਥਿਤੀ != 'R') {
			dst_ਰਸਤਾ = src_ਰਸਤਾ;
		} ਨਹੀਂਤਾਂ {
			ਜੇ (strbuf_getline_nul(&rpath, fp))
				ਰੋਕੋ;
			dst_ਰਸਤਾ = rpath.buf;
		}

		ਜੇ (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_ਮੰਜ਼ਿਲ_hex(&loid));
			ਜੋੜੋ_ਖੱਬਾ_or_ਸੱਜਾ(&submodules, src_ਰਸਤਾ, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_ਮੰਜ਼ਿਲ_hex(&roid));
			ਜੇ (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			ਜੋੜੋ_ਖੱਬਾ_or_ਸੱਜਾ(&submodules, dst_ਰਸਤਾ, buf.buf, 1);
			ਜਾਰੀ;
		}

		ਜੇ (S_ISLNK(lmode)) {
			ਅੱਖਰ *content = get_symlink(repo, dt_options, &loid, src_ਰਸਤਾ);
			ਜੋੜੋ_ਖੱਬਾ_or_ਸੱਜਾ(&symlinks2, src_ਰਸਤਾ, content, 0);
			ਮੁਕਤ(content);
		}

		ਜੇ (S_ISLNK(rmode)) {
			ਅੱਖਰ *content = get_symlink(repo, dt_options, &roid, dst_ਰਸਤਾ);
			ਜੋੜੋ_ਖੱਬਾ_or_ਸੱਜਾ(&symlinks2, dst_ਰਸਤਾ, content, 1);
			ਮੁਕਤ(content);
		}

		ਜੇ (lmode && ਸਥਿਤੀ != 'C') {
			ਜੇ (checkout_ਰਸਤਾ(lmode, &loid, src_ਰਸਤਾ, &lstate)) {
				ret = error("could not write '%s'", src_ਰਸਤਾ);
				ਜਾਓ ਅੰਤ;
			}
		}

		ਜੇ (rmode && !S_ISLNK(rmode)) {
			ਬਣਤਰ working_ਰੁੱਖ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(ਇੰਦਰਾਜ, ਰਸਤਾ, dst_ਰਸਤਾ);
			hashmap_ਇੰਦਰਾਜ_init(&ਇੰਦਰਾਜ->ਇੰਦਰਾਜ, strhash(dst_ਰਸਤਾ));
			ਜੇ (hashmap_get(&working_ਰੁੱਖ_dups, &ਇੰਦਰਾਜ->ਇੰਦਰਾਜ,
					ਨਿਰਾ)) {
				ਮੁਕਤ(ਇੰਦਰਾਜ);
				ਜਾਰੀ;
			}
			hashmap_ਜੋੜੋ(&working_ਰੁੱਖ_dups, &ਇੰਦਰਾਜ->ਇੰਦਰਾਜ);

			ਜੇ (!use_wt_file(repo, workdir, dst_ਰਸਤਾ, &roid)) {
				ਜੇ (checkout_ਰਸਤਾ(rmode, &roid, dst_ਰਸਤਾ,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_ਰਸਤਾ);
					ਜਾਓ ਅੰਤ;
				}
			} ਨਹੀਂਤਾਂ ਜੇ (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce2 =
					make_cache_ਇੰਦਰਾਜ(&wtindex, rmode, &roid,
							 dst_ਰਸਤਾ, 0, 0);

				ਜੋੜੋ_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ(&wtindex, ce2,
						ਜੋੜੋ_CACHE_JUST_APPEND);

				ਜੋੜੋ_ਰਸਤਾ(&rdir, rdir_len, dst_ਰਸਤਾ);
				ਜੇ (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_ਰਸਤਾ);
					ਜਾਓ ਅੰਤ;
				}
				ਜੋੜੋ_ਰਸਤਾ(&wtdir, wtdir_len, dst_ਰਸਤਾ);
				ਜੇ (dt_options->symlinks) {
					ਜੇ (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						ਜਾਓ ਅੰਤ;
					}
				} ਨਹੀਂਤਾਂ {
					ਬਣਤਰ stat st;
					ਜੇ (stat(wtdir.buf, &st))
						st.st_ਬਹੁਲਕ = 0644;
					ਜੇ (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_ਬਹੁਲਕ)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						ਜਾਓ ਅੰਤ;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = ਨਿਰਾ;
	ਜੇ (ਅੰਤ_command(ਬੱਚਾ)) {
		ret = error("error occurred running diff --raw");
		ਜਾਓ ਅੰਤ;
	}

	ਜੇ (!i)
		ਜਾਓ ਅੰਤ;

	/*
	 * Changes to submodules require special treatment.This loop writes a
	 * temporary file to both the left and right directories to show the
	 * change in the recorded SHA1 for the submodule.
	 */
	hashmap_for_each_ਇੰਦਰਾਜ(&submodules, &iter, ਇੰਦਰਾਜ,
				ਇੰਦਰਾਜ /* member name */) {
		write_standin_files(repo, ਇੰਦਰਾਜ, &ldir, ldir_len, &rdir, rdir_len);
	}

	/*
	 * Symbolic links require special treatment. The standard "git diff"
	 * shows only the link itself, not the contents of the link target.
	 * This loop replicates that behavior.
	 */
	hashmap_for_each_ਇੰਦਰਾਜ(&symlinks2, &iter, ਇੰਦਰਾਜ,
				ਇੰਦਰਾਜ /* member name */) {

		write_standin_files(repo, ਇੰਦਰਾਜ, &ldir, ldir_len, &rdir, rdir_len);
	}

	strbuf_setlen(&ldir, ldir_len);
	strbuf_setlen(&rdir, rdir_len);

	ਜੇ (extcmd) {
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, extcmd);
	} ਨਹੀਂਤਾਂ {
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.ਦਲੀਲਾਂ, ldir.buf, rdir.buf, ਨਿਰਾ);
	ret = ਲੜੀਵਾਰ_command(&cmd);

	/* TODO: audit for interaction with sparse-index. */
	ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(&wtindex);

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
	ਲਈ (i = 0; i < wtindex.cache_nr; i++) {
		ਬਣਤਰ hashmap_ਇੰਦਰਾਜ dummy;
		ਅਚਲ ਅੱਖਰ *name = wtindex.cache[i]->name;
		ਬਣਤਰ stat st;

		ਜੋੜੋ_ਰਸਤਾ(&rdir, rdir_len, name);
		ਜੇ (lstat(rdir.buf, &st))
			ਜਾਰੀ;

		ਜੇ ((dt_options->symlinks && S_ISLNK(st.st_ਬਹੁਲਕ)) || !S_ISREG(st.st_ਬਹੁਲਕ))
			ਜਾਰੀ;

		ਜੇ (!indices_loaded) {
			ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ = ਤਾਲਾ_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			ਜੇ (repo_hold_ਤਾਲਾ_file_for_update(repo, &ਤਾਲਾ, buf.buf, 0) < 0 ||
			    write_locked_ਸੂਚਕਾਂਕ(&wtindex, &ਤਾਲਾ, COMMIT_ਤਾਲਾ)) {
				ret = error("could not write %s", buf.buf);
				ਜਾਓ ਅੰਤ;
			}
			ਬਦਲਿਆ_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			ਬਦਲਿਆ_files(repo, &tmp_modified, buf.buf, rdir.buf);
			ਜੋੜੋ_ਰਸਤਾ(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_ਇੰਦਰਾਜ_init(&dummy, strhash(name));
		ਜੇ (hashmap_get(&tmp_modified, &dummy, name)) {
			ਜੋੜੋ_ਰਸਤਾ(&wtdir, wtdir_len, name);
			ਜੇ (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_ਬਹੁਲਕ))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	ਜੇ (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} ਨਹੀਂਤਾਂ {
		remove_dir_recursively(&tmpdir, 0);
		ਜੇ (ret)
			warning(_("failed: %d"), ret);
	}

ਅੰਤ:
	ਜੇ (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_ਰੁੱਖ_dups, ਬਣਤਰ working_ਰੁੱਖ_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);
	hashmap_clear_and_free(&wt_modified, ਬਣਤਰ ਰਸਤਾ_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);
	hashmap_clear_and_free(&tmp_modified, ਬਣਤਰ ਰਸਤਾ_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);
	hashmap_clear_and_free(&submodules, ਬਣਤਰ pair_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);
	hashmap_clear_and_free(&symlinks2, ਬਣਤਰ pair_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);
	release_ਸੂਚਕਾਂਕ(&wtindex);
	ਮੁਕਤ(lbase_dir);
	ਮੁਕਤ(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	ਵਾਪਸ (ret < 0) ? 1 : ret;
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_file_diff(ਪੂਰਨ prompt, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ ਬੱਚਾ_process *ਬੱਚਾ)
{
	strvec_ਧੱਕੋ(&ਬੱਚਾ->env, "GIT_PAGER=");
	strvec_ਧੱਕੋ(&ਬੱਚਾ->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	ਜੇ (prompt > 0)
		strvec_ਧੱਕੋ(&ਬੱਚਾ->env, "GIT_DIFFTOOL_PROMPT=true");
	ਨਹੀਂਤਾਂ ਜੇ (!prompt)
		strvec_ਧੱਕੋ(&ਬੱਚਾ->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	ਬੱਚਾ->git_cmd = 1;
	ਬੱਚਾ->dir = ਅਗੇਤਰ;

	ਵਾਪਸ ਲੜੀਵਾਰ_command(ਬੱਚਾ);
}

ਪੂਰਨ cmd_difftool(ਪੂਰਨ argc,
		 ਅਚਲ ਅੱਖਰ **argv,
		 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_ਸੂਚਕਾਂਕ = 0;
	ਸਥਿਰ ਅੱਖਰ *difftool_cmd = ਨਿਰਾ, *extcmd = ਨਿਰਾ;
	ਬਣਤਰ difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_ਕੋਡ = 0
	};
	ਬਣਤਰ option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_ਸੈੱਟ_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT_F(0, "prompt", &prompt, ਨਿਰਾ,
			1, PARSE_OPT_NONEG | PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "symlinks", &dt_options.symlinks,
			 N_("use symlinks in dir-diff mode")),
		OPT_STRING('t', "tool", &difftool_cmd, N_("tool"),
			   N_("use the specified diff tool")),
		OPT_BOOL(0, "tool-help", &tool_help,
			 N_("print a list of diff tools that may be used with "
			    "`--tool`")),
		OPT_BOOL(0, "trust-exit-code", &dt_options.trust_exit_ਕੋਡ,
			 N_("make 'git-difftool' exit when an invoked diff "
			    "tool returns a non-zero exit code")),
		OPT_STRING('x', "extcmd", &extcmd, N_("command"),
			   N_("specify a custom command for viewing diffs")),
		OPT_BOOL(0, "no-index", &no_ਸੂਚਕਾਂਕ, N_("passed to `diff`")),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ ਬੱਚਾ_process ਬੱਚਾ = ਬੱਚਾ_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	ਜੇ (tool_help)
		ਵਾਪਸ print_tool_help();

	ਜੇ (!no_ਸੂਚਕਾਂਕ && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	ਜੇ (!no_ਸੂਚਕਾਂਕ){
		ਤਿਆਰੀ_work_ਰੁੱਖ(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_ਰਸਤਾ(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_ਰੁੱਖ_ENVIRONMENT, absolute_ਰਸਤਾ(repo_get_work_ਰੁੱਖ(repo)), 1);
	} ਨਹੀਂਤਾਂ ਜੇ (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	ਜੇ (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	ਨਹੀਂਤਾਂ ਜੇ (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	ਜੇ (difftool_cmd) {
		ਜੇ (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		ਨਹੀਂਤਾਂ
			die(_("no <tool> given for --tool=<tool>"));
	}

	ਜੇ (extcmd) {
		ਜੇ (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		ਨਹੀਂਤਾਂ
			die(_("no <cmd> given for --extcmd=<cmd>"));
	}

	setenv("GIT_DIFFTOOL_TRUST_EXIT_CODE",
	       dt_options.trust_exit_ਕੋਡ ? "true" : "false", 1);

	/*
	 * In directory diff mode, 'git-difftool--helper' is called once
	 * to compare the a / b directories. In file diff mode, 'git diff'
	 * will invoke a separate instance of 'git-difftool--helper' for
	 * each file that changed.
	 */
	strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "diff");
	ਜੇ (no_ਸੂਚਕਾਂਕ)
		strvec_ਧੱਕੋ(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--no-index");
	ਜੇ (dir_diff)
		strvec_pushl(&ਬੱਚਾ.ਦਲੀਲਾਂ, "--raw", "--no-abbrev", "-z", ਨਿਰਾ);
	strvec_pushv(&ਬੱਚਾ.ਦਲੀਲਾਂ, argv);

	ਜੇ (dir_diff)
		ਵਾਪਸ ਲੜੀਵਾਰ_dir_diff(repo, &dt_options, extcmd, ਅਗੇਤਰ, &ਬੱਚਾ);
	ਵਾਪਸ ਲੜੀਵਾਰ_file_diff(prompt, ਅਗੇਤਰ, &ਬੱਚਾ);
}
