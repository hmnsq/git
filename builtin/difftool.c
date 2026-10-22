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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	ಖಾಲಿ
};

ರಚನೆ difftool_options {
	ಪೂರ್ಣಾಂಕ has_symlinks;
	ಪೂರ್ಣಾಂಕ symlinks;
	ಪೂರ್ಣಾಂಕ trust_exit_ಸಂಕೇತ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ difftool_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ರಚನೆ difftool_options *dt_options = (ರಚನೆ difftool_options *)cb;
	ಆದರೆ (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_ಸಂಕೇತ = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_tool_help(ಶೂನ್ಯ)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.ವಾದಗಳು, "mergetool", "--tool-help=diff", ಖಾಲಿ);
	ಹಿಂತಿರುಗಿ ಸತತ_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ಸೂಚ್ಯಂಕ_info(ರಚನೆ repository *repo,
			    ಅಕ್ಷರ *p, ಪೂರ್ಣಾಂಕ *mode1, ಪೂರ್ಣಾಂಕ *mode2,
			    ರಚನೆ object_id *oid1, ರಚನೆ object_id *oid2,
			    ಅಕ್ಷರ *ಸ್ಥಿತಿ_2)
{
	ಆದರೆ (*p != ':')
		ಹಿಂತಿರುಗಿ error("expected ':', got '%c'", *p);
	*mode1 = (ಪೂರ್ಣಾಂಕ)strtol(p + 1, &p, 8);
	ಆದರೆ (*p != ' ')
		ಹಿಂತಿರುಗಿ error("expected ' ', got '%c'", *p);
	*mode2 = (ಪೂರ್ಣಾಂಕ)strtol(p + 1, &p, 8);
	ಆದರೆ (*p != ' ')
		ಹಿಂತಿರುಗಿ error("expected ' ', got '%c'", *p);
	ಆದರೆ (parse_oid_hex_algop(++p, oid1, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **)&p, repo->hash_algo))
		ಹಿಂತಿರುಗಿ error("expected object ID, got '%s'", p);
	ಆದರೆ (*p != ' ')
		ಹಿಂತಿರುಗಿ error("expected ' ', got '%c'", *p);
	ಆದರೆ (parse_oid_hex_algop(++p, oid2, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **)&p, repo->hash_algo))
		ಹಿಂತಿರುಗಿ error("expected object ID, got '%s'", p);
	ಆದರೆ (*p != ' ')
		ಹಿಂತಿರುಗಿ error("expected ' ', got '%c'", *p);
	*ಸ್ಥಿತಿ_2 = *++p;
	ಆದರೆ (!*ಸ್ಥಿತಿ_2)
		ಹಿಂತಿರುಗಿ error("missing status");
	ಆದರೆ (p[1] && !ಅಂಕಿಯೇ(p[1]))
		ಹಿಂತಿರುಗಿ error("unexpected trailer: '%s'", p + 1);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ಮಾರ್ಗ(ರಚನೆ strbuf *buf, ಗಾತ್ರ_t ತಳ_len, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	strbuf_setlen(buf, ತಳ_len);
	ಆದರೆ (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, ಮಾರ್ಗ);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_wt_file(ರಚನೆ repository *repo,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *workdir, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
		       ರಚನೆ object_id *oid)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ stat st;
	ಪೂರ್ಣಾಂಕ use = 0;

	strbuf_addstr(&buf, workdir);
	ಕೂಡಿಸು_ಮಾರ್ಗ(&buf, buf.len, name);

	ಆದರೆ (!lstat(buf.buf, &st) && !S_ISLNK(st.st_ಬಹುಲಕ)) {
		ರಚನೆ object_id wt_oid;
		ಪೂರ್ಣಾಂಕ fd = open(buf.buf, O_RDONLY);

		ಆದರೆ (fd >= 0 &&
		    !ಸೂಚ್ಯಂಕ_fd(repo->ಸೂಚ್ಯಂಕ, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			ಆದರೆ (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	ಹಿಂತಿರುಗಿ use;
}

ರಚನೆ working_ವೃಕ್ಷ_ನಮೂದು {
	ರಚನೆ hashmap_ನಮೂದು ನಮೂದು;
	ಅಕ್ಷರ ಮಾರ್ಗ[FLEX_ARRAY];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ working_ವೃಕ್ಷ_ನಮೂದು_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *cmp_ದತ್ತಾಂಶ ಬಳಸದ,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *eptr,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *ನಮೂದು_or_ಕೀಲಿ,
				  ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *keydata ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ working_ವೃಕ್ಷ_ನಮೂದು *a, *b;

	a = container_ದ(eptr, ಸ್ಥಿರಾಂಕ ರಚನೆ working_ವೃಕ್ಷ_ನಮೂದು, ನಮೂದು);
	b = container_ದ(ನಮೂದು_or_ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ರಚನೆ working_ವೃಕ್ಷ_ನಮೂದು, ನಮೂದು);

	ಹಿಂತಿರುಗಿ strcmp(a->ಮಾರ್ಗ, b->ಮಾರ್ಗ);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
ರಚನೆ pair_ನಮೂದು {
	ರಚನೆ hashmap_ನಮೂದು ನಮೂದು;
	ಅಕ್ಷರ ಎಡ[ಮಾರ್ಗ_MAX], ಬಲ[ಮಾರ್ಗ_MAX];
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಮಾರ್ಗ[FLEX_ARRAY];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pair_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *cmp_ದತ್ತಾಂಶ ಬಳಸದ,
		    ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *eptr,
		    ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *ನಮೂದು_or_ಕೀಲಿ,
		    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *keydata ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ pair_ನಮೂದು *a, *b;

	a = container_ದ(eptr, ಸ್ಥಿರಾಂಕ ರಚನೆ pair_ನಮೂದು, ನಮೂದು);
	b = container_ದ(ನಮೂದು_or_ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ರಚನೆ pair_ನಮೂದು, ನಮೂದು);

	ಹಿಂತಿರುಗಿ strcmp(a->ಮಾರ್ಗ, b->ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ಎಡ_or_ಬಲ(ರಚನೆ hashmap *map, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *content, ಪೂರ್ಣಾಂಕ is_ಬಲ)
{
	ರಚನೆ pair_ನಮೂದು *e, *existing;

	FLEX_ALLOC_STR(e, ಮಾರ್ಗ, ಮಾರ್ಗ);
	hashmap_ನಮೂದು_init(&e->ನಮೂದು, strhash(ಮಾರ್ಗ));
	existing = hashmap_get_ನಮೂದು(map, e, ನಮೂದು, ಖಾಲಿ);
	ಆದರೆ (existing) {
		ಬಿಡುಗಡೆ(e);
		e = existing;
	} ಇಲ್ಲದಿದ್ದರೆ {
		e->ಎಡ[0] = e->ಬಲ[0] = '\0';
		hashmap_ಕೂಡಿಸು(map, &e->ನಮೂದು);
	}
	strlcpy(is_ಬಲ ? e->ಬಲ : e->ಎಡ, content, ಮಾರ್ಗ_MAX);
}

ರಚನೆ ಮಾರ್ಗ_ನಮೂದು {
	ರಚನೆ hashmap_ನಮೂದು ನಮೂದು;
	ಅಕ್ಷರ ಮಾರ್ಗ[FLEX_ARRAY];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮಾರ್ಗ_ನಮೂದು_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *cmp_ದತ್ತಾಂಶ ಬಳಸದ,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *eptr,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *ನಮೂದು_or_ಕೀಲಿ,
			  ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ ಮಾರ್ಗ_ನಮೂದು *a, *b;

	a = container_ದ(eptr, ಸ್ಥಿರಾಂಕ ರಚನೆ ಮಾರ್ಗ_ನಮೂದು, ನಮೂದು);
	b = container_ದ(ನಮೂದು_or_ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ರಚನೆ ಮಾರ್ಗ_ನಮೂದು, ನಮೂದು);

	ಹಿಂತಿರುಗಿ strcmp(a->ಮಾರ್ಗ, ಕೀಲಿ ? ಕೀಲಿ : b->ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಬದಲಾಯಿತು_files(ರಚನೆ repository *repo,
			  ರಚನೆ hashmap *ಫಲಿತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *workdir)
{
	ರಚನೆ ಮಗು_process update_ಸೂಚ್ಯಂಕ = ಮಗು_PROCESS_INIT;
	ರಚನೆ ಮಗು_process diff_files = ಮಗು_PROCESS_INIT;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *git_dir = absolute_ಮಾರ್ಗ(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_ಸೂಚ್ಯಂಕ.ವಾದಗಳು,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", ಖಾಲಿ);
	update_ಸೂಚ್ಯಂಕ.no_stdin = 1;
	update_ಸೂಚ್ಯಂಕ.no_stdout = 1;
	update_ಸೂಚ್ಯಂಕ.no_stderr = 1;
	update_ಸೂಚ್ಯಂಕ.git_cmd = 1;
	update_ಸೂಚ್ಯಂಕ.use_shell = 0;
	update_ಸೂಚ್ಯಂಕ.clean_on_exit = 1;
	update_ಸೂಚ್ಯಂಕ.dir = workdir;
	strvec_pushf(&update_ಸೂಚ್ಯಂಕ.env, "GIT_INDEX_FILE=%s", ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ);
	/* Ignore any errors of update-index */
	ಸತತ_command(&update_ಸೂಚ್ಯಂಕ);

	strvec_pushl(&diff_files.ವಾದಗಳು,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "diff-files", "--name-only", "-z", ಖಾಲಿ);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.ಹೊರಹೋಗು = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ);
	ಆದರೆ (ಆರಂಭ_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.ಹೊರಹೋಗು, "r");
	ಆಗ (!strbuf_getline_nul(&buf, fp)) {
		ರಚನೆ ಮಾರ್ಗ_ನಮೂದು *ನಮೂದು;
		FLEX_ALLOC_STR(ನಮೂದು, ಮಾರ್ಗ, buf.buf);
		hashmap_ನಮೂದು_init(&ನಮೂದು->ನಮೂದು, strhash(buf.buf));
		hashmap_ಕೂಡಿಸು(ಫಲಿತಾಂಶ, &ನಮೂದು->ನಮೂದು);
	}
	fclose(fp);
	ಆದರೆ (ಮುಕ್ತಾಯ_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ensure_leading_directories(ರಚನೆ repository *repo, ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಆಯ್ಕೆ (ಸುರಕ್ಷಿತ_create_leading_directories(repo, ಮಾರ್ಗ)) {
		ಸ್ಥಿತಿ SCLD_OK:
		ಸ್ಥಿತಿ SCLD_EXISTS:
			ಹಿಂತಿರುಗಿ 0;
		ಪೂರ್ವನಿಯೋಜಿತ:
			ಹಿಂತಿರುಗಿ error(_("could not create leading directories "
				       "of '%s'"), ಮಾರ್ಗ);
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
ಸ್ಥಿರ ಅಕ್ಷರ *get_symlink(ರಚನೆ repository *repo,
			 ರಚನೆ difftool_options *dt_options,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಅಕ್ಷರ *ದತ್ತಾಂಶ;
	ಆದರೆ (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		ರಚನೆ strbuf link = STRBUF_INIT;
		ಆದರೆ (dt_options->has_symlinks) {
			ಆದರೆ (strbuf_readlink(&link, ಮಾರ್ಗ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ)))
				die(_("could not read symlink %s"), ಮಾರ್ಗ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strbuf_read_file(&link, ಮಾರ್ಗ, 128))
			die(_("could not read symlink file %s"), ಮಾರ್ಗ);

		ದತ್ತಾಂಶ = strbuf_detach(&link, ಖಾಲಿ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಎಣಿಕೆ object_type type;
		ಗಾತ್ರ_t ಗಾತ್ರ_2;
		ದತ್ತಾಂಶ = odb_read_object(repo->objects, oid, &type, &ಗಾತ್ರ_2);
		ಆದರೆ (!ದತ್ತಾಂಶ)
			die(_("could not read object %s for symlink %s"),
				oid_ಗಮ್ಯ_hex(oid), ಮಾರ್ಗ);
	}

	ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_ಮಾರ್ಗ(ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ, ರಚನೆ object_id *oid,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ರಚನೆ checkout *state)
{
	ರಚನೆ cache_ನಮೂದು *ce;
	ಪೂರ್ಣಾಂಕ ret;

	ce = make_transient_cache_ನಮೂದು(ಬಹುಲಕ, oid, ಮಾರ್ಗ, 0, ಖಾಲಿ);
	ret = checkout_ನಮೂದು(ce, state, ಖಾಲಿ, ಖಾಲಿ);

	discard_cache_ನಮೂದು(ce);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_file_ಒಳ_directory(ರಚನೆ repository *repo,
				    ರಚನೆ strbuf *dir, ಗಾತ್ರ_t dir_len,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *content)
{
	ಕೂಡಿಸು_ಮಾರ್ಗ(dir, dir_len, ಮಾರ್ಗ);
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
ಸ್ಥಿರ ಶೂನ್ಯ write_standin_files(ರಚನೆ repository *repo,
				ರಚನೆ pair_ನಮೂದು *ನಮೂದು,
				ರಚನೆ strbuf *ldir, ಗಾತ್ರ_t ldir_len,
				ರಚನೆ strbuf *rdir, ಗಾತ್ರ_t rdir_len)
{
	ಆದರೆ (*ನಮೂದು->ಎಡ)
		write_file_ಒಳ_directory(repo, ldir, ldir_len, ನಮೂದು->ಮಾರ್ಗ, ನಮೂದು->ಎಡ);
	ಆದರೆ (*ನಮೂದು->ಬಲ)
		write_file_ಒಳ_directory(repo, rdir, rdir_len, ನಮೂದು->ಮಾರ್ಗ, ನಮೂದು->ಬಲ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_dir_diff(ರಚನೆ repository *repo,
			ರಚನೆ difftool_options *dt_options,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *extcmd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			ರಚನೆ ಮಗು_process *ಮಗು)
{
	ರಚನೆ strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	ರಚನೆ strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	ರಚನೆ strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	ರಚನೆ strbuf wtdir = STRBUF_INIT;
	ರಚನೆ strbuf tmpdir = STRBUF_INIT;
	ಅಕ್ಷರ *lbase_dir = ಖಾಲಿ, *rbase_dir = ಖಾಲಿ;
	ಗಾತ್ರ_t ldir_len, rdir_len, wtdir_len;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *workdir, *tmp;
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಗಾತ್ರ_t i;
	FILE *fp = ಖಾಲಿ;
	ರಚನೆ hashmap working_ವೃಕ್ಷ_dups = HASHMAP_INIT(working_ವೃಕ್ಷ_ನಮೂದು_cmp,
							ಖಾಲಿ);
	ರಚನೆ hashmap submodules = HASHMAP_INIT(pair_cmp, ಖಾಲಿ);
	ರಚನೆ hashmap symlinks2 = HASHMAP_INIT(pair_cmp, ಖಾಲಿ);
	ರಚನೆ hashmap_iter iter;
	ರಚನೆ pair_ನಮೂದು *ನಮೂದು;
	ರಚನೆ ಸೂಚ್ಯಂಕ_state wtindex = ಸೂಚ್ಯಂಕ_STATE_INIT(repo);
	ರಚನೆ checkout lstate, rstate;
	ಪೂರ್ಣಾಂಕ err = 0;
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;
	ರಚನೆ hashmap wt_modified = HASHMAP_INIT(ಮಾರ್ಗ_ನಮೂದು_cmp, ಖಾಲಿ);
	ರಚನೆ hashmap tmp_modified = HASHMAP_INIT(ಮಾರ್ಗ_ನಮೂದು_cmp, ಖಾಲಿ);
	ಪೂರ್ಣಾಂಕ indices_loaded = 0;

	workdir = repo_get_work_ವೃಕ್ಷ(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_ಕೂಡಿಸು_absolute_ಮಾರ್ಗ(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	ಆದರೆ (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		ಹೋಗು ಮುಕ್ತಾಯ;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	ಆದರೆ (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, ಗಾತ್ರ(lstate));
	lstate.ತಳ_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.ತಳ_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, ಗಾತ್ರ(rstate));
	rstate.ತಳ_dir = rbase_dir = xstrdup(rdir.buf);
	rstate.ತಳ_dir_len = rdir.len;
	rstate.force = 1;

	ldir_len = ldir.len;
	rdir_len = rdir.len;
	wtdir_len = wtdir.len;

	ಮಗು->no_stdin = 1;
	ಮಗು->git_cmd = 1;
	ಮಗು->use_shell = 0;
	ಮಗು->clean_on_exit = 1;
	ಮಗು->dir = ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಮಗು->ಹೊರಹೋಗು = -1;
	ಆದರೆ (ಆರಂಭ_command(ಮಗು))
		die("could not obtain raw diff");
	fp = xfdopen(ಮಗು->ಹೊರಹೋಗು, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	ಆಗ (!strbuf_getline_nul(&info, fp)) {
		ಪೂರ್ಣಾಂಕ lmode, rmode;
		ರಚನೆ object_id loid, roid;
		ಅಕ್ಷರ ಸ್ಥಿತಿ_2;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src_ಮಾರ್ಗ, *dst_ಮಾರ್ಗ;

		ಆದರೆ (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		ಆದರೆ (parse_ಸೂಚ್ಯಂಕ_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &ಸ್ಥಿತಿ_2))
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (strbuf_getline_nul(&lpath, fp))
			ನಿಲ್ಲಿಸು;
		src_ಮಾರ್ಗ = lpath.buf;

		i++;
		ಆದರೆ (ಸ್ಥಿತಿ_2 != 'C' && ಸ್ಥಿತಿ_2 != 'R') {
			dst_ಮಾರ್ಗ = src_ಮಾರ್ಗ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (strbuf_getline_nul(&rpath, fp))
				ನಿಲ್ಲಿಸು;
			dst_ಮಾರ್ಗ = rpath.buf;
		}

		ಆದರೆ (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_ಗಮ್ಯ_hex(&loid));
			ಕೂಡಿಸು_ಎಡ_or_ಬಲ(&submodules, src_ಮಾರ್ಗ, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_ಗಮ್ಯ_hex(&roid));
			ಆದರೆ (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			ಕೂಡಿಸು_ಎಡ_or_ಬಲ(&submodules, dst_ಮಾರ್ಗ, buf.buf, 1);
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (S_ISLNK(lmode)) {
			ಅಕ್ಷರ *content = get_symlink(repo, dt_options, &loid, src_ಮಾರ್ಗ);
			ಕೂಡಿಸು_ಎಡ_or_ಬಲ(&symlinks2, src_ಮಾರ್ಗ, content, 0);
			ಬಿಡುಗಡೆ(content);
		}

		ಆದರೆ (S_ISLNK(rmode)) {
			ಅಕ್ಷರ *content = get_symlink(repo, dt_options, &roid, dst_ಮಾರ್ಗ);
			ಕೂಡಿಸು_ಎಡ_or_ಬಲ(&symlinks2, dst_ಮಾರ್ಗ, content, 1);
			ಬಿಡುಗಡೆ(content);
		}

		ಆದರೆ (lmode && ಸ್ಥಿತಿ_2 != 'C') {
			ಆದರೆ (checkout_ಮಾರ್ಗ(lmode, &loid, src_ಮಾರ್ಗ, &lstate)) {
				ret = error("could not write '%s'", src_ಮಾರ್ಗ);
				ಹೋಗು ಮುಕ್ತಾಯ;
			}
		}

		ಆದರೆ (rmode && !S_ISLNK(rmode)) {
			ರಚನೆ working_ವೃಕ್ಷ_ನಮೂದು *ನಮೂದು;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(ನಮೂದು, ಮಾರ್ಗ, dst_ಮಾರ್ಗ);
			hashmap_ನಮೂದು_init(&ನಮೂದು->ನಮೂದು, strhash(dst_ಮಾರ್ಗ));
			ಆದರೆ (hashmap_get(&working_ವೃಕ್ಷ_dups, &ನಮೂದು->ನಮೂದು,
					ಖಾಲಿ)) {
				ಬಿಡುಗಡೆ(ನಮೂದು);
				ಮುಂದುವರಿ;
			}
			hashmap_ಕೂಡಿಸು(&working_ವೃಕ್ಷ_dups, &ನಮೂದು->ನಮೂದು);

			ಆದರೆ (!use_wt_file(repo, workdir, dst_ಮಾರ್ಗ, &roid)) {
				ಆದರೆ (checkout_ಮಾರ್ಗ(rmode, &roid, dst_ಮಾರ್ಗ,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_ಮಾರ್ಗ);
					ಹೋಗು ಮುಕ್ತಾಯ;
				}
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				ರಚನೆ cache_ನಮೂದು *ce2 =
					make_cache_ನಮೂದು(&wtindex, rmode, &roid,
							 dst_ಮಾರ್ಗ, 0, 0);

				ಕೂಡಿಸು_ಸೂಚ್ಯಂಕ_ನಮೂದು(&wtindex, ce2,
						ಕೂಡಿಸು_CACHE_JUST_APPEND);

				ಕೂಡಿಸು_ಮಾರ್ಗ(&rdir, rdir_len, dst_ಮಾರ್ಗ);
				ಆದರೆ (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_ಮಾರ್ಗ);
					ಹೋಗು ಮುಕ್ತಾಯ;
				}
				ಕೂಡಿಸು_ಮಾರ್ಗ(&wtdir, wtdir_len, dst_ಮಾರ್ಗ);
				ಆದರೆ (dt_options->symlinks) {
					ಆದರೆ (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						ಹೋಗು ಮುಕ್ತಾಯ;
					}
				} ಇಲ್ಲದಿದ್ದರೆ {
					ರಚನೆ stat st;
					ಆದರೆ (stat(wtdir.buf, &st))
						st.st_ಬಹುಲಕ = 0644;
					ಆದರೆ (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_ಬಹುಲಕ)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						ಹೋಗು ಮುಕ್ತಾಯ;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = ಖಾಲಿ;
	ಆದರೆ (ಮುಕ್ತಾಯ_command(ಮಗು)) {
		ret = error("error occurred running diff --raw");
		ಹೋಗು ಮುಕ್ತಾಯ;
	}

	ಆದರೆ (!i)
		ಹೋಗು ಮುಕ್ತಾಯ;

	/*
	 * Changes to submodules require special treatment.This loop writes a
	 * temporary file to both the left and right directories to show the
	 * change in the recorded SHA1 for the submodule.
	 */
	hashmap_for_each_ನಮೂದು(&submodules, &iter, ನಮೂದು,
				ನಮೂದು /* member name */) {
		write_standin_files(repo, ನಮೂದು, &ldir, ldir_len, &rdir, rdir_len);
	}

	/*
	 * Symbolic links require special treatment. The standard "git diff"
	 * shows only the link itself, not the contents of the link target.
	 * This loop replicates that behavior.
	 */
	hashmap_for_each_ನಮೂದು(&symlinks2, &iter, ನಮೂದು,
				ನಮೂದು /* member name */) {

		write_standin_files(repo, ನಮೂದು, &ldir, ldir_len, &rdir, rdir_len);
	}

	strbuf_setlen(&ldir, ldir_len);
	strbuf_setlen(&rdir, rdir_len);

	ಆದರೆ (extcmd) {
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, extcmd);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.ವಾದಗಳು, ldir.buf, rdir.buf, ಖಾಲಿ);
	ret = ಸತತ_command(&cmd);

	/* TODO: audit for interaction with sparse-index. */
	ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(&wtindex);

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
	ಗಾಗಿ (i = 0; i < wtindex.cache_nr; i++) {
		ರಚನೆ hashmap_ನಮೂದು dummy;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = wtindex.cache[i]->name;
		ರಚನೆ stat st;

		ಕೂಡಿಸು_ಮಾರ್ಗ(&rdir, rdir_len, name);
		ಆದರೆ (lstat(rdir.buf, &st))
			ಮುಂದುವರಿ;

		ಆದರೆ ((dt_options->symlinks && S_ISLNK(st.st_ಬಹುಲಕ)) || !S_ISREG(st.st_ಬಹುಲಕ))
			ಮುಂದುವರಿ;

		ಆದರೆ (!indices_loaded) {
			ರಚನೆ ಬೀಗ_file ಬೀಗ = ಬೀಗ_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			ಆದರೆ (repo_hold_ಬೀಗ_file_for_update(repo, &ಬೀಗ, buf.buf, 0) < 0 ||
			    write_locked_ಸೂಚ್ಯಂಕ(&wtindex, &ಬೀಗ, COMMIT_ಬೀಗ)) {
				ret = error("could not write %s", buf.buf);
				ಹೋಗು ಮುಕ್ತಾಯ;
			}
			ಬದಲಾಯಿತು_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			ಬದಲಾಯಿತು_files(repo, &tmp_modified, buf.buf, rdir.buf);
			ಕೂಡಿಸು_ಮಾರ್ಗ(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_ನಮೂದು_init(&dummy, strhash(name));
		ಆದರೆ (hashmap_get(&tmp_modified, &dummy, name)) {
			ಕೂಡಿಸು_ಮಾರ್ಗ(&wtdir, wtdir_len, name);
			ಆದರೆ (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_ಬಹುಲಕ))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	ಆದರೆ (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		remove_dir_recursively(&tmpdir, 0);
		ಆದರೆ (ret)
			warning(_("failed: %d"), ret);
	}

ಮುಕ್ತಾಯ:
	ಆದರೆ (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_ವೃಕ್ಷ_dups, ರಚನೆ working_ವೃಕ್ಷ_ನಮೂದು, ನಮೂದು);
	hashmap_clear_and_free(&wt_modified, ರಚನೆ ಮಾರ್ಗ_ನಮೂದು, ನಮೂದು);
	hashmap_clear_and_free(&tmp_modified, ರಚನೆ ಮಾರ್ಗ_ನಮೂದು, ನಮೂದು);
	hashmap_clear_and_free(&submodules, ರಚನೆ pair_ನಮೂದು, ನಮೂದು);
	hashmap_clear_and_free(&symlinks2, ರಚನೆ pair_ನಮೂದು, ನಮೂದು);
	release_ಸೂಚ್ಯಂಕ(&wtindex);
	ಬಿಡುಗಡೆ(lbase_dir);
	ಬಿಡುಗಡೆ(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	ಹಿಂತಿರುಗಿ (ret < 0) ? 1 : ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_file_diff(ಪೂರ್ಣಾಂಕ prompt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			 ರಚನೆ ಮಗು_process *ಮಗು)
{
	strvec_ತಳ್ಳು(&ಮಗು->env, "GIT_PAGER=");
	strvec_ತಳ್ಳು(&ಮಗು->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	ಆದರೆ (prompt > 0)
		strvec_ತಳ್ಳು(&ಮಗು->env, "GIT_DIFFTOOL_PROMPT=true");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!prompt)
		strvec_ತಳ್ಳು(&ಮಗು->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	ಮಗು->git_cmd = 1;
	ಮಗು->dir = ಪೂರ್ವಪ್ರತ್ಯಯ;

	ಹಿಂತಿರುಗಿ ಸತತ_command(ಮಗು);
}

ಪೂರ್ಣಾಂಕ cmd_difftool(ಪೂರ್ಣಾಂಕ argc,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_ಸೂಚ್ಯಂಕ = 0;
	ಸ್ಥಿರ ಅಕ್ಷರ *difftool_cmd = ಖಾಲಿ, *extcmd = ಖಾಲಿ;
	ರಚನೆ difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_ಸಂಕೇತ = 0
	};
	ರಚನೆ option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_ಗಣ_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_ಗಣ_INT_F(0, "prompt", &prompt, ಖಾಲಿ,
			1, PARSE_OPT_NONEG | PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "symlinks", &dt_options.symlinks,
			 N_("use symlinks in dir-diff mode")),
		OPT_STRING('t', "tool", &difftool_cmd, N_("tool"),
			   N_("use the specified diff tool")),
		OPT_BOOL(0, "tool-help", &tool_help,
			 N_("print a list of diff tools that may be used with "
			    "`--tool`")),
		OPT_BOOL(0, "trust-exit-code", &dt_options.trust_exit_ಸಂಕೇತ,
			 N_("make 'git-difftool' exit when an invoked diff "
			    "tool returns a non-zero exit code")),
		OPT_STRING('x', "extcmd", &extcmd, N_("command"),
			   N_("specify a custom command for viewing diffs")),
		OPT_BOOL(0, "no-index", &no_ಸೂಚ್ಯಂಕ, N_("passed to `diff`")),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ ಮಗು_process ಮಗು = ಮಗು_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	ಆದರೆ (tool_help)
		ಹಿಂತಿರುಗಿ print_tool_help();

	ಆದರೆ (!no_ಸೂಚ್ಯಂಕ && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	ಆದರೆ (!no_ಸೂಚ್ಯಂಕ){
		ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_ಮಾರ್ಗ(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT, absolute_ಮಾರ್ಗ(repo_get_work_ವೃಕ್ಷ(repo)), 1);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	ಆದರೆ (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	ಆದರೆ (difftool_cmd) {
		ಆದರೆ (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("no <tool> given for --tool=<tool>"));
	}

	ಆದರೆ (extcmd) {
		ಆದರೆ (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("no <cmd> given for --extcmd=<cmd>"));
	}

	setenv("GIT_DIFFTOOL_TRUST_EXIT_CODE",
	       dt_options.trust_exit_ಸಂಕೇತ ? "true" : "false", 1);

	/*
	 * In directory diff mode, 'git-difftool--helper' is called once
	 * to compare the a / b directories. In file diff mode, 'git diff'
	 * will invoke a separate instance of 'git-difftool--helper' for
	 * each file that changed.
	 */
	strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "diff");
	ಆದರೆ (no_ಸೂಚ್ಯಂಕ)
		strvec_ತಳ್ಳು(&ಮಗು.ವಾದಗಳು, "--no-index");
	ಆದರೆ (dir_diff)
		strvec_pushl(&ಮಗು.ವಾದಗಳು, "--raw", "--no-abbrev", "-z", ಖಾಲಿ);
	strvec_pushv(&ಮಗು.ವಾದಗಳು, argv);

	ಆದರೆ (dir_diff)
		ಹಿಂತಿರುಗಿ ಸತತ_dir_diff(repo, &dt_options, extcmd, ಪೂರ್ವಪ್ರತ್ಯಯ, &ಮಗು);
	ಹಿಂತಿರುಗಿ ಸತತ_file_diff(prompt, ಪೂರ್ವಪ್ರತ್ಯಯ, &ಮಗು);
}
