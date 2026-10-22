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

#అన్తర్భావ "builtin.h"

#అన్తర్భావ "abspath.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "copy.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "repository.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "entry.h"
#అన్తర్భావ "setup.h"

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	ఖాళీ
};

నిర్మాణం difftool_options {
	పూర్ణాంకం has_symlinks;
	పూర్ణాంకం symlinks;
	పూర్ణాంకం trust_exit_సంకేతం;
};

స్థిర పూర్ణాంకం difftool_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			   స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	నిర్మాణం difftool_options *dt_options = (నిర్మాణం difftool_options *)cb;
	ఒకవేళ (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_సంకేతం = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర పూర్ణాంకం print_tool_help(శూన్య)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.వాదనలు, "mergetool", "--tool-help=diff", ఖాళీ);
	తిరిగి నిరంతరం_command(&cmd);
}

స్థిర పూర్ణాంకం parse_సూచీ_info(నిర్మాణం repository *repo,
			    అక్షరం *p, పూర్ణాంకం *mode1, పూర్ణాంకం *mode2,
			    నిర్మాణం object_id *oid1, నిర్మాణం object_id *oid2,
			    అక్షరం *స్థితి_2)
{
	ఒకవేళ (*p != ':')
		తిరిగి error("expected ':', got '%c'", *p);
	*mode1 = (పూర్ణాంకం)strtol(p + 1, &p, 8);
	ఒకవేళ (*p != ' ')
		తిరిగి error("expected ' ', got '%c'", *p);
	*mode2 = (పూర్ణాంకం)strtol(p + 1, &p, 8);
	ఒకవేళ (*p != ' ')
		తిరిగి error("expected ' ', got '%c'", *p);
	ఒకవేళ (parse_oid_hex_algop(++p, oid1, (స్థిరరాశి అక్షరం **)&p, repo->hash_algo))
		తిరిగి error("expected object ID, got '%s'", p);
	ఒకవేళ (*p != ' ')
		తిరిగి error("expected ' ', got '%c'", *p);
	ఒకవేళ (parse_oid_hex_algop(++p, oid2, (స్థిరరాశి అక్షరం **)&p, repo->hash_algo))
		తిరిగి error("expected object ID, got '%s'", p);
	ఒకవేళ (*p != ' ')
		తిరిగి error("expected ' ', got '%c'", *p);
	*స్థితి_2 = *++p;
	ఒకవేళ (!*స్థితి_2)
		తిరిగి error("missing status");
	ఒకవేళ (p[1] && !అంకెనా(p[1]))
		తిరిగి error("unexpected trailer: '%s'", p + 1);
	తిరిగి 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
స్థిర శూన్య కూడు_మార్గం(నిర్మాణం strbuf *buf, కొలత_t పునాది_len, స్థిరరాశి అక్షరం *మార్గం)
{
	strbuf_setlen(buf, పునాది_len);
	ఒకవేళ (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, మార్గం);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
స్థిర పూర్ణాంకం use_wt_file(నిర్మాణం repository *repo,
		       స్థిరరాశి అక్షరం *workdir, స్థిరరాశి అక్షరం *name,
		       నిర్మాణం object_id *oid)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం stat st;
	పూర్ణాంకం use = 0;

	strbuf_addstr(&buf, workdir);
	కూడు_మార్గం(&buf, buf.len, name);

	ఒకవేళ (!lstat(buf.buf, &st) && !S_ISLNK(st.st_బహుళకం)) {
		నిర్మాణం object_id wt_oid;
		పూర్ణాంకం fd = open(buf.buf, O_RDONLY);

		ఒకవేళ (fd >= 0 &&
		    !సూచీ_fd(repo->సూచీ, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			ఒకవేళ (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} లేకపోతే ఒకవేళ (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	తిరిగి use;
}

నిర్మాణం working_వృక్షం_నమోదు {
	నిర్మాణం hashmap_నమోదు నమోదు_2;
	అక్షరం మార్గం[FLEX_ARRAY];
};

స్థిర పూర్ణాంకం working_వృక్షం_నమోదు_cmp(స్థిరరాశి శూన్య *cmp_దత్తాంశం వాడని,
				  స్థిరరాశి నిర్మాణం hashmap_నమోదు *eptr,
				  స్థిరరాశి నిర్మాణం hashmap_నమోదు *నమోదు_or_కీలకం,
				  స్థిరరాశి శూన్య *keydata వాడని)
{
	స్థిరరాశి నిర్మాణం working_వృక్షం_నమోదు *a, *b;

	a = container_యొక్క(eptr, స్థిరరాశి నిర్మాణం working_వృక్షం_నమోదు, నమోదు_2);
	b = container_యొక్క(నమోదు_or_కీలకం, స్థిరరాశి నిర్మాణం working_వృక్షం_నమోదు, నమోదు_2);

	తిరిగి strcmp(a->మార్గం, b->మార్గం);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
నిర్మాణం pair_నమోదు {
	నిర్మాణం hashmap_నమోదు నమోదు_2;
	అక్షరం ఎడమ[మార్గం_MAX], కుడి[మార్గం_MAX];
	స్థిరరాశి అక్షరం మార్గం[FLEX_ARRAY];
};

స్థిర పూర్ణాంకం pair_cmp(స్థిరరాశి శూన్య *cmp_దత్తాంశం వాడని,
		    స్థిరరాశి నిర్మాణం hashmap_నమోదు *eptr,
		    స్థిరరాశి నిర్మాణం hashmap_నమోదు *నమోదు_or_కీలకం,
		    స్థిరరాశి శూన్య *keydata వాడని)
{
	స్థిరరాశి నిర్మాణం pair_నమోదు *a, *b;

	a = container_యొక్క(eptr, స్థిరరాశి నిర్మాణం pair_నమోదు, నమోదు_2);
	b = container_యొక్క(నమోదు_or_కీలకం, స్థిరరాశి నిర్మాణం pair_నమోదు, నమోదు_2);

	తిరిగి strcmp(a->మార్గం, b->మార్గం);
}

స్థిర శూన్య కూడు_ఎడమ_or_కుడి(నిర్మాణం hashmap *map, స్థిరరాశి అక్షరం *మార్గం,
			      స్థిరరాశి అక్షరం *content, పూర్ణాంకం is_కుడి)
{
	నిర్మాణం pair_నమోదు *e, *existing;

	FLEX_ALLOC_STR(e, మార్గం, మార్గం);
	hashmap_నమోదు_init(&e->నమోదు_2, strhash(మార్గం));
	existing = hashmap_get_నమోదు(map, e, నమోదు_2, ఖాళీ);
	ఒకవేళ (existing) {
		విడుదల(e);
		e = existing;
	} లేకపోతే {
		e->ఎడమ[0] = e->కుడి[0] = '\0';
		hashmap_కూడు(map, &e->నమోదు_2);
	}
	strlcpy(is_కుడి ? e->కుడి : e->ఎడమ, content, మార్గం_MAX);
}

నిర్మాణం మార్గం_నమోదు {
	నిర్మాణం hashmap_నమోదు నమోదు_2;
	అక్షరం మార్గం[FLEX_ARRAY];
};

స్థిర పూర్ణాంకం మార్గం_నమోదు_cmp(స్థిరరాశి శూన్య *cmp_దత్తాంశం వాడని,
			  స్థిరరాశి నిర్మాణం hashmap_నమోదు *eptr,
			  స్థిరరాశి నిర్మాణం hashmap_నమోదు *నమోదు_or_కీలకం,
			  స్థిరరాశి శూన్య *కీలకం)
{
	స్థిరరాశి నిర్మాణం మార్గం_నమోదు *a, *b;

	a = container_యొక్క(eptr, స్థిరరాశి నిర్మాణం మార్గం_నమోదు, నమోదు_2);
	b = container_యొక్క(నమోదు_or_కీలకం, స్థిరరాశి నిర్మాణం మార్గం_నమోదు, నమోదు_2);

	తిరిగి strcmp(a->మార్గం, కీలకం ? కీలకం : b->మార్గం);
}

స్థిర శూన్య మారింది_files(నిర్మాణం repository *repo,
			  నిర్మాణం hashmap *ఫలితం, స్థిరరాశి అక్షరం *సూచీ_మార్గం,
			  స్థిరరాశి అక్షరం *workdir)
{
	నిర్మాణం బిడ్డ_process update_సూచీ = బిడ్డ_PROCESS_INIT;
	నిర్మాణం బిడ్డ_process diff_files = బిడ్డ_PROCESS_INIT;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	స్థిరరాశి అక్షరం *git_dir = absolute_మార్గం(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_సూచీ.వాదనలు,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", ఖాళీ);
	update_సూచీ.no_stdin = 1;
	update_సూచీ.no_stdout = 1;
	update_సూచీ.no_stderr = 1;
	update_సూచీ.git_cmd = 1;
	update_సూచీ.use_shell = 0;
	update_సూచీ.clean_on_exit = 1;
	update_సూచీ.dir = workdir;
	strvec_pushf(&update_సూచీ.env, "GIT_INDEX_FILE=%s", సూచీ_మార్గం);
	/* Ignore any errors of update-index */
	నిరంతరం_command(&update_సూచీ);

	strvec_pushl(&diff_files.వాదనలు,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "diff-files", "--name-only", "-z", ఖాళీ);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.బహిర్గతం = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", సూచీ_మార్గం);
	ఒకవేళ (ప్రారంభం_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.బహిర్గతం, "r");
	అయితే (!strbuf_getline_nul(&buf, fp)) {
		నిర్మాణం మార్గం_నమోదు *నమోదు_2;
		FLEX_ALLOC_STR(నమోదు_2, మార్గం, buf.buf);
		hashmap_నమోదు_init(&నమోదు_2->నమోదు_2, strhash(buf.buf));
		hashmap_కూడు(ఫలితం, &నమోదు_2->నమోదు_2);
	}
	fclose(fp);
	ఒకవేళ (ముగింపు_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

స్థిర పూర్ణాంకం ensure_leading_directories(నిర్మాణం repository *repo, అక్షరం *మార్గం)
{
	ఎంపిక (సురక్షిత_create_leading_directories(repo, మార్గం)) {
		స్థితి SCLD_OK:
		స్థితి SCLD_EXISTS:
			తిరిగి 0;
		అప్రమేయ:
			తిరిగి error(_("could not create leading directories "
				       "of '%s'"), మార్గం);
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
స్థిర అక్షరం *get_symlink(నిర్మాణం repository *repo,
			 నిర్మాణం difftool_options *dt_options,
			 స్థిరరాశి నిర్మాణం object_id *oid, స్థిరరాశి అక్షరం *మార్గం)
{
	అక్షరం *దత్తాంశం;
	ఒకవేళ (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		నిర్మాణం strbuf link = STRBUF_INIT;
		ఒకవేళ (dt_options->has_symlinks) {
			ఒకవేళ (strbuf_readlink(&link, మార్గం, అక్షరశ్రేణి_పొడవు(మార్గం)))
				die(_("could not read symlink %s"), మార్గం);
		} లేకపోతే ఒకవేళ (strbuf_read_file(&link, మార్గం, 128))
			die(_("could not read symlink file %s"), మార్గం);

		దత్తాంశం = strbuf_detach(&link, ఖాళీ);
	} లేకపోతే {
		గణన object_type type;
		కొలత_t కొలత;
		దత్తాంశం = odb_read_object(repo->objects, oid, &type, &కొలత);
		ఒకవేళ (!దత్తాంశం)
			die(_("could not read object %s for symlink %s"),
				oid_గమ్యం_hex(oid), మార్గం);
	}

	తిరిగి దత్తాంశం;
}

స్థిర పూర్ణాంకం checkout_మార్గం(గుర్తులేని బహుళకం, నిర్మాణం object_id *oid,
			 స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి నిర్మాణం checkout *state)
{
	నిర్మాణం cache_నమోదు *ce;
	పూర్ణాంకం ret;

	ce = make_transient_cache_నమోదు(బహుళకం, oid, మార్గం, 0, ఖాళీ);
	ret = checkout_నమోదు(ce, state, ఖాళీ, ఖాళీ);

	discard_cache_నమోదు(ce);
	తిరిగి ret;
}

స్థిర శూన్య write_file_లోపల_directory(నిర్మాణం repository *repo,
				    నిర్మాణం strbuf *dir, కొలత_t dir_len,
				    స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *content)
{
	కూడు_మార్గం(dir, dir_len, మార్గం);
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
స్థిర శూన్య write_standin_files(నిర్మాణం repository *repo,
				నిర్మాణం pair_నమోదు *నమోదు_2,
				నిర్మాణం strbuf *ldir, కొలత_t ldir_len,
				నిర్మాణం strbuf *rdir, కొలత_t rdir_len)
{
	ఒకవేళ (*నమోదు_2->ఎడమ)
		write_file_లోపల_directory(repo, ldir, ldir_len, నమోదు_2->మార్గం, నమోదు_2->ఎడమ);
	ఒకవేళ (*నమోదు_2->కుడి)
		write_file_లోపల_directory(repo, rdir, rdir_len, నమోదు_2->మార్గం, నమోదు_2->కుడి);
}

స్థిర పూర్ణాంకం నిరంతరం_dir_diff(నిర్మాణం repository *repo,
			నిర్మాణం difftool_options *dt_options,
			స్థిరరాశి అక్షరం *extcmd, స్థిరరాశి అక్షరం *ఉపసర్గ,
			నిర్మాణం బిడ్డ_process *బిడ్డ)
{
	నిర్మాణం strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	నిర్మాణం strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	నిర్మాణం strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	నిర్మాణం strbuf wtdir = STRBUF_INIT;
	నిర్మాణం strbuf tmpdir = STRBUF_INIT;
	అక్షరం *lbase_dir = ఖాళీ, *rbase_dir = ఖాళీ;
	కొలత_t ldir_len, rdir_len, wtdir_len;
	స్థిరరాశి అక్షరం *workdir, *tmp;
	పూర్ణాంకం ret = 0;
	కొలత_t i;
	FILE *fp = ఖాళీ;
	నిర్మాణం hashmap working_వృక్షం_dups = HASHMAP_INIT(working_వృక్షం_నమోదు_cmp,
							ఖాళీ);
	నిర్మాణం hashmap submodules = HASHMAP_INIT(pair_cmp, ఖాళీ);
	నిర్మాణం hashmap symlinks2 = HASHMAP_INIT(pair_cmp, ఖాళీ);
	నిర్మాణం hashmap_iter iter;
	నిర్మాణం pair_నమోదు *నమోదు_2;
	నిర్మాణం సూచీ_state wtindex = సూచీ_STATE_INIT(repo);
	నిర్మాణం checkout lstate, rstate;
	పూర్ణాంకం err = 0;
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;
	నిర్మాణం hashmap wt_modified = HASHMAP_INIT(మార్గం_నమోదు_cmp, ఖాళీ);
	నిర్మాణం hashmap tmp_modified = HASHMAP_INIT(మార్గం_నమోదు_cmp, ఖాళీ);
	పూర్ణాంకం indices_loaded = 0;

	workdir = repo_get_work_వృక్షం(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_కూడు_absolute_మార్గం(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	ఒకవేళ (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		వెళ్ళు ముగింపు;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	ఒకవేళ (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, పరిమాణం(lstate));
	lstate.పునాది_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.పునాది_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, పరిమాణం(rstate));
	rstate.పునాది_dir = rbase_dir = xstrdup(rdir.buf);
	rstate.పునాది_dir_len = rdir.len;
	rstate.force = 1;

	ldir_len = ldir.len;
	rdir_len = rdir.len;
	wtdir_len = wtdir.len;

	బిడ్డ->no_stdin = 1;
	బిడ్డ->git_cmd = 1;
	బిడ్డ->use_shell = 0;
	బిడ్డ->clean_on_exit = 1;
	బిడ్డ->dir = ఉపసర్గ;
	బిడ్డ->బహిర్గతం = -1;
	ఒకవేళ (ప్రారంభం_command(బిడ్డ))
		die("could not obtain raw diff");
	fp = xfdopen(బిడ్డ->బహిర్గతం, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	అయితే (!strbuf_getline_nul(&info, fp)) {
		పూర్ణాంకం lmode, rmode;
		నిర్మాణం object_id loid, roid;
		అక్షరం స్థితి_2;
		స్థిరరాశి అక్షరం *src_మార్గం, *dst_మార్గం;

		ఒకవేళ (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		ఒకవేళ (parse_సూచీ_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &స్థితి_2))
			ఆపు;
		ఒకవేళ (strbuf_getline_nul(&lpath, fp))
			ఆపు;
		src_మార్గం = lpath.buf;

		i++;
		ఒకవేళ (స్థితి_2 != 'C' && స్థితి_2 != 'R') {
			dst_మార్గం = src_మార్గం;
		} లేకపోతే {
			ఒకవేళ (strbuf_getline_nul(&rpath, fp))
				ఆపు;
			dst_మార్గం = rpath.buf;
		}

		ఒకవేళ (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_గమ్యం_hex(&loid));
			కూడు_ఎడమ_or_కుడి(&submodules, src_మార్గం, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_గమ్యం_hex(&roid));
			ఒకవేళ (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			కూడు_ఎడమ_or_కుడి(&submodules, dst_మార్గం, buf.buf, 1);
			కొనసాగు;
		}

		ఒకవేళ (S_ISLNK(lmode)) {
			అక్షరం *content = get_symlink(repo, dt_options, &loid, src_మార్గం);
			కూడు_ఎడమ_or_కుడి(&symlinks2, src_మార్గం, content, 0);
			విడుదల(content);
		}

		ఒకవేళ (S_ISLNK(rmode)) {
			అక్షరం *content = get_symlink(repo, dt_options, &roid, dst_మార్గం);
			కూడు_ఎడమ_or_కుడి(&symlinks2, dst_మార్గం, content, 1);
			విడుదల(content);
		}

		ఒకవేళ (lmode && స్థితి_2 != 'C') {
			ఒకవేళ (checkout_మార్గం(lmode, &loid, src_మార్గం, &lstate)) {
				ret = error("could not write '%s'", src_మార్గం);
				వెళ్ళు ముగింపు;
			}
		}

		ఒకవేళ (rmode && !S_ISLNK(rmode)) {
			నిర్మాణం working_వృక్షం_నమోదు *నమోదు_2;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(నమోదు_2, మార్గం, dst_మార్గం);
			hashmap_నమోదు_init(&నమోదు_2->నమోదు_2, strhash(dst_మార్గం));
			ఒకవేళ (hashmap_get(&working_వృక్షం_dups, &నమోదు_2->నమోదు_2,
					ఖాళీ)) {
				విడుదల(నమోదు_2);
				కొనసాగు;
			}
			hashmap_కూడు(&working_వృక్షం_dups, &నమోదు_2->నమోదు_2);

			ఒకవేళ (!use_wt_file(repo, workdir, dst_మార్గం, &roid)) {
				ఒకవేళ (checkout_మార్గం(rmode, &roid, dst_మార్గం,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_మార్గం);
					వెళ్ళు ముగింపు;
				}
			} లేకపోతే ఒకవేళ (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				నిర్మాణం cache_నమోదు *ce2 =
					make_cache_నమోదు(&wtindex, rmode, &roid,
							 dst_మార్గం, 0, 0);

				కూడు_సూచీ_నమోదు(&wtindex, ce2,
						కూడు_CACHE_JUST_APPEND);

				కూడు_మార్గం(&rdir, rdir_len, dst_మార్గం);
				ఒకవేళ (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_మార్గం);
					వెళ్ళు ముగింపు;
				}
				కూడు_మార్గం(&wtdir, wtdir_len, dst_మార్గం);
				ఒకవేళ (dt_options->symlinks) {
					ఒకవేళ (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						వెళ్ళు ముగింపు;
					}
				} లేకపోతే {
					నిర్మాణం stat st;
					ఒకవేళ (stat(wtdir.buf, &st))
						st.st_బహుళకం = 0644;
					ఒకవేళ (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_బహుళకం)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						వెళ్ళు ముగింపు;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = ఖాళీ;
	ఒకవేళ (ముగింపు_command(బిడ్డ)) {
		ret = error("error occurred running diff --raw");
		వెళ్ళు ముగింపు;
	}

	ఒకవేళ (!i)
		వెళ్ళు ముగింపు;

	/*
	 * Changes to submodules require special treatment.This loop writes a
	 * temporary file to both the left and right directories to show the
	 * change in the recorded SHA1 for the submodule.
	 */
	hashmap_for_each_నమోదు(&submodules, &iter, నమోదు_2,
				నమోదు_2 /* member name */) {
		write_standin_files(repo, నమోదు_2, &ldir, ldir_len, &rdir, rdir_len);
	}

	/*
	 * Symbolic links require special treatment. The standard "git diff"
	 * shows only the link itself, not the contents of the link target.
	 * This loop replicates that behavior.
	 */
	hashmap_for_each_నమోదు(&symlinks2, &iter, నమోదు_2,
				నమోదు_2 /* member name */) {

		write_standin_files(repo, నమోదు_2, &ldir, ldir_len, &rdir, rdir_len);
	}

	strbuf_setlen(&ldir, ldir_len);
	strbuf_setlen(&rdir, rdir_len);

	ఒకవేళ (extcmd) {
		strvec_నెట్టు(&cmd.వాదనలు, extcmd);
	} లేకపోతే {
		strvec_నెట్టు(&cmd.వాదనలు, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.వాదనలు, ldir.buf, rdir.buf, ఖాళీ);
	ret = నిరంతరం_command(&cmd);

	/* TODO: audit for interaction with sparse-index. */
	ensure_నిండిన_సూచీ(&wtindex);

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
	కొరకు (i = 0; i < wtindex.cache_nr; i++) {
		నిర్మాణం hashmap_నమోదు dummy;
		స్థిరరాశి అక్షరం *name = wtindex.cache[i]->name;
		నిర్మాణం stat st;

		కూడు_మార్గం(&rdir, rdir_len, name);
		ఒకవేళ (lstat(rdir.buf, &st))
			కొనసాగు;

		ఒకవేళ ((dt_options->symlinks && S_ISLNK(st.st_బహుళకం)) || !S_ISREG(st.st_బహుళకం))
			కొనసాగు;

		ఒకవేళ (!indices_loaded) {
			నిర్మాణం తాళం_file తాళం = తాళం_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			ఒకవేళ (repo_hold_తాళం_file_for_update(repo, &తాళం, buf.buf, 0) < 0 ||
			    write_locked_సూచీ(&wtindex, &తాళం, COMMIT_తాళం)) {
				ret = error("could not write %s", buf.buf);
				వెళ్ళు ముగింపు;
			}
			మారింది_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			మారింది_files(repo, &tmp_modified, buf.buf, rdir.buf);
			కూడు_మార్గం(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_నమోదు_init(&dummy, strhash(name));
		ఒకవేళ (hashmap_get(&tmp_modified, &dummy, name)) {
			కూడు_మార్గం(&wtdir, wtdir_len, name);
			ఒకవేళ (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} లేకపోతే ఒకవేళ (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_బహుళకం))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	ఒకవేళ (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} లేకపోతే {
		remove_dir_recursively(&tmpdir, 0);
		ఒకవేళ (ret)
			warning(_("failed: %d"), ret);
	}

ముగింపు:
	ఒకవేళ (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_వృక్షం_dups, నిర్మాణం working_వృక్షం_నమోదు, నమోదు_2);
	hashmap_clear_and_free(&wt_modified, నిర్మాణం మార్గం_నమోదు, నమోదు_2);
	hashmap_clear_and_free(&tmp_modified, నిర్మాణం మార్గం_నమోదు, నమోదు_2);
	hashmap_clear_and_free(&submodules, నిర్మాణం pair_నమోదు, నమోదు_2);
	hashmap_clear_and_free(&symlinks2, నిర్మాణం pair_నమోదు, నమోదు_2);
	release_సూచీ(&wtindex);
	విడుదల(lbase_dir);
	విడుదల(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	తిరిగి (ret < 0) ? 1 : ret;
}

స్థిర పూర్ణాంకం నిరంతరం_file_diff(పూర్ణాంకం prompt, స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం బిడ్డ_process *బిడ్డ)
{
	strvec_నెట్టు(&బిడ్డ->env, "GIT_PAGER=");
	strvec_నెట్టు(&బిడ్డ->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	ఒకవేళ (prompt > 0)
		strvec_నెట్టు(&బిడ్డ->env, "GIT_DIFFTOOL_PROMPT=true");
	లేకపోతే ఒకవేళ (!prompt)
		strvec_నెట్టు(&బిడ్డ->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	బిడ్డ->git_cmd = 1;
	బిడ్డ->dir = ఉపసర్గ;

	తిరిగి నిరంతరం_command(బిడ్డ);
}

పూర్ణాంకం cmd_difftool(పూర్ణాంకం argc,
		 స్థిరరాశి అక్షరం **argv,
		 స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo)
{
	పూర్ణాంకం use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_సూచీ = 0;
	స్థిర అక్షరం *difftool_cmd = ఖాళీ, *extcmd = ఖాళీ;
	నిర్మాణం difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_సంకేతం = 0
	};
	నిర్మాణం option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_సమితి_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_సమితి_INT_F(0, "prompt", &prompt, ఖాళీ,
			1, PARSE_OPT_NONEG | PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "symlinks", &dt_options.symlinks,
			 N_("use symlinks in dir-diff mode")),
		OPT_STRING('t', "tool", &difftool_cmd, N_("tool"),
			   N_("use the specified diff tool")),
		OPT_BOOL(0, "tool-help", &tool_help,
			 N_("print a list of diff tools that may be used with "
			    "`--tool`")),
		OPT_BOOL(0, "trust-exit-code", &dt_options.trust_exit_సంకేతం,
			 N_("make 'git-difftool' exit when an invoked diff "
			    "tool returns a non-zero exit code")),
		OPT_STRING('x', "extcmd", &extcmd, N_("command"),
			   N_("specify a custom command for viewing diffs")),
		OPT_BOOL(0, "no-index", &no_సూచీ, N_("passed to `diff`")),
		OPT_చివర()
	};
	నిర్మాణం బిడ్డ_process బిడ్డ = బిడ్డ_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	ఒకవేళ (tool_help)
		తిరిగి print_tool_help();

	ఒకవేళ (!no_సూచీ && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	ఒకవేళ (!no_సూచీ){
		అమరిక_work_వృక్షం(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_మార్గం(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_వృక్షం_ENVIRONMENT, absolute_మార్గం(repo_get_work_వృక్షం(repo)), 1);
	} లేకపోతే ఒకవేళ (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	ఒకవేళ (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	లేకపోతే ఒకవేళ (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	ఒకవేళ (difftool_cmd) {
		ఒకవేళ (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		లేకపోతే
			die(_("no <tool> given for --tool=<tool>"));
	}

	ఒకవేళ (extcmd) {
		ఒకవేళ (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		లేకపోతే
			die(_("no <cmd> given for --extcmd=<cmd>"));
	}

	setenv("GIT_DIFFTOOL_TRUST_EXIT_CODE",
	       dt_options.trust_exit_సంకేతం ? "true" : "false", 1);

	/*
	 * In directory diff mode, 'git-difftool--helper' is called once
	 * to compare the a / b directories. In file diff mode, 'git diff'
	 * will invoke a separate instance of 'git-difftool--helper' for
	 * each file that changed.
	 */
	strvec_నెట్టు(&బిడ్డ.వాదనలు, "diff");
	ఒకవేళ (no_సూచీ)
		strvec_నెట్టు(&బిడ్డ.వాదనలు, "--no-index");
	ఒకవేళ (dir_diff)
		strvec_pushl(&బిడ్డ.వాదనలు, "--raw", "--no-abbrev", "-z", ఖాళీ);
	strvec_pushv(&బిడ్డ.వాదనలు, argv);

	ఒకవేళ (dir_diff)
		తిరిగి నిరంతరం_dir_diff(repo, &dt_options, extcmd, ఉపసర్గ, &బిడ్డ);
	తిరిగి నిరంతరం_file_diff(prompt, ఉపసర్గ, &బిడ్డ);
}
