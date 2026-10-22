/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "convert.h"
#include "environment.h"
#include "quote.h"
#include "dir.h"
#include "gettext.h"
#include "object-name.h"
#include "strbuf.h"
#include "parse-options.h"
#include "resolve-undo.h"
#include "string-list.h"
#include "path.h"
#include "pathspec.h"
#include "read-cache.h"
#include "setup.h"
#include "sparse-index.h"
#include "submodule.h"
#include "odb.h"
#include "hex.h"


static ཧྲིལ abbrev;
static ཧྲིལ show_deleted;
static ཧྲིལ show_cached;
static ཧྲིལ show_others;
static ཧྲིལ show_stage;
static ཧྲིལ show_unmerged;
static ཧྲིལ show_resolve_undo;
static ཧྲིལ show_modified;
static ཧྲིལ show_killed;
static ཧྲིལ show_valid_bit;
static ཧྲིལ show_fsmonitor_bit;
static ཧྲིལ line_terminator = '\n';
static ཧྲིལ debug_mode;
static ཧྲིལ show_eol;
static ཧྲིལ recurse_submodules;
static ཧྲིལ skipping_duplicates;
static ཧྲིལ show_sparse_dirs;

static const var_na *prefix;
static ཧྲིལ max_prefix_len;
static ཧྲིལ prefix_len;
static sa_nracanaa pathspec pathspec;
static ཧྲིལ error_unmatch;
static var_na *ps_matched;
static const var_na *with_tree;
static ཧྲིལ exc_given;
static ཧྲིལ exclude_args;
static const var_na *format;

static const var_na *tag_cached = "";
static const var_na *tag_unmerged = "";
static const var_na *tag_removed = "";
static const var_na *tag_other = "";
static const var_na *tag_killed = "";
static const var_na *tag_modified = "";
static const var_na *tag_skip_worktree = "";
static const var_na *tag_resolve_undo = "";

