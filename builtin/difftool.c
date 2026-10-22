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

#ଅନ୍ତର୍ଭାଵ "builtin.h"

#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "copy.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "repository.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "entry.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_difftool_usage[] = {
	N_("git difftool [<options>] [<commit> [<commit>]] [--] [<path>...]"),
	ଖାଲି
};

ସଂରଚନା difftool_options {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_symlinks;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା symlinks;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା trust_exit_code;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା difftool_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			   ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା difftool_options *dt_options = (ସଂରଚନା difftool_options *)cb;
	ଯଦି (!strcmp(var, "difftool.trustexitcode")) {
		dt_options->trust_exit_code = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "core.symlinks")) {
		dt_options->has_symlinks = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_tool_help(ଶୂନ୍ୟ)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

	cmd.git_cmd = 1;
	strvec_pushl(&cmd.args, "mergetool", "--tool-help=diff", ଖାଲି);
	ଫେରନ୍ତୁ run_command(&cmd);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_index_info(ସଂରଚନା repository *repo,
			    ଅକ୍ଷର *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mode1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mode2,
			    ସଂରଚନା object_id *oid1, ସଂରଚନା object_id *oid2,
			    ଅକ୍ଷର *status)
{
	ଯଦି (*p != ':')
		ଫେରନ୍ତୁ error("expected ':', got '%c'", *p);
	*mode1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strtol(p + 1, &p, 8);
	ଯଦି (*p != ' ')
		ଫେରନ୍ତୁ error("expected ' ', got '%c'", *p);
	*mode2 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strtol(p + 1, &p, 8);
	ଯଦି (*p != ' ')
		ଫେରନ୍ତୁ error("expected ' ', got '%c'", *p);
	ଯଦି (parse_oid_hex_algop(++p, oid1, (ଅଚଳ ଅକ୍ଷର **)&p, repo->hash_algo))
		ଫେରନ୍ତୁ error("expected object ID, got '%s'", p);
	ଯଦି (*p != ' ')
		ଫେରନ୍ତୁ error("expected ' ', got '%c'", *p);
	ଯଦି (parse_oid_hex_algop(++p, oid2, (ଅଚଳ ଅକ୍ଷର **)&p, repo->hash_algo))
		ଫେରନ୍ତୁ error("expected object ID, got '%s'", p);
	ଯଦି (*p != ' ')
		ଫେରନ୍ତୁ error("expected ' ', got '%c'", *p);
	*status = *++p;
	ଯଦି (!*status)
		ଫେରନ୍ତୁ error("missing status");
	ଯଦି (p[1] && !isdigit(p[1]))
		ଫେରନ୍ତୁ error("unexpected trailer: '%s'", p + 1);
	ଫେରନ୍ତୁ 0;
}

/*
 * Remove any trailing slash from $workdir
 * before starting to avoid double slashes in symlink targets.
 */
ସ୍ଥିର ଶୂନ୍ୟ add_path(ସଂରଚନା strbuf *buf, size_t base_len, ଅଚଳ ଅକ୍ଷର *path)
{
	strbuf_setlen(buf, base_len);
	ଯଦି (buf->len && buf->buf[buf->len - 1] != '/')
		strbuf_addch(buf, '/');
	strbuf_addstr(buf, path);
}

/*
 * Determine whether we can simply reuse the file in the worktree.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_wt_file(ସଂରଚନା repository *repo,
		       ଅଚଳ ଅକ୍ଷର *workdir, ଅଚଳ ଅକ୍ଷର *name,
		       ସଂରଚନା object_id *oid)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା stat st;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use = 0;

	strbuf_addstr(&buf, workdir);
	add_path(&buf, buf.len, name);

	ଯଦି (!lstat(buf.buf, &st) && !S_ISLNK(st.st_mode)) {
		ସଂରଚନା object_id wt_oid;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open(buf.buf, O_RDONLY);

		ଯଦି (fd >= 0 &&
		    !index_fd(repo->index, &wt_oid, fd, &st, OBJ_BLOB, name, 0)) {
			ଯଦି (is_null_oid(oid)) {
				oidcpy(oid, &wt_oid);
				use = 1;
			} ଅନ୍ୟଥା ଯଦି (oideq(oid, &wt_oid))
				use = 1;
		}
	}

	strbuf_release(&buf);

	ଫେରନ୍ତୁ use;
}

ସଂରଚନା working_ବୃକ୍ଷ_entry {
	ସଂରଚନା hashmap_entry entry;
	ଅକ୍ଷର path[FLEX_ARRAY];
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା working_ବୃକ୍ଷ_entry_cmp(ଅଚଳ ଶୂନ୍ୟ *cmp_data UNUSED,
				  ଅଚଳ ସଂରଚନା hashmap_entry *eptr,
				  ଅଚଳ ସଂରଚନା hashmap_entry *entry_or_ଚାବି,
				  ଅଚଳ ଶୂନ୍ୟ *keydata UNUSED)
{
	ଅଚଳ ସଂରଚନା working_ବୃକ୍ଷ_entry *a, *b;

	a = container_of(eptr, ଅଚଳ ସଂରଚନା working_ବୃକ୍ଷ_entry, entry);
	b = container_of(entry_or_ଚାବି, ଅଚଳ ସଂରଚନା working_ବୃକ୍ଷ_entry, entry);

	ଫେରନ୍ତୁ strcmp(a->path, b->path);
}

/*
 * The `left` and `right` entries hold paths for the symlinks hashmap,
 * and a SHA-1 surrounded by brief text for submodules.
 */
ସଂରଚନା pair_entry {
	ସଂରଚନା hashmap_entry entry;
	ଅକ୍ଷର ବାମ[PATH_MAX], ଡାହାଣ[PATH_MAX];
	ଅଚଳ ଅକ୍ଷର path[FLEX_ARRAY];
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pair_cmp(ଅଚଳ ଶୂନ୍ୟ *cmp_data UNUSED,
		    ଅଚଳ ସଂରଚନା hashmap_entry *eptr,
		    ଅଚଳ ସଂରଚନା hashmap_entry *entry_or_ଚାବି,
		    ଅଚଳ ଶୂନ୍ୟ *keydata UNUSED)
{
	ଅଚଳ ସଂରଚନା pair_entry *a, *b;

	a = container_of(eptr, ଅଚଳ ସଂରଚନା pair_entry, entry);
	b = container_of(entry_or_ଚାବି, ଅଚଳ ସଂରଚନା pair_entry, entry);

	ଫେରନ୍ତୁ strcmp(a->path, b->path);
}

ସ୍ଥିର ଶୂନ୍ୟ add_ବାମ_or_ଡାହାଣ(ସଂରଚନା hashmap *map, ଅଚଳ ଅକ୍ଷର *path,
			      ଅଚଳ ଅକ୍ଷର *content, ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_ଡାହାଣ)
{
	ସଂରଚନା pair_entry *e, *existing;

	FLEX_ALLOC_STR(e, path, path);
	hashmap_entry_init(&e->entry, strhash(path));
	existing = hashmap_get_entry(map, e, entry, ଖାଲି);
	ଯଦି (existing) {
		ମୁକ୍ତ(e);
		e = existing;
	} ଅନ୍ୟଥା {
		e->ବାମ[0] = e->ଡାହାଣ[0] = '\0';
		hashmap_add(map, &e->entry);
	}
	strlcpy(is_ଡାହାଣ ? e->ଡାହାଣ : e->ବାମ, content, PATH_MAX);
}

ସଂରଚନା path_entry {
	ସଂରଚନା hashmap_entry entry;
	ଅକ୍ଷର path[FLEX_ARRAY];
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା path_entry_cmp(ଅଚଳ ଶୂନ୍ୟ *cmp_data UNUSED,
			  ଅଚଳ ସଂରଚନା hashmap_entry *eptr,
			  ଅଚଳ ସଂରଚନା hashmap_entry *entry_or_ଚାବି,
			  ଅଚଳ ଶୂନ୍ୟ *ଚାବି)
{
	ଅଚଳ ସଂରଚନା path_entry *a, *b;

	a = container_of(eptr, ଅଚଳ ସଂରଚନା path_entry, entry);
	b = container_of(entry_or_ଚାବି, ଅଚଳ ସଂରଚନା path_entry, entry);

	ଫେରନ୍ତୁ strcmp(a->path, ଚାବି ? ଚାବି : b->path);
}

ସ୍ଥିର ଶୂନ୍ୟ changed_files(ସଂରଚନା repository *repo,
			  ସଂରଚନା hashmap *result, ଅଚଳ ଅକ୍ଷର *index_path,
			  ଅଚଳ ଅକ୍ଷର *workdir)
{
	ସଂରଚନା child_process update_index = CHILD_PROCESS_INIT;
	ସଂରଚନା child_process diff_files = CHILD_PROCESS_INIT;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *git_dir = absolute_path(repo_get_git_dir(repo));
	FILE *fp;

	strvec_pushl(&update_index.args,
		     "--git-dir", git_dir, "--work-tree", workdir,
		     "update-index", "--really-refresh", "-q",
		     "--unmerged", ଖାଲି);
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
		     "diff-files", "--name-only", "-z", ଖାଲି);
	diff_files.no_stdin = 1;
	diff_files.git_cmd = 1;
	diff_files.use_shell = 0;
	diff_files.clean_on_exit = 1;
	diff_files.out = -1;
	diff_files.dir = workdir;
	strvec_pushf(&diff_files.env, "GIT_INDEX_FILE=%s", index_path);
	ଯଦି (start_command(&diff_files))
		die("could not obtain raw diff");
	fp = xfdopen(diff_files.out, "r");
	ଯେତେବେଳେ (!strbuf_getline_nul(&buf, fp)) {
		ସଂରଚନା path_entry *entry;
		FLEX_ALLOC_STR(entry, path, buf.buf);
		hashmap_entry_init(&entry->entry, strhash(buf.buf));
		hashmap_add(result, &entry->entry);
	}
	fclose(fp);
	ଯଦି (finish_command(&diff_files))
		die("diff-files did not exit properly");
	strbuf_release(&buf);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ensure_leading_directories(ସଂରଚନା repository *repo, ଅକ୍ଷର *path)
{
	ବାଛ (safe_create_leading_directories(repo, path)) {
		ସ୍ଥିତି SCLD_OK:
		ସ୍ଥିତି SCLD_EXISTS:
			ଫେରନ୍ତୁ 0;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			ଫେରନ୍ତୁ error(_("could not create leading directories "
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
ସ୍ଥିର ଅକ୍ଷର *get_symlink(ସଂରଚନା repository *repo,
			 ସଂରଚନା difftool_options *dt_options,
			 ଅଚଳ ସଂରଚନା object_id *oid, ଅଚଳ ଅକ୍ଷର *path)
{
	ଅକ୍ଷର *data;
	ଯଦି (is_null_oid(oid)) {
		/* The symlink is unknown to Git so read from the filesystem */
		ସଂରଚନା strbuf link = STRBUF_INIT;
		ଯଦି (dt_options->has_symlinks) {
			ଯଦି (strbuf_readlink(&link, path, strlen(path)))
				die(_("could not read symlink %s"), path);
		} ଅନ୍ୟଥା ଯଦି (strbuf_read_file(&link, path, 128))
			die(_("could not read symlink file %s"), path);

		data = strbuf_detach(&link, ଖାଲି);
	} ଅନ୍ୟଥା {
		ଗଣନା object_type type;
		size_t size;
		data = odb_read_object(repo->objects, oid, &type, &size);
		ଯଦି (!data)
			die(_("could not read object %s for symlink %s"),
				oid_to_hex(oid), path);
	}

	ଫେରନ୍ତୁ data;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_path(unsigned mode, ସଂରଚନା object_id *oid,
			 ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ସଂରଚନା checkout *state)
{
	ସଂରଚନା cache_entry *ce;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ce = make_transient_cache_entry(mode, oid, path, 0, ଖାଲି);
	ret = checkout_entry(ce, state, ଖାଲି, ଖାଲି);

	discard_cache_entry(ce);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ write_file_in_directory(ସଂରଚନା repository *repo,
				    ସଂରଚନା strbuf *dir, size_t dir_len,
				    ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *content)
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
ସ୍ଥିର ଶୂନ୍ୟ write_standin_files(ସଂରଚନା repository *repo,
				ସଂରଚନା pair_entry *entry,
				ସଂରଚନା strbuf *ldir, size_t ldir_len,
				ସଂରଚନା strbuf *rdir, size_t rdir_len)
{
	ଯଦି (*entry->ବାମ)
		write_file_in_directory(repo, ldir, ldir_len, entry->path, entry->ବାମ);
	ଯଦି (*entry->ଡାହାଣ)
		write_file_in_directory(repo, rdir, rdir_len, entry->path, entry->ଡାହାଣ);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_dir_diff(ସଂରଚନା repository *repo,
			ସଂରଚନା difftool_options *dt_options,
			ଅଚଳ ଅକ୍ଷର *extcmd, ଅଚଳ ଅକ୍ଷର *prefix,
			ସଂରଚନା child_process *child)
{
	ସଂରଚନା strbuf info = STRBUF_INIT, lpath = STRBUF_INIT;
	ସଂରଚନା strbuf rpath = STRBUF_INIT, buf = STRBUF_INIT;
	ସଂରଚନା strbuf ldir = STRBUF_INIT, rdir = STRBUF_INIT;
	ସଂରଚନା strbuf wtdir = STRBUF_INIT;
	ସଂରଚନା strbuf tmpdir = STRBUF_INIT;
	ଅକ୍ଷର *lbase_dir = ଖାଲି, *rbase_dir = ଖାଲି;
	size_t ldir_len, rdir_len, wtdir_len;
	ଅଚଳ ଅକ୍ଷର *workdir, *tmp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	size_t i;
	FILE *fp = ଖାଲି;
	ସଂରଚନା hashmap working_ବୃକ୍ଷ_dups = HASHMAP_INIT(working_ବୃକ୍ଷ_entry_cmp,
							ଖାଲି);
	ସଂରଚନା hashmap submodules = HASHMAP_INIT(pair_cmp, ଖାଲି);
	ସଂରଚନା hashmap symlinks2 = HASHMAP_INIT(pair_cmp, ଖାଲି);
	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା pair_entry *entry;
	ସଂରଚନା index_state wtindex = INDEX_STATE_INIT(repo);
	ସଂରଚନା checkout lstate, rstate;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;
	ସଂରଚନା hashmap wt_modified = HASHMAP_INIT(path_entry_cmp, ଖାଲି);
	ସଂରଚନା hashmap tmp_modified = HASHMAP_INIT(path_entry_cmp, ଖାଲି);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା indices_loaded = 0;

	workdir = repo_get_work_ବୃକ୍ଷ(repo);

	/* Setup temp directories */
	tmp = getenv("TMPDIR");
	strbuf_add_absolute_path(&tmpdir, tmp ? tmp : "/tmp");
	strbuf_trim_trailing_dir_sep(&tmpdir);
	strbuf_addstr(&tmpdir, "/git-difftool.XXXXXX");
	ଯଦି (!mkdtemp(tmpdir.buf)) {
		ret = error("could not create '%s'", tmpdir.buf);
		ଯାଅ finish;
	}
	strbuf_addf(&ldir, "%s/left/", tmpdir.buf);
	strbuf_addf(&rdir, "%s/right/", tmpdir.buf);
	strbuf_addstr(&wtdir, workdir);
	ଯଦି (!wtdir.len || !is_dir_sep(wtdir.buf[wtdir.len - 1]))
		strbuf_addch(&wtdir, '/');
	mkdir(ldir.buf, 0700);
	mkdir(rdir.buf, 0700);

	memset(&lstate, 0, ଆକାର(lstate));
	lstate.base_dir = lbase_dir = xstrdup(ldir.buf);
	lstate.base_dir_len = ldir.len;
	lstate.force = 1;
	memset(&rstate, 0, ଆକାର(rstate));
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
	ଯଦି (start_command(child))
		die("could not obtain raw diff");
	fp = xfdopen(child->out, "r");

	/* Build index info for left and right sides of the diff */
	i = 0;
	ଯେତେବେଳେ (!strbuf_getline_nul(&info, fp)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା lmode, rmode;
		ସଂରଚନା object_id loid, roid;
		ଅକ୍ଷର status;
		ଅଚଳ ଅକ୍ଷର *src_path, *dst_path;

		ଯଦି (starts_with(info.buf, "::"))
			die(N_("combined diff formats ('-c' and '--cc') are "
			       "not supported in\n"
			       "directory diff mode ('-d' and '--dir-diff')."));

		ଯଦି (parse_index_info(repo, info.buf, &lmode, &rmode, &loid, &roid, &status))
			ଭାଙ୍ଗ;
		ଯଦି (strbuf_getline_nul(&lpath, fp))
			ଭାଙ୍ଗ;
		src_path = lpath.buf;

		i++;
		ଯଦି (status != 'C' && status != 'R') {
			dst_path = src_path;
		} ଅନ୍ୟଥା {
			ଯଦି (strbuf_getline_nul(&rpath, fp))
				ଭାଙ୍ଗ;
			dst_path = rpath.buf;
		}

		ଯଦି (S_ISGITLINK(lmode) || S_ISGITLINK(rmode)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&loid));
			add_ବାମ_or_ଡାହାଣ(&submodules, src_path, buf.buf, 0);
			strbuf_reset(&buf);
			strbuf_addf(&buf, "Subproject commit %s",
				    oid_to_hex(&roid));
			ଯଦି (oideq(&loid, &roid))
				strbuf_addstr(&buf, "-dirty");
			add_ବାମ_or_ଡାହାଣ(&submodules, dst_path, buf.buf, 1);
			ଜାରି;
		}

		ଯଦି (S_ISLNK(lmode)) {
			ଅକ୍ଷର *content = get_symlink(repo, dt_options, &loid, src_path);
			add_ବାମ_or_ଡାହାଣ(&symlinks2, src_path, content, 0);
			ମୁକ୍ତ(content);
		}

		ଯଦି (S_ISLNK(rmode)) {
			ଅକ୍ଷର *content = get_symlink(repo, dt_options, &roid, dst_path);
			add_ବାମ_or_ଡାହାଣ(&symlinks2, dst_path, content, 1);
			ମୁକ୍ତ(content);
		}

		ଯଦି (lmode && status != 'C') {
			ଯଦି (checkout_path(lmode, &loid, src_path, &lstate)) {
				ret = error("could not write '%s'", src_path);
				ଯାଅ finish;
			}
		}

		ଯଦି (rmode && !S_ISLNK(rmode)) {
			ସଂରଚନା working_ବୃକ୍ଷ_entry *entry;

			/* Avoid duplicate working_tree entries */
			FLEX_ALLOC_STR(entry, path, dst_path);
			hashmap_entry_init(&entry->entry, strhash(dst_path));
			ଯଦି (hashmap_get(&working_ବୃକ୍ଷ_dups, &entry->entry,
					ଖାଲି)) {
				ମୁକ୍ତ(entry);
				ଜାରି;
			}
			hashmap_add(&working_ବୃକ୍ଷ_dups, &entry->entry);

			ଯଦି (!use_wt_file(repo, workdir, dst_path, &roid)) {
				ଯଦି (checkout_path(rmode, &roid, dst_path,
						  &rstate)) {
					ret = error("could not write '%s'",
						    dst_path);
					ଯାଅ finish;
				}
			} ଅନ୍ୟଥା ଯଦି (!is_null_oid(&roid)) {
				/*
				 * Changes in the working tree need special
				 * treatment since they are not part of the
				 * index.
				 */
				ସଂରଚନା cache_entry *ce2 =
					make_cache_entry(&wtindex, rmode, &roid,
							 dst_path, 0, 0);

				add_index_entry(&wtindex, ce2,
						ADD_CACHE_JUST_APPEND);

				add_path(&rdir, rdir_len, dst_path);
				ଯଦି (ensure_leading_directories(repo, rdir.buf)) {
					ret = error("could not create "
						    "directory for '%s'",
						    dst_path);
					ଯାଅ finish;
				}
				add_path(&wtdir, wtdir_len, dst_path);
				ଯଦି (dt_options->symlinks) {
					ଯଦି (symlink(wtdir.buf, rdir.buf)) {
						ret = error_errno("could not symlink '%s' to '%s'", wtdir.buf, rdir.buf);
						ଯାଅ finish;
					}
				} ଅନ୍ୟଥା {
					ସଂରଚନା stat st;
					ଯଦି (stat(wtdir.buf, &st))
						st.st_mode = 0644;
					ଯଦି (copy_file(repo, rdir.buf, wtdir.buf,
						      st.st_mode)) {
						ret = error("could not copy '%s' to '%s'", wtdir.buf, rdir.buf);
						ଯାଅ finish;
					}
				}
			}
		}
	}

	fclose(fp);
	fp = ଖାଲି;
	ଯଦି (finish_command(child)) {
		ret = error("error occurred running diff --raw");
		ଯାଅ finish;
	}

	ଯଦି (!i)
		ଯାଅ finish;

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

	ଯଦି (extcmd) {
		strvec_ଠେଲ(&cmd.args, extcmd);
	} ଅନ୍ୟଥା {
		strvec_ଠେଲ(&cmd.args, "difftool--helper");
		cmd.git_cmd = 1;
		setenv("GIT_DIFFTOOL_DIRDIFF", "true", 1);
	}
	strvec_pushl(&cmd.args, ldir.buf, rdir.buf, ଖାଲି);
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
	ପାଇଁ (i = 0; i < wtindex.cache_nr; i++) {
		ସଂରଚନା hashmap_entry dummy;
		ଅଚଳ ଅକ୍ଷର *name = wtindex.cache[i]->name;
		ସଂରଚନା stat st;

		add_path(&rdir, rdir_len, name);
		ଯଦି (lstat(rdir.buf, &st))
			ଜାରି;

		ଯଦି ((dt_options->symlinks && S_ISLNK(st.st_mode)) || !S_ISREG(st.st_mode))
			ଜାରି;

		ଯଦି (!indices_loaded) {
			ସଂରଚନା lock_file lock = LOCK_INIT;
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/wtindex", tmpdir.buf);
			ଯଦି (repo_hold_lock_file_for_update(repo, &lock, buf.buf, 0) < 0 ||
			    write_locked_index(&wtindex, &lock, COMMIT_LOCK)) {
				ret = error("could not write %s", buf.buf);
				ଯାଅ finish;
			}
			changed_files(repo, &wt_modified, buf.buf, workdir);
			strbuf_setlen(&rdir, rdir_len);
			changed_files(repo, &tmp_modified, buf.buf, rdir.buf);
			add_path(&rdir, rdir_len, name);
			indices_loaded = 1;
		}

		hashmap_entry_init(&dummy, strhash(name));
		ଯଦି (hashmap_get(&tmp_modified, &dummy, name)) {
			add_path(&wtdir, wtdir_len, name);
			ଯଦି (hashmap_get(&wt_modified, &dummy, name)) {
				warning(_("both files modified: '%s' and '%s'."),
					wtdir.buf, rdir.buf);
				warning(_("working tree file has been left."));
				warning("%s", "");
				err = 1;
			} ଅନ୍ୟଥା ଯଦି (unlink(wtdir.buf) ||
				   copy_file(repo, wtdir.buf, rdir.buf, st.st_mode))
				warning_errno(_("could not copy '%s' to '%s'"),
					      rdir.buf, wtdir.buf);
		}
	}

	ଯଦି (err) {
		warning(_("temporary files exist in '%s'."), tmpdir.buf);
		warning(_("you may want to cleanup or recover these."));
		ret = 1;
	} ଅନ୍ୟଥା {
		remove_dir_recursively(&tmpdir, 0);
		ଯଦି (ret)
			warning(_("failed: %d"), ret);
	}

finish:
	ଯଦି (fp)
		fclose(fp);

	hashmap_clear_and_free(&working_ବୃକ୍ଷ_dups, ସଂରଚନା working_ବୃକ୍ଷ_entry, entry);
	hashmap_clear_and_free(&wt_modified, ସଂରଚନା path_entry, entry);
	hashmap_clear_and_free(&tmp_modified, ସଂରଚନା path_entry, entry);
	hashmap_clear_and_free(&submodules, ସଂରଚନା pair_entry, entry);
	hashmap_clear_and_free(&symlinks2, ସଂରଚନା pair_entry, entry);
	release_index(&wtindex);
	ମୁକ୍ତ(lbase_dir);
	ମୁକ୍ତ(rbase_dir);
	strbuf_release(&info);
	strbuf_release(&lpath);
	strbuf_release(&rpath);
	strbuf_release(&ldir);
	strbuf_release(&rdir);
	strbuf_release(&wtdir);
	strbuf_release(&buf);
	strbuf_release(&tmpdir);

	ଫେରନ୍ତୁ (ret < 0) ? 1 : ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_file_diff(ପୂର୍ଣ୍ଣସଂଖ୍ୟା prompt, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା child_process *child)
{
	strvec_ଠେଲ(&child->env, "GIT_PAGER=");
	strvec_ଠେଲ(&child->env, "GIT_EXTERNAL_DIFF=git-difftool--helper");
	ଯଦି (prompt > 0)
		strvec_ଠେଲ(&child->env, "GIT_DIFFTOOL_PROMPT=true");
	ଅନ୍ୟଥା ଯଦି (!prompt)
		strvec_ଠେଲ(&child->env, "GIT_DIFFTOOL_NO_PROMPT=true");

	child->git_cmd = 1;
	child->dir = prefix;

	ଫେରନ୍ତୁ run_command(child);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_difftool(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		 ଅଚଳ ଅକ୍ଷର **argv,
		 ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_gui_tool = -1, dir_diff = 0, prompt = -1, tool_help = 0, no_index = 0;
	ସ୍ଥିର ଅକ୍ଷର *difftool_cmd = ଖାଲି, *extcmd = ଖାଲି;
	ସଂରଚନା difftool_options dt_options = {
		.has_symlinks = 1,
		.symlinks = 1,
		.trust_exit_code = 0
	};
	ସଂରଚନା option builtin_difftool_options[] = {
		OPT_BOOL('g', "gui", &use_gui_tool,
			 N_("use `diff.guitool` instead of `diff.tool`")),
		OPT_BOOL('d', "dir-diff", &dir_diff,
			 N_("perform a full-directory diff")),
		OPT_SET_INT_F('y', "no-prompt", &prompt,
			N_("do not prompt before launching a diff tool"),
			0, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "prompt", &prompt, ଖାଲି,
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
	ସଂରଚନା child_process child = CHILD_PROCESS_INIT;

	repo_config(repo, difftool_config, &dt_options);
	dt_options.symlinks = dt_options.has_symlinks;

	argc = parse_options(argc, argv, prefix, builtin_difftool_options,
			     builtin_difftool_usage, PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	ଯଦି (tool_help)
		ଫେରନ୍ତୁ print_tool_help();

	ଯଦି (!no_index && !startup_info->have_repository)
		die(_("difftool requires worktree or --no-index"));

	ଯଦି (!no_index){
		setup_work_ବୃକ୍ଷ(repo);
		setenv(GIT_DIR_ENVIRONMENT, absolute_path(repo_get_git_dir(repo)), 1);
		setenv(GIT_WORK_ବୃକ୍ଷ_ENVIRONMENT, absolute_path(repo_get_work_ବୃକ୍ଷ(repo)), 1);
	} ଅନ୍ୟଥା ଯଦି (dir_diff)
		die(_("options '%s' and '%s' cannot be used together"), "--dir-diff", "--no-index");

	die_for_incompatible_opt3(use_gui_tool == 1, "--gui",
				  !!difftool_cmd, "--tool",
				  !!extcmd, "--extcmd");

	/*
	 * Explicitly specified GUI option is forwarded to git-mergetool--lib.sh;
	 * empty or unset means "use the difftool.guiDefault config or default to
	 * false".
	 */
	ଯଦି (use_gui_tool == 1)
		setenv("GIT_MERGETOOL_GUI", "true", 1);
	ଅନ୍ୟଥା ଯଦି (use_gui_tool == 0)
		setenv("GIT_MERGETOOL_GUI", "false", 1);

	ଯଦି (difftool_cmd) {
		ଯଦି (*difftool_cmd)
			setenv("GIT_DIFF_TOOL", difftool_cmd, 1);
		ଅନ୍ୟଥା
			die(_("no <tool> given for --tool=<tool>"));
	}

	ଯଦି (extcmd) {
		ଯଦି (*extcmd)
			setenv("GIT_DIFFTOOL_EXTCMD", extcmd, 1);
		ଅନ୍ୟଥା
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
	strvec_ଠେଲ(&child.args, "diff");
	ଯଦି (no_index)
		strvec_ଠେଲ(&child.args, "--no-index");
	ଯଦି (dir_diff)
		strvec_pushl(&child.args, "--raw", "--no-abbrev", "-z", ଖାଲି);
	strvec_pushv(&child.args, argv);

	ଯଦି (dir_diff)
		ଫେରନ୍ତୁ run_dir_diff(repo, &dt_options, extcmd, prefix, &child);
	ଫେରନ୍ତୁ run_file_diff(prompt, prefix, &child);
}
