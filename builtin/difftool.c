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
	صحيح trust_exit_کوډ;
};

ساكن صحيح difftool_config(ثابت حرف *var, ثابت حرف *ارزښت,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية difftool_options *dt_options = (بنية difftool_options *)cb;
	إذا (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_کوډ = git_config_bool(var, ارزښت);
		أرجع 0;
	}
	إذا (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, ارزښت);
		أرجع 0;
	}

	أرجع git_default_config(var, ارزښت, ctx, cb);
}

ساكن صحيح print_tool_help(فراغ)
{
	بنية ماشوم_process cmd = ماشوم_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.دلیلونه, "mergetool", "--tool-help=diff", عدم);
	أرجع پرلپسې_command(&cmd);
}

ساكن صحيح parse_شاخص_info(بنية repository *repo,
			    حرف *p, صحيح *mode1, صحيح *mode2,
			    بنية object_id *oid1, بنية object_id *oid2,
			    حرف *حالت)
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
	*حالت = *++p;
	إذا (!*حالت)
		أرجع error("missing status");
	إذا (p[1] && !که_عدد(p[1]))
		أرجع error("unexpected trailer: '%s'", p + 1);
	أرجع 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
ساكن فراغ جمع_کړه_لار(بنية strbuf *buf, اندازه_t بنسټ_len, ثابت حرف *لار)
{
	strbuf_setlen(buf, بنسټ_len);
	إذا (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, لار);
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
	جمع_کړه_لار(&buf, buf.len, name);

	إذا (!lstat(buf.buf, &st) && !S_ISLNK(st.st_ډېرپېښ)) {
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

بنية working_ونه_ثبت {
	بنية hashmap_ثبت ثبت;
	حرف لار[FLEX_ARRAY];
};

ساكن صحيح working_ونه_ثبت_cmp(ثابت فراغ *cmp_معلومات ناکارول_شوی,
				  ثابت بنية hashmap_ثبت *eptr,
				  ثابت بنية hashmap_ثبت *ثبت_or_کلی,
				  ثابت فراغ *keydata ناکارول_شوی)
{
	ثابت بنية working_ونه_ثبت *a, *b;

	a = container_د(eptr, ثابت بنية working_ونه_ثبت, ثبت);
	b = container_د(ثبت_or_کلی, ثابت بنية working_ونه_ثبت, ثبت);

	أرجع strcmp(a->لار, b->لار);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
بنية pair_ثبت {
	بنية hashmap_ثبت ثبت;
	حرف کیڼ[لار_MAX], ښی[لار_MAX];
	ثابت حرف لار[FLEX_ARRAY];
};

ساكن صحيح pair_cmp(ثابت فراغ *cmp_معلومات ناکارول_شوی,
		    ثابت بنية hashmap_ثبت *eptr,
		    ثابت بنية hashmap_ثبت *ثبت_or_کلی,
		    ثابت فراغ *keydata ناکارول_شوی)
{
	ثابت بنية pair_ثبت *a, *b;

	a = container_د(eptr, ثابت بنية pair_ثبت, ثبت);
	b = container_د(ثبت_or_کلی, ثابت بنية pair_ثبت, ثبت);

	أرجع strcmp(a->لار, b->لار);
}

ساكن فراغ جمع_کړه_کیڼ_or_ښی(بنية hashmap *map, ثابت حرف *لار,
			      ثابت حرف *content, صحيح is_ښی)
{
	بنية pair_ثبت *e, *existing;

	FLEX_ALLOC_STR(e, لار, لار);
	hashmap_ثبت_init(&e->ثبت, strhash(لار));
	existing = hashmap_get_ثبت(map, e, ثبت, عدم);
	إذا (existing) {
		حرر(e);
		e = existing;
	} وإلا {
		e->کیڼ[0] = e->ښی[0] = '\0';
		hashmap_جمع_کړه(map, &e->ثبت);
	}
	strlcpy(is_ښی ? e->ښی : e->کیڼ, content, لار_MAX);
}

بنية لار_ثبت {
	بنية hashmap_ثبت ثبت;
	حرف لار[FLEX_ARRAY];
};

ساكن صحيح لار_ثبت_cmp(ثابت فراغ *cmp_معلومات ناکارول_شوی,
			  ثابت بنية hashmap_ثبت *eptr,
			  ثابت بنية hashmap_ثبت *ثبت_or_کلی,
			  ثابت فراغ *کلی)
{
	ثابت بنية لار_ثبت *a, *b;

	a = container_د(eptr, ثابت بنية لار_ثبت, ثبت);
	b = container_د(ثبت_or_کلی, ثابت بنية لار_ثبت, ثبت);

	أرجع strcmp(a->لار, کلی ? کلی : b->لار);
}

ساكن فراغ بدل_شوی_files(بنية repository *repo,
			  بنية hashmap *پایله, ثابت حرف *شاخص_لار,
			  ثابت حرف *workdir)
{
	بنية ماشوم_process update_شاخص = ماشوم_PROCESS_INIT;
	بنية ماشوم_process diff_files = ماشوم_PROCESS_INIT;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *git_dir = absolute_لار(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_شاخص.دلیلونه,
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
	strvec_pushf(&update_شاخص.env, "GIT_INDEX_FILE=%s", شاخص_لار);
	/* Ignore any errors of update-index */
	پرلپسې_command(&update_شاخص);

	strvec_pushl(&diff_files.دلیلونه,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "diff-files", "--name-only", "-z", عدم);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.بهر = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", شاخص_لار);
	إذا (پیل_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.بهر, "r");
	طالما (!strbuf_getline_nul(&buf, fp)) {
		بنية لار_ثبت *ثبت;
		FLEX_ALLOC_STR(ثبت, لار, buf.buf);
		hashmap_ثبت_init(&ثبت->ثبت, strhash(buf.buf));
		hashmap_جمع_کړه(پایله, &ثبت->ثبت);
	}
	fclose(fp);
	إذا (پای_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

ساكن صحيح ensure_leading_directories(بنية repository *repo, حرف *لار)
{
	بدل (خوندي_create_leading_directories(repo, لار)) {
		حالة SCLD_OK:
		حالة SCLD_EXISTS:
			أرجع 0;
		افتراضي:
			أرجع error(_("could not create leading directories "
				       "of '%s'"), لار);
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
			 ثابت بنية object_id *oid, ثابت حرف *لار)
{
	حرف *معلومات;
	إذا (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		بنية strbuf link = STRBUF_INIT;
		إذا (dt_options->has_symlinks) {
			إذا (strbuf_readlink(&link, لار, مزي_اوږدوالی(لار)))
				die(_("could not read symlink %s"), لار);
		} وإلا إذا (strbuf_read_file(&link, لار, 128))
			die(_("could not read symlink file %s"), لار);

		معلومات = strbuf_detach(&link, عدم);
	} وإلا {
		تعداد object_type type;
		اندازه_t اندازه;
		معلومات = odb_read_object(repo->objects, oid, &type, &اندازه);
		إذا (!معلومات)
			die(_("could not read object %s for symlink %s"),
				oid_منزل_hex(oid), لار);
	}

	أرجع معلومات;
}

ساكن صحيح checkout_لار(بینښه ډېرپېښ, بنية object_id *oid,
			 ثابت حرف *لار, ثابت بنية checkout *state)
{
	بنية cache_ثبت *ce;
	صحيح ret;

	ce = make_transient_cache_ثبت(ډېرپېښ, oid, لار, 0, عدم);
	ret = checkout_ثبت(ce, state, عدم, عدم);

	discard_cache_ثبت(ce);
	أرجع ret;
}

ساكن فراغ write_file_دننه_directory(بنية repository *repo,
				    بنية strbuf *dir, اندازه_t dir_len,
				    ثابت حرف *لار, ثابت حرف *content)
{
	جمع_کړه_لار(dir, dir_len, لار);
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
				بنية pair_ثبت *ثبت,
				بنية strbuf *ldir, اندازه_t ldir_len,
				بنية strbuf *rdir, اندازه_t rdir_len)
{
	إذا (*ثبت->کیڼ)
		write_file_دننه_directory(repo, ldir, ldir_len, ثبت->لار, ثبت->کیڼ);
	إذا (*ثبت->ښی)
		write_file_دننه_directory(repo, rdir, rdir_len, ثبت->لار, ثبت->ښی);
}

ساكن صحيح پرلپسې_dir_diff(بنية repository *repo,
			بنية difftool_options *dt_options,
			ثابت حرف *extcmd, ثابت حرف *مختاړی,
			بنية ماشوم_process *ماشوم)
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
	بنية hashmap working_ونه_dups = HASHMAP_INIT(working_ونه_ثبت_cmp,
							عدم);
	بنية hashmap submodules = HASHMAP_INIT(pair_cmp, عدم);
	بنية hashmap symlinks2 = HASHMAP_INIT(pair_cmp, عدم);
	بنية hashmap_iter iter;
	بنية pair_ثبت *ثبت;
	بنية شاخص_state wtindex = شاخص_STATE_INIT(repo);
	بنية checkout lstate, rstate;
	صحيح err = 0;
	بنية ماشوم_process cmd = ماشوم_PROCESS_INIT;
	بنية hashmap wt_modified = HASHMAP_INIT(لار_ثبت_cmp, عدم);
	بنية hashmap tmp_modified = HASHMAP_INIT(لار_ثبت_cmp, عدم);
	صحيح indices_loaded = 0;

	workdir = repo_get_work_ونه(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_جمع_کړه_absolute_لار(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	إذا (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		اذهب پای;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	إذا (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, حجم(lstate));
	lstate.بنسټ_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.بنسټ_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, حجم(rstate));
	rstate.بنسټ_dir = rbase_dir = xstrdup(rdir.buf);
	rstate.بنسټ_dir_len = rdir.len;
	rstate.force = 1;

	ldir_len = ldir.len;
	rdir_len = rdir.len;
	wtdir_len = wtdir.len;

	ماشوم->no_stdin = 1;
	ماشوم->git_cmd = 1;
	ماشوم->use_shell = 0;
	ماشوم->clean_on_exit = 1;
	ماشوم->dir = مختاړی;
	ماشوم->بهر = -1;
	إذا (پیل_command(ماشوم))
		die("could not obtain raw diff");
	fp = xfdopen(ماشوم->بهر, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	طالما (!strbuf_getline_nul(&info, fp)) {
		صحيح lmode, rmode;
		بنية object_id loid, roid;
		حرف حالت;
		ثابت حرف *src_لار, *dst_لار;

		إذا (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		إذا (parse_شاخص_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &حالت))
			اكسر;
		إذا (strbuf_getline_nul(&lpath, fp))
			اكسر;
		src_لار = lpath.buf;

		i++;
		إذا (حالت != 'C' && حالت != 'R') {
			dst_لار = src_لار;
		} وإلا {
			إذا (strbuf_getline_nul(&rpath, fp))
				اكسر;
			dst_لار = rpath.buf;
		}

		إذا (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_منزل_hex(&loid));
			جمع_کړه_کیڼ_or_ښی(&submodules, src_لار, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_منزل_hex(&roid));
			إذا (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			جمع_کړه_کیڼ_or_ښی(&submodules, dst_لار, buf.buf, 1);
			استمر;
		}

		إذا (S_ISLNK(lmode)) {
			حرف *content = get_symlink(repo, dt_options, &loid, src_لار);
			جمع_کړه_کیڼ_or_ښی(&symlinks2, src_لار, content, 0);
			حرر(content);
		}

		إذا (S_ISLNK(rmode)) {
			حرف *content = get_symlink(repo, dt_options, &roid, dst_لار);
			جمع_کړه_کیڼ_or_ښی(&symlinks2, dst_لار, content, 1);
			حرر(content);
		}

		إذا (lmode && حالت != 'C') {
			إذا (checkout_لار(lmode, &loid, src_لار, &lstate)) {
				ret = error("could not write '%s'", src_لار);
				اذهب پای;
			}
		}

		إذا (rmode && !S_ISLNK(rmode)) {
			بنية working_ونه_ثبت *ثبت;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(ثبت, لار, dst_لار);
			hashmap_ثبت_init(&ثبت->ثبت, strhash(dst_لار));
			إذا (hashmap_get(&working_ونه_dups, &ثبت->ثبت,
					عدم)) {
				حرر(ثبت);
				استمر;
			}
			hashmap_جمع_کړه(&working_ونه_dups, &ثبت->ثبت);

			إذا (!use_wt_file(repo, workdir, dst_لار, &roid)) {
				إذا (checkout_لار(rmode, &roid, dst_لار,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_لار);
					اذهب پای;
				}
			} وإلا إذا (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				بنية cache_ثبت *ce2 =
					make_cache_ثبت(&wtindex, rmode, &roid,
							 dst_لار, 0, 0);

				جمع_کړه_شاخص_ثبت(&wtindex, ce2,
						جمع_کړه_CACHE_JUST_APPEND);

				جمع_کړه_لار(&rdir, rdir_len, dst_لار);
				إذا (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_لار);
					اذهب پای;
				}
				جمع_کړه_لار(&wtdir, wtdir_len, dst_لار);
				إذا (dt_options->symlinks) {
					إذا (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						اذهب پای;
					}
				} وإلا {
					بنية stat st;
					إذا (stat(wtdir.buf, &st))
						st.st_ډېرپېښ = 0644;
					إذا (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_ډېرپېښ)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						اذهب پای;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = عدم;
	إذا (پای_command(ماشوم)) {
		ret = error("error occurred running diff --raw");
		اذهب پای;
	}

	إذا (!i)
		اذهب پای;

	/*
	 * Changes to submodules require special treatment.This loop writes a
	 * temporary file to both the left and right directories to show the
	 * change in the recorded SHA1 for the submodule.
	 */
	hashmap_for_each_ثبت(&submodules, &iter, ثبت,
				ثبت /* member name */) {
		write_standin_files(repo, ثبت, &ldir, ldir_len, &rdir, rdir_len);
	}

	/*
	 * Symbolic links require special treatment. The standard "git diff"
	 * shows only the link itself, not the contents of the link target.
	 * This loop replicates that behavior.
	 */
	hashmap_for_each_ثبت(&symlinks2, &iter, ثبت,
				ثبت /* member name */) {

		write_standin_files(repo, ثبت, &ldir, ldir_len, &rdir, rdir_len);
	}

	strbuf_setlen(&ldir, ldir_len);
	strbuf_setlen(&rdir, rdir_len);

	إذا (extcmd) {
		strvec_ټېل(&cmd.دلیلونه, extcmd);
	} وإلا {
		strvec_ټېل(&cmd.دلیلونه, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.دلیلونه, ldir.buf, rdir.buf, عدم);
	ret = پرلپسې_command(&cmd);

	/* TODO: audit for interaction with sparse-index. */
	ensure_ډک_شاخص(&wtindex);

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
		بنية hashmap_ثبت dummy;
		ثابت حرف *name = wtindex.cache[i]->name;
		بنية stat st;

		جمع_کړه_لار(&rdir, rdir_len, name);
		إذا (lstat(rdir.buf, &st))
			استمر;

		إذا ((dt_options->symlinks && S_ISLNK(st.st_ډېرپېښ)) || !S_ISREG(st.st_ډېرپېښ))
			استمر;

		إذا (!indices_loaded) {
			بنية قلف_file قلف = قلف_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			إذا (repo_hold_قلف_file_for_update(repo, &قلف, buf.buf, 0) < 0 ||
			    write_locked_شاخص(&wtindex, &قلف, COMMIT_قلف)) {
				ret = error("could not write %s", buf.buf);
				اذهب پای;
			}
			بدل_شوی_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			بدل_شوی_files(repo, &tmp_modified, buf.buf, rdir.buf);
			جمع_کړه_لار(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_ثبت_init(&dummy, strhash(name));
		إذا (hashmap_get(&tmp_modified, &dummy, name)) {
			جمع_کړه_لار(&wtdir, wtdir_len, name);
			إذا (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} وإلا إذا (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_ډېرپېښ))
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

پای:
	إذا (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_ونه_dups, بنية working_ونه_ثبت, ثبت);
	hashmap_clear_and_free(&wt_modified, بنية لار_ثبت, ثبت);
	hashmap_clear_and_free(&tmp_modified, بنية لار_ثبت, ثبت);
	hashmap_clear_and_free(&submodules, بنية pair_ثبت, ثبت);
	hashmap_clear_and_free(&symlinks2, بنية pair_ثبت, ثبت);
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

ساكن صحيح پرلپسې_file_diff(صحيح prompt, ثابت حرف *مختاړی,
			 بنية ماشوم_process *ماشوم)
{
	strvec_ټېل(&ماشوم->env, "GIT_PAGER=");
	strvec_ټېل(&ماشوم->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	إذا (prompt > 0)
		strvec_ټېل(&ماشوم->env, "GIT_DIFFTOOL_PROMPT=true");
	وإلا إذا (!prompt)
		strvec_ټېل(&ماشوم->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	ماشوم->git_cmd = 1;
	ماشوم->dir = مختاړی;

	أرجع پرلپسې_command(ماشوم);
}

صحيح cmd_difftool(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *مختاړی,
		 بنية repository *repo)
{
	صحيح use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_شاخص = 0;
	ساكن حرف *difftool_cmd = عدم, *extcmd = عدم;
	بنية difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_کوډ = 0
	};
	بنية option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_سېټ_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_سېټ_INT_F(0, "prompt", &prompt, عدم,
			1, PARSE_OPT_NONEG | PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "symlinks", &dt_options.symlinks,
			 N_("use symlinks in dir-diff mode")),
		OPT_STRING('t', "tool", &difftool_cmd, N_("tool"),
			   N_("use the specified diff tool")),
		OPT_BOOL(0, "tool-help", &tool_help,
			 N_("print a list of diff tools that may be used with "
			    "`--tool`")),
		OPT_BOOL(0, "trust-exit-code", &dt_options.trust_exit_کوډ,
			 N_("make 'git-difftool' exit when an invoked diff "
			    "tool returns a non-zero exit code")),
		OPT_STRING('x', "extcmd", &extcmd, N_("command"),
			   N_("specify a custom command for viewing diffs")),
		OPT_BOOL(0, "no-index", &no_شاخص, N_("passed to `diff`")),
		OPT_پای_ټکی()
	};
	بنية ماشوم_process ماشوم = ماشوم_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, مختاړی, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (tool_help)
		أرجع print_tool_help();

	إذا (!no_شاخص && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	إذا (!no_شاخص){
		تیاری_work_ونه(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_لار(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_ونه_ENVIRONMENT, absolute_لار(repo_get_work_ونه(repo)), 1);
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
	       dt_options.trust_exit_کوډ ? "true" : "false", 1);

	/*
	 * In directory diff mode, 'git-difftool--helper' is called once
	 * to compare the a / b directories. In file diff mode, 'git diff'
	 * will invoke a separate instance of 'git-difftool--helper' for
	 * each file that changed.
	 */
	strvec_ټېل(&ماشوم.دلیلونه, "diff");
	إذا (no_شاخص)
		strvec_ټېل(&ماشوم.دلیلونه, "--no-index");
	إذا (dir_diff)
		strvec_pushl(&ماشوم.دلیلونه, "--raw", "--no-abbrev", "-z", عدم);
	strvec_pushv(&ماشوم.دلیلونه, argv);

	إذا (dir_diff)
		أرجع پرلپسې_dir_diff(repo, &dt_options, extcmd, مختاړی, &ماشوم);
	أرجع پرلپسې_file_diff(prompt, مختاړی, &ماشوم);
}