static void write_eolinfo(sa_nracanaa index_state *istate,
			  const sa_nracanaa cache_entry *ce, const var_na *path)
{
	གལ་སྲིད (show_eol) {
		sa_nracanaa stat st;
		const var_na *i_txt = "";
		const var_na *w_txt = "";
		const var_na *a_txt = get_convert_attr_ascii(istate, path);
		གལ་སྲིད (ce && S_ISREG(ce->ce_mode))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		གལ་སྲིད (!lstat(path, &st) && S_ISREG(st.st_mode))
			w_txt = get_wt_convert_stats_ascii(path);
		printf("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

static void write_name(const var_na *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, prefix_len ? prefix : NULL,
				   stdout, line_terminator);
}

static void write_name_to_buf(sa_nracanaa strbuf *sb, const var_na *name)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	const var_na *rel = relative_path(name, prefix_len ? prefix : NULL, &buf);

	གལ་སྲིད (line_terminator)
		quote_c_style(rel, sb, NULL, 0);
	དེ་མིན
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

static const var_na *get_tag(const sa_nracanaa cache_entry *ce, const var_na *tag)
{
	static var_na alttag[4];

	གལ་སྲིད (tag && *tag && ((show_valid_bit && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_bit && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		memcpy(alttag, tag, 3);

		གལ་སྲིད (isalpha(tag[0])) {
			alttag[0] = tolower(tag[0]);
		} དེ་མིན གལ་སྲིད (tag[0] == '?') {
			alttag[0] = '!';
		} དེ་མིན {
			alttag[0] = 'v';
			alttag[1] = tag[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		tag = alttag;
	}

	ལོག tag;
}

static void print_debug(const sa_nracanaa cache_entry *ce)
{
	གལ་སྲིད (debug_mode) {
		const sa_nracanaa stat_data *sd = &ce->ce_stat_data;

		printf("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		printf("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		printf("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		printf("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		printf("  size: %u\tflags: %x\n", sd->sd_size, ce->ce_flags);
	}
}

static void show_dir_entry(sa_nracanaa index_state *istate,
			   const var_na *tag, sa_nracanaa dir_entry *ent)
{
	ཧྲིལ len = max_prefix_len;

	གལ་སྲིད (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	གལ་སྲིད (ps_matched)
		dir_path_match(istate, ent, &pathspec, len, ps_matched);

	fputs(tag, stdout);
	write_eolinfo(istate, NULL, ent->name);
	write_name(ent->name);
}

static void show_other_files(sa_nracanaa index_state *istate,
			     const sa_nracanaa dir_struct *dir)
{
	ཧྲིལ i;

	དོན་ལུ (i = 0; i < dir->nr; i++) {
		sa_nracanaa dir_entry *ent = dir->entries[i];
		གལ་སྲིད (!index_name_is_other(istate, ent->name, ent->len))
			continue;
		show_dir_entry(istate, tag_other, ent);
	}
}

static void show_killed_files(sa_nracanaa index_state *istate,
			      const sa_nracanaa dir_struct *dir)
{
	ཧྲིལ i;
	དོན་ལུ (i = 0; i < dir->nr; i++) {
		sa_nracanaa dir_entry *ent = dir->entries[i];
		var_na *cp, *sp;
		ཧྲིལ pos, len, killed = 0;

		དོན་ལུ (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			གལ་སྲིད (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = index_name_pos(istate, ent->name, ent->len);
				གལ་སྲིད (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				སྐབས (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				གལ་སྲིད (istate->cache_nr <= pos)
					bha_nga;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				གལ་སྲིད ((ent->len < len) &&
				    !strncmp(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				bha_nga;
			}
			གལ་སྲིད (0 <= index_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				bha_nga;
			}
		}
		གལ་སྲིད (killed)
			show_dir_entry(istate, tag_killed, dir->entries[i]);
	}
}

static void show_files(sa_nracanaa repository *repo, sa_nracanaa dir_struct *dir);

static void show_submodule(sa_nracanaa repository *superproject,
			   sa_nracanaa dir_struct *dir, const var_na *path)
{
	sa_nracanaa repository subrepo;

	གལ་སྲིད (repo_submodule_init(&subrepo, superproject, path,
				null_oid(superproject->hash_algo)))
		ལོག;

	གལ་སྲིད (repo_read_index(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

static void expand_objectsize(sa_nracanaa repository *repo, sa_nracanaa strbuf *line,
			      const sa_nracanaa object_id *oid,
			      const ga_nanaa object_type type, unsigned ཧྲིལ padded)
{
	static const var_na padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = line->len;
	size_t len;

	གལ་སྲིད (type == OBJ_BLOB) {
		size_t size;
		གལ་སྲིད (odb_read_object_info(repo->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(line, size);
	} དེ་མིན {
		strbuf_addstr(line, "-");
	}
	len = line->len - orig_len;
	གལ་སྲིད (len < min_len)
		strbuf_insert(line, orig_len, padding, min_len - len);
}

static void show_ce_fmt(sa_nracanaa repository *repo, const sa_nracanaa cache_entry *ce,
			const var_na *format, const var_na *fullname) {
	sa_nracanaa strbuf sb = STRBUF_INIT;

	སྐབས (strbuf_expand_step(&sb, &format)) {
		size_t len;
		sa_nracanaa stat st;

		གལ་སྲིད (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		དེ་མིན གལ་སྲིད ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		དེ་མིན གལ་སྲིད (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_mode);
		དེ་མིན གལ་སྲིད (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, &ce->oid, abbrev);
		དེ་མིན གལ་སྲིད (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_mode)));
		དེ་མིན གལ་སྲིད (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 1);
		དེ་མིན གལ་སྲིད (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 0);
		དེ་མིན གལ་སྲིད (skip_prefix(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		དེ་མིན གལ་སྲིད (skip_prefix(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_mode) ?
				      get_cached_convert_stats_ascii(repo->index,
				      ce->name) : "");
		དེ་མིན གལ་སྲིད (skip_prefix(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_mode) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		དེ་མིན གལ་སྲིད (skip_prefix(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->index,
								 fullname));
		དེ་མིན གལ་སྲིད (skip_prefix(format, "(path)", &format))
			write_name_to_buf(&sb, fullname);
		དེ་མིན
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, line_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

static void show_ce(sa_nracanaa repository *repo, sa_nracanaa dir_struct *dir,
		    const sa_nracanaa cache_entry *ce, const var_na *fullname,
		    const var_na *tag)
{
	གལ་སྲིད (max_prefix_len > strlen(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	གལ་སྲིད (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
	    is_submodule_active(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} དེ་མིན གལ་སྲིད (match_pathspec(repo->index, &pathspec, fullname, strlen(fullname),
				  max_prefix_len, ps_matched,
				  S_ISDIR(ce->ce_mode) ||
				  S_ISGITLINK(ce->ce_mode))) {
		གལ་སྲིད (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			ལོག;
		}

		tag = get_tag(ce, tag);

		གལ་སྲིད (!show_stage) {
			fputs(tag, stdout);
		} དེ་མིན {
			printf("%s%06o %s %d\t",
			       tag,
			       ce->ce_mode,
			       repo_find_unique_abbrev(repo, &ce->oid, abbrev),
			       ce_stage(ce));
		}
		write_eolinfo(repo->index, ce, fullname);
		write_name(fullname);
		print_debug(ce);
	}
}

static void show_ru_info(sa_nracanaa repository *repo, sa_nracanaa index_state *istate)
{
	sa_nracanaa string_list_item *item;

	གལ་སྲིད (!istate->resolve_undo)
		ལོག;

	for_each_string_list_item(item, istate->resolve_undo) {
		const var_na *path = item->string;
		sa_nracanaa resolve_undo_info *ui = item->util;
		ཧྲིལ i, len;

		len = strlen(path);
		གལ་སྲིད (len < max_prefix_len)
			continue; /* outside of the prefix */
		གལ་སྲིད (!match_pathspec(istate, &pathspec, path, len,
				    max_prefix_len, ps_matched, 0))
			continue; /* uninterested */
		དོན་ལུ (i = 0; i < 3; i++) {
			གལ་སྲིད (!ui->mode[i])
				continue;
			printf("%s%06o %s %d\t", tag_resolve_undo, ui->mode[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(path);
		}
	}
}

static ཧྲིལ ce_excluded(sa_nracanaa dir_struct *dir, sa_nracanaa index_state *istate,
		       const var_na *fullname, const sa_nracanaa cache_entry *ce)
{
	ཧྲིལ dtype = ce_to_dtype(ce);
	ལོག is_excluded(dir, istate, fullname, &dtype);
}

static void construct_fullname(sa_nracanaa strbuf *out, const sa_nracanaa repository *repo,
			       const sa_nracanaa cache_entry *ce)
{
	strbuf_reset(out);
	གལ་སྲིད (repo->submodule_prefix)
		strbuf_addstr(out, repo->submodule_prefix);
	strbuf_addstr(out, ce->name);
}

static void show_files(sa_nracanaa repository *repo, sa_nracanaa dir_struct *dir)
{
	ཧྲིལ i;
	sa_nracanaa strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	གལ་སྲིད (show_others || show_killed) {
		གལ་སྲིད (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->index, &pathspec);
		གལ་སྲིད (show_others)
			show_other_files(repo->index, dir);
		གལ་སྲིད (show_killed)
			show_killed_files(repo->index, dir);
	}

	གལ་སྲིད (!(show_cached || show_stage || show_deleted || show_modified))
		ལོག;

	དོན་ལུ (i = 0; i < repo->index->cache_nr; i++) {
		const sa_nracanaa cache_entry *ce = repo->index->cache[i];
		sa_nracanaa stat st;
		ཧྲིལ stat_err;

		གལ་སྲིད (S_ISSPARSEDIR(ce->ce_mode) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_full_index(repo->index);
			ce = repo->index->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		གལ་སྲིད ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->index, fullname.buf, ce))
			continue;
		གལ་སྲིད (ce->ce_flags & CE_UPDATE)
			continue;
		གལ་སྲིད ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? tag_unmerged :
				(ce_skip_worktree(ce) ? tag_skip_worktree :
				 tag_cached));
			གལ་སྲིད (skipping_duplicates)
				goto skip_to_next_name;
		}

		གལ་སྲིད (!(show_deleted || show_modified))
			continue;
		གལ་སྲིད (ce_skip_worktree(ce))
			continue;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		གལ་སྲིད (pathspec.nr == 1 &&
		    !match_pathspec(repo->index, &pathspec, fullname.buf,
				    fullname.len, max_prefix_len, NULL,
				    S_ISDIR(ce->ce_mode) ||
				    S_ISGITLINK(ce->ce_mode)))
			continue;
		stat_err = lstat(fullname.buf, &st);
		གལ་སྲིད (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		གལ་སྲིད (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, tag_removed);
			གལ་སྲིད (skipping_duplicates)
				goto skip_to_next_name;
		}
		གལ་སྲིད (show_modified &&
		    (stat_err || ie_modified(repo->index, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, tag_modified);
			གལ་སྲིད (skipping_duplicates)
				goto skip_to_next_name;
		}
		continue;

skip_to_next_name:
		{
			ཧྲིལ j;
			sa_nracanaa cache_entry **cache = repo->index->cache;
			དོན་ལུ (j = i + 1; j < repo->index->cache_nr; j++)
				གལ་སྲིད (strcmp(ce->name, cache[j]->name))
					bha_nga;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
static void prune_index(sa_nracanaa index_state *istate,
			const var_na *prefix, size_t prefixlen)
{
	ཧྲིལ pos;
	unsigned ཧྲིལ first, last;

	གལ་སྲིད (!prefix || !istate->cache_nr)
		ལོག;
	pos = index_name_pos(istate, prefix, prefixlen);
	གལ་སྲིད (pos < 0)
		pos = -pos-1;
	first = pos;
	last = istate->cache_nr;
	སྐབས (last > first) {
		ཧྲིལ next = first + ((last - first) >> 1);
		const sa_nracanaa cache_entry *ce = istate->cache[next];
		གལ་སྲིད (!strncmp(ce->name, prefix, prefixlen)) {
			first = next+1;
			continue;
		}
		last = next;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, last - pos);
	istate->cache_nr = last - pos;
}

static ཧྲིལ get_common_prefix_len(const var_na *common_prefix)
{
	ཧྲིལ common_prefix_len;

	གལ་སྲིད (!common_prefix)
		ལོག 0;

	common_prefix_len = strlen(common_prefix);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	གལ་སྲིད (common_prefix[common_prefix_len - 1] == '/')
		common_prefix_len--;

	ལོག common_prefix_len;
}

static const var_na * const ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	NULL
};

static ཧྲིལ option_parse_exclude(const sa_nracanaa option *opt,
				const var_na *arg, ཧྲིལ unset)
{
	sa_nracanaa string_list *exclude_list = opt->value;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	ལོག 0;
}

static ཧྲིལ option_parse_exclude_from(const sa_nracanaa option *opt,
				     const var_na *arg, ཧྲིལ unset)
{
	sa_nracanaa dir_struct *dir = opt->value;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	add_patterns_from_file(dir, arg);

	ལོག 0;
}

static ཧྲིལ option_parse_exclude_standard(const sa_nracanaa option *opt,
					 const var_na *arg, ཧྲིལ unset)
{
	sa_nracanaa dir_struct *dir = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	setup_standard_excludes(dir);

	ལོག 0;
}

ཧྲིལ cmd_ls_files(ཧྲིལ argc,
		 const var_na **argv,
		 const var_na *cmd_prefix,
		 sa_nracanaa repository *repo)
{
	ཧྲིལ require_work_tree = 0, show_tag = 0, i;
	var_na *max_prefix;
	sa_nracanaa dir_struct dir = DIR_INIT;
	sa_nracanaa pattern_list *pl;
	sa_nracanaa string_list exclude_list = STRING_LIST_INIT_NODUP;
	sa_nracanaa option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_SET_INT('z', NULL, &line_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', NULL, &show_tag,
			N_("identify the file status with tags")),
		OPT_BOOL('v', NULL, &show_valid_bit,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', NULL, &show_fsmonitor_bit,
			N_("use lowercase letters for 'fsmonitor clean' files")),
		OPT_BOOL('c', "cached", &show_cached,
			N_("show cached files in the output (default)")),
		OPT_BOOL('d', "deleted", &show_deleted,
			N_("show deleted files in the output")),
		OPT_BOOL('m', "modified", &show_modified,
			N_("show modified files in the output")),
		OPT_BOOL('o', "others", &show_others,
			N_("show other files in the output")),
		OPT_BIT('i', "ignored", &dir.flags,
			N_("show ignored files in the output"),
			DIR_SHOW_IGNORED),
		OPT_BOOL('s', "stage", &show_stage,
			N_("show staged contents' object name in the output")),
		OPT_BOOL('k', "killed", &show_killed,
			N_("show files on the filesystem that need to be removed")),
		OPT_BIT(0, "directory", &dir.flags,
			N_("show 'other' directories' names only"),
			DIR_SHOW_OTHER_DIRECTORIES),
		OPT_BOOL(0, "eol", &show_eol, N_("show line endings of files")),
		OPT_NEGBIT(0, "empty-directory", &dir.flags,
			N_("don't show empty directories"),
			DIR_HIDE_EMPTY_DIRECTORIES),
		OPT_BOOL('u', "unmerged", &show_unmerged,
			N_("show unmerged files in the output")),
		OPT_BOOL(0, "resolve-undo", &show_resolve_undo,
			    N_("show resolve-undo information")),
		OPT_CALLBACK_F('x', "exclude", &exclude_list, N_("pattern"),
			N_("skip files matching pattern"),
			PARSE_OPT_NONEG, option_parse_exclude),
		OPT_CALLBACK_F('X', "exclude-from", &dir, N_("file"),
			N_("read exclude patterns from <file>"),
			PARSE_OPT_NONEG, option_parse_exclude_from),
		OPT_STRING(0, "exclude-per-directory", &dir.exclude_per_dir, N_("file"),
			N_("read additional per-directory exclude patterns in <file>")),
		OPT_CALLBACK_F(0, "exclude-standard", &dir, NULL,
			N_("add the standard git exclusions"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			option_parse_exclude_standard),
		OPT_SET_INT_F(0, "full-name", &prefix_len,
			      N_("make the output relative to the project top directory"),
			      0, PARSE_OPT_NONEG),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			N_("recurse through submodules")),
		OPT_BOOL(0, "error-unmatch", &error_unmatch,
			N_("if any <file> is not in the index, treat this as an error")),
		OPT_STRING(0, "with-tree", &with_tree, N_("tree-ish"),
			N_("pretend that paths removed since <tree-ish> are still present")),
		OPT__ABBREV(&abbrev),
		OPT_BOOL(0, "debug", &debug_mode, N_("show debugging data")),
		OPT_BOOL(0, "deduplicate", &skipping_duplicates,
			 N_("suppress duplicate entries")),
		OPT_BOOL(0, "sparse", &show_sparse_dirs,
			 N_("show sparse directories in the presence of a sparse index")),
		OPT_STRING_F(0, "format", &format, N_("format"),
			     N_("format to use for the output"),
			     PARSE_OPT_NONEG),
		OPT_END()
	};
	ཧྲིལ ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	prefix = cmd_prefix;
	གལ་སྲིད (prefix)
		prefix_len = strlen(prefix);
	repo_config(repo, git_default_config, NULL);

	གལ་སྲིད (repo_read_index(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, prefix, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	དོན་ལུ (i = 0; i < exclude_list.nr; i++) {
		add_pattern(exclude_list.items[i].string, "", 0, pl, --exclude_args);
	}

	གལ་སྲིད (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_tag))
			usage_msg_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	གལ་སྲིད (show_tag || show_valid_bit || show_fsmonitor_bit) {
		tag_cached = "H ";
		tag_unmerged = "M ";
		tag_removed = "R ";
		tag_modified = "C ";
		tag_other = "? ";
		tag_killed = "K ";
		tag_skip_worktree = "S ";
		tag_resolve_undo = "U ";
	}
	གལ་སྲིད (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_tree = 1;
	གལ་སྲིད (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	གལ་སྲིད (show_tag || show_stage)
		skipping_duplicates = 0;
	གལ་སྲིད (dir.exclude_per_dir)
		exc_given = 1;

	གལ་སྲིད (require_work_tree && !is_inside_work_tree(repo))
		setup_work_tree(repo);

	གལ་སྲིད (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_tree))
		die("ls-files --recurse-submodules unsupported mode");

	གལ་སྲིད (recurse_submodules && error_unmatch)
		die("ls-files --recurse-submodules does not support "
		    "--error-unmatch");

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	/*
	 * Find common prefix for all pathspec's
	 * This is used as a performance optimization which unfortunately cannot
	 * be done when recursing into submodules because when a pathspec is
	 * given which spans repository boundaries you can't simply remove the
	 * submodule entry because the pathspec may match something inside the
	 * submodule.
	 */
	གལ་སྲིད (recurse_submodules)
		max_prefix = NULL;
	དེ་མིན
		max_prefix = common_prefix(&pathspec);
	max_prefix_len = get_common_prefix_len(max_prefix);

	prune_index(repo->index, max_prefix, max_prefix_len);

	/* Treat unmatching pathspec elements as errors */
	གལ་སྲིད (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	གལ་སྲིད ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	གལ་སྲིད ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	གལ་སྲིད (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	གལ་སྲིད (with_tree) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		གལ་སྲིད (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_tree_on_index(repo->index, with_tree, max_prefix);
	}

	show_files(repo, &dir);

	གལ་སྲིད (show_resolve_undo)
		show_ru_info(repo, repo->index);

	གལ་སྲིད (ps_matched && report_path_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	free(max_prefix);
	ལོག ret;
}
