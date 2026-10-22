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

#समावेश "builtin.h"

#समावेश "abspath.h"
#समावेश "config.h"
#समावेश "copy.h"
#समावेश "run-command.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "read-cache-ll.h"
#समावेश "repository.h"
#समावेश "sparse-index.h"
#समावेश "strvec.h"
#समावेश "strbuf.h"
#समावेश "lockfile.h"
#समावेश "object-file.h"
#समावेश "odb.h"
#समावेश "dir.h"
#समावेश "entry.h"
#समावेश "setup.h"

अटल स्थिर अक्षर *स्थिर builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	शून्य
};

काष्ठा difftool_options {
	पूर्णांक has_symlinks;
	पूर्णांक symlinks;
	पूर्णांक trust_exit_code;
};

अटल पूर्णांक difftool_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			   स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	काष्ठा difftool_options *dt_options = (काष्ठा difftool_options *)cb;
	अगर (!म_भेद(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_code = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, मूल्य);
		वापस 0;
	}

	वापस git_default_config(var, मूल्य, ctx, cb);
}

अटल पूर्णांक print_tool_help(व्योम)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "mergetool", "--tool-help=diff", शून्य);
	वापस run_command(&cmd);
}

अटल पूर्णांक parse_सूचकांक_info(काष्ठा repository *repo,
			    अक्षर *p, पूर्णांक *mode1, पूर्णांक *mode2,
			    काष्ठा object_id *oid1, काष्ठा object_id *oid2,
			    अक्षर *status)
{
	अगर (*p != ':')
		वापस error("expected ':', got '%c'", *p);
	*mode1 = (पूर्णांक)म_से_दीर्घ(p + 1, &p, 8);
	अगर (*p != ' ')
		वापस error("expected ' ', got '%c'", *p);
	*mode2 = (पूर्णांक)म_से_दीर्घ(p + 1, &p, 8);
	अगर (*p != ' ')
		वापस error("expected ' ', got '%c'", *p);
	अगर (parse_oid_hex_algop(++p, oid1, (स्थिर अक्षर **)&p, repo->hash_algo))
		वापस error("expected object ID, got '%s'", p);
	अगर (*p != ' ')
		वापस error("expected ' ', got '%c'", *p);
	अगर (parse_oid_hex_algop(++p, oid2, (स्थिर अक्षर **)&p, repo->hash_algo))
		वापस error("expected object ID, got '%s'", p);
	अगर (*p != ' ')
		वापस error("expected ' ', got '%c'", *p);
	*status = *++p;
	अगर (!*status)
		वापस error("missing status");
	अगर (p[1] && !है_अंक(p[1]))
		वापस error("unexpected trailer: '%s'", p + 1);
	वापस 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
अटल व्योम add_path(काष्ठा strbuf *buf, माप_प्रकार base_len, स्थिर अक्षर *path)
{
	strbuf_setlen(buf, base_len);
	अगर (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, path);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
अटल पूर्णांक use_wt_file(काष्ठा repository *repo,
		       स्थिर अक्षर *workdir, स्थिर अक्षर *name,
		       काष्ठा object_id *oid)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा stat st;
	पूर्णांक use = 0;

	strbuf_addstr(&buf, workdir);
	add_path(&buf, buf.len, name);

	अगर (!lstat(buf.buf, &st) && !S_ISLNK(st.st_mode)) {
		काष्ठा object_id wt_oid;
		पूर्णांक fd = खोलो(buf.buf, O_RDONLY);

		अगर (fd >= 0 &&
		    !सूचकांक_fd(repo->सूचकांक, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			अगर (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} अन्यथा अगर (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	वापस use;
}

काष्ठा working_वृक्ष_entry {
	काष्ठा hashmap_entry entry;
	अक्षर path[FLEX_ARRAY];
};

अटल पूर्णांक working_वृक्ष_entry_cmp(स्थिर व्योम *cmp_तथ्य UNUSED,
				  स्थिर काष्ठा hashmap_entry *eptr,
				  स्थिर काष्ठा hashmap_entry *entry_or_कुंजी,
				  स्थिर व्योम *keydata UNUSED)
{
	स्थिर काष्ठा working_वृक्ष_entry *a, *b;

	a = container_of(eptr, स्थिर काष्ठा working_वृक्ष_entry, entry);
	b = container_of(entry_or_कुंजी, स्थिर काष्ठा working_वृक्ष_entry, entry);

	वापस म_भेद(a->path, b->path);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
काष्ठा pair_entry {
	काष्ठा hashmap_entry entry;
	अक्षर बायाँ[PATH_MAX], दायाँ[PATH_MAX];
	स्थिर अक्षर path[FLEX_ARRAY];
};

अटल पूर्णांक pair_cmp(स्थिर व्योम *cmp_तथ्य UNUSED,
		    स्थिर काष्ठा hashmap_entry *eptr,
		    स्थिर काष्ठा hashmap_entry *entry_or_कुंजी,
		    स्थिर व्योम *keydata UNUSED)
{
	स्थिर काष्ठा pair_entry *a, *b;

	a = container_of(eptr, स्थिर काष्ठा pair_entry, entry);
	b = container_of(entry_or_कुंजी, स्थिर काष्ठा pair_entry, entry);

	वापस म_भेद(a->path, b->path);
}

अटल व्योम add_बायाँ_or_दायाँ(काष्ठा hashmap *map, स्थिर अक्षर *path,
			      स्थिर अक्षर *content, पूर्णांक is_दायाँ)
{
	काष्ठा pair_entry *e, *existing;

	FLEX_ALLOC_STR(e, path, path);
	hashmap_entry_init(&e->entry, strhash(path));
	existing = hashmap_get_entry(map, e, entry, शून्य);
	अगर (existing) {
		मुक्त(e);
		e = existing;
	} अन्यथा {
		e->बायाँ[0] = e->दायाँ[0] = '\0';
		hashmap_add(map, &e->entry);
	}
	strlcpy(is_दायाँ ? e->दायाँ : e->बायाँ, content, PATH_MAX);
}

काष्ठा path_entry {
	काष्ठा hashmap_entry entry;
	अक्षर path[FLEX_ARRAY];
};

अटल पूर्णांक path_entry_cmp(स्थिर व्योम *cmp_तथ्य UNUSED,
			  स्थिर काष्ठा hashmap_entry *eptr,
			  स्थिर काष्ठा hashmap_entry *entry_or_कुंजी,
			  स्थिर व्योम *कुंजी)
{
	स्थिर काष्ठा path_entry *a, *b;

	a = container_of(eptr, स्थिर काष्ठा path_entry, entry);
	b = container_of(entry_or_कुंजी, स्थिर काष्ठा path_entry, entry);

	वापस म_भेद(a->path, कुंजी ? कुंजी : b->path);
}

अटल व्योम changed_files(काष्ठा repository *repo,
			  काष्ठा hashmap *परिणाम, स्थिर अक्षर *सूचकांक_path,
			  स्थिर अक्षर *workdir)
{
	काष्ठा child_process update_सूचकांक = CHILD_PROCESS_INIT;
	काष्ठा child_process diff_files = CHILD_PROCESS_INIT;
	काष्ठा strbuf buf = STRBUF_INIT;
	स्थिर अक्षर *git_dir = absolute_path(repo_get_git_dir(repo));
	खाता *fp;

	strvec_pushl(&update_सूचकांक.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", शून्य);
	update_सूचकांक.no_stdin = 1;
	update_सूचकांक.no_stdout = 1;
	update_सूचकांक.no_stderr = 1;
	update_सूचकांक.git_cmd = 1;
	update_सूचकांक.use_shell = 0;
	update_सूचकांक.clean_on_exit = 1;
	update_सूचकांक.dir = workdir;
	strvec_pushf(&update_सूचकांक.env, "GIT_INDEX_FILE=%s", सूचकांक_path);
	/* Ignore any errors of update-index */
	run_command(&update_सूचकांक);

	strvec_pushl(&diff_files.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "diff-files", "--name-only", "-z", शून्य);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.out = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", सूचकांक_path);
	अगर (start_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.out, "r");
	जबतक (!strbuf_getline_nul(&buf, fp)) {
		काष्ठा path_entry *entry;
		FLEX_ALLOC_STR(entry, path, buf.buf);
		hashmap_entry_init(&entry->entry, strhash(buf.buf));
		hashmap_add(परिणाम, &entry->entry);
	}
	ख_बंद(fp);
	अगर (finish_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

अटल पूर्णांक ensure_leading_directories(काष्ठा repository *repo, अक्षर *path)
{
	चयन (safe_create_leading_directories(repo, path)) {
		हाल SCLD_OK:
		हाल SCLD_EXISTS:
			वापस 0;
		शेष:
			वापस error(_("could not create leading directories "
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
अटल अक्षर *get_symlink(काष्ठा repository *repo,
			 काष्ठा difftool_options *dt_options,
			 स्थिर काष्ठा object_id *oid, स्थिर अक्षर *path)
{
	अक्षर *तथ्य;
	अगर (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		काष्ठा strbuf link = STRBUF_INIT;
		अगर (dt_options->has_symlinks) {
			अगर (strbuf_readlink(&link, path, म_माप(path)))
				die(_("could not read symlink %s"), path);
		} अन्यथा अगर (strbuf_read_file(&link, path, 128))
			die(_("could not read symlink file %s"), path);

		तथ्य = strbuf_detach(&link, शून्य);
	} अन्यथा {
		क्रमागत object_type type;
		माप_प्रकार आकार;
		तथ्य = odb_read_object(repo->objects, oid, &type, &आकार);
		अगर (!तथ्य)
			die(_("could not read object %s for symlink %s"),
				oid_to_hex(oid), path);
	}

	वापस तथ्य;
}

अटल पूर्णांक checkout_path(अचिन्हित mode, काष्ठा object_id *oid,
			 स्थिर अक्षर *path, स्थिर काष्ठा checkout *state)
{
	काष्ठा cache_entry *ce;
	पूर्णांक ret;

	ce = make_transient_cache_entry(mode, oid, path, 0, शून्य);
	ret = checkout_entry(ce, state, शून्य, शून्य);

	discard_cache_entry(ce);
	वापस ret;
}

अटल व्योम write_file_in_directory(काष्ठा repository *repo,
				    काष्ठा strbuf *dir, माप_प्रकार dir_len,
				    स्थिर अक्षर *path, स्थिर अक्षर *content)
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
अटल व्योम write_standin_files(काष्ठा repository *repo,
				काष्ठा pair_entry *entry,
				काष्ठा strbuf *ldir, माप_प्रकार ldir_len,
				काष्ठा strbuf *rdir, माप_प्रकार rdir_len)
{
	अगर (*entry->बायाँ)
		write_file_in_directory(repo, ldir, ldir_len, entry->path, entry->बायाँ);
	अगर (*entry->दायाँ)
		write_file_in_directory(repo, rdir, rdir_len, entry->path, entry->दायाँ);
}

अटल पूर्णांक run_dir_diff(काष्ठा repository *repo,
			काष्ठा difftool_options *dt_options,
			स्थिर अक्षर *extcmd, स्थिर अक्षर *prefix,
			काष्ठा child_process *child)
{
	काष्ठा strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	काष्ठा strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	काष्ठा strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	काष्ठा strbuf wtdir = STRBUF_INIT;
	काष्ठा strbuf tmpdir = STRBUF_INIT;
	अक्षर *lbase_dir = शून्य, *rbase_dir = शून्य;
	माप_प्रकार ldir_len, rdir_len, wtdir_len;
	स्थिर अक्षर *workdir, *tmp;
	पूर्णांक ret = 0;
	माप_प्रकार i;
	खाता *fp = शून्य;
	काष्ठा hashmap working_वृक्ष_dups = HASHMAP_INIT(working_वृक्ष_entry_cmp,
							शून्य);
	काष्ठा hashmap submodules = HASHMAP_INIT(pair_cmp, शून्य);
	काष्ठा hashmap symlinks2 = HASHMAP_INIT(pair_cmp, शून्य);
	काष्ठा hashmap_iter iter;
	काष्ठा pair_entry *entry;
	काष्ठा सूचकांक_state wtindex = सूचकांक_STATE_INIT(repo);
	काष्ठा checkout lstate, rstate;
	पूर्णांक err = 0;
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;
	काष्ठा hashmap wt_modified = HASHMAP_INIT(path_entry_cmp, शून्य);
	काष्ठा hashmap tmp_modified = HASHMAP_INIT(path_entry_cmp, शून्य);
	पूर्णांक indices_loaded = 0;

	workdir = repo_get_work_वृक्ष(repo);

	/* Setup temp directories */
	tmp = दो_पर्या("TMPDIR");
	strbuf_add_absolute_path(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	अगर (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		जाओ finish;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	अगर (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	सूची_गढ़ो(ldir.buf, 0700);
	सूची_गढ़ो(rdir.buf, 0700);

	स_रखो(&lstate, 0, माप(lstate));
	lstate.base_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.base_dir_len = ldir.len;
	lstate.force = 1;
	स_रखो(&rstate, 0, माप(rstate));
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
	अगर (start_command(child))
		die("could not obtain raw diff");
	fp = xfdopen(child->out, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	जबतक (!strbuf_getline_nul(&info, fp)) {
		पूर्णांक lmode, rmode;
		काष्ठा object_id loid, roid;
		अक्षर status;
		स्थिर अक्षर *src_path, *dst_path;

		अगर (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		अगर (parse_सूचकांक_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &status))
			अवरोध;
		अगर (strbuf_getline_nul(&lpath, fp))
			अवरोध;
		src_path = lpath.buf;

		i++;
		अगर (status != 'C' && status != 'R') {
			dst_path = src_path;
		} अन्यथा {
			अगर (strbuf_getline_nul(&rpath, fp))
				अवरोध;
			dst_path = rpath.buf;
		}

		अगर (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&loid));
			add_बायाँ_or_दायाँ(&submodules, src_path, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&roid));
			अगर (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			add_बायाँ_or_दायाँ(&submodules, dst_path, buf.buf, 1);
			जारी;
		}

		अगर (S_ISLNK(lmode)) {
			अक्षर *content = get_symlink(repo, dt_options, &loid, src_path);
			add_बायाँ_or_दायाँ(&symlinks2, src_path, content, 0);
			मुक्त(content);
		}

		अगर (S_ISLNK(rmode)) {
			अक्षर *content = get_symlink(repo, dt_options, &roid, dst_path);
			add_बायाँ_or_दायाँ(&symlinks2, dst_path, content, 1);
			मुक्त(content);
		}

		अगर (lmode && status != 'C') {
			अगर (checkout_path(lmode, &loid, src_path, &lstate)) {
				ret = error("could not write '%s'", src_path);
				जाओ finish;
			}
		}

		अगर (rmode && !S_ISLNK(rmode)) {
			काष्ठा working_वृक्ष_entry *entry;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(entry, path, dst_path);
			hashmap_entry_init(&entry->entry, strhash(dst_path));
			अगर (hashmap_get(&working_वृक्ष_dups, &entry->entry,
					शून्य)) {
				मुक्त(entry);
				जारी;
			}
			hashmap_add(&working_वृक्ष_dups, &entry->entry);

			अगर (!use_wt_file(repo, workdir, dst_path, &roid)) {
				अगर (checkout_path(rmode, &roid, dst_path,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_path);
					जाओ finish;
				}
			} अन्यथा अगर (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				काष्ठा cache_entry *ce2 =
					make_cache_entry(&wtindex, rmode, &roid,
							 dst_path, 0, 0);

				add_सूचकांक_entry(&wtindex, ce2,
						ADD_CACHE_JUST_APPEND);

				add_path(&rdir, rdir_len, dst_path);
				अगर (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_path);
					जाओ finish;
				}
				add_path(&wtdir, wtdir_len, dst_path);
				अगर (dt_options->symlinks) {
					अगर (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						जाओ finish;
					}
				} अन्यथा {
					काष्ठा stat st;
					अगर (stat(wtdir.buf, &st))
						st.st_mode = 0644;
					अगर (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_mode)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						जाओ finish;
					}
				}
			}
		}
	}

	ख_बंद(fp);
	fp = शून्य;
	अगर (finish_command(child)) {
		ret = error("error occurred running diff --raw");
		जाओ finish;
	}

	अगर (!i)
		जाओ finish;

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

	अगर (extcmd) {
		strvec_धकेलो(&cmd.args, extcmd);
	} अन्यथा {
		strvec_धकेलो(&cmd.args, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.args, ldir.buf, rdir.buf, शून्य);
	ret = run_command(&cmd);

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_सूचकांक(&wtindex);

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
	क्रम (i = 0; i < wtindex.cache_nr; i++) {
		काष्ठा hashmap_entry dummy;
		स्थिर अक्षर *name = wtindex.cache[i]->name;
		काष्ठा stat st;

		add_path(&rdir, rdir_len, name);
		अगर (lstat(rdir.buf, &st))
			जारी;

		अगर ((dt_options->symlinks && S_ISLNK(st.st_mode)) || !S_ISREG(st.st_mode))
			जारी;

		अगर (!indices_loaded) {
			काष्ठा lock_file lock = LOCK_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			अगर (repo_hold_lock_file_for_update(repo, &lock, buf.buf, 0) < 0 ||
			    write_locked_सूचकांक(&wtindex, &lock, COMMIT_LOCK)) {
				ret = error("could not write %s", buf.buf);
				जाओ finish;
			}
			changed_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			changed_files(repo, &tmp_modified, buf.buf, rdir.buf);
			add_path(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_entry_init(&dummy, strhash(name));
		अगर (hashmap_get(&tmp_modified, &dummy, name)) {
			add_path(&wtdir, wtdir_len, name);
			अगर (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} अन्यथा अगर (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_mode))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	अगर (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} अन्यथा {
		remove_dir_recursively(&tmpdir, 0);
		अगर (ret)
			warning(_("failed: %d"), ret);
	}

finish:
	अगर (fp)
		ख_बंद(fp);

	hashmap_clear_and_free(&working_वृक्ष_dups, काष्ठा working_वृक्ष_entry, entry);
	hashmap_clear_and_free(&wt_modified, काष्ठा path_entry, entry);
	hashmap_clear_and_free(&tmp_modified, काष्ठा path_entry, entry);
	hashmap_clear_and_free(&submodules, काष्ठा pair_entry, entry);
	hashmap_clear_and_free(&symlinks2, काष्ठा pair_entry, entry);
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

	वापस (ret < 0) ? 1 : ret;
}

अटल पूर्णांक run_file_diff(पूर्णांक prompt, स्थिर अक्षर *prefix,
			 काष्ठा child_process *child)
{
	strvec_धकेलो(&child->env, "GIT_PAGER=");
	strvec_धकेलो(&child->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	अगर (prompt > 0)
		strvec_धकेलो(&child->env, "GIT_DIFFTOOL_PROMPT=true");
	अन्यथा अगर (!prompt)
		strvec_धकेलो(&child->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	child->git_cmd = 1;
	child->dir = prefix;

	वापस run_command(child);
}

पूर्णांक cmd_difftool(पूर्णांक argc,
		 स्थिर अक्षर **argv,
		 स्थिर अक्षर *prefix,
		 काष्ठा repository *repo)
{
	पूर्णांक use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_सूचकांक = 0;
	अटल अक्षर *difftool_cmd = शून्य, *extcmd = शून्य;
	काष्ठा difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_code = 0
	};
	काष्ठा option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_SET_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "prompt", &prompt, शून्य,
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
		OPT_BOOL(0, "no-index", &no_सूचकांक, N_("passed to `diff`")),
		OPT_END()
	};
	काष्ठा child_process child = CHILD_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, prefix, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	अगर (tool_help)
		वापस print_tool_help();

	अगर (!no_सूचकांक && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	अगर (!no_सूचकांक){
		setup_work_वृक्ष(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_path(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_वृक्ष_ENVIRONMENT, absolute_path(repo_get_work_वृक्ष(repo)), 1);
	} अन्यथा अगर (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	अगर (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	अन्यथा अगर (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	अगर (difftool_cmd) {
		अगर (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		अन्यथा
			die(_("no <tool> given for --tool=<tool>"));
	}

	अगर (extcmd) {
		अगर (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		अन्यथा
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
	strvec_धकेलो(&child.args, "diff");
	अगर (no_सूचकांक)
		strvec_धकेलो(&child.args, "--no-index");
	अगर (dir_diff)
		strvec_pushl(&child.args, "--raw", "--no-abbrev", "-z", शून्य);
	strvec_pushv(&child.args, argv);

	अगर (dir_diff)
		वापस run_dir_diff(repo, &dt_options, extcmd, prefix, &child);
	वापस run_file_diff(prompt, prefix, &child);
}
