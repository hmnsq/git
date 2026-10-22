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
	पूर्णाङ्क trust_exit_code;
};

स्थिर पूर्णाङ्क difftool_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मूल्य,
			   ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना difftool_options *dt_options = (संरचना difftool_options *)cb;
	यदि (!म_भेद(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_code = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!म_भेद(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	प्रत्यागम git_default_config(var, मूल्य, ctx, cb);
}

स्थिर पूर्णाङ्क print_tool_help(शून्य)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "mergetool", "--tool-help=diff", रिक्त);
	प्रत्यागम run_command(&cmd);
}

स्थिर पूर्णाङ्क parse_index_info(संरचना repository *repo,
			    वर्ण *p, पूर्णाङ्क *mode1, पूर्णाङ्क *mode2,
			    संरचना object_id *oid1, संरचना object_id *oid2,
			    वर्ण *status)
{
	यदि (*p != ':')
		प्रत्यागम error("expected ':', got '%c'", *p);
	*mode1 = (पूर्णाङ्क)म_से_दीर्घ(p + 1, &p, 8);
	यदि (*p != ' ')
		प्रत्यागम error("expected ' ', got '%c'", *p);
	*mode2 = (पूर्णाङ्क)म_से_दीर्घ(p + 1, &p, 8);
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
	*status = *++p;
	यदि (!*status)
		प्रत्यागम error("missing status");
	यदि (p[1] && !है_अंक(p[1]))
		प्रत्यागम error("unexpected trailer: '%s'", p + 1);
	प्रत्यागम 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
स्थिर शून्य add_path(संरचना strbuf *buf, माप_प्रकार base_len, ध्रुव वर्ण *path)
{
	strbuf_setlen(buf, base_len);
	यदि (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, path);
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
	add_path(&buf, buf.len, name);

	यदि (!lstat(buf.buf, &st) && !S_ISLNK(st.st_mode)) {
		संरचना object_id wt_oid;
		पूर्णाङ्क fd = खोलो(buf.buf, O_RDONLY);

		यदि (fd >= 0 &&
		    !index_fd(repo->index, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
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

संरचना working_वृक्ष_entry {
	संरचना hashmap_entry entry;
	वर्ण path[FLEX_ARRAY];
};

स्थिर पूर्णाङ्क working_वृक्ष_entry_cmp(ध्रुव शून्य *cmp_data UNUSED,
				  ध्रुव संरचना hashmap_entry *eptr,
				  ध्रुव संरचना hashmap_entry *entry_or_कुंजी,
				  ध्रुव शून्य *keydata UNUSED)
{
	ध्रुव संरचना working_वृक्ष_entry *a, *b;

	a = container_of(eptr, ध्रुव संरचना working_वृक्ष_entry, entry);
	b = container_of(entry_or_कुंजी, ध्रुव संरचना working_वृक्ष_entry, entry);

	प्रत्यागम म_भेद(a->path, b->path);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
संरचना pair_entry {
	संरचना hashmap_entry entry;
	वर्ण बायाँ[PATH_MAX], दायाँ[PATH_MAX];
	ध्रुव वर्ण path[FLEX_ARRAY];
};

स्थिर पूर्णाङ्क pair_cmp(ध्रुव शून्य *cmp_data UNUSED,
		    ध्रुव संरचना hashmap_entry *eptr,
		    ध्रुव संरचना hashmap_entry *entry_or_कुंजी,
		    ध्रुव शून्य *keydata UNUSED)
{
	ध्रुव संरचना pair_entry *a, *b;

	a = container_of(eptr, ध्रुव संरचना pair_entry, entry);
	b = container_of(entry_or_कुंजी, ध्रुव संरचना pair_entry, entry);

	प्रत्यागम म_भेद(a->path, b->path);
}

स्थिर शून्य add_बायाँ_or_दायाँ(संरचना hashmap *map, ध्रुव वर्ण *path,
			      ध्रुव वर्ण *content, पूर्णाङ्क is_दायाँ)
{
	संरचना pair_entry *e, *existing;

	FLEX_ALLOC_STR(e, path, path);
	hashmap_entry_init(&e->entry, strhash(path));
	existing = hashmap_get_entry(map, e, entry, रिक्त);
	यदि (existing) {
		मुक्त(e);
		e = existing;
	} अन्यथा {
		e->बायाँ[0] = e->दायाँ[0] = '\0';
		hashmap_add(map, &e->entry);
	}
	strlcpy(is_दायाँ ? e->दायाँ : e->बायाँ, content, PATH_MAX);
}

संरचना path_entry {
	संरचना hashmap_entry entry;
	वर्ण path[FLEX_ARRAY];
};

स्थिर पूर्णाङ्क path_entry_cmp(ध्रुव शून्य *cmp_data UNUSED,
			  ध्रुव संरचना hashmap_entry *eptr,
			  ध्रुव संरचना hashmap_entry *entry_or_कुंजी,
			  ध्रुव शून्य *कुंजी)
{
	ध्रुव संरचना path_entry *a, *b;

	a = container_of(eptr, ध्रुव संरचना path_entry, entry);
	b = container_of(entry_or_कुंजी, ध्रुव संरचना path_entry, entry);

	प्रत्यागम म_भेद(a->path, कुंजी ? कुंजी : b->path);
}

स्थिर शून्य changed_files(संरचना repository *repo,
			  संरचना hashmap *result, ध्रुव वर्ण *index_path,
			  ध्रुव वर्ण *workdir)
{
	संरचना child_process update_index = CHILD_PROCESS_INIT;
	संरचना child_process diff_files = CHILD_PROCESS_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	ध्रुव वर्ण *git_dir = absolute_path(repo_get_git_dir(repo));
	खाता *fp;

	strvec_pushl(&update_index.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", रिक्त);
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
		     "diff-files", "--name-only", "-z", रिक्त);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.out = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", index_path);
	यदि (start_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.out, "r");
	यावत् (!strbuf_getline_nul(&buf, fp)) {
		संरचना path_entry *entry;
		FLEX_ALLOC_STR(entry, path, buf.buf);
		hashmap_entry_init(&entry->entry, strhash(buf.buf));
		hashmap_add(result, &entry->entry);
	}
	ख_बंद(fp);
	यदि (finish_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

स्थिर पूर्णाङ्क ensure_leading_directories(संरचना repository *repo, वर्ण *path)
{
	विकल्प (safe_create_leading_directories(repo, path)) {
		पक्ष SCLD_OK:
		पक्ष SCLD_EXISTS:
			प्रत्यागम 0;
		स्वभाव:
			प्रत्यागम error(_("could not create leading directories "
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
स्थिर वर्ण *get_symlink(संरचना repository *repo,
			 संरचना difftool_options *dt_options,
			 ध्रुव संरचना object_id *oid, ध्रुव वर्ण *path)
{
	वर्ण *data;
	यदि (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		संरचना strbuf link = STRBUF_INIT;
		यदि (dt_options->has_symlinks) {
			यदि (strbuf_readlink(&link, path, म_माप(path)))
				die(_("could not read symlink %s"), path);
		} अन्यथा यदि (strbuf_read_file(&link, path, 128))
			die(_("could not read symlink file %s"), path);

		data = strbuf_detach(&link, रिक्त);
	} अन्यथा {
		गणना object_type type;
		माप_प्रकार size;
		data = odb_read_object(repo->objects, oid, &type, &size);
		यदि (!data)
			die(_("could not read object %s for symlink %s"),
				oid_to_hex(oid), path);
	}

	प्रत्यागम data;
}

स्थिर पूर्णाङ्क checkout_path(अचिन्हित mode, संरचना object_id *oid,
			 ध्रुव वर्ण *path, ध्रुव संरचना checkout *state)
{
	संरचना cache_entry *ce;
	पूर्णाङ्क ret;

	ce = make_transient_cache_entry(mode, oid, path, 0, रिक्त);
	ret = checkout_entry(ce, state, रिक्त, रिक्त);

	discard_cache_entry(ce);
	प्रत्यागम ret;
}

स्थिर शून्य write_file_in_directory(संरचना repository *repo,
				    संरचना strbuf *dir, माप_प्रकार dir_len,
				    ध्रुव वर्ण *path, ध्रुव वर्ण *content)
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
स्थिर शून्य write_standin_files(संरचना repository *repo,
				संरचना pair_entry *entry,
				संरचना strbuf *ldir, माप_प्रकार ldir_len,
				संरचना strbuf *rdir, माप_प्रकार rdir_len)
{
	यदि (*entry->बायाँ)
		write_file_in_directory(repo, ldir, ldir_len, entry->path, entry->बायाँ);
	यदि (*entry->दायाँ)
		write_file_in_directory(repo, rdir, rdir_len, entry->path, entry->दायाँ);
}

स्थिर पूर्णाङ्क run_dir_diff(संरचना repository *repo,
			संरचना difftool_options *dt_options,
			ध्रुव वर्ण *extcmd, ध्रुव वर्ण *prefix,
			संरचना child_process *child)
{
	संरचना strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	संरचना strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	संरचना strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	संरचना strbuf wtdir = STRBUF_INIT;
	संरचना strbuf tmpdir = STRBUF_INIT;
	वर्ण *lbase_dir = रिक्त, *rbase_dir = रिक्त;
	माप_प्रकार ldir_len, rdir_len, wtdir_len;
	ध्रुव वर्ण *workdir, *tmp;
	पूर्णाङ्क ret = 0;
	माप_प्रकार i;
	खाता *fp = रिक्त;
	संरचना hashmap working_वृक्ष_dups = HASHMAP_INIT(working_वृक्ष_entry_cmp,
							रिक्त);
	संरचना hashmap submodules = HASHMAP_INIT(pair_cmp, रिक्त);
	संरचना hashmap symlinks2 = HASHMAP_INIT(pair_cmp, रिक्त);
	संरचना hashmap_iter iter;
	संरचना pair_entry *entry;
	संरचना index_state wtindex = INDEX_STATE_INIT(repo);
	संरचना checkout lstate, rstate;
	पूर्णाङ्क err = 0;
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	संरचना hashmap wt_modified = HASHMAP_INIT(path_entry_cmp, रिक्त);
	संरचना hashmap tmp_modified = HASHMAP_INIT(path_entry_cmp, रिक्त);
	पूर्णाङ्क indices_loaded = 0;

	workdir = repo_get_work_वृक्ष(repo);

	/* Setup temp directories */
	tmp = दो_पर्या("TMPDIR");
	strbuf_add_absolute_path(&tmpdir, tmp ? tmp : "/tmp");
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
	सूची_गढ़ो(ldir.buf, 0700);
	सूची_गढ़ो(rdir.buf, 0700);

	स_रखो(&lstate, 0, परिमाण(lstate));
	lstate.base_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.base_dir_len = ldir.len;
	lstate.force = 1;
	स_रखो(&rstate, 0, परिमाण(rstate));
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
	यदि (start_command(child))
		die("could not obtain raw diff");
	fp = xfdopen(child->out, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	यावत् (!strbuf_getline_nul(&info, fp)) {
		पूर्णाङ्क lmode, rmode;
		संरचना object_id loid, roid;
		वर्ण status;
		ध्रुव वर्ण *src_path, *dst_path;

		यदि (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		यदि (parse_index_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &status))
			भङ्ग;
		यदि (strbuf_getline_nul(&lpath, fp))
			भङ्ग;
		src_path = lpath.buf;

		i++;
		यदि (status != 'C' && status != 'R') {
			dst_path = src_path;
		} अन्यथा {
			यदि (strbuf_getline_nul(&rpath, fp))
				भङ्ग;
			dst_path = rpath.buf;
		}

		यदि (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&loid));
			add_बायाँ_or_दायाँ(&submodules, src_path, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&roid));
			यदि (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			add_बायाँ_or_दायाँ(&submodules, dst_path, buf.buf, 1);
			अनुवर्त;
		}

		यदि (S_ISLNK(lmode)) {
			वर्ण *content = get_symlink(repo, dt_options, &loid, src_path);
			add_बायाँ_or_दायाँ(&symlinks2, src_path, content, 0);
			मुक्त(content);
		}

		यदि (S_ISLNK(rmode)) {
			वर्ण *content = get_symlink(repo, dt_options, &roid, dst_path);
			add_बायाँ_or_दायाँ(&symlinks2, dst_path, content, 1);
			मुक्त(content);
		}

		यदि (lmode && status != 'C') {
			यदि (checkout_path(lmode, &loid, src_path, &lstate)) {
				ret = error("could not write '%s'", src_path);
				गच्छ finish;
			}
		}

		यदि (rmode && !S_ISLNK(rmode)) {
			संरचना working_वृक्ष_entry *entry;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(entry, path, dst_path);
			hashmap_entry_init(&entry->entry, strhash(dst_path));
			यदि (hashmap_get(&working_वृक्ष_dups, &entry->entry,
					रिक्त)) {
				मुक्त(entry);
				अनुवर्त;
			}
			hashmap_add(&working_वृक्ष_dups, &entry->entry);

			यदि (!use_wt_file(repo, workdir, dst_path, &roid)) {
				यदि (checkout_path(rmode, &roid, dst_path,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_path);
					गच्छ finish;
				}
			} अन्यथा यदि (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				संरचना cache_entry *ce2 =
					make_cache_entry(&wtindex, rmode, &roid,
							 dst_path, 0, 0);

				add_index_entry(&wtindex, ce2,
						ADD_CACHE_JUST_APPEND);

				add_path(&rdir, rdir_len, dst_path);
				यदि (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_path);
					गच्छ finish;
				}
				add_path(&wtdir, wtdir_len, dst_path);
				यदि (dt_options->symlinks) {
					यदि (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						गच्छ finish;
					}
				} अन्यथा {
					संरचना stat st;
					यदि (stat(wtdir.buf, &st))
						st.st_mode = 0644;
					यदि (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_mode)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						गच्छ finish;
					}
				}
			}
		}
	}

	ख_बंद(fp);
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

	यदि (extcmd) {
		strvec_धकेलो(&cmd.args, extcmd);
	} अन्यथा {
		strvec_धकेलो(&cmd.args, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.args, ldir.buf, rdir.buf, रिक्त);
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
	प्रति (i = 0; i < wtindex.cache_nr; i++) {
		संरचना hashmap_entry dummy;
		ध्रुव वर्ण *name = wtindex.cache[i]->name;
		संरचना stat st;

		add_path(&rdir, rdir_len, name);
		यदि (lstat(rdir.buf, &st))
			अनुवर्त;

		यदि ((dt_options->symlinks && S_ISLNK(st.st_mode)) || !S_ISREG(st.st_mode))
			अनुवर्त;

		यदि (!indices_loaded) {
			संरचना lock_file lock = LOCK_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			यदि (repo_hold_lock_file_for_update(repo, &lock, buf.buf, 0) < 0 ||
			    write_locked_index(&wtindex, &lock, COMMIT_LOCK)) {
				ret = error("could not write %s", buf.buf);
				गच्छ finish;
			}
			changed_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			changed_files(repo, &tmp_modified, buf.buf, rdir.buf);
			add_path(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_entry_init(&dummy, strhash(name));
		यदि (hashmap_get(&tmp_modified, &dummy, name)) {
			add_path(&wtdir, wtdir_len, name);
			यदि (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} अन्यथा यदि (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_mode))
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
		ख_बंद(fp);

	hashmap_clear_and_free(&working_वृक्ष_dups, संरचना working_वृक्ष_entry, entry);
	hashmap_clear_and_free(&wt_modified, संरचना path_entry, entry);
	hashmap_clear_and_free(&tmp_modified, संरचना path_entry, entry);
	hashmap_clear_and_free(&submodules, संरचना pair_entry, entry);
	hashmap_clear_and_free(&symlinks2, संरचना pair_entry, entry);
	release_index(&wtindex);
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

स्थिर पूर्णाङ्क run_file_diff(पूर्णाङ्क prompt, ध्रुव वर्ण *prefix,
			 संरचना child_process *child)
{
	strvec_धकेलो(&child->env, "GIT_PAGER=");
	strvec_धकेलो(&child->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	यदि (prompt > 0)
		strvec_धकेलो(&child->env, "GIT_DIFFTOOL_PROMPT=true");
	अन्यथा यदि (!prompt)
		strvec_धकेलो(&child->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	child->git_cmd = 1;
	child->dir = prefix;

	प्रत्यागम run_command(child);
}

पूर्णाङ्क cmd_difftool(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *prefix,
		 संरचना repository *repo)
{
	पूर्णाङ्क use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_index = 0;
	स्थिर वर्ण *difftool_cmd = रिक्त, *extcmd = रिक्त;
	संरचना difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_code = 0
	};
	संरचना option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_SET_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "prompt", &prompt, रिक्त,
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
	संरचना child_process child = CHILD_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, prefix, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	यदि (tool_help)
		प्रत्यागम print_tool_help();

	यदि (!no_index && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	यदि (!no_index){
		setup_work_वृक्ष(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_path(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_वृक्ष_ENVIRONMENT, absolute_path(repo_get_work_वृक्ष(repo)), 1);
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
	       dt_options.trust_exit_code ? "true" : "false", 1);

	/*
	 * In directory diff mode, 'git-difftool--helper' is called once
	 * to compare the a / b directories. In file diff mode, 'git diff'
	 * will invoke a separate instance of 'git-difftool--helper' for
	 * each file that changed.
	 */
	strvec_धकेलो(&child.args, "diff");
	यदि (no_index)
		strvec_धकेलो(&child.args, "--no-index");
	यदि (dir_diff)
		strvec_pushl(&child.args, "--raw", "--no-abbrev", "-z", रिक्त);
	strvec_pushv(&child.args, argv);

	यदि (dir_diff)
		प्रत्यागम run_dir_diff(repo, &dt_options, extcmd, prefix, &child);
	प्रत्यागम run_file_diff(prompt, prefix, &child);
}
