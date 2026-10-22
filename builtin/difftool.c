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

#அந்தர்பாவ "builtin.h"

#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "copy.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "repository.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "entry.h"
#அந்தர்பாவ "setup.h"

நிலையான மாறிலி எழுத்து *மாறிலி builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	வெற்று
};

கட்டமைப்பு difftool_options {
	முழு has_symlinks;
	முழு symlinks;
	முழு trust_exit_code;
};

நிலையான முழு difftool_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			   மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	கட்டமைப்பு difftool_options *dt_options = (கட்டமைப்பு difftool_options *)cb;
	எனில் (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_code = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

நிலையான முழு print_tool_help(வெறுமை)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "mergetool", "--tool-help=diff", வெற்று);
	திரும்பு run_command(&cmd);
}

நிலையான முழு parse_index_info(கட்டமைப்பு repository *repo,
			    எழுத்து *p, முழு *mode1, முழு *mode2,
			    கட்டமைப்பு object_id *oid1, கட்டமைப்பு object_id *oid2,
			    எழுத்து *status)
{
	எனில் (*p != ':')
		திரும்பு error("expected ':', got '%c'", *p);
	*mode1 = (முழு)strtol(p + 1, &p, 8);
	எனில் (*p != ' ')
		திரும்பு error("expected ' ', got '%c'", *p);
	*mode2 = (முழு)strtol(p + 1, &p, 8);
	எனில் (*p != ' ')
		திரும்பு error("expected ' ', got '%c'", *p);
	எனில் (parse_oid_hex_algop(++p, oid1, (மாறிலி எழுத்து **)&p, repo->hash_algo))
		திரும்பு error("expected object ID, got '%s'", p);
	எனில் (*p != ' ')
		திரும்பு error("expected ' ', got '%c'", *p);
	எனில் (parse_oid_hex_algop(++p, oid2, (மாறிலி எழுத்து **)&p, repo->hash_algo))
		திரும்பு error("expected object ID, got '%s'", p);
	எனில் (*p != ' ')
		திரும்பு error("expected ' ', got '%c'", *p);
	*status = *++p;
	எனில் (!*status)
		திரும்பு error("missing status");
	எனில் (p[1] && !isdigit(p[1]))
		திரும்பு error("unexpected trailer: '%s'", p + 1);
	திரும்பு 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
நிலையான வெறுமை add_path(கட்டமைப்பு strbuf *buf, size_t base_len, மாறிலி எழுத்து *path)
{
	strbuf_setlen(buf, base_len);
	எனில் (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, path);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
நிலையான முழு use_wt_file(கட்டமைப்பு repository *repo,
		       மாறிலி எழுத்து *workdir, மாறிலி எழுத்து *name,
		       கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு stat st;
	முழு use = 0;

	strbuf_addstr(&buf, workdir);
	add_path(&buf, buf.len, name);

	எனில் (!lstat(buf.buf, &st) && !S_ISLNK(st.st_mode)) {
		கட்டமைப்பு object_id wt_oid;
		முழு fd = open(buf.buf, O_RDONLY);

		எனில் (fd >= 0 &&
		    !index_fd(repo->index, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			எனில் (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} இல்லையெனில் எனில் (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	திரும்பு use;
}

கட்டமைப்பு working_மரம்_entry {
	கட்டமைப்பு hashmap_entry entry;
	எழுத்து path[FLEX_ARRAY];
};

நிலையான முழு working_மரம்_entry_cmp(மாறிலி வெறுமை *cmp_data UNUSED,
				  மாறிலி கட்டமைப்பு hashmap_entry *eptr,
				  மாறிலி கட்டமைப்பு hashmap_entry *entry_or_சாவி,
				  மாறிலி வெறுமை *keydata UNUSED)
{
	மாறிலி கட்டமைப்பு working_மரம்_entry *a, *b;

	a = container_of(eptr, மாறிலி கட்டமைப்பு working_மரம்_entry, entry);
	b = container_of(entry_or_சாவி, மாறிலி கட்டமைப்பு working_மரம்_entry, entry);

	திரும்பு strcmp(a->path, b->path);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
கட்டமைப்பு pair_entry {
	கட்டமைப்பு hashmap_entry entry;
	எழுத்து இடது[PATH_MAX], வலது[PATH_MAX];
	மாறிலி எழுத்து path[FLEX_ARRAY];
};

நிலையான முழு pair_cmp(மாறிலி வெறுமை *cmp_data UNUSED,
		    மாறிலி கட்டமைப்பு hashmap_entry *eptr,
		    மாறிலி கட்டமைப்பு hashmap_entry *entry_or_சாவி,
		    மாறிலி வெறுமை *keydata UNUSED)
{
	மாறிலி கட்டமைப்பு pair_entry *a, *b;

	a = container_of(eptr, மாறிலி கட்டமைப்பு pair_entry, entry);
	b = container_of(entry_or_சாவி, மாறிலி கட்டமைப்பு pair_entry, entry);

	திரும்பு strcmp(a->path, b->path);
}

நிலையான வெறுமை add_இடது_or_வலது(கட்டமைப்பு hashmap *map, மாறிலி எழுத்து *path,
			      மாறிலி எழுத்து *content, முழு is_வலது)
{
	கட்டமைப்பு pair_entry *e, *existing;

	FLEX_ALLOC_STR(e, path, path);
	hashmap_entry_init(&e->entry, strhash(path));
	existing = hashmap_get_entry(map, e, entry, வெற்று);
	எனில் (existing) {
		விடுவி(e);
		e = existing;
	} இல்லையெனில் {
		e->இடது[0] = e->வலது[0] = '\0';
		hashmap_add(map, &e->entry);
	}
	strlcpy(is_வலது ? e->வலது : e->இடது, content, PATH_MAX);
}

கட்டமைப்பு path_entry {
	கட்டமைப்பு hashmap_entry entry;
	எழுத்து path[FLEX_ARRAY];
};

நிலையான முழு path_entry_cmp(மாறிலி வெறுமை *cmp_data UNUSED,
			  மாறிலி கட்டமைப்பு hashmap_entry *eptr,
			  மாறிலி கட்டமைப்பு hashmap_entry *entry_or_சாவி,
			  மாறிலி வெறுமை *சாவி)
{
	மாறிலி கட்டமைப்பு path_entry *a, *b;

	a = container_of(eptr, மாறிலி கட்டமைப்பு path_entry, entry);
	b = container_of(entry_or_சாவி, மாறிலி கட்டமைப்பு path_entry, entry);

	திரும்பு strcmp(a->path, சாவி ? சாவி : b->path);
}

நிலையான வெறுமை changed_files(கட்டமைப்பு repository *repo,
			  கட்டமைப்பு hashmap *result, மாறிலி எழுத்து *index_path,
			  மாறிலி எழுத்து *workdir)
{
	கட்டமைப்பு child_process update_index = CHILD_PROCESS_INIT;
	கட்டமைப்பு child_process diff_files = CHILD_PROCESS_INIT;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	மாறிலி எழுத்து *git_dir = absolute_path(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_index.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", வெற்று);
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
		     "diff-files", "--name-only", "-z", வெற்று);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.out = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", index_path);
	எனில் (start_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.out, "r");
	போது (!strbuf_getline_nul(&buf, fp)) {
		கட்டமைப்பு path_entry *entry;
		FLEX_ALLOC_STR(entry, path, buf.buf);
		hashmap_entry_init(&entry->entry, strhash(buf.buf));
		hashmap_add(result, &entry->entry);
	}
	fclose(fp);
	எனில் (finish_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

நிலையான முழு ensure_leading_directories(கட்டமைப்பு repository *repo, எழுத்து *path)
{
	தேர்வு (safe_create_leading_directories(repo, path)) {
		நிலை SCLD_OK:
		நிலை SCLD_EXISTS:
			திரும்பு 0;
		இயல்பு:
			திரும்பு error(_("could not create leading directories "
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
நிலையான எழுத்து *get_symlink(கட்டமைப்பு repository *repo,
			 கட்டமைப்பு difftool_options *dt_options,
			 மாறிலி கட்டமைப்பு object_id *oid, மாறிலி எழுத்து *path)
{
	எழுத்து *data;
	எனில் (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		கட்டமைப்பு strbuf link = STRBUF_INIT;
		எனில் (dt_options->has_symlinks) {
			எனில் (strbuf_readlink(&link, path, strlen(path)))
				die(_("could not read symlink %s"), path);
		} இல்லையெனில் எனில் (strbuf_read_file(&link, path, 128))
			die(_("could not read symlink file %s"), path);

		data = strbuf_detach(&link, வெற்று);
	} இல்லையெனில் {
		எண்ணிக்கை object_type type;
		size_t size;
		data = odb_read_object(repo->objects, oid, &type, &size);
		எனில் (!data)
			die(_("could not read object %s for symlink %s"),
				oid_to_hex(oid), path);
	}

	திரும்பு data;
}

நிலையான முழு checkout_path(unsigned mode, கட்டமைப்பு object_id *oid,
			 மாறிலி எழுத்து *path, மாறிலி கட்டமைப்பு checkout *state)
{
	கட்டமைப்பு cache_entry *ce;
	முழு ret;

	ce = make_transient_cache_entry(mode, oid, path, 0, வெற்று);
	ret = checkout_entry(ce, state, வெற்று, வெற்று);

	discard_cache_entry(ce);
	திரும்பு ret;
}

நிலையான வெறுமை write_file_in_directory(கட்டமைப்பு repository *repo,
				    கட்டமைப்பு strbuf *dir, size_t dir_len,
				    மாறிலி எழுத்து *path, மாறிலி எழுத்து *content)
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
நிலையான வெறுமை write_standin_files(கட்டமைப்பு repository *repo,
				கட்டமைப்பு pair_entry *entry,
				கட்டமைப்பு strbuf *ldir, size_t ldir_len,
				கட்டமைப்பு strbuf *rdir, size_t rdir_len)
{
	எனில் (*entry->இடது)
		write_file_in_directory(repo, ldir, ldir_len, entry->path, entry->இடது);
	எனில் (*entry->வலது)
		write_file_in_directory(repo, rdir, rdir_len, entry->path, entry->வலது);
}

நிலையான முழு run_dir_diff(கட்டமைப்பு repository *repo,
			கட்டமைப்பு difftool_options *dt_options,
			மாறிலி எழுத்து *extcmd, மாறிலி எழுத்து *prefix,
			கட்டமைப்பு child_process *child)
{
	கட்டமைப்பு strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	கட்டமைப்பு strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	கட்டமைப்பு strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	கட்டமைப்பு strbuf wtdir = STRBUF_INIT;
	கட்டமைப்பு strbuf tmpdir = STRBUF_INIT;
	எழுத்து *lbase_dir = வெற்று, *rbase_dir = வெற்று;
	size_t ldir_len, rdir_len, wtdir_len;
	மாறிலி எழுத்து *workdir, *tmp;
	முழு ret = 0;
	size_t i;
	FILE *fp = வெற்று;
	கட்டமைப்பு hashmap working_மரம்_dups = HASHMAP_INIT(working_மரம்_entry_cmp,
							வெற்று);
	கட்டமைப்பு hashmap submodules = HASHMAP_INIT(pair_cmp, வெற்று);
	கட்டமைப்பு hashmap symlinks2 = HASHMAP_INIT(pair_cmp, வெற்று);
	கட்டமைப்பு hashmap_iter iter;
	கட்டமைப்பு pair_entry *entry;
	கட்டமைப்பு index_state wtindex = INDEX_STATE_INIT(repo);
	கட்டமைப்பு checkout lstate, rstate;
	முழு err = 0;
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;
	கட்டமைப்பு hashmap wt_modified = HASHMAP_INIT(path_entry_cmp, வெற்று);
	கட்டமைப்பு hashmap tmp_modified = HASHMAP_INIT(path_entry_cmp, வெற்று);
	முழு indices_loaded = 0;

	workdir = repo_get_work_மரம்(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_add_absolute_path(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	எனில் (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		செல் finish;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	எனில் (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, அளவு(lstate));
	lstate.base_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.base_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, அளவு(rstate));
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
	எனில் (start_command(child))
		die("could not obtain raw diff");
	fp = xfdopen(child->out, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	போது (!strbuf_getline_nul(&info, fp)) {
		முழு lmode, rmode;
		கட்டமைப்பு object_id loid, roid;
		எழுத்து status;
		மாறிலி எழுத்து *src_path, *dst_path;

		எனில் (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		எனில் (parse_index_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &status))
			நிறுத்து;
		எனில் (strbuf_getline_nul(&lpath, fp))
			நிறுத்து;
		src_path = lpath.buf;

		i++;
		எனில் (status != 'C' && status != 'R') {
			dst_path = src_path;
		} இல்லையெனில் {
			எனில் (strbuf_getline_nul(&rpath, fp))
				நிறுத்து;
			dst_path = rpath.buf;
		}

		எனில் (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&loid));
			add_இடது_or_வலது(&submodules, src_path, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&roid));
			எனில் (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			add_இடது_or_வலது(&submodules, dst_path, buf.buf, 1);
			தொடர்;
		}

		எனில் (S_ISLNK(lmode)) {
			எழுத்து *content = get_symlink(repo, dt_options, &loid, src_path);
			add_இடது_or_வலது(&symlinks2, src_path, content, 0);
			விடுவி(content);
		}

		எனில் (S_ISLNK(rmode)) {
			எழுத்து *content = get_symlink(repo, dt_options, &roid, dst_path);
			add_இடது_or_வலது(&symlinks2, dst_path, content, 1);
			விடுவி(content);
		}

		எனில் (lmode && status != 'C') {
			எனில் (checkout_path(lmode, &loid, src_path, &lstate)) {
				ret = error("could not write '%s'", src_path);
				செல் finish;
			}
		}

		எனில் (rmode && !S_ISLNK(rmode)) {
			கட்டமைப்பு working_மரம்_entry *entry;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(entry, path, dst_path);
			hashmap_entry_init(&entry->entry, strhash(dst_path));
			எனில் (hashmap_get(&working_மரம்_dups, &entry->entry,
					வெற்று)) {
				விடுவி(entry);
				தொடர்;
			}
			hashmap_add(&working_மரம்_dups, &entry->entry);

			எனில் (!use_wt_file(repo, workdir, dst_path, &roid)) {
				எனில் (checkout_path(rmode, &roid, dst_path,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_path);
					செல் finish;
				}
			} இல்லையெனில் எனில் (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				கட்டமைப்பு cache_entry *ce2 =
					make_cache_entry(&wtindex, rmode, &roid,
							 dst_path, 0, 0);

				add_index_entry(&wtindex, ce2,
						ADD_CACHE_JUST_APPEND);

				add_path(&rdir, rdir_len, dst_path);
				எனில் (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_path);
					செல் finish;
				}
				add_path(&wtdir, wtdir_len, dst_path);
				எனில் (dt_options->symlinks) {
					எனில் (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						செல் finish;
					}
				} இல்லையெனில் {
					கட்டமைப்பு stat st;
					எனில் (stat(wtdir.buf, &st))
						st.st_mode = 0644;
					எனில் (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_mode)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						செல் finish;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = வெற்று;
	எனில் (finish_command(child)) {
		ret = error("error occurred running diff --raw");
		செல் finish;
	}

	எனில் (!i)
		செல் finish;

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

	எனில் (extcmd) {
		strvec_தள்ளு(&cmd.args, extcmd);
	} இல்லையெனில் {
		strvec_தள்ளு(&cmd.args, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.args, ldir.buf, rdir.buf, வெற்று);
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
	க்கு (i = 0; i < wtindex.cache_nr; i++) {
		கட்டமைப்பு hashmap_entry dummy;
		மாறிலி எழுத்து *name = wtindex.cache[i]->name;
		கட்டமைப்பு stat st;

		add_path(&rdir, rdir_len, name);
		எனில் (lstat(rdir.buf, &st))
			தொடர்;

		எனில் ((dt_options->symlinks && S_ISLNK(st.st_mode)) || !S_ISREG(st.st_mode))
			தொடர்;

		எனில் (!indices_loaded) {
			கட்டமைப்பு lock_file lock = LOCK_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			எனில் (repo_hold_lock_file_for_update(repo, &lock, buf.buf, 0) < 0 ||
			    write_locked_index(&wtindex, &lock, COMMIT_LOCK)) {
				ret = error("could not write %s", buf.buf);
				செல் finish;
			}
			changed_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			changed_files(repo, &tmp_modified, buf.buf, rdir.buf);
			add_path(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_entry_init(&dummy, strhash(name));
		எனில் (hashmap_get(&tmp_modified, &dummy, name)) {
			add_path(&wtdir, wtdir_len, name);
			எனில் (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} இல்லையெனில் எனில் (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_mode))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	எனில் (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} இல்லையெனில் {
		remove_dir_recursively(&tmpdir, 0);
		எனில் (ret)
			warning(_("failed: %d"), ret);
	}

finish:
	எனில் (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_மரம்_dups, கட்டமைப்பு working_மரம்_entry, entry);
	hashmap_clear_and_free(&wt_modified, கட்டமைப்பு path_entry, entry);
	hashmap_clear_and_free(&tmp_modified, கட்டமைப்பு path_entry, entry);
	hashmap_clear_and_free(&submodules, கட்டமைப்பு pair_entry, entry);
	hashmap_clear_and_free(&symlinks2, கட்டமைப்பு pair_entry, entry);
	release_index(&wtindex);
	விடுவி(lbase_dir);
	விடுவி(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	திரும்பு (ret < 0) ? 1 : ret;
}

நிலையான முழு run_file_diff(முழு prompt, மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு child_process *child)
{
	strvec_தள்ளு(&child->env, "GIT_PAGER=");
	strvec_தள்ளு(&child->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	எனில் (prompt > 0)
		strvec_தள்ளு(&child->env, "GIT_DIFFTOOL_PROMPT=true");
	இல்லையெனில் எனில் (!prompt)
		strvec_தள்ளு(&child->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	child->git_cmd = 1;
	child->dir = prefix;

	திரும்பு run_command(child);
}

முழு cmd_difftool(முழு argc,
		 மாறிலி எழுத்து **argv,
		 மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo)
{
	முழு use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_index = 0;
	நிலையான எழுத்து *difftool_cmd = வெற்று, *extcmd = வெற்று;
	கட்டமைப்பு difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_code = 0
	};
	கட்டமைப்பு option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_SET_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "prompt", &prompt, வெற்று,
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
	கட்டமைப்பு child_process child = CHILD_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, prefix, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	எனில் (tool_help)
		திரும்பு print_tool_help();

	எனில் (!no_index && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	எனில் (!no_index){
		setup_work_மரம்(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_path(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_மரம்_ENVIRONMENT, absolute_path(repo_get_work_மரம்(repo)), 1);
	} இல்லையெனில் எனில் (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	எனில் (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	இல்லையெனில் எனில் (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	எனில் (difftool_cmd) {
		எனில் (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		இல்லையெனில்
			die(_("no <tool> given for --tool=<tool>"));
	}

	எனில் (extcmd) {
		எனில் (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		இல்லையெனில்
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
	strvec_தள்ளு(&child.args, "diff");
	எனில் (no_index)
		strvec_தள்ளு(&child.args, "--no-index");
	எனில் (dir_diff)
		strvec_pushl(&child.args, "--raw", "--no-abbrev", "-z", வெற்று);
	strvec_pushv(&child.args, argv);

	எனில் (dir_diff)
		திரும்பு run_dir_diff(repo, &dt_options, extcmd, prefix, &child);
	திரும்பு run_file_diff(prompt, prefix, &child);
}
