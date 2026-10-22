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

#include "builtin.h"

#include "abspath.h"
#include "config.h"
#include "copy.h"
#include "run-command.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "parse-options.h"
#include "path.h"
#include "read-cache-ll.h"
#include "repository.h"
#include "sparse-index.h"
#include "strvec.h"
#include "strbuf.h"
#include "lockfile.h"
#include "object-file.h"
#include "odb.h"
#include "dir.h"
#include "entry.h"
#include "setup.h"

static const var_na *const builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	NULL
};

sa_nracanaa difftool_options {
	පූර්ණ_සංඛ්‍යා has_symlinks;
	පූර්ණ_සංඛ්‍යා symlinks;
	පූර්ණ_සංඛ්‍යා trust_exit_code;
};

static පූර්ණ_සංඛ්‍යා difftool_config(const var_na *var, const var_na *value,
			   const sa_nracanaa config_context *ctx, void *cb)
{
	sa_nracanaa difftool_options *dt_options = (sa_nracanaa difftool_options *)cb;
	නම් (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_code = git_config_bool(var, value);
		ආපසු 0;
	}
	නම් (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, value);
		ආපසු 0;
	}

	ආපසු git_default_config(var, value, ctx, cb);
}

static පූර්ණ_සංඛ්‍යා print_tool_help(void)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "mergetool", "--tool-help=diff", NULL);
	ආපසු run_command(&cmd);
}

