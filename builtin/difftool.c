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

#अन्तर्भाव "builtin.h"

#अन्तर्भाव "abspath.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "copy.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "repository.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "entry.h"
#अन्तर्भाव "setup.h"

स्थिर ध्रुव वर्ण *ध्रुव builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	रिक्त
};

संरचना difftool_options {
	पूर्णाङ्क has_symlinks;
	पूर्णाङ्क symlinks;
	पूर्णाङ्क trust_exit_संहिता;
};

स्थिर पूर्णाङ्क difftool_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			   ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना difftool_options *dt_options = (संरचना difftool_options *)cb;
	यदि (!वाक्यं_तुलय(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_संहिता = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

स्थिर पूर्णाङ्क print_tool_help(शून्य)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.तर्काः, "mergetool", "--tool-help=diff", रिक्त);
	प्रत्यागम सततम्_command(&cmd);
}

स्थिर पूर्णाङ्क parse_सूचकांक_info(संरचना repository *repo,
			    वर्ण *p, पूर्णाङ्क *mode1, पूर्णाङ्क *mode2,
			    संरचना object_id *oid1, संरचना object_id *oid2,
			    वर्ण *स्थितिः)
{
	यदि (*p != ':')
		प्रत्यागम error("expected ':', got '%c'", *p);
	*mode1 = (पूर्णाङ्क)strtol(p + 1, &p, 8);
	यदि (*p != ' ')
		प्रत्यागम error("expected ' ', got '%c'", *p);
	*mode2 = (पूर्णाङ्क)strtol(p + 1, &p, 8);
	यदि (*p != ' ')
		प्रत्यागम error("expected ' ', got '%c'", *p);
	यदि (parse_oid_hex_algop(++p, oid1, (ध्रुव वर्ण **)&p, repo->hash_algo))
		प्रत्यागम error("expected object ID, got '%s'", p);
	यदि (*p != ' ')
		प्रत्यागम error("expected ' ', got '%c'", *p);
	यदि (parse_oid_hex_algop(++p, oid2, (ध्रुव वर्ण **)&p, repo->hash_algo))
		प्रत्यागम error("expected object ID, got '%s'", p);
	यदि (*p != ' ')
		प्रत्यागम error("expected ' ', got '%c'", *p);
	*स्थितिः = *++p;
	यदि (!*स्थितिः)
		प्रत्यागम error("missing status");
	यदि (p[1] && !अङ्कः_वा(p[1]))
		प्रत्यागम error("unexpected trailer: '%s'", p + 1);
	प्रत्यागम 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
स्थिर शून्य सङ्कलय_मार्गः(संरचना strbuf *buf, आकार_t आधारः_len, ध्रुव वर्ण *मार्गः)
{
	strbuf_setlen(buf, आधारः_len);
	यदि (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, मार्गः);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
स्थिर पूर्णाङ्क use_wt_file(संरचना repository *repo,
		       ध्रुव वर्ण *workdir, ध्रुव वर्ण *name,
		       संरचना object_id *oid)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना stat st;
	पूर्णाङ्क use = 0;

	strbuf_addstr(&buf, workdir);
	सङ्कलय_मार्गः(&buf, buf.len, name);

	यदि (!lstat(buf.buf, &st) && !S_ISLNK(st.st_बहुलकम्)) {
		संरचना object_id wt_oid;
		पूर्णाङ्क fd = उद्घाटय(buf.buf, O_RDONLY);

		यदि (fd >= 0 &&
		    !सूचकांक_fd(repo->सूचकांक, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			यदि (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} अन्यथा यदि (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	प्रत्यागम use;
}

संरचना working_वृक्षः_प्रविष्टिः {
	संरचना hashmap_प्रविष्टिः प्रविष्टिः;
	वर्ण मार्गः[FLEX_ARRAY];
};

स्थिर पूर्णाङ्क working_वृक्षः_प्रविष्टिः_cmp(ध्रुव शून्य *cmp_दत्त अप्रयुक्तम्,
				  ध्रुव संरचना hashmap_प्रविष्टिः *eptr,
				  ध्रुव संरचना hashmap_प्रविष्टिः *प्रविष्टिः_or_कुञ्चिका,
				  ध्रुव शून्य *keydata अप्रयुक्तम्)
{
	ध्रुव संरचना working_वृक्षः_प्रविष्टिः *a, *b;

	a = container_सम्बन्धि(eptr, ध्रुव संरचना working_वृक्षः_प्रविष्टिः, प्रविष्टिः);
	b = container_सम्बन्धि(प्रविष्टिः_or_कुञ्चिका, ध्रुव संरचना working_वृक्षः_प्रविष्टिः, प्रविष्टिः);

	प्रत्यागम वाक्यं_तुलय(a->मार्गः, b->मार्गः);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
संरचना pair_प्रविष्टिः {
	संरचना hashmap_प्रविष्टिः प्रविष्टिः;
	वर्ण वामः[मार्गः_MAX], दक्षिणः[मार्गः_MAX];
	ध्रुव वर्ण मार्गः[FLEX_ARRAY];
};

स्थिर पूर्णाङ्क pair_cmp(ध्रुव शून्य *cmp_दत्त अप्रयुक्तम्,
		    ध्रुव संरचना hashmap_प्रविष्टिः *eptr,
		    ध्रुव संरचना hashmap_प्रविष्टिः *प्रविष्टिः_or_कुञ्चिका,
		    ध्रुव शून्य *keydata अप्रयुक्तम्)
{
	ध्रुव संरचना pair_प्रविष्टिः *a, *b;

	a = container_सम्बन्धि(eptr, ध्रुव संरचना pair_प्रविष्टिः, प्रविष्टिः);
	b = container_सम्बन्धि(प्रविष्टिः_or_कुञ्चिका, ध्रुव संरचना pair_प्रविष्टिः, प्रविष्टिः);

	प्रत्यागम वाक्यं_तुलय(a->मार्गः, b->मार्गः);
}

स्थिर शून्य सङ्कलय_वामः_or_दक्षिणः(संरचना hashmap *map, ध्रुव वर्ण *मार्गः,
			      ध्रुव वर्ण *content, पूर्णाङ्क is_दक्षिणः)
{
	संरचना pair_प्रविष्टिः *e, *existing;

	FLEX_ALLOC_STR(e, मार्गः, मार्गः);
	hashmap_प्रविष्टिः_init(&e->प्रविष्टिः, strhash(मार्गः));
	existing = hashmap_get_प्रविष्टिः(map, e, प्रविष्टिः, रिक्त);
	यदि (existing) {
		मुक्त(e);
		e = existing;
	} अन्यथा {
		e->वामः[0] = e->दक्षिणः[0] = '\0';
		hashmap_सङ्कलय(map, &e->प्रविष्टिः);
	}
	strlcpy(is_दक्षिणः ? e->दक्षिणः : e->वामः, content, मार्गः_MAX);
}

संरचना मार्गः_प्रविष्टिः {
	संरचना hashmap_प्रविष्टिः प्रविष्टिः;
	वर्ण मार्गः[FLEX_ARRAY];
};

स्थिर पूर्णाङ्क मार्गः_प्रविष्टिः_cmp(ध्रुव शून्य *cmp_दत्त अप्रयुक्तम्,
			  ध्रुव संरचना hashmap_प्रविष्टिः *eptr,
			  ध्रुव संरचना hashmap_प्रविष्टिः *प्रविष्टिः_or_कुञ्चिका,
			  ध्रुव शून्य *कुञ्चिका)
{
	ध्रुव संरचना मार्गः_प्रविष्टिः *a, *b;

	a = container_सम्बन्धि(eptr, ध्रुव संरचना मार्गः_प्रविष्टिः, प्रविष्टिः);
	b = container_सम्बन्धि(प्रविष्टिः_or_कुञ्चिका, ध्रुव संरचना मार्गः_प्रविष्टिः, प्रविष्टिः);

	प्रत्यागम वाक्यं_तुलय(a->मार्गः, कुञ्चिका ? कुञ्चिका : b->मार्गः);
}

स्थिर शून्य परिवर्तितम्_files(संरचना repository *repo,
			  संरचना hashmap *परिणाम, ध्रुव वर्ण *सूचकांक_मार्गः,
			  ध्रुव वर्ण *workdir)
{
	संरचना child_process update_सूचकांक = CHILD_PROCESS_INIT;
	संरचना child_process diff_files = CHILD_PROCESS_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	ध्रुव वर्ण *git_dir = absolute_मार्गः(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_सूचकांक.तर्काः,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", रिक्त);
	update_सूचकांक.no_stdin = 1;
	update_सूचकांक.no_stdout = 1;
	update_सूचकांक.no_stderr = 1;
	update_सूचकांक.git_cmd = 1;
	update_सूचकांक.use_shell = 0;
	update_सूचकांक.clean_on_exit = 1;
	update_सूचकांक.dir = workdir;
	strvec_pushf(&update_सूचकांक.env, "GIT_INDEX_FILE=%s", सूचकांक_मार्गः);
	/* Ignore any errors of update-index */
	सततम्_command(&update_सूचकांक);

	strvec_pushl(&diff_files.तर्काः,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "diff-files", "--name-only", "-z", रिक्त);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.बहिर्गमनम् = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", सूचकांक_मार्गः);
	यदि (start_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.बहिर्गमनम्, "r");
	यावत् (!strbuf_getline_nul(&buf, fp)) {
		संरचना मार्गः_प्रविष्टिः *प्रविष्टिः;
		FLEX_ALLOC_STR(प्रविष्टिः, मार्गः, buf.buf);
		hashmap_प्रविष्टिः_init(&प्रविष्टिः->प्रविष्टिः, strhash(buf.buf));
		hashmap_सङ्कलय(परिणाम, &प्रविष्टिः->प्रविष्टिः);
	}
	fclose(fp);
	यदि (finish_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

स्थिर पूर्णाङ्क ensure_leading_directories(संरचना repository *repo, वर्ण *मार्गः)
{
	विकल्प (safe_create_leading_directories(repo, मार्गः)) {
		पक्ष SCLD_OK:
		पक्ष SCLD_EXISTS:
			प्रत्यागम 0;
		स्वभाव:
			प्रत्यागम error(_("could not create leading directories "
				       "of '%s'"), मार्गः);
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
स्थिर वर्ण *get_symlink(संरचना repository *repo,
			 संरचना difftool_options *dt_options,
			 ध्रुव संरचना object_id *oid, ध्रुव वर्ण *मार्गः)
{
	वर्ण *दत्त;
	यदि (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		संरचना strbuf link = STRBUF_INIT;
		यदि (dt_options->has_symlinks) {
			यदि (strbuf_readlink(&link, मार्गः, वाक्यदैर्घ्यम्(मार्गः)))
				die(_("could not read symlink %s"), मार्गः);
		} अन्यथा यदि (strbuf_read_file(&link, मार्गः, 128))
			die(_("could not read symlink file %s"), मार्गः);

		दत्त = strbuf_detach(&link, रिक्त);
	} अन्यथा {
		गणना object_type type;
		आकार_t आकार;
		दत्त = odb_read_object(repo->objects, oid, &type, &आकार);
		यदि (!दत्त)
			die(_("could not read object %s for symlink %s"),
				oid_गन्तव्यम्_hex(oid), मार्गः);
	}

	प्रत्यागम दत्त;
}

स्थिर पूर्णाङ्क checkout_मार्गः(अचिह्नित बहुलकम्, संरचना object_id *oid,
			 ध्रुव वर्ण *मार्गः, ध्रुव संरचना checkout *state)
{
	संरचना cache_प्रविष्टिः *ce;
	पूर्णाङ्क ret;

	ce = make_transient_cache_प्रविष्टिः(बहुलकम्, oid, मार्गः, 0, रिक्त);
	ret = checkout_प्रविष्टिः(ce, state, रिक्त, रिक्त);

	discard_cache_प्रविष्टिः(ce);
	प्रत्यागम ret;
}

स्थिर शून्य write_file_अन्तर्_directory(संरचना repository *repo,
				    संरचना strbuf *dir, आकार_t dir_len,
				    ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *content)
{
	सङ्कलय_मार्गः(dir, dir_len, मार्गः);
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
स्थिर शून्य write_standin_files(संरचना repository *repo,
				संरचना pair_प्रविष्टिः *प्रविष्टिः,
				संरचना strbuf *ldir, आकार_t ldir_len,
				संरचना strbuf *rdir, आकार_t rdir_len)
{
	यदि (*प्रविष्टिः->वामः)
		write_file_अन्तर्_directory(repo, ldir, ldir_len, प्रविष्टिः->मार्गः, प्रविष्टिः->वामः);
	यदि (*प्रविष्टिः->दक्षिणः)
		write_file_अन्तर्_directory(repo, rdir, rdir_len, प्रविष्टिः->मार्गः, प्रविष्टिः->दक्षिणः);
}

स्थिर पूर्णाङ्क सततम्_dir_diff(संरचना repository *repo,
			संरचना difftool_options *dt_options,
			ध्रुव वर्ण *extcmd, ध्रुव वर्ण *उपसर्गः,
			संरचना child_process *child)
{
	संरचना strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	संरचना strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	संरचना strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	संरचना strbuf wtdir = STRBUF_INIT;
	संरचना strbuf tmpdir = STRBUF_INIT;
	वर्ण *lbase_dir = रिक्त, *rbase_dir = रिक्त;
	आकार_t ldir_len, rdir_len, wtdir_len;
	ध्रुव वर्ण *workdir, *tmp;
	पूर्णाङ्क ret = 0;
	आकार_t i;
	FILE *fp = रिक्त;
	संरचना hashmap working_वृक्षः_dups = HASHMAP_INIT(working_वृक्षः_प्रविष्टिः_cmp,
							रिक्त);
	संरचना hashmap submodules = HASHMAP_INIT(pair_cmp, रिक्त);
	संरचना hashmap symlinks2 = HASHMAP_INIT(pair_cmp, रिक्त);
	संरचना hashmap_iter iter;
	संरचना pair_प्रविष्टिः *प्रविष्टिः;
	संरचना सूचकांक_state wtindex = सूचकांक_STATE_INIT(repo);
	संरचना checkout lstate, rstate;
	पूर्णाङ्क err = 0;
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	संरचना hashmap wt_modified = HASHMAP_INIT(मार्गः_प्रविष्टिः_cmp, रिक्त);
	संरचना hashmap tmp_modified = HASHMAP_INIT(मार्गः_प्रविष्टिः_cmp, रिक्त);
	पूर्णाङ्क indices_loaded = 0;

	workdir = repo_get_work_वृक्षः(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_सङ्कलय_absolute_मार्गः(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	यदि (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		गच्छ finish;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	यदि (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	स्मृतिं_पूरय(&lstate, 0, परिमाण(lstate));
	lstate.आधारः_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.आधारः_dir_len = ldir.len;
	lstate.force = 1;
	स्मृतिं_पूरय(&rstate, 0, परिमाण(rstate));
	rstate.आधारः_dir = rbase_dir = xstrdup(rdir.buf);
	rstate.आधारः_dir_len = rdir.len;
	rstate.force = 1;

	ldir_len = ldir.len;
	rdir_len = rdir.len;
	wtdir_len = wtdir.len;

	child->no_stdin = 1;
	child->git_cmd = 1;
	child->use_shell = 0;
	child->clean_on_exit = 1;
	child->dir = उपसर्गः;
	child->बहिर्गमनम् = -1;
	यदि (start_command(child))
		die("could not obtain raw diff");
	fp = xfdopen(child->बहिर्गमनम्, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	यावत् (!strbuf_getline_nul(&info, fp)) {
		पूर्णाङ्क lmode, rmode;
		संरचना object_id loid, roid;
		वर्ण स्थितिः;
		ध्रुव वर्ण *src_मार्गः, *dst_मार्गः;

		यदि (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		यदि (parse_सूचकांक_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &स्थितिः))
			भङ्ग;
		यदि (strbuf_getline_nul(&lpath, fp))
			भङ्ग;
		src_मार्गः = lpath.buf;

		i++;
		यदि (स्थितिः != 'C' && स्थितिः != 'R') {
			dst_मार्गः = src_मार्गः;
		} अन्यथा {
			यदि (strbuf_getline_nul(&rpath, fp))
				भङ्ग;
			dst_मार्गः = rpath.buf;
		}

		यदि (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_गन्तव्यम्_hex(&loid));
			सङ्कलय_वामः_or_दक्षिणः(&submodules, src_मार्गः, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_गन्तव्यम्_hex(&roid));
			यदि (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			सङ्कलय_वामः_or_दक्षिणः(&submodules, dst_मार्गः, buf.buf, 1);
			अनुवर्त;
		}

		यदि (S_ISLNK(lmode)) {
			वर्ण *content = get_symlink(repo, dt_options, &loid, src_मार्गः);
			सङ्कलय_वामः_or_दक्षिणः(&symlinks2, src_मार्गः, content, 0);
			मुक्त(content);
		}

		यदि (S_ISLNK(rmode)) {
			वर्ण *content = get_symlink(repo, dt_options, &roid, dst_मार्गः);
			सङ्कलय_वामः_or_दक्षिणः(&symlinks2, dst_मार्गः, content, 1);
			मुक्त(content);
		}

		यदि (lmode && स्थितिः != 'C') {
			यदि (checkout_मार्गः(lmode, &loid, src_मार्गः, &lstate)) {
				ret = error("could not write '%s'", src_मार्गः);
				गच्छ finish;
			}
		}

		यदि (rmode && !S_ISLNK(rmode)) {
			संरचना working_वृक्षः_प्रविष्टिः *प्रविष्टिः;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(प्रविष्टिः, मार्गः, dst_मार्गः);
			hashmap_प्रविष्टिः_init(&प्रविष्टिः->प्रविष्टिः, strhash(dst_मार्गः));
			यदि (hashmap_get(&working_वृक्षः_dups, &प्रविष्टिः->प्रविष्टिः,
					रिक्त)) {
				मुक्त(प्रविष्टिः);
				अनुवर्त;
			}
			hashmap_सङ्कलय(&working_वृक्षः_dups, &प्रविष्टिः->प्रविष्टिः);

			यदि (!use_wt_file(repo, workdir, dst_मार्गः, &roid)) {
				यदि (checkout_मार्गः(rmode, &roid, dst_मार्गः,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_मार्गः);
					गच्छ finish;
				}
			} अन्यथा यदि (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				संरचना cache_प्रविष्टिः *ce2 =
					make_cache_प्रविष्टिः(&wtindex, rmode, &roid,
							 dst_मार्गः, 0, 0);

				सङ्कलय_सूचकांक_प्रविष्टिः(&wtindex, ce2,
						सङ्कलय_CACHE_JUST_APPEND);

				सङ्कलय_मार्गः(&rdir, rdir_len, dst_मार्गः);
				यदि (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_मार्गः);
					गच्छ finish;
				}
				सङ्कलय_मार्गः(&wtdir, wtdir_len, dst_मार्गः);
				यदि (dt_options->symlinks) {
					यदि (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						गच्छ finish;
					}
				} अन्यथा {
					संरचना stat st;
					यदि (stat(wtdir.buf, &st))
						st.st_बहुलकम् = 0644;
					यदि (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_बहुलकम्)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						गच्छ finish;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = रिक्त;
	यदि (finish_command(child)) {
		ret = error("error occurred running diff --raw");
		गच्छ finish;
	}

	यदि (!i)
		गच्छ finish;

	/*
	 * Changes to submodules require special treatment.This loop writes a
	 * temporary file to both the left and right directories to show the
	 * change in the recorded SHA1 for the submodule.
	 */
	hashmap_for_each_प्रविष्टिः(&submodules, &iter, प्रविष्टिः,
				प्रविष्टिः /* member name */) {
		write_standin_files(repo, प्रविष्टिः, &ldir, ldir_len, &rdir, rdir_len);
	}

	/*
	 * Symbolic links require special treatment. The standard "git diff"
	 * shows only the link itself, not the contents of the link target.
	 * This loop replicates that behavior.
	 */
	hashmap_for_each_प्रविष्टिः(&symlinks2, &iter, प्रविष्टिः,
				प्रविष्टिः /* member name */) {

		write_standin_files(repo, प्रविष्टिः, &ldir, ldir_len, &rdir, rdir_len);
	}

	strbuf_setlen(&ldir, ldir_len);
	strbuf_setlen(&rdir, rdir_len);

	यदि (extcmd) {
		strvec_प्रक्षिप(&cmd.तर्काः, extcmd);
	} अन्यथा {
		strvec_प्रक्षिप(&cmd.तर्काः, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.तर्काः, ldir.buf, rdir.buf, रिक्त);
	ret = सततम्_command(&cmd);

	/* TODO: audit for interaction with sparse-index. */
	ensure_पूर्णम्_सूचकांक(&wtindex);

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
	प्रति (i = 0; i < wtindex.cache_nr; i++) {
		संरचना hashmap_प्रविष्टिः dummy;
		ध्रुव वर्ण *name = wtindex.cache[i]->name;
		संरचना stat st;

		सङ्कलय_मार्गः(&rdir, rdir_len, name);
		यदि (lstat(rdir.buf, &st))
			अनुवर्त;

		यदि ((dt_options->symlinks && S_ISLNK(st.st_बहुलकम्)) || !S_ISREG(st.st_बहुलकम्))
			अनुवर्त;

		यदि (!indices_loaded) {
			संरचना तालकम्_file तालकम् = तालकम्_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			यदि (repo_hold_तालकम्_file_for_update(repo, &तालकम्, buf.buf, 0) < 0 ||
			    write_locked_सूचकांक(&wtindex, &तालकम्, COMMIT_तालकम्)) {
				ret = error("could not write %s", buf.buf);
				गच्छ finish;
			}
			परिवर्तितम्_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			परिवर्तितम्_files(repo, &tmp_modified, buf.buf, rdir.buf);
			सङ्कलय_मार्गः(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_प्रविष्टिः_init(&dummy, strhash(name));
		यदि (hashmap_get(&tmp_modified, &dummy, name)) {
			सङ्कलय_मार्गः(&wtdir, wtdir_len, name);
			यदि (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} अन्यथा यदि (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_बहुलकम्))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	यदि (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} अन्यथा {
		remove_dir_recursively(&tmpdir, 0);
		यदि (ret)
			warning(_("failed: %d"), ret);
	}

finish:
	यदि (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_वृक्षः_dups, संरचना working_वृक्षः_प्रविष्टिः, प्रविष्टिः);
	hashmap_clear_and_free(&wt_modified, संरचना मार्गः_प्रविष्टिः, प्रविष्टिः);
	hashmap_clear_and_free(&tmp_modified, संरचना मार्गः_प्रविष्टिः, प्रविष्टिः);
	hashmap_clear_and_free(&submodules, संरचना pair_प्रविष्टिः, प्रविष्टिः);
	hashmap_clear_and_free(&symlinks2, संरचना pair_प्रविष्टिः, प्रविष्टिः);
	release_सूचकांक(&wtindex);
	मुक्त(lbase_dir);
	मुक्त(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	प्रत्यागम (ret < 0) ? 1 : ret;
}

स्थिर पूर्णाङ्क सततम्_file_diff(पूर्णाङ्क prompt, ध्रुव वर्ण *उपसर्गः,
			 संरचना child_process *child)
{
	strvec_प्रक्षिप(&child->env, "GIT_PAGER=");
	strvec_प्रक्षिप(&child->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	यदि (prompt > 0)
		strvec_प्रक्षिप(&child->env, "GIT_DIFFTOOL_PROMPT=true");
	अन्यथा यदि (!prompt)
		strvec_प्रक्षिप(&child->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	child->git_cmd = 1;
	child->dir = उपसर्गः;

	प्रत्यागम सततम्_command(child);
}

पूर्णाङ्क cmd_difftool(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo)
{
	पूर्णाङ्क use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_सूचकांक = 0;
	स्थिर वर्ण *difftool_cmd = रिक्त, *extcmd = रिक्त;
	संरचना difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_संहिता = 0
	};
	संरचना option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_समुच्चयः_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_समुच्चयः_INT_F(0, "prompt", &prompt, रिक्त,
			1, PARSE_OPT_NONEG | PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "symlinks", &dt_options.symlinks,
			 N_("use symlinks in dir-diff mode")),
		OPT_STRING('t', "tool", &difftool_cmd, N_("tool"),
			   N_("use the specified diff tool")),
		OPT_BOOL(0, "tool-help", &tool_help,
			 N_("print a list of diff tools that may be used with "
			    "`--tool`")),
		OPT_BOOL(0, "trust-exit-code", &dt_options.trust_exit_संहिता,
			 N_("make 'git-difftool' exit when an invoked diff "
			    "tool returns a non-zero exit code")),
		OPT_STRING('x', "extcmd", &extcmd, N_("command"),
			   N_("specify a custom command for viewing diffs")),
		OPT_BOOL(0, "no-index", &no_सूचकांक, N_("passed to `diff`")),
		OPT_अन्तः()
	};
	संरचना child_process child = CHILD_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, उपसर्गः, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	यदि (tool_help)
		प्रत्यागम print_tool_help();

	यदि (!no_सूचकांक && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	यदि (!no_सूचकांक){
		सज्जता_work_वृक्षः(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_मार्गः(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_वृक्षः_ENVIRONMENT, absolute_मार्गः(repo_get_work_वृक्षः(repo)), 1);
	} अन्यथा यदि (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	यदि (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	अन्यथा यदि (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	यदि (difftool_cmd) {
		यदि (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		अन्यथा
			die(_("no <tool> given for --tool=<tool>"));
	}

	यदि (extcmd) {
		यदि (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		अन्यथा
			die(_("no <cmd> given for --extcmd=<cmd>"));
	}

	setenv("GIT_DIFFTOOL_TRUST_EXIT_CODE",
	       dt_options.trust_exit_संहिता ? "true" : "false", 1);

	/*
	 * In directory diff mode, 'git-difftool--helper' is called once
	 * to compare the a / b directories. In file diff mode, 'git diff'
	 * will invoke a separate instance of 'git-difftool--helper' for
	 * each file that changed.
	 */
	strvec_प्रक्षिप(&child.तर्काः, "diff");
	यदि (no_सूचकांक)
		strvec_प्रक्षिप(&child.तर्काः, "--no-index");
	यदि (dir_diff)
		strvec_pushl(&child.तर्काः, "--raw", "--no-abbrev", "-z", रिक्त);
	strvec_pushv(&child.तर्काः, argv);

	यदि (dir_diff)
		प्रत्यागम सततम्_dir_diff(repo, &dt_options, extcmd, उपसर्गः, &child);
	प्रत्यागम सततम्_file_diff(prompt, उपसर्गः, &child);
}
