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
	صحيح trust_exit_کد;
};

ساكن صحيح difftool_config(ثابت حرف *var, ثابت حرف *مقدار,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية difftool_options *dt_options = (بنية difftool_options *)cb;
	إذا (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_کد = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, مقدار);
		أرجع 0;
	}

	أرجع git_default_config(var, مقدار, ctx, cb);
}

ساكن صحيح print_tool_help(فراغ)
{
	بنية فرزند_process cmd = فرزند_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.آرگومانها, "mergetool", "--tool-help=diff", عدم);
	أرجع پیاپی_command(&cmd);
}

ساكن صحيح parse_شاخص_info(بنية repository *repo,
			    حرف *p, صحيح *mode1, صحيح *mode2,
			    بنية object_id *oid1, بنية object_id *oid2,
			    حرف *وضعیت)
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
	*وضعیت = *++p;
	إذا (!*وضعیت)
		أرجع error("missing status");
	إذا (p[1] && !آیا_رقم(p[1]))
		أرجع error("unexpected trailer: '%s'", p + 1);
	أرجع 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
ساكن فراغ جمع_کن_مسیر(بنية strbuf *buf, اندازه_t پایه_len, ثابت حرف *مسیر)
{
	strbuf_setlen(buf, پایه_len);
	إذا (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, مسیر);
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
	جمع_کن_مسیر(&buf, buf.len, name);

	إذا (!lstat(buf.buf, &st) && !S_ISLNK(st.st_نما)) {
		بنية object_id wt_oid;
		صحيح fd = open(buf.buf, O_RDONLY);

		إذا (fd >= 0 &&
		    !شاخص_fd(repo->شاخص, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
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

بنية working_درخت_مدخل {
	بنية hashmap_مدخل مدخل;
	حرف مسیر[FLEX_ARRAY];
};

ساكن صحيح working_درخت_مدخل_cmp(ثابت فراغ *cmp_داده استفادهنشده,
				  ثابت بنية hashmap_مدخل *eptr,
				  ثابت بنية hashmap_مدخل *مدخل_or_کلید,
				  ثابت فراغ *keydata استفادهنشده)
{
	ثابت بنية working_درخت_مدخل *a, *b;

	a = container_از(eptr, ثابت بنية working_درخت_مدخل, مدخل);
	b = container_از(مدخل_or_کلید, ثابت بنية working_درخت_مدخل, مدخل);

	أرجع strcmp(a->مسیر, b->مسیر);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
بنية pair_مدخل {
	بنية hashmap_مدخل مدخل;
	حرف چپ[مسیر_MAX], راست[مسیر_MAX];
	ثابت حرف مسیر[FLEX_ARRAY];
};

ساكن صحيح pair_cmp(ثابت فراغ *cmp_داده استفادهنشده,
		    ثابت بنية hashmap_مدخل *eptr,
		    ثابت بنية hashmap_مدخل *مدخل_or_کلید,
		    ثابت فراغ *keydata استفادهنشده)
{
	ثابت بنية pair_مدخل *a, *b;

	a = container_از(eptr, ثابت بنية pair_مدخل, مدخل);
	b = container_از(مدخل_or_کلید, ثابت بنية pair_مدخل, مدخل);

	أرجع strcmp(a->مسیر, b->مسیر);
}

ساكن فراغ جمع_کن_چپ_or_راست(بنية hashmap *map, ثابت حرف *مسیر,
			      ثابت حرف *content, صحيح is_راست)
{
	بنية pair_مدخل *e, *existing;

	FLEX_ALLOC_STR(e, مسیر, مسیر);
	hashmap_مدخل_init(&e->مدخل, strhash(مسیر));
	existing = hashmap_get_مدخل(map, e, مدخل, عدم);
	إذا (existing) {
		حرر(e);
		e = existing;
	} وإلا {
		e->چپ[0] = e->راست[0] = '\0';
		hashmap_جمع_کن(map, &e->مدخل);
	}
	strlcpy(is_راست ? e->راست : e->چپ, content, مسیر_MAX);
}

بنية مسیر_مدخل {
	بنية hashmap_مدخل مدخل;
	حرف مسیر[FLEX_ARRAY];
};

ساكن صحيح مسیر_مدخل_cmp(ثابت فراغ *cmp_داده استفادهنشده,
			  ثابت بنية hashmap_مدخل *eptr,
			  ثابت بنية hashmap_مدخل *مدخل_or_کلید,
			  ثابت فراغ *کلید)
{
	ثابت بنية مسیر_مدخل *a, *b;

	a = container_از(eptr, ثابت بنية مسیر_مدخل, مدخل);
	b = container_از(مدخل_or_کلید, ثابت بنية مسیر_مدخل, مدخل);

	أرجع strcmp(a->مسیر, کلید ? کلید : b->مسیر);
}

ساكن فراغ تغییریافته_files(بنية repository *repo,
			  بنية hashmap *نتیجه, ثابت حرف *شاخص_مسیر,
			  ثابت حرف *workdir)
{
	بنية فرزند_process update_شاخص = فرزند_PROCESS_INIT;
	بنية فرزند_process diff_files = فرزند_PROCESS_INIT;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *git_dir = absolute_مسیر(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_شاخص.آرگومانها,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", عدم);
	update_شاخص.no_stdin = 1;
	update_شاخص.no_stdout = 1;
	update_شاخص.no_stderr = 1;
	update_شاخص.git_cmd = 1;
	update_شاخص.use_shell = 0;
	update_شاخص.clean_on_exit = 1;
	update_شاخص.dir = workdir;
	strvec_pushf(&update_شاخص.env, "GIT_INDEX_FILE=%s", شاخص_مسیر);
	/* Ignore any errors of update-index */
	پیاپی_command(&update_شاخص);

	strvec_pushl(&diff_files.آرگومانها,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "diff-files", "--name-only", "-z", عدم);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.بیرون = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", شاخص_مسیر);
	إذا (شروع_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.بیرون, "r");
	طالما (!strbuf_getline_nul(&buf, fp)) {
		بنية مسیر_مدخل *مدخل;
		FLEX_ALLOC_STR(مدخل, مسیر, buf.buf);
		hashmap_مدخل_init(&مدخل->مدخل, strhash(buf.buf));
		hashmap_جمع_کن(نتیجه, &مدخل->مدخل);
	}
	fclose(fp);
	إذا (پایان_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

ساكن صحيح ensure_leading_directories(بنية repository *repo, حرف *مسیر)
{
	بدل (امن_create_leading_directories(repo, مسیر)) {
		حالة SCLD_OK:
		حالة SCLD_EXISTS:
			أرجع 0;
		افتراضي:
			أرجع error(_("could not create leading directories "
				       "of '%s'"), مسیر);
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
			 ثابت بنية object_id *oid, ثابت حرف *مسیر)
{
	حرف *داده;
	إذا (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		بنية strbuf link = STRBUF_INIT;
		إذا (dt_options->has_symlinks) {
			إذا (strbuf_readlink(&link, مسیر, طول_رشته(مسیر)))
				die(_("could not read symlink %s"), مسیر);
		} وإلا إذا (strbuf_read_file(&link, مسیر, 128))
			die(_("could not read symlink file %s"), مسیر);

		داده = strbuf_detach(&link, عدم);
	} وإلا {
		تعداد object_type type;
		اندازه_t اندازه;
		داده = odb_read_object(repo->objects, oid, &type, &اندازه);
		إذا (!داده)
			die(_("could not read object %s for symlink %s"),
				oid_مقصد_hex(oid), مسیر);
	}

	أرجع داده;
}

ساكن صحيح checkout_مسیر(بیعلامت نما, بنية object_id *oid,
			 ثابت حرف *مسیر, ثابت بنية checkout *state)
{
	بنية cache_مدخل *ce;
	صحيح ret;

	ce = make_transient_cache_مدخل(نما, oid, مسیر, 0, عدم);
	ret = checkout_مدخل(ce, state, عدم, عدم);

	discard_cache_مدخل(ce);
	أرجع ret;
}

ساكن فراغ write_file_درون_directory(بنية repository *repo,
				    بنية strbuf *dir, اندازه_t dir_len,
				    ثابت حرف *مسیر, ثابت حرف *content)
{
	جمع_کن_مسیر(dir, dir_len, مسیر);
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
				بنية pair_مدخل *مدخل,
				بنية strbuf *ldir, اندازه_t ldir_len,
				بنية strbuf *rdir, اندازه_t rdir_len)
{
	إذا (*مدخل->چپ)
		write_file_درون_directory(repo, ldir, ldir_len, مدخل->مسیر, مدخل->چپ);
	إذا (*مدخل->راست)
		write_file_درون_directory(repo, rdir, rdir_len, مدخل->مسیر, مدخل->راست);
}

ساكن صحيح پیاپی_dir_diff(بنية repository *repo,
			بنية difftool_options *dt_options,
			ثابت حرف *extcmd, ثابت حرف *پیشوند,
			بنية فرزند_process *فرزند)
{
	بنية strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	بنية strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	بنية strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	بنية strbuf wtdir = STRBUF_INIT;
	بنية strbuf tmpdir = STRBUF_INIT;
	حرف *lbase_dir = عدم, *rbase_dir = عدم;
	اندازه_t ldir_len, rdir_len, wtdir_len;
	ثابت حرف *workdir, *tmp;
	صحيح ret = 0;
	اندازه_t i;
	FILE *fp = عدم;
	بنية hashmap working_درخت_dups = HASHMAP_INIT(working_درخت_مدخل_cmp,
							عدم);
	بنية hashmap submodules = HASHMAP_INIT(pair_cmp, عدم);
	بنية hashmap symlinks2 = HASHMAP_INIT(pair_cmp, عدم);
	بنية hashmap_iter iter;
	بنية pair_مدخل *مدخل;
	بنية شاخص_state wtindex = شاخص_STATE_INIT(repo);
	بنية checkout lstate, rstate;
	صحيح err = 0;
	بنية فرزند_process cmd = فرزند_PROCESS_INIT;
	بنية hashmap wt_modified = HASHMAP_INIT(مسیر_مدخل_cmp, عدم);
	بنية hashmap tmp_modified = HASHMAP_INIT(مسیر_مدخل_cmp, عدم);
	صحيح indices_loaded = 0;

	workdir = repo_get_work_درخت(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_جمع_کن_absolute_مسیر(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	إذا (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		اذهب پایان;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	إذا (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, حجم(lstate));
	lstate.پایه_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.پایه_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, حجم(rstate));
	rstate.پایه_dir = rbase_dir = xstrdup(rdir.buf);
	rstate.پایه_dir_len = rdir.len;
	rstate.force = 1;

	ldir_len = ldir.len;
	rdir_len = rdir.len;
	wtdir_len = wtdir.len;

	فرزند->no_stdin = 1;
	فرزند->git_cmd = 1;
	فرزند->use_shell = 0;
	فرزند->clean_on_exit = 1;
	فرزند->dir = پیشوند;
	فرزند->بیرون = -1;
	إذا (شروع_command(فرزند))
		die("could not obtain raw diff");
	fp = xfdopen(فرزند->بیرون, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	طالما (!strbuf_getline_nul(&info, fp)) {
		صحيح lmode, rmode;
		بنية object_id loid, roid;
		حرف وضعیت;
		ثابت حرف *src_مسیر, *dst_مسیر;

		إذا (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		إذا (parse_شاخص_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &وضعیت))
			اكسر;
		إذا (strbuf_getline_nul(&lpath, fp))
			اكسر;
		src_مسیر = lpath.buf;

		i++;
		إذا (وضعیت != 'C' && وضعیت != 'R') {
			dst_مسیر = src_مسیر;
		} وإلا {
			إذا (strbuf_getline_nul(&rpath, fp))
				اكسر;
			dst_مسیر = rpath.buf;
		}

		إذا (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_مقصد_hex(&loid));
			جمع_کن_چپ_or_راست(&submodules, src_مسیر, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_مقصد_hex(&roid));
			إذا (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			جمع_کن_چپ_or_راست(&submodules, dst_مسیر, buf.buf, 1);
			استمر;
		}

		إذا (S_ISLNK(lmode)) {
			حرف *content = get_symlink(repo, dt_options, &loid, src_مسیر);
			جمع_کن_چپ_or_راست(&symlinks2, src_مسیر, content, 0);
			حرر(content);
		}

		إذا (S_ISLNK(rmode)) {
			حرف *content = get_symlink(repo, dt_options, &roid, dst_مسیر);
			جمع_کن_چپ_or_راست(&symlinks2, dst_مسیر, content, 1);
			حرر(content);
		}

		إذا (lmode && وضعیت != 'C') {
			إذا (checkout_مسیر(lmode, &loid, src_مسیر, &lstate)) {
				ret = error("could not write '%s'", src_مسیر);
				اذهب پایان;
			}
		}

		إذا (rmode && !S_ISLNK(rmode)) {
			بنية working_درخت_مدخل *مدخل;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(مدخل, مسیر, dst_مسیر);
			hashmap_مدخل_init(&مدخل->مدخل, strhash(dst_مسیر));
			إذا (hashmap_get(&working_درخت_dups, &مدخل->مدخل,
					عدم)) {
				حرر(مدخل);
				استمر;
			}
			hashmap_جمع_کن(&working_درخت_dups, &مدخل->مدخل);

			إذا (!use_wt_file(repo, workdir, dst_مسیر, &roid)) {
				إذا (checkout_مسیر(rmode, &roid, dst_مسیر,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_مسیر);
					اذهب پایان;
				}
			} وإلا إذا (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				بنية cache_مدخل *ce2 =
					make_cache_مدخل(&wtindex, rmode, &roid,
							 dst_مسیر, 0, 0);

				جمع_کن_شاخص_مدخل(&wtindex, ce2,
						جمع_کن_CACHE_JUST_APPEND);

				جمع_کن_مسیر(&rdir, rdir_len, dst_مسیر);
				إذا (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_مسیر);
					اذهب پایان;
				}
				جمع_کن_مسیر(&wtdir, wtdir_len, dst_مسیر);
				إذا (dt_options->symlinks) {
					إذا (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						اذهب پایان;
					}
				} وإلا {
					بنية stat st;
					إذا (stat(wtdir.buf, &st))
						st.st_نما = 0644;
					إذا (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_نما)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						اذهب پایان;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = عدم;
	إذا (پایان_command(فرزند)) {
		ret = error("error occurred running diff --raw");
		اذهب پایان;
	}

	إذا (!i)
		اذهب پایان;

	/*
	 * Changes to submodules require special treatment.This loop writes a
	 * temporary file to both the left and right directories to show the
	 * change in the recorded SHA1 for the submodule.
	 */
	hashmap_for_each_مدخل(&submodules, &iter, مدخل,
				مدخل /* member name */) {
		write_standin_files(repo, مدخل, &ldir, ldir_len, &rdir, rdir_len);
	}

	/*
	 * Symbolic links require special treatment. The standard "git diff"
	 * shows only the link itself, not the contents of the link target.
	 * This loop replicates that behavior.
	 */
	hashmap_for_each_مدخل(&symlinks2, &iter, مدخل,
				مدخل /* member name */) {

		write_standin_files(repo, مدخل, &ldir, ldir_len, &rdir, rdir_len);
	}

	strbuf_setlen(&ldir, ldir_len);
	strbuf_setlen(&rdir, rdir_len);

	إذا (extcmd) {
		strvec_افزودن(&cmd.آرگومانها, extcmd);
	} وإلا {
		strvec_افزودن(&cmd.آرگومانها, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.آرگومانها, ldir.buf, rdir.buf, عدم);
	ret = پیاپی_command(&cmd);

	/* TODO: audit for interaction with sparse-index. */
	ensure_پر_شاخص(&wtindex);

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
		بنية hashmap_مدخل dummy;
		ثابت حرف *name = wtindex.cache[i]->name;
		بنية stat st;

		جمع_کن_مسیر(&rdir, rdir_len, name);
		إذا (lstat(rdir.buf, &st))
			استمر;

		إذا ((dt_options->symlinks && S_ISLNK(st.st_نما)) || !S_ISREG(st.st_نما))
			استمر;

		إذا (!indices_loaded) {
			بنية قفل_file قفل = قفل_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			إذا (repo_hold_قفل_file_for_update(repo, &قفل, buf.buf, 0) < 0 ||
			    write_locked_شاخص(&wtindex, &قفل, COMMIT_قفل)) {
				ret = error("could not write %s", buf.buf);
				اذهب پایان;
			}
			تغییریافته_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			تغییریافته_files(repo, &tmp_modified, buf.buf, rdir.buf);
			جمع_کن_مسیر(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_مدخل_init(&dummy, strhash(name));
		إذا (hashmap_get(&tmp_modified, &dummy, name)) {
			جمع_کن_مسیر(&wtdir, wtdir_len, name);
			إذا (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} وإلا إذا (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_نما))
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

پایان:
	إذا (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_درخت_dups, بنية working_درخت_مدخل, مدخل);
	hashmap_clear_and_free(&wt_modified, بنية مسیر_مدخل, مدخل);
	hashmap_clear_and_free(&tmp_modified, بنية مسیر_مدخل, مدخل);
	hashmap_clear_and_free(&submodules, بنية pair_مدخل, مدخل);
	hashmap_clear_and_free(&symlinks2, بنية pair_مدخل, مدخل);
	release_شاخص(&wtindex);
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

ساكن صحيح پیاپی_file_diff(صحيح prompt, ثابت حرف *پیشوند,
			 بنية فرزند_process *فرزند)
{
	strvec_افزودن(&فرزند->env, "GIT_PAGER=");
	strvec_افزودن(&فرزند->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	إذا (prompt > 0)
		strvec_افزودن(&فرزند->env, "GIT_DIFFTOOL_PROMPT=true");
	وإلا إذا (!prompt)
		strvec_افزودن(&فرزند->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	فرزند->git_cmd = 1;
	فرزند->dir = پیشوند;

	أرجع پیاپی_command(فرزند);
}

صحيح cmd_difftool(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *پیشوند,
		 بنية repository *repo)
{
	صحيح use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_شاخص = 0;
	ساكن حرف *difftool_cmd = عدم, *extcmd = عدم;
	بنية difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_کد = 0
	};
	بنية option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_مجموعه_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_مجموعه_INT_F(0, "prompt", &prompt, عدم,
			1, PARSE_OPT_NONEG | PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "symlinks", &dt_options.symlinks,
			 N_("use symlinks in dir-diff mode")),
		OPT_STRING('t', "tool", &difftool_cmd, N_("tool"),
			   N_("use the specified diff tool")),
		OPT_BOOL(0, "tool-help", &tool_help,
			 N_("print a list of diff tools that may be used with "
			    "`--tool`")),
		OPT_BOOL(0, "trust-exit-code", &dt_options.trust_exit_کد,
			 N_("make 'git-difftool' exit when an invoked diff "
			    "tool returns a non-zero exit code")),
		OPT_STRING('x', "extcmd", &extcmd, N_("command"),
			   N_("specify a custom command for viewing diffs")),
		OPT_BOOL(0, "no-index", &no_شاخص, N_("passed to `diff`")),
		OPT_انتها()
	};
	بنية فرزند_process فرزند = فرزند_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, پیشوند, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (tool_help)
		أرجع print_tool_help();

	إذا (!no_شاخص && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	إذا (!no_شاخص){
		راهاندازی_work_درخت(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_مسیر(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_درخت_ENVIRONMENT, absolute_مسیر(repo_get_work_درخت(repo)), 1);
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
	       dt_options.trust_exit_کد ? "true" : "false", 1);

	/*
	 * In directory diff mode, 'git-difftool--helper' is called once
	 * to compare the a / b directories. In file diff mode, 'git diff'
	 * will invoke a separate instance of 'git-difftool--helper' for
	 * each file that changed.
	 */
	strvec_افزودن(&فرزند.آرگومانها, "diff");
	إذا (no_شاخص)
		strvec_افزودن(&فرزند.آرگومانها, "--no-index");
	إذا (dir_diff)
		strvec_pushl(&فرزند.آرگومانها, "--raw", "--no-abbrev", "-z", عدم);
	strvec_pushv(&فرزند.آرگومانها, argv);

	إذا (dir_diff)
		أرجع پیاپی_dir_diff(repo, &dt_options, extcmd, پیشوند, &فرزند);
	أرجع پیاپی_file_diff(prompt, پیشوند, &فرزند);
}
