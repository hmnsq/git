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

ساکن ثابت حرف *ثابت builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	تہی
};

ساخت difftool_options {
	صحیح has_symlinks;
	صحیح symlinks;
	صحیح trust_exit_رمز;
};

ساکن صحیح difftool_config(ثابت حرف *var, ثابت حرف *قدر,
			   ثابت ساخت config_context *ctx, خالی *cb)
{
	ساخت difftool_options *dt_options = (ساخت difftool_options *)cb;
	اگر (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_رمز = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, قدر);
		واپس 0;
	}

	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن صحیح print_tool_help(خالی)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.دلائل, "mergetool", "--tool-help=diff", تہی);
	واپس تسلسل_command(&cmd);
}

ساکن صحیح parse_اشاریہ_info(ساخت repository *repo,
			    حرف *p, صحیح *mode1, صحیح *mode2,
			    ساخت object_id *oid1, ساخت object_id *oid2,
			    حرف *حالت_2)
{
	اگر (*p != ':')
		واپس error("expected ':', got '%c'", *p);
	*mode1 = (صحیح)strtol(p + 1, &p, 8);
	اگر (*p != ' ')
		واپس error("expected ' ', got '%c'", *p);
	*mode2 = (صحیح)strtol(p + 1, &p, 8);
	اگر (*p != ' ')
		واپس error("expected ' ', got '%c'", *p);
	اگر (parse_oid_hex_algop(++p, oid1, (ثابت حرف **)&p, repo->hash_algo))
		واپس error("expected object ID, got '%s'", p);
	اگر (*p != ' ')
		واپس error("expected ' ', got '%c'", *p);
	اگر (parse_oid_hex_algop(++p, oid2, (ثابت حرف **)&p, repo->hash_algo))
		واپس error("expected object ID, got '%s'", p);
	اگر (*p != ' ')
		واپس error("expected ' ', got '%c'", *p);
	*حالت_2 = *++p;
	اگر (!*حالت_2)
		واپس error("missing status");
	اگر (p[1] && !ہندسہ_ہے(p[1]))
		واپس error("unexpected trailer: '%s'", p + 1);
	واپس 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
ساکن خالی جوڑو_راستہ(ساخت strbuf *buf, سائز_t بنیاد_len, ثابت حرف *راستہ)
{
	strbuf_setlen(buf, بنیاد_len);
	اگر (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, راستہ);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
ساکن صحیح use_wt_file(ساخت repository *repo,
		       ثابت حرف *workdir, ثابت حرف *name,
		       ساخت object_id *oid)
{
	ساخت strbuf buf = STRBUF_INIT;
	ساخت stat st;
	صحیح use = 0;

	strbuf_addstr(&buf, workdir);
	جوڑو_راستہ(&buf, buf.len, name);

	اگر (!lstat(buf.buf, &st) && !S_ISLNK(st.st_منوال)) {
		ساخت object_id wt_oid;
		صحیح fd = open(buf.buf, O_RDONLY);

		اگر (fd >= 0 &&
		    !اشاریہ_fd(repo->اشاریہ, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			اگر (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} ورنہ اگر (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	واپس use;
}

ساخت working_درخت_اندراج {
	ساخت hashmap_اندراج اندراج_2;
	حرف راستہ[FLEX_ARRAY];
};

ساکن صحیح working_درخت_اندراج_cmp(ثابت خالی *cmp_ڈیٹا غیر_مستعمل,
				  ثابت ساخت hashmap_اندراج *eptr,
				  ثابت ساخت hashmap_اندراج *اندراج_or_کلید,
				  ثابت خالی *keydata غیر_مستعمل)
{
	ثابت ساخت working_درخت_اندراج *a, *b;

	a = container_کا(eptr, ثابت ساخت working_درخت_اندراج, اندراج_2);
	b = container_کا(اندراج_or_کلید, ثابت ساخت working_درخت_اندراج, اندراج_2);

	واپس strcmp(a->راستہ, b->راستہ);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
ساخت pair_اندراج {
	ساخت hashmap_اندراج اندراج_2;
	حرف بایاں[راستہ_MAX], دایاں[راستہ_MAX];
	ثابت حرف راستہ[FLEX_ARRAY];
};

ساکن صحیح pair_cmp(ثابت خالی *cmp_ڈیٹا غیر_مستعمل,
		    ثابت ساخت hashmap_اندراج *eptr,
		    ثابت ساخت hashmap_اندراج *اندراج_or_کلید,
		    ثابت خالی *keydata غیر_مستعمل)
{
	ثابت ساخت pair_اندراج *a, *b;

	a = container_کا(eptr, ثابت ساخت pair_اندراج, اندراج_2);
	b = container_کا(اندراج_or_کلید, ثابت ساخت pair_اندراج, اندراج_2);

	واپس strcmp(a->راستہ, b->راستہ);
}

ساکن خالی جوڑو_بایاں_or_دایاں(ساخت hashmap *map, ثابت حرف *راستہ,
			      ثابت حرف *content, صحیح is_دایاں)
{
	ساخت pair_اندراج *e, *existing;

	FLEX_ALLOC_STR(e, راستہ, راستہ);
	hashmap_اندراج_init(&e->اندراج_2, strhash(راستہ));
	existing = hashmap_get_اندراج(map, e, اندراج_2, تہی);
	اگر (existing) {
		آزاد(e);
		e = existing;
	} ورنہ {
		e->بایاں[0] = e->دایاں[0] = '\0';
		hashmap_جوڑو(map, &e->اندراج_2);
	}
	strlcpy(is_دایاں ? e->دایاں : e->بایاں, content, راستہ_MAX);
}

ساخت راستہ_اندراج {
	ساخت hashmap_اندراج اندراج_2;
	حرف راستہ[FLEX_ARRAY];
};

ساکن صحیح راستہ_اندراج_cmp(ثابت خالی *cmp_ڈیٹا غیر_مستعمل,
			  ثابت ساخت hashmap_اندراج *eptr,
			  ثابت ساخت hashmap_اندراج *اندراج_or_کلید,
			  ثابت خالی *کلید)
{
	ثابت ساخت راستہ_اندراج *a, *b;

	a = container_کا(eptr, ثابت ساخت راستہ_اندراج, اندراج_2);
	b = container_کا(اندراج_or_کلید, ثابت ساخت راستہ_اندراج, اندراج_2);

	واپس strcmp(a->راستہ, کلید ? کلید : b->راستہ);
}

ساکن خالی بدلا_files(ساخت repository *repo,
			  ساخت hashmap *نتیجہ, ثابت حرف *اشاریہ_راستہ,
			  ثابت حرف *workdir)
{
	ساخت بچہ_process update_اشاریہ = بچہ_PROCESS_INIT;
	ساخت بچہ_process diff_files = بچہ_PROCESS_INIT;
	ساخت strbuf buf = STRBUF_INIT;
	ثابت حرف *git_dir = absolute_راستہ(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_اشاریہ.دلائل,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", تہی);
	update_اشاریہ.no_stdin = 1;
	update_اشاریہ.no_stdout = 1;
	update_اشاریہ.no_stderr = 1;
	update_اشاریہ.git_cmd = 1;
	update_اشاریہ.use_shell = 0;
	update_اشاریہ.clean_on_exit = 1;
	update_اشاریہ.dir = workdir;
	strvec_pushf(&update_اشاریہ.env, "GIT_INDEX_FILE=%s", اشاریہ_راستہ);
	/* Ignore any errors of update-index */
	تسلسل_command(&update_اشاریہ);

	strvec_pushl(&diff_files.دلائل,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "diff-files", "--name-only", "-z", تہی);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.برآمد = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", اشاریہ_راستہ);
	اگر (آغاز_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.برآمد, "r");
	جب (!strbuf_getline_nul(&buf, fp)) {
		ساخت راستہ_اندراج *اندراج_2;
		FLEX_ALLOC_STR(اندراج_2, راستہ, buf.buf);
		hashmap_اندراج_init(&اندراج_2->اندراج_2, strhash(buf.buf));
		hashmap_جوڑو(نتیجہ, &اندراج_2->اندراج_2);
	}
	fclose(fp);
	اگر (اختتام_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

ساکن صحیح ensure_leading_directories(ساخت repository *repo, حرف *راستہ)
{
	انتخاب (محفوظ_create_leading_directories(repo, راستہ)) {
		حالت SCLD_OK:
		حالت SCLD_EXISTS:
			واپس 0;
		طےشدہ:
			واپس error(_("could not create leading directories "
				       "of '%s'"), راستہ);
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
ساکن حرف *get_symlink(ساخت repository *repo,
			 ساخت difftool_options *dt_options,
			 ثابت ساخت object_id *oid, ثابت حرف *راستہ)
{
	حرف *ڈیٹا;
	اگر (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		ساخت strbuf link = STRBUF_INIT;
		اگر (dt_options->has_symlinks) {
			اگر (strbuf_readlink(&link, راستہ, لڑی_لمبائی(راستہ)))
				die(_("could not read symlink %s"), راستہ);
		} ورنہ اگر (strbuf_read_file(&link, راستہ, 128))
			die(_("could not read symlink file %s"), راستہ);

		ڈیٹا = strbuf_detach(&link, تہی);
	} ورنہ {
		شمار object_type type;
		سائز_t سائز;
		ڈیٹا = odb_read_object(repo->objects, oid, &type, &سائز);
		اگر (!ڈیٹا)
			die(_("could not read object %s for symlink %s"),
				oid_مقصد_hex(oid), راستہ);
	}

	واپس ڈیٹا;
}

ساکن صحیح checkout_راستہ(غیرعلامتی منوال, ساخت object_id *oid,
			 ثابت حرف *راستہ, ثابت ساخت checkout *state)
{
	ساخت cache_اندراج *ce;
	صحیح ret;

	ce = make_transient_cache_اندراج(منوال, oid, راستہ, 0, تہی);
	ret = checkout_اندراج(ce, state, تہی, تہی);

	discard_cache_اندراج(ce);
	واپس ret;
}

ساکن خالی write_file_اندر_directory(ساخت repository *repo,
				    ساخت strbuf *dir, سائز_t dir_len,
				    ثابت حرف *راستہ, ثابت حرف *content)
{
	جوڑو_راستہ(dir, dir_len, راستہ);
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
ساکن خالی write_standin_files(ساخت repository *repo,
				ساخت pair_اندراج *اندراج_2,
				ساخت strbuf *ldir, سائز_t ldir_len,
				ساخت strbuf *rdir, سائز_t rdir_len)
{
	اگر (*اندراج_2->بایاں)
		write_file_اندر_directory(repo, ldir, ldir_len, اندراج_2->راستہ, اندراج_2->بایاں);
	اگر (*اندراج_2->دایاں)
		write_file_اندر_directory(repo, rdir, rdir_len, اندراج_2->راستہ, اندراج_2->دایاں);
}

ساکن صحیح تسلسل_dir_diff(ساخت repository *repo,
			ساخت difftool_options *dt_options,
			ثابت حرف *extcmd, ثابت حرف *سابقہ,
			ساخت بچہ_process *بچہ)
{
	ساخت strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	ساخت strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	ساخت strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	ساخت strbuf wtdir = STRBUF_INIT;
	ساخت strbuf tmpdir = STRBUF_INIT;
	حرف *lbase_dir = تہی, *rbase_dir = تہی;
	سائز_t ldir_len, rdir_len, wtdir_len;
	ثابت حرف *workdir, *tmp;
	صحیح ret = 0;
	سائز_t i;
	FILE *fp = تہی;
	ساخت hashmap working_درخت_dups = HASHMAP_INIT(working_درخت_اندراج_cmp,
							تہی);
	ساخت hashmap submodules = HASHMAP_INIT(pair_cmp, تہی);
	ساخت hashmap symlinks2 = HASHMAP_INIT(pair_cmp, تہی);
	ساخت hashmap_iter iter;
	ساخت pair_اندراج *اندراج_2;
	ساخت اشاریہ_state wtindex = اشاریہ_STATE_INIT(repo);
	ساخت checkout lstate, rstate;
	صحیح err = 0;
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;
	ساخت hashmap wt_modified = HASHMAP_INIT(راستہ_اندراج_cmp, تہی);
	ساخت hashmap tmp_modified = HASHMAP_INIT(راستہ_اندراج_cmp, تہی);
	صحیح indices_loaded = 0;

	workdir = repo_get_work_درخت(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_جوڑو_absolute_راستہ(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	اگر (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		جاؤ اختتام;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	اگر (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, حجم(lstate));
	lstate.بنیاد_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.بنیاد_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, حجم(rstate));
	rstate.بنیاد_dir = rbase_dir = xstrdup(rdir.buf);
	rstate.بنیاد_dir_len = rdir.len;
	rstate.force = 1;

	ldir_len = ldir.len;
	rdir_len = rdir.len;
	wtdir_len = wtdir.len;

	بچہ->no_stdin = 1;
	بچہ->git_cmd = 1;
	بچہ->use_shell = 0;
	بچہ->clean_on_exit = 1;
	بچہ->dir = سابقہ;
	بچہ->برآمد = -1;
	اگر (آغاز_command(بچہ))
		die("could not obtain raw diff");
	fp = xfdopen(بچہ->برآمد, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	جب (!strbuf_getline_nul(&info, fp)) {
		صحیح lmode, rmode;
		ساخت object_id loid, roid;
		حرف حالت_2;
		ثابت حرف *src_راستہ, *dst_راستہ;

		اگر (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		اگر (parse_اشاریہ_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &حالت_2))
			توڑو;
		اگر (strbuf_getline_nul(&lpath, fp))
			توڑو;
		src_راستہ = lpath.buf;

		i++;
		اگر (حالت_2 != 'C' && حالت_2 != 'R') {
			dst_راستہ = src_راستہ;
		} ورنہ {
			اگر (strbuf_getline_nul(&rpath, fp))
				توڑو;
			dst_راستہ = rpath.buf;
		}

		اگر (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_مقصد_hex(&loid));
			جوڑو_بایاں_or_دایاں(&submodules, src_راستہ, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_مقصد_hex(&roid));
			اگر (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			جوڑو_بایاں_or_دایاں(&submodules, dst_راستہ, buf.buf, 1);
			جاری;
		}

		اگر (S_ISLNK(lmode)) {
			حرف *content = get_symlink(repo, dt_options, &loid, src_راستہ);
			جوڑو_بایاں_or_دایاں(&symlinks2, src_راستہ, content, 0);
			آزاد(content);
		}

		اگر (S_ISLNK(rmode)) {
			حرف *content = get_symlink(repo, dt_options, &roid, dst_راستہ);
			جوڑو_بایاں_or_دایاں(&symlinks2, dst_راستہ, content, 1);
			آزاد(content);
		}

		اگر (lmode && حالت_2 != 'C') {
			اگر (checkout_راستہ(lmode, &loid, src_راستہ, &lstate)) {
				ret = error("could not write '%s'", src_راستہ);
				جاؤ اختتام;
			}
		}

		اگر (rmode && !S_ISLNK(rmode)) {
			ساخت working_درخت_اندراج *اندراج_2;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(اندراج_2, راستہ, dst_راستہ);
			hashmap_اندراج_init(&اندراج_2->اندراج_2, strhash(dst_راستہ));
			اگر (hashmap_get(&working_درخت_dups, &اندراج_2->اندراج_2,
					تہی)) {
				آزاد(اندراج_2);
				جاری;
			}
			hashmap_جوڑو(&working_درخت_dups, &اندراج_2->اندراج_2);

			اگر (!use_wt_file(repo, workdir, dst_راستہ, &roid)) {
				اگر (checkout_راستہ(rmode, &roid, dst_راستہ,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_راستہ);
					جاؤ اختتام;
				}
			} ورنہ اگر (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				ساخت cache_اندراج *ce2 =
					make_cache_اندراج(&wtindex, rmode, &roid,
							 dst_راستہ, 0, 0);

				جوڑو_اشاریہ_اندراج(&wtindex, ce2,
						جوڑو_CACHE_JUST_APPEND);

				جوڑو_راستہ(&rdir, rdir_len, dst_راستہ);
				اگر (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_راستہ);
					جاؤ اختتام;
				}
				جوڑو_راستہ(&wtdir, wtdir_len, dst_راستہ);
				اگر (dt_options->symlinks) {
					اگر (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						جاؤ اختتام;
					}
				} ورنہ {
					ساخت stat st;
					اگر (stat(wtdir.buf, &st))
						st.st_منوال = 0644;
					اگر (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_منوال)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						جاؤ اختتام;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = تہی;
	اگر (اختتام_command(بچہ)) {
		ret = error("error occurred running diff --raw");
		جاؤ اختتام;
	}

	اگر (!i)
		جاؤ اختتام;

	/*
	 * Changes to submodules require special treatment.This loop writes a
	 * temporary file to both the left and right directories to show the
	 * change in the recorded SHA1 for the submodule.
	 */
	hashmap_for_each_اندراج(&submodules, &iter, اندراج_2,
				اندراج_2 /* member name */) {
		write_standin_files(repo, اندراج_2, &ldir, ldir_len, &rdir, rdir_len);
	}

	/*
	 * Symbolic links require special treatment. The standard "git diff"
	 * shows only the link itself, not the contents of the link target.
	 * This loop replicates that behavior.
	 */
	hashmap_for_each_اندراج(&symlinks2, &iter, اندراج_2,
				اندراج_2 /* member name */) {

		write_standin_files(repo, اندراج_2, &ldir, ldir_len, &rdir, rdir_len);
	}

	strbuf_setlen(&ldir, ldir_len);
	strbuf_setlen(&rdir, rdir_len);

	اگر (extcmd) {
		strvec_ڈالو(&cmd.دلائل, extcmd);
	} ورنہ {
		strvec_ڈالو(&cmd.دلائل, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.دلائل, ldir.buf, rdir.buf, تہی);
	ret = تسلسل_command(&cmd);

	/* TODO: audit for interaction with sparse-index. */
	ensure_بھرا_اشاریہ(&wtindex);

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
	برائے (i = 0; i < wtindex.cache_nr; i++) {
		ساخت hashmap_اندراج dummy;
		ثابت حرف *name = wtindex.cache[i]->name;
		ساخت stat st;

		جوڑو_راستہ(&rdir, rdir_len, name);
		اگر (lstat(rdir.buf, &st))
			جاری;

		اگر ((dt_options->symlinks && S_ISLNK(st.st_منوال)) || !S_ISREG(st.st_منوال))
			جاری;

		اگر (!indices_loaded) {
			ساخت تالا_file تالا = تالا_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			اگر (repo_hold_تالا_file_for_update(repo, &تالا, buf.buf, 0) < 0 ||
			    write_locked_اشاریہ(&wtindex, &تالا, COMMIT_تالا)) {
				ret = error("could not write %s", buf.buf);
				جاؤ اختتام;
			}
			بدلا_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			بدلا_files(repo, &tmp_modified, buf.buf, rdir.buf);
			جوڑو_راستہ(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_اندراج_init(&dummy, strhash(name));
		اگر (hashmap_get(&tmp_modified, &dummy, name)) {
			جوڑو_راستہ(&wtdir, wtdir_len, name);
			اگر (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} ورنہ اگر (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_منوال))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	اگر (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} ورنہ {
		remove_dir_recursively(&tmpdir, 0);
		اگر (ret)
			warning(_("failed: %d"), ret);
	}

اختتام:
	اگر (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_درخت_dups, ساخت working_درخت_اندراج, اندراج_2);
	hashmap_clear_and_free(&wt_modified, ساخت راستہ_اندراج, اندراج_2);
	hashmap_clear_and_free(&tmp_modified, ساخت راستہ_اندراج, اندراج_2);
	hashmap_clear_and_free(&submodules, ساخت pair_اندراج, اندراج_2);
	hashmap_clear_and_free(&symlinks2, ساخت pair_اندراج, اندراج_2);
	release_اشاریہ(&wtindex);
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

	واپس (ret < 0) ? 1 : ret;
}

ساکن صحیح تسلسل_file_diff(صحیح prompt, ثابت حرف *سابقہ,
			 ساخت بچہ_process *بچہ)
{
	strvec_ڈالو(&بچہ->env, "GIT_PAGER=");
	strvec_ڈالو(&بچہ->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	اگر (prompt > 0)
		strvec_ڈالو(&بچہ->env, "GIT_DIFFTOOL_PROMPT=true");
	ورنہ اگر (!prompt)
		strvec_ڈالو(&بچہ->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	بچہ->git_cmd = 1;
	بچہ->dir = سابقہ;

	واپس تسلسل_command(بچہ);
}

صحیح cmd_difftool(صحیح argc,
		 ثابت حرف **argv,
		 ثابت حرف *سابقہ,
		 ساخت repository *repo)
{
	صحیح use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_اشاریہ = 0;
	ساکن حرف *difftool_cmd = تہی, *extcmd = تہی;
	ساخت difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_رمز = 0
	};
	ساخت option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_سیٹ_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_سیٹ_INT_F(0, "prompt", &prompt, تہی,
			1, PARSE_OPT_NONEG | PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "symlinks", &dt_options.symlinks,
			 N_("use symlinks in dir-diff mode")),
		OPT_STRING('t', "tool", &difftool_cmd, N_("tool"),
			   N_("use the specified diff tool")),
		OPT_BOOL(0, "tool-help", &tool_help,
			 N_("print a list of diff tools that may be used with "
			    "`--tool`")),
		OPT_BOOL(0, "trust-exit-code", &dt_options.trust_exit_رمز,
			 N_("make 'git-difftool' exit when an invoked diff "
			    "tool returns a non-zero exit code")),
		OPT_STRING('x', "extcmd", &extcmd, N_("command"),
			   N_("specify a custom command for viewing diffs")),
		OPT_BOOL(0, "no-index", &no_اشاریہ, N_("passed to `diff`")),
		OPT_انتہا()
	};
	ساخت بچہ_process بچہ = بچہ_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, سابقہ, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	اگر (tool_help)
		واپس print_tool_help();

	اگر (!no_اشاریہ && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	اگر (!no_اشاریہ){
		تیاری_work_درخت(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_راستہ(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_درخت_ENVIRONMENT, absolute_راستہ(repo_get_work_درخت(repo)), 1);
	} ورنہ اگر (dir_diff)
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
	ورنہ اگر (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	اگر (difftool_cmd) {
		اگر (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		ورنہ
			die(_("no <tool> given for --tool=<tool>"));
	}

	اگر (extcmd) {
		اگر (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		ورنہ
			die(_("no <cmd> given for --extcmd=<cmd>"));
	}

	setenv("GIT_DIFFTOOL_TRUST_EXIT_CODE",
	       dt_options.trust_exit_رمز ? "true" : "false", 1);

	/*
	 * In directory diff mode, 'git-difftool--helper' is called once
	 * to compare the a / b directories. In file diff mode, 'git diff'
	 * will invoke a separate instance of 'git-difftool--helper' for
	 * each file that changed.
	 */
	strvec_ڈالو(&بچہ.دلائل, "diff");
	اگر (no_اشاریہ)
		strvec_ڈالو(&بچہ.دلائل, "--no-index");
	اگر (dir_diff)
		strvec_pushl(&بچہ.دلائل, "--raw", "--no-abbrev", "-z", تہی);
	strvec_pushv(&بچہ.دلائل, argv);

	اگر (dir_diff)
		واپس تسلسل_dir_diff(repo, &dt_options, extcmd, سابقہ, &بچہ);
	واپس تسلسل_file_diff(prompt, سابقہ, &بچہ);
}