static පූර්ණ_සංඛ්‍යා parse_index_info(sa_nracanaa repository *repo,
			    var_na *p, පූර්ණ_සංඛ්‍යා *mode1, පූර්ණ_සංඛ්‍යා *mode2,
			    sa_nracanaa object_id *oid1, sa_nracanaa object_id *oid2,
			    var_na *status)
{
	නම් (*p != ':')
		ආපසු error("expected ':', got '%c'", *p);
	*mode1 = (පූර්ණ_සංඛ්‍යා)strtol(p + 1, &p, 8);
	නම් (*p != ' ')
		ආපසු error("expected ' ', got '%c'", *p);
	*mode2 = (පූර්ණ_සංඛ්‍යා)strtol(p + 1, &p, 8);
	නම් (*p != ' ')
		ආපසු error("expected ' ', got '%c'", *p);
	නම් (parse_oid_hex_algop(++p, oid1, (const var_na **)&p, repo->hash_algo))
		ආපසු error("expected object ID, got '%s'", p);
	නම් (*p != ' ')
		ආපසු error("expected ' ', got '%c'", *p);
	නම් (parse_oid_hex_algop(++p, oid2, (const var_na **)&p, repo->hash_algo))
		ආපසු error("expected object ID, got '%s'", p);
	නම් (*p != ' ')
		ආපසු error("expected ' ', got '%c'", *p);
	*status = *++p;
	නම් (!*status)
		ආපසු error("missing status");
	නම් (p[1] && !isdigit(p[1]))
		ආපසු error("unexpected trailer: '%s'", p + 1);
	ආපසු 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
static void add_path(sa_nracanaa strbuf *buf, size_t base_len, const var_na *path)
{
	strbuf_setlen(buf, base_len);
	නම් (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, path);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
static පූර්ණ_සංඛ්‍යා use_wt_file(sa_nracanaa repository *repo,
		       const var_na *workdir, const var_na *name,
		       sa_nracanaa object_id *oid)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa stat st;
	පූර්ණ_සංඛ්‍යා use = 0;

	strbuf_addstr(&buf, workdir);
	add_path(&buf, buf.len, name);

	නම් (!lstat(buf.buf, &st) && !S_ISLNK(st.st_mode)) {
		sa_nracanaa object_id wt_oid;
		පූර්ණ_සංඛ්‍යා fd = open(buf.buf, O_RDONLY);

		නම් (fd >= 0 &&
		    !index_fd(repo->index, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			නම් (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} නැතහොත් නම් (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	ආපසු use;
}

sa_nracanaa working_tree_entry {
	sa_nracanaa hashmap_entry entry;
	var_na path[FLEX_ARRAY];
};

static පූර්ණ_සංඛ්‍යා working_tree_entry_cmp(const void *cmp_data UNUSED,
				  const sa_nracanaa hashmap_entry *eptr,
				  const sa_nracanaa hashmap_entry *entry_or_key,
				  const void *keydata UNUSED)
{
	const sa_nracanaa working_tree_entry *a, *b;

	a = container_of(eptr, const sa_nracanaa working_tree_entry, entry);
	b = container_of(entry_or_key, const sa_nracanaa working_tree_entry, entry);

	ආපසු strcmp(a->path, b->path);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
sa_nracanaa pair_entry {
	sa_nracanaa hashmap_entry entry;
	var_na left[PATH_MAX], right[PATH_MAX];
	const var_na path[FLEX_ARRAY];
};

static පූර්ණ_සංඛ්‍යා pair_cmp(const void *cmp_data UNUSED,
		    const sa_nracanaa hashmap_entry *eptr,
		    const sa_nracanaa hashmap_entry *entry_or_key,
		    const void *keydata UNUSED)
{
	const sa_nracanaa pair_entry *a, *b;

	a = container_of(eptr, const sa_nracanaa pair_entry, entry);
	b = container_of(entry_or_key, const sa_nracanaa pair_entry, entry);

	ආපසු strcmp(a->path, b->path);
}

static void add_left_or_right(sa_nracanaa hashmap *map, const var_na *path,
			      const var_na *content, පූර්ණ_සංඛ්‍යා is_right)
{
	sa_nracanaa pair_entry *e, *existing;

	FLEX_ALLOC_STR(e, path, path);
	hashmap_entry_init(&e->entry, strhash(path));
	existing = hashmap_get_entry(map, e, entry, NULL);
	නම් (existing) {
		free(e);
		e = existing;
	} නැතහොත් {
		e->left[0] = e->right[0] = '\0';
		hashmap_add(map, &e->entry);
	}
	strlcpy(is_right ? e->right : e->left, content, PATH_MAX);
}

sa_nracanaa path_entry {
	sa_nracanaa hashmap_entry entry;
	var_na path[FLEX_ARRAY];
};

static පූර්ණ_සංඛ්‍යා path_entry_cmp(const void *cmp_data UNUSED,
			  const sa_nracanaa hashmap_entry *eptr,
			  const sa_nracanaa hashmap_entry *entry_or_key,
			  const void *key)
{
	const sa_nracanaa path_entry *a, *b;

	a = container_of(eptr, const sa_nracanaa path_entry, entry);
	b = container_of(entry_or_key, const sa_nracanaa path_entry, entry);

	ආපසු strcmp(a->path, key ? key : b->path);
}

static void changed_files(sa_nracanaa repository *repo,
			  sa_nracanaa hashmap *result, const var_na *index_path,
			  const var_na *workdir)
{
	sa_nracanaa child_process update_index = CHILD_PROCESS_INIT;
	sa_nracanaa child_process diff_files = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	const var_na *git_dir = absolute_path(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_index.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", NULL);
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
		     "diff-files", "--name-only", "-z", NULL);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.out = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", index_path);
	නම් (start_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.out, "r");
	අතරතුර (!strbuf_getline_nul(&buf, fp)) {
		sa_nracanaa path_entry *entry;
		FLEX_ALLOC_STR(entry, path, buf.buf);
		hashmap_entry_init(&entry->entry, strhash(buf.buf));
		hashmap_add(result, &entry->entry);
	}
	fclose(fp);
	නම් (finish_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

static පූර්ණ_සංඛ්‍යා ensure_leading_directories(sa_nracanaa repository *repo, var_na *path)
{
	switch (safe_create_leading_directories(repo, path)) {
		pak_sa SCLD_OK:
		pak_sa SCLD_EXISTS:
			ආපසු 0;
		default:
			ආපසු error(_("could not create leading directories "
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
static var_na *get_symlink(sa_nracanaa repository *repo,
			 sa_nracanaa difftool_options *dt_options,
			 const sa_nracanaa object_id *oid, const var_na *path)
{
	var_na *data;
	නම් (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		sa_nracanaa strbuf link = STRBUF_INIT;
		නම් (dt_options->has_symlinks) {
			නම් (strbuf_readlink(&link, path, strlen(path)))
				die(_("could not read symlink %s"), path);
		} නැතහොත් නම් (strbuf_read_file(&link, path, 128))
			die(_("could not read symlink file %s"), path);

		data = strbuf_detach(&link, NULL);
	} නැතහොත් {
		ga_nanaa object_type type;
		size_t size;
		data = odb_read_object(repo->objects, oid, &type, &size);
		නම් (!data)
			die(_("could not read object %s for symlink %s"),
				oid_to_hex(oid), path);
	}

	ආපසු data;
}

static පූර්ණ_සංඛ්‍යා checkout_path(unsigned mode, sa_nracanaa object_id *oid,
			 const var_na *path, const sa_nracanaa checkout *state)
{
	sa_nracanaa cache_entry *ce;
	පූර්ණ_සංඛ්‍යා ret;

	ce = make_transient_cache_entry(mode, oid, path, 0, NULL);
	ret = checkout_entry(ce, state, NULL, NULL);

	discard_cache_entry(ce);
	ආපසු ret;
}

static void write_file_in_directory(sa_nracanaa repository *repo,
				    sa_nracanaa strbuf *dir, size_t dir_len,
				    const var_na *path, const var_na *content)
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
static void write_standin_files(sa_nracanaa repository *repo,
				sa_nracanaa pair_entry *entry,
				sa_nracanaa strbuf *ldir, size_t ldir_len,
				sa_nracanaa strbuf *rdir, size_t rdir_len)
{
	නම් (*entry->left)
		write_file_in_directory(repo, ldir, ldir_len, entry->path, entry->left);
	නම් (*entry->right)
		write_file_in_directory(repo, rdir, rdir_len, entry->path, entry->right);
}

static පූර්ණ_සංඛ්‍යා run_dir_diff(sa_nracanaa repository *repo,
			sa_nracanaa difftool_options *dt_options,
			const var_na *extcmd, const var_na *prefix,
			sa_nracanaa child_process *child)
{
	sa_nracanaa strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	sa_nracanaa strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	sa_nracanaa strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	sa_nracanaa strbuf wtdir = STRBUF_INIT;
	sa_nracanaa strbuf tmpdir = STRBUF_INIT;
	var_na *lbase_dir = NULL, *rbase_dir = NULL;
	size_t ldir_len, rdir_len, wtdir_len;
	const var_na *workdir, *tmp;
	පූර්ණ_සංඛ්‍යා ret = 0;
	size_t i;
	FILE *fp = NULL;
	sa_nracanaa hashmap working_tree_dups = HASHMAP_INIT(working_tree_entry_cmp,
							NULL);
	sa_nracanaa hashmap submodules = HASHMAP_INIT(pair_cmp, NULL);
	sa_nracanaa hashmap symlinks2 = HASHMAP_INIT(pair_cmp, NULL);
	sa_nracanaa hashmap_iter iter;
	sa_nracanaa pair_entry *entry;
	sa_nracanaa index_state wtindex = INDEX_STATE_INIT(repo);
	sa_nracanaa checkout lstate, rstate;
	පූර්ණ_සංඛ්‍යා err = 0;
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	sa_nracanaa hashmap wt_modified = HASHMAP_INIT(path_entry_cmp, NULL);
	sa_nracanaa hashmap tmp_modified = HASHMAP_INIT(path_entry_cmp, NULL);
	පූර්ණ_සංඛ්‍යා indices_loaded = 0;

	workdir = repo_get_work_tree(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_add_absolute_path(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	නම් (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		goto finish;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	නම් (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, parimaa_na(lstate));
	lstate.base_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.base_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, parimaa_na(rstate));
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
	නම් (start_command(child))
		die("could not obtain raw diff");
	fp = xfdopen(child->out, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	අතරතුර (!strbuf_getline_nul(&info, fp)) {
		පූර්ණ_සංඛ්‍යා lmode, rmode;
		sa_nracanaa object_id loid, roid;
		var_na status;
		const var_na *src_path, *dst_path;

		නම් (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		නම් (parse_index_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &status))
			bha_nga;
		නම් (strbuf_getline_nul(&lpath, fp))
			bha_nga;
		src_path = lpath.buf;

		i++;
		නම් (status != 'C' && status != 'R') {
			dst_path = src_path;
		} නැතහොත් {
			නම් (strbuf_getline_nul(&rpath, fp))
				bha_nga;
			dst_path = rpath.buf;
		}

		නම් (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&loid));
			add_left_or_right(&submodules, src_path, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&roid));
			නම් (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			add_left_or_right(&submodules, dst_path, buf.buf, 1);
			continue;
		}

		නම් (S_ISLNK(lmode)) {
			var_na *content = get_symlink(repo, dt_options, &loid, src_path);
			add_left_or_right(&symlinks2, src_path, content, 0);
			free(content);
		}

		නම් (S_ISLNK(rmode)) {
			var_na *content = get_symlink(repo, dt_options, &roid, dst_path);
			add_left_or_right(&symlinks2, dst_path, content, 1);
			free(content);
		}

		නම් (lmode && status != 'C') {
			නම් (checkout_path(lmode, &loid, src_path, &lstate)) {
				ret = error("could not write '%s'", src_path);
				goto finish;
			}
		}

		නම් (rmode && !S_ISLNK(rmode)) {
			sa_nracanaa working_tree_entry *entry;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(entry, path, dst_path);
			hashmap_entry_init(&entry->entry, strhash(dst_path));
			නම් (hashmap_get(&working_tree_dups, &entry->entry,
					NULL)) {
				free(entry);
				continue;
			}
			hashmap_add(&working_tree_dups, &entry->entry);

			නම් (!use_wt_file(repo, workdir, dst_path, &roid)) {
				නම් (checkout_path(rmode, &roid, dst_path,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_path);
					goto finish;
				}
			} නැතහොත් නම් (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				sa_nracanaa cache_entry *ce2 =
					make_cache_entry(&wtindex, rmode, &roid,
							 dst_path, 0, 0);

				add_index_entry(&wtindex, ce2,
						ADD_CACHE_JUST_APPEND);

				add_path(&rdir, rdir_len, dst_path);
				නම් (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_path);
					goto finish;
				}
				add_path(&wtdir, wtdir_len, dst_path);
				නම් (dt_options->symlinks) {
					නම් (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						goto finish;
					}
				} නැතහොත් {
					sa_nracanaa stat st;
					නම් (stat(wtdir.buf, &st))
						st.st_mode = 0644;
					නම් (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_mode)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						goto finish;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = NULL;
	නම් (finish_command(child)) {
		ret = error("error occurred running diff --raw");
		goto finish;
	}

	නම් (!i)
		goto finish;

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

	නම් (extcmd) {
		strvec_push(&cmd.args, extcmd);
	} නැතහොත් {
		strvec_push(&cmd.args, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.args, ldir.buf, rdir.buf, NULL);
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
	සඳහා (i = 0; i < wtindex.cache_nr; i++) {
		sa_nracanaa hashmap_entry dummy;
		const var_na *name = wtindex.cache[i]->name;
		sa_nracanaa stat st;

		add_path(&rdir, rdir_len, name);
		නම් (lstat(rdir.buf, &st))
			continue;

		නම් ((dt_options->symlinks && S_ISLNK(st.st_mode)) || !S_ISREG(st.st_mode))
			continue;

		නම් (!indices_loaded) {
			sa_nracanaa lock_file lock = LOCK_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			නම් (repo_hold_lock_file_for_update(repo, &lock, buf.buf, 0) < 0 ||
			    write_locked_index(&wtindex, &lock, COMMIT_LOCK)) {
				ret = error("could not write %s", buf.buf);
				goto finish;
			}
			changed_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			changed_files(repo, &tmp_modified, buf.buf, rdir.buf);
			add_path(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_entry_init(&dummy, strhash(name));
		නම් (hashmap_get(&tmp_modified, &dummy, name)) {
			add_path(&wtdir, wtdir_len, name);
			නම් (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} නැතහොත් නම් (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_mode))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	නම් (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} නැතහොත් {
		remove_dir_recursively(&tmpdir, 0);
		නම් (ret)
			warning(_("failed: %d"), ret);
	}

finish:
	නම් (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_tree_dups, sa_nracanaa working_tree_entry, entry);
	hashmap_clear_and_free(&wt_modified, sa_nracanaa path_entry, entry);
	hashmap_clear_and_free(&tmp_modified, sa_nracanaa path_entry, entry);
	hashmap_clear_and_free(&submodules, sa_nracanaa pair_entry, entry);
	hashmap_clear_and_free(&symlinks2, sa_nracanaa pair_entry, entry);
	release_index(&wtindex);
	free(lbase_dir);
	free(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	ආපසු (ret < 0) ? 1 : ret;
}

static පූර්ණ_සංඛ්‍යා run_file_diff(පූර්ණ_සංඛ්‍යා prompt, const var_na *prefix,
			 sa_nracanaa child_process *child)
{
	strvec_push(&child->env, "GIT_PAGER=");
	strvec_push(&child->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	නම් (prompt > 0)
		strvec_push(&child->env, "GIT_DIFFTOOL_PROMPT=true");
	නැතහොත් නම් (!prompt)
		strvec_push(&child->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	child->git_cmd = 1;
	child->dir = prefix;

	ආපසු run_command(child);
}

පූර්ණ_සංඛ්‍යා cmd_difftool(පූර්ණ_සංඛ්‍යා argc,
		 const var_na **argv,
		 const var_na *prefix,
		 sa_nracanaa repository *repo)
{
	පූර්ණ_සංඛ්‍යා use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_index = 0;
	static var_na *difftool_cmd = NULL, *extcmd = NULL;
	sa_nracanaa difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_code = 0
	};
	sa_nracanaa option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_SET_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "prompt", &prompt, NULL,
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
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, prefix, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	නම් (tool_help)
		ආපසු print_tool_help();

	නම් (!no_index && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	නම් (!no_index){
		setup_work_tree(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_path(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_TREE_ENVIRONMENT, absolute_path(repo_get_work_tree(repo)), 1);
	} නැතහොත් නම් (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	නම් (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	නැතහොත් නම් (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	නම් (difftool_cmd) {
		නම් (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		නැතහොත්
			die(_("no <tool> given for --tool=<tool>"));
	}

	නම් (extcmd) {
		නම් (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		නැතහොත්
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
	strvec_push(&child.args, "diff");
	නම් (no_index)
		strvec_push(&child.args, "--no-index");
	නම් (dir_diff)
		strvec_pushl(&child.args, "--raw", "--no-abbrev", "-z", NULL);
	strvec_pushv(&child.args, argv);

	නම් (dir_diff)
		ආපසු run_dir_diff(repo, &dt_options, extcmd, prefix, &child);
	ආපසු run_file_diff(prompt, prefix, &child);
}
